// legal stuff:
/*
************************************************************************************
* Program for simultaneous AM modulation of 8 audiostreams taken from the internet *
* Copyright (C) 2020 Eduard Hontele                                                *
*                                                                                  *
* This program is free software; you can redistribute it and/or                    *
* modify it under the terms of the GNU General Public License                      *
* as published by the Free Software Foundation; either version 2                   *
* of the License, or (at your option) any later version.                           *
*                                                                                  *
* This program is distributed in the hope that it will be useful,                  *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
* GNU General Public License for more details.                                     *
*                                                                                  *
* You should have received a copy of the GNU General Public License                *
* along with this program; if not, write to the Free Software                      *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  *
*                                                                                  *
************************************************************************************
*/

/*
***********************************************************************************************
* This program is used on a Raspberry Pi3b+ or a Rpi4.                                        *
* It is not a standalone program and it needs the proper modulation board.                    *
* The ultimate goal is to generate AM modulated carriers at frequencies in the broadcast band.*
* The system has been created for use by musea and radio collectors. It is not a transmitter. *
* A twisted line is connected to the output of the modulator. Each radio will be connected to *
* that line by means of a ferrite clamp. The line is not an antenna. No power is transmitted. *
* No Ham license is required                                                                  *
* The program is distributed on an SD card.                                                   *
* On this card there is also the source and the object file containing the FPGA configuration *
* Please have a look at the files on the SD card.                                             *
* A copy of the GPLv2 licence is there.                                                       *
* The proram is autostarting at powerup. It takes a minute or two to start.                   *
* On the SD card You'll find a file "frequencies.txt". This file is used to link 8 URL's to   *
*   8 frequencies.                                                                            *
***********************************************************************************************
*/

#include "definitions.h"

#include <fcntl.h>
#include <stdint.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <linux/types.h>
#include <errno.h>
#include <termios.h> // Contains POSIX terminal control definitions
#include <dirent.h>
#include <stdio.h>

int fd[MAX_FREQ]; // file descriptor fifo

struct row {
  int type;
  int freq;
  char url[URL_LEN];
  char nom[LEN];
  char pays[LEN]; // code pays (FR/BE/LX/CH/DE/...)
  int conf;
  int boucle; // jouer la source en boucle : oui/non
};

struct row tabel[MAX_FREQ+1]; // tabel[0] contient le nom de la liste

//***************************************************************************//
void init(int i) {
  char str[STR_LEN];

  sprintf(str,"mkfifo -m 0777 /var/tmp/mpd.fifo%02d",i);
  printf("%s\n",str);
  system(str);

  sprintf(str,"mpd %s/ini/mpd%02d.conf",HOME_DIR,tabel[i].conf);

  printf("%s\n",str);
  system(str);

  sprintf(str,"mpc -p 66%02d clear",i);
  printf("%s\n",str);
  system(str);

  if (tabel[i].boucle) {
        sprintf (str,"mpc -p 66%02d update",i);
        system(str);
        }

  sprintf(str,"mpc -p 66%02d add %s",i,tabel[i].url);
  printf("%s\n",str);
  system(str);

  if (tabel[i].boucle) {
        sprintf(str,"mpc -p 66%02d repeat on",i);
        system(str);
        }

  sprintf(str,"mpc -p 66%02d play 1",i);
  printf("%s\n",str);
  system(str);

  sprintf(str,"/var/tmp/mpd.fifo%02d",i);
  printf("%s\n",str);

  fd[i] = open(str, O_RDONLY | O_NONBLOCK);
  if(!fd[i]) printf("Erreur ouverture Fifo %i\n",i);
}
//***************************************************************************//

//***************************************************************************//
int ZoekFreqHttp(char *line, int *type_source,int *freq, char *url, char *nom, int *idx) {

char *virgule;
char poubelle[LEN];

sscanf(strtok_r(line,",",&virgule),"%d",freq);
strcpy(nom,strtok_r(NULL,",",&virgule));
strcpy(poubelle,strtok_r(NULL,",",&virgule)); // Le code pays n'a pas d'importance ici
sscanf(strtok_r(NULL,",",&virgule),"%d",type_source);
strcpy(url,strtok_r(NULL,",",&virgule));
if (strstr(url,"http")) { *idx = IDX_URL ;} else { *idx = IDX_LOCAL;}
  return(1);
}
//***************************************************************************//

//***************************************************************************//
int LeesFile(void)
{
    FILE * fp;
    char * line;
    char *url_st;
    char *nom_st;
    char *nom_frq;
    size_t len = 0;
    ssize_t read;
    int cnt=0;
    int freq_st;
    int type_st;
    int idx_conf;

    line = (char *)malloc(STR_LEN * sizeof(char));
    url_st = (char *)malloc(URL_LEN * sizeof(char));
    nom_st = (char *)malloc(LEN * sizeof(char));
    nom_frq = (char *)malloc(LEN * sizeof(char));

    sprintf (nom_frq,"%s/listes/frequences_op_rfl.txt",HOME_DIR);
    fp = fopen(nom_frq, "r");

    if (!fp) return(-1);

    for (int i=0; i<=MAX_FREQ; i++) {
        tabel[i].freq =
                tabel[i].conf =
        tabel[i].boucle = 0;
       }

   while ((read = getline(&line, &len, fp)) != -1) {
                if(ZoekFreqHttp(line,&type_st,&freq_st,url_st,nom_st,&idx_conf)) {
                        tabel[cnt].type = type_st;
                        tabel[cnt].freq = freq_st;
                        strcpy(tabel[cnt].url, url_st);
                        strcpy(tabel[cnt].nom, nom_st);
                        tabel[cnt].conf = idx_conf+cnt; // pour mpd0x (URL) ou mpd1x (local)
                        tabel[cnt].boucle = idx_conf; // Les sources locales sont lues en boucle
printf ("freq %i - nom %s - url %s - conf %i\n",tabel[cnt].freq,tabel[cnt].nom,tabel[cnt].url,tabel[cnt].conf);
                        cnt++;
                        }
      } // end while
    fclose(fp);
    return(cnt-1);
}
//***************************************************************************//

//***************************************************************************//
// Fonction check_serial
// Vérifie la présence d'un unique port série dans /dev/serial
// Entree(s): char *dev_name nom du port série si trouvé
// Sortie(s) : int -1 si erreur (pas de port ou plusieurs ports), 1 sinon
//***************************************************************************//
int check_serial(char *dev_name)
{
	struct dirent *entry;         
	struct stat s;
	DIR *dev_serial;
	const char* folder;
	folder = "/dev/serial/by-id";
	int count = 0;
	
	// Si pas de dossier /dev/serial/by-id alors pas de port série
	if (stat(folder, &s) != 0 && !S_ISDIR(s.st_mode))
	{
		return -1;
    } 
	
	//Le dossier /dev/serial/by-id existe
	dev_serial = opendir(folder);

	// Balayage du dossier /dev/serial/by-id avec filtre sur les éléments possédant un lien symbolique
	while ((entry = readdir(dev_serial)) != NULL)
	{
        if(entry->d_type == DT_LNK)
		{
			sprintf(dev_name, "/dev/serial/by-id/%s", entry->d_name);
			count++;
		}
    }
    closedir(dev_serial);
	
	// Il y a plus d'un élément
	if (count > 1)
	{
		return -1;
	}
	
	// On a trouvé un seul port série
	return 1;
}
//***************************************************************************//


int main() {

 char buf0[LEN];
 char buf[MAX_FREQ][LEN];
 uint8_t pnt[MAX_FREQ];
 uint16_t b1;
 uint8_t bb1;

 int32_t b321;

 int nbr_stations, LM1,i,ii,r;
 struct timeval tnu;
 long sc[MAX_FREQ];

 FILE *fp_relance;
 FILE *fp_etat; 
 int num_relance, ls_status;
 char *relance, *rm_diffusion, *ordre_gpio_on, *port_name;

 relance = (char *)malloc(LEN * sizeof(char));
 rm_diffusion = (char *)malloc(LEN * sizeof(char));
 ordre_gpio_on = (char *)malloc(LEN * sizeof(char));
 port_name = (char *)malloc(LEN * sizeof(char));
 
 sprintf (relance, "%s/etat/relance",HOME_DIR);
 sprintf (rm_diffusion, "rm %s/etat/relance ; rm %s/etat/diffuser",HOME_DIR,HOME_DIR);
 sprintf (ordre_gpio_on,"gpio write %i 1",PORT_LED_DIFF);

 LM1 = LEN -1;

 
 if (check_serial(port_name) < 0)
 {
	 printf ("AM8 absent, on stoppe"); 
	 return(-1);
 } // sans platine = arrêter
 
 int serial_port = open(port_name, O_RDWR);

  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;


  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most comm>
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handlin>
  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data i>
  tty.c_cc[VMIN] = 0;


   // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  }


 sleep(5);
 printf("load AM frequencies \n");
 nbr_stations = LeesFile(); // recuperation des frequences et des URLs
 printf("nbr_stations: %d \n",nbr_stations);

 if (nbr_stations < 0) {
     printf("Fichier des frequences introuvable\n Fin\n\n ");
     return(1);
     }
     else {
     printf("frequences_op.txt: %d lignes lues \n",nbr_stations);
     }; // endif

 for(i=1; i<=nbr_stations; i++) {
    init(i); 
    }

 sleep(5);
 printf("start MG \n");


// On lance la diffusion

 for (i=0; i<LEN; i++) buf0[i] = 0;

 for (r=0; r<nbr_stations; r++) {
     for(int k=0; k<LEN; k++) buf[r][k] = 0;
     buf[r][0] = 31;
     buf[r][1] = 97;
     buf[r][2] = 52;
     buf[r][3] = 73;
     buf[r][4] = r+1;
     }

 buf0[0] = 31;
 buf0[1] = 97;
 buf0[2] = 52;
 buf0[3] = 73;
 buf0[4] = 99;
 buf0[5] = nbr_stations;

 for(i=1; i<=nbr_stations; i++) {
   buf0[2*i +4] = tabel[i].freq >> 8;
   buf0[2*i +5] = tabel[i].freq & 255;
   };
 write(serial_port,buf0,LEN);


 for (i=0; i<nbr_stations; i++) pnt[i] = 5;

 gettimeofday(&tnu, NULL);
 for(i=0; i<nbr_stations; i++) sc[i] = tnu.tv_sec;

 printf("boucle principale\n");

// On allume le témoin de diffusion

system (ordre_gpio_on);

 while(1){
   
   for(ii=0; ii<nbr_stations; ii++){
   
		 i = ii & 7;
         gettimeofday(&tnu, NULL);
         
		 if(read(fd[i+1], &b1, 2)==2){

         //  bb1 = (b1 + 32767)>>8;

// Eduard Hontele - 20dec2023 : amelioration du taux de modulation
// La valeur de 1.2 a été déterminée par tests en laboratoire (DWK).

           b321 = (((b1 << 16) >>16) *1229)>>10; // *1.2 = *1229/1024 
           b321 = (b321 + 32768) >> 8; 

// fin de la partie amelioration du taux de modulation

           if(b321 > 255) bb1=255;
           else if(b321 < 0) bb1 = 0;
                else bb1 = b321; 


           buf[i][pnt[i]]=bb1;
           if(pnt[i]==LM1){write(serial_port,&buf[i][0],LEN); pnt[i]=4;};
           pnt[i]++;
           sc[i] = tnu.tv_sec;
           } else {
/*
** 27-NOV-23 : 
** On ne scrute pas les coupures de flux pour les sources EXT (type = 3)
*/
           if((tabel[ii+1].type != 3) && (tnu.tv_sec - sc[i])> MAX_COUPURE)
                          {
/* 
** Nous avons détecté une coupure de réception d'un flux internet 
** Nous avons le droit de relancer MAX_RELANCE fois de suite.
** Suite à une MAX_RELANCE relance, nous stoppons la diffusion pour que
** l'utilisateur puisse reprendre la main via l'IHM
*/
                          system (rm_diffusion);
						  printf("Coupure detectee sur flux %d \n",i);
						  sleep(10);
						  // fp_relance = fopen (relance,"r+");
                          // if (!fp_relance) { /* premiere relance */
                                           // fp_relance = fopen (relance,"w");
                                           // fprintf (fp_relance,"%i",1);
                                           // fclose (fp_relance);
                                           // system("reboot");
                                           // } 
                          // else { /* ce n'est pas la premiere relance */
                                // fscanf (fp_relance,"%i",&num_relance);
                                // if (num_relance < MAX_RELANCE) 
                                    // {
                                    // rewind (fp_relance);
                                    // fprintf (fp_relance,"%i",++num_relance);
                                    // fclose (fp_relance);
                                    // system("reboot"); 
                                    // }
                                // else { /* MAX_RELANCE sont faites, on stoppe la diffusion, on ne relance pas */
                                     // fclose (fp_relance);
                                     // system (rm_diffusion);
                                     // /* donnons un peu de temps a lance_am8 pour comprendre qu'il faut stopper la diff */
									// sleep(10);
                                     // }
                                   
                                // } 
                          // } /* fin du traitement des coupures de flux audio */

           }; // end if
		}
     } // end for

   }; // end while

 close(serial_port);
 return(0);
} 


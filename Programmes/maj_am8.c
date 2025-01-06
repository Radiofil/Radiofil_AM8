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
***********************************************************************************************
*/

#include "definitions.h"

int main () {
 
FILE *fp_dir_syst;
FILE *fp_fic_url;
FILE *fp_version_locale;
FILE *fp_version_radiofil;

char *version_locale, *version_radiofil,*commande,*fichier_log, 
     *fichier_url, *url_serveur, *fichier_version, *fichier_archive,
      *numero_version_locale, *numero_version_radiofil, *archive_locale, *dir_syst;
int retour, maj_a_faire = 0;

fichier_log = (char *)malloc(LEN * sizeof(char));
fichier_url = (char *)malloc(LEN * sizeof(char));
url_serveur = (char *)malloc(URL_LEN * sizeof(char));
fichier_version = (char *)malloc(URL_LEN * sizeof(char));
fichier_archive = (char *)malloc(URL_LEN * sizeof(char));
commande = (char *)malloc(STR_LEN * sizeof(char));
version_locale = (char *)malloc(LEN * sizeof(char));
version_radiofil = (char *)malloc(LEN * sizeof(char));
numero_version_radiofil = (char *)malloc(LEN * sizeof(char));
numero_version_locale = (char *)malloc(LEN * sizeof(char));
archive_locale = (char *)malloc(LEN * sizeof(char));
dir_syst = (char *)malloc(LEN * sizeof(char));

// Verification que le r√©pertoire SYST existe
sprintf (dir_syst, "%s/%s",HOME_DIR,SYST_DIR);
fp_dir_syst = fopen (dir_syst,"r");

if (!fp_dir_syst) { 
			  sprintf (commande,"mkdir %s; chmod a+w %s",dir_syst,dir_syst);
			  system (commande);
			  }

sprintf (fichier_log, "%s/wget.log",dir_syst);
sprintf (version_locale, "%s/version_locale.txt",dir_syst);
sprintf (version_radiofil, "%s/version_radiofil.txt",dir_syst);
sprintf (archive_locale, "%s/projet_am8.tar",dir_syst);
sprintf (fichier_url,"%s/url_serveur.txt",dir_syst);

// RÈcupÈration de l'adresse du serveur

fp_fic_url = fopen (fichier_url,"r");

if (!fp_fic_url) { // fichier inexistant, inutile de continuer
printf ("<center>Adresse du serveur inconnue.</center><br>");
}

else { // nous avons bien une URL

// RÈcuperation de l'URL

fgets (url_serveur,URL_LEN,fp_fic_url);
fclose (fp_fic_url);

if (url_serveur[(int)strlen(url_serveur)-1]=='\n') {url_serveur[(int)strlen(url_serveur)-1]=0;};

sprintf (fichier_version,"%s/version.txt",url_serveur);
sprintf (fichier_archive,"%s/%s",url_serveur,NOM_FIC_ARCHIVE);

// R√©cup√©ration du fichier de version de r√©f√©rence
// -4 car IPv4

sprintf (commande,"/usr/bin/wget -4 -O %s %s -o%s",version_radiofil,fichier_version,fichier_log);
retour = system (commande);

if (retour) { // URL non valide 

            printf ("<center>URL non valide, veuillez la corriger.</center><br>");
            }
else { 
     fp_version_radiofil = fopen(version_radiofil,"r");

if (!fp_version_radiofil) { // soucis !  inutile de continuer... 
printf ("<center>Connexion avec le serveur Radiofil impossible, veuillez recommencer plus tard.</center><br>");
                          }

else { // nous avons bien un fichier de r√©f√©rence

     fgets (numero_version_radiofil,LEN,fp_version_radiofil);
     fclose (fp_version_radiofil);

     if (numero_version_radiofil[(int)strlen(numero_version_radiofil)-1]=='\n') 
        {numero_version_radiofil[(int)strlen(numero_version_radiofil)-1]=0;};

     fp_version_locale = fopen(version_locale, "r");

     if (fp_version_locale) { // Lecture de la version installÈe en local
                            fgets (numero_version_locale,LEN,fp_version_locale);
			    fclose (fp_version_locale);

                            if (numero_version_locale[(int)strlen(numero_version_locale)-1]=='\n') 
                               {numero_version_locale[(int)strlen(numero_version_locale)-1]=0;};

			    // Si les deux num√©ros de version sont dif√rents, une mise √ jour s'impose

			    if (strcmp(numero_version_radiofil,numero_version_locale)) {maj_a_faire = 1;}

       		            }

     else { // Fichier de version inexistant. On force la mise ‡ jour.
          maj_a_faire = 1;
          }


    printf ("Version radiofil : %s *-* Version locale : %s\n<br>",numero_version_radiofil,numero_version_locale);
    if (maj_a_faire) { // Nous avons une mise √† jour √† faire 
printf ("T√©l√©chargement de la nouvelle version...<br>");
                     // R√©cup√©ration du fichier archive depuis le serveur
                     // -4 car IPv4
		     sprintf (commande,"/usr/bin/wget -4 -O %s %s -o%s",archive_locale,fichier_archive,fichier_log);
                     system (commande);
printf ("Installation de la nouvelle version...<br>");
                     // Nous avons la nouvelle archive, il faut l'installer
// 1- on desarchive am8.tar et IHM.tar + rÈcupÈration du script de mise ‡ jour
                     sprintf (commande,"cd %s;/usr/bin/tar xf %s",dir_syst,archive_locale);
                     system (commande);
// 2- execution du script de mise ‡ jour
// Executer le script donne plus de latitude sur la gestion des mises ‡ jour (le script pouvant lui-mÍme Ítre mis ‡ jour)
                     sprintf (commande,"%s/maj_am8.sh;",dir_syst);
                     system (commande);
printf ("Mise √† jour effectu√©e !<br>");
                     } // If (maj_a_faire)
    else { printf ("<center>Vous disposez d√©j√† de la derni√®re version !</center>");}

    } // if URL valide
    } // if fichier de reference trouve
    } // if fichier URL trouve
} // Main

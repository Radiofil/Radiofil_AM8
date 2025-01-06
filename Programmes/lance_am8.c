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
 
FILE *fp_etat;

char *diffuser, *eteindre, *lancer_am8, *stopper_am8, *eteindre_am8, *ordre_gpio, *bouton_poussoir;

int diffusion_en_cours = 0;
int etat_bp = 1;

diffuser = (char *)malloc(LEN * sizeof(char));
eteindre = (char *)malloc(LEN * sizeof(char));
lancer_am8 = (char *)malloc(LEN * sizeof(char));
stopper_am8 = (char *)malloc(LEN * sizeof(char));
eteindre_am8 = (char *)malloc(LEN * sizeof(char));
bouton_poussoir = (char *)malloc(LEN * sizeof(char));
ordre_gpio = (char *)malloc(LEN * 2 * sizeof(char));

sprintf (bouton_poussoir, "/sys/class/gpio/gpio%i/value",GPIO_BP);
sprintf (diffuser, "%s/etat/diffuser",HOME_DIR);
sprintf (eteindre, "%s/etat/eteindre",HOME_DIR);
sprintf (lancer_am8, "%s/bin/am031 &",HOME_DIR);
sprintf (stopper_am8, "%s/scripts/stopper_am8.sh &",HOME_DIR);
sprintf (eteindre_am8, "%s/scripts/eteindre_am8.sh &",HOME_DIR);

/*
** Initialisation des ports IO :
** LED_ON : allumée dès le début
** LED_DIFF : éteinte
** RAZ_PICO : port ouvert en sortie
*/

sprintf (ordre_gpio, "gpio mode %i out; gpio write %i 1; gpio mode %i out; gpio write %i 0; gpio mode %i out; gpio write %i 0",PORT_LED_ON,PORT_LED_ON, PORT_LED_DIFF, PORT_LED_DIFF, PORT_RAZ, PORT_RAZ);

system (ordre_gpio);

/*
** Bouton Poussoir : port ouvert en entrée, positionné à 1
** PORT_BP est ensuite mis en export pour un accès via la classe /sys/class/gpio (ici gpio71)
*/

sprintf (ordre_gpio, "gpio mode %i in; gpio mode %i up; gpio export %i in",PORT_BP, PORT_BP, GPIO_BP);

system (ordre_gpio);

/*
** Lancement de la diffusion par defaut
*/

fp_etat = fopen (diffuser,"w");
fclose (fp_etat);

printf ("lance_am8 entre en boucle\n");
// Boucle principale

while (1) { // boucle sans fin

// Est-ce que le bouton poussoir est enfoncé ?

fp_etat = fopen (bouton_poussoir,"r");
fscanf (fp_etat,"%i",&etat_bp);
fclose (fp_etat);

if (!etat_bp) { // le bouton poussoir a été presse (mis à zéro)

              sleep (DELAI_BP);
              fp_etat = fopen (bouton_poussoir,"r");
              fscanf (fp_etat,"%i",&etat_bp);
              fclose (fp_etat);

               if (!etat_bp) { // cela dure plus de 5 secondes : on doit éteindre
		             system (eteindre_am8);
                             }
               }

 fp_etat = fopen(diffuser, "r");

 if (fp_etat) { // il faut lancer l'AM8 si pas déja fait
	         if (!diffusion_en_cours)
		    { 
	            diffusion_en_cours = 1;
                    system (lancer_am8);
		    }
		 fclose (fp_etat);
                 }

 else { fp_etat = fopen(eteindre, "r");
        if (fp_etat) { // il faut stopper l'AM8
		system (eteindre_am8);
                 }

	else { // simple arret de la diffusion sans arreter l'AM8 si pas deja fait
             if (diffusion_en_cours)
                {
	        diffusion_en_cours = 0;
	        system (stopper_am8);
	        }
	     }
      }

sleep (5);

} // Fin de la boucle sans fin

} // Main

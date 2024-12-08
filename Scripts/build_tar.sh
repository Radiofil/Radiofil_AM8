#!/bin/bash
cd /var/www
tar cvzf /home/orangepi/IHM.tar ./html
cd /home/orangepi
tar cvzf ./am8.tar ./amt/bin ./amt/ini ./amt/etat ./amt/listes/*_rfl* ./amt/media ./amt/scripts 
tar cvzf ./projet_am8.tar ./am8.tar ./IHM.tar ./maj_am8.sh ./version_locale.txt  ./url_serveur.txt

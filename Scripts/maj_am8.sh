#!/bin/bash

# Stopper une eventuelle diffusion en cours et "tuer" le programme "lance_am8" avant sa mise à jour éventuelle
cd /home/orangepi/amt/scripts/
source stopper_am8.sh
ps -ef | grep lance_am8 | grep -v grep | awk '{print $2}' | xargs kill -9

# Decompression de l'archive de mise à jour dans les dossier ad hoc
cd /home/orangepi
tar xf /home/orangepi/amt/syst/am8.tar --same-owner
cd /var/www
tar xf /home/orangepi/amt/syst/IHM.tar --same-owner

# Suppression de l'archive de mise à jour et mise à jour du fichier de version locale
cd /home/orangepi/amt/syst
cp version_radiofil.txt version_locale.txt
rm *.tar version_radiofil.txt

# Executer le programme "lance_am8" a nouveau
/home/orangepi/amt/bin/lance_am8 &



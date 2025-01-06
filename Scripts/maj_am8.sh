#!/bin/bash
cd /home/orangepi
tar xf /home/orangepi/amt/syst/am8.tar --same-owner
cd /var/www
tar xf /home/orangepi/amt/syst/IHM.tar --same-owner
cd /home/orangepi/amt/syst
rm *.tar version_radiofil.txt

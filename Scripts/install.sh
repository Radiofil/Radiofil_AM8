#!/bin/bash

echo == Copie des fichiers utiles ==
cp hostname /etc/hostname
cp mpd.conf /etc/mpd.conf
cp rc.local /etc/rc.local
cp am8.tar /home/orangepi 
cp IHM.tar /home/orangepi
cp url_serveur.txt /home/orangepi
cp version_locale.txt /home/orangepi

echo == mise a jour systeme ==
apt-get update
apt-get upgrade -y

echo == installation web ==
apt-get install apache2 php libapache2-mod-php -y
apt-get install avahi-daemon

echo == installation mpd ==
apt-get install mpc mpd -y

echo == install WiringOP ==
apt-get install git -y
cd /home/orangepi
git clone https://github.com/orangepi-xunlong/wiringOP
cd /home/orangepi/wiringOP
./build clean
./build 
cd /home/orangepi
rm -R ./wiringOP

echo == Decompression des logiciels et droits des fichiers ==
tar xf am8.tar --same-owner
source ./amt/scripts/compilation.sh
chown orangepi.orangepi ./amt
chown orangepi.orangepi ./amt/listes
chmod a+rwx ./amt/listes
mkdir ./amt/syst
chown orangepi.orangepi ./amt/syst
chmod a+rwx ./amt/syst
cp url_serveur.txt ./amt/syst
cp version_locale.txt ./amt/syst
chown www-data.www-data ./amt/syst/*.txt
chmod a+rw-x ./amt/syst/*.txt
cd /var/www
tar xf /home/orangepi/IHM.tar --same-owner
rm /var/www/html/index.html

echo == Installation terminée ==
poweroff

#!/bin/bash
echo == mise a jour systeme ==
apt-get update
apt-get upgrade -y
echo == installation web ==
apt-get install apache2 php libapache2-mod-php -y
echo == installation mpd ==
apt-get install mpc mpd -y
cp ./cle_usb/hostname /etc/hostname
cp ./cle_usb/mpd.conf /etc/mpd.conf
cp ./cle_usb/rc.local /etc/rc.local
echo == install WiringOP ==
apt-get install git -y
cd /home/orangepi
git clone https://github.com/orangepi-xunlong/wiringOP
cd /home/orangepi/wiringOP
./build clean
./build 
cd /home/orangepi
rm -R ./wiringOP
tar xf /home/orangepi/cle_usb/am8.tar --same-owner
chown orangepi.orangepi ./amt
chown orangepi.orangepi ./amt/listes
chmod a+rwx ./amt/listes
mkdir ./amt/syst
chown orangepi.orangepi ./amt/syst
chmod a+rwx ./amt/syst
cp ./cle_usb/url_serveur.txt ./amt/syst
cp ./cle_usb/version_locale.txt ./amt/syst
chown www-data.www-data ./amt/syst/*.txt
chmod a+rw-x ./amt/syst/*.txt
cd /var/www
tar xf /home/orangepi/cle_usb/IHM.tar --same-owner
rm /var/www/html/index.html
echo == maj adresse IP ==
nmcli con add con-name eth_am8 ifname eth0 type ethernet ip4 192.168.1.88/24 gw4 192.168.1.1 ipv4.dns "192.168.1.1 8.8.8.8"
nmcli con delete "Wired connection 1"
echo == Installation terminée ==
poweroff

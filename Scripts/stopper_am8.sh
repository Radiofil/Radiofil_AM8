#!/bin/bash
rm /home/orangepi/amt/etat/relance
ps -ef | grep mpd | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep am031 | grep -v grep | awk '{print $2}' | xargs kill -9
gpio write 10 0
gpio write 16 1; sleep 0.1 ;gpio write 16 0 

<?php
$fichier_frq_op="frequences_op_rfl.txt";
$fichier_plan_frq="plan_freqs_rfl.txt";
$fichier_stations="liste_stations.txt";
$fichier_stations_perso="liste_stations_perso.txt";
$fichier_stations_rfl="liste_stations_rfl.txt";
$fichier_version_locale="version_locale.txt";
$fichier_wifi="reseau_wifi.txt";

$home_dir="/home/orangepi/amt/";
//$home_dir="./";
$dir_media= $home_dir."media/";
$dir_listes= $home_dir."listes/";
$dir_etat= $home_dir."etat/";
$dir_syst= $home_dir."syst/";

$allumer = $dir_etat."allumer";
$diffuser = $dir_etat."diffuser";
$eteindre = $dir_etat."eteindre";

$allumage='sudo '.$home_dir.'bin/am031 &';
$extinction='sudo '.$home_dir.'stop.sh';
?>

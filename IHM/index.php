<?php 
require_once "inc/fonctions.php"; 

$allume = lire_variable('allume');
$diffuse = lire_variable('diffuse');

//
// Si on lance l'IHM alors que l'am8 diffuse déjà, il faut en tenir compte
// (allume = 1 et diffuse = 1) 
//


//if (($allume <0) && (realpath($allumer))) $allume = 1;
if ($allume <0) $allume = 1;
if ($allume && ($diffuse <0) && (realpath($diffuser))) $diffuse = 1 ; 
if ($diffuse <0) $diffuse = 0;

// les autres variables ne sont pas dépendantes d'un état antérieur

$source = lire_variable('source');
$conf = lire_variable('conf');
$sel = lire_variable('sel');
$syst = lire_variable('syst');

if ($source <0) $source = 0;
if ($conf <0) $conf = 0;
if ($sel <0) $sel = 0;
if ($syst <0) $syst = 0;

?>

<!DOCTYPE html>
<html>
<head lang="fr">
  <meta charset="UTF-8">
  <title>Modulateur AM RadioFil</title>
  <meta name="author" content="radiofil.org"/>
 <meta name="robots" content="noindex, nofollow"/>
  <link href="img/favicon.ico" sizes="16x16" rel="icon" type="image/ico"/>
  <link href="css/am8.css" rel="stylesheet" type="text/css"/>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js"></script>
</script>
<?php include "inc/style.php"; ?>

</head>
<body>
<main>

<div class="panneau_sup">

  <div class="panneau_gauche">
   <center>
    <a href="https://radiofil.org" target="_blank"><img src="img/logo_c_2022.png" height=30% width=70% alt="RadioFil"></a>
    <br><br><br><br><br><br>
    <?php
     if ($allume) {
	  	  if ($diffuse)  echo '<img src="img/oeil_ferme.jpg" alt="Oeil Magique" width=50% height=20%>';
                   	 else echo '<img src="img/oeil_ouvert.jpg" alt="Oeil Magique" width=50% height=20%>';
                  }
     else  echo '<img src="img/oeil_eteint.jpg" alt="Oeil Magique" width=50% height=20%>';

    echo '<br><br><br><br><br><br>';

     if ($allume && $diffuse) { echo '<img src="img/on_air_allume.jpg" alt="On Air" width=80% height=20%>'; }
	      else echo '<img src="img/on_air_eteint.jpg" alt="On Air" width=80% height=20%>';
     
     ?>
   </center>
  </div>

  <div>
   <center>
   <br>
    <img class="entete" src="img/radiofil-nom.png" alt="RadioFil" width=20% height=100>
    <h2>AM8 : modulateur huit fr&eacute;quences PO/GO</h2>
    <?php
    if ($allume) {
                 echo '<img src="img/Cadran_allume.jpg" alt="Cadran frq" width=90% height=300>';
                 }
    else {
         echo '<img src="img/Cadran_eteint.jpg" alt="Cadran frq" width=90% height=300>';
         }
    ?>
    <br>
    
    <div class="clavier">
    <div></div>
<div>
<?php

$commande = " ";

echo '<a href="index.php?allume=';

if ($allume) {echo '0">'; $commande = "rm ".$eteindre."; touch ".$allumer.";";} 
else {echo '1">'; $commande = "rm ".$allumer.";rm ".$diffuser.";touch ".$eteindre.";";};

echo '<img src="img/btn_arr.jpg" witdh=75% height=100 alt="ARR"></a>';

echo '<a href="index.php?allume='.$allume.'&diffuse=';

if ($diffuse) {echo '0">'; $commande .= "rm ".$eteindre.";touch ".$diffuser.";";} 
else {echo '1">'; $commande .= "rm ".$diffuser.";";};

exec ($commande);

echo '<img src="img/btn_diff.jpg" witdh=75% height=100 alt="DIFF"></a>';

echo '<a href="index.php?allume=',$allume,'&diffuse=',$diffuse,'&sel=1">';

echo '<img src="img/btn_sel.jpg" witdh=75% height=100 alt="SEL"></a>';

echo '<a href="index.php?allume=',$allume,'&diffuse=',$diffuse,'&source=1">';

echo '<img src="img/btn_sour.jpg" witdh=75% height=100 alt="SOURCE"></a>';

echo '<a href="index.php?allume=',$allume,'&diffuse=',$diffuse,'&conf=1">';

echo '<img src="img/btn_conf.jpg" witdh=75% height=100 alt="CONF"></a>';

echo '<a href="index.php?allume=',$allume,'&diffuse=',$diffuse,'&syst=1">';

echo '<img src="img/btn_syst.jpg" witdh=75% height=100 alt="SYST"></a>';

?>

</div>
<div></div>
</div>     
   </center>
</div>
<div>
        <br><br>        <br><br>
  <div class="panneau_liste_freq">

        <h3><center>Liste des stations actives</center></h3>
<?php
$nom_fichier = $dir_listes.$fichier_frq_op;
$lignes = file ($nom_fichier);
$nbr_lignes = 0;
$liste_freq = [];
$freq_actives = "<br>";
foreach ($lignes as $ligne) {
$champs= explode (",",$ligne);
$liste_freq[$champs[0]] =  $champs[1].','.$champs[2].','.$champs[3];
			$nbr_lignes++;
	    }
ksort($liste_freq);

foreach ($liste_freq as $freq_station => $desc_station)
{
$ligne_station = explode (",",$desc_station);
if ($ligne_station[1] == '0') { // nom de la liste 
                              $freq_actives .= '<center><b>'.$ligne_station[0].'</b></center>';
                              }
else if ($ligne_station[2] == '1') {
 		$freq_actives .= '<h4><img src="img/rouge.jpg">' . $freq_station . ' kHz - ' . $ligne_station[0].'</h4>';
     }
     else if ($ligne_station[2] == '2') {
          $freq_actives .= '<h4><img src="img/vert.jpg">' . $freq_station . ' kHz - ' . $ligne_station[0].'</h4>';
          }
          else $freq_actives .= '<h4><img src="img/jaune.jpg">' . $freq_station . ' kHz - ' . $ligne_station[0].'</h4>';
}
echo $freq_actives;
?>
<center><img src="img/rouge.jpg">Web <img src="img/vert.jpg">MP3 <img src="img/jaune.jpg">EXT</center>
   </div> 
 </div>  
<br>
</div>
<br>
<div class="panneau_inf">
<div></div>
<div>
<?php


if ($sel) {
               selection_diffusion($dir_listes,$dir_listes.$fichier_frq_op);
               
               }
else if ($source) {
     
     gestion_ajout_station($dir_listes.$fichier_stations_perso); // ajout dans la liste perso

                  }
else if ($conf) {
                //
                // on fusionne les listes de stations RFL et persos
                //
                $stations_rfl = file_get_contents ($dir_listes.$fichier_stations_rfl);
                $stations_perso = file_get_contents ($dir_listes.$fichier_stations_perso);
                file_put_contents ($dir_listes.$fichier_stations,$stations_rfl.$stations_perso);
                //
                // On peut maintenant appeler la fonction de creation de la nouvelle liste de diffusion
                //
                gestion_configurations_memorisees($dir_listes,$dir_listes.$fichier_plan_frq,$dir_listes.$fichier_stations);              
               }
else if ($syst) {
                $version_locale = file_get_contents ($dir_syst.$fichier_version_locale);
                gestion_parms_systeme($dir_syst.$fichier_wifi,$version_locale);
                }
else {
     if (!$diffuse) {
     echo '<b>Arr:</b> Arr&ecirc;t et extinction du modulateur<br>
     <b>Diff:</b> Activation de la diffusion<br>
     <b>Sel:</b> S&eacute;lection des &eacute;missions diffus&eacute;es par le modulateur<br>
     <b>Sour:</b> Gestion des sources sonores<br>
     <b>Conf:</b> Gestion des configurations pr&eacute;d&eacute;finies<br>
     <b>Syst:</b> Param&egrave;tres syst&egrave;me';
                    }
                    else echo 'Diffusion en cours.<br>';
     }
?>

</div>
<div></div>
</main>
</body>
</html>


<?php

require_once "define.php";

if (!function_exists('str_starts_with')) {
	  function str_starts_with($str, $start) {
		      return (@substr_compare($str, $start, 0, strlen($start))==0);
		        }
}


function lire_variable($ma_variable){
	$ma_valeur=-1;

	if (isset($_GET[$ma_variable])){
		$ma_valeur=$_GET[$ma_variable];
	}else{
		if (isset($_POST[$ma_variable])){
			$ma_valeur=$_POST[$ma_variable];
		}
	}	
	return $ma_valeur;
}

//
// Fonction Gestion_triplets_memorises
// Pour associer une fréquence à un flux et un nom de station
// Fréquences : liste déroulante
// Flux : liste déroulante
// Nom de la station : champ texte en saisie libre
//

function gestion_configurations_memorisees ($rep_listes,$fichier_frequences,$fichier_stations) {

$lignes_frq = file ($fichier_frequences);

$nbr_lignes_frq = 0;

foreach ($lignes_frq as $ligne) {
        $liste_frq[$nbr_lignes_frq++] =  $ligne;
        }
unset ($ligne);

$lignes_stn = file ($fichier_stations);

$liste_stn = [];
foreach ($lignes_stn as $ligne) {
        $champs = explode (",",$ligne);
        $liste_stn[$champs[0]] = $champs[1].','.$champs[2].','.$champs[3]; // Nom, source du flux
        }
unset ($ligne);

ksort($liste_stn);
$canal = 0;

echo '<br><form method="post" action=""><fieldset><center>
  <label for="nom_sta"><b>Nom de la liste :</b></label>
  <input type="text" id="nom_liste" placeholder="Saisir le nom" name="nom_liste" required>
  <br>';

do {

 echo '<br><b>Fr&eacute;quence ',$canal+1,' : </b>';
 
 // Variable qui ajoutera l'attribut selected de la liste déroulante
  $selected = '';
 
  // Parcours tableau des frequences
  echo '<select id="frequence',$canal,'" name="frequence',$canal,'">\n';
  for($i=0; $i<$nbr_lignes_frq; $i++)
  {
    // Le choix par défaut est le rang du numéro de canal
    if($i == $canal)
    {
      $selected = ' selected="selected"';
    }
    // Affichage de la ligne
    echo '<option value="', $liste_frq[$i] ,'"', $selected ,'>', $liste_frq[$i] ,'</option>\n';
    // Remise à zéro de $selected
    $selected='';
  }
  echo '</select> ';
  
  // Parcours tableau des stations
  echo '<select id="station',$canal,'" name="station',$canal,'">\n';
  echo '<option value="">', "Choisir une station",'</option>\n';
  
  foreach ($liste_stn as $Nom => $Adresse)
  {
    // Affichage de la ligne
    echo '<option value="', $Nom ,'">', $Nom ,'</option>\n';
  }
  echo '</select>';

  $canal++;
  
  } while ($canal < 8);
 
 echo '<br><br><input type="submit" value="Valider">';  
 echo '</center></fieldset></form><br>';

if (strcmp($_POST['station0'],"choisir une station")) {
  
// generer le fichier de configuration
// retrouver le type de flux et la source
// Formater la ligne resultat (freq,type,nom,source)
// Ecrire les lignes dans le fichier de configuration
// Dire a l'utilisateur que c'est sauvegarde

$contenu_liste = '0,'.$_POST['nom_liste'].',0,0,0'."\n";

// Pour chaque ligne de la liste, recherche des infos "type de flux" et "source"
// La recherche sur la clÃ© sera a optimiser pour stopper des que trouve

if ($_POST['station0']) {
                        sscanf ($_POST['frequence0'],"%ikHz%s",$freq,$poubelle);

// foreach beurk : a optimiser avec un while pas_trouve

			foreach ($liste_stn as $Nom => $Infos)
				{
				if (!strcmp($Nom,$_POST['station0'])) { $Infos_Stn = $Infos;}
				}
                        $contenu_liste .= $freq.','.$_POST['station0'].','.$Infos_Stn;
                        } // station0

if ($_POST['station1']) {
                        sscanf ($_POST['frequence1'],"%ikHz%s",$freq,$poubelle);

// foreach beurk : a optimiser avec un while pas_trouve

			foreach ($liste_stn as $Nom => $Infos)
				{
				if (!strcmp($Nom,$_POST['station1'])) { $Infos_Stn = $Infos;}
				}
                        $contenu_liste .= $freq.','.$_POST['station1'].','.$Infos_Stn;
                        } // station1

if ($_POST['station2']) {
                        sscanf ($_POST['frequence2'],"%ikHz%s",$freq,$poubelle);

// foreach beurk : a optimiser avec un while pas_trouve

			foreach ($liste_stn as $Nom => $Infos)
				{
				if (!strcmp($Nom,$_POST['station2'])) { $Infos_Stn = $Infos;}
				}
                        $contenu_liste .= $freq.','.$_POST['station2'].','.$Infos_Stn;
                        } // station2

if ($_POST['station3']) {
                        sscanf ($_POST['frequence3'],"%ikHz%s",$freq,$poubelle);

// foreach beurk : a optimiser avec un while pas_trouve

			foreach ($liste_stn as $Nom => $Infos)
				{
				if (!strcmp($Nom,$_POST['station3'])) { $Infos_Stn = $Infos;}
				}
                        $contenu_liste .= $freq.','.$_POST['station3'].','.$Infos_Stn;
                        } // station3

if ($_POST['station4']) {
                        sscanf ($_POST['frequence4'],"%ikHz%s",$freq,$poubelle);

// foreach beurk : a optimiser avec un while pas_trouve

			foreach ($liste_stn as $Nom => $Infos)
				{
				if (!strcmp($Nom,$_POST['station4'])) { $Infos_Stn = $Infos;}
				}
                        $contenu_liste .= $freq.','.$_POST['station4'].','.$Infos_Stn;
                        } // station4

if ($_POST['station5']) {
                        sscanf ($_POST['frequence5'],"%ikHz%s",$freq,$poubelle);

// foreach beurk : a optimiser avec un while pas_trouve

			foreach ($liste_stn as $Nom => $Infos)
				{
				if (!strcmp($Nom,$_POST['station5'])) { $Infos_Stn = $Infos;}
				}
                        $contenu_liste .= $freq.','.$_POST['station5'].','.$Infos_Stn;
                        } // station5

if ($_POST['station6']) {
                        sscanf ($_POST['frequence6'],"%ikHz%s",$freq,$poubelle);

// foreach beurk : a optimiser avec un while pas_trouve

			foreach ($liste_stn as $Nom => $Infos)
				{
				if (!strcmp($Nom,$_POST['station6'])) { $Infos_Stn = $Infos;}
				}
                        $contenu_liste .= $freq.','.$_POST['station6'].','.$Infos_Stn;
                        } // station6

if ($_POST['station7']) {
                        sscanf ($_POST['frequence7'],"%ikHz%s",$freq,$poubelle);

// foreach beurk : a optimiser avec un while pas_trouve

			foreach ($liste_stn as $Nom => $Infos)
				{
				if (!strcmp($Nom,$_POST['station7'])) { $Infos_Stn = $Infos;}
				}
                        $contenu_liste .= $freq.','.$_POST['station7'].','.$Infos_Stn;
                        } // station7

$fichier_nouvelle_liste = $rep_listes.$_POST['nom_liste'].'.lst';
file_put_contents ($fichier_nouvelle_liste,$contenu_liste);

}
         
} // gestion_configurations_memorisees

//
// Fonction gestion_ajout_station
// Pour ajouter un flux et un nom de station
// Nom : champ texte en saisie libre
// Type : web/mp3/ext (1/2/3)
// Source : url / nom de fichier
//

function gestion_ajout_station ($nom_fichier_stations) {

?>
<br><form method="post" action=""><fieldset>
  <label for="nom_sta"><b>Nom de la station :</b></label>
  <input type="text" id="nom_sta" placeholder="Saisir le nom" name="nom_sta" required>
  <br>
  <label for="pays_sta"><b>Pays de la station :</b></label>
  <input type="text" id="pays_sta" placeholder="Saisir le code pays" name="pays_sta" required>
  <br>
  <label for="type_sta"><b>Source :</b></label>
    <select id="type_sta" name="type_sta">
    <option value="1">Web</option>
    <option value="2">MP3</option>
    <option value="3">EXT</option>
  </select>
  <br><label for="source_sta"><b>Emplacement :</b></label>
  <input type="text" id="source_sta" placeholder="Saisir l'URL ou le nom du fichier" name="source_sta" size=50 required>
<br><br>
<center> <input type="submit" value="Valider"></center>
 </fieldset></form><br>

<?php
if (isset($_POST['source_sta']))
   {
$nouvelle_station = $_POST['nom_sta'].','.$_POST['pays_sta'].','.$_POST['type_sta'].','.$_POST['source_sta']."\n";
$fp_liste_sta = fopen ($nom_fichier_stations,"a");
fwrite ($fp_liste_sta,$nouvelle_station);
fclose ($fp_liste_sta);
}

} // gestion_ajout_station

//
// Fonction selection_diffusion
// Pour sélectionner une liste à diffuser
//

function selection_diffusion ($repertoire_fichiers,$fichier_actif) {

// On commence par lister tous les fichiers mémorisés (*.lst dans /media)

$filtre_fichiers = $repertoire_fichiers."*.lst";
$nbr_fichiers = 0;

foreach (glob($filtre_fichiers) as $fichier) {
        
        $liste_fichiers[$nbr_fichiers] =  $fichier;
		$nbr_fichiers++;
	    }

ksort($liste_fichiers);
unset ($fichier);

// Construction de la liste déroulante

$numero = 0;

echo '<br><form method="post" action=""><fieldset><center>
  <label for="nom_liste"><b>S&eacute;lection de la liste : </b></label>';
 
 echo '<select id="nom_fichier" name="nom_fichier">';
 echo '<option value="0"> Choisir un fichier </option>';
   
  // Parcours tableau des fichiers

  foreach ($liste_fichiers as $Nom)
  {
    // on supprime le nom du répertoire et l'extension .lst avant d'afficher le nom des listes 
    $Nom_court = str_replace(".lst","",str_replace($repertoire_fichiers, "", $Nom));
    echo '<option value="', $Nom ,'">', $Nom_court ,'</option>';
  }
  echo '</select><br><br>';


if (isset($_POST['nom_fichier']) && ($_POST['nom_fichier'] != "0"))
  {

echo '<center><b>',str_replace(".lst","",str_replace($repertoire_fichiers, "", $_POST['nom_fichier'])),'</b></center>';

$lignes = file ($_POST['nom_fichier']);
$liste_freq = [];
$freq_actives = "";
foreach ($lignes as $ligne) {
        $champs= explode (",",$ligne);
        $liste_freq[$champs[0]] =  $champs[1].','.$champs[2];
	    }
ksort($liste_freq);

foreach ($liste_freq as $freq_station => $desc_station)
{
$ligne_station = explode (",",$desc_station);
if ($freq_station) $freq_actives .= $freq_station . ' kHz - ' . $ligne_station[0] . '<br>';
else $freq_actives = $lignes_station[0].'<br>';
}
echo $freq_actives,"</br>";

// la nouvelle liste de diffusion vient remplacer la liste active

copy ($_POST['nom_fichier'],$fichier_actif);

} // if isset nom_fichier
echo '<input type="submit" value="Valider"></center></fieldset></form><br>';



} // selection_diffusion

//
// Fonction gestion_parms_systeme
// Pour gérer les paramètres système
//

function gestion_parms_systeme ($nom_fichier_wifi,$num_version) {

echo "
<br><center>

<b>AM8 - Version $num_version</b>"
?>
<br><br>

<!-- Bouton de mise à jour -->
<button type="button" class="btn btn-info btn-sm" data-toggle="modal" data-target="#Modal_maj">Recherche de mise Ã  jour</button>

<!-- Modal -->
<div id="Modal_maj" class="modal fade" role="dialog">
  <div class="modal-dialog">

    <!-- Modal content-->
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal">&times;</button>
        <h4 class="modal-title">Mise Ã  jour</h4>
      </div>
      <div class="modal-body">
        <p>
<?php passthru('/home/orangepi/amt/bin/maj_am8');?>
</p>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
      </div>
    </div>

  </div>
</div>

<p></p>
<?php
} // gestion_parms_systeme


?>

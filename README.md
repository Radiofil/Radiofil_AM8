# Projet AM8 - Radiofil
# Version 1.01 - Mai 2024

Contenu du depot:

## /Configuration:
Fichiers de configuration de l'AM8:
* Listes usine et fréquences
* Configuration du système d'exploitation et du mediaplayer

## /IHM:
* Site WEB de l'IHM en PHP

## /ModulateurAM8
Sources pour le microgiciel du Raspberry Pi pico H (RP2040) installé sur la carte de modulation:
* Le dossier /src contient les sources du microgiciel
* build_modulateur.sh : Script Linux pour compiler le microgiciel du Raspberry Pi pico H
* pico_sdk_import.cmake : Fichier de configuration pour l'importation du kit de developpement "pico_sdk"
* CMakeLists.txt : Fichier de configuration de la compilation

Pour compiler plusieurs options sont possibles:
* En récupérant localement et dans le répertoire le "SDK" depuis son dépôt sur Github
1. `git clone -b master https://github.com/raspberrypi/pico-sdk/`
2. export PICO_SDK_PATH=[chemin absolu vers le dossier contenant le SDK] (exemple "/home/pico-sdk")
* En téléchargeant automatiquement à chaque compilation le SDK:
1. `export PICO_SDK_FETCH_FROM_GIT=ON`
* Pour terminer dans le répertoire contenant les sources du modulateur:
1. `./build_modulateur.sh`
2. Un répertoire "build" sera ainsi créé et il contiendra les binaires du microgiciel : "am8F01.uf2" pour un chargement en mode "glisser/déposer" via USB ou "am8F01.hex" pour un chargement via l'interface de deboggage

## /Programmes:
* am031.c : Programme permettant l'alimentation de la carte de modulation en échantillons audio
* definitions.h : Fichier de définitions générales
* lance_am8.c : Programme permettant de démarrer l'AM8 et de gérer ses état (dont bouton poussoir)
* maj_am8.c : Programme executé pour la mise à jour (téléchargement, désarchivage et execution du script de mise à jour)

Ces programmes peuvent être compilés directement sur le nano ordinateur Orange Pi Zero via le script "compilation.sh" exécuté depuis le dossier "/home/orangepi/amt/scripts"
La compilation des sources est incluse dans le script d'installation.

## /Scripts:
* build_tar.sh : Depuis le nano ordinateur et dans le dossier "home" de l'utilisateur "orangepi", permet de créer le "package" de mise à jour
* compilation.sh : Depuis le nano ordinateur, compilation des programmes en (dossier Programmes)
* eteindre_am8.sh : Script pour éteindre l'AM8
* install.sh : Script d'installation des logiciels et de l'IHM sur un AM8 à partir d'un "package"
* maj_am8.sh : Script de mise à jour de l'AM8 (désarchivage logiciels et IHM)
* start.sh : Script de déamrrage de la diffusion
* stopper_am8.sh : Script pour stopper la diffusion
* cross_compilation.sh : Script pour compiler les programmes en C depuis le conteneur Docker de compilation

## Construction du projet AM8
Le script "build_projet.sh" permet de créer les archives necessaires (dont compilation croisée des programmes en C avec Docker en utilisant AM8_docker comme fichier de création de l'image) pour mettre à jour l'AM8 ou effectuer une première installation.
Le microgiciel pour le Raspberry Pi Pico H est également compilé par ce script et disponible dans l'archive "Artifacts/modulateur_am8.tar" (pico_sdk téléchargé depuis GITHUB)

## Licence:
* GNU General Public License version la plus récente - hors microgiciel ModulateurAM8
* ModulateurAM8 : The MIT License







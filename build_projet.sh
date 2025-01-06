#!/bin/bash

#-Creation du dossier pour les artefacts de construction
rm -rf Artifacts
mkdir Artifacts 

#-Compilation croisée (aarch64) des programmes en C dans le dossier /amt/bin (avec Docker pour GLIBC 2.31)
mkdir Artifacts/amt Artifacts/amt/bin
docker build -t debianbullseye:am8 -f AM8_docker .
docker run -t -i --rm -v ./:/build/ debianbullseye:am8 /bin/bash -c '/build/Scripts/cross_compilation.sh'
docker rmi $(docker images -q)


#-Copie des fichiers non cimpilé dans la structure ad hoc
mkdir Artifacts/amt/src Artifacts/amt/etat Artifacts/amt/ini Artifacts/amt/listes Artifacts/amt/media Artifacts/amt/scripts Artifacts/amt/syst

cp Scripts/maj_am8.sh Artifacts/
cp Configuration/version.txt Artifacts/version_locale.txt
cp Configuration/url_serveur.txt Artifacts/

cp Programmes/*.c Artifacts/amt/src/

cp Configuration/*.conf Artifacts/amt/ini/
cp Configuration/*.ini Artifacts/amt/ini/    

cp Configuration/*rfl.txt Artifacts/amt/listes/
cp Configuration/*.lst Artifacts/amt/listes/

cp Scripts/compilation.sh Artifacts/amt/scripts/
cp Scripts/eteindre_am8.sh Artifacts/amt/scripts/
cp Scripts/stopper_am8.sh Artifacts/amt/scripts/
cp Scripts/start.sh Artifacts/amt/scripts/

cp Scripts/maj_am8.sh Artifacts/amt/syst/
cp Configuration/url_serveur.txt Artifacts/amt/syst/
cp Configuration/version.txt Artifacts/amt/syst/

#-Création des archives et nettoyage
cd Artifacts
tar cvzf ./am8.tar ./amt/bin ./amt/ini ./amt/etat ./amt/listes/*_rfl* ./amt/media ./amt/scripts ./amt/src
cd ../IHM
tar cvzf ../Artifacts/IHM.tar ./
cd ../Artifacts
tar cvzf ./projet_am8.tar ./am8.tar ./IHM.tar ./maj_am8.sh ./version_locale.txt  ./url_serveur.txt

rm -rf amt
rm am8.tar IHM.tar

#-Compilation du modulateur AM8 (RP2040)
cd ../ModulateurAM8
source ./build_modulateur.sh
cp build/am8F01.* ../Artifacts/
rm -rf build
cd ../Artifacts
tar cvzf ./modulateur_am8.tar ./am8F01.hex ./am8F01.uf2
rm -f am8F01*

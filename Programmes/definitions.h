#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define LEN 64
#define STR_LEN 255 // Longueur d'une chaine de car.
#define URL_LEN 200 // Longueur d'une adresse URL
#define MAX_FREQ 8 // 8 canaux maximum
#define IDX_URL 0 // mpd0x.conf pour les sources URL
#define IDX_LOCAL 10 // mpd1x.conf pour les sources locales
#define MAX_RELANCE 5
#define MAX_COUPURE 10

#define PORT_LED_ON 9  // LED Temoin Allumage AM8 - PC15 donc GPIO79 dont le port 9 pour WiringOP
#define PORT_LED_DIFF 10  // LED Temoin Diffusion - PC14 donc GPIO78 dont le port 9 pour WiringOP
#define PORT_RAZ 16 // RaZ RPI Pico - PC10 donc GPIO78 donc le port 16 pour WiringOP
#define PORT_BP 5  // Bouton-Poussoir - PC6 donc GPIO70 dont le port 5 pour WiringOP
#define GPIO_BP 70
#define DELAI_BP 2 // Le bouton doit rester appuyé au moins 2 secondes

#define HOME_DIR "/home/orangepi/amt"
#define SYST_DIR "syst"

#define NOM_FIC_VERSION "version.txt"
#define NOM_FIC_ARCHIVE "projet_am8.tar"


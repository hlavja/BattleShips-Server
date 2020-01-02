#include "main.h"
#include "serverListener.h"
#include "globalVariable.h"
#include "server.h"

void printHelp();

int main(int argc, char *argv[]) {
    pthread_t serverListenerThread;

    printf("START!\n");
    readParameters(argc, argv);
    pthread_create(&serverListenerThread, NULL, &serverListener, NULL);
    startServer();
}

int readParameters(int argc, char *argv[]) {
    int i, j;
    i = argc;
    j = 1;
    while (i > 1) {
        if (strcasecmp(argv[j], "-help") == 0) {
            printHelp();
            exit(0);
        } else if (strcasecmp(argv[j], "-port") == 0 && argv[j + 1] != NULL) {
            int pom;
            pom = atoi(argv[j + 1]);
            if (pom >= 1 && pom <= 65535) {
                SERVER_PORT = pom;
                printf("Nastaven port na %d\n", SERVER_PORT);
            } else {
                printf("Nastaveni portu je chybne! Zustava na %d\n", SERVER_PORT);
            }
            i = i - 2;
            j = j + 2;
            continue;
        }else if (strcasecmp(argv[j], "-address") == 0 && argv[j + 1] != NULL) {

            IP_ADDRESS = argv[j + 1];

            if(validateIpAddress(IP_ADDRESS)){
                printf("Nastavena IP adresa serveru na %s\n", IP_ADDRESS);
            } else{
                printf("Nastavení IP adresy se nezdařilo. Zustává původní nastavení.\n");
            }

            i = i - 2;
            j = j + 2;
            continue;
        }else if (strcasecmp(argv[j], "-rooms") == 0 && argv[j + 1] != NULL) {
            int pom;
            pom = atoi(argv[j + 1]);

            if (pom > 0 && pom < 7) {
                MAX_ROOMS = pom;
                printf("Nastaven maximalni pocet her na %d\n", MAX_ROOMS);
            } else {
                printf("Nastaveni maximalniho poctu her je chybne. Zustava na %d\n", MAX_ROOMS);
            }

            i = i - 2;
            j = j + 2;
            continue;
        }
        i--;
    }
    return 0;
}

void printHelp() {
    printf("%s\tSERVER HELP\n%s");
    printf("-address [ip adresa] \t nastavi ip adresu, na kterem bude server naslouchat.\n");
    printf("-port [cislo] \t nastavi port, na kterem bude server naslouchat (1-65535).\n");
    printf("-rooms [cislo] \t nastavi kolik her bude moct soubezne bezet.\n");
    printf("-help \t\t vypsani napovedy k paramterum.\n");
}

int validateIpAddress() {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, IP_ADDRESS, &(sa.sin_addr));
    return result != 0;
}
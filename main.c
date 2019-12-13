#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "globalVariable.h"
#include "server.h"
#include "main.h"

void printHelp();

int main(int argc, char *argv[]) {
    printf("START!\n");
    readParameters(argc, argv);
    startServer();
}

int readParameters(int argc, char *argv[]) {
    int i = argc;
    int param = 1;

    while (i > 1){
        if (strcasecmp(argv[param], "-help") == 0) {
            printHelp();
            exit(0);
        } else if (strcasecmp(argv[param], "-port") == 0 && argv[param+1] != NULL)  {
            int port = atoi(argv[param + 1]);
            if ( port >= 1 && port <= 65535) {
                SERVER_PORT = port;
                printf("Nastaven port %i", port);
            } else {
                printf("Port %i nejde nastavit!", port);
            }
        i = i - 2;
        param = param + 2;
        continue;
        } else if (strcasecmp(argv[param], "-pending") == 0 && argv[param+1] != NULL){
            int waitForPlayer = atoi(argv[param + 1]);
            if (waitForPlayer >= 1){
                ROOM_WAITING_TIME = waitForPlayer;
                printf("Nastaveno čekání na připojení druhého hráče do nové hry na %i", waitForPlayer);
            } else {
                printf("Nelze nastavit!");
            }
            i = i + 2;
            param = param + 2;
            continue;
        } else if (strcasecmp(argv[param], "-reconnect") == 0 && argv[param+1] != NULL){
            int reconnectTime = atoi(argv[param + 1]);
            if (reconnectTime >= 1){
                TIMEOUT = reconnectTime;
                printf("Nastaven timeout pro opětovné připojení na %i", reconnectTime);
            } else {
                printf("Nastavení timeoutu neproběhlo!");
            }
            i = i + 2;
            param = param + 2;
            continue;
        } else if (strcasecmp(argv[param], "-address") == 0 && argv[param+1] != NULL){
            IP_ADDRESS = argv[param + 1];

            if (validateIpAddress()){
                printf("Nastavena IP adresa serveru na %s", IP_ADDRESS);
            } else {
                printf("Nastavení IP adresy na %s skončilo chybou! Adresa se nenastavila.");
            }
            i = i + 2;
            param = param + 2;
            continue;
        } else if (strcasecmp(argv[param], "-rooms") == 0 && argv[param+1] != NULL){
            int roomCount = atoi(argv[param + 1]);
            if(roomCount >= 1){
                MAX_ROOMS = roomCount;
                printf("Maximální počet místností nastaven na %i", roomCount);
            } else {
                printf("Si blbej a zadal si špatně jednoduché číslo %i", roomCount);
            }
            i = i + 2;
            param = param + 2;
            continue;
        }
        i--;
    }
    return 1;
}

void printHelp() {
    printf("AHOJ");
}

int validateIpAddress() {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, IP_ADDRESS, &(sa.sin_addr));
    return result != 0;
}
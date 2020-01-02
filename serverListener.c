//
// Created by hlavja on 13/12/2019.
//
#include "serverListener.h"
#include "globalVariable.h"
#include "game.h"
#include "logger.h"

void *serverListener(void *arg){

    signal(SIGINT, sigintHandler);

    char *command = calloc(sizeof(char) * BUFFERSIZE, 1);
    while(1){
        printf(">>>\n");
        scanf("%s",command);
        if(strcasecmp(command, "exit") == 0 || strcasecmp(command, "close") == 0){
            logMsg("Server ukoncen prikazem EXIT\n");
            logServerShutdown();
            printf("\nSERVER: Ukonceno prikazem EXIT\n");
            exit(0);
        }
        else if(strcmp(command, "game") == 0){
            printf("\nBezici hry:\n");
            printf("---------------------------\n");
            printGames();
        }
        else if(strcmp(command, "info") == 0){
            printf("INFO: Server bezi na %s:%d\n", IP_ADDRESS, SERVER_PORT);
            printf("INFO: Maximalni pocet her je  %d. \n", MAX_ROOMS);

        }
        else if(strcmp(command, "help") == 0){
            printf("exit (close): ukonci server\n");
            printf("game: vypise prave probihajici hry\n");
            printf("info: vypise informace o server\n");
        }
        else if(strcmp(command, "player") == 0){
            printf("\nPrihlaseni uzivatele:\n");
            printf("---------------------------\n");
            printPlayers();
        }
        else{
            printf("Neznamy prikaz. Pouzijte \"help\" pro napovedu.\n");
        }
        memset(command,0,strlen(command));

    }


}

void sigintHandler(int sig){
    logMsg("Ukonceno pres CTRL+C\n");
    logServerShutdown();
    printf("\nSERVER: Detekovano stiknuti CTRL+C. Server ukoncen!\n");
    exit(1);
}
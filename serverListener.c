//
// Created by hlavja on 13/12/2019.
//
#include "serverListener.h"

void *serverListener(void *arg){

    signal(SIGINT, sigint_handler);

    char *command = calloc(sizeof(char) * BUFFERSIZE, 1);
    while(1){
        printf(">>>");
        scanf("%s",command);
        if(strcasecmp(command, "exit")==0 || strcasecmp(command, "close")==0){
            //log_server_add("Server ukoncen prikazem EXIT\n");
            //log_server_over();
            printf("\nSERVER: Ukonceno prikazem EXIT\n");
            exit(0);
        }
        else if(strcmp(command, "game")==0){
            printf("\nBezici hry:\n");
            printf("---------------------------");
            printGames();
        }
        else if(strcmp(command, "info")==0){
            printf("INFO: Server bezi na %s:%d\n", IP_ADDRESS, SERVER_PORT);
            printf("INFO: Server ceka na protihrace %d s.\n", ROOM_WAITING_TIME);
            printf("INFO: Server ceka na reconnect %d s.\n", TIMEOUT);
            printf("INFO: Maximalni pocet her je  %d. \n", MAX_ROOMS);

        }
        else if(strcmp(command, "help")==0){
            printf("exit (close): ukonci server\n");
            printf("game: vypise prave probihajici hry\n");
            printf("info: vypise informace o server\n");
        }
        else{
            printf("Neznamy prikaz. Pouzijte \"help\" pro napovedu.\n");
        }
        memset(command,0,strlen(command));

    }


}

void sigint_handler(int sig){
    //log_server_add("Ukonceno pres CTRL+C\n");
    //log_server_over();
    printf("\nSERVER: Detekovano stiknuti CTRL+C. Server ukoncen!\n");
    exit(1);
}
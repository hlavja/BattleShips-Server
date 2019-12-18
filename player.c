//
// Created by hlavja on 09/12/2019.
//

#include "player.h"
#include "globalVariable.h"


player* createPlayer(int socket, char *name) {
    player *newPlayer = NULL;
    newPlayer = malloc(sizeof(player));
    newPlayer->playerSocket = socket;
    newPlayer->gameId = 0;
    newPlayer->inGame = 0;
    strncpy(newPlayer->nick, name, 30);

    return newPlayer;
}

void freePlayer(player **player) {
    if (!(*player)) {
        return;
    }
    free((*player)->nick);
    free((*player));
    *player = NULL;
}

player* findPlayerByName(char *name){
    for (int i = 0; i < MAX_PLAYER_COUNT; ++i) {
        if (PLAYERS[i] != NULL && !strcmp(PLAYERS[i]->nick, name)){
            player *temp = NULL;
            temp = PLAYERS[i];
            return temp;
        }
    }
}

void printPlayers(){
    for (int i = 0; i < MAX_PLAYER_COUNT; ++i) {
        if (PLAYERS[i] != NULL) {
            printf("Player %s se socketem %d\n", PLAYERS[i]->nick, PLAYERS[i]->playerSocket);
        }
    }
}
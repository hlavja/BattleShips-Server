//
// Created by hlavja on 09/12/2019.
//

#include "player.h"

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
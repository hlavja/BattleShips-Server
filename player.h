//
// Created by hlavja on 09/12/2019.
//

#ifndef UPS_PLAYER_H
#define UPS_PLAYER_H

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



typedef struct player{
    char nick [30];
    int playerSocket;
    bool inGame;
    int gameId;
} player;

player* createPlayer(int socket, char *name);
void freePlayer(player **player);

player* findPlayerByName(char *name);
void printPlayers();

#endif //UPS_PLAYER_H

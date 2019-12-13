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

typedef struct {
    char nick [30];
    int playerSocket;
    bool inGame;
    int gameId;
} player;

void freePlayer(player **player);

#endif //UPS_PLAYER_H

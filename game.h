//
// Created by hlavja on 09/12/2019.
//
#ifndef UPS_GAME_H
#define UPS_GAME_H

#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include "player.h"

typedef struct game{
    int playerCount;
    long gameId;
    char gameOwnerNick[30];
    int player1Ships, player2Ships;
    int gridPlayer1[6][6], gridPlayer2[6][6];
    bool player1Grid, player2Grid;
    int playerTurn, gameStatus;
    bool player1Connected, player2Connected;
    player *player1;
    player *player2;
    bool player2Repeat, player1Repeat;
} game;

game *createNewGame(player *player1);
void printGames();

bool findGameWithInactivePlayer(char *name);
void freeGames (game **allGames);
game **gameSearchById (game **allGames, int id);
game **gameSearchByNick (game **allGames, char *nick);
void listGames(game *allGames);
int sendAllGames(game *allGames, int fdSet);
#endif //UPS_GAME_H

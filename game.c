//
// Created by hlavja on 09/12/2019.
//

#include "game.h"
#include "globalVariable.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

game *createNewGame(player *player1){
    game *newGame = NULL;
    newGame = malloc(sizeof(game));

    newGame->player2 = NULL;

    newGame->player1Ships = NUMBER_OF_SHIPS;
    newGame->player2Ships = NUMBER_OF_SHIPS;
    newGame->player2Grid = false;
    newGame->player1Grid = false;
    newGame->playerTurn = 0;
    newGame->gameStatus = 1; //running
    newGame->player1Connected = true;
    newGame->player2Connected = false;

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            newGame->gridPlayer1[i][j] = 0;
            newGame->gridPlayer2[i][j] = 0;
        }
    }

    newGame->gameId = GAME_COUNTER;
    GAME_COUNTER++;
    newGame->player1 = player1;
    newGame->playerCount = 1;
    strncpy(newGame->gameOwnerNick, player1->nick, 30);
    return newGame;
}

void freeGames(game **game) {
    if (!(*game)) {
        return;
    }
    free((*game)->gameOwnerNick);
    free((*game));
    *game = NULL;
}

void printGames(){
    for (int i = 0; i < MAX_ROOMS; ++i) {

    }

}

















/*
void freeGames (game **allGames){
    game *temp = NULL;
    while(*allGames){
        temp = (game *) (*allGames)->nextGame;
        printf("Freeing %s...... \n", (*allGames)->gameOwnerNick);
        free(*allGames);
        *allGames = temp;
    }
    *allGames = NULL;
    free(temp);
}


game **gameSearchById (game **allGames, int id){
    if (allGames) {
        while (*allGames) {
            if ((*allGames)->gameId == id) {
                return allGames;
            }
            allGames = (game **) &(*allGames)->nextGame;
        }
    }
    return NULL;
}

game **gameSearchByNick (game **allGames, char *nick){
    if (allGames) {
        while (*allGames) {
            if (strcmp((*allGames)->player1->nick, nick) == 0 || strcmp((*allGames)->player2->nick, nick) == 0){
                return allGames;
            }
            allGames = (game **) &(*allGames)->nextGame;
        }
    }
    return NULL;
}

void listGames(game *allGames) {
    if(!allGames){
        printf("Zadna hra neni spustena\n");
    } else{
        while(allGames) {
            printf("GAME #%ld: %s vs. %s (%d:%d)\n", allGames->gameId, allGames->player1->nick, allGames->player2->nick, allGames->player1Ships, allGames->player2Ships);
            allGames = (game *) allGames->nextGame;
        }
        printf("Zadna dalsi hra neni spustena\n");
    }
}

int sendAllGames(game *allGames, int fdSet){
    return 1;
}
*/
//
// Created by hlavja on 09/12/2019.
//

#ifndef UPS_GLOBALVARIABLE_H
#define UPS_GLOBALVARIABLE_H

#include <sys/select.h>
#include <stdio.h>
#include "game.h"
#include "player.h"

int SERVER_PORT;
int ROOM_WAITING_TIME;
int TIMEOUT;
char *IP_ADDRESS;
int MAX_ROOMS;
long GAME_COUNTER;
int NUMBER_OF_SHIPS;
fd_set CLIENT_SOCKS;
int FD_OFFSET;
int MAX_CLIENT_COUNT, CLIENT_COUNT;
player **PLAYERS;
game **GAMES;
int BUFFERSIZE;
FILE *LOG_FILE;
time_t SERVER_START, SERVER_CLOSE;
long MESSAGE_SENT;
long MESSAGE_RECV;
long BYTE_SENT;
long BYTE_RECV;

#endif //UPS_GLOBALVARIABLE_H

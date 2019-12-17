//
// Created by hlavja on 09/12/2019.
//

#ifndef UPS_GLOBALVARIABLE_H
#define UPS_GLOBALVARIABLE_H

#include <sys/select.h>
#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <arpa/inet.h>
#include "player.h"
#include "game.h"

int SERVER_PORT;
int ROOM_WAITING_TIME;
int TIMEOUT;
char *IP_ADDRESS;
int MAX_ROOMS;
long GAME_COUNTER;
int ACTIVE_ROOMS;
int NUMBER_OF_SHIPS;
fd_set CLIENT_SOCKS;
int FD_OFFSET;
int MAX_PLAYER_COUNT, PLAYER_COUNT;
player **PLAYERS;
game **GAMES;
int BUFFERSIZE;
FILE *LOG_FILE;
time_t SERVER_START, SERVER_CLOSE;
long MESSAGE_SENT;
long MESSAGE_RECV;
long BYTE_SENT;
long BYTE_RECV;
pthread_rwlock_t LOCKTHREAD;

#endif //UPS_GLOBALVARIABLE_H

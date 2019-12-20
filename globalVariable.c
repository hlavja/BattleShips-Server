//
// Created by hlavja on 09/12/2019.
//

#include "globalVariable.h"

pthread_rwlock_t LOCKTHREAD;
int SERVER_PORT = 15000;
int ROOM_WAITING_TIME = 150000;
int TIMEOUT = 1500;
char *IP_ADDRESS;
int MAX_ROOMS = 5, ACTIVE_ROOMS = 0;
long GAME_COUNTER = 1;
int NUMBER_OF_SHIPS = 6;
int MAX_PLAYER_COUNT = 10, PLAYER_COUNT = 0;
int BUFFERSIZE = 200;

FILE *LOG_FILE;
time_t SERVER_START, SERVER_CLOSE;
long MESSAGE_SENT = 0;
long MESSAGE_RECV = 0;
long BYTE_SENT = 0;
long BYTE_RECV = 0;
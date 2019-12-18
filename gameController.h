//
// Created by hlavja on 17/12/2019.
//

#ifndef UPS_GAMECONTROLLER_H
#define UPS_GAMECONTROLLER_H

#include <pthread.h>

void socketCut(int socket);
int parseMessage(int socket, char *msg);
void playerLogout(int socket, char *string);
void playerLogin(int socket, char *string);

void initRoom(int socket, char *ownerName);

void lostConnectionToPlayer(int socket);

void requestRooms(int socket);

void joinRoom(int socket, char *name, char *room);

#endif //UPS_GAMECONTROLLER_H

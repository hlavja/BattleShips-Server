//
// Created by hlavja on 09/12/2019.
//

#ifndef UPS_MAIN_H
#define UPS_MAIN_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

int readParameters(int argc, char *argv[]);
int validateIpAddress();
#endif //UPS_MAIN_H

//
// Created by hlavja on 12/12/2019.
//

#ifndef UPS_CONNECTIONHANDLER_H
#define UPS_CONNECTIONHANDLER_H
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

void *connectionHandler(void *arg);

#endif //UPS_CONNECTIONHANDLER_H

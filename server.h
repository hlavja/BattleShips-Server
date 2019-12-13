//
// Created by hlavja on 09/12/2019.
//

#ifndef UPS_SERVER_H
#define UPS_SERVER_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "server.h"
#include "globalVariable.h"
#include "game.h"
#include "player.h"
#include "logger.h"
#include "connectionHandler.h"
#include "serverListener.h"

int startServer();

#endif //UPS_SERVER_H

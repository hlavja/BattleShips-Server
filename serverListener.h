//
// Created by hlavja on 13/12/2019.
//

#ifndef UPS_SERVERLISTENER_H
#define UPS_SERVERLISTENER_H

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <signal.h>
#include "game.h"
#include "serverListener.h"
#include "globalVariable.h"

void *serverListener(void *arg);
void sigint_handler(int sig);
#endif //UPS_SERVERLISTENER_H

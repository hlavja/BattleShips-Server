//
// Created by hlavja on 13/12/2019.
//

#ifndef UPS_SERVERLISTENER_H
#define UPS_SERVERLISTENER_H

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <signal.h>

void *serverListener(void *arg);
void sigintHandler(int sig);
#endif //UPS_SERVERLISTENER_H

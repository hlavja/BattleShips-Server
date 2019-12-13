//
// Created by hlavja on 12/12/2019.
//

#ifndef UPS_LOGGER_H
#define UPS_LOGGER_H

#include <time.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include "globalVariable.h"

void serverLoggingStart();
void logMsg(char *msg);
void logSent(int bytes);
void logReceive(int bytes);
void logServerShutdown();

#endif //UPS_LOGGER_H

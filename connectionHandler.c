//
// Created by hlavja on 12/12/2019.
//
#include "connectionHandler.h"
#include "gameController.h"
#include "globalVariable.h"
#include "logger.h"

void lostConnectionToPlayer(int socket);

void *connectionHandler(void *arg){
    int clientSocket, receivedBoolean, messageSize, command;
    char message[200], msgSize[3];
    clientSocket = *(int *) arg;
    int missedPings = 0;

    while (1) {
        memset(message, '\0', sizeof(message));
        memset(msgSize, '\0', sizeof(msgSize));
        receivedBoolean = recv(clientSocket, msgSize, 3, 0); //first three bytes determine message length
        logReceive(3);

        while (receivedBoolean < 0 && missedPings < 5) {

            if (missedPings == 3) {
                printf("Sending %i lost con notify %d.\n",missedPings, clientSocket);
//                lostConnectionToPlayer(clientSocket);
            }
            send(clientSocket, "ping\n", 5, 0);
            //printf("Timeout. Sending ping to socket: %d\n", clientSocket);
            receivedBoolean = recv(clientSocket, msgSize, 3, 0);
            logReceive(3);
            missedPings++;

        }

        if (missedPings == 12) {
            socketCut(clientSocket);
            printf("Closing connection. Socket: %d\n", clientSocket);
            free(arg);
            return 0;
        }

        missedPings = 0;
        messageSize = strtol(msgSize, NULL, 10);  //size of receiving message

        if (messageSize > 0) {
            receivedBoolean = recv(clientSocket, message, messageSize, 0);  //save rest of message to message
            logReceive(messageSize);
        }
        if (receivedBoolean < 0) {
            continue;
        }

        //printf("Message form client: %s\n", msgSize);
        command = parseMessage(clientSocket, message);

        if (command == 2) {
            close(clientSocket);
            free(arg);
            break;
        }

        if(receivedBoolean == 0) {
            printf("Connection closed.\n");
            socketCut(clientSocket);
            close(clientSocket);
            free(arg);
            break;
        }

        if (command == 0) {
            printf("Message not recognized\n");
            socketCut(clientSocket);
            close(clientSocket);
            free(arg);
            break;
        }
    }
    return 0;
}

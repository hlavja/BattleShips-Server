//
// Created by hlavja on 09/12/2019.
//
#include "server.h"
#include "globalVariable.h"
#include "logger.h"
#include "connectionHandler.h"

void initializeRoomsAndPlayersArrays() {
    int i;
    PLAYERS = calloc(MAX_PLAYER_COUNT, sizeof(player *));
    GAMES = calloc(MAX_ROOMS, sizeof(game *));
    for (i = 0; i < MAX_PLAYER_COUNT; i++) {
        PLAYERS[i] = NULL;
    }

    for (i = 0; i < MAX_ROOMS; i++) {
        GAMES[i] = NULL;
    }

}

int validIpNumbers(char *ipString) {
    while (*ipString) {
        if (*ipString >= '0' && *ipString <= '9')
            ++ipString;
        else
            return 0;
    }
    return 1;
}

int isIpValid(char *ipString) {
    int num, dots = 0;
    char *ptr;
    if (ipString == NULL)
        return 0;
    ptr = strtok(ipString, ".");
    if (ptr == NULL)
        return 0;
    while (ptr) {
        if (!validIpNumbers(ptr)) {
            return 0;
        }
        num = atoi(ptr);
        if (num >= 0 && num <= 255) {
            ptr = strtok(NULL, ".");
            if (ptr != NULL)
                ++dots;
        } else {
            return 0;
        }
    }
    if (dots != 3)
        return 0;
    return 1;
}

int startServer(){
    int serverSocket, clientSocket, *fd;
    struct sockaddr_in myAddr, remoteAddr;
    pthread_t connectionThread;
    struct timeval timeval = {1,0};
    socklen_t remoteAddrLen = sizeof(struct sockaddr);

    serverLoggingStart();

    printf("Server start!\n");

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    int param = 1;

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &param, sizeof(int)) == -1)
    {
        printf("setsockopt ERR\n");
        exit(1);
    }

    memset(&myAddr, 0, sizeof(struct sockaddr_in));
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(SERVER_PORT);

    int valid = 0;
    if (IP_ADDRESS != NULL){
        char *tmp = malloc(sizeof(char) * strlen(IP_ADDRESS));
        strcpy(tmp, IP_ADDRESS);
        valid = isIpValid(tmp);
    }
    if (valid) {
        myAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    } else {
        myAddr.sin_addr.s_addr = INADDR_ANY;
    }

    if (bind(serverSocket, (struct sockaddr *) &myAddr, sizeof(struct sockaddr_in)) != 0)
    {
        printf("Bind - ERROR\n");
        exit(1);
    }

    if (listen(serverSocket, 5) != 0){
        printf("Listen - ER\n");
        exit(1);
    }

    printf("Server successfully started\n");
    printf("--------------------------------------\n");
    if (IP_ADDRESS != NULL){
        printf("Server is running on %s:%d\n", IP_ADDRESS, SERVER_PORT);
    } else {
        printf("Server is running on port %d\n", SERVER_PORT);
    }
    printf("--------------------------------------\n");

    pthread_rwlock_init(&LOCKTHREAD, NULL);
    initializeRoomsAndPlayersArrays();

    while(1){
        clientSocket = accept(serverSocket, (struct sockaddr *) &remoteAddr, &remoteAddrLen);
        setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *) &timeval, sizeof(struct timeval));
        if (clientSocket > 0){
            fd = malloc(sizeof(int));
            *fd = clientSocket;
            printf("New Connection!\n");
            pthread_create(&connectionThread, NULL, (void *) &connectionHandler, (void *) fd);
        } else{
            printf("ERROR");
            return -1;
        }
    }
}
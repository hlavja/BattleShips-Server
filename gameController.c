//
// Created by hlavja on 17/12/2019.
//

#include "gameController.h"
#include "globalVariable.h"
#include "game.h"
#include "player.h"
#include "logger.h"

void rematch(char *name, char *room);

void resetGame(game *pGame);

void leaveRoom(char *room);

void sendReconnectMessage(int socket, char *name);

int parseMessage(int socket, char *msg) {
    long int type;
    char *name, *room, t[2], *placing, *x, *y;
    t[0] = msg[0];
    t[1] = msg[1];
    type = strtol(t, NULL, 10);
    //printf("%s\n", msg);
    //bytes_in += strlen(msg);
    switch (type) {
        case 1:
            pthread_rwlock_rdlock(&LOCKTHREAD);
            playerLogin(socket, msg + 2);
            printf("Received Login request from player: %s\n", msg);
            pthread_rwlock_unlock(&LOCKTHREAD);
            return 1;
        case 2:
            pthread_rwlock_rdlock(&LOCKTHREAD);
            playerLogout(socket, msg + 2);
            printf("Received logout request: %s\n", msg);
            pthread_rwlock_unlock(&LOCKTHREAD);
            return 2;
        case 3:
            pthread_rwlock_rdlock(&LOCKTHREAD);
            name = strtok(msg + 2, ";");
            printf("Received create room request: %s from %s\n", msg, name);
            initRoom(socket, name);
            pthread_rwlock_unlock(&LOCKTHREAD);
            return 3;
        case 4:
            pthread_rwlock_rdlock(&LOCKTHREAD);
            requestRooms(socket);
            printf("Received rooms request: %s\n", msg);
            pthread_rwlock_unlock(&LOCKTHREAD);
            return 4;
        case 5:
            pthread_rwlock_rdlock(&LOCKTHREAD);
            printf("Received join room request: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            joinRoom(socket, name, room);
            pthread_rwlock_unlock(&LOCKTHREAD);
            return 5;
        case 6:
            pthread_rwlock_rdlock(&LOCKTHREAD);
            printf("Received leave room request: %s\n", msg);
            room = strtok(msg + 2, ";");
            leaveRoom(room);
            pthread_rwlock_unlock(&LOCKTHREAD);
            return 6;
        case 8:
            pthread_rwlock_rdlock(&LOCKTHREAD);
            printf("Received ship placement: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            placing = strtok(NULL, ";");
            // printf("Place %s\n", place);
            setShips(socket, name, room, placing);
            pthread_rwlock_unlock(&LOCKTHREAD);
            return 8;
        case 10:
            pthread_rwlock_rdlock(&LOCKTHREAD);
            printf("Received shoot message: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            x = strtok(NULL, ";");
            y = strtok(NULL, ";");
            shoot(socket, name, room, *x, *y);
            pthread_rwlock_unlock(&LOCKTHREAD);
            return 10;
        /*case 11:
            pthread_rwlock_rdlock(&lock);
            printf("Received leave game: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            leave_game(socket, name, room);
            pthread_rwlock_unlock(&lock);
            return 11;*/
        case 12:
            pthread_rwlock_rdlock(&LOCKTHREAD);
            printf("Received rematch message: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            rematch(name, room);
            pthread_rwlock_unlock(&LOCKTHREAD);
            return 12;
        /*case 13:
            pthread_rwlock_rdlock(&lock);
            printf("Received ping message, sending response.\n");
            send(socket, "alive\n", 6, 0);
            pthread_rwlock_unlock(&lock);
            return 13;*/
        case 14:
            //printf("Received ping response from socket: %d\n", socket);
            return 14;
        /*case 15:
            pthread_rwlock_rdlock(&lock);
            printf("Received game state request.\n");
            send_game_state(socket, msg+2);
            pthread_rwlock_unlock(&lock);
            return 15;
        case 16:
            pthread_rwlock_rdlock(&lock);
            printf("Received relogin request.\n");
            reconnect(socket, msg+2);
            pthread_rwlock_unlock(&lock);
            return 16;*/
        default:
            printf("%s\n", msg);
            return 0;
    }
}

void leaveRoom(char *room) {
    for (int i = 0; i < MAX_ROOMS; ++i) {
        if (GAMES[i] != NULL && !strcmp(GAMES[i]->gameOwnerNick, room)){
            send(GAMES[i]->player1->playerSocket,"endGame\n", 8, 0);
            send(GAMES[i]->player2->playerSocket,"endGame\n", 8, 0);
            GAMES[i] = NULL;
        }
    }

}

void rematch(char *name, char *room) {
    for (int i = 0; i < MAX_ROOMS; ++i) {
        if (GAMES[i] != NULL && !strcmp(GAMES[i]->gameOwnerNick, room)){
            if (!strcmp(GAMES[i]->player1->nick, name)){
                GAMES[i]->player1Repeat = true;
                if (GAMES[i]->player2Repeat && GAMES[i]->player1Repeat){
                    resetGame(GAMES[i]);
                    char *repeat = "repeat\n";
                    send(GAMES[i]->player1->playerSocket, repeat, 7, 0);
                    send(GAMES[i]->player2->playerSocket, repeat, 7, 0);
                    return;
                }
            } else if (!strcmp(GAMES[i]->player2->nick, name)){
                GAMES[i]->player2Repeat = true;
                if (GAMES[i]->player2Repeat && GAMES[i]->player1Repeat){
                    resetGame(GAMES[i]);
                    char *repeat = "repeat\n";
                    send(GAMES[i]->player1->playerSocket, repeat, 7, 0);
                    send(GAMES[i]->player2->playerSocket, repeat, 7, 0);
                    return;
                }
            }
        }
    }



}

void resetGame(game *pGame) {
    pGame->player1Ships = NUMBER_OF_SHIPS;
    pGame->player2Ships = NUMBER_OF_SHIPS;
    pGame->player2Grid = false;
    pGame->player1Grid = false;
    pGame->playerTurn = 1;
    pGame->gameStatus = 1; // 0 waiting, 1 ready, 2 running
    pGame->player1Connected = true;
    pGame->player2Connected = true;

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            pGame->gridPlayer1[i][j] = 0;
            pGame->gridPlayer2[i][j] = 0;
        }
    }
    printf("Set game state with ID: %li  OwnerName: %s to placing stage!\n", pGame->gameId, pGame->gameOwnerNick);

}

void shoot(int socket, char *name, char *room, char x, char y) {
    int fireX = x - 48, fireY = y - 48;
    if (fireX > 5 || fireX < 0 || fireY > 5 || fireY < 0){
        send(socket, "errShoot\n", 4, 0);
        close(socket);
    } else {
        for (int i = 0; i < MAX_ROOMS; ++i) {
            if (GAMES[i] != NULL && !strcmp(GAMES[i]->gameOwnerNick, room)){
                //fire from player1
                if (strcmp(GAMES[i]->player1->nick,name) == 0 && GAMES[i]->playerTurn == 1){
                    GAMES[i]->playerTurn = 2;
                    //hit
                    if (GAMES[i]->gridPlayer2[fireX][fireY] == 1){
                        GAMES[i]->gridPlayer2[fireX][fireY] = 2;
                        GAMES[i]->player2Ships--;
                        printf("Player 2 ships %i\n", GAMES[i]->player2Ships);
                        //player2 lose
                        if (GAMES[i]->player2Ships == 0){
                            printf("Sending end game\n");
                            char *messageShot = NULL;
                            messageShot = calloc(12, sizeof(char));
                            strcat(messageShot, "my;shot;");
                            strcat(messageShot, &x);
                            strcat(messageShot, ",");
                            strcat(messageShot, &y);
                            strcat(messageShot, "\n");
                            send(socket, messageShot, 12, 0);

                            char *enemyShot = NULL;
                            enemyShot = calloc(15, sizeof(char));
                            strcat(enemyShot, "enemy;sink;");
                            strcat(enemyShot, &x);
                            strcat(enemyShot, ",");
                            strcat(enemyShot, &y);
                            strcat(enemyShot, "\n");
                            send(GAMES[i]->player2->playerSocket, enemyShot, 15, 0);

                            sendGameResult(socket, GAMES[i]->player2->playerSocket);
                            return;
                           //game continue
                        } else {
                            char *messageShot = NULL;
                            messageShot = calloc(12, sizeof(char));
                            strcat(messageShot, "my;shot;");
                            strcat(messageShot, &x);
                            strcat(messageShot, ",");
                            strcat(messageShot, &y);
                            strcat(messageShot, "\n");
                            send(socket, messageShot, 12, 0);
                            printf("send response for hit to player 1\n");

                            char *enemyShot = NULL;
                            enemyShot = calloc(15, sizeof(char));
                            strcat(enemyShot, "enemy;sink;");
                            strcat(enemyShot, &x);
                            strcat(enemyShot, ",");
                            strcat(enemyShot, &y);
                            strcat(enemyShot, "\n");
                            send(GAMES[i]->player2->playerSocket, enemyShot, 15, 0);
                            printf("send response for hit to player 2\n");
                        }
                        //miss
                    } else if (GAMES[i]->gridPlayer2[fireX][fireY] == 0){
                        GAMES[i]->gridPlayer2[fireX][fireY] = 3;
                        char *messageShot = NULL;
                        messageShot = calloc(12, sizeof(char));
                        strcat(messageShot, "my;miss;");
                        strcat(messageShot, &x);
                        strcat(messageShot, ",");
                        strcat(messageShot, &y);
                        strcat(messageShot, "\n");
                        send(socket, messageShot, 12, 0);

                        char *enemyShot = NULL;
                        enemyShot = calloc(15, sizeof(char));
                        strcat(enemyShot, "enemy;miss;");
                        strcat(enemyShot, &x);
                        strcat(enemyShot, ",");
                        strcat(enemyShot, &y);
                        strcat(enemyShot, "\n");
                        send(GAMES[i]->player2->playerSocket, enemyShot, 15, 0);
                        //already
                    } else {
                        char *messageShot = NULL;
                        messageShot = calloc(11, sizeof(char));
                        strcat(messageShot, "my;already;");
                        strcat(messageShot, &x);
                        strcat(messageShot, &y);
                        strcat(messageShot, "\n");
                        send(socket, messageShot, 14, 0);
                    }
                    //fire from player2
                } else if (strcmp(GAMES[i]->player2->nick,name) == 0 && GAMES[i]->playerTurn == 2){
                    GAMES[i]->playerTurn = 1;
                    //hit
                    if (GAMES[i]->gridPlayer1[fireX][fireY] == 1){
                        GAMES[i]->gridPlayer1[fireX][fireY] = 2;
                        GAMES[i]->player1Ships--;
                        printf("Player 1 ships %i\n", GAMES[i]->player1Ships);
                        //player1 lose
                        if (GAMES[i]->player1Ships == 0){
                            printf("Sending end game\n");
                            char *messageShot = NULL;
                            messageShot = calloc(12, sizeof(char));
                            strcat(messageShot, "my;shot;");
                            strcat(messageShot, &x);
                            strcat(messageShot, ",");
                            strcat(messageShot, &y);
                            strcat(messageShot, "\n");
                            send(socket, messageShot, 12, 0);

                            char *enemyShot = NULL;
                            enemyShot = calloc(15, sizeof(char));
                            strcat(enemyShot, "enemy;sink;");
                            strcat(enemyShot, &x);
                            strcat(enemyShot, ",");
                            strcat(enemyShot, &y);
                            strcat(enemyShot, "\n");
                            send(GAMES[i]->player1->playerSocket, enemyShot, 15, 0);

                            sendGameResult(socket, GAMES[i]->player1->playerSocket);
                            return;
                            //game continue
                        } else {
                            char *messageShot = NULL;
                            messageShot = calloc(12, sizeof(char));
                            strcat(messageShot, "my;shot;");
                            strcat(messageShot, &x);
                            strcat(messageShot, ",");
                            strcat(messageShot, &y);
                            strcat(messageShot, "\n");
                            send(socket, messageShot, 12, 0);
                            printf("send response for hit to player 2\n");

                            char *enemyShot = NULL;
                            enemyShot = calloc(15, sizeof(char));
                            strcat(enemyShot, "enemy;sink;");
                            strcat(enemyShot, &x);
                            strcat(enemyShot, ",");
                            strcat(enemyShot, &y);
                            strcat(enemyShot, "\n");
                            send(GAMES[i]->player1->playerSocket, enemyShot, 15, 0);
                            printf("send response for hit to player 1\n");
                        }
                        //miss
                    } else if (GAMES[i]->gridPlayer1[fireX][fireY] == 0){
                        GAMES[i]->gridPlayer1[fireX][fireY] = 3;
                        char *messageShot = NULL;
                        messageShot = calloc(12, sizeof(char));
                        strcat(messageShot, "my;miss;");
                        strcat(messageShot, &x);
                        strcat(messageShot, ",");
                        strcat(messageShot, &y);
                        strcat(messageShot, "\n");
                        send(socket, messageShot, 12, 0);

                        char *enemyShot = NULL;
                        enemyShot = calloc(15, sizeof(char));
                        strcat(enemyShot, "enemy;miss;");
                        strcat(enemyShot, &x);
                        strcat(enemyShot, ",");
                        strcat(enemyShot, &y);
                        strcat(enemyShot, "\n");
                        send(GAMES[i]->player1->playerSocket, enemyShot, 15, 0);
                        //already
                    } else {
                        char *messageShot = NULL;
                        messageShot = calloc(11, sizeof(char));
                        strcat(messageShot, "my;already;");
                        strcat(messageShot, &x);
                        strcat(messageShot, &y);
                        strcat(messageShot, "\n");
                        send(socket, messageShot, 14, 0);
                    }
                }
                break;
            }
        }
    }
}

void sendGameResult(int winnerSocket, int loserSocket) {
    char winMessage[] = "won\n";
    send(winnerSocket, winMessage, 4, 0);

    char loseMessage[] = "lost\n";
    send(loserSocket, loseMessage, 5, 0);

}

void lostConnectionToPlayer(int socket){

    for (int i = 0; i < MAX_PLAYER_COUNT; ++i) {
        if (PLAYERS[i] != NULL && socket == PLAYERS[i]->playerSocket){
            PLAYERS[i]->connected = false;
            break;
        }
    }

    for (int j = 0; j < MAX_ROOMS; ++j) {
        if (GAMES[j] != NULL && GAMES[j]->player1->playerSocket == socket){
            GAMES[j]->player1Connected = false;
            if (GAMES[j]->gameStatus == 0){
                GAMES[j] = NULL;
            }
            break;
        }
        if (GAMES[j] != NULL && GAMES[j]->player2->playerSocket == socket){
            GAMES[j]->player2Connected = false;
            if (GAMES[j]->gameStatus == 0){
                GAMES[j] = NULL;
            }
            break;
        }
    }
}

void setShips(int socket, char *name, char *room, char *placing) {
    for (int i = 0; i < MAX_ROOMS; ++i) {
        if (GAMES[i] != NULL && !strcmp(GAMES[i]->gameOwnerNick, room)){
            if (!strcmp(GAMES[i]->player1->nick, name)){
                for (int j = 0; j < 6; ++j) {
                    for (int k = 0; k < 6; ++k) {
                        GAMES[i]->gridPlayer1[j][k] = placing[j * 6 + k] - 48;
                    }

                }
                GAMES[i]->player1Grid = true;
                send(socket,"place\n", 6, 0);
                logSent(6);
            }
            if (!strcmp(GAMES[i]->player2->nick, name)){
                for (int j = 0; j < 6; ++j) {
                    for (int k = 0; k < 6; ++k) {
                        GAMES[i]->gridPlayer2[j][k] = placing[j * 6 + k] - 48;
                    }
                }
                GAMES[i]->player2Grid = true;
                printf("Nastaven grid pro player2\n");
                send(socket,"place\n", 6, 0);
                logSent(6);
            }

            if (GAMES[i]->player2Grid && GAMES[i]->player1Grid){
                GAMES[i]->gameStatus = 2;
                GAMES[i]->playerTurn = 1;

                send(GAMES[i]->player1->playerSocket, "gamestart;0\n",12, 0);
                send(GAMES[i]->player2->playerSocket, "gamestart\n",10, 0);
            }
        }
    }

}

void joinRoom(int socket, char *name, char *room) {
    char *message;
    message = calloc(68, sizeof(char));
    strcat(message, "joined;");
    for (int i = 0; i < MAX_ROOMS; ++i) {
        if (GAMES[i] != NULL && !strcmp(GAMES[i]->gameOwnerNick, room)){
            if (GAMES[i]->gameStatus == 0){
                if (GAMES[i]->playerCount < 2){
                    GAMES[i]->playerCount++;
                    GAMES[i]->player2 = findPlayerByName(name);
                    GAMES[i]->gameStatus = 1;
                    GAMES[i]->player2Connected = true;
                    if (!GAMES[i]->player2){
                        send(socket, "joinERR\n", 8, 0);
                        logSent(8);
                        printf("Nepripojen hrac %s\n", name);
                    } else {
                        strcat(message, GAMES[i]->gameOwnerNick);
                        strcat(message, ";");
                        strcat(message, GAMES[i]->player1->nick);
                        strcat(message, "\n");
                        send(socket, message, 68, 0);
                        logSent(68);
                        printf("Send to player2 %i %s\n", socket, message);
                        notifyJoin(GAMES[i]->player1->playerSocket, GAMES[i]->player2->nick);
                    }
                } else {
                    send(socket, "joinERR\n", 8, 0);
                    logSent(8);
                    printf("Nepripojen hrac %s\n", name);
                }
            } else {
                send(socket, "joinERR\n", 8, 0);
                logSent(8);
                printf("Nepripojen hrac %s\n", name);
            }
        }
    }
}

void notifyJoin(int socket, char *player) {
    char *message2;
    message2 = calloc(43, sizeof(char));
    strcat(message2, "prepareGame;");
    strcat(message2, player);
    strcat(message2, "\n");
    send(socket, message2, 43,0);
    logSent(43);
    printf("Send to player1 %i %s\n", socket, message2);
}

void requestRooms(int socket) {
    char *message;
    message = calloc(30 * MAX_ROOMS + 17, sizeof(char));
    strcat(message, "rooms;");
    for (int i = 0; i < MAX_ROOMS; ++i) {
        if (GAMES[i] != NULL && GAMES[i]->playerCount < 2 && GAMES[i]->gameStatus == 0){
            //printf("Room %s \n", GAMES[i]->gameOwnerNick);
            strcat(message, GAMES[i]->gameOwnerNick);
            strcat(message, ",");
        }
    }
    strcat(message, "\n");
    send(socket, message, strlen(message), 0);
    printf("Sent to player %s \n", message);
    logSent(strlen(message));
    free(message);
    //send(socket, "maxErr\n", 7, 0);
}

void initRoom(int socket, char *ownerName) {
    if (ACTIVE_ROOMS == MAX_PLAYER_COUNT){
        printf("Maximum rooms on server!\n");
        send(socket, "maxErr\n", 7, 0);
        logSent(7);
    }

    for (int i = 0; i < MAX_ROOMS; i++) {
        if ((GAMES[i] != NULL) && (!strcmp(GAMES[i]->gameOwnerNick, ownerName))) {
            printf("Room: %s already exists.\n", ownerName);
            send(socket, "roomERR\n", 8, 0);
            logSent(8);
            return;
        }
    }

    game *newGame = createNewGame(findPlayerByName(ownerName));
    if (!newGame) {
        printf("Error. Couldn't create new room.\n");
        send(socket, "roomERR\n", 8, 0);
        logSent(8);
        return;
    }

    for (int j = 0; j < MAX_ROOMS; ++j) {
        if (GAMES[j] == NULL){
            GAMES[j] = newGame;
            ACTIVE_ROOMS++;
            send(socket, "roomCreat\n", 10, 0);
            logSent(10);
            break;
        }
    }

}

void playerLogout(int socket, char *name) {
    for (int i = 0; i < MAX_PLAYER_COUNT; ++i) {
        if (PLAYERS[i] != NULL && !strcmp(PLAYERS[i]->nick, name)){
            player *temporary = PLAYERS[i];
            //free(&temporary);
            PLAYERS[i] = NULL;
            printf("User %s logged off \n", name);
            PLAYER_COUNT--;
            close(socket);
            return;
        }
    }
    printf("Logout PROBLEM with user %s \n", name);
}

void playerLogin(int socket, char *name) {
    if (PLAYER_COUNT == MAX_PLAYER_COUNT){
        printf("Maximum players on server!\n");
        send(socket, "logErr\n", 7, 0);
        logSent(7);
        return;
    }

    for (int i = 0; i < MAX_PLAYER_COUNT; ++i) {
        if (PLAYERS[i] != NULL && !strcmp(PLAYERS[i]->nick, name)){
            if (!findGameWithInactivePlayer(name)){
                printf("Username already taken!\n");
                send(socket, "nickTaken\n", 10, 0);
                logSent(10);
                return;
            } else if (findGameWithInactivePlayer(name)){
                sendReconnectMessage(socket, name);
                return;
            }
        }
    }

    player *newPlayer = createPlayer(socket, name);

    if (!newPlayer){
        printf("Server ERROR\n");
        send(socket, "logErr\n", 7, 0);
        logSent(7);
    }

    for (int j = 0; j < MAX_PLAYER_COUNT; ++j) {
        if (!PLAYERS[j]){
            PLAYERS[j] = newPlayer;
            break;
        }
    }

    PLAYER_COUNT++;
    printf("User %s logged in!\n", name);
    send(socket, "logged\n", 7, 0);
    logSent(7);
}

void sendReconnectMessage(int socket, char *name) {
    for (int i = 0; i < MAX_ROOMS; ++i) {
        if (GAMES[i] != NULL && !strcmp(GAMES[i]->player1->nick, name)){
            GAMES[i]->player1->playerSocket = socket;
            GAMES[i]->player1Connected = true;
            char *message = calloc(150, sizeof(char));
            if (GAMES[i]->gameStatus == 1){
                //send change to placing stage (activePlacing;roomname\n)
                strcat(message, "activePlacing;");
                strcat(message, GAMES[i]->gameOwnerNick);
                strcat(message, ";\n");
                send(socket, message, 150, 0);
                logSent(150);
                break;
            } else if (GAMES[i]->gameStatus == 2){
                //send game state (activeGame;roomname;myplacing;enemyplacing;turn\n)
                strcat(message, "activeGame;");
                strcat(message, GAMES[i]->gameOwnerNick);
                strcat(message, ";");
                for (int j = 0; j < 6; ++j) {
                    for (int k = 0; k < 6; ++k) {
                        strcat(message, (GAMES[i]->gridPlayer1[j][k] + 48));
                    }
                }
                strcat(message, ";");
                for (int l = 0; l < 6; ++l) {
                    for (int j = 0; j < 6; ++j) {
                        if (GAMES[i]->gridPlayer2[l][j] == 1){
                            strcat(message, "0");
                        } else{
                            strcat(message, (GAMES[i]->gridPlayer2[l][j] + 48));
                        }
                    }
                }
                if (GAMES[i]->playerTurn == 1){
                    strcat(message, ";1;\n");
                } else{
                    strcat(message, ";0;\n");
                }
                send(socket, message, 150, 0);
                logSent(150);
                break;
            }
        }
        if (GAMES[i] != NULL && !strcmp(GAMES[i]->player2->nick, name)){
            GAMES[i]->player2->playerSocket = socket;
            GAMES[i]->player2Connected = true;
            char *message = calloc(150, sizeof(char));
            if (GAMES[i]->gameStatus == 1){
                //send change to placing stage (activePlacing;roomname\n)
                strcat(message, "activePlacing;");
                strcat(message, GAMES[i]->gameOwnerNick);
                strcat(message, ";\n");
                send(socket, message, 150, 0);
                logSent(150);
                break;
            } else if (GAMES[i]->gameStatus == 2){
                //send game state (activeGame;roomname;myplacing;enemyplacing;turn\n)
                strcat(message, "activeGame;");
                strcat(message, GAMES[i]->gameOwnerNick);
                strcat(message, ";");
                for (int j = 0; j < 6; ++j) {
                    for (int k = 0; k < 6; ++k) {
                        strcat(message, (GAMES[i]->gridPlayer2[j][k] + 48));
                    }
                }
                strcat(message, ";");
                for (int l = 0; l < 6; ++l) {
                    for (int j = 0; j < 6; ++j) {
                        if (GAMES[i]->gridPlayer1[l][j] == 1){
                            strcat(message, "0");
                        } else{
                            strcat(message, (GAMES[i]->gridPlayer1[l][j] + 48));
                        }
                    }
                }
                if (GAMES[i]->playerTurn == 2){
                    strcat(message, ";1;\n");
                } else{
                    strcat(message, ";0;\n");
                }
                send(socket, message, 150, 0);
                logSent(150);
                break;
            }
        }
    }
}

void socketCut(int socket){
    for (int i = 0; i < MAX_PLAYER_COUNT; ++i) {
        if (PLAYERS[i] != NULL){
            for (int j = 0; j < MAX_ROOMS; ++j) {
                if (GAMES[i] != NULL && GAMES[i]->player1 != NULL && GAMES[i]->player1->playerSocket == socket){
                    if (GAMES[i]->player2 != NULL){
                        send(GAMES[i]->player2->playerSocket, "won;rooms\n", 4, 0);
                        GAME_COUNTER--;
                        GAMES[i] = NULL;
                        break;
                    }
                } else if (GAMES[i] != NULL && GAMES[i]->player2 != NULL && GAMES[i]->player2->playerSocket == socket) {
                    if (GAMES[i]->player1 != NULL) {
                        send(GAMES[i]->player1->playerSocket, "won;rooms\n", 4, 0);
                        GAME_COUNTER--;
                        GAMES[i] = NULL;
                        break;
                    }
                }
            }
            if (PLAYERS[i]->playerSocket == socket){
                printf("User %s disconnected due violations against server policy!\n", PLAYERS[i]->nick);
                //freePlayer(&PLAYERS[i]);
                PLAYER_COUNT--;
                PLAYERS[i] = NULL;
            }
        }
    }
}
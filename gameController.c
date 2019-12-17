//
// Created by hlavja on 17/12/2019.
//

#include "gameController.h"
#include "globalVariable.h"
#include "game.h"
#include "player.h"
#include "logger.h"

void lostConnectionToPlayer(int socket){

}

int parseMessage(int socket, char *msg) {
    long int type;
    char *name, *room, t[2], *place, *x, *y;
    t[0] = msg[0];
    t[1] = msg[1];
    type = strtol(t, NULL, 10);
    printf("%s\n", msg);
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
        /*case 5:
            pthread_rwlock_rdlock(&lock);
            printf("Received join room request: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            join_room(socket, name, room);
            pthread_rwlock_unlock(&lock);
            return 5;
        case 6:
            pthread_rwlock_rdlock(&lock);
            printf("Received leave room request: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            leave_room(socket, name, room);
            pthread_rwlock_unlock(&lock);
            return 6;
        case 7:
            pthread_rwlock_rdlock(&lock);
            printf("Received start prepare stage request: %s\n", msg);
            start_prep(socket, msg + 2);
            pthread_rwlock_unlock(&lock);
            return 7;
        case 8:
            pthread_rwlock_rdlock(&lock);
            printf("Received ship placement: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            place = strtok(NULL, ";");
            // printf("Place %s\n", place);
            set_placement(socket, name, room, place);
            pthread_rwlock_unlock(&lock);
            return 8;
        case 9:
            pthread_rwlock_rdlock(&lock);
            printf("Received start game request: %s\n", msg);
            start_game(socket, msg + 2);
            pthread_rwlock_unlock(&lock);
            return 9;
        case 10:
            pthread_rwlock_rdlock(&lock);
            printf("Received shoot message: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            x = strtok(NULL, ";");
            y = strtok(NULL, ";");
            shoot1(socket, name, room, *x, *y);
            pthread_rwlock_unlock(&lock);
            return 10;
        case 11:
            pthread_rwlock_rdlock(&lock);
            printf("Received leave game: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            leave_game(socket, name, room);
            pthread_rwlock_unlock(&lock);
            return 11;
        case 12:
            pthread_rwlock_rdlock(&lock);
            printf("Received rematch message: %s\n", msg);
            name = strtok(msg + 2, ";");
            room = strtok(NULL, ";");
            rematch(name, room);
            pthread_rwlock_unlock(&lock);
            return 12;
        case 13:
            pthread_rwlock_rdlock(&lock);
            printf("Received ping message, sending response.\n");
            send(socket, "alive\n", 6, 0);
            pthread_rwlock_unlock(&lock);
            return 13;*/
        case 14:
            printf("Received ping response from socket: %d\n", socket);
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

void requestRooms(int socket) {
    //TODO
    send(socket, "maxErr\n", 7, 0);
}

void initRoom(int socket, char *ownerName) {
    if (ACTIVE_ROOMS == MAX_PLAYER_COUNT){
        printf("Maximum rooms on server!");
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
            printf("User %s logged off", name);
            PLAYER_COUNT--;
            close(socket);
            return;
        }
    }
    printf("Logout PROBLEM with user %s", name);
}

void playerLogin(int socket, char *name) {
    if (PLAYER_COUNT == MAX_PLAYER_COUNT){
        printf("Maximum players on server!");
        send(socket, "logErr\n", 7, 0);
        logSent(7);
    }

    for (int i = 0; i < MAX_PLAYER_COUNT; ++i) {
        if (PLAYERS[i] != NULL && !strcmp(PLAYERS[i]->nick, name)){
            printf("Username already taken!");
            send(socket, "nickTaken\n", 10, 0);
            logSent(10);
        }
    }

    player *newPlayer = createPlayer(socket, name);

    if (!newPlayer){
        printf("Server ERROR");
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
    printf("User %s logged in!", name);
    send(socket, "logged\n", 7, 0);
    logSent(7);
}

void socketCut(int socket){
    for (int i = 0; i < MAX_PLAYER_COUNT; ++i) {
        if (PLAYERS[i] != NULL){
            if (PLAYERS[i]->playerSocket == socket){
                printf("User %s disconnected due violations against server policy!", PLAYERS[i]->nick);
                //freePlayer(&PLAYERS[i]);
                PLAYER_COUNT--;
                PLAYERS[i] = NULL;
            }
        }
    }
}
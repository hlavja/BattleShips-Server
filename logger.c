//
// Created by hlavja on 12/12/2019.
//

#include "logger.h"


sem_t mutex;


void serverLoggingStart(){
    time_t actualTime;
    actualTime = time(NULL);

    sem_init(&mutex, 0, 1);

    SERVER_START = time(NULL);
    LOG_FILE = fopen("server.log", "w");
    fclose(LOG_FILE);
    LOG_FILE = fopen("server.log", "a+");

    fprintf(LOG_FILE, "Server startuje v case %s", ctime(&actualTime));
    fprintf(LOG_FILE, "Server bezi na adrese %s a portu %d.\n", IP_ADDRESS, SERVER_PORT);

    fclose(LOG_FILE);
}

void logMsg(char *msg) {
    LOG_FILE = fopen("server.log", "a+");
    fprintf(LOG_FILE, "%s", msg);
    fclose(LOG_FILE);
}

void logSent(int bytes){
    sem_wait(&mutex);
    MESSAGE_SENT++;
    BYTE_SENT = BYTE_SENT + bytes;
    sem_post(&mutex);
}


void logReceive(int bytes){
    sem_wait(&mutex);
    MESSAGE_RECV++;
    BYTE_RECV = BYTE_RECV + bytes;
    sem_post(&mutex);
}

void logServerShutdown(){
    time_t cas;
    cas = time(NULL);
    int total_time;

    LOG_FILE = fopen("server.log", "a+");

    SERVER_CLOSE = time(NULL);
    total_time = (int) difftime(SERVER_CLOSE, SERVER_START);

    fprintf(LOG_FILE, "Server byl ukoncen v case %s", ctime(&cas));
    fprintf(LOG_FILE, "Server bezel %d sekund.\n", total_time);
    fprintf(LOG_FILE, "-------------------------\n");
    fprintf(LOG_FILE, "STATISTIKA\n");
    fprintf(LOG_FILE, "-------------------------\n");
    fprintf(LOG_FILE, "Celkovy pocet odehranych her: %ld\n", GAME_COUNTER - 1);
    fprintf(LOG_FILE, "Celkovy pocet pripojenych klientu: %d\n", CLIENT_COUNT);
    fprintf(LOG_FILE, "Celkovy pocet odeslanych zprav: %ld\n", MESSAGE_SENT);
    fprintf(LOG_FILE, "Celkovy pocet odeslanych bytu: %ld\n", BYTE_SENT);
    fprintf(LOG_FILE, "Celkovy pocet prijatych zprav: %ld\n", MESSAGE_RECV);
    fprintf(LOG_FILE, "Celkovy pocet prijatych bytu: %ld\n", BYTE_RECV);
    sem_destroy(&mutex);
}
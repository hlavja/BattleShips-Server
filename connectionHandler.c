//
// Created by hlavja on 12/12/2019.
//
#include "connectionHandler.h"

void *connectionHandler(void *arg){
    int client_sock, val, size_rec, res;
    char msg[200], msg_size[3];
    client_sock = *(int *) arg;
    int missed_ping = 0;

    while (1) {
        memset(msg, '\0', sizeof(msg));
        memset(msg_size, '\0', sizeof(msg_size));
        val = recv(client_sock, msg_size, 3, 0);

        while (val < 0 && missed_ping < 12) {

            if (missed_ping == 1) {
                printf("AAAAASending lost con notify.\n");
                //lostcontopl(client_sock);
            }
            send(client_sock, "ping\n", 5, 0);
            printf("Timeout. Sending ping to socket: %d\n", client_sock);
            val = recv(client_sock, msg_size, 3, 0);

            missed_ping++;
        }

        if (missed_ping == 12) {
            //logoffbysock(client_sock);
            printf("Closing connection. Socket: %d\n", client_sock);
            free(arg);
            return 0;
        }

        missed_ping = 0;

        size_rec = strtol(msg_size, NULL, 10);

        if (size_rec > 0) {
            val = recv(client_sock, msg, size_rec, 0);
            //bytes_in += (3);
        }
        if (val < 0) {
            continue;
        }

        res = 2;//parseMessage(client_sock, msg);
        if (res == 2) {
            close(client_sock);
            free(arg);
            break;
        }

        if(val == 0) {
            printf("Connection closed.\n");
            //logoffbysock(client_sock);
            close(client_sock);
            free(arg);
            break;
        }

        if (res == 0) {
            printf("Message not recognized\n");
            //logoffbysock(client_sock);
            close(client_sock);
            free(arg);
            break;
        }
    }
    //printf("Bytes in:%d\n", bytes_in);
    //printf("Bytes out:%d\n", bytes_out);
    return 0;
}
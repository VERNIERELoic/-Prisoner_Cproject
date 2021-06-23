/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: aurelio
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <stdbool.h>

#include "srvcxnmanager.h"
#include "utils.h"
#include "struct.h"

config_action_server config_s;

/*
 *
 */
int main(int argc, char** argv) 
{
    config_s = read_config("config.ini");
    
    int sockfd = -1;
    int index = 1;
    connection_t *connection;
    pthread_t thread;

    // init protocole
    initProtocole();
    /* init array*/
    logs("initialisation des connections");
    init_sockets_array();
    /* create socket */
    sockfd = create_server_socket();

    /* listen on port , stack size 5accept0 for incoming connections*/
    if (listen(sockfd, 50) < 0) {
        logs("Erreur : impossible d'écouter le port");
        fprintf(stderr, "%s: error: cannot listen on port\n", argv[0]);
        return -5;
    }

    logs("écoute..");
    printf("ready and listening\n");

    //Wait for connection
    while (true) {
        /* accept incoming connections */
        connection = (connection_t *) malloc(sizeof (connection_t));
        connection->sockfd = accept(sockfd, &connection->address, &connection->addr_len);
        connection->index = index++;
        if (connection->sockfd <= 0) {
            free(connection);
        } else {
            /* start a new thread but do not wait for it */
            pthread_create(&thread, 0, threadProcess, (void *) connection);
            pthread_detach(thread);
        }
    }
    return (EXIT_SUCCESS);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include "clientcxnmanager.h"
#include "view_action.h"
#include "utils.h"
#include "struct.h"

extern config_client config_c;
extern bool read_file;
extern GtkWidget *win;
int round_nb;
int round_id;
bool round_win;
int round_score;
int score;
int win_or_lose;

/**
 * \brief Récuperation des paquets du serveur et traitement
 * \param ptr int
 * \return void
 */
void *threadProcess(void *ptr)
{
    char buffer_in[BUFFERSIZE];
    int sockfd = *((int *)ptr);
    int len;

    while ((len = read(sockfd, buffer_in, BUFFERSIZE)) != 0)
    {
        if (strncmp(buffer_in, "exit", 4) == 0)
        {
            break;
        }

        game_start game_s;
        round_start round_s;
        send_round_result send_round_res;
        send_game_result send_game_res;

        int *buffer_clone = buffer_in;

        if (*buffer_clone == 3)
        {
            memcpy(&game_s, buffer_in, sizeof(game_start));
            round_nb = game_s.round_nb;
        }
        else if (*buffer_clone == 4)
        {
            memcpy(&round_s, buffer_in, sizeof(round_start));
            round_id = round_s.round_id;
        }
        else if (*buffer_clone == 6)
        {
            memcpy(&send_round_res, buffer_in, sizeof(send_round_result));
            round_win = send_round_res.win_or_lose;
            round_score = send_round_res.score;
        }
        else if (*buffer_clone == 7)
        {
            memcpy(&send_game_res, buffer_in, sizeof(send_game_result));
            win_or_lose = send_game_res.win_or_lose;
            score = send_game_res.score;
        }
        what_to_do(*buffer_clone);

        memset(buffer_in, '\0', BUFFERSIZE);
        memset(buffer_clone, '\0', BUFFERSIZE);
    }
    close(sockfd);
    gtk_window_close(win);
}

/**
 * \brief Ouvre une connexion avec le serveur
 * \return sockfd Numéro du socket
 */
int open_connection()
{
    int sockfd;

    struct sockaddr_in serverAddr;
    int port = read_file == false ? 7799 : atoi(config_c.port);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = read_file == false ? inet_addr("127.0.0.1") : inet_addr(config_c.host);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0)
    {
        printf("Fail to connect to server\n");
        exit(-1);
    };

    return sockfd;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "srvcxnmanager.h"
#include "struct.h"
#include "utils.h"

extern config_action_server config_s;
connection_t *connections[MAXSIMULTANEOUSCLIENTS];
char choice_queue[SIZE_QUEUE];
int index_choice_queue = 0;  
char queue[SIZE_QUEUE];
int waiting_index = 0; 
char lastAction;

extern connection_status confirm_connection_status; 
extern game_start game_start_protocole;
extern round_start start_round_protocole;
extern send_round_result send_round_result_client1;
extern send_round_result send_round_result_client2;
extern send_game_result send_result_game_client1;
extern send_game_result send_result_game_client2;
int round_nb = 1;

/**
 * \brief initialisation du tableau de sockets
 * \return void
 */
void init_sockets_array()
{
    logs("initialisation du tableau de sockets");
    for (int i = 0; i < MAXSIMULTANEOUSCLIENTS; i++)
    {
        connections[i] = NULL;
    }
}

/**
 *\brief Ajoute une connexion au tableau de sockets
 *\param connection Connexion à ajouter
 *\return void
 */
void add(connection_t *connection)
{
    logs("ajout d'une connexion au tableau de sockets");
    for (int i = 0; i < MAXSIMULTANEOUSCLIENTS; i++)
    {
        if (connections[i] == NULL)
        {
            connections[i] = connection;
            return;
        }
    }
    perror("Too much simultaneous connections");
    logs("Erreur : trop de connexions simultanées");
    exit(-5);
}

/**
 *\brief Supprime une connexion au tableau de sockets
 *\param connection Connexion à supprimer
 *\return void
 */
void del(connection_t *connection)
{
    logs("suppression d'une connexion du tableau de sockets");
    for (int i = 0; i < MAXSIMULTANEOUSCLIENTS; i++)
    {
        if (connections[i] == connection)
        {
            connections[i] = NULL;
            return;
        }
    }
    perror("Connection not in pool ");
    logs("Erreur : la connexion n'existe pas et ne peut donc pas être supprimée");
    exit(-5);
}

/**
 *\brief Récupère la position du client dans la queue
 *\param client_id Id du client
 *\return index Position du client dans la queue
 */
int getIndexFromIdClient(char client_id)
{
    logs("récuperation de la position du client dans la queue");
    int index = -1;
    for (int i = 0; i < waiting_index; i++)
    {
        if (queue[i] == client_id)
        {
            index = i;
            break;
        }
    }
    return index;
}

/**
 *\brief Retourne l'adversaire du joueur
 *\param client_id Id du client
 *\return retour Id du joueur adverse
 */
char get_frontplayer_id(char client_id) 
{
    char retour = '0';
    switch (client_id)
    {
    case '1':
        retour = '2';
        break;
    case '2':
        retour = '1';
        break;
    }
    return retour;
}

/**
 *\brief Ajout d'un client dans la queue
 *\param client_id Id du client
 *\return void
 */
void addQueue(char client_id)
{
    logs("ajout d'un id de client dans la queue (en attente d'un adversaire)");
    queue[waiting_index] = client_id;
    waiting_index++;
}

/**
 *\brief Ajout d'un client dans la choice queue
 *\param client_id Id du client
 *\return void
 */
void waiting_queue(char client_id) 
{
    logs("ajout de l'id du client dans choice_queue (en attente du choix de l'adversaire)"); 
    choice_queue[index_choice_queue] = client_id;
    index_choice_queue++;
}

/**
 * \brief Détermine si l'adversaire du client est dans la queue
 * \param client_id Id du client
 * \return retour : 1 si l'adversaire est dans la queue, 0 sinon
 */
int frontplayer_in_queue(char client_id)
{
    bool retour = false;
    for (int i = 0; i < waiting_index; i++)
    {
        if (queue[i] == get_frontplayer_id(client_id))
        {
            retour = true;
            break;
        }
    }
    return retour;
}

/**
 * \brief Détermine si l'adversaire du client est dans la choice queue
 * \param client_id l'id du client
 * \return retour : 1 si l'adversaire est dans la queue, 0 sinon
 */
int waiting_frontplayer_choice(char client_id) 
{
    bool retour = false;
    for (int i = 0; i < index_choice_queue; i++)
    {
        if (choice_queue[i] == get_frontplayer_id(client_id))
        {
            retour = true;
            break;
        }
    }
    return retour;
}

/**
 * \brief Réinitialise la queue
 * \return void
 */
void restart_queue() //restart_queue
{
    logs("réinitialisation de la queue");
    for (int i = 0; i < waiting_index; i++)
    {
        queue[i] = '\0';
    }
    waiting_index = 0;
}

/**
 * \brief Réinitialise la choice queue
 * \return void
 */
void restart_waiting_choice() 
{
    logs("réinitialisation de choice_queue");
    for (int i = 0; i < index_choice_queue; i++)
    {
        choice_queue[i] = '\0';
    }
    index_choice_queue = 0;
}

/**
 * \brief Thread permettant au server de gérer plusieurs connexions clients simultanées
 * \param ptr connection_t
 * \return void
 */
void *threadProcess(void *ptr)
{
    char buffer_in[BUFFERSIZE];
    char log[300];
    int len;
    connection_t *connection;

    if (!ptr)
        pthread_exit(0);
    connection = (connection_t *)ptr;
    logs("nouvelle connexion");
    printf("New incoming connection \n");

    add(connection);

    //Welcome the new client
    printf("Welcome #%i\n", connection->index);

    while ((len = read(connection->sockfd, buffer_in, BUFFERSIZE)) > 0)
    {
        if (strncmp(buffer_in, "bye", 3) == 0)
        {
            break;
        }

        int *buffer_clone = (int *)buffer_in;
        connection_client connectionProtocole;
        send_choice send_choice_protocole;

        switch (*buffer_clone)
        {
        case 1:
            memcpy(&connectionProtocole, buffer_in, sizeof(connection_client));
            if (frontplayer_in_queue(connectionProtocole.client_id) == true)
            {
                write(connection->sockfd, &game_start_protocole, sizeof(game_start));
                write(connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->sockfd, &game_start_protocole, sizeof(game_start));
            }
            else
            {
                addQueue(connectionProtocole.client_id);
                write(connection->sockfd, &confirm_connection_status, sizeof(connection_status));
            }
            break;
            


        case 5:
                    memcpy(&send_choice_protocole, buffer_in, sizeof(send_choice));
            if (waiting_frontplayer_choice(connectionProtocole.client_id) == true)
            {
                if (lastAction == true && send_choice_protocole.coop == true)
                {
                    sprintf(log, "client %c a partagé en %d secondes", connectionProtocole.client_id, send_choice_protocole.time_to_choose);
                    logs(log);
                    sprintf(log, "client %c a partagé en %d secondes", get_frontplayer_id(connectionProtocole.client_id), send_choice_protocole.time_to_choose);
                    logs(log);
                    send_round_result_client1.result = true;
                    send_round_result_client1.nbPoints = config_s.two_coop;
                    send_round_result_client2.result = true;
                    send_round_result_client2.nbPoints = config_s.two_coop;
                    connection->nbPoints += config_s.two_coop;
                    connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->nbPoints += config_s.two_coop;
                    write(connection->sockfd, &send_round_result_client1, sizeof(send_round_result));
                    write(connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->sockfd, &send_round_result_client2, sizeof(send_round_result));
                }
                else if (lastAction == true && send_choice_protocole.coop == false)
                {
                    // ECRIRE DANS LES LOGS LE RESULTAT + PRINT SUR L'INTERFACE
                    sprintf(log, "client %c a pris en %d secondes", connectionProtocole.client_id, send_choice_protocole.time_to_choose);
                    logs(log);
                    sprintf(log, "client %c a partagé en %d secondes", get_frontplayer_id(connectionProtocole.client_id), send_choice_protocole.time_to_choose);
                    logs(log);
                    send_round_result_client1.result = true;
                    send_round_result_client1.nbPoints = config_s.one_btray_1coop;
                    send_round_result_client2.result = false;
                    send_round_result_client2.nbPoints = config_s.one_coop_1btray;
                    connection->nbPoints += config_s.one_btray_1coop;
                    connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->nbPoints += config_s.one_coop_1btray;
                    write(connection->sockfd, &send_round_result_client1, sizeof(send_round_result));
                    write(connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->sockfd, &send_round_result_client2, sizeof(send_round_result));
                }
                else if (lastAction == false && send_choice_protocole.coop == true)
                {
                    sprintf(log, "client %c a partagé en %d secondes", connectionProtocole.client_id, send_choice_protocole.time_to_choose);
                    logs(log);
                    sprintf(log, "client %c a pris en %d secondes", get_frontplayer_id(connectionProtocole.client_id), send_choice_protocole.time_to_choose);
                    logs(log);
                    send_round_result_client1.result = false;
                    send_round_result_client1.nbPoints = config_s.one_coop_1btray;
                    send_round_result_client2.result = true;
                    send_round_result_client2.nbPoints = config_s.one_btray_1coop;
                    connection->nbPoints += config_s.one_coop_1btray;
                    connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->nbPoints += config_s.one_btray_1coop;
                    write(connection->sockfd, &send_round_result_client1, sizeof(send_round_result));
                    write(connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->sockfd, &send_round_result_client2, sizeof(send_round_result));
                }
                else if (lastAction == false && send_choice_protocole.coop == false)
                {
                    sprintf(log, "client %c a pris en %d secondes", connectionProtocole.client_id, send_choice_protocole.time_to_choose);
                    logs(log);
                    sprintf(log, "client %c a pris en %d secondes", get_frontplayer_id(connectionProtocole.client_id), send_choice_protocole.time_to_choose);
                    logs(log);
                    send_round_result_client1.result = false;
                    send_round_result_client1.nbPoints = config_s.two_btray;
                    send_round_result_client2.result = false;
                    send_round_result_client2.nbPoints = config_s.two_btray;
                    connection->nbPoints += config_s.two_btray;
                    connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->nbPoints += config_s.two_btray;
                    write(connection->sockfd, &send_round_result_client1, sizeof(send_round_result));
                    write(connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->sockfd, &send_round_result_client2, sizeof(send_round_result));
                }
                sleep(2);
                round_nb++;
                start_round_protocole.roundId = round_nb;
                if (round_nb > config_s.round_nb)
                {
                    send_result_game_client1.nbPoints = connection->nbPoints;
                    send_result_game_client1.nbPoints = connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->nbPoints;
                    if (send_result_game_client1.nbPoints > send_result_game_client1.nbPoints)
                    {
                        send_result_game_client1.result = 1;
                        send_result_game_client1.result = 0;
                    }
                    else if (send_result_game_client1.nbPoints < send_result_game_client1.nbPoints)
                    {
                        send_result_game_client1.result = 0;
                        send_result_game_client1.result = 1;
                    }
                    else
                    {
                        send_result_game_client1.result = -1;
                        send_result_game_client1.result = -1;
                    }
                    write(connection->sockfd, &send_result_game_client1, sizeof(round_start));
                    write(connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->sockfd, &send_result_game_client1, sizeof(round_start));
                }
                else
                {
                    write(connection->sockfd, &start_round_protocole, sizeof(round_start));
                    write(connections[getIndexFromIdClient(get_frontplayer_id(connectionProtocole.client_id))]->sockfd, &start_round_protocole, sizeof(round_start));
                }
                restart_waiting_choice();
            }
            else
            {
                waiting_queue(connectionProtocole.client_id);
                lastAction = send_choice_protocole.coop;
            }
            //clear input buffer
            memset(buffer_in, '\0', BUFFERSIZE);
            break;

        default:
        logs("erreur dans switch");
        break;
        }


    }
    round_nb = 1;
    logs("la connexion au client s'est terminée");
    restart_waiting_choice();
    restart_queue();
    printf("Connection to client %i ended \n", connection->index);
    close(connection->sockfd);
    del(connection);
    free(connection);
    pthread_exit(0);
}

/**
 * \brief Création du socket entre le serveur et le client
 * \return sockfd Numéro du socket
 */
int create_server_socket()
{
    logs("création du flux entre le serveur et le client");
    int sockfd = -1;
    struct sockaddr_in address;


    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd <= 0)
    {
        logs("Erreur : impossible de créer le socket");
        fprintf(stderr, "%s: error: cannot create socket\n");
        return -3;
    }

    int port = atoi(config_s.port);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(config_s.host);
    address.sin_port = htons(port);


    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse));


    if (bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
    {
        logs("Erreur : impossible de lier le socket au port");
        fprintf(stderr, "error: cannot bind socket to port %d\n", port);
        return -4;
    }

    return sockfd;
}
#ifndef SRV_CXN_MANAGER_H
#define SRV_CXN_MANAGER_H




#define BUFFERSIZE 2048
#define MAXSIMULTANEOUSCLIENTS 100
#define SIZE_QUEUE 50

typedef struct {
    int sockfd;
    struct sockaddr address;
    int addr_len;
    int index;
    int nbPoints;
} connection_t;

typedef struct {
    int id;
    int msg;
} prot_msg;


void init_sockets_array();
void add(connection_t *connection);
void del(connection_t *connection);
void *threadProcess(void *ptr);
int create_server_socket() ;

#endif /* SRV_CXN_MANAGER_H */


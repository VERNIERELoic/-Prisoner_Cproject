#ifndef UTILS_H
#define UTILS_H

#define SIZE 100

typedef struct
{
    char host[SIZE];
    char port[SIZE];
    char client_id[SIZE];
} config_client;

typedef struct
{
    char key[SIZE];
    char value[SIZE];
} option;

config_client read_config(char filename[]);
config_client cp_line(config_client config_c, option optn);
option new_array(char line[]);
void init_item(option item);

#endif /* UTILS_H */

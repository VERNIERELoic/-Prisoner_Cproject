
#ifndef UTILS_H
#define UTILS_H

#define SIZE_CHAR 100

typedef struct {
    char host[SIZE_CHAR];
    char port[SIZE_CHAR];
    int round_nb; 
    int two_btray;
    int two_coop; 
    int one_btray_1coop; 
    int one_coop_1btray; 
} config_action_server;

typedef struct {
    char key[SIZE_CHAR];
    char value[SIZE_CHAR];
}option; 


void logs(char message[]);
config_action_server read_config(char filename[]); 
void init_option(option opt);
config_action_server fillConfigDataServeur(config_action_server config_s, option opt);
option createOptionFromArray(char line[]);

#endif /* UTILS_H */


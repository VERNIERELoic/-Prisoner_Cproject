#ifndef STRUCT_H
#define STRUCT_H

#define SIZE 100

typedef struct
{
    int action_id;
    char client_id;
} connection_server;

typedef struct
{
    int action_id;
    bool status;
    char msg[SIZE];
} connection_status;

typedef struct
{
    int action_id;
    int round_nb;
} game_start;

typedef struct
{
    int action_id;
    int round_id;
} round_start;

typedef struct
{
    int action_id;
    bool coop;
    int time_choice;
} send_choice;

typedef struct
{
    int action_id;
    bool win_or_lose;
    int score;
} send_round_result;

typedef struct
{
    int action_id;
    int win_or_lose;
    int score;
} send_game_result;

void init_game(char client_id);
void what_to_do(int buffer);

#endif /* STRUCT_H */

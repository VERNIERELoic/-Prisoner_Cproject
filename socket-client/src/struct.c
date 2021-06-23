#include <stdbool.h>

#include "struct.h"
#include "view_action.h"

connection_server connection_s;
send_choice send_action;

/**
 * \brief Initialise les variables du protocole
 */
void init_game(char client_id)
{
    connection_s.action_id = 1;
    connection_s.client_id = client_id;
    send_action.action_id = 5;
}

/**
 * \brief Vérifie le protocole et redirige les actions
 */
void what_to_do(int buffer)
{
    if (buffer == 2)
    {
        launch_init_window();
    }
    else if (buffer == 3)
    {
        launch_game_window();
    }
    else if (buffer == 4)
    {
        launch_new_round();
    }
    else if (buffer == 6)
    {
        print_round_result();
    }
    else if (buffer == 7)
    {
        print_game_result();
    }
}

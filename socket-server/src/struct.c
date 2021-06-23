
#include <stdbool.h>

#include "struct.h"
#include "utils.h"

connection_status confirm_connection_status;
game_start game_start_protocole;
round_start start_round_protocole;
send_choice send_choice_protocole;
send_round_result send_round_result_client1;
send_round_result send_round_result_client2;
send_game_result send_result_game_client1;
send_game_result send_result_game_client1;
extern config_action_server config_s;


/**
 *\brief Initialise les protocoles
 *\return void
 */
void initProtocole() {
    confirm_connection_status.action_id = 2;
    confirm_connection_status.worked = true;
    
    game_start_protocole.action_id = 3;
    game_start_protocole.round_nb = config_s.round_nb;
            
    start_round_protocole.action_id = 4;
    
    send_choice_protocole.action_id = 5;
    send_choice_protocole.coop = true;
    
    send_round_result_client1.action_id = 6;
    send_round_result_client2.action_id = 6;
    
    send_result_game_client1.action_id = 7;
    send_result_game_client1.action_id = 7;
}

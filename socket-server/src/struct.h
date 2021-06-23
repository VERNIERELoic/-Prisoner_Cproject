
#ifndef PROTOCOLE_H
#define PROTOCOLE_H

#define SIZE_CHAR 100

/**
 * \struct connection_client
 * \brief Gère les connexions client/serveur
 */
typedef struct {
    int action_id;
    char client_id;
} connection_client; 

/**
 * \struct connection_status
 * \brief Sert au serveur pour envoyer la confirmation de la connexion aux joueurs + leur ID
 */
typedef struct {
    int action_id; //action_id
    bool worked;     // TRUE = a marché, FALSE = a échoué
    char message[SIZE_CHAR];      // message de succès ou d’échec
} connection_status; 

/**
 * \struct game_start
 * \brief Sert à annoncer le début d’une nouvelle partie
 */
typedef struct {
    int action_id;
    int round_nb;
} game_start; 

/**
 * \struct round_start
 * \brief Sert à annoncer le début d’une nouvelle manche
 */
typedef struct {
    int action_id;
    int roundId;
} round_start; 

/**
 * \struct send_choice
 * \brief Sert pour l’envoi du choix
 */
typedef struct {
    int action_id;
    bool coop;     /*!< TRUE = collaboration, FALSE = trahison */
    int time_to_choose;     /*!< Temps que le joueur a mis pour choisir */
} send_choice;

/**
 * \struct send_round_choice
 * \brief Sert pour l’envoi des résultats à la fin de la manche
 */
typedef struct {
    int action_id;
    bool result;     /*!< TRUE = victoire, FALSE = défaite */
    int nbPoints;   /*!< Nombre de points */
} send_round_result;

/**
 * \struct send_game_result
 * \brief Sert pour l’envoi des résultats à la fin de la partie
 */
typedef struct {
    int action_id;
    int result;     /*!< 1 = victoire, 0 = défaite, -1 = match nul */
    int nbPoints;    /*!< Nombre de points */
} send_game_result;   

void initProtocole(); 

#endif /* PROTOCOLE_H */


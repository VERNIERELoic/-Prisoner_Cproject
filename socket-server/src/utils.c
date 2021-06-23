#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "utils.h"


/**
 *\brief Ecrit dans les logs
 *\param message Message à écrire
 *\return void
 */
void logs(char message[]) 
{
    int mois, an;
    time_t now;
    time(&now);
      
    // Renvoie l'heure actuelle
    struct tm *local = localtime(&now);        
    mois = local->tm_mon + 1;     
    an = local->tm_year + 1900;
      
    FILE * fichier = fopen("log.txt", "a");
 
    if (fichier == NULL) {
        FILE * fichier = fopen("log.txt", "w");
        fprintf(fichier, "%02d/%02d/%d | %02d:%02d:%02d : %s \n", local->tm_mday, mois, an, local->tm_hour, local->tm_min, local->tm_sec, message);
        fclose(fichier);
    } else {   
        fprintf(fichier, "%02d/%02d/%d | %02d:%02d:%02d : %s \n", local->tm_mday, mois, an, local->tm_hour, local->tm_min, local->tm_sec, message);
        fclose(fichier);
    }
}



/**
 *\brief Renvoie les paramètres du fichier config serveur
 *\param filename Nom du fichier à lire
 *\return config_s Structure de stockage des paramètres de la configuration serveur
 */
config_action_server read_config(char filename[]) 
{
    logs("récupération de la configuration du serveur dans le fichier .ini");
    FILE *file = fopen(filename, "r");
    option opt;
    char line [128];
    config_action_server config_s;

    //Pour chaque ligne du fichier...
    while (fgets (line, sizeof line, file) != NULL) 
    {
        init_option(opt);

        opt = createOptionFromArray(line);
       
        config_s = fillConfigDataServeur(config_s, opt);
    }

    fclose(file);
        
    return config_s;
}


/**
 *\brief Remplit la structure de stockage de la configuration avec une donnée du fichier config serveur
 *\param config_s Structure de stockage de la configuration serveur
 *\param myItem Structure clé/valeur contenant un paramètre du fichier ini
 *\return config_s Structure de stockage des paramètres de la configuration serveur
 */
config_action_server fillConfigDataServeur(config_action_server config_s, option opt)
{
    //remplit le bon attribut dans l'objet config_action_server
    if (strcmp(opt.key, "round_nb") == 0)
    {
        config_s.round_nb = atoi(opt.value);
    }
    else if (strcmp(opt.key, "two_btray") == 0)
    {
        config_s.two_btray = atoi(opt.value);
    }
    else if (strcmp(opt.key, "two_coop") == 0)
    {
        config_s.two_coop = atoi(opt.value);
    }
    else if (strcmp(opt.key, "one_btray_1coop") == 0)
    {
        config_s.one_btray_1coop = atoi(opt.value);
    }
    else if (strcmp(opt.key, "one_coop_1btray") == 0)
    {
        config_s.one_coop_1btray = atoi(opt.value);
    }
    else if (strcmp(opt.key, "host") == 0)
    {
        strncpy(config_s.host, opt.value, SIZE_CHAR);
    }
    else if (strcmp(opt.key, "port") == 0)
    {
        strncpy(config_s.port, opt.value,SIZE_CHAR);
    }

    
    
    return config_s;
}


/**
 *\brief Récupère un paramètre du fichier ini et le stocke dans une structrure cle/valeur
 *\param line Ligne contenant un paramètre
 *\return myItem Structure contenant la valeur du paramètre et sa clé
 */
option createOptionFromArray(char line[])
{
    option opt;
    
    char* e = strchr(line, '=');
    int index = (int) (e - line);
        
    //écrit la clé dans une variable
    char key[SIZE_CHAR];
    memset(key, '\0', SIZE_CHAR);
    for (int i = 0; i < index; i++) {
        key[i] = line[i];
    }

    //écrit la valeur associée à la clé dans une autre variable
    char value[SIZE_CHAR];
    memset(value, '\0', SIZE_CHAR);
    for (int i = index; i < strlen(line); i++) {
        value[i-index] = line[i+1];
    }

    //remplit la structure option avec les deux variables
    strncpy(opt.key, key, SIZE_CHAR);
    strncpy(opt.value, value, SIZE_CHAR);
    
    
    return opt;
}

/**
 * \brief Initialise un Item à vide
 * \param item Structure à initialiser
 * \return void
 */
void init_option(option opt)
{
    memset(opt.key, '\0', SIZE_CHAR);
    memset(opt.value, '\0', SIZE_CHAR);
}
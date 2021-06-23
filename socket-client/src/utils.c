#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "utils.h"

/**
 * \brief Renvoie les paramètres du fichier config serveur dans la structure config_c
 * \param filename Nom du fichier de configuration client
 * \return config_c Structure de stockage des paramètres de la configuration client
 */
config_client read_config(char filename[])
{
    FILE *file = fopen(filename, "r");
    option optn;
    char line[128];
    config_client config_c;

    if (file != NULL)
    {

        while (fgets(line, sizeof line, file) != NULL)
        {
            init_item(optn);

            optn = new_array(line);

            config_c = cp_line(config_c, optn);
        }
        fclose(file);
    }

    return config_c;
}

/**
 * \brief Remplit la structure de stockage de la configuration avec une donnée du fichier ini
 * \param config_c Structure de stockage de la configuration client
 * \param optn Structure clé/valeur contenant un paramètre du fichier ini
 * \return config_c Structure de stockage des paramètres de la configuration client
 */
config_client cp_line(config_client config_c, option optn)
{

    if (strcmp(optn.key, "host") == 0)
    {
        strncpy(config_c.host, optn.value, SIZE);
    }
    else if (strcmp(optn.key, "port") == 0)
    {
        strncpy(config_c.port, optn.value, SIZE);
    }
    else if (strcmp(optn.key, "client") == 0)
    {
        strncpy(config_c.client_id, optn.value, SIZE);
    }

    return config_c;
}

/**
 * \brief Récupère un paramètre du fichier ini et le stocke dans une structrure cle/valeur
 * \param line Ligne contenant un paramètre
 * \return optn Structure contenant la valeur du paramètre et sa clé
 */
option new_array(char line[])
{
    option optn;

    char *e = strchr(line, '=');
    int index = (int)(e - line);

    char key[SIZE];
    memset(key, '\0', SIZE);
    for (int i = 0; i < index; i++)
    {
        key[i] = line[i];
    }

    char value[SIZE];
    memset(value, '\0', SIZE);
    for (int i = index; i < strlen(line); i++)
    {
        value[i - index] = line[i + 1];
    }

    strncpy(optn.key, key, SIZE);
    strncpy(optn.value, value, SIZE);

    return optn;
}

/**
 * \brief Initialise un Item à vide
 * \param item Structure à initialiser
 * \return void
 */
void init_item(option item)
{
    memset(item.key, '\0', SIZE);
    memset(item.value, '\0', SIZE);
}

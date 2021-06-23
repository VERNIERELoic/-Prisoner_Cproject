# ICS - Dilemme du prisonnier

## Sujet du projet
https://gitlab.com/ClaireDsst/projetics/-/blob/master/Sujet%20Projet.pdf
# Documentation

- [Comment l'utiliser](#Utilisation-du-client)
- [configuration du client](#Configuration-client)

## Utilisation du client

Exécutez la commande suivante dans un terminal.

```bash
# Récuperation du projet git  
$ git clone https://gitlab.com/ClaireDsst/projetics.git

# Aller vers le répertoire
$ cd socket-client

# Compilation du client
$ make all

# Executer le client
$ ./output/client

```

### Configuration client
Le fichier de configuration lié au client ce trouve dans le dossier `src/config.ini` 

```
host=127.0.0.1
port=7799
client_id=1

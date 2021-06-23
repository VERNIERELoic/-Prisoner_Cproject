#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <gtk-3.0/gtk/gtk.h>
#include <unistd.h>

#include "clientcxnmanager.h"
#include "getopt.h"
#include "view_action.h"
#include "struct.h"
#include "utils.h"

int sockfd;
GtkBuilder *builder = NULL;
config_client config_c;
extern connection_server connection_s;
bool read_file = false;
GtkWidget *win;

int main(int argc, char **argv)
{
    char title[SIZE];
    pthread_t thread;
    char oc;
    char client_id = '0';
    int id = 0;

    client_id = id + '0';

    if (client_id == 0)
    {
        read_file = true;
        config_c = read_config("config.ini");
        client_id = config_c.client_id[0];
    }

    config_c = read_config("config.ini");
    init_game(client_id);
    init(sockfd, client_id);

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("../src/glade/AppWin.glade");

    sockfd = open_connection();
    write(sockfd, &connection_s, sizeof(connection_server));

    //Creation d'un pthread de lecture
    pthread_create(&thread, 0, threadProcess, &sockfd);
    //write(connection->sock,"Main APP Still running",15);
    pthread_detach(thread);

    win = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    sprintf(title, "Joueur %c - MM", client_id);
    gtk_window_set_title(win, title);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(win);
    gtk_main();

    return (EXIT_SUCCESS);
}

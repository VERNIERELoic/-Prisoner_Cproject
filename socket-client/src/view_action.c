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
#include <gtk-3.0/gtk/gtklabel.h>

#include "struct.h"
#include "utils.h"

char client_id;
extern int sockfd;
extern GtkBuilder *builder;
int round_num = 1;
int firstTimeMins;
int firstTimeSecs;
extern send_choice send_action;
extern int round_nb;
extern int round_id;
extern bool round_win;
extern int round_score;
extern int score;
extern int win_or_lose;

/**
 * \brief Initialise le numéro du socket et de l'id du client
 * \param number Numéro du socket
 * \param id Iid du client
 * \return void
 */
void init(int number, char id)
{
    client_id = id;
    sockfd = number;
}

/**
 * \brief Modification de l'interface et envoie du choix au serveur (ici coop)
 * \return void
 */
void share_on_clicked()
{
    GtkLabel *lbl_result = GTK_LABEL(gtk_builder_get_object(builder, "lbl_result"));
    gtk_label_set_text(lbl_result, (gchar *)"En attente d'un adversaire ...");

    GtkButton *btn_take = GTK_BUTTON(gtk_builder_get_object(builder, "btn_take"));
    gtk_widget_set_sensitive(btn_take, FALSE);

    GtkButton *btn_share = GTK_BUTTON(gtk_builder_get_object(builder, "btn_share"));
    gtk_widget_set_sensitive(btn_share, FALSE);

    GtkSpinner *spinner = GTK_SPINNER(gtk_builder_get_object(builder, "spinner"));
    gtk_widget_show(spinner);

    time_t now;
    time(&now);
    struct tm *secondTime = localtime(&now);

    send_action.time_choice = ((secondTime->tm_min * 60) - (firstTimeMins * 60)) + (secondTime->tm_sec - firstTimeSecs);
    send_action.coop = true;
    write(sockfd, &send_action, sizeof(send_choice));
}

/**
 * \brief Modification de l'interface et envoie du choix au serveur (ici trahir)
 * \return void
 */
void take_on_clicked()
{
    GtkLabel *lbl_result = GTK_LABEL(gtk_builder_get_object(builder, "lbl_result"));
    gtk_label_set_text(lbl_result, (gchar *)"En attente de votre adversaire ...");

    GtkButton *btn_take = GTK_BUTTON(gtk_builder_get_object(builder, "btn_take"));
    gtk_widget_set_sensitive(btn_take, FALSE);

    GtkButton *btn_share = GTK_BUTTON(gtk_builder_get_object(builder, "btn_share"));
    gtk_widget_set_sensitive(btn_share, FALSE);

    GtkSpinner *spinner = GTK_SPINNER(gtk_builder_get_object(builder, "spinner"));
    gtk_widget_show(spinner);

    time_t now;
    time(&now);
    struct tm *secondTime = localtime(&now);

    send_action.time_choice = ((secondTime->tm_min * 60) - (firstTimeMins * 60)) + (secondTime->tm_sec - firstTimeSecs);
    send_action.coop = false;
    write(sockfd, &send_action, sizeof(send_choice));
}

/**
 * \brief Lancement de l'interface d'initialisation
 * \return void
 */
void launch_init_window()
{
    GtkLabel *lbl_result = GTK_LABEL(gtk_builder_get_object(builder, "lbl_result"));
    gtk_label_set_text(lbl_result, (gchar *)"En attente de votre adversaire ...");

    GtkLabel *lbl_score = GTK_LABEL(gtk_builder_get_object(builder, "lbl_score"));
    gtk_label_set_text(lbl_score, (gchar *)"");

    GtkLabel *lbl_player = GTK_LABEL(gtk_builder_get_object(builder, "lbl_player"));
    gtk_label_set_text(lbl_player, (gchar *)"Bienvenue dans le dilemme du prisonnier");

    GtkLabel *lbl_round = GTK_LABEL(gtk_builder_get_object(builder, "lbl_round"));
    gtk_label_set_text(lbl_round, (gchar *)"");

    GtkLabel *lbl_ask = GTK_LABEL(gtk_builder_get_object(builder, "lbl_ask"));
    gtk_widget_hide(lbl_ask);

    GtkButton *btn_take = GTK_BUTTON(gtk_builder_get_object(builder, "btn_take"));
    gtk_widget_hide(btn_take);

    GtkButton *btn_share = GTK_BUTTON(gtk_builder_get_object(builder, "btn_share"));
    gtk_widget_hide(btn_share);
}

/**
 * \brief Lancement de l'interface de jeux
 * \return void
 */
void launch_game_window()
{
    char round_label_char[SIZE];
    GtkLabel *lbl_result = GTK_LABEL(gtk_builder_get_object(builder, "lbl_result"));
    gtk_label_set_text(lbl_result, (gchar *)"");

    GtkLabel *lbl_player = GTK_LABEL(gtk_builder_get_object(builder, "lbl_player"));
    gtk_label_set_text(lbl_player, (gchar *)"");

    GtkLabel *lbl_score = GTK_LABEL(gtk_builder_get_object(builder, "lbl_score"));
    gtk_label_set_text(lbl_score, (gchar *)"");

    GtkLabel *lbl_round = GTK_LABEL(gtk_builder_get_object(builder, "lbl_round"));
    sprintf(round_label_char, "Round 1 / %d", round_nb);
    gtk_label_set_text(lbl_round, (gchar *)round_label_char);

    GtkLabel *lbl_ask = GTK_LABEL(gtk_builder_get_object(builder, "lbl_ask"));
    gtk_widget_show(lbl_ask);

    GtkButton *btn_take = GTK_BUTTON(gtk_builder_get_object(builder, "btn_take"));
    gtk_widget_show(btn_take);

    GtkButton *btn_share = GTK_BUTTON(gtk_builder_get_object(builder, "btn_share"));
    gtk_widget_show(btn_share);

    GtkSpinner *spinner = GTK_SPINNER(gtk_builder_get_object(builder, "spinner"));
    gtk_widget_hide(spinner);

    time_t now;
    time(&now);
    struct tm *firstTime = localtime(&now);
    firstTimeMins = firstTime->tm_min;
    firstTimeSecs = firstTime->tm_sec;
}

/**
 * \brief Lancement de l'interface d'un nouveau round
 * \return void
 */
void launch_new_round()
{
    sleep(1);
    char round_label_char[SIZE];

    GtkLabel *lbl_result = GTK_LABEL(gtk_builder_get_object(builder, "lbl_result"));
    gtk_label_set_text(lbl_result, (gchar *)"");

    GtkButton *btn_take = GTK_BUTTON(gtk_builder_get_object(builder, "btn_take"));
    gtk_widget_set_sensitive(btn_take, TRUE);

    GtkButton *btn_share = GTK_BUTTON(gtk_builder_get_object(builder, "btn_share"));
    gtk_widget_set_sensitive(btn_share, TRUE);

    GtkSpinner *spinner = GTK_SPINNER(gtk_builder_get_object(builder, "spinner"));
    gtk_widget_hide(spinner);

    GtkLabel *lbl_round = GTK_LABEL(gtk_builder_get_object(builder, "lbl_round"));
    sprintf(round_label_char, "Round %d / %d", round_id, round_nb);
    gtk_label_set_text(lbl_round, (gchar *)round_label_char);

    time_t now;
    time(&now);
    struct tm *firstTime = localtime(&now);
    firstTimeMins = firstTime->tm_min;
    firstTimeSecs = firstTime->tm_sec;
}

/**
 * \brief Affichage d'un nouveau round sur l'interface
 * \return void
 */
void print_round_result()
{
    char score_label_char[SIZE];
    GtkLabel *lbl_result = GTK_LABEL(gtk_builder_get_object(builder, "lbl_result"));
    if (round_win == true)
    {
        gtk_label_set_text(lbl_result, (gchar *)"Vous avez gagné le round !");
    }
    else
    {
        gtk_label_set_text(lbl_result, (gchar *)"Vous avez perdu le round !");
    }

    GtkLabel *lbl_score = GTK_LABEL(gtk_builder_get_object(builder, "lbl_score"));
    char *score = (char *)gtk_label_get_text(lbl_score);

    char *e = strchr(score, ':');
    int index = (int)(e - score);

    char value[SIZE];
    memset(value, '\0', SIZE);
    for (int i = index; i < strlen(score); i++)
    {
        value[i - index] = score[i + 1];
    }

    int totalPoints = atoi(value) + round_score;
    sprintf(score_label_char, "Votre score : %d", totalPoints);
    gtk_label_set_text(lbl_score, (gchar *)score_label_char);
}

/**
 * \brief Affichage du résultat du jeu
 * \return void
 */
void print_game_result()
{
    GtkLabel *lbl_result = GTK_LABEL(gtk_builder_get_object(builder, "lbl_result"));
    if (win_or_lose == 1)
    {
        gtk_label_set_text(lbl_result, (gchar *)"Vous avez gagné !");
    }
    else if (win_or_lose == 0)
    {
        gtk_label_set_text(lbl_result, (gchar *)"Vous avez perdu !");
    }
    else
    {
        gtk_label_set_text(lbl_result, (gchar *)"Vous avez fait égalité !");
    }

    GtkButton *btn_take = GTK_BUTTON(gtk_builder_get_object(builder, "btn_take"));
    gtk_widget_hide(btn_take);

    GtkButton *btn_share = GTK_BUTTON(gtk_builder_get_object(builder, "btn_share"));
    gtk_widget_hide(btn_share);

    GtkSpinner *spinner = GTK_SPINNER(gtk_builder_get_object(builder, "spinner"));
    gtk_widget_hide(spinner);

    GtkLabel *lbl_round = GTK_LABEL(gtk_builder_get_object(builder, "lbl_round"));
    gtk_label_set_text(lbl_round, (gchar *)"");

    GtkLabel *lbl_ask = GTK_LABEL(gtk_builder_get_object(builder, "lbl_ask"));
    gtk_widget_hide(lbl_ask);
}

/**
 * \brief Quitte l'interface
 * \return void
 */
void exit_on_clicked(){
        GtkWidget *message_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK_CANCEL,
            "Voulez vous vraiment quitter cette superbe partie !?");
    unsigned int pressed = gtk_dialog_run(GTK_DIALOG(message_dialog));
    if (pressed == GTK_RESPONSE_OK) {
        printf("OK Pressed \n");
        printf("quitting\n ");
        gtk_widget_destroy(message_dialog);
        gtk_main_quit();
    } else {
        printf("CANCEL Pressed \n");
        gtk_widget_destroy(message_dialog);
    }
}

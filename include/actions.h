#ifndef ACTIONS_H
#define ACTIONS_H

#include <signal.h> // Include this header for kill() function
#include <jansson.h>

#include "common.h"
#include "connection.h"

void on_start_button_clicked(GtkButton *button, gpointer data);
void on_terminate_button_clicked(GtkButton *button, gpointer data);
void on_restart_button_clicked(GtkButton *button, gpointer data);
void on_dark_mode_button_clicked(GtkButton *button, gpointer data);
int read_options_from_application_support(Options *options);
int save_options_to_application_support(const Options *options);
void validate_entry_widget(GtkWidget *entry, gpointer data);

#endif // ACTIONS_H
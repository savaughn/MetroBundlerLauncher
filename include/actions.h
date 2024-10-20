#ifndef ACTIONS_H
#define ACTIONS_H

#include <signal.h> // Include this header for kill() function

#include "common.h"
#include "connection.h"

// Callback function for the start button click
void on_start_button_clicked(GtkButton *button, gpointer data);
// Callback function for the terminate button click
void on_terminate_button_clicked(GtkButton *button, gpointer data);
// Callback function for the restart button click
void on_restart_button_clicked(GtkButton *button, gpointer data);
void on_dark_mode_button_clicked(GtkButton *button, gpointer user_data);
int read_options_from_application_support(Options *options);

#endif // ACTIONS_H
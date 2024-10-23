#ifndef CONTENT_H
#define CONTENT_H

#include "common.h"
#include "actions.h"
#include "button.h"

void init_window(GtkWidget *win, GtkWidget *main_box);
GtkWidget *create_main_window(GApplication *app, Widgets *widgets);
void init_button_box(GtkWidget *button_box, Widgets *widgets);
GtkWidget *init_entry_widget(GtkEntry *entry, const char *label_text, const char *placeholder_text);
void init_inputs_box(GtkWidget *inputs_box, Widgets *widgets);
void init_main_box(GtkWidget *main_box, GtkWidget *inputs_box, GtkWidget *button_box);
void draw_circle(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);

#endif // CONTENT_H
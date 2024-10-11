#ifndef CONTENT_H
#define CONTENT_H

#include "common.h"
#include "actions.h"
#include "button.h"

void init_button_box(GtkWidget *button_box, Widgets *widgets);
GtkWidget *init_entry_widget(GtkEntry *entry, const char *label_text, const char *placeholder_text);
void init_display_box(GtkWidget *display_box, Widgets *widgets);

#endif // CONTENT_H
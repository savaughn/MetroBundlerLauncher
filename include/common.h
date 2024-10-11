#ifndef COMMON_H
#define COMMON_H

#include <gtk/gtk.h>

// Struct to hold label, button widgets, and port entry
typedef struct {
  GtkLabel *status_label;
  GtkButton *start_button;
  GtkButton *terminate_button;
  GtkButton *restart_button;
  GtkButton *dark_mode_button;
  GtkEntry *env_entry;
  GtkEntry *port_entry;
  GtkCheckButton *hermes_checkbox;
} Widgets;

typedef enum
{
  BUTTON_TYPE_START,
  BUTTON_TYPE_TERMINATE,
  BUTTON_TYPE_RESTART,
  BUTTON_TYPE_DARK_MODE
} ButtonType;

#define DEFAULT_ENV "dev" // Define the default IP address
#define DEFAULT_PORT 8090 // Define the default port

#endif // COMMON_H
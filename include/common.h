#ifndef COMMON_H
#define COMMON_H

#include <gtk/gtk.h>

#define APPLICATION_ID "com.savaughn.metro_bundler_launcher"

static const GdkRGBA RED = {1.0, 0.0, 0.0, 1.0};
static const GdkRGBA GREEN = {0.0, 1.0, 0.0, 1.0};

typedef struct {
    const char *port;
    const char *prefix;
    const char *file;
    gboolean debugger_enabled;
    gboolean dark_mode;
} Options;

// Struct to hold label, button widgets, and port entry
typedef struct {
  GtkLabel *port_label;
  GtkButton *start_button;
  GtkButton *terminate_button;
  GtkButton *restart_button;
  GtkButton *dark_mode_button;
  GtkEntry *prefix_entry;
  GtkEntry *port_entry;
  GtkEntry *file_entry;
  GtkCheckButton *hermes_checkbox;
  gboolean is_currently_running;
  GtkDrawingArea *status_light;
} Widgets;

typedef enum
{
  BUTTON_TYPE_START,
  BUTTON_TYPE_TERMINATE,
  BUTTON_TYPE_RESTART,
  BUTTON_TYPE_DARK_MODE
} ButtonType;

void LOG(const char *restrict fmt, ...);

#define DEFAULT_PORT 8080 // Define the default port

typedef enum
{
    STATE_IDLE, // nothing running
    STATE_RUNNING,
    STATE_ERROR
} WidgetsState;

#endif // COMMON_H
#include "common.h"
#include "connection.h"
#include "content.h"

static void on_app_quit(GApplication *app, gpointer user_data)
{
    Widgets *widgets = (Widgets *)user_data;
    g_free(widgets);
}

static void app_activate(GApplication *app)
{
    // Enable dark mode by default
    GtkSettings *settings = gtk_settings_get_default();
    Options options = {0};
    if (read_options_from_application_support(&options) == 0)
    {
        g_object_set(settings, "gtk-application-prefer-dark-theme", options.dark_mode, NULL);
    }
    else
    {
        g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);
        g_print("Options file not found, creating a new one\n");
        options.port = "";
        options.prefix = "";
        options.file = "";
        options.debugger_enabled = FALSE;
        options.dark_mode = TRUE;
        save_options_to_application_support(&options);
    }

    // Create a struct to hold the label and buttons
    Widgets *widgets = g_malloc(sizeof(Widgets));
    if (widgets == NULL)
    {
        g_critical("Failed to allocate memory for widgets");
        return;
    }
    GtkWidget *win = create_main_window(app, widgets);
    gtk_window_present(GTK_WINDOW(win));

    // Check the connection status on startup and update the UI accordingly
    check_connection(widgets);

    g_signal_connect(app, "shutdown", G_CALLBACK(on_app_quit), widgets);
}

int main(int argc, char **argv)
{
    int stat;

    GtkApplication *app = gtk_application_new(APPLICATION_ID, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);

    stat = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return stat;
}

#include "common.h"
#include "connection.h"
#include "content.h"

static void app_activate(GApplication *app)
{
	// Enable dark mode by default
	GtkSettings *settings = gtk_settings_get_default();
	g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

	// Create a struct to hold the label and buttons
	Widgets *widgets = g_malloc(sizeof(Widgets));
	GtkWidget *win = create_main_window(app, widgets);

	gtk_window_present(GTK_WINDOW(win));

	// Check the connection status on startup and update the UI accordingly
	check_connection(widgets);
}

int main(int argc, char **argv)
{
	int stat;

	GtkApplication *app = gtk_application_new("com.vaughn.metro_launcher", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);

	stat = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);

	return stat;
}
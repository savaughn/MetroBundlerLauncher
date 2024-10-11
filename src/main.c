#include "common.h"
#include "connection.h"
#include "content.h"

static void app_activate(GApplication *app)
{
	// Enable dark mode by default
	GtkSettings *settings = gtk_settings_get_default();
	g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

	GtkWidget *win = gtk_application_window_new(GTK_APPLICATION(app));

	// Create a struct to hold the label and buttons
	Widgets *widgets = g_malloc(sizeof(Widgets));

	GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *display_box = gtk_grid_new();
	init_display_box(display_box, widgets);

	GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	init_button_box(button_box, widgets);

	init_main_box(main_box, display_box, button_box);

	gtk_window_set_title(GTK_WINDOW(win), "Metro Bundler Launcher");
	gtk_window_set_default_size(GTK_WINDOW(win), 400, 100);
	gtk_window_set_resizable(GTK_WINDOW(win), FALSE); // Lock the window size
	gtk_window_set_child(GTK_WINDOW(win), main_box);

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
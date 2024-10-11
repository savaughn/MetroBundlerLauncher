#include "content.h"

void init_button_box(GtkWidget *button_box, Widgets *widgets)
{
	GtkWidget *start_button = create_button(BUTTON_TYPE_START);
	GtkWidget *terminate_button = create_button(BUTTON_TYPE_TERMINATE);
	GtkWidget *restart_button = create_button(BUTTON_TYPE_RESTART);
	GtkWidget *dark_mode_button = create_button(BUTTON_TYPE_DARK_MODE);

	widgets->start_button = GTK_BUTTON(start_button);
	widgets->terminate_button = GTK_BUTTON(terminate_button);
	widgets->restart_button = GTK_BUTTON(restart_button);
	widgets->dark_mode_button = GTK_BUTTON(dark_mode_button);

	gtk_box_append(GTK_BOX(button_box), GTK_WIDGET(widgets->start_button));
	gtk_box_append(GTK_BOX(button_box), GTK_WIDGET(widgets->terminate_button));
	gtk_box_append(GTK_BOX(button_box), GTK_WIDGET(widgets->restart_button));
	gtk_box_append(GTK_BOX(button_box), GTK_WIDGET(widgets->dark_mode_button));

	// Connect the buttons' "clicked" signals to the callback functions
	g_signal_connect(widgets->start_button, "clicked", G_CALLBACK(on_start_button_clicked), widgets);
	g_signal_connect(widgets->terminate_button, "clicked", G_CALLBACK(on_terminate_button_clicked), widgets);
	g_signal_connect(widgets->restart_button, "clicked", G_CALLBACK(on_restart_button_clicked), widgets);
	g_signal_connect(widgets->dark_mode_button, "clicked", G_CALLBACK(on_dark_mode_button_clicked), widgets);
}

GtkWidget *init_entry_widget(GtkEntry *entry, const char *label_text, const char *placeholder_text)
{
	// box for ip address entry
	GtkWidget *label = gtk_label_new(label_text);
	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_entry_set_placeholder_text(GTK_ENTRY(entry), placeholder_text);

	gtk_box_append(GTK_BOX(hbox), label);
	gtk_box_append(GTK_BOX(hbox), GTK_WIDGET(entry));

	return hbox;
}

void init_display_box(GtkWidget *display_box, Widgets *widgets)
{
	GtkWidget *status_label = gtk_label_new("");
	GtkWidget *env_entry = gtk_entry_new();
	GtkWidget *port_entry = gtk_entry_new();
	GtkWidget *hermes_checkbox = gtk_check_button_new_with_label("Enable Hermes Debugger");

	widgets->status_label = GTK_LABEL(status_label);
	widgets->env_entry = GTK_ENTRY(env_entry);
	widgets->port_entry = GTK_ENTRY(port_entry);
	widgets->hermes_checkbox = GTK_CHECK_BUTTON(hermes_checkbox);

	GtkWidget *inputs_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_append(GTK_BOX(inputs_box), init_entry_widget(widgets->env_entry, "ENV", "dev"));
	gtk_box_append(GTK_BOX(inputs_box), init_entry_widget(widgets->port_entry, "Port", "8090"));
	gtk_box_append(GTK_BOX(inputs_box), hermes_checkbox);

	// Set the width of each widget to half of the window width
	gtk_widget_set_size_request(inputs_box, 200, -1);
	gtk_widget_set_size_request(status_label, 200, -1);

	// Enable text wrapping for the status_label
	gtk_label_set_wrap(GTK_LABEL(status_label), TRUE);
	gtk_label_set_wrap_mode(GTK_LABEL(status_label), PANGO_WRAP_WORD);

	// Center the text within the status_label
	gtk_label_set_xalign(GTK_LABEL(status_label), 0.5);
	gtk_widget_set_halign(status_label, GTK_ALIGN_CENTER);

	// Add widgets to the grid
	gtk_grid_attach(GTK_GRID(display_box), inputs_box, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(display_box), status_label, 1, 0, 1, 1);
}

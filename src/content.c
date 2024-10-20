#include "content.h"
#include "actions.h"

void init_window(GtkWidget *win, GtkWidget *main_box)
{
    gtk_window_set_title(GTK_WINDOW(win), "Metro Bundler Launcher");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 100);
    gtk_window_set_resizable(GTK_WINDOW(win), FALSE); // Lock the window size
    gtk_window_set_child(GTK_WINDOW(win), main_box);
}

GtkWidget *create_main_window(GApplication *app, Widgets *widgets)
{
	// Setup the main content
    GtkWidget *display_box = gtk_grid_new();
	init_display_box(display_box, widgets);

    // Setup the buttons
	GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	init_button_box(button_box, widgets);

    // Put the display and button boxes into a main container box
	GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	init_main_box(main_box, display_box, button_box);

    // Create the main window and add the main container box
    GtkWidget *win = gtk_application_window_new(GTK_APPLICATION(app));
    init_window(win, main_box);

	return win;
}

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
    GtkWidget *prefix_entry = gtk_entry_new();
    GtkWidget *port_entry = gtk_entry_new();
    GtkWidget *file_entry = gtk_entry_new();
    GtkWidget *hermes_checkbox = gtk_check_button_new_with_label("Enable Hermes Debugger");

    widgets->status_label = GTK_LABEL(status_label);
    widgets->prefix_entry = GTK_ENTRY(prefix_entry);
    widgets->port_entry = GTK_ENTRY(port_entry);
    widgets->file_entry = GTK_ENTRY(file_entry);
    widgets->hermes_checkbox = GTK_CHECK_BUTTON(hermes_checkbox);

    GtkWidget *inputs_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    char *port_text, *prefix_text, *file_text;
    gboolean debugger_enabled = FALSE;

    if (read_options_from_application_support(&port_text, &prefix_text, &file_text, &debugger_enabled) == 0)
    {
        gtk_editable_set_text(GTK_EDITABLE(widgets->port_entry), port_text);
        gtk_editable_set_text(GTK_EDITABLE(widgets->prefix_entry), prefix_text);
        gtk_editable_set_text(GTK_EDITABLE(widgets->file_entry), file_text);
        gtk_check_button_set_active(GTK_CHECK_BUTTON(widgets->hermes_checkbox), debugger_enabled);
    }

    gtk_box_append(GTK_BOX(inputs_box), init_entry_widget(widgets->prefix_entry, "", "env vars"));
    gtk_box_append(GTK_BOX(inputs_box), init_entry_widget(widgets->port_entry, "", "port"));
    gtk_box_append(GTK_BOX(inputs_box), init_entry_widget(widgets->file_entry, "", "RN project root"));
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

void init_main_box(GtkWidget *main_box, GtkWidget *display_box, GtkWidget *button_box)
{
    // Add padding to the main container box
    gtk_widget_set_margin_top(main_box, 10);
    gtk_widget_set_margin_bottom(main_box, 10);
    gtk_widget_set_margin_start(main_box, 10);
    gtk_widget_set_margin_end(main_box, 10);

    // Add display_box to the top and button_box to the bottom
    gtk_box_append(GTK_BOX(main_box), display_box);
    gtk_widget_set_vexpand(display_box, TRUE);
    gtk_widget_set_valign(display_box, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(main_box), button_box);
    gtk_widget_set_valign(button_box, GTK_ALIGN_END);
}

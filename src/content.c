#include "content.h"
#include "actions.h"
#include "draw.h"

#include <cairo.h>
#include <gtk/gtk.h>

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
    GtkWidget *inputs_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(inputs_box, 400, 10);
    init_inputs_box(inputs_box, widgets);

    // Setup the buttons
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    // set button_box width to window width
    gtk_widget_set_size_request(button_box, -1, 10);

    Options options = {0};
    if (read_options_from_application_support(&options) == 0)
    {
        gtk_editable_set_text(GTK_EDITABLE(widgets->port_entry), options.port);
        gtk_editable_set_text(GTK_EDITABLE(widgets->prefix_entry), options.prefix);
        gtk_editable_set_text(GTK_EDITABLE(widgets->file_entry), options.file);
        gtk_check_button_set_active(GTK_CHECK_BUTTON(widgets->hermes_checkbox), options.debugger_enabled);
    }

    init_button_box(button_box, widgets, &options);

    // Put the display and button boxes into a main container box
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    init_main_box(main_box, inputs_box, button_box);

    // Create the main window and add the main container box
    GtkWidget *win = gtk_application_window_new(GTK_APPLICATION(app));
    init_window(win, main_box);

    return win;
}

void init_button_box(GtkWidget *button_box, Widgets *widgets, Options *options)
{
    GtkWidget *start_button = create_button(BUTTON_TYPE_START);
    GtkWidget *terminate_button = create_button(BUTTON_TYPE_TERMINATE);
    GtkWidget *restart_button = create_button(BUTTON_TYPE_RESTART);
    GtkWidget *dark_mode_button = create_button(BUTTON_TYPE_DARK_MODE);
    GtkWidget *port_label = gtk_label_new("");

    widgets->start_button = GTK_BUTTON(start_button);
    widgets->terminate_button = GTK_BUTTON(terminate_button);
    widgets->restart_button = GTK_BUTTON(restart_button);
    widgets->dark_mode_button = GTK_BUTTON(dark_mode_button);
    widgets->port_label = GTK_LABEL(port_label);

    // Left content
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *drawing_area = gtk_drawing_area_new();
    widgets->status_light = GTK_DRAWING_AREA(drawing_area);
    gtk_widget_set_size_request(drawing_area, 20, 20);
    gtk_box_append(GTK_BOX(left_box), drawing_area);
    gtk_widget_set_hexpand(drawing_area, TRUE);
    gtk_widget_set_halign(drawing_area, GTK_ALIGN_START);
    gtk_widget_set_vexpand(drawing_area, TRUE);

    GdkRGBA *circle_color = g_malloc(sizeof(GdkRGBA));
    circle_color->red = RED.red;
    circle_color->green = RED.green;
    circle_color->blue = RED.blue;
    circle_color->alpha = RED.alpha;

    // Set the drawing function for the drawing area with the color
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_circle, circle_color, g_free);

    // Add a label to the right of the circle with port information
    gtk_box_append(GTK_BOX(left_box), port_label);
    gtk_box_append(GTK_BOX(button_box), left_box);
    gtk_widget_set_hexpand(left_box, FALSE);
    gtk_widget_set_halign(left_box, GTK_ALIGN_START);
    gtk_widget_set_valign(left_box, GTK_ALIGN_CENTER);

    // Space before the buttons
    GtkWidget *spacer_before = gtk_label_new(NULL);
    gtk_widget_set_hexpand(spacer_before, TRUE);
    gtk_box_append(GTK_BOX(button_box), spacer_before);

    // Center content
    GtkWidget *center_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    gtk_box_append(GTK_BOX(center_box), GTK_WIDGET(widgets->start_button));
    gtk_box_append(GTK_BOX(center_box), GTK_WIDGET(widgets->terminate_button));
    gtk_box_append(GTK_BOX(center_box), GTK_WIDGET(widgets->restart_button));
    gtk_box_append(GTK_BOX(button_box), center_box);

    // Add another expanding empty space after center_hbox to maintain centering
    GtkWidget *spacer_after = gtk_label_new(NULL);
    gtk_widget_set_hexpand(spacer_after, TRUE);
    gtk_box_append(GTK_BOX(button_box), spacer_after);

    // Right content
    gtk_box_append(GTK_BOX(button_box), GTK_WIDGET(dark_mode_button));
    gtk_widget_set_hexpand(dark_mode_button, FALSE);
    gtk_widget_set_halign(dark_mode_button, GTK_ALIGN_END);

    // Connect the buttons' "clicked" signals to the callback functions
    g_signal_connect(widgets->start_button, "clicked", G_CALLBACK(on_start_button_clicked), widgets);
    g_signal_connect(widgets->terminate_button, "clicked", G_CALLBACK(on_terminate_button_clicked), widgets);
    g_signal_connect(widgets->restart_button, "clicked", G_CALLBACK(on_restart_button_clicked), widgets);
    g_signal_connect(widgets->dark_mode_button, "clicked", G_CALLBACK(on_dark_mode_button_clicked), options);
}

void init_inputs_box(GtkWidget *inputs_box, Widgets *widgets)
{
    GtkWidget *prefix_entry = gtk_entry_new();
    GtkWidget *port_entry = gtk_entry_new();
    GtkWidget *file_entry = gtk_entry_new();
    GtkWidget *hermes_checkbox = gtk_check_button_new_with_label("Enable Hermes Debugger");

    widgets->prefix_entry = GTK_ENTRY(prefix_entry);
    widgets->port_entry = GTK_ENTRY(port_entry);
    widgets->file_entry = GTK_ENTRY(file_entry);
    widgets->hermes_checkbox = GTK_CHECK_BUTTON(hermes_checkbox);

    gtk_entry_set_placeholder_text(widgets->file_entry, "project root");
    gtk_entry_set_placeholder_text(widgets->prefix_entry, "env vars");
    gtk_entry_set_placeholder_text(widgets->port_entry, "port");

    gtk_box_append(GTK_BOX(inputs_box), GTK_WIDGET(widgets->file_entry));
    gtk_box_append(GTK_BOX(inputs_box), GTK_WIDGET(widgets->prefix_entry));

    GtkWidget *port_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(port_box), GTK_WIDGET(widgets->port_entry));
    gtk_box_append(GTK_BOX(port_box), hermes_checkbox);

    gtk_box_append(GTK_BOX(inputs_box), port_box);
    
    gtk_widget_set_hexpand(inputs_box, TRUE);

    // field validation for file_entry
    g_signal_connect(widgets->file_entry, "changed", G_CALLBACK(validate_entry_widget), widgets);
}

void init_main_box(GtkWidget *main_box, GtkWidget *inputs_box, GtkWidget *button_box)
{
    // Add padding to the main container box
    gtk_widget_set_margin_top(main_box, 10);
    gtk_widget_set_margin_bottom(main_box, 10);
    gtk_widget_set_margin_start(main_box, 10);
    gtk_widget_set_margin_end(main_box, 10);

    // Add inputs_box to the top and button_box to the bottom
    gtk_box_append(GTK_BOX(main_box), inputs_box);
    gtk_widget_set_valign(inputs_box, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(main_box), button_box);
}

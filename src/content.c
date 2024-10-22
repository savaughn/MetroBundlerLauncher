#include "content.h"
#include "actions.h"

#include <cairo.h>
#include <gtk/gtk.h>

static void draw_circle(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data)
{
    // Cast user_data to GdkRGBA
    GdkRGBA *color = (GdkRGBA *)user_data;

    // Set the background color to transparent
    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.0);
    cairo_paint(cr);

    // Calculate the center and radius of the circle
    double radius = MIN(width, height) / 10.0; // Calculate the radius
    double center_x = width / 2.0;             // Center X position
    double center_y = height / 2.0;            // Center Y position

    // Draw a dark outline around the entire circle
    cairo_set_source_rgba(cr, 0.1, 0.1, 0.1, 0.75); // Dark color for the outline
    cairo_set_line_width(cr, 0.5);
    cairo_arc(cr, center_x, center_y, radius + 2, 0, 2 * G_PI); // Outline the main circle
    cairo_stroke(cr);

    // Create a radial gradient for the glow effect
    cairo_pattern_t *gradient = cairo_pattern_create_radial(center_x, center_y, radius, center_x, center_y, radius + 7);
    cairo_pattern_add_color_stop_rgba(gradient, 1.0, color->red, color->green, color->blue, 0.0); // Transparent color at the outer edge
    cairo_pattern_add_color_stop_rgba(gradient, 0.0, color->red, color->green, color->blue, 1.0); // Color with full opacity at the center

    // Set the gradient as the source and fill the circle
    cairo_set_source(cr, gradient);
    cairo_arc(cr, center_x, center_y, radius + 7, 0, 2 * G_PI); // Draw the glowing area
    cairo_fill(cr);

    // Draw the main colored circle on top
    cairo_set_source_rgb(cr, color->red, color->green, color->blue);
    cairo_arc(cr, center_x, center_y, radius, 0, 2 * G_PI);
    cairo_fill(cr);

    // Cleanup
    cairo_pattern_destroy(gradient);
}

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
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    // set button_box width to window width
    gtk_widget_set_size_request(button_box, -1, 10);

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
    GtkWidget *port_label = gtk_label_new("");

    widgets->start_button = GTK_BUTTON(start_button);
    widgets->terminate_button = GTK_BUTTON(terminate_button);
    widgets->restart_button = GTK_BUTTON(restart_button);
    widgets->dark_mode_button = GTK_BUTTON(dark_mode_button);
    widgets->port_label = GTK_LABEL(port_label);

    // Left content
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 20, 20);
    gtk_box_append(GTK_BOX(left_box), drawing_area);
    gtk_widget_set_hexpand(drawing_area, TRUE);
    gtk_widget_set_halign(drawing_area, GTK_ALIGN_START);
    gtk_widget_set_vexpand(drawing_area, TRUE);

    // Define the color (e.g., red)
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
    GtkWidget *prefix_entry = gtk_entry_new();
    GtkWidget *port_entry = gtk_entry_new();
    GtkWidget *file_entry = gtk_entry_new();
    GtkWidget *hermes_checkbox = gtk_check_button_new_with_label("Enable Hermes Debugger");

    widgets->prefix_entry = GTK_ENTRY(prefix_entry);
    widgets->port_entry = GTK_ENTRY(port_entry);
    widgets->file_entry = GTK_ENTRY(file_entry);
    widgets->hermes_checkbox = GTK_CHECK_BUTTON(hermes_checkbox);

    GtkWidget *inputs_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    Options options;
    if (read_options_from_application_support(&options) == 0)
    {
        gtk_editable_set_text(GTK_EDITABLE(widgets->port_entry), options.port);
        gtk_editable_set_text(GTK_EDITABLE(widgets->prefix_entry), options.prefix);
        gtk_editable_set_text(GTK_EDITABLE(widgets->file_entry), options.file);
        gtk_check_button_set_active(GTK_CHECK_BUTTON(widgets->hermes_checkbox), options.debugger_enabled);
    }

    gtk_box_append(GTK_BOX(inputs_box), init_entry_widget(widgets->prefix_entry, "", "env vars"));
    gtk_box_append(GTK_BOX(inputs_box), init_entry_widget(widgets->port_entry, "", "port"));
    gtk_box_append(GTK_BOX(inputs_box), init_entry_widget(widgets->file_entry, "", "RN project root"));
    gtk_box_append(GTK_BOX(inputs_box), hermes_checkbox);

    // Set the width of each widget to half of the window width
    gtk_widget_set_size_request(inputs_box, 400, -1);
    // gtk_widget_set_size_request(port_label, 200, -1);

    // Enable text wrapping for the port_label
    // gtk_label_set_wrap(GTK_LABEL(port_label), TRUE);
    // gtk_label_set_wrap_mode(GTK_LABEL(port_label), PANGO_WRAP_WORD);

    // Center the text within the port_label
    // gtk_label_set_xalign(GTK_LABEL(port_label), 0.5);
    // gtk_widget_set_halign(port_label, GTK_ALIGN_CENTER);

    // Add widgets to the grid
    gtk_grid_attach(GTK_GRID(display_box), inputs_box, 0, 0, 1, 1);
    // gtk_grid_attach(GTK_GRID(display_box), port_label, 1, 0, 1, 1);
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
}

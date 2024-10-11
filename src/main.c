#include "common.h"
#include "connection.h"
#include "actions.h"
#include "button.h"

GtkWidget *init_entry_widget(const char *label_text, GtkEntry *entry)
{
    // box for ip address entry
  GtkWidget *label = gtk_label_new(label_text);
  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_append(GTK_BOX(hbox), label);
  gtk_box_append(GTK_BOX(hbox), GTK_WIDGET(entry));

  return hbox;
}

void init_display_box(GtkWidget *display_box, GtkWidget *status_label, Widgets *widgets, GtkWidget *hermes_checkbox)
{
  GtkWidget *inputs_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_append(GTK_BOX(inputs_box), init_entry_widget("ENV", widgets->env_entry));
  gtk_box_append(GTK_BOX(inputs_box), init_entry_widget("Port", widgets->port_entry));
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

static void app_activate(GApplication *app)
{
  // Enable dark mode by default
  GtkSettings *settings = gtk_settings_get_default();
  g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

  GtkWidget *win = gtk_application_window_new(GTK_APPLICATION(app));
  GtkWidget *status_label = gtk_label_new("");

  GtkWidget *start_button = create_button(BUTTON_TYPE_START);
  GtkWidget *terminate_button = create_button(BUTTON_TYPE_TERMINATE);
  GtkWidget *restart_button = create_button(BUTTON_TYPE_RESTART);
  GtkWidget *dark_mode_button = create_button(BUTTON_TYPE_DARK_MODE);

  GtkWidget *env_entry = gtk_entry_new();
  GtkWidget *port_entry = gtk_entry_new();
  GtkWidget *hermes_checkbox = gtk_check_button_new_with_label("Enable Hermes Debugger");

  gtk_entry_set_placeholder_text(GTK_ENTRY(env_entry), "dev");
  gtk_entry_set_placeholder_text(GTK_ENTRY(port_entry), "8090");

  // Create a struct to hold the label and buttons
  Widgets *widgets = g_malloc(sizeof(Widgets));
  widgets->status_label = GTK_LABEL(status_label);
  widgets->start_button = GTK_BUTTON(start_button);
  widgets->terminate_button = GTK_BUTTON(terminate_button);
  widgets->restart_button = GTK_BUTTON(restart_button);
  widgets->dark_mode_button = GTK_BUTTON(dark_mode_button);
  widgets->env_entry = GTK_ENTRY(env_entry);
  widgets->port_entry = GTK_ENTRY(port_entry);
  widgets->hermes_checkbox = GTK_CHECK_BUTTON(hermes_checkbox);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *display_box = gtk_grid_new();
  init_display_box(display_box, status_label, widgets, hermes_checkbox);

  GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_append(GTK_BOX(button_box), start_button);
  gtk_box_append(GTK_BOX(button_box), terminate_button);
  gtk_box_append(GTK_BOX(button_box), restart_button);
  gtk_box_append(GTK_BOX(button_box), dark_mode_button);

  // Connect the buttons' "clicked" signals to the callback functions
  g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), widgets);
  g_signal_connect(terminate_button, "clicked", G_CALLBACK(on_terminate_button_clicked), widgets);
  g_signal_connect(restart_button, "clicked", G_CALLBACK(on_restart_button_clicked), widgets);
  g_signal_connect(dark_mode_button, "clicked", G_CALLBACK(on_dark_mode_button_clicked), widgets);

  // Add padding to the main container box
  gtk_widget_set_margin_top(box, 10);
  gtk_widget_set_margin_bottom(box, 10);
  gtk_widget_set_margin_start(box, 10);
  gtk_widget_set_margin_end(box, 10);

  // Add display_box to the top and button_box to the bottom
  gtk_box_append(GTK_BOX(box), display_box);
  gtk_widget_set_vexpand(display_box, TRUE);
  gtk_widget_set_valign(display_box, GTK_ALIGN_START);

  gtk_box_append(GTK_BOX(box), button_box);
  gtk_widget_set_valign(button_box, GTK_ALIGN_END);

  gtk_window_set_title(GTK_WINDOW(win), "Metro Bundler Launcher");
  gtk_window_set_default_size(GTK_WINDOW(win), 400, 100);
  gtk_window_set_resizable(GTK_WINDOW(win), FALSE); // Lock the window size
  gtk_window_set_child(GTK_WINDOW(win), box);

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
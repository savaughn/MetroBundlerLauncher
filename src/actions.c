#include <jansson.h>
#include <sys/stat.h>
#include "actions.h"

static int save_options_to_application_support(const Options *options);
static int update_single_option_to_application_support(const char *key, json_t *value);

static int update_single_option_to_application_support(const char *key, json_t *value)
{
    // Get the home directory
    const char *home = getenv("HOME");
    if (home == NULL)
    {
        return -1;
    }

    // Create the path to the options file
    char *options_path = g_strdup_printf("%s/Library/Application Support/metro-launcher/options.json", home);

    // Read the JSON object from the file
    json_error_t error;
    json_t *root = json_load_file(options_path, 0, &error);
    if (root == NULL)
    {
        return -1;
    }

    // Set the value of the key in the JSON object
    json_object_set_new(root, key, value);

    // Write the JSON object to the file
    FILE *fp = fopen(options_path, "w");
    if (fp == NULL)
    {
        return -1;
    }

    json_dumpf(root, fp, JSON_INDENT(4));
    fclose(fp);

    // Free the allocated memory
    g_free(options_path);
    json_decref(root);

    return 0;
}

int read_options_from_application_support(Options *options)
{
    // Get the home directory
    const char *home = getenv("HOME");
    if (home == NULL)
    {
        return -1;
    }

    // Create the path to the options file
    char *options_path = g_strdup_printf("%s/Library/Application Support/metro-launcher/options.json", home);

    // Read the JSON object from the file
    json_error_t error;
    json_t *root = json_load_file(options_path, 0, &error);
    if (root == NULL)
    {
        return -1;
    }

    // Get the port, prefix, and file values from the JSON object
    json_t *port_json = json_object_get(root, "port");
    json_t *prefix_json = json_object_get(root, "prefix");
    json_t *file_json = json_object_get(root, "file");
    json_t *debugger_enabled_json = json_object_get(root, "debugger_enabled");
    json_t *dark_mode_json = json_object_get(root, "dark_mode");

    // Set the port, prefix, and file values
    options->port = g_strdup(json_string_value(port_json));
    options->prefix = g_strdup(json_string_value(prefix_json));
    options->file = g_strdup(json_string_value(file_json));
    options->debugger_enabled = json_boolean_value(debugger_enabled_json);
    options->dark_mode = json_boolean_value(dark_mode_json);

    // Free the allocated memory
    g_free(options_path);
    json_decref(root);

    return 0;
}

static int save_options_to_application_support(const Options *options)
{
    // Get the home directory
    const char *home = getenv("HOME");
    if (home == NULL)
    {
        return -1;
    }

    // Create the path to the application support directory
    char *app_support_path = g_strdup_printf("%s/Library/Application Support/metro-launcher", home);

    // Create the directory if it doesn't exist
    struct stat st = {0};
    if (stat(app_support_path, &st) == -1)
    {
        mkdir(app_support_path, 0700);
    }

    // Create the path to the options file
    char *options_path = g_strdup_printf("%s/options.json", app_support_path);

    // Create the JSON object
    json_t *root = json_object();
    json_object_set_new(root, "port", json_string(options->port));
    json_object_set_new(root, "prefix", json_string(options->prefix));
    json_object_set_new(root, "file", json_string(options->file));
    json_object_set_new(root, "debugger_enabled", json_boolean(options->debugger_enabled));

    // Write the JSON object to the file
    FILE *fp = fopen(options_path, "w");
    if (fp == NULL)
    {
        return -1;
    }

    json_dumpf(root, fp, JSON_INDENT(4));
    fclose(fp);

    // Free the allocated memory
    g_free(app_support_path);
    g_free(options_path);
    json_decref(root);

    return 0;
}

// Callback function for the start button click
void on_start_button_clicked(GtkButton *button, gpointer data)
{
    Widgets *widgets = (Widgets *)data;

    // Set default values for port_entry and prefix_entry if they are empty
    const char *port_text = gtk_editable_get_text(GTK_EDITABLE(widgets->port_entry));
    if (strlen(port_text) == 0)
    {
        gtk_editable_set_text(GTK_EDITABLE(widgets->port_entry), g_strdup_printf("%d", DEFAULT_PORT));
        port_text = gtk_editable_get_text(GTK_EDITABLE(widgets->port_entry));
    }

    const char *prefix_text = gtk_editable_get_text(GTK_EDITABLE(widgets->prefix_entry));
    if (strlen(prefix_text) == 0)
    {
        gtk_editable_set_text(GTK_EDITABLE(widgets->prefix_entry), "");
        prefix_text = gtk_editable_get_text(GTK_EDITABLE(widgets->prefix_entry));
    }

    int port = atoi(port_text);
    if (port == 0)
    {
        port = DEFAULT_PORT;
    }

    const char *file_text = gtk_editable_get_text(GTK_EDITABLE(widgets->file_entry));
    if (strlen(file_text) == 0)
    {
        gtk_editable_set_text(GTK_EDITABLE(widgets->file_entry), "");
        file_text = gtk_editable_get_text(GTK_EDITABLE(widgets->file_entry));
    }

    char *p_cmd = g_strdup_printf("cd %s && %s npx react-native start --port %d", file_text, prefix_text, port);

    // append hermes flag if enabled
    gboolean debugger_enabled = gtk_check_button_get_active(GTK_CHECK_BUTTON(widgets->hermes_checkbox));
    if (debugger_enabled)
    {
        p_cmd = g_strdup_printf("%s --experimental-debugger", p_cmd);
    }

    char *osascript_cmd = g_strdup_printf(
        "osascript -e 'tell application \"Terminal\" to do script \"%s\"'", p_cmd);

    // Execute the osascript command to open a new Terminal window and run the command
    system(osascript_cmd);

    // Free the allocated memory for cmd and osascript_cmd
    g_free(p_cmd);
    g_free(osascript_cmd);

    // Immediately disable the start button and port entry
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->start_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->port_entry), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->prefix_entry), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->hermes_checkbox), FALSE);

    Options options = {port_text, prefix_text, file_text, debugger_enabled};
    save_options_to_application_support(&options);

    // Set up a periodic check for the connection and update the label
    g_timeout_add(1000, check_connection, widgets); // Check every 1 second
}

// Callback function for the terminate button click
void on_terminate_button_clicked(GtkButton *button, gpointer data)
{
    Widgets *widgets = (Widgets *)data;

    // Terminate the process running on the specified port
    system("pkill -f 'react-native'");

    // Update the label and button states
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->start_button), TRUE);      // Enable the start button
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->terminate_button), FALSE); // Disable the terminate button
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->restart_button), FALSE);   // Disable the restart button
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->prefix_entry), TRUE);      // Enable the IP entry
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->port_entry), TRUE);        // Enable the port entry
    gtk_widget_set_sensitive(GTK_WIDGET(widgets->hermes_checkbox), TRUE);
}

// Callback function for the restart button click
void on_restart_button_clicked(GtkButton *button, gpointer data)
{
    // Terminate the process running on the specified port
    system("pkill -f 'react-native'");

    // Start the process again
    on_start_button_clicked(button, data);
}

void on_dark_mode_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkSettings *settings = gtk_settings_get_default();
    static gboolean dark_mode;
    g_object_get(settings, "gtk-application-prefer-dark-theme", &dark_mode, NULL);
    g_object_set(settings, "gtk-application-prefer-dark-theme", !dark_mode, NULL);

    update_single_option_to_application_support("dark_mode", dark_mode ? json_boolean(false) : json_boolean(true));
}
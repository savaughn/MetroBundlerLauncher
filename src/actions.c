#include <jansson.h>
#include <sys/stat.h>
#include "actions.h"
#include "widget_state.h"

// Private
static int update_single_option_to_application_support(const char *key, json_t *value);
static gboolean get_json_dark_mode_setting(void);
static gboolean is_valid_directory(const char *path);

static gboolean is_valid_directory(const char *path)
{
    // Copy the path to a new string to avoid modifying the original
    char *_path = g_strdup(path);

    // replace ~ with $HOME
    if (_path[0] == '~')
    {
        const char *home = getenv("HOME");
        if (home == NULL)
        {
            g_free(_path);
            return FALSE;
        }
        char *new_path = g_build_filename(home, _path + 1, NULL);
        g_free(_path);
        _path = new_path;
    }

    // Remove trailing slashes
    size_t len = strlen(_path);
    while (len > 0 && _path[len - 1] == '/')
    {
        _path[len - 1] = '\0';
        len--;
    }

    // Check for package.json file in the specified directory
    char *package_json_path = g_build_filename(_path, "package.json", NULL);
    g_free(_path);

    struct stat st = {0};
    if (stat(package_json_path, &st) == -1)
    {
        g_free(package_json_path);
        return FALSE;
    }

    g_free(package_json_path);
    return TRUE;
}

void validate_entry_widget(GtkWidget *entry, gpointer data)
{
    Widgets *widgets = (Widgets *)data;

    GtkCssProvider *provider = gtk_css_provider_new();
    const char *error_css = ".error {border-color: red;}";
    gtk_css_provider_load_from_string(provider, error_css);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    if (strlen(gtk_editable_get_text(GTK_EDITABLE(entry))) == 0 ||
        !is_valid_directory(gtk_editable_get_text(GTK_EDITABLE(entry))))
    {
        gtk_widget_add_css_class(entry, "error");
        update_widget_ui_state(widgets, STATE_ERROR);
    }
    else
    {
        gtk_widget_remove_css_class(entry, "error");
        update_widget_ui_state(widgets, STATE_IDLE);
    }

    g_object_unref(provider);
}

static gboolean get_json_dark_mode_setting()
{
    GtkSettings *settings = gtk_settings_get_default();
    gboolean dark_mode;
    g_object_get(settings, "gtk-application-prefer-dark-theme", &dark_mode, NULL);

    return dark_mode;
}

static int update_single_option_to_application_support(const char *key, json_t *value)
{
    // Get the home directory
    const char *home = getenv("HOME");
    if (home == NULL)
    {
        return -1;
    }

    // Create the path to the options file
    char *options_path = g_strdup_printf("%s/Library/Application Support/MetroBundlerLauncher/options.json", home);

    // Read the JSON object from the file
    json_error_t error;
    json_t *root = json_load_file(options_path, 0, &error);
    if (root == NULL)
    {
        g_warning("Failed to load JSON from file: %s", error.text);
        g_free(options_path);
        return -1;
    }

    json_object_set_new(root, key, value);

    FILE *fp = fopen(options_path, "w");
    if (fp == NULL)
    {
        g_warning("Failed to open file for writing: %s", options_path);
        g_free(options_path);
        json_decref(root);
        return -1;
    }

    json_dumpf(root, fp, JSON_INDENT(4));
    fclose(fp);

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
    char *options_path = g_strdup_printf("%s/Library/Application Support/MetroBundlerLauncher/options.json", home);

    // Read the JSON object from the file
    json_error_t error;
    json_t *root = json_load_file(options_path, 0, &error);
    if (root == NULL)
    {
        g_warning("Failed to load JSON from file: %s", error.text);
        g_free(options_path);
        return -1;
    }

    json_t *port_json = json_object_get(root, "port");
    json_t *prefix_json = json_object_get(root, "prefix");
    json_t *file_json = json_object_get(root, "file");
    json_t *debugger_enabled_json = json_object_get(root, "debugger_enabled");
    json_t *dark_mode_json = json_object_get(root, "dark_mode");

    options->port = g_strdup(json_string_value(port_json));
    options->prefix = g_strdup(json_string_value(prefix_json));
    options->file = g_strdup(json_string_value(file_json));
    options->debugger_enabled = json_boolean_value(debugger_enabled_json);
    options->dark_mode = json_boolean_value(dark_mode_json);

    g_free(options_path);
    json_decref(root);

    return 0;
}

int save_options_to_application_support(const Options *options)
{
    // Get the home directory
    const char *home = getenv("HOME");
    if (home == NULL)
    {
        return -1;
    }

    // Create the path to the application support directory
    char *app_support_path = g_strdup_printf("%s/Library/Application Support/MetroBundlerLauncher", home);

    struct stat st = {0};
    if (stat(app_support_path, &st) == -1)
    {
        mkdir(app_support_path, 0700);
    }

    char *options_path = g_strdup_printf("%s/options.json", app_support_path);

    json_t *root = json_object();
    json_object_set_new(root, "port", json_string(options->port));
    json_object_set_new(root, "prefix", json_string(options->prefix));
    json_object_set_new(root, "file", json_string(options->file));
    json_object_set_new(root, "debugger_enabled", json_boolean(options->debugger_enabled));
    json_object_set_new(root, "dark_mode", json_boolean(options->dark_mode));

    // Write the JSON object to the file
    FILE *fp = fopen(options_path, "w");
    if (fp == NULL)
    {
        g_warning("Failed to open file for writing: %s", options_path);
        g_free(app_support_path);
        g_free(options_path);
        json_decref(root);

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
    gboolean debugger_enabled = gtk_check_button_get_active(GTK_CHECK_BUTTON(widgets->hermes_checkbox));
    char *osascript_cmd = g_strdup_printf(
        "osascript -e 'tell application \"Terminal\" to do script \"cd %s && %s npx react-native start --port %d%s\"'",
        file_text,
        prefix_text,
        port,
        debugger_enabled ? " --experimental-debugger" : "");
    if (osascript_cmd == NULL)
    {
        g_free(osascript_cmd);
        return;
    }

    // Execute the osascript command to open a new Terminal window and run the command
    GError *error = NULL;
    g_spawn_command_line_async(osascript_cmd, &error);
    if (error != NULL)
    {
        g_printerr("Error spawning command: %s\n", error->message);
        g_error_free(error);
    }

    g_free(osascript_cmd);

    // Immediately disable the UI
    update_widget_ui_state(widgets, STATE_RUNNING);

    // Update the options with the new values from the UI input fields
    Options options = {
        .port = g_strdup_printf("%d", port),
        .prefix = g_strdup(prefix_text),
        .file = g_strdup(file_text),
        .debugger_enabled = debugger_enabled,
        .dark_mode = get_json_dark_mode_setting()};

    if (save_options_to_application_support(&options) != 0)
    {
        g_warning("Failed to save options to application support");

        g_free((gpointer)options.port);
        g_free((gpointer)options.prefix);
        g_free((gpointer)options.file);
        return;
    }

    g_free((gpointer)options.port);
    g_free((gpointer)options.prefix);
    g_free((gpointer)options.file);

    // Set up a periodic check for the connection and update the label
    g_timeout_add(1000, check_connection, widgets); // Check every 1 second
}

// Callback function for the terminate button click
void on_terminate_button_clicked(GtkButton *button, gpointer data)
{
    Widgets *widgets = (Widgets *)data;

    // Terminate the process running on the specified port
    gchar *output = NULL;
    gchar *error = NULL;
    gint exit_status;
    g_spawn_command_line_sync("pkill -f 'react-native'", &output, &error, &exit_status, NULL);
    if (exit_status != 0)
    {
        g_warning("Failed to terminate process: %s", error);
    }
    g_free(output);

    update_widget_ui_state(widgets, STATE_IDLE);
}

// Callback function for the restart button click
void on_restart_button_clicked(GtkButton *button, gpointer data)
{
    // Terminate the process running on the specified port
    gchar *output = NULL;
    gchar *error = NULL;
    gint exit_status;
    g_spawn_command_line_sync("pkill -f 'react-native'", &output, &error, &exit_status, NULL);
    if (exit_status != 0)
    {
        g_warning("Failed to terminate process: %s", error);
    }
    g_free(output);

    // Start the process again
    on_start_button_clicked(button, data);
}

void on_dark_mode_button_clicked(GtkButton *button, gpointer data)
{
    GtkSettings *settings = gtk_settings_get_default();
    gboolean dark_mode;
    g_object_get(settings, "gtk-application-prefer-dark-theme", &dark_mode, NULL);
    g_object_set(settings, "gtk-application-prefer-dark-theme", !dark_mode, NULL);

    // Sync context options struct
    Options *options = (Options *)data;
    options->dark_mode = !dark_mode;

    // Sync dark mode setting to application support
    update_single_option_to_application_support("dark_mode", dark_mode ? json_boolean(false) : json_boolean(true));
}
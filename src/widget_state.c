#include "widget_state.h"

void update_widget_ui_state(Widgets *widgets, WidgetsState new_state)
{
    switch (new_state)
    {
    case STATE_IDLE:
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->file_entry), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->prefix_entry), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->port_entry), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->hermes_checkbox), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->start_button), TRUE);

        gtk_widget_set_sensitive(GTK_WIDGET(widgets->terminate_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->restart_button), FALSE);

        break;
    case STATE_RUNNING:
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->start_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->file_entry), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->port_entry), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->prefix_entry), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->hermes_checkbox), FALSE);

        gtk_widget_set_sensitive(GTK_WIDGET(widgets->terminate_button), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->restart_button), TRUE);
        break;
    case STATE_ERROR:
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->file_entry), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->prefix_entry), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->port_entry), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->hermes_checkbox), TRUE);

        gtk_widget_set_sensitive(GTK_WIDGET(widgets->start_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->terminate_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->restart_button), FALSE);
        break;
    default:
        break;
    }
}
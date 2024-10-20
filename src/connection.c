#include "connection.h"
#include <cairo.h>

// Function to check if something is running on a given port
int is_port_open(int port)
{
  int sockfd;
  struct sockaddr_in serv_addr;

  // Create socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    return 0;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    close(sockfd);
    return 0;
  }

  // Attempt to connect
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    close(sockfd);
    return 0;
  }

  // Close the socket
  close(sockfd);
  return 1;
}

// Function to periodically check the connection and update the label
gboolean check_connection(gpointer data)
{
    Widgets *widgets = (Widgets *)data;
    const char *port_text = gtk_editable_get_text(GTK_EDITABLE(widgets->port_entry));
    int port = atoi(port_text);
    if (port == 0)
    {
        port = DEFAULT_PORT;
    }
    char *label_text;
    if (is_port_open(port))
    {
        label_text = g_strdup_printf("Metro bundler is running on localhost:%d", port);
        gtk_label_set_text(widgets->status_label, label_text);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->start_button), FALSE);    // Disable the start button
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->terminate_button), TRUE); // Enable the terminate button
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->restart_button), TRUE);   // Enable the restart button
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->port_entry), FALSE);      // Disable the port entry
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->prefix_entry), FALSE);        // Disable the IP entry
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->hermes_checkbox), FALSE); // Disable the Hermes checkbox
    }
    else
    {
        label_text = g_strdup_printf("No Metro bundler is running on localhost:%d", port);
        gtk_label_set_text(widgets->status_label, label_text);
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->start_button), TRUE);      // Enable the start button
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->terminate_button), FALSE); // Disable the terminate button
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->restart_button), FALSE);   // Disable the restart button
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->port_entry), TRUE);        // Enable the port entry
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->prefix_entry), TRUE);          // Enable the IP entry
        gtk_widget_set_sensitive(GTK_WIDGET(widgets->hermes_checkbox), TRUE);   // Enable the Hermes checkbox
    }
    g_free(label_text);

    return TRUE; // Continue calling this function
}

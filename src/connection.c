#include "connection.h"
#include "draw.h"
#include "widget_state.h"

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

    if (is_port_open(port))
    {
        draw_green_status_light(widgets);
        gtk_label_set_text(widgets->port_label, port_text);
        update_widget_ui_state(widgets, STATE_RUNNING);
    }
    else
    {
        draw_red_status_light(widgets);
        gtk_label_set_text(widgets->port_label, port_text);
        update_widget_ui_state(widgets, STATE_IDLE);
        if (strcmp(gtk_editable_get_text(GTK_EDITABLE(widgets->file_entry)), "") == 0)
        {
            update_widget_ui_state(widgets, STATE_ERROR);
        }
    }

    return TRUE; // Continue calling this function
}

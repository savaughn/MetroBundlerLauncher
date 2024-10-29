#include "button.h"

#define BUTTON_CSS ".icon-button { background: none; border: none; box-shadow: none; padding: 0; }"

GtkWidget *create_button(ButtonType type)
{
  const char *icon_name = NULL;
  const char *tooltip_text = NULL;

  switch (type)
  {
  case BUTTON_TYPE_START:
    icon_name = "media-playback-start";
    tooltip_text = "Start";
    break;
  case BUTTON_TYPE_TERMINATE:
    icon_name = "media-playback-stop";
    tooltip_text = "Stop";
    break;
  case BUTTON_TYPE_RESTART:
    icon_name = "media-playlist-repeat";
    tooltip_text = "Restart";
    break;
  case BUTTON_TYPE_DARK_MODE:
    icon_name = "dialog-information";
    tooltip_text = "Toggle Dark Mode";
    break;
  default:
    g_warning("Unexpected ButtonType: %d", type);
    return NULL;
  }

  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_string(provider, BUTTON_CSS);
  gtk_style_context_add_provider_for_display(
      gdk_display_get_default(),
      GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_USER);

  GtkWidget *button = gtk_button_new();
  gtk_widget_add_css_class(GTK_WIDGET(button), "icon-button");
  gtk_widget_set_tooltip_text(GTK_WIDGET(button), tooltip_text);
  gtk_button_set_icon_name(GTK_BUTTON(button), icon_name);
  gtk_widget_set_can_focus(GTK_WIDGET(button), FALSE);

  return button;
}
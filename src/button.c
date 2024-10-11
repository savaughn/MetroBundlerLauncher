#include "button.h"

GtkWidget *create_button(ButtonType type)
{
  const char *icon_name;
  const char *label_text;
  const char *tooltip_text;

  switch (type)
  {
  case BUTTON_TYPE_START:
    icon_name = "media-playback-start";
    label_text = "Start Metro Bundler";
    tooltip_text = "Start";
    break;
  case BUTTON_TYPE_TERMINATE:
    icon_name = "media-playback-stop";
    label_text = "Terminate Metro Bundler";
    tooltip_text = "Stop";
    break;
  case BUTTON_TYPE_RESTART:
    icon_name = "media-playlist-repeat";
    label_text = "Restart Metro Bundler";
    tooltip_text = "Restart";
    break;
  case BUTTON_TYPE_DARK_MODE:
    icon_name = "dialog-information";
    label_text = "Toggle Dark Mode";
    tooltip_text = "Toggle Dark Mode";
    break;
  default:
    return NULL;
  }
  
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "style.css");
  gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                             GTK_STYLE_PROVIDER(provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_USER);

  GtkWidget *button = gtk_button_new();
  gtk_widget_add_css_class(button, "icon-button");
  gtk_widget_set_tooltip_text(button, tooltip_text);
  gtk_button_set_icon_name(GTK_BUTTON(button), icon_name);
  gtk_widget_set_can_focus(button, FALSE);

  return button;
}
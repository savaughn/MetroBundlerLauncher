#include "draw.h"

void draw_circle(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data)
{
    GdkRGBA *color = (GdkRGBA *)user_data;

    // Set the background color to transparent
    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.0);
    cairo_paint(cr);

    // Calculate the center and radius of the circle
    double radius = MIN(width, height) / 10.0;
    double center_x = width / 2.0;
    double center_y = height / 2.0;

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

void draw_red_status_light(Widgets *widgets)
{
    GdkRGBA *red = g_malloc(sizeof(GdkRGBA));
    red->red = 1.0;
    red->green = 0.0;
    red->blue = 0.0;
    red->alpha = 1.0;

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(widgets->status_light), draw_circle, red, g_free);
}

void draw_green_status_light(Widgets *widgets)
{
    GdkRGBA *green = g_malloc(sizeof(GdkRGBA));
    green->red = 0.0;
    green->green = 1.0;
    green->blue = 0.0;
    green->alpha = 1.0;

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(widgets->status_light), draw_circle, green, g_free);
}

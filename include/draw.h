#ifndef DRAW_H
#define DRAW_H

#include "common.h"

void draw_circle(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);
void draw_red_status_light(Widgets *widgets);
void draw_green_status_light(Widgets *widgets);

#endif // DRAW_H
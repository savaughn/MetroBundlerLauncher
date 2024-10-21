#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h> // Include for fabs()
#include <stdlib.h> // Include for malloc and free

#define EPSILON 0.0001

typedef struct RGBA {
    float red;
    float green;
    float blue;
    float alpha;
} RGBA;

static void draw_circle(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    RGBA *color = (RGBA *)user_data;

    // Log color values to check what's being passed
    printf("Color1: %f, %f, %f, %f\n", color->red, color->green, color->blue, color->alpha);

    g_assert_true(fabs(color->red - 1.0) < EPSILON);
    g_assert_true(fabs(color->green - 0.0) < EPSILON);
    g_assert_true(fabs(color->blue - 0.0) < EPSILON);
    g_assert_true(fabs(color->alpha - 1.0) < EPSILON);
}

static void on_activate(GApplication *app) {
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    // Allocate color on the heap
    RGBA *color = g_malloc(sizeof(RGBA));
    color->red = 1.0; 
    color->green = 0.0;
    color->blue = 0.0;
    color->alpha = 1.0;

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 300, 300);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_circle, color, g_free); // Free the color when done
    gtk_window_set_child(GTK_WINDOW(window), drawing_area);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("org.example.drawcircle", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}

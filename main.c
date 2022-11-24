#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "NeuralNetwork/main.h"
#include "Image/pre_processing/transformation.h"
#include "Image/Display/display.h"

typedef struct UserInterface
{
    GtkWindow* window;                  // Main window
    GtkButton* load;                    // Load button
    GtkButton *run;
    GtkImage* image;
    char* image_path;
    SDL_Surface** segmentation;

} UserInterface;


//----------------------LOAD IMAGE-----------------------

// Event handler for the "clicked" signal of the load button.
void UI_load_image(GtkButton *button, gpointer user_data)
{
    button = button;

    UserInterface* UI = user_data;

    GtkWidget* dialog;

    //create file chooser dialog
    dialog = gtk_file_chooser_dialog_new("Choose a file",
                                         UI->window,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "OK",
                                         GTK_RESPONSE_OK,
                                         "CANCEL",
                                         GTK_BUTTONS_CANCEL,
                                         NULL);

    //show da box
    gtk_widget_show_all(dialog);


    //get the response
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    //if file chosen
    if(response == GTK_RESPONSE_OK)
    {

        //TODO :

        //WARNING WE CAN CHOOSE EVERYTHING
        //WE ONLY WANT IMAGEs

        //get pixbuf of image
        UI->image_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        GdkPixbuf* image = gdk_pixbuf_new_from_file(UI->image_path, NULL);

        //rescale
        image = gdk_pixbuf_scale_simple (
            image,
            350,
            350,
            GDK_INTERP_BILINEAR);

        //set image
        gtk_image_set_from_pixbuf(UI->image,image);
    }

    //destroy dialog box
    gtk_widget_destroy(dialog);

}

//-----------------------PROCESS--------------------------

void Process_image(GtkButton *button, gpointer user_data)
{
    button = button;
    UserInterface* UI = user_data;
    UI = UI;

    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(UI->image_path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    transformation(surface, UI->segmentation);
    
}


//----------------------MAIN-------------------------------

int main (int argc, char *argv[])
{


    argc = argc;
    argv = argv;

    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "OCR.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkButton* load = GTK_BUTTON(gtk_builder_get_object(builder, "load"));
    GtkButton* run = GTK_BUTTON(gtk_builder_get_object(builder, "run"));
    GtkImage* image = GTK_IMAGE(gtk_builder_get_object(builder, "image_displayed"));

    UserInterface UI =
    {
        .window = window,
        .load = load,
        .run = run,
        .image = image,
        .image_path = NULL,
        .segmentation = malloc(81*sizeof(SDL_Surface*)),
    };

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(load, "clicked", G_CALLBACK(UI_load_image), &UI);
    g_signal_connect(run, "clicked", G_CALLBACK(Process_image), &UI);

    // Runs the main loop.
    gtk_main();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "NeuralNetwork/main.h"
#include "Image/Rotation_Resize/rotation.h"
#include "Image/pre_processing/transformation.h"
#include "Image/Display/display.h"

typedef struct UserInterface
{
    GtkWindow* window;                  // Main window
    GtkButton* load;                    // Load button
    GtkButton *run;
    GtkButton *quit;
    GtkButton *save;
    GtkImage* image;
    GtkSpinButton* rotation;
    char* image_path;
    SDL_Surface** segmentation;

} UserInterface;

//----------------------UPDATE IMAGE--------------------

void update_image(GtkImage* image, char* path)
{
    GdkPixbuf* pix = gdk_pixbuf_new_from_file(path, NULL);

    //rescale
    pix = gdk_pixbuf_scale_simple (
        pix,
        350,
        350,
        GDK_INTERP_BILINEAR);

    //set image
    gtk_image_set_from_pixbuf(image,pix);
}


//----------------------ROTATION IMAGE------------------

void Rotate_image(GtkSpinButton* rota, gpointer user_data)
{
    UserInterface* UI = user_data;

    double angle = gtk_spin_button_get_value(rota);

    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(UI->image_path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    rotate(surface, angle);

    save_image(surface,"test_rotation.png");

    update_image(UI->image,"test_rotation.png");
}

void Save_Rota(GtkButton* button, gpointer user_data)
{
    button = button;
    UserInterface* UI = user_data;
    UI->image_path = ("test_rotation.png");
}


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

        update_image(UI->image,UI->image_path);
    }

    //destroy dialog box
    gtk_widget_destroy(dialog);

}

//-----------------------PROCESS--------------------------

void Process_image(GtkButton *button, gpointer user_data)
{
    button = button;
    UserInterface* UI = user_data;

    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(UI->image_path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    transformation(surface, UI->segmentation);

    save_image(surface,"test_processed.png");

    update_image(UI->image, "test_processed.png");
    UI->image_path = ("test_processed.png");
}


//----------------------MAIN-------------------------------

int main ()
{

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
    GtkButton* save = GTK_BUTTON(gtk_builder_get_object(builder, "save"));
    GtkButton* quit = GTK_BUTTON(gtk_builder_get_object(builder, "quit"));
    GtkButton* run = GTK_BUTTON(gtk_builder_get_object(builder, "run"));
    GtkImage* image = GTK_IMAGE(gtk_builder_get_object(builder, "image_displayed"));
    GtkSpinButton* rota =  GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "rota"));

    UserInterface UI =
    {
        .window = window,
        .load = load,
        .save = save,
        .quit = quit,
        .run = run,
        .image = image,
        .rotation = rota,
        .image_path = NULL,
        .segmentation = malloc(81*sizeof(SDL_Surface*)),
    };

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(save, "clicked", G_CALLBACK(Save_Rota), &UI);
    g_signal_connect(load, "clicked", G_CALLBACK(UI_load_image), &UI);
    g_signal_connect(run, "clicked", G_CALLBACK(Process_image), &UI);
    g_signal_connect(rota, "value_changed", G_CALLBACK(Rotate_image), &UI);

    // Runs the main loop.
    gtk_main();

    free((UI.segmentation));

    return 0;
}

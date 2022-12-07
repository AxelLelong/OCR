#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "NeuralNetwork/main.h"
#include "Solver/sudoLoader.h"
#include "Solver/sudoSolver.h"
#include "Image/Rotation_Resize/rotation.h"
#include "Image/pre_processing/transformation.h"
#include "Image/Display/display.h"

typedef struct UserInterface
{
    GtkWindow* window;                  // Main window
    GtkButton* load;                    // Load button
    GtkButton *run;
    GtkButton *quit;
    GtkButton *resolve;
    //GtkButton *save;
    GtkImage* image;
    GtkImage* result;
    GtkImage** images;
    SDL_Surface ** sur;
    GtkSpinButton* rotation;
    char* image_path;
    int* sudoMat;
    SDL_Surface** segmentation;

} UserInterface;

//-----------------------SAVE FINAL---------------------

void save_image_final(UserInterface* UI)
{
    int w = 28*9;
    SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_SWSURFACE, w, w, 32, 0, 0, 0, 0);

    Uint32* pix = new_img->pixels;
    if (pix == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int pos_d = 0;
    int pos_lig_d = -28;
    SDL_LockSurface(new_img);
    for(int i = 0;i<81;i++)
    {
        pos_d = 28*i%(28*9);
        if(i%9 == 0)
            pos_lig_d+=28;

        Uint32* number = UI->sur[i]->pixels;
        if (number == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        int l_i = 0;
        int pos_i = pos_d;
        for(int j = 0;j<28;j++)
        {

            for(int x = 0; x < 28; x++)
            {
                pos_i = pos_d + x;
                pix[(pos_d+(pos_i%28))+(pos_lig_d+l_i)*w] = number[j*28+x];
            }

            l_i++;

        }
    }

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < w; j++)
        {
            //CONTOUR
            if(i < 3 || i > (9*28)-4 || j < 3 || j > (9*28)-4)
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);

            // BIG TRAIT COLONNE
            if(i <= (28*9)/3+1 && i >= (28*9)/3-1)
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);

            if(i <= 2*((28*9)/3)+1 && i >= 2*((28*9)/3)-1)
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);

            //BIG TRAIT LIGNE
            if(j <= (28*9)/3+1 && j >= (28*9)/3-1)
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);

            if(j <= 2*((28*9)/3)+1 && j >= 2*((28*9)/3)-1)
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);

            //SMALL COL
            if(i == (28*9)/9)
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(i == 2*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(i == 4*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(i == 5*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(i == 7*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(i == 8*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);

            //SMALL LIGNE
            if(j == (28*9)/9)
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(j == 2*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(j == 4*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(j == 5*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(j == 7*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);
            if(j == 8*((28*9)/9))
                pix[i*w+j] = SDL_MapRGB(new_img->format,0,0,0);



        }
    }
    SDL_UnlockSurface(new_img);

    save_image(new_img,"test_sudoku_final.png");
    SDL_FreeSurface(new_img);
}


//----------------------UPDATE IMAGE--------------------

void update_image(GtkImage* image, char* path,int width)
{
    GdkPixbuf* pix = gdk_pixbuf_new_from_file(path, NULL);

    //rescale
    pix = gdk_pixbuf_scale_simple (
        pix,
        width,
        width,
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

    update_image(UI->image,"test_rotation.png",350);
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

    gtk_widget_set_visible(GTK_WIDGET(UI->image),1);
    gtk_widget_set_visible(GTK_WIDGET(UI->result),0);
    for(int i = 0; i<81;i++)
    {
        gtk_widget_set_visible(GTK_WIDGET(UI->images[i]),0);
        update_image(UI->images[i], "Numbers/0.png",28);
    }
    gtk_widget_set_visible(GTK_WIDGET(UI->rotation),1);
    gtk_widget_set_sensitive(GTK_WIDGET(UI->run),1);
    gtk_widget_set_sensitive(GTK_WIDGET(UI->resolve),0);
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

        update_image(UI->image,UI->image_path,350);
    }

    //destroy dialog box
    gtk_widget_destroy(dialog);

}

//-----------------------PROCESS--------------------------

void Process_image(GtkButton *button, gpointer user_data)
{
    button = button;
    UserInterface* UI = user_data;

    gtk_widget_set_visible(GTK_WIDGET(UI->rotation),0);
    gtk_widget_set_sensitive(GTK_WIDGET(UI->resolve),1);
    gtk_widget_set_sensitive(GTK_WIDGET(UI->run),0);

    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(UI->image_path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    transformation(surface, UI->segmentation);

    save_image(surface,"test_processed.png");
    //print numbers
    /*char path[21];
    for(int i = 0; i < 81;i++)
    {
        sprintf(path,"test_%i.png",i);
        save_image(UI->segmentation[i],path);
        }*/
    update_image(UI->image, "test_processed.png",350);
    UI->image_path = ("test_processed.png");

    mainNN(0,0,0,1,1,UI->segmentation,UI->sudoMat);
    Writer(UI->sudoMat);
}

//----------------------RUN AND COMPUTE RESULT-------------

void UI_resolve(GtkButton* button, gpointer user_data)
{
    button = button;
    UserInterface* UI = user_data;
    int* bef = UI->sudoMat;
    GtkImage** images = UI->images;
    int* marquage = calloc(81,sizeof(int));


    Loader("sudo_resolved",bef);


    for(int i = 0; i<81;i++)
    {
        if (bef[i] != 0)
        {
            char str[21];
            sprintf(str,"Numbers/num%ib.png",bef[i]);
            update_image(images[i], str,28);

            SDL_Surface * source = IMG_Load(str);
            if (source == NULL)
                errx(EXIT_FAILURE, "%s", SDL_GetError());
            SDL_Surface* surface = SDL_ConvertSurfaceFormat
                (source, SDL_PIXELFORMAT_RGB888, 0);
            if (surface == NULL)
                errx(EXIT_FAILURE, "%s", SDL_GetError());
            SDL_FreeSurface(source);
            UI->sur[i] = surface;
            marquage[i] = 1;
        }
    }

    int solve = Solve(UI->sudoMat);
    if (solve)
    {
        Writer(UI->sudoMat);
        for (int i = 0; i < 81; i++)
        {
            if (marquage[i] == 0)
            {
                char str[21];
                sprintf(str, "Numbers/num%ir.png", bef[i]);
                update_image(images[i], str,28);

                SDL_Surface * source = IMG_Load(str);
                if (source == NULL)
                    errx(EXIT_FAILURE, "%s", SDL_GetError());
                SDL_Surface* surface = SDL_ConvertSurfaceFormat
                    (source, SDL_PIXELFORMAT_RGB888, 0);
                if (surface == NULL)
                    errx(EXIT_FAILURE, "%s", SDL_GetError());
                SDL_FreeSurface(source);
                UI->sur[i] = surface;
            }
        }
    }

    gtk_widget_set_visible(GTK_WIDGET(UI->image),0);
    gtk_widget_set_visible(GTK_WIDGET(UI->rotation),0);
    gtk_widget_set_visible(GTK_WIDGET(UI->result),1);
    gtk_widget_set_sensitive(GTK_WIDGET(UI->resolve),0);
    gtk_widget_set_sensitive(GTK_WIDGET(UI->run),0);
    for(int i = 0; i<81;i++)
    {
        gtk_widget_set_visible(GTK_WIDGET(UI->images[i]),1);
    }
    free(marquage);

    save_image_final(UI);
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
    //GtkButton* save = GTK_BUTTON(gtk_builder_get_object(builder, "save"));
    GtkButton* quit = GTK_BUTTON(gtk_builder_get_object(builder, "quit"));
    GtkButton* run = GTK_BUTTON(gtk_builder_get_object(builder, "run"));
    GtkButton* resolve = GTK_BUTTON(gtk_builder_get_object(builder, "resolve"));
    GtkImage* image = GTK_IMAGE(gtk_builder_get_object(builder, "image_displayed"));
    GtkImage* result = GTK_IMAGE(gtk_builder_get_object(builder, "result"));
    GtkSpinButton* rota =  GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "rota"));
    GtkImage** images = malloc(81*sizeof(GtkImage*));
    for(int i = 0; i<81;i++)
    {
        char str[21];
        sprintf(str,"%i",i);
        images[i] = GTK_IMAGE(gtk_builder_get_object(builder,str));
    }

    UserInterface UI =
    {
        .window = window,
        .load = load,
        //.save = save,
        .quit = quit,
        .run = run,
        .resolve = resolve,
        .image = image,
        .result = result,
        .sur = malloc(81*sizeof(SDL_Surface*)),
        .images = images,
        .rotation = rota,
        .image_path = NULL,
        .segmentation = malloc(81*sizeof(SDL_Surface*)),
        .sudoMat = malloc(81*sizeof(int))
    };

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(save, "clicked", G_CALLBACK(Save_Rota), &UI);
    g_signal_connect(load, "clicked", G_CALLBACK(UI_load_image), &UI);
    g_signal_connect(run, "clicked", G_CALLBACK(Process_image), &UI);
    g_signal_connect(resolve, "clicked", G_CALLBACK(UI_resolve), &UI);
    g_signal_connect(rota, "value_changed", G_CALLBACK(Rotate_image), &UI);

    // Runs the main loop.
    gtk_main();

    free((UI.sudoMat));
    free((UI.sur));
    free((UI.segmentation));
    free(UI.images);

    return 0;
}

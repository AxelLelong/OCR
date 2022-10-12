#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "filters.h"

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);
}

void save_image(SDL_Surface *image_surface, char *path)
{
    printf("<--💾 Saving output to %s\n", path);

    if (IMG_SavePNG(image_surface, path) != 0)
        errx(EXIT_FAILURE, "could not save the image to '%s': %s.\n", path,
            SDL_GetError());
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Surface * image_surface,char* path)
{
    SDL_Event event;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
	        save_image(image_surface,path);
	        return;

            // If the window is resized, updates and redraws the image.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
		    draw(renderer,texture);
                }
                break;
        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface * source = IMG_Load(path);
    if (source == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* surface = SDL_ConvertSurfaceFormat
    (source, SDL_PIXELFORMAT_RGB888, 0);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(source);
    return surface;
}


void surface_to_grayscale(SDL_Surface* surface)
{

    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    int w = surface->w;
    int h = surface->h;
    int len = w * h;
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);
    for(int i = 0;i<len;i++)
    {
        Uint32 tmp = pixel_to_grayscale(pixels[i],format);
        pixels[i] = contrastefilter(tmp,format);
    }

    Uint8 max = get_max(pixels,len,format);
    NormLight(pixels, format, len, max);

    Uint32* pixels1 = malloc(len*sizeof(Uint32));
    if (pixels1 == NULL)
        errx(EXIT_FAILURE, "C'est de la faute de pixels1 pendant le mallocul");


    medianfilter(pixels,pixels1,format,w,h);

    Uint32* pixels2 = malloc(len*sizeof(Uint32));
    if (pixels2 == NULL)
        errx(EXIT_FAILURE, "C'est de la faute de pixels2 pendant le mallocul");

    GaussianFlou(pixels1,pixels2,format,w,h);

    //global noise of the picture
    float noise = noiseLevel(pixels2,w,h, format);
    double seuil;
    //seuil adaptatif
    if(noise>300)
        seuil = 0.5;
    else
        seuil = 0.15;

    adaptativeThreshold(pixels2,seuil,w,h, format);
    lissage(pixels2,pixels1,format,w,h);
    //int* pixelsLisses =  lissage(pixels2,w,h);*/
    for (int i = 0; i < len ; ++i)
    {
        pixels[i] = pixels2[i];
    }
    free(pixels1);
    free(pixels2);
    SDL_UnlockSurface(surface);
}


int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file save-path");
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Converting into Grayscale", 0, 0, 640, 400,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window,surface->w,surface->h);

    // - Convert the surface into grayscale.
    surface_to_grayscale(surface);

    // - Create a texture from the image.
    SDL_Texture* texturegrayscale = SDL_CreateTextureFromSurface(renderer,surface);
    if (texturegrayscale == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Dispatch the events.
    event_loop(renderer,texturegrayscale,surface,argv[2]);

    // - Free the surface.
    SDL_FreeSurface(surface);
    
    // - Destroy the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texturegrayscale);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}

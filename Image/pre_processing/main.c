#include <err.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include "display.h"
#include "transformation.h"

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

    // - Convert the surface.
    transformation(surface);


    // - Create a texture from the image.
    SDL_Texture* newtexture = SDL_CreateTextureFromSurface(renderer,surface);
    if (newtexture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Dispatch the events.
    event_loop(renderer,newtexture,surface,argv[2]);

    // - Free the surface.
    SDL_FreeSurface(surface);
    
    // - Destroy the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(newtexture);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}

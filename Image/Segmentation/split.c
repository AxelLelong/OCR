//
// Created by mathieu on 16/11/2022.
//

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "../Rotation_Resize/resize.h"
#include "../Display/display.h"
#include "split.h"

void savesquare(SDL_Surface* surface, unsigned int iall)
{
    char str[12];
    snprintf(str, sizeof(str), "%s%d.png", "test_", iall+1);
    save_image(surface, str);
}

SDL_Surface* cropImage(SDL_Surface* surface, SDL_Rect* rect)
{
    Uint32* _pixels = surface->pixels;
    if (_pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_SWSURFACE, rect->w, rect->h, 32, 0, 0, 0, 0);

    // Allocate pixels form dimension of rect
    Uint32* pixels = new_img->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Copy pixels from surface to new_img
    for (int x = rect->x; x < rect->x + rect->h; x++)
    {
        for (int y = rect->y; y < rect->y + rect->w; y++)
        {
            pixels[(x - rect->x)*rect->w + y - rect->y] = _pixels[x*surface->w+y];
        }
    }
    return new_img;
}

void split(SDL_Surface* surface, SDL_Surface** segmentation)
{
    const unsigned int width = surface->w;
    const unsigned int height = surface->h;

    const unsigned int xincrem = width / 9;
    const unsigned int yincrem = height / 9;

    SDL_Rect block;
    unsigned int iall = 0;
    for (unsigned int y = 0; y < height && iall < 81; y += yincrem)
    {
        for (unsigned int x = 0; x < width && iall < 81;
             x += xincrem, iall++)
        {
            if (y + yincrem <= height && x + xincrem <= width)
            {
                block.x = x;
                block.y = y;
                block.w = xincrem;
                block.h = yincrem;

                SDL_Surface* new_img = cropImage(surface, &block);

                // new_img free in resize
                SDL_Surface* imageresized = resize(new_img, 28, 28);


                //------------Clear image_resized----------------
                //----------------------------------------------

                segmentation[iall] = imageresized;
                savesquare(imageresized,iall);
            }
            else
            {
                iall--;
            }
        }
    }
}
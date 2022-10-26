//
// Created by mathieu on 12/10/2022.
//
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "interpolation.h"
#include "rotation.h"
#include <SDL2/SDL_image.h>

void rotate(SDL_Surface* surface, double degree)
{
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    int w = surface->w;
    int h = surface->h;
    double angle = degree * M_PI / 180.0;
<<<<<<< HEAD
    SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
=======
    if (degree < 90)
    {
        new_w = (w * cos(angle) + (h * sin(angle)));
        new_h = (w * sin(angle) + (h * cos(angle)));
    }
    else if (degree == 90)
    {
        new_w = h;
        new_h = w;
    }
    else // theta > 90
    {
        int tmp_h = w;
        int tmp_w = h;
        degree = degree - 90;
        new_w = (tmp_w * cos(angle) + (tmp_h * sin(angle)));
        new_h = (tmp_w * sin(angle) + (tmp_h * cos(angle)));
    }
    SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_SWSURFACE, new_w, new_h, 32, 0, 0, 0, 0);
>>>>>>> 45ae5b0 (salut)
    Uint32* _pixels = new_img->pixels;
    if (_pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_PixelFormat* new_format = new_img->format;
    if (new_format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);
    int mid_w = (w / 2.0);
    int mid_h = (h / 2.0);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            // Calculate new position with matrix rotation
            int x = mid_h + (i - mid_h) * cos(angle) - (j - mid_w) * sin(angle);
            int y = mid_w + (j - mid_w) * cos(angle) + (i - mid_h) * sin(angle);
            if (x >= 0 && x < h && y >= 0 && y < w)
            {
<<<<<<< HEAD
                _pixels[i*w+j] = pixels[x*w+y];
            }
            else
            {
                _pixels[i*w+j] = SDL_MapRGB(new_format, 0, 0, 0);
=======
                _pixels[i * w + j] = pixels[y * w + x];
            }
            else
            {
                _pixels[i * w + j] = SDL_MapRGB(new_format, 0, 0, 0);
>>>>>>> 45ae5b0 (salut)
            }
        }
    }
    *surface = *new_img;
    SDL_UnlockSurface(surface);
    free(new_img);
}

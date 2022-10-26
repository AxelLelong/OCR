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

void resize(SDL_Surface* surface, unsigned int new_w, unsigned int new_h)
{
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    unsigned int w = surface->w;
    unsigned int h = surface->h;
    double x_scale = new_w / (double)w;
    double y_scale = new_h / (double)h;
    SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_SWSURFACE, new_w, new_h, 32, 0, 0, 0, 0);
    Uint32* _pixels = new_img->pixels;
    if (_pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_PixelFormat* new_format = new_img->format;
    if (new_format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);
    for (int i = 0; i < new_h; i++)
    {
        for (int j = 0; j < new_w; j++)
        {
            double old_x = i / x_scale;
            double old_y = j / y_scale;

            unsigned int top = floor(old_y);
            unsigned int bottom = top + 1;
            unsigned int left = floor(old_x);
            unsigned int right = left + 1;

            if (top < h && left < w && bottom < h
                && right < w)
            {
                Uint32 pixel = interpolation(
                        top, bottom, left, right, old_x, old_y, pixels,new_format,w);
                _pixels[i*w+j] = pixel;
            }
        }
    }
    *surface = *new_img;
    SDL_UnlockSurface(surface);
    free(new_img);
}

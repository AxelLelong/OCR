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
    /*Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    unsigned int w = surface->w;
    unsigned int h = surface->h;
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);

    double midX = ((double)w / 2.0);
    double midY = ((double)h / 2.0);

    double angle = degree * M_PI / 180.0;

    Uint32* _pixels = malloc(w*h*sizeof(Uint32));
    if (_pixels == NULL)
        errx(EXIT_FAILURE, "Ca bug dans rotate");
    for (unsigned int i = 0; i < w*h; ++i)
    {
        _pixels[i] = pixels[i];
    }
    for (unsigned int i = 0; i < w*h; ++i)
    {
        pixels[i] = SDL_MapRGB(format,0,0,0);
    }

    double newX;
    double newY;
    // Four pixels around
    unsigned int top;
    unsigned int bottom;
    unsigned int left;
    unsigned int right;

    for (unsigned int i = 0; i < w*h; ++i)
    {
        // Calculate new position with matrix rotation
        newX = ((double)(cos(angle) * ((double)(i/w) - midX)
                         - sin(angle) * ((double)(i%w) - midY))
                + midX);
        newY = ((double)(cos(angle) * ((double)(i%w) - midY)
                         + sin(angle) * ((double)(i/w) - midX))
                + midY);

        //Calculate the new four pixels around
        top = floor(newY);
        bottom = top + 1;
        left = floor(newX);
        right = left + 1;

        if (top < h && bottom < h && left < w
            && right < w)
        {
            pixels[i] = interpolation(top,bottom,left,right,newX,newY,_pixels,format,h);
        }
    }
    free(_pixels);
    SDL_UnlockSurface(surface);*/
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    int w = surface->w;
    int h = surface->h;
    SDL_PixelFormat* format = surface->format;
    if (format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);

    double midX = ((double)w / 2.0);
    double midY = ((double)h / 2.0);

    double angle = degree * M_PI / 180.0;

    Uint32* _pixels = malloc(w*h*sizeof(Uint32));
    if (_pixels == NULL)
        errx(EXIT_FAILURE, "Ca bug dans rotate");
    for (int i = 0; i < w*h; ++i)
    {
        _pixels[i] = pixels[i];
    }
    for (int x = 0; x < h; x++)
    {
        for (int y = 0; y < w; y++)
        {
            double newX, newY;
            // Calculate new position with matrix rotation
            newX = ((double)(cos(angle) * ((double)x - midX)
                             - sin(angle) * ((double)y - midY))
                    + midX);
            newY = ((double)(cos(angle) * ((double)x - midY)
                             + sin(angle) * ((double)y - midX))
                    + midY);

            if (0 <= newX && newX < h && 0 <= newY && newY < w)
            {
                Uint8 r, g, b;
                SDL_GetRGB(_pixels[(int)newX*w+(int)newY], format, &r, &g, &b);
                pixels[x*w+y] = SDL_MapRGB(format, r, g, b);
            }
            else
            {
                pixels[x*w+y] = SDL_MapRGB(format, 0, 0, 0);
            }
        }
    }
    free(_pixels);
    SDL_UnlockSurface(surface);
}

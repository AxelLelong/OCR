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
    int new_w;
    int new_h;
    double angle = degree * M_PI / 180.0;
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
    SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_HWSURFACE, new_w, new_h, 32, 0, 0, 0, 0);
    Uint32* _pixels = new_img->pixels;
    if (_pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_PixelFormat* new_format = new_img->format;
    if (new_format == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(surface);
    int mid_w = (new_w / 2.0);
    int mid_h = (new_h / 2.0);
    for (int i = 0; i < new_h; i++)
    {
        for (int j = 0; j < new_w; j++)
        {
            int x = (i - mid_h) * cos(angle) + (j - mid_w) * sin(angle);
            int y = (j - mid_w) * cos(angle) + (i - mid_h) * sin(angle);
            if (x >= 0 && x < h && y >= 0 && y < w)
            {
                _pixels[j][i] = pixels[y][x];
            }
            else
            {
                _pixels[j][i] = SDL_MapRGB(new_format, 0, 0, 0);
            }
        }
    }
    *surface = *new_img;
    SDL_UnlockSurface(surface);
    SDL_FreeSurface(new_img);


    /*Uint32* _pixels = malloc(w*h*sizeof(Uint32));
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
            int top,bottom,left,right;
            // Calculate new position with matrix rotation
            newX = ((double)(cos(angle) * ((double)x - midX) - sin(angle) * ((double)y - midY)));
            newY = ((double)(cos(angle) * ((double)x - midY) + sin(angle) * ((double)y - midX)));

            //Calculate the new four pixels around
            top = floor(newX);
            bottom = top + 1;
            left = floor(newY);
            right = left + 1;

            //if (0 <= newX && newX < h && 0 <= newY && newY < w)
            if (0 <= top && top < h && 0 <= bottom && bottom < h && 0 <= left && left < w && 0 <= right && right < w)
            {
                //Uint8 r, g, b;
                //SDL_GetRGB(_pixels[(int)newX*w+(int)newY], format, &r, &g, &b);
                //pixels[x*w+y] = SDL_MapRGB(format, r, g, b);
                pixels[x*w+y] = interpolation(top,bottom,left,right,newX,newY,_pixels,format,w);
            }
            else
            {
                pixels[x*w+y] = SDL_MapRGB(format, 0, 0, 0);
            }
        }
    }
    free(_pixels);*/
}

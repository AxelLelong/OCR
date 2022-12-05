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

    SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);

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

                _pixels[i*w+j] = pixels[x*w+y];
            }
            else
            {
                _pixels[i*w+j] = SDL_MapRGB(new_format, 0,0,0);
            }
        }
    }
    *surface = *new_img;
    SDL_UnlockSurface(surface);
    free(new_img);
}

void rotateAll(SDL_Surface* surface, int** lines, double angleDegree,int len)
{
    rotate(image, 360 - angleDegree);

    double angle = (360 - angleDegree) * M_PI / 180.0;
    angle -= 5.095;
    const double middleX = ((double)image->width / 2.0);
    const double middleY = ((double)image->height / 2.0);

    double newX;
    double newY;

    for (int i = 0;i<len;i++)
    {
        newX = ((double)(cos(angle) * ((double)lines[i][0] - middleX)
                         - sin(angle) * ((double)lines[i][1] - middleY))
                + middleX);

        newY = ((double)(cos(angle) * ((double)lines[i][1] - middleY)
                         + sin(angle) * ((double)lines[i][0] - middleX))
                + middleY);

        lines[i][0] = (int)newX;
        lines[i][1] = (int)newY;

        // Calculate new position end
        newX = ((double)(cos(angle) * ((double)lines[i][2] - middleX)
                         - sin(angle) * ((double)lines[i][3] - middleY))
                + middleX);

        newY = ((double)(cos(angle) * ((double)lines[i][3] - middleY)
                         + sin(angle) * ((double)lines[i][2] - middleX))
                + middleY);

        lines[i][2] = (int)newX;
        lines[i][3] = (int)newY;
    }
}

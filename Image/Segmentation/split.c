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
#include "../pre_processing/filters.h"

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
void clearLine(Uint32* pixels, SDL_PixelFormat* format, int i)
{
    for (int j = 0; j < 28; ++j)
    {
        pixels[i*28+j] = SDL_MapRGB(format, 255, 255, 255);
    }
}

void clearCol(Uint32* pixels, SDL_PixelFormat* format, int i)
{
    for (int j = 0; j < 28; ++j)
    {
        pixels[j*28+i] = SDL_MapRGB(format, 255, 255, 255);
    }
}

void clearNoise(Uint32* pixels, SDL_PixelFormat* format, int i)
{
    pixels[i] = SDL_MapRGB(format, 255, 255, 255);
    for (int j = -1; j < 2; ++j) {
        for (int k = -1; k < 2; ++k) {
            Uint8 r, g, b;
            /// - if we are not on an edge or a corner
            if (!((i % 28 == 0 && k == -1) || (i % 28 == 27 && k == 1) || (i < 28 && j == -1) ||
                  (i >= 28 * 27 && j == 1))) {

                SDL_GetRGB(pixels[i + j * 28 + k], format, &r, &g, &b);
                if (r == 0)
                    clearNoise(pixels, format, i + j * 28 + k);
            }
        }
    }

}

int detectNoise(Uint32* pixels, SDL_PixelFormat* format, int i, int* tab)
{
    int n = 0;
    tab[i] = 1;
    for (int j = -1; j < 2; ++j)
    {
        for (int k = -1; k < 2; ++k)
        {
            Uint8 r, g, b;
            /// - if we are not on an edge or a corner
            if (!((i % 28 == 0 && k == -1) || (i % 28 == 27 && k == 1) || (i < 28 && j == -1) ||
            (i >= 28 * 27 && j == 1)))
            {

                SDL_GetRGB(pixels[i + j * 28 + k], format, &r, &g, &b);
                if (r==0 && tab[i + j * 28 + k]==0)
                    n += detectNoise(pixels,format,i + j * 28 + k,tab);
            }
        }
    }
    return n + 1;

}

void clearNumber(Uint32* pixels, SDL_PixelFormat* format)
{
    Uint8 r, g, b;

    for (int i = 0; i < 28*28; ++i)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        if ((r+g+b)/3>127)
            pixels[i] = SDL_MapRGB(format, 255, 255, 255);
        else
            pixels[i] = SDL_MapRGB(format, 0, 0, 0);
    }
    /// - DETECTION LINES & COLUMNS
    int histLines[28] = {0};
    int histCols[28] = {0};
    for (int i = 0; i < 28; ++i)
    {
        for (int j = 0; j < 28; ++j)
        {
            SDL_GetRGB(pixels[i*28+j], format, &r, &g, &b);
            if (r==0)
            {
                histLines[i] += 1;
                histCols[j] += 1;
            }
        }
    }

    /// - DELETE LINES & COLUMNS
    for (int i = 0; i < 28; ++i)
    {
        if (histLines[i]>20)
            clearLine(pixels,format,i);
        if (histCols[i]>20)
            clearCol(pixels,format,i);
    }

    /// - DETECTION NOISE
    int max = -1;
    int maxIndex = -1;
    int nbToSuppr = 0;
    int* toSuppr = NULL;
    int* tab = calloc(28*28,sizeof(int));
    for (int i = 0; i < 28*28; ++i)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        if (tab[i]==0 && r==0)
        {
            int n = detectNoise(pixels, format, i, tab);

            if (n>max)
            {
                if (max!=-1)
                {
                    nbToSuppr++;
                    toSuppr = realloc(toSuppr, nbToSuppr* sizeof(int));
                    toSuppr[nbToSuppr - 1] = maxIndex;
                }
                max= n;
                maxIndex = i;
            }
            else
            {
                nbToSuppr++;
                toSuppr = realloc(toSuppr, nbToSuppr* sizeof(int));
                toSuppr[nbToSuppr-1] = i;
            }
        }
    }

    /// - DELETE NOISE
    for (int i = 0; i < nbToSuppr; ++i)
    {
        clearNoise(pixels,format,toSuppr[i]);
    }
    if (max<15)
    clearNoise(pixels,format,maxIndex);
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
                block.x = y;
                block.y = x;
                block.w = xincrem;
                block.h = yincrem;

                SDL_Surface* new_img = cropImage(surface, &block);

                // new_img free in resize
                SDL_Surface* imageresized = resize(new_img, 28, 28);


                //------------Clear image_resized----------------
                Uint32* pixels = imageresized->pixels;
                if (pixels == NULL)
                    errx(EXIT_FAILURE, "%s", SDL_GetError());


                clearNumber(pixels,imageresized->format);
		/*
                //---------------lissage chiffre-----------------
                int len = imageresized->w*imageresized->h;
                Uint32* pixels1 = malloc(len*sizeof(Uint32));
                if (pixels1 == NULL)
                    errx(EXIT_FAILURE, "C'est de la faute de pixels1 pendant le malloc");
                lissage(pixels,pixels1,imageresized->w,imageresized->h,imageresized->format);
                for (int i = 0; i < len ; ++i)
                {
                    pixels[i] = negativefilter(pixels1[i], imageresized->format);
                }
                free(pixels1);
                //------------------------------------------------
		*/
                segmentation[iall] = imageresized;
                //savesquare(imageresized,iall);
            }
            else
            {
                iall--;
            }
        }
    }
}

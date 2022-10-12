//
// Created by mathieu on 12/10/2022.
//

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "filters.h"
#include "../Dispay/display.h"
#include "transformation.h"

void transformation(SDL_Surface* surface)
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
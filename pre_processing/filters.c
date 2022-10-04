#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "filters.h"






// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

Uint32 negativefilter(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint32 color = SDL_MapRGB(format, 255-r, 255-g, 255-b);
    return color;
}

//Increase contrast
Uint32 contrastefilter(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    int fact = 10;
    for (int i = 0; i<10;i++)
    {
        if (i * 25 <= r && r <= (i + 1) * 25)
            return SDL_MapRGB(format, 255 - (i + 1) * 25, 255 - (i + 1) * 25, 255 - (i + 1) * 25);
    }
    return SDL_MapRGB(format, 255-r, 255-g, 255-b);
}

Uint8 get_max(Uint32* pixels, int len)
{
    Uint8 max = 0;
    for (int i = 0; i < len; ++i)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        if (r>max)
            max = r;
    }
    return max;
}

Uint32 NormLight(Uint32 pixel_color, SDL_PixelFormat* format, Uint8 m)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint32 color = SDL_MapRGB(format, 255-r*(255/m), 255-g*(255/m), 255-b*(255/m));
    return color;
}

Uint8 take_median(Uint8* neigh)
{
    for(int i=1 ; i < 9; i++)
    {
        int j = i;

        while (j > 0 && neigh[j - 1] > neigh[j]) {
            Uint8 tmp = neigh[j];
            neigh[j] = neigh[j - 1];
            neigh[j - 1] = tmp;

            j--;
        }
    }
    return neigh[4];
}

Uint32 medianfilter(Uint32* pixels,int i,SDL_PixelFormat* format,int w, int h)
{
    Uint8* neigh[9];
    int inDaList = 0;
    for (int j = -1; j < 2; ++j)
    {
        for (int k = -1; k < 2; ++k)
        {
            Uint8 r, g, b;
            if ((i%w==0&&k==-1)||(i%w==w-1&&k==1)||(i<w&&j==-1)||(i>w*(h-1)&&j==1))
                neigh[inDaList] = 0;
            else
            {
                SDL_GetRGB(pixels[i+j*w+k], format, &r, &g, &b);
                neigh[inDaList] = r;
            }
            inDaList++;
        }
    }
    return take_median(neigh);
}

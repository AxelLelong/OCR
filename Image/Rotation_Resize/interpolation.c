//
// Created by mathieu on 12/10/2022.
//

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "interpolation.h"


Uint32 interpolation(unsigned int top,unsigned int bottom,unsigned int left,unsigned int right,
                     double horizontal_position,double vertical_position,
                     Uint32* pixels,SDL_PixelFormat* format,int w)
{
    // Determine the values of the corners.
    Uint8 r, g, b;
    SDL_GetRGB(pixels[top*w+left], format, &r, &g, &b);
    double top_left = r;
    Uint8 r1, g1, b1;
    SDL_GetRGB(pixels[top*w+right], format, &r1, &g1, &b1);
    double top_right = r1;
    Uint8 r2, g2, b2;
    SDL_GetRGB(pixels[bottom*w+left], format, &r2, &g2, &b2);
    double bottom_left = r2;
    Uint8 r3, g3, b3;
    SDL_GetRGB(pixels[bottom*w+right], format, &r3, &g3, &b3);
    double bottom_right = r3;

    // Figure out "how far" the output pixel being considered is
    // between *_left and *_right.
    double horizontal_progress = horizontal_position - (double)left;
    double vertical_progress = vertical_position - (double)top;

    // Combine top_left and top_right into one large, horizontal
    // block.
    double top_block = top_left + horizontal_progress * (top_right - top_left);

    // Combine bottom_left and bottom_right into one large, horizontal
    // block.
    double bottom_block = bottom_left + horizontal_progress * (bottom_right - bottom_left);

    // Combine the top_block and bottom_block using vertical
    // interpolation and return as the resulting pixel.
    double pix = top_block + vertical_progress * (bottom_block - top_block);

    return SDL_MapRGB(format, (Uint8)pix, (Uint8)pix, (Uint8)pix);
}
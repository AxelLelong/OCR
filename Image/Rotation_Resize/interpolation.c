//
// Created by mathieu on 12/10/2022.
//

#inclune "interpolation.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>


Uint32 interpolation(Uint32 top, Uint32 bottom,Uint32 left, Uint32 right,
                     double horizontal_position,double vertical_position,
                     Uint32* pixels,SDL_PixelFormat* format)
{
    // Determine the values of the corners.
    Uint8 r, g, b;
    SDL_GetRGB(pixels[left*w+top], format, &r, &g, &b);
    double top_left = r;
    SDL_GetRGB(pixels[right*w+top], format, &r, &g, &b);
    double top_right = r;
    SDL_GetRGB(pixels[left*w+bottom], format, &r, &g, &b);
    double bottom_left = r;
    SDL_GetRGB(pixels[right*w+bottom], format, &r, &g, &b);
    double bottom_right = r;

    // Figure out "how far" the output pixel being considered is
    // between *_left and *_right.
    double horizontal_progress = newX - (double)left;
    double vertical_progress = newY - (double)top;

    // Combine top_left and top_right into one large, horizontal
    // block.
    double top_block = top_left + horizontal_progress * (top_right - top_left);

    // Combine bottom_left and bottom_right into one large, horizontal
    // block.
    double bottom_block =
            bottom_left + horizontal_progress * (bottom_right - bottom_left);

    // Combine the top_block and bottom_block using vertical
    // interpolation and return as the resulting pixel.
    double pix = top_block + vertical_progress * (bottom_block - top_block);

    return SDL_MapRGB(format, (Uint8)pix, (Uint8)pix, (Uint8)pix);
}
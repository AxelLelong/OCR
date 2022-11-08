//
// Created by Jules on 25/10/2022.
//
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>

#ifndef OCR_LIGN_DETECTION_H
#define OCR_LIGN_DETECTION_H

int** houghtransform(Uint32* pixels,int w, int h, SDL_PixelFormat* format,int draw,double *max_Theta,int* lenliste);
void draw_line(Uint32* pixels, int w, int h, int xStart,int yStart,int xEnd,int yEnd, Uint32 color,
               int thickness, int draw,SDL_PixelFormat* format);

#endif //OCR_LIGN_DETECTION_H

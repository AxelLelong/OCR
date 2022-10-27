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

int* HoughTransform(Uint32* pixels,int seuil, int w, int h,SDL_PixelFormat* format, int* pics);

#endif //OCR_LIGN_DETECTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#ifndef SOBEL_H
#define SOBEL_H

double Convolution(SDL_Surface *image, double kernel[3][3], int row, int col);
void SobelEdgeDetection(SDL_Surface *image);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "NeuralNetwork.h"

#define numInputs (28*28)
#define numHiddenNodes 81
#define numOutputs 10


SDL_Surface* GetImages(char *path)
{

  SDL_Surface * source = IMG_Load(path);

  SDL_Surface* surface = SDL_ConvertSurfaceFormat
            (source, SDL_PIXELFORMAT_RGB888, 0);

  SDL_FreeSurface(source);
  return surface;
}

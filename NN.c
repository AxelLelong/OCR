#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "NeuralNetwork/main.h"
#include "NeuralNetwork/GetImages.h"

int main ()
{
    mainNN(1,1,0,0,0,NULL,NULL);

  /*SDL_Surface* images_list[1];

  SDL_Surface* image = GetImages("NeuralNetwork/Train/008.png");

  images_list[0] = image;

  int* res = mainNN(0,0,0,1,1,images_list);

  printf("%i",res[0]);*/

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "NeuralNetwork.h"

#define numInputs (28*28)
#define numHiddenNodes 81
#define numOutputs 1


int Guess(SDL_Surface *Input)
{
  Uint8 r,g,b;

  double inputs[numInputs];
  
  Uint32* pixels = Input->pixels;
  if (pixels == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  
  for(int j = 0; j < 28*28; j++)
    {
      SDL_GetRGB(pixels[j],Input->format,&r,&g,&b);
      inputs[j] = (g == 255 ? 0 : 1);
    }

  
  //const double lr = 0.1f;
  FILE *f;

  //definition of tables
  double* hiddenLayer = calloc(numHiddenNodes,sizeof(double));
  double* outputLayer = calloc(numOutputs,sizeof(double));

  double* hiddenLayerBias = calloc(numHiddenNodes,sizeof(double));
  double* outputLayerBias = calloc(numOutputs,sizeof(double));

  double* hiddenWeights = calloc(numInputs * numHiddenNodes,sizeof(double));
  double* outputWeights = calloc(numHiddenNodes * numOutputs,sizeof(double));


  //load weights
  f = fopen("NeuralNetwork/Weights/WH","r");
  char c;
  char reset_num[10];
  char num[10];
  size_t i = 0;
  size_t j = 0;
  while((c = fgetc(f)) != EOF)
  {
      if (c == '\n')
      {
          hiddenWeights[i] = strtod(num,NULL);
          memcpy(num, reset_num, sizeof(char));
          i++;
          j=0;
      }
      else
      {
          num[j] = c;
          j++;
      }
  }
  fclose(f);
  memcpy(num, reset_num, sizeof(char));

  f = fopen("NeuralNetwork/Weights/BH","r");
  i = 0;
  j = 0;
  while((c = fgetc(f)) != EOF)
  {
      if (c == '\n')
      {
          hiddenLayerBias[i] = strtod(num,NULL);
          memcpy(num, reset_num, sizeof(char));
          i++;
          j=0;
      }
      else
      {
          num[j] = c;
          j++;
      }
      fclose(f);
      memcpy(num, reset_num, sizeof(char));

      f = fopen("NeuralNetwork/Weights/WO","r");
      i = 0;
      j = 0;
      while((c = fgetc(f)) != EOF)
      {
          if (c == '\n')
          {
              outputWeights[i] = strtod(num,NULL);
              memcpy(num, reset_num, sizeof(char));
              i++;
              j=0;
          }
          else
          {
              num[j] = c;
              j++;
          }
      }
      fclose(f);
      memcpy(num, reset_num, sizeof(char));

      f = fopen("NeuralNetwork/Weights/BO","r");
      i = 0;
      j = 0;
      while((c = fgetc(f)) != EOF)
      {
          if (c == '\n')
          {
              outputLayerBias[i] = strtod(num,NULL);
              memcpy(num, reset_num, sizeof(char));
              i++;
              j=0;
          }
          else
          {
              num[j] = c;
              j++;
          }
      }
      fclose(f);
  }


  printf("after load\n");

  // Guess the number //
  //ForwardPass
  // Compute hidden layer activation

  Compute_Hidden(numInputs, numHiddenNodes, hiddenLayer, hiddenLayerBias, inputs, hiddenWeights, 0);
    
  //Compute output layer activation
  Compute_Output(numHiddenNodes, numOutputs, outputLayerBias, outputLayer, hiddenLayer, outputWeights);


  double res = outputLayer[0];

  free(hiddenLayer);
  free(outputLayer);
  free(hiddenLayerBias);
  free(outputLayerBias);
  free(hiddenWeights);
  free(outputWeights);

  return res;
}


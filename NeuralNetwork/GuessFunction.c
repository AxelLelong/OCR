#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NeuralNetwork.h"

#define numInputs (28*28)
#define numHiddenNodes 81
#define numOutputs 10


int Guess (double *Input)
{
  //const double lr = 0.1f;
  FILE *f;

  //definition of tables
  double* hiddenLayer = malloc(numHiddenNodes*sizeof(double));
  double* outputLayer = malloc(numOutputs*sizeof(double));

  double* hiddenLayerBias = malloc(numHiddenNodes*sizeof(double));
  double* outputLayerBias = malloc(numOutputs*sizeof(double));

  double* hiddenWeights = malloc(numInputs * numHiddenNodes*sizeof(double));
  double* outputWeights = malloc(numHiddenNodes * numOutputs*sizeof(double));

  //load weights
  f = fopen("Weights/WH","r");
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

  f = fopen("Weights/BH","r");
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

      f = fopen("Weights/WO","r");
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

      f = fopen("Weights/BO","r");
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


  // Guess the number //

  //ForwardPass
  // Compute hidden layer activation
  Compute_Hidden(numInputs, numHiddenNodes, hiddenLayer, hiddenLayerBias, Input, hiddenWeights, i);
  //Compute output layer activation
  Compute_Output(numHiddenNodes, numOutputs, outputLayerBias, outputLayer, hiddenLayer, outputWeights);

  free(hiddenLayer);
  free(outputLayer);
  free(hiddenLayerBias);
  free(outputLayerBias);
  free(hiddenWeights);
  free(outputWeights);

  return outputLayer[0];
}


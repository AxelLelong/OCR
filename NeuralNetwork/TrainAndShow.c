#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Maths.h"
#include "NeuralNetwork.h"
#include "TrainAndShow.h"
#include "GetImages.h"

#define numInputs (28*28)
#define numHiddenNodes 81
#define numOutputs 10
#define numTrainingSets 10


void TrainAndShow (int train, int verbose, int show, int load, char* set)
{
  set = set;
  verbose = 0;
  time_t t;
  srand((unsigned) time(&t));
  //const double lr = 0.1f;
  FILE *f;

  //definition of tables
  double* hiddenLayer = malloc(numHiddenNodes*sizeof(double));
  double* outputLayer = malloc(numOutputs*sizeof(double));

  double* hiddenLayerBias = malloc(numHiddenNodes*sizeof(double));
  double* outputLayerBias = malloc(numOutputs*sizeof(double));

  double* hiddenWeights = malloc(numInputs * numHiddenNodes*sizeof(double));
  double* outputWeights = malloc(numHiddenNodes * numOutputs*sizeof(double));

  double training_inputs[numTrainingSets * numInputs];

  char path[28];

  Uint8 r,g,b;

  for(int i = 0; i < 10; i++)
    {
      int d = rand()%1;
      int u = rand()%7;
      printf("%i%i%i.png for %i\n",d,u,i,i);
      sprintf(path,"NeuralNetwork/Train/%i%i%i.png",d,u,i);

        SDL_Surface *image = GetImages(path);

        Uint32 *pixels = image->pixels;

        for(int j = 0; j < 28*28; j++)
        {
            SDL_GetRGB(pixels[j],image->format,&r,&g,&b);
            training_inputs[i*numInputs + j] = (g == 255 ? 0 : 1);
        }

    }

  double training_outputs[numTrainingSets * numOutputs] =
    {1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
     0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
     0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
     0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
     0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
     0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,
     0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,
     0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
     0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,
     0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,
    };

  //load weights
  if(load == 1)
  {
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
  else
  {
    //random setup
    setup_Weight(numInputs, numHiddenNodes, hiddenWeights);
    setup_Weight(numHiddenNodes, numOutputs, outputWeights);
    setup_Output_Bias(numOutputs, outputLayerBias);
  }
  int TrainingSetOrder[] = {0,1,2,3,4,5,6,7,8,9};

  int numberOfEpochs = 2000;

  if (show)
      numberOfEpochs = 1;

  // Train the neural network
  for(int epoch = 0; epoch < numberOfEpochs; epoch++)
  {
    if(verbose)
      printf("------------EPOCH %i---------------\n",epoch);
    if(train)
      {
          //shuffle the order of examples
          shuffle(TrainingSetOrder, numTrainingSets);
      }
      for(int x = 0; x < numTrainingSets; x++)
      {
          //choose the example
          int i = TrainingSetOrder[x];

          //ForwardPass
          // Compute hidden layer activation
          Compute_Hidden(numInputs, numHiddenNodes, hiddenLayer, hiddenLayerBias, training_inputs, hiddenWeights, i);
          //Compute output layer activation
          Compute_Output(numHiddenNodes, numOutputs, outputLayerBias, outputLayer, hiddenLayer, outputWeights);

          if(epoch == numberOfEpochs-1)
          {
              printf("Number : %d\n",i);

              for(int ab = 0; ab < 10; ab++)
              {
                  printf("Wanted Output : %g | Predicted Output : %g\n",
                         training_outputs[i*10+ab],outputLayer[ab]);
              }
          }


          //Backpropagation
          if(train)
          {
              //compute change in our output weights

              double deltaOutput[numOutputs];

              for(int j = 0; j < numOutputs; j++)
              {
                  double error = (training_outputs[i * numOutputs + j] - outputLayer[j]);
                  //printf("Error computed for %i == %f\n",j,error * der_sigmoid(outputLayer[j]));
                  deltaOutput[j] = error * der_sigmoid(outputLayer[j]);
              }

              //compute change in out hidden weights

              double deltaHidden[numHiddenNodes];

              for(int j = 0; j < numHiddenNodes; j++)
              {
                  double error = 0.0f;
                  for(int k = 0; k < numOutputs; k++)
                  {
                      error += deltaOutput[k] * outputWeights[j * numOutputs + k];
                  }
                  deltaHidden[j] = error * der_sigmoid(hiddenLayer[j]);
              }
              //Apply changes in output Weights
              apply_output(numHiddenNodes, numOutputs, deltaOutput, hiddenLayer, outputLayerBias, outputWeights);

              //Apply changes in Hidden Weights
              apply_weights(numInputs, numHiddenNodes, deltaHidden, training_inputs, hiddenLayerBias, hiddenWeights, i);
          }
      }
  }
  //save weights

  f = fopen("NeuralNetwork/Weights/WH", "w");
  for(int i = 0; i < numInputs * numHiddenNodes; i++)
  {
      fprintf(f, "%f\n", hiddenWeights[i]);
  }
  fclose(f);

  f = fopen("NeuralNetwork/Weights/BH", "w");
  for(size_t i = 0; i < numHiddenNodes; i++)
      fprintf(f, "%f\n", hiddenLayerBias[i]);
  fclose(f);

  f = fopen("NeuralNetwork/Weights/WO", "w");
  for(size_t i = 0; i < numHiddenNodes * numOutputs; i++)
      fprintf(f, "%f\n", outputWeights[i]);
  fclose(f);

  f = fopen("NeuralNetwork/Weights/BO", "w");
  for(size_t i = 0; i < numOutputs; i++)
      fprintf(f, "%f\n", outputLayerBias[i]);
  fclose(f);

  free(hiddenLayer);
  free(outputLayer);
  free(hiddenLayerBias);
  free(outputLayerBias);
  free(hiddenWeights);
  free(outputWeights);
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include "Maths.h"
#include "NeuralNetwork.h"
#include "TrainAndShow.h"

//nodes + num of examples
#define numInputs (28*28)
#define numHiddenNodes 50 
#define numOutputs 10
#define numTrainingSets 4


int main (int argc, char** argv)
{
  if(argc < 2)
  {
      printf("-train : train the Neural Network \n");
      printf("-verbose : activate verbose \n");
      errx(0, "\n");
  }

  int train = 0;
  int verbose = 0;
  int show = 0;
  int load = 0;

  for(int i = 0; i < argc; i++)
  {
      if(strcmp(argv[i],"-train") == 0)
          train=1;
      if(strcmp(argv[i],"-verbose") == 0)
          verbose = 1;
      if(strcmp(argv[i],"-show") == 0)
      {
          if (train)
              errx(0, "can't train and show result");
          else
              show = 1;
      }
      if(strcmp(argv[i],"-load") == 0)
          load = 1;
  }

  if(!train && !show)
      errx(0,"must train or show, please choose an option");

  if (train || show)
      TrainAndShow(train, verbose, show, load);


  return 1;
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include "Maths.h"
#include "NeuralNetwork.h"
#include "TrainAndShow.h"
#include "GuessFunction.h"

//nodes + num of examples
#define numInputs (28*28)
#define numHiddenNodes 81
#define numOutputs 10
#define numTrainingSets 2

//Prototype of the final function for the Neural Network (get images, guess and write in a text doc)
void mainNN (int train, int verbose, int show, int load, int all)
{
    char* sets [2] = {"00","01"};
    if (show)
    {
        if (train)
            errx(0, "NeuralNetwork: can't train and show result");
        else
            show = 1;
    }
    else if(!train && !show)
        errx(0,"NeuralNetwork: must train or show, please choose an option");
    else if (train || show)
        for(int i = 0; i < numTrainingSets; i++)
        {
            TrainAndShow(train, verbose, show, load, sets[i]);
            printf("set %i done\n",i);
        }
    //condition if we want to do all the way
    else if (all)
    {
        double **images_list = malloc(sizeof(double)*numHiddenNodes);
        int *sudoNumList = malloc(sizeof(int)*numHiddenNodes);
        //TODO____Function to get the images list of arrays
        for (size_t i = 0; i < numHiddenNodes; i++)
        {
            sudoNumList[i] = Guess(images_list[i]);
        }
        //TODO____Function to write in the text doc
    }
}


/*int main (int argc, char** argv)
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
}*/


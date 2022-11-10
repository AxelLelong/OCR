#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include <string.h>
#include "Maths.h"
#include "NeuralNetwork.h"

//nodes + num of exemples
#define numInputs 2
#define numHiddenNodes 2 
#define numOutputs 1
#define numTrainingSets 4


int main (int argc, char** argv)
{
  time_t t;
  srand((unsigned) time(&t));
  //const double lr = 0.1f;

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
  FILE *f;

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

  //definition of tables
  double* hiddenLayer = malloc(numHiddenNodes*sizeof(double));
  double* outputLayer = malloc(numOutputs*sizeof(double));

  double* hiddenLayerBias = malloc(numHiddenNodes*sizeof(double));
  double* outputLayerBias = malloc(numOutputs*sizeof(double));
  
  double* hiddenWeights = malloc(numInputs * numHiddenNodes*sizeof(double));
  double* outputWeights = malloc(numHiddenNodes * numOutputs*sizeof(double));


  double training_inputs[numTrainingSets * numInputs] =
    {0.0f, 0.0f,
     0.0f, 1.0f,
     1.0f, 0.0f,
     1.0f, 1.0f
    };

  double training_outputs[numTrainingSets * numOutputs] =
    {0.0f,
     1.0f,
     1.0f,
     0.0f
    };
  
  //load weights
  if(load)
    {
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
  else{
    //random setup
    setup_Weight(numInputs, numHiddenNodes, hiddenWeights);
    setup_Weight(numHiddenNodes, numOutputs, outputWeights);
    setup_Output_Bias(numOutputs, outputLayerBias);
  }
  int TrainingSetOrder[] = {0,1,2,3};
  
  int numberOfEpochs = 10000;

  if (show)
    numberOfEpochs = 1;

  // Train the neural network
  for(int epoch = 0; epoch < numberOfEpochs; epoch++)
    {

      if(train)
	{
	  //shuffle the order of exemples
	  shuffle(TrainingSetOrder, numTrainingSets);
	}
      for(int x = 0; x < numTrainingSets; x++){

	//choose the exemple
	int i = TrainingSetOrder[x];

	//FowardPass
	//Compute hidden layer activation
	Compute_Hidden(numInputs, numHiddenNodes, hiddenLayer, hiddenLayerBias, training_inputs, hiddenWeights, i);
	//Compute output layer activation
	Compute_Output(numHiddenNodes, numOutputs, outputLayerBias, outputLayer, hiddenLayer, outputWeights);
	if(verbose || show){
	  printf("Input : %g %g | Output : %g | Predicted Output : %g \n",
		 training_inputs[i * numInputs + 0], training_inputs[i * numInputs + 1],
		 outputLayer[0], training_outputs[i * numOutputs + 0]);
	}
	
	
	//Backpropagation
	if(train)
	  {
	    //compite change in our output weights
	
	    double deltaOutput[numOutputs];
	
	    for(int j = 0; j < numOutputs; j++){
	      double error = (training_outputs[i * numOutputs + j] - outputLayer[j]);
	      deltaOutput[j] = error * der_sigmoid(outputLayer[j]);
	    }
	
	    //compute change in out hidden wiegths
      
	    double deltaHidden[numHiddenNodes];
	    
	    for(int j = 0; j < numHiddenNodes; j++){
	      double error = 0.0f;
	      for(int k = 0; k < numOutputs; k++){
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
	
  f = fopen("Weights/WH", "w");
  for(size_t i = 0; i < numInputs * numHiddenNodes; i++)
    fprintf(f, "%f\n", hiddenWeights[i]);
  fclose(f);

  f = fopen("Weights/BH", "w");
  for(size_t i = 0; i < numHiddenNodes; i++)
    fprintf(f, "%f\n", hiddenLayerBias[i]);
  fclose(f);

  f = fopen("Weights/WO", "w");
  for(size_t i = 0; i < numHiddenNodes * numOutputs; i++)
    fprintf(f, "%f\n", outputWeights[i]);
  fclose(f);

  f = fopen("Weights/BO", "w");
  for(size_t i = 0; i < numOutputs; i++)
    fprintf(f, "%f\n", outputLayerBias[i]);
  fclose(f);

  //print Final Weights after done training

  fputs ("Final Hidden Weights \n", stdout);
  for(int j = 0; j < numHiddenNodes; j++){
    fputs ("[ ", stdout);
    for(int k = 0; k < numInputs; k++){
      printf("%f ", hiddenWeights[k * numHiddenNodes + j]);
    }
    fputs(" ]\n", stdout);
  }

  fputs ("Final Hidden Bias \n", stdout);
  for(int j = 0; j < numHiddenNodes; j++){
    fputs ("[ ", stdout);
    printf("%f ", hiddenLayerBias[j]);
    fputs(" ]\n", stdout);
  }

  fputs ("Final Outputs Weights \n", stdout);
  for(int j = 0; j < numOutputs; j++){
    fputs ("[ ", stdout);
    for(int k = 0; k < numHiddenNodes; k++){
      printf("%f ", outputWeights[k * numOutputs + j]);
    }
    fputs(" ]\n", stdout);
  }
  
  fputs ("Final Outputs Bias \n", stdout);
  for(int j = 0; j < numOutputs; j++){
    fputs ("[ ", stdout);
    printf("%f ", outputLayerBias[j]);
    fputs(" ]\n", stdout);
  }

  free(hiddenLayer);
  free(outputLayer);
  free(hiddenLayerBias);
  free(outputLayerBias);
  free(hiddenWeights);
  free(outputWeights);


  return 1;
}


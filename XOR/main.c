#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
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

  if(argc != 2)
    errx(0, "-train : train the Neural Network \n");

  int train = 0;

  for(int i = 0; i < argc; i++)
    if (argv[i] == "-train")
      train = 1;

  //definition of tables
  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];

  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];

  double hiddenWeights[numInputs * numHiddenNodes];
  double outputWeights[numHiddenNodes * numOutputs];


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
  
  //random setup
  setup_Weight(numInputs, numHiddenNodes, hiddenWeights);
  setup_Weight(numHiddenNodes, numOutputs, outputWeights);
  setup_Output_Bias(numOutputs, outputLayerBias);

  int TrainingSetOrder[] = {0,1,2,3};
  int numberOfEpochs = 10000;

  // Train the neural network
  for(int epoch = 0; epoch < numberOfEpochs; epoch++)
    {
    //shuffle the order of exemples
    shuffle(TrainingSetOrder, numTrainingSets);

    for(int x = 0; x < numTrainingSets; x++){

      //choose the exemple
      int i = TrainingSetOrder[x];

      //FowardPass
      //Compute hidden layer activation
      Compute_Hidden(numInputs, numHiddenNodes, hiddenLayer, hiddenLayerBias, training_inputs, hiddenWeights, i);
       //Compute output layer activation
      Compute_Output(numHiddenNodes, numOutputs, outputLayerBias, outputLayer, hiddenLayer, outputWeights);

      printf("Input : %g %g | Output : %g | Predicted Output : %g \n",
	     training_inputs[i * numInputs + 0], training_inputs[i * numInputs + 1],
	     outputLayer[0], training_outputs[i * numOutputs + 0]);


      //Backpropagation

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

      //print Final Weights after done training
      fputs ("Final Hidden Weights \n", stdout);
      for(int j = 0; j < numHiddenNodes; j++){
	fputs ("[ ", stdout);
	for(int k = 0; k < numInputs; k++){
	  printf("%f ", hiddenWeights[k * numHiddenNodes + j]);
	}
	fputs(" ]", stdout);
      }
    }
  }
  return 1;
}


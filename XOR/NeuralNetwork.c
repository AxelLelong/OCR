#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Maths.h"

//nodes + num of exemples
#define numInputs 2
#define numHiddenNodes 2
#define numOutputs 1
#define numTrainingSets 4

//init weights
double init_weights() {return ((double)rand()) / ((double)RAND_MAX); }

//shuffle an array
void shuffle(int *array, size_t n) {

  if (n > 1){

    size_t i;
    for(i = 0; i < n - 1; i++){
      //randomly select a data
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      //shulffle part
      array[j] = array[i];
      array[i] = t;
      }
  }
}

int main()
{
  //time_t t;
  //srand((unsigned) time(&t));
  const double lr = 0.1f;

  //definition of tables
  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];

  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];

  double hiddenWeights[numInputs][numHiddenNodes];
  double outputWeights[numHiddenNodes][numOutputs];


  double training_inputs[numTrainingSets][numInputs] =
    {{0.0f, 0.0f},
     {0.0f, 1.0f},
     {1.0f, 0.0f},
     {1.0f, 1.0f}
    };

  double training_outputs[numTrainingSets][numOutputs] =
    {{0.0f},
     {1.0f},
     {1.0f},
     {0.0f}
    };
  
  //random setup
  for(int i = 0; i < numInputs; i++){
    for (int j = 0; j < numHiddenNodes; j++){
      hiddenWeights[i][j] = init_weights();
    }
  }
  
  for(int i = 0; i < numHiddenNodes; i++){
    for (int j = 0; j < numOutputs; j++){
      outputWeights[i][j] = init_weights();
    }
  }

  for (int j = 0; j < numOutputs; j++){
    outputLayerBias[j] = init_weights();
  }
  int TrainingSetOrder[] = {0,1,2,3};
  int numberOfEpochs = 100000;

  // Train the neural network
  for(int epoch = 0; epoch < numberOfEpochs; epoch++){
    //shuffle the order of exemples
    shuffle(TrainingSetOrder, numTrainingSets);

    for(int x = 0; x < numTrainingSets; x++){

      //choose the exemple
      int i = TrainingSetOrder[x];

      //FowardPass

      //Compute hidden layer activation
      for(int j = 0; j < numHiddenNodes; j++){
	//bias already in the computation, we only have to add the weights, sigmoid etc..
	double activation = hiddenLayerBias[j];

	for(int k = 0; k < numInputs; k++){
	  activation += training_inputs[i][k] * hiddenWeights[k][j];
	}

	//update
	hiddenLayer[j] = sigmoid(activation);
      }

       //Compute output layer activation
      for(int j = 0; j < numOutputs; j++){
	//bias already in the computation, we only have to add the weights, sigmoid etc..
	double activation = outputLayerBias[j];

	for(int k = 0; k < numHiddenNodes; k++){
	  activation += hiddenLayer[k] * outputWeights[k][j];  
	}

	//update
	outputLayer[j] = sigmoid(activation);
      }

      printf("Input : %g %g | Output : %g | Predicted Output : %g \n",
	     training_inputs[i][0], training_inputs[i][1],
	     outputLayer[0], training_outputs[i][0]);


      //Backpropagation

      //compite change in our output weights

      double deltaOutput[numOutputs];

      for(int j = 0; j < numOutputs; j++){
	double error = (training_outputs[i][j] - outputLayer[j]);
	deltaOutput[j] = error * der_sigmoid(outputLayer[j]);
      }

      //compute change in out hidden wiegths
      
      double deltaHidden[numHiddenNodes];
      
      for(int j = 0; j < numHiddenNodes; j++){
	double error = 0.0f;
	for(int k = 0; k < numOutputs; k++){
	  error += deltaOutput[k] * outputWeights[j][k];
	}
	deltaHidden[j] = error * der_sigmoid(hiddenLayer[j]);
      }

      //Apply changes in output Weights

      for(int j = 0; j < numOutputs; j++){
	outputLayerBias[j] += deltaOutput[j] * lr;
	for(int k = 0; k < numHiddenNodes; k++){
	  outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
	}
      }

      //Apply changes in Hidden Weights

      for(int j = 0; j < numHiddenNodes; j++){
	hiddenLayerBias[j] += deltaHidden[j] * lr;
	for(int k = 0; k < numInputs; k++){
	  hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr;
	}
      }

      //print Final Weights after done training
      fputs ("Final Hidden Weights \n", stdout);
      for(int j = 0; j < numHiddenNodes; j++){
	fputs ("[ ", stdout);
	for(int k = 0; k < numInputs; k++){
	  printf("%f ", hiddenWeights[k][j]);
	}
	fputs(" ]", stdout);
      }

    } 
  }
}

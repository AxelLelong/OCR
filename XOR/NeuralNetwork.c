#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Maths.h"

//nodes + num of exemples
/*#define numInputs 2
#define numHiddenNodes 2
#define numOutputs 1
#define numTrainingSets 4*/
#define lr 0.1

//init weights randomly
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

//setup all weights and bias
void setup_Weight(int numFirstLayer,int numSecondLayer,double* WeightLayer)
{
  for(int i = 0; i < numFirstLayer; i++){
    for (int j = 0; j < numSecondLayer; j++){
      WeightLayer[i * numSecondLayer + j] = init_weights();
    }
  }
  
}

void setup_Output_Bias(int numOutputs, double* Layer)
{
  for (int j = 0; j < numOutputs; j++){
    Layer[j] = init_weights();
  }
}

void Compute_Hidden(int Inputs, int Hidden, double* Layer, double* Bias, double* t_inputs, double* Weights, int i)
{
   for(int j = 0; j < Hidden; j++){
	//bias already in the computation, we only have to add the weights, sigmoid etc..
	double activation = Bias[j];

	for(int k = 0; k < Inputs; k++){
	  activation += t_inputs[i * Inputs + k] * Weights[k * Hidden + j];
	}

	//update
	Layer[j] = sigmoid(activation);
      }
}

void Compute_Output(int Hidden, int Outputs, double* Bias, double* Layer, double* HiddenLayer, double* Weights)
{
  for(int j = 0; j < Outputs; j++){
    //bias already in the computation, we only have to add the weights, sigmoid etc..
    double activation = Bias[j];
    
    for(int k = 0; k < Hidden; k++){
      activation += HiddenLayer[k] * Weights[k * Outputs + j];  
    }

    //update
    Layer[j] = sigmoid(activation);
  }
}

void apply_weights(int Inputs, int Hidden, double* delta, double* training_inputs, double* Bias, double* Weights, int i)
{
  for(int j = 0; j < Hidden; j++){
    Bias[j] += delta[j] * lr;
    for(int k = 0; k < Inputs; k++){
      Weights[k * Hidden + j] += training_inputs[i * Inputs + k] * delta[j] * lr;
    }
  }
}

void apply_output(int Hidden, int Output, double* delta, double* H_Layer, double* Bias, double* Weights)
{
  for(int j = 0; j < Output; j++){
    Bias[j] += delta[j] * lr;
    for(int k = 0; k < Hidden; k++){
      Weights[k * Output + j] += H_Layer[k] * delta[j] * lr;
    }
  }
}
/*
int main()
{
  time_t t;
  srand((unsigned) time(&t));
  const double lr = 0.1f;

  //definition of tables
  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];

  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];


  double hiddenWeights[numInputs * numHiddenNodes];
  double outputWeights[numHiddenNodes * numOutputs];
  //double hiddenWeights[numInputs][numHiddenNodes];
  //double outputWeights[numHiddenNodes][numOutputs];

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
      hiddenWeights[i * numHiddenNodes + j] = init_weights();
    }
  }
  
  for(int i = 0; i < numHiddenNodes; i++){
    for (int j = 0; j < numOutputs; j++){
      outputWeights[i * numOutputs + j] = init_weights();
    }
  }

  for (int j = 0; j < numOutputs; j++){
    outputLayerBias[j] = init_weights();
  }
  int TrainingSetOrder[] = {0,1,2,3};
  int numberOfEpochs = 10000;

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
	  activation += training_inputs[i * numInputs + k] * hiddenWeights[k * numHiddenNodes + j];
	}

	//update
	hiddenLayer[j] = sigmoid(activation);
      }

       //Compute output layer activation
      for(int j = 0; j < numOutputs; j++){
	//bias already in the computation, we only have to add the weights, sigmoid etc..
	double activation = outputLayerBias[j];

	for(int k = 0; k < numHiddenNodes; k++){
	  activation += hiddenLayer[k] * outputWeights[k * numOutputs + j];  
	}

	//update
	outputLayer[j] = sigmoid(activation);
      }

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

      for(int j = 0; j < numOutputs; j++){
	outputLayerBias[j] += deltaOutput[j] * lr;
	for(int k = 0; k < numHiddenNodes; k++){
	  outputWeights[k * numOutputs + j] += hiddenLayer[k] * deltaOutput[j] * lr;
	}
      }

      //Apply changes in Hidden Weights

      for(int j = 0; j < numHiddenNodes; j++){
	hiddenLayerBias[j] += deltaHidden[j] * lr;
	for(int k = 0; k < numInputs; k++){
	  hiddenWeights[k * numHiddenNodes + j] += training_inputs[i * numInputs + k] * deltaHidden[j] * lr;
	}
      }

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
  }
*/

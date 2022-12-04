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
#define lr 0.01

//init weights randomly
double init_weights()
{
    return ((double)rand() / RAND_MAX)*2.0-1.0;// * 2.0 - 1.0;
}
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

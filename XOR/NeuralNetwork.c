#include "<stdio.h>"
#include "<stdlib.h>"
#include "<maths.h>"
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
  const double lr = 0.1f;

  //definition of tables
  double hiddenLayer[numHiddenNodes];
  double outputLayer[num Outputs];

  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];

  double hiddenWeights[numInputs][numHiddenNodes];
  double outputWeights[numHiddenNodes][numOutputs];


  double training_inputs[numTrainingSets][NumInputs] =
    {{0.0f, 0.0f},
     {0.0f, 1.0f},
     {1.0f, 0.0f},
     {1.0f, 1.0f}
    };

  double training_outputs[numTrainingSets][NumOutputs] =
    {{0.0f},
     {1.0f},
     {1.0f},
     {0.0f}
    };
  
  //random setup
  for(int i = 0; i < numInputs; i++){
    for (int j = 0; j < numHiddenNodes; j++){
      hiddenWeights[i][j] = init_wieghts();
    }
  }
  
  for(int i = 0; i < numHiddenNodes; i++){
    for (int j = 0; j < numOutputs; j++){
      outputWeights[i][j] = init_wieghts();
    }
  }

  for (int j = 0; j < numOutputs; j++){
    outputLayerBias[j] = init_wieghts();
  }
  int TrainingSetOrder[] = {0,1,2,3};
  int numberOfEpochs = 10000;

  // Train the neural network
  for(int epoch = 0; epoch < numberOfEpochs; epoch++){
    //shuffle the order of exemples
    shuffle(trainingSetorder, numTrainingSets);

    for(int x = 0; x < numTrainingSets; x++){

      //choose the exemple
      int i = trainingSetOrder[x];

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
	double activation = hiddenLayerBias[j];

	for(int k = 0; k < numInputs; k++){
	  activation += training_inputs[i][k] * hiddenWeights[k][j];
	  
	}

	//update
	hiddenLayer[j] = sigmoid(activation);
      }
    }

    
  }
}

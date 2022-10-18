#ifndef NEURAL_H
#define NEURAL_H

double init_weights();
void shuffle(int *array, size_t n);
void setup_Weight(int numFirstLayer,int numSecondLayer,double* WeightLayer);
void setup_Output_Bias(int numOutputs, double* Layer);
void Compute_Hidden(int Inputs, int Hidden, double* Layer, double* Bias, double* t_inputs, double* Weights, int i);
void Compute_Output(int Hidden, int Outputs, double* Bias, double* Layer, double* HiddenLayer, double* Weights);
void apply_weights(int Inputs, int Hidden, double* delta, double* training_inputs, double* Bias, double* Weights, int i);
void apply_output(int Hidden, int Output, double* delta, double* H_Layer, double* Bias, double* Weights);

#endif

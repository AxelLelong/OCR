#include <stdio.h>
#include <stdlib.h>
#include "Maths.h"

float NeuralNet(float Input[], float Output[])
{
    float lr = 0.2;

    float W1 = (float)rand()/RAND_MAX;
    float W2 = (float)rand()/RAND_MAX;
    float W3 = (float)rand()/RAND_MAX;
    float W4 = (float)rand()/RAND_MAX;
    float W5 = (float)rand()/RAND_MAX;
    float W6 = (float)rand()/RAND_MAX;

    float BW1 = (float)rand()/RAND_MAX;
    float BW2 = (float)rand()/RAND_MAX;
    float BW3 = (float)rand()/RAND_MAX;

    float N1 = sigmoid(Input[0] * W1 + Input[1] * W2 + BW1);
    float N2 = sigmoid(Input[0] * W3 + Input[1] * W4 + BW2);
    float N3 = sigmoid(N1 * W5 + N2 * W6 + BW3);

    for (int i = 0; i < 5000; i++)
    {
        float TargetCalculated = Output[0] - N3;

        float eW6 = TargetCalculated * der_sigmoid(N1 * W5 + N2 * W6 + BW3) * N2 * lr;
        float eW5 = TargetCalculated * der_sigmoid(N1 * W5 + N2 * W6 + BW3) * N1 * lr;
        float eW4 = TargetCalculated * W6 * der_sigmoid(Input[0] * W3 + Input[1] * W4 + BW2) * Input[1] * lr;
        float eW3 = TargetCalculated * W6 * der_sigmoid(Input[0] * W3 + Input[1] * W4 + BW1) * Input[0] * lr;
        float eW2 = TargetCalculated * W5 * der_sigmoid(Input[0] * W1 + Input[1] * W2 + BW2) * Input[1] * lr;
        float eW1 = TargetCalculated * W5 * der_sigmoid(Input[0] * W1 + Input[1] * W2 + BW1) * Input[0] * lr;

        float eBW3 = TargetCalculated * der_sigmoid(N1 * W5 + N2 * W6 + BW3) * lr;
        float eBW2 = TargetCalculated * W4 * der_sigmoid(Input[0] * W3 + Input[1] * W4 + BW2) * lr;
        float eBW1 = TargetCalculated * W3 *der_sigmoid(Input[0] * W1 + Input[1] * W2 + BW1) * lr;

        W6 += eW6;
        W5 += eW5;
        W4 += eW4;
        W3 += eW3;
        W2 += eW2;
        W1 += eW1;

        BW3 += eBW3;
        BW2 += eBW2;
        BW1 += eBW1;

        N1 = sigmoid(Input[0] * W1 + Input[1] * W2 + BW1);
        N2 = sigmoid(Input[0] * W3 + Input[1] * W4 + BW2);
        N3 = sigmoid(N1 * W5 + N2 * W6 + BW3);

        printf("Iteration number: %i\n", i+1);
        printf("Input: %f | Output: %f\n", Input, N3);
    }
    return N3;
}

int main() {
    float Input[][2] = {{0, 0},
                        {0, 1},
                        {1, 0},
                        {1, 1}};
    float Output[][1] = {{0},
                         {1},
                         {1},
                         {0}};
    NeuralNet(Input[1], Output[1]);
}

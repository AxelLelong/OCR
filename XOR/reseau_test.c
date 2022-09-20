#include <stdio.h>
#include <stdlib.h>
#include "Maths.h"

int main()
{
    float Input = 1;
    float Output = 0;

    float W1 = (float)rand()/RAND_MAX;
    float W2 = (float)rand()/RAND_MAX;
    float W3 = (float)rand()/RAND_MAX;
    float W4 = (float)rand()/RAND_MAX;

    float N1 = sigmoid(Input * W1);
    float N2 = sigmoid(Input * W2);
    float N3 = sigmoid(N1 * W3 + N2 * W4);

    for (int i = 0; i < 1000; i++)
    {
        float TargetCalculated = Output - N3;

        float eW4 = TargetCalculated * der_sigmoid(N1 * W3 + N2 * W4) * N2 * 0.1;
        float eW3 = TargetCalculated * der_sigmoid(N1 * W3 + N2 * W4) * N1 * 0.1;
        float eW2 = TargetCalculated * W4 * der_sigmoid(Input * W2) * Input * 0.1;
        float eW1 = TargetCalculated * W3 * der_sigmoid(Input * W1) * Input * 0.1;

        W4 += eW4;
        W3 += eW3;
        W2 += eW2;
        W1 += eW1;

        N1 = sigmoid(Input * W1);
        N2 = sigmoid(N1 * W2);
        N3 = sigmoid(N1 * W3 + N2 * W4);

        printf("iteration number: %i\n", i+1);
        printf("Input: %f | Output: %f\n", Input, N3);
    }

    return 1;
}

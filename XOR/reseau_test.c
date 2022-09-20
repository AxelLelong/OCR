#include <stdio.h>
#include <stdlib.h>
#include "Maths.h"

int main()
{
    float Input = 1;
    float Output = 0;

    float W1 = (float)rand()/(float)RAND_MAX;
    float W2 = (float)rand()/(float)RAND_MAX;

    float N1 = sigmoid(Input * W1);
    float N2 = sigmoid(N1 * W2);

    for (int i = 0; i < 5; i++)
    {
        float TargetCalculated = Output - N2;

        float eW2 = TargetCalculated * der_sigmoid(N1 *W2) * N1;
        float eW1 = TargetCalculated * W2 * der_sigmoid(Input * W1) * Input;

        W2 += eW2;
        W1 += eW1;

        N1 = sigmoid(Input * W1);
        N2 = sigmoid(N1 * W2);

        printf("iteration number: %i", i+1);
        printf("Input: %f | Output: %f", Input, N2);
    }

    return 1;
}
#include <stdio.h>
#include <stdlib.h>
#include "Maths.h"
#include <time.h>

float NeuralNet(float Input[], float Output[], long iteration)
{
    //random seed with time
    time_t t;
    srand((unsigned) time(&t));

    float lr = 0.01;

    float W1 = (float)rand()/RAND_MAX;
    float W2 = (float)rand()/RAND_MAX;
    float W3 = (float)rand()/RAND_MAX;
    float W4 = (float)rand()/RAND_MAX;
    float W5 = (float)rand()/RAND_MAX;
    float W6 = (float)rand()/RAND_MAX;
    float Lw[6] = {W1,W2,W3,W4,W5,W6};
    float BW1 = (float)rand()/RAND_MAX;
    float BW2 = (float)rand()/RAND_MAX;
    float BW3 = (float)rand()/RAND_MAX;
    float Lb[3] = {BW1,BW2,BW3};

    float N1 = sigmoid(Input[0] * W1 + Input[1] * W2 + BW1);
    float N2 = sigmoid(Input[0] * W3 + Input[1] * W4 + BW2);
    float N3 = sigmoid(N1 * W5 + N2 * W6 + BW3);

    for (int i = 0; i < iteration; i++)
    {
        printf("----------------------------------------------------------------------------------------------------\n");

        float ErrorComputed = 0.5*((float)(Output[0] - N3)*(float)(Output[0] - N3));

        printf("Error Calculated == %f \n", ErrorComputed);

        /*float eW6 = ErrorComputed * der_sigmoid(N1 * W5 + N2 * W6 + BW3) * N2 * lr;
        float eW5 = ErrorComputed * der_sigmoid(N1 * W5 + N2 * W6 + BW3) * N1 * lr;
        float eW4 = ErrorComputed * W6 * der_sigmoid(Input[0] * W3 + Input[1] * W4 + BW2) * Input[1] * lr;
        float eW3 = ErrorComputed * W6 * der_sigmoid(Input[0] * W3 + Input[1] * W4 + BW1) * Input[0] * lr;
        float eW2 = ErrorComputed * W5 * der_sigmoid(Input[0] * W1 + Input[1] * W2 + BW2) * Input[1] * lr;
        float eW1 = ErrorComputed * W5 * der_sigmoid(Input[0] * W1 + Input[1] * W2 + BW1) * Input[0] * lr;

        float eBW3 = ErrorComputed * der_sigmoid(N1 * W5 + N2 * W6 + BW3) * lr;
        float eBW2 = ErrorComputed * W4 * der_sigmoid(Input[0] * W3 + Input[1] * W4 + BW2) * lr;
        float eBW1 = ErrorComputed * W3 *der_sigmoid(Input[0] * W1 + Input[1] * W2 + BW1) * lr;*/

        float Dw6 = ErrorComputed * der_sigmoid(N1 * W5 + N2 * W6 + BW3) * N2;
        W6 -= Dw6 * lr;
        float eBW3 = ErrorComputed * der_sigmoid(N1 * W5 + N2 * W6 + BW3);
        BW3 -= eBW3 * lr;

        float Dw5 = ErrorComputed * der_sigmoid(N1 * W5 + N2 * W6 + BW3) * N1;
        W5 -= Dw5 * lr;

        float Dw4 = ErrorComputed * W6 * der_sigmoid(Input[0] * W3 + Input[1] * W4 + BW2) * Input[1];
        W4 -= Dw4 * lr;

        float Dw3 = ErrorComputed * W6 * der_sigmoid(Input[0] * W3 + Input[1] * W4 + BW1) * Input[0];
        W3 -= Dw3 * lr;
        float eBW2 = ErrorComputed * der_sigmoid(Input[0] * W3 + Input[1] * W4 + BW2);
        BW2 -= eBW2 * lr;


        float Dw2 = ErrorComputed * W5 * der_sigmoid(Input[0] * W1 + Input[1] * W2 + BW2) * Input[1];
        W2 -= Dw2 * lr;

        float Dw1 = ErrorComputed * W5 * der_sigmoid(Input[0] * W1 + Input[1] * W2 + BW1) * Input[0];
        W1 -= Dw1 * lr;
        float eBW1 = ErrorComputed * der_sigmoid(Input[0] * W1 + Input[1] * W2 + BW1);
        BW1 -= eBW1 * lr;

        /*W6 -= Dw6 * lr;
        W5 -= Dw5 * lr;
        W4 -= Dw4 * lr;
        W3 -= Dw3 * lr;
        W2 -= Dw2 * lr;
        W1 -= Dw1 * lr;

        BW3 -= eBW3 * lr;
        BW2 -= eBW2 * lr;
        BW1 -= eBW1 * lr;*/

        N1 = sigmoid(Input[0] * W1 + Input[1] * W2 + BW1);
        N2 = sigmoid(Input[0] * W3 + Input[1] * W4 + BW2);
        N3 = sigmoid(N1 * W5 + N2 * W6 + BW3);

        printf("Iteration number: %i\n", i+1);
        printf("Input: %f,%f | Output: %f | Expected: %f\n", Input[0],Input[1], N3, Output[0]);

        for (size_t j = 0; Lw[j] != '\0'; ++j) {
            Lw[0] = W1; Lw[1] = W2; Lw[2] = W3; Lw[3] = W4; Lw[4] = W5; Lw[5] = W6;
            printf("W%I64u == %f | ", j+1,Lw[j]);
        }
        printf("\n");
        for (size_t j = 0; j < 3; ++j) {
            Lb[0] = BW1; Lb[1] = BW2; Lb[2] = BW3;
            printf("B%I64u == %f | ", j+1,Lb[j]);
        }
        printf("\n");
    }
    return N3;
}

int main(int argc, char *argv[]) {
    long n = 100;

    if (argc != 2)
        n = 100;
    else
        n = (long)atoi(argv[1]);

    float Input[][2] = {{0, 0},
                        {0, 1},
                        {1, 0},
                        {1, 1}};
    float Output[][1] = {{0},
                         {1},
                         {1},
                         {0}};
    NeuralNet(Input[3], Output[3],n);
}

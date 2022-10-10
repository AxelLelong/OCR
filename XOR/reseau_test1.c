#include <stdio.h>
#include <stdlib.h>
#include "Maths.h"
#include <time.h>

float NeuralNet(float Inputs[][2], float Outputs[][1], long iteration)
{
    //random seed with time
    time_t t;
    srand((unsigned) time(&t));

    float alpha = 0.1;


    //IF POSITIVE GR, TETA ++, ELSE TETA--
    float Teta1 = 0.8;
    float Teta2 = -0.1;
    float Teta3 = 0.3;

    float gr1 = 0;
    float gr2 = 0;
    float gr3 = 0;


    float W1 = 0.5;//(float)rand()/RAND_MAX;
    float W2 = 0.9;//(float)rand()/RAND_MAX;
    float W3 = -0.4;//(float)rand()/RAND_MAX;
    float W4 = 1;//(float)rand()/RAND_MAX;
    float W5 = -1.2;//(float)rand()/RAND_MAX;
    float W6 = 1.1;//(float)rand()/RAND_MAX;
    float Lw[6] = {W1,W2,W3,W4,W5,W6};

    float N1, N2, N3 = 0;

    for (int i = 0; i < iteration; i++)
    {
        printf("----------------------------------------------------------------------------------------------------\n");
        for(int j = 0; j < 4; j++)
        {
            float *Input = Inputs[j];
            float *Output = Outputs[j];

            N1 = sigmoid(Input[0] * W1 + Input[1] * W2 - Teta1);
            N2 = sigmoid(Input[0] * W3 + Input[1] * W4 - Teta2);
            N3 = sigmoid(N1 * W5 + N2 * W6 - Teta3);

            float ErrorComputed = Output[0] - N3;

            printf("Error Calculated == %f \n", ErrorComputed);

            gr3 = der_sigmoid(N3) * ErrorComputed;
            float ew6 = alpha * N2 * gr3;
            float ew5 = alpha * N1 * gr3;
            float eTeta3 = alpha * (-1) * gr3;

            gr2 = der_sigmoid(N2) * ErrorComputed;
            float ew4 = alpha * Input[1] * gr2;
            float ew3 = alpha * Input[0] * gr2;
            float eTeta2 = alpha * (-1) * gr2;

            gr1 = der_sigmoid(N1) * ErrorComputed;
            float ew2 = alpha * Input[1] * gr1;
            float ew1 = alpha * Input[0] * gr1;
            float eTeta1 = alpha * (-1) * gr1;

            W6 += ew6;
            W5 += ew5;
            W4 += ew4;
            W3 += ew3;
            W2 += ew2;
            W1 += ew1;

            Teta1 = gr1 < 0 ? Teta1 + eTeta1 : Teta1 - eTeta1;
            Teta2 = gr2 < 0 ? Teta2 + eTeta2 : Teta2 - eTeta2;
            Teta3 = gr3 < 0 ? Teta3 + eTeta3 : Teta3 - eTeta3;

            printf("Iteration number: %i\n", i + 1);
            printf("Input: %f,%f | Output: %f | Expected: %f\n", Input[0], Input[1], N3, Output[0]);

            for (size_t k = 0; k < 6; k++) {
                Lw[0] = W1;
                Lw[1] = W2;
                Lw[2] = W3;
                Lw[3] = W4;
                Lw[4] = W5;
                Lw[5] = W6;
                printf("W%lu == %f | ", k + 1, Lw[k]);
            }
            printf("\n");
        }
    }

    printf("RESULTS --------------------------------------------------------------\n");
    for (int i = 0; i<4; i++)
    {
        N1 = sigmoid(Inputs[i][0] * W1 + Inputs[i][1] * W2 - Teta1);
        N2 = sigmoid(Inputs[i][0] * W3 + Inputs[i][1] * W4 - Teta2);
        N3 = sigmoid(N1 * W5 + N2 * W6 - Teta3);
        printf("Input: %f,%f | Output: %f | Expected: %f\n", Inputs[i][0],Inputs[i][1], N3, Outputs[i][0]);
    }
    printf("FINALS WEIGHTS AND BIAS ------------------------------------------------\n");
    for (size_t j = 0; Lw[j] != '\0'; ++j)
    {
        printf("W%lu == %f | ", j+1,Lw[j]);
    }
    printf("\n");

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
    NeuralNet(Input, Output,n);

    return 1;
}

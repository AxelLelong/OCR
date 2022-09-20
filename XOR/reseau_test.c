#include <stdio.h>

int main()
{
    int Input = 1;
    int Output = 0;

    float W1 = (float)rand()/(float)RAND_MAX();
    float W2 = (float)rand()/(float)RAND_MAX();

    float N1 = Input * W1;
    float N2 = N1 * W2;

    printf("Input: %i | Output: %f", Input, N2)

    return 1;
}
#include "Maths.h"
#include <math.h>

/// <summary>
/// The sigmoid function
/// </summary>
/// <param name="x"> : a double</param>
/// <returns>the sigmoid of x</returns>
double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

/// <summary>
/// The derivate of the sigmoid function
/// </summary>
/// <param name="x">is already a sigmoid</param>
/// <returns>the derivate of x</returns>
double der_sigmoid(double x)
{
    return x * (1 - x);
}

double RELU(double x)
{
    return fmax(0,x);
}

double der_RELU(double x)
{
    return x < 0 ? 0 : 1
}

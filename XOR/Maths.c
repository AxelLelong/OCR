#include "Maths.h"
#include <math.h>

/// <summary>
/// The sigmoid function
/// </summary>
/// <param name="x"> : a double</param>
/// <returns>the sigmoid of x</returns>
float sigmoid(float x)
{
	return 1.0 / (1.0 + expf(-x));
}

/// <summary>
/// The derivate of the sigmoid function
/// </summary>
/// <param name="x">is already a sigmoid</param>
/// <returns>the derivate of x</returns>
float der_sigmoid(float x)
{
	return x * (1 - x);
}
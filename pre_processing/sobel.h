#ifndef SOBEL.H
#define SOBEL.H

double Convolution(SDL_Surface *image, double kernel[3][3], int row, int col);
void SobelEdgeDetection(SDl_Surface *image);

#endif

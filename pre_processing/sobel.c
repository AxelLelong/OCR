#include "sobel.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

double Convolution(SDL_Surface *image, double kernel[3][3], int row, int col)
{
    double sum = 0;
    unsigned int r;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int x = i + row;
            int y = j + col;
	    //check if we're inside of the picture
            if (x >= 0 && y >= 0 && x < (int)image->width
                && y < (int)image->height)
            {
                r = image->pixels[x][y].r;
                sum += r * kernel[i][j];
            }
        }
    }

    return sum;
}

void SobelEdgeDetection(SDL_Surface *image)
{
    double gx, gy;
    double g_px;

    double kernel_x[3][3] = { { -1.0, 0.0, 1.0 },
                              { -2.0, 0.0, 2.0 },
                              { -1.0, 0.0, 1.0 } };

    double kernel_y[3][3] = { { -1.0, -2.0, -1.0 },
                              { 0.0, 0.0, 0.0 },
                              { 1.0, 2.0, 1.0 } };

    const unsigned int height = image->height;
    const unsigned int width = image->width;
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            gx = Convolution(image, kernel_x, j, i);
            gy = Convolution(image, kernel_y, j, i);
            g_px = sqrt(gx * gx + gy * gy);

	    //update pixel
	    &(image->pixels[j][i]) -> r = g_px > 128 ? 255 : 0;
	    &(image->pixels[j][i]) -> g = g_px > 128 ? 255 : 0;
	    &(image->pixels[j][i]) -> b = g_px > 128 ? 255 : 0;


        }
    }
}

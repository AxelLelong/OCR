#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "filters.h"






// Converts a colored pixel into grayscale.
Uint8 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    Uint8 color = average;
    return color;
}

Uint8 negativefilter(Uint8 pixel_color)
{
    return 255 - pixel_color;
}

/// - Increase contrast
Uint8 contrastefilter(Uint8 pixel_color)
{
    int fact = 10;

    /// - From zero to fact
    for (int i = 0; i<fact;i++)
    {
        /// - If we are in the wanted range
        if (i * 255/fact <= pixel_color && pixel_color <= (i + 1) * 255/fact)

            /// - We fix the value of the pixel at the upper bound of the range
            /// - And we invert colors for the next filter
            return 255 - (i + 1) * 255/fact;
    }
    /// - Or we just invert colors
    return 255-pixel_color;
}


/// - Return the maximum value of the image
Uint8 get_max(Uint8* pixels, int len)
{
    Uint8 max = 0;
    for (int i = 0; i < len; ++i)
    {
        Uint8 r = pixels[i];
        if (r>max)
            max = r;
    }
    return max;
}

void NormLight(Uint8* pixels, int len, Uint8 m)
{
    for (int i = 0; i < len; ++i)
    {
        pixels[i] = 255-pixels[i]*(255/m);
    }

}

Uint8 take_median(Uint8* neigh)
{
    /// - we sort the list by insertion sort
    for(int i=1 ; i < 9; i++)
    {
        int j = i;
        Uint8 v = neigh[i];

        while (j > 0 && neigh[j - 1] > v)
        {
            Uint8 tmp = neigh[j];
            neigh[j] = neigh[j - 1];
            neigh[j - 1] = tmp;

            j--;
        }
        neigh[j] = v;
    }
    /// - We return the median value
    return neigh[4];
}


void medianfilter(Uint8* pixels, Uint8* pixels1,int w, int h)
{
    /// - Init. the array for the pixel and his neighbors
    Uint8* neigh = malloc(9*sizeof(Uint8));
    if (neigh == NULL)
        errx(EXIT_FAILURE, "Ca bug dans medianFilter");

    /// - For each pixel in pixels
    for (int i = 0; i < w*h; ++i)
    {
        int inDaList = 0;

        /// - We take neighbors
        for (int j = -1; j < 2; ++j)
        {
            for (int k = -1; k < 2; ++k)
            {
                /// - if we are on a edge or a corner, some value are 0
                if ((i % w == 0 && k == -1) || (i % w == w - 1 && k == 1) || (i < w && j == -1) ||
                    (i >= w * (h - 1) && j == 1))
                    neigh[inDaList] = 0;
                else
                {
                    /// - We add the pixel in the array
                    neigh[inDaList] = pixels[i + j * w + k];
                }
                inDaList++;
            }
        }
        /// - We change the pixel in pixels1(another array of pixels, else neighbors are going to change)
        /// - We change the pixel by the median value of his neighbors
        pixels1[i] = take_median(neigh);
    }
    free(neigh);
}

Uint8 multiMat(Uint8* m1, Uint8* m2)
{
    /// - Return the sum of m1*m2
    Uint8 sum = 0;
    for (int i = 0 ; i < 9 ; i++)
    {
        sum += m2[i]/m1[i];
    }
    return sum;
}


void GaussianFlou(Uint8* pixels,Uint8* pixels1,int w, int h)
{
    Uint8* neigh = malloc(9*sizeof(Uint8));
    if (neigh == NULL)
        errx(EXIT_FAILURE, "PROBLEM in GaussianFlou");

    /// - Put the pixel and his neighbors in array
    for (int i = 0; i < w*h; ++i)
    {
        int inDaList = 0;
        for (int j = -1; j < 2; ++j)
        {
            for (int k = -1; k < 2; ++k)
            {
                if ((i%w==0&&k==-1)||(i%w==w-1&&k==1)||(i<w&&j==-1)||(i>=w*(h-1)&&j==1))
                    neigh[inDaList] = 0;
                else
                {
                    neigh[inDaList] = pixels[i+j*w+k];
                }
                inDaList++;
            }
        }
        /// - Init Gaussian Matrice
        Uint8 m1[9] = {16,8,16,8,4,8,16,8,16};
        pixels1[i] =  multiMat(m1,neigh);
    }
    free(neigh);
}

void adaptativeThreshold(Uint8* pixels,double seuil, int w ,int h)
{
    Uint32* integral_image = malloc(w*h*sizeof(Uint32));
    if (integral_image == NULL)
        errx(EXIT_FAILURE, "C'est de la faute de integral_image pendant le malloc");

    int s2 = fmax(w, h) / 16;
    long sum = 0;
    int count = 0;
    int x1, y1, x2, y2;

    for (int y = 0; y < w; y++)
    {
        sum += pixels[y];
        integral_image[y] = sum;
    }

    for (int i = 1; i < h; i++)
    {
        sum = 0;
        for (int j = 0; j < w; j++)
        {
            sum += pixels[i*w+j];
            integral_image[i * w + j] = integral_image[(i - 1) * w + j] + sum;
        }
    }
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            x1 = fmax(i - s2, 1);
            x2 = fmin(i + s2, h - 1);
            y1 = fmax(j - s2, 1);
            y2 = fmin(j + s2, w - 1);
            count = (x2 - x1) * (y2 - y1);
            sum = integral_image[x2 * w + y2]
                  - integral_image[x2 * w + (y1 - 1)]
                  - integral_image[(x1 - 1) * w + y2]
                  + integral_image[(x1 - 1) * w + (y1 - 1)];
            //printf("count = %d , sum = %lu \n",count,sum);
            if (pixels[i*w+j] * count < sum * (1.0 - seuil))
            {
                pixels[i*w+j] = 0;
            }
            else
            {
                pixels[i*w+j] = 255;
            }
        }
    }
    free(integral_image);
}

float noiseLevel(Uint8* pixels,int w, int h)
{
    float count = 0.0;
    double medium = 0.0;
    Uint8* neigh = malloc(9*sizeof(Uint8));
    if (neigh == NULL)
        errx(EXIT_FAILURE, "Ca bug dans medianFilter");

    for (int i = 0; i < w*h; ++i)
    {
        int inDaList = 0;
        for (int j = -1; j < 2; ++j)
        {
            for (int k = -1; k < 2; ++k)
            {
                if ((i % w == 0 && k == -1) || (i % w == w - 1 && k == 1) || (i < w && j == -1) ||
                    (i >= w * (h - 1) && j == 1))
                    neigh[inDaList] = 0;
                else
                {
                    neigh[inDaList] = pixels[i + j * w + k];
                }
                inDaList++;
            }
        }
        medium = 0.0;
        for (int k = 0; k < 9; k++)
        {
            medium += neigh[k];
        }
        medium /= 9;
        double val = 1 - (pixels[i] / medium);
        if (val < 0)
        {
            val *= -1;
        }
        if (val > 0.5)
        {
            count++;
        }
    }
    free(neigh);
    return count;
}

void lissage(Uint8* pixels,Uint8* pixels1,int w, int h)
{
    for (int i = 0; i < w*h ; ++i)
    {

        /// - If it's a white pixel
        int isNoWhite = !(pixels[i]==0);
        int j = -1;
        /// - If a neighbor is black
        while (isNoWhite && j<2)
        {
            int k = -1;
            while (isNoWhite && k<2)
            {
                /// - If we are on a corner or an edge we continue
	            if ((i%w==0&&k==-1)||(i%w==w-1&&k==1)||(i<w&&j==-1)||(i>=w*(h-1)&&j==1))
		          {
		              k++;
                      continue;
		          }
	            else
                {
                    /// - If we find a black pixel
                    if (pixels[i+j*w+k]!=255)
                        isNoWhite = 0;
                }
                k++;
            }
            j++;
        }
        /// - If a neighbors is black, the pixel becomes black
        if (isNoWhite)
            pixels1[i] = 0;
        else
            pixels1[i] = 255;
    }

    /// - FONCTION DE LISSAGE A TESTER SVP

    /*for (int i = 0; i < w*h ; ++i)
    {
        Uint8 rm, gm, bm;
        SDL_GetRGB(pixels[i], format, &rm, &gm, &bm);
        /// - If it's a white pixel
        if (rm != 0)
        {
            int j = -1;
            /// - If a neighbor is black
            while (j < 2)
            {
                int k = -1;
                while (k < 2)
                {
                    /// - If we are on a corner or an edge we continue
                    if ((i % w == 0 && k == -1) || (i % w == w - 1 && k == 1) || (i < w && j == -1) ||
                        (i >= w * (h - 1) && j == 1))
                    {
                        k++;
                        continue;
                    }
                    else
                    {
                        Uint8 r, g, b;
                        SDL_GetRGB(pixels[i + j * w + k], format, &r, &g, &b);
                        /// - If we find a black pixel
                        if (r != 255)
                        {
                            pixels1[i] = SDL_MapRGB(format, 0, 0, 0);
                            break;
                        }
                    }
                    k++;
                }
                if (k!=2)
                    break;
                j++;
            }
        }
    }*/
}

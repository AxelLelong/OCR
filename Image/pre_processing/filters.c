#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "filters.h"






// Converts a colored pixel into grayscale.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

Uint32 negativefilter(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint32 color = SDL_MapRGB(format, 255-r, 255-g, 255-b);
    return color;
}

/// - Increase contrast
Uint32 contrastefilter(Uint32 pixel_color, SDL_PixelFormat* format)
{
    int fact = 10;
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    /// - From zero to fact
    for (int i = 0; i<fact;i++)
    {
        /// - If we are in the wanted range
        if (i * 255/fact <= r && r <= (i + 1) * 255/fact)

            /// - We fix the value of the pixel at the upper bound of the range
            /// - And we invert colors for the next filter
            return SDL_MapRGB(format, 255 - (i + 1) * 255/fact, 255 - (i + 1) * 255/fact, 255 - (i + 1) * 255/fact);
    }
    /// - Or we just invert colors
    return SDL_MapRGB(format, 255-r, 255-g, 255-b);
}


/// - Return the maximum value of the image
Uint8 get_max(Uint32* pixels, int len,SDL_PixelFormat* format)
{
    Uint8 max = 0;
    for (int i = 0; i < len; ++i)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        if (r>max)
            max = r;
    }
    return max;
}

void NormLight(Uint32* pixels, SDL_PixelFormat* format, int len, Uint8 m)
{
    for (int i = 0; i < len; ++i)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        pixels[i] = SDL_MapRGB(format, 255-r*(255/m), 255-g*(255/m), 255-b*(255/m));
    }

}

Uint32 take_median(Uint8* neigh,SDL_PixelFormat* format)
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
    return SDL_MapRGB(format, neigh[4], neigh[4], neigh[4]);
}


void medianfilter(Uint32* pixels, Uint32* pixels1,SDL_PixelFormat* format,int w, int h)
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
                Uint8 r, g, b;
                /// - if we are on a edge or a corner, some value are 0
                if ((i % w == 0 && k == -1) || (i % w == w - 1 && k == 1) || (i < w && j == -1) ||
                    (i >= w * (h - 1) && j == 1))
                    neigh[inDaList] = 0;
                else
                {
                    /// - We add the pixel in the array
                    SDL_GetRGB(pixels[i + j * w + k], format, &r, &g, &b);
                    neigh[inDaList] = r;
                }
                inDaList++;
            }
        }
        /// - We change the pixel in pixels1(another array of pixels, else neighbors are going to change)
        /// - We change the pixel by the median value of his neighbors
        pixels1[i] = take_median(neigh, format);
    }
    free(neigh);
}

Uint32 multiMat(Uint8* m1, Uint8* m2,SDL_PixelFormat* format)
{
    /// - Return the sum of m1*m2
    Uint8 sum = 0;
    for (int i = 0 ; i < 9 ; i++)
    {
        sum += m2[i]/m1[i];
    }
    return SDL_MapRGB(format, sum, sum,sum);
}


void GaussianFlou(Uint32* pixels,Uint32* pixels1,SDL_PixelFormat* format,int w, int h)
{
    Uint8* neigh = malloc(9*sizeof(Uint8));
    if (neigh == NULL)
        errx(EXIT_FAILURE, "PROBLEM in GaussianFlou");

    for (int i = 0; i < w*h; ++i)
    {
        int inDaList = 0;
        for (int j = -1; j < 2; ++j)
        {
            for (int k = -1; k < 2; ++k)
            {
                Uint8 r, g, b;
                if ((i%w==0&&k==-1)||(i%w==w-1&&k==1)||(i<w&&j==-1)||(i>=w*(h-1)&&j==1))
                    neigh[inDaList] = 0;
                else
                {
                    SDL_GetRGB(pixels[i+j*w+k], format, &r, &g, &b);
                    neigh[inDaList] = r;
                }
                inDaList++;
            }
        }
        /// - Init Gaussian Matrice
        Uint8 m1[9] = {16,8,16,8,4,8,16,8,16};
        pixels1[i] =  multiMat(m1,neigh,format);
    }
    free(neigh);
}

void adaptativeThreshold(Uint32* pixels,double seuil, int w ,int h, SDL_PixelFormat* format)
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
        Uint8 r, g, b;
        SDL_GetRGB(pixels[y], format, &r, &g, &b);
        sum += r;
        integral_image[y] = sum;
    }

    for (int i = 1; i < h; i++)
    {
        sum = 0;
        for (int j = 0; j < w; j++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i*w+j], format, &r, &g, &b);
            sum += r;
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
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i*w+j], format, &r, &g, &b);
            if (r * count < sum * (1.0 - seuil))
            {
                pixels[i*w+j] = SDL_MapRGB(format, 0, 0, 0);
            }
            else
            {
                pixels[i*w+j] = SDL_MapRGB(format, 255, 255, 255);
            }
        }
    }
    free(integral_image);
}

float noiseLevel(Uint32* pixels,int w, int h, SDL_PixelFormat* format)
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
                Uint8 r, g, b;
                if ((i % w == 0 && k == -1) || (i % w == w - 1 && k == 1) || (i < w && j == -1) ||
                    (i >= w * (h - 1) && j == 1))
                    neigh[inDaList] = 0;
                else
                {
                    SDL_GetRGB(pixels[i + j * w + k], format, &r, &g, &b);
                    neigh[inDaList] = r;
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
        Uint8 rd, gr, bl;
        SDL_GetRGB(pixels[i], format, &rd, &gr, &bl);
        double val = 1 - (rd / medium);
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

void lissage(Uint32* pixels,Uint32* pixels1,int w, int h,SDL_PixelFormat* format)
{
  //invert colors
  /*for(int i = 0; i < h*w; i++)
    negativefilter(pixels[i], format);
  */
    //New picture
    for (int i = 0; i < w*h ; ++i)
    {

        //To verify if it isn't a white in neighbors
        Uint8 rm, gm, bm;
        SDL_GetRGB(pixels[i], format, &rm, &gm, &bm);
        /// - If it's a white pixel
        int isNoWhite = !(rm==0);
        int j = -1;
        while (isNoWhite && j<2)
        {
            int k = -1;
            while (isNoWhite && k<2)
            {
                /// - If we are on a corner or an edge
	            if ((i%w==0&&k==-1)||(i%w==w-1&&k==1)||(i<w&&j==-1)||(i>=w*(h-1)&&j==1))
		          {
		              k++;
                      continue;
		          }
	            else
                {
                    Uint8 r, g, b;
                    SDL_GetRGB(pixels[i+j*w+k], format, &r, &g, &b);
                    /// - If we find a black pixel
                    if (r!=255)
                        isNoWhite = 0;
                }
                k++;
            }
            j++;
        }
        /// - If a neighbors is black, the pixel becomes black
        if (isNoWhite)
            pixels1[i] = SDL_MapRGB(format, 0, 0, 0);
        else
            pixels1[i] = SDL_MapRGB(format, 255, 255, 255);
    }
}

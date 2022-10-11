#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "filters.h"






// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
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

//Increase contrast
Uint32 contrastefilter(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    for (int i = 0; i<10;i++)
    {
        if (i * 25 <= r && r <= (i + 1) * 25)
            return SDL_MapRGB(format, 255 - (i + 1) * 25, 255 - (i + 1) * 25, 255 - (i + 1) * 25);
    }
    return SDL_MapRGB(format, 255-r, 255-g, 255-b);
}

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
    for(int i=1 ; i < 9; i++)
    {
        int j = i;
	Uint8 v = neigh[i];

        while (j > 0 && neigh[j - 1] > v) {
            Uint8 tmp = neigh[j];
            neigh[j] = neigh[j - 1];
            neigh[j - 1] = tmp;

            j--;
        }
	neigh[j] = v;
    }
    return SDL_MapRGB(format, neigh[4], neigh[4], neigh[4]);
}


void medianfilter(Uint32* pixels, Uint32* pixels1,SDL_PixelFormat* format,int w, int h)
{
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
        pixels1[i] = take_median(neigh, format);
    }
    free(neigh);
}

Uint32 multiMat(Uint8* m1, Uint8* m2,SDL_PixelFormat* format)
{
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
        errx(EXIT_FAILURE, "Ca bug dans medianFilter");

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
        Uint8 m1[9] = {16,8,16,8,4,8,16,8,16};
        pixels1[i] =  multiMat(m1,neigh,format);
    }
    free(neigh);
}

void adaptativeThreshold(Uint32* pixels,double seuil, int w ,int h, SDL_PixelFormat* format)
{
    Uint32* integral_image = malloc(w*h*sizeof(Uint32));
    if (integral_image == NULL)
        errx(EXIT_FAILURE, "C'est de la faute de integral_image pendant le mallocul");

    int s2 = fmax(w, h) / 16;
    long sum = 0;
    int count = 0;
    int x1, y1, x2, y2;

    for (int y = 0; y < h; y++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[y], format, &r, &g, &b);
        sum += r;
        integral_image[y] = sum;
    }

    for (int i = 1; i < w; i++)
    {
        sum = 0;
        for (int j = 0; j < h; j++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i*w+j], format, &r, &g, &b);
            sum += r;
            integral_image[i * h + j] = integral_image[(i - 1) * h + j] + sum;
        }
    }
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            x1 = fmax(i - s2, 1);
            x2 = fmin(i + s2, w - 1);
            y1 = fmax(j - s2, 1);
            y2 = fmin(j + s2, h - 1);
            count = (x2 - x1) * (y2 - y1);
            sum = integral_image[x2 * h + y2]
                  - integral_image[x2 * h + (y1 - 1)]
                  - integral_image[(x1 - 1) * h + y2]
                  + integral_image[(x1 - 1) * h + (y1 - 1)];
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i*w+j], format, &r, &g, &b);
            if (r * count < sum * (1.0 - seuil))
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

/*int* lissage(int* pixels,int w, int h)
{
    //New picture
    int newPix[w*h];
    for (int i = 0; i < w*h ; ++i)
    {

        //To verify if it isn't a white in neighbors
        int isNoWhite = !pixels[i];
        int j = -1;
        while (isNoWhite && j<2)
        {
            int k = -1;
            while (isNoWhite && k<2)
            {
                if ((i%w==0&&k==-1)||(i%w==w-1&&k==1)||(i<w&&j==-1)||(i>=w*(h-1)&&j==1))
                    continue;
                else
                {
                    //if we find a white pixel
                    if (pixels[i+j*w+k]!=0)
                        isNoWhite = 0;
                }
                k++;
            }
            j++;
        }
        newPix[i] = !isNoWhite;
    }

    return newPix;
}*/

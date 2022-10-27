//
// Created by Jules on 25/10/2022.
//
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "lign_detection.h"

int* HoughTransform(Uint32* pixels,int seuil, int w, int h,SDL_PixelFormat* format,int* pics)
{
    int diag = sqrt(w*w+h*h);

    /// - Initialise  an array of size 2*diag*θ
    int *M = calloc(181*2*(diag+1),sizeof(int));

    /// - Parcours de l'image
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i*w+j], format, &r, &g, &b);
            /// - Si c'est un pixel blanc
            if (0!=r)
            {
                /// - Pour chaque valeurs de θ (entre 0 et 180)
                for (int theta = 0; theta < 181; ++theta)
                {

                    /// - Ajoute 1 a M[p][θ] avec p = x * cos(θ) + y * sin(θ)
                    int p = j * cos(theta) + i * sin(theta);
		    if(i*w+j == 3011124)
		      printf("p == %i\n | theta == %i\n",p,theta);
                    M[p + diag + 2 * diag * theta] += 1;
                }
            }
        }
    }

    int i = 0;
    /// - Parcours de l'accumulateur
    for (int theta = 0; theta < 180; ++theta)
    {
        for (int p = -diag; p < diag; ++p)
        {
            /// - Si c'est au dessus du seuil
            if (M[p+diag+theta*diag]>seuil)
            {
                /// - Calcul des coordonées de la ligne
                int x0 = p*cos(theta);
                int y0 = p*sin(theta);
                int x1 = x0 + (diag*cos(theta));
                int y1 = y0 + (diag*sin(theta));
                int x2 = x0 - (diag*cos(theta));
                int y2 = y0 - (diag*sin(theta));

                /// - Stockage de chaque ligne
                pics[i + 0] = x1;
                pics[i + 1] = y1;
                pics[i + 2] = x2;
                pics[i + 3] = y2;
                i+=4;
            }
        }
    }
    return pics;
}


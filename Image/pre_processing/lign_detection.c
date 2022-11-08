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
int **initMatriceInt(int x, int y)
{
    int **matrice = NULL;
    matrice = calloc(y + 1, sizeof(int *));
    if (matrice == NULL)
    {
        errx(1, "Memory error");
    }
    for (int j = 0; j < y; j++)
    {
        matrice[j] = calloc(x + 1, sizeof(int));
        if (matrice[j] == NULL)
        {
            errx(1, "Memory error");
        }
    }
    return matrice;
}

void freeMatriceInt(int **matrice, int height)
{
    for (int y = 0; y < height; y++)
    {
        free(matrice[y]);
    }
    free(matrice);
}

int** houghtransform(Uint32* pixels, Image *drawImage, int w, int h, SDL_PixelFormat* format,int draw,double *max_Theta)
{
    // Calculate the diagonal of the image
    const double diagonal = sqrt(w * w + h * h);

    // Initialize the constant values for theta and rho
    const double maxTheta = 180.0, minTheta = 0.0;
    const double maxRho = diagonal, minRho = -diagonal;
    const double nbRho = 2 * diagonal, nbTheta = nbRho;

    // Create an array of rhos
    double rhoStep = (maxRho - minRho) / nbRho;
    double *arrRhos = calloc(nbRho + 1, sizeof(double));
    int index = 0;
    for (double val = minRho; val <= maxRho && index < nbTheta;
         val += rhoStep, index++)
    {
        arrRhos[index] = val;
    }

    // Create an array of theta
    double step = (maxTheta - minTheta) / nbTheta;
    double *arrThetas = calloc(nbTheta + 1, sizeof(double));
    index = 0;
    for (double val = minTheta; val <= maxTheta && index < nbTheta;
         val += step, index++)
    {
        arrThetas[index] = val;
    }
    // Create a save of cos and sin value for each theta, to optimize
    // performance.
    double *saveCos = calloc(nbTheta + 1, sizeof(double));
    double *saveSin = calloc(nbTheta + 1, sizeof(double));
    for (int theta = 0; theta < nbTheta; theta++)
    {
        // Convert each value of theta's array into radians
        arrThetas[theta] = arrThetas[theta] * M_PI / 180.0;

        // Save each cos(theta) and sin(theta) into their respective arrays
        saveCos[theta] = cos(arrThetas[theta]);
        saveSin[theta] = sin(arrThetas[theta]);
    }
    int **accumulator = initMatriceInt(nbTheta + 1, nbRho + 1);

    // We intialize the accumulator with all the value
    // In the same time, we search for the max value in the accumulator

    unsigned int max = 0;
    double rho;
    int croppedRho;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[y*w+x], format, &r, &g, &b);
            if (r == 255)
            {
                for (int theta = 0; theta <= nbTheta; theta++)
                {
                    rho = x * saveCos[theta] + y * saveSin[theta];
                    croppedRho = rho + diagonal;
                    accumulator[croppedRho][theta]++;
                    if (accumulator[croppedRho][theta] > max)
                    {
                        max = accumulator[croppedRho][theta];
                    }
                }
            }
        }
    }

    free(saveCos);
    free(saveSin);

    // Finding edges
    // Computing threshold
    int lineThreshold = max * 0.4;

    // Create line return line array
    int ** allLines = initMatriceInt(nbTheta + 1, nbRho + 1);

    double tempMaxTheta = 0.0;
    unsigned int histogram[181] = { 0 };
    unsigned int rounded_angle;

    int prev = accumulator[0][0];
    int prev_theta = 0, prev_rho = 0;
    int boolIsIncreasing = 1;

    Uint32 pixel = SDL_MapRGB(format, 40, 40, 200);

    for (int theta = 0; theta <= nbTheta; theta++)
    {
        for (int rho = 0; rho <= nbRho; rho++)
        {
            int val = accumulator[rho][theta];

            if (val >= prev)
            {
                prev = val;
                prev_rho = rho;
                prev_theta = theta;
                boolIsIncreasing = 1;
                continue;
            }
            else if (val < prev && boolIsIncreasing)
            {
                boolIsIncreasing = 0;
            }
            else if (val < prev)
            {
                prev = val;
                prev_rho = rho;
                prev_theta = theta;
                continue;
            }

            if (val >= lineThreshold)
            {
                double r = arrRhos[prev_rho], t = arrThetas[prev_theta];

                if (t > tempMaxTheta)
                {
                    tempMaxTheta = t;
                    rounded_angle = (unsigned int)(t * M_PI / 180.0);
                    histogram[rounded_angle]++;
                }

                double c = cos(t), s = sin(t);
                // Calculate d0 point
                int x = (int)(c * r);
                int y = (int)(s * r);
                // Calculate one point of the edge
                int xStart = x + (int)(diagonal * (-s));
                int yStart = y + (int)(diagonal * c);

                // Calculate the other point of the edge
                int xEnd = x - (int)(diagonal * (-s));
                int yEnd = y - (int)(diagonal * c);

                double theta = t;

                // Draw Lines on the copyImage matrice
                if (draw)
                    draw_line(drawImage, width, height, &line, &pixel, 1, draw);

                // Add line on our return list
                void *p = Line_tovptr(line);
                append(&allLines, p);
            }
        }
    }

    // Free cos and sin arrays
    free(arrThetas);
    free(arrRhos);
    freeMatrice(accumulator, nbTheta + 1);

    // Find best angle
    unsigned int angle = 0;
    for (unsigned int i = 0; i < 181; i++)
    {
        if (histogram[i] > histogram[angle])
            angle = i;
    }

    *max_Theta = angle * M_PI / 180.0;
    return allLines;
}


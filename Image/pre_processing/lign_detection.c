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

int** houghtransform(Uint32* pixels,int w, int h, SDL_PixelFormat* format,int draw,double *max_Theta,int *lenliste)
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

    int max = 0;
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
    int lineThreshold = max * 0.6;

    // Create line return line array
    int ** allLines = NULL;
    int lenal = 0;

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
            //condition inutile
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

                //double Theta = t;

                lenal++;
                int *Lines = malloc(4*sizeof(int));
                Lines[0] = xStart;
                Lines[1] = yStart;
                Lines[2] = xEnd;
                Lines[3] = yEnd;
                allLines = realloc(allLines,lenal*sizeof(Lines));
                allLines[lenal-1] = Lines;
                // Draw Lines on the copyImage matrice
                if (draw)
                    draw_line(pixels, w, h, xStart,yStart,xEnd,yEnd, pixel, 1, draw,format);

            }
        }
    }
    // Free cos and sin arrays
    free(arrThetas);
    free(arrRhos);
    freeMatriceInt(accumulator, nbTheta + 1);
    // Find best angle
    unsigned int angle = 0;
    for (unsigned int i = 0; i < 181; i++)
    {
        if (histogram[i] > histogram[angle])
            angle = i;
    }
    *max_Theta = angle * M_PI / 180.0;
    *lenliste = lenal;
    return allLines;
}


void draw_line(Uint32* pixels, int w, int h, int xStart,int yStart,int xEnd,int yEnd, Uint32 color,
               int thickness, int draw,SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(color, format, &r, &g, &b);
    // printf("Drawing line\n");
    int x0 = xStart;
    int y0 = yStart;

    int x1 = xEnd;
    int y1 = yEnd;

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        if (0 <= x0 && x0 < w && 0 <= y0 && y0 < h)
        {
            if (draw)
            {
                pixels[y0*w+x0] = SDL_MapRGB(format,r,g,b);

                if (thickness == 2)
                {
                    if (0 <= (x0 + 1) && (x0 + 1) < w && 0 <= (y0 + 1)
                        && (y0 + 1) < h)
                    {
                        pixels[(y0+1)*w+(x0+1)] = SDL_MapRGB(format,r,g,b);
                    }
                    if (0 <= (x0 - 1) && (x0 - 1) < w && 0 <= (y0 - 1)
                        && (y0 - 1) < h)
                    {
                        pixels[(y0-1)*w+(x0-1)] = SDL_MapRGB(format,r,g,b);
                    }
                }
            }
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}


int** LineSimpl(int** allLines, int* len, int Threshold) {
    int lenRes = 0;
    int **res = NULL;

    for (int i = 0; i < *len - 1; i++)
    {
        if ((*allLines + i) == NULL)
            continue;
        for (int j = i + 1; j < *len; j++)
        {
            if ((*allLines + j) == NULL)
                continue;
            /// - If the two lines are pretty the same
            if (abs((*allLines + i)[0] - (*allLines + j)[0]) < Threshold &&
                abs((*allLines + i)[1] - (*allLines + j)[1]) < Threshold &&
                abs((*allLines + i)[2] - (*allLines + j)[2]) < Threshold &&
                abs((*allLines + i)[3] - (*allLines + j)[3]) < Threshold)
            {
                /// - Put in allLines[i] the average of the two lines
                (*allLines + i)[0] = ((*allLines + i)[0] + (*allLines + j)[0]) / 2;
                (*allLines + i)[1] = ((*allLines + i)[1] + (*allLines + j)[1]) / 2;
                (*allLines + i)[2] = ((*allLines + i)[2] + (*allLines + j)[2]) / 2;
                (*allLines + i)[3] = ((*allLines + i)[3] + (*allLines + j)[3]) / 2;
                free(*(*allLines + j));
                (*allLines + j) = NULL;
            }
            lenRes++;
            res = realloc(res,lenRes*sizeof(*allLines + i));
            (*res+lenRes-1) = (*allLines + i);
        }
    }
    return res;
}

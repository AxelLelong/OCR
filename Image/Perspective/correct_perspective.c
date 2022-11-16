//
// Created by mathieu on 15/11/2022.
//

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "../Detection/lign_detection.h"
#include "../Detection/square_detection.h"
#include "correct_perspective.h"
#include "matrix.h"


void crossProduct(double vect_A[], double vect_B[], double cross_P[])
{
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

void perspectiveMatrix(int src[4][2], double dst[4][2], double** transformation_matrix, double** transformation_matrix_inv)
{
    double P[][9] = {
            { -src[0][0], -src[0][1], -1, 0, 0, 0, src[0][0] * dst[0][0],
                                   src[0][1] * dst[0][0], dst[0][0] },
            { 0, 0, 0, -src[0][0], -src[0][1], -1, src[0][0] * dst[0][1],
                                   src[0][1] * dst[0][1], dst[0][1] },
            { -src[1][0], -src[1][1], -1, 0, 0, 0, src[1][0] * dst[1][0],
                                   src[1][1] * dst[1][0], dst[1][0] },
            { 0, 0, 0, -src[1][0], -src[1][1], -1, src[1][0] * dst[1][1],
                                   src[1][1] * dst[1][1], dst[1][1] },
            { -src[2][0], -src[2][1], -1, 0, 0, 0, src[2][0] * dst[2][0],
                                   src[2][1] * dst[2][0], dst[2][0] },
            { 0, 0, 0, -src[2][0], -src[2][1], -1, src[2][0] * dst[2][1],
                                   src[2][1] * dst[2][1], dst[2][1] },
            { -src[3][0], -src[3][1], -1, 0, 0, 0, src[3][0] * dst[3][0],
                                   src[3][1] * dst[3][0], dst[3][0] },
            { 0, 0, 0, -src[3][0], -src[3][1], -1, src[3][0] * dst[3][1],
                                   src[3][1] * dst[3][1], dst[3][1] },
            { 0, 0, 0, 0, 0, 0, 0, 0, 1 }
    };

    double R[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };

    double P_inv[9][9] = { 0 };

    // Invert matrix P
    inverseMat(P, P_inv, 9);

    // Compute H = P_inv * R
    double* H = calloc(9, sizeof(double));
    multiplyMatStat(P_inv, R, H, 9);

    // Convert H to 3x3 matrix
    int v = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++, v++)
        {
            transformation_matrix[i][j] = H[v];
        }
    }

    inverse3x3Mat(transformation_matrix, transformation_matrix_inv);

    free(H);
}

void correctPerspective(int** square,SDL_Surface* surface, int width , int height)
{
    Uint32* pixels = surface->pixels;
    if (pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    int src[4][2] = { { square[0][0], square[0][1] },
                      { square[1][0], square[1][1] },
                      { square[2][0], square[2][1] },
                      { square[3][0], square[3][1] }
                    };

    int top = getLineLength(square[0]);
    int right = getLineLength(square[1]);
    int bottom = getLineLength(square[2]);
    int left = getLineLength(square[3]);

    double max_edge_length = fmax(fmax(top, right), fmax(bottom, left));

    double dst[4][2] = { { 0, 0 },
                         { max_edge_length, 0 },
                         { max_edge_length, max_edge_length },
                         { 0, max_edge_length } };

    double** transformationMat = allocMat(3);

    double** transformationMatInv = allocMat(3);

    perspectiveMatrix(src, dst, transformationMat, transformationMatInv);

    SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_SWSURFACE, max_edge_length, max_edge_length, 32, 0, 0, 0, 0);

    Uint32* _pixels = new_img->pixels;
    if (_pixels == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for (int i = 0; i < new_img->h; i++)
    {
        for (int j = 0; j < new_img->w; j++)
        {
            double ut = i;
            double vt = j;
            double wt = 1;

            double old_coordinates[3] = { ut, vt, wt };
            double new_coordinates[3] = { 0, 0, 0 };

            multiplyMatBis(transformationMatInv, old_coordinates,
                           new_coordinates, 3);

            int x = (int)(new_coordinates[0] / new_coordinates[2]);
            int y = (int)(new_coordinates[1] / new_coordinates[2]);

            if (x >= 0 && y >= 0 && x < width
                && y < height)
            {
                _pixels[j*new_img->w+i] = pixels[y*width+x];
            }
            else
            {
                _pixels[j*new_img->w+i] = SDL_MapRGB(new_img->format, 0, 0, 0);
            }
        }
    }

    *surface = *new_img;
    freeMat(transformationMat, 3);
    freeMat(transformationMatInv, 3);
    free(new_img);
}
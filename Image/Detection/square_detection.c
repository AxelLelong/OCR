//
// Created by mathieu on 15/11/2022.
//
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "lign_detection.h"
#include "square_detection.h"

int* getIntersection(int* line1,int* line2, int width, int height,int* dot)
{
    if ((line1[2] - line1[0]) != 0
        && (line2[2] - line2[0]) != 0)
    {
        double directCoeff1 = ((double)line1[3] - (double)line1[1])
                              / ((double)line1[2] - (double)line1[0]);
        double directCoeff2 = ((double)line2[3] - (double)line2[1])
                              / ((double)line2[2] - (double)line2[0]);

        double ordOrigin1 = (double)line1[1]
                            - (double)directCoeff1 * (double)line1[0];
        double ordOrigin2 = (double)line2[1]
                            - (double)directCoeff2 * (double)line2[0];

        if (((int)directCoeff1 - (int)directCoeff2) != 0)
        {
            int x = (ordOrigin1 - ordOrigin2) / (directCoeff2 - directCoeff1);
            int y = directCoeff1 * (ordOrigin2 - ordOrigin1)
                    / (directCoeff1 - directCoeff2)
                    + ordOrigin1;

            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                dot[0] = x;
                dot[1] = y;
                return dot;
            }
        }
    }
    dot[0] = -1;
    dot[1] = -1;
    return dot;
}

int getLineLength(int* line)
{
    return sqrt((line[2] - line[0]) * (line[2] - line[0])
                + ((line[3] - line[1]) * (line[3] - line[1])));
}

int getPerimeter(int** square)
{
    // Get perimeter of the square
    return getLineLength(square[3]) + getLineLength(square[1])
           + getLineLength(square[0]) + getLineLength(square[2]);
}

int** sortSquares(int** square1, int** square2)
{
    if(square2 == NULL)
        return square1;

    int Factor1 = getPerimeter(square1);
    int Factor2 = getPerimeter(square2);
    if(Factor1 < Factor2)
        return square2;
    return square1;
}

int** findSquare(int** lineList, int width, int height, int* len)
{
    int** squareFinal = NULL;
    double squareFactor = 40;
    // FIRST COLUMN
    for (int h = 0; h<*len; h++)
    {
        for (int i = 0; i<*len; i++)
        {
            if (i == h)
                continue;

            int* line1 = lineList[h];
            int* line2 = lineList[i];
            // Get all line that actualLine have a intersection point with
            int dot1[2] = {0,0};
            getIntersection(line1, line2, width, height, dot1);

            if (dot1[0] != -1)
            {
                // ALL INTERSECTED EDGES
                for (int j = 0; j<*len; j++)
                {
                    if (i == j)
                        continue;

                    int* line3 = lineList[j];
                    int dot2[2] = {0,0};
                    getIntersection(line2, line3, width, height,dot2);

                    if (dot2[0] != -1)
                    {
                        // ALL INTERSECTED EDGES
                        for (int k = 0; k<*len; k++)
                        {
                            if (k == j)
                                continue;

                            int* line4 = lineList[k];
                            int dot3[2] = {0,0};
                            getIntersection(line3, line4, width, height, dot3);

                            if (dot3[0] != -1)
                            {
                                if (k == h)
                                    continue;

                                // DOES K have intersection with h
                                int dot4[2] = {0,0};
                                getIntersection(line4, line1, width, height, dot4);

                                if (dot4[0] != -1)
                                {
                                    int** square = malloc(4*sizeof(line1));
                                    int* firstLine = malloc(4*sizeof(int));
                                    firstLine[0] = dot1[0];
                                    firstLine[1] = dot1[1];
                                    firstLine[2] = dot2[0];
                                    firstLine[3] = dot2[1];
                                    square[0] = firstLine;

                                    int* secondLine = malloc(4*sizeof(int));
                                    secondLine[0] = dot2[0];
                                    secondLine[1] = dot2[1];
                                    secondLine[2] = dot3[0];
                                    secondLine[3] = dot3[1];
                                    square[1] = secondLine;

                                    int* thirdLine = malloc(4*sizeof(int));
                                    thirdLine[0] = dot3[0];
                                    thirdLine[1] = dot3[1];
                                    thirdLine[2] = dot4[0];
                                    thirdLine[3] = dot4[1];
                                    square[2] = thirdLine;

                                    int* fourthLine = malloc(4*sizeof(int));
                                    fourthLine[0] = dot4[0];
                                    fourthLine[1] = dot4[1];
                                    fourthLine[2] = dot1[0];
                                    fourthLine[3] = dot1[1];
                                    square[3] = fourthLine;

                                    // Not a square
                                    if (!isSquare(square,squareFactor))
                                    {
                                        continue;
                                    }
                                    squareFinal = sortSquares(square, squareFinal);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return squareFinal;
}

int isSquare(int** square,double SQUARE_FACTOR)
{
    // Avoid warning
    //(void)width;
    //(void)height;

    unsigned int lenLeft = getLineLength(square[3]);

    unsigned int lenright = getLineLength(square[1]);

    unsigned int lentop = getLineLength(square[0]);

    unsigned int lenbottom = getLineLength(square[2]);

    unsigned int max = lenLeft;
    if (lenright > max)
        max = lenright;
    if (lenbottom > max)
        max = lenbottom;
    if (lentop > max)
        max = lentop;

    unsigned int min = lenLeft;
    if (lenright < min)
        min = lenright;
    if (lenbottom < min)
        min = lenbottom;
    if (lentop < min)
        min = lentop;

    unsigned int val = max - min;

    if (val > SQUARE_FACTOR)
        return 0;

    return 1;
}

void drawSquare(int** square, Uint32* pixels, int width, int height,
                int thickness,SDL_PixelFormat* format,int draw)
{
    Uint32 pixel = SDL_MapRGB(format, 255, 0, 255);
    draw_line(pixels, width, height, square[3][0],square[3][1],square[3][2],square[3][3], pixel, thickness, draw,format);
    draw_line(pixels, width, height, square[2][0],square[2][1],square[2][2],square[2][3], pixel, thickness, draw,format);
    draw_line(pixels, width, height, square[0][0],square[0][1],square[0][2],square[0][3], pixel, thickness, draw,format);
    draw_line(pixels, width, height, square[1][0],square[1][1],square[1][2],square[1][3], pixel, thickness, draw,format);
}

void compute_Square(int** square)
{
    int dot1[2] = {square[0][0],square[0][1]};
    int dot2[2] = {square[1][0],square[1][1]};
    int dot3[2] = {square[2][0],square[2][1]};
    int dot4[2] = {square[3][0],square[3][1]};

    // Compute addition
    int cord1 = dot1[0] + dot1[1];
    int cord2 = dot2[0] + dot2[1];
    int cord3 = dot3[0] + dot3[1];
    int cord4 = dot4[0] + dot4[1];

    int mark[4] = { 0, 0, 0, 0 };
    int index = 0;

    // Compare them and get all dot
    int min = cord1;
    int* topLeft = dot1;
    if (cord2 < min)
    {
        min = cord2;
        topLeft = dot2;
        index = 1;
    }
    if (cord3 < min)
    {
        min = cord3;
        topLeft = dot3;
        index = 2;
    }
    if (cord4 < min)
    {
        min = cord4;
        topLeft = dot4;
        index = 3;
    }
    mark[index] = 1;

    // Compute max
    int max = cord1;
    int* bottomRight = dot1;
    index = 0;
    if (cord2 > max)
    {
        max = cord2;
        bottomRight = dot2;
        index = 1;
    }
    if (cord3 > max)
    {
        max = cord3;
        bottomRight = dot3;
        index = 2;
    }
    if (cord4 > max)
    {
        max = cord4;
        bottomRight = dot4;
        index = 3;
    }
    mark[index] = 1;

    int* topRight = dot1;
    int* bottomLeft = dot1;
    if (mark[0] == 0 && mark[1] == 0)
    {
        if (dot1[0] < dot2[0])
        {
            bottomLeft = dot1;
            topRight = dot2;
        }
        else
        {
            bottomLeft = dot2;
            topRight = dot1;
        }
    }

    if (mark[0] == 0 && mark[2] == 0)
    {
        if (dot1[0] < dot3[0])
        {
            bottomLeft = dot1;
            topRight = dot3;
        }
        else
        {
            bottomLeft = dot3;
            topRight = dot1;
        }
    }

    if (mark[0] == 0 && mark[3] == 0)
    {
        if (dot1[0] < dot4[0])
        {
            bottomLeft = dot1;
            topRight = dot4;
        }
        else
        {
            bottomLeft = dot4;
            topRight = dot1;
        }
    }

    if (mark[1] == 0 && mark[2] == 0)
    {
        if (dot2[0] < dot3[0])
        {
            bottomLeft = dot2;
            topRight = dot3;
        }
        else
        {
            bottomLeft = dot3;
            topRight = dot2;
        }
    }

    if (mark[1] == 0 && mark[3] == 0)
    {
        if (dot2[0] < dot4[0])
        {
            bottomLeft = dot2;
            topRight = dot4;
        }
        else
        {
            bottomLeft = dot4;
            topRight = dot2;
        }
    }

    if (mark[2] == 0 && mark[3] == 0)
    {
        if (dot3[0] < dot4[0])
        {
            bottomLeft = dot3;
            topRight = dot4;
        }
        else
        {
            bottomLeft = dot4;
            topRight = dot3;
        }
    }

    // Set each start and end point of the line in the square
    square[0][0] = topLeft[0];
    square[0][1] = topLeft[1];
    square[0][2] = topRight[0];
    square[0][3] = topRight[1];


    square[1][0] = topRight[0];
    square[1][1] = topRight[1];
    square[1][2] = bottomRight[0];
    square[1][3] = bottomRight[1];


    square[2][0] = bottomRight[0];
    square[2][1] = bottomRight[1];
    square[2][2] = bottomLeft[0];
    square[2][3] = bottomLeft[1];

    square[3][0] = bottomLeft[0];
    square[3][1] = bottomLeft[1];
    square[3][2] = topLeft[0];
    square[3][3] = topLeft[1];

}

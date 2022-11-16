//
// Created by mathieu on 15/11/2022.
//

#ifndef OCR_SQUARE_DETECTION_H
#define OCR_SQUARE_DETECTION_H


int* getIntersection(int* line1,int* line2, int width, int height, int* dot);
int** findSquare(int** lineList, int width, int height,
                 int* len);
int isSquare(int** square,double SQUARE_FACTOR);
int getLineLength(int* line);
void drawSquare(int** square, Uint32* pixels, int width, int height,
                int thickness,SDL_PixelFormat* format,int draw);
void compute_Square(int** square);
#endif //OCR_SQUARE_DETECTION_H

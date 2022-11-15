//
// Created by mathieu on 15/11/2022.
//

#ifndef TRANSFORMATION_C_CORRECT_PERSPERTIVE_H
#define TRANSFORMATION_C_CORRECT_PERSPERTIVE_H

void crossProduct(double vect_A[], double vect_B[], double cross_P[]);
void perspectiveMatrix(int src[4][2], double dst[4][2], double** transformation_matrix, double** transformation_matrix_inv);
void correctPerspective(int** square,SDL_Surface* surface, int width , int height);

#endif //TRANSFORMATION_C_CORRECT_PERSPERTIVE_H

//
// Created by mathieu on 15/11/2022.
//

#ifndef TRANSFORMATION_C_MATRIX_H
#define TRANSFORMATION_C_MATRIX_H

#define _MAX 9
#define N 9

void Minor(double minorMatrix[9][9], int colMatrix, int sizeMatrix, double newMinorMatrix[9][9]);
double Determinte(double minorMatrix[9][9], int sizeMatrix);
void Transpose(double cofactorMatrix[9][9], double sizeMatrix, double determinte, double coutMatrix[9][9], double transposeMatrix[9][9]);
void Cofactor(double cinMatrix[9][9], double sizeMatrix, double determinte, double coutMatrix[9][9], double transposeMatrix[9][9]);
void Inverse(double cinMatrix[9][9], int sizeMatrix, double determinte, double coutMatrix[9][9], double transposeMatrix[9][9]);
void inverseMat(double cinMatrix[9][9], double coutMatrix[9][9], int sizeMatrix);
void freeMat(double **mat, int n);
void multiplyMat(double **A, double **B, int size);
void inverse3x3Mat(double **M, double **M_inv);
void multiplyMatStat(double M[][_MAX], double v[_MAX], double v_out[_MAX], int size);
void multiplyMatBis(double **M, double *v, double *v_out, int size);
double **allocMat(int size);

#endif //TRANSFORMATION_C_MATRIX_H

#include "sudoSolver.h"
#include <stdio.h>

//test if the board is valid (if there aren't same numbers on the same line/cols/square)
int IsBoardValid(int sudoku[81])
{
    int isValid = 1;

    // Verification lignes -------------------------------------------------------

    size_t i = 0;
    while(isValid == 1 && i < 9)
    {
        int occ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        size_t j = 0;
        while(isValid == 1 && j < 9)
        {
            int n = sudoku[i*9+j] - 1;

            if (n != -1)
            {
                if (occ[n] != 1)
                {
                    occ[n] += 1;
                }
                else
                {
                    isValid = 0;
                }
            }

            j++;
        }

        i++;
     }


    // Verification colonnes ---------------------------------------------------------

    i = 0;
    while(isValid == 1 && i < 9)
    {
        int occ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        size_t j = 0;
        while(isValid ==1 && j < 9)
        {
            int n = sudoku[j*9+i] - 1;

            if (n != -1)
            {
                if (occ[n] != 1)
                {
                    occ[n] += 1;
                }
                else
                {
                    isValid = 0;
                }
            }

            j++;
        }

        i++;
    }

    // Verification cases ----------------------------------------------------------

    i = 0;

    while (isValid == 1 && i<3)
    {
        size_t j = 0;
        while (isValid == 1 && j<3)
        {
            int occ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            size_t k = 0;
            while (isValid == 1 && k<3)
            {
                size_t l = 0;
                while (isValid == 1 && l<3)
                {
                    int n = sudoku[(3 * i + k)*9+3 * j + l]-1;
                    if (n != -1)
                    {
                        if (occ[n] != 1)
                        {
                            occ[n] += 1;
                        }
                        else
                        {
                            isValid = 0;
                        }
                    }

                    l++;
                }

                k++;
            }

            j++;
        }

        i++;
    }
    return isValid;
}

//test if the board is full and valid
int IsSolved(int sudoku[81])
{
    int isFull = 1;
    size_t i = 0;
    while (isFull == 1 && i < 9)
    {
        size_t j = 0;
        while (j<9 && sudoku[i*9+j] != 0)
        {
            j++;
        }

        if (j==9)
            isFull = 1;
        else
            isFull =0;
        i++;
    }
    return (isFull + IsBoardValid(sudoku))/2;
}


//Solve the sudoku by testing all possibilities recursively
int Solve(int sudoku[81])
{
    if (!IsBoardValid(sudoku))
    {
        return 0;
    }

    int i = 0;
    int j;

    while (i < 9)
    {
        j = 0;
        while (j<9 && sudoku[i*9+j]!=0)
        {
            j++;
        }

        if (j == 9)
            i++;
        else
            break;
    }

    if (i == 9)
        return 1;

    int k = 1;
    int isSolve = 0;
    while (!isSolve && k<=9)
    {
        sudoku[i*9+j] = k;
        isSolve = Solve(sudoku);
        k += 1;
    }

    if (!isSolve)
    {
        sudoku[i*9+j] = 0;
    }
    return isSolve;
}

#include "sudoSolver.h"
#include <stdio.h>

public int IsBoardValid(int sudoku[][])
{
    int isValid = 1;

    // Verification colonnes -------------------------------------------------------

    size_t i = 0;
    while(isValid == 1 && i < 9)
    {
        int occ[] = [0, 0, 0, 0, 0, 0, 0, 0, 0];
        size_t j = 0;
        while(isValid == 1 && j < 9)
        {
            int n = sudoku[i][j] - 1;

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

    // Verification lignes ---------------------------------------------------------

    i = 0;
    while(isValid == 1 && i < 9)
    {
        int occ[] = [0, 0, 0, 0, 0, 0, 0, 0, 0];
        size_t j = 0;
        while(isValid ==0 && j < 9)
        {
            int n = Board[j][i] - 1;

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

    while (isValid == 0 && i<3)
    {
        size_t j = 0;
        while (isValid == 0 && j<3)
        {
            int occ[] = [0, 0, 0, 0, 0, 0, 0, 0, 0];
            size_t k = 0;
            while (isValid == 0 && k<3)
            {
                size_t l = 0;
                while (isValid == 0 && l<3)
                {
                    int n = Board[3 * i + k][3 * j + l]-1;
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

public int IsSolved()
{
    int isFull = 1;
    size_t i = 0;
    while (isFull == 1 && i < 9)
    {
        size_t j = 0;
        while (j<9 && Board[i][j] != 0)
        {
            j++;
        }

        if (j==9)
            isFull = 1;
        else
            isFull =0;
        i++;
    }
    return (isFull + IsBoardValid())/2;
}

public bool Solve(int sudoku[][])
{
    if (IsBoardValid(sudoku))
    {
        return 0;
    }

    int i = 0;
    int j = 0;

    while (i < 9)
    {
        j = 0;
        while (j<9 && Board[i][j]!=0)
        {
            j++;
        }

        if (j == 9)
            i++;
        else
            break;
    }

    if (i == 9)
        return true;

    int k = 1;
    bool isSolve = false;
    while (!isSolve && k<=BoardSize)
    {
        Board[i, j] = k;
        isSolve = Solve();
        k += 1;
    }

    if (!isSolve)
    {
        Board[i][j] = 0;
    }
    return isSolve;
}
#include "sudoLoader.h"
#include "sudoSolver.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

void sudoPrint(int sudo[81])
{
    for (size_t i = 0; i<9; i++)
    {
        for (size_t j = 0; j<9;j++)
        {
            if (sudo[i*9+j] == 0)
                printf("   |");

            else
                printf(" %i |",sudo[i*9+j]);
        }
        printf("\n");

    }
}

int main(int argc, char** argv)
{
    if (argc!=2)
    {
        errx(EXIT_FAILURE, "Usage : 1 arg : path");
    }
    /// - Initialize the array of the sudoku
    int* sudoku = malloc(81*sizeof(int));

    /// - Convert the file in an array
    /// - argv[1] is the path of the file
    Loader(argv[1], sudoku);

    /// - Solve the sudoku
    int isSolve = Solve(sudoku);

    /// - Write the sudoku in a new file
    if (isSolve)
        Writer(sudoku);
    else
    {
        errx(EXIT_FAILURE, "The sudoku can't be Solve\n");
    }
    free(sudoku);
    return 0;
}

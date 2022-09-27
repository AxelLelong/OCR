#include "sudoLoader.h"
#include "sudoSolver.h"
#include "stdio.h"

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
        return 1;
    }
    //initialize the array of the sudoku
    int sudoku[81];

    //Convert the file in an array
    //argv[1] is the path of the file
    Loader(argv[1], sudoku);

    //Solve the sudoku
    int isSolve = Solve(sudoku);
    if (isSolve)
        Writer(argv[1],sudoku);
    return 0;
}

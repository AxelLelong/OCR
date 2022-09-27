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

int main() {
    int sudoku[81];
    Loader("./testFileSudo", sudoku);
    sudoPrint(sudoku);
    Solve(sudoku);
    sudoPrint(sudoku);
}

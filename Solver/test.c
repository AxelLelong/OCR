#include "sudoLoader.h"
#include "sudoSolver.h"
#include "stdio.h"

void sudoPrint(int sudo[9][9])
{
    for (size_t i = 0; i<9; i++)
    {
        for (size_t j = 0; j<9;j++)
        {
            if (sudo[i][j] == 0)
                printf("   |");

            else
                printf(" %i |",sudo[i][j]);
        }
        printf("\n");

    }
}

int main() {
    int sudoku[9][9];
    Loader("./testFileSudo", sudoku);
    sudoPrint(sudoku);

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j) {
            printf("%d", sudoku[i][j]);
        }
        printf("\n");
    }
}
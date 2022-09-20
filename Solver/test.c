#include "sudoLoader.h"
#include "stdio.h"

void sudoPrint(int sudo[][])
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
        printf("\n")
    }
}

sudoPrint(Loader("./testFileSudo"))
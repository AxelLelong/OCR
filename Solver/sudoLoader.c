#include "sudoLoader.h"
#include "stdio.h"

//int sudoku[9][9];

void Loader(char path[], int sudoku[9][9])
{
    //initialize the sudoku


    //open the text file
    FILE* txt = fopen(path,"r");

    int j = 0;
    while (j<11)
    {
        //read a line
        char s[11];
        char tmp[3];

        //3 char per 3 char
        for (int i = 0; i < 3; ++i) {
            fscanf(txt,"%s*", tmp);

        }
        //just to see : NEED TO BE DELETED
        printf("line == %s\n",s);


        //for space between boxes
        if (j==3 || j == 6)
        {
            j++;
            continue;
        }

        int i = 0;
        while (i<11)
        {
            //for space between boxes
            if (i == 3 || i == 6)
            {
                i++;
                continue;
            }

            //testing if it is a number
            if (s[i]<58 && s[i]>48)
            {
                //add the value to the sudoku
                sudoku[i][j] = s[i] - '0';
            }
            //it's a dot
            else
            {
                sudoku[i][j] = 0;
            }
            i++;
        }
        j++;

    }
}

/*int IsBoardValid()
{
    int isValid = 1;

    // Verification colonnes -------------------------------------------------------

    size_t i = 0;
    while(isValid == 1 && i < 9)
    {
        int occ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
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
        int occ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        size_t j = 0;
        while(isValid ==0 && j < 9)
        {
            int n = sudoku[j][i] - 1;

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
            int occ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            size_t k = 0;
            while (isValid == 0 && k<3)
            {
                size_t l = 0;
                while (isValid == 0 && l<3)
                {
                    int n = sudoku[3 * i + k][3 * j + l]-1;
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

    int IsSolved()
{
    int isFull = 1;
    size_t i = 0;
    while (isFull == 1 && i < 9)
    {
        size_t j = 0;
        while (j<9 && sudoku[i][j] != 0)
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

int Solve()
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
        while (j<9 && sudoku[i][j]!=0)
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
        sudoku[i][j] = k;
        isSolve = Solve(sudoku);
        k += 1;
    }

    if (!isSolve)
    {
        sudoku[i][j] = 0;
    }
    return isSolve;
}*/
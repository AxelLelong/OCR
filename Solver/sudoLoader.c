#include "sudoLoader.h"
#include "stdio.h"

int[][] Loader(char path[])
{
    //initialize the sudoku
    int sudoku[9][9];


    //open the text file
    txt = fopen(path,"r");

    int j = 0;
    while (j<11)
    {
        //read a line
        char s[] = fscanf(txt,"%s");

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
                sudoku[i][j]=0;
            }
            i++;
        }
        j++;

    }
    return
}


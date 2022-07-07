#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int histogramao(int vetorDec[], int c, int r)
{
    int total =  c * r;
    int histograma[total];

    for (int i = 0; i < total; i++)
    {
        histograma[i] = 0;
    }

    for (int i = 0; i < total; i++)
    {
        for (int j = 0; j < total; j++)
        {
            if (i == vetorDec[j])
            {
                histograma[i]++;
            }
        }
    }
    return histograma;
}
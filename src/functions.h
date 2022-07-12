#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct pgm
{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData;
};

void readPGMImage(struct pgm *pio, char *filename);

void writePGMImage(struct pgm *pio, char *filename);

void viewPGMImage(struct pgm *pio);

//Funções próprias:

int PercorrerMatriz(struct pgm *pio);

int conversorBinDec(int bin);

int histogramaVetor(struct pgm *pio, int vetorDec[]);

int escreverArquivo();


#endif
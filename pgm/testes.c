/* Ambiente de testes controlados com uma matriz propria */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct pgm
{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData; // <- Depois do readPGMImage os dados que utilizaremos estaram salvos nesse ponteiro.
};

void PercorrerMatriz(struct pgm *);
int conversorBinDec(int);
int main()
{
	struct pgm pio;
	pio.c = 10;
	pio.r = 10;

	for (int i = 0; i < pio.c * pio.r; i++)
	{
		*(pio.pData + i) = i;
	}
	for (int k = 0; k < pio.c * pio.r; k++) //(pio->r * pio->c)
	{
		if (!(k % pio.c))
		{
			printf("\n");
		}
		printf("[ %d ]", *(pio.pData + k));
	}
	printf("\n");
	PercorrerMatriz(&pio);
}

void PercorrerMatriz(struct pgm *pio)
{
	int i, j, bin, saida = 0;
	int v[3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	for (i = 1; i < 2; i++) // A linha mais importante -> original (i = 0; i < pio->c * pio->r; i++)
	{
		// Canto superior esquerdo - Caso 1 - Funcionando corretamente (10/07)
		if ((pio->pData + i) == pio->pData)
		{
			v[0][0] = 0; // zera os tres de cima
			v[0][1] = 0;
			v[0][2] = 0;

			v[1][0] = 0; // zera os dois da esquerda
			v[2][0] = 0;

			v[1][1] = *(pio->pData); // preenche os valores

			v[1][2] = *(pio->pData + 1);
			v[2][1] = *(pio->pData + pio->c);
			v[2][2] = *(pio->pData + pio->c + 1);
			printf("CASO 1\n");
		}

		// Centro primeira linha - Caso 2 - Funcionando corretamente (10/07)
		if ((pio->pData + i) < (pio->pData + pio->c - 1) && (pio->pData + i) > (pio->pData))
		{				 // Verifica caso o numero da linha de cima seja zero e os laterais nao.
			v[0][0] = 0; // zera os tres de cima
			v[0][1] = 0;
			v[0][2] = 0;

			v[1][0] = *(pio->pData + i - 1); // zera os dois da esquerda
			v[2][0] = *(pio->pData + pio->c - 1 + i);

			v[1][1] = *(pio->pData + i); // preenche os valores
			v[1][2] = *(pio->pData + i + 1);
			v[2][1] = *(pio->pData + pio->c + i);
			v[2][2] = *(pio->pData + pio->c + 1 + i);
			printf("CASO 2\n");
		}

		// Canto superior direito - Caso 3 - Funcionando corretamente (10/07)
		if ((pio->pData + i) == (pio->pData + pio->c - 1))
		{
			v[0][0] = 0; // zera os tres de cima
			v[0][1] = 0;
			v[0][2] = 0;

			v[1][2] = 0; // zera os dois da direita
			v[2][2] = 0;

			v[1][1] = *(pio->pData + i); // preenche os valores

			v[2][1] = *(pio->pData + pio->c + i);
			v[1][0] = *(pio->pData - 1 + i); // preenche os valores da esquerda
			v[2][0] = *(pio->pData + pio->c + i - 1);
			printf("CASO 3\n");
		}

		// Coluna centro esquerda - Caso 4 - Funcionando corretamente (10/07)
		for (int k = 1; k < pio->r; k++)
		{
			if (((pio->pData + i) == (pio->pData + (pio->c * k))) && (pio->pData + i != pio->pData))
			{				 //((pio->pData + pio->c-1 + (pio->c * k))
				v[0][0] = 0; // zera os tres de esquerda
				v[1][0] = 0;
				v[2][0] = 0;

				v[0][1] = *(pio->pData - pio->c + i);
				v[0][2] = *(pio->pData - pio->c + i + 1);

				v[1][1] = *(pio->pData + i); // preenche os valores

				v[1][2] = *(pio->pData + i + 1);
				v[2][1] = *(pio->pData + pio->c + i);
				v[2][2] = *(pio->pData + i + pio->c + 1);
				printf("CASO 4\n");
				break;
			}
		}

		// Centro - Caso 5 Funcionando corretamente (11/07)
		if ((pio->pData + pio->c) < (pio->pData + i) && (pio->pData + i) < (pio->pData + (pio->c * (pio->r - 1)) - 1))
		{
			int aux = 0;
			for (int k = 2; k < pio->r; k++)
			{
				if (((pio->pData + i) == (pio->pData + k * pio->c)) || ((pio->pData + i) == (pio->pData + (k * pio->c) - 1)))
				{
					aux = 1;
				}
			}
			if (aux == 0)
			{
				v[0][0] = *(pio->pData - pio->c + i - 1);
				v[1][0] = *(pio->pData + i - 1);
				v[2][0] = *(pio->pData + pio->c - 1 + i);
				v[0][1] = *(pio->pData - pio->c + i);
				v[0][2] = *(pio->pData - pio->c + i + 1);
				v[1][1] = *(pio->pData + i);
				v[1][2] = *(pio->pData + i + 1);
				v[2][1] = *(pio->pData + pio->c + i);
				v[2][2] = *(pio->pData + i + pio->c + 1);
			}
		}

		// Coluna centro direita - Caso 6 - Funcionando corretamente (10/07)
		for (int k = 1; k < pio->r; k++)
		{
			if (((pio->pData + i) == (pio->pData + (k * pio->c) - 1)) && (pio->pData + i) != (pio->pData + pio->c - 1) && (pio->pData + i) != (pio->pData + (pio->c * pio->r) - 1))
			{				 //((pio->pData + pio->c-1 + (pio->c * k))
				v[0][2] = 0; // zera os tres da direita
				v[1][2] = 0;
				v[2][2] = 0;

				v[0][0] = *(pio->pData - pio->c + i - 1);
				v[0][1] = *(pio->pData - pio->c + i);
				v[1][0] = *(pio->pData + i - 1);

				v[1][1] = *(pio->pData + i); // preenche os valores

				v[2][0] = *(pio->pData + pio->c + i - 1);
				v[2][1] = *(pio->pData + i + pio->c);
				printf("CASO 6\n");
				break;
			}
		}

		// Canto inferior esquerdo - Caso 7 - Funcionando corretamente (10/07)
		if ((pio->pData + i) == (pio->pData + (pio->c * (pio->r - 1))))
		{
			v[2][0] = 0; // zera os tres de baixo
			v[2][1] = 0;
			v[2][2] = 0;

			v[0][0] = 0; // zera os dois da esquerda
			v[1][0] = 0;

			v[1][1] = *(pio->pData + i); // preenche os valores

			v[0][1] = *(pio->pData - pio->c + i);
			v[0][2] = *(pio->pData - pio->c + 1 + i);
			v[1][2] = *(pio->pData + i + 1);
			printf("CASO 7\n");
		}

		// Centro linha inferior - Caso 8 - Funcionando corretamente (10/07)
		if ((pio->pData + i) > (pio->pData + (pio->c * (pio->r - 1))) && (pio->pData + i) < (pio->pData + (pio->c * pio->r) - 1))
		{
			v[2][0] = 0; // zera os tres de baixo
			v[2][1] = 0;
			v[2][2] = 0;

			v[1][1] = *(pio->pData + i); // preenche os valores

			v[0][0] = *(pio->pData - pio->c + i - 1);
			v[0][1] = *(pio->pData - pio->c + i);
			v[0][2] = *(pio->pData - pio->c + 1 + i);
			v[1][0] = *(pio->pData + i - 1);
			v[1][2] = *(pio->pData + i + 1);
			printf("CASO 8\n");
		}

		// Canto inferior direito - Caso 9 - Funcionando corretamente (10/07)
		if ((pio->pData + i) == (pio->pData + (pio->c * pio->r) - 1))
		{
			v[0][2] = 0; // zera os tres da direita
			v[1][2] = 0;
			v[2][2] = 0;

			v[2][0] = 0; // zera os dois de baixo
			v[2][1] = 0;

			v[1][1] = *(pio->pData + i);

			v[0][0] = *(pio->pData - pio->c + i - 1);
			v[0][1] = *(pio->pData - pio->c + i);
			v[1][0] = *(pio->pData + i - 1);
			printf("CASO 9\n");
		}

		printf("ATUAL: %d\n", *(pio->pData + i));
		printf("\n Nossa matriz: \n");
		for (i = 0; i <= 2; i++)
		{ // mostra a matriz gerada
			for (j = 0; j <= 2; j++)
			{
				printf("[  %d  ]", v[i][j]);
			}
			printf("\n");
		}

		for (int a = 0; a < 3; a++)
		{
			for (int b = 0; b < 3; b++)
			{
				if (a == 1 && b == 1)
				{
					// Nada
				}
				else
				{
					if (v[1][1] > v[a][b])
					{
						v[a][b] = 0;
					}
					else
					{
						v[a][b] = 1;
					}
				}
			}
		}
	}
	bin = (v[0][0] * pow(10, 0)) + (v[0][1] * pow(10, 1)) + (v[0][2] * pow(10, 2)) + (v[1][2] * pow(10, 3)) + (v[2][2] * pow(10, 4)) + (v[2][1] * pow(10, 5)) + (v[2][0] * pow(10, 6)) + (v[1][0] * pow(10, 7));
	printf("\n Nossa binarizada: \n");
	for (i = 0; i <= 2; i++)
	{
		for (j = 0; j <= 2; j++)
		{
			printf("[  %d  ]", v[i][j]);
		}
		printf("\n");
	}
	printf("Binário: %d\n", bin);
	saida = conversorBinDec(bin);
	printf("Decimal: %d\n", saida);
}
int conversorBinDec(int bin)
{
	int total = 0;
	int potenc = 1;

	while (bin > 0)
	{
		total += bin % 10 * potenc;
		bin = bin / 10;
		potenc = potenc * 2;
	}

	return total;
}
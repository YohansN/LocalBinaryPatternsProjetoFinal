#include <stdio.h>
#include <stdlib.h>

struct pgm
{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData; // <- Depois do readPGMImage os dados que utilizaremos estaram salvos nesse ponteiro.
};

void readPGMImage(struct pgm *, char *);
void viewPGMImage(struct pgm *);
void writePGMImage(struct pgm *, char *);
void PercorrerMatriz(struct pgm *);

int main(int argc, char *argv[])
{

	struct pgm img;

	if (argc != 3)
	{
		printf("Formato: \n\t %s <imagemEntrada.pgm> <imagemSaida.pgm>\n", argv[0]);
		exit(1);
	}

	readPGMImage(&img, argv[1]);

	writePGMImage(&img, argv[2]);

	viewPGMImage(&img);

	PercorrerMatriz(&img);

	return 0;
}

void readPGMImage(struct pgm *pio, char *filename)
{

	FILE *fp;
	char ch;

	if (!(fp = fopen(filename, "r")))
	{
		perror("Erro.");
		exit(1);
	}

	if ((ch = getc(fp)) != 'P')
	{
		puts("A imagem fornecida não está no formato pgm");
		exit(2);
	}

	pio->tipo = getc(fp) - 48;

	fseek(fp, 1, SEEK_CUR);

	while ((ch = getc(fp)) == '#')
	{
		while ((ch = getc(fp)) != '\n')
			;
	}

	fseek(fp, -1, SEEK_CUR);

	fscanf(fp, "%d %d", &pio->c, &pio->r);
	if (ferror(fp))
	{
		perror(NULL);
		exit(3);
	}
	fscanf(fp, "%d", &pio->mv);
	fseek(fp, 1, SEEK_CUR);

	pio->pData = (unsigned char *)malloc(pio->r * pio->c * sizeof(unsigned char));

	// Deve haver uma validacao aqui
	if (!pio->pData)
	{
		puts("A memoria nao foi alocada corretamente!");
		exit(1);
	}

	switch (pio->tipo)
	{
	case 2:
		puts("Lendo imagem PGM (dados em texto)");
		for (int k = 0; k < (pio->r * pio->c); k++)
		{
			fscanf(fp, "%hhu", pio->pData + k);
		}
		break;
	case 5:
		puts("Lendo imagem PGM (dados em binário)");
		fread(pio->pData, sizeof(unsigned char), pio->r * pio->c, fp);
		break;
	default:
		puts("Não está implementado");
	}

	fclose(fp);
}

void writePGMImage(struct pgm *pio, char *filename)
{
	FILE *fp;
	char ch;

	if (!(fp = fopen(filename, "wb")))
	{
		perror("Erro.");
		exit(1);
	}

	fprintf(fp, "%s\n", "P5");
	fprintf(fp, "%d %d\n", pio->c, pio->r);
	fprintf(fp, "%d\n", 255);

	fwrite(pio->pData, sizeof(unsigned char), pio->c * pio->r, fp);

	fclose(fp);
}

void viewPGMImage(struct pgm *pio)
{
	printf("Tipo: %d\n", pio->tipo);
	printf("Dimensões: [%d %d]\n", pio->c, pio->r);
	printf("Max: %d\n", pio->mv);

	for (int k = 0; k < pio->c * 8; k++) //(pio->r * pio->c)
	{
		if (!(k % pio->c))
			printf("\n");
		printf("%2hhu ", *(pio->pData + k));
	}
	printf("\n");
}

void PercorrerMatriz(struct pgm *pio)
{
	int i, j = 0;
	int v[3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	for (i = 0; i <= (pio->c * 5) + 2; i++) // A linha mais importante -> original (i = 0; i < pio->c * pio->r; i++)
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
			if (((pio->pData + i) == (pio->pData + k * pio->c - 1)) && (pio->pData + i) != (pio->pData + pio->c - 1) && (pio->pData + i) != (pio->pData + (pio->c * pio->r) - 1))
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

			v[1][1] = *(pio->pData + i); // preenche os valores

			v[0][0] = *(pio->pData - pio->c + i - 1);
			v[0][1] = *(pio->pData - pio->c + i);
			v[1][0] = *(pio->pData + i - 1);
			printf("CASO 9\n");
		}
	}

	printf("%d\n", pio->c);
	printf("%d", pio->r);

	i, j = 0;
	printf("Nossa matriz: \n");
	for (i = 0; i <= 2; i++)
	{ // mostra a matriz gerada
		for (j = 0; j <= 2; j++)
		{
			printf("[  %d  ]", v[i][j]);
		}
		printf("\n");
	}
}

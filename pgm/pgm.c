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
void PercorrerVetor(struct pgm *);
int main(int argc, char *argv[])
{

	struct pgm img;

	if (argc != 3)
	{
		printf("Formato: \n\t %s <imagemEntrada.pgm> <imagemSaida.pgm>\n", argv[0]);
		exit(1);
	}

	readPGMImage(&img, argv[1]);

	// Nosso trabalho
	// PercorrerVetor(&img);

	// Fim do nosso trabalho
	// writePGMImage(&img, argv[2]);

	// viewPGMImage(&img);

	PercorrerVetor(&img);

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

	for (int k = 0; k < (pio->r * pio->c); k++)
	{
		if (!(k % pio->c))
			printf("\n");
		printf("%2hhu ", *(pio->pData + k));
	}
	printf("\n");
}

void PercorrerVetor(struct pgm *pio)
{
	int i, j = 0;
	int v[3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (i = 0; i < 256; i++) // A linha mais importante
	{
		j = -4;
		for (int k = 0; k <= 2; k++)
		{
			for (int l = 0; l <= 2; l++)
			{

				if (!*(pio->pData + i + j))
				{
					v[k][l] = 0;
				}
				else
				{
					v[k][l] = *(pio->pData + i + j);
				}
				j++;
			}
		}
	}
	i, j = 0;
	printf("Nossa matriz: \n");
	for (i = 0; i <= 2; i++)
	{ // mostra a matriz gerada
		for (j = 0; j <= 2; j++)
		{
			printf("[  %d  ]", v[i][j]);
		}
		printf("\n ");
	}
}
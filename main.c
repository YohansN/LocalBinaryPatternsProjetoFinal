#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <time.h>

struct pgm
{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData;
};

int *percorrerMatriz(struct pgm *);
int conversorBinDec(int);
void histogramaVetor(struct pgm *, int *, char);
void readPGMImage(struct pgm *pio, char *filename);
void writePGMImage(struct pgm *pio, char *filename);
void viewPGMImage(struct pgm *pio);

int main(){
    struct pgm img;
	DIR *d;
    struct dirent *dir;
	char base[50];
	int *aux;
	int k;
	clock_t begin, end;
	double time_per_img, time_total=0;
	long long int a = 999999999;
	int QTDIMG;
    d = opendir("./imgs");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strcpy(base, "./imgs/");
			char *caminho = strcat(base, dir->d_name);
            char primeiro = dir->d_name[0];
            
            
            if(strstr(caminho, ".pgm")) {
            	readPGMImage(&img, caminho);
				begin = clock();
				histogramaVetor(&img, percorrerMatriz(&img), primeiro);
				while(a){
					a--;
				}

				end = clock();
				
				time_per_img = (double)(end - begin) / CLOCKS_PER_SEC;
				time_total += time_per_img;
				QTDIMG += 1;
            }
        }
		printf("Tempo médio: %lf\n",time_total/QTDIMG);
		printf("Tempo Total: %lf\n",time_total);
        closedir(d);

    }
	
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
		while ((ch = getc(fp)) != '\n');
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

	if (!pio->pData)
	{
		puts("A memoria nao foi alocada corretamente!");
		exit(1);
	}

	switch (pio->tipo)
	{
	case 2:
		//puts("Lendo imagem PGM (dados em texto)");
		for (int k = 0; k < (pio->r * pio->c); k++)
		{
			fscanf(fp, "%hhu", pio->pData + k);
		}
		break;
	case 5:
		//puts("Lendo imagem PGM (dados em binário)");
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

int *percorrerMatriz(struct pgm *pio)
{
	int *vetorSaida;
	vetorSaida = (int* )malloc(pio->c * pio->r * sizeof(int));
	int i, j, bin, saidaDec = 0;
	int v[3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	for (i = 0; i < pio->c * pio->r; i++) // A linha mais importante -> original (i = 0; i < pio->c * pio->r; i++)
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

		bin = (v[0][0] * pow(10, 0)) + (v[0][1] * pow(10, 1)) + (v[0][2] * pow(10, 2)) + (v[1][2] * pow(10, 3)) + (v[2][2] * pow(10, 4)) + (v[2][1] * pow(10, 5)) + (v[2][0] * pow(10, 6)) + (v[1][0] * pow(10, 7));
		
		
		saidaDec = conversorBinDec(bin);
		

		*(vetorSaida + i) = saidaDec;
	}
	return vetorSaida;
}

int conversorBinDec(int bin)
{
	int dec = 0;
	int potenc = 1;

	while (bin > 0)
	{
		dec += bin % 10 * potenc;
		bin = bin / 10;
		potenc = potenc * 2;
	}

	return dec;
}

void histogramaVetor(struct pgm *pio, int *vetorSaida, char primeiro){
    int total =  pio->c * pio->r;
    int *histograma;
	histograma = malloc( pio->mv * sizeof(int));
	
	for(int i = 0; i <= pio->mv; i++) {
		*(histograma + i) = 0;
	}
	for(int i = 0; i <= pio->mv; i++) {
		for(int j = 0; j <= total; j++){
			if(i == *(vetorSaida + j)) {
				*(histograma + i) += 1;
			}
		}
	}
	
    FILE *file;
    file = fopen("HistogramaFinal.csv","a");
    
    if(file == NULL){
        printf("Ocorreu um erro na aberura do arquivo.");
        exit(1);
    }
	for(int i = 0; i < 256; i++){
		fprintf(file, "%d,", *(histograma + i));
	}
	char aux = '0';
	if(primeiro == aux){
		fprintf(file, "0");
	}
	else{
		fprintf(file, "1");
	}
	fprintf(file, "\n");
    
	free(histograma);
    fclose(file);
}
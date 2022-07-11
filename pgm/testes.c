/* Ambiente de testes controlados com uma matriz propria */
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

void PercorrerVetor(struct pgm *);
int main () {
    struct pgm pio;
    pio.c = 10;
    pio.r = 10;
    
    for(int i = 0; i < pio.c * pio.r; i++) {
        *(pio.pData + i) = i;
        
    }
    for (int k = 0; k < pio.c * pio.r; k++) //(pio->r * pio->c)
	{
		if (!(k % pio.c)){
            printf("\n");
        }
		printf("[ %d ]", *(pio.pData + k));
	}
	printf("\n");
    PercorrerVetor(&pio);
}
void PercorrerVetor(struct pgm *pio){
	int i, j = 0;
	int v[3][3] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	for (i = 19; i < 20; i++) // A linha mais importante -> original (i = 0; i < pio->c * pio->r; i++)
	{
		//Canto superior esquerdo - Caso 1 - Funcionando corretamente (10/07)
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
        
		//Centro primeira linha - Caso 2 - Funcionando corretamente (10/07)
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

		//Canto superior direito - Caso 3 - Funcionando corretamente (10/07)
		if((pio->pData + i) == (pio->pData + pio->c - 1)){
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
        

		//Coluna centro esquerda - Caso 4 - Funcionando corretamente (10/07)
		for(int k = 1; k < pio->r ; k++) {
			if(((pio->pData + i) == (pio->pData +(pio->c * k) )) && (pio->pData + i != pio->pData)) { //((pio->pData + pio->c-1 + (pio->c * k))
				v[0][0] = 0; // zera os tres de esquerda
				v[1][0] = 0;
				v[2][0] = 0;
				
                
				v[0][1] = *(pio->pData - pio->c + i);
				v[0][2] = *(pio->pData - pio->c + i + 1); 

				v[1][1] = *(pio->pData + i);  // preenche os valores
			
				v[1][2] = *(pio->pData + i + 1);
				v[2][1] = *(pio->pData + pio->c + i);
				v[2][2] = *(pio->pData + i + pio->c + 1 );
                printf("CASO 4\n");
				break;
			}
		}
        
		// Centro - Caso 5
		
		
		//Coluna centro direita - Caso 6 - Funcionando corretamente (10/07)
		for(int k = 1; k < pio->r; k++) {
			if(((pio->pData + i) == (pio->pData + k * pio->c - 1 )) && (pio->pData + i) != (pio->pData + pio->c - 1) && (pio->pData + i) != (pio->pData + (pio->c * pio->r) - 1)) { //((pio->pData + pio->c-1 + (pio->c * k))
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
        //caso 5:
        for(int k = 1; k < pio->r; k++) {
			if(((pio->pData + i) == (pio->pData + k * pio->c )) && (pio->pData + i != pio->pData + pio->c)) {
				k += 2;
			}
			else if((pio->pData + i) > (pio->pData + pio->c) && (pio->pData + i) < (pio->pData +(pio->c *(pio->r - 1)) - 1)){
				v[0][0] = *(pio->pData - pio->c + i - 1);
				v[1][0] = *(pio->pData + i - 1);
				v[2][0] = *(pio->pData + pio->c - 1 + i);
				v[0][1] = *(pio->pData - pio->c + i );
				v[0][2] = *(pio->pData - pio->c + i  + 1);
				v[1][1] = *(pio->pData + i);
				v[1][2] = *(pio->pData + i + 1 );
				v[2][1] = *(pio->pData + pio->c + i);
				v[2][2] = *(pio->pData + i + pio->c + 1);
                printf("CASO 5\n");
			}
            
		}
        
		//Canto inferior esquerdo - Caso 7 - Funcionando corretamente (10/07)
		if((pio->pData + i) == (pio->pData + (pio->c * (pio->r - 1)))){
			v[2][0] = 0; // zera os tres de baixo
			v[2][1] = 0;
			v[2][2] = 0;

			v[0][0] = 0; //zera os dois da esquerda
			v[1][0] = 0;

			v[1][1] = *(pio->pData + i); // preenche os valores
			
			v[0][1] = *(pio->pData - pio->c + i);
			v[0][2] = *(pio->pData - pio->c + 1 + i);
			v[1][2] = *(pio->pData + i + 1);
            printf("CASO 7\n");
		}

		//Centro linha inferior - Caso 8 - Funcionando corretamente (10/07)
		if((pio->pData + i) > (pio->pData + (pio->c * (pio->r - 1))) && (pio->pData + i) < (pio->pData + (pio->c*pio->r) - 1))  {
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
		
		//Canto inferior direito - Caso 9 - Funcionando corretamente (10/07)
		if((pio->pData + i) == (pio->pData + (pio->c * pio->r) - 1)){
			v[0][2] = 0; // zera os tres da direita
			v[1][2] = 0;
			v[2][2] = 0;

			v[2][0] = 0; //zera os dois de baixo
			v[2][1] = 0;

			v[1][1] = *(pio->pData + i); // preenche os valores
			
			v[0][0] = *(pio->pData - pio->c + i - 1);
			v[0][1] = *(pio->pData - pio->c + i);
			v[1][0] = *(pio->pData + i - 1);
            printf("CASO 9\n");
		}

		printf("ATUAL: %d\n", *(pio->pData + i));
	}

	//printf("%d\n", pio->c);
	//printf("%d", pio->r);
    
	i, j = 0;
	printf("\n Nossa matriz: \n");
	for (i = 0; i <= 2; i++)
	{ // mostra a matriz gerada
		for (j = 0; j <= 2; j++)
		{
			printf("[  %d  ]", v[i][j]);
		}
		printf("\n");
	}
}
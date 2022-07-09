#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int histogramaVetor(int vetorDec[], int c, int r){
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

int escreverArquivo(){
    FILE *file;
    file = fopen("SaidaFinal.txt","w");//Ver como os dados devem ser gravados pq se for abrir o arquivo para cada linha devemos trocar o "w" por "a" para que ele nao sobreescreva e sim adicione.
    
    if(file == NULL){
        printf("Ocorreu um erro na aberura do arquivo.");
        exit(1);
    }

    fprintf(file, " \n"); //Aqui, entre "" sera passado as linhas que devem ser salvas no arquivo.
    fclose(file);
}
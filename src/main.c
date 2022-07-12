#include "functions.h"

int main(int argc, char *argv[]){
    struct pgm img;

	if (argc != 3)
	{
		printf("Formato: \n\t %s <imagemEntrada.pgm> <imagemSaida.pgm>\n", argv[0]);
		exit(1);
	}

    
    return 0;
}
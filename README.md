
# Algoritmo LBP (*Local Binary Patterns*)
## Projeto Final - Laboratório de Programação

### Sobre LBP:
O Local Binary Pattern é um algoritmo utilizado para auxiliar processos de visão computacional e aprendizado de máquina. Ele faz isso por meio da classificação de texturas em imagens ao extrair informações uteis contidas nas mesmas.

### Como o algorítimo funciona:
**1 . Entrada:** O algorítmo receberá uma ou diversas imagens para serem analisádas.
**2 . Processamento:** Os dados da matriz serão percorridos pelo algoritmo de filtragem que, em uma matriz 3x3, compara o central com os 8 ao redor. A partir disso ele binariza os valores transformando os números menores que o central em 0 (zero) e os maiores ou iguais em 1 (um). Após isso esses valores são pegos em sentido horário a partir do superior esquerdo e são transformados em um valor decimal que será colocado em um vetor.
**3 . Saída:** Um arquivo contendo um histograma em que cada linha representa uma imagem e cada dado a frequência com que o valor naquele índice apareceu. Sendo de 0 a 255, valor mínimo e máximo.

### Conhecimentos utilizados:
- Alocação dinâmica.
- Ponteiros.
- Matrizes e vetores.
- Estruturas.
- Funções.
- Criação, leitura e escrita de arquivos.

### Sobre o Trabalho Final:
Projeto final Cadeira Laboratório de Programação (2º Semestre)
Curso Bacharelado em Ciência da Computação - IFCE Campus Maracanaú.
Projeto realizado utilizando a linguagem C.
Professor da disciplina: Daniel Ferreira.

### Equipe:
[Yohans Nascimento](https://github.com/YohansN)
[Felipe Borges](https://github.com/felipebsoares)
[Julia Morales](https://github.com/juliamoraless)
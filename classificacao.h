//Header das funções classificadoras do KNN
#ifndef __CLASSIFICACAO_H
#define __CLASSIFICACAO_H
int** matrizConfusao( float *rotulosAvaliados, float *rotulosTeste, int numRotulos, int qtdAmostras );
void bubbleSortDouble( float **result, float **rot, int tam );
float KNN( int k , float *rotulos );
float acuracia( float *rotulos, float **matTeste, int tamTeste, int posRotulo );
#endif
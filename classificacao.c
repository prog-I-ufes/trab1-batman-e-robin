#include <stdio.h>
#include <stdlib.h>
#include "vetmanip.h"
#include "classificacao.h"

void bubbleSortDouble( float **result, float **rot, int tam)  //Ordena um vetor de forma crescente e traz para sua mesma posiçao o valor em outro vetor
{
    int i, j;
    float auxResult, auxRot;

    for ( j = tam ; j > 0 ; j-- )
    {
        for( i = 0 ; i < j ; i++ )
        {
            if ( (*result)[i] > (*result)[i+1] )
            {
                auxResult = (*result)[i];
                auxRot = (*rot)[i];
                (*result)[i] = (*result)[i+1];
                (*rot)[i] = (*rot)[i+1];
                (*result)[i+1] = auxResult;
                (*rot)[i+1] = auxRot;
            }
        }
    }
    /*
    Com a intuição do método de ordenação Bubble Sort, esta função funciona de modo bem similar, ao receber um vetor principal, ela ordena seus valores
    do menor para o maior(assim como o método original), porém, o vetor secundario recebido tem suas posições trocadas de acordo com o primeiro. Por exemplo,
    se um vetor [3,2,1] foi ordenado para [1,2,3], seu vetor dependente (secundario) [5,7,9] será ordenado para [9,7,5].
    */
}

int** matrizConfusao( float *rotulosAvaliados, float *rotulosTeste, int numRotulos, int qtdAmostras ) //Calcula a matriz de confusao
{
    int i, j, auxM, auxN, **matrizConfusao;

    matrizConfusao = initMatI( numRotulos, numRotulos );

    for ( i = 0 ; i < numRotulos ; i++ )
    {
        for ( j = 0 ; j < numRotulos ; j++ )
        {
            matrizConfusao[i][j] = 0;
        }
    }

    for ( i = 0 ; i < qtdAmostras ; i++ )
    {   
        auxM = rotulosTeste[i] - 1;
        auxN = rotulosAvaliados[i] - 1;
        matrizConfusao[auxM][auxN]++;
    }

    return matrizConfusao;
    /*
    A matriz é calculada da seguinte forma: 
    A função aloca uma matriz m x m, sendo m a quantidade de rotulos existentes no teste. Em seguida zera todos os valores dela.
    E em um laço que tem duração igual a quantidade de rotulos de teste, ela incrementa o valor da matriz na posição (m,n), sendo m o rotulo
    predito correto e n o rotulo classificado pelo programa.
    */
}

float KNN( int k , float *rotulos ) //Define o rótulo predominante entre os K primeiros rótulos
{
    int i, j, vezesAparece = 0 , vezesApareceAux = 0;
    float rotuloAtual = rotulos[0] , rotuloPredominante = rotulos[0] ;

    for ( i = 0 ; i < k ; i++ )
    {
        for ( j = 0 ; j  < k ; j++ )
        {   
            if ( rotulos[i] == rotulos[j] ) 
            { 
                vezesAparece++;
            }
        }
        
        if ( vezesAparece > vezesApareceAux ) { vezesApareceAux = vezesAparece; rotuloPredominante = rotulos[i]; }
        vezesAparece = 0;
    }

    return rotuloPredominante;
}


float acuracia( float *rotulos, float **matTeste, int tamTeste, int posRotulo ) //Calcula a acuracia, R = A/T
{
    int i;
    float acertos = 0, resultado;

    for ( i = 0 ; i < tamTeste ; i++ )
    {
        if ( rotulos[i] == matTeste[i][posRotulo-1] ) { acertos++; }
    }

    resultado = acertos/tamTeste;

    return resultado;
}
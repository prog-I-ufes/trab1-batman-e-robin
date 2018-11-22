#include <stdlib.h>
#include <stdio.h>
#include "vetmanip.h"

char* stringAlloc( int tam ) //Aloca uma string com o tamanho determinado
{
    char *string;

    string = (char *) malloc( tam * sizeof(char) );
    return string;
}

int sizeInString( int num ) //Verifica o tamanho que um numero ocuparia em forma de string ( cada digito ocupa uma posição num vetor de string )
{
    if ( num >= -9 && num <= 9 ) { return 1; }
    else if ( num >= -99 && num <= -10 || num >= 10 && num <= 99 ) { return 2; }
    else if ( num >= -999 && num <= -100 || num >= 100 && num <= 999 ) { return 3; }
    else if ( num >= -9999 && num <= -1000 || num >= 1000 && num <= 9999 ) { return 4; }
    else if ( num >= -99999 && num <= -10000 || num >= 10000 && num <= 99999 ) { return 5; }
    //Só mede até tamanho 5 msm
}

int** initMatI( int m, int n ) //Inicializa uma matriz m x n do tipo Int
{
    int i, **matriz;

    matriz = (int **) malloc( m * sizeof(int *) );
    for ( i = 0; i < m ; i++ )
    {
        matriz[i] = (int *) malloc( n * sizeof(int) );
    }
    
    return matriz;
}

float** initMatF( int m, int n ) //Inicializa uma matriz m x n do tipo Float
{
    int i;
    float **matriz;

    matriz = (float **) malloc( m * sizeof(float *) );
    for ( i = 0; i < m ; i++ )
    {
        matriz[i] = (float *) malloc( n * sizeof(float) );
    }
    
    return matriz;
}

float maiorNum( float *vetor, int tam ) //Verifica o maior numero de um vetor float ( no caso usei para verificar a quantidade de rotulos )
{   
    int i;
    float maior;

    for ( i = 0 ; i < tam ; i++ )
    {
        if ( i == 0 ) { maior = vetor[i]; }
        else if ( vetor[i] > maior ) { maior = vetor[i]; }
    }

    return maior;
}

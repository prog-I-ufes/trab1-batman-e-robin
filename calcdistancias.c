#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calcdistancias.h"

float distEuclideana( float* vetor1, float* vetor2, int tamanho )
{
    int i;
    float distancia, somatorio = 0;

    for ( i = 0 ; i < tamanho ; i++ )
    {
      somatorio = somatorio + pow ( (vetor2[i] - vetor1[i]), 2) ;
    }
    distancia = sqrt (somatorio);

    return distancia;
}

float distMinkowsky( float* vetor1, float* vetor2, float r, int tamanho )
{
    int i;
    float distancia, somatorio = 0;

    for ( i = 0 ; i < tamanho ; i++ )
    {
        somatorio = somatorio + pow ( ( fabs( vetor1[i] - vetor2[i] ) ), r );
    }

    distancia = pow ( somatorio, 1/r );

    return distancia;
}

float distChebychev( float* vetor1, float* vetor2, int tamanho )
{
    int i;
    float distancia = 0, somatorio = 0, *diferencas;

    diferencas = (float *) malloc( tamanho * sizeof(float));  
    /*
    Vetor auxiliar que armazenará todas as diferencas entre os pontos dos vetores
    Ex: distancia entre (x1,y1) e (x2,y2)
        diferenca[0] = x1 - x2
        diferenca[1] = y1 - y2
    */
    for ( i = 0 ; i < tamanho ; i++ )
    {
        diferencas[i] = fabs(vetor2[i] - vetor1[i]);
        if ( diferencas[i] > distancia ) { distancia = diferencas[i]; }
    }
    
    free(diferencas);

    return distancia;
}

float selecDist( float* vetor1, float* vetor2, int tamanho, float r, char tipoDist )//Função usada apenas para selecionar qual distancia será usada
{
    float resultado;

    switch( tipoDist )
    {
        case 'E': resultado = distEuclideana( vetor1, vetor2, tamanho );
                  break;
        case 'M': resultado = distMinkowsky( vetor1, vetor2, r, tamanho );
                  break;
        case 'C': resultado = distChebychev( vetor1, vetor2, tamanho);
                  break;
    }
    
    return resultado;
}
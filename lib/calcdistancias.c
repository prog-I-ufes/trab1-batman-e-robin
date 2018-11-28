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
    float distancia, somatorio = 0, aux;

    for ( i = 0 ; i < tamanho ; i++ )
    {   
        aux = vetor1[i] - vetor2[i];
        if ( aux < 0 ) { aux = aux * (-1); }
        somatorio = somatorio + pow ( aux , r );
    }

    distancia = pow ( somatorio, 1/r );

    return distancia;
}

float distChebychev( float* vetor1, float* vetor2, int tamanho )
{
    int i;
    float distancia = 0, somatorio = 0, diferenca;

    /*
    Vetor auxiliar que armazenará todas as diferencas entre os pontos dos vetores
    Ex: distancia entre (x1,y1) e (x2,y2)
        diferenca[0] = x1 - x2
        diferenca[1] = y1 - y2
    */
    for ( i = 0 ; i < tamanho ; i++ )
    {
        diferenca = vetor2[i] - vetor1[i];
        if ( diferenca < 0 ) { diferenca = diferenca * (-1); }
        if ( diferenca > distancia ) { distancia = diferenca; }
    }



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
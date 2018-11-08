#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float distMinkowsky( int* vetor1, int* vetor2, float r, int tamanho )
{
    int i;
    float distancia, somatorio = 0;

    for ( i = 0 ; i < tamanho ; i++ )
    {
        somatorio = somatorio + pow ( ( abs( vetor1[i] - vetor2[i] ) ), r );
    }

    distancia = pow ( somatorio, 1/r );

    return distancia;
}


int main()
{
    int i, tamVet, *vet1, *vet2;
    float r, distanciaM;

    printf("Insira o tamanho dos vetores: ");
    scanf (" %d", &tamVet);
    printf("Insira o valor de r: ");
    scanf (" %f", &r);

    vet1 = (int *) malloc( tamVet * sizeof(int));
    vet2 = (int *) malloc( tamVet * sizeof(int));

    printf("Insira os valores do primeiro vetor:");
    for ( i = 0 ; i < tamVet ; i++ )
    {
      scanf(" %d", &vet1[i]);
    }

    printf("Insira os valores do segundo vetor:");
    for ( i = 0 ; i < tamVet ; i++ )
    {
      scanf(" %d", &vet2[i]);
    }

    distanciaM = distMinkowsky( vet1, vet2, r, tamVet);
    printf ("Distancia: %.2f\n", distanciaM);

    return 0;
}
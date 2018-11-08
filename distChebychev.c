#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float distChebychev( int* vetor1, int* vetor2, int tamanho )
{
    int i;
    float distancia, somatorio = 0;

    for ( i = 0 ; i < tamanho ; i++ )
    {
        
    }

    return distancia;
}

int main()
{
    int i, tamVet, *vet1, *vet2;
    float distanciaC;

    printf("Insira o tamanho dos vetores: ");
    scanf (" %d", &tamVet);

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

    distanciaC = distChebychev( vet1, vet2, tamVet);
    printf ("Distancia: %.2f\n", distanciaC);

    return 0;
}
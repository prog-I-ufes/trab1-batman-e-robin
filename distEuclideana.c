#include <stdio.h>
#include <stdlib.h>
#include <math.h>


float distEuclideana( int* vetor1, int* vetor2, int tamanho )
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

int main()  //Apenas para testar a função, sera removido depois
{
  //Testando apenas as distancias
    int i, tamVet, *vet1, *vet2;
    float distanciaE;

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

    distanciaE = distEuclideana( vet1, vet2, tamVet );

    printf("Distancia: %.2f\n", distanciaE);

    return 0;
}
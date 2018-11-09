#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float distChebychev( int* vetor1, int* vetor2, int tamanho )
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
        diferencas[i] = abs(vetor2[i] - vetor1[i]);
        if ( diferencas[i] > distancia ) { distancia = diferencas[i]; }
    }

    return distancia;
}

int main() //Apenas para testar a função, sera removido depois
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
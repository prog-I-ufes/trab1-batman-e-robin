//Header das funções de distância
#ifndef __CALCDISTANCIAS_H
#define __CALCDISTANCIAS_H
float distEuclideana( float* vetor1, float* vetor2, int tamanho );
float distMinkowsky( float* vetor1, float* vetor2, float r, int tamanho );
float distChebychev( float* vetor1, float* vetor2, int tamanho );
float selecDist( float* vetor1, float* vetor2, int tamanho, float r, char tipoDist );
#endif
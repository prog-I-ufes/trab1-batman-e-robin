#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
Código de erros
Informa em qual função o erro ocorreu e o tipo do erro

RR - readPath Null   || FL - fileLines Null || LF - loadFeatures Null ||  FF - fileFeatures NULL
RP - readParam Null  ||                     ||                        || 
                     ||                     ||                        || 
                     ||                     ||                        ||                                           
*/

//====================[Cabeçalho das Funções]======================================================================
int fileFeatures( char *nome, int linha );
int fileLines( char *nome );
float** loadFeatures( char *pathArq, int *linhas, int *features );
void readPath( char **pTreino, char **pTeste, char **pSaida );
void readParam( int **k, char **tDist, float **r );
char* stringAlloc( int tam );
float** initMatF( int m, int n );
float distEuclideana( int* vetor1, int* vetor2, int tamanho );
float distMinkowsky( int* vetor1, int* vetor2, float r, int tamanho );
float distChebychev( int* vetor1, int* vetor2, int tamanho );
//====================[Cabeçalho das Funções]======================================================================


//====================[Funçoes de Arquivo]============================================================================
int fileFeatures( char *nome, int linha ) //Calcula a quantidade de features da linha informada 
{
    int features = 0, linhaAtual = 1;
    char caractere;
    FILE *arq;

    arq = fopen(nome, "r");
    if ( arq == NULL ) { printf("Erro ao abrir arquivo(FF). Encerrando programa...\n"); exit(1); }

    while( !feof(arq) )
    {
        caractere = fgetc(arq);
        if ( linhaAtual == linha ) { 
                                        if ( caractere == ',' ) { features++; } 
                                        else if ( caractere == '\n' ) { fclose(arq); return features; }
                                   }
        else if ( caractere == '\n' ) { linhaAtual++; }
    }

    fclose(arq);
    return 0;
}

int fileLines( char *nome ) 
{
    int linhas = 0;
    char caractere;
    FILE *arq;

    arq = fopen(nome, "r");
    if ( arq == NULL ) { printf("Erro ao abrir arquivo(FL). Encerrando programa...\n"); exit(1); }

    while( !feof(arq) )
    {
        caractere = fgetc(arq);
        if ( caractere == '\n' ) { linhas++; }
    }

    fclose(arq);
    return linhas;
}

float** loadFeatures( char *pathArq, int *linhas, int *features )
{   
    int i, j;
    float **mat;
    char aux = 'a';
    FILE *arquivo;

    arquivo = fopen( pathArq, "r");
    if ( arquivo == NULL ) { printf("Erro ao abrir arquivo(LF). Encerrando programa...\n"); exit(1); }

    *linhas = fileLines( pathArq );
    *features = fileFeatures( pathArq, *linhas );
    
    mat = initMatF( *linhas, *features );
    for( i = 0 ; i < *linhas ; i++ )
    {
        for( j = 0 ; j < *features ; j++ )
        {
            fscanf(arquivo, "%f", &mat[i][j]);
            aux = fgetc(arquivo);
        }
    }
    
    fclose(arquivo);
    
    return mat;
}

void readPath( char **pTreino, char **pTeste, char **pSaida )
{   
    int contador = 1, tamanho;
    char *stringPath, strAux[100];
    FILE *config;

    config = fopen("config.txt", "r");
    if( config == NULL ) { printf("Erro ao abrir arquivo(RP). Encerrando programa...\n"); exit(1); }
    
    do
    {
        fgets(strAux, 99, config);
        tamanho = strlen(strAux);
        strAux[tamanho-1] = '\0';
        
        switch(contador)
        {
            case 1: *pTreino = stringAlloc(tamanho);
                    strcpy(*pTreino, strAux);
                    break;

            case 2: *pTeste = stringAlloc(tamanho);
                    strcpy(*pTeste, strAux);
                    break;

            case 3: *pSaida = stringAlloc(tamanho);
                    strcpy(*pSaida, strAux);
                    break;
        }

        contador++;
    } while( contador < 4);

    fclose(config);
}

void readParam( int **k, char **tDist, float **r )// pq **??
{
    int linhas, contador = 0, posicao = 0, bn = 0, i;
    char caractere;
    FILE *config;

    config = fopen("config.txt", "r");
    if ( config == NULL ) { printf("Erro ao abrir arquivo(RP). Encerrando programa...\n"); exit(1); }
    linhas = fileLines( "config.txt");

    *k = (int *) malloc( (linhas - 3) * sizeof(int));
    *tDist = (char *) malloc( (linhas - 3) * sizeof(char));
    *r = (float *) malloc( (linhas - 3) * sizeof(float));


    while( !feof(config) )
    {
        switch(contador)
        {
            case 0: do
                    {   
                        caractere = fgetc(config);
                        if ( caractere == '\n' ) { bn++; }
                    } while(bn < 3);
                    contador = 1;
                    break;
                    
            case 1: fscanf(config, "%d", &((*k)[posicao]));
                    contador++;
                    break;
            
            case 4: fscanf(config, "%c", &((*tDist)[posicao]));
                    if ( (*tDist)[posicao] != 'M' ) { (*r)[posicao] = 0; }
                    contador++;
                    break;
            
            case 7: fscanf(config, "%f", &((*r)[posicao]));
                    contador++;
                    break;

            default: caractere = fgetc(config);
                     if ( caractere == '\n' ) { contador = 1; posicao++; }
                     else { contador++; }

                     break;
        }
    }
    
    
    fclose(config);
}
//====================[Funçoes de Arquivo]==============================================================================


//====================[Funçoes de Vetor/Matriz]=========================================================================
char* stringAlloc( int tam )
{
    char *string;

    string = (char *) malloc( tam * sizeof(char) );
    return string;
}



float** initMatF( int m, int n )
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

//====================[Funçoes de Vetor/Matriz]=========================================================================


//====================[Funções de Distancia]============================================================================
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
    
    free(diferencas);

    return distancia;
}
//====================[Funções de Distancia]============================================================================



int main()
{   
    int i, j, *k, linhasTreino, featuresTreino, linhasTeste, featuresTeste;
    float *r, **matTreino, **matTeste;
    char *pathTreino, *pathTeste, *pathSaida, *tDist;
    FILE *configTxt;

    printf("Obtendo parametros de configuracao -> ");
    readPath( &pathTreino, &pathTeste, &pathSaida );
    readParam( &k, &tDist, &r );
    printf("OK\n");

    printf("Obtendo parametros de treino -> ");
    matTreino = loadFeatures( pathTreino, &linhasTreino, &featuresTreino );
    printf("OK\n");
    
    printf("Obtendo parametros de teste -> ");
    matTeste = loadFeatures( pathTeste, &linhasTeste, &featuresTeste );
    printf("OK\n");



    for( i = 0 ; i < linhasTreino ; i++ )
    {   
        free(matTreino[i]);
    }
    for( i = 0 ; i < linhasTeste ; i++ )
    {   
        free(matTeste[i]);
    }
    free(matTeste);
    free(matTreino);
    free(k);
    free(tDist);
    free(r);
    free(pathTreino);
    free(pathTeste);
    free(pathSaida);
    
    return 0;
}

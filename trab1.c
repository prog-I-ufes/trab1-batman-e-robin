#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Lista de erros (Orientação do codigo):
RC - readConfig Null || FL - fileLines Null || RP - readPath Null ||  FF - fileFeatures NULL
                     ||                     ||                    || 
                     ||                     ||                    || 
                     ||                     ||                    ||                                           
*/
char* stringAlloc( int tam ) //Função vetor/matriz
{
    char *string;

    string = (char *) malloc( tam * sizeof(char) );
    return string;
}

int fileFeatures( FILE *arq, char *nome, int linha ) //Calcula a quantidade de features da linha informada //Função Arquivo
{
    int features = 0, linhaAtual = 1;
    char caractere;

    arq = fopen(nome, "r");
    if ( arq == NULL ) { printf("Erro ao abrir arquivo(FF). Encerrando programa..."); exit(1); }

    while( !feof(arq) )
    {
        caractere = fgetc(arq);
        if ( linhaAtual == linha ) { 
                                        if ( caractere == ',' ) { features++; } 
                                        else if ( caractere == '\n' ) { return features; }
                                   }
        else if ( caractere == '\n' ) { linhaAtual++; }
    }

    return 0;
}

int fileLines( FILE *arq, char *nome )  //Função Arquivo
{
    int linhas = 0;
    char caractere;

    arq = fopen(nome, "r");
    if ( arq == NULL ) { printf("Erro ao abrir arquivo(FL). Encerrando programa..."); exit(1); }

    while( !feof(arq) )
    {
        caractere = fgetc(arq);
        if ( caractere == '\n' ) { linhas++; }
    }

    fclose(arq);
    return linhas;
}

void readConfig( FILE *config_txt, char *path_treino, char *path_teste, char *path_saida, int *k, char *tDist, float *r )   //Função Arquivo
{
    // Função que le o arquivo txt e armazena todas as informaçoes em ponteiros
    int tamLinha, i, linhas, contador, posicao;
    char caractere;

    config_txt = fopen("config.txt", "r");
    if ( config_txt == NULL ) 
    {
        printf("Erro ao abrir arquivo(RC). Encerrando programa...\n");
        exit(1);
    }
    
    
    fgets(path_treino, 99, config_txt);
    tamLinha = strlen(path_treino);
    path_treino = realloc( path_treino, (tamLinha) * sizeof(char) ); //"+1" por causa do /0
    path_treino[tamLinha - 1] = '\0';

    fgets(path_teste, 99, config_txt);
    tamLinha = strlen(path_teste);
    path_teste = realloc( path_teste, (tamLinha) * sizeof(char) );
    path_teste[tamLinha - 1] = '\0';

    fgets(path_saida, 99, config_txt);
    tamLinha = strlen(path_saida);
    path_saida = realloc( path_saida, (tamLinha) * sizeof(char) );
    path_saida[tamLinha - 1] = '\0';

    linhas = fileLines( config_txt, "config.txt" );

    k = realloc( k, (linhas - 3) * sizeof(int) );
    tDist = realloc( tDist, (linhas - 3) * sizeof(char) );
    r = realloc( r, (linhas - 3) * sizeof(float) );

    contador = 1;
    posicao = 0;

    while( !feof(config_txt) ) //Lendo os parametros de K, distancia e r. (gambiarra?)
    {
        switch(contador)
        {
            case 1: fscanf(config_txt, "%d", &k[posicao]);
                    contador++; 
                    break;

            case 4: fscanf(config_txt, "%c", &tDist[posicao]);
                    if ( tDist[posicao] != 'M' ) { r[posicao] = 0;}
                    contador++;
                    break;

            case 7: fscanf(config_txt, "%f", &r[posicao]);
                    contador++;
                    break;

            default: caractere = fgetc(config_txt);
                     if( caractere == '\n') { contador = 1; posicao++; }
                     else { contador++; }
                     break;
        }
    }
    
    fclose(config_txt);
}

float** initMatF( int m, int n )    //Função vetor/matriz
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

float** readPath( char *pathArq, int *linhas, int *features )   //Função Arquivo
{
    int i, j;
    float **mat;
    char aux = 'a';
    FILE *arquivo;

    arquivo = fopen(pathArq, "r");
    if ( arquivo == NULL ) { printf("Erro ao abrir arquivo(RP). Encerrando programa..."); exit(1); }

    *linhas = fileLines( arquivo, pathArq );
    *features = fileFeatures( arquivo, pathArq, 1 );
    mat = initMatF( *linhas, *features );

    for ( i = 0 ; i < *linhas ; i++ )
    {
        for ( j = 0 ; j <= *features ; j++)
        {   
            fscanf(arquivo, "%f", &mat[i][j]);
            aux = fgetc(arquivo);
        }
    }

    fclose(arquivo);

    return mat;
}

int main()
{   
    int i, j, *k, linhasTreino, featuresTreino;
    float *r, **matTreino;
    char *pathTreino, *pathTeste, *pathSaida, *tDist;
    FILE *configTxt;

    pathTreino = stringAlloc(100); //Aloca um tamanho 100 para as strings depois realoca apenas para o tamanho necessario(isso nao faz o menor sentido)
    pathTeste = stringAlloc(100);
    pathSaida = stringAlloc(100);
    k = (int *) malloc( sizeof(int) );
    tDist = (char *) malloc( sizeof(char) );
    r = (float *) malloc( sizeof(float) );

    printf("Obtendo parametros de configuracao -> ");
    readConfig( configTxt, pathTreino, pathTeste, pathSaida, k, tDist, r );
    printf("OK\n");

    /*printf("=====Main========\n"); //Rm
    printf("%d %c %.1f\n", k[0], tDist[0], r[0]); //<<<<<<<<Nao ta printando o valor obtido na função
    printf("=====Main========\n"); //Rm
    printf("%s\n", pathTreino);    //Rm*/

    printf("Obtendo parametros de treino -> ");
    matTreino = readPath( pathTreino, &linhasTreino, &featuresTreino ); 
    printf("OK\n");

    
    for ( i = 0 ; i < linhasTreino -1 ; i++ )
    {
        free(matTreino[i]);
    }
    free(matTreino);
    free(pathTreino);
    free(pathTeste);
    free(pathSaida);
    free(k);
    free(tDist);
    free(r);

    return 0;
}
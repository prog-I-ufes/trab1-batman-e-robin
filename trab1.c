#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Lista de erros (Orientação do codigo):
R1 - readConfig Null || F1 - fileLines Null || P1 - readPath Null ||  
                     ||                     ||                    || 
                     ||                     ||                    || 
                     ||                     ||                    ||                                           
*/
char* stringAlloc( int tam )
{
    char *string;

    string = (char *) malloc( tam * sizeof(char) );
    return string;
}

int fileLines( FILE *arq, char *nome )
{
    int linhas = 0;
    char caractere;

    arq = fopen(nome, "r");
    if ( arq == NULL ) { printf("Erro ao abrir arquivo(F1). Encerrando programa..."); exit(1); }

    while( !feof(arq) )
    {
        caractere = fgetc(arq);
        if ( caractere == '\n' ) { linhas++; }
    }

    fclose(arq);
    return linhas;
}

void readConfig( FILE *config_txt, char *path_treino, char *path_teste, char *path_saida, int *k, char *tDist, float *r )
{
    // Função que le o arquivo txt e armazena todas as informaçoes em ponteiros
    int tamLinha, i, linhas, contador, posicao;
    char caractere;

    config_txt = fopen("config.txt", "r");
    if ( config_txt == NULL ) 
    {
        printf("Erro ao abrir arquivo(R1). Encerrando programa...\n");
        exit(1);
    }
    
    
    fgets(path_treino, 99, config_txt);
    tamLinha = strlen(path_treino);
    path_treino = realloc( path_treino, (tamLinha + 1) * sizeof(char) ); //"+1" por causa do /0

    fgets(path_teste, 99, config_txt);
    tamLinha = strlen(path_teste);
    path_teste = realloc( path_teste, (tamLinha + 1) * sizeof(char) );

    fgets(path_saida, 99, config_txt);
    tamLinha = strlen(path_saida);
    path_saida = realloc( path_saida, (tamLinha + 1) * sizeof(char) );

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

    printf("=====Funcao========\n");
    printf("%d %c %.1f\n", k[0], tDist[0], r[0]);
    printf("=====Funcao========\n");
}

void readPath( char *pathArq, float **mat )
{
    int linhas;
    FILE *arquivo;

    arquivo = fopen(pathArq, "r");
    if ( arquivo == NULL ) { printf("Erro ao abrir arquivo(P1). Encerrando programa..."); exit(1); }

    linhas = fileLines( arquivo, pathArq );
    printf("%d\n", linhas);

    fclose(arquivo);
}

int main()
{   
    int *k;
    float *r, **matTreino;
    char *pathTreino, *pathTeste, *pathSaida, *tDist;
    FILE *configTxt;

    pathTreino = stringAlloc(100); //Aloca um tamanho 100 para as strings depois realoca apenas para o tamanho necessario(isso nao faz o menor sentido)
    pathTeste = stringAlloc(100);
    pathSaida = stringAlloc(100);
    k = (int *) malloc( sizeof(int) );
    tDist = (char *) malloc( sizeof(char) );
    r = (float *) malloc( sizeof(float) );
    readConfig( configTxt, pathTreino, pathTeste, pathSaida, k, tDist, r );

    printf("=====Main========\n");
    printf("%d %c %.1f\n", k[0], tDist[0], r[0]); //<<<<<<<<Nao ta printando o valor obtido na função
    printf("=====Main========\n");
    printf("%s", pathTreino);

    readPath( pathTreino, matTreino ); //fgets ta lendo o \n tem q tirar isso pra dar certo

    free(pathTreino);
    free(pathTeste);
    free(pathSaida);
    free(k);
    free(tDist);
    free(r);

    return 0;
}
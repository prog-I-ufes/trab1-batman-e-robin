#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vetmanip.h"
#include "filemanip.h"

int fileFeatures( char *nome, int linha ) //Calcula a quantidade de features da linha informada 
{
    int features = 1, linhaTesteAtual = 1;
    char caractere;
    FILE *arq;

    arq = fopen(nome, "r");
    if ( arq == NULL ) { printf("Erro  ao abrir arquivo(FF). Encerrando programa...\n"); exit(1); }

    while( !feof(arq) )
    {
        caractere = fgetc(arq);
        if ( linhaTesteAtual == linha ) { 
                                        if ( caractere == ',' ) { features++; } 
                                        else if ( caractere == '\n' ) { fclose(arq); return features; }
                                   }
        else if ( caractere == '\n' ) { linhaTesteAtual++; }
    }

    fclose(arq);
    return 0;
}

int fileLines( char *nome )  //Informa quantas linhas existem no arquivo
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

int charLine( char *nomeArq, int linhaSelec ) //Informa quantos caracteres existe na linha informada do arquivo
{
    int cont = 0, linhaAtual = 1, qtdChar = 0;
    char caractere;
    FILE *arquivo;

    arquivo = fopen( nomeArq, "r");
    if ( arquivo == NULL ) { printf("Erro ao abrir arquivo(CL). Encerrando programa...\n"); exit(1); }

    while( !feof(arquivo) )
    {   
        caractere = fgetc(arquivo);
        if ( linhaAtual != linhaSelec )
        {
            if ( caractere == '\n') { linhaAtual++; }
        }
        else if ( caractere != '\n' ) { qtdChar++; }
             else { qtdChar++; break; }
    }

    fclose(arquivo);
    return qtdChar;
}

float** loadFeatures( char *pathArq, int *linhas, int *features ) //Carrega todos os features e rotulos em uma matriz alocada dinamicamente de tamanho ( linhas x colunas )
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

void readPath( char **pTreino, char **pTeste, char **pSaida ) //Le o caminho onde está localizado a base de teste, a base de treino e onde será escrito a saida
{   
    int contador = 1, tamanho, tamString;
    char *strAux;
    FILE *config;

    config = fopen("config.txt", "r");
    if( config == NULL ) { printf("Erro ao abrir arquivo(RP). Encerrando programa...\n"); exit(1); }
    
    do
    {
        tamanho = charLine("config.txt", contador);

        strAux = stringAlloc(tamanho+3);
        fgets(strAux, tamanho+1, config);
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

        free(strAux);
        contador++;
    } while( contador < 4);

    fclose(config);
}

void readParam( int **k, char **tDist, float **r , int *execucoes ) //Le e armazena os parametros de execução do config.txt, K vizinhos, tipo de distancia e R se ouver
{
    int linhas, contador = 0, posicao = 0, bn = 0, i;
    char caractere;
    FILE *config;

    config = fopen("config.txt", "r");
    if ( config == NULL ) { printf("Erro ao abrir arquivo(RP). Encerrando programa...\n"); exit(1); }
    linhas = fileLines( "config.txt");
    *execucoes = linhas - 3;

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

void writeFile( int numExe, float *rotulos, int qtdRotulos, char *pathEscrita, float acuracia, int **matConfus, int numRotulos ) //Escreve os resultados finais no arquivo predicao.txt
{   
    int i, j, tamanhoNumero, tamanhoPath;
    char *caminho;
    FILE *predicao;

    tamanhoNumero = sizeInString( numExe + 1);
    tamanhoPath = strlen(pathEscrita);
    
    caminho = (char *) malloc( (tamanhoNumero + tamanhoPath + 14) * sizeof(char) );
    sprintf(caminho, "%spredicao_%d.txt", pathEscrita, (numExe + 1) );

    predicao = fopen(caminho, "w");

    fprintf(predicao, "%.2f\n\n", acuracia);

    for ( i = 0 ; i < numRotulos ; i++ )
    {
        for ( j = 0 ; j < numRotulos ; j++ )
        {   
            if ( j == numRotulos-1 ) { fprintf(predicao, "%d\n", matConfus[i][j]); }
            else { fprintf(predicao, "%d ", matConfus[i][j]); }
        }
    }
    fprintf(predicao, "\n");

    if ( predicao == NULL ) {  printf("Erro ao abrir arquivo(WF). Encerrando programa...\n"); exit(1); }

    for( i = 0 ; i < qtdRotulos ; i++ )
    {
        fprintf(predicao, "%.0f\n", rotulos[i] - 1 );
    }

    fclose(predicao);
    free(caminho);
    /*
    Essa função recebe os parâmetros finais que deverão ser escritos no arquivo txt e os monta em uma ordem. 
    É verificado o tamanho que o "caminho/predicao_x.txt" terá, pois como o nome do caminho e o numero da predição podem variar, o tamanho
    da string a ser alocada também pode. O sprintf junta todos os valores formatados em uma unica string para poder enviar à função fopen.
    Por fim, é printado no arquivo em ordem, a acuracia, a matriz de confusão e, por ultimo, os rotulos classificados pelo programa.
    */
}
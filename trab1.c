#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    if ( arq == NULL ) { printf("Arquivo nao encontrado. Encerrando programa..."); exit(1); }

    while( !feof(arq) )
    {
        caractere = fgetc(arq);
        if ( caractere == '\n' ) { linhas++; }
    }

    fclose(arq);
    return linhas;
}

void readConfig( FILE *configTxt, char *pathTreino, char *pathTeste, char *pathSaida, int *k, char *tDist, float *r )
{
    // Função que le o arquivo txt e armazena todas as informaçoes em ponteiros
    int tamLinha, i, linhas, contador, posicao;
    char caractere;

    configTxt = fopen("config.txt", "r");
    if ( configTxt == NULL ) 
    {
        printf("Config.txt nao encontrado. Encerrando programa...\n");
        exit(1);
    }
    
    //Aloca um tamanho 100 para as strings depois realoca apenas para o tamanho necessario(isso nao faz o menor sentido)
    pathTreino = stringAlloc(100);
    fgets(pathTreino, 99, configTxt);
    tamLinha = strlen(pathTreino);
    pathTreino = realloc ( pathTreino, (tamLinha + 1) * sizeof(char) ); //"+1" por causa do /0

    pathTeste = stringAlloc(100);
    fgets(pathTeste, 99, configTxt);
    tamLinha = strlen(pathTeste);
    pathTeste = realloc ( pathTeste, (tamLinha + 1) * sizeof(char) );

    pathSaida = stringAlloc(100);
    fgets(pathSaida, 99, configTxt);
    tamLinha = strlen(pathSaida);
    pathSaida = realloc ( pathSaida, (tamLinha + 1) * sizeof(char) );

    linhas = fileLines( configTxt, "config.txt" );

    k = (int *) malloc( (linhas - 3) * sizeof(int) );
    tDist = (char *) malloc( (linhas - 3) * sizeof(char) );
    r = (float *) malloc( (linhas - 3) * sizeof(float) );

    contador = 1;
    posicao = 0;

    while( !feof(configTxt) ) //Lendo os parametros de K, distancia e r. (gambiarra?)
    {
        switch(contador)
        {
            case 1: fscanf(configTxt, "%d", &k[posicao]);
                    contador++; 
                    break;

            case 4: fscanf(configTxt, "%c", &tDist[posicao]);
                    if ( tDist[posicao] != 'M' ) { r[posicao] = 0;}
                    contador++;
                    break;

            case 7: fscanf(configTxt, "%f", &r[posicao]);
                    contador++;
                    break;

            default: caractere = fgetc(configTxt);
                     if( caractere == '\n') { contador = 1; posicao++; }
                     else { contador++; }
                     break;
        }
    }

    printf("%s", pathTreino);
    printf("%s", pathTeste);
    printf("%s", pathSaida);

    for(i = 0 ; i < linhas - 3 ; i++ )
    {
        printf("%d, %c, %.1f\n", k[i], tDist[i], r[i]);
    }

    fclose(configTxt);
}


int main()
{   
    int *k;
    float *r;
    char *pathTreino, *pathTeste, *pathSaida, *tDist;
    FILE *configTxt;

    readConfig( configTxt, pathTreino, pathTeste, pathSaida, k, tDist, r );

    free(pathTreino);
    free(pathTeste);
    free(pathSaida);
    free(k);
    free(tDist);
    free(r);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Lista de erros (Orientação do codigo):
RR - readPath Null || FL - fileLines Null || LF - loadFeatures Null ||  FF - fileFeatures NULL
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

float** loadFeatures( char *pathArq, int *linhas, int *features )   //Função Arquivo
{   
    int i, j;
    float **mat;
    char aux = 'a';
    FILE *arquivo;

    arquivo = fopen( pathArq, "r");
    printf(">%s<\n", pathArq);
    if ( arquivo == NULL ) { printf("Erro ao abrir arquivo(LF). Encerrando programa..."); exit(1); }

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

char* readPath( int linha )
{   
    int contador = 0, tamanho;
    char *stringPath, strAux[100];
    FILE *config;

    config = fopen("config.txt", "r");
    if( config == NULL ) { printf("Erro ao abrir arquivo(RP). Encerrando programa..."); exit(1); }
    
    while( contador != linha )
    {
        fgets(strAux, 99, config);
        contador++;
    }
    
    tamanho = strlen(strAux);
    strAux[tamanho-1] = '\0';
    stringPath = stringAlloc( tamanho );
    strcpy( stringPath, strAux );

    fclose(config);
    return stringPath;
}

void readParam( int *k, char *tDist, float *r )
{

}

int main()
{   
    int i, j, *k, linhasTreino, featuresTreino, linhasTeste, featuresTeste;
    float *r, **matTreino, **matTeste;
    char *pathTreino, *pathTeste, *pathSaida, *tDist;
    FILE *configTxt;

    printf("Obtendo parametros de configuracao -> ");
    pathTreino = readPath(1);
    pathTeste = readPath(2);
    pathSaida = readPath(3);

    printf("OK\n");

    printf("Obtendo parametros de treino -> ");
    printf("OK\n");
    
    printf("Obtendo parametros de teste -> ");
    printf("OK\n");

    
    
    
    
    free(pathTreino);
    free(pathTeste);
    free(pathSaida);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* stringAlloc( int tam )
{
    char *string;

    string = (char *) malloc( tam * sizeof(char) );
    return string;
}

void openConfig( FILE *configTxt, char *pathTreino, char *pathTeste, char *pathSaida  )
{
    // Função que le o arquivo txt e armazena todas as informaçoes em ponteiros
    int tamLinha, i;
    char aux[100];

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
    pathTreino = realloc ( pathTreino, (tamLinha + 1) * sizeof(char) );

    pathTeste = stringAlloc(100);
    fgets(pathTeste, 99, configTxt);
    tamLinha = strlen(pathTeste);
    pathTeste = realloc ( pathTeste, (tamLinha + 1) * sizeof(char) );

    pathSaida = stringAlloc(100);
    fgets(pathSaida, 99, configTxt);
    tamLinha = strlen(pathSaida);
    pathSaida = realloc ( pathSaida, (tamLinha + 1) * sizeof(char) );

    fclose(configTxt);
}

void closeAllocs()
{

}

int main()
{
    char *pathTreino, *pathTeste, *pathSaida;
    FILE *configTxt;

    openConfig( configTxt, pathTreino, pathTeste, pathSaida );

    free(pathTreino);
    free(pathTeste);
    free(pathSaida);

    return 0;
}
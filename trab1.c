#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
Código de erros
Informa em qual função o erro ocorreu e o tipo do erro

RR - readPath Null   || FL - fileLines Null || LF - loadFeatures Null ||  FF - fileFeatures NULL
RP - readParam Null  || WF - writeFile Null ||                        || 
                     ||                     ||                        || 
                     ||                     ||                        ||                                           
*/

//====================[Cabeçalho das Funções]======================================================================
int fileFeatures( char *nome, int linha );
int fileLines( char *nome );
int charLine( char *nomeArq, int linhaSelec );
void readPath( char **pTreino, char **pTeste, char **pSaida );
void readParam( int **k, char **tDist, float **r , int *execucoes );
void writeFile( int numExe, float *rotulos, int qtdRotulos, char *pathEscrita, float acuracia, int **matConfus, int numRotulos );
float** loadFeatures( char *pathArq, int *linhas, int *features );

int sizeInString( int num );
char* stringAlloc( int tam );
float** initMatF( int m, int n );
float maiorNum( float *vetor, int tam );

float distEuclideana( float* vetor1, float* vetor2, int tamanho );
float distMinkowsky( float* vetor1, float* vetor2, float r, int tamanho );
float distChebychev( float* vetor1, float* vetor2, int tamanho );
float selecDist( float* vetor1, float* vetor2, int tamanho, float r, char tipoDist );

int** matrizConfusao( float *rotulosAvaliados, float *rotulosTeste, int numRotulos, int qtdAmostras );
void bubbleSortDouble( float **result, float **rot, int tam );
float KNN( int k , float *rotulos );
float acuracia( float *rotulos, float **matTeste, int tamTeste, int posRotulo );
//====================[Cabeçalho das Funções]======================================================================


//====================[Funçoes de Arquivo]============================================================================
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
//====================[Funçoes de Arquivo]==============================================================================


//====================[Funçoes de Vetor/Matriz]=========================================================================
char* stringAlloc( int tam ) //Aloca uma string com o tamanho determinado
{
    char *string;

    string = (char *) malloc( tam * sizeof(char) );
    return string;
}

int** initMatI( int m, int n ) //Inicializa uma matriz m x n do tipo Int
{
    int i, **matriz;

    matriz = (int **) malloc( m * sizeof(int *) );
    for ( i = 0; i < m ; i++ )
    {
        matriz[i] = (int *) malloc( n * sizeof(int) );
    }
    
    return matriz;
}

float** initMatF( int m, int n ) //Inicializa uma matriz m x n do tipo Float
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

void bubbleSortDouble( float **result, float **rot, int tam)  //Ordena um vetor de forma crescente e traz para sua mesma posiçao o valor em outro vetor
{
    int i, j;
    float auxResult, auxRot;

    for ( j = tam ; j > 0 ; j-- )
    {
        for( i = 0 ; i < j ; i++ )
        {
            if ( (*result)[i] > (*result)[i+1] )
            {
                auxResult = (*result)[i];
                auxRot = (*rot)[i];
                (*result)[i] = (*result)[i+1];
                (*rot)[i] = (*rot)[i+1];
                (*result)[i+1] = auxResult;
                (*rot)[i+1] = auxRot;
            }
        }
    }
    /*
    Com a intuição do método de ordenação Bubble Sort, esta função funciona de modo bem similar, ao receber um vetor principal, ela ordena seus valores
    do menor para o maior(assim como o método original), porém, o vetor secundario recebido tem suas posições trocadas de acordo com o primeiro. Por exemplo,
    se um vetor [3,2,1] foi ordenado para [1,2,3], seu vetor dependente (secundario) [5,7,9] será ordenado para [9,7,5].
    */
}

int sizeInString( int num ) //Verifica o tamanho que um numero ocuparia em forma de string ( cada digito ocupa uma posição num vetor de string )
{
    if ( num >= -9 && num <= 9 ) { return 1; }
    else if ( num >= -99 && num <= -10 || num >= 10 && num <= 99 ) { return 2; }
    else if ( num >= -999 && num <= -100 || num >= 100 && num <= 999 ) { return 3; }
    else if ( num >= -9999 && num <= -1000 || num >= 1000 && num <= 9999 ) { return 4; }
    else if ( num >= -99999 && num <= -10000 || num >= 10000 && num <= 99999 ) { return 5; }
    //Só mede até tamanho 5 msm
}

float maiorNum( float *vetor, int tam ) //Verifica o maior numero de um vetor float ( no caso usei para verificar a quantidade de rotulos )
{   
    int i;
    float maior;

    for ( i = 0 ; i < tam ; i++ )
    {
        if ( i == 0 ) { maior = vetor[i]; }
        else if ( vetor[i] > maior ) { maior = vetor[i]; }
    }

    return maior;
}
//====================[Funçoes de Vetor/Matriz]=========================================================================


//====================[Funções de Distancia]============================================================================
float distEuclideana( float* vetor1, float* vetor2, int tamanho )
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

float distMinkowsky( float* vetor1, float* vetor2, float r, int tamanho )
{
    int i;
    float distancia, somatorio = 0;

    for ( i = 0 ; i < tamanho ; i++ )
    {
        somatorio = somatorio + pow ( ( fabs( vetor1[i] - vetor2[i] ) ), r );
    }

    distancia = pow ( somatorio, 1/r );

    return distancia;
}

float distChebychev( float* vetor1, float* vetor2, int tamanho )
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
        diferencas[i] = fabs(vetor2[i] - vetor1[i]);
        if ( diferencas[i] > distancia ) { distancia = diferencas[i]; }
    }
    
    free(diferencas);

    return distancia;
}

float selecDist( float* vetor1, float* vetor2, int tamanho, float r, char tipoDist )//Função usada apenas para selecionar qual distancia será usada
{
    float resultado;

    switch( tipoDist )
    {
        case 'E': resultado = distEuclideana( vetor1, vetor2, tamanho );
                  break;
        case 'M': resultado = distMinkowsky( vetor1, vetor2, r, tamanho );
                  break;
        case 'C': resultado = distChebychev( vetor1, vetor2, tamanho);
                  break;
    }
    
    return resultado;
}
//====================[Funções de Distancia]============================================================================

//====================[Classificação de Saida]==========================================================================
int** matrizConfusao( float *rotulosAvaliados, float *rotulosTeste, int numRotulos, int qtdAmostras ) //Calcula a matriz de confusao
{
    int i, j, auxM, auxN, **matrizConfusao;

    matrizConfusao = initMatI( numRotulos, numRotulos );

    for ( i = 0 ; i < numRotulos ; i++ )
    {
        for ( j = 0 ; j < numRotulos ; j++ )
        {
            matrizConfusao[i][j] = 0;
        }
    }

    for ( i = 0 ; i < qtdAmostras ; i++ )
    {   
        auxM = rotulosTeste[i] - 1;
        auxN = rotulosAvaliados[i] - 1;
        matrizConfusao[auxM][auxN]++;
    }

    return matrizConfusao;
    /*
    A matriz é calculada da seguinte forma: 
    A função aloca uma matriz m x m, sendo m a quantidade de rotulos existentes no teste. Em seguida zera todos os valores dela.
    E em um laço que tem duração igual a quantidade de rotulos de teste, ela incrementa o valor da matriz na posição (m,n), sendo m o rotulo
    predito correto e n o rotulo classificado pelo programa.
    */
}

float KNN( int k , float *rotulos ) //Define o rótulo predominante entre os K primeiros rótulos
{
    int i, j, vezesAparece = 0 , vezesApareceAux = 0;
    float rotuloAtual = rotulos[0] , rotuloPredominante = rotulos[0] ;

    for ( i = 0 ; i < k ; i++ )
    {
        for ( j = 0 ; j  < k ; j++ )
        {   
            if ( rotulos[i] == rotulos[j] ) 
            { 
                vezesAparece++;
            }
        }
        
        if ( vezesAparece > vezesApareceAux ) { vezesApareceAux = vezesAparece; rotuloPredominante = rotulos[i]; }
        vezesAparece = 0;
    }

    return rotuloPredominante;
}


float acuracia( float *rotulos, float **matTeste, int tamTeste, int posRotulo ) //Calcula a acuracia, R = A/T
{
    int i;
    float acertos = 0, resultado;

    for ( i = 0 ; i < tamTeste ; i++ )
    {
        if ( rotulos[i] == matTeste[i][posRotulo-1] ) { acertos++; }
    }

    resultado = acertos/tamTeste;

    return resultado;
}
//====================[Classificação de Saida]==========================================================================





int main()
{   
    int i, j, *k, exeTot, exeAtual = 0, linhaTesteAtual = 0, linhaTreinoAtual = 0, linhasTreino, featuresTreino, linhasTeste, featuresTeste, **matConfus;
    float *r, **matTreino, **matTeste, *resultados, *rotulos, *rotulosAvaliados, *rotulosTeste, precisao, numRotulos;
    char *pathTreino, *pathTeste, *pathSaida, *tDist;
    FILE *predicao;

    printf(">Obtendo parametros de configuracao -> ");
    readPath( &pathTreino, &pathTeste, &pathSaida );///////////TEM QUE ALOCAR DINAMICAMENTE ISSO!!!!!!!
    readParam( &k, &tDist, &r, &exeTot );
    printf("OK\n");

    printf(">Obtendo parametros de treino -> ");
    matTreino = loadFeatures( pathTreino, &linhasTreino, &featuresTreino );
    printf("OK\n");
    
    printf(">Obtendo parametros de teste -> ");
    matTeste = loadFeatures( pathTeste, &linhasTeste, &featuresTeste );
    printf("OK\n");


    printf("Calculando...\n");

    resultados = (float *) malloc( linhasTreino * sizeof(float));
    rotulos = (float *) malloc( linhasTreino * sizeof(float));
    rotulosAvaliados = (float *) malloc( linhasTeste * sizeof(float));
    rotulosTeste = (float *) malloc( linhasTeste * sizeof(float));
    /*
    - resultados armazena as distancias entre o objeto de teste e cada um dos objetos do treino, e armazena o resultado na posição correspondente
    - rotulos armazena o rotulo que aquele objeto de treino possui, na mesma posição do resultado de sua distancia em relação ao objeto de teste.
    - rotulosAvaliados é a resposta de todos os rotulos do treino já classificados
    */

    while( exeAtual < exeTot ) //Executa de acordo com os parametros fornecidos pelo config
    {   
        printf(" -Execucao %d: ", exeAtual+1 );

        for ( linhaTesteAtual = 0 ; linhaTesteAtual < linhasTeste ; linhaTesteAtual++ ) //Pega a linha i do teste e compara com todas as linhas do treino
        {   
            for ( linhaTreinoAtual = 0 ; linhaTreinoAtual < linhasTreino ; linhaTreinoAtual++ )//Percorre todas as linhas do treino
            {   
                resultados[linhaTreinoAtual] = selecDist( matTreino[linhaTreinoAtual], matTeste[linhaTesteAtual], (featuresTreino - 1), r[exeAtual], tDist[exeAtual] );
                rotulos[linhaTreinoAtual] = matTreino[linhaTreinoAtual][featuresTreino-1];
            }

            rotulosTeste[linhaTesteAtual] = matTeste[linhaTesteAtual][featuresTeste-1];
            bubbleSortDouble( &resultados, &rotulos, linhasTreino - 1 );
            rotulosAvaliados[linhaTesteAtual] = KNN( k[exeAtual], rotulos ); 
        }

        //=====Calcula os resultados finais a serem exibidos=====
        numRotulos = maiorNum( rotulosTeste, linhasTeste );
        matConfus = matrizConfusao( rotulosAvaliados, rotulosTeste, numRotulos, linhasTeste);
        precisao = acuracia( rotulosAvaliados, matTeste, linhasTeste, featuresTeste );
        writeFile( exeAtual, rotulosAvaliados, linhasTeste, pathSaida, precisao, matConfus, numRotulos );
        
        //=====Libera a memoria alocada da matriz de confusão para a proxima execução
        for ( i = 0 ; i < numRotulos ; i++ )
        {
            free(matConfus[i]);
        }
        free(matConfus);

        printf("OK\n");
        exeAtual++;
    }
    printf("Finalizado!\n");
      
    
    for( i = 0 ; i < linhasTreino ; i++ )
    {   
        free(matTreino[i]);
    }
    for( i = 0 ; i < linhasTeste ; i++ )
    {   
        free(matTeste[i]);
    }
    free(rotulosTeste);
    free(rotulosAvaliados);
    free(rotulos);
    free(resultados);
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
/*
    TRABALHO COMPUTACIONAL - PROGRAMAÇÃO II
              CLASSIFICADOR KNN
   Bruno Belmock Santos e Kevin Vieira Izabel
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lib/classificacao.h"
#include "lib/vetmanip.h"
#include "lib/filemanip.h"
#include "lib/calcdistancias.h"

int main()
{   
    int i, j, *k, exeTot, exeAtual = 0, linhaTesteAtual = 0, linhaTreinoAtual = 0, linhasTreino, featuresTreino, linhasTeste, featuresTeste, **matConfus;
    float *r, **matTreino, **matTeste, *resultados, *rotulos, *rotulosAvaliados, *rotulosTeste, precisao, numRotulos;
    char *pathTreino, *pathTeste, *pathSaida, *tDist;

    printf(">Obtendo parametros de configuracao -> ");
    readPath( &pathTreino, &pathTeste, &pathSaida );
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

/*
Código de erros
Informa em qual função o erro ocorreu e o tipo do erro
RP - readPath Null   || FL - fileLines Null  || LF - loadFeatures Null || CL - charLine Null ||
RM - readParam Null  || WF - writeFile Null  || FF - fileFeatures Null ||                                           
*/
}

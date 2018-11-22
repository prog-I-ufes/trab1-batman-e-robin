//Header das funções manipuladoras de arquivos
#ifndef __FILEMANIP_H
#define __FILEMANIP_H
int fileFeatures( char *nome, int linha );
int fileLines( char *nome );
int charLine( char *nomeArq, int linhaSelec );
void readPath( char **pTreino, char **pTeste, char **pSaida );
void readParam( int **k, char **tDist, float **r , int *execucoes );
void writeFile( int numExe, float *rotulos, int qtdRotulos, char *pathEscrita, float acuracia, int **matConfus, int numRotulos );
float** loadFeatures( char *pathArq, int *linhas, int *features );
#endif
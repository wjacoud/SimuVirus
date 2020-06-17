/*
 *  ISEC - DEIS - ENGENHARIA INFORMÁTICA
 *  DISCIPLINA - PROGRAMAÇÃO
 *  Prof: Francisco Pereira
 *
 *  Nome: Wilson Hermes Jacoud Junior
 *  N• 2019147314
 *
 *  Turma P1 (Pós Laboral)
 */

#ifndef SIMUVIRUS_H
#define SIMUVIRUS_H

//def estruturas


typedef struct sala local;
typedef struct espaco master, *pmaster;
typedef struct pessoas pessoa, *ppessoa;

//Struct sala é somente referência para a leitura dos ficheiros.
struct sala{
    int id;
    int capacidade;
    int lig[3];
};

//Struct espacos recebe dados tipo struct sala, porém com campos extras necessários à simulação. São organizados em vetores dinámicos de estruturas.

struct espaco{
    int id;
    int capacidade;
    int lig[3];
    int contS;
    int contD;
    int contI;
    int contT;
    ppessoa pp;
};

//Recebe os dados dos ficheiros de texto, depois são organizados em listas ligadas ligadas a seus devidos lugares no vetor dinámico.
struct pessoas {
    char nome[100];
    int idade;
    char estado;
    int dias;
    ppessoa prox;
};

void libertaTudo(pmaster pMstr,int *tl);

void libertaPessoas(pmaster pMstr,int *tl);

int intSafeIn();

void strSafeIn(char *key);

int menuPrincipal();

int menuEspacos();

int menuPopulacao();

int menuSimulacao(int *dia);

int menuRelatorios();

void fileNameGen(int *dia, char *ret);

int verificaLocal(pmaster pMstr, int *t);

pmaster leDadosLocais(char *fDelocais,int *t);

void escreve_espaco(pmaster a,int *t);

int leDadosPessoas(pmaster pMstr, char *f, int *totalP,int *totalL,int *totalS,int *totalD,int *totalI);

int verificaPessoa(pessoa *p);

void escreveRelParc(pmaster pMstr, int *totalL, int *totalP, int *totalS, int *totalD, int *totalI, int *novosS, int *novosD, int *novosI, int *dia);

void escreveFicheirosMaster(pmaster pMstr,char *nomeF, int *totalL, int *novosS, int *novosD, int *novosI);

pmaster leFicheirosMaster(char *nomeF, int *totalL, int *totalP, int *novosS, int *novosD, int *novosI, int *totalS, int *totalD, int *totalI);

void acrescentaDoente(pmaster pMstr, int *totalP, int *totalL);

ppessoa deppendPessoaRdn(pmaster pMstr,int i);

void movePessoas(pmaster pMstr, int *totalL, int *totalP);

void simulacao(pmaster pMstr, int *totalL, int *totalS, int *totalD, int *totalI, int *novosS, int *novosD, int *novosI, int *dia);

void reportGen(int *totalL, int ultDia);

void reportTXTGen(int *totalL, int ultDia);

// Utils

void initRandom();

int intUniformRnd(int a, int b);

int probEvento(float prob);


#endif /* SIMUVIRUS_H */


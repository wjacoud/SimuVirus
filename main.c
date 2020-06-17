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

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "simuVirus.h"


int main(int argc, char** argv){
    
    UINT CPAGE_UTF8 = 65001;
    UINT CPAGE_DEFAULT = GetConsoleOutputCP();
    SetConsoleOutputCP(CPAGE_UTF8);

    char opRec[15] = {"recover000.bin"};
    char *opRecLast = {"recover.bin"};
    char *opEsp1 = {"E1.bin"};
    char *opEsp2 = {"E2.bin"};
    char *opEsp3 = {"E3.bin"};
    char *opPes1 = {"pessoasA.txt"};
    char *opPes2 = {"pessoasB.txt"};
    char *opPes3 = {"pessoasC.txt"};

    int dias = 0,ultDia = 0, *dia;dia=&dias;
    int totalLoc = 0,*tl;tl=&totalLoc;
    int totalPes = 0,*tp;tp=&totalPes;
    int totalSau = 0,*ts;ts=&totalSau;
    int totalDoe = 0,*td;td=&totalDoe;
    int totalImu = 0,*ti;ti=&totalImu;
    int nS[6] = {0};
    int nD[6] = {0};
    int nI[6] = {0};
    int capacidadeTotal, lotacaoEsgotada = 0;
    int c, ciclos, *cc = &c;

    pmaster pMstr = NULL;

    void initRandom();

    puts("\n\n\t*---------* SIMULADOR DE PROPAGAÇÃO VIRAL *---------*\n");

    int opI, opP;
    do{
        MAINMENU:
        opP = menuPrincipal();
        switch (opP) {
            case 1:
                do {
                    opI = menuEspacos();
                    switch (opI) {
                        case 1: pMstr= leDadosLocais(opEsp1,tl);
                            break;
                        case 2: pMstr= leDadosLocais(opEsp2,tl);
                            break;
                        case 3: pMstr= leDadosLocais(opEsp3,tl);
                            break;
                        case 4: goto MAINMENU;
                    }
                    totalPes = 0;
                    dias = 0;
                    escreve_espaco(pMstr,tl);
                    printf("\tRelatório Parcial:\n\n\tTotal de Locais: %d\tTotal de Pessoas: %d\n\n\n",*tl,*tp);
                }while (opI<1 || opI>4);
                break;
            case 2:
                do{
                    opI = menuPopulacao();
                    switch (opI) {
                        case 1: capacidadeTotal = leDadosPessoas(pMstr,opPes1,tp,tl,ts,td,ti);
                            break;
                        case 2: capacidadeTotal = leDadosPessoas(pMstr,opPes2,tp,tl,ts,td,ti);
                            break;
                        case 3: capacidadeTotal = leDadosPessoas(pMstr,opPes3,tp,tl,ts,td,ti);
                            break;
                        case 4:goto MAINMENU;
                    }
                    //lotacao esgotada será flag para outras funcionalidades
                    if(capacidadeTotal == *tp)lotacaoEsgotada=1;
                    escreve_espaco(pMstr,tl);
                    fileNameGen(dia,opRec);
                    //escreve o primeiro ficheiro master temporário "Armazenameto dos dados para análise e/ou regresso de iterações" "Esses ficheiros são deletados ao final da execução."
                    escreveFicheirosMaster(pMstr,opRec,tl,nS,nD,nI);
                    printf("\tRelatório Parcial:\n\n\tTotal de Locais: %d\tTotal de Pessoas: %d\tPessoas Saudáveis: %d\tPessoas Doentes: %d\tPessoas Imunes: %d\n\n\n",*tl,*tp,*ts,*td,*ti);
                }while (opI<1 || opI>4);
                break;
            case 3:
                //Leitura do ficheiro Recover.bin "Este ficheiro é criado no fechamento do programa e permite resgatar a distribuição final da última simulação"
                pMstr = leFicheirosMaster(opRecLast,tl,tp,nS,nD,nI,ts,td,ti);
                fileNameGen(dia,opRec);
                //Escreve o ficheiro master temporário
                escreveFicheirosMaster(pMstr,opRec,tl,nS,nD,nI);
                escreve_espaco(pMstr,tl);
                printf("Relatório Parcial:\n\nTotal de Locais: %d\tTotal de Pessoas: %d\tPessoas Saudáveis: %d\tPessoas Doentes: %d\t\tPessoas Imunes: %d\n\n\n",*tl,*tp,*ts,*td,*ti);
                break;
            case 4:
                do{
			if(*tl ==0){
                        printf("\tLocais não carregados\n");
                        goto MAINMENU;
                    }
                    opI = menuSimulacao(dia);
                    switch (opI) {
                        case 1:
                            //Permite avançar uma ou mais iterações na simulação
                            printf("\tInsira o número de dias a serem avançados:\n\n");
                            ciclos = intSafeIn();
                            for (c = 1 ; c <= ciclos ; c++){
                                simulacao(pMstr,tl,ts,td,ti,nS,nD,nI,dia);
                                //Cada ficheiro temporário terá um nome subsequente referente a cada iteração.
                                fileNameGen(dia,opRec);
                                escreveFicheirosMaster(pMstr,opRec,tl,nS,nD,nI);
                                escreveRelParc(pMstr,tl,tp,ts,td,ti,nS,nD,nI,cc);
                            }
                            escreve_espaco(pMstr,tl);
                            break;
                        case 2: if(lotacaoEsgotada){
                                printf("Lotação esgotada!\n\n");
                            } else{
                                acrescentaDoente(pMstr,tp,tl);
                                //ficheiros são sobrescritos em caso de pessoas acrescentadas ou transferidas de local.
                                escreveFicheirosMaster(pMstr,opRec,tl,nS,nD,nI);
                                escreve_espaco(pMstr,tl);
                                escreveRelParc(pMstr,tl,tp,ts,td,ti,nS,nD,nI,dia);
                                if(capacidadeTotal == *tp)lotacaoEsgotada=1;
                            }
                            break;
                        case 3: if(lotacaoEsgotada) printf("Lotação esgotada!\n\n");
                            else{
                                movePessoas(pMstr,tl,tp);
                                escreveFicheirosMaster(pMstr,opRec,tl,nS,nD,nI);
                                escreve_espaco(pMstr,tl);
                                escreveRelParc(pMstr,tl,tp,ts,td,ti,nS,nD,nI,dia);
                            }
                            break;
                            //permite voltar quantas itarações forem desejadas desde o início da simulação.
                        case 4:printf("Digite o dia a ser resgatado (Estado atual: DIA %d)",*dia);
                            dias = intSafeIn();
                            fileNameGen(dia,opRec);
                            pMstr = leFicheirosMaster(opRec,tl,tp,nS,nD,nI,ts,td,ti);
                            escreve_espaco(pMstr,tl);
                            escreveRelParc(pMstr,tl,tp,ts,td,ti,nS,nD,nI,dia);
                            break;
                        case 5:goto MAINMENU;
                    }
                    if(dias > ultDia)ultDia = dias;
                }while (opI<1 || opI>5);
                break;
            case 5:
                do{
			if(*tl ==0){
                        printf("\tLocais não carregados\n");
                        goto MAINMENU;
                    }
                    opI = menuRelatorios();
                    switch (opI) {
                        case 1: reportGen(tl,ultDia);
                            break;
                        case 2: reportTXTGen(tl,ultDia);
                            break;
                        case 3: goto MAINMENU;
                    }
                }while (opI<1 || opI>3);
                break;
        }
    }while (opP < 6);

    // Romove todos os ficheiros temporários
    int i, *id = &i;
    for (i = 0; i <= ultDia; i++) {
        fileNameGen(id,opRec);
        remove(opRec);
    }
    //Grava um ficheiro master "Recover.bin" com a  distribuição final para poder ser resgatado na próxima execução
    escreveFicheirosMaster(pMstr,opRecLast,tl,nS,nD,nI);
    //Libera a memória alocada em toda a estrutura
    libertaTudo(pMstr,tl);
    printf("\n\n\n\nFim do programa\n\n\n\n");
    return 0;
}
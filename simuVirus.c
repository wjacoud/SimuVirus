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
#include <string.h>
#include<locale.h>
#include <ctype.h>
#include <time.h>
#include"simuVirus.h"


// Função utilizada para libertar as listas ligadas (struct pessoas)
void libertaTudo(pmaster pMstr,int *tl){
    int j;
    ppessoa aux1, aux2;
    for (j = 0; j < *tl; j++) {
        if(pMstr[j].pp){
            aux1 = pMstr[j].pp;
            while (aux1->prox) {
                aux2 = aux1;
                aux1 = aux1->prox;
                free(aux2);
            }
        }
    }
    free(pMstr);
}

// Função utilizada para libertar as listas ligadas (struct pessoas) e o vetor dinâmico (struct espaço "master)
void libertaPessoas(pmaster pMstr,int *tl){
    int j;
    ppessoa aux1, aux2;
    for (j = 0; j < *tl; j++) {
        if(pMstr[j].pp){
            aux1 = pMstr[j].pp;
            while (aux1->prox) {
                aux2 = aux1;
                aux1 = aux1->prox;
                free(aux2);
            }
        }
        pMstr[j].pp = NULL;
    }
}

// Função utilizada para validar a entrada de dados tipo inteiros
int intSafeIn(){
    int safe;
    char keyboard[16];
    do{
        fgets(keyboard, 15, stdin);
        keyboard[15] = 0;
        if(isdigit(keyboard[0]))
            safe = atoi(keyboard);
        else{
            printf("Caracteres inválidos (Utilize somente números sem espaços)");
        }
    }while (!isdigit(keyboard[0]));
    return safe;
}

// Função utilizada para validar a entrada de dados tipo string (permite somente caracteres alfanuméricos sem espaços, como no padrão dos ficheiros de pessoas)
void strSafeIn(char *key){
    int i,flag;
    do {
        flag = 1;
        fgets(key, 99, stdin);
        key[99] = 0;
        for(i=0 ; i<(strlen(key)-1); i++){
            if(!(isalnum(key[i]))) {
                puts("Caracteres inválidos (Utilize somente alfanuméricos sem espaços)");
                flag = 0;
                break;
            }
        }
    }while(flag == 0);
}

// Inicio das funções menu

int menuPrincipal(){
    int i;
    puts("\tMENU PRINCIPAL\n");
    puts("\t1 - Carregar espaço");
    puts("\t2 - Carregar pessoas");
    puts("\t3 - Resgatar último cenário");
    puts("\t4 - Simulação");
    puts("\t5 - Relatórios estatísticos");
    puts("\t6 - Sair do programa\n\n");
    do{
        i = intSafeIn();
        if(i < 1 || i > 6)puts("Opção inválida:\n\n");
    }while (i < 1 || i > 6);
    return i;
}

int menuEspacos(){
    int i;
    puts("\tESCOLHA O ESPACO PARA A SIMULAÇÃO\n");
    puts("\t1 - Espaço 1 (4 locais / Capacidade total: 200 pessoas)");
    puts("\t2 - Espaço 2 (5 locais / Capacidade total: 125 pessoas)");
    puts("\t3 - Espaço 3 (6 locais / Capacidade total: 92 pessoas)");
    puts("\t4 - Voltar ao menu principal\n\n");
    do{
        i = intSafeIn();
        if(i < 1 || i > 4)puts("Opção inválida:\n\n");
    }while (i < 1 || i > 4);
    return i;
}

int menuPopulacao(){
    int i;
    puts("\tESCOLHA A POPULAÇÃO PARA A SIMULAÇÃO\n");
    puts("\t1 - População A ( 10 pessoas)");
    puts("\t2 - População B ( 50 pessoas)");
    puts("\t3 - População C (100 pessoas)");
    puts("\t4 - Voltar ao menu principal\n\n");
    do{
        i = intSafeIn();
        if(i < 1 || i > 4)puts("Opção inválida:\n\n");
    }while (i < 1 || i > 4);
    return i;
}

int menuSimulacao(int *dia){
    int i;
    printf("\tMENU SIMULAÇÃO  -  DIA %2d\n\n",*dia);
    puts("\t1 - Avançar na linha do tempo");
    puts("\t2 - Acrescentar pessoa doente");
    puts("\t3 - Transferir pessoas de lugar");
    puts("\t4 - Voltar a um dia específico");
    puts("\t5 - Voltar ao menu principal\n\n");
    do{
        i = intSafeIn();
        if(i < 1 || i > 5)puts("Opção inválida:\n\n");
    }while (i < 1 || i > 5);
    return i;
}

int menuRelatorios(){
    int i;
    puts("\tMENU RELATÓRIOS ESTATÍSTICOS\n\n");
    puts("\t1 - Obter relatório completo");
    puts("\t2 - Salvar relatório");
    puts("\t3 - Voltar ao menu principal\n\n");
    do{
        i = intSafeIn();
        if(i < 1 || i > 3)puts("Opção inválida:\n\n");
    }while (i < 1 || i > 3);
    return i;
}
// Fim das funções menu

// Função utilizada para gerar nomes ordenados para os ficheiros de dados temporários.
void fileNameGen(int *dia, char *ret){
    int d; d = *dia;
    int cen = d/100;
    int des = (d%100)/10;
    int uni = d%10;
    ret[7] = (cen+48);
    ret[8] = (des+48);
    ret[9] = (uni+48);
}

// Função utilizada para aceder aos ficheiros binários de locais e trasferir seus dados a um vector dinâmico.
pmaster leDadosLocais(char *nomeF,int *t) {
    FILE *f;
    int cont = 0;
    local aux;
    master *pMstr;

    f = fopen(nomeF, "rb");
    if (f == NULL){
        printf("Erro no acesso ao ficheiro de locais\n");
        return NULL;
    }
    pMstr = malloc(sizeof(master));
    if (pMstr == NULL){
        printf("Erro de alocação de memória\n");
        fclose(f);
        return NULL;
    }
// Estrutura "local" é a referência de acesso aos dados do ficheiro
// Estrutura "Master" é onde os dados serão armazenados durante a execução
    while (fread(&aux, sizeof(local), 1, f) == 1) {
        pMstr = realloc(pMstr, sizeof(master) * (cont + 1));
        if (pMstr == NULL){
            printf("Erro de alocação de memória\n");
            free(pMstr);
            fclose(f);
            return NULL;
        }
        pMstr[cont].id = aux.id;
        pMstr[cont].capacidade = aux.capacidade;
        for(int i=0 ; i<3 ; i++){
            pMstr[cont].lig[i] = aux.lig[i];
        }
        // Demais campos inicializados a 0/null
        pMstr[cont].contS = 0;
        pMstr[cont].contD = 0;
        pMstr[cont].contI = 0;
        pMstr[cont].contT = 0;
        pMstr[cont].pp = NULL;
        cont++;
        *t=cont;
    }
    fclose(f);
    printf("Carregamento dos locais efetuado com sucesso\n");
    //Verificação dos dados tipo local
    cont = verificaLocal(pMstr, t);
    if (cont == 0){
        printf("Erro na verificacao dos locais\n");
        free(pMstr);
        return NULL;
    }
    return pMstr;
}

// Função de verificação dos dados tipo Local
int verificaLocal(pmaster pMstr, int *t){
    int i , j , k , l;
    //Verifica ids positivos.
    for(i=0 ; i<*t ; i++){
        if( pMstr[i].id<0 ){
            return 0;
        }
    }
    //Verifica ids repetidos
    for(i = 0 ; i < *t ; i++){
        for(j = i+1; j < *t; j++){
            if (pMstr[i].id == pMstr[j].id){
                return 0;
            }
        }
    }
    //Verifica ligações bilaterais
    for (j = 0; j < *t; j++){
        for (k = 0; k < 3; k++){
            if ((pMstr[j].lig[k]) != -1){
                for (l = 0; l < 3; l++){
                    if ((pMstr[j].lig[k]) == pMstr[l].id){
                        if ((pMstr[j].id) != pMstr[l].lig[0] && pMstr[j].id != pMstr[l].lig[1] && pMstr[j].id != pMstr[l].lig[2])
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    printf("Verificação dos locais efetuada com sucesso\n\n");
    return 1;
}

// Função de impressão na tela de estruturas de locais (com ou sem pessoas alocadas)
void escreve_espaco(pmaster pMstr, int *t) {
    int i, j;
    ppessoa aux;
    for (j = 0; j < *t; j++) {
        printf("\tLocal ");
        printf("Id: %2d\t", pMstr[j].id);
        printf("Capacidade: %d\t", pMstr[j].capacidade);
        for (i = 0; i < 3; i++) {
            if(pMstr[j].lig[i] != -1) printf("\tLig: %d", pMstr[j].lig[i]);
            else{printf("\t");}
        }
        printf("\tSaudáveis: %i\t", pMstr[j].contS);
        printf("Doentes: %i\t", pMstr[j].contD);
        printf("Imunes: %i\t", pMstr[j].contI);
        printf("Total de Pessoas: %i\n", pMstr[j].contT);
        aux = pMstr[j].pp;
        while (aux) {
            printf("\n\tNome: %-30s\t\tIdade: %2d\t\t Estado: %c\t\t", aux->nome, aux->idade, aux->estado);
            if(aux->dias >= 0) printf("Dias:%2d" , aux->dias);
            aux = aux->prox;
        }
        printf("\n\n");
    }
}

// Função de impressão na tela de relatório parcial (dados preliminares da simulação)
void escreveRelParc(pmaster pMstr, int *totalL, int *totalP, int *totalS, int *totalD, int *totalI, int *novosS, int *novosD, int *novosI, int *dia){
    int i,novosST=0, novosDT=0, novosIT=0;
    printf("\t******-------------  RELATÓRIO PARCIAL  -------------******\n\tDia: %2d\n\tTotal de Locais: %d\tTotal de Pessoas: %d\n\tSITUAÇÃO ATUAL\n\tPessoas Saudáveis: %d\tPessoas Doentes: %d\tPessoas Imunes: %d\n", *dia,*totalL,*totalP,*totalS,*totalD,*totalI);
    printf("\n\t\t\tNOVOS CASOS\n");
    printf("\t\t\tPessoas Curadas\t   Saudáveis\t    Imunes \t   Doentes\n");
    for (i = 0; i < *totalL; i++) {
        printf("\tLocal id:%2d\t%8d\t%8d\t%8d\t%8d\n",pMstr[i].id,(novosS[i]+novosI[i]),novosS[i],novosI[i],novosD[i]);
        novosST+=novosS[i];
        novosDT+=novosD[i];
        novosIT+=novosI[i];
    }
    printf("\tTotal--->\t%8d\t%8d\t%8d\t%8d\n\n\n",(novosST+novosIT),novosST,novosIT,novosDT);
}

// Função de leitura de dados tipo pessoa em ficheiros txt
int leDadosPessoas(pmaster pMstr, char *f, int *totalP,int *totalL,int *totalS,int *totalD,int *totalI){
    FILE *f1;
    ppessoa novaPessoa = NULL;
    int cont=*totalP, capacidadeTotal=0;
// Verificação que não permite a leitura antes dos locais estarem prontos
    if (*totalL == 0){
        printf("Locais não carregados\n");
        return 0;
    }
// Primeira contagem e verificação da capacidade total do espaço (usada para o caso de carregar mais de um ficheiro de pessoas)
    for(int i=0; i<*totalL; i++){
        capacidadeTotal += pMstr[i].capacidade;
    }
    if(cont == capacidadeTotal){
        printf("Lotação esgotada!\n");
        return capacidadeTotal;
    }

    f1=fopen(f,"r");
    if(f1==NULL){
        printf("Erro no acesso ao ficheiro de pessoas\n");
        return 0;
    }
    novaPessoa = malloc(sizeof(pessoa));
    if(novaPessoa == NULL){
        printf("Erro de alocação de memória\n");
        fclose (f1);
        return 0;
    }

    *totalP = 0; *totalS = 0; *totalD = 0; *totalI = 0;

    while(fscanf(f1,"%s %d %c %d", novaPessoa->nome, &novaPessoa->idade, &novaPessoa->estado, &novaPessoa->dias)!=EOF){
        //Campo dias inicializado a -1 para identificar e não imprimir em pessoas com estados S ou I.
        if ((novaPessoa->estado) != 'D'){
            novaPessoa->dias = -1;
        }
        // Verificação de dados tipo pessoa
        int v = verificaPessoa(novaPessoa);
        if(v == 0){
            printf("Ficheiro de pessoas em formato inválido\n");
            libertaPessoas(pMstr,totalL);
            fclose (f1);
            return 0;
        }
        // Variável randon para alocar de forma aleatória novas pessoas nos diversos locais atraves do id do vetor dinâmico.
        // ciclo controla a capacidade de cada local, gerando novo id em caso de lotação.
        int randon;
        do{
            randon = intUniformRnd(0, (*totalL-1));
        }while (pMstr[randon].contT  >= pMstr[randon].capacidade);
        //Cada nova estrutura pessoa é adicionada ao início da lista dinâmica que está ligada ao local referente no vector de locais.
        novaPessoa->prox=pMstr[randon].pp;
        pMstr[randon].pp = novaPessoa;
        pMstr[randon].contT +=1;
        if(novaPessoa->estado == 'S') pMstr[randon].contS +=1;
        if(novaPessoa->estado == 'D') pMstr[randon].contD +=1;
        if(novaPessoa->estado == 'I') pMstr[randon].contI +=1;

        cont++;
        // Segunda contagem e verificação da capacidade total do espaço (usada para o caso de esgotar-se ao longo do carregamento de pessoas)
        if(cont == capacidadeTotal){
            fclose(f1);
            printf("Lotação esgotada!\t");
            puts("Ficheiro de pessoas verificado com sucesso.\n");
            for(int i=0; i<*totalL; i++){
                *totalS+=pMstr[i].contS;
                *totalD+=pMstr[i].contD;
                *totalI+=pMstr[i].contI;
                *totalP+=pMstr[i].contT;
            }
            return capacidadeTotal;
        }
        novaPessoa = malloc(sizeof(pessoa));
        if(novaPessoa == NULL){
            printf("Erro de alocação de memória\n");
            fclose(f1);
            libertaPessoas(pMstr,totalL);
            return 0;
        }

    }
    fclose(f1);
    for(int i=0; i<*totalL; i++){
        *totalS+=pMstr[i].contS;
        *totalD+=pMstr[i].contD;
        *totalI+=pMstr[i].contI;
        *totalP+=pMstr[i].contT;
    }
    puts("Ficheiro de pessoa verificado com sucesso.\n");
    return capacidadeTotal;
}

// Função de verificação de dados pessoas
int verificaPessoa(ppessoa p){
    int i;
    for(i=0 ; i<strlen(p->nome); i++){
        if(!(isalnum(p->nome[i]))) return 0;
    }
    if(p->idade < 0 || p->idade > 130) return 0;
    if(p->estado != 'S' && p->estado != 'D' && p->estado != 'I') return 0;
    if(p->dias < -1) return 0;
    return 1;
}

// Função de gravação de ficheiros binários "recover"(ficheiros que armazenam todas a estruturas de locais, pessoas e outros dados para a simulação e compilação de dados estatísticos.)
void escreveFicheirosMaster(pmaster pMstr,char *nomeF, int *totalL, int *novosS, int *novosD, int *novosI){
    FILE *f;
    ppessoa p;
    int i;

    f = fopen(nomeF, "wb");
    if(f == NULL){
        puts("Erro no acesso ao ficheiro\n");
        return;
    }
    //Inteiro com o número de locais
    fwrite(totalL, sizeof(int), 1, f);
    //Vetor de locais
    fwrite(pMstr, sizeof(master), *totalL, f);
    //Estruturas de pessoas
    for(i = 0; i < *totalL; i++){
        p = pMstr[i].pp;
        while(p != NULL){
            fwrite(p, sizeof(pessoa), 1, f);
            p = p->prox;
        }
    }
    //vetores de inteiros contendo variação de estados separados por local
    fwrite(novosS, sizeof(int), *totalL, f);
    fwrite(novosD, sizeof(int), *totalL, f);
    fwrite(novosI, sizeof(int), *totalL, f);
    fclose(f);
}

// Função de leitura de ficheiros binários "recover"
pmaster leFicheirosMaster(char *nomeF, int *totalL, int *totalP, int *novosS, int *novosD, int *novosI, int *totalS, int *totalD, int *totalI){
    pmaster pMstr = NULL;
    ppessoa p, aux;
    FILE *f;
    int i,j;
    *totalP = 0;
    *totalS = 0;
    *totalD = 0;
    *totalI = 0;

    f = fopen(nomeF, "rb");
    if(f == NULL){
        puts("Erro no acesso ao ficheiro\n");
        return NULL;
    }
    fread(totalL, sizeof(int), 1, f);
    pMstr = malloc(sizeof(master) * (*totalL));
    if(pMstr == NULL){
        printf("Erro de alocação de memória\n");
        fclose (f);
        return NULL;
    }
    fread(pMstr, sizeof(master), (*totalL), f);
    for (i = 0; i < *totalL ; i++) {
        *totalP += pMstr[i].contT;
        *totalS += pMstr[i].contS;
        *totalD += pMstr[i].contD;
        *totalI += pMstr[i].contI;
        for (j = 0; j < pMstr[i].contT; j++) {
            p = malloc(sizeof(pessoa));
            if (p == NULL){
                printf("Erro de alocação de memória\n");
                free(pMstr);
                fclose(f);
                return NULL;
            }
            fread(p, sizeof(pessoa), 1, f);
            if(j == 0) {
                pMstr[i].pp = p;
                aux = p;
            }
            else{
                aux->prox = p;
                aux = p;
            }
        }

    }
    fread(novosS, sizeof(int), (*totalL), f);
    fread(novosD, sizeof(int), (*totalL), f);
    fread(novosI, sizeof(int), (*totalL), f);
    fclose(f);
    return pMstr;
}

// Função para acrescentar pessoa doente
void acrescentaDoente(pmaster pMstr, int *totalP, int *totalL){
    int i, id, flag=1, flag2=1;
    char nome[100];
    ppessoa novo;
    //Ciclo de validação (id válido e Lotação do local)
    while(flag || flag2){
        printf("Indique o Id do local onde a pessoa será adicionada(");
        for (i = 0; i < *totalL; i++){
            printf(" %d ",pMstr[i].id);
        }
        printf("): \n\n");
        id = intSafeIn();
        for (i = 0; i < *totalL; i++) {
            if(id == pMstr[i].id){
                flag = 0;
                break;
            } else{
                flag = 1;
            }
        }
        if(pMstr[i].contT<pMstr[i].capacidade) {
            flag2 = 0;
        }else{
            flag2 = 1;
        }
        if(flag2 && !flag)printf("O local escolhido está lotado\n");
        if(flag)printf("Id não encontrado\n");
    }
    novo = malloc(sizeof(pessoa));
    if(novo==NULL){
        printf("Erro de alocação de memória\n");
        return;
    }
    printf("Digite o nome:");
    strSafeIn(nome);
    for (int j = 0; j < strlen(nome) ; j++) {
        if(nome[j] == '\n') nome[j] = '\0';
    }
    strncpy(novo->nome,nome,99);
    printf("Digite a idade:");
    novo->idade = intSafeIn();
    printf("Digite o número de dias doente:");
    novo->dias = intSafeIn();
    novo->estado = 'D';
    novo->prox = pMstr[i].pp;
    pMstr[i].pp = novo;
    pMstr[i].contD+=1;
    pMstr[i].contT+=1;
    //Nova pessoa adicionada no início da lista ligada.
    *totalP+=1;
    printf("Pessoa adicionada com sucesso\n");
    return;
}

//Função auxiliar para selecionar pessoa aleatória e tirar da lista ligada.
ppessoa deppendPessoaRdn(pmaster pMstr, int i){
    int j, turns, tp = pMstr[i].contT;
    ppessoa ret, aux = pMstr[i].pp;
    if(tp == 0){
        printf("Local vazio");
        return NULL;
    }
    // Variável turns determina aleatoriamente a posição da pessoa a ser tirada da lista.
    turns = intUniformRnd(0,(tp - 1));
    if(turns == 0){
        if(pMstr[i].contT == 1){
            pMstr[i].pp = NULL;
            pMstr[i].contT-=1;
            if(aux->estado == 'S')pMstr[i].contS-=1;
            if(aux->estado == 'D')pMstr[i].contD-=1;
            if(aux->estado == 'I')pMstr[i].contI-=1;
            return aux;
        }
        pMstr[i].pp = aux->prox;
        aux->prox = NULL;
        pMstr[i].contT-=1;
        if(aux->estado == 'S')pMstr[i].contS-=1;
        if(aux->estado == 'D')pMstr[i].contD-=1;
        if(aux->estado == 'I')pMstr[i].contI-=1;
        return aux;
    }
    if(turns > 1){
        for (j = 1; j < turns; j++) {
            aux = aux->prox;
        }
    }
    //Função retorna a pessoa tirada da lista
    //Contadores são atualizados
    ret = aux->prox;
    aux->prox = aux->prox->prox;
    ret->prox = NULL;
    pMstr[i].contT-=1;
    if(ret->estado == 'S')pMstr[i].contS-=1;
    if(ret->estado == 'D')pMstr[i].contD-=1;
    if(ret->estado == 'I')pMstr[i].contI-=1;
    return ret;
}

void movePessoas(pmaster pMstr, int *totalL, int *totalP){
    int i, n, iOri, iDes, in, flag=1, flag2=1, lugaresDes;
    ppessoa p;
    if (*totalP == 0){
        printf("Não há pessoas carregadas\n\n");
        return;
    }
    //Verifica lugar de origem( Id válido e se existem pessoas)
    while(flag || flag2){
        printf("Digite o id do lugar de origem (");
        for (iOri = 0; iOri < *totalL; iOri++){
            printf(" %d ",pMstr[iOri].id);
        }
        printf("): \n\n");
        in = intSafeIn();
        for (iOri = 0; iOri < *totalL; iOri++) {
            if(in == pMstr[iOri].id){
                flag = 0;
                break;
            } else{
                flag = 1;
            }
        }
        if(pMstr[iOri].contT != 0){
            flag2 = 0;
        } else{
            flag2 = 1;
        }
        if(flag2 && !flag)printf("O Local escolhido está vazio\n");
        if(flag)printf("Id não encontrado\n");
        if(!flag && !flag2)printf("Existem %d pessoas no local escolhido\n",pMstr[iOri].contT);
    }
    flag = 1;
    flag2 = 1;
    //Verifica lugar de destino(Id válido e se existe espaço disponível)
    while(flag || flag2){
        printf("Digite o id do lugar de destino (");
        for (iDes = 0; iDes < *totalL; iDes++){
            printf(" %d ",pMstr[iDes].id);
        }
        printf("): \n\n");
        in = intSafeIn();
        for (iDes = 0; iDes < *totalL; iDes++) {
            if(in == pMstr[iDes].id){
                flag = 0;
                break;
            }
        }
        if(pMstr[iDes].contT < pMstr[iDes].capacidade){
            flag2 = 0;
            lugaresDes = pMstr[iDes].capacidade - pMstr[iDes].contT;
        }
        if(flag2 && !flag)printf("O Local escolhido está lotado\n");
        if(flag)printf("Id não encontrado\n");
        if(!flag && !flag2)printf("Existem %d lugares disponíveis no local escolhido\n",lugaresDes);
    }
    flag = 1;
    flag2 = 1;
    //Verifica número de pessoas a se transferir (pessoas na origem e espaço no destino)
    while(flag || flag2) {
        printf("Digite o número de pessoas a transferir: \n\n");
        n = intSafeIn();
        if (n <= pMstr[iOri].contT) flag = 0;
        if (n <= lugaresDes) flag2 = 0;
        if(flag)printf("Não existem pessoas suficientes no local de origem\n");
        if(flag2)printf("Não existem lugares suficientes no local de destino\n");
    }
    //Transfere as pessoas usando a função anterior e adicionando ao início da lista no novo local
    for (i = 0; i < n; i++) {
        p = deppendPessoaRdn(pMstr,iOri);
        p->prox = pMstr[iDes].pp;
        pMstr[iDes].pp = p;
        pMstr[iDes].contT+=1;
        if(p->estado == 'S')pMstr[iDes].contS+=1;
        if(p->estado == 'D')pMstr[iDes].contD+=1;
        if(p->estado == 'I')pMstr[iDes].contI+=1;
    }
}

void simulacao(pmaster pMstr, int *totalL, int *totalS, int *totalD, int *totalI, int *novosS, int *novosD, int *novosI, int *dia){
    int i, j, k, randon, true = 0;
    int txDiss, txMaxRec, txImu;
    float txRec;
    ppessoa aux;
    *totalS=0;
    *totalD=0;
    *totalI=0;
    for (i = 0; i < *totalL ; i++) {
        novosS[i] = 0;
        novosD[i] = 0;
        novosI[i] = 0;
        //Taxa de disseminação (Cada doente transmite a 5% do total de pessoas)
        //Variável será os possíveis novos doentes
        //São escolhidos de forma aleatória, se forem saudáveis, passam a doentes.
        txDiss = (pMstr[i].contT) * 0.05 * (pMstr[i].contD);
        for (j = 0; j < txDiss; j++) {
            aux = pMstr[i].pp;
            randon = intUniformRnd(0,(pMstr[i].contT - 1));
            for (k = 0; k < randon; k++) {
                aux = aux->prox;
            }
            if(aux->estado == 'S'){
                aux->estado = 'D';
                aux->dias = 0;
                pMstr[i].contD += 1;
                pMstr[i].contS -= 1;
                novosD[i]+=1;
            }
        }
        aux = pMstr[i].pp;
        for (j = 0; j < (pMstr[i].contT); j++) {
            if(aux->estado == 'D'){
                aux->dias+=1;
                //Probabilidade de cura (1/idade)
                txRec = (float)1/(float)aux->idade;
                true = probEvento(txRec);
                //Máximo de dias doente por idade
                txMaxRec = 5 + aux->idade/10;
                if(aux->dias >= txMaxRec) true = 1;
                if(true){
                    //Imunização 20%
                    true = probEvento((float)0.2);
                    if(true){
                        aux->estado = 'I';
                        aux->dias = -1;
                        pMstr[i].contD -=1;
                        pMstr[i].contI +=1;
                        novosI[i]+=1;
                    }
                    else{
                        aux->estado = 'S';
                        aux->dias = -1;
                        pMstr[i].contD -=1;
                        pMstr[i].contS +=1;
                        novosS[i]+=1;
                    }
                }
            }
            aux = aux->prox;
        }
        *totalS+=pMstr[i].contS;
        *totalD+=pMstr[i].contD;
        *totalI+=pMstr[i].contI;
    }
    *dia+=1;
}

// Função de compilação de dados para o console
void reportGen(int *totalL, int ultDia){
    pmaster pMstr1, pMstr2;
    ppessoa aux1, aux2;
    char opRec1[15] = "recover000.bin";
    char opRec2[15] = "recover000.bin";
    int i,j,add;
    int d = 0, *dia = &d;
    int totalPes = 0,*tp;tp=&totalPes;
    int totalSau = 0,*ts;ts=&totalSau;
    int totalDoe = 0,*td;td=&totalDoe;
    int totalImu = 0,*ti;ti=&totalImu;
    int nS[6] = {0}, nD[6] = {0}, nI[6] = {0};
    int totalPes2 = 0,*tp2;tp2=&totalPes2;
    int totalSau2 = 0,*ts2;ts2=&totalSau2;
    int totalDoe2 = 0,*td2;td2=&totalDoe2;
    int totalImu2 = 0,*ti2;ti2=&totalImu2;
    int nS2[6] = {0}, nD2[6] = {0}, nI2[6] = {0};
    int novosST=0, novosDT=0, novosIT=0;
    int capacidadeTotal;
    float txS=0, txD=0, txI=0;
    float txST=0, txDT=0, txIT=0,     txC=0;
    float mdTxS=0, mdTxD=0, mdTxI=0,   mdTxC=0;
    for (d = 1; d <= ultDia; d++) {
        //Duas estruturas de locais e pessoas simultâneas (para análise das variações dia a dia)
        //Sempre dia atual e anterior.
        d--;
        fileNameGen(dia,opRec1);
        pMstr1 = leFicheirosMaster(opRec1,totalL,tp,nS,nD,nI,ts,td,ti);
        d++;
        fileNameGen(dia,opRec2);
        pMstr2 = leFicheirosMaster(opRec2,totalL,tp2,nS2,nD2,nI2,ts2,td2,ti2);
        if(d == 1){
            printf("\n\n\n\n\n\t****----***---**--*------------------------------- SIMULAÇÃO - RELATÓRIO FINAL -------------------------------*--**---***----****\n\n\n");
            printf("\tINFORMAÇÕES INICIAIS\n\n");
            printf("\tEspaço analisado: ");
            if(*totalL == 4) printf("Espaço 1 (Número de locais: 4 / Capacidade total: 200 pessoas)\t\t\t\tNúmero de dias: %d\n\n", ultDia);
            if(*totalL == 5) printf("Espaço 2 (Número de locais: 5 / Capacidade total: 125 pessoas)\t\t\t\tNúmero de dias: %d\n\n", ultDia);
            if(*totalL == 6) printf("Espaço 3 (Número de locais: 6 / Capacidade total:  92 pessoas)\t\t\t\tNúmero de dias: %d\n\n", ultDia);
            printf("\tPopulação analisada: ");
            if(*tp == 10) printf("População A (Número de Pessoas: %d)\n", *tp);
            if(*tp == 50) printf("População B (Número de Pessoas: %d)\n", *tp);
            if(*tp == 100) printf("População C (Número de Pessoas: %d)\n", *tp);
            if(*tp != 10 && *tp != 50 && *tp != 100) printf("População customizada (Número de Pessoas: %d)\n", *tp);
            printf("\t---------------------------------------------------------------------------------------------------------------------------------\n");
            printf("\tDISTRIBUIÇÃO INICIAL\n\n");
            printf("\tTotal de Locais: %d\t\t\tTotal de Pessoas: %d\tPessoas Saudáveis: %d\tPessoas Doentes: %d\tPessoas Imunes: %d\n\n",*totalL,*tp,*ts,*td,*ti);
//            escreve_espaco(pMstr1,totalL);

            for (j = 0; j < *totalL; j++) {
                printf("\tLocal ");
                printf("Id: %2d\t", pMstr1[j].id);
                printf("Capacidade: %d\t", pMstr1[j].capacidade);
                for (i = 0; i < 3; i++) {
                    if(pMstr1[j].lig[i] != -1) printf("Lig: %d\t", pMstr1[j].lig[i]);
                    else{printf("\t");}
                }
                printf("Saudáveis: %i\t", pMstr1[j].contS);
                printf("Doentes: %i\t", pMstr1[j].contD);
                printf("Imunes: %i\t", pMstr1[j].contI);
                printf("Total de Pessoas: %i\n", pMstr1[j].contT);
                aux1 = pMstr1[j].pp;
                while (aux1) {
                    printf("\n\tNome: %-30s\t\tIdade: %2d\t\t\t\t Estado: %c\t\t\t", aux1->nome, aux1->idade, aux1->estado);
                    if(aux1->dias >= 0) printf("Dias:%2d" , aux1->dias);
                    aux1 = aux1->prox;
                }
                printf("\n\n");
            }
        }
        novosST=0, novosDT=0, novosIT=0;
        printf("\t***----------------------------------------------------  RELATÓRIO DIÁRIO  ---------------------------------------------------***\n\tSITUAÇÃO ATUAL\t\t\tTotal de Pessoas: %d\t\t\t\t\t\t\tDia: %d\n\tPessoas Saudáveis: %d\t\tPessoas Imunes: %d\t\tPessoas Doentes: %d\n",*tp2,d,*ts2,*ti2,*td2);
        printf("\t\t\t\t\tNúmero de Pessoas\t    Saudáveis\t\t      Imunes\t\t\tDoentes\n");
        for (i = 0; i < *totalL; i++){
            printf("\tLocal id:%2d\t\t\t\t%2d\t\t\t%2d\t\t\t%2d\t\t\t  %2d\n",pMstr2[i].id,pMstr2[i].contT,pMstr2[i].contS,pMstr2[i].contI,pMstr2[i].contD);
        }
        printf("\t---------------------------------------------------------- NOVOS CASOS ----------------------------------------------------------\n");
        printf("\t\t\t\t\tPessoas Curadas\t   Curados Não Imunizados    Curados Imunizados     Pessoas Infectadas\n");
        for (i = 0; i < *totalL; i++) {
            //Análise por local feita relacionando novos casos (S D I) aos números (S D I) do dia anterior
            novosST+=nS2[i];
            novosDT+=nD2[i];
            novosIT+=nI2[i];
            txS =(float)nS2[i]*100 / (float)pMstr1[i].contS;
            txD =(float)nD2[i]*100 / (float)pMstr1[i].contD;
            txI =(float)nI2[i]*100 / (float)pMstr1[i].contI;
            txC= (float)(nS2[i] + nI2[i])*100 / (float)(pMstr1[i].contS + pMstr1[i].contI);
            printf("\tLocal id:%2d\t\t %9d (%6.2f %%)\t%9d (%6.2f %%)\t%9d (%6.2f %%)\t%9d (%6.2f %%)\n",pMstr2[i].id,  (nS2[i]+nI2[i]),txC,  nS2[i],txS,  nI2[i],txI,  nD2[i],txD);
        }
            //Análise total
        txST = (float)novosST*100/(float)*ts;
        txDT = (float)novosDT*100/(float)*td;
        txIT = (float)novosIT*100/(float)*ti;
        txC= (float)(novosST + novosIT)*100 / (float)(*ts + *ti);
        mdTxS = (mdTxS * (float)(d-1) + txST)/(float)d;
        mdTxD = (mdTxD * (float)(d-1) + txDT)/(float)d;
        mdTxI = (mdTxI * (float)(d-1) + txIT)/(float)d;
        mdTxC = (mdTxC * (float)(d-1) + txC)/(float)d;
        printf("\tTotal--->\t\t %9d (%6.2f %%)\t%9d (%6.2f %%)\t%9d (%6.2f %%)\t%9d (%6.2f %%)\n\n",(novosST+novosIT),txC,  novosST,txST,  novosIT,txIT,  novosDT,txDT);
        printf("\t------------------------------------------------------  TAXAS ACUMULADAS  -------------------------------------------------------\n");
        printf("\tTaxa proliferação: %6.2f %%                                                                 (Média do período entre o dia 1 e %d)\n",mdTxD,d);
        printf("\tTaxa curados: %6.2f %%\n",mdTxC);
        printf("\tTaxa curados não imunizados: %6.2f %%\n",mdTxS);
        printf("\tTaxa curados imunizados: %6.2f %%",mdTxI);
        for (i = 0; i < *totalL; i++){
            if(pMstr2[i].contT > pMstr1[i].contT){
                printf("\t\t\t\tAtenção, houve adição ou tranferência de pessoa(s) neste dia");
            }
        }
        printf("\n\n");
    }
    printf("\t-----------------------------------------------  FIM - DADOS COMPILADOS COM SUCESSO  --------------------------------------------\n\n\n");
}

// Função de compilação de dados para ficheiro report txt
void reportTXTGen(int *totalL, int ultDia){
    FILE *f1;
    f1=fopen("Report.txt","w");
    if(f1==NULL){
        printf("Erro ao criar ficheiro\n");
        return;
    }
    pmaster pMstr1, pMstr2;
    ppessoa aux1, aux2;
    char opRec1[15] = "recover000.bin";
    char opRec2[15] = "recover000.bin";
    int i,j,add;
    int d = 0, *dia = &d;
    int totalPes = 0,*tp;tp=&totalPes;
    int totalSau = 0,*ts;ts=&totalSau;
    int totalDoe = 0,*td;td=&totalDoe;
    int totalImu = 0,*ti;ti=&totalImu;
    int nS[6] = {0}, nD[6] = {0}, nI[6] = {0};
    int totalPes2 = 0,*tp2;tp2=&totalPes2;
    int totalSau2 = 0,*ts2;ts2=&totalSau2;
    int totalDoe2 = 0,*td2;td2=&totalDoe2;
    int totalImu2 = 0,*ti2;ti2=&totalImu2;
    int nS2[6] = {0}, nD2[6] = {0}, nI2[6] = {0};
    int novosST=0, novosDT=0, novosIT=0;
    int capacidadeTotal;
    float txS=0, txD=0, txI=0;
    float txST=0, txDT=0, txIT=0,     txC=0;
    float mdTxS=0, mdTxD=0, mdTxI=0,   mdTxC=0;

    for (d = 1; d <= ultDia; d++) {
        d--;
        fileNameGen(dia,opRec1);
        pMstr1 = leFicheirosMaster(opRec1,totalL,tp,nS,nD,nI,ts,td,ti);
        d++;
        fileNameGen(dia,opRec2);
        pMstr2 = leFicheirosMaster(opRec2,totalL,tp2,nS2,nD2,nI2,ts2,td2,ti2);
        if(d == 1) {
            fprintf(f1,"\n\n\n\t*-------------------------------------  ISEC - INSTITUTO SUPERIOR DE ENGENHARIA DE COIMBRA  ------------------------------------*\n");
            fprintf(f1,"\n\t                                                              DEIS\n");
            fprintf(f1,"\t                                        DEPARTAMENTO DE ENGENHARIA INFORMÁTICA E DE SISTEMAS \n");
            fprintf(f1,"\t                               Prof: Dr.Francisco José Baptista Pereira  -  Disciplina: Programação \n\n");
            fprintf(f1,"\t                                                   SIMULADOR DE PROPAGAÇÃO VIRAL \n\n");
            fprintf(f1,"\n\t****----***---**--*------------------------------- SIMULAÇÃO - RELATÓRIO FINAL -------------------------------*--**---***----****\n\n\n");
            fprintf(f1, "\tINFORMAÇÕES INICIAIS\n\n");
            fprintf(f1, "\tEspaço analisado: ");
            if (*totalL == 4)
                fprintf(f1,"Espaço 1 (Número de locais: 4 / Capacidade total: 200 pessoas)\t\t\t  Número de dias: %d\n\n",ultDia);
            if (*totalL == 5)
                fprintf(f1,"Espaço 2 (Número de locais: 5 / Capacidade total: 125 pessoas)\t\t\t  Número de dias: %d\n\n",ultDia);
            if (*totalL == 6)
                fprintf(f1,"Espaço 3 (Número de locais: 6 / Capacidade total:  92 pessoas)\t\t\t  Número de dias: %d\n\n",ultDia);
            fprintf(f1, "\tPopulação analisada: ");
            if (*tp == 10) fprintf(f1, "População A (Número de Pessoas: %d)\n", *tp);
            if (*tp == 50) fprintf(f1, "População B (Número de Pessoas: %d)\n", *tp);
            if (*tp == 100)fprintf(f1, "População C (Número de Pessoas: %d)\n", *tp);
            if (*tp != 10 && *tp != 50 && *tp != 100)
                fprintf(f1, "População customizada (Número de Pessoas: %d)\n", *tp);
            fprintf(f1,"\t---------------------------------------------------------------------------------------------------------------------------------\n");
            fprintf(f1, "\tDISTRIBUIÇÃO INICIAL\n\n");
            fprintf(f1,"\tTotal de Locais: %d\t\tTotal de Pessoas: %d\tPessoas Saudáveis: %d\tPessoas Doentes: %d\tPessoas Imunes: %d\n\n",*totalL, *tp, *ts, *td, *ti);

            for (j = 0; j < *totalL; j++) {
                fprintf(f1, "\tLocal ");
                fprintf(f1, "Id: %2d\t", pMstr1[j].id);
                fprintf(f1, "Capacidade: %d", pMstr1[j].capacidade);
                for (i = 0; i < 3; i++) {
                    if (pMstr1[j].lig[i] != -1) fprintf(f1, "\tLig: %d", pMstr1[j].lig[i]);
                    else { fprintf(f1, "\t"); }
                }
                fprintf(f1, "\tSaudáveis: %i\t", pMstr1[j].contS);
                fprintf(f1, "Doentes: %i\t", pMstr1[j].contD);
                fprintf(f1, "Imunes: %i\t", pMstr1[j].contI);
                fprintf(f1, "Total de Pessoas: %i\n", pMstr1[j].contT);
                aux1 = pMstr1[j].pp;
                while (aux1) {
                    fprintf(f1, "\n\tNome: %-30s\t\tIdade: %2d\t\t\t\t Estado: %c\t\t\t", aux1->nome, aux1->idade,
                            aux1->estado, aux1->dias);
                    if (aux1->dias >= 0) fprintf(f1, "Dias:%2d", aux1->dias);
                    aux1 = aux1->prox;
                }
                fprintf(f1, "\n\n");
            }
        }
        novosST=0, novosDT=0, novosIT=0;
        fprintf(f1,"\t***----------------------------------------------------  RELATÓRIO DIÁRIO  ---------------------------------------------------***\n\tSITUAÇÃO ATUAL\t\t\tTotal de Pessoas: %d\t\t\t\t\t\t\t\tDia: %d\n\tPessoas Saudáveis: %d\t\t\tPessoas Imunes: %d\t\t\tPessoas Doentes: %d\n",*tp2,d,*ts2,*ti2,*td2);
        fprintf(f1,"\t\t\t\t\tNúmero de Pessoas\t   Saudáveis\t\t    Imunes\t\t    Doentes\n");
        for (i = 0; i < *totalL; i++){
            fprintf(f1,"\tLocal id:%2d\t\t\t\t%2d\t\t      %2d\t\t      %2d\t\t      %2d\n",pMstr2[i].id,pMstr2[i].contT,pMstr2[i].contS,pMstr2[i].contI,pMstr2[i].contD);
        }
        fprintf(f1,"\t---------------------------------------------------------- NOVOS CASOS ----------------------------------------------------------\n");
        fprintf(f1,"\t\t\t\t\t       Pessoas Curadas     Curados Não Imunizados    Curados Imunizados      Pessoas Infectadas\n");
        for (i = 0; i < *totalL; i++) {
            novosST+=nS2[i];
            novosDT+=nD2[i];
            novosIT+=nI2[i];
            txS =(float)nS2[i]*100 / (float)pMstr1[i].contS;
            txD =(float)nD2[i]*100 / (float)pMstr1[i].contD;
            txI =(float)nI2[i]*100 / (float)pMstr1[i].contI;
            txC= (float)(nS2[i] + nI2[i])*100 / (float)(pMstr1[i].contS + pMstr1[i].contI);
            fprintf(f1,"\tLocal id:%2d\t\t\t %9d (%6.2f %%)\t%9d (%6.2f %%)\t%9d (%6.2f %%)\t%9d (%6.2f %%)\n",pMstr2[i].id,  (nS2[i]+nI2[i]),txC,  nS2[i],txS,  nI2[i],txI,  nD2[i],txD);
        }
        txST = (float)novosST*100/(float)*ts;
        txDT = (float)novosDT*100/(float)*td;
        txIT = (float)novosIT*100/(float)*ti;
        txC= (float)(novosST + novosIT)*100 / (float)(*ts + *ti);
        mdTxS = (mdTxS * (float)(d-1) + txST)/(float)d;
        mdTxD = (mdTxD * (float)(d-1) + txDT)/(float)d;
        mdTxI = (mdTxI * (float)(d-1) + txIT)/(float)d;
        mdTxC = (mdTxC * (float)(d-1) + txC)/(float)d;
        fprintf(f1,"\tTotal--->\t\t\t %9d (%6.2f %%)\t%9d (%6.2f %%)\t%9d (%6.2f %%)\t%9d (%6.2f %%)\n\n",(novosST+novosIT),txC,  novosST,txST,  novosIT,txIT,  novosDT,txDT);
        fprintf(f1,"\t------------------------------------------------------  TAXAS ACUMULADAS  -------------------------------------------------------\n");
        fprintf(f1,"\tTaxa proliferação: %6.2f %%                                                                 (Média do período entre o dia 1 e %d)\n",mdTxD,d);
        fprintf(f1,"\tTaxa curados: %6.2f %%\n",mdTxC);
        fprintf(f1,"\tTaxa curados não imunizados: %6.2f %%\n",mdTxS);
        fprintf(f1,"\tTaxa curados imunizados: %6.2f %%",mdTxI);
        for (i = 0; i < *totalL; i++){
            if(pMstr2[i].contT > pMstr1[i].contT){
                fprintf(f1,"\t\t\t\tAtenção, houve adição ou tranferência de pessoa(s) neste dia");
            }
        }
        fprintf(f1,"\n\n");
    }
    fprintf(f1,"\t-----------------------------------------------  FIM - DADOS COMPILADOS COM SUCESSO  --------------------------------------------\n\n");
    fprintf(f1,"\t                                            Desenvolvido por Wilson Hermes Jacoud Junior \n\n");
    fprintf(f1,"\t----------------------------------------------------------  ISEC - 2020  --------------------------------------------------------\n\n\n");
    fclose(f1);
    printf("\n\nFicheiro Report.txt gerado com sucesso.\n\n\n");
}

// Utils

void initRandom(){
    srand(time(NULL));
}

int intUniformRnd(int a, int b){
    return a + rand()%(b-a+1);
}

int probEvento(float prob){
    return prob > ((float)rand()/RAND_MAX);
}
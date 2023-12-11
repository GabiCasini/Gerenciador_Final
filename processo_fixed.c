#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conversoes.c"

/*
IMPLEMENTAR:
-swapper (funcoes.c) -- raio
-halt (funcoes.c) 
-add printf por todo o codigo para a querida nao ficar perdida
-depois disso tudo, basta ajustar leitura de arquivo, funcao flags e main()

//oq acontece se a ms tiver cheia???
*/


typedef struct processo { //tabela de paginas do processo fica restritamente na MP
    char *identificador;
    char *estado_processo;
    int tam_imagem;
    int tam_pagina;
    int tam_end_logico; //botei aq pra facilitar
    struct processo *prox;
} P;

typedef struct conteudo_tp{
    int bit_p;
    int bit_m;
    int numero_qd;
    struct conteudo_tp *prox;
} ROW;

typedef struct tabela_paginas {
    char *id; //id do processo associado
    ROW *rows; //linhas da tabela
    struct tabela_paginas *prox; //cada processo tera uma tp associada e essas estarao localizadas na MP
} TP;

typedef struct least_recently_used {
    int numero_qd; //seria mais facil aqui trabalhar com o numero do quadro e abstrair o offset
    struct least_recently_used *prox;
} LRU;

typedef struct memoria_principal {
    int tam_mp;
    int num_quadros;
    LRU *fila_lru;
    TP *tabela_paginas;
    int qd[]; 
} MP;

typedef struct memoria_secundaria {
    int tam_ms;
    P *processos;
} MS;

//declaracoes de funcao
MP *inicializa_mp(int tam_mf, int tam_qm);
MS *inicializa_ms(int tamanho);
P *novo_processo(MP* mp, MS *ms, char *nome_processo, int tam_processo, int tam_pag, int tam_end);

void add_tp(P *processo, MP* mp, int size_pag);
ROW *insere_final(ROW *l);
ROW* busca_linha(int pag, MP* mp, P* proc);
P *busca_processo(MS *m_sec, char *nome_processo);
void busca_pagina(P* proc, MP* mp, int pag);
void carrega_pag_mp(P* processo, MP *m_principal, int pag);

int LRU_cheia(LRU *fila, int tam_max);
LRU *insere_fila(LRU *fila, MP* mp);
LRU *retira_fila(LRU *fila, MP* mp, P* proc);
void moverParaFinal(LRU *lista, int val);

int quadro_livre(int qd[], int num_qd);

void halt(char *nome_processo, MP *m_princ, MS *m_sec);

void escrever(P* proc, MP* mp, int end);
void atualizar_ms();
void retira_row(int pag, MP* mp, P* proc);
void add_row(int pag, MP* mp, P* proc, int qd);

// --------------------- ACABA ARQUIVO funcoes_fixed.h

void imprime_processo(P *proc){
    printf("Informações sobre o processo %s: \n", proc->identificador);
    printf("---------------------------------\n");
    printf("Estado do processo: %s \n", proc->estado_processo);
    printf("Tamanho da imagem: %d \n", proc->tam_imagem);
    printf("Tamanho da página: %d \n", proc->tam_pagina);
    printf("---------------------------------\n");
}

MP *inicializa_mp(int tam_mf, int tam_qm){

    MP *novo = (MP*)malloc(sizeof(MP));

    novo->tam_mp = tam_mf;
    novo->num_quadros = tam_mf / tam_qm;
    novo->fila_lru = NULL;
    novo->tabela_paginas = NULL;
    novo->qd[tam_mf/tam_qm]; //assim q inicializa c tam_max? ajuda

    return novo;
}

MS *inicializa_ms(int tamanho){
    MS *novo = (MS*)malloc(sizeof(MS));
    novo->tam_ms = tamanho;
    novo->processos = NULL;

    return novo;
}

P *novo_processo(MP* mp, MS *ms, char *nome_processo, int tam_processo, int tam_pag, int tam_end){

    //nao estou verificando se o processo ja existe porque acredito no bom senso das pessoas q vao escrever o arquivo de instrucoes de entrada

    P *novo = (P*)malloc(sizeof(P));
    novo->identificador = nome_processo; 
    novo->estado_processo = "Novo";
    novo->tam_imagem = tam_processo;
    novo->tam_pagina = tam_pag;
    novo->tam_end_logico = tam_end;
    novo->prox = ms->processos;
    ms->processos = novo;

    //adicionando tp desse processo na mp
    add_tp(novo, mp, tam_pag);

    printf("O processo %s foi adicionado na MS com sucesso! Obs.: Sua TP associada ja foi iniciada na MP.\n", nome_processo);

    //carregar_pg_0
    busca_pagina(novo, mp, 0);

    /* LRU_cheia(mp->fila_lru, mp->tam_mp);
    if(){
        
        mp->fila_lru = insere_fila(mp->fila_lru, 0);
        add_row(int pag, MP* mp, P* proc, int qd);
    else{
        
    }*/
    

    return novo;
}

P *busca_processo(MS *m_sec, char *nome_processo){
    P *aux = m_sec->processos;
    if(!aux) return NULL;
    while(aux){
        if(strcmp(aux->identificador, nome_processo) == 0) return aux;
        else aux = aux->prox;
    }
    return NULL;
}

void halte(char *nome_processo, MP *mp, MS *ms){ 
    TP* tp = mp->tabela_paginas, *ant, *tmp;

    while (strcmp(tp->id, nome_processo) != 0){
        ant = tp;
        tp = tp->prox; 
        tmp = tp->prox; 
    }
    ROW* linha = tp->rows;

    while(linha){
        if (linha->bit_p == 1){
            printf("retira da fila rapa\n");
        }
        linha = linha->prox;
        free(linha);
    }

    //ant->prox = tmp; //essa linha ta dando errado pq preciso verificar antes se tem uma tabela só
    free(tp);
}

/*
void retira_qd_fila(LRU *fila, int num_qd){
    LRU *aux = fila, *ant = NULL, *tmp;
    while(aux){
        if(aux->numero_qd == num_qd){
            tmp = aux;
            ant->prox = aux->prox;
            aux = aux->prox;
            free(tmp);
            break;
        }
        ant = aux;
        aux = aux->prox;
    }
}

void halt(char *nome_processo, MP *m_princ, MS *m_sec){
    //Consertar o halt

    // liberando as linhas da tp e da fila
    TP *aux_tp = m_princ->tabela_paginas;
    TP *ant, *tmp_tp;
    //printf("oiiiiii \n");
    char *nome = nome_processo;
    //printf("%d",strcmp(aux_tp->id, nome));
    // 
    while(aux_tp){
        if(strcmp(aux_tp->id, nome) == 0){
            tmp_tp = aux_tp;
            printf("entrou aq \n");
            ant->prox = aux_tp->prox;
            aux_tp = aux_tp->prox;
            break;
        }
        ant = aux_tp;
        aux_tp = aux_tp->prox;
        printf("1 \n");
    } //
    //aux_tp->id != nome_processo
    printf("%s",aux_tp->id);
    while(strcmp(aux_tp->id, nome) == 0){   
        tmp_tp = aux_tp;
        ant = aux_tp;
        aux_tp = aux_tp->prox;
        printf("1 \n");
    }
    tmp_tp = aux_tp;
    printf("entrou aq \n");
    ant->prox = aux_tp->prox;
    aux_tp = aux_tp->prox;
    
    ROW *aux_row = aux_tp->rows, *tmp_row;
    printf("miau \n");
    while(aux_row){
        // testar aqui dps
        if (aux_row->bit_p == 1){
            printf("2 \n");
            retira_qd_fila(m_princ->fila_lru, aux_row->numero_qd);
            printf("3 \n");
        }
        printf("%d\n", aux_row->numero_qd);
        tmp_row = aux_row;
        aux_row = aux_row->prox;
        free(tmp_row);
    }
    printf("234 \n");
    // liberando a tabela de paginas
    free(tmp_tp);
    
    // liberando a memoria secundaria
    P *aux_ms = m_sec->processos, *ant_ms = NULL, *tmp_ms;
    while(aux_ms){
        printf("aiaiaiaaiai \n");
        if(strcmp(aux_ms->identificador, nome_processo) == 0){
            printf("lallalaalalal \n");
            tmp_ms = aux_ms;
            ant_ms->prox = aux_ms->prox;
            aux_ms = aux_ms->prox;
            free(tmp_ms);
            break;
            printf("miauuuuu \n");
        }
        ant_ms = aux_ms;
        aux_ms = aux_ms->prox;
    }
    printf("Processo %s encerrado com sucesso. \n", nome_processo);
}
*/

void add_tp(P *processo, MP* mp, int size_pag){ //adiciona na memoria principal

	int num_paginas = (processo->tam_imagem)/size_pag;
    TP* novo = (TP*)malloc(sizeof(TP));

    if (mp->tabela_paginas){

        TP* p = mp->tabela_paginas;

        while (p->prox){
            p = p->prox;
        }

        p->prox = novo;
        p->prox->id = processo->identificador;
        for(int i = 0; i < num_paginas; i++){
            p->prox->rows = insere_final(mp->tabela_paginas->rows);
	    }
        p->prox->prox = NULL;
    }

    mp->tabela_paginas = novo;
    mp->tabela_paginas->id = processo->identificador;
	for(int i = 0; i < num_paginas; i++){
        mp->tabela_paginas->rows = insere_final(mp->tabela_paginas->rows);
	}
    mp->tabela_paginas->prox = NULL;
}

ROW *insere_final(ROW *l){

    ROW *novo = (ROW*)malloc(sizeof(ROW));
    novo->bit_m = 0;
    novo->bit_p = 0;
    novo->numero_qd = -1;
    novo->prox = NULL;

   if (l){
        ROW* p = l;

        while (p->prox != NULL){
            p = p->prox;
        }

        p->prox = novo;
        return l;
    }

    return l = novo;
}

LRU *insere_fila(LRU *fila, MP* mp){

    int i = quadro_livre(mp->qd, mp->num_quadros);

    // se o elemento nao esta na fila, entao vamos inseri-lo
    LRU *novo = (LRU*)malloc(sizeof(LRU));
    novo->numero_qd = i;

    if(!fila) return novo;

    LRU *aux = fila;
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = novo;
    novo->prox = NULL;

    return fila;
}

int LRU_cheia(LRU *fila, int tam_max){
    int cheia = 1;
    LRU *aux = fila;
    for(int c = 0; c<tam_max; c++){
        if(aux == NULL){
            cheia = 0;
            break;
        }
        aux = aux->prox;
    }
    return cheia;
}

LRU *retira_fila(LRU *fila, MP* mp, P* proc){

    if (fila){
        int k = fila->numero_qd;
        retira_row(k, mp, proc);
        mp->qd[k] = 0;

        LRU *aux = fila;
        fila = fila->prox;
        free(aux);
    }
    return fila;
}

void moverParaFinal(LRU *lista, int val){ //quando for chamado e resetar a prioridade
    LRU *aux = lista;
    LRU *ant = NULL;

    // Procurar pelo LRU com numero_qd == val
    while (aux != NULL && aux->numero_qd != val) {
        ant = aux;
        aux = aux->prox;
    }

    // Se o LRU com numero_qd == val for encontrado
    if (aux != NULL) {
        // Se já estiver no final da lista, não há nada a fazer
        if (aux->prox == NULL) {
            printf("LRU com numero_qd = %d já esta no final da lista.\n", val);
            return;
        }

        // Remover o LRU da sua posição atual
        if (ant != NULL) {
            ant->prox = aux->prox;
        } else {
            lista = aux->prox;
        }

        // Encontrar o último elemento na lista
        LRU *ultimo = lista;
        while (ultimo->prox != NULL) {
            ultimo = ultimo->prox;
        }

        // Colocar o LRU removido no final da lista
        ultimo->prox = aux;
        aux->prox = NULL;

        printf("LRU com numero_qd = %d movido para o final da lista.\n", val);
    } else {
        printf("LRU com numero_qd = %d nao encontrado na lista.\n", val);
    }
}

ROW* busca_linha(int pag, MP* mp, P* proc){

    TP *tab_paginas = mp->tabela_paginas;

    while (tab_paginas->id != proc->identificador){
        tab_paginas = tab_paginas->prox; //encontra a tp do processo
    }

    int i = 0;
    while (i < pag){
        tab_paginas->rows = tab_paginas->rows->prox;
        i++;
    }

    return tab_paginas->rows;
}

void escrever(P* proc, MP* mp, int end){

    int pag = n_pag (end, proc->tam_pagina, proc->tam_end_logico);
    ROW* linha = busca_linha(pag, mp, proc);
    linha->bit_m = 1;
}

//if bit_m == 1 na hora de tirar da fila √
void atualizar_ms(){
    printf("Copiando o conteudo da página para a Memória Secundária...");
    printf("MS atualizada com sucesso!");
}

void retira_row(int qd, MP* mp, P* proc){

    TP *tab_paginas = mp->tabela_paginas;

    while (tab_paginas->id != proc->identificador){
        tab_paginas = tab_paginas->prox; //encontra a tp do processo
    }

    ROW *linha = tab_paginas->rows;

    while (linha->numero_qd != qd){
        linha = linha->prox;
    }

    if (linha->bit_m == 1){
        atualizar_ms();
    }

    linha->bit_p = 0;
    linha->bit_m = 0;
    linha->numero_qd = -1;
}

void add_row(int pag, MP* mp, P* proc, int qd){
    ROW* linha = busca_linha(pag, mp, proc);
    linha->bit_p = 1;
    linha->numero_qd = qd;
}

int quadro_livre(int qd[], int num_qd){
    for (int i = 0; i < num_qd; i++){
        if (qd[i] != 1){
            qd[i] = 1;
            return i;
        }
    } 
    return -1; 
}


void carrega_pag_mp(P* processo, MP *m_principal, int pag){

    //processo esta esperando (Novo ou bloqueado-suspenso)

    if (!LRU_cheia(m_principal->fila_lru, m_principal->tam_mp)){ //Caso 1: MP nao esta cheia
        int num_quadro = quadro_livre(m_principal->qd, m_principal->num_quadros);
        m_principal->fila_lru = insere_fila(m_principal->fila_lru, m_principal);
        add_row(pag, m_principal, processo, num_quadro);
    }

    else{ //Caso 2: A MP esta cheia
        printf("A MP esta cheia, useremos a política de substituição LRU para carregar a página na MP\n");
        m_principal->fila_lru = retira_fila(m_principal->fila_lru, m_principal, processo); // retira_row() ta aq dentro
        m_principal->fila_lru = insere_fila(m_principal->fila_lru, m_principal);
        int num_quadro = quadro_livre(m_principal->qd, m_principal->num_quadros);
        add_row(pag, m_principal, processo, num_quadro); 

    }

    printf("A página da imagem do processo %s foi carregada para a MP com sucesso\n", processo->identificador);
    //processo->estado_processo = 'Pronto'; //da 3 warnings aq
    printf("O processo passou para o estado 'Pronto'\n");
}

void busca_pagina(P* proc, MP* mp, int pag){

    ROW* linha = busca_linha(pag, mp, proc);

    if (linha->bit_p == 1){
        printf("Página buscada já está na MP\n"); //pode fazer instrucao, escrita e leitura
        LRU *fila = mp->fila_lru;
        moverParaFinal(fila, linha->numero_qd); //traz pag pro topo da fila (mais recentemente usado)
    }
    else{
        printf("Page fault\n");
        carrega_pag_mp(proc, mp, pag);
    }
}

/*
int main(){ //testes

    MP* mp = inicializa_mp(10, 1); //(tam_mp, tam_qd)
    MS* ms = inicializa_ms(20); //(tam_ms)
    P* proc = novo_processo(mp, ms, "P1", 5, 1, 4); //(tam_P1, tam_pag) -- teremos 5 paginas.

    return 0;
}
*/

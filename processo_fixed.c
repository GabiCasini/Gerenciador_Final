#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gerenciador.h"
#include "conversoes.c"

//oq acontece se a ms tiver cheia??

void imprime_processo(P *proc){
    printf("\nInformações sobre o processo %s: \n", proc->identificador);
    printf("---------------------------------\n");
    printf("Estado do processo: %s \n", proc->estado_processo);
    printf("Tamanho da imagem: %d \n", proc->tam_imagem);
    printf("Tamanho da página: %d \n", proc->tam_pagina);
    printf("Numero de páginas: %d \n", (proc->tam_imagem/proc->tam_pagina));
    printf("---------------------------------\n");
}

void ver_processo(P *proc){
    printf("Processo %s: \n", proc->identificador);
    printf("-----------------\n");
    printf("Estado: %s \n", proc->estado_processo);
    printf("Tamanho da imagem: %d \n", proc->tam_imagem);
    printf("Tamanho da página: %d \n", proc->tam_pagina);
    printf("-----------------\n");
}

void ver_ms(MS *m_sec){
    P *aux = m_sec->processos;
    printf("Informações sobre a Memória Secundária: \n");
    printf("----------------------------------------\n");
    printf("Tamanho da memória secundária: %d \n", m_sec->tam_ms);
    while(aux){
        ver_processo(aux);
        aux = aux->prox;
    }
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
    if(strcmp(nome_processo, "P1") == 0) novo->id_numerico = 1;
    else if(strcmp(nome_processo, "P2") == 0) novo->id_numerico = 2;
    else if(strcmp(nome_processo, "P3") == 0) novo->id_numerico = 3;
    novo->estado_processo = "Novo";
    novo->tam_imagem = tam_processo;
    novo->tam_pagina = tam_pag;
    novo->tam_end_logico = tam_end;

    //novo->prox = ms->processos;
    //ms->processos = novo;
    
    //adicionando tp desse processo na mp
    mp->tabela_paginas = add_tp(novo, mp, tam_pag);
    printf("Tabela de páginas: \n");
    TP *aux_tpp = mp->tabela_paginas;
    while(aux_tpp){
        printf("id numerico do processo: %d", aux_tpp->id_processo);
        printf("id string do processo: %s \n", aux_tpp->id);
        aux_tpp = aux_tpp->prox;
    }
    printf("\n");
    /*
    printf("tp:%s ", mp->tabela_paginas->id);
    if (mp->tabela_paginas->prox){
        printf("tp:%s ", mp->tabela_paginas->prox->id);
    }
    */
    novo->prox = ms->processos;
    ms->processos = novo;
    printf("O processo %s foi adicionado na MS com sucesso! Obs.: Sua TP associada ja foi iniciada na MP.\n", nome_processo);

    imprime_processo(novo);

    //carregar_pg_0
    busca_pagina(novo, mp, 0);

    imprime_processo(novo);
    
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


void retira_qd_fila(LRU *fila, int num_qd){
    printf("entrou em retira fila");
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
    printf("saiu de retira fila");
}

void halt(char *nome_processo, MP *m_princ, MS *m_sec){

    // liberando as linhas da tp e da fila
    TP *aux_tp = m_princ->tabela_paginas, *ant, *tmp_tp;
    char *nome = nome_processo;
    
    // aqui checa se o que queremos é o primeiro e se existe mais alguém além dele
    if(strcmp(aux_tp->id, nome) == 0 && aux_tp->prox){
        tmp_tp = aux_tp;
        aux_tp = aux_tp->prox;
    }

    // aqui checa se o que queremos é o primeiro e se só tem ele na tabela
    else if(strcmp(aux_tp->id, nome) == 0 && !aux_tp->prox){
        tmp_tp = aux_tp;
    }
    
    else{ //testar
        while(aux_tp){
            if(strcmp(aux_tp->id, nome) == 0){
                tmp_tp = aux_tp;
                ant->prox = aux_tp->prox;
                aux_tp = aux_tp->prox;
                break;
            }
            ant = aux_tp;
            aux_tp = aux_tp->prox;
        }
    }

    ROW *aux_row = tmp_tp->rows, *tmp_row;

    while(aux_row){
        if (aux_row->bit_p == 1){
            //retira_qd_fila(m_princ->fila_lru, aux_row->numero_qd);
        }
        //printf("%d\n", aux_row->numero_qd);
        tmp_row = aux_row;
        aux_row = aux_row->prox;
        free(tmp_row); //certinho
    }

    // liberando a tabela de paginas
    free(tmp_tp);

    // liberando a memoria secundaria
    P *aux_ms = m_sec->processos, *ant_ms, *tmp_ms;

    if(strcmp(aux_ms->identificador, nome) == 0 && aux_ms->prox){
        tmp_ms = aux_ms;
        aux_ms = aux_ms->prox;
    }
    // aqui checa se o que queremos é o primeiro e se só tem ele na tabela
    else if(strcmp(aux_ms->identificador, nome) == 0 && !aux_ms->prox){
        tmp_ms = aux_ms;
    }
    else{
        while(aux_ms){
            if(strcmp(aux_ms->identificador, nome_processo) == 0){
                tmp_ms = aux_ms;
                ant_ms->prox = aux_ms->prox;
                aux_ms = aux_ms->prox;
                break;
            }
            ant_ms = aux_ms;
            aux_ms = aux_ms->prox;
        }
    }

    tmp_ms->estado_processo = "Finalizado";
    imprime_processo(tmp_ms);
    free(tmp_ms);

    printf("Processo %s encerrado com sucesso. \n", nome_processo);
}


TP* add_tp(P *processo, MP* mp, int size_pag){ //adiciona na memoria principal
   
	int num_paginas = (processo->tam_imagem)/size_pag;
    TP* novo = (TP*)malloc(sizeof(TP));
    char *nome = processo->identificador;

    printf("oiiii \n");
    /*
    if (mp->tabela_paginas){
        printf("lalalalalal \n");
        novo->prox = mp->tabela_paginas;
        mp->tabela_paginas = novo;
    }
    else{
        novo->prox = NULL; 
    }
    */
    printf("processo: %d \n", processo->id_numerico);
    printf("processo: %s \n", nome);
    novo->id = nome;
    //printf("ID adicionado na  TP: %s\n", novo->id);
    /*
    for(int i = 0; i < num_paginas; i++){
        printf("%d \n", i);
        novo->rows = insere_final(novo->rows);
    }

    return novo; 
    */

    /*
    TLSE *tlse_insere_inicio(TLSE *l, int x){
    TLSE *novo = (TLSE*)malloc(sizeof(TLSE));
    novo->info = x;
    novo->prox = l;
    return novo;
}*/

    //TP *novo = (TP*)malloc(sizeof(TP));
    //strcpy(novo->id, processo->identificador);
    printf("ID adicionado na  TP: %s\n", novo->id);

    for(int i = 0; i < num_paginas; i++){
        printf("%d \n", i);
        novo->rows = insere_final(novo->rows);
    }

    // ROW *aux_r = novo->rows;
    // printf("tabela de paginas: \n");
    // TP *auxx = novo;
    // while(auxx){
    //     printf("processo %d: \n", auxx->id_processo);
    //     printf("%s: \n", auxx->id);
    //     while(aux_r){
    //         printf("%d - ", aux_r->numero_qd);
    //         aux_r = aux_r->prox;
    //     }
    //     auxx = auxx->prox;
    //     printf("------------------\n");
    // }
    // printf("\n");
    novo->prox = mp->tabela_paginas;
    printf("saida \n");
    return novo;
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

    while (strcmp(tab_paginas->id, proc->identificador) != 0){
        tab_paginas = tab_paginas->prox; //encontra a tp do processo
        printf("aqio busca linha\n");
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

void swapper(P* processo, MP *m_principal, int pag){

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
    processo->estado_processo = "Pronto";
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
        swapper(proc, mp, pag);
    }
}

void ver_fila_lru(MP *memoria_principal){
    LRU *p;
    LRU *lst = memoria_principal->fila_lru;
    printf("Fila LRU: ");
    for (p = lst; p != NULL; p = p->prox) printf(" %d, ", p->numero_qd);
}

void ver_TP(MP *memoria_principal){
    TP *p = memoria_principal->tabela_paginas;
    printf("Tabela de Paginas: ");
    while (p){
        printf("%s ", p->id);
        p = p->prox;
    }
    //for (p = lst; p != NULL; p = p->prox) printf("%s ", p->id);
    
}
void ver_mp(MP *memoria_principal){
    printf("\nInformações sobre a Memoria Principal\n");
    printf("---------------------------------\n");
    printf("Tamanho da Memoria Principal: %d \n", memoria_principal->tam_mp);
    printf("Numero de quadros: %d \n", memoria_principal->num_quadros);
    ver_fila_lru(memoria_principal);
    printf("\n");
    ver_TP(memoria_principal);
    printf("\n---------------------------------\n\n");
}

/*typedef struct memoria_secundaria {
    int tam_ms;
    P *processos;
} MS;*/
/*
int main(){ //testes

    MP* mp = inicializa_mp(10, 1); //(tam_mp, tam_qd)
    MS* ms = inicializa_ms(20); //(tam_ms)
    P* proc = novo_processo(mp, ms, "P1", 5, 1, 4); //(tam_P1, tam_pag) -- teremos 5 paginas.

    return 0;
}
*/

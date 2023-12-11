/*
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <string.h>
#include "conversoes.c"

MS *inicializa_ms(int tamanho);
P *novo_processo(MP* mp, MS *ms, char *nome_processo, int tam_processo, int tam_pag, int tam_end);
void add_tp(P *processo, MP* mp, int size_pag);
ROW *insere_final(ROW *l);
ROW* busca_linha(int pag, MP* mp, P* proc);
P *busca_processo(MS *m_sec, char *nome_processo);
int LRU_cheia(LRU *fila, int tam_max);
LRU *insere_fila(LRU *fila, int numero_qd);
    novo->tam_end_logico = tam_end;
    novo->prox = ms->processos;
    ms->processos = novo;
    //carregar_pg_0
    printf("O processo %s foi adicionado na MS com sucesso! Obs.: Sua TP associada ja foi iniciada na MP.\n", nome_processo);
    
    //carregar_pg_0 e 1
    printf("O processo %s foi adicionado na MS com sucesso e 2 das suas páginas foram carregadas para MP! Obs.: Sua TP associada ja foi iniciada na MP.\n", nome_processo);
    //novo->estado_processo = "Pronto";  ????
    
    //adicionando tp desse processo na mp
    add_tp(novo, mp, tam_pag);
    return novo; //melhor que seja uma funcao void, mas para testes botei P*
    return novo;
}
P *busca_processo(MS *m_sec, char *nome_processo){
    while(aux){
        if(strcmp(aux->identificador, nome_processo) == 0) return aux;
        else aux = aux->prox;
    }
    } 
    return NULL;
}

void tlse_libera(TLSE *l){
    TLSE *p = l, *t;
    while(p != NULL){
        t = p;
        p = p->prox;
        free(t);
    }
}

void halt(char *nome_processo, MP *m_princ, MS *m_sec){
    // liberando as linhas da tp
    ROW *aux_row = m_princ->tabela_paginas->rows, *tmp_row;
    while(aux_row){
        tmp_row = aux_row;
        aux_row = aux_row->prox;
        free(tmp_row);
    }
    // liberando a tabela de paginas
    TP *aux_tp = m_princ->tabela_paginas, *tmp_tp;
    while(aux_tp){
        tmp_tp = aux_tp;
        aux_tp = aux_tp->prox;
        free(tmp_tp);
    }
    // liberando as paginas da fila da mp
    // liberando a memoria secundaria
    P *aux_ms = m_sec->processos, *ant = NULL;
    while(m_sec){
        if(strcmp(aux_ms->identificador, nome_processo) == 0){
        }
    }
}
void add_tp(P *processo, MP* mp, int size_pag){ //adiciona na memoria principal
	int num_paginas = (processo->tam_imagem)/size_pag;
//SUBSTITUIR ... por num_quadro
/*void carrega_pag_mp(P* processo, MP *m_principal, int end_logico){
void carrega_pag_mp(P* processo, MP *m_principal, int end_logico){
    //processo esta esperando (Novo ou bloqueado-suspenso)
    int pag = n_pag(end_logico, processo->tam_pagina, processo->tam_end_logico);
    if (!LRU_cheia(m_principal->fila_lru, m_principal->tam_mp)){ //Caso 1: MP nao esta cheia
        m_principal->fila_lru = insere_fila(m_principal->fila_lru, ...);
        m_principal->fila_lru = insere_fila(m_principal->fila_lru, num_quadro);
        //atualizar tp:
        ROW* linha = busca_linha(pag, m_principal, processo);
        linha->bit_p = 1
        linha->numero_qd = num_quadro;
    }
    else{ //Caso 2: A MP esta cheia
    printf("O processo passou para o estado 'Pronto'\n");
}
void busca_pagina(P* proc, MP* mp, int end_logico, int qtd_bits_endereco_logico){
void busca_pagina(P* proc, MP* mp, int end_logico){
    int pag = n_pag(end_logico, proc->tam_pagina, qtd_bits_endereco_logico);
    int pag = n_pag(end_logico, proc->tam_pagina, proc->tam_end_logico);
    ROW* linha = busca_linha(pag, mp, proc);
    if (linha->bit_p == 1){
        //carrega_pag_mp(proc, mp, end_logico);
    }
}

int mp_livre(int livre, MP* mp, P* proc){
    return livre -= proc->tam_pagina
    */
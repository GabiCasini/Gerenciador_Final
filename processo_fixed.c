#include <stdio.h>
#include <stdlib.h>
#include "conversoes.c"

/*
IMPLEMENTAR:
-n_quadro e ajeitar carrega_pg_mp
-swapper (funcoes.c)
-halt (funcoes.c)
-modificar_tp (quando pagina é trazida pra mp, retirada da mp ou modificada√)
-simulador de atualizacao de processo na ms quando é realizada escrita sobre a pagina na mp (bit_m == 1)
-add printf por todo o codigo para a querida nao ficar perdida
-depois disso tudo, basta ajustar leitura de arquivo, funcao flags e main()

Inicialmente, o GM deve alocar devidamente um (ou mais) quadro(s) para o processo,
                                                quando este for criado para que ele se torne pronto (? - gabi nao tendeu mt bem)
*/

//oq acontece se a ms tiver cheia???

/*
//da pra fazer uma SIMULACAO sem a estrutura pagina, na realidade a pagina armazenaria um certo conteudo
typedef struct pagina {
    int *tam_pagina;
    int *tam_endereco_virtual;
    int *endereco_virtual;
    struct pagina *prox;
} PG;
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
    int numero_qd; //seria mais facil aqui trabalhar com o numero do quadro e abstrair o offset
    struct conteudo_tp *prox;
} ROW;

typedef struct tabela_paginas {
    char *id; //id do processo associado
    ROW *rows; //linhas da tabela
    struct tabela_paginas *prox; //cada processo tera uma tp associada e essas estarao localizadas na MP
} TP;

typedef struct least_recently_used {
    int numero_qd; //fisico //consultar amandinha
    struct least_recently_used *prox;
} LRU;

typedef struct memoria_principal {
    int tam_mp;
    int num_quadros;
    LRU *fila_lru;
    TP *tabela_paginas;
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

int LRU_cheia(LRU *fila, int tam_max);
LRU *insere_fila(LRU *fila, int numero_qd);
LRU *retira_fila(LRU *fila);
void moverParaFinal(LRU *lista, int val);

//int mp_livre(int livre, MP* mp, P* proc);

void escrever(P* proc, MP* mp, int end);
void atualizar_ms();
void retira_row(int pag, MP* mp, P* proc);
void add_row(int pag, MP* mp, P* proc, int end_qd);

// --------------------- ACABA ARQUIVO funcoes_fixed.h

MP *inicializa_mp(int tam_mf, int tam_qm){

    MP *novo = (MP*)malloc(sizeof(MP));

    novo->tam_mp = tam_mf;
    novo->num_quadros = tam_mf / tam_qm;
    novo->fila_lru = NULL;
    novo->tabela_paginas = NULL;

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
    novo->identificador = nome_processo; //strcpy tava dando Segmentation fault: 11
    novo->estado_processo = "Novo";
    novo->tam_imagem = tam_processo; //*pow(2,6) ???
    novo->tam_pagina = tam_pag;
    novo->tam_end_logico = tam_end;
    novo->prox = ms->processos;
    ms->processos = novo;
    printf("O processo %s foi adicionado na MS com sucesso! Obs.: Sua TP associada ja foi iniciada na MP.\n", nome_processo);

    //adicionando tp desse processo na mp
    add_tp(novo, mp, tam_pag);

    return novo; //melhor que seja uma funcao void, mas para testes botei P*
}

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

LRU *insere_fila(LRU *fila, int numero_qd){  //mudei

    // se o elemento nao esta na fila, entao vamos inseri-lo
    LRU *novo = (LRU*)malloc(sizeof(LRU));
    novo->numero_qd = numero_qd;

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

LRU *retira_fila(LRU *fila){
    if (fila){
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

//if bit_m == 1 na hora de tirar da fila
void atualizar_ms(){
    printf("Copiando o conteudo da página para a Memória Secundária...");
    printf("MS atualizada com sucesso!");
}

void retira_row(int pag, MP* mp, P* proc){
    ROW* linha = busca_linha(pag, mp, proc);
    linha->bit_p = 0;
    linha->bit_m = 0;
    linha->numero_qd = -1;
}

void add_row(int pag, MP* mp, P* proc, int end_qd){
    ROW* linha = busca_linha(pag, mp, proc);
    linha->bit_p = 1;
    linha->numero_qd = end_qd;
}


//SUBSTITUIR ... por num_quadro
/*void carrega_pag_mp(P* processo, MP *m_principal, int end_logico){

    //processo esta esperando (Novo ou bloqueado-suspenso)
    int pag = n_pag(end_logico, processo->tam_pagina, processo->tam_end_logico);
    int num_quadro = n_qd();

    if (!LRU_cheia(m_principal->fila_lru, m_principal->tam_mp)){ //Caso 1: MP nao esta cheia

        m_principal->fila_lru = insere_fila(m_principal->fila_lru, ...);

        //atualizar tp:
        ROW* linha = busca_linha(pag, m_principal, processo);
        linha->bit_p = 1
        linha->numero_qd = num_quadro;

    }

    else{ //Caso 2: A MP esta cheia
        printf("A MP esta cheia, useremos a política de substituição LRU para carregar a página na MP");
        retira_row(pag, m_principal, processo); //mudar esse bglh aq, tem q olhar o quadro. pag em questao é a nova, nao a antiga
        m_principal->fila_lru = retira_fila(m_principal->fila_lru);
        m_principal->fila_lru = insere_fila(m_principal->fila_lru, ...);
        add_row();
    }

    printf("A página da imagem do processo %s foi carregada para a MP com sucesso\n", processo->identificador);
    processo->estado_processo = 'Pronto';
    printf("O processo passou para o estado 'Pronto'\n");
}

void busca_pagina(P* proc, MP* mp, int end_logico, int qtd_bits_endereco_logico){

    int pag = n_pag(end_logico, proc->tam_pagina, qtd_bits_endereco_logico);
    ROW* linha = busca_linha(pag, mp, proc);

    if (linha->bit_p == 1){
        printf("Página buscada já está na MP"); //pode fazer instrucao, escrita e leitura
        LRU *fila = mp->fila_lru;
        moverParaFinal(fila, linha->numero_qd); //traz pag pro topo da fila (mais recentemente usado)
    }
    else{
        printf("Page fault");
        //carrega_pag_mp(proc, mp, end_logico);
    }
}
*/

int mp_livre(int livre, MP* mp, P* proc){
    return livre -= proc->tam_pagina;
}

/*
int main(){ //testes

    MP* mp = inicializa_mp(10, 1); //(tam_mp, tam_qd)
    MS* ms = inicializa_ms(20); //(tam_ms)
    P* proc = novo_processo(mp, ms, "P1", 5, 1, 4); //(tam_P1, tam_pag) -- teremos 5 paginas.

    //int livre = mp->tam_mp;
    //livre = mp_livre(livre, mp, proc);
    //printf("%d", livre);

    return 0;
}
*/

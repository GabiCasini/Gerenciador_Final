#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#include "funcoes.h"

/*
typedef struct tabela_paginas {
    int bit_p;
    int bit_m;
    int end_quadro; // endereÃ§o na memoria principal
    //achoq nao precisa de tam preenchido  kskskssksk
    struct tabela_paginas *prox; // estamos tratando como se fosse uma lista encadeada...
} TP;

typedef struct least_recently_used {
    int end;
    struct least_recently_used *prox;
} LRU;

typedef struct processo {
    char *identificador; // nome do processo lido no arquivo
    char *estado_processo; // informa em qual estado o processo esta: novo, pronto, bloqueado, executando, finalizado
    int tam_imagem; // tamanho do processo
    TP *tab_paginas; // lista encadeada, vamos usar esse TP?
    struct processo *prox; // pois na ms tem uma lista de processos...
} P;

typedef struct pagina {
    int *tam_pagina;
    int *tam_endereco_virtual;
    int *endereco_virtual; // endereco da pag na memoria virtual
    P *processo_associado; // processo a qual a pg esta associada
} PG;

typedef struct memoria_principal {
    int tam_mp;
    int num_quadros;
    int *pags_refs;
    LRU *fila_lru; //na pratica isso aqui nao existe :))))
    TP *tabela_paginas;
} MP;

typedef struct memoria_secundaria {
    int tam_ms;
    P *processos;
} MS;
*/

int casas_decimais_em_binario(int n) {
    int tamanho_binario = 0;
    while (n >= 1) {
        n >>= 1;
        tamanho_binario++;
    }
    return tamanho_binario;
}


// converte de decimal p binario
// n -> valor a ser convertido, *binario Ã© o array int
void binario(int n, int *binario, int tamanho) {
    int i;
    for (i = tamanho - 1; i >= 0; i--) {
        binario[i] = n & 1;
        n >>= 1;
    }
}

P *busca_processo_ms(MS *m_secundaria, char *nome_processo){
    P *aux = m_secundaria->processos;
    while(aux != NULL && strcmp(aux->identificador, nome_processo) != 0){
        aux = aux->prox;
    }
    return aux;
}

LRU *busca_fila(LRU *fila, int end){
    LRU *aux = fila;
    while(aux != NULL){
        if(aux->end == end) break;
        aux = aux->prox;
    }
    return aux;
}
TP *insere_inicio(TP *l){
    TP *novo = (TP*)malloc(sizeof(TP));
    novo->bit_m = 0;
	novo->bit_p = 0;
	novo->end_quadro = -1;
    novo->prox = l;
    return novo;
}
TP *inicia_tabela_de_paginas_proc(P *processo, int size_pag){ 
	
	printf(" que foi\n");
	int num_paginas = (processo->tam_imagem)/size_pag;
	//for(int i = 0; i < num_paginas; i++){
	processo->tab_paginas=insere_inicio(processo->tab_paginas);    //aqui esta o erro
	printf("sera que foi");
	//}
	TP *aux = processo->tab_paginas;
	int cont = 0;
	while(aux){
	    aux->end_quadro = cont * size_pag;
	    cont++;
	    aux = aux->prox;
	}
	return aux;
}
P *aloca_processo(MS *m_secundaria, char *nome_processo, int tam_processo, int tam_pag){

    P *aux = NULL;
    // checa se o processo jÃ¡ esta alocado na memÃ³ria
    aux = busca_processo_ms(m_secundaria, nome_processo); // vai ter q trocar
    // se nÃ£o estiver, aloca o processo na memÃ³ria
    if(!aux){

        P *novo = (P*)malloc(sizeof(P));
        strcpy(novo->identificador, nome_processo);
        strcpy(novo->estado_processo, "Novo");
        novo->tam_imagem = tam_processo*pow(2,6);
		printf(" %d\n", tam_pag);
        novo->tab_paginas=inicia_tabela_de_paginas_proc(novo, tam_pag);
        printf("oi");
        novo->prox = m_secundaria->processos;
        m_secundaria->processos = novo;

        return novo;
    }
    return aux;
}

void halt_MP(MP *mp, P *proc){

    /*  TP *pag = proc->tab_paginas;

      while(pag!=NULL){  //verifica se pag ta na mp pra exclusao

          if (pag->bit_p == 1){

              int end = pag->end_quadro;
              //descobrir numero do quadro!
              //busca na mp o endereco
              //exclui a pagina
          }


      }*/

    TP *pag = proc->tab_paginas;

    while(pag!= NULL){

        TP * linha = pag;

        free(linha);
        pag = pag->prox;
    }
    free(proc->tab_paginas); //exclui tabela de pag do processo da mp
}

void halt_MS(MS *ms, P *proc){

    P *temp = ms->processos;
    P *aux = temp->prox;
    while(temp!=NULL){
        if(strcmp(aux->identificador, proc->identificador)==0){
            temp->prox = aux->prox;
            free(aux);
            break;
        }
        else{
            aux = aux->prox;
            temp = temp->prox;
        }
    }
}

void halt(P *proc, MP *mp, MS *ms ){

    halt_MP(mp, proc); //exclui as pags q estao na mp
    halt_MS(ms, proc); //exclui o processo da ms

    free(proc->identificador);
    free(proc->estado_processo);
    free(proc);
}

LRU *insere_fila_parte2(LRU *fila, int end){
    // se o elemento nÃ£o estÃ¡ na fila, entao vamos inseri-lo
    if(!busca_fila(fila, end)){
        LRU *novo;

        novo = (LRU*)malloc(sizeof(LRU));
        novo->end = end;

        if(!fila) return novo;
        LRU *aux = fila;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
        novo->prox = NULL;
        return fila;
    }
        // se o elemento estÃ¡ na fila, entao buscamos e inserimos no final
    else {
        LRU *ref = NULL, *aux = fila;

        while(aux->prox != NULL){
            if(aux->end == end){
                ref = aux;
            }
            aux = aux->prox;
        }
        aux->prox = ref;
        ref->prox = NULL;
    }
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

LRU *insere_fila(LRU *fila, int end, int tam_max){
    LRU *novo = NULL;
    int cheia = 1;
    if(!LRU_cheia(fila, tam_max)){
        novo = insere_fila_parte2(fila, end);
    }
    return novo;
}

void configuracoes(int *tam_mf, int *tam_ms, int *tam_qm, int *tam_end_logico, PG *pag){

    *tam_qm = 2;
    *tam_mf = 4096;
    printf("Digite o tamanho desejado para cada uma das opcoes abaixo \n");

    printf("1. Tamanho do quadro de memoria (valor inteiro em KB): \n"); //criar opcao pra ser MB ou GB
    //while(*tam_qm <= 0){
        //scanf("%d", tam_qm);
   // }
    *tam_qm *= 1024;

    printf("2. Tamanho da memoria fisica (Deve ser multiplo do tamanho do quadro: %d): \n", *tam_qm);
    //do {
       // if(*tam_mf) printf("Valor incorreto, deve ser multiplo de %d!", *tam_qm);
       // scanf("%d", tam_mf);
   // } while (*tam_mf % (*tam_qm) != 0);

    printf("3. Tamanho da memoria secundaria: \n");
    //scanf("%d", tam_ms);
	*tam_ms=2048;
    printf("4. Tamanho em bits do endereço lógico: \n");
    //scanf("%d", tam_end_logico);
    *tam_end_logico=5;
	*tam_end_logico = (int)pow(2, *tam_end_logico-3); // o que Ã© isso?
    
	//aqui que ele ta parando
	pag->tam_pagina = tam_qm;
    pag->tam_endereco_virtual = tam_end_logico;
}
void instrucao_cpu (){
    printf("\nInstruÃ§Ã£o sendo executada\n");
}
void instrucao_es (){
    printf("\nInterromper, ir para bloqueado\n");
}
void impressao_p (P *proc){
    printf("\nImprimi processo\n");
}
void leitura (){
    printf("\nLeitura\n");
}
void escrita (){
    printf("\nEscrita\n");
}
void imprime_estado_processo(P *processo){
    printf("Dados do processo %s: \n", processo->identificador);
    printf("----------------------\n");
    printf("Estado do processo: %s \n", processo->estado_processo);
    printf("Tamanho do processo: %d \n", processo->tam_imagem);
    printf("Tabela de pÃ¡ginas associada: \n");
//    imprime_tabela_paginas(processo->tab_paginas);
    printf("\n");
}

void flags(char flag_processo, char *nome_processo, int tam_processo, MS *m_secundaria, MP *m_principal){

    P *proc;
    // aqui aloca o processo se ele nao existir, ou busca na memÃ³ria caso jÃ¡ esteja lÃ¡
    proc = aloca_processo(m_secundaria, nome_processo, tam_processo, ((m_principal->tam_mp)/(m_principal->num_quadros)));

	printf("Flagss DPS/n");
    if(flag_processo == 'P'){
        imprime_estado_processo(proc); // antes da alteraÃ§Ã£o
        //busca pag !!!
        instrucao_cpu();
        imprime_estado_processo(proc); // depois da alteraÃ§Ã£o
    }
    if(flag_processo ==  'I'){
        //estado bloqueado a espera de E/S
        impressao_p(proc);
        instrucao_es();
        impressao_p(proc);
    }
    if(flag_processo == 'C'){
        //aloca_processo(); ja ta na linha 43 
        impressao_p(proc); //resolver primeira flag
    }
    if(flag_processo == 'R'){
        //estado esperando estar na mp --> pronto --> executando
        //busca pag
        impressao_p(proc);
        leitura();
        impressao_p(proc);
    }
    if(flag_processo == 'W'){
        //estado esperando estar na mp --> pronto --> executando
        //busca pag
        impressao_p(proc);
        escrita();
        impressao_p(proc);
    }
    if(flag_processo == 'T'){
        //estado final?? :)
        impressao_p(proc);
        //halt();
        impressao_p(proc);
    }
    else {
        printf("OpÃ§Ã£o invÃ¡lida. \n");
    }
}

int busca_pagina_referenciada(MP *m_principal, int end){

    for(int i = 0; i < m_principal->num_quadros; i++){
        if(m_principal->pags_refs[i] == end) return 1;
    }
    return 0;
}
/* operaÃ§Ãµes das estruturas de dados - memÃ³ria principal */

MP *inicializa_mp(int tam_mf, int tam_qm){

    MP *novo = (MP*)malloc(sizeof(MP));
    novo->tam_mp = tam_mf;
    novo->num_quadros = tam_mf / tam_qm;
    printf("qm: %d\n", tam_qm);
	printf("%d\n", novo->num_quadros);
	printf("tam:%d\n", novo->tam_mp);
    // aqui aloca o vetor de paginas referenciadas e inicializa ele
    novo->pags_refs = (int*)malloc(sizeof(int) * novo->num_quadros);
    if(!novo->pags_refs) exit(1);
    for(int i = 0; i < novo->num_quadros; i++){
        novo->pags_refs[i] = -1;
    }

    novo->fila_lru = NULL;

    novo->tabela_paginas = NULL;

    return novo;
}

void insere_pagina(MP *m_principal, P *processo, int end){

    // aqui insere no vetor de paginas
    if(!busca_pagina_referenciada(m_principal, end)){
        for(int i = 0; i < m_principal->num_quadros; i++){
            if(m_principal->pags_refs[i] == -1){
                m_principal->pags_refs[i] = end;
                break;
            }
        }
    }
    else printf("Endereco ja esta na MP");

    // aqui insere a tabela de pags do processo Ã  lista de tabela de pags
    // trocar a estrutura
    TP *novo;
    //novo = (*TP)malloc(sizeof(TP) * );// o que Ã© isso???

    // aqui insere na fila de paginas ja tratando o caso da pagina ja estar referenciada
    m_principal->fila_lru = insere_fila(m_principal->fila_lru, end, m_principal->tam_mp);

}

/* operaÃ§Ãµes das estruturas de dados - memÃ³ria secundÃ¡ria */

MS *inicializa_ms(int tamanho){
    MS *novo = (MS*)malloc(sizeof(MS));
    novo->tam_ms = tamanho;
    novo->processos = NULL;

    return novo;
}

/* operaÃ§Ãµes das estruturas de dados - tabela de pÃ¡ginas */

// acho que ta pronto, pf confiram @gabi @amanda @raio

// Converte binÃ¡rio em decimal
int decimal(const int *binario, int tamanho) {
    int valor = 0;
    for (int i = 0; i < tamanho; i++) {
        valor += binario[i] * (int)pow(2, tamanho - 1 - i);
    }
    return valor;
}

/* operaÃ§Ãµes das estruturas de dados - processo */

int n_pag (int endereco_logico, int page_size, int qtd_bits_endereco_logico){
    int qtd_bits_off_set = casas_decimais_em_binario(page_size);
    int qtd_bits_num_pag = qtd_bits_endereco_logico - qtd_bits_off_set;
    int bin_end_log[qtd_bits_endereco_logico];
    binario(endereco_logico, bin_end_log, qtd_bits_endereco_logico);

    // adiciona os bits do off set e da pag
    int bits_off_set[qtd_bits_off_set];
    int bits_num_pag[qtd_bits_num_pag];
    int pos = 0;
    while (pos < qtd_bits_num_pag) {
        bits_num_pag[pos] = bin_end_log[pos];
        pos++;
    }
    int i = 0;
    while (i < qtd_bits_off_set) {
        bits_off_set[i] = bin_end_log[pos];
        i++;
        pos++;
    }

    int num_pag = decimal(bits_num_pag, qtd_bits_num_pag);
    return num_pag;
}
/*void busca_pagina(P *processo, int end_logico, int qtd_bits_endereco_logico){  
//pra implementar instrucao e tals

    // encontra a pag, ve na tp se ta em mp, se sim otimo e se nao carrega_pag_mp()
    TP * tab_paginas = processo->tab_paginas;

    //usar conversao do hendel pra encontrar aqui o numero da pagina igualar a k;
    int k = n_pag(end_logico,/*tam da pagina, qtd_bits_endereco_logico);
    int i = 0;
    while (i < k){
        tab_paginas = tab_paginas->prox;
        i++;
    }

    if (tab_paginas->bit_p == 1){
        printf("PÃ¡gina buscada ja¡ esta¡ na MP"); //pode fazer instrucao, escrita e leitura;

    }
    else{
        //achar a pagina na ms pra passar como parametro nessa proxima funcao
        // carrega_pag_mp(P* processo, PG* pagina, MP *m_principal);
    }
}
*/
LRU *retira_fila(LRU *fila) {
    if (fila){
        LRU *aux = fila;
        fila = fila->prox;
        free(aux);
    }
    return fila;
}
/*
void carrega_pag_mp(P* processo, PG* pagina, MP *m_principal){

    //processo esta esperando (Novo ou bloqueado)
    int *mp = m_principal->pags_refs; //assim q usa ponteiro? sksksks n lembro


    if (!LRU_cheia(m_principal->fila_lru, m_principal->tam_mp)){ //Caso 1: MP nao esta cheia -- pagina Ã© alocada
        insere_pagina(m_principal, processo, *mp);
    }
    else{ //Caso 2: A MP esta cheia e aplicaremos a polÃ­tica de substituicao LRU
        printf("A MP esta cheia, useremos a politica de substituicao LRU para carregar a imagem na MP");//faltou o ponto e virgula!!!!!!!
        m_principal->fila_lru = retira_fila(m_principal->fila_lru);
        insere_pagina(m_principal, processo, *mp);

    }

    printf("A pagina da imagem do processo X foi carregada para a MP com sucesso\n"); //substituir X
    //estado do processo passa a ser pronto
    strcpy(processo->estado_processo, 'Pronto'); //aparentemente da certo se fizer strcpy(processo->estado_processo, (const char *) 'Pronto');
    // processo->estado = 'Pronto'
    printf("O processo passou para o estado 'Pronto'\n");
}

*/
void moverParaFinal(LRU *lista, int val) {//quando for chamado e resetar a prioridade
    LRU *aux = lista;
    LRU *ant = NULL;

    // Procurar pelo LRU com end == val
    while (aux != NULL && aux->end != val) {
        ant = aux;
        aux = aux->prox;
    }

    // Se o LRU com end == val for encontrado
    if (aux != NULL) {
        // Se jÃ¡ estiver no final da lista, nÃ£o hÃ¡ nada a fazer
        if (aux->prox == NULL) {
            printf("LRU com end = %d jÃ¡ estÃ¡ no final da lista.\n", val);
            return;
        }

        // Remover o LRU da sua posiÃ§Ã£o atual
        if (ant != NULL) {
            ant->prox = aux->prox;
        } else {
            lista = aux->prox;
        }

        // Encontrar o Ãºltimo elemento na lista
        LRU *ultimo = lista;
        while (ultimo->prox != NULL) {
            ultimo = ultimo->prox;
        }

        // Colocar o LRU removido no final da lista
        ultimo->prox = aux;
        aux->prox = NULL;

        printf("LRU com end = %d movido para o final da lista.\n", val);
    } else {
        printf("LRU com end = %d nÃ£o encontrado na lista.\n", val);
    }
}

// como Ã© fila, insere no final
/*TP *insere_pagina(MP *m_principal, PG pagina, TP *tab, int p, int m, int end){

    TP *novo;

    novo = (TP*)malloc(sizeof(TP));
    novo->bit_p = p;
    novo->bit_m = m;
    novo->end_quadro = end;
    if(novo->tam_preenchido += pagina.tam_pagina <= m_principal->tam_mp) novo->tam_preenchido += pagina.tam_pagina;
    else lru(tab);

    if(!tab) return novo;

    TP *aux = tab;
    while(aux->prox != NULL){
        aux = aux->prox;
    }

    aux->prox = novo;
    return tab;
}

TP *remove_pagina(TP *tab){
    if(!tab) return NULL;

    TP *aux = tab;

    tab = tab->prox;
    free(aux);
    return tab;
}*/

TP *busca_processo_mp(MP *memoria_principal, P *processo){
    // implementar
    return NULL;
}

/*TP *preenche_fila(MP *m_principal, P *processo, PG pagina){

    TP *aux;
    aux = busca_processo_mp(m_principal, processo);
    if(!aux){
        TP *aux_proc;
        aux = processo->tab_paginas;

        while(aux != NULL){
            insere_pagina(m_principal, processo, );
            m_principal->fila_lru = insere_pagina();
            aux_proc = aux_proc->prox;
        }
    }
     return aux;
 }*/

int MMU (int endereco_logico, int page_size, int qtd_bits_endereco_logico, TP *tabela) {
    int qtd_bits_off_set = casas_decimais_em_binario(page_size);
    int qtd_bits_num_pag = qtd_bits_endereco_logico - qtd_bits_off_set;
    int bin_end_log[qtd_bits_endereco_logico];
    binario(endereco_logico, bin_end_log, qtd_bits_endereco_logico);

    // adiciona os bits do off set e da pag
    int bits_off_set[qtd_bits_off_set];
    int bits_num_pag[qtd_bits_num_pag];
    int pos = 0;
    while (pos < qtd_bits_num_pag) {
        bits_num_pag[pos] = bin_end_log[pos];
        pos++;
    }
    int i = 0;
    while (i < qtd_bits_off_set) {
        bits_off_set[i] = bin_end_log[pos];
        i++;
        pos++;
    }

    int off_set = decimal(bits_off_set, qtd_bits_off_set);
    int num_pag = decimal(bits_num_pag, qtd_bits_num_pag);

    TP *pagina = tabela;
    for (int j = 0; j < num_pag; j++) pagina = pagina->prox;
    int endereco_real = (pagina->end_quadro * page_size) + off_set;

    return endereco_real;
}


/* operaÃ§Ã£o de tÃ©rmino - processo */

/* impressÃµes */

void imprime_tabela_paginas(TP *tab_pags){

    printf("=======================\n");
    int i = 0;
    TP *aux = tab_pags;
    while(aux != NULL){
        printf("Lacuna %d: \n", i);
        printf("Bit de presenÃ§a na memÃ³ria principal: %d \n", tab_pags->bit_p);
        printf("Bit de modificaÃ§Ã£o da pÃ¡gina: %d \n", tab_pags->bit_m);
        printf("EndereÃ§o do quadro na memÃ³ria principal: %d \n", tab_pags->end_quadro);
        printf("-------------------------\n");
        aux = aux->prox;
    }
}

void ver_mp(MP *mp){
    int num_quadros = (mp->num_quadros);
    TP *quadro_paginas = mp->tabela_paginas;

    printf("Estado da Memoria Principal:\n");
    printf("Numero total de quadros: %d\n", num_quadros);

    while(quadro_paginas) {
        printf("  Bit de PresenÃ§a: %d\n", quadro_paginas->bit_p);
        printf("  Bit de ModificaÃ§Ã£o: %d\n", quadro_paginas->bit_m);
        printf("  EndereÃ§o do Quadro na MemÃ³ria Principal: %d\n", quadro_paginas->end_quadro);
        printf("-----------------------\n");
        quadro_paginas = quadro_paginas->prox;
    }
}

void ver_ms(MS *memoria_secundaria){
    int tam_ms = memoria_secundaria->tam_ms;
    P *processos = memoria_secundaria->processos;

    printf("Estado da MemÃ³ria SecundÃ¡ria:\n");
    printf("Tamanho total da MemÃ³ria SecundÃ¡ria: %d\n", tam_ms);

    P *atual = processos;
    while (atual != NULL) {
        printf("Processo %s:\n", atual->identificador);
        printf("  Estado do Processo: %s\n", atual->estado_processo);
        printf("  Tamanho do Processo: %d\n", atual->tam_imagem);
        // Adicione mais informaÃ§Ãµes conforme necessÃ¡rio
        printf("-----------------------\n");

        atual = atual->prox;
    }
}
/*
void swapper(LRU *lru, TP *tab_paginas) {
    LRU *segundo_processo = lru->prox;
    LRU *temp = lru;
    free(temp);
    lru = segundo_processo;

    TP *aux = tab_paginas;
    while (aux != NULL && aux->prox != NULL) {
        if (aux->end_quadro == lru->end) {
            aux->bit_p = 0;
            if (aux->bit_m == 1) {
            	atualiza_ms();
            }
        }
        aux = aux->prox;
    }
}*/
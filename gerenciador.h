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

TP *add_tp(P *processo, MP* mp, int size_pag);
ROW *insere_final(ROW *l);
ROW* busca_linha(int pag, MP* mp, P* proc);
P *busca_processo(MS *m_sec, char *nome_processo);
void busca_pagina(P* proc, MP* mp, int pag);
void swapper(P* processo, MP *m_principal, int pag);

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

void ver_fila_lru(MP *memoria_principal);
void ver_TP(MP *memoria_principal);
void ver_mp(MP *memoria_principal);
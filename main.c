//#include "funcoes.c"
#include "configuracoes&flags.c"

int main(void){

    //setlocale(LC_ALL, "pt_BR.UTF-8");

    // variaveis para manipulação dos arquivos
    char acao;
    char nome_processo[20];//só pra definir algum tamanho máximo
    char tam_processo;

    // variaveis para manipulação das funções
    int tamanho_mf, tamanho_ms, tamanho_qm, tamanho_end;

    // variaveis para alocacao de memoria
    MP *mem_principal;
    MS *mem_secundaria;

    // configurando o espaco de memoria
    configuracoes(&tamanho_mf, &tamanho_ms, &tamanho_qm, &tamanho_end);

    // alocando a memoria necessaria
    mem_secundaria = inicializa_ms(tamanho_ms);
    mem_principal = inicializa_mp(tamanho_mf, tamanho_qm);

    // abrindo o arquivo
    FILE* fila_processos = fopen("processos.txt", "r");
    if(fila_processos == NULL){
        printf("Erro na leitura da fila de processos. \n");
        exit(1);
    }

    char tam[3];

    while(!feof(fila_processos)){
        //fflush(stdin);

        fscanf(fila_processos, "%s %c %s %s", nome_processo, &acao, &tam_processo, tam);
        printf("\n%s %c %d\n", nome_processo, &acao, &tam_processo);
        flags(acao, nome_processo, tam_processo, mem_secundaria, mem_principal, tamanho_end);
    }
    // fechando o arquivo
    fclose(fila_processos);

    return 0;
}

//#include "funcoes.c"
#include "configuracoes&flags.c"

int main(void){

    //setlocale(LC_ALL, "pt_BR.UTF-8");


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

    // variaveis para manipulação dos arquivos
    char acao[3];
    char nome_processo[3];//só pra definir algum tamanho máximo
    int tam_processo;
    char tam[3];
    char tipoTam[3][20] = {
        "KB",
        "MB",
        "GB"
    };

    while(!feof(fila_processos)){
        //fflush(stdin);

        fscanf(fila_processos, "%s %s %d %s", nome_processo, &acao, &tam_processo, tam);
        if(strcmp(tam, tipoTam[0])==0) {
            tam_processo=tam_processo*1024;
            printf("\n%d", tam_processo);
        }
        else if(strcmp(tam, tipoTam[1])==0){
            tam_processo=tam_processo*(1024*1024);
            printf("\n%d", tam_processo);
        }
        else if(strcmp(tam, tipoTam[2])==0){
            tam_processo=tam_processo*(1024*1024*1024);
            printf("\n%d", tam_processo);
        }

        printf("\n%s %s %d %s\n", nome_processo, acao, tam_processo, tam);
        flags(acao, nome_processo, tam_processo, mem_secundaria, mem_principal, tamanho_end);
    }
    // fechando o arquivo
    fclose(fila_processos);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <locale.h>
#include <string.h>
#include <math.h>


int casas_decimais_em_binario(int n);
void binario(int n, int *binario, int tamanho);
int decimal(const int *binario, int tamanho);
int n_pag (int endereco_logico, int page_size, int qtd_bits_endereco_logico);

int casas_decimais_em_binario(int n) {
    int tamanho_binario = 0;
    while (n >= 1) {
        n >>= 1;
        tamanho_binario++;
    }
    return tamanho_binario;
}

void binario(int n, int *binario, int tamanho) {
    int i;
    for (i = tamanho - 1; i >= 0; i--) {
        binario[i] = n & 1;
        n >>= 1;
    }
}

int decimal(const int *binario, int tamanho) {
    int valor = 0;
    for (int i = 0; i < tamanho; i++) {
        valor += binario[i] * (int)pow(2, tamanho - 1 - i);
    }
    return valor;
}

int n_pag (int endereco_logico, int page_size, int qtd_bits_endereco_logico){
    int qtd_bits_off_set = casas_decimais_em_binario(page_size);
    int qtd_bits_num_pag = qtd_bits_endereco_logico - qtd_bits_off_set;
    int bin_end_log[qtd_bits_endereco_logico];
    binario(endereco_logico, bin_end_log, qtd_bits_endereco_logico);

    int bits_num_pag[qtd_bits_num_pag];
    int pos = 0;
    while (pos < qtd_bits_num_pag) {
        bits_num_pag[pos] = bin_end_log[pos];
        pos++;
    }

    int num_pag = decimal(bits_num_pag, qtd_bits_num_pag);
    return num_pag;
}

/*
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



int main(){
    int p = n_pag(1502,2,11);
    printf("%d\n", p);


    return 0;
}*/
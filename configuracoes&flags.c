#include "processo_fixed.c"


//botei pra pessoa diva por o tamanho em Bytes pra facilitar o codigo, mas podemos mudar isso e deixar ela selecionar KB/MB/GB
void configuracoes(int *tam_mf, int *tam_ms, int *tam_qm, int *tam_end_logico){

    printf("Digite o tamanho desejado para cada uma das opcoes abaixo \n");

    printf("1. Tamanho do quadro de memoria em Bytes: \n");
    while(*tam_qm <= 0){
        scanf("%d", tam_qm);
    }

    printf("2. Tamanho da Memória Principal em Bytes: (Deve ser multiplo do tamanho do quadro: %d) \n", *tam_qm);
    do {
        scanf("%d", tam_mf);
    } while (*tam_mf % (*tam_qm) != 0);

    printf("3. Tamanho da memória secundaria em Bytes: \n");
    scanf("%d", tam_ms);

    printf("4. Tamanho do endereço lógico em bits: \n");
    scanf("%d", tam_end_logico);
}


void instrucao_cpu (){
    printf("\nInstrução sendo executada\n");
}
void instrucao_es (){
    printf("\nInterromper, ir para bloqueado\n");
}
void impressao_p (P *proc){
    printf("\nImprime processo\n");
}
void leitura (){
    printf("\nLeitura\n");
}
void escrita (){
    printf("\nEscrita\n");
}
void estado_processo(P *processo){
    printf("O processo %s está agora no estado %s. \n", processo->identificador, processo->estado_processo);
}


void flags(char flag_processo, char *nome_processo, int tam_processo, MS *m_secundaria, MP *m_principal, int tamanho_end){

    P *proc = novo_processo(m_principal, m_secundaria, nome_processo, tam_processo, (m_principal->tam_mp)/(m_principal->num_quadros), tamanho_end);

    //funcao nao esta funcionando certinho, nao consegue identificar qual é a flag de fato :/
    if(flag_processo == 'P'){
        estado_processo(proc);
        //busca pag !!!
        instrucao_cpu();
        estado_processo(proc);
    }
    /*
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

    */
    else {
        printf("Opcao Invalida. \n");
    }
}

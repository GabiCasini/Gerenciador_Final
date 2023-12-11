#include "processo_fixed.c"


//botei pra pessoa diva por o tamanho em Bytes pra facilitar o codigo, mas podemos mudar isso e deixar ela selecionar KB/MB/GB
void configuracoes(int *tam_mf, int *tam_ms, int *tam_qm, int *tam_end_logico){
    int op;
    printf("Digite o tamanho desejado para cada uma das opcoes abaixo \n");

    printf("1.1 Tamanho Bytes:\n1-KB\n2-MB\n3-GB\n");
    scanf("%d", &op);

    printf("1. Tamanho do quadro de memoria em Bytes: \n");
    while(*tam_qm <= 0){
        scanf("%d", tam_qm);
    }

    printf("2. Tamanho da Memoria Principal em Bytes: (Deve ser multiplo do tamanho do quadro: %d) \n", *tam_qm);
    do {
        scanf("%d", tam_mf);
    } while (*tam_mf % (*tam_qm) != 0);

    switch (op) {
        case 1:
            *tam_mf = *tam_mf * 1024;
            *tam_qm = *tam_qm * 1024;
            break;
        case 2:
            *tam_mf = *tam_mf * (1024 * 1024);
            *tam_qm = *tam_qm * (1024 * 1024);
            break;
        case 3:
            *tam_mf = *tam_mf * (1024 * 1024 * 1024);
            *tam_qm = *tam_qm * (1024 * 1024 * 1024);
            break;
        default:
            printf("Opção inválida!\n");
    }
    printf("3. Tamanho da memoria secundaria em Bytes: \n");
    scanf("%d", tam_ms);

    printf("4. Tamanho do endereço logico em bits: \n");
    scanf("%d", tam_end_logico);
}


void instrucao_cpu (){
    printf("\nInstrucao sendo executada\n");
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

char listaDeEntradas[5][20] = {
        "Disco",
        "Teclado",
        "Mouse",
        "Impressora",
        "Som"
    };
    
void flags(char *acao, char *nome_processo, int tam_processo_ou_end, MS *m_secundaria, MP *m_principal, int tamanho_end){

    P *proc = busca_processo(m_secundaria, nome_processo);
    char flag_processo = *acao;

    //int pag = n_pag(end_logico, proc->tam_pagina, proc->tam_end_logico);

    //funcao nao esta funcionando certinho, nao consegue identificar qual é a flag de fato :/
    if(flag_processo == 'C'){
        proc = novo_processo(m_principal, m_secundaria, nome_processo, tam_processo_ou_end, (m_principal->tam_mp)/(m_principal->num_quadros), tamanho_end);
        //impressao_p(proc); //resolver primeira flag
    }
    
    else if(flag_processo == 'P'){
        //int pag = n_pag(tam_processo_ou_end, proc->tam_pagina, proc->tam_end_logico);
        //busca_pagina(proc, m_principal, pag);
        printf("\n");
        instrucao_cpu();
    }
    
    else if(flag_processo == 'I'){
        //estado bloqueado a espera de E/S
        printf("\nI Dispositivo de entrada: %s\n", listaDeEntradas[tam_processo_ou_end]);
        impressao_p(proc);
        instrucao_es();
        impressao_p(proc);
    }

    else if(flag_processo == 'R'){
        //estado esperando estar na mp --> pronto --> executando
        //busca pag
        impressao_p(proc);
        leitura();
        impressao_p(proc);
    }
    else if(flag_processo == 'W'){
        //estado esperando estar na mp --> pronto --> executando
        //busca pag
        impressao_p(proc);
        escrita();
        //escrever(proc, mp, tam_processo_ou_end); 
        impressao_p(proc);
    }
    else if(flag_processo == 'T'){
        //estado final?? :)
        impressao_p(proc);
        ver_mp(m_principal);
        //halt(nome_processo, m_principal, m_secundaria);
    }
    else {
        printf("Opcao Invalida.\n");
    }
}

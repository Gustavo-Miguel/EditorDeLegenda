/*
 *-----------------------------------------------------------
 * UNIFAL – Universidade Federal de Alfenas.
 * Trabalho..: Editor de Legendas usando lista encadeada
 * Disciplina: Estrutura de Dados I
 * Professor.: Luiz Eduardo da Silva
 * Aluno(s)..: Alexandre William Miya - 2014.1.08.004
 *             Gustavo Miguel         - 2014.1.08.013
 *             Sylvester Alvarenga    - 2014.1.08.028
 * Data......: 07/05/2015
 *------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct noString {
    char letra;
    struct noString *proximo;

} *ptNoString; //estrutura para String dinâmica

typedef struct noLegenda {
    char inicio[13]; // 12 caracteres + "\0"
    char fim[13]; // 12 caracteres + "\0"
    ptNoString texto;
    struct noLegenda *proximo;
} *ptNoLegenda;

ptNoString iniciaString() {
    return NULL;
}

ptNoLegenda iniciaLegenda() {
    return NULL;
}

void insercaoTextoViaTeclado(ptNoString *ptPtNoString) {//Criar string dinâmica a partir do teclado
    ptNoString ptAux, ptUltimoNo;
    char caractere;
    int fimDigitacao = 0;

    do {
        caractere = getchar();
        if (caractere == '\n') {
            caractere = '\r';
            fimDigitacao++;
        } else {
            if (fimDigitacao > 0) {
                fimDigitacao--;
            }
        }
        ptAux = (ptNoString) malloc(sizeof (struct noString));
        ptAux->letra = caractere;
        ptAux->proximo = NULL;
        if (!(*ptPtNoString)) {
            *ptPtNoString = ptAux;
            ptUltimoNo = ptAux;
        } else {
            ptUltimoNo->proximo = ptAux;
            ptUltimoNo = ptAux;
        }
        if (caractere == '\r') {
            caractere = '\n';
            ptAux = (ptNoString) malloc(sizeof (struct noString));
            ptAux->letra = caractere;
            ptAux->proximo = NULL;
            if (!(*ptPtNoString)) {
                *ptPtNoString = ptAux;
                ptUltimoNo = ptAux;
            } else {
                ptUltimoNo->proximo = ptAux;
                ptUltimoNo = ptAux;
            }
        }
    } while (fimDigitacao != 2);
}

void insercaoTextoViaArquivo(ptNoString *ptPtNoString, FILE *arquivo) {//Criar string dinâmica a partir do arquivo ".srt"
    ptNoString ptAux, ptUltimoNo;
    char caractere;
    int fimString = 0, carreto = 0;


    while ((fimString != 2) && (fscanf(arquivo, "%c", &caractere) != EOF)) {
        if (caractere == '\r') {
            carreto = 1;
        }
        if ((caractere != '\n') && (caractere != '\r')) {
            carreto = 0;
        }
        if ((caractere == '\n') && (carreto == 0)) {
            caractere = '\r';
        }

        if (caractere == '\n') {
            fimString++;
        }
        if ((caractere != '\n') && (caractere != '\r')) {
            fimString = 0;
        }

        ptAux = (ptNoString) malloc(sizeof (struct noString));
        ptAux->letra = caractere;
        ptAux->proximo = NULL;
        if (!(*ptPtNoString)) {// Se ptNoString apontar para null, ou seja, não existir nenhum noString vinculado
            *ptPtNoString = ptAux;
            ptUltimoNo = ptAux;
        } else {
            ptUltimoNo->proximo = ptAux;
            ptUltimoNo = ptAux;
        }
        if ((caractere == '\r') && (carreto == 0)) {
            caractere = '\n';
            ptAux = (ptNoString) malloc(sizeof (struct noString));
            ptAux->letra = caractere;
            ptAux->proximo = NULL;
            if (!(*ptPtNoString)) {
                *ptPtNoString = ptAux;
                ptUltimoNo = ptAux;
            } else {
                ptUltimoNo->proximo = ptAux;
                ptUltimoNo = ptAux;
            }
        }
    }
    if (fimString != 2) {
        ptAux = (ptNoString) malloc(sizeof (struct noString));
        ptAux->letra = '\r';
        ptAux->proximo = NULL;
        ptUltimoNo->proximo = ptAux;
        ptUltimoNo = ptAux;

        ptAux = (ptNoString) malloc(sizeof (struct noString));
        ptAux->letra = '\n';
        ptAux->proximo = NULL;
        ptUltimoNo->proximo = ptAux;
        ptUltimoNo = ptAux;
    }
}

void escreveTextoTela(ptNoString pt) {//escrever string dinâmica na tela
    while (pt) { //enquanto pt != null
        printf("%c", pt->letra);
        pt = pt->proximo;
    }
}

void escreveTextoArquivo(ptNoString pt, FILE *arquivo) {//escrever string dinâmica no arquivo
    while (pt) { //enquanto pt != null
        fprintf(arquivo, "%c", pt->letra);
        pt = pt->proximo;
    }
}

void deletarTexto(ptNoString texto) {//Deletar string dinâmica, liberando todos os nós criados com malloc
    ptNoString aux;

    while (texto) {
        aux = texto->proximo;
        free(texto);
        texto = aux;
    }
}

ptNoLegenda encontraLegenda(int nro, ptNoLegenda pt) {//nro deve ser >= 1
    int i;

    for (i = 1; i < nro; i++) {
        pt = pt->proximo;
    }

    return pt;
}

void escreveNoLegendaNoArquivo(ptNoLegenda pt, FILE *arquivo) {
    fprintf(arquivo, "%s", pt->inicio);
    fprintf(arquivo, " --> ");
    fprintf(arquivo, "%s", pt->fim);
    escreveTextoArquivo(pt->texto, arquivo);
}

void escreveTodasLegendasNoArquivo(ptNoLegenda pt, int nroTotalLegendas, FILE *arquivo) {
    int i;

    for (i = 0; i < nroTotalLegendas; i++) {
        fprintf(arquivo, "%d\r\n", (i + 1));
        escreveNoLegendaNoArquivo(pt, arquivo);
        pt = pt->proximo;
    }
}

void escreveNoLegendaNaTela(ptNoLegenda pt) {
    printf("%s", pt->inicio);
    printf(" --> ");
    printf("%s", pt->fim);
    escreveTextoTela(pt->texto);
}

void escreveLegendasNaTela(ptNoLegenda pt, int cont, int nroTotalLegendas) {
    int i = 0;

    printf("\n");
    while ((i < 6) && (cont <= nroTotalLegendas)) {
        printf("%d\n", (cont));
        escreveNoLegendaNaTela(pt);
        pt = pt->proximo;
        cont++;
        i++;
    }
}

void escreveTodasLegendasNaTela(ptNoLegenda pt, int nroTotalLegendas) {
    int i;

    printf("\n");
    for (i = 0; i < nroTotalLegendas; i++) {
        printf("%d\n", (i + 1));
        escreveNoLegendaNaTela(pt);
        pt = pt->proximo;
    }
}

void criarNoLegenda(ptNoLegenda *ptPtNoLegenda, int nro, int *ptNroTotalLegendas, char inicio[13], char fim[13], ptNoString texto) {
    ptNoLegenda ptAux, anterior;

    anterior = encontraLegenda((nro - 1), (*ptPtNoLegenda));

    if ((*ptPtNoLegenda)) {// Se ptNoLegenda apontar para algum nó (!= null), ou seja, se existir algum noLegenda vinculado
        if (nro != 1) {
            ptAux = anterior->proximo;
            anterior->proximo = (ptNoLegenda) malloc(sizeof (struct noLegenda));
            anterior->proximo->proximo = ptAux;

            strcpy(anterior->proximo->inicio, inicio);
            strcpy(anterior->proximo->fim, fim);
            anterior->proximo->texto = texto;
        } else {
            ptAux = anterior;
            (*ptPtNoLegenda) = (ptNoLegenda) malloc(sizeof (struct noLegenda));
            (*ptPtNoLegenda)->proximo = ptAux;

            strcpy((*ptPtNoLegenda)->inicio, inicio);
            strcpy((*ptPtNoLegenda)->fim, fim);
            (*ptPtNoLegenda)->texto = texto;
        }
    } else {
        (*ptPtNoLegenda) = (ptNoLegenda) malloc(sizeof (struct noLegenda));
        (*ptPtNoLegenda)->proximo = iniciaLegenda();

        strcpy((*ptPtNoLegenda)->inicio, inicio);
        strcpy((*ptPtNoLegenda)->fim, fim);
        (*ptPtNoLegenda)->texto = texto;
    }

    (*ptNroTotalLegendas)++;
}

void deletaNoLegenda(ptNoLegenda *ptPtNoLegenda, int nro, int *ptNroTotalLegendas) {
    ptNoLegenda deletar, anterior;
    deletar = encontraLegenda(nro, (*ptPtNoLegenda));
    anterior = encontraLegenda((nro - 1), (*ptPtNoLegenda));

    if (nro != 1) {
        anterior->proximo = deletar->proximo;
    } else {//quando entrar neste else, anterior é o ponteiro que contém o endereço do primeiro noLegenda, ou seja, ele é um ponteiro e não um noLegenda, logo não possui o campo próximo.
        (*ptPtNoLegenda) = deletar->proximo;
    }

    deletarTexto(deletar->texto);
    free(deletar);

    (*ptNroTotalLegendas)--;
}

int verificaTempo(char inicio[13], char fim[13]) {//verifica consistência entre tempo inicial e final de uma mesma legenda
    if (strcmp(inicio, fim) >= 0) {
        return 0;
    } else {
        return 1;
    }
}

int verificaConsistenciaInsercao(ptNoLegenda pt, int nro, int nroTotalLegendas, char inicio[13], char fim[13]) {//Verifica consistência de tempo
    if (!(verificaTempo(inicio, fim))) {
        return 0;
    } else {
        if ((nro > 1) && (nro <= nroTotalLegendas)) {//Se a legenda que está sendo verificada não for nem a primeira nem a última da lista
            ptNoLegenda anterior, proxima;

            anterior = encontraLegenda((nro - 1), pt);
            proxima = encontraLegenda(nro, pt);

            if ((strcmp(anterior->fim, inicio) <= 0) && (strcmp(fim, proxima->inicio) <= 0)) {//Se (anterior->fim <= inicio) e (fim <= proximo->inicio) não há inconsistência;
                return 1;
            } else {
                return 0;
            }
        } else {
            if (nro > nroTotalLegendas) {//Se a legenda que está sendo verificada for a última da lista
                ptNoLegenda anterior;

                anterior = encontraLegenda(nroTotalLegendas, pt);

                if (strcmp(anterior->fim, inicio) <= 0) {
                    return 1;
                } else {
                    return 0;
                }
            } else {//Se a legenda que está sendo verificada for a primeira da lista, considerando-se que já exista pelo menos um nó na lista
                ptNoLegenda proxima;

                proxima = pt;

                if (strcmp(fim, proxima->inicio) <= 0) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    }
}

int verificaConsistenciaAtualizacao(ptNoLegenda pt, int nro, int nroTotalLegendas, char inicio[13], char fim[13]) {//Verifica consistência de tempo
    if (!(verificaTempo(inicio, fim))) {
        return 0;
    } else {
        if ((nro > 1) && (nro < nroTotalLegendas)) {//Se a legenda que está sendo verificada não for nem a primeira nem a última da lista
            ptNoLegenda anterior, proxima;

            anterior = encontraLegenda((nro - 1), pt);
            proxima = encontraLegenda((nro + 1), pt);

            if ((strcmp(anterior->fim, inicio) <= 0) && (strcmp(fim, proxima->inicio) <= 0)) {//Se (anterior->fim <= inicio) e (fim <= proximo->inicio) não há inconsistência;
                return 1;
            } else {
                return 0;
            }
        } else {//Se a legenda que está sendo verificada for a última da lista
            ptNoLegenda anterior;

            anterior = encontraLegenda((nro - 1), pt);

            if (strcmp(anterior->fim, inicio) <= 0) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}

int receberNroLegendaInsercao(int nroTotalLegendas) {//Recebe número da legenda PARA SER UTILIZADO NA FUNÇÃO "inserirLegenda", verificando consistência
    int nro;

    printf("Informe o número da legenda: ");
    scanf("%d", &nro);
    while ((nro <= 0) || (nro > (nroTotalLegendas + 1))) {
        printf("Erro, número de legenda incorreto!\n");
        printf("Informe o número da legenda: ");
        scanf("%d", &nro);
    }

    return nro;
}

int receberNroLegendaRemocao(int nroTotalLegendas) {//Recebe número da legenda PARA SER UTILIZADO NA FUNÇÃO "removerNoLegenda", verificando consistência
    int nro;

    printf("Informe o número da legenda: ");
    scanf("%d", &nro);
    while ((nro <= 0) || (nro > (nroTotalLegendas))) {
        printf("Erro, número de legenda incorreto!\n");
        printf("Informe o número da legenda: ");
        scanf("%d", &nro);
    }

    return nro;
}

void inserirLegenda(ptNoLegenda *ptPtNoLegenda, int *ptNroTotalLegendas) {
    int nro;
    char inicio[13];
    char fim[13];
    ptNoString texto;
    texto = iniciaString();

    nro = receberNroLegendaInsercao((*ptNroTotalLegendas));
    printf("Informe o momento inicial da legenda (ex 00:00:00,000): ");
    scanf("%s", inicio);
    printf("Informe o momento final da legenda (ex 00:00:00,001): ");
    scanf("%s", fim);

    if ((*ptNroTotalLegendas) > 0) {//Se já houver algum noLegenda na lista é necessário verificar a consistência dos tempos(entra no if), caso contrário não(não entra no if)
        while (!(verificaConsistenciaInsercao((*ptPtNoLegenda), nro, (*ptNroTotalLegendas), inicio, fim))) {
            printf("Erro, tempo inicial e/ou final inconsistente!\n");
            nro = receberNroLegendaInsercao((*ptNroTotalLegendas));
            printf("Informe o momento inicial da legenda (ex 00:00:00,000): ");
            scanf("%s", inicio);
            printf("Informe o momento final da legenda (ex 00:00:00,001): ");
            scanf("%s", fim);
        }
    } else {
        while (!(verificaTempo(inicio, fim))) {
            printf("Erro, tempo inicial e/ou final inconsistente!\n");
            nro = receberNroLegendaInsercao((*ptNroTotalLegendas));
            printf("Informe o momento inicial da legenda (ex 00:00:00,000): ");
            scanf("%s", inicio);
            printf("Informe o momento final da legenda (ex 00:00:00,001): ");
            scanf("%s", fim);
        }
    }

    printf("Informe o texto da legenda: ");
    insercaoTextoViaTeclado(&texto);
    criarNoLegenda(ptPtNoLegenda, nro, ptNroTotalLegendas, inicio, fim, texto);
}

void removerNoLegenda(ptNoLegenda *ptPtNoLegenda, int *ptNroTotalLegendas) {
    if ((*ptNroTotalLegendas) != 0) {
        deletaNoLegenda(ptPtNoLegenda, receberNroLegendaRemocao(*ptNroTotalLegendas), ptNroTotalLegendas);
    } else {
        printf("Erro, não há nenhuma legenda!");
    }
}

void desalocaTodasLegendas(ptNoLegenda pt) {
    ptNoLegenda aux;

    while (pt) {
        aux = pt->proximo;
        deletarTexto(pt->texto);
        free(pt);
        pt = aux;
    }
}

void consultarLegenda(ptNoLegenda pt, int nroTotalLegendas) {
    char inicio[13];
    int cont = 1;
    ptNoLegenda ultimoNoLegenda = encontraLegenda(nroTotalLegendas, pt);

    if (nroTotalLegendas != 0) {//Se ja houver algum noLegenda na lista
        printf("Informe o tempo inicial da legenda (ex 00:00:00,000): ");
        scanf("%s", inicio);
        if (strcmp(inicio, ultimoNoLegenda->inicio) < 0) {//Se o tempo informado for menor que o tempo inicial da ultima legenda presente na lista
            while (strcmp(inicio, pt->inicio) >= 0) {
                pt = pt->proximo;
                cont++;
            }
            escreveLegendasNaTela(pt, cont, nroTotalLegendas);
        } else {
            printf("Não há legendas posteriores ao tempo informado!");
        }
    } else {
        printf("Não há nenhuma legenda!");
    }
}

void atualizaLegenda(ptNoLegenda pt, int nroTotalLegendas) {
    if (nroTotalLegendas > 0) {
        int nro;
        char inicio[13];
        char fim[13];

        nro = receberNroLegendaRemocao(nroTotalLegendas);
        printf("Informe o novo momento inicial da legenda (ex 00:00:00,000): ");
        scanf("%s", inicio);
        printf("Informe o novo momento final da legenda (ex 00:00:00,001): ");
        scanf("%s", fim);

        if (nroTotalLegendas > 1) {
            while (!(verificaConsistenciaAtualizacao(pt, nro, nroTotalLegendas, inicio, fim))) {
                printf("Erro, tempo inicial e/ou final inconsistente!\n");
                printf("Informe o novo momento inicial da legenda (ex 00:00:00,000): ");
                scanf("%s", inicio);
                printf("Informe o novo momento final da legenda (ex 00:00:00,001): ");
                scanf("%s", fim);
            }
            pt = encontraLegenda(nro, pt);
        } else {
            while (!(verificaTempo(inicio, fim))) {
                printf("Erro, tempo inicial e/ou final inconsistente!\n");
                printf("Informe o novo momento inicial da legenda (ex 00:00:00,000): ");
                scanf("%s", inicio);
                printf("Informe o novo momento final da legenda (ex 00:00:00,001): ");
                scanf("%s", fim);
            }
        }

        strcpy(pt->inicio, inicio);
        strcpy(pt->fim, fim);
        deletarTexto(pt->texto);
        pt->texto = iniciaString();
        printf("Informe o texto da legenda: ");
        insercaoTextoViaTeclado(&(pt->texto));
    } else {
        printf("Erro, não há nenhuma legenda!");
    }
}

void atualizaTempo(ptNoLegenda pt, int nroTotalLegendas) {
    if (nroTotalLegendas > 0) {
        int nro, horasInicial, horasFinal, minutosInicial, minutosFinal, segundosInicial, segundosFinal, milisegundosInicial, milisegundosFinal;
        int horasAdd, minutosAdd, segundosAdd, milisegundosAdd, tempoTotalInicial, tempoTotalFinal, tempoTotalAdd;
        char tempoAdd[13], vetaux[4];

        nro = receberNroLegendaRemocao(nroTotalLegendas);
        pt = encontraLegenda(nro, pt);
        printf("Informe o Tempo a ser adicionado (00:00:00,001): ");
        scanf("%s", tempoAdd);
        sscanf(tempoAdd, "%d:%d:%d,%d", &horasAdd, &minutosAdd, &segundosAdd, &milisegundosAdd);

        while (pt) {
            //Convertendo tempos em string para o tipo int
            sscanf(pt->inicio, "%d:%d:%d,%d", &horasInicial, &minutosInicial, &segundosInicial, &milisegundosInicial);
            sscanf(pt->fim, "%d:%d:%d,%d", &horasFinal, &minutosFinal, &segundosFinal, &milisegundosFinal);

            //Convertendo tempos totais para milisegundos
            tempoTotalInicial = (milisegundosInicial + (segundosInicial * 1000) + (minutosInicial * 60000) + (horasInicial * 3600000));
            tempoTotalFinal = (milisegundosFinal + (segundosFinal * 1000) + (minutosFinal * 60000) + (horasFinal * 3600000));
            tempoTotalAdd = (milisegundosAdd + (segundosAdd * 1000) + (minutosAdd * 60000) + (horasAdd * 3600000));

            tempoTotalInicial += tempoTotalAdd;
            tempoTotalFinal += tempoTotalAdd;

            //Convertendo tempos de volta para hh:mm:ss,mmm
            horasInicial = (tempoTotalInicial / 3600000);
            tempoTotalInicial = (tempoTotalInicial % 3600000);
            minutosInicial = (tempoTotalInicial / 60000);
            tempoTotalInicial = (tempoTotalInicial % 60000);
            segundosInicial = (tempoTotalInicial / 1000);
            milisegundosInicial = (tempoTotalInicial % 1000);

            horasFinal = (tempoTotalFinal / 3600000);
            tempoTotalFinal = (tempoTotalFinal % 3600000);
            minutosFinal = (tempoTotalFinal / 60000);
            tempoTotalFinal = (tempoTotalFinal % 60000);
            segundosFinal = (tempoTotalFinal / 1000);
            milisegundosFinal = (tempoTotalFinal % 1000);

            //Covertendo tempos int de volta para string
            if (horasInicial < 10) {
                pt->inicio[0] = '0';
                pt->inicio[1] = (char) (horasInicial + 48);
            } else {
                sprintf(vetaux, "%d", horasInicial);
                pt->inicio[0] = vetaux[0];
                pt->inicio[1] = vetaux[1];
            }
            pt->inicio[2] = ':';

            if (minutosInicial < 10) {
                pt->inicio[3] = '0';
                pt->inicio[4] = (char) (minutosInicial + 48);
            } else {
                sprintf(vetaux, "%d", minutosInicial);
                pt->inicio[3] = vetaux[0];
                pt->inicio[4] = vetaux[1];
            }
            pt->inicio[5] = ':';

            if (segundosInicial < 10) {
                pt->inicio[6] = '0';
                pt->inicio[7] = (char) (segundosInicial + 48);
            } else {
                sprintf(vetaux, "%d", segundosInicial);
                pt->inicio[6] = vetaux[0];
                pt->inicio[7] = vetaux[1];
            }
            pt->inicio[8] = ',';

            sprintf(vetaux, "%d", milisegundosInicial);
            if (milisegundosInicial < 100) {
                pt->inicio[9] = '0';
                if (milisegundosInicial > 10) {
                    pt->inicio[10] = vetaux[0];
                    pt->inicio[11] = vetaux[1];
                } else {
                    pt->inicio[10] = '0';
                    pt->inicio[11] = vetaux[0];
                }
            } else {
                pt->inicio[9] = vetaux[0];
                pt->inicio[10] = vetaux[1];
                pt->inicio[11] = vetaux[2];
            }
            pt->inicio[12] = '\0';


            if (horasFinal < 10) {
                pt->fim[0] = '0';
                pt->fim[1] = (char) (horasFinal + 48);
            } else {
                sprintf(vetaux, "%d", horasFinal);
                pt->fim[0] = vetaux[0];
                pt->fim[1] = vetaux[1];
            }
            pt->fim[2] = ':';

            if (minutosFinal < 10) {
                pt->fim[3] = '0';
                pt->fim[4] = (char) (minutosFinal + 48);
            } else {
                sprintf(vetaux, "%d", minutosFinal);
                pt->fim[3] = vetaux[0];
                pt->fim[4] = vetaux[1];
            }
            pt->fim[5] = ':';

            if (segundosFinal < 10) {
                pt->fim[6] = '0';
                pt->fim[7] = (char) (segundosFinal + 48);
            } else {
                sprintf(vetaux, "%d", segundosFinal);
                pt->fim[6] = vetaux[0];
                pt->fim[7] = vetaux[1];
            }
            pt->fim[8] = ',';

            sprintf(vetaux, "%d", milisegundosFinal);
            if (milisegundosFinal < 100) {
                pt->fim[9] = '0';
                if (milisegundosFinal > 10) {
                    pt->fim[10] = vetaux[0];
                    pt->fim[11] = vetaux[1];
                } else {
                    pt->fim[10] = '0';
                    pt->fim[11] = vetaux[0];
                }
            } else {
                pt->fim[9] = vetaux[0];
                pt->fim[10] = vetaux[1];
                pt->fim[11] = vetaux[2];
            }
            pt->fim[12] = '\0';


            pt = pt->proximo;
        }
    } else {
        printf("Erro, não há nenhuma legenda!");
    }
}

void lerArquivo(FILE *arquivo, ptNoLegenda *ptPtNoLegenda, int *nroTotalLegendas) {
    char descarte[4], inicio[13], fim[13];
    ptNoString texto;

    while (fscanf(arquivo, "%s", descarte) != EOF) {
        texto = iniciaString();
        fscanf(arquivo, "%s", inicio);
        fscanf(arquivo, "%s", descarte);
        fscanf(arquivo, "%s", fim);
        insercaoTextoViaArquivo(&texto, arquivo);
        criarNoLegenda(ptPtNoLegenda, ((*nroTotalLegendas) + 1), nroTotalLegendas, inicio, fim, texto);
        //deletarTexto(texto);
    }
}

void escreveMenuPrincipal() {

    puts("\n  |------------------------------------------------|\n");
    puts("  |--------------------- Menu ---------------------|\n");
    puts("  |---------------------- de ----------------------|\n");
    puts("  |------------------- Legendas -------------------|\n");
    puts("  |                                                |\n");
    puts("  |  1-Inserir Legenda         2-Remover Legenda   |\n");
    puts("  |                                                |\n");
    puts("  |  3-Consultar Legenda       4-Atualizar Legenda |\n");
    puts("  |                                                |\n");
    puts("  |  5-Atualizar Tempo         6-Mostar Legendas   |\n");
    puts("  |                                                |\n");
    puts("  |  0-Sair                                        |\n");
    puts("  --------------------------------------------------");
}

int menu(ptNoLegenda *ptPtNoLegenda, int *ptNroTotalLegendas) {// tratar excessoes de nro legenda
    int resposta = 1;

    while (resposta) {
        escreveMenuPrincipal();
        scanf("%d", &resposta);
        switch (resposta) {
            case 0:
                return 0;
            case 1:
                inserirLegenda(ptPtNoLegenda, ptNroTotalLegendas);
                return 1;
            case 2:
                removerNoLegenda(ptPtNoLegenda, ptNroTotalLegendas);
                return 1;
            case 3:
                consultarLegenda((*ptPtNoLegenda), (*ptNroTotalLegendas));
                return 1;
            case 4:
                atualizaLegenda((*ptPtNoLegenda), (*ptNroTotalLegendas));
                return 1;
            case 5:
                atualizaTempo((*ptPtNoLegenda), (*ptNroTotalLegendas));
                return 1;
            case 6:
                escreveTodasLegendasNaTela((*ptPtNoLegenda), (*ptNroTotalLegendas));
                return 1;
        }
    }
}

int main() {
    int nroTotalLegendas = 0;
    ptNoLegenda pt = iniciaLegenda();
    char nomeLegenda[100];
    FILE *arquivo;

    printf("Informe o nome do arquivo de legenda (ex: arquivo.srt):\n");
    scanf("%s", nomeLegenda);
    arquivo = fopen(nomeLegenda, "r");
    if (arquivo != NULL) {
        lerArquivo(arquivo, &pt, &nroTotalLegendas);
        fclose(arquivo);
    }
    
    while (menu(&pt, &nroTotalLegendas));

    arquivo = fopen(nomeLegenda, "w");
    escreveTodasLegendasNoArquivo(pt, nroTotalLegendas, arquivo);
    fclose(arquivo);
    desalocaTodasLegendas(pt);
}
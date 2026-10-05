/******************************************************************************
* Programa: Jogo de Quebra-Cabeca de Palavras
* Descricao: Organizar letras em um tabuleiro 4x4 para formar palavras
*             Versao sem variaveis globais
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_CAD 50 /* Quantidade maxima de palavras cadastraveis */

/***********************************************************************
* Apresenta o tabuleiro 4x4 na tela
* Parametros:
*   tab - matriz 4x4 de caracteres (tabuleiro do jogo)
***********************************************************************/
void mostrarTabuleiro(char tab[4][4]) {
    int i, j; /* i = linha, j = coluna */

    /* Cabecalho com numeros das colunas */
    printf("\n   1  2  3  4\n");

    /* Percorre cada linha do tabuleiro */
    for (i = 0; i < 4; i++) {
        printf("%d  ", i + 1);

        /* Percorre cada coluna da linha atual */
        for (j = 0; j < 4; j++)
            printf("%c  ", tab[i][j]);

        printf("\n");
    }
    printf("\n");
}

/***********************************************************************
* Gera um numero inteiro aleatorio entre 1 e 15
* Nao recebe parametros
* Retorno: numero inteiro entre 1 e 15
***********************************************************************/
int numeroAleatorio() {
    return rand() % 15 + 1;
}

/***********************************************************************
* Obtem do jogador a linha e coluna da letra a ser trocada
* Valida se os valores estao entre 1 e 4 (critica de dados)
* Usa 0 como sentinela para interromper o jogo
* Parametros:
*   p_lin - ponteiro para a linha escolhida (parametro de saida)
*   p_col - ponteiro para a coluna escolhida (parametro de saida)
***********************************************************************/
void obterPosicao(int *p_lin, int *p_col) {
    int valido; /* Controle de validacao do valor digitado */

    /* Leitura e validacao da linha */
    valido = 0;
    while (!valido) {
        printf("Linha (1-4) ou 0 para sair: ");
        scanf("%d", p_lin);

        if (*p_lin == 0) /* Sentinela: jogador quer encerrar */
            return;

        if (*p_lin < 1 || *p_lin > 4)
            printf("Invalido! Digite entre 1 e 4.\n");
        else
            valido = 1; /* Valor aceito, sai do laco */
    }

    /* Leitura e validacao da coluna */
    valido = 0;
    while (!valido) {
        printf("Coluna (1-4) ou 0 para sair: ");
        scanf("%d", p_col);

        if (*p_col == 0) /* Sentinela */
            return;

        if (*p_col < 1 || *p_col > 4)
            printf("Invalido! Digite entre 1 e 4.\n");
        else
            valido = 1;
    }
}

/***********************************************************************
* Realiza a troca de uma letra com o vazio no tabuleiro
* Parametros:
*   lin - linha da letra (1 a 4) (parametro de entrada)
*   col - coluna da letra (1 a 4) (parametro de entrada)
*   tab - matriz 4x4 do tabuleiro (parametro de entrada-saida)
***********************************************************************/
void realizarTroca(int lin, int col, char tab[4][4]) {
    int i, j;          /* Contadores para percorrer o tabuleiro */
    int linVazio = -1; /* Linha onde o vazio esta */
    int colVazio = -1; /* Coluna onde o vazio esta */

    /* Busca a posicao atual do vazio no tabuleiro */
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (tab[i][j] == '-') {
                linVazio = i;
                colVazio = j;
            }

    /* Coloca a letra no lugar do vazio e o vazio no lugar da letra */
    tab[linVazio][colVazio] = tab[lin - 1][col - 1];
    tab[lin - 1][col - 1] = '-';
}

/***********************************************************************
* Verifica se o jogador venceu (todas as palavras organizadas)
* Parametros:
*   tab        - matriz 4x4 do tabuleiro (parametro de entrada)
*   escolhidas - matriz 4x5 com as palavras escolhidas (parametro de entrada)
* Retorno: 1 se venceu, 0 caso contrario
***********************************************************************/
int verificarVitoria(char tab[4][4], char escolhidas[4][5]) {
    int k, l;    /* k = indice da palavra, l = indice da letra */
    int pos = 0; /* Posicao linear que percorre o tabuleiro (0 a 14) */

    /* Percorre cada palavra escolhida */
    for (k = 0; k < 4; k++) {
        int tam = strlen(escolhidas[k]); /* Tamanho da palavra atual */

        /* Percorre cada letra da palavra */
        for (l = 0; l < tam; l++) {
            /* Converte posicao linear em linha e coluna da matriz */
            if (tab[pos / 4][pos % 4] != escolhidas[k][l])
                return 0; /* Letra nao confere, nao venceu */
            pos++;
        }
    }

    return 1; /* Todas as letras estao no lugar correto */
}

/************************************************************/
/* Programa principal: Jogo de Quebra-Cabeca de Palavras    */
/************************************************************/
int main() {
    /* Vetores para cadastro de palavras (matrizes de caracteres) */
    char palavras4[MAX_CAD][5]; /* Palavras de 4 letras cadastradas */
    char palavras3[MAX_CAD][4]; /* Palavras de 3 letras cadastradas */
    int qtde4;                  /* Quantidade de palavras de 4 letras */
    int qtde3;                  /* Quantidade de palavras de 3 letras */

    /* Palavras escolhidas para a partida (antes era global) */
    char escolhidas[4][5]; /* 3 palavras de 4 letras + 1 de 3 letras */

    /* Variaveis do jogo */
    char tabuleiro[4][4];   /* Matriz 4x4 do tabuleiro */
    char sequencia[16];     /* Sequencia linear de 15 letras + '\0' */
    int jogadas;            /* Contador de jogadas realizadas */
    int linha, coluna;      /* Posicao informada pelo jogador */
    int novamente;          /* 1 = jogar de novo, 0 = sair */
    int fimJogo;            /* 1 = jogo atual terminou */

    /* Auxiliares */
    int i, j, k;            /* Contadores de lacos */
    int pos;                /* Posicao atual na sequencia */
    int aleat;              /* Numero aleatorio gerado */
    int escolha;            /* Opcao escolhida pelo jogador */
    int livre;              /* 1 = posicao livre encontrada */
    int linVazio, colVazio; /* Posicao do vazio (validacao de jogada) */

    /* Inicializa o gerador de numeros aleatorios com o tempo atual */
    srand(time(NULL));

    printf("=====================================\n");
    printf("  JOGO DE QUEBRA-CABECA DE PALAVRAS\n");
    printf("=====================================\n\n");

    /**************************/
    /* Cadastro de palavras   */
    /**************************/

    printf("Quantas palavras de 4 letras deseja cadastrar? ");
    scanf("%d", &qtde4);

    for (i = 0; i < qtde4; i++) {
        printf("  Palavra %d: ", i + 1);
        scanf("%s", palavras4[i]);
    }

    printf("\nQuantas palavras de 3 letras deseja cadastrar? ");
    scanf("%d", &qtde3);

    for (i = 0; i < qtde3; i++) {
        printf("  Palavra %d: ", i + 1);
        scanf("%s", palavras3[i]);
    }

    /*************************************/
    /* Loop do jogo (permite jogar novo) */
    /*************************************/

    novamente = 1;

    while (novamente) {

        /****************************************/
        /* Escolha das palavras para a partida  */
        /****************************************/

        printf("\nPalavras de 4 letras:\n");
        for (i = 0; i < qtde4; i++)
            printf("  %d - %s\n", i + 1, palavras4[i]);

        printf("\nEscolha 3 palavras de 4 letras:\n");
        for (i = 0; i < 3; i++) {
            printf("  Palavra %d: ", i + 1);
            scanf("%d", &escolha);
            strcpy(escolhidas[i], palavras4[escolha - 1]);
        }

        printf("\nPalavras de 3 letras:\n");
        for (i = 0; i < qtde3; i++)
            printf("  %d - %s\n", i + 1, palavras3[i]);

        printf("\nEscolha 1 palavra de 3 letras: ");
        scanf("%d", &escolha);
        strcpy(escolhidas[3], palavras3[escolha - 1]);

        /* Mostra as palavras que foram selecionadas */
        printf("\nSelecionadas:");
        for (k = 0; k < 4; k++)
            printf(" [%s]", escolhidas[k]);
        printf("\n");

        /*************************************************/
        /* Montagem da sequencia linear de 15 letras     */
        /*************************************************/

        pos = 0;
        for (k = 0; k < 4; k++) {
            int tam = strlen(escolhidas[k]); /* Tamanho da palavra */
            for (i = 0; i < tam; i++) {
                sequencia[pos] = escolhidas[k][i];
                pos++;
            }
        }
        sequencia[pos] = '\0';

        /********************************************************/
        /* Distribuicao aleatoria das letras no tabuleiro       */
        /********************************************************/

        /* Limpa todas as 16 posicoes do tabuleiro */
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                tabuleiro[i][j] = ' ';

        /* Para cada letra, sorteia uma posicao livre entre 1 e 15 */
        for (k = 0; k < 15; k++) {
            livre = 0;
            while (!livre) {
                aleat = numeroAleatorio();
                i = (aleat - 1) / 4; /* Linha correspondente (0 a 3) */
                j = (aleat - 1) % 4; /* Coluna correspondente (0 a 3) */

                if (tabuleiro[i][j] == ' ') {
                    tabuleiro[i][j] = sequencia[k];
                    livre = 1; /* Posicao preenchida, sai do while */
                }
            }
        }

        /* Posicao 16 (linha 4, coluna 4) recebe o vazio */
        tabuleiro[3][3] = '-';

        /******************************/
        /* Loop principal da partida  */
        /******************************/

        jogadas = 0;
        fimJogo = 0;

        printf("\nTabuleiro inicial:\n");
        mostrarTabuleiro(tabuleiro);

        while (!fimJogo) {
            obterPosicao(&linha, &coluna);

            /* Sentinela: jogador quer encerrar */
            if (linha == 0 || coluna == 0) {
                printf("\nJogo interrompido.\n");
                fimJogo = 1;
            }
            else {
                /* Busca a posicao atual do vazio no tabuleiro */
                linVazio = -1;
                colVazio = -1;
                for (i = 0; i < 4; i++)
                    for (j = 0; j < 4; j++)
                        if (tabuleiro[i][j] == '-') {
                            linVazio = i;
                            colVazio = j;
                        }

                /* Nao pode escolher a propria posicao do vazio */
                if ((linha - 1) == linVazio && (coluna - 1) == colVazio) {
                    printf("Essa posicao ja eh o vazio!\n");
                }
                /* A letra precisa estar na mesma linha ou coluna do vazio */
                else if ((linha - 1) == linVazio || (coluna - 1) == colVazio) {
                    realizarTroca(linha, coluna, tabuleiro);
                    jogadas++;

                    mostrarTabuleiro(tabuleiro);

                    /* Verifica se o jogador atingiu o objetivo */
                    if (verificarVitoria(tabuleiro, escolhidas)) {
                        printf("PARABENS! Voce organizou todas as palavras!\n");
                        fimJogo = 1;
                    }
                }
                else {
                    printf("Jogada invalida! Precisa estar na mesma linha ou coluna do vazio.\n");
                }
            }
        }

        /**************************/
        /* Resultado da partida   */
        /**************************/

        printf("\nTotal de jogadas: %d\n", jogadas);

        printf("\nJogar novamente? (1 = Sim / 0 = Nao): ");
        scanf("%d", &novamente);
    }

    printf("\nObrigado por jogar!\n");

    return 0;
}
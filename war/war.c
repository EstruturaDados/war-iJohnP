// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 8
#define NUM_MISSOES 4
#define TAM_NOME 30
#define TAM_COR 15

// --- Estrutura de Dados ---
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// --- Prototipos das Funcoes ---

// Funcoes de setup e gerenciamento de memoria:
Territorio* alocarMapa(int quantidade);
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

// Funcoes de interface com o usuario:
void exibirMenuPrincipal(void);
void exibirMapa(const Territorio* mapa);
void exibirMissao(int idMissao);

// Funcoes de logica principal do jogo:
void faseDeAtaque(Territorio* mapa);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int sortearMissao(void);
int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador);

// Funcao utilitaria:
void limparBufferEntrada(void);

// --- Funcao Principal (main) ---
int main() {

    setlocale(LC_ALL, "pt_BR.UTF-8");
    // 1. Configuracao Inicial (Setup):
    srand(time(NULL));

    Territorio* mapa = alocarMapa(NUM_TERRITORIOS);
    if (mapa == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para o mapa!\n");
        return 1;
    }

    inicializarTerritorios(mapa);

    char corJogador[] = "azul";
    int missaoJogador = sortearMissao();

    printf("=== WAR ESTRUTURADO - INICIANDO JOGO ===\n");
    printf("Missao atribuida ao jogador %s!\n\n", corJogador);

    // 2. Laco Principal do Jogo (Game Loop):
    int opcao;
    int jogoAtivo = 1;

    do {
        exibirMapa(mapa);
        exibirMissao(missaoJogador);
        exibirMenuPrincipal();

        printf("\nEscolha uma acao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;

            case 2:
                if (verificarVitoria(mapa, missaoJogador, corJogador)) {
                    printf("\n>>> PARABENS! Voce cumpriu sua missao e VENCEU o jogo! <<<\n");
                    jogoAtivo = 0;
                } else {
                    printf("\n>>> Missao ainda nao cumprida. Continue jogando!\n");
                }
                break;

            case 0:
                printf("\nSaindo do jogo...\n");
                jogoAtivo = 0;
                break;

            default:
                printf("\n>>> Opcao invalida! Tente novamente.\n");
                break;
        }

        if (jogoAtivo) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (jogoAtivo);

    // 3. Limpeza:
    liberarMemoria(mapa);
    printf("Jogo encerrado. Memoria liberada.\n");

    return 0;
}

// --- Implementacao das Funcoes ---

// alocarMapa(): Aloca dinamicamente a memoria para o vetor de territorios
Territorio* alocarMapa(int quantidade) {
    return (Territorio*)calloc(quantidade, sizeof(Territorio));
}

// inicializarTerritorios(): Preenche os dados iniciais de cada territorio
void inicializarTerritorios(Territorio* mapa) {
    // Territorios iniciais com cores e tropas balanceadas
    const char* nomes[] = {"Brasil", "Argentina", "Chile", "Peru",
                          "Colombia", "Venezuela", "Uruguai", "Paraguai"};
    const char* cores[] = {"azul", "vermelho", "verde", "amarelo",
                          "azul", "vermelho", "verde", "amarelo"};
    const int tropas[] = {5, 4, 3, 4, 3, 5, 2, 3};

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i]);
        mapa[i].tropas = tropas[i];
    }
}

// liberarMemoria(): Libera a memoria alocada para o mapa
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// exibirMenuPrincipal(): Exibe o menu de acoes disponiveis
void exibirMenuPrincipal(void) {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1 - Realizar ataque\n");
    printf("2 - Verificar vitoria\n");
    printf("0 - Sair do jogo\n");
}

// exibirMapa(): Mostra o estado atual de todos os territorios
void exibirMapa(const Territorio* mapa) {
    printf("\n=== MAPA DO MUNDO ===\n");
    printf("+------------------------+----------------+--------+\n");
    printf("| TERRITORIO             | COR            | TROPAS |\n");
    printf("+------------------------+----------------+--------+\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-22s | %-14s | %6d |\n",
               mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }

    printf("+------------------------+----------------+--------+\n");
}

// exibirMissao(): Exibe a descricao da missao atual
void exibirMissao(int idMissao) {
    printf("\n=== SUA MISSAO ===\n");

    switch (idMissao) {
        case 0:
            printf(">>> DESTRUIR o exercito VERMELHO completamente!\n");
            break;
        case 1:
            printf(">>> CONQUISTAR pelo menos 4 territorios!\n");
            break;
        case 2:
            printf(">>> DESTRUIR o exercito VERDE completamente!\n");
            break;
        case 3:
            printf(">>> CONQUISTAR pelo menos 5 territorios!\n");
            break;
        default:
            printf(">>> Missao secreta especial!\n");
            break;
    }
}

// faseDeAtaque(): Gerencia a interface para acao de ataque
void faseDeAtaque(Territorio* mapa) {
    printf("\n=== FASE DE ATAQUE ===\n");

    // Mostrar territorios disponiveis para ataque
    printf("Territorios disponiveis para ataque:\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor, "azul") == 0 && mapa[i].tropas > 1) {
            printf("%d - %s (%d tropas)\n", i + 1, mapa[i].nome, mapa[i].tropas);
        }
    }

    int origem, destino;

    printf("\nSelecione o territorio de ORIGEM (numero): ");
    scanf("%d", &origem);
    limparBufferEntrada();

    printf("Selecione o territorio de DESTINO (numero): ");
    scanf("%d", &destino);
    limparBufferEntrada();

    // Validar indices
    if (origem < 1 || origem > NUM_TERRITORIOS ||
        destino < 1 || destino > NUM_TERRITORIOS) {
        printf(">>> Territorios invalidos!\n");
        return;
    }

    // Ajustar para indices base 0
    origem--;
    destino--;

    // Validar se o territorio de origem pertence ao jogador
    if (strcmp(mapa[origem].cor, "azul") != 0) {
        printf(">>> Voce so pode atacar com territorios azuis!\n");
        return;
    }

    // Validar se nao esta atacando proprio territorio
    if (strcmp(mapa[origem].cor, mapa[destino].cor) == 0) {
        printf(">>> Nao pode atacar territorio da mesma cor!\n");
        return;
    }

    // Validar tropas minimas
    if (mapa[origem].tropas <= 1) {
        printf(">>> Territorio precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }

    // Realizar ataque
    simularAtaque(&mapa[origem], &mapa[destino]);
}

// simularAtaque(): Executa a logica de uma batalha entre dois territorios
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    printf("\n*** BATALHA: %s (%s) vs %s (%s) ***\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Rolar dados para atacante e defensor
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // Atacante vence
        printf(">>> VITORIA! %s conquistou %s!\n", atacante->nome, defensor->nome);

        // Calcular tropas conquistadas (minimo 1)
        int tropasConquistadas = defensor->tropas / 2;
        if (tropasConquistadas < 1) tropasConquistadas = 1;

        // Atualizar territorio conquistado
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = tropasConquistadas;

        // Mover uma tropa do atacante para o conquistado
        atacante->tropas--;
        defensor->tropas++;

        printf(">>> %s perdeu 1 tropa, agora tem %d tropas\n",
               atacante->nome, atacante->tropas);
        printf(">>> %s agora tem %d tropas\n", defensor->nome, defensor->tropas);

    } else {
        // Defensor vence ou empate
        printf(">>> DERROTA! %s defendeu com sucesso!\n", defensor->nome);

        // Atacante perde uma tropa
        atacante->tropas--;
        printf(">>> %s perdeu 1 tropa, agora tem %d tropas\n",
               atacante->nome, atacante->tropas);
    }
}

// sortearMissao(): Sorteia e retorna um ID de missao aleatorio
int sortearMissao(void) {
    return rand() % NUM_MISSOES;
}

// verificarVitoria(): Verifica se o jogador cumpriu os requisitos da missao
int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador) {
    int territoriosConquistados = 0;
    int exercitoVermelhoDestruido = 1;
    int exercitoVerdeDestruido = 1;

    // Contar territorios e verificar exercitos inimigos
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territoriosConquistados++;
        }

        if (strcmp(mapa[i].cor, "vermelho") == 0 && mapa[i].tropas > 0) {
            exercitoVermelhoDestruido = 0;
        }

        if (strcmp(mapa[i].cor, "verde") == 0 && mapa[i].tropas > 0) {
            exercitoVerdeDestruido = 0;
        }
    }

    // Verificar condicoes de vitoria baseadas na missao
    switch (idMissao) {
        case 0: // Destruir exercito vermelho
            return exercitoVermelhoDestruido;

        case 1: // Conquistar 4 territorios
            return territoriosConquistados >= 4;

        case 2: // Destruir exercito verde
            return exercitoVerdeDestruido;

        case 3: // Conquistar 5 territorios
            return territoriosConquistados >= 5;

        default:
            return 0;
    }
}

// limparBufferEntrada(): Limpa o buffer de entrada do teclado
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

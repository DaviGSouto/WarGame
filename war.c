#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- ESTRUTURAS ---
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- VARIÁVEIS GLOBAIS ---
Territorio* g_mapa_global = NULL;
int g_num_territorios_global = 0;
char g_cor_jogador_global[10];
char g_territorio_principal_global[30];

// --- PROTÓTIPOS ---
Territorio* cadastrarTerritorios(int* num_territorios);
void exibirMapa(const Territorio* mapa, int num_territorios);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);
void pausar();
void limparBuffer();

void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* cor_jogador);
void exibirMissao(const char* missao, const char* cor_jogador);

// ------------------------------------------------------------
// LIMPA BUFFER
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// ------------------------------------------------------------
// CADASTRAR TERRITÓRIOS
Territorio* cadastrarTerritorios(int* num_territorios) {
    int i;
    Territorio* mapa;

    printf("Quantos territorios voce deseja cadastrar? ");
    if (scanf("%d", num_territorios) != 1 || *num_territorios <= 0) {
        printf("Numero invalido. Usando 5 como padrao.\n");
        *num_territorios = 5;
    }
    limparBuffer();

    mapa = (Territorio*) calloc(*num_territorios, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    printf("\nCadastrando %d territorios...\n\n", *num_territorios);

    for (i = 0; i < *num_territorios; i++) {
        printf("--- Territorio %d ---\n", i + 1);

        printf("Nome: ");
        scanf("%29s", mapa[i].nome);
        limparBuffer();

        printf("Cor do exercito: ");
        scanf("%9s", mapa[i].cor);
        limparBuffer();

        printf("Tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1)
            mapa[i].tropas = 1;
        limparBuffer();

        printf("\n");
    }

    return mapa;
}

// ------------------------------------------------------------
// EXIBIR MAPA
void exibirMapa(const Territorio* mapa, int num_territorios) {
    printf("===================================\n");
    printf("        MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("===================================\n\n");

    for (int i = 0; i < num_territorios; i++) {
        printf("%d. %s (Exercito %s, Tropas %d)\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
    printf("\n");
}

// ------------------------------------------------------------
// ATAQUE
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O Atacante %s rolou: %d\n", atacante->nome, dadoA);
    printf("O Defensor %s rolou: %d\n", defensor->nome, dadoD);

    if (dadoA > dadoD) {
        if (defensor->tropas <= 1) {
            printf("VITORIA! Territorio %s CONQUISTADO!\n", defensor->nome);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = atacante->tropas / 2;
            atacante->tropas -= defensor->tropas;
        } else {
            defensor->tropas--;
            printf("O defensor perdeu 1 tropa.\n");
        }
    } else {
        atacante->tropas--;
        printf("DERROTA! O atacante perdeu 1 tropa.\n");
    }

    if (atacante->tropas < 0) atacante->tropas = 0;
    if (defensor->tropas < 0) defensor->tropas = 0;
}

// ------------------------------------------------------------
// LIBERAR MEMÓRIA
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// ------------------------------------------------------------
// PAUSAR
void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// ------------------------------------------------------------
// EXIBIR MISSÃO
void exibirMissao(const char* missao, const char* cor_jogador) {
    printf("--- SUA MISSAO (Exercito %s) ---\n", cor_jogador);
    printf("%s\n\n", missao);
}

// ------------------------------------------------------------
// ATRIBUIR MISSÃO (CORRIGIDO!)
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    char buffer[200];

    if (idx == 0) {
        snprintf(buffer, sizeof(buffer), "Conquistar o territorio %s", g_territorio_principal_global);
    }
    else if (idx == 1) {
        strcpy(buffer, "Controlar pelo menos 3 territorios");
    }
    else {
        // missão 3: cor alvo aleatória EXCETO do jogador
        char corAlvo[10];
        strcpy(corAlvo, g_cor_jogador_global);

        int tentativas = 0;
        while (strcmp(corAlvo, g_cor_jogador_global) == 0 && tentativas < 20) {
            strcpy(corAlvo, g_mapa_global[rand() % g_num_territorios_global].cor);
            tentativas++;
        }

        snprintf(buffer, sizeof(buffer), "Eliminar todas as tropas da cor %s", corAlvo);
    }

    strcpy(destino, buffer);
}

// ------------------------------------------------------------
// VERIFICAR MISSÃO (CORRIGIDO!)
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* cor_jogador) {

    // Missão 1: Conquistar o território X
    if (strncmp(missao, "Conquistar o territorio ", 24) == 0) {

        char alvo[30];
        strcpy(alvo, missao + 24);

        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].nome, alvo) == 0) {
                return strcmp(mapa[i].cor, cor_jogador) == 0;
            }
        }
        return 0;
    }

    // Missão 2
    if (strcmp(missao, "Controlar pelo menos 3 territorios") == 0) {
        int count = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, cor_jogador) == 0)
                count++;
        return count >= 3;
    }

    // Missão 3
    if (strncmp(missao, "Eliminar todas as tropas da cor ", 32) == 0) {
        char alvoCor[10];
        strcpy(alvoCor, missao + 32);

        int total = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, alvoCor) == 0)
                total += mapa[i].tropas;

        return total == 0;
    }

    return 0;
}

// ------------------------------------------------------------
// MAIN
int main() {
    Territorio* mapa = NULL;
    int num_territorios = 0;
    srand(time(NULL));

    mapa = cadastrarTerritorios(&num_territorios);

    g_mapa_global = mapa;
    g_num_territorios_global = num_territorios;

    int idx_principal = -1;
    do {
        printf("Escolha seu territorio principal (1-%d): ", num_territorios);
        scanf("%d", &idx_principal);
        limparBuffer();
    } while (idx_principal < 1 || idx_principal > num_territorios);

    strcpy(g_cor_jogador_global, mapa[idx_principal - 1].cor);
    strcpy(g_territorio_principal_global, mapa[idx_principal - 1].nome);

    char* missoes[3] = {
        "Conquistar o territorio %s",
        "Controlar pelo menos 3 territorios",
        "Eliminar todas as tropas da cor %s"
    };

    char* missao = malloc(200);
    atribuirMissao(missao, missoes, 3);

    printf("\n--- MISSAO ATRIBUIDA ---\n");
    exibirMissao(missao, g_cor_jogador_global);
    pausar();

    int opcao;
    int jogo = 0;

    while (!jogo) {

        exibirMapa(mapa, num_territorios);
        exibirMissao(missao, g_cor_jogador_global);

        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 0) break;

        if (opcao == 2) {
            if (verificarMissao(missao, mapa, num_territorios, g_cor_jogador_global)) {
                printf("MISSAO CONCLUIDA!\n");
                jogo = 1;
            } else {
                printf("Ainda nao concluiu.\n");
            }
            pausar();
            continue;
        }

        if (opcao == 1) {
            int a, d;

            printf("Atacante (1-%d, 0 cancela): ", num_territorios);
            scanf("%d", &a); limparBuffer();
            if (a == 0) continue;

            printf("Defensor (1-%d): ", num_territorios);
            scanf("%d", &d); limparBuffer();

            if (a < 1 || a > num_territorios || d < 1 || d > num_territorios || a == d) {
                printf("Opcao invalida.\n");
                pausar();
                continue;
            }

            if (strcmp(mapa[a-1].cor, mapa[d-1].cor) == 0) {
                printf("Nao pode atacar territorio da mesma cor.\n");
                pausar();
                continue;
            }

            atacar(&mapa[a-1], &mapa[d-1]);
            pausar();

            if (verificarMissao(missao, mapa, num_territorios, g_cor_jogador_global)) {
                printf("MISSAO CONCLUIDA!\n");
                jogo = 1;
                pausar();
            }

            continue;
        }

        printf("Opcao invalida.\n");
        pausar();
    }

    free(missao);
    liberarMemoria(mapa);
    return 0;
}
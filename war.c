#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct Territorio para armazenar os dados de cada territorio.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Funcao para alocar memoria dinamicamente e cadastrar os territorios.
Territorio* cadastrarTerritorios(int* num_territorios);

// Funcao para exibir o estado atual do mapa.
void exibirMapa(const Territorio* mapa, int num_territorios);

// Funcao para simular o ataque entre dois territorios.
void atacar(Territorio* atacante, Territorio* defensor);

// Funcao para liberar a memoria alocada.
void liberarMemoria(Territorio* mapa);

// Funcao para pausar a execucao.
void pausar();

// --- IMPLEMENTACAO DAS FUNCOES ---

// Funcao para cadastrar territorios com alocacao dinamica.
Territorio* cadastrarTerritorios(int* num_territorios) {
    int i;
    Territorio* mapa;
    
    printf("Quantos territorios voce deseja cadastrar? ");
    if (scanf("%d", num_territorios) != 1 || *num_territorios <= 0) {
        printf("Numero de territorios invalido. Usando 5 como padrao.\n");
        *num_territorios = 5;
    }
    
    mapa = (Territorio*) calloc(*num_territorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    
    while (getchar() != '\n');

    printf("\nVamos cadastrar os %d territorios iniciais do nosso mundo.\n\n", *num_territorios);

    for (i = 0; i < *num_territorios; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        printf("Nome do Territorio: ");
        scanf("%29s", (mapa + i)->nome); 
        
        printf("Cor do exercito: ");
        scanf("%9s", (mapa + i)->cor);

        printf("Numero de Tropas: ");
        scanf("%d", &(mapa + i)->tropas);
        
        while (getchar() != '\n');
        
        printf("\n");
    }
    
    return mapa;
}

void exibirMapa(const Territorio* mapa, int num_territorios) {
    int i;
    
    printf("===================================\n");
    printf("        MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("===================================\n\n");

    for (i = 0; i < num_territorios; i++) {
        printf("%d. %s (Exercito %s, Tropas %d)\n", 
               i + 1, 
               (mapa + i)->nome, 
               (mapa + i)->cor, 
               (mapa + i)->tropas);
    }
    printf("\n");
}

void atacar(Territorio* atacante, Territorio* defensor) {
    // Simulacao de rolagem de dado (1 a 6)
    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O Atacante %s rolou um dado e tirou: %d\n", atacante->nome, dado_ataque);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dado_defesa);
    
    if (dado_ataque > dado_defesa) {
        printf("VITORIA DO ATAQUE! ");
        
        if (strcmp(atacante->cor, defensor->cor) != 0) {
            if (defensor->tropas <= 1) {
                printf("O territorio %s foi CONQUISTADO!\n", defensor->nome);
                strcpy(defensor->cor, atacante->cor);
                defensor->tropas = atacante->tropas / 2;
                atacante->tropas -= defensor->tropas;
            } else {
                defensor->tropas--;
                printf("O defensor perdeu 1 tropa.\n");
            }
        }
    } else {
        printf("DERROTA DO ATAQUE! O atacante perdeu 1 tropa.\n");
        atacante->tropas--;
    }
    
    if (atacante->tropas < 0) atacante->tropas = 0;
    if (defensor->tropas < 0) defensor->tropas = 0;
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("\nMemoria liberada com sucesso.\n");
}

void pausar() {
    printf("\nPressione ENTER para continuar para o proximo turno...");
    while (getchar() != '\n');
    getchar(); 
}

int main() {
    Territorio* mapa = NULL;
    int num_territorios = 0;
    int idx_atacante, idx_defensor;
    
    srand(time(NULL));

    mapa = cadastrarTerritorios(&num_territorios);

    do {
        exibirMapa(mapa, num_territorios);
        
        printf("--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", num_territorios);
        if (scanf("%d", &idx_atacante) != 1) idx_atacante = -1; // Trata entrada invalida
        
        if (idx_atacante == 0) {
            break;
        }
        
        if (idx_atacante < 1 || idx_atacante > num_territorios) {
            printf("Opcao invalida. Tente novamente.\n");
            pausar();
            continue;
        }

        printf("Escolha o territorio defensor (1 a %d): ", num_territorios);
        if (scanf("%d", &idx_defensor) != 1) idx_defensor = -1;
        
        while (getchar() != '\n');

        if (idx_defensor < 1 || idx_defensor > num_territorios || idx_atacante == idx_defensor) {
            printf("Opcao invalida ou atacante e defensor sao o mesmo. Tente novamente.\n");
            pausar();
            continue;
        }
        
        if (strcmp((mapa + idx_atacante - 1)->cor, (mapa + idx_defensor - 1)->cor) == 0) {
            printf("Voce nao pode atacar um territorio da sua propria cor (%s).\n", (mapa + idx_atacante - 1)->cor);
            pausar();
            continue;
        }
        
        if ((mapa + idx_atacante - 1)->tropas <= 0) {
            printf("O territorio atacante (%s) nao tem tropas suficientes para atacar.\n", (mapa + idx_atacante - 1)->nome);
            pausar();
            continue;
        }

        atacar(mapa + idx_atacante - 1, mapa + idx_defensor - 1);
        
        pausar();

    } while (1);

    liberarMemoria(mapa);

    return 0;
}
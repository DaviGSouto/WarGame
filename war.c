#include <stdio.h>
#include <string.h>

// Struct Territorio para armazenar os dados de cada territorio.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Vetor de structs para armazenar 5 territorios.
    Territorio mapa[5];
    int i;
    int max_territorios = 5;

    printf("Vamos cadastrar os %d territorios iniciais do nosso mundo.\n\n", max_territorios);

    // --- Cadastro dos Territorios ---
    for (i = 0; i < max_territorios; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        // Entrada do Nome do Territorio
        printf("Nome do Territorio: ");
        // Usando scanf para ler o nome.
        if (scanf("%29s", mapa[i].nome) != 1) {
             // Para um Tratamento basico de erro futuro.
             // Para este exercicio, assumimos que a entrada está correta.
        }
        
        // Entrada da Cor do exercito
        printf("Cor do exercito: ");
        if (scanf("%9s", mapa[i].cor) != 1) {
            
        }

        // Entrada do Numero de Tropas
        printf("Numero de Tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1) {
            
        }
        
        while (getchar() != '\n');
        
        printf("\n");
    }

    // --- Exibicao dos Dados ---
    printf("===================================\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("===================================\n\n");

    for (i = 0; i < max_territorios; i++) {
        printf("TERRITORIO %d:\n", i + 1);
        printf("    - Nome: %s\n", mapa[i].nome);
        printf("    - Dominado por: Exercito %s\n", mapa[i].cor);
        printf("    - Tropas: %d\n\n", mapa[i].tropas);
    }

    printf("Pressione ENTER para fechar a aplicacao...");
    getchar();

    return 0;
}
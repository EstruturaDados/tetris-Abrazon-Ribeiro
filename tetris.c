#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5  // Tamanho fixo da fila de peças

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único
} Peca;

// Estrutura da fila circular de peças
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ---------- Funções de controle da fila ----------

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->quantidade = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->quantidade == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// Gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4]; // Escolhe um tipo aleatório
    nova.id = id;
    return nova;
}

// Insere uma peça no final da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n⚠️ A fila está cheia! Não é possível adicionar mais peças.\n");
        return;
    }
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->pecas[f->fim] = p;
    f->quantidade++;
}

// Remove a peça da frente da fila (dequeue)
void desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("\n⚠️ A fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }
    Peca p = f->pecas[f->inicio];
    printf("\n🎮 Peça jogada: [%c %d]\n", p.nome, p.id);
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("\n=== Fila de peças ===\n");
    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }

    int i, idx;
    for (i = 0; i < f->quantidade; i++) {
        idx = (f->inicio + i) % TAMANHO_FILA;
        printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
    }
    printf("\n======================\n");
}

// ---------- Função principal ----------

int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL));

    int opcao, contadorID = 0;

    // Inicializa a fila com 5 peças automáticas
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca p = gerarPeca(contadorID++);
        enfileirar(&fila, p);
    }

    do {
        exibirFila(&fila);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&fila);
                break;
            case 2:
                if (!filaCheia(&fila)) {
                    Peca nova = gerarPeca(contadorID++);
                    enfileirar(&fila, nova);
                    printf("✅ Nova peça adicionada: [%c %d]\n", nova.nome, nova.id);
                } else {
                    printf("⚠️ Não é possível adicionar: fila cheia!\n");
                }
                break;
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

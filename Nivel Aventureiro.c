#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5   // Fila de peças futuras
#define TAMANHO_PILHA 3  // Pilha de peças reservadas

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// Estrutura de fila circular
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// Estrutura de pilha linear
typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo;
} Pilha;

// ---------------- Funções de Fila ----------------

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

// Adiciona uma peça no fim da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n⚠️  Fila cheia! Não é possível adicionar mais peças.\n");
        return;
    }
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->pecas[f->fim] = p;
    f->quantidade++;
}

// Remove uma peça do início da fila (dequeue)
Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) {
        printf("\n⚠️  Fila vazia! Nenhuma peça para jogar.\n");
        return removida;
    }
    removida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;
    return removida;
}

// ---------------- Funções de Pilha ----------------

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == TAMANHO_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Adiciona uma peça no topo da pilha (push)
void empilhar(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("\n⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->topo++;
    p->pecas[p->topo] = x;
}

// Remove a peça do topo da pilha (pop)
Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) {
        printf("\n⚠️  Pilha vazia! Nenhuma peça reservada para usar.\n");
        return removida;
    }
    removida = p->pecas[p->topo];
    p->topo--;
    return removida;
}

// ---------------- Funções de Jogo ----------------

// Gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n================= ESTADO ATUAL =================\n");

    printf("Fila de peças: ");
    if (filaVazia(f)) {
        printf("[vazia]");
    } else {
        for (int i = 0; i < f->quantidade; i++) {
            int idx = (f->inicio + i) % TAMANHO_FILA;
            printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
        }
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
        }
    }

    printf("\n================================================\n");
}

// ---------------- Função Principal ----------------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorID = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila com peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorID++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Jogar peça (remove da fila)
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1) {
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca(contadorID++)); // Mantém a fila cheia
                }
                break;
            }
            case 2: {
                // Reservar peça (move da fila para pilha)
                if (pilhaCheia(&pilha)) {
                    printf("\n⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
                } else if (!filaVazia(&fila)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\n📦 Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca(contadorID++)); // Reposição automática
                } else {
                    printf("\n⚠️  Nenhuma peça disponível na fila para reservar.\n");
                }
                break;
            }
            case 3: {
                // Usar peça reservada (remove do topo da pilha)
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1) {
                    printf("\n🧱 Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                }
                break;
            }
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\n⚠️  Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

// Estrutura de uma peça
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único
} Peca;

// Estrutura de Fila Circular
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// Estrutura de Pilha Linear
typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo;
} Pilha;

// ----------- Funções utilitárias -----------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ----------- Controle da Fila -----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->quantidade = 0;
}

int filaCheia(Fila *f) {
    return f->quantidade == TAMANHO_FILA;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->pecas[f->fim] = p;
    f->quantidade++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;
    return removida;
}

// ----------- Controle da Pilha -----------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAMANHO_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void empilhar(Pilha *p, Peca x) {
    if (!pilhaCheia(p)) p->pecas[++p->topo] = x;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (!pilhaVazia(p)) removida = p->pecas[p->topo--];
    return removida;
}

// ----------- Exibição -----------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n================ ESTADO ATUAL ================\n");

    printf("Fila de peças: ");
    if (filaVazia(f)) printf("[vazia]");
    else {
        for (int i = 0; i < f->quantidade; i++) {
            int idx = (f->inicio + i) % TAMANHO_FILA;
            printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
        }
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) printf("[vazia]");
    else {
        for (int i = p->topo; i >= 0; i--)
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }

    printf("\n==============================================\n");
}

// ----------- Ações principais -----------

// Troca a peça da frente da fila com o topo da pilha
void trocarPecaTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\n⚠️  Não há peças suficientes para realizar a troca.\n");
        return;
    }

    int idxFrente = f->inicio;
    Peca temp = f->pecas[idxFrente];
    f->pecas[idxFrente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;

    printf("\n🔄 Troca realizada entre a peça da frente da fila e o topo da pilha.\n");
}

// Troca múltipla (3 primeiras da fila com 3 da pilha)
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) {
        printf("\n⚠️  Não há peças suficientes para realizar a troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->inicio + i) % TAMANHO_FILA;
        int idxPilha = p->topo - i;
        Peca temp = f->pecas[idxFila];
        f->pecas[idxFila] = p->pecas[idxPilha];
        p->pecas[idxPilha] = temp;
    }

    printf("\n⚙️  Troca múltipla realizada entre as 3 primeiras peças da fila e as 3 da pilha.\n");
}

// ----------- Função Principal -----------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorID = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorID++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                if (!filaCheia(&fila))
                    enfileirar(&fila, gerarPeca(contadorID++));
                break;
            }
            case 2: {
                if (pilhaCheia(&pilha))
                    printf("\n⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
                else if (!filaVazia(&fila)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\n📦 Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca(contadorID++));
                }
                break;
            }
            case 3: {
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("\n🧱 Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                break;
            }
            case 4:
                trocarPecaTopo(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\n⚠️  Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

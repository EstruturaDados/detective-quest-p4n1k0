#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.
typedef struct Sala {
    char nome[50];
    char pista[100]; // Campo para armazenar uma pista associada à sala
    int coletada; // Flag para indicar se a pista já foi coletada
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct PistaNode {
    char texto[100];
    struct PistaNode* esq;
    struct PistaNode* dir;
} PistaNode;


Sala* criarSala(const char* nome, const char* pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));

    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }

    strncpy(nova->nome, nome, sizeof(nova->nome) - 1);
    nova->nome[sizeof(nova->nome) - 1] = '\0';

    if (pista != NULL) {
        strncpy(nova->pista, pista, sizeof(nova->pista) - 1);
        nova->pista[sizeof(nova->pista) - 1] = '\0';
    } else {
        nova->pista[0] = '\0'; // Sem pista
    }

    nova->coletada = 0; // Pista ainda não coletada
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

PistaNode* inserirPista(PistaNode* raiz, const char* texto) {
    if (texto == NULL || texto[0] == '\0') {
        return raiz;
    }

    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));

        strcpy(novo->texto, texto);
        novo->esq = novo->dir = NULL;

        return novo;
    }

    int cmp = strcmp(texto, raiz->texto);

    if (cmp < 0) {
        raiz->esq = inserirPista(raiz->esq, texto);
    } else if (cmp > 0) {
        raiz->dir = inserirPista(raiz->dir, texto);
    }
    
    return raiz;
}


void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }

    exibirPistas(raiz->esq);
    printf("- %s\n", raiz->texto);
    exibirPistas(raiz->dir);
}


void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas){
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (!atual->coletada && strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
            atual->coletada = 1; // Marca a pista como coletada
        }

        printf("Opcoes:\n");

        if (atual->esquerda) {
            printf("e - Ir para a esquerda\n");
        }
        if (atual->direita) {
            printf("d - Ir para a direita\n");
        }

        printf("s - Sair da exploração\n");
        printf("Escolha uma opção: ");
        scanf(" %c", &opcao);

        switch (opcao) {
            case 'e':
                if (atual->esquerda) {
                    atual = atual->esquerda;
                } else {
                    printf("Não há sala à esquerda.\n");
                }
                break;

            case 'd':
                if (atual->direita) {
                    atual = atual->direita;
                } else {
                    printf("Não há sala à direita.\n");
                }
                break;

            case 's':
                return;

            default:
                printf("Opção inválida.\n");

        }
    }
}

void liberarSalas(Sala *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);    
}

void liberarPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarPistas(raiz->esq);
    liberarPistas(raiz->dir);
    free(raiz);
}

int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.    

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.
    Sala *hall = criarSala("Hall de Entrada", "");

    hall->esquerda = criarSala("Sala de Estar", "Pegadas no tapete");
    hall->direita = criarSala("Cozinha", "Faca com digitais");

    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro fora do lugar");
    hall->esquerda->direita = criarSala("Escritório", "Computador ligado");

    hall->direita->esquerda = criarSala("Sala de Jantar", "");
    hall->direita->direita = criarSala("Jardim", "Pegadas na terra");

    PistaNode* pistas = NULL;

    explorarSalasComPistas(hall, &pistas);

    printf("\n==== PISTAS COLETADAS (ORDEM ALFABÉTICA) ====\n");
    exibirPistas(pistas);

    liberarSalas(hall);
    liberarPistas(pistas);

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}

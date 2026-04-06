#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.
#define TAM 10

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

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* prox;
} HashNode;

HashNode* tabela[TAM];

typedef struct ListaPistas {
    char pista[100];
    struct ListaPistas* prox;
} ListaPistas;

typedef struct Suspeito {
    char nome[50];
    int qtdPistas;
    ListaPistas* pistas;
    struct Suspeito* prox;
} Suspeito;

Suspeito* listaSuspeitos = NULL;

int hash(const char* chave) {
    int soma = 0;

    while (*chave) {
        soma += *chave++;        
    }
    return soma % TAM;
}

void inicializarHash() {
    for (int i = 0; i < TAM; i++) {
        tabela[i] = NULL;
    }
}

void inserirNaHash(const char* pista, const char* suspeito) {
    int idx = hash(pista);

    HashNode* novo = malloc(sizeof(HashNode));

    if (novo == NULL) {
        exit(1);
    }

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->prox = tabela[idx];
    tabela[idx] = novo;
}

void liberarHash() {
    for (int i = 0; i < TAM; i++) {
        HashNode* atual = tabela[i];

        while (atual) {
            HashNode* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}

const char* encontrarSuspeito(const char* pista) {
    HashNode* atual = tabela[hash(pista)];

    while (atual) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->prox;
    }
    return "Desconhecido";
}

Suspeito* buscarOuCriarSuspeito(const char* nome) {
    Suspeito* atual = listaSuspeitos;

    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    Suspeito* novo = malloc(sizeof(Suspeito));

    if (novo == NULL) {
        exit(1);
    }

    strcpy(novo->nome, nome);
    novo->qtdPistas = 0;
    novo->pistas = NULL;

    novo->prox = listaSuspeitos;
    listaSuspeitos = novo;

    return novo;
}

void adicionarPistaSuspeito(const char* nome, const char* pista) {
    Suspeito* s = buscarOuCriarSuspeito(nome);

    ListaPistas* nova = malloc(sizeof(ListaPistas));
    if (nova == NULL) {
        exit(1);
    }

    strcpy(nova->pista, pista);
    nova->prox = s->pistas;
    s->pistas = nova;

    s->qtdPistas++;
}

void listarSuspeitos() {
    printf("\n=== SUSPEITOS E PISTAS ===\n");
    Suspeito* s = listaSuspeitos;

    while (s) {
        printf("\n%s (%d pistas):\n", s->nome, s->qtdPistas);

        ListaPistas* p = s->pistas;

        while (p) {
            printf("- %s\n", p->pista);
            p = p->prox;
        }
        s = s->prox;
    }
}

void suspeitoMaisProvavel() {
    Suspeito* s = listaSuspeitos;
    Suspeito* melhor = NULL;

    while (s) {
        if (melhor == NULL || s->qtdPistas > melhor->qtdPistas) {
            melhor = s;
        }
        s = s->prox;
    }

    if (melhor) {
        printf("\nMais provavel: %s (%d pistas)\n", melhor->nome, melhor->qtdPistas);
    }
}

void liberarSuspeitos() {
    Suspeito* s = listaSuspeitos;

    while (s) {
        ListaPistas* p = s->pistas;

        while (p) {
            ListaPistas* tempP = p;
            p = p->prox;
            free(tempP);
        }

        Suspeito* tempS = s;
        s = s->prox;
        free(tempS);
    }
}

PistaNode* inserirPista(PistaNode* r, const char* txt) {
    if (txt == NULL || !*txt) {
        return r;
    }

    if (r == NULL) {
        PistaNode* n = malloc(sizeof(PistaNode));
        strcpy(n->texto, txt);
        n->esq = n->dir = NULL;
        return n;
    }

    int cmp = strcmp(txt, r->texto);

    if (cmp < 0) {
        r->esq = inserirPista(r->esq, txt);
    } else if (cmp > 0) {
        r->dir = inserirPista(r->dir, txt);
    }
    
    return r;
}

void exibirPistas(PistaNode* r) {
    if (r == NULL) {
        return;
    }

    exibirPistas(r->esq);
    printf("- %s\n", r->texto);
    exibirPistas(r->dir);
}

Sala* criarSala(const char* nome, const char* pista) {
    Sala* s = malloc(sizeof(Sala));
    strcpy(s->nome, nome); 
    strcpy(s->pista, pista ? pista : "");
    s->coletada = 0;
    s->esquerda = s->direita = NULL;
    return s;
}

void explorarSalas(Sala* atual, PistaNode** pistas) {
    char op;

    while (atual) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (!atual->coletada && atual->pista[0]) {
            printf("Pista: %s\n", atual->pista);

            *pistas = inserirPista(*pistas, atual->pista);

            const char* suspeito = encontrarSuspeito(atual->pista);
            printf("Suspeito associado: %s\n", suspeito);

            adicionarPistaSuspeito(suspeito, atual->pista);

            atual->coletada = 1; // Marca a pista como coletada
        }

        printf("[e] esquerda | [d] direita | [s] sair: ");
        scanf(" %c", &op);

        if (op == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (op == 'd' && atual->direita) {
            atual = atual->direita;
        } else if (op == 's') {
            break;
        } else {
            printf("Opção inválida\n");
        }
    }
}

void verificarFinal() {
    char nome[50];

    printf("\nQuem é o culpado? ");
    scanf(" %[^\n]", nome);

    Suspeito* s = listaSuspeitos;

    while (s) {
        if (strcmp(s->nome, nome) == 0) {
            if (s->qtdPistas >= 2) {
                printf("✔ Acusação correta! (%d pistas)\n", s->qtdPistas);
            } else {
                printf("❌ Evidências insuficientes (%d)\n", s->qtdPistas);
            }
            return;
        }
        s = s->prox;
    }
    printf("❌ Suspeito não encontrado\n");
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
    inicializarHash();

    inserirNaHash("Pegadas no tapete", "Ana");
    inserirNaHash("Faca com digitais", "Carlos");
    inserirNaHash("Livro fora do lugar", "Bruno");
    inserirNaHash("Computador ligado", "Carlos");
    inserirNaHash("Pegadas na terra", "Ana");

    Sala *hall = criarSala("Hall de Entrada", "");
    hall->esquerda = criarSala("Sala de Estar", "Pegadas no tapete");
    hall->direita = criarSala("Cozinha", "Faca com digitais");

    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro fora do lugar");
    hall->esquerda->direita = criarSala("Escritório", "Computador ligado");
    hall->direita->direita = criarSala("Jardim", "Pegadas na terra");

    PistaNode* pistas = NULL;

    explorarSalas(hall, &pistas);

    printf("\n==== PISTAS COLETADAS ====\n");
    exibirPistas(pistas);

    listarSuspeitos();
    suspeitoMaisProvavel();
    verificarFinal();
    liberarHash();
    liberarSuspeitos();

    return 0;
}

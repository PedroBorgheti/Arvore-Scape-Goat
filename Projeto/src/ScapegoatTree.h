#ifndef SCAPEGOAT_TREE_H
#define SCAPEGOAT_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 1. Estrutura de um Nó individual da árvore
typedef struct Noh {
    int chave;               // Valor armazenado
    struct Noh* esquerda;    // Ponteiro para subárvore esquerda
    struct Noh* direita;     // Ponteiro para subárvore direita
} Noh;

// 2. Estrutura Descritora (Cabeçalho) da Scapegoat Tree
typedef struct {
    Noh* raiz;               // Ponteiro para a raiz principal
    int n;                   // Número atual de elementos (nós)
    int max_n;               // Maior valor de 'n' desde a última reconstrução total
    double alpha;            // Fator de balanceamento (ex: 0.70)
} ScapegoatTree;

// Protótipos das funções públicas da biblioteca
ScapegoatTree* criarArvore(double alpha);
void destruirArvore(ScapegoatTree* arvore);
Noh* buscar(ScapegoatTree* arvore, int chave);
void inserir(ScapegoatTree* arvore, int chave);
void remover(ScapegoatTree* arvore, int chave);
Noh* reconstruirSubarvore(Noh* raiz);
int tamanhoSubarvore(Noh* raiz);

#endif

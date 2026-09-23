#include "ScapegoatTree.h"
#include "math.h"

// Cria e inicializa o cabeçalho da árvore
ScapegoatTree* criarArvore(double alpha) {
    ScapegoatTree* arvore = (ScapegoatTree*) malloc(sizeof(ScapegoatTree));
    if (arvore != NULL) {
        arvore->raiz = NULL;
        arvore->n = 0;
        arvore->max_n = 0;
        arvore->alpha = alpha;
    }
    return arvore;
}

// Retorna a quantidade de nós em uma subárvore (recursivo)
int tamanhoSubarvore(Noh* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + tamanhoSubarvore(raiz->esquerda) + tamanhoSubarvore(raiz->direita);
}

// Busca tradicional de ABB: O(log n) no pior caso
Noh* buscar(ScapegoatTree* arvore, int chave) {
    Noh* atual = arvore->raiz;
    
    while (atual != NULL) {
        if (chave == atual->chave) {
            return atual; // Achou!
        } else if (chave < atual->chave) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }
    
    return NULL; // Não encontrado
}

// 1. Percorre a subárvore em ordem e guarda os ponteiros dos nós no vetor
static void emOrdemParaVetor(Noh* raiz, Noh** vetor, int* indice) {
    if (raiz == NULL) {
        return;
    }

    emOrdemParaVetor(raiz->esquerda, vetor, indice);
    
    vetor[*indice] = raiz;
    (*indice)++;
    
    emOrdemParaVetor(raiz->direita, vetor, indice);
}

// 2. Constrói uma subárvore perfeitamente balanceada a partir do vetor ordenado
static Noh* construirBalanceada(Noh** vetor, int inicio, int fim) {
    if (inicio > fim) {
        return NULL;
    }

    // O elemento do meio vira a raiz da subárvore
    int meio = inicio + (fim - inicio) / 2;
    Noh* raiz = vetor[meio];

    // Reconstrói recursivamente as metades esquerda e direita
    raiz->esquerda = construirBalanceada(vetor, inicio, meio - 1);
    raiz->direita = construirBalanceada(vetor, meio + 1, fim);

    return raiz;
}

// 3. Função principal que orquestra a reconstrução de qualquer subárvore
Noh* reconstruirSubarvore(Noh* raiz) {
    int tamanho = tamanhoSubarvore(raiz);
    
    // Aloca vetor temporário de ponteiros para os nós
    Noh** vetor = (Noh**) malloc(sizeof(Noh*) * tamanho);
    
    int indice = 0;
    emOrdemParaVetor(raiz, vetor, &indice);

    // Reconstrução perfeita
    Noh* novaRaiz = construirBalanceada(vetor, 0, tamanho - 1);

    free(vetor);
    return novaRaiz;
}

// Calcula a altura máxima permitida: floor(log_{1/alpha}(n))
static int profundidadeMaxima(int n, double alpha) {
    return (int) floor(log(n) / log(1.0 / alpha));
}

// 1. Inserção padrão de ABB que calcula a profundidade onde o nó caiu
static Noh* inserirRecursivo(Noh* raiz, int chave, int profundidadeAtual, int* profundidadeInserido) {
    if (raiz == NULL) {
        Noh* novo = (Noh*) malloc(sizeof(Noh));
        novo->chave = chave;
        novo->esquerda = NULL;
        novo->direita = NULL;
        *profundidadeInserido = profundidadeAtual;
        return novo;
    }

    if (chave < raiz->chave) {
        raiz->esquerda = inserirRecursivo(raiz->esquerda, chave, profundidadeAtual + 1, profundidadeInserido);
    } else if (chave > raiz->chave) {
        raiz->direita = inserirRecursivo(raiz->direita, chave, profundidadeAtual + 1, profundidadeInserido);
    } else {
        // Chave duplicada: não insere e marca -1
        *profundidadeInserido = -1;
    }

    return raiz;
}

// 2. Sobe o caminho da inserção procurando o primeiro nó que violou o alfa-balanceamento por peso
static Noh* verificarEBalancear(Noh* raiz, int chave, double alpha, int* reconstruiu) {
    if (raiz == NULL || *reconstruiu) {
        return raiz;
    }

    // Desce na árvore seguindo o caminho até o nó inserido
    if (chave < raiz->chave) {
        raiz->esquerda = verificarEBalancear(raiz->esquerda, chave, alpha, reconstruiu);
    } else if (chave > raiz->chave) {
        raiz->direita = verificarEBalancear(raiz->direita, chave, alpha, reconstruiu);
    }

    // Na volta da recursão (de baixo para cima), se ainda não reconstruímos:
    if (!(*reconstruiu)) {
        int tamAtual = tamanhoSubarvore(raiz);
        int tamEsq = tamanhoSubarvore(raiz->esquerda);
        int tamDir = tamanhoSubarvore(raiz->direita);

        // Regra de peso: se algum dos filhos tiver mais que alpha * tamanho_total
        if (tamEsq > alpha * tamAtual || tamDir > alpha * tamAtual) {
            Noh* novaSubarvore = reconstruirSubarvore(raiz);
            *reconstruiu = 1; // Bode expiatório encontrado e reestruturado com sucesso
            return novaSubarvore;
        }
    }

    return raiz;
}

// 3. Função pública de inserção
void inserir(ScapegoatTree* arvore, int chave) {
    if (arvore == NULL) return;

    int profundidadeInserido = 0;
    arvore->raiz = inserirRecursivo(arvore->raiz, chave, 0, &profundidadeInserido);

    // Se o elemento foi inserido (não era duplicado)
    if (profundidadeInserido != -1) {
        arvore->n++;
        if (arvore->n > arvore->max_n) {
            arvore->max_n = arvore->n; // Atualiza o pico máximo de elementos
        }

        // Se a profundidade ultrapassou o limite máximo logarítmico, dispara a busca
        int hMax = profundidadeMaxima(arvore->n, arvore->alpha);
        if (profundidadeInserido > hMax) {
            int reconstruiu = 0;
            arvore->raiz = verificarEBalancear(arvore->raiz, chave, arvore->alpha, &reconstruiu);
        }
    }
}

// Encontra o nó com o menor valor em uma subárvore
static Noh* obterMinimo(Noh* raiz) {
    Noh* atual = raiz;
    while (atual && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

// Remoção padrão de ABB
static Noh* removerRecursivo(Noh* raiz, int chave, int* removido) {
    if (raiz == NULL) return NULL;

    if (chave < raiz->chave) {
        raiz->esquerda = removerRecursivo(raiz->esquerda, chave, removido);
    } else if (chave > raiz->chave) {
        raiz->direita = removerRecursivo(raiz->direita, chave, removido);
    } else {
        *removido = 1;

        // Caso 1 e 2: Nó com apenas um filho ou folha
        if (raiz->esquerda == NULL) {
            Noh* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            Noh* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        // Caso 3: Nó com dois filhos
        Noh* temp = obterMinimo(raiz->direita);
        raiz->chave = temp->chave;
        raiz->direita = removerRecursivo(raiz->direita, temp->chave, removido);
    }
    return raiz;
}

// Função pública de remoção
void remover(ScapegoatTree* arvore, int chave) {
    if (arvore == NULL || arvore->raiz == NULL) return;

    int removido = 0;
    arvore->raiz = removerRecursivo(arvore->raiz, chave, &removido);

    if (removido) {
        arvore->n--;

        // Se n caiu para um valor <= alpha * max_n, reconstrói a árvore inteira
        if (arvore->n <= arvore->alpha * arvore->max_n) {
            arvore->raiz = reconstruirSubarvore(arvore->raiz);
            arvore->max_n = arvore->n;
        }
    }
}

// Libera toda a memória alocada pelos nós
static void destruirNohs(Noh* raiz) {
    if (raiz == NULL) return;
    destruirNohs(raiz->esquerda);
    destruirNohs(raiz->direita);
    free(raiz);
}

// Função pública para destruir a árvore
void destruirArvore(ScapegoatTree* arvore) {
    if (arvore != NULL) {
        destruirNohs(arvore->raiz);
        free(arvore);
    }
}

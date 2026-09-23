#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ScapegoatTree.h"

#define N 10000
#define OPERACOES 30000

static uint32_t estado = 0x12345678u;
static volatile unsigned long long checksum;

static uint32_t aleatorio(void) {
    estado ^= estado << 13;
    estado ^= estado >> 17;
    estado ^= estado << 5;
    return estado;
}

static double agora(void) {
    struct timespec tempo;
    clock_gettime(CLOCK_MONOTONIC, &tempo);
    return tempo.tv_sec + tempo.tv_nsec / 1e9;
}

static void medir(const char* nome, int operacoes, void (*carga)(ScapegoatTree*)) {
    ScapegoatTree* arvore = criarArvore(0.70);
    double inicio = agora();
    carga(arvore);
    double total = agora() - inicio;
        printf("%s,%d,%d,%.3f,%.1f,%llu\n", nome, N, arvore->n,
            total * 1000.0, total * 1e9 / operacoes, checksum);
    destruirArvore(arvore);
}

static void insercao_sequencial(ScapegoatTree* arvore) {
    for (int chave = 0; chave < N; chave++) inserir(arvore, chave);
}

static void insercao_aleatoria(ScapegoatTree* arvore) {
    for (int i = 0; i < N; i++) inserir(arvore, (int)(aleatorio() % N));
}

static void carga_mista(ScapegoatTree* arvore) {
    for (int chave = 0; chave < N / 10; chave++) inserir(arvore, chave);
    for (int i = 0; i < OPERACOES; i++) {
        unsigned int tipo = aleatorio() % 10;
        int chave = (int)(aleatorio() % N);
        if (tipo < 5) inserir(arvore, chave);
        else if (tipo < 9) checksum += buscar(arvore, chave) != NULL;
        else remover(arvore, chave);
    }
}

static void leitura_intensiva(ScapegoatTree* arvore) {
    for (int chave = 0; chave < N / 10; chave++) inserir(arvore, chave);
    for (int i = 0; i < OPERACOES; i++) {
        unsigned int tipo = aleatorio() % 20;
        int chave = (int)(aleatorio() % N);
        if (tipo < 18) checksum += buscar(arvore, chave) != NULL;
        else if (tipo == 18) inserir(arvore, chave);
        else remover(arvore, chave);
    }
}

static void escrita_intensiva(ScapegoatTree* arvore) {
    for (int chave = 0; chave < N / 10; chave++) inserir(arvore, chave);
    for (int i = 0; i < OPERACOES; i++) {
        unsigned int tipo = aleatorio() % 10;
        int chave = (int)(aleatorio() % N);
        if (tipo < 7) inserir(arvore, chave);
        else if (tipo == 7) checksum += buscar(arvore, chave) != NULL;
        else remover(arvore, chave);
    }
}

static void zipfian_hotset(ScapegoatTree* arvore) {
    for (int chave = 0; chave < N; chave++) inserir(arvore, chave);
    for (int i = 0; i < OPERACOES; i++) {
        uint32_t valor = aleatorio() % 100;
        int chave = (valor * valor * N) / 10000;
        checksum += buscar(arvore, chave) != NULL;
    }
}

int main(void) {
    printf("workload,chaves,n_final,tempo_ms,ns_por_operacao,checksum\n");
    medir("insercao_sequencial", N, insercao_sequencial);
    medir("insercao_aleatoria", N, insercao_aleatoria);
    medir("carga_mista", OPERACOES, carga_mista);
    medir("leitura_intensiva", OPERACOES, leitura_intensiva);
    medir("escrita_intensiva", OPERACOES, escrita_intensiva);
    medir("zipfian_hotset", OPERACOES, zipfian_hotset);
    return 0;
}

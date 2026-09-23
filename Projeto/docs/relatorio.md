# Projeto Prático 1 - Scapegoat Tree

**Integrantes:** Pedro Borgheti, Maísa V. Kuhne e Vinicius Nunes  
**Disciplina:** Árvores e Grafos  
**Professor:** Evandro Pizzini  
**Data:** 22 de setembro de 2026

## Objetivo

Medir o custo da Scapegoat Tree em um banco chave-valor em memoria, usando `put` (insercao), `get` (busca) e `delete` (remocao).

## Metodo

O benchmark foi escrito em C e usa `clock_gettime(CLOCK_MONOTONIC)` para medir apenas as operacoes. Foram usados `N = 10.000` chaves, `30.000` operacoes nas cargas mistas e `alpha = 0,70`. O gerador xorshift e a semente `0x12345678` tornam as chaves reproduziveis. O checksum evita que buscas sejam eliminadas pelo compilador.

| Carga | Operacoes | Distribuicao |
|---|---:|---|
| Insercao sequencial | 10.000 puts | chaves 0 a 9.999 |
| Insercao aleatoria | 10.000 puts | chaves pseudoaleatorias |
| Carga mista | 30.000 | 50% put, 40% get, 10% delete |
| Leitura intensiva | 30.000 | 90% get, 5% put, 5% delete; preload de 10% |
| Escrita intensiva | 30.000 | 70% put, 10% get, 20% delete |
| Zipfian hotset | 30.000 gets | maior concentracao nas chaves iniciais |

## Resultados

Os valores abaixo são uma amostra da execução local e também estão registrados em [`../results/resultados.csv`](../results/resultados.csv). Cada linha vem diretamente da execução do benchmark; os tempos podem variar conforme o ambiente.

| Carga | Tempo (ms) | ns/op | n final |
|---|---:|---:|---:|
| Insercao sequencial | 3,923 | 392,3 | 10.000 |
| Insercao aleatoria | 0,790 | 79,0 | 6.340 |
| Carga mista | 4,348 | 144,9 | 7.137 |
| Leitura intensiva | 3,485 | 116,2 | 1.992 |
| Escrita intensiva | 7,941 | 264,7 | 7.307 |
| Zipfian hotset | 10,041 | 334,7 | 10.000 |

*Amostra executada localmente em macOS; os tempos do container Linux podem variar.*

## Como reproduzir

### Container Linux

Na pasta `Projeto`:

```sh
docker build -t scapegoat-benchmark .
docker run --rm scapegoat-benchmark > results/resultados.csv
```

O comando solicitado usa a imagem `gcc:13`, compila com `-O2` e executa em Linux dentro do container. Para visualizar a tabela no terminal, use `docker run --rm scapegoat-benchmark`.

### Sem container

```sh
sh scripts/run.sh
```

## Origem dos dados

Os dados da tabela são produzidos pelo próprio programa [`../src/main.c`](../src/main.c), não por uma fonte externa. As regras das cargas foram retiradas do enunciado do Projeto Prático 1 - Árvores Balanceadas (UTFPR, período 2026.2). A implementação da Scapegoat Tree está em [`../src/ScapegoatTree.c`](../src/ScapegoatTree.c) e [`../src/ScapegoatTree.h`](../src/ScapegoatTree.h). `../results/resultados.csv` é o registro bruto da execução.

## Conclusao

A insercao sequencial testa o caso de maior estresse estrutural. As cargas mistas mostram o comportamento geral do banco, enquanto leitura e escrita intensivas isolam perfis de uso diferentes. O resultado deve ser comparado apenas entre execucoes feitas no mesmo ambiente, pois CPU, compilador e carga do sistema alteram os tempos.
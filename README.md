# Projeto Scapegoat Tree

Implementação de uma **Scapegoat Tree** em C, com benchmark de inserções,
buscas e remoções em diferentes cargas de trabalho.

## Identificação

| Campo | Informação |
|---|---|
| Disciplina | Árvores e Grafos |
| Professor | Evandro Pizzini |
| Alunos | Pedro Borgheti, Maísa V. Kuhne e Vinicius Nunes |
| Data | 22 de setembro de 2026 |

## Objetivos

- Implementar uma árvore de busca binária autoajustável usando a estratégia Scapegoat Tree.
- Reconstruir subárvores quando o fator de balanceamento é violado.
- Comparar o comportamento da estrutura em operações de leitura e escrita.
- Registrar os resultados do benchmark em formato CSV.

## Estrutura do projeto

```text
.
├── Projeto/
│   ├── src/              # Implementação em C
│   ├── docs/             # Explicações e relatório
│   ├── results/          # Resultados do benchmark
│   ├── scripts/          # Scripts de execução
│   ├── Dockerfile        # Execução reproduzível em Linux
│   └── historico/        # Cópias e artefatos originais preservados
├── LICENSE
└── README.md
```

## Implementação

Cada nó armazena uma chave inteira e ponteiros para os filhos esquerdo e direito.
O cabeçalho da árvore mantém a raiz, a quantidade atual de nós, o maior tamanho
registrado e o fator `alpha`.

Na configuração do benchmark, `alpha = 0.70`. Após uma inserção que ultrapasse
a profundidade permitida, a implementação procura um ancestral desbalanceado e
reconstrói sua subárvore a partir de um percurso em ordem. Após remoções, a
árvore completa é reconstruída quando o tamanho cai abaixo do limite definido
pelo pico anterior.

## Cargas avaliadas

| Carga | Operações | Característica |
|---|---:|---|
| Inserção sequencial | 10.000 | Chaves de 0 a 9.999 |
| Inserção aleatória | 10.000 | Chaves pseudoaleatórias |
| Carga mista | 30.000 | 50% inserções, 40% buscas e 10% remoções |
| Leitura intensiva | 30.000 | 90% buscas, 5% inserções e 5% remoções |
| Escrita intensiva | 30.000 | 70% inserções, 10% buscas e 20% remoções |
| Zipfian hotset | 30.000 | Buscas concentradas nas chaves iniciais |

O gerador xorshift usa a semente `0x12345678`, permitindo repetir as mesmas
sequências de chaves. O checksum impede que as buscas sejam eliminadas pelo
compilador durante a otimização.

## Como executar

### macOS ou Linux

Na raiz do repositório:

```sh
cd Projeto
sh scripts/run.sh
```

O executável é criado em um arquivo temporário e o CSV atualizado é salvo em
`Projeto/results/resultados.csv`.

### Docker

```sh
cd Projeto
docker build -t scapegoat-benchmark .
docker run --rm scapegoat-benchmark
```

Para salvar a saída em um arquivo no host:

```sh
docker run --rm scapegoat-benchmark > results/resultados.csv
```

### Compilação manual

```sh
cd Projeto
gcc -O2 -std=c11 -Wall -Wextra -pedantic \
	src/main.c src/ScapegoatTree.c -lm -o /tmp/scapegoat-benchmark
/tmp/scapegoat-benchmark
```

## Resultados

Os resultados registrados estão em [`Projeto/results/resultados.csv`](Projeto/results/resultados.csv).
Os tempos dependem do processador, do compilador e da carga do sistema, por
isso a comparação deve ser feita entre execuções realizadas no mesmo ambiente.

O relatório detalhado está em [`Projeto/docs/relatorio.md`](Projeto/docs/relatorio.md),
e as notas sobre a estrutura estão em [`Projeto/docs/informacoes.md`](Projeto/docs/informacoes.md).

O relatório formatado para entrega está em
[`Projeto/docs/relatorio_projeto_scapegoat.docx`](Projeto/docs/relatorio_projeto_scapegoat.docx).
Ele inclui capa, identificação do grupo, sumário, explicação ampliada da
implementação e tabelas com linhas e destaque visual para as cargas e os
resultados.

Para regenerar o Word depois de atualizar os dados ou o texto:

```sh
python3 -m pip install python-docx
python3 Projeto/scripts/gerar_relatorio_word.py
```

## Referências do código

- [`Projeto/src/ScapegoatTree.h`](Projeto/src/ScapegoatTree.h): estruturas e protótipos públicos.
- [`Projeto/src/ScapegoatTree.c`](Projeto/src/ScapegoatTree.c): operações da árvore.
- [`Projeto/src/main.c`](Projeto/src/main.c): cargas do benchmark e medição.

Arquivo para guardar algumas características importantes para entendimento da ScapegoatTree

O nó dessa árvore é igual ao de uma árvore binária de busca tradicional
-> Um valor
-> Ponteiro para o filho da esquerda
-> Ponteiro para o filho da direita

A diferença entre uma Árvore Binária de Busca e uma ScapegoatTree se da no monitoramento da sua própria forma.
Se for inserido dados ordenados em uma ABB, ela pode ser degradada em uma lista encadeada, aumetnando o tempo de busca. A ScapegoatTree garante
uma busca logarítimica e tempos de atualização. Ela faz isso sem armazenar informações extras como cor ou pesos de números em cada nó.

Na Scapegoat Tree: quando a árvore fica alta demais após uma inserção, o algoritmo sobe procurando qual nó ancestral 
causou essa desordem para colocar toda a "culpa" (e o trabalho de reestruturação) nele.

As informações de balanceamento ficam em uma estrutura global que representa aárvore como um todo (o "cabeçalho")
-> raiz: ponteiro para o nó principal
-> n: quantidade atual de nós na árvore
-> max_n: maior número de nós que a árvore ja teve (usado para reconstruir após remoções)
-> alpha: um valor real entre 0.5 e 1.0 -> define o quão "tolerante" a árvore é com desequilíbrios -> quanto menor, mais exigente e baixinha
a árvore fica, quanto maior, mais tolerante e com ramos compridos fica, fazendo menos reconstruçoes.
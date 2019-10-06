#include <stdio.h>
#include <stdlib.h>

typedef struct Aresta {
    int u; // Origem da aresta
    int v; // Destino da aresta
    int c; // Custo da aresta
} Aresta;

typedef struct Grafo {
    int n; // Numero maximo de vertices
    int m; // Numero maximo de arestas
    int mAtual; // Número de arestas atualmente no grafo
    Aresta *arestas; // Ponteiro para o vetor de arestas que será inicializado
} Grafo;

Grafo *inicializar (int n, int m) {
    Grafo *grafo = malloc(sizeof(Grafo));
    grafo->n = n;
    grafo->m = m;

    grafo->mAtual = 0;
    grafo->arestas = malloc(m * sizeof(Aresta));
    return grafo;
}

void adicionarAresta (Grafo *g, int u, int v, int c) {
    Aresta *novaAresta = malloc(sizeof(Aresta));

    novaAresta->u = u;
    novaAresta->v = v;
    novaAresta->c = c;

    g->arestas[g->mAtual] = *novaAresta;
    g->mAtual++;
}

Grafo *criarGrafo() {
    int n, m;
    scanf("%d %d", &n, &m);

    Grafo *novoGrafo = inicializar(n, m);

    for (int i = 0; i < m; i++) {
        int u, v, c;
        scanf("%d %d %d", &u, &v, &c);
        adicionarAresta(novoGrafo, u-1, v-1, c);
    }
    return novoGrafo;

}

int comparaArestas (const void *a, const void *b) {
    Aresta *a1 = (Aresta*) a;
    Aresta *a2 = (Aresta*) b;
    return a1->c - a2->c;
}

int find(int unionFind[], int vertice) {
    int raiz = vertice;

    while (raiz != unionFind[raiz]) {
        raiz = unionFind[raiz];
    }

    while (vertice != raiz) {
        int prox = unionFind[vertice];
        unionFind[vertice] = raiz;
        vertice = prox;
    }
    return raiz;
}

void unir (int unionFind[], int grupo1, int grupo2) {
    int raiz1 = find(unionFind, grupo1);
    int raiz2 = find(unionFind, grupo2);

    if (raiz1 == raiz2) {
        return;
    }
    unionFind[raiz1] = raiz2;
}

void Kruskal (Grafo *grafo) {

    int indiceArestasOrdenadas = 0;
    int indiceArestasAGM = 0;

    Aresta agm[grafo->n];

    qsort(grafo->arestas, (size_t) grafo->mAtual, sizeof(grafo->arestas[0]), comparaArestas);   // Ordenando as arestas pelo custo
    int unionFind [grafo->n]; // Union-find, em que cada posição i indica que o vértice i está num certo grupo g
    for (int i = 0; i < grafo->n; i++) {
        unionFind[i] = i;   // O parente de cada vértice é inicalizado como ele mesmo
    }

    while (indiceArestasAGM < grafo->m - 1 && indiceArestasOrdenadas < grafo->m) {
        Aresta arestaAtual = grafo->arestas[indiceArestasOrdenadas++];

        int grupoOrigem = find(unionFind, arestaAtual.u);
        int grupoDestino = find(unionFind, arestaAtual.v);

        if (grupoDestino != grupoOrigem) {
            agm[indiceArestasAGM++] = arestaAtual;
            unir(unionFind, grupoOrigem, grupoDestino);
        }
    }

    printf("Árvore geradora de custo mínimo: ");
    for (int i = 0; i < indiceArestasAGM; i++) {
        printf("(%d, %d) ", agm[i].u + 1, agm[i].v + 1);
    }
    printf("\n");
    int custo = 0;
    for (int i = 0; i < indiceArestasAGM; i++) {
        custo += agm[i].c;
    }
    printf("Custo: %d", custo);

}

int main() {
    Grafo *grafo = criarGrafo();
    Kruskal(grafo);
    return 0;
}
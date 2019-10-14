/**
 * Arquivo: ep1.c
 * Alunos: Luis Filipe de Lima Sales, Raimundo Azevedo, Iago Moura, Dener Jamil
 *
 * Descrição: Este algoritmo faz parte do programa solicitado como parte da nota de trabalho da N1 na disciplina de
 * Grafos. A única entrada deste algoritmo consiste em, ao início da execução do programa, o caminho para um arquivo
 * de texto com o seguinte formato:
 * n m
 * u1 v1 c1
 * u2 v2 c2
 * ...
 * um vm cm
 *
 * Onde n é a quantidade de vértices do grafo, m a quantidade de arestas, ui e vi as extremidades da aresta i e ci o custo
 * da aresta i.
 *
 * Com a leitura deste grafo feita e armazenada em uma estrutura de dados, se executa o algoritmo de Kruskal em cima do
 * grafo lido, sendo então exibido na tela uma árvore geradora de custo mínimo do grafo, seguido do custo desta árvore.
 * É utilizado um union-find para se determinar a coloração de determinado vértice após a inserção de uma aresta.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 * A struct Aresta é utilizada para armazenar os dados de uma aresta, sendo utilizado na struct Grafo.
 */
typedef struct Aresta {
    int u; // Uma extremidade da aresta
    int v; // Outra extremidade da aresta
    int c; // Custo da aresta
} Aresta;

/**
 * A struct Grafo armazena o grafo lido como uma lista de arestas. Como será utilizado no algoritmo de Kruskal, que funciona
 * adicionando arestas de maneira gulosa em relação a seus pesos, esta acaba sendo uma boa estrutura para o propósito,
 * principalmente para a ordenação que é feita posteriormente dessas arestas em relação a seus pesos.
 */
typedef struct Grafo {
    int n; // Numero maximo de vertices.
    int m; // Numero maximo de arestas.
    int mAtual; // Número de arestas atualmente no grafo.
    Aresta *arestas; // Ponteiro para a matriz de arestas que será inicializada posteriormente
} Grafo;

/**
 * A função inicializar serve para alocar a memória necessária para o armazenamento de um grafo na memória e inicializando
 * variáveis conforme necessário.
 * @param n O número de vértices que o grafo terá.
 * @param m O número de arestas que o grafo terá.
 * @return Devolve um ponteiro para um grafo "configurado".
 */
Grafo *inicializar (int n, int m) {
    Grafo *grafo = malloc(sizeof(Grafo));
    grafo->n = n;
    grafo->m = m;

    grafo->mAtual = 0;
    // Diferenciamos m de mAtual pois mAtual serve para marcar o índice da próxima aresta a ser adicionada
    // no grafo, enquanto que m marca o número máximo de arestas que devem ser inseridas.

    grafo->arestas = malloc(m * sizeof(Aresta));    // Como são m arestas no máximo, alocar o espaço de m arestas.
    return grafo;
}

/**
 * A função adicionarAresta adiciona uma nova aresta no grafo g.
 * @param g Ponteiro para o grafo em que se deseja adicionar a aresta
 * @param u Uma das extremidades da aresta
 * @param v Outra extremidade da aresta
 * @param c Custo da aresta
 */
void adicionarAresta (Grafo *g, int u, int v, int c) {
    Aresta *novaAresta = malloc(sizeof(Aresta));

    novaAresta->u = u;
    novaAresta->v = v;
    novaAresta->c = c;

    g->arestas[g->mAtual] = *novaAresta;
    g->mAtual++;
}

/**
 * criarGrafo cria o grafo, lê as informações do arquivo indicado por path, adiciona arestas e retorna um ponteiro para
 * o grafo que foi criado
 * @param path  String que indica o caminho do arquivo a ser lido
 * @return  Retorna um ponteiro para um grafo.
 */
Grafo *criarGrafo(char path[]) {

    int n, m;   // n e m serão lidos do arquivo posteriormente.

    char buff[255]; // String utilizada para armazenar um valor lido do arquivo, para ser posteriormente convertido para inteiro.
    FILE *fp = fopen(path, "r");
    if (fp) {
        fscanf(fp, "%s", buff);     // Lê o primeiro número do arquivo
        n = atoi(buff);             // e o salva na variável n. Idem para m.
        fscanf(fp, "%s", buff);
        m = atoi(buff);

        Grafo *grafo = inicializar(n, m);  // Inicializa o grafo com n vértices e m arestas, no máximo.

        for (int i = 0; i < m; i++) {       // Executado até lermos m arestas.
            int u, v, c;                    // Lê-se os valores u, v e c de uma linha no arquivo.
            fscanf(fp, "%s", buff);
            u = atoi(buff);
            fscanf(fp, "%s", buff);
            v = atoi(buff);
            fscanf(fp, "%s", buff);
            c = atoi(buff);
            adicionarAresta(grafo, u-1, v-1, c);    // Com os valores lidos acima, é adicionada uma aresta no grafo.
                                                    // Usamos u-1 e v-1 , pois supomos que os números dos vértices
                                                    // começariam a partir do 1, conforme o exemplo do exercício.
        }
        fclose(fp);
        return grafo;
    }
    exit(-1);   // Se ocorrer um erro na abertura do arquivo, para a execução do programa.
}

/**
 * Função utilizada no qsort, empregado no algoritmo de Kruskal para se ordenar as arestas por seus pesos.
 * @param a Uma aresta a ser comparada.
 * @param b Outra aresta a se comparar.
 * @return Retorna um inteiro. Se maior que 0, a possui custo maior que b, se menor que 0, o contrário.
 */
int comparaArestas (const void *a, const void *b) {
    Aresta *a1 = (Aresta*) a;
    Aresta *a2 = (Aresta*) b;
    return a1->c - a2->c;
}

/**
 * Operação find utilizada no union-find para se descobrir a qual "categoria" um vértice pertence. Utiliza path
 * compression para melhor funcionamento.
 * @param unionFind Array que representa a estrutura de dados.
 * @param vertice Vértice cuja raiz se deseja descobrir.
 * @return Retorna a "raiz" do vértice na union-find.
 */
int find(int unionFind[], int vertice) {
    int raiz = vertice;         // Como ainda não sabemos qual a raiz, iniciamos ela com vertice.

    while (raiz != unionFind[raiz]) {   // Enquanto não chegamos na raiz...
        raiz = unionFind[raiz];         // vamos para o próximo grupo do union-find, indicado por raiz.
    }

    while (vertice != raiz) {           // Path compression
        int prox = unionFind[vertice];  // Iniciando no unionFind[vertice], e sabendo qual a raiz,
        unionFind[vertice] = raiz;      // dizemos que unionFind[vertice] é raiz. Repetimos isso até chegarmos na raiz.
        vertice = prox;                 // Vamos para o próximo vértice.
    }
    return raiz;
}

/**
 * Operação union utilizada no union-find para unir as "categorias" de um vértice.
 * @param unionFind Array que representa a estrutura de dados.
 * @param grupo1 Grupo que se deseja unir
 * @param grupo2 Grupo que se deseja unir
 */
void unir (int unionFind[], int grupo1, int grupo2) {
    int raiz1 = find(unionFind, grupo1);       // Encontramos a raiz do primeiro grupo.
    int raiz2 = find(unionFind, grupo2);       // O mesmo para o segundo.

    if (raiz1 == raiz2) {                      // Se as raízes forem as mesmas, não há nada a ser feito.
        return;
    }
    unionFind[raiz1] = raiz2;                   // Se não, a raiz de um grupo torna-se a mesma que a do outro grupo.
}

/**
 * Implementação do algoritmo de Kruskal. Primeiramente ordena as arestas do grafo admitido conforme seu peso, inicializa
 * o union-find, e executa o algoritmo de Kruskal em si. Por fim, imprime as arestas obtidas para a AGM e o peso da mesma.
 * @param grafo Ponteiro para o grafo
 */
void Kruskal (Grafo *grafo) {

    int indiceArestasOrdenadas = 0;      // Variável utilizada para percorrer o vetor de arestas, após terem sido ordenadas.
    int indiceArestasAGM = 0;            // Variável utilizada para manter o controle das arestas inseridas na AGM.

    Aresta agm[grafo->n];                // Criamos o vetor de arestas que estão contidas na AGM.

    qsort(grafo->arestas, (size_t) grafo->mAtual, sizeof(grafo->arestas[0]), comparaArestas);   // Ordenando as arestas pelo custo.

    int min = INT_MAX;
    for (int i = 0; i < grafo->m; i++) { // Verificamos se existe alguma aresta com custo negativo no grafo.
        if (grafo->arestas[i].c < min) {
            min = grafo->arestas[i].c;   // Se houver, armazenamos esse valor.
        }
    }

    if (min < 0) {                              // Se houver aresta com custo negativo...
        for (int i = 0; i < grafo->m; i++) {
            grafo->arestas[i].c += ((-1) * min) + 1;     // Incrementamos esse valor em todas as arestas
        }
    }

    int unionFind [grafo->n]; // Union-find, em que cada posição i indica que o vértice i está num certo grupo unionFind[i].

    for (int i = 0; i < grafo->n; i++) {
        unionFind[i] = i;   // O grupo de cada vértice é inicalizado como ele mesmo.
    }

    while (indiceArestasAGM < grafo->n - 1 && indiceArestasOrdenadas < grafo->m) {
        // Enquanto menos de (n-1) arestas tenham sido inseridas (vindo da propriedade que relaciona o número de vértices
        // e o número de arestas de uma árvore), e enquanto não tivermos percorrido todas as arestas do grafo.

        Aresta arestaAtual = grafo->arestas[indiceArestasOrdenadas++];  // "Pegamos" a próxima aresta, previamente
                                                                        // ordernada.

        int grupoV1 = find(unionFind, arestaAtual.u);   // Obtemos o grupo de uma extremidade da aresta...
        int grupoV2 = find(unionFind, arestaAtual.v);   // e o grupo da outra extremidade.

        if (grupoV2 != grupoV1) {                  // Se forem diferentes, não há ciclo ao inserirmos essa aresta na AGM.
            agm[indiceArestasAGM++] = arestaAtual; // Adicionamos essa aresta na AGM...
            unir(unionFind, grupoV1, grupoV2);     // e unimos os dois grupos.
        }
    }

    if (min < 0) {                                 // Se houver aresta de custo negativo
        for (int i = 0; i < grafo->m; i++) {
            agm[i].c -= ((-1) * min) + 1;         // Para inverter o que foi feito antes, somamos o valor mínimo a todas as arestas da AGM.
        }
    }

    printf("Árvore geradora de custo mínimo: ");
    for (int i = 0; i < indiceArestasAGM; i++) {
        printf("(%d, %d) ", agm[i].u + 1, agm[i].v + 1);    // Pois assumimos que a indexação dos vértices começa pelo 1.
    }
    printf("\n");
    int custo = 0;
    for (int i = 0; i < indiceArestasAGM; i++) {
        custo += agm[i].c;                      // Acumulamos o custo de todas as arestas na AGM.
    }
    printf("Custo: %d", custo);

}

int main() {
    char path[4096];
    scanf("%s", path);
    Grafo *grafo = criarGrafo(path);
    Kruskal(grafo);
    return 0;
}

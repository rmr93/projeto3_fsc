#include "calcularCaminho.h"

void arvoreMinima() {

    listaArestas *aux = (listaArestas *) malloc(sizeof(listaArestas));
    listaArestas *copia = (listaArestas *) malloc(sizeof(listaArestas));
    listaArestas *arvore_minima;
    lista_arestas_iniciar(aux);
    lista_arestas_iniciar(copia);
    int verticesUsados[100] = {0};
    int i, numeroVertices = 0;
    int vertOrigem = l_vertices->inicio->info->vertice0;

    no_aresta *na;
    no_vertices *nv = l_vertices->inicio;

    while (numeroVertices < l_vertices->tam) {
        arvore_minima = (listaArestas *) malloc(sizeof(listaArestas));
        lista_arestas_iniciar(arvore_minima);
        copiarLista(l_arestas, copia);
        i = 0;
        vertOrigem = nv->info->vertice0;
        while (verticesUsados[i] != 0) {
            verticesUsados[i] = 0;
            i++;
        }
        i = 0;
        verticesUsados[i] = vertOrigem;
        while (!lista_arestas_vazia(copia)) {
            // Inicia a busca da árvore pelo vértice de origem
            na = copia->inicio;
            while (na != NULL) {
                // Verifica se os vértices da aresta é igual ao vértice de origem
                if (na->info->vertice1 == vertOrigem || na->info->vertice2 == vertOrigem) {
                    // Verifica se não há repetição de vértices
                    if (vertices_usados(verticesUsados, na->info->vertice1, na->info->vertice2)) {
                        lista_arestas_remover(copia, na);
                        na = na->prox;
                        continue;
                    } else {
                        // Se a lista auxiliar estiver vazia, insere o nó
                        if (lista_arestas_vazia(aux)) {
                            lista_arestas_inserir(aux, na->info);
                        }
                        // Senão, compara com o início da lista, se for de custo menor o nó é inserido no lugar
                        else if (na->info->custo < aux->inicio->info->custo) {
                            lista_arestas_remover(aux, aux->inicio);
                            lista_arestas_inserir(aux, na->info);
                        } //fim else
                    } // fim else
                } // fim if

                na = na->prox;

            } // fim while

            if (aux->inicio != NULL) {
                // Atualiza o vértice de origem
                if (aux->inicio->info->vertice1 == vertOrigem)
                    vertOrigem = aux->inicio->info->vertice2;
                else if (aux->inicio->info->vertice2 == vertOrigem)
                    vertOrigem = aux->inicio->info->vertice1;
                verticesUsados[++i] = vertOrigem;
                lista_arestas_inserir(arvore_minima, aux->inicio->info);
                lista_arestas_remover(copia, aux->inicio);
                lista_arestas_remover(aux, aux->inicio);
            } else {
                while (copia->tam != 0)
                    lista_arestas_remover(copia, copia->inicio);
                break;
            }

        } // fim while
        numeroVertices++;
        no_vertices *resultado = (no_vertices *) malloc(sizeof(no_vertices));
        resultado->l_arestas = arvore_minima;
        resultado->info->vertice0 = nv->info->vertice0;
        lista_vertices_imprimir(resultado);
        nv = nv->prox;
    } // fim while

} // fim

void lerArquivo(char *caminho) {
    FILE *file;
    int numeroArestas;
    char c_atual, c_anterior = 0;

    // Criando lista de arestas e de vértices de origem
    l_arestas = (listaArestas *) malloc(sizeof(listaArestas));
    l_vertices = (listaVerticesOrigem *) malloc(sizeof(listaVerticesOrigem));
    lista_arestas_iniciar(l_arestas);
    lista_vertices_iniciar(l_vertices);

    if (!(file = fopen(caminho, "r"))) {
        printf("\nNão foi possível abrir o arquivo!!\n");
        exit(1);
    } else {

        // Leitura da primeira linha
        fscanf(file, "%d\n", &numeroArestas);

        // Leitura das arestas
        for ( ; numeroArestas != 0; numeroArestas--) {
            dadosArestas *a = (dadosArestas *) malloc(sizeof(dadosArestas));
            fscanf(file, "R%d,%d R%d,%d C%d\n", &a->aresta, &a->vertice1, &a->aresta, &a->vertice2, &a->custo);
            lista_arestas_inserir(l_arestas, a);
        }

        // Leitura dos vértices de origem de tráfego
        while ( (c_atual = fgetc(file)) != '0') {
            if (c_atual == 'V') {
                c_anterior = c_atual;
            } else if (c_anterior == 'V') {
                dadosVerticesOrigem *v = (dadosVerticesOrigem *) malloc(sizeof(dadosVerticesOrigem));
                v->vertice0 = atoi(&c_atual);
                c_anterior = 0;
                lista_vertices_inserir(l_vertices, v);
            }
        }

        fclose(file);
    }
}

int vertices_usados(int verticesUsados[], int v1, int v2) {
    int i = 0, k = 0;

    while (verticesUsados[i] != 0 && k != 2) {
        if (v1 == verticesUsados[i])
            k++;
        else if (v2 == verticesUsados[i])
            k++;
        i++;
    }

    if (k == 2)
        return 1;
    else
        return 0;
}

void copiarLista(listaArestas *origem, listaArestas *destino) {
    no_aresta *a = origem->inicio;

    while (a != NULL) {
        lista_arestas_inserir(destino, a->info);
        a = a->prox;
    }
}

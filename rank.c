/*******************************************
 * Calcula centralidade de intermediacao de
 * todos os vértices de um grafo direcionado
 * com peso nas arestas.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "centralidade.h"

int main()
{

  // cria grafo vazio
  grafo G = cria_grafo(1);

  // le e adiciona vertices e arestas
  int n, m;

  scanf("%d %d", &n, &m);

  for (int i = 1; i <= n; i++)
  {
    char rotulo[10];
    adiciona_vertice(i, rotulo, G);
  }

  for (int i = 1; i <= m; i++)
  {
    int u, v, w;
    char rotulo[10];
    scanf("%d %d %d", &u, &v, &w);
    adiciona_aresta(i, rotulo, w, u, v, G);
  }

  // calcula todos os caminhos minimos de G, retornando
  // em ant uma matriz de lista de antecessores
  lista **ant = floydWarshall(G, n);

  // constroi todos os caminhos minimos usando a matriz ant,
  // retornando em C uma matriz de lista de caminhos mínimos
  lista **cam = constroi_caminhos(ant, G, n);

  // Criação de um array para armazenar as informações de centralidade de cada vértice
  VerticeCentrality *centralityArray = malloc(n * sizeof(VerticeCentrality));

  // para cada par de vértices u e v do grafo
  int size;
  for (no p = primeiro_no(vertices(G)); p; p = proximo(p))
  {
    for (no q = primeiro_no(vertices(G)); q; q = proximo(q))
    {
      vertice u = conteudo(p);
      vertice v = conteudo(q);
      // calcula a quantidade de caminhos mínimos entre u e v que
      // passa por cada vértice do grafo
      int *sigma_w = sigma(u, v, cam, &size, n);

      // usa sigma_w para calcular a centralidade do vértice
      for (no r = primeiro_no(vertices(G)); r; r = proximo(r))
      {
        vertice w = conteudo(r);
        if (w != u && w != v && size != 0)
        {
          double centralidade = (double)sigma_w[w->id - 1] / size;
          centralityArray[w->id - 1].id = w->id;
          centralityArray[w->id - 1].centralidade += centralidade;
        }
      }
      // libera a memória alocada para sigma_w
      free(sigma_w);
    }
  }

  // Ordenar os vértices pelo valor da centralidade (em ordem decrescente)
  qsort(centralityArray, n, sizeof(VerticeCentrality), compareCentralidade);

  // Imprimir os vértices ordenados pela centralidade
  for (int i = 0; i < n; ++i)
  {
    printf("%d %.1f\n", centralityArray[i].id, centralityArray[i].centralidade);
  }

  // Libera a memória alocada para o array de centralidades
  free(centralityArray);

  // destroi grafo
  destroi_grafo(G);

  return 0;
}
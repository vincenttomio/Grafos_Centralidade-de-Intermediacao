/*******************************************
 * Implementação de biblioteca para grafos.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------------------
// getters:

int vertice_id(vertice v)
{
  return v->id;
};
char *vertice_rotulo(vertice v)
{
  return v->rotulo;
};
int custo(vertice v)
{
  return v->custo;
};
int estado(vertice v)
{
  return v->estado;
};
vertice pai(vertice v)
{
  return v->pai;
};
lista fronteira(vertice v)
{
  return v->fronteira;
};
int aresta_id(aresta e)
{
  return e->id;
};
char *aresta_rotulo(aresta e)
{
  return e->rotulo;
};
int peso(aresta e)
{
  return e->peso;
};
vertice vertice_u(aresta e)
{
  return e->u;
};
vertice vertice_v(aresta e)
{
  return e->v;
};
int grafo_id(grafo G)
{
  return G->id;
}
lista vertices(grafo G)
{
  return G->vertices;
};
lista arestas(grafo G)
{
  return G->arestas;
};

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo(int id)
{
  grafo G = (grafo)malloc(sizeof(t_grafo));
  if (!G)
    exit(_ERRO_MALLOC_);
  G->id = id;
  G->vertices = cria_lista();
  G->arestas = cria_lista();
  return G;
}

// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G)
{
  // desalocar os vertices
  lista lv = vertices(G);
  for (no n = primeiro_no(lv); n; n = proximo(n))
  {
    vertice v = conteudo(n);
    free(v);
  }

  // desalocar as arestas
  lista la = arestas(G);
  for (no n = primeiro_no(la); n; n = proximo(n))
  {
    aresta a = conteudo(n);
    free(a);
  }
  free(G);
}

// cria novo vertice com id <id> rotulo <rotulo> e adiciona ao grafo G
void adiciona_vertice(int id, char *rotulo, grafo G)
{
  vertice v = (vertice)malloc(sizeof(t_vertice));
  if (!v)
    exit(_ERRO_MALLOC_);
  v->id = id;
  strcpy(v->rotulo, rotulo);
  v->fronteira = cria_lista();
  empilha(v, vertices(G));
}

// remove vertice com id <id> do grafo G e o destroi
// [deve remover e destruir tambem as arestas incidentes]
void remove_vertice(int id, grafo G)
{
  // Busca o vértice com o id passado como parâmetro
  vertice v = busca_chave(id, vertices(G), (int_f_obj)vertice_id);

  if (!v)
    return;

  // Remove todas as arestas incidentes ao vértice e as destroi
  while (!vazio(fronteira(v)))
  {
    aresta e = desempilha(fronteira(v));
    if (vertice_u(e) != v)
    {
      remove_chave(aresta_id(e), fronteira(vertice_u(e)), (int_f_obj)aresta_id);
    }
    else
    {
      remove_chave(aresta_id(e), fronteira(vertice_v(e)), (int_f_obj)aresta_id);
    }
    remove_chave(aresta_id(e), arestas(G), (int_f_obj)aresta_id);
    free(e);
  }

  // Remove o vértice da lista de vértices do grafo e o destroi
  remove_chave(id, vertices(G), (int_f_obj)vertice_id);
  free(v);
}

// cria aresta com id <id>, rotulo <rotulo> e peso <peso>,
// incidente a vertices com ids <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, char *rotulo, int peso, int u_id, int v_id, grafo G)
{
  // cria vértices u e v correspondentes aos ids passados como parâmetros
  vertice u = busca_chave(u_id, vertices(G), (int_f_obj)vertice_id);
  vertice v = busca_chave(v_id, vertices(G), (int_f_obj)vertice_id);

  // cria a aresta com id passado como parâmetro e os vértices u e v
  aresta e = (aresta)malloc(sizeof(t_aresta));
  if (!e)
    exit(_ERRO_MALLOC_);
  e->id = id;
  strcpy(e->rotulo, rotulo);
  e->peso = peso;
  e->u = u;
  e->v = v;

  // adiciona a aresta na lista de arestas do grafo
  empilha(e, arestas(G));

  // adiciona a aresta nas listas de fronteiras dos vértices u e v
  empilha(e, fronteira(u));
  empilha(e, fronteira(v));
}

// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G)
{
  aresta e = busca_chave(id, arestas(G), (int_f_obj)aresta_id);

  if (!e)
    return;

  // remove a aresta da lista de fronteiras dos vértices u e v
  remove_chave(id, fronteira(vertice_u(e)), (int_f_obj)aresta_id);
  remove_chave(id, fronteira(vertice_v(e)), (int_f_obj)aresta_id);

  // remove a aresta da lista de arestas do grafo
  remove_chave(id, arestas(G), (int_f_obj)aresta_id);

  // desaloca memória da aresta
  free(e);
}

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// calcula e devolve o grau do vertice v
int grau(vertice v)
{
  int d_v = 0;
  for (no n = primeiro_no(fronteira(v)); n; n = proximo(n))
    ++d_v;
  return d_v;
}

// imprime o grafo G
void imprime_grafo(grafo G)
{
  printf("\tGRAFO(");
  printf("%d)\n", grafo_id(G));
  printf("\nEstrutura: ");
  imprime_lista(arestas(G), (void_f_obj)imprime_estrutura_aresta);
  printf("\nVertices: ");
  imprime_lista(vertices(G), (void_f_obj)imprime_vertice);
  printf("\nArestas: ");
  imprime_lista(arestas(G), (void_f_obj)imprime_aresta);
  printf("\n\n\n");
}

// imprime o grafo G ponderado
void imprime_grafo_ponderado(grafo G)
{
  printf("\nVertices: <id> - [grau]( <fronteira> )");
  printf("\nVertices: ");
  imprime_lista(vertices(G), (void_f_obj)imprime_vertice);
  printf("\nArestas: <id>:{u,v}<w>");
  printf("\nArestas: ");
  imprime_lista(arestas(G), (void_f_obj)imprime_aresta);
  printf("\n");
}

// imprime o vertice v
void imprime_vertice(vertice v)
{
  printf("(id:%d, rotulo:%s, grau:%d, fronteira:{ ", vertice_id(v), vertice_rotulo(v), grau(v));
  imprime_lista(fronteira(v), (void_f_obj)imprime_aresta);
  printf("})");
}

// imprime a aresta e
void imprime_aresta(aresta e)
{
  int u_id = vertice_id(vertice_u(e));
  int v_id = vertice_id(vertice_v(e));
  printf("(id:%d, rotulo:%s, peso:%d, {%d,%d})", aresta_id(e), aresta_rotulo(e), peso(e), u_id, v_id);
}

// imprime apenas a estrutura da aresta e
void imprime_estrutura_aresta(aresta e)
{
  int u_id = vertice_id(vertice_u(e));
  int v_id = vertice_id(vertice_v(e));
  printf("{%d,%d}", u_id, v_id);
}

lista **floydWarshall(grafo G, int n)
{
  // matriz de distâncias: aloca e inicializa com "infinito"
  int **dist = (int **)malloc(n * sizeof(int *));
  for (int i = 0; i < n; i++)
  {
    dist[i] = (int *)malloc(n * sizeof(int));
    for (int j = 0; j < n; j++)
    {
      if (i == j)
        dist[i][j] = 0;
      else
        dist[i][j] = INT_MAX;
    }
  }

  // matriz de antecessores: aloca e inicializa (cria lista vazia)
  lista **ant = (lista **)malloc(n * sizeof(lista *));
  for (int i = 0; i < n; i++)
  {
    ant[i] = (lista *)malloc(n * sizeof(lista));
    for (int j = 0; j < n; j++)
    {
      ant[i][j] = (lista)malloc(sizeof(t_lista));
      ant[i][j]->primeiro_no = NULL;
    }
  }

  // inicializa matrizes dist e ant com arestas do grafo
  lista arestas = G->arestas;
  no atual = arestas->primeiro_no;

  while (atual != NULL)
  {
    aresta a = (aresta)atual->conteudo;
    int u = a->u->id - 1;
    int v = a->v->id - 1;
    dist[u][v] = a->peso;
    no novo_no = (no)malloc(sizeof(t_no));
    novo_no->conteudo = a->u;
    novo_no->proximo = ant[u][v]->primeiro_no;
    ant[u][v]->primeiro_no = novo_no;
    atual = atual->proximo;
  }

  // algoritmo principal
  for (int k = 0; k < n; k++)
  {
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
            dist[i][k] + dist[k][j] < dist[i][j])
        {
          // Limpa a lista de antecessores para adicionar os novos
          no antecessor_atual = ant[i][j]->primeiro_no;
          while (antecessor_atual != NULL)
          {
            no antecessor_remover = antecessor_atual;
            antecessor_atual = antecessor_atual->proximo;
            free(antecessor_remover);
          }
          ant[i][j]->primeiro_no = NULL;

          // Adiciona os novos antecessores
          no antecessor_k = ant[k][j]->primeiro_no;
          while (antecessor_k != NULL)
          {
            no novo_no = (no)malloc(sizeof(t_no));
            novo_no->conteudo = antecessor_k->conteudo;
            novo_no->proximo = ant[i][j]->primeiro_no;
            ant[i][j]->primeiro_no = novo_no;
            antecessor_k = antecessor_k->proximo;
          }

          // Atualiza a distância mínima
          dist[i][j] = dist[i][k] + dist[k][j];
        }
        // Caso do peso igual
        else if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                 dist[i][k] + dist[k][j] == dist[i][j])
        {
          // Adiciona antecessores que ainda não estão presentes na lista
          no antecessor_k = ant[k][j]->primeiro_no;
          while (antecessor_k != NULL)
          {
            // Verifica se o antecessor já existe na lista
            int antecessor_presente = 0;
            no antecessor_atual = ant[i][j]->primeiro_no;
            while (antecessor_atual != NULL)
            {
              if (antecessor_atual->conteudo == antecessor_k->conteudo)
              {
                antecessor_presente = 1;
                break;
              }
              antecessor_atual = antecessor_atual->proximo;
            }

            // Adiciona antecessor à lista se ele ainda não estiver presente
            if (!antecessor_presente)
            {
              no novo_no = (no)malloc(sizeof(t_no));
              novo_no->conteudo = antecessor_k->conteudo;
              novo_no->proximo = ant[i][j]->primeiro_no;
              ant[i][j]->primeiro_no = novo_no;
            }

            antecessor_k = antecessor_k->proximo;
          }
        }
      }
    }
  }

  // Desaloca matriz dist
  for (int i = 0; i < n; i++)
    free(dist[i]);
  free(dist);

  // Retorna antecessores
  return ant;
}

// Função constroi_caminhos(ant,G,n) recebe como parâmetros
// a matriz de listas com os antecessores gerada por
// floydWarshall(G,n), o grafo G e o número de vértice n.
// Constroi todos os caminhos mínimos entre todos os pares
// de vértices em G.
// Devolve uma matriz C onde cada posição C[i][j]
// é uma lista de caminhos mínimos entre os vértices u e v,
// onde u é o vértice com id i+1 e v é o vértice com id j+1.
// Nessa matriz, um caminho mínimo é uma lista de vértices com
// extremos u e v. Isto é, C[i][j] é uma lista de listas de vértices.
lista **constroi_caminhos(lista **ant, grafo G, int n)
{

  // matriz de caminhos minimos: aloca
  lista **C = (lista **)malloc(sizeof(lista *) * n);
  for (int i = 0; i < n; ++i)
    C[i] = (lista *)malloc(sizeof(lista) * n);

  // Constroi uma lista com todos os caminhos minimos entre todos
  // os pares de vértices u e v e atribui a matriz C
  for (no p = primeiro_no(vertices(G)); p; p = proximo(p))
    for (no q = primeiro_no(vertices(G)); q; q = proximo(q))
    {
      vertice u = conteudo(p);
      vertice v = conteudo(q);
      C[vertice_id(u) - 1][vertice_id(v) - 1] = caminhos_minimos_REC(u, v, ant);
    }

  return C;
}

// Função caminhos_minimos_REC, chamada pela função constroi_caminhos
lista caminhos_minimos_REC(vertice u, vertice v, lista **ant)
{
  // a lista com todos os caminhos até v será preenchida
  // recursivamente
  lista todos_caminhos_v = cria_lista();

  // pega lista de antecessores do vértice v
  lista antecessores = ant[vertice_id(u) - 1][vertice_id(v) - 1];

  // para cada antecessor w
  for (no p = primeiro_no(antecessores); p; p = proximo(p))
  {
    vertice w = conteudo(p);
    lista todos_caminhos_w;

    // se w == u => caminho mínimo será (u,w,v)
    if (w == u)
    {
      lista caminho = cria_lista(); // cria caminho contendo (u)
      empilha(u, caminho);
      todos_caminhos_w = cria_lista(); // o unico caminho até w é (u,w)
      empilha(caminho, todos_caminhos_w);
    }
    // senão, encontra todos os caminhos até w recursivamente
    else
    {
      todos_caminhos_w = caminhos_minimos_REC(u, w, ant);
    }

    // para cada caminho mínimo de u até w, inclui o vértice v
    for (no q = primeiro_no(todos_caminhos_w); q; q = proximo(q))
    {
      lista caminho = conteudo(q);
      empilha(v, caminho);
      // salva na lista como um caminho mínimo de u até v
      empilha(caminho, todos_caminhos_v);
    }

    limpa_lista(todos_caminhos_w);
    free(todos_caminhos_w);
  }

  return todos_caminhos_v;
}

// Função sigma(u,v,C,&size,n) recebe como parâmetros
// dois vértices u e v, a matriz de listas de caminhos mínimos
// gerada por constroi_caminhos(ant,G,n), variável size e o
// número de vértices n.
// Calcula a quantidade de caminhos mínimos entre u e v que
// passam por cada vértice do grafo.
// Devolve um vetor sigma_w de tamanho n onde cada posicão
// sigma_w[i] contém a quantidade de caminhos mínimos entre u
// e v que passam pelo vértice com id i+1. A variável size
// é preenchida com a quantidade total de cmainhos mínimos
// entre u e v.
int *sigma(vertice u, vertice v, lista **caminhos_minimos, int *size, int n)
{
  *size = 0;

  // aloca vetor de contadores para os vértices
  int *sigma_w = (int *)malloc(sizeof(int) * n);
  for (int i = 0; i < n; ++i)
    sigma_w[i] = 0;

  int i = vertice_id(u) - 1;
  int j = vertice_id(v) - 1;

  // para cada caminho mínimo entre u e v,
  for (no p = primeiro_no(caminhos_minimos[i][j]); p; p = proximo(p))
  {
    lista caminho = conteudo(p);
    (*size)++;
    // percorre todos os vértices internos do caminho, i.e., excluindo u e v
    no q = primeiro_no(caminho);
    for (q = proximo(q); q; q = proximo(q))
      if (proximo(q))
      {
        // contabiliza uma ocorrência do vértice w
        vertice w = conteudo(q);
        sigma_w[vertice_id(w) - 1]++;
      }
  }

  return sigma_w;
}
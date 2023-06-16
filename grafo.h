/*******************************************
 * Implementação de biblioteca para grafos.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#ifndef _GRAFO_
#define _GRAFO_

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // INT_MAX

#define ABERTO 1
#define PROCESSADO 2
#define FECHADO 3

typedef struct t_vertice *vertice;
typedef struct t_aresta *aresta;
typedef struct t_grafo *grafo;

typedef struct t_vertice {
  int id;
  char rotulo[10];
  int custo;        // para algoritmos de busca
  int estado;
  vertice pai;
  lista fronteira;
} t_vertice;

typedef struct t_aresta {
  int id;
  char rotulo[10];
  int peso;
  vertice u, v;
} t_aresta;

typedef struct t_grafo {
  int id;
  lista vertices;
  lista arestas;
} t_grafo ;

//---------------------------------------------------------
// getters:

int vertice_id(vertice v);
char* vertice_rotulo(vertice v);
int custo(vertice v);
int estado(vertice v);
vertice pai(vertice v);
lista fronteira(vertice v);
int aresta_id(aresta e);
char *aresta_rotulo(aresta e);
int peso(aresta e);
vertice vertice_u(aresta e);
vertice vertice_v(aresta e);
int grafo_id(grafo G);
lista vertices(grafo G);
lista arestas(grafo G);

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo(int id);

// destroi grafo G (desaloca toda a memoria)
void destroi_grafo(grafo G);

// cria novo vertice com id <id> rotulo <rotulo> e adiciona ao grafo G
void adiciona_vertice(int id, char *rotulo, grafo G);

// remove vertice com id <id> do grafo G e o destroi
// [deve remover e destruir tambem as arestas incidentes]
void remove_vertice(int id, grafo G);

// cria aresta com id <id>, rotulo <rotulo> e peso <peso>,
// incidente a vertices com ids <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, char *rotulo, int peso, int u_id, int v_id, grafo G);

// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G);

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// imprime o grafo G
void imprime_grafo(grafo G);

// imprime o vertice v
void imprime_vertice(vertice v);

// imprime a aresta e
void imprime_aresta(aresta e);

// imprime apenas a estrutura da aresta e
void imprime_estrutura_aresta(aresta e);

// Função floydWarshall(G,n) recebe como parâmetros
// um grafo G e o número de vértices n.
// Aplica o algoritmo de Floyd-Warshall para encontrar
// todos os caminhos mínimos entre todos os pares
// de vértices de G.
// Devolve uma matriz ant onde cada posição ant[i][j]
// é uma lista de vértices contendo os antecessores do
// vértice v nos caminhos mínimos entre u e v, onde
// u é o vértice com id i+1 e v é o vértice com id j+1
lista **floydWarshall(grafo G, int n);

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
lista **constroi_caminhos(lista **ant, grafo G, int n);

// Função caminhos_minimos_REC, chamada pela função constroi_caminhos
lista caminhos_minimos_REC(vertice u, vertice v, lista **ant);

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
int *sigma(vertice u, vertice v, lista **caminhos_minimos, int *size, int n);

#endif

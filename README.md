# Trabalho 2 - Centralidade de Intermediação

Este projeto foi realizado em dupla pelos alunos:
```
 Vincent Tomio - GRR20206365
 Yan Oliveira da Costa - GRR20204095
```

Como parte da disciplina de CI1054 - Algoritmos em Grafos e Otimização, o objetivo do trabalho foi desenvolver um programa capaz de receber um grafo ponderado, e utlizando o algoritmo de Floyd-Warshall, como elemento principal, calcular a centralidade de intermediação de todos os vértices do grafo, e por fim devolver ao usuário um rank contendo os vertices do mais central ao menos central.

O código é escrito em C e utiliza as bibliotecas grafo.h, lista.h e centralidade.h para manipular as estruturas de grafo, e realizar o processamento do grafo e devolução do rank.

Para rodar este programa, você deve ter um arquivo .in contendo a descrição do grafo.

O comando para executar o programa é:
```
./rank < grafo.in
```


## Floyd-Warshall

O algoritmo Floyd-Warshall é um algoritmo de programação dinâmica usado para encontrar o caminho mais curto entre todos os pares de vértices em um grafo ponderado. Ele constrói uma matriz de distâncias mínimas, atualizando os valores iterativamente por meio de uma abordagem de "relaxamento" dos caminhos. O algoritmo é eficiente para grafos pequenos a médios, com complexidade de tempo de O(n³), onde n é o número de vértices do grafo.

### Implementação

```
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
```

O código fornecido implementa o algoritmo de Floyd-Warshall para encontrar o caminho mais curto entre todos os pares de vértices em um grafo ponderado. Explicação passo a passo do que cada parte do código faz:

1. Primeiro, é definida uma função chamada `floydWarshall` que recebe um grafo `G` e o número de vértices `n`. A função retorna uma matriz de listas de antecessores.

2. Em seguida, é alocada uma matriz `dist` de tamanho `n` por `n` que será usada para armazenar as distâncias mínimas entre os vértices. A matriz é inicializada com valores "infinitos" (`INT_MAX`), exceto pela diagonal principal que é definida como zero.

3. Uma matriz de listas de antecessores `ant` também é alocada e inicializada. Cada elemento da matriz é uma lista vazia.

4. O código percorre todas as arestas do grafo e atualiza a matriz `dist` e a lista de antecessores `ant` com as informações das arestas.

5. Em seguida, o algoritmo principal de Floyd-Warshall é executado. Ele usa três loops aninhados para iterar sobre todos os pares de vértices e atualizar as distâncias mínimas.

6. Dentro do loop, é verificado se há um caminho mais curto passando pelo vértice intermediário `k`. Se existir, a distância mínima é atualizada e a lista de antecessores é atualizada.

7. Se houver mais de um caminho de mesma distância mínima entre `i` e `j`, os antecessores são adicionados à lista de antecessores `ant` somente se eles ainda não estiverem presentes.

8. Após a conclusão do algoritmo, a matriz `dist` é desalocada.

9. Por fim, a função retorna a matriz de listas de antecessores `ant`.





## Main (Algoritmo de rankeamento)

```
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
```

O código calcula a centralidade de cada vértice em um grafo usando o algoritmo de centralidade de grau. Explicação passo a passo do que cada parte do código faz:

1. É criado um array `centralityArray` para armazenar as informações de centralidade de cada vértice. O tamanho do array é igual ao número de vértices `n`. A estrutura `VerticeCentrality` é usada para armazenar o ID do vértice e sua centralidade.

2. Em seguida, são percorridos todos os pares de vértices `u` e `v` do grafo usando dois loops aninhados.

3. Para cada par de vértices `u` e `v`, é chamada a função `sigma` para calcular a quantidade de caminhos mínimos entre `u` e `v` que passam por cada vértice do grafo. A função `sigma` retorna um array `sigma_w` e a variável `size` que representa o tamanho desse array.

4. O código percorre todos os vértices `w` do grafo usando um loop e calcula a centralidade do vértice `w` usando a fórmula `centralidade = sigma_w[w->id - 1] / size`. A centralidade é adicionada ao valor existente no array `centralityArray` correspondente ao vértice `w`.

5. Após o cálculo da centralidade para todos os pares de vértices `u` e `v`, a memória alocada para o array `sigma_w` é liberada.

6. Em seguida, o array `centralityArray` é ordenado em ordem decrescente com base no valor da centralidade usando a função `qsort` e a função `compareCentralidade` como função de comparação.

7. Por fim, o código imprime os vértices ordenados (Rankeados) pelo valor da centralidade, exibindo o ID do vértice e o valor da centralidade.

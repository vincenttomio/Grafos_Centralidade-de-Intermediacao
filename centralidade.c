#include <stdio.h>
#include <stdlib.h>
#include "centralidade.h"


// Função de comparação para qsort
int compareCentralidade(const void *a, const void *b)
{
  VerticeCentrality *v1 = (VerticeCentrality *)a;
  VerticeCentrality *v2 = (VerticeCentrality *)b;

  if (v1->centralidade > v2->centralidade)
    return -1;
  else if (v1->centralidade < v2->centralidade)
    return 1;
  else
    return 0;
}

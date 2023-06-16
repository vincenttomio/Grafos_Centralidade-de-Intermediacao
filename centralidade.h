typedef struct
{
  int id;
  double centralidade;
} VerticeCentrality;

// Função de comparação para qsort
int compareCentralidade(const void *a, const void *b);
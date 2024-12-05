/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 24/09/2024 para a disciplina CI1001 - Programação 1.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

//imprime um vetor de racionais
void imprime_vetor(struct racional **v, int n) {
  int i;

  if (!v)
    v = NULL;
    
  printf("VETOR = ");
  i = 0;
  imprime_r(v[i]);
  for (int i = 1; i < n; i++) {
    printf(" ");
    imprime_r(v[i]);
  }
  printf("\n");
}

//elimina racionais invalidos de um vetor
void elimina_NaN(struct racional **v, int *n) {

  if (!v || !*v) {
    v = NULL;
    return;
  }
  if (!n)
    return;

  for (int i = 0; i < *n; ) {
    if (!valido_r(v[i])) {
      destroi_r(v[i]);
      v[i] = v[*n - 1];
      (*n)--;
    }
    else
      i++;
  }
}

//ordena um vetor de forma crescente
void ordena_vetor(struct racional **v, int n) {

  if (!v || !*v) {
    v = NULL;
    return;
  }

  struct racional *temp;

  for (int i = 0; i < n - 1; i++)
    for (int j = i + 1; j < n; j++)
      if (compara_r(v[i], v[j]) == 1) {
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
        }
}

//aloca e atribui elementos de um vetor 
int preenche_vetor(struct racional **v, int n) {
  long num, den;
  
  if (!v || !*v) {
    v = NULL;
    return 0;
  }
  for (int i = 0; i < n; i++) {
    scanf("%ld", &num);
    scanf("%ld", &den);
    v[i] = cria_r(num, den);
    if (!v[i]) {
      return 1;
    }
  }
  return 1;
}

//libera memoria dos elementos do vetor
void libera_vetor(struct racional **v, int n) {

  if (!v || !*v) {
    v = NULL;
    return;
  }
  for (int i = 0; i < n ; i++) {
    destroi_r(v[i]);
  }
}

/* programa principal */
int main() {

  struct racional **v;
  struct racional *soma;
  struct racional *resultado;
  int n;
  
  //aloca vetor de ponteiros e aloca seus elementos em seguida
  scanf("%d", &n);
  v = malloc(n * sizeof(struct racional*));
  if (!v) 
    return -1;
  if (!preenche_vetor(v, n)){
    libera_vetor (v, n);
    free (v);
    return -1;
  }
  imprime_vetor(v, n);
  
  //elimina Not a Number's
  elimina_NaN(v, &n);
  imprime_vetor(v, n);
  
  //ordena o vetor
  ordena_vetor(v, n);
  imprime_vetor(v, n);

  //calcula a soma do vetor
  soma = cria_r(0, 1);
  if (!soma) {
    libera_vetor(v, n);
    free(v);
    return -1;
  }
  for (int i = 0; i < n; i++) {
    resultado = cria_r(0, 1);
    if (!resultado) {
      destroi_r(soma);
      libera_vetor(v, n);
      free(v);
      return -1;
    }

    if (soma_r(soma, v[i], resultado)) {
      destroi_r(soma);
      soma = resultado;
    } 
    else
      destroi_r(resultado);
  }  
  
  printf("SOMA = ");
  imprime_r(soma);
  printf("\n");
  
  //libera a memória alocada
  destroi_r(soma);
  libera_vetor(v, n);
  imprime_vetor(v ,n);
  free(v);

  return 0;
}

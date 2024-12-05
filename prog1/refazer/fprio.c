// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples
// Trabalho feito por Vinicius Leon Paula - grr20241170

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria () {
  struct fprio_t *aux;
  
  aux = malloc(sizeof(struct fprio_t));
  if (!aux)
    return NULL;

  aux->prim = NULL;
  aux->num = 0;
  return aux;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f) {
  struct fpnodo_t *aux;

  if (!f)
    return NULL;

  if (!f->prim) {
    free(f);
    return NULL;
 }
 
  //lopping para apagar todos os elementos
  while (f->prim != NULL) {
    aux = f->prim;
    f->prim = f->prim->prox;
    free(aux);
  }

  //libera a fila
  free (f);
  return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {
  struct fpnodo_t *novoNodo, *aux;

  if (!f || !item) {
    return -1;
  }

  //alocando memoria para o novo nodo.
  novoNodo = malloc(sizeof(struct fpnodo_t));
  if (!novoNodo) {
    free(item);
    fprio_destroi(f);
    return -1;
  }

  //preenchendo novo nodo.
  novoNodo->item = item;
  novoNodo->tipo = tipo;
  novoNodo->prio = prio;
  novoNodo->prox = NULL;

  aux = f->prim;
  //lopping para verificar se o elemento já existe.
  while (aux != NULL && aux->item != item)
    aux = aux->prox;

  //caso de um item igual ser encontrado na lista.
  if (aux != NULL) {
    free(novoNodo);
    free(item);
    return -1;
  }

  //fila vazia ou inserindo nodo no começo.
  if (f->prim == NULL || prio < f->prim->prio) {
    novoNodo->prox = f->prim;
    f->prim = novoNodo;
  }
  //colocando nodo no meio ou final da fila.
  else {
    aux = f->prim;
    //looping para aux chegar até a posição desejada e verificar se há elementos iguais.
    while (aux->prox != NULL && aux->prox->prio <= prio)
      aux = aux->prox;
    //coloca o novo nodo na posição.
    novoNodo->prox = aux->prox;
    aux->prox = novoNodo;
  }
  f->num++;
  return f->num;
}


// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
  struct fpnodo_t *aux;
  void *itemAuxiliar;

  //fila vazia ou inválida
  if (!f || !f->prim || !tipo || !prio)
    return NULL;

  aux = f->prim;
  //copia as informações do item retirado
  itemAuxiliar = aux->item;
  *tipo = aux->tipo;
  *prio = aux->prio;

  f->prim = aux->prox;
  free(aux);
  f->num--;

  return itemAuxiliar;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f) {
  if (!f)
    return -1;
  return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f){
  struct fpnodo_t *aux;

  if (!f || !f->prim)
    return;

  aux = f->prim;
  printf("(%d %d)", aux->tipo, aux->prio);
  
  //lopping para imprimir a fila de prioridades
  for (int i = 1; i < f->num; i++) {
    aux = aux->prox;
    printf(" (%d %d)", aux->tipo, aux->prio);
  }
}
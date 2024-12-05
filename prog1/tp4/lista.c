// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular
//
//Trabalho feito por: Vinicius Leon Paula - grr20241170;
#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL em erro.
struct lista_t *lista_cria () {
  struct lista_t *nova;

  nova = malloc(sizeof(struct lista_t));
  if (!nova)
    return NULL;
  
  //inicia valores da lista 
  nova->prim = NULL;
  nova->ult = NULL;
  nova->tamanho = 0;
  return nova;
}

// Remove todos os itens da lista e libera a memória.
// Retorno: NULL.
struct lista_t *lista_destroi (struct lista_t *lst) {
  struct item_t *aux, *prox;

  if (!lst) {
    return NULL;
  }

  aux = lst->prim;

  while (lst->tamanho > 0) {
    prox = aux->prox;
    free(aux);
    aux = prox;
    (lst->tamanho)--;
  }

  free(lst);
  return NULL;
}

// Nas operações insere/retira/consulta/procura, a lista inicia na
// posição 0 (primeiro item) e termina na posição TAM-1 (último item).

// Insere o item na lista na posição indicada;
// se a posição for além do fim da lista ou for -1, insere no fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_insere (struct lista_t *lst, int item, int pos) {
  struct item_t *novoItem, *aux;
 
  if (!lst)
    return -1;

  novoItem = malloc(sizeof(struct item_t));
  if (!novoItem)
    return -1;
  novoItem->valor = item;

  //verifica se a lista está vazia ou se é uma inserção no inicio.
  if (!(lst->prim) || pos == 0) {
    novoItem->prox = lst->prim;
    novoItem->ant = NULL;
    if (lst->prim != NULL)
      lst->prim->ant = novoItem;
    lst->prim = novoItem;
    if (lst->ult == NULL)
      lst->ult = novoItem;
  }
  else {
    aux = lst->prim;

    //insere na ultima posição
    if (pos == -1 || pos >= lst->tamanho) {
      aux = lst->ult;
      novoItem->prox = NULL;
      novoItem->ant = aux;
      aux->prox = novoItem;
      lst->ult = novoItem;
    }
    else {
      //lopping para aux chegar até a posição anterior que se quer inserir.
      for (int i = 0; i < pos - 1 && aux->prox != NULL; i++)
        aux = aux->prox;

      //inseri elemento no meio ou no fim
      novoItem->prox = aux->prox;
      novoItem->ant = aux;
      if (aux->prox != NULL)
        aux->prox->ant = novoItem;
      aux->prox = novoItem;
    }
  }

  (lst->tamanho)++;
  return lst->tamanho;
}

// Retira o item da lista da posição indicada.
// se a posição for -1, retira do fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_retira(struct lista_t *lst, int *item, int pos) {
    struct item_t *aux, *prev;

    if (!lst || !item)
        return -1;

    if (!lst->prim)
        return -1;

    aux = lst->prim;
    prev = NULL;

    if (pos == -1)
        pos = lst->tamanho - 1;

    // Looping para aux chegar à posição que se quer retirar
    for (int i = 0; i < pos && aux != NULL; i++) {
        prev = aux;
        aux = aux->prox;
    }

    // Testa posição inválida
    if (aux == NULL)
        return -1;

    // Guarda o valor do item removido
    *item = aux->valor;

    // Remove o primeiro item
    if (aux == lst->prim) {
        lst->prim = aux->prox;
        if (lst->prim != NULL) {
            lst->prim->ant = NULL;
        }
    } else {
        prev->prox = aux->prox;
        if (aux->prox != NULL) {
            aux->prox->ant = prev;
        }
    }

    free(aux);
    lst->tamanho--;

    return lst->tamanho;
}

// Informa o valor do item na posição indicada, sem retirá-lo.
// se a posição for -1, consulta do fim.
// Retorno: número de itens na lista ou -1 em erro.
int lista_consulta (struct lista_t *lst, int *item, int pos) {
  struct item_t *aux;

  if (!lst || !item)
    return -1;

  if (lst->tamanho == 0)
    return -1;
  
  aux = lst->prim;

  if(pos == -1)
    pos = lst->tamanho -1;

  //verifica se a posição é válida
  if (pos < 0 || pos >= lst->tamanho)
    return -1;

  //looping para aux chegar a posição indicada.
  for (int i = 0; i < pos && aux != NULL; i++)
    aux = aux->prox;

  *item = aux->valor;
  return lst->tamanho;
}

// Informa a posição da 1ª ocorrência do valor indicado na lista.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int lista_procura (struct lista_t *lst, int valor) {
  struct item_t *aux;

  if (!lst)
    return -1;

  if (!lst->prim)
    return -1;
  
  aux = lst->prim;

  //looping para verificar elementos da lista.
  for (int i = 0; i < lst->tamanho; i++) {
    //checa elemento atual da lista
    if (aux->valor == valor)
      return i;

    //vai para o proximo elemento
    aux = aux->prox;
  }
  
  //valor não encontrado
  return -1;
}

// Informa o tamanho da lista (o número de itens presentes nela).
// Retorno: número de itens na lista ou -1 em erro.
int lista_tamanho (struct lista_t *lst) {
  if (!lst)
    return -1;

  return lst->tamanho;
}

// Imprime o conteúdo da lista do inicio ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void lista_imprime (struct lista_t *lst) {
  struct item_t *aux;

  if (!lst)
    return;

  if (!lst->prim)
    return;
  
  //imprime primeiro elemento.
  aux = lst->prim;
  printf("%d", aux->valor);

  aux = aux->prox;
  
  //looping para imprimir o restante da lista.
  for (int i = 1; i < lst->tamanho; i++) {
    printf(" %d", aux->valor);
    aux = aux->prox;
  }
}

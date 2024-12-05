#ifndef MUNDO_H
#define MUNDO_H

#include <stdbool.h>
#include <sys/types.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"

/*
===================================
estruturas de dados
===================================
*/

struct coordenada_t {
  int x;
  int y;
};

struct heroi_t {
  int ID;
  struct cjto_t *habilidades;
  int paciencia;
  int velocidade;
  int experiencia;
  int local;
  bool vivo;
};

struct base_t {
  int ID;
  int lotacao;
  struct cjto_t *presentes;
  struct lista_t *espera;
  struct coordenada_t local;
  int distanciaProxMissao;
  int missoesAtendidas;
  int filaMax;
};

struct missao_t {
  int ID;
  struct cjto_t *habilidades;
  int perigo;
  struct coordenada_t local;
  int tentativas;
  bool cumprida;
};

struct mundo_t {
  int nHerois;
  struct heroi_t **herois;
  int nBases;
  struct base_t **bases;
  int nMissoes;
  struct missao_t **missoes;
  struct coordenada_t tamanhoMundo;
  int relogio;
  int missoesRealizadas;
  int eventosTratados;
  int minMissoes;
  int maxMissoes; 
};

struct evento_t {
  struct heroi_t *heroi;
  struct base_t *base;
  struct missao_t *missao;
};

/*
===================================
funções auxiliares para criar 
e destruir de estruturas
===================================
*/

struct heroi_t *criaHeroi(int ID, int local);

struct heroi_t *destroiHeroi(struct heroi_t *h);

struct base_t *criaBase(int ID, int tamanhoMundo);

struct base_t *destroiBase(struct base_t *b);

struct missao_t *criaMissao(int ID, int tamanhoMundo);

struct missao_t *destroiMissao(struct missao_t *m);

struct mundo_t *criaMundo(int nHerois, struct heroi_t **herois, int nBases, struct base_t **bases,
                          int nMissoes, struct missao_t **missoes, struct coordenada_t tamanhoMundo);

struct mundo_t *destroiMundo(struct mundo_t *m);

struct evento_t *criaEvento(struct heroi_t *heroi, struct base_t *base, struct missao_t *missao);

struct evento_t *destroiEvento(struct evento_t *e);

/*
===================================
eventos iniciais
===================================
*/

//define o tempo aleatório de chegada de cada herói em uma base aleatória
void inicializaHerois(int relogio, struct mundo_t *m, struct fprio_t *LEF);

//agenda quando cada missão ocorrera
void inicializaMissoes(int relogio, struct mundo_t *m, struct fprio_t *LEF, int FimDoMundo);

//agenda o fim da simulação.
void agendaFim(struct fprio_t *LEF, int FimDoMundo);

/*
===================================
eventos da simulação
===================================
*/

//Representa um herói H chegando em uma base B no instante T.
void chega(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF);

//O herói H entra na fila de espera da base B.
void espera(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF);

//O herói H desiste de entrar na base B.
void desiste(int relogio, struct heroi_t *heroi, struct mundo_t *mundo, struct fprio_t *LEF);

//O porteiro da base B trata a fila de espera.
void avisa(int relogio, struct base_t *base, struct fprio_t *LEF);

//O herói H entra na base B.
void entra(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF);

//O herói H sai da base B.
void sai(int relogio, struct heroi_t *heroi, struct base_t *base, struct mundo_t *mundo, struct fprio_t *LEF);

//O herói H se desloca para uma base D.
void viaja(int relogio, struct heroi_t *heroi, struct base_t *base, struct mundo_t *mundo, struct fprio_t *LEF);

//O herói H morre no instante T.
void morre(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF);

//Uma missão M é disparada no instante T.
void missao(int relogio, struct missao_t *missao, struct mundo_t *mundo, struct fprio_t *LEF);

//Encerra a simulação no instante T.
void fim(int relogio, struct mundo_t *mundo, struct fprio_t *LEF);

#endif
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "mundo.h"
#include "conjunto.h"

/*
===================================
funções auxiliares gerais
===================================
*/
int aleat(int max) {
  return rand() % max;
}

int calculaDistancia(struct base_t *origem, struct base_t *destino) {
  int distancia;

  if (!origem || !destino)
    return 0;

  distancia = sqrt(pow(destino->local.x - origem->local.x, 2) + pow(destino->local.y - origem->local.y, 2));
  return distancia;
}
/*
===================================
funções auxiliares para criação 
e destruição de estruturas
===================================
*/

struct heroi_t *criaHeroi(int ID, struct cjto_t *habilidades, int paciencia,
                          int velocidade, int experiencia, int local, bool vivo) {
  struct heroi_t *h;

  if (!habilidades)
    return NULL;

  h = malloc(sizeof(struct heroi_t));

  if (!h)
    return NULL;

  h->ID = ID;
  h->habilidades = habilidades;
  h->paciencia = paciencia;
  h->velocidade = velocidade;
  h->experiencia = experiencia;
  h->local = local;
  h->vivo = true;

  return h;
}

struct heroi_t *destroiHeroi(struct heroi_t *h) {
  if (!h)
    return NULL;

  if (!h->habilidades) {
    free(h);
    return NULL;
  }
        
  cjto_destroi(h->habilidades);
  free(h);
}

struct base_t *criaBase(int ID, int lotacao, struct cjto_t *presentes, struct lista_t *espera,
                                                 struct coordenada_t local, int distancia_prox_missao) {
  struct base_t *b;

  if (!presentes && espera) {
    lista_destroi(espera);
    return NULL;
  }

  if (presentes && !espera) {
    cjto_destroi(presentes);
    return NULL;
  }

  if (!presentes && !espera)
    return NULL;


  b = malloc(sizeof(struct base_t));

  if (!b)
    return NULL;

  b->ID = ID;
  b->lotacao = lotacao;
  b->presentes = presentes;
  b->espera = espera;
  b->local = local;
  b->distancia_prox_missao = distancia_prox_missao;

  return b;
}

struct base_t *destroiBase(struct base_t *b) {
  if (!b)
    return NULL;
        
  if (!b->presentes) {
    if (!b->espera)
      free(b);
    else
      lista_destroi(b->espera);

    free(b);
    return NULL;
    }

  if (!b->espera) {
    cjto_destroi(b->presentes);
    free(b);
    return NULL;
    }
        
  cjto_destroi(b->presentes);
  lista_destroi(b->espera);
  free(b);
  return NULL;
}

struct missao_t *criaMissao(int ID, struct cjto_t *habilidades, int perigo, struct coordenada_t local, bool cumprida) {
  struct missao_t *m;
  
  if (!habilidades)
    return NULL;

  m = malloc(sizeof(struct missao_t));

  if (!m)
    return NULL;

  m->ID = ID;
  m->habilidades = habilidades;
  m->perigo = perigo;
  m->local = local;
  m->cumprida = cumprida;

  return m;
}

struct missao_t *destroiMissao(struct missao_t *m) {
  if (!m)
    return NULL;

  if (!m->habilidades) {
    free(m);
    return NULL;
  }
        
  cjto_destroi(m->habilidades);
  free(m);
}

struct mundo_t *criaMundo(int nHerois, struct heroi_t *herois, int nBases,
                          struct base_t *bases, int nMissoes, struct missao_t *missoes,
                          struct coordenada_t tamanhoMundo, int inicio) {               
  struct mundo_t *m;

  if (!herois || !bases || !missoes)
    return NULL;
  

  m = malloc(sizeof(struct mundo_t));

  if (!m)
    return NULL;

  m->nHerois = nHerois;
  m->herois = herois;
  m->nBases = nBases;
  m->bases = bases;
  m->nMissoes = nMissoes;
  m->missoes = missoes;
  m->tamanhoMundo = tamanhoMundo;
  m->relogio = inicio;

  return m;
}

struct mundo_t *destroiMundo(struct mundo_t *m) {
  if (!m)
    return NULL;

  if (!m->herois) {
    if (!m->bases) {
      if (!m->missoes) {
        free(m);
        return NULL;
      }
      else {
        //se m->missoes não for NULL
        for (int i = 0; i < m->nMissoes; i++)
          destroi_missao(&m->missoes[i]);
        free(m);
        return NULL;
      }
    }
    else {
      if (!m->missoes) {
        //se m->bases não for NULL
        for (int i = 0; i < m->nBases; i++)
          destroi_base(&m->bases[i]);
        free(m);
        return NULL;
      }
      else {
        //se m->bases e m->missoes não forem NULL
        for (int i = 0; i < m->nBases; i++)
          destroi_base(&m->bases[i]);
        for (int i = 0; i < m->nMissoes; i++)
          destroi_missao(&m->missoes[i]);
        free(m);
        return NULL;
      }
    }
  }
  else {
    if (!m->bases && !m->missoes) {
      //se m->herois não for NULL
      for (int i = 0; i < m->nHerois; i++)
        destroi_heroi(&m->herois[i]);
      free(m);
      return NULL;
    }
    else if (!m->bases) {
      //se m->herois e m->missoes não forem NULL
      for (int i = 0; i < m->nHerois; i++)
        destroi_heroi(&m->herois[i]);
      for (int i = 0; i < m->nMissoes; i++)
        destroi_missao(&m->missoes[i]);
      free(m);
      return NULL;
    }
    else if (!m->missoes) {
      //se m->herois e m->bases não forem NULL
      for (int i = 0; i < m->nHerois; i++)
        destroi_heroi(&m->herois[i]);
      for (int i = 0; i < m->nBases; i++)
        destroi_base(&m->bases[i]);
      free(m);
      return NULL;
    }
    else {
      //se m->herois, m->bases e m->missoes não forem NULL
      for (int i = 0; i < m->nHerois; i++)
        destroi_heroi(&m->herois[i]);
      for (int i = 0; i < m->nBases; i++)
        destroi_base(&m->bases[i]);
      for (int i = 0; i < m->nMissoes; i++)
        destroi_missao(&m->missoes[i]);
      free(m);
      return NULL;
    }
  }
}

/*
===================================
eventos iniciais
===================================
*/

//define o tempo aleatório de chegada de cada herói em uma base aleatória
void inicializaHerois(int relogio, struct mundo_t *m, struct fprio_t *LEF) {
  int base, tempo;

  if (!m || !LEF)
    return;
    
  for (int i = 0; i < m->nHerois; i++) {
    base = aleat(m->nBases);
    tempo = aleat(4320);
    f_prio_insere(LEF, &m->herois[i], 1, relogio + tempo);
  }
}

//agenda quando cada missão ocorrera
void inicializaMissoes(int relogio, struct mundo_t *m, struct fprio_t *LEF, int fimDoMundo) {
  int tempo;

  if (!m || !LEF)
    return;
    
  for (int i = 0; i < m->nMissoes; i++) {
    tempo = aleat(fimDoMundo - relogio);
    f_prio_insere(LEF, &m->missoes[i], 9, relogio + tempo);
  }
}

//agenda o fim da simulação.
void agendaFim(int relogio, struct fprio_t *LEF, int FimDoMundo) {
  if (!LEF)
    return;
  f_prio_insere(LEF, 2012, 10, FimDoMundo);
}

/*
===================================
eventos da simulação
===================================
*/

//Representa um herói H chegando em uma base B no instante T.
//tipo 1.
void chega(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF) {
  // Atualiza a base do herói
  heroi->local = base->ID;

  bool espera = false;
  if (base->presentes->num < base->lotacao && base->espera->tamanho == 0)
    espera = true;
  else
    espera = heroi->paciencia > (10 * base->espera->tamanho);

  if (espera)
    //insere o evento chega na lista de eventos futuros
    f_prio_insere(LEF, heroi, 1, relogio);
  else
    //insere o evento desiste na lista de eventos futuros
    f_prio_insere(LEF, heroi, 3, relogio);
}

//insere o herói na fila de espera da base
//tipo 2.
void espera(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF) {
  if (!base || !heroi || !LEF)
    return;

  lista_insere(base->espera, heroi, -1);
  //insere o evento avisa na lista de eventos futuros
  f_prio_insere(LEF, base, 4, relogio);
}

//O herói H desiste de entrar na base B e escolhe uma base aleatória para viajar.
//tipo 3.
void desiste(int relogio, struct heroi_t *heroi, struct mundo_t *mundo, struct fprio_t *LEF) {
  int destino;
  
  if(!mundo || !heroi || !LEF)
    return;
  
  //escolhe uma base aleatória para viajar
  destino = aleat(mundo->nBases);
  //insere o evento viaja na lista de eventos futuros
  f_prio_insere(LEF, heroi, 7, relogio);

  /*
  ===============
  duvidaaaaaaa
  ===============
  */
}

//O porteiro da base B trata a fila de espera.
//tipo 4.
void avisa(int relogio, struct base_t *base, struct fprio_t *LEF) {
  if (!base || !LEF)
    return;

  while (base->presentes->num < base->lotacao && base->espera->tamanho > 0) {
    struct heroi_t *heroi;
    lista_retira(base->espera, &heroi, 0);
    cjto_insere(base->presentes, heroi->ID);
    //insere o evento entra na lista de eventos futuros
    f_prio_insere(LEF, heroi, 5, relogio);
  }
  
}

//O herói H entra na base B.
//tipo 5.
void entra(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF) {
  if (!heroi || !base || !LEF)
    return;

  //calcula o tempo de permanência do herói na base
  int tpb = 15 + heroi->paciencia + aleat(20);
  //insere o evento sai na lista de eventos futuros
  f_prio_insere(LEF, heroi, 6, relogio + tpb);
}

//O herói H sai da base B.
//tipo 6.
void sai(int relogio, struct heroi_t *heroi, struct base_t *base, struct mundo_t *mundo, struct fprio_t *LEF) {
  if (!heroi || !base || !LEF)
    return;

  cjto_remove(base->presentes, heroi->ID);

  //esolhe uma base aleatória para viajar
  int destino = aleat(mundo->nBases);

  //insere o evento viaja na lista de eventos futuros
  f_prio_insere(LEF, heroi, 7, relogio);  
  //insere o evento avisa na lista de eventos futuros
  f_prio_insere(LEF, base, 4, relogio);
}

//O herói H se desloca para uma base D.
//tipo 7.
void viaja(int relogio, struct heroi_t *heroi, struct base_t *origem, struct base_t *destino, struct fprio_t *LEF) {
  if (!heroi || !destino || !LEF)
    return;

  //calcula a distancia cartesina entre a base atual e a base destino
  int distancia = calculaDistancia(origem, destino);
  //calcula o tempo de viagem
  int tv = distancia / heroi->velocidade;
  //insere o evento chega na lista de eventos futuros
  f_prio_insere(LEF, heroi, 1, relogio + tv);
}

//O herói H morre no instante T.
//tipo 8.
void morre(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF) {
  if (!heroi || !base || !LEF)
    return;

  //remove o herói da base
  cjto_remove(base->presentes, heroi->ID);

  //muda o status do herói para morto
  heroi->vivo = false;

  //insere o evento avisa na lista de eventos futuros
  f_prio_insere(LEF, base, 4, relogio);
}

//Uma missão M é disparada no instante T.
//tipo 9.
void missao(int relogio, struct missao_t *missao, struct mundo_t *mundo, struct fprio_t *LEF){
  struct base_t *bmp, *baseCapaz;
  if (!missao || !mundo || !LEF)
    return;
  
  baseCapaz = NULL;

  bmp = malloc(mundo->nBases * sizeof(struct base_t));
  if (!bmp)
    return;

  //calcula a distancia entre cada base e a missão
  for (int i = 0; i < mundo->nBases; i++) {
    mundo->bases[i].distancia_prox_missao = calculaDistancia(&mundo->bases[i], missao);
    bmp[i] = mundo->bases[i];
  }
  //ordena as bases de acordo com a distancia da missão
  for (int i = 0; i < mundo->nBases; i++) {
    for (int j = 0; j < mundo->nBases; j++) {
      if (bmp[i].distancia_prox_missao < bmp[j].distancia_prox_missao) {
        struct base_t aux = bmp[i];
        bmp[i] = bmp[j];
        bmp[j] = aux;
      }
    }
  }
  
  //encontra a base mais próxima da missão cujos heróis possuem as habilidades necessárias para cumpri-la (bmp)
  for (int i = 0; i < mundo->nBases; i++) {
    //caso a base possua as habilidades necessárias
    if (cjto_contem(missao->habilidades, bmp[i].presentes)) {
      baseCapaz = &bmp[i];
      break;
    }
  }
  if (baseCapaz) {

    for (int i = 0; i < mundo->nHerois; i++) {
      //caso o heroi exista na base capaz
      if (cjto_pertence(baseCapaz->presentes, mundo->herois[i].ID)) {
        //calcula o risco de morte do heroi
        float risco = missao->perigo / (mundo->herois[i].paciencia + mundo->herois[i].experiencia + 1.0);
        if (risco > aleat(30))
          //insere o evento morre na lista de eventos futuros
          f_prio_insere(LEF, &mundo->herois[i], 8, relogio);
        else 
          mundo->herois[i].experiencia += 1;
      }
    }
  }
  else
    //insere o evento missao na lista de eventos futuros
    f_prio_insere(LEF, missao, 9, relogio + 24*60);
}

//Encerra a simulação no instante T.
//tipo 10.
void fim(int relogio, struct mundo_t *mundo,  struct fprio_t *LEF) {
  if (!LEF)
    return;
  
  printf("%d: FIM\n\n", relogio);

  //imprime as estatísticas da simulação;

  //imprime herois  
  for (int i = 0; i < mundo->nHerois; i++) {
    if (mundo->herois[i].vivo)
      printf("Heroi %d: vivo\n", mundo->herois[i].ID);
    else
      printf("Heroi %d: morto\n", mundo->herois[i].ID);
  }

  //imprime bases
  for (int i = 0; i < mundo->nBases; i++) {
    printf("Base %d lot %d FILA MAX  \n", mundo->bases[i].ID, mundo->bases[i].presentes->num);
  }

  //imprime missoes

  //libera a memória alocada
  destroiMundo(mundo);
}
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
int aleat(int min, int max) {
  return min + rand() % (max - min);
}

double calculaDistancia(struct base_t *origem, struct base_t *destino) {
  double distancia;

  if (!origem || !destino)
    return 0;

  distancia = sqrt(pow(destino->local.x - origem->local.x, 2) + pow(destino->local.y - origem->local.y, 2));
  return distancia;
}

double calculaDistanciaMissao(struct base_t *origem, struct missao_t *missao) {
  double distancia;

  if (!origem || !missao)
    return 0;

  distancia = sqrt(pow(missao->local.x - origem->local.x, 2) + pow(missao->local.y - origem->local.y, 2));
  return distancia;
}

/*
===================================
funções auxiliares para criação 
e destruição de estruturas
===================================
*/


struct heroi_t *criaHeroi(int ID, int local) {
  struct heroi_t *h;

  h = malloc(sizeof(struct heroi_t));

  if (!h)
    return NULL;

  h->habilidades = cjto_cria(3);

  h->ID = ID;
  h->habilidades = cjto_aleat(aleat(1, 3), 3);
  h->paciencia = aleat(0, 100);
  h->velocidade = aleat(50, 5000);
  h->experiencia = 0;
  h->local = local;
  h->vivo = true;

  return h;
}

struct heroi_t *destroiHeroi(struct heroi_t *h) {
  if (!h)
    return NULL;
  
  if (h->habilidades) {
    cjto_destroi(h->habilidades);
  }
        
  free(h);
  return NULL;
}

struct base_t *criaBase(int ID, int tamanhoMundo) {
  struct base_t *b;
  struct coordenada_t localBase;

  b = malloc(sizeof(struct base_t));
  if (!b)
    return NULL;

  //inicializa os valores da base
  b->ID = ID;
  b->lotacao = aleat(3, 10);
  b->presentes = cjto_cria(b->lotacao);
  b->espera = lista_cria();
  //inicializa a localização da base
  localBase.x = aleat(0, tamanhoMundo);
  localBase.y = aleat(0, tamanhoMundo);  
  b->local = localBase;
  //inicializa variaveis auxiliares
  b->distanciaProxMissao = 0;
  b->missoesAtendidas = 0;
  b->filaMax = 0;

  return b;
}

struct base_t *destroiBase(struct base_t *b) {
  if (!b)
    return NULL;

  if (b->presentes)
    cjto_destroi(b->presentes);

  if (b->espera)
    lista_destroi(b->espera);

  free(b);
  return NULL;
}

struct missao_t *criaMissao(int ID, int tamanhoMundo) {
  struct missao_t *m;
  struct coordenada_t localMissao;

  m = malloc(sizeof(struct missao_t));

  if (!m)
    return NULL;

  m->ID = ID;
  m->habilidades = cjto_aleat(aleat(6, 10), 10);
  m->perigo = aleat(0, 100);
  localMissao.x = aleat(0, tamanhoMundo);
  localMissao.y = aleat(0, tamanhoMundo);
  m->local = localMissao;
  m->cumprida = false;
  m->tentativas = 0;

  return m;
}

struct missao_t *destroiMissao(struct missao_t *m) {
  if (!m)
    return NULL;

  if (m->habilidades) {
    cjto_destroi(m->habilidades);
  }

  free(m);
  return NULL;
}

struct mundo_t *criaMundo(int nHerois, struct heroi_t **herois, int nBases, struct base_t **bases,
                          int nMissoes, struct missao_t **missoes, struct coordenada_t tamanhoMundo) {               
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
  m->relogio = 0;
  m->missoesRealizadas = 0;
  m->eventosTratados = 0;
  m->minMissoes = 0;
  m->maxMissoes = 0;

  return m;
}

struct mundo_t *destroiMundo(struct mundo_t *m) {
  if (!m)
    return NULL;
  
  //destroi vetor de herois
  if (m->herois) {
    for (int i = 0; i < m->nHerois; i++)
      if (m->herois[i])
        destroiHeroi(m->herois[i]);
    free(m->herois);
  }

  //destroi vetor de bases
  if (m->bases) {
    for (int i = 0; i < m->nBases; i++)
      if (m->bases[i])
        destroiBase(m->bases[i]);
    free(m->bases);
  }

  //destroi vetor de missoes
  if (m->missoes) {
    for (int i = 0; i < m->nMissoes; i++)
      if (m->missoes[i])
        destroiMissao(m->missoes[i]);
    free(m->missoes);
  }

  free(m);
  return NULL;
}

struct evento_t *criaEvento(struct heroi_t *heroi, struct base_t *base, struct missao_t *missao){
  struct evento_t *e;

  e = malloc(sizeof(struct evento_t));

  if (!e)
    return NULL;

  e->heroi = heroi;
  e->base = base;
  e->missao = missao;

  return e;
}

struct evento_t *destroiEvento(struct evento_t *e){
  if (!e)
    return NULL;
  
  //destroi os ponteiros
  if (e->heroi)
    destroiHeroi(e->heroi);

  if (e->base)
    destroiBase(e->base);

  if (e->missao)
    destroiMissao(e->missao);

  free(e);
  return NULL;
}

/*
===================================
eventos iniciais
===================================
*/

//define o tempo aleatório de chegada de cada herói em uma base aleatória
void inicializaHerois(int relogio, struct mundo_t *m, struct fprio_t *LEF) {
  int base, tempo;
  struct evento_t *evento;

  if (!m || !LEF)
    return;

  //para cada heroi sorteia uma base e um tempo de chegada
  for (int i = 0; i < m->nHerois; i++) {
    base = aleat(0, m->nBases);
    tempo = aleat(0, 4320);

    //cria e insere o evento chega na lista de eventos futuros
    evento = criaEvento(m->herois[i], m->bases[base], NULL);
    fprio_insere(LEF, evento, 1, relogio + tempo);
  }
}

//agenda quando cada missão ocorrera
void inicializaMissoes(int relogio, struct mundo_t *m, struct fprio_t *LEF, int fimDoMundo) {
  int tempo;
  struct evento_t *evento;

  if (!m || !LEF)
    return;
    
  //para cada missao sorteia um tempo em que ela ocorrerá
  for (int i = 0; i < m->nMissoes; i++) {
    tempo = aleat(0, fimDoMundo);
    //cria e insere o evento missao na lista de eventos futuros
    evento = criaEvento(NULL, NULL, m->missoes[i]);
    fprio_insere(LEF, evento, 9, relogio + tempo);
  }
}

//agenda o fim da simulação.
void agendaFim(struct fprio_t *LEF, int FimDoMundo) {
  struct evento_t *evento;

  if (!LEF)
    return;

  //cria e insere o evento fim do mundo na lista de eventos futuros
  evento = criaEvento(NULL, NULL, NULL);
  fprio_insere(LEF, evento, 10, FimDoMundo);
}

/*
===================================
eventos da simulação
===================================
*/

//Representa um herói H chegando em uma base B no instante T.
//tipo 1.
void chega(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF) {
  struct evento_t *evento;

  if (!heroi || !base || !LEF)
    return;
  
  //testa se o heroi está morto
  if (heroi->vivo == false)
    return;

  // Atualiza a base do herói
  heroi->local = base->ID;
  
  bool espera = false;
  if (base->presentes->num < base->lotacao && base->espera->tamanho == 0)
    espera = true;
  else
    espera = heroi->paciencia > (10 * base->espera->tamanho);

  if (espera) {
    //insere o evento chega na lista de eventos futuros
    evento = criaEvento(heroi, base, NULL);
    fprio_insere(LEF, evento, 1, relogio);
  }
  else {
    //insere o evento desiste na lista de eventos futuros
    evento = criaEvento(heroi, base, NULL);
    fprio_insere(LEF, evento, 3, relogio);
  }
}

//insere o herói na fila de espera da base
//tipo 2.
void espera(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF) {
  struct evento_t *evento;

  if (!base || !heroi || !LEF)
    return;

  //testa se o heroi está morto
  if (heroi->vivo == false)
    return;

  //insere o heroi na fila de espera
  lista_insere(base->espera, heroi->ID, -1);

  //insere o evento avisa na lista de eventos futuros
  evento = criaEvento(NULL, base, NULL);
  fprio_insere(LEF, evento, 4, relogio);
}

//O herói H desiste de entrar na base B e escolhe uma base aleatória para viajar.
//tipo 3.
void desiste(int relogio, struct heroi_t *heroi, struct mundo_t *mundo, struct fprio_t *LEF) {
  int destino;
  struct evento_t *evento;

  if(!mundo || !heroi || !LEF)
    return;

  //testa se o heroi está morto
  if (heroi->vivo == false)
    return;
  
  //escolhe uma base aleatória para viajar
  destino = aleat(0, mundo->nBases);

  //insere o evento viaja na lista de eventos futuros
  evento = criaEvento(heroi, mundo->bases[destino], NULL);
  fprio_insere(LEF, evento, 7, relogio);
}

//O porteiro da base B trata a fila de espera.
//tipo 4.
void avisa(int relogio, struct base_t *base, struct fprio_t *LEF) {
  struct evento_t *evento;

  if (!base || !LEF)
    return;

  while (base->presentes->num < base->lotacao && base->espera->tamanho > 0) {
    /*
    ============================================
      trocar por heroi dentro do struct mundo_t
    ============================================
    */
    struct heroi_t *heroi;


    //retira o heroi da fila de espera e coloca dentro da base
    lista_retira(base->espera, &heroi->ID, 0);
    cjto_insere(base->presentes, heroi->ID);

    //insere o evento entra na lista de eventos futuros
    evento = criaEvento(heroi, base, NULL);
    fprio_insere(LEF, evento, 5, relogio);
  }
}

//O herói H entra na base B.
//tipo 5.
void entra(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF) {
  struct evento_t *evento;

  if (!heroi || !base || !LEF)
    return;

  //testa se o heroi está morto
  if (heroi->vivo == false)
    return;

  //calcula o tempo de permanência do herói na base
  int tpb = 15 + heroi->paciencia + aleat(1, 20);

  //insere o evento sai na lista de eventos futuros
  evento = criaEvento(heroi, base, NULL);
  fprio_insere(LEF, evento, 6, relogio + tpb);
}

//O herói H sai da base B.
//tipo 6.
void sai(int relogio, struct heroi_t *heroi, struct base_t *base, struct mundo_t *mundo, struct fprio_t *LEF) {
  struct evento_t *viaja, *avisa;

  if (!heroi || !base || !LEF)
    return;

  //testa se o heroi está morto
  if (heroi->vivo == false)
    return;

  cjto_retira(base->presentes, heroi->ID);

  //esolhe uma base aleatória para viajar
  int destino = aleat(1, mundo->nBases);

  //insere o evento viaja na lista de eventos futuros
  viaja = criaEvento(heroi, mundo->bases[destino], NULL);
  fprio_insere(LEF, viaja, 7, relogio);  
  //insere o evento avisa na lista de eventos futuros
  avisa = criaEvento(NULL, base, NULL);
  fprio_insere(LEF, avisa, 4, relogio);
}

//O herói H se desloca para uma base D.
//tipo 7.
void viaja(int relogio, struct heroi_t *heroi, struct base_t *base, struct mundo_t *mundo, struct fprio_t *LEF) {
  double distancia;
  int tempoViagem;
  struct evento_t *evento;

  if (!heroi || !mundo || !LEF)
    return;

  //testa se o heroi está morto
  if (heroi->vivo == false)
    return;

  //calcula a distancia cartesina entre a base atual e a base destino
  distancia = calculaDistancia(mundo->bases[heroi->local], base);

  //calcula o tempo de viagem
  tempoViagem = distancia / heroi->velocidade;

  //insere o evento chega na lista de eventos futuros
  evento = criaEvento(heroi, base, NULL);
  fprio_insere(LEF, evento, 1, relogio + tempoViagem);
}

//O herói H morre no instante T.
//tipo 8.
void morre(int relogio, struct heroi_t *heroi, struct base_t *base, struct fprio_t *LEF) {
  struct evento_t *avisa;

  if (!heroi || !base || !LEF)
    return;

  //testa se o heroi ja está morto
  if (heroi->vivo == false)
    return;

  //remove o herói da base
  cjto_retira(base->presentes, heroi->ID);

  //muda o status do herói para morto
  heroi->vivo = false;

  //insere o evento avisa na lista de eventos futuros
  avisa = criaEvento(NULL, base, NULL);
  fprio_insere(LEF, avisa, 4, relogio);
}

//Uma missão M é disparada no instante T.
//tipo 9.
void missao(int relogio, struct missao_t *missao, struct mundo_t *mundo, struct fprio_t *LEF){
  struct base_t **bmp, *baseCapaz;
  struct evento_t *eMissao, *eMorre;

  //testa ponteiros
  if (!missao || !mundo || !LEF)
    return;
  
  missao->tentativas += 1;
  baseCapaz = NULL;

  bmp = malloc(mundo->nBases * sizeof(struct base_t*));
  if (!bmp)
    return;

  //calcula a distancia entre cada base e a missão
  for (int i = 0; i < mundo->nBases; i++) {
    mundo->bases[i]->distanciaProxMissao = calculaDistanciaMissao(mundo->bases[i], missao);
    bmp[i] = mundo->bases[i];
  }
  //ordena as bases de acordo com a distancia da missão
  for (int i = 0; i < mundo->nBases; i++) {
    for (int j = 0; j < mundo->nBases; j++) {
      if (bmp[i]->distanciaProxMissao < bmp[j]->distanciaProxMissao) {
        struct base_t *aux = bmp[i];
        bmp[i] = bmp[j];
        bmp[j] = aux;
      }
    }
  }
  
  //encontra a base mais próxima da missão cujos heróis possuem as habilidades necessárias para cumpri-la (bmp)
  for (int i = 0; i < mundo->nBases; i++) {
    //caso a base possua as habilidades necessárias
    if (cjto_contem(missao->habilidades, bmp[i]->presentes)) {
      baseCapaz = bmp[i];
      break;
    }
  }

  //caso exista uma base capaz de cumprir a missão
  if (baseCapaz) {
    //atualiza os status de missão
    missao->cumprida = true;
    mundo->missoesRealizadas += 1;

    for (int i = 0; i < mundo->nHerois; i++) {
      //caso o heroi exista na base capaz
      if (cjto_pertence(baseCapaz->presentes, mundo->herois[i]->ID)) {

        //calcula o risco de morte do heroi
        float risco = missao->perigo / (mundo->herois[i]->paciencia + mundo->herois[i]->experiencia + 1.0);
        //joga os dados para ver se o herói morre
        if (risco > aleat(0, 30)) {
          //insere o evento morre na lista de eventos futuros
          eMorre = criaEvento(mundo->herois[i], baseCapaz, NULL);
          fprio_insere(LEF, eMorre, 8, relogio);
        }
        else
          //herói ganha experiencia 
          mundo->herois[i]->experiencia += 1;
      }
    }
  }
  //caso não exista nenhuma base capaz de cumprir a missão
  else {
    //insere o evento missao na lista de eventos futuros para o dia seguinte
    eMissao = criaEvento(NULL, NULL, missao);
    fprio_insere(LEF, eMissao, 9, relogio + 24*60);
  }
  //libera o vetor de bases mais proximas
  free(bmp);
}

//Encerra a simulação no instante T.
void fim(int relogio, struct mundo_t *mundo, struct fprio_t *LEF) {
  if (!LEF)
    return;
    
  printf("%d: FIM\n\n", relogio);

  //imprime as estatisitcas da simulação:

  //imprime herois
  for (int i = 0; i < mundo->nHerois; i++) {
    if (mundo->herois[i]->vivo) {
      printf("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS [ ", mundo->herois[i]->ID, mundo->herois[i]->paciencia, mundo->herois[i]->velocidade, mundo->herois[i]->experiencia);
      cjto_imprime(mundo->herois[i]->habilidades);
      printf("]\n");
    }
    else {
      printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ ", mundo->herois[i]->ID, mundo->herois[i]->paciencia, mundo->herois[i]->velocidade, mundo->herois[i]->experiencia);
      cjto_imprime(mundo->herois[i]->habilidades);
      printf("]\n");
    }
  }
  //imprime bases
  for (int i = 0; i < mundo->nBases; i++) {
    printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", mundo->bases[i]->ID, mundo->bases[i]->lotacao, mundo->bases[i]->filaMax, mundo->bases[i]->missoesAtendidas);
  }

  //imprime informações gerais
  printf("EVENTOS TRATADOS: %d\n", mundo->eventosTratados);
  printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", mundo->missoesRealizadas, mundo->nMissoes, (mundo->missoesRealizadas / mundo->nMissoes) * 100);
  printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", mundo->minMissoes, mundo->maxMissoes, (mundo->minMissoes + mundo->maxMissoes) / 2.0);

  //calcula quantos herois estão vivos
  int heroisVivos = 0;
  for (int i = 0; i < mundo->nHerois; i++)
    if (mundo->herois[i]->vivo)
      heroisVivos++;

  printf("TAXA MORTALIDADE: %.1f%%\n", (mundo->nHerois - heroisVivos) / mundo->nHerois * 100);  
}


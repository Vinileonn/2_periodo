/*
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 20/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"


//retorna 1 se o racional é negativo e zero se positivo
int testa_negativo(struct racional *r) {

  if (!r)
    return 0;
 
  if (r->num < 0 && r->den > 0)
    return 1;
  if (r->den < 0 && r->num > 0)
    return 1;
  return 0;
}

// Maximo Divisor Comum entre a e b
long mdc (long a, long b) {                                                         
                                                                                                                      
  int temp;
	
  while (b != 0) {
    temp = b;
    b = a % b;
    a = temp;
  }
	
  return a;

}                                                

// calcula mmc entre a e b
long mmc (long a, long b) {
  return (a * b) / mdc(a, b);
}

// Simplifica o número racional indicado no parâmetro.
int simplifica_r (struct racional *r) {
  int num, den;

  if (!valido_r(r))
    return 0;
  if (!r)
    return 0;

  //simplifica o numerador e denominador
  num = r->num / mdc (r->num, r->den);
  den = r->den / mdc (r->num, r->den);
  r->num = num;
  r->den = den;
  
  if (r->num < 0 && r->den < 0) {
    r->num *= -1;
    r->den *= -1;
  }
  if (r->num > 0 && r->den < 0) {
    r->num *= -1;
    r->den *= -1;
  }
  return 1;
}

//cria um número racional com o numerador e denominador indicados
struct racional *cria_r (long numerador, long denominador) {
  struct racional *c_r;
	  
  c_r = malloc(sizeof(struct racional));
  if (!c_r)
    return NULL;

  c_r->num = numerador;
  c_r->den = denominador;
  
  return c_r;
}

// Libera a memória alocada para o racional apontado por r
void destroi_r (struct racional *r) {
  if (r)  
    free (r);
}
                                                                                                                      
// Retorna 1 se o racional r for válido ou 0 se for inválido.                                             
int valido_r (struct racional *r) {
  if (!r || r->den == 0)
    return 0;
  return 1;
}
                                                                                                                      
// Imprime um racional r, respeitando estas regras:            
void imprime_r (struct racional *r) {
  
  simplifica_r(r);

  if (!r) {
    r = NULL;
    return;
  }

  if (!valido_r(r)) {
    printf("NaN");
    return;
  }
  if (r->num == 0) {
    printf("0");
    return;
  }
  if (r->den == 1) {
    printf("%ld", r->num);
    return;
  }
  if (r->den == r->num) {
    printf("1");
    return;
  }

  if (testa_negativo(r)) {
    printf("%ld/%ld", r->num, r->den);
    return;
  }
  if (r->num < 0 && r->den < 0) {
    printf("%ld/%ld", r->num * -1, r->den * -1);
  }
  printf("%ld/%ld", r->num, r->den);
}
                                                                                                                      
// Compara dois números racionais r1 e r2.                                                        
int compara_r (struct racional *r1, struct racional *r2) {
  long mmc_den, num1_normalizado, num2_normalizado;

  if (!r1 || !valido_r(r1))
    return -2;
  if(!r2 || !valido_r(r2))
    return -2;
  
  mmc_den = mmc(r1->den, r2->den);
  num1_normalizado = r1->num * (mmc_den / r1->den);
  num2_normalizado = r2->num * (mmc_den / r2->den);

  if (num1_normalizado < num2_normalizado)
    return -1;
  if (num1_normalizado > num2_normalizado)  
    return 1;
  //r1 = r2
  return 0; 
}
                                                                                                                      
// Coloca em *r3 a soma simplificada dos racionais *r1 e *r2.                
int soma_r (struct racional *r1, struct racional *r2, struct racional *r3) {
  if (!r1 || !valido_r(r1))
    return 0;
  if(!r2 || !valido_r(r2))
    return 0;
  if(!r3)
    return 0;

  //calcula a soma                                 
  r3->den = mmc(r1->den, r2->den);
  r3->num = (r1->num * (r3->den / r1->den)) + (r2->num * (r3->den / r2->den));

  return 1;
}

// Coloca em *r3 a diferença simplificada dos racionais *r1 e *r2.
int subtrai_r (struct racional *r1, struct racional *r2, struct racional *r3) {
  if (!r1 || !valido_r(r1))
    return 0; 
  if(!r2 || !valido_r(r2))
    return 0; 
  if(!r3)
    return 0;
        
  //calcula a subtração                                 
  r3->den = mmc(r1->den, r2->den); 
  r3->num = (r1->num * (r3->den / r1->den)) - (r2->num * (r3->den / r2->den));  

  return 1;
}

// Coloca em *r3 o produto simplificado dos racionais *r1 e *r2.
int multiplica_r (struct racional *r1, struct racional *r2, struct racional *r3) {
  if (!r1 || !valido_r(r1))
    return 0; 
  if(!r2 || !valido_r(r2))
    return 0; 
  if(!r3)
    return 0;

  //calcula o produto
  r3->num = r1->num * r2->num;
  r3->den = r1->den * r2->den;

  return 1;
}

// Coloca em *r3 a divisão simplificada do racional *r1 por *r2.
int divide_r (struct racional *r1, struct racional *r2, struct racional *r3) {
  if (!r1 || !valido_r(r1))
    return 0;
  if(!r2 || !valido_r(r2))
    return 0; 
  if(!r3)
    return 0;

  //calcula divisão
  r3->num = r1->num * r2->den;
  r3->den = r1->den * r2->num;

  if (r3 -> den == 0)
    return 0;
  return 1;
}


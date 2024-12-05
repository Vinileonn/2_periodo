/*
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 20/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/


#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

// retorna um número aleatório entre min e max, inclusive.
long aleat (long min, long max)
{
	return (rand () % (max - min + 1) + min) ;
}


// calcula o MDC pelo método de Euclides
long mdc (long a, long b)
{
	
	int temp;
	
	while (b != 0) {
		temp = b;
		b = a % b;
		a = temp;
	}
	
	return a;

}

// calcula o mínimo Múltiplo Comum entre a e b
long mmc (long a, long b)
{
	return (a * b) / mdc(a, b);
}

// Recebe um número racional e o simplifica.
struct racional simplifica_r (struct racional r)
{
	
	struct racional s_r;

	if (!valido_r(r))
		return r;
	//simplifica o numerador
	s_r.num = r.num / mdc (r.num, r.den);
	//simplifica o denominador
	s_r.den = r.den / mdc (r.num, r.den);
	
	return s_r;

}

// Cria um número racional com o numerador e denominador indicados.
struct racional cria_r (long numerador, long denominador) {
	
	struct racional cria_r;
	
	cria_r.num = numerador;
	cria_r.den = denominador;
	
	return cria_r;
}

// Retorna 1 se o racional r for válido ou 0 se for inválido.
int valido_r (struct racional r) {	
	
	if (r.den == 0)
		return 0;
	return 1;
}

// Retorna um número racional aleatório na forma simplificada.
struct racional sorteia_r (long min, long max) {

	struct racional sorteia_r;

	sorteia_r.num = aleat (min, max);
	sorteia_r.den = aleat (min, max);

	return simplifica_r (sorteia_r);
}

// Imprime um racional r na forma simplificada
void imprime_r (struct racional r) {
	
	long resultado;

	r = simplifica_r(r);
	
	if (!valido_r (r))
		printf("NaN");	
	
	else if (r.num == 0)
		printf("0");
	
	else if (r.den == 1)
		printf("%ld", r.num);
	
	else if (r.den == r.num)
		printf("1");
	//coloca o sinal de negativo na frente de racionais negativos	
	else if ((r.num * r.den) < 0) {
		if (r.num < 0)
			r.num = r.num*-1;
		if (r.den < 0)
			r.den = r.den*-1;
		printf("-%ld/%ld", r.num, r.den);
	}
	// transforma fração de dois numeros negativos em uma fração positiva
	else if (r.num < 0 && r.den < 0){
		r.num = r.num*-1;
		r.den = r.den*-1;
		printf("%ld/%ld", r.num, r.den);
	}
	// calcula e imprime resultados inteiros
	else if ((r.num % r.den) == 0) {
		resultado = r.num / r.den;
		printf("%ld", resultado);
	}

	else
		printf("%ld/%ld", r.num, r.den);

	return;
}


// Compara dois racionais r1 e r2
int compara_r (struct racional r1, struct racional r2) {
	
	if (!valido_r(r1) || !valido_r (r2))
		return -2;
	
	if ((r1.num * 1.00) / r1.den < (r2.num * 1.00) / r2.den)
		return -1;
	
	if ( (r1.num * 1.00) / r1.den == (r2.num * 1.00) / r2.den)
                return 0;
	
	// r1 > r2
	return 1;
}



// Retorna a soma dos racionais r1 e r2 no parametro *r3.
int soma_r (struct racional r1, struct racional r2, struct racional *r3) {
                                                                                                
	if (!valido_r(r1) || !valido_r(r2))                                                                           
        	return 0;                                                                                         
	if (r3 == NULL)                                                                    
		return 0;
        
        //calcula a soma                                 
        r3 -> den = mmc(r1.den, r2.den);
	r3 -> num = (r1.num * (r3 -> den / r1.den)) + (r2.num * (r3 -> den / r2.den));

        simplifica_r(*r3);                                                                                                
        
        return 1;   
}

// Retorna a subtracao dos racionais r1 e r2 no parametro *r3.
int subtrai_r (struct racional r1, struct racional r2, struct racional *r3) {   
	                                                      
        if (!valido_r(r1) || !valido_r(r2))
		return 0;
	if (r3 == NULL)
		return 0;
        
        //calcula a subtracao                                 
        r3 -> den = mmc(r1.den, r2.den);                                                               
        r3 -> num = (r1.num * (r3 -> den / r1.den)) - (r2.num * (r3 -> den / r2.den));

        simplifica_r(*r3);                                                                                                
        return 1;   

}

// Retorna a multiplicacao dos racionais r1 e r2 no parametro *r3.
int multiplica_r (struct racional r1, struct racional r2, struct racional *r3) {

        
        if (!valido_r(r1) || !valido_r(r2))
                return 0;	
	if (r3 == NULL)
		return 0;
	
        r3 -> num = r1.num * r2.num;
        r3 -> den = r1.den * r2.den;
        
	return 1;

}

// Retorna a divisao dos racionais r1 e r2 no parametro *r3.
int divide_r (struct racional r1, struct racional r2, struct racional *r3) {

        if (!valido_r(r1) || !valido_r(r2))
                return 0;
        if (r3 == NULL)
                return 0;

	r3 -> num = r1.num * r2.den;
        r3 -> den = r1.den * r2.num;

        if (r3 -> den == 0)
                return 0;

        return 1;

}


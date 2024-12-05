/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"
/*
 * Implemente aqui as funcoes definidas no racionais.h; caso precise,
 * pode definir aqui funcoes auxiliares adicionais, que devem ser usadas
 * somente neste arquivo.
*/

/* retorna um número aleatório entre min e max, inclusive. */
long aleat (long min, long max) {
	return (rand () % (max - min + 1) + min) ;
}


// calcula o MDC entre a e b
long mdc (long a, long b) {
	int temp;
	while (b != 0) {
		temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

// calcula o MMC entre a e b
long mmc (long a, long b) {
	return (a * b) / mdc(a, b);
}

// Recebe um número racional e o simplifica
struct racional simplifica_r (struct racional r) {
	struct racional s_r;
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
	// testa se o racional eh invalido
	if (!valido_r (r))
		printf("INVALIDO ");	
	// testa se o resultado imediato da fracao eh 0
	else if (r.num == 0)
		printf("0 ");
	// testa se o resultado imediato da fracao eh o numerador
	else if (r.den == 1)
		printf("%ld ", r.num);
	// testa se o resultado imediato da fracao eh 1
	else if (r.den == r.num)
		printf("1 ");
	// coloca sinal de negativo na frente do numero
	else if ((r.num * r.den) < 0) {
		if (r.num < 0)
			r.num = r.num*-1;
		if (r.den < 0)
			r.den = r.den*-1;
		printf("-%ld/%ld ", r.num, r.den);
	}
	// transforma fracoes de dois numeros negativos em uma fracao positiva
	else if (r.num < 0 && r.den < 0){
		r.num = r.num*-1;
		r.den = r.den*-1;
		printf("%ld/%ld ", r.num, r.den);
	}
	// calcula e imprime resultados inteiros
	else if ((r.num % r.den) == 0) {
		resultado = r.num / r.den;
		printf("%ld ", resultado);
	}
	// imprime um racional
	else
		printf("%ld/%ld ", r.num, r.den);

	return;
}

// Retorna a soma dos racionais r1 e r2.
struct racional soma_r (struct racional r1, struct racional r2) {

	struct racional soma_r;
	int num, den, div;

	//testa se r1 ou r2 eh valido e se nao retorna r1
	if (!valido_r(r1) || !valido_r(r2))
		return r1;

	den = mmc(r1.den, r2.den);
	num = (r1.num * (den / r1.den)) + (r2.num * (den / r2.den));
	div = mdc(num, den);
	num /= div;
	den /= div;
	soma_r.num = num;
	soma_r.den = den;
	return soma_r;


// Retorna a subtração dos racionais r1 e r2.
struct racional subtrai_r (struct racional r1, struct racional r2) {
	
	struct racional subtrai_r;
        int num, den, div;
                
        //testa se r1 ou r2 eh valido e se nao retorna r1
        if (!valido_r(r1) || !valido_r(r2))
                return r1;

        den = mmc(r1.den, r2.den);
        num = (r1.num * (den / r1.den)) - (r2.num * (den / r2.den));
        div = mdc(num, den);
        num /= div;
        den /= div;
        subtrai_r.num = num;
        subtrai_r.den = den;
        return subtrai_r;
}


// Retorna a multiplicação dos racionais r1 e r2.
struct racional multiplica_r (struct racional r1, struct racional r2) {
	
	struct racional multiplica_r;

        //testa se r1 ou r2 eh valido e se nao retorna r1
        if (!valido_r(r1) || !valido_r(r2))
                return r1;

	multiplica_r.num = r1.num * r2.num;
	multiplica_r.den = r1.den * r2.den;
	return multiplica_r;
}

// Retorna a divisão dos racionais r1 e r2.
struct racional divide_r (struct racional r1, struct racional r2) {
	
	struct racional divide_r;
                
        //testa se r1 ou r2 eh valido e se nao retorna r1
        if (!valido_r(r1) || !valido_r(r2))
                return r1;

	divide_r.num = r1.num * r2.den;
	divide_r.den = r1.den * r2.num;
	if (divide_r.den == 0) {
		r1.num = 1;
		r1.den = 0;
		return r1; 
	}
	return divide_r;

}

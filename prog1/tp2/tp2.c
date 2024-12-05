/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 24/09/2024 para a disciplina CI1001 - Programação 1.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

#define maxVet 100
/* coloque aqui as funções auxiliares que precisar neste arquivo */

//imprime um vetor de racionais
void imprime_vetor(struct racional v[], int n) {
	
	printf("VETOR = ");

	for (int i = 0; i < n; i++) {
		imprime_r(v[i]);
		printf(" ");
	}

	printf("\n");
}

//le um vetor de racionais
void le_vetor(struct racional v[], int n) {
	
	long num, den;

	for (int i = 0; i < n; i++) {
        	scanf("%ld", &num);
        	scanf("%ld", &den);

        	v[i] = cria_r(num, den);
	}
}

//elimina racionais invalidos de um vetor
void elimina_NaN(struct racional v[], int *n) {
	
	for (int i = 0; i < *n; ) {
		if (!valido_r(v[i])) {
            		v[i] = v[*n - 1];
			(*n)--;
        	} 
		else
            		i++;
    	}
}

//ordena um vetor de forma crescente
void ordena_vetor(struct racional v[], int n) {
	
	struct racional aux;
    
	for (int i = 0; i < n - 1; i++)  
		for (int j = i + 1; j < n; j++) 
			if (compara_r(v[i], v[j]) == 1) {
                		aux = v[i];
				v[i] = v[j];
				v[j] = aux;
            		}
}

//soma todos os racionais válidos de um vetor de racionais
struct racional soma_do_vetor(struct racional v[], int n) {
    
	struct racional soma;
	struct racional resultado;

	soma = cria_r(0, 1);

	for (int i = 0; i < n; i++) {
		if (soma_r(soma, v[i], &resultado))
        		soma = resultado;
	}

	return soma;
}

/* programa principal */
int main() {
	
	struct racional v[maxVet];
	int n;

	scanf("%d", &n);

    	le_vetor(v, n);
    	imprime_vetor(v, n);

    	elimina_NaN(v, &n);
    	imprime_vetor(v, n);

    	ordena_vetor(v, n);
    	imprime_vetor(v, n);

    	printf("SOMA = ");
    	imprime_r(soma_do_vetor(v, n));
    	printf("\n");

    	return 0;
}


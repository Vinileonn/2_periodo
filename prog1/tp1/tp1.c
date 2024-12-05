/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* programa principal */
int main ()
{
	int n, max;
	struct racional r1, r2, soma, subtracao, multiplicacao, divisao;
	// inicia seed
	srand (0);
	//le n e max
	scanf("%d", &n);
	scanf("%d", &max);
	
	//imprime resultados
	for (int i = 1; i <= n; i++) {
		printf("%d: ", i);
		r1 = sorteia_r (-max, max);
		r2 = sorteia_r (-max, max);
		imprime_r (r1);
		imprime_r (r2);
		// testa e imprime se os numeros sao invalidos
		if (!valido_r(r1) || !valido_r(r2)) {
			printf("NUMERO INVALIDO\n");
			return 1;
		}
		else {
			// realiza os calculos dos numeros racionais
			soma = soma_r(r1, r2);
			subtracao = subtrai_r(r1, r2);
			multiplicacao = multiplica_r(r1, r2);
			divisao = divide_r(r1, r2);
			// testa e imprime se a divisao eh invalida
			if (!valido_r(divisao)){
				printf("DIVISAO INVALIDA\n");
				return 1;
			}
			// imprime numeros racionais validos
			else {
				imprime_r (soma);
				imprime_r (subtracao);
				imprime_r (multiplicacao);
				imprime_r (divisao);
				printf("\n");
			}
		}
	}
	return 0;
}

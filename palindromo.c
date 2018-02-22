#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  0 == 0
#define FALSE !TRUE

typedef int bool;
/*
	Funcion que imprime el substring del string 'str', que va
	desde la posicion 'inf' hasta la posicion 'sup'.
*/
void printSubStr( char* str, int inf, int sup){
	int i;
	for(i = inf; i <= sup; ++i)
		printf("%c", str[i]);
	printf(", ");
}

void subPalindromos(char *str){
	int n = strlen(str); // Tamaño del string.

	// Variables de iteracion
	int i,j,k;

	// tabla[i][j] sera falso si str[i..j] no es palindromo.
	// Caso contrario, tabla[i][j] sera verdad.
	bool tabla[n][n];

	// Inicializamos todos los espacios del arreglo con puros 0,
	// equivalentes a false.
	memset(tabla, 0, sizeof(tabla));

	// Todos los substring de tamaño 1 son Palindromos.
	// Se inicializan con verdad para que el algoritmo funcione correctamente.
	for (i = 0; i < n; ++i)
		tabla[i][i] = TRUE;

	// Iteramos deacuerdo al tamaño de cada substring (k).
	for (k = 3; k <= n; ++k){
		// i es el indice inicla del substring.
		for (i = 0; i < n-k+1 ; ++i){
			// Obtenemos el final del substring
			j = i + k - 1;

			// Chequeamos si es un palindromo.
			if (tabla[i+1][j-1] && str[i] == str[j]){
				tabla[i][j] = TRUE;
				
	            printSubStr(str, i, j);
			}
		}
	}
}

// Main para probar la funcion
int main(){
	char str[] = "abbabba";
	subPalindromos(str);
	printf("\n");

	exit(0);
}

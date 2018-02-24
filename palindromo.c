#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  0 == 0
#define FALSE !TRUE

typedef int bool;
/*
	Funcion que imprime el arreglo de strings 'arreglo' 
	de tamaño 'tam' separando cada elemento por comas
	y liberando el espacio utilizado.
*/
void printArray(char* arreglo[], int tam){
	int i;
	for(i = 0; i < tam; ++i){
		if (i != tam-1) printf("%s, ", arreglo[i]);
		else printf("%s\n", arreglo[i]);
		free(arreglo[i]);
	}
}

/*
	Funcion que detecta y almacena en el arreglo 'palindromos' 
	(el cual ya posee 'nPalindromos' almacenados), los substring
	que sean Palindromos del string 'str'. Retornara la cantidad de 
	palindromos encontrados.
*/
int subPalindromos(char* str, char* palindromos[], int nPalindromos){
	int n = strlen(str); // Tamaño del string.

	// Variables de iteracion
	int i, j ,k, posArreglo = nPalindromos;

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

	// Detecta todos los substring de tamaño 2 que se repitan las letras, 
	// ej: 'aa', 'bb'.
    for (i = 0; i < n-1; ++i)
        if (str[i] == str[i+1])
            tabla[i][i+1] = TRUE;
    

	// Iteramos deacuerdo al tamaño de cada substring (k).
	for (k = 3; k <= n; ++k){
		// i es el indice inicla del substring.
		for (i = 0; i < n-k+1 ; ++i){
			// Obtenemos el final del substring
			j = i + k - 1;

			// Chequeamos si es un palindromo.
			if (tabla[i+1][j-1] && str[i] == str[j]){
				tabla[i][j] = TRUE;
				
				char * aux = malloc(sizeof(char)*(j-i)+1);

				int c1 = 0, c2;
				for(c2 = i; c2 <= j; c2++){
					aux[c1] = str[c2];
					c1++;
				}
				aux[c1] = '\0';

				int l;
				bool esta = FALSE;
				for(l = 0; l < posArreglo; l++){
					if (strcmp(palindromos[l], aux) == 0){
						esta = TRUE;
						break;
					}
				}

				if(!esta){
					palindromos[posArreglo] = aux;
					posArreglo++;
				}
			}
		}
	}

	return posArreglo;
}



// Main para probar la funcion
int main(){
	char str[] = "ababa";
	char* palindromos[10000]; 
	int nPalindromos = 0;
	int nPalindromos = subPalindromos(str, palindromos, nPalindromos);

	printArray(palindromos, nPalindromos);

	exit(0);
}

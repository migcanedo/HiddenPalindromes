#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
	Funcion que dado un caracter, retorna TRUE si el 
	caracter es una letra bien sea Mayuscula ('A'..'Z') o 
	Minuscula ('a'..'z'). En caso contrario, retornara FALSE.
*/
bool esLetra(char c){
	if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
		return TRUE;
	return FALSE;
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
	// equivalentes a falso.
	memset(tabla, 0, sizeof(tabla));

	// Todos los substring de tamaño 1 son Palindromos.
	// Se inicializan con verdad para que el algoritmo funcione correctamente.
	for (i = 0; i < n; ++i){
		tabla[i][i] = TRUE;

		// Si el caracter es acento, ocupa el doble de espacio, y
		// por tanto se inicializa todo su espacio ocupado como verdad.
		if (str[i] == -61) {
			tabla[i][i+1] = TRUE;
			tabla[i+1][i] = TRUE;
		}
	}

	// Detecta todos los substring de tamaño 2 que se repitan las letras, 
	// ej: 'aa', 'bb', 'aA', 'Bb'.
    for (i = 0; i < n-1; ++i)
        if (str[i] == str[i+1] || 
        		(esLetra(str[i]) &&	(str[i] - str[i+1] == 32 || str[i+1] - str[i] == 32)))
            tabla[i][i+1] = TRUE;


	// Iteramos deacuerdo al tamaño de cada substring (k).
	for (k = 3; k <= n; ++k){
		// i es el indice inicla del substring.
		for (i = 0; i < n-k+1 ; ++i){	
			// Obtenemos el final del substring.
			j = i + k - 1;
			
			// Ignora los espacios intermedios para evaluar si son palindromos.
			int auxi = i, auxj = j;
			int especialesIguales = 0;
			if (str[auxi] == -61 && str[auxi] == str[auxj-1] && 
				(str[auxi+1] == str[auxj] || (str[auxi+1] - str[auxj] == 32 || str[auxj] - str[auxi+1] == 32) ) ) {
				auxi++; auxj--;
				especialesIguales = 1;
			}
			
			// Chequeamos si es un palindromo, sin discriminar mayusculas de minusculas, ej: 'a' == 'A'.
			if (tabla[auxi+1][auxj-1] && 
					((str[i] > 0  && str[i] == str[j]) || especialesIguales ||
						( esLetra(str[i]) && (str[i] - str[j] == 32 || str[j] - str[i] == 32)))) {

				tabla[i][j] = TRUE;
				
				// Copiamos el Substring en el apuntador 'aux'.	
				char * aux = malloc(sizeof(char)*(j-i)+1);
				int c1 = 0, c2;
				for(c2 = i; c2 <= j; c2++){
					aux[c1] = str[c2];
					c1++;
				}
				aux[c1] = '\0';
				
				// Luego comparamos que el Subpalindromo ya no este reportado como conseguido.
				int l;
				bool esta = FALSE;
				for(l = 0; l < posArreglo; l++){
					if (strcmp(palindromos[l], aux) == 0){
						esta = TRUE;
						break;
					}
				}
				// Si no lo esta, lo agregamos al arreglo.
				if(!esta){
					palindromos[posArreglo] = strdup(aux);
					posArreglo++;
				}

				free(aux);
			}
		}
	}

	return posArreglo;
}



// Main para probar la funcion
int main(){
	char str[] = "$a";
	printf("%d\n", strlen(str));
	char* palindromos[10000]; 
	int nPalindromos = 0;
	nPalindromos += subPalindromos(str, palindromos, nPalindromos);

	printArray(palindromos, nPalindromos);

	exit(0);
}

//---------------------------------------------------------
//  HIDDEN PALINDROMES
//
//  Fecha de elaboracion: 28/02/2018
//
//  Autores:
//           Jose Donato Bracuto Delgado 13-10173
//           Miguel Clemente Canedo Rodriguez 13-10214
//---------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "buscaPalindromos.h"


/*
	Funcion principal donde se correra el programa.
*/
int main(int argc, char* argv[]){
	// Obtengo el path.
	char path[100000];
	getcwd(path, sizeof(path)); 

	int maxProf = -1;	// -1 := Que debe recorrer todo hasta no poder mas.
	int tomarArchivos = 0;	// Indicara si los Archivos se tomaran en cuenta para detectar los palindromos.

	// Detectamos si se especifico algun flag para correr el programa.
	int opt;
	while((opt = getopt(argc, argv, "fm:d:")) != -1){
		switch(opt){
			case 'f': 					// Especifica si se toman en cuenta los archivos.
					tomarArchivos = 1;
					break;
			case 'm':					// Especifica si tendra una profundidad especifica el arbol.
					maxProf = atoi(optarg);
					break;
			case 'd':					// Especifica desde que directorio empezar a armar el arbol.
					strcpy(path, optarg);
					// if (path[strlen(path)-1] == '/') path[strlen(path)-1] = '\0';
					break;
			default:					// Si se especifica un flag no valido.
					printf("Error. Flags no validos.\n");
					exit(0);
		}
	}

	buscaPalindromos(tomarArchivos, maxProf, path);
	exit(0);
}


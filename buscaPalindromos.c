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
#include "buscaPalindromos.h"


int maxProf; 				// Profundidad maxima del arbol de directorios, sera -1 si no se especifica alguna.
int p[2]; 					// Pipe por donde se comunicaran los procesos.
int tomarArchivos;			// Indicara si se deben tomar en cuanta los archivos.
sem_t *leer, *escribir;		// Semaforos para saber cuando se puede leer y escribir, respectivamente, del pipe.
char * pathOrigen;			// Path donde se empieza a correr el programa.

/*
	Funcion que recorre de manera recursiva los directorios y archivos hasta una profundidad establecida.
	Si la profundidad dada es -1, se recorren todos los directorios.
*/
void recorrer(char* path, int profundidad){
	int tieneDir = 0; // Indica si el directorio ha abrir tiene directorios dentro.
	
	char *pathReal = (char*) concat(pathOrigen, path); 
	DIR* srcdir = opendir(pathReal); // Abrimos el directorio indicado en el path.
	struct dirent* actual; 

	if (srcdir == NULL) {
		if(errno == EACCES) goto sinPermisos; 	// Si no se pudo abrir el directorio por Falta de persmisos, se ignora su contenido.
		perror("opendir");
		exit(-1);
	}
		
	while ((actual = readdir(srcdir)) != 0){
		struct stat st; 	// Inicializamos la estrutura que tendra la informacionde los archivos y directorios.

		// Ignoro los directorios '.'(Referencia al directorio) y '..'(referencia a directorio padre).
		if(strcmp(actual->d_name, ".") == 0 || strcmp(actual->d_name, "..") == 0 )
			continue;

		// Revisa si tengo permisos para leer el directorio. Si no los tiene, simplemente lo ignora y sigue
		if (fstatat(dirfd(srcdir), actual->d_name, &st, 0) < 0) 
			continue;
	

		// Revisar los archivos si el programa pide agregarlos al path.
		if (S_ISREG(st.st_mode) && tomarArchivos){
			tieneDir = 1;
			char* nuevoPath = (char*) concat(path, actual->d_name);
			
			// Comunicamos el path al proceso padre mediante el pipe.
			sem_wait(escribir);
			if (write(p[1], nuevoPath, 100000) < 0)
				sem_post(escribir);
			sem_post(leer); 				// Indicamos que ya se peude leer el pipe.
		}

		//Reviso si es un directorio y lo reviso recursivamente.
		else if (S_ISDIR(st.st_mode) && profundidad < maxProf){
			tieneDir = 1; 	// Indicamos que el directorio posee direcctorios, por tanto no es una hoja.

			char* nuevoPath = (char*) concat(path, actual->d_name);			
			
			int aux = profundidad+1;
			recorrer(nuevoPath, aux);
		}
	}
	
	sinPermisos: 	// Etiqueta donde se enviara cuando no se tenga permisos para leer el directorio.
	if(!tieneDir && profundidad){
		// Comunicamos el path al proceso padre mediante el pipe.
		sem_wait(escribir); 		// Esperamos que podamos escribir. 
		write(p[1], path, 100000);
		sem_post(leer); 			// Indicamos que ya se peude leer el pipe.
	}
	closedir(srcdir);
}

/*
	Funcion que utilizando dos procesos, uno para recorrer los directorios recursivamente y 
	otro para evaluar los path, encontrara todos los palindromos ocultos en cada uno de los path.
*/
void buscaPalindromos(int tArch, int maxP, char *pth){
	pathOrigen = pth; 	// Actualizamos el path del directorio donde se empezara a correr el programa.

	tomarArchivos = tArch;
	maxProf = maxP;
	
	// Inicializamos el pipe por donde se comunicaran los procesos.
	pipe(p);

	// Abrimos los Semaforos Nombrados que usaran los procesos para sincronizarse.
	leer = sem_open("semL", (O_CREAT|O_EXCL), 1, 0);
	escribir = sem_open("semE", (O_CREAT|O_EXCL), 1, 1);

	sem_unlink("semL");
	sem_unlink("semE");

	// Creamos los procesos.
	pid_t childpid = fork();
	
	// Proceso Hijo, encargado de la recursion.
	if (childpid == 0){
		// Recorro el Arbol de Directorios.		
		recorrer("", 0);
	}
	// Proceso Padre, encargado de detectar los palindromos en los path que le indique el hijo.
	else{
		char pathM[1000000]; 		// String que representa el path donde se buscaran los palindromos.
		char* palindromos[100000]; 	// Arreglo donde se guardaran los palindromos encontrados.
		int nPalindromos = 0;		// Cantidad de palindromos encontrada.


		if (maxProf){ 
			// Cerramos el lado de escritura del pipe para comenzar a leer.
			close(p[1]);
			while(read(p[0], pathM, sizeof(pathM)) > 0){
				sem_wait(leer);					// Esperamos que podamos leer.
				// Quitamos los separadores de directorios en el path ('/').
				quitarSeparador(pathM);			
				// Buscamos los subpalindromos del path, y actualizamos la cantidad de Palindromos que hay.
				nPalindromos = subPalindromos(pathM, palindromos, nPalindromos);
				sem_post(escribir);				// Indicamos que se puede escribir.			
			}
		}
		wait(NULL);		// Esperamos que el hijo termine
		
		// Imprimimos el arreglo de todos los Palindromos encontrados.
		printf("\n\n");
		if (nPalindromos) 
			printArray(palindromos, nPalindromos);
		else
			printf("No Existen.\n");
		printf("\n\n");
	}
}
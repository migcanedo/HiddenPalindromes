#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>

int maxProf; 				// Profundidad maxima del arbol de directorios, sera -1 si no se especifica alguna.
int p[2]; 					// Pipe por donde se comunicaran los procesos.
int tomarArchivos;			// Indicara si se deben tomar en cuanta los archivos.
sem_t *leer, *escribir;		// Semaforos para saber cuando se puede leer y escribir, respectivamente, del pipe.

void recorrer(char* path, int profundidad){
	int tieneDir = 0; // Indica si el directorio ha abrir tiene directorios dentro.
		
	DIR* srcdir = opendir(path); // Abrimos el directorio indicado en el path.
	struct dirent* actual; 

	if (srcdir == NULL) {
		if(errno == EACCES) return; 	// Si no se pudo abrir el directorio por Falta de persmisos, se ignora sin error.
		perror("opendir");
		exit(-1);
	}
		
	while ((actual = readdir(srcdir)) != 0){
		struct stat st; 	// Inicializamos la estrutura que tendra la informacionde los archivos y directorios.

		// Ignoro los directorios '.'(Referencia al directorio) y '..'(referencia a directorio padre).
		if(strcmp(actual->d_name, ".") == 0 || strcmp(actual->d_name, "..") == 0 || strcmp(actual->d_name, ".git") == 0 )
			continue;

		if (fstatat(dirfd(srcdir), actual->d_name, &st, 0) < 0) {
			perror(actual->d_name);
			continue;
		}

		// Revisar los archivos si el programa pide agregarlos al path.
		if (S_ISREG(st.st_mode) && tomarArchivos){
			char* nuevoPath = (char*) concat(path, actual->d_name);

			// Comunicamos el path al proceso padre mediante el pipe.
			sem_wait(escribir); 		// Esperamos que podamos escribir. 
			write(p[1], nuevoPath, 10000);
			sem_post(leer); 			// Indicamos que ya se peude leer el pipe.
		}
		//Reviso si es un directorio y lo reviso recursivamente.
		else if (S_ISDIR(st.st_mode) && (profundidad == -1 || profundidad < maxProf)){
			tieneDir = 1; 	// Indicamos que el directorio posee direcctorios, por tanto no es una hoja.

			char* nuevoPath = (char*) concat(path, actual->d_name);			

			int aux = (profundidad != -1) ? profundidad + 1 : profundidad;
			recorrer(nuevoPath, aux);
		}
	}

	if(!tieneDir){
		// Comunicamos el path al proceso padre mediante el pipe.
		sem_wait(escribir); 		// Esperamos que podamos escribir. 
		write(p[1], path, 10000);
		sem_post(leer); 			// Indicamos que ya se peude leer el pipe.
	}
}


int main(int argc, char* argv[]){
	// Obtengo el path.
	char path[100000];
	getcwd(path, sizeof(path)); 

	maxProf = -1;	// -1 := Que debe recorrer todo hasta no poder mas.
	tomarArchivos = 0;	// Indicara si los Archivos se tomaran en cuenta para detectar los palindromos.

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
					break;
			default:					// Si se especifica un flag no valido.
					printf("Error. Flags no validos.\n");
					exit(0);
		}
	}

	// // Copio el path y pruebo el eliminar los '/'
	// char* pathM = (char*) malloc(strlen(path)*sizeof(char));
	// strcpy(pathM, path);
	// quitarSeparador(pathM);
	// printf("Path Sin '/': %s\n\n", pathM);
	
	pipe(p);

	leer = sem_open("semL", (O_CREAT|O_EXCL), 0, 0);
	escribir = sem_open("semE", (O_CREAT|O_EXCL), 0, 1);

	sem_unlink("semL");
	sem_unlink("semE");

	pid_t childpid = fork();
	
	// Proceso Hijo, encargado de la recursion.
	if (childpid == 0){
		// Recorro el Arbol de Directorios.
		if (maxProf != -1) recorrer(path, 0);
		else recorrer(path, maxProf);
	}
	// Proceso Padre, encargado de detectar los palindromos en los path que le indique el hijo.
	else{
		char pathM[10000];

		// Cerramos el lado de escritura del pipe para comenzar a leer.
		close(p[1]);
		while(read(p[0], pathM, sizeof(pathM))){
			sem_wait(leer);					// Esperamos que podamos leer. 
			// Aqui se resolvera todo lo que tenga qu ver con los palindromos.
			printf("Path: %s\n", pathM);
			sem_post(escribir);				// Indicamos que se puede escribir.
		}
		wait(NULL);		// Esperamos que el hijo termine
	}

	exit(0);
}


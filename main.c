#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

int maxProf;
int p[2]; 	// Pipe por donde se comunicaran los procesos.
int tomarArchivos;

void recorrer(char* path, int profundidad){
	int tieneDir = 0; // Indica si el directorio ha abrir tiene directorios dentro.
	DIR* srcdir = opendir(path);
	struct dirent* actual;

	if (srcdir == NULL) {
		if(errno == EACCES) return; 	// Si no se pudo abrir el directorio por Falta de persmisos, se ignora sin error.
		perror("opendir");
		exit(-1);
	}
		
	while ((actual = readdir(srcdir)) != 0){
		struct stat st;

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
			printf("Archivo: %s\n", nuevoPath);
		}
		//Reviso si es un directorio y lo reviso recursivamente.
		else if (S_ISDIR(st.st_mode) && (profundidad == -1 || profundidad < maxProf)){
			tieneDir = 1;
			char* nuevoPath = (char*) concat(path, actual->d_name);
			// Se podria agregar un opendir aqui, de tal forma de revisar los permisos de lectura
			// antes de abrir una pila de recursion.
			int aux = (profundidad != -1) ? profundidad + 1 : profundidad;
			recorrer(nuevoPath, aux);
		}
	}

	if(!tieneDir){
		// Aqui se correria el algoritmo de encontrar los Palindromos con 'path'.
		printf("Directorio: %s\n", path);

		//Cerramos el lado de lectura del pipe para comenzar a escribir
        // close(p[0]);
        write(p[1], path, strlen(path));
	}
}


int main(int argc, char* argv[]){
	// Obtengo el path.
	char path[100000];
    getcwd(path, sizeof(path)); 
    maxProf = -1;
    tomarArchivos = 0;

    int opt;
    while((opt = getopt(argc, argv, "fm:d:")) != -1){
    	switch(opt){
    		case 'f': 
    				tomarArchivos = 1;
    				break;
    		case 'm':
    				maxProf = atoi(optarg);
    				break;
    		case 'd':
    				strcpy(path, optarg);
    				break;
    		default:
    				printf("Error. Flags no validos.\n");
    				exit(0);
    	}
    }
    
	// char* path = (char *) get_current_dir_name();

	printf("\nPath Inicial: %s\n\n", path);

	// // Copio el path y pruebo el eliminar los '/'
	// char* pathM = (char*) malloc(strlen(path)*sizeof(char));
	// strcpy(pathM, path);
	// quitarSeparador(pathM);
	// printf("Path Sin '/': %s\n\n", pathM);
	
	pid_t childpid = fork();
	pipe(p);

	if (childpid == 0){
		// Recorro el Arbol de Directorios.
		if (maxProf != -1) recorrer(path, 0);
		else recorrer(path, maxProf);
	}else{
		wait(NULL);
		char pathM[100000];

		// Cerramos el lado de escritura del pipe para comenzar a leer.
        // close(p[1]);
     //    while(read(p[0], pathM, 100000)){
	    //     printf("Directorio En Padre: %s\n", pathM);
	    // }
	}
	
	exit(0);
}


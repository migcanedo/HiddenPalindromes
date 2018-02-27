#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <strings.h>
#include <errno.h>

// #define EACCES 13      // Numero de Error de Permiso Denegado.


void recorrer(char* path){
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
		if(strcmp(actual->d_name, ".") == 0 || strcmp(actual->d_name, "..") == 0 || strcmp(actual->d_name, ".git") == 0)
			continue;

		if (fstatat(dirfd(srcdir), actual->d_name, &st, 0) < 0) {
			perror(actual->d_name);
			continue;
		}

		//Reviso si es un directorio e imprimo el nombre.
		if (S_ISDIR(st.st_mode)){
			char* nuevoPath = (char*) concat(path, actual->d_name);

			printf("Directorio: %s\n", nuevoPath);
			recorrer(nuevoPath);
		}
	}
}


int main(int argc, char* argv[]){
	// Obtengo el path.
	char* path = (char *) get_current_dir_name();

	printf("\nPath Inicial: %s\n\n", path);

	// Copio el path y prubeo el eliminar los '/'
	char* pathM = (char*) malloc(strlen(path)*sizeof(char));
	strcpy(pathM, path);
	quitarSeparador(pathM);
	printf("Path Sin '/': %s\n\n", pathM);
	
	// Recorro el Arbol de Directorios.
	recorrer(path);

	exit(0);
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <string.h>


void recorrer(char* path){
	DIR* srcdir = opendir(path);
	struct dirent* actual;

    if (srcdir == NULL) {
        perror("opendir");
        exit(-1);
    }
		
    while ((actual = readdir(srcdir)) != 0){
    	struct stat st;

    	// Ignoro los directorios '.'(Referencia a el directorio) y '..'(referencia a directorio padre).
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
    reemplazarChar(pathM, '/', ' ');
    quitarEspacios(pathM);
    printf("Path Sin '/': %s\n\n", pathM);
	
	// Recorro el Arbol de Directorios.    
	recorrer(path);

	exit(0);
}


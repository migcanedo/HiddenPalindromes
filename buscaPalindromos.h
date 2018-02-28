//---------------------------------------------------------
//  HIDDEN PALINDROMES
//
//  Fecha de elaboracion: 28/02/2018
//
//  Autores:
//           Jose Donato Bracuto Delgado 13-10173
//           Miguel Clemente Canedo Rodriguez 13-10214
//---------------------------------------------------------


#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>

//----------------------------------------------------------------------
// Funciones.
//----------------------------------------------------------------------

/*
	Funcion que recorre de manera recursiva los directorios y archivos hasta una profundidad establecida.
	Si la profundidad dada es -1, se recorren todos los directorios.
*/
void recorrer(char*, int);

/*
	Funcion que utilizando dos procesos, uno para recorrer los directorios recursivamente y 
	otro para evaluar los path, encontrara todos los palindromos ocultos en cada uno de los path.
*/
void buscaPalindromos(int tArch, int maxP, char *pth);

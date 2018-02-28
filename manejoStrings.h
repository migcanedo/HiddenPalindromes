//---------------------------------------------------------
//  HIDDEN PALINDROMES
//
//  Fecha de elaboracion: 28/02/2018
//
//  Autores:
//           Jose Donato Bracuto Delgado 13-10173
//           Miguel Clemente Canedo Rodriguez 13-10214
//---------------------------------------------------------

//----------------------------------------------------------------------
// Macros.
//----------------------------------------------------------------------

/*
	Macro utilizado para hacer mas legible el uso del booleano 'TRUE'.
*/
#define TRUE  0 == 0 

/*
	Macro utilizado para hacer mas legible el uso del booleano 'FALSE'.
*/
#define FALSE !TRUE

//----------------------------------------------------------------------
// Tipos.
//----------------------------------------------------------------------

/*
	Definicion de tipo que facilitara el uso de booleanos.
*/
typedef int bool;

//----------------------------------------------------------------------
// Funciones.
//----------------------------------------------------------------------

/*
    Funcion que concatena dos string dados, pero agregando entre ellos
    un separador. En este caso el separador es: '/'.
*/
char* concat (char* str1, char* str3);

/*
    Funcion que eliminara los separadores de un string dado.
    En este caso, el separador es: '/'.
*/
void quitarSeparador(char* str);


/*
    Funcion que imprime el arreglo de strings 'arreglo' 
    de tamaño 'tam' separando cada elemento por comas
    y liberando el espacio utilizado.
*/
void printArray(char* arreglo[], int tam);


/*
    Funcion que dado un caracter, retorna TRUE si el 
    caracter es una letra bien sea Mayuscula ('A'..'Z') o 
    Minuscula ('a'..'z'). En caso contrario, retornara FALSE.
*/
bool esLetra(char c);


/*
    Funcion que detecta y almacena en el arreglo 'palindromos' 
    (el cual ya posee 'nPalindromos' almacenados), los substring
    que sean Palindromos del string 'str'. Retornara la cantidad de 
    palindromos encontrados.
*/
int subPalindromos(char* str, char* palindromos[], int nPalindromos);
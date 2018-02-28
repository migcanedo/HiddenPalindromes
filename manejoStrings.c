//---------------------------------------------------------
//  HIDDEN PALINDROMES
//
//  Fecha de elaboracion: 28/02/2018
//
//  Autores:
//           Jose Donato Bracuto Delgado 13-10173
//           Miguel Clemente Canedo Rodriguez 13-10214
//---------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "manejoStrings.h"

/*
    Funcion que concatena dos string dados, pero agregando entre ellos
    un separador. En este caso el separador es: '/'.
*/
char* concat (char* str1, char* str3) {
    char c = '/';
    
    int len = strlen(str1);
    char* str2 = malloc(strlen(str1)+2);
    strcpy(str2, str1);
    str2[len] = c;
    str2[len + 1] = '\0';
    
    char* str4 = (char*) malloc((1+strlen(str2)+strlen(str3))*sizeof(char));
    strcpy(str4, str2);
    strcat(str4, str3);
    
    free(str2);
    return  ((char*) str4);
}

/*
    Funcion que eliminara los separadores de un string dado.
    En este caso, el separador es: '/'.
*/
void quitarSeparador(char* str){
    char* i = str;
    char* j = str;
    while(*j != 0){
        *i = *j++;
        if(*i != '/')
            i++;
    }
    *i = 0;
}

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
        // free(arreglo[i]);
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
    int i, j ,k, posArreglo = nPalindromos, especialesIguales;

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
    // ej: 'aa', 'bb', 'aA', 'Bb', 'áá', 'Áá'.
    for (i = 0; i < n-1; i++){
        // Si hay dos caracteres del Español entrara en este condicional.
        if (str[i] == -61 && str[i] == str[i+2] && 
            (str[i+1] == str[i+3] || (str[i+1] - str[i+3] == 32 || str[i+3] - str[i+1] == 32) ) ) {
            tabla[i][i+3] = TRUE;
            continue;
        }
        // En caso de que los dos caracteres no sean del español y sean iguales, entra en este condicional. 
        if ((str[i] != -61 && str[i] == str[i+1]) || 
                (esLetra(str[i]) && (str[i] - str[i+1] == 32 || str[i+1] - str[i] == 32)))
            tabla[i][i+1] = TRUE;
    }

    // Iteramos deacuerdo al tamaño de cada substring (k).
    for (k = 3; k <= n; ++k){
        // i es el indice inicial del substring.
        for (i = 0; i < n-k+1 ; ++i){   
            // Obtenemos el final del substring.
            j = i + k - 1;
            
            // Ignora los espacios intermedios para evaluar si son palindromos.
            int auxi = i, auxj = j;
            especialesIguales = 0;
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
                    if (strcmp(aux, palindromos[l]) == 0){
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


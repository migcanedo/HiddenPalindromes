#include <stdlib.h>
#include <string.h>

/*

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

void quitarEspacios(char* str){
	char* i = str;
	char* j = str;
	while(*j != 0){
		*i = *j++;
		if(*i != ' ')
			i++;
	}
	*i = 0;
}


char* reemplazarChar(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos){
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}

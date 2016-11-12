#include "definiciones.h"

FILE * in;
FILE * out;

RegTS TS[TAMTS] = { {"inicio", INICIO}, {"fin", FIN}, {"leer", LEER}, {"escribir", ESCRIBIR}, {"$", 0} }; /*tabla de simbolos inicial con solo las palabras reservadas y el centinela al final*/

char buffer[TAMLEX+1]; /*buffer para guardar identificadores y constantes*/
TOKEN tokenActual;
int flagToken = 0;

int main(int argc, char * argv[]){
	int l;
	printf("COMPILADOR DE MICRO\n\n");	
    /*Validaciones*/
	if ( argc == 1 )
	    {
	        printf("Error. Debe ingresar el nombre del archivo entrada y el nombre del archivo de salida\n");
	        return -1;
	    }
	if ( argc !=3 )
	    {
	        printf("Error. Numero incorrecto de argumentos.\n");
	        return -2;
	    }
	l = strlen(argv[1]);
	if ( argv[1][l-1] != 'm' || argv[1][l-2] != '.' )
	    {
	        printf("Error. El archivo de entrada debe finalizar con \".m\"\n");
	        return -3;
	    }
	if ( (in = fopen(argv[1], "r") ) == NULL )
	    {
	        printf("No se pudo abrir archivo de entrada\n");
	        return -4;
	    }
	if ( (out = fopen(argv[2], "w") ) == NULL)
	    {
	        printf("No se pudo abrir archivo de salida\n");
	        fclose(in);
	        return -5;
	    }

    /*Inicio proceso de compilacion con el primer PAS */
    Objetivo();
    
    /*Cierre*/
    fclose(in);
    fclose(out);
	return 0;
}

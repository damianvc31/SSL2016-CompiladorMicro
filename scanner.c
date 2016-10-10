#include <stdio.h>

#include <string.h>

#include <ctype.h>

#define NUMEROESTADOS 15

#define NUMEROCOLUMNAS 13

#define TAMLIN 120
 
FILE * in;
FILE * out;

//asigna un valor numerico a cada elemento del tipo TOKEN (inicia en 0)
typedef enum{
	INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,

	PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT,

	ERRORLEXICO

} TOKEN;

// creación de tabla de estados
int tabla[NUMEROESTADOS][NUMEROCOLUMNAS] = { 
//    L  D  +  -  (  )  ,   ;   :  =  fdt sp otro
	{ 1, 3, 5, 6, 7, 8, 9, 10, 11, 14, 13, 0, 14 },         
	{ 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 14, 14, 14 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
};
		
int columna(int c);		

int estadoFinal(int e);
	
int validacion(int argc, char * argv[]);

TOKEN scanner();

int main(int argc, char * argv[]){
	TOKEN t;
	int valid;
	if( (valid = validacion(argc,argv)) != 0 ){
		return valid;
	}
	do{
		t = scanner();
	}while(t != FDT && t != ERRORLEXICO);
	
	if(t == FDT){
		printf("Archivo leido correctamente\n");
	}
	else{
		printf("Se detecto un error, fin del programa\n");
	}
	return 0;
}

//recive un caracter y devuelve su valor numerico
int columna(int c){
	if ( isalpha(c) ) return 0; // isaplha devuelve 1 si c es un caracter
	if ( isdigit(c) ) return 1; // isdigit devuelve 1 si c es un numero
	if ( c == '+' ) return 2;
	if ( c == '-' ) return 3;
	if ( c == '(' ) return 4;
	if ( c == ')' ) return 5;
	if ( c == ',' ) return 6;
	if ( c == ';' ) return 7;
	if ( c == ':' ) return 8;
	if ( c == '=' ) return 9;
	if ( c == EOF ) return 10;
	if ( isspace(c) ) return 11;// isspace devuelve 1 si c es un espacio.
	return 12;
}
		
//recibe un caracter pasado por columna y devuelve V o F
int estadoFinal(int e){
	if ( e == 0 || e == 1 || e == 3 || e == 11 || e == 14 ) return 0; 
	return 1;
}
	

TOKEN scanner(){
	int estado = 0;
	int contador = 0; //i
	int caracter; //caracter leido
	char buffer[TAMLIN+1];
	int col; //contiene un numero del 0 al 12
	
	//cambia el valor del caracter por el de la tabla, busca si no es espacio y lo agrega al buffer
	do{

		caracter = fgetc(in); // in es el puntero del archivo de entrada, fgetc devuelve el siguiente char
		col = columna(caracter); 
		estado = tabla[estado][col]; //me muevo al siguiente estado

		//si no es el fin de la palabra(espacio), agrega al buffer el caracter que obtiene
		if ( col != 11 ) {
			buffer[contador] = caracter;
			contador++;  // corre el puntero del buffer
		}
	} while ( !estadoFinal(estado) && !(estado == 14) ); //mientras no se llegue a un estado final o al estado de rechazo
		
	// una vez terminado cierra la cadena con espacio.
	buffer[contador] = '\0';
	
	//dependiendo el estado final realizo la accion correspondiente
	
	switch ( estado ){

		case 2 : 
				//para cadenas alfanumericas
				if ( col != 11 ){ //si lo ultimo leido no fue un espacio
					// ungetc 'rebobina' el archivo un caracter
					ungetc(caracter, in); 
					// solo aplicable a archivos, cuando es solo lector deberia reemplazarse
					//trunco la cadena quitando el caracter extra leido
					buffer[contador-1] = '\0';
				}
	
				//chequeo si es palabra reservada y devuelvo el token correspondiente
				if( strcmp(buffer,"inicio") == 0 ){
					printf("Lexema detectado: INICIO\n");
					return INICIO;
				}
				if( strcmp(buffer,"fin") == 0 ){
					printf("Lexema detectado: FIN\n");
					return FIN;
				}
				if( strcmp(buffer,"leer") == 0 ){
					printf("Lexema detectado: LEER\n");
					return LEER;
				}
				if( strcmp(buffer,"escribir") == 0 ){
					printf("Lexema detectado: ESCRIBIR\n");
					return ESCRIBIR;
				}
				if( strlen(buffer) > 32 ){
					/* error por id muy largo */
				}
				else{
					/*almacenar identificador en tabla de simbolos */
					printf("Lexema detectado: ID\n");
					return ID;
				}
				break;
		case 4 : 
				//para numeros
				if ( col != 11 ) //si lo ultimo leido no fue un espacio
				{
					// ungetc 'rebobina' el archivo un caracter
					ungetc(caracter, in);
					//trunco la cadena quitando el caracter extra leido
					buffer[contador-1] = '\0';
				}
					
				/*almacenar secuencia de digitos en tabla de simbolos */				
				printf("Lexema detectado: CONSTANTE\n");
				return CONSTANTE;
				break;

		case 5 : printf("Lexema detectado: SUMA\n"); return SUMA; break;
		case 6 : printf("Lexema detectado: RESTA\n"); return RESTA; break; 
		case 7 : printf("Lexema detectado: PARENIZQUIERDO\n"); return PARENIZQUIERDO; break;
		case 8 : printf("Lexema detectado: PARENDERECHO\n"); return PARENDERECHO; break; 
		case 9 : printf("Lexema detectado: COMA\n"); return COMA; break;
		case 10 : printf("Lexema detectado: PUNTOYCOMA\n"); return PUNTOYCOMA; break;
		case 12 : printf("Lexema detectado: ASIGNACION\n"); return ASIGNACION; break;
		case 13 : printf("Lexema detectado: FDT\n"); return FDT; break;
		case 14 : printf("Lexema detectado: ERRORLEXICO\n"); return ERRORLEXICO; break;
	}
	
	return 0;
}


int validacion(int argc, char * argv[]){
	
	if ( argc == 1 ){
		printf("Debe ingresar el nombre del archivo fuente (en lenguaje Micro) en la linea de comandos\n");
		return -1;
	}

	if ( argc != 2 ){
		printf("Numero incorrecto de argumentos\n");
		return -2;
	}
	int tam = strlen(argv[1]);
	if ( argv[1][tam-1] != 'm' || argv[1][tam-2] != '.' ){
		printf("Nombre incorrecto del archivo fuente\n");
		return -3;
	}

	if ( (in = fopen(argv[1], "r") ) == NULL ){
		printf("No se pudo abrir archivo fuente\n");
		return -4;
	}
	return 0;
}

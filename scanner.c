#include "definiciones.h"
 
extern FILE * in;
extern char buffer[];

/*SCANNER*/

/*Recibe un caracter y devuelve su valor numerico*/
int columna(int c){
	if ( isalpha(c) ) return 0; /* isaplha devuelve 1 si c es un caracter*/
	if ( isdigit(c) ) return 1; /* isdigit devuelve 1 si c es un numero*/
	if ( c == '+' ) return 2;
	if ( c == '-' ) return 3;
	if ( c == '(' ) return 4;
	if ( c == ')' ) return 5;
	if ( c == ',' ) return 6;
	if ( c == ';' ) return 7;
	if ( c == ':' ) return 8;
	if ( c == '=' ) return 9;
	if ( c == EOF ) return 10;
	if ( isspace(c) ) return 11;/* isspace devuelve 1 si c es un espacio*/
	return 12;
}
		
/*recibe un estado devuelve V si es estado final o F si no lo es*/
int estadoFinal(int e){
	if ( e == 0 || e == 1 || e == 3 || e == 11 || e == 14 ) return 0; /*estados no finales*/
	return 1; /*el resto son finales*/
}
	
/*Lee caracteres desde el archivo fuente y devuelve el token proximo que encuentre*/
TOKEN scanner(){
	int tabla[NUMEROESTADOS][NUMEROCOLUMNAS] = { 
/*    L  D  +  -  (  )  ,   ;   :  =  fdt sp otro*/
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
	
	int estado = 0;
	int i = 0; /*para iterar*/
	char caracter; /*caracter leido*/
	int col; /*contiene un numero del 0 al 12*/
	
	/*obtiene el siguiente caracter del archivo, si no es espacio lo agrega al buffer*/
	do{

		caracter = fgetc(in); 
		col = columna(caracter); 
		estado = tabla[estado][col]; /*me muevo al siguiente estado*/

		if ( col != 11 ) {
			buffer[i] = caracter;
			i++;  
		}
	} while ( !estadoFinal(estado) && !(estado == 14) ); /*mientras no se llegue a un estado final o al estado de rechazo*/
		
	/* una vez terminado cierra la cadena con el nulo*/
	buffer[i] = '\0';
	
	/*dependiendo el estado final realizo la accion correspondiente*/
	
	switch ( estado ){

		case 2 : 
				/*para cadenas alfanumericas*/
				if ( col != 11 )
					{ /*si lo ultimo leido no fue un espacio
						ungetc 'rebobina' el archivo un caracter*/
					ungetc(caracter, in); 
					/*trunco la cadena quitando el caracter extra leido del buffer*/
					buffer[i-1] = '\0';
					}
					
				return ID;
				break;
		case 4 : 
				/*para numeros es similar*/
				if ( col != 11 ) 
					{
					ungetc(caracter, in);
					buffer[i-1] = '\0';
					}			

				return CONSTANTE;
				break;
				
		/*en el resto de los casos solo devuelvo el token que corresponde*/
		case 5 : return SUMA; break;
		case 6 : return RESTA; break; 
		case 7 : return PARENIZQUIERDO; break;
		case 8 : return PARENDERECHO; break; 
		case 9 : return COMA; break;
		case 10 : return PUNTOYCOMA; break;
		case 12 : return ASIGNACION; break;
		case 13 : return FDT; break;
		case 14 : return ERRORLEXICO; break;
	}
	
	return 0;
}




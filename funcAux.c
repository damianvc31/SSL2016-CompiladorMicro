#include "definiciones.h"

extern FILE * out;
extern RegTS TS[];
extern char buffer[];
extern TOKEN tokenActual;
extern int flagToken;

/*FUNCIONES AUXILIARES*/

void Match(TOKEN t)
{
    if ( !(t == ProximoToken()) ) ErrorSintactico(); /*lanza error sintactico si el proximo token no coincide*/
    flagToken = 0; /*limpia el flag una vez que hizo el match */
}

TOKEN ProximoToken()
{
    if ( !flagToken ) /*solo obtiene un nuevo token si se matcheo el anterior*/
    {
        tokenActual = scanner();
        if ( tokenActual == ERRORLEXICO ) ErrorLexico(); /*lanza error lexico si el scanner devuelve ese token*/
        flagToken = 1; /*setea el flag para indicar que un nuevo token ha de ser matcheado*/
        if ( tokenActual == ID ) Buscar(buffer, &tokenActual); /*si el token es alfanumerico llama a Buscar para buscarlo en la tabla de simbolos*/
    }
    return tokenActual; 
}

void ErrorLexico() /*muestra mensaje de error lexico en la pantalla e imprime el mensaje en el archivo de salida*/
{
    printf("\033[33mError Lexico\033[37m\n");
    fprintf(out, "Error Lexico\n");
    
}

void ErrorSintactico() /*muestra mensaje de error sintactico en la pantalla e imprime el mensaje en el archivo de salida*/
{
    printf("\033[31mError Sintactico\033[37m\n");
    fprintf(out, "Error Sintactico\n");
}

void Generar(char * accion, char * a, char * b, char * c)
{
    /* Produce la salida de la instruccion para la MV por pantalla y por archivo de salida */
    printf("\033[34m%s\033[37m %s%c%s%c%s\n", accion, a, ',', b, ',', c);
    fprintf(out,"%s %s%c%s%c%s\n", accion, a, ',', b, ',', c );
}

char * Extraer(REG_EXPRESION * reg)
{
    /* Retorna la cadena del registro semantico */
    return reg->nombre;
}

int Buscar(char * id, TOKEN * t)
{
    /* Determina si un identificador esta en la TS */
    int i = 0;
    while ( strcmp("$", TS[i].identificadorDeLexema) ) /*recorre la tabla hasta el centinela*/
    {
        if ( !strcmp(id, TS[i].identificadorDeLexema) ) /*si coincide con el id en el buffer*/
        {
            *t = TS[i].t; /*actualiza el token actual (se cambia por el token dela palabra reservada en caso de que sea una de ellas)*/
            return 1;
        }
        i++;
    }
    return 0;
}

void Colocar(char * id)
{
    /* Agrega un identificador a la TS */
    int i = 4; 
    while ( strcmp("$", TS[i].identificadorDeLexema) ) i++; /*recorre la tabla hasta el centinela*/
    {
        if ( i < (TAMTS -1) ) /*si hay espacio ingresa el nuevo id*/
        {
            strcpy(TS[i].identificadorDeLexema, id );
            TS[i].t = ID;
            strcpy(TS[++i].identificadorDeLexema, "$" );
        }
    }
}

void Chequear(char * s)
{
    /* Si la cadena No esta en la Tabla de Simbolos la agrega,y si es el nombre de una variable genera la instruccion */
    TOKEN t;
    if ( !Buscar(s, &t) )
    {
        Colocar(s);
        Generar("Declara", s, "Entera", "");
    }
}

void Comenzar(void)
{
    /* Inicializaciones Semanticas */
    printf("\033[34mInicializacion\033[37m\n");
    fprintf(out,"Inicializacion\n");
}

void Terminar(void)
{
    /* Genera la instruccion para terminar la ejecucion del programa */
    printf("\033[34mDetiene\033[37m\n");
    fprintf(out,"Detiene\n");
}

void Asignar(REG_EXPRESION izq, REG_EXPRESION der)
{
    /* Genera la instruccion para la asignacion */
    Generar("Almacena", Extraer(&der), izq.nombre, "");
}

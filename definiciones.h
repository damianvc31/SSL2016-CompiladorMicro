#ifndef DEFINICIONES_H_

#define  DEFINICIONES_H_

/*includes librerias estandar*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Constantes */

#define NUMEROESTADOS 15
#define NUMEROCOLUMNAS 13
#define TAMLEX 32
#define TAMTS 1000


typedef enum /*Tokens*/
	{
	    INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO, PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT, ERRORLEXICO
	} TOKEN;

typedef struct /*Registro Tabla de Simbolos*/
	{
	    char identificadorDeLexema[TAMLEX+1];
	    TOKEN t;
	} RegTS;


typedef struct /*Registro Expresion*/
	{
	    TOKEN clase;
	    char nombre[TAMLEX+1];
	    int valor;
	} REG_EXPRESION;

typedef struct /*Registro Operacion*/
	{
	    TOKEN clase;
	    char operador;
	} REG_OPERACION;

/*Prototipos funciones Scanner*/
int columna(int c);		
int estadoFinal(int e);
TOKEN scanner();

/*Prototipos PAS*/
void Objetivo(void);
void Programa(void);
void ListaSentencias(void);
void Sentencia(void);
void ListaIdentificadores(void);
void ListaExpresiones(void);
void Expresion(REG_EXPRESION * resultado);
void Primaria(REG_EXPRESION * resultado);
void OperadorAditivo(REG_OPERACION * op);
void Identificador(REG_EXPRESION * resultado);

/*Prototipos funciones auxiliares*/
void Match(TOKEN t);
TOKEN ProximoToken();
void ErrorLexico();
void ErrorSintactico();
void Generar(char * co, char * a, char * b, char * c);
char * Extraer(REG_EXPRESION * reg);
int Buscar(char * id, TOKEN * t);
void Colocar(char * id);
void Chequear(char * s);
void Comenzar(void);
void Terminar(void);
void Asignar(REG_EXPRESION izq, REG_EXPRESION der);

/*Prototipos rutinas semanticas*/
REG_EXPRESION ProcesarCte(void);
REG_EXPRESION ProcesarId(void);
REG_OPERACION ProcesarOp(void);
void Leer(REG_EXPRESION in);
void Escribir(REG_EXPRESION out);
REG_EXPRESION GenInfijo(REG_EXPRESION e1, REG_OPERACION op, REG_EXPRESION e2);

#endif

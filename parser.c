#include "definiciones.h"

/* PROCEDIMIENTOS DE ANALISIS SINTACTICO (PAS) */

void Objetivo(void)
{
    /* <objetivo> -> <programa> FDT #terminar */
    Programa();
    Match(FDT);
    Terminar();
}

void Programa(void)
{
    /* <programa> -> #comenzar INICIO <listaSentencias> FIN */
    Comenzar();
    Match(INICIO);
    ListaSentencias();
    Match(FIN);
}

void ListaSentencias(void)
{
    /* <listaSentencias> -> <sentencia> {<sentencia>} */
    Sentencia();
    while ( 1 )
    {
        switch ( ProximoToken() )
        {
        case ID : case LEER : case ESCRIBIR :
            Sentencia();
            break;
        default : 
			return; /* fin switch */
        }
    }
}

void Sentencia(void)
{
    TOKEN tok = ProximoToken();
    REG_EXPRESION izq,der;
    switch ( tok )
    {
        case ID :
            /* <sentencia>-> <identificador> := <expresion> #asignar; */
            Identificador(&izq);
            Match(ASIGNACION);
            Expresion(&der);
            Asignar(izq,der);
            Match(PUNTOYCOMA);
            break;
        case LEER :
            /* <sentencia> -> LEER ( <listaIdentificadores> ) */
            Match(LEER);
            Match(PARENIZQUIERDO);
            ListaIdentificadores();
            Match(PARENDERECHO);
            Match(PUNTOYCOMA);
            break;
        case ESCRIBIR :
            /* <sentencia> -> ESCRIBIR ( <listaExpresiones> ) */
            Match(ESCRIBIR);
            Match(PARENIZQUIERDO);
            ListaExpresiones();
            Match(PARENDERECHO);
            Match(PUNTOYCOMA);
            break;
        default :
			ErrorSintactico(tok);
            break;
    }
}

void ListaIdentificadores(void)
{
	/* <listaIdentificadores> -> <identificador> #leer_id {COMA <identificador> #leer_id} */
    REG_EXPRESION reg;
    Identificador(&reg);
    Leer(reg);
    while ( 1 )
    {
        switch ( ProximoToken() )
		{
			case COMA:
				Match(COMA);
				Identificador(&reg);
				Leer(reg);
				break;
			default : 
				return; /* fin switch */
        }
    }
}

void ListaExpresiones(void)
{
	/* <listaExpresiones> -> <expresion> #escribir_exp {COMA <expresion> #escribir_exp} */
    REG_EXPRESION reg;
    Expresion(&reg);
    Escribir(reg);
    while ( 1 )
    {
        switch ( ProximoToken() )
        {
			case COMA:
				Match(COMA);
				Expresion(&reg);
				Escribir(reg);
				break;
			default : 
				return; /* fin switch */
        }
    }
}

void Expresion(REG_EXPRESION * resultado)
{
    /* <expresion> -> <primaria> { <operadorAditivo> <primaria> #gen_infijo} */
    REG_EXPRESION operandoIzq,operandoDer;
    REG_OPERACION op;
    TOKEN t;
    Primaria(&operandoIzq);
    for ( t = ProximoToken(); t == SUMA || t == RESTA; t = ProximoToken() )
    {
        OperadorAditivo(&op);
        Primaria(&operandoDer);
        operandoIzq = GenInfijo(operandoIzq, op, operandoDer);
    }
    *resultado = operandoIzq;
}

void Primaria(REG_EXPRESION * resultado)
{
    TOKEN tok = ProximoToken();
    switch ( tok )
    {
        case ID :
            /* <primaria> -> <identificador> */
            Identificador(resultado);
            break;
        case CONSTANTE :
            /* <primaria> -> CONSTANTE #procesar_cte */
            Match(CONSTANTE);
            *resultado = ProcesarCte();
            break;
        case PARENIZQUIERDO :
            /* <primaria> -> PARENIZQUIERDO <expresion> PARENDERECHO */
            Match(PARENIZQUIERDO);
            Expresion(resultado);
            Match(PARENDERECHO);
            break;
        default :
            ErrorSintactico(tok);
            break;
    }
}

void OperadorAditivo(REG_OPERACION * op)
{
    /* <operadorAditivo> -> SUMA #procesar_op | RESTA #procesar_op */
    TOKEN t = ProximoToken();
    if ( t == SUMA || t == RESTA )
    {
        Match(t);
        *op = ProcesarOp();
    }
    else
        ErrorSintactico();
}

void Identificador(REG_EXPRESION * resultado)
{
    /* <identificador> -> ID #procesar_id */
    Match(ID);
    *resultado = ProcesarId();
}

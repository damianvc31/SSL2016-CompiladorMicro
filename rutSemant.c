#include "definiciones.h"

extern char buffer[];
extern TOKEN tokenActual;

/* RUTINAS SEMANTICAS */

REG_EXPRESION ProcesarCte(void)
{
    /* Convierte cadena que representa numero a numero entero y construye un registro semantico */
    REG_EXPRESION t;
    t.clase = CONSTANTE;
    strcpy(t.nombre, buffer);
    sscanf(buffer, "%d", &t.valor);
    return t;
}

REG_EXPRESION ProcesarId(void)
{
    /* Declara ID y construye el correspondiente registro semantico */
    REG_EXPRESION t;
    Chequear(buffer);
    t.clase = ID;
    strcpy(t.nombre, buffer);
    return t;
}

REG_OPERACION ProcesarOp(void)
{
    /* Declara OP y construye el correspondiente registro semantico */
    REG_OPERACION op;
	op.operador = buffer[0]; /* '+' o '-' */
	op.clase = tokenActual; /* SUMA o RESTA*/
    return op;
}

void Leer(REG_EXPRESION in)
{
    /* Genera la instruccion para leer */
    Generar("Read", in.nombre, "Entera", "");
}

void Escribir(REG_EXPRESION out)
{
    /* Genera la instruccion para escribir */
    Generar("Write", Extraer(&out), "Entera", "");
}

REG_EXPRESION GenInfijo(REG_EXPRESION e1, REG_OPERACION op, REG_EXPRESION e2)
{
	/* Genera la instruccion para una operacion infija y construye un registro semantico con el resultado */
    REG_EXPRESION reg;
    static unsigned int numTemp = 1;
    char cadTemp[TAMLEX+1] ="Temp&";
    char cadNum[TAMLEX+1];
    char cadOp[TAMLEX+1];
    if ( op.clase == SUMA ) strcpy(cadOp, "Suma");
    if ( op.clase == RESTA ) strcpy(cadOp, "Resta");
    sprintf(cadNum, "%d", numTemp);
    numTemp++;
    strcat(cadTemp, cadNum);
    if ( e1.clase == ID) Chequear(Extraer(&e1));
    if ( e2.clase == ID) Chequear(Extraer(&e2));
    Chequear(cadTemp);
    Generar(cadOp, Extraer(&e1), Extraer(&e2), cadTemp);
    strcpy(reg.nombre, cadTemp);
    return reg;
}

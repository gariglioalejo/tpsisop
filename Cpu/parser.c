/*
 * parser.h
 *
 *  Created on: 14/10/2014
 *      Author: utnso
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../ensalada de funciones/funciones.h"
#include "cpu.h"


int parseador(char* palabra, t_tcb* tcb){

	int i;

	i=strcmp(palabra,"LOAD");
	if(i==0){
	printf("ENTRO EN LOAD \n");
	load(tcb);
		}

	else if (i!=0){
	i=strcmp(palabra,"GETM");
	if(i==0){
	printf("ENTRO EN GETM \n");
	getm(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"SETM");
	if(i==0){
	printf("ENTRO EN SETM \n");
	//poner funcion
	}

	else if (i!=0){
	i=strcmp(palabra,"MOVR");
	if(i==0){
	printf("ENTRO EN MOVR \n");
	movr(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"ADDR");
	if(i==0){
	printf("ENTRO EN ADDR \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"SUBR");
	if(i==0){
	printf("ENTRO EN SUBR \n");
	//poner funcion
	}

	else if (i!=0){
	i=strcmp(palabra,"MULR");
	if(i==0){
	printf("ENTRO EN MULR \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"MODR");
	if(i==0){
	printf("ENTRO EN MODR \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"DIVR");
	if(i==0){
	printf("ENTRO EN DIVR \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"INCR");
	if(i==0){
	printf("ENTRO EN INCR \n");
	incr(tcb);
	}
	else if (i!=0){
	i=strcmp(palabra,"DECR");
	if(i==0){
	printf("ENTRO EN DECR \n");
	decr(tcb);
	}
	else if (i!=0){
	i=strcmp(palabra,"COMP");
	if(i==0){
	printf("ENTRO EN COMP \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"CGEQ");
	if(i==0){
	printf("ENTRO EN CGEQ \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"CLEQ");
	if(i==0){
	printf("ENTRO EN CLEQ \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"GOTO");
	if(i==0){
	printf("ENTRO EN GOTO \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"JMPZ");
	if(i==0){
	printf("ENTRO EN JMPZ \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"JPNZ");
	if(i==0){
	printf("ENTRO EN JPNZ \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"INTE");
	if(i==0){
	printf("ENTRO EN INTE \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"FLCL");
	if(i==0){
	printf("ENTRO EN FLCL \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"SHIF");
	if(i==0){
	printf("ENTRO EN SHIF \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"NOPP");
	if(i==0){
	printf("ENTRO EN NOPP \n");
	nopp(tcb);
	}
	else if (i!=0){
	i=strcmp(palabra,"PUSH");
	if(i==0){
	printf("ENTRO EN PUSH \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"TAKE");
	if(i==0){
	printf("ENTRO EN TAKE \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"XXXX");
	if(i==0){
	printf("ENTRO EN XXXX \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"MALC");
	if(i==0){
	printf("ENTRO EN MALC \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"FREE");
	if(i==0){
	printf("ENTRO EN FREE \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"INNN");
	if(i==0){
	printf("ENTRO EN INNN \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"INNC");
	if(i==0){
	printf("ENTRO EN INNC \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"OUTN");
	if(i==0){
	printf("ENTRO EN OUTN \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"OUTC");
	if(i==0){
	printf("ENTRO EN OUTC \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"CREA");
	if(i==0){
	printf("ENTRO EN CREA \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"JOIN");
	if(i==0){
	printf("ENTRO EN JOIN \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"BLOK");
	if(i==0){
	printf("ENTRO EN BLOK \n");
	//poner funcion
	}
	else if (i!=0){
	i=strcmp(palabra,"WAKE");
	if(i==0){
	printf("ENTRO EN WAKE \n");
	//poner funcion
	}

	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	}
	return 0;
}


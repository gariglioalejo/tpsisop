/*
 * parser.h
 *
 *  Created on: 14/10/2014
 *      Author: utnso
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include "../ensalada de funciones/funciones.h"
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
	setm(tcb);
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
	addr(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"SUBR");
	if(i==0){
	printf("ENTRO EN SUBR \n");
	subr(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"MULR");
	if(i==0){
	printf("ENTRO EN MULR \n");
	mulr(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"MODR");
	if(i==0){
	printf("ENTRO EN MODR \n");
	modr(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"DIVR");
	if(i==0){
	printf("ENTRO EN DIVR \n");
	divr(tcb);
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
	comp(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"CGEQ");
	if(i==0){
	printf("ENTRO EN CGEQ \n");
	cgeq(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"CLEQ");
	if(i==0){
	printf("ENTRO EN CLEQ \n");
	cleq(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"GOTO");
	if(i==0){
	printf("ENTRO EN GOTO \n");
	gotoo(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"JMPZ");
	if(i==0){
	printf("ENTRO EN JMPZ \n");
	jmpz(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"JPNZ");
	if(i==0){
	printf("ENTRO EN JPNZ \n");
	jpnz(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"INTE");
	if(i==0){
	printf("ENTRO EN INTE \n");
	inte(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"SHIF");
	if(i==0){
	printf("ENTRO EN SHIF \n");
	shif(tcb);
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
	push(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"TAKE");
	if(i==0){
	printf("ENTRO EN TAKE \n");
	take(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"XXXX");
	if(i==0){
	printf("ENTRO EN XXXX \n");
	xxxx(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"MALC");
	if(i==0){
	printf("ENTRO EN MALC \n");
	fnMALC(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"FREE");
	if(i==0){
	printf("ENTRO EN FREE \n");
	fnFREE(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"INNN");
	if(i==0){
	printf("ENTRO EN INNN \n");
	fnINNN(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"INNC");
	if(i==0){
	printf("ENTRO EN INNC \n");
	fnINNC(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"OUTN");
	if(i==0){
	printf("ENTRO EN OUTN \n");
	fnOUTN(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"OUTC");
	if(i==0){
	printf("ENTRO EN OUTC \n");
	fnOUTC(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"CREA");
	if(i==0){
	printf("ENTRO EN CREA \n");
	fnCREA(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"JOIN");
	if(i==0){
	printf("ENTRO EN JOIN \n");
	fnJOIN(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"BLOK");
	if(i==0){
	printf("ENTRO EN BLOK \n");
	fnBLOK(tcb);
	}

	else if (i!=0){
	i=strcmp(palabra,"WAKE");
	if(i==0){
	printf("ENTRO EN WAKE \n");
	fnWAKE(tcb);
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


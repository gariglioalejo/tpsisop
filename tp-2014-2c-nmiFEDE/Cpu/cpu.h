/*
 * cpu.h
 *
 *  Created on: 06/10/2014
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_


#include <commons/log.h>
#include <commons/config.h>
#include <commons/error.h>
#include <commons/process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>

//#include "../ensalada de funciones/funciones.h"
#include "../tpsisoplib/tpsisoplib.h"

int systemcall;
int ultimainstruccion;
int socketK;
int socketM;
int segmentatioFault;

//Funciones Privilegiadas
int fnMALC (t_tcb * tcb);
int fnFREE (t_tcb * tcb);
int fnINNN (t_tcb * tcb);
int fnINNC (t_tcb * tcb);
int fnOUTN (t_tcb * tcb);
int fnOUTC (t_tcb * tcb);
int fnCREA (t_tcb * tcb);
int fnJOIN (t_tcb * tcb);
int fnBLOK (t_tcb * tcb);
int fnWAKE (t_tcb * tcb);

//Parser
int parseador(char*,t_tcb*);

//FuncionesNoPrivilegiadas
int load(t_tcb*);
int movr(t_tcb*);
int getm(t_tcb*);
int decr(t_tcb*);
int incr(t_tcb*);
int nopp(t_tcb*);
int addr(t_tcb*);
int subr(t_tcb*);
int mulr(t_tcb*);
int gotoo(t_tcb*);
int jmpz(t_tcb*);
int jpnz(t_tcb*);
int comp(t_tcb*);
int cgeq(t_tcb*);
int cleq(t_tcb*);
int divr(t_tcb*);
int modr(t_tcb*);
int xxxx(t_tcb*);
int setm(t_tcb*);
int inte(t_tcb * tcb);
int push(t_tcb * tcb);
int take(t_tcb * tcb);
int shif(t_tcb * tcb);

#endif /* CPU_H_ */

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
#include "funcionesCpu.h"
#include "../ensalada de funciones/funciones.h"


//Funciones Privilegiadas
int fnMALC (t_tcb * cpu);
int fnFREE (void);
int fnINNN (void);
int fnINNC (void);
int fnOUTN (void);
int fnOUTC (void);
int fnCREA (void);
int fnJOIN (void);
int fnBLOK (void);
int fnWAKE (void);

//Parser
int parseador(char*,t_tcb*);

//FuncionesNoPrivilegiadas
int load(t_tcb*);


#endif /* CPU_H_ */

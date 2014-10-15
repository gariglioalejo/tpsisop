/*
 * funcionesNoBloqueantes.h
 *
 *  Created on: 15/10/2014
 *      Author: utnso
 */

#ifndef FUNCIONESNOBLOQUEANTES_H_
#define FUNCIONESNOBLOQUEANTES_H_

#include "../ensalada de funciones/funciones.h"
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

int socketK;
int socketM;

int load(t_tcb * tcb){

	char registro;
	int valor;

	t_solicitarMemoria solicitador;

	solicitador.PID=tcb->pid;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	send(socketK,&solicitador,sizeof(t_solicitarMemoria),0);

	recv(socketK,registro,1,0);

	solicitador.PID=tcb->pid;
	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=4;

	send(socketK,&solicitador,sizeof(t_solicitarMemoria),0);

	recv(socketK,valor,4,0);





	return 0;
}



#endif /* FUNCIONESNOBLOQUEANTES_H_ */

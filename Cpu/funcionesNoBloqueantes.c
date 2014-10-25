/*
 * funcionesNoBloqueantes.h
 *
 *  Created on: 15/10/2014
 *      Author: utnso
 */


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

	int codigoSolicitarMemoria=2;

	t_solicitarMemoria solicitador;

	solicitador.PID=tcb->pid;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

	send(socketK,&solicitador,sizeof(t_solicitarMemoria),0);

	recv(socketK,&registro,sizeof(char),0);

	solicitador.PID=tcb->pid;
	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=4;

	send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

	send(socketK,&solicitador,sizeof(t_solicitarMemoria),0);

	recv(socketK,&valor,sizeof(int),0);

	if(registro==tcb->registroA.nombre){
		tcb->registroA.valores=tcb->registroA.valores+valor;
		printf("El registro: %c, se aumento: %d",tcb->registroA.nombre,valor);
	}

	if(registro==tcb->registroB.nombre){
			tcb->registroB.valores=tcb->registroB.valores+valor;
			printf("El registro: %c, se aumento: %d",tcb->registroB.nombre,valor);
	}

	if(registro==tcb->registroC.nombre){
			tcb->registroC.valores=tcb->registroC.valores+valor;
			printf("El registro: %c, se aumento: %d",tcb->registroC.nombre,valor);
	}

	if(registro==tcb->registroD.nombre){
			tcb->registroD.valores=tcb->registroD.valores+valor;
			printf("El registro: %c, se aumento: %d",tcb->registroD.nombre,valor);
	}

	if(registro==tcb->registroE.nombre){
			tcb->registroE.valores=tcb->registroE.valores+valor;
			printf("El registro: %c, se aumento: %d",tcb->registroE.nombre,valor);
	}

	tcb->P=tcb->P+9;

	printf("Termino LOAD");

	return 0;
}

int getm(t_tcb * tcb){

	char registro1;
	char registro2;

	int aux;

	int codigoSolicitarMemoria=2;

	t_solicitarMemoria solicitador;

	solicitador.PID=tcb->pid;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

	send(socketK,&solicitador,sizeof(t_solicitarMemoria),0);

	recv(socketK,&registro1,sizeof(char),0);

	solicitador.PID=tcb->pid;
	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

	send(socketK,&solicitador,sizeof(t_solicitarMemoria),0);

	recv(socketK,&registro2,sizeof(char),0);

	if(registro2==tcb->registroA.nombre){
			aux=tcb->registroA.valores;
					}

		if(registro2==tcb->registroB.nombre){
				aux=tcb->registroB.valores;
		}

		if(registro2==tcb->registroC.nombre){
			aux=tcb->registroC.valores;
		}

		if(registro2==tcb->registroD.nombre){
			aux=tcb->registroA.valores;
		}

		if(registro2==tcb->registroE.nombre){
			aux=tcb->registroA.valores;
		}

		if(registro1==tcb->registroA.nombre){
			tcb->registroA.valores=aux;
			printf("El registro: %c, se paso A: %d",tcb->registroA.nombre,aux);
		}

		if(registro1==tcb->registroB.nombre){
				tcb->registroB.valores=aux;
				printf("El registro: %c, se paso A: %d",tcb->registroB.nombre,aux);
		}

		if(registro1==tcb->registroC.nombre){
				tcb->registroC.valores=aux;
				printf("El registro: %c, se paso A: %d",tcb->registroC.nombre,aux);
		}

		if(registro1==tcb->registroD.nombre){
				tcb->registroD.valores=aux;
				printf("El registro: %c, se paso A: %d",tcb->registroD.nombre,aux);
		}

		if(registro1==tcb->registroE.nombre){
				tcb->registroE.valores=aux;
				printf("El registro: %c, se paso A: %d",tcb->registroE.nombre,aux);
		}

		tcb->P=tcb->P+6;

		printf("Termino GETM");

	return 0;
}



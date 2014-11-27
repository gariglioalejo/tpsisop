/*
 * funcionesNoBloqueantes.h
 *
 *  Created on: 15/10/2014
 *      Author: utnso
 */


/*
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
*/

#include "cpu.h"

int load(t_tcb * tcb){uint32_t pidAux;
if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;


	char registro;
	int valor;

	int valorrec;
	int exitorec;

	int codigoSolicitarMemoria=4;

	t_solicitarMemoria solicitador;
	t_devolucion devolucion;

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	//recv(socketM,&devolucion,sizeof(t_devolucion),0);

	exitorec = recibirInt(socketM);
	valorrec = recibirInt(socketM);
	devolucion.exito = exitorec;
	devolucion.respuesta = valorrec;

	//Evalua Seg Fault
	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	registro=(char)devolucion.respuesta;

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=4;

	send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	//recv(socketM,&devolucion,sizeof(t_devolucion),0);
	exitorec = recibirInt(socketM);
	valorrec = recibirInt(socketM);
	devolucion.exito = exitorec;
	devolucion.respuesta = valorrec;

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	valor=(int32_t)devolucion.respuesta;

	if(registro==tcb->registroA.nombre){
		tcb->registroA.valores=valor;
		printf("Al registro: %c, se le asigno: %d \n",tcb->registroA.nombre,valor);
	}

	if(registro==tcb->registroB.nombre){
			tcb->registroB.valores=valor;
			printf("Al registro: %c, se le asigno: %d \n",tcb->registroB.nombre,valor);
	}

	if(registro==tcb->registroC.nombre){
			tcb->registroC.valores=valor;
			printf("Al registro: %c, se le asigno: %d \n",tcb->registroC.nombre,valor);
	}

	if(registro==tcb->registroD.nombre){
			tcb->registroD.valores=valor;
			printf("Al registro: %c, se le asigno: %d \n",tcb->registroD.nombre,valor);
	}

	if(registro==tcb->registroE.nombre){
			tcb->registroE.valores=valor;
			printf("Al registro: %c, se se le asigno: %d \n",tcb->registroE.nombre,valor);
	}

	tcb->P=tcb->P+9;

	printf("Termino LOAD \n");

	return 0;
}

int movr(t_tcb * tcb){

	char registro1;
	char registro2;

	int32_t aux;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	int codigoSolicitarMemoria=4;

	t_solicitarMemoria solicitador;
	t_devolucion devolucion;

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	//recv(socketM,&devolucion,sizeof(t_devolucion),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	registro1=(char)devolucion.respuesta;

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	registro2=(char)devolucion.respuesta;

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
			aux=tcb->registroD.valores;
		}

		if(registro2==tcb->registroE.nombre){
			aux=tcb->registroE.valores;
		}

		if(registro1==tcb->registroA.nombre){
			tcb->registroA.valores=aux;
			printf("El registro: %c, se paso a: %d \n",tcb->registroA.nombre,aux);
		}

		if(registro1==tcb->registroB.nombre){
				tcb->registroB.valores=aux;
				printf("El registro: %c, se paso a: %d \n",tcb->registroB.nombre,aux);
		}

		if(registro1==tcb->registroC.nombre){
				tcb->registroC.valores=aux;
				printf("El registro: %c, se paso a: %d \n",tcb->registroC.nombre,aux);
		}

		if(registro1==tcb->registroD.nombre){
				tcb->registroD.valores=aux;
				printf("El registro: %c, se paso a: %d \n",tcb->registroD.nombre,aux);
		}

		if(registro1==tcb->registroE.nombre){
				tcb->registroE.valores=aux;
				printf("El registro: %c, se paso a: %d \n",tcb->registroE.nombre,aux);
		}

		tcb->P=tcb->P+6;

		printf("Termino MOVR\n");

	return 0;
}
int getm(t_tcb * tcb){

	char reg1;
	char reg2;
	int valor;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;

	t_solicitarMemoria solicitador;
	t_devolucion devolucion;

	int solicitarMemoria=4;

	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg1=(char)devolucion.respuesta;

	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg2=(char)devolucion.respuesta;

	if(reg2==tcb->registroA.nombre){

		send(socketM,&solicitarMemoria,sizeof(int),0);

		solicitador.PID=pidAux;
		solicitador.direccion=(uint32_t)tcb->registroA.valores;
		solicitador.tamanio=1;

		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		send(socketM,&solicitador.PID,sizeof(int),0);
		send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
		send(socketM,&solicitador.tamanio,sizeof(int),0);

		devolucion.exito=recibirInt(socketM);
		devolucion.respuesta=recibirInt32(socketM);

		if(devolucion.exito<0){
			segmentatioFault++;
			return 0;
		}

		valor=(int32_t)devolucion.respuesta;

		}

	if(reg2==tcb->registroB.nombre){

		send(socketM,&solicitarMemoria,sizeof(int),0);

		solicitador.PID=pidAux;
		solicitador.direccion=(uint32_t)tcb->registroB.valores;
		solicitador.tamanio=1;

		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		send(socketM,&solicitador.PID,sizeof(int),0);
		send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
		send(socketM,&solicitador.tamanio,sizeof(int),0);

		devolucion.exito=recibirInt(socketM);
		devolucion.respuesta=recibirInt32(socketM);

			if(devolucion.exito<0){
				segmentatioFault++;
				return 0;
		}

		valor=(int32_t)devolucion.respuesta;
		}

	if(reg2==tcb->registroC.nombre){

		send(socketM,&solicitarMemoria,sizeof(int),0);

		solicitador.PID=pidAux;
		solicitador.direccion=(uint32_t)tcb->registroC.valores;
		solicitador.tamanio=1;

		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		send(socketM,&solicitador.PID,sizeof(int),0);
		send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
		send(socketM,&solicitador.tamanio,sizeof(int),0);

		devolucion.exito=recibirInt(socketM);
		devolucion.respuesta=recibirInt32(socketM);

		if(devolucion.exito<0){
				segmentatioFault++;
				return 0;
			}

		valor=(int32_t)devolucion.respuesta;

		}

		if(reg2==tcb->registroD.nombre){

			send(socketM,&solicitarMemoria,sizeof(int),0);

			solicitador.PID=pidAux;
			solicitador.direccion=(uint32_t)tcb->registroD.valores;
			solicitador.tamanio=1;

			//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

			send(socketM,&solicitador.PID,sizeof(int),0);
			send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
			send(socketM,&solicitador.tamanio,sizeof(int),0);

			devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

				if(devolucion.exito<0){
					segmentatioFault++;
					return 0;
				}

				valor=(int32_t)devolucion.respuesta;
			}

			if(reg2==tcb->registroE.nombre){

				send(socketM,&solicitarMemoria,sizeof(int),0);

				solicitador.PID=pidAux;
				solicitador.direccion=(uint32_t)tcb->registroE.valores;
				solicitador.tamanio=1;

				//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

				send(socketM,&solicitador.PID,sizeof(int),0);
				send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
				send(socketM,&solicitador.tamanio,sizeof(int),0);

				devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

						if(devolucion.exito<0){
							segmentatioFault++;
							return 0;
						}

						valor=(int32_t)devolucion.respuesta;
			}


			if(reg1==tcb->registroA.nombre){
					tcb->registroA.valores=valor;
					printf("El registro: %c, vale: %d \n",tcb->registroA.nombre,valor);
				}

				if(reg1==tcb->registroB.nombre){
						tcb->registroB.valores=valor;
						printf("El registro: %c, vale: %d \n",tcb->registroB.nombre,valor);
				}

				if(reg1==tcb->registroC.nombre){
						tcb->registroC.valores=valor;
						printf("El registro: %c, vale: %d \n",tcb->registroC.nombre,valor);
				}

				if(reg1==tcb->registroD.nombre){
						tcb->registroD.valores=valor;
						printf("El registro: %c, vale: %d \n",tcb->registroD.nombre,valor);
				}

				if(reg1==tcb->registroE.nombre){
						tcb->registroE.valores=valor;
						printf("El registro: %c, vale: %d \n",tcb->registroE.nombre,valor);
				}


				tcb->P=tcb->P+6;

				printf("Se salio del GETM \n");

return 0;

}

int decr(t_tcb * tcb){

	char registro;

	int solicitar=4;

	t_solicitarMemoria solicitador;
	t_devolucion devolucion;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	send(socketM,&solicitar,sizeof(int),0);

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	registro=(char)devolucion.respuesta;

	if(registro==tcb->registroA.nombre){
		tcb->registroA.valores=tcb->registroA.valores-1;
	}

	if(registro==tcb->registroB.nombre){
		tcb->registroB.valores=tcb->registroB.valores-1;
	}

	if(registro==tcb->registroC.nombre){
		tcb->registroC.valores=tcb->registroC.valores-1;
	}
	if(registro==tcb->registroD.nombre){
		tcb->registroD.valores=tcb->registroD.valores-1;
	}
	if(registro==tcb->registroE.nombre){
		tcb->registroE.valores=tcb->registroE.valores-1;
	}

	printf("Se decremento el registro: %c en una unidad \n",registro);

	tcb->P=tcb->P+5;

	printf("Salio DECR \n");
	
	return 0;
}

int incr(t_tcb * tcb){

	char registro;

	int solicitar=4;

	t_solicitarMemoria solicitador;
	t_devolucion devolucion;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	send(socketM,&solicitar,sizeof(int),0);

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	registro=(char)devolucion.respuesta;

	if(registro==tcb->registroA.nombre){
		tcb->registroA.valores=tcb->registroA.valores+1;
	}

	if(registro==tcb->registroB.nombre){
		tcb->registroB.valores=tcb->registroB.valores+1;
	}

	if(registro==tcb->registroC.nombre){
		tcb->registroC.valores=tcb->registroC.valores+1;
	}
	if(registro==tcb->registroD.nombre){
		tcb->registroD.valores=tcb->registroD.valores+1;
	}
	if(registro==tcb->registroE.nombre){
		tcb->registroE.valores=tcb->registroE.valores+1;
	}

	printf("Se incremento el registro: %c en una unidad \n",registro);

	tcb->P=tcb->P+5;

	printf("Se salio de INCR \n");
	
	return 0;
}

int nopp(t_tcb * tcb){

	printf("Se consumió un ciclo de quantum \n");

	tcb->P=tcb->P+4;
	
	printf("Se salio de NOPP\n");
	
	return 0;
}

int inte(t_tcb * tcb){


	uint32_t dirSyscall = pedirDireccion(socketM, tcb);

	int codigo = 3;

	tcb->P=tcb->P+8;
	enviarInt(codigo,socketK);
	enviarTcb(tcb,socketK);
	enviarInt(dirSyscall,socketK);


	systemcall++;

	printf("Se Salio de INTE \n");
	
	return 0;
}

int addr(t_tcb * tcb){
	
	char reg1;
	char reg2;
	int aux2;
	t_solicitarMemoria solicitador;
	int32_t aux1;
	t_devolucion devolucion;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	
	int solicitarMemoria=4;
	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;
	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);


	
	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg1=(char)devolucion.respuesta;
	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	send(socketM,&solicitarMemoria,sizeof(int),0);

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg2=(char)devolucion.respuesta;
	printf("%c %c",reg2,reg1);
	switch(reg2){

	case 'M':
	aux1=tcb->M;
	break;


	case 'A':
	aux1=tcb->registroA.valores;
	break;

	case 'B':
	aux1=tcb->registroB.valores;
	break;

	case 'C':
	aux1=tcb->registroC.valores;
	break;

	case 'D':
	aux1=tcb->registroD.valores;
	break;

	case 'E':
	aux1=tcb->registroE.valores;
	break;
	
	case 'S':
	aux1=tcb->S;
	break;

	}


	switch(reg1){
	case 'A':
	aux2=tcb->registroA.valores;
	break;

	case 'B':
	aux2=tcb->registroB.valores;
	break;

	case 'C':
	aux2=tcb->registroC.valores;
	break;

	case 'D':
	aux2=tcb->registroD.valores;
	break;

	case 'E':
	aux2=tcb->registroE.valores;
	break;

	}

	tcb->registroA.valores=aux1+aux2;


	tcb->P=tcb->P+6;

	printf("Se aumento el registro: %c en: %d unidades \n",reg1,(aux1+aux2));

	printf("Se salio de ADDR \n");
	
	return 0;
}

int subr(t_tcb * tcb){

	char reg1;
	char reg2;
	t_solicitarMemoria solicitador;
	int32_t aux1;
	t_devolucion devolucion;

	int solicitarMemoria=4;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg1=(char)devolucion.respuesta;

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	send(socketM,&solicitarMemoria,sizeof(int),0);

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg2=(char)devolucion.respuesta;

	switch(reg2){

	case 'A':
	aux1=tcb->registroA.valores;
	break;

	case 'B':
	aux1=tcb->registroB.valores;
	break;

	case 'C':
	aux1=tcb->registroC.valores;
	break;

	case 'D':
	aux1=tcb->registroD.valores;
	break;

	case 'E':
	aux1=tcb->registroE.valores;
	break;
	}

	switch(reg1){

	case 'A':
	tcb->registroA.valores=tcb->registroA.valores-aux1;
	break;

	case 'B':
	tcb->registroB.valores=tcb->registroB.valores-aux1;
	break;

	case 'C':
	tcb->registroC.valores=tcb->registroC.valores-aux1;
	break;

	case 'D':
	tcb->registroD.valores=tcb->registroD.valores-aux1;
	break;

	case 'E':
	tcb->registroE.valores=tcb->registroE.valores-aux1;
	break;

	}

	tcb->P=tcb->P+6;

	printf("Se disminuyo el registro: %c en: %d unidades \n",reg1,aux1);
	
	printf("Se salio de SUBR\n");

	return 0;
}

int mulr(t_tcb * tcb){

	char reg1;
	char reg2;
	t_solicitarMemoria solicitador;
	int32_t aux1;
	t_devolucion devolucion;

	int solicitarMemoria=4;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg1=(char)devolucion.respuesta;

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	send(socketM,&solicitarMemoria,sizeof(int),0);

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg2=(char)devolucion.respuesta;

	switch(reg2){

	case 'A':
	aux1=tcb->registroA.valores;
	break;

	case 'B':
	aux1=tcb->registroB.valores;
	break;

	case 'C':
	aux1=tcb->registroC.valores;
	break;

	case 'D':
	aux1=tcb->registroD.valores;
	break;

	case 'E':
	aux1=tcb->registroE.valores;
	break;
	}

	switch(reg1){

	case 'A':
	tcb->registroA.valores=tcb->registroA.valores*aux1;
	break;

	case 'B':
	tcb->registroB.valores=tcb->registroB.valores*aux1;
	break;

	case 'C':
	tcb->registroC.valores=tcb->registroC.valores*aux1;
	break;

	case 'D':
	tcb->registroD.valores=tcb->registroD.valores*aux1;
	break;

	case 'E':
	tcb->registroE.valores=tcb->registroE.valores*aux1;
	break;

	}

	tcb->P=tcb->P+6;

	printf("Se multiplico el registro: %c por: %d \n",reg1,aux1);
	
	printf("Se salio del MULR \n");

	return 0;
}

int gotoo(t_tcb * tcb){


	char reg;

	int solicitarMemoria=4;

	t_solicitarMemoria solicitador;
	t_devolucion devolucion;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg=(char)devolucion.respuesta;

	switch(reg){

	case 'A':
	tcb->P=(uint32_t)tcb->registroA.valores+tcb->M;
	break;

	case 'B':
	tcb->P=(uint32_t)tcb->registroB.valores+tcb->M;
	break;

	case 'C':
	tcb->P=(uint32_t)tcb->registroC.valores+tcb->M;
	break;

	case 'D':
	tcb->P=(uint32_t)tcb->registroD.valores+tcb->M;
	break;

	case 'E':
	tcb->P=(uint32_t)tcb->registroE.valores+tcb->M;
	break;

	}

	printf("Se cambio el flujo de ejecución al pc: %u \n",tcb->P);

	printf("Se salio de gotoo \n");
	
	return 0;
}

int jmpz(t_tcb * tcb){

	if(tcb->registroA.valores==0){

	int solicitarMemoria=4;
	uint32_t nuevadir;

	t_devolucion devolucion;
	t_solicitarMemoria solicitador;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=4;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	nuevadir=devolucion.respuesta;

	tcb->P=nuevadir+tcb->M;

	printf("Se cambio la ejecución porque A vale 0, al pc: %u \n",nuevadir);

	}
	else{
		printf("No se hace nada porque es jump zero, y el valor del reg A es: %d \n",tcb->registroA.valores);
		tcb->P=tcb->P+8;
		printf("Se salio del jmpz\n");
		return 0;
	}

	printf("Se salio del jmpz \n");
	
	return 0;
}

int jpnz(t_tcb * tcb){

	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;

	if(tcb->registroA.valores==0){

	printf("No se hace nada porque es jump NO zero, y el valor del reg A es: %d \n",tcb->registroA.valores);
			tcb->P=tcb->P+8;
			printf("Se salio del jmpnz \n");
			return 0;
	}
	else{

		int solicitarMemoria=4;
		uint32_t nuevadir;

		t_devolucion devolucion;

		t_solicitarMemoria solicitador;

		send(socketM,&solicitarMemoria,sizeof(int),0);

		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+4;
		solicitador.tamanio=4;

		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		send(socketM,&solicitador.PID,sizeof(int),0);
		send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
		send(socketM,&solicitador.tamanio,sizeof(int),0);

		devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

		if(devolucion.exito<0){
			segmentatioFault++;
			return 0;
		}

		nuevadir=devolucion.respuesta;

		tcb->P=nuevadir+tcb->M;

		printf("Se cambio la ejecución porque A vale:%d, al pc: %u \n",tcb->registroA.valores,nuevadir);

		
	}

	printf("Se salio del jmpnz\n");
	return 0;
}

int comp(t_tcb * tcb){

	char reg1;
	char reg2;
	int32_t aux1;
	int32_t aux2;

	t_devolucion devolucion;

	int solicitarMemoria=4;

	t_solicitarMemoria solicitador;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg1=(char)devolucion.respuesta;

	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg2=(char)devolucion.respuesta;

	switch(reg1){

	case 'A':
	aux1=tcb->registroA.valores;
	break;

	case 'B':
	aux1=tcb->registroB.valores;
	break;

	case 'C':
	aux1=tcb->registroC.valores;
	break;

	case 'D':
	aux1=tcb->registroD.valores;
	break;

	case 'E':
	aux1=tcb->registroE.valores;
	break;

	case 'X':
	aux1=tcb->X;
	break;

	case 'S':
	aux1=tcb->S;
	break;

	case 'M':
	aux1=tcb->M;
	break;

	}

	switch(reg2){

	case 'A':
	aux2=tcb->registroA.valores;
	break;

	case 'B':
	aux2=tcb->registroB.valores;
	break;

	case 'C':
	aux2=tcb->registroC.valores;
	break;

	case 'D':
	aux2=tcb->registroD.valores;
	break;

	case 'E':
	aux2=tcb->registroE.valores;
	break;

	case 'X':
	aux2=tcb->X;
	break;

	case 'S':
	aux2=tcb->S;
	break;

	case 'M':
	aux2=tcb->M;
	break;


	}

	if(aux1==aux2){
		tcb->registroA.valores=1;
	}
	else{
		tcb->registroA.valores=0;
	}

	tcb->P=tcb->P+6;

	printf("Se puso el valor: %d en el registro A \n",tcb->registroA.valores);
	
	printf("Se salio de COMP \n");
	
	return 0;
}

int cgeq(t_tcb * tcb){

	char reg1;
	char reg2;
	int32_t aux1;
	int32_t aux2;
	t_devolucion devolucion;

	int solicitarMemoria=4;

	t_solicitarMemoria solicitador;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg1=(char)devolucion.respuesta;

	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg2=(char)devolucion.respuesta;

	switch(reg1){

		case 'A':
		aux1=tcb->registroA.valores;
		break;

		case 'B':
		aux1=tcb->registroB.valores;
		break;

		case 'C':
		aux1=tcb->registroC.valores;
		break;

		case 'D':
		aux1=tcb->registroD.valores;
		break;

		case 'E':
		aux1=tcb->registroE.valores;
		break;
		}

	switch(reg2){

		case 'A':
		aux2=tcb->registroA.valores;
		break;

		case 'B':
		aux2=tcb->registroB.valores;
		break;

		case 'C':
		aux2=tcb->registroC.valores;
		break;

		case 'D':
		aux2=tcb->registroD.valores;
		break;

		case 'E':
		aux2=tcb->registroE.valores;
		break;
		}

		if(aux1>=aux2){
			tcb->registroA.valores=1;
		}
		else{
			tcb->registroA.valores=0;
		}

		tcb->P=tcb->P+6;

		printf("Se puso el valor: %d en el registro A \n",tcb->registroA.valores);
	
		printf("Se salio de cgeq \n");
		
		return 0;
}

int cleq(t_tcb * tcb){

	char reg1;
	char reg2;
	int32_t aux1;
	int32_t aux2;
	t_devolucion devolucion;

	int solicitarMemoria=4;

	t_solicitarMemoria solicitador;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg1=(char)devolucion.respuesta;

	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg2=(char)devolucion.respuesta;

	switch(reg1){

			case 'A':
			aux1=tcb->registroA.valores;
			break;

			case 'B':
			aux1=tcb->registroB.valores;
			break;

			case 'C':
			aux1=tcb->registroC.valores;
			break;

			case 'D':
			aux1=tcb->registroD.valores;
			break;

			case 'E':
			aux1=tcb->registroE.valores;
			break;
			}

		switch(reg2){

			case 'A':
			aux2=tcb->registroA.valores;
			break;

			case 'B':
			aux2=tcb->registroB.valores;
			break;

			case 'C':
			aux2=tcb->registroC.valores;
			break;

			case 'D':
			aux2=tcb->registroD.valores;
			break;

			case 'E':
			aux2=tcb->registroE.valores;
			break;
			}

			if(aux1<=aux2){
				tcb->registroA.valores=1;
			}
			else{
				tcb->registroA.valores=0;
			}

			tcb->P=tcb->P+6;

			printf("Se puso el valor: %d en el registro A \n",tcb->registroA.valores);
			
			printf("Se salio de cleq \n");

			return 0;
}

int divr(t_tcb* tcb){

	char reg1;
	char reg2;
	int32_t aux1;
	int32_t aux2;

	int solicitarMemoria=4;

	t_solicitarMemoria solicitador;
	t_devolucion devolucion;
	int pidAux;
	if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.PID=pidAux;
	solicitador.direccion=tcb->P+4;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg1=(char)devolucion.respuesta;

	send(socketM,&solicitarMemoria,sizeof(int),0);

	solicitador.direccion=tcb->P+5;
	solicitador.tamanio=1;

	//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

	send(socketM,&solicitador.PID,sizeof(int),0);
	send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
	send(socketM,&solicitador.tamanio,sizeof(int),0);

	devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

	if(devolucion.exito<0){
		segmentatioFault++;
		return 0;
	}

	reg2=(char)devolucion.respuesta;

	switch(reg1){

		case 'A':
		aux1=tcb->registroA.valores;
		break;

		case 'B':
		aux1=tcb->registroB.valores;
		break;

		case 'C':
		aux1=tcb->registroC.valores;
		break;

		case 'D':
		aux1=tcb->registroD.valores;
		break;

		case 'E':
		aux1=tcb->registroE.valores;
		break;
		}

	switch(reg2){

		case 'A':
		aux2=tcb->registroA.valores;
		break;

		case 'B':
		aux2=tcb->registroB.valores;
		break;

		case 'C':
		aux2=tcb->registroC.valores;
		break;

		case 'D':
		aux2=tcb->registroD.valores;
		break;

		case 'E':
		aux2=tcb->registroE.valores;
		break;
		}

		if(aux2==0){
			printf("DIVR sobre 0, ERROR \n");
			tcb->P=tcb->P+6;
			return 0;
			
		}
		else {
			tcb->registroA.valores=(aux1/aux2);
		}

		tcb->P=tcb->P+6;

		printf("Se puso el valor: %d en el registro A \n",tcb->registroA.valores);
		
		printf("Se salio del divr\n");

	return 0;
}

int modr(t_tcb* tcb){

		char reg1;
		char reg2;
		int32_t aux1;
		int32_t aux2;

		int solicitarMemoria=4;

		t_devolucion devolucion;
		t_solicitarMemoria solicitador;
		int pidAux;
		if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
		send(socketM,&solicitarMemoria,sizeof(int),0);

		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+4;
		solicitador.tamanio=1;

		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		send(socketM,&solicitador.PID,sizeof(int),0);
		send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
		send(socketM,&solicitador.tamanio,sizeof(int),0);

		devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

		if(devolucion.exito<0){
			segmentatioFault++;
			return 0;
		}

		reg1=(char)devolucion.respuesta;

		send(socketM,&solicitarMemoria,sizeof(int),0);

		solicitador.direccion=tcb->P+5;
		solicitador.tamanio=1;

		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		send(socketM,&solicitador.PID,sizeof(int),0);
		send(socketM,&solicitador.direccion,sizeof(uint32_t),0);
		send(socketM,&solicitador.tamanio,sizeof(int),0);

		devolucion.exito=recibirInt(socketM);
	devolucion.respuesta=recibirInt32(socketM);

		if(devolucion.exito<0){
			segmentatioFault++;
			return 0;
		}

		reg2=(char)devolucion.respuesta;

		switch(reg1){

		case 'A':
		aux1=tcb->registroA.valores;
		break;

		case 'B':
		aux1=tcb->registroB.valores;
		break;

		case 'C':
		aux1=tcb->registroC.valores;
		break;

		case 'D':
		aux1=tcb->registroD.valores;
		break;

		case 'E':
		aux1=tcb->registroE.valores;
		break;
		}

		switch(reg2){

		case 'A':
		aux2=tcb->registroA.valores;
		break;

		case 'B':
		aux2=tcb->registroB.valores;
		break;

		case 'C':
		aux2=tcb->registroC.valores;
		break;

		case 'D':
		aux2=tcb->registroD.valores;
		break;

		case 'E':
		aux2=tcb->registroE.valores;
		break;
		}

		tcb->registroA.valores=(aux1%aux2);

		tcb->P=tcb->P+6;

		printf("Se puso el valor: %d en el registro A \n",tcb->registroA.valores);

		printf("Se salio del modr \n");
		
	return 0;
}

int push(t_tcb * tcb){

		char registro;
		int numero;
		void * bytesApush;
		int codigoSolicitarMemoria=4;
		t_solicitarMemoria solicitador;
		int exito = 0;
		int pidAux;
		if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
		//Numero
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+4;
		solicitador.tamanio=4;

		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);
		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);

		recv(socketM,&exito,sizeof(int),0);
		recv(socketM,&numero,sizeof(int),0);

		//Registro
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+8;
		solicitador.tamanio=1;

		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);
		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);

		recv(socketM,&exito,sizeof(int),0);
		recv(socketM,&registro,sizeof(char),0);


		bytesApush = malloc(numero);
		//memcpy(bytesApush,(void*)tcb->registroA.valores,numero);
	
		switch(registro){

		case 'A':
			memcpy(bytesApush,&(tcb->registroA.valores),numero);
			break;

		case 'B':
			memcpy(bytesApush,&(tcb->registroB.valores),numero);
			break;

		case 'C':
			memcpy(bytesApush,&(tcb->registroC.valores),numero);
			break;

		case 'D':
			memcpy(bytesApush,&(tcb->registroD.valores),numero);
			break;

		case 'E':
			memcpy(bytesApush,&(tcb->registroE.valores),numero);
			break;

		}

		int nuevoCursor = tcb->S + numero;
		int direccionAgrabar = tcb->S;

		if(escribirMemoria(tcb->pid,direccionAgrabar,numero,(char *)bytesApush,socketM)){

			tcb->S = nuevoCursor;
		} else {
			printf("no se pudo pushear en Stack\n");
		}


		free(bytesApush);
		tcb->P=tcb->P+9;
		
		printf("HOLA MARTIN BASILE; ES LA VENGANZA MUAJAJAJAJA \n");
		
		printf("Termino PUSH\n");

	return 0;
}


int take(t_tcb * tcb){

		char registro;
		int numero;
		int exito;
		void * bytesApop;
		int codigoSolicitarMemoria=4;
		t_solicitarMemoria solicitador;

		printf("== Base: %d\n",tcb->X);
		printf("== Cursor: %d\n",tcb->S);


		int pidAux;
		if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
		//Numero
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+4;
		solicitador.tamanio=4;

		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);
		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);

		exito = recibirInt(socketM);
		if(exito<0){
			segmentatioFault++;
			return 0;
		}
		recv(socketM,&numero,sizeof(int),0);

		//Registro
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+8;
		solicitador.tamanio=1;

		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);
		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);

		exito = recibirInt(socketM);
		if(exito<0){
			segmentatioFault++;
			return 0;
		}
		recv(socketM,&registro,sizeof(char),0);


		//SolicitarNBytes
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->S-numero;
		solicitador.tamanio=numero;printf("%u\n",tcb->S);
		printf("%u\n",solicitador.direccion);
		bytesApop = malloc(numero);

		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);
		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);

	
		exito=recibirInt(socketM);printf("%u\n",exito);

		if(exito<0){
			segmentatioFault++;
			return 0;
		}
		recv(socketM,bytesApop,numero,0);
		

		switch(registro){

		case 'A':
			memcpy(&(tcb->registroA.valores),bytesApop,numero);
			break;

		case 'B':
			memcpy(&(tcb->registroB.valores),bytesApop,numero);
			break;

		case 'C':
			memcpy(&(tcb->registroC.valores),bytesApop,numero);
			break;

		case 'D':
			memcpy(&(tcb->registroD.valores),bytesApop,numero);
			break;

		case 'E':
			memcpy((void*)tcb->registroE.valores,bytesApop,numero);
			break;

		}


		uint32_t nuevocursor = tcb->S-numero;
		tcb->S = nuevocursor;


	free(bytesApop);
	tcb->P=tcb->P+9;
	printf("Termino TAKE");
	return 0;
}



int shif(t_tcb * tcb){

		char registro;
		int numero;
		int codigoSolicitarMemoria=4;
		int exito;
		t_solicitarMemoria solicitador;
		t_devolucion devolucion;
		int pidAux;
		if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
		//Numero
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+4;
		solicitador.tamanio=4;

		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);




		devolucion.exito = recibirInt(socketM);
		numero = recibirInt(socketM);

		if(devolucion.exito<0){
			segmentatioFault++;
			return 0;
		}

		//Registro
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+8;
		solicitador.tamanio=1;

		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);

		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);


		//recv(socketM,&devolucion,sizeof(t_devolucion),0);

		devolucion.exito = recibirInt(socketM);
		//recv(socketM,&registro,sizeof(char),0);
		devolucion.respuesta = recibirInt(socketM);

		registro = (char) devolucion.respuesta;

		if(devolucion.exito<0){
			segmentatioFault++;
			return 0;
		}



		switch(registro){

		case 'A':

			if(numero > 0){
				int32_t aux = logicalRightShift(tcb->registroA.valores,numero);
				tcb->registroA.valores = aux;
			} else if(numero < 0){
				int32_t aux = logicalLeftShift(tcb->registroA.valores,numero);
				tcb->registroA.valores = aux;
			}

			break;


		case 'B':

			if(numero > 0){
				int32_t aux = logicalRightShift(tcb->registroB.valores,numero);
				tcb->registroB.valores = aux;
			} else if(numero < 0){
				int32_t aux = logicalLeftShift(tcb->registroB.valores,numero);
				tcb->registroB.valores = aux;
			}


			break;


		case 'C':

			if(numero > 0){
				int32_t aux = logicalRightShift(tcb->registroC.valores,numero);
				tcb->registroC.valores = aux;
			} else if(numero < 0){
				int32_t aux = logicalLeftShift(tcb->registroC.valores,numero);
				tcb->registroC.valores = aux;
			}

			break;


		case 'D':

			if(numero > 0){
				int32_t aux = logicalRightShift(tcb->registroD.valores,numero);
				tcb->registroD.valores = aux;
			} else if(numero < 0){
				int32_t aux = logicalLeftShift(tcb->registroD.valores,numero);
				tcb->registroD.valores = aux;
			}


			break;

		case 'E':

					if(numero > 0){
						int32_t aux = logicalRightShift(tcb->registroE.valores,numero);
						tcb->registroE.valores = aux;
					} else if(numero < 0){
						int32_t aux = logicalLeftShift(tcb->registroE.valores,numero);
						tcb->registroE.valores = aux;
					}


					break;

		}

		tcb->P=tcb->P+9;

		printf("Termino SHIF\n");

	return 0;
}



int xxxx(t_tcb* tcb){

	tcb->P=tcb->P+4;

	ultimainstruccion++;
	
	printf("Termino XXXX \n");

	return 0;
}

int setm(t_tcb* tcb){
		int numero; 
		char registro1;
		char registro2;
		t_solicitarMemoria solicitador;
		int exito=0; 
		int codigoSolicitarMemoria=4;
	 	void* bytesApush;
		uint32_t direccionAgrabar;
		int pidAux;
		if(tcb->km==1) pidAux=1;else pidAux=tcb->pid;
		//Numero
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+4;
		solicitador.tamanio=4;
		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);
		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);
		recv(socketM,&exito,sizeof(int),0);
		recv(socketM,&numero,sizeof(int),0);
		
		//Registro1
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+8;
		solicitador.tamanio=1;

		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);
		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);

		recv(socketM,&exito,sizeof(int),0);
		recv(socketM,&registro1,sizeof(char),0);

		//Registro2
		solicitador.PID=pidAux;
		solicitador.direccion=tcb->P+9;
		solicitador.tamanio=1;

		send(socketM,&codigoSolicitarMemoria,sizeof(int),0);
		//send(socketM,&solicitador,sizeof(t_solicitarMemoria),0);

		enviarInt(solicitador.PID,socketM);
		enviarInt32(solicitador.direccion,socketM);
		enviarInt(solicitador.tamanio,socketM);

		recv(socketM,&exito,sizeof(int),0);
		recv(socketM,&registro2,sizeof(char),0);

		bytesApush = malloc(numero);
		//memcpy(bytesApush,(void*)tcb->registroA.valores,numero);
	
		switch(registro2){

		case 'A':
			memcpy(bytesApush,&(tcb->registroA.valores),numero);
			break;

		case 'B':
			memcpy(bytesApush,&(tcb->registroB.valores),numero);
			break;

		case 'C':
			memcpy(bytesApush,&(tcb->registroC.valores),numero);
			break;

		case 'D':
			memcpy(bytesApush,&(tcb->registroD.valores),numero);
			break;

		case 'E':
			memcpy(bytesApush,&(tcb->registroE.valores),numero);
			break;

		}

		switch(registro1){

		case 'A':
			direccionAgrabar=tcb->registroA.valores;
			break;

		case 'B':
			direccionAgrabar=tcb->registroB.valores;
			break;

		case 'C':
			direccionAgrabar=tcb->registroC.valores;
			break;

		case 'D':
			direccionAgrabar=tcb->registroD.valores;
			break;

		case 'E':
			direccionAgrabar=tcb->registroE.valores;
			break;

		}

	if(!escribirMemoria(pidAux,direccionAgrabar,numero,(char *)bytesApush,socketM)){
	segmentatioFault++;
	printf("no se pudo escribir en memoria\n");
	}
		free(bytesApush);
		tcb->P=tcb->P+10;
		printf("Termino setm\n");

	return 0;
}


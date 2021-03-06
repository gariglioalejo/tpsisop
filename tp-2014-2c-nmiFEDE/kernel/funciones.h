/*
 * consola.h
 *
 *  Created on: Aug 30, 2014
 *      Author: utnso
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <sys/select.h>
#include <semaphore.h>
//#include <panel/kernel.h>
//#include "/home/utnso/ansisop-panel/panel/kernel.h"
//#include "/home/utnso/ansisop-panel/panel/panel.h"

typedef struct {
	uint32_t base;
	bool exito;
} t_crearSegmento;

typedef struct {
	char nombre;
	int32_t valores;
} t_registro;

typedef struct {
	int pid;
	int tid;
	int km;
	uint32_t M;					//Base del segmento de codigo
	int tam_seg_cod;
	int tam_seg_stack;
	uint32_t P;	//Ok el tipo?	//Puntero de instruccion
	uint32_t X;	//Ok el tipo?	//Base del stack
	uint32_t S;	//Ok el tipo?	//Cursor de stack
	t_registro registroA;
	t_registro registroB;
	t_registro registroC;
	t_registro registroD;
	t_registro registroE;
	int socketConsola;
	int socketCpu;
	uint32_t direccionSyscallPendiente;
} t_tcb;

typedef struct {
	t_tcb * tcb;
	int idRecurso;
} t_nodoRecurso;

typedef struct {
	t_tcb * tcb;
	bool exito;
} t_reservarSegmentos;

typedef struct {
	t_tcb * tcb;
	int tidAEsperar;
} t_join;

typedef struct {
	int socketConsola;
	int socketCpu;
	int tipoEE;
} t_paraEntradaEstandar;

typedef struct {
	int pid;
	int hilos;
} t_listaHilos;

typedef struct {
	int pid;
	int socketConsola;
} t_listaSocketsConsola;

int recibirInt(int socket) {
	int unInt;
	int recibido;
	if ((recibido = recv(socket, &unInt, sizeof(int), 0)) == -1) {//DETALLE, el size del recv es de int y no de u_32int, anda en la vm de la catedra,pero en una implementacion con otro size de int puede generar problemas al enviar u_32int
		printf("Fallo el recv de recibirInt\n");
		exit(1);
	}
	//printf("valorRecibido: %d\n",recibido);
	/*
	 if (recibido == 0) {
	 printf("Fallo el recv de recibirInt por desconexion\n");
	 //exit(1);
	 }
	 */
	return unInt;
}

uint32_t recibirInt32(int socket) {
	uint32_t int32;
	int recibido;
	if ((recibido = recv(socket, &int32, sizeof(uint32_t), 0)) == -1) {	//DETALLE, el size del recv es de int y no de u_32int, anda en la vm de la catedra,pero en una implementacion con otro size de int puede generar problemas al enviar u_32int
		printf("Fallo el recv de recibirInt32\n");
		exit(1);
	}
	return int32;

}
void enviarInt32(uint32_t mensaje, int socket) {
	int enviados;
	if ((enviados = send(socket, &mensaje, sizeof(mensaje), 0)) == -1) {
		printf("Fallo el send de enviarInt32\n");
		exit(1);
	}
}

void enviarInt(int mensaje, int socket) {
	int enviados;
	if ((enviados = send(socket, &mensaje, sizeof(mensaje), 0)) == -1) {
		printf("Fallo el send de enviarInt\n");
		exit(1);
	}
}
void enviarBeso(int size_beso, char * beso, int socket) {
	int enviados;

	if ((enviados = send(socket, beso, size_beso, 0)) == -1) {
		printf("Fallo el send de enviarBeso\n");
		exit(1);
	}
	//printf("%d\n",enviados);
}

char * recibirBeso(int size_beso, int socket) {
	char * literal_beso = malloc(size_beso);
	int recibido;
	//printf("%d\n",*size_beso);
	if ((recibido = recv(socket, literal_beso, size_beso, 0)) == -1) {
		printf("Fallo el recv de recibirBeso\n");
		exit(1);
	}
	return literal_beso;
}
void enviarTcb(t_tcb * tcb, int socket) {
	int enviados;
	if ((enviados = send(socket, tcb, sizeof(t_tcb), 0)) == -1) {
		printf("Fallo el send de enviarTcb\n");
		exit(1);
	}
}

t_tcb * recibirTcb(int socket) {
	t_tcb * tcb = malloc(sizeof(t_tcb));
	int recibido;
	//printf("%d\n",*size_beso);
	if ((recibido = recv(socket, tcb, sizeof(t_tcb), 0)) == -1) {
		printf("Fallo el recv de recibirTcb\n");
		exit(1);
	}
	return tcb;
}
char * copiarArchivo(char * direccion) {

	FILE * archivo;
	char * literal_codigo;
	struct stat stat_archivo;
	int i;
	archivo = fopen(direccion, "rb");
	if (archivo == NULL ) {
		printf("No pudo abrirse el archivo\n");
		exit(1);
	}
	if (stat(direccion, &stat_archivo)) {
		printf("Fallo el stat\n");
		exit(1);
	}
	literal_codigo = malloc(stat_archivo.st_size);
	for (i = 0; i < stat_archivo.st_size; i++) {
		literal_codigo[i] = getc(archivo);
	}
	//literal_codigo[i] = '\0';//Agregar si se quiere que sea printeable el resultado (solo funciona para un txt, no para los codigo BESO).

	return literal_codigo;
}

/*
 int sizeArchivo(char * direccionArchivo) {
 FILE * archivo;
 struct stat stat_archivo;
 int i;
 archivo = fopen(direccionArchivo, "rb");
 if (archivo == NULL ) {
 printf("No pudo abrirse el archivo\n");
 exit(1);
 }
 if (stat(direccionArchivo, &stat_archivo)) {
 printf("Fallo el stat\n");
 exit(1);
 }
 return stat_archivo.st_size;
 }
 */

char * recibir_serializado(int socketCliente) {
	int size;
	int recibido;
	if ((recibido = recv(socketCliente, &size, sizeof(int), 0)) == -1) {
		printf("Fallo el recv1");
		exit(1);
	}
	char * mensaje = malloc(size);
	if ((recibido = recv(socketCliente, mensaje, size, 0)) == -1) {
		printf("Fallo el recv2");
		exit(1);
	}
	//printf("%s\n",mensaje);
	return mensaje;

}

char * recibir_serializado_beso(int socketCliente) {
	int size;
	int recibido;
	if ((recibido = recv(socketCliente, &size, sizeof(int), 0)) == -1) {
		printf("Fallo el recv1");
		exit(1);
	}
	char * mensaje = malloc(sizeof(size) + size);
	memcpy(mensaje, &size, sizeof(size));
	if ((recibido = recv(socketCliente, mensaje + sizeof(size), size, 0))
			== -1) {
		printf("Fallo el recv2");
		exit(1);
	}
	//printf("%d\n",recibido);

	return mensaje;

}

void enviar_serializado_beso(int codigo, int size_beso, char *beso, int socket) {
	//Serializa un char * con codigo Beso (radioactivo, no imprimible) y lo manda por el socket indicado
	//Mandar con codigo == -1 si solo se quiere mandar el size y el mensaje del beso.

	if (codigo == -1) {
		int enviados;
		char * mensaje_serializado;
		mensaje_serializado = malloc(sizeof(size_beso) + size_beso);
		memcpy(mensaje_serializado, &size_beso, sizeof(size_beso));
		memcpy(mensaje_serializado + sizeof(size_beso), beso, size_beso);

		//printf("%d\n%s\n",*mensaje_serializado,mensaje_serializado+sizeof(size));
		if ((enviados = send(socket, mensaje_serializado,
				sizeof(size_beso) + size_beso, 0)) == -1) {
			printf("Fallo el send  de enviar_serializado_beso");
			exit(1);
		}
		//printf("%d\n",enviados);
		free(mensaje_serializado);
	}
	if (codigo != -1) {
		int enviados;
		char * mensaje_serializado;
		mensaje_serializado = malloc(
				sizeof(codigo) + sizeof(size_beso) + size_beso);
		memcpy(mensaje_serializado, &codigo, sizeof(codigo));
		memcpy(mensaje_serializado + sizeof(codigo), &size_beso,
				sizeof(size_beso));
		memcpy(mensaje_serializado + sizeof(codigo) + sizeof(size_beso), beso,
				size_beso);

		if ((enviados = send(socket, mensaje_serializado,
				sizeof(codigo) + sizeof(size_beso) + size_beso, 0)) == -1) {
			printf("Fallo el send");
			exit(1);
		}
		//printf("%d\n",enviados);
		free(mensaje_serializado);
	}

}

char * enviar_serializado(int codigo, char * mensaje, int socket) { //Debe tener '/0' al final el mensaje!
//Serializa un char * y lo manda por el socket indicado
//Mandar con codigo == -1 si solo se quiere mandar el size y el mensaje

	char * mensaje_serializado;
	if (codigo == -1) {
		int enviados;
		int size = strlen(mensaje) + 1;
		mensaje_serializado = malloc(sizeof(size) + size);
		memcpy(mensaje_serializado, &size, sizeof(size));
		memcpy(mensaje_serializado + sizeof(size), mensaje, size);

//printf("%d\n%s\n",*mensaje_serializado,mensaje_serializado+sizeof(size));
		if ((enviados = send(socket, mensaje_serializado, sizeof(size) + size,
				0)) == -1) {
			printf("Fallo el send");
			exit(1);
		}
	}
	if (codigo != -1) {
		int enviados;
		int size = strlen(mensaje) + 1;
		mensaje_serializado = malloc(sizeof(codigo) + sizeof(size) + size);
		memcpy(mensaje_serializado, &codigo, sizeof(codigo));
		memcpy(mensaje_serializado + sizeof(codigo), &size, sizeof(size));
		memcpy(mensaje_serializado + sizeof(codigo) + sizeof(size), mensaje,
				size);

		//printf("%d\n%s\n",*mensaje_serializado,mensaje_serializado+sizeof(size));
		if ((enviados = send(socket, mensaje_serializado,
				sizeof(codigo) + sizeof(size) + size, 0)) == -1) {
			printf("Fallo el send\n");
			exit(1);
		}
	}
	return mensaje_serializado;
}

int conectarse(char * ip, char * puerto) { //Devuelve el socketServer ya conectado al server

	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	int serverSocket;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;     //IPv4
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(ip, puerto, &hints, &servinfo)) != 0) {
		printf("Fallo el getaddrinfo\n");
		exit(1);
	}

	if ((serverSocket = socket(servinfo->ai_family, servinfo->ai_socktype,
			servinfo->ai_protocol)) == -1) {
		printf("Fallo el socket\n");
		exit(1);
	}

	if (connect(serverSocket, servinfo->ai_addr, servinfo->ai_addrlen)) {
		printf("Fallo el connect\n");
		exit(1);
	}

	freeaddrinfo(servinfo);

	return serverSocket;

}
int crearServer(char * puerto) {

	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	int listenningSocket;
	int yes = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;     //IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, puerto, &hints, &servinfo)) != 0) {
		printf("Fallo el getaddrinfo\n");
		exit(1);
	}
	if ((listenningSocket = socket(servinfo->ai_family, servinfo->ai_socktype,
			servinfo->ai_protocol)) == -1) {
		printf("Fallo el socket\n");
		exit(1);
	}
	if (setsockopt(listenningSocket, SOL_SOCKET, SO_REUSEADDR, &yes,
			sizeof(int)) == -1) {
		printf("Fallo el setsockopt");
		exit(1);
	}

	if (bind(listenningSocket, servinfo->ai_addr, servinfo->ai_addrlen)) {
		printf("Fallo el bind\n");
		exit(1);
	}
	freeaddrinfo(servinfo);

	if (listen(listenningSocket, 15)) {	//Backlog de 15, no creo que sea necesario mas
		printf("Fallo el listen\n");
		exit(1);
	}

	return listenningSocket;
}
int aceptarConexion(int listenningSocket) {
	int socketCliente;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	if ((socketCliente = accept(listenningSocket, (struct sockaddr *) &addr,
			&addrlen)) == -1) {
		printf("Fallo el accept\n");
		exit(1);
	}
	return socketCliente;
}

t_crearSegmento * crearSegmento(int pid, int tam, int socket) {
	int crear_segmento = 1;
	int hayLugar;
	enviarInt(crear_segmento, socket);
	enviarInt(pid, socket);
	enviarInt(tam, socket);
	hayLugar = recibirInt(socket);
	t_crearSegmento * resultado = malloc(sizeof(t_crearSegmento));
	if (hayLugar == 1) {
		resultado->base = recibirInt32(socket);
		resultado->exito = true;
		return resultado;
	} else {
		resultado->exito = false;
		return resultado;
	}
}

bool escribirMemoria(int pid, uint32_t direccion, int size, char * mensaje,
		int socket) {
	enviarInt(3, socket);
	enviarInt(pid, socket);
	enviarInt32(direccion, socket);
	enviarInt(size, socket);
	enviarBeso(size, mensaje, socket);
	int hayLugar = recibirInt(socket);
	if (hayLugar) {
		return true;
	}
	return false;
}

bool destruirSegmento(int pid, uint32_t base, int socket) {
	enviarInt(2, socket);
	enviarInt(pid, socket);
	enviarInt32(base, socket);
	int exito = recibirInt(socket);
	if (exito == 1) {
		return true;
	}
	return false;
}

t_reservarSegmentos * reservarStackCrea(int pid, t_tcb * tcbHijo, int stack, int socketMsp, int socketCpu) {

	t_reservarSegmentos * tcb_resultado = malloc(sizeof(t_reservarSegmentos));
	tcb_resultado->tcb = malloc(sizeof(t_tcb));
	t_crearSegmento * resultado = malloc(sizeof(t_crearSegmento));
	int diferenciaBaseCursor;

	resultado = crearSegmento(pid, stack, socketMsp);
	if (resultado->exito) {
		diferenciaBaseCursor = tcbHijo->S - tcbHijo->X;
		tcbHijo->X = resultado->base;
		tcbHijo->S = resultado->base + diferenciaBaseCursor;
	} else {
		printf("No pudo crear segmento para el stack en reservarStackCrea\n");
		//Indica que fallo la creacion del Stack, debe abortarse el padre y todos los hijos.
		tcb_resultado->exito = false;
		return tcb_resultado;
	}
	tcb_resultado->tcb = tcbHijo;
	tcb_resultado->exito = true;

	return tcb_resultado;
}

t_reservarSegmentos reservarSegmentos(int pid, int sizeBeso, char * beso,
		int stack, int socketMsp, int socketCliente) {
	t_reservarSegmentos tcb_resultado;
	t_tcb * tcb = malloc(sizeof(t_tcb));
	t_crearSegmento * resultado;
	resultado = crearSegmento(pid, sizeBeso, socketMsp);
	if (resultado->exito) {
		tcb->M = resultado->base;
		tcb->P = resultado->base;
	} else {
		printf("No pudo crear segmento para el codigo en reservarSegmentos\n");
		if (socketCliente != -1) {
			enviar_serializado(1,
					"No hay memoria disponible para crear segmento de codigo",
					socketCliente);
		}
		tcb_resultado.exito = false;
		return tcb_resultado;
		//falta aca como abortar

	}
	if (escribirMemoria(pid, resultado->base, sizeBeso, beso, socketMsp)) {
		tcb->tam_seg_cod = sizeBeso;
	} else {
		printf("Segmentation Fault al Escribir Memoria en reservarSegmentos\n");
		if (socketCliente != -1) {
			enviar_serializado(1,
					"Segmentation Fault al escribir el segmento de codigo",
					socketCliente);
		}
		tcb_resultado.exito = false;
		return tcb_resultado;
	}
	resultado = crearSegmento(pid, stack, socketMsp);
	if (resultado->exito) {
		tcb->X = resultado->base;
		tcb->S = resultado->base;
	} else {
		printf("No pudo crear segmento para el stack en reservarSegmentos\n");
		if (socketCliente != -1) {
			enviar_serializado(1,
					"No hay memoria disponible para crear segmento de stack",
					socketCliente);
		}
		tcb_resultado.exito = false;
		return tcb_resultado;
	}
	tcb_resultado.tcb = tcb;
	tcb_resultado.exito = true;
	return tcb_resultado;
}

struct stat hacerStat(char * direccion) {
	struct stat stat_beso;
	if (stat(direccion, &stat_beso)) {
		printf("Fallo el stat de hacerStat\n");
		exit(1);
	}
	return stat_beso;
}

char *inputString(FILE* fp, size_t size) {

	char *str;
	int ch;
	size_t len = 0;
	str = realloc(NULL, sizeof(char) * size);
	if (!str)
		return str;
	while (EOF != (ch = fgetc(fp)) && ch != '\n') {
		str[len++] = ch;
		if (len == size) {
			str = realloc(str, sizeof(char) * (size += 1));
			if (!str)
				return str;
		}
	}
	str[len++] = '\0';

	return realloc(str, sizeof(char) * len);
}

void checkArgument(int argumentsDesired, int argc) {
	if (argc != argumentsDesired) {
		printf("No hay suficients argumentos\n");
		exit(1);
	}
}

bool estaEnLaListaElInt(t_list * lista, int elInt) {
	bool esElInt(void * interger) {
		int * intergerAux = interger;
		return *intergerAux == elInt;
	}
	return list_any_satisfy(lista, esElInt);
}
int * removerDeLaListaElInt(t_list * lista, int elInt) {
	bool esElInt(void * interger) {
		int * intergerAux = interger;
		return *intergerAux == elInt;
	}
	return list_remove_by_condition(lista, esElInt);
}
bool estaEnlaListaSocketsConsola(t_list * lista, int socketConsola) {
	bool esElDelSocket(void * nodoConsola) {
		t_listaSocketsConsola* nodoConsolaAux = nodoConsola;
		return nodoConsolaAux->socketConsola == socketConsola;
	}
	return list_any_satisfy(lista, esElDelSocket);
}

t_listaSocketsConsola * removerDeLaListaSocketsConsola(t_list * lista,
		int socketConsola) {
	bool esElDelSocket(void * nodoConsola) {
		t_listaSocketsConsola * nodoConsolaAux = nodoConsola;
		return nodoConsolaAux->socketConsola == socketConsola;
	}
	return list_remove_by_condition(lista, esElDelSocket);
}

t_tcb * removerTcbConElPid(t_list * lista, int pid) {
	bool esElTcbDelPid(void * tcb) {
		t_tcb * tcbAux = tcb;
		if (tcbAux->km != 1) {
			return tcbAux->pid == pid;
		} else {
			return false;
		}
	}
	return list_remove_by_condition(lista, esElTcbDelPid);
}

t_tcb * obtenerTcbConElPid(t_list * lista, int pid) {
	bool esElTcbDelPid(void * tcb) {
		t_tcb * tcbAux = tcb;
		if (tcbAux->km != 1) {
			return tcbAux->pid == pid;
		} else {
			return false;
		}
	}
	return list_find(lista, esElTcbDelPid);
}

bool hayTcbConElPid(t_list * lista, int pid) {
	bool esElTcbDelPid(void * tcb) {
		t_tcb * tcbAux = tcb;
		if (tcbAux->km != 1) {
			return tcbAux->pid == pid;
		} else {
			return false;
		}
	}
	return list_any_satisfy(lista, esElTcbDelPid);
}

t_tcb * removerTcbConElTid(t_list * lista, int tid) {
	bool esElTcbDelTid(void * tcb) {
		t_tcb * tcbAux = tcb;
		if (tcbAux->km != 1) {
			return tcbAux->tid == tid;
		} else {
			return false;
		}
	}
	return list_remove_by_condition(lista, esElTcbDelTid);
}

t_tcb * removerTcbConElSocketCpu(t_list * lista, int socket) {
	bool esElTcbDelSocketCpu(void * tcb) {
		t_tcb * tcbAux = tcb;
		if (tcbAux->km != 1) {
			return tcbAux->socketCpu == socket;
		} else {
			return false;
		}
	}
	return list_remove_by_condition(lista, esElTcbDelSocketCpu);
}

bool hayTcbConElSocketCpu(t_list * lista, int socket) {
	bool esElTcbDelSocketCpu(void * tcb) {
		t_tcb * tcbAux = tcb;
		if (tcbAux->km != 1) {
			return tcbAux->socketCpu == socket;
		} else {
			return false;
		}
	}
	return list_any_satisfy(lista, esElTcbDelSocketCpu);
}

t_tcb * obtenerTcbConElSocketCpu(t_list * lista, int socket) {
	bool esElTcbDelSocketCpu(void * tcb) {
		t_tcb * tcbAux = tcb;
		return tcbAux->socketCpu == socket;
	}
	return list_find(lista, esElTcbDelSocketCpu);
}

t_tcb * sacarElKM(t_list * lista) {
	bool esElKM(void * tcb) {
		t_tcb * tcbAux = tcb;
		return tcbAux->km == 1;
	}
	return list_remove_by_condition(lista, esElKM);
}

bool hayNodoJoinConElPid(t_list * lista, int pid) {
	bool esElDelPid(void * nodoJoin) {
		t_join* nodoJoinAux = nodoJoin;
		return nodoJoinAux->tcb->pid == pid;
	}
	return list_any_satisfy(lista, esElDelPid);
}

bool hayNodoJoinConElTid(t_list * lista, int pid, int tid) {//ESTA ES PARA BUSCAR NODO CON EL TIDAESPERAR QUE CONCUERDE CON EL TID DEL PARAMETRO
	bool esElDelTid(void * nodoJoin) {
		t_join* nodoJoinAux = nodoJoin;
		if (nodoJoinAux->tcb->pid == pid) {	//EN REALIDAD,NO ES NECESARIO MANDARLE EL PID COMO PARAMETRO, PERO WE.
			return nodoJoinAux->tidAEsperar == tid;
		} else {
			return false;
		}
	}
	return list_any_satisfy(lista, esElDelTid);
}

bool hayNodoJoinConElTidPropio(t_list * lista, int tid) {//ESTA ES PARA BUSCAR UN NODO QUE DENTRO DEL TCB->TID SEA IGUAL AL TID DEL PARAMETRO
	bool esElDelTidPropio(void * nodoJoin) {
		t_join* nodoJoinAux = nodoJoin;
		return nodoJoinAux->tcb->tid == tid;
	}
	return list_any_satisfy(lista, esElDelTidPropio);
}

t_join* removerNodoJoinDelPid(t_list * lista, int pid) {
	bool esElDelPid(void * nodoJoin) {
		t_join* nodoJoinAux = nodoJoin;
		return nodoJoinAux->tcb->pid == pid;
	}
	return list_remove_by_condition(lista, esElDelPid);
}

t_join* removerNodoJoinDelTid(t_list * lista, int pid, int tid) {//ESTA ES PARA REMOVER NODO CON EL TIDAESPERAR QUE CONCUERDE CON EL TID DEL PARAMETRO
	bool esElDelTid(void * nodoJoin) {
		t_join* nodoJoinAux = nodoJoin;
		if (nodoJoinAux->tcb->pid == pid) {
			return nodoJoinAux->tidAEsperar == tid;
		} else {
			return false;
		}
	}
	return list_remove_by_condition(lista, esElDelTid);
}

bool hayNodoHilosConElPid(t_list * lista, int pid) {
	bool esElDelPid(void * nodoHilos) {
		t_listaHilos* nodoHilosAux = nodoHilos;
		return nodoHilosAux->pid == pid;
	}
	return list_any_satisfy(lista, esElDelPid);
}

t_listaHilos* removerNodoHilosDelPid(t_list * lista, int pid) {
	bool esElDelPid(void * nodoHilos) {
		t_listaHilos* nodoHilosAux = nodoHilos;
		return nodoHilosAux->pid == pid;
	}
	return list_remove_by_condition(lista, esElDelPid);
}

bool hayNodoRecursoConElPid(t_list * lista, int pid) {
	bool esElDelPid(void * nodoRecurso) {
		t_nodoRecurso* nodoRecursoAux = nodoRecurso;
		return nodoRecursoAux->tcb->pid == pid;
	}
	return list_any_satisfy(lista, esElDelPid);
}

bool hayNodoRecursoConElTid(t_list * lista, int tid) {
	bool esElDelTid(void * nodoRecurso) {
		t_nodoRecurso* nodoRecursoAux = nodoRecurso;
		return nodoRecursoAux->tcb->tid == tid;
	}
	return list_any_satisfy(lista, esElDelTid);
}

t_nodoRecurso * removerNodoRecursoDelPid(t_list * lista, int pid) {
	bool esElDelPid(void * nodoRecurso) {
		t_nodoRecurso* nodoRecursoAux = nodoRecurso;
		return nodoRecursoAux->tcb->pid == pid;
	}
	return list_remove_by_condition(lista, esElDelPid);
}

t_nodoRecurso * removerNodoRecursoPorRecurso(t_list * lista, int recurso) {
	bool esElDelRecurso(void * nodoRecurso) {
		t_nodoRecurso* nodoRecursoAux = nodoRecurso;
		return nodoRecursoAux->idRecurso==recurso;
	}
	return list_remove_by_condition(lista, esElDelRecurso);
}




#endif /* CONSOLA_H_ */

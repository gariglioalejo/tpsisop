/*
 * tpsisoplib.h
 *
 *  Created on: 26/08/2014
 *      Author: utnso
 */

#ifndef TPSISOPLIB_H_
#define TPSISOPLIB_H_




#endif /* TPSISOPLIB_H_ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <unistd.h>
#include <commons/config.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>

typedef struct {
	int pid;
	int tid;
	int km;
	int M;					//Base del segmento de codigo
	int tam_seg_cod;
	char * P;	//Ok el tipo?	//Puntero de instruccion
	char * X;	//Ok el tipo?	//Base del stack
	char * S;	//Ok el tipo?	//Cursor de stack
	int A;
	int B;
	int C;
	int D;
	int E;
} t_tid;

typedef struct {

	uint32_t segmento;
	uint32_t pagina;
	uint32_t offset;

} t_msp_dir;


char * recibir_serializado(int socketCliente);
char * enviar_serializado(int codigo, char * mensaje, int socket);
int conectarse(char * ip, char * puerto);
int crearServer(char * puerto);
int aceptarConexion(int listenningSocket);
t_msp_dir * convertirIntADir(uint32_t dir);


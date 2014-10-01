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

typedef struct {
	uint32_t base;
	bool exito;
} t_crearSegmento;

typedef struct {
	int pid;
	int tid;
	int km;
	uint32_t M;					//Base del segmento de codigo
	int tam_seg_cod;
	uint32_t P;	//Ok el tipo?	//Puntero de instruccion
	uint32_t X;	//Ok el tipo?	//Base del stack
	uint32_t S;	//Ok el tipo?	//Cursor de stack
	int A;
	int B;
	int C;
	int D;
	int E;
} t_tcb;

typedef struct {
	t_tcb * tcb;
	bool exito;
} t_reservarSegmentos;


char * recibir_serializado(int socketCliente);
char * enviar_serializado(int codigo, char * mensaje, int socket);
int conectarse(char * ip, char * puerto);
int crearServer(char * puerto);
int aceptarConexion(int listenningSocket);
t_msp_dir * convertirIntADir(uint32_t dir);
uint32_t convertirDirAInt(t_msp_dir * direccion);
int recibirInt(int socket);


uint32_t recibirInt32(int socket) ;
void enviarInt32(uint32_t mensaje, int socket);
void enviarInt(int mensaje, int socket);
void enviarBeso(int size_beso, char * beso, int socket) ;
char * recibirBeso(int size_beso, int socket) ;
void enviarTcb(t_tcb * tcb, int socket) ;
t_tcb * recibirTcb(int socket) ;
char * copiarArchivo(char * direccion) ;
/*
 int sizeArchivo(char * direccionArchivo);
 */

char * recibir_serializado(int socketCliente) ;
char * recibir_serializado_beso(int socketCliente);
void enviar_serializado_beso(int codigo, int size_beso, char *beso, int socket);
char * enviar_serializado(int codigo, char * mensaje, int socket) ;
int conectarse(char * ip, char * puerto) ;
int crearServer(char * puerto) ;
int aceptarConexion(int listenningSocket);
t_crearSegmento * crearSegmento(int pid, int tam, int socket);
bool escribirMemoria(int pid, uint32_t direccion, int size, char * mensaje, int socket) ;
t_reservarSegmentos reservarSegmentos(int pid, int sizeBeso, char * beso, int stack, int socketMsp, int socketCliente) ;
struct stat hacerStat(char * direccion);



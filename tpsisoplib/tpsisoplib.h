/*
 * tpsisoplib.h
 *
 *  Created on: 26/08/2014
 *      Author: utnso
 */



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
	char nombre;
	int32_t valores;
}t_registro;

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
	bool exito;
} t_reservarSegmentos;

typedef struct {
	int PID;
	uint32_t direccion;
	int tamanio;
} t_solicitarMemoria;



int recibirInt(int socket);
uint32_t recibirInt32(int socket) ;
void enviarInt32(uint32_t mensaje, int socket);
void enviarInt(int mensaje, int socket) ;

void enviarTcb(t_tcb * tcb, int socket);
t_tcb * recibirTcb(int socket) ;
char * copiarArchivo(char * direccion) ;
/*
 int sizeArchivo(char * direccionArchivo);
 */
char * recibir_serializado(int socketCliente);
char * recibir_serializado_beso(int socketCliente) ;
void enviar_serializado_beso(int codigo, int size_beso, char *beso, int socket) ;
char * enviar_serializado(int codigo, char * mensaje, int socket);
int conectarse(char * ip, char * puerto);
int crearServer(char * puerto) ;
int aceptarConexion(int listenningSocket) ;
t_crearSegmento * crearSegmento(int pid, int tam, int socket);
bool destruirSegmentoAllocado(int pid, uint32_t base, int socket);
bool escribirMemoria(int pid, uint32_t direccion, int size, char * mensaje,	int socket);
t_reservarSegmentos reservarSegmentos(int pid, int sizeBeso, char * beso, int stack, int socketMsp, int socketCliente);
struct stat hacerStat(char * direccion);
char* pedirPrimeraPalabra(int socketMSP,t_tcb* tcb);
char* pedirString(int socketMSP,t_tcb* tcb);
int pedirDireccion(int socketMSP,t_tcb* tcb);
char *inputString(FILE* fp, size_t size);
int copiarTcb(t_tcb * tcbviejo, t_tcb * tcbnuevo);
int duplicarStack(t_tcb * tcb, t_tcb * nuevotcb, int socketMsp);






t_msp_dir * convertirIntADir(uint32_t dir);
uint32_t convertirDirAInt(t_msp_dir * direccion);

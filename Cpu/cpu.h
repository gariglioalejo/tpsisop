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
#include <commons/collections/list.h>
#include <commons/string.h>
//#include <panel/cpu.h>
//#include <panel/panel.h>
//#include <panel/kernel.h>

#include <stdbool.h>
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



//===INICIO PANEL
typedef enum {
	KERNEL, CPU
} t_tipo_proceso;
typedef enum {
	NEW, READY, EXEC, BLOCK, EXIT
} t_cola;

typedef struct {
	uint32_t pid;
	uint32_t tid;
	bool kernel_mode;
	uint32_t segmento_codigo;
	uint32_t segmento_codigo_size;
	uint32_t puntero_instruccion;
	uint32_t base_stack;
	uint32_t cursor_stack;
	int32_t registros[5];
	t_cola cola;
} t_hilo;

typedef struct {
	int32_t registros_programacion[5]; //A, B, C, D y E
	uint32_t M; //Base de segmento de código
	uint32_t P; //Puntero de instrucción
	uint32_t X; //Base del segmento de Stack
	uint32_t S; //Cursor de stack
	uint32_t K; //Kernel Mode
	uint32_t I; //PID
} t_registros_cpu;

t_registros_cpu * hola;
t_log* logp;
t_list* parametros;


void inicializar_panel(t_tipo_proceso tipo_proceso, char* path);
void comienzo_ejecucion(t_hilo* hilo, uint32_t quantum);
void fin_ejecucion();
void ejecucion_instruccion(char* mnemonico, t_list* parametros);
void cambio_registros(t_registros_cpu registros);
void ejecucion_hilo(t_hilo* hilo, uint32_t quantum);



//===PANEL AUX

void copiarHiloPanel(t_hilo * hiloPanel, t_tcb * tcb);
void copiarRegistrosPanel(t_registros_cpu * registrosPanel, t_tcb * tcb);
void panel_instruccion(char** params, int cant, char* instruccion);

//===FIN PANEL



//Funciones Privilegiadas
int fnMALC(t_tcb * tcb);
int fnFREE(t_tcb * tcb);
int fnINNN(t_tcb * tcb);
int fnINNC(t_tcb * tcb);
int fnOUTN(t_tcb * tcb);
int fnOUTC(t_tcb * tcb);
int fnCREA(t_tcb * tcb);
int fnJOIN(t_tcb * tcb);
int fnBLOK(t_tcb * tcb);
int fnWAKE(t_tcb * tcb);

//Parser
int parseador(char*, t_tcb*);

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

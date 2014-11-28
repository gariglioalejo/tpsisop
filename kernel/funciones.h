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
#include <string.h>
#include "commons/collections/list.h"
#include "commons/string.h"
#include "commons/log.h"
//#include <panel/kernel.h>
//#include "/home/utnso/ansisop-panel/panel/kernel.h"
//#include "/home/utnso/ansisop-panel/panel/panel.h"

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

t_log* logAux;

/*
 * Las funciones declaradas en los headers panel.h, cpu.h,
 * y kernel.h deben ser invocadas a modo de notificación de
 * los eventos que representan.
 */

/**
 * Debe invocarse luego de inicializar el proceso en cuestión (CPU o Kernel).
 * Por ejemplo: inicializar_panel(CPU, "/home/utnso/panel");
 *
 * @param  tipo_proceso Indica si es un CPU o un Kernel.
 * @param  path  Es la ruta absoluta a un directorio donde el panel pueda
 *               generar archivos de log libremente. El directorio debe existir.
 **/
void inicializar_panel(t_tipo_proceso tipo_proceso, char* path);

t_list* kernel_cpus_conectadas;
t_list* kernel_consolas_conectadas;

/**
 * Debe invocarse tras conectarse una CPU.
 *
 * @param  id  ID de la CPU.
 */
void conexion_cpu(uint32_t id);

/**
 * Debe invocarse tras desconectarse una CPU.
 *
 * @param  id  ID de la CPU.
 */
void desconexion_cpu(uint32_t id);

/**
 * Debe invocarse tras conectarse una consola de un programa.
 *
 * @param  id  ID de la consola.
 */
void conexion_consola(uint32_t id);

/**
 * Debe invocarse tras desconectarse una consola de un programa.
 *
 * @param  id  ID de la CPU.
 */
void desconexion_consola(uint32_t id);

/**
 * Debe invocarse cada vez que ocurra algún cambio en algun campo
 * del TCB de un hilo.
 *
 */
void hilos(t_list* hilos);

/**
 * Debe invocarse cada vez que la CPU invoque una funcionalidad definida
 * en la sección del enunciado "Servicios expuestos a la CPU".
 *
 * @param  mnemonico  Nombre de la funcionalidad.
 * @param  hilo Estructura TCB del hilo que invocó la funcionalidad.
 */
void instruccion_protegida(char* mnemonico, t_hilo* hilo);

typedef struct {
	int32_t registros_programacion[5]; //A, B, C, D y E
	uint32_t M; //Base de segmento de código
	uint32_t P; //Puntero de instrucción
	uint32_t X; //Base del segmento de Stack
	uint32_t S; //Cursor de stack
	uint32_t K; //Kernel Mode
	uint32_t I; //PID
} t_registros_cpu;

/**
 * Debe invocarse cada vez que un hilo ingrese a la CPU.
 *
 * @param  hilo  Estructura conteniendo todos los campos del TCB del hilo.
 * @param  quantum  Tamaño del Quantum.
 */
void comienzo_ejecucion(t_hilo* hilo, uint32_t quantum);

/**
 * Debe invocarse cada vez que un hilo salga de la CPU.
 */
void fin_ejecucion();

/**
 * Debe invocarse cada vez se vaya a ejecutar una instrucción.
 * Por ejemplo: ejecucion_instruccion("ABCD", "soy", 1, "parametro");
 *
 * @param  mnemonico  Nombre de la instrucción a ejecutar.
 * @param  parametros  Parametros de la instrucción. a ejecutar.
 */
void ejecucion_instruccion(char* mnemonico, t_list* parametros);

/**
 * Debe invocarse cada vez que ocura algún cambio en alguno de los
 * registros de la CPU (una vez por instruccion a ejecutar, luego de
 * llamar a ejecucion_instruccion()).
 *
 * @param  registros  Estructura conteniendo cada uno de los registros de la CPU.
 */
void cambio_registros(t_registros_cpu registros);

//-------------------------------------------------
//Retrocompatibilidad con el ejemplo del enunciado:
void ejecucion_hilo(t_hilo* hilo, uint32_t quantum);

void inicializar_panel(t_tipo_proceso tipo_proceso, char* path) {
	char* tipo_proceso_str;

	if (tipo_proceso == KERNEL)
		tipo_proceso_str = "kernel";
	else if (tipo_proceso == CPU)
		tipo_proceso_str = "cpu";
	else
		tipo_proceso_str = "?";

	char* logFile = string_duplicate(path);
	string_append(&logFile, tipo_proceso_str);
	string_append(&logFile, ".log");

	remove(logFile);
	logAux = log_create(logFile, tipo_proceso_str, true, LOG_LEVEL_INFO);

	log_info(logAux, "Inicializando panel para %s, en \"%s\"", tipo_proceso_str,
			logFile);

	free(logFile);

	kernel_cpus_conectadas = list_create();
	kernel_consolas_conectadas = list_create();
}

char* ids_desde_lista(t_list* lista) {
	char* ids = string_new();

	void _escribirId(int* id) {
		string_append_with_format(&ids, "%d-", *id);
	}
	list_iterate(lista, (void*) _escribirId);

	if (!string_is_empty(ids))
		ids[strlen(ids) - 1] = '\0';
	return ids;
}

char* hilos_desde_lista(t_list* lista) {
	char* resumen = string_new();

	void _imprimirTcb(t_hilo* hilo) {
		string_append_with_format(&resumen,
				"    { PID: %d, TID: %d, KM: %d, PC: %d, M: %d (%db) }\n",
				hilo->pid, hilo->tid, hilo->kernel_mode,
				hilo->puntero_instruccion, hilo->segmento_codigo,
				hilo->segmento_codigo_size);
		string_append_with_format(&resumen,
				"      { X: %d, S: %d, A: %d, B: %d, C: %d, D: %d, E: %d }",
				hilo->base_stack, hilo->cursor_stack, hilo->registros[0],
				hilo->registros[1], hilo->registros[2], hilo->registros[3],
				hilo->registros[4]);
	}
	list_iterate(lista, (void*) _imprimirTcb);

	return resumen;
}

void conexion_cpu(uint32_t id) {
	int* num = malloc(sizeof(uint32_t));
	memcpy(num, &id, sizeof(uint32_t));
	list_add(kernel_cpus_conectadas, num);

	char* ids = ids_desde_lista(kernel_cpus_conectadas);
	log_info(logAux, "Nueva CPU (%d) => [%s]", id, ids);
	free(ids);
}
void desconexion_cpu(uint32_t id) {
	bool _esElId(int* _id) {
		return *_id == id;
	}
	list_remove_and_destroy_by_condition(kernel_cpus_conectadas,
			(void*) _esElId, free);

	char* ids = ids_desde_lista(kernel_cpus_conectadas);
	log_info(logAux, "Desconexión CPU (%d) => [%s]", id, ids);
	free(ids);
}

void conexion_consola(uint32_t id) {
	int* num = malloc(sizeof(uint32_t));
	memcpy(num, &id, sizeof(uint32_t));
	list_add(kernel_consolas_conectadas, num);

	char* ids = ids_desde_lista(kernel_consolas_conectadas);
	log_info(logAux, "Nueva Consola (%d) => [%s]", id, ids);
	free(ids);
}

void desconexion_consola(uint32_t id) {
	bool _esElId(int* _id) {
		return *_id == id;
	}
	list_remove_and_destroy_by_condition(kernel_consolas_conectadas,
			(void*) _esElId, free);

	char* ids = ids_desde_lista(kernel_consolas_conectadas);
	log_info(logAux, "Desconexión Consola (%d) => [%s]", id, ids);
	free(ids);
}

void hilos(t_list* hilos) {
	char* resumen = string_new();

	string_append(&resumen, "Hilos ejecutando:\n");

	bool _esNew(t_hilo* hilo) {
		return hilo->cola == NEW;
	}
	;
	bool _esReady(t_hilo* hilo) {
		return hilo->cola == READY;
	}
	;
	bool _esExec(t_hilo* hilo) {
		return hilo->cola == EXEC;
	}
	;
	bool _esBlock(t_hilo* hilo) {
		return hilo->cola == BLOCK;
	}
	;
	bool _esExit(t_hilo* hilo) {
		return hilo->cola == EXIT;
	}
	;

	void agregarResumen(char* nombre, bool (*filtro)(t_hilo*)) {
		string_append(&resumen, nombre);
		t_list* filtrada = list_filter(hilos, (void*) filtro);
		char* coso = hilos_desde_lista(filtrada);
		list_destroy(filtrada);
		string_append(&resumen, coso);
		free(coso);
	}

	agregarResumen("  NEW:\n", _esNew);
	agregarResumen("\n  READY:\n", _esReady);
	agregarResumen("\n  EXEC:\n", _esExec);
	agregarResumen("\n  BLOCK:\n", _esBlock);
	agregarResumen("\n  EXIT:\n", _esExit);

	log_info(logAux, resumen);
	free(resumen);
}

void instruccion_protegida(char* mnemonico, t_hilo* hilo) {
	log_info(logAux, "El hilo { PID: %d, TID: %d } ejecutó la instrucción %s",
			hilo->pid, hilo->tid, mnemonico);
}

void comienzo_ejecucion(t_hilo* hilo, uint32_t quantum) {
	char* mensaje = string_new();

	string_append_with_format(&mensaje, "Ejecuta hilo { PID: %d, TID: %d }",
			hilo->pid, hilo->tid);
	if (hilo->kernel_mode)
		string_append(&mensaje, " en modo kernel");

	log_info(logAux, mensaje);
	free(mensaje);
}

void fin_ejecucion() {
	log_info(logAux, "Empieza a estar iddle");
}

void ejecucion_instruccion(char* mnemonico, t_list* parametros) {
	char* mensaje = string_new();

	string_append_with_format(&mensaje, "Instrucción %s [", mnemonico);

	bool primero = true;
	void _imprimirParametro(char* parametro) {
		if (!primero)
			string_append(&mensaje, ", ");
		string_append_with_format(&mensaje, "%s", parametro);
		primero = false;
	}
	list_iterate(parametros, (void*) _imprimirParametro);

	string_append(&mensaje, "]");

	log_info(logAux, mensaje);
	free(mensaje);
}

void cambio_registros(t_registros_cpu registros) {
	log_info(logAux,
			"Registros: { A: %d, B: %d, C: %d, D: %d, E: %d, M: %d, P: %d, S: %d, K: %d, I: %d }",
			registros.registros_programacion[0],
			registros.registros_programacion[1],
			registros.registros_programacion[2],
			registros.registros_programacion[3],
			registros.registros_programacion[4], registros.M, registros.P,
			registros.S, registros.K, registros.I);
}

//Retrocompatibilidad con el ejemplo del enunciado:
void ejecucion_hilo(t_hilo* hilo, uint32_t quantum) {
	comienzo_ejecucion(hilo, quantum);
}

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
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
	int tidMaximo;
	int baseDatos;
	int pointerDatos;
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

void instruccionProtegida(char * instruccion, t_tcb * tcb, int tipoLista) {
	void * convertirTcb(void * tcbAux) {
		t_hilo * hilo = malloc(sizeof(t_hilo));
		t_tcb * tcb = tcbAux;
		hilo->pid = tcb->pid;
		hilo->tid = tcb->tid;
		hilo->kernel_mode = tcb->km;
		hilo->segmento_codigo = tcb->M;
		hilo->segmento_codigo_size = tcb->tam_seg_cod;
		hilo->puntero_instruccion = tcb->P;
		hilo->base_stack = tcb->X;
		hilo->cursor_stack = tcb->S;
		hilo->registros[0] = tcb->registroA.valores;
		hilo->registros[1] = tcb->registroB.valores;
		hilo->registros[2] = tcb->registroC.valores;
		hilo->registros[3] = tcb->registroD.valores;
		hilo->registros[4] = tcb->registroE.valores;
		hilo->cola = tipoLista;
		return (void *) hilo;
	}
	t_hilo * hiloTcb = convertirTcb(tcb);
	instruccion_protegida(instruccion, hiloTcb);
}

void convertirColaKM(t_list * listaMadre, t_queue * colaKM, int tipoLista) {
	void * convertirTcb(void * tcbAux) {
			t_hilo * hilo = malloc(sizeof(t_hilo));
			t_tcb * tcb = tcbAux;
			hilo->pid = tcb->pid;
			hilo->tid = tcb->tid;
			hilo->kernel_mode = tcb->km;
			hilo->segmento_codigo = tcb->M;
			hilo->segmento_codigo_size = tcb->tam_seg_cod;
			hilo->puntero_instruccion = tcb->P;
			hilo->base_stack = tcb->X;
			hilo->cursor_stack = tcb->S;
			hilo->registros[0] = tcb->registroA.valores;
			hilo->registros[1] = tcb->registroB.valores;
			hilo->registros[2] = tcb->registroC.valores;
			hilo->registros[3] = tcb->registroD.valores;
			hilo->registros[4] = tcb->registroE.valores;
			hilo->cola = tipoLista;
			return (void *) hilo;
		}
	if(queue_size(colaKM)){
		t_hilo * hiloTcb = convertirTcb(queue_pop(colaKM));
		list_add(listaMadre,hiloTcb);
	}
}

void convertirCola(t_list * listaMadre, t_list * listaTcb, int tipoLista) {
	void * convertirTcb(void * tcbAux) {
		t_hilo * hilo = malloc(sizeof(t_hilo));
		t_tcb * tcb = tcbAux;
		hilo->pid = tcb->pid;
		hilo->tid = tcb->tid;
		hilo->kernel_mode = tcb->km;
		hilo->segmento_codigo = tcb->M;
		hilo->segmento_codigo_size = tcb->tam_seg_cod;
		hilo->puntero_instruccion = tcb->P;
		hilo->base_stack = tcb->X;
		hilo->cursor_stack = tcb->S;
		hilo->registros[0] = tcb->registroA.valores;
		hilo->registros[1] = tcb->registroB.valores;
		hilo->registros[2] = tcb->registroC.valores;
		hilo->registros[3] = tcb->registroD.valores;
		hilo->registros[4] = tcb->registroE.valores;
		hilo->cola = tipoLista;
		return (void *) hilo;
	}
	t_list * listaConvertida = list_create();
	listaConvertida = list_map(listaTcb, convertirTcb);
	list_add_all(listaMadre, listaConvertida);
}

void convertirColaJoin(t_list * listaMadre, t_list * listaTcb, int tipoLista) {
	void * convertirTcb(void * nodoJoin) {
		t_hilo * hilo = malloc(sizeof(t_hilo));
		t_join * nodoJoinAux = nodoJoin;
		t_tcb * tcb = nodoJoinAux->tcb;
		hilo->pid = tcb->pid;
		hilo->tid = tcb->tid;
		hilo->kernel_mode = tcb->km;
		hilo->segmento_codigo = tcb->M;
		hilo->segmento_codigo_size = tcb->tam_seg_cod;
		hilo->puntero_instruccion = tcb->P;
		hilo->base_stack = tcb->X;
		hilo->cursor_stack = tcb->S;
		hilo->registros[0] = tcb->registroA.valores;
		hilo->registros[1] = tcb->registroB.valores;
		hilo->registros[2] = tcb->registroC.valores;
		hilo->registros[3] = tcb->registroD.valores;
		hilo->registros[4] = tcb->registroE.valores;
		hilo->cola = tipoLista;
		return (void *) hilo;
	}
	t_list * listaConvertida = list_map(listaTcb, convertirTcb);
	list_add_all(listaMadre, listaConvertida);
}

void convertirColaRecurso(t_list * listaMadre, t_list * listaTcb, int tipoLista) {
	void * convertirTcb(void * nodoRecurso) {
		t_hilo * hilo = malloc(sizeof(t_hilo));
		t_nodoRecurso * nodoRecursoAux = nodoRecurso;
		t_tcb * tcb = nodoRecursoAux->tcb;
		hilo->pid = tcb->pid;
		hilo->tid = tcb->tid;
		hilo->kernel_mode = tcb->km;
		hilo->segmento_codigo = tcb->M;
		hilo->segmento_codigo_size = tcb->tam_seg_cod;
		hilo->puntero_instruccion = tcb->P;
		hilo->base_stack = tcb->X;
		hilo->cursor_stack = tcb->S;
		hilo->registros[0] = tcb->registroA.valores;
		hilo->registros[1] = tcb->registroB.valores;
		hilo->registros[2] = tcb->registroC.valores;
		hilo->registros[3] = tcb->registroD.valores;
		hilo->registros[4] = tcb->registroE.valores;
		hilo->cola = tipoLista;
		return (void *) hilo;
	}
	t_list * listaConvertida = list_map(listaTcb, convertirTcb);
	list_add_all(listaMadre, listaConvertida);
}

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

t_reservarSegmentos * reservarStackCrea(int pid, t_tcb * tcbHijo, int stack,
		int socketMsp, int socketCpu) {

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
			enviarInt(0, socketCliente);
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
			destruirSegmento(pid, tcb->M, socketMsp);
			enviar_serializado(1,
					"Segmentation Fault al escribir el segmento de codigo",
					socketCliente);
			enviarInt(0, socketCliente);
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
			destruirSegmento(pid, tcb->M, socketMsp);
			enviar_serializado(1,
					"No hay memoria disponible para crear segmento de stack",
					socketCliente);
			enviarInt(0, socketCliente);
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

bool hayNodoJoinConElTid(t_list * lista, int tid) {	//ESTA ES PARA BUSCAR NODO CON EL TIDAESPERAR QUE CONCUERDE CON EL TID DEL PARAMETRO
	bool esElDelTid(void * nodoJoin) {
		t_join* nodoJoinAux = nodoJoin;
		return nodoJoinAux->tidAEsperar == tid;
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
		return nodoRecursoAux->idRecurso == recurso;
	}
	return list_remove_by_condition(lista, esElDelRecurso);
}

#endif /* CONSOLA_H_ */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

//prototipos funciones usadas en main
void gestionarMemoria(uint32_t,uint32_t,uint32_t,char*);

//variables globales
pthread_rwlock_t * semaforoCola_memoria;
uint32_t enteroParaComparaciones;
pthread_mutex_t * mutexComparador;
uint32_t cantidadMemoriaDisponible;
pthread_mutex_t * mutexCantidadMemoriaDisponible;
t_queue * direccionColaMarcos;
pthread_mutex_t * mutexColaMarcos;
uint32_t direccionMemoriaPrincipal;
t_list * direccionListaProcesos;

//estructuras usadas en la MSP
typedef struct{uint32_t PID;t_list direccion_tablaSegmentos}elem_tipoProceso;
typedef struct{uint32_t numSegmento;uint32_t tamanio;uint32_t direccion_tablaPaginas;}elem_tipoSegmento;
typedef struct{uint32_t exito;uint32_t direccion;}tipo_respuesta_crearSegmento;
typedef struct{uint32_t PID;uint32_t numPagina;uint32_t numSegmento;uint32_t numMarco;uint32_t bitReferencia}elem_tipoColaAuxiliar;


int main(uint32_t argc, char *agrv){
	t_config * archivoConfiguracion;
	uint32_t puerto;
	uint32_t cantidadMemoria;
	uint32_t cantidadSwap;
	char * sustPags;
	pthread_rwlock_init(semaforoCola_memoria,NULL);
	direccionColaMarcos=queue_create();
	direccionListaProcesos=list_create();
	*mutexComparador=1;
	if (argc != 2){
				printf("no ingreso suficientes argumentos");
				return 0;}
	archivoConfiguracion=config_create(argv(2));
	puerto=get_int_value(archivoConfiguracion,"PUERTO");
	cantidadMemoria=get_int_value(archivoConfiguracion,"CANTIDAD_MEMORIA");
	cantidadSwap=get_int_value(archivoConfiguracion,"CANTIDAD_SWAP");
	sustPags=get_string_value(archivoConfiguracion,"SUST_PAGS");
	pthread_rwlock_destroy(semaforoCola_memoria);
	gestionarMemoria(puerto,cantidadMemoria,cantidadSwap,sustPags);
	return 1;
}




/*
 * funcionesMemoria.c
 *
 *  Created on: 12/09/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <pthread.h>

uint32_t inicializarMarcos(uint32_t);
void consolaMSP(cantididadMemoriaDisponible,direccionMemoriaPrincipal,direccionColaMarcos,direccionListaProcesos);




void gestionarMemoria(uint32_t puerto,uint32_t cantidadMemoria,uint32_t cantidadSwap,char* algoritmo){

uint32_t cantidadMemoriaDisponible;
t_queue * direccionColaMarcos;
uint32_t direccionMemoriaPrincipal;
t_list * direccionListaProcesos;
pthread_t atenderConsola
uint32_t iret1;

 	cantidadMemoriaDisponible=cantidadMemoria*1024+cantidadSwap*1048576;
  	direccionMemoriaPrincipal=inicializarMarcos(cantidadMemoria);
	direccionColaMarcos=queue_create();
	direccionListaProcesos=list_create();
	iret1=pthread_create(&atenderConsola,NULL,consolaMSP(puerto,cantidadMemoriaDisponible,direccionMemoriaPrincipal,direccionColaMarcos,direccionListaProcesos),NULL);
			if(iret1){
						fprintf(stderr, "Fallo  creacion hilo atender consola retorno: %d\n",iret1);
						exit(1);
						}
	gestionarConexiones(puerto,cantidadMemoriaDisponible,direccionMemoriaPrincipal,direccionColaMarcos,direccionListaProcesos,algoritmo);
}

uint32_t inicializarMarcos(uint32_t cantidadMemoria){
	uint32_t memoriaPrincipal[cantidadMemoria*4][256];
	return &memoriaPrincipal[0][0];
}

void * aceptarConexiones(listenningSocket,cantididadMemoriaDisponible,direccionMemoriaPrincipal,direccionColaMarcos,direccionListaProcesos,algoritmo);


void gestionarConexiones(uint32_t puerto, uint32_t cantididadMemoriaDisponible,uint32_t direccionMemoriaPrincipal,uint32_t direccionColaMarcos,uint32_t direccionListaProcesos,char* algoritmo){
	uint32_t listenningSocket=crearServer(puerto);
	while(1){
		aceptarConexiones(listenningSocket,cantididadMemoriaDisponible,direccionMemoriaPrincipal,direccionColaMarcos,direccionListaProcesos,algoritmo);
	}

}




void * atenderConexion(void *arg){
	printf("Lalal");
}

void * aceptarConexiones(uint32_t listenningSocket,uint32_t cantididadMemoriaDisponible,uint32_t direccionMemoriaPrincipal,uint32_t direccionColaMarcos,uint32_t direccionListaProcesos,char* algoritmo){ // recibe todas las conexiones en general y crea un socket y un hilo para atenderlas

			uint32_t socketCliente;
			uint32_t iret2;
			pthread_t atenderConexion;
			socketCliente=aceptarConexion(listenningSocket);
			iret2=pthread_create(&atenderConexion,NULL,atenderConexion,NULL);
			if (iret2) {
						fprintf(stderr, "Fallo  creacion hilo atender conexion: %d\n",iret2);
						exit(1);

			}
}

void consolaMSP(uint32_t cantididadMemoriaDisponible,uint32_t direccionMemoriaPrincipal,uint32_t direccionColaMarcos,uint32_t direccionListaProcesos){
	char* opcion;

			while(1){
				printf("Menu consola: Que desea hacer? \n");
				printf("Crear segmento\nDestruir segmento\n");
				printf("Escribir Memoria\nLeer Memoria\n");
				printf("Tabla de segmentos\n Tabla de paginas");
				printf("Listar marcos \n");
				scanf("%s",&opcion);
				switch(opcion){
				case 'Crear segmento': break;
				case 'Destruir segmento': break;
				case 'Escribir memoria': break;
				case 'Leer memoria': break;
				case 'Tabla de segmentos': printf("usted quiere escribir\n");break;
				case 'Tabla de paginas': break;
				case 'listar marcos': break;
				default: printf("El codigo ingresado no es correcto\n");
				}
}


typedef struct{uint32_t PID;t_list direccion_tablaSegmentos}elem_tipoProceso;
typedef struct{uint32_t numPagina; uint32_t estado;} elem_tipoPagina;
typedef struct{uint32_t numSegmento;uint32_t tamanio;uint32_t direccion_tablaPaginas;}elem_tipoSegmento;
typedef struct{uint32_t exito;uint32_t direccion;}tipo_respuesta_crearSegmento;


uint32_t hayMemoria(uint32_t,uint32_t);
uint32_t crearSegmento(uint32_t,uint32_t,uint32_t,uint32_t);/*recibe pid, tamaño, direccion contador espacio, direccio tabla procesos*/
tipo_respuesta_crearSegmento crearSegmento(uint32_t PID,uint32_t tamanio,uint32_t direccion_espacioLibre,uin32_t direccion_tablaProcesos){
	tipo_respuesta_crearSegmento respuesta;
	if (hayMemoria(direccion_espacioLibre,tamanio)){

					uint32_t direccionBase;
					elem_tipoSegmento *tablaSegmentos;
					tablaSegmentos=tablaDeSegmentos(PID,direccion_tablaProcesos);
					direccionBase=ingresarSegmento(tablaSegmentos,tamanio);
					respuesta.exito=1;
					respuesta.direccion=direccionBase;
					return respuesta;};respuesta.exito=0;
				return respuesta;
};
uint32_t hayMemoria(uint32_t direccion_espacioLibre,uint32_t tamanio);{
	uint32_t resto;
	uint32_t *dirLibre;dirLibre=direccion_espacioLibre;
	if (tamanio mod 256!=0)tamanio=tamanio+(256-(tamanio mod 256));
	resto=(*dirLibre-tamanio);
	if (resto >= 0) return 1;return 0;
}

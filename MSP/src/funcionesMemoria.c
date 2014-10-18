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

//prototipos funciones usadas en gestionarMemoria
uint32_t inicializarMarcos(uint32_t);
void gestionarConexiones(uint32_t,uint32_t,uint32_t,uint32_t direccionColaMarcos,uint32_t direccionListaProcesos,char* algoritmo);
void consolaMSP(cantididadMemoriaDisponible,direccionMemoriaPrincipal,direccionColaMarcos,direccionListaProcesos);


void gestionarMemoria(uint32_t puerto,uint32_t cantidadMemoria,uint32_t cantidadSwap,char* algoritmo){
	pthread_t atenderConsola;
	uint32_t iret1;
	cantidadMemoriaDisponible=cantidadMemoria*1024+cantidadSwap*1048576;
  	direccionMemoriaPrincipal=inicializarMarcos(cantidadMemoria);
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

int escribirMemoria(uint32_t PID,uint32_t DireccionLogica,uint32_t BytesAEscribir,uint32_t Tamanio); //es int escribirMemoria??

void consolaMSP(uint32_t cantididadMemoriaDisponible,uint32_t direccionMemoriaPrincipal,uint32_t direccionColaMarcos,uint32_t direccionListaProcesos){
	char* opcion;

			while(1){
				printf("Menu consola: Que desea hacer? \n");
				printf("Crear segmento[PID][TAMAÑO]\nDestruir segmento[PID][DIRECCION BASE]\n");
				printf("Escribir Memoria[PID][DIRECCION LOGICA][BYTES A ESCRIBIR][TAMAÑO]\nLeer Memoria[PID][Dirección Virtual][Tamaño]\n");
				printf("Tabla de segmentos\n Tabla de paginas[PID]");
				printf("Listar marcos \n");
				scanf("%s",&opcion);
				switch(opcion){
				case 'Crear segmento': break;
				case 'Destruir segmento': break;
				case 'Escribir memoria': printf("PID: "); scanf("%d",PID);
										 printf("Direccion logica: ");	scanf("%d",DireccionLogica);
										 printf("Bytes a escribir: "); scanf("%d",BytesAEscribir);
										 printf("Tamaño: "); scanf("%d",Tamanio);
										 escribirMemoria(PID,DireccionLogica,BytesAEscribir,Tamanio);
										 break;
				case 'Leer memoria': break;
				case 'Tabla de segmentos': printf("usted quiere escribir\n");break;
				case 'Tabla de paginas': break;
				case 'listar marcos': break;
				default: printf("El codigo ingresado no es correcto\n");
				}
}



uint32_t hayMemoria(uint32_t,uint32_t);
t_list * buscarTablaDeSegmento(uint32_t);
uint32_t ingresarSegmento(t_list *,uint32_t); //ver tipo tablaSegmentos
uint32_t existeProceso(uint32_t); //anySatisfy
t_list * crearProceso(uint32_t);

tipo_respuesta_crearSegmento crearSegmento(uint32_t PID,uint32_t tamanio){
	tipo_respuesta_crearSegmento respuesta;
	uint32_t direccionBaseSegmento;
	t_list * tablaSegmentos;
		if (hayMemoria(tamanio)){
			if (existeProceso(PID)){
				tablaSegmentos=buscarTablaDeSegmento(PID);
				pthread_mutex_lock(mutexTablaSegmento); //hay que declarar el mutex
				direccionBaseSegmento=ingresarSegmento(tablaSegmentos,tamanio);
				pthread_mutex_unlock(mutexTablaSegmento);
				respuesta.exito=1;
				respuesta.direccion=direccionBaseSegmento;
				return respuesta;}
			else
				tablaSegmentos=crearProceso(PID); // hay que definirla, lo incluye en la lista de procesos y devuelve la tabla
				pthread_mutex_lock(mutexTablaSegmento); //hay que declarar el mutex
				direccionBaseSegmento=ingresarSegmento(tablaSegmentos,tamanio);
				pthread_mutex_unlock(mutexTablaSegmento);
				respuesta.exito=1;
				respuesta.direccion=direccionBaseSegmento;
				return respuesta;}
			}
			respuesta.exito=0;
			return respuesta;
	}


uint32_t hayMemoria(uint32_t tamanio){
		uint32_t resto;
		if (tamanio%256!=0){
			tamanio=tamanio+(256-(tamanio%256));}
		resto=(cantidadMemoriaDisponible-tamanio); //ver nombre de la variable global
		if (resto >= 0) return 1;
		return 0;
}




uint32_t extraerSegmento(uint32_t direccionLogica){
	return direccionLogica/1048756;
}


uint32_t extraerPagina (uint32_t direccionLogica){
	return ((direccionLogica%1048576)/256);
}

uint32_t extraerOffset (uint32_t direccionLogica){
	return (((direccionLogica%1048576)%256);
}


void modificar(char* direccionMarco,char* bytesAEscribir,int offset,int tamanio){
        uint32_t i;
		direccionMarco=direccionMarco+offset;
        for(i=0;i<=tamanio;i++){
                *direccionMarco=*bytesAEscribir;
                direccionMarco++;
                bytesAEscribir++;}
}

uint32_t llevarAMemoria(char,uint32_t,uint32_t,uint32_t);
uint32_t escribirMarco(uint32_t,uint32_t,uint32_t,uint32_t,char*,char*,uint32_t);
char* escribirPagina(uint32_t PID,uint32_t segmmento,uint32_t pagina,uint32_t offset,char* bytesAEscribir,
			char* dirTablaPaginas,uint32_t tamanio){
				uint32_t exito=0;
				while (exito!=1){
					if(*(dirTablaPagias+pagina)!='M')
		 			 	llevarAMemoria(dirTablaPagias+pagina,PID,segmento,pagina);
					exito=escribirMarco(PID,segmento,pagina,offset,bytesAEscribir,tamanio);
				}
				return (bytesAEscribir+tamanio);
}

uint32_t hayMarcoDispoible(); //hay que desarrollarla

uint32_t llevarAMemoria(char parametro,uint32_t PID, uint32_t segmento, uint32_t pagina){
	FILE* archivo;
	uint32_t exito;
	char bytes[257],nombreArchivo,numeroPid,numeroSegmento,numeroPagina; //bytes[257]??
	if(parametro=='S'){
		numeroPid=string_itoa(PID);
		numeroSegmento=string_itoa(segmento);
		numeroPagina=string_itoa(pagina);
		nombreArchivo=string_append(numeroPid,string_append("_",string_append
		                (numeroSegmento,string_append("_",string_append(numeroPagina,".txt")))));
		archivo=fopen(nombreArchivo,"rt");
		fgets(bytes,257,archivo);
		fclose(archivo);
		remove(nombreArchivo);
	}
	exito=0;
	while(exito==0){
		pthread_mutex_lock(mutexColaMacos);/*hay queagregarlo*/
		if(hayMarcoDispoible()==0)
			exito=hacerSwapEIgresar(PID,segmento,pagina,bytes);
		else
			exito=ingresar(PID,segmento,pagina,bytes);
		pthread_mutex_unlock(mutexColaMarcos);
	}
	return 1;
}


uint32_t esIgualAComparador(elem_tipoColaAuxiliar);
uint32_t escribirMarco(uint32_t PID, uint32_t segmento,uint32_t pagina,	uint32_t offset,char* bytesAEscribir,
			 						uint32_t tamanio){
		elem_tipoColaAuxiiar elementoDeCola;
		pthread_rwlock_wrlock(semaforoCola_memoria);
		pthread_mutex_lock(mutexComparador);
		enteroParaComparaciones=PID;
		if (list_any_satisfy(direccionColaMarcos.elements,esIgualAComparador){
			elementoDeCola=list_find(direccionColaMarcos.elements,esIgualAComparador);
			pthread_mutex_unlock(mutexComparador);
			modificar(elementoDeCola.numMarco,bytesAEscribir,offset,tamanio);
			pthread_rwlock_unlock(semaforoCola_memoria);
			return 1;}
		else
			pthread_mutex_unlock(mutexComparador);
			pthread_rwlock_unlock(semaforoCola_memoria);
			return0;
}

uint32_t esIgualAComparador(elem_tipoColaAxuiliar nodo){
	return (nodo.PID==enteroParaComparaciones);
}





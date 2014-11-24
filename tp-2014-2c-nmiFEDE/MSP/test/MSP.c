/*
 ============================================================================
 Name        : msp.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <commons/string.h>
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
typedef struct {uint32_t numSegmento;uint32_t tamanio;uint32_t direccion_tablaPaginas;}elem_tipoSegmento;
typedef struct {uint32_t PID;t_list *direccion_tablaSegmentos;}elem_tipoProceso;
typedef struct {uint32_t PID;uint32_t segmento;uint32_t pagina;uint32_t referencia;char* direccionMarco;int numMarco;}elem_tipoColaAuxiliar;
typedef struct {int numMarco;char * direccionMarco;}elem_colaMarcosLibres;
typedef struct {int exito;uint32_t direccion;}respuesta_t;
typedef struct{uint32_t PID;int orden;uint32_t referencia;;int numMarco;}t_list_ord;

char* puerto;
int cantidadSwap;
int cantidadMemoria;
int cantidadMemoriaDisponible;
int enteroParaComparaciones;
t_list* direccionListaProcesos;
t_queue * direccionColaMarcos;
char algoritmo;
t_queue * colaMarcosLibres;
pthread_mutex_t mutexTablaProceso;
pthread_mutex_t mutexTablaSegmento;
pthread_mutex_t mutexCantidadMemoria;
pthread_mutex_t mutexCantidadMemoriaDisponible;
pthread_mutex_t mutexColaMarcos;
pthread_mutex_t mutexLlevarAMemoria;
pthread_mutex_t mutexColaMarcosLibres;
pthread_mutex_t mutexComparador;
pthread_rwlock_t semaforoCola_memoria;

void eliminarProceso(elem_tipoProceso* proceso);
void consola();void inicializarMemoria();
uint32_t pot(uint32_t,uint32_t);int escribirMemoria(uint32_t,uint32_t,char*,uint32_t);
void modificar(char*,char*,int,int);
char* escribirPagina(uint32_t,uint32_t,uint32_t,uint32_t,char*,char*,uint32_t);
void inicializarMemoria();
uint32_t convertirANumero(char *,int);
respuesta_t solicitarMemoria(uint32_t,uint32_t,uint32_t);
void solicitarPagina(uint32_t,uint32_t,uint32_t,uint32_t,char*,char*,uint32_t);
uint32_t solicitarMarco(uint32_t,uint32_t,uint32_t,uint32_t,char*,uint32_t);
char* escribirPagina(uint32_t,uint32_t,uint32_t,uint32_t, char*,char*,uint32_t);
uint32_t escribirMarco(uint32_t,uint32_t,uint32_t,uint32_t,char*,uint32_t);
uint32_t destruirSegmento(uint32_t,uint32_t);
respuesta_t crearSegmento(uint32_t,uint32_t);
uint32_t crearTablaPaginas(uint32_t);
uint32_t ingresarSegmento(t_list *,uint32_t);
t_list * crearProceso(uint32_t);
uint32_t hayMemoria(uint32_t);
void sacarDeMemoria(uint32_t,uint32_t,uint32_t);
void borrarSwap(uint32_t,uint32_t,uint32_t);
respuesta_t obtenerTablaSegmentos(uint32_t);
respuesta_t entraYDirTablaPags(t_list *,uint32_t,uint32_t);
int entra(uint32_t,uint32_t,uint32_t,uint32_t);
void llevarAMemoria(uint32_t,uint32_t,uint32_t);
int hayMarcoDisponible();
void hacerSwap();void listarMarcos();
void LRU();void CLOCK();
void ingresar(uint32_t,uint32_t,uint32_t,char*);
void modificar(char*,char*,int,int);void modificarAlt(char*,char*,int,int);
bool esLaPagina(elem_tipoColaAuxiliar*);
uint32_t esElSegmento(elem_tipoSegmento *);
uint32_t esElPID(elem_tipoProceso *);
int extraerSegmento(int);
int extraerOffset(int);uint32_t recibirInt32();
int extraerPagina(int);void gestionarConexiones();
uint32_t imprimirTablaSegmentos();
uint32_t imprimirTablaPaginas();
void aceptarConexiones(uint32_t);
int recibirInt(int);
void enviarInt32(uint32_t,int);
void enviarInt(int,int);char * recibirBeso(int,int);void enviarBeso(int,char *,int);
int aceptarConexion(int);int crearServer(char *);void gestionarMemoria();void imprimirInformacionProceso(elem_tipoProceso*);

void imprimirInformacionSegmento(elem_tipoSegmento*);
void imprimirInformacionPaginas();
bool marcoMenor(t_list_ord*,t_list_ord*);



int main(void) {t_config * archivoConfiguracion;
 	archivoConfiguracion=config_create("archiconf/MSP.cfg");
	puerto=config_get_string_value(archivoConfiguracion,"PUERTO");
	cantidadMemoria=config_get_int_value(archivoConfiguracion,"CANTIDAD_MEMORIA");
	cantidadSwap=config_get_int_value(archivoConfiguracion,"CANTIDAD_SWAP");
	if(!(strcmp(config_get_string_value(archivoConfiguracion,"SUST_PAGS"),"LRU"))) algoritmo='L';
	else algoritmo='C';inicializarMemoria();
	gestionarMemoria();
	return 1;
}

void inicializarCantidadMemoriaDisponible();
void inicializarSemaforos();
void crearTablaProcesos();
void inicializarMarcos();
void inicializarMemoria(){
	inicializarMarcos();
	inicializarCantidadMemoriaDisponible();
	inicializarSemaforos();
	crearTablaProcesos();
}

void inicializarMarcos(){
	char* marco;
	int i;
	elem_colaMarcosLibres * auxiliar;
	colaMarcosLibres=queue_create();
	direccionColaMarcos=queue_create();
	for(i=0;i!=(cantidadMemoria*4);i++){auxiliar=malloc(8);
	marco=malloc(256);
	auxiliar->numMarco=i;
	auxiliar->direccionMarco=marco;
	queue_push(colaMarcosLibres,auxiliar);};
}

void inicializarSemaforos(){
pthread_mutex_init(&mutexCantidadMemoriaDisponible,NULL);
pthread_mutex_init(&mutexCantidadMemoria,NULL);
pthread_mutex_init(&mutexLlevarAMemoria,NULL);
pthread_mutex_init(&mutexColaMarcosLibres,NULL);
pthread_mutex_init(&mutexComparador,NULL);
pthread_mutex_init(&mutexColaMarcos,NULL);
pthread_mutex_init(&mutexTablaSegmento,NULL);
pthread_mutex_init(&mutexTablaProceso,NULL);
pthread_rwlock_init(&semaforoCola_memoria,NULL);
}


void crearTablaProcesos(){
	direccionListaProcesos=list_create();
}

void inicializarCantidadMemoriaDisponible(){
	cantidadMemoriaDisponible=cantidadMemoria*1024+cantidadSwap*1048576;}









void gestionarMemoria(){


pthread_t atenderConexiones;
uint32_t iret1;


 	iret1=pthread_create(&atenderConexiones,NULL,gestionarConexiones,NULL);
			if(iret1){
						fprintf(stderr, "Fallo  creacion hilo atender consola retorno: %d\n",iret1);
						exit(1);
						}
	consola();

}






void gestionarConexiones(){
	uint32_t listenningSocket=crearServer(puerto);
	while(1){
		aceptarConexiones(listenningSocket);
	}

}

void  atenderConection(int);




void  aceptarConexiones(uint32_t listenningSocket){ // recibe todas las conexiones en general y crea un socket y un hilo para atenderlas

			uint32_t socketCliente;
			uint32_t iret2;
			pthread_t atenderConexion;
			socketCliente=aceptarConexion(listenningSocket);
			iret2=pthread_create(&atenderConexion,NULL,atenderConection,socketCliente);
			if (iret2) {
						fprintf(stderr, "Fallo  creacion hilo atender conexion: %d\n",iret2);
						exit(1);

			}
}

void  atenderConection(int socketCliente){
	int flag;
	int i;
	int recibido;
	flag=1;
	recibido=recv(socketCliente, &i, sizeof(int), 0);
	if((recibido==-1)){
		printf("Fallo el recv de recibirInt\n");
	flag=0;}
	if(recibido==0){puts("hubo desconexion");
	flag=0;
}


while(flag){



if(i==1){ int PID;int tamanio;respuesta_t respuesta;
		PID=recibirInt(socketCliente);
		tamanio=recibirInt(socketCliente);
		respuesta=crearSegmento(PID,tamanio);
		enviarInt(respuesta.exito,socketCliente);
		enviarInt32(respuesta.direccion,socketCliente);}
if(i==2){int PID; uint32_t direccion;
		PID=recibirInt(socketCliente);
		direccion=recibirInt32(socketCliente);
		enviarInt(destruirSegmento(PID,direccion),socketCliente);}
if(i==3){int i;int PID; uint32_t direccion;int tamanio; char* bytes;
		PID=recibirInt(socketCliente);
		direccion=recibirInt32(socketCliente);//fede lo tiene que mandar como int32
		tamanio=recibirInt(socketCliente);
		bytes=	recibirBeso(tamanio, socketCliente);
		i=escribirMemoria(PID,direccion,bytes,tamanio);
		if(i<1) enviarInt(0,socketCliente);
		else enviarInt(1,socketCliente);}
if(i==4){int PID;uint32_t direccion;int tamanio;respuesta_t respuesta;char* algo;
		PID=recibirInt(socketCliente);
		direccion=recibirInt32(socketCliente);//fede lo tiene que mandar como int32
		tamanio=recibirInt(socketCliente);
		respuesta=solicitarMemoria(PID,direccion,tamanio);
		if(respuesta.exito<1)enviarInt(-1,socketCliente);
		else enviarInt(1,socketCliente);algo=respuesta.direccion;
		enviarBeso(tamanio,algo,socketCliente);}

recibido = recv(socketCliente, &i, sizeof(int), 0);
	if (( recibido== -1)){
		printf("Fallo el recv de recibirInt\n");
	flag=0;}
if(recibido==0){puts("hubo desconexion");
flag=0;
}

}}


void consola(){
	char * charAux;
	char * opcion;
	char * aux;
	char * texto;
	int recorrerTextoSolicitadoMemoria;
	int respuestaEntera;
	respuesta_t respuesta;
	uint32_t PID;
	uint32_t tamanio;
	uint32_t direccionBase;
	uint32_t direccionVirtual;
	int pos;
	int pos2;
	opcion=malloc(100);
	aux=malloc(40);
	texto=malloc(40);
	int sigueCorriendo=1;
	while(sigueCorriendo){
	inicio:
	pos=0;
	pos2=0;
	printf("CONSOLA MSP: \n  CREAR.SEGMENTO[PID][Tamaño]\n  DESTRUIR.SEGMENTO[PID][Dirección Base]\n");
	printf("  ESCRIBIR.MEMORIA[PID][Dirección Virtual][Tamaño][Texto]\n  LEER.MEMORIA[PID][Dirección Virtual][Tamaño]\n");
	printf("  TABLA.SEGMENTOS\n  TABLA.PAGINAS[PID]\n  LISTAR.MARCOS\n  MEMORIA.LIBRE \n");
	printf("  TERMINAR.MSP \n");
	scanf("%s",opcion);
	if (!(strcmp(opcion,"TERMINAR.MSP"))) {
		/*pthread_mutex_lock(&mutexTablaProceso);
		if (!list_is_empty(direccionListaProcesos)) {list_iterate(direccionListaProcesos,eliminarProceso);}
		list_destroy(direccionListaProcesos);
		pthread_mutex_unlock(&mutexTablaProceso);*/
		sigueCorriendo=0;
	}
	if (!(strcmp(opcion,"LISTAR.MARCOS"))) {
			listarMarcos();

		}
	if (!(strcmp(opcion,"TABLA.SEGMENTOS"))) {
				imprimirTablaSegmentos(direccionListaProcesos);
			}
	if(!(strcmp(opcion,"MEMORIA.LIBRE"))){
	pthread_mutex_lock(&mutexCantidadMemoriaDisponible);
	printf("Cantidad memoria: %u \n", cantidadMemoriaDisponible);
	pthread_mutex_unlock(&mutexCantidadMemoriaDisponible);
	}
	if (strcmp(opcion,"TABLA.SEGMENTOS")&&strcmp(opcion,"LISTAR.MARCOS")&&strcmp(opcion,"TERMINAR.MSP")&&strcmp(opcion,"MEMORIA.LIBRE")){
	while(!(opcion[pos]=='[')&&!(opcion[pos]=='\0')){
			aux[pos]=opcion[pos];
			pos++;
	}
	if (opcion[pos]=='\0') { puts("COMANDO INVALIDO \n"); goto inicio; }
	aux[pos]='\0';
	pos++;
	if (!(strcmp(aux,"TABLA.PAGINAS"))) {
			while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
				aux[pos2]=opcion[pos];
				pos2++;
				pos++;
			}
			if (opcion[pos]=='\0') {puts ("NO CERRO EL CORCHETE\n"); goto inicio;}
			PID=convertirANumero(aux,pos2);
			printf("PID %u\n",PID);
			imprimirTablaPaginas(PID);
	}
	if (!(strcmp(aux,"LEER.MEMORIA"))) {
			while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
				aux[pos2]=opcion[pos];
				pos2++;
				pos++;
			}
			if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
			PID=convertirANumero(aux,pos2);
			pos2=0;
			pos++;
			pos++;
			while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
				aux[pos2]=opcion[pos];
				pos2++;
				pos++;
			}
			if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
			direccionVirtual=convertirANumero(aux,pos2);
			pos2=0;
			pos++;
			pos++;
			while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
				aux[pos2]=opcion[pos];
				pos2++;
				pos++;
			}
			if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
			tamanio=convertirANumero(aux,pos2);
			printf("PID %u Dir Vir %u tamanio %u\n",PID,direccionVirtual,tamanio);
			respuesta=solicitarMemoria(PID,direccionVirtual,tamanio);
			if (respuesta.exito==1) {
				charAux=respuesta.direccion;
				for(recorrerTextoSolicitadoMemoria=0;recorrerTextoSolicitadoMemoria<tamanio;recorrerTextoSolicitadoMemoria++){
					printf("%c",*charAux);
					charAux++;
				}
				printf("\n");
			} else {
				//if (respuesta.exito==) // que otros casos de respuesta hay?
			}
	} //fin if leer memoria
	if (!(strcmp(aux,"CREAR.SEGMENTO"))) {
		while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
			aux[pos2]=opcion[pos];
			pos2++;
			pos++;
		}
		if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
		PID=convertirANumero(aux,pos2);
		pos2=0;
		pos++;
		pos++;
		while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
			aux[pos2]=opcion[pos];
			pos2++;
			pos++;
		}
		if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
		tamanio=convertirANumero(aux,pos2);
		printf("PID %u TAM %u\n",PID,tamanio);
		respuesta=crearSegmento(PID,tamanio);
		if (respuesta.exito==1){printf("Direccion: %u\n", respuesta.direccion);}
		if (respuesta.exito==0){printf("No hay suficiente memoria\n");}
		if (respuesta.exito==-1){printf("Tamaño supera el maximo permitido por segmento\n");}
	}
	if (!(strcmp(aux,"DESTRUIR.SEGMENTO"))) {
		while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
			aux[pos2]=opcion[pos];
			pos2++;
			pos++;
		}
		if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
		PID=convertirANumero(aux,pos2);
		pos2=0;
		pos++;
		pos++;
		while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
			aux[pos2]=opcion[pos];
			pos2++;
			pos++;
		}
		if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
		direccionBase=convertirANumero(aux,pos2);
		printf("PID %u Dir %u\n",PID,direccionBase);
		respuestaEntera=destruirSegmento(PID,direccionBase);
		if (respuestaEntera== 0){
			printf("No existe el proceso\n");
		}
		else{
			if (respuestaEntera==-1) {
				printf("No existe el segmento \n");
			}
		}
	}
	if (!(strcmp(aux,"ESCRIBIR.MEMORIA"))) {
			while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
				aux[pos2]=opcion[pos];
				pos2++;
				pos++;
			}
			if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
			PID=convertirANumero(aux,pos2);
			pos2=0;
			pos++;
			pos++;
			while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
				aux[pos2]=opcion[pos];
				pos2++;
				pos++;
			}
			if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
			direccionVirtual=convertirANumero(aux,pos2);
			pos2=0;
			pos++;
			pos++;
			while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
				aux[pos2]=opcion[pos];
				pos2++;
				pos++;
			}
			if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
			tamanio=convertirANumero(aux,pos2);
			pos2=0;
			pos++;
			pos++;
			while(!(opcion[pos]==']')&&!(opcion[pos]=='\0')){
				aux[pos2]=opcion[pos];
				pos2++;
				pos++;
			}
			if (opcion[pos]=='\0') {puts("NO CERRO EL CORCHETE\n"); goto inicio;}
			strncpy(texto,aux,pos2);
			printf("PID %u Dir %u Tamanio %u texto %s\n",PID,direccionVirtual,tamanio,texto);
			respuestaEntera=escribirMemoria(PID,direccionVirtual,texto,tamanio);
			if(respuestaEntera==0){puts("no entra en el segmento");}
			if (respuestaEntera==-1) {printf("No existe el Proceso");}
			else {if (respuestaEntera==-2) {printf("No existe el segmento solicitado");}
			     }

	}
	}

	if (!sigueCorriendo) {free(opcion);free(aux);free(texto);}
	} //del while 1
}

void eliminarSegmento(elem_tipoSegmento* segmento);

void eliminarProceso(elem_tipoProceso* proceso){
	pthread_mutex_lock(&mutexComparador);
	enteroParaComparaciones=proceso->PID;
	list_iterate(proceso->direccion_tablaSegmentos,eliminarSegmento);
	pthread_mutex_unlock(&mutexComparador);
	free(proceso);
}


void eliminarSegmento(elem_tipoSegmento* segmento){
		destruirSegmento(enteroParaComparaciones,segmento->numSegmento*1048576); //como obtener dirreccion base
}




uint32_t convertirANumero(char * txt,int longi){
			uint32_t num=0;
			uint32_t numAux;
			uint32_t multiplicador=pot(10,(longi-1));
			int pos;
			for(pos=0;pos<longi;pos++){
			switch(txt[pos]){
				case '0': numAux=0; break;
				case '1': numAux=1; break;
				case '2': numAux=2; break;
				case '3': numAux=3; break;
				case '4': numAux=4; break;
				case '5': numAux=5; break;
				case '6': numAux=6; break;
				case '7': numAux=7; break;
				case '8': numAux=8; break;
				case '9': numAux=9; break;
			}
			num=num+numAux*multiplicador;
			multiplicador=multiplicador/10;
			}
			return num;
}


uint32_t pot(uint32_t base,uint32_t expo){
	int i;
	uint32_t aux=1;

	for(i=1;i<=expo;i++){
		aux=aux*base;
	}
	return aux;
}


respuesta_t solicitarMemoria(uint32_t PID,uint32_t direccion,uint32_t tamanio){
	int i;char* buffer;uint32_t posicionInicial;t_list* auxiliar;
	respuesta_t  respuestaFinal;respuesta_t respuesta_tablaSegmentos;respuesta_t respuesta_entra;
	uint32_t segmento; uint32_t pagina;char *dirTablaPaginas;
	buffer=malloc(tamanio);
	respuesta_tablaSegmentos=obtenerTablaSegmentos(PID);
	if(!respuesta_tablaSegmentos.exito) {respuestaFinal.exito= -1;return respuestaFinal;};//exit, no exite PID(seg fault)
	auxiliar=respuesta_tablaSegmentos.direccion;
	respuesta_entra=entraYDirTablaPags(auxiliar,direccion,tamanio);
	if(respuesta_entra.exito==-1){respuestaFinal.exito= -2;return respuestaFinal;}//NO EXISTE EL SEGMENTO NPARA EL PID(seg fault)
	if(respuesta_entra.exito==0){respuestaFinal.exito= 0;return respuestaFinal;}//excede el limite de segmento(seg fault)
	segmento=extraerSegmento(direccion);
	pagina=extraerPagina(direccion);
	direccion=extraerOffset(direccion);//direccion se usa como offset
	dirTablaPaginas=respuesta_entra.direccion;
	if(tamanio<=256-direccion){
		solicitarPagina(PID,segmento,pagina,direccion,buffer,dirTablaPaginas,tamanio);
		respuestaFinal.exito= 1;
		respuestaFinal.direccion=buffer;
		return respuestaFinal;};
	solicitarPagina(PID,segmento,pagina,direccion,buffer,dirTablaPaginas,256-direccion);
	tamanio=tamanio-256+direccion;
	posicionInicial=buffer;
	buffer=buffer+256-direccion;
	pagina++;
	for(i=0;i!=tamanio/256;i++){
	    solicitarPagina(PID,segmento,pagina,0,buffer,dirTablaPaginas,256);
		buffer=buffer+256;
		pagina++;};
	if((tamanio%256)==0){
		solicitarPagina(PID,segmento,pagina,direccion,buffer,dirTablaPaginas,tamanio);
		respuestaFinal.exito= 1;
		respuestaFinal.direccion=posicionInicial;
		return respuestaFinal;};
		solicitarPagina(PID,segmento,pagina,0,buffer,dirTablaPaginas,tamanio%256);
		respuestaFinal.exito= 1;
		respuestaFinal.direccion=posicionInicial;
		return respuestaFinal;}



void solicitarPagina(uint32_t PID,uint32_t segmento,uint32_t pagina
						,uint32_t offset, char* buffer,char* dirTablaPaginas,
						uint32_t tamanio){
	if(solicitarMarco(PID,segmento,pagina,offset,buffer,tamanio))return ;//page fault
	pthread_mutex_lock(&mutexLlevarAMemoria);
	if(*(dirTablaPaginas+pagina)!='M'){
		llevarAMemoria(PID,segmento,pagina);
		*(dirTablaPaginas+pagina)='M';
		};
	pthread_mutex_unlock(&mutexLlevarAMemoria);//hay que agregar el mutex Llevar a memoria
	solicitarMarco(PID,segmento,pagina,offset,buffer,tamanio);
	}




uint32_t solicitarMarco(uint32_t PID, uint32_t segmento,uint32_t pagina,
							uint32_t offset,char* buffer,
			 				uint32_t tamanio){	elem_tipoColaAuxiliar *elementoDeCola; /*xiliar es un elemento de la cola d marcos REVISAR*/
	pthread_rwlock_rdlock(&semaforoCola_memoria);
	pthread_mutex_lock(&mutexComparador);
	enteroParaComparaciones=PID+256*pagina+1048576*segmento;
	if (list_is_empty(direccionColaMarcos->elements)) {pthread_rwlock_unlock(&semaforoCola_memoria);pthread_mutex_unlock(&mutexComparador);return 0;}
	if (list_any_satisfy(direccionColaMarcos->elements,esLaPagina)){
		elementoDeCola=list_find(direccionColaMarcos->elements,esLaPagina);
		pthread_mutex_unlock(&mutexComparador);
		modificarAlt(buffer,elementoDeCola->direccionMarco,offset,tamanio);
		pthread_mutex_lock(&mutexColaMarcos);
		if(algoritmo=='L'){
			pthread_mutex_lock(&mutexComparador);enteroParaComparaciones=PID+256*pagina+1048576*segmento;
			list_remove_by_condition(direccionColaMarcos->elements,esLaPagina);
			pthread_mutex_unlock(&mutexComparador);
			queue_push(direccionColaMarcos,elementoDeCola);}
		else
			elementoDeCola->referencia=1;//no se como se llama el campo de bit referencia
		pthread_rwlock_unlock(&semaforoCola_memoria);
		pthread_mutex_unlock(&mutexColaMarcos);
		return 1;};
	pthread_rwlock_unlock(&semaforoCola_memoria);
	pthread_mutex_unlock(&mutexComparador);
	return 0;}



int escribirMemoria(uint32_t PID,uint32_t direccion,char* bytes,uint32_t tamanio){
	int i;respuesta_t respuesta_tablaSegmentos;respuesta_t respuesta_entra;
	uint32_t segmento; uint32_t pagina;char *dirTablaPaginas;t_list* auxiliar;
	respuesta_tablaSegmentos=obtenerTablaSegmentos(PID);
	if(!respuesta_tablaSegmentos.exito) {return -1;};auxiliar=respuesta_tablaSegmentos.direccion;//exit, no exite PID
	respuesta_entra=entraYDirTablaPags(auxiliar,direccion,tamanio);
	if(respuesta_entra.exito==-1){return -2;}//NO EXISTE EL SEGMENTO NPARA EL PID
	if(respuesta_entra.exito==0){return 0;}//creo que seg fault(excede el limite de segmento)
	segmento=extraerSegmento(direccion);
	pagina=extraerPagina(direccion);
	direccion=extraerOffset(direccion);//direccion se usa como offset
	dirTablaPaginas=respuesta_entra.direccion;
	if(tamanio<=256-direccion){
		bytes=escribirPagina(PID,segmento,pagina,direccion,bytes,dirTablaPaginas,tamanio);return 1;};
	bytes=escribirPagina(PID,segmento,pagina,direccion,bytes,dirTablaPaginas,256-direccion);
	tamanio=tamanio-256+direccion;
	pagina++;
	for(i=0;i!=tamanio/256;i++){
	    	bytes=escribirPagina(PID,segmento,pagina,0,bytes,dirTablaPaginas,256);
		pagina++;};
	if((tamanio%256)==0) return 1;
	bytes=escribirPagina(PID,segmento,pagina,0,bytes,dirTablaPaginas,tamanio%256);
	return 1;}


char* escribirPagina(uint32_t PID,uint32_t segmento,uint32_t pagina
						,uint32_t offset, char* bytesAEscribir,char* dirTablaPaginas,
						uint32_t tamanio){
	if(escribirMarco(PID,segmento,pagina,offset,bytesAEscribir,tamanio))return (bytesAEscribir+tamanio);//page fault
	pthread_mutex_lock(&mutexLlevarAMemoria);
	if(*(dirTablaPaginas+pagina)!='M'){
		llevarAMemoria(PID,segmento,pagina);
		*(dirTablaPaginas+pagina)='M';
		};
	pthread_mutex_unlock(&mutexLlevarAMemoria);//hay que agregar el mutex Llevar a memoria
	escribirMarco(PID,segmento,pagina,offset,bytesAEscribir,tamanio);
	return (bytesAEscribir+tamanio);}





uint32_t escribirMarco(uint32_t PID, uint32_t segmento,uint32_t pagina,
							uint32_t offset,char* bytesAEscribir,
			 				uint32_t tamanio){	elem_tipoColaAuxiliar *elementoDeCola; /*xiliar es un elemento de la cola d marcos REVISAR*/
	pthread_rwlock_wrlock(&semaforoCola_memoria);
	pthread_mutex_lock(&mutexComparador);
	enteroParaComparaciones=PID+256*pagina+1048576*segmento;
	if (list_is_empty(direccionColaMarcos->elements)) {pthread_rwlock_unlock(&semaforoCola_memoria);pthread_mutex_unlock(&mutexComparador);return 0;}
	if (list_any_satisfy(direccionColaMarcos->elements,esLaPagina)){
		elementoDeCola=list_find(direccionColaMarcos->elements,esLaPagina);
		pthread_mutex_unlock(&mutexComparador);
		modificar(elementoDeCola->direccionMarco,bytesAEscribir,offset,tamanio);
		pthread_mutex_lock(&mutexColaMarcos);
		pthread_mutex_lock(&mutexColaMarcosLibres);
		if(algoritmo=='L'){
			pthread_mutex_lock(&mutexComparador);enteroParaComparaciones=PID+256*pagina+1048576*segmento;
			list_remove_by_condition(direccionColaMarcos->elements,esLaPagina);
			pthread_mutex_unlock(&mutexComparador);
			queue_push(direccionColaMarcos,elementoDeCola);}
		else
			elementoDeCola->referencia=1;//no se como se llama el campo de bit referencia
		pthread_rwlock_unlock (&semaforoCola_memoria);
		pthread_mutex_unlock(&mutexColaMarcos);
		pthread_mutex_unlock(&mutexColaMarcosLibres);
		return 1;};
	pthread_rwlock_unlock(&semaforoCola_memoria);
	pthread_mutex_unlock(&mutexComparador);
	return 0;}

uint32_t destruirSegmento(uint32_t PID,uint32_t baseSegmento){
	uint32_t numSegmento=baseSegmento/1048576;
	elem_tipoProceso * proceso;
	elem_tipoSegmento * segmento;
	int cantDePags;
	int i;

	pthread_mutex_lock(&mutexComparador);
	enteroParaComparaciones=PID;
	pthread_mutex_lock(&mutexTablaProceso);
	if(list_is_empty(direccionListaProcesos)){pthread_mutex_unlock(&mutexTablaProceso);
	pthread_mutex_unlock(&mutexComparador);return 0;}
	proceso=list_find(direccionListaProcesos,esElPID);
	pthread_mutex_unlock(&mutexTablaProceso);
	pthread_mutex_unlock(&mutexComparador);
	if (proceso==NULL) {return 0;} //no existia el proceso
	pthread_mutex_lock(&mutexComparador);
	enteroParaComparaciones=numSegmento;
	pthread_mutex_lock(&mutexTablaSegmento);
	segmento=list_remove_by_condition(proceso->direccion_tablaSegmentos,esElSegmento);
	pthread_mutex_unlock(&mutexTablaSegmento);
	pthread_mutex_unlock(&mutexComparador);
	if (segmento==NULL){return -1;};
	i=segmento->tamanio;
	if(segmento->tamanio%256!=0)i=i+256-(i%256);
	cantDePags=i/256;
	pthread_mutex_lock(&mutexLlevarAMemoria);
	for(i=0;i!=cantDePags;i++){
		if(segmento->direccion_tablaPaginas+i=='M') sacarDeMemoria(PID,numSegmento,i);//faltaba hacerlas
		if(segmento->direccion_tablaPaginas+i=='S') borrarSwap(PID,numSegmento,i);}//faltaba hacerlas
	free(segmento->direccion_tablaPaginas);
	pthread_mutex_unlock(&mutexLlevarAMemoria);
	pthread_mutex_lock(&mutexCantidadMemoriaDisponible);
	cantidadMemoriaDisponible=cantidadMemoriaDisponible+i;
	pthread_mutex_unlock(&mutexCantidadMemoriaDisponible);
	free(segmento);
	if(list_is_empty(proceso->direccion_tablaSegmentos)){
		pthread_mutex_lock(&mutexComparador);
		enteroParaComparaciones=PID;
		pthread_mutex_lock(&mutexTablaProceso);
		list_remove_by_condition(direccionListaProcesos,esElPID);
		pthread_mutex_unlock(&mutexTablaProceso);
		pthread_mutex_unlock(&mutexComparador);
		list_destroy(proceso->direccion_tablaSegmentos);
		free(proceso);}
		return 1;
}




respuesta_t crearSegmento(uint32_t PID,uint32_t tamanio){
	elem_tipoProceso * proceso;
	uint32_t tamanioAux;
	respuesta_t respuesta;
	uint32_t direccionBaseSegmento;
	t_list * tablaSegmentos;
		if(tamanio>1048576){respuesta.exito=-1;return respuesta;}
		if (hayMemoria(tamanio)){
			pthread_mutex_lock(&mutexComparador);
			enteroParaComparaciones=PID;
			pthread_mutex_lock(&mutexTablaProceso);
			if(list_is_empty(direccionListaProcesos)){pthread_mutex_unlock(&mutexTablaProceso);
				pthread_mutex_unlock(&mutexComparador);
				tablaSegmentos=crearProceso(PID);
								pthread_mutex_lock(&mutexTablaSegmento); //hay que declarar el mutex
								direccionBaseSegmento=ingresarSegmento(tablaSegmentos,tamanio);
								pthread_mutex_unlock(&mutexTablaSegmento);
								respuesta.exito=1;
								respuesta.direccion=direccionBaseSegmento;
								tamanioAux=tamanio;
								if (tamanio%256!=0){
											tamanioAux=tamanio+(256-(tamanio%256));}
								pthread_mutex_lock(&mutexCantidadMemoriaDisponible);
								cantidadMemoriaDisponible=cantidadMemoriaDisponible-tamanioAux;
								pthread_mutex_unlock(&mutexCantidadMemoriaDisponible);
								return respuesta;}//declarar el mutex
			proceso=list_find(direccionListaProcesos,esElPID);
			pthread_mutex_unlock(&mutexTablaProceso);
			pthread_mutex_unlock(&mutexComparador);
			if (proceso != NULL){
				tablaSegmentos=proceso->direccion_tablaSegmentos;
				pthread_mutex_lock(&mutexTablaSegmento); //hay que declarar el mutex
				direccionBaseSegmento=ingresarSegmento(tablaSegmentos,tamanio);
				pthread_mutex_unlock(&mutexTablaSegmento);
				if(direccionBaseSegmento==NULL){respuesta.exito=0; return respuesta;}
				respuesta.exito=1;
				respuesta.direccion=direccionBaseSegmento;
				tamanioAux=tamanio;
				if (tamanio%256){
							tamanioAux=tamanio+(256-(tamanio%256));}
				pthread_mutex_lock(&mutexCantidadMemoriaDisponible);
				cantidadMemoriaDisponible=cantidadMemoriaDisponible-tamanioAux;
				pthread_mutex_unlock(&mutexCantidadMemoriaDisponible);
				return respuesta;}
			else {
				tablaSegmentos=crearProceso(PID);
				pthread_mutex_lock(&mutexTablaSegmento); //hay que declarar el mutex
				direccionBaseSegmento=ingresarSegmento(tablaSegmentos,tamanio);
				pthread_mutex_unlock(&mutexTablaSegmento);
				respuesta.exito=1;
				respuesta.direccion=direccionBaseSegmento;
				tamanioAux=tamanio;
				if (tamanio%256!=0){
							tamanioAux=tamanio+(256-(tamanio%256));}
				pthread_mutex_lock(&mutexCantidadMemoriaDisponible);
				cantidadMemoriaDisponible=cantidadMemoriaDisponible-tamanioAux;
				pthread_mutex_unlock(&mutexCantidadMemoriaDisponible);
				return respuesta;}
			}
			respuesta.exito=0;
			return respuesta;
	}


uint32_t ingresarSegmento(t_list * tablaSegmentos,uint32_t tamanio){
		int tamanioAux;elem_tipoSegmento * nuevoSegmento;
		nuevoSegmento=malloc(12);nuevoSegmento->numSegmento=list_size(tablaSegmentos);
		if(!(nuevoSegmento->numSegmento-4096)){free(nuevoSegmento); return NULL;}
		if (tamanio%256!=0)tamanioAux=tamanio+(256-(tamanio%256));
		else  tamanioAux=tamanio;
		nuevoSegmento->tamanio=tamanio;
		nuevoSegmento->direccion_tablaPaginas=crearTablaPaginas(tamanioAux);
		list_add(tablaSegmentos,nuevoSegmento);
		return 1048576*nuevoSegmento->numSegmento;
}

uint32_t crearTablaPaginas(uint32_t tam){
	pthread_mutex_lock(&mutexLlevarAMemoria);
	uint32_t cont;
	char* dirTablaAux;
	dirTablaAux=malloc(tam/256);
	for(cont=0;cont<(tam/256);cont++){
		*(dirTablaAux+cont)='N';
	}
	pthread_mutex_unlock(&mutexLlevarAMemoria);
	return dirTablaAux;
}





t_list * crearProceso(uint32_t PID){ //hay que pedir semaforo
	elem_tipoProceso * proceso;

	proceso=malloc(8);
	proceso->PID=PID;
	proceso->direccion_tablaSegmentos=list_create();
	pthread_mutex_lock(&mutexTablaProceso); //declararlo como global
	list_add(direccionListaProcesos,proceso);
	pthread_mutex_unlock(&mutexTablaProceso);
	return proceso->direccion_tablaSegmentos;
}


uint32_t hayMemoria(uint32_t tamanio){
	if (tamanio%256!=0){
			tamanio=tamanio+(256-(tamanio%256));
		}pthread_mutex_lock(&mutexCantidadMemoriaDisponible);
	if (cantidadMemoriaDisponible >= tamanio) {pthread_mutex_unlock(&mutexCantidadMemoriaDisponible);return 1;}
	pthread_mutex_unlock(&mutexCantidadMemoriaDisponible);return 0;
}



void sacarDeMemoria(uint32_t PID, uint32_t segmento, uint32_t pagina){
	elem_tipoColaAuxiliar *nodo;
	pthread_mutex_lock(&mutexColaMarcos);
	pthread_rwlock_wrlock(&semaforoCola_memoria);
	pthread_mutex_lock(&mutexComparador);
	enteroParaComparaciones=PID+256*pagina+1048576*segmento;
	nodo=list_remove_by_condition(direccionColaMarcos->elements,esLaPagina);
	pthread_mutex_unlock(&mutexColaMarcos);pthread_mutex_unlock(&mutexComparador);
	pthread_rwlock_unlock(&semaforoCola_memoria);
	pthread_mutex_lock(&mutexColaMarcosLibres);
	queue_push(colaMarcosLibres,nodo->numMarco);
	pthread_mutex_unlock(&mutexColaMarcosLibres);
	}




void borrarSwap(uint32_t PID, uint32_t segmento, uint32_t pagina){
char* nombreArchivo;
	nombreArchivo=string_new();
	string_append_with_format(&nombreArchivo,"archivos/%u_%u_%u.txt",PID,segmento,pagina);
	remove(nombreArchivo);}


respuesta_t obtenerTablaSegmentos(uint32_t PID){
	elem_tipoProceso * someProceso;
	respuesta_t respuestaAux;
	pthread_mutex_lock(&mutexComparador);
	pthread_mutex_lock(&mutexTablaProceso);//FALTA AGREGARLO CREO
	enteroParaComparaciones=PID;
	if(list_is_empty(direccionListaProcesos)){pthread_mutex_unlock(&mutexTablaProceso);
					pthread_mutex_unlock(&mutexComparador);respuestaAux.exito=0;return respuestaAux;}
	someProceso=list_find(direccionListaProcesos,esElPID);
	pthread_mutex_unlock(&mutexTablaProceso);
	pthread_mutex_unlock(&mutexComparador);
	if (someProceso==NULL) {respuestaAux.exito=0;}
	else {
		respuestaAux.exito=1;
		respuestaAux.direccion=someProceso->direccion_tablaSegmentos;
	}
	return respuestaAux;
}

respuesta_t entraYDirTablaPags(t_list * tablaSegmentos,uint32_t direccion,uint32_t tamanio){
	uint32_t numeroSegmentoBuscado=extraerSegmento(direccion);
	elem_tipoSegmento * someSegment;
	respuesta_t respuestaAux;
	pthread_mutex_lock(&mutexComparador);
	pthread_mutex_lock(&mutexTablaSegmento);
	enteroParaComparaciones=numeroSegmentoBuscado;if(list_is_empty(tablaSegmentos)){pthread_mutex_unlock(&mutexTablaSegmento);
	pthread_mutex_unlock(&mutexComparador);respuestaAux.exito=-1;return respuestaAux;}
	someSegment=list_find(tablaSegmentos,esElSegmento);
	pthread_mutex_unlock(&mutexTablaSegmento);
	pthread_mutex_unlock(&mutexComparador);
	if (someSegment==NULL) {respuestaAux.exito=-1;}
	else {if(entra(1048576*(someSegment->numSegmento),someSegment->tamanio,direccion,tamanio)){
		respuestaAux.exito=1;
		respuestaAux.direccion=someSegment->direccion_tablaPaginas;

	return respuestaAux;};
	respuestaAux.exito=0;
	}
return respuestaAux;
}

int entra(uint32_t direccionBaseSegmento,uint32_t tamanioSegmento,uint32_t punteroAescribr,uint32_t cantBytes){
	return(!((direccionBaseSegmento+tamanioSegmento)<(punteroAescribr+cantBytes)));}

void llevarAMemoria(uint32_t PID, uint32_t segmento, uint32_t pagina){
							FILE* archivo;char bytes[256];char* nombreArchivo;
	nombreArchivo=string_new();
	string_append_with_format(&nombreArchivo,"archivos/%u_%u_%u.txt",PID,segmento,pagina);
	archivo=fopen(nombreArchivo,"r+");
	if(archivo!=NULL){int i;
		for(i=0;(i!=256);i++)
		bytes[i]=fgetc(archivo);
		fclose(archivo);
		remove(nombreArchivo);};
	free(nombreArchivo);
	pthread_mutex_lock(&mutexColaMarcos);
	if(!hayMarcoDisponible())
		hacerSwap();//falta hacer marco disp y swap
	ingresar(PID,segmento,pagina,bytes);//falta hacer ingresar
	pthread_mutex_unlock(&mutexColaMarcos);}

int hayMarcoDisponible(){int i;
				pthread_mutex_lock(&mutexColaMarcosLibres);
				i=!list_is_empty(colaMarcosLibres->elements);
				pthread_mutex_unlock(&mutexColaMarcosLibres);
				return i;}

void hacerSwap(){
			if(algoritmo=='L') LRU();else CLOCK();}

void LRU()	{elem_tipoColaAuxiliar *nodo;FILE* archivoSwap; char* nombreArchivo;elem_colaMarcosLibres* auxiliar;char txtAux[257];
respuesta_t respuesta_entra;char* dirTablaPaginas;respuesta_t respuesta_tablaSegmentos;uint32_t direccion;
	pthread_rwlock_wrlock(&semaforoCola_memoria);
	nodo=queue_pop(direccionColaMarcos);
	pthread_rwlock_unlock(&semaforoCola_memoria);
	nombreArchivo=string_new();
	string_append_with_format(&nombreArchivo,"archivos/%u_%u_%u.txt",nodo->PID,nodo->segmento,nodo->pagina);
	archivoSwap=fopen(nombreArchivo,"w+");pthread_mutex_lock(&mutexColaMarcosLibres);
	modificar(txtAux,nodo->direccionMarco,0,256);
	txtAux[256]='\0';
	txt_write_in_file(archivoSwap,txtAux);
	auxiliar=malloc(8);
	auxiliar->direccionMarco=nodo->direccionMarco;
	auxiliar->numMarco=nodo->numMarco;
	queue_push(colaMarcosLibres,auxiliar);pthread_mutex_unlock(&mutexColaMarcosLibres);
	fclose(archivoSwap);direccion=(nodo->segmento)*1048576;
	respuesta_tablaSegmentos=obtenerTablaSegmentos(nodo->PID);
	respuesta_entra=entraYDirTablaPags(respuesta_tablaSegmentos.direccion,direccion,0);
	dirTablaPaginas=respuesta_entra.direccion;
	*(dirTablaPaginas+(nodo->pagina))='S';
}




void CLOCK(){elem_tipoColaAuxiliar *nodo;FILE* archivoSwap; char* nombreArchivo;elem_colaMarcosLibres* auxiliar;char txtAux[257];
respuesta_t respuesta_entra;char* dirTablaPaginas;respuesta_t respuesta_tablaSegmentos;uint32_t direccion;
	pthread_rwlock_wrlock(&semaforoCola_memoria);
	nombreArchivo=string_new();
	nodo=queue_pop(direccionColaMarcos);
	while(nodo->referencia){ // fijarse el nobre de referencia
		nodo->referencia=0;
		queue_push(direccionColaMarcos,nodo);
		nodo=queue_pop(direccionColaMarcos);}

	pthread_rwlock_unlock(&semaforoCola_memoria);
	string_append_with_format(&nombreArchivo,"archivos/%u_%u_%u.txt",nodo->PID,nodo->segmento,nodo->pagina);
	archivoSwap=fopen(nombreArchivo,"w+");pthread_mutex_lock(&mutexColaMarcosLibres);direccion=(nodo->segmento)*1048576;
	auxiliar=malloc(8);
	auxiliar->direccionMarco=nodo->direccionMarco;
	auxiliar->numMarco=nodo->numMarco;
	queue_push(colaMarcosLibres,auxiliar);
	modificar(txtAux,nodo->direccionMarco,0,256);
	txtAux[256]='\0';
	txt_write_in_file(archivoSwap,txtAux);
	pthread_mutex_unlock(&mutexColaMarcosLibres);
	fclose(archivoSwap);
	respuesta_tablaSegmentos=obtenerTablaSegmentos(nodo->PID);
	respuesta_entra=entraYDirTablaPags(respuesta_tablaSegmentos.direccion,direccion,0);
	dirTablaPaginas=respuesta_entra.direccion;
	*(dirTablaPaginas+(nodo->pagina))='S';

}



void ingresar(uint32_t PID,uint32_t segmento, uint32_t pagina, char* bytes){
	char* marco;elem_tipoColaAuxiliar *elementoDeCola;elem_colaMarcosLibres* auxiliar;

	pthread_rwlock_wrlock(&semaforoCola_memoria);
	pthread_mutex_lock(&mutexColaMarcosLibres);
	auxiliar=queue_pop(colaMarcosLibres);
	pthread_mutex_unlock(&mutexColaMarcosLibres);
	elementoDeCola=malloc(24);
	modificar(auxiliar->direccionMarco,bytes,0,256);
	elementoDeCola->PID=PID;
	elementoDeCola->segmento=segmento;
	elementoDeCola->pagina=pagina;
	elementoDeCola->numMarco=auxiliar->numMarco;//no se como se llaman estos campos
	elementoDeCola->direccionMarco=auxiliar->direccionMarco; //esta bien esto? lo cambie porque antes decia elemdeCola->dir=elemdeCola->dir.
	elementoDeCola->referencia=0;//IDEM
	queue_push(direccionColaMarcos,elementoDeCola);
	pthread_rwlock_unlock(&semaforoCola_memoria);}






void modificar(char* direccionMarco,char* bytesAEscribir,int offset,int tamanio){
																							uint32_t i;
	direccionMarco=direccionMarco+offset;
	for(i=0;i<tamanio;i++){
		*direccionMarco=*bytesAEscribir;
		direccionMarco++;
		bytesAEscribir++;};}
void modificarAlt(char* direccionMarco,char* bytesAEscribir,int offset,int tamanio){
																							uint32_t i;
	bytesAEscribir=bytesAEscribir+offset;
	for(i=0;i<tamanio;i++){
		*direccionMarco=*bytesAEscribir;
		direccionMarco++;
		bytesAEscribir++;};}




bool esLaPagina(elem_tipoColaAuxiliar *nodo){uint32_t PID;uint32_t segmento;uint32_t pagina;
	PID=extraerOffset(enteroParaComparaciones);
	pagina=extraerPagina(enteroParaComparaciones);
	segmento=extraerSegmento(enteroParaComparaciones);
	return ((nodo->PID==PID)&&(nodo->segmento==segmento)&&(nodo->pagina==pagina));}
uint32_t esElSegmento(elem_tipoSegmento * segmento){
	if (segmento->numSegmento==enteroParaComparaciones) {return 1;}
	else {return 0;}
}

int extraerSegmento(int entero){return (entero/1048576);}
int extraerOffset(int entero){return(entero%256);}
int extraerPagina(int entero){return((entero%1048576)/256);}


uint32_t imprimirTablaSegmentos(t_list * listaProcesos){ //ver semaforos para acceder a las listas a leer
	list_iterate(listaProcesos,imprimirInformacionProceso);
	printf("--- Fin de tabla de segmentos --- \n\n");
	return 1;
}


void imprimirInformacionProceso(elem_tipoProceso *proceso){
	printf("PID %u \n",proceso->PID);
	list_iterate(proceso->direccion_tablaSegmentos,imprimirInformacionSegmento);
	printf("---FIN TABLA DEL PID %u--- \n \n",proceso->PID);
}


void imprimirInformacionSegmento(elem_tipoSegmento *segmento){
	printf("Numero de Segmento %u Tamaño %u Direccion virtual base %u \n",segmento->numSegmento,segmento->tamanio,segmento->numSegmento*1048576); //hay que reemplazar el 123 por el PID, para eso
	//ghacerlo global proceso.PID
}




uint32_t imprimirTablaPaginas(uint32_t PID){ //ver semaforos para acceder a las listas a leer
	elem_tipoProceso * proceso; // no recibe puntero?
	pthread_mutex_lock(&mutexComparador);
	enteroParaComparaciones=PID; //mantengo el mutex mientras lo uso?
	proceso=list_find(direccionListaProcesos,esElPID);
	pthread_mutex_unlock(&mutexComparador);
	if(proceso==NULL) {puts("no existe proceso\n");return 0;}
	list_iterate(proceso->direccion_tablaSegmentos,imprimirInformacionPaginas);
	printf(" --- Fin de la tabla de paginas --- \n");
	return 1;
}



void imprimirInformacionPaginas(elem_tipoSegmento *segmento){
	char * direccionTablaPaginas; //la vamos a hacer de ints??
	uint32_t posicion;int cantPags;
	direccionTablaPaginas=segmento->direccion_tablaPaginas;
	cantPags=segmento->tamanio/256;
	if(segmento->tamanio%256)cantPags++;printf("segmento:%u \n",segmento->numSegmento);
	for(posicion=0;posicion<cantPags;posicion++){
		if (*(direccionTablaPaginas+posicion)=='S') {
			printf("Numero de Pagina: %u En DISCO \n",posicion);}
		if (*(direccionTablaPaginas+posicion)=='N')printf("Numero de Pagina: %u Sin cargar \n",posicion);
		else	{printf("Numero de Pagina: %u En MEMORIA \n",posicion);}
	}
}


uint32_t esElPID(elem_tipoProceso * unProceso){
		return (unProceso->PID==enteroParaComparaciones);
}
void ingresar_en_queue_ord(t_queue*,t_queue*);
void listarMarcos(){int contador;int cantidadMarcos;int i;
	t_queue* ColaMarcos;
	int flag=0;i=0;
	pthread_rwlock_rdlock(&semaforoCola_memoria);pthread_mutex_lock(&mutexColaMarcos);pthread_mutex_lock(&mutexColaMarcosLibres);
	ColaMarcos=queue_create();
	//list_add_all(ColaLibres->elements,colaMarcosLibres->elements);
	//list_add_all(ColaMarcos->elements,direccionColaMarcos->elements);
	ingresar_en_queue_ord(ColaMarcos,direccionColaMarcos);
	//int cont2=list_size(ColaLibres->elements);

	list_sort(ColaMarcos->elements,marcoMenor);
	cantidadMarcos=cantidadMemoria*4;
	//list_sort(ColaLibres->elements,libreMenor);
	t_list_ord* elementoDeCola;
	//elem_colaMarcosLibres* elementoMarcoLibre;
	if (!queue_is_empty(ColaMarcos)) elementoDeCola=queue_pop(ColaMarcos);
	else {elementoDeCola=malloc(sizeof(t_list_ord));
			elementoDeCola->numMarco=cantidadMarcos+1;
			flag=1;
	}
	for(contador=0;contador<cantidadMarcos;contador++){
		if (contador<elementoDeCola->numMarco) printf("Num marco %u LIBRE \n",contador);
		else {
			printf("PID: %u Num Marco: %u ",elementoDeCola->PID,elementoDeCola->numMarco);
			if (algoritmo=='L') {printf("Posicion %u \n", i); i++;}
			else printf("bit referencia %u \n",elementoDeCola->referencia);
			if (!queue_is_empty(ColaMarcos)) elementoDeCola=queue_pop(ColaMarcos);
			else {elementoDeCola=malloc(sizeof(elem_tipoColaAuxiliar));
								elementoDeCola->numMarco=cantidadMarcos+1;
				}
			}
	}
	pthread_rwlock_unlock(&semaforoCola_memoria);pthread_mutex_unlock(&mutexColaMarcos);pthread_mutex_unlock(&mutexColaMarcosLibres);
	if (flag==1) free(elementoDeCola);
	queue_destroy(ColaMarcos);
}
bool marcoMenor(t_list_ord* marco,t_list_ord* marco2){
	return(marco->numMarco<marco2->numMarco);}


void ingresar_en_queue_ord(t_queue* queueSalida,t_queue* queueEntrada){


	void agregar_en_queueSalida(elem_tipoColaAuxiliar* nodo){t_list_ord* nuevoNodo;
	nuevoNodo=malloc(sizeof(t_list_ord));
	nuevoNodo->PID=nodo->PID;
	nuevoNodo->numMarco=nodo->numMarco;
	nuevoNodo->referencia=nodo->referencia;
	nuevoNodo->orden=enteroParaComparaciones;
	enteroParaComparaciones++;
	queue_push(queueSalida,nuevoNodo);
	}
	pthread_mutex_lock(&mutexComparador);
	enteroParaComparaciones=0;
	list_iterate(queueEntrada->elements,agregar_en_queueSalida);
	pthread_mutex_unlock(&mutexComparador);}

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
uint32_t recibirInt32(int socket) {
	uint32_t int32;
	int recibido;
	if ((recibido = recv(socket, &int32, sizeof(uint32_t), 0)) == -1) {	//DETALLE, el size del recv es de int y no de u_32int, anda en la vm de la catedra,pero en una implementacion con otro size de int puede generar problemas al enviar u_32int
		printf("Fallo el recv de recibirInt32\n");
		exit(1);
	}
	return int32;

}
int recibirInt(int socket) {
	int unInt;
	int recibido;
	if ((recibido = recv(socket, &unInt, sizeof(int), 0)) == -1) {//DETALLE, el size del recv es de int y no de u_32int, anda en la vm de la catedra,pero en una implementacion con otro size de int puede generar problemas al enviar u_32int
		printf("Fallo el recv de recibirInt\n");
		exit(1);
	}	return unInt;
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


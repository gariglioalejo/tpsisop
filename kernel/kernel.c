/*
 * kernel.c
 *
 *  Created on: Sep 1, 2014
 *      Author: utnso
 */

#include "../consola/funciones.h"

int socketMsp;
t_config * kernel_config;
char * ip_msp;
char * puerto_msp;
char * puerto_kernel;
int quantum;
char * syscalls;
int size_stack;
int listenningSocket;
t_tcb * tcbKM;
t_queue * colaKM;
t_queue * colaReady;
int i;
int size_syscalls;
int handshake;
int pid = 1, tid = 1;
int iret1;
pthread_t cpuLibres;
fd_set readfds;
fd_set master;
int setmax;
int rv;
int x = 1;
t_reservarSegmentos tcb_resultado;
pthread_mutex_t mutexReady = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexColaKM = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaCpu = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaExec = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexListaBloq = PTHREAD_MUTEX_INITIALIZER;
sem_t hayEnReady;
sem_t hayCpu;
t_list * listaCpuLibres;
t_list * listaSocketsCpu;
t_list * listaExec;
t_list * listaExit;
t_list * listaBloq;
t_list * listaSocketsConsola;
int unaOperacion;
int recibido;
int tidTcb;
t_tcb * tcbDeCpu;
uint32_t direccionSyscall;
t_tcb * tcbBloq;
int pidEE;
int tipoEE;
int socketEE;

enum {
	TERMINO_QUANTUM,
	CONCLUYO_EJECUCION,
	EJECUCION_ERRONEA,
	SYSCALL,
	ENTRADA_ESTANDAR,
	SALIDA_ESTANDAR,
	CREAR_HILO,
	JOIN,
	BLOQUEAR,
	DESPERTAR
};

bool comparar(void * socketConCambios) {
	return *(int *) socketConCambios == i;
}
bool compararTcbSocket(void * tcb) {
	t_tcb * tcbaux = tcb;
	return tcbaux->socketCpu == i;
}

bool compararTcb(void * tcb) {
	t_tcb * tcbaux = tcb;
	return tcbaux->tid == tidTcb;
}

void * manejoCpuLibres(void * arg) {
	int socketCpu;

	int posicion = 0;

	while (1) {

		sem_wait(&hayCpu);
		pthread_mutex_lock(&mutexListaExec);
		pthread_mutex_lock(&mutexListaBloq);
		pthread_mutex_lock(&mutexColaKM);
		pthread_mutex_lock(&mutexListaCpu);

		if (list_size(listaBloq) && queue_size(colaKM)) {
			t_tcb * tcb = malloc(sizeof(t_tcb));
			t_tcb *tcbKernelMode = malloc(sizeof(t_tcb));
			tcbKernelMode = queue_pop(colaKM);
			tcb = list_get(listaBloq, 0);
			tcbKernelMode->pid = tcb->pid;
			tcbKernelMode->tid = tcb->tid;			//es necesario esto del tid?
			tcbKernelMode->A = tcb->A;
			tcbKernelMode->B = tcb->B;
			tcbKernelMode->C = tcb->C;
			tcbKernelMode->D = tcb->D;
			tcbKernelMode->E = tcb->E;
			tcbKernelMode->P = tcb->direccionSyscallPendiente;
			socketCpu = *(int *) (list_remove(listaCpuLibres, posicion));
			enviarInt(-1, socketCpu);
			enviarTcb(tcbKernelMode, socketCpu);
			tcbKernelMode->socketCpu = socketCpu;
			list_add(listaExec, tcbKernelMode);
			pthread_mutex_unlock(&mutexListaCpu);
			pthread_mutex_unlock(&mutexListaBloq); //feo
			pthread_mutex_unlock(&mutexColaKM);
			pthread_mutex_unlock(&mutexListaExec);
		} else {

			pthread_mutex_unlock(&mutexListaCpu);
			pthread_mutex_unlock(&mutexListaBloq); //feo
			pthread_mutex_unlock(&mutexColaKM);
			pthread_mutex_unlock(&mutexListaExec);
			sem_wait(&hayEnReady);
			pthread_mutex_lock(&mutexListaExec);
			pthread_mutex_lock(&mutexListaCpu);
			pthread_mutex_lock(&mutexReady);
			t_tcb * tcb = malloc(sizeof(t_tcb));
			tcb = queue_pop(colaReady);
			socketCpu = *(int *) (list_remove(listaCpuLibres, posicion));
			enviarInt(quantum, socketCpu);
			enviarTcb(tcb, socketCpu);
			tcb->socketCpu = socketCpu;
			list_add(listaExec, tcb);
			pthread_mutex_unlock(&mutexListaCpu);
			pthread_mutex_unlock(&mutexReady);
			pthread_mutex_unlock(&mutexListaExec);
		}
	}
}

void checkArgument( argc) {
	if (argc != 3) {
		printf("No hay suficients argumentos\n");
		exit(1);
	}
}

int main(int argc, char ** argv) {

	checkArgument(argc);

	kernel_config = config_create(argv[1]);

	puerto_kernel = config_get_string_value(kernel_config, "PUERTO");
	ip_msp = config_get_string_value(kernel_config, "IP_MSP");
	puerto_msp = config_get_string_value(kernel_config, "PUERTO_MSP");
	syscalls = config_get_string_value(kernel_config, "SYSCALLS");
	quantum = config_get_int_value(kernel_config, "QUANTUM");
	size_stack = config_get_int_value(kernel_config, "STACK");

	colaKM = queue_create();
	colaReady = queue_create();
	listaCpuLibres = list_create();
	listaSocketsCpu = list_create();
	listaExec = list_create();
	listaExit = list_create();
	listaBloq = list_create();
	listaSocketsConsola = list_create();

	sem_init(&hayEnReady, 0, 0);
	sem_init(&hayCpu, 0, 0);

	syscalls = copiarArchivo(argv[2]);
	size_syscalls = hacerStat(argv[2]).st_size;
	socketMsp = conectarse(ip_msp, puerto_msp);
	handshake = 1;
	enviarInt(handshake, socketMsp);

	tcb_resultado = reservarSegmentos(pid, size_syscalls, syscalls, size_stack,
			socketMsp, -1);	//No implemente la posibilidad de que falle al copiar el de syscalls, es algo a considerar? Dsps lo hago :P
//printf("%d\n%d\n%d\n", tcbKM->X, tcbKM->P, tcbKM->tam_seg_cod);
	tcbKM = tcb_resultado.tcb;
	tcbKM->km = 1;
	tcbKM->pid = pid;
	tcbKM->tid = tid;
	queue_push(colaKM, tcbKM);
//printf("%d\n",queue_size(colaKM));

	listenningSocket = crearServer(puerto_kernel);

	iret1 = pthread_create(&cpuLibres, NULL, manejoCpuLibres, NULL );
	if (iret1) {
		fprintf(stderr, "Fallo  creacion hilo manejoCpuLibres retorno: %d\n",
				iret1);
		exit(1);
	}

	FD_ZERO(&readfds);
	FD_ZERO(&master);
	FD_SET(listenningSocket, &master);
	setmax = listenningSocket;

	while (x) {
		readfds = master;
		rv = select(setmax + 1, &readfds, NULL, NULL, NULL );
		if (rv == -1) {
			perror("select");
			printf("Error en el select\n");
		} else {
			for (i = 0; i <= setmax; i++) {
				if (FD_ISSET(i, &readfds)) {
					if (i == listenningSocket) {
						int socketCliente = aceptarConexion(listenningSocket);
						int codigo = recibirInt(socketCliente);
						if (codigo == 1) {
							char * sizeConBeso = recibir_serializado_beso(
									socketCliente);
							t_reservarSegmentos tcb_resultado =
									reservarSegmentos(pid, *(int *) sizeConBeso,
											sizeConBeso + sizeof(int),
											size_stack, socketMsp,
											socketCliente);
							if (tcb_resultado.exito) {
								t_tcb * tcb = tcb_resultado.tcb;
								pid++;
								tid++;
								tcb->km = 0;
								tcb->pid = pid;
								tcb->tid;
								printf("valor socketCliente:%d\n",
										socketCliente);
								tcb->socketConsola = socketCliente;
								printf("valor tcb->socketConsola:%d\n",
										tcb->socketConsola);
								FD_SET(socketCliente, &master);
								if (socketCliente > setmax) {
									setmax = socketCliente;
								}

								pthread_mutex_lock(&mutexReady);
								queue_push(colaReady, tcb);
								sem_post(&hayEnReady);
								pthread_mutex_unlock(&mutexReady);

								/*printf("%d\n%d\n%d\n", tcb->X, tcb->P,
								 tcb->tam_seg_cod);
								 */
								//x=0;
							}

						}
						if (codigo == 2) {	//CPU
							FD_SET(socketCliente, &master);
							if (socketCliente > setmax) {
								setmax = socketCliente;
							}
							printf("socatoa\n");
							int * socketClienteAux = malloc(sizeof(int));
							*socketClienteAux = socketCliente;
							list_add(listaSocketsCpu, socketClienteAux);
							pthread_mutex_lock(&mutexListaCpu);
							list_add(listaCpuLibres, socketClienteAux);
							sem_post(&hayCpu);
							pthread_mutex_unlock(&mutexListaCpu);

							//x = 0;

						}

						//x=0;
					}

					if (list_any_satisfy(listaSocketsCpu, comparar)) {

						//printf("hello\n");
						if ((recibido = recv(i, &unaOperacion, sizeof(int), 0))
								== -1) {
							printf("Fallo el recv de una Cpu\n");
							exit(1);
						} else if (recibido == 0) {
							x = 0;
							printf("Nos fuimos\n");
						} else {

							switch (unaOperacion) {
							case TERMINO_QUANTUM: {
								t_tcb * tcbCpu = malloc(sizeof(t_tcb));
								tcbCpu = recibirTcb(i);
								tidTcb = tcbCpu->tid;
								pthread_mutex_lock(&mutexListaExec);
								pthread_mutex_lock(&mutexReady);
								pthread_mutex_lock(&mutexListaCpu);
								pthread_mutex_lock(&mutexListaBloq);
								list_remove_by_condition(listaExec,
										compararTcb);
								printf("Tamanio Lista Exec: %d\n",
										list_size(listaExec));
								if (tcbCpu->km == 1) {
									queue_push(colaKM, tcbCpu);
									printf("Volvio el KM: %d\n", tcbCpu->km);
									t_tcb * tcbBloqueado = malloc(
											sizeof(t_tcb));
									tcbBloqueado = list_remove(listaBloq, 0);
									tcbBloqueado->A = tcbCpu->A;
									tcbBloqueado->B = tcbCpu->B;
									tcbBloqueado->C = tcbCpu->C;
									tcbBloqueado->D = tcbCpu->D;
									tcbBloqueado->E = tcbCpu->E;
									queue_push(colaReady, tcbBloqueado);
								} else {
									queue_push(colaReady, tcbCpu);
								}
								int * iAux = malloc(sizeof(int));
								*iAux = i;
								list_add(listaCpuLibres, iAux);
								printf("Tamanio Lista CpuLibres: %d\n",
										list_size(listaCpuLibres));
								pthread_mutex_unlock(&mutexListaBloq);
								pthread_mutex_unlock(&mutexReady);
								pthread_mutex_unlock(&mutexListaCpu);
								pthread_mutex_unlock(&mutexListaExec);
								sem_post(&hayEnReady);
								sem_post(&hayCpu);
								/*
								 int valorHayEnReady,valorHayCpu;
								 sem_getvalue(&hayEnReady,&valorHayEnReady);
								 sem_getvalue(&hayCpu,&valorHayCpu);
								 printf("ready:%d\n",valorHayEnReady);
								 printf("cpu:%d\n",valorHayCpu);
								 //printf("final\n");
								 */

								break;
							}
							case CONCLUYO_EJECUCION: {
								t_tcb * tcbCpu = malloc(sizeof(t_tcb));
								tcbCpu = recibirTcb(i);
								tidTcb = tcbCpu->tid;
								pthread_mutex_lock(&mutexListaExec);
								pthread_mutex_lock(&mutexListaCpu);
								list_remove_by_condition(listaExec,
										compararTcb);
								printf("Tamanio Lista Exec: %d\n",
										list_size(listaExec));
								int * iAux = malloc(sizeof(int));
								*iAux = i;
								list_add(listaCpuLibres, iAux);
								sem_post(&hayCpu);
								pthread_mutex_unlock(&mutexListaCpu);
								pthread_mutex_unlock(&mutexListaExec);
								list_add(listaExit, tcbCpu);
								printf("%d\n", tcbCpu->socketConsola);
								enviar_serializado(1,
										"Concluyo ejecucion normalmente",
										tcbCpu->socketConsola);
								enviarInt(0, tcbCpu->socketConsola);
								break;
							}
							case EJECUCION_ERRONEA: {
								t_tcb * tcbCpu = malloc(sizeof(t_tcb));
								tcbCpu = recibirTcb(i);
								tidTcb = tcbCpu->tid;
								pthread_mutex_lock(&mutexListaExec);
								pthread_mutex_lock(&mutexListaCpu);
								list_remove_by_condition(listaExec,
										compararTcb);
								printf("Tamanio Lista Exec: %d\n",
										list_size(listaExec));
								int * iAux = malloc(sizeof(int));
								*iAux = i;
								list_add(listaCpuLibres, iAux);
								sem_post(&hayCpu);
								pthread_mutex_unlock(&mutexListaCpu);
								pthread_mutex_unlock(&mutexListaExec);
								list_add(listaExit, tcbCpu);
								printf("%d\n", tcbCpu->socketConsola);
								enviar_serializado(1,
										"Concluyo por ejecucion erronea",
										tcbCpu->socketConsola);
								enviarInt(0, tcbCpu->socketConsola);
								break;
							}
							case SYSCALL: {
								t_tcb * tcbCpu = malloc(sizeof(t_tcb));
								tcbCpu = recibirTcb(i);
								tidTcb = tcbCpu->tid;
								direccionSyscall = recibirInt32(i);
								tcbCpu->direccionSyscallPendiente =
										direccionSyscall;
								pthread_mutex_lock(&mutexListaExec);
								pthread_mutex_lock(&mutexListaCpu);
								pthread_mutex_lock(&mutexListaBloq);
								list_remove_by_condition(listaExec,
										compararTcb);
								printf("Tamanio Lista Exec: %d\n",
										list_size(listaExec));
								int * iAux = malloc(sizeof(int));
								*iAux = i;
								list_add(listaCpuLibres, iAux);
								list_add(listaBloq, tcbCpu);
								sem_post(&hayCpu);
								pthread_mutex_unlock(&mutexListaCpu);
								pthread_mutex_unlock(&mutexListaBloq);
								pthread_mutex_unlock(&mutexListaExec);
								break;
							}
							case ENTRADA_ESTANDAR: {
								pidEE = recibirInt(i);
								tipoEE = recibirInt(i);
								if (tipoEE == 0) {
									pthread_mutex_lock(&mutexListaExec);
									t_tcb * tcbCpu = malloc(sizeof(t_tcb));
									tcbCpu = list_find(listaExec,
											compararTcbSocket);
									pthread_mutex_unlock(&mutexListaExec);
									enviarInt(3, tcbCpu->socketConsola);
									int intIngresado;
									intIngresado = recibirInt(
											tcbCpu->socketConsola);
									printf("intIngresado:%d\n", intIngresado);
									enviarInt(intIngresado, i);
									break;

								}
								if (tipoEE == 1) {
									int inputMax;
									inputMax = recibirInt(i);
									pthread_mutex_lock(&mutexListaExec);
									t_tcb * tcbCpu = malloc(sizeof(t_tcb));
									tcbCpu = list_find(listaExec,
											compararTcbSocket);
									pthread_mutex_unlock(&mutexListaExec);
									enviarInt(2, tcbCpu->socketConsola);
									enviarInt(inputMax, tcbCpu->socketConsola);
									char * string;
									string = recibir_serializado(
											tcbCpu->socketConsola);
									printf("%s\n", string);
									enviar_serializado(-1, string,
											tcbCpu->socketCpu);
								}
								break;
							}
							case SALIDA_ESTANDAR: {
								int pidSE;
								char * string;
								pidSE = recibirInt(i);
								string = recibir_serializado(i);
								pthread_mutex_lock(&mutexListaExec);
								t_tcb * tcbCpu = malloc(sizeof(t_tcb));
								tcbCpu = list_find(listaExec,
										compararTcbSocket);
								pthread_mutex_unlock(&mutexListaExec);
								enviarInt(1, tcbCpu->socketConsola);
								enviar_serializado(-1, string,
										tcbCpu->socketConsola);
								break;
							}

							}

						}

					}
				}
			}

		}

	}


pthread_join(cpuLibres, NULL );
return 0;
}

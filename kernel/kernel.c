/*
 * kernel.c
 *
 *  Created on: Sep 1, 2014
 *      Author: utnso
 */

#include "../ensalada de funciones/funciones.h"

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
int unaOperacion;
int recibido;
int tidTcb;
t_tcb * tcbDeCpu;
uint32_t direccionSyscall;
t_tcb * tcbBloq;

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
bool compararTcb(void * tcb) {
	t_tcb * tcbaux = tcb;
	return tcbaux->tid == tidTcb;
}

void * manejoCpuLibres(void * arg) {
	int socketCpu;
	t_tcb * tcb;
	int posicion = 0;
	t_tcb *tcbKernelMode;
	while (1) {

		sem_wait(&hayCpu);
		pthread_mutex_lock(&mutexListaExec);
		pthread_mutex_lock(&mutexListaBloq);
		pthread_mutex_lock(&mutexColaKM);
		pthread_mutex_lock(&mutexListaCpu);

		if (list_size(listaBloq) && queue_size(colaKM)) {
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
			enviarInt(-1, socketCpu);		//comentar esta convencion con cpu
			enviarTcb(tcbKernelMode, socketCpu);
			tcbKernelMode->socketCpu = socketCpu;
			list_add(listaExec, tcbKernelMode);
			pthread_mutex_unlock(&mutexListaCpu);
			pthread_mutex_unlock(&mutexListaBloq);//se ve poco eficiente poner estos dos unlock aca...
			pthread_mutex_unlock(&mutexColaKM);
			pthread_mutex_unlock(&mutexListaExec);
		} else {

			pthread_mutex_unlock(&mutexListaCpu);
			pthread_mutex_unlock(&mutexListaBloq);//se ve poco eficiente poner estos dos unlock aca...
			pthread_mutex_unlock(&mutexColaKM);
			pthread_mutex_unlock(&mutexListaExec);
			sem_wait(&hayEnReady);
			pthread_mutex_lock(&mutexListaExec);
			pthread_mutex_lock(&mutexListaCpu);
			pthread_mutex_lock(&mutexReady);
			tcb = queue_pop(colaReady);
			socketCpu = *(int *) (list_remove(listaCpuLibres, posicion));
			//printf("Tamanio Lista CpuLibres: %d\n", list_size(listaCpuLibres));
			enviarInt(quantum, socketCpu);
			enviarTcb(tcb, socketCpu);
			tcb->socketCpu = socketCpu;
			list_add(listaExec, tcb);
			//printf("Tamanio Lista Exec: %d\n", list_size(listaExec));
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

	iret1 = pthread_create(&cpuLibres, NULL, manejoCpuLibres, NULL);
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
		rv = select(setmax + 1, &readfds, NULL, NULL, NULL);
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
							list_add(listaSocketsCpu, &socketCliente);
							pthread_mutex_lock(&mutexListaCpu);
							list_add(listaCpuLibres, &socketCliente);
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
							case TERMINO_QUANTUM:
								//printf("Aloha\n");
								tcbDeCpu = recibirTcb(i);
								tidTcb = tcbDeCpu->tid;
								pthread_mutex_lock(&mutexListaExec);
								pthread_mutex_lock(&mutexReady);
								pthread_mutex_lock(&mutexListaCpu);
								pthread_mutex_lock(&mutexListaBloq);
								list_remove_by_condition(listaExec,
										compararTcb);
								printf("Tamanio Lista Exec: %d\n",
										list_size(listaExec));
								if (tcbDeCpu->km == 1) {
									queue_push(colaKM, tcbDeCpu);
									printf("Volvio el KM: %d\n", tcbDeCpu->km);
									tcbBloq=list_remove(listaBloq, 0);
									tcbBloq->A = tcbDeCpu->A;
									tcbBloq->B = tcbDeCpu->B;
									tcbBloq->C = tcbDeCpu->C;
									tcbBloq->D = tcbDeCpu->D;
									tcbBloq->E = tcbDeCpu->E;
									queue_push(colaReady, tcbBloq);
								} else {
									queue_push(colaReady, tcbDeCpu);
								}
								list_add(listaCpuLibres, &i);
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
							case CONCLUYO_EJECUCION:
								tcbDeCpu = recibirTcb(i);
								tidTcb = tcbDeCpu->tid;
								pthread_mutex_lock(&mutexListaExec);
								pthread_mutex_lock(&mutexListaCpu);
								list_remove_by_condition(listaExec,
										compararTcb);
								printf("Tamanio Lista Exec: %d\n",
										list_size(listaExec));
								list_add(listaCpuLibres, &i);
								sem_post(&hayCpu);
								pthread_mutex_unlock(&mutexListaCpu);
								pthread_mutex_unlock(&mutexListaExec);
								list_add(listaExit, tcbDeCpu);
								printf("%d\n", tcbDeCpu->socketConsola);
								enviar_serializado(1,
										"Concluyo ejecucion normalmente",
										tcbDeCpu->socketConsola);
								enviarInt(0, tcbDeCpu->socketConsola);
								break;
							case EJECUCION_ERRONEA:
								tcbDeCpu = recibirTcb(i);
								tidTcb = tcbDeCpu->tid;
								pthread_mutex_lock(&mutexListaExec);
								pthread_mutex_lock(&mutexListaCpu);
								list_remove_by_condition(listaExec,
										compararTcb);
								printf("Tamanio Lista Exec: %d\n",
										list_size(listaExec));
								list_add(listaCpuLibres, &i);
								sem_post(&hayCpu);
								pthread_mutex_unlock(&mutexListaCpu);
								pthread_mutex_unlock(&mutexListaExec);
								list_add(listaExit, tcbDeCpu);
								printf("%d\n", tcbDeCpu->socketConsola);
								enviar_serializado(1,
										"Concluyo por ejecucion erronea",
										tcbDeCpu->socketConsola);
								enviarInt(0, tcbDeCpu->socketConsola);
								break;
							case SYSCALL:
								tcbDeCpu = recibirTcb(i);
								tidTcb = tcbDeCpu->tid;
								direccionSyscall = recibirInt32(i);
								tcbDeCpu->direccionSyscallPendiente =
										direccionSyscall;
								pthread_mutex_lock(&mutexListaExec);
								pthread_mutex_lock(&mutexListaCpu);
								pthread_mutex_lock(&mutexListaBloq);
								list_remove_by_condition(listaExec,
										compararTcb);
								printf("Tamanio Lista Exec: %d\n",
										list_size(listaExec));
								list_add(listaCpuLibres, &i);
								list_add(listaBloq, tcbDeCpu);
								sem_post(&hayCpu);
								pthread_mutex_unlock(&mutexListaCpu);
								pthread_mutex_unlock(&mutexListaBloq);
								pthread_mutex_unlock(&mutexListaExec);

							}

						}
					}
				}

			}

		}

	}
	pthread_join(cpuLibres, NULL);
	return 0;
}

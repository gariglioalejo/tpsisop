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
pthread_mutex_t mutexListaCpu = PTHREAD_MUTEX_INITIALIZER;
sem_t hayEnReady;
sem_t hayCpu;
t_list * listaCpuLibres;
/*
 void * funcionLoader(void* arg) {
 int socketCliente = *((int*) arg);
 char * beso = recibir_serializado_beso(socketCliente);
 //printf("%d\n",socketMsp);
 enviar_serializado_beso(1, *(int*) beso, beso + 4, socketMsp);
 //printf("%s\n", mensaje);
 return NULL ;
 }
 */
void * manejoCpuLibres(void * arg) {
	int  socketCpu;
	t_tcb * tcb;
	int posicion=0;

	sem_wait(&hayCpu);
	sem_wait(&hayEnReady);
	pthread_mutex_unlock(&mutexReady);
	pthread_mutex_unlock(&mutexListaCpu);
	tcb = queue_pop(colaReady);
	socketCpu = *(int *)(list_remove(listaCpuLibres, posicion));
	if (tcb->km == 1) {
		enviarInt(-1, socketCpu);
	} else {
		enviarInt(quantum, socketCpu);
	}
	enviarTcb(tcb, socketCpu);
	pthread_mutex_unlock(&mutexListaCpu);
	pthread_mutex_unlock(&mutexReady);

	free(tcb);


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
		fprintf(stderr, "Fallo  creacion hilo manejoCpuLibres retorno: %d\n", iret1);
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
				if (FD_ISSET(i,&readfds)) {
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
								FD_SET(socketCliente, &master);
								if (socketCliente > setmax) {
									setmax = socketCliente;
								}
								pthread_mutex_lock(&mutexReady);
								queue_push(colaReady, tcb);
								sem_post(&hayEnReady);
								pthread_mutex_unlock(&mutexReady);

								printf("%d\n%d\n%d\n", tcb->X, tcb->P,
										tcb->tam_seg_cod);
							}
						}
						if (codigo == 2) {	//CPU
							FD_SET(socketCliente, &master);
							if (socketCliente > setmax) {
								setmax = socketCliente;
							}
							//printf("socatoa\n");
							pthread_mutex_lock(&mutexListaCpu);
							list_add(listaCpuLibres, &socketCliente);
							sem_post(&hayCpu);
							pthread_mutex_unlock(&mutexListaCpu);

							//x = 0;

						}
					}

				}
			}
		}

	}

	pthread_join(cpuLibres,NULL);
	return 0;
}



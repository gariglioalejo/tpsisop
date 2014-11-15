/*
 * kernel.c
 *
 *  Created on: Nov 13, 2014
 *      Author: utnso
 */

#include "funciones.h"

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

sem_t hayEnReady;
sem_t hayCpu;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * manejoCpuLibres(void * arg) {
	return NULL ;
}

void * esperarEntradaEstandar(void * arg) {
	t_paraEntradaEstandar * datosSocket = arg;
	if (datosSocket->tipoEE == 0) {
		int intIngresado;
		intIngresado = recibirInt(datosSocket->socketConsola);
		enviarInt(intIngresado, datosSocket->socketCpu);
	}
	if (datosSocket->tipoEE == 1) {
		char * string;
		string = recibir_serializado(datosSocket->socketConsola);
		printf("%s\n", string);
		enviar_serializado(-1, string, datosSocket->socketCpu);
	}
	return NULL ;
}

int main(int argc, char ** argv) {
	checkArgument(3, argc);

	t_config * kernel_config;
	kernel_config = config_create(argv[1]);

	char * puerto_kernel;
	puerto_kernel = config_get_string_value(kernel_config, "PUERTO");

	char * ip_msp;
	ip_msp = config_get_string_value(kernel_config, "IP_MSP");

	char * puerto_msp;
	puerto_msp = config_get_string_value(kernel_config, "PUERTO_MSP");

	char * syscalls;
	syscalls = config_get_string_value(kernel_config, "SYSCALLS");

	int quantum;
	quantum = config_get_int_value(kernel_config, "QUANTUM");

	int size_stack;
	size_stack = config_get_int_value(kernel_config, "STACK");

	t_queue * colaKM;
	colaKM = queue_create();

	t_list * listaReady;
	listaReady = list_create();

	t_list * listaCpuLibres;
	listaCpuLibres = list_create();

	t_list * listaSocketsCpu;
	listaSocketsCpu = list_create();

	t_list * listaSocketsConsola;
	listaSocketsConsola = list_create();

	t_list * listaExec;
	listaExec = list_create();

	t_list * listaExit;
	listaExit = list_create();

	t_list * listaBloq;
	listaBloq = list_create();

	t_list * listaBloqJoin;
	listaBloqJoin = list_create();

	t_list * listaHilos;	//****Cambiar el nombre a algo como hilos por PID
	listaHilos = list_create();

	t_list * listaBloqRecurso;
	listaBloqRecurso = list_create();

	t_list * listaPidBaneados;
	listaPidBaneados = list_create();

	t_list * listaCpusEjecutandoUnBaneado;	//** es necesaria esta cola?
	listaCpusEjecutandoUnBaneado = list_create();

	char * literal_syscalls;//{ROVISORIAMENTE SE PUEDE CAMBIAR EL PATH DE SYSCALLS EN EL CFG.
	literal_syscalls = copiarArchivo(syscalls);	//COPIA EL ARCHIVO DE SYSCALLS Y DEVUELVE UN PUNTERO.

	int size_syscalls;
	size_syscalls = hacerStat(syscalls).st_size;//OBTIENE EL TAMANIO DE EL ARCHIVO DE SYSCALLS

	int socketMsp;
	socketMsp = conectarse(ip_msp, puerto_msp);	//***No es necesario handshake con la msp,no?

	t_reservarSegmentos tcb_resultado;
	int pid = 1, tid = 1;
	tcb_resultado = reservarSegmentos(pid, size_syscalls, syscalls, size_stack,
			socketMsp, -1);
	t_tcb * tcbKM;
	tcbKM = tcb_resultado.tcb;
	tcbKM->km = 1;
	tcbKM->pid = pid;
	tcbKM->tid = tid;
	queue_push(colaKM, tcbKM);

	int listenningSocket;
	listenningSocket = crearServer(puerto_kernel);
	//
	int iret1;
	pthread_t cpuLibres;
	iret1 = pthread_create(&cpuLibres, NULL, manejoCpuLibres, NULL );
	if (iret1) {
		fprintf(stderr, "Fallo  creacion hilo manejoCpuLibres retorno: %d\n",
				iret1);
		exit(1);
	}

	sem_init(&hayEnReady, 0, 0);
	sem_init(&hayCpu, 0, 0);

	fd_set readfds;
	fd_set master;
	int setmax;
	FD_ZERO(&readfds);
	FD_ZERO(&master);
	FD_SET(listenningSocket, &master);
	setmax = listenningSocket;

	while (1) {								//***Nunca va a salir del ciclo, no?
		readfds = master;
		int rv;
		rv = select(setmax + 1, &readfds, NULL, NULL, NULL );
		if (rv == -1) {
			perror("select");
			printf("Error en el select\n");
		} else {
			int i;
			for (i = 0; i <= setmax; i++) {
				if (FD_ISSET(i, &readfds)) {
					if (i == listenningSocket) {
						int socketCliente;
						socketCliente = aceptarConexion(listenningSocket);
						int codigo;
						codigo = recibirInt(socketCliente);	//PARA SABER SI ES CONSOLA O CPU (1=CONSOLA NUEVA,2=CPU NUEVA)
						if (codigo == 1) {
							char * sizeConBeso;
							sizeConBeso = recibir_serializado_beso(	//DEVUELVE SERIALIZADO EL TAMANIO Y EL SCRIPT MANDADO POR LA CONSOLA
									socketCliente);
							t_reservarSegmentos tcb_resultado;//NO RECUERDO PORQUE HICE QUE recibir_serializado_beso DEVUELVA SERIALIZADO, POR ESO LOS CASTEOS EN LA LLAMADA A ESTA FUNCION.
							tcb_resultado = reservarSegmentos(pid,
									*(int *) sizeConBeso,
									sizeConBeso + sizeof(int), size_stack,
									socketMsp, socketCliente);
							if (tcb_resultado.exito) {
								t_tcb * tcb = tcb_resultado.tcb;
								pid++;
								tid++;
								tcb->km = 0;
								tcb->pid = pid;
								tcb->tid = tid;
								t_listaHilos * nodoListaHilos = malloc(	//CREA UN NUEVO NODO EN LA LISTA HILOS PARA EL PROCESO NUEVO Y LE ASIGNA UN VALOR DE 1 A SU CANTIDAD DE HILOS (VA A IR AUMENTANDO ESTE VALOR A MEDIDA QUE SE USE CREA).
										sizeof(t_listaHilos));
								nodoListaHilos->pid = pid;
								nodoListaHilos->hilos = 1;
								//***faltaria lo de listaSocketsConsola
								list_add(listaHilos, nodoListaHilos);
								if (socketCliente > setmax) {	//PARA EL SELECT
									setmax = socketCliente;
								}
								t_listaSocketsConsola * nodoConsola = malloc(//AGREGA A LA COLA DE SOCKETS CONSOLA UN NODO CON EL PID ASOCIADO A ESA CONSOLA Y EL SOCKET QUE LE CORRESPONDE
										sizeof(t_listaSocketsConsola));
								nodoConsola->pid = pid;
								nodoConsola->socketConsola = socketCliente;
								list_add(listaSocketsConsola, &nodoConsola);
								pthread_mutex_lock(&mutex);
								list_add(listaReady, tcb);//AGREGA A LA LISTA DE READY Y SUBE EL SEMAFORO PARA QUE EL HILO CORRA
								sem_post(&hayEnReady);
								pthread_mutex_unlock(&mutex);

							}	//FIN tcb_resultado.exito

						}	//FIN codigo==1.
						if (codigo == 123) {
							FD_SET(socketCliente, &master);
							if (socketCliente > setmax) {
								setmax = socketCliente;
							}
							int * socketClienteAux = malloc(sizeof(int));//AGREGO A LA COLA DE SOCKETS DE CPUS AL SOCKET CLIENTE
							*socketClienteAux = socketCliente;
							list_add(listaSocketsCpu, socketClienteAux);
							pthread_mutex_lock(&mutex);
							list_add(listaCpuLibres, socketClienteAux);	//AGREGO A LA COLA DE CPUS LIBRES AL SOCKET CLIENTE Y SUBO EL SEMAFORO PARA EL HILO
							sem_post(&hayCpu);
							pthread_mutex_unlock(&mutex);
						}	//FIN codigo==2.

					}	//FIN DEL i==listenningSocket.
					if (estaEnlaListaSocketsConsola(listaSocketsConsola, i)) {
						int recibido;
						int intInutil;
						if ((recibido = recv(i, &intInutil, sizeof(int), 0))
								== -1) {
							printf("Fallo el recv de una Cpu\n");
							exit(1);
						}	//FIN DEL recibido= recv(i,...).
						if (recibido == 0) {
							pthread_mutex_lock(&mutex);
							t_listaSocketsConsola * nodoRemovido =
									removerDeLaListaSocketsConsola(
											listaSocketsConsola, i);
							int * pidBaneado = malloc(sizeof(int));
							*pidBaneado = nodoRemovido->pid;//SE SACA DE LA LISTA A EL NODO CORRESPONDIENTE A LA CONSOLA DESCONECTADA. SE GUARDA EL PID Y SE LO AGREGA A LA LISTA DE PID BANEADOS (ESTO SE VA A USAR PARA CORTAR LA EJECUCION CUANDO ALGUNA CPU DEVUELVA HILOS ASOCIADOS A ESTE PID).
							list_add(listaPidBaneados, pidBaneado);	//SE REMUEVEN DE TODAS LAS LISTAS MENOS LA DE EXEC LOS NODOS ASOCIADOS AL PID DE LA CONSOLA DESCONECTADA.
							while (hayTcbConElPid(listaReady, *pidBaneado)) {//ACA SE SACAN TODOS LOS NODOS ASOCIADOS AL PID,EXCEPTO EL KM EN CASO DE QUE ESTE ASOCIADO.
								t_tcb * tcbRemovido = removerTcbConElPid(
										listaReady, *pidBaneado);
								list_add(listaExit, tcbRemovido);
							}	//FIN DEL while(hayTcbConElPid(listaReady,...).
							while (hayTcbConElPid(listaBloq, *pidBaneado)) {
								t_tcb * tcbRemovido = removerTcbConElPid(
										listaBloq, *pidBaneado);
								list_add(listaExit, tcbRemovido);
							}	//FIN DEL while(hayTcbConElPid(listaBloq,...).
							while (hayNodoJoinConElPid(listaBloqJoin,
									*pidBaneado)) {
								t_join * nodoRemovido = removerNodoJoinDelPid(
										listaBloqJoin, *pidBaneado);
								t_tcb * tcbRemovido = nodoRemovido->tcb;
								list_add(listaExit, tcbRemovido);
							}//FIN DEL while(hayNodoJoinConELPid(listaBloqJoin,...).
							while (hayNodoHilosConElPid(listaHilos, *pidBaneado)) {
								removerNodoHilosDelPid(listaHilos, *pidBaneado);
							}	//FIN DEL while(hayNodoHilosCon...)
							while (hayNodoRecursoConElPid(listaBloqRecurso,
									*pidBaneado)) {
								t_nodoRecurso * nodoRemovido =
										removerNodoRecursoDelPid(
												listaBloqRecurso, *pidBaneado);
								t_tcb * tcbRemovido = nodoRemovido->tcb;
								list_add(listaExit, tcbRemovido);
							}	//FIN DEL while(hayNodoRecursoConEl...)
							pthread_mutex_unlock(&mutex);
						}
						//FIN DEL recibido==0.

					}//FIN DEL if (estaEnlaListaSocketsConsola(listaSocketsConsola, i).
					if (estaEnLaListaElInt(listaSocketsCpu, i)) {
						int recibido;
						int unaOperacion;
						if ((recibido = recv(i, &unaOperacion, sizeof(int), 0))
								== -1) {
							printf("Fallo el recv de la Cpu: %d\n", i);
							exit(1);
						}	//FIN DEL if((recibido=recv(i,...)).
						if (recibido == 0) {
							pthread_mutex_lock(&mutex);
							removerDeLaListaElInt(listaSocketsCpu, i);
							removerDeLaListaElInt(listaCpuLibres, i);
							if (hayTcbConElSocketCpu(listaExec, i)) {//***recordar que es importante en el hilo asignarle la CPU a la que se lo manda al TCB.
								t_tcb * tcbQueSeEjecutaba =
										removerTcbConElSocketCpu(listaExec, i);	//SE REMUEVE DE LA LISTA EXEC EL TCB QUE SE EJECUTABA EN LA CPU QUE SE DESCONECTO. SE DEJAN EN LA LISTA EXEC LOS TCBS ASOCIADOS AL PID DEL QUE SE REMOVIO.
								list_add(listaExit, tcbQueSeEjecutaba);
								int * pidBaneado = malloc(sizeof(int));
								*pidBaneado = tcbQueSeEjecutaba->pid;
								list_add(listaPidBaneados, pidBaneado);
								while (hayTcbConElPid(listaReady, *pidBaneado)) {//SE REMUEVEN DE TODAS LAS LISTAS MENOS LA DE EXEC LOS NODOS ASOCIADOS AL PID DEL TCB QUE SE EJECUTABA EN LA CPU DESCONECTADA.
									t_tcb * tcbRemovido = removerTcbConElPid(
											listaReady, *pidBaneado);//ACA SE SACAN TODOS LOS NODOS ASOCIADOS AL PID,EXCEPTO EL KM EN CASO DE QUE ESTE ASOCIADO.
									list_add(listaExit, tcbRemovido);// ***se tienen que mandar los hijos tambien a la cola de exit, no?
								}//FIN DEL while(hayTcbConElPid(listaReady,...).
								while (hayTcbConElPid(listaBloq, *pidBaneado)) {
									t_tcb * tcbRemovido = removerTcbConElPid(
											listaBloq, *pidBaneado);
									list_add(listaExit, tcbRemovido);
								}//FIN DEL while(hayTcbConElPid(listaBloq,...).
								while (hayNodoJoinConElPid(listaBloqJoin,
										*pidBaneado)) {
									t_join * nodoRemovido =
											removerNodoJoinDelPid(listaBloqJoin,
													*pidBaneado);
									t_tcb * tcbRemovido = nodoRemovido->tcb;
									list_add(listaExit, tcbRemovido);
								}//FIN DEL while(hayNodoJoinConELPid(listaBloqJoin,...).
								while (hayNodoHilosConElPid(listaHilos,
										*pidBaneado)) {
									removerNodoHilosDelPid(listaHilos,
											*pidBaneado);
								}	//FIN DEL while(hayNodoHilosCon...)
								while (hayNodoRecursoConElPid(listaBloqRecurso,
										*pidBaneado)) {
									t_nodoRecurso * nodoRemovido =
											removerNodoRecursoDelPid(
													listaBloqRecurso,
													*pidBaneado);
									t_tcb * tcbRemovido = nodoRemovido->tcb;
									list_add(listaExit, tcbRemovido);
								}//FIN DEL while(hayNodoRecursoConElPid(listaBloqRecurso,...).
								enviar_serializado(1,
										"Concluyo ejecucion abruptamente por desconexion de una CPU que atendia el proceso",
										tcbQueSeEjecutaba->socketConsola);
								enviarInt(0, tcbQueSeEjecutaba->socketConsola);

							}//FIN DEL if(hayTcbConElSocketCpu(listaExec,...).
							pthread_mutex_unlock(&mutex);
						}			//FIN DEL if(recibido==0).
						else {
							switch (unaOperacion) {
							case TERMINO_QUANTUM: {
								t_tcb * tcbCpu = malloc(sizeof(t_tcb));
								tcbCpu = recibirTcb(i);
								if (estaEnLaListaElInt(listaPidBaneados,
										tcbCpu->pid)) {
									pthread_mutex_lock(&mutex);
									if (tcbCpu->km == 1) {
										sacarElKM(listaExec);
										queue_push(colaKM, tcbCpu);
									}
									int * iAux = malloc(sizeof(int));
									*iAux = i;
									list_add(listaCpuLibres, iAux);
									pthread_mutex_unlock(&mutex);
									sem_post(&hayCpu);
									list_add(listaExit, tcbCpu);
									break;
								}//FIN DEL if(estaEnLaListaElInt(listaPidBaneados,...).
								pthread_mutex_lock(&mutex);
								removerTcbConElSocketCpu(listaExec, i);
								if (tcbCpu->km == 1) {
									queue_push(colaKM, tcbCpu);
									t_tcb * tcbBloqueado;
									tcbBloqueado = removerTcbConElTid(listaBloq,
											tcbCpu->tid);
									tcbBloqueado->A = tcbCpu->A;
									tcbBloqueado->B = tcbCpu->B;
									tcbBloqueado->C = tcbCpu->C;
									tcbBloqueado->D = tcbCpu->D;
									tcbBloqueado->E = tcbCpu->E;
									list_add(listaReady, tcbBloqueado);
								} else {
									list_add(listaReady, tcbCpu);//***chequear que no queden queue_push no asociados a la colaKM
								}			//FIN DEL if(tcbCpu->km==1).
								int * iAux = malloc(sizeof(int));
								*iAux = i;
								list_add(listaCpuLibres, iAux);
								pthread_mutex_unlock(&mutex);
								sem_post(&hayEnReady);
								sem_post(&hayCpu);
								break;
							}			//FIN DEL case TERMINO_QUANTUM.
							case CONCLUYO_EJECUCION: {
								t_tcb * tcbCpu = malloc(sizeof(t_tcb));
								tcbCpu = recibirTcb(i);
								if (estaEnLaListaElInt(listaPidBaneados,
										tcbCpu->pid)) {
									pthread_mutex_lock(&mutex);
									int * iAux = malloc(sizeof(int));
									*iAux = i;
									list_add(listaCpuLibres, iAux);
									pthread_mutex_unlock(&mutex);
									sem_post(&hayCpu);
									list_add(listaExit, tcbCpu);
									break;
								}//FIN DEL if(estaEnLaListaElInt(listaPid...,...).

								if (hayNodoJoinConElTid(listaBloqJoin,
										tcbCpu->pid, tcbCpu->tid)) {
									t_join * nodoRemovido =
											removerNodoJoinDelTid(listaBloqJoin,
													tcbCpu->pid, tcbCpu->tid);
									t_tcb * tcbDesbloqueado = nodoRemovido->tcb;
									pthread_mutex_lock(&mutex);
									list_add(listaReady, tcbDesbloqueado);
									pthread_mutex_unlock(&mutex);
									sem_post(&hayEnReady);
								}
								t_listaHilos * nodoRemovido =
										removerNodoHilosDelPid(listaHilos,
												tcbCpu->pid);
								pthread_mutex_lock(&mutex);
								removerTcbConElSocketCpu(listaExec, i);
								int * iAux = malloc(sizeof(int));
								*iAux = i;
								list_add(listaCpuLibres, iAux);
								pthread_mutex_unlock(&mutex);
								sem_post(&hayCpu);
								list_add(listaExit, tcbCpu);
								if (nodoRemovido->hilos == 1) {	//de aca hasta...
									enviar_serializado(1,
											"Concluyo ejecucion normalmente",
											tcbCpu->socketConsola);
									enviarInt(0, tcbCpu->socketConsola);
								} else {
									nodoRemovido->hilos = nodoRemovido->hilos
											- 1;
									list_add(listaHilos, nodoRemovido);
								}//...aca, saque para probar Despertar y Bloquear
								break;
							}	//FIN DEL case CONCLUYO EJECUCION.
							case EJECUCION_ERRONEA: {
								t_tcb * tcbCpu = malloc(sizeof(t_tcb));
								tcbCpu = recibirTcb(i);
								if (estaEnLaListaElInt(listaPidBaneados,
										tcbCpu->pid)) {
									pthread_mutex_lock(&mutex);
									if (tcbCpu->km == 1) {
										//***aca hay que abortar el kernel, o solo abortar el proceso asociado al pid que tenia el KM cuando se ejecuto erroneamente??
									}
									int * iAux = malloc(sizeof(int));
									*iAux = i;
									list_add(listaCpuLibres, iAux);
									pthread_mutex_unlock(&mutex);
									sem_post(&hayCpu);
									list_add(listaExit, tcbCpu);
									break;
								}
								pthread_mutex_lock(&mutex);
								t_tcb * tcbQueSeEjecutaba =
										removerTcbConElSocketCpu(listaExec, i);
								list_add(listaExit, tcbCpu);
								int * pidBaneado = malloc(sizeof(int));
								*pidBaneado = tcbCpu->pid;
								list_add(listaPidBaneados, pidBaneado);
								while (hayTcbConElPid(listaReady, tcbCpu->pid)) {//SE REMUEVEN DE TODAS LAS LISTAS MENOS LA DE EXEC LOS NODOS ASOCIADOS AL PID DEL TCB QUE SE EJECUTABA EN LA CPU DESCONECTADA.
									t_tcb * tcbRemovido = removerTcbConElPid(
											listaReady, tcbCpu->pid);//ACA SE SACAN TODOS LOS NODOS ASOCIADOS AL PID,EXCEPTO EL KM EN CASO DE QUE ESTE ASOCIADO.
									list_add(listaExit, tcbRemovido);// ***se tienen que mandar los hijos tambien a la cola de exit, no?
								}//FIN DEL while(hayTcbConElPid(listaReady,...).
								while (hayTcbConElPid(listaBloq, tcbCpu->pid)) {
									t_tcb * tcbRemovido = removerTcbConElPid(
											listaBloq, tcbCpu->pid);
									list_add(listaExit, tcbRemovido);
								}//FIN DEL while(hayTcbConElPid(listaBloq,...).
								while (hayNodoJoinConElPid(listaBloqJoin,
										tcbCpu->pid)) {
									t_join * nodoRemovido =
											removerNodoJoinDelPid(listaBloqJoin,
													tcbCpu->pid);
									t_tcb * tcbRemovido = nodoRemovido->tcb;
									list_add(listaExit, tcbRemovido);
								}//FIN DEL while(hayNodoJoinConELPid(listaBloqJoin,...).
								while (hayNodoHilosConElPid(listaHilos,
										tcbCpu->pid)) {
									removerNodoHilosDelPid(listaHilos,
											tcbCpu->pid);
								}	//FIN DEL while(hayNodoHilosCon...)
								while (hayNodoRecursoConElPid(listaBloqRecurso,
										tcbCpu->pid)) {
									t_nodoRecurso * nodoRemovido =
											removerNodoRecursoDelPid(
													listaBloqRecurso,
													tcbCpu->pid);
									t_tcb * tcbRemovido = nodoRemovido->tcb;
									list_add(listaExit, tcbRemovido);
								}//FIN DEL while(hayNodoRecursoConElPid(listaBloqRecurso,...).
								enviar_serializado(1,
										"Concluyo ejecucion por ejecucion erronea",
										tcbCpu->socketConsola);
								enviarInt(0, tcbCpu->socketConsola);
								int * iAux = malloc(sizeof(int));
								*iAux = i;
								list_add(listaCpuLibres, iAux);
								pthread_mutex_unlock(&mutex);
								sem_post(&hayCpu);
								break;
							}	//FIN DEL case EJECUCION_ERRONEA
							case SYSCALL: {
								t_tcb * tcbCpu = malloc(sizeof(t_tcb));
								tcbCpu = recibirTcb(i);
								uint32_t direccionSyscall = recibirInt32(i);
								if (estaEnLaListaElInt(listaPidBaneados,
										tcbCpu->pid)) {
									pthread_mutex_lock(&mutex);
									int * iAux = malloc(sizeof(int));
									*iAux = i;
									list_add(listaCpuLibres, iAux);
									pthread_mutex_unlock(&mutex);
									sem_post(&hayCpu);
									list_add(listaExit, tcbCpu);
									break;
								}//FIN del if(estaEnLaListaElInt(listaPid...,...).
								tcbCpu->direccionSyscallPendiente =
										direccionSyscall;
								pthread_mutex_lock(&mutex);
								removerTcbConElSocketCpu(listaExec, i);
								int * iAux = malloc(sizeof(int));
								*iAux = i;
								list_add(listaCpuLibres, iAux);
								list_add(listaBloq, tcbCpu);
								pthread_mutex_unlock(&mutex);
								sem_post(&hayCpu);
								break;
							}	//FIN DEL case SYSCALL
							case ENTRADA_ESTANDAR: {
								int pidEE;
								int tipoEE;
								pidEE = recibirInt(i);//TIPO EE PARA SABER SI HAY QUE INGRESAR UN INT O UN STRING(0=Int,1=String)
								tipoEE = recibirInt(i);
								if (estaEnLaListaElInt(listaPidBaneados,
										pidEE)) {
									if (tipoEE == 0) {
										enviarInt(7, i);//BASURA,LO MANDO PARA QUE CONTIENE LA CPU EJECUTANDO
									}	//FIN DEL if(tipoEE==0)
									if (tipoEE == 1) {
										int inputMax;
										inputMax = recibirInt(i);//***anda enviar el "a" asi, no?
										enviar_serializado(-1, "a", i); //BASURA,LO MANDO PARA QuE CONTIENE LA CPU EJECUTANDO
									}	//FIN DEL if(tipoEE==1)

									break;
								}//FIN DEL if(estaEnLaListaElInt(listaPid...,...).
								if (tipoEE == 0) {
									pthread_mutex_lock(&mutex);
									t_tcb * tcbBloqueado = obtenerTcbConElPid(
											listaBloq, pidEE);
									enviarInt(3, tcbBloqueado->socketConsola);
									int iret2;
									pthread_t manejoEntradaEstandar;
									t_paraEntradaEstandar * nodoEE = malloc(
											sizeof(t_paraEntradaEstandar));
									nodoEE->socketConsola =
											tcbBloqueado->socketConsola;
									nodoEE->socketCpu = i;
									nodoEE->tipoEE = tipoEE;
									iret2 = pthread_create(
											&manejoEntradaEstandar, NULL,
											esperarEntradaEstandar, nodoEE);
									if (iret2) {
										fprintf(stderr,
												"Fallo  creacion hilo esperarEntradaEstandar retorno: %d\n",
												iret2);
										exit(1);
									}	//FIN DEL if(iret2).
									pthread_mutex_unlock(&mutex);
								}		//FIN DEL if(tipoEE==0).
								if (tipoEE == 1) {
									int inputMax;
									inputMax = recibirInt(i);
									pthread_mutex_lock(&mutex);
									t_tcb * tcbBloqueado = obtenerTcbConElPid(
											listaBloq, pidEE);
									pthread_mutex_unlock(&mutex);
									enviarInt(2, tcbBloqueado->socketConsola);
									enviarInt(inputMax,
											tcbBloqueado->socketConsola);
									int iret3;
									pthread_t manejoEntradaEstandar;
									t_paraEntradaEstandar * nodoEE = malloc(
											sizeof(t_paraEntradaEstandar));
									nodoEE->socketConsola =
											tcbBloqueado->socketConsola;
									nodoEE->socketCpu = i;
									nodoEE->tipoEE = tipoEE;
									iret3 = pthread_create(
											&manejoEntradaEstandar, NULL,
											esperarEntradaEstandar, nodoEE);
									if (iret3) {
										fprintf(stderr,
												"Fallo  creacion hilo esperarEntradaEstandar retorno: %d\n",
												iret3);
										exit(1);
									}	//FIN DEL if(iret3).
								}		//FIN DEL if(tipoEE==1).
								break;
							}	//FIN DEL case ENTRADA_ESTANDAR
							case SALIDA_ESTANDAR: {
								int pidSE;
								char * string;
								pidSE = recibirInt(i);
								string = recibir_serializado(i);
								if (estaEnLaListaElInt(listaPidBaneados,
										pidSE)) {
									break;
								}//FIN DEL if(estaEnLAListaElInt(listaPidBaneados,pidSE).
								pthread_mutex_lock(&mutex);
								t_tcb * tcbBloqueado = obtenerTcbConElPid(
										listaBloq, pidSE);
								pthread_mutex_unlock(&mutex);
								enviarInt(1, tcbBloqueado->socketConsola);
								enviar_serializado(-1, string,
										tcbBloqueado->socketConsola);
								break;
							}	//FIN DEL case SALIDA_ESTANDAR
							case CREAR_HILO:{

							}//FIN DEL case CREA
							}			//FIN del switch.
						}			//FIN DEL else.
					}	//FIN DEL if(estaEnLaListaElInt(listaSocketsCpu,i).
				}
				//FIN DEL FD_ISSET(i,...).

			}	//FIN DEL FOR(i=0,...).
		}	//FIN DEL ELSE.
	} //FIN DEL WHILE(1).

	return 0;

}

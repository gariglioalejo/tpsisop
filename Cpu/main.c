#include "cpu.h"

int main(int argc, char** argv) {

	printf("Se levanto una CPU\n");

	char* puerto_kernel;
	char* puerto_memoria;
	char* ip_kernel;
	char* ip_memoria;
	int retardo;

	t_tcb * tcb;

	int i;

	t_log * log_de_cpu;
	log_de_cpu = log_create("log_de_cpu", "cpu.c", 0, LOG_LEVEL_TRACE);

	log_info(log_de_cpu,
			"-x-x-x-x-x-x-x-x-x---CPU INICIALIZADA---x-x-x-x-x-x-x-x-x-x-x- \n");

	if (argc != 2) {
		log_error(log_de_cpu, "Insuficientes parametros recibidos, ABORTO\n");
		exit(1);
	}

	//PANEL
	inicializar_panel(CPU,"/home/utnso/git/tp-2014-2c-nmi/panel");
	t_registros_cpu registrosPanel;
	t_hilo * hiloPanel = malloc(sizeof(t_hilo));




	//Leo el archivo de cf
	t_config* config = config_create(argv[1]);

	ip_kernel = config_get_string_value(config, "IP_KERNEL");
	puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
	ip_memoria = config_get_string_value(config, "IP_MSP");
	puerto_memoria = config_get_string_value(config, "PUERTO_MSP");
	retardo = config_get_int_value(config, "RETARDO");

	//Creo el socket Kernel
	socketK = conectarse(ip_kernel, puerto_kernel);

	if (socketK > 0) {
		log_info(log_de_cpu, "Se conecto la cpu al kernel\n");
	} else {
		log_error(log_de_cpu,
				"Error al intentar conectarse con el kernel. ABORTO. \n");
		exit(1);
	}

	//Envio handshake a kernel AVISAAAAR
	int handshake = 123;
	send(socketK, &handshake, sizeof(int), 0);

	//Creo el socket MSP
	socketM = conectarse(ip_memoria, puerto_memoria);

	if (socketM > 0) {
		log_info(log_de_cpu, "Se conecto la cpu a la MSP\n");
	} else {
		log_error(log_de_cpu,
				"Error al intentar conectarse con la MSP. ABORTO. \n");
		exit(1);
	}

	//Recibo el quantum del kernel AVISAAAAAAAR
	int quantum;
	recv(socketK, &quantum, sizeof(int), 0);

	//ciclo infinito
	while (1) {

		char* primeras4;
		primeras4 = malloc(5);

		//se recibe el tcb del kernel
		tcb = recibirTcb(socketK);

		//==Hola soy un Panel
		//==Hola Panel, como estas?
		copiarHiloPanel(hiloPanel,tcb);
		copiarRegistrosPanel(&registrosPanel,tcb);
		comienzo_ejecucion(hiloPanel,quantum);


		i = 0;
		ultimainstruccion = 0;
		systemcall = 0;
		segmentatioFault = 0;

		if (tcb->km == 1) {

			while ((!ultimainstruccion)&(!segmentatioFault) ){

				log_info(log_de_cpu, "EJECUTANDO KERNEL MODE");

				primeras4 = pedirPrimeraPalabraKM(socketM, tcb);
				primeras4[4] = '\0';
				parseador(primeras4, tcb);
				copiarRegistrosPanel(&registrosPanel,tcb);
				cambio_registros(registrosPanel);

				sleep(retardo / 1000);

			}
			
			if (ultimainstruccion > 0) {
				int encolarBien = 0;
				send(socketK, &encolarBien, sizeof(int), 0);
				enviarTcb(tcb, socketK);
				printf("TCB KM ENVIADO A TERMINO EJECUCION \n");
				printf("--------------- \n");
			} else {
				//SegmentationFault
				if (segmentatioFault > 0) {
					int encolarSegFault = 2;
					send(socketK, &encolarSegFault, sizeof(int), 0);
					enviarTcb(tcb, socketK);
					printf("TCB KM ENVIADO A SEGMENTATION FAULT \n");
					printf("--------------- \n");
				}
		}}else {
			//mientras el quantum deje y no haya una llamada al sistema
			while ((i < quantum) && (!systemcall) && (!ultimainstruccion)
					&& (!segmentatioFault)) {

				log_info(log_de_cpu, "XXXXXXXXXXXXXX QUANTUM %d XXXXXXXXXXXXX",
						quantum - i);
				i++;

				//Solicitar instruccion a la MSP

				primeras4 = pedirPrimeraPalabra(socketM, tcb);
				primeras4[4] = '\0';

				//LLama al parser creado por cada instruccion y a dicha funcion

				parseador(primeras4, tcb);
				copiarRegistrosPanel(&registrosPanel,tcb);

				cambio_registros(registrosPanel);


				sleep(retardo / 1000);

			}

			if (ultimainstruccion > 0) {
				int encolarEnExit = 1;
				send(socketK, &encolarEnExit, sizeof(int), 0);
				enviarTcb(tcb, socketK);
				fin_ejecucion();
				printf("TCB ENVIADO A EXIT \n");
				printf("--------------- \n");
			} else {
				//SegmentationFault
				if (segmentatioFault > 0) {
					int encolarSegFault = 2;
					send(socketK, &encolarSegFault, sizeof(int), 0);
					enviarTcb(tcb, socketK);
					fin_ejecucion();
					printf("TCB ENVIADO A SEGMENTATION FAULT \n");
					printf("--------------- \n");
				} else {

					//Block
					if (systemcall > 0) {
						//int encolarEnBloqueado=3;
						//send(socketK,&encolarEnBloqueado,sizeof(int),0);
						//enviarTcb(tcb,socketK);
						fin_ejecucion();
					} else {

						if (quantum == i) {
							int encolarEnReady = 0;
							send(socketK, &encolarEnReady, sizeof(int), 0);
							//send(socketK,&tcb,sizeof(t_tcb),0);
							enviarTcb(tcb, socketK);
							fin_ejecucion();
							printf("TCB ENVIADO A READY \n");
							printf("mensaje oculto: MARTIN BASILE Y SU CHEESCAKE \n");
							printf("--------------- \n");

						}

					}

				}

			}

			//Ready

			//Exit

			free(primeras4);
		}

	}

	free(hiloPanel);
	log_info(log_de_cpu, "-x-x-x-x-x-x---CPU CERRADA---x-x-x-x-x-x-");
	return 0;
}


//==PANEL AUX

void copiarHiloPanel(t_hilo * hiloPanel, t_tcb * tcb) {
	hiloPanel->base_stack = tcb->X;
	hiloPanel->cola = EXEC;
	hiloPanel->cursor_stack = tcb->S;
	hiloPanel->kernel_mode = (tcb->km == 1);
	hiloPanel->pid = tcb->pid;
	hiloPanel->puntero_instruccion = tcb->P;
	hiloPanel->registros[0] = tcb->registroA.valores;
	hiloPanel->registros[1] = tcb->registroB.valores;
	hiloPanel->registros[2] = tcb->registroC.valores;
	hiloPanel->registros[3] = tcb->registroD.valores;
	hiloPanel->registros[4] = tcb->registroE.valores;
	hiloPanel->segmento_codigo = tcb->M;
	hiloPanel->segmento_codigo_size = tcb->tam_seg_cod;
	hiloPanel->tid = tcb->tid;

}

void copiarRegistrosPanel(t_registros_cpu * registrosPanel, t_tcb * tcb) {


	registrosPanel->I = tcb->pid;
	registrosPanel->K = tcb->km;
	registrosPanel->M = tcb->M;
	registrosPanel->P = tcb->P;
	registrosPanel->S = tcb->S;
	registrosPanel->X = tcb->X;
	registrosPanel->registros_programacion[0] = tcb->registroA.valores;
	registrosPanel->registros_programacion[1] = tcb->registroB.valores;
	registrosPanel->registros_programacion[2] = tcb->registroC.valores;
	registrosPanel->registros_programacion[3] = tcb->registroD.valores;
	registrosPanel->registros_programacion[4] = tcb->registroE.valores;

}




void panel_instruccion(char** params, int cant, char* instruccion) {
	t_list* argumentos = list_create();
	int i;
	for (i = 0; i < cant; i++) {
		list_add(argumentos, string_duplicate(params[i]));
	}

	ejecucion_instruccion(instruccion, argumentos);
	list_destroy_and_destroy_elements(argumentos, free);

}



//===INICIO PANEL
void inicializar_panel(t_tipo_proceso tipo_proceso, char* path){
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
	logp = log_create(logFile, tipo_proceso_str, true, LOG_LEVEL_INFO);

	log_info(logp, "Inicializando panel para %s, en \"%s\"", tipo_proceso_str, logFile);

	free(logFile);

}





void comienzo_ejecucion(t_hilo* hilo, uint32_t quantum) {
	char* mensaje = string_new();

	string_append_with_format(&mensaje, "Ejecuta hilo { PID: %d, TID: %d }", hilo->pid, hilo->tid);
	if (hilo->kernel_mode) string_append(&mensaje, " en modo kernel");

	log_info(logp, mensaje);
	free(mensaje);
}

void fin_ejecucion() {
	log_info(logp, "Empieza a estar iddle");
}

void ejecucion_instruccion(char* mnemonico, t_list* parametros) {
	char* mensaje = string_new();

	string_append_with_format(&mensaje, "Instrucción %s [", mnemonico);

	bool primero = true;
	void _imprimirParametro(char* parametro) {
		if (!primero) string_append(&mensaje, ", ");
		string_append_with_format(&mensaje, "%s", parametro);
		primero = false;
	}
	list_iterate(parametros, (void*) _imprimirParametro);

	string_append(&mensaje, "]");

	log_info(logp, mensaje);
	free(mensaje);
}

void cambio_registros(t_registros_cpu registros) {
	log_info(logp, "Registros: { A: %d, B: %d, C: %d, D: %d, E: %d, M: %d, P: %d, S: %d, K: %d, I: %d }",
		registros.registros_programacion[0],
		registros.registros_programacion[1],
		registros.registros_programacion[2],
		registros.registros_programacion[3],
		registros.registros_programacion[4],
		registros.M, registros.P, registros.S, registros.K, registros.I
	);
}

//-------------------------------------------------
//Retrocompatibilidad con el ejemplo del enunciado:
void ejecucion_hilo(t_hilo* hilo, uint32_t quantum) {
	comienzo_ejecucion(hilo, quantum);
}
//===FIN PANEL

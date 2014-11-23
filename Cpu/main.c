#include "cpu.h"


int main (int argc, char** argv){

	char* puerto_kernel;
	char* puerto_memoria;
	char* ip_kernel;
	char* ip_memoria;
	int retardo;
	int dirsyscall;
	t_tcb * tcb;
	int instruccion = 0;
	int i;


	t_log * log_de_cpu;
	log_de_cpu = log_create("log_de_cpu", "cpu.c", 0, LOG_LEVEL_TRACE);

	log_info(log_de_cpu,"-x-x-x-x-x-x-x-x-x---CPU INICIALIZADA---x-x-x-x-x-x-x-x-x-x-x- \n");

	if (argc != 2) {
			log_error(log_de_cpu, "Insuficientes parametros recibidos, ABORTO\n");
			exit(1);
		}

	//Leo el archivo de cf
	t_config* config = config_create(argv[1]);

	ip_kernel = config_get_string_value(config, "IP_KERNEL");
	puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
	ip_memoria = config_get_string_value(config, "IP_MSP");
	puerto_memoria = config_get_string_value(config, "PUERTO_MSP");
	retardo= config_get_int_value(config, "RETARDO");


	//Creo el socket Kernel
	socketK = conectarse(ip_kernel,puerto_kernel);

	if(socketK>0){
		log_info(log_de_cpu,"Se conecto la cpu al kernel\n");
	}
	else{
		log_error(log_de_cpu,"Error al intentar conectarse con el kernel. ABORTO. \n");
		exit(1);
	}

	//Envio handshake a kernel AVISAAAAR
	int handshake = 123;
	send(socketK,&handshake,sizeof(int),0);

	//Creo el socket MSP
	socketM = conectarse(ip_memoria,puerto_memoria);

	if(socketM>0){
			log_info(log_de_cpu,"Se conecto la cpu a la MSP\n");
		}
		else{
			log_error(log_de_cpu,"Error al intentar conectarse con la MSP. ABORTO. \n");
			exit(1);
		}


	//Recibo el quantum del kernel AVISAAAAAAAR
	int quantum;
	recv(socketK,&quantum,sizeof(int),0);


	//ciclo infinito
	while(1){

		char* primeras4;
		primeras4=malloc(5);

		//se recibe el tcb del kernel
		tcb=recibirTcb(socketK);

		i=0;
		ultimainstruccion=0;
		systemcall=0;
		segmentatioFault=0;

		if(tcb->km==1){

		while(!ultimainstruccion){

			log_info(log_de_cpu,"EJECUTANDO KERNEL MODE");

			primeras4=pedirPrimeraPalabraKM(socketM,tcb);
			primeras4[4]='\0';
			parseador(primeras4,tcb);

			sleep(retardo/1000);

		}
		//Devuelvo el TCB KM1
		int devuelvoKernelMode=0;
		send(socketK,&devuelvoKernelMode,sizeof(int),0);
		//send(socketK,&tcb,sizeof(t_tcb),0);
		printf("tcb enviado: %d\n",tcb->pid);
		enviarTcb(tcb,socketK);

		}

		else {
		//mientras el quantum deje y no haya una llamada al sistema
		while((i<quantum)&&(!systemcall)&&(!ultimainstruccion)&&(!segmentatioFault)){

			log_info(log_de_cpu,"XXXXXXXXXXXXXX QUANTUM %d XXXXXXXXXXXXX",quantum-i);
			i++;

			//Solicitar instruccion a la MSP


			primeras4=pedirPrimeraPalabra(socketM,tcb);
			primeras4[4]='\0';

			//LLama al parser creado por cada instruccion y a dicha funcion

			parseador(primeras4,tcb);

			sleep(retardo/1000);

			}


		if(ultimainstruccion>0){
			int encolarEnExit=1;
			send(socketK,&encolarEnExit,sizeof(int),0);
			enviarTcb(tcb,socketK);
		} else {
			//SegmentationFault
			if(segmentatioFault>0){
				int encolarSegFault=2;
				send(socketK,&encolarSegFault,sizeof(int),0);
				enviarTcb(tcb,socketK);
				} else {

					//Block
					if(systemcall>0){
						//int encolarEnBloqueado=3;
						//send(socketK,&encolarEnBloqueado,sizeof(int),0);
						//enviarTcb(tcb,socketK);
					} else {

						if (quantum==i){
							int encolarEnReady=0;
							send(socketK,&encolarEnReady,sizeof(int),0);
							//send(socketK,&tcb,sizeof(t_tcb),0);
							enviarTcb(tcb,socketK);
						}



					}



				}



		}

		//Ready



		//Exit

		

		free(primeras4);
		}

	}
	log_info(log_de_cpu,"-x-x-x-x-x-x---CPU CERRADA---x-x-x-x-x-x-");
	return 0;
}

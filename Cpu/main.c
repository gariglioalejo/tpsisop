#include <commons/log.h>
#include "tpsisoplib.h"

int main (int argc, char** argv){

	char* puerto_kernel;
	char* puerto_memoria;
	char* ip_kernel;
	char* ip_memoria;
	int retardo;

	t_log * log_de_cpu;
	log_de_cpu = log_create("log_de_cpu", "cpu.c", 0, LOG_LEVEL_TRACE);

	log_info(log_de_cpu,"-x-x-x-x-x-x-x-x-x---CPU INICIALIZADA---x-x-x-x-x-x-x-x-x-x-x- \n");

	if (argc != 2) {
			log_error(log_de_cpu, "Insuficientes parametros recibidos, ABORTO\n");
			exit(1);
		}

	//Leo el archivo de cfg
	t_config* config = config_create(argv[1]);

	ip_kernel = config_get_string_value(config, "IP_KERNEL");
	puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
	ip_memoria = config_get_string_value(config, "IP_MSP");
	puerto_memoria = config_get_string_value(config, "PUERTO_MSP");
	retardo= config_get_int_value(config, "RETARDO");


	//Creo el socket Kernel
	int socketK = conectarse(ip_kernel,puerto_kernel);

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
	int socketM = conectarse(ip_memoria,puerto_memoria);

	if(socketM>0){
			log_info(log_de_cpu,"Se conecto la cpu a la MSP\n");
		}
		else{
			log_error(log_de_cpu,"Error al intentar conectarse con la MSP. ABORTO. \n");
			exit(1);
		}

	//Envio handshake AVISAAAAARRR
	send(socketM,&handshake,sizeof(int),0);

	//Recivo el quantum del kernel AVISAAAAAAAR
	int quantum;
	recv(socketK,&quantum,sizeof(int),0);


	log_info(log_de_cpu,"-x-x-x-x-x-x---CPU CERRADA---x-x-x-x-x-x- \n");
	return 0;
}

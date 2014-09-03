#include <commons/log.h>
#include <commons/config.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main (int argc, char** argv){

	char* puerto_kernel;
	char* puerto_memoria;
	char* ip_kernel;
	char* ip_memoria;

	t_log * log_de_cpu;
	log_de_cpu = log_create("log_de_cpu", "cpu.c", 0, LOG_LEVEL_TRACE);

	log_info(log_de_cpu,"CPU inicializada \n");

	if (argc != 2) {
			log_error(log_de_cpu, "Insuficientes parametros recibidos, ABORTO\n");
			exit(1);
		}

	t_config* config = config_create(argv[1]);

	ip_kernel = config_get_string_value(config, "IP_KERNEL");
	puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
	ip_memoria = config_get_string_value(config, "IP_MSP");
	puerto_memoria = config_get_string_value(config, "PUERTO_MSP");


	return 0;
}

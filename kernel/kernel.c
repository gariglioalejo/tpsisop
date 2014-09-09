/*
 * kernel.c
 *
 *  Created on: 05/09/2014
 *      Author: utnso
 */

#include "kernel.h"

//SEMAFOROS
//^SEMAFOROS

//HILOS
//^HILOS

//VARIABLES GLOBALES
t_config *kernel_config;
char *config_path;

t_kernel *kernel;

//^VARIABLES GLOBALES


int main(int argc, char ** argv){

	/*TEST Convertir Direcciones
	t_msp_dir *direccion = convertirIntADir(1772423663);
	printf("Segmento: %d \n Pagina: %d\n Offset: %d\n",direccion->segmento,direccion->pagina,direccion->offset);
	uint32_t intdir = convertirDirAInt(direccion);
	printf("Dir en int: %d\n",intdir);
	*/



	uint32_t result_config = cargarArchivoConfig(config_path);

	if (result_config == 0){
		perror("\nError al cargar Archivo de configuracion\n");
		exit (EXIT_FAILURE);
	}


	uint32_t result_conexion_msp = conectarAMSP();

	if (result_conexion_msp == 0){
		perror("\nError al conectarse a la MSP\n");
		exit (EXIT_FAILURE);
	}



	//TODO Posibles Alternativas
	// A- 1 hilo que sirva para atender las entradas al sistema, que va a tener un SELECT. Si es una consola, lo pone en una lista para que lo agarre el loader... si es una CPU lo pone en una lista de CPU
	//








	return 0;
}

uint32_t cargarArchivoConfig(char * config_path){

	uint32_t result = 0;

	kernel_config = config_create(config_path);

	kernel->listen_port = (uint32_t) config_get_string_value(kernel_config, "PUERTO");
	kernel->ip_MSP = config_get_string_value(kernel_config, "IP_MSP");
	kernel->puerto_MSP = config_get_string_value(kernel_config, "PUERTO_MSP");
	kernel->quantum = (uint32_t) config_get_string_value(kernel_config, "QUANTUM");
	kernel->ruta_syscall = config_get_string_value(kernel_config, "SYSCALLS");


	//param = config_get_string_value(kernel_config, "PARAM");
	//TODO Agregar IF para manejar errores de cargar config


	return result;
}

uint32_t conectarAMSP(void){

	uint32_t result = 0;

	kernel->socketMSP = (uint32_t) conectarse(kernel->ip_MSP,kernel->puerto_MSP);

	if (kernel->socketMSP > 0){
		result = 1;
	}


	return result;
}


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



	int result_config = cargarArchivoConfig(config_path);

	if (!result_config){

		perror("\nError al cargar Archivo de configuracion\n");
		exit (EXIT_FAILURE);
	}







	return 0;
}

int cargarArchivoConfig(char * config_path){

	int result = 0;

	kernel_config = config_create(config_path);
	//param = config_get_string_value(kernel_config, "PARAM");
	//TODO Agregar IF para manejar errores de cargar config


	return result;
}

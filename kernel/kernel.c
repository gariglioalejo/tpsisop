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
pthread_t hiloLoader;
pthread_t hiloPlanificador;
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

	config_path = "kernel.conf";

	kernel = malloc(sizeof(t_kernel));
	uint32_t result_config = cargarArchivoConfig(config_path);

	if (result_config != 0){
		perror("\nError al cargar Archivo de configuracion\n");
		exit (EXIT_FAILURE);
	}


	uint32_t result_conexion_msp = 1;//conectarAMSP();

	if (result_conexion_msp == 0){
		perror("\nError al conectarse a la MSP\n");
		exit (EXIT_FAILURE);
	}



	//Lanzo el Hilo Loader
	if ( pthread_create( &hiloLoader, NULL, fn_hiloLoader, NULL) ) {
	    printf("error al crear el hilo Loader.");
	    exit(EXIT_FAILURE);
	 }

	//Lanzo el Hilo Planificador
	if ( pthread_create( &hiloPlanificador, NULL, fn_hiloPlanificador, NULL) ) {
	    printf("error al crear el hilo Planificador.");
	    exit(EXIT_FAILURE);
	 }



	printf("Soy el hilo principal\n");



	//Join del Hilo Loader
	if ( pthread_join ( hiloPlanificador, NULL ) ) {
	    printf("error al joinear el hilo Planificador.");
	    exit(EXIT_FAILURE);
	}

	//Join del Hilo Loader
	  if ( pthread_join ( hiloLoader, NULL ) ) {
	    printf("error al joinear el hilo Loader.");
	    exit(EXIT_FAILURE);
	  }



	free(kernel);
	return 0;
}



void * fn_hiloLoader(void * args){

	//Funcionalidad del Hilo Loader

	printf("Soy el Hilo Loader\n");

	// TODO Crear socket de escucha de nuevas conexiones
	// TODO Al recibir un nuevo proceso consola crear su TCB y luego reservar en MSP

	return NULL;
}

void * fn_hiloPlanificador(void * args){

	//Funcionalidad del Hilo Planificador

	printf("Soy el Hilo Planificador\n");
	// TODO Crear socket de escucha de nuevas conexiones de CPU
	// TODO Queda a la espera de nuevos programas

	return NULL;
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




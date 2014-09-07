/*
 * kernel.h
 *
 *  Created on: 05/09/2014
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <unistd.h>
#include <commons/config.h>
#include <pthread.h>


//ESTRUCTURAS

typedef struct{

	int listen_port;
	char* ip_MSP;
	int puerto_MSP;
	int quantum;
	char* ruta_syscall;

}t_kernel;


//^ESTRUCTURAS

int main(int argc, char ** argv);
int cargarArchivoConfig(char * config_path);


#endif /* KERNEL_H_ */

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
#include <stdint.h>
#include <math.h>
#include <tpsisoplib.h>

//ESTRUCTURAS

typedef struct{

	uint32_t listen_port;
	char* ip_MSP;
	char* puerto_MSP;
	uint32_t socketMSP;
	uint32_t quantum;
	char* ruta_syscall;

}t_kernel;


//^ESTRUCTURAS

int main(int argc, char ** argv);
uint32_t cargarArchivoConfig(char * config_path);
uint32_t conectarAMSP(void);


#endif /* KERNEL_H_ */

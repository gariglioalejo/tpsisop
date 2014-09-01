/*
 * consola.c
 *
 *  Created on: Aug 30, 2014
 *      Author: utnso
 */

#include "./funciones.h"

FILE * beso;
char * direccion_beso;
struct stat stat_beso;
char * literal_beso;
int i;
t_config *eso_config;
char * ip_kernel;
char * puerto_kernel;
int socketKernel;
int i = 1;
int recibido;
int codigo;
char * mensaje;

int main(int argc, char ** argv) {

	if (argc != 2) {
		printf("No hay argumento\n");
		return 1;
	}
	direccion_beso = argv[1];
	beso = fopen(direccion_beso, "r");
	if (beso == NULL ) {
		printf("No pudo abrirse correctamente el beso\n");
		return 1;
	}
	if (stat(argv[1], &stat_beso)) {
		printf("No se realizo el stat correctamente\n");
		return 1;
	}
	literal_beso = malloc(stat_beso.st_size);
	for (i = 0; (literal_beso[i] = getc(beso)) != EOF; i++)
		;
	literal_beso[i] = '\0';	//Notar que al imprimirlo no muestra un ? raro al final, eso indica que corre bien

//Para setear variable de entorno:
//export ESO_CONFIG=./consola.cfg
//Para mostrar el contenido:
//echo $ESO_CONFIG

	eso_config = config_create(getenv("ESO_CONFIG"));
	ip_kernel = config_get_string_value(eso_config, "IP_KERNEL");
	puerto_kernel = config_get_string_value(eso_config, "PUERTO_KERNEL");
	socketKernel = conectarse(ip_kernel, puerto_kernel);
	serializar_enviar_mensaje(literal_beso, socketKernel);

	while (i) {
		if ((recibido = recv(socketKernel, &codigo, sizeof(int), 0)) == -1) {
			printf("Fallo el recv1\n");
			exit(1);
		}
		if(recibido==0){
			printf("Se desconecto el Kernel\n");
			exit(1);
		}
		switch (codigo) {
		case (1):																//Imprimir por pantalla
			mensaje= recibir_serializado(socketKernel);
			printf("%s\n", mensaje);
			free(mensaje);
			i=0; 					//TEMPORAL	i=0, puesto para probar que ande el resto
			break;
		//Faltan el resto de los cases, generalmente tienen que ver con obtener algun tipo de valor por teclado



		}

	}

return 0;
}

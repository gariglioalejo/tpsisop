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
char * codigo_beso;
int i;
t_config *consola_config;
char * ip_kernel;
char * puerto_kernel;
int socketKernel;
int i = 1;
int recibido;
int codigo;
char * mensaje;
int size_codigo;
int longcadena;
int32_t valor;
char trash;
int prueba;
float valorIngresado;
char * stringIngresado;
int input;
int inputMax;
int j;
int intIngresado;

int main(int argc, char ** argv) {

	if (argc != 2) {
		printf("No hay argumento\n");
		return 1;
	}
	/*
	 direccion_beso = argv[1];
	 beso = fopen(direccion_beso, "rb");
	 if (beso == NULL ) {
	 printf("No pudo abrirse correctamente el beso\n");
	 return 1;
	 }
	 if (stat(argv[1], &stat_beso)) {
	 printf("No se realizo el stat correctamente\n");
	 return 1;
	 }
	 literal_beso = malloc(stat_beso.st_size);
	 for (i = 0;i<stat_beso.st_size ; i++)
	 literal_beso[i] = getc(beso)
	 ;
	 //literal_beso[i] = '\0';	//Notar que al imprimirlo no muestra un ? raro al final, eso indica que corre bien
	 */
	codigo_beso = copiarArchivo(argv[1]);

	size_codigo = hacerStat(argv[1]).st_size;
	printf("%d\n", size_codigo);

//Para setear variable de entorno:
//export ESO_CONFIG=./consola.cfg
//Para mostrar el contenido:
//echo $ESO_CONFIG

	consola_config = config_create(getenv("ESO_CONFIG"));
	ip_kernel = config_get_string_value(consola_config, "IP_KERNEL");
	puerto_kernel = config_get_string_value(consola_config, "PUERTO_KERNEL");
	socketKernel = conectarse(ip_kernel, puerto_kernel);
	enviar_serializado_beso(1, size_codigo, codigo_beso, socketKernel);	//Cambie el -1 por 1

	while (i) {
		if ((recibido = recv(socketKernel, &codigo, sizeof(int), 0)) == -1) {
			printf("Fallo el recv1\n");
			exit(1);
		}
		if (recibido == 0) {
			printf("Se desconecto el Kernel\n");
			exit(1);
		}
		switch (codigo) {
		case (0):
			i = 0;
			break;

		case (1):
			//Imprimir por pantalla
			mensaje = recibir_serializado(socketKernel);
			printf("%s\n", mensaje);
			free(mensaje);
			break;
		case (2):
			inputMax = recibirInt(socketKernel);
			j = 1;
			while (j) {
				printf("Ingrese un string de un tamanio maximo de: %d\n",
						inputMax);
				stringIngresado = inputString(stdin, 1);
				if (strlen(stringIngresado) <= inputMax) {
					printf("String ingresado correctamente\n");
					printf("%s\n", stringIngresado);
					 enviar_serializado(-1, stringIngresado, socketKernel);
					free(stringIngresado);
					j = 0;
				} else {
					printf("String ingresado incorrectamente\n");
				}
			}
			break;

		case 3:
			valor = 0;
			valorIngresado = 0;
			printf(
					"Ingrese  un interger con valor entre –2.147.483.648 y 2.147.483.647: \n");
			int input = scanf("%f", &valorIngresado);
			while (input != 1 || valorIngresado > 2147483647.0
					|| valorIngresado < -2147483648.0 || (prueba =
							valorIngresado) != valorIngresado) {

				printf("Valor ingresado incorrectamente \n");
				printf(
						"Ingrese un interger con valor entre –2.147.483.648 y 2.147.483.647: \n");

				while (trash != '\n') {
					trash = getchar();
				}
				trash = '.';
				input = scanf("%f", &valorIngresado);
			}

			valor = (int32_t) valorIngresado;
			printf("Valor integer: %d\n", valor);
			getchar();			//asco pero corre :P

			enviarInt(valor, socketKernel);
			break;

		}

	}

	return 0;
}

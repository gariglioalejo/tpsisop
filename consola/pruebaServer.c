#include "funciones.h"

int socketCliente;

int main(int argc, char ** argv) {

	socketCliente = crearServer_y_escuchar("5501");
	char * mensaje= recibir_serializado(socketCliente);

	printf("%s\n",mensaje);

	imprimir_consola("Hola Maquina", socketCliente);


	return 0;
}


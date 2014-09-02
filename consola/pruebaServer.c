#include "funciones.h"

int listenningSocket;
int socketCliente;

int main(int argc, char ** argv) {

	listenningSocket = crearServer("5501");
	socketCliente=aceptarConexion(listenningSocket);
	char * mensaje= recibir_serializado(socketCliente);

	printf("%s\n",mensaje);

	enviar_serializado(1,"Hola Maquina", socketCliente);


	return 0;
}


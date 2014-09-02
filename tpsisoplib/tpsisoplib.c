/*
 * tpsisoplib.c
 *
 *  Created on: 26/08/2014
 *      Author: utnso
 */

#include "tpsisoplib.h"



char * recibir_serializado(int socketCliente) {
	int size;
	int recibido;
	if ((recibido = recv(socketCliente, &size, sizeof(int), 0)) == -1) {
		printf("Fallo el recv1");
		exit(1);
	}
	char * mensaje = malloc(size);
	if ((recibido = recv(socketCliente, mensaje, size, 0)) == -1) {
		printf("Fallo el recv2");
		exit(1);
	}

	return mensaje;

}

char * enviar_serializado(int codigo, char * mensaje, int socket) { //Debe tener '/0' al final el mensaje!
//Serializa un char * y lo manda por el socket indicado
//Mandar con codigo == -1 si solo se quiere mandar el size y el mensaje

	char * mensaje_serializado;
	if (codigo == -1) {
		int enviados;
		int size = strlen(mensaje) + 1;
		mensaje_serializado = malloc(sizeof(size) + size);
		memcpy(mensaje_serializado, &size, sizeof(size));
		memcpy(mensaje_serializado + sizeof(size), mensaje, size);

//printf("%d\n%s\n",*mensaje_serializado,mensaje_serializado+sizeof(size));
		if ((enviados = send(socket, mensaje_serializado, sizeof(size) + size,
				0)) == -1) {
			printf("Fallo el send");
			exit(1);
		}
	}
	if(codigo != -1){
				int enviados;
				int size = strlen(mensaje) + 1;
				mensaje_serializado = malloc(sizeof(codigo)+sizeof(size) + size);
				memcpy(mensaje_serializado,&codigo,sizeof(codigo));
				memcpy(mensaje_serializado+sizeof(codigo), &size, sizeof(size));
				memcpy(mensaje_serializado +sizeof(codigo)+ sizeof(size), mensaje, size);

		//printf("%d\n%s\n",*mensaje_serializado,mensaje_serializado+sizeof(size));
				if ((enviados = send(socket, mensaje_serializado, sizeof(codigo)+sizeof(size) + size,
						0)) == -1) {
					printf("Fallo el send");
					exit(1);
				}
	}
	return mensaje_serializado;
}

int conectarse(char * ip, char * puerto) { //Devuelve el socketServer ya conectado al server

	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	int serverSocket;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;     //IPv4
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(ip, puerto, &hints, &servinfo)) != 0) {
		printf("Fallo el getaddrinfo\n");
		exit(1);
	}

	if ((serverSocket = socket(servinfo->ai_family, servinfo->ai_socktype,
			servinfo->ai_protocol)) == -1) {
		printf("Fallo el socket\n");
		exit(1);
	}

	if (connect(serverSocket, servinfo->ai_addr, servinfo->ai_addrlen)) {
		printf("Fallo el connect\n");
		exit(1);
	}

	freeaddrinfo(servinfo);

	return serverSocket;

}
int crearServer(char * puerto) {

	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	int listenningSocket;
	int yes = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;     //IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, puerto, &hints, &servinfo)) != 0) {
		printf("Fallo el getaddrinfo\n");
		exit(1);
	}
	if ((listenningSocket = socket(servinfo->ai_family, servinfo->ai_socktype,
			servinfo->ai_protocol)) == -1) {
		printf("Fallo el socket\n");
		exit(1);
	}
	if (setsockopt(listenningSocket, SOL_SOCKET, SO_REUSEADDR, &yes,
			sizeof(int)) == -1) {
		printf("Fallo el setsockopt");
		exit(1);
	}

	if (bind(listenningSocket, servinfo->ai_addr, servinfo->ai_addrlen)) {
		printf("Fallo el bind\n");
		exit(1);
	}
	freeaddrinfo(servinfo);

	if (listen(listenningSocket, 15)) {	//Backlog de 15, no creo que sea necesario mas
		printf("Fallo el listen\n");
		exit(1);
	}

	return listenningSocket;
}
int aceptarConexion(int listenningSocket) {
	int socketCliente;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	if ((socketCliente = accept(listenningSocket, (struct sockaddr *) &addr,
			&addrlen)) == -1) {
		printf("Fallo el accept\n");
		exit(1);
	}
	return socketCliente;
}

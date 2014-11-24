/*
 * pruebas.c
 *
 *  Created on: 15/11/2014
 *      Author: utnso
 */

#include "funciones.h"

int main(int argc, char ** argv) {

	t_listaSocketsConsola * nodoConsola = malloc( //AGREGA A LA COLA DE SOCKETS CONSOLA UN NODO CON EL PID ASOCIADO A ESA CONSOLA Y EL SOCKET QUE LE CORRESPONDE
			sizeof(t_listaSocketsConsola));
	nodoConsola->pid = 2;
	nodoConsola->socketConsola =3;

	int i=3;

	t_list * lista =list_create();

	list_add(lista,nodoConsola);

	bool resultado =estaEnlaListaSocketsConsola(lista, i);
	printf("%d\n",resultado);

	return 0;
}

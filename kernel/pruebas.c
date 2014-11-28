/*
 * pruebas.c
 *
 *  Created on: 15/11/2014
 *      Author: utnso
 */

#include "funciones.h"

int main(int argc, char ** argv) {
	inicializar_panel(KERNEL, "/home/utnso/panel");
	t_list * listaMadre = list_create();
	t_tcb * tcb1= malloc(sizeof(t_tcb));
	t_tcb * tcb2 = malloc(sizeof(t_tcb));
	tcb1->pid=1;tcb1->tid=2;tcb2->pid=11;tcb2->tid=12;
	t_list * listaReady = list_create();
	list_add(listaReady,tcb1);list_add(listaReady,tcb2);
	convertirCola(listaMadre,listaReady, READY);
	t_hilo * hilo1 = list_get(listaMadre,1);
	printf("%d\n----%d\n",hilo1->tid,hilo1->cola);
	hilos(listaMadre);
	instruccionProtegida("Hinchar los huevos",tcb1,READY);
	return 0;
}

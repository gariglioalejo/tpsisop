#include "cpu.h"

//Funciones Privilegiadas

int fnMALC(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf(
				"El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);

		tcb->P = tcb->P + 4;
		return -1;
	}

	t_list* argumentos = list_create();
	ejecucion_instruccion("MALC", argumentos);
	list_destroy_and_destroy_elements(argumentos, free);

	//t_devolucion devolucion;

	t_crearSegmento * segmentoCreado = malloc(sizeof(t_crearSegmento));
	int result = 0;
	int size = tcb->registroA.valores;
	int pid = tcb->pid;

	segmentoCreado = crearSegmento(pid, size, socketM);

	if (segmentoCreado->exito) {

		tcb->registroA.valores = segmentoCreado->base;

	} else {
		segmentatioFault++;
		free(segmentoCreado);
		return -1;
	}

	free(segmentoCreado);
	list_destroy(parametros);
	printf("MALC Ejecutada \n");

	tcb->P = tcb->P + 4;
	return result;
}

int fnFREE(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf(
				"El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);
		tcb->P = tcb->P + 4;
		return -1;
	}


	t_list* argumentos = list_create();
	ejecucion_instruccion("FREE", argumentos);
	list_destroy_and_destroy_elements(argumentos, free);

	int result = 0;
	int dir = tcb->registroA.valores;
	int pid = tcb->pid;

	bool exito = destruirSegmento(pid, dir, socketM);

	if (exito) {
		printf("FREE Ejecutada \n");
		tcb->P = tcb->P + 4;
		return result;

	} else {
		segmentatioFault++;
		result = -1;
	}

	return 0;
}

int fnINNN(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf(
				"El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);
		tcb->P = tcb->P + 4;
		return -1;
	}


	t_list* argumentos = list_create();
	ejecucion_instruccion("INNN", argumentos);
	list_destroy_and_destroy_elements(argumentos, free);

	int codigo = 4; //Entrada Standard
	int tipo = 0; //0 es INNN, 1 es INNC

	int32_t valorIngresado = 0;

	enviarInt(codigo, socketK);
	enviarInt(tcb->pid, socketK);
	enviarInt(tipo, socketK);

	valorIngresado = recibirInt32(socketK);

	tcb->registroA.valores = valorIngresado;

	printf("INNN Ejecutada \n");

	tcb->P = tcb->P + 4;

	return 0;
}

int fnINNC(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf(
				"El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);

		tcb->P = tcb->P + 4;
		return -1;
	}

	t_list* argumentos = list_create();
	ejecucion_instruccion("INNC", argumentos);
	list_destroy_and_destroy_elements(argumentos, free);

	int codigo = 4; //Entrada Standard
	int tipo = 1; //0 es INNN, 1 es INNC

	int32_t tamanioCadena = tcb->registroB.valores;
	int32_t posMemoria = tcb->registroA.valores;
	int32_t pid = tcb->pid;

	char * cadenaIngresada = malloc(sizeof(char) * tamanioCadena);
	//char  buff[tamanioCadena];

	enviarInt(codigo, socketK);
	enviarInt(tcb->pid, socketK);
	enviarInt(tipo, socketK);
	enviarInt(tamanioCadena, socketK);

	//nbytes = recv(socketK, buff, sizeof(buff), 0);
	cadenaIngresada = recibir_serializado(socketK);
	printf("cadena: %s\n", cadenaIngresada);

	if (strlen(cadenaIngresada) > 0) {

		if (!(escribirMemoria(pid, posMemoria, strlen(cadenaIngresada) + 1,
				cadenaIngresada, socketM))) {

			segmentatioFault++;
			return 0;

		}

	}

	printf("INNC Ejecutada \n");

	tcb->P = tcb->P + 4;
	return 0;
}

int fnOUTN(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf(
				"El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);
		//

		tcb->P = tcb->P + 4;
		return -1;
	}


	t_list* argumentos = list_create();
	ejecucion_instruccion("OUTN", argumentos);
	list_destroy_and_destroy_elements(argumentos, free);

	char cadena[10];


	int codigo = 5; //Salida Standard

	int32_t valorAimprimir = tcb->registroA.valores;
	//cadena = itoa(valorAimprimir,cadena,10);
	sprintf(cadena, "%d", valorAimprimir);
	printf("valor int: %s\n", cadena);

	enviarInt(codigo, socketK);
	enviarInt(tcb->pid, socketK);
	enviar_serializado(-1, cadena, socketK);
	//enviarInt(valorAimprimir,socketK);

	printf("OUTN Ejecutada \n");

	tcb->P = tcb->P + 4;

	return 0;
}

int fnOUTC(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf(
				"El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);
		//

		tcb->P = tcb->P + 4;
		return -1;
	}


	t_list* argumentos = list_create();
	ejecucion_instruccion("OUTC", argumentos);
	list_destroy_and_destroy_elements(argumentos, free);


	int codigo = 5; //Salida Standard



//	int32_t direccion = tcb->registroA.valores;
	int32_t tamanio = tcb->registroB.valores;

	char * cadena = malloc(tamanio * sizeof(char));

	enviarInt(codigo, socketK); //Aviso de un pedido de entrada estandar
	enviarInt(tcb->pid, socketK);

	cadena = pedirString(socketM, tcb);

	enviar_serializado(-1, cadena, socketK);

	free(cadena);

	printf("OUTC Ejecutada \n");

	tcb->P = tcb->P + 4;

	return 0;
}

int fnCREA(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf(
				"El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);
		//Revisar Logear!

		tcb->P = tcb->P + 4;
		return -1;
	}


	t_list* argumentos = list_create();
	ejecucion_instruccion("CREA", argumentos);
	list_destroy_and_destroy_elements(argumentos, free);


	int codigo = 6;

	t_tcb * nuevotcb = malloc(sizeof(t_tcb));

	copiarTcb(tcb, nuevotcb);

	nuevotcb->P = tcb->registroB.valores;

	nuevotcb->tid = tcb->tidMaximo;
	nuevotcb->km = 0;
	tcb->registroA.valores = nuevotcb->tid;
	printf("se le asigni tid %u", nuevotcb->tid);
//	nuevotcb->pid = tcb->registroA.valores;

	//Duplicar Stack
	int segfant = segmentatioFault;
	int resultadoDuplicar = duplicarStack(tcb, nuevotcb, socketM,&segmentatioFault);

	if (segfant < segmentatioFault) {

		return -1;
	}


	enviarInt(codigo, socketK);
	enviarTcb(nuevotcb, socketK);

	free(nuevotcb);
	printf("CREA Ejecutada \n");

	tcb->P = tcb->P + 4;

	return 0;
}

int fnJOIN(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf(
				"El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);
		//Revisar Logear!

		tcb->P = tcb->P + 4;
		return -1;
	}


	t_list* argumentos = list_create();
	ejecucion_instruccion("JOIN", argumentos);
	list_destroy_and_destroy_elements(argumentos, free);


	int codigo = 7;
	int recurso = tcb->registroA.valores;

	enviarInt(codigo, socketK);
	enviarInt(tcb->tid, socketK);
	enviarInt(recurso, socketK);

	printf("JOIN Ejecutada \n");

	tcb->P = tcb->P + 4;

	return 0;
}

int fnBLOK(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);
		tcb->P = tcb->P + 4;
		return -1;
	}


	t_list* argumentos = list_create();
	ejecucion_instruccion("BLOK", argumentos);
	list_destroy_and_destroy_elements(argumentos, free);


	int codigo = 8; //Bloquear

	int32_t recurso = tcb->registroB.valores;

	enviarInt(codigo, socketK); //Aviso de un pedido de Bloqueo
	enviarInt(recurso, socketK);

	printf("BLOK Ejecutada \n");

	tcb->P = tcb->P + 4;
	return 0;
}

int fnWAKE(t_tcb * tcb) {

	if (tcb->km != 1) {
		printf(
				"El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",
				tcb->pid);
		//Revisar Logear!

		tcb->P = tcb->P + 4;
		return -1;
	}


	t_list* argumentos = list_create();
	ejecucion_instruccion("WAKE", argumentos);//que paja!!!!
	list_destroy_and_destroy_elements(argumentos, free);


	int codigo = 9;

	int32_t recurso = tcb->registroB.valores;

	enviarInt(codigo, socketK);
	enviarInt(recurso, socketK);

	printf("WAKE Ejecutada \n");

	tcb->P = tcb->P + 4;
	return 0;
}

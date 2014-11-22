
#include "cpu.h"


//Funciones Privilegiadas

int fnMALC (t_tcb * tcb){

	if (tcb->km != 1){
		printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);

		tcb->P=tcb->P+4;
		return -1;
	}

	//t_devolucion devolucion;
	t_crearSegmento * segmentoCreado = malloc(sizeof(t_crearSegmento));
	int result = 0;
	int size = tcb->registroA.valores;
	int pid = tcb->pid;

	segmentoCreado = crearSegmento(pid, size, socketM);



	if (segmentoCreado->exito){

		 tcb->registroA.valores = segmentoCreado->base;

	} else {
		segmentatioFault++;
		free(segmentoCreado);
		return -1;
	}

	free(segmentoCreado);
	printf("MALC Ejecutada \n");


	tcb->P=tcb->P+4;
	return result;
}


int fnFREE (t_tcb * tcb){

		if (tcb->km != 1){
			printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);

		tcb->P=tcb->P+4;
			return -1;
		}

	int result = 0;
	int dir = tcb->registroA.valores;
	int pid = tcb->pid;

	bool exito = destruirSegmentoAllocado(pid, dir, socketM);

	if (exito){


		} else {
			segmentatioFault++;
			result = -1;
		}


		printf("FREE Ejecutada \n");
		return result;



		tcb->P=tcb->P+4;
	return 0;
}


int fnINNN (t_tcb * tcb){

	if (tcb->km != 1){
			printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);


		tcb->P=tcb->P+4;
			return -1;
		}

		int codigo = 4; //Entrada Standard
		int tipo = 1; //1 es INNN, 2 es INNC



		int32_t valorIngresado = 0;

		enviarInt(codigo,socketK);
		enviarInt(tipo,socketK);

		valorIngresado = recibirInt32(socketK);

		tcb->registroA.valores = valorIngresado;

		printf("INNN Ejecutada \n");


		tcb->P=tcb->P+4;


	return 0;
}


int fnINNC (t_tcb * tcb){

	if (tcb->km != 1){
		printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);


		tcb->P=tcb->P+4;
		return -1;
	}


		int codigo = 4; //Entrada Standard
		int tipo = 2; //1 es INNN, 2 es INNC
		int nbytes;


		int32_t tamanioCadena = tcb->registroB.valores;
		int32_t posMemoria = tcb->registroA.valores;
		int32_t pid = tcb->pid;

		char * cadenaIngresada = malloc(sizeof(char) * tamanioCadena);
		char buff[tamanioCadena];

		enviarInt(codigo,socketK);
		enviarInt(tipo,socketK);


		nbytes = recv(socketK, buff, sizeof(buff), 0);

		if (nbytes > 0){
			cadenaIngresada = buff;

			if(escribirMemoria(pid,posMemoria,sizeof(cadenaIngresada),cadenaIngresada,socketM)){

				segmentatioFault++;
				return 0;

			}

		}



		printf("INNC Ejecutada \n");


		tcb->P=tcb->P+4;
	return 0;
}


int fnOUTN (t_tcb * tcb){

	if (tcb->km != 1){
		printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);
		//

		tcb->P=tcb->P+4;
		return -1;
	}


		int codigo = 5; //Salida Standard
		int tipo = 1; //1 es OUTN, 2 es OUTC
		int result = 0;

		int32_t valorAimprimir = tcb->registroA.valores;

		enviarInt(codigo,socketK);
		enviarInt(tipo,socketK);
		enviarInt(valorAimprimir,socketK);


		result = recibirInt(socketK); //TODO chequear!!! Esperar resultado


		printf("OUTN Ejecutada \n");


		tcb->P=tcb->P+4;


	return 0;
}


int fnOUTC (t_tcb * tcb){

	if (tcb->km != 1){
		printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);
		//

		tcb->P=tcb->P+4;
		return -1;
	}

		int codigo = 5; //Salida Standard
		int tipo = 2; //1 es OUTN, 2 es OUTC
		int result = 0;

		int32_t direccion = tcb->registroA.valores;
		int32_t tamanio = tcb->registroB.valores;

		char * cadena = malloc(tamanio * sizeof(char));


		enviarInt(codigo,socketK); //Aviso de un pedido de entrada estandar
		enviarInt(tipo,socketK); //Aviso tipo de dato a pedir

		cadena = pedirString(socketM,tcb);

		send(socketK,cadena,sizeof(cadena),0);


		result = recibirInt(socketK); //Esperar resultado

		//TODO Revisar Evaluar resultado

		free(cadena);

		printf("OUTC Ejecutada \n");

		tcb->P=tcb->P+4;


	return 0;
}

int fnCREA (t_tcb * tcb){//TODO REVISAR EL PID!!!!

	if (tcb->km != 1){
		printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);
		//Revisar Logear!

		tcb->P=tcb->P+4;
		return -1;
	}


	int codigo = 6;

	t_tcb * nuevotcb = malloc(sizeof(t_tcb));

	copiarTcb(tcb,nuevotcb);

	nuevotcb->P = tcb->registroB.valores;
	nuevotcb->pid = tcb->registroA.valores;
	nuevotcb->tid = tcb->tid + 1;
	nuevotcb->km = 0;


	//Duplicar Stack
	int segfant = segmentatioFault;
	int resultadoDuplicar = duplicarStack(tcb, nuevotcb, socketM,&segmentatioFault);

	if (segfant < segmentatioFault){

		return -1;
	}


	enviarInt(codigo,socketK);
	enviarTcb(nuevotcb, socketK);

	printf("CREA Ejecutada \n");

	tcb->P=tcb->P+4;


	return 0;
}


int fnJOIN (t_tcb * tcb){

	if (tcb->km != 1){
		printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);
		//Revisar Logear!

		tcb->P=tcb->P+4;
		return -1;
	}


	int codigo = 7;
	int recurso = tcb->registroA.valores;

	enviarInt(codigo,socketK);
	enviarInt(recurso,socketK);





	printf("JOIN Ejecutada \n");

	tcb->P=tcb->P+4;


	return 0;
}


int fnBLOK (t_tcb * tcb){ //TODO SEGF

	if (tcb->km != 1){
		printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);
		//Revisar Logear!

		tcb->P=tcb->P+4;
		return -1;
	}



	int codigo = 8; //Bloquear

	int32_t recurso = tcb->registroB.valores;


	enviarInt(codigo,socketK); //Aviso de un pedido de Bloqueo
	enviarInt(recurso,socketK);

	printf("BLOK Ejecutada \n");


	tcb->P=tcb->P+4;
	return 0;
}

int fnWAKE (t_tcb * tcb){

	if (tcb->km != 1){
		printf("El proceso %d no tiene permisos para ejecutar la funcion privilegiada\n",tcb->pid);
		//Revisar Logear!

		tcb->P=tcb->P+4;
		return -1;
	}


	int codigo = 9;

	int32_t recurso = tcb->registroB.valores;


	enviarInt(codigo,socketK);
	enviarInt(recurso,socketK);

	printf("WAIT Ejecutada \n");


	tcb->P=tcb->P+4;
	return 0;
}

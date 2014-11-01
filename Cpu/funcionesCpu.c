
#include "cpu.h"


//Funciones Privilegiadas

/*MALC: Reserva una cantidad de memoria especificada por el registro A. La direccion de esta se
almacena en el registro A. Crea en la MSP un nuevo segmento del tamaño especificado asociado
al programa en ejecución.*/
int fnMALC (t_tcb * tcb){

	t_crearSegmento * segmentoCreado = malloc(sizeof(t_crearSegmento));
	int result = 0;
	int size = tcb->registroA.valores;
	int pid = tcb->pid;

	segmentoCreado = crearSegmento(pid, size, socketM);


	if (segmentoCreado->exito){

		 tcb->registroA.valores = segmentoCreado->base;

	} else {
		result = -1; //TODO Logear?
	}

	free(segmentoCreado);
	printf("MALC Ejecutada \n");
	return result;
}



/*26. FREE
Libera la memoria apuntada por el registro A. Solo se podrá liberar memoria alocada por la
instrucción de MALC. Destruye en la MSP el segmento indicado en el registro A.*/
int fnFREE (t_tcb * tcb){


	int result = 0;
	int dir = tcb->registroA.valores;
	int pid = tcb->pid;

	bool exito = destruirSegmentoAllocado(pid, dir, socketM);

	if (exito){



		} else {
			result = -1; //TODO Logear?
		}


		printf("FREE Ejecutada \n");
		return result;




	return 0;
}



/*27. INNN
Pide por consola del programa que se ingrese un número, con signo entre –2.147.483.648 y
2.147.483.647. El mismo será almacenado en el registro A. Invoca al servicio correspondiente en
el proceso Kernel.*/
int fnINNN (t_tcb * tcb){

		int codigo = 4; //Entrada Standard
		int tipo = 1; //1 es INNN, 2 es INNC



		int32_t valorIngresado = 0;

		//send(socketK,&codigo,sizeof(int),0); //Aviso de un pedido de entrada estandar
		enviarInt(codigo,socketK);
		//send(socketK,&tipo,sizeof(int),0); //Aviso tipo de dato a pedir
		enviarInt(tipo,socketK);

		valorIngresado = recibirInt32(socketK);

		tcb->registroA.valores = valorIngresado;


	return 0;
}



/*28. INNC
Pide por consola del programa que se ingrese una cadena no más larga de lo indicado por el
registro B. La misma será almacenada en la posición de memoria apuntada por el registro A.
Invoca al servicio correspondiente en el proceso Kernel.*/
int fnINNC (t_tcb * tcb){

			int codigo = 4; //Entrada Standard
			int tipo = 2; //1 es INNN, 2 es INNC
			int nbytes;


			int32_t tamanioCadena = tcb->registroB.valores;
			int32_t posMemoria = tcb->registroA.valores;
			int32_t pid = tcb->pid;

			char * cadenaIngresada = malloc(sizeof(char) * tamanioCadena);
			char buff[tamanioCadena];

			//send(socketK,&codigo,sizeof(int),0); //Aviso de un pedido de entrada estandar
			//send(socketK,&tipo,sizeof(int),0); //Aviso tipo de dato a pedir

			enviarInt(codigo,socketK);
			enviarInt(tipo,socketK);


			nbytes = recv(socketK, buff, sizeof(buff), 0);

			if (nbytes > 0){
				cadenaIngresada = buff;

				if(escribirMemoria(pid,posMemoria,sizeof(cadenaIngresada),cadenaIngresada,socketM)){

					//TODO Ejecucion Erronea?
				}

			}






	return 0;
}



/*29. OUTN
Imprime por consola del programa el número, con signo almacenado en el registro A. Invoca al
servicio correspondiente en el proceso Kernel.*/
int fnOUTN (t_tcb * tcb){


				int codigo = 5; //Salida Standard
				int tipo = 1; //1 es OUTN, 2 es OUTC
				int result = 0;

				int32_t valorAimprimir = tcb->registroA.valores;


				//send(socketK,&codigo,sizeof(int),0); //Aviso de un pedido de entrada estandar
				//send(socketK,&tipo,sizeof(int),0); //Aviso tipo de dato a pedir

				enviarInt(codigo,socketK);
				enviarInt(tipo,socketK);


				enviarInt(valorAimprimir,socketK);


				result = recibirInt(socketK); //Esperar resultado

				//TODO Evaluar resultado



	return 0;
}




/*30. OUTC
Imprime por consola del programa una cadena de tamaño indicado por el registro B que se
encuentra en la direccion apuntada por el registro A. Invoca al servicio correspondiente en el
proceso Kernel.*/
int fnOUTC (t_tcb * tcb){

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

			//TODO Evaluar resultado

			free(cadena);

	return 0;
}


/*31. CREA
Crea un hilo, hijo del TCB que ejecutó la llamada al sistema correspondiente. El nuevo hilo
tendrá su Program Counter apuntado al número almacenado en el registro B. El identificador del
nuevo hilo se almacena en el registro A.
Para lograrlo debe generar un nuevo TCB como copia del TCB actual, asignarle un nuevo TID
correlativo al actual, cargar en el Puntero de Instrucción la rutina donde comenzará a ejecutar el
nuevo hilo (registro B), pasarlo de modo Kernel a modo Usuario, duplicar el segmento de stack
desde la base del stack, hasta el cursor del stack. Asignar la base y cursor de forma acorde (tal
que la diferencia entre cursor y base se mantenga igual)13 y luego invocar al servicio
correspondiente en el proceso Kernel con el TCB recién generado.*/
int fnCREA (t_tcb * tcb){

	return 0;
}



/*32. JOIN
Bloquea el programa que ejecutó la llamada al sistema hasta que el hilo con el identificador
almacenado en el registro A haya finalizado. Invoca al servicio correspondiente en el proceso
Kernel.*/
int fnJOIN (t_tcb * tcb){

	return 0;
}


/*33. BLOK
Bloquea el programa que ejecutó la llamada al sistema hasta que el recurso apuntado por B se
libere.
La evaluación y decisión de si el recurso está libre o no es hecha por la llamada al sistema WAIT
pre-compilada.*/
int fnBLOK (t_tcb * tcb){

	return 0;
}

/*34. WAKE
Desbloquea al primer programa bloqueado por el recurso apuntado por B.
La evaluación y decisión de si el recurso está libre o no es hecha por la llamada al sistema
SIGNAL pre-compilada.*/
int fnWAKE (t_tcb * tcb){

	return 0;
}
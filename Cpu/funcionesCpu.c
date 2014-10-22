
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

	//todo preguntarSiElSegmento fue allocado por MALC... agregar lista de Segmentos en t_tcb
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

	//Invocar servicio en Kernel?
		int32_t valorIngresado = 0;

		printf("Ingrese valor entre –2.147.483.648 y 2.147.483.647: \n");
		int input = scanf("%d", &valorIngresado);

		while (input != 1){
			printf("Valor ingresado incorrectamente \n");
			printf("Ingrese valor entre –2.147.483.648 y 2.147.483.647: \n");
			getchar();
			input = scanf("%d", &valorIngresado);
		}

		if (valorIngresado >= 0){

		tcb->registroA.valores = valorIngresado;
		enviarTcb(tcb, socketK);


		} else { //Valor negativo en registroA

	}


	return 0;
}



/*28. INNC
Pide por consola del programa que se ingrese una cadena no más larga de lo indicado por el
registro B. La misma será almacenada en la posición de memoria apuntada por el registro A.
Invoca al servicio correspondiente en el proceso Kernel.*/
int fnINNC (t_tcb * tcb){

	//Invocar servicio en Kernel?
	char * cadenaIngresada = 0;
	uint32_t longcadena = tcb->registroB.valores;
			printf("Ingrese una cadena no mas larga que %d caracteres: \n", longcadena);
			char * input = scanf("%s", &cadenaIngresada);

			while ((input != 1) || strlen(cadenaIngresada) > longcadena){
				printf("Cadena Invalidad\n");
				printf("Ingrese una cadena no mas larga que %d caracteres: \n", longcadena);
				getchar();
				input = scanf("%s", &cadenaIngresada);
			}

			if ((strlen(cadenaIngresada) > 0) && (strlen(cadenaIngresada) <= longcadena)){

			uint32_t direccion = tcb->registroA.valores;
			int pid = tcb->pid;
			escribirMemoria(pid,direccion,sizeof(cadenaIngresada),cadenaIngresada,socketM);



			} else { //Valor negativo en registroA

		}



	return 0;
}



/*29. OUTN
Imprime por consola del programa el número, con signo almacenado en el registro A. Invoca al
servicio correspondiente en el proceso Kernel.*/
int fnOUTN (t_tcb * tcb){

	return 0;
}




/*30. OUTC
Imprime por consola del programa una cadena de tamaño indicado por el registro B que se
encuentra en la direccion apuntada por el registro A. Invoca al servicio correspondiente en el
proceso Kernel.*/
int fnOUTC (t_tcb * tcb){

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

#include "cpu.h"
#include "../ensalada de funciones/funciones.h"

//Funciones Privilegiadas

/*MALC: Reserva una cantidad de memoria especificada por el registro A. La direccion de esta se
almacena en el registro A. Crea en la MSP un nuevo segmento del tamaño especificado asociado
al programa en ejecución.*/
int fnMALC (t_tcb * cpu){

	int result = 0;
	int size = cpu->A;
	int dir = 0;

	// dir = solicitarMemoriaAMSP(size, cpu) --> Esta funcion solicita memoria a la MSP y devuelve la direccion del segmento

	if (dir > 0){

		cpu->A = dir;
	} else {
		result = -1; //TODO Logear?
	}

	return result;
}



/*26. FREE
Libera la memoria apuntada por el registro A. Solo se podrá liberar memoria alocada por la
instrucción de MALC. Destruye en la MSP el segmento indicado en el registro A.*/
int fnFREE (void){

	return 0;
}



/*27. INNN
Pide por consola del programa que se ingrese un número, con signo entre –2.147.483.648 y
2.147.483.647. El mismo será almacenado en el registro A. Invoca al servicio correspondiente en
el proceso Kernel.*/
int fnINNN (void){

	return 0;
}



/*28. INNC
Pide por consola del programa que se ingrese una cadena no más larga de lo indicado por el
registro B. La misma será almacenada en la posición de memoria apuntada por el registro A.
Invoca al servicio correspondiente en el proceso Kernel.*/
int fnINNC (void){

	return 0;
}



/*29. OUTN
Imprime por consola del programa el número, con signo almacenado en el registro A. Invoca al
servicio correspondiente en el proceso Kernel.*/
int fnOUTN (void){

	return 0;
}




/*30. OUTC
Imprime por consola del programa una cadena de tamaño indicado por el registro B que se
encuentra en la direccion apuntada por el registro A. Invoca al servicio correspondiente en el
proceso Kernel.*/
int fnOUTC (void){

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
int fnCREA (void){

	return 0;
}



/*32. JOIN
Bloquea el programa que ejecutó la llamada al sistema hasta que el hilo con el identificador
almacenado en el registro A haya finalizado. Invoca al servicio correspondiente en el proceso
Kernel.*/
int fnJOIN (void){

	return 0;
}


/*33. BLOK
Bloquea el programa que ejecutó la llamada al sistema hasta que el recurso apuntado por B se
libere.
La evaluación y decisión de si el recurso está libre o no es hecha por la llamada al sistema WAIT
pre-compilada.*/
int fnBLOK (void){

	return 0;
}

/*34. WAKE
Desbloquea al primer programa bloqueado por el recurso apuntado por B.
La evaluación y decisión de si el recurso está libre o no es hecha por la llamada al sistema
SIGNAL pre-compilada.*/
int fnWAKE (void){

	return 0;
}

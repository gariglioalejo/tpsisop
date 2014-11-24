/*
 * prueba2.c
 *
 *  Created on: Oct 18, 2014
 *      Author: utnso
 */
#include "funciones.h"

int listenningSocket;
int socketCliente;

int main(int argc, char ** argv) {


	puts("Ingrese un valor");
	int c;
	int currentSize=1;
	char * string = malloc(currentSize);
	int i=0;
	while((c=getchar())!= '\n'){
		string=realloc(string,currentSize+1);
		string[i++]=(char)c;
	}
	string[i]='\0';
	printf("Input size:%d\n",strlen(string));
	printf("%s\n",string);



	/*
	puts("Ingrese un valor");
	int x = 1;
	while (x) {
		float eso;
		int retorno = scanf("%f", &eso);
		if ((eso == (int) eso) && (retorno == 1)) {
			puts("Numero ingresado correctamente");
			printf("%d\n",(int)eso);
			x = 0;
		} else {
			puts("Numero ingresado incorrectamente, ingrese un nuevo valor");
		}

	}
	*/


return 0;
}




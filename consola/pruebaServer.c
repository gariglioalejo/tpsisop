#include "funciones.h"

int listenningSocket;
int socketCliente;

int main(int argc, char ** argv) {



	char *inputString(FILE* fp, size_t size){

	    char *str;
	    int ch;
	    size_t len = 0;
	    str = realloc(NULL, sizeof(char)*size);
	    if(!str)return str;
	    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
	        str[len++]=ch;
	        if(len==size){
	            str = realloc(str, sizeof(char)*(size+=1));
	            if(!str)return str;
	        }
	    }
	    str[len++]='\0';

	    return realloc(str, sizeof(char)*len);
	}


	    char *m;

	    printf("input string : ");
	    m = inputString(stdin, 1);
	    printf("%s\n", m);

	    free(m);
	    return 0;




	/*
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

	*/
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
*/	/*
	int32_t valor = 0;
		char trash;
		int prueba;

		float valorIngresado = 0;

				printf("Ingrese valor entre –2.147.483.648 y 2.147.483.647: \n");
				int input = scanf("%f", &valorIngresado);

				while (input != 1 || valorIngresado > 2147483647.0 || valorIngresado < -2147483648.0 || (prueba=valorIngresado)!=valorIngresado){

					printf("Valor ingresado incorrectamente \n");
					printf("Ingrese valor entre –2.147.483.648 y 2.147.483.647: \n");

					while (trash != '\n'){
						trash = getchar();
					}
					trash = '.';
					input = scanf("%f", &valorIngresado);
				}



		valor = (int32_t) valorIngresado;
		printf("Valor integer: %d\n", valor);

*/
return 0;
}


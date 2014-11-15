#include "funciones.h"

int listenningSocket;
int socketCliente;

int main(int argc, char ** argv) {



	t_tcb * removerDeLaListaTcb(t_list * lista,int socketDelDesconectado){
		bool esElTcbDelDesconectado(void * tcb){
			t_tcb * tcbAux = tcb;
			return tcbAux->socketCpu == socketDelDesconectado;
		}
		return list_remove_by_condition(lista, esElTcbDelDesconectado);
	}

	t_list * listaTcb = list_create();
	t_tcb * tcb = malloc(sizeof(t_tcb));
	tcb->socketCpu = 5;
	list_add(listaTcb,tcb);
	t_tcb * removido =removerDeLaListaTcb(listaTcb,7);
	t_tcb * jaja =list_get(listaTcb,0);
	printf("%d\n",jaja->socketCpu);
	printf("%d\n",list_size(listaTcb));















	/*

	int * removerDeLaLista(t_list * lista,int socketDelDesconectado){
		bool esElSocketDesconectado(void * socket){
			int * socketAux = socket;
			return *socketAux == socketDelDesconectado;
		}
		return list_remove_by_condition(lista, esElSocketDesconectado);
	}
	t_list * listaDeInts= list_create();
	int socketDesconectado = 4;
	list_add(listaDeInts,&socketDesconectado);
	int * removido=removerDeLaLista(listaDeInts,socketDesconectado);
	printf("%d\n",*removido);
	printf("%d\n",list_size(listaDeInts));

*/




	/*
	typedef struct {
		int pid;
		int hilos;
	} t_listaHilos;


	t_list * listaHilos = list_create();

	t_listaHilos * nodo = malloc(sizeof(t_listaHilos));
	nodo->pid = 11;
	nodo->hilos = 22;
	int pidHilo = 11;
	list_add(listaHilos, nodo);

	bool compararHilos(void * nodoHilos) {
			t_listaHilos * nodoHilosAux = nodoHilos;
			return nodoHilosAux->pid == pidHilo;
		}

	t_listaHilos * nodoDos = list_remove_by_condition(listaHilos,
			compararHilos);

	printf("PID nodoDos:%d\nHilos nodoDos: %d\n",nodoDos->pid,nodoDos->hilos);
	*/
	/*

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

	 */

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
	 *//*
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


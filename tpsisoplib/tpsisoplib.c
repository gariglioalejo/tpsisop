/*
 * tpsisoplib.c
 *
 *  Created on: 26/08/2014
 *      Author: utnso
 */

#include "tpsisoplib.h"

//CONVERTIR UN INT32 A UNA ESTRUCTURA DE DIRECCIONES
//USO t_msp_dir *direccion = convertirIntADir(1772423663);
t_msp_dir * convertirIntADir(uint32_t dir){

		t_msp_dir *msp_dir_aux = malloc(sizeof(t_msp_dir));

		uint32_t aux = dir;

		//printf("%d\n",aux);
		//printf("%d\n",sizeof(aux));

		uint32_t elevando1 = pow(2,8);
		uint32_t aux1 = (aux % elevando1);
		//printf("%d\n",aux1);
		msp_dir_aux->offset = aux1;

		uint32_t elevando2 = pow(2,20);
		uint32_t aux2 = ((aux % elevando2) / elevando1);
		//printf("%d\n",aux2);
		msp_dir_aux->pagina = aux2;

		uint32_t aux3 = (aux / elevando2);
		//printf("%d\n",aux3);
		msp_dir_aux->segmento = aux3;


	return msp_dir_aux;
}

//CONVERTIR UNA ESTRUCTURA DE DIRECCIONES A UN INT
//USO uint32_t intdir = convertirDirAInt(direccion);
uint32_t convertirDirAInt(t_msp_dir * direccion){

	uint32_t segmento = direccion->segmento ;
	uint32_t pagina = direccion->pagina;
	uint32_t offset = direccion->offset;
	uint32_t elevando1 = pow(2,8);
	uint32_t elevando2 = pow(2,20);

	uint32_t aux = offset + (pagina * elevando1) + (segmento * elevando2);

	return aux;

}



/*
 * parcialYaar.h
 *
 *  Created on: Nov 5, 2014
 *      Author: utnso
 */

#ifndef PARCIALYAAR_H_
#define PARCIALYAAR_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>

typedef struct __PIRATA {
	t_list * listaObjetos;
	int cantidadMonedas;
	int nivelEbriedad;
	bool esEspia;
	struct __PIRATA* pirataQueLoInvito;
} t_pirata;

typedef struct {
	int mision;
	char * objetoEspecial; //Sirve para CONVERTIRSE_EN_LEYENDA
	int cantidadMonedasDeterminada; //Sirve para SAQUEO
	int victima;			//Sirve para SAQUEO
	void * instanciaVictima;			//Sirve para SAQUEO
} t_mision;

typedef struct {
	t_mision * mision;
	int capacidad;
	t_list * tripulantes;
} t_barco;

typedef struct {
	int habitantes;
} t_ciudad;

typedef struct {
	t_pirata * pirata;
	int contador;
} t_nodoPirata;

bool tieneElObjeto(t_list * lista_objetos, char * objeto);
bool tieneAlgunoDeLosObjetos(t_list * lista_objetos, char ** objetos,
		int numeroObjetos);
bool estaPasadoDeGrog(void * pirata);
bool seAnimaASaquear(int victima, t_pirata * pirata);
bool esUtil(t_mision * mision, t_pirata * pirata);
bool puedeFormarParte(t_barco * barco, t_pirata * pirata);
bool incorporarTripulante(t_barco * barco, t_pirata * pirata);
t_list * muerteALosInutiles(t_list * lista_piratas, t_mision * mision);
t_barco * cambiarMision(t_mision * mision, t_barco * barco);
bool compararEbriedad(void *lpirata, void *rpirata);
t_pirata * pirataMasEbrio(t_barco * barco);
void * consecuenciasDelAnclaje(void * pirata);
t_barco * anclarEn(t_ciudad * ciudad, t_barco * barco);
bool alguienLoTiene(t_list * lista_piratas, char * objeto);
bool estanTodosPasadosDeGrog(t_list * lista_piratas);
bool esVulnerable(t_barco * barco);
bool sosTemible(t_barco * barco);
t_list * tripulantesPasados(t_barco * barco);
int cantidadTripulantesPasados(t_barco * barco);
int cantidadItemsDistintosEntreLosPasados(t_barco * barco);
bool compararCantidadMonedas(void *lpirata, void *rpirata);
t_pirata * pirataMasRico(t_list * listaPiratas);
t_pirata * pirataMasRicoDeLosPasados(t_barco * barco);
bool tieneSuNodoEnLaLista(t_list * listaDeNodos, t_pirata * pirata);
t_nodoPirata * obtenerNodoDelPirata(t_list * listaDeNodos, t_pirata * pirata);
bool compararInvitaciones(void *lNodoPirata, void *rNodoPirata);
t_pirata * pirataMasInvitador(t_barco * barco);

#endif /* PARCIALYAAR_H_ */

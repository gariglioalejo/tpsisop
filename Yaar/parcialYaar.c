/*
 * parcialYaar.c
 *
 *  Created on: Nov 5, 2014
 *      Author: utnso
 */

#include "parcialYaar.h"


enum {
	BUSQUEDA_DEL_TESORO, CONVERTIRSE_EN_LEYENDA, SAQUEO
};
enum {
	BARCO_PIRATA, CIUDAD_COSTERA
};

bool tieneElObjeto(t_list * lista_objetos, char * objeto) {
	bool compararObjeto(void * objetoDeLista) {
		char * objetoAux = objetoDeLista;
		return (strcmp(objetoAux, objeto) == 0);
	}
	return list_any_satisfy(lista_objetos, compararObjeto);
}

bool tieneAlgunoDeLosObjetos(t_list * lista_objetos, char ** objetos,
		int numeroObjetos) {
	while (numeroObjetos) {
		if (tieneElObjeto(lista_objetos, objetos[numeroObjetos - 1])) {
			return 1;
		}
		numeroObjetos = numeroObjetos - 1;
	}
	return 0;
}

bool estaPasadoDeGrog(void * pirata) {
	t_pirata * pirataAux = pirata;
	if (pirataAux->esEspia == 1) {
		return 0;
	}
	return pirataAux->nivelEbriedad >= 90;
}

bool seAnimaASaquear(int victima, t_pirata * pirata) {
	switch (victima) {
	case BARCO_PIRATA: {
		if (estaPasadoDeGrog(pirata)) {
			return 1;
		}
		return 0;

	}
	case CIUDAD_COSTERA: {
		if (pirata->nivelEbriedad >= 50) {
			if (pirata->esEspia == 1) {
				if (tieneElObjeto(pirata->listaObjetos,
						"permiso de la corona")) {
					return 1;
				}
				return 0;
			}
			return 1;
		}
		return 0;

	}
	}

}

bool esUtil(t_mision * mision, t_pirata * pirata) {
	switch (mision->mision) {
	case BUSQUEDA_DEL_TESORO: {
		char * objeto1 = "brujula";
		char * objeto2 = "mapa";
		char * objeto3 = "botellaDeGrogXD";
		char * objetos[3];
		objetos[0] = objeto1;
		objetos[1] = objeto2;
		objetos[2] = objeto3;
		if (tieneAlgunoDeLosObjetos(pirata->listaObjetos, objetos, 3)
				&& (pirata->cantidadMonedas <= 5)) {
			return 1;
		}
		return 0;
	}
	case CONVERTIRSE_EN_LEYENDA: {
		if ((list_size(pirata->listaObjetos) >= 10)
				&& (tieneElObjeto(pirata->listaObjetos, mision->objetoEspecial))) {
			return 1;
		}
		return 0;
	}
	case SAQUEO: {
		if ((pirata->cantidadMonedas <= mision->cantidadMonedasDeterminada)
				&& seAnimaASaquear(mision->victima, pirata)) {
			return 1;
		}
		return 0;
	}

	}
}

bool puedeFormarParte(t_barco * barco, t_pirata * pirata) {
	if ((list_size(barco->tripulantes) <= barco->capacidad)
			&& esUtil(barco->mision, pirata)) {
		return 1;
	}
	return 0;

}

bool incorporarTripulante(t_barco * barco, t_pirata * pirata) {
	if (puedeFormarParte(barco, pirata)) {
		list_add(barco->tripulantes, pirata);
		return 1;
	}
	return 0;
}

t_list * muerteALosInutiles(t_list * lista_piratas, t_mision * mision) {
	bool esUtilMision(void * pirata) {
		t_pirata * pirataAux = pirata;
		return esUtil(mision, pirataAux);
	}
	return list_filter(lista_piratas, esUtilMision);
}

t_barco * cambiarMision(t_mision * mision, t_barco * barco) {
	barco->mision = mision;
	barco->tripulantes = muerteALosInutiles(barco->tripulantes, mision);
	return barco;
}

bool compararEbriedad(void *lpirata, void *rpirata) {
	t_pirata * lpirataAux = lpirata;
	t_pirata * rpirataAux = rpirata;
	return lpirataAux->nivelEbriedad >= rpirataAux->nivelEbriedad;
}

t_pirata * pirataMasEbrio(t_barco * barco) {
	list_sort(barco->tripulantes, compararEbriedad);
	return list_get(barco->tripulantes, 0);
}

void * consecuenciasDelAnclaje(void * pirata) {
	t_pirata * pirataAux = pirata;
	pirataAux->nivelEbriedad = pirataAux->nivelEbriedad + 5;
	pirataAux->cantidadMonedas = pirataAux->cantidadMonedas - 1;
	return pirataAux;
}

t_barco * anclarEn(t_ciudad * ciudad, t_barco * barco) {
	list_map(barco->tripulantes, consecuenciasDelAnclaje);
	list_sort(barco->tripulantes, compararEbriedad);
	list_remove(barco->tripulantes, 0);
	ciudad->habitantes++;
	return barco;
}

bool alguienLoTiene(t_list * lista_piratas, char * objeto) {
	bool loTiene(void * pirata) {
		t_pirata * pirataAux = pirata;
		return tieneElObjeto(pirataAux->listaObjetos, objeto);
	}
	return list_any_satisfy(lista_piratas, loTiene);
}

bool estanTodosPasadosDeGrog(t_list * lista_piratas) {
	return list_all_satisfy(lista_piratas, estaPasadoDeGrog);
}

bool esVulnerable(t_barco * barco) {
	t_mision * mision = barco->mision;
	switch (mision->victima) {
	case BARCO_PIRATA: {
		t_barco * instanciaVictimaAux = mision->instanciaVictima;
		t_list * listaTripulantesVictima = instanciaVictimaAux->tripulantes;
		t_list * listaTripulantesAtacante = barco->tripulantes;
		int cantidadTripulantesVictima = list_size(listaTripulantesVictima);
		int cantidadTripulantesAtacante = list_size(listaTripulantesAtacante);
		if (cantidadTripulantesVictima <= (cantidadTripulantesAtacante / 2)) {
			return 1;
		} else {
			return 0;
		}

	}
	case CIUDAD_COSTERA: {
		int cantidadHabitantesCiudadCostera =
				((t_ciudad *) mision->instanciaVictima)->habitantes;
		if (list_size(barco->tripulantes)
				>= (cantidadHabitantesCiudadCostera * 0.4)) {
			return 1;
		}
		if (estanTodosPasadosDeGrog(barco->tripulantes)) {
			return 1;
		}
		return 0;
	}
	}
}

bool sosTemible(t_barco * barco) {
	if (list_size(barco->tripulantes) >= (0.9 * barco->capacidad)) {
		switch (barco->mision->mision) {
		case BUSQUEDA_DEL_TESORO: {
			if (alguienLoTiene(barco->tripulantes, "llaveDelCofre")) {
				return 1;
			}
		}

		case CONVERTIRSE_EN_LEYENDA: {
			return 1;
		}
		case SAQUEO: {
			if (esVulnerable(barco)) {
				return 1;
			} else {
				return 0;
			}
		}

		}

	}
	return 0;
}

t_list * tripulantesPasados(t_barco * barco) {
	t_list * listaBorrachos = list_filter(barco->tripulantes, estaPasadoDeGrog);
	return listaBorrachos;
}

int cantidadTripulantesPasados(t_barco * barco) {
	t_list * listaBorrachos = tripulantesPasados(barco);
	return list_size(listaBorrachos);
}

int cantidadItemsDistintosEntreLosPasados(t_barco * barco) {
	t_list * listaBorrachos = list_create();
	listaBorrachos = tripulantesPasados(barco);
	t_list * listaObjetos = list_create();
	int cantidadPiratasBorrachos = list_size(listaBorrachos);
	while (cantidadPiratasBorrachos) {
		t_pirata * pirataBorracho = list_get(listaBorrachos,
				cantidadPiratasBorrachos - 1);
		int itemsBorracho = list_size(pirataBorracho->listaObjetos);
		while (itemsBorracho) {
			if (tieneElObjeto(listaObjetos,
					list_get(pirataBorracho->listaObjetos,
							itemsBorracho - 1))) {
				itemsBorracho--;
			} else {
				list_add(listaObjetos,
						list_get(pirataBorracho->listaObjetos,
								itemsBorracho - 1));
				itemsBorracho--;
			}
		}

		cantidadPiratasBorrachos--;
	}
	return list_size(listaObjetos);
}

bool compararCantidadMonedas(void *lpirata, void *rpirata) {
	t_pirata * lpirataAux = lpirata;
	t_pirata * rpirataAux = rpirata;
	return lpirataAux->cantidadMonedas >= rpirataAux->cantidadMonedas;
}

t_pirata * pirataMasRico(t_list * listaPiratas) {
	list_sort(listaPiratas, compararCantidadMonedas);
	return list_get(listaPiratas, 0);
}

t_pirata * pirataMasRicoDeLosPasados(t_barco * barco) {
	t_list * listaBorrachos = list_create();
	listaBorrachos = tripulantesPasados(barco);
	t_pirata * pirataMasRicoDeLosBorrachos = pirataMasRico(listaBorrachos);
	return pirataMasRicoDeLosBorrachos;
}




bool tieneSuNodoEnLaLista(t_list * listaDeNodos, t_pirata * pirata) {
	bool tieneSuNodo(void * nodoPirata) {
		t_nodoPirata * nodoPirataAux = nodoPirata;
		return nodoPirataAux->pirata == pirata;
	}
	return list_any_satisfy(listaDeNodos, tieneSuNodo);
}

t_nodoPirata * obtenerNodoDelPirata(t_list * listaDeNodos, t_pirata * pirata) {
	bool tieneSuNodo(void * nodoPirata) {
		t_nodoPirata * nodoPirataAux = nodoPirata;
		return nodoPirataAux->pirata == pirata;
	}
	return list_find(listaDeNodos, tieneSuNodo);
}

bool compararInvitaciones(void *lNodoPirata, void *rNodoPirata) {
	t_nodoPirata * lNodoPirataAux = lNodoPirata;
	t_nodoPirata * rNodoPirataAux = rNodoPirata;
	return lNodoPirataAux->contador >= rNodoPirataAux->contador;
}


t_pirata * pirataMasInvitador(t_barco * barco) {

	t_list * listaNodoPirata = list_create();

	int cantidadTripulantes = list_size(barco->tripulantes);
	while (cantidadTripulantes) {
		t_pirata * pirata = list_get(barco->tripulantes,
				cantidadTripulantes - 1);
		if (tieneSuNodoEnLaLista(listaNodoPirata, pirata)) {
			t_nodoPirata * nodoConElPirata = obtenerNodoDelPirata(listaNodoPirata,pirata);
			nodoConElPirata->contador++;
		} else {
			t_nodoPirata * nodoNuevo = malloc(sizeof(t_nodoPirata));
			nodoNuevo->pirata = pirata;
			nodoNuevo->contador = 1;
			list_add(listaNodoPirata,nodoNuevo);
		}
		cantidadTripulantes--;
	}
	list_sort(listaNodoPirata,compararInvitaciones);
	t_nodoPirata * nodoDelMasInvitador = list_get(listaNodoPirata,0);
	return nodoDelMasInvitador->pirata;
}

/*
 * main.c
 *
 *  Created on: Nov 5, 2014
 *      Author: utnso
 */
#include "CUnit/Basic.h"
#include "parcialYaar.h"

enum {
	BUSQUEDA_DEL_TESORO, CONVERTIRSE_EN_LEYENDA, SAQUEO
};
enum {
	BARCO_PIRATA, CIUDAD_COSTERA
};

void testTieneElObjeto() {
	t_list * listaObjetos = list_create();
	list_add(listaObjetos, "mermelada");
	bool resultado = tieneElObjeto(listaObjetos, "mermelada");
	CU_ASSERT_TRUE(resultado);
}

void testTieneAlgunoDeLosObjetos() {
	t_list * listaObjetos = list_create();
	char * objeto1 = "brujula";
	char * objeto2 = "mapa";
	char * objeto3 = "botellaDeGrogXD";
	char * objetos[3];
	objetos[0] = objeto1;
	objetos[1] = objeto2;
	objetos[2] = objeto3;
	list_add(listaObjetos, "brujula");
	bool resultado = tieneAlgunoDeLosObjetos(listaObjetos, objetos, 3);
	CU_ASSERT_TRUE(resultado);
}

void testEsUtilBusquedaDelTesoro() {
	t_mision * mision = malloc(sizeof(t_mision));
	mision->mision = BUSQUEDA_DEL_TESORO;
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->listaObjetos = list_create();
	list_add(pirata->listaObjetos, "brujula");
	pirata->cantidadMonedas = 5;
	bool resultado = esUtil(mision, pirata);
	CU_ASSERT_TRUE(resultado);
}

void testEsUtilConvertirseEnLeyenda() {
	t_mision * mision = malloc(sizeof(t_mision));
	mision->mision = CONVERTIRSE_EN_LEYENDA;
	mision->objetoEspecial = "escafandra";
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->listaObjetos = list_create();
	int i = 1;
	while (i < 11) {
		list_add(pirata->listaObjetos, "objeto");
		i++;
	}
	list_add(pirata->listaObjetos, mision->objetoEspecial);
	bool resultado = esUtil(mision, pirata);
	CU_ASSERT_TRUE(resultado);
}

void testSeAnimaASaquear() {
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->nivelEbriedad = 99;
	bool resultado1 = seAnimaASaquear(BARCO_PIRATA, pirata);
	bool resultado2 = seAnimaASaquear(CIUDAD_COSTERA, pirata);
	CU_ASSERT_TRUE(resultado1);
	CU_ASSERT_TRUE(resultado2);
}

void testEsUtilSaqueo() {
	t_mision * mision = malloc(sizeof(t_mision));
	mision->mision = SAQUEO;
	mision->victima = BARCO_PIRATA;
	mision->cantidadMonedasDeterminada = 20;
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->cantidadMonedas = 15;
	pirata->nivelEbriedad = 99;
	bool resultado1 = esUtil(mision, pirata);
	CU_ASSERT_TRUE(resultado1);
	mision->victima = CIUDAD_COSTERA;
	bool resultado2 = esUtil(mision, pirata);
	CU_ASSERT_TRUE(resultado2);
}

void testPuedeFormarParte() {
	t_mision * mision = malloc(sizeof(t_mision));
	mision->mision = BUSQUEDA_DEL_TESORO;
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->listaObjetos = list_create();
	list_add(pirata->listaObjetos, "brujula");
	pirata->cantidadMonedas = 5;
	t_barco * barco = malloc(sizeof(t_barco));
	barco->tripulantes = list_create();
	barco->capacidad = 20;
	barco->mision = mision;
	int i = 1;
	while (i < 4) {
		list_add(barco->tripulantes, pirata);
		i++;
	}
	bool resultado = puedeFormarParte(barco, pirata);
	CU_ASSERT_TRUE(resultado);
}

void testIncorporarTripulante() {
	t_mision * mision = malloc(sizeof(t_mision));
	mision->mision = BUSQUEDA_DEL_TESORO;
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->listaObjetos = list_create();
	list_add(pirata->listaObjetos, "brujula");
	pirata->cantidadMonedas = 5;
	t_barco * barco = malloc(sizeof(t_barco));
	barco->tripulantes = list_create();
	barco->capacidad = 20;
	barco->mision = mision;
	int i = 1;
	while (i < 4) {
		list_add(barco->tripulantes, pirata);
		i++;
	}
	bool resultado = incorporarTripulante(barco, pirata);
	CU_ASSERT_TRUE(resultado);
	int cantidadTripulantes = list_size(barco->tripulantes);
	CU_ASSERT_PTR_EQUAL(pirata,
			list_get(barco->tripulantes, cantidadTripulantes - 1));
}

void testMuerteALosInutiles() {
	t_mision * mision = malloc(sizeof(t_mision));
	mision->mision = BUSQUEDA_DEL_TESORO;
	t_pirata * pirata1 = malloc(sizeof(t_pirata));
	pirata1->listaObjetos = list_create();
	list_add(pirata1->listaObjetos, "brujula");
	pirata1->cantidadMonedas = 5;
	t_pirata * pirata2 = malloc(sizeof(t_pirata));
	pirata2->listaObjetos = list_create();
	list_add(pirata2->listaObjetos, "matafuegos");
	pirata2->cantidadMonedas = 10;
	bool resultado1 = esUtil(mision, pirata1);
	bool resultado2 = esUtil(mision, pirata2);
	t_list * listaPiratas = list_create();
	list_add(listaPiratas, pirata1);
	list_add(listaPiratas, pirata2);
	t_list * listaUtiles = muerteALosInutiles(listaPiratas, mision);
	printf("%d\n", list_size(listaUtiles));
	CU_ASSERT_TRUE(resultado1);
	CU_ASSERT_FALSE(resultado2);
	CU_ASSERT_EQUAL(list_size(listaUtiles), 1);
	CU_ASSERT_PTR_EQUAL(list_get(listaUtiles, 0), pirata1);
}

void testCambiarMision() {
	t_mision * mision1 = malloc(sizeof(t_mision));
	mision1->mision = SAQUEO;
	t_mision * mision2 = malloc(sizeof(t_mision));
	mision2->mision = BUSQUEDA_DEL_TESORO;
	t_pirata * pirata1 = malloc(sizeof(t_pirata));
	pirata1->listaObjetos = list_create();
	list_add(pirata1->listaObjetos, "brujula");
	pirata1->cantidadMonedas = 5;
	t_pirata * pirata2 = malloc(sizeof(t_pirata));
	pirata2->listaObjetos = list_create();
	list_add(pirata2->listaObjetos, "matafuegos");
	pirata2->cantidadMonedas = 10;
	t_list * listaPiratas = list_create();
	list_add(listaPiratas, pirata1);
	list_add(listaPiratas, pirata2);
	t_barco * barco = malloc(sizeof(t_barco));
	barco->tripulantes = listaPiratas;
	barco->capacidad = 20;
	barco->mision = mision1;
	barco = cambiarMision(mision2, barco);
	CU_ASSERT_EQUAL(barco->mision->mision, BUSQUEDA_DEL_TESORO);
	CU_ASSERT_EQUAL(list_size(barco->tripulantes), 1);
	CU_ASSERT_PTR_EQUAL(list_get(barco->tripulantes, 0), pirata1);

}

void testPirataMasEbrio() {
	t_pirata * pirata1 = malloc(sizeof(t_pirata));
	pirata1->nivelEbriedad = 10;
	t_pirata * pirata2 = malloc(sizeof(t_pirata));
	pirata2->nivelEbriedad = 20;
	t_pirata * pirata3 = malloc(sizeof(t_pirata));
	pirata3->nivelEbriedad = 30;
	t_list * listaPiratas = list_create();
	list_add(listaPiratas, pirata1);
	list_add(listaPiratas, pirata2);
	list_add(listaPiratas, pirata3);
	t_barco * barco = malloc(sizeof(t_barco));
	barco->tripulantes = listaPiratas;
	t_pirata * elMasEbrio = pirataMasEbrio(barco);
	CU_ASSERT_PTR_EQUAL(elMasEbrio, pirata3);
	CU_ASSERT_EQUAL(elMasEbrio->nivelEbriedad, 30);
}

void testAnclarEn() {
	t_pirata * pirata1 = malloc(sizeof(t_pirata));
	pirata1->nivelEbriedad = 10;
	pirata1->cantidadMonedas = 5;
	t_pirata * pirata2 = malloc(sizeof(t_pirata));
	pirata2->nivelEbriedad = 20;
	pirata2->cantidadMonedas = 10;
	t_pirata * pirata3 = malloc(sizeof(t_pirata));
	pirata3->nivelEbriedad = 30;
	pirata3->cantidadMonedas = 15;
	t_list * listaPiratas = list_create();
	list_add(listaPiratas, pirata1);
	list_add(listaPiratas, pirata2);
	list_add(listaPiratas, pirata3);
	t_barco * barco = malloc(sizeof(t_barco));
	barco->tripulantes = listaPiratas;
	t_ciudad * ciudad = malloc(sizeof(t_ciudad));
	ciudad->habitantes = 10;
	barco = anclarEn(ciudad, barco);
	CU_ASSERT_EQUAL(ciudad->habitantes, 11);
	CU_ASSERT_EQUAL(pirata1->cantidadMonedas, 4);
	CU_ASSERT_EQUAL(pirata1->nivelEbriedad, 15);
	CU_ASSERT_EQUAL(pirata2->cantidadMonedas, 9);
	CU_ASSERT_EQUAL(pirata2->nivelEbriedad, 25);
	CU_ASSERT_PTR_NOT_EQUAL(list_get(barco->tripulantes, 0), pirata3);
	CU_ASSERT_EQUAL(list_size(barco->tripulantes), 2);
}

void testSosTemibleBusquedaDelTesoro() {
	t_mision * mision = malloc(sizeof(t_mision));
	mision->mision = BUSQUEDA_DEL_TESORO;
	t_barco * barco = malloc(sizeof(t_barco));
	barco->mision = mision;
	barco->capacidad = 10;
	barco->tripulantes = list_create();
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->listaObjetos = list_create();
	list_add(pirata->listaObjetos, "llaveDelCofre");
	int i = 0;
	while (i <= 8) {
		t_pirata * pirata2 = malloc(sizeof(t_pirata));
		pirata2->listaObjetos = list_create();
		list_add(pirata2->listaObjetos, "antorcha");
		list_add(barco->tripulantes, pirata2);
		i++;
	}
	list_add(barco->tripulantes, pirata);
	bool resultado = sosTemible(barco);
	CU_ASSERT_TRUE(resultado);

}

void testSosTemibleSaqueo() {
	t_mision * mision = malloc(sizeof(t_mision));
	mision->mision = SAQUEO;
	mision->victima = BARCO_PIRATA;
	t_barco * barco = malloc(sizeof(t_barco));
	barco->mision = mision;
	barco->capacidad = 10;
	barco->tripulantes = list_create();
	int i = 0;
	while (i < 10) {
		t_pirata * pirata = malloc(sizeof(t_pirata));
		list_add(barco->tripulantes, pirata);
		i++;
	}
	t_barco * barcoVictima = malloc(sizeof(t_barco));
	barcoVictima->tripulantes = list_create();
	int j = 0;
	while (j < 5) {
		t_pirata * pirataVictima = malloc(sizeof(t_pirata));
		list_add(barcoVictima->tripulantes, pirataVictima);
		j++;
	}
	mision->instanciaVictima = barcoVictima;
	bool resultado = sosTemible(barco);
	CU_ASSERT_TRUE(resultado);
	t_ciudad * ciudad = malloc(sizeof(t_ciudad));
	ciudad->habitantes = 10;
	mision->victima = CIUDAD_COSTERA;
	mision->instanciaVictima = ciudad;
	bool resultado2 = sosTemible(barco);
	CU_ASSERT_TRUE(resultado2);
	ciudad->habitantes = 200;
	list_clean(barco->tripulantes);
	int x = 0;
	while (x < 11) {
		t_pirata * pirataPasadoDeGrog = malloc(sizeof(t_pirata));
		pirataPasadoDeGrog->nivelEbriedad = 100;
		list_add(barco->tripulantes, pirataPasadoDeGrog);
		x++;
	}
	bool resultado3 = sosTemible(barco);
	CU_ASSERT_TRUE(resultado3);
}

void testEspiaDeLaCorona() {
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->esEspia = true;
	bool resultado = estaPasadoDeGrog(pirata);
	CU_ASSERT_FALSE(resultado);
	///////////////////////////////
	pirata->listaObjetos = list_create();
	list_add(pirata->listaObjetos, "permiso de la corona");
	pirata->nivelEbriedad = 60;
	bool resultado2 = seAnimaASaquear(CIUDAD_COSTERA, pirata);
	CU_ASSERT_TRUE(resultado2);
}

void testCantidadTripulantesPasados() {
	t_barco * barco = malloc(sizeof(t_barco));
	barco->tripulantes = list_create();
	int i = 0;
	while (i < 10) {
		t_pirata * pirata = malloc(sizeof(t_pirata));
		pirata->nivelEbriedad = 199;
		list_add(barco->tripulantes, pirata);
		i++;
	}
	t_pirata * pirataSobrio = malloc(sizeof(t_pirata));
	pirataSobrio->nivelEbriedad = 0;
	list_add(barco->tripulantes, pirataSobrio);
	int cantidad = cantidadTripulantesPasados(barco);
	CU_ASSERT_EQUAL(cantidad, 10);
}

void testCantidadItemsDistintintosEntreLosPasados() {
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->nivelEbriedad = 100;
	pirata->listaObjetos = list_create();
	list_add(pirata->listaObjetos, "alfiler");
	t_pirata * pirata2 = malloc(sizeof(t_pirata));
	pirata2->nivelEbriedad = 200;
	pirata2->listaObjetos = list_create();
	list_add(pirata2->listaObjetos, "pantalones");
	t_pirata * pirata3 = malloc(sizeof(t_pirata));
	pirata3->nivelEbriedad = 120;
	pirata3->listaObjetos = list_create();
	list_add(pirata3->listaObjetos, "mortero");
	list_add(pirata3->listaObjetos, "pantalones");
	t_pirata * pirataSobrio = malloc(sizeof(t_pirata));
	pirataSobrio->nivelEbriedad = 50;
	pirataSobrio->listaObjetos = list_create();
	list_add(pirataSobrio->listaObjetos, "telescopio");

	t_barco * barco = malloc(sizeof(t_barco));
	barco->tripulantes = list_create();
	list_add(barco->tripulantes, pirata);
	list_add(barco->tripulantes, pirata2);
	list_add(barco->tripulantes, pirata3);
	list_add(barco->tripulantes, pirataSobrio);

	int cantidad = cantidadItemsDistintosEntreLosPasados(barco);

	CU_ASSERT_EQUAL(cantidad, 3);
}

void testPirataMasRicoDeLosPasados() {
	t_pirata * pirata = malloc(sizeof(t_pirata));
	pirata->nivelEbriedad = 100;
	pirata->cantidadMonedas = 100;
	t_pirata * pirata2 = malloc(sizeof(t_pirata));
	pirata2->nivelEbriedad = 200;
	pirata2->cantidadMonedas = 120;
	t_pirata * pirata3 = malloc(sizeof(t_pirata));
	pirata3->nivelEbriedad = 120;
	pirata3->cantidadMonedas = 160;
	t_pirata * pirataSobrio = malloc(sizeof(t_pirata));
	pirataSobrio->nivelEbriedad = 50;
	pirataSobrio->cantidadMonedas = 200;

	t_barco * barco = malloc(sizeof(t_barco));
	barco->tripulantes = list_create();
	list_add(barco->tripulantes, pirata);
	list_add(barco->tripulantes, pirata2);
	list_add(barco->tripulantes, pirata3);
	list_add(barco->tripulantes, pirataSobrio);

	t_pirata * pirataMasRicoDeLosBorrachos = pirataMasRicoDeLosPasados(barco);

	CU_ASSERT_PTR_EQUAL(pirataMasRicoDeLosBorrachos, pirata3);
}

void testPirataMasInvitador() {
	t_pirata * pirata1 = malloc(sizeof(t_pirata));
	t_pirata * pirata2 = malloc(sizeof(t_pirata));
	t_pirata * pirata3 = malloc(sizeof(t_pirata));
	t_pirata * pirata4 = malloc(sizeof(t_pirata));

	pirata1->pirataQueLoInvito = pirata4;
	pirata2->pirataQueLoInvito = pirata4;
	pirata3->pirataQueLoInvito = pirata2;
	pirata4->pirataQueLoInvito = pirata1;

	t_barco * barco = malloc(sizeof(t_barco));
	barco->tripulantes = list_create();
	list_add(barco->tripulantes, pirata1);
	list_add(barco->tripulantes, pirata2);
	list_add(barco->tripulantes, pirata3);
	list_add(barco->tripulantes, pirata4);

	t_pirata * pirataQueMasInvito = pirataMasInvitador(barco);

	CU_ASSERT_PTR_EQUAL(pirataQueMasInvito,pirata4);

}

int main() {
	CU_initialize_registry();

	CU_pSuite prueba = CU_add_suite("Suite de Prueba", NULL, NULL );
	CU_add_test(prueba, "uno", testTieneElObjeto);
	CU_add_test(prueba, "dos", testTieneAlgunoDeLosObjetos);
	CU_add_test(prueba, "tres", testEsUtilBusquedaDelTesoro);
	CU_add_test(prueba, "cuatro", testEsUtilConvertirseEnLeyenda);
	CU_add_test(prueba, "cinco", testSeAnimaASaquear);
	CU_add_test(prueba, "seis", testEsUtilSaqueo);
	CU_add_test(prueba, "siete", testPuedeFormarParte);
	CU_add_test(prueba, "ocho", testIncorporarTripulante);
	CU_add_test(prueba, "nueve", testMuerteALosInutiles);
	CU_add_test(prueba, "diez", testCambiarMision);
	CU_add_test(prueba, "once", testPirataMasEbrio);
	CU_add_test(prueba, "doce", testAnclarEn);
	CU_add_test(prueba, "trece", testSosTemibleBusquedaDelTesoro);
	CU_add_test(prueba, "catorce", testSosTemibleSaqueo);
	CU_add_test(prueba, "quince", testEspiaDeLaCorona);
	CU_add_test(prueba, "16", testCantidadTripulantesPasados);
	CU_add_test(prueba, "17", testCantidadItemsDistintintosEntreLosPasados);
	CU_add_test(prueba, "18", testPirataMasRicoDeLosPasados);
	CU_add_test(prueba,"19",testPirataMasInvitador);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}


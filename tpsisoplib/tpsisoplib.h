/*
 * tpsisoplib.h
 *
 *  Created on: 26/08/2014
 *      Author: utnso
 */
typedef struct {

	uint32_t segmento;
	uint32_t pagina;
	uint32_t offset;

} t_msp_dir;

t_msp_dir * convertirIntADir(uint32_t dir);
uint32_t convertirDirAInt(t_msp_dir * direccion);

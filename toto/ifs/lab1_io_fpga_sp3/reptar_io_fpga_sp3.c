/**************************************************************************
 * HEIG-VD, Institut REDS
 *
 * File       : reptar_io_fpga_sp3.c
 * Author     : Alexandre Malki
 * Created on : 28.02.2017
 *
 * Description  : Ce programme permet d'effectuer des actions
 *		  entre le FPGA SP3 et le CPU.
 *
 * modified by: Thomas Rieder
 **************************************************************************
 */

#include <common.h>

typedef volatile unsigned short vushort;
typedef volatile unsigned int   vuint;

//Les acces a la FPGA sont sur 16 bits. Il faut utiliser un type short
//Le type volatile indique que cette donnee ne peut pas etre mise dans le cash
#define FPGA_SP3_BASE_ADDR      0x1A000000
#define FPGA_SP3_CST            *(vushort *)(FPGA_SP3_BASE_ADDR + 0x4)
#define FPGA_SP3_SW             *(vushort *)(FPGA_SP3_BASE_ADDR + 0xa)
#define FPGA_SP3_LEDS_RED       *(vushort *)(FPGA_SP3_BASE_ADDR + 0x6)
#define FPGA_SP3_LEDS_GREEN     *(vushort *)(FPGA_SP3_BASE_ADDR + 0x8)

#define MASK_SW_PB0             0x400
#define MASK_SW_PB1             0x800

//adresse pour les acces de SW4 du CPU
#define GPIO_DATAIN_REGISTER_BANK4  *(vuint *) (0x49054038)
#define MASK_SW_4                   0x40000000


/* Declarer les fonctions ici */
vushort read_cst(void);
vushort read_sw_10_5(void);
void add_leds_red(vushort cpt);
void write_leds_green(vushort data);
void write_leds_red(vushort data);


int reptar_io_fpga_sp3(void)
{
	app_startup(NULL);

	printf("Uboot: Reptar io fpga sp3\n\n");
	printf("Constante: %#x\n", read_cst());

	write_leds_green(0);
	write_leds_red(0);

	vushort cpt = 0;
	vushort SW_PB1_IS_PRESS = 0;

	/*Quitte l'application quand le SW4 du CPU est pressé*/
	while (!(GPIO_DATAIN_REGISTER_BANK4 & MASK_SW_4)) {

		//SW_PB0 pressé
		if(FPGA_SP3_SW & MASK_SW_PB0) {

			vushort sw = read_sw_10_5();
			write_leds_green(sw);
		}

		//SW_PB1 pressé
		if(FPGA_SP3_SW & MASK_SW_PB1) {

			if(!SW_PB1_IS_PRESS){

				SW_PB1_IS_PRESS = 1;

				if(cpt < 6) {

					cpt++;
					add_leds_red(cpt);

				} else {

					cpt = 0;
					write_leds_red(0);
				}

			}
		} else {

			SW_PB1_IS_PRESS = 0;
		}
	}

	return (0);
}


/*Definisser les fonctions ici */
vushort read_cst(void){

	return (vushort) FPGA_SP3_CST;
}

vushort read_sw_10_5(void) {

	//récupère la val des switchs
	vushort sw = (vushort) FPGA_SP3_SW;

	//shift pour avoir le switch 5 tout à droite
	sw = sw >> 4;

	return sw;
}

void add_leds_red(vushort cpt) {

	vushort i;
	vushort data = 0x1;
	vushort inc = 0x1;

	for(i = 0; i < cpt; i++){

		data |= inc;
		inc <<= 1;
	}

	write_leds_red(data);

}


void write_leds_red(vushort data) {

	//set les 10 bits de poids fort de la donnée à 0
	data = data & 0x3f;

	//set les 6 bits de poids faible à 0
	FPGA_SP3_LEDS_RED &= 0xffc0;

	//set les 6 bits de poids faible avec la data
	FPGA_SP3_LEDS_RED |= data;
}

void write_leds_green(vushort data) {

	//set les 10 bits de poids fort de la donnée à 0
	data = data & 0x3f;

	//set les 6 bits de poids faible à 0
	FPGA_SP3_LEDS_GREEN &= 0xffc0;

	//set les 6 bits de poids faible avec la data
	FPGA_SP3_LEDS_GREEN |= data;
}

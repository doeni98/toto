/**************************************************************************
 * HEIG-VD, Institut REDS
 *
 * File       : labo2_io_fpga.c (Adaptation du filchier reptar_fpga_std.c)
 * Author     : Alexandre MALKI
 * Created on : 09.03.2017
 *
 * Description  : Programme c pour le labo2 io fpga sp6.
 *                Realisation d'une application avec les io de la fpga
 *
 *************************************************************************
 * Ver  Date        Author     Description
 * 0.4  09.03.2017  AMX        Adaptation pour le labo2 io fpga
 * 
 *
 ************************************************************************** 
 */

#include <common.h>

typedef volatile unsigned short vushort;

/*Les acces a la FPGA sont sur 16 bits. Il faut utiliser un type short */
/*Le type volatile indique que cette donnee ne peut pas etre mise dans le cash*/
#define FPGA_SP6_BASE_ADDR      0x........
#define FPGA_SP6_CST            (...)


  /* Creer les define nécessaires pour votre programme                */



/* Adresse pour les acces de SW4 du CPU */
#define GPIO_DATAIN_REGISTER_BANK4  *(volatile unsigned int *) (0x49054038)
#define MASK_SW_4                                               0x40000000


/* Uniquement declaration des fonctions ici ! */


int lab2_io_fpga(void)
{

      printf("Uboot: Reptar io fpga sp6\n");
   

/*Quitte l'application quand le SW4 du CPU est presse*/
      while (!(GPIO_DATAIN_REGISTER_BANK4 & MASK_SW_4)) {
         
         
      }

      return (0);
}

/* Definition des fonctions ici */

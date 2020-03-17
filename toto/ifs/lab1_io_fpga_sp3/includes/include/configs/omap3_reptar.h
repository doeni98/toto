/*
 * (C) Copyright 2011 Logic Product Development <www.logicpd.com>
 *	Peter Barada <peter.barada@logicpd.com>
 *
 * Configuration settings for the Logic OMAP35x/DM37x SOM LV/Torpedo
 * reference boards.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* override base for compatibility with MLO the device ships with */
#define CONFIG_SYS_TEXT_BASE		0x80008000
#define CONFIG_REPTAR_VERSION		2
#define CONFIG_STANDALONE_LOAD_ADDR	0x81600000


/*Set console as a device */
#define CONFIG_SYS_CONSOLE_IS_IN_ENV	1
/*
 * High Level Configuration Options
 */

#define CONFIG_NR_DRAM_BANKS	2	/* CS1 may or may not be populated */

#include <configs/ti_omap3_common.h>
#define CONFIG_OMAP3_REPTAR		/* working with REDS Reptar boards */
/*
 * Display CPU and Board information
 */

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/* Clock Defines */
#define V_OSCK			26000000	/* Clock output from T2 */
#define V_SCLK			(V_OSCK >> 1)

#define CONFIG_MISC_INIT_R		/* misc_init_r dumps the die id */

#define CONFIG_REVISION_TAG

#define CONFIG_CMDLINE_EDITING			/* cmd line edit/history */
#define CONFIG_ZERO_BOOTDELAY_CHECK		/* check keypress w/no delay */

/*
 * Hardware drivers
 */

#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_OMAP_HSMMC
#define CONFIG_DOS_PARTITION

/* commands to include */
#define CONFIG_CMD_CACHE
#define CONFIG_CMD_EXT2		/* EXT2 Support			*/
#define CONFIG_CMD_FAT		/* FAT support			*/

#define CONFIG_CMD_I2C		/* I2C serial bus support	*/
#define CONFIG_CMD_MMC		/* MMC support			*/

#if 0
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#endif

#define CONFIG_SYS_NO_FLASH

/*
 * I2C
 */
#if 0
#define CONFIG_SYS_I2C
#define CONFIG_SYS_OMAP24_I2C_SPEED	100000
#define CONFIG_SYS_OMAP24_I2C_SLAVE	1
#define CONFIG_SYS_I2C_OMAP34XX
#endif

/*
 * TWL4030
 */



/* Environment information */

#define CONFIG_EXTRA_ENV_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	"bootdir=/boot\0" \
	"bootfile=zImage\0" \
	"usbtty=cdc_acm\0" \
	"console=ttyO2,115200n8\0" \
	"mpurate=auto\0" \
	"optargs=\0" \
	"vram=12M\0" \
	"dvimode=1024x768MR-16@60\0" \
	"defaultdisplay=dvi\0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 rw\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"nandroot=ubi0:rootfs ubi.mtd=4\0" \
	"nandrootfstype=ubifs\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"mpurate=${mpurate} " \
		"vram=${vram} " \
		"omapfb.mode=dvi:${dvimode} " \
		"omapdss.def_disp=${defaultdisplay} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"nandargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"mpurate=${mpurate} " \
		"vram=${vram} " \
		"omapfb.mode=dvi:${dvimode} " \
		"omapdss.def_disp=${defaultdisplay} " \
		"root=${nandroot} " \
		"rootfstype=${nandrootfstype}\0" \
	"loadbootscript=load mmc ${mmcdev} ${loadaddr} boot.scr\0" \
	"bootscript=echo Running boot script from mmc ...; " \
		"source ${loadaddr}\0" \
	"loadbootenv=load mmc ${mmcdev} ${loadaddr} uEnv.txt\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t ${loadaddr} ${filesize}\0" \
	"loaduimage=load mmc ${mmcdev} ${loadaddr} uImage\0" \
	"mmcboot=echo Booting from mmc...; " \
		"run mmcargs; " \
		"bootm ${loadaddr}\0" \
	"loadzimage=load mmc ${mmcdev}:2 ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadfdt=load mmc ${mmcdev}:2 ${fdtaddr} ${bootdir}/${fdtfile}\0" \
	"loadubizimage=ubifsload ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadubifdt=ubifsload ${fdtaddr} ${bootdir}/${fdtfile}\0" \
	"mmcbootfdt=echo Booting with DT from mmc ...; " \
		"run mmcargs; " \
		"bootz ${loadaddr} - ${fdtaddr}\0" \
	"nandboot=echo Booting from nand ...; " \
		"run nandargs; " \
		"if nand read ${loadaddr} linux; then " \
			"bootm ${loadaddr};" \
		"fi;\0" \
	"nanddtsboot=echo Booting from nand with DTS...; " \
		"run nandargs; " \
		"ubi part rootfs; "\
		"ubifsmount ubi0:rootfs; "\
		"run loadubifdt; "\
		"run loadubizimage; "\
		"bootz ${loadaddr} - ${fdtaddr}\0" \

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev}; if mmc rescan; then " \
		"if run loadbootscript; then " \
			"run bootscript; " \
		"fi;" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv};" \
			"run importbootenv;" \
		"fi;" \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...;" \
			"run uenvcmd;" \
		"fi;" \
		"if run loaduimage; then " \
			"run mmcboot;" \
		"fi;" \
		"if run loadzimage; then " \
			"if test -z \"${fdtfile}\"; then " \
				"setenv fdtfile omap3-${boardname}-${expansionname}.dtb;" \
			"fi;" \
			"if run loadfdt; then " \
				"run mmcbootfdt;" \
			"fi;" \
		"fi;" \
	"fi;" \
	"run nandboot; " \
	"if test -z \"${fdtfile}\"; then "\
		"setenv fdtfile omap3-${boardname}-${expansionname}.dtb;" \
	"fi;" \
	"run nanddtsboot; " \

#define CONFIG_AUTO_COMPLETE
/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)


/* memtest works on */
#define CONFIG_SYS_MEMTEST_START	(OMAP34XX_SDRC_CS0)
#define CONFIG_SYS_MEMTEST_END		(OMAP34XX_SDRC_CS0 + \
					0x01F00000) /* 31MB */

/* NAND */
#define CONFIG_CMD_NAND		/* NAND support			*/
#define CONFIG_NAND_OMAP_GPMC
#define CONFIG_SYS_NAND_ADDR		NAND_BASE	/* physical address */
#define CONFIG_SYS_FLASH_BASE		NAND_BASE
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_FLASH_BASE
#define CONFIG_NAND_OMAP_GPMC
#define CONFIG_SYS_MAX_NAND_DEVICE	1		/* Max number of */
							/* NAND devices */
#define CONFIG_SYS_NAND_BUSWIDTH_16BIT
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_NAND_PAGE_COUNT	64
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	0
#define CONFIG_SYS_NAND_ECCPOS		{2, 3, 4, 5, 6, 7, 8, 9,\
						10, 11, 12, 13}
#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	12
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_HAM1_CODE_HW

/* MTD */
#define MTDIDS_DEFAULT			"nand0=nand"
#define MTDPARTS_DEFAULT		"mtdparts=nand:512k@0(x-loader),"\
					"1280k(u-boot),768k(u-boot-env),"\
					"5m(kernel),-(fs)"
#define CONFIG_CMD_MTDPARTS	/* Enable MTD parts commands */
#define CONFIG_MTD_DEVICE	/* needed for mtdparts commands */

/* JFFS2 */
#undef CONFIG_CMD_FS_GENERIC
#define CONFIG_CMD_JFFS2
#define CONFIG_JFFS2_NAND 1
#define CONFIG_JFFS2_DEV		"nand0" /* nand device jffs2 lives on */
#define CONFIG_JFFS2_PART_OFFSET	0x780000 /* start of jffs2 partition */
#define CONFIG_JFFS2_PART_SIZE		0xf880000	/* size of jffs2 */

/* env */
#define CONFIG_ENV_OFFSET		0x240000 /* environment starts here */
#define CONFIG_ENV_SIZE			(128 << 10)	/* 128 KiB */
#define CONFIG_ENV_ADDR			CONFIG_ENV_OFFSET
#define CONFIG_SYS_ENV_SECT_SIZE	(128 << 10)	/* 128 KiB */
#define CONFIG_ENV_IS_IN_NAND		1


#define CONFIG_SYS_INIT_RAM_ADDR	0x4020f800
#define CONFIG_SYS_INIT_RAM_SIZE	0x800

/*
 * SMSC922x Ethernet
 */
#if defined(CONFIG_CMD_NET)

#define CONFIG_SMC911X
#define CONFIG_SMC911X_32_BIT
#define CONFIG_SMC911X_BASE	0x2C000000
#endif /* (CONFIG_CMD_NET) */


#ifndef GPMC_BASE		
#define GPMC_BASE	0x6E000000
#endif

/*----------------------------------------------------------------------------
 * GPMC CS Regs Base Addresses
 *----------------------------------------------------------------------------
 */


#define GPMC_CONFIG_CS1		0x90
#define GPMC_CONFIG_CS2		0xC0
#define GPMC_CONFIG_CS3		0xF0
#define GPMC_CONFIG_CS4		0x120
#define GPMC_CONFIG_CS5		0x150
#define GPMC_CONFIG_CS1_BASE	(GPMC_BASE + GPMC_CONFIG_CS1)
#define GPMC_CONFIG_CS2_BASE	(GPMC_BASE + GPMC_CONFIG_CS2)
#define GPMC_CONFIG_CS3_BASE	(GPMC_BASE + GPMC_CONFIG_CS3)
#define GPMC_CONFIG_CS4_BASE	(GPMC_BASE + GPMC_CONFIG_CS4)
#define GPMC_CONFIG_CS5_BASE	(GPMC_BASE + GPMC_CONFIG_CS5)
/*----------------------------------------------------------------------------
 * SPARTAN6 and SPARTAN3 connected to GPMC
 *----------------------------------------------------------------------------
 */

/* FPGAs mappings */

/* Spartan6 */
#define CONFIG_SP6_BASE          0x18000000
/* SP6 "user" range starts at 16MB offset */
#define CONFIG_SP6_USER_BASE     (CONFIG_SP6_BASE + 0X01000000)
#define CONFIG_SP6_USER_SIZE     0X01000000 /* 16MB user */
#define CONFIG_SP6_SIZE          0X02000000 /* 32MB  */

/* Spartan3 */
#define CONFIG_SP3_BASE          (CONFIG_SP6_BASE + CONFIG_SP6_SIZE)
#define CONFIG_SP3_SIZE          0X02000000 /* 32MB  */

/* Spartan6 and Spartan3 together */
#define CONFIG_FPGAS_BASE        CONFIG_SP6_BASE
#define CONFIG_FPGAS_SIZE        (CONFIG_SP6_SIZE + CONFIG_SP3_SIZE)
#define CONFIG_FPGAS_GPMC_SIZE   GPMC_SIZE_64M /* Encoded CS size in GPMC_CONFIG7 */

#define CFG_FPGA_DDR_BASE        0x20000000
#define CFG_FPGA_DDR_SIZE        GPMC_SIZE_128M

#if defined(CONFIG_CMD_FPGA)
/*----------------------------------------------------------------------------
 * fpga from Xilinx (spartan 6, spartan 3): bitstream configuration
 *----------------------------------------------------------------------------
 */
#define CONFIG_FPGA	1
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_SPARTAN3 /* spartan 6 can be programmed same way as spartan 3: to be tested*/
#define CONFIG_FPGA_COUNT 1  /* or 2 spartan 6 and spartan 3?  */
#define CONFIG_SYS_FPGA_PROG_FEEDBACK 1
#define CONFIG_SYS_FPGA_CHECK_CTRLC
#define CONFIG_SYS_FPGA_WAIT_INIT	500  /* 500 ms (This is default. See if needed to be changed: TODO) */
# define CONFIG_SYS_FPGA_WAIT_CONFIG	0 /* Default is 200. Hw guys set 0 in bitstream. Maybe should let the default 200: TODO*/
//# define CONFIG_SYS_FPGA_WAIT_CONFIG	200 /* Default is 200. Hw guys set 0 in bitstream. Maybe should let the default 200: TODO*/
#define FPGA_DEBUG

#endif /* (CONFIG_CMD_FPGA) */




#endif /* __CONFIG_H */

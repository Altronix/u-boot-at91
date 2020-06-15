/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration file for the SAMA5D27 SOM1 EK Board.
 *
 * Copyright (C) 2017 Microchip Corporation
 *		      Wenyou Yang <wenyou.yang@microchip.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "at91-sama5_common.h"

#undef CONFIG_SYS_AT91_MAIN_CLOCK
#define CONFIG_SYS_AT91_MAIN_CLOCK      24000000 /* from 24 MHz crystal */

/* SDRAM */
#define CONFIG_SYS_SDRAM_BASE		0x20000000
#define CONFIG_SYS_SDRAM_SIZE		0x8000000

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SYS_INIT_SP_ADDR		0x218000
#else
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_SDRAM_BASE + 16 * 1024 - GENERATED_GBL_DATA_SIZE)
#endif

#define CONFIG_SYS_LOAD_ADDR		0x22000000 /* load address */

/* NAND flash */
#ifdef CONFIG_CMD_NAND
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		ATMEL_BASE_CS3
#define CONFIG_PMECC_SECTOR_SIZE        512
/* our ALE is AD21 */
#define CONFIG_SYS_NAND_MASK_ALE	BIT(21)
/* our CLE is AD22 */
#define CONFIG_SYS_NAND_MASK_CLE	BIT(22)
#define CONFIG_SYS_NAND_ONFI_DETECTION
#endif
/* SPI flash */

#undef CONFIG_BOOTCOMMAND
#ifdef CONFIG_DOUBLE_COPY
/*
 * Double copy boot support
 * Note - these settings are eventually over-written by swupdate.
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"nand_load_dts=nand read 0x21000000 0x0 0x80000\0" \
	"nand_load_img=nand read 0x22000000 0x20000 0x600000\0" \
	"boot_img=bootz 0x22000000 - 0x21000000\0" \
	"bootdelay=1\0"
#define CONFIG_BOOTCOMMAND \
	"run nand_load_dts;" \
        "run nand_load_img;" \
        "run boot_img;"
#elif CONFIG_NAND_BOOT
/*
 * Nand BOOT
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"nand_load_dts=nand read 0x21000000 0x180000 0x80000\0" \
	"nand_load_img=nand read 0x22000000 0x200000 0x600000\0" \
	"boot_img=bootz 0x22000000 - 0x21000000\0" \
	"bootdelay=1\0"
#define CONFIG_BOOTCOMMAND \
	"run nand_load_dts;" \
        "run nand_load_img;" \
        "run boot_img;"
#elif CONFIG_QSPI_BOOT
/*
 * QSPI BOOT
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"qspi_probe=sf probe\0" \
	"qspi_load_dts=sf read 0x21000000 0xc0000 0xe0000\0" \
	"qspi_load_img=sf read 0x22000000 0xe0000 0x400000\0" \
	"boot_img=bootz 0x22000000 - 0x21000000\0" \
	"bootdelay=1\0"
#define CONFIG_BOOTCOMMAND \
       	"run qspi_probe;" \
	"run qspi_load_dts;" \
        "run qspi_load_img;" \
        "run boot_img;"
#else
/*
 *
 */
#undef CONFIG_BOOTCOMMAND
#ifdef CONFIG_SD_BOOT
/* u-boot env in sd/mmc card */
#define CONFIG_ENV_OFFSET       0xb0000
#define CONFIG_ENV_SIZE		0x4000
/* bootstrap + u-boot + env in sd card */
#define CONFIG_BOOTCOMMAND	"fatload mmc " CONFIG_ENV_FAT_DEVICE_AND_PART " 0x21000000 at91-sama5d27_som1_m5.dtb; " \
				"fatload mmc " CONFIG_ENV_FAT_DEVICE_AND_PART " 0x22000000 zImage; " \
				"bootz 0x22000000 - 0x21000000"
#endif
#endif

/*
// wtf
#ifdef CONFIG_QSPI_BOOT
#undef CONFIG_BOOTARGS
#define CONFIG_BOOTARGS \
	"console=ttyS0,115200 earlyprintk root=/dev/mmcblk0p2 rw rootwait"
#endif
*/

/* SPL */
#define CONFIG_SPL_TEXT_BASE		0x200000
#define CONFIG_SPL_MAX_SIZE		0x10000
#define CONFIG_SPL_BSS_START_ADDR	0x20000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x80000
#define CONFIG_SYS_SPL_MALLOC_START	0x20080000
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x80000

#define CONFIG_SYS_MONITOR_LEN		(512 << 10)

#ifdef CONFIG_SD_BOOT
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION	1
#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME		"u-boot.img"
#endif

#ifdef CONFIG_QSPI_BOOT
#define CONFIG_SYS_SPI_U_BOOT_OFFS	0x10000
#endif

#endif

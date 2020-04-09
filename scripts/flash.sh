#!/bin/bash

if [ -z "$1" ] 
then 
	# Print USAGE
	echo "USAGE: ./flash.sh /path/to/bootstrap [/path/to/uboot]";
else
	echo "writing qspiflash\n"
	( sam-ba -b sama5d27-som1-m5 -p serial -a qspiflash \
		-c erase::0x40000 \
		-c writeboot:$1 \
		-c verifyboot:$1 
	) || exit 1
	if [ -z "$2" ]
	then
		echo "Skipping u-boot"
	else
		# Program UBOOT
		echo "writing uboot\n"
		( sam-ba -b sama5d27-som1-m5 -p serial -a nandflash \
			-c erase:0x40000:0x80000 \
			-c write:$2:0x40000 \
			-c verify:$2:0x40000
		) || exit 1
	fi

	#Program DEV fuses
	echo "writing bureg0,bscr\n"
	( sam-ba -b sama5d27-som1-m5 -p serial -a bootconfig \
		-c writecfg:bureg0:EXT_MEM_BOOT,UART1_IOSET1,QSPI1_IOSET2,QSPI0_DISABLED,NFC_DISABLED,SPI0_DISABLED,SPI1_DISABLED,SDMMC0_DISABLED,SDMMC1_DISABLED \
		-c writecfg:bscr:bureg0,valid
	) || exit 1
fi

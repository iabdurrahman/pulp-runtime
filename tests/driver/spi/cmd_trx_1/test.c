/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdint.h>

#include <pulp.h>


/* spi padmux config */

#define SPI_CS_PAD          4 /* CSN0 */
#define SPI_SCK_PAD         6
#define SPI_MOSI_IO0_PAD    0
#define SPI_MISO_IO1_PAD    1

#define SPI_CS_PAD_MUX_VALUE        0
#define SPI_SCK_PAD_MUX_VALUE       0
#define SPI_MOSI_IO0_PAD_MUX_VALUE  0
#define SPI_MISO_IO1_PAD_MUX_VALUE  0


#ifndef DUMMY_CYCLE_1
#define DUMMY_CYCLE_1  8
#endif

#ifndef DUMMY_CYCLE_2
#define DUMMY_CYCLE_2  16
#endif

/* cmd don't have to be in shared l2 */
uint8_t cmd = 0x40;

__attribute__ ((section(".l2_data"))) uint8_t tx[20];
__attribute__ ((section(".l2_data"))) uint8_t rx[26];

int main(void)
{
	spim_t        *spim;
	spim_conf_t    spim_conf;


	tx[ 0] = 0x37; /*  55 */
	tx[ 1] = 0x7b; /* 123 */
	tx[ 2] = 0x30; /*  48 */
	tx[ 3] = 0x54; /*  84 */
	tx[ 4] = 0x45; /*  69 */
	tx[ 5] = 0x93; /* 147 */
	tx[ 6] = 0x60; /*  96 */
	tx[ 7] = 0xfb; /* 251 */
	tx[ 8] = 0x6b; /* 107 */
	tx[ 9] = 0x3b; /*  59 */
	tx[10] = 0x78; /* 120 */
	tx[11] = 0x12; /*  18 */
	tx[12] = 0x57; /*  87 */
	tx[13] = 0x3b; /*  59 */
	tx[14] = 0x92; /* 146 */
	tx[15] = 0xc9; /* 201 */
	tx[16] = 0x08; /*   8 */
	tx[17] = 0xda; /* 218 */
	tx[18] = 0x3b; /*  59 */
	tx[19] = 0x6c; /* 108 */

	rx[ 0] = 0xdc; /* 220 */
	rx[ 1] = 0x5b; /*  91 */
	rx[ 2] = 0x0d; /*  13 */
	rx[ 3] = 0xb6; /* 182 */
	rx[ 4] = 0x1a; /*  26 */
	rx[ 5] = 0x58; /*  88 */
	rx[ 6] = 0x00; /*   0 */
	rx[ 7] = 0x52; /*  82 */
	rx[ 8] = 0xf1; /* 241 */
	rx[ 9] = 0x3c; /*  60 */
	rx[10] = 0x57; /*  87 */
	rx[11] = 0x86; /* 134 */
	rx[12] = 0x48; /*  72 */
	rx[13] = 0x91; /* 145 */
	rx[14] = 0xfe; /* 254 */
	rx[15] = 0x4b; /*  75 */
	rx[16] = 0x9f; /* 159 */
	rx[17] = 0x30; /*  48 */
	rx[18] = 0x33; /*  51 */
	rx[19] = 0x01; /*   1 */
	rx[20] = 0x06; /*   6 */
	rx[21] = 0x39; /*  57 */
	rx[22] = 0xc8; /* 200 */
	rx[23] = 0xa1; /* 161 */
	rx[24] = 0xc7; /* 199 */
	rx[25] = 0x7c; /* 124 */

	/* set correct i/o pad function */
	hal_apb_soc_pad_set_function(SPI_CS_PAD, SPI_CS_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_SCK_PAD, SPI_SCK_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_MOSI_IO0_PAD, SPI_MOSI_IO0_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_MISO_IO1_PAD, SPI_MISO_IO1_PAD_MUX_VALUE);

	/* initialize config */
	spim_conf_init(&spim_conf);

	/* set necessary configuration */
	spim_conf.max_baudrate = 100000;
	spim_conf.id = 0;
	spim_conf.cs = 0;

	/* open spim device based on configuration */
	spim = spim_open(&spim_conf);
	if (spim == NULL)
	{
		return -1;
	}

	/* TRX 1 */
	spim_cmd_trx(spim, &cmd, 8, DUMMY_CYCLE_1, &(tx[0]), &(rx[2]), 10);

	/* TRX 2 */
	spim_cmd_trx(spim, &cmd, 8, DUMMY_CYCLE_2, &(tx[10]), &(rx[14]), 10);

	/* let dma transfer finish before close the peripheral */
	pos_delay_busy_ms(1);

	/* close */
	spim_close(spim);

	/* inspect spi_rx_data here */
	__asm__ volatile (
		".global     INTERRUPT_HERE\n" \
		"INTERRUPT_HERE:\n"
		:
		:
		:
	);

	return 0;
}

/* required by crt0.S */
void pe_start(void)
{
}

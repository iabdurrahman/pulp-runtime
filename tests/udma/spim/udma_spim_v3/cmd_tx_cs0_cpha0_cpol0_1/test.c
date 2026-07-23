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

/* store udma_spim_command in L2 */
__attribute__ ((section(".l2_data"))) uint32_t udma_spim_command[32];

/**
	* store tx rx buffer in l2
	* these buffers is the one actually hold data that transferred at spi bus
	*/
__attribute__ ((section(".l2_data"))) uint8_t spi_tx_data[32];

int main(void)
{
	/* initialize spi_tx_data with random value */
	spi_tx_data[ 0] = 0x72; /* 114 */
	spi_tx_data[ 1] = 0x46; /*  70 */
	spi_tx_data[ 2] = 0xa4; /* 164 */
	spi_tx_data[ 3] = 0x5a; /*  90 */
	spi_tx_data[ 4] = 0xd0; /* 208 */
	spi_tx_data[ 5] = 0xef; /* 239 */
	spi_tx_data[ 6] = 0x80; /* 128 */
	spi_tx_data[ 7] = 0x96; /* 150 */
	spi_tx_data[ 8] = 0x69; /* 105 */
	spi_tx_data[ 9] = 0x82; /* 130 */
	spi_tx_data[10] = 0x2d; /*  45 */
	spi_tx_data[11] = 0x7f; /* 127 */
	spi_tx_data[12] = 0x3b; /*  59 */
	spi_tx_data[13] = 0x8d; /* 141 */
	spi_tx_data[14] = 0x95; /* 149 */
	spi_tx_data[15] = 0x3b; /*  59 */
	spi_tx_data[16] = 0x99; /* 153 */
	spi_tx_data[17] = 0xd7; /* 215 */
	spi_tx_data[18] = 0x1a; /*  26 */
	spi_tx_data[19] = 0xe4; /* 228 */
	spi_tx_data[20] = 0x27; /*  39 */
	spi_tx_data[21] = 0xe1; /* 225 */
	spi_tx_data[22] = 0x0a; /*  10 */
	spi_tx_data[23] = 0xdb; /* 219 */
	spi_tx_data[24] = 0x30; /*  48 */
	spi_tx_data[25] = 0x8d; /* 141 */
	spi_tx_data[26] = 0x2b; /*  43 */
	spi_tx_data[27] = 0x30; /*  48 */
	spi_tx_data[28] = 0x82; /* 130 */
	spi_tx_data[29] = 0x24; /*  36 */
	spi_tx_data[30] = 0xf6; /* 246 */
	spi_tx_data[31] = 0xbf; /* 191 */


	/* set correct i/o pad function */
	hal_apb_soc_pad_set_function(SPI_CS_PAD, SPI_CS_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_SCK_PAD, SPI_SCK_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_MOSI_IO0_PAD, SPI_MOSI_IO0_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_MISO_IO1_PAD, SPI_MISO_IO1_PAD_MUX_VALUE);

	/* enable peripheral */
	plp_udma_cg_set(plp_udma_cg_get() | (1 << ARCHI_UDMA_SPIM_ID(0)));

	/* enable event */
	soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_PERIPH_EVT_BASE(ARCHI_UDMA_SPIM_ID(0)) + ARCHI_UDMA_SPIM_RX_EVT);
	soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_PERIPH_EVT_BASE(ARCHI_UDMA_SPIM_ID(0)) + ARCHI_UDMA_SPIM_TX_EVT);
	soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_PERIPH_EVT_BASE(ARCHI_UDMA_SPIM_ID(0)) + ARCHI_UDMA_SPIM_CMD_EVT);
	soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_PERIPH_EVT_BASE(ARCHI_UDMA_SPIM_ID(0)) + ARCHI_UDMA_SPIM_EOT_EVT);


	/* create spi udma_spim_command */

	/* configuration */
	udma_spim_command[0] = SPI_CMD_CFG(SPI_CMD_CFG_CLKDIV(2), SPI_CMD_CFG_CPOL_NEG, SPI_CMD_CFG_CPHA_STD); /* spi configuration */

	/* actual transfer */

	udma_spim_command[1] = SPI_CMD_SOT(SPI_CMD_SOT_CS0); /* argument 0 for csn[0], argument n is for csn[n] */

	/* cmd */
	udma_spim_command[2] = SPI_CMD_SEND_CMD((uint16_t) 0x4b, 8, SPI_CMD_QPI_DIS); /* data = 0x4b, (maximum 16-bit), number of bit = 8, qspi = false */

	udma_spim_command[3] = SPI_CMD_DUMMY(8); /* add dummy 8 bit */

	/* trx */
	udma_spim_command[4] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, spi_tx_data); /* transfer from spi_tx_data address */
	udma_spim_command[5] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 32); /* byte length (tx en = 1) */
	udma_spim_command[6] = SPI_CMD_TX_DATA(32, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, SPI_CMD_MSB_FIRST); /* transfer (start=1) */

	/*udma_spim_command[7] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[7] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) udma_spim_command, 8 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

	/* wait until finish */
	while (plp_udma_busy(UDMA_SPIM_CMD_ADDR(0)))
	{
		/*__asm__ volatile ("wfi\n");*/
	}

	__asm__ volatile (
		".global     INTERRUPT_HERE\n" \
		"INTERRUPT_HERE:\n"
		:
		:
		:
	);


	return 0;
}

void pe_start(void)
{
}

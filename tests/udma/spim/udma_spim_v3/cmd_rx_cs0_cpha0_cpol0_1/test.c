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
__attribute__ ((section(".l2_data"))) uint8_t spi_rx_data[32];

int main(void)
{
	/**
		* initialize spi_rx_data with random value,
		* this random value should be overwritten by spim rx
		*/
	spi_rx_data[ 0] = 0x52; /*  82 */
	spi_rx_data[ 1] = 0x5d; /*  93 */
	spi_rx_data[ 2] = 0xc9; /* 201 */
	spi_rx_data[ 3] = 0xf0; /* 240 */
	spi_rx_data[ 4] = 0xdc; /* 220 */
	spi_rx_data[ 5] = 0xf1; /* 241 */
	spi_rx_data[ 6] = 0xce; /* 206 */
	spi_rx_data[ 7] = 0x04; /*   4 */
	spi_rx_data[ 8] = 0x8e; /* 142 */
	spi_rx_data[ 9] = 0x2e; /*  46 */
	spi_rx_data[10] = 0x30; /*  48 */
	spi_rx_data[11] = 0x49; /*  73 */
	spi_rx_data[12] = 0x70; /* 112 */
	spi_rx_data[13] = 0x8c; /* 140 */
	spi_rx_data[14] = 0xd8; /* 216 */
	spi_rx_data[15] = 0xa9; /* 169 */
	spi_rx_data[16] = 0x52; /*  82 */
	spi_rx_data[17] = 0x3c; /*  60 */
	spi_rx_data[18] = 0xa8; /* 168 */
	spi_rx_data[19] = 0x9a; /* 154 */
	spi_rx_data[20] = 0xca; /* 202 */
	spi_rx_data[21] = 0x40; /*  64 */
	spi_rx_data[22] = 0x2b; /*  43 */
	spi_rx_data[23] = 0x6d; /* 109 */
	spi_rx_data[24] = 0x54; /*  84 */
	spi_rx_data[25] = 0xe2; /* 226 */
	spi_rx_data[26] = 0x21; /*  33 */
	spi_rx_data[27] = 0x16; /*  22 */
	spi_rx_data[28] = 0x3d; /*  61 */
	spi_rx_data[29] = 0x5e; /*  94 */
	spi_rx_data[30] = 0xe7; /* 231 */
	spi_rx_data[31] = 0x27; /*  39 */
	spi_rx_data[32] = 0x35; /*  53 */



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

	/* actual spi transfer */

	udma_spim_command[1] = SPI_CMD_SOT(SPI_CMD_SOT_CS0); /* argument 0 for csn[0], argument n is for csn[n] */

	/* cmd */
	udma_spim_command[2] = SPI_CMD_SEND_CMD((uint16_t) 0x4b, 8, SPI_CMD_QPI_DIS); /* command = 0x4b, (maximum 16-bit), number of bit = 8, qspi = false */

	udma_spim_command[3] = SPI_CMD_DUMMY(8); /* add dummy 8 bit */

	/* trx */
	udma_spim_command[4] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, spi_rx_data); /* transfer from spi_rx_data address */
	udma_spim_command[5] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 32); /* byte length (tx en = 1) */
	udma_spim_command[6] = SPI_CMD_RX_DATA(32, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, SPI_CMD_MSB_FIRST); /* transfer (start=1) */

	/*udma_spim_command[7] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[7] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) (&(udma_spim_command[0])), 8 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

	/* wait until finish */
	while (plp_udma_busy(UDMA_SPIM_CMD_ADDR(0)))
	{
		/*__asm__ volatile ("wfi\n");*/
	}

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

void pe_start(void)
{
}

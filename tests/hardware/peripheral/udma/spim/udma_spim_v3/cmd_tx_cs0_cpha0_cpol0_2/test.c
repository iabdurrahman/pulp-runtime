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
__attribute__ ((section(".l2_data"))) uint8_t spi_tx_data_0[32];
__attribute__ ((section(".l2_data"))) uint8_t spi_tx_data_1[16];

int main(void)
{
	/* initialize spi_tx_data with random value */
	spi_tx_data_0[ 0] = 0x72; /* 114 */
	spi_tx_data_0[ 1] = 0x46; /*  70 */
	spi_tx_data_0[ 2] = 0xa4; /* 164 */
	spi_tx_data_0[ 3] = 0x5a; /*  90 */
	spi_tx_data_0[ 4] = 0xd0; /* 208 */
	spi_tx_data_0[ 5] = 0xef; /* 239 */
	spi_tx_data_0[ 6] = 0x80; /* 128 */
	spi_tx_data_0[ 7] = 0x96; /* 150 */
	spi_tx_data_0[ 8] = 0x69; /* 105 */
	spi_tx_data_0[ 9] = 0x82; /* 130 */
	spi_tx_data_0[10] = 0x2d; /*  45 */
	spi_tx_data_0[11] = 0x7f; /* 127 */
	spi_tx_data_0[12] = 0x3b; /*  59 */
	spi_tx_data_0[13] = 0x8d; /* 141 */
	spi_tx_data_0[14] = 0x95; /* 149 */
	spi_tx_data_0[15] = 0x3b; /*  59 */
	spi_tx_data_0[16] = 0x99; /* 153 */
	spi_tx_data_0[17] = 0xd7; /* 215 */
	spi_tx_data_0[18] = 0x1a; /*  26 */
	spi_tx_data_0[19] = 0xe4; /* 228 */
	spi_tx_data_0[20] = 0x27; /*  39 */
	spi_tx_data_0[21] = 0xe1; /* 225 */
	spi_tx_data_0[22] = 0x0a; /*  10 */
	spi_tx_data_0[23] = 0xdb; /* 219 */
	spi_tx_data_0[24] = 0x30; /*  48 */
	spi_tx_data_0[25] = 0x8d; /* 141 */
	spi_tx_data_0[26] = 0x2b; /*  43 */
	spi_tx_data_0[27] = 0x30; /*  48 */
	spi_tx_data_0[28] = 0x82; /* 130 */
	spi_tx_data_0[29] = 0x24; /*  36 */
	spi_tx_data_0[30] = 0xf6; /* 246 */
	spi_tx_data_0[31] = 0xbf; /* 191 */


	spi_tx_data_1[ 0] = 0x2d; /*  45 */
	spi_tx_data_1[ 1] = 0x3d; /*  61 */
	spi_tx_data_1[ 2] = 0x17; /*  23 */
	spi_tx_data_1[ 3] = 0xa2; /* 162 */
	spi_tx_data_1[ 4] = 0x3f; /*  63 */
	spi_tx_data_1[ 5] = 0xb7; /* 183 */
	spi_tx_data_1[ 6] = 0x48; /*  72 */
	spi_tx_data_1[ 7] = 0x6f; /* 111 */
	spi_tx_data_1[ 8] = 0xdb; /* 219 */
	spi_tx_data_1[ 9] = 0x31; /*  49 */
	spi_tx_data_1[10] = 0xba; /* 186 */
	spi_tx_data_1[11] = 0x5a; /*  90 */
	spi_tx_data_1[12] = 0xc6; /* 198 */
	spi_tx_data_1[13] = 0xa7; /* 167 */
	spi_tx_data_1[14] = 0x82; /* 130 */
	spi_tx_data_1[15] = 0xb9; /* 185 */



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

	udma_spim_command[2] = SPI_CMD_SEND_CMD((uint16_t) 0x4b, 8, SPI_CMD_QPI_DIS); /* command = 0x4b, (maximum 16-bit), number of bit = 8, qspi = false */

	udma_spim_command[3] = SPI_CMD_DUMMY(8); /* add dummy 8 bit */

	udma_spim_command[4] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, spi_tx_data_0); /* transfer from spi_tx_data_0 address */
	udma_spim_command[5] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 32); /* byte length (tx en = 1) */
	udma_spim_command[6] = SPI_CMD_TX_DATA(32, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, SPI_CMD_MSB_FIRST); /* transfer */

	/*udma_spim_command[7] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[7] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) (&(udma_spim_command[0])), 8 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

	/**
		* Compiler barrier: flushes previous write to memory
		* prevent "dead store elimination" or "store coalescing"
		*/
	__asm__ volatile (
		""
		:
		:
		: "memory"
	);

	/* wait until finish */
	while (plp_udma_busy(UDMA_SPIM_CMD_ADDR(0)))
	{
		/*__asm__ volatile ("wfi\n");*/
	}

	__asm__ volatile (
		".global     INTERRUPT_HERE_1\n" \
		"INTERRUPT_HERE_1:\n"
		:
		:
		:
	);



	/* reuse same udma_spim_command buffer, but with different content */

	/* configuration */
	udma_spim_command[ 8] = SPI_CMD_CFG(SPI_CMD_CFG_CLKDIV(4), SPI_CMD_CFG_CPOL_POS, SPI_CMD_CFG_CPHA_STD); /* spi configuration */

	/* actual transfer */
	udma_spim_command[ 9] = SPI_CMD_SOT(SPI_CMD_SOT_CS0); /* argument 0 for csn[0], argument n is for csn[n] */

	udma_spim_command[10] = SPI_CMD_SEND_CMD((uint16_t) 0x4c, 8, SPI_CMD_QPI_DIS); /* data = 0x4c, (maximum 16-bit), number of bit = 8, qspi = false */

	udma_spim_command[11] = SPI_CMD_DUMMY(16); /* add dummy 16 bit */

	udma_spim_command[12] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, spi_tx_data_1); /* transfer from spi_tx_data_1 address */
	udma_spim_command[13] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 16); /* byte length (tx en = 1) */
	udma_spim_command[14] = SPI_CMD_TX_DATA(16, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, SPI_CMD_MSB_FIRST); /* transfer */
	/*udma_spim_command[15] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[15] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) (&(udma_spim_command[8])), 8 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

	/**
		* Compiler barrier: flushes previous write to memory
		* prevent "dead store elimination" or "store coalescing"
		*/
	__asm__ volatile (
		""
		:
		:
		: "memory"
	);

	/* wait until finish */
	while (plp_udma_busy(UDMA_SPIM_CMD_ADDR(0)))
	{
		/*__asm__ volatile ("wfi\n");*/
	}

	__asm__ volatile (
		".global     INTERRUPT_HERE_2\n" \
		"INTERRUPT_HERE_2:\n"
		:
		:
		:
	);


	return 0;
}

void pe_start(void)
{
}

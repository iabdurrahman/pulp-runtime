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
__attribute__ ((section(".l2_data"))) uint32_t udma_spim_command[64];

/**
	* store tx rx buffer in l2
	* these buffers is the one actually hold data that transferred at spi bus
	*/
__attribute__ ((section(".l2_data"))) uint8_t spi_tx_data_0[8];
__attribute__ ((section(".l2_data"))) uint8_t spi_rx_data_0[12];

__attribute__ ((section(".l2_data"))) uint8_t spi_tx_data_1[4];
__attribute__ ((section(".l2_data"))) uint8_t spi_rx_data_1[8];

int main(void)
{
	/**
		* initialize spi_rx_data with random value,
		* this random value should be overwritten by spim rx
		*/
	spi_tx_data_0[ 0] = 0xb1; /* 177 */
	spi_tx_data_0[ 1] = 0xb5; /* 181 */
	spi_tx_data_0[ 2] = 0xb2; /* 178 */
	spi_tx_data_0[ 3] = 0x48; /*  72 */
	spi_tx_data_0[ 4] = 0xbc; /* 188 */
	spi_tx_data_0[ 5] = 0x4a; /*  74 */
	spi_tx_data_0[ 6] = 0xad; /* 173 */
	spi_tx_data_0[ 7] = 0x63; /*  99 */

	spi_rx_data_0[ 0] = 0x3a; /*  58 */
	spi_rx_data_0[ 1] = 0xc5; /* 197 */
	spi_rx_data_0[ 2] = 0xca; /* 202 */
	spi_rx_data_0[ 3] = 0xb7; /* 183 */
	spi_rx_data_0[ 4] = 0xc1; /* 193 */
	spi_rx_data_0[ 5] = 0xed; /* 237 */
	spi_rx_data_0[ 6] = 0xb9; /* 185 */
	spi_rx_data_0[ 7] = 0x6e; /* 110 */
	spi_rx_data_0[ 8] = 0x27; /*  39 */
	spi_rx_data_0[ 9] = 0xa0; /* 160 */
	spi_rx_data_0[10] = 0x1e; /*  30 */
	spi_rx_data_0[11] = 0x64; /* 100 */

	spi_tx_data_1[ 0] = 0x87; /* 135 */
	spi_tx_data_1[ 1] = 0x9e; /* 158 */
	spi_tx_data_1[ 2] = 0x40; /*  64 */
	spi_tx_data_1[ 3] = 0xf0; /* 240 */

	spi_rx_data_1[ 0] = 0xbe; /* 190 */
	spi_rx_data_1[ 1] = 0xee; /* 238 */
	spi_rx_data_1[ 2] = 0x2f; /*  47 */
	spi_rx_data_1[ 3] = 0x27; /*  39 */
	spi_rx_data_1[ 4] = 0xe8; /* 232 */
	spi_rx_data_1[ 5] = 0x10; /*  16 */
	spi_rx_data_1[ 6] = 0x2b; /*  43 */
	spi_rx_data_1[ 7] = 0x2f; /*  47 */



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
	udma_spim_command[4] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_tx_data_0[0])); /* transfer from spi_tx_data address */
	udma_spim_command[5] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 8); /* byte length (tx en = 1) */
	udma_spim_command[6] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_rx_data_0[2])); /* transfer to spi_rx_data address */
	udma_spim_command[7] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 8); /* byte length (tx en = 1) */
	udma_spim_command[8] = SPI_CMD_FUL(8, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_MSB_FIRST);

	/*udma_spim_command[9] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[9] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) (&(udma_spim_command[0])), 10 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

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

	/* inspect spi_rx_data here */
	__asm__ volatile (
		".global     INTERRUPT_HERE_1\n" \
		"INTERRUPT_HERE_1:\n"
		:
		:
		:
	);



	/* reuse same udma_spim_command buffer, but with different content */

	/* configuration */
	udma_spim_command[10] = SPI_CMD_CFG(SPI_CMD_CFG_CLKDIV(4), SPI_CMD_CFG_CPOL_POS, SPI_CMD_CFG_CPHA_STD); /* spi configuration */

	/* actual transfer */
	udma_spim_command[11] = SPI_CMD_SOT(SPI_CMD_SOT_CS0); /* argument 0 for csn[0], argument n is for csn[n] */

	udma_spim_command[12] = SPI_CMD_SEND_CMD((uint16_t) 0x4c, 8, SPI_CMD_QPI_DIS); /* data = 0x4c, (maximum 16-bit), number of bit = 8, qspi = false */

	udma_spim_command[13] = SPI_CMD_DUMMY(16); /* add dummy 16 bit */

	udma_spim_command[14] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_tx_data_1[0])); /* transfer from spi_tx_data address */
	udma_spim_command[15] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 4); /* byte length (tx en = 1) */
	udma_spim_command[16] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_rx_data_1[2])); /* transfer to spi_rx_data address */
	udma_spim_command[17] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 4); /* byte length (tx en = 1) */
	udma_spim_command[18] = SPI_CMD_FUL(4, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_MSB_FIRST);

	/*udma_spim_command[19] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[19] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) (&(udma_spim_command[10])), 10 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

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

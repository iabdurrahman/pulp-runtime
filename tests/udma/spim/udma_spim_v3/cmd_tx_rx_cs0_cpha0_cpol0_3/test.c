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
__attribute__ ((section(".l2_data"))) uint8_t spi_rx_data_1[10];

__attribute__ ((section(".l2_data"))) uint8_t spi_tx_data_2_a[6];
__attribute__ ((section(".l2_data"))) uint8_t spi_tx_data_2_b[4];
__attribute__ ((section(".l2_data"))) uint8_t spi_rx_data_2_b[8];
__attribute__ ((section(".l2_data"))) uint8_t spi_rx_data_2_c[10];

int main(void)
{
	/**
		* initialize spi_rx_data with random value,
		* this random value should be overwritten by spim rx
		*/
	spi_tx_data_0[ 0] = 0x9d; /* 157 */
	spi_tx_data_0[ 1] = 0x34; /*  52 */
	spi_tx_data_0[ 2] = 0x22; /*  34 */
	spi_tx_data_0[ 3] = 0x71; /* 113 */
	spi_tx_data_0[ 4] = 0xa2; /* 162 */
	spi_tx_data_0[ 5] = 0xfd; /* 253 */
	spi_tx_data_0[ 6] = 0x38; /*  56 */
	spi_tx_data_0[ 7] = 0x8f; /* 143 */

	spi_rx_data_0[ 0] = 0xb9; /* 185 */
	spi_rx_data_0[ 1] = 0xd7; /* 215 */
	spi_rx_data_0[ 2] = 0x46; /*  70 */
	spi_rx_data_0[ 3] = 0xd6; /* 214 */
	spi_rx_data_0[ 4] = 0x56; /*  86 */
	spi_rx_data_0[ 5] = 0x58; /*  88 */
	spi_rx_data_0[ 6] = 0xc4; /* 196 */
	spi_rx_data_0[ 7] = 0x4f; /*  79 */
	spi_rx_data_0[ 8] = 0xb0; /* 176 */
	spi_rx_data_0[ 9] = 0xbc; /* 188 */
	spi_rx_data_0[10] = 0xf6; /* 246 */
	spi_rx_data_0[11] = 0x0d; /*  13 */

	spi_tx_data_1[ 0] = 0x36; /*  54 */
	spi_tx_data_1[ 1] = 0xd7; /* 215 */
	spi_tx_data_1[ 2] = 0xdd; /* 221 */
	spi_tx_data_1[ 3] = 0xe4; /* 228 */

	spi_rx_data_1[ 0] = 0x65; /* 101 */
	spi_rx_data_1[ 1] = 0xa6; /* 166 */
	spi_rx_data_1[ 2] = 0x77; /* 119 */
	spi_rx_data_1[ 3] = 0xda; /* 218 */
	spi_rx_data_1[ 4] = 0xb5; /* 181 */
	spi_rx_data_1[ 5] = 0xf7; /* 247 */
	spi_rx_data_1[ 6] = 0x4f; /*  79 */
	spi_rx_data_1[ 7] = 0xf6; /* 246 */
	spi_rx_data_1[ 8] = 0x90; /* 144 */
	spi_rx_data_1[ 9] = 0xf7; /* 247 */

	spi_tx_data_2_a[ 0] = 0xf3; /* 243 */
	spi_tx_data_2_a[ 1] = 0x96; /* 150 */
	spi_tx_data_2_a[ 2] = 0xe5; /* 229 */
	spi_tx_data_2_a[ 3] = 0x84; /* 132 */
	spi_tx_data_2_a[ 4] = 0xe3; /* 227 */
	spi_tx_data_2_a[ 5] = 0xe9; /* 233 */

	spi_tx_data_2_a[ 0] = 0xa6; /* 166 */
	spi_tx_data_2_a[ 1] = 0x6e; /* 110 */
	spi_tx_data_2_a[ 2] = 0x4d; /*  77 */
	spi_tx_data_2_a[ 3] = 0x28; /*  40 */

	spi_rx_data_2_b[ 0] = 0x9e; /* 158 */
	spi_rx_data_2_b[ 1] = 0x31; /*  49 */
	spi_rx_data_2_b[ 2] = 0x24; /*  36 */
	spi_rx_data_2_b[ 3] = 0x39; /*  57 */
	spi_rx_data_2_b[ 4] = 0x87; /* 135 */
	spi_rx_data_2_b[ 5] = 0xb4; /* 180 */
	spi_rx_data_2_b[ 6] = 0x49; /*  73 */
	spi_rx_data_2_b[ 7] = 0x09; /*   9 */

	spi_rx_data_2_c[ 0] = 0xd4; /* 212 */
	spi_rx_data_2_c[ 1] = 0x11; /*  17 */
	spi_rx_data_2_c[ 2] = 0xb6; /* 182 */
	spi_rx_data_2_c[ 3] = 0xb7; /* 183 */
	spi_rx_data_2_c[ 4] = 0x97; /* 151 */
	spi_rx_data_2_c[ 5] = 0xc0; /* 192 */
	spi_rx_data_2_c[ 6] = 0xf1; /* 241 */
	spi_rx_data_2_c[ 7] = 0x1f; /*  31 */
	spi_rx_data_2_c[ 8] = 0x64; /* 100 */
	spi_rx_data_2_c[ 9] = 0x9c; /* 156 */



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

	udma_spim_command[3] = SPI_CMD_DUMMY(16); /* add dummy 8 bit */

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

	udma_spim_command[13] = SPI_CMD_DUMMY(8); /* add dummy 16 bit */

	udma_spim_command[14] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_tx_data_1[0])); /* transfer from spi_tx_data address */
	udma_spim_command[15] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 4); /* byte length (tx en = 1) */
	udma_spim_command[16] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_rx_data_1[2])); /* transfer to spi_rx_data address */
	udma_spim_command[17] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 4); /* byte length (tx en = 1) */
	udma_spim_command[18] = SPI_CMD_FUL(4, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_MSB_FIRST);
	udma_spim_command[19] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_rx_data_1[8])); /* transfer to spi_rx_data address */
	udma_spim_command[20] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 0, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 2); /* byte length (tx en = 1) */
	udma_spim_command[21] = SPI_CMD_RX_DATA(2, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, SPI_CMD_MSB_FIRST); /* transfer (start=1) */

	/*udma_spim_command[22] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[22] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) (&(udma_spim_command[10])), 13 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

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



	/* reuse same udma_spim_command buffer, but with different content */

	/* configuration */
	udma_spim_command[23] = SPI_CMD_CFG(SPI_CMD_CFG_CLKDIV(2), SPI_CMD_CFG_CPOL_POS, SPI_CMD_CFG_CPHA_STD); /* spi configuration */

	/* actual transfer */
	udma_spim_command[24] = SPI_CMD_SOT(SPI_CMD_SOT_CS0); /* argument 0 for csn[0], argument n is for csn[n] */
	udma_spim_command[25] = SPI_CMD_SEND_CMD((uint16_t) 0x4d, 8, SPI_CMD_QPI_DIS); /* data = 0x4c, (maximum 16-bit), number of bit = 8, qspi = false */
	udma_spim_command[26] = SPI_CMD_DUMMY(16); /* add dummy 16 bit */

	udma_spim_command[27] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_tx_data_2_a[0])); /* transfer from spi_tx_data_1 address */
	udma_spim_command[28] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 1, /* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 6); /* byte length (tx en = 1) */
	udma_spim_command[29] = SPI_CMD_TX_DATA(6, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, SPI_CMD_MSB_FIRST); /* transfer (start=1) */
	/*udma_spim_command[30] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[30] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_CS_KEEP_OFFSET); /* don't generate event, keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) (&(udma_spim_command[23])), 8 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

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

	/* setup new buffer (spi_[tr]x_data_2_b) */
	udma_spim_command[31] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 1,/* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_tx_data_2_b[0])); /* transfer from spi_tx_data_2 address */
	udma_spim_command[32] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 1,/* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 4); /* byte length (tx en = 1) */
	udma_spim_command[33] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 0,/* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_rx_data_2_b[2])); /* transfer from spi_tx_data_2 address */
	udma_spim_command[34] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 0,/* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 4); /* byte length (tx en = 1) */
	udma_spim_command[35] = SPI_CMD_FUL(4, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_MSB_FIRST); /* transfer (start=1) */
	/*udma_spim_command[36] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[36] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_CS_KEEP_OFFSET); /* don't generate event, keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) (&(udma_spim_command[31])), 6 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

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

	/* setup new buffer (spi_rx_data_2_c) */
	udma_spim_command[37] = SPI_CMD_SETUP_UCA(/* TX: 1; RX: 0 */ 0,/* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, &(spi_rx_data_2_c[0])); /* transfer from spi_tx_data_2 address */
	udma_spim_command[38] = SPI_CMD_SETUP_UCS(/* TX: 1; RX: 0 */ 0,/* dma increment: 0:8-bit, 1:16-bit, 2:32-bit */ 0, 6); /* byte length (tx en = 1) */
	udma_spim_command[39] = SPI_CMD_RX_DATA(6, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, SPI_CMD_MSB_FIRST); /* transfer (start=1) */
	/*udma_spim_command[40] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	udma_spim_command[40] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	/* set register address to point to udma_spim_command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) (&(udma_spim_command[37])), 4 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

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
		".global     INTERRUPT_HERE_3\n" \
		"INTERRUPT_HERE_3:\n"
		:
		:
		:
	);


	return 0;
}

void pe_start(void)
{
}

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

/* store command in L2 */
__attribute__ ((section(".l2_data"))) uint32_t command[32];

int main(void)
{
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


	/* create spi command */
	command[0] = SPI_CMD_CFG(SPI_CMD_CFG_CLKDIV(2),
		SPI_CMD_CFG_CPOL_NEG,
		SPI_CMD_CFG_CPHA_STD); /* spi configuration */
	command[1] = SPI_CMD_SOT(SPI_CMD_SOT_CS0); /* argument 0 for csn[0], argument n is for csn[n] */
	command[2] = SPI_CMD_SEND_CMD((uint16_t) 0xaa55, 16, SPI_CMD_QPI_DIS); /* data = 0xaa, (maximum 16-bit), number of bit = 16, qspi = false */
	command[3] = SPI_CMD_DUMMY(16); /* add dummy 16 bit */
	/*command[4] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_ENA, SPI_CMD_EOT_GEN_EVT_OFFSET);*/ /* generate event, don't keep cs */
	command[4] = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	/* set register address to point to command address */
	pulp_write32(UDMA_SPIM_CMD_ADDR(0), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(0), (uintptr_t) command, 5 * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);
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

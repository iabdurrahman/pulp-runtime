/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <pulp.h> 

#if ((ARCHI_UDMA_NB_SPIM) > 0)
static L2_DATA spim_t __spim[ARCHI_UDMA_NB_SPIM];
#else

static L2_DATA spim_t __spim[1];
#endif

typedef struct {
	unsigned int cmd[4];
} spim_cmd_t;

static inline int __spim_id(int periph_id)
{
	return periph_id - ARCHI_UDMA_SPIM_ID(0); // 
}

int spi_get_div(int spi_freq)
{
	int periph_freq = pos_freq_domains[PI_FREQ_DOMAIN_PERIPH];
	if (spi_freq >= periph_freq)
	{
		return 0;
	}
	else
	{
		// Round-up the divider to obtain an SPI frequency which is below the maximum
		int div = (periph_freq + spi_freq - 1)/ spi_freq;

		// The SPIM always divide by 2 once we activate the divider, thus increase by 1
		// in case it is even to not go avove the max frequency.
		if (div & 1) div += 1;
		div >>= 1;

		return div;
	}
}

static inline int spim_get_byte_align(int wordsize, int big_endian)
{
	return wordsize == SPIM_WORDSIZE_32 && big_endian;
}

spim_t *spim_open(spim_conf_t *conf)
{

	if (conf == NULL){
		return NULL; 
	}

	if (conf->id < 0 || conf->id >= ARCHI_UDMA_NB_SPIM) { 
		return NULL;
	}

	int periph_id = ARCHI_UDMA_SPIM_ID(conf->id);

	spim_t *spim = &__spim[__spim_id(periph_id)];

	spim->channel = periph_id;

	spim->id = conf->id;

	spim->wordsize = conf->wordsize;
	spim->bitOrder = conf->bitOrder;
	spim->big_endian = conf->big_endian;
	spim->polarity = conf->polarity;
	spim->phase = conf->phase;
	spim->max_baudrate = conf->max_baudrate;
	spim->cs = conf->cs;
	spim->byte_align = spim_get_byte_align(conf->wordsize, conf->big_endian);

	int div = spi_get_div(spim->max_baudrate);
	spim->div = div;

	spim->cfg = SPI_CMD_CFG(div, conf->polarity, conf->phase);

	int id = periph_id - ARCHI_UDMA_SPIM_ID(0);

	plp_udma_cg_set(plp_udma_cg_get() | (1<<spim->channel));

	/* enable event */
	soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_PERIPH_EVT_BASE(spim->channel) + ARCHI_UDMA_SPIM_RX_EVT);
	soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_PERIPH_EVT_BASE(spim->channel) + ARCHI_UDMA_SPIM_TX_EVT);
	soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_PERIPH_EVT_BASE(spim->channel) + ARCHI_UDMA_SPIM_CMD_EVT);
	soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_PERIPH_EVT_BASE(spim->channel) + ARCHI_UDMA_SPIM_EOT_EVT);

	return spim;
}

void spim_conf_init(spim_conf_t *conf)
{
	conf->wordsize = SPIM_WORDSIZE_8;
	conf->big_endian = 0;
	conf->max_baudrate = 10000000;
	conf->cs_gpio = -1;
	conf->cs = -1;
	conf->id = -1;
	conf->polarity = 0;
	conf->phase = 0;
	conf->bitOrder = SPI_CMD_MSB_FIRST;
}

void spim_transfer(spim_t *spim, void *tx_data, void *rx_data, size_t len, spim_cs_e mode)
{
	static L2_DATA spim_cmd_t cmd_l2;
	spim_cmd_t *cmd = &cmd_l2;

	cmd->cmd[0] = spim->cfg;
	cmd->cmd[1] = SPI_CMD_SOT(spim->cs);
	cmd->cmd[2] = SPI_CMD_FUL(len/8, SPI_CMD_1_WORD_PER_TRANSF, 8, spim->bitOrder);
	cmd->cmd[3] = SPI_CMD_EOT(1, mode == SPIM_CS_KEEP);

	int buffer_size = len/8;
	int cfg = UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN;

	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(spim->id), (int)cmd, 4*4, cfg);
	plp_udma_enqueue(UDMA_SPIM_TX_ADDR(spim->id), (int)tx_data, buffer_size, cfg);
	plp_udma_enqueue(UDMA_SPIM_RX_ADDR(spim->id), (int)rx_data, buffer_size, cfg);

	while(plp_udma_busy(UDMA_SPIM_TX_ADDR(spim->id)));
	while(plp_udma_busy(UDMA_SPIM_RX_ADDR(spim->id)));
	while(plp_udma_busy(UDMA_SPIM_CMD_ADDR(spim->id)));
}

void spim_transfer16(spim_t *spim, void *tx_data, void *rx_data, size_t len, spim_cs_e mode)
{
	static L2_DATA spim_cmd_t cmd_l2;
	spim_cmd_t *cmd = &cmd_l2;

	cmd->cmd[0] = spim->cfg;
	cmd->cmd[1] = SPI_CMD_SOT(spim->cs);
	cmd->cmd[2] = SPI_CMD_FUL(len/16, SPI_CMD_1_WORD_PER_TRANSF, 16, spim->bitOrder);
	cmd->cmd[3] = SPI_CMD_EOT(1, mode == SPIM_CS_KEEP);

	int buffer_size = len/16;
	int cfg = UDMA_CHANNEL_CFG_SIZE_16 | UDMA_CHANNEL_CFG_EN;

	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(spim->id), (int)cmd, 4*4, cfg);
	plp_udma_enqueue(UDMA_SPIM_TX_ADDR(spim->id), (int)tx_data, buffer_size, cfg);
	plp_udma_enqueue(UDMA_SPIM_RX_ADDR(spim->id), (int)rx_data, buffer_size, cfg);

	while(plp_udma_busy(UDMA_SPIM_TX_ADDR(spim->id)));
	while(plp_udma_busy(UDMA_SPIM_RX_ADDR(spim->id)));
	while(plp_udma_busy(UDMA_SPIM_CMD_ADDR(spim->id)));
}

void spim_close(spim_t *spim)
{
	if (spim != 0)
	{
		plp_udma_cg_set(plp_udma_cg_get() & ~(1<<(spim->channel)));
	}
}












int spim_cmd_trx(spim_t * __restrict handle,
	const uint8_t * __restrict cmd_buf, size_t cmd_bit_len, /* little-endian */
	uint8_t dummy_cycle,
	const uint8_t * __restrict tx_buf, uint8_t * __restrict rx_buf, size_t trx_buf_len)
{
	bool has_cmd_to_transfer   = (((cmd_buf == NULL) || (cmd_bit_len <= 0)) ? false : true);
	bool has_dummy_to_transfer = ((dummy_cycle <= 0) ? false : true);
	bool has_trx_to_transfer   = ((((tx_buf == NULL) && (rx_buf == NULL)) || (trx_buf_len <= 0)) ? false : true);

	size_t len = 0; /* length of spim command, in dword (uint32_t) unit */

	size_t cmd_word_len = 0; /* number of generated SPI_CMD_SEND_CMD instruction */

	size_t i;

	uint32_t *udma_spim_cmd;
	uint32_t *p;

	/**
		* calculate spim cmd to allocate
		* cmd can be transferred per maximum 16-bit
		*/
	if (has_cmd_to_transfer)
	{
		cmd_word_len = cmd_bit_len / 16;
		cmd_bit_len %= 16;

		len +=  cmd_word_len;
		len += (cmd_bit_len > 0) ? 1 : 0;
	}

	/* check if we need transfer dummy */
	len += (has_dummy_to_transfer ? 1 : 0);


	/* need 3 command for trx */
	if (has_trx_to_transfer)
	{
		/* calculate command */
		if (tx_buf != NULL)
		{
			len += 2;
		}

		if (rx_buf != NULL)
		{
			len += 2;
		}

		len++;
	}

	/* no transaction */
	if (len <= 0)
	{
		return 0;
	}

	/* add prologue and epilogue */
	len += 3;

	/**
		* FIXME: sanitize len to prevent user/caller to abuse memory allocation
		*/

	/* allocate (hopefully shared l2) buffer to create command */
	udma_spim_cmd = (uint32_t *) pi_l2_malloc(len * sizeof(uint32_t));
	if (udma_spim_cmd == NULL)
	{
		return (-1); /* should return ENOMEM */
	}

	/* prologue: assert cs */
	p = udma_spim_cmd;
	/**p++ = SPI_CMD_CFG(spi_get_div(handle->max_baudrate),
		handle->polarity, handle->phase);*/
	*p++ = handle->cfg; /* cfg is prebuilt SPI_CMD_CFG command from init
		* for now we cannot provide a (simple) way to set SPI_CMD_CFG
		* for every spim transaction */
	*p++ = SPI_CMD_SOT(handle->cs);

	/* send cmd */
	if (has_cmd_to_transfer)
	{
		for (i = 0; i < cmd_word_len; ++i)
		{
			uint16_t cmd_word;
			cmd_word   = (uint16_t) *cmd_buf++;
			cmd_word <<= 8;
			cmd_word  |= (uint16_t) *cmd_buf++;

			/* 16-bit */
			*p++ = SPI_CMD_SEND_CMD(cmd_word, 16, SPI_CMD_QPI_DIS);
		}

		if (cmd_bit_len > 0)
		{
			uint16_t cmd_word;
			cmd_word   = (uint16_t) *cmd_buf++;
			if (cmd_bit_len > 8)
			{
				uint8_t mask = (1 << (cmd_bit_len - 8)) - 1;
				cmd_word <<= (cmd_bit_len - 8);
				cmd_word  |= (uint16_t) ((*cmd_buf/*++*/) & mask);
			}

			/* maximum bit */
			*p++ = SPI_CMD_SEND_CMD(cmd_word, cmd_bit_len, SPI_CMD_QPI_DIS);
		}
	}


	/* send dummy */
	if (has_dummy_to_transfer)
	{
		*p++ = SPI_CMD_DUMMY(dummy_cycle); /* add dummy */
	}

	/* trx */
	if (has_trx_to_transfer)
	{
		if (tx_buf != NULL)
		{
			*p++ = SPI_CMD_SETUP_UCA(1, 0, tx_buf);
			*p++ = SPI_CMD_SETUP_UCS(1, 0, trx_buf_len);
		}

		if (rx_buf != NULL)
		{
			*p++ = SPI_CMD_SETUP_UCA(0, 0, rx_buf);
			*p++ = SPI_CMD_SETUP_UCS(0, 0, trx_buf_len);
		}

		if (tx_buf != NULL)
		{
			if (rx_buf != NULL)
			{
				*p++ = SPI_CMD_FUL(trx_buf_len, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), handle->bitOrder);
			}
			else
			{
				*p++ = SPI_CMD_TX_DATA(trx_buf_len, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, handle->bitOrder);
			}
		}
		else
		{
			*p++ = SPI_CMD_RX_DATA(trx_buf_len, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, handle->bitOrder);
		}
	}

	/* epilogue */
	*p++ = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	pulp_write32(UDMA_SPIM_CMD_ADDR(handle->id), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(handle->id), (uintptr_t) udma_spim_cmd, len * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

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
	while (plp_udma_busy(UDMA_SPIM_CMD_ADDR(handle->id)))
	{
		/*__asm__ volatile ("wfi\n");*/
	}

	/* free udma cmd */
	pi_l2_free(udma_spim_cmd, len * sizeof(uint32_t));

	return 0;
}

int spim_cmd_tx_rx(spim_t * __restrict handle,
	const uint8_t * __restrict cmd_buf, size_t cmd_bit_len,
	uint8_t dummy_cycle,
	const uint8_t * __restrict tx_buf, size_t tx_buf_len,
	uint8_t * __restrict rx_buf, size_t rx_buf_len)
{
	bool has_cmd_to_transfer   = (((cmd_buf == NULL) || (cmd_bit_len <= 0)) ? false : true);
	bool has_dummy_to_transfer = ((dummy_cycle <= 0) ? false : true);
	bool has_tx_to_transfer    = (((tx_buf == NULL) || (tx_buf_len <= 0)) ? false : true);
	bool has_rx_to_transfer    = (((rx_buf == NULL) || (rx_buf_len <= 0)) ? false : true);

	size_t len = 0; /* length of spim command, in dword (uint32_t) unit */

	size_t cmd_word_len = 0; /* number of generated SPI_CMD_SEND_CMD instruction */

	size_t i;

	uint32_t *udma_spim_cmd;
	uint32_t *p;

	/**
		* calculate spim cmd to allocate
		* cmd can be transferred per maximum 16-bit
		*/
	if (has_cmd_to_transfer)
	{
		cmd_word_len = cmd_bit_len / 16;
		cmd_bit_len %= 16;

		len +=  cmd_word_len;
		len += (cmd_bit_len > 0) ? 1 : 0;
	}

	/* check if we need transfer dummy */
	len += (has_dummy_to_transfer ? 1 : 0);


	if (has_tx_to_transfer)
	{
		len += 3;
	}

	if (has_rx_to_transfer)
	{
		len += 3;
	}

	/* exit if no transaction */
	if (len <= 0)
	{
		return 0;
	}

	/* add prologue and epilogue */
	len += 3;

	/**
		* FIXME: sanitize len to prevent user/caller to abuse memory allocation
		*/

	/* allocate (hopefully shared l2) buffer */
	udma_spim_cmd = (uint32_t *) pi_l2_malloc(len * sizeof(uint32_t));
	if (udma_spim_cmd == NULL)
	{
		return (-1); /* should return ENOMEM */
	}

	/* prologue: assert cs */
	p = udma_spim_cmd;
	/**p++ = SPI_CMD_CFG(spi_get_div(handle->max_baudrate),
		handle->polarity, handle->phase);*/
	*p++ = handle->cfg; /* cfg is prebuilt SPI_CMD_CFG command from init
		* for now we cannot provide a (simple) way to set SPI_CMD_CFG
		* for every spim transaction */
	*p++ = SPI_CMD_SOT(handle->cs);

	/* send cmd */
	if (has_cmd_to_transfer)
	{
		for (i = 0; i < cmd_word_len; ++i)
		{
			uint16_t cmd_word;
			cmd_word   = (uint16_t) *cmd_buf++;
			cmd_word <<= 8;
			cmd_word  |= (uint16_t) *cmd_buf++;

			/* 16-bit */
			*p++ = SPI_CMD_SEND_CMD(cmd_word, 16, SPI_CMD_QPI_DIS);
		}

		if (cmd_bit_len > 0)
		{
			uint16_t cmd_word;
			cmd_word   = (uint16_t) *cmd_buf++;
			if (cmd_bit_len > 8)
			{
				uint8_t mask = (1 << (cmd_bit_len - 8)) - 1;
				cmd_word <<= (cmd_bit_len - 8);
				cmd_word  |= (uint16_t) ((*cmd_buf/*++*/) & mask);
			}

			/* maximum bit */
			*p++ = SPI_CMD_SEND_CMD(cmd_word, cmd_bit_len, SPI_CMD_QPI_DIS);
		}
	}


	/* send dummy */
	if (has_dummy_to_transfer)
	{
		*p++ = SPI_CMD_DUMMY(dummy_cycle); /* add dummy */
	}

	/* tx, rx */
	if (has_tx_to_transfer)
	{
		*p++ = SPI_CMD_SETUP_UCA(1, 0, tx_buf);
		*p++ = SPI_CMD_SETUP_UCS(1, 0, tx_buf_len);
		*p++ = SPI_CMD_TX_DATA(tx_buf_len, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, handle->bitOrder);
	}

	if (has_rx_to_transfer)
	{
		*p++ = SPI_CMD_SETUP_UCA(0, 0, rx_buf);
		*p++ = SPI_CMD_SETUP_UCS(0, 0, rx_buf_len);
		*p++ = SPI_CMD_RX_DATA(rx_buf_len, SPI_CMD_1_WORD_PER_TRANSF, SPI_CMD_DATA_WITDH(8), SPI_CMD_QPI_DIS, handle->bitOrder);
	}


	/* epilogue */
	*p++ = SPI_CMD_EOT(SPI_CMD_EOT_EVENT_DIS, SPI_CMD_EOT_GEN_EVT_OFFSET); /* don't generate event, don't keep cs */

	pulp_write32(UDMA_SPIM_CMD_ADDR(handle->id), UDMA_CHANNEL_CFG_CLEAR);
	plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(handle->id), (uintptr_t) udma_spim_cmd, len * sizeof(uint32_t), UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_EN_BIT);

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
	while (plp_udma_busy(UDMA_SPIM_CMD_ADDR(handle->id)))
	{
		/*__asm__ volatile ("wfi\n");*/
	}

	/* free udma cmd */
	pi_l2_free(udma_spim_cmd, len * sizeof(uint32_t));

	return 0;
}

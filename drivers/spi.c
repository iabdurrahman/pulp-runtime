#include <pulp.h> 

#if ((ARCHI_UDMA_NB_SPIM) > 0)
static L2_DATA rt_spim_t __spim[ARCHI_UDMA_NB_SPIM];
#else
static L2_DATA rt_spim_t __spim[1];
#endif

typedef struct {
    unsigned int cmd[4];
} rt_spim_cmd_t;

static inline int __spim_id(int periph_id)
{
  return periph_id - ARCHI_UDMA_SPIM_ID(0); // 
}

static int __rt_spi_get_div(int spi_freq)
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

static inline int __rt_spim_get_byte_align(int wordsize, int big_endian)
{
  return wordsize == RT_SPIM_WORDSIZE_32 && big_endian;
}

rt_spim_t *rt_spim_open(rt_spim_conf_t *conf)
{

  if (conf == NULL)
  {
    return NULL; 
  }

  if (conf->id < 0 || conf->id >= ARCHI_UDMA_NB_SPIM) { 
    return NULL;
  }

  int periph_id = ARCHI_UDMA_SPIM_ID(conf->id);

  rt_spim_t *spim = &__spim[__spim_id(periph_id)];

  spim->channel = periph_id;

  spim->id = conf->id;

  spim->wordsize = conf->wordsize;
  spim->big_endian = conf->big_endian;
  spim->polarity = conf->polarity;
  spim->phase = conf->phase;
  spim->max_baudrate = conf->max_baudrate;
  spim->cs = conf->cs;
  spim->byte_align = __rt_spim_get_byte_align(conf->wordsize, conf->big_endian);

  int div = __rt_spi_get_div(spim->max_baudrate);
  spim->div = div;

  spim->cfg = SPI_CMD_CFG(div, conf->polarity, conf->phase);

  int id = periph_id - ARCHI_UDMA_SPIM_ID(0);

  plp_udma_cg_set(plp_udma_cg_get() | (1<<spim->channel));
  soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_PERIPH_EVT_BASE(spim->channel) + ARCHI_UDMA_SPIM_EOT_EVT);


  return spim;
}

void __rt_spim_send_async(rt_spim_t *spim, void *data, size_t len, int qspi, rt_spim_cs_e cs_mode)
{
  static L2_DATA rt_spim_cmd_t cmd_l2;
  rt_spim_cmd_t *cmd = &cmd_l2; 

  // cmd->cmd[0] = handle->cfg;
  cmd->cmd[0] = spim->cfg;
  // cmd->cmd[1] = SPI_CMD_SOT(handle->cs);
  cmd->cmd[1] = SPI_CMD_SOT(spim->cs);
//   cmd->cmd[2] = SPI_CMD_TX_DATA(len/32, SPI_CMD_1_WORD_PER_TRANSF, 32, qspi, SPI_CMD_MSB_FIRST);
  cmd->cmd[2] = SPI_CMD_TX_DATA(len/8, SPI_CMD_1_WORD_PER_TRANSF, 8, qspi, SPI_CMD_MSB_FIRST);
  cmd->cmd[3] = SPI_CMD_EOT(1, cs_mode == RT_SPIM_CS_KEEP);

  int buffer_size = len/8;
  int cfg = UDMA_CHANNEL_CFG_EN;

    plp_udma_enqueue(UDMA_SPIM_CMD_ADDR(spim->id), (int)cmd, 4*4, cfg);
    plp_udma_enqueue(UDMA_SPIM_TX_ADDR(spim->id), (int)data, buffer_size, cfg);

    while (plp_udma_busy(UDMA_SPIM_CMD_ADDR(spim->id))); 
    while (plp_udma_busy(UDMA_SPIM_TX_ADDR(spim->id))); 
}

// void __rt_spim_send(rt_spim_t *handle, void *data, size_t len, int qspi, rt_spim_cs_e cs_mode, rt_event_t *event)
void __rt_spim_send(rt_spim_t *handle, void *data, size_t len, int qspi, rt_spim_cs_e cs_mode)
{
//   int irq = rt_irq_disable();

//   rt_event_t *call_event = __rt_wait_event_prepare(event);

//   __rt_spim_send_async(handle, data, len, qspi, cs_mode, call_event);
  __rt_spim_send_async(handle, data, len, qspi, cs_mode);

//   __rt_wait_event_check(event, call_event);

//   rt_irq_restore(irq);
}

void rt_spim_conf_init(rt_spim_conf_t *conf)
{
  conf->wordsize = RT_SPIM_WORDSIZE_8;
  conf->big_endian = 0;
  conf->max_baudrate = 10000000;
  conf->cs_gpio = -1;
  conf->cs = -1;
  conf->id = -1;
  conf->polarity = 0;
  conf->phase = 0;
}
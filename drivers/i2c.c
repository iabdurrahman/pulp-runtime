#include <stdint.h>
#include <string.h>
#include "pulp.h"

#define I2C_CMD_BUFFER_SIZE 16

#if ((ARCHI_UDMA_NB_I2C) > 0)
static L2_DATA i2c_t __i2c[ARCHI_UDMA_NB_I2C];
#else
/*
  * at-least allocate 1 element
  *   FIXME: we should not allocated anything if hardware is not exist
  *          to reserve memory
  */
static L2_DATA i2c_t __i2c[1];
#endif

// // Get local array index from peripheral ID
static inline int __i2c_id(int periph_id)
{
    return periph_id - ARCHI_UDMA_I2C_ID(0);
}

// Compute divider from desired I2C frequency
static int i2c_get_div(int i2c_freq)
{
    int div = (pos_freq_domains[PI_FREQ_DOMAIN_PERIPH] + i2c_freq - 1) / i2c_freq;

    if (div & 1) div += 1;
    div >>= 1;

    return div;
}

i2c_t *i2c_open(i2c_dev_t *dev)
{

    if (dev == NULL) {
        return NULL;
    }

    if (dev->id < 0 || dev->id >= ARCHI_UDMA_NB_I2C) {
        return NULL;
    }

    int periph_id = ARCHI_UDMA_I2C_ID(dev->id); // 2

    i2c_t *i2c =  &__i2c[__i2c_id(periph_id)];

    i2c->periph_id = periph_id * 2;
    i2c->id = dev->id;
    i2c->cs = dev->cs;
    i2c->max_baudrate = dev->max_baudrate;
    i2c->div = i2c_get_div(dev->max_baudrate);

    plp_udma_cg_set(plp_udma_cg_get() | (1<<periph_id));
    soc_eu_fcEventMask_setEvent(periph_id);
    soc_eu_fcEventMask_setEvent(periph_id + 1);

    return i2c;
}

void i2c_close(i2c_t *i2c)
{
    if (i2c != NULL) {
        plp_udma_cg_set(plp_udma_cg_get() & ~(1<<i2c->periph_id));
    }
}

void i2c_write(i2c_t *dev, unsigned char *data, int length)
{
    unsigned char udma_cmd[I2C_CMD_BUFFER_SIZE];
    int seq_index = 0;

    int i;

    udma_cmd[seq_index++] = I2C_CMD_CFG;
    udma_cmd[seq_index++] = (dev->div >> 8) & 0xFF;
    udma_cmd[seq_index++] = (dev->div & 0xFF);

    udma_cmd[seq_index++] = I2C_CMD_START;
    udma_cmd[seq_index++] = I2C_CMD_WR;
    udma_cmd[seq_index++] = dev->cs;

    if (length > 1) {
        udma_cmd[seq_index++] = I2C_CMD_RPT;
        udma_cmd[seq_index++] = length;
    }

    udma_cmd[seq_index++] = I2C_CMD_WR;
    for (i = 0; i<length; i++) {
        udma_cmd[seq_index++] = data[i];
    }

    udma_cmd[seq_index++] = I2C_CMD_STOP;

    plp_udma_enqueue(UDMA_I2C_TX_ADDR(dev->id), (unsigned int)udma_cmd, seq_index, UDMA_CHANNEL_CFG_EN);

    while (plp_udma_busy(UDMA_I2C_TX_ADDR(dev->id)));
    while (plp_i2c_tx_busy(dev->id));
}

void i2c_read(i2c_t *dev, unsigned char *rx_buff, int length, int pending)
{
    unsigned char udma_cmd[I2C_CMD_BUFFER_SIZE];
    int seq_index = 0;

    udma_cmd[seq_index++] = I2C_CMD_CFG;
    udma_cmd[seq_index++] = (dev->div >> 8) & 0xFF;
    udma_cmd[seq_index++] = (dev->div & 0xFF);

    udma_cmd[seq_index++] = I2C_CMD_START;
    udma_cmd[seq_index++] = I2C_CMD_WR;
    udma_cmd[seq_index++] = dev->cs | 0x1; // READ

    if (length > 1)
    {
        udma_cmd[seq_index++] = I2C_CMD_RPT;
        udma_cmd[seq_index++] = length - 1;
        udma_cmd[seq_index++] = I2C_CMD_RD_ACK;
    }

    udma_cmd[seq_index++] = I2C_CMD_RD_NACK;

    if (!pending)
        udma_cmd[seq_index++] = I2C_CMD_STOP;

    plp_udma_enqueue(UDMA_I2C_DATA_ADDR(dev->id), (unsigned int)rx_buff, length, UDMA_CHANNEL_CFG_EN);
    plp_udma_enqueue(UDMA_I2C_TX_ADDR(dev->id), (unsigned int)udma_cmd, seq_index, UDMA_CHANNEL_CFG_EN);

    while (plp_udma_busy(UDMA_I2C_DATA_ADDR(dev->id)));
    while (plp_udma_busy(UDMA_I2C_TX_ADDR(dev->id)));
    while (plp_i2c_tx_busy(dev->id));
}

void i2c_dev_init(i2c_dev_t *dev)
{
  dev->cs = -1;
  dev->id = -1;
  dev->max_baudrate = 200000;
}
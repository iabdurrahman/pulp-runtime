/*
 * Copyright (C) 2019 ETH Zurich and University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __PULP_H__
#define __PULP_H__

#ifndef CLUSTER_STACK_SIZE
#define CLUSTER_STACK_SIZE 0x800
#endif

#ifndef LANGUAGE_ASSEMBLY

#include <bench/bench.h>
#include <archi/pulp.h>
#include <hal/pulp.h>
#include <data/data.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  PI_FREQ_DOMAIN_FC     = 0,
  PI_FREQ_DOMAIN_CL     = 2, //1
  PI_FREQ_DOMAIN_PERIPH = 1, //2
  PI_FREQ_NB_DOMAINS    = 2  //3
} pi_freq_domain_e;

#ifdef ARCHI_HMR
  void pos_hmr_store_part_to_stack(void);
  void pos_hmr_store_rest_to_stack(void);
  void pos_hmr_load_part_from_stack(void);
  void pos_hmr_store_state_to_stack(void);
  void pos_hmr_load_state_from_stack(void);
  void pos_hmr_sw_reload(void);
  void pos_hmr_tmr_irq(void);
  void pos_hmr_synch(void);
  void pos_hmr_tmr_synch_entry(void);
  void pos_hmr_tmr_synch_exit(void);
  void pos_hmr_tmr_synch(void);
  void pos_hmr_dmr_synch_entry(void);
  void pos_hmr_dmr_synch_exit(void);
  void pos_hmr_dmr_synch(void);
  int  hmr_tmr_critical_section(int (*function_handle)(void));
  int  hmr_dmr_critical_section(int (*function_handle)(void));
  void hmr_tmr_performance_section(void (*function_handle)(void));
  void hmr_tmr_perf_setup_sp(void);
  void hmr_dmr_performance_section(void (*function_handle)(void));
  void hmr_dmr_perf_setup_sp(void);
#endif

#ifdef ARCHI_HAS_CLUSTER

extern L1_DATA char *cluster_stacks;

void cluster_start(int cid, int (*entry)());

void cluster_entry_stub();

int cluster_wait(int cid);

#else

static inline void cluster_start(int cid, int (*entry)())
{
}

static inline int cluster_wait(int cid)
{
  return 0;
}

#endif

void _start();

#include <implem/implem.h>

#define get_core_id hal_core_id
#define rt_core_id hal_core_id
#define rt_cluster_id hal_cluster_id
#define get_cluster_id hal_cluster_id

#ifdef ARCHI_CLUSTER_NB_PE
static inline int get_core_num() { return ARCHI_CLUSTER_NB_PE; }
#endif


int uart_open(int uart_id, int baudrate);
void uart_close(int uart_id);
int uart_write(int uart_id, void *buffer, uint32_t size);
int uart_read(int uart_id, void *buffer, uint32_t size);

//i2c section

typedef struct {
    int periph_id;
    int id;
    char cs;
    unsigned int  max_baudrate;
    unsigned int  div;
} i2c_t;

#define I2C_CMD_BUFFER_SIZE 16

typedef struct {
  signed char id;
  signed char cs;
  unsigned int max_baudrate;
} i2c_dev_t;


i2c_t *i2c_open(i2c_dev_t *dev);
void i2c_close(i2c_t *i2c);
void i2c_write(i2c_t *dev, unsigned char *data, int length);
void i2c_read(i2c_t *dev_i2c, unsigned char *rx_buff, int length, int pending);
void i2c_dev_init(i2c_dev_t *dev);


//i2c section

//SPI section

typedef enum {
  SPIM_WORDSIZE_8 = 0,     /*!< Each element is 8 bits. Thus the endianness has no effect. */
  SPIM_WORDSIZE_32 = 1     /*!< Each element is 32 bits. The way each element is stored in memory can then be specified with the endianness. */
} spim_wordsize_e;

typedef struct {
  int id; 
  char channel;
  char wordsize;          
  char big_endian;        
  char polarity;          
  char phase;             
  int max_baudrate;       
  char cs;
  char byte_align;         
  unsigned int div;
  unsigned int cfg;
  signed char cs_gpio; 
} spim_t;

typedef struct {
  int max_baudrate;       /*!< Maximum baudrate for the SPI bitstream which can be used with the opened device . */
  char wordsize;          /*!< Wordsize of the elements in the bitstream. Can be RT_SPIM_WORDSIZE_8 for 8 bits data or RT_SPIM_WORDSIZE_32 for 32 bits data. This is used to interpret the endianness. */
  char big_endian;        /*!< If 1, the elements are stored in memory in a big-endian way, i.e. the most significant byte is stored at the lowest address. This is taken into account only if the wordsize is 32 bits. */
  char polarity;          /*!< Polarity of the clock. */
  char phase;             /*!< Phase of the clock. */
  signed char cs_gpio;    /*!< If it is different from -1, the specified number is used to drive a GPIO which is used as a chip select for the SPI device. The cs field is then ignored. */
  signed char cs;         /*!< If cs_gpio is -1, the normal chip select pins are used and this field specifies which one to use for the device. */
  signed char id;         /*!< If it is different from -1, this specifies on which SPI interface the device is connected. */
} spim_conf_t;

typedef enum {
  SPIM_CS_AUTO     = 0,    /*!< Handles the chip select automatically. It is set low just before the transfer is started and set back high when the transfer is finished. */
  SPIM_CS_KEEP     = 1,    /*!< Handle the chip select manually. It is set low just before the transfer is started and is kept low until the next transfer. */
  SPIM_CS_NONE     = 2,    /*!< Don't do anything with the chip select. */
} spim_cs_e;

spim_t *spim_open(spim_conf_t *spim);
void spim_send(spim_t *spim, void *data, size_t len, int qspi, spim_cs_e mode);
void spim_conf_init(spim_conf_t *spim);

//SPI section

void synch_barrier();


void *pi_l1_malloc(int cid, int size);
void pi_l1_free(int cid, void *chunk, int size);

void *pi_l2_malloc(int size);
void pi_l2_free(void *_chunk, int size);

#ifdef __cplusplus
}
#endif



#endif

#endif

/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna
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

#include "pulp.h"



#define UART_BAUDRATE 115200

#define RX_BUFFER_SIZE 128
#define User_Area_Limit 64
#define uDMA_Area_Size 64
#define uDMA_Start_idx 64 

/**
  * always generate at least 1 uart buffer, even if uart is not uart
  * FIXME: this code/API should not even available if uart ip core is not synthesized
  */
#if ((! defined(ARCHI_UDMA_NB_UART)) || ((ARCHI_UDMA_NB_UART) <= 0))
#define __ARCHI_UDMA_NB_UART 1 /* generate at least 1 uart buffer even if no uart hardware available */
#else
#define __ARCHI_UDMA_NB_UART ARCHI_UDMA_NB_UART
#endif

/**
  * FIXME: there are should be a more proper way to define flexible array of uart buffer here
  */

static L2_DATA volatile uint8_t RX_BUFFER_0[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#if ((__ARCHI_UDMA_NB_UART) >= 2)
static L2_DATA volatile uint8_t RX_BUFFER_1[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 3)
static L2_DATA volatile uint8_t RX_BUFFER_2[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 4)
static L2_DATA volatile uint8_t RX_BUFFER_3[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 5)
static L2_DATA volatile uint8_t RX_BUFFER_4[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 6)
static L2_DATA volatile uint8_t RX_BUFFER_5[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 7)
static L2_DATA volatile uint8_t RX_BUFFER_6[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 8)
static L2_DATA volatile uint8_t RX_BUFFER_7[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 9)
static L2_DATA volatile uint8_t RX_BUFFER_8[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 10)
static L2_DATA volatile uint8_t RX_BUFFER_9[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 11)
static L2_DATA volatile uint8_t RX_BUFFER_10[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 12)
static L2_DATA volatile uint8_t RX_BUFFER_11[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 13)
static L2_DATA volatile uint8_t RX_BUFFER_12[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 14)
static L2_DATA volatile uint8_t RX_BUFFER_13[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 15)
static L2_DATA volatile uint8_t RX_BUFFER_14[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 16)
static L2_DATA volatile uint8_t RX_BUFFER_15[RX_BUFFER_SIZE] __attribute__((aligned(4)));
#endif
#if ((__ARCHI_UDMA_NB_UART)  > 16)
  #error only support up to 16 uart ip core
#endif

/**
  * rx buffer table,
  * don't have allovated in (shared) L2, just put in .code or even at .text is enough
  * support up-to 16 uart ip core
  */
static volatile uint8_t * const RX_BUFFER_TABLE[] =
{
  RX_BUFFER_0, /* ARCHI_UDMA_NB_UART == 1 */
#if ((__ARCHI_UDMA_NB_UART) >= 2)
  RX_BUFFER_1, /* ARCHI_UDMA_NB_UART == 2 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 3)
  RX_BUFFER_2, /* ARCHI_UDMA_NB_UART == 3 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 4)
  RX_BUFFER_3, /* ARCHI_UDMA_NB_UART == 4 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 5)
  RX_BUFFER_4, /* ARCHI_UDMA_NB_UART == 5 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 6)
  RX_BUFFER_5, /* ARCHI_UDMA_NB_UART == 6 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 7)
  RX_BUFFER_6, /* ARCHI_UDMA_NB_UART == 7 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 8)
  RX_BUFFER_7, /* ARCHI_UDMA_NB_UART == 8 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 9)
  RX_BUFFER_8, /* ARCHI_UDMA_NB_UART == 9 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 10)
  RX_BUFFER_9, /* ARCHI_UDMA_NB_UART == 10 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 11)
  RX_BUFFER_10, /* ARCHI_UDMA_NB_UART == 11 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 12)
  RX_BUFFER_11, /* ARCHI_UDMA_NB_UART == 12 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 13)
  RX_BUFFER_12, /* ARCHI_UDMA_NB_UART == 13 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 14)
  RX_BUFFER_13, /* ARCHI_UDMA_NB_UART == 14 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 15)
  RX_BUFFER_14, /* ARCHI_UDMA_NB_UART == 15 */
#endif
#if ((__ARCHI_UDMA_NB_UART) >= 16)
  RX_BUFFER_15, /* ARCHI_UDMA_NB_UART == 16 */
#endif
#if ((__ARCHI_UDMA_NB_UART) > 16)
  #error only support up to 16 uart ip core
#endif
  0
};

/* position control for each rx buffer */
static volatile uint32_t user_head[__ARCHI_UDMA_NB_UART] = {-1};
static volatile uint32_t user_tail[__ARCHI_UDMA_NB_UART] = {0};
static uint32_t last_hw_count[__ARCHI_UDMA_NB_UART] = {0};
static uint32_t current_hw_count[__ARCHI_UDMA_NB_UART] = {0};


static void uart_wait_tx_done(int periph)
{
  while (plp_udma_busy(UDMA_UART_TX_ADDR(periph - ARCHI_UDMA_UART_ID(0))))
  {
  }

  // while(plp_uart_tx_busy(periph - ARCHI_UDMA_UART_ID(0)));
}


static void uart_wait_rx_done(int periph)
{
  while (plp_udma_busy(UDMA_UART_RX_ADDR(periph - ARCHI_UDMA_UART_ID(0))))
  {
  }
  // while(plp_uart_rx_busy(periph - ARCHI_UDMA_UART_ID(0)));
}



static void uart_setup(int channel, int baudrate)
{
  int div =  (pos_freq_domains[PI_FREQ_DOMAIN_PERIPH] + baudrate/2) / baudrate;

  plp_uart_setup(channel - ARCHI_UDMA_UART_ID(0), 0, div-1);
}



int uart_open(int uart_id, int baudrate)
{
  int periph_id = ARCHI_UDMA_UART_ID(uart_id);
  int channel = UDMA_EVENT_ID(periph_id);

  /**
    * FIXME: uart_id is come from API user, doesn't it need to be sanitized?
    *        to prevent overflow. right now we handoff the responsibility to check
    *        uart_id to (API) user
    */
  volatile uint8_t *__RX_BUFFER = RX_BUFFER_TABLE[uart_id];

  unsigned int rx_base = hal_udma_channel_base(channel);
  
  pulp_write32(rx_base + UDMA_CHANNEL_CFG_OFFSET, (1 << 6));

  user_head[uart_id] = -1;
  user_tail[uart_id] = 0;
  last_hw_count[uart_id] = 0;

  /* FIXME: maybe each RX_BUFFER can have different size? */
  for (int i = 0; i<RX_BUFFER_SIZE; i++) __RX_BUFFER[i] = 0;

  plp_udma_cg_set(plp_udma_cg_get() | (1<<periph_id));

  soc_eu_fcEventMask_setEvent(channel);
  soc_eu_fcEventMask_setEvent(channel+1);

  uart_setup(periph_id, baudrate);

  unsigned int base = hal_udma_channel_base(channel);

  plp_udma_enqueue(base, (int)(&(__RX_BUFFER[uDMA_Start_idx])), uDMA_Area_Size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8 | UDMA_CHANNEL_CFG_CONT);

  return 0;
}



int uart_available(int uart_id)
{
  int periph_id = ARCHI_UDMA_UART_ID(uart_id);
  int channel = UDMA_EVENT_ID(periph_id);

  /* FIXME: please sanitize uart_id! we assume that user use this API with good intention which is unfortunately not always true */
  volatile uint8_t *__RX_BUFFER = RX_BUFFER_TABLE[uart_id];

  uint32_t __user_head = user_head[uart_id];
  uint32_t __user_tail = user_tail[uart_id];

  uint32_t __last_hw_count = last_hw_count[uart_id];
  uint32_t __current_hw_count = current_hw_count[uart_id];

  unsigned int base = hal_udma_channel_base(channel);

  uint32_t remaining = pulp_read32(base + UDMA_CHANNEL_SIZE_OFFSET);
  __current_hw_count = uDMA_Area_Size - remaining;

  if (__current_hw_count != __last_hw_count) {
    int count = (__current_hw_count - __last_hw_count + uDMA_Area_Size) % uDMA_Area_Size;
    int remaining_user_area = (User_Area_Limit - 1) - __user_head;

    if (count > remaining_user_area) {
      int shift = count - remaining_user_area;

      if (__user_head >= 0) {
        int bytes_to_move = (__user_head - shift + 1);

        if(bytes_to_move > 0) {
          for (int i = 0; i < bytes_to_move; i++) {
            __RX_BUFFER[i] = __RX_BUFFER[i + shift];
          }
          __user_head -= shift;
        } else {
          __user_head = -1;
        }

        if (__user_tail >= shift) {
          __user_tail -= shift;
        } else {
          __user_tail = 0;
        }
      }
    }

    int write_position = __user_head + 1;
    for (int i = 0; i < count; i++) { /* emulate memmove? */
      uint32_t udma_idx = uDMA_Start_idx + ((__last_hw_count + i) % uDMA_Area_Size);
      if (write_position + i < User_Area_Limit) {
        __RX_BUFFER[write_position + i] = __RX_BUFFER[udma_idx];
        __RX_BUFFER[udma_idx] = 0;
      }
    }

    __user_head += count;
    if (__user_head >= User_Area_Limit) __user_head = User_Area_Limit - 1;

    __last_hw_count = __current_hw_count;
  }

  /**
    * update all volatile variable
    * do we need to tell compiler to not reorder?
    */
  user_head[uart_id] = __user_head;
  user_tail[uart_id] = __user_tail;

  last_hw_count[uart_id] = __last_hw_count;
  current_hw_count[uart_id] = __current_hw_count;

  if (((int32_t) (__user_head)) < 0) return 0;
  return (__user_head - __user_tail + 1);
}

int uart_peek(int uart_id)
{
  /* FIXME: please sanitize @arg uart_id */
  volatile uint8_t *__RX_BUFFER = RX_BUFFER_TABLE[uart_id];

  if (uart_available(uart_id) == 0) return -1;

  /**
    * is succesfull call to uart_available (return is more than 0)
    * guarantee that user_tail is 0 or positive?
    */
  return __RX_BUFFER[user_tail[uart_id]];
}

int uart_read(int uart_id)
{
  /* FIXME: please sanitize @arg uart_id */
  volatile uint8_t *__RX_BUFFER = RX_BUFFER_TABLE[uart_id];

  uint32_t          __user_head = user_head[uart_id];
  uint32_t          __user_tail = user_tail[uart_id];

  if (uart_available(uart_id) <= 0) return -1;

  uint8_t data = __RX_BUFFER[__user_tail];
  __RX_BUFFER[__user_tail] = 0;

  if (__user_tail < __user_head) {
    __user_tail++;
  } else {
    __user_head = -1;
    __user_tail = 0;
  }

  /**
    * update volatile data
    * do we need to tell compiler to not reorder? maybe -O3 will mess this up
    */
  user_head[uart_id] = __user_head;
  user_tail[uart_id] = __user_tail;

  return (int)data;
}



void uart_close(int uart_id)
{
  int periph_id = ARCHI_UDMA_UART_ID(uart_id);
  int channel = UDMA_EVENT_ID(periph_id);
  unsigned int rx_base = hal_udma_channel_base(channel);

  pulp_write32(rx_base + UDMA_CHANNEL_CFG_OFFSET, UDMA_CHANNEL_CFG_CLEAR);

  uart_wait_tx_done(periph_id);

  plp_uart_disable(periph_id);      

  plp_udma_cg_set(plp_udma_cg_get() & ~(1<<periph_id));
}



int uart_write(int uart_id, const void *buffer, uint32_t size)
{
  int periph_id = ARCHI_UDMA_UART_ID(uart_id);
  int channel = UDMA_EVENT_ID(periph_id) + 1;

  unsigned int base = hal_udma_channel_base(channel);

  plp_udma_enqueue(base, (int)buffer, size, UDMA_CHANNEL_CFG_EN | UDMA_CHANNEL_CFG_SIZE_8);
  
  uart_wait_tx_done(periph_id);

  return 0;
}

/**
  * Copyright (C) 2025 Indonesia Chip Design Collaborative Center (ICDEC)
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
  * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT.
  */

#include <stdbool.h>
#include <stdint.h>

#include "pulp.h"

/**
  * set clock source
  */
void timer_set_clock_source(unsigned int addr, timer_id_t id, bool is_use_32khz)
{
  timer_cfg_lo_t    cfg_lo_reg_value;
  timer_cfg_hi_t    cfg_hi_reg_value;


  switch (id)
  {
  case (TIMER_LO):
    /* get current cfg register value */
    cfg_lo_reg_value.raw = timer_cfg_lo_get(addr);

    if (is_use_32khz)
    {
      /* is_use_32khz == true */
      cfg_lo_reg_value.ccfg = 1;
    }
    else
    {
      /**
        * is_use_32khz == false
        * use FLL
        */
      cfg_lo_reg_value.ccfg = 0;
    }

    /* write new register value */
    timer_cfg_lo_set(addr, (uint32_t) cfg_lo_reg_value.raw);
    break;

  case (TIMER_HI):
    /* same with low */
    cfg_hi_reg_value.raw = timer_cfg_hi_get(addr);
    if (is_use_32khz)
    {
      cfg_hi_reg_value.clkcfg = 1;
    }
    else
    {
      cfg_hi_reg_value.clkcfg = 0;
    }
    timer_cfg_hi_set(addr, (uint32_t) cfg_hi_reg_value.raw);
    break;
  }
}


/* set prescaler, and optionally enable prescaler */
void timer_set_prescaler(unsigned int addr, timer_id_t id, bool is_enable, uint8_t prescaler_value)
{
  /**
    * prescaler can be use or not
    * even if is_enable is false, prescaler_value will still set to cfg register
    */
  timer_cfg_lo_t    cfg_lo_reg_value;
  timer_cfg_hi_t    cfg_hi_reg_value;


  switch (id)
  {
  case (TIMER_LO):
    cfg_lo_reg_value.raw = timer_cfg_lo_get(addr);

    /* enable or disable prescaler */
    if (is_enable)
    {
      cfg_lo_reg_value.pen = 1;
    }
    else
    {
      cfg_lo_reg_value.pen = 0;
    }

    /* set prescaler value */
    cfg_lo_reg_value.pval = prescaler_value;

    /* write new register value */
    timer_cfg_lo_set(addr, (uint32_t) cfg_lo_reg_value.raw);
    break;

  case (TIMER_HI):
    cfg_hi_reg_value.raw = timer_cfg_hi_get(addr);

    /* enable or disable prescaler */
    if (is_enable)
    {
      /* is_enable == true */
      cfg_hi_reg_value.pen = 1;
    }
    else
    {
      /* is_enable == false */
      cfg_hi_reg_value.pen = 0;
    }

    /* set prescaler value */
    cfg_hi_reg_value.pval = prescaler_value;

    /* write new register value */
    timer_cfg_hi_set(addr, (uint32_t) cfg_hi_reg_value.raw);
    break;
  }
}

/**
  * set counter counting mode
  *
  * if is_oneshot set to true, then counter will automatically disabled (stop counting)
  * after counter value reach compare value (only 1 interrupt is generated)
  *
  * if is_oneshot set to false, then counter will continue to run even after counter value
  * reach compare value, the behaviour of counting value is defined by is_reset_after_compare_match
  * argument
  *
  * is_reset_after_compare_match value will be set to cfg register regardless is_oneshot value
  * is_reset_after_compare_match only takes effect in timer peripheral if is_oneshot set to false
  *
  * if is_reset_after_compare_match set to false then after counter reach compare value,
  * timer will continue to increment counter value, to change counter value, call
  * timer_set_counter_value
  *
  * if is_reset_after_compare_match set to true then after counter reach compare value,
  * counter value will be set to 0
  */
void timer_set_continuity(unsigned int addr, timer_id_t id, bool is_oneshot, bool is_reset_after_compare_match)
{
  timer_cfg_lo_t    cfg_lo_reg_value;
  timer_cfg_hi_t    cfg_hi_reg_value;


  switch (id)
  {
  case (TIMER_LO):
    cfg_lo_reg_value.raw = timer_cfg_lo_get(addr);

    /* enable or disable oneshot */
    if (is_oneshot)
    {
      /* is_oneshot == true */
      cfg_lo_reg_value.one_s = 1;
    }
    else
    {
      /* is_oneshot == false */
      cfg_lo_reg_value.one_s = 0;
    }

    /* set continous counter mode */
    if (is_reset_after_compare_match)
    {
      /* is_reset_after_compare_match == true */
      cfg_lo_reg_value.mode = 1;
    }
    else
    {
      /* is_reset_after_compare_match == false */
      cfg_lo_reg_value.mode = 0;
    }

    /* write new register value */
    timer_cfg_lo_set(addr, (uint32_t) cfg_lo_reg_value.raw);
    break;

  case (TIMER_HI):
    /* same with lo */
    cfg_hi_reg_value.raw = timer_cfg_hi_get(addr);

    if (is_oneshot)
    {
      cfg_hi_reg_value.one_s = 1;
    }
    else
    {
      cfg_hi_reg_value.one_s = 0;
    }

    if (is_reset_after_compare_match)
    {
      cfg_hi_reg_value.mode = 1;
    }
    else
    {
      cfg_hi_reg_value.mode = 0;
    }

    timer_cfg_hi_set(addr, (uint32_t) cfg_hi_reg_value.raw);
    break;
  }
}

void timer_enable_irq(unsigned int addr, timer_id_t id)
{
  timer_cfg_lo_t    cfg_lo_reg_value;
  timer_cfg_hi_t    cfg_hi_reg_value;

  switch (id)
  {
  case (TIMER_LO):
    /**
      * FIXME: these operation should be atomic
      */

    /* get current cfg value */
    cfg_lo_reg_value.raw = timer_cfg_lo_get(addr);

    /* enable irq */
    cfg_lo_reg_value.irqen = 1;

    /* update cfg register */
    timer_cfg_lo_set(addr, cfg_lo_reg_value.raw);
    break;

  case (TIMER_HI):
    /* same with lo timer */
    cfg_hi_reg_value.raw = timer_cfg_hi_get(addr);
    cfg_hi_reg_value.irqen = 1;
    timer_cfg_hi_set(addr, cfg_hi_reg_value.raw);
    break;
  }
}

void timer_disable_irq(unsigned int addr, timer_id_t id)
{
  timer_cfg_lo_t    cfg_lo_reg_value;
  timer_cfg_hi_t    cfg_hi_reg_value;

  switch (id)
  {
  case (TIMER_LO):
    /**
      * FIXME: these operation should be atomic
      */

    /* get current cfg value */
    cfg_lo_reg_value.raw = timer_cfg_lo_get(addr);

    /* enable irq */
    cfg_lo_reg_value.irqen = 0;

    /* update cfg register */
    timer_cfg_lo_set(addr, cfg_lo_reg_value.raw);
    break;

  case (TIMER_HI):
    /* same with lo timer */
    cfg_hi_reg_value.raw = timer_cfg_hi_get(addr);
    cfg_hi_reg_value.irqen = 0;
    timer_cfg_hi_set(addr, cfg_hi_reg_value.raw);
    break;
  }
}

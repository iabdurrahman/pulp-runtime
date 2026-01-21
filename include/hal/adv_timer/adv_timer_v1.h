/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __HAL_ADV_TIMER_ADV_TIMER_V1_H__
#define __HAL_ADV_TIMER_ADV_TIMER_V1_H__

#include <stdint.h>
#include <stddef.h>

#include "hal/pulp.h"

/* archi version of this library */
#include "archi/adv_timer/adv_timer_v1.h"

#ifdef __cplusplus
extern "C"
{
#endif


int adv_timer_get_timer_and_channel_from_io(
	unsigned int gpio, unsigned int mux_index,
	size_t *timer, size_t *channel);

void adv_timer_start(size_t timer);

void adv_timer_stop(size_t timer); /* stop */
void adv_timer_stop_and_reset(size_t timer);

void adv_timer_update(size_t timer);

void adv_timer_enable_clock(size_t timer);
void adv_timer_disable_clock(size_t timer);

void adv_timer_config_frequency(size_t timer,
	uint8_t clock_prescaler, uint16_t counter_threshold);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ! defined(__HAL_ADV_TIMER_ADV_TIMER_V1_H__) */

/**
	* Copyright (C) 2025 ICDEC
	*/

#include "implem/tick.h"
#include "implem/delay.h"

void pos_delay_busy_ms(long delay_ms)
{
	long timeout;

	/* immediately return if given delay is 0 or negative */
	if (delay_ms <= 0)
	{
		return;
	}

	/* calculate timeout */
	timeout = pos_tick_get_counter_ms() + delay_ms;

	/* use function get tick to check whether we should return */
	while ((timeout - pos_tick_get_counter_ms()) > 0)
	{
		/* loop if tick smaller than timeout  */
	}
}

void pos_delay_busy_us(long delay_us)
{
	long timeout;

	/* immediately return if given delay is 0 or negative */
	if (delay_us <= 0)
	{
		return;
	}

	/* calculate timeout */
	timeout = pos_tick_get_counter_us() + delay_us;

	/* use function get tick to check whether we should return */
	while ((timeout - pos_tick_get_counter_us()) > 0)
	{
		/* loop if tick smaller than timeout  */
	}
}

/**
	* Copyright (C) 2025 ICDEC
	*/

#include <stdint.h>

#include "pulp.h"
#include "implem/tick.h"

/*static void stop_when_retval_is_non_zero(int retval);*/

static uint64_t get_counter_ms_unsigned(void);
static uint64_t get_counter_us_unsigned(void);

static int safe_pull_counter_value(uint64_t *tick_counter_ms_value, uint64_t *timer_counter_value);

void pos_tick_init(void)
{
	/* stop and reset timer */
	timer_stop(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID);

	timer_reset(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID);

	/**
		* **************************************************
		* enable interrupt (apb interrupt cntr and core (mstatus, mtvec, etc))
		* **************************************************
		*/

	/**
		* interrupt already enabled before this function is called
		* (in pos_init_start() in init.c)
		* by calling hal_irq_enable()
		*/

	/**
		* attach interrupt timer handler (tick_1ms_interrupt_handler)
		* provided by tick.h (declaration) & tick32.S (definition)
		* to vector table
		*/
#if     ((POS_TICK_TIMER_ID) == 0) /* timer_lo */
	rt_irq_set_handler(10, &pos_tick_1ms_interrupt_handler);
	hal_itc_enable_set(hal_itc_enable_value_get() | (1 << 10));

#elif   ((POS_TICK_TIMER_ID) == 1) /* timer_hi */
	rt_irq_set_handler(11, &pos_tick_1ms_interrupt_handler);
	hal_itc_enable_set(hal_itc_enable_value_get() | (1 << 11));

#else
#error  POS_TICK_TIMER_ID should be defined with value 0 or 1 (0 is for enabling timer_lo as tick or 1 is for enabling timer_hi as tick)

#endif


	/**
		* **************************************************
		* setup timer
		* **************************************************
		*/

	/* set timer clock source is fll */
	timer_set_clock_source(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID, false);


	/**
		* enable (or disable) prescaler so timer counting in TICK_TIMER_COUNTER_US_GRANULARITY
		*/
#if     ((POS_TICK_USE_PRESCALER) == 0)
	/* don't use prescaler */
	timer_set_prescaler(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID, false, 1); /* set prescaler to dummy value 1 */
#else
	/* use prescaler */
	timer_set_prescaler(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID, true, POS_TICK_PRESCALER_VALUE);
#endif

	/**
		* mode continue
		* reset counter value after compare value reached
		*/
	timer_set_continuity(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID, false, true);

	/**
		* set compare value according previous calculation
		*/
	timer_cmp_set(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID, POS_TICK_TIMER_COMPARE_VALUE);


	/**
		* enable interrupt signal to be sent to apb interrupt cntr
		*/
	timer_enable_irq(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID);

	/* start timer */
	timer_start(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID);
}

void pos_tick_stop(void)
{
	/* stop timer */
	timer_stop(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID);

	/* TODO: clear interrupt handling */
}


long pos_tick_get_counter_ms(void)
{
	return (long) get_counter_ms_unsigned();
}

long pos_tick_get_counter_us(void)
{
	return (long) get_counter_us_unsigned();
}


static int safe_pull_counter_value(uint64_t *tick_counter_ms_value, uint64_t *timer_counter_value)
{
	/**
		* prepare 2 storage to save counter value
		* based on tick counter value, we can predict which tick ms counter that is safe to use
		*
		* assume timer counter value is a and second capture of timer counter value is a*
		* assume tick ms counter value is b and second capture of tick ms counter value is b*
		*
		* we capture in following sequence:
		* 1. a
		* 2. b
		* 3. a*
		* 4. b*
		*
		* if (a*  > a) then it is guaranteed that step 1 - 3 is free from interrupt, we can assume that b is safe, and b* is unsafe (use a for timer counter value)
		* if (a*  < a) then it is guaranteed that interrupt is occured in 1 - 3, we can assume that b is unsafe, and b* is safe (use a* for timer counter value)
		* if (a* == a) and (b* != b) then interrupt happen in 1 - 4, no value can be used, just read again and use latest reading
		*
		* additional notes:
		* reading value of timer counter is atomic operation (just 1 lw instruction)
		* but reading tick ms counter value is non atomic operation (need at least 2 lw operation)
		* so it is safe to compare timer counter value with less than or grater than operator
		* but we can only use equal or not equal operator for comparing tick ms value, since reading is not atomic
		*/

	uint64_t  __local_tick_counter_ms_1;
	uint32_t  __local_timer_counter_value_1;

	uint64_t  __local_tick_counter_ms_2;
	uint32_t  __local_timer_counter_value_2;



	/**
		* get timer counter and tick counter ms (phase 1)
		*/

	/* read timer counter register */
	__local_timer_counter_value_1  = (uint32_t) timer_count_get(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID);

	/* copy current tick value */
	__local_tick_counter_ms_1      =            pos_tick_counter_ms;



	/**
		* get timer counter and tick counter ms (phase 2)
		*/

	/* read timer counter register again */
	__local_timer_counter_value_2  = (uint32_t) timer_count_get(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID);

	/* copy current tick value again */
	__local_tick_counter_ms_2      =            pos_tick_counter_ms;


	/* check __local_timer_counter_value_1 and __local_timer_counter_value_2 relation */
	if      (__local_timer_counter_value_2 > __local_timer_counter_value_1)
	{
		/**
			* no interrupt occured in first reading of tick_counter_ms
			* return first reading
			*/
		*timer_counter_value    = (uint64_t)  __local_timer_counter_value_1;
		*tick_counter_ms_value  =             __local_tick_counter_ms_1;
	}
	else if (__local_timer_counter_value_2 < __local_timer_counter_value_1)
	{
		/**
			* there is interrupt occured in first reading of tick_counter_ms
			* return second reading
			*/
		*timer_counter_value    = (uint64_t)  __local_timer_counter_value_2;
		*tick_counter_ms_value  =             __local_tick_counter_ms_2;
	}
	else if ((__local_timer_counter_value_2 == __local_timer_counter_value_1) && (__local_tick_counter_ms_2 != __local_tick_counter_ms_1))
	{
		/**
			* we should never reach this, but if we did reach this
			* then interrupt occured in any of reading but maybe cpu is too fast than timer
			* that we still read same value for a
			*
			* then just simply read again, because interrupt is already fired anyway
			*/
		*timer_counter_value    = (uint64_t)  timer_count_get(POS_TICK_TIMER_ADDRESS, POS_TICK_TIMER_ID);
		*tick_counter_ms_value  =             pos_tick_counter_ms;
	}
	else
	{
		/**
			* a == a*
			* b == b*
			* no interrupt occured in whole process, use any values
			*/
		*timer_counter_value    = (uint64_t)  __local_timer_counter_value_1;
		*tick_counter_ms_value  =             __local_tick_counter_ms_1;
	}

	return 0; /* return ok */
}

static uint64_t get_counter_ms_unsigned(void)
{
	uint64_t        __local_timer_counter;
	uint64_t        __local_tick_counter_ms;

	int retval;

	/* get counter value */
	retval = safe_pull_counter_value(&__local_tick_counter_ms, &__local_timer_counter);
	if (retval != 0)
	{
		/* return invalid value if error */
		return (uint64_t) (-1L);
	}

	/**
		* convert timer counter to ms
		* by multiplying it by granularity (to get us) then divide it by 1000 (to get ms)
		*/
	__local_timer_counter = __local_timer_counter * POS_TICK_TIMER_COUNTER_US_GRANULARITY / 1000UL;

	/* add with current counter ms */
	return __local_tick_counter_ms + __local_timer_counter;
}

static uint64_t get_counter_us_unsigned(void)
{
	uint64_t        __local_timer_counter;
	uint64_t        __local_tick_counter_ms;

	int retval;

	/* get counter value */
	retval = safe_pull_counter_value(&__local_tick_counter_ms, &__local_timer_counter);
	if (retval != 0)
	{
		/* return invalid value if error */
		return (uint64_t) (-1L);
	}

	/**
		* convert timer counter value to us
		* by multiplying it by granularity
		*/
	__local_timer_counter *= POS_TICK_TIMER_COUNTER_US_GRANULARITY;

	/* finally add with current counter */
	return (__local_tick_counter_ms * 1000UL) + __local_timer_counter;
}

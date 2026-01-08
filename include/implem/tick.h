/**
	* Copyright (C) 2025 ICDEC
	*/

#ifndef __POS_IMPLEM_TICK_H__
#define __POS_IMPLEM_TICK_H__

/**
	* definition
	*/

#ifndef LANGUAGE_ASSEMBLY

/**
	* tick address
	* only used by c function (timer_*)
	*/
#ifndef POS_TICK_TIMER_ADDRESS
#define POS_TICK_TIMER_ADDRESS     timer_base_fc(0, 0) /* need to include "pulp.h" */
#endif

#endif /* ! defined(LANGUAGE_ASSEMBLY) */

/**
	* timer to be used:
	* valid options are:
	* 0: use timer_lo
	* 1: use timer_hi
	*/
#ifndef POS_TICK_TIMER_ID
#define POS_TICK_TIMER_ID                 1
#endif

/**
	* use prescaler or not: if yes set to 1, if no, set to 0
	*/
#ifndef POS_TICK_USE_PRESCALER
#define POS_TICK_USE_PRESCALER            1
#endif

/**
	* prescaler value (value must be 8 bit: 0 - 255)
	* ignored if TICK_USE_PRESCALER == 0
	*/
#ifndef POS_TICK_PRESCALER_VALUE
#define POS_TICK_PRESCALER_VALUE          16 /* for 16 MHz clk, set 16 for prescaler to get 1 us per prescaler count */
#endif

/**
	* timer compare value 32 bit
	* this value should be used by TICK_MS_GRANULARITY to get real ms counter
	*/
#ifndef POS_TICK_TIMER_COMPARE_VALUE
#define POS_TICK_TIMER_COMPARE_VALUE   99999999 /* for 16 MHz clock, and prescaler 16, if we set cmp value to 100_000_000, we will get interupt every 100 s */
#endif

/**
	* safe margin for reading timer counter with non-atomic
	* way, beware that reading timer counter with atomic operation
	* cost suspending hardware timer, so this treshold is used to
	* avoid atomic read as much as possible, but if many interrupt call is present
	* and have higher priority than timer interrupt also considerable runtime
	* you have to decrease this safe atomic value
	*
	* bigger value (closer to TICK_TIMER_COMPARE_VALUE) is for better performance (lesser atomic operation)
	*   but may fail if other interrupt preempt timer interrupt and take considerable runtime
	* smaller value (closer to 0) is for better guaranteed that reading counter will be done in atomic way
	*   but timer performance will be degraded as counter read will likely pause timer
	*/
/*#ifndef TICK_TIMER_COMPARE_VALUE_SAFE_NON_ATOMIC_READ
#define TICK_TIMER_COMPARE_VALUE_SAFE_NON_ATOMIC_READ ((TICK_TIMER_COMPARE_VALUE) - 59)
#endif*/

/**
	* increment of ms counter in integer ms
	* (default is 1, which means counter will increment 1 for every timer interrupt)
	* global variable tick_counter_ms is always have floor value of ms since ticka started
	*/
#ifndef POS_TICK_MS_GRANULARITY
#define POS_TICK_MS_GRANULARITY                   100000
#endif

/**
	* increment of us for every increment of timer counter
	* (default is 1, which means every timer value increment of 1,
	* 1 us has been passed which happen if prescaler set to counting clock to 1 us)
	*
	* e.g. for counter clock 20 MHz, if prescaler set to 200, then counter will increment
	*      at rate 100 kHz. for every counter increment, 10 us will already be passed
	*      then this define should be set to 10
	*/
#ifndef POS_TICK_TIMER_COUNTER_US_GRANULARITY
#define POS_TICK_TIMER_COUNTER_US_GRANULARITY       1UL
#endif

/**
	* allowed tolerance difference of us and ms
	*/
/*#ifndef TICK_TIMER_COUNTER_TO_MS_MAX_TOLERANCE
#define TICK_TIMER_COUNTER_TO_MS_MAX_TOLERANCE       2
#endif*/

/**
	* calculation to get us from counter ms and timer counter
	*/
/*#ifndef TICK_MS_FROM_COUNTER_AND_TIMER
#define TICK_US_FROM_COUNTER_AND_TIMER(timer_counter) ((tick_counter_ms * 1000UL) + ((timer_counter) * (TICK_TIMER_COUNTER_US_GRANULARITY)))
#endif*/


/**
	* following part only valid for inclusion in c and c++
	*/
#ifndef LANGUAGE_ASSEMBLY

/* for 64-bit data definition */
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
	* exported global variable
	*/

/* counter ms (64-bit) */
extern uint64_t pos_tick_counter_ms;

/**
	* exported public function
	*/
void pos_tick_init(void);
void pos_tick_stop(void);

long pos_tick_get_counter_ms(void);

long pos_tick_get_counter_us(void);

/* this function is implemented in assembly (tick32.S) */
__attribute__ ((naked)) void pos_tick_1ms_interrupt_handler(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ! defined(LANGUAGE_ASSEMBLY) */
#endif /* ! defined(__POS_IMPLEM_TICK_H__) */

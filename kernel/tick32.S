/**
	* Copyright (C) 2025 ICDEC
	*/


/**
	* force to define LANGUAGE_ASSEMBLY even if it defined to something
	* define LANGUAGE_ASSEMBLY before any inclusion
	*/
#ifndef LANGUAGE_ASSEMBLY
#define LANGUAGE_ASSEMBLY
#endif

/* only need to include tick.h (for POS_TICK_TIMER_ID and POS_TICK_MS_GRANULARITY) */
#include "implem/tick.h"


/**
	* **************************************************
	* put following section in .data (read-write, data, non-executable)
	*/
.section   .data


/**
	* global variable tick_counter_ms
	*
	* NOTE: we assume .word is 32 bit (4 byte)
	*       and tick_counter_ms should be (unsigned) 64-bit
	*/
.global     pos_tick_counter_ms
pos_tick_counter_ms:
__tick_counter_ms_low_32:
.word	0
__tick_counter_ms_high_32:
.word	0





/**
	* **************************************************
	* put following section in .text (read-only, executable)
	*/
.section   .text


/**
	* interrupt handler for tick increment
	* this handler assume to be run in machine mode thus the usage of mret
	* this handler assume to be run in risc-v 32
	*
	* NOTE: this handler is made with more focus in readibility
	*       so optimization are welcome
	*/
.global     pos_tick_1ms_interrupt_handler
pos_tick_1ms_interrupt_handler:
/**
	* push stack to store 16 byte for s0 and a0, a1
	* (4 byte * (number of register to be pushed + 1))
	*/
add    sp, sp, -16

/* save register */
sw     s0, 12 (sp) /* save frame pointer */

sw     a0,  8 (sp) /* save general purpose register */
sw     a1,  4 (sp)

add    s0, sp, 16  /* set new frame pointer (for return address) */

/**
	* **************************************************
	* acknowledge interrupt
	* this is needed in pulpissimo soc,
	* since interrupt is controlled by apb_interrupt_ctrl
	* **************************************************
	*/
/* base address of apb interrupt ctrl */
li     a1, 0x1a109000

/* mask value (1 << 10) for timer lo, (1 << 11) for timer hi */
#if     ((POS_TICK_TIMER_ID) == 0)
li     a0, 0x400 /* 1 << 10 */
#elif   ((POS_TICK_TIMER_ID) == 1)
li     a0, 0x800 /* 1 << 11 */
#else
#error  POS_TICK_TIMER_ID should be defined with value 0 or 1 (0 is for enabling timer_lo as tick or 1 is for enabling timer_hi as tick)
#endif

/* write mask to ack set register (offset 0x1c) */
sw     a0,  0x1c (a1)


/**
	* **************************************************
	* increment (and test overflow) low part of tick_counter_ms
	* **************************************************
	*/

/* load value of low part of tick_counter_ms */
lw     a0, __tick_counter_ms_low_32

/* increment by POS_TICK_MS_GRANULARITY, and store the result in a1 */
li     a1, POS_TICK_MS_GRANULARITY
add    a1, a1, a0

/**
	* if a1 now is smaller than a0, then overflow is happened
	* in this (very rare) case,
	* increment __tick_counter_ms_high_32
	*/

/* jump to increment high part */
bltu   a1, a0, __increment_high_part

/**
	* if we reach here, then we enter common case
	* which is increment only low part
	* which is the value already saved in a1
	*/
la     a0,  __tick_counter_ms_low_32    /* use a0 to store address of counter low part */
sw     a1,    0 (a0)                    /* store a1 in address hold by a0 + 0 */

/**
	* **************************************************
	* restore register state from stack and restore stack then
	* return from interrupt
	* **************************************************
	*/
lw     s0, 12 (sp) /* restore frame pointer */

lw     a0,  8 (sp) /* restore general purpose register */
lw     a1,  4 (sp)

add    sp, sp, 16  /* restore stack pointer */
mret

/**
	* **************************************************
	* jump target for increment high part, for incrementing low part only
	* beqz above won't cause jump
	* **************************************************
	*/
__increment_high_part:

/**
	* save low part first, same sequence as if we don't branch
	* incrementation result is in a1, use a0 to hold address of __tick_counter_ms_low_32
	*
	* NOTE: we assume this interrupt process of tick increment don't have preemption problem
	*       even if interrupt preemption is enabled/possible, but we asume this whole interrupt process is
	*       finished before next interrupt arrive (since tick/timer interrupt is periodic)
	*       and tick_counter_ms only modified by this interrupt handler
	*/
la     a0,  __tick_counter_ms_low_32
sw     a1,    0 (a0) /* save low part (a1) to memory */

/**
	* load high part of counter
	*/
lw     a1,  __tick_counter_ms_high_32

/* increment by 1 (carry value is always 1) */
add    a1, a1, 1

/**
	* store the high part of result (offset 4 byte higher)
	* we don't need to do la a0, __tick_counter_ms_high_32
	* because sw can be done with offset
	* and a0 still hold __tick_counter_ms_low_32 address
	*/
sw     a1,    4 (a0)

/**
	* **************************************************
	* restore register state from stack and restore stack then
	* return from interrupt
	* **************************************************
	*/
lw     s0, 12 (sp)
lw     a0,  8 (sp)
lw     a1,  4 (sp)
add    sp, sp, 16
mret

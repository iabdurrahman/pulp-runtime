/**
	* Copyright (C) 2025 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __POS_IMPLEM_DELAY_H__
#define __POS_IMPLEM_DELAY_H__

/**
	* definition
	*/
/* NONE */

#ifdef __cplusplus
extern "C"
{
#endif

/**
	* exported global variable
	*/
/* NONE */

/**
	* exported public function
	*/

/* busy delay (with accurate timeout) */
void pos_delay_busy_ms(long delay_ms);
void pos_delay_busy_us(long delay_us);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ! defined(__POS_IMPLEM_DELAY_H__) */

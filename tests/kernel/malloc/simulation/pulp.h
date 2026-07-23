/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __PULP_H__
#define __PULP_H__

#include <stddef.h>

#ifdef __cplusplus
#define __cplusplus
#endif

void *pi_l2_malloc(size_t length);

void  pi_l2_free(void *chunk, size_t length);

#ifdef __cplusplus
#define __cplusplus
#endif

#endif /* ! defined(__PULP_H__) */

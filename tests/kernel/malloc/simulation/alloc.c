/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdint.h>
#include <stddef.h>

#include <pulp.h>

static uint8_t shared_storage[256];

void *pi_l2_malloc(size_t length)
{
	(void) length;
	return (void *) shared_storage;
}

void  pi_l2_free(void *chunk, size_t length)
{
	(void) chunk;
	(void) length;
}

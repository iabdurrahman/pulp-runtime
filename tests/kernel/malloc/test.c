/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdint.h> /* for fixed size integer (uint8_t, etc.) */
#include <stddef.h> /* for NULL definition */

#include <string.h> /* for memcpy and memcmp */

#ifndef __riscv__
#include <stdlib.h> /* abort() */
#endif

#include <pulp.h>

/**
	* don't inline this function
	* leverage compiler (gcc) to omit function epilogue (function must return void)
	*/
static void catch_error(void) __attribute__ ((noinline, noreturn));

#define CHUNK_SIZE    16

/* random value for evaluation */
#define TEST_MEMORY_CONTENT_0   0x9d /* 157 */
#define TEST_MEMORY_CONTENT_1   0x68 /* 104 */
#define TEST_MEMORY_CONTENT_2   0xac /* 172 */
#define TEST_MEMORY_CONTENT_3   0xe5 /* 229 */
#define TEST_MEMORY_CONTENT_4   0xdb /* 219 */
#define TEST_MEMORY_CONTENT_5   0xcf /* 207 */
#define TEST_MEMORY_CONTENT_6   0x51 /*  81 */
#define TEST_MEMORY_CONTENT_7   0xfc /* 252 */
#define TEST_MEMORY_CONTENT_8   0x33 /*  51 */
#define TEST_MEMORY_CONTENT_9   0x1f /*  31 */
#define TEST_MEMORY_CONTENT_10  0xa2 /* 162 */
#define TEST_MEMORY_CONTENT_11  0x7d /* 125 */
#define TEST_MEMORY_CONTENT_12  0x1b /*  27 */
#define TEST_MEMORY_CONTENT_13  0xab /* 171 */
#define TEST_MEMORY_CONTENT_14  0x74 /* 116 */
#define TEST_MEMORY_CONTENT_15  0x58 /*  88 */

/*#ifndef DONT_COMPARE_MEMORY_CONTENT
#define DONT_COMPARE_MEMORY_CONTENT 1
#endif*/

/*#ifndef DONT_COMPARE_MEMORY_LOCATION
#define DONT_COMPARE_MEMORY_LOCATION 0
#endif*/

/**
	* main test
	*/
int main(void)
{
	/* sample to be compared */
	const uint8_t sample[] = {
		TEST_MEMORY_CONTENT_0,
		TEST_MEMORY_CONTENT_1,
		TEST_MEMORY_CONTENT_2,
		TEST_MEMORY_CONTENT_3,
		TEST_MEMORY_CONTENT_4,
		TEST_MEMORY_CONTENT_5,
		TEST_MEMORY_CONTENT_6,
		TEST_MEMORY_CONTENT_7,
		TEST_MEMORY_CONTENT_8,
		TEST_MEMORY_CONTENT_9,
		TEST_MEMORY_CONTENT_10,
		TEST_MEMORY_CONTENT_11,
		TEST_MEMORY_CONTENT_12,
		TEST_MEMORY_CONTENT_13,
		TEST_MEMORY_CONTENT_14,
		TEST_MEMORY_CONTENT_15
	};

	void *ptr_a;
	void *ptr_b;

	/* allocate memory at l2 for ptr a */
	ptr_a = pi_l2_malloc(CHUNK_SIZE * sizeof(uint8_t));
	if (ptr_a == NULL)
	{
		catch_error(); /* error, cannot allocate chunk memory */
	}

	/* write a data to ptr_a */
	memcpy(ptr_a, sample, CHUNK_SIZE * sizeof(uint8_t));

	/**
		* free ptr_a
		* ptr_a passed as value, so we still keep previous allocated space address
		* DO NOT USE SPACE POINTED BY ADDRESS AFTER FREE'D
		*/
	pi_l2_free(ptr_a, CHUNK_SIZE * sizeof(uint8_t));


	/* reallocated memory at l2, and put it in ptr b */
	ptr_b = pi_l2_malloc(CHUNK_SIZE * sizeof(uint8_t));
	if (ptr_b == NULL)
	{
		catch_error(); /* catch_error */
	}

	/* check if reallocated memory is same */
#if ((! defined(DONT_COMPARE_MEMORY_CONTENT)) || (defined(DONT_COMPARE_MEMORY_CONTENT) && ((DONT_COMPARE_MEMORY_CONTENT) == 0)))
	if (ptr_a == ptr_b) /* this won't work in virtualized memory allocation environment */
	{
		int retval;
		retval = memcmp(ptr_b, sample, CHUNK_SIZE * sizeof(uint8_t));
		if (retval != 0)
		{
			catch_error();
		}
	}
#if ((! defined(DONT_COMPARE_MEMORY_LOCATION)) || (defined(DONT_COMPARE_MEMORY_LOCATION) && ((DONT_COMPARE_MEMORY_LOCATION) == 0)))
	else
	{
		/**
			* compare ptr_a and ptr_b by default (DONT_COMPARE_MEMORY_LOCATION not defined or defined but non-zero)
			*
			* if ptr_a != ptr_b then error
			*/
		catch_error();
	}
#endif
#endif

	/* free ptr_b */
	pi_l2_free(ptr_b, CHUNK_SIZE * sizeof(uint8_t));

	/* label for indicate process is finished */
	__asm__ volatile (
		".global     INTERRUPT_HERE\n" \
		"INTERRUPT_HERE:\n"
		:
		:
		:
	);

	return 0;
}

/* required by crt0.S */
void pe_start(void)
{
}

/* if error, go to here */
static void catch_error(void)
{
	/* global label to catch error */
	__asm__ volatile (
		".global     INTERRUPT_HERE_TO_GET_ERROR\n" \
		"INTERRUPT_HERE_TO_GET_ERROR:\n"
		:
		:
		:
	);

#ifdef __riscv__
	/* don't exit */
	while (1)
	{
	}
#else
	abort();
#endif
}

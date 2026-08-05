/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h> /* stddef is included by string.h */

#include <stdlib.h>
#include <stdio.h>

#include "pulp.h"

/**
	* **************************************************
	* private define
	*/


/* spi device index */
#ifndef SPI_ID
#define SPI_ID   0
#endif

#ifndef SPI_BAUDRATE
#define SPI_BAUDRATE   1000000
#endif

/* index of spi cs {0-3} */
#ifndef SPI_CS_N
#define SPI_CS_N    0
#endif


/* spi padmux config */

/* padmux configuration to spi cs */
#ifndef SPI_CS_N
	#error SPI_CS_N should be defined, the value is number 0-3 correspond to index of cs to be used
#else
	#if   ((SPI_CS_N) == 0)
		#define SPI_CS_PAD                  4 /* CSN0 */
		#define SPI_CS_PAD_MUX_VALUE        0
	#elif ((SPI_CS_N) == 1)
		/* use cs 1 is not supported */
		#error  SPI_CS_PAD and SPI_CS_PAD_MUX_VALUE cannot be defined for SPI_CS_N == 1 \
			due to hardware limitation, please choose another value SPI_CS_N {0-3} !1
	#elif ((SPI_CS_N) == 2)
		/* use cs 2 is not supported */
		#error  SPI_CS_PAD and SPI_CS_PAD_MUX_VALUE cannot be defined for SPI_CS_N == 2 \
			due to hardware limitation, please choose another value SPI_CS_N {0-3} !2

	#elif ((SPI_CS_N) == 3)
		/* use cs 3 is not supported */
		#error  SPI_CS_PAD and SPI_CS_PAD_MUX_VALUE cannot be defined for SPI_CS_N == 3 \
			due to hardware limitation, please choose another value SPI_CS_N {0-3} !3
	#else
		/* unknown number chosen for cs */
		#error  invalid SPI_CS_N value, SPI_CS_N value should be 0-3
	#endif
#endif /* defined(SPI_CS_N) */


/* padmux configuration for clk, mosi, and miso */
#define SPI_SCK_PAD         6
#define SPI_MOSI_IO0_PAD    0
#define SPI_MISO_IO1_PAD    1

#define SPI_SCK_PAD_MUX_VALUE       0
#define SPI_MOSI_IO0_PAD_MUX_VALUE  0
#define SPI_MISO_IO1_PAD_MUX_VALUE  0



/* report buffer */
#ifndef REPORT_SIZE
#define REPORT_SIZE    384
#endif

#if ((REPORT_SIZE) < 256)
	#error report size should not less than 1024 (1 kiB) \
		please set REPORT_SIZE to value greater or equal to 1024
#endif

/* line ending for debugging */
#ifndef LINEEND
#define LINEEND    "\r\n"
#endif

/* maximum number of parameter */
#ifndef JESD216_MAX_NUMBER_OF_PARAMETER
#define JESD216_MAX_NUMBER_OF_PARAMETER   2
#endif

/* parameter size in dword unit (uint32_t) */
#ifndef JESD216_PARAMETER_MAX_SIZE_DWORD
#define JESD216_PARAMETER_MAX_SIZE_DWORD   20
#endif

#if ((JESD216_PARAMETER_MAX_SIZE_DWORD) < 9)
	#error jesd216 define 1st standardized parameter must have length of 9; \
		please set JESD216_PARAMETER_MAX_SIZE_DWORD to bigger or equal to 9
#endif

/**
	* **************************************************
	* custom type declaration
	*/
struct jesd216_sfdp_header_s;
struct jesd216_parameter_header_s;
struct jesd216_parameter_s;

struct jesd216_sfdp_info_s;

/**
	* **************************************************
	* custom type definition
	*/
struct jesd216_sfdp_header_s
{
	uint32_t header_value[2];
};

struct jesd216_parameter_header_s
{
	uint32_t header_value[2];
};

struct jesd216_parameter_s
{
	uint32_t parameter_value[JESD216_PARAMETER_MAX_SIZE_DWORD];
};

/* sfdp information */
struct jesd216_sfdp_info_s
{
	struct jesd216_sfdp_header_s          sfdp_header;
	struct jesd216_parameter_header_s     parameter_header[JESD216_MAX_NUMBER_OF_PARAMETER];
	struct jesd216_parameter_s            parameter[JESD216_MAX_NUMBER_OF_PARAMETER];
};


/**
	* **************************************************
	* private function declaration
	*/

/**
	* write enable and disable
	*/

/**
	* sector access
	*/

/**
	* required by crt0.S
	*/
void pe_start(void);



/**
	* **************************************************
	* private variable declaration & definition
	*/

/**
	* **************************************************
	* private & public function definition
	*/

int main(void)
{
	spim_t        *spim;
	spim_conf_t    spim_conf;

	struct jesd216_sfdp_info_s info;

	uint8_t        buffer[JESD216_PARAMETER_MAX_SIZE_DWORD * sizeof(uint32_t)];
	uint8_t        cmd[JESD216_MAX_NUMBER_OF_PARAMETER * sizeof(uint32_t)];
	/*size_t         buffer_len;*/

	size_t         i, j;

	uint32_t       address;

	uint32_t       number_of_parameter_header;
	uint32_t       original_number_of_parameter_header;

	char           report[REPORT_SIZE];
	size_t         report_offset;

	/* set correct i/o pad function */
	hal_apb_soc_pad_set_function(SPI_CS_PAD, SPI_CS_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_SCK_PAD, SPI_SCK_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_MOSI_IO0_PAD, SPI_MOSI_IO0_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_MISO_IO1_PAD, SPI_MISO_IO1_PAD_MUX_VALUE);


	puts("Probing SPI NOR flash chip, by reading JESD216 SFDP register (0x5a)");

	/* initialize sfdp info structure */
	memset(&info, 0, sizeof(struct jesd216_sfdp_info_s));

	/**
		* set id number of parameter header to 0xff to indicate
		* that this parameter is not from jedec specification
		* (we won't read parameter that is not standardized)
		*/
	for (i = 0; i < JESD216_MAX_NUMBER_OF_PARAMETER; ++i)
	{
		info.parameter_header[i].header_value[0] |= 0x000000ff;
	}



	/* initialize cmd to all zero */
	memset(cmd, 0, JESD216_MAX_NUMBER_OF_PARAMETER * sizeof(uint32_t) * sizeof(uint8_t));


	/* initialize to flash not found */
	strcpy(report, "FLASH NOT FOUND\n"); /* it is ok to use strcpy here because input string has fixed length less than REPORT_SIZE */
	report_offset = strlen(report);


	/**
		* **************************************************
		* initialize spi configuration
		*/

	/*printf("Initializing spim object" LINE_END);*/
	puts("Initializing spim object");

	/* initialize config */
	spim_conf_init(&spim_conf);

	/* set necessary configuration */
	spim_conf.max_baudrate = SPI_BAUDRATE;
	spim_conf.id = SPI_ID;
	spim_conf.cs = SPI_CS_N;


	/**
		* **************************************************
		* initialize spi
		*/

	/*printf("Opening spim connection" LINE_END);*/
	puts("Opening spim connection");

	/* open spim device based on configuration */
	spim = spim_open(&spim_conf);
	if (spim == NULL)
	{
		return -1;
	}


	/**
		* **************************************************
		* read sfdp header
		* **************************************************
		*/

	/**
		* **************************************************
		* step 1: read SFDP header and check it's validity
		*
		* transaction is following (standardized by JESD216):
		* byte 0 : register address is at 0x5a
		* byte 1 : SFDP header address is at 0x00000000 [23:16]
		* byte 2 : SFDP header address is at 0x00000000 [15: 8]
		* byte 3 : SFDP header address is at 0x00000000 [7 : 0]
		* byte 4 : dummy
		* byte 5 : started to get continous rx (no warp around)
		*/

	address = 0x00; /* SFDP register */

	/* initialize command to read data */
	cmd[0] = 0x5a; /* read sfdp register */
	cmd[1] = (address >> 16) & 0xff;
	cmd[2] = (address >>  8) & 0xff;
	cmd[3] =  address        & 0xff;

	/* empty out rx */
	memset(buffer, -1, 2 * sizeof(uint32_t));

	/**
		* send command read sfdp register, keep cs low
		* use buffer for temporary storege, read value will be discarded
		*/
	spim_cmd_rx(spim, cmd, 4 * 8, 8, buffer, 2 * sizeof(uint32_t));
	pos_delay_busy_ms(1);

	/* reconstruct data */
	info.sfdp_header.header_value[0] = \
		((uint32_t)  ((uint8_t) (buffer[0])))        | \
		(((uint32_t) ((uint8_t) (buffer[1]))) <<  8) | \
		(((uint32_t) ((uint8_t) (buffer[2]))) << 16) | \
		(((uint32_t) ((uint8_t) (buffer[3]))) << 24);

	info.sfdp_header.header_value[1] = \
		((uint32_t)  ((uint8_t) (buffer[4])))        | \
		(((uint32_t) ((uint8_t) (buffer[5]))) <<  8) | \
		(((uint32_t) ((uint8_t) (buffer[6]))) << 16) | \
		(((uint32_t) ((uint8_t) (buffer[7]))) << 24);

	/* check if this flash respond to read sfdp header command */
	if ((info.sfdp_header.header_value[0] != 0x50444653) || \
		((info.sfdp_header.header_value[1] & 0xff000000) != 0xff000000) || \
		((info.sfdp_header.header_value[1] & 0x0000ff00)  < 0x00000100)) /* major revision should >= 1 */
	{
		/* spi flash not detected of spi flash not responding to sfdp */
		puts("error: sfdp header: invalid sfdp header constants or invalid major revision: 0");

		/* don't continue */
		goto do_exit;
	}

	printf("SFDP header detected with version: major %u, minor %u" LINEEND,
		(unsigned int) ((info.sfdp_header.header_value[1] & 0x0000ff00) >>  8),
		(unsigned int)  (info.sfdp_header.header_value[1] & 0x000000ff));


	/* get number of parameter header */
	number_of_parameter_header = (((size_t) (info.sfdp_header.header_value[1] & 0x00ff0000)) >> 16) + 1;

	strcpy(report, "FLASH FOUND\n"); /* it is ok to use unsafe strcpy here, because input string has fixed known length */
	report_offset = strlen(report);

	snprintf(report + report_offset, REPORT_SIZE - report_offset,
		"SFDP REVISION MAJOR: %u\nSFDP REVISION MINOR: %u\n",
		(unsigned int) ((info.sfdp_header.header_value[1] & 0x0000ff00) >>  8),
		(unsigned int)  (info.sfdp_header.header_value[1] & 0x000000ff));
	report_offset = strlen(report); /* snprintf is guarateed to add '\0' */

	/*assert(report_offset >= REPORT_SIZE)*/


	/**
		* **************************************************
		* step 2: loop read parameter header
		*
		* same as reading sfdp header but with address 0x08
		*/

	printf("reading %u SFDP parameter header" LINEEND,
		(unsigned int) number_of_parameter_header);

	address = 0x08;

	/* initialize command to read data */
	cmd[0] = 0x5a; /* read sfdp register */
	cmd[1] = (address >> 16) & 0xff;
	cmd[2] = (address >>  8) & 0xff;
	cmd[3] =  address        & 0xff;

	/**
		* prepare receive buffer, only read as much as we capable of
		* if number_of_parameter_header exceed JESD216_MAX_NUMBER_OF_PARAMETER,
		* silently clip it to JESD216_MAX_NUMBER_OF_PARAMETER
		*/

	/* save number_of_parameter_header to original_number_of_parameter_header */
	original_number_of_parameter_header = number_of_parameter_header;

	/* clip it if necessary */
	number_of_parameter_header = (number_of_parameter_header > JESD216_MAX_NUMBER_OF_PARAMETER) ?
		JESD216_MAX_NUMBER_OF_PARAMETER :
		number_of_parameter_header;

	memset(buffer, -1, number_of_parameter_header * 2 * sizeof(uint32_t));

	/**
		* send command read sfdp register
		* read from address 0x08 to read SFDP parameter header
		* we read all sfdp parameter header at once
		*/
	spim_cmd_rx(spim, cmd, 4 * 8, 8, buffer, number_of_parameter_header * 2 * sizeof(uint32_t));
	pos_delay_busy_ms(1);

	/* reconstruct info */
	for (i = 0; i < number_of_parameter_header; ++i)
	{
		j = i * 2 * sizeof(uint32_t); /* move every 8 byte */

		/* reconstruct info from received buffer */
		info.parameter_header[i].header_value[0] = \
			((uint32_t)  ((uint8_t) (buffer[j])))            | \
			(((uint32_t) ((uint8_t) (buffer[j + 1]))) <<  8) | \
			(((uint32_t) ((uint8_t) (buffer[j + 2]))) << 16) | \
			(((uint32_t) ((uint8_t) (buffer[j + 3]))) << 24);

		info.parameter_header[i].header_value[1] = \
			((uint32_t)  ((uint8_t) (buffer[j + 4])))        | \
			(((uint32_t) ((uint8_t) (buffer[j + 5]))) <<  8) | \
			(((uint32_t) ((uint8_t) (buffer[j + 6]))) << 16) | \
			(((uint32_t) ((uint8_t) (buffer[j + 7]))) << 24);


		/* check if parameter length is correct (>= 1) */
		if ((info.parameter_header[i].header_value[0] & 0xff000000) < 0x01000000)
		{
			/* error, parameter length is incorrect */

			/* don't continue */
			puts("error: parameter_header: length incorrect: 0");
			goto do_exit;
		}

		/* check parameter header */
		if ((info.parameter_header[i].header_value[0] & 0x000000ff) == 0)
		{
			/* this header is JEDEC HEADER */

			/* check if revision is correct */
			if ((info.parameter_header[i].header_value[0] & 0x0000ff00) < 0x00000100)
			{
				/* error, jedec major revision start from 1 */

				/* don't continue */
				puts("error: parameter_header: major revision incorrect: 0");
				goto do_exit;
			}
		}
		/* cannot handle non-JEDEC parameter header, so just save it, and silently forget it */


		/**
			* check parameter header 2
			*
			* msb must 0xff
			* address of parameter should more than parameter header
			*/

		if (( (info.parameter_header[i].header_value[1] & 0xff000000) != 0xff000000) || \
			( (info.parameter_header[i].header_value[1] & 0x00ffffff)  < ((original_number_of_parameter_header + 2) * 4)) || \
			(((info.parameter_header[i].header_value[1] & 0x00ffffff)  % 4) != 0))
		{
			/* error, MSB should 0xff, address should be more than header length, address should be 4 byte aligned */

			/* don't continue */
			puts("error: parameter_header: msb invalid or invalid address");
			goto do_exit;
		}
	}


	/**
		* **************************************************
		* step 3: loop read parameter
		*
		* reading with parameter length from parameter header
		*/

	printf("reading %u SFDP parameter" LINEEND,
		(unsigned int) number_of_parameter_header);

	for (i = 0; i < number_of_parameter_header; ++i)
	{
		/* parameter length is 1 based, 1 : 1 dword */
		size_t parameter_length = (info.parameter_header[i].header_value[0] & 0xff000000) >> 24;

		/* clip parameter length */
		if (parameter_length > JESD216_PARAMETER_MAX_SIZE_DWORD)
		{
			parameter_length = JESD216_PARAMETER_MAX_SIZE_DWORD;
		}

		/* get parameter address */
		address = info.parameter_header[i].header_value[1] & 0x00ffffff;

		/* initialize command to read data */
		cmd[0] = 0x5a; /* read sfdp register */
		cmd[1] = (address >> 16) & 0xff;
		cmd[2] = (address >>  8) & 0xff;
		cmd[3] =  address        & 0xff;

		/* prepare rx buffer */
		memset(buffer, -1, parameter_length	* sizeof(uint32_t));

		/* read parameter */
		spim_cmd_rx(spim, cmd, 4 * 8, 8, buffer, parameter_length * sizeof(uint32_t));
		pos_delay_busy_ms(1);

		/* collect parameter value */
		for (j = 0; j < parameter_length; ++j)
		{
			size_t k = j * sizeof(uint32_t); /* byte addressing of buffer, move every 4 byte */

			/* reconstruct data */
			info.parameter[i].parameter_value[j] = \
				((uint32_t)  ((uint8_t) (buffer[k])))            | \
				(((uint32_t) ((uint8_t) (buffer[k + 1]))) <<  8) | \
				(((uint32_t) ((uint8_t) (buffer[k + 2]))) << 16) | \
				(((uint32_t) ((uint8_t) (buffer[k + 3]))) << 24);
		}
	}


	/**
		* **************************************************
		* get flash size
		* in parameter 2
		*/

	if (((info.parameter_header[0].header_value[0] & 0x000000ff) == 0) &&
		((info.parameter_header[0].header_value[0] & 0xff000000) >= 0x09000000))
	{
		/* this parameter is part of jedec with at least 9 parameter */

		/* check parameter 2 */
		if ((info.parameter[0].parameter_value[1] & 0x80000000) == 0x80000000)
		{
			/* 4 Gibit or more */
			if (report_offset < REPORT_SIZE)
			{
				const char   s[] = "FLASH DENSITY BIT: >= 4 Gibit\n";
				const size_t s_len = strlen(s);

				memcpy(report + report_offset, s,
					((report_offset + s_len + 1) > REPORT_SIZE) ? REPORT_SIZE - report_offset : s_len + 1);
				report_offset += s_len;
			}
		}
		else
		{
			/* 2 Gibit or less */
			if (report_offset < REPORT_SIZE)
			{
				const unsigned long flash_size = ((unsigned long) (info.parameter[0].parameter_value[1] & 0x7fffffff)) + 1;

				snprintf(report + report_offset, REPORT_SIZE - report_offset,
					"FLASH DENSITY BIT: == %lu\n", flash_size);
				report_offset = strlen(report);
			}
		}
	}



do_exit:

	/* add end of report */
	if (report_offset < REPORT_SIZE)
	{
		const char   s[] = "END OF REPORT\n";
		const size_t s_len = strlen(s);

		memcpy(report + report_offset, s,
			((report_offset + s_len + 1) > REPORT_SIZE) ? REPORT_SIZE - report_offset : s_len + 1);
		report_offset += s_len;
	}




	/**
		* give debugger access here to get report
		*
		* DO NOT ADD SECTION DIRECTIVE HERE!
		*/
	__asm__ volatile (
		".global     INTERRUPT_HERE_TO_GET_REPORT\n" \
		"INTERRUPT_HERE_TO_GET_REPORT:\n"
		:
		:
		:
	);


	/* actually print the report to serial */
	if (report_offset < REPORT_SIZE)
	{
		report[report_offset] = '\0'; /* make sure we have zero termination */
		puts(report); /* send report to serial */
	}
	else
	{
		/**
			* cannot add '\0' to report buffer to terminate string
			* it is unsafe to send unterminated string to serial with
			* stdio's functions
			*/
		puts("cannot print report because " \
			"report_offset already match or more than REPORT_SIZE");
	}

	/**
		* **************************************************
		* close spi
		*/

	spim_close(spim);
	spim = NULL; /* prevent usage of spim */

	puts("SPI closed");

	puts("Done");

	return 0;
}

/**
	* needed by crt0.S
	*/
void pe_start(void)
{
}

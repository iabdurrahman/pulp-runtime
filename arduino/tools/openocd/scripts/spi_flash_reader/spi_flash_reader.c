/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/*#include <stdbool.h>*/
#include <stdint.h>
#include <string.h> /* stddef is included by string.h */

#include <stdlib.h>
#include <stdio.h>

#include <pulp.h>



/* spi padmux config */

#define SPI_CS_PAD          4 /* CSN0 */
#define SPI_SCK_PAD         6
#define SPI_MOSI_IO0_PAD    0
#define SPI_MISO_IO1_PAD    1

#define SPI_CS_PAD_MUX_VALUE        0
#define SPI_SCK_PAD_MUX_VALUE       0
#define SPI_MOSI_IO0_PAD_MUX_VALUE  0
#define SPI_MISO_IO1_PAD_MUX_VALUE  0

/**
	* start address to read spi flash
	* make sure the value is same with spi flash writer
	*/
#ifndef START_READ_ADDRESS
#define START_READ_ADDRESS    0x00000000
#endif

/**
	* line ending, for simulation should set to "\n"
	* for platform deployment, should set to "\r\n"
	*/
#ifndef LINE_END
#define LINE_END   "\r\n"
#endif

/**
	* check if flash size is defined, use default value 64 Mib if not defined
	*/
#ifndef SPI_FLASH_SIZE_BYTES
	#if (defined(SIMULATION) && ((SIMULATION) != 0))
		#warning  SPI_FLASH_SIZE_BYTES is not defined in simulation, \
			include "test/simulation/spi_flash_simulation_posix.h" \
			to define SPI_FLASH_SIZE_BYTES; if another flash size is desired, \
			please define SPI_FLASH_SIZE_BITS instead of SPI_FLASH_SIZE_BYTES \
			SPI_FLASH_SIZE_BYTES is calculated based on SPI_FLASH_SIZE_BITS value
	#endif
	#warning  SPI_FLASH_SIZE_BYTES is not defined, set default to 64 Mbit
	#define SPI_FLASH_SIZE_BYTES    ((64 * 1024 * 1024) / 8)
#endif

/**
	* check if sector size is changed by user
	* you may use different sector size for simulation
	* but the simulation result may not reflect with
	* actual hardware behavior
	*/
#ifdef SPI_FLASH_SECTOR_SIZE_BYTES
	#warning  SPI_FLASH_SECTOR_SIZE_BYTES is defined \
		(default sector size is 4 kiB if \
		SPI_FLASH_SECTOR_SIZE_BYTES is undefined), \
		except you know what your doing, \
		please undefine SPI_FLASH_SECTOR_SIZE_BYTES
#else
	#define SPI_FLASH_SECTOR_SIZE_BYTES    (4 * 1024)
#endif

/**
	* check if page size is changed by user
	* you may use different page size for simulation
	* but the simulation result may not reflect with
	* actual hardware behavior
	*/
#ifdef SPI_FLASH_PAGE_SIZE_BYTES
	#warning  SPI_FLASH_PAGE_SIZE_BYTES is defined \
		(default page size is 256 bytes if \
		SPI_FLASH_PAGE_SIZE_BYTES is undefined), \
		except you know what your doing, \
		please undefine SPI_FLASH_PAGE_SIZE_BYTES
#else
	#define SPI_FLASH_PAGE_SIZE_BYTES    256
#endif


/* check and make sure page size is smaller than sector size */
#if ((SPI_FLASH_SECTOR_SIZE_BYTES) < (SPI_FLASH_PAGE_SIZE_BYTES))
	#error  SPI_FLASH_SECTOR_SIZE_BYTES value should be bigger than \
		SPI_FLASH_PAGE_SIZE_BYTES
#elif (((SPI_FLASH_SECTOR_SIZE_BYTES) % (SPI_FLASH_PAGE_SIZE_BYTES)) != 0)
	#error  SPI_FLASH_SECTOR_SIZE_BYTES should be divisible by \
		SPI_FLASH_PAGE_SIZE_BYTES \
		(SPI_FLASH_SECTOR_SIZE_BYTES = SPI_FLASH_PAGE_SIZE_BYTES * N ; \
		where N is integer)
#endif


/**
	* check whether we start to write flash from sector start or not
	* if not, give warning
	*/
#if (((START_READ_ADDRESS) % (SPI_FLASH_SECTOR_SIZE_BYTES)) != 0)
	#error  this program only support read address start align with sector address \
		plese set START_READ_ADDRESS align with with SPI_FLASH_SECTOR_SIZE_BYTES \
		(SPI_FLASH_SECTOR_SIZE_BYTES * n, with n = {0, 1, 2, 3, ...})
	/*#warning  START_READ_ADDRESS not align with start of sector, \
		sector write will be done twice per session; \
		set START_READ_ADDRESS to any of sector start address \
		(SPI_FLASH_SECTOR_SIZE_BYTES * n, with n = {0, 1, 2, 3, ...})*/
#endif


/**
	* **************************************************
	* custom type declaration
	*/

/**
	* **************************************************
	* custom type definition
	*/

/**
	* **************************************************
	* private function declaration
	*/


/* read full sector */
static size_t  spi_nor_flash_read_sector(spim_t * restrict spim, uint32_t sector, void * restrict data);


/* write data to random address */
/*static size_t  spi_nor_flash_read_data(spim_t * restrict spim, uint32_t base_address, void * restrict data, size_t size);*/


/**
	* required by crt0.S
	*/
void pe_start(void);



/**
	* **************************************************
	* private variable declaration & definition
	*/

/* set this to bool data type will breaking gdb */
int            stop_read    __attribute__ ((section(".data")));

/**
	* **************************************************
	* private & public function definition
	*/

int main(void)
{
	spim_t         *spim;
	spim_conf_t     spim_conf;

	size_t          base_address = START_READ_ADDRESS;

	uint8_t         buffer[SPI_FLASH_SECTOR_SIZE_BYTES];
	size_t          buffer_len = SPI_FLASH_SECTOR_SIZE_BYTES; /* preset to sector size */

	/* set correct i/o pad function */
	hal_apb_soc_pad_set_function(SPI_CS_PAD, SPI_CS_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_SCK_PAD, SPI_SCK_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_MOSI_IO0_PAD, SPI_MOSI_IO0_PAD_MUX_VALUE);
	hal_apb_soc_pad_set_function(SPI_MISO_IO1_PAD, SPI_MISO_IO1_PAD_MUX_VALUE);


	/*printf("Reading External NOR Flash" LINE_END);*/
	puts("Reading External NOR Flash");


	/**
		* **************************************************
		* initialize spi configuration
		*/

	/*printf("Initializing spim object" LINE_END);*/
	puts("Initializing spim object");

	/* initialize config */
	spim_conf_init(&spim_conf);

	/* set necessary configuration */
	spim_conf.max_baudrate = 1000000;
	spim_conf.id = 0;
	spim_conf.cs = 0;


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

	/* loop read data */
	do
	{
		/* stop read if debugger not requested it */
		stop_read = 1;

		/* write data if debugger set length to non-zero */
		if (buffer_len > 0)
		{
			size_t read_length;

			printf("Reading data from external flash " \
				"at address: 0x%08lx, with length: %lu" LINE_END,
				(unsigned long) base_address, (unsigned long) /*buffer_len*/ SPI_FLASH_SECTOR_SIZE_BYTES);

			/**
				* read data from spi flash
				*/
			read_length = spi_nor_flash_read_sector(spim, (uint32_t) (base_address / SPI_FLASH_SECTOR_SIZE_BYTES), buffer);
			/*read_length = spi_nor_flash_read_data(spim, (uint32_t) base_address, buffer, buffer_len);*/

			/* move address */
			base_address += read_length;
		}



		/**
			* add label for breakpoint
			* NOTICE: when optimization -O3 is used, this loop will be duplicated
			*         thus appear twice in proper form before assembly
			*         as the result this function will have 2 assembly label
			*         (you can check the generated proper with -S switch in gcc)
			*         therefore, you should use other optimization strategy for this file
			*         currently -Os (optimize for size), -Og (optimize for size), -O0 (no optimization)
			*         will produce single label
			*         -O2, -O3 will produce duplicate
			*/
		__asm__ volatile (
			".global     INTERRUPT_HERE_TO_READ_DATA_FROM_FLASH\n" \
			"INTERRUPT_HERE_TO_READ_DATA_FROM_FLASH:\n"
			:
			:
			:
		);



		/**
			* keep looping until debugger not changing stop_read
			*/
	}
	while ((stop_read == 0) && (base_address < SPI_FLASH_SIZE_BYTES));

	/**
		* **************************************************
		* close spi
		*/

	spim_close(spim);
	spim = NULL; /* prevent usage of spim */

	puts("SPI closed");

	puts("Done");

	/* add label for breakpoint */
	__asm__ volatile (
		".global     INTERRUPT_HERE_TO_FINISH_PROGRAM\n" \
		"INTERRUPT_HERE_TO_FINISH_PROGRAM:\n"
		:
		:
		:
	);

	return 0;
}

/**
	* needed by crt0.S
	*/
void pe_start(void)
{
}

/**
	* read full sector
	* this function expect data buffer is as big as 1 sector
	* return number of data written
	*/
static size_t spi_nor_flash_read_sector(spim_t * restrict spim, uint32_t sector, void * restrict data)
{
	const uint32_t NUMBER_OF_SECTOR =
		SPI_FLASH_SIZE_BYTES / SPI_FLASH_SECTOR_SIZE_BYTES;

	const uint32_t base_address =
		sector * SPI_FLASH_SECTOR_SIZE_BYTES;

	/* allocate in page basis */
	uint8_t cmd[4];

	/* make sure we write correct sector */
	if (sector >= NUMBER_OF_SECTOR)
	{
		/* nothing to write */
		return 0;
	}

	/* empty out cmd */
	memset(cmd, 0, SPI_FLASH_PAGE_SIZE_BYTES * sizeof(uint8_t));

	/* initialize command to read data */
	cmd[0] = 0x03; /* read data */
	cmd[1] = (base_address >> 16) & 0xff;
	cmd[2] = (base_address >>  8) & 0xff;
	cmd[3] =  base_address        & 0xff;

	/* send command read, read whole sector */
	spim_cmd_rx(spim, cmd, 4 * 8, 0, data, SPI_FLASH_SECTOR_SIZE_BYTES);

	return SPI_FLASH_SECTOR_SIZE_BYTES;
}

#if 0
static size_t spi_nor_flash_read_data(spim_t * restrict spim, uint32_t base_address, void * restrict data, size_t size)
{
	/* allocate in page basis */
	uint8_t cmd[4];

	/* we should not write data again, use _data! */
	uint8_t * restrict _data = data;

	size_t number_of_data_received;

	/* make sure data is not NULL */
	if (_data == NULL)
	{
		return 0;
	}

	/* initialize command to read data */
	cmd[0] = 0x03; /* read data */

	/* read per page size */
	number_of_data_received = 0; /* set number_of_data_received to 0 */
	while ((size > 0) && (base_address < SPI_FLASH_SIZE_BYTES))
	{
		size_t read_size;

		/* check if number of read is more than page size */
		if (size > SPI_FLASH_PAGE_SIZE_BYTES)
		{
			read_size = SPI_FLASH_PAGE_SIZE_BYTES;
		}
		else
		{
			read_size = size;
		}

		/* check if number of read will pass flash size */
		if (base_address + read_size >= SPI_FLASH_SIZE_BYTES)
		{
			read_size = SPI_FLASH_PAGE_SIZE_BYTES - base_address;
		}

		/* otherwise, don't need to change read_size */


		/* set address, read command already set before loop */
		cmd[1] = (base_address >> 16) & 0xff;
		cmd[2] = (base_address >>  8) & 0xff;
		cmd[3] =  base_address        & 0xff;

		/* read data, at max 1 page at a time */
		spim_cmd_rx(spim, cmd, 4 * 8, 0, _data, read_size);

		/* move buffer */
		_data        += read_size;

		/* accumulate number of data received */
		number_of_data_received += read_size;

		/* rest of the buffer to fill */
		size         -= read_size;
		base_address += (uint32_t) read_size;

		/* loop until size is 0 */
	}

	return number_of_data_received;
}
#endif

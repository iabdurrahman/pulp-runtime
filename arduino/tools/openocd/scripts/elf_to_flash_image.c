/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/**
	* this program is alternative/simple replacement of
	* plp_mkflash python script from pulpissimo
	* this program will try to do very limited feature which are:
	* - it can read RISC-V RV32 ELF file
	*   note: current implementation only check machine listed in
	*         Executable and Linking Format (ELF) Specification Version 1.2,
	*         so theoritically this program will accept any machine ELF
	*         that not listed in the specification
	* - it will write new binary file that boot program from that ELF file
	*
	* pitfall (please update if you modify the program):
	* - this program only run in host machine with same endianness with risc-v
	*   (little endian) or target elf
	* - this program don't support merging multiple executable component
	* - this program don't support file system creation inside flash
	* - this program don't support encryption
	*/

#include <stdint.h>
#include <stddef.h> /* offsetof */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h> /* FOR DEBUGGING ONLY */

#include <errno.h>
#include <assert.h>


/**
	* flash sector size, all the segment will be saved in sector boudary
	*/
#ifndef SPI_FLASH_SECTOR_SIZE_BYTES
#define SPI_FLASH_SECTOR_SIZE_BYTES   (4 * 1024)  /* 4 kiB */
#endif

/**
	* fixed value of attribute bootaddr of class FlashImage
	* (pulp-sdk/tools/runner/python/runner/plp_flash_stimuli.py: around line 87)
	*/
#ifndef L2_BOOT_ADDRESS
#define L2_BOOT_ADDRESS     0x1c000000
#endif

/**
	* these definition are taken from
	* Tool Interface Standard (TIS)
	* Executable and Linking Format (ELF) Specification
	* Version 1.2
	*/

/**
	* ELF
	*/
#define ET_NONE            0   /* No file type */
#define ET_REL             1   /* Relocatable file */
#define ET_EXEC            2   /* Executable file */
#define ET_DYN             3   /* Shared object file */
#define ET_CORE            4   /* Core file */
#define ET_LOPROC     0xff00   /* Processor-specific */
#define ET_HIPROC     0xffff   /* Processor-specific */

#define ET_NONE            0   /* No machine */
#define EM_M32             1   /* AT&T WE 32100 */
#define EM_SPARC           2   /* SPARC */
#define EM_386             3   /* Intel Architecture */
#define EM_68K             4   /* Motorola 68000 */
#define EM_88K             5   /* Motorola 88000 */
#define EM_860             7   /* Intel 80860 */
#define EM_MIPS            8   /* MIPS RS3000 Big-Endian */
#define EM_MIPS_RS4_BE    10   /* MIPS RS4000 Big-Endian */
	/* 11-16 is Reserved for future use */

#define EV_NONE            0   /* Invalid version */
#define EV_CURRENT         1   /* Current version */

/* ELF e_ident index */
#define EI_MAG0            0   /* File identification */
#define EI_MAG1            1   /* File identification */
#define EI_MAG2            2   /* File identification */
#define EI_MAG3            3   /* File identification */
#define EI_CLASS           4   /* File class */
#define EI_DATA            5   /* Data encoding */
#define EI_VERSION         6   /* File version */
#define EI_PAD             7   /* Start of padding bytes */
#define EI_NIDENT         16   /* Size of e_ident[] */


/* ELF e_ident known constants */

#define ELFMAG0 ((uint8_t) 0x7f)  /* e_ident[EI_MAG0] */
#define ELFMAG1 ((uint8_t)  'E')  /* e_ident[EI_MAG1] */
#define ELFMAG2 ((uint8_t)  'L')  /* e_ident[EI_MAG2] */
#define ELFMAG3 ((uint8_t)  'F')  /* e_ident[EI_MAG3] */

#define ELFCLASSNONE       0   /* Invalid class */
#define ELFCLASS32         1   /* 32-bit objects */
#define ELFCLASS64         2   /* 64-bit objects */

#define ELFDATANONE        0   /* Invalid data encoding */
#define ELFDATA2LSB        1   /* See below */
#define ELFDATA2MSB        2   /* See below */



/**
	* ELF program header
	*/
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff


/* ELF loadable segment permission */
#define PF_X 1
#define PF_W 2
#define PF_R 4



/**
	* struct declaration only, definition will follow later
	*/

struct Elf32_Ehdr_s;

struct Elf32_Phdr_s;

struct pulpisimmo_elf_header_s;

struct pulpisimmo_elf_program_header_s;

struct pulpisimmo_comp_header_s;

/**
	* fixed size integer definition
	*/
typedef  int32_t Elf32_Sword;
typedef uint32_t Elf32_Word;

typedef uint16_t Elf32_Half;

typedef uint32_t Elf32_Addr;

typedef uint32_t Elf32_Off;



/* ELF Header */
__attribute__ ((packed))
struct Elf32_Ehdr_s
{
	uint8_t       e_ident[EI_NIDENT];
	Elf32_Half    e_type;
	Elf32_Half    e_machine;
	Elf32_Word    e_version;
	Elf32_Addr    e_entry;
	Elf32_Off     e_phoff;
	Elf32_Off     e_shoff;
	Elf32_Word    e_flags;
	Elf32_Half    e_ehsize;
	Elf32_Half    e_phentsize;
	Elf32_Half    e_phnum;
	Elf32_Half    e_shentsize;
	Elf32_Half    e_shnum;
	Elf32_Half    e_shstrndx;
};

/* ELF Program Header */
__attribute__ ((packed))
struct Elf32_Phdr_s
{
	Elf32_Word    p_type;
	Elf32_Off     p_offset;
	Elf32_Addr    p_vaddr;
	Elf32_Addr    p_paddr;
	Elf32_Word    p_filesz;
	Elf32_Word    p_memsz;
	Elf32_Word    p_flags;
	Elf32_Word    p_align;
};


typedef  struct Elf32_Ehdr_s  Elf32_Ehdr;

typedef  struct Elf32_Phdr_s  Elf32_Phdr;


/**
	* this struct should be defined first before pulpisimmo_elf_header_s definition
	*
	* this struct definition is derived from:
	* pulp-sdk/tools/runner/python/runner/plp_flash_stimuli.py
	* at method: __dumpFlashHeader_v2 (around line 305)
	*
	* BinarySegment has member data, but it not used in header
	*/
__attribute__ ((packed))
struct pulpisimmo_elf_program_header_s
{
	uint32_t   offset; /* offset in spi flash (value of flashOffset) the value should be absolute address that align with sector */
	uint32_t   base_address; /* ELFFile(elf).segments[n]["p_paddr"] value */
	uint32_t   segment_size; /* len(ELFFile(elf).segments[n].data) */
	uint32_t   number_of_flash_sector; /* self.bootBinary.segments[n].nbBlocks ((segment_size + sector_size - 1) / sector_size) */
};


/**
	* this struct definition is derived from:
	* pulp-sdk/tools/runner/python/runner/plp_flash_stimuli.py
	* at method: __dumpFlashHeader_v2 (around line 298)
	*
	* we need to add crc at the end of header, but because of vla
	* we can't really do it
	*/
__attribute__ ((packed))
struct pulpisimmo_elf_header_s
{
	uint32_t elf_size; /* size of all program, header + segment data; get from flash offset (round-up, divisible by 8) */
	uint32_t number_of_program_header; /* len(self.bootBinary.segments) */
	uint32_t entry; /* ELFFile(elf)["e_entry"] */
	uint32_t boot_address; /* constant to 0x1c000000 */
	struct pulpisimmo_elf_program_header_s program_header[]; /* VLA of program_header */
};

/* additional component (we can only handle 1 component) */
__attribute__ ((packed))
struct pulpisimmo_comp_header_s
{
	uint64_t header_size;       /* constant 12 */
	uint32_t number_of_component; /* 0 for no component */
	uint32_t __reserved_padding_0; /* used as replacement for sizeof() operator */
};


static void get_riscv_elf_header_from_buffer_le(Elf32_Ehdr * restrict header,
	const uint8_t * restrict buffer);

#if 0
static void get_riscv_elf_header_from_buffer_be(Elf32_Ehdr * restrict header,
	const uint8_t * restrict buffer);

static uint16_t littleendian_to_bigendian_16(const uint8_t value_le[2]);

static uint32_t littleendian_to_bigendian_32(const uint8_t value_le[4]);
#endif


static Elf32_Phdr * get_riscv_elf_program_header_from_buffer_le(FILE *fd, size_t file_offset, size_t length);
/*static Elf32_Phdr * get_riscv_elf_program_header_from_buffer_be(FILE *fd, size_t file_offset, size_t length);*/


static int riscv_elf_sanitize(const Elf32_Ehdr *header);

static size_t generate_pulpissimo_elf_header_in_buffer(
	const Elf32_Ehdr * restrict elf32_header,
	const Elf32_Phdr * restrict elf32_program_header,
	void * restrict buffer, size_t buffer_len);

/*static void get_pulpissimo_elf_header_le(struct pulpisimmo_elf_header_s * restrict header,
	const uint8_t * restrict buffer);*/

static uint32_t get_crc(const void *buffer, size_t buffer_len);


/* FOR DEBUGGING ONLY */
static void hex_dump(size_t offset, const void * __restrict buffer, size_t buffer_len,
	FILE * __restrict stream);


/**
	* we use this single main function to read executable file with standard elf format
	* and then generate bitstream/binary file that can be directly copied to spi nor flash
	*
	* spi nor device is byte-addressable non-volatile data storage, so random access at any
	* address are possible (both read and write)
	* but in order to write data, a specific area in that write address need to be ereased first
	* and smallest area erase that can be done is not a byte but a sector which typically 4 kiB
	* so, to write data, a sector need to read first to memory, modified in memory then written
	* (per page/256 byte) to erased sector
	* you can see nor flash storage as big collection of small fixed address, fixed sized sector
	* to picture this, lets say we want to access sector 3,
	* sector 3 will cover address (4 kiB * 3) until (4 kiB * 4 - 1)
	* so you can see sector number like memory address
	*
	* pulpissimo elf is organized per flash sector
	* (although this executable will generate file, the file content should be seen as
	* transparent nor flash content; e.g. 4th byte in file equal to flash content (byte) at address 4)
	* 1st sector is allocated for elf header
	* 2nd sector and later allocated for program/loadable segment
	* every segment will be accupy at least 1 sector, and padded with 0 until
	* it's size divisible by sector size (4 kiB)
	* header consist of elf header, then directly followed by segment header
	* after that padding is added (as consequence of multiplication by 4) then
	* crc is added
	*
	* each loadable segment from original elf will be rounded to sector size will be added sequantially
	* segement header should accurately record the where is the segment and
	* the size of segment (non rounded-up)
	*/
int main(int argc, char const *argv[])
{

	const char *elf32_file_path;
	const char *pulpisimmo_elf_file_path;

	uint8_t        elf32_header_stream_buffer[sizeof(Elf32_Ehdr)];
	Elf32_Ehdr     elf32_header;
	Elf32_Phdr    *elf32_program_header;

	/* pulpissimo elf header */
	struct pulpisimmo_elf_header_s  *pulpisimmo_elf_header;
	uint8_t                          pulpisimmo_elf_header_buffer[SPI_FLASH_SECTOR_SIZE_BYTES];


	struct pulpisimmo_comp_header_s  *pulpisimmo_comp_header;
	/*uint8_t                           pulpisimmo_comp_header_buffer[sizeof(struct pulpisimmo_comp_header_s)];*/
	uint8_t                           pulpisimmo_comp_header_buffer[offsetof(struct pulpisimmo_comp_header_s, __reserved_padding_0)];


	FILE   *elf32_fd;
	FILE   *pulpisimmo_elf_fd;

	size_t i, j;

	size_t next_segment;

	int _errno;
	int  retval;

	size_t retval2;

	/* check how many argument are given */
	if (argc < 3)
	{
		/* given argument is less than needed */
		fputs("need more argumen\n", stderr);
		fprintf(stderr, "call with: \"%s\" <input_elf_file> <output_flash_file>\n", argv[0]);

		/* don't continue */
		return (-1);
	}

	/* elf file path is in argument 1, pulpissimo elf is in argument 2 */
	elf32_file_path = argv[1];
	pulpisimmo_elf_file_path = argv[2];

	/* open file from argumen 1 */
	errno = 0;
	elf32_fd = fopen(elf32_file_path, "r");
	_errno = errno;

	/* check if opening elf file is failed? */
	if (elf32_fd == NULL)
	{
		const char *s;

		/* if error, e.g. permission denied, errno should not zero */
		assert(_errno != 0);

		s = strerror(_errno);
		if (s == NULL)
		{
			fprintf(stderr, "fopen(\"%s\") error: %d, " \
				"but strerror cannot stringify the error\n",
				elf32_file_path, _errno);
		}
		else
		{
			fputs(s, stderr);
			fputc('\n', stderr);
		}

		/* return error, don't continue */
		return (-3);
	}

	/* errno should 0 when fopen success */
	assert(_errno == 0);

	/* read elf header */
	retval2 = fread(elf32_header_stream_buffer, sizeof(Elf32_Ehdr), 1, elf32_fd);
	if (retval2 < 1) /* retval2 < sizeof(Elf32_Ehdr) */
	{
		/**
			* whatever happen, if retval2 less than struct size then
			* assume this file is not elf file
			*/
		fprintf(stderr, "cannot read complete ELF32 Header from file \"%s\"; " \
			"is file truncated? ot it is not an ELF file?\n", elf32_file_path);

		/* close stream, then exit */
		errno = 0;
		retval = fclose(elf32_fd);
		_errno = errno;

		/* fclose should success */
		assert((retval == 0) && (_errno == 0));

		return (-2);
	}

	/**
		* get elf header from buffer with correct endianness
		* please make sure to use correct endianness with running machine
		*/
	get_riscv_elf_header_from_buffer_le(&elf32_header, elf32_header_stream_buffer);
	/*get_riscv_elf_header_from_buffer_be(&elf32_header, elf32_header_stream_buffer);*/

	/* check if we indeed have risc-v elf */
	retval = riscv_elf_sanitize(&elf32_header);
	if (retval != 0)
	{
		/* error if sanitize failed */

		/* close stream */
		errno = 0;
		retval = fclose(elf32_fd);
		_errno = errno;

		/* fclose should success */
		assert((retval == 0) && (_errno == 0));

		return (-2);
	}

	/* report that elf file is sanitized */
	fputs("ELF file is sanitized\n", stderr);

	/* display entry */
	fprintf(stderr, "(ELF) program entry (e_entry): 0x%08x\n", elf32_header.e_entry);



	/**
		* get program header
		*/
	elf32_program_header = get_riscv_elf_program_header_from_buffer_le(elf32_fd, elf32_header.e_phoff, elf32_header.e_phnum);
	/*elf32_program_header = get_riscv_elf_program_header_from_buffer_be(elf32_fd, elf32_header.e_phoff, elf32_header.e_phnum);*/
	if (elf32_program_header == NULL)
	{
		/* close file */
		errno = 0;
		retval = fclose(elf32_fd);
		_errno = errno;

		/* fclose should success */
		assert((retval == 0) && (_errno == 0));

		return (-2);
	}


	/**
		* print program header
		*/
	for (i = 0, next_segment = 0; i < elf32_header.e_phnum; ++i)
	{
		if (elf32_program_header[i].p_type != PT_LOAD)
		{
			fprintf(stderr, "(ELF: program header) program header %u is not PT_LOAD\n", (unsigned int) i);
		}
		else
		{
			char flag[4];

			/* check if file size and memory load size is same */
			if (elf32_program_header[i].p_filesz != elf32_program_header[i].p_memsz)
			{
				fprintf(stderr, "(ELF: program header) PT_LOAD (%u) filesz != memsz (%u != %u); " \
					"is this .bss or loader-specific non-initialized data segment?\n", (unsigned int) i,
					elf32_program_header[i].p_filesz, elf32_program_header[i].p_memsz);

				if (elf32_program_header[i].p_filesz > elf32_program_header[i].p_memsz)
				{
					/* invalid */
					fputs("invalid filesz and memsz\n", stderr);

					free(elf32_program_header);

					errno = 0;
					retval = fclose(elf32_fd);
					_errno = errno;
					assert((retval == 0) && (_errno == 0));

					return (-2);
				}
			}

			/* check if current offset overlap with previous segment */
			if (next_segment > elf32_program_header[i].p_offset)
			{
				fputs("CURRENT SEGMENT FILESZ OVERLAP WITH PREVIOUS SEGMENT\n", stderr);

				free(elf32_program_header);

				errno = 0;
				retval = fclose(elf32_fd);
				_errno = errno;
				assert((retval == 0) && (_errno == 0));

				return (-2);
			}

			/* flag */
			flag[0] = ((elf32_program_header[i].p_flags & PF_R) == PF_R) ? 'R' : ' ';
			flag[1] = ((elf32_program_header[i].p_flags & PF_W) == PF_W) ? 'W' : ' ';
			flag[2] = ((elf32_program_header[i].p_flags & PF_X) == PF_X) ? 'X' : ' ';
			flag[3] = '\0';

			fprintf(stderr, "(ELF: program header) PT_LOAD (%u) %s  POS: 0x%08x (%6u) | VIMEM: 0x%08x | PHMEM: 0x%08x\n", (unsigned int) i,
				flag, elf32_program_header[i].p_offset, elf32_program_header[i].p_filesz, elf32_program_header[i].p_vaddr, elf32_program_header[i].p_paddr);
			fprintf(stderr, "(ELF: program header) PT_LOAD (%u) alignment: %6u (0x%08x)\n", (unsigned int) i,
				elf32_program_header[i].p_align, elf32_program_header[i].p_align);

			/* calculate boundary for next segment, next segment start should be exactly of after this value */
			next_segment = elf32_program_header[i].p_offset + elf32_program_header[i].p_filesz;
		}
	}



	/**
		* to make output binary, plp_mkflash will call method generate from class FlashImage
		* in generate method, it will call __dumpToBuff that finally call 2 method to generate data:
		* 1. __dumpBootBinaryToBuff() : __dumpFlashHeader_v2()
		* 2. __dumpCompsToBuff()
		*
		* then generated byte stream self.buff will written to file
		*
		* in this code, instead collecting and processing all data from source elf file, then dump it all
		* this code will read and write at same time
		*/

	/**
		* **************************************************
		* 1st phase:
		* emulate __dumpFlashHeader_v2()
		*/

	/* make pulpissimo elf header */
	retval2 = generate_pulpissimo_elf_header_in_buffer(
		&elf32_header, elf32_program_header, pulpisimmo_elf_header_buffer, SPI_FLASH_SECTOR_SIZE_BYTES);
	if (retval2 == 0)
	{
		/* failed to generate header */
		free(elf32_program_header);

		errno = 0;
		retval = fclose(elf32_fd);
		_errno = errno;
		assert((retval == 0) && (_errno == 0));

		return (-2);
	}

	/* in here, we assume 1 sector size is enough to pulpisimmo elf header */
	assert((retval2 > 0) && (retval2 <= SPI_FLASH_SECTOR_SIZE_BYTES));



	fputc('\n', stderr);
	fputs("generate pulpissimo elf file/spi flash image...\n", stderr);



	/**
		* DEBUG!!!
		* print pulpisimmo elf header
		*/
	fputs("\n          PULPISSIMO ELF HEADER\n", stderr);
	hex_dump(0, pulpisimmo_elf_header_buffer, retval2, stderr);
	fputc('\n', stderr);

	/* pad with 0xff */
	memset(&pulpisimmo_elf_header_buffer[retval2], -1, SPI_FLASH_SECTOR_SIZE_BYTES - retval2);

	/**
		* set pulpisimmo_elf_header as alias to buffer
		* so we can directly access struct rather than calculate offset ourself
		* NOTE: for host machine with big endian byte order, please reorder first before write to file
		*/
	pulpisimmo_elf_header = (struct pulpisimmo_elf_header_s *) pulpisimmo_elf_header_buffer;




	/* write to file output */
	errno = 0;
	pulpisimmo_elf_fd = fopen(pulpisimmo_elf_file_path, "w");
	_errno = errno;

	/* check if opening elf file is failed? */
	if (pulpisimmo_elf_fd == NULL)
	{
		const char *s;

		/* if error, e.g. permission denied, errno should not zero */
		assert(_errno != 0);

		s = strerror(_errno);
		if (s == NULL)
		{
			fprintf(stderr, "fopen(\"%s\") error: %d, " \
				"but strerror cannot stringify the error\n",
				pulpisimmo_elf_file_path, _errno);
		}
		else
		{
			fputs(s, stderr);
			fputc('\n', stderr);
		}

		free(elf32_program_header);

		errno = 0;
		fclose(pulpisimmo_elf_fd);
		_errno = errno;

		assert((retval == 0) && (_errno == 0));

		/* return error, don't continue */
		return (-3);
	}

	/* errno should 0 when fopen success */
	assert(_errno == 0);


	/* dump header to file */
	retval2 = fwrite(pulpisimmo_elf_header_buffer, sizeof(uint8_t), SPI_FLASH_SECTOR_SIZE_BYTES, pulpisimmo_elf_fd);
	if (retval2 != SPI_FLASH_SECTOR_SIZE_BYTES)
	{
		/* fail to write, no space available? */
		fprintf(stderr, "cannot write elf header to file \"%s\"; " \
			"is no space left in your disk space?\n", pulpisimmo_elf_file_path);

		free(elf32_program_header);

		/* close stream, then exit */
		errno = 0;
		retval = fclose(elf32_fd);
		_errno = errno;

		errno = 0;
		retval = fclose(pulpisimmo_elf_fd);
		_errno = errno;

		/* fclose should success */
		assert((retval == 0) && (_errno == 0));

		return (-2);
	}

	/* write rest of segment */
	for (i = 0, j = 0; i < elf32_header.e_phnum; ++i)
	{
		if (elf32_program_header[i].p_type == PT_LOAD)
		{
			unsigned long seek_position = elf32_program_header[i].p_offset;

			size_t   segment_size = elf32_program_header[i].p_filesz;
			uint8_t  segment[SPI_FLASH_SECTOR_SIZE_BYTES];

			/* make sure this segment match with pulpisimmo_elf */
			assert(elf32_program_header[i].p_paddr == pulpisimmo_elf_header->program_header[j].base_address);

			/* seek elf32 file */
			errno = 0;
			retval = fseek(elf32_fd, (long) seek_position, SEEK_SET);
			_errno = errno;
			if (retval == (-1))
			{
				const char *s;

				assert(_errno != 0); /* errno should not 0 */

				/* get error message */
				s = strerror(_errno);

				/* print error message */
				if (s == NULL)
				{
					fprintf(stderr, "fseek to program segment at address 0x%08lx (%lu)\n", \
						seek_position, seek_position);
				}
				else
				{
					fputs(s, stderr);
					fputc('\n', stderr);
				}

				free(elf32_program_header);

				errno = 0;
				retval = fclose(elf32_fd);
				_errno = errno;

				assert((retval == 0) && (_errno == 0));

				errno = 0;
				retval = fclose(pulpisimmo_elf_fd);
				_errno = errno;

				assert((retval == 0) && (_errno == 0));

				return (-2);
			}

			assert((retval == 0) && (_errno == 0));

			/* copy data segment from elf input to pulpissimo output */
			do
			{
				size_t segment_size_to_copy;
				size_t padding;

				if (segment_size > SPI_FLASH_SECTOR_SIZE_BYTES)
				{
					padding = 0;

					segment_size_to_copy = SPI_FLASH_SECTOR_SIZE_BYTES;
					segment_size -= SPI_FLASH_SECTOR_SIZE_BYTES;
				}
				else
				{
					padding = SPI_FLASH_SECTOR_SIZE_BYTES - segment_size;

					segment_size_to_copy = segment_size;
					segment_size = 0;
				}

				/* read segment */
				retval2 = fread(segment, sizeof(uint8_t), segment_size_to_copy, elf32_fd);
				if (retval2 != segment_size_to_copy)
				{
					fprintf(stderr, "fread segment with length %u failed; elf file truncated?\n", \
						(unsigned int) segment_size_to_copy);

					free(elf32_program_header);

					errno = 0;
					retval = fclose(elf32_fd);
					_errno = errno;

					assert((retval == 0) && (_errno == 0));

					errno = 0;
					retval = fclose(pulpisimmo_elf_fd);
					_errno = errno;

					assert((retval == 0) && (_errno == 0));

					return (-2);
				}

				/* write segment */
				retval2 = fwrite(segment, sizeof(uint8_t), segment_size_to_copy, pulpisimmo_elf_fd);
				if (retval2 != segment_size_to_copy)
				{
					fprintf(stderr, "fwrite segment with length %u failed; no space left?\n", \
						(unsigned int) segment_size_to_copy);

					free(elf32_program_header);

					errno = 0;
					retval = fclose(elf32_fd);
					_errno = errno;

					assert((retval == 0) && (_errno == 0));

					errno = 0;
					retval = fclose(pulpisimmo_elf_fd);
					_errno = errno;

					assert((retval == 0) && (_errno == 0));

					return (-2);
				}

				/* do we need padding */
				if (padding > 0)
				{
					assert(padding < SPI_FLASH_SECTOR_SIZE_BYTES);

					/* pad with 0xff */
					memset(segment, -1, sizeof(uint8_t) * padding);

					/* reuse segment buffer for padding */
					retval2 = fwrite(segment, sizeof(uint8_t), padding, pulpisimmo_elf_fd);
					if (retval2 != padding)
					{
						fprintf(stderr, "fwrite segment padding with length %u failed; no space left?\n", \
							(unsigned int) padding);

						free(elf32_program_header);

						errno = 0;
						retval = fclose(elf32_fd);
						_errno = errno;

						assert((retval == 0) && (_errno == 0));

						errno = 0;
						retval = fclose(pulpisimmo_elf_fd);
						_errno = errno;

						assert((retval == 0) && (_errno == 0));

						return (-2);
					}
				}
			}
			while (segment_size > 0);

			/* increment indexing for pulpisimmo_elf_header */
			j++;
		}
	}

	/* make sure number of segment in elf32 match with segment in pulpissimo elf */
	assert(j == ((size_t) (pulpisimmo_elf_header->number_of_program_header)));

	/**
		* we don't need source elf file anymore
		* close source file resource (file descriptor, allocated program header memory)
		*/

	/* free program_header */
	free(elf32_program_header);

	/* close file */
	errno = 0;
	retval = fclose(elf32_fd);
	_errno = errno;

	/* fclose should success */
	assert((retval == 0) && (_errno == 0));





	/**
		* **************************************************
		* 2nd phase:
		* emulate __dumpCompsToBuff()
		*/

	pulpisimmo_comp_header = (struct pulpisimmo_comp_header_s *) pulpisimmo_comp_header_buffer;

	/**
		* set buffer and number of component
		* FIXME: this is only work if host machine has same endianess with RISC-V (little endian)
		*/
	/*pulpisimmo_comp_header->header_size = sizeof(struct pulpisimmo_comp_header_s);*/
	pulpisimmo_comp_header->header_size = offsetof(struct pulpisimmo_comp_header_s, __reserved_padding_0);
	pulpisimmo_comp_header->number_of_component = 0; /* fixed 0 component */
	/* do not need to set __reserved_padding_0 attribute */


	/**
		* DEBUG!!!
		* print pulpisimmo comp header
		*/
	/*fputs("\n          PULPISSIMO COMP HEADER\n", stderr);*/
	fputs("          PULPISSIMO COMP HEADER\n", stderr);
	hex_dump(0, pulpisimmo_comp_header_buffer, offsetof(struct pulpisimmo_comp_header_s, __reserved_padding_0), stderr);
	fputc('\n', stderr);



	/* write to output file */
	/* dump header to file */
	/*retval2 = fwrite(pulpisimmo_comp_header_buffer, sizeof(uint8_t), sizeof(struct pulpisimmo_comp_header_s), pulpisimmo_elf_fd);*/
	retval2 = fwrite(pulpisimmo_comp_header_buffer, sizeof(uint8_t), offsetof(struct pulpisimmo_comp_header_s, __reserved_padding_0), pulpisimmo_elf_fd);
	/*if (retval2 != sizeof(struct pulpisimmo_comp_header_s))*/
	if (retval2 != offsetof(struct pulpisimmo_comp_header_s, __reserved_padding_0))
	{
		/* fail to write, no space available? */
		fprintf(stderr, "cannot write comp header to file \"%s\"; " \
			"is no space left in your disk space?\n", pulpisimmo_elf_file_path);

		/* close output file */
		errno = 0;
		retval = fclose(pulpisimmo_elf_fd);
		_errno = errno;

		/* fclose should success */
		assert((retval == 0) && (_errno == 0));

		return (-2);
	}




	/**
		* process finished!
		* do clean up
		*/
	fputs("done\n", stderr);




	/* close file */
	errno = 0;
	retval = fclose(pulpisimmo_elf_fd);
	_errno = errno;

	assert((retval == 0) && (_errno == 0));

	return 0;
}


static void get_riscv_elf_header_from_buffer_le(Elf32_Ehdr * restrict header,
	const uint8_t * restrict buffer)
{
	/* for little endian host machine (x86, x86_64, ARM, ARM64, RISC-V), just do memcpy */
	memcpy(header, buffer, sizeof(Elf32_Ehdr));
}

#if 0
static void get_riscv_elf_header_from_buffer_be(Elf32_Ehdr * restrict header,
	const uint8_t * restrict buffer)
{
	/* for big endian host machine (Power PC), rotate byte */

	/* don't need to reverse e_ident */
	memcpy(header->e_ident, buffer, EI_NIDENT * sizeof(uint8_t));

	header->e_type      = littleendian_to_bigendian_16(&(buffer[offsetof(Elf32_Ehdr, e_type)      / sizeof(uint8_t)]));
	header->e_machine   = littleendian_to_bigendian_16(&(buffer[offsetof(Elf32_Ehdr, e_machine)   / sizeof(uint8_t)]));
	header->e_version   = littleendian_to_bigendian_32(&(buffer[offsetof(Elf32_Ehdr, e_version)   / sizeof(uint8_t)]));
	header->e_entry     = littleendian_to_bigendian_32(&(buffer[offsetof(Elf32_Ehdr, e_entry)     / sizeof(uint8_t)]));
	header->e_phoff     = littleendian_to_bigendian_32(&(buffer[offsetof(Elf32_Ehdr, e_phoff)     / sizeof(uint8_t)]));
	header->e_shoff     = littleendian_to_bigendian_32(&(buffer[offsetof(Elf32_Ehdr, e_shoff)     / sizeof(uint8_t)]));
	header->e_flags     = littleendian_to_bigendian_32(&(buffer[offsetof(Elf32_Ehdr, e_flags)     / sizeof(uint8_t)]));
	header->e_ehsize    = littleendian_to_bigendian_16(&(buffer[offsetof(Elf32_Ehdr, e_ehsize)    / sizeof(uint8_t)]));
	header->e_phentsize = littleendian_to_bigendian_16(&(buffer[offsetof(Elf32_Ehdr, e_phentsize) / sizeof(uint8_t)]));
	header->e_phnum     = littleendian_to_bigendian_16(&(buffer[offsetof(Elf32_Ehdr, e_phnum)     / sizeof(uint8_t)]));
	header->e_shentsize = littleendian_to_bigendian_16(&(buffer[offsetof(Elf32_Ehdr, e_shentsize) / sizeof(uint8_t)]));
	header->e_shnum     = littleendian_to_bigendian_16(&(buffer[offsetof(Elf32_Ehdr, e_shnum)     / sizeof(uint8_t)]));
	header->e_shstrndx  = littleendian_to_bigendian_16(&(buffer[offsetof(Elf32_Ehdr, e_shstrndx)  / sizeof(uint8_t)]));
}

/* small function to rotate byte */
static uint16_t littleendian_to_bigendian_16(const uint8_t value_le[2])
{
	uint16_t value_0 = value_le[0]; /* lsb in big endian */
	uint16_t value_1 = value_le[1];
	return                                     (value_1 <<  8) | value_0;
}

static uint32_t littleendian_to_bigendian_32(const uint8_t value_le[4])
{
	uint32_t value_0 = value_le[0]; /* lsb in big endian */
	uint32_t value_1 = value_le[1];
	uint32_t value_2 = value_le[2];
	uint32_t value_3 = value_le[3];
	return (value_3 << 24) | (value_2 << 16) | (value_1 <<  8) | value_0;
}
#endif

/**
	* sanitize header to check if we actually dealing with riscv elf file
	*/
static int riscv_elf_sanitize(const Elf32_Ehdr *header)
{
	/* make sure magic is correct */
	if (header->e_ident[EI_MAG0] != ELFMAG0) return (-1);
	if (header->e_ident[EI_MAG1] != ELFMAG1) return (-1);
	if (header->e_ident[EI_MAG2] != ELFMAG2) return (-1);
	if (header->e_ident[EI_MAG3] != ELFMAG3) return (-1);

	/* we currently only support 32-bit risc-v machine (RV32) */
	if (header->e_ident[EI_CLASS] != ELFCLASS32) return (-1);

	/* we currently only support little endian risc-v machine */
	if (header->e_ident[EI_DATA] != ELFDATA2LSB) return (-1);

	if ((header->e_ident[EI_VERSION] != EV_CURRENT)
		|| (header->e_version != EV_CURRENT)) return (-1);

	/* we have no idea what pad value is */

	/* only process elf (ready to execute) file */
	if (header->e_type != ET_EXEC) return (-1);

	switch (header->e_machine)
	{
		case (ET_NONE):
		case (EM_M32):
		case (EM_SPARC):
		case (EM_386):
		case (EM_68K):
		case (EM_88K):
		case (EM_860):
		case (EM_MIPS):
		case (EM_MIPS_RS4_BE):
			return (-1);
		default:
			if ((header->e_machine >= 11) && (header->e_machine <= 16)) return (-1);

			/**
				* cannot definitely sure that this elf is for risc-v (e.g. ARM)
				* but at least we sure that this elf is not for machine above
				*/
			fprintf(stderr, "(ELF) e_machine: %u (0x%02x)\n", header->e_machine, header->e_machine);
	}

	/* executable should have entry point */
	if (header->e_entry == 0) return (-1);

	/* check header size */
	if (header->e_ehsize != sizeof(Elf32_Ehdr)) return (-1);

	/* cannot check e_flags */
	fprintf(stderr, "(ELF) e_flags: 0x%02x\n", header->e_flags);

	/**
		* we only care about executable perspective and not linking perspective
		* so we will only check program header
		*/
	if (header->e_phoff == 0) return (-1);
	if (header->e_phentsize != sizeof(Elf32_Phdr)) return (-1);
	if (header->e_phnum == 0) return (-1);


	return 0;
}


static Elf32_Phdr * get_riscv_elf_program_header_from_buffer_le(FILE *fd, size_t file_offset, size_t length)
{
	Elf32_Phdr *p;
	uint8_t     buffer[sizeof(Elf32_Ehdr) * length];

	int retval;
	int _errno;

	size_t retval2;


	p = (Elf32_Phdr *) malloc(sizeof(Elf32_Phdr) * length);
	if (p == NULL)
	{
		return NULL;
	}


	/* seek file */
	errno = 0;
	retval = fseek(fd, (long) file_offset, SEEK_SET); /* seek to program_header */
	_errno = errno;
	if (retval == (-1))
	{
		const char *s;

		assert(_errno != 0); /* errno should not 0 */

		/* get error message */
		s = strerror(_errno);

		/* print error message */
		if (s == NULL)
		{
			fprintf(stderr, "fseek to program header at address 0x%08x (%u)\n", \
				(unsigned int) file_offset, (unsigned int) file_offset);
		}
		else
		{
			fputs(s, stderr);
			fputc('\n', stderr);
		}

		/* free buffer */
		free(p);

		/* return error */
		return NULL;
	}

	assert((retval == 0) && (_errno == 0));

	/* read data */
	retval2 = fread(buffer, sizeof(Elf32_Phdr), length, fd);
	if (retval2 < length)
	{
		fprintf(stderr, "try to get %u program header with fread, but failed (offset: 0x%08x [%u])\n", \
			(unsigned int) length, (unsigned int) file_offset, (unsigned int) file_offset);
		free(p);
		return NULL;
	}

	/* for little endian, just do memcpy */
	memcpy(p, buffer, sizeof(Elf32_Phdr) * length);

	return p;
}

/*static Elf32_Phdr * get_riscv_elf_program_header_from_buffer_be(FILE *fd, size_t file_offset, size_t length)
{
	not implemented
}*/

/* generate byte stream of header */
static size_t generate_pulpissimo_elf_header_in_buffer(
	const Elf32_Ehdr * restrict elf32_header,
	const Elf32_Phdr * restrict elf32_program_header,
	void * restrict buffer, size_t buffer_len)
{
	size_t    pulpisimmo_elf_header_size = 0, next_size; /* initialize size of header to 0 */
	/*ptrdiff_t actual_header_size;*/
	uintptr_t actual_header_size;

	uint32_t   elf_size = 0;
	size_t    flash_sector_segment_offset;

	uint32_t  *crc_offset;

	size_t     number_of_segment; /*= 0*/

	Elf32_Half i, j;

	/**
		* use buffer as storage for pulpissimo elf header
		* note that restrict keyword here makes change in pulpissimo_elf_header
		* and buffer can be cached/reordered and write and read access to buffer
		* will be undefined when address are overlap
		*/
	struct pulpisimmo_elf_header_s * restrict pulpisimmo_elf_header = (struct pulpisimmo_elf_header_s * restrict) buffer;

	/* assume input is sane (all pointer are not NULL and buffer_len is more than 0) */

	/**
		* get number_of_segment value that is program segment with type PT_LOAD
		* actually we can do this later
		*/
	for (i = 0, number_of_segment = 0;
		i < elf32_header->e_phnum;
		j = i++, number_of_segment += (elf32_program_header[j].p_type == PT_LOAD) ? 1 : 0)
	{
	}


	/* we can write if buffer_len at least 4 + 4 + 4 + 4 + 16 * 4 * number_of_segment + 4 */
	/*if (buffer_len < (
		offsetof(struct pulpisimmo_elf_header_s, program_header)
		+ (sizeof(struct pulpisimmo_elf_program_header_s) * 4 * number_of_segment)
		+ sizeof(uint32_t)))
	{
		return 0;
	}*/


	/**
		* set buffer through pulpisimmo_elf_header struct
		* this way of assignment will follow host machine endianness
		* you need to fix this endianess problem (convert big endian to little endian (risc-v))
		* if your machine is big endian machine
		*/

	/* check if buffer will fit */
	if (next_size = pulpisimmo_elf_header_size + offsetof(struct pulpisimmo_elf_header_s, program_header), next_size >= buffer_len)
	{
		return 0; /* not enough space */
	}

	/*pulpisimmo_elf_header->elf_size = ;*/ /* cannot set this for now */
	pulpisimmo_elf_header->number_of_program_header = (uint32_t) number_of_segment;
	pulpisimmo_elf_header->entry = (uint32_t) (elf32_header->e_entry);
	pulpisimmo_elf_header->boot_address = L2_BOOT_ADDRESS; /* somehow this is constant */

	/* set current written size */
	pulpisimmo_elf_header_size = next_size;

	/**
		* write segment
		* set elf_size to 0, this is just dummy value for offset,
		* WE WILL FIX THIS offset VALUE AFTER PULPISIMMO ELF HEADER SIZE IS KNOWN
		* WHICH WILL BE ONLY KNOWN AFTER CRC SPACE CALCULATED
		*/
	for (i = 0, j = 0; i < elf32_header->e_phnum; ++i)
	{
		if (elf32_program_header[i].p_type == PT_LOAD)
		{
			/* only add segment with PT_LOAD */

			uint32_t segment_size           = elf32_program_header[i].p_filesz;
			uint32_t number_of_flash_sector = (segment_size + SPI_FLASH_SECTOR_SIZE_BYTES - 1) / SPI_FLASH_SECTOR_SIZE_BYTES; /* round up number of sector occupied */

			/**
				* check if buffer is enough
				* why multiply by 4? it is in plp_flash_stimuli
				*/
			if (next_size = pulpisimmo_elf_header_size + (sizeof(struct pulpisimmo_elf_program_header_s) * 4), next_size >= buffer_len)
			{
				return 0; /* not enough space */
			}

			/* indexing program_header with j, indexing elf32_program_header with i */

			pulpisimmo_elf_header->program_header[j].offset = elf_size; /* set this with elf_size, fix this later */
			pulpisimmo_elf_header->program_header[j].base_address = elf32_program_header[i].p_paddr;

			/* pyelftools/elftools/elf/segments.py, method data() from class Segment use p_filesz */
			pulpisimmo_elf_header->program_header[j].segment_size = segment_size;

			pulpisimmo_elf_header->program_header[j].number_of_flash_sector = number_of_flash_sector;

			/* count added segment with j, to later verify that it match with number_of_segment */
			j++;

			/* accumulate size */
			pulpisimmo_elf_header_size = next_size;

			/* increment elf size to next sector */
			elf_size += (number_of_flash_sector * SPI_FLASH_SECTOR_SIZE_BYTES);
		}
	}

	assert(((size_t) j) == number_of_segment);

	/* get actual size of header */
	actual_header_size = ((uintptr_t) &(pulpisimmo_elf_header->program_header[j])) - ((uintptr_t) pulpisimmo_elf_header);

	/**
		* add padding
		* space discrepency between actual header size and pulpisimmo elf header size should be filled by padding
		* we don't need to check buffer_len (it is already check in previous loop) we only fill (with 0xff) the empty space
		*/
	if (pulpisimmo_elf_header_size > ((size_t) actual_header_size))
	{
		memset(((uint8_t * restrict) buffer) + (actual_header_size / sizeof(uint8_t)), -1, (pulpisimmo_elf_header_size - actual_header_size) / sizeof(uint8_t));
	}


	/* prepare space for crc, BUT DO NOT WRITE IT YET, BECAUSE WE HAVE SOME SOME ATTRIBUTE DEFERRED */
	if (next_size = pulpisimmo_elf_header_size + sizeof(uint32_t), next_size >= buffer_len)
	{
		return 0; /* not enough space */
	}

	/* next size contain size after crc calculated */

	/* calculate segment offset */
	flash_sector_segment_offset = ((next_size + SPI_FLASH_SECTOR_SIZE_BYTES - 1) / SPI_FLASH_SECTOR_SIZE_BYTES) * SPI_FLASH_SECTOR_SIZE_BYTES; /* round-up to full sector */

	/* set deferred value */
	pulpisimmo_elf_header->elf_size = ((uint32_t) flash_sector_segment_offset) + elf_size;
	for (i = 0; i < pulpisimmo_elf_header->number_of_program_header; ++i)
	{
		/* add sector size for elf header to the offset */
		pulpisimmo_elf_header->program_header[i].offset += ((uint32_t) flash_sector_segment_offset);
	}

	/**
		* calculate crc up to pulpisimmo_elf_header_s
		* use pulpisimmo_elf_header and not _buffer because it is pulpisimmo_elf_header that we write
		*/
	crc_offset = (uint32_t *) (((uint8_t * restrict) buffer) + pulpisimmo_elf_header_size);
	*crc_offset = get_crc(pulpisimmo_elf_header, pulpisimmo_elf_header_size);

	return next_size;
}

/* copy header from le buffer to le host machine */
/*static void get_pulpissimo_elf_header_le(struct pulpisimmo_elf_header_s * restrict header,
	const uint8_t * restrict buffer)
{
}*/

/**
	* this method copied from plp_flash_stimuli.p around line 157
	*/
static uint32_t get_crc(const void *buffer, size_t buffer_len)
{
	const uint8_t * _buffer = (const uint8_t *) buffer;

	uint32_t crc = 0xffffffff;

	size_t i, j;

	/* sanity check */
	if ((buffer == NULL) || (buffer_len == 0))
	{
		return 0;
	}

	for (i = 0; i < buffer_len; ++i)
	{
		uint32_t u_data = _buffer[i];

		/* calculate with same type (uint32_t) */
		crc ^= u_data;

		/* scan bit in crc */
		for (j = 0; j < 8; ++j)
		{
			uint32_t mask = ((crc & 1) == 1) ? 0xffffffff : 0;
			crc = (crc >> 1) ^ (0xedb88320 & mask);
		}
	}

	return crc ^ 0xffffffff;
}




/**
	* DEBUGGING TOOLS
	*
	* print in hex format
	* __restrict is extension
	*/
static void hex_dump(size_t offset, const void * __restrict buffer, size_t buffer_len,
	FILE * __restrict stream)
{
	const unsigned char *_buffer = (const unsigned char *) buffer;
	size_t i, j;

	for (i = 0; i < buffer_len; i += 16)
	{
		/* 1. Print the offset (like 00000010:) */
		fprintf(stream, "%08zx: ", offset + i); /* z is for size_t, like l is for long */

		/* 2. Print the hex values */
		for (j = 0; j < 16; j++)
		{
			if ((i + j) < buffer_len)
			{
				fprintf(stream, "%02x ", (unsigned int) _buffer[i + j]);
			}
			else
			{
				fputs("   ", stream); /* Padding for the last line */
			}
		}

		/* separator to literal value */
		fputc(' ', stream);

		/* 3. Print the ASCII sidebar */
		for (j = 0; j < 16; j++)
		{
			if ((i + j) < buffer_len)
			{
				char c = (char) _buffer[i + j];

				/* Only print printable characters; dots for everything else */
				fputc(isprint(c) ? c : '.', stream);
			}
		}

		/* next line */
		fputc('\n', stream);
	}
}

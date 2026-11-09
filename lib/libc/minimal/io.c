/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include "hal/pulp.h"
#include "compatibility.h"
#include "io.h"
#include <pulp.h>

/* for implementation of strtol and strtod */
#include <ctype.h>
#include <limits.h>
/*#include <errno.h>*/

/* for pow() used in strtod, if strtod is not used, you don't have to link with -lm */
#include <math.h>

/**
    * when arduino is used, provide implementation for:
    * strnlen
    * strcasecmp
    * strncasecmp
    * strtok_r
    *
    * when arduino is not used (e.g. when compiling with
    * makefile to native test/unit test or to pulp-runtime-examples), rely on libc
    * implementation. link with -lc
    */
#ifdef ARDUINO
/**
    * instead of inclusion of pgmspace.h, which will messed up this library,
    * just declare necessary function prototype
    */
extern size_t pgmspace_strnlen(const char *s, size_t maxlen);

extern int pgmspace_strcasecmp(const char *s1, const char *s2);
extern int pgmspace_strncasecmp(const char *s1, const char *s2, size_t n);

extern char *pgmspace_strtok_r(char * restrict str, const char * restrict delim,
    char ** restrict saveptr);

/* pgmspace_strnlen is renamed to strnlen_P when POSIX.1-2008 is not defined */
/* pgmspace_strcasecmp, pgmspace_strncasecmp, pgmspace_strtok_r is renamed
to strcasecmp_P, strncasecmp_P, strtok_rP when POSIX.1-2001 is not defined */
#if (! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200809L)))
#define    pgmspace_strnlen          strnlen_P
#endif

#if (! (defined(_POSIX_C_SOURCE) && ((_POSIX_C_SOURCE) >= 200112L)))
#define    pgmspace_strcasecmp       strcasecmp_P
#define    pgmspace_strncasecmp      strncasecmp_P
#define    pgmspace_strtok_r         strtok_rP
#endif

#endif /* defined(ARDUINO) */


#if defined(CONFIG_IO_UART) && CONFIG_IO_UART == 1 && defined(ARCHI_HAS_CLUSTER)
static L1_DATA int io_lock = 0;
#endif



#if defined(CONFIG_IO_UART) && CONFIG_IO_UART == 1
static int pos_io_uart_enabled = 0;
static PI_L2 uint8_t pos_io_uart_buffer;
#endif



static int errno;
int *__errno() { return &errno; }



int strcmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s1 == *s2)
    {
        s1++;
        s2++;
    }

    return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}



int strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;

    while (n-- != 0 && *s1 == *s2)
    {
        if (n == 0 || *s1 == '\0')
            break;
        s1++;
        s2++;
    }

    return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}


/**
    * when arduino is used, strcasecmp and strncasecmp is provided by pgmspace
    * when arduino is not used, rely on linking with libc (-lc)
    */
#ifdef ARDUINO
int strcasecmp(const char *s1, const char *s2)
{
    return pgmspace_strcasecmp(s1, s2);
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
    return pgmspace_strncasecmp(s1, s2, n);
}
#endif /* defined(ARDUINO) */

size_t strlen(const char *str)
{
    const char *start = str;

    while (*str)
        str++;
    return str - start;
}

/**
    * when arduino is used, strnlen is provided by pgmspace
    * when arduino is not used, rely on linking with libc (-lc)
    */
#ifdef ARDUINO
size_t  strnlen(const char *str, size_t maxlen)
{
    return pgmspace_strnlen(str, maxlen);
}
#endif /* defined(ARDUINO) */

/**
    * when arduino is used, strtok_r is provided by pgmspace
    * when arduino is not used, rely on linking with libc (-lc)
    */
#ifdef ARDUINO
char * strtok_r(char * restrict str, const char * restrict delim,
    char ** restrict saveptr)
{
    return pgmspace_strtok_r(str, delim, saveptr);
}
#endif /* defined(ARDUINO) */

int memcmp(const void *m1, const void *m2, size_t n)
{
    unsigned char *s1 = (unsigned char *) m1;
    unsigned char *s2 = (unsigned char *) m2;

    while (n--)
    {
        if (*s1 != *s2)
        {
            return *s1 - *s2;
        }
        s1++;
        s2++;
    }
    return 0;
}



void *memset(void *m, int c, size_t n)
{
    char *s = (char *)m;
    while (n--)
        *s++ = (char) c;

    return m;
}



void *memcpy(void *dst0, const void *src0, size_t len0)
{
    void *save = dst0;

    char src_aligned = (((size_t) src0) & 3) == 0;
    char dst_aligned = (((size_t) dst0) & 3) == 0;
    char copy_full_words = (len0 & 3) == 0;

    if (src_aligned && dst_aligned && copy_full_words)
    {
        /* all accesses are aligned => can copy full words */
        uint32_t *dst = (uint32_t *) dst0;
        uint32_t *src = (uint32_t *) src0;

        while (len0)
        {
            *dst++ = *src++;
            len0 -= 4;
        }
    }
    else
    {
        /* copy bytewise */
        char *dst = (char *) dst0;
        char *src = (char *) src0;

        while (len0)
        {
            *dst++ = *src++;
            len0--;
        }
    }

    return save;
}


void *memchr(const void *s, int c, size_t n)
{
    size_t          i;
    const uint8_t *_s;

    /* check input buffer */
    if ((s == NULL) || (n == 0))
    {
        return NULL; /* not found if buffer is invalid or have zero length */
    }

    /* scan buffer s */
    for (i = 0, _s = s; i < n; ++i, ++_s)
    {
        /* check if matching byte is found */
        if ((*_s) == ((uint8_t) c))
        {
            /* match found */
            return (void *) _s;
        }
    }

    /* no match found */
    return NULL;
}


void *memmove(void *d, const void *s, size_t n)
{
    char *dest = d;
    const char *src  = s;

    if ((size_t) (dest - src) < n)
    {
        /*
         * The <src> buffer overlaps with the start of the <dest> buffer.
         * Copy backwards to prevent the premature corruption of <src>.
         */

        while (n > 0)
        {
            n--;
            dest[n] = src[n];
        }
    }
    else
    {
        /* It is safe to perform a forward-copy */
        while (n > 0)
        {
            *dest = *src;
            dest++;
            src++;
            n--;
        }
    }

    return d;
}



char *strcpy(char *d, const char *s)
{
    char *dest = d;
    while (*s != '\0')
    {
        *d = *s;
        d++;
        s++;
    }
    *d = '\0';
    return dest;
}



char *strcat(char *dest, const char *src)
{
    strcpy(dest + strlen(dest), src);
    return dest;
}



char *strchr(const char *s, int c)
{
    char tmp = (char) c;

    while ((*s != tmp) && (*s != '\0'))
        s++;

    return (*s == tmp) ? (char *) s : NULL;
}



static void pos_libc_putc_stdout(char c)
{
    *(volatile uint32_t *)(long)(ARCHI_STDOUT_ADDR + STDOUT_PUTC_OFFSET + (hal_core_id()<<3) + (hal_cluster_id()<<7)) = c;
}



#if defined(CONFIG_IO_UART) && CONFIG_IO_UART == 1
static void pos_libc_putc_uart(char c)
{
    if (pos_io_uart_enabled)
    {
        pos_io_uart_buffer = c;
        uart_write(CONFIG_IO_UART_ITF, &pos_io_uart_buffer, 1);
    }
}
#endif



static void pos_putc(char c)
{
#if defined(CONFIG_IO_UART) && CONFIG_IO_UART == 1
    pos_libc_putc_uart(c);
#else
    pos_libc_putc_stdout(c);
#endif
}



int puts(const char *s)
{
    char c;
    do
    {
        c = *s;
        if (c == 0)
        {
            pos_putc('\n');
            break;
        }
        pos_putc(c);
        s++;
    } while(1);

    return 0;
}



int fputc(int c, FILE *stream)
{
    pos_putc(c);

    return 0;
}


int pos_libc_fputc_locked(int c, FILE *stream)
{
    pos_putc(c);

    return 0;
}



int putchar(int c)
{
    return fputc(c, stdout);
}


#if defined(CONFIG_IO_UART) && CONFIG_IO_UART == 1 && defined(ARCHI_HAS_CLUSTER)

static void uart_io_lock()
{
    while (tas_lock_32((int)&io_lock) == -1)
    {
        volatile int i;
        for (i=0; i<100; i++);
    }
}

static void uart_io_unlock()
{
    tas_unlock_32((int)&io_lock, 0);
}

#endif


int pos_libc_prf_locked(int (*func)(), void *dest, char *format, va_list vargs)
{
    int err;

#if defined(CONFIG_IO_UART) && CONFIG_IO_UART == 1 && defined(ARCHI_HAS_CLUSTER)
    uart_io_lock();
#endif

    err =  pos_libc_prf(func, dest, format, vargs);

#if defined(CONFIG_IO_UART) && CONFIG_IO_UART == 1 && defined(ARCHI_HAS_CLUSTER)
    uart_io_unlock();
#endif

    return err;
}


static void __attribute__((noreturn)) pos_wait_forever()
{
#if defined(ITC_VERSION)
    if (hal_is_fc())
    {
        hal_itc_enable_clr(0xffffffff);
        while(1) hal_itc_wait_for_interrupt();
    }
    else
    {
#if defined(EU_VERSION) && EU_VERSION >=3
        eu_evt_maskClr(0xffffffff);
        eu_evt_wait();
#endif
    }
#elif defined(EU_VERSION) && EU_VERSION >=3
    eu_evt_maskClr(0xffffffff);
    eu_evt_wait();
#endif
    while(1);
}


__attribute__ ((noreturn)) void exit(int status)
{
#ifndef ARCHI_NO_FC
    apb_soc_status_set(status);
#endif
    pos_wait_forever();
}



__attribute__ ((noreturn)) void abort()
{
    exit(-1);
}



int pos_io_start()
{
#if defined(CONFIG_IO_UART) && CONFIG_IO_UART == 1

    uart_open(CONFIG_IO_UART_ITF, CONFIG_IO_UART_BAUDRATE);

    pos_io_uart_enabled = 1;

#endif

    return 0;
}



int pos_io_stop()
{
#if defined(CONFIG_IO_UART) && CONFIG_IO_UART == 1

    pos_io_uart_enabled = 0;

    uart_close(CONFIG_IO_UART_ITF);

#endif

  return 0;
}

/* needed by g++ */
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE * restrict stream)
{
    const char *ptr_c = (const char *) ptr;

    size_t i, j;

    bool is_error = false;

    for (i = 0; (! is_error) && (i < nmemb); ++i)
    {
        for (j = 0; (! is_error) && (j < size); ++j)
        {
            int c = *ptr_c++;

            /* make sure fputc success */
            if (fputc((int) c, stream) != c)
            {
                is_error = true;
            }

            if (is_error) break;
        }

        if (is_error) break;
    }

    return i;
}

/* needed by g++ */
ssize_t write(int fd, const void *ptr, size_t size)
{
    return (ssize_t) fwrite(ptr, 1, size, (FILE *) fd);
}

/* needed by g++ */
int fputs(const char * restrict s, FILE * restrict stream)
{
    /* ignore stream and reuse puts */
    (void) stream;

    return puts(s);
}

long strtol(const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    bool negative_number = false;

    int c;

    /**
        * indicate status of parsing
        * 0: nothing to parse (not found any number)
        * 1: something is parsed (maybe just 0)
        * -1: overflow
        */
    int any = 0;

    int cutlim; /* set cutlim to int so comparison with c don't need cast */
    unsigned long cutoff;

    unsigned long accumulate = 0;

    /* 1. Skip whitespace */
    while (isspace((int) (*s)))
    {
        s++;
    }

    /* 2. Handle sign */
    if ((*s) == '-')
    {
        negative_number = true;
        s++;
    }
    else if ((*s) == '+')
    {
        s++;
    }

    /* 3. Handle base auto-detection (base 0) and at same time skip leading "0x" when found */
    if (((base == 0) || (base == 16)) && ((*s) == '0') && (((*(s + 1)) == 'x') || ((*(s + 1)) == 'X')))
    {
        s += 2;
        base = 16;
    }

    /* if base is not detected as base 16, try detect it by checking if it has prefix 0 */
    if (base == 0)
    {
        base = ((*s) == '0') ? 8 : 10;
    }

    /* 4. Overflow boundaries */
    cutoff  = negative_number ? (unsigned long) -(LONG_MIN) /*((LONG_MIN) * (-1l))*/ : LONG_MAX;

    /* get last value of digit possible to add right before overflow */
    cutlim  = (int) (cutoff % ((unsigned long) base));

    /* get last value of accumulated digit divisible by base right before overflow addition */
    cutoff /= (unsigned long) base;

    /* 5. Conversion loop */
    for (accumulate = 0, any = 0;; s++)
    {
        c = *s;
        if      (isdigit(c)) c -= '0';
        else if (isalpha(c)) c  = tolower(c) - 'a' + 10;
        else                 break;

        if (c >= base)       break;

        /* keep parsing even if any is -1, because we need to consume all digits */
        if ((any < 0)
            || (accumulate > cutoff)
            || ((accumulate == cutoff) && (c > cutlim))) /* look ahead if conversion can cause overflow */
        {
            /* keep flag indicate error, or set it to error */
            any = -1;
        }
        else
        {
            /* do conversion */
            any = 1; /* something is parsed */

            /* accumulate */
            accumulate *= base;
            accumulate += c; /* c is already in a number (not ascii digit character) */
        }
    }

    /* 6. Finalize result and endptr */
    if (any < 0)
    {
        accumulate = negative_number ? LONG_MIN : LONG_MAX;
        errno = -1; /*ERANGE*/
    }
    else if (negative_number)
    {
        accumulate *= (-1l);
    }

    /* set endptr if it not NULL */
    if (endptr != NULL)
    {
        /**
            * when no parsing was done, return nptr
            * when error (overflow) or success occur, return next buffer
            *
            * cast to char * to remove qualifier
            */
        *endptr = (char *) ((any != 0) ? s : nptr);
    }

    /* return parsed value */
    return accumulate;
}

double strtod(const char *nptr, char **endptr)
{
    const char *s = nptr;

    /**
        * set 1. if positive (default)
        * set -1. if negative
        */
    double sign = 1.0;

    /* accumulated all part */
    double res = 0.0;

    /* fractional indicator, getting smaller as farther we parse fraction part */
    double factor = 1.0;

    bool has_content = false;

    /* 1. Skip whitespace */
    while (isspace((int) (*s)))
    {
        s++;
    }

    /* 2. Handle sign */
    if ((*s) == '-')
    {
        sign = -1.0;
        s++;
    }
    else if ((*s) == '+')
    {
        s++;
    }

    /* 3. Integer part */
    while (isdigit((int) (*s)))
    {
        res = (res * 10.0) + ((*s) - '0');
        s++;

        /* set flag that we found integer part */
        has_content = true;
    }

    /**
        * 4. Fractional part
        *    find period, then parse fraction part
        */
    if ((*s) == '.')
    {
        s++;

        /* parse fraction */
        while (isdigit((int) (*s)))
        {
            factor *= 0.1;

            res += ((*s) - '0') * factor;
            s++;

            /* set flag that we found fractional part */
            has_content = true;
        }
    }

    /* 5. Exponent part (e+10, E-5) */
    if (has_content && (((*s) == 'e') || ((*s) == 'E')))
    {
        /* save current ptr before moving forward */
        const char *backtrack = s;

        /**
            * exponent maybe negative
            * set to 1 if positive (default)
            * set to -1 if exponent is negative
            */
        int exp_sign = 1;

        /* move to pointer parse exponent number */
        s++;

        /* check sign (exponent maybe don't have sign) */
        if ((*s) == '-')
        {
            exp_sign = -1;
            s++;
        }
        else if (*s == '+')
        {
            s++;
        }

        /**
            * if after e and sign we found digit, parse it
            * as valid exponent value, if not, backtrack
            */
        if (isdigit((int) (*s)))
        {
            int exp_val = 0; /* set accumulator to 0 */

            while (isdigit((int) (*s)))
            {
                /* FIXME: check overflow */
                exp_val = (exp_val * 10) + ((*s) - '0');
                s++;
            }
            res *= pow(10.0, (double) (exp_sign * exp_val));
        }
        else
        {
            s = backtrack; /* No valid exponent, backtrack */
        }
    }

    /* set endptr if not NULL */
    if (endptr != NULL)
    {
        /* typecast to remove qualifier */
        *endptr = (char *) (has_content ? s : nptr);
    }

    /* fix sign */
    return res * sign;
}

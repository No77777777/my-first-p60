/*=============================================================================
 
									libdl_print.c 
 
 GENERAL DESCRIPTION
		 libdl print functions
		 
 EXTERNAL FUNCTIONS
		None.
 
 INITIALIZATION AND SEQUENCING REQUIREMENTS
		None.
 
Copyright (c) 2013  by Qualcomm Technologies, Inc.  All Rights Reserved
 
 =============================================================================*/
 
/*
 * Copyright (c) 2002-2004, Karlsruhe University
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * Australian Public Licence B (OZPLB)
 *
 * Version 1-0
 *
 * Copyright (c) 2007, Open Kernel Labs, Inc.
 *
 * All rights reserved. 
 *
 * Developed by: Embedded, Real-time and Operating Systems Program (ERTOS)
 *               National ICT Australia
 *               http://www.ertos.nicta.com.au
 *
 * Permission is granted by National ICT Australia, free of charge, to
 * any person obtaining a copy of this software and any associated
 * documentation files (the "Software") to deal with the Software without
 * restriction, including (without limitation) the rights to use, copy,
 * modify, adapt, merge, publish, distribute, communicate to the public,
 * sublicense, and/or sell, lend or rent out copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimers.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimers in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of National ICT Australia, nor the names of its
 *       contributors, may be used to endorse or promote products derived
 *       from this Software without specific prior written permission.
 *
 * EXCEPT AS EXPRESSLY STATED IN THIS LICENCE AND TO THE FULL EXTENT
 * PERMITTED BY APPLICABLE LAW, THE SOFTWARE IS PROVIDED "AS-IS", AND
 * NATIONAL ICT AUSTRALIA AND ITS CONTRIBUTORS MAKE NO REPRESENTATIONS,
 * WARRANTIES OR CONDITIONS OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO ANY REPRESENTATIONS, WARRANTIES OR CONDITIONS
 * REGARDING THE CONTENTS OR ACCURACY OF THE SOFTWARE, OR OF TITLE,
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT,
 * THE ABSENCE OF LATENT OR OTHER DEFECTS, OR THE PRESENCE OR ABSENCE OF
 * ERRORS, WHETHER OR NOT DISCOVERABLE.
 *
 * TO THE FULL EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL
 * NATIONAL ICT AUSTRALIA OR ITS CONTRIBUTORS BE LIABLE ON ANY LEGAL
 * THEORY (INCLUDING, WITHOUT LIMITATION, IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY CLAIM, LOSS, DAMAGES OR OTHER
 * LIABILITY, INCLUDING (WITHOUT LIMITATION) LOSS OF PRODUCTION OR
 * OPERATION TIME, LOSS, DAMAGE OR CORRUPTION OF DATA OR RECORDS; OR LOSS
 * OF ANTICIPATED SAVINGS, OPPORTUNITY, REVENUE, PROFIT OR GOODWILL, OR
 * OTHER ECONOMIC LOSS; OR ANY SPECIAL, INCIDENTAL, INDIRECT,
 * CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES, ARISING OUT OF OR IN
 * CONNECTION WITH THIS LICENCE, THE SOFTWARE OR THE USE OF OR OTHER
 * DEALINGS WITH THE SOFTWARE, EVEN IF NATIONAL ICT AUSTRALIA OR ITS
 * CONTRIBUTORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH CLAIM, LOSS,
 * DAMAGES OR OTHER LIABILITY.
 *
 * If applicable legislation implies representations, warranties, or
 * conditions, or imposes obligations or liability on National ICT
 * Australia or one of its contributors in respect of the Software that
 * cannot be wholly or partly excluded, restricted or modified, the
 * liability of National ICT Australia or the contributor is limited, to
 * the full extent permitted by the applicable legislation, at its
 * option, to:
 * a.  in the case of goods, any one or more of the following:
 * i.  the replacement of the goods or the supply of equivalent goods;
 * ii.  the repair of the goods;
 * iii. the payment of the cost of replacing the goods or of acquiring
 *  equivalent goods;
 * iv.  the payment of the cost of having the goods repaired; or
 * b.  in the case of services:
 * i.  the supplying of the services again; or
 * ii.  the payment of the cost of having the services supplied again.
 *
 * The construction, validity and performance of this licence is governed
 * by the laws in force in New South Wales, Australia.
 */
 
#define false (0)
#define true (1)
#define bool int
#define NULL ((void *) 0)
typedef unsigned int word_t;

typedef __builtin_va_list va_list;

#define va_arg(ap, type) __builtin_va_arg((ap), type)
#define va_copy(dest, src) __builtin_va_copy((ap), type)
#define va_end(ap) __builtin_va_end((ap))
#define va_start(ap, parmN) __builtin_stdarg_start((ap), (parmN))

/* convert nibble to lowercase hex char */
#define hexchars(x) (((x) < 10) ? ('0' + (x)) : ('a' + ((x) - 10)))
#define putc writechr 

extern int sys_write(int, const char *, int);

static void writechr(const char chr)
{
	char buf[2];
	buf[0] = chr;
	buf[1] = '\0';
    sys_write(1, buf, 1);
}


static int print_hex(const word_t val,
                                  int width ,
                                  int precision ,
                                  bool adjleft ,
                                  bool nullpad )
{
    long i, n = 0;
    long nwidth = 0;

    // Find width of hexnumber
    while ((val >> (4 * nwidth)) && (word_t) nwidth <  2 * sizeof (word_t))
        nwidth++;

    if (nwidth == 0)
        nwidth = 1;

    // May need to increase number of printed digits
    if (precision > nwidth)
        nwidth = precision;

    // May need to increase number of printed characters
    if (width == 0 && width < nwidth)
        width = nwidth;

    // Print number with padding
    if (! adjleft)
        for (i = width - nwidth; i > 0; i--, n++)
            putc (nullpad ? '0' : ' ');
    for (i = 4 * (nwidth - 1); i >= 0; i -= 4, n++)
        putc (hexchars ((val >> i) & 0xF));
    if (adjleft)
        for (i = width - nwidth; i > 0; i--, n++)
            putc (' ');

    return n;
}

/**
 *      Print a string
 *
 *      @param s        zero-terminated string to print
 *      @param width    minimum width of printed string
 *
 *      Prints the zero-terminated string using putc().  The printed
 *      string will be right padded with space to so that it will be
 *      at least WIDTH characters wide.
 *
 *      @returns the number of charaters printed.
 */
static int print_string(const char * s,
                                     const int width ,
                                     const int precision )
                                
                                     //const int width = 0,
                                     //const int precision = 0)
{
    int n = 0;

    for (;;)
    {
        if (*s == 0)
            break;

        putc(*s++);
        n++;
        if (precision && n >= precision)
            break;
    }

    while (n < width) { putc(' '); n++; }

    return n;
}


/**
 *      Print decimal value
 *
 *      @param val      value to print
 *      @param width    width of field
 *      @param pad      character used for padding value up to width
 *
 *      Prints a value as a decimal in the given WIDTH with leading
 *      whitespaces.
 *
 *      @returns the number of characters printed (may be more than WIDTH)
 */
static int print_dec(const word_t val, const int width, bool negative, const char pad)
{
    word_t divisor;
    int digits, extra;

    /* estimate number of spaces and digits */
    for (divisor = 1, digits = 1; val/divisor >= 10; divisor *= 10, digits++);

    if (negative)
        digits++;
    extra = 0;
    /* print spaces */
    for ( ; digits < width; digits++ ) {
        extra ++;
        putc(pad);
    }

    if (negative)
        putc('-');
    /* print digits */
    do {
        putc(((val/divisor) % 10) + '0');
    } while (divisor /= 10);

    /* report number of chars printed */
    return digits + extra;
}


/**
 *      Does the real printf work
 *
 *      @param format_p         pointer to format string
 *      @param args             list of arguments, variable length
 *
 *      Prints the given arguments as specified by the format string.
 *      Implements a subset of the well-known printf plus some L4-specifics.
 *
 *      @returns the number of characters printed
 */
static int do_osprintf(const char* format_p, va_list args)
{
    const char* format = format_p;
    int n = 0;
    int width = 8;
    int precision = 0;
    bool adjleft = false, nullpad = false;

#define arg(x) va_arg(args, x)

    /* sanity check */
    if (format == NULL)
    {
        n = 0;
        goto end;
    }

    while (*format)
    {
        switch (*(format))
        {
        case '%':
            width = precision = 0;
            adjleft = nullpad = false;
        reentry:
            switch (*(++format))
            {
                /* modifiers */
            case '.':
                for (format++; *format >= '0' && *format <= '9'; format++)
                    precision = precision * 10 + (*format) - '0';
                if (*format == 'w')
                {
                    // Set precision to printsize of a hex word
                    precision = sizeof (word_t) * 2;
                    format++;
                }
                format--;
                goto reentry;
            case '0':
                nullpad = (width == 0);
                /* fall through */
            case '1': 
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                width = width*10 + (*format)-'0';
                goto reentry;
            case 'w':
                // Set width to printsize of a hex word
                width = sizeof (word_t) * 2;
                goto reentry;
            case '-':
                adjleft = true;
                goto reentry;
            case 'l':
                goto reentry;
            case 'c':
                putc(arg(int));
                n++;
                break;
            //case 'm':   /* microseconds */
            //{
            //    /*lint -e571 flint doesn't deal with va_arg very well*/
            //    n += print_hex64(arg(u64_t), width, precision,
            //                   adjleft, nullpad);
            //    break;
            //}
            case 'd':
            {
                long val = arg(long);
                bool negative = false;
                if (val < 0)
                {
                    negative = true;
                    val = -val;
                }
                n += print_dec(val, width, negative, ' ');
                break;
            }
            case 'u':
                n += print_dec(arg(long), width, 0, ' ');
                break;
            case 'p':
                precision = sizeof (word_t) * 2;
                /* fall through */
            case 'x':
                n += print_hex(arg(long), width, precision,
                               adjleft, nullpad);
                break;
            case 's':
            {
                char* s = arg(char*);
                if (s)
                    n += print_string(s, width, precision);
                else
                    n += print_string("(null)", width, precision);
            }
            break;

            //case 't':
            //case 'T':
            //    n += print_tcb (arg (word_t), width, precision, adjleft);
            //    break;
            //case 'S':
            //    n += print_space (arg (word_t), width, precision, adjleft);
            //    break;

            case '%':
                putc('%');
                n++;
                format++;
                continue;
            default:
                n += print_string("?", 0, 0);
                break;
            };
            break;
        default:
            putc(*format);
            n++;
            break;
        }
        format++;
    }

end:
    putc('\n');
    return n;
}

static volatile int depth=0;
int stack_track(void)
{
    depth++;
    return 0;
}

/**
 *      Flexible print function
 *
 *      @param format   string containing formatting and parameter type
 *                      information
 *      @param ...      variable list of parameters
 *
 *      @returns the number of characters printed
 */
int libdl_mutex = 0;
int libl_mutext_init = 0;

extern int sys_Mtxlock(void *);
extern int sys_Mtxunlock(void*);
extern void sys_Mtxinit(void **mutex);



int libdl_printf(const char* format,  va_list args)
{
    
    int i;
    i = do_osprintf(format, args);
    return i;
};

int libdl_vprintf(const char* format, va_list args)
{
    int i;
    i = do_osprintf(format, args);
    return i;
}



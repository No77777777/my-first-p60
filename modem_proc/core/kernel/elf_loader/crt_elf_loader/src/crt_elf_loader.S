/*=============================================================================

                 crt0.S -- H E A D E R  F I L E

GENERAL DESCRIPTION
    Start up functions

EXTERNAL FUNCTIONS
   None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

      Copyright (c) 2013  by Qualcomm Technologies, Inc.  All Rights Reserved.

=============================================================================*/


	.section .start, "ax", @progbits
	.subsection 0

	.weak	_start, start
	.set    _start, ustart
	.set    start, ustart

	.org	0			/* Must start at address 0. */
/*
||  This file uses PC-relative addressing to find its global symbols
||   so that it's position independent.  Because performance of this
||   code is not important, PC-relative is fine here and will work
||   even in non-position-independent builds.
*/

	.type	ustart, @function
ustart:
	/* Initialize FP and SP first */

	{
	fp = #0
	sp = add(pc, ##(ustack+4096)@PCREL)
    r0 = #0
	}

  #if __HEXAGON_ARCH__ > 60
    C16 = r0          // disable FRML
  #endif

	/* GP = _SDA_BASE_ && qurt_thread_set_gp(GP) */

	r0 = add(pc, ##_SDA_BASE_@PCREL)
	r1 = add(pc, ##qurt_thread_set_gp@PCREL)
	{
	gp = r0
	callr r1
	}

	/* zero_mem_range(&__bss_start, &_end) */

	r0 = add(pc, ##__bss_start@PCREL)
	r1 = add(pc, ##_end@PCREL)
	call zero_mem_range

	/* zero_mem_range(&__sbss_start, &__sbss_end) */

	r0 = add(pc, ##__sbss_start@PCREL)
	r1 = add(pc, ##__sbss_end@PCREL)
	call zero_mem_range

	/* __stack_chk_guard = R16 */

	r0 = add(pc, ##__stack_chk_guard@PCREL)
	memw(r0) = r16

	/* qurt_tcm_utcb_from_stack = R17 */

	r1 = add(pc, ##qurt_tcm_utcb_from_stack@PCREL)
	memw(r1) = r17

	/* call qurt_elf_loader_user_mode_init() */

	r2 = add(pc, ##qurt_elf_loader_user_mode_init@PCREL)
	callr r2


    r0 = add(pc, ##ustack@PCREL)
	r1 = ##0x1000

	/* main() */
	r2 = add(pc, ##rtld_main@PCREL)
	callr r2
	/* In case lib_qurt_init returns.  It shouldn't. */

	jump .

	/*
	 * void zero_mem_range (start, end)
	 *
	 * Break it into thread parts, head_size, middle_size and end_size
	 * Perform a byte wise copy for head_size and end_size and
	 * perform a double word copy for middle_size
	 *
	 * NOTE:
	 * ----
	 * - Doesn't use stack
	 * - r0, r1, r2, r3, r4, r5, p0 are clobberred
	 * - r4, r5 are set to "0"
	 */
     .global zero_mem_range
	.type	zero_mem_range, @function
zero_mem_range:
	{
		r3 = add (r0, #7)
		r4 = #0
	}
	{
		r3 = insert (r4, #3, #0)
		r5 = #0
	}
	p0 = cmp.gt (r3, r1)
	if (p0) r3 = r1
1:
	{
		p0 = cmp.eq (r0, r3)
		if p0.new jump:nt 2f
	}
	{
		memb (r0++#1) = r4
		jump 1b
	}
2:
	{
		// If both start and end falls within same double word
		p0 = cmp.eq (r0, r1)
		if (p0.new) jump:nt 3f
		r3 = r1
	}
	r3 = insert (r4, #3, #0)
1:
	{
		p0 = cmp.eq (r0, r3)
		if (p0.new) jump:nt 2f
	}
	{
		memd (r0++#8) = r5:4
		jump 1b
	}
2:
	r3 = r1
1:
	{
		p0 = cmp.eq (r0, r3)
		if (p0.new) jump:nt 3f
	}
	{
		memb (r0++#1) = r4
		jump 1b
	}
3:
	jumpr r31
	.size	zero_mem_range, .-zero_mem_range


	.section .bss
	.align	8
ustack:
	.space	4096

/*
|| This variable "shadows" the same variable which is part of the QuRT configuration.
||  To keep the code simple, we just assume this variable always exists, and we start
||  each new process with the configured value in R17.  The process initializes this
||  variable after clearing bss.
*/

	.global qurt_tcm_utcb_from_stack
qurt_tcm_utcb_from_stack:
	.space	4

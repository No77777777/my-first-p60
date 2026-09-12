
	.text
	.globl dczero
	.type	dczero, @function

dczero:
	{
		r1 = lsr(r1, #5)
		if (cmp.eq(r1.new, #0)) jump:nt .Exit
	}
	{
		loop0(.dczeroa_loop, r1)
	}
	.falign

.dczeroa_loop:	
	{
		r0 = add(r0, #32)
		dczeroa(r0)
	}:endloop0

.Exit:	
	{
		jumpr lr
	}
        
	.size	dczero, .-dczero

#ifndef _QRLBN_MACROS_H
#define _QRLBN_MACROS_H

/*
 * These are branch-free (except are paranoia level 0) macros for
 * simple arithmetic comparisions.  They all return 1 for true and 0
 * for false.
 *
 * CAUTION: branch-free at the C level does not not necessarily mean
 * branch-free at machine code level or at the machine level.  If the
 * compiler is generating code for an add of uint64_t values on a 32
 * bit machine, it might add the two lower parts, and then branch on
 * the carry condition code.  Also, on microcoded machines,
 * instruction timing may be data-dependent.
 *
 * The compile time flag QRLBN_MACROS_PARNOIA_LEVEL controls effort
 * spent to ensure freedom from leaks and resiliance against perverse
 * compilers.
 *
 * Level 0 just uses ordinary C operators, which may be constant time on ARM.  
 * Level 1 uses branch-free implementation
 * Level 2 uses branch-free implementation with defense against the
 *         compiler "figuring out" what is happening and generating branches.
 * 
 * The default paranoia level is 2.
 *
 * These are parameterized by size. The can accept types of equal or
 * smaller size, but larger sizes are slower.
 *
 * Note that these macros may evalute their arguments more that once.  
 *
 * At level 2 there must be a variable named volatile_zero
 * in scope whereever any of these macros are used.  The declartion
 * should look like this:
 *
 * volatile int64_t volatile_zero = 0;
 *
 */

#ifndef QRLBN_MACROS_PARANOIA_LEVEL
#define QRLBN_MACROS_PARANOIA_LEVEL 2
#endif

#if QRLBN_MACROS_PARANOIA_LEVEL == 0

#define nonzero16(a)  ((uint16_t)(a) != 0)
#define nonzero32(a)  ((uint32_t)(a) != 0)
#define nonzero64(a)  ((uint64_t)(a) != 0)

#define equal16unsigned(a, b) ((uint16_t)(a) == (uint16_t)(b))
#define equal32unsigned(a, b) ((uint32_t)(a) == (uint32_t)(b))
#define equal64unsigned(a, b) ((uint64_t)(a) == (uint64_t)(b))

#define equal16signed(a, b) ((int16_t)(a) == (int16_t)(b))
#define equal32signed(a, b) ((int32_t)(a) == (int32_t)(b))
#define equal64signed(a, b) ((int64_t)(a) == (int64_t)(b))

#define negative16(a) ((int16_t)(a) < 0)
#define negative32(a) ((int32_t)(a) < 0)
#define negative64(a) ((int64_t)(a) < 0)

#else 

  
#if QRLBN_MACROS_PARANOIA_LEVEL == 1
#define QRLBN_SZERO ((int64_t)(0))
#else
#define QRLBN_SZERO (volatile_zero)  // type is int64_t
#endif // end of if QRLBN_MACROS_PARANOIA_LEVEL == 1

// If we get here, paranoia level is 1 or 2


#define nonzero16(a) \
  ((int)(((uint32_t)QRLBN_SZERO - (uint32_t)(uint16_t)(a)) >> 31)) 
  

#define nonzero32(a) \
  ((int)(((uint32_t)QRLBN_SZERO - (((uint32_t)(a) & 0xffff) | ((uint32_t)(a) >> 16))) >> 31))

#define nonzero64(a) \
  ((int) (((uint64_t)QRLBN_SZERO - (((uint64_t)(a) & 0xffffffffULL)  |	\
				    ((uint64_t)(a) >> 32))) >> 63))


#define equal16unsigned(a, b)  \
  (1 - (int)(((unsigned)QRLBN_SZERO - ((uint16_t)(a) ^ (uint16_t)(b))) >> 31))

#define equal32unsigned(a, b)						\
  (1 - (int) ((((uint32_t)(a) - (uint32_t)(b) + (uint32_t)QRLBN_SZERO) |      \
	       ((uint32_t)(b) - (uint32_t)(a))) >> 31))

#define equal64unsigned(a, b)						\
  (1 - (int) ((((uint64_t)(a) - (uint64_t)(b) + (uint64_t)QRLBN_SZERO) | \
		       ((uint64_t)(b) - (uint64_t)(a))) >> 63))




#define equal16signed(a, b) equal16unsigned((int16_t)(a), (int16_t)b)
#define equal32signed(a, b) equal32unsigned((int32_t)(a), (int32_t)(b))
#define equal64signed(a, b) equal64unsigned((int64_t)(a), (int64_t)(b))

#define negative16(a) ((int)(((uint32_t)((int32_t)(int16_t)(a) + (int32_t)QRLBN_SZERO) >> 31)))
#define negative32(a) ((int)((uint32_t)((int32_t)(a) + (int32_t)QRLBN_SZERO) >> 31))
#define negative64(a) ((int) ((uint64_t)((int64_t)(a) + (int64_t)QRLBN_SZERO) >> 63))

#endif  // paramoia level > 0


#endif
#pragma once
/**
   @file histogram.h 
   @author anandj 
   @details A generic histogram of fixed bins with power-of-2 bin widths 
 */

/** @brief Maximum number of bins */
#define HISTOGRAM_MAX_BINS 32

/** @brief Macro for number of elements in array */
#define HISTOGRAM_ARRAY_SIZE(a) ( sizeof( ( a ) )/sizeof( ( a )[ 0 ] ) )

/** @brief Macro to compute range of a bin */
#define HISTOGRAM_BIN_RANGE( hist ) ( 1 << ( hist )->order )

/** @brief Macro to compute minimum of A and B */
#define HISTOGRAM_MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )

/** @brief Type of a bin */
typedef struct
{
    /** @brief Low value (inclusive) */
    unsigned int lo_inclusive;

    /** @brief High value (exclusive) */
    unsigned int hi_exclusive;

    /** @brief Count of sample that fall within [lo_inclusive, hi_exclusive) */
    unsigned int count;
} histogram_bin_t;
/** @brief Histogram abstract datatype */
typedef struct
{

    /** @brief Low value. Samples below these end up in < low bin */
    unsigned int lo;

    /** @brief High value. Samples above this end up in > high bin */
    unsigned int hi;

    /** @brief Bins (where counts are stored) */
    histogram_bin_t * bins;

    /** @brief Actual number of bins in the histogram */
    unsigned int num_bins;

    /** @brief Order (ie 2^order) for width of bin */
    unsigned char order;

    /** @brief Has this histogram been initialized? */
    unsigned int initialized;

} histogram_t;

/**
 * @brief Initialize histogram
 * @param hist [in] Histogram object
 * @param lo [in] Low bound
 * @param hi [in] Upper bound
 * @param bins [in] Array for storage of counts
 * @param n [in] Number of elements in counts array
 */
extern void histogram_init (
    histogram_t *  hist,
    unsigned int   lo,
    unsigned int   hi,
    histogram_bin_t * bins,
    unsigned int      n,
    unsigned char     order
);

/**
 * @brief Is this histogram initialized?
 * @param hist [in] Histogram object
 * @return unsigned int 0 - uninitialized, 1 - initialized
 */
static inline unsigned int histogram_is_initialized (histogram_t const * hist)
{
    return hist->initialized;
}

/**
 @brief Put value into the histogram
 @param [in] hist Histogram object
 @param [in] value Value to put
*/
static inline void histogram_put (histogram_t * hist, unsigned int value)
{
    if ( value < hist->lo )
    {
        hist->bins[ 0 ].count++;
    }
    else if ( value > hist->hi )
    {
        hist->bins[ hist->num_bins - 1 ].count++;
    }
    else
    {
        value -= hist->lo;
        hist->bins[ ( value >> hist->order ) + 1 ].count++;
    }
}


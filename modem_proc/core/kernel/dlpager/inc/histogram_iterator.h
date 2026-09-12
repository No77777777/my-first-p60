#pragma once
/**
   @file histogram_iterator.h 
   @author anandj 
   @details Iterator for histogram_t 
 */
#include "histogram.h"

/** @brief Histogram interator abstract datatype */
typedef struct
{
    /** @brief Index of bin being "pointed to" */
    unsigned int idx;

    /** @brief Low value (inclusive) of current bin */
    unsigned int lo;

    /** @brief High value (exclusive) of current bin */
    unsigned int hi;

    /** @brief Count in current bin */
    unsigned int count;

    /** @brief The histogram. This iterator is a "friend" of histogram_t */
    histogram_t const * hist;
    
} histogram_iterator_t;

/**
 * @brief Initialize iterator
 * @param i [in] Iterator object to initialize
 * @param hist [in] Histogram to iterate over 
 * @warning MUST be called ONCE on an iterator being using other functions 
 */
static inline void histogram_iterator_init (histogram_iterator_t * i, histogram_t const * hist)
{
    i->idx  = 0;
    i->hist = hist;
}

/**
 * @brief Get value of current iterator
 * @param i [in] Iterator object
 * @return histogram_iterator_t const* 
 */
static inline histogram_iterator_t const * histogram_iterator_get (histogram_iterator_t * i)
{
    if ( i->idx == 0 )
    {
        i->lo = 0;
        i->hi = i->hist->lo;
    }
    else if ( i->idx >= ( i->hist->num_bins - 1 ) )
    {
        i->lo = i->hist->hi;
        i->hi = ~0U;
    }
    else
    {
        i->lo = i->hist->lo + ( HISTOGRAM_BIN_RANGE( i->hist ) * ( i->idx -1 ) );
        i->hi = HISTOGRAM_MIN( i->hist->hi, i->lo + HISTOGRAM_BIN_RANGE( i->hist ) );
    }

    i->count = i->hist->bins[ i->idx ].count;

    return i;
}

/**
 * @brief Reset iterator
 * @param i [in] Iterator object
 * @return histogram_iterator_t const* 
 * @warning MAY be called MULTIPLE times on an iterator object 
 */
static inline histogram_iterator_t const * histogram_iterator_reset (histogram_iterator_t * i)
{
    if ( ! histogram_is_initialized( i->hist ) )
        return 0;

    i->idx = 0;
    return histogram_iterator_get( i );
}

/**
 * @brief Iterate to next element
 * @param i [in] Iterator object
 * @return histogram_iterator_t const* or 0 if no more elements
 */
static inline histogram_iterator_t const * histogram_iterator_next (histogram_iterator_t * i)
{
    if ( ! histogram_is_initialized( i->hist ) )
        return 0;

    if ( i->idx >= ( i->hist->num_bins - 1 ) )
        return 0;

    i->idx++;
    return histogram_iterator_get( i );
}


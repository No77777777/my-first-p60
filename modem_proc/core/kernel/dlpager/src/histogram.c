#include "histogram.h"
#include <string.h>

void histogram_init (
    histogram_t *  hist,
    unsigned int   lo,
    unsigned int   hi,
    histogram_bin_t * bins,
    unsigned int      n,
    unsigned char     order
)
{
    memset( hist, 0, sizeof( *hist ) );

    hist->lo = lo;
    hist->hi = hi;
    hist->bins = bins;
    hist->order = order;

    hist->num_bins = ( hi - lo ) >> hist->order;
    if ( ( hi - lo ) % HISTOGRAM_BIN_RANGE( hist ) )
    {
        hist->num_bins++;
    }

    /* For less than lo and greater than hi bins */
    hist->num_bins += 2;

    if ( hist->num_bins <= n )
    {
        unsigned int i;

        /* Memory for bins sufficient  */
        hist->initialized = 1;
        hist->bins[ 0 ].lo_inclusive = 0;
        hist->bins[ 0 ].hi_exclusive = hist->lo;
        hist->bins[ 0 ].count = 0;

        for ( i = 1; i < hist->num_bins - 1; i++ )
        {
            hist->bins[ i ].lo_inclusive = hist->lo + ( HISTOGRAM_BIN_RANGE( hist ) * ( i - 1) );
            hist->bins[ i ].hi_exclusive = HISTOGRAM_MIN( hist->hi, hist->bins[ i ].lo_inclusive + HISTOGRAM_BIN_RANGE( hist ) );
            hist->bins[ i ].count = 0;
        }

        hist->bins[ hist->num_bins - 1 ].lo_inclusive = hist->hi;
        hist->bins[ hist->num_bins - 1 ].hi_exclusive = ~0U;
        hist->bins[ hist->num_bins - 1 ].count = 0;
    }
}


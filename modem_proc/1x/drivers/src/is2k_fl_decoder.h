#ifndef IS2K_FL_DECODER_H
#define IS2K_FL_DECODER_H
/*!
  \mainpage IS-2000 Forward Link Decoder Module Public API
*/
/*=============================================================================

Copyright(c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/drivers/src/is2k_fl_decoder.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
05/23/14   trc     Add is2k_fl_decoder_get_info() API
04/22/14   trc     Initial version

=============================================================================*/

/* External interfaces */
#include <stddef.h>
#include <stdint.h>

/*! Supported decoder channel configurations */
typedef enum {
  IS2K_FL_SYNC,
  IS2K_FL_PCH_9600,
  IS2K_FL_PCH_4800,
  IS2K_FL_RC1_FCH_9600,
  IS2K_FL_RC1_FCH_4800,
  IS2K_FL_RC1_FCH_2400,
  IS2K_FL_RC1_FCH_1200,
  IS2K_FL_RC2_FCH_14400,
  IS2K_FL_RC2_FCH_7200,
  IS2K_FL_RC2_FCH_3600,
  IS2K_FL_RC2_FCH_1800,
  IS2K_FL_RC3_FCH_9600,
  IS2K_FL_RC3_FCH_4800,
  IS2K_FL_RC3_FCH_2700,
  IS2K_FL_RC3_FCH_1500,
  IS2K_FL_RC4_FCH_9600,
  IS2K_FL_RC4_FCH_4800,
  IS2K_FL_RC4_FCH_2700,
  IS2K_FL_RC4_FCH_1500,
  IS2K_FL_RC5_FCH_14400,
  IS2K_FL_RC5_FCH_7200,
  IS2K_FL_RC5_FCH_3600,
  IS2K_FL_RC5_FCH_1800,
  IS2K_FL_RC11_FCH_9600,
  IS2K_FL_RC11_FCH_5000,
  IS2K_FL_RC11_FCH_3000,
  IS2K_FL_RC11_FCH_1800,

  IS2K_FL_CHAN_TYPE_MAX
} is2k_fl_chan_type;

/*! Opaque decoder object type for use as a handle by decoder client */
typedef struct is2k_fl_decoder_struct is2k_fl_decoder_type;

/*! Soft decision symbols are expected in signed 8-bit integer format */
typedef int8_t is2k_fl_symbol_type;

/*! Output frame structure, contains output and details of decode operation */
typedef struct {
  size_t num_bits;
  size_t symbol_energy;
  size_t symbol_errors;
  uint8_t *pOutBuffer;
} is2k_fl_frame_type;

/*! Decoder informational structure */
typedef struct {
  is2k_fl_chan_type channel;
  size_t max_num_symbols;
} is2k_fl_decoder_info_type;

/*! Create a new decoder object for a given channel type */
is2k_fl_decoder_type *is2k_fl_decoder_new( is2k_fl_chan_type channel );

/*! Release a decoder object created by is2k_fl_decoder_new() */
void is2k_fl_decoder_free( is2k_fl_decoder_type *pDecoder );

/*! Decode a frame of soft-decision symbols, given a decoder handle and
    input symbols */
is2k_fl_frame_type *is2k_fl_decode_frame( is2k_fl_decoder_type *pDecoder,
                                          is2k_fl_symbol_type *pSymbols,
                                          size_t num_symbols );

/*! Reset the Viterbi decoder and initialize state metrics accordingly 
    for the current mode of operation */
void is2k_fl_decoder_reset( is2k_fl_decoder_type *pDecoder );

/*! Get any useful decoder information */
is2k_fl_decoder_info_type
  *is2k_fl_decoder_get_info( is2k_fl_decoder_type *pDecoder );

/*! Print out the profiling measurements of the latest decode and
    return the overall duration in units determined by timestamp delta
    conversion function: is2k_fl_timestamp_delta_ext() */
size_t is2k_fl_decode_perfdump( is2k_fl_decoder_type *pDecoder );

#endif /* IS2K_FL_DECODER_H */

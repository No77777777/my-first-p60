#ifndef MEDIA_FMT_API_H
#define MEDIA_FMT_API_H

/*==============================================================================
  @file media_fmt_api.h
  @brief This file contains media format IDs and definitions

Copyright (c) 2019 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/

/*==============================================================================
                       EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $header:

  when       who        what, where, why
  --------   --- ------------------------------------------------------
  05/31/18   rbhatnk         Initial Draft
  03/26/19   pbramhas        h2xml changes
==============================================================================*/

/*------------------------------------------------------------------------------
 *  Header Includes
 *----------------------------------------------------------------------------*/
#include "module_cmn_api.h"

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

/** @h2xml_title1           {Media Format APIs}
    @h2xml_title_agile_rev  {Media Format APIs}
    @h2xml_title_date       {August 13, 2018} */
/**
   @h2xmlx_xmlNumberFormat {int}
*/


/** Infinity */
#define INFINITE                         -1

/** Zero is invalid value */
#define INVALID_VALUE                     0

/*********************************************** Channel Map Values ***************************************************/
/** Front left channel. */
#define PCM_CHANNEL_L                     1

/** Front right channel. */
#define PCM_CHANNEL_R                     2

/** Front center channel. */
#define PCM_CHANNEL_C                     3

/** Left surround channel.*/
#define PCM_CHANNEL_LS                    4

/** Right surround channel. */
#define PCM_CHANNEL_RS                    5

/** Low frequency effect channel. */
#define PCM_CHANNEL_LFE                   6

/** Center surround channel;
 * rear center channel. */
#define PCM_CHANNEL_CS                    7

/** Center back channel. */
#define PCM_CHANNEL_CB                    PCM_CHANNEL_CS

/** Left back channel;
 * rear left channel. */
#define PCM_CHANNEL_LB                    8

/** Right back channel;
 * rear right channel. */
#define PCM_CHANNEL_RB                    9

/** Top surround channel. */
#define PCM_CHANNEL_TS                    10

/** Center vertical height channel. */
#define PCM_CHANNEL_CVH                   11

/** Top front center channel. */
#define PCM_CHANNEL_TFC                   PCM_CHANNEL_CVH

/** Mono surround channel. */
#define PCM_CHANNEL_MS                    12

/** Front left of center channel. */
#define PCM_CHANNEL_FLC                   13

/** Front right of center channel. */
#define PCM_CHANNEL_FRC                   14

/** Rear left of center channel. */
#define PCM_CHANNEL_RLC                   15

/** Rear right of center channel. */
#define PCM_CHANNEL_RRC                   16

/** Secondary low frequency effect channel. */
#define PCM_CHANNEL_LFE2                  17

/** Side left channel. */
#define PCM_CHANNEL_SL                    18

/** Side right channel. */
#define PCM_CHANNEL_SR                    19

/** Top front left channel. */
#define PCM_CHANNEL_TFL                   20

/** Left vertical height channel. */
#define PCM_CHANNEL_LVH                   PCM_CHANNEL_TFL

/** Top front right channel. */
#define PCM_CHANNEL_TFR                   21

/** Right vertical height channel. */
#define PCM_CHANNEL_RVH                   PCM_CHANNEL_TFR

/** Top center channel. */
#define PCM_CHANNEL_TC                    22

/** Top back left channel. */
#define PCM_CHANNEL_TBL                   23

/** Top back right channel. */
#define PCM_CHANNEL_TBR                   24

/** Top side left channel. */
#define PCM_CHANNEL_TSL                   25

/** Top side right channel. */
#define PCM_CHANNEL_TSR                   26

/** Top back center channel. */
#define PCM_CHANNEL_TBC                   27

/** Bottom front center channel. */
#define PCM_CHANNEL_BFC                   28

/** Bottom front left channel. */
#define PCM_CHANNEL_BFL                   29

/** Bottom front right channel. */
#define PCM_CHANNEL_BFR                   30

/** Left wide channel. */
#define PCM_CHANNEL_LW                    31

/** Right wide channel. */
#define PCM_CHANNEL_RW                    32

/** Left side direct channel. */
#define PCM_CHANNEL_LSD                   33

/** Right side direct channel. */
#define PCM_CHANNEL_RSD                   34

/** Channel map 48 to 63 are reserved for custom channel maps */
#define PCM_CUSTOM_CHANNEL_MAP_1          48
#define PCM_CUSTOM_CHANNEL_MAP_2          49
#define PCM_CUSTOM_CHANNEL_MAP_3          50
#define PCM_CUSTOM_CHANNEL_MAP_4          51
#define PCM_CUSTOM_CHANNEL_MAP_5          52
#define PCM_CUSTOM_CHANNEL_MAP_6          53
#define PCM_CUSTOM_CHANNEL_MAP_7          54
#define PCM_CUSTOM_CHANNEL_MAP_8          55
#define PCM_CUSTOM_CHANNEL_MAP_9          56
#define PCM_CUSTOM_CHANNEL_MAP_10         57
#define PCM_CUSTOM_CHANNEL_MAP_11         58
#define PCM_CUSTOM_CHANNEL_MAP_12         59
#define PCM_CUSTOM_CHANNEL_MAP_13         60
#define PCM_CUSTOM_CHANNEL_MAP_14         61
#define PCM_CUSTOM_CHANNEL_MAP_15         62
#define PCM_CUSTOM_CHANNEL_MAP_16         63

#define PCM_MAX_CHANNEL_MAP               63

/*********************************************** Bits Per Sample Values ***********************************************/
/* Bits per sample (= sample word size) */
#define BITS_PER_SAMPLE_16        16
#define BITS_PER_SAMPLE_24        24
#define BITS_PER_SAMPLE_32        32

/********************************************** Bytes Per Sample Values ***********************************************/
/* Bytes per sample */
#define BYTES_PER_SAMPLE_TWO              2
#define BYTES_PER_SAMPLE_THREE            3
#define BYTES_PER_SAMPLE_FOUR             4

/********************************************** Bit Width Values ******************************************************/
/* Bit width (actual width of the sample in a word) */
#define BIT_WIDTH_16                16
#define BIT_WIDTH_24                24
#define BIT_WIDTH_32                32

/********************************************** Alignment Values ******************************************************/
/**
 * Alignment
 */
#define PCM_LSB_ALIGNED                   1
#define PCM_MSB_ALIGNED                   2

/********************************************** Q Factor Values *******************************************************/
/**
 * Q factors
 */
#define PCM_Q_FACTOR_15                   15
#define PCM_Q_FACTOR_23                   23
#define PCM_Q_FACTOR_27                   27
#define PCM_Q_FACTOR_31                   31

/** Shift factor for Q31 <=> Q28 conversion for 32-bit P
*/
#define PCM_QFORMAT_SHIFT_FACTOR         (PCM_Q_FACTOR_31 - PCM_Q_FACTOR_27)

/********************************************** Sampling Rates ********************************************************/
/** Sample rate is 8 kHz. */
#define SAMPLE_RATE_8K                    8000

/** Sample rate is 11.025 kHz. */
#define SAMPLE_RATE_11_025K               11025

/** Sample rate is 12 kHz. */
#define SAMPLE_RATE_12K                   12000

/** Sample rate is 16 kHz. */
#define SAMPLE_RATE_16K                   16000

/** Sample rate is 22.05 kHz. */
#define SAMPLE_RATE_22_05K                22050

/** Sample rate is 24 kHz. */
#define SAMPLE_RATE_24K                   24000

/** Sample rate is 32 kHz. */
#define SAMPLE_RATE_32K                   32000

/** Sample rate is 44.1 kHz. */
#define SAMPLE_RATE_44_1K                 44100

/** Sample rate is 48 kHz. */
#define SAMPLE_RATE_48K                   48000

/** Sample rate is 88.2 kHz. */
#define SAMPLE_RATE_88_2K                 88200

/** Sample rate is 96 kHz. */
#define SAMPLE_RATE_96K                   96000

/** Sample rate is 176.4 kHz.*/
#define SAMPLE_RATE_176_4K                176400

/** Sample rate is 192 kHz. */
#define SAMPLE_RATE_192K                  192000

/** Sample rate is 352.8 kHz. */
#define SAMPLE_RATE_352_8K                352800

/** Sample rate is 384 kHz. */
#define SAMPLE_RATE_384K                  384000

/********************************************** Endianess Values*******************************************************/
/**
 * Endianness
 */
#define PCM_LITTLE_ENDIAN                 1
#define PCM_BIG_ENDIAN                    2

/********************************************** Interleaving Values*****************************************************/
/**
 * Interleaved PCM
 */
#define PCM_INTERLEAVED                   1
/**
 * Packed Deinterleaved PCM:
 * A buffer of max size M with C channels and N/C actual bytes per channel
 * is deinterleaved-packed if (M - N) is zero.
 */
#define PCM_DEINTERLEAVED_PACKED          2
/**
 * Unpacked Deinterleaved PCM:
 * A buffer of max size M with C channels and N/C actual bytes per channel
 * is deinterleaved-unpacked if (M - N) is nonzero.
 * OR each channel has its own buffers with actual length being less than max length.
 */
#define PCM_DEINTERLEAVED_UNPACKED        3

/********************************************** Data Formats **********************************************************/

/** Data format is fixed point */
#define DATA_FORMAT_FIXED_POINT           1

/** Data format is IEC61937 packetized stream
 * Data has properties such as sample rate, number of channels, bit-width like PCM etc */
#define DATA_FORMAT_IEC61937_PACKETIZED   2

/** Data format is IEC60958 packetized stream for PCM only
 * Data has properties such as sample rate, number of channels, bit-width like PCM etc*/
#define DATA_FORMAT_IEC60958_PACKETIZED   3

/** Data format is DSD over PCM stream
 * Data has properties such as sample rate, number of channels, bit-width like PCM etc*/
#define DATA_FORMAT_DSD_OVER_PCM          4

/** Data format is generic compressed stream
 * Data has properties such as sample rate, number of channels, bit-width like PCM etc*/
#define DATA_FORMAT_GENERIC_COMPRESSED    5

/** Data format is raw compressed stream.
 * Data is raw compressed bit stream. Data does NOT have properties
 * such as sample rate, number of channels and bit-width like PCM */
#define DATA_FORMAT_RAW_COMPRESSED        6

/** Compressed bitstreams packetized like PCM using a Qualcomm-designed packetizer*/
#define DATA_FORMAT_COMPR_OVER_PCM_PACKETIZED     7

/** Data format is IEC60958 packetized stream for compressed streams
 * Data has properties such as sample rate, number of channels, bit-width like PCM etc*/
#define DATA_FORMAT_IEC60958_PACKETIZED_NON_LINEAR 8

/********************************************* Vocoder Rates ************************************************/

/** Enumerations for various vocoder rates */

/** Enumeration for invalid vocoder Rate **/
#define VOCODER_INVALID_RATE          0

/** Enumeration for 1/8 vocoder rate.*/
#define VOCODER_ONE_EIGHTH_RATE       1

/** Enumeration for 1/4 vocoder rate. */
#define VOCODER_QUARTER_RATE       2

/** Enumeration for 1/2 vocoder rate. */
#define VOCODER_HALF_RATE             3

/** Enumeration for full vocoder rate. */
#define VOCODER_FULL_RATE             4

/********************************************** Configuration Modes****************************************************/
/** Configured parameter (like bps, num_channels, etc) will be a
   don't care - The module should continue to use the previously
   set configuration (or module defaults).
   E.g. if client wants to control num_channels from host but bits per sample from ACDB, then
   ACDB will have the value of -2. After graph-open, host can issue command with bits per sample as -2
   and num_channels with proper value*/
#define PARAM_VAL_UNSET     (-2)

/** Configured parameter (like bps, num_channels, etc) will need to follow
    the input media format that the module will receive or has received. */
#define PARAM_VAL_NATIVE    (-1)

/** Configured parameter (like bps, num_channels, etc) is considered invalid
    and the set_param should error out.
    This is used to detect explicit initialization vs. initialization by zero memset.*/
#define PARAM_VAL_INVALID     0

#define PARAM_ID_LIB_VERSION                                   0x00010937

/* Structure for Querying module lib version of any Audio processing modules. */
typedef struct lib_version_t lib_version_t;
/** @h2xmlp_parameter   {"PARAM_ID_LIB_VERSION", PARAM_ID_LIB_VERSION}
    @h2xmlp_description {To query the lib version of any audio processing module.}
    @h2xmlp_toolPolicy  {RTC_READONLY}
    @h2xmlp_readOnly    {true}*/

#include "gk_begin_pack.h"
#include "gk_begin_pragma.h"

/* Payload of the PARAM_ID_LIB_VERSION parameter used by
 any Audio Processing module
 */
struct lib_version_t
{
   uint32_t lib_version_low;
   /**< @h2xmle_description  {Version of the module LSB.} */


   uint32_t lib_version_high;
    /**< @h2xmle_description  { Version of the module MSB} */

}
#include "gk_end_pragma.h"
#include "gk_end_pack.h"
;

/************************* IEC61937_PACKETIZED and IEC60958_PACKETIZED data format ************************************/
#include "gk_begin_pack.h"
#include "gk_begin_pragma.h"

/**
 * Payload of data_format = DATA_FORMAT_IEC61937_PACKETIZED or DATA_FORMAT_IEC60958_PACKETIZED
 * Contains media format information independent of payload for packetized media_fmt_id
 */
struct payload_data_fmt_iec_packetized_t
{
   uint32_t sample_rate;
   /**< Number of samples per second.

        @values 0 to 192000 Hz */
   /**< @h2xmle_description {Sampling rate of audio stream}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"44.1 kHz"=44100;
                             "48 kHz"=48000;
                             "88.2 kHz"=88200;
                             "96 kHz"=96000;
                             "176.4 kHz"=176400;
                             "192 kHz"=192000}
        @h2xmle_policy      {Basic} */

   uint16_t num_channels;
   /**< @h2xmle_description {Number of channels}
        @h2xmle_default     {0}
        @h2xmle_rangeList   { "TWO"=2;
                              "EIGHT"=8 }
        @h2xmle_policy      {Basic} */

   uint16_t reserved;
   /**< @h2xmle_description {Used for alignment; must be set to 0.}
        @h2xmle_policy      {Basic} */
}
#include "gk_end_pragma.h"
#include "gk_end_pack.h"
;
typedef struct payload_data_fmt_iec_packetized_t payload_data_fmt_iec_packetized_t;


/*********************************** Encoder Configuration Block (Encoder Output Config) ******************************/
/**
 * Param ID used to configure encoder
 *
 * All encoders may not support encoder output config
 *
 */
#define PARAM_ID_ENCODER_OUTPUT_CONFIG                   0x08001009


/** @h2xmlp_parameter   {"PARAM_ID_ENCODER_OUTPUT_CONFIG", PARAM_ID_ENCODER_OUTPUT_CONFIG}
    @h2xmlp_description {Param ID used to configure encoder.
                         Encoding happens at the incoming sample rate, channels and bit width.
                         These are controlled by either resamplers, MFC, channel mixer, or PCM converter.
                         Overall struct contains\n
                         - \n
                         --     param_id_encoder_output_config_t cfg;     \n
                         --     uint8_t custom_enc_cfg[payload_size];\n
                         --     uint8_t padding[if_any];\n
                         -\n
                         }
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
#include "gk_begin_pragma.h"
/**
 * Structure for PARAM_ID_ENCODER_OUTPUT_CONFIG
 *
 * overall struct is
 * {
 *    param_id_encoder_output_config_t cfg;
 *    uint8_t custom_enc_cfg[payload_size];
 *    uint8_t padding[if_any];
 * }
 *
 * Encoding happens at the incoming sample rate, channels and bit width.
 * These are controlled by either resamplers, MFC, channel mixer, or PCM converter.
 *
 */
struct param_id_encoder_output_config_t
{
   uint32_t      data_format;
    /**< @h2xmle_description {Format of the data}
         @h2xmle_default     {0}
         @h2xmle_rangeList   {"PARAM_VAL_INVALID"=0,
                              "DATA_FORMAT_FIXED_POINT"=1}
         @h2xmle_policy      {Basic} */


   uint32_t       fmt_id;
    /**< @h2xmle_description {Format ID of the data stream.
                              For PCM encoding use case this must match the fmt_id in PARAM_ID_PCM_OUTPUT_FORMAT_CFG.}
         @h2xmle_default     {0}
         @h2xmle_rangeList   {"INVALID_VALUE"=0}
		              "Media format ID of PCM"=MEDIA_FMT_ID_PCM
                              "Media format ID of AAC"=MEDIA_FMT_ID_AAC
         @h2xmle_policy      {Basic} */

   uint32_t       payload_size;
    /**< @h2xmle_description {Size of the custom payload that follows this structure\n
                              the struct of payload is defined by combination of data_format and fmt_id.
                              E.g. PCM fixed point (payload_media_fmt_pcm_t) and floating point may have different payloads.
                              This size does not include bytes added for 32-bit alignment}
         @h2xmle_default     {0}
         @h2xmle_range       {0..0xFFFFFFFF}
         @h2xmle_policy      {Basic} */
   uint8_t  payload[0];
    /**<
         @h2xmle_description {Payload}
         h2xmle_rangeList   {"For fmt_id = PARAM_ID_PCM_OUTPUT_FORMAT_CFG and data_format=DATA_FORMAT_FIXED_POINT"=0,
                   "For fmt_id = PARAM_ID_ENCODER_OUTPUT_CONFIG and data_format=DATA_FORMAT_RAW_COMPRESSED"=1}
         @h2xmle_policy      {Basic}
         h2xmle_variableArraySize { (payload_size*4+3) / 4}*/
}
#include "gk_end_pragma.h"
#include "gk_end_pack.h"
;
typedef struct param_id_encoder_output_config_t param_id_encoder_output_config_t;

/*************************************************** PCM Media Format *************************************************/
/**
 * Media format ID for identifying PCM streams.
 */
#define MEDIA_FMT_ID_PCM                  0x09001000

#include "gk_begin_pack.h"
#include "gk_begin_pragma.h"

/**
 * Payload of fmt-id = MEDIA_FMT_ID_PCM and data_format = DATA_FORMAT_FIXED_POINT
 *
 * struct complete_media_fmt_pcm_t
 * {
 *    payload_media_fmt_pcm_t pcm;
 *    uint8_t ch_map[num_channels];
 * }
 */
struct payload_media_fmt_pcm_t
{
   uint32_t sample_rate;
   /**< Number of samples per second.

        @values 0 to 384000 Hz */
   /**< @h2xmle_description {Sampling rate of audio stream}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"8 kHz"=8000;
                             "11.025 kHz"=11025;
                             "12 kHz"=12000;
                             "16 kHz"=16000;
                             "22.05 kHz"=22050;
                             "24 kHz"=24000;
                             "32 kHz"=32000;
                             "44.1 kHz"=44100;
                             "48 kHz"=48000;
                             "88.2 kHz"=88200;
                             "96 kHz"=96000;
                             "176.4 kHz"=176400;
                             "192 kHz"=192000;
                             "352.8 kHz"=352800;
                             "384 kHz"=384000}
        @h2xmle_policy      {Basic} */

   uint16_t bit_width;
   /**< @h2xmle_description {bit width of each sample. E.g. 16 bit, 24 bit or 32 bit}
        @h2xmle_default     {0}
        @h2xmle_rangeList   { "INVALID_VALUE"=0;
                              ""=16;
                              ""=24;
                              ""=32}
        @h2xmle_policy      {Basic} */

   uint16_t alignment;
   /**< @h2xmle_description {Indicates the alignment of bits_per_sample in sample_word_size. \n
                             Relevant only when bits_per_sample is 24 and word_size is 32}
        @h2xmle_default     {0}
        @h2xmle_rangeList   { "INVALID_VALUE"=0;
                              "PCM_LSB_ALIGNED"=1;
                              "PCM_MSB_ALIGNED"=2}
        @h2xmle_policy      {Basic} */


   uint16_t bits_per_sample;
   /**<
        @h2xmle_description {Bits needed to store one sample.
                             - 16-bits per sample always contain 16-bit samples.
                             - 24-bits per sample always contain 24-bit samples.
                             - 32-bits per sample have below cases:
                               - If bit width = 24 and alignment = LSB aligned, then
                                  24-bit samples are placed in the lower 24 bits of a 32-bit word.
                                  Upper bits may or may not be sign-extended.
                               - If bit width = 24 and alignment = MSB aligned, then
                                  24-bit samples are placed in the upper 24 bits of a 32-bit word.
                                  Lower bits may or may not be zeroed.
                               - If bit width = 32, 32-bit samples are placed in the
                                 32-bit words
                            }
        @h2xmle_default     {0}
        @h2xmle_rangeList   { "INVALID_VALUE"=0;
                              ""=16;
                              ""=24;
                              ""=32
                            }
        @h2xmle_policy      {Basic} */

   uint16_t q_factor;
   /**< @h2xmle_description {Q factor of the PCM data.
                             15 for 16 bit signed data
                             23 for 24 bit signed packed (24 word size) data
                             27 for LSB aligned 24 bit unpacked (32 word size) signed data used internal to Gecko.
                             31 for MSB aligned 24 bit unpacked (32 word size) signed data
                             24 for LSB aligned 24 bit unpacked (32 word size) signed data
                             31 for 32 bit signed data}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {
                              "INVALID_VALUE"=0;
                              ""=15;
                              ""=23;
                              ""=24;
                              ""=27;
                              ""=31
                            }
        @h2xmle_policy      {Basic} */


   uint16_t endianness;
   /**< @h2xmle_description {Indicates whether PCM samples are stored in little endian or big endian format.}
        @h2xmle_default     {0}
        @h2xmle_rangeList   { "INVALID_VALUE"=0;
                              "PCM_LITTLE_ENDIAN"=1;
                              "PCM_BIG_ENDIAN"=2
                            }
        @h2xmle_policy      {Basic} */

   uint16_t num_channels;
   /**< @h2xmle_description {Number of channels}
        @h2xmle_default     {0}
        @h2xmle_range       { 0..32 }
        @h2xmle_policy      {Basic} */


   uint8_t channel_mapping[0];
   /**< @h2xmle_description {Channel mapping array of variable size.
                             Size of this array depends upon number of channels.
                             Channel[i] mapping describes channel i. Each element i of the array
                             describes channel i inside the buffer where i is less than num_channels.
                             An unused channel is set to 0.}
        h2xmle_variableArraySize {(num_channels*4 + 3) / 4}
        @h2xmle_default     {0}
        @h2xmle_policy      {Basic} */
}
#include "gk_end_pragma.h"
#include "gk_end_pack.h"
;
typedef struct payload_media_fmt_pcm_t payload_media_fmt_pcm_t;

/*************************************************** AAC Media Format *************************************************/
/**
 * Media format ID for AAC, which identifies both MPEG2 and
 * MPEG4 AAC formats.
 */
#define MEDIA_FMT_ID_AAC                  0x09001001

/** Enumeration for the audio data transport stream AAC format. */
#define AAC_FORMAT_FLAG_ADTS 0

/** Enumeration for the low overhead audio stream AAC format. */
#define AAC_FORMAT_FLAG_LOAS 1

/** Enumeration for the audio data interchange format AAC format. */
#define AAC_FORMAT_FLAG_ADIF 2

/** Enumeration for the raw AAC format. */
#define AAC_FORMAT_FLAG_RAW 3

/** Enumeration for the low overhead audio transport multiplex AAC format. */
#define AAC_FORMAT_FLAG_LATM 4

/** Enumeration for the AAC low complexity audio object type:
    - When used for aac_fmt_flag in payload_media_fmt_aac_t, this parameter
      specifies that the AOT in the AAC stream is LC.
    - When used for enc_mode in payload_enc_cfg_aac_t, this parameter specifies
      that the Encoding mode to be used is LC.
*/
#define AAC_AOT_LC 2

/** Enumeration for the AAC spectral band replication AOT:
    - When used for aac_fmt_flag in payload_media_fmt_aac_t, this parameter
      specifies that the AOT in the AAC stream is SBR.
    - When used for enc_mode in payload_enc_cfg_aac_t, this parameter specifies
      that the Encoding mode to be used is AAC+.
*/
#define AAC_AOT_SBR 5

/** Enumeration for the AAC parametric stereo AOT:
    - When used for aac_fmt_flag in payload_media_fmt_aac_t, this parameter
      specifies that the AOT in the AAC stream is PS.
    - When used for enc_mode in payload_enc_cfg_aac_t, this parameter specifies
      that the Encoding mode to be used is eAAC+.
*/
#define AAC_AOT_PS 29

/** Enumeration for the bit-sliced arithmetic coding AOT:
    - When used for aac_fmt_flag in payload_media_fmt_aac_t, this parameter
      specifies that the AOT in the AAC stream is BSAC.
    - This parameter must not be used for enc_mode in
      payload_enc_cfg_aac_t. BSAC encoding is not supported.
*/
#define AAC_AOT_BSAC 22

/** AOT=17 is the ER AAC-LC format, which currently is not  supported. */

typedef struct payload_media_fmt_aac_t payload_media_fmt_aac_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload of the AAC decoder format block parameters in the
                       MEDIA_FMT_ID_AAC media format. If the total size of PCE > 0 bits, the PCE information follows the
                       structure. In this case, the client must pad the additional bits for 32-bit alignment}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct payload_media_fmt_aac_t
{
   uint16_t aac_fmt_flag;
   /**< @h2xmle_description {Bit stream format option.}
        @h2xmle_rangeList   {"AAC_FORMAT_FLAG_ADTS"=0,
                             "AAC_FORMAT_FLAG_LOAS"=1,
                             "AAC_FORMAT_FLAG_ADIF"=2,
                             "AAC_FORMAT_FLAG_RAW"=3,
                             "AAC_FORMAT_FLAG_LATM"=4}
        @h2xmle_policy      {Basic} */

   uint16_t audio_obj_type;
   /**< @h2xmle_description {Audio Object Type (AOT) present in the AAC stream.}
        @h2xmle_default     {AAC_AOT_LC}
        @h2xmle_rangeList   {"AAC_AOT_LC"=2,
                             "AAC_AOT_SBR"=5,
                             "AAC_AOT_BSAC"=22,
                             "AAC_AOT_PS"=29}
        @h2xmle_policy      {Basic} */

   uint16_t num_channels;
   /**< @h2xmle_description {Number of channels present in the AAC stream.}
        @h2xmle_rangeList   {"PCE"=0,
                             "Mono"=1,
                             "Stereo"=2,
                             "5.1 content"=6}
        @h2xmle_policy      {Basic} */

   uint16_t total_size_of_PCE_bits;
   /**< @h2xmle_description {For RAW formats and if channel_config=0 (PCE), the client can send
                             the bit stream containing PCE immediately following this structure
                             (in band)
                             If this field is set to 0, the PCE information is assumed to be
                             available in the audio bit stream and not in band.
                             If this field is greater than 0, the PCE information follows this
                             structure. Additional bits might be required for 32-bit alignment.}
        @h2xmle_range       {0...0xFFFF}
        @h2xmle_policy      {Basic} */

   uint32_t sample_rate;
   /**< @h2xmle_description {Number of samples per second. This field must be equal to the sample
                             rate of the AAC-LC decoder output.
                             - For MP4 or 3GP containers, this sample rate is indicated by the
                               samplingFrequencyIndex field in the AudioSpecificConfig element.
                             - For ADTS format, this sample rate is indicated by the
                               samplingFrequencyIndex in the ADTS fixed header.
                             - For ADIF format, this sample rate is indicated by the
                               samplingFrequencyIndex in the program_config_element present in the
                               ADIF header.}
    @h2xmle_default     {0}
        @h2xmle_rangeList   {INVALID_VALUE=0, 8000, 11025, 12000, 16000, 22050, 24000, 32000,
                             44100, 48000, 64000, 88200, 96000}
        @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

typedef struct aac_enc_cfg_t aac_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the AAC encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct aac_enc_cfg_t
{
   uint16_t enc_mode;
   /**<
        @h2xmle_description {AAC encoding mode}
        @h2xmle_default     {AAC_AOT_LC}
        @h2xmle_rangeList   { "AAC_AOT_LC"=2;
                              "AAC_AOT_SBR"=5;
                              "AAC_AOT_PS"=29 }
        @h2xmle_policy      {Basic} */

   uint16_t aac_fmt_flag;
   /**<
        @h2xmle_description {AAC format flag}
        @h2xmle_default     {AAC_FORMAT_FLAG_ADTS}
        @h2xmle_rangeList   { "AAC_FORMAT_FLAG_ADTS"=0;
                              "AAC_FORMAT_FLAG_LOAS"=1;
                              "AAC_FORMAT_FLAG_RAW" =3;
                              "AAC_FORMAT_FLAG_LATM"=4 }
        @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

/*************************************************** AC3/EAC3 Media Format ********************************************/
/**
 * Media format ID for AC3 streams
 */
#define MEDIA_FMT_ID_AC3    0x09001018

/**
 * Media format ID for EAC3 streams
 */
#define MEDIA_FMT_ID_EAC3   0x09001017

/*************************************************** DTS Media Format ********************************************/
#define MEDIA_FMT_ID_DTS    0x09001016

/*************************************************** ATRAC Media Format ***********************************************/
#define MEDIA_FMT_ID_ATRAC  0x09001012

/*********************************************** DTHD Media Format ******************************************/
#define MEDIA_FMT_ID_DTHD   0x09001014

/*************************************************** MAT Media Format *************************************************/
#define MEDIA_FMT_ID_MAT    0x09001015

/*************************************************** ALAC Media Format ************************************************/
/**
 * Media format ID for ALAC streams
 */
#define MEDIA_FMT_ID_ALAC                 0x09001002

typedef struct payload_media_fmt_alac_t payload_media_fmt_alac_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload of the ALAC decoder format block parameters in the
                          MEDIA_FMT_ID_ALAC media format}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct payload_media_fmt_alac_t
{
  uint32_t frame_length;
  /**< @h2xmle_description {Frames per packet when no explicit frames per packet setting is
   present in the packet header. The encoder frames per packet can be explicitly set, but for maximum
   compatibility, use the default encoder setting of 4096.}
       @h2xmle_policy      {Basic} */

  uint8_t compatible_version;
  /**< @h2xmle_description {Specifies the compatible version.}
       @h2xmle_rangeList   {"0" = 0}
       @h2xmle_policy      {Basic} */

  uint8_t bit_depth;
  /**< @h2xmle_description {Bit depth of the source PCM data.}
       @h2xmle_default     {16}
       @h2xmle_rangeList   {"16" = 16,
                            "24" = 24}
       @h2xmle_policy      {Basic} */

  uint8_t pb;
  /**< @h2xmle_description {Tuning parameter. (Currently not used.)}
       @h2xmle_rangeList   {"40" = 40}
       @h2xmle_default     {40}
       @h2xmle_policy      {Basic} */

  uint8_t mb;
  /**< @h2xmle_description {Tuning parameter. (Currently not used.)}
       @h2xmle_rangeList   {"10" = 10}
       @h2xmle_default     {10}
       @h2xmle_policy      {Basic} */

  uint8_t kb;
  /**< @h2xmle_description {Tuning parameter. (Currently not used.)}
       @h2xmle_rangeList   {"14" = 14}
       @h2xmle_default     {14}
       @h2xmle_policy      {Basic} */

  uint8_t num_channels;
  /**< @h2xmle_description {Number of channels for multichannel decoding.}
       @h2xmle_rangeList   {INVALID_VALUE=0,1...8}
       @h2xmle_policy      {Basic} */

  uint16_t max_run;
  /**< @h2xmle_description {Currently not used}
       @h2xmle_rangeList   {"255" = 255}
       @h2xmle_default     {255}
       @h2xmle_policy      {Basic} */

  uint32_t max_frame_bytes;
  /**< @h2xmle_description {Maximum size of an Apple Lossless packet within the encoded stream.}
       @h2xmle_range       {0..0xFFFFFFFF}
       @h2xmle_policy      {Basic} */

  uint32_t avg_bit_rate;
  /**< @h2xmle_description {Average bit rate in bits per second of the Apple Lossless stream.}
       @h2xmle_range       {0..0xFFFFFFFF}
       @h2xmle_policy      {Basic} */

  uint32_t sample_rate;
  /**< @h2xmle_description {Number of samples per second in Hertz.}
       @h2xmle_rangeList   {INVALID_VALUE=0, "8000"=8000, "11025"=11025, "12000"=12000, 
	                    "16000"=16000, "22050"=22050, "24000"=24000, "32000"=32000,
                            "44100"=44100, "48000"=48000, "64000"=64000, "88200"=88200, 
			    "96000"=96000, "128000"=128000, "176400"=176400, "192000"=192000}
       @h2xmle_policy      {Basic} */

  /* following channel layout of the bitstream is defined in ALACAudioTypes.h)*/
  uint32_t channel_layout_tag;
  /**< @h2xmle_description {Indicates whether channel layout information is present in the
                            bit stream.0 means, Channel layout information is not present.
                            Constants are extracted when channel layout information is present}
       @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;
/*************************************************** APE Media Format *************************************************/
/**
 * Media format ID for APE streams
 */
#define MEDIA_FMT_ID_APE                  0x09001003

typedef struct payload_media_fmt_ape_t payload_media_fmt_ape_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload of the APE decoder format block parameters in the
                          MEDIA_FMT_ID_APE media format}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct payload_media_fmt_ape_t
{
    uint16_t           version;
    /**< @h2xmle_description {Specifies the compatible version.}
         @h2xmle_rangeList   {"0" = 0}
         @h2xmle_policy      {Basic} */

    uint16_t           compression_level;
    /**< @h2xmle_description {Compression level present in the encoded packet.
                              The nBlocksPerFrame parameter value is based on the compression level.}
         @h2xmle_rangeList   {"COMPRESSION_LEVEL_FAST"       = 1000,
                              "COMPRESSION_LEVEL_NORMAL"     = 2000,
                              "COMPRESSION_LEVEL_HIGH"       = 3000,
                              "COMPRESSION_LEVEL_EXTRA_HIGH" = 4000,
                              "COMPRESSION_LEVEL_INSANE"     = 5000,
                              INVALID_VALUE=0}
         @h2xmle_policy      {Basic} */

    uint32_t           format_flags;
    /**< @h2xmle_description {Reserved for future use.}
         @h2xmle_policy      {Basic} */

    uint32_t           blocks_per_frame;
    /**< @h2xmle_description {Number of audio blocks (channel samples) in one frame present in the
                              encoded packet header.}
         @h2xmle_rangeList   {"Any value present in the APE header" = 0}
         @h2xmle_policy      {Basic} */

    uint32_t           final_frame_blocks;
    /**< @h2xmle_description {Number of audio blocks (channel samples) in the final frame present
                              in the encoded packet header}
         @h2xmle_rangeList   {"Any value present in the APE header" = 0}
         @h2xmle_policy      {Basic} */

    uint32_t           total_frames;
    /**< @h2xmle_description {Total number of frames.}
         @h2xmle_rangeList   {"Any value present in the APE header" = 0}
         @h2xmle_policy      {Basic}  */

    uint16_t           bit_width;
    /**< @h2xmle_description {Bit width of the source PCM data.}
         @h2xmle_default     {16}
         @h2xmle_rangeList   {"16" = 16,"24" = 24}
         @h2xmle_policy      {Basic} */

    uint16_t           num_channels;
    /**< @h2xmle_description {Number of channels for decoding}
         @h2xmle_rangeList   {"Mono" = 1,"Stereo"= 2,INVALID_VALUE=0}
         @h2xmle_policy      {Basic} */

    uint32_t           sample_rate;
    /**< @h2xmle_description {Number of samples per second.}
	     @h2xmle_rangeList   {INVALID_VALUE=0, "8000"=8000, "11025"=11025, "12000"=12000, 
	                          "16000"=16000, "22050"=22050, "24000"=24000, "32000"=32000,
                                  "44100"=44100, "48000"=48000, "64000"=64000, "88200"=88200, 
				  "96000"=96000, "128000"=128000, "176400"=176400, "192000"=192000}
         @h2xmle_policy      {Basic} */

    uint32_t           is_seek_table_present;
    /**< @h2xmle_description {Flag used to indicate the presence of the seek table. This flag is
                              not part of the APE header.}
         @h2xmle_rangeList   {"Not present" = 0,
                                  "Present" = 1}
         @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

/*************************************************** FLAC Media Format ************************************************/
/**
 * Media format ID for FLAC streams
 */
#define MEDIA_FMT_ID_FLAC                 0x09001004

typedef struct payload_media_fmt_flac_t payload_media_fmt_flac_t;

/** @h2xmlp_subStruct
    @h2xmlp_description {Payload of the FLAC decoder format block structure parameters in the
   MEDIA_FMT_ID_FLAC media format..\n }
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct payload_media_fmt_flac_t
{
    uint16_t          num_channels;
    /**< @h2xmle_description {Number of channels present in the Flac stream.}
          @h2xmle_default     {1}
         @h2xmle_rangeList       {"Mono"=1,
                              "Stereo"=2}
         @h2xmle_policy      {Basic} */


    uint16_t          sample_size;
    /**< @h2xmle_description {Bits per sample}
         @h2xmle_default     {16}
         @h2xmle_rangeList   {16,24}
         @h2xmle_policy      {Basic} */

    uint16_t          min_blk_size;
    /**< @h2xmle_description {Minimum block size (in samples) used in the stream.
                              It must be lessthan or equal to max_blk_size.}
         @h2xmle_policy      {Basic} */

    uint16_t          max_blk_size;
    /**< @h2xmle_description {Maximum block size (in samples) used in the stream.
                              If the minimum block size equals the maximum block size,
                              a fixed block size stream is implied}
         @h2xmle_policy      {Basic} */


    uint32_t          sample_rate;
    /**< @h2xmle_description {Supported sampling rates}
  @h2xmle_default     {0}
          @h2xmle_rangeList   {INVALID_VALUE=0,0,8000, 11025, 12000, 16000, 22050, 24000, 32000,
                             44100, 48000, 64000, 88200, 96000, 128000, 176400, 192000}
         @h2xmle_policy      {Basic} */

    uint32_t          min_frame_size;
    /**< @h2xmle_description {Minimum frame size. This should be greater than 0}
         @h2xmle_policy      {Basic} */

    uint32_t          max_frame_size;
    /**< @h2xmle_description {Maximum frame size. This should be greater than 0}
         @h2xmle_policy      {Basic} */

}
#include "gk_end_pack.h"
;
/*************************************************** Vorbis Media Format **********************************************/
/**
 * Media format ID for Vorbis streams
 */
#define MEDIA_FMT_ID_VORBIS               0x09001005

typedef struct payload_media_fmt_vorbis_t payload_media_fmt_vorbis_t;

/** @h2xmlp_subStruct
    @h2xmlp_description {Payload of the VORBIS decoder format block structure parameters in the
   MEDIA_FMT_ID_VORBIS media format. \n }
    @h2xmlp_toolPolicy  {Calibration} */
#include "gk_begin_pack.h"
struct payload_media_fmt_vorbis_t
{
    uint32_t          bit_stream_fmt;
    /**< @h2xmle_description {Transcoded bit stream containing the size of the frame
                          as the first word in each frame.}
                                        @h2xmle_default     {1}
         @h2xmle_rangeList       {"Transcoded Bit Stream"=1}
         @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

/*************************************************** WMAPro Media Format **********************************************/
/**
 * Media format ID for the WMAPRO streams.
 */
#define MEDIA_FMT_ID_WMAPRO               0x09001006

typedef struct payload_media_fmt_wmapro_t payload_media_fmt_wmapro_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload of the WMAPRO decoder format block parameters in the
                          MEDIA_FMT_ID_WMAPRO media format}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct payload_media_fmt_wmapro_t
{
    uint16_t          fmt_tag;
  /**< @h2xmle_description {WMA format type.}
       @h2xmle_rangeList   {"WMA 9 Pro"           = 0x162,
                            "WMA 9 Pro Lossless"  = 0x163,
                            "WMA 10 Pro"          = 0x166,
                            "WMA 10 Pro Lossless" = 0x167,
                            INVALID_VALUE=0}
       @h2xmle_policy      {Basic} */

    uint16_t          num_channels;
  /**< @h2xmle_description {Number of channels encoded in the input stream.}
       @h2xmle_rangeList   {INVALID_VALUE=0,1...8}
       @h2xmle_policy      {Basic} */

    uint32_t          sample_rate;
  /**< @h2xmle_description {Number of samples per second in Hertz.}
       @h2xmle_rangeList   {INVALID_VALUE=0, 11025, 16000, 22050, 32000,
                             44100, 48000, 88200, 96000}
       @h2xmle_policy      {Basic} */

    uint32_t          avg_bytes_per_sec;
  /**< @h2xmle_description {Bitrate expressed as the average bytes per second}
       @h2xmle_rangeList   {INVALID_VALUE=0,2000..96000}
       @h2xmle_policy      {Basic} */

    uint16_t          blk_align;
  /**< @h2xmle_description {Size of the bit stream packet size. WMA Pro files have a payload of one
                            block per bit stream packet.}
       @h2xmle_rangeList   {"13376"=13376}
       @h2xmle_default     {13376}
       @h2xmle_policy      {Basic} */

    uint16_t          bits_per_sample;
  /**< @h2xmle_description {Number of bits per sample in the encoded WMA stream.}
       @h2xmle_default     {16}
       @h2xmle_rangeList   {"16" = 16,
                            "24" = 24}
       @h2xmle_policy      {Basic} */

    uint32_t          channel_mask;
  /**< @h2xmle_description {Bit-packed double word (32-bits) that indicates the recommended
                            speaker positions for each source channel.}
       @h2xmle_policy      {Basic} */

    uint16_t          enc_options;
  /**< @h2xmle_description {Bit-packed word with values that indicate whether certain features of
                            the bit stream are used.}
       @h2xmle_rangeList   {"ENCOPT3_PURE_LOSSLESS"          = 0x0001,
                            "ENCOPT3_FRM_SIZE_MOD"           = 0x0006,
                            "ENCOPT3_SUBFRM_DIV"             = 0x0038,
                            "ENCOPT3_WRITE_FRAMESIZE_IN_HDR" = 0x0040,
                            "ENCOPT3_GENERATE_DRC_PARAMS"    = 0x0080,
                            "ENCOPT3_RTMBITS"                = 0x0100,
                            INVALID_VALUE=0}
       @h2xmle_policy      {Basic} */

    uint16_t          advanced_enc_option;
  /**< @h2xmle_description {Advanced encoding option. This field is ignored.}
       @h2xmle_policy      {Basic} */

    uint32_t          advanced_enc_options2;
  /**< @h2xmle_description {Advanced encoding option 2. This field is ignored.}
       @h2xmle_policy      {Basic} */

}
#include "gk_end_pack.h"
;

/*************************************************** WMAStd Media Format **********************************************/
/**
 * Media format ID for the WMASTD streams.
 */
#define MEDIA_FMT_ID_WMASTD               0x09001007

typedef struct payload_media_fmt_wmastd_t payload_media_fmt_wmastd_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload of the WMASTD decoder format block parameters in the
                          MEDIA_FMT_ID_WMASTD media format}
    @h2xmlp_toolPolicy  {Calibration} */


#include "gk_begin_pack.h"
struct payload_media_fmt_wmastd_t
{
    uint16_t          fmt_tag;
    /**< @h2xmle_description {WMA format tag.}
         @h2xmle_rangeList   {"WMA v9 standard"     = 0x161}
         @h2xmle_default     {0x161}
         @h2xmle_policy      {Basic} */

    uint16_t          num_channels;
    /**< @h2xmle_description {Number of channels in the stream.}
         @h2xmle_rangeList   {"Mono" = 1, "Stereo" = 2, INVALID_VALUE=0}
         @h2xmle_policy      {Basic} */

    uint32_t          sample_rate;
    /**< @h2xmle_description {Number of samples per second in Hertz.}
	     @h2xmle_rangeList   {INVALID_VALUE=0, "8000"=8000, "11025"=11025,
	                          "16000"=16000, "22050"=22050, "32000"=32000,
                                  "44100"=44100, "48000"=48000}
         @h2xmle_policy      {Basic} */

    uint32_t          avg_bytes_per_sec;
    /**< @h2xmle_description {Bitrate expressed as the average bytes per second}
         @h2xmle_policy      {Basic} */

    uint16_t          blk_align;
    /**< @h2xmle_description {Block align. All WMA files with a maximum packet size of 13376
                          are supported.}
         @h2xmle_policy      {Basic} */

    uint16_t          bits_per_sample;
    /**< @h2xmle_description {Number of bits per sample in the output}
         @h2xmle_rangeList   {"16" = 16}
         @h2xmle_default     {16}
         @h2xmle_policy      {Basic} */

    uint32_t          channel_mask;
    /**< @h2xmle_description {Channel mask.}
         @h2xmle_rangeList  {"Stereo (front left/front right)" = 3,
                          "Mono  (centre)" = 4, INVALID_VALUE=0}
         @h2xmle_policy      {Basic} */

    uint16_t          enc_options;
    /**< @h2xmle_description {Options used during encoding.}
         @h2xmle_policy      {Basic} */

    uint16_t          reserved;
    /**< @h2xmle_description {Used for alignment; must be set to 0.}
         @h2xmle_policy      {Basic} */

}
#include "gk_end_pack.h"
;

/*************************************************** SBC Media Format *************************************************/
/**
 * Media format ID for the SBC
 */
#define MEDIA_FMT_ID_SBC                                 0x09001008

/** Enumeration for SBC channel Mono mode. */
#define MEDIA_FMT_SBC_CHANNEL_MODE_MONO                     0

/** Enumeration for SBC channel Stereo mode. */
#define MEDIA_FMT_SBC_CHANNEL_MODE_STEREO                   1

/** Enumeration for SBC channel Dual Mono mode. */
#define MEDIA_FMT_SBC_CHANNEL_MODE_DUAL_MONO                8

/** Enumeration for SBC channel Joint Stereo mode. */
#define MEDIA_FMT_SBC_CHANNEL_MODE_JOINT_STEREO             9

/** Enumeration for SBC bit allocation method = loudness. */
#define MEDIA_FMT_SBC_ALLOCATION_METHOD_LOUDNESS            0

/** Enumeration for SBC bit allocation method = SNR.  */
#define MEDIA_FMT_SBC_ALLOCATION_METHOD_SNR                 1

/* SBC encoder configuration structure. */
typedef struct sbc_enc_cfg_t sbc_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the SBC encoder module} */

#include "gk_begin_pack.h"
struct sbc_enc_cfg_t
{
    uint32_t          num_subbands;
   /**<
    @h2xmle_description {Number of subbands}
    @h2xmle_default     {4}
    @h2xmle_rangeList   { "4" = 4,
                          "8" = 8}
    @h2xmle_policy      {Basic} */

    uint32_t          blk_len;
   /**<
    @h2xmle_description {Size of the encoded block in samples}
    @h2xmle_default     {4}
    @h2xmle_rangeList   { "4"  = 4,
                          "8"  = 8,
                          "12" = 12,
                          "16" = 16}
    @h2xmle_policy      {Basic} */

    uint32_t          channel_mode;
   /**<
    @h2xmle_description {Mode used to allocate bits between channels.

                        -Native mode indicates that encoding must be performed with the number
                          of channels at the input.

                        -If postprocessing outputs one-channel data, Mono mode is used. If
                          postprocessing outputs two-channel data, Stereo mode is used.

                        -The number of channels must not change during encoding. }
    @h2xmle_default     {0}
    @h2xmle_rangeList   { "NATIVE_MODE" = 0}
	                  "MEDIA_FMT_SBC_CHANNEL_MODE_MONO"
                          "MEDIA_FMT_SBC_CHANNEL_MODE_DUAL_MONO"
                          "MEDIA_FMT_SBC_CHANNEL_MODE_STEREO"
                          "MEDIA_FMT_SBC_CHANNEL_MODE_JOINT_STEREO"
    @h2xmle_policy      {Basic} */

    uint32_t          alloc_method;
   /**<
    @h2xmle_description {Encoder bit allocation method.}
    @h2xmle_default     {0}
    @h2xmle_rangeList   { "MEDIA_FMT_SBC_ALLOCATION_METHOD_LOUDNESS"=0,
                          "MEDIA_FMT_SBC_ALLOCATION_METHOD_SNR"=1}
    @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;
/*************************************************** MP3 Media Format *************************************************/
/**
 * Media format ID for MP3 streams
 */
#define MEDIA_FMT_ID_MP3                  0x09001009

/*************************************************** AMRWB+ Media Format **********************************************/
/**
 * Media format ID for the amrwbplus streams
 */
#define MEDIA_FMT_ID_AMRWBPLUS            0x0900100A

/** @ingroup MEDIA_FMT_ID_AMRWBPLUS
    Media format ID for the AMR-WB Plus format block.

TODO: Check the following lines
    @par AMR-WB Plus format block (payload_media_fmt_amrwbplus_t)
    @table{weak__payload__amrwbplus__fmt__blk__t}
*/
typedef struct payload_media_fmt_amrwbplus_t payload_media_fmt_amrwbplus_t;
/** @h2xmlp_subStruct
28  @h2xmlp_description {Payload of the AMRWBPLUS decoder format block parameters in the
29                       MEDIA_FMT_ID_AMRWBPLUS media format}
30  @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"

struct payload_media_fmt_amrwbplus_t
{
    uint32_t          amr_frame_fmt;
    /**< @h2xmle_description {Transport Interface Format (TIF)}
         @h2xmle_rangeList   {"contains a 2-byte header for each frame within the superframe" = 6,
                               "contains one header per superframe"= 0}
         @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;
/*************************************************** AMRNB Media Format *************************************************/
/**
 * Media format ID for AMRNB streams
 */
#define MEDIA_FMT_ID_EAMR                0x0900100B

#define MEDIA_FMT_ID_AMRNB_IF2           0x0900100C

#define MEDIA_FMT_ID_AMRNB_MODEM         0x0900100D

#define MEDIA_FMT_ID_AMRNB_FS            0x0900100E


/** Enumeration for 4.75 kbps AMR-NB Encoding mode. */
#define AMRNB_FS_ENCODE_MODE_MR475                0

/** Enumeration for 5.15 kbps AMR-NB Encoding mode. */
#define AMRNB_FS_ENCODE_MODE_MR515                1

/** Enumeration for 5.90 kbps AMR-NB Encoding mode. */
#define AMRNB_FS_ENCODE_MODE_MMR59                2

/** Enumeration for 6.70 kbps AMR-NB Encoding mode. */
#define AMRNB_FS_ENCODE_MODE_MMR67                3

/** Enumeration for 7.40 kbps AMR-NB Encoding mode. */
#define AMRNB_FS_ENCODE_MODE_MMR74                4

/** Enumeration for 7.95 kbps AMR-NB Encoding mode. */
#define AMRNB_FS_ENCODE_MODE_MMR795               5

/** Enumeration for 10.20 kbps AMR-NB Encoding mode. */
#define AMRNB_FS_ENCODE_MODE_MMR102               6

/** Enumeration for 12.20 kbps AMR-NB Encoding mode. */
#define AMRNB_FS_ENCODE_MODE_MMR122               7

/** Enumeration for disabling AMR-NB DTX mode. */
#define AMRNB_FS_DTX_MODE_OFF                     0

/** Enumeration for enabling AMR-NB DTX mode VAD1. */
#define AMRNB_FS_DTX_MODE_VAD1                    1

/** Enumeration for enabling AMR-NB DTX mode VAD2. */
#define AMRNB_FS_DTX_MODE_VAD2                    2

/** Enumeration for enabling AMR-NB automatic DTX mode. */
#define AMRNB_FS_DTX_MODE_AUTO                    3


typedef struct amrnb_enc_cfg_t amrnb_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the AMRNB encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct amrnb_enc_cfg_t
{
    uint16_t          enc_mode;
    /**< @h2xmle_description {AMR-NB encoding rate.}
         @h2xmle_default     {0}
         @h2xmle_rangeList   {"AMRNB_FS_ENCODE_MODE_MR475"=0,
                              "AMRNB_FS_ENCODE_MODE_MR515"=1,
                              "AMRNB_FS_ENCODE_MODE_MMR59"=2,
                              "AMRNB_FS_ENCODE_MODE_MMR67"=3,
                              "AMRNB_FS_ENCODE_MODE_MMR74"=4,
                              "AMRNB_FS_ENCODE_MODE_MMR795"=5,
                              "AMRNB_FS_ENCODE_MODE_MMR102"=6,
                              "AMRNB_FS_ENCODE_MODE_MMR122"=7}
         @h2xmle_policy      {Basic} */ // TODO : check the policy
    uint16_t          dtx_mode;
    /**< @h2xmle_description {Specifies whether DTX mode is enabled}
             @h2xmle_default     {0}
             @h2xmle_rangeList   {"Enable"=1,
                                  "Disable"=0}
             @h2xmle_policy      {Basic} */ // TODO : check the policy
}
#include "gk_end_pack.h"
;

typedef struct payload_media_fmt_eamr_t payload_media_fmt_eamr_t;

/** @h2xmlp_subStruct
    @h2xmlp_description {Payload of the EAMR decoder format block structure to indicate the decoding format}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct payload_media_fmt_eamr_t
{
    uint32_t          media_format_id;
    /**< @h2xmle_description {Configures the decoding format of the AMRNB decoder}
         @h2xmle_default     {MEDIA_FMT_ID_AMRNB_MODEM}
         @h2xmle_rangeList   {"MEDIA_FMT_ID_EAMR"=0x0900100B,
                              "MEDIA_FMT_ID_AMRNB_IF2"=0x0900100C,
                              "MEDIA_FMT_ID_AMRNB_MODEM"=0x0900100D,
                              "MEDIA_FMT_ID_AMRNB_FS"=0x0900100E}
         @h2xmle_policy      {Basic} */
}

#include "gk_end_pack.h"
;

/*********************************************** EVRC Media format *********************************************************/
/**
 * Media format ID for EVRC streams
 */
#define MEDIA_FMT_ID_EVRC 			     0x0900100F
#define MEDIA_FMT_ID_EVRC_FS             0x09001024


typedef struct evrc_enc_cfg_t evrc_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the EVRC encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct evrc_enc_cfg_t
{
	uint16_t          max_rate;
    /**< @h2xmle_description {Maximum allowed encoder frame rate.}
         @h2xmle_default     {VOCODER_INVALID_RATE}
         @h2xmle_rangeList   {"VOCODER_ONE_EIGHTH_RATE"=1,
                              "VOCODER_ONE_FOURTH_RATE"=2,
                              "VOCODER_HALF_RATE"=3,
                              "VOCODER_FULL_RATE"=4,
                              "VOCODER_INVALID_RATE"=0}
         @h2xmle_policy      {Basic} */
	uint16_t          min_rate;
    /**< @h2xmle_description {Maximum allowed encoder frame rate.}
         @h2xmle_default     {VOCODER_INVALID_RATE}
         @h2xmle_rangeList   {"VOCODER_ONE_EIGHTH_RATE"=1,
                              "VOCODER_ONE_FOURTH_RATE"=2,
                              "VOCODER_HALF_RATE"=3,
                              "VOCODER_FULL_RATE"=4,
                              "VOCODER_INVALID_RATE"=0}
         @h2xmle_policy      {Basic} */
	uint16_t          rate_mod_cmd;
	/**< @h2xmle_description {Rate modulation command
	                         If bit 0=1, rate control is enabled.
	                         If bit 1=1, the maximum number of consecutive full rate frames is
	                         limited with numbers supplied in bits 2 to 10.
	                         If bit 1=0, the minimum number of non-full rate frames in between
	                         two full rate frames is forced to the number supplied in bits 2 to 10
	                         In both cases of bit 1, if necessary, half rate is used to substitute full rate.
	                         Bits 15 to 10 are reserved and must all be set to 0.}
	     @h2xmle_default     {0}
	     @h2xmle_policy      {Basic} */
	   uint16_t         reserved;
/**< @h2xmle_description {Clients must set this field to 0.}
	   @h2xmle_rangeList {"0"=0}
	   @h2xmle_visibility	{hide} */
}
#include "gk_end_pack.h"
;

/************************************************************************************************************************/

/************************************************ HR Media format *****************************************************/

/**
 * Media format ID for HR streams
 */
#define MEDIA_FMT_ID_HR              0x09001019


/************************************************************************************************************************/

/************************************************ FR Media format *****************************************************/

/**
 * Media format ID for FR streams
 */
#define MEDIA_FMT_ID_FR              0x0900101B
#define MEDIA_FMT_ID_FR_FS           0x0900101C


/************************************************************************************************************************/

/************************************************ EFR Media format *****************************************************/

/**
 * Media format ID for EFR streams
 */
#define MEDIA_FMT_ID_EFR              0x0900101A


/************************************************************************************************************************/

/************************************************ G711 Media format *****************************************************/

/**
 * Media format ID for G711 streams
 */
#define MEDIA_FMT_ID_G711_ALAW              0x09001032
#define MEDIA_FMT_ID_G711_MLAW              0x09001033
#define MEDIA_FMT_ID_G711_ALAW_V2           0x09001034
#define MEDIA_FMT_ID_G711_MLAW_V2           0x09001035
#define MEDIA_FMT_ID_G711_ALAW_FS           0x09001036
#define MEDIA_FMT_ID_G711_MLAW_FS           0x09001037

typedef struct payload_media_fmt_g711_t payload_media_fmt_g711_t;

/** @h2xmlp_subStruct
    @h2xmlp_description {Payload of the G711 decoder format block structure}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct payload_media_fmt_g711_t
{
    uint32_t          sample_rate;
    /**< @h2xmle_description {Sampling rate}
  	  	 @h2xmle_default     {8000}
         @h2xmle_rangeList   {"8000" = 8000, "16000" = 16000}
         @h2xmle_policy      {Basic} */
}

#include "gk_end_pack.h"
;

/** Parameter that sets plc mode to the decoder */
#define PARAM_ID_G711_DEC_PLC_MODE 0x080011A7

/** @h2xmlp_parameter   {"PARAM_ID_G711_DEC_PLC_MODE", PARAM_ID_G711_DEC_PLC_MODE}
    @h2xmlp_description {parameter which sets the PLC mode on the decoder }
    @h2xmlp_toolPolicy  {Calibration} */

typedef struct param_id_g711_dec_plc_mode_t param_id_g711_dec_plc_mode_t;

#include "gk_begin_pack.h"
struct param_id_g711_dec_plc_mode_t
{
	uint32_t plc_mode;
    /**< @h2xmle_description {Specifies whether PLC mode is disabled (0) or enabled (1)}
         @h2xmle_default     {1}
         @h2xmle_rangeList   {"Enable"=1,
                              "Disable"=0}
         @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

/************************************************************************************************************************/


/************************************************ G729AB Media format *****************************************************/

/**
 * Media format ID for G729AB streams
 */
#define MEDIA_FMT_ID_G729AB              0x09001010


/*********************************************** LDAC Media format *********************************************************/
/**
 * Media format ID for LDAC streams
 */
#define MEDIA_FMT_ID_LDAC 			     0x0900101D

/** Enumeration for Native mode on the LDAC channel. */
#define MEDIA_FMT_LDACBT_CHANNEL_MODE_NATIVE         0

/** Enumeration for Stereo mode on the LDAC channel. */
#define MEDIA_FMT_LDACBT_CHANNEL_MODE_STEREO         1

/** Enumeration for Dual Channel mode on the LDAC channel. */
#define MEDIA_FMT_LDACBT_CHANNEL_MODE_DUAL_CHANNEL   2

/** Enumeration for Mono mode on the LDAC channel. */
#define MEDIA_FMT_LDACBT_CHANNEL_MODE_MONO           4

/************************************************ V13K Media format *****************************************************/
#define MEDIA_FMT_ID_V13K                             0x09001025
#define MEDIA_FMT_ID_V13K_FS                          0x09001026


/** Enumeration for 14.4 kbps V13K Encoding mode. */
#define V13K_FS_ENCODE_MODE_MR1440                0

/** Enumeration for 12.2 kbps V13K Encoding mode. */
#define V13K_FS_ENCODE_MODE_MR1220                1

/** Enumeration for 11.2 kbps V13K Encoding mode. */
#define V13K_FS_ENCODE_MODE_MR1120                2

/** Enumeration for 9.0 kbps V13K Encoding mode. */
#define V13K_FS_ENCODE_MODE_MR90                  3

/** Enumeration for 7.2 kbps V13K Encoding mode. */
#define V13K_FS_ENCODE_MODE_MR720                 4


typedef struct v13k_enc_cfg_t v13k_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the V13K encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct v13k_enc_cfg_t
{
	uint16_t          max_rate;
    /**< @h2xmle_description {Maximum allowed encoder frame rate.}
         @h2xmle_default     {VOCODER_FULL_RATE}
         @h2xmle_rangeList   {"VOCODER_ONE_EIGHTH_RATE"=1,
                              "VOCODER_ONE_FOURTH_RATE"=2,
                              "VOCODER_HALF_RATE"=3,
                              "VOCODER_FULL_RATE"=4,
                              "VOCODER_INVALID_RATE"=0}
         @h2xmle_policy      {Basic} */
	uint16_t          min_rate;
    /**< @h2xmle_description {Minimum allowed encoder frame rate.}
         @h2xmle_default     {VOCODER_ONE_EIGHTH_RATE}
         @h2xmle_rangeList   {"VOCODER_ONE_EIGHTH_RATE"=1,
                              "VOCODER_ONE_FOURTH_RATE"=2,
                              "VOCODER_HALF_RATE"=3,
                              "VOCODER_FULL_RATE"=4,
                              "VOCODER_INVALID_RATE"=0}
         @h2xmle_policy      {Basic} */
	uint16_t          reduced_rate_cmd;
	/**< @h2xmle_description {Reduced rate command, used to change the average bit-rate
	                         of the vocoder}
	     @h2xmle_rangeList   {"V13K_FS_ENCODE_MODE_MR1440"=0,
                              "V13K_FS_ENCODE_MODE_MR1220"=1,
                              "V13K_FS_ENCODE_MODE_MR1120"=2,
                              "V13K_FS_ENCODE_MODE_MR90"=3,
                              "V13K_FS_ENCODE_MODE_MR720"=4}
	     @h2xmle_default     {V13K_FS_ENCODE_MODE_MR1440}
	     @h2xmle_policy      {Basic} */
	uint16_t          rate_mod_cmd;
	/**< @h2xmle_description {Rate modulation command
	                         If bit 0=1, rate control is enabled.
	                         If bit 1=1, the maximum number of consecutive full rate frames is
	                         limited with numbers supplied in bits 2 to 10.
	                         If bit 1=0, the minimum number of non-full rate frames in between
	                         two full rate frames is forced to the number supplied in bits 2 to 10
	                         In both cases of bit 1, if necessary, half rate is used to substitute full rate.
	                         Bits 15 to 10 are reserved and must all be set to 0.}
	     @h2xmle_default     {0}
	     @h2xmle_range       {0..0x03FF}
	     @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

/************************************************ AMRWB Media format *****************************************************/

/**
 * Media format ID for AMRWB streams
 */
#define MEDIA_FMT_ID_AMRWB              0x09001027
#define MEDIA_FMT_ID_AMRWB_FS           0x09001028


/** Enumeration for 6.60 kbps AMR-WB Encoding mode. */
#define AMRWB_FS_ENCODE_MODE_MR66                 0

/** Enumeration for 8.85 kbps AMR-WB Encoding mode. */
#define AMRWB_FS_ENCODE_MODE_MR885                1

/** Enumeration for 12.65 kbps AMR-WB Encoding mode. */
#define AMRWB_FS_ENCODE_MODE_MR1265               2

/** Enumeration for 14.25 kbps AMR-WB Encoding mode. */
#define AMRWB_FS_ENCODE_MODE_MR1425               3

/** Enumeration for 15.85 kbps AMR-WB Encoding mode. */
#define AMRWB_FS_ENCODE_MODE_MR1585               4

/** Enumeration for 18.25 kbps AMR-WB Encoding mode. */
#define AMRWB_FS_ENCODE_MODE_MR1825               5

/** Enumeration for 19.85 kbps AMR-WB Encoding mode. */
#define AMRWB_FS_ENCODE_MODE_MR1985               6

/** Enumeration for 23.05 kbps AMR-WB Encoding mode. */
#define AMRWB_FS_ENCODE_MODE_MR2305               7

/** Enumeration for 23.85 kbps AMR-WB Encoding mode. */
#define AMRWB_FS_ENCODE_MODE_MR2385               8

typedef struct amrwb_enc_cfg_t amrwb_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the AMRWB encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct amrwb_enc_cfg_t
{
    uint16_t          enc_mode;
    /**< @h2xmle_description {AMR-WB encoding rate.}
         @h2xmle_default     {0}
         @h2xmle_rangeList   {"AMRWB_FS_ENCODE_MODE_MR66"=0,
                              "AMRWB_FS_ENCODE_MODE_MR885"=1,
                              "AMRWB_FS_ENCODE_MODE_MR1265"=2,
                              "AMRWB_FS_ENCODE_MODE_MR1425"=3,
                              "AMRWB_FS_ENCODE_MODE_MR1585"=4,
                              "AMRWB_FS_ENCODE_MODE_MR1825"=5,
                              "AMRWB_FS_ENCODE_MODE_MR1985"=6,
                              "AMRWB_FS_ENCODE_MODE_MR2305"=7,
                              "AMRWB_FS_ENCODE_MODE_MR2385"=8}
         @h2xmle_policy      {Basic} */ // TODO : check the policy
    uint16_t          dtx_mode;
    /**< @h2xmle_description {Specifies whether DTX mode is enabled}
             @h2xmle_default     {0}
             @h2xmle_rangeList   {"Enable"=1,
                                  "Disable"=0}
             @h2xmle_policy      {Basic} */ // TODO : check the policy
}
#include "gk_end_pack.h"
;


/************************************************************************************************************************/

/*********************************************** CELT Media format *********************************************************/
/**
 * Media format ID for the CELT
 */
#define MEDIA_FMT_ID_CELT                0x0900102A 

/* CELT encoder configuration structure. */
typedef struct celt_enc_cfg_t celt_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the CELT encoder module} */

#include "gk_begin_pack.h"
struct celt_enc_cfg_t
{

	  uint16_t          frame_size;
	  /**< 
	     @h2xmle_description {Frame size used for encoding.}
         @h2xmle_default     {256}
         @h2xmle_rangeList   {"64" = 64,
                              "128" = 128,
                              "256" = 256,
                              "512" = 512}
         @h2xmle_policy      {Basic} */
		 

      uint16_t          complexity;
	  /**< 
	     @h2xmle_description {Complexity of the algorithm.
		   - Currently, this parameter is used to enable or disable the pitch
			 analysis. In the future, it can be scaled to add more levels of
			 complexity.
		   - value less than 2 - Disable pitch analysis
           - Value > 2 - Enable pitch analysis}
         @h2xmle_default     {3}
         @h2xmle_range       {0..10}
         @h2xmle_policy      {Basic} */
	  
  
      uint16_t          prediction_mode;
	  /**< 
	     @h2xmle_description {Switch variable for the prediction feature.
		  - This parameter is used to choose between the level of interframe
			  predictions allowed while encoding.
		  - 0 -- Independent frames
          - 1 -- Short-term interframe prediction allowed
          - 2 -- Long-term prediction allowed}
         @h2xmle_default     {2}
         @h2xmle_rangeList   {"0" = 0,
		                      "1" = 1,
							  "2" = 2}
         @h2xmle_policy      {Basic} */
  
      uint16_t          vbr_flag;
	  /**< 
	     @h2xmle_description {Variable bit rate flag.
		   Currently, this parameter is disabled. In the future, it can be
           enabled if use cases require it.}
         @h2xmle_default     {0}
         @h2xmle_policy      {Basic} */
	  
}
#include "gk_end_pack.h"
;
/************************************************ FOURGV Media format *****************************************************/

/**
 * Media format ID for FOURGV streams
 */
#define MEDIA_FMT_ID_EVRCB_FS                           0x0900102B
#define MEDIA_FMT_ID_EVRCWB_FS                          0x0900102C
#define MEDIA_FMT_ID_4GV_NB_MODEM                       0x0900102D
#define MEDIA_FMT_ID_4GV_WB_MODEM                       0x0900102E
#define MEDIA_FMT_ID_4GV_NW_MODEM                       0x0900102F
#define MEDIA_FMT_ID_4GV_NW                             0x09001030
#define MEDIA_FMT_ID_EVRC_NW_2K                         0x09001031


/** Enumeration for 9.3 kbps EVRC-B Encoding mode. */
#define EVRCB_FS_ENCODE_MODE_MR930        0

/** Enumeration for 8.5 kbps EVRC-B Encoding mode. */
#define EVRCB_FS_ENCODE_MODE_MR850        1

/** Enumeration for 7.5 kbps EVRC-B Encoding mode. */
#define EVRCB_FS_ENCODE_MODE_MR750        2

/** Enumeration for 7.0 kbps EVRC-B Encoding mode. */
#define EVRCB_FS_ENCODE_MODE_MR700        3

/** Enumeration for 6.6 kbps EVRC-B Encoding mode. */
#define EVRCB_FS_ENCODE_MODE_MR660        4

/** Enumeration for 6.2 kbps EVRC-B Encoding mode. */
#define EVRCB_FS_ENCODE_MODE_MR620        5

/** Enumeration for 5.8 kbps EVRC-B Encoding mode. */
#define EVRCB_FS_ENCODE_MODE_MR580        6

/** Enumeration for 4.8 kbps EVRC-B Encoding mode. */
#define EVRCB_FS_ENCODE_MODE_MR480        7

/* EVCR-B encoder configuration structure. */
//typedef struct evrcb_enc_cfg_t evrcb_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the EVRC-B encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct evrcb_enc_cfg_t
{
	uint16_t          max_rate;
    /**< @h2xmle_description {Maximum allowed encoder frame rate.}
         @h2xmle_default     {EVRCB_FS_ENCODE_MODE_MR930}
         @h2xmle_rangeList   {"EVRCB_FS_ENCODE_MODE_MR930"=0,
                              "EVRCB_FS_ENCODE_MODE_MR850"=1,
                              "EVRCB_FS_ENCODE_MODE_MR750"=2,
                              "EVRCB_FS_ENCODE_MODE_MR700"=3,
                              "EVRCB_FS_ENCODE_MODE_MR660"=4,
                              "EVRCB_FS_ENCODE_MODE_MR620"=5,
                              "EVRCB_FS_ENCODE_MODE_MR580"=6,
                              "EVRCB_FS_ENCODE_MODE_MR480"=7}
         @h2xmle_policy      {Basic} */
	uint16_t          min_rate;
    /**< @h2xmle_description {Minimum allowed encoder frame rate.}
         @h2xmle_default     {EVRCB_FS_ENCODE_MODE_MR480}
         @h2xmle_rangeList   {"EVRCB_FS_ENCODE_MODE_MR930"=0,
                              "EVRCB_FS_ENCODE_MODE_MR850"=1,
                              "EVRCB_FS_ENCODE_MODE_MR750"=2,
                              "EVRCB_FS_ENCODE_MODE_MR700"=3,
                              "EVRCB_FS_ENCODE_MODE_MR660"=4,
                              "EVRCB_FS_ENCODE_MODE_MR620"=5,
                              "EVRCB_FS_ENCODE_MODE_MR580"=6,
                              "EVRCB_FS_ENCODE_MODE_MR480"=7}
         @h2xmle_policy      {Basic} */
    uint16_t          dtx_mode;
    /**< @h2xmle_description {Specifies whether DTX mode is enabled}
             @h2xmle_default     {0}
             @h2xmle_rangeList   {"Enable"=1,
                                  "Disable"=0}
             @h2xmle_policy      {Basic} */

    uint16_t          reduced_rate_level;
    /**< @h2xmle_description {Reduced rate level for the average encoding rate}
         @h2xmle_default     {EVRCB_FS_ENCODE_MODE_MR480}
         @h2xmle_rangeList   {"EVRCB_FS_ENCODE_MODE_MR930"=0,
                              "EVRCB_FS_ENCODE_MODE_MR850"=1,
                              "EVRCB_FS_ENCODE_MODE_MR750"=2,
                              "EVRCB_FS_ENCODE_MODE_MR700"=3,
                              "EVRCB_FS_ENCODE_MODE_MR660"=4,
                              "EVRCB_FS_ENCODE_MODE_MR620"=5,
                              "EVRCB_FS_ENCODE_MODE_MR580"=6,
                              "EVRCB_FS_ENCODE_MODE_MR480"=7}
 	 	@h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

/** Enumeration for 8.5 kbps EVRC-WB Encoding mode.  */
#define EVRCWB_FS_ENCODE_MODE_MR850        0

/** Enumeration for 9.3 kbps EVRC-WB Encoding mode. */
#define EVRCWB_FS_ENCODE_MODE_MR930        4

/** Enumeration for 4.8 kbps EVRC-WB Encoding mode. */
#define EVRCWB_FS_ENCODE_MODE_MR480        7

/* EVRC-WB encoder configuration structure. */
typedef struct evrcwb_enc_cfg_t evrcwb_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the EVRC-WB encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"

struct evrcwb_enc_cfg_t
{
	uint16_t          max_rate;
    /**< @h2xmle_description {Maximum allowed encoder frame rate.}
         @h2xmle_default     {VOCODER_FULL_RATE}
         @h2xmle_rangeList   {"VOCODER_ONE_EIGHTH_RATE"=1,
                              "VOCODER_ONE_FOURTH_RATE"=2,
                              "VOCODER_HALF_RATE"=3,
                              "VOCODER_FULL_RATE"=4,
                              "VOCODER_INVALID_RATE"=0}
         @h2xmle_policy      {Basic} */
	uint16_t          min_rate;
    /**< @h2xmle_description {Minimum allowed encoder frame rate.}
         @h2xmle_default     {VOCODER_ONE_EIGHTH_RATE}
         @h2xmle_rangeList   {"VOCODER_ONE_EIGHTH_RATE"=1,
                              "VOCODER_ONE_FOURTH_RATE"=2,
                              "VOCODER_HALF_RATE"=3,
                              "VOCODER_FULL_RATE"=4,
                              "VOCODER_INVALID_RATE"=0}
         @h2xmle_policy      {Basic} */
    uint16_t          dtx_enable;
    /**< @h2xmle_description {Specifies whether DTX mode is enabled}
             @h2xmle_default     {0}
             @h2xmle_rangeList   {"Enable"=1,
                                  "Disable"=0}
             @h2xmle_policy      {Basic} */
	uint16_t          reduced_rate_level;
	/**< @h2xmle_description {Reduced rate level for the average encoding rate.}
         @h2xmle_default     {EVRCWB_FS_ENCODE_MODE_MR850}
         @h2xmle_rangeList   {"EVRCWB_FS_ENCODE_MODE_MR850"=0,
                              "EVRCWB_FS_ENCODE_MODE_MR930"=4,
                              "EVRCWB_FS_ENCODE_MODE_MR480"=7}
         @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

/*********************************************** EVS Media format *********************************************************/
/**
 * Media format ID for EVRC streams
 */
#define MEDIA_FMT_ID_EVS             0x09001023

/** Enumeration for 06.60 kbps, AMR-WB IO (WB). */
#define EVS_CODEC_MODE_0660               0

/** Enumeration for 08.85 kbps, AMR-WB IO (WB). */
#define EVS_CODEC_MODE_0885               1

/** Enumeration for 12.65 kbps, AMR-WB IO (WB). */
#define EVS_CODEC_MODE_1265               2

/** Enumeration for 14.25 kbps, AMR-WB IO (WB). */
#define EVS_CODEC_MODE_1425               3

/** Enumeration for 15.85 kbps, AMR-WB IO (WB). */
#define EVS_CODEC_MODE_1585               4

/** Enumeration for 18.25 kbps, AMR-WB IO (WB). */
#define EVS_CODEC_MODE_1825               5

/** Enumeration for 19.85 kbps, AMR-WB IO (WB). */
#define EVS_CODEC_MODE_1985               6

/** Enumeration for 23.05 kbps, AMR-WB IO (WB). */
#define EVS_CODEC_MODE_2305               7

/** Enumeration for 23.85 kbps, AMR-WB IO (WB). */
#define EVS_CODEC_MODE_2385               8

/** Enumeration for 05.90 kbps, EVS (NB, WB). */
#define EVS_CODEC_MODE_0590               9

/** Enumeration for 07.20 kbps, EVS (NB, WB). */
#define EVS_CODEC_MODE_0720               10

/** Enumeration for 08.00 kbps, EVS (NB, WB). */
#define EVS_CODEC_MODE_0800               11

/** Enumeration for 09.60 kbps, EVS (NB, WB, SWB). */
#define EVS_CODEC_MODE_0960               12

/** Enumeration for 13.20 kbps, EVS (NB, WB, SWB). */
#define EVS_CODEC_MODE_1320               13

/** Enumeration for 16.40 kbps, EVS (NB, WB, SWB, FB). */
#define EVS_CODEC_MODE_1640               14

/** Enumeration for 24.40 kbps, EVS (NB, WB, SWB, FB) */
#define EVS_CODEC_MODE_2440               15

/** Enumeration for 32.00 kbps, EVS (WB, SWB, FB). */
#define EVS_CODEC_MODE_3200               16

/** Enumeration for 48.00 kbps, EVS (WB, SWB, FB). */
#define EVS_CODEC_MODE_4800               17

/** Enumeration for 64.00 kbps, EVS (WB, SWB, FB). */
#define EVS_CODEC_MODE_6400               18

/** Enumeration for 96.00 kbps, EVS (WB, SWB, FB). */
#define EVS_CODEC_MODE_9600               19

/** Enumeration for 128.00 kbps, EVS (WB, SWB, FB). */
#define EVS_CODEC_MODE_12800               20


typedef struct evs_enc_cfg_t evs_enc_cfg_t;
/** @h2xmlp_subStruct
    @h2xmlp_description {Payload for configuring the EVS encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct evs_enc_cfg_t
{
   uint32_t bit_rate;
   /**< @h2xmle_description {EVS encoding bit rate}
        @h2xmle_rangeList   {"EVS_CODEC_MODE_0660"=0,
        					 "EVS_CODEC_MODE_0885"=1,
        					 "EVS_CODEC_MODE_1265"=2,
        					 "EVS_CODEC_MODE_1425"=3,
        					 "EVS_CODEC_MODE_1585"=4,
        					 "EVS_CODEC_MODE_1825"=5,
        					 "EVS_CODEC_MODE_1985"=6,
        					 "EVS_CODEC_MODE_2305"=7,
        					 "EVS_CODEC_MODE_2385"=8,
        					 "EVS_CODEC_MODE_0590"=9,
        					 "EVS_CODEC_MODE_0720"=10,
        					 "EVS_CODEC_MODE_0800"=11,
        					 "EVS_CODEC_MODE_0960"=12,
        					 "EVS_CODEC_MODE_1320"=13,
        					 "EVS_CODEC_MODE_1640"=14,
        					 "EVS_CODEC_MODE_2440"=15,
        					 "EVS_CODEC_MODE_3200"=16,
        					 "EVS_CODEC_MODE_4800"=17,
        					 "EVS_CODEC_MODE_6400"=18,
        					 "EVS_CODEC_MODE_9600"=19,
        					 "EVS_CODEC_MODE_12800"=20}
        @h2xmle_default     {13}
        @h2xmle_policy      {Basic} */

   uint32_t bandwidth;
   /**< @h2xmle_description {EVS encoder bandwidth}
        @h2xmle_rangeList   {"SAMPLE_RATE_8K"=0,
        					 "SAMPLE_RATE_16K"=1,
        					 "SAMPLE_RATE_32K"=2,
        					 "SAMPLE_RATE_48K"=3}
        @h2xmle_default     {2}
        @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;

/*==============================================================================
   Param ID
==============================================================================*/

/** ID of the parameter used to configure the encoder bit rate & bandwidth */
#define PARAM_ID_EVS_ENC_MODE 0x0800113F


/** @h2xmlp_parameter   {"PARAM_ID_EVS_ENC_MODE", PARAM_ID_EVS_ENC_MODE}
    @h2xmlp_description {Parameter for setting the bit rate, bandwidth for the EVS encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct param_id_evs_enc_mode_t
{
	uint32_t bit_rate;
	   /**< @h2xmle_description {EVS encoding bit rate}
	        @h2xmle_rangeList   {"EVS_CODEC_MODE_0660"=0,
	        					 "EVS_CODEC_MODE_0885"=1,
	        					 "EVS_CODEC_MODE_1265"=2,
	        					 "EVS_CODEC_MODE_1425"=3,
	        					 "EVS_CODEC_MODE_1585"=4,
	        					 "EVS_CODEC_MODE_1825"=5,
	        					 "EVS_CODEC_MODE_1985"=6,
	        					 "EVS_CODEC_MODE_2305"=7,
	        					 "EVS_CODEC_MODE_2385"=8,
	        					 "EVS_CODEC_MODE_0590"=9,
	        					 "EVS_CODEC_MODE_0720"=10,
	        					 "EVS_CODEC_MODE_0800"=11,
	        					 "EVS_CODEC_MODE_0960"=12,
	        					 "EVS_CODEC_MODE_1320"=13,
	        					 "EVS_CODEC_MODE_1640"=14,
	        					 "EVS_CODEC_MODE_2440"=15,
	        					 "EVS_CODEC_MODE_3200"=16,
	        					 "EVS_CODEC_MODE_4800"=17,
	        					 "EVS_CODEC_MODE_6400"=18,
	        					 "EVS_CODEC_MODE_9600"=19,
	        					 "EVS_CODEC_MODE_12800"=20}
	        @h2xmle_default     {13}
	        @h2xmle_policy      {Basic} */

	   uint32_t bandwidth;
	   /**< @h2xmle_description {EVS encoder bandwidth}
	        @h2xmle_rangeList   {"SAMPLE_RATE_8K"=0,
	        					 "SAMPLE_RATE_16K"=1,
	        					 "SAMPLE_RATE_32K"=2,
	        					 "SAMPLE_RATE_48K"=3}
	        @h2xmle_default     {2}
	        @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;
typedef struct param_id_evs_enc_mode_t param_id_evs_enc_mode_t;


/** ID of the parameter used to configure the Channel aware mode */
#define PARAM_ID_EVS_ENC_CHANNEL_AWARE_MODE 0x0800114A


/** @h2xmlp_parameter   {"PARAM_ID_EVS_ENC_CHANNEL_AWARE_MODE", PARAM_ID_EVS_ENC_CHANNEL_AWARE_MODE}
    @h2xmlp_description {Parameter for setting the channel aware mode on the encoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct param_id_evs_enc_channel_aware_mode_t
{
   uint32_t enable;
   /**< @h2xmle_description {Enables the Channel Aware mode}
        @h2xmle_rangeList   {Disabled=0;Enabled=1}
        @h2xmle_default     {0} */

   uint32_t fec_offset;
   /**< @h2xmle_description {Forward error correction offset}
        @h2xmle_rangeList   {"2" = 2; "3" = 3; "5" = 5; "7" = 7}
        @h2xmle_default     {2} */

   uint32_t fer_rate;
   /**< @h2xmle_description {Frame error rate threshold}
        @h2xmle_rangeList   {"0" = 0; "1" = 1}
        @h2xmle_default     {0} */
}
#include "gk_end_pack.h"
;
typedef struct param_id_evs_enc_channel_aware_mode_t param_id_evs_enc_channel_aware_mode_t;

/* Unique Paramter id */
#define PARAM_ID_EVS_ENC_DTX_MODE 0x08001199

/* Structure definition for Parameter */

/** @h2xmlp_parameter   {"PARAM_ID_EVS_ENC_DTX_MODE", PARAM_ID_EVS_ENC_DTX_MODE}
    @h2xmlp_description {Enables/Disables the Discontinuous Transmission Mode}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct param_id_evs_enc_dtx_mode_t
{
	uint32_t dtx_mode;
	/**< @h2xmle_description {Enables the DTX transmission mode, either of Adaptive or non-adaptive}
     @h2xmle_rangeList     { "DTX Disabled"=0; "DTX Enabled"=1; "Adaptive DTX Enabled"=2 }
     @h2xmle_default       { 0 } */
}
#include "gk_end_pack.h"
;
typedef struct param_id_evs_enc_dtx_mode_t param_id_evs_enc_dtx_mode_t;

/** ID of the parameter used to configure the decoder output sampling rate */
#define PARAM_ID_EVS_DEC_OUT_SAMPLING_RATE 0x0800114E


/** @h2xmlp_parameter   {"PARAM_ID_EVS_DEC_OUT_SAMPLING_RATE", PARAM_ID_EVS_DEC_OUT_SAMPLING_RATE}
    @h2xmlp_description {Parameter for setting the output sampling rate for the EVS decoder module}
    @h2xmlp_toolPolicy  {Calibration} */

#include "gk_begin_pack.h"
struct param_id_evs_dec_out_sampling_rate_t
{
	   uint32_t sampling_rate;
	   /**< @h2xmle_description {EVS decoder output sampling rate}
	        @h2xmle_rangeList   {"SAMPLE_RATE_8K"=0,
	        					 "SAMPLE_RATE_16K"=1,
	        					 "SAMPLE_RATE_32K"=2,
	        					 "SAMPLE_RATE_48K"=3}
	        @h2xmle_default     {2}
	        @h2xmle_policy      {Basic} */
}
#include "gk_end_pack.h"
;
typedef struct param_id_evs_dec_out_sampling_rate_t param_id_evs_dec_out_sampling_rate_t;

/************************************************************************************************************************/


/********************************************** Media Format Parameter ************************************************/
/**
 *  Param that indicates media format of the future buffers in the stream.

    This param-id is accepted only when subgraph is in STOP/PREPATE state.

    Some decoders may not work without receiving either
    PARAM_ID_MEDIA_FORMAT or DATA_CMD_WR_SH_MEM_EP_MEDIA_FORMAT

    Payload is of type media_format_t

    This param-id is set using APM_CMD_SET_CFG
 */
#define PARAM_ID_MEDIA_FORMAT       0x0800100C

/** @h2xmlp_parameter   {"PARAM_ID_MEDIA_FORMAT", PARAM_ID_MEDIA_FORMAT}
    @h2xmlp_toolPolicy  {Calibration}
    @h2xmlp_description {Parameter for setting the media format on any Shared Memory End Point module.} */

#include "gk_begin_pack.h"
#include "gk_begin_pragma.h"
/**
 * Immediately following payload_size amount of bytes
 * which represent the actual media-fmt block
 *
 * Some opcodes that use this payload are:
 * -PARAM_ID_MEDIA_FORMAT
 * -DATA_CMD_WR_SH_MEM_EP_MEDIA_FORMAT
 * -DATA_EVENT_ID_RD_SH_MEM_EP_MEDIA_FORMAT
 *
 * Payload struct
 * struct
 * {
 *    media_format_t mf;
 *    uint8_t payload[payload_size];
 * }
 */
struct media_format_t
{
   uint32_t data_format;
    /**< @h2xmle_description {Format of the data}
         @h2xmle_default     {0}
         @h2xmle_rangeList   {"INVALID_VALUE"=0,
                              "DATA_FORMAT_FIXED_POINT"=1,
                              "DATA_FORMAT_IEC61937_PACKETIZED"=2,
                              "DATA_FORMAT_IEC60958_PACKETIZED"=3,
                              "DATA_FORMAT_DSD_OVER_PCM"=4,
                              "DATA_FORMAT_GENERIC_COMPRESSED"=5,
                              "DATA_FORMAT_RAW_COMPRESSED"=6}
         @h2xmle_policy      {Basic} */

   uint32_t fmt_id;
    /**< @h2xmle_description {Format ID of the data stream}
         @h2xmle_default     {0}
         @h2xmle_rangeList   {"INVALID_VALUE"=0,
                              "Media format ID of PCM"=MEDIA_FMT_ID_PCM,
                              "Media format ID of AAC"=MEDIA_FMT_ID_AAC,
                              "Media format ID of ALAC"=MEDIA_FMT_ID_ALAC,
                              "Media format ID of APE"=MEDIA_FMT_ID_APE,
                              "Media format ID of FLAC"=MEDIA_FMT_ID_FLAC,
                              "Media format ID of VORBIS"=MEDIA_FMT_ID_VORBIS,
                              "Media format ID of WMAPro"=MEDIA_FMT_ID_WMAPRO,
                              "Media format ID of WMAStd"=MEDIA_FMT_ID_WMASTD,
                              "Media format ID of SBC"=MEDIA_FMT_ID_SBC,
                              "Media format ID of MP3"=MEDIA_FMT_ID_MP3,
                              "Media format ID of AMRWB+"=MEDIA_FMT_ID_AMRWBPLUS}
         @h2xmle_policy      {Basic} */

   uint32_t payload_size;
    /**< @h2xmle_description {Size of the payload immediately following this structure.\n
                              The struct of payload is defined by combination of data_format and fmt_id.
                              E.g. PCM fixed point (payload_media_fmt_pcm_t) and floating point may have different payloads.
                              This size does not include bytes added for 32-bit alignment}
         @h2xmle_default     {0}
         @h2xmle_range       {0..0xFFFFFFFF}
         @h2xmle_policy      {Basic} */

   uint8_t  payload[0];
    /**<
         @h2xmle_description {Payload}
         h2xmle_rangeList    {"For fmt_id = PARAM_ID_PCM_OUTPUT_FORMAT_CFG and data_format=DATA_FORMAT_FIXED_POINT"=payload_media_fmt_pcm_t}
         @h2xmle_policy      {Basic}
         h2xmle_variableArraySize { (payload_size*4+3) / 4}*/
}
#include "gk_end_pragma.h"
#include "gk_end_pack.h"
;
typedef struct media_format_t media_format_t;

#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* MEDIA_FMT_API_H */

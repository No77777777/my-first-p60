#ifndef __VS_VOC4GV_H__
#define __VS_VOC4GV_H__

/**
  @file vs_voc4gv.h
  @brief This file contains the definitions of the constants,
         data structures, and interfaces for FOURGV vocoder

*/

/*
  ============================================================================
  Copyright (C) 2017 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc. 
  ============================================================================ 
  $Header: //components/rel/avs.mpss/9.0.c51/api/vs/vs_voc4gv.h#1 $
  $Author: pwbldsvc $
  ============================================================================
*/

/****************************************************************************
 * INCLUDE HEADER FILES                                                     *
 ****************************************************************************/

#include "mmdefs.h"

/****************************************************************************
 * Module ID and media ID DEFINITIONS                                                  *
 ****************************************************************************/
#define VS_VOC4GV_ENC_MODULE_ID ( 0x07001092 )
#define VS_VOC4GV_DEC_MODULE_ID ( 0x07001091 )

/** EVRC-B vocoder format.*/
#define VS_VOCEVRCB_MEDIA_ID ( 0x00010FC3 )

/** EVRC-NW vocoder format. */
#define VS_VOCEVRCNW_MEDIA_ID ( 0x00010FC5 )

/** EVRC-NW2K vocoder format. */
#define VS_VOCEVRCNW2K_MEDIA_ID ( 0x00010FD7 )

/** EVRC-WB vocoder format.*/
#define VS_VOCEVRCWB_MEDIA_ID ( 0x00010FC4 )


/** 
 * Four GV average encoding rates for active speech.
 *
 * It indicates the capacity operating point of encoder.
 *
 * Encoder rates of each Four-GV encoder corresponding to enum values in vs_voc4gv_avg_enc_rate_t:
 * encoder rates of EVRC-B :{10000, 8500, 7500, 7000, 6600, 6200, 5800, 4800}
 * encoder rates of EVRC-WB: {8500, 0, 0, 0, 10000, 0, 0, 4800}
 * encoder rates of EVRC- NW: {8500, 10000, 7500, 7000, 6600, 6200, 5800, 4800}
 * encoder rates of EVRC-NW 2K: {8500, 10000, 7500, 2400, 6600, 6200, 5800, 4800}
 */
typedef enum vs_voc4gv_avg_enc_rate_t {

  VS_VOC4GV_AVG_RATE_COP_0 = 0,
  VS_VOC4GV_AVG_RATE_COP_1 = 1,
  VS_VOC4GV_AVG_RATE_COP_2 = 2,
  VS_VOC4GV_AVG_RATE_COP_3 = 3,
  VS_VOC4GV_AVG_RATE_COP_4 = 4,
  VS_VOC4GV_AVG_RATE_COP_5 = 5,
  VS_VOC4GV_AVG_RATE_COP_6 = 6,
  VS_VOC4GV_AVG_RATE_COP_7 = 7,
} vs_voc4gv_avg_enc_rate_t;


/**
 * FourGV encoders frame rate information. 
 */
typedef enum vs_voc4gv_frame_rate_t
{
  VS_VOC4GV_FRAME_BLANK_RATE = 0,
    /**< Blank. */
  VS_VOC4GV_FRAME_EIGHTH_RATE = 1,
    /**< Rate 1/8. */
  VS_VOC4GV_FRAME_QUARTER_RATE = 2,
    /**< Rate 1/4. */
  VS_VOC4GV_FRAME_HALF_RATE = 3,
    /**< Rate 1/2. */
  VS_VOC4GV_FRAME_FULL_RATE = 4,
    /**< Rate 1. */
  VS_VOC4GV_FRAME_ERASURE_RATE = 14,
    /**< Erasure rate; applicable only for downlink. */
  VS_VOC4GV_FRAME_EIGHTH_RATE_NONCRITICAL = 255,
   /** <Non critical 1/8 rate. */
  
} vs_voc4gv_frame_rate_t;


/**
 * Frame Header structures of FOURGV encoders..
 */
typedef struct vs_vocevrcb_frame_header_t {

  uint8_t rate;
    /**< frame rate. */

} vs_vocevrcb_frame_header_t;

#endif /* __VS_VOC4GV_H__ */

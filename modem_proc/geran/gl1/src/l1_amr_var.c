/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      A M R  - G L O B A L    D A T A

GENERAL DESCRIPTION
  This file contains global data needed by AMR Full and Half Rate feature.

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_amr_var.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
06/06/13   cs       Further TSTS Updates
21/05/13   cs       TSTS Updates
05/03/05   pp       Changes to support 14.10 test case and CMC jumps of more
                    than one step
03/18/04   gsc      Lint Cleanup
02/18/04   gsc      Added new global amr_thresh_hyst_defined
10/03/03   gsc      U/L codec rate adaptation changes
08/11/03   gsc      Made curr_norm_c_over_i uint16.
05/07/03   gsc      First revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif


#ifdef FEATURE_GSM_AMR
#include "l1_amr.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/*---------------------------------------------------------------------------
 *                       GLOBAL DATA
 *-------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 * Phase data
 *-------------------------------------------------------------------------*/
cmi_phase_type curr_phase[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( CMI_PHASE_ODD ) };

/*---------------------------------------------------------------------------
 * Current UL/DL ACS and ICM, Number of Codec Modes in ACS
 *-------------------------------------------------------------------------*/
uint8 curr_dl_icm[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };
uint8 curr_ul_icm[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };

#define GL1_CURR_DL_ACS_INIT \
                 { GL1_MSG_AMR_MODE_UNDEF, GL1_MSG_AMR_MODE_UNDEF, \
                   GL1_MSG_AMR_MODE_UNDEF, GL1_MSG_AMR_MODE_UNDEF }

gl1_msg_amr_mode_type curr_dl_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES] =
{
    GL1_CURR_DL_ACS_INIT
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  , GL1_CURR_DL_ACS_INIT
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#if defined ( FEATURE_TRIPLE_SIM )
  , GL1_CURR_DL_ACS_INIT
#endif /* FEATURE_TRIPLE_SIM */
};

#define GL1_CURR_UL_ACS_INIT \
                 { GL1_MSG_AMR_MODE_UNDEF, GL1_MSG_AMR_MODE_UNDEF, \
                   GL1_MSG_AMR_MODE_UNDEF, GL1_MSG_AMR_MODE_UNDEF }

gl1_msg_amr_mode_type curr_ul_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES] =
{
    GL1_CURR_UL_ACS_INIT
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  , GL1_CURR_UL_ACS_INIT
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#if defined ( FEATURE_TRIPLE_SIM )
  , GL1_CURR_UL_ACS_INIT
#endif /* FEATURE_TRIPLE_SIM */
};

/*---------------------------------------------------------------------------
 * Num of codec modes in ACS
 *-------------------------------------------------------------------------*/
uint8 curr_dl_num_acm[NUM_GERAN_DATA_SPACES];
uint8 curr_ul_num_acm[NUM_GERAN_DATA_SPACES];

/*---------------------------------------------------------------------------
 * Current Threshold/Hysteresis data
 *-------------------------------------------------------------------------*/
#define GL1_CURR_THRESH_TBL_INIT { 0x3F, 0x3F, 0x3F }

uint8 curr_thresh_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1] = {
          GL1_CURR_THRESH_TBL_INIT
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        , GL1_CURR_THRESH_TBL_INIT
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#if defined ( FEATURE_TRIPLE_SIM )
        , GL1_CURR_THRESH_TBL_INIT
#endif /* FEATURE_TRIPLE_SIM */
};

#define GL1_CURR_HYST_TBL_INIT { 0xF, 0xF, 0xF }

uint8 curr_hyst_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1] = {
          GL1_CURR_HYST_TBL_INIT
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        , GL1_CURR_HYST_TBL_INIT
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#if defined ( FEATURE_TRIPLE_SIM )
        , GL1_CURR_HYST_TBL_INIT
#endif /* FEATURE_TRIPLE_SIM */
};

/*---------------------------------------------------------------------------
 * Current CMC/CMR; Current UL/DL CMI; Computed CMR; amr_channel
 *-------------------------------------------------------------------------*/
uint8 curr_cmc[NUM_GERAN_DATA_SPACES];
uint8 new_cmc[NUM_GERAN_DATA_SPACES];
uint8 curr_cmr[NUM_GERAN_DATA_SPACES];
uint8 computed_cmr[NUM_GERAN_DATA_SPACES];
uint8 curr_ul_cmi[NUM_GERAN_DATA_SPACES];
uint8 curr_dl_cmi[NUM_GERAN_DATA_SPACES];
uint32 curr_norm_c_over_i[NUM_GERAN_DATA_SPACES];

int32 prev_c_over_i_filter_output[NUM_GERAN_DATA_SPACES] =
        { INITIAL_VALUE( 0 ) };

channel_type_T amr_channel[NUM_GERAN_DATA_SPACES];
boolean amr_thresh_hyst_defined[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( TRUE ) };

#endif /* FEATURE_GSM_AMR */

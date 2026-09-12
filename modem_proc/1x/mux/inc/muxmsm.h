#ifndef MUXMSM_H
#define MUXMSM_H
/*===========================================================================

           M S M   R E G I S T E R   A C C E S S   M A C R O S

                F O R   T H E   M U X   S U B S Y S T E M

                          H E A D E R    F I L E

DESCRIPTION
  This file contains macros to support interaction with the QUALCOMM
  MSM Register Set by the MUX subsystem (including the encoder and decoder).

  Copyright (c) 2003 - 2013 Qualcomm Technologies, Inc. 
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/



/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/mux/inc/muxmsm.h#1 $



when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/15/13   srk     Replace mcs hwio with m1x hwio
08/18/11   adw     Mainline 1x message router support.
07/01/10   jtm     Only include rf_mdsp_tx_agc.h if FEATURE_1X_SUPPORTS_MSGR
                   is *not* defined.
06/02/10   trc     Remove MUXMSM_SET_MOD_MODE_1X macro
05/18/10   vks     Mainline FEATURE_MDSP_SUPPORTS_TX_AGC.
05/14/10   vks     Move MUXMSM_AGC_CTL_MASK_DELAY functionality to muxmdsp.c
                   (muxmdsp_set_agc_ctl_mask_delay() )
05/07/10   vks     Fix lint/compiler warnings.
05/07/10   sst     Updates for 1.1 first call
03/04/10   vks     Call RF-FW update macro everytime a FW register is touched
                   using RF macros
10/08/09   vks     Moved mdsp read/writes in muxmsm.h to muxmdsp.h and
                   renamed these macros from muxmsm* to muxmdsp*
04/01/09   jtm     Added customer.h and comdef.h and featurized necessary includes.
02/13/07   vlc/vm  Fix broken RPC in TX Gating Mode by changing value in
                   MUXMSM_AGC_CTL_MASK_DELAY macro (for feature
                   FEATURE_MDSP_SUPPORTS_TX_AGC only.)
01/09/07   vlc     Merged the following change to main line from SC2X branch.
                   Mainly, adding support for FEATURE_MDSP_SUPPORTS_TX_AGC.
           vm      Added the support for Firmware TX
06/09/06   trc     Remove T_MSM6700 featurization
05/31/05   ejv     Include msm_help.h.
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
10/27/04   jyw     added demod macros from the msm_drv.h
09/28/04   sr      Merged in changes from 6550 2009 build
08/09/04   ejv     Added some DEM1X macros from msm_drv.h.
04/20/04   jrp     Fixed lint warnings.
04/09/04   bn      added macro for getting SCH NT_IO and WTD_PILOT
11/05/03   ejv     Added MUXMSM_FPC_IQACC_SCH_SCALE.
10/20/03   vlc     Undid changes to MUXMSM_GET_TD_STATUS_DONE.
09/25/03   vlc     Featurize MUXMSM_GET_TD_STATUS_DONE modification.
09/24/03   vlc     Modified MUXMSM_GET_TD_STATUS_DONE.
07/21/03   ejv     Added MUXMSM_DEMOD_SET_FRAME_OFFSET.
06/06/03   dna     Created file.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "m1x_hwio_mpss.h"

//other
#include "msm_drv_i.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*===========================================================================

  COMBINER CONTROL REGs for CH4/CH5; used for GCH

===========================================================================*/
#define DEMOD_ENABLE_CH4_HW_COMBINING() \
    MSM_SET( DEM1X_CHANNEL4_FRAME, HW_EN )
#define DEMOD_DISABLE_CH4_HW_COMBINING() \
    MSM_CLEAR( DEM1X_CHANNEL4_FRAME, HW_EN )

#define DEMOD_ENABLE_CH5_HW_COMBINING() \
    MSM_SET( DEM1X_CHANNEL5_FRAME, HW_EN )
#define DEMOD_DISABLE_CH5_HW_COMBINING() \
    MSM_CLEAR( DEM1X_CHANNEL5_FRAME, HW_EN )

#define DEMOD_SET_MAC_RND_CH4( decoder ) \
    MSM_DRV_W_MASKED( DEM1X_CHANNEL4_FRAME, SOFTDEC_RND, (decoder) )
#define DEMOD_SET_MAC_RND_CH5( decoder ) \
    MSM_DRV_W_MASKED( DEM1X_CHANNEL5_FRAME, SOFTDEC_RND, (decoder) )

#define DEMOD_SET_CH4_SPR(spr)\
    MSM_DRV_W_MASKED( DEM1X_CHANNEL4_FRAME, SPR_MASK, (spr) )
#define DEMOD_SET_CH5_SPR(spr)\
    MSM_DRV_W_MASKED( DEM1X_CHANNEL5_FRAME, SPR_MASK, (spr) )

#define DEMOD_SET_SW_BETA_CH4( scale ) \
    MSM_DRV_W_MASKED( DEM1X_CHANNEL4_FRAME, SW_BETA, (scale)  )
#define DEMOD_SET_SW_BETA_CH5( scale ) \
    MSM_DRV_W_MASKED( DEM1X_CHANNEL5_FRAME, SW_BETA, (scale)  )


/*===========================================================================

  Fowrard Power Control

===========================================================================*/

/*==========================================================================
MACRO MUXMSM_FPC_ENABLE
MACRO MUXMSM_FPC_DISABLE

DESCRIPTION    These macros are used for enabling/disabling forward power
               control.

DEPENDECIES    These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.


RETURN VALUE   None

SIDE EFFECTS   None.

=============================================================================*/

#define MUXMSM_FPC_ENABLE( ) \
    MSM_DRV_SET( DEM1X_COMBINER_CTL, FPC_EN )

#define MUXMSM_FPC_DISABLE( ) \
    MSM_DRV_CLEAR( DEM1X_COMBINER_CTL, FPC_EN )

/*===========================================================================

MACRO MUXMSM_FPC_SEL

DESCRIPTION

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#define MUXMSM_FPC_SEL( val ) \
   MSM_DRV_W_MASKED( DEM1X_FWD_PWR_CTL_FRAME, FPC_SEL, (val) )

/*===========================================================================

MACRO MUXMSM_FPC_WALSH_GAIN

DESCRIPTION

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/

#define MUXMSM_FPC_WALSH_GAIN( val ) \
   MSM_DRV_W_MASKED( DEM1X_FWD_PWR_CTL_FRAME, WALSH_GAIN, (val) )

/*===========================================================================

MACRO MUXMSM_FPC_SCH_POS_DSP

DESCRIPTION
  Control the timing strobe to DSP

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/

#define MUXMSM_FPC_SCH_POS_DSP( val ) \
   MSM_DRV_W_MASKED ( DEM1X_FWD_PWR_CTL_TIMING, SCH_POS_DSP, (val) )

/*===========================================================================

MACRO MUXMSM_FPC_SCH_POS
MACRO MUXMSM_FPC_POSITION1
MACRO MUXMSM_FPC_POSITION2

DESCRIPTION

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#define MUXMSM_FPC_SCH_POS( val ) \
   MSM_DRV_W_MASKED( DEM1X_FWD_PWR_CTL_TIMING, SCH_POS, (val) )
#define MUXMSM_FPC_POSITION1( val ) \
   MSM_DRV_W_MASKED( DEM1X_FWD_PWR_CTL_TIMING, POS1, (val) )
#define MUXMSM_FPC_POSITION2( val ) \
   MSM_DRV_W_MASKED( DEM1X_FWD_PWR_CTL_TIMING, POS2, (val) )

/*===========================================================================

  Reverse Power Control

===========================================================================*/

/*===========================================================================

MACRO MUXMSM_RPC_SET_POWER_CTL

DESCRIPTION

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
//lint -emacro(506, MUXMSM_RPC_SET_POWER_CTL)  Constant Value Boolean
#define MUXMSM_RPC_SET_POWER_CTL(power_ctl_pct, punc_pos_sel, pc_even)        \
    HWIO_OUTM(DEM1X_TRAFFIC_REV_PWR_CTL,                                      \
      HWIO_FMSK(DEM1X_TRAFFIC_REV_PWR_CTL,POWER_CTL_PCT) |                    \
      HWIO_FMSK(DEM1X_TRAFFIC_REV_PWR_CTL,PUNC_POS_SEL)  |                    \
      HWIO_FMSK(DEM1X_TRAFFIC_REV_PWR_CTL,PC_EVEN),                           \
      ((power_ctl_pct)?HWIO_FMSK(DEM1X_TRAFFIC_REV_PWR_CTL,POWER_CTL_PCT):0)| \
      ((punc_pos_sel) ?HWIO_FMSK(DEM1X_TRAFFIC_REV_PWR_CTL,PUNC_POS_SEL) :0)| \
      ((pc_even)      ?HWIO_FMSK(DEM1X_TRAFFIC_REV_PWR_CTL,PC_EVEN)      :0))

/*===========================================================================

                              DEMODULATOR

===========================================================================*/


/*==========================================================================
MACRO MUXMSM_SET_PILOT_GATING_MASK

DESCRIPTION

    This bitfield is used to program the mask value, which is used to
    determine when to disable the pc_symbol and not_pc_decision flags
    to the QDSP. A '1' is used to disable these signals to the QDSP during
    an entire PCG.
    Program these 4 bits as given below depending on the gating rate.

    Mask Value Gating Rate
    ---------- -----------
    0000       1
    1010       1/2
    1101       1/4

DEPENDECIES    These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE   None

SIDE EFFECTS   None.

=============================================================================*/

#define MUXMSM_SET_PILOT_GATING_MASK( mask ) \
        MSM_DRV_W_MASKED( DEM1X_COMBINER_CTL, PILOT_GATING_MASK, (mask) )

/*==========================================================================
MACRO MUXMSM_DEMOD_SET_FRAME_OFFSET

DESCRIPTION

    This bitfield is used to specify the frame offset in
    units of PCG (0-15).  It is used to compute the frame
    count and establish frame boundaries.

DEPENDECIES    These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE   None

SIDE EFFECTS   Frame boundary may change.

=============================================================================*/

#define MUXMSM_DEMOD_SET_FRAME_OFFSET( offset ) \
        MSM_DRV_W_MASKED( DEM1X_FRAME_OFFSET, DATA, (offset) )

/*=========================================================================
MACRO DEMOD_ENABLE_MAC_ENGINE_CH1
MACRO DEMOD_DISABLE_MAC_ENGINE_CH1

DESCRIPTION    These macros enable/disable the MAC engine for CH1.

DEPENDENCIES   These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE   None

SIDE EFFECTS   None
==========================================================================*/

#define DEMOD_ENABLE_CH1_HW_COMBINING() \
    MSM_DRV_SET( DEM1X_CHANNEL1_FRAME, HW_EN )

#define DEMOD_DISABLE_CH1_HW_COMBINING() \
    MSM_DRV_CLEAR( DEM1X_CHANNEL1_FRAME, HW_EN )

/*=========================================================================
MACRO DEMOD_ENABLE_MAC_ENGINE_CH2
MACRO DEMOD_DISABLE_MAC_ENGINE_CH2
MACRO DEMOD_ENABLE_CHx_HW_COMBINING

DESCRIPTION    These macros enable/disable the MAC engine for CH2.

DEPENDENCIES   These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE   None

SIDE EFFECTS   None
==========================================================================*/

#define DEMOD_ENABLE_MAC_ENGINE()         DEMOD_ENABLE_CH2_HW_COMBINING()
#define DEMOD_ENABLE_MAC_ENGINE_FOR_CH2() DEMOD_ENABLE_CH2_HW_COMBINING()

#define DEMOD_ENABLE_CH2_HW_COMBINING() \
    MSM_DRV_SET( DEM1X_CHANNEL2_FRAME, HW_EN )

#define DEMOD_DISABLE_CH2_HW_COMBINING() \
    MSM_DRV_CLEAR( DEM1X_CHANNEL2_FRAME, HW_EN )

#define DEMOD_ENABLE_CH3_HW_COMBINING() \
    MSM_DRV_SET( DEM1X_CHANNEL3_FRAME, HW_EN )

#define DEMOD_DISABLE_CH3_HW_COMBINING() \
    MSM_DRV_CLEAR( DEM1X_CHANNEL3_FRAME, HW_EN )

/*===========================================================================

                                  DECODER

===========================================================================*/

/*===========================================================================

MACRO MUXMSM_GET_TD_STATUS_DONE

DESCRIPTION

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#define MUXMSM_GET_TD_STATUS_DONE() \
        MSM_DRV_R_MASKED( TD_STATUS, DONE )


/*===========================================================================

                                  TX AGC

===========================================================================*/


/*===========================================================================

MACRO MUXMSM_AGC_CTL_TX_MSM3100_MODE

DESCRIPTION

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#define MUXMSM_AGC_CTL_TX_MSM3100_MODE() \
  MSM_DRV_SET( TX_AGC_CTL2, TX_MSM3100_MODE )






/*===========================================================================

                              MISCELLANEOUS

===========================================================================*/


/*===========================================================================

MACRO MUXMSM_GET_TIME_INT_PHASE

DESCRIPTION

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#define MUXMSM_GET_TIME_INT_PHASE() \
          ((MSM_DRV_R_MASKED( DEM1X_TIME_INT_PHASE, TIME_INT2_PHASE )) & 0x3)

#endif /* MUXMSM_H */


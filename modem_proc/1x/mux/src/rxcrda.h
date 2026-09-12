#ifndef RXCRDA_H
#define RXCRDA_H

/*===========================================================================

       C D M A    R A T E    D E T E R M I N A T I O N   M O D U L E

DESCRIPTION
  This header file contains definitions for the rate determination module
  that are used by the rxc unit.  This header file is internal to the rxc
  unit.

  Copyright (c) 1997 - 2016 Qualcomm Technologies, Inc.
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

$PVCSPath: O:/src/asw/COMMON/vcs/rxcrda.h_v   1.7   31 May 2002 18:35:22   lchan  $
$Header: //components/rel/1x.mpss/8.0/mux/src/rxcrda.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/12/16   srk     Use locally stored/passed in RC instead of global variable
                   to avoid race condition between RX Task and TC ISR.
06/14/13   pap     Aggregating different global variables into structures.
02/24/12   srk     Mainlined FEATURE_IS2000_P2
12/13/11   srk     Feature clean up: Mainlined FEATURE_RDA_CORRECTION
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
12/03/10   jtm     Removed MDR/SCCH support.
10/30/10   jtm     Modem 1x SU API changes.
05/05/10   jtm     Added support for new RLP API using FEATURE_IS95_RLP_MUX_COMMON_API
09/14/09   adw     Changes to support modem data header CMI refactoring.
08/05/09   jtm     Lint fixes.
03/26/09   jtm     Eliminated implicit includes.
03/06/07   vlc     Fixed compiler errors when REL A is turned off.
04/20/04   jrp     Fixed lint warnings.
05/31/02   lcc     Added rxcrda_update_gating_rate to inform RDA of gating
                   rate when in control hold.
05/30/02   vlc     VP2 changes are now featurized under FEATURE_IS2000_REL_A_SVD
                   and FEATURE_IS2000_P2 per request of development team.
                   Implemented comments from VP2 code review.
05/13/02   vlc     Added rxc_dtx_det_dcch() for DTX detection on DCCH
                   (feature FEATURE_IS2000_REL_A_VP2.)
09/12/01   bgc     Renamed filter for SCH and added filter function for DCCH.
06/15/01   day     Delete FEATURE_SPECIAL_MDR
04/20/01   sr      Merged in from MSM5100 archive
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           bgc     Added rxc_dtx_det_dcch() for DTX detection on DCCH.
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
//common
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

//mux
#include "rxc_i.h"

//drivers
#include "dec5000.h"
#include "dec5000_v.h"

//cp
#include "cai_v.h"

//other
#include "mdrrlp_api.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* Indicates whether supplemental frame is full rate or erasure */
typedef enum
{
  RXC_SUP_ERASURE = 0,
  RXC_SUP_FULL
}rxc_tc_sup_rate_type;

typedef struct
{
  /* debug flag to output RDA related debug message */
  boolean rxcrda_dtx_msg_enable;
  int8    rxcrda_sch_dtx_fix_counter;
  int8    rxcrda_dcch_dtx_fix_counter;
} rxcrda_counter_data_t;

extern rxcrda_counter_data_t rxcrda_data;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION RXC_DEC_RATE_SEL

DESCRIPTION
  This function calls the appropriate rate procedure to
  process the decoder data received from the Traffic Channel
  and determine the data rate depending on the current rate set.

DEPENDENCIES
  None.

RETURN VALUE
  Decoder rate.

SIDE EFFECTS
  None.

===========================================================================*/
extern cai_data_rate_type rxc_dec_rate_sel( deint_rc_type curr_rc );

/*===========================================================================

FUNCTION RXC_MAR_RATE_SEL

DESCRIPTION
  This function determines the proper rate to input to the vocoder based
  on the frame CAI category ( mux1 category ).

DEPENDENCIES
  None.

RETURN VALUE
  Vocoder frame rate.

SIDE EFFECTS
  None.

===========================================================================*/
extern cai_data_rate_type rxc_mar_rate_sel( word mux1 );

/*===========================================================================

FUNCTION RXC_VOC_RATE_SEL

DESCRIPTION
  This function determines the proper rate to input to the vocoder based
  on the frame CAI category ( mux1 category ).

DEPENDENCIES
  None.

RETURN VALUE
  Vocoder frame rate.

SIDE EFFECTS
  None.

===========================================================================*/
extern cai_data_rate_type rxc_voc_rate_sel( word mux1 );

/*===========================================================================

FUNCTION RXCRDA_SCH_CORRECTION

DESCRIPTION
  If a DTX frame was incorrectly detected as ERASURE, this function tries to
  re-classify it as DTX.  It is a filter to encourage runs of DTX and correct
  sporadic erasures.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
rxc_frame_type rxc_sch_dtx_filter
(
  rxc_frame_type frame_status
);

/*===========================================================================

FUNCTION RXCRDA_DCCH_CORRECTION

DESCRIPTION
  If a DTX frame was incorrectly detected as ERASURE, this function tries to
  re-classify it as DTX.  It is a filter to encourage runs of DTX and fix
  sporadic erasures that should have been classified as DTX.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
rxc_frame_type rxc_dcch_dtx_filter
(
  rxc_frame_type frame_status
);

/*========================================================================

FUNCTION RXC_DTX_DET_DCCH

DESCRIPTION This function determines if the current frame is a DTX frame,
an erasure frame, or a full frame in the dedicated control channel.

DEPENDENCIES None

RETURN VALUE enum, either RXC_GOOD_FRAME, RXC_DTX_FRAME, or RXC_ERASURE_FRAME


SIDE EFFECTS None

=========================================================================*/
extern cai_data_rate_type rxc_dtx_det_dcch( dec_rc_type rc );

/*========================================================================

FUNCTION RXCRDA_UPDATE_GATING_RATE

DESCRIPTION This function is used to update the pc_acc_scaler variable which is
            used to scale the pc_acc in DCCH channel quality and DTX detection.
            This update is actually done in 2 steps.  The new value is first copied
            into a temporary storage (pending_pc_acc_scaler).  The function argument
            also contains the system time when the new value should become effective.
            The actual update is done in rxc_dtx_det_dcch, when the specified time
            arrives.

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxcrda_update_gating_rate
(
  cai_pilot_gating_rate_type gating_rate
);
#endif /* RXCRDA_H */

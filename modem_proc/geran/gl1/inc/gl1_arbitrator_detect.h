#ifndef GARB_DETECT_H
#define GARB_DETECT_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                    G L 1  A R B I T R A T O R  D E T E C T

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/gl1_arbitrator_detect.h#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
26/12/17   sn        CR2160741: Gl1 Coex timeline updation at Wakeup Cancel of Idle Sub
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
14/05/15   mc        CR.821519 : TH 2.0: WLAN COEX timeline reporting errors for FACCH and SDCCH
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
03/12/14   npt       CR.759467 : Fix compiler warnings
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
04/11/13   mc        CR.556742 : G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
23/09/13   sk        2.0 syncup from 1.1
29/07/13   sk        CR519663 Partial QBTA bringup changes
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifndef CUSTOMER_H
  #include "customer.h"
#endif
#ifndef COMDEF_H
  #include "comdef.h"
#endif

/*===========================================================================

                         DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                        PUBLIC DECLARATIONS

===========================================================================*/

#ifdef FEATURE_GSM_COEX
/*===========================================================================

FUNCTION      garb_detect_and_resolve_rx_conflicts

DESCRIPTION   Detects any conflict between Tx activities on this GAS and
              activities on the peer GAS. Performs arbitration if a conflict
              is detected.

              In the case of DSDA a conflict could be an RxTx desense - RxRx
              conflicts are not a concern in DSDA.

              This function will set the CXM priority tier for the specified
              activity. If there is a conflict then the CXM priority tier
              will be selected based on conflict resolution, otherwise the
              selected CXM priority will be a default based on channel type.

RETURN VALUE  None

===========================================================================*/
void garb_detect_and_resolve_rx_conflicts(garb_frame_t* frame, uint8 ts, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_detect_and_resolve_tx_conflicts

DESCRIPTION   Detects any conflict between Tx activities on this GAS and
              activities on the peer GAS. Performs arbitration if a conflict
              is detected.

              In the case of DSDA a conflict could be an RxTx desense or TxTx
              VBATT droop or SAR violation.

              This function will set the CXM priority tier for the specified
              activity. If there is a conflict then the CXM priority tier
              will be selected based on conflict resolution, otherwise the
              selected CXM priority will be a default based on channel type.

RETURN VALUE  None

===========================================================================*/
void garb_detect_and_resolve_tx_conflicts(garb_frame_t* frame, uint8 ts, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_detect_and_resolve_monitor_conflicts

DESCRIPTION   Detects any conflict between monitor activities on this GAS
              and activities on the peer GAS. Performs monitor arbitration
              if a conflict is detected.

RETURN VALUE  TRUE if no-zones were configured, FALSE otherwise

===========================================================================*/
boolean garb_detect_and_resolve_monitor_conflicts(gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_reset_timeline

DESCRIPTION   This function clears the timeline of the supplied GAS.

RETURN VALUE  None

===========================================================================*/
void garb_reset_timeline(uint8 write_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_append_frame_to_timeline

DESCRIPTION   This function appends the supplied frame to the timeline
              of the supplied GAS.

RETURN VALUE  None

===========================================================================*/
void garb_append_frame_to_timeline(garb_frame_t* frame, uint8 index, uint8 write_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_get_frame

DESCRIPTION   Builds the anticipated frame for the specified FN and GAS.

RETURN VALUE  None

===========================================================================*/
boolean garb_get_frame(uint32 fn, uint8 frame_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_get_frame_at_wakeup_conflict

DESCRIPTION   Builds the anticipated frame for the specified FN and GAS at Wakeup Cancel.

RETURN VALUE  None

===========================================================================*/

void garb_get_frame_at_wakeup_conflict(uint32 fn, uint8 frame_index, gas_id_t gas_id);


#endif /*  FEATURE_GSM_COEX */

#endif /* GARB_DETECT_H */

#ifndef SRCH_IRAT_H
#define SRCH_IRAT_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                S E A R C H   I R A T   S T M   G R O U P

                         H E A D E R   F I L E


GENERAL DESCRIPTION
   This file contains exports from src/irat directory

EXTERNALIZED FUNCTIONS
   All files listed below

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Copyright (c) 2013
                Qualcomm Technologies Incorporated. All Rights Reserved
                Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.
===========================================================================*/

/*==========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/api/srch_irat.h#1 $
$DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/25/14   pk      Changes to support new RF Meas API for L to 1x meas
09/26/14   pap     Add support for API to get carrier frequency.
12/10/13   bph     Remove dependancy on L1 for cell info 
12/05/13   bph     FR3839 - IRATMan dynamic Tbackoff
12/24/11   cjb     IRAT:1x->LTE: Changes to use IRATMan for LTE list management.
08/26/11   cjb     Changes for IRAT:1x->LTE. Initial working version.
04/20/11   cjb     Fixed compilation errors.
04/20/11   cjb     SU API split for "srch_irat.h".
04/20/11   cjb     Export the APIs required by FTM test support for Conn meas.
03/29/11   cjb     Using shared memory for logging FW information.
03/25/11   cjb     LTO1X Conn meas: FTM changes to avoid delay in RSP to DIAG.
03/16/11   sst     Add support for FTM mode LTE->1x Measurements
02/23/11   adw     Removed unnused get_pilot_meas interface func.
02/17/11   bb      Update changes for unified interface between MC and SRCH
02/07/11   bb      Changes to define srch_irat_lto1x_process_resel_info() method
02/01/11   sst     Refactor of srchlte_tt_sm.* to new irat directory
01/13/11   adw     Enhancements to lto1x diag command.
01/12/11   adw     Added support for lto1x diag command.
12/16/10   adw     Modified srch_irat_lto1x_process_tt_info func params for clarity.
11/01/10   bb      Changes to support eCSFB featute
10/14/10   bb      Changes to support connected mode measurements for
                   LTE to 1X IRAT feature
10/05/10   bb      Changes to adde srch_irat_lto1x_get_pilot_meas_info defintion
09/27/10   bb      Code changes to fix the IRAT issues
01/18/10   bb      Modified LTE to 1X message structure names to
                   inline with HDR
10/22/09   bb      Initial implementation

==========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

/* Common */
#include "comdef.h"
#include "customer.h"
#include "onex_srch_lte_msg.h"

/* Srch */
#include "srchi.h"
#include "srch_rx_t.h"

/* Other */
#include "rfm_device_types.h"
#include "sys.h"


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

FUNCTION      SRCH_IRAT_GET_CARRIER_FREQ_FOR_BC
DESCRIPTION   This function will provide the carrier freq based on band\chan 
              information.

DEPENDENCIES  None
RETURN VALUE  Frequency in KHz.
SIDE EFFECTS  None

===========================================================================*/
extern uint32 srch_irat_get_carrier_freq_for_bc
(
  srch_rx_band_type         band,           /* band class */
  srch_rx_chan_type         channel         /* channel number */
);


/*==========================================================================
  =                                                                        =
  =                  LTE -> 1X FUNCTION PROTOTYPES                         =
  =                                                                        =
  ==========================================================================*/

/*===========================================================================

FUNCTION      SRCH_IRAT_LTO1X_STATE
DESCRIPTION   This function is the state handler for LTE TIME TRANSFER state.
              It will be active, whenever Time Transfer request either from
              LTE ML1 or MC.

DEPENDENCIES  N/A.
RETURN VALUE  The new state of the searcher task: CDMA or NA
SIDE EFFECTS  None

===========================================================================*/
extern srch_state_enum_type srch_irat_lto1x_state( void );

/*===========================================================================
FUNCTION      SRCH_IRAT_X_TO_ONEX_MEAS_ENTER

DESCRIPTION   This API uses the target RF information, the source tech 
              provided and sets all of the target related fields of the 
              rfm_meas_common_param_type structure. 
              And then calls the rfm_meas_common_script_enter() API
              to trigger the RF enter procedure.

DEPENDENCIES  The source tech must first initialize an rfm_meas_common_param_type 
              structure by invoking the rfm_meas_common_init_params() API, 
              and then set all of the source related fields. The common field is a 
              pointer to this structure. 

RETURN VALUE  None.

SIDE EFFECTS  None.
=========================================================================*/
extern void srch_irat_x_to_onex_meas_enter
(
  onex_srch_irat_meas_rf_params_type *rf_params
);

/*===========================================================================
FUNCTION      SRCH_IRAT_X_TO_ONEX_MEAS_EXIT

DESCRIPTION   This API calls the rfm_meas_common_script_exit() API
              to trigger the RF enter procedure.

DEPENDENCIES  rfm_meas_common_param_type structure passed to this API must be 
              the same one, earlier passed to srch_irat_x_to_onex_meas_enter().

RETURN VALUE  None.

SIDE EFFECTS  None.
=========================================================================*/
extern void srch_irat_x_to_onex_meas_exit
(
  onex_srch_irat_meas_rf_params_type *rf_params
);

/*===========================================================================

FUNCTION      SRCH_IRAT_LTO1X_FTM_MEAS_INIT
DESCRIPTION   This function will
               Connected Mode:
                 Enable FW in connected measurement mode.
                 Request RF to generate the tune scripts.
                 This will lead to a stall before FW executed RF scripts.
               Idle Mode:
                 Enable FW in idle measurement mode.
                 Request RF to tune via API's.
                 This will lead to a stall after the RF has been tuned.

DEPENDENCIES  OneX SRCH should be in the INACTIVE state
               (and preferrably all RAT's in inactive states)
RETURN VALUE  None
SIDE EFFECTS  None

===========================================================================*/
extern void srch_irat_lto1x_ftm_meas_init
(
  boolean                conn_md,          /* flag to indicate Idle
                                               or Connected Mode          */
  sys_channel_type       channel,          /* Band and Channel to measure */
  int16                  tcxomgr_rotator_type,
                                           /* Initial Rotator value       */
  rfcom_device_enum_type rf_device
);

/*===========================================================================

FUNCTION      SRCH_IRAT_LTO1X_FTM_MEAS_EXEC
DESCRIPTION   This function in
               Connected Mode:
                 Requests FW to use RF scripts to tune the RF.
               Idle Mode:
                 Move to the measurement state, as the tune was done via API's.

              This will lead to a stall before measurement are done
              (no measurements will be issued).

DEPENDENCIES  srch_irat_lto1x_ftm_meas_init shall have been called first
RETURN VALUE  None
SIDE EFFECTS  None

===========================================================================*/
extern void srch_irat_lto1x_ftm_meas_exec( void );

/*===========================================================================

FUNCTION      SRCH_IRAT_LTO1X_FTM_MEAS_EXIT
DESCRIPTION   This function will disable FW, marking the end of a measurement
               visit

DEPENDENCIES  srch_irat_lto1x_ftm_meas_exec shall have been called first
RETURN VALUE  None
SIDE EFFECTS  None

===========================================================================*/
extern void srch_irat_lto1x_ftm_meas_exit( void );


/*==========================================================================
  =                                                                        =
  =                  1X -> LTE FUNCTION PROTOTYPES                         =
  =                                                                        =
  ==========================================================================*/
/*===========================================================================

FUNCTION      SRCH_ONEXTOL_WAKEUP_CB
DESCRIPTION   This function is called by IRATMan to request 1x to do a
              LTE measurement.

DEPENDENCIES  1x protocal should be active.
RETURN VALUE  TRUE if measurement request is accepted. Else, FALSE.
SIDE EFFECTS  None.

===========================================================================*/
extern boolean srch_onextol_wakeup_cb( void );
#endif /* SRCH_IRAT_H */

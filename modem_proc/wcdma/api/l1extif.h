#ifndef L1EXTIF_H
#define L1EXTIF_H

/*===========================================================================
                              L1EXTIF.H

GENERAL DESCRIPTION

  This file contains all the routines for CFN callback mechanism. The CFN 
  callback mechansim allows any module (RRC etc) can register for a 
  callback function that will be called every CFN in all the states.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright(c)2005-2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:$
  $Header: //components/rel/wcdma.mpss/8.4.0/api/l1extif.h#3 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

 when        who     what, where, why  
 --------   ---     ----------------------------------------------------------
 06/03/15    pr      W+W Phase8 Checkin.
 04/09/15    mk      Split l1extif.h in to l1extif.h (exposed externally) and
                     l1extif_v.h (Private to wcdma)
 02/28/12    rsr     W+W Phase 2 Checkin.
 07/01/14    hk      Header file inclusion cleanup
 07/24/12    geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
 04/05/12    gsk     Moving wl1_check_state_for_delayed_gstmr_int to wl1api.h
 04/29/09    mg      Add extern l1m_print_debug_msgs
 01/13/09    vsr     Added a structure variable to change the way MAC callback
                     is avoided during HHO
 07/15/08    rgn     Added backward compatible cfn based notify function
 02/22/07    mg      Add wl1_check_state_for_delayed_gstmr_int
 07/06/06    rmak    Changed l1m_get_state to return uint8 to avoid
                     exposing l1m_state_enum_struct to non-L1 code
 07/13/05    vp      Code changes to support the new MAC-CFN update mechanism.
 --------   ---     ---------------------------------------------------------

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "wfw_sw_intf.h"

#ifdef FEATURE_WCDMA_DUAL_SYNC_INTERFACE
typedef enum
{
  START_G2W_MODE = WFW_START_MODE_X2W,
  /** START PAGE WCDMA*/
  START_PAGE_MODE = WFW_START_MODE_PAGE,
  /** START WCDMA*/
  START_WCDMA_MODE = WFW_START_MODE_WCDMA,
  /* SLEEP WCDMA */
  SLEEP_WCDMA_MODE
}mdsp_sync_start_cfg_enum_type;
#else
typedef enum
{
  START_G2W_MODE = WFW_START_MODE_X2W,
  /** START PAGE WCDMA*/
  START_PAGE_MODE = WFW_START_MODE_WCDMA,
  /** START WCDMA*/
  START_WCDMA_MODE = WFW_START_MODE_WCDMA,
  /* SLEEP WCDMA */
  SLEEP_WCDMA_MODE
}mdsp_sync_start_cfg_enum_type;
#endif /*FEATURE_DUAL_WCDMA*/

/* call back function type for failed block passing to higher layer info */
#ifdef FEATURE_UMTS_BMC_MSIM
typedef void (*DL_DEC_CTCH_PDU_DECODE_ERROR_CB_FUNC_TYPE)(dword,sys_modem_as_id_e_type);
#else
typedef void (*DL_DEC_CTCH_PDU_DECODE_ERROR_CB_FUNC_TYPE)(dword);
#endif
#endif /* L1EXTIF_H */


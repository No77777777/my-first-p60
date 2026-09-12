#ifndef L1EXTIF_V_H
#define L1EXTIF_V_H

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
  $Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/l1extif_v.h#4 $ $DateTime: 2020/03/05 09:52:21 $ $Author: pwbldsvc $

 when        who     what, where, why
 --------   ---     ----------------------------------------------------------
 04/09/15    mk      Split l1extif.h in to l1extif.h (exposed externally) and
                     l1extif_v.h (Private to wcdma).
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
#include "wcdma_variation.h"
#include "comdef.h"

/* Call back function type to get l1 sequencer cfn for every cfn update event*/
typedef void (*L1_SEQ_GET_CFN_CB_FUNC_TYPE) (uint8, wsub_id_e_type);

#define WL1_INFINITE_CFN_UPDATE 0xFFFF

typedef struct
{
  /** cfn notify callback function */
  L1_SEQ_GET_CFN_CB_FUNC_TYPE cfn_notify_cb_func;
  /** cfn for which the callback needs to be called, set it to
   *  continous update  */
  uint16 notify_cfn;
  /** Flag to avoid calls when HHO is ongoing*/
  boolean avoid_during_hho;
}l1_cfn_update_notify_struct_type;

/*===========================================================================
FUNCTION     L1_SEQ_REGISTER_GET_CFN_CB_FUNC

DESCRIPTION  This function registers the call back function which is used to notify the cfn

DEPENDENCIES
  The callback function will be called in the interrupt context. Hence the callback 
  function has to be crisp. Also, L1 may have to do some modifications if any new
  modules deicdes to use this module.

RETURN VALUE
  Boolean TRUE if registration succeed else FALSE

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_seq_register_get_cfn_cb_func(L1_SEQ_GET_CFN_CB_FUNC_TYPE cb_func, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1_SEQ_DEREGISTER_GET_CFN_CB_FUNC

DESCRIPTION  This function deregisters the call back function which is used to notify the cfn., 
                    last entry of the call back func table is copied to the deleted entry and Number of 

DEPENDENCIES
  None

RETURN VALUE
  Boolean TRUE if deregistration succeed else FALSE

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_seq_deregister_get_cfn_cb_func(L1_SEQ_GET_CFN_CB_FUNC_TYPE cb_func, wsub_id_e_type wsub_id);
/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                         Functions                                       */
/*                                                                         */
/*-------------------------------------------------------------------------*/


/*===========================================================================

FUNCTION      l1m_get_state

DESCRIPTION
  This function returns the value of l1m_state as uint8

DEPENDENCIES
  None

RETURN VALUE
   (uint8) l1m_state

SIDE EFFECTS

===========================================================================*/
extern
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR 
#endif
uint8 l1m_get_state( wsub_id_e_type wsub_id);


#ifdef FEATURE_RRC_DELAY_ERR_FATAL
/*===========================================================================

FUNCTION      L1M_PRINT_DEBUG_MSGS

DESCRIPTION
  Print debug information.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void l1m_print_debug_msgs(wsub_id_e_type wsub_id);
#endif /* FEATURE_RRC_DELAY_ERR_FATAL */

#endif /* L1EXTIF_V_H */


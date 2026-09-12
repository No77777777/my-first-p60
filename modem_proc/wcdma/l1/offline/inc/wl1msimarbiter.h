#ifndef WL1_MSIM_ARBITER_H
#define WL1_MSIM_ARBITER_H
/*===========================================================================
                           WCDMA MSIM Aribiter

GENERAL DESCRIPTION
  This file contains the code for arbitrating the functionality between two WCDMA Subscriptions

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2015-2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1msimarbiter.h#3 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who    what, where, why
--------   ---    ----------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
10/22/15   abs    Mods to support ACQ in DR-DSDS mode
08/19/15   abs    Changes to inform DRX about an upcoming W2W QTA for PICH
07/02/15   ms     Fix to use wcdma_convert_stack_index_to_subid for AS_ID
06/19/15   gn     Adding sub_id to fix sub_id related errors
05/20/15   sks    Compilation fix.
05/19/15   sks    Changes to support PICH and measurements in L/W/G + /G.
02/27/15   ap     Initial check-in for WL1 QSH framework 
03/23/15   rsr    W + W Phase 6 Checkin.
02/28/15   rsr    Checked in initial version of file.
===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "wcdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "l1def.h"
#include "l1const.h"

#if defined FEATURE_WCDMA_QSH_DUMP|| defined FEATURE_WCDMA_QSH_EVENT_METRIC
#include "wl1_qsh_ext.h"
#endif /*FEATURE_WCDMA_QSH_DUMP|| defined FEATURE_WCDMA_QSH_EVENT_METRIC*/

extern wsub_id_e_type wl1_conn_sub_id;
extern uint32 wl1_diag_sub_id[WCDMA_NUM_SUBS];

#ifdef FEATURE_DUAL_SIM
  #ifdef FEATURE_DUAL_WCDMA
  #define WL1_MSIM_VALIDATE_SUB_ID() ASSERT((wsub_id <= SYS_MODEM_AS_ID_2) && (wsub_id != SYS_MODEM_AS_ID_NONE))
  #else
  #define WL1_MSIM_VALIDATE_SUB_ID() ASSERT(wsub_id == WL1_DEFAULT_SUB_ID)
  #endif /* FEATURE_DUAL_WCDMA */
#else
  #define WL1_MSIM_VALIDATE_SUB_ID() ASSERT(wsub_id == WL1_DEFAULT_SUB_ID)
#endif /* FEATURE_DUAL_SIM */

#ifdef FEATURE_DUAL_SIM
  #ifdef FEATURE_DUAL_WCDMA
  #define WL1_MSIM_MAP_TO_WSUB_ID(as_id) \
    do { \
       wsub_id = as_id; \
       ASSERT((wsub_id <= SYS_MODEM_AS_ID_2) && (wsub_id != SYS_MODEM_AS_ID_NONE)); \
     }while(0)
  #else
     #define WL1_MSIM_MAP_TO_WSUB_ID(as_id) \
     do { \
       wsub_id = WL1_DEFAULT_SUB_ID;  \
     }while(0)
  #endif /* FEATURE_DUAL_WCDMA */
#else
  #define WL1_MSIM_MAP_TO_WSUB_ID(as_id) \
  do { \
     wsub_id = WL1_DEFAULT_SUB_ID;  \
  }while(0)
#endif /* FEATURE_DUAL_SIM */

/* Get AS ID to be passed to GL1 Macro, based on the current Wsub id*/
#define WL1_MSIM_GET_AS_ID_FOR_G() (wcdma_convert_stack_index_to_subid(wsub_id))

#define WL1_MSIM_VALIDATE_CONN_SUBID() (wsub_id == wl1_msim_get_conn_subid())

#define WL1_MSIM_VALIDATE_UL_CONN_SUBID() (WL1_MSIM_VALIDATE_CONN_SUBID())

typedef enum
{
  WL1_FW_APP_DISABLE,
  WL1_FW_APP_ENABLE,
  WL1_FW_APP_RESUME,
  WL1_FW_APP_SUSPEND
}wl1_fw_app_type;

/*===========================================================================
FUNCTION     WL1MSIMARBITER_REGISTER_TASK

DESCRIPTION
  This function registers wsub_id for every task with pthread TLS
  using the value passed from argument.
  
DEPENDENCIES
  None.

PARAMETERS
  subscription id of the task.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_msim_register_task(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1MSIMARBITER_GET_CURRENT_SUB_ID

DESCRIPTION
  This function gets the wsub_id based on the executing task.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern wsub_id_e_type wl1_msim_get_subid_from_tls(void);

/*===========================================================================
FUNCTION     WL1MSIMARBITER_SET_CON_MODE_SUB

DESCRIPTION
  This function sets the sub_id of W task which is in connected mode.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_msim_set_conn_subid(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1MSIMARBITER_GET_CON_MODE_SUB

DESCRIPTION
  This function gets the wsub_id of W task in connected mode.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR 
#endif
extern wsub_id_e_type wl1_msim_get_conn_subid(void);

#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR 
#endif
extern wsub_id_e_type wl1_mdump_get_conn_subid(void);


/*===============================================================================
FUNCTION     WL1_MSIM_ARBITER_FW_APP_CTRL

DESCRIPTION
  This function handles the FW APP transitions consolidating across both Subscriptions

DEPENDENCIES
  None.

PARAMETERS
  fw_state: Requested FW App Transition
  wsub_id: WCDMA Subscription ID

RETURN VALUE
  None

SIDE EFFECTS
  None.
===============================================================================*/
extern void wl1_msim_arbiter_fw_app_ctrl(const wl1_fw_app_type fw_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_MSIM_ARBITER_INIT

DESCRIPTION
  This function initializes arbiter critical section
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_msim_arbiter_init(void);

#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
FUNCTION     WL1_MSIM_FILL_QSH_INFO

DESCRIPTION
  This function populates MSIM related dump info

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void wl1_msim_fill_qsh_info(wl1_common_mini_debug_info_struct_type *common_dump_ptr);
#endif /* FEATURE_WCDMA_QSH_DUMP*/
/*===========================================================================
FUNCTION     WL1_MSIM_IDLE_OPTS_ALLOWED

DESCRIPTION
  This function checks if offline PICH/FCDC can be allowed for the given wsub_id
  
DEPENDENCIES
  None.

PARAMETERS
  wsub_id: WCDMA sub id.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_msim_idle_opts_allowed(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_MSIM_CHECK_FW_APP_STATUS

DESCRIPTION
  This function checks the FW App Status
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_msim_check_fw_app_status(wl1_fw_app_type fw_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_MSIM_W2W_QTA_FOR_PICH_IND

DESCRIPTION
  The function is invoked by W_Data subscription to indicate that it is about
  to perform a W2W QTA for W_Idle's PICH occasion

  Upon receiving this indication from W_Data arbitrator will invoke a DRX API
  of W_Idle if its next wakeup is programmed for offline PICH so that it can
  re-program to online PICH

DEPENDENCIES
  None

PARAMETERS
  wsub_id: WCDMA sub id

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_msim_w2w_qta_for_pich_ind(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_MSIM_CHECK_NBREJ_IN_PROGRESS_ALT_SUB

DESCRIPTION
  This function checks NBREJ status of the alternate subsciption
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean : nbrej status on the alt_sub

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_msim_check_nbrej_in_progress_alt_sub(wsub_id_e_type wsub_id);

#endif /* WL1_MSIM_ARBITER_H */


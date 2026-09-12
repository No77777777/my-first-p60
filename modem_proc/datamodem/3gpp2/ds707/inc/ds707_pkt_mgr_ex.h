#ifndef DSHDR_PKT_MGR_EX_H
#define DSHDR_PKT_MGR_EX_H

/*===========================================================================
Copyright (c) 2010-2018 by Qualcomm Technologies, Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/data.mpss/3.5..2.1/3gpp2/ds707/inc/ds707_pkt_mgr_ex.h#2 $
$Author: pwbldsvc $
$DateTime: 2019/10/22 02:53:17 $
                     
when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/18      psn     initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "dstask_v.h"
#include "sys.h"
#include "ds707_pdn_context.h"


/*===========================================================================
                            FORWARD DECLARATIONS
===========================================================================*/


/*===========================================================================
                          VARIABLES AND MACRO DEFINITIONS
==========================================================================*/
#ifdef FEATURE_EHRPD
/*===========================================================================
FUNCTION DS707_PKT_MGR_PROCESS_CM_SUB_NOT_AVAIL_CMD

DESCRIPTION
  This function processes the cm sub not avail cmd. It has additional
  functionality regarding OTASN PDN.
  
PARAMETERS  
  ds_cmd_ptr : cmd info ptr
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None
==========================================================================*/
void ds707_pkt_mgr_process_cm_sub_not_avail_cmd
(
  ds_cmd_type                *ds_cmd_ptr
);
/*===========================================================================
FUNCTION DS707_PKT_MGR_IS_ANY_OTASN_PDN_UP

DESCRIPTION
  This function determines if there is any OTASN PDN up on the particular sub
  
PARAMETERS  
  pdncntx_cb_ptr: if a valid pdn_cnt_pr is passed it, then return true if
                  any other pdn except the one passed in is up
  sub_id
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE : if any OTASN pdn is up
  FALSE : Otherwise
==========================================================================*/
boolean  ds707_pkt_mgr_is_any_otasn_pdn_up
(
  ds707_pdn_context_cb_type       *pdncntx_cb_ptr ,
  sys_modem_as_id_e_type           subs_id
);

/*===========================================================================
FUNCTION ds707_pkt_mgr_handle_otasn_pdn_down

DESCRIPTION
  This function handles cleanup for OTASN PDN when any other PDN goes down
  while the OTASN timewr is running

PARAMETERS 
  pdncntx_cb_ptr: ptr to the PDN that is going down.
  sub_id 

DEPENDENCIES
  None

RETURN VALUE  
  None

SIDE EFFECTS  
  None
===========================================================================*/
void ds707_pkt_mgr_handle_otasn_pdn_down
(
  ds707_pdn_context_cb_type       *pdncntx_cb_ptr ,
  sys_modem_as_id_e_type           subs_id
);
/*===========================================================================
FUNCTION DS707_PKT_MGR_FALLBACK_TO_LEGACY_SUBS_NOT_AVAIL_PROCEDURE

DESCRIPTION
  This function immediately processes the sub_not_avail procedure because
  of a priority req by CM for LPM mode.
  
PARAMETERS 
  None 
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
 None 
==========================================================================*/
void ds707_pkt_mgr_fallback_to_legacy_subs_not_avail_procedure
(
  sys_modem_as_id_e_type             subs_id
);
/*===========================================================================
FUNCTION DS707_PKT_MGR_IS_PDN_OF_PDN_LABEL_TYPE

DESCRIPTION
  This function checks if the passed PDN is of pdn_label type
  
PARAMETERS  
  pdn pointer
  incoming_pdn_label : the label to search for
  subs_id
      
DEPENDENCIES 
  None.
  
RETURN VALUE 
  TRUE - If PDN passed in of of type pdn_label
  FALSE - otherwise.
==========================================================================*/
boolean ds707_pkt_mgr_is_pdn_of_pdn_label_type
(
  ds707_pdn_context_cb_type       *pdncntx_cb_ptr,
  const char                      *incoming_pdn_label,  
  sys_modem_as_id_e_type           subs_id
);
/*===========================================================================
FUNCTION      DS707_OTASN_TIMER_EXPIRY_CB_HANDLER

DESCRIPTION   Processes the OTASN_TIMER_EXPIRY command

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds707_otasn_timer_expiry_cb_handler
(
  ds_cmd_type   *ds_cmd_ptr
);
#endif /* FEATURE_EHRPD */
#endif /*DSHDR_PKT_MGR_EX_H*/

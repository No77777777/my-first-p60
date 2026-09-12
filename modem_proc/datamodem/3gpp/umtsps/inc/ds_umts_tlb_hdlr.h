/*!
  @file
  ds_iumts_tlb_hdlr.h

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.

*/


#ifndef __DS_UMTS_TLB_HDLR_H__
#define __DS_UMTS_TLB_HDLR_H__

/*===========================================================================

  Copyright (c) 2009-2015 Qualcomm Technologies Incorporated. All Rights Reserved

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


/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/umtsps/inc/ds_umts_tlb_hdlr.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------

==============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"

#include "ds_3gpp_msg.h"
#include "sys.h"
#include "tc_cm.h"
#include "ds_3gpp_bearer_context.h"
#include "ds_3gpp_tlb_rm_hdlr.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif


typedef struct
{
  boolean                      test_mode_active;
  cm_test_control_type_e_type  tlb_mode;
  boolean                      tlb_mode_closed;
  uint16                       nsapi_mask;
  boolean                      wm_reg_complete;
}ds_umts_tlb_info_type_s;

typedef struct
{
  ds_umts_tlb_info_type_s      umts_tlb_info;
  ds_3gpp_rm_tlb_state_type_s  rmsm_state;
  sys_modem_as_id_e_type       subs_id;
  boolean                      in_use;
}ds_umts_tlb_per_subs_info_type;

typedef enum
{
  DS_UMTS_TLB_EV_MIN,
  DS_UMTS_TLB_UM_IFACE_EV = 1,
  DS_UMTS_TLB_EV_MAX
}ds_umts_tlb_ev_e_type;

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_umts_tlb_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_umts_tlb_qsh_mdump_collect
(
  void
);
#endif


ds_umts_tlb_per_subs_info_type *ds_umts_tlb_get_hndl
(
  sys_modem_as_id_e_type  subs_id,
  boolean                 free_hndl
);

void ds_umts_tlb_handle_oprt_mode_ev
(
  sys_oprt_mode_e_type        oprt_mode,
  sys_modem_as_id_e_type      subs_id,
  cm_test_control_type_e_type test_control_type
);

void ds_umts_tlb_handle_test_control_ev
(
  cm_test_control_type_e_type mode,
  sys_modem_as_id_e_type	  subs_id
);
void ds_umts_tlb_process_close_tlb
(
  sys_modem_as_id_e_type  subs_id
);

void ds_umts_tlb_hdlr_cmd
(
  const ds_cmd_type *cmd_ptr
);

boolean ds_umts_tlb_is_wm_reg_allowed
(
  sys_modem_as_id_e_type subs_id
);

ds_umts_tlb_per_subs_info_type *ds_umts_tlb_get_hndl_from_rm_iface
(
  ps_iface_type* rm_iface_ptr
);

boolean ds_umts_tlb_rm_rx_data_cb
(
  ps_sig_enum_type sig,
  void			   *user_data_p
);

extern int ds_umts_tlb_setup_rm_data_path
(
  ps_iface_type  *this_iface_ptr,
  ps_iface_type  *bridge_iface_ptr,
  sys_modem_as_id_e_type subs_id  
);

void ds_umts_tlb_wm_reg_cnf
(
  uint8                  nsapi,
  sys_modem_as_id_e_type subs_id
);

boolean ds_umts_tlb_is_loopback_closed
(
  ds_umts_tlb_per_subs_info_type *hndl
);

void ds_umts_tlb_init(void);




#endif

#ifndef PS_IFACE_HANDOFF_H
#define PS_IFACE_HANDOFF_H
/*===========================================================================
  @file ps_iface_handoff.h

  TODO

  Copyright (c) 2009-2011 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/inc/ps_iface_handoff.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-03-10 sun Added changes to support Pre Register Handoff
  2011-06-06 sid Added support for Partial PPP context
===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "ps_iface_defs.h"
#include "ps_acl.h"


/*===========================================================================

                              STRUCTURE DECLARATION

===========================================================================*/
typedef struct
{

  boolean retain_srat_flag; /*SRAT will not be torn down if flag is true*/
  
}ps_iface_handoff_swap_rat_params;


/*===========================================================================

                              EXTERNAL FUNCTIONS

===========================================================================*/
int32 ps_iface_handoff_initiate
(
  ps_iface_type         * ps_iface_ptr,
  acl_policy_info_type  * acl_policy_ptr,
  int16                 * ps_errno_ptr
);

int32 ps_iface_handoff_swap_rat
(
  ps_iface_type                     * ps_iface_ptr,
  ps_iface_handoff_swap_rat_params  * params,
  int16                             * ps_errno_ptr,
  void                              * client_data_ptr
);

int32 ps_iface_handoff_common_updates 
(
  ps_iface_type  * ps_iface_ptr,
  void           * client_data_ptr
);

int32 ps_iface_handoff_failure
(
  ps_iface_type  * ps_iface_ptr,
  int16          * ps_errno_ptr
);

#ifdef FEATURE_DATA_PS_IPV6
int32 ps_iface_handoff_transfer_sec_ipv6_addr
(
  ps_iface_type  * ps_iface_ptr,
  int16          * ps_errno_ptr
);
#endif /* FEATURE_DATA_PS_IPV6 */

/**
  @brief compares ip addressess. 
 
  This method compares ip addressess of given interfaces
 
  @param[in]  target_iface_ptr  Pointer to the target interface
  @param[in]  src_iface_ptr     Pointer to the source interface
 
  @return
    TRUE  on success
    FALSE on failure 
  
  @dependencies 
 	 None
*/
boolean ps_iface_compare_primary_ip_addr
(
  ps_iface_type  *target_iface_ptr,
  ps_iface_type  *src_iface_ptr
);

#endif /* PS_IFACE_HANDOFF_H */

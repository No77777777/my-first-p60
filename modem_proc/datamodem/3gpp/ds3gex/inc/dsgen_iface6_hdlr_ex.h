#ifndef DS_DSSNET6_EX_H
#define DS_DSSNET6_EX_H
/*===========================================================================

                      D S G E N _ I F A C E 6 _ H D L R_EX . H
                   
DESCRIPTION
  The Data Services Sockets Network state machine header file for IPv6.
  (This is an INTERNAL sockets file)

EXTERNALIZED FUNCTIONS
  dssnet6i_reg_handoff_hdlr()
    Register the handoff handler with EPC MH
  dssnet6i_reg_ip_config_hdlr()
    Register the ip config handler with EPC MH
  dssnet6i_set_prefix_for_third_party_ho()
    Check if it's 3rd party triggered handoff. 
    

 Copyright (c)2003-2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/ds3gex/inc/dsgen_iface6_hdlr_ex.h#1 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when         who    what, where, why
--------   ---    ----------------------------------------------------------
04/25/18   yujie   Created Module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#if defined (FEATURE_DATA_PS) && defined (FEATURE_DATA_PS_IPV6)
#include "dstask_v.h"
#include "dsgen_iface6_hdlr.h"
#include "ps_ip6_sm.h"

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
                            FORWARD DECLARATIONS
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
//struct __dssnet6_sm_cb_s; /* the state machine data structure              */

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

/* structure for DS_CMD_3GPP_APPLY_V6_PREFIX_IND */
typedef struct
{
  void                        *iface_ptr;
  boolean                      is_3gpp_iface;
}dssnet6i_apply_v6_prefix_cmd_type;


typedef boolean (*dssnet6i_reg_handoff_hdlr_f_type)
(
  ps_iface_type          * iface_ptr
);

typedef boolean (*dssnet6i_reg_ip_config_hdlr_f_type)
(
  ps_iface_type          * iface_ptr
);

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION DSSNET6I_REG_HANDOFF_HDLR

DESCRIPTION
  This function is used to register the handoff handler with EPC MH

PARAMETERS
  handoff_hdlr_f - Pointer to the handoff handler provided by EPC MH

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void dssnet6i_reg_handoff_hdlr
(
  dssnet6i_reg_handoff_hdlr_f_type handoff_hdlr_f
);

/*===========================================================================
FUNCTION DSSNET6I_REG_IP_CONFIG_HDLR

DESCRIPTION
  This function is used to register the ip config handler with EPC MH

PARAMETERS
  handoff_hdlr_f - Pointer to the ip config handler provided by EPC MH

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void dssnet6i_reg_ip_config_hdlr
(
  dssnet6i_reg_ip_config_hdlr_f_type ip_config_hdlr_f
);

/*===========================================================================
FUNCTION DSSNET6I_IS_THIRD_PARTY_HO

DESCRIPTION
  This function is used to check if it's 3rd party triggered handoff

PARAMETERS
  instance_ptr - DSSNET6 SM instance 

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean.

SIDE EFFECTS
  None.

===========================================================================*/
boolean dssnet6i_is_third_party_ho
(
  dssnet6_sm_cb_type     *instance_ptr          /* DSSNET6 SM instance    */
);

/*===========================================================================
FUNCTION DSSNET6I_SET_PREFIX_FOR_THIRD_PARTY_HO

DESCRIPTION
  Copy IPv6 prefix from EPC MH and send valid_ra_ind

PARAMETERS
  instance_ptr - DSSNET6 SM instance 

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void dssnet6i_set_prefix_for_third_party_ho
(
  dssnet6_sm_cb_type     *instance_ptr          /* DSSNET6 SM instance    */
);

/*===========================================================================
FUNCTION DSSNET6I_APPLY_V6_PREFIX_FROM_IFACE()

DESCRIPTION
 Applies v6 prefix from 3gpp / 3gpp2 iface and generates valid RA ind.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dssnet6i_apply_v6_prefix_from_iface
(
  dssnet6_sm_cb_type	 *instance_ptr
);

/*===========================================================================
FUNCTION ds_3gpp_net6i_apply_v6_prefix_from_iface_cmd()

DESCRIPTION
 Applies v6 prefix from 3gpp / 3gpp2 iface and generates valid RA ind.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_net6i_apply_v6_prefix_from_iface_cmd
(
  void  *cmd_payload_ptr
);

#endif /* FEATURE_DATA_PS && FEATURE_DATA_PS_IPV6 */
#endif /* DS_DSSNET6_EX_H */

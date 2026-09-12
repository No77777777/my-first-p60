#ifndef IPSEC_IFACE_RT_ACL_H
#define IPSEC_IFACE_RT_ACL_H

/*===========================================================================
                    I P S E C _ I F A C E _ R T _ A C L . H

DESCRIPTION
  Routing ACL(s) for IPsec IFACE.

Copyright (c) 2009 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/ifacectls/src/ipsec_iface_rt_acl.h#2 $
  $Author: pwbldsvc $
  $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/26/09    sp     Created module - Moved IPsec Mode Handler to Data
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#ifdef FEATURE_SEC_IPSEC

#include "ps_acl.h"
#include "ps_aclrules.h"

/*===========================================================================

                         ACCESS CONTROL LIST DEFINITION

===========================================================================*/

ACL_DEF( ipsec_rt_acl );
ACL_DEF( ipsec_inbound_acl );

#endif /* FEATURE_SEC_IPSEC */

#endif /* IPSEC_IFACE_RT_ACL_H */

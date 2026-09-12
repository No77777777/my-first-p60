#ifndef __PS_CLAT_RT_ACL_H_
#define __PS_CLAT_RT_ACL_H_
/*===========================================================================

                    P S _ C L A T _ R T _ A C L . H 
                   
DESCRIPTION
  The header file for CLAT ACLs

EXTERNALIZED FUNCTIONS

Copyright (c)2014 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/inet/src/ps_clat_rt_acl.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $
--------    ---    ----------------------------------------------------------
10/10/14    ssh    IP Translation Engine 464XLAT  
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#if defined (FEATURE_DATA_PS) && defined (FEATURE_DATA_PS_IPV6)
#ifdef FEATURE_DATA_PS_464XLAT
#include "ps_acl.h"
#include "ps_aclrules.h"

/*===========================================================================

                 ACCESS CONTROL LIST NAME DEFINITIONS

===========================================================================*/

ACL_DEF( ps_clat_rt_acl );

ACL_DEF( ps_clat_in_rt_acl );

ACL_POST_PROC_DEF( ps_clat_rt_acl_post_proc );

ACL_CFG_SUPPORTED_DEF2( ps_clat_rt_is_cfg_supported );
#endif /* FEATURE_DATA_PS_464XLAT */
#endif /* FEATURE_DATA_PS && FEATURE_DATA_PS_IPV6*/
#endif /* __PS_CLAT_RT_ACL_H_ */

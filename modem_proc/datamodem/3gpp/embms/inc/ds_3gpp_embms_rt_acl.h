#ifndef DS_3GPP_EMBMS_RT_ACL_H
#define DS_3GPP_EMBMS_RT_ACL_H

/*===========================================================================

   eMBMS SPECIFIC RT ACL FILE.
GENERAL DESCRIPTION
  The eMBMS specific routing ACL is used to enable policy and address
  based routing across the eMBMS interface.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/embms/inc/ds_3gpp_embms_rt_acl.h#2 $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/26/11    hs     Created file/Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#ifdef FEATURE_DATA_EMBMS

#include "ps_acl.h"
#include "ps_aclrules.h"

/*===========================================================================

                 ACCESS CONTROL LIST NAME DEFINITIONS

===========================================================================*/

ACL_DEF( ds_3gpp_embms_rt_acl );

#endif /* FEATURE_DATA_EMBMS */
#endif /* DS_3GPP_EMBMS_RT_ACL_H */


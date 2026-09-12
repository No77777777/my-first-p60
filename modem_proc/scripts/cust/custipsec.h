#ifndef CUSTIPSEC_H
#define CUSTIPSEC_H
/*===========================================================================

              C U S T O M I Z A T I O N   O F   I P S E C

DESCRIPTION
  Configuration for IPSec and IKE.

  Copyright (c) 2003  by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/scripts.mpss/2.1.3/cust/custipsec.h#1 $ $DateTime: 2019/04/24 01:57:11 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/04/03   om      Created file.

===========================================================================*/


/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
                          Security Services
---------------------------------------------------------------------------*/
#define FEATURE_SEC_SSL
#define FEATURE_SEC_IPSEC  //#define FEATURE_SEC_IPSEC  defined in tbc_modem.builds too, remember to fix it
/* Security Services
*/
#ifdef FEATURE_SEC_IPSEC

  /* VPN remote access support */
  #define FEATURE_SEC_IPSEC_VPN

  /* VPN test client code (via UI test menu) */
  #ifdef FEATURE_SEC_TESTAPPS
    #define FEATURE_SEC_IPSEC_VPN_CLNT
  #else
    #undef FEATURE_SEC_IPSEC_VPN_CLNT
  #endif

#endif  /* FEATURE_SEC_IPSEC */

#endif  /* CUSTIPSEC_H */

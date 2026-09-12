#ifndef IPSEC_IFACE_HDLR_H
#define IPSEC_IFACE_HDLR_H

/*===========================================================================
                    I P S E C _ I F A C E _ H D L R . H

DESCRIPTION
  IPsec IFACE handler functions.

Copyright (c) 2009,2015 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/ifacectls/inc/ipsec_iface_hdlr.h#2 $
  $Author: pwbldsvc $
  $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/26/09    sp     Created module - Moved IPsec Mode Handler to Data
===========================================================================*/

#include "datamodem_variation.h"
#ifdef FEATURE_SEC_IPSEC

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================
FUNCTION  IPSEC_IFACE_HDLR_IFACE_INIT

DESCRIPTION
  This function creates and initialises the IPsec Ifaces.
  This is called from the cdc_task() function.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ipsec_iface_hdlr_iface_init
(
  void
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

#endif /* FEATURE_SEC_IPSEC */

#endif /* IPSEC_IFACE_HDLR_H */

#ifndef PS_IPFLTRI_H
#define PS_IPFLTRI_H

/*===========================================================================

                          P S _ I P F L T R I . H

GENERAL DESCRIPTION
  This header file provides utility macros related to IPFLTR.

EXTERNALIZED FUNCTIONS
  None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/inc/ps_ipfltri.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/28/14   sj      Initial version

===========================================================================*/


/*---------------------------------------------------------------------------
  PS_IPFLTR_TYPE_IS_ESP macro definition
---------------------------------------------------------------------------*/
#define PS_IPFLTR_TYPE_IS_ESP(ipfltr)                                        \
      (((ipfltr.ip_vsn == IP_V4) &&                                       \
       (ipfltr.ip_hdr.v4.field_mask & IPFLTR_MASK_IP4_NEXT_HDR_PROT) &&   \
       (ipfltr.ip_hdr.v4.next_hdr_prot == PS_IPPROTO_ESP)) ||             \
      ((ipfltr.ip_vsn == IP_V6) &&                                        \
       (ipfltr.ip_hdr.v6.field_mask & IPFLTR_MASK_IP6_NEXT_HDR_PROT) &&   \
       (ipfltr.ip_hdr.v6.next_hdr_prot == PS_IPPROTO_ESP)))

#endif /* PS_IPFLTRI_H */

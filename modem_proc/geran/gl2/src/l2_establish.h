#ifndef L2_ESTABLISH_H
#define L2_ESTABLISH_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  L 2   E S T A B L I S H   H E A D E R   F I L E

DESCRIPTION
   Contains functions and variables related to establishing a link 

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/GSM/L2/vcs/l2_establish.h_v   1.2   07 Mar 2002 14:16:38   jachan  $
$Header: //components/rel/geran.mpss/7.4.0/gl2/src/l2_establish.h#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   -------- ---------------------------------------------
10/19/01   JAC       Initial version

===========================================================================*/

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

void       l2_establish_link(l2_store_T *, SAPI_T, l2_channel_type_T);
l2_event_T l2_handle_sabm_reception(l2_store_T *, SAPI_T, l2_channel_type_T, boolean);
l2_event_T l2_handle_establish_dm_reception(l2_store_T *, SAPI_T, l2_channel_type_T);
l2_event_T l2_retransmit_sabm(l2_store_T *, SAPI_T, l2_channel_type_T);
l2_event_T l2_handle_establish_ua_reception(l2_store_T *, SAPI_T, l2_channel_type_T);

#endif


#ifndef _WLAN_MPD_CIPHER_H
#define _WLAN_MPD_CIPHER_H

/**
@file wlan_mpd_cipher.h
@brief Relay Message Buffer
*/

/*===========================================================================
   Copyright (c) 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/core.mpss/10.0/power/ccpm_r/wlan_md/inc/wlan_md_cipher.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ------------------------------------
01/11/12   mm       Made Compatible to Virtio 1.1.1 flat file
12/20/11   mm       Initial version.

===========================================================================*/

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "comdef.h"

/*----------------------------------------------------------------------------
 * Function Declarations
 * -------------------------------------------------------------------------*/
void wlan_md_crypto_init(void);
void wlan_md_encrypt_region(uint64 r_base, uint64 r_len, uint32 r_num);

#endif /* _WLAN_MPD_CIPHER_H */

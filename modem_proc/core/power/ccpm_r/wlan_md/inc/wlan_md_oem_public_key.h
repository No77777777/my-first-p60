#ifndef _WLAN_MPD_OEM_PUBLIC_KEY_H
#define _WLAN_MPD_OEM_PUBLIC_KEY_H

/*===========================================================================

                                OEM public key for minidump encryption
                                

GENERAL DESCRIPTION
  This header file contains OEM public key for minidump encryption

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None
  
Copyright 2017 by Qualcomm Technologies, Inc.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/power/ccpm_r/wlan_md/inc/wlan_md_oem_public_key.h#1 $
  $DateTime: 2019/04/24 00:03:26 $ 
  $Author: pwbldsvc $

when       who          what, where, why
--------   --------     ----------------------------------------------------------
26/04/17   bd           Initial version
============================================================================*/

/*===========================================================================*/


typedef struct
{
  uint32      bitLength;   
  char  *n;          
  const char  *p;
  const char  *q;
  char  *e;
  const char  *d;
}wlan_md_raw_key_t;


wlan_md_raw_key_t wlan_md_oem_public_key = {
    2048,
    /* n */
    "00E8574F48246F1EE106C14B55F6EA30FC0934640F4C02C7DB4F38DCB6E41B5430C98C73071DBA0"
	"822C824887CFA9F2DF498CBF01640444A2CDA151F23DB0D322FA316A9A4DCC432326740849467"
	"8A0BEF1EEA576AC27301E08C1EFF95D42B05E53BFB2AC5A9BDD5C93386250E310D78DCF5D2748"
	"0025B56DB36840C5FC80C1426131003A07355694BB516742657A2482ACFDDE5808DFC3F62E3C2"
	"C12CE7F6250A5C747772AD65F3212D5D895B3E1A17CB85DC373FC91537F6AD2FB94CDF4CB20F7"
	"D532B91201618AF2F8E9A012110A7A6DF88DC3A24C5E6FD58DEDC9610511E9AC82B8FC722E2C0"
	"005AE3C039A55082BF949B11680509470122873DC6A88BACD1",

    /* p */
    "",

    /* q */
    "",

    /* e */
    "000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000010001",

    
    /* d */
    "",
};

#endif  /* _WLAN_MPD_OEM_PUBLIC_KEY_H */

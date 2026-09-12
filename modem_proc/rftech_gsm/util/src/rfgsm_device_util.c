/*!
   @file rfgsm_core_util.c

   @brief

   @details
/*===========================================================================
Copyright (c) 2008 - 2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/util/src/rfgsm_device_util.c#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/17/15   avi     Initial version
============================================================================*/

#include "rfcom.h"

/*----------------------------------------------------------------------------*/
/*!
   @brief

   @details

   @param  

   @retval 

*/

uint32 rfgsm_core_convert_arfcn_to_tx_freq(rfcom_gsm_band_type band, uint16 arfcn )
{
  uint32 rf_freq=0;

  switch (band) 
  {
  case RFCOM_BAND_GSM850:
    if (arfcn >= 128 && arfcn <= 251)
    {
      rf_freq = 824200000 + 200000*(arfcn - 128);      
    }
    else
    {
      MSG_FATAL("Invalid channel for GSM850(128 - 251) band: %d", arfcn, 0, 0);
    }
    break;

  case RFCOM_BAND_GSM900:
    if (arfcn <= 124)
    {
      rf_freq = 890000000 + 200000*arfcn;
    }
    else if (arfcn >= 975 && arfcn <= 1023)
    {
      rf_freq = 890000000 + 200000*(arfcn - 1024);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,
            "Invalid channel for GSM900(0-124 975-1023) band: %d", arfcn); 
    }
    break;

  case RFCOM_BAND_GSM1800:
  case RFCOM_BAND_GSM1800_B:
    if (arfcn >= 512 && arfcn <= 885)
    {
      rf_freq = 1710200000 + 200000*(arfcn - 512);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,
            "Invalid channel for GSM1800(512-885) band: %d", arfcn);
    }
    break;

  case RFCOM_BAND_GSM1900:
    if (arfcn >= 512 && arfcn <= 810)
    {
      rf_freq = 1850200000 + 200000*(arfcn - 512);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid channel for GSM1800(512-810) band: %d", arfcn);
    }
    break;

  default:
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Unsupported RF band: %d", band);
  }

  return rf_freq;

} /* end of rfgsm_core_convert_arfcn_to_tx_freq */

/*----------------------------------------------------------------------------*/
/*!
   @brief

   @details

   @param  

   @retval 

*/
uint32 rfgsm_core_convert_arfcn_to_rx_freq(rfcom_gsm_band_type band, uint16 chan )
{
  uint32 rf_freq=0;

  /* get the rx freq fro the channel */
  rf_freq = rfgsm_core_convert_arfcn_to_tx_freq(band, chan);
  
  /* if the rf_freq is zero then the arfcn is invalid and a error message has
     been already printed so, just return the 0 value. */
  if (rf_freq == 0)
  {
    return rf_freq;
  }

  /* add the Uplink and downlink separation freq offset */
  switch (band)
  {
  case RFCOM_BAND_GSM850:
  case RFCOM_BAND_GSM900:
    rf_freq += 45000000;;
    break;

  case RFCOM_BAND_GSM1800:
  case RFCOM_BAND_GSM1800_B:
    rf_freq += 95000000;
    break;

  case RFCOM_BAND_GSM1900:
    rf_freq += 80000000;
    break;

  default:
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Unsupported RF band: %d", band);
  }

  return rf_freq;

} /* end of rfgsm_core_convert_arfcn_to_rx_freq */





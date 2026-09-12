/*!
 @file rf_tdscdma_core_util.c

 @brief
  This file has a collection of utility functions for TDSCDMA technology.

 @details

*/

/*===========================================================================

Copyright (c) 2011 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_tdscdma.mpss/3.11/util/src/rf_tdscdma_device_util.cpp#1 $

When       who     what, where, why
-------------------------------------------------------------------------------
08/18/15   avi     Moved #define/Constants to rf_tdscdma_core_util.h
07/16/15   avi     initial version.

============================================================================*/

#include "rf_tdscdma_core_util.h"


/*----------------------------------------------------------------------------*/
/*!

 @brief
  This function returns the DL/UL frequency in Khz based on the
  downlink/uplink channel number passed in as an argument for TDSCDMA.

 @details
  The downlink/uplink channel numbers do not overlap; hence the band
  information is not needed but it is already passed by L1(??).
  Please note that the channel raster is 200kHz, which means the
  carrier frequency must be a multiple of 200kHz

  Desired LO frequency in Khz unit for Downlink in uint32.
  (eg: for 2110Mhz, it returns 2110000).

 @param
  chan: Downlink/Uplink channel number.
  band: Band that the chan in argument 1 belongs to.

*/
uint32 rf_tdscdma_core_util_get_freq_from_uarfcn(uint16 chan, rfcom_tdscdma_band_type band)
{
  uint32 freq = 0;

  switch (band)
  {
    case RFCOM_BAND_TDSCDMA_B34:
     case RFCOM_BAND_TDSCDMA_B34_B:
	  if( (chan >= RF_TDSCDMA_B34_MIN_CHAN) && (chan <= RF_TDSCDMA_B34_MAX_CHAN) )
	  {
		freq = chan * RF_TDSCDMA_CHAN_RASTER_IN_KHZ;
	  }
      else
      {
        MSG_ERROR("Invalid TDSCDMA chan %d for Band %d", chan, band, 0);
      }
	  break;

    case RFCOM_BAND_TDSCDMA_B39:
     case RFCOM_BAND_TDSCDMA_B39_B:
	  if( (chan >= RF_TDSCDMA_B39_MIN_CHAN) && (chan <= RF_TDSCDMA_B39_MAX_CHAN) )
	  {
		freq = chan * RF_TDSCDMA_CHAN_RASTER_IN_KHZ;
	  }
      else
      {
        MSG_ERROR("Invalid TDSCDMA chan %d for Band %d", chan, band, 0);
      }
	  break;

    case RFCOM_BAND_TDSCDMA_B40:
	case RFCOM_BAND_TDSCDMA_B40_B:
	  if( (chan >= RF_TDSCDMA_B40_MIN_CHAN) && (chan <= RF_TDSCDMA_B40_MAX_CHAN) )
	  {
		freq = chan * RF_TDSCDMA_CHAN_RASTER_IN_KHZ;
	  }
      else
      {
        MSG_ERROR("Invalid TDSCDMA chan %d for Band %d", chan, band, 0);
      }
	  break;

    default :
      MSG_ERROR("Invalid RF Band for TDSCDMA = %d", band, 0, 0);
      break;
  }

  return (freq);

} /*rf_tdscdma_core_get_freq_from_uarfcn*/





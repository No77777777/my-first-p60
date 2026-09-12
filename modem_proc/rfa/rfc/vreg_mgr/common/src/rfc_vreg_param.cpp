/*!
   @file rfc_vreg_param.cpp

   @brief


   @details
   This file implements the rfc_vreg_mgr abstract class.

*/
/*===========================================================================
Copyright (c) 2012 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:11:02 $ $Author: pwbldsvc $
$Header: //components/rel/rfa.mpss/4.11/rfc/vreg_mgr/common/src/rfc_vreg_param.cpp#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
05/12/15   rcui   change  voting to be device based
04/02/12   sr     Initial revision.
05/03/12   sb     Fixed KW warning
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
extern "C" {
#include "rfcom.h"
#include "msg.h"
}
#include "rfc_vreg_param.h"

/* ----------------------------------------------------------------------- */
/*!
   @brief
   constructor.

   @details

*/
rfc_vreg_param::rfc_vreg_param(void)
: mode(RFM_INVALID_MODE), band(0), rf_device(RFM_DEVICE_0), path(RF_PATH_MAX), rf_state(RF_PATH_MAX_STATE)
{

}

/* ----------------------------------------------------------------------- */
/*!
   @brief
   constructor.

   @details

*/
rfc_vreg_param::rfc_vreg_param(rfcom_mode_enum_type  mode)
{
  this->band = 0;
  this->rf_device = RFM_DEVICE_0;
  this->path = RF_PATH_MAX;
  this->rf_state = RF_PATH_MAX_STATE;
  set_mode(mode);
}


/* ----------------------------------------------------------------------- */
/*!
   @brief
   constructor.

   @details

*/
rfc_vreg_param::rfc_vreg_param(rf_path_enum_type  path, rfcom_mode_enum_type  mode)
{
  this->band = 0;
  this->rf_device = RFM_DEVICE_0;
  this->path = RF_PATH_MAX;
  this->rf_state = RF_PATH_MAX_STATE;
  set_path(path);
  set_mode(mode);
}


/* ----------------------------------------------------------------------- */
/*!
   @brief
   Destructor.

   @details

*/
rfc_vreg_param::~rfc_vreg_param()
{
  // Do nothing
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
boolean rfc_vreg_param::set_path(rf_path_enum_type  path)
{
  if (path < RF_PATH_0 || path >= RF_PATH_MAX )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"invalid path (path=%d)",path);
    return FALSE;
  }

  this->path = path;
  return TRUE;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
boolean rfc_vreg_param::set_mode(rfcom_mode_enum_type  mode)
{
  if (mode < RFM_PARKED_MODE || mode >= RFM_INVALID_MODE )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"invalid mode (mode=%d)",mode);
    return FALSE;
  }

  this->mode = mode;
  return TRUE;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
boolean rfc_vreg_param::set_band(uint16  band)
{
  this->band = band;
  return TRUE;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
boolean rfc_vreg_param::set_device(rfm_device_enum_type  rf_device)
{
  if (rf_device < RFM_DEVICE_0 || rf_device > RFM_MAX_DEVICES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"invalid rf_device (rf_device=%d)",rf_device);
    return FALSE;
  }
  this->rf_device = rf_device;
  return TRUE;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
boolean rfc_vreg_param::set_rf_state(rfm_path_state  rf_state)
{
  if (rf_state < RF_PATH_SLEEP_STATE || rf_state >= RF_PATH_MAX_STATE)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"invalid rf state (rf_state=%d)",rf_state);
    return FALSE;
  }
  this->rf_state = rf_state;
  return TRUE;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
rf_path_enum_type rfc_vreg_param::get_path(void)
{
  if (path == RF_PATH_MAX )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"trying to use invalid path (path=%d)",path);
  }

  return path;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
rfcom_mode_enum_type rfc_vreg_param::get_mode(void)
{
  if (mode == RFM_INVALID_MODE )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"trying to use invalid mode (mode=%d)",mode);
  }

  return mode;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
uint16 rfc_vreg_param::get_band(void)
{
  return band;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
rfm_device_enum_type rfc_vreg_param::get_device(void)
{
  if (rf_device < RFM_DEVICE_0 || rf_device > RFM_MAX_DEVICES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"trying to use invalid rf_device (rf_device=%d)",rf_device);
  }
  return rf_device;
}

/* ----------------------------------------------------------------------- */
/*!
   @brief

   @details

*/
rfm_path_state rfc_vreg_param::get_rf_state(void)
{
  if (rf_state == RF_PATH_MAX_STATE)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"trying to use invalid rf state (rf_state=%d)",rf_state);
  }

  return rf_state;
}





/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                           
                       F T M  G S M  C T L

GENERAL DESCRIPTION
  This is the FTM RF file which contains RF specific commands

Copyright (c) 2000, 2002  by Qualcomm Technologies, Incorporated.  All Rights Reserved.
Copyright (c) 2007 - 2015  by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: 
  $DateTime: 2021/03/09 05:10:17 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/20   mpa     To support MiniDump Feature
11/27/15   hkm     TXRXSplit support changes
08/31/15   zc      Fix KW Errors
08/14/15   zc      Rx Meas/Tx Control rftest Interface update
08/12/15   zc      Fix KW Errors
07/27/15   zc      Initial Revision
===========================================================================*/


/*===========================================================================
                              INCLUDES
===========================================================================*/

#include "rfa_variation.h"
#include "comdef.h"
#include "target.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftmicap.h"
#include "rex.h"

#ifdef FTM_HAS_UMTS

#ifdef FEATURE_GSM

#include "ftm.h"
#include "ftm_gsm_ctl.h"
#include "ftm_gsm_rfctl.h"
#include "ftm_gsm.h"
#include "ftm_gsm_device.h"
#include "ftm_rf_test_radio_config.h"
#include "ftm_rf_test_interface.h"
#include "ftm_gsm_rftest.h"
#include "rfgsm_core_util.h"
#include "rfgsm_mc.h"
#include "rfgsm_nv_cmn.h"
#include "rfgsm_mdsp_cal.h"
#include "rfcommon_math.h"
#include "ftm_msg.h"
#include "ftm_gsm_rfprot.h"
#include "rfcommon_msg.h"
#include "rfc_common.h"

/* For reference to SYS_BAND macros. */
#include "sys_type.h"


#ifdef FEATURE_GSM_RFLM
#include "rxlm_intf.h"
#include "txlm_intf.h"
#endif

#include "rfcommon_nv_mm.h"
#include "ftm_common_concurrency_manager.h"
#include "ftm_device_manager.h"


#define DEBUG_BUF_SIZE 10
#define INVALID_ARFCN 0xFFFF

static ftm_gsm_rftest_global_t ftm_gsm_rftest_globals = 
{
  /*ftm_gsm_rftest_radio_config_t ftm_gsm_rftest_radio_data[2]*/
  {
    {RFM_INVALID_DEVICE, RFM_INVALID_DEVICE, RFM_INVALID_DEVICE, FTM_PHONE_MODE_MAX, INVALID_ARFCN, NULL, NULL},
    {RFM_INVALID_DEVICE, RFM_INVALID_DEVICE, RFM_INVALID_DEVICE, FTM_PHONE_MODE_MAX, INVALID_ARFCN, NULL, NULL}
  },

  /*ftm_gsm_rftest_radio_control_t ftm_gsm_rftest_radio_control[2]*/
  {
    {
      {0, FTM_TX_DATA_SOURCE_PSDRND, 0, 0, TRUE},
      {0},
      {0, FTM_RX_FUNCTION_RF_RX_HW_ON},
      {0, 0}
    },
    {
      {0, FTM_TX_DATA_SOURCE_PSDRND, 0, 0, TRUE},
      {0},
      {0, FTM_RX_FUNCTION_RF_RX_HW_ON},
      {0, 0}
    }
  },

  /*uint8 ftm_gsm_rftest_chain_to_sub_map[FTM_GSM_INVALID_CHAIN]*/
  {
    UNDEFINED_SUB_ID,
    UNDEFINED_SUB_ID,
    UNDEFINED_SUB_ID,
    UNDEFINED_SUB_ID
  },

  /*boolean ftm_gsm_rftest_enabled*/
  FALSE,

  /*rex_crit_sect_type ftm_gsm_rftest_device_lock*/
  {0}
};




/*===========================================================================
INTERNAL FUNCTIONS
===========================================================================*/

/*===========================================================================

FUNCTION ftm_gsm_rftest_set_rx_burst_params

DESCRIPTION
   Sets RF test Rx Burst Params
   
DEPENDENCIES
   None.

RETURN VALUE
   
===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_set_rx_burst_params
(
  ftm_gsm_rftest_radio_control_t * rftest_ctl_data,
  uint8 slot_num,
  ftm_enum_rx_function_type ftm_rx_funct,
  int16 param1,
  int16 param2
)
{
  uint32 status = 0;
  if(rftest_ctl_data == NULL)
  {
    RF_MSG_2(FTM_ERROR, "Null rftest_ctl_data, returning", 0, 0);
    status |= 2;
  }
  else
  {
    rftest_ctl_data->rx_burst.slot_num = slot_num;
    rftest_ctl_data->rx_burst.ftm_rx_funct = ftm_rx_funct;
    rftest_ctl_data->rx_burst_params[0] = param1;
    rftest_ctl_data->rx_burst_params[1] = param2;
  }

  return status;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_set_rx_burst_params

DESCRIPTION
   Sets RF test Rx Burst Params
   
DEPENDENCIES
   None.

RETURN VALUE
   
===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_set_tx_burst_params
(
  ftm_gsm_rftest_radio_control_t * rftest_ctl_data,
  uint8 slot_num,
  boolean tx_on,
  int16 tx_power,
  int16 mcs_type
)
{
  uint32 status = 0;
  slot_num = slot_num & 0x7;
  if(rftest_ctl_data == NULL)
  {
    RF_MSG_2(FTM_ERROR, "Null rftest_ctl_data, returning", 0, 0);
    status |= 2;
  }
  else
  {
    rftest_ctl_data->tx_burst.slot_num = slot_num;
    rftest_ctl_data->frame_matrix[slot_num].slot_num = slot_num;
    rftest_ctl_data->frame_matrix[slot_num].on_off = tx_on;
    rftest_ctl_data->frame_matrix[slot_num].power_dB = tx_power;
    rftest_ctl_data->frame_matrix[slot_num].mcs = mcs_type;
  }
  return status;
}



/*===========================================================================

FUNCTION ftm_gsm_rftest_get_ftm_mode_from_sys_band

DESCRIPTION
   Gets ftm mode from sys band
   
DEPENDENCIES
   None.

RETURN VALUE
   
===========================================================================*/

ftm_mode_id_type ftm_gsm_rftest_get_ftm_mode_from_sys_band(sys_band_class_e_type band)
{
  ftm_mode_id_type ftm_mode = FTM_PHONE_MODE_GSM_900;

  switch(band)
  {
    case SYS_BAND_GSM_850:
      ftm_mode = FTM_PHONE_MODE_GSM_850;
      break;

    case SYS_BAND_GSM_EGSM_900:
      ftm_mode = FTM_PHONE_MODE_GSM_900;
      break;

    case SYS_BAND_GSM_DCS_1800:
      ftm_mode = FTM_PHONE_MODE_GSM_1800;
      break;

    case SYS_BAND_GSM_PCS_1900:
      ftm_mode = FTM_PHONE_MODE_GSM_1900;
      break;

    default:
      RF_MSG_1(FTM_ERROR, "Invalid GSM band %d", band);
      ftm_mode = FTM_PHONE_MODE_MAX;
  }

  return ftm_mode;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_radio_config_dev_add

DESCRIPTION
   Adds a new device to be configured (band, chan, tx/rx)
   
DEPENDENCIES
   None.

RETURN VALUE
   Error code if device not added correctly


===========================================================================*/

ftm_mode_id_type ftm_gsm_rftest_convert_arfcn_to_band(uint16 arfcn)
{
  ftm_mode_id_type ftm_gsm_mode;
  rfcom_gsm_band_type rfcom_gsm_band = rfgsm_core_convert_arfcn_to_band( arfcn );
  ftm_gsm_mode = ftm_gsm_rftest_get_ftm_mode_from_sys_band((sys_band_class_e_type)rfgsm_core_rfcom_to_rfband(rfcom_gsm_band));
  return ftm_gsm_mode;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_check_enabled_devices

DESCRIPTION
   Checks to see if any devices are enabled, after a drop device is called. 
   If none are enabled, will disable rf_test global hook in ftm_gsm_device.c
   
DEPENDENCIES
   None.

RETURN VALUE
...None


===========================================================================*/

void ftm_gsm_rftest_check_any_enabled_devices(void)
{
  ftm_gsm_rftest_radio_config_t * rftest_data = NULL;
  uint8 sub_id;
  boolean ftm_gsm_rftest_enabled = FALSE;
  for(sub_id = 0; sub_id < UNDEFINED_SUB_ID; sub_id++)
  {
    rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
    ftm_gsm_rftest_enabled = (ftm_gsm_rftest_enabled ) ||
      !((rftest_data->rx_device == RFM_INVALID_DEVICE) &&
       (rftest_data->tx_device == RFM_INVALID_DEVICE) &&
       (rftest_data->drx_device == RFM_INVALID_DEVICE) );
  }

  if(!ftm_gsm_rftest_enabled)
  {
    RF_MSG_1(FTM_HIGH, "No enabled devices on either sub, disabling ftm_rftest_gsm", 0);
  }

  ftm_gsm_rftest_set_rftest_enabled(ftm_gsm_rftest_enabled);
  
}

/*===========================================================================

FUNCTION ftm_gsm_rftest_clear_prx_chain_vars

DESCRIPTION
   Clears the rfctl variables and the rftest global storage variables
   
DEPENDENCIES
   None.

RETURN VALUE
...None


===========================================================================*/

void ftm_gsm_rftest_clear_prx_chain_vars
(
  rfm_device_enum_type rfm_dev,
  ftm_mode_id_type gsm_mode,
  uint8 sub_id
)
{
  ftm_gsm_rftest_radio_config_t * rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
  
  if(rftest_data == NULL)
  {
    RF_MSG_2(FTM_ERROR, "Null rftest_data, returning", 0, 0);
    return;
  }
  
  ftm_gsm_device_clear_chain_vars(rfm_dev);

  if(rftest_data->rx_device == rfm_dev)
  {
    RF_MSG_1(FTM_HIGH, "Dropping prx dev %d", rfm_dev);
    rftest_data->rx_device = RFM_INVALID_DEVICE;
    rftest_data->arfcn = INVALID_ARFCN;
    rftest_data->rx_device = RFM_INVALID_DEVICE;
    rftest_data->gsm_mode = gsm_mode;
    
    ftm_gsm_rftest_set_sub_from_device( rfm_dev, UNDEFINED_SUB_ID );
  }
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_clear_associated_chain_vars

DESCRIPTION
   Clears the rfctl variables and the rftest global storage variables
   
DEPENDENCIES
   None.

RETURN VALUE
...None


===========================================================================*/

void ftm_gsm_rftest_clear_associated_chain_vars
(
  rfm_device_enum_type rfm_dev, 
  uint8 sub_id
)
{
  ftm_gsm_rftest_radio_config_t * rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
  
  if(rftest_data == NULL)
  {
    RF_MSG_2(FTM_ERROR, "Null rftest_data, returning", 0, 0);
    return;
  }

  if(rftest_data->rx_device <= RFM_MAX_DEVICES)
  {
    if(ftm_gsm_get_chain_from_dispatch_device(rftest_data->rx_device) != ftm_gsm_get_chain_from_dispatch_device(rfm_dev))
    {
      ftm_gsm_device_clear_chain_vars(rfm_dev);
    }
  }
  else
  {
    ftm_gsm_device_clear_chain_vars(rfm_dev);
  }
    
  ftm_gsm_free_vars( rfm_dev ); //free ftm_gsm_rfctl_vars

  /*Get the Prx Device*/
  ftm_gsm_chain_vars_s* prx_chain_vars_ptr = 
    ftm_gsm_get_chain_vars_ptr(ftm_gsm_get_chain_from_dispatch_device( rftest_data->rx_device));
  
  if(rftest_data->drx_device == rfm_dev)
  {
    RF_MSG_1(FTM_HIGH, "Dropping drx dev %d", rfm_dev);
    rftest_data->drx_device = RFM_INVALID_DEVICE;
    ftm_gsm_rftest_set_sub_from_device( rfm_dev, UNDEFINED_SUB_ID );
    if(prx_chain_vars_ptr != NULL)
    {
      prx_chain_vars_ptr->associated_drx_dev = RFM_INVALID_DEVICE;
    }
  }
  
  if(rftest_data->tx_device == rfm_dev)
  {
    RF_MSG_1(FTM_HIGH, "Dropping tx dev %d", rfm_dev);
    rftest_data->tx_device = RFM_INVALID_DEVICE;
    ftm_gsm_rftest_set_sub_from_device( rfm_dev, UNDEFINED_SUB_ID );
    if(prx_chain_vars_ptr != NULL)
    {
      prx_chain_vars_ptr->associated_drx_dev = RFM_INVALID_DEVICE;
    }
  }
}



/*===========================================================================

FUNCTION ftm_gsm_rftest_create_rfctl_vars

DESCRIPTION
   Configures rfctl_vars for Prx device and associated devices (Assoc Rx, Assoc Tx,)
   
DEPENDENCIES
   None.

RETURN VALUE
...Error Code if function ran into exception


===========================================================================*/

ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_create_and_update_prx_rfctl_vars
(
  rfm_device_enum_type rfm_dev, 
  uint8 sub_id, 
  ftm_rf_test_device_radio_trx_t trx
)
{
  ftm_rf_test_field_error_code_mask_type status = 0;
  ftm_gsm_rftest_radio_config_t * rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
  ftm_gsm_chains_enum_type gsm_curr_chain = FTM_GSM_INVALID_CHAIN;
  ftm_gsm_chain_vars_s * prx_chain_vars_ptr = NULL;
  ftm_gsm_chain_vars_s * assoc_chain_vars_ptr = NULL;
  
  if((trx != FTM_RF_TEST_RADIO_TRX_INVALID) && (rftest_data != NULL) && (rfm_dev < RFM_MAX_DEVICES))
  {

    /* Allocate PRx device FTM vars */
    if((trx == FTM_RF_TEST_RADIO_RX))
    {
      RF_MSG_1(FTM_HIGH, "Adding prx dev %d", rfm_dev);
      rftest_data->rx_device = rfm_dev;

      /*Update Chain Vars*/
      gsm_curr_chain = ftm_gsm_get_chain_from_dispatch_device( rfm_dev );
      prx_chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( gsm_curr_chain );      
      prx_chain_vars_ptr->prx_dev = rfm_dev;
      prx_chain_vars_ptr->rx_vars_ptr  = ftm_gsm_get_vars_ptr(prx_chain_vars_ptr->prx_dev);
      prx_chain_vars_ptr->sub_id = sub_id;
      ftm_gsm_device_create_cmn_vars_ptr( prx_chain_vars_ptr );

      /*Assign any existing associated devices*/
      if(IS_DEVICE_VALID(rftest_data->drx_device))
      {
        ftm_gsm_rfctl_vars_s * drx_vars = ftm_gsm_get_vars_ptr(rftest_data->drx_device);
        drx_vars->dispatch_device = rfm_dev;
        prx_chain_vars_ptr->associated_drx_dev = rftest_data->drx_device;
      }
      if(IS_DEVICE_VALID(rftest_data->tx_device))
      {
        ftm_gsm_rfctl_vars_s * tx_vars = ftm_gsm_get_vars_ptr(rftest_data->drx_device);
        tx_vars->dispatch_device = rfm_dev;
        prx_chain_vars_ptr->associated_tx_dev = rftest_data->tx_device;
        if (ftm_gsm_device_is_tx_supported(prx_chain_vars_ptr->associated_tx_dev)) 
        {
          prx_chain_vars_ptr->tx_vars_ptr  = tx_vars;
        }
      }
    }
    else
    {
      RF_MSG_1(FTM_ERROR, "Invalid Primary Trx = %d", trx);
      status |= 0x2;
      return status;
    }

  }
  else
  {
    RF_MSG_1(FTM_ERROR, "ftm_gsm_rftest_radio_config_dev_add: Invalid Trx or Null rftest ptr id %d", trx);
    status |= 0x2;
  }
  return status;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_create_rfctl_vars

DESCRIPTION
   Configures rfctl_vars for Prx device and associated devices (Assoc Rx, Assoc Tx,)
   
DEPENDENCIES
   None.

RETURN VALUE
...Error Code if function ran into exception


===========================================================================*/

ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_create_and_update_assoc_rfctl_vars
(
  rfm_device_enum_type rfm_dev, 
  uint8 sub_id, 
  ftm_rf_test_device_radio_trx_t trx
)
{
  ftm_rf_test_field_error_code_mask_type status = 0;
  ftm_gsm_rftest_radio_config_t * rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
  ftm_gsm_rfctl_vars_s * assoc_vars = ftm_gsm_get_vars_ptr(rfm_dev);
  ftm_gsm_chains_enum_type gsm_curr_chain = FTM_GSM_INVALID_CHAIN;
  ftm_gsm_chain_vars_s * prx_chain_vars_ptr = NULL;
  ftm_gsm_chain_vars_s * assoc_chain_vars_ptr = NULL;
  
  if((trx != FTM_RF_TEST_RADIO_TRX_INVALID) && (rftest_data != NULL) && (rfm_dev < RFM_MAX_DEVICES))
  {
    /*Get the Prx Chain Var*/
    if(IS_DEVICE_VALID(rftest_data->rx_device))
    {
      prx_chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( ftm_gsm_get_chain_from_dispatch_device( rftest_data->rx_device ));
      assoc_vars->dispatch_device = rftest_data->rx_device;
    }
    
    /*Allocate Associate Chain Vars Ptr*/
    assoc_chain_vars_ptr = ftm_gsm_get_chain_vars_ptr(ftm_gsm_get_chain_from_dispatch_device( rfm_dev ));
    assoc_chain_vars_ptr->sub_id = sub_id;

    /* Allocate associated DRx device FTM vars */
    if((trx == FTM_RF_TEST_RADIO_RX))
    {
      RF_MSG_1(FTM_HIGH, "Adding drx dev %d", rfm_dev);
      rftest_data->drx_device = rfm_dev;
      if(prx_chain_vars_ptr != NULL)
      {
        prx_chain_vars_ptr->associated_drx_dev = rfm_dev;
      }
    }

    /* Allocate associated Tx device FTM vars */
    if(trx == FTM_RF_TEST_RADIO_TX )
    {
      RF_MSG_1(FTM_HIGH, "Adding tx dev %d", rfm_dev);
      rftest_data->tx_device = rfm_dev;
      if(prx_chain_vars_ptr != NULL)
      {
        prx_chain_vars_ptr->associated_tx_dev = rfm_dev;
        if (ftm_gsm_device_is_tx_supported(prx_chain_vars_ptr->associated_tx_dev)) 
        {
          prx_chain_vars_ptr->tx_vars_ptr  = ftm_gsm_get_vars_ptr(prx_chain_vars_ptr->associated_tx_dev);
        }
      }
    }
    
    ftm_gsm_device_create_cmn_vars_ptr( assoc_chain_vars_ptr );
  
  }
  else
  {
    RF_MSG_1(FTM_ERROR, "ftm_gsm_rftest_radio_config_dev_add: Invalid Trx or Null rftest ptr id %d", trx);
    status |= 0x2;
  }
  return status;
}



/*===========================================================================

FUNCTION ftm_gsm_rftest_check_and_set_default_assoc_devices

DESCRIPTION
   Checks to see if missing assoc_rx/assoc_tx devices. If missing, set default device
   
DEPENDENCIES
   None.

RETURN VALUE
...Error Code

===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_finalize_radio_config(uint8 sub_id)
{
  ftm_gsm_rftest_radio_config_t * rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
  ftm_gsm_rftest_radio_control_t * rftest_ctl_data = ftm_gsm_rftest_get_rf_control_data_ptr(sub_id);
  uint32 status = 0;
  
  if(rftest_data == NULL)
  {
    RF_MSG_1(FTM_ERROR, "Null rftest_data sub_id = %d", sub_id);
    status |= 2;
    return status;
  }
  
  /*Calls Set Mode Set Chan*/
  if((rftest_data->rx_device < RFM_MAX_DEVICES) && (rftest_data->gsm_mode < FTM_PHONE_MODE_MAX) && (rftest_data->arfcn != INVALID_ARFCN))
  {
    ftm_gsm_chain_vars_s* chain_vars_ptr = 
          ftm_gsm_get_chain_vars_ptr(ftm_gsm_get_chain_from_dispatch_device(rftest_data->rx_device));

    /*Configure Drx Device*/
    if(rftest_data->drx_device >= RFM_MAX_DEVICES)
    {
      ftm_gsm_rftest_create_and_update_assoc_rfctl_vars(rfc_common_get_preferred_associated_rx_device(rftest_data->rx_device), 
                                                        sub_id, 
                                                        FTM_RF_TEST_RADIO_RX);
      ftm_gsm_rftest_set_sub_from_device(rftest_data->drx_device, sub_id );
      FTM_MSG_1(FTM_HIGH, "Setting default rxD device %d ", rftest_data->drx_device);
    }

    /*Configure Tx Device*/
    if(rftest_data->drx_device >= RFM_MAX_DEVICES)
    {
      ftm_gsm_rftest_create_and_update_assoc_rfctl_vars(ftm_gsm_get_associated_tx_device(rftest_data->rx_device), 
                                                        sub_id, 
                                                        FTM_RF_TEST_RADIO_TX);
      ftm_gsm_rftest_set_sub_from_device(rftest_data->tx_device, sub_id );
      FTM_MSG_1(FTM_HIGH, "Setting default Tx device %d ", rftest_data->tx_device);
    }

    /*Set Mode Set Chan*/
    ftm_gsm_set_mode(rftest_data->rx_device, rftest_data->gsm_mode);
    ftm_gsm_set_arfcn(chain_vars_ptr, rftest_data->arfcn);
    
    /*Enable Rx*/
    ftm_gsm_rftest_set_rx_burst_params(rftest_ctl_data, 0, FTM_RX_FUNCTION_RF_RX_HW_ON, 0, 0);
    ftm_gsm_rx_burst_command(rftest_data->rx_device, &rftest_ctl_data->rx_burst, 0);
    
  }

  else
  {
    FTM_MSG_4(FTM_ERROR, "invalid Prx Parameters dev %d, mode %d, chan %d, sub_id %d ", rftest_data->rx_device, rftest_data->gsm_mode, rftest_data->arfcn, sub_id);
  }

  return status;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_set_mode_set_chan

DESCRIPTION
   Effectively calls set_mode_set_chan and takes care of the internal data keeping
   
DEPENDENCIES
   None.

RETURN VALUE
...Error Code


===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_set_mode_set_chan
(
  rfm_device_enum_type device, 
  uint8 sub_id, 
  ftm_rf_test_device_radio_trx_t trx,
  boolean is_primary,
  ftm_mode_id_type gsm_mode,
  uint16 arfcn
)
{
  ftm_rf_test_field_error_code_mask_type status = 0;
  ftm_gsm_rftest_radio_config_t * rftest_config_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);

  if(rftest_config_data == NULL)
  {
    status |= 2;
    RF_MSG_2(FTM_ERROR, "Null rftest_data, returning", 0, 0);
    return status;
  }
  
  /*Exit Mode Case*/
  if(gsm_mode == FTM_PHONE_MODE_SLEEP)
  {
    RF_MSG_2(FTM_HIGH, "Calling ftm_gsm_rftest_set_mode sleep device=%d, mode=%d", device, gsm_mode);

    /*Only Call Exit Mode on Prx. Free up ftm_gsm_rfctl_var for device param*/
    if(device == rftest_config_data->rx_device)
    {
      ftm_gsm_set_mode(rftest_config_data->rx_device, gsm_mode);
      ftm_gsm_rftest_clear_prx_chain_vars(device, gsm_mode, sub_id);
    }
    else
    {
      /*Free up chain vars for device param*/
      ftm_gsm_rftest_clear_associated_chain_vars(device, sub_id);
    }
  }
  /*Enter Mode Case*/
  else
  {
    RF_MSG_4(FTM_HIGH, "Calling ftm_gsm_rftest_set_mode create_update device=%d, mode=%d, trx=%d, is_pri=%d", 
      device, gsm_mode, trx, is_primary);
    /*Create the Chain Vars/rfctl vars, and update the device and sub parameters*/

    if(is_primary && (trx == FTM_RF_TEST_RADIO_RX))
    {
      status |= ftm_gsm_rftest_create_and_update_prx_rfctl_vars( device, sub_id, trx);
    }
    else
    {
      status |= ftm_gsm_rftest_create_and_update_assoc_rfctl_vars( device, sub_id, trx);
    }

    if(!status)
    {
      ftm_gsm_rftest_set_sub_from_device( device, sub_id );
      
      if(is_primary && (trx == FTM_RF_TEST_RADIO_RX))
      {
        rftest_config_data->gsm_mode = gsm_mode;
        //ftm_gsm_set_mode(rftest_data->rx_device, gsm_mode);
        rftest_config_data->arfcn = arfcn;
        //ftm_gsm_set_arfcn(chain_vars_ptr, arfcn);
      }

      /*Enable Diversity if drx device is passed in*/
      if(!is_primary && (trx == FTM_RF_TEST_RADIO_RX))
      {
        ftm_gsm_set_diversity(device, 1);
      }
    }
  }

  return status;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_radio_config_dev_add

DESCRIPTION
   Adds a new device to be configured (band, chan, tx/rx)
   
DEPENDENCIES
   None.

RETURN VALUE
   Error code if device not added correctly


===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_radio_config_dev_add
(
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data,
  rfm_device_enum_type device
)

{
  ftm_rf_test_field_error_code_mask_type status = 0;
  struct ftm_rf_test_rc_add_list_t * add_list = &command_unpack_data->add_list;
  uint8 sub_id = RFTEST_GET_SUB( command_unpack_data->subscriber );
  ftm_mode_id_type ftm_gsm_mode = FTM_PHONE_MODE_SLEEP;
  
  if(SUB_ID_BOUNDS_FAIL(sub_id))
  {
    RF_MSG_1(FTM_ERROR, "Invalid sub_id %d", sub_id);
    status |= 2;
  }
  else
  {
    RF_MSG_3(FTM_HIGH, "Calling ftm_gsm_rftest_radio_config_dev_add  dev:%d, band:%d, arfcn:%d", 
            device,
            add_list->cmd_params[device].band,
            add_list->cmd_params[device].per_carrier[0].channel);

    if(!ftm_gsm_create_and_init_vars( device, device ))
    {
      RF_MSG_1(FTM_ERROR, "Could not allocate memory for FTM device %d", device);
      status |= 2;
      return status;
    }
    else
    {
      /*Try to get band from command parameters*/
      ftm_gsm_mode = ftm_gsm_rftest_get_ftm_mode_from_sys_band(add_list->cmd_params[device].band);

      /*If no band in parameters, try to get band from arfcn*/
      if(ftm_gsm_mode == FTM_PHONE_MODE_MAX)
      {
        RF_MSG_1(FTM_ERROR, "Invalid Band Passed in %d", add_list->cmd_params[device].band);
        ftm_gsm_mode = ftm_gsm_rftest_convert_arfcn_to_band((uint16)add_list->cmd_params[device].per_carrier[0].channel);
        RF_MSG_1(FTM_ERROR, "Converted arfcn to ftm_gsm_band %d", ftm_gsm_mode);
      }

      /*If still no band, then return error*/
      if(ftm_gsm_mode == FTM_PHONE_MODE_MAX)
      {
        RF_MSG_1(FTM_ERROR, "Invalid Arfcn Passed in %d. Returning!", add_list->cmd_params[device].per_carrier[0].channel);
        status |= 2;
        return status;
      }
      
      /*Try to initialize G and Set Mode*/
      status |= ftm_gsm_rftest_set_mode_set_chan(device, 
                              sub_id, 
                              add_list->cmd_params[device].trx, 
                              add_list->cmd_params[device].is_primary, 
                              ftm_gsm_mode,
                              (uint16)add_list->cmd_params[device].per_carrier[0].channel);

    }
  }

  return status;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_radio_config_dev_drop

DESCRIPTION
   Removes a configured device (band, chan, tx/rx)
   
DEPENDENCIES
   None.

RETURN VALUE
   Error code if device not dropped correctly


===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_radio_config_dev_drop
(
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data,
  rfm_device_enum_type device
)
{
  ftm_rf_test_field_error_code_mask_type status = 0;
  uint8 sub_id = RFTEST_GET_SUB( command_unpack_data->subscriber );

  RF_MSG_1(FTM_HIGH, "Calling ftm_gsm_rftest_radio_config_dev_drop  dev:%d", device);
  
  /*Need to free associated Rx Device/Drx Device/Associated Tx Device/Chain Vars/Sub Vars  too (handled in ftm_gsm_rftest_set_mode)*/
  status |= ftm_gsm_rftest_set_mode_set_chan(device, 
                          sub_id, 
                          FTM_RF_TEST_RADIO_TRX_INVALID,
                          FALSE,
                          FTM_PHONE_MODE_SLEEP,
                          0);

  return status;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_radio_config_dev_update

DESCRIPTION
   Updates a currently configured device's band, channel, tx/rx configuration
   
DEPENDENCIES
   None.

RETURN VALUE
   Error code if device not updated correctly


===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_radio_config_dev_reconfig
(
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data,
  rfm_device_enum_type device
)
{
  ftm_rf_test_field_error_code_mask_type status = 0;
  struct ftm_rf_test_rc_reconfig_list_t * reconfig_list = &command_unpack_data->reconfig_list;
  ftm_mode_id_type ftm_gsm_mode = FTM_PHONE_MODE_SLEEP;
  uint8 sub_id = RFTEST_GET_SUB( command_unpack_data->subscriber );

  if(SUB_ID_BOUNDS_FAIL(sub_id))
  {
    RF_MSG_1(FTM_ERROR, "Invalid sub_id %d", sub_id);
    status |= 2;
  }
  else
  {
    RF_MSG_3(FTM_HIGH, "Calling ftm_gsm_rftest_radio_config_dev_reconfig  dev:%d, band:%d, arfcn:%d", 
            device,
            reconfig_list->cmd_params[device].band,
            reconfig_list->cmd_params[device].per_carrier[0].channel);

    if(!ftm_gsm_vars_created(device))
    {
      RF_MSG_1(FTM_ERROR, "No memory allocated for FTM device %d", device);
      status |= 2;
      return status;
    }
    else
    {
      /*Try to get band from command parameters*/
      ftm_gsm_mode = ftm_gsm_rftest_get_ftm_mode_from_sys_band(reconfig_list->cmd_params[device].band);

      /*If no band in parameters, try to get band from arfcn*/
      if(ftm_gsm_mode == FTM_PHONE_MODE_MAX)
      {
        RF_MSG_1(FTM_ERROR, "Invalid Band Passed in %d", reconfig_list->cmd_params[device].band);
        ftm_gsm_mode = ftm_gsm_rftest_convert_arfcn_to_band((uint16)reconfig_list->cmd_params[device].per_carrier[0].channel);
        RF_MSG_1(FTM_ERROR, "Converted arfcn to ftm_gsm_band %d", ftm_gsm_mode);
      }

      /*If still no band, then return error*/
      if(ftm_gsm_mode == FTM_PHONE_MODE_MAX)
      {
        RF_MSG_1(FTM_ERROR, "Invalid Arfcn Passed in %d. Returning!", reconfig_list->cmd_params[device].per_carrier[0].channel);
        status |= 2;
        return status;
      }

    
      /*Create the Chain Vars/rfctl vars, and update the device and sub parameters*/
      status |= ftm_gsm_rftest_set_mode_set_chan(device, 
                              sub_id, 
                              reconfig_list->cmd_params[device].trx, 
                              reconfig_list->cmd_params[device].is_primary,
                              ftm_gsm_mode,
                              reconfig_list->cmd_params[device].per_carrier[0].channel);

    }
  }
  
  return status;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_radio_config_dev_config

DESCRIPTION
   Adds a new device to be configured (band, chan, tx/rx)
   
DEPENDENCIES
   None.

RETURN VALUE
   Error code if device not added correctly

===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_radio_config_dev
(
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data, 
  ftm_gsm_rftest_dev_config_t config_type
)
{
  /*Error Code*/
  ftm_rf_test_field_error_code_mask_type status = 0;
  uint8 sub_id;
  ftm_gsm_rftest_radio_config_t * radio_config_data;
  if(command_unpack_data == NULL)
  {
    ERR_FATAL("Null Pointer command_unpack_data passed in", 0, 0, 0);
  }

  /*Get the sub_id*/
  sub_id = RFTEST_GET_SUB( command_unpack_data->subscriber );
  
  /*Get pointer to rftest_config global data*/
  radio_config_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);

  /*For device extraction to be dropped/added/reconfigured*/
  ftm_rf_test_rfm_device_mask_type device_mask = 0;
  rfm_device_enum_type device = RFM_MAX_WAN_DEVICES;

  if(radio_config_data)
  {
    /*Get the correct device mask and the function pointer for the device config type*/
    switch(config_type)
    {
      case GRFTEST_DEVCONFIG_DROP:
        device_mask = command_unpack_data->drop_list.device_mask;
        radio_config_data->ftm_gsm_rftest_dev_config_fp = ftm_gsm_rftest_radio_config_dev_drop;
        break;

      case GRFTEST_DEVCONFIG_ADD:
        device_mask = command_unpack_data->add_list.device_mask;
        radio_config_data->ftm_gsm_rftest_dev_config_fp = ftm_gsm_rftest_radio_config_dev_add;
        break;

      case GRFTEST_DEVCONFIG_RECONFIG:
        device_mask = command_unpack_data->reconfig_list.device_mask;
        radio_config_data->ftm_gsm_rftest_dev_config_fp = ftm_gsm_rftest_radio_config_dev_reconfig;
        break;

      default:
        RF_MSG_1(FTM_ERROR, "Invalid Dev Config Type id %d", config_type);
    }

    /*Loop through the devices and run the corresponding function pointer*/
    while(device_mask != 0)
    {
      device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location( device_mask );
      device_mask &= (~rf_extract_first_one_bit_mask( device_mask ));

      /*Bounds Check*/
      if(device >=RFM_MAX_WAN_DEVICES)
      {
        RF_MSG_1(FTM_ERROR, "ftm_gsm_rftest_radio_config_dev_add: Invalid device id %d", device);
        status |= 0x2;
        continue;
      }

      /*Process the drop/add/or config*/
      status |= radio_config_data->ftm_gsm_rftest_dev_config_fp( command_unpack_data, device );
    }
  }
  else
  {
    RF_MSG_1(FTM_ERROR, "NULL grf radio config pointer", 0);
    status |= 0x2;
  }
  
  return status;
}


/*===========================================================================
 EXTERNAL FUNCTIONS
===========================================================================*/


/*===========================================================================

FUNCTION ftm_gsm_rftest_init

DESCRIPTION
   Function inits gsm rftest
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
void ftm_gsm_rftest_init( void )
{
  static boolean init_once = FALSE;

  /*Register the Radio config Command*/
  ftm_rf_test_radio_config_register_tech_cb(RFM_EGSM_MODE, ftm_gsm_rftest_radio_config);
  
  /*Rx Measure Command*/
  ftm_rf_test_rx_measure_register_tech_cb(RFM_EGSM_MODE,ftm_gsm_rftest_rx_measure);

  /*Tx Control command*/
  ftm_rf_test_tx_control_register_default_tech_cb(RFM_EGSM_MODE,ftm_gsm_rftest_tx_control);

  /*Initialize Mutex*/
  if(!init_once)
  {
    rex_init_crit_sect(&ftm_gsm_rftest_globals.ftm_gsm_rftest_device_lock);
  }
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_lock_device/unlock_device

DESCRIPTION
   Lock/unlock rftest device mutex
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
void ftm_gsm_rftest_lock_device(void)
{
  rf_common_enter_critical_section(&ftm_gsm_rftest_globals.ftm_gsm_rftest_device_lock);

  FTM_MSG_1( FTM_HIGH,
                 "rftest_debug: Locked rftest device", 0);
}

void ftm_gsm_rftest_unlock_device(void)
{
  rf_common_enter_critical_section(&ftm_gsm_rftest_globals.ftm_gsm_rftest_device_lock);

  FTM_MSG_1( FTM_HIGH,
                 "rftest_debug: unLocked rftest device", 0);
}

/*===========================================================================

FUNCTION ftm_gsm_rftest_set_sub_from_device

DESCRIPTION
   Function takes the device and sub_id passed in, and sets the sub_id for the mapped chain
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
void ftm_gsm_rftest_set_sub_from_device(rfm_device_enum_type dev, uint8 sub_id)
{
  FTM_MSG_2(FTM_HIGH, "ftm_gsm_rftest_set_sub_from_device dev %d, sub %d", dev, sub_id);
  ftm_gsm_rftest_globals.ftm_gsm_rftest_chain_to_sub_map[ftm_gsm_get_chain_from_dispatch_device( dev )] = sub_id;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_get_sub_from_device

DESCRIPTION
   Function returns the sub from the device passed in
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
uint8 ftm_gsm_rftest_get_sub_from_device(rfm_device_enum_type dev)
{
  uint8 sub_id = ftm_gsm_rftest_globals.ftm_gsm_rftest_chain_to_sub_map[ftm_gsm_get_chain_from_dispatch_device(dev)];
  FTM_MSG_2(FTM_HIGH, "ftm_gsm_rftest_get_sub_from_device dev %d, sub %d", dev, sub_id);
  return sub_id;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_get_sub_from_chain

DESCRIPTION
   Function returns the sub from the chain passed in
   
DEPENDENCIES
   None.

RETURN VALUE
   uint8

SIDE EFFECTS
  None

===========================================================================*/
uint8 ftm_gsm_rftest_get_sub_from_chain(ftm_gsm_chains_enum_type chain)
{
  FTM_MSG_2(FTM_HIGH, "ftm_gsm_rftest_get_sub_from_chain chain %d, sub %d", chain, ftm_gsm_rftest_globals.ftm_gsm_rftest_chain_to_sub_map[chain]);
  return ftm_gsm_rftest_globals.ftm_gsm_rftest_chain_to_sub_map[chain];
}


/*===========================================================================

FUNCTIONs:  ftm_gsm_rftest_get_remapped_device, ftm_gsm_rftest_get_associated_tx_device, ftm_gsm_rftest_get_associated_rx_device

DESCRIPTION
   If the new rftest_dev_config is enabled by the host, the functions:
   ftm_gsm_rftest_get_associated_rx_device, ftm_gsm_rftest_get_associated_rx_device, ftm_gsm_rftest_get_associated_rx_device
   need to return the below corresponding functions, instead of the static value stored in RFC.
   
DEPENDENCIES
   None.

RETURN VALUE
   RFM Device associated with the request

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type ftm_gsm_rftest_get_remapped_device(uint8 sub_id)
{
  rfm_device_enum_type dev = RFM_INVALID_DEVICE;
  ftm_gsm_rftest_radio_config_t * rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
  
  if(rftest_data != NULL)
  {
    dev = rftest_data->rx_device;
  }
  else
  {
    FTM_MSG_1(FTM_ERROR, "Invalid Sub ID = %d, returning invalid device", sub_id);
  }
  
  FTM_MSG_2(FTM_HIGH, "ftm_gsm_rftest_get_remapped_device dev %d, sub %d", dev, sub_id);
  
  return dev;
}

rfm_device_enum_type ftm_gsm_rftest_get_associated_tx_device(uint8 sub_id)
{
  rfm_device_enum_type dev = RFM_INVALID_DEVICE;
  ftm_gsm_rftest_radio_config_t * rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
  
  if(rftest_data != NULL)
  {
    dev = rftest_data->tx_device;
  }
  else
  {
    FTM_MSG_1(FTM_ERROR, "Invalid Sub ID = %d, returning invalid device", sub_id);
  }
  
  FTM_MSG_2(FTM_HIGH, "ftm_gsm_rftest_get_associated_tx_device dev %d, sub %d", dev, sub_id);
  return dev;
}

rfm_device_enum_type ftm_gsm_rftest_get_associated_rx_device(uint8 sub_id)
{
  rfm_device_enum_type dev = RFM_INVALID_DEVICE;
  ftm_gsm_rftest_radio_config_t * rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
  
  if(rftest_data != NULL)
  {
    dev = rftest_data->drx_device;
  }
  else
  {
    FTM_MSG_1(FTM_ERROR, "Invalid Sub ID = %d, returning invalid device", sub_id);
  }
  
  FTM_MSG_2(FTM_HIGH, "ftm_gsm_rftest_get_associated_rx_device dev %d, sub %d", dev, sub_id);
  
  return dev;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_get/set_rftest_enabled

DESCRIPTION
   Get rf_test_enabled flag
   
DEPENDENCIES
   None.

RETURN VALUE
   Boolean (TRUE if new rftest interface is enabled, false if otherwise)

SIDE EFFECTS
  None

===========================================================================*/
boolean ftm_gsm_rftest_get_rftest_enabled(void)
{
  return ftm_gsm_rftest_globals.ftm_gsm_rftest_enabled;
}
void ftm_gsm_rftest_set_rftest_enabled(boolean enabled)
{
  ftm_gsm_rftest_globals.ftm_gsm_rftest_enabled = enabled;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_get_rftest_data_ptr

DESCRIPTION
   Function handles common radio_config command from host machine
   
DEPENDENCIES
   None.

RETURN VALUE
   Pointer to ftm_gsm_rftest_radio_data

SIDE EFFECTS
  None

===========================================================================*/
ftm_gsm_rftest_radio_config_t * ftm_gsm_rftest_get_rftest_data_ptr(uint8 sub_id)
{
  ftm_gsm_rftest_radio_config_t * ptr_to_return;
  if(SUB_ID_BOUNDS_FAIL(sub_id))
  {
    FTM_MSG_1(FTM_ERROR, "Invalid Sub ID = %d, returning NULL Pointer", sub_id);
    ptr_to_return = NULL;
    
  }
  else
  {
    ptr_to_return = &ftm_gsm_rftest_globals.ftm_gsm_rftest_radio_data[sub_id];
  }

  return ptr_to_return;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_get_rf_control_data_ptr

DESCRIPTION
   Get pointer to RF control data (for Rx measure and Tx pwr control)
   
DEPENDENCIES
   None.

RETURN VALUE
   Pointer to ftm_gsm_rftest_radio_control

SIDE EFFECTS
  None

===========================================================================*/
ftm_gsm_rftest_radio_control_t * ftm_gsm_rftest_get_rf_control_data_ptr(uint8 sub_id)
{
  ftm_gsm_rftest_radio_control_t * ptr_to_return;
  if(SUB_ID_BOUNDS_FAIL(sub_id))
  {
    FTM_MSG_1(FTM_ERROR, "Invalid Sub ID = %d, returning NULL Pointer", sub_id);
    ptr_to_return = NULL;  
  }
  else
  {
    ptr_to_return = &ftm_gsm_rftest_globals.ftm_gsm_rftest_radio_control[sub_id];
  }
  return ptr_to_return;
}


/*===========================================================================

FUNCTION ftm_gsm_rftest_rx_measure

DESCRIPTION
   Function handles common rx_measure command from host machine. 
   Command is broken down per device
   Rx_Carrier_X:
   Device_Y
   Expected AGC
   LNA Gain State
   
DEPENDENCIES
   None.

RETURN VALUE
   None.

SIDE EFFECTS
   Effectively calls set_mode, set_band, set_chan, set_tx_band, enable_tx based off the parameters passed in

===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_rx_measure
(
  ftm_rf_test_rx_measure_unpacked_data_t *query_data,
  uint32 num_of_measurements
)
{
  rfm_device_enum_type rfm_dev = RFM_INVALID_DEVICE;
  int16 expected_rx_pwr = 0;
  uint8 sub_id = UNDEFINED_SUB_ID;
  ftm_gsm_rftest_radio_config_t * rftest_data = NULL;
  ftm_gsm_rftest_radio_control_t * rftest_ctl_data = NULL;
  uint32 status = 0;
  FTM_MSG_1(FTM_HIGH, "ftm_gsm_rftest_rx_measure: num_measurements: %d", num_of_measurements);

  for(uint32 i = 0; i < num_of_measurements; i++)
  {
    rfm_dev = query_data[i].input.device;
    expected_rx_pwr = query_data[i].input.expected_agc; //passed in as dBx16
    sub_id = ftm_gsm_rftest_get_sub_from_device(rfm_dev);
    rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
    rftest_ctl_data = ftm_gsm_rftest_get_rf_control_data_ptr(sub_id);

    FTM_MSG_4(FTM_HIGH, "ftm_gsm_rftest_rx_measure sub_id: %d, rfm_dev: %d, expected_agc: %d, meas_num: %d", sub_id, rfm_dev, expected_rx_pwr, i);
    
    if((rftest_data == NULL) || (rftest_ctl_data == NULL))
    {
      FTM_MSG_2(FTM_ERROR, "Invalid Device/Sub_ID combo dev %d, sub_id %d ", 
        rfm_dev, 
        sub_id);
      status |= 2;
      break;
    }

    if((rfm_dev == rftest_data->rx_device) || (rfm_dev == rftest_data->drx_device))
    {
      ftm_gsm_rfctl_vars_s *rx_rfctl_vars = ftm_gsm_get_vars_ptr(rfm_dev);
      
      /*For now until common_control code is updated, for the num bursts averaging, we'll use 3*/
      status |= ftm_gsm_rftest_set_rx_burst_params(rftest_ctl_data, 0, FTM_RX_FUNCTION_RF_RX_HW_ON, expected_rx_pwr, 3);
      ftm_gsm_core_setup_rx_burst_calver(rfm_dev, rftest_ctl_data->rx_burst_params);
      query_data[i].output.rxagc = ftm_gsm_get_rssi_dbx16(rfm_dev);
      query_data[i].output.lna_gain_state = (uint16)rx_rfctl_vars->ftm_gain_range.range;

      FTM_MSG_2(FTM_HIGH, "agc_measured = %d, lna_gain_state = %d", query_data[i].output.rxagc, query_data[i].output.lna_gain_state);
    }
  }
  
  return status;
}



/*===========================================================================

FUNCTION ftm_gsm_rftest_tx_control

DESCRIPTION
   Function handles common tx_control command from host machine. 
   Command is broken down per device
   Carrier_X:
   Device_Y
  .Tx Action
   Tx Power
   
DEPENDENCIES
   None.

RETURN VALUE
   None.

SIDE EFFECTS
   Effectively calls set_mode, set_band, set_chan, set_tx_band, enable_tx based off the parameters passed in

===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_tx_control
(
  ftm_rf_test_tx_control_unpacked_data_t *query_data,
  uint32 num_actions
)
{
  rfm_device_enum_type rfm_dev = RFM_INVALID_DEVICE;
  boolean tx_on = FALSE;
  uint32 status = 0;
  int16 tx_power = 0;
  uint8 sub_id = UNDEFINED_SUB_ID;
  ftm_gsm_rftest_radio_config_t * rftest_data = NULL;
  ftm_gsm_rftest_radio_control_t * rftest_ctl_data = NULL;
  FTM_MSG_1(FTM_HIGH, "ftm_gsm_rftest_tx_control: num_actions: %d", num_actions);
  for(uint32 i = 0; i < num_actions; i++)
  {
    rfm_dev = query_data[i].input.device;
    tx_on = query_data[i].input.tx_action;
    tx_power = query_data[i].input.tx_power;
    sub_id = ftm_gsm_rftest_get_sub_from_device(rfm_dev);
    rftest_data = ftm_gsm_rftest_get_rftest_data_ptr(sub_id);
    rftest_ctl_data = ftm_gsm_rftest_get_rf_control_data_ptr(sub_id);

    FTM_MSG_4(FTM_HIGH, "ftm_gsm_rftest_tx_control sub_id: %d, rfm_dev: %d, tx_power: %d, tx_on: %d", sub_id, rfm_dev, tx_power, tx_on);

    if((rftest_data == NULL) || (rftest_ctl_data == NULL))
    {
      FTM_MSG_2(FTM_ERROR, "Invalid Device/Sub_ID combo dev %d, sub_id %d ", 
        rfm_dev, 
        sub_id);
      status |= 2;
      break;
    }
    
    if(rfm_dev == rftest_data->tx_device)
    {
      status |= ftm_gsm_rftest_set_tx_burst_params(rftest_ctl_data, 0, tx_on, tx_power, GL1_HW_PDTCHU_MCS1);
      
      /*Set Tx Burst*/
      ftm_gsm_tx_burst_command(rfm_dev, &rftest_ctl_data->tx_burst);

      /*Set Frame Matrix*/
      ftm_gsm_set_frame_matrix(rfm_dev, rftest_ctl_data->frame_matrix);

      /*Set Tx ON*/
      ftm_gsm_set_tx(rfm_dev, tx_on);
    }
    else
    {
      FTM_MSG_2(FTM_ERROR, "RFTEST Tx Device %d doesn't match existing Tx Device %d", 
        rfm_dev, 
        rftest_data->tx_device);
      status |= 2;
      break;
    }
    
  }
  
  
  return status;
}



/*===========================================================================

FUNCTION ftm_gsm_rftest_radio_config

DESCRIPTION
   Function handles common radio_config command from host machine
   Command is broken down per device
   Rx_Carrier_X:
   Device_Y
   Band
   Channel
   
DEPENDENCIES
   None.

RETURN VALUE
   None.

SIDE EFFECTS
   Effectively calls set_mode, set_band, set_chan, set_tx_band, enable_tx based off the parameters passed in

===========================================================================*/
ftm_rf_test_field_error_code_mask_type ftm_gsm_rftest_radio_config( ftm_rf_test_radio_config_unpack_data_t *command_unpack_data )
{
  ftm_rf_test_field_error_code_mask_type status = 0;

  FTM_MSG_1(FTM_HIGH, "ftm_gsm_rftest_radio_config: Received start command ", 0);

  ftm_gsm_rftest_set_rftest_enabled(TRUE);
  
  /*ISR might be running. Need to lock here to prevent issues with concurrency*/
  ftm_gsm_rftest_lock_device();

  /*Follow this order of Drop, Add and Reconfig*/

  /*Step 1 : Process the Drop list*/
  status |= ftm_gsm_rftest_radio_config_dev(command_unpack_data, GRFTEST_DEVCONFIG_DROP);

  /*Step 2 :  Process the Add list*/
  status |= ftm_gsm_rftest_radio_config_dev(command_unpack_data, GRFTEST_DEVCONFIG_ADD);

  /*Step 3: Process the Reconfig list*/
  status |= ftm_gsm_rftest_radio_config_dev(command_unpack_data, GRFTEST_DEVCONFIG_RECONFIG);

  FTM_MSG_1(FTM_HIGH, "ftm_gsm_rftest_radio_config: Processed start command! Status = %d ", status);

  /*Fill G default devices if not updated. And set the global ftm_rf_test_enabled flag to true*/
  if(status == 0)
  {
    FTM_MSG_1(FTM_HIGH, "Finalizing RF Config ", 0);
    ftm_gsm_rftest_finalize_radio_config(RFTEST_GET_SUB(command_unpack_data->subscriber));
  }

  ftm_gsm_rftest_check_any_enabled_devices();

  ftm_gsm_rftest_unlock_device();
  
  return status;


}
#ifdef FEATURE_QSH_MDUMP
/*===========================================================================

FUNCTION ftm_gsm_get_rftest_globals_address

DESCRIPTION
   Returns the address of ftm_gsm_rftest_globals

DEPENDENCIES
   None.

RETURN VALUE
   Address of ftm_gsm_rftest_globals

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR ftm_gsm_rftest_global_t * ftm_gsm_get_rftest_globals_address( void )
{
  return &ftm_gsm_rftest_globals;
}
#endif /* FEATURE_QSH_MDUMP */


#endif //FEATURE_GSM
#endif // FTM_HAS_UMTS
#endif //FEATURE_FACTORY_TESTMODE

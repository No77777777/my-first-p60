/*===========================================================================

               G S M   DEVICE     F U N C T I O N S

DESCRIPTION
   This file contains implementations for FTM GSM device functions

   Copyright (c) 2015 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  Initially, comments will be entered manually. Later, as the module is
  placed under version control, the version control tool will automatic-
  ally insert change comments into this section. [Delete this comment].

  
  $Header: //components/rel/rftech_gsm.mpss/3.11/ftm/src/ftm_gsm_device.c#1 $ 
  $DateTime: 2021/03/09 05:10:17 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/02/20   mpa     To support MiniDump Feature
12/14/15   hkm     GSM850 unsuupported RF Cards crashes because of default band set as 850
11/27/15   hkm     TXRXSplit support changes
09/09/15   tsr     Initialise ftm_gsm_sub_vars[sub_id] to null after freeing memory
08/26/15   tsr      Clear rfctl vars/chain vars for assoc tx/rx devices
07/31/15   hoh     adapt frame sweep cal to work with Rx/Tx device split architecture
08/05/15   zc      Add support for FTM RF Test Framework
07/24/15   tsr     Fix Idle Frame processing in FTM 
07/17/15   tsr     Adde new api to retrieve Non-Signalling device from RFC based on FDM 
07/13/15   tsr     Move GSM Non-Signalling state variables under common chain variables
07/07/15   tsr     Added new api to get preferred rx device from RFC for Non-Signalling 
06/19/15   tsr     Added new api to retrieve sub id from a tx/rx device by querying ftm chain vars 
06/10/15   tsr     Added FTM and Non-Signalling Support on a chain if TX device is not supported  
05/27/15   tsr     Add new apis to check ftm state in RFGSM CORE LAYER    
04/24/15   tsr     Add api to query if the device passed in is a Primary RX device 
04/15/15   sc      Rely on chain vars for split Rx/Tx info throughout call-flow
04/08/15   tsr     Initial version created.Introduced ftm gsm chains based framework for split RX/TX devices
=============================================================================*/

#include "ftm.h"
#include "ftm_msg.h"
#include "ftm_gsm_device.h"
#include "ftm_gsm_ctl.h"
#include "rfgsm_core.h"
#include "rf_test_task_v.h"
#include "rfcommon_core_utils.h"
#include "modem_mem.h"
#include "err.h"
#include "ftm_gsm_rftest.h"
#include "rfcommon_core_utils.h"

#ifdef FEATURE_QSH_MDUMP 
#include "qsh.h"
#endif

/*============================================================================
             LOCAL/EXTERNAL VARIABLES                                                      
=============================================================================*/

/* ftm_gsm_chain_vars will have all the state variables specific to a ftm gsm chain */

static ftm_gsm_chain_vars_s * ftm_gsm_chain_vars[FTM_GSM_MAX_CHAINS] = {0};

static ftm_gsm_sub_vars_s * ftm_gsm_sub_vars[FTM_SUB_ID_MAX]={0};

extern boolean ftm_gsm_ber_create_and_init_vars( ftm_gsm_cmn_vars_s* cmn_vars_ptr);

#define RFGSM_FTM_RFC_BAND_SUPPORTED(mask, band)  ((mask>>band) & 1)

/*===========================================================================
             FUNCTION IMPLEMENTATIONS
===========================================================================*/

/*===========================================================================

FUNCTION ftm_gsm_get_preferred_rx_device

DESCRIPTION 
  This function queries rfc to obtain a preferred rx device for a given mode
 
 
PARAMETERS
  ftm_mode_id_type mode- The mode for which preferred rx device is to be obtained from RFC


RETURN VALUE
  preferred rx device

SIDE EFFECTS
  None

===========================================================================*/
static rfm_device_enum_type ftm_gsm_get_preferred_rx_device( rfcom_band_type_u band )
{
  rfm_device_enum_type pref_rx_device = RFM_INVALID_DEVICE;

  sys_band_class_e_type sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_EGSM_MODE, band);
  
  if( sys_band == SYS_BAND_CLASS_NONE )
  {
    FTM_MSG_1(FTM_ERROR,"ftm_gsm_get_preferred_rx_device: Invalid"
                        "sys_band from GSM band %d", band.gsm_band);
    return pref_rx_device;
  }

  pref_rx_device = rfc_common_get_preferred_rx_device(sys_band); 

  return pref_rx_device;
}

/*===========================================================================

FUNCTION ftm_gsm_initialise_chains

DESCRIPTION


  Parameters:
 

DEPENDENCIES
   

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ftm_gsm_initialise_chains( void )
{
  ftm_gsm_chains_enum_type ftm_gsm_chain;

  for( ftm_gsm_chain=0; ftm_gsm_chain < FTM_GSM_MAX_CHAINS; ftm_gsm_chain++ )
  {
    (void)ftm_gsm_create_and_init_chain_vars( ftm_gsm_chain );
  }
}

/*===========================================================================

FUNCTION ftm_gsm_get_associated_tx_device

DESCRIPTION 
  This function queries rfc to obtain a associated tx device for a given prx device
 
 
PARAMETERS
  prx_dev- The prx device for which associated tx device is to be obtained from RFC


RETURN VALUE
  remapped rx device

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type ftm_gsm_get_associated_tx_device( rfm_device_enum_type prx_dev )
{
  rfm_device_enum_type tx_dev = RFM_INVALID_DEVICE;
  uint8 sub_id = 0xFF;
  sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfcom_band_type_u                  rfcom_band = {0};
  rfcom_gsm_band_type                rfgsm_com_band = RFCOM_BAND_GSM850;

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(prx_dev);
  
  if (rfgsm_core_handle_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_handle_ptr is NULL!", 0);
    return tx_dev;
  }

  if( prx_dev >= RFM_MAX_WAN_DEVICES )
  {
    ERR_FATAL("ftm_gsm_get_associated_tx_device() invalid prx_dev %d", prx_dev, 0, 0);
  }
  
  for ( rfgsm_com_band = 0; rfgsm_com_band < RFGSM_MAX_UNIQUE_RFCOM_BANDS; rfgsm_com_band++ )
  {
    rfcom_band.gsm_band = rfgsm_com_band;
    if (RFGSM_FTM_RFC_BAND_SUPPORTED(rfgsm_core_handle_ptr->device_capability.rx_supported_band_mask, rfgsm_com_band))
    {
       break;
    }
    else
    {
       continue;
    }
  }
		
  if(ftm_gsm_rftest_get_rftest_enabled())
  {
    sub_id = ftm_gsm_rftest_get_sub_from_device(prx_dev);
    tx_dev = ftm_gsm_rftest_get_associated_tx_device( sub_id );

  }
  else
  {
    sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_EGSM_MODE, rfcom_band);
    tx_dev = rfc_common_get_preferred_tx_device(sys_band);
  }

  return tx_dev;
}

/*===========================================================================

FUNCTION ftm_gsm_get_associated_rx_device

DESCRIPTION 
  This function queries rfc to obtain a associated drx device for a given prx device
 
 
PARAMETERS
  prx_dev- The prx device for which associated drx device is to be obtained from RFC


RETURN VALUE
  remapped rx device

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type ftm_gsm_get_associated_rx_device( rfm_device_enum_type prx_dev )
{
  rfm_device_enum_type drx_dev = RFM_INVALID_DEVICE;
  uint8 sub_id = 0xFF;
  
  if( prx_dev >= RFM_MAX_WAN_DEVICES )
  {
    ERR_FATAL("ftm_gsm_get_associated_rx_device() invalid prx_dev %d", prx_dev, 0, 0);
  }

  if(ftm_gsm_rftest_get_rftest_enabled())
  {
    sub_id = ftm_gsm_rftest_get_sub_from_device(prx_dev);
    drx_dev = ftm_gsm_rftest_get_associated_rx_device( sub_id );

  }
  else
  {
    /* Get the associated DRx device from the PRx device */
    drx_dev = rfc_common_get_preferred_associated_rx_device( prx_dev );
  }

  return drx_dev;
}

/*===========================================================================

FUNCTION ftm_gsm_get_remapped_rx_device_info

DESCRIPTION 
  This function queries rfc to obtain a remapped device for a given dispatch device
 
 
PARAMETERS
  The dispatch device for which remapped rx device has to be obtained from RFC


RETURN VALUE
  remapped rx device

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type ftm_gsm_get_remapped_rx_device_info( rfm_device_enum_type dispatch_device )
{
  //source device band is hardcoded here but may need  to change if device mapping is band dependent in the future
  rfc_cal_device_remap_info_type source_device_info = {RFM_MAX_WAN_DEVICES, RFM_EGSM_MODE, RFC_CONFIG_RX, RFCOM_BAND_GSM850, 0};
  rfc_cal_device_remap_info_type remapped_device_info;
  uint8 sub_id = 0xFF;
  source_device_info.device = dispatch_device;
  
  if( dispatch_device < RFM_MAX_WAN_DEVICES )
  {
    if(!ftm_gsm_rftest_get_rftest_enabled())
    {
      rfc_common_get_remapped_device_info(&source_device_info, &remapped_device_info);
    }
    else
    {
      sub_id = ftm_gsm_rftest_get_sub_from_device(dispatch_device);
      remapped_device_info.device = ftm_gsm_rftest_get_remapped_device(sub_id);
    }
  }
  else
  {
    ERR_FATAL("ftm_gsm_get_remapped_rx_device_info() invalid dispatch_device %d", dispatch_device, 0, 0);
  }

  return remapped_device_info.device;
}

/*===========================================================================

FUNCTION ftm_gsm_get_ns_assigned_device

DESCRIPTION 
  This function queries rfc to obtain a preferred rx device for a Non_Signalling device
 
 
PARAMETERS
  None

RETURN VALUE
  gsm_ns_device 

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type ftm_gsm_allocate_ns_device( void )
{
  rfcom_band_type_u                  rfcom_band;
  rfcom_gsm_band_type                rfgsm_com_band;
  rfm_device_enum_type gsm_ns_device = RFM_INVALID_DEVICE;
  for ( rfgsm_com_band = 0; rfgsm_com_band < RFGSM_MAX_UNIQUE_RFCOM_BANDS; rfgsm_com_band++ )
  {
    rfcom_band.gsm_band = rfgsm_com_band;
    gsm_ns_device = ftm_gsm_get_preferred_rx_device(rfcom_band);

    if (gsm_ns_device == RFM_INVALID_DEVICE) 
    {
      FTM_MSG_2( FTM_MED, 
                 "ftm_gsm_allocate_ns_device():Invalid "
                 "device (%d) returned for gsm band:%d!", 
                 gsm_ns_device, rfcom_band.gsm_band ); 
      /*still need to check if there is preferred device for remaining bands*/
      continue;
    } 
    else 
    {
      FTM_MSG_2( FTM_MED, 
                 "ftm_gsm_allocate_ns_device():Pref "
                 "device (%d) returned for gsm band:%d!", 
                 gsm_ns_device, rfcom_band.gsm_band ); 
      break;
    }
  }

  /*Check if none of the bands returned preferred rx device*/
  if (gsm_ns_device == RFM_INVALID_DEVICE) 
  {
     FTM_MSG( FTM_ERROR, 
              "ftm_gsm_allocate_ns_device():Invalid "
              "device returned for all bands)" );
  }

  return gsm_ns_device;
}



/*===========================================================================

FUNCTION ftm_gsm_device_init_cmn_vars

DESCRIPTION 
  This function initialises common variables for a ftm gsm chain
 
PARAMETERS
  ftm_gsm_cmn_vars_s - pointer to the common varibles


RETURN VALUE
  preferred rx device

SIDE EFFECTS
  None

===========================================================================*/
void ftm_gsm_device_init_cmn_vars (ftm_gsm_cmn_vars_s* cmn_vars)
{
  if (FALSE == cmn_vars->init_once)
  {
    cmn_vars->gsm_current_state = FTM_STATE_PARK;

    /*gsm cal flag set during rx/tx calibration*/
    cmn_vars->ftm_gsm_cal_flag = FALSE;

    /*By default Idle frame processing flag is set to TRUE*/
    cmn_vars->ftm_gsm_idle_proc_flag = TRUE;

    /*Initialise gsm ber variables*/
    ftm_gsm_ber_create_and_init_vars( cmn_vars );

    cmn_vars->init_once = TRUE;

    cmn_vars->frame_sweep_fn_count = 0;

    cmn_vars->gsm_frame_sweep_enabled = FALSE;

    cmn_vars->gsm_frame_sweep_paused_count = 0;

    cmn_vars->frame_sweep_header_ptr = NULL;
	
    cmn_vars->ftm_gl1_hw_gsm_band = SYS_BAND_NONE;
  }
  else
  {
     FTM_MSG( FTM_MED,
              "ftm_gsm_device_init_cmn_vars()"
              "already initialised");
  }

}

/*===========================================================================

FUNCTION ftm_gsm_device_get_and_update_chain_vars

DESCRIPTION
     This function updates the ftm gsm chain variables based on dispatch rx device and returns a pointer to ftm gsm
     chain vars

PARAMETERS
  dispatched_rx_device

DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
ftm_gsm_chain_vars_s* ftm_gsm_device_get_and_update_chain_vars( rfm_device_enum_type dispatched_rx_device)
{
  ftm_gsm_chains_enum_type gsm_curr_chain = ftm_gsm_get_chain_from_dispatch_device( dispatched_rx_device );

  ftm_gsm_chain_vars_s* chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( gsm_curr_chain );

  if (chain_vars_ptr==NULL) 
  {
     FTM_MSG_ERROR("GSM FTM variables not created for FTM GSM CHAIN: %d",
                   gsm_curr_chain,0,0 );
     return NULL;
  }

  /* Update the sub */
  chain_vars_ptr->sub_id = ftm_gsm_get_sub_from_chain( gsm_curr_chain );

  /* This is to ensure that user has not provided the RX device */
  if( chain_vars_ptr->prx_dev == RFM_INVALID_DEVICE )
  {
     chain_vars_ptr->prx_dev = dispatched_rx_device;
  }
  
  /* This is to ensure that user has not provided the TX device */
  if( chain_vars_ptr->associated_tx_dev == RFM_INVALID_DEVICE )
  { 
    chain_vars_ptr->associated_tx_dev = ftm_gsm_get_associated_tx_device(chain_vars_ptr->prx_dev);
  }

  /* This is to ensure that user has not provided the DRX device*/
  if( chain_vars_ptr->associated_drx_dev == RFM_INVALID_DEVICE )
  {
    chain_vars_ptr->associated_drx_dev = ftm_gsm_get_associated_rx_device(chain_vars_ptr->prx_dev);
  }

  /* Update the Common, RX and TX chain variables ptrs */
  ftm_gsm_device_create_cmn_vars_ptr( chain_vars_ptr );

  ftm_gsm_device_init_cmn_vars(chain_vars_ptr->cmn_vars_ptr);
  
  if (chain_vars_ptr->rx_vars_ptr == NULL) 
  {
     chain_vars_ptr->rx_vars_ptr  = ftm_gsm_get_vars_ptr(chain_vars_ptr->prx_dev);
  }

  if (chain_vars_ptr->tx_vars_ptr == NULL) 
  {
     /*Only create GSM TX variables if TX is supported on this chain*/
     if (ftm_gsm_device_is_tx_supported(chain_vars_ptr->associated_tx_dev)) {
        chain_vars_ptr->tx_vars_ptr  = ftm_gsm_get_vars_ptr(chain_vars_ptr->associated_tx_dev);
     }
  }

  RF_MSG_4( RF_HIGH, 
            "FTM GSM get chain data: dispatched PRx dev %d, remapped to PRx %d, DRx %d and Tx dev %d", 
            dispatched_rx_device, chain_vars_ptr->prx_dev, chain_vars_ptr->associated_drx_dev, chain_vars_ptr->associated_tx_dev );
  
  RF_MSG_2( RF_HIGH, 
            "FTM GSM get chain data: chain %d, sub %d",
            chain_vars_ptr->gsm_chain, chain_vars_ptr->sub_id );

  return chain_vars_ptr;
}


/*===========================================================================

FUNCTION ftm_gsm_device_clear_chain_vars

DESCRIPTION
     This function clears the allocated chan vars

PARAMETERS
  dispatched_rx_device
  remapped_rx_device 

DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
ftm_gsm_chain_vars_s* ftm_gsm_device_clear_chain_vars( rfm_device_enum_type dispatched_device )
{
  ftm_gsm_rfctl_vars_s* device_vars_ptr = NULL;
  ftm_gsm_chains_enum_type gsm_curr_chain = ftm_gsm_get_chain_from_dispatch_device( dispatched_device );

  ftm_gsm_chain_vars_s* chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( gsm_curr_chain );

  if (chain_vars_ptr==NULL) 
  {
     FTM_MSG_ERROR("GSM FTM variables not created for FTM GSM CHAIN: %d",
                   gsm_curr_chain,0,0 );
  }
  else
  {
    /* Update the Common, RX and TX chain variables ptrs */
    ftm_gsm_device_free_cmn_vars_ptr( chain_vars_ptr );

    /*Clear the chain_vars_ptr memory*/
    modem_mem_free( chain_vars_ptr, MODEM_MEM_CLIENT_RFA );

    chain_vars_ptr = NULL;
  }
  
  return chain_vars_ptr;
}

/*===========================================================================

FUNCTION ftm_gsm_clear_chain_vars

DESCRIPTION
     This function clears the allocated chan vars

PARAMETERS
  gsm curr_chain

DEPENDENCIES
  None.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
boolean ftm_gsm_clear_chain_vars( ftm_gsm_chains_enum_type gsm_curr_chain )
{
  ftm_gsm_chain_vars_s* chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( gsm_curr_chain );

  if (chain_vars_ptr==NULL) 
  {
     FTM_MSG_MED("GSM FTM Chain Vars are alreay NULL for FTM GSM CHAIN: %d",
                  gsm_curr_chain,0,0 );
  }
  else
  {
    /* Update the Common, RX and TX chain variables ptrs */
    ftm_gsm_device_free_cmn_vars_ptr( chain_vars_ptr );

    /*Clear the chain_vars_ptr memory*/
    modem_mem_free( ftm_gsm_chain_vars[gsm_curr_chain], MODEM_MEM_CLIENT_RFA );

    ftm_gsm_chain_vars[gsm_curr_chain] = NULL;

    FTM_MSG_HIGH("Freeing FTM GSM CHAIN Vars ptr: %d",
                   ftm_gsm_chain_vars[gsm_curr_chain], 0, 0 );
  }
  
  return TRUE;
}


/*===========================================================================

FUNCTION ftm_gsm_create_and_init_chain_vars

DESCRIPTION
  This function creates and initialises gsm chain variables based on the current gsm chain 
  and returns the gsm chain variable pointer

PARAMETERS:
  ftm_gsm_chain - The gsm chain for which gsm chain variables need to be initialised

DEPENDENCIES
  None.

RETURN VALUE
  ftm_gsm_chain_vars_s * - A pointer to ftm gsm chain variables

SIDE EFFECTS
  None

===========================================================================*/
ftm_gsm_chain_vars_s * ftm_gsm_create_and_init_chain_vars(ftm_gsm_chains_enum_type ftm_gsm_chain)
{
  if( ftm_gsm_chain >= FTM_GSM_MAX_CHAINS)
  {
    MSG_1( MSG_SSID_FTM, MSG_LEGACY_ERROR, "Unknown FTM GSM CHAIN %d", 
           ftm_gsm_chain);
    return NULL;
  }

  if( ftm_gsm_chain_vars[ftm_gsm_chain] == NULL )
  {
    MSG_1( MSG_SSID_FTM, MSG_LEGACY_HIGH, "Initializing GSM FTM vars for FTM GSM CHAIN %d", 
           ftm_gsm_chain);

    ftm_gsm_chain_vars[ftm_gsm_chain] = modem_mem_alloc( sizeof(ftm_gsm_chain_vars_s),
                                                   MODEM_MEM_CLIENT_RFA );

    if( ftm_gsm_chain_vars[ftm_gsm_chain] != NULL )
    {
       ftm_gsm_chain_vars[ftm_gsm_chain]->gsm_chain           = ftm_gsm_chain;
       ftm_gsm_chain_vars[ftm_gsm_chain]->sub_id              = 0xFF;
       ftm_gsm_chain_vars[ftm_gsm_chain]->prx_dev             = RFM_INVALID_DEVICE;
       ftm_gsm_chain_vars[ftm_gsm_chain]->associated_drx_dev  = RFM_INVALID_DEVICE;
       ftm_gsm_chain_vars[ftm_gsm_chain]->associated_tx_dev   = RFM_INVALID_DEVICE;
       ftm_gsm_chain_vars[ftm_gsm_chain]->cmn_vars_ptr        = NULL;
       ftm_gsm_chain_vars[ftm_gsm_chain]->rx_vars_ptr         = NULL;
       ftm_gsm_chain_vars[ftm_gsm_chain]->tx_vars_ptr         = NULL;
    }
    else
    {
      ERR_FATAL( "Mem alloc for FTM GSM Vars for FTM GSM CHAIN %d failed",
                 ftm_gsm_chain,0,0);
      return NULL;
    }
  }
  else
  {
     FTM_MSG_ERROR("GSM FTM variables already initialised for FTM GSM CHAIN: %d", 
                    ftm_gsm_chain,0,0 );
  }

  return ftm_gsm_chain_vars[ftm_gsm_chain];
}


/*===========================================================================
FUNCTION  ftm_gsm_device_clear_sub_vars

DESCRIPTION
  This function frees heap memory pointed to by ftm_gsm_sub_vars[sub_id]  for a given sub id

PARAMETERS 
  sub_id

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
   None.

===========================================================================*/

void ftm_gsm_device_clear_sub_vars(ftm_gsm_subs_enum_type sub_id)
{
  if(ftm_gsm_sub_vars_created(sub_id))
  {
    MSG_1( MSG_SSID_FTM, MSG_LEGACY_HIGH, "Freeing GSM FTM vars for FTM GSM SUB %d", sub_id);
    modem_mem_free(ftm_gsm_sub_vars[sub_id], MODEM_MEM_CLIENT_RFA);
    ftm_gsm_sub_vars[sub_id] = NULL;
  }
  else
  {
    MSG_1( MSG_SSID_FTM, MSG_LEGACY_HIGH, "Sub vars not initialized for FTM GSM SUB %d", sub_id);
  }
}

/*===========================================================================
FUNCTION  ftm_gsm_create_and_init_sub_vars

DESCRIPTION
  This function creates and initialises subscription variables for a given sub id

PARAMETERS 
  sub_id

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
   None.

===========================================================================*/
void ftm_gsm_create_and_init_sub_vars(ftm_gsm_subs_enum_type sub_id)
{
  if( sub_id >=  FTM_SUB_ID_MAX)
  {
    ERR_FATAL( "Unknown FTM GSM CHAIN %d", 
              sub_id,0,0);
    return;
  }

  if (!ftm_gsm_sub_vars_created(sub_id)) 
  {

    MSG_1( MSG_SSID_FTM, MSG_LEGACY_HIGH, "Initializing GSM FTM vars for FTM GSM SUB %d", sub_id);

    ftm_gsm_sub_vars[sub_id] = modem_mem_alloc( sizeof(ftm_gsm_sub_vars_s),
                                                   MODEM_MEM_CLIENT_RFA );

    if( ftm_gsm_sub_vars[sub_id]!= NULL )
    {
       ftm_gsm_sub_vars[sub_id]->active_gsm_chain = FTM_GSM_MAX_CHAINS;
       ftm_gsm_sub_vars[sub_id]->sub_id           = FTM_SUB_ID_MAX;
       ftm_gsm_sub_vars[sub_id]->ftm_frame_count  = 0;
       ftm_gsm_sub_vars[sub_id]->ftm_tdma_isr_fnc = NULL;
       ftm_gsm_sub_vars[sub_id]->chain_vars_ptr   = NULL;
    }
    else
    {
      ERR_FATAL( "Mem alloc for FTM GSM Vars for FTM GSM subscription %d failed", sub_id, 0, 0);
      return;
    }
  }
  else
  {
    FTM_MSG_HIGH("GSM FTM variables already initialised for FTM GSM subscription: %d",
                  sub_id,0,0 );
  }

}

/*===========================================================================
FUNCTION  ftm_gsm_get_sub_vars_ptr

DESCRIPTION
  This function returns ftm gsm subscription variables for an active ftm gsm
  subscription

PARAMETERS 
  active sub

DEPENDENCIES
  None.

RETURN VALUE
  pointer to sub vars

SIDE EFFECTS
   None.

===========================================================================*/
ftm_gsm_sub_vars_s * ftm_gsm_get_sub_vars_ptr( uint8 active_sub )
{
  if ( ftm_mode != FTM_MODE )
  {
    ERR_FATAL("Attempt to access FTM variables in ONLINE MODE", 0,0,0);
    return NULL;
  }

  if ( active_sub < FTM_SUB_ID_MAX )
  {
    if (ftm_gsm_sub_vars[active_sub] == NULL)
    {
      ftm_gsm_create_and_init_sub_vars(active_sub);
    }
    return ftm_gsm_sub_vars[active_sub];
  }
  else
  {
    ERR_FATAL("Invalid ftm sub specified: %d", active_sub,0,0);
    return NULL;
  }
}

/*===========================================================================
FUNCTION ftm_gsm_assign_sub_vars_chain_ptr

DESCRIPTION
  This function assigns gsm chain variable pointer to active sub variables

PARAMETERS 
  sub_id- Active Subscription
  chain_vars_ptr - Current FTM GSM CHAIN variable pointer

DEPENDENCIES
  None.

RETURN VALUE
  boolean- TRUE or False

SIDE EFFECTS
   None.

===========================================================================*/
void ftm_gsm_assign_sub_vars_chain_ptr( uint8 sub_id, ftm_gsm_chain_vars_s * chain_vars_ptr )
{
  if( chain_vars_ptr == NULL )
  {
    ERR_FATAL("ftm_gsm_assign_sub_vars_chain_ptr() chain_vars_ptr NULL", 0, 0, 0);
  }

  if( sub_id >= FTM_SUB_ID_MAX )
  {
    ERR_FATAL("ftm_gsm_assign_sub_vars_chain_ptr() invalid sub_id %d", sub_id, 0, 0);
  }

  if( ftm_gsm_sub_vars[sub_id] == NULL )
  {
    ERR_FATAL("ftm_gsm_assign_sub_vars_chain_ptr() ftm_gsm_sub_vars not allocated for sub_id %d", sub_id, 0, 0);
  }

  ftm_gsm_sub_vars[sub_id]->chain_vars_ptr = chain_vars_ptr;
}

/*===========================================================================
FUNCTION ftm_gsm_update_sub_vars

DESCRIPTION
  This function updates the current ftm gsm chain and its associated variables to the active subscription

PARAMETERS 
  curr_chain- Current FTM GSM CHAIN
  active sub- Active Subscription

DEPENDENCIES
  None.

RETURN VALUE
  boolean- TRUE or False

SIDE EFFECTS
   None.

===========================================================================*/
boolean ftm_gsm_update_sub_vars( ftm_gsm_chains_enum_type curr_chain,
                                 uint8 active_sub )
{
  ftm_gsm_chain_vars_s * chain_vars_ptr = NULL;
  ftm_gsm_sub_vars_s * sub_vars_ptr = NULL;

  /* Validate chain */
  if( curr_chain >= FTM_GSM_MAX_CHAINS )
  {
    ERR_FATAL("ftm_gsm_update_sub_vars() invalid chain %d", curr_chain, 0, 0);
    return FALSE;
  }

  /* Validate subscription */
  if( active_sub >= FTM_SUB_ID_MAX )
  {
    ERR_FATAL("ftm_gsm_update_sub_vars() invalid subscription %d", active_sub, 0, 0);
    return FALSE;
  }

  /* Retrieve the ftm gsm chain pointer */
  chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( curr_chain );
  if ( chain_vars_ptr == NULL)
  {
    ERR_FATAL("ftm_gsm_update_sub_vars() NULL chain_vars_ptr for chain %d, sub ID %d", curr_chain, active_sub, 0);
    return FALSE;
  }

  sub_vars_ptr = ftm_gsm_get_sub_vars_ptr(active_sub);
  if ( sub_vars_ptr == NULL)
  {
    ERR_FATAL("ftm_gsm_update_sub_vars() NULL sub_vars_ptr for chain %d, sub ID %d", curr_chain, active_sub, 0);
    return FALSE;
  }

  sub_vars_ptr->sub_id           = active_sub;
  sub_vars_ptr->active_gsm_chain = curr_chain;
  sub_vars_ptr->chain_vars_ptr   = chain_vars_ptr;
  sub_vars_ptr->ftm_tdma_isr_fnc = (active_sub == FTM_SUB_ID_0) ? ftm_tdma_isr_sub0 : ftm_tdma_isr_sub1;

  return TRUE;
}

/*===========================================================================
FUNCTION ftm_gsm_get_chain_vars_ptr

DESCRIPTION
  This function obtains the chain variables pointer for a given ftm gsm chain

PARAMETERS 
  curr_chain- Current FTM GSM CHAIN

DEPENDENCIES
  None.

RETURN VALUE
  ftm_gsm_chain_vars_s * - a pointer to FTM GSM Chain variables

SIDE EFFECTS
   None.

===========================================================================*/
ftm_gsm_chain_vars_s * ftm_gsm_get_chain_vars_ptr( ftm_gsm_chains_enum_type curr_chain )
{
  if ( ftm_mode != FTM_MODE )
  {
    ERR_FATAL("Attempt to access FTM variables in ONLINE MODE",
               0,0,0);
    return NULL;
  }

  if ( !(curr_chain < FTM_GSM_MAX_CHAINS) )
  {
    ERR_FATAL("Invalid ftm chain specified: %d", 
              curr_chain,0,0);
    return NULL;
  }

  if (ftm_gsm_chain_vars[curr_chain] == NULL)
  {
    ftm_gsm_create_and_init_chain_vars(curr_chain);
  }

  return ftm_gsm_chain_vars[curr_chain];
}

/*===========================================================================
FUNCTION ftm_gsm_get_chain_from_dispatch_device

DESCRIPTION
  This function obtains ftm gsm chain for a given dispatch device.
  FTM GSM  will  utilize three Primary ftm gsm chains C0/C2/C4 for TX/RX operations
 
PARAMETERS 
  dispatch_device
 
DEPENDENCIES
  None.

RETURN VALUE
  ftm_gsm_chains_type

SIDE EFFECTS
   None.

===========================================================================*/
ftm_gsm_chains_enum_type ftm_gsm_get_chain_from_dispatch_device( rfm_device_enum_type dispatch_device )
{
  ftm_gsm_chains_enum_type ftm_gsm_chain = FTM_GSM_C0;

  if( dispatch_device >= RFM_MAX_DEVICES )
  {
     MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid device for FTM GSM %d ,Initialise ftm gsm chain to FTM_GSM_C0", dispatch_device); 
     return ftm_gsm_chain;
  }

  /* convert device into ftm gsm chains */
  switch( dispatch_device ) 
  {
  case RFM_DEVICE_0:
  case RFM_DEVICE_1:
       ftm_gsm_chain = FTM_GSM_C0;
       break;
  case RFM_DEVICE_2:
  case RFM_DEVICE_3:
       ftm_gsm_chain = FTM_GSM_C2;
       break;

  case RFM_DEVICE_4:
  case RFM_DEVICE_5:
       ftm_gsm_chain = FTM_GSM_C4;
       break;
  default:
    ftm_gsm_chain =FTM_GSM_C0;
    RF_MSG(RF_ERROR,"Default case: FTM_GSM_C0 chain returned");
    break;
  }

  return ftm_gsm_chain;
}

/*===========================================================================
FUNCTION  ftm_gsm_sub_vars_created

DESCRIPTION
  This function test if the GSM FTM Subscription variables have already been created
   and initialised for a given SUB

PARAMETERS 
   sub_id  - FTM GSM Subsrciption id
 
DEPENDENCIES
  None.

RETURN VALUE
  boolean -TRUE or FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean ftm_gsm_sub_vars_created( ftm_gsm_subs_enum_type sub_id )
{
  if ( ftm_gsm_sub_vars[sub_id] == NULL )
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

/*===========================================================================
FUNCTION   ftm_gsm_device_is_tx_supported

DESCRIPTION
  This function checks if a logical device is TX capable.If a TX device is not supported by
  a RFC,it will return RFM_INVALID_DEVICE. The function returns false if device provided is
  RFM_INVALID_DEVICE or supported band mask is not set.If TX device is not supported on a
  FTM chain, FTM initiate RX only Wakeup for FTM

PARAMETERS 
   tx_device
 
DEPENDENCIES
  None.

RETURN VALUE
  boolean -TRUE or FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean ftm_gsm_device_is_tx_supported(rfm_device_enum_type tx_device)
{
  if( tx_device >= RFM_MAX_DEVICES )
  {
    return FALSE;
  }

  return rfgsm_core_get_device_tx_capability(tx_device);
}

/*===========================================================================
FUNCTION  ftm_gsm_get_sub_from_chain

DESCRIPTION
  This function assigns a subscription id for a given ftm gsm chain

PARAMETERS 
   gsm_chain
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
uint8 ftm_gsm_get_sub_from_chain( ftm_gsm_chains_enum_type gsm_chain )
{
  uint8 sub_id = 0xFF;


  if(!ftm_gsm_rftest_get_rftest_enabled())
  {
    switch( gsm_chain )
    {
      case FTM_GSM_C0:
      //case FTM_GSM_C1:
        sub_id = 0;
        break;

    case FTM_GSM_C2:
    //case FTM_GSM_C3:
    case FTM_GSM_C4:
    //case FTM_GSM_C5:
      sub_id = 1;
      break;

      default:
        ERR_FATAL("ftm_gsm_get_chain_to_sub_mapping() invalid chain %d", gsm_chain, 0, 0);
        break;
    }
  }
  else
  {
    sub_id = ftm_gsm_rftest_get_sub_from_chain(gsm_chain);
  }

  return sub_id;
}


/*===========================================================================
FUNCTION  ftm_gsm_device_get_cmn_vars_ptr

DESCRIPTION
  This function creates common variables pointer for a given chain

PARAMETERS 
   chain_vars_ptr - The chain variable pointer for a given ftm gsm chain
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ftm_gsm_device_create_cmn_vars_ptr( ftm_gsm_chain_vars_s * chain_vars_ptr )
{
  if( chain_vars_ptr != NULL )
  {
    if( chain_vars_ptr->cmn_vars_ptr == NULL )
    {
      RF_MSG_1( RF_HIGH, 
                "ftm_gsm_device_get_cmn_vars_ptr() allocating cmn_vars_ptr for chain %d", 
                chain_vars_ptr->gsm_chain );

      /* Allocate the common vars pointer for the chain */
      chain_vars_ptr->cmn_vars_ptr = (ftm_gsm_cmn_vars_s*)modem_mem_alloc( sizeof(ftm_gsm_cmn_vars_s), MODEM_MEM_CLIENT_RFA );

      if( chain_vars_ptr->cmn_vars_ptr == NULL )
      {
        ERR_FATAL( "ftm_gsm_device_get_cmn_vars_ptr() cmn_vars_ptr failed to allcoate for chain %d", chain_vars_ptr->gsm_chain, 0, 0 );
        return;
      }
      chain_vars_ptr->cmn_vars_ptr->init_once = FALSE;
    }
    else
    {
      RF_MSG_1( RF_HIGH, 
                "ftm_gsm_device_get_cmn_vars_ptr() cmn_vars_ptr already allocated for chain %d",
                chain_vars_ptr->gsm_chain );
    }
  }
  else
  {
    ERR_FATAL("ftm_gsm_device_get_cmn_vars_ptr() chain_vars_ptr input NULL", 0, 0, 0);
  }
}


/*===========================================================================
FUNCTION  ftm_gsm_device_free_cmn_vars_ptr

DESCRIPTION
  This function frees common variables pointer for a given chain

PARAMETERS 
   chain_vars_ptr - The chain variable pointer for a given ftm gsm chain
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ftm_gsm_device_free_cmn_vars_ptr( ftm_gsm_chain_vars_s * chain_vars_ptr )
{
  if( chain_vars_ptr != NULL )
  {
    if( chain_vars_ptr->cmn_vars_ptr != NULL )
    {
      RF_MSG_1( RF_HIGH, 
                "ftm_gsm_device_free_cmn_vars_ptr() freeing cmn_vars_ptr for chain %d", 
                chain_vars_ptr->gsm_chain );


      /* Free  the common vars pointer for the chain */
      modem_mem_free( chain_vars_ptr->cmn_vars_ptr, MODEM_MEM_CLIENT_RFA );
      chain_vars_ptr->cmn_vars_ptr = NULL;
      
    }
    else
    {
      RF_MSG_1( RF_HIGH, 
                "ftm_gsm_device_free_cmn_vars_ptr() cmn_vars_ptr already freed for chain %d",
                chain_vars_ptr->gsm_chain );
    }
  }
}


/*===========================================================================
FUNCTION  ftm_gsm_is_device_prx  

DESCRIPTION
  This function returns TRUE if the remmapped rx device is a Primary RX device 

PARAMETERS 
  dispatch_device - RF logical device to obtain current chain
  remapped_rx_device - remapped RX device passed in
   
DEPENDENCIES
  None.

RETURN VALUE
  boolean -TRUE or FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean ftm_gsm_is_device_prx (rfm_device_enum_type dispatch_device ,
                               rfm_device_enum_type remapped_rx_device)
{
  ftm_gsm_chain_vars_s * chain_vars_ptr = NULL;
  ftm_gsm_chains_enum_type gsm_curr_chain = FTM_GSM_INVALID_CHAIN;

  gsm_curr_chain = ftm_gsm_get_chain_from_dispatch_device( dispatch_device);
  chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( gsm_curr_chain );

  if ( chain_vars_ptr == NULL)
  {
    ERR_FATAL("ftm_gsm_is_device_prx () NULL chain_vars_ptr for chain %d, dispatch device %d", 
              gsm_curr_chain, 
              dispatch_device,
              0);
    return FALSE;
  }
  if (remapped_rx_device  == chain_vars_ptr->prx_dev)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================
FUNCTION  ftm_gsm_get_prx_device 

DESCRIPTION
  This function returns Primary RX device for associated DRX and TX devices 

PARAMETERS 
  dispatch_device - RF logical device to obtain current chain
  device - the device passed in
   
DEPENDENCIES
  None.

RETURN VALUE
  rfm_device_enum_type - Primary RX device stored in curr chain

SIDE EFFECTS
  None.

===========================================================================*/
rfm_device_enum_type ftm_gsm_get_prx_device (rfm_device_enum_type dispatch_device ,
                                             rfm_device_enum_type device)
{
  ftm_gsm_chain_vars_s * chain_vars_ptr = NULL;
  ftm_gsm_chains_enum_type gsm_curr_chain = FTM_GSM_INVALID_CHAIN;

  if( dispatch_device >= RFM_MAX_DEVICES ||
      device >= RFM_MAX_DEVICES )
  {
    MSG_2( MSG_SSID_RF, MSG_LEGACY_ERROR,"ftm_gsm_get_prx_device() Invalid device: dispatch device: %d returned device %d", 
           dispatch_device, 
           device); 
    return device;
  }

  gsm_curr_chain = ftm_gsm_get_chain_from_dispatch_device( dispatch_device);
  chain_vars_ptr = ftm_gsm_get_chain_vars_ptr( gsm_curr_chain );

  if ( chain_vars_ptr == NULL)
  {
    ERR_FATAL("ftm_gsm_get_prx_device() NULL chain_vars_ptr for chain %d, dispatch device %d", 
              gsm_curr_chain, 
              dispatch_device,
              0);
  }

  /*if device passed in is prx/drx/tx then return the associated PRX device*/
  if ( device  == chain_vars_ptr->associated_drx_dev || 
       device  == chain_vars_ptr->associated_tx_dev  ||
       device  == chain_vars_ptr->prx_dev)
  {
    return chain_vars_ptr->prx_dev;
  }
  else
  {
     MSG_2( MSG_SSID_RF, MSG_LEGACY_ERROR,"ftm_gsm_get_prx_device() The passed in device %d doesn't match stored devices in Chain: %d", 
            dispatch_device, 
            gsm_curr_chain);
     return device;
  }

}

/*===========================================================================

FUNCTION ftm_gsm_tx_device_is_ftm_state

DESCRIPTION
  This function checks if UE is in FTM Calibration or legacy test mode
  for given tx device.In case of Non-sig and Online, returns False.

DEPENDENCIES
  None

RETURN VALUE
  boolean  -TRUE/FALSE

SIDE EFFECTS
  None

==================================================================*/
boolean ftm_gsm_tx_device_is_ftm_state(rfm_device_enum_type tx_device)
{ 
  ftm_gsm_rfctl_vars_s * tx_vars_ptr = NULL;

  if ( ftm_mode != FTM_MODE )
  {
    return FALSE;
  }
  
  /* Get the Tx Vars to retrieve the dispatched device */
  tx_vars_ptr = ftm_gsm_get_vars_ptr( tx_device );

  if( tx_vars_ptr == NULL )
  {
    ERR_FATAL("ftm_gsm_tx_device_get_ftm_state() tx_vars_ptr is NULL for tx_device %d", tx_device, 0, 0);
  }

  /*Since there is no separate ftm enter mode execution for DRX, FTM_STATE_GSM is only set for PRX*/
  if(ftm_get_current_state(ftm_gsm_get_prx_device( tx_vars_ptr->dispatch_device,
                                                   tx_device)) == FTM_STATE_GSM) /*Check for FTM_STATE_GSM*/
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION ftm_gsm_rx_device_is_ftm_state

DESCRIPTION
  This function checks if UE is in FTM Calibration or legacy test mode
  for given rx device.In case of Non-sig and Online, returns False.

DEPENDENCIES
  None

RETURN VALUE
  boolean  -TRUE/FALSE

SIDE EFFECTS
  None

==================================================================*/
boolean ftm_gsm_rx_device_is_ftm_state(rfm_device_enum_type rx_device)
{ 
  ftm_gsm_rfctl_vars_s * rx_vars_ptr = NULL;

  if ( ftm_mode != FTM_MODE )
  {
    return FALSE;
  }

  /* Get the Rx Vars to retrieve the dispatched device */
  rx_vars_ptr = ftm_gsm_get_vars_ptr( rx_device );

  if( rx_vars_ptr == NULL )
  {
    ERR_FATAL("ftm_gsm_rx_device_get_ftm_state() rx_vars_ptr is NULL for rx device %d", rx_device, 0, 0);
  }

  /*Since there is no separate ftm enter mode execution for DRX, FTM_STATE_GSM is only set for PRX*/
  if(ftm_get_current_state(ftm_gsm_get_prx_device ( rx_vars_ptr->dispatch_device,
                                                    rx_device)) == FTM_STATE_GSM) /*Check if in FTM cal mode*/
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}


/*===========================================================================

FUNCTION ftm_gsm_get_sub_id_from_device

DESCRIPTION
  This function returns ftm subscription stored in ftm chain variables.
  The input device( Rx/Tx) is used to retrieve dispatch device and
  corresponding FTM Chain
 
PARAMETERS 
  device - TX or RX device
 

DEPENDENCIES
  None

RETURN VALUE
  uint8  - FTM GSM Subscription sub_id

SIDE EFFECTS
  None

==================================================================*/
uint8 ftm_gsm_get_sub_id_from_device( rfm_device_enum_type device )
{
  uint8 sub_id = FTM_INVALID_SUB_ID;
  ftm_gsm_chains_enum_type gsm_curr_chain = FTM_GSM_INVALID_CHAIN;
  ftm_gsm_rfctl_vars_s* device_vars_ptr = NULL;
  ftm_gsm_chain_vars_s* chain_vars_ptr =NULL;
  /* Get the device Vars to retrieve the dispatched device in order to get the   */
  /* chain pointer which stores the sub ID                                   */
  device_vars_ptr = ftm_gsm_get_vars_ptr(device); 
  if (device_vars_ptr == NULL) 
  {
    ERR_FATAL( "ftm_gsm_get_sub_id_from_device() "
               "dev_vars_ptr is NULL for device %d",
               device, 0, 0);
    return sub_id;
  }

  gsm_curr_chain = ftm_gsm_get_chain_from_dispatch_device( device_vars_ptr->dispatch_device );

  /*Get the GSM chain variables for current ftm gsm chain*/
  chain_vars_ptr = ftm_gsm_get_chain_vars_ptr(gsm_curr_chain); 
  if (chain_vars_ptr == NULL) 
  {
    ERR_FATAL( "ftm_gsm_get_sub_id_from_device() chain_vars_ptr NULL "
               "for device %d and chain %d", 
               device, gsm_curr_chain, 0);
    return sub_id;
  }

  sub_id = chain_vars_ptr->sub_id;

  MSG_2( MSG_SSID_RF, MSG_LEGACY_LOW,
         "ftm_gsm_get_sub_id_from_device() current ftm gsm chain %d returned sub id %d", 
         gsm_curr_chain, 
         sub_id); 

  return sub_id;

}
#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION  ftm_gsm_get_chain_vars_ptr_address 

DESCRIPTION
  This function returns the address of ftm_gsm_chain_vars 

PARAMETERS 
  None.
   
DEPENDENCIES
  None.

RETURN VALUE
  rfm_device_enum_type - address of ftm_gsm_chain_vars 

SIDE EFFECTS
  None.

===========================================================================*/

QSH_MDUMP_FN_ATTR ftm_gsm_chain_vars_s ** ftm_gsm_get_chain_vars_ptr_address ( void )
{
  return &ftm_gsm_chain_vars[0];
}




#endif /* FEATURE_QSH_MDUMP */


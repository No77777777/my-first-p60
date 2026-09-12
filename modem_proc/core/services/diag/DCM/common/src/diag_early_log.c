/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         Diagnostic early logging configuration

General Description
  Handles the diag early logging configuration such as setting the masks for 
  F3s, Logs and Events and also sets diag in circular buffering mode such that 
  early boot up data gets buffered and sent out once the external connection is 
  established.

Copyright (c) 2018 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                              Edit History

 $Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diag_early_log.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/29/18   ph      Enable diag boot up logging.

===========================================================================*/
#include "core_variation.h"
#include "comdef.h"
#include <string.h>
#include "DALPropDef.h"
#include "DALSys.h"

#include "diag_early_log_cfg.h"
#include "ULogFront.h"
#if defined(DIAG_FEATURE_EFS2)
#include "fs_public.h"
#endif
#include "msgi.h"   
#include "eventi.h"
#include "diaglogi.h"
#include "diag_diagIDi.h"

void diag_read_early_devcfg(void);

#if defined(DIAG_FEATURE_EFS2)
void diag_early_log_nv_config_create(void);
boolean diag_check_early_log_nv_enabled(void);
#endif

/* This is the callback invoked by diag idle processing that clears
   all the masks to avoid generation of data */
void diag_early_mask_cleanup_cb(void* param);


/* Global flag that specifies whether early logging has been
   enabled or not */
boolean diag_early_log_enabled = FALSE;


extern boolean msg_set_rt_mask_adv (uint16 ssid, uint32 mask, byte stream_id, uint8 preset_id);
extern boolean msg_masks_enabled_check(byte stream_id);
extern ULogHandle diag_debug_ulog_handle;
extern osal_timer_t diag_early_log_timer;

/* EFS file paths for early logging configuration */
#define DIAG_EARLY_LOG_CONFIG_FILE_PATH     "/nv/item_files/conf/diag_bootup.conf" /* EFS NV item config file path */
#define DIAG_EARLY_LOG_FLAG_PATH            "/nv/item_files/services/diag/diag_bootup_flag"  /* EFS NV item path */

uint8 diag_early_log_src = DIAG_EARLY_LOG_CONFIG_SRC_NONE;
/*==========================================================================

FUNCTION DIAG_READ_EARLY_LOG_ENABLE

DESCRIPTION
  This function checks whether masks has been enabled and sets the TX mode to 
  circular buffering mode and also starts an early logging timer.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

===========================================================================*/
void diag_early_log_enable(uint8 config)
{
  int return_val = 0;
    
  /* Update the source information */
  diag_early_log_src = config;
      
  /* Update the early logging flag, set to circular buffer and start a timer */
  if(!diag_early_log_enabled)
  {
    diag_early_log_enabled = TRUE;

    /* Set buffering mode to circular */
    diag_set_tx_mode_stream(DIAG_TX_MODE_BUFFERED_CIR, DIAG_STREAM_1, diag_get_diagID());

    /* Set timer of DIAG_EARLY_LOG_TIMER_LEN duration and clear the boot up masks when expires */
    return_val = osal_set_timer(&diag_early_log_timer, DIAG_EARLY_LOG_TIMER_LEN);
    ASSERT(OSAL_SUCCESS == return_val);
  }
}
/*==========================================================================

FUNCTION DIAG_READ_EARLY_DEVCFG

DESCRIPTION
  This function attempts to read the diag boot up configuration file via DEVCFG
  and sets the F3/Log/Event masks appropriately.

PARAMETERS
  None

DEPENDENCIES
  F3 SSID or Log masks Or Event IDs to be updated in devcfg XML files.

RETURN VALUE
  None

===========================================================================*/
void diag_read_early_devcfg(void)
{
  DALSYSPropertyVar propValue;
  DALSYSPropertyVar propVal;
  uint32 *count_ptr = NULL;
  uint32 set_count = 0;
  int i =0;
  uint32* pLogAddr = NULL;
  diag_early_msg_config *pMsg = NULL;

  DALSYS_PROPERTY_HANDLE_DECLARE(pHandle);

  DALSYS_GetDALPropertyHandleStr("diag_early_log_config",pHandle);

  // Check for diag_early_log_control
  if (DAL_SUCCESS == DALSYS_GetPropertyValue(pHandle,"diag_early_log_control", 0, &propValue))
  {
    if(0 == (propValue.Val.dwVal & DIAG_EARLY_LOGGING_ENABLED))
    {
      return;
    }
  }
  else
  {
    ULogFront_RealTimePrintf(diag_debug_ulog_handle, 0, "Could not read diag_early_log_control devcfg");
    return;
  }

  /* Clear all the masks mask if set before */
  (void) log_config_mask (-1, FALSE, DIAG_STREAM_ALL, DIAG_PRESET_MASK_ALL);
  msg_set_all_rt_masks_adv(0, DIAG_STREAM_ALL, DIAG_PRESET_MASK_ALL);
  event_set_all_rt_masks(0, 0);
  
  diag_early_log_enable(DIAG_EARLY_LOG_CONFIG_SRC_DEVCFG);

  /* Extract the log codes that can be enabled */
  if (DAL_SUCCESS == DALSYS_GetPropertyValue(pHandle,"diag_early_log_mask", 0, &propValue))
  {
    pLogAddr = propValue.Val.pdwVal;
    if( DAL_SUCCESS == DALSYS_GetPropertyValue(pHandle,"diag_early_log_mask_size", 0, &propVal))
    {
      count_ptr = propVal.Val.pdwVal;
      set_count = *count_ptr;
      ULogFront_RealTimePrintf(diag_debug_ulog_handle, 1, "Log code enabled count is 0x%x", set_count);

      /* Enable the log codes for STREAM_1 */
      for(i=0;i<set_count;i++)
      {
        log_config_mask( pLogAddr[i], TRUE, DIAG_STREAM_1, DIAG_PRESET_MASK_1);
      }
    }
  }
  else
  {
    ULogFront_RealTimePrintf(diag_debug_ulog_handle, 0, "Could not read diag_early_log_mask devcfg");
  }

  /* Read the event IDs and update the mask for them */
  if (DAL_SUCCESS == DALSYS_GetPropertyValue(pHandle,"diag_early_event_mask", 0, &propValue))
  {
    pLogAddr = propValue.Val.pdwVal;

    if(DAL_SUCCESS == DALSYS_GetPropertyValue(pHandle,"diag_early_event_mask_size", 0, &propVal))
    {
      count_ptr = propVal.Val.pdwVal;
      set_count = *count_ptr;
      ULogFront_RealTimePrintf(diag_debug_ulog_handle, 1, "EVENT count enabled is 0x%x", set_count);

      /* Enable the event IDs for STREAM_1 */
      for(i=0;i<set_count;i++)
      {
        event_config_mask( pLogAddr[i], TRUE, DIAG_STREAM_1, DIAG_PRESET_MASK_1);
      }

      /* Toggle the event_config variable if events are set. This is needed since event_report()
      or event_report_payload checks for event_config and then only proceeds with checking the mask
      and allocating the event */
      if(set_count >0)
      {
        event_control( 0xFF, EVENT_CONFIG_COMM_MASK );
      }
    }
  }
  else
  {
    ULogFront_RealTimePrintf(diag_debug_ulog_handle, 0, "Could not read diag_early_event_mask devcfg");
  }

  /* Read the Message SSIDs that need to be enabled and update the mask */
  if (DAL_SUCCESS == DALSYS_GetPropertyValue(pHandle,"diag_early_message_array", 0, &propValue))
  {
    uint16 ssid = 0;
    uint32 mask = 0;
    pMsg = (diag_early_msg_config *)propValue.Val.pdwVal;
    ssid = pMsg->ssid;
    mask = 1<<pMsg->level;

    if(DAL_SUCCESS == DALSYS_GetPropertyValue(pHandle,"diag_early_message_array_size", 0, &propVal))
    {
      count_ptr = propVal.Val.pdwVal;
      set_count = *count_ptr;
      ULogFront_RealTimePrintf(diag_debug_ulog_handle, 1, "MSG SSID enabled count is 0x%x", set_count);
      for(i=0;i<set_count;i++)
      {
        /* Update the mask with bitwise OR of all the levels mentioned. Once the full mask for an SSID is
        available, invoke the appropriate msg set mask function */
        if (ssid == pMsg->ssid)
        {
          mask|= 1<<pMsg->level;
        }
        else
        {
          msg_set_rt_mask_adv(ssid, mask , DIAG_STREAM_1, DIAG_PRESET_MASK_1);
          ssid = pMsg->ssid;
          mask|= 1<<pMsg->level;
        }
        if ( i == (set_count - 1))
        {
          msg_set_rt_mask_adv(ssid, mask , DIAG_STREAM_1, DIAG_PRESET_MASK_1);
        }
        pMsg++;
      }

      /*
      Toggle the msg_enabled_mask variable if some MSG SSIDs are set. This is needed since MSG
      APIs checks for msg_enabled_mask and then only proceeds with checking the mask
      and allocating the message in diagbuf
      */
      msg_masks_enabled_check(DIAG_STREAM_1);
    }
  }
}

#if defined(DIAG_FEATURE_EFS2)
/*==========================================================================

FUNCTION DIAG_EARLY_LOG_NV_CONFIG_CREATE

DESCRIPTION
  This function creates a EFS configuration file required for 
  displaying the NV item by the tool. (NV#74127)

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

===========================================================================*/
void diag_early_log_nv_config_create(void)
{

  /* File Descriptor */
   int32  fd;
   struct fs_stat temp_buf;
   char str[] = DIAG_EARLY_LOG_FLAG_PATH "\n";

  /* If the conf file does not exist create a new conf file */
  if ( efs_stat(DIAG_EARLY_LOG_CONFIG_FILE_PATH, &temp_buf)!= 0 )    /* 0 -- file exists   (-1)-- file does not exist*/
  {
    ULogFront_RealTimePrintf(diag_debug_ulog_handle, 1, "Creating the config file %s", str);

    /* Open the diag boot up config file. If its not present then create it. */
    fd = efs_open( DIAG_EARLY_LOG_CONFIG_FILE_PATH, O_CREAT|O_AUTODIR|O_WRONLY|O_TRUNC);
    if (fd < 0)
    {
      ULogFront_RealTimePrintf(diag_debug_ulog_handle, 1, "Error opening EFS config file error %d", fd);
      return;
    }

    efs_write( fd, str, (size_t)strlen(str) );

    /* Close the file */
    efs_close( fd );
  }
}
/*==========================================================================

FUNCTION DIAG_CHECK_EARLY_LOG_NV_ENABLED

DESCRIPTION
  This function checks if NV item that controls whether to delete the
  .DIAGIN file after the masks has been set.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

===========================================================================*/
boolean diag_check_early_log_nv_enabled(void)
{
  struct fs_stat temp_buf;
  int result;
  boolean nv_enabled = FALSE;

  /* Check if NV EFS file exists; 0 -- file exists   (-1)-- file does not exist*/
  if(efs_stat(DIAG_EARLY_LOG_FLAG_PATH, &temp_buf) == 0)
  {
    uint8 diag_early_log_flag= 0;
    /* Returns the number of bytes copied in the buffer */
    result = efs_get(DIAG_EARLY_LOG_FLAG_PATH, &diag_early_log_flag,sizeof(diag_early_log_flag));
    if(result == sizeof(diag_early_log_flag))
    {
      if(diag_early_log_flag == DIAG_EARLY_LOGGING_ENABLED)
      {
        nv_enabled = TRUE;
      }
      else
      {
        nv_enabled = FALSE;
        ULogFront_RealTimePrintf(diag_debug_ulog_handle, 2, "Error returned by efs_get(). efs_errno = %d result=%d ",efs_errno,result);
      }
    }
  }
  else
  {
    ULogFront_RealTimePrintf(diag_debug_ulog_handle, 0, "DIAG_EARLY_LOG_FLAG_PATH doesn't exist");
  }

  return nv_enabled;
}
#endif
/*==========================================================================

FUNCTION DIAG_EARLY_MASK_CLEANUP

DESCRIPTION
  This is the callback invoked by diag idle processing. This clears
  the masks to avoid generation of data and there by not sending the data
  to APPS .

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

===========================================================================*/
void diag_early_mask_cleanup(void)
{
  /* Clear all the masks if not received from apps */
  if ( diag_early_log_enabled )
  {
    msg_set_all_rt_masks_adv(0, DIAG_STREAM_ALL, DIAG_PRESET_MASK_ALL);
    (void) log_config_mask (-1, FALSE, DIAG_STREAM_ALL, DIAG_PRESET_MASK_ALL);
    event_set_all_rt_masks(0, DIAG_STREAM_ALL);
  }
}
/*==========================================================================

FUNCTION DIAG_EARLY_MASK_CLEANUP_CB

DESCRIPTION
  This is the callback invoked by diag idle processing. This clears
  the masks to avoid generation of data and there by not sending the data
  to APPS .

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

===========================================================================*/
void diag_early_mask_cleanup_cb(void* param)
{
  /* Clear all the masks if not received from apps */
  diag_early_mask_cleanup();

  (void) diag_idle_processing_unregister(diag_early_mask_cleanup_cb, NULL);
}
/*==========================================================================

FUNCTION DIAG_EARLY_LOG_TIMER_CB

DESCRIPTION
  This callback is invoked when the boot up log timer expires. This registers
  a callback with diag idle processing that clears all the masks enabled.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

===========================================================================*/
void diag_early_log_timer_cb(uint32 params)
{
  /* Reset the timer */
  osal_reset_timer(&diag_early_log_timer);

  /* Register for a idle processing callback and clear the masks in that
  callback. This is to avoid updating the masks(synchronization) in two task
  contexts i.e. timer and DIAG. (As mutexes cannot be used in timer callbacks) */
  (void) diag_idle_processing_register (diag_early_mask_cleanup_cb, NULL);
}

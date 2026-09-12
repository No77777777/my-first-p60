/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          Diag voting changes

General Description
This files contains logging changes.


Copyright (c) 2006 - 2017 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
============================================================================

   $Header: //components/rel/wcdma.mpss/8.4.0/common/src/util_diag_vote.c#4 $
   $DateTime: 2022/03/22 00:20:48 $

                           Edit History

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/30/17   as      Changes to trigger diag vote when log mask is enabled
01/11/22   ha      Disable diag vote changes

===========================================================================*/

#ifdef __cplusplus
  extern "C" {
#endif

#include "customer.h"
#include "diagcmd.h" 
#include "dog_hb_rex.h"
#include "comdef.h"
#include "customer.h"
#include "msg.h"
#include "err.h"
#include "queue.h"
#include "rex.h"
#include "event.h"
#include "logging_ext.h"
#include "logging_int.h"
#include "modem_mem.h"
#include "memheap.h"
#include "rcinit.h"
#include "rcevt.h"
#include "rcinit_rex.h"
#include "stringl.h"
#include "npa.h" /* For npa_client_handle, etc */
#include "coremca.h"          /* for MCA macros */
#include "diag.h"
#include "rcevt_rex.h"


extern boolean diag_mask_active;
extern uint32 modem_log_util_nv;

#define LOG_UTIL_DIAG_VOTE_CIRCULAR_MODE 0x02
#define LOG_UTIL_DIAG_VOTE_DISABLE_STREAMING_MODE 0x04

/*===========================================================================

FUNCTION log_util_check_daig_vote_is_needed

DESCRIPTION
  This procedure votes against sleep for the Diag task.

===========================================================================*/
boolean log_util_check_daig_vote_is_needed(void)
{  
  if(diag_masks_enabled())
  {
    if(diag_get_tx_mode() != DIAG_TX_MODE_BUFFERED_CIR)
    {
     if(!(modem_log_util_nv & LOG_UTIL_DIAG_VOTE_DISABLE_STREAMING_MODE))
     {
       return TRUE;
     }
    }
    else
    {
     if(modem_log_util_nv & LOG_UTIL_DIAG_VOTE_CIRCULAR_MODE)
     {
       return TRUE;
     }
    }
  }
  return FALSE;
} 

/*===========================================================================

FUNCTION log_util_diag_mask_change

DESCRIPTION
  takes care of voting for clock when diag mask is enabled and releasing vote if diag mask is not enabled

===========================================================================*/
void log_util_diag_mask_change(void)
{
  if(log_util_check_daig_vote_is_needed())
  {
    if(diag_mask_active == FALSE)
    {
      log_util_npa_request_vote_for_logging();
      diag_mask_active = TRUE;
      /* For debugging purpose */
      log_util_save_mask_change_history();
    }
  }
  else
  {
    if(diag_mask_active == TRUE)
    {
      log_util_npa_release_vote();
      diag_mask_active = FALSE;
      /* For debugging purpose */
      log_util_save_mask_change_history();
    }
  }
}


/*===========================================================================

FUNCTION log_util_diag_registration

DESCRIPTION
  Registration with diag for diag mask chagne update

===========================================================================*/
void log_util_diag_registration(void)
{
  RCEVT_SIGEX_SIGREX rcevt_sigex;
  rcevt_sigex.signal = rex_self();                     
  rcevt_sigex.mask = LOG_UTIL_DIAG_MASK_CHANGE;     
  
  if(RCEVT_NULL == rcevt_register_sigex_name(DIAG_EVENT_MASK_CHANGE_NOTIFY, RCEVT_SIGEX_TYPE_SIGREX, &rcevt_sigex))
  {
    ERR_FATAL("registration failed for rcevt_register_sigex_name", 0, 0, 0);
  }
  if(RCEVT_NULL == rcevt_register_sigex_name(DIAG_LOG_MASK_CHANGE_NOTIFY, RCEVT_SIGEX_TYPE_SIGREX, &rcevt_sigex))
  {
    ERR_FATAL("registration failed for rcevt_register_sigex_name", 0, 0, 0);
  }
  if(RCEVT_NULL == rcevt_register_sigex_name(DIAG_MSG_MASK_CHANGE_NOTIFY, RCEVT_SIGEX_TYPE_SIGREX, &rcevt_sigex))
  {
    ERR_FATAL("registration failed for rcevt_register_sigex_name", 0, 0, 0);
  }
  if(RCEVT_NULL == rcevt_register_sigex_name(DIAG_TX_MODE_CHANGE_NOTIFY, RCEVT_SIGEX_TYPE_SIGREX, &rcevt_sigex))
  {
    ERR_FATAL("registration failed for rcevt_register_sigex_name", 0, 0, 0);
  }
} 

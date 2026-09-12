/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


              U I M   U T I L   S T A T E   M A C H I N E

GENERAL DESCRIPTION
  This module contains the state machine for the UIM UTILS of the UIM
  interface.

EXTERNALIZED FUNCTIONS

  uim_process_util_command
    Processes a generic UIM server command

  uim_send_util_err_rpt
    Processes an error condition for a util UIM server command

  uim_util_command_response
    This function processes the UIM responses from cdma APDUs.

INITIALIZATION REQUIREMENTS

Copyright (c) 2003,2009-2020 QUALCOMM Technologies, Inc (QTI) and its licensors.All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE


$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimutil.c#5 $
$DateTime: 2020/07/30 05:35:11 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/29/20   ykr     Add null check for channel-id in open channel APDU.
06/16/20   ykr     Update the channel state when manage channel APDU is streamed 
02/20/20   rkg     Add new TLV in Suscription manager
09/17/19   mm      Add EID to SimSlotStatus radio config HAL
05/24/19   ykr     Fix KW errors in uim_get_file_type
04/19/18   gm      Update warning status words
01/22/18   gm      Update warning status word as final response
08/24/17   ks      Fix KW error
05/10/17   ku      Switch  card mode if no MF is indicated in ATR
04/07/17   gm      Not retrying for memory problem status word
03/01/17   gm      Added extra bytes to prevent uart buffer overflow
12/21/16   ks      Workaround to select MF after call control envelope command
12/07/16   ssr     ATR Parsing Optimization
11/24/16   ks      Adding support for spcom interface
11/04/16   ks      Fix in Case-2 stream apdu handling
10/03/16   dd      Removed lint comments
09/22/16   gm      Added support for set APDU behavior
07/25/16   ks      Correcting AID Length check
05/24/16   ssr     F3 Optimization on curr path
05/23/16   na      F3 Optimization
05/17/16   ssr     Remove msg for malloc failure
04/20/16   sam     Support for enhanced subscription manager
03/28/16   sam     Support for SAM card
03/09/16   na      Iccid length is being filled from file size received in FCP of EF-ICCID
12/16/15   ssr     Delay local card power up in Modem SSR in BTSAP mode
11/23/15   sam     Added workaround to select MF after call connect envelope
11/02/15   sam     Added util function to check if a given AID is ARA-M aid
09/02/15   ks      Correcting the powerup logging enums
08/20/15   ks      Resending error notification if mmgsdi_notigy_error fails
08/07/15   ks      Updating the select file type appropriately 
08/07/15   ks      Consolidating the F3 prints to reduce text memory
06/30/15   lm      Enable memory leak detection
06/18/15   ks      Consolidating the F3s that print the directory path
06/11/15   ks      New NV to protect workarounds for non-spec compliant cards
05/28/15   ks      Select MF after EF-PSLOCI update for a buggy card
08/04/15   sam     FR24498: Powerup logging new changes
04/07/15   ks      Select MF after EF-LOCI update for a buggy card
01/07/14   sam     Modification of msg macros for FR24498-UIM Powerup Logging
09/18/14   akv     UIMDRV UT enhancements
08/06/14   js      Handle SW 6280, 63C1 for non-telecom ADFs for activate ADF
07/03/14   sam     Explicit MF selection after location status envelop when 
                   NV 'UIMDRV_FEATURE_ENABLE_EXPLICIT_SELECTION_OF_MF_OR_ADF' is enabled
06/09/14   ll      Add interruptible time delay and return value for update params
06/04/14   na/js   Handling of case 4 commands for non telecom apps
03/21/14   ll      Remove return_ptr and remove/change uim_free to UIM_FREE
03/05/14   sam     Fix to pet dog during power up wait/delay
03/03/14   nmb     Directory maintenance I&T bug fixes
02/25/14   am      Fix compile error due to "stringl.h" inclusion
02/20/14   ak      Fix compile errors due to strict compiler on 9x35
01/29/14   sam     Usage of newer style diag macros
01/14/14   nmb     Move MANAGE CHANNEL command to generic cmd processing
01/14/14   nmb     Remove ACCESS_F and related unused functionality
12/18/13   sam     uim_util_get_hotswap_enabled_slot function is
                   wrongly returning false eventhough hotswap is enabled
10/15/13   sam     Removed include file virtual_uimdrv.h -> file moved to
                   offtarget location
10/03/13   na      F3 log optimization
09/26/13   ak      Remove unnecessary feature flags
09/20/13   ak      Hotswap debug message cleanup
09/16/13   yk      Removed the code for old targets and for old features
08/21/13   hyo/js  Hotswap support on MOB
08/13/31   js      UIMDRV Memory De-Couple changes
08/08/13   ks      Advanced Subscription Manager changes
07/12/13   nmb     Global Reorganization
06/24/13   js      TSTS changes
06/17/13   ks      Removal of F3 messages as part of memory optimization
05/16/13   na      Secure memory copy
02/12/13   js      General F3 macro cleanup
12/05/12   js      UIM Parallel Processing changes
10/26/12   akv     HW Enumeration changes (without features)
10/26/12   akv     HW Enumeration changes
08/09/12   js/rm   Fixes for UIM controller hotswap and hardware WWT features
05/25/12   js      Handle 62 00 SW with correct transaction status
05/14/12   js      Handle 62 00 SW for MANAGE CHANNEL as UIM_DONE
05/01/12   av      Replaced FEATURE_UIM_SUPPORT_HOTSWAP with an nvitem
03/20/12   rm      Changes for WWT and hotswap support
03/20/12   js      Correct P2 param for MANAGE CHANNEL (Open)
03/15/12   rm      Changes for DIME
02/21/12   rm/ak   Setting card detection polarity based on NV item
02/29/12   ak      Fix a klocwork warning
02/18/12   av      Cleaned up remaining ALWAYS ON features
01/20/12   ak      Platform ID/NV item logic for hotswap polarity
12/30/11   ssr     Manage channel for ICC mode
                   1) Fixed manage channel command response for empty response
                      and P2 data, where P2 contains the channel number
                   2) We should not allow Manage channel command in ICC mode
                   3) Reset the channel number to default channel whenever,
                      UIM is resetting the protocol to the ICC mode
12/14/11   rm      Fix to apply card detection polarity for card detection
                   interrupts as well
11/09/11   ms      Fixed compilation warning
09/09/11   ssr     Fix to allow the reset command when switch is un-pressed where
                   client want to disable the hotswap interrupt at runtime,
                   Added new API to return the real time card status,
                   Added new API to disable the hotswap interrupt
09/01/11   ak      GPIO hotswap support for 9x15
08/30/11   yb      Added support for complete recovery solution
                   Added support for caching channel information slot wise
                   Fixed crash on card removal and inserting new card
08/11/11   nmb     Expanded Logical Channels
07/08/11   rm      Catch-up with latest on Hot-swap
04/25/11   yb      Adding support for hotswap
01/17/11   yb      Fix for open channel issue
11/10/10   yt      Memory reduction changes
09/01/10   ps      Merge UIM server DSDS changes
09/26/09   kp      ZI memory reduction changes
05/02/09   ps      Fix 8200 Warnings
12/17/07   rm      Fix to correct the instruction case in some situations
12/11/06   tml     Clear the uim_last_sel_dir for the specific channel and slot
                   upon close channel request
04/03/06   pv      Lint Fixes
03/08/06   pv      Load the status words to the report buffer even if we
                   errored out because of un-known status words
11/14/05   pv      Lint Fixes
08/08/04   tml     Added user data
07/28/05   pv      replaced UIM_MAX_NUM_CDD_ATTEMPTS with UIM_MAX_NUM_ATTEMPTS
                   where not necessary.
07/12/05   tml     Assigned user data in util error
06/08/05   wli     Replaced UIM_MAX_NUM_ATTEMPTS with UIM_MAX_NUM_CMD_ATTEMPTS
           for command retry.
03/07/05   jk      Set the cmd count to max to avoid sending of failed PIN
                   related APDUs (so that PINs don't get blocked)
05/17/04   ck      Removed the use of logical channel table and instead use the
                   channel information from the command.
01/12/04   ck      Added a header file outside the feature to avoid warning.
12/29/03   ck      Merged in the inteface changes in the data direction
                   parameter of the command to the driver.
02/10/03   jk      Created module.  This module contains the UTIL state
                   machine.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"
#include "comdef.h"
#include "bit.h"
#include "uimdrv_msg.h"
#include "err.h"
#include "mmgsdilib_p.h"
#include "uimdrv.h"
#include "uimdrv_main.h"
#include "uimdrv_uartdm.h"
#include "uimi.h"
#include "uimutil.h"
#include <memory.h>
#include "uimgen.h"
#include "fs_public.h"
#include "DDIPlatformInfo.h"
#include <stringl/stringl.h>
#include "uimglobals.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif

/* Index of Interface Byte in ATR */
#define   IFB_1ST     0
#define   IFB_2ND     1
#define   IFB_3RD     2
#define   IFB_4TH     3
#define   IFB_5TH     4

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*===========================================================================
  I N T E R N A L   F U N C T I O N   P R O T O T Y P E S
===========================================================================*/

/*===========================================================================

FUNCTION UIM_UTIL_GET_UIM_CLASS

DESCRIPTION
  Returns the appropriate standard or extended logical channel class byte.

DEPENDENCIES
  None

RETURN VALUE
  Class byte

SIDE EFFECTS
  None

===========================================================================*/
byte uim_util_get_uim_class
(
  byte             uim_ins_class,
  uim_channel_type channel_id
)
{
  if(channel_id < UIM_FIRST_EXTENDED_CHANNEL)
  {
    return (byte)(uim_ins_class | channel_id);
  }
  else
  {
    return (byte)((channel_id - UIM_FIRST_EXTENDED_CHANNEL)
                  | uim_ins_class | UIM_UICC_EXTENDED_LC_CLASS);
  }
}/* uim_util_get_uim_class */


/*===========================================================================
FUNCTION UIM_MALLOC

DESCRIPTION
  The uim_malloc called TMC alloc with different heap_size based the size
  of the allocation required.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  a pointer is allocated.
===========================================================================*/
void * uim_malloc(dword size)
{
  void *return_mem = NULL;
  return_mem = modem_mem_calloc(1,(size),MODEM_MEM_CLIENT_UIM);
  return return_mem;
}/* uim_malloc */

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /*FEATURE_UIM_TEST_FRAMEWORK*/

/*===========================================================================

FUNCTION UIM_MEMSCPY

DESCRIPTION
  This function provides the wrapper to memscpy, which is a secure version of
  memory copy
  dst_ptr is the destination buffer in which contents of src_ptr will be copied.
  dst_len represnts the length/dimension of the dst_ptr while src_len then length
  of contents to be copied into dst_ptr.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_memscpy
(
  void       *dst_ptr,
  size_t      dst_len,
  const void *src_ptr,
  size_t      src_len
)
{
  (void)memscpy(dst_ptr,
                dst_len,
                src_ptr,
                src_len);
} /* uim_memscpy */


/*===========================================================================

FUNCTION UIM_FORMAT_CHV

DESCRIPTION
  This procedure memsets the trailing characters in the CHV buffer with pad
  character (0xFF)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the value of the CHV passed in as a parameter.

===========================================================================*/
void uim_format_chv
(
  byte                     *chv,   /* This points to the CHV buffer that holds the CHV digits.
                                      The digits in this buffer are formatted  */
  uim_instance_global_type *uim_ptr
)
{
  /* Index for processing the CHV buffer */
  byte i = 0;
  /* This holds the index of the last CHV digit */
  byte j = UIM_MAX_CHV_DIGITS;

  if(chv == NULL)
  {
    return;
  }

  /* Look for the end of the CHV digits. */
  for (i = 0; i < UIM_MAX_CHV_DIGITS; i++)
  {
     if (chv[i] == '\0')
     {
       /* This is past the last byte of the CHV digits. */
       /* Start adding pad characters after the CHV digits. */
       j = i;
       break;
     }
  }

  /* Fill the buffer with pad characters */
  for (i = j; i < UIM_MAX_CHV_DIGITS; i++)
  {
    chv[i] = UIM_CHV_PAD_CHAR;
  }
} /* uim_format_chv */


/*========================================================================

FUNCTION UIM_PROCESSING_STREAM_APDU

DESCRIPTION
  This function returns whether UIM is processing a stream apdu command.

DEPENDENCIES
  None

RETURN VALUE
  TRUE : UIM processing STREAM APDU COMMAND
  FALSE: UIM not processing STREAM APDU COMMAND

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_processing_stream_apdu
(
  uim_instance_global_type *uim_ptr
)
{
  /* Check if we are processing ENVELOPE command */
  if( (UIM_STREAM_APDU_ST   == *uim_ptr->command.generic_state_ptr  ||
       UIM_STREAM_SELECT_ST == *uim_ptr->command.generic_state_ptr) &&
       UIM_GENERIC_CMD      ==  uim_ptr->state.cmd_mode)
  {
    return TRUE;
  }

  return FALSE;
} /* uim_processing_stream_apdu */


/*===========================================================================

FUNCTION UIM_IS_MORE_TIME_PCMD_FETCHED

DESCRIPTION
  This procedure tells whether the fetched command is MORE TIME or not

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if the data buffer contains MORE TIME proactive command
  FALSE - if the data buffer does not contain MORE TIME proactive cmd

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_is_more_time_pcmd_fetched(
  const uint8    *data
)
{
  boolean ret_status = FALSE;
  if ( (NULL !=data) && *data == UIM_PROACTIVE_CMD_TAG )
  {
    /* if length encoding is 1 */
    if ( (*(data + 1) != 0x81 && *(data + 5) == PROA_MORE_TIME) ||
         (*(data + 1) == 0x81 && *(data + 6) == PROA_MORE_TIME) )
    {
      UIM_MSG_MED_1 ("Card requested for MORE TIME 0x%x",(*(data + 1)));
      ret_status = TRUE;
    }
  }
  return (ret_status);
} /* uim_is_more_time_pcmd_fetched */


/*===========================================================================

FUNCTION uim_is_location_status_envelope

DESCRIPTION
The function verifies if the current envelop is location status envwlop or not
DEPENDENCIES
  None

RETURN VALUE
  TRUE -- if location status envelop
  FALSE -- if not a location status envelop

SIDE EFFECTS
 NONE
===========================================================================*/

boolean uim_is_location_status_envelope
(
  const uint8    *data
)
{
  if ( (NULL !=data) && *data == UIM_EVT_DOWNLOAD_ENVELOPE)
  {
    if((*(data + 2) == UIM_ENVELOPE_EVENT_TAG)&& 
	(*(data + 3) == 0x01)  && 
	(*(data + 4) == UIM_LOCATION_STATUS_EVT))
    {
      UIM_MSG_MED_0("Curent envelop recieved is a location status envelope");
      return TRUE;
    }
  }
  
  return FALSE;
} /* uim_is_location_status_envelope() */


/*===========================================================================

FUNCTION uim_is_call_connect_envelope

DESCRIPTION
The function verifies if the current envelop is call connect envwlop or not
DEPENDENCIES
  None

RETURN VALUE
  TRUE -- if call connect envelop
  FALSE -- if not a call connect envelop

SIDE EFFECTS
 NONE
===========================================================================*/
boolean uim_is_call_connect_envelope
(
  const uint8    *data
)
{
  int i = 2;
  boolean ret_status = FALSE;

  if ( (NULL !=data) && *data == UIM_EVT_DOWNLOAD_ENVELOPE)
  {
    /*if 1st byte is 0x81 then the length field in the envelope is 2 bytes.
     skip 2 bytes*/
    if(*(data +1) == UIM_ENV_2_BYTE_LENGTH_FIRST_VALUE)
    {
      i = 3;
    }
    if((*(data + i) == UIM_ENVELOPE_EVENT_TAG)&& 
	(*(data + i+1) == 0x01)  && 
	(*(data + i+2) == UIM_CALL_CONNECT_EVT))
    {
      UIM_MSG_MED_1("Curent envelop recieved is a call connect envelope at i 0x%x", i);
      ret_status = TRUE;
    }
  }
  return (ret_status);
} /* uim_is_call_connect_envelope() */

/*===========================================================================

FUNCTION uim_is_call_control_envelope

DESCRIPTION
The function verifies if the current envelop is call control envelop or not
DEPENDENCIES
  None

RETURN VALUE
  TRUE -- if call control envelop
  FALSE -- if not a call control envelop

SIDE EFFECTS
 NONE
===========================================================================*/
boolean uim_is_call_control_envelope
(
  const uint8    *data
)
{
  boolean ret_status = FALSE;

  if ( (NULL != data) && *data == UIM_CALL_CONTROL_TAG )
  {
    UIM_MSG_MED_0("Current envelop recieved is a call control envelope");
    return TRUE;
  }
  return (ret_status);
} /* uim_is_call_control_envelope() */

/*===========================================================================

FUNCTION uim_get_file_type

DESCRIPTION
The function returns the file type of the path received as argument.

DEPENDENCIES
  None

RETURN VALUE
File type of the file path passed

SIDE EFFECTS
 NONE
===========================================================================*/

uim_file_type uim_get_file_type(const uim_path_type path)
{
  /* Set default to EF, proprietary files which doesn't fall into the below 
      category will be set to UIM_EF  */
  uim_file_type file_type = UIM_EF;
  uint8 dir_level = 0;
  
  if((path.len < 1) || (path.len > 5))
  {
    return file_type;
  }
  
  dir_level = (path.path[path.len - 1] & UIM_FILE_ID_LEVEL_MASK) >> UIM_FILE_ID_LEVEL_SHIFT;

  if(dir_level == UIM_MF_LEVEL)
  {
    file_type = UIM_MF;
  }
  else if((dir_level == UIM_DF_LEVEL_2) || (dir_level == UIM_DF_LEVEL_1))
  {
    file_type = UIM_DF;
  }
  else if((dir_level == UIM_EF_UNDER_MF) || (dir_level == UIM_EF_UNDER_2ND_DF) || (dir_level == UIM_EF_UNDER_1ST_DF))
  {
    file_type = UIM_EF; 
  }

  return file_type;
} /* uim_get_file_type() */


/*===========================================================================

FUNCTION uim_is_mf_selection_needed_after_update_bin_command

DESCRIPTION
The function verifies if the current update operation is on location status EF
DEPENDENCIES
  None

RETURN VALUE
  TRUE -- if location status EF or EF PSLOCI is the one to be updated
  FALSE --if location status EF or EF PSLOCI is not the one to be updated

SIDE EFFECTS
 NONE
===========================================================================*/
boolean uim_is_mf_selection_needed_after_update_bin_command
(
  uim_path_type *cmd_path_ptr,
  uim_instance_global_type *uim_ptr

)
{
  uim_path_type ef_loci_path   = {0x3, {0x3F00,0x7FFF,0x6F7E}};
  uim_path_type ef_psloci_path = {0x3, {0x3F00,0x7FFF,0x6F73}};
  boolean       is_mf_selection_needed = FALSE;



  if( uim_ptr->setting.work_arounds_for_non_spec_cards 
          & UIM_MF_SELECTION_AFTER_LOCI_UPDATE_COMMAND )
  {
    if(0 == memcmp(cmd_path_ptr->path, ef_loci_path.path, ef_loci_path.len * 2))
    {
      is_mf_selection_needed = TRUE;
    }
  }

  if( uim_ptr->setting.work_arounds_for_non_spec_cards 
          & UIM_MF_SELECTION_AFTER_EF_PSLOCI_UPDATE_COMMAND )
  {  
    if(0 == memcmp(cmd_path_ptr->path, ef_psloci_path.path, ef_psloci_path.len * 2))
    {
      is_mf_selection_needed = TRUE;
    }
  }
  
  UIM_MSG_HIGH_1("is_mf_selection_needed 0x%x ", is_mf_selection_needed);
  return (is_mf_selection_needed);
} /* uim_is_location_status_update_command() */


/*===========================================================================

FUNCTION uim_is_ef_gsm_loci

DESCRIPTION
  The function verifies if the current operation is on EF-LOCI

DEPENDENCIES
  None

RETURN VALUE
  TRUE -- If current file is ef_loci
  FALSE -- If curent file is not ef_loci

SIDE EFFECTS
 NONE
===========================================================================*/
boolean uim_is_ef_gsm_loci
(
  uim_path_type *cmd_path_ptr
)
{
  uim_path_type ef_loci_path   = {0x3, {0x3F00,0x7F20,0x6F7E}};
  boolean       is_ef_loci     = FALSE;

  if((ef_loci_path.len == cmd_path_ptr->len) &&
     (0 == memcmp(cmd_path_ptr->path, ef_loci_path.path, ef_loci_path.len * 2)))
  {
    is_ef_loci = TRUE;
  }
    
  return is_ef_loci;
} /* uim_is_ef_gsm_loci() */


/*===========================================================================

FUNCTION uim_clk_busy_wait

DESCRIPTION
  This function is a wrapper around clk_busy_wait to avoid data conversion
  issues for different targets and to validate the supplied values.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_clk_busy_wait(dword busy_wait_time)
{
  if ( (int)busy_wait_time > UIM_MAX_CLK_BUSY_WAIT )
  {
    busy_wait_time = UIM_MAX_CLK_BUSY_WAIT;
  }
  DALSYS_BusyWait( (int) busy_wait_time );
} /* uim_clk_busy_wait */


/*===========================================================================

FUNCTION uim_timed_sleep_ext

DESCRIPTION
  This function is a wrapper around rex_timed_wait() to replace rex_sleep()
  with a non-deferrable timer with additional signals to wait for if needed.

DEPENDENCIES
  None

RETURN VALUE
  Return the signals 

SIDE EFFECTS
  None

===========================================================================*/
rex_sigs_type uim_timed_sleep_ext
(
  unsigned long             ms,
  uim_instance_global_type *uim_ptr,
  rex_sigs_type             additional_sigs_to_wait
)
{
#ifndef FEATURE_UIM_TEST_FRAMEWORK
  /* UIM timed sleep timer to replace rex_sleep */
  static rex_timer_type uim_timed_sleep_ext_timer[UIM_MAX_INSTANCES];
  static rex_timer_type *uim_timed_sleep_ext_timer_ptr[UIM_MAX_INSTANCES];
  rex_sigs_type  recved_singal;

  if(uim_timed_sleep_ext_timer_ptr[uim_ptr->id] == NULL)
  {
    timer_def( &uim_timed_sleep_ext_timer[uim_ptr->id], &uim_ptr->task.always_on_timer_group,
               uim_ptr->tcb_ptr, UIM_TIMED_SLEEP_TIMER_SIG,NULL, 0);
    uim_timed_sleep_ext_timer_ptr[uim_ptr->id] = &uim_timed_sleep_ext_timer[uim_ptr->id];

  }
  (void)rex_set_timer(&uim_timed_sleep_ext_timer[uim_ptr->id], ms );
  recved_singal = uim_pet_and_wait((UIM_TIMED_SLEEP_TIMER_SIG|additional_sigs_to_wait), uim_ptr);
  (void)rex_clr_timer(&uim_timed_sleep_ext_timer[uim_ptr->id]);
  (void)rex_clr_sigs(uim_ptr->tcb_ptr, UIM_TIMED_SLEEP_TIMER_SIG);
  return recved_singal;
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
} /* uim_timed_sleep_ext */


/*===========================================================================

FUNCTION uim_timed_sleep

DESCRIPTION
  This function is a wrapper around rex_timed_wait() to replace rex_sleep()
  with a non-deferrable timer with additional signals to wait for if needed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_timed_sleep
(
  unsigned long             ms,
  uim_instance_global_type *uim_ptr,
  rex_sigs_type             additional_sigs_to_wait
)
{
#ifndef FEATURE_UIM_TEST_FRAMEWORK
  /* UIM timed sleep timer to replace rex_sleep */
  static rex_timer_type uim_timed_sleep_timer[UIM_MAX_INSTANCES];
  static rex_timer_type *uim_timed_sleep_timer_ptr[UIM_MAX_INSTANCES];

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  if(uim_timed_sleep_timer_ptr[uim_ptr->id] == NULL)
  {
    timer_def( &uim_timed_sleep_timer[uim_ptr->id], &uim_ptr->task.always_on_timer_group,
               uim_ptr->tcb_ptr, UIM_TIMED_SLEEP_TIMER_SIG,NULL, 0);
    uim_timed_sleep_timer_ptr[uim_ptr->id] = &uim_timed_sleep_timer[uim_ptr->id];

  }
  (void)rex_set_timer(&uim_timed_sleep_timer[uim_ptr->id], ms );
  (void)uim_pet_and_wait((UIM_TIMED_SLEEP_TIMER_SIG|additional_sigs_to_wait), uim_ptr);
  (void)rex_clr_timer(&uim_timed_sleep_timer[uim_ptr->id]);
  (void)rex_clr_sigs(uim_ptr->tcb_ptr, UIM_TIMED_SLEEP_TIMER_SIG);
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
} /* uim_timed_sleep */


/*===========================================================================
FUNCTION UIM_MAX_CH_NUM_OPENED

DESCRIPTION
  This function returns the value of the maximum channel number which was
  opened before recovery

PARAMETER
  None

DEPENDENCIES
  uim_is_ch_open

RETURN VALUE
  uim_channel_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
uim_channel_type uim_max_ch_num_opened
(
  uim_instance_global_type *uim_ptr
)
{
  uim_channel_type ch=0;
  for(ch=UIM_MAX_CHANNELS-1; ch>0; ch--)
  {
    if(uim_ptr->state.is_ch_open[ch]!= FALSE)
    {
      return (ch);
    }
  }
  return 0;
} /* uim_max_ch_num_opened */


/*===========================================================================

FUNCTION UIM_START_POWER_SAVE

DESCRIPTION
  For USB cards this function informs the USB driver that the USB interface is
  not currently needed for any ICCD transaction and hence it can be suspended.
  For legacy ISO cards this function processes a request to turn the UIM clock
  off. It uses a global control variable to determine how to control the clock.

DEPENDENCIES
  For legacy ISO cards this function uses a global variable to determine
  how to control the UIM clock.

RETURN VALUE
  TRUE/FALSE.

SIDE EFFECTS
  For USB cards if all functional interfaces over the USB interface have voted
  for a suspend, the USB interface to the card will be suspended.
  For legacy ISO cards the UIM clock will either be left high or low or will be
   left on, depending on the preferences of the UIM card.

===========================================================================*/
boolean uim_start_power_save
(
  uim_instance_global_type *uim_ptr
)
{
  dword guardtime_wait_value = 0;
  /* Keep a local buffer for controlling the clock. */
  uim_op_params_type local_uim_op_params_buf;

  /* Return status */
  boolean status = TRUE;

  /* First, wait for the guardtime to pass.  This is 2 etus */
  if((uim_ptr->state.op_params_buf.FI < UIM_CRCF_SIZE) &&
     (uim_ptr->state.op_params_buf.DI < UIM_BRAF_SIZE))
  {
    /* No wait needed when card is not available */
    if(braf_values[uim_ptr->state.op_params_buf.DI] !=0)
    {
      guardtime_wait_value = ((crcf_values[uim_ptr->state.op_params_buf.FI]*1000000)/
                            (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq *
                             braf_values[uim_ptr->state.op_params_buf.DI]));
    }
  }
  else
  {
    UIMDRV_MSG_MED_0(uim_ptr->id, "Attempt to access array out of bounds");
    return FALSE;
  }
  if(!uim_ptr->flag.interface_gone_bad)
  {
    /* No wait needed when card is not available */
    if(braf_values[uim_ptr->state.op_params_buf.DI] !=0)
    {
      uim_clk_busy_wait (guardtime_wait_value);
      uim_clk_busy_wait (guardtime_wait_value);
    }
    /* Make sure to wait long enough after the last transaction to
    turn off the clock. */
    uim_clk_busy_wait ((1860*1000000)/uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq);
  }

  if((uim_ptr->command.mode == UIM_PASSIVE_MODE) &&
     (uim_ptr->flag.passive_clk_stop_allowed == FALSE))
  {
    /* Storing the UIM clock preferences. Preferences could be
       CLOCK_STOP_HIGH,CLOCK_STOP_LOW and CLOCK_STOP_NOT_ALLOWED
     */
    uim_ptr->state.clock_stop_cntrl_passive_mode = uim_ptr->state.clock_stop_cntrl;
    uim_ptr->state.clock_stop_cntrl = UIM_CLOCK_STOP_NOT_ALLOWED;
    uim_ptr->flag.passive_clk_stop_allowed = TRUE;
  }
  else if((UIM_PASSIVE_MODE == uim_ptr->command.mode) &&
          (TRUE == uim_ptr->flag.passive_clk_stop_allowed))
  {
     /* Retriveing the stored value of CLOCK STOP preferences */
     uim_ptr->state.clock_stop_cntrl = uim_ptr->state.clock_stop_cntrl_passive_mode;
  }

  /* Determine what to do about stopping the UIM clock. */
  switch (uim_ptr->state.clock_stop_cntrl)
  {
    case UIM_CLOCK_STOP_HIGH:
      {
        /* Stop the clock at a high state */
        local_uim_op_params_buf.change_baud_rate = FALSE;
        local_uim_op_params_buf.change_clk_freq = TRUE;
        local_uim_op_params_buf.change_guardtime = FALSE;
        /* Get the last clock frequency used for the UIM. */
        local_uim_op_params_buf.clock_frequency = UIMDRV_CLK_STOP_HIGH;
        /* Program the clock frequency */
        status = uim_update_op_params( &local_uim_op_params_buf, uim_ptr);
      } /* end case - stop the clock at a high state */
      break;

    case UIM_CLOCK_STOP_LOW:
      {
        /* Stop the clock at a low state */
        local_uim_op_params_buf.change_baud_rate = FALSE;
        local_uim_op_params_buf.change_clk_freq = TRUE;
        local_uim_op_params_buf.change_guardtime = FALSE;
        /* Get the last clock frequency used for the UIM. */
        local_uim_op_params_buf.clock_frequency = UIMDRV_CLK_STOP_LOW;
        /* Program the clock frequency */
        status = uim_update_op_params( &local_uim_op_params_buf, uim_ptr);
      } /* end case - stop the clock at a low state */
      break;

    default:
      {
        /* Do not stop the clock as the card has given a preference to keep it
           on or the control variable is out of range. */
        status = FALSE;
      } /* end case - do not stop the clock */
      break;

  } /* end switch - check clock control */

  return ( status );
} /* uim_start_power_save */


/*===========================================================================

FUNCTION UIM_STOP_POWER_SAVE

DESCRIPTION
  For USB cards this function sends a request to the USB driver to resume
  the USB interface as we need to communicate with the card over the ICCD
  functional interface.
  For legacy ISO cards this function processes a request to turn the UIM clock
  back on. It uses a global control variable to determine how to control the
  clock. It also uses a global variable to determine what frequency to set
  the clock.

DEPENDENCIES
  For legacy ISO cards this function uses a global variable to determine how
  to control the UIM clock and another global variable to determine the
  clock frequency.

RETURN VALUE
  None

SIDE EFFECTS
  For USB cards the USB driver will be requested to resume the interface.
  For ISO cards the UIM clock will be turned on and set to a frequency
  previously determined.

===========================================================================*/
void uim_stop_power_save
(
  uim_instance_global_type *uim_ptr
)
{
  /* Keep a local buffer for controlling the clock. */
  uim_op_params_type local_uim_op_params_buf;

  if(uim_ptr->setting.phy_layer != UIM_ISO)
  {
    return;
  }

  /* Determine what to do about stopping the UIM clock. */
  switch (uim_ptr->state.clock_stop_cntrl)
  {
    case UIM_CLOCK_STOP_HIGH:
    case UIM_CLOCK_STOP_LOW:
      {
        /* Turn the clock back on */
        local_uim_op_params_buf.change_baud_rate = FALSE;
        local_uim_op_params_buf.change_clk_freq = TRUE;
        local_uim_op_params_buf.change_guardtime = FALSE;
        /* Get the last clock frequency used for the UIM. */
        local_uim_op_params_buf.clock_frequency =
          uim_ptr->state.op_params_buf.clock_frequency;
        /* Program the clock frequency */
        uim_update_op_params( &local_uim_op_params_buf, uim_ptr);
        /* Make sure to wait long enough after turning the clock back on
           before processing the next transaction. */
        uim_clk_busy_wait ((700*1000000)/uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq);

      } /* end case - stop the clock at a low state */
      break;

    default:
      {
        /* Do not turn the clock back on as the clock was not turned off
           from before. */
      } /* end case - do not stop the clock */
      break;

  } /* end switch - check clock control */
} /* uim_stop_power_save */


/*===========================================================================

FUNCTION UIM_PRINT_CURR_PATH

DESCRIPTION
  This procedure prints the currently selected path

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_print_curr_path
(
  const uim_path_type      *path,
  uim_instance_global_type *uim_ptr
)
{
  uim_path_type			  temp_path;
 
  if(path->len == 0)
  { 
    return;
  }

  memset(&temp_path, 0, sizeof(uim_path_type));
  temp_path.len = path->len;
  uim_memscpy(temp_path.path,sizeof(temp_path.path),path->path, 2*(path->len));
  

  UIMDRV_MSG_HIGH_6(uim_ptr->id, "Path length is 0x%x Path 0x%X 0x%X 0x%X 0x%X 0x%X",
  	                temp_path.len,
                        temp_path.path[0], 
		        temp_path.path[1],
                        temp_path.path[2],
                        temp_path.path[3],
                        temp_path.path[4]);
  
  return;
} /* uim_print_curr_path */



/*===========================================================================

FUNCTION UIM_PRINT_CURR_PATH_EXT

DESCRIPTION
  This procedure prints the currently selected path only if it is changed from
  previous stored value.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_print_curr_path_ext
(
  const uim_path_type      *path,
  uim_instance_global_type *uim_ptr
)
{
  static uim_path_type	temp_path = {0, {0,0,0,0,0}};
 
  if((path->len == 0) ||
     ((path->len == temp_path.len) && 
      (0 == memcmp(path->path, temp_path.path, (temp_path.len * 2)))))
  { 
    return;
  }

  memset(&temp_path, 0, sizeof(uim_path_type));
  temp_path.len = path->len;
  uim_memscpy(temp_path.path,sizeof(temp_path.path),path->path, 2*(path->len));
  UIMDRV_MSG_HIGH_6(uim_ptr->id, "Path length is 0x%x Path 0x%X 0x%X 0x%X 0x%X 0x%X",
  	                temp_path.len,
                        temp_path.path[0], 
		        temp_path.path[1],
                        temp_path.path[2],
                        temp_path.path[3],
                        temp_path.path[4]);
  
  return;
} /* uim_print_curr_path_ext */



/*===========================================================================
FUNCTION UIM_IS_TELECOM_AID

DESCRIPTION
 This function is used to determine apps based on AID AID could belong to
 TELECOM as well as NON-TELECOM.For NON-TELECOM app,GET RESPONSE will not be
 issued upon receipt of sw1 as 0x62 and sw2 as 0x83

DEPENDENCIES
  None

RETURN VALUE
  TRUE   if AID is TELECOM or non-ADF type file is present, also default return value.
  FALSE  if AID is NON-TELECOM.

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_is_telecom_aid
(
  uim_aid_type aid
)
{
  byte telecom_aid[UIM_TELECOM_AID_NUM][UIM_AID_RID_LEN] =
  {
    /**
    * 'A000000087' for the 3GPP
    * 'A000000343' for the 3GPP2.
    */
    {0xA0, 0x00, 0x00, 0x00, 0x87},
    {0xA0, 0x00, 0x00, 0x03, 0x43}
  };
  uint8  i = 0;

  /* Input param check */

  if (aid.aid_length < UIM_AID_RID_LEN || aid.aid_length > UIM_AID_MAX_NAME_LENGTH )
  {
    return FALSE;
  }

  for(i = 0; i < UIM_TELECOM_AID_NUM; i++ )
  {
    if(0 == memcmp(aid.data,telecom_aid[i],(UIM_AID_RID_LEN )))
    {
      return TRUE;
    }
  }
  UIM_MSG_MED_0("NON-TELECOM AID found");
  return FALSE;
}/* uim_is_telecom_aid */


/*===========================================================================
FUNCTION UIM_IS_ARAM_AID

DESCRIPTION
 This function is used to determine is the AID is an ARA-M app AID

DEPENDENCIES
  None

RETURN VALUE
  TRUE   if AID is ARA-M AID.
  FALSE  if AID is not ARA-M AID.

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_is_aram_aid
(
  uim_aid_type aid
)
{
  byte aram_aid[9] = {0xA0, 0x00, 0x00, 0x01, 0x51, 0x41, 0x43, 0x4C, 0x00};

  if( (0 != aid.aid_length) &&
      (memcmp(aid.data, aram_aid, aid.aid_length) == 0) )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}/* uim_is_aram_aid */


/*===========================================================================
FUNCTION UIM_UTIL_MMGSDI_NOTIFY_CARD_ERROR

DESCRIPTION
 This function is a wrapper function to notify error to mmgsdi

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_util_mmgsdi_notify_card_error
(
  uim_instance_global_type *uim_ptr,
  uim_status_type error_condition,
  uim_slot_type   slot,
  boolean         temporary_error
)
{
  uint8 i = 0;
  mmgsdi_return_enum_type  notification_status = MMGSDI_ERROR;

  for( i=0; i < UIM_MAX_NOTIFICATION_RETRIES ; i++)
  {
    notification_status = uim_interface_mmgsdi_notify_card_error( error_condition, slot, temporary_error );
    if(notification_status == MMGSDI_SUCCESS)
    {
      break;
    }
#ifndef TEST_FRAMEWORK
    uim_timed_sleep(UIM_DELAY_BEFORE_RETRYING_ERROR_NOTIFICATION,
                    uim_ptr,
                    UIM_ALL_ZERO_SIG);
#endif /* TEST_FRAMEWORK */
  }
}/* uim_is_telecom_aid */


/*===========================================================================
FUNCTION UIM_UTIL_GET_EF_FILE_SIZE

DESCRIPTION
 This function is used to retrieve the file size from FCP parameter

DEPENDENCIES
  None

RETURN VALUE
  File size of EF

SIDE EFFECTS
  None

===========================================================================*/
unsigned int uim_util_get_ef_file_size
(
  uim_instance_global_type *uim_ptr,
  uim_rsp_buf_type      *rsp_ptr
)
{
  
  uint16 i = 0; /* index variable */
  uint16 j = 0; /* Temp storage for length */
  uint16 k = 0; /* Innerloop variable for proprietary information */
  
  byte const *data     = rsp_ptr->rsp.data;
  word num_bytes       = rsp_ptr->cmd_rsp_size;
    
  if (num_bytes > UIM_MAX_CHARS)
  {
    return ((unsigned int)0);
  }

  if(UIM_ICC  == uim_ptr->command.cmd_ptr->hdr.protocol)
  {
    return ((unsigned int)FLIPW(rsp_ptr->rsp.ef.file_size));
  }
  
  /* Process all the bytes in FCP */
  while (i < num_bytes)
  {
    if (data == NULL)
    {
      UIMDRV_MSG_LOW_0(uim_ptr->id,"uim_get_ef_file_size data is NULL");
      return ((unsigned int)0);
    }
    switch (data[i++])
    {
      case UIM_FCP_TEMPLATE_TAG:
        i++; /* Move past the length field */
        break;
  
      case UIM_FILE_DESCRIPTOR_TAG:
      if (i >= num_bytes)
      {
        return((unsigned int)0);
      }
      /* store the length to advance */
      j = data[i++];
      if ((i + j) >= num_bytes)
      {
        return((unsigned int)0);
      }
      switch (data[i] & UIM_UICC_EF_STRUCT_MASK)
      {
        case UIM_UICC_TRANSPARENT_EF:
        case UIM_UICC_LINEAR_EF:
        case UIM_UICC_CYCLIC_EF:
        default:
          break;
      }
      /* Skip j bytes */
      i+=j;
      break;
  
      case UIM_PROPRIETARY_TAG:
      if (i >= num_bytes)
      {
        return ((unsigned int)0);
      }
      k = i + data[i];
      i++;
      while(i < k)
      {
        if (i >= num_bytes)
        {
          return ((unsigned int)0);
        }
        switch(data[i++])
        {
          case UIM_CHARACTERISTICS_TAG:
          /* check here if i is a good index for "j" and the file_char */
          if ((i + 1) >= num_bytes)
          {
            return ((unsigned int)0);
          }
          j = data[i++];
          i+=j;
          break;
  
          case UIM_SUPPORTED_CMD_TAG:
          /* check here if i is a good index for "j" and supported_cmd_present */
            if ((i + 1) >= num_bytes)
            {
              return ((unsigned int)0);
            }
  
            j = data[i++];
            i+=j;
            break;
  
            default:
            if (i >= num_bytes)
            {
              return ((unsigned int)0);
            }
            j = data[i++];
            i+=j;
            break;
          }
        }
        break;

      case UIM_FILE_SIZE_TAG:
        /* j is at length offset  */
        if ((i + 1) >= num_bytes)
        {
          return((unsigned int)0);
        }
        j = data[i++];
        /* if Length is of more that one byte long */
        if (j > 1 )
        {
          return((int)FLIPW(data + i));
        }

        /* Length is of one byte long */
        return(data[i]);
      
      case UIM_FILE_ID_TAG:
      case UIM_DF_NAME_TAG:
      case UIM_TOTAL_FILE_SIZE_TAG:
      case UIM_SECURITY_ATTR_TAG1:
      case UIM_SECURITY_ATTR_TAG2:
      case UIM_SECURITY_ATTR_TAG3:
      case UIM_SECURITY_ATTR_TAG4:
      case UIM_LIFE_CYCLE_STATUS_TAG:

      case UIM_SFI_TAG:
       /* Ignore the tags that are not needed */
      default:
      if (i >= num_bytes)
      {
        return ((unsigned int)0);
      } /* Get the length */
      j = data[i++];
      /* Skip j bytes */
      i+=j;
      break;
    }
  }
  return ((unsigned int)0);
} /* uim_util_get_ef_file_size */


/*===========================================================================
FUNCTION UIM_PROCESS_STREAM_ISO7816_APDU_F_RESPONSE

DESCRIPTION
 This function process the response of stream_iso7816_apdu

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void uim_process_stream_iso7816_apdu_f_response
(
  uim_rsp_buf_type const   *rsp_ptr,
  uim_instance_global_type *uim_ptr
)
{
  /* Report back the status as success */
  if (uim_ptr->command.cmd_ptr->hdr.rpt_function != NULL)
  {
    uim_ptr->command.rpt_buf.rpt_type = UIM_STREAM_ISO7816_APDU_R;

    /* Fill in necessary fields of command block */
    uim_ptr->command.rpt_buf.rpt_hdr.task_ptr = NULL;

    uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.num_data_bytes = 0;

    /* As per 10221, warning status words should be sent as final status words
       So, Updating intermediate status words into final status words */
    if(uim_ptr->card_cmd.get_resp_sw1 == SW1_WARNINGS1 || 
       uim_ptr->card_cmd.get_resp_sw1 == SW1_WARNINGS2)
    {
      uim_ptr->command.rpt_buf.sw1 = uim_ptr->card_cmd.get_resp_sw1;
      uim_ptr->command.rpt_buf.sw2 = uim_ptr->card_cmd.get_resp_sw2;
    }

    if (uim_ptr->command.rd_wr_info.num_bytes_rsp > 0)
    {
      /* copy the data into report buffer */
      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.num_data_bytes =
                                        uim_ptr->command.rd_wr_info.num_bytes_rsp;

      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.data_ptr =
                        uim_ptr->command.rd_wr_info.alloc_mem;

      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw1 = uim_ptr->card_cmd.get_resp_sw1;
      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw2 = uim_ptr->card_cmd.get_resp_sw2;
    }
    else if (rsp_ptr->cmd_rsp_size > 0)
    {
      /* copy the data into report buffer */
      /* Case, when default channel is selected */
      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.num_data_bytes =
                                                   rsp_ptr->cmd_rsp_size;
      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.data_ptr =
                    uim_malloc(rsp_ptr->cmd_rsp_size);
      if (uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.data_ptr != NULL)
      {
        uim_memscpy(uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.data_ptr,
                    rsp_ptr->cmd_rsp_size,
                     rsp_ptr->rsp.data,
                     rsp_ptr->cmd_rsp_size);
      }
      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw1 = uim_ptr->card_cmd.get_resp_sw1;
      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw2 = uim_ptr->card_cmd.get_resp_sw2;
    }
    else
    {
      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw1 = (uim_sw1_type) 0;
      uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw2 = (uim_sw2_type) 0;
    }
    /* check If Truncation of response data with Le is enabled */
    if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_LE_SUPPORT_FOR_7816_STREAM_APDU,
                                  uim_ptr) == TRUE)
    {
      if(uim_ptr->card_cmd.stream_apdu_rpt_data_length < uim_ptr->command.rd_wr_info.num_bytes_rsp)
      {
        uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.num_data_bytes = uim_ptr->card_cmd.stream_apdu_rpt_data_length;
        uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw2 = (uim_sw2_type)uim_ptr->card_cmd.stream_apdu_rpt_data_length;
        if( (uim_ptr->card_cmd.stream_apdu_rpt_data_length == 0) ||
            (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes == UIM_CASE2_7816_APDU_SIZE) )
        {
           uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw1 = (uim_sw1_type) 0;
           uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw2 = (uim_sw2_type) 0;
        }
      }
    }
    
    /* Fill the channel state if the streamed APDU is manage channel command */
    if(uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_INSTRN_OFFSET] == MANAGE_CHANNEL && 
       uim_ptr->command.rpt_buf.rpt_status == UIM_PASS)
    {
      if(uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_P1_OFFSET] == UIM_MANAGE_CHANNEL_P1_OPEN &&
         uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.data_ptr != NULL)
      {
        uim_ptr->state.is_ch_open[uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.data_ptr[0]] = TRUE;
      }
      if(uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_P1_OFFSET] == UIM_MANAGE_CHANNEL_P1_CLOSE)
      {
        uim_ptr->state.is_ch_open[uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_P2_OFFSET]]       = FALSE;
        uim_ptr->state.ch_apdu_behavior[uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_P2_OFFSET]] = UIM_APDU_BEHAVIOR_FOR_TELECOM_APP;
      }
    }


    UIMDRV_MSG_HIGH_3(uim_ptr->id,"Total Response data length 0x%x rpt_data_len 0x%x num_bytes_rsp",
                        uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.num_data_bytes,
                        uim_ptr->card_cmd.stream_apdu_rpt_data_length,
                        uim_ptr->command.rd_wr_info.num_bytes_rsp);

    /* Report to requesting task */
    (*(uim_ptr->command.cmd_ptr->hdr.rpt_function))( &uim_ptr->command.rpt_buf );
    if (uim_ptr->command.rd_wr_info.num_bytes_rsp > 0)
    {
      /* Release the allocated memory for stream apdu commands */
      uim_free_reset_rd_wr_info(uim_ptr);
    }
    else if (rsp_ptr->cmd_rsp_size > 0)
    {
      /* Case, when default channel is selected */
      if (uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.data_ptr != NULL)
      {
        UIM_FREE(uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.data_ptr);
      }
    }
    uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.data_ptr = NULL;
  } /* end if - is the report function defined. */
}/*uim_process_stream_iso7816_apdu_f_response*/

/*===========================================================================
FUNCTION UIM_PROCESS_VOLTAGE_MISMATCH

DESCRIPTION
 This function is used to process voltage mismatch

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void uim_process_voltage_mismatch
(
  rex_sigs_type            *i_mask,    /* Mask of signals interested in */
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      status
)
{
  if((!uim_ptr->flag.static_buffer_used) && (uim_ptr->command.cmd_ptr != NULL)
     && (uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_F) && (status == UIM_VOLTAGE_MISMATCH))
  {
     UIMDRV_MSG_MED_0(uim_ptr->id,"Processing response for the reset command");
     if(UIM_CARD_SAM  == uim_ptr->card_mode)
     {
       uim_sam_process_response(&uim_ptr->command.rsp_buf, uim_ptr);
     }
     else
     {
       uim_process_generic_response(&uim_ptr->command.rsp_buf, uim_ptr);
     }
  }
  /* set the status to ERROR */
  uim_ptr->state.status = UIM_ERR_S;

  /* Clear out any votes to keep UIM powered on */
  uim_ptr->state.ctl = UIM_NONE;
  uim_ptr->flag.powerdown = TRUE;

  /* flush all the commands in the command queue */
  uim_flush_command(UIM_FLUSH_ALL, uim_ptr);

  /* Whenever we receive a command, send an error report as the
  SIM is to be powered down */
  *i_mask |= UIM_CMD_Q_SIG;

  if(UIM_CARD_TELECOM  == uim_ptr->card_mode)
  {
    if((uim_ptr->extended_recovery.extended_recovery_enabled == TRUE) &&
	   (status == UIM_ICCID_MISMATCH))
    {
      uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TIMER_START);
    }
  }

  /* Notify the higher layers that UIM is declared faulty */
  if(UIM_MAX_INSTANCES > uim_ptr->id)
  {
    uim_notify_error(UIM_VOLTAGE_MISMATCH_S, uim_ptr);
    uim_sam_reset_card_mode(i_mask,uim_ptr);
  }

  /* reset the directories and send a power down to UIM */
  uim_reset_dir(uim_ptr);
  uim_power_down(uim_ptr);

  /* Reset the static command count */
  uim_ptr->command.static_cmd_buf.hdr.cmd_count = 1;

  /* reset the sw1 warning flag to FALSE without the need to reselect MF */
  uim_ptr->flag.sw1_warnings1_normal_end_occured = FALSE;

  /* reset the static buffer used flag */
  uim_ptr->flag.static_buffer_used = FALSE;

  /* Reset the command in progress flag */
  uim_ptr->flag.command_in_progress = FALSE;

  /* Clear the global command pointer as we are done with this command. */
  uim_ptr->command.cmd_ptr = NULL;
}/*uim_process_voltage_mismatch*/


/*===========================================================================
FUNCTION UIM_PROCESS_CONVENTION_MISMATCH

DESCRIPTION
 This function is used to process convention mismatch

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void uim_process_convention_mismatch
(
  rex_sigs_type            *i_mask,    /* Mask of signals interested in */
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      status
)
{
  (void)status;
  if(!uim_ptr->flag.static_buffer_used)
  {
    if (uim_ptr->command.cmd_ptr != NULL)
    {
      UIMDRV_MSG_MED_1(uim_ptr->id,
                          "command 0x%x",
                          uim_ptr->command.cmd_ptr->hdr.command);
      if(uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_F)
      {
        uim_insert_cmd_at_head_of_queue(uim_ptr->command.cmd_ptr, uim_ptr);
      }
    }
    *i_mask |= UIM_CMD_Q_SIG;
  }
  else
  {
    uim_reset_uim (i_mask, uim_ptr->flag.me_powerup,uim_ptr);
  }
  /* reset the sw1 warning flag to FALSE without the need to reselect MF */
  uim_ptr->flag.sw1_warnings1_normal_end_occured = FALSE;
}/*uim_process_convention_mismatch*/


/**
 * This function is used to compute TCK byte
 *
 * @param uim_ptr         Pointer to the UIM global instance
 * @param atr_bytes_len   atr_bytes_len
 * @param atr_bytes       Pointer to atr_bytes
 * 
 * @return  uint8         Return the computed TCK bytes
 *
 */
uint8 uim_util_compute_tck_bytes
(
  uint8 atr_bytes_len,
  uint8 *atr_bytes_ptr
)
{
  uint8 tmp_TCK = 0x00;
  uint8 index   = 0x00;
  
  if((atr_bytes_len < 2) || (atr_bytes_ptr == NULL))
  {
    return (tmp_TCK);
  }

  /* -1 is to exclude the TCK byte of ATR
    TCK is an XOR of all bytes
    starting from T0 and up to the end of the ATR.*/
  index = 1;
  while(index < (atr_bytes_len -1))
  {
    tmp_TCK ^= atr_bytes_ptr[index];
    index += 1;
  }
  return(tmp_TCK);
}/* uim_util_compute_tck_bytes */


/**
 * This function is used to parse ATR bytes and store them into global
 *
 * @param uim_ptr           Pointer to the UIM global instance
 * @param atr_len           Length of ATR bytes
 * @param atr_bytes_ptr     ATR Bytes received from UICC/ICC
 * 
 * @return  boolean          TRUE if decoded successfully else FALSE    
 *
 */
boolean uim_util_atr_parse_and_populate
(
  uim_instance_global_type *uim_ptr,
  uint8                     atr_len, 
  uint8                    *atr_bytes_ptr
)
{
  uint8 tmp_interface_character = 0;
  uint8 tmp_historical_bytes    = 0;
  uint8 index                   = 1;
  boolean tck_byte_conditional  = FALSE;
  
  if((atr_len < 2)          ||
     (NULL == uim_ptr) ||
     (NULL == atr_bytes_ptr))
  {
    return (FALSE);
  }
  
  memset(&(uim_ptr->decoded_atr), 0, sizeof(uim_ptr->decoded_atr));
  uim_ptr->decoded_atr.TS = atr_bytes_ptr[0];
  uim_ptr->decoded_atr.T0 = atr_bytes_ptr[1];
  /* Extracting Interface character
   ,----,----,----,----,----,----,----,----,
   | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 |
   '----'----'----'----'----'----'----'----'
   :<------- Y1 ------>:<-------- K ------>:

    Y1 : indicator for the presence of the interface characters
    TA1 is transmitted when b5=1
    TB1 is transmitted when b6=1
    TC1 is transmitted when b7=1
    TD1 is transmitted when b8=1
  */
  if(uim_ptr->decoded_atr.T0 & 0xF0)
  {
    uim_ptr->decoded_atr.is_interface_byte_present = TRUE;
  }

  tmp_historical_bytes = atr_bytes_ptr[index] & UIM_K_FIELD;
  if (tmp_historical_bytes)
  {
    uim_ptr->decoded_atr.is_historical_bytes_present = TRUE;
  }

  /* Parsing of the interface byte, index is initialize with 1*/
  do
  {
    if(index > atr_len)
    {
      return (FALSE);
    }
    tmp_interface_character = (atr_bytes_ptr[index] & 0xF0) >> UIM_Y_FIELD_SHIFT;
    if(tmp_interface_character & 0x1)  /*  TAi Parsing */
    {
      tmp_interface_character = tmp_interface_character & 0x0E;
      index += 1;
      if(index >= atr_len || uim_ptr->decoded_atr.interface_byte.TAx_count >= UIM_ATR_INTERFACE_BYTES_MAX_SIZE)
      {
        return (FALSE);
      }
      uim_ptr->decoded_atr.interface_byte.TAx[uim_ptr->decoded_atr.interface_byte.TAx_count].TA 
                 = atr_bytes_ptr[index];    
      uim_ptr->decoded_atr.interface_byte.TAx[
        uim_ptr->decoded_atr.interface_byte.TAx_count].is_TA_byte_present = TRUE;
    }
  
    if(tmp_interface_character & 0x02) /* TBi Parsing */
    { 
      tmp_interface_character = tmp_interface_character & 0x0D;
      index += 1;
      if(index >= atr_len || uim_ptr->decoded_atr.interface_byte.TBx_count >= UIM_ATR_INTERFACE_BYTES_MAX_SIZE)
      {
        return (FALSE);
      }
      uim_ptr->decoded_atr.interface_byte.TBx[
        uim_ptr->decoded_atr.interface_byte.TBx_count].is_TB_byte_present = TRUE;
      uim_ptr->decoded_atr.interface_byte.TBx[
        uim_ptr->decoded_atr.interface_byte.TBx_count].TB 
                  = atr_bytes_ptr[index];
    }

    if(tmp_interface_character & 0x04) /* TCi Parsing */
    {
      tmp_interface_character = tmp_interface_character & 0x0B;
      index += 1;
      if(index >= atr_len || uim_ptr->decoded_atr.interface_byte.TCx_count >= UIM_ATR_INTERFACE_BYTES_MAX_SIZE)
      {
        return (FALSE);
      }
      uim_ptr->decoded_atr.interface_byte.TCx[
        uim_ptr->decoded_atr.interface_byte.TCx_count].is_TC_byte_present = TRUE;
      uim_ptr->decoded_atr.interface_byte.TCx[
        uim_ptr->decoded_atr.interface_byte.TCx_count].TC 
                   = atr_bytes_ptr[index];
    }
  
    if(tmp_interface_character & 0x08)  /* TDi Parsing */
    {
      tmp_interface_character = tmp_interface_character & 0x07;
      index += 1;
      if(index >= atr_len || uim_ptr->decoded_atr.interface_byte.TDx_count >= UIM_ATR_INTERFACE_BYTES_MAX_SIZE)
      {
        return (FALSE);
      }
      uim_ptr->decoded_atr.interface_byte.TDx[
        uim_ptr->decoded_atr.interface_byte.TDx_count].is_TD_byte_present = TRUE;
      uim_ptr->decoded_atr.interface_byte.TDx[
        uim_ptr->decoded_atr.interface_byte.TDx_count].TD 
                  = atr_bytes_ptr[index] & 0x0F;
      uim_ptr->decoded_atr.interface_byte.TDx_count += 1;
      /* If T=0 and T=15 are present and in all the other cases, 
          TCK shall be present. */
      tck_byte_conditional = TRUE;
	  /* Read the Next Interface byte */
      tmp_interface_character = (atr_bytes_ptr[index] & 0xF0) >> UIM_Y_FIELD_SHIFT;	  
    }

    uim_ptr->decoded_atr.interface_byte.TAx_count += 1;
    uim_ptr->decoded_atr.interface_byte.TBx_count += 1;
    uim_ptr->decoded_atr.interface_byte.TCx_count += 1;
    /* Repeat while TD bit is set */
  }while(tmp_interface_character & 0x0F);  

  if(tmp_historical_bytes && (atr_len > index + tmp_historical_bytes))
  {
    uim_ptr->decoded_atr.historical_bytes.Kx_count = tmp_historical_bytes;
    uim_memscpy((void *)&uim_ptr->decoded_atr.historical_bytes.Kx[0],
                 UIM_MAX_HIST_CHARS, 
                 &atr_bytes_ptr[index + 1 ],
                 tmp_historical_bytes);
  }

  if (tck_byte_conditional && (atr_len > index + tmp_historical_bytes + 1))
  {
    /* If only T=0 is indicated, possibly by default, then TCK shall be absent. 
       If T=0 and T=15 are present and in all the other cases, TCK shall be present. 
       When TCK is present, exclusive-oring all the bytes T0 to TCK inclusive
       shall give '00'. Any other value is invalid.
    */
    uim_ptr->decoded_atr.TCK = atr_bytes_ptr[index + tmp_historical_bytes + 1];
    if(uim_ptr->decoded_atr.TCK == uim_util_compute_tck_bytes(atr_len,
                                                              atr_bytes_ptr))
    {
      uim_ptr->decoded_atr.is_tck_byte_valid = TRUE;
    }
    else
    {
      uim_ptr->decoded_atr.is_tck_byte_valid = FALSE;
      return FALSE;
    }
  }
  return (TRUE);
}/* uim_util_atr_parse_and_populate */


/**
 * This function is used to validate the format bytes
 *
 * @param  uim_ptr           Pointer to the UIM global instance
 * 
 * @return boolean           TRUE if format byte is correct else FALSE    
 *
 */
boolean uim_util_is_format_byte_valid_in_atr
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr == NULL)
  {
    return (FALSE);
  }

  if((uim_ptr->decoded_atr.TS != UIM_ATR_TS_DIRECT_CONV) &&
     (uim_ptr->decoded_atr.TS != UIM_ATR_TS_INVERSE_CONV))
  {
    return FALSE;
  }

  return TRUE;
}/* uim_util_is_format_byte_valid */


/**
 * This function is used to validate the protocol in ATR
 *
 * @param uim_ptr      Pointer to the UIM global instance
 * 
 * @return boolean     TRUE if TD1 < TD2 and so on else FALSE
 *
 */
boolean uim_is_protocol_valid_in_atr
(
  uim_instance_global_type *uim_ptr
)
{
  uint8   index      = 0;

  if(uim_ptr == NULL)
  {
    return (FALSE);
  }
  /*  T=15 is invalid in TD1 */
  if( uim_ptr->decoded_atr.interface_byte.TDx[IFB_1ST].is_TD_byte_present &&
     ((uim_ptr->decoded_atr.interface_byte.TDx[IFB_1ST].TD & UIM_T_FIELD) == UIM_T_FIELD))
  {
    return FALSE;
  }
   
  /* Now check for TD2 to TDj where j >=2 and shall be ascending order */
  for(index = 0; 
      (uim_ptr->decoded_atr.interface_byte.TDx[index].is_TD_byte_present &&
       uim_ptr->decoded_atr.interface_byte.TDx[index +1].is_TD_byte_present);
      index++)
  {
    /* If TD1, TD2 and so on are present, the encoded types T shall be in ascending
            numerical order. If present, T=0
            shall be first, T=15 shall be last. T=15 is invalid in TD1.
       */
    if ((uim_ptr->decoded_atr.interface_byte.TDx[index].TD & UIM_T_FIELD) >
        (uim_ptr->decoded_atr.interface_byte.TDx[index + 1].TD & UIM_T_FIELD))
    {
      return FALSE;
    }  
  }
  return TRUE;
}/* uim_is_protocol_valid_in_atr */


/**
 * This function is used to retrieve len of historical byte present in ATR
 *
 * @param uim_ptr          Pointer to the UIM global instance
 * 
 * @return  uint8          Available Historical bytes
 *
 */
uint8 uim_util_get_historical_bytes_len_from_atr
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr != NULL && uim_ptr->decoded_atr.is_historical_bytes_present ) 
  {
    return(uim_ptr->decoded_atr.historical_bytes.Kx_count);
  }
  return (uint8)0;
}/* uim_util_get_historical_bytes_len_from_atr */


/**
 * This function is used to copy historical bytes in actual argument
 *
 * @param uim_ptr           Pointer to the UIM global instance
 * @param hist_bytes_ptr    Historical bytes are to be stored
 * 
 * @return  boolean         TRUE if historical bytes are present else FALSE    
 *
 */
boolean uim_util_get_historical_bytes_from_atr
(
  uim_instance_global_type *uim_ptr, 
  uint8                    *hist_bytes_ptr
)
{

  if(uim_ptr != NULL  && hist_bytes_ptr != NULL &&
     uim_ptr->decoded_atr.is_historical_bytes_present)
  {
    uim_memscpy((void *)hist_bytes_ptr, 
      (size_t)UIM_MAX_HIST_CHARS,
      (const void *)&uim_ptr->decoded_atr.historical_bytes.Kx[0],
      (size_t)uim_ptr->decoded_atr.historical_bytes.Kx_count);
    return (TRUE);
  }
  
  return(FALSE);
}/* uim_util_get_historical_bytes_from_atr */


/**
 * This function is used to check if MF is present in ATR or not
 *
 * @param uim_ptr           Pointer to the UIM global instance
 * 
 * @return  boolean         TRUE if MF is present in ATR else FALSE    
 *
 */
boolean uim_util_is_mf_present_indicated_in_atr
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr == NULL)
  {
    return TRUE;
  }
  
  /* As per ISO 7816-4,If the first historical byte is set to '00', then
     the remaining historical bytes consist of optional consecutive
     COMPACT-TLV data objects followed by a mandatory status indicator
     (the last three bytes, not in TLV).*/
  if((uim_ptr->atr.hist_char_buf.data[0] == 0x80 && 
      uim_ptr->atr.hist_char_buf.length >= 3)    ||
     (uim_ptr->atr.hist_char_buf.data[0] == 0x00 &&
      uim_ptr->atr.hist_char_buf.length >= 6))	   
  {	
    uint8 i = 1;

    while(i < uim_ptr->atr.hist_char_buf.length - 1  && i < UIM_MAX_HIST_CHARS - 1)
    {
      if((uim_ptr->atr.hist_char_buf.data[i] & 0xF0) == 0x30)
      {						  
        if((uim_ptr->atr.hist_char_buf.data[i+1] & 0x01) == 0x01)
        { 						
          return FALSE;
        }
        return TRUE;
      }
      i += (uim_ptr->atr.hist_char_buf.data[i] & 0x0F) + 1;
    }  	  
  }  
	  
  return TRUE;
}/* uim_util_is_mf_present_indicated_in_atr */


/**
* This function is used to populate Fi and Di parameters
*
* @param uim_ptr                      Pointer to the UIM global instance
* @param voltage_class_supported_ptr  Pointer to voltage_class_supported_ptr
*
* @return  boolean                    Return TRUE if volatge is supported else FALSE
*
*/
boolean uim_util_get_voltage_support_from_atr
(
  uim_instance_global_type      *uim_ptr,
  uim_uicc_voltage_class_type   *voltage_class_supported_ptr
)
{ 
  boolean ret_status = FALSE;
  uint8   ta_data     = 0x00;
  
  if((NULL == uim_ptr) || (NULL == voltage_class_supported_ptr))
  {
    return (FALSE);
  }

  /* The TA3 with TD2 with T=15 */
  if(uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].is_TD_byte_present &&
    (UIM_T_FIELD == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].TD & UIM_T_FIELD)) &&
     uim_ptr->decoded_atr.interface_byte.TAx[IFB_3RD].is_TA_byte_present)
  {
    ta_data |= (uim_ptr->decoded_atr.interface_byte.TAx[IFB_3RD].TA) & 0x3F;
    ret_status = TRUE;
  }

  /* The TA4 With TD3 with T=15 */
  if(uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].is_TD_byte_present &&
    (UIM_T_FIELD == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].TD & UIM_T_FIELD)) &&
     uim_ptr->decoded_atr.interface_byte.TAx[IFB_4TH].is_TA_byte_present)
  {
    ta_data |= (uim_ptr->decoded_atr.interface_byte.TAx[IFB_4TH].TA) & 0x3F;
    ret_status = TRUE;
  }

  if (ret_status)
  {
    /*Bits 6 and 1  -
      0x01 - Class A
      0x02 - Class B
      0x04 - Class C
      Other - RFU */
    voltage_class_supported_ptr->only_class_c_supported = FALSE;
    voltage_class_supported_ptr->only_class_b_supported = FALSE;
    if( UIM_UI_1_8_UIM_UICC == (ta_data & UIM_UI_1_8_UIM_UICC))
    {
      voltage_class_supported_ptr->only_class_c_supported = TRUE;
    }
    if( UIM_UI_3V_UIM_UICC == (ta_data & UIM_UI_3V_UIM_UICC))
    {
      voltage_class_supported_ptr->only_class_b_supported = TRUE;
    }
  }
  return ret_status;
}/* uim_util_get_voltage_support_from_atr */


/**
* This function is used to populate clock indicator
*
* @param uim_ptr              Pointer to the UIM global instance
* @param clk_cntrl_ptr        Pointer to clk_cntrl_ptr
*
* @return  boolean            FALSE if validation is failed else TRUE
*
*/
boolean uim_util_get_clock_stop_indicator_from_atr
(
  uim_instance_global_type    *uim_ptr,
  uim_clock_stop_cntrl_type   *clk_cntrl_ptr
)
{
  uint8 clk_stop_indicator = 0x00; 
  boolean ret_status = FALSE;

  if( NULL == uim_ptr || NULL == clk_cntrl_ptr)
  {
    return (FALSE);
  }

  /* The TA3 with TD2 with T=15 */
  if(uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].is_TD_byte_present &&
    (UIM_T_FIELD == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].TD & UIM_T_FIELD)) &&
     uim_ptr->decoded_atr.interface_byte.TAx[IFB_3RD].is_TA_byte_present)
  {
    clk_stop_indicator = (uim_ptr->decoded_atr.interface_byte.TAx[IFB_3RD].TA >> 0x6) & 0x03;
    ret_status = TRUE;
  }

  /* The TA4 With TD3 with T=15 */
  if(uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].is_TD_byte_present &&
    (UIM_T_FIELD == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].TD & UIM_T_FIELD)) &&
     uim_ptr->decoded_atr.interface_byte.TAx[IFB_4TH].is_TA_byte_present)
  {
    clk_stop_indicator = (uim_ptr->decoded_atr.interface_byte.TAx[IFB_4TH].TA >> 0x6) & 0x03;
    ret_status = TRUE;
  }

  if (ret_status)
  {
    /*Bits 8 and 7  -
      00 - X Clock not supported
      01 - State L
      10 - State H
      11 - No Preference */

    if(clk_stop_indicator == 0x00)
    {
      *clk_cntrl_ptr = UIM_CLOCK_STOP_NOT_ALLOWED;
    }
    else if((clk_stop_indicator == 0x01) ||
            (clk_stop_indicator == 0x03))
    {
      *clk_cntrl_ptr = UIM_CLOCK_STOP_LOW;
    }
    else
    {
      *clk_cntrl_ptr = UIM_CLOCK_STOP_HIGH;
    }
  }
  return ret_status;
}/* uim_util_get_clock_stop_indicator_from_atr */


/**
* This function is used to check whether T=0 protocol is supported or not
*
* @param uim_ptr           Pointer to the UIM global instance
*
* @return  boolean         FALSE if it supports else TRUE
*
*/
boolean uim_util_is_T0_supported_in_atr
(
  uim_instance_global_type  *uim_ptr
)
{
  if(NULL == uim_ptr)
  {
    return FALSE;
  }

  if(!uim_ptr->decoded_atr.interface_byte.TDx[IFB_1ST].is_TD_byte_present)
  {
    return TRUE;
  }
  
  if(UIM_T_0_PROTOCOL == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_1ST].TD & UIM_T_FIELD))
  { 
    return TRUE;
  }
  return FALSE;
}/* uim_util_is_T0_supported_in_atr */


/**
* This function is used to check whether T=1 protocol is supported or not.
*
* @param uim_ptr         Pointer to the UIM global instance
*
* @return  boolean       FALSE if it is not supported else TRUE
*
*/
#if defined( FEATURE_UIM_T_1_SUPPORT )
boolean uim_util_is_T1_supported_in_atr
(
  uim_instance_global_type *uim_ptr
)
{
  if(NULL == uim_ptr)
  {
    return (FALSE);
  }
  
  /* Currently supporting TD1\TD2 and TD3 interface for T=1 Protocol */
  if( uim_ptr->decoded_atr.interface_byte.TDx[IFB_1ST].is_TD_byte_present && 
      UIM_T_1_PROTOCOL == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_1ST].TD & 
                          UIM_T_FIELD))
  { 
    return TRUE;
  }

  if( uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].is_TD_byte_present && 
      UIM_T_1_PROTOCOL == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].TD & 
                          UIM_T_FIELD))
  { 
    return TRUE;
  }

  if( uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].is_TD_byte_present && 
      UIM_T_1_PROTOCOL == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].TD & 
                          UIM_T_FIELD))
  { 
    return TRUE;
  }
  return FALSE;
}/* uim_util_is_T1_supported_in_atr */


/**
* This function is used to retrieved IFSC value from TA3 byte of ATR
*
* @param  uim_ptr        uim global instance specific ptr
* @param  ifsc_ptr       IFSC value
*
* @return  boolean       TRUE if IFSC is present else FALSE
*                         
*/
boolean uim_util_is_IFSC_present_in_atr
(
  uim_instance_global_type *uim_ptr,
  uint8                    *ifsc_ptr
)
{
  if((NULL == uim_ptr) || (NULL == ifsc_ptr))
  {
    return (FALSE);
  }

  if( uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].is_TD_byte_present && 
      UIM_T_1_PROTOCOL == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].TD & UIM_T_FIELD) &&
      uim_ptr->decoded_atr.interface_byte.TAx[IFB_3RD].is_TA_byte_present)
  {
    *ifsc_ptr = uim_ptr->decoded_atr.interface_byte.TAx[IFB_3RD].TA;
     return TRUE;
  }
  return FALSE;
}/* uim_util_is_IFSC_present_in_atr */


/**
* This function is used to retrieved CWI & BWI value from TA3 byte of ATR
*
* @param  uim_ptr        uim global instance specific ptr
* @param  cwi_ptr         CWI value
* @param  bwi_ptr         BWI value
*
* @return  boolean        TRUE if BWI is present else FALSE
*                         
*/
boolean uim_util_is_BWI_CWI_present_in_atr
(
  uim_instance_global_type *uim_ptr,
  uint8                    *cwi_ptr,
  uint8                    *bwi_ptr
)
{
  if((NULL == uim_ptr)||(NULL == cwi_ptr)||(NULL == bwi_ptr))
  {
    return (FALSE);
  }

  if( uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].is_TD_byte_present && 
      UIM_T_1_PROTOCOL == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].TD & UIM_T_FIELD) &&
      uim_ptr->decoded_atr.interface_byte.TBx[IFB_3RD].is_TB_byte_present)
  {
    *cwi_ptr = uim_ptr->decoded_atr.interface_byte.TBx[IFB_3RD].TB & UIM_T_1_CWI_FIELD;
    *bwi_ptr = ((uim_ptr->decoded_atr.interface_byte.TBx[IFB_3RD].TB & UIM_T_1_BWI_FIELD)
                       >> UIM_T_1_BWI_FIELD_SHIFT);
    return TRUE;
  }
  return FALSE;  
}/* uim_util_is_BWI_CWI_present_in_atr */


/**
* This function is used to retrieved CRC value from TC3 byte of ATR
*
* @param  uim_ptr                  uim global instance specific ptr
* @param  edc_ptr                  CWI value
* @param  epilogue_field_size_ptr  Epilogue value
*
* @return  boolean                 TRUE if EDC is present else FALSE
*                         
*/
boolean uim_util_is_CRC_supported_in_atr
(
  uim_instance_global_type *uim_ptr,
  uint8                    *edc_ptr,
  uint8                    *epilogue_field_size_ptr
)
{
  if((NULL == uim_ptr) || (NULL == edc_ptr) ||(NULL == epilogue_field_size_ptr))
  {
    return (FALSE);
  }

  if( uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].is_TD_byte_present && 
      UIM_T_1_PROTOCOL == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].TD & UIM_T_FIELD) &&
      uim_ptr->decoded_atr.interface_byte.TCx[IFB_3RD].is_TC_byte_present)
  {
    if(UIM_T_1_EDC_FIELD & uim_ptr->decoded_atr.interface_byte.TCx[IFB_3RD].TC)
    {
      *edc_ptr = UIM_T_1_EDC_CRC;
      *epilogue_field_size_ptr = UIM_T_1_CRC_SIZE;
      return TRUE;
    }
  }  
  return FALSE;
}/* uim_util_is_CRC_supported_in_atr */


/**
* This function is used to check whether T=1 is supported or not. If it is 
* supported then it retrieves IFSC, BWI, CWI and CRC
*
* @param *uim_ptr         Pointer to the UIM global instance
*
*/
void uim_util_update_t_1_params_from_atr
(
  uim_instance_global_type  *uim_ptr
)
{ 
  /* Set initial values of send sequence numbers */
  uim_ptr->t1.me_send_sequence_num = UIM_T_1_INIT_SEQ_NUM;
  uim_ptr->t1.card_send_sequence_num = UIM_T_1_INIT_SEQ_NUM;
  uim_ptr->t1.t_1_ifsd = UIM_T_1_DEFAULT_IFS;

  if(uim_util_is_T1_supported_in_atr(uim_ptr))
  {
    if(!uim_util_is_IFSC_present_in_atr(uim_ptr, (uint8 *)&uim_ptr->t1.t_1_ifsc))
    {
      /* Set default values of IFS */
      uim_ptr->t1.t_1_ifsc = UIM_T_1_DEFAULT_IFS;
    }

    if(!uim_util_is_BWI_CWI_present_in_atr(uim_ptr, 
                                   (uint8 *)&uim_ptr->t1.t_1_cwi,
                                   (uint8 *)&uim_ptr->t1.t_1_bwi))
    {
      /* Set the default waiting times indeces for T=1 protocol */
      uim_ptr->t1.t_1_bwi = UIM_T_1_DEFAULT_BWI;
      uim_ptr->t1.t_1_cwi = UIM_T_1_DEFAULT_CWI;
    }

    if(!uim_util_is_CRC_supported_in_atr(uim_ptr, 
                                 (uint8 *)&uim_ptr->t1.t_1_edc,
                                 (uint8 *)&uim_ptr->t1.epilogue_field_size))
    {
      /* Set the default EDC controls */
      uim_ptr->t1.t_1_edc = UIM_T_1_EDC_LRC;
      uim_ptr->t1.epilogue_field_size = UIM_T_1_LRC_SIZE;
    }
  }
}/* uim_util_update_t_1_params_from_atr */
#endif /* FEATURE_UIM_T_1_SUPPORT */


/**
* This function is used to populate specific parameters
*
* @param uim_ptr                Pointer to the UIM global instance
* @param specifi_mode_ptr       Pointer to specifi_mode_ptr
*
* @return  boolean              FALSE if validation is failed else TRUE
*
*/
boolean uim_util_get_specific_mode_info_from_atr
(
  uim_instance_global_type      *uim_ptr,
  uim_global_specific_mode_type *specifi_mode_ptr
)
{
  if((NULL == uim_ptr) || (NULL == specifi_mode_ptr))
  {
    return (FALSE);
  }
  /*extract specific mode params. encoded in TA2 which comes after TD1*/
  if(uim_ptr->decoded_atr.interface_byte.TAx[IFB_2ND].is_TA_byte_present)
  {
    /* Presence of TA2 indicates the UICC is in specific mode */
    specifi_mode_ptr->uim_atr_ta2_byte_present = TRUE;
    
    if(uim_ptr->decoded_atr.interface_byte.TAx[IFB_2ND].TA & UIM_BIT8_CHECK)
    {
      specifi_mode_ptr->uim_atr_b8_bit_ta2 = TRUE; 
    }
    else
    {
      specifi_mode_ptr->uim_atr_b8_bit_ta2 = FALSE;
    }

    if(uim_ptr->decoded_atr.interface_byte.TAx[IFB_2ND].TA & UIM_BIT5_CHECK)
    {
      specifi_mode_ptr->uim_atr_b5_bit_ta2 = TRUE; 
    }
    else
    {
      specifi_mode_ptr->uim_atr_b5_bit_ta2 = FALSE;
    }

    /*protcol to be used in specific mode*/
    specifi_mode_ptr->protocol 
             = (uim_transport_protocol_type)
                        (uim_ptr->decoded_atr.interface_byte.TAx[IFB_2ND].TA & UIM_T_FIELD);
    return TRUE;
  }

  return (FALSE);
}/* uim_util_get_specific_mode_info_from_atr */


/**
 * This function is used to check whether PPS to be performed or not.
 *
 * @param uim_ptr           Pointer to the UIM global instance
 * 
 * @return  boolean         TRUE if PPS to be performed else FALSE
 *
 */
boolean uim_util_is_pps_to_be_performed_in_atr
(
  uim_instance_global_type *uim_ptr
)
{
  if(NULL == uim_ptr)
  {
    return (FALSE);
  }
  /* TA1 contain this information */
  if(uim_ptr->decoded_atr.interface_byte.TAx[IFB_1ST].is_TA_byte_present)
  {
    if((uim_ptr->decoded_atr.interface_byte.TAx[IFB_1ST].TA != UIM_NO_PPS_VALUE1) &&
       (uim_ptr->decoded_atr.interface_byte.TAx[IFB_1ST].TA != UIM_NO_PPS_VALUE2))
    {
      /* PPS to be executed */
      return TRUE;
    }
  }
  /* PPS to be skipped */
  return FALSE;
}/* uim_util_is_pps_to_be_performed_in_atr */


/**
* This function is used to populate Fi and Di parameters
*
* @param uim_ptr      Pointer to the UIM global instance
* @param FI_ptr       pointer to FI_ptr
* @param DI_ptr       pointer to DI_ptr
*
* @return  boolean    FALSE if validation failed else TRUE
*
*/
boolean uim_util_get_fi_di_from_atr
(
  uim_instance_global_type               *uim_ptr,
  uim_clock_rate_conversion_factor_type  *FI_ptr,
  uim_baud_rate_adjustment_factor_type   *DI_ptr
)
{
  if((NULL == FI_ptr)|| (NULL == DI_ptr) || (NULL == uim_ptr))
  {
    return (FALSE);
  }
  
  /* The Interface character TA1 holds the Fi and Di */
  if(uim_ptr->decoded_atr.interface_byte.TAx[IFB_1ST].is_TA_byte_present)
  {
    *FI_ptr= (uim_clock_rate_conversion_factor_type)
                (uim_ptr->decoded_atr.interface_byte.TAx[IFB_1ST].TA >> UIM_FI_SHIFT_OF_TA1);

    *DI_ptr = (uim_baud_rate_adjustment_factor_type)
                 (uim_ptr->decoded_atr.interface_byte.TAx[IFB_1ST].TA & UIM_DI_MASK_OF_TA1);
    return (TRUE);
  }
  return (FALSE);
}/* uim_util_get_fi_di_from_atr */


/**
* This function is used to get the TC1 from ATR 
*
* @param uim_ptr           Pointer to the UIM global instance
* @param tc1_ptr           pointer to tc1
* @param t1_guard_time     Pointer to t1 guard time
*
* @return  boolean         FALSE if it falls out of specified value else TRUE
*
*/
boolean uim_util_get_tc1_byte_from_atr
(
  uim_instance_global_type  *uim_ptr,
  uint8                     *tc1_ptr
)
{
  if(NULL == uim_ptr ||  NULL == tc1_ptr)
  {
    return (FALSE);
  }
  /* Process Gaurd time present in TC1 */
  if(uim_ptr->decoded_atr.interface_byte.TCx[IFB_1ST].is_TC_byte_present)
  {
    *tc1_ptr = uim_ptr->decoded_atr.interface_byte.TCx[IFB_1ST].TC;
    return (TRUE);
  }
  return (FALSE);
}/* uim_util_get_tc1_byte_from_atr */


/**
* This function is used to validate Waiting Time
*
* @param uim_ptr    Pointer to the UIM global instance
*
* @return  uint8    TC2 value if it is within range or default value
*
*/
uint8 uim_util_get_waiting_time_from_atr
(
  uim_instance_global_type *uim_ptr
)
{
  if(NULL == uim_ptr)
  {
    return ((uint8)0);
  }

  if(uim_ptr->decoded_atr.interface_byte.TCx[IFB_2ND].is_TC_byte_present)
  {
    if((0x00 == uim_ptr->decoded_atr.interface_byte.TCx[IFB_2ND].TC) ||
       (0xFF == uim_ptr->decoded_atr.interface_byte.TCx[IFB_2ND].TC))
    {
      return((uint8)10);
    }
    return(uim_ptr->decoded_atr.interface_byte.TCx[IFB_2ND].TC);
  }
  else
  {
    return((uint8)10);
  }
}/* uim_util_get_waiting_time_from_atr */


/**
* This function is used to check whether CLF is supported or not
*  
* @param  uim_ptr          Pointer to the UIM global instance
*
* @return  boolean          FALSE If SWP is not supported
*                           TRUE:  SWP is supported
*/
boolean uim_util_is_swp_supported_in_atr
(
  uim_instance_global_type *uim_ptr
)
{
  if(NULL == uim_ptr)
  {
    return (FALSE);
  }

  /* tBi (i > 2) after T = 15 of the ATR, TB3  */
  if((uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].is_TD_byte_present) &&
     (uim_ptr->decoded_atr.interface_byte.TBx[IFB_3RD].is_TB_byte_present))
  {
    if((UIM_T_FIELD == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].TD & UIM_T_FIELD)) &&
       (UIM_UICC_CLF_INTERFACE_SUPPORTED == (UIM_UICC_CLF_INTERFACE_SUPPORTED & 
                               uim_ptr->decoded_atr.interface_byte.TBx[IFB_3RD].TB)))
    {
      return TRUE;
    } 
  }

  /* tBi (i > 2) after T = 15 of the ATR, TB4  */
  if((uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].is_TD_byte_present) &&
     (uim_ptr->decoded_atr.interface_byte.TBx[IFB_4TH].is_TB_byte_present ))
  {
    if((UIM_T_FIELD == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].TD & UIM_T_FIELD)) &&
       (UIM_UICC_CLF_INTERFACE_SUPPORTED == (UIM_UICC_CLF_INTERFACE_SUPPORTED & 
                               uim_ptr->decoded_atr.interface_byte.TBx[IFB_4TH].TB)))
    {
      return TRUE;
    } 
  }
  return FALSE;
}/* uim_util_is_swp_supported_in_atr */


/**
* This function is used to check whether eUICC is supported or not
*
* @param  uim_ptr          Pointer to the UIM global instance
*
* @return  boolean          FALSE: If eUICC is not supported
*                           TRUE:  eUICC is supported
*/
boolean uim_util_is_euicc_supported_in_atr
(
  uim_instance_global_type *uim_ptr
)
{
  if(NULL == uim_ptr)
  {
    return (FALSE);
  }

   /* tBi (i > 2) after T = 15 of the ATR, TB3  */
  if(uim_ptr->decoded_atr.interface_byte.TBx[IFB_3RD].is_TB_byte_present &&
     uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].is_TD_byte_present)
  {
    if((UIM_T_FIELD == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_2ND].TD & UIM_T_FIELD)) &&
       (UIM_EUICC_SUPPORTED == (UIM_EUICC_SUPPORTED & 
                                   uim_ptr->decoded_atr.interface_byte.TBx[IFB_3RD].TB)))
    {
      return TRUE;
    } 
  }

   /* tBi (i > 2) after T = 15 of the ATR, TB4  */
  if(uim_ptr->decoded_atr.interface_byte.TBx[IFB_4TH].is_TB_byte_present &&
     uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].is_TD_byte_present)
  {
    if((UIM_T_FIELD == (uim_ptr->decoded_atr.interface_byte.TDx[IFB_3RD].TD & UIM_T_FIELD)) &&
       (UIM_EUICC_SUPPORTED == (UIM_EUICC_SUPPORTED & 
                                   uim_ptr->decoded_atr.interface_byte.TBx[IFB_4TH].TB)))
    {
      return TRUE;
    } 
  }
  return FALSE;
}/* uim_util_is_euicc_supported_in_atr */


/**
* This function is used to parse the input buffer that contains the length bytes of a TLV
  and returns back a single uint32 with length derived from those bytes
  (For length values greater than 127, the length is encoded in a multiple
  bytes)
*
* @return  uim_return_type
*
*/
uim_return_type uim_util_get_len_from_tlv 
(
  uint32                              *out_len_ptr,
  uint8                               *out_num_len_bytes_ptr,
  uint32                               in_data_len,
  uint8                               *in_data_ptr
)
{
  uint8                     offset = 0;

  if(in_data_ptr == NULL ||
     in_data_len == 0 ||
     out_len_ptr == NULL)
  {
    return UIM_GENERIC_ERROR;
  }

  if(in_data_ptr[offset] < UIM_ONE_BYTE_LEN)
  {
    /* Length is encoded in just one byte */
    *out_len_ptr = in_data_ptr[offset];
    if(out_num_len_bytes_ptr)
    {
      *out_num_len_bytes_ptr = 1;
    }
  }
  else if(in_data_ptr[offset] > UIM_ONE_BYTE_LEN &&
          in_data_len > (offset + 1))
  {
    uint8  len_of_len              = 0;
    uint8  num_len_bytes           = 0;
    uint32 temp_len                = 0;

    /* Length is encoded in multiple bytes */
    num_len_bytes = (in_data_ptr[offset++] & UIM_NUM_LEN_BYTES_MASK);
    if(num_len_bytes == 0 ||
       num_len_bytes > 4 || /* We don't allow more than 4
                               bytes long (uint32) length field */
       in_data_len < (offset + num_len_bytes))
    {
      return UIM_GENERIC_ERROR;
    }
    while(len_of_len < num_len_bytes)
    {
      temp_len = in_data_ptr[offset++];
      *out_len_ptr |=
        (temp_len << (8*(num_len_bytes - len_of_len - 1)));
      len_of_len ++;
    }
    if(out_num_len_bytes_ptr)
    {
      *out_num_len_bytes_ptr = num_len_bytes + 1;
    }
  }
  else
  {
    /* Length = 0x80 indicating indefinite length format(we don't support that)
       OR
       in_data_len is invalid */
    return UIM_GENERIC_ERROR;
  }

  return UIM_SUCCESS;
} /* uim_util_get_len_from_tlv */


/**
* This function is used to parse ASN1 TLV based on input data.
*
* @return  uim_return_type
*
*/
uim_return_type uim_util_asn1_decode_parse_tlv
(
  uint32                            data_len,
  uint8                             *data_ptr,
  uint32                            offset,
  uim_asn1_cmds_type                cmds,
  uint32                            min_len,
  uint32                            max_len,
  uint32                            *offset_out_ptr,
  uint32                            *v_len_out_ptr,
  uim_asn1_tlv_type_type            *tlv_type_out_ptr,
  boolean                           is_optional
)
{
  uint32                 i                   = 0;
  uint8                  current_tlv_len_len = 0;
  uint32                 current_tlv_len     = 0;
  uint32                 current_offset      = offset;
  boolean                found_tag           = FALSE;

  if((1 < cmds.num_of_tlv_types &&
      NULL == tlv_type_out_ptr) ||
      NULL == data_ptr          ||
      data_len == 0             ||
      offset > data_len)
  {
    return UIM_GENERIC_ERROR;
  }

  if(offset == data_len)
  {
    /* Nothing to further process
       Return */
    return UIM_SUCCESS;
  }

  /* Initialize the output type value */
  if(NULL != tlv_type_out_ptr)
  {
    *tlv_type_out_ptr = UNKNOWN_TLV_TYPE;
  }

  /* Find and check the correct tag is present */
  for(i = 0; i < cmds.num_of_tlv_types; i++)
  {
    if(ANY_TLV_TYPE == cmds.tlv_type[i])
    {
      /* Any tag will be consider as a match.
         This is used to skip TLVs that we don't care */
      found_tag = TRUE;
      if(UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM <= (data_len - current_offset) &&
         UIM_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM == cmds.tlv_tag[i].tag_len)
      {
        current_offset += UIM_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
      }
      else if(UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM <= (data_len - current_offset) &&
              UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM == cmds.tlv_tag[i].tag_len)
      {
        current_offset += UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
      }
      
      if(NULL != tlv_type_out_ptr)
      {
        *tlv_type_out_ptr = cmds.tlv_type[i];
      }
      break;
    }

    if(UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM      <= (data_len - current_offset) &&
       cmds.tlv_tag[i].tag[UIM_ASN1_FIRST_TAG_INDEX] == data_ptr[current_offset])
    {
      if(UIM_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM   <= (data_len - current_offset)  &&
         UIM_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM   == cmds.tlv_tag[i].tag_len)
      {
        if(cmds.tlv_tag[i].tag[UIM_ASN1_SECOND_TAG_INDEX] == data_ptr[current_offset + 1])
        {
          found_tag = TRUE;
          current_offset += UIM_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
          if(NULL != tlv_type_out_ptr)
          {
            *tlv_type_out_ptr = cmds.tlv_type[i];
          }
          break;
        }
      }
      else if(UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM   == cmds.tlv_tag[i].tag_len)
      {
        found_tag = TRUE;
        current_offset += UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
        if(NULL != tlv_type_out_ptr)
        {
          *tlv_type_out_ptr = cmds.tlv_type[i];
        }
        break;
      }
    }
  }

  if (FALSE == found_tag)
  {
    if(FALSE == is_optional)
    {
      /* Did not match any expecting tags */
      return UIM_GENERIC_ERROR;
    }
  }
  else
  {
    /* Get the V len */
    if((data_len <= current_offset) ||
        UIM_SUCCESS != uim_util_get_len_from_tlv(&current_tlv_len,
                                                 &current_tlv_len_len,
                                                (data_len - current_offset),
                                                (data_ptr + current_offset)) ||
       (data_len - current_offset - current_tlv_len_len) < current_tlv_len)
    {
      return UIM_GENERIC_ERROR;
    }
    current_offset += current_tlv_len_len;

    /* Check the V len is in range for min_len, max_len and (data_len-offset) */
    if((min_len > 0 && (min_len > current_tlv_len)) ||
       (max_len > 0 && (max_len < current_tlv_len)))
    {
      return UIM_GENERIC_ERROR;
    }
    /* Update outputs, is this the offset for the value field*/
    if(NULL != offset_out_ptr)
    {
      *offset_out_ptr = current_offset;
    }

    if(NULL != v_len_out_ptr)
    {
      /* If v_len_out_ptr is NULL, it means
         the caller doesn't care about the v len value.
         Most likely it's a fixed value.*/
      *v_len_out_ptr  = current_tlv_len;
    }
  }

  return UIM_SUCCESS;
} /* uim_util_asn1_decode_parse_tlv */



/**
* This function is used to check whether AID record present in EF-DIR is valid or not
*
* @param uim_rsp_buf_type           Pointer to the response data
*
* @return  boolean                  TRUE if valid else FALSE
*
*/
boolean uim_util_is_aid_valid
(
  uim_rsp_buf_type  *rsp_ptr
)
{
  uint8                                  application_template_len   = 0;  
  uint8                                  aid_length                 = 0;  
  
  if(NULL == rsp_ptr)
  {
    return FALSE;
  }

  /* Per Spec 102.221, the first 4 bytes & AID are mandatory */
  if (rsp_ptr->cmd_rsp_size < UIM_EF_DIR_APP_TEMPLATE_ENTRY_MIN_LEN)
  {
    UIM_MSG_ERR_1("Invalid length, 0x%x", rsp_ptr->cmd_rsp_size);
    return FALSE;
  }

  /* Some commercial cards in market have wrong application length in the encoding.
     So deriving application template length from actual received buffer */
  if (rsp_ptr->rsp.data[UIM_EF_DIR_TEMPLATE_LEN_OFFSET] >
      (rsp_ptr->cmd_rsp_size - 2))
  {
    application_template_len = rsp_ptr->cmd_rsp_size - 2;
  }
  else
  {
    application_template_len = rsp_ptr->rsp.data[UIM_EF_DIR_TEMPLATE_LEN_OFFSET];
  }

  /* Ensure byte [0] = '61', byte [1] is at least 3 bytes &
     byte [2] = '4F' else flag error */
  if((rsp_ptr->rsp.data[UIM_EF_DIR_TEMPLATE_TAG_OFFSET] ==
        UIM_APP_TEMPLATE_TAG) &&
     (application_template_len >= UIM_EF_DIR_APP_TEMPLATE_MIN_LEN) &&
     (rsp_ptr->rsp.data[UIM_EF_DIR_AID_TAG_OFFSET] ==
        UIM_APP_ID_TAG))
  {
    aid_length = rsp_ptr->rsp.data[UIM_EF_DIR_AID_LEN_OFFSET];
  
    if ((aid_length == 0) ||
        (aid_length > UIM_AID_MAX_NAME_LENGTH) ||
        (aid_length > (rsp_ptr->cmd_rsp_size -
                       UIM_EF_DIR_APP_TEMPLATE_ENTRY_MIN_LEN)) ||
        (aid_length > (application_template_len - (UIM_EF_DIR_AID_LEN_OFFSET - 1))))
    {
      UIM_MSG_ERR_1(" Invalid AID length, 0x%x", aid_length);
      return FALSE;
    }  
  }
  else
  {
    UIM_MSG_ERR_0(" Invalid tag parsing for AID");
    return FALSE;
  }  

  return TRUE;
}/* uim_util_is_aid_valid */
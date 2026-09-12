/*==============================================================================
  FILE:         uim_logging.c

  OVERVIEW:     File contains the functions for APDU and EFS logging.

  DEPENDENCIES: N/A

                Copyright (c) 2014-2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/

/*=============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_logging.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when        who        what, where, why
------      ----       ---------------------------------------------------------
12/20/17    ak         Added a NULL check to cmd_ptr
08/30/17    gm         Added control byte support in 0x19B7
05/09/17    gm         Boot-up APDU logging
02/17/17    gm         Masking sensitive information
01/19/17    vdc        Added wrapper function to commit log message
11/30/16    gm         Store response data in buffer and then commit
10/14/16    ku         APDU Logging for extended log packets
10/03/16    dd         Removed lint comments
06/15/16    gm         Stub out Hotswap related functionality
04/27/16    ks         Fix KW issue
09/25/15    sam        Remove the logic to store timeout info in EFS
09/10/15    na         Restricting EFS APDU Logging with Feature
08/10/15    lm         Fix compilation issues on linux mob
08/07/15    lm         Replace strcpy_s with strncpy
07/02/15    sam        Offartget change- correcting the destination buffer size 
                       in strcpy_s
05/11/15    hyo        In efs logging, clear the buffer size when ptr is freed
04/08/15    sam        FR24498: Powerup logging changes
04/06/15    ll         UIMDRV migrates to UIM COMMON EFS APIs for all EFS items
02/27/15    xj         Add EFS apdu logging ut support
02/23/15    ks         Fix with APDU logging in EFS during efs clean-up
01/07/14    sam        Modification of msg macros for FR24498-UIM Powerup Logging
12/10/14    na         Making APDU Logging file instance based
11/10/14    akv        Support for user to customize LDO settings
10/01/14    ll         KW errors
09/18/14    akv        UIMDRV UT enhancements
06/16/14    ll         Switch to new string based TLMM APIs
03/03/14    nmb        Directory maintenance I&T bug fixes
02/20/14    ak         Fix compile errors due to strict compiler on 9x35
01/29/14    akv        RX ISR refactoring changes
01/23/14    yk         The first revision
==============================================================================*/ 
#include "uimglobals.h"
#include "uim_logging.h"
#include "uimdrv_gpio.h"
#include "uim_common_lib.h"

static void uim_log_commit_buffer_ext(uim_log_message_type            msg_type,
                               uint8                                 *buffer_ptr, 
                               uint16                                 buffer_len,
                               qword                                  timestamp,
                               uint16                                *seq_no_ptr,
                               uint8                                 *slot_id_ptr,
                               const uim_instance_global_type        *uim_ptr);


/**
 *  DEFINITION OF INTERNAL FUNCTIONS
 */
/**
 * This function allocate memory and and put it in cache
 *
 * @param msg_type       Message type of logging data
 * @param buffer         Data to be logged.
 * @param buffer_len     Size of log packet.
 * @param seq_number     Sequence Number.
 * @param slot_id        Slot id.
 */
static void uim_log_insert_log_queue(uim_log_message_type             msg_type,
                                     uint8                           *buffer_ptr, 
                                     uint16                           buffer_len,                      
                                     uint16                           seq_number,
                                     uint8                            slot_id)
{
  uim_boot_up_apdu_log_node     *temp_node         = NULL;
  uim_shared_global_type        *shared_global_ptr = uim_get_common_globals_ptr();  
  log_hdr_type                   log_hdr;  

  if(shared_global_ptr == NULL)
  {
    return;
  }

  /* FPOS will give the length of element in structure till apdu_data which is 
     fixed component + apdu length which is denoted by buffer_len. 
     So malloc will allocate memory which is required */
  temp_node = uim_malloc(FPOS(uim_boot_up_apdu_log_node, apdu_data) + buffer_len);

  if(temp_node == NULL)
  {
    return;
  }

  memset(&log_hdr, 0x00, sizeof(log_hdr));

  /* Get the timestamp from DIAG */
  log_set_timestamp(&log_hdr);
  uim_memscpy(temp_node->timestamp, sizeof(temp_node->timestamp), log_hdr.ts, sizeof(log_hdr.ts));  

  uim_memscpy(temp_node->apdu_data, buffer_len, buffer_ptr, buffer_len);
  temp_node->apdu_data_len  = buffer_len;  
  temp_node->message_type   = msg_type;
  temp_node->seq_number     = seq_number;
  temp_node->slot_id        = slot_id;
  temp_node->next_ptr       = NULL;

  if(shared_global_ptr->boot_up_apdu_log_cache.head_node == NULL)
  {
    shared_global_ptr->boot_up_apdu_log_cache.head_node = temp_node;
    shared_global_ptr->boot_up_apdu_log_cache.tail_node = temp_node;
    shared_global_ptr->boot_up_apdu_log_cache.num_node = 1;
  }
  else if (shared_global_ptr->boot_up_apdu_log_cache.tail_node != NULL)
  {
    shared_global_ptr->boot_up_apdu_log_cache.tail_node->next_ptr = temp_node;
    shared_global_ptr->boot_up_apdu_log_cache.tail_node = temp_node;
    shared_global_ptr->boot_up_apdu_log_cache.num_node++;
  }
}/* uim_log_insert_log_queue */


/**
 * This function delete from log queue cache and return
 *
 * @return uim_boot_up_apdu_log_cache       log node from cache
 */
static uim_boot_up_apdu_log_node* uim_log_pop_from_log_queue(void)
{
  uim_boot_up_apdu_log_node *temp_node         = NULL;
  uim_shared_global_type    *shared_global_ptr = uim_get_common_globals_ptr();  

  if(shared_global_ptr == NULL)
  {
    return NULL;
  }    
  
  if(shared_global_ptr->boot_up_apdu_log_cache.head_node == NULL)
  {
    return NULL;
  }
  temp_node = shared_global_ptr->boot_up_apdu_log_cache.head_node;
  
  shared_global_ptr->boot_up_apdu_log_cache.head_node = shared_global_ptr->boot_up_apdu_log_cache.head_node->next_ptr;

  if(shared_global_ptr->boot_up_apdu_log_cache.num_node > 0)
  {
    shared_global_ptr->boot_up_apdu_log_cache.num_node--;
  }

  if(shared_global_ptr->boot_up_apdu_log_cache.num_node == 0)
  {
    shared_global_ptr->boot_up_apdu_log_cache.tail_node = NULL;
  }
  
  return temp_node;
}/* uim_log_pop_from_log_queue */


/**
 * This function allocate memory and and put it in cache
 *
 * @param msg_type       Message type of logging data
 * @param buffer         Data to be logged.
 * @param buffer_len     Size of log packet.
 * @param seq_number     Sequence Number.
 * @param slot_id        Slot id.
 */
static void uim_log_boot_up_cache(uim_log_message_type             msg_type,
                                  uint8                           *buffer_ptr, 
                                  uint16                           buffer_len,                      
                                  uint16                           seq_number,
                                  uint8                            slot_id)
{
  uim_boot_up_apdu_log_node    *temp_node         = NULL;
  uim_shared_global_type       *shared_global_ptr = uim_get_common_globals_ptr();

  if(shared_global_ptr == NULL)
  {
    return;
  }
  
  if(shared_global_ptr->boot_up_apdu_log_cache.num_node < 
            shared_global_ptr->boot_up_apdu_log_cache.boot_up_apdu_log_config.num_boot_up_apdu_log)
  {
    uim_log_insert_log_queue(msg_type, buffer_ptr, buffer_len, seq_number, slot_id);
  }
  else
  {
    if(shared_global_ptr->boot_up_apdu_log_cache.boot_up_apdu_log_config.cache_mechanism == UIM_LAST_N_APDU)
    {
      temp_node = uim_log_pop_from_log_queue();
      uim_log_insert_log_queue(msg_type, buffer_ptr, buffer_len, seq_number, slot_id);
      UIM_FREE(temp_node);
    }
  }
}/* uim_log_boot_up_cache */


/**
 * This function commit all the logs in cache to DIAG
 *
 */
void uim_log_boot_up_commit(void)
{
  uim_boot_up_apdu_log_node    *temp_node           = NULL;
  uim_instance_global_type     *uim_ptr             = NULL;
  uim_shared_global_type       *shared_global_ptr   = uim_get_common_globals_ptr();  
  uint16                        i                   = 0;

  if(shared_global_ptr == NULL)
  {
    return;
  }

  /* Leaving critical section after updating "is_cache_commit_done" so that ISR 
     will not be blocked until cache logs are being push to DIAG */
  rex_enter_crit_sect(&shared_global_ptr->boot_up_apdu_log_cache.boot_up_apdu_log_cache_crit_section);  
  shared_global_ptr->boot_up_apdu_log_cache.is_cache_commit_done = TRUE;
  rex_leave_crit_sect(&shared_global_ptr->boot_up_apdu_log_cache.boot_up_apdu_log_cache_crit_section);      

  temp_node = uim_log_pop_from_log_queue();

  while(temp_node != NULL)
  {   
    switch (temp_node->slot_id)
    {
      case 1:
        uim_ptr = uim_get_instance_ptr(UIM_INSTANCE_1);
        break;
  
      case 2:
        uim_ptr = uim_get_instance_ptr(UIM_INSTANCE_2);
        break;
    
      case 3:
        uim_ptr = uim_get_instance_ptr(UIM_INSTANCE_3);
        break;
    
      default:
        uim_ptr = NULL;
        break;
    }  

    if(uim_ptr != NULL)
    {
      uim_log_commit_buffer_ext(temp_node->message_type,
                                temp_node->apdu_data,
                                temp_node->apdu_data_len,
                                temp_node->timestamp,
                               &temp_node->seq_number,
                               &temp_node->slot_id,
                                uim_ptr);
    }

    UIM_FREE(temp_node);

    temp_node = uim_log_pop_from_log_queue();    
  }  
}/* uim_log_boot_up_commit */


/**
 * This function allocate memory and copy buffered data to the
 * memory for diag. It initializes the buffer parameters when
 * copying is done.
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return boolean Success or Fail.
 */
static boolean uim_log_put_buffer
(
  uim_instance_global_type *uim_ptr
)
{
  uint8             log_len_local = 0;
  uim_log_data_type *log_ptr  = NULL;
  boolean           status = FALSE;
#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

  if (uim_ptr->debug.log_data.length == 0)
    return TRUE; /* Nothing to log */

  /* Use local variable to store length of buffer, so that it
     does not get updated while switching to isr context */
  log_len_local = uim_ptr->debug.log_data.length;

  if (MULTI_SLOT_LOGGING_MASK_14CE == uim_ptr->debug.apdu_log_mask.log_mask)
  {
    log_ptr = (uim_log_data_type *) log_alloc (LOG_UIM_DS_DATA_C,
              FPOS(uim_log_data_type, data) + log_len_local);
  }
  else if (SINGLE_SLOT_LOGGING_MASK_1098 == uim_ptr->debug.apdu_log_mask.log_mask)
  {
    log_ptr = (uim_log_data_type *) log_alloc (LOG_UIM_DATA_C,
                FPOS(uim_log_data_type, data) + log_len_local);
  }

  if (log_ptr != NULL)
  {
    log_ptr->length = log_len_local;

    if (log_ptr->length <= (LOG_UIM_MAX_CHARS + LOG_UIM_TSTAMP_SIZE))
    {
      uim_memscpy( (void *) (log_ptr->data),
               sizeof(log_ptr->data),
              (void *) uim_ptr->debug.log_data.data,
              log_ptr->length );
    }

    /* Do not commit data if uim_log_data got updated after log_alloc
       as we may loose data because of this */
    if(log_len_local != uim_ptr->debug.log_data.length)
    {
      log_free((uim_log_data_type  *) log_ptr);
      return FALSE;
    }
    uim_log_packet_commit(SYS_MODEM_AS_ID_NONE, (void *) log_ptr);
    status = TRUE;

#ifdef FEATURE_UIM_TEST_FRAMEWORK
      #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
  }

  /* Reset the parameters */
  uim_ptr->debug.log_data.length = 0;
  uim_ptr->debug.log_char.attrib = UIM_LOG_ATTRIB_INIT;
  return status;
} /* uim_log_put_buffer */


/**
 * This funciton put a byte along with the attrib byte to the
 * buffer.
 *
 * @param uim_ptr Pointer to the global data common.
 */
static void uim_log_put_char( uim_instance_global_type *uim_ptr )
{
  /* LOG_UIM_MAX_CHARS is 247 while uim_log_data_ptr->data[] size is
     255 bytes. Last 8 bytes are reserved for time stamp.
     Need to add boundary checks so that for always keep last 8 bytes
     free for time stamp.

     We add minimum three log bytes. We need to adjust boundary check
     appropriately.
  */
  if (MULTI_SLOT_LOGGING_MASK_14CE == uim_ptr->debug.apdu_log_mask.log_mask &&
        uim_ptr->debug.log_data.length  <= (LOG_UIM_MAX_CHARS - 3))
  {
    /* Store the attribute byte */
    uim_ptr->debug.log_data.data[uim_ptr->debug.log_data.length++] = uim_ptr->debug.log_char.attrib;
    /* Store the slot byte */
    uim_ptr->debug.log_data.data[uim_ptr->debug.log_data.length++] = uim_ptr->debug.log_char.slot_id;
    /* Store the byte */
    uim_ptr->debug.log_data.data[uim_ptr->debug.log_data.length++] = uim_ptr->debug.log_char.the_char;
  }
  else if (SINGLE_SLOT_LOGGING_MASK_1098 == uim_ptr->debug.apdu_log_mask.log_mask &&
             uim_ptr->debug.log_data.length  <= (LOG_UIM_MAX_CHARS - 2))
  {
    /* Store the attribute byte */
    uim_ptr->debug.log_data.data[uim_ptr->debug.log_data.length++] = uim_ptr->debug.log_char.attrib;
    /* Store the byte */
    uim_ptr->debug.log_data.data[uim_ptr->debug.log_data.length++] = uim_ptr->debug.log_char.the_char;
  }

  if(uim_ptr->debug.log_data.length  >= LOG_UIM_MAX_CHARS)
  {
    if(!uim_log_put_buffer(uim_ptr))
    {
      /* As buffer could not be committed, reset the length of buffer
         to prevent buffer overflow */
      uim_ptr->debug.log_data.length = 0;
      uim_ptr->debug.log_char.attrib = UIM_LOG_ATTRIB_INIT;
    }
  }
} /* uim_log_put_char */


/**
 * This function put time stamp along with attrib byte to the
 * buffer
 *
 * @param uim_ptr Pointer to the global data common.
 */
static void uim_log_put_tstamp
(
  uim_instance_global_type *uim_ptr
)
{
  qword                t_stamp;
  uim_log_data_type   *uim_log_data_ptr = NULL;

  if (uim_ptr->id  >= UIM_MAX_INSTANCES)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"uim_log_put_tstamp invalid instance");
    return;
  }

  uim_log_data_ptr = &uim_ptr->debug.log_data;

  /* LOG_UIM_MAX_CHARS is 247 while uim_log_data_ptr->data[] size is
     255 bytes. Last 8 bytes are reserved for time stamp.
     Need to add boundary checks so that for always keep last 8 bytes
     free for time stamp.

     We add minimum three log bytes log bytes before time stamp. We need to
     adjust boundary check appropriately.
  */
  if (uim_log_data_ptr->length < (LOG_UIM_MAX_CHARS - 1))
  {
    uim_ptr->debug.log_char.attrib = UIM_LOG_TSTAMP;
    if (MULTI_SLOT_LOGGING_MASK_14CE == uim_ptr->debug.apdu_log_mask.log_mask)
    {
      /* Store the attribute byte */
      uim_log_data_ptr->data[uim_log_data_ptr->length++] = uim_ptr->debug.log_char.attrib;
      /* Store the slot byte */
      uim_log_data_ptr->data[uim_log_data_ptr->length++] = uim_ptr->debug.log_char.slot_id;
    }
    else if (SINGLE_SLOT_LOGGING_MASK_1098 == uim_ptr->debug.apdu_log_mask.log_mask)
    {
      /* Store the attribute byte */
      uim_log_data_ptr->data[uim_log_data_ptr->length++] = uim_ptr->debug.log_char.attrib;
    }
    /* Get time stamp */
    (void)time_get(t_stamp);
    /* Copy the time stamp to the buffer */
    uim_memscpy( (void *) &uim_log_data_ptr->data[uim_log_data_ptr->length],
            (sizeof(uim_log_data_ptr->data) - uim_log_data_ptr->length),
            (void *) t_stamp,
            LOG_UIM_TSTAMP_SIZE);
    /* Update the buffer length */
    uim_log_data_ptr->length   += LOG_UIM_TSTAMP_SIZE;
  }

  if(uim_log_data_ptr->length  >= LOG_UIM_MAX_CHARS)
  {
    if(!uim_log_put_buffer(uim_ptr))
    {
      /* As buffer could not be committed, reset the length of buffer
         to prevent buffer overflow */
      uim_log_data_ptr->length = 0;
      uim_ptr->debug.log_char.attrib = UIM_LOG_ATTRIB_INIT;
    }
  }
} /* uim_log_put_tstamp */


/**
 * DEFINITIONS OF EXTERNAL FUNTIONS
 *
 */

/**
 * Put a byte to APDU log buffer
 *
 * @param attr Attribute of logging data
 * @param ch Data to be logged.
 * @param uim_ptr Pointer to the global data common.
 */
void uim_log_put_byte(uim_log_attrib_type        attr,
                      char                       ch,
                      uim_instance_global_type  *uim_ptr)
{
  uim_ptr->debug.log_char.attrib = attr;
  uim_ptr->debug.log_char.the_char = ch;
  uim_ptr->debug.log_char.slot_id = uim_ptr->id;

  uim_log_put_char(uim_ptr);
  if( (uim_nv_is_feature_enabled(UIMDRV_FEATURE_LOG_APDU_TO_EFS,
                               uim_ptr) == TRUE) &&
      (TRUE == uim_ptr->debug.efslog_logging_in_progress)
    )
  {
    uim_efslog_apdu_to_buffer(uim_ptr);
  }
} /* uim_log_put_byte */


/**
 * Put whole data to APDU log buf
 *
 * @param msg_type       Message type of logging data
 * @param buffer         Data to be logged.
 * @param buffer_len     Size of log packet.
 * @param timestamp      Timestamp
 * @param seq_no_ptr     Sequence Number
 * @param slot_id        Slot id
 * @param uim_ptr        Pointer to the global data common.
 */
static void uim_log_commit_buffer_ext(uim_log_message_type            msg_type,
                               uint8                                 *buffer_ptr, 
                               uint16                                 buffer_len,
                               qword                                  timestamp,
                               uint16                                *seq_no_ptr,
                               uint8                                 *slot_id_ptr,
                               const uim_instance_global_type        *uim_ptr)
{
  uim_ext_log_data_type     *log_ptr                = NULL;
  uint16                     seq_number             = 0;
  static uint16              slot_1_seq_no          = 0;
  static uint16              slot_2_seq_no          = 0;
  static uint16              slot_3_seq_no          = 0; 
  uint8                      slot_id                = 0;
  uim_shared_global_type    *shared_global_ptr      = uim_get_common_globals_ptr();
  boolean                    log_cached  = FALSE;
  
  if((uim_ptr == NULL)  ||
     (buffer_len == 0)   ||
     (shared_global_ptr == NULL) ||
     (uim_nv_is_feature_enabled(UIMDRV_FEATURE_DEBUG_LOG, uim_ptr) == FALSE))
  {
    return;
  }

  /* If valid Sequence number is passed, use it */
  if(seq_no_ptr)
  {
    seq_number = *seq_no_ptr;
  }
  else
  {
    switch (uim_ptr->id)
    {
      case UIM_INSTANCE_1:
        seq_number = slot_1_seq_no++;
        break;
  
      case UIM_INSTANCE_2:
        seq_number = slot_2_seq_no++;
        break;
    
      case UIM_INSTANCE_3:
        seq_number = slot_3_seq_no++;
        break;
    
      default:
        return;
    }  
  }     

  /* If valid slot number is passed, use it */
  if(slot_id_ptr)
  {
    slot_id = *slot_id_ptr;
  }
  else
  {
    switch (uim_ptr->id)
    {
      case UIM_INSTANCE_1:
        slot_id   = 1;
        break;
  
      case UIM_INSTANCE_2:
        slot_id    = 2;
        break;
    
      case UIM_INSTANCE_3:
        slot_id    = 3;
        break;
    
      default:
        return;
    }  
  }

  rex_enter_crit_sect(&shared_global_ptr->boot_up_apdu_log_cache.boot_up_apdu_log_cache_crit_section);
  /* If "is_cache_commit_done"  is TRUE and if log packet is not enabled then cache the APDU's */
  log_cached = !shared_global_ptr->boot_up_apdu_log_cache.is_cache_commit_done && 
                     !log_status(LOG_UIM_APDU_LOG_C);
  if(log_cached)
  {
    uim_log_boot_up_cache(msg_type, buffer_ptr, buffer_len, seq_number, slot_id);
  }
  rex_leave_crit_sect(&shared_global_ptr->boot_up_apdu_log_cache.boot_up_apdu_log_cache_crit_section);

  if(log_cached)
  {
    return;
  }
  
  /* Allocate memory for log commit, log_alloc will fail if MASK is not set */
  log_ptr = (uim_ext_log_data_type *) log_alloc (LOG_UIM_APDU_LOG_C, FPOS(uim_ext_log_data_type, data) + buffer_len);

  if (log_ptr == NULL)
  {
    return;
  }

  log_ptr->version = 0x01;  
  
  /* Populate the message type */
  switch (msg_type)
  {
    case UIM_LOG_COLD_RESET:
    case UIM_LOG_WARM_RESET:
    case UIM_LOG_POWER_OFF:
    case UIM_LOG_TX:
    case UIM_LOG_RX:
    case UIM_LOG_PPS_TX:
    case UIM_LOG_PPS_RX:
    case UIM_LOG_ATR_RX:
      log_ptr->msg_type = (uint8)msg_type;
      break;

    default:
      return;
  }

  /* Populate the control byte bit-0 */
  switch (uim_ptr->setting.phy_layer)
  {
#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC  
    #error code not present
#endif
    case UIM_REMOTE:
      /* Update the control byte bit-0 to '1' */
      log_ptr->control_byte |= 0x01;
      break;
      
    default:
      /* Update the control byte bit-0 to '0' */
      log_ptr->control_byte &= 0xFE;
      break;
  }  

  log_ptr->slot_id = slot_id;
  log_ptr->seq_number = seq_number;

  if(timestamp != NULL)
  {
    uim_memscpy(log_ptr->hdr.ts, sizeof(log_ptr->hdr.ts), timestamp, sizeof(log_ptr->hdr.ts));
  }  
  
  memset(log_ptr->rfu, 0x00, sizeof(log_ptr->rfu));
  log_ptr->data_length = buffer_len;
  uim_memscpy(log_ptr->data, 
              log_ptr->data_length, 
              buffer_ptr,
              buffer_len);

  uim_log_packet_commit(SYS_MODEM_AS_ID_NONE, (void *) log_ptr);
  log_ptr = NULL;
} /* uim_log_commit_buffer_ext */


/**
 * This function put power off data along with attrib byte to the
 * buffer
 *
 * @param msg_type Message type of logging data
 * @param uim_ptr  Pointer to the global data common.
 */
void uim_log_put_power_off_data(const uim_instance_global_type *uim_ptr)
{
  uint8 buffer[2] = {0, 0};

  buffer[0] = UIM_LOG_CLOCK_0_MHz;
  buffer[1] = UIM_LOG_VOLTAGE_0P0;

  /* Logging of extended packets */	
  uim_log_commit_buffer_ext(UIM_LOG_POWER_OFF,
                            buffer,
                            sizeof(buffer),
                            NULL,
                            NULL,
                            NULL,
                            uim_ptr); 
} /* uim_log_put_power_off_data */


/**
 * This function put reset data along with attrib byte to the
 * buffer
 *
 * @param is_cold_reset Reset type
 * @param uim_ptr       Pointer to the global data common.
 */
void uim_log_put_reset_data(boolean                                 is_cold_reset,
                            const uim_instance_global_type         *uim_ptr)
{
  uint8 buffer[2] = {0, 0};

  switch (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq)
  {
    case 3840000:
      buffer[0] = UIM_LOG_CLOCK_3P84_MHz;
      break;

    case 4800000:
      buffer[0] = UIM_LOG_CLOCK_4P8_MHz;
      break;

    default:
      return;
  }

  switch (uim_ptr->state.current_voltage_class)
  {
    case UIM_VOLTAGE_CLASS_C:
      buffer[1] = UIM_LOG_VOLTAGE_1P8;
      break;

    case UIM_VOLTAGE_CLASS_B:
      buffer[1] = UIM_LOG_VOLTAGE_3P0;
      break;

    default:
      return;
  }

  /* Logging of extended packets */	
  if(is_cold_reset)
  {
    uim_log_commit_buffer_ext(UIM_LOG_COLD_RESET,
                              buffer,
                              sizeof(buffer),
                              NULL,
                              NULL,
                              NULL,
                              uim_ptr);
  }  
  else
  {
    uim_log_commit_buffer_ext(UIM_LOG_WARM_RESET,
                              buffer,
                              sizeof(buffer),
                              NULL,
                              NULL,
                              NULL,
                              uim_ptr);
  }
} /* uim_log_put_reset_data */


/**
 * This function puts ATR recieved from the card
 * along with attrib byte to the buffer
 *
 * @param buffer         Data to be logged.
 * @param buffer_len     Size of log packet.
 * @param uim_ptr        Pointer to the global data common.
 */
void uim_log_put_atr_data(uint8                                 *buffer,
                          uint16                                 buffer_len,                           
                          const uim_instance_global_type        *uim_ptr) 
{  
  uim_log_commit_buffer_ext(UIM_LOG_ATR_RX,                            
                            buffer,
                            buffer_len,       
                            NULL,
                            NULL,
                            NULL,
                            uim_ptr);
} /* uim_log_put_atr_data */


/**
 * This function puts PPS to the buffer
 *
 * @param is_tx          Type of PPS data whether recieved from card or send to card
 * @param buffer         Data to be logged.
 * @param buffer_len     Size of log packet.
 * @param uim_ptr        Pointer to the global data common.
 */
void uim_log_put_pps_data(boolean                                is_tx, 
                          uint8                                 *buffer,
                          uint16                                 buffer_len,  
                          const uim_instance_global_type        *uim_ptr)
{  
  uim_log_commit_buffer_ext(is_tx ? UIM_LOG_PPS_TX : UIM_LOG_PPS_RX,
                            buffer,
                            buffer_len,
                            NULL,
                            NULL,
                            NULL,
                            uim_ptr);
} /* uim_log_put_pps_data */


/**
 * This function put TX RX data to the
 * buffer
 *
 * @param is_tx          APDU Data depending on whether the packet is transmitted or recieved
 * @param is_commit      Data to commit.
 * @param buffer         Data to be logged.
 * @param buffer_len     Size of log packet.
 * @param uim_ptr        Pointer to the global data common.
 */
void uim_log_put_apdu_data(boolean                          is_tx,
                           boolean                          is_commit,
                           uint8                           *buffer,
                           uint16                           buffer_len,
                           uim_instance_global_type        *uim_ptr)
{
  uint8        *temp_buffer                 = NULL;
  uint16        offset                      = 0;
  uint16        i                           = 0;

  if(uim_ptr == NULL || uim_ptr->command.cmd_ptr == NULL )
  {
    return;
  }
  
  temp_buffer = &uim_ptr->debug.ext_log_buffer_data.buffer[0];
  offset = uim_ptr->debug.ext_log_buffer_data.buffer_offset;  

  if(buffer != NULL && offset < UIM_EXT_LOG_MAX_SIZE)
  {
    uim_memscpy(&temp_buffer[offset],
                UIM_EXT_LOG_MAX_SIZE - offset,
                buffer,
                buffer_len);
    offset = ((offset + buffer_len) > UIM_EXT_LOG_MAX_SIZE) ? UIM_EXT_LOG_MAX_SIZE : (offset + buffer_len);
  }

  if(is_commit && offset > 0)
  {
    if(is_tx && uim_ptr->command.cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr != NULL)
    {
      if(uim_ptr->rxtx_state_machine.tx_state == UIM_TX_SND_CMD_DATA)
      {
        uim_ptr->command.cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr(temp_buffer, temp_buffer, offset);
      }
    }
    else if(!is_tx && 
           (uim_ptr->command.cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr != NULL) &&
           (offset > (NUM_ACK_BYTES + NUM_STATUS_BYTES)))
    {
      uim_ptr->command.cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr(temp_buffer+1, temp_buffer+1, offset - (NUM_ACK_BYTES + NUM_STATUS_BYTES));
    }

    for(i = 0; i < offset; i++)
    {
      uim_log_put_byte(is_tx ? UIM_LOG_TX_DATA : UIM_LOG_RX_DATA, *(temp_buffer+i), uim_ptr);
    }
    uim_log_put_last_byte(uim_ptr);

    uim_log_commit_buffer_ext(is_tx ? UIM_LOG_TX : UIM_LOG_RX,
                              temp_buffer,
                              offset,
                              NULL,
                              NULL,
                              NULL,
                              uim_ptr);
    offset = 0;
  }
  uim_ptr->debug.ext_log_buffer_data.buffer_offset = offset;
} /* uim_log_put_apdu_data */


/**
 * Put a time stamp then logging all data in APDU log bug.
 *
 * @param uim_ptr Pointer to the global data common.
 */
void uim_log_put_last_byte(uim_instance_global_type *uim_ptr)
{
  uim_ptr->debug.log_char.slot_id = uim_ptr->id;

  if (uim_ptr->debug.log_data.length != 0)
  {
    uim_log_put_tstamp(uim_ptr);
    uim_log_put_buffer(uim_ptr);
  }
} /* uim_log_put_last_byte */

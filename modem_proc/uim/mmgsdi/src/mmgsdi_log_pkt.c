/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                        MMGSDI BOOTUP MESSAGE LOGGING


GENERAL DESCRIPTION

  This source file contains the functions/method for the bootup message logging.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2015 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_log_pkt.c#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/19/17   vdc     Added wrapper function to commit log message
11/3/16    me      Fix compilation error 
04/13/16   ar      CDMA/HDR support on 2nd SUB
02/01/16   bcho    Remove MMGSDI_TEST_SIM_EVT and related code
10/05/15   bcho    log packet timestamp is encoded
09/10/15   kv      Features_status_list is split into device and slot items
08/27/15   ma      initial version


===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmgsdiutil.h"
#include "mmgsdi_nv.h"
#include "time_svc.h"
#include "mmgsdi_log_pkt.h"
#include "mmgsdilib_common.h"
#include "uim_common_lib.h"
#include "log.h"
#include "rex.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

#define MMGSDI_LOG_PKT_TIMER_VALUE         7000  /*7 sec timer*/
#define MMGSDI_LOG_PKT_MAX_PROV_SESSION    4
#define MMGSDI_LOG_PKT_MAX_SLOT            2
#define MMGSDI_LOG_PKT_MAX_APP             9
#define MMGSDI_LOG_PKT_VERSION             0x01

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_AFTER_NV_READ_TYPE
============================================================================*/
/* Stores the data for AFTER_NV_READ in packed format.
*/
typedef PACKED struct PACKED_POST {
  uint64    timestamp;
  uint8     nv_4398;
  uint8     nv_855[MMGSDI_LOG_PKT_MAX_SLOT];
  uint8     set_nv_855[MMGSDI_LOG_PKT_MAX_SLOT];
  uint8     nv_use_sfi;
  uint8     nv_use_3gpd[MMGSDI_LOG_PKT_MAX_SLOT];
  uint8     nv_skip_prov_app_termination;
  uint8     nv_feature_multisim_auto_provisioning;
  uint8     nv_ignore_init_error;
} mmgsdi_log_pkt_data_after_nv_read_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_LINK_EST_REQ_TYPE
============================================================================*/
/* Stores the data for LINK_EST_REQ in packed format.
*/
typedef PACKED struct PACKED_POST {
  uint64    timestamp;
  uint8     slot_id;
  uint8     protocol;
} mmgsdi_log_pkt_data_link_est_req_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_CARD_INSERTED_EVT_TYPE
============================================================================*/
/* Stores the data for CARD_INSERTED_EVT in packed format.
*/
typedef PACKED struct PACKED_POST {
  uint64    timestamp;
  uint8     slot_id;
  uint8     app_type[MMGSDI_LOG_PKT_MAX_APP];
  uint8     ef_dir_number_of_apps;
} mmgsdi_log_pkt_data_card_inserted_evt_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_SESSION_CHANGED_EVT_TYPE
============================================================================*/
/* Stores the data for SESSION_CHANGED_EVT in packed format.
*/
typedef PACKED struct PACKED_POST {
  uint64    timestamp;
  uint8     slot_id;
  uint8     session_changed_status;
  uint8     session_type;
  uint8     app_type;
  uint8     num_connections;
  uint8     channel_id;
} mmgsdi_log_pkt_data_session_changed_evt_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_PIN1_EVT_TYPE
============================================================================*/
/* Stores the data for PIN1_EVT in packed format.
*/
typedef PACKED struct PACKED_POST {
  uint64    timestamp;
  uint8     pin_status;
  uint8     session_type;
  uint8     pin_id;
  uint8     slot_id;
} mmgsdi_log_pkt_data_pin1_evt_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_TEST_SIM_EVT_TYPE
============================================================================*/
/* Stores the data for TEST_SIM_EVT in packed format.
*/
typedef PACKED struct PACKED_POST {
  uint64    timestamp;
  uint8     slot_id;
  uint8     test_sim;
  uint8     anite_gcf_nv;
} mmgsdi_log_pkt_data_test_sim_evt_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_PERSO_EVT_TYPE
============================================================================*/
/* Stores the data for PERSO_EVT in packed format.
*/
typedef PACKED struct PACKED_POST {
  uint64    timestamp;
  uint8     perso_status;
  uint8     feature;
  uint8     session_type;
  uint8     slot_id;
} mmgsdi_log_pkt_data_perso_evt_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_SUB_READY_EVT_TYPE
============================================================================*/
/* Stores the data for SUB_READY_EVT in packed format.
*/
typedef PACKED struct PACKED_POST {
  uint64    timestamp;
  uint8     session_type;
  uint8     slot_id;
} mmgsdi_log_pkt_data_sub_ready_evt_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_CARD_ERROR_EVT_TYPE
============================================================================*/
/* Stores the data for CARD_ERROR_EVT in packed format.
*/
typedef PACKED struct PACKED_POST {
  uint64    timestamp;
  uint8     card_err_cond;
  uint8     slot_id;
} mmgsdi_log_pkt_data_card_error_evt_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_DATA_TYPE
============================================================================*/
/* Stores the data of all events in a packed format.
*/
typedef PACKED struct PACKED_POST {
  mmgsdi_log_pkt_data_after_nv_read_type           after_nv_read;
  mmgsdi_log_pkt_data_link_est_req_type            link_est_req[MMGSDI_LOG_PKT_MAX_SLOT];
  mmgsdi_log_pkt_data_card_inserted_evt_type       card_inserted_evt[MMGSDI_LOG_PKT_MAX_SLOT];
  mmgsdi_log_pkt_data_session_changed_evt_type     session_changed_evt[MMGSDI_LOG_PKT_MAX_PROV_SESSION];
  mmgsdi_log_pkt_data_pin1_evt_type                pin1_evt[MMGSDI_LOG_PKT_MAX_PROV_SESSION];
  mmgsdi_log_pkt_data_test_sim_evt_type            test_sim_evt[MMGSDI_LOG_PKT_MAX_SLOT];
  mmgsdi_log_pkt_data_perso_evt_type               perso_evt[MMGSDI_LOG_PKT_MAX_PROV_SESSION];
  mmgsdi_log_pkt_data_sub_ready_evt_type           sub_ready_evt[MMGSDI_LOG_PKT_MAX_PROV_SESSION];
  mmgsdi_log_pkt_data_card_error_evt_type          card_error_evt[MMGSDI_LOG_PKT_MAX_SLOT];
} mmgsdi_log_pkt_data_type;

/*===========================================================================
   STRUCT:      MMGSDI_LOG_PKT_LOG_DATA_TYPE
============================================================================*/
/* Stores the data for the entire log packet including its header.
*/

/* Define data structure */
LOG_RECORD_DEFINE(LOG_MMGSDI_BOOTUP_MSG_C)
  uint8                    version;
  mmgsdi_log_pkt_data_type data;
LOG_RECORD_END

typedef LOG_MMGSDI_BOOTUP_MSG_C_type mmgsdi_log_pkt_log_data_type;

/* Pointer to the heap memory */
mmgsdi_log_pkt_data_type      *mmgsdi_log_pkt_ptr;

/* Timer to flush data on expiry */
static rex_timer_type          mmgsdi_log_pkt_timer;

/* Critical section to protect accesses to MMGSDI LOG PKT structure */
static rex_crit_sect_type      mmgsdi_log_pkt_cache_crit_sect;

/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_GET_SESSION_INDEX

   DESCRIPTION:
     This function returns the session index.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    Uint8 Session index

   SIDE EFFECTS:
    None

==========================================================================*/
static uint8 mmgsdi_log_pkt_get_session_index(
  mmgsdi_session_type_enum_type session_type
)
{
  uint8 session_index = MMGSDI_LOG_PKT_MAX_PROV_SESSION;

  switch(session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
      session_index = 0x00;
      break;

    case MMGSDI_1X_PROV_PRI_SESSION:
      session_index = 0x01;
      break;

    case MMGSDI_GW_PROV_SEC_SESSION:
      session_index = 0x02;
      break;

    case MMGSDI_1X_PROV_SEC_SESSION:
      session_index = 0x03;
      break;

    default:
      break;
  }
  return session_index;
} /* mmgsdi_log_pkt_get_session_index */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_FLUSH

   DESCRIPTION:
     This function flushes the data stored in the heap and then frees the heap.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
static void mmgsdi_log_pkt_flush(
  unsigned long unused
)
{
  /* Flushing Data */
  uint16                         log_len_local = 0;
  mmgsdi_log_pkt_log_data_type  *log_ptr       = NULL;

  (void)unused;

  /*Nothing to log */
  if(mmgsdi_log_pkt_ptr == NULL)
  {
    return;
  }

  /* Use local variable to store length of buffer, so that it
     does not get updated while switching to isr context */
  log_len_local = sizeof(mmgsdi_log_pkt_data_type);

  /*Allocate memory to log_ptr */
  log_ptr = (mmgsdi_log_pkt_log_data_type *) log_alloc (0x1901,
                                                        FPOS(mmgsdi_log_pkt_log_data_type, data) + log_len_local);

  if (log_ptr != NULL)
  {
    rex_enter_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);

    mmgsdi_memscpy((void *) (&log_ptr->data),
                   sizeof(log_ptr->data),
                   (void *) mmgsdi_log_pkt_ptr,
                   sizeof(mmgsdi_log_pkt_data_type));

    log_ptr->version = MMGSDI_LOG_PKT_VERSION;

    rex_leave_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);

    /*Commiting the log */
    uim_log_packet_commit(SYS_MODEM_AS_ID_NONE , (void *)log_ptr);
  }

  rex_enter_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);

  /* Freeing the heap */
  MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_log_pkt_ptr);

  rex_leave_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);

  /* Clear and undefine timer as it will not be used any further */
  (void)rex_clr_timer(&mmgsdi_log_pkt_timer);
  (void)rex_undef_timer(&mmgsdi_log_pkt_timer);
} /* mmgsdi_log_pkt_flush */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_INIT

   DESCRIPTION:
     This function initializes the timer and allocates memory to the log packet
     pointer.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_init
(
  void
)
{
  /* Do allocations and initializations only if mmgsdi_log_pkt_ptr is NULL */
  if(mmgsdi_log_pkt_ptr == NULL)
  {
    /* Initializing critical section */
    rex_init_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);

    /*Timer Defination*/
    rex_def_timer_ex(&mmgsdi_log_pkt_timer, mmgsdi_log_pkt_flush, 0);

    /*Starting the Timer*/
    (void)rex_set_timer(&mmgsdi_log_pkt_timer, MMGSDI_LOG_PKT_TIMER_VALUE);

#ifndef FEATURE_UIM_TEST_FRAMEWORK
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(mmgsdi_log_pkt_ptr,
                                       sizeof(mmgsdi_log_pkt_data_type));
#endif /* !FEATURE_UIM_TEST_FRAMEWORK */
  }
} /*mmgsdi_log_pkt_init*/


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_GET_TIME

   DESCRIPTION:
     This function get the system time and encodes it in below format.
     --------------------------------------------------------------------------
     |  Year     | Month |  Day  |  Hour |  Minutes  |  Sec  |   Miliseconds  |
     |  (LSB)    |       |       |       |           |       |   (MSB)        |
     --------------------------------------------------------------------------
     |1Byte      |1Byte  |1Byte  |1Byte  |1 Byte     |1Byte  |2Bytes          |
     |number of  |number |number |number |number of  |number |number of       |  
     |years since|of     |of     |of     |minutes    |of     |miliseconds     |
     |1980       |months |days   |hours  |           |seconds|                |
     --------------------------------------------------------------------------

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
static void mmgsdi_log_pkt_get_time(
  uint64 * timestamp_ptr
)
{
  time_type        time_ms         = {0};
  uint64           temp_time_in_ms = 0;
  uint8            encoded_time[8] = {0};
  uint16           milisec         = 0;
  time_julian_type jul_tstamp;

  if(timestamp_ptr == NULL)
  {
    return;
  }

  memset(&jul_tstamp, 0x00, sizeof(time_julian_type));

  /*Get Time in MS*/
  time_get_ms(time_ms);
  temp_time_in_ms = (((uint64)time_ms[1]) << 32) | ((uint64)time_ms[0]);

  /*Convert time to julian date*/
  time_jul_from_secs((uint32)(temp_time_in_ms/1000), &jul_tstamp);
  milisec = (uint16) (temp_time_in_ms % 1000);

  /* Number of years since 1980 (0x7BC)*/
  if(jul_tstamp.year > 0x07BC)
  {
    encoded_time[0] = (uint8)(jul_tstamp.year - 0x07BC); 
  }

  encoded_time[1] = (uint8)jul_tstamp.month;
  encoded_time[2] = (uint8)jul_tstamp.day;;
  encoded_time[3] = (uint8)jul_tstamp.hour; 
  encoded_time[4] = (uint8)jul_tstamp.minute;
  encoded_time[5] = (uint8)jul_tstamp.second;
  encoded_time[6] = (uint8)(milisec >> 8);
  encoded_time[7] = (uint8)(0x00FF & milisec);

  mmgsdi_memscpy(timestamp_ptr, sizeof(uint64), encoded_time, sizeof(encoded_time));
} /* mmgsdi_log_pkt_get_time */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_NV_ITEMS

   DESCRIPTION:
     This is the logging function for the NV values,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_nv_items
(
  void
)
{
  mmgsdi_nv_item_cache_type nv_4398          = {0};
  mmgsdi_nv_item_cache_type nv_855           = {0};
  mmgsdi_nv_context_type    context_itr      = MMGSDI_NV_CONTEXT_PRIMARY;
  uint64                    temp_timestamp   = 0;

  rex_enter_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);

  do
  {
    /* Check global variable to avoid storing information after the packet is already sent */
    if(mmgsdi_log_pkt_ptr == NULL)
    {
      break;
    }

    /*Extracting And Packing Information*/

    /*NV 4398*/

    /*Setting value to 0xff if unable to read*/
    if(mmgsdi_nv_get_item(NV_UIM_SELECT_DEFAULT_USIM_APP_I, &nv_4398,
                          MMGSDI_NV_CONTEXT_DEVICE_SPECIFIC) == NV_DONE_S)
    {
      mmgsdi_log_pkt_ptr->after_nv_read.nv_4398 =
        nv_4398.item_value.uim_select_default_usim_app;
    }
    else
    {
      mmgsdi_log_pkt_ptr->after_nv_read.nv_4398 = 0xff;
    }


    /*NV 855: Support for subscription specific by 
      using index 0 for SUB0 and index 1 for SUB1.*/
    for(context_itr = MMGSDI_NV_CONTEXT_PRIMARY; context_itr < MMGSDI_NV_CONTEXT_TERTIARY; context_itr++)
    {
      /*Setting default value if unable to read*/
      if(mmgsdi_nv_get_item(NV_RTRE_CONFIG_I, &nv_855,
                            context_itr) == NV_DONE_S)
      {
        mmgsdi_log_pkt_ptr->after_nv_read.nv_855[context_itr] = (uint8)nv_855.item_value.rtre_config;
      }
      else
      {
        mmgsdi_log_pkt_ptr->after_nv_read.nv_855[context_itr] = 0xff;
      }
      mmgsdi_log_pkt_ptr->after_nv_read.set_nv_855[context_itr] = 0;

      /*NV feature USE_3GPD Primary*/
      if(mmgsdi_nv_get_slot_feature_status(
         MMGSDI_FEATURE_USE_3GPD_CREDENTIALS_FROM_NV,
         context_itr) == MMGSDI_FEATURE_ENABLED)
      {
        mmgsdi_log_pkt_ptr->after_nv_read.nv_use_3gpd[context_itr] = 1;
      }
    }

    /*NV feature USE_SFI*/
    if(mmgsdi_nv_get_device_feature_status(
         MMGSDI_FEATURE_USE_SFI) == MMGSDI_FEATURE_ENABLED)
    {
      mmgsdi_log_pkt_ptr->after_nv_read.nv_use_sfi = 1;
    }

    /*NV SKIP_PROV_APP_TERMINATION*/
    if(mmgsdi_nv_get_device_feature_status(
         MMGSDI_FEATURE_SKIP_PROV_APP_TERMINATION) == MMGSDI_FEATURE_ENABLED)
    {
      mmgsdi_log_pkt_ptr->after_nv_read.nv_skip_prov_app_termination = 1;
    }

    /*NV MULTISIM_AUTO_PROVISIONING*/
    if(mmgsdi_nv_get_device_feature_status(
         MMGSDI_FEATURE_MULTISIM_AUTO_PROVISIONING) == MMGSDI_FEATURE_ENABLED)
    {
      mmgsdi_log_pkt_ptr->after_nv_read.nv_feature_multisim_auto_provisioning = 1;
    }

    /*NV IGNORE_INIT_ERROR*/
    if(mmgsdi_nv_get_device_feature_status(
         MMGSDI_FEATURE_IGNORE_INIT_ERROR) == MMGSDI_FEATURE_ENABLED)
    {
      mmgsdi_log_pkt_ptr->after_nv_read.nv_ignore_init_error = 1;
    }

    /* We need to use temporary variable because timestamp is inside packed
       structure and its pointer might not be properly aligned */
    mmgsdi_log_pkt_get_time(&temp_timestamp);
    mmgsdi_log_pkt_ptr->after_nv_read.timestamp = temp_timestamp;
  }while(0);

    rex_leave_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);
}/* mmgsdi_log_pkt_nv_items */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_RTRE_UPDATE

   DESCRIPTION:
     This is the logging function to update rtre NV value,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_rtre_update
(
  mmgsdi_attribute_enum_type  attr_type
)
{
  mmgsdi_nv_item_cache_type     nv_855        = {0};
  mmgsdi_session_type_enum_type session_type  = MMGSDI_1X_PROV_PRI_SESSION;
  mmgsdi_return_enum_type       mmgsdi_status = MMGSDI_SUCCESS;
  mmgsdi_nv_context_type        nv_context    = MMGSDI_NV_CONTEXT_INVALID;

  if (attr_type >= MMGSDI_LOG_PKT_MAX_SLOT)
  {
    return;
  }

  rex_enter_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);

  do
  {
    /* Check global variable to avoid storing information after the packet is already sent */
    if(mmgsdi_log_pkt_ptr == NULL)
    {
      break;
    }

    mmgsdi_status = mmgsdi_util_get_session_type_from_tech_and_attr(MMGSDI_TECH_3GPP2,
                                                                    attr_type,
                                                                    &session_type);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    mmgsdi_status = mmgsdi_util_get_efs_item_index(MMGSDI_INVALID_SESSION_ID,
                                                   &session_type, &nv_context);

    /*Extracting And Packing Information

      NV 855:currently this NV is for primary session so copying the value to index 0. 
      In future it can be expanded to allow support for subscription specific by 
      using index 0 for SUB0 and index 1 for SUB1.

      Setting default value if unable to read*/
    if (nv_context == MMGSDI_NV_CONTEXT_PRIMARY || nv_context == MMGSDI_NV_CONTEXT_SECONDARY)
    {
      if(mmgsdi_nv_get_item(NV_RTRE_CONFIG_I, &nv_855, nv_context) == NV_DONE_S)
      {
        mmgsdi_log_pkt_ptr->after_nv_read.nv_855[nv_context] = (uint8)nv_855.item_value.rtre_config;
      }
      else
      {
        mmgsdi_log_pkt_ptr->after_nv_read.nv_855[nv_context] = 0xff;
      }
      mmgsdi_log_pkt_ptr->after_nv_read.set_nv_855[nv_context] = 1;
    }
  }while(0);

    rex_leave_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);
}/* mmgsdi_log_pkt_rtre_update */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_LINK_ESTABLISHED

   DESCRIPTION:
     This is the logging function for the link established ,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_link_established
(
  const mmgsdi_notify_link_est_req_type* req_ptr
)
{
  uint8                          slot_index;
  uint64                         temp_timestamp = 0;

  if(req_ptr == NULL)
  {
    return;
  }

  rex_enter_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);

  do
  {
    /* Check global variable to avoid storing information after the packet is already sent */
    if(mmgsdi_log_pkt_ptr == NULL)
    {
      break;
    }

    /*Extracting And Packing Information*/
    if(mmgsdi_util_get_slot_index (req_ptr->request_header.slot_id,
                                   &slot_index) != MMGSDI_SUCCESS || 
       slot_index >= MMGSDI_LOG_PKT_MAX_SLOT)
    {
      break;
    }

    mmgsdi_log_pkt_ptr->link_est_req[slot_index].slot_id =
      (req_ptr->request_header.slot_id & 0xff);
    mmgsdi_log_pkt_ptr->link_est_req[slot_index].protocol =
      (req_ptr->link_est_data.protocol & 0xff);

    /* We need to use temporary variable because timestamp is inside packed
       structure and its pointer might not be properly aligned */
    mmgsdi_log_pkt_get_time(&temp_timestamp);
    mmgsdi_log_pkt_ptr->link_est_req[slot_index].timestamp = temp_timestamp;
  }while(0);
  
    rex_leave_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);
}/* mmgsdi_log_pkt_link_established */


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_mmgsdi_event

   DESCRIPTION:
     This is the logging function for the mmgsdi events ,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_mmgsdi_event
(
  const mmgsdi_event_data_type * event_ptr
)
{
  uint64                         time_ms_com;
  uint8                          session_index;
  mmgsdi_session_type_enum_type  session_type;
  uint8                          slot_index;
  mmgsdi_slot_id_enum_type       slot_id;

  if(event_ptr == NULL)
  {
    return;
  }

  rex_enter_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);

  do
  {
    /* Check global variable to avoid storing information after the packet is already sent */
    if(mmgsdi_log_pkt_ptr == NULL)
    {
      break;
    }

    /*Get Time in MS*/
    mmgsdi_log_pkt_get_time(&time_ms_com);

    switch(event_ptr->evt)
    {
      case MMGSDI_CARD_INSERTED_EVT:
      {
        uint32 index = 0;

        /*Extracting And Packing Information*/
        if(mmgsdi_util_get_slot_index(event_ptr->data.card_inserted.slot,
                                      &slot_index) != MMGSDI_SUCCESS || 
           slot_index >= MMGSDI_LOG_PKT_MAX_SLOT)
        {
          break;
        }

        mmgsdi_log_pkt_ptr->card_inserted_evt[slot_index].slot_id =
          (event_ptr->data.card_inserted.slot & 0xff);
        mmgsdi_log_pkt_ptr->card_inserted_evt[slot_index].ef_dir_number_of_apps = 
          (event_ptr->data.card_inserted.num_aids_avail & 0xff);

        mmgsdi_log_pkt_ptr->card_inserted_evt[slot_index].timestamp = time_ms_com;

        for(index = 0;
            index < event_ptr->data.card_inserted.num_aids_avail && index < MMGSDI_LOG_PKT_MAX_APP;
            index++)
        {
          mmgsdi_log_pkt_ptr->card_inserted_evt[slot_index].app_type[index] =
            (event_ptr->data.card_inserted.aid_info[index].app_type  &  0xff);
        }
        break;
      }

      case MMGSDI_SESSION_CHANGED_EVT:
      {
        mmgsdi_app_enum_type      app_type         = MMGSDI_APP_NONE;
        mmgsdi_channel_info_type *channel_info_ptr = NULL;

        /*Extracting And Packing Information*/
        if(mmgsdi_util_get_session_type(event_ptr->session_id, &session_type,
                                        NULL) != MMGSDI_SUCCESS)
        {
          break;
        }

        session_index = mmgsdi_log_pkt_get_session_index(session_type);

        /*Break on invalid session index*/
        if(session_index >= MMGSDI_LOG_PKT_MAX_PROV_SESSION)
        {
          break;
        }

        mmgsdi_log_pkt_ptr->session_changed_evt[session_index].session_changed_status = 
          event_ptr->data.session_changed.activated;
        app_type = event_ptr->data.session_changed.app_info.app_data.app_type;
        mmgsdi_log_pkt_ptr->session_changed_evt[session_index].slot_id =
          (event_ptr->data.session_changed.app_info.slot & 0xff);

        if(mmgsdi_util_get_session_and_channel_info(
             event_ptr->session_id, NULL,
             &channel_info_ptr) != MMGSDI_SUCCESS)
        {
          break;
        }

        mmgsdi_log_pkt_ptr->session_changed_evt[session_index].channel_id =
          channel_info_ptr->channel_id;
        mmgsdi_log_pkt_ptr->session_changed_evt[session_index].session_type =
          session_index;
        mmgsdi_log_pkt_ptr->session_changed_evt[session_index].app_type =
          (app_type & 0xff);
        mmgsdi_log_pkt_ptr->session_changed_evt[session_index].num_connections =
          (channel_info_ptr->num_connections & 0xff);
        mmgsdi_log_pkt_ptr->session_changed_evt[session_index].timestamp = time_ms_com;
        break;
      }

      case MMGSDI_PIN1_EVT:
      {
        /*Extracting And Packing Information*/
        if(mmgsdi_util_get_session_type(event_ptr->session_id, &session_type,
                                        NULL) != MMGSDI_SUCCESS)
        {
          break;
        }

        session_index = mmgsdi_log_pkt_get_session_index(session_type);

        /*Break on invalid session index*/
        if(session_index >= MMGSDI_LOG_PKT_MAX_PROV_SESSION)
        {
          break;
        }

        /*Packing the information in the struct*/
        mmgsdi_log_pkt_ptr->pin1_evt[session_index].slot_id =
          (event_ptr->data.pin.slot & 0xff);
        mmgsdi_log_pkt_ptr->pin1_evt[session_index].pin_status =
          (event_ptr->data.pin.pin_info.status & 0xff);
        mmgsdi_log_pkt_ptr->pin1_evt[session_index].session_type =
          session_index;
        mmgsdi_log_pkt_ptr->pin1_evt[session_index].pin_id = 
          (event_ptr->data.pin.pin_info.pin_id & 0xff);
        mmgsdi_log_pkt_ptr->pin1_evt[session_index].timestamp = time_ms_com;
        break;
      }

      case MMGSDI_PERSO_EVT:
      {
        /*Extracting And Packing Information*/
        if(mmgsdi_util_get_session_type(event_ptr->session_id, &session_type, 
                                        NULL) != MMGSDI_SUCCESS)
        {
          break;
        }

        session_index = mmgsdi_log_pkt_get_session_index(session_type);

        /*Break on invalid session index*/
        if(session_index >= MMGSDI_LOG_PKT_MAX_PROV_SESSION)
        {
          break;
        }

        if(mmgsdi_util_get_prov_session_info(event_ptr->session_id, NULL,
                                             &slot_id, NULL) != MMGSDI_SUCCESS)
        {
          break;
        }

        /*Packing the information in the struct*/
        mmgsdi_log_pkt_ptr->perso_evt[session_index].perso_status =
          (event_ptr->data.perso.status & 0xff);
        mmgsdi_log_pkt_ptr->perso_evt[session_index].feature =
          (event_ptr->data.perso.feature & 0xff);
        mmgsdi_log_pkt_ptr->perso_evt[session_index].session_type =
          session_index;
        mmgsdi_log_pkt_ptr->perso_evt[session_index].slot_id =
          (slot_id & 0xff);
        mmgsdi_log_pkt_ptr->perso_evt[session_index].timestamp = time_ms_com;
        break;
      }

      case MMGSDI_SUBSCRIPTION_READY_EVT:
      {
        /*Extracting And Packing Information*/
        if(mmgsdi_util_get_session_type(event_ptr->session_id, &session_type,
                                        NULL) != MMGSDI_SUCCESS)
        {
          break;
        }

        session_index = mmgsdi_log_pkt_get_session_index(session_type);

        /*Break on invalid session index*/
        if(session_index >= MMGSDI_LOG_PKT_MAX_PROV_SESSION)
        {
          break;
        }

        /*Packing the information in the struct*/
        mmgsdi_log_pkt_ptr->sub_ready_evt[session_index].slot_id =
          (event_ptr->data.subscription_ready.app_info.slot & 0xff);
        mmgsdi_log_pkt_ptr->sub_ready_evt[session_index].session_type =
          session_index;
        mmgsdi_log_pkt_ptr->sub_ready_evt[session_index].timestamp = time_ms_com;
        break;
      }

      case MMGSDI_CARD_ERROR_EVT:
      {
        /*Extracting Information*/
        if(mmgsdi_util_get_slot_index(event_ptr->data.card_error.slot,
                                      &slot_index) != MMGSDI_SUCCESS || 
           slot_index >= MMGSDI_LOG_PKT_MAX_SLOT)
        {
          break;
        }

        /*Packing the information in the struct*/
        mmgsdi_log_pkt_ptr->card_error_evt[slot_index].slot_id =
          (event_ptr->data.card_error.slot & 0xff);
        mmgsdi_log_pkt_ptr->card_error_evt[slot_index].card_err_cond =
          (event_ptr->data.card_error.info & 0xff);
        mmgsdi_log_pkt_ptr->card_error_evt[slot_index].timestamp = time_ms_com;
        break;
      }

      default:
        break;
    }
  }while(0);

  rex_leave_crit_sect(&mmgsdi_log_pkt_cache_crit_sect);
}/* mmgsdi_log_pkt_mmgsdi_event */

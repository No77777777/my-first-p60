/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                P S _  I F A C E _ I P F L T R _ L O G G I N G . C

GENERAL DESCRIPTION
- This file defines the APIs to log all the filter related info on qxdm log packets

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_iface_ipfltr_logging.c#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $
--------    ---    ----------------------------------------------------------
05/08/15    vrk    Created file
===========================================================================*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                                INCLUDE FILES

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"

#include <stringl/stringl.h>

#include "ps_iface.h"
#include "ps_utils.h"
#include "ps_crit_sect.h"
#include "ps_iface_ipfltr.h"
#include "ps_iface_logging.h"
#include "ps_iface_ipfltr_logging.h"
#include "ps_logging_helper.h"
#include "ps_qos_spec_logging.h"
#include "ps_iface_ipfltr_hw.h"

#include "ps_byte.h"
#include "log.h"
#include "log_codes.h"

#include <stdio.h>
#include "ds_Utils_DebugMsg.h"

/*===========================================================================

                           LOCAL DATA DECLARATIONS

===========================================================================*/
#define PS_FILTER_INFO_LOG_PKT_VERSION 1

/*-------------------------------------------------------------------------
  IFACE FILTER PACKET LOG packet Specification 
  Format can also be found in ICD docuemnt 80-V1294-1
-------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
steps   Params
  1 |uint32     version:8;    ///< Log packet version 0-255
  2 |ps_filter_event_type_enum event:8; ///< Filter event type 
  3 |uint8  iface_instance; ///< Iface instnace 
  5 |uint16 iface_name; ///< Iface Name , this will be set to 0x0 while sending the global filters applicable to all ifaces
  6 |  uint8  num_active_filter_clients; ///< numer of active filter clients 
------------------------------------------------------------------------------
-------Array starts---- ==> client_filters[i] where 0 <= i < num_active_filter_clients <==
           ps_client_filter_info_type client_filters[MAX_IP_FLTR_LOGGING_CLIENT]; ///< client type filters 
------------------------------------------------------------------------------
  7|  ip_filter_client_enum_type client_type:8; ///< client type which installs filters 
  8|  uint8 num_client_filters; ///< Number of filters installed by client 
  ----------------------------------------------------------------------
  -------Array starts---- ==>  filter[i] where 0 <= i < num_client_filters <==-----
           ps_client_filter_info_type client_filters[MAX_IP_FLTR_CLIENT]; ///< client type filters 
  ----------------------------------------------------------------------
  9|  uint8                 is_disabled;///< enable 0, disable 1
      ip_filter_type--->> Same structure of unions as in the QOS_SPEC_LOGGING...
  ----------------------------------------------------------------------
  First six fields are mandatory and min pkt length
---------------------------------------------------------------------------*/

/*currently we are supporting only socket & powersave filters 
    so num_active_filter_clients = 2 & max we can push is 4*/
#define MAX_IP_FLTR_LOGGING_CLIENT 4
#define MAX_CLIENT_FILTER_NUM 24

/*Till step 6 in above Log packet spec*/
#define MIN_FILTER_INFO_LOG_PKT_LEN  (6 + LOG_HDR_SIZE)
/*step 7,8 in above Log packet spec*/
#define MIN_CLIENT_FILTER_INFO_LEN  2
/*step 9 in above Log packet spec*/
#define FILTER_SPEC_INFO_LEN  (1 + sizeof(ip_filter_type))

#define MAX_FILTER_LOG_PKT_LEN ( MIN_FILTER_INFO_LOG_PKT_LEN +              \
             (MAX_IP_FLTR_LOGGING_CLIENT * (MIN_CLIENT_FILTER_INFO_LEN +    \
              MAX_CLIENT_FILTER_NUM * FILTER_SPEC_INFO_LEN)) )

#define PS_IFACE_IPFLTR_LOG_VALID_FILTERS(client_mask, client_id)           \
              ((client_mask & (1 << client_id)) ? TRUE : FALSE)

#define PS_IFACE_IPFLTR_LOG_GET_VALID_IFACE_TYPE(iface_ptr)                 \
     ((iface_ptr->name == CDMA_SN_IFACE || iface_ptr->name == UMTS_IFACE || \
       iface_ptr->name == SIO_IFACE || iface_ptr->name == EPC_IFACE ) ?     \
      TRUE : FALSE)

#define PS_IFACE_IPFLTR_LOG_GET_NUM_CLIENT_FILTERS(iface_ptr, client)       \
             (iface_ptr == NULL) ? q_cnt(&global_ipfltr_info[client]) :     \
                     q_cnt(&((iface_ptr)->iface_private.ipfltr_info[client]))

typedef enum
{
  LOG_ERR_NONE,
  LOG_ERR_VALIDATION_FAILED,
  LOG_ERR_NO_CLIENTS_TO_LOG,
  LOG_ERR_NOTHING_TO_LOG,
  LOG_ERR_PKT_SIZE_EXCEED,
  LOG_ERR_PKT_ALLOC_FAILED
}ps_iface_ipfltr_log_err_info_type;

/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/

/** 
       ps_iface_ipfilteri_log_is_valid_event
  @brief  This funciton will check given event is valid or not

  @param[in] log_filter_event - Type of the event where this logging is done.
  @param[out] boolean Possible values are
  @return  TRUE               On Success.
  @return  FALSE              On failure.
*/

static boolean ps_iface_ipfilteri_log_is_valid_event
(
  ps_iface_ipfltr_log_event_enum_type log_filter_event 
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (log_filter_event ==  LOG_FILTER_ADD_EV ||
      log_filter_event ==  LOG_FILTER_DELETE_EV ||
      log_filter_event ==  LOG_FILTER_ON_DEMAND_EV )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/** 
       PS_IFACE_IPFLTR_LOG_ALL_FILTERS_PER_IFACE
  @brief  This funciton will log filter info related each iface 
  @param[in] log_filter_event   Type of the event where this logging is done
  @param[in] clients_mask      specific clients filters to be logged.

    Refer to the Filter log packet spec at top of file for byte order

  @return None
*/
static void ps_iface_ipfltr_log_all_filters_per_iface
(
  ps_iface_type * iface_ptr,
  ps_iface_ipfltr_log_event_enum_type log_filter_event,
  ps_iface_ipfltr_client_mask_type    client_mask
)
{
   uint8   * log_ptr = NULL;
   uint8   * resp_ptr = NULL;
   uint8     num_active_clients, num_client_filters, num_filters;
   uint16    log_pkt_len = 0;
   int32     ret_val = 0; 
   int32     client_id = 0;
   int32     index = 0;
   q_type                   * q_ptr = NULL;
   ps_iface_ipfilteri_type  * filter_buf_ptr = NULL;
   ps_iface_ipfltr_log_err_info_type err_val = LOG_ERR_NONE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
      Refer to the Filter log packet spec at top of file for byte order
  -------------------------------------------------------------------------*/
  do
  {
    /*-------------------------------------------------------------------------
        Log Filter info only if log code is enabled
     -------------------------------------------------------------------------*/
     if (!(log_status(LOG_PS_FILTER_INFO_C) &&
           ps_iface_ipfilteri_log_is_valid_event(log_filter_event)))
     {
       err_val = LOG_ERR_VALIDATION_FAILED;
       break;
     }

     if(client_mask == 0)
     {
       err_val = LOG_ERR_NO_CLIENTS_TO_LOG;
       break;
     }
     /*Change the requested mask,
           to log only currently supported filter client mask*/
     client_mask &= LOG_ALL_SUPPORTED_FLTR_CLIENTS;

   /*-----------------------------------------------------------------------
       Find how much memory needs to be allocated for log pkt
   -----------------------------------------------------------------------*/
      /*Loop through all the filter clients and 
            find total number of fitlers to log*/
     for( client_id = 0, num_filters = 0, 
           num_active_clients = 0, num_client_filters = 0;
          client_id < IP_FLTR_CLIENT_MAX;
          client_id++)
     {
       if( !PS_IFACE_IPFLTR_LOG_VALID_FILTERS(client_mask, client_id))
       {
         continue;
       }
       /*Only MAX_CLIENT_FILTER_NUM clients can be supported per log packet*/
       num_client_filters = MIN(MAX_CLIENT_FILTER_NUM,
               PS_IFACE_IPFLTR_LOG_GET_NUM_CLIENT_FILTERS(iface_ptr, client_id));
       if(num_client_filters > 0)
       { 
         num_active_clients++;
         num_filters += num_client_filters;
       }
       num_client_filters = 0;
     }

     if (num_filters == 0 || num_active_clients > MAX_IP_FLTR_LOGGING_CLIENT)
     {
        err_val = LOG_ERR_NOTHING_TO_LOG;
        break;
     }

     log_pkt_len = ( MIN_FILTER_INFO_LOG_PKT_LEN + 
                    (num_active_clients * MIN_CLIENT_FILTER_INFO_LEN) + 
                    (num_filters * FILTER_SPEC_INFO_LEN) );

     if( (log_pkt_len == 0) || (log_pkt_len > MAX_FILTER_LOG_PKT_LEN) )
     {
        err_val = LOG_ERR_PKT_SIZE_EXCEED;
        break;
     }

     /*Allocate the log packet with calculated size*/
     resp_ptr = log_ptr = (uint8 *) log_alloc(LOG_PS_FILTER_INFO_C,
                                          log_pkt_len);
     if (log_ptr == NULL)
     {
        err_val = LOG_ERR_PKT_ALLOC_FAILED;
        break;
     }

     /*Start pushing the info bytes by bytes as per the 
            Log packet spec @refer top of file.*/
     log_ptr += LOG_HDR_SIZE;
     /*Leaving the header size memset rest of memory*/
     memset(log_ptr, 0x0, (log_pkt_len - LOG_HDR_SIZE));
     
     log_ptr = put8(log_ptr, PS_FILTER_INFO_LOG_PKT_VERSION);

     log_ptr = put8(log_ptr, (uint8)log_filter_event);

     /*Iface will be NULL if we are logging global filters, 
          in that case fill zeros else iface name*/
     if(iface_ptr != NULL)
     {
       log_ptr = put8(log_ptr, (uint8)iface_ptr->instance);
       ret_val = memscpy((char *) log_ptr,
                         sizeof(uint16),
                         (char *) &iface_ptr->name,
                         sizeof(uint16));
       log_ptr += ret_val;
     }
     else
     {
       log_ptr = put8(log_ptr, 0);
       log_ptr = put16(log_ptr, 0);
     }

     /*Push number of Active filter clients.
           This would be the array length for client list */
     log_ptr = put8(log_ptr, num_active_clients);

    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
    
    /*Loop through all the filter clients and 
           push all filter info of each clientt */
     for( client_id = 0, num_client_filters = 0;
          client_id < IP_FLTR_CLIENT_MAX;
          client_id++)
     {
       if( !PS_IFACE_IPFLTR_LOG_VALID_FILTERS(client_mask, client_id))
       {
         continue;
       }
       /*MAX_CLIENT_FILTER_NUM filters can only be logged per client */
       num_client_filters = MIN(MAX_CLIENT_FILTER_NUM, 
               PS_IFACE_IPFLTR_LOG_GET_NUM_CLIENT_FILTERS(iface_ptr, client_id));

       /*Only log filters if this client have some*/
       if(num_client_filters == 0)
       {
         continue;
       }
       
       /*Push client type & number of filters of this client. 
                This would be the array length for  filter list */
       log_ptr = put8(log_ptr, (uint8)client_id);
       log_ptr = put8(log_ptr, num_client_filters);
       
       /*if Iface is NULL then log global filters else log filters on iface*/
       if (iface_ptr == NULL)
       {
         q_ptr = &(global_ipfltr_info[client_id]);
       }
       else if (PS_IFACE_IS_VALID(iface_ptr))
       {
         q_ptr = &(iface_ptr->iface_private.ipfltr_info[client_id]);
       }

       /*Loop through all the filters of this client and 
                them one by one till num_client_filters*/
       for (filter_buf_ptr = q_check(q_ptr), index = 0;
            filter_buf_ptr != NULL && index < num_client_filters;
            filter_buf_ptr = q_next(q_ptr, &(filter_buf_ptr->link)), index++)
       {
          /*Push the status of the filter whether it is enabled or disabled*/
          log_ptr = put8(log_ptr, filter_buf_ptr->disabled);

          /*This utility function will write the filter spec into log_ptr*/
          ps_qsli_fill_fltr_param(&log_ptr, 
                            &filter_buf_ptr->filter.fi_default);
       }
       num_client_filters = 0;
     }
          
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

     DS_UTILS_ASSERT((log_ptr - resp_ptr) <= log_pkt_len);
     /*-----------------------------------------------------------------------
       Since all fields of ip_fltr may not have been specified by
       an application, logged bytes could be less than log_pkt_len
     -----------------------------------------------------------------------*/
     log_shorten(resp_ptr, (uint32) (log_ptr - resp_ptr));
     log_commit(resp_ptr);
  }while(0);

   /*avoid printing log for ifaces with no filters as we loop all*/
  if(err_val != LOG_ERR_NONE && err_val != LOG_ERR_NOTHING_TO_LOG)
  {
    LOG_MSG_ERROR_INT_3("ps_iface_ipfltr_log_all_filters_per_iface:"
                     " Loggin failed for event:%d"
                     "err_val %d, pkt_size: %d",log_filter_event, 
                     err_val, log_pkt_len);
  }
  return;
}

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/** 
  @brief   This funciton will log all filter related information on to the qxdm log packet

  @param[in] log_filter_event           Type of the event where this logging is done
  @param[in] clients_mask          specific clients filters to be logged
  
  @return None
 */
void ps_iface_ipfltr_log_all_filters_info
( 
  ps_iface_ipfltr_log_event_enum_type    log_filter_event,
  ps_iface_ipfltr_client_mask_type       client_mask
)
{
   int32   index;
   ps_iface_type * iface_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
     Log Filter info only if log code is enabled
  -------------------------------------------------------------------------*/
  if (! (log_status(LOG_PS_FILTER_INFO_C) && 
         ps_iface_ipfilteri_log_is_valid_event(log_filter_event)))
  {
    return;
  }

   /*Loop through all the ifaces and log filters if it valid enough*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
    for (index = 0;
         index < MAX_SYSTEM_IFACES && global_iface_ptr_array[index] != 0;
         index++)
    {
      iface_ptr = global_iface_ptr_array[index];
      /*Log filters for  3gpp/3gpp2/Rmnet/EPC ifaces which are 
                   valid & in UP/Routable state */
      if (! ( PS_IFACE_IS_VALID(iface_ptr) &&
              PS_IFACE_IPFLTR_LOG_GET_VALID_IFACE_TYPE(iface_ptr) )
         )
      {
        continue;
      }
      /*Log all filters for this IFACE*/
      ps_iface_ipfltr_log_all_filters_per_iface(iface_ptr, log_filter_event,
                                                client_mask);
    }
  PS_LEAVE_CRIT_SECTION (&global_ps_crit_section);

  /*Log the global Iface filters also*/
  ps_iface_ipfltr_log_all_filters_per_iface(NULL, log_filter_event,
                                            client_mask);

  /*call IPA API to log HW filters for on_demand only as 
          rest of the events are already logged in IPA*/
  if(log_filter_event == LOG_FILTER_ON_DEMAND_EV)
  { 
    (void) ps_iface_ipfltr_hw_log_filters();
  }
  
  return;
}


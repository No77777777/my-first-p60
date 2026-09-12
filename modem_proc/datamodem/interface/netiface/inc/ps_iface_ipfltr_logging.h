#ifndef PS_IFACE_IPFLTR_LOGGING_H
#define PS_IFACE_IPFLTR_LOGGING_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               P S _  I F A C E _ I P F L T R _ L O G G I N G . H


GENERAL DESCRIPTION
- This file provides the APIs to log all the filter related info on qxdm log packets

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None
  
  Copyright (c) 2015 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $PVCSPath:  $
  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/inc/ps_iface_ipfltr_logging.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $
when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/12/15    vrk    Created file
===========================================================================*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                                INCLUDE FILES

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"

#include "ps_iface_defs.h"

/*===========================================================================

                           LOCAL DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

typedef enum
{
  LOG_FILTER_ADD_EV, /*Log Filter addition event*/
  LOG_FILTER_DELETE_EV,/*Log Filter deletion event*/
  LOG_FILTER_ON_DEMAND_EV /*Log On demand from application*/
} ps_iface_ipfltr_log_event_enum_type;

/**
  Filter Client Mask for Logging. This mask is equivalent to the value of 
    each client type corresponding to ps_iface_ipfltr_client_id_enum_type
 */
/*0 - IP_FLTR_CLIENT_SOCKETS*/
#define LOG_FLTR_CLIENT_SOCKETS               0x01
/*4 - IP_FLTR_CLIENT_POWERSAVE*/
#define LOG_FLTR_CLIENT_POWERSAVE             0x10

/*This mask is 'OR' of all currently supported clients */
/*This will Log all current supported filter clients*/
#define LOG_ALL_SUPPORTED_FLTR_CLIENTS       0x11

typedef uint16 ps_iface_ipfltr_client_mask_type;

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
);

#endif /* PS_IFACE_IPFLTR_LOGGING_H */

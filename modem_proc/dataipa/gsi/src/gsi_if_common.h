/*!
  @file
  gsi_if_common.h

  @brief
  This file contains declaration of all the of the GSI driver's IF common (not 
  protocol-specific) operations.


*/
/*===========================================================================

  Copyright (c) 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/17   tz      8-Bytes TRE support
03/22/16   tz      Support MCS inter-EE commands
03/12/15   tz      GSI test-framework changes
03/03/15   tz      Initial version
===========================================================================*/
#ifndef GSI_IF_COMMON_H
#define GSI_IF_COMMON_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <comdef.h>
#include "gsi_hw_common.h"
#include "gsi.h"
#include "gsi_core.h"
#include "gsi_osal.h"

/*==============================================================================

                      Macros

==============================================================================*/

#define GSI_IF_MAX_CLIENT_NUM 4
#define GSI_IF_MAX_ALLOCATED_CHANNELS 32
#define GSI_IF_MAX_ALLOCATED_EVENTS 16

#define GSI_IF_INC_STATS(st) st++

#define GSI_IF_CLIENT_NUM_VALID_CHECK(client_num) ((client_num < GSI_IF_MAX_CLIENT_NUM) && \
  ipa_if_ctl.gsi_if_client[client_num] != NULL)

#define GSI_HDL_CLNT_MASK 0xFF
#define GSI_HDL_CLNT_SHIFT 0x0
#define GSI_HDL_CHID_MASK 0xFF
#define GSI_HDL_CHID_SHIFT 0x8
#define GSI_IF_GENERATE_GSI_CH_HANDLE(clnt, chid) (gsi_ch_handle_t)ipa_if_ctl.gsi_if_client[clnt]->channels[chid]
#define GSI_IF_GET_CHID_FROM_HDL(hdl) ((gsi_if_channel_s*)hdl)->chid
#define GSI_IF_GET_CLNT_FROM_HDL(hdl) ((gsi_if_channel_s*)hdl)->clnt_id
#define GSI_IF_CH_HANDLE_VALID_CHECK(ch_hdl) (ch_hdl != NULL) 
/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/



/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/
#define GSI_IF_LOG_STR_MAX_LEN 140

typedef void (*gsi_if_ch_callback_func_type)(gsi_core_event_info_s* event_info, uint32 user_data);

typedef struct {
  char str[GSI_IF_LOG_STR_MAX_LEN];
}gsi_if_stats_log_entry;


typedef struct gsi_if_client_item_s gsi_if_client_item_s;

/*!@brief GPI IF channel event statistics structure */
typedef struct {
  uint64 event_eot;
  uint64 event_eob;
  uint64 event_overflow;
  uint64 total_transfer_events;
  uint64 total_nop_events;
  uint64 total_transfer_irqs;
}gsi_if_gpi_er_xfer_stats_s;

/*!@brief GPI IF channel event statistics structure */
typedef struct {
  gsi_if_gpi_er_xfer_stats_s xfer_events;
}gsi_if_gpi_er_stats_s;

typedef struct {
  /*! Global event statistics */
  uint32 st;
}gsi_if_common_error_event_stats_s;

/*!@brief GSI common event statistics structure */
typedef struct {
  uint32 total_global_errors;

  gsi_if_common_error_event_stats_s error_events;
}gsi_if_common_event_stats_s;

/*!@brief GSI common statistics structure */
typedef struct {
  /*! Event statistics */
  gsi_if_common_event_stats_s event_stats;
}gsi_if_common_stats_s;

/*!@brief GSI client control structure */
typedef struct {
  /*! Client configurations */
  gsi_ee_cfg_s ee_cfg;
  /*! Allocated core EE handle */
  gsi_core_gsi_handle_t core_ee_handle;
}gsi_if_client_s;

/*!@brief GSI IF channel event statistics structure */
typedef struct {
  uint32 total_control_events;
}gsi_if_er_event_ctl_stats_s;

/*!@brief GSI IF channel event statistics structure */
typedef struct {
  gsi_if_er_event_ctl_stats_s control_events;
}gsi_if_er_event_stats_s;

/*!@brief GSI IF channel statistics structure */
typedef struct {
  gsi_if_er_event_stats_s events;
}gsi_if_er_stats_s;

/*!@brief GSI IF event  structure */
typedef struct  
{
  gsi_if_er_stats_s stats;
  uint32 ref_count;
  gsi_ev_common_cfg_s ev_cfg;
  gsi_core_ev_ch_cfg_s core_ev_ch_cfg;
  gsi_core_ev_handle_t ev_core_handle;
  void* protocol_info;
  gsi_callback_func_type ch_ctl_cb;
  uint32 user_data;
  gsi_if_client_item_s *client;
}gsi_if_event_ring_s;

/*!@brief GSI IF channel event statistics structure */
typedef struct {
  uint32 total_control_events;
}gsi_if_ch_event_ctl_stats_s;

/*!@brief GSI IF channel event statistics structure */
typedef struct {
  gsi_if_ch_event_ctl_stats_s control_events;
}gsi_if_ch_event_stats_s;

/*!@brief GSI IF channel statistics structure */
typedef struct {
  gsi_if_ch_event_stats_s events;
}gsi_if_ch_stats_s;

/*!@brief GSI IF common channel structure -
   will be used by GPI/MHI/XDCI handlers for holding common GSI 
   information. */
typedef struct {
  gsi_if_client_s *common_client_info;
  gsi_core_ch_handle_t ch_core_handle;
  gsi_callback_func_type ch_ctl_cb;
  gsi_core_tr_ch_cfg_s core_tr_ch_cfg;
  uint8 evid;
  gsi_ch_handle_t gsi_ch_handle;
  void* protocol_info;
  gsi_if_ch_stats_s stats;
  gsi_hw_channel_state_type_e state;
  uint32 chid;
  uint32 clnt_id;
  uint8 ch_init_done;
}gsi_if_channel_s;

/*!@brief GSI client control list item */
struct gsi_if_client_item_s{
  gsi_if_event_ring_s *events[GSI_IF_MAX_ALLOCATED_EVENTS];
  gsi_if_channel_s* channels[GSI_IF_MAX_ALLOCATED_CHANNELS];
  gsi_if_client_s client_cfg;
  gsi_if_common_stats_s common_stats;
};



/*!@brief GSI core control structure */
typedef struct {
  boolean is_driver_init; 
  GSI_OSAL_CRIT_SECT_T gsi_if_global_crit_sect;
  gsi_if_client_item_s* gsi_if_client[GSI_IF_MAX_CLIENT_NUM];
  boolean en_data_path_log;
  uint32 clnt_ref_cnt;
}gsi_if_ctl_s;

/*==============================================================================

                                FUNCTIONS

==============================================================================*/

/*==============================================================================

  FUNCTION:  gsi_gpi_xfer_completion_callback

==============================================================================*/
/*!
  @brief
  Handle completed transfers interrupt from GSI.

  @param 
  event_info, completed transfer information (originating ER)
  core_clnt_id, core client ID 

  @retval 

*/
/*============================================================================*/
void gsi_gpi_xfer_completion_callback(gsi_core_event_info_s* event_info, uint32 core_clnt_id);

/*==============================================================================

  FUNCTION:  gsi_if_common_get_client_info

==============================================================================*/
/*!
  @brief
  Get client information from client data-base.

  @param
  client_id, GSI client information to retrieve
  client_info - output parameter: handle to the information structure pointer
  to be filled by GSI driver.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_if_common_get_client_info
(
  gsi_handle_t  client_id,
  gsi_if_client_s **client_info
);

/*==============================================================================

  FUNCTION:  gsi_if_gpi_one_time_init

==============================================================================*/
/*!
  @brief
  One-time SW initialization for the GSI IF GPI driver 

  @param  

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
void gsi_if_gpi_one_time_init(void);

/*==============================================================================

  FUNCTION:  gsi_common_update_ch_ctx_channel

==============================================================================*/
/*!
  @brief
  Common update of Channel context for GSI channel. Protocol handler
  (MHI/xDCI/GPI) should supply 
  the channel specific parameters for this process.

  @param
  gsi_handle, GSI EE handle received through gsi_init_ee(). 
  chid, GSI CHID
  tr_ch_cfg, GSI channel configuration structure
  ch_protocol_info, channel user data for the protocol handler to be registered on 
    common data structure

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_common_update_ch_ctx_channel
(
  gsi_handle_t gsi_handle,
  uint8 chid,
  gsi_core_tr_ch_cfg_s *tr_ch_cfg
);

/*==============================================================================

  FUNCTION:  gsi_common_update_ev_ctx_channel

==============================================================================*/
/*!
  @brief
  Common update of event context for GSI event. Protocol handler
  (MHI/xDCI/GPI) should supply 
  the event specific parameters for this process.

  @param
  gsi_handle, GSI EE handle received through gsi_init_ee(). 
  evid, GSI event ID
  ev_ch_cfg, GSI event configuration structure

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_common_update_ev_ctx_channel
(
  gsi_handle_t gsi_handle, 
  uint8 evid,
  gsi_core_ev_ch_cfg_s *ev_ch_cfg
);

/*==============================================================================

  FUNCTION:  gsi_common_init_channel

==============================================================================*/
/*!
  @brief
  Common initialization for GSI channel. Protocol handler (MHI/xDCI/GPI) should supply 
  the channel specific parameters for this process.

  @param
  gsi_handle, GSI EE handle received through gsi_init_ee(). 
  evid, GSI EVID
  chid, GSI CHID
  ev_ch_cfg, GSI channel configuration structure
  tr_ch_cfg, GSI event configuration structure
  ch_protocol_info, channel user data for the protocol handler to be registered on common data structure
  ev_protocol_info, event user data for the protocol handler to be registered on common data structure
  ch_ctl_cb, channel callback to be invoked during channel control events
  gsi_ch_handle, output parameter: allocated GSI driver handle for this channel.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_common_init_channel 
(
  gsi_handle_t gsi_handle, 
  uint8 chid,
  uint32 evid,
  gsi_core_ev_ch_cfg_s* ev_ch_cfg,
  gsi_core_tr_ch_cfg_s* tr_ch_cfg, 
  void* ch_protocol_info,
  void* ev_protocol_info,
  gsi_callback_func_type ch_ctl_cb,
  gsi_ch_handle_t* gsi_ch_handle
);

gsi_if_gpi_er_stats_s* gpi_er_get_stats(uint32 erid);

/*==============================================================================

  FUNCTION:  gsi_common_deinit_channel

==============================================================================*/
/*!
  @brief
  Common de-initialization for GSI channel. 

  gsi_ch_handle, allocated GSI driver handle for this channel.

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_common_deinit_channel (gsi_ch_handle_t  gsi_ch_handle);

/*==============================================================================

  FUNCTION:  gsi_if_common_dump_ch_common

==============================================================================*/
/*!
  @brief
  Logs GSI channel common configuration

  @param
  ch_common, channel common configuration
*/
/*============================================================================*/
void gsi_if_common_dump_ch_common
(
  gsi_ch_common_cfg_s *ch_common
);

/*==============================================================================

  FUNCTION:  gsi_if_common_dump_ev_common

==============================================================================*/
/*!
  @brief
  Logs GSI event common configuration

  @param
  ev_common, event common configuration
*/
/*============================================================================*/
void gsi_if_common_dump_ev_common
(
  gsi_ev_common_cfg_s *ev_common
);

#endif /* GSI_IF_COMMON_H */

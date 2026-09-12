/*!
  @file
  ds_wcdma_throughput_hdlr.h

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.

*/


#ifndef __DS_WCDMA_TPUT_HDLR_H__
#define __DS_WCDMA_TPUT_HDLR_H__

/*===========================================================================

  Copyright (c) 2009-2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/


/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/umtsps/inc/ds_wcdma_throughput_hdlr.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------

==============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"

#include "IWCDMA.h"
#include "rlcdsapif.h"

#include "ds_3gpp_msg.h"
#include "ds3gtimer.h"
#include "ds3gtputmgr.h"


/*===========================================================================
FUNCTION  DS_WCDMA_CHECK_AND_UPDATE_L1_TPUT_STATUS

DESCRIPTION
  This function checks the current status of WCDMA L1 reporting status and
  sends (start/stop) WCDMA L1 request if required

  If report frequency is 0, this function and reporting is going on
  this function will send to stop report request .
  If reporting frequcny is positive and reporting is stop, this function
  will send start request and return TRUE

PARAMETERS
  subs_id - sys_modem_as_id_e_type (Subscription ID)
  need_to_report - TRUE if reporting needs to start, FALSE otherwise

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void ds_wcdma_check_and_update_l1_tput_status
(
   sys_modem_as_id_e_type subs_id,
   boolean need_to_report
);


/*===========================================================================
FUNCTION  DS_WCDMA_GET_ACTUAL_REPORT_INTERVAL

DESCRIPTION
  This function checks with WCDMA l1 for actual reporting interval based on
  requested interval from client

PARAMETERS 
  requested_interval - client requested interval 
  subs_id - sys_modem_as_id_e_type (Subscription ID)

DEPENDENCIES
  None.

RETURN VALUE
  actual interval

SIDE EFFECTS
  None.

===========================================================================*/

uint32 ds_wcdma_get_actual_report_interval
(
   sys_modem_as_id_e_type subs_id,
   uint32 requested_interval
);

#ifdef FEATURE_DATA_RAVE_SUPPORT
/*===========================================================================
FUNCTION ds_wcdma_uplink_data_rate_ind_hdlr

DESCRIPTION
  This function is used to handle the ul throughput report from wcdma rrc.  
  the report provides the uplink throughput value and confidence

PARAMETERS
  void* cmd_payload_ptr

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/ 
void ds_wcdma_uplink_data_rate_ind_hdlr(void *cmd_payload_ptr);
#endif

#endif /* __DS_WCDMA_TPUT_HDLR_H__*/

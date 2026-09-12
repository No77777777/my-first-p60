#ifndef TDSL1WLANCOEX_H
#define TDSL1WLANCOEX_H

/*===========================================================================

                   T D S L 1   D S D S   F U N C T I O N

DESCRIPTION
  This file contains code for L1 WALN co-existence handling.
  
  
INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000-2012 by Qualcomm Technologies, Incorporated. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsl1wlancoex.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/03/14   yufu   created


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "cxm_intf_types.h"
#include "cxm.h"
#include "wcn_coex_mgr.h"
#include "tfw_sw_intf_msg.h"
#include "tfw_sw_intf_multi_sim.h"

#include "tdsl1m_rcinit.h"
#ifdef FEATURE_TDSCDMA_WLAN_COEX /* FEATURE_TDSCDMA_WLAN_COEX */

/*For slot adv ntc*/

#define TDSL1WLANCOEX_CHANNEL_NONE       (0)
#define TDSL1WLANCOEX_CHANNEL_UPPCH      (1<<0)
#define TDSL1WLANCOEX_CHANNEL_ULDPCH     (1<<1)
#define TDSL1WLANCOEX_CHANNEL_HSSICH     (1<<2)
#define TDSL1WLANCOEX_CHANNEL_ERUCCH     (1<<3)
#define TDSL1WLANCOEX_CHANNEL_EPUCH      (1<<4)
#define TDSL1WLANCOEX_CHANNEL_PRACH      (1<<5)

#define TDSL1WLANCOEX_CHANNEL_FPACH      (1<<8)
#define TDSL1WLANCOEX_CHANNEL_DLDPCH     (1<<9)
#define TDSL1WLANCOEX_CHANNEL_HSSCCH     (1<<10)
#define TDSL1WLANCOEX_CHANNEL_HSPDSCH    (1<<11)
#define TDSL1WLANCOEX_CHANNEL_EAGCH      (1<<12)
#define TDSL1WLANCOEX_CHANNEL_EHICH      (1<<13)
#define TDSL1WLANCOEX_CHANNEL_FACH       (1<<14)
#define TDSL1WLANCOEX_CHANNEL_MEAS       (1<<15)


#define TDSL1WLANCOEX_SLOT_INDEX(slot) ((slot>0)?(slot+2):slot)

#define TDSL1WLANCOEX_SLOT_CHANNEL_EXIST(slot_index, mask) (tdsl1_wlan_coex_slot_info.slot_usage[slot_index].channel_type & (mask))
#define TDSL1WLANCOEX_SLOT_CHANNEL_ADD(slot_index, mask) (tdsl1_wlan_coex_slot_info.slot_usage[slot_index].channel_type |= (mask))
#define TDSL1WLANCOEX_SLOT_CHANNEL_DEL(slot_index, mask) (tdsl1_wlan_coex_slot_info.slot_usage[slot_index].channel_type &= (~(mask)))

#define TDSL1WLANCOEX_TRIGGER_SLOT_ADV_NTC_NORMAL 0x01
#define TDSL1WLANCOEX_TRIGGER_SLOT_ADV_NTC_HSPDSCH 0x02

#define TDSL1WLANCOEX_SLOT_ADV_NTC_ISR_LOCK()                    REX_ISR_LOCK(&tdsl1wlancoex_slot_adv_ntc_mutex)
#define TDSL1WLANCOEX_SLOT_ADV_NTC_ISR_UNLOCK()                  REX_ISR_UNLOCK(&tdsl1wlancoex_slot_adv_ntc_mutex)


typedef struct 
{
  cxm_tds_slot_info_s slot_adv_ntc;
  uint16 channel_type;
}tdsl1_wlan_coex_slot_usage_struct_type;

typedef struct 
{
  uint16 sub_sfn;
  tdsl1_wlan_coex_slot_usage_struct_type slot_usage[CXM_NUM_TDS_SLOTS];
}tdsl1_wlan_coex_slot_info_struct_type;


typedef struct
{
  cxm_action_e               action;  /*!< whether to START/READ/STOP metric collection */
  cxm_metric_req_u           payload; /*!< associated payload for the type of action */
}tdsl1_wlan_coex_noise_metric_struct_type;

/*===========================================================================

                     MACROS

===========================================================================*/
#define USTMR_INVAILD_VALUE (0xFFFFFFFF)
#define FREQ_INVAILD_VALUE  0xFFFF
#define SUBFN_INVAILD_VALUE 0xFFFF
#define CX8_INVAILD_VALUE   0xFFFF

typedef enum
{
  TDSL1_WLAN_COEX_HIGH_PRIO_IND_BOUNDRY_START,
  TDSL1_WLAN_COEX_HIGH_PRIO_IND_BOUNDRY_END
} tdsl1_wlan_coex_high_prio_ind_boundry_type;

typedef struct
{
  tdsl1_time_info_struct_type start;
  tdsl1_time_info_struct_type end;
} tdsl1_wlan_coex_high_prio_tds_timing_struct_type;

typedef struct
{
  boolean ind_for_pich_sent_before_sleep;
  uint32  pich_sfn;
} tdsl1_wlan_coex_high_prio_pich_sent_struct_type;

typedef struct
{
/*for active poliy config*/
 cxm_coex_active_policy_ind_s current_policy;
 cxm_coex_metrics_req_s noise_metric;
 boolean                      policy_is_pending;
 boolean                      metric_start_is_pending;
 boolean                      metric_stop_is_pending;
}tdsl1_wlan_coex_db_struct_type;

extern tdsl1_wlan_coex_high_prio_pich_sent_struct_type tdsl1_wlan_coex_high_prio_pich_sent_info;

extern tdsl1_wlan_coex_db_struct_type tdsl1_wlan_coex_db;



/*===========================================================================
FUNCTION     TDSL1_wlan_coex_INIT

DESCRIPTION
  This function initialized TDSL1 wlan coex related globals. This function must be
  once after power-on.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
	
void tdsl1_wlan_coex_init(void);

/*===========================================================================
FUNCTION     TDSL1_wlan_coex_policy_handling

DESCRIPTION
  This function initialized TDSL1 dual sim related globals. This function must be
  once after power-on.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsl1_wlan_coex_policy_handling( cxm_coex_active_policy_ind_s *policy_ind_ptr);

/*===========================================================================
FUNCTION     TDSL1_wlan_coex_policy_to_FW

DESCRIPTION
  This function to send the waln coex policy to TFW.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsl1_wlan_coex_set_policy_to_fw(void);

/*===========================================================================
FUNCTION   tdsl1_in_wlan_coex_mode

DESCRIPTION
  This function to return if wlan coex exists
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsl1_in_wlan_coex_mode(void);

/*===========================================================================
FUNCTION   tdsl1_wlan_coex_send_fw_active_policy

DESCRIPTION
  This function send TDSCDMA_FW_MULTI_SIM_CONFIG_IND to FW, configure it as non coex mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsl1_wlan_coex_send_fw_active_policy(cxm_coex_active_policy_ind_s *policy_ind_ptr);


/*===========================================================================
FUNCTION     tdsl1_wlan_coex_metrics_req_handling

DESCRIPTION
  This functionto to handle the noise start/read/stop cmd from mcs. mcs->L1

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsl1_wlan_coex_metrics_req_handling(cxm_coex_metrics_req_s *metrics_req_ptr);

/*===========================================================================
FUNCTION     tdsl1_wlan_coex_tfw_metrics_ctr_cmd

DESCRIPTION
  This function to send TFW start/stop running the noise filter. TL1->TFW

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsl1_wlan_coex_tfw_metrics_ctr_cmd(cxm_coex_metrics_req_s *metrics_req_ptr);

/*===========================================================================
FUNCTION     tdsl1_wlan_coex_tfw_txfrmdnl_ind

DESCRIPTION
  This functionto to send txfrmdnl from TFW to MCS. TL1->MCS

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsl1_wlan_coex_tfw_txfrmdnl_ind(tfw_wlan_txfrmdnl_ind_t *tfw_txfrmdnl_ind);



/*===========================================================================
FUNCTION     tdsl1_wlan_coex_sleep_wakup_duration_ind

DESCRIPTION
  This functionto to send mcs sleep and wakeup duration every sleep/wake cycle.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsl1_wlan_coex_sleep_wakeup_duration_ind(boolean to_sleep);

/*===========================================================================

FUNCTION    tdsl1_wlan_coex_high_prio_ind_process_acq

DESCRIPTION
          process acq

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsl1_wlan_coex_high_prio_ind_process_acq(tdsl1_wlan_coex_high_prio_ind_boundry_type action_type, uint16 arfcn);


/*===========================================================================

FUNCTION    tdsl1_wlan_coex_high_prio_ind_process_rach

DESCRIPTION
          process rach

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsl1_wlan_coex_high_prio_ind_process_rach(tdsl1_wlan_coex_high_prio_ind_boundry_type action_type, uint16 action_sub_fn);

/*===========================================================================

FUNCTION    tdsl1_wlan_coex_high_prio_ind_process_bho

DESCRIPTION
          process bho

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsl1_wlan_coex_high_prio_ind_process_bho(tdsl1_wlan_coex_high_prio_ind_boundry_type action_type);

/*===========================================================================

FUNCTION    tdsl1_wlan_coex_high_prio_ind_process_dl_ch

DESCRIPTION
          process PCCPCH, PCH and PICH

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsl1_wlan_coex_high_prio_ind_process_dl_ch(tdsdlphch_notify_enum_type operation, mcaltds_dl_phch_cfg_parms_struct_type* phch_cfg_parms_ptr);

/*===========================================================================
FUNCTION     tdsl1_wlan_coex_high_prio_ind

DESCRIPTION
  This functionto to send high prio ind for some certain event.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsl1_wlan_coex_high_prio_ind(cxm_high_prio_oper_e op_id, tdsl1_wlan_coex_high_prio_tds_timing_struct_type* tds_timing, uint8 num_freqs, uint32* freq_array);

/*===========================================================================
FUNCTION tdsl1_wlan_coex_send_msg

DESCRIPTION
  Send a Message to the RF task through the Message Router
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
static void tdsl1_wlan_coex_send_msg( msgr_umid_type umid, void *msg, uint32 msg_size );

/*===========================================================================
FUNCTION tdsl1_wlan_coex_trigger_slot_adv_notice

DESCRIPTION
  Trigger and send the slot adv notice to MCS in connected mode
  NORMAL point: at end of TS3 for normal channel
  HSPDSCH point: at scch dec done ISR for HSPDSCH update
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsl1_wlan_coex_trigger_slot_adv_notice(uint8 trigger_point);

/*===========================================================================
FUNCTION     tdsl1_wlan_coex_frame_timing_ind

DESCRIPTION
  This function is to send frame timing information every 10ms in connected mode. L1->mcs

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsl1_wlan_coex_frame_timing_ind(void);

#endif /* FEATURE_TDSCDMA_WLAN_COEX */
#endif


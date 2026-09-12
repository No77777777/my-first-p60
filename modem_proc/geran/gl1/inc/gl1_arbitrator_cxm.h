#ifndef GARB_CXM_H
#define GARB_CXM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
            G E R A N  A R B I T R A T O R  C X M  I N T E R F A C E

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/gl1_arbitrator_cxm.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $


when       who       what, where, why
--------   --------  ----------------------------------------------------------
07/04/16   snjv      CR1000205 Retrieve Rx/Tx Channel Id based on Rx/Tx client id
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
17/06/15   mc        CR855818 : COEX:MCS_CXM_WWAN_TECH_STATE_IND registrations for X2G IRAT need to indicate CXM_LNK_TYPE_IRAT
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
17/03/15   shr       CR808932: COEX support for DTM
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
27/02/15   am        CR646584 WLAN coex and CR    - PART 1
18/12/14   cgc       CR766576 FR23109 Adaptive Power monitor during CS in DSDA G + TDD ( TDS/LTE TDD)
03/12/14   npt       CR759467 Fix compiler warnings
08/05/14   ws        CR 661576 GL1 Support for sending MCS_CXM_BAND_AVOID_PWR_IND
26/06/14   ab        CR 682000 - COEX - GL1 to update ARFCN if they are not registered when start start is next radio block
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
19/02/14   ap	     CR600310 GSM L1 to send channel Id to GSM FW corresponding to the primary RF device 
04/11/13   mc        CR556742  : G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
11/10/13   pjr       CR.555300 : Changes to correctly register freqid during monitors  and acq
01/10/13   sk        CR552259 CR sync from 1.0/1.1
23/09/13   sk        2.0 syncup from 1.1
15/08/13   sk        CR524039 GBTA bringup changes
29/07/13   sk        CR519663 Partial QBTA bringup changes
13/06/13   cs        TSTS Updates
05/04/13   mc        CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13   mc        CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifndef CUSTOMER_H
  #include "customer.h"
#endif
#ifndef COMDEF_H
  #include "comdef.h"
#endif

#include "cxm_intf_types.h"
#include "geran_dual_sim.h"

#ifdef FEATURE_GSM_COEX_SW_CXM

#include "gl1_arbitrator_types.h"
/*===========================================================================

                         DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                        PUBLIC DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION      garb_cxm_register_priority_table_callback

DESCRIPTION   Registers the priority callback function with CXM. See the
              callback funtion for more info.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_register_priority_table_callback(gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_cxm_deregister_priority_table_callback

DESCRIPTION   De-registers the priority callback function with CXM. See
              the callback funtion for more info.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_deregister_priority_table_callback(gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_is_g_agressing_x

DESCRIPTION   This function indicates if GSM Tx is desending X.

RETURN VALUE  BOOLEAN

===========================================================================*/
boolean garb_is_g_agressing_x(void);

/*===========================================================================

FUNCTION      garb_cxm_arfcn_indication

DESCRIPTION   Notify CXM of our frequency list. The CXM supports a maximum
              of 24 registered ARFCNs at any time. Note that for each ARFCN
              UL and DL are handled separately and are assigned different
              CXM freq IDs.

              For CS and PS assignments, if there are 10 or less ARFCNs in our
              assigned frequency list then we can do a one-shot registration
              with CXM which will be good for the lifetime of the assignment.
              If however there are more than 10 ARFCNs in the assigned
              frequency list then we need to periodically update CXM with a 
              new list of registered ARFCNs.

              This function sends MCS_CXM_WWAN_TECH_STATE_IND to CXM.
 
RETURN VALUE  None

===========================================================================*/
void garb_cxm_arfcn_indication(ARFCN_T* arfcns, uint8 num_arfcns, cxm_tech_link_direction direction, boolean register_bands, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_cxm_notify_freqid_list

DESCRIPTION   Notifies GL1 of the list of CXM freqids to be associated with
              the reported ARFCNs.

              This function is called from L1 Task on receipt of an
              MCS_CXM_FREQID_LIST_GSM sent by CXM in response to a
              MCS_CXM_WWAN_TECH_STATE_IND.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_notify_freqid_list(cxm_freqid_info_ind_s* msg, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_cxm_power_indication_arfcn

DESCRIPTION   Notify the CXM of our updated average UL and DL power for
              each serving cell FOR ONLY one ARFCN

              This function sends MCS_CXM_COEX_POWER_IND to CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_power_indication_arfcn(ARFCN_T arfcn,int32 rx_power_dbm10,gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_cxm_band_power_ind

DESCRIPTION   Notify the CXM of our updated average DL power at BAND_LEVEL
              

RETURN VALUE  None

===========================================================================*/
void garb_cxm_band_power_ind(sys_band_T band, int32 pwr_dBm_x16, gas_id_t gas_id);


/*===========================================================================

FUNCTION      garb_cxm_power_indication

DESCRIPTION   Notify the CXM of our updated average UL and DL power for
              each serving cell ARFCN.

              This function sends MCS_CXM_COEX_POWER_IND to CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_power_indication(gas_id_t gas_id);

/*===========================================================================
FUNCTION      garb_cxm_band_avoid_pwr_ind

DESCRIPTION   Notify the CXM of our updated average UL and DL power for
              each serving cell ARFCN for Band avoidance purposes

              This function sends MCS_CXM_BAND_AVOID_PWR_IND to CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_band_avoid_pwr_ind(gas_id_t gas_id);

#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_GSM_COEX
/*===========================================================================

FUNCTION      garb_cxm_activity_timeline_indication

DESCRIPTION   Notify CXM of our activity timeline. This is only reported to
              CXM when requested via MCS_CXM_REQUEST_ACTIVITY_INFO_IND. During
              CS or PS calls this is sent initially as a one-shot report with
              subsequent updates sent for e.g. if timing drifts beyond that
              which can be absorbed by the guard period.

              For CS and PS(TCH) the indication is updated every
              GARB_IND_UPDATE_PERIOD frames to correct for any
              USTMR drift. For RACH and CS(SDCCH) the notification is sent 
              single-shot for each UL burst.

              This function sends MCS_CXM_SET_ACTIVITY_TIMELINE_IND to
              CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_activity_timeline_indication(uint8 read_t_index,gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_cxm_notify_request_activity_info

DESCRIPTION   Notification from CXM that GL1 is requested to send
              MCS_CXM_SET_ACTIVITY_TIMELINE_IND reports.

              This function is called from L1 Task on receipt of an
              MCS_CXM_REQUEST_ACTIVITY_INFO_GSM_IND.

RETURN VALUE  None
 
===========================================================================*/
void garb_cxm_notify_request_activity_info(cxm_request_activity_info_s* cxm_request_activity_info, gas_id_t gas_id);
/*===========================================================================

FUNCTION      garb_cxm_notify_coex_bcast_ind

DESCRIPTION   Notification from CXM to notify TECH RX/TX state.

              This function is called from L1 Task on receipt of an
              MCS_CXM_COEX_TECH_STATE_BCAST_IND

RETURN VALUE  None

===========================================================================*/
void garb_cxm_notify_coex_bcast_ind(cxm_coex_tech_state_bcast_ind_s* cxm_coex_tech_state_bcast_info);

#endif /* FEATURE_GSM_COEX */
#endif /* FEATURE_DUAL_SIM */

#ifdef FEATURE_GSM_COEX_FW_CXM
/*===========================================================================

FUNCTION      garb_return_cxm_channel_id

DESCRIPTION   returns the cxm channel ID for the specified Client Id
 
DEPENDENCIES  This function uses cxm_get_channel_id , which returns the Channel id based 
              on the Device fed to it, So, We need to Pass the appro. Rx/Tx Device
              to get the correct Rx/Tx Channel Id
              Its return structure even though has members for rx/tx arent configured that way
 
              For GBTA, When The Calling client will have an Invalid RF Path,
              Other G Clients Rx Path ONLY will be used, since operation concerned is only Rx                  
  
RETURN VALUE  cxm channel type

===========================================================================*/
cxm_channel_out_type garb_return_cxm_channel_id( grm_client_enum_t grm_client, gas_id_t gas_id  );
#endif /* FEATURE_GSM_COEX_FW_CXM */

/*===========================================================================

FUNCTION      garb_cxm_get_freqid

DESCRIPTION   This function returns the CXM freqid associated with the 
              requested ARFCN.
 
RETURN VALUE  The CXM freqid associated with the requested band.
 
===========================================================================*/
uint32 garb_cxm_get_freqid(ARFCN_T arfcn, garb_rxtx_t rxtx, gas_id_t gas_id);

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING

/*===========================================================================

FUNCTION      garb_cxm_get_prio_from_activity_tier

DESCRIPTION   This function retrieves the priority associated with the 
              specified CXM activity tier. If the specified activity tier is
              invalid then a priority of zero is returned.

RETURN VALUE  CXM priority
 
===========================================================================*/
uint32 garb_cxm_get_prio_from_activity_tier(cxm_activity_type tier, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_cxm_set_slot_activity_tl

DESCRIPTION   Notifies CXM of GSM timeline information. This provides a much
              more detailed timeline report than the more primative
              MCS_CXM_SET_ACTIVITY_TIMELINE_IND and will likely obsolete it.

              This UMID is used to provide a rolling 4 frame timeline window
              to MCS. This function is called on every frame to build the 4
              frame timeline.

              This information is used by TECH L1s and WLAN to avoid
              conflicts with GSM.

              This function sends MCS_CXM_SET_SLOT_ACTIVITY_TL to
              CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_set_slot_activity_timeline(uint8 read_t_index,gas_id_t gas_id);

#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

#ifdef FEATURE_WLAN_COEX_SW_CXM
void garb_cxm_reset_coex_active_policy( gas_id_t gas_id );
void garb_cxm_coex_active_policy(cxm_coex_active_policy_ind_s* msg, gas_id_t gas_id);
void garb_cxm_reset_coex_metrics( gas_id_t gas_id );
void garb_cxm_coex_metrics_req(cxm_coex_metrics_req_s* msg, gas_id_t gas_id);
uint32 garb_cxm_coex_metrics_alpha( gas_id_t gas_id );
void garb_cxm_fw_priority_fail_report( GfwWlanCxmConfigRsp* config_rsp, gas_id_t gas_id );
void garb_cxm_send_sleep_duration( boolean sleep, uint32 duration, gas_id_t gas_id);
void garb_cxm_high_priority_event_ind( uint32 start_fn, uint32 frames_required, cxm_high_prio_oper_e op_id, gas_id_t gas_id );
#endif

#endif /* FEATURE_GSM_COEX_SW_CXM */
#endif /* GARB_CXM_H */

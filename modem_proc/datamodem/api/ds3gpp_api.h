#ifndef DS3GPP_H
#define DS3GPP_H
/*===========================================================================

               3 G P P   P A C K E T - S W I T C H E D   D A T A

                    E X T E R N A L   H E A D E R   F I L E

  DESCRIPTION
  This file contains external data declarations and function prototypes for 
  3gpp Packet-Switched Data Services.

  INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2010-2018 Qualcomm Technologies, Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary.  Export of this technology or software is regulated
  by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/api/ds3gpp_api.h#2 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
12/14/12   nd      Sharing API to RRC for PCH indication to applications.
01/25/11   ss      SU API defeaturization and removal of customer.h.
12/29/10   ua      Added API, ds3gpp_is_service_req_allowed_on_plmn.
07/26/10   yz      First revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "sys.h"
#include "cm_gw.h"
#include "ps_iface_defs.h"
#include "rlcdsapif.h"
#include "IxErrno.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------- 
  Fn ptr to be called when reset attach profile list to default profile is
  successful.
  Result - TRUE if attach cache was successfully updated
  ----------------------------------------------------------------------------*/
typedef void (*ds_3gpp_reset_attach_profile_list_cb_fptr)
(
  boolean result
);

/*---------------------------------------------------------------------------
  RRC states in connected mode. PCH states are URA_PCH and CELL_PCH out of PCH
  states are CELL_FACH and CELL_DCH.
---------------------------------------------------------------------------*/
typedef enum ds_3gpp_pch_event_e{
  DS_3GPP_EVENT_OUT_OF_PCH,
             /* UTRAN moves UE out of PCH states(URA_PCH and CELL_PCH) */
  DS_3GPP_EVENT_IN_PCH
             /* UTRAN moves UE into PCH states(URA_PCH and CELL_PCH) */
} ds_3gpp_pch_event_e_type;

typedef struct
{
 ds_3gpp_pch_event_e_type  pch_event;
 sys_sys_mode_e_type       call_mode;
 sys_modem_as_id_e_type    subs_id;
}ds_3gpp_rrc_pch_ind_proc_cmd_type;

typedef struct
{
 sys_modem_as_id_e_type               subs_id;
 sys_sys_mode_e_type                  sys_mode; 
 sys_wcdma_rrc_state_e_type           rrc_state;
}ds_3gpp_rrc_state_ind_cmd_type;


typedef struct
{
  uint32 rab_id;
  uint32 ul_data_rate;
  sys_modem_as_id_e_type subs_id;
}ds_3gpp_ul_rab_info_type;



typedef struct
{
  uint32 uplink_configured_kbps;
  uint32 downlink_configured_kbps;
  sys_modem_as_id_e_type subs_id;
}ds_3gpp_configured_datarate_type;


typedef struct
{
  uint32 uplink_datarate_kbps;
  uint8  confidence;
  sys_modem_as_id_e_type subs_id;
}ds_3gpp_uplink_datarate_ind;

typedef struct
{
  uint32 data_rate_in_kbps;
  uint8  confidence;
  boolean data_freeze;
  boolean moved_out;
  sys_modem_as_id_e_type subs_id;
}ds_3gpp_downlink_estimated_data_ind;


/*---------------------------------------------------------------------------
  This structure is used to read the NV item(NV:global_throttling). Other
  modem subsystems (e.g: NAS) can read this NV item using this struct.
---------------------------------------------------------------------------*/
typedef struct
{ 
  uint8   version ;          /* version of the NV item*/
  boolean global_throttling; /* NV item value */
  uint8 reserve_bytes[2];    /* reserve bytes for future use*/
}ds_3gpp_global_throttle_s;

/*---------------------------------------------------------------------------
 type of the notification other modules making to DS 
---------------------------------------------------------------------------*/
typedef enum 
{
  DS_3GPP_INVALID_IND = -1,
  DS_3GPP_RAU_IND= 0
}ds_3gpp_notify_ind_e_type;

/*---------------------------------------------------------------------------
 RAU events that other modules can notify
---------------------------------------------------------------------------*/
typedef enum 
{
DS_3GPP_INVALID_EVT = -1,
DS_3GPP_RAU_COMPLETE_EVT, //? Currently we need only this event. Added other for future cases.
DS_3GPP_RAU_STARTED,
DS_3GPP_RAU_FAILED
}ds_3gpp_rau_event_e_type;

typedef enum 
{
  DS_3GPP_INVALID_RAU_TYPE = -1,
  DS_3GPP_PERIODIC_TYPE,
  DS_3GPP_RA_CAHNGE_TYPE,
  DS_3GPP_OTHER_TYPE,
  DS_3GPP_RAU_MAX

}ds_3gpp_rau_type;

typedef void (*ds_3gpp_bearer_err_ind_cb_type)
(
  uint8 eps_bearer_id,  /*EPS bearer ID  */
  boolean prev_resync_passed, /* Previous sync flag passed */
  uint8 as_id
);

typedef struct
{
  sys_modem_as_id_e_type     subs_id;
  sys_sys_mode_e_type        sys_mode;
  ds_3gpp_rau_event_e_type   event;
  ds_3gpp_rau_type           rau_type;
}ds_3gpp_rau_event_s_type;

typedef struct 
{
  uint8	version ;		   /* version of the NV item*/
  boolean bcm_info_validation_enabled; /* NV item value */
  uint8 reserve_bytes[2];    /* reserve bytes for future use*/
}ds_3gpp_bcm_info_validation_s;

typedef struct
{
  ds_3gpp_bearer_err_ind_cb_type pdpc_cb_fptr;
  uint8   threshhold_pkt_cnt;
}ds_3gpp_bearer_err_info_type;

typedef enum 
{
  DS_3GPP_IP_TYPE_INVALID = 0,
  DS_3GPP_IPV4,
  DS_3GPP_IPV6,
  DS_3GPP_IPV4V6,
  DS_3GPP_PPP
} ds_3gpp_bearer_ip_support_type_e;

/*---------------------------------------------------------------------------
 RAN bit rate support configuration data struct
---------------------------------------------------------------------------*/
typedef struct
{
  boolean    rec_bit_data_rate_support;
  boolean    query_bit_data_rate_support;
} ds_3gpp_ran_bit_data_rate_support_config;

typedef struct
{
  boolean                 criteria_is_met;
  sys_modem_as_id_e_type  subs_id;
}ds_3gpp_downlink_actual_throughput_ind_type;

typedef void (*ds_3gpp_downlink_actual_throughput_ind_cb_type)
(
  ds_3gpp_downlink_actual_throughput_ind_type  actual_report_result
);
/*===========================================================================
                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION DS_3GPP_NOTIFY_IND

DESCRIPTION
   This is a generic function, used to notify DS module.

PARAMETERS
 indication type and 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If Data is available
  FALSE - If Data is not available

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_notify_ind(ds_3gpp_notify_ind_e_type indication, void *notify_ind_data_ptr);

/*===========================================================================
FUNCTION DS3GPP_PLMN_BLOCKED_FORCE_LTE_PREF

DESCRIPTION
   This is called by lower layers during powere off to see if any PLMN is blocked.
   lower layer will set their next pref sys based on the PLMN blocked status

PARAMETERS
  subs_id 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If any PLMN is blocked
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3gpp_is_plmn_blocked_force_lte_pref
(
  sys_modem_as_id_e_type	  subs_id
);

/*===========================================================================
FUNCTION DS3GPP_IS_PS_DATA_AVAILABLE

DESCRIPTION
   This function returns TRUE if data is available in any of the uplink
   buffers, otherwise, it returns FALSE.

PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If Data is available
  FALSE - If Data is not available

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3gpp_is_ps_data_available(void);

/*===========================================================================
FUNCTION DS3GPP_PER_SUBS_IS_PS_DATA_AVAILABLE

DESCRIPTION
  This function returns TRUE if data is available in any of the uplink
  buffers, otherwise, it returns FALSE.

PARAMETERS 
   Subscription Id 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If Data is available
  FALSE - If Data is not available

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3gpp_per_subs_is_ps_data_available
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS3GPP_IS_UE_INIT_QOS_SUPPORTED

DESCRIPTION
   This function returns TRUE if umts UE initiated QOS support
   is available, otherwise, it returns FALSE.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If UE initiated UMTS QOS support is available
  FALSE - If UE initiated UMTS QOS support is not available

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3gpp_is_ue_init_qos_supported(void);

/*===========================================================================
FUNCTION DS3GPP_IS_SERVICE_REQ_ALLOWED_ON_PLMN

DESCRIPTION
  This function is used to check if the service request throttling timer is 
  currently running on the PLMN passed into the function.

  If the throttling algorithm has never been kicked off in this PLMN or if the
  throttling timer is not running currently for the PLMN, the UE can send the 
  service request over the air.

PARAMETERS
  plmn_id - The PLMN id in which this the service request is to be sent over air

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If it is ok to send Service request over the air for this PLMN
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3gpp_is_service_req_allowed_on_plmn(sys_plmn_id_s_type plmn_id);


/*===========================================================================
FUNCTION DS3GPP_PER_SUBS_IS_SERVICE_REQ_ALLOWED_ON_PLMN

DESCRIPTION
  This function is used to check if the service request throttling timer is 
  currently running on the PLMN passed into the function.

  If the throttling algorithm has never been kicked off in this PLMN or if the
  throttling timer is not running currently for the PLMN, the UE can send the 
  service request over the air.

PARAMETERS
  plmn_id - The PLMN id in which this the service request is to be sent over air
  subs_id - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If it is ok to send Service request over the air for this PLMN
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3gpp_per_subs_is_service_req_allowed_on_plmn
(
  sys_plmn_id_s_type          plmn_id,
  sys_modem_as_id_e_type      subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_UPDATE_PCH_STATE

DESCRIPTION
  This function is used by RRC in WCDMA and TDSCDMA mode to indicate when
  UTRAN moves UE to PCH states: CELL_PCH and URA_PCH. Same needs to be
  propagated to applications. It will be useful for saving power.

PARAMETERS
  sys_mode  - system network mode, PCH states are applicable only in case of
             TDSCDMA and WCDMA.
  pch_event - In or out of PCH state.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_update_pch_state
(
  sys_sys_mode_e_type sys_mode,
  ds_3gpp_pch_event_e_type pch_event
);
/*===========================================================================
FUNCTION DS_3GPP_UPDATE_PCH_STATE_SUB

DESCRIPTION
  This function is used by RRC in WCDMA and TDSCDMA mode to indicate when
  UTRAN moves UE to PCH states: CELL_PCH and URA_PCH. Same needs to be
  propagated to applications. It will be useful for saving power.

PARAMETERS
  subs_id   - Subscription ID
  sys_mode  - system network mode, PCH states are applicable only in case of
             TDSCDMA and WCDMA.
  pch_event - In or out of PCH state.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_update_pch_state_sub
(
  sys_modem_as_id_e_type   subs_id,
  sys_sys_mode_e_type      sys_mode,
  ds_3gpp_pch_event_e_type pch_event  
);

/*===========================================================================
FUNCTION DS_3GPP_RRC_PCH_STATE_UPDATE

DESCRIPTION
  This function is used by RRC in WCDMA/TDS mode to indicate rrc state changes

PARAMETERS
  subs_id   - Subscription ID
  rrc_state

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_rrc_state_update
(
   sys_modem_as_id_e_type               subs_id,
   sys_sys_mode_e_type                  sys_mode,
   sys_wcdma_rrc_state_e_type           rrc_state
);
/*==============================================================================

FUNCTION:  ds_eps_bearer_reenqueue_pkts

DESCRIPTION
  This API will be used to traverse the DSM chain(head_dsm_ptr) and
  enqueue each packet to WM (wm_ptr).It will also increase the dne
  count if required based on the num_pkt_bytes

PARAMETERS
    wm_ptr : WM to which pkts need to be re-enqueued
    num_pkt_byes: Number of bytes to be enqueued
    head_dsm_ptr: This will be a chain of DSM items

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

*/

void ds_eps_bearer_reenqueue_pkts
(
  dsm_watermark_type* wm_ptr,     
  uint32 num_pkt_bytes,               
  dsm_item_type* head_dsm_ptr,
  uint8 as_id
);

/*==============================================================================

FUNCTION:  ds_eps_bearer_reenqueue_pkts_pktcount

DESCRIPTION
  This API will be used to traverse the DSM chain(head_dsm_ptr) and
  enqueue each packet to WM (wm_ptr).It will also increase the dne
  count if required based on the num_pkt_bytes

PARAMETERS
    wm_ptr : WM to which pkts need to be re-enqueued
    num_pkt_byes: Number of bytes to be enqueued
    head_dsm_ptr: This will be a chain of DSM items

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

*/

void ds_eps_bearer_reenqueue_pkts_pktcount
(
  dsm_watermark_type* wm_ptr,     
  uint32         num_pkt_bytes,               
  dsm_item_type* head_dsm_ptr,
  uint32         num_pkts                   
);

/*===========================================================================

FUNCTION:  DS_EPS_BEARER_REENQUEUE_PKTS_PKTCOUNT_SPECIAL

DESCRIPTION
  This API will be used to traverse the DSM chain(head_dsm_ptr) and
  enqueue each packet to WM (wm_ptr). This is a special API which will be called
  when control pkts are to be queued during handover. The pkts in this case will
  not be dropped.

PARAMETERS
    wm_ptr : WM to which pkts need to be re-enqueued    
    head_dsm_ptr: This will be a chain of DSM items

DEPENDENCIES
  None.

RETURN VALUE
  Num pkts enqueued

SIDE EFFECTS
  None.  
===========================================================================*/
uint32 ds_eps_bearer_reenqueue_pkts_pktcount_special
(
  dsm_watermark_type* wm_ptr,     /*WM to which pkts need to be re-enqueued*/  
  dsm_item_type* head_dsm_ptr     /*Head of DSM chain*/  
);

/*===========================================================================

FUNCTION:  ds_eps_bearer_flush_bytes

DESCRIPTION
  This API will be used to flush the eps bearer and ensure that the wm
  number of bytes to be remaining at the end is less than or equal to the
  bytes specified
  

PARAMETERS
  eps_id : eps id
  num_pkt_byes: Number of bytes to be remaining

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.  
===========================================================================*/
void ds_eps_bearer_flush_bytes
(
  uint8   eps_id,                        /*eps bearer id which needs to be flused*/
  uint32  num_pkt_bytes,               /*Total number of bytes to be remaining at end of procedure*/
  uint8   as_id
);

/*===========================================================================
FUNCTION DS_TDSCDMA_CONFIGURE_UL_DATA_RATE

DESCRIPTION
  This function is used by RRC in TDSCDMA mode to indicate the updated
  data rate on a particular rab_id/nsapi changes. Based on which
  wm levels are adjusted on that bearer

PARAMETERS
  rab_id  - Identifier used to uniquely identify a data bearer
  ul_data_rate - Updated data rate of the bearer
  subs_id      - Subscription on which this call is made

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void ds_tdscdma_configure_ul_data_rate
(
   uint32 rab_id,  
   uint32  ul_data_rate,
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_COMPARE_PDP_ADDRESS

DESCRIPTION
  This function is used by NAS to compare the pdp_address when network
  sends MODIFY_REQUEST with pdp_address included. DS will compare the
  pdp_address passed by NAS against the pdp_address which is configured on
  V4/V6 interfaces.

PARAMETERS
  identifier   - Identifier used to uniquely identify a PDP/PDN Context.
                 (For W/G/TD it would be nsapi, For LTE it would be eps_bid)
  sys_mode     - Current sys_mode.
  subs_id      - Subscription on which the PDP/PDN is active.
  *pdp_address - Structure containing the pdp_address and pdp_type_num 
                 given by network, which would be used for comparison.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE -  if PDP address sent by network is same as the one configured.
  FALSE - if PDP address sent by network is different than the one
          configured.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_compare_pdp_address
(
  uint8 identifier,
  sys_sys_mode_e_type sys_mode,
  sys_modem_as_id_e_type subs_id,
  pdp_address_T *pdp_address
);

/*===========================================================================
FUNCTION DS_WCDMA_SET_CONFIGURED_DATA_RATE_IND

DESCRIPTION
  This function is used by  WCDMA rrc to set the configured uplink data rate
  and configured downlink data rate

PARAMETERS
  subs_id      - Subscription on which this call is made
  uplink_configured_kbps - Uplink configured data rate
  downlink_configured_kbps - Downlink configured data rate

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_set_configured_data_rate_ind
(
   sys_modem_as_id_e_type  subs_id,
   uint32  uplink_configured_kbps,
   uint32  downlink_configured_kbps
);
/*===========================================================================
FUNCTION DS_WCDMA_QUAL_ESTIMATION_REPORT_IND

DESCRIPTION
  This function is used by  WCDMA rlc to set the downlink estimated rate
  and confidence

PARAMETERS
  subs_id      - Subscription on which this call is made
  wcdma_qual_estimation_report_data - Downlink data
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ds_wcdma_qual_estimation_report_ind
(
   sys_modem_as_id_e_type  subs_id,
   ds_3gpp_downlink_estimated_data_ind w_data
);
/*===========================================================================
FUNCTION DS_WCDMA_SET_UPLINK_DATA_RATE_IND

DESCRIPTION
  This function is used by  WCDMA rrc to set the uplink data rate
  and confidence

PARAMETERS
  subs_id      - Subscription on which this call is made
  uplink_data_rate_kbps - Uplink configured data rate
  confidence            - Uplink confidence

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
#ifndef TEST_FRAMEWORK
void ds_wcdma_set_uplink_data_rate_ind
(
   sys_modem_as_id_e_type  subs_id,
   uint32  uplink_data_rate_kbps,
   uint8   confidence
);
#endif

#ifdef FEATURE_DATA_LTE

/*===========================================================================
FUNCTION DS_3GPP_GET_BEARER_ERR_INFO_PTR

DESCRIPTION
  This function returns pointer to to bearer error cache variable

PARAMETERS
None
  
DEPENDENCIES
  None.

RETURN VALUE
  Cache Pointer
  
SIDE EFFECTS
  None.
  
===========================================================================*/
ds_3gpp_bearer_err_info_type* ds_3gpp_get_bearer_err_info_ptr (void);

/*===========================================================================
FUNCTION DS_3GPP_GET_BEARER_ERR_THRESHHOLD_CNT

DESCRIPTION
  This function returns threshhold count value for malformed packets 
   cached at DS 

PARAMETERS
None
  
DEPENDENCIES
  None.

RETURN VALUE
  Threshhold packet count
  
SIDE EFFECTS
  None.
  
===========================================================================*/
uint8 ds_3gpp_get_bearer_err_threshhold_cnt
(
  sys_modem_as_id_e_type         subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_REG_PDCP_BEARER_ERR_CB

DESCRIPTION
  This function is exposed to pdcp to register a callback function to be 
  triggered when malformed packet count on particular bearer exceeds 
  threshhold limit.

PARAMETERS
  pdcp_cb_ptr - callback ptr paseed from PDCP
  threshhold count- malformed packet limit
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE.
  
SIDE EFFECTS
  None.
  
===========================================================================*/
boolean ds_3gpp_reg_pdcp_bearer_err_cb 
( 
  ds_3gpp_bearer_err_ind_cb_type pdcp_cb_fptr, 
  uint8   threshhold_pkt_count 
);

#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION:  ds_3gpp_get_ip_type

DESCRIPTION
  This API will be used to get the IP type for the bearer.
  
PARAMETERS
  inst_id    : subs_id
  bearer_id  : eps_bearer_id
  call_mode  : system call_mode

DEPENDENCIES
  None.

RETURN VALUE
  Supported IP Type

SIDE EFFECTS
  None.  
===========================================================================*/
#ifdef FEATURE_MSIM_L_PLUS_L
ds_3gpp_bearer_ip_support_type_e ds_3gpp_get_ip_type
(
  uint8                   bearer_id,
  sys_sys_mode_e_type     call_mode,
  uint8                   as_id 
);
#else
ds_3gpp_bearer_ip_support_type_e ds_3gpp_get_ip_type
(
  uint8                   inst_id, 
  uint8                   bearer_id,
  sys_sys_mode_e_type     call_mode
);
#endif

/*===========================================================================
FUNCTION ds3gpp_per_subs_get_config_multi_pdn_same_apn

DESCRIPTION
  This function gets the multi pdn same apn nv value for each subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether Multi Pdn same apn nv is enabled.

SIDE EFFECTS
===========================================================================*/

boolean ds3gpp_per_subs_get_config_multi_pdn_same_apn
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION ds3gpp_per_subs_get_config_umts_nw_init_qos_support

DESCRIPTION
  This function gets the umts_nw_init_qos_support NV item: 70331
  modem/data/3gpp/umts_nw_initiated_qos_support

PARAMETERS
  None
  
DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether UMTS NW Init QoS is enabled or not

SIDE EFFECTS
  None

===========================================================================*/
boolean ds3gpp_per_subs_get_config_umts_nw_init_qos_support
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_PROFILE_DELETE_ATTACH_PROFILES

DESCRIPTION
  This function when invoked reads the attach profiles configured and deletes all of them
  If the attach profile happens to be a default profile, then the contents of the profile
  will be reset
    
PARAMETERS: 
  None
 
DEPENDENCIES
  None.
  
RETURN VALUE 
  boolean
 
SIDE EFFECTS 
  None 
  
===========================================================================*/
void  ds_3gpp_profile_delete_attach_profiles
(
   sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_RESET_ATTACH_PROFILE_LIST_TO_DEFAULT_PROFILE

DESCRIPTION
  This function when invoked resets the attach profile list i.e
  sets the new attach profile list to default embedded profile number.
  This will be picked up in the next attach procedure
 
  This api will also accept a fn ptr which will be called when
  updating the attach profile list is successful
    
PARAMETERS: 
  subs_id
 
DEPENDENCIES
  None.
  
RETURN VALUE 
  boolean
 
SIDE EFFECTS 
  None 
  
===========================================================================*/
void  ds_3gpp_reset_attach_profile_list_to_default_profile
(
   ds_3gpp_reset_attach_profile_list_cb_fptr cb_fn_ptr,
   sys_modem_as_id_e_type subs_id
);

#ifdef FEATURE_LAPP_UFS
#error code not present
#endif /* FEATURE_LAPP_UFS */

/*===========================================================================
FUNCTION DS_3GPP_BIT_DATA_RATE_SUPP_CONFIG

DESCRIPTION
  This function is the API for RRC. During the attach, RRC would call this
  API to get the latest configuration for bit data rate support.
    
PARAMETERS: 
  subs_id
  *config
 
DEPENDENCIES
  None.
  
RETURN VALUE 
  E_SUCCESS: valid configuration populated to *config
  E_FAILURE: No invalid configuration to *config. config = NULL
 
SIDE EFFECTS 
  None 
  
===========================================================================*/
errno_enum_type ds3gpp_get_bit_data_rate_supp_config
(
  sys_modem_as_id_e_type                     subs_id,
  ds_3gpp_ran_bit_data_rate_support_config  *config
);

/*===========================================================================
FUNCTION DS_3GPP_SET_WRRC_CB_FUNCTION

DESCRIPTION
  This function is to store wrrc callback function pointer.

PARAMETERS
  as_id: Subscription ID
  wcdma_rrc_cb_fptr: WCDMA RRC callback funciton pointer
  
DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None.
  
===========================================================================*/
void ds_3gpp_set_wrrc_cb_function
(
  sys_modem_as_id_e_type                         as_id,
  ds_3gpp_downlink_actual_throughput_ind_cb_type wcdma_rrc_cb_fptr
);

/*===========================================================================
FUNCTION DS_3GPP_SET_WRRC_CB_FUNCTION

DESCRIPTION
  This function returns pointer to to cache variable

PARAMETERS
  None
  
DEPENDENCIES
  None.

RETURN VALUE
  Cache Pointer
  
SIDE EFFECTS
  None.
  
===========================================================================*/
ds_3gpp_downlink_actual_throughput_ind_cb_type ds_3gpp_get_wrrc_cb_function
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_REG_DOWNLINK_THROUGHPUT_REPORT_CB

DESCRIPTION
  This function is exposed to WCDMA RRC to register a callback function to be 
  triggered when downlink actual throughput reporting is sent.
 
PARAMETERS
  as_id: Subscription ID
  wcdma_rrc_cb_fptr: Callback pointer passed from WCDMA RRC
  
DEPENDENCIES
  None.

RETURN VALUE  
  TRUE: registration successful
  FALSE: registration failed

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_reg_downlink_throughput_report_cb 
( 
  sys_modem_as_id_e_type                         as_id,
  ds_3gpp_downlink_actual_throughput_ind_cb_type wcdma_rrc_cb_fptr
);

/*===========================================================================
FUNCTION DS_3GPP_IS_W2L_REDIR_ENABLE

DESCRIPTION
  This function checks whether W to L redirection is enabled or not.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  TRUE: w2l redirection feature is enabled
  FALSE: w2l redirection feature is disabled

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_is_w2l_redir_enable
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_GET_W2L_REDIR_TSEARCH

DESCRIPTION
  This function gets the time(in seconds)that modem can spend in finding LTE cell.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  Time in seconds

SIDE EFFECTS
  None

===========================================================================*/
uint32 ds_3gpp_get_w2l_redir_tsearch
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_GET_W2L_REDIR_T2

DESCRIPTION
  This function gets minimum time (in seconds) that the UE must wait to
  evaluate singaling power/quality of LTE ferq while in WCDMA connected state.
  
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  Time in seconds

SIDE EFFECTS
  None

===========================================================================*/
uint32 ds_3gpp_get_w2l_redir_t2
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_GET_W2L_REDIR_LTE_RSRQ

DESCRIPTION
  This function gets Reference Signal Received Quality LTE frequency(in dB)
  must meet.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  Reference Signal Received Quality

SIDE EFFECTS
  None

===========================================================================*/
int32 ds_3gpp_get_w2l_redir_lte_rsrq
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_GET_W2L_REDIR_LTE_RSRP

DESCRIPTION
  This function gets the Reference Signal Received Power that LTE frequency
  (in dB) must meet.
 
PARAMETERS
  subs_id: Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE  
  Reference Signal Received Power

SIDE EFFECTS
  None

===========================================================================*/
int32 ds_3gpp_get_w2l_redir_lte_rsrp
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_EPSID_IS_ALLOWED_TO_USE_DISCARD_TIMER

DESCRIPTION
  This function determines if input eps_id is allowed to use discard timer
  currently,only internet default bearer is allowed 

PARAMETERS

  byte        eps_id
  uint8       subs_id--same as sys_modem_as_id_e_type

RETURN VALUE
  boolean TRUE:allowed
          FALSE:not allowed

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean  ds_3gpp_epsid_is_allowed_to_use_discard_timer
(
  byte                   eps_id,
  uint8                  subs_id
);


#endif /* DS3GPP_H */

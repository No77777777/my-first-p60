#ifndef _PS_DPM_HW
#define _PS_DPM_HW
/*===========================================================================

                     PS_DPM_HW . H
DESCRIPTION
Header file describing DPM hardware definitions.

Copyright (c) 2013-15 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/interface/dpm/src/ps_dpm_hw.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
=========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "ps_dpmi.h"
#include "ps_pkt_info_utils.h"

extern boolean ps_dpm_hw_dpl_enabled;

/**
  @brief  Hardware packet stats enum type

*/
typedef struct
{
  struct
  {
    uint64  num_ipv4_bytes;
    uint64  num_ipv6_bytes;
    uint32  num_ipv4_pkts;
    uint32  num_ipv6_pkts;    
  } dl_stats;
} ps_dpm_hw_stats_info_type;

/* @brief Struct: DSM IP packet app field info */
typedef struct
{
  /*! Filter result */
  uint32 filter_result           : 16;
  /*! Who did the filtering (Hardware of DPM) */
  uint32 filter_source           : 1; 
  /*! If filter result is valid or not */
  uint32 is_filter_result_valid  : 1;  
  /*! Version of IP packet */
  uint32 ip_version              : 2;  
  /*! HW logged the packet */
  uint32 is_packet_logged        : 1; 
  /*! Reserved for future use */
  uint32 reserved                : 2;  
  /*! Reserved for futre use for checksum */
  uint32 is_pseudo_header_cksum_computed  : 1;
  /*! If checksum is computed or not */
  uint32 is_cksum_computed       : 1;
  /*! Checksum start offset */
  uint32 cksum_start_offset      : 7;

} ps_dpm_dsm_app_field_info_type;

/* @brief Struct: DSM IP packet app ptr info */
typedef struct
{
  /*! Length of IP packet */
  uint32 pkt_length      : 16;  
  /*! Checksum value */
  uint32 cksum_value     : 16;
} ps_dpm_dsm_app_ptr_info_type;

/*===========================================================================

                         MACROS

===========================================================================*/
/*===========================================================================
MACRO PS_DPM_HW_IS_DPL_ENABLED()

DESCRIPTION
  This macro indicates if hardware dpl logging is enabled or not

PARAMETERS

RETURN VALUE

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
#define PS_DPM_HW_IS_DPL_ENABLED() (TRUE == ps_dpm_hw_dpl_enabled)
/*===========================================================================
MACRO PS_DPM_HW_PKT_INFO_GET_VERSION()

DESCRIPTION
  This macro retrieve version from pkt info

PARAMETERS
  pkt_ref_ptr : dsm item pointer
  
RETURN VALUE
  Returns version of the packet
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
#define PS_DPM_HW_PKT_INFO_GET_VERSION(dsm_item_ptr) \
ps_dpm_hw_pkt_info_get_version(dsm_item_ptr)

/*===========================================================================
MACRO PS_DPM_HW_PKT_INFO_IS_FILTER_RESULT_SET()

DESCRIPTION
  This macro checks if filter result is set by hardware or not

PARAMETERS
  pkt_ref_ptr : dsm item pointer
  
RETURN VALUE
  Returns if filter result is set or not

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
#define PS_DPM_HW_PKT_INFO_IS_FILTER_RESULT_SET(dsm_item_ptr) \
ps_dpm_hw_pkt_info_is_filter_result_set(dsm_item_ptr)

/*===========================================================================
MACRO PS_DPM_HW_PKT_INFO_GET_FILTER_RESULT()

DESCRIPTION
  This macro retrieve filter result

PARAMETERS
  pkt_ref_ptr : dsm item pointer
  
RETURN VALUE
  Returns filter result

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
#define PS_DPM_HW_PKT_INFO_GET_FILTER_RESULT(dsm_item_ptr) \
ps_dpm_hw_pkt_info_get_filter_result(dsm_item_ptr)

/**
  @brief
  Checks if checksum is computed for the packet

  @param[in] pkt   DSM packet

  @return  TRUE   : Checksum is computed for the packet.
           FALSE  : Checksum is not computed for the packet.

  @code
  @endcode
*/
#define PS_DPM_HW_IS_CKSUM_COMPUTED(dsm_item_ptr) \
   ((ps_dpm_dsm_app_field_info_type *)(&((dsm_item_ptr)->app_field)))->is_cksum_computed
 
/**
  @brief
  Checks if checksum value includes pseuodo header

  @param[in] pkt   DSM packet

  @return  TRUE   : Checksum is computed for the packet.
           FALSE  : Checksum is not computed for the packet.

  @code
  @endcode
*/ 
#define PS_DPM_HW_IS_PSEUDO_HEADER_CKSUM_COMPUTED(dsm_item_ptr) \
   ((ps_dpm_dsm_app_field_info_type *)(&((dsm_item_ptr)->app_field)))->is_pseudo_header_cksum_computed
 
/**
  @brief
  Returns checksum start offset if checksum is calculated

  @param[in] pkt   DSM packet

  @return  Checksum start offset

  @code
  @endcode
*/
#define PS_DPM_HW_GET_CKSUM_VALUE(dsm_item_ptr) \
   ((ps_dpm_dsm_app_ptr_info_type *)(&((dsm_item_ptr)->app_ptr)))->cksum_value

/**
  @brief
  Returns checksum start offset if checksum is calculated

  @param[in] pkt   DSM packet

  @return  Checksum start offset

  @code
  @endcode
*/
#define PS_DPM_HW_GET_CKSUM_START_OFFSET(dsm_item_ptr) \
   ((ps_dpm_dsm_app_field_info_type *)(&((dsm_item_ptr)->app_field)))->cksum_start_offset   
   
/**
  @brief
  Sets the checksum field of DSM app_ptr to newly computed value.

  @param[in] pkt              DSM packet
  @param[in] checksum_value   checksum value

  @return  None

  @code
  @endcode
*/
#define PS_DPM_HW_SET_CKSUM_VALUE(dsm_item_ptr, cksum) \
  ((ps_dpm_dsm_app_ptr_info_type *)(&((dsm_item_ptr)->app_ptr)))->cksum_value = cksum;
 
/**
  @brief
  Sets the checksum offload flag value

  @param[in] dsm_item_ptr        DSM packet
  @param[in] cksum_start_offset  Checksum start offset

  @return  None

  @code
  @endcode
*/
#define PS_DPM_HW_SET_CKSUM_START_OFFSET_VALUE(dsm_item_ptr, cksum_start_offset_val) \
  ((ps_dpm_dsm_app_field_info_type *)(&((dsm_item_ptr)->app_field)))->cksum_start_offset = cksum_start_offset_val;
  
/**
  @brief
  Sets the checksum offload flag value

  @param[in] dsm_item_ptr        DSM packet
  @param[in] is_cksum_computed   checksum is computed or not

  @return  None

  @code
  @endcode
*/
#define PS_DPM_HW_SET_IS_CKSUM_COMPUTED_FLAG(dsm_item_ptr, is_cksum_computed_val) \
  ((ps_dpm_dsm_app_field_info_type *)(&((dsm_item_ptr)->app_field)))->is_cksum_computed = is_cksum_computed_val;
 
/**
  @brief
  Sets the all checksum field values

  @param[in] ip_pkt_info     IP packet info
  @param[in] pkt_ref_ptr     dsm item ptr

  @return  None

  @code
  @endcode
*/
#define PS_DPM_HW_SET_CKSUM_OFFLOAD_VALUES(ip_pkt_info, dsm_item_ptr) \
{                                                                 \
  PS_PKT_INFO_SET_CKSUM_OFFLOAD_FLAG                               \
  (                                                                \
    ip_pkt_info,                                                   \
    PS_DPM_HW_IS_CKSUM_COMPUTED(dsm_item_ptr)                      \
  );                                                               \
  PS_PKT_INFO_SET_CKSUM                                            \
  (                                                                \
    ip_pkt_info,                                                   \
    ps_ntohs(PS_DPM_HW_GET_CKSUM_VALUE(dsm_item_ptr))              \
  );                                                               \
  PS_PKT_INFO_SET_CKSUM_OFFSET                                     \
  (                                                                \
    ip_pkt_info,                                                   \
    PS_DPM_HW_GET_CKSUM_START_OFFSET(dsm_item_ptr)                 \
  );                                                               \
  PS_PKT_INFO_SET_IS_PSEUDO_HEADER_CKSUM_COMPUTED                  \
  (                                                                \
    ip_pkt_info,                                                   \
    PS_DPM_HW_IS_PSEUDO_HEADER_CKSUM_COMPUTED(dsm_item_ptr)        \
  );                                                               \
}

/**
  @brief
  Returns version of the packet

  @param[in] pkt   DSM packet

  @return  TRUE   : Checksum is computed for the packet.
           FALSE  : Checksum is not computed for the packet.

  @code
  @endcode
*/ 
#define PS_DPM_HW_GET_IP_VERSION(dsm_item_ptr) \
   ((ps_dpm_dsm_app_field_info_type *)(&((dsm_item_ptr)->app_field)))->ip_version
   
   
/**
  @brief
  Returns version of the packet

  @param[in] pkt   DSM packet

  @return  TRUE   : Checksum is computed for the packet.
           FALSE  : Checksum is not computed for the packet.

  @code
  @endcode
*/ 
#define PS_DPM_HW_GET_PKT_LENGTH(dsm_item_ptr) \
   ((ps_dpm_dsm_app_ptr_info_type *)(&((dsm_item_ptr)->app_ptr)))->pkt_length
   
/**
  @brief
  Checks if HW(IPA) logged IP packet

  @param[in] pkt   DSM packet

  @return  TRUE   :  IPA logged the IP packet
               FALSE  : Otherwise.

  @code
  @endcode
*/
#define PS_DPM_HW_IS_PKT_LOGGED(dsm_item_ptr) \
   ((ps_dpm_dsm_app_field_info_type *)(&((dsm_item_ptr)->app_field)))->is_packet_logged
   
/**
  @brief
  Enable HW(IPA) logged IP packet flag

  @param[out] pkt   DSM packet

  @return  None

  @code
  @endcode
*/
#define PS_DPM_HW_SET_HW_LOGGED_PKT(dsm_item_ptr) \
   ((ps_dpm_dsm_app_field_info_type *)(&((dsm_item_ptr)->app_field)))->is_packet_logged = TRUE;

/*===========================================================================
MACRO PS_DPM_HW_PKT_IS_VERSION_IPV4()

DESCRIPTION
  This macro checks if pkt version is v4

PARAMETERS
  dsm_item_ptr : ptr to dsm item
  
RETURN VALUE
  Returns whether version of packet is v4 or not
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
#define PS_DPM_HW_PKT_IS_VERSION_IPV4 ps_dpm_hw_pkt_is_version_ipv4

/*===========================================================================
MACRO PS_DPM_HW_PKT_IS_VERSION_IPV6()

DESCRIPTION
  This macro checks if pkt version is v6

PARAMETERS
  dsm_item_ptr : ptr to dsm item
  
RETURN VALUE
  Returns whether version of packet is v6 or not
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
#define PS_DPM_HW_PKT_IS_VERSION_IPV6 ps_dpm_hw_pkt_is_version_ipv6

/*===========================================================================

                         EXTERNAL DATA DECLARATIONS

===========================================================================*/
/**
  @brief  This API updates stats from hardware

  @param[in] uid            UID
  @param[in] iface_ptr      Iface pointer
  @param[in] stats_ptr      Stats adjustment pointer

  @return
  0 : Success
  -1 : Failure   

  @code
  @endcode
*/
int ps_dpm_hw_get_iface_stat_adjustment
(
  uint8                            uid,
  ps_iface_type                  * iface_ptr,   
  ps_dpm_hw_stats_info_type      * stats_ptr
);

/**
  @brief  This API registers bearer with hardware

  @param[in] uid              Unique ID for bearer
  @param[in] eps_id           EPS ID for bearer
  @param[in] subs_id          Subscriber ID
  @param[in] l2_to_ps_wm_ptr  Pointer to l2_to_ps_wm
  @param[in] hw_to_ps_wm_ptr  Pointer to hw_to_ps_wm 
  @param[in] sio_stream_id    sio  stream id

  @param[in] rat              RAT technology    
  @param[in] ul_pkt_len       ul packet length
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int ps_dpm_hw_reg_bearer
(
  uint8                      uid,
  uint8                      eps_id,
  uint32                     subs_id,
  dsm_watermark_type	   * l2_to_ps_wm_ptr,
  dsm_watermark_type	   * hw_to_ps_wm_ptr,
  sio_stream_id_type         sio_stream_id,
  ps_sys_rat_ex_enum_type    rat
);

/**
  @brief  This API updates the uplink packet len with IPA

  @param[in] uid              Unique ID for bearer
  @param[in] ul_pkt_len       ul packet length
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int ps_dpm_hw_update_ul_pkt_len
(
  uint8                      uid,
  uint16                     uplink_pkt_size
);


/**
  @brief  This API deregisters bearer with hardware

  @param[in] uid                       Uid of the bearer
  @param[in] sio_srteam_id             SIO stream id
  @param[in] is_data_path_bridged      is data path bridged with Hardware
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int ps_dpm_hw_dereg_bearer
(
  uint8                     uid,
  sio_port_id_type          sio_port_id,
  boolean                   is_data_path_bridged
);

/**
  @brief  This API bridges bearer with hardware

  @param[in] uid               Uid of the bearer
  @param[in] sio_port_id       SIO port id
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int16 ps_dpm_hw_bridge_bearer
(
  uint8                            uid, 
  sio_port_id_type                 sio_port_id  
);

/**
  @brief  This API unbridges bearer with hardware
  @param[in] uid               UID
  @param[in] dpm_um_info_ptr   Ptr to dpm um info
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int16 ps_dpm_hw_unbridge_bearer
(
  uint8                            uid,
  sio_port_id_type                 sio_port_id 
);

/**
  @brief  This API configures dpl with hardware

  @param[in] uid               Uid of the bearer
  @param[in] dpl_pkt_len_v4    DPL v4 pkt length
  @param[in] dpl_pkt_len_v6    DPL v6 pkt length
  @param[in] dpm_um_handle     DPM um handle
  @param[in] iface_ptr         Ptr to iface
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int ps_dpm_hw_config_dpl
(
  uint8                      uid,
  uint16                     dpl_pkt_len_v4,
  uint16                     dpl_pkt_len_v6,
  uint32                     dpm_um_handle,
  ps_iface_type            * iface_ptr
);


/**
  @brief  This API unregisters dpl with hardware

  @param[in] uid               Uid of the bearer

  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
void ps_dpm_hw_dereg_dpl
(
  uint8                      uid
);

/**
  @brief  This API suspends bearer with hardware

  @param[in] uid       Uid of the bearer
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int ps_dpm_hw_suspend_bearer
(
  uint8   uid
);

/**
  @brief  This API registers callback with hardware

  @param[in] None
  
  @return None

  @code
  @endcode
*/
void ps_dpm_hw_reg_event_cb
(
  void
);


/**
  @brief  This API activates bearer with hardware

  @param[in] uid       Uid of the bearer
  
  @return None

  @code
  @endcode
*/
void ps_dpm_hw_activate_bearer
(
  uint8   uid
);

/**
  @brief  This function waits for hardware for response
          and activates signal

  @param None
  
  @return None

  @code
  @endcode
*/
void ps_dpmi_hw_wait_for_ev_cback
(
  void
);

/**
  @brief  This function is called during powerup
  
  @return

  @code
  @endcode
*/
void ps_dpm_hw_dpl_powerup_init
(
  void
);

/**
  @brief  This function is called by DPM to configure sio stream id
          with hardware
  
  @param[in] addr_family            Address family
  @param[in] dpm_rm_info_ptr        DPM RM information
  @param[in] num_bytes              Number of bytes to log

  @return

  @code
  @endcode
*/
void ps_dpm_config_dpl_sio
(
  ps_iface_addr_family_type    addr_family,
  ps_dpm_rm_info_type        * dpm_rm_info_ptr,
  uint16                       num_bytes
);

/**
  @brief  This function enables or disables dpl logging with 
          hardware
  
  @param[in] pkt_len     pkt length
  
  @return

  @code
  @endcode
*/
void ps_dpm_hw_control_dpl_logging
(
  uint32         pkt_len
);

/**
  @brief  This function is used to register the DPM DPL WM with 
          hardware
          
  @param[in] dpl_wmk_ptr - Pointer to ps_dpm_dpl_wmk
  
  @return

  @code
  @endcode
*/
void ps_dpm_hw_reg_dpl_wmk
(
  dsm_watermark_type *dpl_wmk_ptr
);

/**
  @brief  Signal handler for PS_DPM_DPL_SIGNAL
   
  @param[in] sig  - Signal
  @param[in] user_data_ptr  - User data pointer (unused) 
   
  @return  boolean   TRUE: no more processing to be done
                     FALSE: Needs to be called again.

  @code
  @endcode
*/
boolean ps_dpm_hw_dpl_sig_handler
(
  ps_sig_enum_type    sig,
  void              * user_data_ptr
);

/**
  @brief  Offload checksum to hardware
      
  @return  None

  @code
  @endcode
*/
void ps_dpm_hw_offload_checksum
(
  void
);

/**
  @brief  API returns version of the packet
   
  @param[in] pkt_ref_ptr  - Pointer to DSM packet
   
  @return  Version of the packet

  @code
  @endcode
*/
uint8 ps_dpm_hw_pkt_info_get_version
(
  dsm_item_type           * dsm_item_ptr
);

/**
  @brief  API checks if filter result is set by hardware or not
   
  @param[in] dsm_item_ptr  - Pointer to DSM packet
   
  @return  TRUE : Filter result is set by hardware
           FALSE : Filter result is not set by hardware

  @code
  @endcode
*/
boolean ps_dpm_hw_pkt_info_is_filter_result_set
(
  dsm_item_type           * dsm_item_ptr
);

/**
  @brief  API returns filter result by hardware
   
  @param[in] dsm_item_ptr  - Pointer to DSM packet
   
  @return  TFilter result set by hardware
  
  @code
  @endcode
*/
uint16 ps_dpm_hw_pkt_info_get_filter_result
(
  dsm_item_type           * pkt_ref_ptr
);

/**
  @brief  API checks if pkt version is v4 or not
   
  @param[in] version  - Version to compare
   
  @return  TRUE : Pkt version is v4 
  
  @code
  @endcode
*/
boolean ps_dpm_hw_pkt_is_version_ipv4
(
  dsm_item_type           * pkt_ref_ptr
);

/**
  @brief  API checks if pkt version is v6 or not
   
  @param[in] version  - Version to compare
   
  @return  TRUE : Pkt version is v6 
  
  @code
  @endcode
*/
boolean ps_dpm_hw_pkt_is_version_ipv6
(
  dsm_item_type           * pkt_ref_ptr
);

#ifdef FEATURE_DATA_PS_464XLAT
/**
  @brief  This function sets the clat context informtion
          per pdn on the hardware

  @param[in] clat_reg_info_ptr         clat pdn context
  @param[out] clat_hw_handle           clat hardware handle
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int ps_dpm_hw_clat_reg_pdn_context
(
  ps_dpm_clat_reg_pdn_cntxt_info_type       * clat_reg_info_ptr,
  ps_dpm_clat_hw_handle_type                * clat_hw_handle,
  ps_dpm_error_enum_type                    * dpm_err_no
);

/**
  @brief  This function sets the clat global configuration
          on the hardware

  @param[in] clat_global_cntxt_info  clat global config
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int ps_dpm_hw_clat_reg_global_context
(
  ps_dpm_clat_global_cntxt_info_type   * clat_global_cntxt_info
);

/**
  @brief  This function asociates the sio stream id with the 
          clat pdn context

  @param[in] clat_hw_handle  clat hw handle
  @param[in] sio_stream_id   stream id

  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int ps_dpm_hw_clat_assoc_sio_stream
(
  ps_dpm_clat_hw_handle_type           * clat_hw_handle,
  sio_stream_id_type                     sio_stream_id
);

/**
  @brief  This function reconfigures the clat context informtion
          per pdn on the hardware

  @param[in] clat_reg_info_ptr         clat pdn context
  @param[in] clat_hw_handle            Hardware handle
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
int ps_dpm_hw_clat_rereg_pdn_context
(
  ps_dpm_clat_reg_pdn_cntxt_info_type       * clat_reg_info_ptr,
  ps_dpm_clat_hw_handle_type                * clat_hw_handle
);

/**
  @brief  This function deregisters the clat context information
          per pdn on the hardware

  @param[in] clat_hw_handle            Hardware handle
  
  @return 
  0 : Success
  -1 : Failure

  @code
  @endcode
*/
void ps_dpm_hw_clat_dereg_pdn_context
(
  ps_dpm_clat_hw_handle_type     * clat_hw_handle
);
#endif /* FEATURE_DATA_PS_464XLAT */

/**
  @brief  Registers CB with HW for HFN error indication
             call back would be invoked in two cases
             1)when continuous invalid packet count
             reaches threshold.
             2)first valid packet after invalid count threshold.
   
  @param[in] error_pkt_threshold:   threshold count
  @param[in] err_ind_cb_fn:            call back to be invoked by HW
   
  @return  TRUE : Pkt version is v6 
  
  @code
  @endcode
*/
void ps_dpm_hw_register_bearer_err_ind
(
  uint32                           err_pkt_threshold,
  void*                            err_ind_cb_fn_ptr
);
	
/**
  @brief  Deregisters CB with HW registered for HFN error indication.
   
  @return  NA
  
  @code
  @endcode
*/
void ps_dpm_hw_deregister_bearer_err_ind
(
  void
);

/**
  @brief  Registers CB with HW for Uplink monitor application to
          retrieve information about DNS and TCP Syn packets.
   
  @return  NA
  
  @code
  @endcode
*/
void ps_dpm_hw_register_ul_monitor_cb
(
  void
);

/**
  @brief  Register PS callback function for HW loopback
   
  @param[in] loopback_cfg:  loopback configuration info
   
  @return 
  
  @code
  @endcode
*/
void ps_dpm_hw_loopback_reg_cfg
(
  ps_dpm_loopback_cfg_type * loopback_cfg
);

/**
  @brief  Register message callback function which would be used by
          the loopback module to post a message to PS task message queue
   
  @param[in] ipa_dl_send_msg_func_cb_type: message callback function pointer 
   
  @return 
  
  @code
  @endcode
*/
void ps_dpm_hw_loopback_reg_sync_msg_cb
(
  void
);

/*!
    @brief 
    This is a wrapper function for HW loopback message handler
    
    @param[in] msg_data : user message
    
    @return
*/
void ps_dpm_hw_loopback_msg_hdlr
(
  uint32 msg_data
);

#endif /* _PS_DPM_HW */

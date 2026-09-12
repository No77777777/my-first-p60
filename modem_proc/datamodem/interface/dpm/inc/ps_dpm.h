#ifndef PS_DPM_H
#define PS_DPM_H
/*===========================================================================

                     PS_DPM . H
DESCRIPTION
Header file describing global DPM config definitions.

Copyright (c) 2013-2015 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/interface/dpm/inc/ps_dpm.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/10/13    bvd     Initial Version
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "ps_iface_defs.h"
#include "ds_rmnet_meta_sm.h"
#include "dsm.h"
#include "ps_sys.h"
#include "ps_lan_llci.h"
#include "ps_ifacei.h"

/*===========================================================================

                         EXTERNAL DATA DECLARATIONS

===========================================================================*/
#define   PS_DPM_UL_PKT_INVALID_LENGTH  -1

/**
  @brief Enum describing DPM error codes
*/
typedef enum
{
  PS_DPM_SUCCESS  = 0x0,
  
  /* DPM general error codes */
  PS_DPM_ERROR_ARG_INVALID ,
  PS_DPM_ERROR_MEM_ALLOC_FAILED,
  PS_DPM_ERROR_IFACE_INVALID,
  PS_DPM_ERROR_DPM_UM_INFO_NULL,
  PS_DPM_ERROR_DPM_RM_INFO_NULL,
  PS_DPM_ERROR_DPM_BEARER_INFO_NULL,
  PS_DPM_ERROR_WMK_EMPTY,
  PS_DPM_ERROR_CLAT_CNTXT_NULL,
  PS_DPM_ERROR_CLAT_CNTXT_NOT_NULL,
  PS_DPM_ERROR_DPM_BEARER_NOT_BRIDGED,
  PS_DPM_ERROR_DPM_BEARER_BRIDGED,
  
  /* Hardware specific */
  PS_DPM_ERROR_HARDWARE_OP_FAILED,
  PS_DPM_ERROR_HARDWARE_OP_NOT_SUPPORTED,
  
  PS_DPM_ERROR_MAX
} ps_dpm_error_enum_type;

/**
  @brief Enum describing different global um dpm event
*/
typedef enum
{
  PS_DPM_UM_EV_BEARER_REG_COMPLETE,
  PS_DPM_UM_EV_IFACE_REG_COMPLETE,
  PS_DPM_UM_EV_BEARER_DEREG_COMPLETE,
  PS_DPM_UM_EV_IFACE_DEREG_COMPLETE,
  PS_DPM_UM_EV_BEARER_REREG_COMPLETE,
#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS */
} ps_dpm_um_ev_enum_type;


typedef enum
{
  PS_DPM_DATA_PATH_HARDWARE,  /* Hardware Data Path */
  PS_DPM_DATA_PATH_SOFTWARE   /* Software Data Path */
} ps_dpm_data_path_enum_type;

/* @brief Loopback type*/
typedef enum
{
  PS_DPM_NO_LOOPBACK = 0,
  PS_DPM_WM_LOOPBACK,
  PS_DPM_PC_LOOPBACK,
  PS_DPM_CB_LOOPBACK,
  PS_DPM_L2_LOOPBACK
}ps_dpm_loopback_enum_type;

/* @brief Loopback call back type */
typedef void (*ps_dpm_loopback_cb_func_ptr)
(
  uint32 user_data
);

/* @brief Loopback configuration from DS Modehandler */
typedef struct
{
   /* Type of Loopback test */
  ps_dpm_loopback_enum_type    lb_type;
   /* Call back function to initiate UL packets to be loopbacked */
  ps_dpm_loopback_cb_func_ptr  lb_cb_func_ptr;
   /* Call back function data */
  uint32                       lb_cb_func_data;
   /* Clock vote mask */
  uint32                       lb_clock_vote_mask;
}ps_dpm_loopback_cfg_type;

/**
  @brief Data type associated with registering UM with DPM
  @see   ps_dpm_um_reg
*/
typedef struct
{
  ps_iface_type            * v4_iface_ptr;
  ps_iface_type            * v6_iface_ptr;
  ps_phys_link_type        * ps_phys_link_ptr;
  dsm_watermark_type       * l2_to_ps_wm_ptr;  //l2_to_ipa  DL wmk
  ps_sys_rat_ex_enum_type    rat;
  uint8                      eps_id;
} ps_dpm_um_reg_bearer_info_type;

/**
  @brief Data type associated with registering UM with DPM
  @see   ps_dpm_um_reg
*/
typedef struct
{
  ps_iface_type            * v4_iface_ptr;
  ps_iface_type            * v6_iface_ptr;
  ps_phys_link_type        * ps_phys_link_ptr;
  dsm_watermark_type       * l2_to_ps_wm_ptr;  //l2_to_ipa  DL wmk
  ps_sys_rat_ex_enum_type    rat;
  uint8                      eps_id;
} ps_dpm_um_rereg_bearer_info_type;

/**
  @brief Data type associated with registering UM with DPM
  @see   ps_dpm_um_reg
*/
typedef struct
{
  ps_iface_type            * v4_iface_ptr;
  ps_iface_type            * v6_iface_ptr;
  ps_phys_link_type        * ps_phys_link_ptr;
  dsm_watermark_type       * l2_to_ps_wm_ptr;  //l2_to_ps  DL wmk
  dsm_watermark_type       * hw_to_ps_wm_ptr;  //hw_to_ps  DL wmk
  ps_sys_rat_ex_enum_type    rat;
  uint8                      eps_id;
} ps_dpm_um_reg_tlb_bearer_info_type;

typedef struct
{
  ps_phys_link_type        * ps_phys_link_ptr;
  uint16                     ul_pkt_len;
} ps_dpm_update_tlb_bearer_ul_len_info_type;


/**
  @brief Data type associated with registering UM with DPM
  @see   ps_dpm_um_reg
*/
typedef struct
{
  ps_iface_type  * registered_iface_ptr;
  ps_iface_type  * iface_ptr;
} ps_dpm_um_reg_iface_info_type;

/**
  @brief Data type associated with registering RM with DPM
  @see   ps_dpm_rm_reg
*/
typedef struct
{
  ps_iface_type                * rm_iface_ptr;
  dsm_watermark_type           * sio_rx_wm_ptr;
  dsm_watermark_type           * sio_tx_wm_ptr;
  rmnet_sm_link_prot_e_type      link_prot;
  rmnet_data_agg_enum_type       ul_data_agg_protocol;
  rmnet_data_agg_enum_type       dl_data_agg_protocol;
  lan_lle_enum_type              lan_llc_instance;
  sio_stream_id_type             sio_stream_id;
  uint8                          data_format;
  boolean                        te_flow_control;
  uint32                         ul_data_agg_max_num;
  uint8                          mux_id;
} ps_dpm_rm_reg_iface_info_type;

/**
  @brief Data type associated with deregistering of bearer with DPM
  @see   ps_dpm_um_dereg
*/
typedef struct
{
  ps_iface_type      * v4_iface_ptr;
  ps_iface_type      * v6_iface_ptr;
  ps_phys_link_type  * ps_phys_link_ptr;
} ps_dpm_um_dereg_bearer_info_type;


/**
  @brief Data type associated with deregistering of iface with DPM
  @see   ps_dpm_um_dereg
*/
typedef struct
{
  ps_iface_type  * v4_iface_ptr;
  ps_iface_type  * v6_iface_ptr;
} ps_dpm_um_dereg_iface_info_type;

/**
  @brief Data type associated with deregistering of DPM
  @see   ps_dpm_deregister
*/
typedef struct
{
  ps_iface_type       * rm_iface_ptr;
  sio_stream_id_type    sio_stream_id;
} ps_dpm_rm_dereg_iface_info_type;

typedef struct
{
  sio_stream_id_type               sio_stream_id;
  ps_iface_type                  * rm_iface_ptr;
  llc_frm_packed_mode_zero_type    eth_hdr;
} ps_dpm_rm_eth_hdr_info_type;

/**
  @brief Data type indicating if dpm reg/dereg is
         successful/unsuccessful.
*/
typedef struct
{
  ps_iface_type                   * v4_iface_ptr;
  ps_iface_type                   * v6_iface_ptr;
  ps_phys_link_type               * phys_link_ptr;
  ps_sys_rat_ex_enum_type           rat;
  boolean                           result;
  boolean                           is_hw_accelerated;
} ps_dpm_um_ev_cback_info_type;

/**
  @brief This is callback provided by client which is to
         be called when particular event is triggered.
*/
typedef void (* ps_dpm_um_ev_cback_fptr_type)
(
  ps_dpm_um_ev_enum_type      dpm_event,
  void                      * event_info_ptr
);

/**
  @brief Data type associated with switch data path to SW/HW
*/
typedef struct
{
  ps_dpm_data_path_enum_type             data_path;
  ps_iface_type                        * iface_ptr;
  ps_iface_data_path_clients_enum_type   iface_client;

} ps_dpm_switch_data_path_info_type;

/**
  @brief This is callback provided by Mode handlers which is to
     be called when error packet threshold count reached.
     this is called by DPM(dl_opt) in couple of cases.
     1)when continuous error packet count reached threshold
     2)first valid packet after reaching threshold.
*/
typedef void (* ps_dpm_err_ind_cback_fptr_type)
(
/* phys link of the bearer */
  ps_phys_link_type         * phys_link_ptr,
/* indicate if recovered from earlier failure */
  boolean                     resync_status
);


/**
  @brief Data type to maintain mode handler 
     specific info for HFN error indication
     registered mode handlers CB would be invoked in couple of cases:
     1)When continuous invalid packet count reaches threshold
     2)first valid packet after threshold.
*/
typedef struct
{
  uint8                           err_pkt_threshold;
  ps_dpm_err_ind_cback_fptr_type  err_ind_cb_fn_ptr;
} ps_dpm_cont_err_pkt_cb_info;


typedef enum
{
  PS_DPM_PKT_TYPE_DNS,
  PS_DPM_PKT_TYPE_TCP_SYN,
  PS_DPM_PKT_TYPE_MAX
} ps_dpm_ul_monitor_pkt_type;
/**
  @brief Data type associated with uplink monitor callbacks
*/
typedef struct
{
  ps_dpm_ul_monitor_pkt_type  pkt_type;
  ip_version_enum_type        ip_ver;
} ps_dpm_ul_monitor_info_type;

/**
  @brief This is callback provided by uplink monitor client which is to
         be called when certain packet matches filter installed by
         the client
*/
typedef int (* ps_dpm_ul_monitor_cback_fptr_type)
(
  ps_dpm_ul_monitor_info_type   * ul_monitor_info_ptr
);

/*===========================================================================

                         MACROS

===========================================================================*/
#ifdef FEATURE_DATA_PS_464XLAT
/**
  @brief
  If iface is clat iface, then retrieve assoc iface and then call 
  PS_IFACE_GET_BASE_IFACE api. For clat iface, inherit ip flag info is 
  set to false as ip address of clat is different from base iface.

  @param[in] iface_ptr    Iface pointer

  @return  Base iface pointer.

  @code
  @endcode
*/
#define PS_DPM_GET_BASE_IFACE ps_dpm_get_base_iface
INLINE ps_iface_type * ps_dpm_get_base_iface
(
  ps_iface_type        * iface_ptr
)
{
  ps_iface_type * base_iface_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (!PS_IFACE_IS_VALID(iface_ptr))
  {
    LOG_MSG_ERROR_1("Invalid iface 0x%p", iface_ptr);
    return iface_ptr;
  }
  
  if (CLAT_IFACE == PS_IFACE_GET_NAME(iface_ptr))
  {
    base_iface_ptr = PS_IFACEI_GET_ASSOC_IFACE(iface_ptr);
  }
  else
  {
    base_iface_ptr = iface_ptr;
  }
  
  if (NULL != base_iface_ptr)
  {
    return PS_IFACE_GET_BASE_IFACE(base_iface_ptr);  
  }

  return iface_ptr; 
} /* ps_dpm_get_base_iface */
#endif /* FEATURE_DATA_PS_464XLAT */

/*===========================================================================

                         EXTERNAL FUNCTIONS

===========================================================================*/
/**
  @brief  Does DPM specific initialization

  @code
  @endcode
*/
void ps_dpm_init
(
  void
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/**
  @brief  Modehandler will Register cb with dpm for
          retrieving status of registration deregistration
          completion with DPM.

  @return  N/A

  @code
  @endcode
*/
void ps_dpm_um_ev_cback_reg
(
  ps_sys_tech_enum_type                tech_type,
  ps_dpm_um_ev_cback_fptr_type         ev_cback_fptr
);


/**
  @brief  This API allows Modehandler to register bearer with
          DPM when call is brought up.

  @param[in] reg_info_ptr    Info required to register Um side

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/
int32 ps_dpm_um_reg_bearer
(
  ps_dpm_um_reg_bearer_info_type   * reg_info_ptr
);

/**
  @brief  This API allows Modehandler to register iface. MH
          would call when PDN goes from single IP PDN to
          dual IP PDN

  @param[in] reg_info_ptr    Info required to register iface

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/
int32 ps_dpm_um_reg_iface
(
  ps_dpm_um_reg_iface_info_type   * reg_info_ptr
);

/**
  @brief  This API allows RMnet to register with DPM.

  @param[in] reg_info_ptr    Info required to register Rm side
  @param[out] ps_errno       Failure reason.
                             DS_EFAULT : Invalid arguments
                             DS_NOMEMORY : Out of memory
                             DS_EINVAL : Invalid Operation

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/
int32 ps_dpm_rm_reg_iface
(
  ps_dpm_rm_reg_iface_info_type   * reg_info_ptr,
  int16                           * ps_errno
);


/**
  @brief  This API allows modehandler to deregister bearer
          with DPM

  @param[in] dereg_info_ptr    Info required to deregister

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/
int32 ps_dpm_um_dereg_bearer
(
  ps_dpm_um_dereg_bearer_info_type  * dereg_info_ptr
);

/**
  @brief  This API allows RM side to deregister with DPM

  @param[in] dereg_info_ptr     Info required to deregister

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/
int32 ps_dpm_rm_dereg_iface
(
  ps_dpm_rm_dereg_iface_info_type  * dereg_info_ptr
);


/**
  @brief  Modehandler will use this API to reregister the new
          watermark in case of IRAT scenario( W to LTE or vice versa)
          Rest of the entries in DPM remains the same except
          rat and l2_to_ps_wmk.

  @param[in] rereg_info_ptr   Info required to re-register

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/
int32 ps_dpm_um_rereg_bearer
(
  ps_dpm_um_rereg_bearer_info_type  * rereg_info_ptr
);


/**
  @brief  Modehandler will use this API to deregister the
          iface with DPM. MH would call this API when PDN
          goes from dual IP to single IP

  @param[in] dereg_info_ptr     Info required to Dereg

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/
int32 ps_dpm_um_dereg_iface
(
  ps_dpm_um_dereg_iface_info_type  * dereg_info_ptr
);

/**
  @brief  switch (Suspend or Resumes) data path path

  @return  N/A

  @code
  @endcode
*/

void ps_dpm_switch_data_path
(
  ps_dpm_switch_data_path_info_type *data_path_info
);

/**
  @brief  This API allows would be called by rmnet when it recieves
          ethernet address. For ethernet mode, we dont bridge till
          rmnet registers ethernet with Hardware.

  @param[in] rm_iface_ptr   Ptr to rm iface

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/
int32 ps_dpm_rm_config_eth_hdr
(
  ps_dpm_rm_eth_hdr_info_type  * eth_hdr_info_ptr
);

/**
  @brief  This API allows Modehandler to register hw_to_ps wmk 
          in loopback mode

  @param[in] rereg_info_ptr   Ptr to loopback registration info

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/
int32 ps_dpm_um_reg_tlb_bearer
(
  ps_dpm_um_reg_tlb_bearer_info_type   * reg_info_ptr
);


/**
  @brief  This API update the uplink packet length with the IPA in
          the loopback mode

  @param[in] ul_pkt_len_info_ptr  Ptr to update the ul_pkt_len in loopback mode info

  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/

int32 ps_dpm_update_tlb_bearer_ul_len
(
  ps_dpm_update_tlb_bearer_ul_len_info_type  * um_update_tlb_bearer_ul_len_ptr
);

/**
  @brief  Returns if data path is in software or hardware mode

  @param[in] iface_ptr   Iface ptr 
  
  @return 
  1 : Software mode
  0 : Hardware mode
  -1 : Other error 

  @code
  @endcode
*/
int ps_dpm_is_data_path_software
(
  ps_iface_type * iface_ptr
);

/**
  @brief  Returns if DPM UM info exists corresponding to the PS Iface

  @param[in] iface_ptr   Iface ptr 
  
  @return 
   TRUE:  If Um info exist for Iface
   FALSE: Otherwise? 

  @code
  @endcode
*/

boolean ps_dpm_check_dpm_um_info_exist
(
  ps_iface_type * iface_ptr
);

/**
  @brief  Offloads checksum calculation to Hardware
  
  @return  DSS_SUCCESS      On Success.
  @return  DSS_ERROR        On failure.

  @code
  @endcode
*/

int32 ps_dpm_offload_checksum
(
  void
);

/**
  @brief  This API is used to transmit IP packets on downlink towards sio path
    
  @param[in] iface_ptr        -  The interface on which to transmit the packet
  @param[in] dsm_item_ptr_ptr -  message (in dsm chain) to be transmitted
  @param[in] meta_info_ptr    -  meta info associated with the packet
  @param[in] client_info      -  user data passed that contains rmnet_meta_sm info ptr
    
  @return    0      On Success.
             -1     On Failure
    
  @code
  @endcode
*/

int ps_dpm_ip_mode_tx_cmd
(
  ps_iface_type      * iface_ptr,
  dsm_item_type      ** dsm_item_ptr_ptr,
  ps_meta_info_type  * meta_info_ptr,
  void               * client_info
);

/**
  @brief  This API would execute powersave filters on every incoming pkts on rm 
  iface and then send the data on downlink to sio path
        
  @param[in] iface_ptr     -  The interface on which to transmit the packet
  @param[in] pkt_ref_ptr   -  message (in dsm chain) to be transmitted
  @param[in] meta_info_ptr -  meta info associated with the packet
  @param[in] client_info   -  user data passed that contains rmnet_meta_sm info ptr
        
  @return    0      On Success.
             -1     On Failure
        
  @code
  @endcode
*/

int ps_dpm_powersave_mode_tx_cmd
(
  ps_iface_type      * iface_ptr,
  dsm_item_type      ** pkt_ref_ptr,
  ps_meta_info_type  * meta_info_ptr,
  void               * client_info
);


/**
  @brief  Modehandler will Register cb with dpm to know 
    1)Continuous error packet count reached threshold.
    2)first valid packet after previous failure.

  @param[in] err_pkt_thresh      error packet threshold count
  @param[in] err_ind_cb_fn       MH CB to be invoked
  
  @return  N/A

  @code
  @endcode
*/
void ps_dpm_err_ind_cback_reg
(
  uint32                         err_pkt_threshold,
  ps_dpm_err_ind_cback_fptr_type err_ind_cb_fn_ptr
);


/**
  @brief  Modehandler will deregister cb with dpm when LTE detached.
  @return  N/A

  @code
  @endcode
*/
void ps_dpm_err_ind_cback_dereg
(
  void
);

/**
  @brief  This would be invoked by HW when continuous error packet count
          reaches threshold (or) when recieved valid packet after recovery..
          DPM gets the phys_link_ptr with the uid and invokes MH callback to
          report HFN failure/success.

  @param[in] uid:                      UID of the bearer
  @param[in] resync_status:
             resync_status would be false when UE recieved threshold number
             of invalid packets continously.
             resync_status would be true when UE recieved first valid packet after
             threshold number of invlid packets.
  
  @return  N/A

  @code
  @endcode
*/
void ps_dpm_cont_err_pkt_ind_handler
(
  uint8      uid,
  boolean    resync_status
);

/*--------------------TEST FRAMEWORK APIS ONLY -----------------*/
#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/**
  @brief  
   Function checks if data path bridging is allowed based on global(NV/SYS) 
   and iface setting.

  @param[in] iface_ptr   Iface ptr 
  
  @return 
   TRUE:  If  bridging is allowed
   FALSE: Otherwise? 

  @code
  @endcode
*/
boolean ps_dpm_is_bridging_allowed
(
  ps_iface_type * iface_ptr
);

 /**
   @brief  
    Function will return companion iface
 
   @param[in] iface_ptr   Iface ptr 
   
   @return 
     iface_ptr:  Companion iface pointer
    Null: Otherwise? 
 
   @code
   @endcode
 */
 ps_iface_type * ps_dpm_get_companion_iface
(
  ps_iface_type  * um_iface_ptr
);

/**
  @brief  sets client status information in DPM Global stats structure.
          based on client status DPM decides whether
          to invoke data activity resumed system event or not.

  @param[in] subscription_id 
  @param[in] client_status 
  
  @code
  @endcode
*/
void ps_dpm_data_activity_client_status_ind
(
  ps_sys_subscription_enum_type subscription_id,
  boolean                       client_status
);

 /**
   @brief  
    Function for UpLink Monitoring module to register cback with DPM
 
   @param[in] cback_fptr   Callaback registered
   
   @return 
 
   @code
   @endcode
 */
void ps_dpm_reg_ul_monitor_cback
(
  ps_dpm_ul_monitor_cback_fptr_type  cback_fptr
);

 /**
   @brief  
    Function for UpLink Monitoring module to deregister cback with DPM
   
   @return 
 
   @code
   @endcode
 */
void ps_dpm_dereg_ul_monitor_cback
(
  void
);

/*
    @brief 
    MH will invoke this API to register PS callback function for testing loopback. 
    User can enable loopback via diag commands.  
    Callback function to be called to dequeue and loopback UL packet.
    Watermark pointer to be passed as part of callback API.
    Callback data to be passed along with callback function.
    
    @param[in]  loopback_cfg: Loop back configuration
    
    @return None

*/
void ps_dpm_loopback_reg_cfg
(
  ps_dpm_loopback_cfg_type *loopback_cfg
);

/*
    @brief 
    This funtion returns iface ptrs that has the phys link ptr passed in
    
    @param[in]  phys_link_ptr: phys link ptr
                ip_vsn       : IP version of the iface
    
    @return ps_iface_type*: um_iface_ptr

*/
ps_iface_type * ps_dpm_um_iface_from_phys_link
(
  ps_phys_link_type                      *phy_link_ptr,
  ip_version_enum_type                    ip_vsn
);

#endif /* PS_DPM_H */

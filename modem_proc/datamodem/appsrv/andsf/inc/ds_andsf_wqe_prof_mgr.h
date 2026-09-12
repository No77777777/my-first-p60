/*===========================================================================
                      DS_ANDSF_WQE_PROF_MGR.H

DESCRIPTION
 Header file for wqe profile managing.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2015 - 2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
05/14/2015   fjia                  First version of file
===========================================================================*/
#ifndef DS_ANDSF_WQE_PROF_MGR_H
#define DS_ANDSF_WQE_PROF_MGR_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"

#ifdef FEATURE_DATA_ANDSF_RULE_MGR
#include "ds_sys.h"
#include "ps_sys_ioctl.h"
#include "ps_sys_event.h"
#include "ps_sys.h"

#define DS_ANDSF_WQE_MAX_CLIENT 2

/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/
typedef ps_sys_set_wqe_profile_quality_meas_info_type ds_andsf_wqe_prof_wifi_thres_info_type;

typedef struct
{
  ps_sys_subscription_enum_type subs_id;
  ps_sys_wqe_profile_type       profile_info;
} ds_andsf_wqe_prof_wqe_prof_type;

typedef struct
{
  ps_sys_subscription_enum_type subs_id;
  ps_sys_set_wqe_profile_quality_meas_info_list_type threshold_list;
} ds_andsf_wqe_prof_wifi_thres_ioctl_list_type;

typedef struct
{
  ps_sys_subscription_enum_type          subs_id;
  ps_sys_quality_meas_policy_change_type threshold;
} ds_andsf_policy_threshold_chg_type;

typedef struct
{
  ps_sys_subscription_enum_type           subs_id;
  ps_sys_wlan_set_wqe_profile_status_type status_info;
} ds_andsf_wqe_prof_status_type;

typedef struct
{
  /**< is RSSI high threshold valid */
  int16 rssi_high;
  /**< RSSI high threshold*/
  boolean is_rssi_high_valid;
  /**< RSSI low threshold*/
  int16 rssi_low;
  /**< is RSSI low threshold*/
  boolean is_rssi_low_valid;
} ds_andsf_wqe_cfg_param_type;



/*===========================================================================
                           FUNCTION DECLARATIONS
===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================
FUNCTION      DS_ANDSF_WQE_PROF_MGR_INIT

DESCRIPTION
  Initialize WQE profile manager module.

PARAMETERS
  None

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_andsf_wqe_prof_mgr_init(void);

/*===========================================================================
FUNCTION      DS_ANDSF_SYS_IOCTL_SET_WQE_PROF_TYPE

DESCRIPTION
  This function handles PS_SYS_IOCTL_SET_WQE_PROFILE_TYPE from application.
  It caches requested wqe_profile_type from applciation, e.g. IMS, and
  sets wqe profile type to CNE through PS SYS event.

PARAMETERS
  arg_val_ptr- carrys required information
  ps_errno- type of error

DEPENDENCIES
  None.

RETURN VALUE
 0- Success
 -1- Failure

SIDE EFFECTS
  None
===========================================================================*/
int ds_andsf_sys_ioctl_set_wqe_prof_type(
                                         void*                         arg_val_ptr,
                                         ps_sys_subscription_enum_type subs_id,
                                         int16*                        ps_errno
                                         );

/*===========================================================================
FUNCTION      DS_ANDSF_WQE_PROF_MGR_SYS_IOCTL_WIFI_THRES_UPDATE

DESCRIPTION   This function handles PS_SYS_IOCTL_SET_WQE_PROFILE_THRESOLD_INFO
              from application. It caches updated wifi threshold
              information from applciation, e.g. IMS, and pass the updated wifi
              threshold inforamation to CNE through PS SYS event.

PARAMETERS    arg_val_ptr- carrys required information
              ps_errno- type of error

DEPENDENCIES  None.

RETURN VALUE   0- Success
               -1- Failure

SIDE EFFECTS  None.
===========================================================================*/
int ds_andsf_wqe_prof_mgr_sys_ioctl_wifi_thres_update(
                                                      void*                         arg_val_ptr,
                                                      ps_sys_subscription_enum_type subs_id,
                                                      int16*                        ps_errno
                                                      );

/*===========================================================================
FUNCTION      DS_ANDSF_WQE_PROF_MGR_GET_THRESHOLD_CACHE_INDEX

DESCRIPTION
  This function returns on which bit the WQE profile mask bit 1 is set.
  This index will be mapped to threshold cache array index.
  Please note that this search function will only work if the wifi threshold
  update from application is done on a per profile basis (means there is only
  one bit set to 1).

PARAMETERS
  WQE profile mask

DEPENDENCIES
  None.

RETURN VALUE
  On which bit is the number "1" sit for the 64 bit profile mask.
  For default profile, 0 will be returned.

SIDE EFFECTS
  None
===========================================================================*/
uint8 ds_andsf_wqe_prof_mgr_get_threshold_cache_index(
                                                      uint64 wqe_prof_mask,
                                                      uint32 subs_idx,
                                                      uint32 client_id
                                                      );

ds_andsf_wqe_prof_wifi_thres_info_type* ds_andsf_wqe_prof_mgr_get_threshold_cache_info(
                                                                                       uint64 wqe_prof_mask,
                                                                                       uint32 subs_idx,
                                                                                       uint32 client_id
                                                                                       );


/*===========================================================================
FUNCTION      DS_ANDSF_WQE_PROF_MGR_WLAN_SRV_CHG_CB

DESCRIPTION   This callback is called to notify WLAN status change.

PARAMETERS    None.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_andsf_wqe_prof_mgr_wlan_srv_chg_cb(
  ps_sys_tech_enum_type         tech_type,
  ps_sys_event_enum_type        event_name,
  ps_sys_subscription_enum_type subs_id,
  void*                         event_info_ptr,
  void*                         user_data_ptr
                                           );


/*===========================================================================
FUNCTION      DS_ANDSF_SYS_IOCTL_SET_WQE_PROF_QUALITY_STATUS

DESCRIPTION
  This function handles PS_SYS_IOCTL_SET_WQE_PROFILE_QUALITY_STATUS from AP.

PARAMETERS
  arg_val_ptr - carrys status information
  ps_errno    - type of error

DEPENDENCIES
  None.

RETURN VALUE
 0- Success
 -1- Failure

SIDE EFFECTS
  None
===========================================================================*/
int ds_andsf_sys_ioctl_set_wqe_prof_quality_status(
  void*                         arg_val_ptr,
  ps_sys_subscription_enum_type subs_id,
  int16*                        ps_errno
                                                   );

/*===========================================================================
FUNCTION      DS_ANDSF_WQE_PROF_MGR_POLICY_CHANGE_REQ_CB

DESCRIPTION   This callback is called when policy threshold(LTE/1X/WIFI)
              changed.

PARAMETERS    Tech type for the PS_SYS event registered.
              event name for the PS_SYS event registered.
              ptr to the event info.
              ptr to the user data ptr.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_andsf_wqe_prof_mgr_policy_change_req_cb(
  ps_sys_tech_enum_type         tech_type,
  ps_sys_event_enum_type        event_name,
  ps_sys_subscription_enum_type subs_id,
  void                          *event_info_ptr,
  void                          *user_data_ptr
                                                );


uint16 ds_andsf_notify_policy_chg_req(
                                      ds_sys_subscription_enum_type             subs_id,
                                      const ds_andsf_policy_threshold_chg_type* threshold_info_ptr
                                      );

void ds_andsf_wqe_prof_mgr_set_quality_status(ds_andsf_wqe_prof_status_type* wqe_prof_status_ptr);

void ds_andsf_wqe_prof_mgr_nv_refresh(uint32 subs_idx);

void ds_andsf_wqe_prof_set_threshold_cache(
                                           ds_andsf_wqe_cfg_param_type* cache_info,
                                           uint16                       array_index,
                                           uint32                       subs_idx,
                                           uint32                       client_id
                                           );
uint32 ds_andsf_wqe_get_client_idx_from_meas_id(uint64 measurement_id);
boolean ds_andsf_wqe_is_modem_cne_profiles_func_enabled(void);

#ifdef __cplusplus
}
#endif

#endif /*FEATURE_DATA_ANDSF_RULE_MGR*/
#endif /* DS_ANDSF_RAN_MEAS_H */

/*===========================================================================
                      DS_ANDSF_CONFIG.H

DESCRIPTION
 Config header file for ANDSF.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2014-2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
06/09/14    Youjunf                          First version of file
===========================================================================*/
#ifndef DS_ANDSF_CONFIG_H
#define DS_ANDSF_CONFIG_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_ANDSF

#include "ds_andsf_wqe_prof_mgr.h"
#include "ds_andsf_api.h"

/*===========================================================================
                   EXTERNAL DEFINITIONS AND TYPES
===========================================================================*/
#define DS_ANDSF_CFG_FILE_ROOT_PATH          "/data"
#define DS_ANDSF_CFG_FILE_PATH               "/data/ds_andsf_config.txt"
#define DS_ANDSF_CFG_WQE_WIFI_THRESHOLD_ROOT_PATH "/data"

#ifdef __cplusplus
extern "C"
{
#endif


/*===========================================================================
                    EXTERNAL FUNCTION PROTOTYPES
===========================================================================*/
void ds_andsf_cfg_read_from_efs(uint8 subs_idx);
void ds_andsf_cfg_set_default_params(uint8 subs_idx);
void   ds_andsf_check_policy_in_efs(uint8 subs_idx);
uint8  ds_andsf_cfg_get_rule_mgr_active_status(uint8 subs_idx);
float  ds_andsf_cfg_get_lte_meas_alpha(uint8 subs_idx);
uint32 ds_andsf_cfg_get_lte_sample_interval(uint8 subs_idx);
uint32 ds_andsf_cfg_get_lte_avg_interval(uint8 subs_idx);
float  ds_andsf_cfg_get_min_wifi_meas_alpha(void);
uint32 ds_andsf_cfg_get_min_wifi_sample_interval(void);;
uint32 ds_andsf_cfg_get_min_wifi_avg_interval(void);
float  ds_andsf_cfg_get_1x_meas_alpha(uint8 subs_idx);
uint32 ds_andsf_cfg_get_1x_avg_interval(uint8 subs_idx);
uint32 ds_andsf_cfg_get_1x_acq_hyst_interval(uint8 subs_idx);
uint32 ds_andsf_cfg_get_1x_lost_hyst_interval(uint8 subs_idx);
uint32 ds_andsf_cfg_get_1x_sample_interval(uint8 subs_idx);
uint16 ds_andsf_cfg_get_pdn_profile_id(uint8 subs_idx);
char*  ds_andsf_cfg_get_policy_server_url(uint8 subs_idx);
uint8  ds_andsf_cfg_get_policy_max_retry(uint8 subs_idx);
uint32 ds_andsf_cfg_get_policy_retry_interval(uint8 subs_idx);
uint32 ds_andsf_cfg_get_policy_refresh_interval(uint8 subs_idx);
uint16 ds_andsf_cfg_get_sms_dest_port(uint8 subs_idx);
void   ds_andsf_cfg_get_http_username(uint8 subs_idx, char *username);
void   ds_andsf_cfg_get_http_password(uint8 subs_idx, char *password);

// WQE cache file fcn
boolean ds_andsf_cfg_wqe_construct_wifi_thres_efs(
                                                  ds_andsf_wqe_prof_wifi_thres_info_type *threshold_cache_ptr,
                                                  ds_andsf_wqe_prof_iccid_info_s_type    *iccid_info_ptr,
                                                  uint32                                  subs_idx,
                                                  uint32                                  client_idx
                                                  );
void ds_andsf_cfg_wqe_read_from_efs(uint32 subs_idx);

#ifdef __cplusplus
}
#endif

#endif /* FEATURE_DATA_ANDSF */
#endif /* DS_ANDSF_CONFIG_H */

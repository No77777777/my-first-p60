#ifndef DS_MPPM_EFS_PARSER_H
#define DS_MPPM_EFS_PARSER_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
         
                      D S  M P P M  E F S  P A R S E R

GENERAL DESCRIPTION

  All the declarations and definitions necessary to support parsing  
  of MPPM database related EFS file.

Copyright (c) 2012-2020 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*==================================================================
                        EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/wlan/mapcon/inc/ds_mppm_efs_parser.h#4 $

when       who         what, where, why
--------   -----       ----------------------------------------------------------
12/02/19   vl          Added support for rat preference IMS_PREFERRED
01/13/2020 vls         Added this_pdn_timer_info_ptr to avoid accessing 
                       already freed pdn_timer_info_ptr in expiry function.
05/21/2018 vls         Added changes for introducing time period before processing 
                       WIFI UI switch
05/23/2018 vl          Added changes for FR 47777: IWLAN refactoring for AndroidP
06/29/2017 ak          Added support handover between ePDG and 2G(CR 2011611)
06/15/2016 fn          Added support for dsds capable subs
03/02/2015 vl          Added support handover between ePDG and 3G(FR 20728)
03/14/2015 vl          Add support for FR: No handover in roaming LTE network
07/14/2014 scb         Add support for ds_mppm_get_pdn_policy_db_entry
06/30/2014 fn          MPPM queue changes per subscription
24/04/2014 scb         Add support for per PDN T_MINONWWAN_TIMER on IWLAN
11/16/2013 vl          Added changes to write PDN DB Queue contents to EFS
                       file
11/23/2013 scb         Added support for PDN DB UPDATE INFO in MPPM
11/21/2013 scb         Added support for WLAN_MAX_CONN reached indication in MPPM
09/23/2013 scb         Added support for throttling in MPPM.
09/16/2013 scb         Added changes to support LTE_ATTACH_IN_PROGRESS support
10/23/2012 scb         Create MPPM EFS module
===========================================================================*/

/*===========================================================================
                              INCLUDE FILES
===========================================================================*/
#include "queue.h"

#ifdef FEATURE_DATA_WLAN_MAPCON

#include "rex.h"
#include "ds_sys.h"
#include "ps_sys.h"
#include "dsutil.h"
#include "data_msg.h"
#include "modem_mem.h"
#include "ds_profile.h"

#ifdef FEATURE_LAPP_UFS
#error code not present
#else
#define PDN_CONFIG_MAX_SUPPORTED_RATS 3
#endif /*FEATURE_LAPP_UFS*/

#define TECH_NON_EPC_WWAN_POLICY_MASK (0x10000000)
#ifdef FEATURE_LAPP_UFS
#error code not present
#endif /*FEATURE_LAPP_UFS*/
#define TECH_NON_EPC_WWAN_POLICY_MASK_WITH_WCDMA_GSM (0x10000000)
#define TECH_GSM_POLICY_MASK                         (0x00100000)
#define TECH_WCDMA_POLICY_MASK                       (0x00010000)
#define TECH_LTE_POLICY_MASK                         (0x00001000)
#define TECH_EHRPD_POLICY_MASK                       (0x00000100)
#define TECH_WIFI_LB_POLICY_MASK                     (0x00000010)
#define TECH_WIFI_IWLAN_POLICY_MASK                  (0x00000001)

#ifdef FEATURE_LAPP_UFS
#error code not present
#else                                         
#define TECH_WWAN_WLAN_POLICY_MASK ( TECH_WIFI_IWLAN_POLICY_MASK | \
                                         TECH_WIFI_LB_POLICY_MASK | \
                                         TECH_EHRPD_POLICY_MASK | \
                                         TECH_LTE_POLICY_MASK | \
                                         TECH_NON_EPC_WWAN_POLICY_MASK )
#endif /*FEATURE_LAPP_UFS*/
                                         
#define TECH_EPC_WWAN_POLICY_MASK  ( TECH_LTE_POLICY_MASK | \
                                     TECH_EHRPD_POLICY_MASK)

#define TECH_WWAN_POLICY_MASK  ( TECH_LTE_POLICY_MASK | \
                                 TECH_EHRPD_POLICY_MASK | \
                                 TECH_NON_EPC_WWAN_POLICY_MASK )

#ifdef FEATURE_LAPP_UFS
#error code not present
#else
#define DS_MPPM_MAX_TECH_SUPPORTED 7
#endif /*FEATURE_LAPP_UFS*/

#define DS_SYS_MAX_APN_STR_LEN   DS_SYS_MAX_APN_LEN-1

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
    MAX PDN COUNT - Total number of profiles + buffer
---------------------------------------------------------------------------*/
#define DS_MAX_PDN_COUNT                    PS_SYS_MAX_APNS

/*===========================================================================
                              DATA STRUCTURES
===========================================================================*/

/*---------------------------------------------------------------------------
    Data structures for AP assist mode.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_MPPM_PREF_SYS_TYPE_NONE = -1,
  DS_MPPM_PREF_SYS_TYPE_WWAN = PS_SYS_APN_PREF_SYS_WWAN,
  DS_MPPM_PREF_SYS_TYPE_IWLAN = PS_SYS_APN_PREF_SYS_IWLAN
} ds_mppm_pref_sys_enum_type;

/*---------------------------------------------------------------------------
  MPPM tech mask enumeration.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_MPPM_TECH_MASK_NON_EPC_WWAN,
  DS_MPPM_TECH_MASK_WWAN_WLAN,
  DS_MPPM_TECH_MASK_EPC_WWAN,
  DS_MPPM_TECH_MASK_WWAN,
#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /*FEATURE_LAPP_UFS*/
  DS_MPPM_TECH_MASK_MAX 
} ds_mppm_tech_mask_enum_type;

/*---------------------------------------------------------------------------
  Enum for override types
---------------------------------------------------------------------------*/
typedef enum
{
  MPPM_OVERRIDE_TYPE_NONE = 0,
  MPPM_OVERRIDE_TYPE_API,
  MPPM_OVERRIDE_TYPE_OPTION,
  MPPM_OVERRIDE_TYPE_MAX = MPPM_OVERRIDE_TYPE_OPTION
}ds_mppm_override_type;

/*---------------------------------------------------------------------------
  Per PDN timer pointer.
---------------------------------------------------------------------------*/
typedef enum
{
  MPPM_PDN_TIMER_TYPE_NONE = 0,
  MPPM_PDN_TIMER_TYPE_T_MINONWWAN,
  MPPM_PDN_TIMER_TYPE_BLOCK_EHRPD_FOR_IWLAN,
  MPPM_PDN_TIMER_TYPE_WIFI_SWITCH_HYSTERESIS,
  MPPM_PDN_TIMER_TYPE_AP_ASSIST_INTENT_RESP,
  MPPM_PDN_TIMER_TYPE_MAX
}ds_mppm_pdn_timer_e_type;

/*---------------------------------------------------------------------------
  MPPM APN NAME structure
---------------------------------------------------------------------------*/
typedef struct
{
  uint32                              apn_string_len;
  char                                apn_string[DS_SYS_MAX_APN_LEN];
}ds_mppm_apn_name_type;

/*---------------------------------------------------------------------------
  MPPM PDN TIMER INFO TYPE structure
  //TODO-SAP: Adding dsd_subs_id, remove when DSDA support is added.
---------------------------------------------------------------------------*/
typedef struct ds_mppm_pdn_timer_info_type_s ds_mppm_pdn_timer_info_type;

struct ds_mppm_pdn_timer_info_type_s
{
  ds_mppm_pdn_timer_info_type   *this_pdn_timer_info_ptr;
  ps_sys_subscription_enum_type  dsd_subs_id;
  ds_mppm_pdn_timer_e_type       pdn_timer_type;
  rex_timer_type                 pdn_timer;
};


/*---------------------------------------------------------------------------
    current_pref_sys     -
    prev_pref_sys        -
    intent_pref_sys      - Pref_sys sent in the INTENT to AP.
    pref_sys_set_by_ap   - Indicates AP set the pref_sys
    send_intent          - to indicate whether INTENT should be sent to AP.
---------------------------------------------------------------------------*/
typedef struct
{
  ds_mppm_pref_sys_enum_type    current_pref_sys;
  ds_mppm_pref_sys_enum_type    prev_pref_sys;
  ds_mppm_pref_sys_enum_type    intent_pref_sys;
  boolean                       pref_sys_set_by_ap;
  boolean                       send_intent;
  ds_mppm_pdn_timer_info_type   *pdn_timer_info_ptr;
}ds_mppm_ap_assist_info_s_type;

/*---------------------------------------------------------------------------
  MPPM PROFILE UPDATE CB DATA TYPE structure.
  This is the callback data structure used while registering for per profile                             .
  upadtes. (Whenever a PDN comesup we register for profile update for that
  profile so that we can get any romaing ho flag updates)
---------------------------------------------------------------------------*/
typedef struct ds_mppm_profile_update_cb_data_type_s 
                                       ds_mppm_profile_update_cb_data_type;
struct ds_mppm_profile_update_cb_data_type_s
{
  ds_mppm_profile_update_cb_data_type   *this_profile_cb_data_ptr;
  ds_mppm_apn_name_type                 apn_name;
  ds_profile_cb_handle_type             profile_handle;
};

#define DS_MPPM_PROFILE_CB_DATA_PTR_IS_VALID(profile_cb_data_ptr)         \
  ((profile_cb_data_ptr != NULL) &&                                       \
  (profile_cb_data_ptr->this_profile_cb_data_ptr == profile_cb_data_ptr))     

#define DS_MPPM_PDN_TIMER_INFO_PTR_IS_VALID(pdn_timer_info_ptr)         \
  ((pdn_timer_info_ptr != NULL) &&                                       \
  (pdn_timer_info_ptr->this_pdn_timer_info_ptr == pdn_timer_info_ptr))     

/*---------------------------------------------------------------------------
  MPPM UP PDN INFO TYPE structure.
  Rat, profile_num, dsd_subs_id are valid when PDN is up. profile_num
  is used to register for profile updates and the profile handle is stored
  in profile_handle. iwlan_to_lte_roaming_ho_supported and 
  lte_to_iwlan_roaming_ho_supported flags are read from profile and indicate
  whether handover in roaming is supported for the respective technology.
---------------------------------------------------------------------------*/
typedef struct
{
  ps_sys_subscription_enum_type dsd_subs_id;
  uint16                        profile_num;
  ds_mppm_profile_update_cb_data_type *profile_update_cb_data_ptr;
  ps_sys_pdn_status_enum_type   v4_pdn_status;
  ps_sys_pdn_status_enum_type   v6_pdn_status;
  ps_sys_tech_enum_type         tech_type;
  ps_sys_rat_ex_enum_type       rat;
  boolean                       iwlan_to_lte_roaming_ho_supported;
  boolean                       lte_to_iwlan_roaming_ho_supported;
  ds_mppm_pdn_timer_info_type   *pdn_timer_info_ptr;
}ds_mppm_up_pdn_info_type;

/*---------------------------------------------------------------------------
  Policy database entry element structure.
  The link element is supposed to be the first field for every element of
  the queue structure.
  The APNname contains a valid APN string with length > 0.
  NULL APNs are indicated by the "NULL" string.
  overide_type is used to indicate the override type of type
  ds_mppm_override_type.
  supported_rat_priority_list is a priority list of utmost 3 RATs which
  typically indicates the priority order in which the APN supports
  WWAN, IWLAN and WLAN_LB RAT types.
  Added boolean to indicate lte_attach_in_progress for a particular entry
  Additional override_mask which will be changed for a particular PDN
  depending on inputs. The inputs that will manipulate this mask currently
  are: MPPM_TECH_THROTTLE, IWLAN_MAX_PDN, T_MINONWWAN_TIMER.   
---------------------------------------------------------------------------*/
typedef struct
{
  q_link_type                   link;
  ds_mppm_apn_name_type         apn_name;
  boolean                       default_apn;
  ds_mppm_override_type         override_type;
  uint32                         
                  supported_rat_priority_list[PDN_CONFIG_MAX_SUPPORTED_RATS];
  uint32                         
                  original_rat_priority_list[PDN_CONFIG_MAX_SUPPORTED_RATS];
  boolean                       lte_attach_in_progress;
  uint32                        override_mask;
  uint32                        override_cause_list[DS_MPPM_MAX_TECH_SUPPORTED];
  boolean                       emergency_calls_supported;
  ds_mppm_up_pdn_info_type      *up_pdn_info_ptr;
  uint32                         
                  andsf_base_rat_priority_list[PDN_CONFIG_MAX_SUPPORTED_RATS]; //TODO-vl:Make heap ptr
  ds_mppm_ap_assist_info_s_type *ap_assist_info_ptr;
  ds_mppm_pdn_timer_info_type   *wifi_switch_hys_pdn_timer_info_ptr;
}mppm_pdn_policy_database_entry_s_type;

/*---------------------------------------------------------------------------
  Enum for parameter names specified in the profile file in EFS.
  Any update to this enum requires a corresponding update to
  mppm_pdn_policy_db_param_name_id.
---------------------------------------------------------------------------*/
typedef enum
{
  MPPM_PDN_POLICY_DB_PARAM_ID_MIN = 0,
  PDN_APN_STRING = MPPM_PDN_POLICY_DB_PARAM_ID_MIN,
  DEFAULT_APN,
  OVERRIDE_TYPE,
  SUPPORTED_RAT_PRIORITY_LIST,
  MPPM_PDN_POLICY_DB_PARAM_ID_MAX = SUPPORTED_RAT_PRIORITY_LIST
}mppm_pdn_policy_db_param_name_type;

/*===========================================================================
                      EXTERNAL FUNCTION DECLARATIONS
===========================================================================*/

/*===========================================================================
FUNCTION      DS_MPPM_INIT_TECH_MASKS

DESCRIPTION   

PARAMETERS    

DEPENDENCIES  None

RETURN VALUE  

SIDE EFFECTS  .
===========================================================================*/
void ds_mppm_init_tech_masks();

/*===========================================================================
FUNCTION      DS_MPPM_INIT_TECH_MASKS_PER_SUBS

DESCRIPTION   

PARAMETERS 
  subscription_id  - subs_id in ps_sys format 

DEPENDENCIES  None

RETURN VALUE  

SIDE EFFECTS  .
===========================================================================*/
void ds_mppm_init_tech_masks_per_subs
( 
  ps_sys_subscription_enum_type subscription_id 
);

/*===========================================================================
FUNCTION      DS_MPPM_GET_TECH_MASK

DESCRIPTION   

PARAMETERS    

DEPENDENCIES  None

RETURN VALUE  

SIDE EFFECTS  .
===========================================================================*/
uint32 ds_mppm_get_tech_mask
(
  ps_sys_subscription_enum_type    dsd_subs_id,
  ds_mppm_tech_mask_enum_type      tech_mask_type
);

/*===========================================================================
FUNCTION      DS_MPPM_ADD_PDN_TO_POLICY_DB

DESCRIPTION   This function will add a PDN entry to the policy database

PARAMETERS    *pdn_policy_db_q - Pointer to queue to store PDN database 
                                 entries
              *apn_name_ptr - APN name associated with PDN entry.
              *fallback_apn_pdn_policy_db_entry - Fallback PDN data entry

DEPENDENCIES  None

RETURN VALUE  TRUE - if successful, FALSE otherwise

SIDE EFFECTS  The mppm_pdn_policy_db_q will be populated with default values 
              for the PDN specified by the apn_name string or with the
              contents of the fallback_pdn_db_entry if VALID.
===========================================================================*/
boolean ds_mppm_add_pdn_to_policy_db_by_apn
(
  q_type                                *pdn_policy_db_q,
  ds_mppm_apn_name_type                 *apn_name_ptr,
  mppm_pdn_policy_database_entry_s_type *fallback_apn_pdn_policy_db_entry,
  boolean                               new_apn_policy,
  ps_sys_subscription_enum_type         dsd_subs_id
);

/*===========================================================================
FUNCTION      DS_MPPM_ADD_UP_PDN_INFO_TO_POLICY_DB_ENTRY

DESCRIPTION   This function will add UP PDN Info entry to the PDN DB Entry

PARAMETERS 
 
DEPENDENCIES  None

RETURN VALUE  TRUE - if successful, FALSE otherwise

SIDE EFFECTS  
===========================================================================*/
boolean ds_mppm_add_up_pdn_info_to_policy_db_entry
(
  mppm_pdn_policy_database_entry_s_type   *pdn_policy_db_entry_ptr
);

/*===========================================================================
FUNCTION      DS_MPPM_DELETE_UP_PDN_INFO_FROM_POLICY_DB_ENTRY

DESCRIPTION   This function will delete UP PDN Info entry from the PDN DB 
              Entry

PARAMETERS 
 
DEPENDENCIES  None

RETURN VALUE  TRUE - if successful, FALSE otherwise

SIDE EFFECTS  
===========================================================================*/
void ds_mppm_delete_up_pdn_info_from_policy_db_entry
(
  mppm_pdn_policy_database_entry_s_type   *pdn_policy_db_entry_ptr
);

/*===========================================================================
FUNCTION      DS_MPPM_READ_PDN_POLICY_DB_FROM_EFS

DESCRIPTION   This function will read PDN rules associated with options
              from EFS

PARAMETERS    q_type *q - Pointer to queue to store database entries
              char* efs_file_name - EFS configuration file to be read
                and parsed to store database entry.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  The mppm_pdn_policy_db_q will be populated with values read
              from EFS.
===========================================================================*/
boolean ds_mppm_read_pdn_policy_db_from_efs
(
  q_type           *q, 
  char			   *efs_file_name,
  uint8             mppm_subs_idx
);

/*===========================================================================
FUNCTION      DS_MPPM_PDN_POLICY_DB_Q_COMPARE_FCT

DESCRIPTION   Utility function to compare two items in the pdn db queue.

DEPENDENCIES  None.

RETURN VALUE  1: if the items are equal
              0: if items are not equal

SIDE EFFECTS  None.
===========================================================================*/
int ds_mppm_pdn_policy_db_q_compare_fct
(
  void *item_ptr,
  void *target_val
);

/*===========================================================================
FUNCTION      DS_MPPM_PRINT_POLICY_DB_ENTRY

DESCRIPTION   This function will print contents of a PDN entry in the 
              policy database

PARAMETERS    pdn_policy_db_entry - Pointer to PDN DB entry

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  
===========================================================================*/
void ds_mppm_print_policy_db_entry
(
  mppm_pdn_policy_database_entry_s_type *pdn_policy_db_entry
);

/*===========================================================================
FUNCTION      DS_MPPM_GET_AGGR_SUPPORTED_RAT_PRIORITY_LIST

DESCRIPTION   This function return the OR of rats in 
              supported_rat_priority_list

PARAMETERS    pdn_policy_db_entry - Pointer to PDN DB entry

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  
===========================================================================*/
uint32 ds_mppm_get_aggr_supported_rat_priority_list
(
  mppm_pdn_policy_database_entry_s_type *pdn_policy_db_entry
);

/*===========================================================================
FUNCTION  DS_MPPM_COPY_PDN_POLICY_DB_Q

DESCRIPTION This function is used to copy the contents from one PDN policy DB
            queue to another as is.
      
PARAMETERS
  None.
    
DEPENDENCIES  All elements of the queue should be of type 
              mppm_pdn_policy_database_entry_s_type
  
RETURN VALUE  Number of elements in the from_q_ptr that was copied.
  

SIDE EFFECTS
  None.

===========================================================================*/
uint8 ds_mppm_copy_pdn_policy_db_q
( 
  q_type *to_q_ptr,
  q_type *from_q_ptr
);

/*===========================================================================
FUNCTION      DS_MPPM_GET_PDN_POLICY_DB_ENTRY

DESCRIPTION   Utility function to get a pointer to the pdn_policy_db
              in the pdn db queue.

DEPENDENCIES  None.

RETURN VALUE  1: if the items are equal
              0: if items are not equal

SIDE EFFECTS  None.
===========================================================================*/
mppm_pdn_policy_database_entry_s_type* ds_mppm_get_pdn_policy_db_entry
(
  q_type                           *q_ptr,
  ds_mppm_apn_name_type            *apn_name_ptr,
  ps_sys_subscription_enum_type     dsd_subs_id
);

/*===========================================================================
FUNCTION      DS_MPPM_CONVERT_APN_STRING_TO_MPPM_APN_NAME_TYPE

DESCRIPTION 
 
DEPENDENCIES  None.

RETURN VALUE  

SIDE EFFECTS  None.
===========================================================================*/
void ds_mppm_convert_apn_string_to_mppm_apn_name_type
(
  ds_mppm_apn_name_type            *apn_name_ptr,
  char                             *apn_string_ptr
);

/*===========================================================================
FUNCTION      DS_MPPM_GET_PDN_POLICY_DB_ENTRY_FOR_THE_APN

DESCRIPTION   

DEPENDENCIES  None.

RETURN VALUE  

SIDE EFFECTS  None.
===========================================================================*/
mppm_pdn_policy_database_entry_s_type* ds_mppm_get_pdn_policy_db_entry_for_the_apn
(
  ps_sys_subscription_enum_type     dsd_subs_id,
  ds_mppm_apn_name_type            *apn_name_ptr
);

/*===========================================================================
FUNCTION      DS_MPPM_PDN_DB_WRITE_TO_EFS_FILE

DESCRIPTION   Writes the Policies from PDB DB Queue to EFS Policy DB file

PARAMETERS    pdn_policy_db_q_ptr - Policy DB Queue

DEPENDENCIES  None

RETURN VALUE  TRUE - If PDB DB Queue is written successfully to EFS
              FALSE - EFS writing failed.

SIDE EFFECTS
===========================================================================*/
boolean ds_mppm_write_to_efs_file
(
  q_type* pdn_policy_db_q_ptr,
  ps_sys_subscription_enum_type subscription_id
);

/*===========================================================================
FUNCTION      DS_MPPM_IS_IMS_PDN_UP

DESCRIPTION   

PARAMETERS    

DEPENDENCIES  None.

RETURN VALUE  

SIDE EFFECTS  None.
===========================================================================*/
boolean ds_mppm_is_ims_pdn_up
(
  ps_sys_subscription_enum_type            dsd_subs_id
);

#endif /*FEATURE_DATA_WLAN_MAPCON*/
#endif /* DS_MPPM_EFS_PARSER_H*/

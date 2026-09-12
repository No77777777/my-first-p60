#ifndef DS_EPC_NV_MGR_H
#define DS_EPC_NV_MGR_H
/*===========================================================================

                      DS_EPC_PDN_CTL.H

DESCRIPTION


EXTERNALIZED FUNCTIONS

 Copyright (c) 2011-2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/epchandoff/inc/ds_epc_nv_mgr.h#1 $
  $Author: pwbldsvc $ 

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/07/11    vpk    Add 5 PDN support
09/01/11    fjia    Created Module
===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_EPC_HANDOFF
#include "comdef.h"
#include "customer.h"
#include "ps_iface.h"
#include "ps_iface_ioctl.h"
#include "ps_acl.h"
#include "sys.h"
#include "rex.h"
#include "stm2.h"
#include "ds_epc_ds_cmd_hdlr.h"
#include "ds3geventmgr.h"

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/
#define EPC_EFS_FILE_NAME                  "/epc/srat_cleanup_timer.txt"
#define EPC_EMC_EFS_FILE_NAME              "/epc/srat_emc_cleanup_timer.txt"
#define EPC_EFS_WWAN_RETRY_FILE_NAME       "/epc/wwan_ho_retry_backoff_timer.txt"
#define DS_EPC_SRAT_CLEANUP_DURATION       30000
#define DS_EPC_EMC_SRAT_CLEANUP_DURATION   500

#define DS_EPC_IWLAN_HO_RETRY_COUNT        0
#define DS_EPC_IWLAN_EMC_HO_RETRY_COUNT    3
#define DS_EPC_WWAN_HO_RETRY_COUNT         1

#define DS_EPC_WWAN_HO_MAX_RETRY_COUNT     20
#ifdef FEATURE_DATA_WLAN_MAPCON
#define DS_EPC_TRAT_GIVEUP_DURATION        500
#endif /*FEATURE_DATA_WLAN_MAPCON*/

/* off-target testing use 100 ms */
#if defined(T_WINNT) && defined(TEST_FRAMEWORK)
#error code not present
#else  /* WINNT && TEST_FRAMEWORK*/
#define DS_EPC_RETRY_BACKOFF_DURATION      500
#endif

/*enum definition */
#define DS_EPC_THROTTLE_MAX_TIMER_CNT 6
#define DS_EPC_THROTTLE_MAX_EMC_TIMER_CNT 5

/* Default value 1, restricting the max value to 255*/
extern uint8               
  ds_epc_iwlan_ho_retry_count[DS3GSUBSMGR_SUBS_ID_MAX];
extern uint8 
  ds_epc_iwlan_emc_ho_retry_count[DS3GSUBSMGR_SUBS_ID_MAX];

extern rex_timer_cnt_type   
  ds_epc_iwlan_ho_retry_timer_val[DS3GSUBSMGR_SUBS_ID_MAX];

extern rex_timer_cnt_type   
  ds_epc_iwlan_emc_ho_retry_timer_val[DS3GSUBSMGR_SUBS_ID_MAX];

extern uint8                
  ds_epc_wwan_ho_retry_count[DS3GSUBSMGR_SUBS_ID_MAX];

extern rex_timer_cnt_type   
  ds_epc_wwan_ho_retry_backoff_timer[DS3GSUBSMGR_SUBS_ID_MAX][DS_EPC_WWAN_HO_MAX_RETRY_COUNT];

extern boolean 
  ho_thrtl_timer_NV_configured[DS3GSUBSMGR_SUBS_ID_MAX] ;

/*---------------------------------------------------------------------------
  PDN Throttle Timer suite for LTE/EHRPD. All values in Minutes.
  Values come from Suli's Sepc NUMBER WLAN-WWAN mobility r1.16.3
---------------------------------------------------------------------------*/
extern uint8 
  ds_epc_ho_thrtl_mgr_throttle_timer_vals[DS3GSUBSMGR_SUBS_ID_MAX][DS_EPC_THROTTLE_MAX_TIMER_CNT];

extern boolean 
  ho_thrtl_emc_timer_NV_configured[DS3GSUBSMGR_SUBS_ID_MAX];

extern uint8  
  ds_epc_ho_thrtl_mgr_throttle_emc_timer_vals[DS3GSUBSMGR_SUBS_ID_MAX][DS_EPC_THROTTLE_MAX_EMC_TIMER_CNT];


#ifdef FEATURE_DATA_WLAN_MAPCON
/*---------------------------------------------------------------------------
  Structure used to read the EMC Handoff retry EFS item from the item file in EFS
--------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{ 
  uint8 data_epc_ho_retry_value;
} epc_ho_retry_nv_from_efs;

/*---------------------------------------------------------------------------
  Structure used to read the EMC Handoff retry EFS item from the item file in EFS
--------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{ 
  uint8 data_epc_emc_ho_retry_value;
} epc_emc_ho_retry_nv_from_efs;

#endif /*FEATURE_DATA_WLAN_MAPCON*/


/*---------------------------------------------------------------------------
 This enum indicates the bit in the EPC Handoff local abort timer bitmask 
 EFS item.
 If the bit value indicates DS_EPC_SRAT_IWLAN_BIT, Local Abort timer will apply
 in iWLAN-3GPP direction.
 If the bit value indicates DS_EPC_SRAT_LTE_BIT, Local ABort timer will apply 
 in LTE->iWLAN direction.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_EPC_SRAT_IWLAN_BIT  = 0x00000001,
  DS_EPC_SRAT_NR5G_BIT   = 0x00000002,
  DS_EPC_SRAT_LTE_BIT    = 0x00000004,
  DS_EPC_SRAT_WCDMA_BIT  = 0x00000008,
  DS_EPC_SRAT_GSM_BIT    = 0x00000010,
  DS_EPC_SRAT_TDS_BIT    = 0x00000020,
} ds_epc_srat_bit_type;

/*---------------------------------------------------------------------------
 Default value of SRAT Local ABort timer in mill-seconds.
---------------------------------------------------------------------------*/
#define DS_EPC_LOCAL_ABORT_TIMER_VAL_DEFAULT 1000

/*---------------------------------------------------------------------------
  SRAT Local Abort timer value structure read from EFS. 
  The NV contains 2 fields:
  - Bitmask indicating which handover direction does the SRAT local abort
    timer apply to. 
  - Timer value in milli-seconds which indicates how long will EPC 
    wait before forcing tear down of the SRAT PDN.
---------------------------------------------------------------------------*/
typedef struct
{
  uint32 handover_type_bitmask;
  uint32 timer_val;
} ds_epc_local_abort_timer_efs_type;

extern ds_epc_local_abort_timer_efs_type 
         ds_epc_local_abort_timer_efs_val[DS3GSUBSMGR_SUBS_ID_MAX];

/*===========================================================================
FUNCTION DS_EPC_READ_EPC_IWLAN_HANDOFF_RETRY_COUNT

DESCRIPTION
  This function reads the NV item epc_handoff_retry_count

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_epc_read_epc_iwlan_handoff_retry_count
(
  ds3gsubsmgr_subs_id_e_type  subs_indx
);
/*===========================================================================
FUNCTION DS_EPC_READ_EPC_IWLAN_EMC_HANDOFF_RETRY_COUNT

DESCRIPTION
  This function reads the NV item epc_handoff_retry_count

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_epc_read_epc_iwlan_emc_handoff_retry_count
(
  ds3gsubsmgr_subs_id_e_type  subs_indx
);
/*===========================================================================

FUNCTION       DS_EPC_READ_SRAT_TIMER_VAL

DESCRIPTION    Read srat timer value from EFS.

DEPENDENCIES  None

RETURN VALUE  boolean - TRUE successful read
                        FALSE error in reading

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_read_srat_timer_val 
( 
  ds3gsubsmgr_subs_id_e_type  subs_indx
);
/*===========================================================================
FUNCTION       DS_EPC_READ_EMC_SRAT_TIMER_VAL

DESCRIPTION    Read emc srat timer value from EFS.

DEPENDENCIES  None

RETURN VALUE  boolean - TRUE successful read
                        FALSE error in reading

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_read_emc_srat_timer_val 
(
  ds3gsubsmgr_subs_id_e_type  subs_indx
);
/*===========================================================================
FUNCTION      DS_EPC_READ_HO_THROTTLE_MGR_TIMER_VAL

DESCRIPTION   Read throttle timer value from NV item. Please note that the 
              timer value read from NV is in Minutes. 

DEPENDENCIES  None. 
 
PARAMETERS    None

RETURN VALUE  NV read success 
              NV read failed, use default value  

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_read_ho_throttle_mgr_timer_val
(  
  ds3gsubsmgr_subs_id_e_type  subs_indx
);
/*===========================================================================
FUNCTION      ds_epc_read_ho_throttle_mgr_emc_timer_val

DESCRIPTION   Read emc throttle timer value from NV item. Please note that the 
              timer value read from NV is in Minutes. 

DEPENDENCIES  None. 
 
PARAMETERS    None

RETURN VALUE  NV read success 
              NV read failed, use default value  

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_read_ho_throttle_mgr_emc_timer_val
(  
  ds3gsubsmgr_subs_id_e_type  subs_indx
);

/*===========================================================================
FUNCTION       DS_EPC_READ_WWAN_RETRY_TIMER_EFS

DESCRIPTION    Read the wwan retry timer EFS and get the retry count and 
               timer values 

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_epc_read_wwan_retry_timer
(
  ds3gsubsmgr_subs_id_e_type  subs_indx
);

/*===========================================================================
FUNCTION  ds_epc_nv_refresh_cmd_proc

DESCRIPTION
 This functions is invoked a part of nv refresh command per subscription

PARAMETERS
  cmd_ptr

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_epc_nv_refresh_cmd_proc
(
  const ds_cmd_type      *cmd_ptr
);
/*===========================================================================
FUNCTION  ds_epc_nv_refresh_cmd_proc

DESCRIPTION
 This functions is invoked a part of nv refresh command per subscription

PARAMETERS
  cmd_ptr

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_epc_nv_refresh_cb
(
  ds3geventmgr_event_type        event_id,
  ds3geventmgr_filter_type      *filter_info_ptr
);
/*===========================================================================
FUNCTION DS_EPC_UPDATE_DATA_CONFIG_INFO
  
DESCRIPTION
  Puts all the required nv item file paths in the data_config_info.conf 
  file. Would be called from dsutils during powerup.
    
PARAMETERS  
  file_paths_buf: To hold the efs item file paths
      
DEPENDENCIES 
  None.
    
RETURN VALUE 
  0 for success.
  -1 for failure.
     
SIDE EFFECTS 
  None.
===========================================================================*/
int32 ds_epc_update_data_config_info
(
  char  *file_paths_buf
);
/*===========================================================================
FUNCTION      DS_EPC_EFS_ITEM_FILE_PATH_SIZE

DESCRIPTION   Returns the total EPC EFS item file paths size

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
uint32 ds_epc_efs_item_file_path_size
(
  void
);

/*===========================================================================
FUNCTION       DS_EPC_READ_NV

DESCRIPTION    Read epc relative nv items and efs 

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_epc_read_nv(void);

/*===========================================================================
FUNCTION      ds_epc_print_nv_values

DESCRIPTION   Returns the total EPC EFS item file paths size

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_epc_print_nv_values(void);

#endif /*FEATURE_EPC_HANDOFF*/
#endif /*DS_EPC_NV_MGR_H*/

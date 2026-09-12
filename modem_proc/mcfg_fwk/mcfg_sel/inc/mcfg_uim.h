#ifndef MCFG_UIM_H
#define MCFG_UIM_H
/*==============================================================================

                M O D E M   C O N F I G   U I M

GENERAL DESCRIPTION
  Module supporting accessing the UIM for the purposes of automatically
  selecting an associated configuration. This module is confined to reading the
  identifying parameters from the UIM - the actual configuration selection is
  done in mcfg_sel.

Copyright (c) 2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_sel/inc/mcfg_uim.h#1 $
$DateTime: 2021/04/08 00:04:19 $
$Author: pwbldsvc $
$Change: 30003610 $

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
09/07/12 bd   Created module

==============================================================================*/

#include "mcfg_task.h"
#include "mmgsdilib.h"
#include "mmgsdisessionlib.h"
#include "mcfg_common.h"
#include "persistent_device_configuration_v01.h"
#include "mcfg_trl.h"

/*==============================================================================

                PUBLIC DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/
/*==============================================================================
  Constants and Macros
==============================================================================*/

/* Length of EF-ICCID in bytes, per ETSI TS 102 221 (when encoded in BCD) */
#define MCFG_UIM_ICCID_LEN (10)

/* Minimum size, in bytes, of a buffer to fit the ICCID encoded as an ASCII
   string (includes space for null termination) */
#define MCFG_UIM_ICCID_STR_BUF_LEN (MCFG_UIM_ICCID_LEN * 2 + 1)

/* Minimum length of a valid ICCID string, in characters */
#define MCFG_UIM_ICCID_STR_MIN_LEN (19)

/* Maximum length of a valid IMPI string, in characters */
#define MCFG_UIM_IMPI_MAX_LEN (255)

/* Maximum length of a valid GID in bytes as per Spec 131.102 */
#define MCFG_UIM_GID_MAX_LEN (127)

/* Minimum size, in bytes, of a buffer to fit the GID encoded as an ASCII
   string (includes space for null termination) */
#define MCFG_UIM_GID_STR_BUF_LEN (MCFG_UIM_GID_MAX_LEN * 2 + 1)

/* Maximum length of a valid SPN in bytes as per Spec 131.102 */
#define MCFG_UIM_SPN_MAX_LEN (16)

/* Buffer size to fit the SPN string (includes space for null termination) */
#define MCFG_UIM_SPN_STR_BUF_LEN (MCFG_UIM_SPN_MAX_LEN + 1)

/* Card info EFS version */
#define MCFG_UIM_CARD_INFO_VERSION (1)

/* Default setting for whether the UIM-based autoselect feature should be 
   enabled when the NV item is not present */
#define MCFG_TARGET_FEATURE_SETTING 7

#ifdef MCFG_TARGET_FEATURE_SETTING
  #define MCFG_UIM_AUTOSELECT_DEFAULT (MCFG_TARGET_FEATURE_SETTING)
#else
  #define MCFG_UIM_AUTOSELECT_DEFAULT (0x0)
#endif

#define MCFG_UIM_NV_AUTOSELECT_MASK      (0x01)
#define MCFG_UIM_NV_FLEXMAP_MASK         (0x02)
#define MCFG_UIM_NV_IMSISELECT_MASK      (0x04)
#define MCFG_UIM_NV_UPDATEONLY_MASK      (0x40)
#define MCFG_UIM_NV_REFRESH_MASK         (0x80)
#define MCFG_UIM_NV_UNINIT               (0xDE)
#define MCFG_UIM_NV_HYBRIDSELECT_MASK    (MCFG_UIM_NV_AUTOSELECT_MASK|MCFG_UIM_NV_IMSISELECT_MASK)

/*==============================================================================
  Typedefs
==============================================================================*/
typedef enum mcfg_uim_ext_session_e {
  MCFG_UIM_EXT_SESSION_GW = 0,
  MCFG_UIM_EXT_SESSION_1X,
  MCFG_UIM_NUM_OF_EXT_SESSION
} mcfg_uim_ext_session_e_type;

typedef enum mcfg_uim_card_state_e {
  MCFG_UIM_CARD_STATE_NOT_INIT = 0,
  MCFG_UIM_CARD_STATE_NO_CARD  = 1,
  MCFG_UIM_CARD_STATE_INSERTED = 2
} mcfg_uim_card_state_e_type;

typedef enum mcfg_uim_cmd_e {
  MCFG_UIM_CMD_MIN = 0,

  MCFG_UIM_CMD_INIT       = 0,
  MCFG_UIM_CMD_MMGSDI_RSP = 1,
  MCFG_UIM_CMD_MMGSDI_EVT = 2,
  MCFG_UIM_CMD_MAPPING_TIMEOUT = 3,
  MCFG_UIM_CMD_RESELECT = 4,
  MCFG_UIM_CMD_MAX
} mcfg_uim_cmd_e_type;

typedef enum mcfg_gid_index_e {
  MCFG_GID_INDEX_1 = 0,
  MCFG_GID_INDEX_2,
  MCFG_GID_INDEX_MAX
} mcfg_gid_index_e_type;

typedef union mcfg_uim_cmd_payload_s {
  struct {
    mmgsdi_return_enum_type status;
    mmgsdi_cnf_enum_type    cnf;
    mmgsdi_cnf_type         cnf_data;
  } rsp;

  mmgsdi_event_data_type evt;

  mcfg_slot_index_type_e_type sloti;
} mcfg_uim_cmd_payload_s_type;

typedef struct mcfg_uim_cmd_data_s {
  mcfg_uim_cmd_e_type cmd_id;

  mcfg_uim_cmd_payload_s_type data;
} mcfg_uim_cmd_data_s_type;

typedef struct mcfg_uim_card_info_s
{
  //for future extension
  uint8 version;

  //ICCID (optional)
  char ICCID[MCFG_UIM_ICCID_STR_BUF_LEN];

  //IMSI
  mcfg_trl_carrier_mcc_mnc_s_type plmn[MCFG_UIM_NUM_OF_EXT_SESSION][2];

  // selected carrie_index
  pdc_selection_carrier_enum_v01 carrier_index;

  //feature tag
  uint16_t country;
  uint16_t ims_feature;

  //IMPI (optional)
  char IMPI[MCFG_UIM_IMPI_MAX_LEN];

  //GID (optional)
  char group_id1[MCFG_UIM_GID_STR_BUF_LEN];
  char group_id2[MCFG_UIM_GID_STR_BUF_LEN];

  //matching config ID 
  mcfg_config_id_s_type config_id;

  // Customer specific parameter value to use for MBN seleciton
  uint32 customid;

  char spn[MCFG_UIM_SPN_STR_BUF_LEN];

} mcfg_uim_card_info_s_type;

/*==============================================================================
  Variables
==============================================================================*/


/*==============================================================================

                    PUBLIC FUNCTION DECLARATIONS FOR MODULE

==============================================================================*/
/*===========================================================================

  FUNCTION mcfg_uim_nv_autoselect_enabled

  DESCRIPTION
    Checks whether UIM-based automatic MCFG_SW selection is enabled.

  DEPENDENCIES
    None

  PARAMETERS
    sub_id

  RETURN VALUE
    TRUE if enabled for specified slot index, FALSE otherwise
    TRUE if enabled for any slot if slot index is provided invalid

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_nv_autoselect_enabled
(
  mcfg_slot_index_type_e_type slot_index
);

/*===========================================================================

  FUNCTION  mcfg_uim_nv_refresh_enabled

  DESCRIPTION
    This function is used to get refresh setting

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_nv_refresh_enabled
(
  mcfg_slot_index_type_e_type slot_index
);

/*===========================================================================

  FUNCTION  mcfg_uim_nv_updateonly_enabled

  DESCRIPTION
    This function is used to get updateonly setting

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/

boolean mcfg_uim_nv_updateonly_enabled
(
  mcfg_slot_index_type_e_type slot_index
);

/*===========================================================================

  FUNCTION mcfg_uim_nv_imsiselect_enabled

  DESCRIPTION
    Checks whether UIM-based IMSI MCFG_SW selection is enabled.

  DEPENDENCIES
    None

  PARAMETERS
    sub_id

  RETURN VALUE
    TRUE if enabled for specified slot index, FALSE otherwise
    TRUE if enabled for any slot if slot index is provided invalid

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_nv_imsiselect_enabled
(
  mcfg_slot_index_type_e_type slot_index
);

boolean mcfg_uim_nv_flexmap_enabled
(
  mcfg_slot_index_type_e_type slot_index
);

boolean mcfg_uim_nv71546_update
(
  mcfg_slot_index_type_e_type sloti,
  uint8   value,
  uint8   mask
);

/*===========================================================================

  FUNCTION  mcfg_uim_nv71546_write

  DESCRIPTION
    This function is used to write mcfg autoenabled setting into EFS

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
uint8 mcfg_uim_nv71546_write
(
  mcfg_slot_index_type_e_type sloti
);

/*===========================================================================

  FUNCTION mcfg_uim_cmd_send

  DESCRIPTION
    This function sends mcfg uim commands for processing. 

  DEPENDENCIES
    None

  PARAMETERS
    uim_cmd    [in]  uim commands

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_uim_cmd_send
(
  mcfg_uim_cmd_e_type uim_cmd, 
  mcfg_uim_cmd_payload_s_type *data
);

/*===========================================================================

  FUNCTION mcfg_uim_cmd_proc

  DESCRIPTION
    Processes a mcfg_uim command in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_uim_cmd_proc
(
  mcfg_task_cmd_s_type *cmd
);

/*===========================================================================

  FUNCTION mcfg_uim_mmgsdi_init

  DESCRIPTION
    Starts the MCFG_UIM MMGSDI initialization procedure by requesting a
    client ID and registering for card events.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_uim_mmgsdi_init
(
  void
);

/*===========================================================================

  FUNCTION mcfg_uim_init

  DESCRIPTION
    Starts the MCFG_UIM initialization .

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_uim_init
(
  void
);

/*===========================================================================

  FUNCTION mcfg_uim_decode_iccid

  DESCRIPTION
    Decodes the ICCID from its SIM encoding (binary coded decimal) into an
    ASCII string.

  DEPENDENCIES
    None

  PARAMETERS
    iccid_bcd    [in] input ICCID in EF-ICCID format (BCD)
    iccid_ascii [out] array to store decoded ICCID; buffer must be at least
                      MCFG_UIM_ICCID_STR_BUF_LEN bytes long

  RETURN VALUE
    TRUE if ICCID successfully decoded, FALSE otherwise (e.g. bad ICCID)

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_decode_iccid
(
  const uint8 *iccid_bcd,
  char        *iccid_ascii
);

/*===========================================================================

  FUNCTION mcfg_uim_map_sub_id_to_slot_index

  DESCRIPTION

  DEPENDENCIES
    None

  PARAMETERS
    sub index

  RETURN VALUE
    slot index

  SIDE EFFECTS
    None

===========================================================================*/
mcfg_slot_index_type_e_type mcfg_uim_map_sub_id_to_slot_index
(
  mcfg_sub_id_type_e_type sub_id
);

/*===========================================================================

  FUNCTION mcfg_uim_map_sub_id_to_slot_index

  DESCRIPTION

  DEPENDENCIES
    None

  PARAMETERS
    slot id

  RETURN VALUE
    slot index

  SIDE EFFECTS
    None

===========================================================================*/
mcfg_sub_id_type_e_type mcfg_uim_map_slot_index_to_sub_id
(
  mcfg_slot_index_type_e_type slot_index
);

/*===========================================================================

  FUNCTION mcfg_uim_get_mmgsdi_client_id

  DESCRIPTION
    This function to get the registered client id with MMGSDI.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
mmgsdi_client_id_type mcfg_uim_get_mmgsdi_client_id
(
  void
);

/*===========================================================================

  FUNCTION mcfg_uim_process_mapping

  DESCRIPTION
    Process UIM mapping table for flex mapping

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_process_mapping
(
  mcfg_slot_index_type_e_type slot_index,
  mcfg_sub_id_type_e_type     sub_id,
  boolean                     refresh
);

boolean mcfg_uim_update_card_info
(
  mcfg_slot_index_type_e_type sloti,
  mcfg_uim_card_info_s_type  *card_info
);

#ifdef MCFG_DEBUG
void mcfg_uim_simulate_card_removal
(
   mcfg_sub_id_type_e_type sub
);
#endif 

boolean mcfg_uim_get_card_info
(
  mcfg_slot_index_type_e_type sloti,
  const char                 *iccid,
  mcfg_uim_card_info_s_type  *card_info
);

boolean mcfg_uim_update_iccid
(
  mcfg_slot_index_type_e_type sloti,
  const char *iccid
);

boolean mcfg_uim_mmgsdi_read_ef
(
  mmgsdi_session_id_type session_id,
  mmgsdi_file_enum_type file_name,
  mmgsdi_data_type* read_data_p
);

boolean mcfg_uim_read_gid
(
  mcfg_slot_index_type_e_type slot_index,
  mcfg_uim_ext_session_e_type session_index,
  mcfg_gid_index_e_type gid_index,
  mmgsdi_data_type* gid_p
);

boolean mcfg_uim_decode_gid
(
  const uint8 *gid_bcd,
  uint32       data_len,
  char        *gid_ascii
);

boolean mcfg_uim_read_and_update_uimdata
(
  mcfg_slot_index_type_e_type sloti,
  mcfg_uim_ext_session_e_type session,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn
);

boolean mcfg_uim_update_plmn
(
  mcfg_slot_index_type_e_type      sloti,
  mcfg_uim_ext_session_e_type      session,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn
);

mcfg_uim_card_info_s_type *mcfg_uim_get_card_info_p
(
  mcfg_slot_index_type_e_type sloti
);

boolean mcfg_uim_remove_all_card_infos
(
  void
);

#endif /* MCFG_UIM_H */

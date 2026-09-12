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

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_sel/src/mcfg_uim.c#3 $
$DateTime: 2024/06/18 18:05:58 $
$Author: pwbldsvc $
$Change: 54528259 $

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
09/07/12 bd   Created module

==============================================================================*/

#include "comdef.h"

#include "mmgsdilib.h"
#include "mmgsdisessionlib.h"
#include "mmgsdisessionlib_v.h"

#include "mcfg_int.h"
#include "mcfg_nv.h"
#include "mcfg_fs.h"
#include "mcfg_osal.h"
#include "mcfg_sel.h"
#include "mcfg_uim.h"
#include "mcfg_utils.h"
#include "mcfg_proc.h"
#include "mcfg_map.h"
#include "mcfg_setting.h"

/*==============================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/
/*==============================================================================
  Constants and Macros
==============================================================================*/

#define MNC_LEN_BYTE  (4)

/*==============================================================================
  Typedefs
==============================================================================*/

#define MCFG_MMGSDI_NUM_REGISTRED_FILES 2
#define MCFG_NUM_OF_SESSION_TYPE (MMGSDI_CARD_SESSION_SLOT_3+1)
#define MCFG_NUM_OF_EXT_SESSION  2

#define MCFG_CARD_INFO_DIR           "/nv/item_files/mcfg/cards"
#define MCFG_CARD_INFO_FNAME_MAX     60

#define MCFG_NUM_OF_CARDS_INFO_MIN            10
#define MCFG_NUM_OF_CARDS_INFO_DEFAULT        25
#define MCFG_CARDS_INFO_LIMIT_EFS_PATH        "/nv/item_files/mcfg/cards_info_limit"
#define MCFG_NUM_OF_CARDS_INFOS_TO_DELETE      5
typedef struct
{
  //mmgsdi_session_type_enum_type               session_type;  
  mcfg_sub_id_type_e_type                       sub_id;        
  mmgsdi_session_id_type                        session_id;    
  mmgsdi_app_enum_type                          app_type;      
  size_t                                        num_mnc_digits;
  uint8                                         imsi[MMGSDI_IMSI_LEN];
} mcfg_mmgsdi_ctx_t;

/*==============================================================================
  Variables
==============================================================================*/

static struct {
  mmgsdi_client_id_type  client_id;

  struct {
    /* session id (for slot session)*/
    mmgsdi_session_id_type        session_id;

    /* Set to TRUE once client ID & session ID successfully allocated */
    boolean                       ready;

    mcfg_uim_card_state_e_type    card_state;

    mcfg_sub_id_type_e_type       sub_id;

    uint8                          autoselect_nv;
  }slot[MCFG_NUM_OF_UIM_SLOT];

  struct {
    /* ext session id (for subscriptions)*/
    mmgsdi_session_id_type        ext_session_id[MCFG_UIM_NUM_OF_EXT_SESSION];
    mmgsdi_app_enum_type          ext_app_type[MCFG_UIM_NUM_OF_EXT_SESSION];
  }sub[MCFG_NUM_OF_SUB_ID];

  mcfg_uim_card_info_s_type  card_info[MCFG_NUM_OF_UIM_SLOT];

} mcfg_uim_state;

static uint16 mcfg_uim_current_num_of_cards_info = 0;

static uint16 mcfg_uim_max_num_of_cards_info;
static timer_type       mcfg_uim_flexmap_timer;  
static timer_group_type mcfg_uim_flexmap_timer_group;
#define MCFG_UIM_FLEXMAP_TIMER_TIMEOUT (2) 

/*==============================================================================

                 PRIVATE FUNCTION FORWARD DECLARATIONS

==============================================================================*/

static uint8 mcfg_uim_map_session_id_to_slot_index
(
  mmgsdi_session_id_type session_id
);

mcfg_slot_index_type_e_type mcfg_uim_map_sub_id_to_slot_index
(
  mcfg_sub_id_type_e_type sub_id
);

static mcfg_sub_id_type_e_type mcfg_uim_map_session_id_to_sub_id
(
  mmgsdi_session_id_type session_id
);

static mcfg_slot_index_type_e_type mcfg_uim_map_mmgsdi_slot_to_slot_index
(
  mmgsdi_slot_id_enum_type slot_id
);

static void mcfg_uim_mmgsdi_client_id_and_evt_reg_cnf_proc
(
  mmgsdi_return_enum_type                      status,
  const mmgsdi_client_id_and_evt_reg_cnf_type *cnf_data
);

static void mcfg_uim_mmgsdi_evt_cb
(
  const mmgsdi_event_data_type *event
);

static void mcfg_uim_mmgsdi_evt_proc
(
  const mmgsdi_event_data_type *event
);

static void mcfg_uim_mmgsdi_read_cnf_proc
(
  mmgsdi_return_enum_type     status,
  const mmgsdi_read_cnf_type *cnf_data
);

static void mcfg_uim_read_iccid_if_ready
(
  uint8 slot_index
);

static void mcfg_uim_mmgsdi_rsp_cb
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type  *cnf_ptr
);

static void mcfg_uim_mmgsdi_rsp_proc
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type  *cnf_ptr
);

static void mcfg_uim_mmgsdi_session_open_ext_cnf_proc
(
  mmgsdi_return_enum_type                 status,
  const mmgsdi_session_open_ext_cnf_type *cnf_data
);

#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
static void mcfg_uim_read_imsi_if_ready
(
  uint8 slot_index,
  mmgsdi_app_enum_type app_type
);
#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */

/*==============================================================================

                     INTERNAL FUNCTION DECLARATIONS

==============================================================================*/
/*===========================================================================

  FUNCTION  mcfg_uim_read_autoselect_nv

  DESCRIPTION
    This function is used to get mcfg refresh autoenabled setting

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
uint8 mcfg_uim_nv_read
(
  mcfg_slot_index_type_e_type slot_index
)
{
  uint8 autoselect_enabled;
  mcfg_nv_status_e_type status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(slot_index <= MCFG_REFRESH_INDEX_MAX);

  status = mcfg_nv_read(MCFG_NV_ITEM_AUTOSELECT_BY_UIM, 0, slot_index,
                        &autoselect_enabled, sizeof(autoselect_enabled));
  if (status != MCFG_NV_STATUS_OK)
  {
    autoselect_enabled = MCFG_UIM_AUTOSELECT_DEFAULT;
    if (status != MCFG_NV_STATUS_NOT_ACTIVE)
    {
      MCFG_MSG_MED_2("Couldn't read autoselect feature NV item: status %d slot %d",
                       status, slot_index);
    }
  }
  else if (((autoselect_enabled & MCFG_UIM_NV_HYBRIDSELECT_MASK) > 0) &&
    ((autoselect_enabled & MCFG_UIM_NV_HYBRIDSELECT_MASK) != MCFG_UIM_NV_HYBRIDSELECT_MASK))
  {
    autoselect_enabled = autoselect_enabled | MCFG_UIM_NV_HYBRIDSELECT_MASK;
    MCFG_MSG_MED_1("selection enabled, force iccid and imsi selection: %x", autoselect_enabled);

    status = mcfg_nv_write(MCFG_NV_ITEM_AUTOSELECT_BY_UIM, 0, slot_index,
                           &autoselect_enabled, sizeof(autoselect_enabled));
    if (status != MCFG_NV_STATUS_OK)
    {
      MCFG_MSG_ERROR_2("Couldn't write autoselect feature NV item: status %d slot %d",
                       status, slot_index);
    }
  }

  MCFG_MSG_LOW_2("read_autoselect_nv sloti%d NV=%x", slot_index, autoselect_enabled);

  return autoselect_enabled;
}

/*===========================================================================

  FUNCTION mcfg_uim_nv_autoselect_enabled

  DESCRIPTION
    Checks whether UIM-based automatic MCFG_SW selection is enabled.

  DEPENDENCIES
    None

  PARAMETERS
    sub_id  subscription ID

  RETURN VALUE
    TRUE if enabled, FALSE otherwise;
    TURE if enabled for any subscription with MCFG_NUM_OF_SUB_ID as input.

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_nv_autoselect_enabled
(
  mcfg_slot_index_type_e_type slot_index
)
{
  uint8 autoselect_enabled;
  mcfg_nv_status_e_type status;
  mcfg_slot_index_type_e_type start, end, i;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(slot_index <= MCFG_NUM_OF_UIM_SLOT && slot_index >=0);

  if (slot_index < MCFG_NUM_OF_UIM_SLOT)
  {
    start = slot_index;
    end = (slot_index+1);
    MCFG_MSG_MED_1("mcfg_uim_nv_autoselect_enabled for sloti %d", slot_index);
  }
  else
  {
    start = MCFG_SLOT_INDEX_FIRST;
    end = MCFG_NUM_OF_UIM_SLOT;
    MCFG_MSG_MED("mcfg_uim_nv_autoselect_enabled for any slot");
  }

  for (i = start; i < end; i++)
  {
    MCFG_MSG_HIGH_2("autoselect_nv 0x%02x for slot %d", mcfg_uim_state.slot[i].autoselect_nv, i);
    if(mcfg_uim_state.slot[i].autoselect_nv == MCFG_UIM_NV_UNINIT)
    {
      autoselect_enabled = mcfg_uim_nv_read(i);
      mcfg_uim_state.slot[i].autoselect_nv = autoselect_enabled;
    }
    else
    {
      autoselect_enabled = mcfg_uim_state.slot[i].autoselect_nv;
    }

    if ((autoselect_enabled & MCFG_UIM_NV_AUTOSELECT_MASK) > 0)
    {
      break;
    }
  }

  MCFG_MSG_MED_1("mcfg_uim_nv_autoselect_enabled NV=%x", autoselect_enabled);
  return ((autoselect_enabled & MCFG_UIM_NV_AUTOSELECT_MASK) > 0);
} /* mcfg_uim_nv_autoselect_enabled() */

/*===========================================================================

  FUNCTION mcfg_uim_nv_flexmap_enabled

  DESCRIPTION
    Checks whether UIM-based flexmapping selection is enabled.

  DEPENDENCIES
    None

  PARAMETERS
    sub_id  subscription ID

  RETURN VALUE
    TRUE if enabled, FALSE otherwise;

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_nv_flexmap_enabled
(
  mcfg_slot_index_type_e_type slot_index
)
{
  uint8 nv_value;
  boolean flexmap_enabled = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(slot_index < MCFG_NUM_OF_UIM_SLOT);

  if(mcfg_uim_state.slot[slot_index].autoselect_nv == MCFG_UIM_NV_UNINIT)
  {
    nv_value = mcfg_uim_nv_read(slot_index);
    mcfg_uim_state.slot[slot_index].autoselect_nv = nv_value;
  }
  else
  {
    nv_value = mcfg_uim_state.slot[slot_index].autoselect_nv;
  }

  if((nv_value != MCFG_UIM_NV_UNINIT) && (nv_value & MCFG_UIM_NV_FLEXMAP_MASK))
  {
    if(nv_value & (MCFG_UIM_NV_AUTOSELECT_MASK | MCFG_UIM_NV_IMSISELECT_MASK))
    {
      flexmap_enabled = TRUE;
    }
    else
    {
      mcfg_uim_state.slot[slot_index].autoselect_nv = nv_value & (~MCFG_UIM_NV_FLEXMAP_MASK);
      MCFG_MSG_MED_2("Disable Flex mapping with auto selection disabled nv from %x to %x", 
                     nv_value, mcfg_uim_state.slot[slot_index].autoselect_nv);
      mcfg_uim_nv71546_write(slot_index);

      return FALSE;
  }
  }

  MCFG_MSG_MED_2("nv_flexmap_enabled %d NV=%x", flexmap_enabled, nv_value);
  return flexmap_enabled;
} /* mcfg_uim_nv_flexmap_enabled() */

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
)
{
  boolean ret = FALSE;
#ifdef FEATURE_MCFG_REFRESH
  uint8 nv_value;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(slot_index <= MCFG_REFRESH_INDEX_MAX);

  if(mcfg_uim_state.slot[slot_index].autoselect_nv == MCFG_UIM_NV_UNINIT)
  {
    nv_value = mcfg_uim_nv_read(slot_index);
    mcfg_uim_state.slot[slot_index].autoselect_nv = nv_value;
  }
  else
  {
    nv_value = mcfg_uim_state.slot[slot_index].autoselect_nv;
  }

  if ((nv_value & MCFG_UIM_NV_REFRESH_MASK) > 0)
  {
    MCFG_MSG_LOW_1("mcfg_refresh_disabled sloti %d", slot_index);
    ret = FALSE;
  }
  else
  {
    MCFG_MSG_LOW_1("mcfg_refresh_enabled sloti %d", slot_index);
    ret = TRUE;
  }
#endif

  return ret;
}

/*===========================================================================

  FUNCTION mcfg_uim_nv_updateonly_enabled

  DESCRIPTION
    Checks whether updateonly is enabled.

  DEPENDENCIES
    None

  PARAMETERS
    sub_id  subscription ID

  RETURN VALUE
    TRUE if enabled, FALSE otherwise;

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_nv_updateonly_enabled
(
  mcfg_slot_index_type_e_type slot_index
)
{
  uint8 nv_value;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(slot_index < MCFG_NUM_OF_UIM_SLOT);

  if(mcfg_uim_state.slot[slot_index].autoselect_nv == MCFG_UIM_NV_UNINIT)
  {
    nv_value = mcfg_uim_nv_read(slot_index);
    mcfg_uim_state.slot[slot_index].autoselect_nv = nv_value;
  }
  else
  {
    nv_value = mcfg_uim_state.slot[slot_index].autoselect_nv;
  }

  MCFG_MSG_MED_1("updateonly_enabled NV=%x", nv_value);
  return ((nv_value & MCFG_UIM_NV_UPDATEONLY_MASK) > 0);
} /* mcfg_uim_nv_updateonly_enabled() */

/*===========================================================================

  FUNCTION mcfg_uim_nv_imsiselect_enabled

  DESCRIPTION
    Checks whether imsi based auto selection is enabled.

  DEPENDENCIES
    None

  PARAMETERS
    sub_id  subscription ID

  RETURN VALUE
    TRUE if enabled, FALSE otherwise;

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_nv_imsiselect_enabled
(
  mcfg_slot_index_type_e_type slot_index
)
{
  uint8 imsiselect_enabled;
  mcfg_nv_status_e_type status;
  mcfg_slot_index_type_e_type start, end, i;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(slot_index <= MCFG_NUM_OF_UIM_SLOT && slot_index >=0);

  if (slot_index < MCFG_NUM_OF_UIM_SLOT)
  {
    start = slot_index;
    end = (slot_index+1);
    MCFG_MSG_MED_1("mcfg_uim_nv_imsiselect_enabled for sloti %d", slot_index);
  }
  else
  {
    start = MCFG_SLOT_INDEX_FIRST;
    end = MCFG_NUM_OF_UIM_SLOT;
    MCFG_MSG_MED("mcfg_uim_nv_imsiselect_enabled for any slot");
  }

  for (i = start; i < end; i++)
  {
    MCFG_MSG_HIGH_2("imsiselect_nv 0x%02x for slot %d", mcfg_uim_state.slot[i].autoselect_nv, i);
    if(mcfg_uim_state.slot[i].autoselect_nv == MCFG_UIM_NV_UNINIT)
    {
      imsiselect_enabled = mcfg_uim_nv_read(i);
      mcfg_uim_state.slot[i].autoselect_nv = imsiselect_enabled;
    }
    else
    {
      imsiselect_enabled = mcfg_uim_state.slot[i].autoselect_nv;
    }

    if ((imsiselect_enabled & MCFG_UIM_NV_IMSISELECT_MASK) > 0)
    {
      break;
    }
  }

  MCFG_MSG_MED_1("mcfg_uim_nv_imsiselect_enabled NV=%x", imsiselect_enabled);
  return ((imsiselect_enabled & MCFG_UIM_NV_IMSISELECT_MASK) > 0);
} /* mcfg_uim_nv_imsiselect_enabled() */


boolean mcfg_uim_nv71546_update
(
  mcfg_slot_index_type_e_type sloti,
  uint8   value,
  uint8   mask
)
{
  uint8 nv;
  boolean changed = FALSE; 
  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);

  nv = mcfg_uim_state.slot[sloti].autoselect_nv;
  changed = !((nv & mask) == (value & mask));

  nv = nv & (~mask);
  nv = nv | (value & mask);

  mcfg_uim_state.slot[sloti].autoselect_nv = nv;
  return changed;
}

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
)
{
  mcfg_nv_status_e_type status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);

  MCFG_MSG_LOW_2("write autoselect_nv 0x%x for sloti %d", 
                 mcfg_uim_state.slot[sloti].autoselect_nv,
                 sloti);
  if (mcfg_uim_state.slot[sloti].autoselect_nv == MCFG_UIM_NV_UNINIT || 
      mcfg_uim_state.slot[sloti].autoselect_nv == MCFG_UIM_AUTOSELECT_DEFAULT)
  {
    status = mcfg_nv_delete(MCFG_NV_ITEM_AUTOSELECT_BY_UIM, 0, sloti);
  }
  else
  {
    status = mcfg_nv_write(MCFG_NV_ITEM_AUTOSELECT_BY_UIM, 0, sloti,
                           &mcfg_uim_state.slot[sloti].autoselect_nv, 
                           sizeof(mcfg_uim_state.slot[sloti].autoselect_nv));
  }

  if (status != MCFG_NV_STATUS_OK)
  {
    MCFG_MSG_ERROR_2("Couldn't write or delete autoselect feature NV item: status %d sloti %d",
                     status, sloti);
    return FALSE;
  }

  return TRUE;
}

/*===========================================================================

  FUNCTION  mcfg_uim_get_flexmap_timer

  DESCRIPTION

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
uint8 mcfg_uim_get_flexmap_timer
(
  void
)
{
  uint8 value;
  mcfg_nv_status_e_type status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  status = mcfg_nv_read(MCFG_NV_ITEM_UIM_SLOT_MAPPING_TIMER, 0, 
                        MCFG_SLOT_INDEX_FIRST,
                        &value, sizeof(value));
  if (status != MCFG_NV_STATUS_OK)
  {
    value = MCFG_UIM_FLEXMAP_TIMER_TIMEOUT;
    if (status != MCFG_NV_STATUS_NOT_ACTIVE)
    {
      MCFG_MSG_HIGH_1("Couldn't read flexmap timer NV item: status %d", status);
    }
  }

  MCFG_MSG_MED_1("mcfg_uim_get_flexmap_timer NV=%d", value);
  return value;
} /* mcfg_uim_get_flexmap_timer() */

boolean mcfg_uim_update_card_info
(
  mcfg_slot_index_type_e_type sloti,
  mcfg_uim_card_info_s_type  *card_info
)
{
  char path[MCFG_CARD_INFO_FNAME_MAX];
  char last_path[MCFG_CARD_INFO_FNAME_MAX];
  int efs_ret= -1;

  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);

  if(strlen(card_info->ICCID) <= 0)
  {
    memset(mcfg_uim_state.card_info[sloti].ICCID, 0x30, MCFG_UIM_ICCID_STR_BUF_LEN-1);
  }

  mcfg_uim_state.card_info[sloti].version = MCFG_UIM_CARD_INFO_VERSION;
  snprintf(path, MCFG_CARD_INFO_FNAME_MAX, "%s/%s", MCFG_CARD_INFO_DIR, mcfg_uim_state.card_info[sloti].ICCID);

  do
  {
    if(MCFG_FS_STATUS_OK != mcfg_fs_write(path, card_info, sizeof(mcfg_uim_card_info_s_type), O_WRONLY | O_TRUNC | O_CREAT | O_AUTODIR,
                                          ALLPERMS, MCFG_FS_TYPE_EFS, MCFG_FS_SUBID_0))
    {
      MCFG_MSG_SPRINTF_2("mcfg_uim_update_card_info: Error %d writing card info for path: %s", 
             efs_errno, path);
      break;
    }

    if (sloti == MCFG_SLOT_INDEX_FIRST)
    {
      snprintf(last_path, MCFG_CARD_INFO_FNAME_MAX, "%s/last_iccid", MCFG_CARD_INFO_DIR);
    }
    else
    {
      snprintf(last_path, MCFG_CARD_INFO_FNAME_MAX, "%s/last_iccid_Subscription%02d", MCFG_CARD_INFO_DIR, sloti);
    }

    efs_unlink(last_path);
    efs_ret = efs_symlink(path, last_path);
    if (efs_ret != 0)
    {
      MCFG_MSG_HIGH_2("efs_symlink -  efs ret %d efs_errno %d", efs_ret, efs_errno);
    }
  }while(0);

  return (efs_ret == 0);
}

mcfg_uim_card_info_s_type *mcfg_uim_get_card_info_p
(
  mcfg_slot_index_type_e_type sloti
)
{
  MCFG_ASSERT_COND_RET_ZERO(sloti < MCFG_NUM_OF_UIM_SLOT);

  return &mcfg_uim_state.card_info[sloti];
}

/*===========================================================================

  FUNCTION mcfg_uim_get_num_of_cards_infos_from_efs

  DESCRIPTION
   Calculates the number of cards info present inside cards folder in EFS.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    uint16 - Number of cards info present in EFS

  SIDE EFFECTS
    None

===========================================================================*/
static uint16 mcfg_uim_get_num_of_cards_info_from_efs( void )
{

  EFSDIR *dirp = NULL;
  struct fs_dirent *dirent = NULL;
  struct fs_stat f_stat;
  uint32 fmode;
  char path[MCFG_CARD_INFO_FNAME_MAX];
  uint16 num_of_cards_info = 0;

  /* Open the cards directory */
  dirp = efs_opendir (MCFG_CARD_INFO_DIR);
  if(!dirp)
  {
    MCFG_MSG_ERROR_1("mcfg_uim_get_num_of_cards_info_from_efs: unable to open cards directory for error - %d",efs_errno);
    return num_of_cards_info;
  }
  /* Iterate through each item in cards directory */
  while ((dirent = efs_readdir (dirp)) != NULL)
  {    
	/* Get the file type(fmode), if filetype is not already populated, get it from efs_lstat   */
    if (FS_MASK_CHECK (dirent->d_stats_present, FS_DIRENT_HAS_ST_MODE))
    {
      fmode = dirent->d_stat.st_mode;
    }
    else
    {
      memset(path, 0, sizeof(path));
      snprintf(path, MCFG_CARD_INFO_FNAME_MAX, "%s/%s", MCFG_CARD_INFO_DIR, dirent->d_name);
      memset((void *)&f_stat, 0, sizeof(f_stat));
      if(efs_lstat (path, &f_stat) < 0)
      {
        continue;
      }
      fmode = f_stat.st_mode;
    }
    /*Ignore symlinks and other special files/directories if any */
    if (S_ISREG(fmode) || S_ISITM(fmode))
    {
      num_of_cards_info++;
    }

   }

   efs_closedir (dirp);
   MCFG_MSG_HIGH_1("mcfg_uim_get_num_of_cards_info_from_efs:num_of_cards_info in EFS - %u", num_of_cards_info);
   return num_of_cards_info;

}

/*===========================================================================

  FUNCTION mcfg_uim_cards_info_make_room

  DESCRIPTION
    This function will make space for the new cards by removing the old cards info.
  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_cards_info_make_room( void )
{
  boolean is_last_card_info_present[MCFG_NUM_OF_UIM_SLOT] = {0};
  mcfg_slot_index_type_e_type sloti;
  EFSDIR *dirp = NULL;
  struct fs_dirent *dirent = NULL;
  struct fs_stat f_stat;
  uint32 fmode;
  char path[MCFG_CARD_INFO_FNAME_MAX];
  uint16 num_of_cards_info_deleted = 0;
  int16 num_of_cards_info_to_delete = 0;
  
  /* Calculate num of cards info to delete */
  num_of_cards_info_to_delete =  (mcfg_uim_current_num_of_cards_info) - (mcfg_uim_max_num_of_cards_info - MCFG_NUM_OF_CARDS_INFOS_TO_DELETE);
  MCFG_ASSERT_COND_RET(num_of_cards_info_to_delete > 0);
  
  MCFG_MSG_HIGH_1("mcfg_uim_cards_info_make_room:num of cards info to delete - %d",num_of_cards_info_to_delete);
  /*Get last_iccid for each slot*/
  for ( sloti= MCFG_SLOT_INDEX_FIRST; sloti < MCFG_NUM_OF_UIM_SLOT; sloti ++)
  {
    
    if(strlen(mcfg_uim_state.card_info[sloti].ICCID)==0)
    {
      if(mcfg_uim_get_card_info(sloti, NULL, &mcfg_uim_state.card_info[sloti]))
      {
        MCFG_MSG_HIGH_1("mcfg_uim_cards_info_make_room: last iccid present for slot %d",(int)sloti);
        is_last_card_info_present[sloti] = TRUE;
      }
    }
    else
    {
      is_last_card_info_present[sloti] = TRUE;
    }
  }
  
  /* Open the cards directory */
  dirp = efs_opendir (MCFG_CARD_INFO_DIR);
  if(!dirp)
  {
    MCFG_MSG_ERROR_1("mcfg_uim_cards_info_make_room: unable to open cards directory for error - %d",efs_errno);
    return ;
  }
  /* Iterate through each item in cards directory */
  while ((dirent = efs_readdir (dirp)) != NULL)
  {
	memset(path, 0, sizeof(path));
    snprintf(path, MCFG_CARD_INFO_FNAME_MAX, "%s/%s", MCFG_CARD_INFO_DIR, dirent->d_name);

    /* Get the file type(fmode), if filetype is not already populated, get it from efs_lstat   */
    if (FS_MASK_CHECK (dirent->d_stats_present, FS_DIRENT_HAS_ST_MODE))
    {
      fmode = dirent->d_stat.st_mode;
    }
    else
    {
      memset((void *)&f_stat, 0, sizeof(f_stat));
      if(efs_lstat (path, &f_stat) < 0)
      {
        continue;
      }
      fmode = f_stat.st_mode;
    }
    /*Ignore symlinks and other special files/directories if any */
    if (S_ISREG(fmode) || S_ISITM(fmode))
    {
      if(efs_unlink(path) != 0)
      {
        MCFG_MSG_ERROR("mcfg_uim_cards_info_make_room: Deletion of card info failed");
        MCFG_MSG_SPRINTF_1("%s",path);		
	  }
	  num_of_cards_info_deleted++;
	  
	  /* This never goes below zero, already handled this above through assert check */
	  mcfg_uim_current_num_of_cards_info--;
    }

	if(num_of_cards_info_deleted >= num_of_cards_info_to_delete)
    {
      break;
	}

   }
   
   MCFG_MSG_HIGH_2("mcfg_uim_cards_info_make_room: Num of card infos deleted - %u current num of cards - %u", num_of_cards_info_deleted, mcfg_uim_current_num_of_cards_info);
   
   for (sloti= MCFG_SLOT_INDEX_FIRST; sloti < MCFG_NUM_OF_UIM_SLOT; sloti++)
   {
     if(is_last_card_info_present[sloti])
     {
       /* Check if last iccid card_info is deleted or not. If it is deleted write it back into EFS */
       if(!mcfg_uim_get_card_info(sloti, mcfg_uim_state.card_info[sloti].ICCID, &mcfg_uim_state.card_info[sloti]))
       {
         MCFG_MSG_MED_1("mcfg_uim_cards_info_make_room: writing back the last iccid back to EFS for slot %d",(int)sloti);
         if(!mcfg_uim_update_card_info(sloti, &mcfg_uim_state.card_info[sloti]))
         {
           MCFG_MSG_ERROR_1("mcfg_uim_cards_info_make_room:failed to write card info  on sloti %d",sloti);
         }
         else
         {
	       mcfg_uim_current_num_of_cards_info++;
         }
       }
	
     }       
   }
   MCFG_MSG_HIGH_1("mcfg_uim_cards_info_make_room: Current_num_of_cards after performing make room - %d", mcfg_uim_current_num_of_cards_info);
   efs_closedir (dirp);
   return;

}

boolean mcfg_uim_remove_all_card_infos
(
  void
)
{
  boolean status = FALSE;

  mcfg_slot_index_type_e_type slot_index;
  MCFG_MSG_MED("mcfg_uim_remove_all_card_infos");
  for(slot_index = MCFG_SLOT_INDEX_FIRST; slot_index < MCFG_NUM_OF_UIM_SLOT; slot_index++)
  {
    memset(&mcfg_uim_state.card_info[slot_index].plmn, 0, sizeof(mcfg_trl_carrier_mcc_mnc_s_type)*(MCFG_UIM_NUM_OF_EXT_SESSION * 2));
    memset(&mcfg_uim_state.card_info[slot_index].IMPI, 0, MCFG_UIM_IMPI_MAX_LEN);
    memset(&mcfg_uim_state.card_info[slot_index].group_id1, 0, MCFG_UIM_GID_STR_BUF_LEN);
    memset(&mcfg_uim_state.card_info[slot_index].group_id2, 0, MCFG_UIM_GID_STR_BUF_LEN);
    memset(&mcfg_uim_state.card_info[slot_index].spn, 0, MCFG_UIM_SPN_STR_BUF_LEN);
    memset(&mcfg_uim_state.card_info[slot_index].config_id, 0, sizeof(mcfg_config_id_s_type));
    mcfg_uim_state.card_info[slot_index].customid = 0;
    mcfg_uim_state.card_info[slot_index].carrier_index = 0;
    mcfg_uim_state.card_info[slot_index].country = 0;
    mcfg_uim_state.card_info[slot_index].ims_feature = 0;
  }
  if(efs_deltree(MCFG_CARD_INFO_DIR)==0)
  {
    /* set current number of card info files count to '0' */
    mcfg_uim_current_num_of_cards_info = 0;
    status = TRUE;
  }

  return status;
}

#ifdef FEATURE_MCFG_UNIT_TESTING
void mcfg_uim_simulate_card_removal(mcfg_sub_id_type_e_type sub)
{
  mcfg_slot_index_type_e_type slot_index; 

  slot_index = mcfg_uim_map_sub_id_to_slot_index(sub);
  MCFG_MSG_MED_2("Simulating card removal event on slot %d index %d", 
                  slot_index + 1, slot_index);
  MCFG_ASSERT_COND_RET(slot_index < MCFG_NUM_OF_UIM_SLOT);

  mcfg_uim_state.slot[slot_index].card_state = MCFG_UIM_CARD_STATE_NO_CARD;
  memset(&mcfg_uim_state.card_info[slot_index], 0, sizeof(mcfg_uim_card_info_s_type));
}
#endif /* FEATURE_MCFG_UNIT_TESTING */

boolean mcfg_uim_get_card_info
(
  mcfg_slot_index_type_e_type sloti,
  const char                 *iccid,
  mcfg_uim_card_info_s_type  *card_info
)
{
  char path[MCFG_CARD_INFO_FNAME_MAX];
  mcfg_fs_sub_id_e_type sub = MCFG_FS_SUBID_0;
  uint8 version = 0xFF; /* Invalid */
  int fd = -1;
  boolean status = TRUE;
  int efsret = 0;

  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);
  memset(path, 0, MCFG_CARD_INFO_FNAME_MAX);

  if (iccid != NULL)
  {
    snprintf(path, MCFG_CARD_INFO_FNAME_MAX, "%s/%s", MCFG_CARD_INFO_DIR, iccid);
  }
  else
  {
    snprintf(path, MCFG_CARD_INFO_FNAME_MAX, "%s/last_iccid", MCFG_CARD_INFO_DIR);
    sub = (mcfg_fs_sub_id_e_type) sloti;
  }

  // Read version from card info file and delete it when it doesn't match with current build version supported
  fd = mcfg_fopen(path, O_RDONLY, MCFG_FS_ALLPERMS, MCFG_FS_TYPE_EFS, sub);
  if (fd < 0)
  {
    MCFG_MSG_MED("mcfg_uim_get_card_info: card info not found");
    return FALSE;
  }

  efsret = mcfg_fread(fd, &version, sizeof(version), MCFG_FS_TYPE_EFS);
  if(efsret <= 0)
  {
    MCFG_MSG_SPRINTF_3("mcfg_uim_get_card_info: Error %d reading version. efsret%d path %s", 
                 efs_errno, efsret, path);
    status = FALSE;
  }

  mcfg_fclose(fd, MCFG_FS_TYPE_EFS);

  MCFG_MSG_LOW_1("mcfg_uim_get_card_info: card info version=%d", version);

  if(version != MCFG_UIM_CARD_INFO_VERSION)
  {
    MCFG_MSG_SPRINTF_2("mcfg_uim_get_card_info: card info version %d is different for path:%s",
                 version, path);
    status = FALSE;
  }

  if(!status)
  {
    /* Delete card info if unable to read version or version is not matching */
    efs_unlink(path);
    return FALSE;
  }

  return (MCFG_FS_STATUS_OK == mcfg_fs_read(path, card_info, sizeof(mcfg_uim_card_info_s_type), MCFG_FS_TYPE_EFS, sub));
}

boolean mcfg_uim_update_iccid
(
  mcfg_slot_index_type_e_type sloti,
  const char *iccid
)
{
  mcfg_uim_card_info_s_type  card_info;
  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);
  MCFG_CHECK_NULL_PTR_RET_FALSE(iccid);
  MCFG_ASSERT_COND_RET_FALSE(strlen(iccid)>0);
  boolean status = FALSE;

  //if cached ICCID has never been set, load as last iccid first
  if(strlen(mcfg_uim_state.card_info[sloti].ICCID)==0)
  {
    MCFG_MSG_LOW("card info cache is NULL"); 
    mcfg_uim_get_card_info(sloti, NULL, &mcfg_uim_state.card_info[sloti]);
  }

  if(memcmp(mcfg_uim_state.card_info[sloti].ICCID, iccid, strlen(iccid))==0)
  {
    MCFG_MSG_HIGH("same card is inserted");
    return FALSE;
  }

  memset(&card_info, 0, sizeof(mcfg_uim_card_info_s_type));
  if (!mcfg_uim_get_card_info(sloti, iccid, &card_info))
  {
    /* Check for max limit*/
    if(mcfg_uim_current_num_of_cards_info >= mcfg_uim_max_num_of_cards_info)
    {
      MCFG_MSG_MED_2("mcfg_uim_update_iccid: current_num_of_cards_info - %u max_num_of_cards_info - %u",mcfg_uim_current_num_of_cards_info,mcfg_uim_max_num_of_cards_info);
	  /* Make room for this new card by deleting few old cards info */
      (void) mcfg_uim_cards_info_make_room();
    }
    
    memset(&mcfg_uim_state.card_info[sloti], 0, sizeof(mcfg_uim_card_info_s_type));
    memscpy(mcfg_uim_state.card_info[sloti].ICCID, MCFG_UIM_ICCID_STR_BUF_LEN,
            iccid, strlen(iccid));
    status = mcfg_uim_update_card_info(sloti, &mcfg_uim_state.card_info[sloti]);

    if(!status)
    {
      MCFG_MSG_ERROR_1("mcfg_uim_update_iccid:failed to write card info on sloti %d",sloti);

    }
    else
    {
	  mcfg_uim_current_num_of_cards_info++;
      MCFG_MSG_MED_1("mcfg_uim_update_iccid: current_num_of_cards_info - %u", mcfg_uim_current_num_of_cards_info);
    }
    MCFG_MSG_MED_1("new card is inserted status=%d", status);
  }
  else
  {
    memset(&mcfg_uim_state.card_info[sloti], 0, sizeof(mcfg_uim_card_info_s_type));
    memscpy(&mcfg_uim_state.card_info[sloti], sizeof(mcfg_uim_card_info_s_type),
            &card_info, sizeof(mcfg_uim_card_info_s_type));
    status = !mcfg_uim_update_card_info(sloti, &card_info);
    MCFG_MSG_MED_1("old card is inserted back status=%d", status);
  }

  return status;
}

/*===========================================================================

  FUNCTION mcfg_uim_read_and_update_uimdata

  DESCRIPTION
    Reads other info (GID1/GID2) from MMGSDI and updates all info in card.

  DEPENDENCIES
    None

  PARAMETERS
    sloti [in] slot index
    session_type [in] session type (GW/1X)
    plmn [in] plmn received from UIM

  RETURN VALUE
    TRUE if card info is changed, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_read_and_update_uimdata
(
  mcfg_slot_index_type_e_type sloti,
  mcfg_uim_ext_session_e_type session_type,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn
)
{
  mcfg_trl_carrier_mcc_mnc_s_type *plmn_cache;
  char group_id[MCFG_UIM_GID_STR_BUF_LEN];
  mmgsdi_data_type gid1;
  mmgsdi_data_type gid2;
  boolean update_status = FALSE;

  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);
  MCFG_ASSERT_COND_RET_FALSE(session_type < MCFG_UIM_NUM_OF_EXT_SESSION);
  MCFG_CHECK_NULL_PTR_RET_FALSE(plmn);

  memset(&gid1, 0, sizeof(mmgsdi_data_type));
  memset(&gid2, 0, sizeof(mmgsdi_data_type));

  //if cached ICCID has never been set, load as last iccid first
  if(strlen(mcfg_uim_state.card_info[sloti].ICCID)==0)
  {
    mcfg_uim_get_card_info(sloti, NULL, &mcfg_uim_state.card_info[sloti]);
  }
  else 
  {
    MCFG_MSG_HIGH_1("card info populated with ICCID %s", mcfg_uim_state.card_info[sloti].ICCID);
  }

  //Check if the first one matches
  plmn_cache = &mcfg_uim_state.card_info[sloti].plmn[session_type][0];
  MCFG_MSG_MED_2("mcfg_uim_read_and_update_uimdata before0 mcc=%d mnc=%d", 
                plmn_cache->mcc, plmn_cache->mnc);
  MCFG_MSG_MED_2("mcfg_uim_read_and_update_uimdata before1 mcc=%d mnc=%d", 
                (plmn_cache+1)->mcc, (plmn_cache+1)->mnc);
  if (plmn_cache->mcc == plmn->mcc && plmn_cache->mnc == plmn->mnc)
  {
    MCFG_MSG_MED_3("no plmn update for sessiontype=%d, mcc=%d mnc=%d", 
                  session_type, plmn_cache->mcc, plmn_cache->mnc);
  }
  else
  {
    update_status |= TRUE;
    //Shift the first to 2nd
    (plmn_cache+1)->mcc = plmn_cache->mcc;
    (plmn_cache+1)->mnc = plmn_cache->mnc;
    //set the first as new plmn
    plmn_cache->mcc = plmn->mcc;
    plmn_cache->mnc = plmn->mnc;
    MCFG_MSG_MED_2("mcfg_uim_read_and_update_uimdata after0 mcc=%d mnc=%d", 
                  plmn_cache->mcc, plmn_cache->mnc);
    MCFG_MSG_MED_2("mcfg_uim_read_and_update_uimdata after1 mcc=%d mnc=%d", 
                  (plmn_cache+1)->mcc, (plmn_cache+1)->mnc);
  }

  /* Read GID info for GW session */
  if(session_type == MCFG_UIM_EXT_SESSION_GW)
  {
    /* Read GID1 value from MMGSDI cache */
    if(!mcfg_uim_read_gid(sloti, session_type, MCFG_GID_INDEX_1, &gid1))
    {
      MCFG_MSG_ERROR_1("mcfg_uim_read_and_update_uimdata: read gid1 failed for slot:%d", sloti);
    }
    else if((gid1.data_len <= 0) || 
            (gid1.data_len > MCFG_UIM_GID_MAX_LEN) ||
            (gid1.data_ptr == NULL))
    {
      MCFG_MSG_ERROR_1("mcfg_uim_read_and_update_uimdata: gid1 read error, length:%ld", gid1.data_len);
    }
    else if(!mcfg_uim_decode_gid(gid1.data_ptr, gid1.data_len, group_id))
    {
      MCFG_MSG_ERROR("Couldn't decode GID1");
    }
    else
    {
      if(memcmp(mcfg_uim_state.card_info[sloti].group_id1, group_id, MCFG_UIM_GID_STR_BUF_LEN)==0)
      {
        MCFG_MSG_SPRINTF_2("mcfg_uim_read_and_update_uimdata: no update of gid1, len:%ld val:%s", 
                strlen(group_id), group_id);
      }
      else 
      {
        update_status |= TRUE;
        memscpy(&mcfg_uim_state.card_info[sloti].group_id1, MCFG_UIM_GID_STR_BUF_LEN,
                group_id, MCFG_UIM_GID_STR_BUF_LEN);
        MCFG_MSG_SPRINTF_2("mcfg_uim_read_and_update_uimdata: gid1 from UIM, len:%ld val:%s", 
                strlen(group_id), group_id);
      }
    }

    if(gid1.data_ptr)
    {
      mcfg_free(gid1.data_ptr);
    }

    /* Read GID2 value from MMGSDI cache */
    if(!mcfg_uim_read_gid(sloti, session_type, MCFG_GID_INDEX_2, &gid2))
    {
      MCFG_MSG_ERROR_1("mcfg_uim_read_and_update_uimdata: read gid2 failed for slot:%d", sloti);
    }
    else if((gid2.data_len <= 0) || 
            (gid2.data_len > MCFG_UIM_GID_MAX_LEN) ||
            (gid2.data_ptr == NULL))
    {
      MCFG_MSG_ERROR_1("mcfg_uim_read_and_update_uimdata: gid2 read error, length:%ld", gid2.data_len);
    }
    else if (!mcfg_uim_decode_gid(gid2.data_ptr, gid2.data_len, group_id))
    {
      MCFG_MSG_ERROR("Couldn't decode GID2");
    }
    else
    {
      if(memcmp(mcfg_uim_state.card_info[sloti].group_id2, group_id, MCFG_UIM_GID_STR_BUF_LEN)==0)
      {
        MCFG_MSG_SPRINTF_2("mcfg_uim_read_and_update_uimdata: no update of gid2, len:%ld val:%s ", 
                strlen(group_id), group_id);
      }
      else 
      {
        update_status |= TRUE;
        memscpy(&mcfg_uim_state.card_info[sloti].group_id2, MCFG_UIM_GID_STR_BUF_LEN,
                group_id, MCFG_UIM_GID_STR_BUF_LEN);
        MCFG_MSG_SPRINTF_2("mcfg_uim_read_and_update_uimdata gid2 from UIM, len:%ld val:%s ", 
                strlen(group_id), group_id);
      }
    }

    if(gid2.data_ptr)
    {
      mcfg_free(gid2.data_ptr);
    }
  }

  if (update_status == TRUE)
  {
    MCFG_MSG_MED("Updating card info");
    update_status = mcfg_uim_update_card_info(sloti, &mcfg_uim_state.card_info[sloti]);
  }

  MCFG_MSG_MED_1("mcfg_uim_read_and_update_uimdata status %d", update_status);
  return update_status;
}

boolean mcfg_uim_update_plmn
(
  mcfg_slot_index_type_e_type   sloti,
  mcfg_uim_ext_session_e_type      session,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn
)
{
  mcfg_trl_carrier_mcc_mnc_s_type *plmn_cache;
  MCFG_ASSERT_COND_RET_FALSE(sloti < MCFG_NUM_OF_UIM_SLOT);
  MCFG_ASSERT_COND_RET_FALSE(session < MCFG_UIM_NUM_OF_EXT_SESSION);
  MCFG_CHECK_NULL_PTR_RET_FALSE(plmn);


  //Check if the first one matches
  plmn_cache = &mcfg_uim_state.card_info[sloti].plmn[session][0];
  MCFG_MSG_MED_2("mcfg_uim_update_plmn before0 mcc=%d mnc=%d", 
                 plmn_cache->mcc, plmn_cache->mnc);
  MCFG_MSG_MED_2("mcfg_uim_update_plmn before1 mcc=%d mnc=%d", 
                 (plmn_cache+1)->mcc, (plmn_cache+1)->mnc);
  if (plmn_cache->mcc == plmn->mcc && plmn_cache->mnc == plmn->mnc)
  {
    return FALSE;
  }
  //Shift the first to 2nd
  (plmn_cache+1)->mcc = plmn_cache->mcc;
  (plmn_cache+1)->mnc = plmn_cache->mnc;
  //set the first as new plmn
  plmn_cache->mcc = plmn->mcc;
  plmn_cache->mnc = plmn->mnc;

  MCFG_MSG_MED_2("mcfg_uim_update_plmn after0 mcc=%d mnc=%d", 
                 plmn_cache->mcc, plmn_cache->mnc);
  MCFG_MSG_MED_2("mcfg_uim_update_plmn after1 mcc=%d mnc=%d", 
                 (plmn_cache+1)->mcc, (plmn_cache+1)->mnc);
  return mcfg_uim_update_card_info(sloti, &mcfg_uim_state.card_info[sloti]);
}

boolean mcfg_uim_update_carrier_index
(
  mcfg_slot_index_type_e_type sloti,
  pdc_selection_carrier_enum_v01 carrier_index
)
{

  return TRUE;
}

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
)
{
  uint32 i;
  uint8 digit;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(iccid_ascii);

  memset(iccid_ascii, 0, MCFG_UIM_ICCID_STR_BUF_LEN);
  if (iccid_bcd)
  {
    for (i = 0; i < MCFG_UIM_ICCID_LEN; i++)
    {
      digit = (iccid_bcd[i] & 0x0F);
      if (digit < 0xA)
      {
        iccid_ascii[i * 2] = (char) (digit + 0x30);
      }
      else
      {
        /* A~F: 65~70 */
        iccid_ascii[i * 2] = (char) (digit + 55);
      }

      digit = ((iccid_bcd[i] >> 4) & 0x0F);
      if (digit < 0xA)
      {
        iccid_ascii[(i * 2) + 1] = (char) (digit + 0x30);
      }
      else
      {
        /* A~F: 65~70 */
        iccid_ascii[(i * 2) + 1] = (char) (digit + 55);
      }
    }
  }
  else
  {
    MCFG_MSG_MED("set all zero for invalid ICCID");
    memset(iccid_ascii, 0x30, MCFG_UIM_ICCID_STR_BUF_LEN-1);
  }

  return TRUE;
} /* mcfg_uim_decode_iccid() */

/*===========================================================================

  FUNCTION mcfg_uim_decode_gid

  DESCRIPTION
    Decodes the GID from its SIM encoding (binary or hex bytes) into an
    ASCII string.

  DEPENDENCIES
    None

  PARAMETERS
    gid_bcd    [in] input GID in EF-GID format (binary/hex bytes)
    data_len   [in] input GID data length valid
    gid_ascii [out] array to store decoded GID; buffer must be at least
                      MCFG_UIM_GID_STR_BUF_LEN bytes long

  RETURN VALUE
    TRUE if GID successfully decoded, FALSE otherwise (Null pointers)

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_decode_gid
(
  const uint8 *gid_bcd,
  uint32       data_len,
  char        *gid_ascii
)
{
  uint32 i;
  uint8 digit;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(gid_bcd);
  MCFG_CHECK_NULL_PTR_RET_FALSE(gid_ascii);
  MCFG_ASSERT_COND_RET_FALSE(data_len <= MCFG_UIM_GID_MAX_LEN);

  memset(gid_ascii, 0, MCFG_UIM_GID_STR_BUF_LEN);
  if (gid_bcd)
  {
    for (i = 0; i < data_len; i++)
    {
      digit = ((gid_bcd[i] >> 4) & 0x0F);
      if (digit < 0xA)
      {
        gid_ascii[i * 2] = (char) (digit + 0x30);
      }
      else
      {
        /* A~F: 65~70 */
        gid_ascii[i * 2] = (char) (digit + 55);
      }

      digit = (gid_bcd[i] & 0x0F);
      if (digit < 0xA)
      {
        gid_ascii[(i * 2) + 1] = (char) (digit + 0x30);
      }
      else
      {
        /* A~F: 65~70 */
        gid_ascii[(i * 2) + 1] = (char) (digit + 55);
      }
    }
  }

  return TRUE;
} /* mcfg_uim_decode_gid() */

#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
/*===========================================================================

  FUNCTION mcfg_uim_iccid_blacklisted

  DESCRIPTION
    Check ICCID is blacklisted for selection, if yes, fall back to imsi selection

  DEPENDENCIES
    None

  PARAMETERS
    iccid_ascii [in] array to store decoded ICCID; buffer must be at least
                      MCFG_UIM_ICCID_STR_BUF_LEN bytes long

  RETURN VALUE
    TRUE if ICCID is blacklisted, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_uim_iccid_blacklisted
(
  char *iccid
)
{
   /* TODO: add new EFS items for a list of blacklisted iccid */
  return TRUE;
} /* mcfg_uim_iccid_blacklisted */

/*===========================================================================

  FUNCTION mcfg_uim_decode_imsi

  DESCRIPTION
    Decodes the EF-IMSI format into PLMN/MCC_MNC.

  DEPENDENCIES
    None

  PARAMETERS
    imsi  [in]  input imsi in EF-imsi format
    plmn  [out] MCC/MNC

  RETURN VALUE
    TRUE if imsi successfully decoded, FALSE otherwise (e.g. bad imsi)

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_uim_decode_imsi
(
  const uint8 *imsi,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn,
  uint8 num_of_mnc
)
{
  uint8 i = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(imsi);
  MCFG_CHECK_NULL_PTR_RET_FALSE(plmn);
  
  plmn->mcc = (imsi[1] >>  4)*100 + (imsi[2] & 0xF)*10 + (imsi[2] >>  4);

  if (num_of_mnc == 3)
  {
    plmn->mnc = (imsi[3] & 0xF)*100 + (imsi[3] >> 4)*10 + (imsi[4] & 0xF);
  }
  else
  {
    plmn->mnc = (imsi[3] & 0xF)*10 + (imsi[3] >> 4);
  }

  MCFG_MSG_HIGH_3("mcfg_uim_decode_imsi %d digit mcc=%03d mnc=%03d", num_of_mnc, plmn->mcc, plmn->mnc);
  return TRUE;
} /* mcfg_uim_decode_imsi() */

static boolean mcfg_uim_decode_cdma_imsi
(
  const uint8 *cdma_imsi,
  mcfg_trl_carrier_mcc_mnc_s_type *plmn
)
{
  uint8  tens = 0;
  uint8  ones = 0;
  uint8  hundreds = 0;
  uint16 temp = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(cdma_imsi);
  MCFG_CHECK_NULL_PTR_RET_FALSE(plmn);
  
  //MCC
  temp =  (cdma_imsi[MMGSDI_CDMA_IMSI_LEN - 1] & 0x3);
  temp =  (temp << 8);
  temp |=  cdma_imsi[MMGSDI_CDMA_IMSI_LEN - 2];
  MCFG_MSG_LOW_1("mcfg_uim_decode_cdma_imsi mcc 0x%03x", temp);
  temp += 111;

  ones = temp - 10 * (temp / 10);
  tens = (temp / 10) - 10 * (temp / 100);
  hundreds = temp / 100;
  MCFG_MSG_LOW_3("mcfg_uim_decode_cdma_imsi mcc %d hundreds %d tens %d ones", hundreds, tens, ones);

  if (ones == 0) {tens--;}
  if (tens == 0) {hundreds--;}
  plmn->mcc = 100 * hundreds + 10 * tens + ones;

  //MNC
  temp = cdma_imsi[MMGSDI_CDMA_IMSI_LEN - 4];
  MCFG_MSG_LOW_1("mcfg_uim_decode_cdma_imsi mcc 0x%02x", temp);
  temp += 11;

  ones = temp - 10 * (temp / 10);
  tens = (temp / 10) - 10 * (temp / 100);
  hundreds = temp / 100;
  MCFG_MSG_LOW_3("mcfg_uim_decode_cdma_imsi mnc %d hundreds %d tens %d ones", hundreds, tens, ones);

  if (ones == 0) {tens--;}
  if (tens == 0) {hundreds--;}
  plmn->mnc = 10 * tens + ones;

  MCFG_MSG_HIGH_2("mcfg_uim_decode_cdma_imsi mcc=%03d mnc=%02d", plmn->mcc, plmn->mnc);
  return TRUE;
} /* mcfg_uim_decode_cdma_imsi() */
#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */

/*===========================================================================

  FUNCTION mcfg_uim_map_session_id_to_slot_index

  DESCRIPTION

  DEPENDENCIES
    None

  PARAMETERS
    session id

  RETURN VALUE
    slot index

  SIDE EFFECTS
    None

===========================================================================*/
static uint8 mcfg_uim_map_session_id_to_slot_index
(
  mmgsdi_session_id_type session_id
)
{
  uint8 slot_index;
  uint8 ext_index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (slot_index = 0; slot_index < MCFG_NUM_OF_UIM_SLOT; slot_index++)
  {
    if (mcfg_uim_state.slot[slot_index].session_id == session_id)
    {
      return slot_index;
    }

    for (ext_index = 0; ext_index < MCFG_UIM_NUM_OF_EXT_SESSION; ext_index++)
    {
      if (mcfg_uim_state.sub[slot_index].ext_session_id[ext_index] == session_id)
      {
        return mcfg_uim_map_sub_id_to_slot_index(slot_index);
      }
    }
  }

  return MCFG_NUM_OF_UIM_SLOT;
} /* mcfg_uim_map_session_id_to_slot_index() */

/*===========================================================================

  FUNCTION mcfg_uim_map_session_id_to_sub_id

  DESCRIPTION

  DEPENDENCIES
    None

  PARAMETERS
    session id

  RETURN VALUE
    slot index

  SIDE EFFECTS
    None

===========================================================================*/
static mcfg_sub_id_type_e_type mcfg_uim_map_session_id_to_sub_id
(
  mmgsdi_session_id_type session_id
)
{
  mcfg_sub_id_type_e_type sub_id;
  uint8 ext_index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (sub_id = 0; sub_id < MCFG_NUM_OF_SUB_ID; sub_id++)
  {
    for (ext_index = 0; ext_index < MCFG_UIM_NUM_OF_EXT_SESSION; ext_index++)
    {
      MCFG_MSG_LOW_3("sub %d gw/1x %d %x", sub_id, ext_index, mcfg_uim_state.sub[sub_id].ext_session_id[ext_index]);
      if (mcfg_uim_state.sub[sub_id].ext_session_id[ext_index] == session_id)
      {
        return sub_id;
      }
    }
  }

  MCFG_MSG_ERROR_1("Session ID not found %llx", session_id);
  return MCFG_NUM_OF_SUB_ID;
} /* mcfg_uim_map_session_id_to_sub_id() */

/*===========================================================================

  FUNCTION mcfg_uim_map_mmgsdi_slot_to_slot_index

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
static mcfg_slot_index_type_e_type mcfg_uim_map_mmgsdi_slot_to_slot_index
(
  mmgsdi_slot_id_enum_type slot_id
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_ERROR(slot_id > 0, MCFG_NUM_OF_UIM_SLOT);

  return (slot_id - 1);
} /* mcfg_uim_map_mmgsdi_slot_to_slot_index() */

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
mcfg_slot_index_type_e_type mcfg_uim_map_sub_id_to_slot_index
(
  mcfg_sub_id_type_e_type sub_id
)
{
  mcfg_slot_index_type_e_type slot_index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_ERROR(sub_id < MCFG_NUM_OF_SUB_ID, MCFG_NUM_OF_UIM_SLOT);

  for (slot_index = 0; slot_index < MCFG_NUM_OF_UIM_SLOT; slot_index++ )
  {
    if (mcfg_uim_state.slot[slot_index].sub_id == sub_id)
    {
      return slot_index;
    }
  }


  return MCFG_NUM_OF_UIM_SLOT;
} /* mcfg_uim_map_sub_id_to_slot_index() */

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
)
{
  mcfg_sub_id_type_e_type sub_id;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_ERROR(slot_index < MCFG_NUM_OF_UIM_SLOT, MCFG_NUM_OF_SUB_ID);

  return mcfg_uim_state.slot[slot_index].sub_id;
} /* mcfg_uim_map_sub_id_to_slot_index() */

/*===========================================================================

  FUNCTION mcfg_uim_write_mapping

  DESCRIPTION
    Write UIM mapping table into EFS

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_write_mapping
(
  void
)
{
  mcfg_nv_status_e_type       status;
  mcfg_slot_index_type_e_type slot_index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_MSG_LOW("Write mapping table");

  for (slot_index = MCFG_SLOT_INDEX_FIRST; 
        slot_index < MCFG_NUM_OF_UIM_SLOT; slot_index++)
  {
    if (mcfg_uim_state.slot[slot_index].sub_id == 
        (mcfg_sub_id_type_e_type)slot_index)
    {
      status = mcfg_nv_delete(MCFG_NV_ITEM_UIM_SLOT_MAPPING, 0, slot_index);
      if (status != MCFG_NV_STATUS_OK)
      {
        MCFG_MSG_ERROR_2("Couldn't delete uim mapping slot %d NV item: status %d",
                         slot_index, status);
        return FALSE;
      }
    }
    else
    {
      status = mcfg_nv_write(MCFG_NV_ITEM_UIM_SLOT_MAPPING, 0, slot_index, 
                             (void *) &mcfg_uim_state.slot[slot_index].sub_id, 
                             sizeof(mcfg_uim_state.slot[slot_index].sub_id));
      if (status != MCFG_NV_STATUS_OK)
      {
        MCFG_MSG_ERROR_2("Couldn't write uim mapping slot %d NV item: status %d",
                         slot_index, status);
        return FALSE;
      }
    }
  }

  return TRUE;
} /* mcfg_uim_write_mapping() */

/*===========================================================================

  FUNCTION mcfg_uim_validate_mapping

  DESCRIPTION
    Validate UIM mapping table, if it's not validate, set as 1:1 mapping

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_validate_mapping
(
  void
)
{
  mcfg_slot_index_type_e_type slot_index;
  uint8                       sub_mask = 0;
  boolean                     valid = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (slot_index = 0; slot_index < MCFG_NUM_OF_UIM_SLOT; slot_index++)
  {
    if ((mcfg_uim_state.slot[slot_index].sub_id >= MCFG_NUM_OF_SUB_ID) ||
        (sub_mask & (1 << mcfg_uim_state.slot[slot_index].sub_id)))
    {
      MCFG_MSG_ERROR_3("Invalid mapping slot %d to sub_id %d with submask %x",
                       slot_index, mcfg_uim_state.slot[slot_index].sub_id, sub_mask);
      valid = FALSE;
      break;
    }
  }

  if (!valid)
  {
    for (slot_index = 0; slot_index < MCFG_NUM_OF_UIM_SLOT; slot_index++)
    {
      mcfg_uim_state.slot[slot_index].sub_id = (mcfg_sub_id_type_e_type)slot_index;
    }
    mcfg_uim_write_mapping();
  }

  return valid;
} /* mcfg_uim_validate_mapping() */

/*===========================================================================

  FUNCTION mcfg_uim_read_mapping

  DESCRIPTION
    Read UIM mapping table from EFS and validate it

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_read_mapping
(
  void
)
{
  mcfg_slot_index_type_e_type slot_index;
  mcfg_sub_id_type_e_type     sub_id;
  mcfg_nv_status_e_type       status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (slot_index = 0; slot_index < MCFG_NUM_OF_UIM_SLOT; slot_index++)
  {
    status = mcfg_nv_read(MCFG_NV_ITEM_UIM_SLOT_MAPPING, 0, slot_index, 
                          &sub_id, sizeof(sub_id));
    if (status == MCFG_NV_STATUS_NOT_ACTIVE)
    {
      mcfg_uim_state.slot[slot_index].sub_id = (mcfg_sub_id_type_e_type)slot_index;
    }
    else if (status == MCFG_NV_STATUS_OK && sub_id < MCFG_NUM_OF_SUB_ID)
    {
      mcfg_uim_state.slot[slot_index].sub_id = sub_id;
    }
    else
    {
      mcfg_uim_state.slot[slot_index].sub_id = MCFG_SUB_ID_INVALID;
    }
  }

  return mcfg_uim_validate_mapping();
} /* mcfg_uim_read_mapping() */

/*===========================================================================

  FUNCTION mcfg_uim_process_mapping

  DESCRIPTION
    Process UIM mapping table for flex mapping

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE
    TRUE if MBN is switched, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_process_mapping
(
  mcfg_slot_index_type_e_type slot_index,
  mcfg_sub_id_type_e_type     sub_id,
  boolean                     refresh
)
{
  mcfg_config_id_s_type       active_id;
  mcfg_config_id_s_type       oppo_active_id;
  static mcfg_slot_index_type_e_type oppo_slot_index = 0;
  static mcfg_sub_id_type_e_type     oppo_sub_id = 0;
  boolean config_matches = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(slot_index < MCFG_NUM_OF_UIM_SLOT);
  MCFG_ASSERT_COND_RET_FALSE(sub_id < MCFG_NUM_OF_SUB_ID);

  MCFG_MSG_LOW_3("Update mapping slot %d to sub %d from %d",slot_index, sub_id,
                 mcfg_uim_state.slot[slot_index].sub_id);
  if(mcfg_uim_state.slot[slot_index].sub_id == sub_id)
  {
    MCFG_MSG_MED_2("Same mapping, ignore slot %d to sub %d",slot_index, sub_id);
    return FALSE;
  }

  memset(&active_id,      0, sizeof(active_id));
  memset(&oppo_active_id, 0, sizeof(oppo_active_id));

  oppo_sub_id = mcfg_uim_state.slot[slot_index].sub_id;

  for (oppo_slot_index = MCFG_SLOT_INDEX_FIRST; 
        oppo_slot_index < MCFG_NUM_OF_UIM_SLOT; oppo_slot_index++)
  {
    if(mcfg_uim_state.slot[oppo_slot_index].sub_id == sub_id)
    {
      MCFG_MSG_MED_1("Get active ID for sub %d", sub_id);
      if(!mcfg_utils_get_active_config(MCFG_TYPE_SW, &active_id, sub_id))
      {
        MCFG_MSG_MED_1("No active ID for sub %d, get pending instead", sub_id);
        if(!mcfg_utils_get_selected_config(MCFG_TYPE_SW, &active_id, sub_id))
        {
          MCFG_MSG_MED_1("No pending ID for sub %d either", sub_id);
        }
      }

      MCFG_MSG_MED_1("Get active ID for oppo sub %d", oppo_sub_id);
      if(!mcfg_utils_get_active_config(MCFG_TYPE_SW, &oppo_active_id, oppo_sub_id))
      {
        MCFG_MSG_MED_1("No active ID for oppo sub %d, get pending instead", oppo_sub_id);
        if(!mcfg_utils_get_selected_config(MCFG_TYPE_SW, &oppo_active_id, oppo_sub_id))
        {
          MCFG_MSG_MED_1("No pending ID for oppo sub %d either", oppo_sub_id);
        }
      }

      config_matches = mcfg_utils_config_id_matches(&active_id, &oppo_active_id);
      if (!config_matches)
      {
        MCFG_MSG_MED_1("Deactivate sub items for sub %d", sub_id);
        mcfg_utils_deactivate_config_by_type(MCFG_TYPE_SW, sub_id, FALSE, MCFG_REFRESH_TYPE_SUBS);

        MCFG_MSG_MED_1("Deactivate sub items for oppo sub %d", oppo_sub_id);
        mcfg_utils_deactivate_config_by_type(MCFG_TYPE_SW, oppo_sub_id, FALSE, MCFG_REFRESH_TYPE_SUBS);

        if (oppo_active_id.id_len > 0)
        {
          MCFG_MSG_MED_1("Select for sub %d", sub_id);
          mcfg_utils_set_selected_config(MCFG_TYPE_SW, &oppo_active_id, sub_id);
        }

        if (active_id.id_len > 0)
        {
          MCFG_MSG_MED_1("Select for oppo sub %d", oppo_sub_id);
          mcfg_utils_set_selected_config(MCFG_TYPE_SW, &active_id, oppo_sub_id);
        }
      }

      mcfg_uim_state.slot[oppo_slot_index].sub_id = oppo_sub_id;
      mcfg_uim_state.slot[slot_index].sub_id = sub_id;

      mcfg_uim_write_mapping();
      MCFG_MSG_MED("clean up saved iccid for flex mapping");
#ifndef FEATURE_MCFG_SEL_DB
    mcfg_sel_sw_config_store_updated(MCFG_NUM_OF_UIM_SLOT);
#endif

      if (!config_matches)
      {
        if (!refresh)
        {
        MCFG_MSG_HIGH_4("Activate after flex mapping slot%d-sub%d slot%d-sub%d", 
                        slot_index, sub_id, oppo_slot_index, oppo_sub_id);
        mcfg_utils_activate_config(MCFG_TYPE_SW, MCFG_ACTIVATION_CROSSMAPPING);
      }
#ifdef FEATURE_MCFG_REFRESH
      else
      {
          MCFG_MSG_MED_4("MBN pending for refresh after flex mapping slot%d-sub%d slot%d-sub%d", 
                          slot_index, sub_id, oppo_slot_index, oppo_sub_id);
        }
#endif
      }
      else
      {
        MCFG_MSG_MED_4("No need to reset after flex mapping slot%d-sub%d slot%d-sub%d for matching config", 
                        slot_index, sub_id, oppo_slot_index, oppo_sub_id);
      }

      return (!config_matches);
    }
  }

  MCFG_MSG_ERROR_2("Can't find matching pair for flex mapping slot %d to sub %d",
                   slot_index, sub_id);
  return FALSE;
} /* mcfg_uim_process_mapping() */

/*===========================================================================

  FUNCTION mcfg_uim_update_mapping

  DESCRIPTION
    Update UIM mapping table for flex mapping

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_update_mapping
(
  mcfg_slot_index_type_e_type slot_index,
  mcfg_sub_id_type_e_type     sub_id,
  boolean                     timeout
)
{
  static mcfg_slot_index_type_e_type cached_slot_index = 0;
  static mcfg_sub_id_type_e_type     cached_sub_id = 0;
  static boolean              holding = FALSE;
  boolean                     ret = TRUE;
  uint8                       timer_len;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(slot_index < MCFG_NUM_OF_UIM_SLOT);
  MCFG_ASSERT_COND_RET_FALSE(sub_id < MCFG_NUM_OF_SUB_ID);

  if(timeout)
  {
    if (holding)
    {
      mcfg_uim_process_mapping(cached_slot_index, cached_sub_id, FALSE);
      cached_slot_index = 0;
      cached_sub_id = 0;
      holding = FALSE;
    }
    else
    {
      MCFG_MSG_ERROR("Ignore timeout cb without holding");
    }
  }
  else
  {
    MCFG_MSG_MED_3("Update mapping slot %d to sub %d from %d", slot_index, sub_id,
                   mcfg_uim_state.slot[slot_index].sub_id);
    if(mcfg_uim_state.slot[slot_index].sub_id == sub_id)
    {
      MCFG_MSG_LOW_2("Same mapping, ignore slot %d to sub %d", slot_index, sub_id);
    }
    else if (holding == FALSE)
    {
      timer_len = mcfg_uim_get_flexmap_timer();
      if (timer_len == 0)
      {
        mcfg_uim_process_mapping(slot_index, sub_id, FALSE);
      }
      else
      { 
        MCFG_MSG_HIGH_2("Deboucing flex mapping slot%d-sub%d", slot_index, sub_id);
        timer_set(&mcfg_uim_flexmap_timer, timer_len, 0, T_SEC);
        cached_slot_index = slot_index;
        cached_sub_id = sub_id;
        holding = TRUE;
      }
    }
    else if (cached_slot_index == slot_index && cached_sub_id == sub_id)
    {
      MCFG_MSG_LOW_2("Same mapping as cached, ignore slot %d to sub %d", slot_index, sub_id);
      ret = FALSE;
    }
    else
    {
      timer_clr(&mcfg_uim_flexmap_timer, T_SEC);
      cached_slot_index = 0;
      cached_sub_id = 0;
      holding = FALSE;
      mcfg_uim_process_mapping(slot_index, sub_id, FALSE);
    }
  }

  return ret;
} /* mcfg_uim_update_mapping() */

/*===========================================================================

  FUNCTION mcfg_uim_flexmap_timer_cb

  DESCRIPTION
    Flexmap Timer callback. 

  DEPENDENCIES
    None

  PARAMETERS
    data    [in]  timer callback data

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_flexmap_timer_cb(timer_cb_data_type data)
{
  mcfg_task_cmd_s_type *cmd;
  mcfg_uim_cmd_data_s_type *uim_cmd_data;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_MSG_HIGH("flex mapping cb");
  cmd = mcfg_task_cmd_alloc(
    MCFG_TASK_CMD_UIM, sizeof(mcfg_uim_cmd_data_s_type));
  if (cmd != NULL)
  {
    uim_cmd_data = (mcfg_uim_cmd_data_s_type *) cmd->data;
    uim_cmd_data->cmd_id = MCFG_UIM_CMD_MAPPING_TIMEOUT;

    if (!mcfg_task_cmd_send(cmd))
    {
      MCFG_MSG_ERROR("Couldn't send command for MMGSDI event!");
      mcfg_task_cmd_free(cmd);
    }
  }
} /* mcfg_utils_cmd_reset_timer_cb() */

/*===========================================================================

  FUNCTION mcfg_uim_proc_mapping_timeout

  DESCRIPTION
    Update UIM mapping table for flex mapping

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_proc_mapping_timeout
(
  void
)
{
  mcfg_uim_update_mapping(MCFG_SLOT_INDEX_FIRST, MCFG_SUB_ID_FIRST, TRUE);
}

/*===========================================================================

  FUNCTION mcfg_uim_reselect

  DESCRIPTION
    Reselect

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_reselect
(
  mcfg_slot_index_type_e_type sloti
)
{
  pdc_get_feature_ind_msg_v01 feature;
  mcfg_sub_id_type_e_type sub;
  mcfg_uim_ext_session_e_type ext_session;

  MCFG_ASSERT_COND_RET(sloti < MCFG_NUM_OF_UIM_SLOT);

  MCFG_MSG_LOW_1("mcfg_uim_reselect sloti %d", sloti);
#ifdef FEATURE_MCFG_FEATURETAG
  if (MCFG_ERR_NONE != mcfg_get_feature(sloti, MCFG_SETTING_CURR, &feature))
#else
  if (MCFG_ERR_NONE != mcfg_get_feature(sloti, &feature))
#endif
  {
    MCFG_MSG_ERROR_1("mcfg_uim_reselect sloti %d fail to get feature", sloti);
    return;
  }

  if (feature.selection_mode == PDC_SELECTION_MODE_DISABLED_V01)
  {
#ifdef FEATURE_MCFG_REFRESH
    mcfg_refresh_abort_all();
#endif

    MCFG_MSG_MED_2("mcfg_uim_reselect sloti %d select disabled %d cleanup saved iccid/imsi", sloti, feature.selection_mode);
#ifndef FEATURE_MCFG_SEL_DB
    mcfg_sel_sw_config_store_updated(MCFG_NUM_OF_UIM_SLOT);
#else
    mcfg_uim_remove_all_card_infos();
#endif
  }

  if (feature.selection_mode == PDC_SELECTION_MODE_DISABLED_V01)
  {
    MCFG_MSG_MED_2("mcfg_uim_reselect off sloti %d select mode %d", sloti, feature.selection_mode);
    mcfg_sel_activate_sw_config(feature.carrier,
       mcfg_uim_map_slot_index_to_sub_id(sloti));
  }
  else // modem centric selection
  {
    if(!mcfg_uim_state.slot[sloti].ready)
    {
      MCFG_MSG_MED_2("mcfg_uim_reselect sloti %d not ready %d", sloti, mcfg_uim_state.slot[sloti].ready);
    }
    else
    {
      if (feature.selection_mode == PDC_SELECTION_MODE_IIN_V01)
      {
        MCFG_MSG_MED_2("mcfg_uim_reselect iin sloti %d select mode %d", sloti, feature.selection_mode);
        mcfg_uim_read_iccid_if_ready(sloti);
      }
      else
      {
        MCFG_MSG_MED_2("mcfg_uim_reselect imsi sloti %d select mode %d", sloti, feature.selection_mode);
        sub = mcfg_uim_map_slot_index_to_sub_id(sloti);
        MCFG_ASSERT_COND_RET(sub < MCFG_NUM_OF_SUB_ID);

        mcfg_uim_read_imsi_if_ready(sloti, mcfg_uim_state.sub[sub].ext_app_type[MCFG_UIM_EXT_SESSION_GW]);
        mcfg_uim_read_imsi_if_ready(sloti, mcfg_uim_state.sub[sub].ext_app_type[MCFG_UIM_EXT_SESSION_1X]);
      }
    }
  }
}


/*===========================================================================

  FUNCTION mcfg_uim_update_app_type

  DESCRIPTION

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE

  SIDE EFFECTS
    None

===========================================================================*/
static boolean mcfg_uim_update_app_type
(
      mmgsdi_session_id_type session_id, 
      mmgsdi_app_enum_type   app_type
)
{
  mcfg_sub_id_type_e_type sub_id;
  uint8 ext_index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  for (sub_id = 0; sub_id < MCFG_NUM_OF_SUB_ID; sub_id++)
  {
    for (ext_index = 0; ext_index < MCFG_UIM_NUM_OF_EXT_SESSION; ext_index++)
    {
      MCFG_MSG_LOW_3("sub %d gw/1x %d %x", sub_id, ext_index, mcfg_uim_state.sub[sub_id].ext_session_id[ext_index]);
      if (mcfg_uim_state.sub[sub_id].ext_session_id[ext_index] == session_id)
      {
        mcfg_uim_state.sub[sub_id].ext_app_type[ext_index] = app_type;

        return TRUE;
      }
    }
  }

  MCFG_MSG_ERROR_2("Session ID not found %llx for app type %d", session_id, app_type);
  return FALSE;

}

/*===========================================================================

  FUNCTION mcfg_uim_mmgsdi_client_id_and_evt_reg_cnf_proc

  DESCRIPTION
    Processes the response to mmgsdi_client_id_and_evt_reg(). If successful,
    attempts to open a card session on slot 1.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_mmgsdi_client_id_and_evt_reg_cnf_proc
(
  mmgsdi_return_enum_type                      status,
  const mmgsdi_client_id_and_evt_reg_cnf_type *cnf_data
)
{
  mmgsdi_session_type_enum_type slot_session[] = 
  {
    MMGSDI_CARD_SESSION_SLOT_1_MASK
    #ifdef FEATURE_MCFG_MULTISIM_SUPPORT
    ,MMGSDI_CARD_SESSION_SLOT_2_MASK
    #ifdef FEATURE_MCFG_MULTISIM_TSTS_SUPPORT
    ,MMGSDI_CARD_SESSION_SLOT_3_MASK
    #endif
    #endif
  };
  mmgsdi_session_type_enum_type sub_session[] = 
  {
    MMGSDI_GW_PROV_PRI_SESSION_MASK | MMGSDI_1X_PROV_PRI_SESSION_MASK
    #ifdef FEATURE_MCFG_MULTISIM_SUPPORT
    ,MMGSDI_GW_PROV_SEC_SESSION_MASK | MMGSDI_1X_PROV_SEC_SESSION_MASK
    #ifdef FEATURE_MCFG_MULTISIM_TSTS_SUPPORT
    ,MMGSDI_GW_PROV_TER_SESSION_MASK | MMGSDI_1X_PROV_TER_SESSION_MASK
    #endif
    #endif
  };
  uint8 slot_index;
  uint32 session_type_mask  = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(cnf_data);

  if (status != MMGSDI_SUCCESS)
  {
    MCFG_MSG_ERROR_1("Allocating MMGSDI client ID failed with status %d",
                     status);
    return;
  }

  MCFG_MSG_MED("Successfully allocated MMGSDI client ID");
  mcfg_uim_state.client_id = cnf_data->response_header.client_id;

  for (slot_index = MCFG_SLOT_INDEX_FIRST; 
        slot_index < MCFG_NUM_OF_UIM_SLOT; slot_index++)
    {
      //if (mcfg_uim_nv_autoselect_enabled(slot_index))
      {
        session_type_mask |= slot_session[slot_index];
      }

      //if (mcfg_uim_nv_flexmap_enabled(slot_index))
      {
        session_type_mask |= sub_session[slot_index];
      }

      //if (mcfg_uim_nv_imsiselect_enabled(slot_index))
      {
        session_type_mask |= slot_session[slot_index];
        session_type_mask |= sub_session[slot_index];
      }
    }

  MCFG_MSG_MED_1("Opening ext session with mask %X", session_type_mask);
  status = mmgsdi_session_open_ext(mcfg_uim_state.client_id,
                                   session_type_mask,
                                   mcfg_uim_mmgsdi_evt_cb,
                                   0,
                                   mcfg_uim_mmgsdi_rsp_cb,
                                   0);
  if (status != MMGSDI_SUCCESS)
      {
    MCFG_MSG_ERROR_1("Opening ext sessions failed with status %d", status);
  }
} /* mcfg_uim_mmgsdi_client_id_and_evt_reg_cnf_proc() */

/*===========================================================================

  FUNCTION mcfg_uim_mmgsdi_evt_cb

  DESCRIPTION
    MMGSDI event callback. Posts command to MCFG task for processing.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_mmgsdi_evt_cb
(
  const mmgsdi_event_data_type *event
)
{
  mcfg_task_cmd_s_type *cmd;
  mcfg_uim_cmd_data_s_type *uim_cmd_data;
  uint32 slot = MMGSDI_MAX_SLOT_ID_ENUM;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (event != NULL)
  {
    if (event->evt == MMGSDI_CARD_INSERTED_EVT)
    {
      slot = event->data.card_inserted.slot;
    }
    else if (event->evt == MMGSDI_CARD_REMOVED_EVT)
    {
      slot = event->data.card_removed.slot;
    }
    else if (event->evt == MMGSDI_CARD_ERROR_EVT)
    {
      slot = event->data.card_error.slot;
    }
    else if (event->evt == MMGSDI_SUBSCRIPTION_READY_EVT)
    {
      slot = event->data.subscription_ready.app_info.slot;
    }
    else if (event->evt == MMGSDI_SESSION_CHANGED_EVT)
    {
      slot = event->data.session_changed.app_info.slot;
    }

    if (slot <= MMGSDI_SLOT_3)
    {
      MCFG_MSG_MED_2("Got card event %x on slot index %x; sending cmd", 
                      event->evt, mcfg_uim_map_mmgsdi_slot_to_slot_index(slot));
      cmd = mcfg_task_cmd_alloc(
        MCFG_TASK_CMD_UIM, sizeof(mcfg_uim_cmd_data_s_type));
      if (cmd != NULL)
      {
        uim_cmd_data = (mcfg_uim_cmd_data_s_type *) cmd->data;
        uim_cmd_data->cmd_id = MCFG_UIM_CMD_MMGSDI_EVT;
        uim_cmd_data->data.evt = *event;

        if (!mcfg_task_cmd_send(cmd))
        {
          MCFG_MSG_ERROR("Couldn't send command for MMGSDI event!");
          mcfg_task_cmd_free(cmd);
        }
      }
    }
  }
} /* mcfg_uim_mmgsdi_evt_cb() */

/*===========================================================================

  FUNCTION mcfg_uim_mmgsdi_evt_proc

  DESCRIPTION
    Handles MMGSDI events from the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_mmgsdi_evt_proc
(
  const mmgsdi_event_data_type *event
)
{
  mcfg_slot_index_type_e_type slot_index = 0;
  mcfg_sub_id_type_e_type  sub_id = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(event);

  MCFG_MSG_MED_2("Processing MMGSDI event %d for session %x", 
                 event->evt, event->session_id);
  switch (event->evt)
  {
    case MMGSDI_CARD_INSERTED_EVT:
      slot_index = mcfg_uim_map_mmgsdi_slot_to_slot_index(event->data.card_inserted.slot);
      MCFG_MSG_MED_2("Card inserted slot %d index %d", 
                   event->data.card_inserted.slot, slot_index);
      MCFG_ASSERT_COND_RET(slot_index < MCFG_NUM_OF_UIM_SLOT);

      mcfg_uim_state.slot[slot_index].card_state = MCFG_UIM_CARD_STATE_INSERTED;
#ifdef FEATURE_MCFG_REFRESH
      if(mcfg_refresh_autoselect_enabled((mcfg_refresh_index_type)slot_index))
      {
        MCFG_MSG_MED("Ignore card inserted for refresh");
      }
      else
#endif
      {
          mcfg_uim_read_iccid_if_ready(slot_index);
        }
      break;

    case MMGSDI_CARD_REMOVED_EVT:
      slot_index = mcfg_uim_map_mmgsdi_slot_to_slot_index(event->data.card_removed.slot);
      MCFG_MSG_MED_2("Card is not available slot %d index %d", 
                   event->data.card_removed.slot, slot_index);
      MCFG_ASSERT_COND_RET(slot_index < MCFG_NUM_OF_UIM_SLOT);

      mcfg_uim_state.slot[slot_index].card_state = MCFG_UIM_CARD_STATE_NO_CARD;
      memset(&mcfg_uim_state.card_info[slot_index], 0, sizeof(mcfg_uim_card_info_s_type));
      break;

    case MMGSDI_CARD_ERROR_EVT:
      slot_index = mcfg_uim_map_mmgsdi_slot_to_slot_index(event->data.card_error.slot);
      MCFG_MSG_MED_2("Card is not available slot %d index %d", 
                   event->data.card_error.slot, slot_index);
      MCFG_ASSERT_COND_RET(slot_index < MCFG_NUM_OF_UIM_SLOT);

      mcfg_uim_state.slot[slot_index].card_state = MCFG_UIM_CARD_STATE_NO_CARD;
      memset(&mcfg_uim_state.card_info[slot_index], 0, sizeof(mcfg_uim_card_info_s_type));
      break;

    case MMGSDI_SESSION_CHANGED_EVT:
      slot_index = 
        mcfg_uim_map_mmgsdi_slot_to_slot_index(event->data.session_changed.app_info.slot);
      MCFG_MSG_MED_3("mmgsdi_session_changed_evt slot %d activate %d app type %d", 
                     slot_index, event->data.session_changed.activated,
                     event->data.session_changed.app_info.app_data.app_type);

      mcfg_uim_update_app_type(event->session_id, event->data.session_changed.app_info.app_data.app_type);
      if(event->data.session_changed.activated && mcfg_uim_nv_flexmap_enabled(slot_index))
      {
        sub_id = mcfg_uim_map_session_id_to_sub_id(event->session_id);
        if (sub_id < MCFG_NUM_OF_SUB_ID)
        {
          mcfg_uim_update_mapping(slot_index, sub_id, FALSE);
        }
      }
      break;

    case MMGSDI_SUBSCRIPTION_READY_EVT:
      MCFG_MSG_MED_2("mmgsdi_subscription_ready_evt mmgsdi slot %d app %d", 
                      event->data.subscription_ready.app_info.slot,
                      event->data.subscription_ready.app_info.app_data.app_type);
      mcfg_map_print_map(FALSE);
#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
      slot_index = mcfg_uim_map_mmgsdi_slot_to_slot_index(event->data.subscription_ready.app_info.slot);
        mcfg_uim_read_imsi_if_ready(slot_index,
                                    event->data.subscription_ready.app_info.app_data.app_type);
#endif /*FEATURE_MCFG_SELECTION_BY_IMSI*/
      break;

    default:
      MCFG_MSG_ERROR_1("Unexpected event %d", event->evt);
  }
} /* mcfg_uim_mmgsdi_evt_proc() */

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
)
{
  mmgsdi_return_enum_type mmgsdi_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_MSG_MED("Starting MCFG_UIM MMGSDI initialization");
  mmgsdi_status = mmgsdi_client_id_and_evt_reg(
    mcfg_uim_mmgsdi_evt_cb, mcfg_uim_mmgsdi_rsp_cb, 0);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MCFG_MSG_ERROR_1("Couldn't register client ID with MMGSDI: status %d",
                     mmgsdi_status);
  }
} /* mcfg_uim_mmgsdi_init() */

/*===========================================================================

  FUNCTION mcfg_uim_set_cards_info_limit

  DESCRIPTION
    Reads the configured cards_info limit value from EFS if present and update the same into the global cache.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_uim_set_cards_info_limit( void )
{

  boolean read_success = FALSE;
  uint16 cards_info_limit = 0;
  mcfg_fs_status_e_type fs_ret = MCFG_FS_STATUS_OK;
  

  fs_ret = mcfg_fs_read(MCFG_CARDS_INFO_LIMIT_EFS_PATH, &cards_info_limit,
      sizeof(cards_info_limit), MCFG_FS_TYPE_EFS, MCFG_FS_SUBID_0);

  if (fs_ret != MCFG_FS_STATUS_OK)
  {
    MCFG_MSG_ERROR_2("mcfg_uim_set_cards_info_limit: efs read failed with status %d, efs_errno %d!",
                      fs_ret, efs_errno);
    MCFG_MSG_SPRINTF_1("%s", MCFG_CARDS_INFO_LIMIT_EFS_PATH);
    /*if efs is not configured set the limit as MCFG_NUM_OF_CARDS_INFO_DEFAULT */
    cards_info_limit = MCFG_NUM_OF_CARDS_INFO_DEFAULT;
  }
  MCFG_MSG_HIGH_1("mcfg_uim_set_cards_info_limit: cards_info_limit from efs %u", cards_info_limit);

  /* Ensure cards_infos_limit value should be alteast MCFG_NUM_OF_CARDS_INFO_MIN */
  if(cards_info_limit < MCFG_NUM_OF_CARDS_INFO_MIN)
  {
    cards_info_limit = MCFG_NUM_OF_CARDS_INFO_MIN;
  }

  mcfg_uim_max_num_of_cards_info = cards_info_limit;
  
  MCFG_MSG_HIGH_1("mcfg_uim_set_cards_info_limit: cards_info_limit final value - %u", mcfg_uim_max_num_of_cards_info);

  return;

}
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
)
{
  mcfg_slot_index_type_e_type i;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_MSG_MED("Starting MCFG_UIM initialization");
  memset(&mcfg_uim_state, 0, sizeof(mcfg_uim_state));

  for (i = MCFG_SLOT_INDEX_FIRST; i < MCFG_NUM_OF_UIM_SLOT; i++ )
  {
    mcfg_uim_state.slot[i].autoselect_nv = MCFG_UIM_NV_UNINIT;

    if (mcfg_uim_nv_autoselect_enabled(i))
    {
      MCFG_MSG_MED_2("NV71546 for slot %d set to %d", i, mcfg_uim_state.slot[i].autoselect_nv);
  }
}

  /* define timer for flex mapping */
  timer_def(&mcfg_uim_flexmap_timer,
            &mcfg_uim_flexmap_timer_group,
            NULL,
            0,
            mcfg_uim_flexmap_timer_cb,
            0);

  mcfg_uim_read_mapping();
 /* Configure the max_num_of_cards_info value */
 (void) mcfg_uim_set_cards_info_limit();

 /* Get the number of cards info present in EFS */
  mcfg_uim_current_num_of_cards_info = mcfg_uim_get_num_of_cards_info_from_efs();

  /*Check for max limit */
  if(mcfg_uim_current_num_of_cards_info > mcfg_uim_max_num_of_cards_info)
  {
    /*Delete few cards info to make space for new cards */
    mcfg_uim_cards_info_make_room();
  }
} /* mcfg_uim_init() */

static void mcfg_uim_read_num_of_mnc
(
  mcfg_mmgsdi_ctx_t         *p_ctx
)
{
  mmgsdi_return_enum_type mmgsdi_status;
  mmgsdi_access_type access_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(p_ctx);
  MCFG_ASSERT_COND_RET(p_ctx->sub_id < MCFG_NUM_OF_SUB_ID);
  MCFG_MSG_MED_3("Attempting to read num of mnc sub %d app %d session %x", 
                 p_ctx->sub_id, p_ctx->app_type, p_ctx->session_id);

  access_type.access_method  = MMGSDI_EF_ENUM_ACCESS;
  if (p_ctx->app_type == MMGSDI_APP_SIM)
  {
    access_type.file.file_enum = MMGSDI_GSM_AD;
  }
  else if (p_ctx->app_type == MMGSDI_APP_USIM)
  {
    access_type.file.file_enum = MMGSDI_USIM_AD;
  }
  else
  {
    //access_type.file.file_enum = MMGSDI_CSIM_IMSI_T;
    MCFG_MSG_LOW_1("mcfg_uim_read_num_of_mnc n/a for CSIM/RUIM app type %d", 
                   p_ctx->app_type);
    return;
  }

  mmgsdi_status = mmgsdi_session_read_transparent(
    p_ctx->session_id, access_type, 0, MNC_LEN_BYTE, 
    mcfg_uim_mmgsdi_rsp_cb, (mmgsdi_client_data_type)p_ctx);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MCFG_MSG_ERROR_2("Couldn't read num_of_mnc sub %d: status %d", p_ctx->sub_id, 
                     mmgsdi_status);
  }

  return;
} /* mcfg_uim_read_num_of_mnc() */

/*===========================================================================

  FUNCTION mcfg_uim_mmgsdi_read_cnf_proc

  DESCRIPTION
    Handles a read confirmation from MMGSDI. Decodes the ICCID and initiates
    the autoselection procedure.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_mmgsdi_read_cnf_proc
(
  mmgsdi_return_enum_type     status,
  const mmgsdi_read_cnf_type *cnf_data
)
{
  char card_iccid[MCFG_UIM_ICCID_STR_BUF_LEN];
#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
  mcfg_trl_carrier_mcc_mnc_s_type plmn;
  mcfg_mmgsdi_ctx_t *p_ctx;
#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */
  uint8 *data_ptr;
  uint8 slot_index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(cnf_data);
  slot_index = mcfg_uim_map_session_id_to_slot_index(cnf_data->response_header.session_id);
  MCFG_MSG_HIGH_2("mcfg_uim_mmgsdi_read_cnf_proc: status %d session_id %llx", status, 
                   cnf_data->response_header.session_id);
  MCFG_ASSERT_COND_RET(slot_index < MCFG_NUM_OF_UIM_SLOT);

  switch (cnf_data->access.file.file_enum)
  {
#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
    case MMGSDI_GSM_IMSI:
    case MMGSDI_USIM_IMSI:
      p_ctx = (mcfg_mmgsdi_ctx_t*)cnf_data->response_header.client_data;
      if (!p_ctx)
      {
        MCFG_MSG_ERROR("invalid ctx");
      }
      else if (status != MMGSDI_SUCCESS || 
          cnf_data->read_data.data_len != MMGSDI_IMSI_LEN ||
          cnf_data->read_data.data_ptr == NULL)
      {
        MCFG_MSG_ERROR_3("Couldn't read IMSI: status %d len %ld p=%p", 
                         status, cnf_data->read_data.data_len, cnf_data->read_data.data_ptr);
        mcfg_free(p_ctx);
      }
      else
      {
        memscpy(p_ctx->imsi, MMGSDI_IMSI_LEN,
                cnf_data->read_data.data_ptr, cnf_data->read_data.data_len);
        mcfg_uim_read_num_of_mnc(p_ctx);
      }
      break;

    case MMGSDI_USIM_AD:
    case MMGSDI_GSM_AD:
      p_ctx = (mcfg_mmgsdi_ctx_t*)cnf_data->response_header.client_data;
      if (!p_ctx)
      {
        MCFG_MSG_ERROR("invalid ctx");
      }
      else
      {
        if (status != MMGSDI_SUCCESS ||
               cnf_data->read_data.data_len < MNC_LEN_BYTE ||
               cnf_data->read_data.data_ptr == NULL)
        {
          MCFG_MSG_ERROR_3("Couldn't read num of mnc: status %d len %ld p=%p defaulting to 2 digits", 
                           status, cnf_data->read_data.data_len, cnf_data->read_data.data_ptr);
          p_ctx->num_mnc_digits = 2;
        }
        else
        { 
          p_ctx->num_mnc_digits = cnf_data->read_data.data_ptr[MNC_LEN_BYTE-1] & 0x0F;
        }

        if (!mcfg_uim_decode_imsi(p_ctx->imsi, &plmn, p_ctx->num_mnc_digits))
        {
          MCFG_MSG_ERROR_1("Couldn't decode IMSI: status %d", status);
        }
        else
        {
          MCFG_MSG_HIGH_2("Selecting config using IMSI MCC=%d MNC=%d", plmn.mcc, plmn.mnc);
            mcfg_sel_sw_by_plmn(mcfg_uim_state.slot[slot_index].sub_id, &plmn, MCFG_UIM_EXT_SESSION_GW);
        }

        mcfg_free(p_ctx);
      }

      break;

  case MMGSDI_CDMA_IMSI_M:
  case MMGSDI_CSIM_IMSI_M:
    if (status != MMGSDI_SUCCESS ||
        cnf_data->read_data.data_len != MMGSDI_CDMA_IMSI_LEN ||
        cnf_data->read_data.data_ptr == NULL)
    {
      MCFG_MSG_ERROR_3("Couldn't read CDMA IMSI: status %d len %lu p=%p",
                       status, cnf_data->read_data.data_len, cnf_data->read_data.data_ptr);
    }
    else if (!mcfg_uim_decode_cdma_imsi(cnf_data->read_data.data_ptr, &plmn))
    {
      MCFG_MSG_ERROR_1("Couldn't decode cdma IMSI: status %d", status);
    }
    else if (plmn.mcc != 460 || plmn.mnc != 3)
    {
      MCFG_MSG_HIGH_2("Ignore non-CT CDMA IMSI MCC=%d MNC=%d", plmn.mcc, plmn.mnc);
    }
    else
    {
      MCFG_MSG_HIGH_2("Selecting config using CDMA IMSI MCC=%d MNC=%d", plmn.mcc, plmn.mnc);
      mcfg_sel_sw_by_plmn(mcfg_uim_state.slot[slot_index].sub_id, &plmn, MCFG_UIM_EXT_SESSION_1X);
    }
    break;

#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */

  case MMGSDI_ICCID:
      if (status != MMGSDI_SUCCESS || 
          cnf_data->read_data.data_len < MCFG_UIM_ICCID_LEN ||
          cnf_data->read_data.data_ptr == NULL)
      {
        MCFG_MSG_ERROR_1("Couldn't read ICCID: status %d", status);
      }
      else if (!mcfg_uim_decode_iccid(cnf_data->read_data.data_ptr, card_iccid))
      {
        MCFG_MSG_ERROR_1("Couldn't decode ICCID: status %d", status);
      }
      else
      {
        MCFG_MSG_SPRINTF_1("ICCID %s", card_iccid); 
        mcfg_sel_sw_by_iccid(mcfg_uim_state.slot[slot_index].sub_id, card_iccid);
      }
      break;

    default:
      MCFG_MSG_ERROR_1("Unknown UIM File %x", cnf_data->access.file.file_enum);
      break;
  }

  if(cnf_data->read_data.data_ptr)
  {
    data_ptr = cnf_data->read_data.data_ptr;
    mcfg_free(data_ptr);
  }
} /* mcfg_uim_mmgsdi_read_cnf_proc() */

/*===========================================================================

  FUNCTION mcfg_uim_read_iccid_if_ready

  DESCRIPTION
    Issues a request to read the ICCID if we are ready (session is opened
    and card is inserted).

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_read_iccid_if_ready
(
  uint8 slot_index
)
{
  mmgsdi_return_enum_type mmgsdi_status;
  mmgsdi_access_type access_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET(slot_index < MCFG_NUM_OF_UIM_SLOT);

  if (!mcfg_uim_state.slot[slot_index].ready)
  {
    MCFG_MSG_MED_1("Not ready to query ICCID %d: waiting on client/session init",
                   slot_index);
  }
  else if (mcfg_uim_state.slot[slot_index].card_state != 
             MCFG_UIM_CARD_STATE_INSERTED)
  {
    MCFG_MSG_MED_1("Not ready to query ICCID %d: waiting on card inserted event",
                   slot_index);
  }
  else
  {
    MCFG_MSG_MED_1("Attempting to read ICCID %d", slot_index);
    access_type.access_method  = MMGSDI_EF_ENUM_ACCESS;
    access_type.file.file_enum = MMGSDI_ICCID;
    mmgsdi_status = mmgsdi_session_read_transparent(
      mcfg_uim_state.slot[slot_index].session_id, access_type, 0, 
      MCFG_UIM_ICCID_LEN, mcfg_uim_mmgsdi_rsp_cb, 0);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      MCFG_MSG_ERROR_2("Couldn't read ICCID %d: status %d", slot_index, 
                       mmgsdi_status);
    }
  }
} /* mcfg_uim_read_iccid_if_ready() */

#ifdef FEATURE_MCFG_SELECTION_BY_IMSI
/*===========================================================================

  FUNCTION mcfg_uim_read_imsi_if_ready

  DESCRIPTION
    Issues a request to read the imsi if we are ready (session is opened
    and card is inserted).

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_read_imsi_if_ready
(
  uint8 slot_index,
  mmgsdi_app_enum_type app_type_input
)
{
  mmgsdi_return_enum_type mmgsdi_status;
  mmgsdi_access_type access_type;
  mcfg_uim_ext_session_e_type session_index;
  uint8 sub_id;
  mmgsdi_app_enum_type app_type;
  mcfg_mmgsdi_ctx_t *p_ctx;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET(slot_index < MCFG_NUM_OF_UIM_SLOT);
  if (!mcfg_uim_state.slot[slot_index].ready)
  {
    MCFG_MSG_MED_1("Not ready to query IMSI %d: waiting on client/session init",
                   slot_index);
  }
  else if (mcfg_uim_state.slot[slot_index].card_state != 
             MCFG_UIM_CARD_STATE_INSERTED)
  {
    MCFG_MSG_MED_1("Not ready to query IMSI %d: waiting on card inserted event",
                   slot_index);
  }
  else
  {
    sub_id = mcfg_uim_map_slot_index_to_sub_id(slot_index);
    MCFG_ASSERT_COND_RET(sub_id < MCFG_NUM_OF_SUB_ID);
    MCFG_MSG_MED_3("Attempting to read IMSI slot %d sub %d app %d", 
                   slot_index, sub_id, app_type_input);

    for (session_index = MCFG_UIM_EXT_SESSION_GW; 
          session_index < MCFG_UIM_NUM_OF_EXT_SESSION; session_index++)
    {
      app_type = mcfg_uim_state.sub[sub_id].ext_app_type[session_index];
      MCFG_MSG_LOW_2("app_type %d gw1x %d", app_type, session_index);
      if (app_type_input != app_type)
      {
        continue;
      }

      access_type.access_method  = MMGSDI_EF_ENUM_ACCESS;
      if (app_type == MMGSDI_APP_SIM)
      {
        access_type.file.file_enum = MMGSDI_GSM_IMSI;
      }
      else if (app_type == MMGSDI_APP_USIM)
      {
        access_type.file.file_enum = MMGSDI_USIM_IMSI;
      }
      else if (app_type == MMGSDI_APP_RUIM)
      {
        access_type.file.file_enum = MMGSDI_CDMA_IMSI_M;
      }
      else if (app_type == MMGSDI_APP_CSIM)
      {
        access_type.file.file_enum = MMGSDI_CSIM_IMSI_M;
      }
      else
      {
        MCFG_MSG_LOW_1("mmgsdi_session_read_transparent unknown app type %d", 
                       app_type_input);
        continue;
      }

      p_ctx = mcfg_malloc(sizeof(mcfg_mmgsdi_ctx_t));
      if (!p_ctx)
      {
        MCFG_MSG_ERROR_1("fail to malloc ctx size %d", sizeof(mcfg_mmgsdi_ctx_t));
        continue;
      }

      p_ctx->session_id = mcfg_uim_state.sub[sub_id].ext_session_id[session_index];
      p_ctx->app_type = app_type;
      p_ctx->sub_id = sub_id;
      p_ctx->num_mnc_digits = 0;

      MCFG_MSG_MED_2("mmgsdi_session_read_transparent IMSI session %x gw1x %d", 
                     mcfg_uim_state.sub[sub_id].ext_session_id[session_index], 
                     session_index);
      mmgsdi_status = mmgsdi_session_read_transparent(
        p_ctx->session_id, access_type, 0, 
        (access_type.file.file_enum == MMGSDI_CDMA_IMSI_M || access_type.file.file_enum == MMGSDI_CSIM_IMSI_M) ? MMGSDI_CDMA_IMSI_LEN : MMGSDI_IMSI_LEN, 
        mcfg_uim_mmgsdi_rsp_cb, (mmgsdi_client_data_type)p_ctx);
      if (mmgsdi_status != MMGSDI_SUCCESS)
      {
        MCFG_MSG_ERROR_2("Couldn't read imsi sub %d: status %d", sub_id, 
                         mmgsdi_status);
      }
    }
  }
} /* mcfg_uim_read_imsi_if_ready() */
#endif /* FEATURE_MCFG_SELECTION_BY_IMSI */

/*===========================================================================

  FUNCTION mcfg_uim_mmgsdi_read_ef

  DESCRIPTION
    Generic function to read any EF (Elimentary File) info from MMGSDI.

  DEPENDENCIES
    None

  PARAMETERS
    session_id [in] session_id of the respective app/slot
    file_name [in] MMGSDI EF enum index
    read_data_p [out] EF data read from MMGSDI

  RETURN VALUE
    TRUE if EF info is read from MMGSDI, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_mmgsdi_read_ef
(
  mmgsdi_session_id_type session_id,
  mmgsdi_file_enum_type file_name,
  mmgsdi_data_type* read_data_p
)
{
  mmgsdi_return_enum_type mmgsdi_status = MMGSDI_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET_FALSE(read_data_p);

  read_data_p->data_len = 0;
  read_data_p->data_ptr = NULL;

  /* Pet the DOG before SIM read */
  mcfg_task_dog_hb_report();
  
  /* Read Elimentary file length from card */
  mmgsdi_status = mmgsdi_session_read_cache_file_size(session_id, file_name, &read_data_p->data_len);

  if((mmgsdi_status != MMGSDI_SUCCESS) || (read_data_p->data_len == 0))
  {
    MCFG_MSG_ERROR_3("mcfg_uim_mmgsdi_read_ef: file size read failure! ef:%u status:%d len:%lu", 
            file_name, mmgsdi_status, read_data_p->data_len);
    return FALSE;
  }

  read_data_p->data_ptr = (uint8*) mcfg_malloc(read_data_p->data_len);
  MCFG_CHECK_NULL_PTR_RET_FALSE(read_data_p->data_ptr);

  /* Pet the DOG before SIM read */
  mcfg_task_dog_hb_report();

  /* Read Elimentary file data from card */
  mmgsdi_status = mmgsdi_session_read_cache(session_id, file_name, *read_data_p);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    MCFG_MSG_ERROR_3("mcfg_uim_mmgsdi_read_ef: file read failure! ef:%u status:%d len:%lu", 
            file_name, mmgsdi_status, read_data_p->data_len);
    return FALSE;
  }

  return TRUE;
}

/*===========================================================================

  FUNCTION mcfg_uim_read_gid

  DESCRIPTION
    Reads info (GID1/GID2) from MMGSDI.

  DEPENDENCIES
    None

  PARAMETERS
    slot_index [in] slot index
    session_index [in] session index (GW/1x)
    gid_index [in] GID index (GID1/GID2)
    gid_p [out] GID value read from UIM

  RETURN VALUE
    TRUE if GID value is read from MMGSDI, FALSE otherwise

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_uim_read_gid
(
  mcfg_slot_index_type_e_type slot_index,
  mcfg_uim_ext_session_e_type session_index,
  mcfg_gid_index_e_type gid_index,
  mmgsdi_data_type* gid_p
)
{
  mcfg_sub_id_type_e_type sub_id;
  mmgsdi_session_id_type session_id;
  mmgsdi_file_enum_type file_name;
  mmgsdi_app_enum_type app_type;
  boolean status = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_ASSERT_COND_RET_FALSE(slot_index < MCFG_NUM_OF_UIM_SLOT);
  MCFG_ASSERT_COND_RET_FALSE(session_index < MCFG_UIM_NUM_OF_EXT_SESSION);
  MCFG_ASSERT_COND_RET_FALSE(gid_index < MCFG_GID_INDEX_MAX);
  MCFG_CHECK_NULL_PTR_RET_FALSE(gid_p);

  gid_p->data_len = 0;
  gid_p->data_ptr = NULL;

  if (!mcfg_uim_state.slot[slot_index].ready)
  {
    MCFG_MSG_MED_3("Not ready to query GID%d on slot%d sessionidx%d: waiting on client/session init", 
        gid_index+1, slot_index, session_index);
  }
  else if (mcfg_uim_state.slot[slot_index].card_state != MCFG_UIM_CARD_STATE_INSERTED)
  {
    MCFG_MSG_MED_3("Not ready to query GID%d on slot%d sessionidx%d: waiting on card inserted event", 
        gid_index+1, slot_index, session_index);
  }
  else
  {
    sub_id = mcfg_uim_map_slot_index_to_sub_id(slot_index);
    MCFG_ASSERT_COND_RET_FALSE(sub_id < MCFG_NUM_OF_SUB_ID);

    MCFG_MSG_MED_3("Attempting to read GID%d for slot%d sub%d", 
              gid_index+1, slot_index, sub_id);

    session_id = mcfg_uim_state.sub[sub_id].ext_session_id[session_index];
    app_type = mcfg_uim_state.sub[sub_id].ext_app_type[session_index];

    if((app_type == MMGSDI_APP_CSIM) || (app_type == MMGSDI_APP_RUIM))
    {
      file_name = ((gid_index == MCFG_GID_INDEX_1) ? MMGSDI_CSIM_GID1 : MMGSDI_CSIM_GID2);
    }
    else if(app_type == MMGSDI_APP_USIM)
    {
      file_name = ((gid_index == MCFG_GID_INDEX_1) ? MMGSDI_USIM_GID1 : MMGSDI_USIM_GID2);
    }
    else
    {
      file_name = ((gid_index == MCFG_GID_INDEX_1) ? MMGSDI_GSM_GID1 : MMGSDI_GSM_GID2);
    }

    MCFG_MSG_MED_3("mcfg_uim_read_gid: sess_idx%d app%d session %llu", 
              session_index, app_type, session_id);

    status = mcfg_uim_mmgsdi_read_ef(session_id, file_name, gid_p);
  }

  return status;
  
}

/*===========================================================================

  FUNCTION mcfg_uim_mmgsdi_rsp_cb

  DESCRIPTION
    MMGSDI command response callback. Posts command to MCFG task for
    processing.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_mmgsdi_rsp_cb
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type  *cnf_ptr
)
{
  mcfg_task_cmd_s_type *cmd;
  mcfg_uim_cmd_data_s_type *uim_cmd_data;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmd = mcfg_task_cmd_alloc(
    MCFG_TASK_CMD_UIM, sizeof(mcfg_uim_cmd_data_s_type));
  if (cmd != NULL)
  {
    uim_cmd_data = (mcfg_uim_cmd_data_s_type *) cmd->data;
    uim_cmd_data->cmd_id = MCFG_UIM_CMD_MMGSDI_RSP;
    uim_cmd_data->data.rsp.status = status;
    uim_cmd_data->data.rsp.cnf = cnf;
    if (cnf_ptr != NULL)
    {
      uim_cmd_data->data.rsp.cnf_data = *cnf_ptr;
      if (cnf == MMGSDI_READ_CNF)
      {
        uim_cmd_data->data.rsp.cnf_data.read_cnf.read_data.data_ptr = 
          mcfg_malloc(cnf_ptr->read_cnf.read_data.data_len);
        if (!uim_cmd_data->data.rsp.cnf_data.read_cnf.read_data.data_ptr)
        {
          MCFG_MSG_HIGH_1("Couldn't alloc memory for MMGSDI_READ_CNF! len=%ld", 
                           cnf_ptr->read_cnf.read_data.data_len);
          uim_cmd_data->data.rsp.cnf_data.read_cnf.read_data.data_len = 0;
          mcfg_task_cmd_free(cmd);

          return;
        }
        else
        {
          memscpy(uim_cmd_data->data.rsp.cnf_data.read_cnf.read_data.data_ptr,
                  uim_cmd_data->data.rsp.cnf_data.read_cnf.read_data.data_len,
                  cnf_ptr->read_cnf.read_data.data_ptr,
                  cnf_ptr->read_cnf.read_data.data_len);
        }
      }
      else if(cnf == MMGSDI_SESSION_OPEN_EXT_CNF && 
              cnf_ptr->session_open_ext_cnf.num_sessions > 0)
      {
        uim_cmd_data->data.rsp.cnf_data.session_open_ext_cnf.session_info_ptr = 
          mcfg_malloc(cnf_ptr->session_open_ext_cnf.num_sessions * sizeof(mmgsdi_session_open_info_type));
        if (!uim_cmd_data->data.rsp.cnf_data.session_open_ext_cnf.session_info_ptr)
        {
          MCFG_MSG_HIGH_1("Couldn't alloc memory for MMGSDI_SESSION_OPEN_EXT_CNF! %d session", 
                           cnf_ptr->session_open_ext_cnf.num_sessions);
          mcfg_task_cmd_free(cmd);
          return;
        }
        else
        {
          memscpy(uim_cmd_data->data.rsp.cnf_data.session_open_ext_cnf.session_info_ptr,
            cnf_ptr->session_open_ext_cnf.num_sessions * sizeof(mmgsdi_session_open_info_type),
            cnf_ptr->session_open_ext_cnf.session_info_ptr,
            cnf_ptr->session_open_ext_cnf.num_sessions * sizeof(mmgsdi_session_open_info_type));
        }
      }
    }

    if (!mcfg_task_cmd_send(cmd))
    {
      MCFG_MSG_ERROR("Couldn't send command for MMGSDI response!");
      mcfg_task_cmd_free(cmd);
    }
  }
} /* mcfg_uim_mmgsdi_rsp_cb() */

/*===========================================================================

  FUNCTION mcfg_uim_mmgsdi_rsp_proc

  DESCRIPTION
    Dispatches MMGSDI response data to the respective handler function.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_mmgsdi_rsp_proc
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type  *cnf_ptr
)
{
  MCFG_CHECK_NULL_PTR_RET(cnf_ptr);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_MSG_MED_1("Processing MMGSDI cnf %d", cnf);
  switch (cnf)
  {
    case MMGSDI_CLIENT_ID_AND_EVT_REG_CNF:
      mcfg_uim_mmgsdi_client_id_and_evt_reg_cnf_proc(
        status, &cnf_ptr->client_id_and_evt_reg_cnf);
      break;

    case MMGSDI_SESSION_OPEN_CNF:
      //mcfg_uim_mmgsdi_session_open_cnf_proc(status, &cnf_ptr->session_open_cnf);
      break;

    case MMGSDI_SESSION_OPEN_EXT_CNF:
      mcfg_uim_mmgsdi_session_open_ext_cnf_proc(status, &cnf_ptr->session_open_ext_cnf);
      break;

    case MMGSDI_READ_CNF:
      mcfg_uim_mmgsdi_read_cnf_proc(status, &cnf_ptr->read_cnf);
      break;

    default:
      MCFG_MSG_ERROR_1("Unexpected cnf %d", cnf);
  }
} /* mcfg_uim_mmgsdi_rsp_proc() */

/*===========================================================================

  FUNCTION mcfg_uim_mmgsdi_session_open_ext_cnf_proc

  DESCRIPTION
    Handles a session open_ext response.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void mcfg_uim_mmgsdi_session_open_ext_cnf_proc
(
  mmgsdi_return_enum_type                 status,
  const mmgsdi_session_open_ext_cnf_type *cnf_data
)
{  
  mcfg_slot_index_type_e_type slot_index = MCFG_NUM_OF_UIM_SLOT;
  mcfg_sub_id_type_e_type sub_index = MCFG_NUM_OF_SUB_ID;
  mcfg_uim_ext_session_e_type session_index;
  uint16 i = 0;
  mmgsdi_session_open_info_type *data_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  MCFG_CHECK_NULL_PTR_RET(cnf_data);
  MCFG_CHECK_NULL_PTR_RET(cnf_data->session_info_ptr);

  if (status != MMGSDI_SUCCESS)
  {
    MCFG_MSG_ERROR_1("opening MMGSDI ext sessions failed with status %d", status);
  }
  else
  {
    MCFG_MSG_MED_1("Successfully open card ext sessions with %d session(s)", 
                   cnf_data->num_sessions);
    for (i = 0; i < cnf_data->num_sessions; i++)
    {
      MCFG_MSG_MED_4("Open session index %d slot_id %d session_id %x session_type %d", i, 
                      cnf_data->session_info_ptr[i].slot_id,
                      cnf_data->session_info_ptr[i].session_id,
                      cnf_data->session_info_ptr[i].session_type);
      
      slot_index = MCFG_NUM_OF_UIM_SLOT;
      sub_index = MCFG_NUM_OF_SUB_ID;
      switch (cnf_data->session_info_ptr[i].session_type)
      {
        case MMGSDI_GW_PROV_PRI_SESSION:
          sub_index = MCFG_SUB_ID_FIRST;
          session_index = MCFG_UIM_EXT_SESSION_GW;
          break;

        case MMGSDI_GW_PROV_SEC_SESSION:
          sub_index = MCFG_SUB_ID_SECOND;
          session_index = MCFG_UIM_EXT_SESSION_GW;
          break;

        #ifdef FEATURE_MCFG_MULTISIM_TSTS_SUPPORT
        case MMGSDI_GW_PROV_TER_SESSION:
          sub_index = MCFG_SUB_ID_THIRD;
          session_index = MCFG_UIM_EXT_SESSION_GW;
          break;
        #endif

        case MMGSDI_1X_PROV_PRI_SESSION:
          sub_index = MCFG_SUB_ID_FIRST;
          session_index = MCFG_UIM_EXT_SESSION_1X;
          break;

        case MMGSDI_1X_PROV_SEC_SESSION:
          sub_index = MCFG_SUB_ID_SECOND;
          session_index = MCFG_UIM_EXT_SESSION_1X;
          break;

        #ifdef FEATURE_MCFG_MULTISIM_TSTS_SUPPORT
        case MMGSDI_1X_PROV_TER_SESSION:
          sub_index = MCFG_SUB_ID_THIRD;
          session_index = MCFG_UIM_EXT_SESSION_1X;
          break;
        #endif

        case MMGSDI_CARD_SESSION_SLOT_1:
        case MMGSDI_CARD_SESSION_SLOT_2:
        case MMGSDI_CARD_SESSION_SLOT_3:
          slot_index = mcfg_uim_map_mmgsdi_slot_to_slot_index(
            cnf_data->session_info_ptr[i].slot_id);
          break;
        //mcfg_uim_state.slot[slot_index].sub_id = 
        default:
          slot_index = MCFG_NUM_OF_UIM_SLOT;
          break;
      }
        //  mcfg_uim_map_slot_index_to_sub_id(slot_index);
      if (slot_index < MCFG_NUM_OF_UIM_SLOT)
      {
        mcfg_uim_state.slot[slot_index].session_id = 
          cnf_data->session_info_ptr[i].session_id;
        mcfg_uim_state.slot[slot_index].ready = TRUE;
        MCFG_MSG_MED_3("Slot %d session type %d id %x", 
                        slot_index,
                        cnf_data->session_info_ptr[i].session_type, 
                        mcfg_uim_state.slot[slot_index].session_id);
        mcfg_uim_read_iccid_if_ready(slot_index);
      }
      else if (sub_index < MCFG_NUM_OF_SUB_ID)
      {
        mcfg_uim_state.sub[sub_index].ext_session_id[session_index] = 
          cnf_data->session_info_ptr[i].session_id;
        mcfg_uim_state.sub[sub_index].ext_app_type[session_index]
           = cnf_data->session_info_ptr[i].app_info.app_type;
        MCFG_MSG_MED_5("Sub %d session gw/1x %d type %d id %x app_type %d", 
                        sub_index, session_index,
                        cnf_data->session_info_ptr[i].session_type, 
                        mcfg_uim_state.sub[sub_index].ext_session_id[session_index],
                        mcfg_uim_state.sub[sub_index].ext_app_type[session_index]);
      }
      else
      {
        MCFG_MSG_MED_1("Invalid session type for mmgsdislot_id %x", 
                       cnf_data->session_info_ptr[i].slot_id);
      }
    }
  }

  if(cnf_data->session_info_ptr)
  {
    data_ptr = cnf_data->session_info_ptr;
    mcfg_free(data_ptr);
  }
} /* mcfg_uim_mmgsdi_session_open_ext_cnf_proc() */

/*==============================================================================
                    PUBLIC FUNCTION DECLARATIONS FOR MODULE
==============================================================================*/
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
mmgsdi_client_id_type mcfg_uim_get_mmgsdi_client_id()
{
  return mcfg_uim_state.client_id;
}

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
)
{
  mcfg_uim_cmd_data_s_type *uim_cmd_data;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (cmd == NULL || cmd->data == NULL)
  {
    MCFG_MSG_ERROR("Unexpected NULL pointer");
  }
  else
  {
    uim_cmd_data = (mcfg_uim_cmd_data_s_type *) cmd->data;
    switch (uim_cmd_data->cmd_id)
    {
      case MCFG_UIM_CMD_MMGSDI_RSP:
        mcfg_uim_mmgsdi_rsp_proc(
          uim_cmd_data->data.rsp.status, uim_cmd_data->data.rsp.cnf,
          &uim_cmd_data->data.rsp.cnf_data);
        break;

      case MCFG_UIM_CMD_MMGSDI_EVT:
        mcfg_uim_mmgsdi_evt_proc(&uim_cmd_data->data.evt);
        break;

      case MCFG_UIM_CMD_MAPPING_TIMEOUT:
        mcfg_uim_proc_mapping_timeout();
        break;

      case MCFG_UIM_CMD_RESELECT:
        mcfg_uim_reselect(uim_cmd_data->data.sloti);
        break;

      default:
        MCFG_MSG_ERROR_1("Unexpected UIM cmd_id %d", uim_cmd_data->cmd_id);
    }
  }
} /* mcfg_uim_cmd_proc() */

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
)
{
  mcfg_task_cmd_s_type *cmd;
  mcfg_uim_cmd_data_s_type *uim_cmd_data;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmd = mcfg_task_cmd_alloc(MCFG_TASK_CMD_UIM, sizeof(mcfg_uim_cmd_data_s_type));
  if (cmd != NULL)
  {
    uim_cmd_data = (mcfg_uim_cmd_data_s_type *) cmd->data;
    uim_cmd_data->cmd_id = uim_cmd;

    if (data)
    {
      memscpy(&uim_cmd_data->data, sizeof(mcfg_uim_cmd_payload_s_type),
              data, sizeof(mcfg_uim_cmd_payload_s_type));
    }

    if (!mcfg_task_cmd_send(cmd))
    {
      MCFG_MSG_ERROR_1("Couldn't send UIM command %d", uim_cmd_data->cmd_id);
      mcfg_task_cmd_free(cmd);
    }
    else
    {
      MCFG_MSG_HIGH_1("Uim command sent %d", uim_cmd_data->cmd_id);
    }
  }
} /* mcfg_uim_cmd_send() */

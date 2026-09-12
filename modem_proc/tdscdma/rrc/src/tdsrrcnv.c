/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


           U S E R   I N T E R F A C E   N V   M O D U L E

GENERAL DESCRIPTION

  This source file provides the NV interface functions for RRC.

EXTERNALIZED FUNCTIONS
  tdsrrc_get_nv
    Get an item from NV, handle non-active items

  tdsrrc_put_nv
    Write an item to NV

  tdsrrc_replace_nv
    Replaces an item in NV

  tdsrrc_free_nv
    Free an item from NV

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2010-2012 Qualcomm Technologies INCORPORATED.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcnv.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "tdscdma_variation.h"
#include "err.h"
#include "nv.h"
#include "tdsrrcsigs.h"
#include "dog.h"
#include "tdsrrccspdb.h"
#include "nv_items.h"
#include "tdsrrcdispatcher.h"
#include "tdsrrcnv.h"
#include "tdsrrccspi.h"

// This feature is only for TDS code
//#define FEATURE_TDSCDMA_SUPPORT_NV_IN_FS

#include "string.h"
#include "fs_fcntl.h"
#include "fs_public.h"
#include "fs_sys_types.h"
#include "fs_lib.h"
#include "tdsrrclogging.h"
#include "tdsrrcdataext.h"
#include "tdsrrccmccnvdb.h"
#include "tdsCFAParser_i.h"
#include "tdsl1rrcif.h"

#define TDS_RRC_NV_CONF_FILE "/nv/item_files/conf/tdscdma_rrc.conf"
#define TDS_RRC_NV_ITEM_PATH "/nv/item_files/modem/tdscdma/rrc/"
#define TDS_RRC_NV_MMCP_THIN_UI_PATH "/nv/item_files/Thin_UI/"

/* The version number must be increased whenever
 * there is NV data structure change or 
 * a new item is added. History values,
 * 1: Initial release version
 * 2: LTE NV added
 * 3: ACQ DB size increased to 32 from 10
 * 4: ACQ DB size change to 10, keep NV_ACQ DB size as 32
 *    Add new NV TDS_RRC_NV_PRIM_FREQ_LIST
 *    Add new NV TDS_RRC_NV_INTERRAT_FEATURE_CTRL
 *    Add new NV TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST
 * 5: Add new NV TDS_RRC_NV_TDS_OPTIONAL_UECI_LIST
 * 6: Add new NV TDS_RRC_NV_SIB_SLEEP_BEFORE_SB
 * 7: Add new NV TDS_RRC_NV_BAND_SEARCH_MASK
 * 8: Add new NV TDS_RRC_NV_BPLMN_IGNOR_SIB19
 * 9: Add new NV TDS_RRC_NV_PSEUDO_SIB19
 * 10: Add new NV TDS_RRC_NV_FLOW_CONTROL_PARAMETERS
 * 11: Add new NV TDS_RRC_NV_L2T_ACQDB_CNT_THRESHOLD_REDIRECTION
 */
#define TDSRRC_NV_VERSION 11

#define RRC_NV_CONF_FILE_PERMISSIONS_COMMON 0777


/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.


===========================================================================*/

/* MST requested not write default value back to NV. Enable this feature for bring-up where default NV value 
    write-back to EFS is needed */

// #define FEATURE_TDSCDMA_RRC_NV_WRITE_BACK


/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

tdsrrcnv_data_type tdsrrcnv_data_sim[2];

static  nv_cmd_type tdsrrc_nvi;           /* nv command buffer */


/* variable to indicate support of feature list supported.
 * This variable will be set using the value of NV item 'NV_WCDMA_OPTIONAL_FEATURE_LIST_I'
 * TDS_NV_OPTIONAL_FEATURE_HSDPA 0x1<<1 
 * TDS_NV_OPTIONAL_FEATURE_HSUPA 0x1<<2
 * TDS_NV_OPTIONAL_FEATURE_MBMS 0x1<<3
 * TDS_NV_OPTIONAL_FEATURE_RX_DIVERSITY 0x1<<4
 * TDS_NV_OPTIONAL_FEATURE_PS_DATA_HO 0x1<<5
 * all other bits are spare bits - for future use

 * TDSCDMA_NV_OPTIONAL_FEATURE_ANRITSU8820_RF 0x10000
 * TDSCDMA_NV_OPTIONAL_FEATURE_MTNET_SUPPORT 0x800000
 * TDSCDMA_NV_OPTIONAL_FEATURE_CFA_SUPPORTED 0x1000000
 * TDSCDMA_NV_OPTIONAL_FEATURE_ASSERT_L1_ERR_IND 0x2000000
 * TDSCDMA_NV_OPTIONAL_FEATURE_DELAY_IDT_UNTIL_RCE_ACK 0x4000000
 */
uint32 tds_nv_optional_feature_list;
uint32 tds_nv_l2_opt_feature_list;
uint32 tds_nv_interrat_feature_ctrl;
uint8  tds_nv_delay_oos_ind_timer;
tdsnv_flow_control_value_type tdscdma_flow_control_nv; 

boolean tds_nv_l3_trace_high_flg = TRUE;
boolean tds_nv_l3_trace_med_flg  = FALSE;
boolean tds_nv_l2_trace_high_flg = TRUE;
boolean tds_nv_l2_trace_med_flg  = FALSE;

/* variable to indicate support of ueci list supported.
 * This variable will be set using the value of NV item 'NV_WCDMA_OPTIONAL_FEATURE_LIST_I'
 * TDS_NV_OPTIONAL_UECI_THIN_UI 0x1<<1
 * all other bits are spare bits - for future use
 */
uint32 tds_nv_optional_ueci_list;
tds_nv_lock_freq_cell_type tdsrrccsp_nv_lock_freq_cell;
/* variable to indicate support of MMCP THIN UI config */
boolean tds_nv_mmcp_enable_thin_ui_config;
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
uint8 tdsrrc_rlc_reestablish_nv;
#endif

tds_nv_est_fail_bar_cell_type tdsrrcrce_nv_est_fail_bar_cell;


#ifdef FEATURE_TDSCDMA_HSUPA

/* variable to store the hsupa category.
 * This variable will be set using the value of NV item 'NV_WCDMA_HSUPA_CATEGORY_I'
 */
uint16 tds_nv_hsupa_category = TDSRRC_TDS_HSUPA_DEFAULT_CATEGORY;

#endif
#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
uint8 tdsrrc_nv_t2l_ping_pang_redir_bar_time = 0;
#define TDSRRC_T2L_PING_PANG_REDIR_BAR_TIME_DEFAULT 180
#endif
/* !!!Must match the order in tds_nv_idx_type!!! */
/* Also update tdsrrcnv_get_default_value accordingly */
static char tdsrrc_nv_file_name[TDS_RRC_NV_MAX][TDS_RRC_NV_FILE_NAME_LEN_MAX] = {
   "tdsrrc_nv_version",
   "acq_db",
   "acq_list",
   "integrity_enabled",
   "ciphering_enabled",
   "fake_sec_enabled",
   "special_freq_enabled",
   "special_freq",
   "pdcp_disabled",
   "tds_rrc_version",
   "hsdpa_cat",
   "hsupa_cat",
   "tds_opt_feature_list",
   "cs_voice_over_hspa_enabled",
   "geran_feature_pack",
   "irat_nacc_support",
   "cfa_conf",
   "ignore_cell_bar_reserve_status",
   "snow3g_enabled",
   "set_srb2_act_time",
   "csfb_skip_sib11_opt",
   "freq_lock",
   "do_not_forward_page",
   "a2_power_opt",
   "fast_return_to_lte_after_csfb",
   "fast_return_to_lte_after_csfb_timer",
   "special_test_setting_enabled",
   "prim_freq_list",
   "interrat_feature_ctrl",
   "tds_l2_opt_feature_list",
   "delay_oos_ind_timer",
   "tds_opt_ueci_list",
   "sib_sleep_before_sb",
   "sib7_exp_time_factor",
   "silent_redial_opt",
   "band_search_mask",
   "freq_cell_lock",   
   "tds_rlc_reestablish_enable",
   "bplmn_ignore_sib19",
   "pseudo_sib19",
   "tdscdma_flow_control_parameters",
   "l2t_acqdb_cnt_threshold_redirection",
   "tdsrrc_hold_ps_tmr_value",
   "tds_redir_fail_bar_time",/*FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT*/
   "enable_audio_scr_mode",
   "tdsrce_est_fail_bar_cell",
   "cs_fail_bar_time"
};

/* NV items under this array are not belong to RRC but RRC read them locally.
   !!!Must match the order in tds_rrc_non_rrc_nv_idx_type!!!
   Increasing NV items in this list should NOT affect TDS_RRC_NV_VERSION (.conf generation)
*/
static char tdsrrc_non_rrc_nv_file_name[TDS_RRC_NV_MAX][TDS_RRC_NV_FILE_NAME_LEN_MAX] = {
   "enable_thin_ui_cfg",         /*!< file path /nv/item_files/Thin_UI/ */
};

static uint32 tdsrrc_nv_status_bitmask = 0; 

#define TDSRRCNV_BM_CONF_FILE_NOT_EXIST     0x00000001
#define TDSRRCNV_BM_CONF_FILE_OPEN_FAIL     0x00000002
#define TDSRRCNV_BM_NV_VER_INCORRECT        0x00000004
#define TDSRRCNV_BM_REGEN_CONF_FILE         0x00000008
#define TDSRRCNV_BM_CONF_FILE_CREATE_FAIL   0x00000010
#define TDSRRCNV_BM_NV_ITEM_PATH_INVALID    0x00000020
#define TDSRRCNV_BM_CONF_FILE_WRITE_FAIL    0x00000040
#define TDSRRCNV_BM_NV_ITEM_FOLDER_FAIL     0x00000080
#define TDSRRCNV_BM_NV_VER_WRITE_FAIL       0x00000100

static boolean tdsrrc_mtnet_nv_read = FALSE;

/* Global NVs */
uint8 tdsrrc_ignore_cell_bar_nv = 0;
boolean tdsrrc_snow3g_security_algo_supported_nv = FALSE;
uint8 tdsrrc_set_srb2_act_time_nv = 0;
uint8 tdsrrc_csfb_skip_sib11_opt_nv = 0;
uint16 tdsrrc_nv_channel_lock_item_value = 0; 
uint8 tdsrrc_do_not_forward_page_nv = 0;
uint8 tdsrrc_integrity_opt_nv = 0; /* a2_power_opt */
boolean tdsrrc_fast_return_to_lte_after_csfb_nv = FALSE;
uint16 tdsrrc_fast_return_to_lte_after_csfb_timer_nv = 0;

uint8 tdsrrc_special_settings_for_testing_nv = 0;
boolean tdsrrc_special_settings_nv_read = FALSE;

/* FR 3803 one NV setting for QMI */
tds_rrc_config_e_type tdsrrc_active_config = TDS_CONFIG_FIELD;
tds_rrc_config_e_type tdsrrc_desired_config = TDS_CONFIG_FIELD;

static const uint16 tdsrrc_nv_prim_freq_list_default[TDS_MAX_PRIM_FREQ_NBR+1] =
{24, /* number of prim freqs */
10055, 10063, 10071, 10080, 10088, 10096, 10104, 10112, 10120, 9412, 9476,
9404,  9405,  9437,  9445,  9455,  10054, 10062, 10070, 10079, 10087, 10095,
9420, 10074};

uint8 tds_nv_sib7_exp_time_factor;
boolean tds_nv_silent_redial_opt = FALSE;

/* the NV value of band_search_mask
 *  bit 0 - Band A
 *  bit 1 - N/A
 *  bit 2 - N/A
 *  bit 3 - N/A
 *  bit 4 - Band E
 *  bit 5 - Band F
 *  bit 6~15 - N/A
 */
uint16 tdsrrc_nv_band_search_mask;

boolean tdsrrc_bplmn_ignore_sib19_nv = FALSE;

tds_nv_pseudo_sib19_type  tdsrrcnv_pseudo_sib19;
/* Default value for pseudo SIB19 */
#define TDSRRCNV_PSEUDO_SIB19_UTRA_PRI 3
#define TDSRRCNV_PSEUDO_SIB19_S_PRI_SRCH1 13
#define TDSRRCNV_PSEUDO_SIB19_THR_SERV_LOW 5
#define TDSRRCNV_PSEUDO_SIB19_EUTRA_PRI 7
#define TDSRRCNV_PSEUDO_SIB19_EUTRA_MEAS_BW 2 /* tdsrrc_EUTRA_MeasurementBandwidth_mbw25 */
#define TDSRRCNV_PSEUDO_SIB19_THR_X_HIGH 5
#define TDSRRCNV_PSEUDO_SIB19_THR_X_LOW 5
#define TDSRRCNV_PSEUDO_SIB19_EUTRA_QRXLEVMIN (-59)
#define TDSRRCNV_PSEUDO_SIB19_RESERVE (0x1234)

/* successful count threshold to split lte tdscdma irat acq db entries for l2t redirection */
uint8  tdsrrc_nv_l2t_acqdb_cnt_threshold_redirection;

uint8 tdsrrc_nv_hold_ps_tmr_value = 0;

boolean tdsrrc_nv_enable_audio_scr_mode = TRUE;

#define TDSRRCNV_L2T_ACQDB_CNT_THRESHOLD_REDIRECTION_DEFAULT 10

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */



/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ? 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */


#ifdef FEATURE_TDSCDMA_HSUPA
/*=========================================================================

FUNCTION     : tdsrrc_get_hsupa_category

DESCRIPTION  : RRC returns the HSUPA Category to L1. RRC reads NV to determine 
               UE CAT (valid NV value, invalid NV value, NV not present, 
               NV reading failure)  

DEPENDENCIES : None

RETURN VALUE : HSUPA Category value 1..6

SIDE EFFECTS : None

=========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
uint16 tdsrrc_get_hsupa_category (void)
{
  return(tds_nv_hsupa_category);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

#endif


/*===========================================================================
FUNCTION RRC_GET_NV

DESCRIPTION
  Get an item from the nonvolatile memory.  Handles nonactive items by
  providing a default value.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type tdsrrc_get_nv(
  nv_items_enum_type item,        /* which item */
  nv_item_type *tdsdata_ptr          /* pointer to space for item */
)
{
  tdsrrc_nvi.tcb_ptr = rex_self();          /* notify this task when done */
  tdsrrc_nvi.sigs = TDSRRC_NV_SIG;
  tdsrrc_nvi.done_q_ptr = NULL;             /* command goes on no queue when done */

  tdsrrc_nvi.item = item;                 /* item to read */
  tdsrrc_nvi.cmd = NV_READ_F;

  /* Set up NV so that it will read the data into the correct location */
  tdsrrc_nvi.data_ptr = tdsdata_ptr;


  /* Clear the return signal, call NV, and wait for a response */
  (void) rex_clr_sigs( rex_self(), TDSRRC_NV_SIG );
  nv_cmd( &tdsrrc_nvi );
  tdsrrc_event_wait(TDSRRC_NV_SIG);

/* Handle a NV_NOTACTIVE_S or NV_FAIL_S status internally by replacing
** the random data returned with a default value of our own.  Items that
** share the same structure are lumped together in the switch.
*/

  if( tdsrrc_nvi.status == NV_NOTACTIVE_S || tdsrrc_nvi.status == NV_FAIL_S)
  {  
    tdsrrc_nvi.status = NV_DONE_S;

    switch( tdsrrc_nvi.item )
    {           /* provide our own values */
      case NV_HSDPA_COMPRESSED_MODE_ENABLED_I:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"NV Item status was %d for NV_HSDPA_COMPRESSED_MODE_ENABLED_I",tdsrrc_nvi.status); 
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Nonactive NV item 0x%x",tdsrrc_nvi.item);
        tdsrrc_nvi.status = NV_NOTACTIVE_S;
        break;
    }
  }
  else
  {
    if( tdsrrc_nvi.status != NV_DONE_S )
    {
      ERR( "NV Read Failed Item %d Code %d", tdsrrc_nvi.item, tdsrrc_nvi.status, 0 );
    }
  }

  return( tdsrrc_nvi.status );
}


/*===========================================================================
FUNCTION RRC_PUT_NV

DESCRIPTION
  Write an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the RRC task.
===========================================================================*/
nv_stat_enum_type tdsrrc_put_nv(
  nv_items_enum_type item,        /* which item */
  nv_item_type *tdsdata_ptr          /* pointer to data for item */
)
{

  tdsrrc_nvi.tcb_ptr = rex_self();        /* Notify this task when done */
  tdsrrc_nvi.sigs = TDSRRC_NV_SIG;
  tdsrrc_nvi.done_q_ptr = NULL;           /* command goes to no queue when done */

  tdsrrc_nvi.item = item;                 /* item to write */
  tdsrrc_nvi.cmd = NV_WRITE_F;

  tdsrrc_nvi.data_ptr =  tdsdata_ptr;        /* the data to write */


  /* Clear the signal, call NV, wait for it to finish */
  (void) rex_clr_sigs( rex_self(), TDSRRC_NV_SIG );
  nv_cmd( &tdsrrc_nvi );
  tdsrrc_event_wait(TDSRRC_NV_SIG);
  
  if( tdsrrc_nvi.status != NV_DONE_S )
  {
    ERR( "NV Write Failed Item %d Code %d", tdsrrc_nvi.item, tdsrrc_nvi.status, 0 );
  }

  return( tdsrrc_nvi.status );

}

/*===========================================================================
FUNCTION tdsrrcnv_nv2config

DESCRIPTION
  Utility function to convert NV69731 to tds_rrc_config_e_type

RETURN VALUE
  tds_rrc_config_e_type

DEPENDENCIES
  None.

===========================================================================*/
tds_rrc_config_e_type tdsrrcnv_nv2config(uint8 nv_value)
{
   tds_rrc_config_e_type ret_config = TDS_CONFIG_FIELD;

   switch (nv_value)
   {
      case 0:
         ret_config = TDS_CONFIG_USER;
         break;
         
      case 1:
         ret_config = TDS_CONFIG_LAB;
         break;
         
      case 2:
         ret_config = TDS_CONFIG_FIELD;
         break;
         
      default:
         TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid NV69731 value %d,use TDS_CONFIG_FIELD for config",nv_value);
         ret_config = TDS_CONFIG_FIELD;
         break;
   }

   return ret_config;
}

/*===========================================================================
FUNCTION tesrrcnv_config2nv

DESCRIPTION
  Utility function to convert tds_rrc_config_e_type to NV69731 value

RETURN VALUE
  NV69731 value

DEPENDENCIES
  None.

===========================================================================*/
uint8 tdsrrcnv_config2nv(tds_rrc_config_e_type config)
{
   uint8 ret_nv = 2;

   switch (config)
   {
      case TDS_CONFIG_USER:
         ret_nv = 0;
         break;
         
      case TDS_CONFIG_LAB:
         ret_nv = 1;
         break;
         
      case TDS_CONFIG_FIELD:
         ret_nv = 2;
         break;

      default:
         TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid NV69731 config value %d,use 2 for NV config",config);
         ret_nv = 2;
         break;     
   }

   return ret_nv;
}

/*===========================================================================

FUNCTION          tdsrrcnv_read_nv_forbid_cells

DESCRIPTION       This function reads the forbidden cells from NAS NV/EFS file.
           
DEPENDENCIES
                  None

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.

NOTE
                  None.
===========================================================================*/
void tdsrrcnv_read_nv_forbid_cells(sys_modem_as_id_e_type as_id)
{
  /* Each RAT(L/T/W/G) has 20 forbidden cells at most. */
  tdsrrc_efs_forbid_cell_type forbid_cells[RRC_NV_NUM_FORBID_CELLS << 2];
  struct fs_stat stat_buf;
  uint32 i = 0, j = 0, valid_cell_count = 0;
  
  mcfg_fs_status_e_type   status_forbidden_cell;
  mcfg_fs_sub_id_e_type   sub_id;

  tdsrrcnv_data_sim[as_id].forbid_cells.num_forbid_cell = 0;

  if(as_id == SYS_MODEM_AS_ID_1)
  {
    sub_id = MCFG_FS_SUBID_0;
  }
  else
  {
    /* for this NV, we read it from MCFG_FS_SUBID_0 for both SUBs. */
    sub_id = MCFG_FS_SUBID_0;
  }
   
  memset(&stat_buf, 0, sizeof(struct fs_stat));
  if ((mcfg_fs_stat("/nv/reg_files/modem/forbidden_cell_list",
                    &stat_buf, MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)sub_id) != MCFG_FS_STATUS_OK)
      || (stat_buf.st_size == 0))
  { 
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid file of forbidden_cell_list, as_id: %d", as_id);
    return;
  }
  
  if ((stat_buf.st_size % sizeof(tdsrrc_efs_forbid_cell_type) != 0)|| (stat_buf.st_size > sizeof(forbid_cells)))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid length of file forbidden_cell_list, as_id: %d", as_id);
    return;
  }
  
  status_forbidden_cell = mcfg_fs_read("/nv/reg_files/modem/forbidden_cell_list",
                          forbid_cells, 
                          stat_buf.st_size,
                          MCFG_FS_TYPE_EFS,
                          sub_id);

  if (status_forbidden_cell == MCFG_FS_STATUS_OK)
  {
    valid_cell_count = stat_buf.st_size / sizeof(tdsrrc_efs_forbid_cell_type);
      
    /* each RAT(L/T/W/G) has 20 cells at most, so multiply 4 here. */
    for(i = 0; (i < valid_cell_count && i < (RRC_NV_NUM_FORBID_CELLS << 2) && j < RRC_NV_NUM_FORBID_CELLS); i++)
    {
      if(forbid_cells[i].rat == SYS_SYS_MODE_TDS)
      {
        tdsrrcnv_data_sim[as_id].forbid_cells.forbid_cell[j].sys_plmn_id = forbid_cells[i].irat_sys_plmn_id;
        tdsrrcnv_data_sim[as_id].forbid_cells.forbid_cell[j].cell_id = forbid_cells[i].cell_id;
        tdsrrcnv_data_sim[as_id].forbid_cells.forbid_cell[j].lac = forbid_cells[i].lac;
        j++;
      }
    }
    tdsrrcnv_data_sim[as_id].forbid_cells.num_forbid_cell = j;

    for(j = 0; j < tdsrrcnv_data_sim[as_id].forbid_cells.num_forbid_cell; j++)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"forbid_cells from NV: bytes of plmn_id: 0x%x,0x%x,0x%x", 
                    tdsrrcnv_data_sim[as_id].forbid_cells.forbid_cell[j].sys_plmn_id.identity[0],
                    tdsrrcnv_data_sim[as_id].forbid_cells.forbid_cell[j].sys_plmn_id.identity[1],
                    tdsrrcnv_data_sim[as_id].forbid_cells.forbid_cell[j].sys_plmn_id.identity[2]);
                    
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"forbid_cells from NV: cell_id: 0x%x, lac: 0x%x, as_id: %d.", 
                    tdsrrcnv_data_sim[as_id].forbid_cells.forbid_cell[j].cell_id, 
                    tdsrrcnv_data_sim[as_id].forbid_cells.forbid_cell[j].lac, as_id);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"failed to read file forbidden_cell_list, as_id: %d", as_id);
  }
}

/*===========================================================================

FUNCTION          tdsrrcnv_read_nv_refresh

DESCRIPTION       This function reads all the NV/EFS after refresh or sub change.
           
DEPENDENCIES
                  None

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.

NOTE
                  None.
===========================================================================*/
void tdsrrcnv_read_nv_refresh(sys_modem_as_id_e_type as_id)
{
  //current only support NV refresh for forbidden cells.
  tdsrrcnv_read_nv_forbid_cells(as_id);  
}

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/*===========================================================================

FUNCTION tdsrrcnv_read_nv_tds_rlc_reestablish

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_RLC_REESTABLISH and stores it in 
  rrc global variable 'tdsrrc_rlc_reestablish_nv'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_tds_rlc_reestablish
(
  void
)
{
  uint8 local_nv_rlc_reestablish = TDS_NV_RLC_REESTABLISH_NONE_SUPPORTED;

  tdsrrc_rlc_reestablish_nv = TDS_NV_RLC_REESTABLISH_NONE_SUPPORTED;

  /*read optional ueci list from NV*/
  if (tdsrrcnv_efs_read(TDS_RRC_NV_RLC_REESTABLISH, 
                        (byte *) &local_nv_rlc_reestablish,
                        sizeof(local_nv_rlc_reestablish)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_RLC_REESTABLISH is 0x%x", local_nv_rlc_reestablish);
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_RLC_REESTABLISH,
                                   (byte *) &local_nv_rlc_reestablish,
                                   sizeof(local_nv_rlc_reestablish),
                                   TRUE))
    {
       /* Mismatch of NV item */
       local_nv_rlc_reestablish = TDS_NV_DEFAULT_RLC_REESTABLISH_BITMASK;
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_RLC_REESTABLISH item error. Setting to default: %d", local_nv_rlc_reestablish);
    }

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read failed for TDS_RRC_NV_RLC_REESTABLISH, set default to TDS_NV_DEFAULT_RLC_REESTABLISH_BITMASK");
  }
  
  
  /* Set stored nv variable value to set rrc global variable */
  tdsrrc_rlc_reestablish_nv = local_nv_rlc_reestablish;
}
#endif   
/*===========================================================================

FUNCTION tdsrrcnv_read_cmcc_test_nv_value

DESCRIPTION
  Retrieve CMCC test NV for lab/field from pre-defined value.

DEPENDENCIES
  TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING must have been visited.

RETURN VALUE
  Actual size read. 0 indicates an error.
  
  *size_ptr is set to the actual number of bytes that were copied into the
  buffer pointed by data_ptr. If the read failed, *size_ptr = 0

SIDE EFFECTS
  None.


==========================================================================*/
uint32 tdsrrcnv_read_cmcc_test_nv_value
(
        uint8 test_cfg_idx,
            /* passing the cmcc special test setting cfg in use.
            */
        tds_rrc_nv_idx_type          nv_item,
            /* passing the nv_item to read.
            */
        byte                        *data_ptr,
            /* Pointer to a data buffer where to store the data that is
            ** associated with the EFS item.
            */
        uint32                       size
            /* Size of the data_ptr buffer 
            */
)
{
   uint32      actual_size = 0;
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
   mcfg_fs_status_e_type  status;
#else
   int         fd;
#endif
   char        file_name[TDS_RRC_NV_FULL_PATH_LEN_MAX];
   uint32      nv_value = 0;
 
   if (!data_ptr || !size)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid EFS NV buffer data");
      return actual_size;
   }

   if (test_cfg_idx >= sizeof(tds_nv_cmcc_test_setting)/sizeof(tds_nv_cmcc_test_setting[0]))
   {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid special test setting cfg idx");
	 return actual_size;
   }

   if (nv_item == TDS_RRC_NV_MAX)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid EFS NV item");
      return actual_size;
   }

   switch (nv_item)
   {
      case TDS_RRC_NV_RRC_INTEGRITY_ENABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_INTEGRITY_ENABLED];
         }
         break;

      case TDS_RRC_NV_RRC_CIPHERING_ENABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_CIPHERING_ENABLED];
         }
         break;
         
      case TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_FAKE_SECURITY_ENABLED];
         }
         break;

      case TDS_RRC_NV_SPECIAL_FREQ_ENABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_SPECIAL_FREQ_ENABLED];
         }
         break;

      case TDS_RRC_NV_SPECIAL_FREQ:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint32 *) data_ptr) = (uint32) tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_SPECIAL_FREQ];
         }
         break;

      case TDS_RRC_NV_RRC_PDCP_DISABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_PDCP_DISABLED];
         }
         break;

#if 0
      case TDS_RRC_NV_RRC_VERSION:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
	   /* If NV69731=2 (FT), NV66017(TDS RRC Version) will not be controlled by NV69731 */
	   if (test_cfg_idx == 1) 
	   {
	     TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDS RRC Version not controlled by NV69731 for FT");
	     actual_size = 0;
	   }
	   else
	   {
            *((uint32 *) data_ptr) = (uint32) tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_VERSION];
         }
         }
         break;
#endif

      case TDS_RRC_NV_HSDPA_CAT:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = (uint8) tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_HSDPA_CAT];
         }
         break;

      case TDS_RRC_NV_HSUPA_CAT:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = (uint8) tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_HSUPA_CAT];
         }
         break;

      case TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint32 *) data_ptr) = (uint32) tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_TDS_OPTIONAL_FEATURE_LIST];

			/* for CMCC FT/mass product, leave the predefined user pref bits for OEM control */
            if (test_cfg_idx == 1)
            {
              /* read NV to get actual value */
              memset((void *)file_name, '\0', sizeof(file_name));
              snprintf(file_name, TDS_RRC_NV_FULL_PATH_LEN_MAX, "%s%s", TDS_RRC_NV_ITEM_PATH, tdsrrc_nv_file_name[nv_item]);
 
#ifdef FEATURE_TDSCDMA_EFS_NEW_API          
              status = mcfg_fs_read(file_name, (void*)&nv_value, size, MCFG_FS_TYPE_EFS, tdsrrc_get_efs_storage_sub_id());
          
              if (status != MCFG_FS_STATUS_OK)
#else
              fd = efs_get(file_name, (void*)&nv_value, size);
          
              if ((fd < 0) || ((uint32) fd > size))
#endif
              {      
                 /* set to default value since read error happened */
  			     TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS Optional Feature List for CMCC FT, user NV not set, apply default 0x%x,not modify user pref bits",(uint32)tds_nv_cmcc_test_setting[test_cfg_idx][TDS_RRC_NV_CMCC_TEST_SETTING_TDS_OPTIONAL_FEATURE_LIST]);
              }
              else
              {
                 /* Set predefined user pref bits in return data_ptr value to the corresponding bits in actual NV value */
  			   *((uint32 *) data_ptr) |= (uint32) TDS_RRC_NV_CMCC_TEST_SETTING_TDS_OPTIONAL_FEATURE_LIST_USER_PREF_BITS;
                 *((uint32 *) data_ptr) &= ((uint32)nv_value) | (~((uint32) TDS_RRC_NV_CMCC_TEST_SETTING_TDS_OPTIONAL_FEATURE_LIST_USER_PREF_BITS));
                 TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDS Optional Feature List for CMCC FT,modify user pref bits 0x%x by NV value 0x%x",(uint32) TDS_RRC_NV_CMCC_TEST_SETTING_TDS_OPTIONAL_FEATURE_LIST_USER_PREF_BITS,(uint32)nv_value);
               }
            }
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS Optional Feature List is 0x%x",*((uint32 *) data_ptr));
         }
         break;

      case TDS_RRC_NV_SILENT_REDIAL_OPT:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"CMCC test NV invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = FALSE;
         }
         break;
         /* TBD to be added TDS_RRC_NV_PRIM_FREQ_LIST */

      default:
         break;
   }

   return actual_size;
}

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_tdscdma_optional_ueci_list

DESCRIPTION
  This  function gets the NV item NV_WCDMA_OPTIONAL_FEATURE_LIST_I and stores it in 
  rrc global variable 'tds_nv_optional_ueci_list'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_tdscdma_optional_ueci_list
(
  void
)
{
  uint32 local_nv_opt_ueci_list = TDS_NV_OPTIONAL_UECI_NONE_SUPPORTED;

  tds_nv_optional_ueci_list = TDS_NV_OPTIONAL_UECI_NONE_SUPPORTED;

  /*read optional ueci list from NV*/
  if (tdsrrcnv_efs_read(TDS_RRC_NV_TDS_OPTIONAL_UECI_LIST, 
                        (byte *) &local_nv_opt_ueci_list,
                        sizeof(local_nv_opt_ueci_list)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_TDS_OPTIONAL_UECI_LIST is 0x%x", local_nv_opt_ueci_list);
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_TDS_OPTIONAL_UECI_LIST,
                                   (byte *) &local_nv_opt_ueci_list,
                                   sizeof(local_nv_opt_ueci_list),
                                   TRUE))
    {
       /* Mismatch of NV item */
       local_nv_opt_ueci_list = TDS_NV_OPTIONAL_UECI_DEFAULT_SUPPORT_BITMASK;
	     local_nv_opt_ueci_list |= TDS_NV_OPTIONAL_UECI_L2L3_TRACE_ENABLE_BITMASK;

       TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_TDS_OPTIONAL_UECI_LIST item error. Setting to default: %d", local_nv_opt_ueci_list);
    }

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read failed for TDS_RRC_NV_TDS_OPTIONAL_UECI_LIST, set default to TDS_NV_OPTIONAL_UECI_DEFAULT_SUPPORT_BITMASK");
  }
  
  /* TDS_NV_OPTIONAL_UECI_DEVICE_TYPE Original feature dependency logic:
     #if(defined(FEATURE_THIN_UI) && (!defined(FEATURE_TDSCDMA_CPC_DRX)))
  */
#ifdef FEATURE_TDSCDMA_CPC_DRX
  local_nv_opt_ueci_list &= ~TDS_NV_OPTIONAL_UECI_DEVICE_TYPE;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"FEATURE_TDSCDMA_CPC_DRX enabled, remove TDS_NV_OPTIONAL_UECI_DEVICE_TYPE from tds_nv_optional_ueci_list");
#endif
  
  /* Set stored nv variable value to set rrc global variable */
  tds_nv_optional_ueci_list = local_nv_opt_ueci_list;

  tds_nv_l3_trace_high_flg = (TDSCHECK_OPTIONAL_UECI_ENABLE_L3_TRACE_HIGH) ? TRUE : FALSE;
  tds_nv_l3_trace_med_flg  = (TDSCHECK_OPTIONAL_UECI_ENABLE_L3_TRACE_MED) ? TRUE : FALSE;
  tds_nv_l2_trace_high_flg = (TDSCHECK_OPTIONAL_UECI_ENABLE_L2_TRACE_HIGH) ? TRUE : FALSE;
  tds_nv_l2_trace_med_flg  = (TDSCHECK_OPTIONAL_UECI_ENABLE_L2_TRACE_MED) ? TRUE : FALSE;
}

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_tdscdma_optional_feature_list

DESCRIPTION
  This  function gets the NV item NV_WCDMA_OPTIONAL_FEATURE_LIST_I and stores it in 
  rrc global variable 'tds_nv_optional_feature_list'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_tdscdma_optional_feature_list
(
  void
)
{

  uint32 local_nv_opt_feature_list = 0;

  tds_nv_optional_feature_list = TDS_NV_OPTIONAL_FEATURE_NONE_SUPPORTED;

  /*read tds rrc release indicator from NV*/
  if (tdsrrcnv_efs_read(TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST, 
                        (byte *) &local_nv_opt_feature_list,
                        sizeof(local_nv_opt_feature_list)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST is 0x%x",local_nv_opt_feature_list);
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST,
                                   (byte *) &local_nv_opt_feature_list,
                                   sizeof(local_nv_opt_feature_list),
                                   TRUE))
    {
       /* Mismatch of NV item */
       local_nv_opt_feature_list = TDS_NV_OPTIONAL_FEATURE_ALL_SUPPORTED;
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST item error. Setting to default: %d",local_nv_opt_feature_list);
    }

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read failed for TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST,set default all enabled");
  }
  
  /* use stored nv variable value to set rrc global variable */
  tds_nv_optional_feature_list = local_nv_opt_feature_list;

/* if features are not defined then update the bit-mask accordingly */

#ifndef FEATURE_TDSCDMA_HSUPA
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"FEATURE_TDSCDMA_HSUPA not defined. NV...OPTIONAL_FEATURE_LIST_I: 0x%x ",local_nv_opt_feature_list);
  tds_nv_optional_feature_list = local_nv_opt_feature_list & (~TDS_NV_OPTIONAL_FEATURE_HSUPA);
#else
#endif

  tdsrrc_mtnet_nv_read = TRUE;

  /* read TDS_RRC_NV_DELAY_OOS_IND_TIMER, since it is associated with CMCC_RLF_SKIP_SVCREQ_BIT 
   in TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST
   */
  if (tdsrrcnv_efs_read(TDS_RRC_NV_DELAY_OOS_IND_TIMER, 
                       (byte *) &tds_nv_delay_oos_ind_timer,
		    		   sizeof(tds_nv_delay_oos_ind_timer)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Successfully read TDS_RRC_NV_DELAY_OOS_IND_TIMER");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read TDS_RRC_NV_DELAY_OOS_IND_TIMER failed,set default value");
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_DELAY_OOS_IND_TIMER,
                                   (byte *) &tds_nv_delay_oos_ind_timer,
                                   sizeof(tds_nv_delay_oos_ind_timer),
								   TRUE))
    {
	   /* Mismatch of NV item */
	   tds_nv_delay_oos_ind_timer = 0;
	   TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot get TDS_RRC_NV_DELAY_OOS_IND_TIMER default value");
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tds_nv_delay_oos_ind_timer: %d seconds,CR430034",tds_nv_delay_oos_ind_timer);

  if (tds_nv_delay_oos_ind_timer > 7)
  {
    tds_nv_delay_oos_ind_timer = 7;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"tds_nv_delay_oos_ind_timer is capped at 7 seconds");
  }

  /* CR694274 remove this delay due to NAS sending frequent SERVICE_REQ 
   * and stop mode requests on SGLTE and DSDS.
   * Also SERVICE_REQ can include time as well. 
   */
  tds_nv_delay_oos_ind_timer = 0;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CR694274: tds_nv_delay_oos_ind_timer hard-coded to %d seconds",tds_nv_delay_oos_ind_timer);

}  


/*===========================================================================

FUNCTION tdsrrcnv_read_nv_special_settings_for_testing

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING and stores it in 
  rrc global variable 'tdsrrc_special_settings_for_testing_nv'

  !!!THIS FUNCTION SHOULD ONLY BE CALLED ONCE AND IN tds_rrc_task()!!!

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_special_settings_for_testing
(
  void
)
{
  uint8 local_nv_special_settings_for_testing = 0;

  /*read tds rrc release indicator from NV*/
  if (tdsrrcnv_efs_read(TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING, 
                        (byte *) &local_nv_special_settings_for_testing,
                        sizeof(local_nv_special_settings_for_testing)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING is 0x%x",local_nv_special_settings_for_testing);
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING,
                                   (byte *) &local_nv_special_settings_for_testing,
                                   sizeof(local_nv_special_settings_for_testing),
                                   TRUE))
    {
       /* Mismatch of NV item */
       local_nv_special_settings_for_testing = 2;
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read failed for TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING,set default value 2");
    }

    TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING item error. Setting to default: %d",local_nv_special_settings_for_testing);
  }
  
  /* use stored nv variable value to set rrc global variable */
  tdsrrc_special_settings_for_testing_nv = local_nv_special_settings_for_testing;

  tdsrrc_special_settings_nv_read = TRUE;

  tdsrrc_active_config = tdsrrcnv_nv2config(tdsrrc_special_settings_for_testing_nv);
  tdsrrc_desired_config = tdsrrc_active_config;
  
}

/*====================================================================
FUNCTION: tdsrrc_get_mtnet_nv_status

DESCRIPTION:
  This function returns true of NV status for MTNet support.

DEPENDENCIES:
  None

RETURN VALUE:
  TDSRRC_MTNET_NV_OFF = 0,
  TDSRRC_MTNET_NV_ON  = 1,
  TDSRRC_MTNET_NV_INVALID = -1

SIDE EFFECTS:
====================================================================*/
tdsrrc_mtnet_nv_status_e_type tdsrrc_get_mtnet_nv_status
(
  void
)
{
  if(!tdsrrc_mtnet_nv_read)
  {
    return TDSRRC_MTNET_NV_INVALID;
  }

  return (TDSCHECK_OPTIONAL_OPTIONAL_FEATURE_MTNET_SUPPORT)? TDSRRC_MTNET_NV_ON:TDSRRC_MTNET_NV_OFF;
  
}



/*====================================================================
FUNCTION: tdsrrc_get_special_settings_for_testing

DESCRIPTION:
  This function returns true of NV status for special_settings_for_testing.

DEPENDENCIES:
  None

RETURN VALUE:
    TDSRRC_SPECIAL_SETTINGS_FOR_TESTING_NV_OFF = 0,
    TDSRRC_SPECIAL_SETTINGS_FOR_TESTING_NV_ON  = 1,
    TDSRRC_SPECIAL_SETTINGS_FOR_TESTING_NV_INVALID = -1

Note: This function is obsolete after new function tdsrrc_get_special_test_settings_config below is used. This
        function is maintained only for backward compatibility and compilation purpose.

SIDE EFFECTS:
====================================================================*/
tdsrrc_special_test_settings_status_e_type tdsrrc_get_special_settings_for_testing
(
  void
)
{
  if(!tdsrrc_special_settings_nv_read)
  {
    return TDSRRC_SPECIAL_SETTINGS_FOR_TESTING_NV_INVALID;
  }

  return (tdsrrc_special_settings_for_testing_nv==1)? TDSRRC_SPECIAL_SETTINGS_FOR_TESTING_NV_ON:TDSRRC_SPECIAL_SETTINGS_FOR_TESTING_NV_OFF;

}

/*====================================================================
FUNCTION: tdsrrc_get_special_test_settings_config

DESCRIPTION:
  This function returns the config value of NV69731 for CMCC special_settings_for_testing.

DEPENDENCIES:
  None

RETURN VALUE:
    TDSRRC_SPECIAL_TEST_SETTINGS_CONFIG_ENGINEERING = 0,
    TDSRRC_SPECIAL_TEST_SETTINGS_CONFIG_CMCC_LAB = 1,
    TDSRRC_SPECIAL_TEST_SETTINGS_CONFIG_CMCC_FIELD = 2,
    TDSRRC_SPECIAL_TEST_SETTINGS_CONFIG_INVALID = -1

SIDE EFFECTS:
====================================================================*/
tdsrrc_special_test_settings_config_e_type tdsrrc_get_special_test_settings_config
(
  void
)
{
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"special_settings_nv_read: %d,special_settings_for_testing_nv: %d",tdsrrc_special_settings_nv_read,tdsrrc_special_settings_for_testing_nv);
  if ( (!tdsrrc_special_settings_nv_read) ||
  	   (tdsrrc_special_settings_for_testing_nv > 2) )
  {
    return TDSRRC_SPECIAL_TEST_SETTINGS_CONFIG_INVALID;
  }

  return (tdsrrc_special_test_settings_config_e_type)tdsrrc_special_settings_for_testing_nv;
}

/*====================================================================
FUNCTION: tdsrrc_get_l2_opt_nv

DESCRIPTION:
  This function returns the value of NV item TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST. It is a utility function
  for L2 to access this NV item so L2 does not need a separate NV handling utility.

DEPENDENCIES:
  None

RETURN VALUE:
  UINT32

SIDE EFFECTS:
====================================================================*/

uint32 tdsrrc_get_l2_opt_nv
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS L2 get tds_nv_l2_opt_feature_list value: 0x%x",tds_nv_l2_opt_feature_list);
  return tds_nv_l2_opt_feature_list;
}


/*===========================================================================

FUNCTION tdsrrc_get_rlc_nv_list

DESCRIPTION
  It is a utility function for L2 to access this NV item
  so L2 does not need a separate NV handling utility.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrc_rlc_nv_list_type


SIDE EFFECTS

  None

===========================================================================*/
tdsrrc_rlc_nv_list_type tdsrrc_get_rlc_nv_list(void)
{
  tdsrrc_rlc_nv_list_type local_rlc_nv;
  
  /*get RRC NV version*/
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9)
  {
    local_rlc_nv.rrc_version = TDSRRC_REL_VERSION_9;
  }
  else if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
  {
    local_rlc_nv.rrc_version = TDSRRC_REL_VERSION_8;
  }
  else if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
    local_rlc_nv.rrc_version = TDSRRC_REL_VERSION_7;
  }
  else if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
  {
    local_rlc_nv.rrc_version = TDSRRC_REL_VERSION_6;
  }
  else if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    local_rlc_nv.rrc_version = TDSRRC_REL_VERSION_5;
  }
  else
  {
    local_rlc_nv.rrc_version = TDSRRC_REL_VERSION_4;
  }

  /*get featrue list*/
  local_rlc_nv.l2_opt_bitmask = tds_nv_l2_opt_feature_list;
  //TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDS L2 get tdsrrc_get_rlc_nv_list: version:%d featurelist: 0x%x",local_rlc_nv.rrc_version,local_rlc_nv.l2_opt_bitmask);

  //Copying the flow control NVs.	
  local_rlc_nv.l2_flow_control_nv.tdsrlc_fc_min_win_size = tdscdma_flow_control_nv.tdsrlc_fc_min_win_size;
  local_rlc_nv.l2_flow_control_nv.tdsrlc_fc_num_of_steps = tdscdma_flow_control_nv.tdsrlc_fc_num_of_steps;
  memscpy(&local_rlc_nv.l2_flow_control_nv.tdsfw_dl_fc_rtbs_value, (8*sizeof(uint8)), &tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value, (8*sizeof(uint8)));
  
  return local_rlc_nv;

}

#ifdef FEATURE_TDSCDMA_HSUPA
/*===========================================================================

FUNCTION tdsrrcnv_read_nv_tds_hsupa_category

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_HSUPA_CAT and stores it in 
  rrc global variable 'tds_nv_hsupa_category'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_tds_hsupa_category
(
  void
)
{
  uint8 local_hsupa_category = 0;

  /*read tds rrc release indicator from NV*/
  if (tdsrrcnv_efs_read(TDS_RRC_NV_HSUPA_CAT, 
                        (byte *) &local_hsupa_category,
                        sizeof(local_hsupa_category)))
  {
    /* check for valid hsupa category in NV item - 
       this validity may differ from target to target. Need to take care of this check
       later by adding appropriate check for new target */
    if ((local_hsupa_category < 1) || (local_hsupa_category > 6))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: TDS_RRC_NV_HSUPA_CAT contains invalid category(%d). set to default",local_hsupa_category);
      local_hsupa_category = TDSRRC_TDS_HSUPA_DEFAULT_CATEGORY;
    }
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL: TDS_RRC_NV_HSUPA_CAT is set to %d",local_hsupa_category);
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_HSUPA_CAT,
                                   (byte *) &local_hsupa_category,
                                   sizeof(local_hsupa_category),
                                   TRUE))
    {
       /* Mismatch of NV item */
       local_hsupa_category = TDSRRC_TDS_HSUPA_DEFAULT_CATEGORY;
       
    }

    TDSRRC_MSG1(MSG_LEGACY_ERROR,"No NV setting for TDSRRC_TDS_HSUPA_DEFAULT_CATEGORY,Set to default: %d",local_hsupa_category);
  }
  
  /* use stored nv variable value to set rrc global variable */
  tds_nv_hsupa_category = local_hsupa_category;

  return;
}  

#endif /* FEATURE_TDSCDMA_HSUPA */

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_tds_l2_opt_feature_list

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST and stores it in 
  rrc global variable 'tds_nv_l2_opt_feature_list'. L2 will call API tdsrrc_get_l2_opt_nv() to access
  the value of this NV item. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_tds_l2_opt_feature_list
(
  void
)
{
  if (tdsrrcnv_efs_read(TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST, 
                        (byte *) &tds_nv_l2_opt_feature_list,
                        sizeof(tds_nv_l2_opt_feature_list)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Successfully read TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST failed,set default value");
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST,
                                   (byte *) &tds_nv_l2_opt_feature_list,
                                   sizeof(tds_nv_l2_opt_feature_list),
                                   TRUE))
    {
       /* Mismatch of NV item */
       tds_nv_l2_opt_feature_list = 0;
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot get TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST default value,diable all bits");
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tds_nv_l2_opt_feature_list: 0x%x",tds_nv_l2_opt_feature_list);
}  

/*===========================================================================

FUNCTION TDSRLC_GET_FLOW_CONTROL_NV_ITEMS

DESCRIPTION
  The function get read the NV items using the RRC NV read api.
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/


void tdsrrcnv_read_flow_control_nv_items()
{
   
	
   /*read Flow control data from NV*/
   if (tdsrrcnv_efs_read(TDS_RRC_NV_FLOW_CONTROL_PARAMETERS, 
                         (byte *) &tdscdma_flow_control_nv,
                         sizeof(tdsnv_flow_control_value_type)))
   {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDS_RRC_NV_FLOW_CONTROL_PARAMETERS is read successfully");
      if(tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[5] > 63) 
      ERR_FATAL("tdsrrcnv_flow control reading wrong values",0,0,0);
	  
   }

   else
   	{
	   TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDS_RRC_NV_FLOW_CONTROL_PARAMETERS is not read successfully win size %d, steps %d and Test dir %d, so using default values", 
           tdscdma_flow_control_nv.tdsrlc_fc_min_win_size, tdscdma_flow_control_nv.tdsrlc_fc_num_of_steps,  tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[6]);
	   tdscdma_flow_control_nv.tdsrlc_fc_min_win_size= 20;
	   tdscdma_flow_control_nv.tdsrlc_fc_num_of_steps=10;
	   tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[0]= 18;
	   tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[1]= 38;
	   tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[2]= 49;
	   tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[3]= 55;
	   tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[4]= 59;
	   tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[5]= 63;
	   tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[6]= 2;
	   tdscdma_flow_control_nv.tdsfw_dl_fc_rtbs_value[7]= 0;
	   
   	}

   

}


/*===========================================================================

FUNCTION tdsrrcnv_read_nv_interrat_feature_ctrl

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_INTERRAT_FEATURE_CTRL and stores it in 
  rrc global variable 'tds_nv_interrat_feature_ctrl'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_interrat_feature_ctrl
(
  void
)
{
  if (tdsrrcnv_efs_read(TDS_RRC_NV_INTERRAT_FEATURE_CTRL, 
                        (byte *) &tds_nv_interrat_feature_ctrl,
                        sizeof(tds_nv_interrat_feature_ctrl)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Successfully read TDS_RRC_NV_INTERRAT_FEATURE_CTRL");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read TDS_RRC_NV_INTERRAT_FEATURE_CTRL failed,set default value");
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_INTERRAT_FEATURE_CTRL,
                                   (byte *) &tds_nv_interrat_feature_ctrl,
                                   sizeof(tds_nv_interrat_feature_ctrl),
                                   TRUE))
    {
       /* Mismatch of NV item */
       tds_nv_interrat_feature_ctrl = 0;
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot get TDS_RRC_NV_INTERRAT_FEATURE_CTRL default value,disable all features");
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tds_nv_interrat_feature_ctrl: 0x%x",tds_nv_interrat_feature_ctrl);
}  

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_sib7_exp_time_factor

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_SIB7_EXP_TIME_FACTOR and stores it in 
  rrc global variable 'tds_nv_sib7_exp_time_factor'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_sib7_exp_time_factor
(
  void
)
{
  if (tdsrrcnv_efs_read(TDS_RRC_NV_SIB7_EXP_TIME_FACTOR, 
                        (byte *) &tds_nv_sib7_exp_time_factor,
                        sizeof(tds_nv_sib7_exp_time_factor)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Successfully read TDS_RRC_NV_SIB7_EXP_TIME_FACTOR %d",tds_nv_sib7_exp_time_factor);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read TDS_RRC_NV_SIB7_EXP_TIME_FACTOR failed,set default value");
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_SIB7_EXP_TIME_FACTOR,
                                   (byte *) &tds_nv_sib7_exp_time_factor,
                                   sizeof(tds_nv_sib7_exp_time_factor),
                                   TRUE))
    {
       /* Mismatch of NV item */
       tds_nv_sib7_exp_time_factor = TDS_NV_DEFAULT_SIB7_EXP_TIME_FACTOR;
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"Cannot get TDS_RRC_NV_SIB7_EXP_TIME_FACTOR default value,set to %d",tds_nv_sib7_exp_time_factor);
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tds_nv_sib7_exp_time_factor: %d",tds_nv_sib7_exp_time_factor);
} 

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_mmcp_enable_thin_ui_config

DESCRIPTION
  This  function gets the NV item enable_thin_ui_config and stores it in 
  rrc global variable 'tds_nv_mmcp_enable_thin_ui_config'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_mmcp_enable_thin_ui_config
(
  void
)
{

  boolean local_nv_mmcp_enable_thin_ui_config = FALSE;

  char path_name[TDS_RRC_NV_FULL_PATH_LEN_MAX];
  int full_path_size;

  tds_nv_mmcp_enable_thin_ui_config = FALSE;

  memset((void *)path_name, '\0', sizeof(path_name));
  full_path_size = snprintf(path_name, TDS_RRC_NV_FULL_PATH_LEN_MAX, "%s", TDS_RRC_NV_MMCP_THIN_UI_PATH);
  if (full_path_size >= TDS_RRC_NV_FULL_PATH_LEN_MAX || full_path_size < 0)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"Insufficient file path buffer for tds nv file");
     tds_nv_mmcp_enable_thin_ui_config = FALSE;
     return;
  }

  /*read enable_thin_ui_cfg from NV*/
  if (tdsrrcnv_efs_read_generic(TDS_RRC_NON_RRC_NV_MMCP_ENABLE_THIN_UI_CONFIG, 
                                (byte *) &local_nv_mmcp_enable_thin_ui_config,
                                sizeof(local_nv_mmcp_enable_thin_ui_config),
                                path_name))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NON_RRC_NV_MMCP_ENABLE_THIN_UI_CONFIG is %d",local_nv_mmcp_enable_thin_ui_config);
  }
  else
  {
    if (!tdsrrcnv_get_non_rrc_nv_default_value(TDS_RRC_NON_RRC_NV_MMCP_ENABLE_THIN_UI_CONFIG,
                                                 (byte *) &local_nv_mmcp_enable_thin_ui_config,
                                                 sizeof(local_nv_mmcp_enable_thin_ui_config)))
    {
       /* Mismatch of NV item */
       local_nv_mmcp_enable_thin_ui_config = FALSE;
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NON_RRC_NV_MMCP_ENABLE_THIN_UI_CONFIG item error. Setting to default: %d",local_nv_mmcp_enable_thin_ui_config);
    }

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read failed for TDS_RRC_NON_RRC_NV_MMCP_ENABLE_THIN_UI_CONFIG,set default to FALSE");
  }
  
  /* use stored nv variable value to set rrc global variable */
  tds_nv_mmcp_enable_thin_ui_config = local_nv_mmcp_enable_thin_ui_config;

}

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_silent_redial_opt

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_SILENT_REDIAL_OPT and stores it in 
  rrc global variable 'tds_nv_silent_redial_opt'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_silent_redial_opt(void)
{
  if (tdsrrcnv_efs_read(TDS_RRC_NV_SILENT_REDIAL_OPT, 
                        (byte *) &tds_nv_silent_redial_opt,
                        sizeof(tds_nv_silent_redial_opt)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Successfully read TDS_RRC_NV_SILENT_REDIAL_OPT");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read TDS_RRC_NV_SILENT_REDIAL_OPT failed,set default value");
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_SILENT_REDIAL_OPT,
                                   (byte *) &tds_nv_silent_redial_opt,
                                   sizeof(tds_nv_silent_redial_opt),
                                   TRUE))
    {
       /* Mismatch of NV item */
       tds_nv_silent_redial_opt = FALSE;
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot get TDS_RRC_NV_SILENT_REDIAL_OPT default value,set to FALSE");
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tds_nv_silent_redial_opt: %d",tds_nv_silent_redial_opt);
}

#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
/*===========================================================================

FUNCTION tdsrrcnv_read_nv_t2l_ping_pong_redir_bar_time

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_T2L_PING_PONG_REDIR_BAR_TIME and stores it in 
  rrc global variable 'tdsrrc_nv_t2l_ping_pang_redir_bar_time'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_t2l_ping_pong_redir_bar_time(void)
{
  if (tdsrrcnv_efs_read(TDS_RRC_NV_T2L_PING_PONG_REDIR_BAR_TIME, 
                        (byte *) &tdsrrc_nv_t2l_ping_pang_redir_bar_time,
                        sizeof(tdsrrc_nv_t2l_ping_pang_redir_bar_time)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Successfully read TDS_RRC_NV_T2L_PING_PONG_REDIR_BAR_TIME");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read TDS_RRC_NV_T2L_PING_PONG_REDIR_BAR_TIME failed,set default value");
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_T2L_PING_PONG_REDIR_BAR_TIME,
                                   (byte *) &tdsrrc_nv_t2l_ping_pang_redir_bar_time,
                                   sizeof(tdsrrc_nv_t2l_ping_pang_redir_bar_time),
                                   TRUE))
    {
       /* Mismatch of NV item */
       tdsrrc_nv_t2l_ping_pang_redir_bar_time = TDSRRC_T2L_PING_PANG_REDIR_BAR_TIME_DEFAULT;
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot get TDS_RRC_NV_T2L_PING_PONG_REDIR_BAR_TIME default value,set to 180");
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrc_nv_t2l_ping_pang_redir_bar_time: %d",tdsrrc_nv_t2l_ping_pang_redir_bar_time);
}
#endif
/*===========================================================================

FUNCTION tdsrrcnv_read_nv_items

DESCRIPTION
  This  function should call all the functions which read NV items to be used in RRC

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_items(void)
{

  sys_modem_as_id_e_type as_id;

  tdsrrcnv_read_nv_tdscdma_optional_feature_list();
  tdsrrcnv_read_nv_tds_l2_opt_feature_list();
  tdsrrcnv_read_nv_interrat_feature_ctrl();
  tdsrrcnv_read_nv_tdscdma_optional_ueci_list();
  tdsrrcnv_read_nv_sib7_exp_time_factor();
  tdsrrcnv_read_flow_control_nv_items();

  tdsrrcnv_read_nv_silent_redial_opt();

#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrcnv_read_nv_tds_hsupa_category();

#endif /* FEATURE_TDSCDMA_HSUPA */  

  tdsrrcnv_read_nv_mmcp_enable_thin_ui_config();

#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
  tdsrrcnv_read_nv_t2l_ping_pong_redir_bar_time();
#endif

  for(as_id = SYS_MODEM_AS_ID_1; as_id <= SYS_MODEM_AS_ID_2; as_id++)
  {
    tdsrrcnv_read_nv_refresh(as_id);
  }
}

/*===========================================================================

FUNCTION tdsrrcnv_init

DESCRIPTION
  This function checks the NV conf file and creates it if it doesn't exist.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_init(void)
{
   int conf_fd, i;
   fs_ssize_t fs;
   struct fs_stat  conf_stat;
   char buf[TDS_RRC_NV_FULL_PATH_LEN_MAX];
   uint32 nv_ver = 0;
   int full_path_size = 0;
   boolean regenerate_conf_file = FALSE;

#ifdef FEATURE_TDSCDMA_EFS_NEW_API
   if (mcfg_fs_stat(TDS_RRC_NV_CONF_FILE, &conf_stat, MCFG_FS_TYPE_EFS, tdsrrc_get_efs_storage_sub_id()) != MCFG_FS_STATUS_OK)
#else
   if (efs_stat(TDS_RRC_NV_CONF_FILE, &conf_stat) < 0)
#endif
  {
      regenerate_conf_file = TRUE;
      tdsrrc_nv_status_bitmask |= TDSRRCNV_BM_CONF_FILE_NOT_EXIST;
   }
   else 
   {
      uint32 nv_ver;
      
      if (tdsrrcnv_efs_read(TDS_RRC_NV_VERSION, (byte *) &nv_ver, sizeof(nv_ver)) == 0)
      {
         regenerate_conf_file = TRUE;
         tdsrrc_nv_status_bitmask |= TDSRRCNV_BM_CONF_FILE_OPEN_FAIL;
      }
      else
      {
         if (nv_ver != (uint32) TDSRRC_NV_VERSION)
         {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"Mismatch TDS NV version,existing %d expecting %d",nv_ver,TDSRRC_NV_VERSION);
            regenerate_conf_file = TRUE;         
            tdsrrc_nv_status_bitmask |= TDSRRCNV_BM_NV_VER_INCORRECT;
         }
      }
   }

   if (regenerate_conf_file)
   {
      /* Re-generate conf file.
      ** It is safe to assume /nv/item_files/conf/ already exists.
      */
      tdsrrc_nv_status_bitmask |= TDSRRCNV_BM_REGEN_CONF_FILE;

      if (tdsrrc_nv_status_bitmask & TDSRRCNV_BM_CONF_FILE_NOT_EXIST)
      {
         /* NV config file doesn't exist */
         conf_fd = efs_creat(TDS_RRC_NV_CONF_FILE, RRC_NV_CONF_FILE_PERMISSIONS_COMMON);
         TDSRRC_MSG0(MSG_LEGACY_HIGH," Create CONF File tdscdma_rrc.conf");
      }
      else
      {
         /* NV config file already exists */
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
         conf_fd = mcfg_fopen(TDS_RRC_NV_CONF_FILE,
                            O_RDWR | O_TRUNC, RRC_NV_CONF_FILE_PERMISSIONS_COMMON,
                            MCFG_FS_TYPE_EFS, tdsrrc_get_efs_storage_sub_id());
#else
         conf_fd = efs_open(TDS_RRC_NV_CONF_FILE,
                            O_RDWR | O_TRUNC, RRC_NV_CONF_FILE_PERMISSIONS_COMMON);
#endif
         TDSRRC_MSG0(MSG_LEGACY_HIGH," Writing into CONF File tdscdma_rrc.conf");
      }

      if (conf_fd < 0)
      {
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
         TDSRRC_MSG1(MSG_LEGACY_ERROR, "CONF file open failed,efs_errno=%d",mcfg_fs_errno(MCFG_FS_TYPE_EFS));
#else
         TDSRRC_MSG1(MSG_LEGACY_ERROR, "CONF file open failed,efs_errno=%d",efs_errno);
#endif
         tdsrrc_nv_status_bitmask |= TDSRRCNV_BM_CONF_FILE_CREATE_FAIL;
         return;
      }

      for (i = 0; i < TDS_RRC_NV_MAX; i++)
      {         
         memset((void *)buf,'\0', sizeof(buf));
         full_path_size = snprintf(buf, TDS_RRC_NV_FULL_PATH_LEN_MAX, "%s%s\r\n", TDS_RRC_NV_ITEM_PATH, tdsrrc_nv_file_name[i]);
         
         if (full_path_size >= TDS_RRC_NV_FULL_PATH_LEN_MAX || full_path_size < 0)
         {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Insufficient file path buffer for tds nv file %d",i);
            tdsrrc_nv_status_bitmask |= TDSRRCNV_BM_NV_ITEM_PATH_INVALID;
            return;
         }

#ifdef FEATURE_TDSCDMA_EFS_NEW_API         
         fs = mcfg_fwrite(conf_fd, buf, (fs_size_t)full_path_size, MCFG_FS_TYPE_EFS);
#else
         fs = efs_write(conf_fd, buf, (fs_size_t) full_path_size);
#endif
         if (fs <= 0)
         {
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"NV file %d write error",tdsrrc_nv_file_name[i],mcfg_fs_errno(MCFG_FS_TYPE_EFS));
#else
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"NV file %d write error",tdsrrc_nv_file_name[i],efs_errno);
#endif
            tdsrrc_nv_status_bitmask |= TDSRRCNV_BM_CONF_FILE_WRITE_FAIL;
            return;
         }
      }      

      /* Nothing we can do if close failure */
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
      (void) mcfg_fclose(conf_fd, MCFG_FS_TYPE_EFS);

      /* create /nv/item_files/modem/tdscdma/rrc/ 
      ** It is safe to assume /nv/item_files/ already exists
      ** ZZTBD: needs to remove all the NV files (efs_unlink)?
      */
      (void) mcfg_mkdir("/nv/item_files/modem", 0777, MCFG_FS_TYPE_EFS);
      TDSRRC_MSG1(MSG_LEGACY_MED,"create /nv/item_files/modem,efs_errno=%d",mcfg_fs_errno(MCFG_FS_TYPE_EFS));

      (void) mcfg_mkdir("/nv/item_files/modem/tdscdma", 0777, MCFG_FS_TYPE_EFS);
      TDSRRC_MSG1(MSG_LEGACY_MED,"create /nv/item_files/modem/tdscdma,efs_errno=%d",mcfg_fs_errno(MCFG_FS_TYPE_EFS));

      (void) mcfg_mkdir(TDS_RRC_NV_ITEM_PATH, 0777, MCFG_FS_TYPE_EFS);
      TDSRRC_MSG1(MSG_LEGACY_MED,"create /nv/item_files/modem/tdscdma/rrc,efs_errno=%d",mcfg_fs_errno(MCFG_FS_TYPE_EFS));

      if (mcfg_fs_stat(TDS_RRC_NV_ITEM_PATH, &conf_stat, MCFG_FS_TYPE_EFS, tdsrrc_get_efs_storage_sub_id()) != MCFG_FS_STATUS_OK)
#else
      (void) efs_close(conf_fd);

      /* create /nv/item_files/modem/tdscdma/rrc/ 
      ** It is safe to assume /nv/item_files/ already exists
      ** ZZTBD: needs to remove all the NV files (efs_unlink)?
      */
      (void) efs_mkdir("/nv/item_files/modem", 0777);
      TDSRRC_MSG1(MSG_LEGACY_MED,"create /nv/item_files/modem,efs_errno=%d",efs_errno);

      (void) efs_mkdir("/nv/item_files/modem/tdscdma", 0777);
      TDSRRC_MSG1(MSG_LEGACY_MED,"create /nv/item_files/modem/tdscdma,efs_errno=%d",efs_errno);

      (void) efs_mkdir(TDS_RRC_NV_ITEM_PATH, 0777);
      TDSRRC_MSG1(MSG_LEGACY_MED,"create /nv/item_files/modem/tdscdma/rrc,efs_errno=%d",efs_errno);

      if (efs_stat(TDS_RRC_NV_ITEM_PATH, &conf_stat) < 0)
#endif
      {
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"Can't create /nv/item_files/modem/tdscdma/rrc");
         tdsrrc_nv_status_bitmask |= TDSRRCNV_BM_NV_ITEM_FOLDER_FAIL;
         return;
      }

      /* Write default value to TDS_RRC_NV_VERSION */
      if (!tdsrrcnv_get_default_value(TDS_RRC_NV_VERSION, (byte *) &nv_ver, sizeof(nv_ver), TRUE))
      {
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"Error writing TDS_RRC_NV_VERSION");
         tdsrrc_nv_status_bitmask |= TDSRRCNV_BM_NV_VER_WRITE_FAIL;
         return;
      }
   }

   TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcnv_init complete");
}

/*===========================================================================

FUNCTION tdsrrcnv_get_default_value

DESCRIPTION
  This function gets default value of a particular NV item. 
  Any change of default NV value should update this function.

DEPENDENCIES
  None.

RETURN VALUE
  Actual size of the NV item. 0 indicates an error.

===========================================================================*/
int tdsrrcnv_get_default_value
(
   tds_rrc_nv_idx_type  nv_item,
   byte                *data_ptr,
   int32                size,
   boolean              write_to_nv
)
{
   int actual_size = 0;

#ifndef FEATURE_TDSCDMA_RRC_NV_WRITE_BACK
   /* MST requested not write default value back to NV */
   write_to_nv = FALSE;
#endif

   if (!data_ptr || !size)
   {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid params: data_ptr=0x%x,size=0x%x",data_ptr,size);
      return actual_size;
   }

   switch(nv_item)
   {
      case TDS_RRC_NV_VERSION:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint32 *) data_ptr) = (uint32) TDSRRC_NV_VERSION;
         }
         break;
         
      case TDS_RRC_NV_ACQ_DB:
         actual_size = sizeof(tds_nv_acq_db_type);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            tds_nv_acq_db_type * acq_db_ptr = (tds_nv_acq_db_type *) data_ptr;
            int i = 0;

            /* Empty ACQ DB */
            acq_db_ptr->curr_rd_index = 0;
            acq_db_ptr->curr_wr_index = 0;
            acq_db_ptr->last_entry_invalid = TRUE;

            for (i = 0; i < TDS_MAX_ACQ_DB_ENTRIES; i++)
               acq_db_ptr->acq_list_indices[i] = i;
			if (TDS_MAX_ACQ_DB_NV_ENTRIES > TDS_MAX_ACQ_DB_ENTRIES)
               memset(&acq_db_ptr->acq_list_indices[TDS_MAX_ACQ_DB_ENTRIES], 0, (TDS_MAX_ACQ_DB_NV_ENTRIES-TDS_MAX_ACQ_DB_ENTRIES)*sizeof(uint8));
         }
         break;
         
      case TDS_RRC_NV_ACQ_LIST:
         actual_size = TDS_MAX_ACQ_DB_NV_ENTRIES * sizeof(tds_nv_acq_entry_type);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            memset((void *) data_ptr, 0x0, actual_size);         
         }
         break;
         
	  case TDS_RRC_NV_PRIM_FREQ_LIST:
		 actual_size = sizeof(tdsrrc_nv_prim_freq_list_default);
		 if (actual_size > size)
		 {
		   TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
		   actual_size = 0;
		 }
		 else
		 {
		   memscpy((void *) data_ptr, actual_size,(void *)tdsrrc_nv_prim_freq_list_default, actual_size);		  
		 }
		 break;
         
      case TDS_RRC_NV_RRC_INTEGRITY_ENABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            /* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
			*((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_INTEGRITY_ENABLED];
         }
         break;
         
      case TDS_RRC_NV_RRC_CIPHERING_ENABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            /* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
            *((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_CIPHERING_ENABLED];
         }
         break;
         
      case TDS_RRC_NV_RRC_FAKE_SECURITY_ENABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            /* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
            *((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_FAKE_SECURITY_ENABLED];
         }
         break;
         
      case TDS_RRC_NV_SPECIAL_FREQ_ENABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            /* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
			*((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_SPECIAL_FREQ_ENABLED];
         }
         break;
         
      case TDS_RRC_NV_SPECIAL_FREQ:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            /* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
			*((uint32 *) data_ptr) = (uint32)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_SPECIAL_FREQ];
         }
         break;
         
      case TDS_RRC_NV_RRC_PDCP_DISABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            /* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
			*((boolean *) data_ptr) = (boolean)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_PDCP_DISABLED];
         }
         break;
         
      case TDS_RRC_NV_RRC_VERSION:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
           
#if defined(FEATURE_TDSCDMA_REL8) || defined(FEATURE_TDSCDMA_REL9)
			   *((uint32 *) data_ptr) = 3; /* value 3 indicates Rel4/Rel5/REL7/Rel9 */
#else
			   *((uint32 *) data_ptr) = 2; /* value 2 indicates Rel4/Rel5/REL7 */
#endif
            ///* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
			//*((uint32 *) data_ptr) = (uint32)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_RRC_VERSION];
         }
         break;
         
      case TDS_RRC_NV_HSDPA_CAT:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            /* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
			*((uint8 *) data_ptr) = (uint8)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_HSDPA_CAT];
         }
         break;
         
      case TDS_RRC_NV_HSUPA_CAT:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            /* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
			*((uint8 *) data_ptr) = (uint8)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_HSUPA_CAT];

         }
         break;
         
      case TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            /* when NV69731 is set/default to 0, and user does not specify its value, this NV takes the predefined value for CMCC field test */
			*((uint32 *) data_ptr) = (uint32)tds_nv_cmcc_test_setting[1][TDS_RRC_NV_CMCC_TEST_SETTING_TDS_OPTIONAL_FEATURE_LIST];
         }
         break;
         
      case TDS_RRC_NV_DELAY_OOS_IND_TIMER:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {    
            *((uint8 *) data_ptr) = (uint8) 1; /* default value 1 second */
         }
         break;
         
      case TDS_RRC_NV_TDS_L2_OPT_FEATURE_LIST:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {    
            *((uint32 *) data_ptr) = (uint32) 0; /* disable all bits */
         }
         break;

      case TDS_RRC_NV_INTERRAT_FEATURE_CTRL:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {    
            *((uint32 *) data_ptr) = (uint32) 0; /* no support */
         }
         break;

      case TDS_RRC_NV_CS_VOICE_OVER_HSPA_ENABLED:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = FALSE;
         }
         break;
         
      case TDS_RRC_NV_GERAN_FEATURE_PACK:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = (uint8) 0; /* no support */
         }
         break;
         
      case TDS_RRC_NV_INTERRAT_NACC_SUPPORT:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = (uint8) 0; /* no support */
         }
         break;
         
      case TDS_RRC_NV_CFA_CONF:                          /* uint32                                    */
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint32 *) data_ptr) = (uint32) 0x0003001C; 
         }
         break;
         
      case TDS_RRC_NV_IGNORE_CELL_BAR_RESERVE_STATUS:    /* uint8                                     */
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = (uint8) 0;
         }
         break;
         
      case TDS_RRC_NV_SNOW3G_ENABLED:                    /* boolean                                   */
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = FALSE;
         }
         break;
         
      case TDS_RRC_NV_SET_SRB2_ACT_TIME:                 /* uint8                                     */
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = (uint8) 0;
         }
         break;
         
      case TDS_RRC_NV_CSFB_SKIP_SIB11_OPT:               /* uint8                                     */
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = (uint8) 0;
         }
         break;
         
      case TDS_RRC_NV_FREQ_CELL_LOCK:                         /* uint16                                    */
         actual_size = sizeof(tds_nv_lock_freq_cell_type);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            tds_nv_lock_freq_cell_type * tds_lock_freq_cell = (tds_nv_lock_freq_cell_type *) data_ptr;

            tds_lock_freq_cell->lock_mode = TDSRRC_CSP_NO_LOCK; /* enable by default */
         }
         break;
         
      case TDS_RRC_NV_DO_NOT_FORWARD_PAGE:               /* uint8                                     */
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = (uint8) 0;
         }
         break;
         
      case TDS_RRC_NV_A2_POWER_OPT:                      /* uint8                                     */
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = (uint8) 0;
         }
         break;
         
      case TDS_RRC_NV_FAST_RET_TO_LTE_AFTER_CSFB:        /* boolean                                   */
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = FALSE;
         }
         break;
         
      case TDS_RRC_NV_FAST_RET_TO_LTE_AFTER_CSFB_TIMER:  /* uint16                                    */
         actual_size = sizeof(uint16);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
            *((uint16 *) data_ptr) = (uint16) 2000;
#else
            *((uint16 *) data_ptr) = (uint16) 0;
#endif
         }
         break;
         
      case TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = 2;
         }
         break;
      case TDS_RRC_NV_TDS_OPTIONAL_UECI_LIST:
         actual_size = sizeof(uint32);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {  
            *((uint32 *) data_ptr) = (uint32) TDS_NV_OPTIONAL_UECI_DEFAULT_SUPPORT_BITMASK;
			      *((uint32 *) data_ptr) |= TDS_NV_OPTIONAL_UECI_L2L3_TRACE_ENABLE_BITMASK;
         }
         break;
      case TDS_RRC_NV_SIB_SLEEP_BEFORE_SB:
         actual_size = sizeof(tds_nv_sib_sleep);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {  
            tds_nv_sib_sleep * sib_sleep_ptr = (tds_nv_sib_sleep *) data_ptr;

            sib_sleep_ptr->sib_sleep_flag = 0; /* disable by default */
            sib_sleep_ptr->repetition_len = 128;
            sib_sleep_ptr->sib_end_frame = 70;
            sib_sleep_ptr->misc_data = 0;
         }
         break;
      case TDS_RRC_NV_SIB7_EXP_TIME_FACTOR:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {  
            *((uint8 *) data_ptr) = (uint8) TDS_NV_DEFAULT_SIB7_EXP_TIME_FACTOR;
         }
         break;

      case TDS_RRC_NV_SILENT_REDIAL_OPT:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = FALSE;
         }
       break;
      case TDS_RRC_NV_BAND_SEARCH_MASK:
         actual_size = sizeof(uint16);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {  
            *((uint16 *) data_ptr) = (uint16)(SYS_BAND_MASK_TDS_BANDA|SYS_BAND_MASK_TDS_BANDF);
         }
         break;
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
       case TDS_RRC_NV_RLC_REESTABLISH:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {  
            *((uint8 *) data_ptr) = (uint8) (TDS_NV_DEFAULT_RLC_REESTABLISH_BITMASK);
         }
         break;
#endif

      case TDS_RRC_NV_BPLMN_IGNOR_SIB19:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = FALSE;
         }
         break;

      case TDS_RRC_NV_PSEUDO_SIB19:
         actual_size = sizeof(tds_nv_pseudo_sib19_type);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {  
            tds_nv_pseudo_sib19_type * pseudo_sib19_ptr = (tds_nv_pseudo_sib19_type *) data_ptr;

            pseudo_sib19_ptr->is_pseudo_sib19_enabled = TRUE; /* enable by default */
            pseudo_sib19_ptr->utra_priority = TDSRRCNV_PSEUDO_SIB19_UTRA_PRI;
            pseudo_sib19_ptr->s_priority_search1 = TDSRRCNV_PSEUDO_SIB19_S_PRI_SRCH1;
            pseudo_sib19_ptr->thresh_serving_low = TDSRRCNV_PSEUDO_SIB19_THR_SERV_LOW;
            pseudo_sib19_ptr->eutra_priority = TDSRRCNV_PSEUDO_SIB19_EUTRA_PRI;
            pseudo_sib19_ptr->eutra_meas_bandwidth = TDSRRCNV_PSEUDO_SIB19_EUTRA_MEAS_BW;
            pseudo_sib19_ptr->eutra_thresh_high = TDSRRCNV_PSEUDO_SIB19_THR_X_HIGH;
            pseudo_sib19_ptr->eutra_thresh_low = TDSRRCNV_PSEUDO_SIB19_THR_X_LOW;
            pseudo_sib19_ptr->eutra_qrxlevmin = TDSRRCNV_PSEUDO_SIB19_EUTRA_QRXLEVMIN;
            pseudo_sib19_ptr->reserved = TDSRRCNV_PSEUDO_SIB19_RESERVE;
         }
         break;

      case TDS_RRC_NV_L2T_ACQDB_CNT_THRESHOLD_REDIRECTION:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((uint8 *) data_ptr) = TDSRRCNV_L2T_ACQDB_CNT_THRESHOLD_REDIRECTION_DEFAULT;
         }
         break;
#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
      case TDS_RRC_NV_T2L_PING_PONG_REDIR_BAR_TIME:
         actual_size = sizeof(uint8);
         if (actual_size > size)
         {
           TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
           actual_size = 0;
         }
         else
         { 	 
           *((uint8 *) data_ptr) = (uint8) TDSRRC_T2L_PING_PANG_REDIR_BAR_TIME_DEFAULT; /* default value 180 second */
         }
         break;
#endif
      case TDS_RRC_NV_ENABLE_AUDIO_SCR_MODE:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = TRUE;
         }
         break;

      case TDS_RRC_NV_EST_FAIL_BAR_CELL:
         actual_size = sizeof(tds_nv_est_fail_bar_cell_type);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            tds_nv_est_fail_bar_cell_type * tds_est_fail_bar_cell = (tds_nv_est_fail_bar_cell_type *) data_ptr;

            tds_est_fail_bar_cell->bar_cell_enable = TRUE; /* enable by default*/
            tds_est_fail_bar_cell->bar_cell_rscp_threshold = -85; /*-85dbm*/
            tds_est_fail_bar_cell->bar_cell_n300_failure_timer = 320; /*320s*/
         }
         break;
	  case TDS_RRC_NV_CS_FAIL_BAR_TIME:
		  actual_size = sizeof(uint32);
		  if (actual_size > size)
		  {
			TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
			actual_size = 0;
		  }
		  else
		  {   
			*((uint32 *) data_ptr) = (uint32) TDS_CS_DOMAIN_BAR_TIME_LENGTH; 
		  }
		  break;	 

      default:
         TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid NV item %d",nv_item);
         break;
   }

   if (actual_size && write_to_nv)
   {
      if (tdsrrcnv_efs_write(nv_item, data_ptr, actual_size))
      {
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"Item %d written to NV success size=%d",nv_item,actual_size);
      }
      else
      {
         TDSRRC_MSG2(MSG_LEGACY_ERROR,"Item %d written to NV failed size=%d",nv_item,actual_size);
      }
   }
   
   return actual_size;
}

/*===========================================================================

FUNCTION tdsrrcnv_get_non_rrc_nv_default_value

DESCRIPTION
  This function gets default value of a particular non-RRC NV item. 
  Any change of default NV value should update this function.

DEPENDENCIES
  None.

RETURN VALUE
  Actual size of the NV item. 0 indicates an error.

===========================================================================*/
uint32 tdsrrcnv_get_non_rrc_nv_default_value
(
   tds_rrc_non_rrc_nv_idx_type  nv_item,
   byte                         *data_ptr,
   uint32                       size
)
{
   uint32 actual_size = 0;

   if (!data_ptr || !size)
   {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid params: data_ptr=0x%x,size=0x%x",data_ptr,size);
      return actual_size;
   }

   switch(nv_item)
   {
      case TDS_RRC_NON_RRC_NV_MMCP_ENABLE_THIN_UI_CONFIG:
         actual_size = sizeof(boolean);
         if (actual_size > size)
         {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid params: item=%d, actual_size=%d > size=%d", nv_item, actual_size, size);
            actual_size = 0;
         }
         else
         {
            *((boolean *) data_ptr) = FALSE;
         }
         break;
      default:
         TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid NV item %d",nv_item);
         break;
   }
   
   return actual_size;
}

/*===========================================================================

FUNCTION tdsrrcnv_efs_delete

DESCRIPTION
  Delete an nv item from the EFS

DEPENDENCIES
  None

RETURN VALUE
  TRUE: sucess
  FALSE: failure

SIDE EFFECTS
  None.

===========================================================================*/

boolean tdsrrcnv_efs_delete
(
        tds_rrc_nv_idx_type          nv_item
)
{
   boolean ret = FALSE;
   
#if defined(FEATURE_EFS)
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
   mcfg_fs_status_e_type  status;
#else
   int      fd;
#endif
   char     file_name[TDS_RRC_NV_FULL_PATH_LEN_MAX];
   int      full_path_size;
 
   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   if (nv_item == TDS_RRC_NV_MAX)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid efs nv item");
      return ret;
   }
 
   /* Map nv_item to filename
   */
   memset((void *)file_name, '\0', sizeof(file_name));
   full_path_size = snprintf(file_name, TDS_RRC_NV_FULL_PATH_LEN_MAX, "%s%s", TDS_RRC_NV_ITEM_PATH, tdsrrc_nv_file_name[nv_item]);

   if (full_path_size >= TDS_RRC_NV_FULL_PATH_LEN_MAX || full_path_size < 0)
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Insufficient file path buffer for %d",nv_item);
      return ret;
   }

   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
   TDSRRC_MSG1(MSG_LEGACY_MED,"Delete nv item file %d ",nv_item);

#ifdef FEATURE_TDSCDMA_EFS_NEW_API
  status = mcfg_fs_delete(file_name, MCFG_FS_TYPE_EFS, tdsrrc_get_efs_storage_sub_id());
 
   if ( status != MCFG_FS_STATUS_OK )
   {
      TDSRRC_MSG2(MSG_LEGACY_ERROR, "mcfg fs delete fail status=%d,errno=%d",status,mcfg_fs_errno(MCFG_FS_TYPE_EFS));
   }
#else
   fd = efs_unlink(file_name);
 
   if ( fd != 0 )
   {
      TDSRRC_MSG2(MSG_LEGACY_ERROR, "efs delete fail efs_err=%d,errno=%d",fd,efs_errno);
   } // if(fd < 0)
#endif
   else
   {
      TDSRRC_MSG0(MSG_LEGACY_MED,"Delete file succeeded");
      ret = TRUE;
   }
#else
   SYS_ARG_NOT_USED(nv_item);
#endif /*   #if defined(FEATURE_EFS) */

   return ret;
}

/*===========================================================================

FUNCTION tdsrrcnv_efs_write

DESCRIPTION
  Write contents of data_ptr to EFS item denoted by nv_item

DEPENDENCIES
  None

RETURN VALUE
  TRUE: sucess
  FALSE: failure

SIDE EFFECTS
  None.

===========================================================================*/

boolean tdsrrcnv_efs_write
(
        tds_rrc_nv_idx_type          nv_item,
            /* passing the nv_item to write.
            */
        const byte                  *data_ptr,
            /* Pointer to a data buffer to write to EFS.
            */
        uint32                       size
            /* Size of the data_ptr buffer.
            */
)
{
   boolean ret = FALSE;
 
#ifdef FEATURE_TDSCDMA_EFS_NEW_API  
   mcfg_fs_status_e_type      status;
#else
   int      fd;
#endif
   char     file_name[TDS_RRC_NV_FULL_PATH_LEN_MAX];
   int      full_path_size;
 
   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   if (!data_ptr || !size)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid efs data");
      return ret;
   }

   if (nv_item == TDS_RRC_NV_MAX)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid efs nv item");
      return ret;
   }
 
   /* Map nv_item to filename
   */
   memset((void *)file_name, '\0', sizeof(file_name));
   full_path_size = snprintf(file_name, TDS_RRC_NV_FULL_PATH_LEN_MAX, "%s%s", TDS_RRC_NV_ITEM_PATH, tdsrrc_nv_file_name[nv_item]);

   if (full_path_size >= TDS_RRC_NV_FULL_PATH_LEN_MAX || full_path_size < 0)
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Insufficient file path buffer for %d",nv_item);
      return ret;
   }

   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
   TDSRRC_MSG1(MSG_LEGACY_MED,"Put nv item file %d ",nv_item);

#ifdef FEATURE_TDSCDMA_EFS_NEW_API
   status = mcfg_fs_write( file_name, (void*)data_ptr, size, O_RDWR|O_CREAT|O_TRUNC|O_AUTODIR, 0777, 
   	                   MCFG_FS_TYPE_EFS, tdsrrc_get_efs_storage_sub_id() );
 
   if ( status != MCFG_FS_STATUS_OK )
   {
      TDSRRC_MSG2(MSG_LEGACY_ERROR, "mcfg fs write fail status=%d,errno=%d",status,mcfg_fs_errno(MCFG_FS_TYPE_EFS));
   }
#else
   fd = efs_put( file_name, (void*)data_ptr, (int)size, O_RDWR|O_CREAT|O_TRUNC|O_AUTODIR, 0777);
 
   if ( fd != 0 )
   {
      TDSRRC_MSG2(MSG_LEGACY_ERROR, "efs write fail efs_err=%d,errno=%d",fd,efs_errno);
   } // if(fd < 0)
#endif
   else
   {
      TDSRRC_MSG0(MSG_LEGACY_MED,"Put file succeeded");
      ret = TRUE;
   }

   return ret;
}

/*===========================================================================

FUNCTION tdsrrcnv_efs_read

DESCRIPTION
  Read EFS item denoted by efs_id and copy to data_ptr.
  If not read successfully, substitute default values.

DEPENDENCIES
  None

RETURN VALUE
  Actual size read. 0 indicates an error.
  
  *size_ptr is set to the actual number of bytes that were copied into the
  buffer pointed by data_ptr. If the read failed, *size_ptr = 0

SIDE EFFECTS
  None.


==========================================================================*/
uint32 tdsrrcnv_efs_read
(
        tds_rrc_nv_idx_type          nv_item,
            /* passing the nv_item to read.
            */
        byte                        *data_ptr,
            /* Pointer to a data buffer where to store the data that is
            ** associated with the EFS item.
            */
        uint32                       size
            /* Size of the data_ptr buffer 
            */
)
{
   uint32      actual_size = 0;

#ifdef FEATURE_TDSCDMA_EFS_NEW_API
   mcfg_fs_status_e_type  status;
#else
   int         fd;
#endif
   char        file_name[TDS_RRC_NV_FULL_PATH_LEN_MAX];
   int         full_path_size;
   tdsrrc_special_test_settings_config_e_type special_test_cfg;
 
   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
   if (!data_ptr || !size)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid efs data");
      return actual_size;
   }

   if (nv_item == TDS_RRC_NV_MAX)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid efs nv item");
      return actual_size;
   }
 
   /* before reading efs files, check if any CMCC special test setting should apply due to NV69731 cfg */
   special_test_cfg = tdsrrc_get_special_test_settings_config();
   
   if ( (special_test_cfg != TDSRRC_SPECIAL_TEST_SETTINGS_CONFIG_INVALID) &&
   	    (special_test_cfg != TDSRRC_SPECIAL_TEST_SETTINGS_CONFIG_ENGINEERING) )
   {
      TDSRRC_MSG0(MSG_LEGACY_MED,"CMCC special test setting cfg applied");

      /* when NV69731 set to 0, no predefined cmcc test setting enforced. So indx to static array tds_nv_cmcc_test_setting
               should be special_test_cfg-1 
          */
      actual_size = tdsrrcnv_read_cmcc_test_nv_value((uint8)special_test_cfg-1, nv_item, data_ptr, size);

      if (actual_size)
      {
         TDSRRC_MSG1(MSG_LEGACY_MED,"CMCC test NV item %d",nv_item);
         return actual_size;
      }
      else
      {
         TDSRRC_MSG1(MSG_LEGACY_MED,"Not a CMCC test NV item %d",nv_item);
      }
   }

   /* Map nv_item to filename
   */
   memset((void *)file_name, '\0', sizeof(file_name));
   full_path_size = snprintf(file_name, TDS_RRC_NV_FULL_PATH_LEN_MAX, "%s%s", TDS_RRC_NV_ITEM_PATH, tdsrrc_nv_file_name[nv_item]);

   if (full_path_size >= TDS_RRC_NV_FULL_PATH_LEN_MAX || full_path_size < 0)
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Insufficient file path buffer for %d",nv_item);
      return actual_size;
   }
 
   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
   TDSRRC_MSG1(MSG_LEGACY_MED,"Get nv item file %d ",nv_item);
  
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
   status = mcfg_fs_read(file_name, data_ptr, size, MCFG_FS_TYPE_EFS, tdsrrc_get_efs_storage_sub_id());
 
   if (status != MCFG_FS_STATUS_OK)
   {      
      TDSRRC_MSG3(MSG_LEGACY_ERROR, "get nv_item=%d status=%d errno=%d", nv_item, status, mcfg_fs_errno(MCFG_FS_TYPE_EFS));
#else
   fd = efs_get(file_name, data_ptr, size);
 
   if ((fd < 0) || ((uint32) fd > size))
   {      
      TDSRRC_MSG3(MSG_LEGACY_ERROR, "get nv_item=%d fd=%d errno=%d", nv_item, fd, efs_errno);
      TDSRRC_MSG2(MSG_LEGACY_ERROR, "nv expected_size=%d,actual_size=%d",size,fd);
#endif
 
      /* Reset size_ptr as error indication */
      actual_size = 0;

     /* If read failed, do not substitute a default value
       */
   }
   else
   {
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
      actual_size = size;
#else
      actual_size = (uint32) fd;
#endif
      TDSRRC_MSG1(MSG_LEGACY_MED, "Get file succeeded,size=%d",actual_size);
   }

   return actual_size;
}

/*===========================================================================

FUNCTION tdsrrcnv_efs_read_generic

DESCRIPTION
  Read EFS item denoted by efs_id (EFS items outside RRC)
  and copy to data_ptr.
  If not read successfully, substitute default values.

DEPENDENCIES
  None

RETURN VALUE
  Actual size read. 0 indicates an error.
  
  *size_ptr is set to the actual number of bytes that were copied into the
  buffer pointed by data_ptr. If the read failed, *size_ptr = 0

SIDE EFFECTS
  None.


==========================================================================*/
uint32 tdsrrcnv_efs_read_generic
(
        tds_rrc_non_rrc_nv_idx_type          nv_item,
            /* passing the nv_item to read.
            */
        byte                        *data_ptr,
            /* Pointer to a data buffer where to store the data that is
            ** associated with the EFS item.
            */
        uint32                       size,
            /* Size of the data_ptr buffer 
            */
        char                         *path_ptr
            /* Pointer to the NV item path
            */
)
{
   uint32      actual_size = 0;

#ifdef FEATURE_TDSCDMA_EFS_NEW_API
   mcfg_fs_status_e_type  status;
#else
   int         fd;
#endif
   char        file_name[TDS_RRC_NV_FULL_PATH_LEN_MAX];
   int         full_path_size;
 
   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
   if (!data_ptr || !size)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid efs data");
      return actual_size;
   }

   if (nv_item == TDS_RRC_NON_RRC_NV_MAX)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid efs nv item");
      return actual_size;
   }
 
   /* Map nv_item to filename
   */
   memset((void *)file_name, '\0', sizeof(file_name));
   full_path_size = snprintf(file_name, TDS_RRC_NV_FULL_PATH_LEN_MAX, "%s%s", path_ptr, tdsrrc_non_rrc_nv_file_name[nv_item]);

   if (full_path_size >= TDS_RRC_NV_FULL_PATH_LEN_MAX || full_path_size < 0)
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Insufficient file path buffer for %d",nv_item);
      return actual_size;
   }
 
   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
   TDSRRC_MSG1(MSG_LEGACY_MED,"Get nv item file %d ",nv_item);
  
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
   status = mcfg_fs_read(file_name, data_ptr, size, MCFG_FS_TYPE_EFS, tdsrrc_get_efs_storage_sub_id());
 
   if (status != MCFG_FS_STATUS_OK)
   {      
      TDSRRC_MSG3(MSG_LEGACY_ERROR, "get nv_item=%d status=%d errno=%d", nv_item, status, mcfg_fs_errno(MCFG_FS_TYPE_EFS));
#else
   fd = efs_get(file_name, data_ptr, size);
 
   if ((fd < 0) || ((uint32) fd > size))
   {      
      TDSRRC_MSG3(MSG_LEGACY_ERROR, "get nv_item=%d fd=%d errno=%d", nv_item, fd, efs_errno);
      TDSRRC_MSG2(MSG_LEGACY_ERROR, "nv expected_size=%d,actual_size=%d",size,fd);
#endif
 
      /* Reset size_ptr as error indication */
      actual_size = 0;

     /* If read failed, do not substitute a default value
       */
   }
   else
   {
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
      actual_size = size;
#else
      actual_size = (uint32) fd;
#endif
      TDSRRC_MSG1(MSG_LEGACY_MED, "Get file succeeded,size=%d",actual_size);
   }

   return actual_size;
}

/*===========================================================================

FUNCTION tdsrrcnv_read_nv_item_default

DESCRIPTION
This function similar to WCDMA's rrc_read_nv_item.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_read_nv_item_default(void)
{

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_IGNORE_CELL_BAR_RESERVE_STATUS, 
                        (byte *) &tdsrrc_ignore_cell_bar_nv,
                        sizeof(tdsrrc_ignore_cell_bar_nv)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_IGNORE_CELL_BAR_RESERVE_STATUS,
                                   (byte *) &tdsrrc_ignore_cell_bar_nv,
                                   sizeof(tdsrrc_ignore_cell_bar_nv),
                                   TRUE))
    {
      tdsrrc_ignore_cell_bar_nv = 0;
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_SNOW3G_ENABLED, 
                        (byte *) &tdsrrc_snow3g_security_algo_supported_nv,
                        sizeof(tdsrrc_snow3g_security_algo_supported_nv)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_SNOW3G_ENABLED,
                                   (byte *) &tdsrrc_snow3g_security_algo_supported_nv,
                                   sizeof(tdsrrc_snow3g_security_algo_supported_nv),
                                   TRUE))
    {
      tdsrrc_snow3g_security_algo_supported_nv = FALSE;
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_SET_SRB2_ACT_TIME, 
                        (byte *) &tdsrrc_set_srb2_act_time_nv,
                        sizeof(tdsrrc_set_srb2_act_time_nv)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_SET_SRB2_ACT_TIME,
                                   (byte *) &tdsrrc_set_srb2_act_time_nv,
                                   sizeof(tdsrrc_set_srb2_act_time_nv),
                                   TRUE))
    {
      tdsrrc_set_srb2_act_time_nv = 0;
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_CSFB_SKIP_SIB11_OPT, 
                        (byte *) &tdsrrc_csfb_skip_sib11_opt_nv,
                        sizeof(tdsrrc_csfb_skip_sib11_opt_nv)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_CSFB_SKIP_SIB11_OPT,
                                   (byte *) &tdsrrc_csfb_skip_sib11_opt_nv,
                                   sizeof(tdsrrc_csfb_skip_sib11_opt_nv),
                                   TRUE))
    {
      tdsrrc_csfb_skip_sib11_opt_nv = 0;
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_FREQ_CELL_LOCK, 
                        (byte *) &tdsrrccsp_nv_lock_freq_cell,
                        sizeof(tdsrrccsp_nv_lock_freq_cell)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_FREQ_CELL_LOCK,
                                   (byte *) &tdsrrccsp_nv_lock_freq_cell,
                                   sizeof(tdsrrccsp_nv_lock_freq_cell),
                                   TRUE))
    { /*If get default value fail, treat as TDSRRC_CSP_NO_LOCK*/
      tdsrrccsp_nv_lock_freq_cell.lock_mode = TDSRRC_CSP_NO_LOCK;
    }
  }
  else
  { /* if lock_mode exceed LOCK_CPID, treat as TDSRRC_CSP_NO_LOCK*/
    if (tdsrrccsp_nv_lock_freq_cell.lock_mode > TDSRRC_CSP_LOCK_CELL)
    {
        tdsrrccsp_nv_lock_freq_cell.lock_mode = TDSRRC_CSP_NO_LOCK;
    }
  }
  tdsrrccsp_lock_freq_cell.lock_mode = tdsrrccsp_nv_lock_freq_cell.lock_mode;
  tdsrrccsp_lock_freq_cell.lock_freq = tdsrrccsp_nv_lock_freq_cell.lock_freq;
  tdsrrccsp_lock_freq_cell.lock_cpid = tdsrrccsp_nv_lock_freq_cell.lock_cpid;
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"lock_mode = %d, lock_freq = %d, lock_cpid = %d", 
             tdsrrccsp_lock_freq_cell.lock_mode, tdsrrccsp_lock_freq_cell.lock_freq, tdsrrccsp_lock_freq_cell.lock_cpid);

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_DO_NOT_FORWARD_PAGE, 
                        (byte *) &tdsrrc_do_not_forward_page_nv,
                        sizeof(tdsrrc_do_not_forward_page_nv)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_DO_NOT_FORWARD_PAGE,
                                   (byte *) &tdsrrc_do_not_forward_page_nv,
                                   sizeof(tdsrrc_do_not_forward_page_nv),
                                   TRUE))
    {
      tdsrrc_do_not_forward_page_nv = 0;
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_A2_POWER_OPT, 
                        (byte *) &tdsrrc_integrity_opt_nv,
                        sizeof(tdsrrc_integrity_opt_nv)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_A2_POWER_OPT,
                                   (byte *) &tdsrrc_integrity_opt_nv,
                                   sizeof(tdsrrc_integrity_opt_nv),
                                   TRUE))
    {
      tdsrrc_integrity_opt_nv = 0;
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_FAST_RET_TO_LTE_AFTER_CSFB, 
                        (byte *) &tdsrrc_fast_return_to_lte_after_csfb_nv,
                        sizeof(tdsrrc_fast_return_to_lte_after_csfb_nv)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_FAST_RET_TO_LTE_AFTER_CSFB,
                                   (byte *) &tdsrrc_fast_return_to_lte_after_csfb_nv,
                                   sizeof(tdsrrc_fast_return_to_lte_after_csfb_nv),
                                   TRUE))
    {
      tdsrrc_fast_return_to_lte_after_csfb_nv = FALSE;
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_FAST_RET_TO_LTE_AFTER_CSFB_TIMER, 
                        (byte *) &tdsrrc_fast_return_to_lte_after_csfb_timer_nv,
                        sizeof(tdsrrc_fast_return_to_lte_after_csfb_timer_nv)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_FAST_RET_TO_LTE_AFTER_CSFB_TIMER,
                                   (byte *) &tdsrrc_fast_return_to_lte_after_csfb_timer_nv,
                                   sizeof(tdsrrc_fast_return_to_lte_after_csfb_timer_nv),
                                   TRUE))
    {
      tdsrrc_fast_return_to_lte_after_csfb_timer_nv = 0;
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_BAND_SEARCH_MASK, 
                        (byte *) &tdsrrc_nv_band_search_mask,
                        sizeof(tdsrrc_nv_band_search_mask)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_BAND_SEARCH_MASK,
                                   (byte *) &tdsrrc_nv_band_search_mask,
                                   sizeof(tdsrrc_nv_band_search_mask),
                                   TRUE))
    {
      tdsrrc_nv_band_search_mask = (SYS_BAND_MASK_TDS_BANDA|SYS_BAND_MASK_TDS_BANDF);
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_BPLMN_IGNOR_SIB19, 
                        (byte *) &tdsrrc_bplmn_ignore_sib19_nv,
                        sizeof(tdsrrc_bplmn_ignore_sib19_nv)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_BPLMN_IGNOR_SIB19,
                                   (byte *) &tdsrrc_bplmn_ignore_sib19_nv,
                                   sizeof(tdsrrc_bplmn_ignore_sib19_nv),
                                   TRUE))
    {
      tdsrrc_bplmn_ignore_sib19_nv = FALSE;
    }
  }

  tdsrrcnv_get_pseudo_sib19();

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_L2T_ACQDB_CNT_THRESHOLD_REDIRECTION, 
                        (uint8 *) &tdsrrc_nv_l2t_acqdb_cnt_threshold_redirection,
                        sizeof(tdsrrc_nv_l2t_acqdb_cnt_threshold_redirection)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_L2T_ACQDB_CNT_THRESHOLD_REDIRECTION,
                                   (uint8 *) &tdsrrc_nv_l2t_acqdb_cnt_threshold_redirection,
                                   sizeof(tdsrrc_nv_l2t_acqdb_cnt_threshold_redirection),
                                   TRUE))
    {
      tdsrrc_nv_l2t_acqdb_cnt_threshold_redirection = TDSRRCNV_L2T_ACQDB_CNT_THRESHOLD_REDIRECTION_DEFAULT;
    }
  }

  if (!tdsrrcnv_efs_read(TDS_RRC_NV_HOLD_PS_TMR_VALUE, 
                        (uint8 *) &tdsrrc_nv_hold_ps_tmr_value,
                        sizeof(tdsrrc_nv_hold_ps_tmr_value)))
  {
    tdsrrc_nv_hold_ps_tmr_value = 0;
  }

   if (!tdsrrcnv_efs_read(TDS_RRC_NV_ENABLE_AUDIO_SCR_MODE, 
                         (byte *) &tdsrrc_nv_enable_audio_scr_mode,
                         sizeof(tdsrrc_nv_enable_audio_scr_mode)))
   {
     if (!tdsrrcnv_get_default_value(TDS_RRC_NV_ENABLE_AUDIO_SCR_MODE,
                                    (byte *) &tdsrrc_nv_enable_audio_scr_mode,
                                    sizeof(tdsrrc_nv_enable_audio_scr_mode),
                                    TRUE))
     {
       tdsrrc_nv_enable_audio_scr_mode = TRUE;
     }
   }

 if (!tdsrrcnv_efs_read(TDS_RRC_NV_EST_FAIL_BAR_CELL, 
                          (byte *) &tdsrrcrce_nv_est_fail_bar_cell,
                          sizeof(tdsrrcrce_nv_est_fail_bar_cell)))
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_EST_FAIL_BAR_CELL,
                                   (byte *) &tdsrrcrce_nv_est_fail_bar_cell,
                                   sizeof(tdsrrcrce_nv_est_fail_bar_cell),
                                   TRUE))
    { 
      /*If get default value fail, set default value*/
      tdsrrcrce_nv_est_fail_bar_cell.bar_cell_enable = TRUE; /* enable by default*/
      tdsrrcrce_nv_est_fail_bar_cell.bar_cell_rscp_threshold = -85; /*-85dbm*/
      tdsrrcrce_nv_est_fail_bar_cell.bar_cell_n300_failure_timer = 320; /*320s*/
    }
	TDSRRC_MSG3(MSG_LEGACY_HIGH,"bar_cell_enable = %d, bar_cell_rscp_threshold = %d, bar_cell_n300_failure_timer = %d", 
             tdsrrcrce_nv_est_fail_bar_cell.bar_cell_enable, 
             tdsrrcrce_nv_est_fail_bar_cell.bar_cell_rscp_threshold, 
             tdsrrcrce_nv_est_fail_bar_cell.bar_cell_n300_failure_timer);
  }
}

/*===========================================================================

FUNCTION tdsrrcnv_process_get_cmcc_config

DESCRIPTION
  This function gets CMCC config value.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_process_get_cmcc_config(void)
{
   errno_enum_type status;
   tds_rrc_get_config_rsp_type config_rsp;

   memset(&config_rsp,0,sizeof(tds_rrc_get_config_rsp_type));

   config_rsp.active_config = tdsrrc_active_config;
   config_rsp.desired_config = tdsrrc_desired_config;

   msgr_init_hdr(&config_rsp.msg_hdr, MSGR_TDSCDMA_RRC, TDSCDMA_RRC_GET_CONFIG_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
   msgr_set_hdr_variant(&config_rsp.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
   msgr_set_hdr_inst(&config_rsp.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
   status = msgr_send(&config_rsp.msg_hdr, sizeof(tds_rrc_get_config_rsp_type));

   TDSRRC_MSG3(MSG_LEGACY_HIGH, "Send TDSCDMA_RRC_GET_CONFIG_RSP, status = %d, act_cfg = %d, des_cfg = %d", 
               status,
               tdsrrc_active_config,
               tdsrrc_desired_config);
   
   if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      tdscfa_send_message_to_qmi(&config_rsp.msg_hdr, sizeof(tds_rrc_get_config_rsp_type));
   }
   
}

/*===========================================================================

FUNCTION tdsrrcnv_process_set_cmcc_config

DESCRIPTION
  This function sets CMCC config value.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_process_set_cmcc_config(tdsrrc_cmd_type* cmd_ptr)
{
   errno_enum_type status;
   boolean ret_status = TRUE;
   uint8 nv_value;
   tds_rrc_config_e_type config;
   tds_rrc_set_config_rsp_type set_rsp;

   if (!cmd_ptr)
   {
      ERR_FATAL("tdsrrcnv_process_set_cmcc_config received null cmd_ptr",0,0,0);
   }

   memset(&set_rsp,0,sizeof(tds_rrc_set_config_rsp_type));

   config = cmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.tds_rrc_set_config_req.config;

   TDSRRC_MSG3(MSG_LEGACY_HIGH,"new_config = %d, act_config = %d, des_cfg = %d",
               config,
               tdsrrc_active_config,
               tdsrrc_desired_config);
   
   if (config == tdsrrc_active_config)
   {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"The new config %d already active,set to desired config as well",config);

      tdsrrc_desired_config = config;
      ret_status = TRUE;
   }
   else if (config == tdsrrc_desired_config)
   {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"The new config %d already set (act_config = %d),please power cycle the UE",config,tdsrrc_active_config);

      ret_status = TRUE;
   }
   else
   {
      nv_value = tdsrrcnv_config2nv(config);

      if (tdsrrcnv_efs_write(TDS_RRC_NV_SPECIAL_SETTINGS_FOR_TESTING, &nv_value, sizeof(uint8)))
      {
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"Writing NV69731 success, new value = %d,config = %d",nv_value,config);
   
         ret_status = TRUE;
         tdsrrc_desired_config = config;
      }
      else
      {
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"Writing NV69731 failure, NV69731 remains %d,failure config = %d",tdsrrc_special_settings_for_testing_nv,config);

         ret_status = FALSE;
      }
   }
   
   set_rsp.status = ret_status;
   
   msgr_init_hdr(&set_rsp.msg_hdr, MSGR_TDSCDMA_RRC, TDSCDMA_RRC_SET_CONFIG_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
   msgr_set_hdr_variant(&set_rsp.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
   msgr_set_hdr_inst(&set_rsp.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
   status = msgr_send(&set_rsp.msg_hdr, sizeof(tds_rrc_set_config_rsp_type));

   TDSRRC_MSG4(MSG_LEGACY_HIGH,"Send TDSCDMA_RRC_SET_CONFIG_RSP, MSGR status = %d, ret_status = %d, act_cfg = %d, des_cfg = %d", 
               status,
               ret_status,
               tdsrrc_active_config,
               tdsrrc_desired_config);
   
   if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      tdscfa_send_message_to_qmi(&set_rsp.msg_hdr, sizeof(tds_rrc_set_config_rsp_type));
   }
}

/*===========================================================================

FUNCTION tdsrrcnv_get_pseudo_sib19

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_PSEUDO_SIB19 and stores it in 
  rrc global variable 'tdsrrcnv_pseudo_sib19'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcnv_get_pseudo_sib19()
{
   if (tdsrrcnv_efs_read(TDS_RRC_NV_PSEUDO_SIB19, 
                         (byte *) &tdsrrcnv_pseudo_sib19,
                         sizeof(tds_nv_pseudo_sib19_type)))
   {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_PSEUDO_SIB19 is read,enable %d",tdsrrcnv_pseudo_sib19.is_pseudo_sib19_enabled);
   }
   else
   {
      if (!tdsrrcnv_get_default_value(TDS_RRC_NV_PSEUDO_SIB19,
                                      (byte *) &tdsrrcnv_pseudo_sib19,
                                      sizeof(tds_nv_pseudo_sib19_type),
                                      TRUE))
      {
            tdsrrcnv_pseudo_sib19.is_pseudo_sib19_enabled = TRUE; /* enable by default */
            tdsrrcnv_pseudo_sib19.utra_priority = TDSRRCNV_PSEUDO_SIB19_UTRA_PRI;
            tdsrrcnv_pseudo_sib19.s_priority_search1 = TDSRRCNV_PSEUDO_SIB19_S_PRI_SRCH1;
            tdsrrcnv_pseudo_sib19.thresh_serving_low = TDSRRCNV_PSEUDO_SIB19_THR_SERV_LOW;
            tdsrrcnv_pseudo_sib19.eutra_priority = TDSRRCNV_PSEUDO_SIB19_EUTRA_PRI;
            tdsrrcnv_pseudo_sib19.eutra_meas_bandwidth = TDSRRCNV_PSEUDO_SIB19_EUTRA_MEAS_BW;
            tdsrrcnv_pseudo_sib19.eutra_thresh_high = TDSRRCNV_PSEUDO_SIB19_THR_X_HIGH;
            tdsrrcnv_pseudo_sib19.eutra_thresh_low = TDSRRCNV_PSEUDO_SIB19_THR_X_LOW;
            tdsrrcnv_pseudo_sib19.eutra_qrxlevmin = TDSRRCNV_PSEUDO_SIB19_EUTRA_QRXLEVMIN;
            tdsrrcnv_pseudo_sib19.reserved = TDSRRCNV_PSEUDO_SIB19_RESERVE;
         
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"tds_nv_pseudo_sib19_type item error. Setting to default values");
      }

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read failed for TDS_RRC_NV_PSEUDO_SIB19,set default all enabled");
   }

   /* Check the validity of tdsrrcsib_sib_sleep. */
   if (tdsrrcnv_pseudo_sib19.is_pseudo_sib19_enabled)
   {
      boolean rewrite_sleep_nv = FALSE;
      
      if (tdsrrcnv_pseudo_sib19.utra_priority > 7)
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcnv_pseudo_sib19.utra_priority = TDSRRCNV_PSEUDO_SIB19_UTRA_PRI;
      }

      if (tdsrrcnv_pseudo_sib19.s_priority_search1 > 31)
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcnv_pseudo_sib19.s_priority_search1 = TDSRRCNV_PSEUDO_SIB19_S_PRI_SRCH1;
      }

      if (tdsrrcnv_pseudo_sib19.thresh_serving_low > 31)
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcnv_pseudo_sib19.thresh_serving_low = TDSRRCNV_PSEUDO_SIB19_THR_SERV_LOW;
      }

      if (tdsrrcnv_pseudo_sib19.eutra_priority > 7)
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcnv_pseudo_sib19.eutra_priority = TDSRRCNV_PSEUDO_SIB19_EUTRA_PRI;
      }

      if (tdsrrcnv_pseudo_sib19.eutra_meas_bandwidth > 5) /*  tdsrrc_EUTRA_MeasurementBandwidth_mbw100 */
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcnv_pseudo_sib19.eutra_meas_bandwidth = TDSRRCNV_PSEUDO_SIB19_EUTRA_MEAS_BW;
      }

      if (tdsrrcnv_pseudo_sib19.eutra_thresh_high > 31)
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcnv_pseudo_sib19.eutra_thresh_high = TDSRRCNV_PSEUDO_SIB19_THR_X_HIGH;
      }

      if (tdsrrcnv_pseudo_sib19.eutra_thresh_low > 31)
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcnv_pseudo_sib19.eutra_thresh_low = TDSRRCNV_PSEUDO_SIB19_THR_X_LOW;
      }

      if ((tdsrrcnv_pseudo_sib19.eutra_qrxlevmin < -70) || (tdsrrcnv_pseudo_sib19.eutra_qrxlevmin > -22))
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcnv_pseudo_sib19.eutra_qrxlevmin = TDSRRCNV_PSEUDO_SIB19_EUTRA_QRXLEVMIN;
      }

      if (rewrite_sleep_nv)
      {
         boolean del_flag = tdsrrcnv_efs_delete(TDS_RRC_NV_PSEUDO_SIB19);

         TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_PSEUDO_SIB19 Delete the NV status %d",del_flag);

         if (tdsrrcnv_efs_write(TDS_RRC_NV_PSEUDO_SIB19, 
                                (const byte *)&tdsrrcnv_pseudo_sib19, 
                                sizeof(tds_nv_pseudo_sib19_type)))
         {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDS_RRC_NV_PSEUDO_SIB19 written to NV success");
         }
         else
         {
            /* We still keep valid values in tdsrrcsib_sib_sleep */
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDS_RRC_NV_PSEUDO_SIB19 written to NV failed");
         }
      }
   }
}


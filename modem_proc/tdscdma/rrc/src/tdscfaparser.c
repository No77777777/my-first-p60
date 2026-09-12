/*===========================================================================
                        TDSCDMA CFA PARSER

DESCRIPTION

 This module has the interface for CFA parser for TDSCDMA.



EXTERNALIZED FUNCTIONS 


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdscfaparser.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------     
07/13/11   mkh     Moved TDSL1_CPHY_CELL_BAR_REQ to get parsed
06/2811    ps      File created

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "customer.h"
#include "tdsl2dltask.h"
#include "tdsl2ultask.h"
#include "tdsl1mtask.h"
#include "ms.h"
#include "tdsrrclcm.h"
#include "gs.h"
#include "cfa.h"           /* For CFA Logging */
#include "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"

#include "tdsl1rrcif.h"
#include "tdsCFAParser.h"
#include "tdsCFAParser_i.h"
#include "tdsrrcsigs.h"
#include "tdsrrcnv.h"
#include "tdsrrc_rrc_if.h"
#include "tdsrrccspi.h"
#include "tdsrrcrcei.h"
#include "tdsrrcrce.h"
#include "tdsrrcmcm.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcdt.h"
#include "tdsrrccu.h"
#include "tdsrrcllc.h"
#include "tdsrrcdata.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#ifdef TEST_FRAMEWORK
#error code not present
#else
#include "time_svc.h"
#endif

#include "tdsrrcwrm.h"

 /* -----------------------------------------------------------------------
 ** Constant / Define Declarations
 ** ----------------------------------------------------------------------- */
 /*Only 255 bytes can be logeed in CFA 
   * This contains 254 bytes of version filed and the interface msg to be logged
   */
 #define TDSCFA_MSG_LEN 255
 /*First byte is the version field*/
 #define TDSCFA_MSG_VERSION_POS 0

 /* The interface command msg to be logged starts at the offset of 1*/
 #define TDSCFA_MSG_CMD_POS 1
 /*Lenght of interface msg to be logged*/
 #define TDSCFA_MSG_CMD_LEN 254

 /* Maximum number of external command in RRC command history + 1.
  * The extra entry (0, 0, 0) is for last command indication.
  */
 #define TDSRRC_COMMAND_HIST_MAX_ENTRY  63 
 /* Maximum number of external command/event in RRC DSDS command history + 1.
  */
 #define TDSRRC_DSDS_COMMAND_HIST_MAX_ENTRY  256
 #define TDSRRC_COMMAND_HIST_DROP_ALL (0xdeadbeef)
 
 #define TDSRRC_COMMAND_HIST_MM_HIST_MAX_ENTRY 41
 
 /* -----------------------------------------------------------------------
 ** Type Declarations
 ** ----------------------------------------------------------------------- */

typedef struct {
   rfm_device_enum_type    tds_grant;
   trm_reason_enum_t       tds_reason;
   trm_unlock_event_enum_t last_evt;
   rfm_device_enum_type    gsm1_grant;
   trm_reason_enum_t       gsm1_reason;
   rfm_device_enum_type    gsm2_grant;
   trm_reason_enum_t       gsm2_reason;
   rfm_device_enum_type	   irat_grant;
   trm_reason_enum_t 	   irat_reason;
   tdrm_client_state_enum_type tl1;
   tdrm_client_state_enum_type trrc;
   tdrm_client_state_enum_type tdiv;
   tdrm_client_state_enum_type ttx;
} tdsrrc_cmd_hist_trm_status;

typedef struct {
   boolean                            ta_allow;
   tdsrrc_tuneaway_block_reason_type  ta_block_reason;
   tdsrrc_tuneaway_type               ta_type;
   tdsrrc_state_e_type                ta_state;
   uint32                             lta_duration;
   trm_unlock_event_enum_t            tdsrrcwrm_unlock_type;
   boolean                            trm_exch_suc;
#ifdef FEATURE_3GPP_CSFB_TDS
   boolean                            csfb_call;
#endif
} tdsrrc_cmd_hist_ta_status;

typedef struct {
   tdsrrc_state_e_type          rrc;
   tdsrrc_csp_substate_e_type   csp;
   tdsrrc_csp_scan_e_type       csp_scan;
   tdsrrcrce_substate_e_type    rce;
   tdsrrccu_substate_e_type     cu;
   tdsrrcllc_substate_e_type    llc;
   tdsrrcmcm_state_e_type       mcm;
   tdsrrc_mode_e_type           mode;
   boolean                      cs_open;
   boolean                      ps_open;
   tdsl1m_state_enum_type       l1_prev_st;
   tdsl1m_state_enum_type       l1_curr_st;
#ifdef FEATURE_WRLF_SYSTEM_SEL
   boolean                      ue_in_conn_oos;
#endif
#ifdef FEATURE_SGLTE
   sys_ue_mode_e_type           sglte;
#endif
  boolean                      oos_due_to_no_resource;
  boolean                      csp_waiting_for_wl1_to_release_lock;
  boolean                      preemption_handling_in_progress;
  boolean                      set_l1_to_release_lock_when_drop_all;
  sys_modem_dual_standby_pref_e_type  nas_ds_pref;
  sys_modem_device_mode_e_type        nas_dev_mode;
  boolean                       sim_valid;
  sys_modem_as_id_e_type        sim_id;

  tdsrrc_cmd_hist_trm_status    trm;
  tdsrrc_cmd_hist_ta_status     ta;
} tdsrrc_cmd_hist_rrc_states;

typedef struct {
   tds_cfa_queue_type   dest;
   uint32               cmd_id;
   uint32               cmd_addr;
   void               * cmd_dup_ptr;
   uint32               cmd_size;
   uint32               cmd_time;
   uint32               rep_count;
   tdsrrc_cmd_hist_rrc_states rrc_states;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L   
   sys_modem_as_id_e_type as_id;
#endif
} tdsrrc_cmd_entry_type;

typedef struct {
   tdsrrc_cmd_entry_type  cmd_list[TDSRRC_COMMAND_HIST_MAX_ENTRY];
   uint32                 eoc_index;
} tdsrrc_cmd_hist_type;


typedef struct {
   tdsrrc_dsds_event_type dsds_event;
   tds_cfa_queue_type   dest;
   uint32               cmd_id;
   msgr_umid_type       cmd_umid;
   uint32               cmd_time;
     
   tdsrrc_cmd_hist_rrc_states rrc_states;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L   
   sys_modem_as_id_e_type as_id;
#endif
} tdsrrc_dsds_cmd_entry_type;

typedef struct {
   tdsrrc_dsds_cmd_entry_type  cmd_list[TDSRRC_DSDS_COMMAND_HIST_MAX_ENTRY];
   uint32                 eoc_index;
} tdsrrc_dsds_cmd_hist_type;

typedef struct {
   boolean initialized;
   boolean incl_rrc_int;
   boolean incl_cmd_data;
   boolean drop_cmd;
   boolean no_repeat;
} tdsrrc_cmd_hist_settings_type;

typedef struct {
   tds_cfa_queue_type   dest;
   uint32               cmd_id;
} tdsrrc_cmd_hist_cmd_id_type;

typedef struct {
   tdsrrc_cmd_e_type  mm_to_rrc;
   rrc_to_mm_sink_e_type rrc_to_mm;
   uint32                cmd_time;
   tdsrrc_cmd_hist_rrc_states rrc_states;
} tdsrrc_cmd_hist_mm_cmd_type;

typedef struct {
   tdsrrc_cmd_hist_mm_cmd_type cmd_list[TDSRRC_COMMAND_HIST_MM_HIST_MAX_ENTRY];
   uint32                      eoc;
} tdsrrc_cmd_hist_mm_cmd_hist_type;

 /* -----------------------------------------------------------------------
 ** Global Constant Data Declarations
 ** ----------------------------------------------------------------------- */
 
 /* -----------------------------------------------------------------------
 ** Global Data Declarations
 ** ----------------------------------------------------------------------- */
extern trm_unlock_event_enum_t tdsrrcwrm_unlock_type;
extern tdsrrc_ds_tuneaway_status_change_ind_type tds_ds_tuneaway_status_change_ind;
extern boolean tdsrrc_tdrm_exchange_success;
#ifdef FEATURE_3GPP_CSFB_TDS
extern boolean tdsrrc_csfb_call_status;
#endif
 
extern tdsrrc_csp_int_data_type  tdsrrc_csp_int_data;
 
extern tdsl1m_global_type tdsl1m_global;

rex_crit_sect_type            tdsrrccfa_add_entry_cs;
rex_crit_sect_type            tdsrrccfa_dsds_add_entry_cs;

uint32 tdsrrccfa_nv_settings = 0;

static tdsrrc_cmd_hist_settings_type tdsrrccfa_cmd_hist_settings = {FALSE, FALSE, FALSE, FALSE, FALSE};
   
/* Following command will not be captured in command history */
static tdsrrc_cmd_hist_cmd_id_type tdsrrccfa_cmd_hist_cmd_drop[] = {
      {TDSCFA_RRC_Q, TDSRRC_TIMER_FOR_RSSI_BER_EXPIRED_IND},
      {TDSCFA_CM_Q, CM_RRC_STATUS_IND},
      {TDSCFA_L1_Q, TDSL1_CPHY_LSM_TIMETAG_REQ},
      {TDSCFA_L1_Q, TDSL1_CPHY_LSM_CELL_MEAS_REQ},

      /* Make sure this is the last element */
      {TDSCFA_MAX_Q, 0}
};

tdsrrc_cmd_hist_type tdsrrccfa_cmd_hist;

tdsrrc_cmd_hist_mm_cmd_hist_type tdsrrccfa_mm_cmd_hist;
tdsrrc_dsds_cmd_hist_type tdsrrccfa_dsds_cmd_hist;
 /* -----------------------------------------------------------------------
 ** Local Object Definitions
 ** ----------------------------------------------------------------------- */
 
 /* NV settings for TDS_RRC_NV_CFA_CONF 32-bit and the (default values)
      bit 0: reserved for future use
      bit 1: tdsrrccfa_cmd_hist_settings.incl_rrc_int (FALSE)
      bit 2: tdsrrccfa_cmd_hist_settings.incl_cmd_data (TRUE)
      bit 3: tdsrrccfa_cmd_hist_settings.drop_cmd (TRUE)
      bit 4: tdsrrccfa_cmd_hist_settings.no_repeat (TRUE)
      bit 5: reserved for future use
      bit 6: reserved for future use
      bit 7: reserved for future use
      
      bit 8 - 15: unused
      
      bit 16: drop TDSCFA_L2DL_Q command (TRUE)
      bit 17: drop TDSCFA_L2UL_Q command (TRUE)
      bit 18: drop TDSCFA_RRC_Q MAC command 3xxx (FALSE)
      bit 19: drop TDSCFA_RRC_Q RLC command 4xxx (FALSE)
      bit 20: reserved for future use
      bit 21: reserved for future use
      bit 22: reserved for future use
      bit 23: reserved for future use

      bit 24 - 31: unused

      Default value: 0x0003001C
*/

#define TDSRRCCFA_NV_SETTINGS_DEFAULT_VALUE 0x0003001C

#define TDSRRCCFA_NV_MASK_DROP_L2DL 0x00010000
#define TDSRRCCFA_NV_MASK_DROP_L2UL 0x00020000
#define TDSRRCCFA_NV_MASK_DROP_MAC  0x00040000
#define TDSRRCCFA_NV_MASK_DROP_RLC  0x00080000
 
 /* -----------------------------------------------------------------------
 ** Forward Declarations
 ** ----------------------------------------------------------------------- */
 /* Any forward declarations go here */

static void tdsrrc_cmd_hist_free_data(int index);

 
 /* =======================================================================
 **                            Function Definitions
 ** ======================================================================= */
void tdsrrccfa_init()
{
   uint8 i;
   
   /*read integrity and ciphering ENABLE/DISABLE flags from NV*/
   if (tdsrrcnv_efs_read(TDS_RRC_NV_CFA_CONF, 
                         (byte *) &tdsrrccfa_nv_settings,
                         sizeof(tdsrrccfa_nv_settings)))
   {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_CFA_CONF is 0x%x",tdsrrccfa_nv_settings);
   }
   else
   {
      tdsrrccfa_nv_settings = TDSRRCCFA_NV_SETTINGS_DEFAULT_VALUE;
      
      if (tdsrrcnv_efs_write(TDS_RRC_NV_CFA_CONF, 
                             (const byte *)&tdsrrccfa_nv_settings, 
                             sizeof(tdsrrccfa_nv_settings)))
      {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_CFA_CONF written to NV success value 0x%x",tdsrrccfa_nv_settings);
      }
      else
      {
         tdsrrccfa_nv_settings = TDSRRCCFA_NV_SETTINGS_DEFAULT_VALUE;
         TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_CFA_CONF written to NV failed. Set to default 0x%x",tdsrrccfa_nv_settings);
      }
   }

   memset((void *) &tdsrrccfa_cmd_hist, 0, sizeof(tdsrrccfa_cmd_hist));
   memset((void *) &tdsrrccfa_mm_cmd_hist, 0, sizeof(tdsrrccfa_mm_cmd_hist));
   memset((void *) &tdsrrccfa_dsds_cmd_hist, 0, sizeof(tdsrrccfa_dsds_cmd_hist));
   for (i = 0; i < TDSRRC_COMMAND_HIST_MM_HIST_MAX_ENTRY; i++)
   {
      tdsrrccfa_mm_cmd_hist.cmd_list[i].mm_to_rrc = TDSRRC_MM_CMD_BASE;
      tdsrrccfa_mm_cmd_hist.cmd_list[i].rrc_to_mm = RRC_MM_SINK_CMD_MAX;
   }

#ifndef FEATURE_MODEM_RCINIT
   rex_init_crit_sect(&tdsrrccfa_add_entry_cs);
   rex_init_crit_sect(&tdsrrccfa_dsds_add_entry_cs);
#endif
   tdsrrccfa_cmd_hist_settings.initialized = TRUE;
   tdsrrccfa_cmd_hist_settings.incl_rrc_int = (tdsrrccfa_nv_settings & (((uint32) 1) << 1)) ? TRUE : FALSE;
   tdsrrccfa_cmd_hist_settings.incl_cmd_data  = (tdsrrccfa_nv_settings & (((uint32) 1) << 2)) ? TRUE : FALSE;
   tdsrrccfa_cmd_hist_settings.no_repeat = (tdsrrccfa_nv_settings & (((uint32) 1) << 3)) ? TRUE : FALSE;
   tdsrrccfa_cmd_hist_settings.drop_cmd = (tdsrrccfa_nv_settings & (((uint32) 1) << 4)) ? TRUE : FALSE;
}

void tdsrrccfa_free()
{
   int i = 0;

   if (!tdsrrccfa_cmd_hist_settings.initialized)
      return;
   
   if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
   {
      /* Free command data */
      for (i = 0; i < TDSRRC_COMMAND_HIST_MAX_ENTRY; i++)
         tdsrrc_cmd_hist_free_data(i);
   }
   
   memset((void *) &tdsrrccfa_cmd_hist, 0, sizeof(tdsrrccfa_cmd_hist));

   memset((void *) &tdsrrccfa_cmd_hist_settings, 0, sizeof(tdsrrccfa_cmd_hist_settings));
   memset((void *) &tdsrrccfa_dsds_cmd_hist, 0, sizeof(tdsrrccfa_dsds_cmd_hist));
}
 
static void tdsrrc_cmd_hist_fill_up_rrc_states(tdsrrc_cmd_hist_rrc_states * rrc_states_ptr)
{
#ifndef TEST_FRAMEWORK
   trm_get_info_input_type get_info_input; 
   trm_get_info_return_type get_info_output;
#endif

   if (!rrc_states_ptr)
      return;

   rrc_states_ptr->rrc = tdsrrc_get_state();
   rrc_states_ptr->csp = tdsrrccsp_get_csp_substate();
   rrc_states_ptr->csp_scan = tdsrrccsp_get_curr_scan();
   rrc_states_ptr->rce = tdsrrcrce_get_rce_sub_state();
   rrc_states_ptr->cu = tdsrrccu_return_cu_substate();
   rrc_states_ptr->llc = tdsrrcllc_return_current_substate();
   rrc_states_ptr->mcm = tdsrrcmcm_get_substate();
   rrc_states_ptr->mode = tdsrrcmcm_get_rrc_mode();
   rrc_states_ptr->cs_open = tdsrrcdt_is_cs_domain_open();
   rrc_states_ptr->ps_open = tdsrrcdt_is_ps_domain_open();
   rrc_states_ptr->l1_prev_st = tdsl1m_global.l1_prev_state;
   rrc_states_ptr->l1_curr_st = tdsl1m_global.l1_state;
#ifdef FEATURE_WRLF_SYSTEM_SEL
   rrc_states_ptr->ue_in_conn_oos = TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS();
#endif

#ifdef FEATURE_SGLTE
   rrc_states_ptr->sglte = tdsrrc_csp_int_data.ue_mode;
#endif

   rrc_states_ptr->oos_due_to_no_resource = tdsrrc_csp_int_data.oos_due_to_no_resource;
   rrc_states_ptr->csp_waiting_for_wl1_to_release_lock = tdsrrc_csp_int_data.csp_waiting_for_wl1_to_release_lock;
   rrc_states_ptr->preemption_handling_in_progress = tdsrrc_csp_int_data.preemption_handling_in_progress;
   rrc_states_ptr->set_l1_to_release_lock_when_drop_all = tdsrrc_csp_int_data.set_l1_to_release_lock_when_drop_all;
   rrc_states_ptr->trm.last_evt = tdsrrcwrm_unlock_type;

   rrc_states_ptr->nas_ds_pref = tdsrrc_csp_int_data.dual_standby_mode_info;
   rrc_states_ptr->nas_dev_mode = tdsrrc_csp_int_data.device_mode;
   
   rrc_states_ptr->sim_valid = tdsrrc_is_sim_id_valid;
   rrc_states_ptr->sim_id = tdsrrc_sim_id_recvd;

#if !defined(TEST_FRAMEWORK)
   get_info_input.client_id = TRM_TDSCDMA;
   get_info_input.info_type = TRM_CLIENT_INFO;
   get_info_input.input_data.client_info.requested_client_id = TRM_TDSCDMA;

   trm_get_info(&get_info_input, &get_info_output);

   rrc_states_ptr->trm.tds_grant = get_info_output.info.client_info.dev;
   rrc_states_ptr->trm.tds_reason = get_info_output.info.client_info.reason;

   get_info_input.client_id = TRM_TDSCDMA;
   get_info_input.info_type = TRM_CLIENT_INFO;
   get_info_input.input_data.client_info.requested_client_id = TRM_GSM1;

   trm_get_info(&get_info_input, &get_info_output);

   rrc_states_ptr->trm.gsm1_grant = get_info_output.info.client_info.dev;
   rrc_states_ptr->trm.gsm1_reason = get_info_output.info.client_info.reason;

   get_info_input.client_id = TRM_TDSCDMA;
   get_info_input.info_type = TRM_CLIENT_INFO;
   get_info_input.input_data.client_info.requested_client_id = TRM_GSM2;

   trm_get_info(&get_info_input, &get_info_output);

   rrc_states_ptr->trm.gsm2_grant = get_info_output.info.client_info.dev;
   rrc_states_ptr->trm.gsm2_reason = get_info_output.info.client_info.reason;
   
   get_info_input.client_id = TRM_TDSCDMA;
   get_info_input.info_type = TRM_CLIENT_INFO;
   get_info_input.input_data.client_info.requested_client_id = TRM_IRAT;

   trm_get_info(&get_info_input, &get_info_output);

   rrc_states_ptr->trm.irat_grant = get_info_output.info.client_info.dev;
   rrc_states_ptr->trm.irat_reason = get_info_output.info.client_info.reason;
#endif /*#if !defined(TEST_FRAMEWORK)*/

   rrc_states_ptr->trm.tl1 = TDRM_GET_TDRM_CLI_STATE(TDRM_CLIENT_L1);
   rrc_states_ptr->trm.trrc = TDRM_GET_TDRM_CLI_STATE(TDRM_CLIENT_RRC);
   rrc_states_ptr->trm.tdiv = TDRM_GET_TDRM_CLI_STATE(TDRM_CLIENT_DIV);
   rrc_states_ptr->trm.ttx = TDRM_GET_TDRM_CLI_STATE(TDRM_CLIENT_TX);

   rrc_states_ptr->ta.ta_allow = tds_ds_tuneaway_status_change_ind.tuneaway_status;
   rrc_states_ptr->ta.ta_block_reason = ta_block_reason;
   rrc_states_ptr->ta.ta_type = ta_type;
   rrc_states_ptr->ta.ta_state = ta_state;
   rrc_states_ptr->ta.lta_duration = lta_duration;
   rrc_states_ptr->ta.tdsrrcwrm_unlock_type = tdsrrcwrm_unlock_type;
   rrc_states_ptr->ta.trm_exch_suc = tdsrrc_tdrm_exchange_success;
#ifdef FEATURE_3GPP_CSFB_TDS
   rrc_states_ptr->ta.csfb_call = tdsrrc_csfb_call_status;
#endif
}

static void tdsrrc_cmd_hist_add_mm_entry(tdsrrc_cmd_hist_mm_cmd_type * mm_entry_ptr)
{
   if (!mm_entry_ptr)
      return;

   tdsrrccfa_mm_cmd_hist.cmd_list[tdsrrccfa_mm_cmd_hist.eoc] = *mm_entry_ptr;
   tdsrrc_cmd_hist_fill_up_rrc_states(&(tdsrrccfa_mm_cmd_hist.cmd_list[tdsrrccfa_mm_cmd_hist.eoc].rrc_states));
   tdsrrccfa_mm_cmd_hist.eoc++;

   if (tdsrrccfa_mm_cmd_hist.eoc >= TDSRRC_COMMAND_HIST_MM_HIST_MAX_ENTRY)
      tdsrrccfa_mm_cmd_hist.eoc = 0;

   tdsrrccfa_mm_cmd_hist.cmd_list[tdsrrccfa_mm_cmd_hist.eoc].mm_to_rrc = TDSRRC_MM_CMD_BASE;
   tdsrrccfa_mm_cmd_hist.cmd_list[tdsrrccfa_mm_cmd_hist.eoc].rrc_to_mm = RRC_MM_SINK_CMD_MAX;
   tdsrrccfa_mm_cmd_hist.cmd_list[tdsrrccfa_mm_cmd_hist.eoc].cmd_time = 0;
}

static boolean tdsrrc_cmd_hist_is_cmd_to_drop(tds_cfa_queue_type dest, uint32 cmd_id)
{
   int i = 0;
   int len = sizeof(tdsrrccfa_cmd_hist_cmd_drop) / sizeof(tdsrrc_cmd_hist_cmd_id_type);

   if (!tdsrrccfa_cmd_hist_settings.drop_cmd)
      return FALSE;
   
   for (i = 0; i < len; i++)
   {
      if ((tdsrrccfa_cmd_hist_cmd_drop[i].dest == dest &&
           tdsrrccfa_cmd_hist_cmd_drop[i].cmd_id == cmd_id) ||
          (tdsrrccfa_cmd_hist_cmd_drop[i].dest == dest &&
           tdsrrccfa_cmd_hist_cmd_drop[i].cmd_id == TDSRRC_COMMAND_HIST_DROP_ALL)
         )
      {
         return TRUE;
      }
   }

   if ((dest == TDSCFA_L2DL_Q) && (tdsrrccfa_nv_settings & TDSRRCCFA_NV_MASK_DROP_L2DL))
      return TRUE;

   if ((dest == TDSCFA_L2UL_Q) && (tdsrrccfa_nv_settings & TDSRRCCFA_NV_MASK_DROP_L2UL))
      return TRUE;
   
   if ((dest == TDSCFA_RRC_Q) && (tdsrrccfa_nv_settings & TDSRRCCFA_NV_MASK_DROP_MAC))
   {
      if (cmd_id >= TDSRRC_CMD_ID_MAC_PLACEHLDER_BEGIN &&
          cmd_id <= TDSRRC_CMD_ID_MAC_PLACEHLDER_END)
         return TRUE;
   }

   if ((dest == TDSCFA_RRC_Q) && (tdsrrccfa_nv_settings & TDSRRCCFA_NV_MASK_DROP_RLC))
   {
      if (cmd_id >= TDSRRC_CMD_ID_RLC_PLACEHLDER_BEGIN &&
          cmd_id <= TDSRRC_CMD_ID_RLC_PLACEHLDER_END)
         return TRUE;
   }

   return FALSE;
}

static void tdsrrc_cmd_hist_dup_incoming_ext_cmd(tdsrrc_cmd_entry_type * cmd_entry_ptr, void * ext_cmd_ptr)
{
   if (cmd_entry_ptr == NULL || ext_cmd_ptr == NULL)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrc_cmd_hist_dup_incoming_ext_cmd: empty command entry");
      return;
   }

   if (cmd_entry_ptr->cmd_id >= TDSRRC_CMD_ID_L1_PLACEHLDER_BEGIN &&
       cmd_entry_ptr->cmd_id <= TDSRRC_CMD_ID_L1_PLACEHLDER_END)
   {
      cmd_entry_ptr->cmd_size = sizeof(tdsrrc_l1_cmd_type);
      cmd_entry_ptr->cmd_dup_ptr = tdsrrc_malloc(cmd_entry_ptr->cmd_size);
      if (cmd_entry_ptr->cmd_dup_ptr)
         (void) memscpy(cmd_entry_ptr->cmd_dup_ptr,cmd_entry_ptr->cmd_size, ext_cmd_ptr, cmd_entry_ptr->cmd_size);
   }
   else if (cmd_entry_ptr->cmd_id >= TDSRRC_CMD_ID_MAC_PLACEHLDER_BEGIN &&
            cmd_entry_ptr->cmd_id <= TDSRRC_CMD_ID_MAC_PLACEHLDER_END)
   {
      cmd_entry_ptr->cmd_size = sizeof(tdsrrc_mac_cmd_type);
      cmd_entry_ptr->cmd_dup_ptr = tdsrrc_malloc(cmd_entry_ptr->cmd_size);
      if (cmd_entry_ptr->cmd_dup_ptr)
               (void) memscpy(cmd_entry_ptr->cmd_dup_ptr, cmd_entry_ptr->cmd_size,ext_cmd_ptr, cmd_entry_ptr->cmd_size);
   }
   else if (cmd_entry_ptr->cmd_id >= TDSRRC_CMD_ID_RLC_PLACEHLDER_BEGIN &&
            cmd_entry_ptr->cmd_id <= TDSRRC_CMD_ID_RLC_PLACEHLDER_END)
   {
      cmd_entry_ptr->cmd_size = sizeof(tdsrrc_rlc_cmd_type);
      cmd_entry_ptr->cmd_dup_ptr = tdsrrc_malloc(cmd_entry_ptr->cmd_size);
      if (cmd_entry_ptr->cmd_dup_ptr)
               (void) memscpy(cmd_entry_ptr->cmd_dup_ptr,cmd_entry_ptr->cmd_size, ext_cmd_ptr, cmd_entry_ptr->cmd_size);
   }
   else if (cmd_entry_ptr->cmd_id >= TDSRRC_CMD_ID_MM_PLACEHLDER_BEGIN &&
            cmd_entry_ptr->cmd_id <= 0x05FFFFFF)
   {
      cmd_entry_ptr->cmd_size = sizeof(tdsrrc_mm_cmd_type);
      cmd_entry_ptr->cmd_dup_ptr = tdsrrc_malloc(cmd_entry_ptr->cmd_size);
      if (cmd_entry_ptr->cmd_dup_ptr)
               (void) memscpy(cmd_entry_ptr->cmd_dup_ptr,cmd_entry_ptr->cmd_size,ext_cmd_ptr, cmd_entry_ptr->cmd_size);
   }
   else if (cmd_entry_ptr->cmd_id >= 0x06000000 &&
            cmd_entry_ptr->cmd_id <= 0x06FFFFFF)
   {
      cmd_entry_ptr->cmd_size = sizeof(tdsrrc_tc_cmd_type);
      cmd_entry_ptr->cmd_dup_ptr = tdsrrc_malloc(cmd_entry_ptr->cmd_size);
      if (cmd_entry_ptr->cmd_dup_ptr)
               (void) memscpy(cmd_entry_ptr->cmd_dup_ptr, cmd_entry_ptr->cmd_size,ext_cmd_ptr, cmd_entry_ptr->cmd_size);
   }
   else if (cmd_entry_ptr->cmd_id >= 0x07000000 &&
            cmd_entry_ptr->cmd_id <= TDSRRC_CMD_ID_RR_PLACEHLDER_END)
   {
      cmd_entry_ptr->cmd_size = sizeof(tdsrrc_rr_cmd_type);
      cmd_entry_ptr->cmd_dup_ptr = tdsrrc_malloc(cmd_entry_ptr->cmd_size);
      if (cmd_entry_ptr->cmd_dup_ptr)
               (void) memscpy(cmd_entry_ptr->cmd_dup_ptr, cmd_entry_ptr->cmd_size,ext_cmd_ptr, cmd_entry_ptr->cmd_size);
   }
   else if (cmd_entry_ptr->cmd_id == TDSRRC_MSGR_CMD)
   {
      cmd_entry_ptr->cmd_size = sizeof(tds_rrc_msgr_cmd_type);
      cmd_entry_ptr->cmd_dup_ptr = tdsrrc_malloc(cmd_entry_ptr->cmd_size);
      if (cmd_entry_ptr->cmd_dup_ptr)
               (void) memscpy(cmd_entry_ptr->cmd_dup_ptr, 
                              cmd_entry_ptr->cmd_size,
                             &(((tdsrrc_cmd_type *) ext_cmd_ptr)->cmd.tds_rrc_msgr_cmd), 
                             cmd_entry_ptr->cmd_size);

   }
   else if (cmd_entry_ptr->cmd_id >= TDSRRC_CMD_ID_WCDMA_RRC_PLACEHLDER_BEGIN &&
            cmd_entry_ptr->cmd_id <= TDSRRC_CMD_ID_WCDMA_RRC_PLACEHLDER_END)
   {
      cmd_entry_ptr->cmd_size = sizeof(tdsrrc_rrc_cmd_type);
      cmd_entry_ptr->cmd_dup_ptr = tdsrrc_malloc(cmd_entry_ptr->cmd_size);
      if (cmd_entry_ptr->cmd_dup_ptr)
               (void) memscpy(cmd_entry_ptr->cmd_dup_ptr, cmd_entry_ptr->cmd_size,ext_cmd_ptr, cmd_entry_ptr->cmd_size);
   }
   else
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"tdsrrc_cmd_hist_dup_incoming_ext_cmd: cmd_id 0x%x not yet supported",cmd_entry_ptr->cmd_id);   
   }
}

/* !!!Should only be called by tdsrrccfa_free() and tdsrrc_cmd_hist_add_entry ()!!! */
static void tdsrrc_cmd_hist_free_data(int index)
{
   if (index < 0 || index >= TDSRRC_COMMAND_HIST_MAX_ENTRY)
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"tdsrrc_cmd_hist_free_data: invalid index %d",index);
      return;
   }

   if (tdsrrccfa_cmd_hist.cmd_list[index].cmd_dup_ptr == NULL ||
       tdsrrccfa_cmd_hist.cmd_list[index].cmd_size == 0)
      return;

   if (tdsrrccfa_cmd_hist.cmd_list[index].dest > TDSCFA_UNKNOWN_Q &&
       tdsrrccfa_cmd_hist.cmd_list[index].dest < TDSCFA_MAX_Q)
   {
         /* SDU, NAS data, Meas, internal data etc. that are inside 
          * command buffer should have be freed along with original command. 
          */
         tdsrrc_free(tdsrrccfa_cmd_hist.cmd_list[index].cmd_dup_ptr);
         tdsrrccfa_cmd_hist.cmd_list[index].cmd_dup_ptr = NULL;
         tdsrrccfa_cmd_hist.cmd_list[index].cmd_size = 0;
   }
}
 
static void tdsrrc_cmd_hist_add_entry(tdsrrc_cmd_entry_type * cmd_entry_ptr)
{
   uint32 last_ndx = 0;

   REX_ISR_LOCK(&tdsrrccfa_add_entry_cs);
   
   last_ndx = tdsrrccfa_cmd_hist.eoc_index ? tdsrrccfa_cmd_hist.eoc_index : TDSRRC_COMMAND_HIST_MAX_ENTRY;
   
   if (cmd_entry_ptr == NULL)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrc_cmd_hist_add_entry: empty command entry");
   }
   else
   {
      last_ndx--;

      if (tdsrrc_cmd_hist_is_cmd_to_drop(cmd_entry_ptr->dest, cmd_entry_ptr->cmd_id))
      {
         /* Free dup command data */
         if (tdsrrccfa_cmd_hist_settings.incl_cmd_data && 
             (cmd_entry_ptr->cmd_dup_ptr != NULL && cmd_entry_ptr->cmd_size != 0))
         {
            tdsrrc_free(cmd_entry_ptr->cmd_dup_ptr);
            cmd_entry_ptr->cmd_dup_ptr = NULL;
            cmd_entry_ptr->cmd_size =0;
         }
      }
      else
      {
#ifdef TEST_FRAMEWORK
         #error code not present
#else
         {
            time_type up_ms;
            uint32 lo32;
            
            time_get_uptime_ms(up_ms);

            lo32 = qw_lo(up_ms);
           
            
            /* Take only the low 32-bit. 
             * Enough for the uptime differentiation.
             */
            cmd_entry_ptr->cmd_time = lo32;
         }
#endif

         {
            tdsrrc_cmd_hist_mm_cmd_type mm_entry;
            
            if (cmd_entry_ptr->dest == TDSCFA_MM_Q)
            {
               mm_entry.mm_to_rrc = TDSRRC_MM_CMD_BASE;
               mm_entry.rrc_to_mm = cmd_entry_ptr->cmd_id;
               mm_entry.cmd_time = cmd_entry_ptr->cmd_time;

               tdsrrc_cmd_hist_add_mm_entry(&mm_entry);
            }
            else if ((cmd_entry_ptr->dest == TDSCFA_RRC_Q)
                     && (cmd_entry_ptr->cmd_id >= TDSRRC_CMD_ID_MM_PLACEHLDER_BEGIN && cmd_entry_ptr->cmd_id <= 0x05FFFFFF))
            {
               mm_entry.mm_to_rrc = cmd_entry_ptr->cmd_id;
               mm_entry.rrc_to_mm = RRC_MM_SINK_CMD_MAX;
               mm_entry.cmd_time = cmd_entry_ptr->cmd_time;

               tdsrrc_cmd_hist_add_mm_entry(&mm_entry);
            }
         }

         if (tdsrrccfa_cmd_hist_settings.no_repeat
             && ((cmd_entry_ptr->dest == tdsrrccfa_cmd_hist.cmd_list[last_ndx].dest) &&
                 (cmd_entry_ptr->cmd_id == tdsrrccfa_cmd_hist.cmd_list[last_ndx].cmd_id))
             && ((cmd_entry_ptr->dest != TDSCFA_L1_Q) ||
                 ((cmd_entry_ptr->dest == TDSCFA_L1_Q) && (cmd_entry_ptr->cmd_id == TDSL1_CPHY_SIB_SCHED_CHANGE_IND))))
         {   
            /* Only the cmd_dup_ptr is freed. No other member has changed */
            if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
               tdsrrc_cmd_hist_free_data(last_ndx);
            
            /* Overwirte the last same command */
            tdsrrccfa_cmd_hist.cmd_list[last_ndx].cmd_addr = cmd_entry_ptr->cmd_addr;
            tdsrrccfa_cmd_hist.cmd_list[last_ndx].cmd_dup_ptr = cmd_entry_ptr->cmd_dup_ptr;
            tdsrrccfa_cmd_hist.cmd_list[last_ndx].cmd_time = cmd_entry_ptr->cmd_time; 
            tdsrrccfa_cmd_hist.cmd_list[last_ndx].cmd_size = cmd_entry_ptr->cmd_size;

            tdsrrccfa_cmd_hist.cmd_list[last_ndx].rep_count++;

            tdsrrc_cmd_hist_fill_up_rrc_states(&(tdsrrccfa_cmd_hist.cmd_list[last_ndx].rrc_states));
         }
         else
         {
            tdsrrccfa_cmd_hist.cmd_list[tdsrrccfa_cmd_hist.eoc_index] = *cmd_entry_ptr;
            tdsrrc_cmd_hist_fill_up_rrc_states(&(tdsrrccfa_cmd_hist.cmd_list[tdsrrccfa_cmd_hist.eoc_index].rrc_states));
            tdsrrccfa_cmd_hist.eoc_index++;

            if (tdsrrccfa_cmd_hist.eoc_index >= TDSRRC_COMMAND_HIST_MAX_ENTRY)
               tdsrrccfa_cmd_hist.eoc_index = 0;

            if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
               tdsrrc_cmd_hist_free_data(tdsrrccfa_cmd_hist.eoc_index);

            memset((void *) &tdsrrccfa_cmd_hist.cmd_list[tdsrrccfa_cmd_hist.eoc_index],
                   0,
                   sizeof(tdsrrc_cmd_entry_type));
         }
      }
   }

   REX_ISR_UNLOCK(&tdsrrccfa_add_entry_cs);

}

static void tdsrrc_dsds_cmd_hist_add_entry(tdsrrc_dsds_cmd_entry_type * cmd_entry_ptr)
{
   uint32 last_ndx = 0;

   REX_ISR_LOCK(&tdsrrccfa_dsds_add_entry_cs);
   
   last_ndx = tdsrrccfa_dsds_cmd_hist.eoc_index ? tdsrrccfa_dsds_cmd_hist.eoc_index : TDSRRC_DSDS_COMMAND_HIST_MAX_ENTRY;
   
   if (cmd_entry_ptr->dsds_event == TDSRRC_DSDS_EVENT_NONE)
   {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"tdsrrc_dsds_cmd_hist_add_entry: none DSDS event to add");
   }
   else
   {
      last_ndx--;
#ifdef TEST_FRAMEWORK
      #error code not present
#else
      {
         time_type up_ms;
         uint32 lo32;
         
         time_get_uptime_ms(up_ms);
         lo32 = qw_lo(up_ms);
     
         /* Take only the low 32-bit. 
          * Enough for the uptime differentiation.
          */
         cmd_entry_ptr->cmd_time = lo32;
      }
#endif
      if ((cmd_entry_ptr->dsds_event == TDSRRC_DSDS_EXTERNAL_COMMAND) || (cmd_entry_ptr->dsds_event == TDSRRC_DSDS_INTERNAL_COMMAND))
      {
        if (!(tdsrrc_cmd_hist_is_cmd_to_drop(cmd_entry_ptr->dest, cmd_entry_ptr->cmd_id)))
        {
           if (tdsrrccfa_cmd_hist_settings.no_repeat
               && ((cmd_entry_ptr->dest == tdsrrccfa_dsds_cmd_hist.cmd_list[last_ndx].dest) &&
                   (cmd_entry_ptr->cmd_id == tdsrrccfa_dsds_cmd_hist.cmd_list[last_ndx].cmd_id))
               && ((cmd_entry_ptr->dest != TDSCFA_L1_Q) ||
                   ((cmd_entry_ptr->dest == TDSCFA_L1_Q) && (cmd_entry_ptr->cmd_id == TDSL1_CPHY_SIB_SCHED_CHANGE_IND))))
           {   
              /* Overwirte the last same command */
              tdsrrccfa_dsds_cmd_hist.cmd_list[last_ndx].cmd_time = cmd_entry_ptr->cmd_time;      


              tdsrrc_cmd_hist_fill_up_rrc_states(&(tdsrrccfa_dsds_cmd_hist.cmd_list[last_ndx].rrc_states));
           }
           else
           {
              tdsrrccfa_dsds_cmd_hist.cmd_list[tdsrrccfa_dsds_cmd_hist.eoc_index] = *cmd_entry_ptr;
              tdsrrc_cmd_hist_fill_up_rrc_states(&(tdsrrccfa_dsds_cmd_hist.cmd_list[tdsrrccfa_dsds_cmd_hist.eoc_index].rrc_states));
              tdsrrccfa_dsds_cmd_hist.eoc_index++;

              if (tdsrrccfa_dsds_cmd_hist.eoc_index >= TDSRRC_DSDS_COMMAND_HIST_MAX_ENTRY)
              {
                 tdsrrccfa_dsds_cmd_hist.eoc_index = 0;
              }
              memset((void *) &tdsrrccfa_dsds_cmd_hist.cmd_list[tdsrrccfa_dsds_cmd_hist.eoc_index],
                     0,
                     sizeof(tdsrrc_dsds_cmd_entry_type));
           }
        }
      }
      else      
      {
        tdsrrccfa_dsds_cmd_hist.cmd_list[tdsrrccfa_dsds_cmd_hist.eoc_index] = *cmd_entry_ptr;
        tdsrrc_cmd_hist_fill_up_rrc_states(&(tdsrrccfa_dsds_cmd_hist.cmd_list[tdsrrccfa_dsds_cmd_hist.eoc_index].rrc_states));
        tdsrrccfa_dsds_cmd_hist.eoc_index++;
        if (tdsrrccfa_dsds_cmd_hist.eoc_index >= TDSRRC_DSDS_COMMAND_HIST_MAX_ENTRY)
        {
            tdsrrccfa_dsds_cmd_hist.eoc_index = 0;
        }
        memset((void *) &tdsrrccfa_dsds_cmd_hist.cmd_list[tdsrrccfa_dsds_cmd_hist.eoc_index],
                0,
                sizeof(tdsrrc_dsds_cmd_entry_type));
      }
   }
   REX_ISR_UNLOCK(&tdsrrccfa_dsds_add_entry_cs);
}
static void tdscfa_send_message_to_msgr
(
   tds_cfa_queue_type     dest,
   msgr_hdr_struct_type * message_ptr,
   uint32                 length
)
{
   tdsrrc_cmd_entry_type cmd_entry;
   tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
   msgr_umid_type umid;
   
   if ((!message_ptr) || (!length))
      return;

   umid = message_ptr->id;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L   
   MSGR_CLEAR_VARIANT(umid);
#endif

   memset((void *) &cmd_entry, 0, sizeof(tdsrrc_cmd_entry_type));
      
   cmd_entry.dest = dest;
   cmd_entry.cmd_id = (uint32)umid;
   cmd_entry.cmd_addr = (uint32) message_ptr;
   memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));
   dsds_cmd_entry.dest = dest;
   dsds_cmd_entry.cmd_id = (uint32)umid;

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
   cmd_entry.as_id = (sys_modem_as_id_e_type)MSGR_GET_VARIANT(message_ptr);
   dsds_cmd_entry.as_id = cmd_entry.as_id;
#endif

   if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
   {
      /* Use RRC malloc anyway */
      cmd_entry.cmd_dup_ptr = tdsrrc_malloc(length);
      if (cmd_entry.cmd_dup_ptr)
      {
         cmd_entry.cmd_size = length;
         (void) memscpy(cmd_entry.cmd_dup_ptr,cmd_entry.cmd_size,(void *)message_ptr, cmd_entry.cmd_size);

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
         MSGR_CLEAR_VARIANT(((msgr_hdr_struct_type *)cmd_entry.cmd_dup_ptr)->id);
#endif
      }
   }

   /* Add to command history */
   tdsrrc_cmd_hist_add_entry(&cmd_entry);
   dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
   tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
}
 
 /*===========================================================================
 
 FUNCTION tdsl2ul_parse_cfa
 
 DESCRIPTION
 
   Parses  CFA for L2 Ul msg sent to l2 Ul module
 
 
 RETURN VALUE
 
   None.
 
 SIDE EFFECTS
 
   None.
 
 ===========================================================================*/
#if 0
static void tdsl2ul_parse_cfa
(
  tdsl2_ul_cmd_type    *cmd_ptr
)
{
  byte tdscfa_buff[TDSCFA_MSG_LEN];
  
  switch(cmd_ptr->cmd_hdr.cmd_id)
  
  {
    /*MAC RRC*/
    case TDSCMAC_UL_CONFIG_REQ:         /* Configure UL MAC */
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
    case TDSMAC_UL_PERSISTENCE_VAL_CHANGE_IND: /* Update Persistence value */
#endif    
    
    /* Configure UL MAC traffic volume measurements*/
    case TDSCMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ:
    
    /* RRC can request MAC to stop transmission on DTCH using this command*/
    case TDSCMAC_UL_BLOCK_RACH_TRAFFIC_REQ:
    
    case TDSCMAC_UL_TFC_SUBSET_CFG_REQ:
    
      /*First  byte of msg will have version field*/
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
      
      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), TDSCFA_MSG_CMD_LEN);
      cfa_log_packet_ex( GS_QUEUE_TDSRRC,                   /* QUEUE */
                     MS_TDS_MACUL,               /* the message set */
                     (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                     TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                     tdscfa_buff );       /* Pointer to the message */    
      break;
    /*RRC RLC*/
    
    case TDSCRLC_UL_TM_CONFIG_REQ:      /* Configure UL TM RLC */
    
    case TDSCRLC_UL_UM_CONFIG_REQ:      /* Configure UL UM RLC */
    
    case TDSCRLC_AM_CONFIG_REQ:         /* Configure AM RLC */
    
    case TDSCRLC_AM_RESET_REQ:          /* RESET AM RLC */
    
    case TDSCRLC_UL_CIPHER_CONFIG_REQ:  /* Configure Ciphering on DL RLC*/
    
    case TDSCRLC_UL_RESUME_REQ:         /* Resume transmission on RLC channels*/
    
    case TDSCRLC_RELEASE_ALL_REQ:       /* Release all non-MBMS RLC entities.  */
    
    
    case TDSCRLC_UL_REG_FOR_DATA_IND_REQ: /* Register incoming data indication. */
    
    case TDSCRLC_UL_UNREG_FOR_DATA_IND_REQ: /* Register incoming data indication. */
    case TDSRLC_REGISTER_AM_SDU_CONFIRM_CALLBACK_REQ: /* Request for SDU confirm    */
                                          /* callback function for RRC  */
    case TDSRLC_UL_PURGE_WM_REQ:            /* Command to purge the uplink WM */
    
    case TDSCRLC_UL_ABORT_CIPHER_CONFIG_REQ: /* Command to abort the new UL ciphering
                                    configuration                                */
    case TDSCRLC_UL_CONTINUE_REQ:    /* Continue transmission on Stopped RLC channels*/
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
      
      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), TDSCFA_MSG_CMD_LEN);
      cfa_log_packet_ex( GS_QUEUE_TDSRRC,                   /* QUEUE */
                     MS_TDS_RLCUL,               /* the message set */
                     (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                     TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                     tdscfa_buff );       /* Pointer to the message */    
      break;
      
    case TDSRLC_UL_REGISTER_SRVC_REQ:   /* Request for service registration for RRC */
    
      switch(cmd_ptr->cmd_data.ul_reg.rlc_data[0].lc_id)
      {
        case   TDSRRCLCM_UL_TM_START_IDX:/* --CCCH*/
        case    TDSRRCLCM_UL_UM_START_IDX://-- SBr 1
        case    TDSRRCLCM_UL_AM_DATA_START_IDX://srb2
        case    (TDSRRCLCM_UL_AM_DATA_START_IDX +1): //srb 3
        case    (TDSRRCLCM_UL_AM_DATA_START_IDX+2)://srb4
          tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
        
          memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), TDSCFA_MSG_CMD_LEN);
          cfa_log_packet_ex( GS_QUEUE_TDSRRC,                   /* QUEUE */
                         MS_TDS_RLCUL,               /* the message set */
                         (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                         TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                         tdscfa_buff );       /* Pointer to the message */    
        break;
        default:
         tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
        
         memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), TDSCFA_MSG_CMD_LEN);
             cfa_log_packet_ex( GS_QUEUE_TDSOTHER,                   /* QUEUE */
                         MS_TDS_RLCUL,               /* the message set */
                         (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                         TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                         tdscfa_buff );       /* Pointer to the message */    
        break;
      }
    
    /* RLC Other*/
    case TDSRLC_UL_DEREGISTER_SRVC_REQ:   /* DeRequest for service registration for RRC */
    
    case TDSRLC_OPEN_RX_WIN:            /* Open the Receiving Window. */
    
    case TDSRLC_CLOSE_RX_WIN:           /* Close the Receiving Window.  */
    
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
      
      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), TDSCFA_MSG_CMD_LEN);
      cfa_log_packet_ex( GS_QUEUE_TDSOTHER,                   /* QUEUE */
                     MS_TDS_RLCUL,               /* the message set */
                     (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                     TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                     tdscfa_buff );       /* Pointer to the message */    
    break;
    /**RLC  RLC***/
    case TDSRLCI_UL_RESET_REQ:          /* RESET request for UL AM RLC  */
    
    case TDSRLCI_DL_CONFIG_CNF:         /* Confirm the completion of DL config  */
    
    case TDSRLCI_DL_RESET_DONE:         /* Confirm the completion of the DL RESET */
    
    
    case TDSRLCI_UL_TX_AM_STATUS:             /* command to indicate UL to send status PDU */
    
    case TDSRLCI_UL_DUP_RESET_REQ:      /* command to indicate duplicate RESET reception on DL*/
    
    
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
      
      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), TDSCFA_MSG_CMD_LEN);
      cfa_log_packet_ex( GS_QUEUE_TDSRLCDL,                   /* QUEUE */
                     MS_TDS_RLCUL,               /* the message set */
                     (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                     TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                     tdscfa_buff );       /* Pointer to the message */    
    break;
    
    
    default:
    break;
  }
  
}
#endif
 /*===========================================================================
 
 FUNCTION tdsl2dl_parse_cfa
 
 DESCRIPTION
 
 Parses  CFA for L2 Dl msg sent to l2 Dl module

 
 
 
 RETURN VALUE
 
   None.
 
 SIDE EFFECTS
 
   None.
 
 ===========================================================================*/
#if 0
static void tdsl2dl_parse_cfa
(

  tdsl2_dl_cmd_type    *cmd_ptr
    /* pointer to command to be enqueued */
)
{
  byte tdscfa_buff[TDSCFA_MSG_LEN];
  byte tdscfa_cmd_len = 0;

  memset(tdscfa_buff,0,TDSCFA_MSG_LEN);
  tdscfa_cmd_len = (byte)(sizeof(tdsl2_dl_cmd_data_type));
  tdscfa_cmd_len = (tdscfa_cmd_len < TDSCFA_MSG_CMD_LEN) ? tdscfa_cmd_len : TDSCFA_MSG_CMD_LEN;
  
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
  
  /*RLC->RLC*/
    case TDSRLCI_DL_TX_AM_STATUS:     /* Copy nak_list_q (DL) to tx_nak_q (UL) */
    case TDSRLC_HANDLE_SDU_DISCARD_TIMEOUT_EVENT: /* Command to handle SDU discard timer */
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
  
      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), tdscfa_cmd_len);
      cfa_log_packet_ex( GS_QUEUE_TDSRLCDL,                   /* QUEUE */
                       MS_TDS_RLCDL,               /* the message set */
                       (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                       TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                       tdscfa_buff );       /* Pointer to the message */    

      break;
    case TDSRLCI_DL_RESET_REQ:        /* RESET request for DL AM RLC  */
    case TDSRLCI_DL_CONFIG_REQ:       /* Configure DL AM RLC */
    case TDSRLCI_DL_RELEASE_ALL_REQ:  /* Release All non-MBMS request to the Downlink */
    case TDSRLCI_UL_RESET_DONE:       /* Confirm the RESET completion of UL AM RLC  */
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
  
      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), tdscfa_cmd_len);
      cfa_log_packet_ex( GS_QUEUE_TDSRLCUL,                   /* QUEUE */
                       MS_TDS_RLCDL,               /* the message set */
                       (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                       TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                       tdscfa_buff );       /* Pointer to the message */    
      break;
    /*MAC RRC*/
    case TDSCMAC_DL_CONFIG_REQ:       /* Configure DL MAC */
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
      
      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), tdscfa_cmd_len);
      cfa_log_packet_ex( GS_QUEUE_TDSRRC,                   /* QUEUE */
                       MS_TDS_MACDL,               /* the message set */
                       (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                       TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                       tdscfa_buff );       /* Pointer to the message */   
      break;
  /*RLC RRC*/
    case TDSCRLC_DL_TM_CONFIG_REQ:    /* Configure DL TM RLC */
  
    case TDSCRLC_DL_UM_CONFIG_REQ:    /* Configure DL UM RLC */

    case TDSCRLC_DL_CONTINUE_REQ:      /*Continue the stopped RBs in UM mode*/
  
    case TDSCRLC_DL_CIPHER_CONFIG_REQ:/* Configure Cophering on DL RLC*/
  
    case TDSCRLC_DL_ABORT_CIPHER_CONFIG_REQ:          /* Request to abort the new DL
                                                   ciphering configuration         */
  
    case TDSRLC_REGISTER_AM_SDU_DISCARD_CALLBACK_REQ: /* Request for registration of SDU  */
                                              /* discard callback function for    */
                                              /* RRC                              */
  
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
      
      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), tdscfa_cmd_len);
      cfa_log_packet_ex( GS_QUEUE_TDSRRC,                   /* QUEUE */
                       MS_TDS_RLCDL,               /* the message set */
                       (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                       TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                       tdscfa_buff );       /* Pointer to the message */   
      break;

   
    case TDSRLC_DL_REGISTER_SRVC_REQ: /* Register Downlink Watermark */
      if(cmd_ptr->cmd_data.dl_reg.rlc_data[0].rlc_post_rx_proc_func_ptr == tdsrrclcm_post_event_for_dl_sdu)
      {
        tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
       
        memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd_data), tdscfa_cmd_len);
        cfa_log_packet_ex( GS_QUEUE_TDSRRC,                   /* QUEUE */
                        MS_TDS_RLCDL,               /* the message set */
                        (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                        TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                        tdscfa_buff );       /* Pointer to the message */   
 
      }
      else
      {
      
        cfa_log_packet_ex( GS_QUEUE_TDSOTHER,                   /* QUEUE */
                       MS_TDS_RLCDL,               /* the message set */
                       (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                       TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                       tdscfa_buff );       /* Pointer to the message */   

      }
      break;  
    
  /*MAC HS to RLC*/
    case TDSRLC_PROCESS_MAC_HS_RESET_REQ:
     
      cfa_log_packet_ex( GS_QUEUE_TDSMACHS,                   /* QUEUE */
                       MS_TDS_RLCDL,               /* the message set */
                       (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                       TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                       tdscfa_buff );       /* Pointer to the message */   
      break;
  
    /*Data to rlc*/
  
    case TDSRLC_PDCP_CLEANUP_WM:
  
    case TDSRLC_DL_DEREGISTER_SRVC_REQ: /* DeRegister Downlink Watermark */
    
      cfa_log_packet_ex( GS_QUEUE_TDSOTHER,                   /* QUEUE */
                       MS_TDS_RLCDL,               /* the message set */
                       (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                       TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                       tdscfa_buff );       /* Pointer to the message */   
      break;
  /*Not handled by CFA*/
    case TDSL2_DL_MAX_CMDS:            /* Number of DL L2 Command types. must be last entry */
    default:
      break;
  }
}
#endif
/*===========================================================================

FUNCTION tdsl1_prase_cfa

DESCRIPTION

Parses  CFA for L1 msg sent to L1 module



RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
#if 0
static void tdsl1_parse_cfa
(
  tdsl1_ext_cmd_type* cmd_ptr
)
{
  byte tdscfa_buff[TDSCFA_MSG_LEN];
  byte tdscfa_cmd_len = 0;

  memset(tdscfa_buff,0,TDSCFA_MSG_LEN);
  tdscfa_cmd_len = (byte)sizeof(tdsl1_cmd_union_type);
  tdscfa_cmd_len = (tdscfa_cmd_len < TDSCFA_MSG_CMD_LEN) ? tdscfa_cmd_len : TDSCFA_MSG_CMD_LEN;
   
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    /* Commands from RRC */
    case TDSL1_CPHY_IDLE_REQ:     
    case TDSL1_CPHY_DEACTIVATE_REQ:
    case TDSL1_CPHY_STOP_TDSCDMA_MODE_REQ:
    case TDSL1_CPHY_START_TDSCDMA_MODE_REQ:
    case TDSL1_CPHY_SUSPEND_TDSCDMA_MODE_REQ:
    case TDSL1_CPHY_RESUME_TDSCDMA_MODE_REQ:
    case TDSL1_CPHY_FREQ_SCAN_REQ:
    case TDSL1_CPHY_ACQ_REQ:
    case TDSL1_CPHY_CELL_PARM_REQ:
    case TDSL1_CPHY_SETUP_REQ:
    case TDSL1_CPHY_DRX_REQ:
    case TDSL1_CPHY_CIPHERING_KEY_REQ:
    case TDSL1_CPHY_MEASUREMENT_REQ:
    case TDSL1_CPHY_CELL_SELECTION_REQ:
    case TDSL1_CPHY_CELL_TRANSITION_REQ:
    case TDSL1_CPHY_ACT_TIME_CANCEL_REQ:
    case TDSL1_CPHY_UPD_OTA_PARMS_IND:
    case TDSL1_CPHY_UL_PHY_CHANNEL_CTRL_REQ:
    case TDSL1_CPHY_SIB_SCHED_CHANGE_IND:
    case TDSL1_CPHY_GO_TO_SLEEP_IND:
    case TDSL1_CPHY_CELL_BAR_REQ:
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;

      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), tdscfa_cmd_len);
      cfa_log_packet_ex( GS_QUEUE_TDSRRC,                   /* QUEUE */
                        MS_TDS_L1,               /* the message set */
                        (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                        TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                        tdscfa_buff );       /* Pointer to the message */    
      break;
    
    
    /* Commands from MAC */
    case TDSL1_CPHY_UL_TX_CONTROL_REQ:
    case TDSL1_CPHY_MAC_UL_CONFIG_CNF:
    case TDSL1_CPHY_MAC_DL_CONFIG_CNF:
    case TDSL1_CPHY_MAC_DL_CONFIG_AT_L1_FAIL_CNF:
      tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
      
      memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), tdscfa_cmd_len);
      cfa_log_packet_ex( GS_QUEUE_TDSMACUL,                   /* QUEUE */
                        MS_TDS_L1,               /* the message set */
                        (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                        TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                        tdscfa_buff );       /* Pointer to the message */    
      
      break;
    
    /*Msg not handled by CFA at present*/
    /* Commands from LSM (Location Services Manager) */
    case TDSL1_CPHY_LSM_TIMETAG_REQ:
    case TDSL1_CPHY_LSM_CELL_MEAS_REQ:
    case TDSL1_CPHY_UPD_DL_SYNC_PARMS:
    /* MBMS related command and indication */
    /* ----------------------------------- */
    
    /* MCCH DRX request */
    case TDSL1_CPHY_MBMS_MCCH_DRX_REQ:
    /* MBMS IDLE request */
    case TDSL1_CPHY_MBMS_IDLE_REQ:
    /* MBMS Preferred layer (FLC) request */
    case TDSL1_CPHY_MBMS_PL_PARM_REQ:
    
    
    /* Number of External Commands. Should be the last element.*/
    case TDSL1_CPHY_NUM_EXT_REQ_CMDS :
    default:
      break;
  }
/* Enable CFA logging when commands are supported by CFA library */
 
}
#endif

/*===========================================================================

FUNCTION tdsrrc_parse_cfa

DESCRIPTION

Parses  CFA for RRC msg sent to RRC module



RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
#if 0
static void tdsrrc_parse_cfa
(
    
      tdsrrc_cmd_type    *cmd_ptr

)
{
  byte tdscfa_buff[TDSCFA_MSG_LEN];
  if(cmd_ptr->cmd_hdr.cmd_id >= TDSRRC_LCS_CMD_BASE)
  {
   //no cfa for lsm right now
  }
  else if(cmd_ptr->cmd_hdr.cmd_id >= TDSRRC_RR_CMD_ID_BASE)
  {
    
    tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
               
    memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), TDSCFA_MSG_CMD_LEN);
    /* Log the message: first 256 bytes */
    cfa_log_packet_ex( GS_QUEUE_RR,                   /* QUEUE */
                         MS_TDS_RRC,                      /* the message set */
                         (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                         TDSCFA_MSG_LEN,                            /* First TDSCFA_MSG_LEN for now */
                         tdscfa_buff );       /* Pointer to the message */
  }
  else if(cmd_ptr->cmd_hdr.cmd_id >= TDSRRC_MM_CMD_BASE)
  {
    tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
             
    memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), TDSCFA_MSG_CMD_LEN);
  
    /* Log the message: first 256 bytes */
    cfa_log_packet_ex( GS_QUEUE_MM,                   /* QUEUE */
                       MS_TDS_RRC,                      /* the message set */
                       (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                       TDSCFA_MSG_LEN,                            /* First TDSCFA_MSG_LEN for now */
                      tdscfa_buff );       /* Pointer to the message */
  }
  else if(cmd_ptr->cmd_hdr.cmd_id >= TDSRRC_TC_CMD_BASE) 
  {
    tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
          
    memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), TDSCFA_MSG_CMD_LEN);
  
    cfa_log_packet_ex( GS_QUEUE_TC,                /* QUEUE */
                       MS_TDS_RRC,                       /* the message set */
                       (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                       TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                       tdscfa_buff );       /* Pointer to the message */
  
  }
  else if(cmd_ptr->cmd_hdr.cmd_id >= TDSRRC_RLC_CMD_BASE)
  {
  
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
      case  TDSRRC_CRLC_STATUS_IND:
      case TDSRRC_CRLC_UL_TM_CONFIG_CNF:
      case TDSRRC_CRLC_UL_UM_CONFIG_CNF:
      case TDSRRC_CRLC_AM_CONFIG_CNF:
      case TDSRRC_CRLC_RELEASE_ALL_CNF:
       /* Log the message: first 256 bytes */
       tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
      
       memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), TDSCFA_MSG_CMD_LEN);
       cfa_log_packet_ex( GS_QUEUE_TDSRLCUL,                   /* QUEUE */
                         MS_TDS_RRC,                      /* the message set */
                         (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                         TDSCFA_MSG_LEN,                            /* First TDSCFA_MSG_LEN for now */
                         tdscfa_buff );       /* Pointer to the message */
       break;
     case TDSRRC_CRLC_DL_UM_CONFIG_CNF:
     case TDSRRC_CRLC_DL_TM_CONFIG_CNF:
       /* Log the message: first 256 bytes */
       tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
      
       memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), TDSCFA_MSG_CMD_LEN);
       cfa_log_packet_ex( GS_QUEUE_TDSRLCDL,                   /* QUEUE */
                         MS_TDS_RRC,                      /* the message set */
                         (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                         TDSCFA_MSG_LEN,                            /* First TDSCFA_MSG_LEN for now */
                         tdscfa_buff );       /* Pointer to the message */
       break;
     default:
       break;
    }
  }
  else if(cmd_ptr->cmd_hdr.cmd_id >= TDSRRC_MAC_CMD_BASE)
  {
    tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
    
    memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), TDSCFA_MSG_CMD_LEN);
    
    cfa_log_packet_ex( GS_QUEUE_TDSMACUL,                   /* QUEUE */
                    MS_TDS_RRC,                      /* the message set */
                    (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                    TDSCFA_MSG_LEN,                            /* First TDSCFA_MSG_LEN for now */
                    tdscfa_buff );       /* Pointer to the message */
    
  }
  else if(cmd_ptr->cmd_hdr.cmd_id >= TDSRRC_L1_CMD_BASE)
  {
    tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
    
    memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), TDSCFA_MSG_CMD_LEN);
    
    cfa_log_packet_ex( GS_QUEUE_TDSL1,                   /* QUEUE */
                MS_TDS_RRC,                      /* the message set */
                (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                TDSCFA_MSG_LEN,                            /* First TDSCFA_MSG_LEN for now */
                tdscfa_buff );       /* Pointer to the message */
    
  }
  else if(cmd_ptr->cmd_hdr.cmd_id >= TDSRRC_INT_CMD_BASE)
  {
    tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;
    
    memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), TDSCFA_MSG_CMD_LEN);
    
    cfa_log_packet_ex( GS_QUEUE_TDSRRC,                   /* QUEUE */
                MS_TDS_RRC,                      /* the message set */
                (byte)cmd_ptr->cmd_hdr.cmd_id,  /* the message id  */
                TDSCFA_MSG_LEN,                            /* First TDSCFA_MSG_LEN for now */
                tdscfa_buff );       /* Pointer to the message */
    
  }
  else
  {
  //which q is this??
  }
      
}

#endif
/*===========================================================================

FUNCTION tdsmm_parse_cfa

DESCRIPTION

Parses  CFA for MM msg sent to MM module



RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
#if 0
static void tdsmm_parse_cfa
(
  mm_cmd_type    *cmd_ptr
)
{
  /*IMH_T *  IMHP = (IMH_T *)(&(cmd_ptr->cmd.hdr));*/
  byte * msg_ptr=(byte *)(&(cmd_ptr->cmd));
  /*word      message_length;
  message_length = IMHP->message_len_lsb  + (0x100 * IMHP->message_len_msb) ;*/
  
  /* Log using CFA packet */
  cfa_log_packet_ex( GS_QUEUE_MM,
               msg_ptr[0],
               msg_ptr[1],
               TDSCFA_MSG_LEN,
               msg_ptr + sizeof(IMH_T)
               );
  
}
#endif
/*===========================================================================

FUNCTION tdsrabm_parse_cfa

DESCRIPTION

Parses  CFA for RABM  msg sent to RABM module




RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
#if 0
static void tdsrabm_parse_cfa
(
  rabm_cmd_type    *cmd_ptr
)
{
  byte tdscfa_buff[TDSCFA_MSG_LEN];
  byte tdscfa_cmd_len = 0;

  memset(tdscfa_buff,0,TDSCFA_MSG_LEN);
  tdscfa_buff[TDSCFA_MSG_VERSION_POS] = 0;

  tdscfa_cmd_len = sizeof(cmd_ptr->cmd);
  tdscfa_cmd_len = (tdscfa_cmd_len < TDSCFA_MSG_CMD_LEN) ? tdscfa_cmd_len : TDSCFA_MSG_CMD_LEN;
  
  memscpy(&tdscfa_buff[TDSCFA_MSG_CMD_POS],TDSCFA_MSG_LEN,&(cmd_ptr->cmd), tdscfa_cmd_len);
  
  cfa_log_packet_ex( GS_QUEUE_MM,                   /* QUEUE */
                      MS_RABM_TDSRRC,               /* the message set */
                      (byte)cmd_ptr->header.cmd_id,  /* the message id  */
                      TDSCFA_MSG_LEN,                            /* first TDSCFA_MSG_LEN for now */
                      tdscfa_buff );       /* Pointer to the message */    
  
}


#endif


/*===========================================================================
 
 FUNCTION tds_parse_cfa
 
 DESCRIPTION
 
   !!! Parses  CFA for TDS messages mainly for within TDS AS layers. !!! 
   Message will be parsed regardless.
   !!! All QTF unit test messages should also come here in particular. !!!
   q_type indicates the queue in which the message is being put to
   cmd_ptr is the pointer to be logged.
 
 
 RETURN VALUE
 
   None.
 
 SIDE EFFECTS
 
   None.
 
 ===========================================================================*/

 void tds_parse_cfa
(
  tds_cfa_queue_type q_type,
  void   * cmd_ptr

)
{
  tdsrrc_cmd_entry_type cmd_entry;
  boolean add_to_hist = FALSE;
  tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
  boolean add_to_dsds_hist = FALSE;
    
  if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
     TDSRRC_MSG1(MSG_LEGACY_HIGH,"CFA is not supported by NV setting: cmd id=0x%x",((tdsrrc_cmd_type *)cmd_ptr)->cmd_hdr.cmd_id);
     return;
  }

  if (!cmd_ptr)
  {
    ERR_FATAL("tds_parse_cfa: received null cmd_ptr for q_type %d",q_type,0,0);   
    return;
  }
  
  memset((void *) &cmd_entry, 0, sizeof(tdsrrc_cmd_entry_type));

  cmd_entry.cmd_addr = (uint32) cmd_ptr;
  memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));
  switch(q_type)
  {

    case TDSCFA_L2UL_Q:
      //tdsl2ul_parse_cfa((tdsl2_ul_cmd_type *)cmd_ptr);  /*We dont invoke this function as its not needed */
      {
         tdsl2_ul_cmd_type * l2ulcmd_ptr = (tdsl2_ul_cmd_type *)cmd_ptr;

         cmd_entry.dest = TDSCFA_L2UL_Q;
         cmd_entry.cmd_id = (uint32) l2ulcmd_ptr->cmd_hdr.cmd_id;
         dsds_cmd_entry.dest = TDSCFA_L2UL_Q;
         dsds_cmd_entry.cmd_id = (uint32) l2ulcmd_ptr->cmd_hdr.cmd_id;
         if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
         {
            cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(tdsl2_ul_cmd_type));
            if (cmd_entry.cmd_dup_ptr)
            {
               cmd_entry.cmd_size = sizeof(tdsl2_ul_cmd_type);
               (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(tdsl2_ul_cmd_type),cmd_ptr, cmd_entry.cmd_size);
            }
         }

         add_to_hist = TRUE;
         dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
         add_to_dsds_hist = TRUE;
      }
      break;
    case TDSCFA_L2DL_Q:
      //tdsl2dl_parse_cfa((tdsl2_dl_cmd_type *)cmd_ptr); /*We dont invoke this function as its not needed */
      {
         tdsl2_dl_cmd_type * l2dlcmd_ptr = (tdsl2_dl_cmd_type *)cmd_ptr;

         cmd_entry.dest = TDSCFA_L2DL_Q;
         cmd_entry.cmd_id = (uint32) l2dlcmd_ptr->cmd_hdr.cmd_id;
         dsds_cmd_entry.dest = TDSCFA_L2DL_Q;
         dsds_cmd_entry.cmd_id = (uint32) l2dlcmd_ptr->cmd_hdr.cmd_id;
         if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
         {
            cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(tdsl2_dl_cmd_type));
            if (cmd_entry.cmd_dup_ptr)
            {
               cmd_entry.cmd_size = sizeof(tdsl2_dl_cmd_type);
               (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(tdsl2_dl_cmd_type),cmd_ptr, cmd_entry.cmd_size);
            }
         }

         add_to_hist = TRUE;
         dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
         add_to_dsds_hist = TRUE;
      }
      break;
    case TDSCFA_L1_Q:
      //tdsl1_parse_cfa((tdsl1_ext_cmd_type *)cmd_ptr); /*We dont invoke this function as its not needed */
      {
         tdsl1_ext_cmd_type * l1cmd_ptr = (tdsl1_ext_cmd_type *)cmd_ptr;

         cmd_entry.dest = TDSCFA_L1_Q;
         cmd_entry.cmd_id = (uint32) l1cmd_ptr->cmd_hdr.cmd_id;
         dsds_cmd_entry.dest = TDSCFA_L1_Q;
         dsds_cmd_entry.cmd_id = (uint32) l1cmd_ptr->cmd_hdr.cmd_id;
         if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
         {
            cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(tdsl1_ext_cmd_type));
            if (cmd_entry.cmd_dup_ptr)
            {
               cmd_entry.cmd_size = sizeof(tdsl1_ext_cmd_type);
               (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(tdsl1_ext_cmd_type),cmd_ptr, cmd_entry.cmd_size);
            }
         }

         add_to_hist = TRUE;
         dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
         add_to_dsds_hist = TRUE;
      }
      break;
    case TDSCFA_RRC_Q:
      //tdsrrc_parse_cfa((tdsrrc_cmd_type *)cmd_ptr); /*We dont invoke this function as its not needed */
      {
         /* Only maintain header structure 
          * RR and WRRC use different command body
          */
         tdsrrc_cmd_type * rrccmd_ptr = (tdsrrc_cmd_type *)cmd_ptr;

         cmd_entry.dest = TDSCFA_RRC_Q;
         cmd_entry.cmd_id = (uint32) rrccmd_ptr->cmd_hdr.cmd_id;
         dsds_cmd_entry.dest = TDSCFA_RRC_Q;
         dsds_cmd_entry.cmd_id = (uint32) rrccmd_ptr->cmd_hdr.cmd_id;
         if (dsds_cmd_entry.cmd_id == TDSRRC_MSGR_CMD)
         {
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L         
            cmd_entry.as_id = (sys_modem_as_id_e_type)MSGR_GET_VARIANT(&(rrccmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr));
            dsds_cmd_entry.as_id = cmd_entry.as_id;
            MSGR_CLEAR_VARIANT(rrccmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id);
#endif
  
            dsds_cmd_entry.cmd_umid = rrccmd_ptr->cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id;
         }
         /* For external command */
         if (rrccmd_ptr->cmd_hdr.cmd_id < TDSRRC_INT_CMD_BASE ||
             rrccmd_ptr->cmd_hdr.cmd_id >= TDSRRC_CMD_ID_L1_PLACEHLDER_BEGIN)
         {
            add_to_hist = TRUE;
            add_to_dsds_hist = TRUE;
            dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
            if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
            {
               tdsrrc_cmd_hist_dup_incoming_ext_cmd(&cmd_entry, cmd_ptr);
            }
         }
         else 
         {/* For RRC internal command */
            if (tdsrrccfa_cmd_hist_settings.incl_rrc_int)
            {
               add_to_hist = TRUE;
               add_to_dsds_hist = TRUE;
               dsds_cmd_entry.dsds_event = TDSRRC_DSDS_INTERNAL_COMMAND;
               if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
               {
                  cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(tdsrrc_cmd_type));
                  if (cmd_entry.cmd_dup_ptr)
                  {
                     cmd_entry.cmd_size = sizeof(tdsrrc_cmd_type);
                     /* Not necessarily copied everything.
                      * Buffer to secondary pointer is not copied
                      */
                     (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(tdsrrc_cmd_type),cmd_ptr, cmd_entry.cmd_size);
                  }
               }
            }
            else
            {
               if ((dsds_cmd_entry.cmd_id == TDSRRC_RESET_L1_REQ) ||
                   (dsds_cmd_entry.cmd_id == TDSRRC_RESET_L1_CNF) ||
                   (dsds_cmd_entry.cmd_id == TDSRRC_DUAL_SIM_WRM_UNLOCK_BY_TIMER_EXP_IND) ||
                   (dsds_cmd_entry.cmd_id == TDSRRC_NO_RESOURCE_AVAILABLE_FROM_L1) ||
                   (dsds_cmd_entry.cmd_id == TDSRRC_WRM_PREEMPT_IND) ||
                   (dsds_cmd_entry.cmd_id == TDSRRC_INITIATE_CELL_SELECTION_IND) ||
                   (dsds_cmd_entry.cmd_id == TDSRRC_CELL_UPDATE_INITIATE_REQ)
                  )
               {
                  add_to_dsds_hist = TRUE;
                  dsds_cmd_entry.dsds_event = TDSRRC_DSDS_INTERNAL_COMMAND;
               }
            }
         }
      }
      break;
    case TDSCFA_MM_Q:
      /* Only for QTF message */
      /*tdsmm_parse_cfa((mm_cmd_type *)cmd_ptr); */
      break;   /*We dont invoke this function as its not needed */
    case TDSCFA_RABM_Q:
      /* Only for QTF message */
      /*tdsrabm_parse_cfa((rabm_cmd_type *)cmd_ptr); */
      break;  /*We dont invoke this function as its not needed */
    case TDSCFA_RR_Q:
      /* TODO: add CFA parsing for RR_Q only for QTF */
      break;
    default:
      cmd_entry.dest = TDSCFA_UNKNOWN_Q;
      cmd_entry.cmd_id = 0x12345678;
      add_to_hist = TRUE;
      dsds_cmd_entry.dest = TDSCFA_UNKNOWN_Q;
      dsds_cmd_entry.cmd_id = 0x12345678;
      dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
      add_to_dsds_hist = TRUE; 
      break;
  }

  if (add_to_hist)
     tdsrrc_cmd_hist_add_entry(&cmd_entry);
  if (add_to_dsds_hist)
  {
     tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
  }
}

void tdscfa_mm_put_cmd(mm_cmd_type *cmd_ptr)
{
   /* No parsing here because it is parsed in MM CFA */

   mm_cmd_type * mmcmd_ptr = (mm_cmd_type *)cmd_ptr;
   tdsrrc_cmd_entry_type cmd_entry;
   tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
   if (!cmd_ptr)
   {
      ERR_FATAL("tdscfa_mm_put_cmd: received null ptr",0,0,0);
      return;
   }

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC->MM: cmd_id=%d",mmcmd_ptr->cmd.hdr.message_id);
   
   if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      return;
   }
   
   memset((void *) &cmd_entry, 0, sizeof(tdsrrc_cmd_entry_type));
      
   cmd_entry.dest = TDSCFA_MM_Q;
   cmd_entry.cmd_id = (uint32) mmcmd_ptr->cmd.hdr.message_id;
   cmd_entry.cmd_addr = (uint32) cmd_ptr;
   memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));
      
   dsds_cmd_entry.dest = TDSCFA_MM_Q;
   dsds_cmd_entry.cmd_id = (uint32) mmcmd_ptr->cmd.hdr.message_id;
   if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
   {
      /* Use RRC malloc anyway */
      cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(mm_cmd_type));
      if (cmd_entry.cmd_dup_ptr)
      {
         cmd_entry.cmd_size = sizeof(mm_cmd_type);
         /* Not necessarily deep copy. NAS OTA data may not be copied */
         (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(mm_cmd_type),(void *) cmd_ptr, cmd_entry.cmd_size);
      }
   }
   
   /* Add to command history */
   tdsrrc_cmd_hist_add_entry(&cmd_entry);
   dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
   tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
}

void tdscfa_rabm_put_cmd(rabm_cmd_type *cmd_ptr)
{
   /* No parsing here because it is parsed in RABM CFA */
   
   rabm_cmd_type * rabmcmd_ptr = (rabm_cmd_type *)cmd_ptr;
   tdsrrc_cmd_entry_type cmd_entry;
   tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
   if (!cmd_ptr)
   {
      ERR_FATAL("tdscfa_rabm_put_cmd: received null ptr",0,0,0);
      return;
   }

   TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSRRC->RABM: cmd_id=%d, connection_id=%d", 
               rabmcmd_ptr->header.cmd_id,
               rabmcmd_ptr->header.connection_id);

   if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      return;
   }

   memset((void *) &cmd_entry, 0, sizeof(tdsrrc_cmd_entry_type));

   cmd_entry.dest = TDSCFA_RABM_Q;
   cmd_entry.cmd_id = (uint32) rabmcmd_ptr->header.cmd_id;
   cmd_entry.cmd_addr = (uint32) cmd_ptr;
   memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));

   dsds_cmd_entry.dest = TDSCFA_RABM_Q;
   dsds_cmd_entry.cmd_id = (uint32) rabmcmd_ptr->header.cmd_id;
   if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
   {
      /* Use RRC malloc anyway */
      cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(rabm_cmd_type));
      if (cmd_entry.cmd_dup_ptr)
      {
         cmd_entry.cmd_size = sizeof(rabm_cmd_type);
         (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(rabm_cmd_type),(void *) cmd_ptr, cmd_entry.cmd_size);
      }
   }
   
   /* Add to command history */
   tdsrrc_cmd_hist_add_entry(&cmd_entry);
   dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
   tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
}

void tdscfa_comp_put_cmd(comp_cmd_type *cmd_ptr)
{
   /* TODO: add CFA parsing for COMP_Q */ 
   
   comp_cmd_type * compcmd_ptr = (comp_cmd_type *)cmd_ptr;
   tdsrrc_cmd_entry_type cmd_entry;
   tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
   if (!cmd_ptr)
   {
      ERR_FATAL("tdscfa_comp_put_cmd: received null ptr",0,0,0);
      return;
   }

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC->COMP: cmd_id=%d", compcmd_ptr->hdr.cmd_id);

   if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      return;
   }

   memset((void *) &cmd_entry, 0, sizeof(tdsrrc_cmd_entry_type));

   cmd_entry.dest = TDSCFA_COMP_Q;
   cmd_entry.cmd_id = (uint32) compcmd_ptr->hdr.cmd_id;
   cmd_entry.cmd_addr = (uint32) cmd_ptr;
   memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));

   dsds_cmd_entry.dest = TDSCFA_COMP_Q;
   dsds_cmd_entry.cmd_id = (uint32) compcmd_ptr->hdr.cmd_id;
   if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
   {
      /* Use RRC malloc because Comp task uses global array */
      cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(comp_cmd_type));
      if (cmd_entry.cmd_dup_ptr)
      {
         cmd_entry.cmd_size = sizeof(comp_cmd_type);
         (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(comp_cmd_type),(void *) cmd_ptr, cmd_entry.cmd_size);
      }
   }
   
   /* Add to command history */
   tdsrrc_cmd_hist_add_entry(&cmd_entry);
   dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
   tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
}

void tdscfa_cm_put_cmd(cm_rpt_type *cmd_ptr)
{
   /* TODO: add CFA parsing for CM_Q */ 
   
   cm_rpt_type * cmcmd_ptr = (cm_rpt_type *)cmd_ptr;
   tdsrrc_cmd_entry_type cmd_entry;
   tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
   if (!cmd_ptr)
   {
      ERR_FATAL("tdscfa_cm_put_cmd: received null ptr",0,0,0);
      return;
   }

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC->CM: cmd_id=%d", cmcmd_ptr->hdr.cmd);

   if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      return;
   }

   memset((void *) &cmd_entry, 0, sizeof(tdsrrc_cmd_entry_type));
      
   cmd_entry.dest = TDSCFA_CM_Q;
   cmd_entry.cmd_id = (uint32) cmcmd_ptr->hdr.cmd;
   cmd_entry.cmd_addr = (uint32) cmd_ptr;
   memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));
      
   dsds_cmd_entry.dest = TDSCFA_CM_Q;
   dsds_cmd_entry.cmd_id = (uint32) cmcmd_ptr->hdr.cmd;
   if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
   {
      /* Use RRC malloc anyway */
      cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(cm_rpt_type));
      if (cmd_entry.cmd_dup_ptr)
      {
         cmd_entry.cmd_size = sizeof(cm_rpt_type);
         (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(cm_rpt_type),(void *) cmd_ptr, cmd_entry.cmd_size);
      }
   }
   
   /* Add to command history */
   tdsrrc_cmd_hist_add_entry(&cmd_entry);
   dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
   tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
}

void tdscfa_tc_put_cmd(tc_cmd_type *cmd_ptr)
{
   /* TODO: add CFA parsing for TC_Q */ 
   
   tc_cmd_type * tccmd_ptr = (tc_cmd_type *)cmd_ptr;
   tdsrrc_cmd_entry_type cmd_entry;
   tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
   if (!cmd_ptr)
   {
      ERR_FATAL("tdscfa_tc_put_cmd: received null ptr",0,0,0);
      return;
   }

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC->TC: cmd_id=%d", tccmd_ptr->header.cmd_id);

   if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      return;
   }

   memset((void *) &cmd_entry, 0, sizeof(tdsrrc_cmd_entry_type));

   cmd_entry.dest = TDSCFA_TC_Q;
   cmd_entry.cmd_id = (uint32) tccmd_ptr->header.cmd_id;
   cmd_entry.cmd_addr = (uint32) cmd_ptr;
   memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));

   dsds_cmd_entry.dest = TDSCFA_TC_Q;
   dsds_cmd_entry.cmd_id = (uint32) tccmd_ptr->header.cmd_id;

   if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
   {
      /* Use RRC malloc anyway */
      cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(tc_cmd_type));
      if (cmd_entry.cmd_dup_ptr)
      {
         cmd_entry.cmd_size = sizeof(tc_cmd_type);
         (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(tc_cmd_type),(void *) cmd_ptr, cmd_entry.cmd_size);
      }
   }
   
   /* Add to command history */
   tdsrrc_cmd_hist_add_entry(&cmd_entry);
   dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
   tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
}

#ifdef FEATURE_TDSCDMA_TO_WCDMA
void tdscfa_rrc_put_tdsrrc_cmd(rrc_tdsrrc_cmd_type *cmd_ptr)
{
   /* TODO: add CFA parsing for WRRC_Q */ 
   
   rrc_tdsrrc_cmd_type * wrrccmd_ptr = (rrc_tdsrrc_cmd_type *)cmd_ptr;
   tdsrrc_cmd_entry_type cmd_entry;
   tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
   if (!cmd_ptr)
   {
      ERR_FATAL("tdscfa_rrc_put_tdsrrc_cmd: received null ptr",0,0,0);
      return;
   }

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC->WRRC: cmd_id=0x%x",wrrccmd_ptr->cmd_hdr.cmd_id);

   if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      return;
   }

   memset((void *) &cmd_entry, 0, sizeof(tdsrrc_cmd_entry_type));

   cmd_entry.dest = TDSCFA_WRRC_Q;
   cmd_entry.cmd_id = (uint32) wrrccmd_ptr->cmd_hdr.cmd_id;
   cmd_entry.cmd_addr = (uint32) cmd_ptr;
   memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));

   dsds_cmd_entry.dest = TDSCFA_WRRC_Q;
   dsds_cmd_entry.cmd_id = (uint32) wrrccmd_ptr->cmd_hdr.cmd_id;

   if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
   {
      /* Use RRC malloc anyway */
      cmd_entry.cmd_dup_ptr = tdsrrc_malloc(sizeof(rrc_tdsrrc_cmd_type));
      if (cmd_entry.cmd_dup_ptr)
      {
         cmd_entry.cmd_size = sizeof(rrc_tdsrrc_cmd_type);
         (void) memscpy(cmd_entry.cmd_dup_ptr, sizeof(rrc_tdsrrc_cmd_type),(void *) cmd_ptr, cmd_entry.cmd_size);
      }
   }
   
   /* Add to command history */
   tdsrrc_cmd_hist_add_entry(&cmd_entry);
   dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
   tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
}
#endif

void tdscfa_tdsrr_send_rrc_message_to_rr
(
   void *                message_ptr,
   tdsrr_rrc_cmd_e_type  rrc_cmd_id,
   word                  length
)
{
   /* No parsing here because it is parsed in RR CFA */

   tdsrrc_cmd_entry_type cmd_entry;
   tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
   if ((!message_ptr) || (!length))
   {
      ERR_FATAL("tdscfa_tdsrr_send_rrc_message_to_rr: invalid data msg_ptr=0x%x, len=%d",(uint32) message_ptr,length,0);   
      return;
   }

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC->RR: cmd_id=%d",rrc_cmd_id);

   if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      return;
   }

   memset((void *) &cmd_entry, 0, sizeof(tdsrrc_cmd_entry_type));
      
   cmd_entry.dest = TDSCFA_RR_Q;
   cmd_entry.cmd_id = (uint32) rrc_cmd_id;
   /* message header is only set after the command is sent to RR */
   cmd_entry.cmd_addr = (uint32) message_ptr;
   memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));
      
   dsds_cmd_entry.dest = TDSCFA_RR_Q;
   dsds_cmd_entry.cmd_id = (uint32) rrc_cmd_id;
   /* message header is only set after the command is sent to RR */
   if (tdsrrccfa_cmd_hist_settings.incl_cmd_data)
   {
      /* Use RRC malloc anyway */
      cmd_entry.cmd_dup_ptr = tdsrrc_malloc(length);
      if (cmd_entry.cmd_dup_ptr)
      {
         IMH_T *imh_ptr = (IMH_T *) cmd_entry.cmd_dup_ptr;

         cmd_entry.cmd_size = length;
         (void) memscpy(cmd_entry.cmd_dup_ptr, length,message_ptr, cmd_entry.cmd_size);

         /* Process message header as did in tdsrr_send_rrc_message_to_rr() */
         imh_ptr->message_id = (byte) cmd_entry.cmd_id;
         imh_ptr->message_set = MS_TDSRRC_RR;
         PUT_IMH_LEN((length - sizeof(IMH_T)),imh_ptr);
      }
   }

   /* Add to command history */
   tdsrrc_cmd_hist_add_entry(&cmd_entry);
   dsds_cmd_entry.dsds_event = TDSRRC_DSDS_EXTERNAL_COMMAND;
   tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
}

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
void tdscfa_send_message_to_lte_rrc
(
   msgr_hdr_struct_type * message_ptr,
   uint32                 length
)
{
   msgr_umid_type umid; 

   /* TODO: add CFA parsing for LTE_Q */ 

   if ((!message_ptr) || (!length))
   {
      ERR_FATAL("tdscfa_send_message_to_lte_rrc: invalid data msg_ptr=0x%x, len=%d",(uint32) message_ptr,length,0);   
      return;
   }

   umid = message_ptr->id;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L   
   MSGR_CLEAR_VARIANT(umid);
#endif

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC->LTE: UMID=0x%x",umid);

   if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      return;
   }

   tdscfa_send_message_to_msgr(TDSCFA_LTE_Q, message_ptr, length);
}
#endif

void tdscfa_send_message_to_qmi
(
   msgr_hdr_struct_type * message_ptr,
   uint32                 length
)
{
   /* TODO: add CFA parsing for LTE_Q */ 

   if ((!message_ptr) || (!length))
   {
      ERR_FATAL("tdscfa_send_message_to_qmi: invalid data msg_ptr=0x%x, len=%d",(uint32) message_ptr,length,0);   
      return;
   }

   if(!TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
   {
      return;
   }

   tdscfa_send_message_to_msgr(TDSCFA_QMI_Q, message_ptr, length);
}

void tdsrrc_wait_ms(uint32 wait_time)
{
   static rex_timer_type  poll_timer;
   
   rex_def_timer ( &poll_timer, tds_rrc_get_tcb(), TDSRRC_TEST_WAIT_MS_SIG);

   (void)rex_clr_sigs( tds_rrc_get_tcb(), TDSRRC_TEST_WAIT_MS_SIG );

   (void)rex_set_timer( &poll_timer, (rex_timer_cnt_type) wait_time );

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrc_wait_ms waits for %d ms",wait_time);

   (void)rex_wait (TDSRRC_TEST_WAIT_MS_SIG);

   (void)rex_clr_sigs( tds_rrc_get_tcb(), TDSRRC_TEST_WAIT_MS_SIG);

   rex_undef_timer(&poll_timer);

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrc_wait_ms returns after %d ms",wait_time);
}
void tdscfa_dsds_add_event(tdsrrc_dsds_event_type dsds_event)
{
  tdsrrc_dsds_cmd_entry_type dsds_cmd_entry;
  memset((void *) &dsds_cmd_entry, 0, sizeof(tdsrrc_dsds_cmd_entry_type));
  dsds_cmd_entry.dsds_event = dsds_event;
  tdsrrc_dsds_cmd_hist_add_entry(&dsds_cmd_entry);
  tdsrrc_print_trm_status();
}

void tdsrrc_print_trm_status(void)
{
#if !defined(TEST_FRAMEWORK)


   trm_get_info_input_type get_info_input; 
   trm_get_info_return_type get_info_output;

   rfm_device_enum_type tds_granted;
   rfm_device_enum_type tds2_granted;
   rfm_device_enum_type g1_granted;
   rfm_device_enum_type g2_granted;
   rfm_device_enum_type irat_granted;

   trm_reason_enum_t tds_reason;
   trm_reason_enum_t tds2_reason;
   trm_reason_enum_t g1_reason;
   trm_reason_enum_t g2_reason;
   trm_reason_enum_t irat_reason;

   TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDS TRM API New"); 

   get_info_input.client_id = TRM_TDSCDMA;
   get_info_input.info_type = TRM_CLIENT_INFO;
   get_info_input.input_data.client_info.requested_client_id = TRM_TDSCDMA;

   trm_get_info(&get_info_input, &get_info_output);

   tds_granted = get_info_output.info.client_info.dev;
   tds_reason = get_info_output.info.client_info.reason;

   get_info_input.client_id = TRM_TDSCDMA;
   get_info_input.info_type = TRM_CLIENT_INFO;
   get_info_input.input_data.client_info.requested_client_id = TRM_TDSCDMA_SECONDARY;

   trm_get_info(&get_info_input, &get_info_output);

   tds2_granted = get_info_output.info.client_info.dev;
   tds2_reason = get_info_output.info.client_info.reason;

   get_info_input.client_id = TRM_TDSCDMA;
   get_info_input.info_type = TRM_CLIENT_INFO;
   get_info_input.input_data.client_info.requested_client_id = TRM_GSM1;

   trm_get_info(&get_info_input, &get_info_output);

   g1_granted = get_info_output.info.client_info.dev;
   g1_reason = get_info_output.info.client_info.reason;

   get_info_input.client_id = TRM_TDSCDMA;
   get_info_input.info_type = TRM_CLIENT_INFO;
   get_info_input.input_data.client_info.requested_client_id = TRM_GSM2;

   trm_get_info(&get_info_input, &get_info_output);

   g2_granted = get_info_output.info.client_info.dev;
   g2_reason = get_info_output.info.client_info.reason;

   get_info_input.client_id = TRM_TDSCDMA;
   get_info_input.info_type = TRM_CLIENT_INFO;
   get_info_input.input_data.client_info.requested_client_id = TRM_IRAT;

   trm_get_info(&get_info_input, &get_info_output);

   irat_granted = get_info_output.info.client_info.dev;
   irat_reason = get_info_output.info.client_info.reason;

   TDSRRC_MSG5(MSG_LEGACY_HIGH,"TDS TRM granted: T %d, Tdiv %d, G1 %d, G2 %d, IRAT %d",
               tds_granted,
               tds2_granted,
               g1_granted,
               g2_granted,
               irat_granted);

   TDSRRC_MSG5(MSG_LEGACY_HIGH,"TDS TRM reasons: T %d, Tdiv %d, G1 %d, G2 %d, IRAT %d",
               tds_reason,
               tds2_reason,
               g1_reason,
               g2_reason,
               irat_reason);

#endif /* #if !defined(TEST_FRAMEWORK) */
}



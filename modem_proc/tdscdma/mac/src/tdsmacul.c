/*================================================================================================

                            U P L I N K     M A C    H A N D L E R

GENERAL DESCRIPTION

  This module contains functions to :
  - process uplink MAC task events
  - process uplink ciphering configuration data
  - process uplink RNTI information
  - process uplink CCTrCh configuration
  - process UL MAC configuration
  - Process UL MAC commands

Copyright (c) 2010-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


================================================================================================*/

/*================================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmacul.c#2 $ $DateTime: 2021/02/04 23:39:42 $ $Author: pwbldsvc $

when      who   what, where, why
--------  ---   ---------------------------------------------------------------------------------- 
03/30/15  sarao  FR26213 for QSH support 
04/25/12  mr      Fixed Compiler warning
09/08/11  mkh   Events renamed to TDS
02/02/11  ps    Changes for MAC HSUPA code
10/24/10  guo   Initial release Upmerged WCDMA to TDS

================================================================================================*/
/*================================================================================================
                     INCLUDE FILES
================================================================================================*/
#include "tdscdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "task.h"

#ifdef FEATURE_QSH_EVENT_METRIC
#include "tdsmac_qsh_ext.h"
#endif/*FEATURE_QSH_EVENT_METRIC*/
#include  "tdsmacrrcif.h"
#include  "tdsl1macif.h"
#include  "tdsl1sapcommon.h"
#include  "tdsl1macdata.h"
#include  "tdsmacinternal.h"
#include  "tdsl2ultask.h"
#include  "tdsmaculdata.h"
#include  "tdsmacrach.h"
#include  "tdsmaculdata.h"
#include  "tdsmacul.h"
#include  "tdsmacdl.h"
#include  "tdsmaculdch.h"
#include  "tdsmacultfcs.h"
#include  "tdsmactraffic.h"
#include  <string.h>
#include  "err.h"
#include "tdsstmr.h"
#include "event.h"
#include "event_defs.h"

#ifdef FEATURE_TDSCDMA_HSUPA
#include "tdsmaculhs.h"
#endif
#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
  #include "tdsmacullog.h"
#endif
#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
#include "tdsmaculhslog.h"
#endif

#include "tdsl1extif.h"

  #include "tdsmaculdl.h"

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  #include "tdsmaculdl.h"
#endif

#include "tdsmaculsubtask.h"
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

#include "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"
#include "tdsrrcdata.h"

#ifdef FEATURE_QSH_EVENT_METRIC
extern tdsrlc_ul_lc_info_type tdsmac_ul_lc_info;
extern tdsmac_ulhs_state_e_type tdsmac_ulhs_state;
#endif


extern tdsmac_ul_tfc_control_e_type        tdsmac_ul_full_tfcs_control_table[TDSUE_MAX_TFC];
extern tdsmac_ul_tfc_control_e_type        tdsmac_ul_default_tfcs_control_table[TDSUE_MAX_TFC];
extern tdsmac_ul_tfc_control_e_type        tdsmac_ul_current_tfcs_control_table[TDSUE_MAX_TFC];
tdsmac_ul_tfc_control_e_type               tdsmac_ul_pending_tfcs_control_table[TDSUE_MAX_TFC];
extern tdsmac_ul_tfcs_type                 tdsmac_ul_tfcs_table;
//extern tdstfcs_control_ptr_in_use_e_type   mac_ul_tfcs_control_ptr_in_use;
boolean                                 tdsmac_ul_tfcc_config_pending=FALSE;
boolean                                 tdsmac_ul_tfcc_config_in_process = FALSE;
uint16                                  tdsmac_ul_tfcc_activation_cfn=0xFFFF;
uint16                                  tdsmac_ul_tfcs_pending_control_duration;
#ifdef FEATURE_TDSCDMA_TFC_ROUNDROBIN
extern uint16                           tdsmac_tfc_roundrobin_prev_tfci;
#endif

/* Traffic volume timer*/
rex_timer_type                          tdsul_tfc_ctrl_tmr_blk;

boolean             tdsmac_enable_amr_debug_dump = FALSE;

extern tdsul_tfcs_info_type   *tdsint_ul_tfcs_info;

/* Stores the information about which of the signalling channel, there was
data built in the last TTI. This information will later be used at the
time when a transport block is trashed at L1 */
extern tdsrlc_status_report_req_type  tdsrlc_status_report_req_info;

extern  void tdsmac_ulhs_update_tvm_lchinfo(void);

/*================================================================================================
                         CONSTANT    D E C L A R A T I O N S
================================================================================================*/



#define CHECK_FOR_ACT_CFN_EXPIRY(S,A,R)   ((uint32)(((uint16)256+(A)-(S)) & 0xFF) <= ((uint32)((uint16)256+(R)-(S)) & 0xFF))


/*================================================================================================
                         D A T A    D E C L A R A T I O N S
================================================================================================*/
/* Uplink CCTrCh info internally used by MAC */
tdsul_cctrch_info_type       tdsint_ul_cctrch_info;

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
/* Notes the uplink cfn */
uint8  tdscur_ul_cfn;
#endif

/* this array contains the indexes where the transport channel information for that */
/* transport channel id is stored. */
tdstrch_id_to_idx_map_type tdsul_trch_id_to_idx_table[MAX_TRCH];

/* Uplink Logical Channel info table internally used by MAC */
tdsul_lc_info_type         tdsint_ul_lc_info[TDSUE_MAX_UL_LOGICAL_CHANNEL];

/* UL Transport Channel Info table internally used by MAC */
tdsul_trch_info_type       tdsint_ul_trch_info[TDSUE_MAX_TRCH];


/* UL Frame Build Info */
tdsmac_ul_build_frame_ind_type   tdsbld_frame_ind_info;

/* TRUE if the C-RNTI is valid. FALSE otherwise */
boolean     tdsul_mac_crnti_valid = FALSE;

/* C-RNTI value */
uint16      tdsul_mac_crnti;

/* Flag to denote that MAC is still building previous request */
boolean tdsmac_build_pending = FALSE;

extern tdsl1_tti_enum_type    tdscurrent_tti;


/* The configuration parameters are stored in here */
tdsmac_ul_config_type    tdsul_cfginfo;

/* TRUE implies MAC received a new configuration parameters from RRC and waiting    */
/* L1 signal to actually configure. FALSE implies there are no pending configuration*/
/* requests now */
boolean               tdsul_configuration_pending=FALSE;



extern uint8  tdsmac_tm_bearers_count;

/* The debug messages in UL will be printed every 500 ms. */
uint8 tdsmac_ul_debug_print_interval = 0xF;

/* Poking this flag to TRUE at run time will provide debug messages to see where the
ciphering went wrong */
extern boolean tdsciphering_garbled;

boolean                   tdsmac_increment_hfn_on_rollover[TDSMAX_CN_DOMAIN];
tdsmac_ciphering_table       tdscfn_ciphering_table[256];

/* Ordered ciphering config data */
tdsmac_cipher_config_type    tdscur_ul_cipher_info[TDSMAX_CN_DOMAIN];

/* Before the ciphering activation time, the ciphering configuration that is
received from RRC is stored in this structure */
tdsmac_cipher_config_type    tdsnew_ul_cipher_info[TDSMAX_CN_DOMAIN];

/* TRUE indicates that there is a ciphering configuration that is pending*/
boolean                   tdsul_cipher_activation_pending[TDSMAX_CN_DOMAIN];

#define TDSINVALID_PREV_CFN 0xFFFF

/* Save the prev cfn value sent by physical layer every TTI */
uint16                     tdsprev_cfn= TDSINVALID_PREV_CFN;

/* Count C value for each of the domain */
uint32                    tdscount_c[TDSMAX_CN_DOMAIN];

/* Table that keeps track of the HFN associated with the cfn */
uint32                    tdscfn_hfn_table[TDSMAX_CN_DOMAIN][256];

/* This flag will indicate to MAC that it needs to get a new value of HFN from RRC
on reaching the activation time for the first time */
boolean                   tdsget_start_value_to_transmit[TDSMAX_CN_DOMAIN];

boolean                   tdsprev_mac_increment_hfn_on_rollover[TDSMAX_CN_DOMAIN];
tdsmac_ciphering_table       tdsprev_cfn_ciphering_table[256];

/* Ordered ciphering config data */
tdsmac_cipher_config_type    tdsprev_cur_ul_cipher_info[TDSMAX_CN_DOMAIN];

/* Before the ciphering activation time, the ciphering configuration that is
received from RRC is stored in this structure */
tdsmac_cipher_config_type    tdsprev_new_ul_cipher_info[TDSMAX_CN_DOMAIN];

/* TRUE indicates that there is a ciphering configuration that is pending*/
boolean                   tdsprev_ul_cipher_activation_pending[TDSMAX_CN_DOMAIN];

/* Save the prev cfn value sent by physical layer every TTI */
uint16                    tdsold_prev_cfn= TDSINVALID_PREV_CFN;

/* Count C value for each of the domain */
uint32                    tdsprev_count_c[TDSMAX_CN_DOMAIN];

/* Table that keeps track of the HFN associated with the cfn */
uint32                    tdsprev_cfn_hfn_table[TDSMAX_CN_DOMAIN][256];

/* This flag will indicate to MAC that it needs to get a new value of HFN from RRC
on reaching the activation time for the first time */
boolean                   tdsprev_get_start_value_to_transmit[TDSMAX_CN_DOMAIN];


tdsl1_ul_tfc_pwr_state_enum_type       tdstfc_excess_pwr_state[TDSUE_MAX_TFC];

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT)                  \
    || defined(FEATURE_TDSCDMA_DMO_SUPPORT)                             \
    || defined(FEATURE_TDSCDMA_DSDS_QTA)
boolean       tdstfc_dmo_allow_state[TDSUE_MAX_TFC];
#endif

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
/* Indicates if the loopback test mode is 1, 2 or invalid*/
tdsmac_loop_back_mode_e_type tdsloopback_test_mode;
#endif

// This stores the information regarding the codec type for this call.
extern uint32    tdsmac_codec_info;

boolean       tdstfc_rlc_size_for_dch_defined=FALSE;

/* This structre will hold information of the transport channels and the logical
channels mapped to it at various priority levels. */

tdsmaci_ul_trch_priority_info_type     **tdsint_ul_trch_priority_info;

/* Thsi structure holds information as to the number of pdus that can be built
for a particular transport channel having a particular transport format */

tdsmaci_ul_trch_tf_type tdsint_ul_trch_tf_info[TDSUE_MAX_TRCH][TDSUE_MAX_TF];

// Indicates if it is PS only, CS only, multirab call.
extern tdsmac_rab_status_e_type   tdsrab_status;

/*-----------------------------------------------------------------------
  Implementation of Rel-5 spec CR 2272.
-----------------------------------------------------------------------*/
tdsmac_saved_cipher_info_type tdssaved_new_ul_cipher_info[TDSMAX_CN_DOMAIN];


#ifdef FEATURE_TDSCDMA_HSUPA
/* This will be used by the HSUPA module to appropriately
set the CFN value for the log packets and also for using
the value for timers*/
uint8 tdsmac_hs_ul_cur_cfn;

extern boolean tdsmac_hs_ul_reconfig_in_progress;

#endif

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
rex_crit_sect_type tdsmac_cipher_param_mutex;
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
#define TDSMAC_QSH_MAX_METRICS 2
tdsmac_qsh_status_e_type tdsmac_qsh_r99_coll_status[TDSCDMA_NUM_SUBS];
tdsmac_qsh_status_e_type tdsmac_qsh_eul_coll_status[TDSCDMA_NUM_SUBS];
tdsmac_qsh_metric_address_type tdsmac_metric_r99_address[TDSCDMA_NUM_SUBS];
tdsmac_qsh_metric_address_type tdsmac_metric_eul_address[TDSCDMA_NUM_SUBS];
tdsmac_qsh_metric_r99_stat_s tdsmac_qsh_r99_metrics;
tdsmac_qsh_metric_eul_stat_s tdsmac_qsh_eul_metrics;

extern void tdsmac_qsh_debug_callback(qsh_client_cb_params_s *param);

//Unit testing help:
qsh_ext_metric_cfg_s tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_MAX_METRICS];

#endif

/*================================================================================================
 FUNCTION NAME  tdsinit_ul_mac_ciphering_params

 DESCRIPTION  This function initialises UL MAC ciphering parameters
================================================================================================*/
void  tdsinit_ul_mac_ciphering_params
(
  void
)
{
  uint8 i;

  for (i=0;i<TDSMAX_CN_DOMAIN;i++)
  {
    /* Clear the memory */
    tdscur_ul_cipher_info[i].cipher_enable = FALSE;
    tdsnew_ul_cipher_info[i].cipher_enable = FALSE;
    tdscur_ul_cipher_info[i].key_idx = 0xFF;
    tdsnew_ul_cipher_info[i].key_idx = 0xFF;

    /* Initialize ciphering pending to FALSE for both CS and PS domains */
    tdsul_cipher_activation_pending[i]=FALSE;

    tdsget_start_value_to_transmit[i]=TRUE;
    tdsmac_increment_hfn_on_rollover[i] = FALSE;

   /*
    * Initialize fields of saved_new_ul_cipher_info here.
    */
    tdssaved_new_ul_cipher_info[i].cipher_info.cipher_enable = FALSE;
    tdssaved_new_ul_cipher_info[i].cipher_info_saved = FALSE;
  }

#ifndef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  rex_init_crit_sect(&tdsmac_cipher_param_mutex);
#endif
#endif
}


#ifdef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
/*================================================================================================
 FUNCTION NAME  tdsinit_ul_mac_crit_sect

 DESCRIPTION  This function initialises UL MAC
================================================================================================*/
void  tdsinit_ul_mac_crit_sect(void)
{

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  rex_init_crit_sect(&tdsmac_cipher_param_mutex);
#endif
  tdsmac_ulhs_crit_sect_init();

}
#endif

/*================================================================================================
 FUNCTION NAME  tdsinit_ul_mac

 DESCRIPTION  This function initialises UL MAC
================================================================================================*/
void  tdsinit_ul_mac
(
void
)
{
  /* Initialize the traffic volume measurements table */
  tdsmac_ul_traffic_volume_init();

  if (tdsl1extif_seq_register_get_cfn_cb_func(tdsmac_update_ciphering_params))
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Registered with L1 for CFN update event");
  }
  else
  {
    ERR_FATAL("Unable to register for CFN update event with L1",0,0,0);
    
  }

  /* Initialize ciphering parameters */
  tdsinit_ul_mac_ciphering_params();


  // Define the tfcs control timer.
  rex_def_timer(&tdsul_tfc_ctrl_tmr_blk,tds_l2_ul_get_tcb(), TDSMAC_UL_TFCS_CONTROL_TMR_EXP_SIG);

  // Memset the current config.
  memset(tdsmac_ul_current_tfcs_control_table, (uint8)TDSTFC_NOT_ALLOWED,
          TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

  memset(tdsmac_ul_default_tfcs_control_table, (uint8)TDSTFC_NOT_ALLOWED,
          TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

  memset(tdsmac_ul_full_tfcs_control_table, (uint8)TDSTFC_NOT_ALLOWED,
          TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

  memset(tdsmac_ul_pending_tfcs_control_table, (uint8)TDSTFC_NOT_ALLOWED,
          TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

  tdsmac_ul_tfcc_config_in_process = FALSE;

  // Initialize
  tdsrlc_status_report_req_info.nchan = 0;

  memset(&tdsl2_ivoice, 0, sizeof(tdsl2_ivoice_type));

#ifdef FEATURE_TDSCDMA_HSUPA
  tdsmac_ulhs_init(TRUE);
#endif
}

/*===============================================================================================
 FUNCTION NAME  tdsproc_ul_cipher_info

 DESCRIPTION  This function processes the Cipher information received from RRC.

===============================================================================================*/
void    tdsproc_ul_cipher_info (tdsmac_cipher_config_type   cipherptr[])
{
  uint8  i;
  uint32 j;

  if ((tdsmac_tm_bearers_count == 0) || (TDSGET_UL_CCTRCH_TYPE == TDSRACH))
  {
    // Entry 0 is for CS domain.
    // Clear the old configuration.
    TDSL2_ACQ_CIPHER_PARAM_LOCK();
    tdscur_ul_cipher_info[0].cipher_enable = FALSE;
    tdsnew_ul_cipher_info[0].cipher_enable = FALSE;
    tdscur_ul_cipher_info[0].key_idx = 0xFF;
    tdsnew_ul_cipher_info[0].key_idx = 0xFF;
    tdsul_cipher_activation_pending[0]=FALSE;
    tdsget_start_value_to_transmit[0]=TRUE;
    tdsmac_increment_hfn_on_rollover[0] = FALSE;
    TDSL2_ACQ_CIPHER_PARAM_UNLOCK();
    return;
  }

  for (i=0;i<TDSMAX_CN_DOMAIN;i++)
  {
    // if ciphering is enabled
    if (cipherptr[i].cipher_enable)
    {
      TDSL2_ACQ_CIPHER_PARAM_LOCK();
      // Check the type of the config received
      switch (cipherptr[i].cipher_cfg_type)
      {
        case TDSHO_STEP1_CIPHER_CFG:

          TDSMAC_MSG3(MSG_LEGACY_HIGH,"Step-1 Cipher cfg, key: %d, act_time: 0x%x, hfn: 0x%x",
            cipherptr[i].key_idx,
            cipherptr[i].ciph_act_time,
            cipherptr[i].hfn);

          // Copy the new ciphering parametrs received to the current cfg directly
          // as there is no new config to switch to at activation time.
          //memcpy(&tdscur_ul_cipher_info[i], &cipherptr[i],sizeof(tdsmac_cipher_config_type));
          memscpy(&tdscur_ul_cipher_info[i], sizeof(tdsmac_cipher_config_type), &cipherptr[i],sizeof(tdsmac_cipher_config_type));

          /* Initialize the cfn hfn table */
          //memset(tdscfn_hfn_table[i], tdscur_ul_cipher_info[i].hfn, 256*(sizeof(uint32)));

          for (j=0;j<256;j++)
          {
            tdscfn_hfn_table[i][j] = cipherptr[i].hfn;
            tdscfn_ciphering_table[j].cipher_algo = cipherptr[i].cipher_algo;
            tdscfn_ciphering_table[j].cipher_enable =cipherptr[i].cipher_enable;
            tdscfn_ciphering_table[j].key_idx = cipherptr[i].key_idx;
          }
          TDSMAC_MSG3(MSG_LEGACY_HIGH,"Step-1 Cipher cfg, key: %d, algo: 0x%x, hfn: 0x%x",
            tdscfn_ciphering_table[0].key_idx,
            tdscfn_ciphering_table[0].cipher_algo,
            tdscfn_hfn_table[0][0]);

          // Indicate that there is a new config to which MAC has to switch to at
          // ciphering activation time.

          tdsul_cipher_activation_pending[i]=FALSE;

          /* Initialize the tdsprev_cfn to 0 */
          tdsprev_cfn = TDSINVALID_PREV_CFN;
          TDSMAC_MSG1(MSG_LEGACY_HIGH,"Prev cfn 0x%x",tdsprev_cfn);

          break;

        case TDSSMC_INITIATED_NON_HO_CIPHER_CFG:
          TDSMAC_MSG3(MSG_LEGACY_HIGH,"SMC Initiated Cipher cfg key %d, act_time: 0x%x, hfn: 0x%x",
            cipherptr[i].key_idx,
            cipherptr[i].ciph_act_time,
            cipherptr[i].hfn);
          TDSMAC_MSG1(MSG_LEGACY_HIGH,"SMC Initiated Cipher cfg algo %d",cipherptr[i].cipher_algo);

          /*-----------------------------------------------------------------------
             We have received a new ciphering configuration while the previous one
             is still pending. Save it for future use. Rel-5 Spec CR 2272.
           -----------------------------------------------------------------------*/
          if (tdsul_cipher_activation_pending[i])
          {
            /*memcpy(&tdssaved_new_ul_cipher_info[i].cipher_info, &cipherptr[i],
              sizeof(tdsmac_cipher_config_type));*/

            memscpy(&tdssaved_new_ul_cipher_info[i].cipher_info,
                    sizeof(tdsmac_cipher_config_type),
                    &cipherptr[i],
              sizeof(tdsmac_cipher_config_type));
            tdssaved_new_ul_cipher_info[i].cipher_info_saved = TRUE;
          }
          else
          {
            /* Store the new configuration recvd and switch to it at the cfn*/
            // Copy the new ciphering parametrs received.
            //memcpy(&tdsnew_ul_cipher_info[i], &cipherptr[i],sizeof(tdsmac_cipher_config_type));
            memscpy(&tdsnew_ul_cipher_info[i], sizeof(tdsmac_cipher_config_type), &cipherptr[i],sizeof(tdsmac_cipher_config_type));

            /* Store the new configuration recvd and switch to it at the cfn*/
            // Copy the new ciphering parametrs received.
            //memcpy(&tdscur_ul_cipher_info[i], &cipherptr[i],sizeof(tdsmac_cipher_config_type));
            memscpy(&tdscur_ul_cipher_info[i], sizeof(tdsmac_cipher_config_type), &cipherptr[i],sizeof(tdsmac_cipher_config_type));

            // Indicate that there is a new config to which MAC has to switch to at
            // ciphering activation time.
            tdsul_cipher_activation_pending[i] = TRUE;
            tdsmac_increment_hfn_on_rollover[i] = FALSE;
            /* Initialize the tdsprev_cfn to 0 */
            tdsprev_cfn = TDSINVALID_PREV_CFN;
          }
          break;

        case TDSHO_STEP2_CIPHER_CFG:
          TDSMAC_MSG3(MSG_LEGACY_HIGH,"Step-2 Cipher cfg key 0x%x, act_time 0x%x, hfn 0x%x",
            cipherptr[i].key_idx,
            cipherptr[i].ciph_act_time,
            cipherptr[i].hfn);

       case TDSNON_HO_STEP2_CIPHER_CFG: /*lint !e616*/
          TDSMAC_MSG3(MSG_LEGACY_HIGH,"NON HO Step-2 Cipher cfg key 0x%x, act_time 0x%x, hfn 0x%x",
            cipherptr[i].key_idx,
            cipherptr[i].ciph_act_time,
            cipherptr[i].hfn);

        case TDSNON_HO_CIPHER_CFG: /*lint !e616*/
          TDSMAC_MSG3(MSG_LEGACY_HIGH,"Non-HO Cipher cfg key 0x%x, act_time 0x%x, hfn 0x%x",
            cipherptr[i].key_idx,
            cipherptr[i].ciph_act_time,
            cipherptr[i].hfn);


          if ((cipherptr[i].cipher_cfg_type==TDSNON_HO_CIPHER_CFG) &&
            (cipherptr[i].key_idx == tdscur_ul_cipher_info[i].key_idx))
          {
            TDSMAC_MSG1(MSG_LEGACY_HIGH,"Prev cfn 0x%x",tdsprev_cfn);

            break;
          }

          /* Initialize the tdsprev_cfn to invalid */
          tdsprev_cfn = TDSINVALID_PREV_CFN;
          TDSMAC_MSG3(MSG_LEGACY_HIGH,"Prev cfn 0x%x prev_key %d, cur_key %d", tdsprev_cfn,tdscur_ul_cipher_info[i].key_idx ,
            cipherptr[i].key_idx);

          /*--------------------------------------------------------------------------
          If the parameters have changed, then copy them to tdsnew_ul_cipher_info
          and switch to the new configuration at ciphering activation time which is
          provided by the CFN number.
          ---------------------------------------------------------------------------*/

          /* Store the new configuration recvd and switch to it at the cfn*/
          // Copy the new ciphering parametrs received.
          //memcpy(&tdsnew_ul_cipher_info[i], &cipherptr[i],sizeof(tdsmac_cipher_config_type));
          memscpy(&tdsnew_ul_cipher_info[i], sizeof(tdsmac_cipher_config_type),&cipherptr[i],sizeof(tdsmac_cipher_config_type));


          // Indicate that there is a new config to which MAC has to switch to at
          // ciphering activation time.
          tdsul_cipher_activation_pending[i]=TRUE;

          /*-------------------------------------------------------------------------
          If previously ciphering was not enabled and now it is enabled, then copy the
          ciphering parameters to the both current & new cipher configuratiosn that
          MAC maintains.
          -------------------------------------------------------------------------*/
          if (!tdscur_ul_cipher_info[i].cipher_enable)
          {
            /* Store the new configuration recvd and switch to it at the cfn*/
            // Copy the new ciphering parametrs received.
            //memcpy(&tdscur_ul_cipher_info[i], &cipherptr[i],sizeof(tdsmac_cipher_config_type));
            memscpy(&tdscur_ul_cipher_info[i], sizeof(tdsmac_cipher_config_type), &cipherptr[i],sizeof(tdsmac_cipher_config_type));

            // Initialize
            tdsget_start_value_to_transmit[i]=TRUE;

          }
          else if (TDSNON_HO_CIPHER_CFG == cipherptr[i].cipher_cfg_type
            && tdscur_ul_cipher_info[i].cipher_enable)
          {
            TDSMAC_MSG0(MSG_LEGACY_HIGH,"Roll-over case for Call-Wait");
            tdsmac_increment_hfn_on_rollover[i] = TRUE;
          }

          if ((tdsmac_increment_hfn_on_rollover[i] == FALSE) &&
            (cipherptr[i].cipher_cfg_type != TDSHO_STEP2_CIPHER_CFG))
          {
            for (j=0;j<256;j++)
            {
              tdscfn_hfn_table[i][j] = cipherptr[i].hfn;
            }
          }
          break;

        default:
          break;
      } /* end switch */
      TDSL2_ACQ_CIPHER_PARAM_UNLOCK();
    } /* end if ciphering enabled */
    else if (!cipherptr[i].cipher_enable)
    /* if ciphering is disabled */
    {
      TDSL2_ACQ_CIPHER_PARAM_LOCK();
      // Clear the old configuration.
      tdscur_ul_cipher_info[i].cipher_enable = FALSE;
      tdsnew_ul_cipher_info[i].cipher_enable = FALSE;
      tdscur_ul_cipher_info[i].key_idx = 0xFF;
      tdsnew_ul_cipher_info[i].key_idx = 0xFF;
      tdsul_cipher_activation_pending[i]=FALSE;
      tdsget_start_value_to_transmit[i]=TRUE;
      tdsmac_increment_hfn_on_rollover[i] = FALSE;
      TDSL2_ACQ_CIPHER_PARAM_UNLOCK();	
    } /* end else */

  } // for number of domains

} /* end function */


/*===============================================================================================
FUNCTION NAME tdsproc_ul_rnti_info

PURPOSE     This function processes UL RNTI information received from RRC
===============================================================================================*/
void    tdsproc_ul_rnti_info
(
tdsmac_rnti_config_type   *rntiptr)
{
  /* Process CRNTI info based on RNTI validity flasg */
  switch (rntiptr->rnti_valid)
  {
    /* CRNTI is valid or BOTH valid */
    case TDSCRNTI_VALID:
    case TDSBOTH_VALID:
      tdsul_mac_crnti_valid = TRUE;
      tdsul_mac_crnti = rntiptr->crnti;
      break;

      /* No valid C-RNTI */
    default:
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"No valid C-RNTI");
      tdsul_mac_crnti_valid = FALSE;
      break;
  }
}

/*=================================================================================================
 FUNCTION NAME    tdsproc_ul_cctrch_info

 DESCRIPTION    This function processes uplink CCTrCh information
=================================================================================================*/
void    tdsproc_ul_cctrch_info
(
  /* Pointer to ordered config data of UL CCTrCh */
  tdsmac_ul_cctrch_config_type *cctrch_ptr
)
{
  /* Update info based on CCTrCh type */
  switch (cctrch_ptr->cctrch_type)
  {

    /* RACH type CCTrCh */
    case TDSRACH:

      /* Update UL CCTrCh info */
      /* This is internally used by MAC */
      tdsint_ul_cctrch_info.cctrch_type = TDSRACH;
      tdsint_ul_cctrch_info.ntrch = 1;

      /* Process RACH configuration data */
      tdsproc_rach_cctrch_cfg (&cctrch_ptr->rach_info);

      (void)tdspopulate_rach_trch_id();

      tdsmac_rach_rxd_enable = FALSE;
      /*------------------------------------------------------------------------------
      PRACH would always be enabled. Hence RLC need not trigger MAC to enable PRACH
      when it sees data in its buffer.
      -------------------------------------------------------------------------------*/
      break;

      /* DCH type CCTrCh */
    case TDSDCH:

      /* Update UL CCTrCh info */
      /* This is internally used by MAC */
      tdsint_ul_cctrch_info.cctrch_type = TDSDCH;
      tdsint_ul_cctrch_info.ntrch = cctrch_ptr->dch_info.ndchs;

      /* Process uplink DCH configuration data */
      tdsproc_ul_dch_cctrch_cfg (&cctrch_ptr->dch_info);


      break;

      /* Invalid CCTrCh type -- Set CCTrCh type to TDSINVALID */
    default:
      tdsint_ul_cctrch_info.cctrch_type = TDSINVALID_UL_CCTRCH;
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Uplink CCTrCh released",cctrch_ptr->cctrch_type);
      break;
  }
}

/*==============================================================================================
 FUNCTION NAME  tdsconfig_ul_mac

 PURPOSE    This function processes UL MAC Configuration Data
===============================================================================================*/
void    tdsconfig_ul_mac
(
  /* Pointer to ordered config data of UL MAC */
  tdsmac_ul_config_type    *cfgptr
)
{
  int i,j;
#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING
  uint8 tdstrch_id[TDSUE_MAX_TRCH];
#endif

  tdsmac_ul_allocate_mem_ZI_opt();

  /* If no configuration parameters have actually changed in MAC then copy only the
  TFCS ptr and nothing else */
  if (cfgptr->mac_config_changed == TDSMAC_CFG_NOT_CHANGED)
  {
    /*for (i=0;i<tdsoc_ul_tfcs_ptr->ntfc;i++)
    {
      memcpy(&cfgptr->tfcs_ptr->tfc_info[i].control_flag,
             &tdsoc_ul_tfcs_ptr->tfc_info[i].control_flag, sizeof(tdsmac_ul_tfc_control_e_type));
    }
    tdsoc_ul_tfcs_ptr = cfgptr->tfcs_ptr;*/
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC config not changed");
    return;
  }

  /* If only need to update UL RNTI configuration parameters, update RNTI then return */
  if (cfgptr->mac_config_changed == TDSMAC_CFG_UPDATE_URNTI_ONLY)
  {

    TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

    /* Process UL MAC RNTI information */
    tdsproc_ul_rnti_info (&cfgptr->rnti_info);

    if ((cfgptr->cctrch_ptr != NULL) && (cfgptr->cctrch_ptr->cctrch_type == TDSRACH))
    {  
      tdsproc_update_rach_mac_header(&(cfgptr->cctrch_ptr->rach_info));
    }

    TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
		
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC config only changed ul rnti");
    return;
  }

  /* If only need to update UL ciphering  parameters, update ciphering parameters then return */
  if (cfgptr->mac_config_changed == TDSMAC_CFG_UPDATE_CIPH_CFG_ONLY)
  {

    TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

    /* Process UL MAC Ciphering information */
    tdsproc_ul_cipher_info (cfgptr->cipher_info);

    TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
		
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC config only changed ul ciphering parameters");
    return;
  }

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  if ((cfgptr->mac_config_changed == TDSMAC_LB_CFG_CHANGED) ||
      (cfgptr->mac_config_changed == TDSMAC_LB_AND_UL_CFG_CHANGED))
  {
    // Clear up the loopback mode type.s
    tdsloopback_test_mode = TDSMAC_LOOPBACK_NA;

    /* Store the Transport channel IDs */
    if (cfgptr->cctrch_ptr->cctrch_type == TDSRACH)
    {
      /* Indicated if the TC is in loopback mode 1 or 2 or not applicable */
      if (TDSMAC_LOOPBACK_MODE_1 == cfgptr->cctrch_ptr->rach_info.loopback_test_mode)
      {
        tdsloopback_test_mode = TDSMAC_LOOPBACK_MODE_1;
        // Loopback mode 2 can not be supported in RACH state.
        TDSMAC_MSG0(MSG_LEGACY_HIGH,"Loopback mode 1 config recvd in FACH state");
      }
    }
    else if (cfgptr->cctrch_ptr->cctrch_type == TDSDCH)
    {
      tdsloopback_test_mode = cfgptr->cctrch_ptr->dch_info.loopback_test_mode;
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Loopback mode %d config recvd in TDSDCH state",tdsloopback_test_mode);
    }

    if (cfgptr->mac_config_changed == TDSMAC_LB_CFG_CHANGED)
        return;
  }
#endif
  /*else*/
  {
    
    TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);


    /* Clear uplink logical channel information table */
    memset (tdsint_ul_lc_info, 0x00, TDSUE_MAX_UL_LOGICAL_CHANNEL * sizeof(tdsul_lc_info_type));

    /* Clear uplink transport channel information table */
    memset (tdsint_ul_trch_info, 0x00, TDSUE_MAX_TRCH * sizeof(tdsul_trch_info_type));

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
    // Clear up the loopback mode type.s
    if (cfgptr->mac_config_changed == TDSMAC_CFG_CHANGED)
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"No LB cfg changed,only MAC cfg changed");
      tdsloopback_test_mode = TDSMAC_LOOPBACK_NA;
    }
#endif

    /* Initialize the id to idx map table */
    for (i=0;i<MAX_TRCH;i++)
    {
      tdsul_trch_id_to_idx_table[i].valid= FALSE;
    }

    /* Initialize */
    for (i=0;i<TDSUE_MAX_TRCH;i++)
    {
      for (j=0;j<TDSMAC_UL_MAX_MLPS; j++)
      {
        tdsint_ul_trch_priority_info[i][j].num_lc = 0;
        tdsint_ul_trch_priority_info[i][j].total_bo = 0;
      }
    }


    /* Process UL MAC RNTI information */
    tdsproc_ul_rnti_info (&cfgptr->rnti_info);

    /* Process RACH Transmit Control Parameters */
    tdsproc_rach_tx_parameters (&cfgptr->rach_tx_cntl_info);

    /* Update uplink logical channel information */
    /* This information is internally used by MAC */
    if (cfgptr->cctrch_ptr != NULL)
      tdsproc_ul_cctrch_info (cfgptr->cctrch_ptr);

    /* Process UL MAC Ciphering information */
    tdsproc_ul_cipher_info (cfgptr->cipher_info);

    /* Process TFCS information */
    if (cfgptr->tfcs_ptr != NULL)
      {
        if(cfgptr->new_tfcs_list_present == TRUE)
        {
          TDSMAC_MSG0(MSG_LEGACY_HIGH,"TFCC: Processing UL TFCS info");
          tdsproc_ul_tfcs_info (cfgptr->tfcs_ptr);
        }
        else if( tdsoc_ul_tfcs_ptr != NULL)
        {
          /* MAC should always update the TFS information sent by RRC */
          /* Clear uplink TFCS information table internally used by MAC */
          memset (tdsmac_ul_tfcs_table.tfs_info, 0, 
                     (TDSUE_MAX_TRCH * sizeof(tdsmac_ul_tfs_type)));
          
          /* Copy config TFS info to the TFS table internally used by MAC */
          /*memcpy (tdsmac_ul_tfcs_table.tfs_info, cfgptr->tfcs_ptr->tfs_info, 
                     (TDSUE_MAX_TRCH * sizeof(tdsmac_ul_tfs_type)));*/
          memscpy (tdsmac_ul_tfcs_table.tfs_info, 
                   (TDSUE_MAX_TRCH * sizeof(tdsmac_ul_tfs_type)),
                   cfgptr->tfcs_ptr->tfs_info, 
                     (TDSUE_MAX_TRCH * sizeof(tdsmac_ul_tfs_type)));
          /* Save TFS Info into the ordered TFC config data */
          /*memcpy (tdsoc_ul_tfcs_ptr->tfs_info, cfgptr->tfcs_ptr->tfs_info, 
                     (TDSUE_MAX_TRCH * sizeof(tdsmac_ul_tfs_type)));*/

          memscpy (tdsoc_ul_tfcs_ptr->tfs_info, 
                   (TDSUE_MAX_TRCH * sizeof(tdsmac_ul_tfs_type)),
                   cfgptr->tfcs_ptr->tfs_info, 
                     (TDSUE_MAX_TRCH * sizeof(tdsmac_ul_tfs_type)));

          TDSMAC_MSG0(MSG_LEGACY_HIGH,"TFS inforamtion is intialized ");
        }
        else
        {
          TDSMAC_MSG0(MSG_LEGACY_ERROR," tdsoc_ul_tfcs_ptr is NULL");
        }

      /* Store the PDU size during re-configuration */
        tdsmac_ul_assign_pdu_size();

#ifdef FEATURE_TDSCDMA_TFC_ROUNDROBIN
        tdsmac_tfc_roundrobin_prev_tfci = TDSMAC_INVALID_PREV_TFCI;
#endif
      }

    TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);

    
#ifdef FEATURE_TDSCDMA_MAC_QXDM_LOGGING

    /* Send DL MAC configuration data to QXDM */
    tdsmac_ul_send_cfg_log_packet(&cfgptr);
    tdsmac_ul_send_rach_param_packet(&cfgptr->rach_tx_cntl_info);

    /* Store the Transport channel IDs */
    if(cfgptr->cctrch_ptr != NULL)
    {
      if (cfgptr->cctrch_ptr->cctrch_type == TDSRACH)
      {
        /* If CCTRCH type = RACH */
        tdstrch_id[0] = (uint8)cfgptr->cctrch_ptr->rach_info.trch_id;
        if(cfgptr->tfcs_ptr != NULL)
        {        
          tdsmac_ul_send_tfcs_cfg_packet(cfgptr->tfcs_ptr, &tdstrch_id[0]);
        }
      }
      else if (cfgptr->cctrch_ptr->cctrch_type == TDSDCH)
      {
        /* If CCTRCH type = DCH */
        for (j = 0; j < cfgptr->cctrch_ptr->dch_info.ndchs;j++)
        {
          tdstrch_id[j] = (uint8)cfgptr->cctrch_ptr->dch_info.dch_info[j].trch_id;
        }

        if(cfgptr->tfcs_ptr != NULL)
        {
          tdsmac_ul_send_tfcs_cfg_packet(cfgptr->tfcs_ptr, &tdstrch_id[0]);
        }
      }
      else
      {
        TDSMAC_MSG0(MSG_LEGACY_LOW,"Invalid CCTRCH type ");
      }
    }
#endif

  }


  TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);

#ifdef FEATURE_TDSCDMA_HSUPA
  tdsmac_hs_ul_reconfig_in_progress = TRUE;

  TDSMAC_MSG0(MSG_LEGACY_HIGH,"Mac reconfig in progress");

  /* If action is NOOP then check for the reset indicator flag */
  tdsmac_ulhs_config_handler(cfgptr->mac_e_config_ptr,
                               cfgptr->mac_e_tsn_action,
                               cfgptr->mac_e_reset_indicator,
                               cfgptr->mac_eul_action);

#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
  if (log_status(TDSCDMA_MAC_HS_UL_CONFIG_LOG_PACKET))
  {
    tdscdma_mac_hs_ul_config_log_packet(cfgptr);
  }
#endif

#endif

  /* update lch mapping information for tvm. */
  tdsmac_ulhs_update_tvm_lchinfo();

  /* Update the transport channel info list for traffic volume measurements */
  tdsmac_ul_update_trch_info();
  
  TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);




  /* Enable Tx after re-configuring MAC is complete
        and only if reconfigured CCTrCh is a DCH */
  if (TDSGET_UL_CCTRCH_TYPE == TDSDCH)
  {
    tdsblock_transmission = FALSE;
  }

}

/*====================================================================
FUNCTION: tdsmac_ul_compute_amr_rate()

DESCRIPTION:
  This function looks at the uplink tfcs table and computes the new
  rate based on what TFCI's are allowed at this point. Based on the
  allowed TFCI's the rate is computed and vocoder is configured with
  this new rate.

  This function is used to change rates for only the AMR. No effect
  on the PS.

DEPENDENCIES:
  VOCODER has to support multiple rates. RRC will have to support
  multiple rates for AMR.

RETURN VALUE:


SIDE EFFECTS:
  There is a worst case delay of 40ms and we can lose upto one frame
  in the process of siwtching the rates

====================================================================*/
void
tdsmac_ul_compute_amr_rate
(
  tdsmac_amr_mode_info_type *mac_amr_mode_info_ptr
)
{
  tdsul_trch_info_type           *int_tc_ptr;        /* Pointer to Transport channel info */
  tdsul_lc_info_type             *int_lc_ptr;        /* Pointer to Logical channel info */
  tdsul_tfcs_info_type           *tfci_ptr;         /* pointer to TFCS information stored internally in MAC */
  uint8                       ix, iy, iz;
  uint8                       tfi;
  uint16                      lowest_amr_rate_tfci = 0;
  boolean                     found_amr_rate = FALSE;
  uint32                      amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
  uint32                      lowest_amr_rate = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;
  uint8 index=0;
  uint16 amr_class_size[3];
  uint32  current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
  uint16 total_bits;
  boolean found  = TRUE;

  tds_ul_amr_rate_event_type start_event_log;

  start_event_log.num_rates =0;
  start_event_log.codec_type =TDSRRC_NO_CODEC;/*In valid*/
  for(ix=0;ix< 9; ix++)
  {
    start_event_log.amr_rate[ix] = TDSMAC_AMR_MODE_UNDEF;
  }

  /* Initialize the values in the structure to max values */
  mac_amr_mode_info_ptr->amr_rate = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
  mac_amr_mode_info_ptr->lowest_amr_rate = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;
  mac_amr_mode_info_ptr->amr_rate_type = TDSAMR_RATE_UNDEFINED;

  if (tdsmac_enable_amr_debug_dump)
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Get AMR Mapping info");
  }

  /*
  * Get the RLC Logical Channel IDs for UL, and use
  * RB IDs extracted here to get Logical Channel IDs
  * for DL
  */
  for (ix=0; ix < tdsoc_ul_tfcs_ptr->ntfc; ix++)
  {
    index =0;
    found = TRUE;
    /* Since for every rate it iterated, reset the values  */
    amr_class_size[0] =0;
    amr_class_size[1]=0;
    amr_class_size[2]=0;
    
    /* Get the pointer to the tfci information stored internally in MAC */
    tfci_ptr = &tdsint_ul_tfcs_info[ix];

    /* Check if the TFC is allowed*/
    if ((tdsoc_ul_tfcs_ptr->tfc_info[ix].control_flag == TDSTFC_MANDATORY) || 
        ((tdsoc_ul_tfcs_ptr->tfc_info[ix].control_flag == TDSTFC_ALLOWED) && 
         (tfci_ptr->control_flag == TDSTFC_ALLOWED) &&
         (tfci_ptr->ratem_state == TDSTFC_RATEM_PASSED))
       )
    {
      /* Go through the number of transport channels configured*/
      for (iy=0; iy <  tdsint_ul_cctrch_info.ntrch; iy++)
      {

        int_tc_ptr = TDSGET_UL_TRCH_INFO_PTR(iy);

        int_lc_ptr = TDSGET_UL_LC_INFO_PTR(int_tc_ptr->rlc_id[0]);

        if ((int_lc_ptr->lc_type == UE_LOGCHAN_DTCH) &&
            (int_lc_ptr->rlc_mode == UE_MODE_TRANSPARENT))
        {
          /* Since we allocated for only 3 RBs for voice */
          if(index == 3 )
          {
            TDSMAC_MSG1(MSG_LEGACY_HIGH,"only 3 transport channel allowed index> %d so break ",index);
            break;
          }
          /* Get the transport format index for TFC "ix" and TFI corresponding to Transoprt channel "iy"*/
          tfi = tdsoc_ul_tfcs_ptr->tfc_info[ix].tfi[iy];
          if (tdsmac_enable_amr_debug_dump)
          {
            TDSMAC_MSG3(MSG_LEGACY_HIGH,"tb size %d, num blks %d tfi %d",tdsoc_ul_tfcs_ptr->tfs_info[iy].tf_info[tfi].tb_size,tdsoc_ul_tfcs_ptr->tfs_info[iy].tf_info[tfi].ntblks, tfi);
          }
          /* If the number of transport blocks are > 0 */
          if (tdsoc_ul_tfcs_ptr->tfs_info[iy].tf_info[tfi].ntblks > 0)
          {
		  	if(index < 3)
		  	{
              /* Check the TB_size*/
              amr_class_size[index] = tdsoc_ul_tfcs_ptr->tfs_info[iy].tf_info[tfi].tb_size;
                     /* update the rlc_id, rb_id for Class A*/
              index++;
		  	}
          }
        }
      }


      total_bits = amr_class_size[0] + amr_class_size[1] +amr_class_size[2];
      if (tdsmac_enable_amr_debug_dump)
      {
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"Total bits %d in amr mode",total_bits);
      }
      switch(total_bits)
      {
        case TDSAMR_WB_MODE_660_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_0660;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found WB-AMR rate 6.60 K");
        }
        break;
        case TDSAMR_WB_MODE_885_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_0885;
          TDSMAC_MSG0(MSG_LEGACY_HIGH,"found WB-AMR rate 8.85 K");
        }
        break;
        case TDSAMR_WB_MODE_1265_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1265;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found WB-AMR rate 12.65 K");
        }
        break;
        case TDSAMR_WB_MODE_1425_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1425;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found WB-AMR rate 14.25 K");
        }
        break;
        case TDSAMR_WB_MODE_1585_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1585;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found WB-AMR rate 15.85 K");
        }
        break;
        case TDSAMR_WB_MODE_1825_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1825;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found WB-AMR rate 18.25 K");
        }
        break;
        case TDSAMR_WB_MODE_1985_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1985;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found WB-AMR rate 19.85 K");
        }
        break;
        case TDSAMR_WB_MODE_2305_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_2305;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found WB-AMR rate 23.05 K");
        }
        break;
        case TDSAMR_WB_MODE_2385_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_2385;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found WB-AMR rate 23.85 K");
        }
        break;
        case TDSAMR_MODE_122_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_1220;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found NB-AMR rate 12.20 K");
        }
        break;
        case TDSAMR_MODE_102_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_1020;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found NB-AMR rate 10.20 K");
        }
        break;
        case TDSAMR_MODE_795_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0795;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found NB-AMR rate 7.95 K");
        }
        break;
        case TDSAMR_MODE_740_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0740;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found NB-AMR rate 7.40 K");
        }
        break;
        case TDSAMR_MODE_670_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0670;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found NB-AMR rate 6.70 K");
        }
        break;
        case TDSAMR_MODE_590_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0590;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found NB-AMR rate 5.90 K");
        }
        break;
        case TDSAMR_MODE_515_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0515;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found NB-AMR rate 5.15 K");
        }
        break;
        case TDSAMR_MODE_475_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
          TDSMAC_MSG0(MSG_LEGACY_HIGH," found NB-AMR rate 4.75 K");
        }
        break;
        default:
          if((total_bits == TDSAMR_CLASS_NO_DATA_FRAME_SIZE)
             || (total_bits == TDSAMR_WB_CLASS_A_BIT_SIZE)
             ||(total_bits == TDSAMR_MODE_8_CLASS_A_BIT_SIZE))
          {
            TDSMAC_MSG1(MSG_LEGACY_MED,"SID frames found or No dat frames found %d",total_bits);
          }
          else
          {
            TDSMAC_MSG1(MSG_LEGACY_MED,"wrong total size %d for  AMR",total_bits);
          }
          found = FALSE;
        break;
      }
      if(found == TRUE)
      {
        boolean rate_present_in_event = FALSE;
        found_amr_rate = TRUE;
        
        if (tdsmac_enable_amr_debug_dump)
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"Total bits %d and  amr mode is %d",total_bits,current_amr_mode);
        }
        
        for(iz =0; (iz<start_event_log.num_rates) && (iz<(sizeof(start_event_log.amr_rate)/sizeof(tdsrrc_event_ul_amr_rate_type)));iz++)
        {
          
            if(start_event_log.amr_rate[iz] == ((tdsrrc_event_ul_amr_rate_type)current_amr_mode) )
            {
              rate_present_in_event = TRUE;
              break;
            }

        }
        if((rate_present_in_event == FALSE) && 
            (start_event_log.num_rates < (sizeof(start_event_log.amr_rate)/sizeof(tdsrrc_event_ul_amr_rate_type))))
        {
            start_event_log.amr_rate[start_event_log.num_rates] =
                (tdsrrc_event_ul_amr_rate_type) current_amr_mode;
            start_event_log.num_rates++;
        }
        
        if( amr_mode < current_amr_mode)
        {
          if (tdsmac_enable_amr_debug_dump)
          {
            TDSMAC_MSG2(MSG_LEGACY_HIGH,"Multirate wb-amr lower amr rate %d higher rate %d ",amr_mode,current_amr_mode);
          }
          amr_mode = current_amr_mode;
        }

        if(current_amr_mode < lowest_amr_rate)
        {
          lowest_amr_rate_tfci = ix;
        }
        lowest_amr_rate = MIN(current_amr_mode,lowest_amr_rate);  
      }

    }
  }
  
  /* EVENT_WCDMA_UL_AMR_RATE - logs the All UL AMR allowed Rates */
  start_event_log.codec_type =TDSRRC_NB_AMR_CODEC;
  event_report_payload(EVENT_TDSCDMA_MAC_UL_AMR_RATE, sizeof(start_event_log),
                       (void *)&start_event_log);
  
  if (found_amr_rate)
  {
    mac_amr_mode_info_ptr->amr_rate = amr_mode;
    mac_amr_mode_info_ptr->lowest_amr_rate = lowest_amr_rate;
    mac_amr_mode_info_ptr->lowest_amr_rate_tfci = lowest_amr_rate_tfci;
    
    tfci_ptr = &tdsint_ul_tfcs_info[lowest_amr_rate_tfci];
    tfci_ptr->control_flag = TDSTFC_MANDATORY;
    tdsoc_ul_tfcs_ptr->tfc_info[lowest_amr_rate_tfci].control_flag = TDSTFC_MANDATORY;

    mac_amr_mode_info_ptr->amr_rate_type =
    ((amr_mode == lowest_amr_rate) ? TDSSINGLE_RATE_AMR : TDSMULTI_RATE_AMR);
    if (tdsmac_enable_amr_debug_dump)
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"AMR rate %d",mac_amr_mode_info_ptr->amr_rate);
    }
  }
  else
  {
    mac_amr_mode_info_ptr->amr_rate = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;
    mac_amr_mode_info_ptr->lowest_amr_rate = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;
  }
} /* end tdsmac_ul_compute_amr_rate() */

/*==============================================================================================
 FUNCTION NAME  void tdsmac_ul_set_amr_rate

 DESCRIPTION : This fn. will set the vocoder rate
===============================================================================================*/
void tdsmac_ul_set_amr_rate(uint32 amr_mode)
{
  tdscdma_ivoice_event_request_codec_mode_t params;

  params.codec_mode = amr_mode;

  if (amr_mode < TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF)
  {
    if((tdsmac_codec_info == TDSCDMA_IVOCODER_ID_AMR) ||
        (tdsmac_codec_info == TDSCDMA_IVOCODER_ID_AMRWB))
    {
      if( tdsl2_ivoice.ivoice_cb!= NULL)
      {
        tdsl2_ivoice.ivoice_cb(tdsrrc_get_as_id(), TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE, &params, sizeof(tdscdma_ivoice_event_request_codec_mode_t));
        TDSMAC_MSG0(MSG_LEGACY_HIGH,"send EVENT_REQUEST_CODEC_MODE");
      }
      else
      {
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"AMR event CB is NULL");
      }
    }
    else
    {
      TDSMAC_MSG1(MSG_LEGACY_ERROR,"MVS codec configured is %d",tdsmac_codec_info);
    }
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"MVS AMR mode not SET !!");
  }
}



#ifdef FEATURE_QSH_EVENT_METRIC

/*===========================================================================
FUNCTION tdsmac_send_qsh_cmd

DESCRIPTION
  Send local cmd TDSL2_QSH_CMD to L2- MAC once its received from MAC CB from QSH . it's called by the tds qsh timer registered callback
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsmac_send_qsh_cmd( qsh_client_cb_params_s *param )
{
  tdsl2_ul_cmd_type *mac_cmd_ptr; /* local command pointer */

  /* send MSG to trigger log packet output */
  if ((mac_cmd_ptr = tdsl2_ul_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("MAC_QSH::NULL Ptr when sending TDSL2_QSH_CMD ",0,0,0);
  }
  else
  {
    mac_cmd_ptr->cmd_hdr.cmd_id= TDSMAC_QSH_CMD;
    mac_cmd_ptr->cmd_data.tdsmac_qsh_cmd.param= *param;
    tdsl2_ul_put_cmd( mac_cmd_ptr );
	TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH::Sent the QSH command to MAC");
    
  }
}




/*===========================================================================
FUNCTION tdsmac_qsh_debug_callback

DESCRIPTION
This is the callback function for QSH to collect data from TDSMAC. The callback function will 
send a local command to L2-MAC to trigger the metric collection.
  
DEPENDENCIES
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsmac_qsh_debug_callback(qsh_client_cb_params_s *param)
{
  ASSERT(param != NULL);
  
  switch(param->action)
  {
    
#ifdef FEATURE_QSH_EVENT_METRIC

    case QSH_ACTION_METRIC_CFG:
    case QSH_ACTION_METRIC_TIMER_EXPIRY:
      
      tdsmac_send_qsh_cmd(param);
      
      break;
#endif

#ifdef FEATURE_QSH_DUMP
    case QSH_ACTION_DUMP_COLLECT:
      /* placeholder for QSH DUMP support */
    break;
#endif

    default:
      break;
  }
}

/*===========================================================================

FUNCTION TDSMAC_QSH_TC_METRICS_INIT

DESCRIPTION
  Initializing the metrics for test support
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmac_qsh_tc_metrics_init(void)
{


   qsh_client_metric_cfg_init(tdsmac_qsh_metric_cfg_arr,TDSMAC_QSH_MAX_METRICS);
   
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_R99_STAT].id = TDSMAC_QSH_METRIC_R99_STAT;
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_R99_STAT].sampling_period_ms = 1000;   
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_R99_STAT].subs_id = 0;
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_R99_STAT].fifo.element_size_bytes = sizeof(tdsmac_qsh_metric_r99_stat_s);
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_R99_STAT].fifo.element_count_total = 10;

   
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_EUL_STAT].id = TDSMAC_QSH_METRIC_EUL_STAT;
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_EUL_STAT].sampling_period_ms = 1000;
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_EUL_STAT].subs_id = 0;
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_EUL_STAT].fifo.element_size_bytes = sizeof(tdsmac_qsh_metric_eul_stat_s);
   tdsmac_qsh_metric_cfg_arr[TDSMAC_QSH_METRIC_EUL_STAT].fifo.element_count_total = 10;

}

/*===========================================================================

FUNCTION TDSMAC_QSH_REGISTER

DESCRIPTION
  Registering TDS L2 for QSH metrics collection with QSH
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsmac_qsh_register(void)
{
	qsh_client_reg_s tds_mac_reg_req;
	qsh_client_reg_init(&tds_mac_reg_req);

	//Adding some support for test:
	tdsmac_qsh_tc_metrics_init();
        tds_mac_reg_req.metric_info.metric_cfg_arr_ptr = &tdsmac_qsh_metric_cfg_arr[0];
        tds_mac_reg_req.metric_info.metric_cfg_count = TDSMAC_QSH_MAX_METRICS;
	
	
	tds_mac_reg_req.client = QSH_CLT_TMAC;
	tds_mac_reg_req.client_cb_ptr = tdsmac_qsh_debug_callback;
	tds_mac_reg_req.cb_action_support_mask = (QSH_ACTION_METRIC_CFG|QSH_ACTION_METRIC_TIMER_EXPIRY);
	tds_mac_reg_req.major_ver = TDSMAC_QSH_MAJOR_VER;
	tds_mac_reg_req.minor_ver = TDSMAC_QSH_MINOR_VER;
	qsh_client_reg(&tds_mac_reg_req);
}

/*===========================================================================

FUNCTION tdsmac_update_metric_address

DESCRIPTION
  This will update the local copy of next address and metric context recieved from QSH
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsmac_qsh_write_metrics(tdsmac_qsh_metric_mac_e metric_id, sys_modem_as_id_e_type subs_id)
{

    if(metric_id == TDSMAC_QSH_METRIC_R99_STAT)
   		{
   		memscpy((tdsmac_qsh_metric_r99_stat_s*)(tdsmac_metric_r99_address[subs_id].start_address), sizeof(tdsmac_qsh_metric_r99_stat_s), (tdsmac_qsh_metric_r99_stat_s*)&tdsmac_qsh_r99_metrics, sizeof(tdsmac_qsh_metric_r99_stat_s));
   		TDSMAC_MSG1(MSG_LEGACY_MED,"MAC_QSH::Buffer Occupancy is %d   ",tdsmac_qsh_r99_metrics.mac_r99ul_bo_bytes);
   		}
	else if(metric_id == TDSMAC_QSH_METRIC_EUL_STAT)
		{
		memscpy((tdsmac_qsh_metric_eul_stat_s*)tdsmac_metric_eul_address[subs_id].start_address, sizeof(tdsmac_qsh_metric_eul_stat_s), (tdsmac_qsh_metric_eul_stat_s*)&tdsmac_qsh_eul_metrics, sizeof(tdsmac_qsh_metric_eul_stat_s));
		TDSMAC_MSG1(MSG_LEGACY_MED,"MAC_QSH::Buffer Occupancy is %d   ",tdsmac_qsh_eul_metrics.mac_eul_bo_bytes);
		TDSMAC_MSG1(MSG_LEGACY_MED,"MAC_QSH::Number of SI sent is %d   ",tdsmac_qsh_eul_metrics.num_si_sent_pri);		
		}
		
   
}



void tdsmac_update_metric_address(qsh_metric_id_t metric_id, qsh_client_metric_context_id_t qsh_metric_context_id, sys_modem_as_id_e_type subs_id, uint8* qsh_address )
{

		if(qsh_address == NULL)
			{
				
				ERR_FATAL("RLC_QSH::Next address provided is NULL  ",0,0,0);
				
			}	

		switch(metric_id)                                                                           
  		{                                                                                 
 
    	case TDSMAC_QSH_METRIC_R99_STAT:                                                   
 			{
     		tdsmac_metric_r99_address[subs_id].start_address = (uint8*)qsh_address;
			tdsmac_metric_r99_address[subs_id].mac_qsh_metric_id = metric_id;
			tdsmac_metric_r99_address[subs_id].mac_qsh_metric_cntxt_id = qsh_metric_context_id;
    		}
      	break;   

		case TDSMAC_QSH_METRIC_EUL_STAT:
			{
			tdsmac_metric_eul_address[subs_id].start_address =(uint8*)qsh_address;
			tdsmac_metric_eul_address[subs_id].mac_qsh_metric_id = metric_id;
			tdsmac_metric_eul_address[subs_id].mac_qsh_metric_cntxt_id = qsh_metric_context_id;
			}		
 		break;
   	
    	//If newer metrics are required we can add here:                                                                        
 
    	default:                                                                        
 
      	break;                                                                        
 
  		}                 
		

}


/*===========================================================================

FUNCTION tdsmac_qsh_process_cfg

DESCRIPTION
  This will process the Metric cfg command from QSH. It also saves the start address
  and does a check on alloted size.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/


void tdsmac_qsh_process_cfg(qsh_client_cb_params_s* param)
{

	qsh_metric_id_t metric_id;
	qsh_client_metric_context_id_t metric_context_id;
  sys_modem_as_id_e_type  subs_id;
  	 
  	metric_id = param->action_params.metric_cfg.id;
	  metric_context_id = param->action_params.metric_cfg.metric_context_id;
    subs_id = param->action_params.metric_cfg.subs_id;

    if ((subs_id < SYS_MODEM_AS_ID_1) || (subs_id > (sys_modem_as_id_e_type)(TDSCDMA_NUM_SUBS-1)))
    {
      TDSMAC_MSG1(MSG_LEGACY_MED,"MAC_QSH: invalid sub id %d",subs_id);
      return;
    }
	 
  	if(param->action_params.metric_cfg.action== QSH_METRIC_ACTION_START)
  	{
 		TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH::Rcvd QSH START  command from QSH for MAC ");

		if((metric_id == TDSMAC_QSH_METRIC_R99_STAT) && (param->action_params.metric_cfg.size_bytes >= sizeof(tdsmac_qsh_metric_r99_stat_s)))
			{
				tdsmac_qsh_r99_coll_status[subs_id] = TDSMAC_QSH_R99_METRICS_ENABLED;
				memset( &tdsmac_qsh_r99_metrics, 0, sizeof(tdsmac_qsh_metric_r99_stat_s) );
				tdsmac_update_metric_address(metric_id,metric_context_id, subs_id, param->action_params.metric_cfg.start_addr);
   			}
		else if((metric_id == TDSMAC_QSH_METRIC_EUL_STAT) && (param->action_params.metric_cfg.size_bytes >= sizeof(tdsmac_qsh_metric_eul_stat_s)))
			{
				tdsmac_qsh_eul_coll_status[subs_id] = TDSMAC_QSH_EUL_METRICS_ENABLED;
				memset( &tdsmac_qsh_eul_metrics, 0, sizeof(tdsmac_qsh_metric_eul_stat_s) );
				tdsmac_update_metric_address(metric_id,metric_context_id, subs_id, param->action_params.metric_cfg.start_addr);
    		}
		else
			{
				TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH::Size bytes allocated is less than size of MAC_statss ");
			}
  	}

	if(param->action_params.metric_cfg.action== QSH_METRIC_ACTION_STOP)
  	{
		TDSRLC_MSG0(MSG_LEGACY_HIGH,"RLC_QSH::Rcvd QSH STOP  command from QSH for RLC ");
		if(metric_id == TDSMAC_QSH_METRIC_R99_STAT)
			{
				tdsmac_qsh_r99_coll_status[subs_id] = TDSMAC_QSH_R99_METRICS_DISABLED;
				tdsmac_metric_r99_address[subs_id].start_address = NULL;
				
			}
		else if (metric_id == TDSMAC_QSH_METRIC_EUL_STAT)
			{
				tdsmac_qsh_eul_coll_status[subs_id] =TDSMAC_QSH_EUL_METRICS_DISABLED;
				tdsmac_metric_eul_address[subs_id].start_address = NULL;
			}
			

	}



}



/*===========================================================================

FUNCTION TDSMAC_QSH_COLLECT_REPORT_METRICS

DESCRIPTION
  This function collects the metrics on timer expiry callback. Writes the data on
  to the address provided and then notifies QSH of the data written.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsmac_qsh_collect_report_metrics(qsh_client_cb_params_s* param)
{

	uint8* addr= NULL;
	uint8 index=0;
	qsh_metric_id_t metric_id;
	qsh_client_metric_log_done_s  mac_log_done;
  sys_modem_as_id_e_type subs_id;

	for(index=0; index < param->action_params.metric_timer_expiry.metric_id_count; index++)
	{
		metric_id = param->action_params.metric_timer_expiry.params[index].metric_id;  //Since we have to monitor only one metric in RLC, implicitly checking only first element of array. 
		subs_id =  param->action_params.metric_timer_expiry.params[index].subs_id;

        if ((subs_id < SYS_MODEM_AS_ID_1)||(subs_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
        {
          TDSMAC_MSG1(MSG_LEGACY_MED,"MAC_QSH:: invalid as_id %d ",subs_id);
          continue;
        }
		if (subs_id != tdsrrc_get_as_id())
        {
          TDSMAC_MSG2(MSG_LEGACY_MED,"MAC_QSH:: mismatched as_id. qsh as_id %d and current as_id %d ",subs_id,tdsrrc_get_as_id());
          continue;
        }
		if((tdsmac_qsh_r99_coll_status[subs_id] == TDSMAC_QSH_R99_METRICS_ENABLED)&& (TDSMAC_QSH_METRIC_R99_STAT ==metric_id)&& (tdsmac_ul_lc_info.nchan > 0))
		{
			
			//Collect and report the data 
			TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH:: QSH called by L2-MAC R99 for reporting the metrics ");
			//copy tdsrlc_qsh_metrics to QSH provided memory
			tdsmac_qsh_write_metrics(TDSMAC_QSH_METRIC_R99_STAT, subs_id);
		
			//Notify QSH 
			qsh_client_metric_log_done_init(&mac_log_done);
			mac_log_done.client = QSH_CLT_TMAC;
			mac_log_done.metric_id = metric_id;
			mac_log_done.log_reason = QSH_CLIENT_METRIC_LOG_REASON_TIMER_EXPIRY;
			mac_log_done.metric_context_id = tdsmac_metric_r99_address[subs_id].mac_qsh_metric_cntxt_id;
			
			addr = qsh_client_metric_log_done(&mac_log_done);	
	
			tdsmac_update_metric_address(metric_id,tdsmac_metric_r99_address[subs_id].mac_qsh_metric_cntxt_id, subs_id, addr);

			//Clear the stats for next collection duration
			memset( &tdsmac_qsh_r99_metrics, 0, sizeof(tdsmac_qsh_metric_r99_stat_s) );
			TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH:: In L2-MAC R99 All the metrics and counters cleared for next duration ");
	
		}
		else if((tdsmac_qsh_eul_coll_status[subs_id] == TDSMAC_QSH_EUL_METRICS_ENABLED)&& (TDSMAC_QSH_METRIC_EUL_STAT ==metric_id) && (tdsmac_ulhs_state == TDSMAC_ULHS_ACTIVE))
		{

			//Collect and report the data 
			TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH:: QSH called by L2-MAC EUL  for reporting the metrics ");
			//copy tdsrlc_qsh_metrics to QSH provided memory
			tdsmac_qsh_write_metrics(TDSMAC_QSH_METRIC_EUL_STAT, subs_id);
		
			//Notify QSH 
			qsh_client_metric_log_done_init(&mac_log_done);
			mac_log_done.client = QSH_CLT_TMAC;
			mac_log_done.metric_id = metric_id;
			mac_log_done.log_reason = QSH_CLIENT_METRIC_LOG_REASON_TIMER_EXPIRY;
			mac_log_done.metric_context_id = tdsmac_metric_eul_address[subs_id].mac_qsh_metric_cntxt_id;
			
			addr = qsh_client_metric_log_done(&mac_log_done);	
			
			
	
			tdsmac_update_metric_address(metric_id,tdsmac_metric_eul_address[subs_id].mac_qsh_metric_cntxt_id, subs_id, addr);

			//Clear the stats for next collection duration
			memset( &tdsmac_qsh_eul_metrics, 0, sizeof(tdsmac_qsh_metric_eul_stat_s) );
			TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH:: In L2-MAC EUL All the metrics and counters cleared for next duration ");


		}
		//Do this for other metrics if added in future
		
	}
	

}



/*===========================================================================

FUNCTION TDSMAC_QSH_PROCESS_CMD

DESCRIPTION
  This function is a command handler for QSH commands CFG and TIMER EXPIRY
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsmac_qsh_process_cmd(tdsmac_qsh_command_type* mac_qsh_cmd)
{
	qsh_client_action_done_s cb_done;
	
	TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH::Rcvd command from QSH for RLC ");
	if(mac_qsh_cmd->param.action == QSH_ACTION_METRIC_CFG)
		{
			TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH::Rcvd QSH  command from QSH for MAC ");
			tdsmac_qsh_process_cfg(&(mac_qsh_cmd->param));
			
		}

	else if (mac_qsh_cmd->param.action == QSH_ACTION_METRIC_TIMER_EXPIRY)
		{
			TDSMAC_MSG0(MSG_LEGACY_HIGH,"MAC_QSH::Rcvd Timer Expiry command from QSH for MAC ");
			tdsmac_qsh_collect_report_metrics(&(mac_qsh_cmd->param));
		}

	else
		{
			
			TDSMAC_MSG0(MSG_LEGACY_ERROR,"MAC_QSH::Rcvd wrong command from QSH for MAC,supported only Metric CFG and Timer Expiry cmds ");
		}


	qsh_client_action_done_init(&cb_done);
	cb_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
	cb_done.cb_params_ptr = &(mac_qsh_cmd->param);
	qsh_client_action_done(&cb_done);
	
}

#endif /*FEATURE_QSH_EVENT_METRIC*/



/*=================================================================================================
 FUNCTION NAME  tdsproc_ul_mac_tfcc_command

 PURPOSE    This function process UL TFCC config Commands
================================================================================================*/

void tdsproc_ul_mac_tfcc_command
(
  tdsmac_tfc_subset_restriction_info_type *tfcc_config_ptr
)
{
  uint8                       tfc_idx, ntrch, ntf, tfi,trch_idx,i;
  uint16                      tfci;
  uint16                      *tfci_list_ptr, ntfc;
  tdstrch_id_to_idx_map_type     *id_to_idx_ptr;
  tdsmac_tfc_subset_restriction_trch_info_type *trch_ptr;
  boolean                     status = TRUE;
  tdsrrc_mac_cmd_type                *rrc_cmd_ptr;
  boolean                     found_tfi = FALSE;
  tdsmac_amr_mode_info_type      amr_mode_info;

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  TDSL2_ACQ_UL_LW_LOCK(TDSUE_MAX_UL_LOGICAL_CHANNEL);
#endif
  // Number of TFC's configured by RRC initially.
  if (tdsoc_ul_tfcs_ptr->ntfc == 0)
  {
    status = FALSE;
  }
  else
  {
    //Initialize all entries to not_allowed
    memset(tdsmac_ul_pending_tfcs_control_table,(uint8) TDSTFC_NOT_ALLOWED,
               TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

    switch (tfcc_config_ptr->tfci_restriction_choice)
    {
      case TDSMIN_ALLOWED_TFCI:
        // Get the minimum not_allowed tfi
        tfci = tfcc_config_ptr->restriction_info.tfci ;

        // Check if this TFCI is within the range
        if ((tfci > TDSUE_MAX_TFC) || (tfci > tdsoc_ul_tfcs_ptr->ntfc))
        {
          // If not, then set the status to FALSE.
          status = FALSE;

          TDSMAC_MSG2(MSG_LEGACY_ERROR,"TFCI %d not among the list of configured tfci's %d",tfci,tdsoc_ul_tfcs_ptr->ntfc);

          break; // Breaks ffrom the case sattement
        }

        // Starting from the TFCI listed upto the number of
        for (tfc_idx = 0;
            ((tfc_idx <= tfcc_config_ptr->restriction_info.tfci) && (tfc_idx < TDSUE_MAX_TFC));
             tfc_idx++ )
        {
          if (tdsoc_ul_tfcs_ptr->tfc_info[tfc_idx].control_flag != TDSTFC_REMOVED)
          {
            // Mark those allowed which are less than the min tfci specified.
            tdsmac_ul_pending_tfcs_control_table[tfc_idx] = TDSTFC_ALLOWED;
          }
        }

        TDSMAC_MSG1(MSG_LEGACY_HIGH,"Recvd MIN_ALLOWED %d tfci",tfci);

        break;

      case TDSALLOWED_TFCI:
        ntfc = tfcc_config_ptr->restriction_info.tfci_restriction_info.ntfc;
        tfci_list_ptr = &tfcc_config_ptr->restriction_info.tfci_restriction_info.tfci_list[0];

        TDSMAC_MSG0(MSG_LEGACY_HIGH,"TDSALLOWED TFCI's are ");

        if (ntfc <= tdsoc_ul_tfcs_ptr->ntfc)
        {
          // GO through the list of TFCI's that are allowed
          for (tfc_idx = 0; tfc_idx < ntfc; tfc_idx++)
          {
            // Get the TFCI
            tfci = tfci_list_ptr[tfc_idx];

            if (tfci > tdsoc_ul_tfcs_ptr->ntfc)
            {
              status = FALSE;

              TDSMAC_MSG2(MSG_LEGACY_ERROR,"TFCI %d not among the list of configured tfci's %d",tfci,tdsoc_ul_tfcs_ptr->ntfc);
              break;
            }
            if (tdsoc_ul_tfcs_ptr->tfc_info[tfci].control_flag != TDSTFC_REMOVED)
            {
              tdsmac_ul_pending_tfcs_control_table[tfci] = TDSTFC_ALLOWED;
              TDSMAC_MSG1(MSG_LEGACY_HIGH,"TFCI %d",tfci);
            }
          }
        }
        else
        {
          status = FALSE;
        }

        break;

      case TDSNOT_ALLOWED_TFCI:

        ntfc = tfcc_config_ptr->restriction_info.tfci_restriction_info.ntfc;
        tfci_list_ptr = &tfcc_config_ptr->restriction_info.tfci_restriction_info.tfci_list[0];

        if (ntfc <= tdsoc_ul_tfcs_ptr->ntfc)
        {
          TDSMAC_MSG0(MSG_LEGACY_HIGH,"NOT TDSALLOWED TFCI's are ");

          //Initialize all entries in the TFCS table to allowed
          memset(tdsmac_ul_pending_tfcs_control_table, (uint8)TDSTFC_ALLOWED,
           tdsoc_ul_tfcs_ptr->ntfc * sizeof(tdsmac_ul_tfc_control_e_type));

          // GO through the list of NOT TDSALLOWED TFCI's
          for (tfc_idx = 0; tfc_idx < ntfc; tfc_idx++)
          {
            // Get the tfci
            tfci = tfci_list_ptr[tfc_idx];

            // If tfci exceeds the max_number configured then reject the msg
            if (tfci > tdsoc_ul_tfcs_ptr->ntfc)
            {
              status = FALSE;

              TDSMAC_MSG2(MSG_LEGACY_ERROR,"TFCI %d not among the list of configured tfci's %d",tfci,tdsoc_ul_tfcs_ptr->ntfc);
              break;
            }
            else
            {
              if (tdsoc_ul_tfcs_ptr->tfc_info[tfci].control_flag == TDSTFC_REMOVED)
              {
                tdsmac_ul_pending_tfcs_control_table[tfci] = TDSTFC_NOT_ALLOWED;
                TDSMAC_MSG1(MSG_LEGACY_HIGH,"TFCI %d",tfci);
              }
              // Mark it as NOT allowed
              tdsmac_ul_pending_tfcs_control_table[tfci] = TDSTFC_NOT_ALLOWED;

              TDSMAC_MSG1(MSG_LEGACY_HIGH,"TFCI %d",tfci);
            }
          }
        }

        break;

      case TDSRESTRICTED_TFCI:

        ntrch = tfcc_config_ptr->restriction_info.trch_restriction_info_list.ntrch;

        TDSMAC_MSG0(MSG_LEGACY_HIGH,"Restricted TFCI list is ");

        //Initialize all entries to Allowed
        memset(tdsmac_ul_pending_tfcs_control_table,(uint8) TDSTFC_ALLOWED,
               TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

        for (trch_idx = 0; trch_idx < ntrch; trch_idx++ )
        {
          trch_ptr = &tfcc_config_ptr->restriction_info.
                     trch_restriction_info_list.trch_restriction_info[trch_idx];

          // Get the number of transport formats that are restricted.
          ntf = (uint8)trch_ptr->ntf;

          // Get the info for this transort channel id
          id_to_idx_ptr = TDSGET_UL_TRCH_ID_TO_IDX_PTR(trch_ptr->trch_id);

          // If the transport channel exists in MAC
          if (id_to_idx_ptr->valid)
          {
            // If the number of transport formats specified in the TFCC
            // msg exceeds the number of transport formats specified in the
            // TFCS table, then discard this message and send failure to RRC
            if (ntf > tdsoc_ul_tfcs_ptr->tfs_info[id_to_idx_ptr->idx].ntf)
            {
              status = FALSE;

              TDSMAC_MSG2(MSG_LEGACY_ERROR,"Mismatch in ntf %d in TFCC msg and ntf %d in TFCS table",ntf,tdsoc_ul_tfcs_ptr->tfs_info[id_to_idx_ptr->idx].ntf);
              break;
            }

            for (tfc_idx = 0; tfc_idx < tdsoc_ul_tfcs_ptr->ntfc; tfc_idx++ )
            {
              /* Don't check TFCI which are made as Not allowed by the previous TrCh */
              if (tdsmac_ul_pending_tfcs_control_table[tfc_idx] == TDSTFC_NOT_ALLOWED)
              {
                continue;
              }

              found_tfi = FALSE;

              // Go through the number of tranport formats
              for (i=0;i<ntf;i++)
              {
                // Get the transport format index.
                tfi = trch_ptr->tfi[i];
                TDSMAC_MSG3(MSG_LEGACY_HIGH,"TFCI %d, Trch %d, TF %d", tfc_idx, trch_ptr->trch_id, tfi);
                if (tdsoc_ul_tfcs_ptr->tfc_info[tfc_idx].tfi[id_to_idx_ptr->idx] == tfi)
                {
                  if ((tdsoc_ul_tfcs_ptr->tfc_info[tfc_idx].control_flag != TDSTFC_REMOVED)
                      &&  (tdsmac_ul_pending_tfcs_control_table[tfc_idx] == TDSTFC_ALLOWED))
                  {
                    tdsmac_ul_pending_tfcs_control_table[tfc_idx] = TDSTFC_ALLOWED;
                    TDSMAC_MSG3(MSG_LEGACY_HIGH,"AllowedList : TFCI %d, Trch %d, TF %d", tfc_idx, trch_ptr->trch_id, tfi);
                  }
                  found_tfi=TRUE;
                  break;
                }
              }

              if (found_tfi==FALSE)
              {
                tdsmac_ul_pending_tfcs_control_table[tfc_idx] = TDSTFC_NOT_ALLOWED;
              }
            }
          }
          else
          {
            // The config is invalid.
            status = FALSE;
            break;
          }
        }
        break;

      case TDSFULL_TFCI:
        TDSMAC_MSG0(MSG_LEGACY_HIGH,"Full TFCI choice selected");
        //Initialize all entries in the TFCS table to allowed
        /*memcpy(tdsmac_ul_pending_tfcs_control_table, tdsmac_ul_full_tfcs_control_table,
                 sizeof(tdsmac_ul_full_tfcs_control_table));*/
        memscpy(tdsmac_ul_pending_tfcs_control_table,
                sizeof(tdsmac_ul_pending_tfcs_control_table),
                tdsmac_ul_full_tfcs_control_table,
                 sizeof(tdsmac_ul_full_tfcs_control_table));
				
        for (tfc_idx = 0;tfc_idx < tdsmac_ul_tfcs_table.ntfc; tfc_idx++)
        {
          if(tdsmac_ul_pending_tfcs_control_table[tfc_idx] == TDSTFC_NOT_ALLOWED)
          {
            TDSMAC_MSG1(MSG_LEGACY_HIGH,"Making NOT_ALLOWED TFCI to TDSALLOWED for TFCI tfc_idx %d",tfc_idx);
            tdsmac_ul_pending_tfcs_control_table[tfc_idx] = TDSTFC_ALLOWED;
          }
        }
        break;
      default:
        status = FALSE;
        TDSMAC_MSG0(MSG_LEGACY_ERROR,"Error Not a valid choice");
        break;
      }
  }

  if (tfcc_config_ptr->control_duration > 5120)
  {
    TDSMAC_MSG1(MSG_LEGACY_ERROR,"Control duration is invalid %d",tfcc_config_ptr->control_duration);

    status = FALSE;
  }

  // If all parameters are valid
  if (status == TRUE)
  {
    // Always use the current config. Only when the control duration
    // expires use the default.
    //mac_ul_tfcs_control_ptr_in_use = TDSCURRENT_TFCC_TABLE;

    if (tfcc_config_ptr->control_duration == 0)
    {
      if (tfcc_config_ptr->activation_time == 0)
      {

        tdsmac_ul_tfcc_config_in_process = TRUE;
        
        // In this case bothe curren config and default config will be the same.

        /* Clear uplink TFCS information table internally used by MAC */
        /*memcpy (tdsmac_ul_default_tfcs_control_table,
                tdsmac_ul_pending_tfcs_control_table,
                TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));*/

        memscpy (tdsmac_ul_default_tfcs_control_table,
                 TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type),
                tdsmac_ul_pending_tfcs_control_table,
                TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

        /* Clear uplink TFCS information table internally used by MAC */
        /*memcpy (tdsmac_ul_current_tfcs_control_table,
            tdsmac_ul_pending_tfcs_control_table,
            TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));*/
        memscpy (tdsmac_ul_current_tfcs_control_table,
                 TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type),
            tdsmac_ul_pending_tfcs_control_table,
            TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

        for (tfc_idx = 0;tfc_idx < tdsmac_ul_tfcs_table.ntfc; tfc_idx++)
        {
          // Update the TFCS table with this control flag.
          tdsmac_ul_tfcs_table.tfc_info[tfc_idx].control_flag =
            tdsmac_ul_current_tfcs_control_table[tfc_idx];

          // If this TFCI has not moved to blocked state then initialize
          if ((tdsint_ul_tfcs_info[tfc_idx].ratem_state != TDSTFC_RATEM_FAILED)
           && (tdsint_ul_tfcs_info[tfc_idx].power_state != TDSBLOCKED_STATE))
          {
            tdsint_ul_tfcs_info[tfc_idx].control_flag =
              tdsmac_ul_current_tfcs_control_table[tfc_idx];

            if (tdsint_ul_tfcs_info[tfc_idx].control_flag == TDSTFC_ALLOWED)
            {
              TDSMAC_MSG2(MSG_LEGACY_HIGH,"TFCI %d allowed (int_ul),%d",tfc_idx,tdsmac_ul_tfcs_table.tfc_info[tfc_idx].control_flag);
            }
          }

        }

        // Indicate that there is no pending config.
        tdsmac_ul_tfcc_config_pending = FALSE;

        // Clear any timers that may be running
        (void)rex_clr_timer(&tdsul_tfc_ctrl_tmr_blk);

       if((tdsrab_status == TDSCS_RABS) ||
          (tdsrab_status == TDSCS_PLUS_PS_RABS))
       {
         tdsmac_ul_compute_amr_rate(&amr_mode_info);
         tdsmac_ul_set_amr_rate(amr_mode_info.amr_rate);
         tdsmac_compute_minimum_set_of_tfcs(&amr_mode_info);
       }

#ifdef FEATURE_TDSCDMA_TFC_ROUNDROBIN
       tdsmac_tfc_roundrobin_prev_tfci = TDSMAC_INVALID_PREV_TFCI;
#endif

       tdsmac_ul_tfcc_config_in_process = FALSE;
      }
      else
      {
        // Indicate that there is pending config.
        tdsmac_ul_tfcc_config_pending = TRUE;
        tdsmac_ul_tfcc_activation_cfn = tfcc_config_ptr->activation_time;
        tdsmac_ul_tfcs_pending_control_duration = tfcc_config_ptr->control_duration;
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"TFCC msg act_cfn %d",tdsmac_ul_tfcc_activation_cfn);
      }
    }
    else
    {
      //TDSMAC_MSG0(MSG_LEGACY_HIGH,"Control Duration is not supported now");

      if (tfcc_config_ptr->activation_time == 0)
      {
        tdsmac_ul_tfcc_config_in_process = TRUE;
        
        /* Clear uplink TFCS information table internally used by MAC */
        /*memcpy (tdsmac_ul_current_tfcs_control_table,
            tdsmac_ul_pending_tfcs_control_table,
            TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));*/
        memscpy (tdsmac_ul_current_tfcs_control_table,
                 TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type),
            tdsmac_ul_pending_tfcs_control_table,
            TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

        for (tfc_idx = 0;tfc_idx < tdsmac_ul_tfcs_table.ntfc; tfc_idx++)
        {
          tdsmac_ul_tfcs_table.tfc_info[tfc_idx].control_flag =
            tdsmac_ul_current_tfcs_control_table[tfc_idx];

          // If this TFCI has not moved to blocked state then initialize
          if ((tdsint_ul_tfcs_info[tfc_idx].ratem_state != TDSTFC_RATEM_FAILED)
           && (tdsint_ul_tfcs_info[tfc_idx].power_state != TDSBLOCKED_STATE))
          {
            tdsint_ul_tfcs_info[tfc_idx].control_flag =
              tdsmac_ul_current_tfcs_control_table[tfc_idx];
          }
        }
#ifdef FEATURE_TDSCDMA_TFC_ROUNDROBIN
        tdsmac_tfc_roundrobin_prev_tfci = TDSMAC_INVALID_PREV_TFCI;
#endif
        tdsmac_ul_tfcc_config_pending = FALSE;

        // Clear any old timers if it is running
        (void)rex_clr_timer(&tdsul_tfc_ctrl_tmr_blk);

        // Set this new timer.
        (void)rex_set_timer(&tdsul_tfc_ctrl_tmr_blk,(uint32)(tfcc_config_ptr->control_duration));
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"Started timer for duration %d for this TFCC config",tfcc_config_ptr->control_duration);

        if((tdsrab_status == TDSCS_RABS) ||
          (tdsrab_status == TDSCS_PLUS_PS_RABS))
        {
          tdsmac_ul_compute_amr_rate(&amr_mode_info);
          tdsmac_ul_set_amr_rate(amr_mode_info.amr_rate);
          tdsmac_compute_minimum_set_of_tfcs(&amr_mode_info);
        }

        tdsmac_ul_tfcc_config_in_process = FALSE;
      }
      else
      {
        tdsmac_ul_tfcc_config_pending = TRUE;
        tdsmac_ul_tfcc_activation_cfn = tfcc_config_ptr->activation_time;
        tdsmac_ul_tfcs_pending_control_duration = tfcc_config_ptr->control_duration;
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"TFCC msg act_cfn %d,control_duration %d",tdsmac_ul_tfcc_activation_cfn,tdsmac_ul_tfcs_pending_control_duration);
      }
    }

    tdsoc_ul_tfcs_ptr = &tdsmac_ul_tfcs_table;
  }
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
    TDSL2_ACQ_UL_LW_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    TDSL2_ACQ_UL_LW_UNLOCK(&tdse_ul_isr_l2_datapath_mutex);
#endif
    // Post a command to RRC
    /* Failed to allocate memory for RRC command */
    if ((rrc_cmd_ptr = tdsrrc_get_mac_cmd_buf()) == NULL) /*lint !e539*/
    {
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"Could not get RRC cmd buffer");
    }
    /* Send TDSRRC_CMAC_UL_TFC_SUBSET_CFG_CNF message to RRC */
    else /*lint !e539*/
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_CMAC_UL_TFC_SUBSET_CFG_CNF to RRC");

      /*Note : This feature definition would be removed only when RRC also removes
      this FEATURE definition */
      /* Fill in command header */
      rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CMAC_UL_TFC_SUBSET_CFG_CNF;

      /* Fill in command data */
      rrc_cmd_ptr->cmd.mac_tfc_subset_cfg_cnf_info.transaction_id =
        tfcc_config_ptr->transaction_id;
      rrc_cmd_ptr->cmd.mac_tfc_subset_cfg_cnf_info.rb_id =
        tfcc_config_ptr->rb_id;
      rrc_cmd_ptr->cmd.mac_tfc_subset_cfg_cnf_info.status = status;
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"TFCC:Sending TDSRRC_CMAC_UL_TFC_SUBSET_CFG_CNF to RRC status %d",status);
      /* Send command to RRC */
      tdsrrc_put_mac_cmd(rrc_cmd_ptr);
    }
}

/*=================================================================================================
 FUNCTION NAME  tdsmac_ul_handle_tfcs_control_timer_expiry

 PURPOSE    This function process expiry of the control duration
================================================================================================*/

void tdsmac_ul_handle_tfcs_control_timer_expiry(void)
{
  uint16 tfc_idx;
  tdsmac_amr_mode_info_type      amr_mode_info;

  for (tfc_idx = 0;tfc_idx < tdsmac_ul_tfcs_table.ntfc; tfc_idx++)
  {
    // Start using the default config from this point.
    tdsmac_ul_tfcs_table.tfc_info[tfc_idx].control_flag =
      tdsmac_ul_default_tfcs_control_table[tfc_idx];

    // If this TFCI has not moved to blocked state then initialize
    if ((tdsint_ul_tfcs_info[tfc_idx].ratem_state != TDSTFC_RATEM_FAILED)
     && (tdsint_ul_tfcs_info[tfc_idx].power_state != TDSBLOCKED_STATE))
    {
      tdsint_ul_tfcs_info[tfc_idx].control_flag =
      tdsmac_ul_default_tfcs_control_table[tfc_idx];
    }

    // Memset the current config.
    memset(tdsmac_ul_current_tfcs_control_table, (uint8)TDSTFC_NOT_ALLOWED,
        TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));
  }

  if((tdsrab_status == TDSCS_RABS) ||
    (tdsrab_status == TDSCS_PLUS_PS_RABS))
  {
    tdsmac_ul_compute_amr_rate(&amr_mode_info);
    tdsmac_ul_set_amr_rate(amr_mode_info.amr_rate);
    tdsmac_compute_minimum_set_of_tfcs(&amr_mode_info);
  }

#ifdef FEATURE_TDSCDMA_TFC_ROUNDROBIN
  tdsmac_tfc_roundrobin_prev_tfci = TDSMAC_INVALID_PREV_TFCI;
#endif

  TDSMAC_MSG0(MSG_LEGACY_HIGH,"Timer expired for the TFCC msg");

}


/*=================================================================================================
 FUNCTION NAME  tdsproc_ul_mac_commands

 PURPOSE    This function process UL MAC Commands
================================================================================================*/
void  tdsproc_ul_mac_commands
(
  /* Command ID */
  tdsl2_ul_cmd_enum_type   cmd_id,

  /* Pointer to command data */
  tdsl2_ul_cmd_data_type   *cmd_data_ptr
)
{
  /* Process the command data based on command ID */
  switch (cmd_id)
  {

    /* UL MAC Congiguration Request */
    case TDSCMAC_UL_CONFIG_REQ:

      TDSMAC_MSG0(MSG_LEGACY_MED,"Rcvd TDSCMAC_UL_CONFIG_REQ");

      tdsl2_get_nv_rel_ver_opt_mask();

      /*-----------------------------------------------------------------------------
      MAC sets ul_configuration to TRUE when RRC asks MAC to wait for the signal
      from L1.
      ------------------------------------------------------------------------------*/
      if (cmd_data_ptr->mac_config_info.wait_for_l1_sig)
      {
        /* Clear the memory */
        memset(&tdsul_cfginfo,0,sizeof(tdsmac_ul_config_type));

        /* Save the pointer */
        //memcpy (&tdsul_cfginfo,&cmd_data_ptr->mac_config_info, sizeof(tdsmac_ul_config_type));;
        memscpy (&tdsul_cfginfo,sizeof(tdsmac_ul_config_type),&cmd_data_ptr->mac_config_info, sizeof(tdsmac_ul_config_type));

        /*Set the configuration pending flag to TRUE to indicate that MAC is */
        /* waiting for the signal from L1 to configure UL MAC */
        tdsul_configuration_pending=TRUE;

        TDSMAC_MSG0(MSG_LEGACY_MED,"Waiting for L1 signal to reconfigure");
      }
      /*-----------------------------------------------------------------------------
      If the configuration received from RRC has nothing related to activation time
      then reconfigure MAC now.
      ------------------------------------------------------------------------------*/
      else
      {
        tdsconfig_ul_mac (&cmd_data_ptr->mac_config_info);
      }
    break;
    
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
    /* RRC updates Persistence value  */
    case TDSMAC_UL_PERSISTENCE_VAL_CHANGE_IND:
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Rcvd TDSMAC_UL_PERSISTENCE_VAL_CHANGE_IND");
    tdsmac_ul_proc_rach_tx_update_parameters(&cmd_data_ptr->persistence_info);
    break;
#endif


    /* UL MAC traffic volume vonfiguration request */
  case TDSCMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ:
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Rcvd TDSCMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ");
    tdsconfig_ul_mac_traffic_volume(cmd_data_ptr->ul_traffic_volume_cfg);
    break;

    /* UL MAC block DTCH transmissions on RACH */
  case TDSCMAC_UL_BLOCK_RACH_TRAFFIC_REQ:
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Rcvd TDSCMAC_UL_BLOCK_RACH_TRAFFIC_REQ");
    tdsmac_ul_block_rach_tx(cmd_data_ptr->block_rach);
    break;

    /* UL MAC block Traffic Volume Measurement */
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
    case TDSCMAC_UL_TRAFFIC_VOLUME_BLOCK_REQ:
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Rcvd TDSCMAC_UL_TRAFFIC_VOLUME_BLOCK_REQ");
    tdsmac_ul_block_tvm_report(cmd_data_ptr->block_time_interval);
    break;
#endif


      /* RRC sends the TFCC config information */
    case TDSCMAC_UL_TFC_SUBSET_CFG_REQ:
      tdsproc_ul_mac_tfcc_command(&cmd_data_ptr->mac_tfc_subset_config_info);
      break;

    case TDSMAC_RACH_STATUS_IND:
      tdsproc_ul_tx_status_ind(cmd_data_ptr->rach_tx_status_ind);
      break;
	  
#ifdef FEATURE_QSH_EVENT_METRIC
	case TDSMAC_QSH_CMD:
		tdsmac_qsh_process_cmd(&cmd_data_ptr->tdsmac_qsh_cmd);	
	  break;	
#endif	  
	  
    /* Invalid command */
  default:
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"Rcvd invalid command %d",cmd_id);
    break;
  }
}


/*==============================================================================================
 FUNCTION NAME  tdsbuild_ul_frame

 FUNCTION   This function processes build UL frame event
===============================================================================================*/
void  tdsproc_build_ul_frame_ind
(
  void
)
{
  uint32 current_time;

  /* Current CCTrCh Type */
  switch (TDSGET_UL_CCTRCH_TYPE)
  {

    /* CCTrCh Type is RACH */
    case TDSRACH:

      /* Build RACH Transport Blocks */
      tdsbuild_rach_tblks (&tdsbld_frame_ind_info);
      TDS_INTLOCK();

      /* Done building RACH data */
      if (tdsmac_build_pending == TRUE)
      {
        current_time = tdsstmr_get_current_time();
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"Current time %d",current_time);
      }

      tdsbld_frame_ind_info.tx_buf_ptr->mac_busy = FALSE;
      TDS_INTFREE();
      break;

      /* CCTrCh type is DCH */
    case TDSDCH:

      tdstfc_rlc_size_for_dch_defined = TRUE;

      /* Build UL Transport Blocks opeartion was successful */
      if (tdsbuild_ul_tblks (&tdsbld_frame_ind_info))
      {
        TDSMAC_MSG0(MSG_LEGACY_LOW,"TDSDCH Frame sent to L1");

      }

      TDS_INTLOCK();

      /* Done building DCH data */
      if (tdsmac_build_pending == TRUE)
      {
        current_time = tdsstmr_get_current_time();
        TDSMAC_MSG1(MSG_LEGACY_HIGH,"Current time %d",current_time);
      }

      tdsbld_frame_ind_info.tx_buf_ptr->mac_busy = FALSE;
      TDS_INTFREE();

      break;

      /* Invalid CCTrCh type */
    default:
      TDSMAC_MSG0(MSG_LEGACY_LOW,"tdsbuild_ul_frame: invalid cctrch type");
      break;
  }
}

/*==============================================================================================
 FUNCTION NAME  void tdsmac_handle_activation_cfn_expiry

 DESCRIPTION : This fn. is called to figure out if act-cfn has reached during
               Suspension-Resumption handling as well as regular handling of
               build_frame_ind
===============================================================================================*/
#ifdef __GNUC__
static inline void tdsmac_handle_activation_cfn_expiry(uint8 cfn, uint8 i)
#else /* __GNUC__*/
__inline void tdsmac_handle_activation_cfn_expiry(uint8 cfn, uint8 i)
#endif /* __GNUC__ */
{
  /*
   * Flag to see if have to check for saved ciphering configuration info
   */
  boolean check_for_saved_cipher_info = FALSE;

  TDSMAC_MSG3(MSG_LEGACY_HIGH,"Expiry cnf 0x%x, i %d cfg is %d", cfn, i, tdscur_ul_cipher_info[i].cipher_cfg_type);

  tdsul_cipher_activation_pending[i] = FALSE;

  if ((tdsnew_ul_cipher_info[i].cipher_cfg_type == TDSNON_HO_CIPHER_CFG) &&
      (TRUE == tdsget_start_value_to_transmit[i]))
  {
    tdsnew_ul_cipher_info[i].hfn = tdsrrc_get_start_value_to_transmit();

    tdsget_start_value_to_transmit[i]=FALSE;

    /*
     * Indicate that we should check if ciphering configuration was saved.
     */
    check_for_saved_cipher_info = TRUE;

    TDSMAC_MSG3(MSG_LEGACY_HIGH,"New HFN from RRC 0x%x, cfn 0x%x, tdsprev_cfn 0x%x",
             tdsnew_ul_cipher_info[i].hfn, cfn,tdsprev_cfn);
  }
  else if (tdsnew_ul_cipher_info[i].cipher_cfg_type == TDSSMC_INITIATED_NON_HO_CIPHER_CFG)
  {
    tdsnew_ul_cipher_info[i].cipher_cfg_type = TDSNON_HO_CIPHER_CFG;
    TDSMAC_MSG3(MSG_LEGACY_HIGH,"New HFN from RRC 0x%x, cfn 0x%x, tdsprev_cfn 0x%x",
             tdsnew_ul_cipher_info[i].hfn, cfn,tdsprev_cfn);
  }
  else if (tdsnew_ul_cipher_info[i].cipher_cfg_type == TDSHO_STEP2_CIPHER_CFG)
  {
    tdsnew_ul_cipher_info[i].hfn++;
    tdsnew_ul_cipher_info[i].cipher_cfg_type = TDSNON_HO_CIPHER_CFG;

    /*
     * Indicate that we should check if ciphering configuration was saved.
     */
    check_for_saved_cipher_info = TRUE;

    TDSMAC_MSG3(MSG_LEGACY_HIGH,"Step-2 key %d hfn 0x%x cfn 0x%x", tdsnew_ul_cipher_info[i].key_idx,
             tdsnew_ul_cipher_info[i].hfn, cfn);
  }
  else if(tdsnew_ul_cipher_info[i].cipher_cfg_type == TDSNON_HO_STEP2_CIPHER_CFG)
  {
    
    /* MAC needs to update activation time from the received cmd and
     * MAC needs to get the HFN that needs to be applied at activation time from RRC. 
     */
    tdsnew_ul_cipher_info[i].hfn = tdsrrc_get_start_value_to_transmit();
    
    tdsget_start_value_to_transmit[i]=FALSE;
    
    tdsnew_ul_cipher_info[i].cipher_cfg_type = TDSNON_HO_CIPHER_CFG;

    /*
     * Indicate that we should check if ciphering configuration was saved.
     */
    check_for_saved_cipher_info = TRUE;

    TDSMAC_MSG3(MSG_LEGACY_HIGH,"NON HO Step-2 key %d hfn 0x%x cfn 0x%x", tdsnew_ul_cipher_info[i].key_idx,
             tdsnew_ul_cipher_info[i].hfn, cfn);
  }

  // Copy the new configuration parameters
  tdscur_ul_cipher_info[i] = tdsnew_ul_cipher_info[i];

  /*
   * Check to see if we had received a new ciphering configuration before reaching
   * this activation time.
   */
  if (check_for_saved_cipher_info && tdssaved_new_ul_cipher_info[i].cipher_info_saved)
  {
    /*
     * copy over the saved ciphering configuration into new ciphering configuration.
     */
    /*memcpy(&tdsnew_ul_cipher_info[i], &tdssaved_new_ul_cipher_info[i].cipher_info,
           sizeof(tdsmac_cipher_config_type));*/
    memscpy(&tdsnew_ul_cipher_info[i], sizeof(tdsmac_cipher_config_type),&tdssaved_new_ul_cipher_info[i].cipher_info,
           sizeof(tdsmac_cipher_config_type));

    /*
     * Indicate that the ciphering configuration is pending and that HFN has to be
     * incremented by one upon CFN rollover.
     */
    tdsul_cipher_activation_pending[i] = tdsmac_increment_hfn_on_rollover[i] = TRUE;

    /*
     * Reset tdsprev_cfn flag
     */
    tdsprev_cfn = TDSINVALID_PREV_CFN;

    /*
     * Reset the saved ciphering config data structure
     */
    memset(&tdssaved_new_ul_cipher_info[i].cipher_info, 0x0, sizeof(tdsmac_cipher_config_type));

    tdssaved_new_ul_cipher_info[i].cipher_info_saved = FALSE;
  }
  else
  {
    // Clear the memory
    memset(tdsnew_ul_cipher_info,0x00, sizeof(tdsmac_cipher_config_type));
  }

  if(cfn < tdscur_ul_cipher_info[i].ciph_act_time)
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Missed act_time,roll-over occured");
    tdscur_ul_cipher_info[i].hfn++;
  }
}

/*==============================================================================================
 FUNCTION NAME  void tdsmac_update_ciphering_params

 DESCRIPTION : 1. MAC maintains 2 configurations for ciphering. Current Ciphering Parameters (tdscur_ul_cipher_info)
 and New Ciphering Parameters (tdsnew_ul_cipher_info).

 2. If there is an activation time pending, then tdsnew_ul_cipher_info is used. The parameters
 are copied into tdscur_ul_cipher_info at activation time.

 3. tdscfn_hfn_table is a map of the CFN and the corresponding HFN. CFN serves as the
 index of this array and the value at an index is the HFN. DL MAC reads from this
 table to get the HFN associated with a CFN. UL MAC maintains this table.

 4. When MAC starts ciphering for the first time and an activation time is pending,
   the sequence to increment the HFN is Reach Activation Time, ROLL OVER.
   To illustrate lets consider 2 cases

   Case 1: ciphering_activation_time = 12 and CFN = 30. Now the CFN has to roll over and then reach the
   activation time of 12. HFN would be incremented only after CFN rolls over after the activation time.

   Case 2: ciphering_activation_time = 220 and CFN = 5. Ciphering Activation time is reached before the
   CFN roll over and hence not treated as a special case.

   The flag first_time_rolled_over serves to address the case 1 above for us not to increment HFN
   if there is a CFN roll over and ciphering_activation_time is not reached.

 5. Subsequently every time there is a roll over MAC would increment the HFN.
 roll over is detected by the following piece of code :
 if ( cfn < tdsprev_cfn). MAC maintains the last CFN value. This code is generic and would
 be able to handle even couple of build frame indications get missed for some reason.

 6. In TDSHO_STEP1_CIPHER_CFG : the ciphering_activation_pending flag would be set to FALSE.
 The HFN would not be incremented even if we reach activation time and there are roll overs.
 This is a temporary state when WCDMA has received a request for Handover from GSM and WCDMA
 side has not yet sent the confirm. The same is applicable for hard handovers.

 7. In TDSHO_STEP2_CIPHER_CFG : This configuration is received when WCDMA has sent a confirm.
 On receiving this configuration, MAC would increment the HFN by 1 when it hits activation
 time without waiting for the roll over. After this, the behaviour is as in the case of
 TDSNON_HO_CIPHER_CFG.

===============================================================================================*/

void tdsmac_update_ciphering_params
(
  uint8 cfn
)
{
  uint16 i;

  TDSL2_ACQ_CIPHER_PARAM_LOCK();
  #ifdef FEATURE_TDSCDMA_HSUPA
  tdsmac_hs_ul_cur_cfn = cfn+1;
  #endif

  cfn += 2;

  for (i=0;i<TDSMAX_CN_DOMAIN;i++)
  {
    /* If there is a new config specified to which MAC has to switch to at
    ciphering activation time then ... */
    if (tdsul_cipher_activation_pending[i])
    {
      if(TDSINVALID_PREV_CFN == tdsprev_cfn )
      {
        tdsprev_cfn = cfn;
      }

      if(CHECK_FOR_ACT_CFN_EXPIRY(tdsprev_cfn, tdsnew_ul_cipher_info[i].ciph_act_time, cfn))
      {
        tdsmac_handle_activation_cfn_expiry(cfn, (uint8)i);
      }
      else
      {
        /* If there is another security mode command which is causing the UE to switch to
        new set of parameters at activation time, then at the time of rollover, increment
        the hfn without actuvally having to wait for the activation time to do so. */
        if (tdsmac_increment_hfn_on_rollover[i])
        {
          /* If the rollover happens */
          if (cfn < tdsprev_cfn)
          {
            /* Then increment the current hfn */
            tdscur_ul_cipher_info[i].hfn++;
            /* Set this flag immediately to false */
            tdsmac_increment_hfn_on_rollover[i] = FALSE;
            TDSMAC_MSG3(MSG_LEGACY_HIGH,"CallWait cfn 0x%x tdsprev_cfn 0x%x act_time 0x%x", cfn, tdsprev_cfn,
                     tdsnew_ul_cipher_info[i].ciph_act_time);
          }
        }
      }

      // Compute the count c value.
      tdscount_c[i] = ((tdscur_ul_cipher_info[i].hfn << 8) | cfn);

      /* ciphering_grabled boolean can be set to TRUE at run-time
      inorder to debug garbled voice etc */
      if ((!(cfn & tdsmac_ul_debug_print_interval) && tdsciphering_garbled ))
      {
        TDSMAC_MSG3(MSG_LEGACY_HIGH,"Prior ciph_act 0x%x tdscount_c 0x%x cfn 0x%x",
                 tdsnew_ul_cipher_info[i].ciph_act_time, tdscount_c[i], cfn);

        TDSMAC_MSG1(MSG_LEGACY_HIGH,"Prior ciph_key 0x%x",tdscur_ul_cipher_info[i].key_idx);
      }

      /* Save the current value of the cfn */
      tdsprev_cfn = cfn;

    } /* activation time is pending */

    /* Activation time is not pending and if the ciphering is enabled */
    else if (tdscur_ul_cipher_info[i].cipher_enable)
    {
      if(TDSINVALID_PREV_CFN == tdsprev_cfn )
      {
        tdsprev_cfn = cfn;
      }

      if (tdscur_ul_cipher_info[i].cipher_cfg_type == TDSNON_HO_CIPHER_CFG)
      {
        // Compute the new hfn when there is a roll over
        // activation time has reached
        if (cfn < tdsprev_cfn)
        {
          tdscur_ul_cipher_info[i].hfn++;
        }
      }

      /* In the case of TDSHO_STEP1_CIPHER_CFG we do not need to
      increment the hfn, keep using the cfn even if activation time is
      reached. The following operation would take care of that so
      no special processing needs to be done for TDSHO_STEP1_CIPHER_CFG*/

      tdscount_c[i] = ((tdscur_ul_cipher_info[i].hfn << 8) | cfn);

      /* ciphering_grabled is a boolean. This boolean can be set to TRUE at run-time
      inorder to debug garbled voice etc
      if (tdsciphering_garbled && (!(cfn & tdsmac_ul_debug_print_interval)))*/
      {
        TDSMAC_MSG3(MSG_LEGACY_HIGH,"After ciph_act tdscount_c 0x%x cfn 0x%x, key 0x%x", tdscount_c[i], cfn,
        tdscur_ul_cipher_info[i].key_idx);
      }

      /* Save the current value of the cfn */
      tdsprev_cfn = cfn;
    }

    if (i == 0)
    {
      if (tdscur_ul_cipher_info[i].cipher_cfg_type != TDSSMC_INITIATED_NON_HO_CIPHER_CFG)
      {
        // Store the hfn corresponding to the CFN. This owuld be used by DL MAC
        tdscfn_hfn_table[i][cfn] = tdscur_ul_cipher_info[i].hfn;

        /* This table will be poipulated by Uplink and will be used as a lookup table in the downlink.
        DL will refer to this information to populate thier ciphering keys, tdscount_c etc */
        tdscfn_ciphering_table[cfn].key_idx = tdscur_ul_cipher_info[i].key_idx;
        tdscfn_ciphering_table[cfn].cipher_algo = tdscur_ul_cipher_info[i].cipher_algo;
        tdscfn_ciphering_table[cfn].cipher_enable = tdscur_ul_cipher_info[i].cipher_enable;
      }
      else
      {
        tdscfn_ciphering_table[cfn].cipher_enable = FALSE;
      }
    }
  }
  TDSL2_ACQ_CIPHER_PARAM_UNLOCK();
}

/*==============================================================================================
 FUNCTION NAME  void tdsl1_suspend_ind

 DESCRIPTION : This function is called 2 times by layer 1. Once at the time when Layer 1 is
 suspended and there is a W->G handover. This function is called again when Layer 1 is
 resumed (G->W this may happen if the reconfiguration fails or...).

 cfn : indicates the current value of the CFN
 num_roll_overs : The number of roll-overs that occureed during the time L1 was suspended
 to the time the Layer 1 resumed .

 If the reconfiguration took
   a) < 2.5 s ==> the variable num_roll_overs = 0
   b) 2.5s <= reconfig_time < 5.12s ===> num_roll_overs = 1
      ....
MAC based on the number of roll overs that ocurred would increment
its hfn count.

===============================================================================================*/
void tdsl1_suspend_ind
(
  uint8                     cfn,
  tdsl1_hho_action_enum_type      hho_action
)
{
  uint8   i=0;
  uint16  j = 0;

  // Adjust the cfn to align with the build frame ind cfn
  cfn += 2;

  TDSL2_ACQ_CIPHER_PARAM_LOCK();
  if (!tdsul_cipher_activation_pending[0] && (cfn < tdsprev_cfn))
  {
    tdscur_ul_cipher_info[0].hfn++;
  }

  tdsprev_cfn = cfn;

  if (hho_action == TDSL1_HHO_REV_TI)
  {
    /* No need to save the configuration . On going back to the old cell
    the ciphering parameters will be restored */
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Recvd TDSL1_HHO_REV_TI from L1,No-Op");
  }
  else
  {
    /* for hho_action == HHO_FW_TI, HHO_FW_TM, HHO_REV_TM  the config needs
    to be saved */
    //memcpy (tdsprev_cfn_ciphering_table, tdscfn_ciphering_table, 256 * sizeof(tdsmac_ciphering_table));
    memscpy (tdsprev_cfn_ciphering_table, 256 * sizeof(tdsmac_ciphering_table),tdscfn_ciphering_table, 256 * sizeof(tdsmac_ciphering_table));

    for (i = 0; i < TDSMAX_CN_DOMAIN; i++)
    {
      /* Save the configuration currently being used in prev. In case of reverting
      back to old configuration this will be used */
      tdsprev_mac_increment_hfn_on_rollover[i] = tdsmac_increment_hfn_on_rollover[i];
      tdsprev_new_ul_cipher_info[i] = tdsnew_ul_cipher_info[i];
      tdsprev_cur_ul_cipher_info[i] = tdscur_ul_cipher_info[i];
      tdsprev_ul_cipher_activation_pending[i] = tdsul_cipher_activation_pending[i];
      tdsold_prev_cfn = tdsprev_cfn;
      tdsprev_count_c[i] = tdscount_c[i];

      /* Initialize the cfn hfn table */
      for (j = 0; j < 256; j++)
      {
        tdsprev_cfn_hfn_table[i][j] = tdscfn_hfn_table[i][j];
      }

      tdsprev_get_start_value_to_transmit[i] = tdsget_start_value_to_transmit[i];

      TDSMAC_MSG3(MSG_LEGACY_HIGH,"Saving the old config cfn 0x%x, tdscount_c 0x%x, hfn 0x%x",
               tdsprev_cfn,tdscount_c[0],tdsprev_cur_ul_cipher_info[i].hfn);
    }
  }

  /*
   * MAC HSUPA Suspend logic and no resume necessary yet.
   * Critical section is handed within.
   * no action if hsupa is inactive
   */
  tdsmac_ulhs_suspend();

  TDSL2_ACQ_CIPHER_PARAM_UNLOCK();
  TDSMAC_MSG2(MSG_LEGACY_HIGH,"Suspend cfn 0x%x,tdsprev_cfn 0x%x",cfn,tdsprev_cfn);
}

/*==============================================================================================
 FUNCTION NAME  void tdsl1_resume_ind

 DESCRIPTION : This function is called 2 times by layer 1. Once at the time when Layer 1 is
 suspended and there is a W->G handover. This function is called again when Layer 1 is
 resumed (G->W this may happen if the reconfiguration fails or...).

 cfn : indicates the current value of the CFN
 num_roll_overs : The number of roll-overs that occureed during the time L1 was suspended
 to the time the Layer 1 resumed .

 If the reconfiguration took
   a) < 2.5 s ==> the variable num_roll_overs = 0
   b) 2.5s <= reconfig_time < 5.12s ===> num_roll_overs = 1
      ....
MAC based on the number of roll overs that ocurred would increment
its hfn count.

===============================================================================================*/
void tdsl1_resume_ind
(
  uint8                     cfn,
  uint8                     num_roll_overs,
  tdsl1_hho_action_enum_type   hho_action
)
{
  uint8   i=0;
  uint16  j = 0;
  uint16  num_cfns_to_update=0;
  uint8   t1,t2,t3;
  uint32  hfn;
  boolean ciph_act_cfn_reached_on_resume = FALSE;

  // Adjust the cfn to align with the build frame indication cfn.
  cfn += 2;

  if (hho_action == TDSL1_HHO_FW_TI)
  {
    /* No need to restore the ciphering configuration. In all other cases
    restore the ciphering configuration */
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Recvd TDSL1_HHO_FW_TI from L1,No-Op");
  }
  else /* hho_action == HHO_REV_TM, HHO_FW_TM, HHO_FW_TI */
  {
    TDSL2_ACQ_CIPHER_PARAM_LOCK();
    /* Restore  the saved configuration we have reverted to old configuration
    and this needs to be used again */
    //memcpy (tdscfn_ciphering_table, tdsprev_cfn_ciphering_table, 256 * sizeof(tdsmac_ciphering_table));

    memscpy (tdscfn_ciphering_table, 256 * sizeof(tdsmac_ciphering_table),tdsprev_cfn_ciphering_table, 256 * sizeof(tdsmac_ciphering_table));

    for (i = 0; i < TDSMAX_CN_DOMAIN; i++)
    {
      /* Save the configuration currently being used in prev. In case of reverting
      back to old configuration this will be used */
      tdsmac_increment_hfn_on_rollover[i] = tdsprev_mac_increment_hfn_on_rollover[i];

      tdscur_ul_cipher_info[i] = tdsprev_cur_ul_cipher_info[i];
      tdsnew_ul_cipher_info[i] = tdsprev_new_ul_cipher_info[i];

      tdsul_cipher_activation_pending[i] = tdsprev_ul_cipher_activation_pending[i];
      tdsprev_cfn = tdsold_prev_cfn;
      tdscount_c[i] = tdsprev_count_c[i];

      /* Initialize the cfn hfn table */
      for (j = 0; j < 256; j++)
      {
        tdscfn_hfn_table[i][j] = tdsprev_cfn_hfn_table[i][j];
      }

      tdsget_start_value_to_transmit[i] = tdsprev_get_start_value_to_transmit[i];
    }
    TDSL2_ACQ_CIPHER_PARAM_UNLOCK();

    TDSMAC_MSG3(MSG_LEGACY_HIGH,"Restoring the old config tdsprev_cfn 0x%x, tdscount_c 0x%x"
             ,tdsprev_cfn,tdscount_c[0],0);
  }

  TDSMAC_MSG3(MSG_LEGACY_HIGH,"Resume cfn 0x%x, roll_over %d, tdsprev_cfn 0x%x", cfn, num_roll_overs,tdsprev_cfn);

  /* If the time between suspend and resume takes more than 2.5 seconds
  and if there was a pending activation time then we would have reached
  the activation time sometime on between suspend and resume. Hence we
  need to get the new value for the hfn using tdsget_start_value_to_transmit
  and update the hfn accordingly */
  for (i=0;i<TDSMAX_CN_DOMAIN;i++)
  {
    /* Do the following only if ciphering is enabled and wcdma is not in
    suspended state */
    if (tdscur_ul_cipher_info[i].cipher_enable)
    {
      TDSL2_ACQ_CIPHER_PARAM_LOCK();
      /* If there was a pending activation time during the suspend - resume time frame */
      if (tdsul_cipher_activation_pending[i])
      {
        if (num_roll_overs >= 1)
        {
          // This indicates that activation time was reached during the time that
          // l1 was suspended.
          // Compute the new hfn when there is a roll over
          // activation time has reached
          ciph_act_cfn_reached_on_resume = TRUE;

          TDSMAC_MSG3(MSG_LEGACY_HIGH,"ActCFN-reached sus 0x%x res 0x%x roll_ovrs 0x%x", tdsprev_cfn, cfn, num_roll_overs);
        }
        else
        {
          if((t1=(cfn-1)) != tdsprev_cfn)
          {
            if ((t2=(cfn-2)) != tdsprev_cfn)
            {
              if ((t3=(cfn-3)) != tdsprev_cfn)
              {
                num_cfns_to_update = 3;
                cfn = t3;
              }
              else
              {
                num_cfns_to_update = 2;
                cfn = t2;
              }
            }
            else
            {
              num_cfns_to_update = 1;
              cfn = t1;
            }
          }

          /* The G2W took less than 2.5 seconds */
          if (CHECK_FOR_ACT_CFN_EXPIRY(tdsprev_cfn, tdsnew_ul_cipher_info[i].ciph_act_time, cfn))
          {
            ciph_act_cfn_reached_on_resume = TRUE;
            TDSMAC_MSG3(MSG_LEGACY_HIGH,"ActCFN-reached sus 0x%x res 0x%x act_time 0x%x",
                     tdsprev_cfn, cfn, tdsnew_ul_cipher_info[i].ciph_act_time);
          }
          else
          {
            /* If there is another security mode command which is causing the UE to switch to
            new set of parameters at activation time, then at the time of rollover, increment the hfn
            without actuvally having to wait for the activation time to do so. */
            if (tdsmac_increment_hfn_on_rollover[i])
            {
              /* If the rollover happens */
              if (cfn < tdsprev_cfn)
              {
                /* Then increment the current hfn */
                tdscur_ul_cipher_info[i].hfn++;
                /* Set this flag immediately to false */
                tdsmac_increment_hfn_on_rollover[i] = FALSE;
                TDSMAC_MSG3(MSG_LEGACY_HIGH,"CallWait cfn 0x%x tdsprev_cfn 0x%x act_time 0x%x",
                         cfn, tdsprev_cfn, tdsnew_ul_cipher_info[i].ciph_act_time);
              }
            }
            TDSMAC_MSG3(MSG_LEGACY_HIGH,"ActCFN not reached sus 0x%x, res 0x%x, act_time 0x%x",
                     tdsprev_cfn, cfn, tdsnew_ul_cipher_info[i].ciph_act_time);
          }
        } // Failed G2W took less than 2.5 seconds

        if (ciph_act_cfn_reached_on_resume)
        {
          tdsmac_handle_activation_cfn_expiry(cfn, i);
        } /* If ciphering activation time was reached during the time L1 was suspended */
      } // If there is no pending activation time
      else
      {
        if ((t1=(cfn-1)) != tdsprev_cfn)
        {
          if ((t2=(cfn-2)) != tdsprev_cfn)
          {
            if ((t3=(cfn-3)) != tdsprev_cfn)
            {
              num_cfns_to_update = 3;
              cfn = t3;
            }
            else
            {
              num_cfns_to_update = 2;
              cfn = t2;
            }
          }
          else
          {
            num_cfns_to_update = 1;
            cfn = t1;
          }
        }

        // Compute the new hfn when there is a roll over
        // activation time has reached
        if (cfn < tdsprev_cfn)
        {
          tdscur_ul_cipher_info[i].hfn++;
        }
        TDSMAC_MSG3(MSG_LEGACY_HIGH,"Non-Pending-Act Sus 0x%x res 0x%x nroll %d", tdsprev_cfn, cfn, num_roll_overs);
      }

      /* Add the number of roll overs that has occurred during the time
      when WCDMA was in suspended state */
      hfn = tdscur_ul_cipher_info[i].hfn + (uint32)num_roll_overs;

      tdscfn_hfn_table[i][cfn] = hfn;

      tdscur_ul_cipher_info[i].hfn += (uint32)num_roll_overs;

      tdsprev_cfn = cfn;

      TDSL2_ACQ_CIPHER_PARAM_UNLOCK();
      for (j=0;j<num_cfns_to_update;j++)
      {
        cfn = cfn+1;
        TDSMAC_MSG2(MSG_LEGACY_HIGH,"cfn 0x%x populated %d",cfn,j);
        // Note that the tdsmac_update_ciphering_params function will
        // also adjust the cfn according to the build frame indication
        // Hence before calling the follow. function the cfn needs to be adjusted
        tdsmac_update_ciphering_params(cfn-2);
      }
    }
  } // Go through each domain
}



/*==================================================================================================
 FUNCTION NAME  tdsl1_phy_ul_build_frame_ind

 DESCRIPTION  This function processes UL frame build indication received from PHY layer


 CONTEXT    Called by PHY layer in interrupt context
==================================================================================================*/
boolean   tdsl1_phy_ul_build_frame_ind
(
  /* current tti */
  tdsl1_tti_enum_type     tti,

  /* current CFN */
  uint8               cfn,

  boolean             ul_tx_disabled,

  /*  Array of TFCI states for power based TFC elimination
   *  TRUE indicates the TFCI is in excess_pwer_state
   *  FALSE indicates the TFCI is in supported_state
  */
  
  const tdsl1_ul_tfc_pwr_state_enum_type * tfc_pwr_state_ptr,

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)|| defined(FEATURE_TDSCDMA_DSDS_QTA)
  boolean *allowed_tfci_list,
#endif

  boolean need_srb_status,

  /* pointer to Tx data buffer */
  tdsl1_ul_tx_data_type  *tx_buf_ptr
)

{
  uint8 i;

  /* return valuse */
  boolean   rv = FALSE;

  uint16  ntfc;


  uint16    tfc_idx;
  tdsmac_amr_mode_info_type      amr_mode_info;


  REX_ISR_LOCK(&tdsr99_ul_isr_l2_datapath_mutex);

  if (TDSGET_UL_CCTRCH_TYPE == TDSINVALID_UL_CCTRCH)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"No Trch configured at MAC!");
    REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    return rv;
  }

  if (tdsmac_ul_tfcc_config_in_process)
  {
    TDSMAC_MSG0(MSG_LEGACY_ERROR," TFCC config in process,so skip BFI");
    REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    return rv;
  }

  if (tdsmac_ul_tfcc_config_pending)
  {
    if (tdsmac_ul_tfcc_activation_cfn == cfn)
    {
      TDSMAC_MSG2(MSG_LEGACY_HIGH,"Activation time reached cfn %d,act_cfn %d",cfn,tdsmac_ul_tfcc_activation_cfn);
      tdsmac_ul_tfcc_config_pending = FALSE;
      for (tfc_idx = 0;tfc_idx < tdsmac_ul_tfcs_table.ntfc; tfc_idx++)
      {
        tdsmac_ul_tfcs_table.tfc_info[tfc_idx].control_flag =
          tdsmac_ul_pending_tfcs_control_table[tfc_idx];

        // If this TFCI has not moved to blocked state then initialize
        if ((tdsint_ul_tfcs_info[tfc_idx].ratem_state != TDSTFC_RATEM_FAILED)
         && (tdsint_ul_tfcs_info[tfc_idx].power_state != TDSBLOCKED_STATE))
        {
          tdsint_ul_tfcs_info[tfc_idx].control_flag =
            tdsmac_ul_pending_tfcs_control_table[tfc_idx];
        }

        tdsmac_ul_tfcc_config_pending = FALSE;

        /* Clear uplink TFCS information table internally used by MAC */
        /*memcpy (tdsmac_ul_current_tfcs_control_table,
            tdsmac_ul_pending_tfcs_control_table,
            TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));*/
        memscpy (tdsmac_ul_current_tfcs_control_table,
                 TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type),
            tdsmac_ul_pending_tfcs_control_table,
            TDSUE_MAX_TFC * sizeof(tdsmac_ul_tfc_control_e_type));

        if (tdsmac_ul_tfcs_pending_control_duration > 0)
        {
          TDSMAC_MSG1(MSG_LEGACY_HIGH,"Started timer for duration %d for this TFCC config",tdsmac_ul_tfcs_pending_control_duration);
          // Clear any old timers if it is running
          (void)rex_clr_timer(&tdsul_tfc_ctrl_tmr_blk);
          (void)rex_set_timer(&tdsul_tfc_ctrl_tmr_blk, (uint32)(tdsmac_ul_tfcs_pending_control_duration));
        }
      }

     if((tdsrab_status == TDSCS_RABS) ||
          (tdsrab_status == TDSCS_PLUS_PS_RABS))
     {
       tdsmac_ul_compute_amr_rate(&amr_mode_info);
       tdsmac_ul_set_amr_rate(amr_mode_info.amr_rate);
       tdsmac_compute_minimum_set_of_tfcs(&amr_mode_info);
     }

#ifdef FEATURE_TDSCDMA_TFC_ROUNDROBIN
     tdsmac_tfc_roundrobin_prev_tfci = TDSMAC_INVALID_PREV_TFCI;
#endif
    }
  }

  /*
  If L1 had trashed the previous build frame indication data
  then this flag will be set to TRUE so that RLC can re-send
  the buffer status for those channels for which data was built
  */

  if (need_srb_status == TRUE)
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"L1 has trashed the last L2 ack 0x%x",cfn);

    /* If there was some data built from the signalling channel
    then request RLC to send the report */
    if (tdsrlc_status_report_req_info.nchan > 0)
    {
      TDSMAC_MSG1(MSG_LEGACY_HIGH,"Req RLC to re-send status report for nchan %d",tdsrlc_status_report_req_info.nchan);
      // Call RLC function
      tdsrlc_process_status_report_req(&tdsrlc_status_report_req_info);

      tdsrlc_status_report_req_info.nchan = 0;
    }
  }

  if (tdsoc_ul_tfcs_ptr!= NULL)
  {
    /* Clear off the old values*/
    memset(tdstfc_excess_pwr_state, 0x0, tdsoc_ul_tfcs_ptr->ntfc*sizeof(tdsl1_ul_tfc_pwr_state_enum_type));
    
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT)         \
    || defined(FEATURE_TDSCDMA_DMO_SUPPORT)                    \
    || defined(FEATURE_TDSCDMA_DSDS_QTA)
    memset(tdstfc_dmo_allow_state, TRUE, tdsoc_ul_tfcs_ptr->ntfc*sizeof(boolean));
#endif	
    /* Only for DCH apply the power restriction */
    if (TDSGET_UL_CCTRCH_TYPE == TDSDCH)
    {
      for (ntfc = 0; ntfc < tdsoc_ul_tfcs_ptr->ntfc; ntfc++)
      {
        if (tfc_pwr_state_ptr != NULL)
        {
          tdstfc_excess_pwr_state[ntfc] = tfc_pwr_state_ptr[ntfc];
        }
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT)          \
    || defined(FEATURE_TDSCDMA_DMO_SUPPORT)                     \
    || defined(FEATURE_TDSCDMA_DSDS_QTA)
        if (allowed_tfci_list != NULL)
        {
          tdstfc_dmo_allow_state[ntfc] = allowed_tfci_list[ntfc];
        }
#endif
      }
    }
  }
  
  /* Save the Build Frame Indication */
  tdsbld_frame_ind_info.cfn = cfn;
  tdsbld_frame_ind_info.tti = tti;

  /* Save the current tti */
  tdscurrent_tti = tti;

  if (ul_tx_disabled)
  {
    tdseliminate_tfc_based_on_ue_tx_pwr();
    REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
    return rv;
  }

  /* Transmit Data Buffer pointer is NULL */
  if (tx_buf_ptr == NULL)
  {
    TDSMAC_MSG0(MSG_LEGACY_MED,"Tx Data Buffer pointer is NULL");

    /*-------------------------------------------------------------------
    During the time when L1 has switched to the new config and is
    waiting for MAC to switch to the new configuration. MAC would be
    getting NULL tx_bufptr from L1.
    --------------------------------------------------------------------*/
    if (tdsul_configuration_pending)
    {
      TDSMAC_MSG0(MSG_LEGACY_LOW,"Reconfiguring MAC");
    }

  }
  /* Invalid TTI type */
  else if (tti >= TDSL1_NUM_TTI)
  {
    TDSMAC_MSG1(MSG_LEGACY_HIGH,"invalid TTI type %d",tti);
  }
  /* Valid UL MAC Command Buffer Pointer */
  else
  {
    if (!tx_buf_ptr->mac_busy)
    {
      if (tdsmac_build_pending)
      {
        /* MAC has pending build frame */
        if (tdsbld_frame_ind_info.tti <= tti)
        {
          TDSMAC_MSG1(MSG_LEGACY_HIGH,"Now at right TTI %d",tdsbld_frame_ind_info.tti);
          tdsmac_build_pending = FALSE;
        }
        else
        {
          TDSMAC_MSG2(MSG_LEGACY_HIGH,"Waiting for right TTI %d now %d",tdsbld_frame_ind_info.tti,tti);
          REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
          return(rv);
        }
      }
      else
      {
        tx_buf_ptr->mac_busy = TRUE;
      }
    }
    else
    {
      tdsmac_build_pending = TRUE;
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"MAC busy with prev request");
      REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
      return(rv);
    }

   
        
#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
    /* Save the cfn */
    tdscur_ul_cfn = cfn;
#endif




    tdsbld_frame_ind_info.tx_buf_ptr = tx_buf_ptr;

    /* Initialize TX Buffer Data */
    tx_buf_ptr->num_trch = 0;
    for (i=0; i <TDSL1_UL_MAX_TRCH; i++)
    {
      tx_buf_ptr->tx_data[i].num_tb = 0;
      tx_buf_ptr->tx_data[i].tb_size = 0;
    }

    if (tdsoc_ul_tfcs_ptr == NULL)
    {
      TDSMAC_MSG0(MSG_LEGACY_HIGH,"TFCS ptr NULL");
      REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
      return rv;
    }
    else if ((tdsoc_ul_tfcs_ptr->ntfc > TDSUE_MAX_TFC) ||
             (tdsoc_ul_tfcs_ptr->nchan > TDSUE_MAX_TRCH))
    {
      ERR_FATAL("Configured values exceed UE_MAX %d %d",tdsoc_ul_tfcs_ptr->ntfc,tdsoc_ul_tfcs_ptr->nchan,0);
      
    }

    /* Process Build UL Frame Indication */
    tdsproc_build_ul_frame_ind();

    rv = TRUE;
  }
  REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
  return(rv);
}

/*===============================================================================================
 FUNCTION NAME  tdsl1_phy_ul_trch_status_ind

 DESCRIPTION  This function processes transmit status indication received from PHY layer

 CONTEXT    Called by PHY layer in interrupt context
===============================================================================================*/
void  tdsl1_phy_ul_trch_status_ind
(
/* PRACH transmit status */
tdsl1_prach_status_enum_type rach_tx_status_ind
)
{
  tdsl2_ul_cmd_type  *l2_ul_cmd_ptr;

  /* Fill in the MAC uplink parameters and send the primitive */
  if ((l2_ul_cmd_ptr = tdsl2_ul_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Cmd Ptr when sending MAC_RACH_STATUS_IND",0,0,0);
  }
  
  /* Fill in the Command details */
  l2_ul_cmd_ptr->cmd_hdr.cmd_id = TDSMAC_RACH_STATUS_IND;
  l2_ul_cmd_ptr->cmd_data.rach_tx_status_ind = rach_tx_status_ind;

  tdsl2_ul_put_cmd(l2_ul_cmd_ptr);
  TDSMAC_MSG1(MSG_LEGACY_HIGH,"MAC_RACH: Posting RACH Status Ind %d",l2_ul_cmd_ptr->cmd_data.rach_tx_status_ind);
}


/*-------------------------------------------------------------------
FUNCTION tdsmac_ul_allocate_mem_ZI_opt

DESCRIPTION
 This function will be called by RRC at init time so that MAC can go for 
 dynamic memory allocation for the required varaibles.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
--------------------------------------------------------------------*/
void  tdsmac_ul_allocate_mem_ZI_opt(void)
{

  uint16    num_rows = 0;

  /* Dynamical allocation for the tdsint_ul_tfcs_info */
  if (tdsint_ul_tfcs_info == NULL)
  {
    TDSMAC_MSG2(MSG_LEGACY_HIGH,"Allocating %d bytes for tdsint_ul_tfcs_info,client ID %d",sizeof(tdsul_tfcs_info_type) * TDSUE_MAX_TFC,MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    
    tdsint_ul_tfcs_info = (tdsul_tfcs_info_type *)modem_mem_alloc
                         (sizeof(tdsul_tfcs_info_type) * TDSUE_MAX_TFC, 
                           MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    
    if (NULL == tdsint_ul_tfcs_info)
    {
      ERR_FATAL("MAC_ERR: failed to allocate %d bytes for tdsint_ul_tfcs_info, client %d ",sizeof(tdsul_tfcs_info_type) * TDSUE_MAX_TFC,MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT,0);
      
    }
    memset(tdsint_ul_tfcs_info, 0, (sizeof(tdsul_tfcs_info_type) * TDSUE_MAX_TFC));
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH," tdsint_ul_tfcs_info dynamic allocation already done ");
  }


  /* Dynamical allocation for the tdsrlc_ul_data_info_ptr */
  if(tdsrlc_ul_data_info_ptr == NULL)
  {
    TDSMAC_MSG2(MSG_LEGACY_HIGH,"Allocating %d bytes for tdsrlc_ul_data_info,client ID %d",sizeof(tdsrlc_ul_frame_data_type),MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    
    tdsrlc_ul_data_info_ptr = (tdsrlc_ul_frame_data_type *)modem_mem_alloc
                         (sizeof(tdsrlc_ul_frame_data_type), 
                           MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    
    if (NULL == tdsrlc_ul_data_info_ptr)
    {
      ERR_FATAL("MAC_ERR: failed to allocate %d bytes for tdsrlc_ul_data_info, client %d",sizeof(tdsrlc_ul_frame_data_type),MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT,0);
      
    }
    memset(tdsrlc_ul_data_info_ptr, 0, sizeof(tdsrlc_ul_frame_data_type));
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH," tdsrlc_ul_data_info_ptr dynamic allocation already done ");
  }


  /* Dynamical allocation for the tdsint_ul_trch_priority_info */
  if (tdsint_ul_trch_priority_info == NULL)
  {
    TDSMAC_MSG3(MSG_LEGACY_HIGH,"Allocating %d bytes for tdsint_ul_trch_priority_info %d pointers, client ID %d",
              sizeof(tdsmaci_ul_trch_priority_info_type *) * TDSUE_MAX_TRCH, TDSUE_MAX_TRCH,
              MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    
    tdsint_ul_trch_priority_info = (tdsmaci_ul_trch_priority_info_type **)modem_mem_alloc
                         (sizeof(tdsmaci_ul_trch_priority_info_type *) * TDSUE_MAX_TRCH, 
                           MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    
    if (NULL == tdsint_ul_trch_priority_info)
    {
      ERR_FATAL("MAC_ERR: failed to allocate %d bytes for %d pointers, client %d",sizeof(tdsmaci_ul_trch_priority_info_type *) * TDSUE_MAX_TRCH,MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT,0);
      
    }
    
    /* Allocate memory for each Transport Channle */
    for (num_rows = 0; num_rows < TDSUE_MAX_TRCH; num_rows++)
    {
      tdsint_ul_trch_priority_info[num_rows] = (tdsmaci_ul_trch_priority_info_type *)modem_mem_alloc
                         (sizeof(tdsmaci_ul_trch_priority_info_type) * TDSMAC_UL_MAX_MLPS, 
                           MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    
      if (NULL == tdsint_ul_trch_priority_info[num_rows])
      {
        ERR_FATAL("MAC_ERR: failed to allocate %d bytes for tdsint_ul_trch_priority_info, client %d",
                   sizeof(tdsmaci_ul_trch_priority_info_type) * TDSMAC_UL_MAX_MLPS,
                   TDSMAC_UL_MAX_MLPS, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
       
      }
    }
  
    /* MemSet to zero for each Transport Channle */
    for (num_rows = 0; num_rows < TDSUE_MAX_TRCH; num_rows++)
    {
      memset(tdsint_ul_trch_priority_info[num_rows], 0, 
             (sizeof(tdsmaci_ul_trch_priority_info_type) * TDSMAC_UL_MAX_MLPS));
    }
  }
  else
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH," tdsint_ul_trch_priority_info dynamic allocation already done ");
  }
  


}


/*-------------------------------------------------------------------
FUNCTION tdsmac_ul_free_mem_ZI_opt

DESCRIPTION
 This function will be called by RRC release WCDMA time, so that MAC can de-allocate/free
 the dynamic memory allocated varaibles.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
--------------------------------------------------------------------*/
void  tdsmac_ul_free_mem_ZI_opt(void)
{

  uint16    num_rows = 0;

  TDSL2_MSG1(MSG_LEGACY_HIGH,"Freeing MAC UL ZI variables,client ID %d",MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);

  if (tdsint_ul_tfcs_info != NULL)
  {
    modem_mem_free(tdsint_ul_tfcs_info, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    tdsint_ul_tfcs_info = NULL;
  }

  if (tdsrlc_ul_data_info_ptr != NULL)
  {
    modem_mem_free(tdsrlc_ul_data_info_ptr, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    tdsrlc_ul_data_info_ptr = NULL;
  }

  if (tdsint_ul_trch_priority_info != NULL)
  {
    for( num_rows = 0; num_rows < TDSUE_MAX_TRCH; num_rows++)
  {
      modem_mem_free(tdsint_ul_trch_priority_info[num_rows], MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    }
    modem_mem_free(tdsint_ul_trch_priority_info, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
    tdsint_ul_trch_priority_info = NULL;
  }

}




/*==============================================================================================
 FUNCTION NAME  tdsproc_ul_mac_config_signal

 FUNCTION   This function processes the config signal received from the physical layer.
===============================================================================================*/
void tdsproc_ul_mac_config_signal ( void )
{
  const tdsl1_ul_tfc_pwr_state_enum_type *tfc_pwr_state_ptr = NULL;
  const tdsl1_ul_rm_state_enum_type *tfc_ratem_state_ptr = NULL;

  /* Process RRC UL pending config */
  if (tdsul_configuration_pending)
  {
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"Recvd L1 sig,reconfig MAC now");

    /* Configure MAC only after this signal is recvd from L1 */
    tdsconfig_ul_mac(&tdsul_cfginfo);

    /* */
    TDSMAC_L1_TFC_EXCESS_PWR_INTLOCK();
    tfc_pwr_state_ptr = tdsl1ul_get_tfc_excess_pwr_states();
    TDSMAC_L1_TFC_EXCESS_PWR_INTFREE();

    if ((tfc_pwr_state_ptr != NULL) && (TDSGET_UL_CCTRCH_TYPE != TDSINVALID_UL_CCTRCH) &&
        (tdsoc_ul_tfcs_ptr != NULL))
    {
      tdsmac_initialize_pwr_state_for_tfcs(tfc_pwr_state_ptr);
    }
    else
    {
      TDSMAC_MSG0(MSG_LEGACY_ERROR,"TFC power state NULL ptr from L1 after reconfig");
    }
	
	/* Perform Rate matching update */
	tfc_ratem_state_ptr = tdsl1ul_get_rate_matching_states();
	if (tfc_ratem_state_ptr)
	{
      tdsmac_init_ratem_state_for_tfcs(tfc_ratem_state_ptr);
	}
	else
	{
	  TDSMAC_MSG0(MSG_LEGACY_ERROR,"TFC rate match state NULL ptr from L1 after reconfig");
	}  
	  
    /* Post a sucess message to L1 */
    tdsmaci_post_l1_config_ind(TDSL1_CPHY_MAC_UL_CONFIG_CNF);

    /* Clear the tdsul_configuration_pending flag */
    tdsul_configuration_pending=FALSE;
  }
  else
  {
    /* At this point, there is no config */
    TDSMAC_MSG0(MSG_LEGACY_ERROR,"Recvd L1 sig,NO pending configurations");
  }
}

/*-------------------------------------------------------------------
FUNCTION tdsmac_rrc_get_cipher_pending_status

DESCRIPTION
 This function will be called by RRC in the case when it receives
 a SMC message

DEPENDENCIES
  None

RETURN VALUE
  Boolean ( TRUE indicates MAC has not reached the ciphering
  activation time for the current config under use. FALSE indicates
  that there is no pending configuration in MAC and ciphering is
  ongoing

SIDE EFFECTS
  None
--------------------------------------------------------------------*/
boolean
tdsmac_rrc_get_cipher_pending_status(void)
{
  boolean ul_cipher_pending_stat;

  TDSL2_ACQ_CIPHER_PARAM_LOCK();
  ul_cipher_pending_stat = tdsul_cipher_activation_pending[0];
  TDSL2_ACQ_CIPHER_PARAM_UNLOCK();
  /* Return the ciphering pending status for the CS domain to RRC */
  return (ul_cipher_pending_stat);
}


/*-------------------------------------------------------------------
FUNCTION uint32 tdsmac_get_cnt_c_val()

DESCRIPTION
This function will be called by L1 to get the tdscount_c value

DEPENDENCIES
  None

RETURN VALUE
  The tdscount_c value corresponding the current cfn will be returned to L1
  for CS domain only. Ther is no tdscount_c maintained by MAC for the PS
  domain

SIDE EFFECTS
  None
--------------------------------------------------------------------*/
uint32 tdsmac_get_cnt_c_val(uint8 cfn)
{
  /* Return the ciphering pending status for the CS domain to RRC */
  return (tdscfn_hfn_table[0][cfn] );
}




/*-------------------------------------------------------------------
FUNCTION tdsmac_ul_allocate_mem_ext_cmd

DESCRIPTION
  This function is called by the external modules like RRC in order to allocate 
  dynamic memory. 
DEPENDENCIES
  The memory manager must have been previously initialized.

RETURN VALUE
  A pointer to the allocated memory if the memory was successfully allocated,
  otherwise NULL.

SIDE EFFECTS
  None.

--------------------------------------------------------------------*/
void *  tdsmac_ul_allocate_mem_ext_cmd
(
  size_t size
)
{
  void * return_ptr =  NULL;
  
  return_ptr = 
       modem_mem_calloc(1,size,
          MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
  

  if(return_ptr == NULL)
  {
    ERR_FATAL("Failed to allocate memory of size 0x%x for MAC ext cmd",size,0,0);
    
  }

  return(return_ptr);
}

/*-------------------------------------------------------------------
FUNCTION tdsmac_ul_free_mem_ext_cmd

DESCRIPTION
  This function is called   in order to free 
  dynamic memory allocated for the external commands. 

DEPENDENCIES
  The memory manager must have been previously initialized.

RETURN VALUE
  None

SIDE EFFECTS
  None.

--------------------------------------------------------------------*/
void  tdsmac_ul_free_mem_ext_cmd(void * ptr)
{
  modem_mem_free(ptr, MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);
}
/*===============================================================================================
FUNCTION NAME tdsmac_ul_get_trch_type

DESCRIPTION   This function returns the up-link transport channel type corresponding to given RLC & RB IDs
===============================================================================================*/

log_tdscdma_trch_type tdsmac_ul_get_trch_type(rlc_lc_id_type rlc_id, rb_id_type rb_id)
{
  log_tdscdma_trch_type return_trch_val = TDSCDMA_TRCH_INVALID;

  if(TDSGET_UL_CCTRCH_TYPE == TDSRACH)
  {
    return_trch_val = tdsmac_ul_check_rach_trch(rlc_id, rb_id);
    return return_trch_val;
  }
  else
  {
    return_trch_val = tdsmac_ul_check_dch_trch(rlc_id, rb_id);
    if(return_trch_val != TDSCDMA_TRCH_INVALID)
    {
      return return_trch_val;
    }

    return return_trch_val;
  }
}

/*===============================================================================================
FUNCTION NAME tdsl2_ul_get_trch_type

DESCRIPTION   This function returns the up-link transport channel type corresponding to given RLC & RB IDs
===============================================================================================*/

tdscdma_trch_type tdsl2_ul_get_trch_type(rlc_lc_id_type rlc_id, rb_id_type rb_id)
{
  tdscdma_trch_type return_trch_val = TDSCDMA_MAC_TRCH_INVALID;

  if(TDSGET_UL_CCTRCH_TYPE == TDSRACH)
  {
    return_trch_val = (tdscdma_trch_type)tdsmac_ul_check_rach_trch(rlc_id, rb_id);
    return return_trch_val;
  }
  else
  {
    return_trch_val = (tdscdma_trch_type)tdsmac_ul_check_dch_trch(rlc_id, rb_id);
    if(return_trch_val != TDSCDMA_MAC_TRCH_INVALID)
    {
      return return_trch_val;
    }

    return return_trch_val;
  }
}

/*===============================================================================================
FUNCTION NAME tdsl2_get_trch_type_for_given_rb_id

DESCRIPTION   
===============================================================================================*/

log_tdscdma_trch_type  tdsl2_get_trch_type_for_given_rb_id(rb_id_type rb_id, boolean direction, rlc_lc_id_type rlc_id)
{

  log_tdscdma_trch_type return_trch_val = TDSCDMA_TRCH_INVALID;

  if(direction == 1)
  {
    return_trch_val = tdsmac_ul_get_trch_type(rlc_id, rb_id);
  }
  else if(direction == 0)
  {
    return_trch_val = tdsmac_dl_get_trch_type(rlc_id, rb_id);
  }
  else
  {
    TDSMAC_MSG1(MSG_LEGACY_ERROR,"Invalid direction (%d) in function l2_get_trch_type_for_given_rb_id ",direction);
  }
  return return_trch_val;
}

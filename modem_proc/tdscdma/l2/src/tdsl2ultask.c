/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===

        L A Y E R  2   U P L I N K   TASK

GENERAL DESCRIPTION
  This file contains the Layer 2 Uplink task function.

Copyright (c) 2010-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

=============================================================================================*/
/*=============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/L2/vcs/l2ultask.c_v   1.18   10 Jul 2002 10:07:34   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/l2/src/tdsl2ultask.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ---------------------------------------------------------------------- 
03/30/15  sarao  FR26213 for QSH support 
10/13/14  sarao  CFCM changes for RLC
06/02/14  sarao   Removed the inclusion of tmc.h as its being discontinued in core.
02/02/11  ps      Changed for TD MAC HSUPA
10/24/10  guo   Initial release Upmerged WCDMA to TDS


=============================================================================================*/

/*=============================================================================================

                      I N C L U D E    F I L E S

=============================================================================================*/
#include "tdscdma_variation.h"
#include  "rex.h"
#include  "task.h"
#ifdef FEATURE_DOG
  //#include  "dog.h"
#include "dog_hb_rex.h"   //using DOG HB mechanism
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
#include"tdsrlc_qsh_ext.h"
#endif //FEATURE_QSH_EVENT_METRIC

#include  "msg.h"
#include  "err.h"
#include  "tdsl2ultask.h"
#include  "tdsmaculsubtask.h"
#include  "tdsrlculsubtask.h"
#include  "tdsrlcul.h"
#include  "tdsrlculam.h"
#include  "tdsmaculdata.h"
#include  "tdsl2external.h"
#include  "log_msgs_tdscdma.h"
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
#include  "tdsrlcfc.h"
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

#include  "tdsmaculsubtask.h"
#ifdef FEATURE_TDSCDMA_HSUPA
#include  "tdsmaculhssubtask.h"
#endif
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif
#include "tdsCFAParser.h"

#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
#include "tdsmaculhslog.h"
#endif
#include "tdsrrcnv.h"
#include "fs_fcntl.h"
#include "fs_public.h"
#include "fs_sys_types.h"
#include "fs_lib.h"
#include "tdsl1rrcif.h"
#ifdef FEATURE_MODEM_RCINIT 
#include "rcinit.h"
#endif

#ifdef FEATURE_SEGMENT_LOADING
#include "mcfg_seg_load.h"
#endif /*FEATURE_SEGMENT_LOADING*/
#include "tdsrrcdata.h"

/*=============================================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

=============================================================================================*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Uplink layer 2 command queue */
q_type        tdsl2_ul_cmd_q;

/* L2 UL TCB handle */
static rex_tcb_type* tds_l2_ul_tcb_ptr = RCINIT_NULL; 

#if 0 //using DOG HB mechanism
#ifdef FEATURE_DOG
  #undef  DOG_TDS_L2UL_RPT
  #define DOG_TDS_L2UL_RPT tds_l2ul_dog_rpt_var
  dog_report_type   tds_l2ul_dog_rpt_var           = 0;      /* Initial Safety */

  #undef  TDSDOG_TDS_L2UL_RPT_TIME
  #define TDSDOG_WCDMA_L2UL_RPT_TIME tds_l2ul_dog_rpt_time_var
  uint32  tds_l2ul_dog_rpt_time_var      = 0xFFFF; /* Initial Safety */

/* L2 UL watchdog report timer block  */
rex_timer_type tdsl2_ul_dog_rpt_timer_blk;
#endif
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


tdsl2_nv_rel_indicator_e_type  tdsl2_nv_rel_indicator;  /* nv release indicator */
uint32 tdsl2_opt_bitmask; /*Opt Bitmask NV */
tdsl2_flow_control_value_type tdsl2_flow_control_value;  /* Flow Control NV */


char*  l2_specail_test_item = "/nv/item_files/modem/tdscdma/rrc/special_test_setting_enabled"; 
tdsl2_special_test_nv_status_e_type  l2_specail_test_value = TDSL2_SPECIAL_SETTINGS_FOR_DEFAULT;
tdsl2_ivoice_type tdsl2_ivoice;

#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA
tdsl2_ta_opt_info_type 	tdsl2_ta_opt_info;				//record the info for ta optimization
#endif

#ifdef FEATURE_SEGMENT_LOADING
extern void tds_task_null (void);
#endif /*FEATURE_SEGMENT_LOADING*/

void tds_l2_ul_init_tcb(void)
{
  if (tds_l2_ul_tcb_ptr == RCINIT_NULL)
  {
    tds_l2_ul_tcb_ptr = rcinit_lookup_rextask("tds_l2_ul"); 
    if (tds_l2_ul_tcb_ptr == RCINIT_NULL)
    {
      TDSL2_MSG0(MSG_LEGACY_ERROR,"Lookup for tds_l2_ul_tcb failed");
      ASSERT(0);
    }
  }
}


rex_tcb_type* tds_l2_ul_get_tcb(void)
{
  if (tds_l2_ul_tcb_ptr == RCINIT_NULL)
  {
    tds_l2_ul_init_tcb();
  }
  return tds_l2_ul_tcb_ptr;
   
}

/*=============================================================================================

FUNCTION  tdsl2_ul_wait

DESCRIPTION
  This function waits for any signal in a specified signal mask to be set.
  If this procedure detects that any signal in the specified signal mask is
  set, it will return the entire mask of signals set in the task's tcb.

DEPENDENCIES
  None.

RETURN VALUE
  Mask of signals returned by the rex_wait call.

SIDE EFFECTS
  None.

=============================================================================================*/

uint32  tdsl2_ul_wait
  (
/* signals to wait on */
  uint32  wait_sigs
  )
{
  /* signals returned by rex_wait call */
  uint32  sigs;

  /* Wait for requested signals */
  while (((sigs = rex_get_sigs( rex_self()) ) & wait_sigs) == 0 )
  {

    (void)rex_wait(wait_sigs);

  }

  return( sigs );

} /* tdsl2_ul_wait */

/*=============================================================================================

FUNCTION  tdsproc_l2_ul_cmd_queue

DESCRIPTION
  This function processes the uplink layer 2 command queue data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/

void  tdsproc_l2_ul_cmd_queue ( void )
{
  tdsl2_ul_cmd_enum_type   cmd_id;
  tdsl2_ul_cmd_type      *cmd_ptr;

  while ((cmd_ptr = q_get(&tdsl2_ul_cmd_q)) != NULL)
  {
   
    /* Get the command ID */
    cmd_id = cmd_ptr->cmd_hdr.cmd_id;

    /* Process the command queue data based on the command ID */
    switch (cmd_id)
    {

    /* UL MAC Commands */
    case TDSCMAC_UL_CONFIG_REQ:

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
    /* RRC updates Persistence value  */
    case TDSMAC_UL_PERSISTENCE_VAL_CHANGE_IND:
#endif

      /* Get the configuration data from RRC for traffic volume measurements */
    case TDSCMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ:

      /* RRC can ask MAC to stop transmissions on RACH with this command */
    case TDSCMAC_UL_BLOCK_RACH_TRAFFIC_REQ:

      /* RRC ask MAC to block Traffic Volume Measurement */
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
    case TDSCMAC_UL_TRAFFIC_VOLUME_BLOCK_REQ: 
#endif

      /* RRC sends the TFCC config information */
    case TDSCMAC_UL_TFC_SUBSET_CFG_REQ:
    case TDSMAC_RACH_STATUS_IND:
#ifdef FEATURE_QSH_EVENT_METRIC
	case TDSMAC_QSH_CMD:              /* command sent by QSH for metrics logging in RLC */
		
#endif		

      tdsproc_ul_mac_commands (cmd_id, &cmd_ptr->cmd_data);
      break;


      /* UL RLC Commands */
    case TDSCRLC_UL_TM_CONFIG_REQ:
    case TDSCRLC_UL_UM_CONFIG_REQ:
    case TDSCRLC_AM_CONFIG_REQ:
    case TDSCRLC_AM_RESET_REQ:
    case TDSCRLC_UL_CIPHER_CONFIG_REQ:
    case TDSCRLC_UL_RESUME_REQ:
    case TDSRLC_UL_REGISTER_SRVC_REQ:
    case TDSRLC_UL_DEREGISTER_SRVC_REQ:
    case TDSRLC_REGISTER_AM_SDU_CONFIRM_CALLBACK_REQ:
    case TDSRLC_OPEN_RX_WIN:
    case TDSRLC_CLOSE_RX_WIN:
    case TDSRLCI_DL_CONFIG_CNF:          /* DL confirm config completion */
    case TDSRLCI_DL_RESET_DONE:
    case TDSRLCI_UL_RESET_REQ:
    case TDSRLCI_UL_DUP_RESET_REQ:
    case TDSCRLC_RELEASE_ALL_REQ:
    case TDSCRLC_UL_REG_FOR_DATA_IND_REQ:
    case TDSCRLC_UL_UNREG_FOR_DATA_IND_REQ:
    case TDSRLC_UL_PURGE_WM_REQ:         /*Command to purge the UL WM    */

    case TDSCRLC_UL_ABORT_CIPHER_CONFIG_REQ: /* Command to abort the new UL
                                             ciphering configuration */
    case TDSCRLC_UL_CONTINUE_REQ:
    case TDSRLCI_UL_TX_AM_STATUS:      /* command to indicate UL to send status PDU */
    case TDSRLCI_DL_RELEASE_ALL_CNF:  /*command to indicate dl release all cnf*/
#ifdef FEATURE_QSH_EVENT_METRIC
	case TDSRLC_QSH_CMD:              /* command sent by QSH for metrics logging in RLC */
#endif
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA
    case TDSRLC_UL_TA_IND:    /*The command to handle QTA\LTA indication from RRC */
#endif
      tdsproc_ul_rlc_commands (cmd_id, &cmd_ptr->cmd_data);

      break;

      /* Invalid command */
    default:
      ERR ("Invalid command:%d", cmd_ptr->cmd_hdr.cmd_id, 0,0  );
      break;
    }/* switch cmd-id */

    /* Return the command buffer to free queue */
    /*lint -e424 */
    
    modem_mem_free (cmd_ptr,MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);

	/*lint +e424 */

  }/* while (cmd_ptr != NULL) */

} /* tdsproc_l2_ul_cmd_queue */

/*=============================================================================================

FUNCTION  tds_l2_ul_task_init

DESCRIPTION
  This task initializes downlink layer 2 queues.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/
#ifdef FEATURE_SEGMENT_LOADING
void tds_l2_ul_task_init ( void ) __attribute__((section(".task_text")));
#endif /*FEATURE_SEGMENT_LOADING*/

void  tds_l2_ul_task_init ( void )
{
#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_tdscdma_segment = NULL;

  pi_tdscdma_segment = get_tdscdma_interface();
  if(pi_tdscdma_segment == NULL)
  {
    return;
  }
#endif /*FEATURE_SEGMENT_LOADING*/

  /* Initialize uplink layer 2 task command queue */
  (void)q_init(&tdsl2_ul_cmd_q);
#ifdef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
  tdsrlc_ul_crit_sect_init();
  tdsinit_ul_mac_crit_sect();
#endif

}

/*=============================================================================================

FUNCTION  tds_l2_ul_task

DESCRIPTION
  This task performs uplink layer 2 processing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/


#ifdef FEATURE_SEGMENT_LOADING
void tds_l2_ul_task (dword dummy) __attribute__((section(".task_text")));
#endif  /*FEATURE_SEGMENT_LOADING*/

void  tds_l2_ul_task
  (
  dword dummy
/* Required for REX, ignore */
/*lint -esym(715,dummy)
** Have lint not complain about the ignored parameter 'dummy' which is
** specified to make this routine match the template for rex_def_task().
*/
  )
{
  uint32        sigs;
  /* This default value here is meaningless. Just keep lint happy. */
  //tdsl2_nv_rel_indicator_e_type  nv_release_flag = TDSL2_REL_INDICATOR_5;

#ifdef FEATURE_DOG
  dog_report_type 		dogRpt;
#endif

#ifdef FEATURE_MODEM_RCINIT
  /* initialize the threshold count for counting Events from TRM */
  RCEVT_THRESHOLD current;
#endif  

#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_tdscdma_segment = NULL;
  pi_tdscdma_segment = get_tdscdma_interface();
  if(pi_tdscdma_segment == NULL)
  {
    tds_task_null();
    return;
  }
#endif /*FEATURE_SEGMENT_LOADING*/

  #ifndef FEATURE_MODEM_RCINIT
  /* Initialize uplink layer 2 task command queue */
  (void)q_init(&tdsl2_ul_cmd_q);
  #endif

  /* Waits for start signal from TMC */
  #ifdef FEATURE_MODEM_RCINIT 
  /* Send Handshake to RCINIT */
  rcinit_handshake_startup();
  #else
  /* Perform Startup Handshaking with TMC */
  /* Sets ACK signal when receives the task start signal */
  tmc_task_start();
  #endif

  tds_l2_ul_init_tcb();

#if 0
  /* read release info from NV.
     If NV not set or out of range value, use default.
     Default is to take the max. support. */
  if ( NV_DONE_S ==  (tdsl2_get_nv( NV_WCDMA_RRC_VERSION_I,
                    (nv_item_type *) &nv_release_flag)))
  {
    TDSL2_MSG1(MSG_LEGACY_HIGH,"NV_WCDMA_RRC_VERSION_I is %d",nv_release_flag);
    if( (nv_release_flag > TDSL2_REL_INDICATOR_5)
        /* nv_release_flag is uint. impossible to get a negative value */
        /* || (nv_release_flag < TDSL2_REL_INDICATOR_R4) */ )
    {
      nv_release_flag = TDSL2_REL_INDICATOR_5;
    }
  }
  else
  {
    TDSL2_MSG1(MSG_LEGACY_HIGH,"NV_WCDMA_RRC_VERSION_I not read. Setting to %d",nv_release_flag);
    nv_release_flag = TDSL2_REL_INDICATOR_5;

  }


  /*-------------------------------------------------------------------------
    Setting the NV flag value. If the nv data returns an error, this value
    would be 1 which means release 5.
  -------------------------------------------------------------------------*/
  tdsl2_nv_rel_indicator = nv_release_flag;
#endif

  tdsl2_read_special_test_nv();

  tdsrlc_ul_init();

  /* Initialize UL MAC */
  tdsinit_ul_mac();

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
  /* Initialize RLC-UL flow control */
  tdsrlc_ul_fc_init();
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

#ifdef FEATURE_QSH_EVENT_METRIC
  tdsrlc_qsh_register();
  tdsmac_qsh_register();

#endif


#ifdef FEATURE_MODEM_RCINIT 
#if !(defined(TEST_FRAMEWORK) || defined(TF_UNIT_TEST))
  current = rcevt_getcount_name("TRM_INIT_COMPLETE");	
  if(0==current) {
  (void) rcevt_wait_name("TRM_INIT_COMPLETE");
	}
#endif
#endif  

#ifdef FEATURE_DOG
#if 0 //using DOG HB mechanism
  /*Register with DOG, get the Task ID*/
  tds_l2ul_dog_rpt_var			= dog_register(&tds_l2_ul_tcb, \
																DOG_DEFAULT_TIMEOUT, DOG_DEADLKCHK_DISABLE);
  /*Get report interval*/
  tds_l2ul_dog_rpt_time_var = dog_get_report_period(tds_l2ul_dog_rpt_var);

  /* If the return value from dog_get_report_period() was zero then we MUST
  ** NOT report to dog directly, as DOG will monitor us in a different manner.
  ** Hence we won't initialize or start any timer.
  */
  if ( tds_l2ul_dog_rpt_time_var > 0 ) 
  {
    /* Initialize taskname watchdog report timer */
    rex_def_timer(&tdsl2_ul_dog_rpt_timer_blk, &tds_l2_ul_tcb, TDSL2_UL_DOG_RPT_TIMER_SIG);
    (void)rex_set_sigs (&tds_l2_ul_tcb, TDSL2_UL_DOG_RPT_TIMER_SIG);
  }
  else
  {
    TDSL2_MSG1(MSG_LEGACY_HIGH,"dog report time %d",tds_l2ul_dog_rpt_time_var);//auto-gen, to change remove 'QSR_' and first param
  }
#else
  /* Register with Dog HB. NOTE: Do not register multiple times per task context! */
  dogRpt = dog_hb_register_rex((rex_sigs_type)TDSL2_UL_DOG_RPT_TIMER_SIG);
#endif
#endif

  /* Task loop */
  /*lint -e716 */
  while (1)
  /*lint +e716 */
  {

    /* Wait for UL L2 signal to be set */
    sigs = tdsl2_ul_wait ( TDSL2_UL_CMD_Q_SIG   |
      TDSMAC_UL_BUILD_FRAME_IND_SIG  |
#ifdef FEATURE_DOG
      TDSL2_UL_DOG_RPT_TIMER_SIG     |
#endif

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
      TDSRLC_UL_AM_LOG_TIMER_EXP_SIG |
#endif
#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
      TDSMAC_HSUPA_STATUS_TIMER_EXP_SIG |
#endif


    /*signal set when the 10ms base traffic volume timer expires*/
      TDSMAC_UL_TVM_EVENT_TMR_EXP_SIG |
      TDSMAC_UL_TVM_PERIODIC_TMR_EXP_SIG|
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
      TDSMAC_UL_TVM_BLOCK_TMR_EXP_SIG|
#endif
      TDSMAC_PHY_UL_CONFIG_IND       |

      TDSMAC_UL_TFCS_CONTROL_TMR_EXP_SIG |
      TDSRLC_AM_RESET_REQUEST_SIG    |
      TDSRLC_UL_RESET_REPORT_SIG     |
      TDSRLC_AM_UL_SDU_TX_CNF_SIG    |
#ifdef FEATURE_TDSCDMA_HSUPA    /* Handle timers related to HSUPA */
      TDS_MACULHS_T_WAIT_TMR_EXP_SIG |
      TDS_MACULHS_PERIODIC_SI_TMR_EXP_SIG |
      TDS_MACULHS_PERIODIC_NON_SCHED_SI_TMR_EXP_SIG |
      TDS_MACULHS_T_RUCCH_TMR_EXP_SIG |
      TDS_MACULHS_T_RUCCH_HYST_TMR_EXP_SIG |
      TDS_MACULHS_ERUCCH_STATUS_IND_SIG |  

#endif
      TDSRLC_AM_UL_RESET_DONE_SIG
#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
      |
      TDSRLC_UL_FC_SIG|
      TDSRLC_UL_FC_TIMER_SIG
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
      |
      TDSRLC_UL_START_LOGGING_SIG
#endif
      | TDSRLC_UL_AM_BASE_TIMER_SIG
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
			#error code not present
#endif/*TEST_FRAMEWORK*/     

      );

#ifdef FEATURE_DOG
    /* L2 UL watchdog report timer sig  */
    if (sigs & TDSL2_UL_DOG_RPT_TIMER_SIG)
    {
      /* Clear UL watchdog report timer sig */
      (void)rex_clr_sigs (rex_self(), TDSL2_UL_DOG_RPT_TIMER_SIG);

#if 0 //using DOG HB mechanism
      if (tds_l2ul_dog_rpt_time_var > 0) 
      {    /* Zero means don't report */
        (void)rex_set_timer(&tdsl2_ul_dog_rpt_timer_blk, tds_l2ul_dog_rpt_time_var);
        dog_report(tds_l2ul_dog_rpt_var);
      }
      else
      {
        TDSL2_MSG1(MSG_LEGACY_HIGH,"dog report time %d",tds_l2ul_dog_rpt_time_var);//auto-gen, to change remove 'QSR_' and first param
      }
#else
      /* Report to the Dog HB */
      dog_hb_report(dogRpt);
#endif
    }
#endif

    /* Uplink L2 Command Queue Signal is set */
    if (sigs & TDSL2_UL_CMD_Q_SIG)
    {
      /* Clear uplink Layer 2 Command Queue Signal */
      (void)rex_clr_sigs (rex_self(), TDSL2_UL_CMD_Q_SIG);

      /* Process uplink layer 2 command queue */
      tdsproc_l2_ul_cmd_queue ();

    }


    /* UL Build Frame Indication */
    if (sigs & TDSMAC_UL_BUILD_FRAME_IND_SIG)
    {
      /* Clear UL Transmit Status Indication Signal */
      (void)rex_clr_sigs (rex_self(), TDSMAC_UL_BUILD_FRAME_IND_SIG);

      /* Process Build Frame Indication Signal */
      tdsproc_build_ul_frame_ind ();
    }

    if (sigs & TDSRLC_AM_RESET_REQUEST_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), TDSRLC_AM_RESET_REQUEST_SIG);

      /* Post RLC RESET message */
      tdsrlci_am_reset();
    }

    if (sigs & TDSRLC_UL_RESET_REPORT_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), TDSRLC_UL_RESET_REPORT_SIG);

      /* Post RLC reset to RRC */
      tdsrlci_post_reset_to_rrc();
    }

    if (sigs & TDSRLC_AM_UL_RESET_DONE_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), TDSRLC_AM_UL_RESET_DONE_SIG);

      /* Post RLC UL RESET Done to RLC DL */
      tdsrlci_post_rlc_ul_reset_done_cmd();
    }

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING

    if (sigs & TDSRLC_UL_AM_LOG_TIMER_EXP_SIG)
    {
      /* Clear UL AM0 10ms Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), TDSRLC_UL_AM_LOG_TIMER_EXP_SIG);

      /* Call AM log timer expiration handler  */
      tdsrlci_ul_am_handle_qxdm_log_timer(TDSRLC_UL_AM_LOG_TIMER_EXP_SIG);
    }

#endif

#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
    if (sigs & TDSMAC_HSUPA_STATUS_TIMER_EXP_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), TDSMAC_HSUPA_STATUS_TIMER_EXP_SIG);

      if (log_status(TDSCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET))
      {
	    tdscdma_mac_hs_ul_etfci_status_log_packet();
      }

      if (log_status(TDSCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET))
      {
	    tdscdma_mac_hs_ul_header_status_log_packet();
      }

      /* restart tmr */
      rex_set_timer(&tdsmac_hsupa_tmr_log_blk,(uint32)TDSMAC_HSUPA_STATUS_LOG_TIMER_BASE);
      
    }
#endif

    if (sigs & TDSRLC_AM_UL_SDU_TX_CNF_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), TDSRLC_AM_UL_SDU_TX_CNF_SIG);

      /* Post RLC UL SDU tx cnformation */
      tdsrlci_post_sdu_tx_cnf();
    }

 if (sigs & TDSMAC_UL_TVM_EVENT_TMR_EXP_SIG)
    {
      /* Clear UL Traffic volume 10ms Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), TDSMAC_UL_TVM_EVENT_TMR_EXP_SIG);

      /* Handle the expiry of the 10ms timer signal */
      tdsmac_ul_handle_tvm_event_tmr();
    }

    if (sigs & TDSMAC_UL_TVM_PERIODIC_TMR_EXP_SIG)
    {
      /* Clear UL Traffic volume (10ms * TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE) Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), TDSMAC_UL_TVM_PERIODIC_TMR_EXP_SIG);

      /* Handle the expiry of the 10ms timer signal */
      tdsmac_ul_handle_tvm_periodic_tmr();
    }

#ifdef FEATURE_TDSCDMA_TVM_BLOCK
    if (sigs & TDSMAC_UL_TVM_BLOCK_TMR_EXP_SIG)
    {
      /* Clear UL TVM Block Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), TDSMAC_UL_TVM_BLOCK_TMR_EXP_SIG);

      /* Handle the expiry of the block timer signal */
      tdsmac_ul_handle_tvm_block_tmr();
    }
#endif

    /* This signal is posted by PHYSICAL layer. It indicates to MAC that
    it should using the new configuration parameters already sent by RRC */
    if (sigs & TDSMAC_PHY_UL_CONFIG_IND)
    {
      /* Clear UL config signal  */
      (void)rex_clr_sigs (rex_self(), TDSMAC_PHY_UL_CONFIG_IND);

      /* Process the signal received from L1. */
      tdsproc_ul_mac_config_signal();
    }



    if (sigs & TDSMAC_UL_TFCS_CONTROL_TMR_EXP_SIG)
    {
      /* Clear UL Traffic volume 10ms Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), TDSMAC_UL_TFCS_CONTROL_TMR_EXP_SIG);

      TDSL2_MSG0(MSG_LEGACY_HIGH,"Control timer expired for TFCC msg");

      /* Handle the expiry of the 10ms timer signal */
      tdsmac_ul_handle_tfcs_control_timer_expiry();
    }


#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
    if(sigs & TDSRLC_UL_FC_SIG)
    {
      (void)rex_clr_sigs(rex_self(), TDSRLC_UL_FC_SIG);
      tdsrlc_fc_cmd_hdlr(TDSRLC_FC_UPLINK);
    }
#endif

#ifdef FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL
    if(sigs & TDSRLC_UL_FC_TIMER_SIG)
    {
      (void)rex_clr_sigs(rex_self(), TDSRLC_UL_FC_TIMER_SIG);
      tdsrlc_fc_timer_cmd_hdlr(TDSRLC_FC_UPLINK);
    }
#endif /* FEATURE_TDSCDMA_CPU_BASED_FLOW_CONTROL */

#ifdef FEATURE_TDSCDMA_HSUPA

      
    if (sigs & TDS_MACULHS_PERIODIC_SI_TMR_EXP_SIG )
    {

      (void)rex_clr_sigs (rex_self(), TDS_MACULHS_PERIODIC_SI_TMR_EXP_SIG);

      /* Handle the expiry of the timer signal */
      tdsmac_ulhs_handle_timer_expiry(TDS_MACULHS_PERIODIC_SI_TMR_EXP_SIG);

    }

    if (sigs & TDS_MACULHS_T_WAIT_TMR_EXP_SIG)
    {
      
      (void)rex_clr_sigs (rex_self(), TDS_MACULHS_T_WAIT_TMR_EXP_SIG);

      /* Handle the expiry of the timer signal */
      tdsmac_ulhs_handle_timer_expiry(TDS_MACULHS_T_WAIT_TMR_EXP_SIG);

    }
	 if (sigs & TDS_MACULHS_PERIODIC_NON_SCHED_SI_TMR_EXP_SIG )
    {

      (void)rex_clr_sigs (rex_self(), TDS_MACULHS_PERIODIC_NON_SCHED_SI_TMR_EXP_SIG);

      /* Handle the expiry of the timer signal */
      tdsmac_ulhs_handle_timer_expiry(TDS_MACULHS_PERIODIC_NON_SCHED_SI_TMR_EXP_SIG);

    }

    if (sigs & TDS_MACULHS_T_RUCCH_TMR_EXP_SIG)
    {
      
      (void)rex_clr_sigs (rex_self(), TDS_MACULHS_T_RUCCH_TMR_EXP_SIG);

      /* Handle the expiry of the timer signal */
      tdsmac_ulhs_handle_timer_expiry(TDS_MACULHS_T_RUCCH_TMR_EXP_SIG);

    }
	if (sigs & TDS_MACULHS_T_RUCCH_HYST_TMR_EXP_SIG)
    {
      
      (void)rex_clr_sigs (rex_self(), TDS_MACULHS_T_RUCCH_HYST_TMR_EXP_SIG);

      /* Handle the expiry of the timer signal */
      tdsmac_ulhs_handle_timer_expiry(TDS_MACULHS_T_RUCCH_HYST_TMR_EXP_SIG);

    }
	if (sigs & TDS_MACULHS_ERUCCH_STATUS_IND_SIG)
    {
      
      (void)rex_clr_sigs (rex_self(), TDS_MACULHS_ERUCCH_STATUS_IND_SIG);

      tdsmac_ulhs_proc_erucch_status();

    }
#endif // FEATURE_TDSCDMA_HSUPA

#ifdef FEATURE_TDSCDMA_RLC_QXDM_LOGGING
    if(sigs & TDSRLC_UL_START_LOGGING_SIG)
    {
      (void)rex_clr_sigs(rex_self(), TDSRLC_UL_START_LOGGING_SIG);
      tdsrlc_start_ul_logging();
    }
#endif


    if (sigs & TDSRLC_AM_RESET_STS_TMR_REQ_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), TDSRLC_AM_RESET_STS_TMR_REQ_SIG);

      tdsrlci_reset_status_timer_cmd();
    }


    if (sigs & TDSRLC_UL_AM_BASE_TIMER_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), TDSRLC_UL_AM_BASE_TIMER_SIG);

      tdsrlci_ul_am_base_timer_handler();
    }

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif/*TEST_FRAMEWORK*/     
    
  } /* while loop */

#ifdef FEATURE_DOG
  /* Deregister if leaving the task so that we don't ERR_FATAL */
  dog_hb_deregister(dogRpt);
#endif

}/* tds_l2_ul_task */

/*=============================================================================================

FUNCTION  tdsl2_ul_get_cmd_buf

DESCRIPTION
  This function should be called to get a command buffer from uplink Layer 2
  task command free queue

DEPENDENCIES
  None.

RETURN VALUE

  Pointer to the uplink L2 command buffer if one is available
  NULL  otherwise

SIDE EFFECTS

=============================================================================================*/
tdsl2_ul_cmd_type    *tdsl2_ul_get_cmd_buf
  (
  void
  )
{
  void * return_ptr =NULL;
  size_t cmd_size = sizeof(tdsl2_ul_cmd_type);
   
  return_ptr = modem_mem_calloc(1,cmd_size,
          MODEM_MEM_CLIENT_TDSCDMA_L2_CRIT);


  if(return_ptr == NULL)
  {
    ERR_FATAL("Failed to allocate from Modem Mem Heap: size:0x%x",cmd_size,0,0);
  }

  return (tdsl2_ul_cmd_type *)return_ptr;

} /* tdsl2_ul_put_cmd */


/*=============================================================================================

FUNCTION  tdsl2_ul_put_cmd

DESCRIPTION
  This function should be called to send a command to the uplink Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Item will be enqueued for uplink Layer 2 task and signal will be set to
  indicate that the item has been enqueued.

=============================================================================================*/

void  tdsl2_ul_put_cmd
  (

  tdsl2_ul_cmd_type    *cmd_ptr
/* pointer to command to be enqueued */
  )
{
  tds_parse_cfa(TDSCFA_L2UL_Q,(void *)cmd_ptr);
  /* initialize link field */
  (void)q_link(cmd_ptr, &cmd_ptr->cmd_hdr.link );
  
  /* put item on queue for tds_l2_ul_task */
  q_put( &tdsl2_ul_cmd_q, &cmd_ptr->cmd_hdr.link );

  /* Set signal for the tdsl2_ul_cmd_q */
  (void)rex_set_sigs( tds_l2_ul_get_tcb(), TDSL2_UL_CMD_Q_SIG );

} /* tdsl2_ul_put_cmd */

/*===========================================================================

FUNCTION tdsl2_get_flow_control_nv

DESCRIPTION
  This  function returns the NV release flag to mac and rlc.

DEPENDENCIES
  None.

RETURN VALUE
  tdsl2_nv_rel_indicator_e_type.

===========================================================================*/
tdsl2_flow_control_value_type tdsl2_get_flow_control_nv
(
  void
)
{

  tdsrrc_rlc_nv_list_type tdsrrc_rlc_nv_list;  
  tdsrrc_rlc_nv_list =  tdsrrc_get_rlc_nv_list(); 
  return tdsrrc_rlc_nv_list.l2_flow_control_nv;

}


/*===========================================================================

FUNCTION tdsl2_get_release_flag_indicator

DESCRIPTION
  This  function returns the NV release flag to mac and rlc.

DEPENDENCIES
  None.

RETURN VALUE
  tdsl2_nv_rel_indicator_e_type.

===========================================================================*/
tdsl2_nv_rel_indicator_e_type tdsl2_get_release_flag_indicator
(
  void
)
{
  tdsrrc_rlc_nv_list_type tdsrrc_rlc_nv_list;

  tdsrrc_rlc_nv_list = tdsrrc_get_rlc_nv_list();

  switch (tdsrrc_rlc_nv_list.rrc_version)
  {
    case TDSRRC_REL_VERSION_4:
      tdsl2_nv_rel_indicator = TDSL2_REL_INDICATOR_R4;
      break;

    case TDSRRC_REL_VERSION_5:
      tdsl2_nv_rel_indicator = TDSL2_REL_INDICATOR_5;
      break;

    case TDSRRC_REL_VERSION_6:
      tdsl2_nv_rel_indicator = TDSL2_REL_INDICATOR_6;
      break;

    case TDSRRC_REL_VERSION_7:
      tdsl2_nv_rel_indicator = TDSL2_REL_INDICATOR_7;
      break;      

    case TDSRRC_REL_VERSION_8:
      tdsl2_nv_rel_indicator = TDSL2_REL_INDICATOR_8;
      break;      

    case TDSRRC_REL_VERSION_9:
      tdsl2_nv_rel_indicator = TDSL2_REL_INDICATOR_9;
      break;      

    default:
      TDSL2_MSG1(MSG_LEGACY_ERROR,"not find this rrc rel version %d",tdsrrc_rlc_nv_list.rrc_version);
      tdsl2_nv_rel_indicator = TDSL2_REL_INDICATOR_5;
      break;            
      
  }
    
  return tdsl2_nv_rel_indicator;
}

/*===========================================================================

FUNCTION tdsl2_get_optimization_mask

DESCRIPTION
  This  function returns the NV release flag to mac and rlc.

DEPENDENCIES
  None.

RETURN VALUE
  tdsl2_nv_rel_indicator_e_type.

===========================================================================*/
uint32 tdsl2_get_optimization_mask
(
  void
)
{
  tdsrrc_rlc_nv_list_type tdsrrc_rlc_nv_list;

  tdsrrc_rlc_nv_list = tdsrrc_get_rlc_nv_list();

  return tdsrrc_rlc_nv_list.l2_opt_bitmask;
}

/*===========================================================================
FUNCTION tdsl2_get_nv_rel_ver_opt_mask

DESCRIPTION
  Get items NV Rel version and Opt mask from RRC. Since its a compressed function,
  we decide to read and store the values locally.

RETURN VALUE
  None

DEPENDENCIES
  None
===========================================================================*/
void tdsl2_get_nv_rel_ver_opt_mask
(
  void
)
{
  tdsl2_nv_rel_indicator = tdsl2_get_release_flag_indicator();
  tdsl2_opt_bitmask = tdsl2_get_optimization_mask(); 
  tdsl2_flow_control_value = tdsl2_get_flow_control_nv();
  
	/* Store the NV items for use.*/ 
  TDSL2_MSG2(MSG_LEGACY_HIGH,"tdsl2_nv_rel_indicator is %d and tdsl2_opt_bitmask is %d",tdsl2_nv_rel_indicator,tdsl2_opt_bitmask);
}

/*===========================================================================
FUNCTION L2_GET_NV

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
nv_stat_enum_type tdsl2_get_nv
(
  nv_items_enum_type nv_item,        /* which item */
  nv_item_type *tdsdata_ptr          /* pointer to space for item */
)
{
  nv_cmd_type          l2_nv_cmd;

  /* Prepare the NV read command buffer. */
  l2_nv_cmd.item       = nv_item;           /* item to read */
  l2_nv_cmd.cmd        = NV_READ_F;         /* NV operation */
  l2_nv_cmd.data_ptr   = tdsdata_ptr;          /* buffer to store read data  */
  l2_nv_cmd.tcb_ptr    = rex_self();        /* Notify this task when done */
  l2_nv_cmd.sigs       = TDSL2_NV_CMD_SIG;    /* Signal with this sig when done */
  l2_nv_cmd.done_q_ptr = NULL;              /* Return cmd to NO Q when done */
  l2_nv_cmd.status     = NV_STAT_ENUM_MAX;  /* Set a default value */

  /* Read to NV by:
  **
  ** 1. Clearing the NV signal.
  ** 2. Queuing the read command to the NV task.
  ** 3. Waiting for the NV task to finish servicing the read command.
  */
  (void) rex_clr_sigs( rex_self(), TDSL2_NV_CMD_SIG);
  nv_cmd( &l2_nv_cmd );
  (void) tdsl2_ul_wait(TDSL2_NV_CMD_SIG);
  (void) rex_clr_sigs( rex_self(), TDSL2_NV_CMD_SIG);

  return l2_nv_cmd.status;
}

/*===========================================================================

FUNCTION tdsl2_read_special_test_nv

DESCRIPTION
  Reads special test from EFS
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
#ifdef FEATURE_TDSCDMA_EFS_NEW_API
void tdsl2_read_special_test_nv(void)
{
  mcfg_fs_status_e_type status;

  status = mcfg_fs_read(l2_specail_test_item,(void*)&(l2_specail_test_value), sizeof(uint8),
                         MCFG_FS_TYPE_EFS,tdsrrc_get_efs_storage_sub_id());

  if(status != MCFG_FS_STATUS_OK)
  {
    TDSL2_MSG2(MSG_LEGACY_HIGH,"l2_specail_test_value: specail test NV item read failed (mcfg fs status: %d, efs errno: %d)",status,mcfg_fs_errno(MCFG_FS_TYPE_EFS));
    /* go with the default configuration */
    l2_specail_test_value = TDSL2_SPECIAL_SETTINGS_FOR_DEFAULT;
  }
  else
  {
    TDSL2_MSG1(MSG_LEGACY_HIGH,"Read Specail test %d from EFS",l2_specail_test_value);
  }
}
#else
void tdsl2_read_special_test_nv(void)
{
  int8 efs_ret_val;

  efs_ret_val = efs_get(l2_specail_test_item,(void*)&(l2_specail_test_value), sizeof(uint8));
  if((efs_ret_val < 0) || ((uint8)efs_ret_val > TDSL2_SPECIAL_SETTINGS_FOR_FIELD))
  {
    TDSL2_MSG2(MSG_LEGACY_HIGH,"l2_specail_test_value: specail test NV item read failed (efs errno:%d) or value %d bigger than 2.",efs_errno,efs_ret_val);
    /* go with the default configuration */
    l2_specail_test_value = TDSL2_SPECIAL_SETTINGS_FOR_DEFAULT;
  }
  else
  {
    TDSL2_MSG1(MSG_LEGACY_HIGH,"Read Specail test %d from EFS",l2_specail_test_value);
  }
}
#endif
#ifdef FEATURE_DUAL_WCDMA
/*===========================================================================

FUNCTION:        tdsl2_ul_service_register_sub

DESCRIPTION:
  This function should be called to send a L2 command to L2-UL task 
  for water mark registering or de-registering.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  Command will be enqueued in L2-UL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/
boolean tdsl2_ul_service_register_sub
(
  sys_modem_as_id_e_type as_id,
  tdsl2_ul_service_register_type *srvc_ptr
)
{
  TDSL2_MSG1(MSG_LEGACY_HIGH,"received as_id: %d",as_id);
  return tdsl2_ul_service_register(srvc_ptr);
}
#endif
/*===========================================================================

FUNCTION:        L2_UL_SERVICE_REGISTER

DESCRIPTION:
  This function should be called to send a L2 command to L2-UL task 
  for water mark registering or de-registering.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  Command will be enqueued in L2-UL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/
boolean tdsl2_ul_service_register
(
  tdsl2_ul_service_register_type *srvc_ptr
)
{
  uint16 i;

  tdsl2_ul_cmd_type *ul_srvc_cmd_ptr;

  /* Validate # LC number */
  if (srvc_ptr->nchan > TDSL2_MAX_UL_LC)
  {
    ERR_FATAL("number of channels:%d trying to register l2 dl over limite:%d",srvc_ptr->nchan,TDSL2_MAX_UL_LC,0);    
  }
  
  ul_srvc_cmd_ptr = tdsl2_ul_get_cmd_buf();

  
  if (ul_srvc_cmd_ptr == NULL)
  {
    TDSL2_MSG0(MSG_LEGACY_ERROR,"Couldn't get L2 uplink cmd buffer");
    return FALSE;
  }

  if (srvc_ptr->service == TRUE)
  {
    ul_srvc_cmd_ptr->cmd_hdr.cmd_id = TDSRLC_UL_REGISTER_SRVC_REQ;
    ul_srvc_cmd_ptr->cmd_data.ul_reg.nchan = srvc_ptr->nchan;

    for (i = 0; i < srvc_ptr->nchan; i++)
    {
      ul_srvc_cmd_ptr->cmd_data.ul_reg.rlc_data[i].lc_id = srvc_ptr->rlc_id[i];
      ul_srvc_cmd_ptr->cmd_data.ul_reg.rlc_data[i].ul_wm_ptr = srvc_ptr->ul_wm_ptr[i];        
    }
  }
  else
  {
    ul_srvc_cmd_ptr->cmd_hdr.cmd_id = TDSRLC_UL_DEREGISTER_SRVC_REQ;
    ul_srvc_cmd_ptr->cmd_data.ul_dereg.nchan = srvc_ptr->nchan;

    for (i = 0; i < srvc_ptr->nchan; i++)
    {
      ul_srvc_cmd_ptr->cmd_data.ul_dereg.rlc_id[i] = srvc_ptr->rlc_id[i];
    }
  }
 

  tdsl2_ul_put_cmd(ul_srvc_cmd_ptr);
    
  return TRUE;
}
/*===========================================================================

FUNCTION:        tdsl2_ul_submit_datamodem_msg

DESCRIPTION:
  This function should be called by the DATA MODEM to send a command
  to the L2 UL task
    The pointer for cmd_ptr passed  should be freed by the caller.
DEPENDENCIES:
  None.

RETURN VALUE:
  boolean TRUCE if succes else FALSE.

SIDE EFFECTS:
  The pointer for cmd_ptr passed  should be freed by the caller.

  Command will be enqueued in L2-UL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/

boolean tdsl2_ul_submit_datamodem_msg
(
  tdsl2_ul_datamodem_api_type * api_ptr
)
{
#ifdef FEATURE_TDSCDMA
  tdsl2_ul_cmd_type * l2_cmd_ptr;
  switch(api_ptr->api_id)
  {
    case  TDSRLC_UL_MODEMDATA_REG_DEREG_SRVC_REQ: /* Register Deregister Uplink Watermark */
      if(tdsl2_ul_service_register(&api_ptr->api_data.ul_reg_dereg_cmd) == FALSE)
      {
        ERR_FATAL("Unable to allocate memory for Reg Dereg command in L2 Ul",0,0,0);
      }
    break;
    case TDSRLC_UL_DATAMODEM_OPEN_RX_WIN:            /* Open the Receiving Window. */
    {
      l2_cmd_ptr = tdsl2_ul_get_cmd_buf();
      if(l2_cmd_ptr == NULL)
      {
        TDSL2_MSG0(MSG_LEGACY_ERROR,"Couldn't get L2 uplink cmd buffer");
        return FALSE;
      }
      l2_cmd_ptr->cmd_hdr.cmd_id = TDSRLC_OPEN_RX_WIN;
      l2_cmd_ptr->cmd_data.ul_rlc_data_id
         = api_ptr->api_data.ul_rlc_data_id;

      tdsl2_ul_put_cmd(l2_cmd_ptr);
    }
    break;
     case TDSRLC_UL_DATAMODEM_CLOSE_RX_WIN:           /* Close the Receiving Window.  */
    {
      l2_cmd_ptr = tdsl2_ul_get_cmd_buf();
      if(l2_cmd_ptr == NULL)
      {
        TDSL2_MSG0(MSG_LEGACY_ERROR,"Couldn't get L2 uplink cmd buffer");
        return FALSE;
      }
      l2_cmd_ptr->cmd_hdr.cmd_id = TDSRLC_CLOSE_RX_WIN;
      l2_cmd_ptr->cmd_data.ul_rlc_data_id
         = api_ptr->api_data.ul_rlc_data_id;
      
      tdsl2_ul_put_cmd(l2_cmd_ptr);
    }
    break;

    default:
      TDSL2_MSG1(MSG_LEGACY_ERROR,"Unexpected command %d sent to l2 Ul by data modem",api_ptr->api_id);
  }

  return TRUE;
#else
  MSG_ERROR("TDS doesn't exist", 0,0,0);
  return FALSE;
#endif
}
#ifdef FEATURE_DUAL_WCDMA
/*===========================================================================

FUNCTION:        tdsl2_ul_submit_datamodem_msg_sub

DESCRIPTION:
  This function should be called by the DATA MODEM to send a command
  to the L2 UL task
  The pointer for cmd_ptr passed  should be freed by the caller.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  boolean TRUE if succes else FALSE.

SIDE EFFECTS:
  The pointer for cmd_ptr passed  should be freed by the caller.
  
  Command will be enqueued in L2-UL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/


boolean tdsl2_ul_submit_datamodem_msg_sub
(
  sys_modem_as_id_e_type as_id,
  tdsl2_ul_datamodem_api_type * cmd_ptr
)
{
  return tdsl2_ul_submit_datamodem_msg(cmd_ptr);
}

#endif

#ifdef FEATURE_TDSCDMA_SILENT_FRAME

/*==============================================================================================
 FUNCTION NAME  tdscdma_register_vfr_notification

 DESCRIPTION  This function implements the registration/de-registration of call back from MVS for silent frame
===============================================================================================*/

void  tdscdma_register_vfr_notification
(
tds_vfr_cb_type cb_ptr, /* CB pointer which will be saved */
void* client_data
)

{
  REX_ISR_LOCK(&tdsr99_ul_isr_l2_datapath_mutex);
//Check if its reg or de-reg

 if(cb_ptr == NULL)
 {
	TDSL2_MSG0(MSG_LEGACY_HIGH,"Silent frame: MVS VFR De-registration started ");
	tdsmac_save_vfr_cb(cb_ptr);
	tdsmac_save_vfr_client_data(NULL);
 }

 else
 {

	TDSL2_MSG0(MSG_LEGACY_HIGH,"Silent frame: MVS VFR Registration started ");
	//if reg: Save the CB ptr in a global variable so that we can use it when we call MVS.
	tdsmac_save_vfr_cb(cb_ptr);
	if(NULL != client_data)
		{
		tdsmac_save_vfr_client_data(client_data);
		}
	else
		{
		TDSL2_MSG0(MSG_LEGACY_HIGH,"Silent frame: Client data sent is NULL ");
		}
  }
  REX_ISR_UNLOCK(&tdsr99_ul_isr_l2_datapath_mutex);
}

#endif /*FEATURE_TDSCDMA_SILENT_FRAME*/


/*==============================================================================================
 FUNCTION NAME  tds_api_call

 DESCRIPTION  This function implements the wrapper for all public APIs. This wrapper function can be used by external modules to use 
 TDSCDMA APIs. Currently this function supports the following  L2 external APIs tdsl2_ul_service_register(), tdsl2_dl_service_register()
 and  tdscdma_register_vfr_notification().
 
 Instead of calling the APIs directly, MVS/Audio or other modules can call the generic tdscdma_l2_api_call() and provide the arguments and
 cmd_id suggesting which API they are referring. The wrapper function will internally call the APIs and provide the result back.
 
 The return value is defined in enum tds_l2_api_ret_value_t. Users can use this enum to find out the error cause.
 
 DEPENDENCIES:
  None.

RETURN VALUE:
  tds_l2_api_ret_value_t.

SIDE EFFECTS:
None.
===============================================================================================*/
tds_l2_api_ret_value_t tdscdma_l2_api_call 
(
  uint32   cmd_id,   /*This will tell us which API needs to be called */
  void*    params,   /* This will provide the API arguments */
  uint32   size      
)
{

	tds_l2_api_ret_value_t rc = TDS_EOK;
	boolean ret_value = FALSE;

	/* Perform common parameter checks */

	if( size >0) 
	{
		if(params == NULL)
		{
			TDSL2_MSG2(MSG_LEGACY_ERROR,"Unexpected NULL params received cmd_id %d and size %d ",cmd_id,size);
			return TDS_EBADPARAM;
		}
		
	}
	
	else
	{
			TDSL2_MSG2(MSG_LEGACY_ERROR,"Unexpected Zero size params received cmd_id %d and size %d ",cmd_id,size);
			return TDS_EBADPARAM;
	}
	
	 /* Command parser. */
	 switch (cmd_id)
	 {
#ifdef FEATURE_TDSCDMA
	 case TDSL2_CMD_UL_SERVICE_REGISTER: {
		tdsl2_cmd_ul_service_register_t* args = ((tdsl2_cmd_ul_service_register_t*)params);
		/*if(size != sizeof(tdsl2_cmd_ul_service_register_t))
		{
			TDSL2_MSG2(MSG_LEGACY_ERROR,"Unexpected cmd size received for cmd_id %d and size %d ",cmd_id,size);
			return TDS_EBADPARAM;
		} */ ////Need to check if required
			
		if(NULL ==(args -> service))
		{
			TDSL2_MSG2(MSG_LEGACY_ERROR,"Unexpected NULL pointer for UL service reg cmd_id %d and size %d ",cmd_id,size);
			return TDS_EBADPARAM;
		}
		ret_value = tdsl2_ul_service_register(args->service);
		if(ret_value)
		{
				rc = TDS_EOK;
		}
		else
		{
				rc = TDS_EFAILED;
		}
		
	  } break;
	 
	 
	 case TDSL2_CMD_DL_SERVICE_REGISTER: {
		tdsl2_cmd_dl_service_register_t* args = ((tdsl2_cmd_dl_service_register_t*)params);
		/*if(size != sizeof(tdsl2_cmd_dl_service_register_t))
		{
			TDSL2_MSG2(MSG_LEGACY_ERROR,"Unexpected cmd size received for cmd_id %d and size %d ",cmd_id,size);
			return TDS_EBADPARAM;
		} */  //Need to check if required
		
		if(NULL == (args-> service))
		{
			TDSL2_MSG2(MSG_LEGACY_ERROR,"Unexpected NULL pointer for DL service reg cmd_id %d and size %d ",cmd_id,size);
			return TDS_EBADPARAM;
		}
		ret_value = tdsl2_dl_service_register(args->service);
		if(ret_value)
		{
				rc = TDS_EOK;
		}
		else
		{
				rc = TDS_EFAILED;
		}
			
	 } break;
	 
	 
	 case TDSL2_CMD_VFR_NOTIFICATION_REGISTER: {
	 
		tdscdma_register_vfr_notification_t* args = ((tdscdma_register_vfr_notification_t*)params);
		tdscdma_register_vfr_notification(args-> service, args ->client_dt);
		rc = TDS_EOK;
		
	 }  break;

#endif //FEATURE_TDSCDMA	 
	 
	 default:
	 {
		TDSL2_MSG2(MSG_LEGACY_ERROR,"Unsupported command cmd_id %d and size %d ",cmd_id,size);
		rc = TDS_EUNSUPPORTED;
		
	 
	 }  break;
	 
	} 
	 if ( rc != TDS_EOK ) 
	 {
		TDSL2_MSG1(MSG_LEGACY_ERROR,"An error %d occurred processing command  %d",cmd_id);
	 }

	return rc;
	 
}

/*==============================================================================================
 FUNCTION NAME  tdsl2_set_vfr_notify
 
 DEPENDENCIES:
  None.

RETURN VALUE:


SIDE EFFECTS:
None.
===============================================================================================*/
boolean tdsl2_set_vfr_notify 
(
  sys_modem_as_id_e_type as_id,
  boolean enable_flag
)
{
  tdsl2_ivoice.enable_flag = enable_flag;
  tdsl2_ivoice.as_id = tdsrrc_get_as_id();
  TDSL2_MSG3(MSG_LEGACY_HIGH,"received as_id %d, current as_id %d, enable_flag %d",as_id, tdsl2_ivoice.as_id, enable_flag);
  
  return TRUE;
}

/*==============================================================================================
 FUNCTION NAME  tdsl2_register_deregister_ivoice_cb
 
 DEPENDENCIES:
  None.

RETURN VALUE:


SIDE EFFECTS:
None.
===============================================================================================*/
boolean tdsl2_register_deregister_ivoice_cb
(
  tdscdma_icommon_event_callback_fn_t event_cb, 
  boolean reg_dereg
)
{
  boolean ret_value = FALSE;
  
  if((event_cb != NULL) || (reg_dereg == FALSE))
  {
    tdsl2_ivoice.ivoice_cb = event_cb;
    
    TDSL2_MSG2(MSG_LEGACY_HIGH,"Register/deregister %d voice callback 0x%x",reg_dereg,event_cb);
	ret_value = TRUE;
  }
  else
  {
    TDSL2_MSG2(MSG_LEGACY_ERROR,"Command passed with Invalid args. Register/deregister %d voice callback 0x%x",reg_dereg,event_cb);
	ERR_FATAL("Command passed with Invalid args",0,0,0);
    ret_value = FALSE;
  }

  return ret_value;
}


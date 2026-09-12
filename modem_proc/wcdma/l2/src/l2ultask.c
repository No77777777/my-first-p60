/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===

        L A Y E R  2   U P L I N K   TASK

GENERAL DESCRIPTION
  This file contains the Layer 2 Uplink task function.

Copyright (c) 2000 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2001-2009 by Qualcomm Technologies, Inc.  All Rights Reserved.
=============================================================================================*/
/*=============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/L2/vcs/l2ultask.c_v   1.18   10 Jul 2002 10:07:34   skrishna  $
$Header: //components/rel/wcdma.mpss/8.4.0/l2/src/l2ultask.c#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when      who     what, where, why
--------  ---     ---------------------------------------------------------------------- 
04/22/16  kc      Separting out SI timers for grant and no grant cases
04/07/16  scv     Fixed KW warnings.
03/28/16  kc      Adding loopback mode 4 changes
10/28/15  aa      Adding events and handler support to QSH
09/22/15  kv      Redesign 0x4322/23 logging to remove race condition in double buffer
08/18/15  as      Convert SUB_ID to AS_ID before getting TCB ptr for UL task
07/22/15  kv      FR 22377: RAVE-API
07/17/15  as      L2 to validate as_id during task entry after checking for FEATURE_SEGMENT_LOADING.
07/16/15  aa      L2 to pass/process sub-id correctly when DUAL_WCDMA feature is disabled.
06/26/15  aa      Removing extern declarations from L2 code
06/08/15  as      W+W MAC UL/DL HS task variable doubling.
06/03/15  svh     TA featurisation.
06/03/15  pr      W+W Phase8 Checkin.
06/01/15  scv     QSH: Changes to support SYNC_OPTIONAL for multiple metric config
06/01/15  scv     Code changes to support QSH
05/26/15  aa      RLC optimization to trigger Status PDUs on all AM channels after LTA
05/18/15  as      Added new NV item 73667 to control FR22545 changes.
05/20/15  aa      W+W changes for CFCM
04/25/15  scv     W+W Changes to have messages support SubID
04/08/15  as      L2-RRC interdependent changes for W+W
03/23/15  rsr     W + W Phase 6 checkin.
03/19/15  aa      RRC-L2 interface changes for passing AS ID as part of L2 commands
03/10/15  aa      Duplicated L2 task related globals and removed AS_ID hardcodings
02/09/15  aa      SUB_ID changes for L2-RRC interface
01/29/15  aa      L2 global reorg for W+W
09/04/14  scv     L2 changes for FEATURE_WCDMA_DIME_SW Cleanup
07/26/14  rkmk    Reduce image size by using right macro depending upon number of parameters
07/15/14  ac      NV reorganization
07/01/14  aa      CFCM changes for RLC
05/13/14  ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
03/19/14  kc      Changing FEATURE for heap corruption issue.
03/18/14  ts      Fix for heap memory corruption issue, to allocate and free heap memory fo rlog buffers in l2ultask itself
12/20/13  grk     Moved timer callback processing from timer task to L2 UL 
                  task for TM timer based SDU discard.
11/27/13  scv     Code changes for UL Compression feature 
11/26/13  geg     Segment loading FSL: add dummy task 
11/12/13  kv      Removed deprecated header tmc.h 
11/07/13  ts      DC-HSUPA: Changes for SI timer redesign by keeping a single base timer and reducing signals
05/22/13  kc      Added DC-HSUPA Changes
03/26/13  scv     L2 changes to support new dog architecture 
01/29/13  ksr     Disabled the registration with CPU based FC mechanism
01/04/13  kc      Modularization changes & duplicate/redundant code removal
08/22/12  kc      New log packet 0x4314 for HS RACH MAC timer info
07/13/12  grk     MBMS feature cleanup at L2(MBMS_DEBUG_1, FEATURE_MODEM_MBMS)
07/24/12  geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
03/28/12  zr      Adding feature definitions for Dime
06/18/12  mrg     L2 - MAC-I and HS-RACH code changes
04/12/12  grk     Fix to have separate PDU header and log buffers for EUL and R99.
03/06/12  mrg     RACH staus info from L1 to MAC indicaiton with signal based approch enhanced with double buffer.
03/08/12  amj     Added changes for RC init task initialization.
02/02/12  Ksr     Mutex lock and unlock sequences are updated in RLC to ensure it will not block ISR for a long time.
02/01/12  grk     Feture cleanup
12/06/11  ksr     Fix to handle back to back RACH ABORT and RACH NO ACK to MAC from L1
                  Moving to command based interface from signal based approach.
02/28/11  sv      Support for transition from TMC heap to Modem Heap in WCDMA.
04/26/11  vp      Free HS modem heap memory only of HSDPA feature is defined.
03/09/11  ssg     MAC-ehs support on Apollo.
12/07/10  pj      Handled new signal definition for enqueuing data into UL TM channel
12/07/10  mrg     Merge and Compilation errors are fixed.
11/24/10  mrg     MOB compilation error fixed.
11/11/10  mrg     MAC DL HS ZI memory optimization changes
11/18/11  ssg     Corrected the missed out featuring.
10/18/10  ssg     Added changes for new WCDMA-L2 interface.
10/01/10  mrg     TVM event and periodic measurements handled with different timers
09/17/10  grk     Moving Dynamic Dog API changes under FEATURE_WCDMA_L2_WDOG_DYNAMIC.
09/14/10  grk     Support to use Dynamic DOG APIs to register L2 tasks for dog monitoring.
07/09/10  grk     1. Changes to indicate UL ISR to send RESET_ACK only after 
                     finishing UL RESET procedure.
                  2. Introduced new command to send RESET_ACK for duplicate RESETs.
07/07/10  pj      Added changes to use a global base timer for status processing
04/12/10  grk     Fixed compilation error on silver MOB.
                  1. Removed test header inclusion in source files.
                  2. Stubbed L2 function calls.
03/30/10  grk     Changes for RLC QTF.
02/02/09  av      Moving DSM & DUP FC to UL task 
12/16/09  grk     Fixed KW warnings.
12/04/09  ssg     New L2 interface
11/24/08  pj      Added signal processing for resetting status prohibit timer
10/23/09  av      Temperature FC 
05/07/09  ssg     Updated Copyright Information
05/04/09  kvk     Added support to send the FC command for WINDOW size update 
                  rather than directly updating the WINDOW size from timer CB function
09/12/08  mrg     log_status() check is added to avoid unnecessary memory 
                  allocation and log packet preperation.
07/29/08  mrg     MAC-EUL log_packet data structures are taken out from the
                  l2_ul_cmd_data_type and maintained interanally to MAC.
03/17/08  ar      Moved UL flow control handling under FEATURE_UL_CPU_BASED_FC
02/01/08  pj      Changed FEATURE_MBMS flag to FEATURE_MODEM_MBMS.
13/12/07  vg      added handling for RLCI_UL_TX_AM_STATUS cmd
11/30/07  ssg     Added mbms support under FEATURE_MBMS.
09/18/07  vg      added CRLC_UL_CONTINUE_REQ to call ul_proc function
05/03/07  sk      Removed some duplicate MSG_HIGH's
03/30/07  sk      Remove unwanted F3's
02/10/07  sk      Logging support for the internal log packet - 0x4323
02/02/07  sk      Added MAC_HS_UL_STATUS_LOG_TMR_EXP_SIG and handling of the signal for log code 0x4322
02/01/07  ttl     Lint fix.
01/11/07  vg      added handling to call UL PDU logging on receiving
                  RLC_UL_START_LOGGING_SIG in TC
12/18/06  ttl     Compile warning fix.
11/23/06  ssg     Removed CRLC_UL_SUSPEND_REQ command.
08/28/06  ttl     Lint fix.
08/14/06  sk      Added HSUPA support
08/11/06  ttl     Support CPU based flow control feature.
07/03/06  ssg     Added handling for the command RLC_AM_UL_SDU_TX_CNF_SIG.
05/09/06  ttl     Lint Fix.
01/23/06  ttl     Give default values for NV item reading of RRC release support.
11/14/05  ttl     Remove define checking of NV_WCDMA_RRC_VERSION_I since it is an enum.
11/11/05  pdv     Fix for CR79364
11/01/05  aw      Added the functionality to read l2 release version from NV data and
                  a function to pass this data to mac and rlc.
07/20/05  mv      Fixed lint errors and warnings.
05/03/05  mv      Added the command CRLC_UL_ABORT_CIPHER_CONFIG_REQ to abort the new
                  ciphering configuration.
02/11/05  mv      Added the command RLC_UL_PURGE_WM_REQ to purge the UL WM for specified
                  logical channels.
07/16/04  sk      Added support for TFCC under FEATURE_TFCS_CONTROL_ENABLE
                  1. Added handling of CMAC_UL_TFC_SUBSET_CFG_REQ.
                  2. Added handling of MAC_UL_TFCS_CONTROL_TMR_EXP_SIG
01/07/04  ttl     Added CELL_PCH support.
07/29/03  ttl     Processed RLC_UL_RESET_REPORT_SIG.
05/05/03  mv      Removed  RLC_UL_POST_DL_RESET_SIG. Now RLC UL stores reset
                  request and reset done messages directly in the L2 cmd buf
02/21/03  ttl     Processing RLC_AM_RESET_REQUEST_SIG.
01/20/03  ttl     Processed RLCI_UL_TX_AM_STATUS UL RLC command.
08/27/02  sk      Removed mac_dl_config_cnf_sig. Now MAC DL posts config cnf directly tp L1
07/31/02  ttl     Removed RLC_UL_AM0_TIMER_EXP_SIG, RLC_UL_AM1_TIMER_EXP_SIG,
                  RLC_UL_AM2_TIMER_EXP_SIG and RLC_UL_AM3_TIMER_EXP_SIG.
                  Removed RLCI_UL_10MS_TIMER.
07/08/02  sk      Removed FEATURE_TRAFFIC_VOLUME_MEAS.
06/06/02  sk      Added handling of signals MAC_PHY_UL_CONFIG_IND and MAC_DL_CONFIG_CNF_SIG
                  under FEATURE_MAC_ACTIVATION_TIME
                  Added CMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ, CMAC_UL_BLOCK_RACH_TRAFFIC_REQ
                  to handle traffic volume related requests from RRC. Also added
                  MAC_UL_TRAFFIC_VOLUME_TMR_EXP_SIG to handle the expiry of the 10ms
                  timer signal for traffic volume internally maintained by MAC.
05/14/02  ttl     Naming changed of wcdma_l2_ul_task and wcdma_l2_ul_tcb.
05/13/02  ttl     Added RLC_UL_DEREGISTER_SRVC_REQ, RLC_OPEN_RX_WIN and
                  RLC_CLOSE_RX_WIN primitives.
03/15/02  sk      Added support to handle the expiry of qxdm log timer signal.
11/30/01  ttl     Added Processing of RLC_UL_POST_DL_RESET_SIG and RLC_UL_RESET_REPORT_SIG.
09/18/01  ttl     Added watchdog for L2 UL.
06/29/01  ttl     Added sign RLC_UL_AM0_TIMER_EXP_SIG, RLC_UL_AM1_TIMER_EXP_SIG,
                  RLC_UL_AM2_TIMER_EXP_SIG and RLC_UL_AM3_TIMER_EXP_SIG.
06/18/01  ps      Converted UL TX Status and Build Frame Indications from commands to signals
06/08/01  rc      Removed primitive RLCI_DL_RELEASE_ALL_CNF.
06/07/01  rc      Added primitives CRLC_RELEASE_ALL_REQ and RLCI_DL_RELEASE_ALL_CNF.
05/02/01  ttl     Added RLCI_UL_10MS_TIMER.
03/26/01  ttl     Added primitives CRLC_AM_RESET_REQ,
                  RLC_REGISTER_AM_SDU_CONFIRM_CALLBACK_REQ,
                  RLCI_DL_RESET_DONE and RLCI_UL_RESET_REQ.
03/01/01  ps      Modified to support AM RLC
02/17/01  ps      Fixed bugs found during unit testing
01/05/01  ps      Added MAC_UL_TX_STATUS_IND
11/22/00  ps      Initial release

=============================================================================================*/

/*=============================================================================================

                      I N C L U D E    F I L E S

=============================================================================================*/
#include "wcdma_variation.h"
#include  "rex.h"
#include  "task.h"

#ifdef FEATURE_DOG
  #include  "dog.h"
#endif
#include  "msg.h"
#include  "err.h"
#include  "l2ultask.h"
#include  "macul.h"
#include  "maculsubtask.h"
#include  "rlculsubtask.h"
#include  "rlcul.h"
#include  "rlcdl.h"
#include  "rlculam.h"
#include  "rlcfc.h"
#include  "maculsubtask.h"
#include  "macdlhs.h"

#ifdef FEATURE_WCDMA_HSUPA
#include  "maculhssubtask.h"
#endif

#ifdef FEATURE_SEGMENT_LOADING
#include "mcfg_seg_load.h"
#endif /* FEATURE_SEGMENT_LOADING */

/*=============================================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

=============================================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define WCDMA_L2_UL_TASK_NAME(as_id) (as_id == SYS_MODEM_AS_ID_1) ? "wl2_ul_0" : "wl2_ul_1"

/* Uplink layer 2 command queue */
q_type        l2_ul_cmd_q[L2_NUM_W_SUBS];

#ifdef FEATURE_WCDMA_NEW_L2_INTERFACE
q_type        wcdma_l2_ul_cmd_q[L2_NUM_W_SUBS];
#endif

#ifdef FEATURE_DOG
#ifdef FEATURE_WCDMA_L2_WDOG_DYNAMIC   /* Dynamic Dog Interface */
  #undef  DOG_WCDMA_L2UL_RPT
  #define DOG_WCDMA_L2UL_RPT wcdma_l2ul_dog_rpt_var
  dog_report_type   wcdma_l2ul_dog_rpt_var[L2_NUM_W_SUBS] = {0};
#endif   /* FEATURE_WCDMA_L2_WDOG_DYNAMIC  */

/* L2 UL watchdog report timer block  */
rex_timer_type l2_ul_dog_rpt_timer_blk[L2_NUM_W_SUBS];
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

wl2_nv_data_type_s wl2_nv_list[L2_NUM_W_SUBS];

/* WCDMA L2 DL TCB handle */
static rex_tcb_type* wcdma_l2_ul_tcb_ptr[L2_NUM_W_SUBS];

#ifdef FEATURE_SEGMENT_LOADING
extern void wcdma_task_null (void);
#endif /* FEATURE_SEGMENT_LOADING */

/*=============================================================================================

FUNCTION  l2_ul_wait

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

uint32  l2_ul_wait
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

} /* l2_ul_wait */

/*=============================================================================================

FUNCTION  proc_l2_ul_cmd_queue

DESCRIPTION
  This function processes the uplink layer 2 command queue data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/

void  proc_l2_ul_cmd_queue (sys_modem_as_id_e_type as_id)
{
  l2_ul_cmd_enum_type   cmd_id;
  l2_ul_cmd_type      *cmd_ptr;
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);

  while ((cmd_ptr = q_get(&l2_ul_cmd_q[as_id])) != NULL)
  {
    /*Validate as_id in dequeued command*/
    if (cmd_ptr->cmd_hdr.as_id != as_id)
    {
      WL2_MSG_2(ERROR, rlc_ul_table_ptr, "AS_ID field in dequeued cmd %d doesn't match with task AS_ID %d",
      	                                  cmd_ptr->cmd_hdr.as_id, as_id);
      return;
    }

    /* Get the command ID */
    cmd_id = cmd_ptr->cmd_hdr.cmd_id;

    /* Process the command queue data based on the command ID */
    switch (cmd_id)
    {
    case RLC_UL_REGISTER_SRVC_REQ:
    case RLC_UL_DEREGISTER_SRVC_REQ:
      proc_ul_rlc_commands (rlc_ul_table_ptr, cmd_id, &cmd_ptr->cmd_data);
      break;

      /* Invalid command */
    default:
      ERR ("Invalid command:%d", cmd_ptr->cmd_hdr.cmd_id, 0,0  );
      break;
    }/* switch cmd-id */

    /* Return the command buffer to free queue */
#ifdef FEATURE_WCDMA_USE_MODEM_HEAP
    modem_mem_free(cmd_ptr, MODEM_MEM_CLIENT_WCDMA_EXT_CMD_CRIT);
#else
    /*lint -e424 */
    mem_free (&tmc_heap, cmd_ptr);
    /*lint +e424 */
#endif /* FEATURE_WCDMA_USE_MODEM_HEAP */

  }/* while (cmd_ptr != NULL) */

} /* proc_l2_ul_cmd_queue */

#ifdef FEATURE_WCDMA_NEW_L2_INTERFACE
/*=============================================================================================

FUNCTION  proc_wcdma_l2_ul_cmd_queue

DESCRIPTION
  This function processes the uplink layer 2 command queue data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/

void  proc_wcdma_l2_ul_cmd_queue (sys_modem_as_id_e_type as_id)
{
  wcdma_l2_ul_cmd_enum_type   cmd_id;
  wcdma_l2_ul_cmd_type      *cmd_ptr;
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_downlink_table_type *rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);
  mac_ul_table_type *ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);
  mac_eul_table_type *eul_table_ptr = GET_MAC_EUL_TABLE_PTR(as_id);
  
  while ((cmd_ptr = q_get(&wcdma_l2_ul_cmd_q[as_id])) != NULL)
  {
    /*Validate as_id in dequeued command*/
    if (cmd_ptr->cmd_hdr.as_id != as_id)
    {
      WL2_MSG_2(ERROR, rlc_ul_table_ptr, "AS_ID field in dequeued cmd %d doesn't match with task AS_ID %d",
      	                                  cmd_ptr->cmd_hdr.as_id, as_id);
      return;
    }

    /* Get the command ID */
    cmd_id = cmd_ptr->cmd_hdr.cmd_id;

    /* Process the command queue data based on the command ID */
    switch (cmd_id)
    {

    /* UL MAC Commands */
    case CMAC_UL_CONFIG_REQ:

      /* Get the configuration data from RRC for traffic volume measurements */
    case CMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ:

      /* RRC can ask MAC to stop transmissions on RACH with this command */
    case CMAC_UL_BLOCK_RACH_TRAFFIC_REQ:

      /* RRC sends the TFCC config information */
    case CMAC_UL_TFC_SUBSET_CFG_REQ:

      proc_ul_mac_commands (cmd_id, &cmd_ptr->cmd_data, ul_table_ptr, eul_table_ptr);
      break;

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
     case CMAC_QSH_PROCESS_METRIC_COMMAND_REQ:
       wmac_qsh_proc_metric_command(&cmd_ptr->cmd_data.wmac_rlc_qsh_cfg, ul_table_ptr->as_id);
     break;
#endif

#ifdef FEATURE_MAC_HS_UL_QXDM_LOGGING
    case CMAC_HS_UL_PROCESS_HDR_LOG_PKT_REQ:
     
       if (log_status(WCDMA_MAC_HS_UL_HEADER_STATUS_LOG_PACKET))
       {
         mac_hs_ul_process_header_log_packet(eul_table_ptr);
       }
       else
       {
         WL2_MSG_0(LOW, rlc_ul_table_ptr, "MAC_UL_HS Header Status log packet is not configured ");
       }
       
      break;

    case CMAC_HS_UL_PROCESS_ETFCI_STATUS_LOG_PKT_REQ:
     
      if (log_status(WCDMA_MAC_HS_UL_ETFCI_STATUS_LOG_PACKET))
      {
        mac_hs_ul_process_etfci_status_log_packet(eul_table_ptr);
      }
      else
      {
        WL2_MSG_0(LOW, rlc_ul_table_ptr, "MAC_UL_HS Etfci Status log packet is not configured ");
      }

      break;

#ifdef  FEATURE_WCDMA_HS_RACH

    case CMAC_HS_UL_PROCESS_MAC_HSRACH_TIMERS_LOG_PKT_REQ:

      if(log_status(WCDMA_HS_RACH_MAC_TIMERS_LOG_PACKET))
      {
        mac_hs_ul_process_hsrach_timers_log_pkt(eul_table_ptr);
      }
      else
      {
        WL2_MSG_0(HIGH, rlc_ul_table_ptr, "MAC_HSRACH_LOG: Timer log packet not configured");
      }
      
    break;      
#endif /* FEATURE_WCDMA_HS_RACH */

#ifdef FEATURE_MAC_I
      case CMAC_HS_UL_PROCESS_MAC_I_IS_LOG_PKT_REQ:
       
        if (log_status(WCDMA_MAC_HS_UL_MAC_I_LOG_PACKET))
        {
          mac_hs_ul_process_maci_is_log_packet(eul_table_ptr);
        }
        else
        {
          WL2_MSG_0(HIGH, rlc_ul_table_ptr, "MAC_UL_MAC_I_IS log packet is not configured "); 
        }
        
        break;
#endif /*  FEATURE_MAC_I  */

#endif

      /* UL RLC Commands */
    case CRLC_UL_TM_CONFIG_REQ:
    case CRLC_UL_UM_CONFIG_REQ:
    case CRLC_AM_CONFIG_REQ:
    case CRLC_AM_RESET_REQ:
    case CRLC_UL_CIPHER_CONFIG_REQ:
    case CRLC_UL_RESUME_REQ:
    case CRLC_UL_HFN_REQ:
    case RLC_REGISTER_AM_SDU_CONFIRM_CALLBACK_REQ:
    case RLC_OPEN_RX_WIN:
    case RLC_CLOSE_RX_WIN:
    case RLCI_DL_CONFIG_CNF:          /* DL confirm config completion */
    case RLCI_DL_RESET_DONE:
    case RLCI_UL_RESET_REQ:
    case RLCI_UL_DUP_RESET_REQ:
    case CRLC_RELEASE_ALL_REQ:
    case CRLC_UL_REG_FOR_DATA_IND_REQ:
    case CRLC_UL_UNREG_FOR_DATA_IND_REQ:
    case RLC_UL_PURGE_WM_REQ:         /*Command to purge the UL WM    */

    case CRLC_UL_ABORT_CIPHER_CONFIG_REQ: /* Command to abort the new UL
                                             ciphering configuration */
    case CRLC_UL_CONTINUE_REQ:
    case RLCI_UL_TX_AM_STATUS:      /* command to indicate UL to send status PDU */
    case RLC_HANDLE_TM_TIMER_BASED_SDU_DISCARD_CB_IND:
    case RLC_UL_TA_IND:
    case CRLC_QSH_PROCESS_COMMAND_REQ:
      proc_wcdma_ul_rlc_commands (rlc_ul_table_ptr, rlc_dl_table_ptr, cmd_id, &cmd_ptr->cmd_data);

      break;

      /* Invalid command */
    default:
      ERR ("Invalid command:%d", cmd_ptr->cmd_hdr.cmd_id, 0,0  );
      break;
    }/* switch cmd-id */

    /* Return the command buffer to free queue */
#ifdef FEATURE_WCDMA_USE_MODEM_HEAP
    modem_mem_free(cmd_ptr, MODEM_MEM_CLIENT_WCDMA_EXT_CMD_CRIT);
#else
    /*lint -e424 */
    mem_free (&tmc_heap, cmd_ptr);
    /*lint +e424 */
#endif /* FEATURE_WCDMA_USE_MODEM_HEAP */

  }/* while (cmd_ptr != NULL) */

} /* proc_wcdma_l2_ul_cmd_queue */
#endif

/*=============================================================================================

FUNCTION  wcdma_l2_ul_task_init

DESCRIPTION
  This task initializes uplink layer 2 queues.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/
#ifdef FEATURE_SEGMENT_LOADING
void  wcdma_l2_ul_task_init ( sys_modem_as_id_e_type as_id ) __attribute__((section(".task_text")));
void  wcdma_l2_ul_0_task_init ( void ) __attribute__((section(".task_text")));
void  wcdma_l2_ul_1_task_init ( void ) __attribute__((section(".task_text")));
#else
void  wcdma_l2_ul_task_init ( sys_modem_as_id_e_type as_id );
#endif /* FEATURE_SEGMENT_LOADING */

void  wcdma_l2_ul_0_task_init ( void )
{
  wcdma_l2_ul_task_init(SYS_MODEM_AS_ID_1);
}
void  wcdma_l2_ul_1_task_init ( void )
{
#ifdef FEATURE_DUAL_WCDMA
  wcdma_l2_ul_task_init(SYS_MODEM_AS_ID_2);
#endif
}

void  wcdma_l2_ul_task_init ( sys_modem_as_id_e_type as_id )
{
#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_wcdma_segment = NULL;

  pi_wcdma_segment = get_wcdma_interface();
  if(pi_wcdma_segment == NULL)
  {
    return;
  }
#endif /* FEATURE_SEGMENT_LOADING */

  /* Initialize uplink layer 2 task command queue */
  (void)q_init(&l2_ul_cmd_q[as_id]);
  #ifdef FEATURE_WCDMA_NEW_L2_INTERFACE
  (void)q_init(&wcdma_l2_ul_cmd_q[as_id]);
  #endif
}

/*=============================================================================================

FUNCTION  wcdma_l2_ul_task

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
void wcdma_l2_ul_task(sys_modem_as_id_e_type as_id) __attribute__((section(".task_text")));
void wcdma_l2_ul_0_task(dword dummy) __attribute__((section(".task_text")));
void wcdma_l2_ul_1_task(dword dummy) __attribute__((section(".task_text")));
#else
void wcdma_l2_ul_task(sys_modem_as_id_e_type as_id);
#endif /* FEATURE_SEGMENT_LOADING */

void wcdma_l2_ul_0_task(dword dummy)
{
  wcdma_l2_ul_task(SYS_MODEM_AS_ID_1);
}
void wcdma_l2_ul_1_task(dword dummy)
{
#ifdef FEATURE_DUAL_WCDMA
  wcdma_l2_ul_task(SYS_MODEM_AS_ID_2);
#endif
}

void  wcdma_l2_ul_task
(
  sys_modem_as_id_e_type as_id
)
{
  uint32        sigs;
  mac_ul_table_type *ul_table_ptr;
  mac_eul_table_type *eul_table_ptr;
  rlci_uplink_table_type *rlc_ul_table_ptr;
  rlci_downlink_table_type *rlc_dl_table_ptr;

#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_wcdma_segment = NULL;

  pi_wcdma_segment = get_wcdma_interface();
  if(pi_wcdma_segment == NULL)
  {
    wcdma_task_null ();
    return;
  }
#endif /* FEATURE_SEGMENT_LOADING */

  l2_validate_as_id(as_id);

  eul_table_ptr = GET_MAC_EUL_TABLE_PTR(as_id);
  ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);
  rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);
  
  wcdma_l2_ul_tcb_ptr[as_id] = RCINIT_NULL;
#ifdef FEATURE_MODEM_RCINIT 
  /* Handshake with the RC Init */
  rcinit_handshake_startup();
#else
  wcdma_l2_ul_task_init(as_id);
  /* Waits for start signal from TMC */
  /* Sets ACK signal when receives the task start signal */
  tmc_task_start();
#endif

#ifdef FEATURE_DOG
#ifndef FEATURE_WCDMA_L2_WDOG_DYNAMIC
  /* Setup watchdog timer */

  rex_def_timer(&l2_ul_dog_rpt_timer_blk[as_id], wcdma_l2_ul_get_tcb(as_id), L2_UL_DOG_RPT_TIMER_SIG);
  (void)rex_set_timer(&l2_ul_dog_rpt_timer_blk[as_id], DOG_L2UL_RPT_TIME);
#else
  /*Register with DOG, get the Task ID*/

  wcdma_l2ul_dog_rpt_var[as_id] = dog_hb_register_rex((rex_sigs_type)L2_UL_DOG_RPT_TIMER_SIG);

#endif
#endif

  wcdma_l2_read_nv(as_id);

  rlc_ul_init(as_id);

  /* Initialize UL MAC */
  init_ul_mac(as_id);

  /* Initialize RLC-UL flow control */
  rlc_ul_fc_init(as_id);

  /* Task loop */
  /*lint -e716 */
  while (1)
  /*lint +e716 */
  {

    /* Wait for UL L2 signal to be set */
    sigs = l2_ul_wait ( L2_UL_CMD_Q_SIG   |
      CMAC_UL_HFN_REQ_SIG         |
      MAC_UL_TX_STATUS_IND_SIG    |
#ifdef FEATURE_DOG
      L2_UL_DOG_RPT_TIMER_SIG     |
#endif

#ifdef FEATURE_RLC_QXDM_LOGGING
      RLC_UL_AM_LOG_TIMER_EXP_SIG |
#endif


    /*signal set when the 10ms base traffic volume timer expires*/
      MAC_UL_TVM_EVENT_TMR_EXP_SIG |
      MAC_UL_TVM_PERIODIC_TMR_EXP_SIG |
      MAC_PHY_UL_CONFIG_IND       |

      MAC_UL_TFCS_CONTROL_TMR_EXP_SIG |
      RLC_AM_RESET_REQUEST_SIG    |
      RLC_UL_RESET_REPORT_SIG     |
      RLC_AM_UL_SDU_TX_CNF_SIG    |
#ifdef FEATURE_WCDMA_HSUPA    /* Handle timers related to HSUPA */
      MAC_HS_UL_SI_NO_GRANT_TMR_EXP_SIG |
      MAC_HS_UL_SI_GRANT_TMR_EXP_SIG |
#endif
#ifdef FEATURE_WCDMA_DC_HSUPA    /* Handle timers related to HSUPA */
      MAC_HS_UL_SECONDARY_SI_NO_GRANT_TMR_EXPIRED_SIG |
      MAC_HS_UL_SECONDARY_SI_GRANT_TMR_EXPIRED_SIG |
#endif

#ifdef FEATURE_DATA_RAVE_SUPPORT 
      MAC_HS_UL_RAVE_TMR_EXPIRED_SIG         |
#endif

      RLC_AM_UL_RESET_DONE_SIG |
      RLC_UL_FC_SIG|
      RLC_UL_FC_TIMER_SIG

#ifdef FEATURE_RLC_QXDM_LOGGING
      |
      RLC_UL_START_LOGGING_SIG
#endif

#ifdef FEATURE_WCDMA_UL_COMPR
      |
      RLC_UL_COMP_DSM_ALLOC
#endif

      | RLC_UL_AM_BASE_TIMER_SIG
#ifdef FEATURE_WCDMA_NEW_L2_INTERFACE
      | WCDMA_L2_UL_CMD_Q_SIG
#endif
      );

#ifdef FEATURE_DOG
    /* L2 UL watchdog report timer sig  */
    if (sigs & L2_UL_DOG_RPT_TIMER_SIG)
    {
      /* Clear UL watchdog report timer sig */
      (void)rex_clr_sigs (rex_self(), L2_UL_DOG_RPT_TIMER_SIG);
#ifndef FEATURE_WCDMA_L2_WDOG_DYNAMIC
      /* Kick watchdog */
      dog_report(DOG_L2UL_RPT);
      //WL2_MSG_0(HIGH, ul_table_ptr, "Kick watchdog l2ul");
      /* Set watchdog report timer for next report  */
      (void)rex_set_timer(&l2_ul_dog_rpt_timer_blk[as_id], DOG_L2UL_RPT_TIME);
#else
      dog_hb_report(wcdma_l2ul_dog_rpt_var[as_id]);
#endif
    }
#endif

    /* Uplink L2 Command Queue Signal is set */
    if (sigs & L2_UL_CMD_Q_SIG)
    {
      /* Clear uplink Layer 2 Command Queue Signal */
      (void)rex_clr_sigs (rex_self(), L2_UL_CMD_Q_SIG);

      /* Process uplink layer 2 command queue */
      proc_l2_ul_cmd_queue (as_id);

    }

#ifdef FEATURE_WCDMA_UL_COMPR
    /* Signal is set to allocate DSMs for UL Compression */
    if (sigs & RLC_UL_COMP_DSM_ALLOC)
    {
      /* Clear RLC_UL_COMP_DSM_ALLOC Signal */
      (void)rex_clr_sigs (rex_self(), RLC_UL_COMP_DSM_ALLOC);

      /* To allocate DSM items for compression */
      rlc_ul_am_comp_dsm_alloc(rlc_ul_table_ptr);
    }
#endif

#ifdef FEATURE_WCDMA_NEW_L2_INTERFACE
    /* Uplink L2 Command Queue Signal is set */
    if (sigs & WCDMA_L2_UL_CMD_Q_SIG)
    {
      /* Clear uplink Layer 2 Command Queue Signal */
      (void)rex_clr_sigs (rex_self(), WCDMA_L2_UL_CMD_Q_SIG);

      /* Process uplink layer 2 command queue */
      proc_wcdma_l2_ul_cmd_queue (as_id);
    }
#endif

    /* UL MAC HFN Request Signal is set */
    if (sigs & CMAC_UL_HFN_REQ_SIG)
    {
      /* Clear uplink MAC HFN Request Signal */
      (void)rex_clr_sigs (rex_self(), CMAC_UL_HFN_REQ_SIG);
    }

    /* UL Transmit Status Indication */
    if (sigs & MAC_UL_TX_STATUS_IND_SIG)
    {
      /* Clear UL Transmit Status Indication Signal */
      (void)rex_clr_sigs (rex_self(), MAC_UL_TX_STATUS_IND_SIG);

      /* Process RACH transmit status */
      proc_ul_tx_status_ind (ul_table_ptr, eul_table_ptr);
    }

    if (sigs & RLC_AM_RESET_REQUEST_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), RLC_AM_RESET_REQUEST_SIG);

      /* Post RLC RESET message */
      rlci_am_reset(rlc_ul_table_ptr, rlc_dl_table_ptr);
    }

    if (sigs & RLC_UL_RESET_REPORT_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), RLC_UL_RESET_REPORT_SIG);

      /* Post RLC reset to RRC */
      rlci_post_reset_to_rrc(rlc_ul_table_ptr);
    }

    if (sigs & RLC_AM_UL_RESET_DONE_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), RLC_AM_UL_RESET_DONE_SIG);

      /* Post RLC UL RESET Done to RLC DL */
      rlci_post_rlc_ul_reset_done_cmd(rlc_ul_table_ptr);
    }

#ifdef FEATURE_RLC_QXDM_LOGGING

    if (sigs & RLC_UL_AM_LOG_TIMER_EXP_SIG)
    {
      /* Clear UL AM0 10ms Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), RLC_UL_AM_LOG_TIMER_EXP_SIG);

      /* Call AM log timer expiration handler  */
      rlci_ul_am_handle_qxdm_log_timer(rlc_ul_table_ptr, RLC_UL_AM_LOG_TIMER_EXP_SIG);
    }

#endif

    if (sigs & RLC_AM_UL_SDU_TX_CNF_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), RLC_AM_UL_SDU_TX_CNF_SIG);

      /* Post RLC UL SDU tx cnformation */
      rlci_post_sdu_tx_cnf(rlc_ul_table_ptr);
    }

    if (sigs & MAC_UL_TVM_EVENT_TMR_EXP_SIG)
    {
      /* Clear UL Traffic volume 10ms Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), MAC_UL_TVM_EVENT_TMR_EXP_SIG);

      /* Handle the expiry of the 10ms timer signal */
      mac_ul_handle_tvm_event_tmr(ul_table_ptr, eul_table_ptr);
    }

    if (sigs & MAC_UL_TVM_PERIODIC_TMR_EXP_SIG)
    {
      /* Clear UL Traffic volume (10ms * MAC_UL_PERIODIC_TVM_TIMER_SCALE) Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), MAC_UL_TVM_PERIODIC_TMR_EXP_SIG);

      /* Handle the expiry of the 10ms timer signal */
      mac_ul_handle_tvm_periodic_tmr(ul_table_ptr, eul_table_ptr);
    }

    /* This signal is posted by PHYSICAL layer. It indicates to MAC that
    it should using the new configuration parameters already sent by RRC */
    if (sigs & MAC_PHY_UL_CONFIG_IND)
    {
      /* Clear UL config signal  */
      (void)rex_clr_sigs (rex_self(), MAC_PHY_UL_CONFIG_IND);

      /* Process the signal received from L1. */
      proc_ul_mac_config_signal(ul_table_ptr, eul_table_ptr);
    }

    if (sigs & MAC_UL_TFCS_CONTROL_TMR_EXP_SIG)
    {
      /* Clear UL Traffic volume 10ms Timer expiration signal  */
      (void)rex_clr_sigs (rex_self(), MAC_UL_TFCS_CONTROL_TMR_EXP_SIG);
      
      WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Control timer expired for TFCC msg");
      /* Handle the expiry of the 10ms timer signal */
      mac_ul_handle_tfcs_control_timer_expiry(ul_table_ptr);
    }

    if(sigs & RLC_UL_FC_SIG)
    {
      (void)rex_clr_sigs(rex_self(), RLC_UL_FC_SIG);
      rlc_fc_cmd_hdlr(rlc_ul_table_ptr, rlc_dl_table_ptr, RLC_FC_UPLINK);
    }

    if(sigs & RLC_UL_FC_TIMER_SIG)
    {
      (void)rex_clr_sigs(rex_self(), RLC_UL_FC_TIMER_SIG);
      rlc_fc_timer_cmd_hdlr(rlc_ul_table_ptr, rlc_dl_table_ptr, RLC_FC_UPLINK);
    }

    #ifdef FEATURE_WCDMA_HSUPA
    if (sigs & MAC_HS_UL_SI_NO_GRANT_TMR_EXP_SIG)
    {
      /* Clear HS UL timer for SI when NO grant is available */
      (void)rex_clr_sigs (rex_self(), MAC_HS_UL_SI_NO_GRANT_TMR_EXP_SIG);

      /* Handle the expiry of the timer signal */
      mac_hs_ul_handle_si_no_grant_tmr_expiry(MAC_HS_UL_SI_TMR_EXPIRED, eul_table_ptr);

    }
    if (sigs & MAC_HS_UL_SI_GRANT_TMR_EXP_SIG)
    {
      /* Clear HS UL timer for SI when grant is available */
      (void)rex_clr_sigs (rex_self(), MAC_HS_UL_SI_GRANT_TMR_EXP_SIG);

      /* Handle the expiry of the timer signal */
      mac_hs_ul_handle_si_grant_tmr_expiry(MAC_HS_UL_SI_TMR_EXPIRED, eul_table_ptr);

    }

#endif // feature_wcdma_hsupa

#ifdef FEATURE_WCDMA_DC_HSUPA

if (sigs & MAC_HS_UL_SECONDARY_SI_GRANT_TMR_EXPIRED_SIG)
{
  /* Clear HS UL timer for SI when grant is available */
  (void)rex_clr_sigs (rex_self(), MAC_HS_UL_SECONDARY_SI_GRANT_TMR_EXPIRED_SIG);

  /* Handle the expiry of the timer signal */
  mac_hs_ul_handle_si_grant_tmr_expiry(MAC_HS_UL_SECONDARY_SI_TMR_EXPIRED, eul_table_ptr);
}

if (sigs & MAC_HS_UL_SECONDARY_SI_NO_GRANT_TMR_EXPIRED_SIG)
{
  /* Clear HS UL timer for SI when NO grant is available */
  (void)rex_clr_sigs (rex_self(), MAC_HS_UL_SECONDARY_SI_NO_GRANT_TMR_EXPIRED_SIG);

  /* Handle the expiry of the timer signal */
  mac_hs_ul_handle_si_no_grant_tmr_expiry(MAC_HS_UL_SECONDARY_SI_TMR_EXPIRED, eul_table_ptr);
}

#endif

#ifdef FEATURE_DATA_RAVE_SUPPORT 
if (sigs & MAC_HS_UL_RAVE_TMR_EXPIRED_SIG)
{  /* Clear RAVE timer expiry signal */
  (void)rex_clr_sigs(rex_self(), MAC_HS_UL_RAVE_TMR_EXPIRED_SIG);
  /* Handle the expiry of timer */
  mac_ul_handle_rave_timer_expiry(eul_table_ptr, ul_table_ptr);
}
#endif

#ifdef FEATURE_RLC_QXDM_LOGGING
    if(sigs & RLC_UL_START_LOGGING_SIG)
    {
      (void)rex_clr_sigs(rex_self(), RLC_UL_START_LOGGING_SIG);
      rlc_start_ul_logging(rlc_ul_table_ptr);
      rlc_start_eul_logging(rlc_ul_table_ptr);
    }
#endif

    if (sigs & RLC_UL_AM_BASE_TIMER_SIG)
    {
      /* Clear REX sig  */
      (void)rex_clr_sigs (rex_self(), RLC_UL_AM_BASE_TIMER_SIG);

      rlci_ul_am_base_timer_handler(rlc_ul_table_ptr, ul_table_ptr);
    }
  } /* while loop */

#ifdef FEATURE_WCDMA_HSUPA
  #ifdef FEATURE_MAC_HS_UL_QXDM_LOGGING
  mac_eul_deallocate_mem_log_records(eul_table_ptr);
  #endif
#endif

#ifdef FEATURE_DOG
#ifdef FEATURE_WCDMA_L2_WDOG_DYNAMIC
  dog_hb_deregister(wcdma_l2ul_dog_rpt_var[as_id]);
#endif
#endif

}/* wcdma_l2_ul_task */

/*===========================================================================

FUNCTION     L2_FREE_CMD_BUF

DESCRIPTION  Releases the command to HEAP or CRIT section, which is allocated
             earlier in Task context using get_cmd_buf
===========================================================================*/
void l2_free_cmd_buf(void *cmd_buf_ptr, rlci_downlink_table_type *rlc_dl_table_ptr)
{
  WL2_MSG_0(HIGH, rlc_dl_table_ptr, "Releasing L2 Cmd in function");
#ifdef FEATURE_WCDMA_USE_MODEM_HEAP
  modem_mem_free(cmd_buf_ptr, MODEM_MEM_CLIENT_WCDMA_EXT_CMD_CRIT);
#else
  /*lint -e424 */
  mem_free (&tmc_heap, cmd_buf_ptr);
  /*lint +e424 */
#endif /* FEATURE_WCDMA_USE_MODEM_HEAP */
}

/*=============================================================================================

FUNCTION  l2_ul_get_cmd_buf

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
l2_ul_cmd_type    *l2_ul_get_cmd_buf
  (
  void
  )
{
  l2_ul_cmd_type * return_ptr =  NULL;
  uint8 i;
  size_t cmd_size = sizeof(l2_ul_cmd_type);

#ifdef FEATURE_WCDMA_USE_MODEM_HEAP
  return_ptr = modem_mem_calloc(1,cmd_size, MODEM_MEM_CLIENT_WCDMA_EXT_CMD_CRIT);
#else
  return_ptr = mem_malloc (&tmc_heap, cmd_size);
#endif

  if(return_ptr == NULL)
  {
#ifdef FEATURE_WCDMA_USE_MODEM_HEAP
    /* As we are not clear on sub id here, we will use the default id, but this shouldnt impact debugging as it is not a functional issue */
    WRLC_ERR_FATAL(L2_SUB_1, "Failed to allocate from Modem Mem Heap: size:0x%x",cmd_size,0,0);
#else
    WRLC_ERR_FATAL(L2_SUB_1, "Failed to allocate from TMC Heap: size:0x%x",cmd_size,0,0);
#endif
  }
  else
  {
    return_ptr->cmd_hdr.as_id = L2_SUB_1;
    for(i=0; i<MAX_UL_LOGICAL_CHANNEL; i++)
    {
      return_ptr->cmd_data.ul_reg.rlc_data[i].wm_reg_ind_cb = NULL;
      return_ptr->cmd_data.ul_reg.rlc_data[i].bearer_context = NULL;
      return_ptr->cmd_data.ul_dereg.cb_info[i].wm_reg_ind_cb = NULL;
      return_ptr->cmd_data.ul_dereg.cb_info[i].bearer_context = NULL;
    }
  }

  return (return_ptr);

} /* l2_ul_get_cmd_buf */

/*=============================================================================================

FUNCTION  wcdma_l2_ul_get_cmd_buf

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
wcdma_l2_ul_cmd_type    *wcdma_l2_ul_get_cmd_buf
  (
  void
  )
{
  wcdma_l2_ul_cmd_type * return_ptr =  NULL;
  size_t cmd_size = sizeof(wcdma_l2_ul_cmd_type);

#ifdef FEATURE_WCDMA_USE_MODEM_HEAP
  return_ptr = modem_mem_calloc(1,cmd_size, MODEM_MEM_CLIENT_WCDMA_EXT_CMD_CRIT);
#else
  return_ptr = mem_malloc (&tmc_heap, cmd_size);
#endif

  if(return_ptr == NULL)
  {
#ifdef FEATURE_WCDMA_USE_MODEM_HEAP
    WRLC_ERR_FATAL(L2_SUB_1, "Failed to allocate from Modem Mem Heap: size:0x%x",cmd_size,0,0);
#else
    WRLC_ERR_FATAL(L2_SUB_1, "Failed to allocate from TMC Heap: size:0x%x",cmd_size,0,0);
#endif
  }
  else
  {
    return_ptr->cmd_hdr.as_id = L2_SUB_1;
  }
  return (return_ptr);

} /* wcdma_l2_ul_get_cmd_buf */

/*=============================================================================================

FUNCTION  l2_ul_put_cmd

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

void  l2_ul_put_cmd
  (
  l2_ul_cmd_type    *cmd_ptr
/* pointer to command to be enqueued */
  )
{
  sys_modem_as_id_e_type sub_id = cmd_ptr->cmd_hdr.as_id;
  sys_modem_as_id_e_type as_id = wcdma_convert_subid_to_stack_index(sub_id);
  l2_validate_as_id(as_id);
  cmd_ptr->cmd_hdr.as_id = as_id;
  
#ifdef FEATURE_WCDMA_NEW_L2_INTERFACE
  if ((cmd_ptr->cmd_hdr.cmd_id >= RLC_UL_REGISTER_SRVC_REQ) &&
     ((cmd_ptr->cmd_hdr.cmd_id <= RLC_UL_DEREGISTER_SRVC_REQ)))
  {
#endif
  /* initialize link field */
  (void)q_link(cmd_ptr, &cmd_ptr->cmd_hdr.link );

  /* put item on queue for wcdma_l2_ul_task */
  q_put( &l2_ul_cmd_q[as_id], &cmd_ptr->cmd_hdr.link );

  /* Set signal for the l2_ul_cmd_q */
  (void)rex_set_sigs(wcdma_l2_ul_get_tcb(as_id), L2_UL_CMD_Q_SIG );
#ifdef FEATURE_WCDMA_NEW_L2_INTERFACE
  }
  else
  {
     WRLC_ERR_FATAL(as_id, "Invalid L2 UL command: %d ",cmd_ptr->cmd_hdr.cmd_id,0,0);    
  }
#endif

} /* l2_ul_put_cmd */

/*=============================================================================================

FUNCTION  wcdma_l2_ul_put_cmd

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

void  wcdma_l2_ul_put_cmd
  (

  wcdma_l2_ul_cmd_type    *cmd_ptr
/* pointer to command to be enqueued */
  )
{
  sys_modem_as_id_e_type sub_id = cmd_ptr->cmd_hdr.as_id;
  sys_modem_as_id_e_type as_id = wcdma_convert_subid_to_stack_index(sub_id);
  l2_validate_as_id(as_id);
  cmd_ptr->cmd_hdr.as_id = as_id;

  /* initialize link field */
  (void)q_link(cmd_ptr, &cmd_ptr->cmd_hdr.link );

  /* put item on queue for wcdma_l2_ul_task */
  q_put( &wcdma_l2_ul_cmd_q[as_id], &cmd_ptr->cmd_hdr.link );

  /* Set signal for the l2_ul_cmd_q */
  (void)rex_set_sigs(wcdma_l2_ul_get_tcb(as_id), WCDMA_L2_UL_CMD_Q_SIG );
} /* wcdma_l2_ul_put_cmd */

/*===========================================================================

FUNCTION l2_get_release_flag_indicator

DESCRIPTION
  This  function returns the NV release flag to mac and rlc.

DEPENDENCIES
  None.

RETURN VALUE
  l2_nv_rel_indicator_e_type.

===========================================================================*/
l2_nv_rel_indicator_e_type l2_get_release_flag_indicator
(
  sys_modem_as_id_e_type as_id
)
{
  return (wl2_nv_list[as_id].l2_nv_rel_indicator);
}

/*===========================================================================

FUNCTION l2_get_release_flag_indicator

DESCRIPTION
  This  function returns the NV feature flag of L2.

===========================================================================*/
uint32 l2_get_feature_flag_nv(sys_modem_as_id_e_type as_id)
{
  return (wl2_nv_list[as_id].wl2_feature_flg);
}

/*-------------------------------------------------------------------
FUNCTION l2_free_mem_ZI_opt

DESCRIPTION
 This function will be called by RRC in the call back , so that L2 (MAC) can de-allocate/free
 the dynamic memory allocated data strucutre varaibles.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
--------------------------------------------------------------------*/
void l2_free_mem_ZI_opt(sys_modem_as_id_e_type as_id)
{
  mac_hs_table_type *hs_table_ptr;
  l2_validate_as_id(as_id);
  hs_table_ptr = GET_MAC_HS_TABLE_PTR(as_id);

// Its wrapper function called from RRC.So any code 
//added in this function please keep under feature flags

#ifdef FEATURE_WCDMA_MAC_UL_ZI_REDUCTION
  #error code not present
#endif /* f FEATURE_WCDMA_MAC_UL_ZI_REDUCTION */

  mac_hs_dl_free_mem_ZI_opt(hs_table_ptr);

}

/*===========================================================================
 FUNCTION     WCDMA_L2_UL_INIT_TCB

 DESCRIPTION
   This function populates the cached WCDMA L2 UL TCB pointer through rcinit. 

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   An assertion will fail if the lookup fails.
===========================================================================*/
void wcdma_l2_ul_init_tcb(sys_modem_as_id_e_type as_id)
{
  if (wcdma_l2_ul_tcb_ptr[as_id] == RCINIT_NULL)
  {
    wcdma_l2_ul_tcb_ptr[as_id] = rcinit_lookup_rextask(WCDMA_L2_UL_TASK_NAME(as_id)); 
    if (wcdma_l2_ul_tcb_ptr[as_id] == RCINIT_NULL)
    {
      WL2_MSG_0(ERROR, RLCI_GET_UL_TBL_PTR(as_id), "Lookup for wcdma_l2_ul_tcb failed" );
      ASSERT(0);
    }
  }
}

/*===========================================================================
 FUNCTION     WCDMA_L2_UL_GET_TCB

 DESCRIPTION
   This function returns a pointer to the WCDMA L2 UL TCB. 

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   A pointer to the WCDMA L2 UL TCB.

 SIDE EFFECTS
   An assertion will fail if the lookup fails.
===========================================================================*/
rex_tcb_type* wcdma_l2_ul_get_tcb(sys_modem_as_id_e_type sub_id)
{
  sys_modem_as_id_e_type as_id = wcdma_convert_subid_to_stack_index(sub_id);
  l2_validate_as_id(as_id);
  
  if (wcdma_l2_ul_tcb_ptr[as_id] == RCINIT_NULL)
  {
    WL2_MSG_0(ERROR, RLCI_GET_UL_TBL_PTR(as_id),  "Trying to get tcb before WL2 UL is initialized");
    wcdma_l2_ul_init_tcb(as_id);
  }
  return wcdma_l2_ul_tcb_ptr[as_id];
}
/*===========================================================================
 FUNCTION     WCDMA_L2_READ_NV

 DESCRIPTION
   Function reads L2 related NV items. 
===========================================================================*/
void wcdma_l2_read_nv(sys_modem_as_id_e_type as_id)
{

#ifdef FEATURE_REL_ID_NV_SUPPORT
  /* This default value here is meaningless. Just keep lint happy. */
  l2_nv_rel_indicator_e_type  nv_release_flag = L2_REL_INDICATOR_5;

  /* read release info from NV.
     If NV not set or out of range value, use default.
     Default is to take the max. support. */
  wnv_api_status wnv_status = wnv_get_nv_msim(WNV_ID_RRC_VERSION_I_FOR_L2, 
                                             &nv_release_flag, 
                                             sizeof(nv_release_flag), as_id);
  
  WL2_MSG_2(HIGH, RLCI_GET_UL_TBL_PTR(as_id), "NV_WCDMA_RRC_VERSION_I is %d, read from NV: %d",
                                            nv_release_flag, (wnv_status==WNV_SUCCESS));
  
  if( (nv_release_flag > WNV_GET_DEFAULT(WNV_ID_RRC_VERSION_I_FOR_L2))
        /* nv_release_flag is uint. impossible to get a negative value */
        /* || (nv_release_flag < L2_REL_INDICATOR_99) */ )
  {
    nv_release_flag = WNV_GET_DEFAULT(WNV_ID_RRC_VERSION_I_FOR_L2);
  }

  /*-------------------------------------------------------------------------
    Setting the NV flag value. If the nv data returns an error, this value
    would be 1 which means release 5.
  -------------------------------------------------------------------------*/
  wl2_nv_list[as_id].l2_nv_rel_indicator = nv_release_flag;


#else //FEATURE_REL_ID_NV_SUPPORT

  /* Default is to take the max. support. */
  wl2_nv_list[as_id].l2_nv_rel_indicator = WNV_GET_DEFAULT(WNV_ID_RRC_VERSION_I_FOR_L2);

#endif //FEATURE_REL_ID_NV_SUPPORT

  wnv_init_efs_conf_and_nv_files_msim(WNV_CONF_ID_L2,as_id);  

  wnv_status = wnv_get_nv_msim(WNV_ID_L2_FEATURE_FLAG_NV,&(wl2_nv_list[as_id].wl2_feature_flg),
                                             sizeof(wl2_nv_list[as_id].wl2_feature_flg),as_id);

  WL2_MSG_2(HIGH, RLCI_GET_UL_TBL_PTR(as_id), "L2_FEATURE_FLAG_NV is %d, read from NV: %d",
                                wl2_nv_list[as_id].wl2_feature_flg, (wnv_status==WNV_SUCCESS));

}

/*===========================================================================================
                               R A C H    H A N D L E R

GENERAL DESCRIPTION

  This module provides functions to handle RACh functions

Copyright (c) 2001-2004 by Qualcomm Technologies Incorporated.  All Rights Reserved.
Copyright (c) 2005-2009 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===============================================================================================*

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macrach.c_v   1.17   10 Jul 2002 10:01:34   skrishna  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/src/macrach.c#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when      who  what, where, why
--------  ---  -----------------------------------------------------------
04/22/16  as    Remove excessive F3s
04/13/16  as    Fix for EUL ISR delay crash due to MAC reconfig on other sub
11/06/15  kv    RACH vs QTA changes
10/21/15  kv    MAC to handle (HS)RACH_ABORT_QTA_FMO indication from UL
08/18/15  as    RLC optimization to use 1 Large DSM items instead of 26 during R99 UL PDU building.
07/16/15  aa    L2 to pass/process sub-id correctly when DUAL_WCDMA feature is disabled.
06/26/15  aa    Removing extern declarations from L2 code
06/08/15  as    W+W MAC UL/DL HS task variable doubling.
06/01/15  as    In FACH state MAC should send CMAC_STATUS_IND only for TM CCCH UL data.
06/01/15  kc    Remember Aborted Status PDU on HS-RACH
04/25/15  scv   W+W Changes to have messages support SubID
03/19/15  aa    RRC-L2 interface changes for passing AS ID as part of L2 commands
03/10/15  aa    Duplicated L2 task related globals and removed AS_ID hardcodings
02/28/15  rsr   W + W Phase 2 Checkin.
02/23/15  kv    Enable RxD after back-off timer expired and turn on RxD after persistency passed
01/29/15  kc    L2 global reorg for W+W
01/15/15  vr    Wrapping up some UL globals used by L2 under MACROs
11/07/14  as    Added code change to not to acquire HS-RACH resource and also to trigger polling 
                in RWS situation.
10/08/14  kc    Decouple data indication in FACH & trigger based RACH logics
07/30/14  aa/kv Fix compiler warnings on Bolt
07/26/14  rkmk  Reduce image size by using right macro depending upon number of parameters
06/19/14  kc    Mainline FEATURE_WCDMA_HS_RACH_DELAY_CU
04/02/14  as    Fix to avoid concurency issue of MAC clearing the RACH status as part of config.
05/15/14  kc    Changes for enabling RxD for FACH Signaling when HS-RACH is configured
05/06/14  geg   Use safer versions of memcpy() and memmove()
01/06/13  kc    Data Indication to RRC during FACH state
10/29/13  ts    Added API mac_fetch_random_number() which calls secapi_get_random() having very good cryptographic properties
06/12/13  as    HSRACH - Fix to clear TM mode partial info at MAC as well during all scenarios
                while flushing RLC TM water mark and partial info.
05/09/13  kc    Fix to clear status at RLC while moving to PCH & RACH abort is received
05/08/13  scv   Fix L2 compilation errors
02/19/13  kc    Fix to pend status only if trashed pdu is RLC AM CTRL pdu
02/14/13  pr    Changed error fatal in mac_ul_rach_check_for_bfi_failure() to msg_error. 
02/07/13  as    Added debug code to crash if L2 has returned true for pending data in 
                buffer but fails during PDU build event when UE is in RACH state.
01/21/13  ksr   Initialization of hs_rach_state to RACH_IN_IDLE on config
07/16/12  mrg   MAC_I & HS-RACH Compilation Warnings fixed
09/13/12  mrg   HS-RACH Fix to start T2 & Backoff timers after collision resolution timer expires
09/04/12  kc    Fix to free DSM item pre-allocated at RLC when the very first 
                RACH procedure fails persistence and RACH abort is received thereafter.
07/25/12  as    New WM reg api() added for trigger base rach
08/13/12  ash   Correction to the ASC num value for HSRACH preamble procedure
08/13/12  mrg   HS-RACH & MAC-I Feature updated code check-in.
07/17/12  mrg   Moved enable/disable indication of Trigger based RACH from BFI to l1_query_mac_rach_data()
08/13/12  mrg   In FACH state, when L2 ack dropped due to RACH ABORT form L1 - 
                 inform to RLC to do re-tx of this control PDU
07/24/12  geg   Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/19/12  mrg   L2 - MAC-I and HS-RACH code changes
04/24/12  grk   Fixed compiler warnings
03/20/12  kc    Fixed concurrency issue between MAC config and RACH procedure
03/06/12  mrg   RACH staus info from L1 to MAC indicaiton with signal based approch enhanced with double buffer.
03/09/12  mrg   Persistence F3 are changed 
02/16/12   Ksr  Added support for Trigger Based Rach Procedure
01/02/12  grk   Feature cleanup
12/06/11  ksr   Fix to handle back to back RACH ABORT and RACH NO ACK to MAC from L1
                Moving to command based interface from signal based approach.
11/23/11  grk   Added mutex protection for RACH reset.
06/02/11  kc    Added support for logical channel to transport channel mapping info
12/16/10  mrg   In case of RACH abort and persistency failure MAC informs RLC to re-send status report
01/22/10  mrg   QCHAT MAC side changes
10/07/09  mrg   Fixed Klocwork warnings
06/26/08  av    l1-l2 interface cleanup 
05/07/09  ssg   Updated Copyright Information
04/29/09  av    Fixed compiler warnings in L2 
10/06/08  grk   Lint warning fixes.
09/16/08  ssg   MAC now indicates the sucessful TX of RACH msg to RRC.
04/11/07  ssg   Changed the included interface file.
09/26/07  mrg   Added support for ATF under FEATURE_CCCH_IE_ENH  
12/07/06  sk    Fixed compiler warning Removed valid declaration
05/09/06  sk/tliou Added code to remove dm dup under FEATURE_WCDMA_BUILD_PDU_DSM_OPT ASAP
05/27/05  sk    Removed compiler warnings.
05/05/05  sk    Changed the name of asc_selection to mac_asc_selection. Now this
                function is called from maculdata.c after L1 is queried to see if
                there is any available subchannel for that particular message.
03/09/05  sk    Fixed all lint errors.
02/09/05  sk    Fix for critical lint errors.
01/10/05  src   Replaced an instance of FEATURE_INTER_RAT_CELL_FACH_RE_SELECTION
                with FEATURE_CELL_FACH_MEAS_OCCASION to build without the former.
12/30/04  sk    Fixed lint errors
12/08/04  sk    Fixed compiler warnings.
11/12/04  yiz   Renamed RACH ACK status to L1_PRACH_MSG_DONE.
04/19/04  sk    Removed compiler warning for comparing an uitn with <= 0
03/30/03  sk    In persistence_is_ok() reverted the changes of randomly delaying prach based on
                persistence values.
03/13/03  sk    Fix for CR #25935. RACH is randomly delayed based on the persistence values.
12/04/02  sk    mac_reset_rach(): MAC will not register the call back function with RLC.
                PRACH would always be enabled during the lifetime of the call.
11/15/02  sk    Temporarily turn off the random delay caused in RACH transmissions for ASC_SELECTION.
11/07/02  sk    If persistence is not ok clear the num_trch = 0 to indicate that MAC is not ready
                to transmit this TTI
10/31/02  sk    mac_free_tx_dsm_items(): Clearing the number of transport channels to 0.
10/10/02  sk    Copied the CCCH parameters correctly for RLC size restrictions.
10/07/02  sk    Handled the case when MAC receives a L1_PRACH_WRONG_TTI from L1.
09/25/02  sk    proc_ul_tx_status_ind(): Ensured that MAC is able to handle RACH_ABORT ireespective
                of the current state of RACH. Copied rlc_size info whenthe cctrch type is RACH.
                This is under FEATURE_TFC_RLC_SIZE.
09/18/02  sk    Handled the case when RACH is waiting for the Right TTI to transmit after we
                get a NO_ACK from L1.
09/11/02  sk    Removed some unnecessary comments.
06/08/02  sk    Removed FEATURE_TRAFFIC_VOLUME_MEAS
05/13/02  sk    Populate the trch_id_to_idx_map_type structure for the purpose of
                traffic volume measurements. This addition is defined under the
                #ifdef FEATURE_TRAFFIC_VOLUME_MEAS
03/22/02  sh    l1_ul_trigger_tx_req(): use activation time only if
                FEATURE_L1_ACT_TIME is not defined.
03/20/02  ps    Changed the msg level of "Released DSM Item" from HIGH to LOW
03/20/02  ps    Fixed the DSM release problem
03/20/02  ps    Added F3 message when DSM items are released
03/12/02  ps    proc_rach_cctrch_cfg() : Do not process dedicated logical channel info if the
                number of dedicated logical channels is zero
02/12/02  ps    Deleted Uplink U-RNTI related code (U-RNTI is never used in UL)
11/29/01  ps    build_rach_tblks() : Added code to check number of transport channels in l1_tx_buf
08/07/01  ps    Fixed the leading zero message formatting problem
07/01/01  ps    Removed the code that sets and clears "mac_busy" flag (not used anymore)
                Removed the extra call to "asc_selection"
06/19/01  ps    Converted UL TX Status and Build Frame Indications from commands to signals
06/16/01  ps    Added code to process L1_PRACH_ABORT status and adjusted MSG levels
04/04/01  ps    Initial release after merging "macrachcfg.c" and "mactxstatus.c"

==============================================================================================*/

/*==============================================================================================
                                    INCLUDE FILES
==============================================================================================*/
#include "wcdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "rrccmd_v.h"
#include  "macrrcif_v.h"
#include  "macrlcif.h"
#include  "l1sapcommon.h"
#include  "l1task_v.h"
#include  "l2ultask.h"
#include  "macinternal.h"
#include  "macul.h"
#include  "maculdata.h"
#include  "ran.h"
#include  "secapi.h"
#include  "maculsubtask.h"
#include  "macdlhs.h"

#include "l1macif.h"

#include  <string.h>
#include  "macrach.h"

#ifdef FEATURE_QCHAT
#include "rrc_qchatupk.h"
#endif

/*===============================================================================================
                           DATA DECLARATIONS
===============================================================================================*/
#ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
 #define  MAC_RACH_CONSEC_TTI_WITHOUT_DATA   5  
#endif 

/*==============================================================================================
 FUNCTION NAME  l1_ul_trigger_tx_req

 DESCRIPTION  This function triggers L1 to enable / disable PRACH
===============================================================================================*/
void  l1_ul_trigger_tx_req
  (
/* L1 TX Trigger Request Type */
  l1_ul_phychan_ctrl_type   action, mac_ul_table_type *ul_table_ptr
  )
{
  /* L1 External Command Buffer Pointer */
  l1_ext_cmd_type   *cmd_ptr;

  /* Get External Command Buffer Pointer */
  cmd_ptr = l1_get_cmd_buf();

  /* L1 External Command Buffer Pointer is NULL */
  if (cmd_ptr == NULL)
  {
    WL2_MSG_0(HIGH, ul_table_ptr , "CPHY_UL_TX_CONTROL_REQ command buffer allocation failed");
  }
  else
  {
    /* Populate Command Header fields */
    cmd_ptr->cmd_hdr.cmd_id = CPHY_UL_TX_CONTROL_REQ;

    /* This should be removed when activation time is a regular feature */

    /* Populate ommand data fields */
    cmd_ptr->cmd.ul_tx_ctrl.chan = L1_UL_PHYCHAN_PRACH;
    cmd_ptr->cmd.ul_tx_ctrl.action = action;
#ifdef FEATURE_DUAL_SIM
    cmd_ptr->cmd_hdr.sys_modem_as_id = ul_table_ptr->as_id;
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif

    /* Put the command in L1 External Command Queue */
    wl1_put_cmd (cmd_ptr, ul_table_ptr->as_id);

    WL2_MSG_1(HIGH, ul_table_ptr , "CPHY_UL_TX_CONTROL_REQ sent %u",
                                                action);

  }
}

/*===============================================================================================
FUNCTION NAME mac_free_tx_dsm_items

DESCRIPTION   Frees Transmit DSM items
===============================================================================================*/
void  mac_free_tx_dsm_items
  (
/* Transmit Data Buffer Pointer */
  l1_ul_tx_data_type    *tx_buf_ptr,
  mac_ul_table_type *ul_table_ptr
  )
{
  uint8 j;

  /* Transport channel TX info pointer */
  l1_ul_trch_data_type  *trch_tx_ptr;

  /* Pointer to Transport channel info */
  trch_tx_ptr = &tx_buf_ptr->tx_data[0];

  WL2_MSG_0(HIGH, ul_table_ptr, "Released DSM items, set Trch=0");

  tx_buf_ptr->num_trch = 0;

  /* Release the DSM item of each Transport Block */
  for (j=0; j < ul_table_ptr->rach_info.num_tx_tbs; j++)
  {

    l1_l2_dsm_free_packet (ul_table_ptr->as_id,&trch_tx_ptr->trch_data[j].data_ptr);

    WL2_MSG_1(LOW, ul_table_ptr , "Released DSM item %d", j);
  }
}

/*===============================================================================================
FUNCTION NAME mac_reset_rach

DESCRIPTION   Resets RACH state
===============================================================================================*/
void  mac_reset_rach
  (
  mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr
  )
{

  mac_rach_table_type *rach_info_ptr = &ul_table_ptr->rach_info;
#ifdef FEATURE_WCDMA_HS_RACH
  if (eul_table_ptr->mac_hs_rach_info.hs_rach_active == FALSE)
#endif
  {
    /* Free the DSM items */
    mac_free_tx_dsm_items (rach_info_ptr->l1_tx_buf_ptr, ul_table_ptr);
  }

  memset(&rach_info_ptr->mac_rach_tx_buf, 0x0, sizeof(l1_ul_tx_data_type));
  WL2_MSG_0(HIGH, ul_table_ptr, "MAC_RACH: Clearing mac_rach_tx_buf");

  rach_info_ptr->rach_state = RACH_IN_IDLE;   /* Set RACH state to IDLE */

  rach_info_ptr->mac_rach_ccch_data_presence = FALSE;
#ifdef FEATURE_WCDMA_HS_RACH
  rach_info_ptr->hs_rach_state = RACH_IN_IDLE;
#endif
  rach_info_ptr->data_present_on_srb = FALSE;
  
  rach_info_ptr->npreamble_cycles = 0;   /* Clear preamble ramping cycle count */

#ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
  rach_info_ptr->num_tti_consec_without_data = 0;
#endif

#ifdef FEATURE_QCHAT
  if (qchatupk_wcdma_asc_enabled())
  {
    qchatupk_mac_reset_rach();
  }
#endif
}

/*===============================================================================================
FUNCTION NAME mac_send_tx_status_ind_to_rrc

DESCRIPTION   Sends CMAC_STATUS_IND to RRC
===============================================================================================*/
void  mac_send_tx_status_ind_to_rrc
  (
  sys_modem_as_id_e_type as_id,
  mac_status_ind_e_type status
  )
{
  rrc_cmd_type   *rrc_cmd_ptr;
  mac_ul_table_type *mac_ul_table_ptr; 
  
  mac_ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id); 
  
  /* Failed to allocate memory for RRC command */
  if ((rrc_cmd_ptr = rrc_get_mac_cmd_buf()) == NULL)
  {
    WL2_MSG_0(HIGH, mac_ul_table_ptr , "NULL Ptr when sending CMAC_STATUS_IND");
  }
  /* Send RRC_CMAC_STATUS_IND to RRC */
  else
  {
    WL2_MSG_0(HIGH, mac_ul_table_ptr , "Sending RRC_CMAC_STATUS_IND to RRC");
    
    /* Fill in command header */
    rrc_cmd_ptr->cmd_hdr.cmd_id = RRC_CMAC_STATUS_IND;

    /* Fill in command data */
    rrc_cmd_ptr->cmd.cmac_status_ind = status;

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif

    /* Send command to RRC */
    rrc_put_mac_cmd(as_id, rrc_cmd_ptr);
  }
}

#ifdef FEATURE_WCDMA_HS_RACH
/*===============================================================================================
FUNCTION NAME proc_rach_tx_parameters

DESCRIPTION   Processes RACH Transmit Control Parameters
===============================================================================================*/
void  proc_hs_rach_tx_parameters(mac_hs_rach_config_s_type *hs_rach_config_ptr, mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr)
{
  mac_rach_table_type *rach_info_ptr;
  rach_info_ptr = &ul_table_ptr->rach_info;
  
  /* The number of ASC's should be atleast 1 */
  if ((hs_rach_config_ptr->mac_ul_rach_txcntl.nASC == 0) ||
      (hs_rach_config_ptr->mac_ul_rach_txcntl.nASC > MAX_ASC) ||
      (hs_rach_config_ptr->ccch_asc > MAX_ASC))
  {
    ERR_FATAL("HS_RACH: Invalid number of ASC %d or ccch_asc %d", 
               hs_rach_config_ptr->mac_ul_rach_txcntl.nASC, hs_rach_config_ptr->ccch_asc, 0);
  }
  
  /* Copy RACH transmit control parameters to local memory */
  WCDMA_MEMCPY(&(eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info),
               sizeof(mac_ul_rach_txcntl_config_type), 
               &(hs_rach_config_ptr->mac_ul_rach_txcntl), 
               sizeof(mac_ul_rach_txcntl_config_type));

  eul_table_ptr->mac_hs_rach_info.hs_rach_ccch_asc = hs_rach_config_ptr->ccch_asc;
  
  memset(ul_table_ptr->hs_rach_tx_status, L1_HSRACH_MAC_INVALID, sizeof(ul_table_ptr->hs_rach_tx_status));
  
  /* Change the hs_rach_active to TRUE */
  eul_table_ptr->mac_hs_rach_info.hs_rach_active = TRUE;
  
  #ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
  rach_info_ptr->num_tti_consec_without_data = 0;
  #endif

  eul_table_ptr->mac_hs_rach_rxd_enable = FALSE; /* Initialize rxd_enable to false during config */
}

void proc_hs_rach_cfg(mac_ul_config_type *cfgptr, mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr)
{
  mac_rach_table_type *rach_info_ptr;
  rach_info_ptr = &ul_table_ptr->rach_info;

  if (cfgptr->hs_rach_action != MAC_HS_RACH_NOOP)
  {
    if ((cfgptr->hs_rach_action == MAC_HS_RACH_START) || 
        (cfgptr->hs_rach_action == MAC_HS_RACH_CONFIG))
    {
      if (cfgptr->hs_rach_config_ptr != NULL)
      {
        /* Process RACH Transmit Control Parameters */
        proc_hs_rach_tx_parameters (cfgptr->hs_rach_config_ptr, ul_table_ptr, eul_table_ptr);
      }
      else
      {
        ERR_FATAL("HS_RACH: NULL HS-RACH Cfg Ptr", 0, 0, 0);
      }
    }
    else
    {
      eul_table_ptr->mac_hs_rach_info.hs_rach_active = FALSE;
    }
    
    rach_info_ptr->hs_rach_state = RACH_IN_IDLE;
  }
  WL2_MSG_3(HIGH, ul_table_ptr, "HS_RACH: Config action %d, hs_rach_active %d, hs_rach_state %d",
         cfgptr->hs_rach_action, eul_table_ptr->mac_hs_rach_info.hs_rach_active, rach_info_ptr->hs_rach_state); 
}
#endif

/*===============================================================================================
FUNCTION NAME proc_rach_tx_parameters

DESCRIPTION   Processes RACH Transmit Control Parameters
===============================================================================================*/
void  proc_rach_tx_parameters
  (
  mac_ul_rach_txcntl_config_type    *rach_tx_prmtrs,  mac_ul_table_type *ul_table_ptr
  )
{
 mac_rach_table_type *rach_info_ptr;
 rach_info_ptr = &ul_table_ptr->rach_info;

  /* Copy RACH transmit control parameters to local memory */
  WCDMA_MEMCPY(&rach_info_ptr->rach_cntl_info,
               sizeof(mac_ul_rach_txcntl_config_type), 
               rach_tx_prmtrs, 
               sizeof(mac_ul_rach_txcntl_config_type));

  /* The number of ASC's should be atleast 1 */
  if (rach_info_ptr->rach_cntl_info.nASC == 0)
  {
    WL2_MSG_0(ERROR, ul_table_ptr, "Number of ASC's is 0");
  }

  rach_info_ptr->mac_ul_rach_abrt_rcvd = FALSE;
}



/*===============================================================================================
FUNCTION NAME pick_mlp

DESCRIPTION   Selects the maximum MAC logical channel priority.

RETURNS     1 - 8 Depending on the selected MLP
===============================================================================================*/
uint8 pick_mlp(rlc_ul_frame_format_type *rlc_chan_info_ptr, mac_ul_table_type *ul_table_ptr)
{
  uint8 mlp = MAC_UL_MAX_MLP_ID+1; // MAC_UL_MAX_MLP_ID = 8
  uint8           i;
  ul_lc_info_type *int_lc_ptr;
  rlc_ul_logchan_format_type    *lc_chan_ptr;

  /* Eliminate TFCs based on logical channel buffer occupancy */
  for (i=0 ; i < rlc_chan_info_ptr->nchan; i++)
  {
    lc_chan_ptr = &rlc_chan_info_ptr->chan_fmt[i];
    /* Get the logical channel ptr corresponding to the rlc_id */
    int_lc_ptr = GET_UL_LC_INFO_PTR((rlc_lc_id_type)(lc_chan_ptr->rlc_id));

    /* The number of pdus is greater than 0 */
    if (lc_chan_ptr->npdus > 0)
    {
      if (mlp > int_lc_ptr->asc_or_mlp)
      {
        mlp = int_lc_ptr->asc_or_mlp;
      }
    }

  }
  WL2_MSG_1(HIGH, ul_table_ptr, "Picked MLP %d", mlp);
  return mlp;
}


/*===============================================================================================
FUNCTION NAME asc_selection

DESCRIPTION   Selects an Access Service Class

RETURNS     0 - 7 Depending on the selected ASC
===============================================================================================*/
uint8 mac_asc_selection (rlc_ul_frame_format_type *chan_fmt, mac_ul_table_type *ul_table_ptr)
{
  mac_rach_table_type *rach_info_ptr;
  rach_info_ptr = &ul_table_ptr->rach_info;

  uint8 asc=0;

  uint8 mlp;

  /* If the ccch is enabled and if the number of dedicated logical
  channels mapped to ccch is 0 */
  if ((rach_info_ptr->rach_cfg_ptr->ccch_enable) && (rach_info_ptr->rach_cfg_ptr->ndlchan==0))
  {
    /* Since the value of asc is used as an index into rach_cntl_info.asc array
    to get the persistence values, care should be taken not to access the invalid
    entries in the array. hence the following check. rach_cntl_info.nASC gives the
    number of ASC's signalled to UE */
    if (rach_info_ptr->rach_cfg_ptr->ccch_asc >= rach_info_ptr->rach_cntl_info.nASC)
    {
      asc = rach_info_ptr->rach_cntl_info.nASC-1;
    }
    else /* its safe to use the ccch_asc specified */
    {
      asc = rach_info_ptr->rach_cfg_ptr->ccch_asc;
    }

    WL2_MSG_1(HIGH, ul_table_ptr, "Picked ASC %d", asc);
  }
  else
  {
    asc = rach_info_ptr->rach_cntl_info.nASC;

    /* Pick the mlp with the highest priority */
    mlp = pick_mlp(chan_fmt, ul_table_ptr);

    /* Among the ASC and MLP, pick the one with the highest priority */
    if (asc > mlp)
    {
      WL2_MSG_2(HIGH, ul_table_ptr, "Picked ASC: %d MLP %d",
                                                 asc, mlp);
      asc = mlp;
    }
    else
    {
      /* Since the value of asc is used as an index into rach_cntl_info.asc array
      to get the persistence values, care should be taken not to access the invalid
      entries in the array. hence the following check. rach_cntl_info.nASC gives the
      number of ASC's signalled to UE */
      WL2_MSG_2(HIGH, ul_table_ptr, "Picked ASC: %d MLP %d",
                                                 asc, mlp);
      asc = rach_info_ptr->rach_cntl_info.nASC-1;
    }
  }

#ifdef FEATURE_QCHAT
  if (qchatupk_wcdma_asc_enabled())
  {
    qchatupk_mac_asc_selection(&asc);
  }
#endif /* FEATURE_QCHAT */

  return(asc);
}

/*===============================================================================================
FUNCTION NAME persistence_is_ok

DESCRIPTION   Checks to see if the persistence is OK or not

RETURNS     TRUE  if the persistence is OK FALSE otherwise

===============================================================================================*/
boolean   persistence_is_ok (mac_ul_table_type *ul_table_ptr)
{
  boolean rv = FALSE;
  mac_rach_table_type *rach_info_ptr;
  rach_info_ptr = &ul_table_ptr->rach_info;


  uint32  random_num;

  // Below API returns a random number
  random_num = mac_fetch_random_number(ul_table_ptr->as_id);

  /* Normalize this random number to a value between 0 & 65535 */
  random_num = ran_dist(random_num, (uint16)0, (uint16)65535);

  /* If the random value is less than the persistence associated with the
  acess service class that was selected, then persistence test has passed */
  if (random_num <= rach_info_ptr->rach_cntl_info.asc[rach_info_ptr->l1_tx_buf_ptr->asc_num].pval)
  {
    rv = TRUE;
    WL2_MSG_2(HIGH, ul_table_ptr, "MAC_RACH: Passed Persistence. random_num %d, Persistence %d",
                                               random_num,
                                               rach_info_ptr->rach_cntl_info.asc[rach_info_ptr->l1_tx_buf_ptr->asc_num].pval);
  }
  /* Else the persistence test has failed and wait for the next TTI to
  generate a new random number ... */
  else
  {
    WL2_MSG_2(HIGH, ul_table_ptr, "MAC_RACH: Failed Persistence. rnum %d Pi %d",
               random_num, rach_info_ptr->rach_cntl_info.asc[rach_info_ptr->l1_tx_buf_ptr->asc_num].pval);
  }


  return(rv);
}

/*===============================================================================================

MACRO     DCCH_DTCH_HDR_WITH_CRNTI_ON_RACH

DESCRIPTION   This macro decides MAC header value and size when DCCH/DTCH is mapped to
        RACH, and C-RNTI is used

          If one DCCH/DTCH is mapped to RACH, MAC header size is 20-bits long and it
        consists of :
        - TCTF     : 2-bits long and its value = 01
        - UE ID Type : 2-bits long and its value = 01
        - UE ID    : 16-bits long and its value is the C-RNTI

        If more than one DCCH/DTCH is mapped to RACH, MAC header size is 24-bits
        long and it consists of :
        - TCTF     : 2-bits long and its value = 01
        - UE ID Type : 2-bits long and its value = 01
        - UE ID    : 16-bits long and its value is C-RNTI
        - C/T    : 4-bits long & value is one minus MAC Logical channel identity

==============================================================================================*/
#define   DCCH_DTCH_HDR_WITH_CRNTI_ON_RACH\
\
    /* Get CRNTI value */ \
    crnti = GET_UL_MAC_CRNTI; \
\
    /* 1st header byte consists of TCTF, UE-ID type & 4 MSBits of UE-ID */  \
    hdrval[0] = (uint8)(0x50 + (crnti >> 12));  \
\
    /* 2nd header byte contains the next 8 MSBits of UE-ID */ \
    hdrval[1] = (uint8)(crnti >> 4);  \
\
    /* 4 MSBits of 3rd header byte contains 4 LSBits of UE ID */  \
    hdrval[2] = (uint8)((crnti << 4) & 0xf0); \
\
    /* Multiple DCCH/DTCH mapped to RACH - Add C/T field */ \
    if (ndlchan > 1) {  \
\
       hdrval[2] |= (oc_dlcptr->mac_id - 1);  \
\
       int_lc_ptr->mac_hdr_size = 24; \
    } \
\
    /* One DCCH/DTCH mapped to RACH - There is no C/T field */  \
    else {  \
\
       /* Header size = 20 bits */  \
       int_lc_ptr->mac_hdr_size = 20; \
    } \
    /* Save header value */   \
    WCDMA_MEMCPY(int_lc_ptr->mac_hdr_value,           \
                 sizeof(uint8) * MAC_UL_MAX_HDR_SIZE, \
                 hdrval,                              \
                 MAC_UL_MAX_HDR_SIZE);                \


/*==============================================================================================
FUNCTION NAME   get_rach_cfg_ptr

DESCRIPTION     This function returns rach_cfg_ptr
==============================================================================================*/
mac_ul_rach_cctrch_config_type *get_rach_cfg_ptr(mac_ul_table_type *ul_table_ptr)
{
   return (ul_table_ptr->rach_info.rach_cfg_ptr);
}

/*==============================================================================================
 FUNCTION NAME    proc_rach_cctrch_cfg

 DESCRIPTION    This function processes ordered config data of RACH
==============================================================================================*/
void  proc_rach_cctrch_cfg
  (
/* Pointer to ordered config data of RACH */
  mac_ul_rach_cctrch_config_type    *oc_rachptr,
  mac_ul_table_type *ul_table_ptr
  )
{
  uint8 i, ndlchan, hdrval[MAC_UL_MAX_HDR_SIZE];
  mac_ul_ded_logch_config_type  *oc_dlcptr;
  ul_lc_info_type         *int_lc_ptr;
  ul_trch_info_type       *int_tc_ptr;
  //boolean                   valid; /*lint -e550*/
  uint16                    crnti;
  trch_id_to_idx_map_type   *id_to_idx_ptr;

  maci_ul_trch_priority_info_type  *priority_info_ptr;
  mac_rlc_data_table_type      *data_table_ptr;
  uint8     rlc_count=0;
  
  data_table_ptr = GET_MAC_RLC_DATA_TABLE_PTR(ul_table_ptr->as_id);
  
  /* Initialize */
  data_table_ptr->mac_ul_lc_info.nchan = 0;

  /* Get pointer to internal Transport Channel information */
  /* Right now, it is assumed that there is only one RACH */
  int_tc_ptr = GET_UL_TRCH_INFO_PTR(0);
  id_to_idx_ptr = GET_UL_TRCH_ID_TO_IDX_PTR(oc_rachptr->trch_id);
  /* Populate the trch_id_to_idx_table */
  id_to_idx_ptr->idx = 0;
  id_to_idx_ptr->valid = TRUE;

  WL2_MSG_0(LOW, ul_table_ptr , "RACH Configuration Parameters:");

  WCDMA_MEMCPY(&ul_table_ptr->rach_info.rach_cfg_info,           \
               sizeof(mac_ul_rach_cctrch_config_type), \
               oc_rachptr,                              \
               sizeof(mac_ul_rach_cctrch_config_type));  

  /* Save RACH Ordered Config Data pointer */
  ul_table_ptr->rach_info.rach_cfg_ptr = &ul_table_ptr->rach_info.rach_cfg_info;

  /* Transport channel ID */
  int_tc_ptr->trch_id = oc_rachptr->trch_id;

  /* CCCH is mapped to RACH */
  if (oc_rachptr->ccch_enable)
  {

    /* CCCH ID is invalid */
    if (UL_RLC_ID_IS_INVALID(oc_rachptr->ccch_id))
    {
      WL2_MSG_1(HIGH, ul_table_ptr , "CCCH ID is invalid %d",
                                                  oc_rachptr->ccch_id);
    }
    /* Access Service Class is invalid */
    else if (UL_ASC_IS_INVALID(oc_rachptr->ccch_asc))
    {
      WL2_MSG_1(HIGH, ul_table_ptr , "ASC is invalid %d",
                                                  oc_rachptr->ccch_asc);
    }
    /* CCCH parameters are valid */
    else
    {
      /* RLC info pointer */
      int_lc_ptr = GET_UL_LC_INFO_PTR(oc_rachptr->ccch_id);

      /* Set Enable flag to TRUE */
      int_lc_ptr->enable = TRUE;

      /* Logical channel type is CCCH */
      int_lc_ptr->lc_type = UE_LOGCHAN_CCCH;

      /* Set RLC mode to TM_MODE */
      int_lc_ptr->rlc_mode = UE_MODE_TRANSPARENT;

      /* Save the rlc size restriction information */
      int_lc_ptr->rlc_size_info.all_tfi_valid = FALSE;
      int_lc_ptr->rlc_size_info.ntfi_per_lc = 1;
    /* if ATF present , ATF that CCCH should use assigned to int_lc_ptr */
    if (ul_table_ptr->rach_info.rach_cfg_ptr->atf_present)
    {
      int_lc_ptr->rlc_size_info.lc_tfi_list[0] = ul_table_ptr->rach_info.rach_cfg_ptr->atf_ccch_tf_index;
    }
    else
    {
      int_lc_ptr->rlc_size_info.lc_tfi_list[0] = 0;
    }

    /* Access Service Class */
    int_lc_ptr->asc_or_mlp = oc_rachptr->ccch_asc;

    /* MAC Header Size = 2 bits */
    int_lc_ptr->mac_hdr_size = 2;

    /* Save the Logical channel ID in transport channel information */
    int_tc_ptr->rlc_id[int_tc_ptr->nlchan] = oc_rachptr->ccch_id;

    /* Update logical channel count of transport channel info */
    int_tc_ptr->nlchan++;

    if (rlc_count < UE_MAX_UL_LOGICAL_CHANNEL)
    {
      data_table_ptr->mac_ul_lc_info.rlc_id[rlc_count++] = oc_rachptr->ccch_id;
    }
    data_table_ptr->mac_ul_lc_info.nchan = rlc_count;

    WL2_MSG_3(LOW, ul_table_ptr , "CCCH: ASC= %d RLC Mode= %d RLC ID= %d",
                                               int_lc_ptr->asc_or_mlp,
                                               int_lc_ptr->rlc_mode,
                                               oc_rachptr->ccch_id);

      // Get the priority information of the transport channel 0 with priority
      // as indicated by the ccch_asc field.
      if (oc_rachptr->ccch_asc == 0)
      {
        priority_info_ptr = &ul_table_ptr->int_ul_trch_priority_info[0][0];
      }
      else
      {
        priority_info_ptr = GET_UL_TRCH_PRIORITY_INFO_PTR(0,int_lc_ptr->asc_or_mlp);
      }
      // Populate the logical channel id which has this priority.
      priority_info_ptr->lc_id[priority_info_ptr->num_lc] = oc_rachptr->ccch_id;
      priority_info_ptr->num_lc++;

      WL2_MSG_2(LOW, ul_table_ptr, "Pri %d, NChan %d",
                                    oc_rachptr->ccch_asc,
                                    priority_info_ptr->num_lc);

    }
  }

  /* Number of DCCH/DTCHs */
  ndlchan = oc_rachptr->ndlchan;

  if (ndlchan >0 )
  {

    /* Valid number of DCCH/DTCHs mapped to RACH */
    if (ndlchan > MAX_DED_LOGCHAN_PER_TRCH)
    {
      WL2_MSG_1(HIGH, ul_table_ptr , "invalid number of DCCH/DTCHs %d",
                                                  ndlchan);
    }
    /* Invalid C-RNTI */
    else if (UL_MAC_CRNTI_IS_VALID == FALSE)
    {
      WL2_MSG_0(HIGH, ul_table_ptr , "UE has no valid CRNTI");
    }
    /* Process config info of each DCCH/DTCH */
    else
    {
      for (i=0; i < ndlchan; i++)
      {

        /* Initially mark the parameter validity flag to FALSE */
        //valid = FALSE;

        /* Pointer to ordered config parameters of DCCH/DTCH */
        oc_dlcptr = &oc_rachptr->dlchan_info[i];

        /* Invalid RLC ID */
        if (UL_RLC_ID_IS_INVALID(oc_dlcptr->rlc_id))
        {
          WL2_MSG_2(HIGH, ul_table_ptr , "DCCH/DTCH%d with invalid RLC ID %d",
                                                      i, oc_dlcptr->rlc_id);
        }
        /* Invalid RB ID */
        else if (UL_RB_ID_IS_INVALID(oc_dlcptr->rb_id))
        {
          WL2_MSG_2(HIGH, ul_table_ptr , "DCCH/DTCH%d with invalid RB ID %d",
                                                      i, oc_dlcptr->rb_id);
        }
        /* Invalid MAC Priority */
        else if (UL_LC_PRIORITY_IS_INVALID(oc_dlcptr->priority))
        {
          WL2_MSG_2(HIGH, ul_table_ptr , "DCCH/DTCH%d with invalid priority %d",
                                                      i, oc_dlcptr->priority);
        }
        /* Invalid RLC Mode */
        else if (UL_RLC_MODE_IS_INVALID(oc_dlcptr->rlc_mode))
        {
          WL2_MSG_2(HIGH, ul_table_ptr , "DCCH/DTCH%d with invalid RLC Mode %d",
                                                      i, oc_dlcptr->rlc_mode);
        }
        /* Invalid MAC ID */
        else if ((ndlchan > 1) && UL_MAC_ID_IS_INVALID(oc_dlcptr->mac_id))
        {
          WL2_MSG_2(HIGH, ul_table_ptr , "DCCH/DTCH%d with invalid MAC ID %d",
                                                      i, oc_dlcptr->mac_id);
        }
        /* Otherwise, parameters are valid */
        else
        {
          /* Uplink logical channel information table entry pointer */
          int_lc_ptr = GET_UL_LC_INFO_PTR(oc_dlcptr->rlc_id);

          /* Clear header value */
          memset (hdrval, 0,MAC_UL_MAX_HDR_SIZE);

          /* MAC Header Size and Header Value */
          DCCH_DTCH_HDR_WITH_CRNTI_ON_RACH

          /* Set Enable flag to TRUE */
          int_lc_ptr->enable = TRUE;

          /* Logical channel type */
          int_lc_ptr->lc_type = oc_dlcptr->chan_type;

          /* Save RLC mode */
          int_lc_ptr->rlc_mode = oc_dlcptr->rlc_mode;

          /* Save RB ID */
          int_lc_ptr->rb_id = oc_dlcptr->rb_id;

          /* Save priority */
          int_lc_ptr->asc_or_mlp = oc_dlcptr->priority;

          if (rlc_count < UE_MAX_UL_LOGICAL_CHANNEL)
          {
            data_table_ptr->mac_ul_lc_info.rlc_id[rlc_count++] = oc_dlcptr->rlc_id;
          }
          data_table_ptr->mac_ul_lc_info.nchan = rlc_count;


          /* Save the rlc size restriction information */
          WCDMA_MEMCPY(&int_lc_ptr->rlc_size_info,
                       sizeof(mac_ul_rlc_size_info_type),
                       &oc_dlcptr->rlc_size_info,
                       sizeof(mac_ul_rlc_size_info_type));

          /* RLC Header Length */
          if (int_lc_ptr->rlc_mode == UE_MODE_TRANSPARENT)
            int_lc_ptr->rlc_hdr_length = 0;
          else if (int_lc_ptr->rlc_mode == UE_MODE_UNACKNOWLEDGED)
            int_lc_ptr->rlc_hdr_length = 8;
          else
            int_lc_ptr->rlc_hdr_length = 16;

          priority_info_ptr = GET_UL_TRCH_PRIORITY_INFO_PTR(0, oc_dlcptr->priority);

          // Populate the logical channel id which has this priority.
          priority_info_ptr->lc_id[priority_info_ptr->num_lc] = oc_dlcptr->rlc_id;
          priority_info_ptr->num_lc++;
          WL2_MSG_2(LOW, ul_table_ptr, "Pri %d, NChan %d",
                                        oc_rachptr->ccch_asc,
                                        priority_info_ptr->num_lc);


          /* Save the Logical channel ID in transport channel information */
          /* Update logical channel count of transport channel info */
          int_tc_ptr->rlc_id[int_tc_ptr->nlchan] = oc_dlcptr->rlc_id;
          int_tc_ptr->nlchan++;

          WL2_MSG_3(LOW, ul_table_ptr , "DCCH/DTCH%d:HdrSize= %d Hdr[0]= %x",
                                                     i,
                                                     int_lc_ptr->mac_hdr_size,
                                                     int_lc_ptr->mac_hdr_value[0]);
          WL2_MSG_3(LOW, ul_table_ptr , ":Hdr[1]= %x Hdr[2]= %x Hdr[3]= %x",
                                                     int_lc_ptr->mac_hdr_value[1],
                                                     int_lc_ptr->mac_hdr_value[2],
            int_lc_ptr->mac_hdr_value[3]);
          WL2_MSG_3(LOW, ul_table_ptr , ":Hdr[4]= %x RB ID= %d RLC Mode= %d",
                                                     int_lc_ptr->mac_hdr_value[4],
                                                     int_lc_ptr->rb_id,
                                                     int_lc_ptr->rlc_mode);
        }
      }
    }
  }

#ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
  ul_table_ptr->rach_info.num_tti_consec_without_data = 0;
#endif
  
}

#ifdef  FEATURE_WCDMA_TRIGGER_BASED_RACH
/*==============================================================================================
 FUNCTION NAME  mac_ul_trigger_tx_req
 DESCRIPTION  This function is called by RLC whenever there is data queued in RLC buffers
        RLC notifies MAC to trigger L1 for PRACH enable
===============================================================================================*/
void  mac_rach_ul_trigger_tx_req(l1_ul_phychan_ctrl_type ul_req_type, sys_modem_as_id_e_type as_id)
{
  mac_ul_table_type *ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);
  WL2_MSG_1(HIGH, ul_table_ptr, "TRIG_RACH: Trigger L1 to enable/disable %d BFI",
                                             ul_req_type);
  l1_ul_trigger_tx_req (ul_req_type, ul_table_ptr);
}

void  mac_check_for_bfi_disable_trigger(mac_ul_table_type *ul_table_ptr)
{
 mac_rach_table_type *rach_info_ptr;
 rach_info_ptr = &ul_table_ptr->rach_info;

  rach_info_ptr->num_tti_consec_without_data++;
  
  if (rach_info_ptr->num_tti_consec_without_data >= MAC_RACH_CONSEC_TTI_WITHOUT_DATA)
  {
    WL2_MSG_1(HIGH, ul_table_ptr, "TRIG_RACH: %d consecutive TTI didnt have any data - Requesting L1 to stop BFI",
                    rach_info_ptr->num_tti_consec_without_data);
                
    rach_info_ptr->num_tti_consec_without_data = 0;
    
    if (l1_mac_rach_disable_trigger_needed(ul_table_ptr->as_id) == TRUE)
    {
      rlc_ul_register_trigger_based_rach_api_active_rbs(ul_table_ptr);
      mac_rach_ul_trigger_tx_req(L1_UL_PHYCHAN_DISABLE, ul_table_ptr->as_id);
    }
    
  }
}

#endif
/*==============================================================================================
 FUNCTION NAME  mac_ul_trigger_tx_req

 DESCRIPTION  This function is called by RLC whenever there is data queued in RLC buffers
        RLC notifies MAC to trigger L1 for PRACH enable

===============================================================================================*/
void  mac_ul_trigger_tx_req
(
  dsm_watermark_type* wm_ptr,
  void*               func_data
)
{
  mac_ul_table_type *ul_table_ptr = (mac_ul_table_type *)func_data;
  #ifdef  FEATURE_WCDMA_TRIGGER_BASED_RACH
  if ((l1_mac_rach_enable_trigger_needed(ul_table_ptr->as_id) == TRUE))
  {
    mac_rach_ul_trigger_tx_req(L1_UL_PHYCHAN_ENABLE, ul_table_ptr->as_id);
  }

  if(ul_table_ptr->mac_ul_config_in_progress != TRUE)
  {
    rlc_ul_call_data_ind_in_fach_cb(ul_table_ptr->as_id);
  }  
  #else

  WL2_MSG_0(HIGH, ul_table_ptr, "Enabling PRACH");
  /* Trigger L1 to enable PRACH */
  l1_ul_trigger_tx_req (L1_UL_PHYCHAN_ENABLE, ul_table_ptr);
  #endif
}

/*===============================================================================================
 FUNCTION NAME  proc_tx_status

 DESCRIPTION  This function processes transmit status received from PHY layer

===============================================================================================*/
void  proc_ul_tx_status_ind
(
  mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr
)
{

  uint8                       loop_index;
  mac_rach_table_type *rach_info_ptr = &ul_table_ptr->rach_info;
  
  REX_ISR_LOCK(&(rlc_mutexes[ul_table_ptr->as_id].r99_ul_isr_l2_datapath_mutex));
  
  for(loop_index = 0; loop_index < 2; loop_index++)
  {
    if (ul_table_ptr->rach_tx_status[loop_index] != L1_PRACH_MAC_INVALID)
    {

      if (rach_info_ptr->rach_state != RACH_IN_IDLE)
      {
        /* RACH is waiting for Status from Layer 1 */
        switch (ul_table_ptr->rach_tx_status[loop_index])
        {
        
          /* Received Tx Status is ACK */
          case L1_PRACH_MSG_DONE:
          /* RACH is not waiting for status */
          if (rach_info_ptr->rach_state != RACH_WAITING_FOR_STATUS)
          {
            WL2_MSG_1(HIGH, ul_table_ptr , "rcvd Tx status %x in wrong rach state %x",
                       ul_table_ptr->rach_tx_status[loop_index]);
          }
          else
          {
             WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_PRACH_ACK from L1");
             
            /* Send sucess indication to RRC */
             if (rach_info_ptr->mac_rach_ccch_data_presence)
             {
               mac_send_tx_status_ind_to_rrc(ul_table_ptr->as_id, RACH_TX_SUCCESS);
             }
        
             /* Reset RACH */
             mac_reset_rach(ul_table_ptr, eul_table_ptr);
          }
          break;

#ifdef FEATURE_CELL_FACH_MEAS_OCCASION
          case L1_PRACH_DELAY:
#else
          case L1_PRACH_WRONG_TTI:
#endif
          /* RACH is not waiting for status */
          if (rach_info_ptr->rach_state != RACH_WAITING_FOR_STATUS)
          {
              WL2_MSG_1(HIGH, ul_table_ptr , "rcvd Tx status %x in wrong rach state %x",
                                  ul_table_ptr->rach_tx_status[loop_index]);
          }
          else
          {
#ifdef FEATURE_INTER_RAT_CELL_FACH_RE_SELECTION
            WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_PRACH_DELAY from L1");
#else
            WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_PRACH_WRONG_TTI from L1");
#endif
            /* Set RACH state to RCVD_NO_ACK */
            rach_info_ptr->rach_state = RACH_RCVD_NOACK;
          }
          break;

          /* Received TX Status is NO ACK */
          case L1_PRACH_NO_ACK:

         /* RACH is not waiting for status */
          if (rach_info_ptr->rach_state != RACH_WAITING_FOR_STATUS)
          {
            WL2_MSG_1(HIGH, ul_table_ptr , "rcvd Tx status %x in wrong rach state %x",
                      ul_table_ptr->rach_tx_status[loop_index]);
          }
          else
          {
            WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_PRACH_NO_ACK from L1");
            
            /* Preamble Ramping cycles exceeded maximum count */
            if (rach_info_ptr->npreamble_cycles >= rach_info_ptr->rach_cntl_info.mmax)
            {
               WL2_MSG_2(HIGH, ul_table_ptr , "Reached max no. of Preamble cycles(%d)",
                                       rach_info_ptr->npreamble_cycles, rach_info_ptr->rach_cntl_info.mmax);            

             /* Send CMAC_STATUS_IND to RRC */
             if (rach_info_ptr->mac_rach_ccch_data_presence)
             {
               mac_send_tx_status_ind_to_rrc(ul_table_ptr->as_id, MAX_RACH_CYCLES_REACHED);
             }             
             /* Reset MAC */
              mac_reset_rach(ul_table_ptr, eul_table_ptr);
            }
            else
            {
              /* Set RACH state to RCVD_NO_ACK */
              rach_info_ptr->rach_state = RACH_RCVD_NOACK;

#ifdef FEATURE_QCHAT
              if (qchatupk_wcdma_asc_enabled())
              {
                qchatupk_mac_rach_noack(rach_info_ptr->npreamble_cycles);
              }
#endif
            }
          }
          break;

          /* Received TX Status is NACK */
          case L1_PRACH_NAK:
          /* RACH is not waiting for status */
          if (rach_info_ptr->rach_state != RACH_WAITING_FOR_STATUS)
          {
             WL2_MSG_1(HIGH, ul_table_ptr , "rcvd Tx status %x in wrong rach state %x",
                                                         ul_table_ptr->rach_tx_status[loop_index]);
          }
          else
          {

            WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_PRACH_NAK from L1");

            /* Preamble Ramping cycles exceeded maximum count */
            if (rach_info_ptr->npreamble_cycles >= rach_info_ptr->rach_cntl_info.mmax)
            {
              WL2_MSG_2(HIGH, ul_table_ptr , "Reached max no. of Preamble cycles(%d)",
                     rach_info_ptr->npreamble_cycles, rach_info_ptr->rach_cntl_info.mmax);

             /* Send CMAC_STATUS_IND to RRC */
             if (rach_info_ptr->mac_rach_ccch_data_presence)
             {
               mac_send_tx_status_ind_to_rrc(ul_table_ptr->as_id, MAX_RACH_CYCLES_REACHED);
             }  
             /* Reset MAC */
              mac_reset_rach(ul_table_ptr, eul_table_ptr);
            }
            else
            {

              /* Set RACH state to Backoff */
              rach_info_ptr->rach_state = RACH_IN_BACKOFF;

              /* set backoff counter value */

              rach_info_ptr->backoff_val =
                 ran_dist(mac_fetch_random_number(ul_table_ptr->as_id), (uint16)rach_info_ptr->rach_cntl_info.nbo1min, (uint16)(rach_info_ptr->rach_cntl_info.nbo1max+1));

              WL2_MSG_3(HIGH, ul_table_ptr, "Backoff_Val %d, lo_val %d, hi_val %d",
                 rach_info_ptr->backoff_val, rach_info_ptr->rach_cntl_info.nbo1min, rach_info_ptr->rach_cntl_info.nbo1max);

#ifdef FEATURE_QCHAT
              if (qchatupk_wcdma_asc_enabled())
              {
                qchatupk_mac_rach_nack(rach_info_ptr->npreamble_cycles);
              }
#endif
            }
          }
          break;

        /* Received Abort Request from L1 due to QTA or FMO */
	case L1_PRACH_ABORT_QTA_OR_FMO:		
	  WL2_MSG_0(HIGH, ul_table_ptr, "rcvd L1_PRACH_ABORT_QTA_OR_FMO from L1");
	  rach_info_ptr->rach_state = RACH_RCVD_NOACK;
          rach_info_ptr->npreamble_cycles--;	
	  break;

        /* Received Abort Request from L1 */
        case L1_PRACH_ABORT:
        if(rach_info_ptr->num_tx_tbs > 0)
        {
          WL2_MSG_1(HIGH, ul_table_ptr, "Req RLC to re-send status report for ntbs %d",
                                                     rach_info_ptr->num_tx_tbs);
          rlc_enh_indicate_ul_data_trash(ul_table_ptr->as_id);
          rach_info_ptr->mac_ul_rach_abrt_rcvd = TRUE;
        }
        {

          WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_PRACH_ABORT from L1");

          /* Send CMAC_STATUS_IND to RRC */
          if (rach_info_ptr->mac_rach_ccch_data_presence)
          {
            mac_send_tx_status_ind_to_rrc(ul_table_ptr->as_id, RACH_TX_ABORTED);
          }  
          /* Reset MAC */
           mac_reset_rach(ul_table_ptr, eul_table_ptr);
         }
         break;

         /* Invalid TX Status */
         default:
            WL2_MSG_1(HIGH, ul_table_ptr , "rcvd invalid Tx status (%d)",
                                ul_table_ptr->rach_tx_status[loop_index]);
          break;
        }
      }
      else
      {
        WL2_MSG_1(HIGH, ul_table_ptr, "RACH is in IDLE State - Not expected response %d",
                    ul_table_ptr->rach_tx_status[loop_index]);
      }     
      ul_table_ptr->rach_tx_status[loop_index] = L1_PRACH_MAC_INVALID;     
    }
  }
  
  REX_ISR_UNLOCK(&(rlc_mutexes[ul_table_ptr->as_id].r99_ul_isr_l2_datapath_mutex));
}

/*================================================================================================
FUNCTION NAME rach_tx_control

DESCRIPTION   This function controls RACH transmission and is called once every TTI
================================================================================================*/
boolean   rach_tx_control
  (
  mac_ul_table_type *ul_table_ptr
  )
{
  mac_ul_tfcs_info_type *tfcs_info_ptr;
  mac_ul_tfcs_type      *oc_tfcs_ptr;
  mac_rach_table_type *rach_info_ptr;

  rach_info_ptr = &ul_table_ptr->rach_info;
  tfcs_info_ptr = &ul_table_ptr->tfcs_info;
  oc_tfcs_ptr   = tfcs_info_ptr->oc_ul_tfcs_ptr;

  /* Initially set OK to transmit flag to FALSE */
  boolean   rach_tx_is_ok = FALSE;

  /* RACH Tx Control is performed based on RACH state */
  switch (rach_info_ptr->rach_state)
  {

  /* RACH in BACOFF state */
  case RACH_IN_BACKOFF:

    /* If backoff timer has expired, check the persistence */
    if (rach_info_ptr->backoff_val == 0)
    {
      WL2_MSG_0(HIGH, ul_table_ptr, "Backoff Tmr expired");

      /* If persistence check has failed, set RACH state to RACH_PERSISTENCE_FAILED
         and UE is not allowed to transmit on RACH */
      if (persistence_is_ok(ul_table_ptr) == TRUE)
      { /* Check if this is the right TTI to transmit */
        if (tfcs_info_ptr->current_tti >= oc_tfcs_ptr->tfs_info[0].tti_info)
        {
          rach_tx_is_ok = TRUE;
          WL2_MSG_1(HIGH, ul_table_ptr, "Found right TTI to transmit : cur_tti %d",
                                                     tfcs_info_ptr->current_tti);
        }
        else
        {
          rach_info_ptr->rach_state = RACH_WAITING_FOR_RIGHT_TTI;
          WL2_MSG_1(HIGH, ul_table_ptr, "RACH_WAITING_FOR_RIGHT_TTI to transmit : cur_tti %d",
                                                     tfcs_info_ptr->current_tti);
        }
      }
      else
        rach_info_ptr->rach_state = RACH_PERSISTENCE_FAILED;
    }
    else
    {   /*Update backoff timer value */
      WL2_MSG_1(HIGH, ul_table_ptr, "Backoff Val:%d",
                                                 rach_info_ptr->backoff_val);
      rach_info_ptr->backoff_val--;
    }
    break;

    /* RACH in RCVD_NO_ACK state or PERSISTENCE FAILED state */
  case RACH_WAITING_FOR_RIGHT_TTI:
  case RACH_RCVD_NOACK:
  case RACH_PERSISTENCE_FAILED:
    /* IF the persistence test has passed */
    if (persistence_is_ok (ul_table_ptr) == TRUE)
    {
      /* Check if this is the right TTI to transmit */
      if (tfcs_info_ptr->current_tti >= oc_tfcs_ptr->tfs_info[0].tti_info)
      {
        rach_tx_is_ok = TRUE;
        WL2_MSG_1(HIGH, ul_table_ptr, "Found right TTI to transmit : cur_tti %d",
                                                   tfcs_info_ptr->current_tti);
      }
      else
      {
        rach_info_ptr->rach_state = RACH_WAITING_FOR_RIGHT_TTI;
        WL2_MSG_1(HIGH, ul_table_ptr, "RACH_WAITING_FOR_RIGHT_TTI to transmit : cur_tti %d",
                                                   tfcs_info_ptr->current_tti);
      }
    }
    else
      rach_info_ptr->rach_state = RACH_PERSISTENCE_FAILED;
    break;

    /* RACH in Waiting for Status mode */
  case RACH_WAITING_FOR_STATUS:
    break;

  default:
    WL2_MSG_1(HIGH, ul_table_ptr , "invalid RACH state",
                                                rach_info_ptr->rach_state);
    break;
  }

  return(rach_tx_is_ok);
}

/*===============================================================================================
FUNCTION NAME build_rach_tblks

DESCRIPTION   This function builds RACH Transport Blocks
===============================================================================================*/
void  build_rach_tblks
(
  mac_ul_table_type *ul_table_ptr
)
{

  uint8           selected_tfi;
  mac_ul_build_frame_ind_type   *bld_frame_ind_ptr;
  mac_rach_table_type           *rach_info_ptr;
  mac_ul_tfcs_info_type         *tfcs_info_ptr;
  mac_ul_tfcs_type              *oc_tfcs_ptr;

  tfcs_info_ptr = &ul_table_ptr->tfcs_info;
  oc_tfcs_ptr   = tfcs_info_ptr->oc_ul_tfcs_ptr;

  bld_frame_ind_ptr = &ul_table_ptr->bld_frame_ind_info;
  rach_info_ptr = &ul_table_ptr->rach_info;
  
  if(ul_table_ptr->mac_ul_config_in_progress == TRUE)
  {
    WL2_MSG_0(ERROR, ul_table_ptr, "MAC_ERR: UL MAC config in progress - cannot build rach tblks");
    return;
  }
  L2_MUTEX_LOCK(&mac_mutexes[ul_table_ptr->as_id].mac_config_mutex);

  /* Layer 1 Transmit Data Buffer Pointer */
  rach_info_ptr->l1_tx_buf_ptr = bld_frame_ind_ptr->tx_buf_ptr;

  /* RACH in idle state and hence build new Transport blocks */
  if (rach_info_ptr->rach_state == RACH_IN_IDLE)
  {

    /* Build UL Transport Blocks */
    (void)build_ul_tblks (ul_table_ptr);

    if (rach_info_ptr->l1_tx_buf_ptr->num_trch == 1)
    {

      /* Selected ASC */
      //l1_tx_buf_ptr->asc_num = mac_asc_selection();

      /* Selected TFCI */
      rach_info_ptr->l1_tx_buf_ptr->TFCI = GET_SELECTED_TFCI;

      /* Save Transport Block Info */
      rach_info_ptr->num_tx_tbs = rach_info_ptr->l1_tx_buf_ptr->tx_data[0].num_tb;

      /* Persistence check has failed, set RACH state to RACH_PERSISTENCE_FAILED and */
      /* UE is not allowed to transmit on RACH */
      if (persistence_is_ok(ul_table_ptr) == FALSE)
      {
        rach_info_ptr->rach_state = RACH_PERSISTENCE_FAILED;
        rach_info_ptr->l1_tx_buf_ptr->num_trch = 0;
      }
      /* Persistence check has passed. It is OK to transmit on RACH */
      else
      {

        /* Set preamble ramping cycle count to one */
        rach_info_ptr->npreamble_cycles = 1;

        /* Set RACH state to RACH_WAITING_FOR_STATUS */
        rach_info_ptr->rach_state = RACH_WAITING_FOR_STATUS;

        WL2_MSG_0(LOW, ul_table_ptr , "UL Frame Queued on RACH");
      }
    }
  }
  /* OK to transmit in this TTI */
  else if (rach_tx_control(ul_table_ptr))
  {

    /* Set RACH state to RACH_WAITING_FOR_STATUS */
    rach_info_ptr->rach_state = RACH_WAITING_FOR_STATUS;

    /* Increment number of preamble ramping cycle count */
    rach_info_ptr->npreamble_cycles++;

    WL2_MSG_1(HIGH, ul_table_ptr, "npreamble_cycles %d",
                                               rach_info_ptr->npreamble_cycles);
    /* Set Active Transport Channel Count to 1 */
    rach_info_ptr->l1_tx_buf_ptr->num_trch = 1;

    /* Selected Transport Format Combination Pointer */
    selected_tfi = oc_tfcs_ptr->tfc_info[GET_SELECTED_TFCI].tfi[0];

    /* IF it is OK to transmit in this TTI then assign the transport blocks
    and the transport block information */
    rach_info_ptr->l1_tx_buf_ptr->tx_data[0].num_tb  = (uint8)oc_tfcs_ptr->tfs_info[0].tf_info[selected_tfi].ntblks;
    rach_info_ptr->l1_tx_buf_ptr->tx_data[0].tb_size = oc_tfcs_ptr->tfs_info[0].tf_info[selected_tfi].tb_size ;

    WL2_MSG_0(LOW, ul_table_ptr , "UL Frame Requeued on RACH");
  }

  /* Save Transport Block Info */
  if (rach_info_ptr->l1_tx_buf_ptr->num_trch ==1)
  {
    rach_info_ptr->num_tx_tbs = rach_info_ptr->l1_tx_buf_ptr->tx_data[0].num_tb;

    /* Few contents of L1 Transmit Buffer are reset by encoder at the end of UL ISR.
       Copying them as they are used while pend status checking during RACH ABORT */
    WCDMA_MEMCPY(&rach_info_ptr->mac_rach_tx_buf, 
                 sizeof(l1_ul_tx_data_type),
                 rach_info_ptr->l1_tx_buf_ptr, 
                 sizeof(l1_ul_tx_data_type));

    WL2_MSG_1(HIGH, ul_table_ptr, "MAC_RACH: updating mac_rach_tx_buf--num_tx_tbs %d",
                                               rach_info_ptr->num_tx_tbs);
  }

  if(rach_info_ptr->rach_state == RACH_WAITING_FOR_STATUS)
  {   
    rach_info_ptr->mac_ul_rach_abrt_rcvd = FALSE;
  }
  L2_MUTEX_UNLOCK(&mac_mutexes[ul_table_ptr->as_id].mac_config_mutex);
}

/*===============================================================================================
FUNCTION NAME mac_ul_check_rach_trch

DESCRIPTION   This function checks if the input rlc id and rb id pair map to a RACH transport channel
===============================================================================================*/

log_wcdma_trch_type mac_ul_check_rach_trch(rlc_lc_id_type rlc_id, rb_id_type rb_id, mac_ul_table_type *ul_table_ptr)
{
  log_wcdma_trch_type return_trch_val = WCDMA_TRCH_INVALID;
  ul_lc_info_type *int_lc_ptr;

  /* Get the logical channel ptr corresponding to the rlc_id  */
  int_lc_ptr = GET_UL_LC_INFO_PTR(rlc_id);

  if(int_lc_ptr == NULL)
  {
    WL2_MSG_0(ERROR, ul_table_ptr, "mac_ul_check_rach_trch: GET_UL_LC_INFO_PTR returns NULL");
  }
  /* Check if given RB ID mactch logical channel's RB ID */
  else if(int_lc_ptr->rb_id == rb_id)
  {
    return_trch_val = WCDMA_TRCH_RACH;
  }
  else
  { 
    WL2_MSG_2(HIGH, ul_table_ptr, "mac_ul_check_rach_trch: rrc_rb_id (%d) does not match l2_rb_id",
                                               rb_id, int_lc_ptr->rb_id);
  }

  return return_trch_val;
}

/*=========================================================================
FUNCTION MAC_UL_RACH_CHECK_FOR_BFI_FAILURE

DESCRIPTION  
  This routine is called by L1 when L2 has data in buffer but fails to build PDU during 
  build frame indication event.

DEPENDENCIES

RETURN VALUE
None

SIDE EFFECTS
  None
=========================================================================*/
void mac_ul_rach_check_for_bfi_failure(sys_modem_as_id_e_type as_id)
{
  mac_ul_table_type *ul_table_ptr;
  mac_rach_table_type *rach_info_ptr;
  mac_traffic_info_type *traffic_info_ptr;

  l2_validate_as_id(as_id);

  ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);
  rach_info_ptr = &ul_table_ptr->rach_info;
  traffic_info_ptr = &ul_table_ptr->traffic_info;

  if ((rach_info_ptr->rach_state == RACH_IN_IDLE) &&
      (FALSE == traffic_info_ptr->block_transmission) &&
      (FALSE == ul_table_ptr->mac_ul_config_in_progress))
  {
    WL2_MSG_3(ERROR, ul_table_ptr, "MAC_ERR: PDU building failed due to RACH_STATE %d or block_transmission %d or mac_config %d",
           rach_info_ptr->rach_state, traffic_info_ptr->block_transmission, ul_table_ptr->mac_ul_config_in_progress);
  }
  else
  {
    WL2_MSG_3(ERROR, ul_table_ptr, "MAC_ERR:NA PDU building failed due to RACH_STATE %d or block_transmission %d or mac_config %d",
                rach_info_ptr->rach_state, traffic_info_ptr->block_transmission, ul_table_ptr->mac_ul_config_in_progress);
  }
}

/*===============================================================================================
FUNCTION NAME mac_fetch_random_number

DESCRIPTION Based on featurization, it calls ran_next() or secapi_get_random()
            secapi_get_random() has better cryptographic properties

RETURNS     uint32 Random Number value
===============================================================================================*/
uint32 mac_fetch_random_number(sys_modem_as_id_e_type as_id)
{
  uint32 rand_num = 0;
  secerrno_enum_type  errorVal;
  
  /* secapi_get_random() function, declared in secapi.h, fetches random data from its internal 
     random buffer which is in turn filled from PRNG HW */
  errorVal = secapi_get_random(SECAPI_SECURE_RANDOM, (uint8*)&rand_num, sizeof(rand_num));
  
  if(errorVal != E_SUCCESS)
  {
     WMAC_ERR_FATAL(as_id, "RACH: Random Number was not generated properly, errorVal %d",(int)errorVal, 0, 0);
  } 
  return rand_num;
}

#ifdef FEATURE_WCDMA_HS_RACH

/*===============================================================================================
 FUNCTION NAME  : mac_hsrach_srb_rxd_manage

 DESCRIPTION    :  * If there is any data on HSRACH in UL, Call the RxD FACH function 
                   * which enables the RxD (if not yet enabled) and manages FMO 
===============================================================================================*/
void mac_hsrach_srb_rxd_manage(boolean data_present_on_srb, mac_eul_table_type *eul_table_ptr)
{
  if (eul_table_ptr->mac_hs_rach_info.hs_rach_active == TRUE)
  {
    if (data_present_on_srb == FALSE)
    {
      if (eul_table_ptr->mac_hs_rach_rxd_enable == TRUE)
      {
        eul_table_ptr->mac_hs_rach_rxd_enable = FALSE;
        WL2_MSG_0(ERROR, eul_table_ptr, "MAC_HS_RACH: Call RxD FACH Disable as SRB data NOT available");
        wl1_rxd_fach_signalling_data_available(FALSE, eul_table_ptr->as_id);
      }
    }
    else
    {
      if (eul_table_ptr->mac_hs_rach_rxd_enable == FALSE)
      {
        eul_table_ptr->mac_hs_rach_rxd_enable = TRUE;
        WL2_MSG_0(ERROR, eul_table_ptr, "MAC_HS_RACH: Call RxD FACH Enable as SRB data available");
        wl1_rxd_fach_signalling_data_available(TRUE, eul_table_ptr->as_id);
      }
    }
  }
}

boolean   mac_hs_rach_persistence_is_ok (uint8  selected_asc, mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr)
{
  boolean rv = FALSE;
  uint32  random_num;
  mac_rach_table_type *rach_info_ptr;
  rach_info_ptr = &ul_table_ptr->rach_info;
  
  random_num = mac_fetch_random_number(ul_table_ptr->as_id);
  
  /* Normalize this random number to a value between 0 & 65535 */
  random_num = ran_dist(random_num, (uint16)0, (uint16)65535);
  
  /* If the random value is less than the persistence associated with the
  acess service class that was selected, then persistence test has passed */
  if ((selected_asc < MAX_ASC) && (random_num <= eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.asc[selected_asc].pval))
  {
    rv = TRUE;
  }

  WL2_MSG_3(HIGH, ul_table_ptr, "HS_RACH: PASS(1)/FAIL(0) PERSISTENCY %d - random_num %d, Pi %d",
             rv, random_num, eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.asc[selected_asc].pval);
  
  return(rv);
}

uint8 mac_hs_rach_asc_selection(mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr)
{
  uint8       selected_asc = 0xFF, mlp;

  selected_asc = eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.nASC;
  
  if (eul_table_ptr->mac_hs_rach_info.mac_hsrach_logch_tx_type == L1_MAC_HS_RACH_CCCH_TRANS_TYPE)
  {
    /* Select the highest ASC, if the hs_rach_ccch_asc is greater than configured */
    if (eul_table_ptr->mac_hs_rach_info.hs_rach_ccch_asc >= eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.nASC)
    {
      selected_asc = eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.nASC - 1;
    }
    else
    {
      selected_asc = eul_table_ptr->mac_hs_rach_info.hs_rach_ccch_asc;
    }
  }  
  else if(eul_table_ptr->mac_hs_rach_info.mac_hsrach_logch_tx_type == L1_MAC_HS_RACH_DTCH_DCCH_TRANS_TYPE)
  {
    mlp = mac_hsrach_pick_mlp(ul_table_ptr, eul_table_ptr);
    
    /* Select the highest ASC, if the hs_rach_ccch_asc is greater than configured */
    if (selected_asc > mlp)
    {
      selected_asc = mlp;
    }
    else
    {
      selected_asc = eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.nASC - 1;
    }
  }
 
  WL2_MSG_3(HIGH, ul_table_ptr, "HS_RACH: configured ccch_asc %d, num_asc %d - Selected ASC %d",
           eul_table_ptr->mac_hs_rach_info.hs_rach_ccch_asc, eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.nASC, selected_asc);

  return(selected_asc);
}

boolean l1_phy_hs_rach_check_rach_trigger(uint32 * asc_num, boolean *srb_data_present, sys_modem_as_id_e_type as_id)
{
  uint8          selected_asc;
  boolean        buffer_staus_not_empty_flag, persistency_result, data_present_on_srb;
  mac_ul_table_type *ul_table_ptr; 
  mac_rach_table_type *rach_info_ptr;

  mac_eul_table_type *eul_table_ptr;

  l2_validate_as_id(as_id);

  persistency_result = FALSE;
  ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);
  rach_info_ptr = &ul_table_ptr->rach_info;
  eul_table_ptr = GET_MAC_EUL_TABLE_PTR(as_id);
  data_present_on_srb = rach_info_ptr->data_present_on_srb;

  if (eul_table_ptr->mac_hs_rach_info.hs_rach_active == TRUE) 
  {
    /* If there is a resource REL IND, Complete RACH procedure is cleared and RACH state moved to RACH_IN_IDLE */
    if ((eul_table_ptr->mac_hs_rach_info.mac_hsrach_edch_rsrc_rel_cause_valid == TRUE) &&
        ((eul_table_ptr->mac_hs_rach_info.mac_hsrach_edch_rsrc_rel_cause == WL1_HS_RACH_EDCH_REL_PHYCHAN_EST_FAIL) ||
         (eul_table_ptr->mac_hs_rach_info.mac_hsrach_edch_rsrc_rel_cause == WL1_HS_RACH_EDCH_REL_RL_FAILURE) ||
         (eul_table_ptr->mac_hs_rach_info.mac_hsrach_edch_rsrc_rel_cause == WL1_HS_RACH_EDCH_REL_AGCH_COLL_TIMER_EXPIRY)))
    {      
      mac_hsrach_phychan_estblish_failure_handler(ul_table_ptr, eul_table_ptr);
      WL2_MSG_1(HIGH, ul_table_ptr, "HS_RACH: RACH is released on L1 Resource Rel Ind %d",
                    eul_table_ptr->mac_hs_rach_info.mac_hsrach_edch_rsrc_rel_cause);
    } 
    else if (rach_info_ptr->hs_rach_state == RACH_IN_IDLE)
    {
      //buffer_staus_not_empty_flag = rlc_buffers_are_not_empty(&(mac_hs_ul_lc_info));
      buffer_staus_not_empty_flag = mac_hsrach_buffer_status(&data_present_on_srb, ul_table_ptr, eul_table_ptr);
      
      if (buffer_staus_not_empty_flag == TRUE)
      {
        /* If buffers are non empty - 
         *  - Step 1: ASC selection 
         *  - Step 2: Persistency Check 
         */
        selected_asc = mac_hs_rach_asc_selection(ul_table_ptr, eul_table_ptr);
        *asc_num = selected_asc;
        
        if(selected_asc < MAX_ASC)
        {
          persistency_result = mac_hs_rach_persistence_is_ok(selected_asc, ul_table_ptr, eul_table_ptr);
        }
        else
        {
          WL2_MSG_0(ERROR, ul_table_ptr, "Number of access service classes reached maximum number of access service classes supported by the UE");
        }
        
        if (persistency_result == TRUE)
        {
          mac_hsrach_srb_rxd_manage(data_present_on_srb, eul_table_ptr); 
          rach_info_ptr->npreamble_cycles = 1;
          rach_info_ptr->hs_rach_state = RACH_WAITING_FOR_STATUS;
        } 
        
        #ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
        rach_info_ptr->num_tti_consec_without_data = 0;
        #endif /*  FEATURE_WCDMA_TRIGGER_BASED_RACH  */
      }
      #ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
      else
      {  
        mac_check_for_bfi_disable_trigger(ul_table_ptr);
      }
      #endif /* #ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH */          
    }
    else
    {
      if (hs_rach_tx_control(ul_table_ptr, eul_table_ptr) == TRUE)
      {
        rach_info_ptr->hs_rach_state = RACH_WAITING_FOR_STATUS;
        mac_hsrach_srb_rxd_manage(data_present_on_srb, eul_table_ptr);
        persistency_result = TRUE;
        rach_info_ptr->npreamble_cycles++;
      }
      else
      {
        WL2_MSG_1(HIGH, ul_table_ptr, "MAC_HS_RACH: not ready to tx hs_rach_state:%d",
                                            rach_info_ptr->hs_rach_state);
      }
    }
    rach_info_ptr->data_present_on_srb = data_present_on_srb;    
    *srb_data_present = data_present_on_srb;    
    WL2_MSG_1(HIGH, ul_table_ptr, "FACH_QTA : SRB Data present : %d",*srb_data_present);
  }
  else
  {
    WL2_MSG_0(ERROR, ul_table_ptr, "HS_RACH: MAC HS RACH is NOT ACTIVE");
  }
  
  return (persistency_result);
}

void l1_mac_hsrach_preamble_proc_status(l1_hsrach_status_enum_type status, sys_modem_as_id_e_type as_id)
{
   uint8                       loop_index;
   mac_ul_table_type *ul_table_ptr;
   mac_rach_table_type *rach_info_ptr;
   mac_eul_table_type *eul_table_ptr;
   mac_hs_ul_logical_ch_mac_d_flow_info_type *hs_lc_macd_flow_map = NULL;

   l2_validate_as_id(as_id);

   eul_table_ptr = GET_MAC_EUL_TABLE_PTR(as_id);
   ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);   
   rach_info_ptr = &ul_table_ptr->rach_info;
   
   L2_MUTEX_LOCK(GET_WL1_EUL_ISR_DATAPATH_MUTEX(as_id));

   ul_table_ptr->hs_rach_tx_status[0] = status;
   hs_lc_macd_flow_map = get_mac_hs_logch_macd_flow_mapping_ptr(eul_table_ptr);
   for(loop_index = 0; loop_index < 2; loop_index++)
   {
     if (ul_table_ptr->hs_rach_tx_status[loop_index] != L1_HSRACH_MAC_INVALID)
     {
       if ((rach_info_ptr->hs_rach_state != RACH_IN_IDLE) && (rach_info_ptr->hs_rach_state == RACH_WAITING_FOR_STATUS))
       {
         if(eul_table_ptr->mac_hs_rach_rxd_enable == TRUE)
         {
           /* Disable RxD if *any* valid HSRACH status is received. 
            * For NAK/NOACK/ABORT, UL would start new HSRACH procedure later
            * where we would enable RxD if there is data pending Tx on SRBs */         
           mac_hsrach_srb_rxd_manage(FALSE, eul_table_ptr);
         }
         /* RACH is waiting for Status from Layer 1 */
         switch (ul_table_ptr->hs_rach_tx_status[loop_index])
         {
           case L1_HSRACH_ACK:
           {
              WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_HSRACH_ACK from L1");
              
              /* Send sucess indication to RRC */
              if ((L1_MAC_HS_RACH_CCCH_TRANS_TYPE == eul_table_ptr->mac_hs_rach_info.mac_hsrach_logch_tx_type) &&
                   (MAC_I_CHECK_SDU_SEG_NOT_PRESENT(hs_lc_macd_flow_map, eul_table_ptr->mac_hs_rach_info.mac_hsrach_ccch_rlc_id))) 
              {
                mac_send_tx_status_ind_to_rrc(ul_table_ptr->as_id, RACH_TX_SUCCESS);
              }

              mac_reset_rach(ul_table_ptr, eul_table_ptr);
           }
           break;
 
           case L1_HSRACH_NO_ACK:
           {
             WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_HSRACH_NO_ACK from L1");
 
             /* Preamble Ramping cycles exceeded maximum count */
             if (rach_info_ptr->npreamble_cycles >= eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.mmax)
             {
               WL2_MSG_2(HIGH, ul_table_ptr , "Reached max no. of Preamble cycles(%d)",
                               rach_info_ptr->npreamble_cycles, 
                               eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.mmax);
 
              /* Send CMAC_STATUS_IND to RRC */
               if (L1_MAC_HS_RACH_CCCH_TRANS_TYPE == eul_table_ptr->mac_hs_rach_info.mac_hsrach_logch_tx_type)
               {
                 mac_send_tx_status_ind_to_rrc (ul_table_ptr->as_id, MAX_RACH_CYCLES_REACHED);
               }

               mac_reset_rach(ul_table_ptr, eul_table_ptr);
               mac_hs_ul_clear_rlc_id_partial_info(eul_table_ptr->mac_hs_rach_info.mac_hsrach_ccch_rlc_id, ul_table_ptr->as_id);
               rlci_enh_ul_hsrach_flush_tm_pdus(ul_table_ptr->as_id, eul_table_ptr->mac_hs_rach_info.mac_hsrach_ccch_rlc_id);//Flush the RLC CCCH DSM item]
             }
             else
             {
               /* Set RACH state to RCVD_NO_ACK */
               rach_info_ptr->hs_rach_state = RACH_RCVD_NOACK;
             }
           }
           break;
 
           case L1_HSRACH_NACK:
           {
 
             WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_HSRACH_NAK from L1");
 
             /* Preamble Ramping cycles exceeded maximum count */
             if (rach_info_ptr->npreamble_cycles >= eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.mmax)
             {
                WL2_MSG_2(HIGH, ul_table_ptr , "Reached max no. of Preamble cycles(%d)",
                         rach_info_ptr->npreamble_cycles, eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.mmax);
 
              /* Send CMAC_STATUS_IND to RRC */
              if (L1_MAC_HS_RACH_CCCH_TRANS_TYPE == eul_table_ptr->mac_hs_rach_info.mac_hsrach_logch_tx_type)
              {
                mac_send_tx_status_ind_to_rrc (ul_table_ptr->as_id, MAX_RACH_CYCLES_REACHED);
              }

              /* Reset MAC */
               mac_reset_rach(ul_table_ptr, eul_table_ptr);
               mac_hs_ul_clear_rlc_id_partial_info(eul_table_ptr->mac_hs_rach_info.mac_hsrach_ccch_rlc_id, ul_table_ptr->as_id);
               rlci_enh_ul_hsrach_flush_tm_pdus(ul_table_ptr->as_id, eul_table_ptr->mac_hs_rach_info.mac_hsrach_ccch_rlc_id);//Flush the RLC CCCH DSM item
             }
             else
             {
 
               /* Set RACH state to Backoff */
               rach_info_ptr->hs_rach_state = RACH_IN_BACKOFF;
 
               /* set backoff counter value */
 
               rach_info_ptr->backoff_val =
                  ran_dist(mac_fetch_random_number(eul_table_ptr->as_id), (uint16)eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.nbo1min, (uint16)(eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.nbo1max + 1));
 
               WL2_MSG_3(HIGH, ul_table_ptr, "Backoff_Val %d, lo_val %d, hi_val %d",
                                         rach_info_ptr->backoff_val, 
                                         eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.nbo1min, 
                                         eul_table_ptr->mac_hs_rach_info.hs_rach_cntl_info.nbo1max); 
             }
           }
           break;
 
	 case L1_HSRACH_ABORT_QTA_OR_FMO:
         {
           WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_HSRACH_ABORT_QTA_OR_FMO from L1");                       
           rach_info_ptr->hs_rach_state = RACH_RCVD_NOACK;
           rach_info_ptr->npreamble_cycles--;
         }
         break;

         case L1_HSRACH_ABORT:
          {
            WL2_MSG_0(HIGH, ul_table_ptr , "rcvd L1_HSRACH_ABORT from L1");
            rlc_enh_indicate_ul_data_trash(ul_table_ptr->as_id);
            mac_reset_rach(ul_table_ptr, eul_table_ptr);
          }
          break;
 
          default:
             WMAC_ERR_FATAL (eul_table_ptr->as_id, "rcvd invalid Tx status (%d)", ul_table_ptr->hs_rach_tx_status[loop_index], 0, 0);
           break;
         }
       }
       else
       {
         WL2_MSG_1(ERROR, ul_table_ptr, "MAC_HS_RACH: RACH is either in IDLE or not in WAITING for response State - Not expected response %d",
                                  ul_table_ptr->hs_rach_tx_status[loop_index]);
       }       
       ul_table_ptr->hs_rach_tx_status[loop_index] = L1_HSRACH_MAC_INVALID;      
     }
   }

   L2_MUTEX_UNLOCK(GET_WL1_EUL_ISR_DATAPATH_MUTEX(as_id));
}

/*================================================================================================
FUNCTION NAME rach_tx_control

DESCRIPTION   This function controls RACH transmission and is called once every TTI
================================================================================================*/
boolean   hs_rach_tx_control(mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr)
{
  mac_rach_table_type *rach_info_ptr;
  boolean   rach_tx_is_ok = FALSE;
  uint8     selected_asc = 0;

  rach_info_ptr = &ul_table_ptr->rach_info;
  
  switch (rach_info_ptr->hs_rach_state)
  {
    case RACH_IN_BACKOFF:
      if (rach_info_ptr->backoff_val == 0)
      {
        WL2_MSG_0(HIGH, ul_table_ptr, "HS_RACH: Backoff Tmr expired");
        selected_asc = mac_hs_rach_asc_selection(ul_table_ptr, eul_table_ptr);
        if ((selected_asc < MAX_ASC) && (mac_hs_rach_persistence_is_ok(selected_asc,ul_table_ptr, eul_table_ptr)) == TRUE)
        { 
          rach_tx_is_ok = TRUE;          
        }
        else
        {
          rach_info_ptr->hs_rach_state = RACH_PERSISTENCE_FAILED;
        }
      }
      else
      {
        WL2_MSG_1(HIGH, ul_table_ptr, "HS_RACH: Backoff Val %d > 0",
                                                   rach_info_ptr->backoff_val);
        rach_info_ptr->backoff_val--;
      }
      break;
    
    case RACH_RCVD_NOACK:
    case RACH_PERSISTENCE_FAILED:
      selected_asc = mac_hs_rach_asc_selection(ul_table_ptr, eul_table_ptr);
      if ((selected_asc < MAX_ASC) && (mac_hs_rach_persistence_is_ok(selected_asc, ul_table_ptr, eul_table_ptr)) == TRUE)
      {
        rach_tx_is_ok = TRUE;
      }
      else
      {
        rach_info_ptr->hs_rach_state = RACH_PERSISTENCE_FAILED;
      }
      break;
    
    case RACH_WAITING_FOR_STATUS:
      break;
    
    default:
      WL2_MSG_1(HIGH, ul_table_ptr , "HS_RACH: Invalid RACH state %d",
                                                  rach_info_ptr->hs_rach_state);
      break;
  }

  return(rach_tx_is_ok);
}
#endif

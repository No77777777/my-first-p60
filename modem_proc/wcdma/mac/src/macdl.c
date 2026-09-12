/*========================================================================================

                D O W N L I N K   M A C

GENERAL DESCRIPTION

  This module contains the declaration of the data types and functions
  needed to process DL transport blocks

Copyright (c) 2001 by Qualcomm Technologies Incorporated.  All Rights Reserved.
Copyright (c) 2002 - 2009 by Qualcomm Technologies Incorporated.  All Rights Reserved.

========================================================================================*/

/*===========================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macdl.c_v   1.6   11 Jun 2002 17:43:38   skrishna  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/src/macdl.c#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when      who   what, where, why
--------  ---   --------------------------------------------------------------------------
11/12/15  as    Changes to process HS data only for the W sub for which HS is active
10/13/15  as    Code changes to clear MAC EHS partial queue and reordering queue correctly 
                to avoid crash during L1 flush and T1_timer expiry.
07/16/15  aa    L2 to pass/process sub-id correctly when DUAL_WCDMA feature is disabled.
06/26/15  aa    Removing extern declarations from L2 code
06/08/15  as    W+W MAC UL/DL HS task variable doubling.
06/01/15  scv   Code changes to support QSH
05/01/15  scv   W+W Changes to have log packets support SubID
04/25/15  scv   W+W Changes to have messages support SubID
03/10/15  aa    Duplicated L2 task related globals and removed AS_ID hardcodings
01/29/15  kc    L2 global reorg for W+W
01/26/15  ac    Datapath global reorg for X+W
07/26/14  rkmk  Reduce image size by using right macro depending upon number of parameters
05/03/14  geg   Use safer versions of memcpy() and memmove()
12/09/13  as    Code change to EDRX full sleep if there pending partials at MAC. 
12/05/13  aa    Fixed compilation errors for off-target test framework (C++ to C switch)
05/03/13  as    Change to discard DL PDUs if PDU health passed by decoder is false
01/17/13  as    Removed unwanted F3.
01/04/13   kc   Removing duplicate/redundant code
08/17/12  grk   Irrespective of mac_e_ehs_type value,when RRC sends HS-STOP -clear the L2 mac-hs-dl config
08/09/12  grk   Fix to process DL R99 and HS config signals separately.
07/13/12  grk   MBMS feature cleanup at L2(MBMS_DEBUG_1, FEATURE_MODEM_MBMS)
08/13/12  mrg   HS-RACH Tb timer restart when MAC-ehs DL data recieved
07/24/12  geg   Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
03/28/12  zr    Adding feature definitions for Dime
06/19/12  mrg   L2 - MAC-I and HS-RACH code changes
01/02/12  grk   Feature cleanup
01/23/12  kc    Changes for free floating tasks
09/05/11  grk   Fix to consider SRBs as highest priority during TFCI selection.
06/02/11  kc    Added support for logical channel to transport channel mapping info
05/12/11  grk   Added correct array bound check for no. of TBs in data read.
03/09/11  ssg   MAC-ehs support on Apollo.
12/20/10  grk    WCDMA_INTLOCK() cleanup.
12/15/10  vp    Fixed compiler warning
11/30/10  pj    L2 mutex cleanup
10/18/10  ssg   Added changes for new WCDMA-L2 interface.
11/2/10   kvk   Fixed compile warning
10/25/10  grk   Fixed featurization issue.
10/19/10  kvk   Added support to flush the TSNs when HS is stopped
09/19/10  av    HS_FACH and Enhanced 0x4220 log changes
09/07/10  mrg   Fixed Klockwork warning - buffer overflow 
05/25/10  grk   Fixed compilation error.
05/24/10  grk   Fix to pass correct hdr_info incase PDUs are invalidated during header 
                read.
03/24/10  prk   Fixed compilation error.
03/23/10  kvk   Introduced new mutex varibale mac_hs_cfg_mutex to exclusively protect the data read isr 
                and cfg handler.
05/14/10  grk   Fixed KW warning
05/06/10  grk   Fixed compiler warning
04/30/10  prk   changes for MAC QTF.
04/13/10  grk   changes for ZI memory reduction.
03/31/10  prk   chaging the featurization in l1_phy_dl_trblk_hs_data_ind.
01/20/10  prk   Removing mac_gcf_test_flag
11/19/09  kvk   Fixed compile error on Non DL data path 
11/16/09  av    Fixing mob compilation error
10/28/09  kvk   Added support for A2 and seperated the MODEM ACCEL specific code into
                FEATURE_WCDMA_DL_MODEM_ACC
10/20/09  mrg   Deleting staled PDUs which are 10 TTIs crossed
                in UL queue for loopback mode-1
10/05/09  prk   Fixed Klocwork warnings
09/01/09  kvk   Fixed compile error
08/21/09  grk   Fixed compiler warning.
08/14/09  kvk   Copy src desc list instead dec_src_descriptor_move_in if 
                FEATURE_WCDMA_PP_HW_DL in header read ind
08/12/09  kvk   Moved dec_dst_descriptor_move_in access under 
                FEATURE_WCDMA_PP_HW_DL
08/10/09  kvk   Increased fake DOB ext buf size to 25k
07/28/09  kvk   Added support for p-t-c enhacements(p-t-c functionality handled in MAC HS task)
07/21/09  ssg   Added fix to store DL-CFN in "app_ptr" only for LB-2, TM.
07/09/09  mrg   Lint errors fixed
07/02/09  mrg   Coade added under feature FEATURE_TC_CONSTANT_DELAY_ENH
                for the TS 34.121 TC 7.9.1A fix
                with this fix we are maintaing a constant delay for the LB-2, 
                if the delay corssed more than 10 TTIs we are dropingthe SDUs.
06/26/08  av    l1-l2 interface cleanup 
05/22/09  prk   Fixed Lint errors 
05/14/09  mrg   When Partial Q is empty and received TSN has Complete+Partial PDUs, then 
                moving PC PDUs to RLC code logic now under GCF feature flag- 
                To avoid 0x421E logging problems.
05/07/09  ssg   Updated Copyright Information
04/30/09  av    Fixed MOB errors 
04/29/09  av    Fixed compiler warnings in L2 
03/11/09  pj    Added code changes to have UL/DL-task & EUL-ISR concurrency 
                protection under light-weight mutex
02/05/09  kvk   Added mutex protection to avoid concurrenct between p-t-c handling and 
                flushing the partial TSN Q in MAC HS due to MAC hs reset on MAC HS task context 
01/22/09  grk   Merged L2 mutex changes from taxis to mainline.
12/18/08  ksr   L2 Mutex changes to avoid concurrency issues
10/17/08  grk   Merged taxis changes to mainline.
09/03/08  ksr   Changes for PP_HW_DL and log_status check addition
10/10/08  grk   Lint warning fixes.
10/03/08  ssg   Added fix to set LSB(app_field)=1 for TM PDUs with out modifying other bits.
10/03/08  ssg   Added fix to not set app_field=1 for TM PDUs.
09/12/08  mrg   log_status() check is added to avoid unnecessary memory 
                allocation and log packet preperation.
04/03/08  sk    Renamed FEATURE_MBMS to FEATURE_MODEM_MBMS
03/24/08  pj    Added support for data path optimizations under
                FEATURE_WCDMA_UL_DATA_PATH_OPT & FEATURE_WCDMA_DL_DATA_PATH_OPT
          ksr   1. MAC DL Changes for DL Data path optimizations
          sm    2. a. Updated code with correct understanding of l1_dl_tb_data_struct_type->num_tb_set
                      and l1_dl_tb_data_struct_type->trch_ref_type
                   b. Fill rlc_hdr_data correctly for R99 path and update dsm variable accordingly
                   c. Always check for queue tsn_info_q in l1_phy_dl_trblk_data_ind() because
          vg    3. Set complete_pdu_ptr->iovec.length in 'bits' for TM mode lc_id's in
                   l1_phy_dl_trblk_data_ind() because RLC expects length in bits for TM lc_id's
01/29/08  sk    Support for MBMS under FEATURE_MODEM_MBMS
01/04/07  ssg   Indicate RLC about the completion of downlink PDU header processing to
                set downlink cipher PDU packets logging signal.
08/24/06  gsc   Converted Error Fatals to MSG Errors.
07/18/06  gsc   Fix for CR 97673
04/26/06  gsc   Added support for HSDPA re-config optimizations
03/12/06  sk    Removed dl_ciph_pending flag. It was not being used anywhere.
11/10/05  sk    Code added to handle the excution of Rel5 functionality i.e HSDPA related
                functionality only when the NV switch is turned on.
11/03/05  sk/gsc Added function proc_dl_mac_config_at_l1_failure_signal(). This function
                processes the config signal received from the physical layer.
09/21/05  gsc   Fixed coverity error.
09/09/05  ttl   Fix compilation error.
09/09/05  gsc   Fix for CR 75199. Fix for the issue where if mac_hs state was incorrectly
                being set to "invalid" upon receiving incorrect HS action from RRC.
08/08/05  gsc   Removed code to handle signal from MAC-HS after MAC-HS config is completed.
                This also fixes 66149.
07/06/05  gsc   Change MAC-HS current state to new at the activation time. Fix
                for CR 68687.
06/27/05  gsc   Reverted to previous revision (rev 26).
06/16/05  gsc   Added a check to only log HS configuration if there is any MAC-HS config change.
04/20/05  gsc   Update tb_id only for non HS-DSCH transport channel type.
03/07/05  gsc   Lint Fixes
02/17/05  gsc   Completed implementation of mac_hs_reset as per discussions
                with L1 folks.
02/15/05  gsc   Added HSDPA support under FEATURE_HSDPA feature flag.
02/09/05  sk    Fix for lint errors.
10/28/04  sk    Added a boolean cfn_marked. This flag is initialized at the beginnign of header processing.
                Once a transport block is received on DCH trch for which loopback is enabled then set this
                flag to TRUE. Hence mark the cfn at which the pdu is recieved only once.
06/04/04  sk    Print the messages only if ciphering is enabled.
06/03/04  sk    Added some debug messages for ciphering purposes
12/12/03  sk    Fix for CR 36875
                Remove unnecessary processing for dl_configuration_pending in proc_dl_mac_commands()
04/29/03  sk    Intialiazes the lb_delay_type structure.This function is called at the time of
                MAC initialization. Defined under FEATURE_TC_CONSTANT_DELAY.
01/31/03  sk    proc_dl_cipher_info(): Check to see if the cipher_ptr is NULL or not.
12/13/02  sk    Added #include macdlsubtask.h to remove link errors.
09/26/02  sk    add a check to see if ciperhing was previously enabled.
09/04/02  sk    * config_ul_mac() : Call proc_dl_cipher_info() by passing an array
                  of 2 mac_cipher_config_type structures.
                * init_dl_mac() : Initialize the structures needed for ciphering
                * proc_dl_cipher_info() : Copy the ciphering configuration for
                  both CS and PS domain based on whether the ciphering is enabled or not.
                  These are done under FEATURE_UMTS_CIPHERING_ENABLE
08/24/02  sk    Removed function maci_post_ul_cfg_cnf_sig(). MAC DL posts the config signal
                directly to L1.
08/19/02  tliou To fix the DSM item being over-written in the DL when the RLC DL does not
                have enough time to process the passed-in PDU from MAC.
08/02/02  sk    Commented the function void proc_dl_cipher_info (mac_cipher_config_type *cipherptr).
                config_dl_mac(): Commented calling of function proc_dl_cipher_info();
                proc_dl_mac_commands(): Included a check to switch to the new config based
                on the flag wait_for_l1_sig set by RRC.
06/02/02  sk    In proc_dl_mac_commands(): Copy the configuration parameters that MAC gets
                from RRC to local memory. COnfigure MAC only when it gets a signal from
                PHYSICAL layer. Added functions to process physical leyr signal
                proc_dl_mac_config_signal(). Post a config confirm signal to UL MAC using the
                function maci_post_ul_config_signal().
11/26/01  ps    Added additional checks for PDU size & PDU pointer in l1_phy_dl_trblk_data_ind()
08/21/01  rc    Added support for QXDM Logging.
08/07/01  ps    Fixed the leading zero message format problem
06/18/01  ps    Adjusted MSG levels
5/2/01    ps    Changed few functions to MACROs
4/3/01    ps    Initial release after merging "macdlconfig.h", "macdlhdr.h" and "macdata.h"

===========================================================================================*/

/*===========================================================================================
                     INCLUDE FILES
===========================================================================================*/
#include "wcdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "dsm.h"
#include  "task.h"
#include  "macrrcif_v.h"
#include  "macrlcif.h"
#include  "l1sapcommon.h"
#include  "l1mathutil.h"
#include  "l1macdata.h"
#include  "l2dltask.h"
#include  "macinternal.h"
#include  "macdl.h"
#include  "macbch.h"
#include  "macpch.h"
#include  "macfach.h"
#include  "macdldch.h"
#include  "macdsch.h"
#include  "macdsch.h"
#include  "macdlhs.h"
#include  "macdlsubtask.h"
#include  "l2ultask.h"

 #include "macdllog.h"

  #include "maculdl.h"

#include "macdlhs.h"
#include "machstask.h"
#include "machslog.h"
#include "err.h"

#include "l1macif.h"

#include "mcalwcdma_dec.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif

/*===========================================================================================
                            DATA DECLARATIONS
===========================================================================================*/

mac_dl_table_type mac_dl_table[L2_NUM_W_SUBS];

/*---------------------------------------------------------------------------
 *   Flag used to determine the test is related to GCF testing or not.
 *-------------------------------------------------------------------------*/
boolean         mac_gcf_test_flag = FALSE;

/*============================================================================================
 FUNCTION NAME  init_dl_mac()

 DESCRIPTION  This function initializes the variables needed for ciphering

============================================================================================*/
void init_dl_mac(sys_modem_as_id_e_type as_id)
{
  uint8 i;
  mac_dl_table_type *dl_table_ptr;

  dl_table_ptr = GET_MAC_DL_TABLE_PTR(as_id);

  dl_table_ptr->as_id = as_id;
 
  for (i=0;i<MAX_CN_DOMAIN;i++)
  {
    /* Clear the memory */
    dl_table_ptr->cur_dl_cipher_info[i].cipher_enable = FALSE;
    dl_table_ptr->new_dl_cipher_info[i].cipher_enable = FALSE;
    dl_table_ptr->cur_dl_cipher_info[i].key_idx = 0xFF;
    dl_table_ptr->new_dl_cipher_info[i].key_idx = 0xFF;
  }

#ifdef FEATURE_TC_CONSTANT_DELAY
  /* Initialize the first_tb boolean to TRUE so that UL MAC would use this
  flag to calculate the delay and subsequently this constant delay would
  be maintained for transmitting any block in the UL */
  dl_table_ptr->lb_delay_type.first_tb = TRUE;
  dl_table_ptr->lb_delay_type.start_idx = 0;
  dl_table_ptr->lb_delay_type.cur_idx = 0;
#endif

  dl_table_ptr->cfn_marked = FALSE;
  dl_table_ptr->dl_configuration_pending = FALSE;
  dl_table_ptr->dl_mac_crnti_valid = FALSE;
  dl_table_ptr->dl_mac_urnti_valid = FALSE;
  
  dl_table_ptr->ciphering_garbled = TRUE;
  dl_table_ptr->mac_dl_debug_print_interval = 50;
  dl_table_ptr->prev_print_cfn = 0xFF;
}

/*===========================================================================================
 FUNCTION NAME  proc_dl_cipher_info

 DESCRIPTION  This function processes the Cipher information received from RRC.

============================================================================================*/
void    proc_dl_cipher_info
(
  mac_cipher_config_type   cipherptr[],       /*Pointer to DL MAC Ciphering Config Info */ 
  mac_dl_table_type *dl_table_ptr
)
{
  uint8  i;
  mac_ul_table_type *ul_table_ptr;
  mac_ul_cipher_table *mac_cipher_info_ptr;
  
  ul_table_ptr = GET_MAC_UL_TABLE_PTR(dl_table_ptr->as_id);
  mac_cipher_info_ptr = &ul_table_ptr->mac_cipher_info;
  
  if (cipherptr==NULL)
  {
    WL2_MSG_0(ERROR, dl_table_ptr, "DL Cipher ptr is NULL");
  }
  else
  {
    for (i=0; i<MAX_CN_DOMAIN ; i++)
    {
      if (cipherptr[i].cipher_enable)
      {
        /*-----------------------------------------------------------------------------
        If the ciphering was previously diabled and if the ciphering has been enabled
        now, then save the ciphering parameters given by RRC.
        Check if the ciphering parameters have changed. MAC knows if the parameters
        have changed by comparing the new key index with the one already exisiting
        in MAC. If the parameters have changed, then copy them to new_ul_cipher_info
        and switch to the new configuration at ciphering activation time which is
        provided by the CFN number.
        ---------------------------------------------------------------------------*/
        if (cipherptr[i].key_idx != dl_table_ptr->cur_dl_cipher_info[i].key_idx)
        {
          // Copy the new ciphering parametrs received.
          WCDMA_MEMCPY(&dl_table_ptr->new_dl_cipher_info[i],
                       sizeof(mac_cipher_config_type),
                       &cipherptr[i],
                       sizeof(mac_cipher_config_type));

          // Indicate that there is a new config to which MAC has to switch to at
          // ciphering activation time.
          if (!dl_table_ptr->cur_dl_cipher_info[i].cipher_enable)
            WCDMA_MEMCPY(&dl_table_ptr->cur_dl_cipher_info[i],
                         sizeof(mac_cipher_config_type),
                         &cipherptr[i],
                         sizeof(mac_cipher_config_type));

        } // ciphering parameters have changed

      }// ciphering is enabled
      else
      {

        /* Clear the memory */
        dl_table_ptr->cur_dl_cipher_info[i].cipher_enable = FALSE;
        dl_table_ptr->new_dl_cipher_info[i].cipher_enable = FALSE;
        dl_table_ptr->cur_dl_cipher_info[i].key_idx = 0xFF;
        dl_table_ptr->new_dl_cipher_info[i].key_idx = 0xFF;

        if (i == 0)
        {
          memset(mac_cipher_info_ptr->cfn_ciphering_table, 0x00, 256 * sizeof(mac_ciphering_table));
        }
      }
    }
  }
}

/*============================================================================================
 FUNCTION NAME  proc_dl_rnti_info

 DESCRIPTION  This function processes the RNTI information received from RRC.

============================================================================================*/
void    proc_dl_rnti_info
  (
/* Pointer to DL RNTI Config Info */
  mac_rnti_config_type   *rntiptr, mac_dl_table_type *dl_table_ptr
  )
{
  /* Process CRNTI info based on RNTI valid flag */
  switch (rntiptr->rnti_valid)
  {

  /* C-RNTI is valid */
  case CRNTI_VALID:
    dl_table_ptr->dl_mac_crnti_valid = TRUE;
    dl_table_ptr->dl_mac_urnti_valid = FALSE;
    dl_table_ptr->dl_mac_crnti = rntiptr->crnti;
    WL2_MSG_1(LOW, dl_table_ptr, "C-RNTI = %x",dl_table_ptr->dl_mac_crnti);
    break;
    
    /* U-RNTI is valid */
  case URNTI_VALID:
    dl_table_ptr->dl_mac_urnti_valid = TRUE;
    dl_table_ptr->dl_mac_crnti_valid = FALSE;
    dl_table_ptr->dl_mac_urnti = rntiptr->urnti;
    WL2_MSG_1(LOW, dl_table_ptr, "U-RNTI = %x",dl_table_ptr->dl_mac_urnti);
    break;
    
    /* C-RNTI and U-RNTI are valid */
  case BOTH_VALID:
    dl_table_ptr->dl_mac_urnti_valid = TRUE;
    dl_table_ptr->dl_mac_crnti_valid = TRUE;
    dl_table_ptr->dl_mac_crnti = rntiptr->crnti;
    dl_table_ptr->dl_mac_urnti = rntiptr->urnti;
    WL2_MSG_1(LOW, dl_table_ptr, "C-RNTI = %x",dl_table_ptr->dl_mac_crnti);
    WL2_MSG_1(LOW, dl_table_ptr, "U-RNTI = %x",dl_table_ptr->dl_mac_urnti);
    break;
    
    /* No valid RNTI */
  case NO_RNTI_VALID:
    dl_table_ptr->dl_mac_urnti_valid = FALSE;
    dl_table_ptr->dl_mac_crnti_valid = FALSE;
    WL2_MSG_0(LOW, dl_table_ptr,"No valid RNTI");
    break;
    
    /* Invalid RNTI valid flag */
  default:
    WL2_MSG_0(HIGH, dl_table_ptr, "Invalid DL RNTI Config");
    break;
  }
}


/*=============================================================================================
FUNCTION NAME config_dl_mac

DESCRIPTION   This function processes DL MAC Configaration Data received from RRC

=============================================================================================*/
void    config_dl_mac
  (
/* Pointer to DL MAC Config Data */
  mac_dl_config_type *cfgptr, mac_dl_table_type *dl_table_ptr, mac_hs_table_type *hs_table_ptr
  )

{
  if ( hs_table_ptr->mac_hs_dl_config.operation.save_non_hs_params)
  {
    WL2_MSG_0(HIGH, dl_table_ptr, "starting non-hs config");
    
    L2_ACQ_DL_LOCK();

    /* Process RNTI information */
    proc_dl_rnti_info (&cfgptr->rnti_info, dl_table_ptr);

    /* Process Ciphering information */
    proc_dl_cipher_info (cfgptr->cipher_info, dl_table_ptr);

    /* Process BCH configuration data */
    process_bch_config (cfgptr->num_bchs, &cfgptr->bch_info[0], dl_table_ptr);

    /* Process PCH configuration data */
    process_pch_config (cfgptr->num_pchs,&cfgptr->pch_info[0], dl_table_ptr);

    /* Process FACH configuration data */
    process_fach_config (cfgptr->num_fachs,&cfgptr->fach_info[0], dl_table_ptr);
    
    /* Process DCH configuration data */
    process_dl_dch_config (cfgptr->num_dchs,&cfgptr->dch_info[0], dl_table_ptr);

    /* Process DSCH configuration data */
    process_dsch_config (cfgptr->num_dschs,&cfgptr->dsch_info[0], dl_table_ptr);

    L2_ACQ_DL_UNLOCK();
    /* Send MAC Configuration Data to QXDM*/
    mac_dl_send_cfg_log_packet(cfgptr, dl_table_ptr);

    L2_ACQ_DL_LOCK();

    hs_table_ptr->mac_hs_dl_config.operation.save_non_hs_params = FALSE;

    L2_ACQ_DL_UNLOCK();
    WL2_MSG_0(HIGH, dl_table_ptr, "ending non-hs config");
  }
  else
  {
    WL2_MSG_0(HIGH, dl_table_ptr, "no NON HS config");
  }
}


 /*============================================================================================
 FUNCTION NAME  l1_phy_dl_trblk_hs_data_ind

 DESCRIPTION  This function processes DL HS transport blocks received from 
              PHY layer as part of PP DL HW handling

 CONTEXT    Called by PHY layer MDM in interrupt context 
            (seperate from regular Data ISR which is only for R99 and non PP HS call)
=============================================================================================*/
void l1_phy_dl_trblk_hs_data_ind(l1_dl_hs_tti_enum_type tti_type, sys_modem_as_id_e_type wsub_id)
{
  q_type *dec_tsn_info_q_ptr = mcalwcdma_decdata_get_tsn_info_q();
  q_type *dec_concatenated_pdu_tsn_q_ptr = mcalwcdma_decdata_get_concatenated_pdu_tsn_q();

  uint8   mac_hs_num_tsn_rcvd = 0;
  mac_hs_table_type *hs_table_ptr;

  l2_validate_as_id(wsub_id);
  hs_table_ptr = GET_MAC_HS_TABLE_PTR(wsub_id);
  
  if ((tti_type == MACDL_HS_CURR_TTI) || tti_type == MACDL_HS_CURR_PREV_TTI)
  {
    if (q_cnt(dec_tsn_info_q_ptr) > 0)
    {
      if ((hs_table_ptr->mac_hs_dl_curr_state != MAC_HS_ACTIVE)
           && !hs_table_ptr->mac_hs_reconfig_in_progress)
      {
        WL2_MSG_1(ERROR, hs_table_ptr, "MAC_ERR: HS_DSCH not active; HS State = %d", hs_table_ptr->mac_hs_dl_curr_state);
      }
      else
      {
        REX_ISR_LOCK(&mac_mutexes[wsub_id].mac_hs_cfg_mutex);
        process_dl_hs_enh_dsch_tbs_data(&mac_hs_num_tsn_rcvd, hs_table_ptr);
        REX_ISR_UNLOCK(&mac_mutexes[wsub_id].mac_hs_cfg_mutex);
      }
    }
    /*
       If the TTI is current and if there are TSN which needs to be processed signal MAC HS task
    */
    if ((tti_type == MACDL_HS_CURR_TTI) && (mac_hs_num_tsn_rcvd > 0))
    {
      (void) rex_set_sigs(wcdma_mac_hs_dl_get_tcb(hs_table_ptr), MAC_HS_DL_DATA_IND_SIG);
    }
  }
    
  /*
   * If any useful TSN rcvd or there are any P-to-C present, 
   * Signal the mac_dl_hs task to pick up the processing
   */
  if ((tti_type == MACDL_HS_PREV_TTI) || tti_type == MACDL_HS_CURR_PREV_TTI)
  {
    if (
#ifdef FEATURE_HSDPA_MAC_EHS
        (q_cnt(dec_concatenated_pdu_tsn_q_ptr) > 0) ||
#endif
        (mac_hs_num_tsn_rcvd > 0))
    {
      hs_table_ptr->mac_hs_dl_rlc_pdu_concatenate_info.num_complete_pdus = 0;

      (void) rex_set_sigs(wcdma_mac_hs_dl_get_tcb(hs_table_ptr), MAC_HS_DL_DATA_IND_SIG);
    }
  }
}


/*============================================================================================
 FUNCTION NAME  l1_phy_dl_flush_tsn

 DESCRIPTION  This function will flush the TSNs based on L1 request

 CONTEXT    Called by PHY layer in interrupt context
=============================================================================================*/
void  l1_phy_dl_flush_tsn(sys_modem_as_id_e_type wsub_id)
{
  q_type *dec_tsn_info_q_ptr = mcalwcdma_decdata_get_tsn_info_q();
  q_type *dec_concatenated_pdu_tsn_q_ptr = mcalwcdma_decdata_get_concatenated_pdu_tsn_q();
  q_type *dec_concatenated_pdu_tsn_free_q_ptr = mcalwcdma_decdata_get_concatenated_pdu_tsn_free_q();

  l1_dec_hs_tsn_info_type *tsn_info_ptr;
#if (((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7))) )
  mcalwcdma_dec_concatenated_pdu_tsn_type *concatenated_pdu_tsn_info_ptr = NULL;
  int i;
#endif
  mac_hs_table_type *hs_table_ptr;
  mac_mutexes_type *mac_mutex_ptr;

  l2_validate_as_id(wsub_id);
  hs_table_ptr = GET_MAC_HS_TABLE_PTR(wsub_id);
  mac_mutex_ptr = MAC_GET_MUTEX_PTR(wsub_id);
  /*Lock the mutex*/
  L2_MUTEX_LOCK(&(mac_mutex_ptr->mac_hs_tsn_flush_mutex));

  /*Go over the dec_tsn_info_q*/
  while ((tsn_info_ptr = (l1_dec_hs_tsn_info_type *) q_get(dec_tsn_info_q_ptr)) != NULL)
  {
    mac_hs_dl_discard_tsn(wsub_id, tsn_info_ptr);
    mac_hs_enqueue_free_tsn(wsub_id, tsn_info_ptr);
  }
  
  L2_MUTEX_UNLOCK(&(mac_mutex_ptr->mac_hs_tsn_flush_mutex));

  /*Flush reordering q*/
  L2_ACQ_DL_LOCK();
  mac_hs_tsn_empty_recovery(hs_table_ptr);
  L2_ACQ_DL_UNLOCK();
  
#if (((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7))) )
  REX_ISR_LOCK(&(mac_mutex_ptr->mac_ehs_recfg_mutex));
  for (i = 0; i < MAC_HS_DL_MAX_QUEUES; i++)
  {
    while ((tsn_info_ptr = (l1_dec_hs_tsn_info_type *) q_get(&hs_table_ptr->mac_partial_tsn_q[i])) != NULL)
    {
      mac_hs_dl_discard_tsn(wsub_id, tsn_info_ptr);
      mac_hs_enqueue_free_tsn(wsub_id, tsn_info_ptr);
    }
  }
  /*Flush dec concantanated q*/
  while ((concatenated_pdu_tsn_info_ptr = 
              (mcalwcdma_dec_concatenated_pdu_tsn_type *) q_get(dec_concatenated_pdu_tsn_q_ptr)) != NULL)
  {
    concatenated_pdu_tsn_info_ptr->tsn_info_buf = NULL;
    q_put(dec_concatenated_pdu_tsn_free_q_ptr, &(concatenated_pdu_tsn_info_ptr->link));
  }
  REX_ISR_UNLOCK(&(mac_mutex_ptr->mac_ehs_recfg_mutex));
#endif

  /*Flush RLC tsn info q*/
  rlc_enh_dl_tsn_flush_hs(wsub_id);
}

/*============================================================================================
 FUNCTION NAME  l1_phy_dl_trblk_data_ind

 DESCRIPTION  This function processes DL transport blocks received from PHY layer

 CONTEXT    Called by PHY layer in interrupt context
=============================================================================================*/
void    l1_phy_dl_trblk_data_ind
  (
/* Pointer to TB Data Information */
  l1_dl_tb_data_struct_type *dl_tb_data_info,
  sys_modem_as_id_e_type     as_id
  )
{
  q_type *dec_tsn_info_q_ptr = mcalwcdma_decdata_get_tsn_info_q();
  q_type *dec_concatenated_pdu_tsn_q_ptr = mcalwcdma_decdata_get_concatenated_pdu_tsn_q();

  uint8   i,j;

  /* TB index */
  uint8   tb_index;

  uint8   mac_hs_num_tsn_rcvd = 0;


  dl_tb_info_type   *dl_tb_info_ptr;

  /* Pointer to Transport Block Set info */
  l1_dl_tb_set_data_struct_type   *tbsptr = NULL;

  /* Pointer to Transport Block Data info */
  l1_dl_tb_data_in_tbset_struct_type  *tbptr;

  mac_dl_table_type *dl_table_ptr;
  mac_hs_table_type *hs_table_ptr;
  mac_ul_table_type *ul_table_ptr;

  /* Number of transport block sets */
  uint8   num_tb_set;

  uint16 length;
  boolean first = TRUE;
  complete_pdu_type *complete_pdu_ptr;
  rlc_enh_data_ind_type *rlc_enh_data_ind_ptr = NULL;
  boolean is_tm_mode = FALSE;
  uint8 num_tb = 0;
#ifdef FEATURE_TC_CONSTANT_DELAY_ENH
  uint8 cfn_idx = 0;
#endif

   l2_validate_as_id(as_id);
   
   dl_table_ptr = GET_MAC_DL_TABLE_PTR(as_id);
   hs_table_ptr = GET_MAC_HS_TABLE_PTR(as_id);
   ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);

  /* TB Data Info Pointer is NULL */
  if (dl_tb_data_info == NULL)
  {
    WL2_MSG_0(HIGH, dl_table_ptr, "TB Data info pointer is NULL");
  }
  /* Valid number of TB Data Blocks */
  else
  {
    /* Number of transport block sets */
    num_tb_set = dl_tb_data_info->num_tb_set;

    /* Process each TBS Data info */
    for (i=0 ;i <num_tb_set; i++)
    {

      /* Pointer to transport block set */
      tbsptr = &dl_tb_data_info->tb_set_data[i];

      /* TBS Data pointer is NULL */
      if (tbsptr == NULL) /*lint !e774*/
      {
        WL2_MSG_1(HIGH, dl_table_ptr, "TBS %d Data pointer is NULL", i);
      }
      /* Invalid number of transport blocks */
#ifdef FEATURE_WCDMA_RLC_ZI_REDUCTION_PHASE3
      else if ((num_tb + tbsptr->num_tb) > UE_OPT_MAX_DL_RLC_PDUS)
#else
      else if ((num_tb + tbsptr->num_tb) > UE_MAX_DL_RLC_PDUS)
#endif
      {
        WL2_MSG_2(HIGH, dl_table_ptr, "TBS %d Data with inv number of TBs %d", i, tbsptr->num_tb);
      }
      /* Process Transport Blocks */
      else
      {
        for (j=0 ;j< tbsptr->num_tb; j++)
        {

          /* Pointer to Transport Block Data info */
          tbptr = &tbsptr->tb_data_info[j];
          /* Invalid TB Data info pointer */
          if (tbptr == NULL) /*lint !e774*/
          {
            WL2_MSG_2(HIGH, dl_table_ptr, "TBS %d TB %d pointer is NULL", i, j);
            continue;
          }

          /* Get internal TB Data info pointer */
          tb_index = (uint8) (tbsptr->tb_data_info[j].tb_id - MAC_DL_MIN_TB_ID);
          dl_tb_info_ptr = GET_DL_TB_INFO_PTR(tb_index, dl_table_ptr);

          /* TB is invalid */
          if (dl_tb_info_ptr->valid == FALSE)
          {
            WL2_MSG_2(HIGH, dl_table_ptr, "TBS %d TB %d is not valid", i, j);
          }
          /* Invalid PDU size */
          else if (tbptr->data_size == 0)
          {
            WL2_MSG_2(HIGH, dl_table_ptr, "TBS %d PDU %d size is zero", i, j);
          }
          /* Invalid PDU pointer */
          else if (tbptr->tb_data_buffer == NULL)
          {
            WL2_MSG_2(HIGH, dl_table_ptr, "TBS %d PDU %d pointer is NULL", i, j);
          }
          else
          {
            if (first == TRUE)
            {
              /* Get free data entry to update the RLC PDU info */
              rlc_enh_data_ind_ptr = rlc_enh_get_free_data_q_entry(dl_table_ptr->as_id);
              if (rlc_enh_data_ind_ptr != NULL)
              {
                rlc_enh_data_ind_ptr->num_complete_pdus = 0;
              }
              else
              {
                WL2_MSG_0(ERROR, dl_table_ptr, "MAC_ERR: Ran out of rlc_enh_data_queue entries");
                continue;
              }
              first = FALSE;
            }
            rlc_enh_data_ind_ptr->decode_result = tbsptr->decode_result;

            complete_pdu_ptr = &(rlc_enh_data_ind_ptr->complete_pdu[rlc_enh_data_ind_ptr->num_complete_pdus]);

            complete_pdu_ptr->rlc_id = dl_tb_info_ptr->rlc_id;
            complete_pdu_ptr->rlc_hdr_data =
              (uint32) ((tbsptr->tb_data_info[j].tb_data_buffer)->data_ptr[0]<<8) |
              (uint32) (tbsptr->tb_data_info[j].tb_data_buffer)->data_ptr[1];

            length = rlc_enh_calc_rlc_hdr_length(dl_table_ptr->as_id, dl_tb_info_ptr->rlc_id,
                         (tbsptr->tb_data_info[j].tb_data_buffer)->data_ptr[0], &is_tm_mode);
            if (length > 0)
            {
              (tbsptr->tb_data_info[j].tb_data_buffer)->used -= length;
              (tbsptr->tb_data_info[j].tb_data_buffer)->data_ptr += length;
            }
            /*
             * Assuming that, In case of non-HS all the RLC PDU from Layer1
             * will come as "one PDU - one DSM" - No Chaining expected
             */
            complete_pdu_ptr->iovec.ptr = tbsptr->tb_data_info[j].tb_data_buffer;
            complete_pdu_ptr->iovec.offset = 0;

#ifdef FEATURE_TC_CONSTANT_DELAY_ENH
            if ((ul_table_ptr->loopback_test_mode != MAC_LOOPBACK_NA) && 
                (is_tm_mode == TRUE))
            {
              /* Set app_ptr to DL-CFN only for LB-2 case */
              if (dl_table_ptr->lb_delay_type.cur_idx == 0)
              {
                cfn_idx = MAX_NUM_CFN - 1;
              }
              else
              {
                cfn_idx = dl_table_ptr->lb_delay_type.cur_idx - 1;
              }
              complete_pdu_ptr->iovec.ptr->app_ptr = NULL;
              /*Using memcpy to avoid compiler waring on 8200*/
              WCDMA_MEMCPY(&complete_pdu_ptr->iovec.ptr->app_ptr,
                           sizeof(complete_pdu_ptr->iovec.ptr->app_ptr),
                           &dl_table_ptr->lb_delay_type.dl_cfn[cfn_idx],
                           sizeof(uint16));
            }
#endif

            if (is_tm_mode == TRUE)
            {
              complete_pdu_ptr->iovec.ptr->app_field |= 0x1; /* for R99 path */
              complete_pdu_ptr->iovec.length = (uint16)((tbsptr->tb_data_info[j].data_size ));
              is_tm_mode = FALSE;
            }
            else
            {
              complete_pdu_ptr->iovec.ptr->app_field = 1; /* for R99 path */
              complete_pdu_ptr->iovec.length = (uint16)((tbsptr->tb_data_info[j].data_size >> 3) - length);
            }

            rlc_enh_data_ind_ptr->num_complete_pdus++;
            num_tb++;
          }
        }
      }
    } /* for loop */
  }
  
  if ((l2_get_release_flag_indicator(as_id) == L2_REL_INDICATOR_5) &&
      (as_id == wl1_msim_get_conn_subid()) &&
      (q_cnt(dec_tsn_info_q_ptr) > 0))
  {
    if ((hs_table_ptr->mac_hs_dl_curr_state != MAC_HS_ACTIVE)
         && !hs_table_ptr->mac_hs_reconfig_in_progress)
    {
      WL2_MSG_1(HIGH, hs_table_ptr, "HS_DSCH not active; HS State = %d", hs_table_ptr->mac_hs_dl_curr_state);
    }
    else
    {
      REX_ISR_LOCK(&mac_mutexes[ul_table_ptr->as_id].mac_hs_cfg_mutex);
      process_dl_hs_enh_dsch_tbs_data(&mac_hs_num_tsn_rcvd, hs_table_ptr);
      REX_ISR_UNLOCK(&mac_mutexes[ul_table_ptr->as_id].mac_hs_cfg_mutex);
    }
  }

  if (rlc_enh_data_ind_ptr != NULL)
  {
    if (rlc_enh_data_ind_ptr->num_complete_pdus > 0)
    {
      rlc_enh_dl_pdu_data_ind(dl_table_ptr->as_id, rlc_enh_data_ind_ptr);
      }
    else
    {
      /* return the entry to free queue */
      rlc_enh_add_to_free_data_q_entry(dl_table_ptr->as_id, rlc_enh_data_ind_ptr);
    }
  }

  if ((l2_get_release_flag_indicator(as_id) == L2_REL_INDICATOR_5) && 
      (hs_table_ptr->mac_hs_dl_curr_state == MAC_HS_ACTIVE) &&
      ((mac_hs_num_tsn_rcvd) 
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
      || (q_cnt(dec_concatenated_pdu_tsn_q_ptr) > 0)
#endif
      ))
  {
    /*
     * Signal the mac_dl_hs task to pick up the processing
     */
    (void) rex_set_sigs(wcdma_mac_hs_dl_get_tcb(hs_table_ptr), MAC_HS_DL_DATA_IND_SIG);
  }
}

#if (((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7))) )
/*============================================================================================
FUNCTION NAME   l1_phy_dl_query_allowed_buf_size_for_p_to_c

DESCRIPTION   This function returns the buffersize used in L1 for 
              Partial to Complete PDU preparation

CONTEXT     L1 interrupt context
=============================================================================================*/
uint16 dummy_buf_size = 25000;
uint16 l1_phy_dl_query_allowed_buf_size_for_p_to_c()
{
  //uint16 dummy_buf_size = 3000;
  return (dummy_buf_size);
}
#endif /* ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7))) ) */

/*============================================================================================
FUNCTION NAME   l1_phy_dl_trblk_header_ind

DESCRIPTION   This function decodes the header (includes MAC as well as RLC) and
        decides the ciphering parameters of each transport block.

CONTEXT     L1 interrupt context
=============================================================================================*/
void  l1_phy_dl_trblk_header_ind
  (
/* Pointer to DL Transport Block Set Header Info received from Layer 1 */
  l1_dl_tb_hdr_decode_struct_type *hdrptr,
  sys_modem_as_id_e_type as_id
  )
{
  uint8   i, j, num_tb_set, tb_id, num_tb;

  uint8   tsn_to_queue_map_idx = 0;

  /* Pointer to internal BCH info */
  bch_config_info_type    *int_bch_ptr;

  /* Pointer to internal PCH info */
  pch_config_info_type    *int_pch_ptr;

  /* Pointer to internal FACH config info */
  fach_config_info_type   *int_fach_ptr;

  /* Pointer to internal DCH config info */
  dl_dch_config_info_type   *int_dch_ptr;

  /* Pointer to internal DSCH config info table */
  dsch_config_info_type   *int_dsch_ptr;

  /* Pointer to header info of transpport block set */
  l1_dl_tb_set_hdr_struct_type    *tbs_hdr_ptr;

  mac_dl_table_type   *dl_table_ptr;
  mac_hs_table_type   *hs_table_ptr;
  mac_ul_table_type   *ul_table_ptr;
  mcalwcdma_dec_rlc_pdu_concatenate_info_struct_type *concat_info_ptr;

  mac_hs_e_type int_hs_type = MAC_HS_TYPE_INVALID;

  rex_crit_sect_type *mac_hs_cfg_mutex_ptr;

  l2_validate_as_id(as_id);
  
  /* Transport Block Index = 0 */
  tb_id = 0;
  dl_table_ptr = GET_MAC_DL_TABLE_PTR(as_id);
  hs_table_ptr = GET_MAC_HS_TABLE_PTR(as_id);
  ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);
  mac_hs_cfg_mutex_ptr = &mac_mutexes[as_id].mac_hs_cfg_mutex;
  /* Header indication data pointer is NULL */
  if (hdrptr == NULL)
  {
    WL2_MSG_0(HIGH, dl_table_ptr, "Header Indication Data pointer is NULL");
  }
  /* Invalid number of transport block sets */
  else if (((l2_get_release_flag_indicator(as_id) == L2_REL_INDICATOR_5)) &&
           ((hdrptr->num_tb_set > MAC_HS_MAX_NUM_TB_SET_PER_10_MS_FRAME )))
  {
    WL2_MSG_1(HIGH, dl_table_ptr, "invalid number of TB sets %d", hdrptr->num_tb_set);
  }
  else if ((((l2_get_release_flag_indicator(as_id) == L2_REL_INDICATOR_99)) &&
            (hdrptr->num_tb_set > UE_MAX_TRCH)))
  {
    WL2_MSG_1(HIGH, dl_table_ptr, "invalid number of TB sets %d", hdrptr->num_tb_set);
  }
  /* Process transport block set header information */
  else
  {
    concat_info_ptr = &hs_table_ptr->mac_hs_dl_rlc_pdu_concatenate_info;
    
    /* Number of Transport Block Sets in Header Info */
    num_tb_set = hdrptr->num_tb_set;
    
    /*-----------------------------------------------------------------
    Mark the cfn at which some data on a DCH transport channel is
    received which needs to be looped back.So if there are transport blocks
    received on multiple tranport channels in the same tti then note
    the cfn only once. This flag will ensure that the cfn is marked only
    once. Initialize the flag to FALSE.
    -----------------------------------------------------------------*/
    dl_table_ptr->cfn_marked = FALSE;

    /* Processes each transport block set (i.e.transport channel) */
    for (i=0;(i < num_tb_set); i++)
    {
      /* Get pointer to header info of transport channel */
      tbs_hdr_ptr = &hdrptr->tb_set_hdr[i];

      /* TBS Header Info Pointer is NULL */
      if (tbs_hdr_ptr == NULL) /*lint !e774*/
      {
        WL2_MSG_1(HIGH, dl_table_ptr, "TBS %d Header Pointer is NULL", i);
      }
      else if (((l2_get_release_flag_indicator(as_id) == L2_REL_INDICATOR_5)) &&
               ((tbs_hdr_ptr->trch_ref_type != L1_DL_TRCH_HS_DSCH_TYPE) 
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
               && (tbs_hdr_ptr->trch_ref_type != L1_DL_TRCH_EHS_DSCH_TYPE)
#endif
               )
               &&
               ((tb_id + tbs_hdr_ptr->num_tb) >= UE_MAX_DL_RLC_PDUS ))
      {
         
         /* Mark each TB of the set as invalid */
         for (j=0 ; j < tbs_hdr_ptr->num_tb; j++)
              tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;

         WL2_MSG_3(HIGH, dl_table_ptr, "TrCh %d with invalid number of TBs %d %d",
                   tbs_hdr_ptr->info.non_hs.trch_id, tb_id, tbs_hdr_ptr->num_tb);
      }
      else if (((l2_get_release_flag_indicator(as_id) == L2_REL_INDICATOR_99)) &&
               ((tb_id + tbs_hdr_ptr->num_tb) >= UE_MAX_DL_RLC_PDUS))
      {
        /* Mark each TB of the set as invalid */
        for (j=0 ; j < tbs_hdr_ptr->num_tb; j++)
          tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;

        WL2_MSG_3(HIGH, dl_table_ptr, "TrCh %d with invalid number of TBs %d %d",
                  tbs_hdr_ptr->info.non_hs.trch_id, tb_id, tbs_hdr_ptr->num_tb);
      }
      /* Process TBS Header Info */
      else
      {
        /* Get number of transport blocks */
        num_tb = tbs_hdr_ptr->num_tb;

#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
        if ((l2_get_release_flag_indicator(as_id) >= L2_REL_INDICATOR_5))
#else
        if ((l2_get_release_flag_indicator(as_id) == L2_REL_INDICATOR_5))
#endif
        {

#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
          L2_MUTEX_LOCK(mac_hs_cfg_mutex_ptr);
          int_hs_type = hs_table_ptr->mac_hs_or_ehs;
          L2_MUTEX_UNLOCK(mac_hs_cfg_mutex_ptr);
          if ((tbs_hdr_ptr->trch_ref_type == L1_DL_TRCH_EHS_DSCH_TYPE) &&
              (int_hs_type == MAC_EHS))
          {
            /* Maximum of 26 pdus per mac-ehs pdu */
            if (MAC_EHS_DL_NUM_TB_PER_TSN_IS_INVALID(tbs_hdr_ptr->info.ehs.num_reorder_sdu))
            {
              WL2_MSG_2(ERROR, dl_table_ptr, "MAC_ERR: TBS Id %d - Invalid num_reorder_sdu %d rcvd", 
                            i, tbs_hdr_ptr->info.ehs.num_reorder_sdu);
              tsn_to_queue_map_idx++;
              continue;
            }
            /* Mark each TB of the set as invalid */
            for (j=0 ; j <tbs_hdr_ptr->info.ehs.num_reorder_sdu; j++)
              tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;
            
          }
          else
#endif 
          {
            L2_MUTEX_LOCK(mac_hs_cfg_mutex_ptr);
            int_hs_type = hs_table_ptr->mac_hs_or_ehs;
            L2_MUTEX_UNLOCK(mac_hs_cfg_mutex_ptr);
            if((tbs_hdr_ptr->trch_ref_type == L1_DL_TRCH_HS_DSCH_TYPE) 
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
                && (int_hs_type == MAC_HS)
#endif
             )
            {
              /*
               * Maximum of 70 mac-d pdus per mac-hs pdu
               */
              if (MAC_HS_DL_NUM_TB_PER_TSN_IS_INVALID(num_tb))
              {
                WL2_MSG_2(ERROR, dl_table_ptr, "TBS Id %d; Invalid num_tb %d rcvd", i, num_tb);
                tsn_to_queue_map_idx++;
                continue;
              }
            }
            /* Mark each TB of the set as invalid */
            for (j=0 ; j <num_tb; j++)
              tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;
          }
        }
        else
        {
          /* Mark each TB of the set as invalid */
          for (j=0 ; j <num_tb; j++)
            tbs_hdr_ptr->tb_hdr_info[j].valid = FALSE;
        }
        
        
        /* Process header info based on transport channel reference type */
        switch (tbs_hdr_ptr->trch_ref_type)
        {

        /* BCH Type TrCh */
        case L1_DL_TRCH_SBCH_TYPE:
        case L1_DL_TRCH_NBCH_TYPE:
          /* CCTrCh is invalid */
          if (DL_CCTRCH_ID_IS_INVALID(tbs_hdr_ptr->cctrch_id))
          {
            WL2_MSG_1(HIGH, dl_table_ptr, "BCH with invalid CCTrCh ID %d", tbs_hdr_ptr->cctrch_id);
          }
          /* Valid CCTrCh ID */
          else
          {
            /* Pointer to BCH config table entry */
            int_bch_ptr = GET_BCH_INFO_PTR(tbs_hdr_ptr->cctrch_id, dl_table_ptr);

            /* BCH is not enabled in MAC */
            if (int_bch_ptr->bch_enable != TRUE)
            {
              WL2_MSG_1(HIGH, dl_table_ptr, "BCH %d not enabled", tbs_hdr_ptr->cctrch_id);
            }
            else
            {
              process_bch_tbs_hdr (tb_id, int_bch_ptr, tbs_hdr_ptr, dl_table_ptr);
            }
          }
          break;

          /* PCH Type TrCh */
        case L1_DL_TRCH_PCH_TYPE:
          /* CCTrCh is invalid */
          if (DL_CCTRCH_ID_IS_INVALID(tbs_hdr_ptr->cctrch_id))
          {
            WL2_MSG_1(HIGH, dl_table_ptr, "PCH with invalid CCTrCh ID %d", tbs_hdr_ptr->cctrch_id);
          }
          /* Valid CCTrCh ID */
          else
          {
            /* Pointer to PCH config table entry */
            int_pch_ptr = GET_PCH_INFO_PTR(tbs_hdr_ptr->cctrch_id, dl_table_ptr);

            /* PCH is not enabled in MAC */
            if (int_pch_ptr->pch_enable != TRUE)
            {
              WL2_MSG_1(HIGH, dl_table_ptr, "PCH %d not enabled", tbs_hdr_ptr->cctrch_id);
            }
            else
            {
              process_pch_tbs_hdr (tb_id, int_pch_ptr, tbs_hdr_ptr, dl_table_ptr);
            }
          }
          break;

          /* FACH type TrCh */
        case L1_DL_TRCH_FACH_TYPE:
           /* Transport Channel ID is invalid */
           if (DL_TRCH_ID_IS_INVALID(tbs_hdr_ptr->info.non_hs.trch_id))
           {
             WL2_MSG_2(HIGH, dl_table_ptr, "FACH %d with invalid TrCh ID %d", i, tbs_hdr_ptr->info.non_hs.trch_id);
           }
           else
           {
             /* Get transport chanel config info pointer */
             int_fach_ptr = GET_FACH_INFO_PTR(tbs_hdr_ptr->info.non_hs.trch_id, dl_table_ptr);
             
             /* FACH is not enabled */
             if (FACH_IS_ENABLED(tbs_hdr_ptr->info.non_hs.trch_id, dl_table_ptr) == FALSE)
             {
               WL2_MSG_1(HIGH, dl_table_ptr, "FACH %d not enabled", tbs_hdr_ptr->info.non_hs.trch_id);
             }
             /* Process FACH TBS Header Information */
             else
             {
               process_fach_tbs_hdr (tb_id, int_fach_ptr, tbs_hdr_ptr, dl_table_ptr);
             }
           }

          break;

          /* DCH type TrCh */
        case L1_DL_TRCH_DCH_TYPE:
           /* Transport Channel ID is invalid */
           if (DL_TRCH_ID_IS_INVALID(tbs_hdr_ptr->info.non_hs.trch_id))
           {
             WL2_MSG_1(HIGH, dl_table_ptr, "DCH %d with invalid TrCh ID", tbs_hdr_ptr->info.non_hs.trch_id);
           }
           else
           {
              /* Get transport chanel config info pointer */
              int_dch_ptr = GET_DL_DCH_INFO_PTR(tbs_hdr_ptr->info.non_hs.trch_id, dl_table_ptr);

              /* DCH is not enabled */
              if (DL_DCH_IS_ENABLED(tbs_hdr_ptr->info.non_hs.trch_id, dl_table_ptr) == FALSE)
              {
                WL2_MSG_1(HIGH, dl_table_ptr, "DCH %d not enabled", tbs_hdr_ptr->info.non_hs.trch_id);
              }
              /* Process DL DCH TBS Header Information */
              else
              {
                process_dl_dch_tbs_hdr (tb_id, int_dch_ptr, tbs_hdr_ptr, dl_table_ptr, ul_table_ptr);
              }
           }
          break;

          /* HS-DSCH type TrCh */
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
        case L1_DL_TRCH_EHS_DSCH_TYPE:
#endif           
        case L1_DL_TRCH_HS_DSCH_TYPE:

#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
           if ((l2_get_release_flag_indicator(as_id) >= L2_REL_INDICATOR_5))
#else
           if ((l2_get_release_flag_indicator(as_id) == L2_REL_INDICATOR_5))
#endif
           {
             /* DCH is not enabled */
             if ((hs_table_ptr->mac_hs_dl_curr_state != MAC_HS_ACTIVE )
                 && !hs_table_ptr->mac_hs_reconfig_in_progress)
             {
               WL2_MSG_1(HIGH, dl_table_ptr, "HS_DSCH not active; HS State = %d", hs_table_ptr->mac_hs_dl_curr_state);
             }
             /* Process DL DCH TBS Header Information */
             else
             {
               if (tsn_to_queue_map_idx >= MAC_HS_DL_MAX_TSN_PER_10_MS_FRAME)
               {
                 WL2_MSG_1(ERROR, dl_table_ptr, "Rcvd %d TSNs in 10 ms frame", tsn_to_queue_map_idx);
               }
               else
               {
                 #ifdef FEATURE_WCDMA_HS_RACH
                 mac_hsrach_stop_tebs0_timer_dl_data_recevied(as_id);
                 #endif /* FEATURE_WCDMA_HS_RACH */
                 
                 L2_MUTEX_LOCK(mac_hs_cfg_mutex_ptr);
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
                 if (hs_table_ptr->mac_hs_or_ehs == MAC_EHS)
                 {
                   process_dl_ehs_dsch_tbs_hdr(i, &tsn_to_queue_map_idx, tbs_hdr_ptr,as_id);
                   tsn_to_queue_map_idx++;
                 }
                 else if (hs_table_ptr->mac_hs_or_ehs == MAC_HS)
#endif
                 {
                   process_dl_hs_dsch_tbs_hdr (i, tsn_to_queue_map_idx, tbs_hdr_ptr, hs_table_ptr);
                   tsn_to_queue_map_idx++;
                 }
                 L2_MUTEX_UNLOCK(mac_hs_cfg_mutex_ptr);

               }
             }
           }
           break;

          /* DSCH type TrCh */
         case L1_DL_TRCH_DSCH_TYPE:
            /* Transport Channel ID is invalid */
            if (DL_TRCH_ID_IS_INVALID(tbs_hdr_ptr->info.non_hs.trch_id))
            {
              WL2_MSG_1(HIGH, dl_table_ptr, "DSCH %d with invalid TrCh ID", tbs_hdr_ptr->info.non_hs.trch_id);
            }
            else
            {
              /* Get transport chanel config info pointer */
              int_dsch_ptr = GET_DSCH_INFO_PTR(tbs_hdr_ptr->info.non_hs.trch_id, dl_table_ptr);

              /* DSCH is not enabled */
              if (DSCH_IS_ENABLED(tbs_hdr_ptr->info.non_hs.trch_id, dl_table_ptr) == FALSE)
              {
                WL2_MSG_1(HIGH, dl_table_ptr, "DSCH %d not enabled", tbs_hdr_ptr->info.non_hs.trch_id);
              }
              /* Process DL DCH TBS Header Information */
              else
              {
                process_dsch_tbs_hdr (tb_id, int_dsch_ptr, tbs_hdr_ptr, dl_table_ptr);
              }
            }
          break;

          /* Invalid transport channel type */
        default:
          WL2_MSG_2(HIGH, dl_table_ptr, "TBS %d: invalid TrCh type %d", i, tbs_hdr_ptr->trch_ref_type);
          break;
        }

        if ((tbs_hdr_ptr->trch_ref_type != L1_DL_TRCH_HS_DSCH_TYPE)
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
            && (tbs_hdr_ptr->trch_ref_type != L1_DL_TRCH_EHS_DSCH_TYPE)
#endif
           )
        {
           /* Update TB ID value */
           tb_id = (uint8)(tb_id + num_tb);
        }
      }
    }

#ifdef FEATURE_RLC_QXDM_LOGGING
    /*-----------------------------------------------------------------------
      Set the RLC_DL_LOG_CIPHER_PKT_SIG signal.
    -----------------------------------------------------------------------*/
    rlc_dl_set_cipher_logging(dl_table_ptr->as_id);
#endif
  }
}


/*==============================================================================================
 FUNCTION NAME  proc_dl_mac_commands

 DESCRIPTION  Processes DL MAC commands and responses received from other tasks

==============================================================================================*/
void  proc_dl_mac_commands
  (
/* Command ID */
  wcdma_l2_dl_cmd_enum_type   cmd_id,

/* Pointer to command data */
  wcdma_l2_dl_cmd_data_type   *cmd_data_ptr,

  sys_modem_as_id_e_type as_id
  )
{

  mac_hs_dl_config_req_type *hs_config_ptr;
  mac_dl_table_type *dl_table_ptr;
  mac_hs_table_type *hs_table_ptr;

  dl_table_ptr = GET_MAC_DL_TABLE_PTR(as_id);
  hs_table_ptr = GET_MAC_HS_TABLE_PTR(as_id);  
  hs_config_ptr = &hs_table_ptr->mac_hs_dl_config;
  
  /* Process the command data based on command ID */
  switch (cmd_id)
  {

  /* DL MAC Congiguration Request */
  case  CMAC_DL_CONFIG_REQ:
   #ifdef FEATURE_WCDMA_SRB_PRIO_READJ
  #error code not present
#endif
   mac_hs_dl_validate_config_req(&cmd_data_ptr->mac_config_info, hs_table_ptr);
   WL2_MSG_3(HIGH, dl_table_ptr, "MAC config validate, config hs params %d non hs params %d reset %d", 
            hs_config_ptr->operation.save_hs_params,
            hs_config_ptr->operation.save_non_hs_params,
            hs_config_ptr->operation.perform_hs_reset);

    /* Clear the memory */
    memset(&dl_table_ptr->dl_cfginfo,0,sizeof(mac_dl_config_type));

    /* Copy the configuration information received from RRC*/
    WCDMA_MEMCPY(&dl_table_ptr->dl_cfginfo,
                 sizeof(mac_dl_config_type),
                 &cmd_data_ptr->mac_config_info, 
                 sizeof(mac_dl_config_type));

    /*-----------------------------------------------------------------------------
    MAC sets dl_configuration to TRUE when RRC asks MAC to wait for the signal
    from L1.
    ------------------------------------------------------------------------------*/
    if (cmd_data_ptr->mac_config_info.wait_for_l1_sig)
    {
      /*Set the boolean to TRUE indicating that MAC DL is now waiting for the signal
      from PHY layer to switch to the new configuration */
      dl_table_ptr->dl_configuration_pending=TRUE;
      
      WL2_MSG_0(HIGH, dl_table_ptr, "Wait for L1 signal to reconfig MAC");
    }
    else // Switch to the new configuration NOW
    {
      WL2_MSG_0(HIGH, dl_table_ptr, "continue with NON HS config");
      
      config_dl_mac (&cmd_data_ptr->mac_config_info, dl_table_ptr, hs_table_ptr);
      
      if ((l2_get_release_flag_indicator(as_id) >= L2_REL_INDICATOR_5) &&
          (hs_config_ptr->operation.perform_hs_reset ||
           hs_config_ptr->operation.save_hs_params ))
      {
        WL2_MSG_0(HIGH, dl_table_ptr, "continue with HS config");
        proc_dl_mac_hs_config_signal(as_id);
      }
      else
      {
        WL2_MSG_3(HIGH, dl_table_ptr, "config params %d non hs params %d reset %d", 
                  hs_config_ptr->operation.save_hs_params,
                  hs_config_ptr->operation.save_non_hs_params,
                  hs_config_ptr->operation.perform_hs_reset);
      }
    }
    
    break;

    /* Invalid command */
  default:
    WL2_MSG_1(HIGH, dl_table_ptr, "rcvd invalid command %d", cmd_id);
    break;
  }
}

/*==============================================================================================
 FUNCTION NAME  proc_dl_mac_config_signal

 FUNCTION   This function processes the config signal received from the physical layer.
===============================================================================================*/
void proc_dl_mac_config_signal (mac_dl_table_type *dl_table_ptr, mac_hs_table_type *hs_table_ptr)
{
  if (dl_table_ptr->dl_configuration_pending)
  {
    WL2_MSG_0(HIGH, dl_table_ptr, "rcvd non HS config signal from L1");
    
    /* Configure MAC only after this signal is recvd from L1 */
    config_dl_mac(&dl_table_ptr->dl_cfginfo, dl_table_ptr, hs_table_ptr);
    
    /* Post a sucess message to L1 task */
    maci_post_l1_config_ind(CPHY_MAC_DL_CONFIG_CNF, dl_table_ptr->as_id);
    
    /*set configuration_pending to false */
    dl_table_ptr->dl_configuration_pending=FALSE;
  }
  else
  {
    WL2_MSG_0(ERROR, dl_table_ptr, "Recvd L1 sig, NO configuration pending !!");
  }
}

/*==============================================================================================
 FUNCTION NAME  proc_dl_mac_config_signal

 FUNCTION   This function processes the config signal received from the physical layer.

            NOTE -- mac_hs_reset will be performed in L2-DL task context rather than mac-hs task
            context.
===============================================================================================*/
void proc_dl_mac_config_at_l1_failure_signal (mac_dl_table_type *dl_table_ptr, mac_hs_table_type *hs_table_ptr)
{
  if (dl_table_ptr->dl_configuration_pending)
  {
    REX_ISR_LOCK(&mac_mutexes[dl_table_ptr->as_id].mac_hs_cfg_mutex);
    /* If we were to perform hs_reset and we received a config failed sig from L1*/
    if (hs_table_ptr->mac_hs_dl_config.operation.perform_hs_reset)
    {
      /* Start mac_hs_reset processing */
      mac_hs_dl_start_reset_req(hs_table_ptr);

      /* Complete mac_hs_reset processing */
      mac_hs_dl_finish_reset_req(hs_table_ptr);

      /* Reset local data structure that saves what operations need to be performed */
#if ((defined (FEATURE_HSDPA_MAC_EHS )) && (defined (FEATURE_WCDMA_REL7)))
      
      HSPA_PLUS_MAC_HS_TYPE_VALID_CHECK(hs_table_ptr->mac_hs_dl_config.mac_hs_or_ehs, dl_table_ptr)
      
      if(hs_table_ptr->mac_hs_dl_config.mac_hs_or_ehs == MAC_EHS)
      {
        mac_enhs_dl_reset_local_config_data(hs_table_ptr);
      }
      else
#endif
      {
        mac_hs_dl_reset_local_config_data(hs_table_ptr);
      }
    }
    else
    {
      WL2_MSG_0(ERROR, dl_table_ptr, "Rcvd L1 config fail sig, no hs-reset to be performed");
    }
    REX_ISR_UNLOCK(&mac_mutexes[dl_table_ptr->as_id].mac_hs_cfg_mutex);
    /* Post a sucess message to L1 task */
    maci_post_l1_config_ind(CPHY_MAC_DL_CONFIG_AT_L1_FAIL_CNF, dl_table_ptr->as_id);
    
    /*set configuration_pending to false */
    dl_table_ptr->dl_configuration_pending = FALSE;
  }
  else
  {
    WL2_MSG_0(ERROR, dl_table_ptr, "Recvd L1 sig, NO configuration pending !!");
  }
}

/*==============================================================================================
 FUNCTION NAME  proc_dl_hs_config_signal

 FUNCTION   This function processes the HS config signal received from the physical layer.
===============================================================================================*/
void proc_dl_mac_hs_config_signal (sys_modem_as_id_e_type as_id)
{
  mac_dl_table_type *dl_table_ptr;
  mac_hs_table_type *hs_table_ptr;
  
  dl_table_ptr = GET_MAC_DL_TABLE_PTR(as_id);
  hs_table_ptr = GET_MAC_HS_TABLE_PTR(as_id);
  
  REX_ISR_LOCK(&mac_mutexes[as_id].mac_hs_cfg_mutex);
#if ((defined (FEATURE_HSDPA_MAC_EHS )) && (defined (FEATURE_WCDMA_REL7)))
  
  HSPA_PLUS_MAC_HS_TYPE_VALID_CHECK(hs_table_ptr->mac_hs_dl_config.mac_hs_or_ehs, dl_table_ptr)
  
  if (((hs_table_ptr->mac_hs_dl_config.mac_hs_or_ehs != MAC_HS_TYPE_INVALID)||
       (dl_table_ptr->dl_cfginfo.mac_hsdpa_action == HSDPA_STOP)) &&
#else
  if (((l2_get_release_flag_indicator(as_id) == L2_REL_INDICATOR_5)) &&
#endif
      ( hs_table_ptr->mac_hs_dl_config.operation.perform_hs_reset ||
        hs_table_ptr->mac_hs_dl_config.operation.save_hs_params ))
  {
    WL2_MSG_0(HIGH, dl_table_ptr, "Pre-configuring HS DSCH/HSPA+");
    
    /*----------------------------------------------------------------
     *  There will be a context switch inside the function below where
     *  L2-DL task signals MAC-HS task to pick up MAC-HS configuration.
     *  For details discussion, please see the MAC-HS design document.
     ----------------------------------------------------------------*/
    process_dl_hs_dsch_config(&dl_table_ptr->dl_cfginfo, hs_table_ptr);
    
    /*----------------------------------------------------------------
     * L2-DL task procesing will resume here
     ----------------------------------------------------------------*/

#ifdef FEATURE_MAC_HS_QXDM_LOGGING
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
    if (log_status(WCDMA_MAC_HS_CONFIG_LOG_PACKET_V2))
    {
      mac_ehs_dl_send_hs_config_log_packet(&dl_table_ptr->dl_cfginfo, hs_table_ptr);
    }
#else
    if (log_status(WCDMA_MAC_HS_CONFIG_LOG_PACKET))
    {
      mac_hs_dl_send_hs_config_log_packet(&dl_table_ptr->dl_cfginfo, hs_table_ptr);
    }
#endif /*((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7))) */
#endif

    if (l2_get_release_flag_indicator(as_id) >= L2_REL_INDICATOR_5)
    {
      if (hs_table_ptr->mac_hs_dl_new_state != MAC_HS_INVALID)
      {
        hs_table_ptr->mac_hs_dl_curr_state = hs_table_ptr->mac_hs_dl_new_state;
        hs_table_ptr->mac_hs_dl_new_state = MAC_HS_INVALID;
      }
    }
  }
  else
  {
    WL2_MSG_0(ERROR, dl_table_ptr, "Recvd HS config sig, HS config not pending !!");
  }
  REX_ISR_UNLOCK(&mac_mutexes[as_id].mac_hs_cfg_mutex);
}

/*===============================================================================================
FUNCTION NAME mac_dl_get_trch_type

DESCRIPTION   This function returns the down-link transport channel type corresponding to given RLC & RB IDs
===============================================================================================*/

log_wcdma_trch_type mac_dl_get_trch_type(rlc_lc_id_type rlc_id, rb_id_type rb_id, mac_dl_table_type *dl_table_ptr)
{
  log_wcdma_trch_type return_trch_val = WCDMA_TRCH_INVALID;

  return_trch_val = mac_dl_check_dch_trch(rlc_id, rb_id, dl_table_ptr);
  if(return_trch_val != WCDMA_TRCH_INVALID)
  {
    return return_trch_val;
  }

  return_trch_val = mac_dl_check_fach_trch(rlc_id, rb_id, dl_table_ptr);
  if(return_trch_val != WCDMA_TRCH_INVALID)
  {
    return return_trch_val;
  }

  return_trch_val = mac_dl_check_dsch_trch(rlc_id, rb_id, dl_table_ptr);
  if(return_trch_val != WCDMA_TRCH_INVALID)
  {
    return return_trch_val;
  }

  return return_trch_val;
}
/*==============================================================================================
 FUNCTION NAME  mac_rlc_check_pending_dl_data

 FUNCTION   This function if there is any data pending to be received or processed in DL.
===============================================================================================*/
boolean mac_rlc_check_pending_dl_data(mac_hs_table_type *hs_table_ptr)
{
  boolean ret_val = FALSE;

  ret_val = mac_dl_hs_check_pending_tsns(hs_table_ptr);
  if (FALSE == ret_val)
  {
    rlc_check_pending_dl_active_rb_data(hs_table_ptr->as_id);
  }

  return ret_val;
}
#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
FUNCTION WMAC_QSH_DUMP_R99_DL_INFO

DESCRIPTION Function collects MAC R99 DL config to the address specified 
by QSH.
===========================================================================*/
void wmac_qsh_dump_r99_dl_info(uint8 as_id,void *write_dump_addr)
{

  uint8 num_lch,num_rlc_id;
  mac_dl_table_type *dl_table_ptr;
  wmac_qsh_r99_dl_mini_dump_s *wmac_r99_dl;

  l2_validate_as_id(as_id);

  dl_table_ptr = GET_MAC_DL_TABLE_PTR(as_id);
  dl_table_ptr = GET_MAC_DL_TABLE_PTR(as_id);
  wmac_r99_dl = (wmac_qsh_r99_dl_mini_dump_s *)write_dump_addr;

  wmac_r99_dl->as_id = as_id;
  wmac_r99_dl->mac_cfg_pendig = dl_table_ptr->dl_configuration_pending;
  wmac_r99_dl->num_active_dl_dchs = dl_table_ptr->num_active_dl_dchs;
  wmac_r99_dl->num_active_dschs = dl_table_ptr->num_active_dschs;
  wmac_r99_dl->num_active_fachs = dl_table_ptr->num_active_fachs;

  for ( num_lch = 0; num_lch < UE_REDUCED_MAX_CCTRCH ; num_lch++)
  {
    wmac_r99_dl->mac_non_hs_cfg.bch_cfg_info[num_lch].bcch_id = dl_table_ptr->bch_cfg_info[num_lch].bcch_id;
    wmac_r99_dl->mac_non_hs_cfg.bch_cfg_info[num_lch].bch_enable = dl_table_ptr->bch_cfg_info[num_lch].bch_enable;
    wmac_r99_dl->mac_non_hs_cfg.bch_cfg_info[num_lch].bcch_id = dl_table_ptr->bch_cfg_info[num_lch].bcch_id;
    wmac_r99_dl->mac_non_hs_cfg.bch_cfg_info[num_lch].bch_enable = dl_table_ptr->bch_cfg_info[num_lch].bch_enable;
  }

  if (dl_table_ptr->num_active_fachs)
  {
    for ( num_lch = 0; num_lch < UE_REDUCED_MAX_TRCH ; num_lch++)
    {
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].cctrch_id = dl_table_ptr->fach_cfg_info[num_lch].cctrch_id;
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].ccch_enable = dl_table_ptr->fach_cfg_info[num_lch].ccch_enable;
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].trch_id = dl_table_ptr->fach_cfg_info[num_lch].trch_id;
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].bcch_enable = dl_table_ptr->fach_cfg_info[num_lch].bcch_enable;
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].bcch_id = dl_table_ptr->fach_cfg_info[num_lch].bcch_id;
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].ctch_enable = dl_table_ptr->fach_cfg_info[num_lch].ctch_enable;
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].ctch_id = dl_table_ptr->fach_cfg_info[num_lch].ctch_id;
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].dlc_enable = dl_table_ptr->fach_cfg_info[num_lch].dlc_enable;
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].num_dlc = dl_table_ptr->fach_cfg_info[num_lch].num_dlc;
      wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].trch_id = dl_table_ptr->fach_cfg_info[num_lch].trch_id;
  
      for (num_rlc_id =0; num_rlc_id < dl_table_ptr->fach_cfg_info[num_lch].num_dlc;num_rlc_id++)
      {
        wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].dlcinfo[num_rlc_id].enable = 
         dl_table_ptr->fach_cfg_info[num_lch].dlcinfo[num_rlc_id].enable;
        wmac_r99_dl->mac_non_hs_cfg.fach_cfg_info[num_lch].dlcinfo[num_rlc_id].rlc_id = 
         dl_table_ptr->fach_cfg_info[num_lch].dlcinfo[num_rlc_id].rlc_id;
      }
    }
  }

  if (dl_table_ptr->num_active_dl_dchs)
  {
    for ( num_lch = 0; num_lch < UE_REDUCED_MAX_TRCH ; num_lch++)
    {
      wmac_r99_dl->mac_non_hs_cfg.dl_dch_cfg_info[num_lch].cctrch_id = dl_table_ptr->dl_dch_cfg_info[num_lch].cctrch_id;
      wmac_r99_dl->mac_non_hs_cfg.dl_dch_cfg_info[num_lch].trch_id = dl_table_ptr->dl_dch_cfg_info[num_lch].trch_id;
      wmac_r99_dl->mac_non_hs_cfg.dl_dch_cfg_info[num_lch].mac_d_flow_id = dl_table_ptr->dl_dch_cfg_info[num_lch].mac_d_flow_id;
      wmac_r99_dl->mac_non_hs_cfg.dl_dch_cfg_info[num_lch].cs_or_ps_domain = dl_table_ptr->dl_dch_cfg_info[num_lch].cs_or_ps_domain;
      wmac_r99_dl->mac_non_hs_cfg.dl_dch_cfg_info[num_lch].num_dlc = dl_table_ptr->dl_dch_cfg_info[num_lch].num_dlc;
  
      for (num_rlc_id =0; num_rlc_id < dl_table_ptr->dl_dch_cfg_info[num_lch].num_dlc;num_rlc_id++)
      {
        wmac_r99_dl->mac_non_hs_cfg.dl_dch_cfg_info[num_lch].dlcinfo[num_rlc_id].enable = 
                                        dl_table_ptr->dl_dch_cfg_info[num_lch].dlcinfo[num_rlc_id].enable ;
        wmac_r99_dl->mac_non_hs_cfg.dl_dch_cfg_info[num_lch].dlcinfo[num_rlc_id].mac_id = 
                                        dl_table_ptr->dl_dch_cfg_info[num_lch].dlcinfo[num_rlc_id].mac_id;
        wmac_r99_dl->mac_non_hs_cfg.dl_dch_cfg_info[num_lch].dlcinfo[num_rlc_id].rlc_id = 
                                        dl_table_ptr->dl_dch_cfg_info[num_lch].dlcinfo[num_rlc_id].rlc_id;
      }
    }
  }
}
#endif
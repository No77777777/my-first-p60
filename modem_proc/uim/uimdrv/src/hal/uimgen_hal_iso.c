/*===========================================================================

                            U I M G E N  H A L  I S O

DESCRIPTION
  This file contains generic state machine functions that are specific to
  ISO UIM

Copyright (c) 2015 - 2021, 2024 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimgen_hal_iso.c#11 $
$DateTime: 2024/09/10 02:34:39 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/10/24   gnr     Ignore tech problem error for fetch command
08/30/21   ykr     Ignore voltage class error in MF selection.
08/26/21   ykr     Ignore poll error under NV protection under gcf flag
08/04/21   ykr     Fixed KW issues
02/20/20   rkg     Add new TLV in Suscription manager
02/12/20   ykr     Do not do anything in case recovery fails
02/11/20   rkg     NV workaround to continue recovery without TP
01/31/20   ykr     Disabling slot as per SIMLOCK configuration
09/17/19   mm      Add EID to SimSlotStatus radio config HAL
12/26/18   ng      Select EF-IMSI before Authentication command
09/19/18   ng      Fix issue of endless loop in uimdrv when wrong FCP response
09/05/18   ng      Do not do anything in case recovery fails
07/17/18   srm     Update poll duration properly as part of poll interval handling
05/24/18   ku      Preventing overwrite of stream_iso7816_apdu union
03/27/18   gm      Start extended recovery when card is present
02/01/18   nc      BT-SAP incorrect error code for card error fix
01/10/18   ku      Send the second poll command irrespective of sw1 and sw2
10/30/17   ku      Trigger recovery after bad SW for initial SELECT MF
10/11/17   gm      Do not send select MF on failure for non-telecom app
09/20/17   gm      Using extra guard time in TC1 only for SAM card
07/06/17   ku      Poll card for twice when detecting flaky removal interupt
07/18/17   gm      Send warning status word for stream APDU failure response
07/17/17   gm      Send warning status word for non-telecom app
07/13/17   ks      Do not trigger recovery during 7FFF failure before AUTH
07/11/17   ks      Fixing few issues with passive mode
06/19/17   gm      Do not re-select after GET-REPONSE for SELECT command
06/12/17   ks      Do not apply non-spec complaint card workarounds to GCF cards
06/05/17   ks      Stream APDU select by file ID directory maintaince fix
05/16/17   gm      MF select when received invalid FCI/FCM parameter
05/12/17   gm      MF select if FCP parameter is invalid
05/10/17   ku      Switch Card Mode if no MF is indicated in ATR
05/05/17   ku      Incrementing state pointer on poll error for buggy card
05/02/17   ku      Adding code to access critical section for ATR
04/26/17   kv      Sam card mode handling updates, euicc check during power up
04/07/17   gm      Not retrying for memory problem status word
04/06/17   gm      Added support for extended APDU
02/23/17   ku      UIMDRV KW errors fix
02/22/17   ar      Call LPA APIs only if LPA is modem centric
02/13/17   ks      Limit the ICCID to 10 bytes even if card has more bytes
02/01/17   ks      Fix for non-spec complaint card for poll error under 7F10
01/27/17   ks      Send proper slot parameter while retriving terminal cap
01/24/17   ks      Ignore poll error under 7F10 directory under NV protection
12/28/16   gm      Handling fetch indication during intermediate select
12/07/16   av      LPA phase2 initial changes
12/07/16   ssr     ATR Parsing Optimization
12/05/16   gm      Sending default PPS while 4th retry
11/24/16   ks      Adding support for spcom interface
11/04/16   ks      Fix in Case-2 stream apdu handling
10/24/16   ku      KW errors fix
09/30/16   gm      Re-set globals during power-up
09/22/16   gm      Added support for set APDU behavior
08/29/16   ku      Updating the ATR message format
08/16/16   ks      Re-storing the file path after successful recovery
07/14/16   ks      Enhancing the extended recovery feature
07/14/16   ks      Making the "reattempted_select_mf_upon_tech_prob" as global
07/14/16   gm      Sending APDU stream select without modification
07/08/16   ks      Correcting the BWT calculation
07/05/16   av      Fix incorrect parsinf of terminal capability EFS file
07/05/16   ks      New global to count PPS attempts
07/01/16   ks      Fix to re-attempt select MF upon receiving 6F 00
06/30/16   av      Fix slot id to efs context conversion for uim common APIs
06/23/16   sam     F3 messages optimization
06/21/16   sam     Avoid selection of 7FFF file on non-provisioning application
06/16/16   av      Support for setting terminal capability from AP
06/14/16   ks      ATR parsing correction for TB3 and TB4 bytes
06/10/16   gm      Remove the test code of Simulate Null byte
05/25/16   na      Removed unused enumeration
06/07/16   gm      F3 optimization
06/06/16   ks      Fix issue with ATR parsing for eUICC information in TB4
05/24/16   ssr     F3 Optimization on curr path
05/23/16   na      F3 Optimization
05/17/16   ssr     Remove msg for malloc failure
05/10/16   hyo     Reset is_ef flag streaming select
05/09/16   sam     Assign internally opened channel to client open channel request
04/29/16   ks      Fix issue wtih ATR parsing for eUICC information
04/27/16   ks      Fix KW issue
04/22/16   na      Corrected the calculation of cwt for T=1
04/20/16   sam     Support for enhanced subscription manager
04/13/16   ar      CDMA/HDR support on 2nd SUB
04/11/16   ks      Changes for LPA support
04/01/16   sam     Send generic error report for the toolkit commands removed
                   from uim cmd queue during recovery
03/28/16   sam     Support for SAM card
03/21/16   na      Do not retry manage channel request upon receipt of sw1 0x6E
03/09/16   na      Iccid length is being filled from file size received in FCP of EF-ICCID
02/19/16   na      Reduce number of ATR prints under 20 F3s/ms
02/09/16   sam     Workaround to select MF/ADF on open channels after a succesful
                   close of channel on which ARA-M app selection failed.
02/06/16   ll      Extracting the first T15 TA byte from ATR for clock stop
01/20/16   sam     Reset Guard time before sending PPS
01/19/16   na      Corrected the condition logic
01/06/16   na      Do not update current path for streaming apdu in ICC mode
12/29/15   ks      BTSAP client should ignore voltage parameters parsing
11/26/15   ks      Do not attempt close channel command upon its failure
11/26/15   sam     Add logic to update EF-TST in recovery
11/23/15   ks      Recovery upon failure of close channel
                   where ARAM app is selected
11/10/15   hyo     Voltage class dependent max power capability advertisement in TC
11/02/15   sam     1. Added logic to queue ADF/MF selection on other channels
                      if ara-m app selection failed
                   2. Added logic to trigger recovery if MF or ADF selection fails
10/20/15   hyo     Check characteristics change for offtarget
09/16/15   hyo     KW issue fix
09/02/15   hyo     Initial Revision
===========================================================================*/
#include "intconv.h"
#include "mmgsdilib_p.h"
#include "nvruim_p.h"
#include "uimdrv_uartdm.h"
#include "uim_remote.h"
#include "uimdrv_hal_iso.h"
#include "uimdrv_main.h"
#include "uimdrv_msg.h"
#include "uimgen_hal_iso.h"
#include "uimsub_manager.h"
#include "uimutil.h"
#include "lpalib.h"
#include "uim_common_efs.h"
#include "mcfg_nv_api.h"

/*=============================================================================
  MACROS
=============================================================================*/
/* This defines the amount of time to delay after receiving the ATR and
 * before sending out a PPS request to the CARD
 */
#define UIM_DELAY_TIME_AFTER_ATR    1

/* This defines the delay after powering the UIM and after PPS. */
#define UIM_CARD_DELAY_TIMEOUT 20
#define BGT_22_ETU             22

/* Milliseconds correction */
#define UIM_CLK_MS_PER_TICK 1

/* Macros for F=372 and D=12 baud-rate representation in TA1 byte */
#define UIM_F372_D12_COMB1     0x18
#define UIM_F372_D12_COMB2     0x08

/* Extra BGT time (in milli seconds) due to some unknown timing issues */
#define UIM_EXTRA_BGT_TIME    10

/* This defines the size of the historical characters for buggy cards that need
     extra delay before the response */
#define UIM_HIST_CHAR_CMP_SIZE 9
static byte const UIM_MORE_DELAY_HIST_CHARS[] =
{0x4D, 0x4D, 0x41, 0x52, 0xF2, 0x2B, 0x56, 0x32, 0x2E};
#define UIM_MIN_MINOR_REV_MORE_DELAY '3'
#define UIM_MAX_MINOR_REV_MORE_DELAY '4'
#define UIM_EUICC_AID_LEN            16

#define UIM_MF_RESELECT_COUNT_UPON_INVALID_FCP_TEMPLATE     2

/* Buffer to hold ICCID used in GCF test cases */
static byte const UIM_TEST_ICCID[UIM_ICCID_SIZE] = {0x98, 0x94, 0x00, 0x20, 0x20, 0x41, 0x00, 0x00, 0x40, 0xF5 };

/* Global to hold the svn */
uint8   uim_svn[UIM_MAX_NUM_SLOTS][UIM_SVN_LEN]   = {{UIM_SVN_BYTE1_DEFAULT, UIM_SVN_BYTE2_DEFAULT, 0x00},
                                                    {UIM_SVN_BYTE1_DEFAULT, UIM_SVN_BYTE2_DEFAULT, 0x00}};


/*=============================================================================
  STATES
==============================================================================*/
/*  Additional states to recover ICC card */
static const uim_generic_state_type UIM_RECOVER_ICC_SLOT_STATES[] =
{
  UIM_RECOVERY_FLUSH_MANAGE_CHANNEL_ST,
  UIM_CHECK_FOR_CDMA_DF,
  UIM_RECOVERY_VERIFY_CHV1_ST,
  UIM_RECOVERY_VERIFY_CHV2_ST,
  UIM_RECOVERY_PERFORM_TP_ST,
  UIM_RECOVERY_CHECK_REHAB_REQD_ST,
  UIM_SELECT_ST,
  UIM_RECOVERY_REHAB_ST,
  UIM_RECOVERY_CHECK_EF_TST_UPDATE_REQD_ST,
  UIM_RECOVERY_SELECT_EF_TST_ST,
  UIM_RECOVERY_WRITE_EF_TST_ST,
  UIM_DONE_ST
};

/* Additional states to recover UICC card */
static const uim_generic_state_type UIM_RECOVER_UICC_SLOT_STATES[] =
{
  UIM_RECOVERY_PERFORM_TP_ST,
  UIM_RECOVERY_MANAGE_CHANNEL_FOR_UICC_ST,
  UIM_RECOVERY_CLOSE_CHANNEL_FOR_UICC_ST,
  UIM_RECOVERY_CHECK_FOR_SELECT_AID_ST,
  UIM_SELECT_ST,
  UIM_SET_APDU_BEHAVIOR_ST,
  UIM_RECOVERY_VERIFY_CHV1_ST,
  UIM_RECOVERY_VERIFY_CHV2_ST,
  UIM_RECOVERY_SEND_STATUS_COMMAND_ST,
  UIM_RECOVERY_CHECK_EF_TST_UPDATE_REQD_ST,
  UIM_RECOVERY_SELECT_EF_TST_ST,
  UIM_RECOVERY_WRITE_EF_TST_ST,
  UIM_RECOVERY_VERIFY_UPIN_FOR_UICC_ST ,
  UIM_DONE_ST
};


/*=============================================================================
  INTERNAL FUNCTION PROTOTYPES
==============================================================================*/
static void uim_set_uicc_pin_param_p2
(
  uim_chv_type chv,
  byte *p2,
  uim_instance_global_type *uim_ptr
);

static int uim_compare_manage_ch_cmd
(
  void *item_ptr,
  void *param
);

static void uim_remove_manage_ch_cmd
(
  void                   *item_ptr,
  void                   *param
);

static void uim_parse_uicc_select_response
(
  uim_instance_global_type                              *uim_ptr,
  byte const                                            *data,
  word                                                   num_bytes,
  boolean                                               *ef_rsp_ptr,
  byte                                                  *file_char,
  uint8                                                 *supported_cmd_present,
  uim_num_apps_data_type                                *num_record_ptr  
);

static void uim_parse_uicc_status_response
(
  byte const               *data,
  word                      num_bytes,
  uim_dir_type             *file_id,
  uim_aid_type             *aid,
  uim_instance_global_type *uim_ptr
);

static void uim_update_current_dir_uicc
(
  uim_instance_global_type *uim_ptr,
  boolean                   is_ef,
  uim_select_cmd_type      *sel_cmd_ptr,
  uim_channel_type          curr_ch
);

static void uim_update_current_dir_sfi
(
  uim_instance_global_type *uim_ptr,
  uim_path_type            *path,
  uim_channel_type          curr_ch
);

static void uim_update_current_dir_icc
(
  uim_instance_global_type *uim_ptr,
  boolean                   is_ef,
  uim_path_type            *path,
  uint8                     path_position
);

static int uim_compare_toolkit_cmds
(
  void *item_ptr,
  void *param
);

static void uim_remove_toolkit_cmds
(
  void *item_ptr,
  void *param
);

static void uim_save_current_req_buf
(
  uim_instance_global_type *uim_ptr
);

static void uim_restore_current_req_buf
(
  uim_instance_global_type *uim_ptr
);

static uint8 uim_calculate_guard_time
(
  uim_instance_global_type *uim_ptr
);

static uint8 uim_get_start_path_index
(
  uim_instance_global_type *uim_ptr,
  uim_path_type            *select_path_ptr
);

static void uim_select_state
(
  uim_instance_global_type *uim_ptr,
  uim_path_type            *target_path_ptr,
  uim_req_buf_type         *uim_req_buf_static_ptr
);

static void uim_prepare_select_failed_st
(
  uim_instance_global_type *uim_ptr,
  uim_channel_type          curr_ch,
  uim_apdu_status_type      cmd_status
);

static void uim_stream_apdu_success
(
  uim_instance_global_type *uim_ptr,
  uim_req_buf_type         *uim_req_buf_static_ptr,
  uim_rsp_buf_type         *rsp_ptr
);

static void uim_reselect_cur_dir_state
(
  uim_instance_global_type *uim_ptr,
  uim_path_type            *target_path_ptr,
  uim_req_buf_type         *uim_req_buf_static_ptr
);

static uint16 uim_check_characteristics_get_next_df
(
  uim_instance_global_type *uim_ptr,
  uint16                   curr_df
);

void uim_silent_recovery_cb
(
  uim_rpt_type *report_ptr
);

uim_return_type uim_get_euicc_phase
(
  uim_slot_type               slot_id,
  uim_euicc_phase_enum_type   *euicc_phase_ptr
);

void uim_send_get_data_apdu
(
  uim_instance_global_type *uim_ptr
);

void uim_send_store_data_apdu
(
  uim_instance_global_type *uim_ptr
);

uim_return_type uim_set_svn 
(
  uim_slot_type          slot_id,
  uint8                  svn_len,
  uint8                 *svn_ptr
);

uim_return_type uim_parse_eid_resp
(
  uint32                          data_len,
  uint8                          *data_ptr,
  uim_eid_buf_type               *resp_ptr
);

uim_return_type uim_decode_select_isdr_resp 
(
  uint32                                 data_len,
  uint8                                 *data_ptr,
  uim_select_isdr_resp_type             *resp_ptr
);


/*===========================================================================
FUNCTION UIM_IS_TEST_ICCID

DESCRIPTION
  This function checks if the current ICCID is GCF ICCID or not.

RETURN VALUE
  Boolean
    TRUE : If current iccid is same as iccid used in GCF test cases
    FALSE: If current iccid is NOT same as iccid used in GCF test cases

DEPENDENCIES
  None
===========================================================================*/
static boolean uim_is_test_iccid
(
  uim_instance_global_type *uim_ptr
)
{
  if(memcmp(uim_ptr->cache.iccid_buf.data, UIM_TEST_ICCID, UIM_ICCID_SIZE) == 0)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}/* uim_is_test_iccid */


/*=============================================================================
  GENERIC COMMAND STATES
==============================================================================*/
/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_POWER_DOWN_ST
=============================================================================*/
void uimgen_iso_handle_power_down_st
(
  uim_instance_global_type *uim_ptr
)
{
  UIMDRV_MSG_HIGH_0(uim_ptr->id, "Turning off UIM because of POWER down cmd");
  uim_power_down(uim_ptr);

 /*set the logical slot power down flag*/
 if(uim_ptr->command.cmd_ptr->hdr.command  == UIM_POWER_DOWN_F)
 {
   uim_set_logical_slot_power_down_flag(uim_ptr, TRUE);
 }

  /* We are done, notify UIM */
  (void)rex_set_sigs(uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG);
  /* Set the response command status as done */
  uim_ptr->command.rsp_buf.cmd_status = UIM_DONE;
} /* uimgen_iso_handle_power_down_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_POWER_UP_ST
=============================================================================*/
void uimgen_iso_handle_power_up_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Reset globals during power-up */
  uim_reset_power_up_globals(uim_ptr);

  if(UIM_CARD_TELECOM  == uim_ptr->card_mode)
  {
    /* If this is the first time we are executing this command and if the
       uim_first_instn_class is set to GSM, then toggle the flag */
    if (uim_ptr->command.cmd_ptr->hdr.cmd_count == 1)
    {
      if (uim_ptr->state.first_inst_class == NV_UIM_FIRST_INST_CLASS_GSM_SIM)
      {
        uim_ptr->flag.toggle_instrn_class = TRUE;
        /* Set baud rate and clk to 3.8MHz for GSM INST class if CLK is at 4.8MHz */
        UIMDRV_MSG_HIGH_1(uim_ptr->id, "GSM instruction class  - evaluate clk freq change 0x%x",
       UIM_IS_CLK_FREQ_AT_4_8_MHZ(uim_ptr->id));
        if (UIM_IS_CLK_FREQ_AT_4_8_MHZ(uim_ptr->id))
        {
          uim_power_down(uim_ptr);
          UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ(uim_ptr->id);
          SET_UIM_BAUD_RATE_SLOT ( uim_ptr->id, UIM_CRCF_372_1, UIM_BRAF_1 );
        }
      }
    }
  }
  uim_power_up(uim_ptr);
  if(FALSE == uim_ptr->flag.card_activation_indicated_to_mmgsdi &&
     UIM_INTERNAL_WAKE_UP_F != uim_ptr->command.cmd_ptr->hdr.command  &&
     FALSE == uim_ptr->extended_recovery.extended_recovery_in_progress )
  {
    /*Notify mmgsdi of powerup only if the slot is active*/
    if(UIM_SLOT_STATE_ACTIVE == uim_ptr->activity_status)
    {
      (void)uim_interface_mmgsdi_notify_sim_pre_init((uim_slot_type)(uim_ptr->id + 1));
    }
    /*set the notify_card_indication flag, so that multiple indications are
    avoided for the same activation.
    (for instace to avoid when we move to 3v powerup or when there is a clk_fre change)*/
    uim_ptr->flag.card_activation_indicated_to_mmgsdi = TRUE;
  }
#if defined( FEATURE_UIM_T_1_SUPPORT )
  /* Set the current protocol to T=0 during power up */
  uim_ptr->state.current_protocol = UIM_T_0_PROTOCOL;
#endif /* FEATURE_UIM_T_1_SUPPORT */
  /* Signal is set internally for proper operation of state machine */
  if(IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
  {
    (void)rex_set_sigs(uim_ptr->tcb_ptr,UIM_CMD_RSP_SIG);
     return;
  }
  /* This intentional wait is to delay the reset by 20ms. The delay
     timer is associated with the UIM_CMD_RSP_SIG. */
  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_INTERFACE_NOT_USED,
                               uim_ptr) == FALSE)
  {
    uim_timed_sleep(UIM_CARD_DELAY_TIMEOUT, uim_ptr, UIM_ALL_ZERO_SIG);
  (void)rex_set_sigs(uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG);
  }
} /* uimgen_iso_handle_power_up_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RESET_ST
=============================================================================*/
void uimgen_iso_handle_reset_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Indicate that we have not yet received an ATR */
  uim_ptr->atr.atr_received = FALSE;
  uim_ptr->flag.uicc_supports_swp = FALSE;
  /* Set the initial work waiting time */
  /* Compute a new work waiting time */
  if(uim_ptr->setting.phy_layer == UIM_ISO)
  {
    if(uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq == 0)
    {
      UIMDRV_MSG_ERR_0(uim_ptr->id, "uim_clk_freq can not be zero");
      return;
    }
    if(uim_is_fi_di_supported(uim_ptr, uim_ptr->state.op_params_buf.FI,uim_ptr->state.op_params_buf.DI))
    {
      uim_ptr->card_cmd.work_waiting_time = ((960 * 10) *
                            crcf_values[UIM_CRCF_372_1]) /
                           (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000) +
        (480 * crcf_values[UIM_CRCF_372_1]) /
        ( uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000);
      uim_ptr->card_cmd.work_waiting_time_etus = 960*10 + 480;
    }
    else
    {
      uim_ptr->card_cmd.work_waiting_time = ((960 * 10) *
                            crcf_values[UIM_CRCF_372_1]) /
                           (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000) +
                            UIM_CLK_MS_PER_TICK;
      uim_ptr->card_cmd.work_waiting_time_etus = 960*10;
    }

#if defined(FEATURE_UIM_T_1_SUPPORT)
    /* for T=1 protocol, Block Guard Time is 22 Etu's */
    uim_ptr->t1.t_1_bgt = (BGT_22_ETU * crcf_values[UIM_CRCF_372_1]) /
                   (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000) + UIM_CLK_MS_PER_TICK;
#endif /* FEATURE_UIM_T_1_SUPPORT */

    if (uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_TC1_BYTE_FOR_EXTRA_GUARD_TIME, uim_ptr) == FALSE)
    {
      /* Do not use the minimum guardtime.  This is determined in the
         ATR */
      uim_ptr->t1.use_minimum_guardtime_size = FALSE;
    }
  }
  
  /* Set the flag to FALSE for all modes.*/
  uim_ptr->atr.atr_pps_done= FALSE;
  uim_ptr->flag.invalid_pps_received = FALSE;
  
  /* Reset the UIM card */
  uim_reset( &uim_ptr->command.rsp_buf, uim_ptr);

  if(UIM_CARD_TELECOM  == uim_ptr->card_mode)
  {
    /* The protocol is used in one case for voltage control
       while parsing the ATR in the UIM_RESET_ST(response)
       set it properly here */
    if (!uim_ptr->flag.toggle_instrn_class ) 
    {
      /* Set the protocol to UICC */
      uim_ptr->command.cmd_ptr->hdr.protocol = UIM_UICC;
      uim_ptr->state.dfs_1.protocol          = UIM_UICC;
    }
    else
    {
      /* Set the protocol to ICC */
      uim_ptr->command.cmd_ptr->hdr.protocol = UIM_ICC;
      uim_ptr->state.dfs_1.protocol          = UIM_ICC;
    }
  }
} /* uimgen_iso_handle_reset_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_PPS_ST
=============================================================================*/
void uimgen_iso_handle_pps_st
(
  uim_instance_global_type *uim_ptr
)
{
  byte FI = 0, DI = 0; /* Transmission factors from ATR */
  uint16 uim_guard_time  = 0;

  UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_86, uim_ptr->id, "Sending Default PPS");
  /* This uses TA(1) from the ATR to negotiate the PPS. */
  uim_ptr->atr.pps_char_buf[0] = UIM_PPS_DEFAULT_PPSS;
  uim_ptr->atr.no_pps_attempts++;
 /* Separate FI from DI */
  FI = uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX] >> UIM_FI_SHIFT_OF_TA1;
  DI = uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX] & UIM_DI_MASK_OF_TA1;
  /* Check if TA1 is valid */
  /* Default baud-rate for a card with F=372 and D=12 */
  if (((uim_nv_is_feature_enabled(
          UIMDRV_FEATURE_TRY_DEFAULT_BAUD_RATE_FOR_F372_D12_CARD,
        uim_ptr) == TRUE)
        && ((braf_values[DI] == 0) || (crcf_values[FI] == 0) ||
           (UIM_F372_D12_COMB1 == uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX]) ||
           (UIM_F372_D12_COMB2 == uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX])) )
      || ((uim_nv_is_feature_enabled(
          UIMDRV_FEATURE_TRY_DEFAULT_BAUD_RATE_FOR_F372_D12_CARD,
          uim_ptr) == FALSE)
         && ((braf_values[DI] == 0) || (crcf_values[FI] == 0))) )
  {
    uim_ptr->atr.pps_req_buf.size_of_pps_request = UIM_PPS_DEFAULT_REQ_SIZE;
    uim_ptr->atr.pps_char_buf[1] = UIM_PPS_DEFAULT_PPS0;
    uim_ptr->atr.pps_char_buf[2] = UIM_PPS_DEFAULT_PCK;
  }
  else
  {
    /* If we are in the third reset attempt */
    if (uim_ptr->atr.no_pps_attempts > (UIM_MAX_NUM_ATTEMPTS - 1) ||
       (uim_ptr->command.cmd_ptr->hdr.cmd_count > (UIM_MAX_NUM_ATTEMPTS - 1)))
    {
      uim_ptr->atr.pps_req_buf.size_of_pps_request = UIM_PPS_DEFAULT_REQ_SIZE;
      uim_ptr->atr.pps_char_buf[1] = UIM_PPS_DEFAULT_PPS0;
      uim_ptr->atr.pps_char_buf[2] = UIM_PPS_DEFAULT_PCK;
    }
    else
    {
      uim_ptr->atr.pps_req_buf.size_of_pps_request = UIM_PPS_REQ_SIZE;
      uim_ptr->atr.pps_char_buf[1] = (int)UIM_PPS_PPS1_PPS0
#if defined( FEATURE_UIM_T_1_SUPPORT )
        | (int)uim_ptr->state.operating_protocol
#endif /* FEATURE_UIM_T_1_SUPPORT */
      ;
      uim_ptr->atr.pps_char_buf[2] = uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX];

#if defined( FEATURE_UIM_T_1_SUPPORT )
      if (UIM_T_1_PROTOCOL == (int)uim_ptr->state.operating_protocol)
      {
        if (!(uim_ptr->atr.pps_char_buf[2] == UIM_NO_PPS_VALUE1 ||
              uim_ptr->atr.pps_char_buf[2] == UIM_NO_PPS_VALUE2))
        {
          /* CWT value should be greater than or equal to 2ms to support the current design with Max Clock */
          uint32 cwt_in_ms = (uint32)( ((crcf_values[FI] * 1000) * (11 + (1 << uim_ptr->t1.t_1_cwi))) /
                                        (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq * braf_values[DI]));
          if ( cwt_in_ms < 2)
          {
            uim_ptr->atr.pps_char_buf[2] = UIM_NO_PPS_VALUE1;
            UIMDRV_MSG_HIGH_1(uim_ptr->id,
               "Move to default baud rate for T1 protocol due to less value of cwt 0x%x in ms", cwt_in_ms);
          }
        }
      }
#endif /* FEATURE_UIM_T_1_SUPPORT */

      uim_ptr->atr.pps_char_buf[3] =
        (uim_ptr->atr.pps_char_buf[0] ^ uim_ptr->atr.pps_char_buf[1] ^ uim_ptr->atr.pps_char_buf[2]);
    }
  }

  /*intialise with default gaurd time*/
  uim_guard_time = UIM_DEFLT_GUARDTIME_BITS;

  /*set gaurd time for PPS based on ATR*/
  if((uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_TC1_BYTE_FOR_EXTRA_GUARD_TIME, uim_ptr) == TRUE) &&
     (uim_ptr->atr.atr_buf.data[UIM_ATR_T0_BYTE] & (UIM_ATR_TC_PRESENT)))
  {
    /* TC1 value is added to the minimum guard time. for TC1 values other than 0 and 255*/
    if( uim_ptr->atr.atr_tc1_byte != UIM_TC1_IS_0 &&
          uim_ptr->atr.atr_tc1_byte != UIM_TC1_IS_255 )
    {
      uim_guard_time = UIM_DEFLT_GUARDTIME_BITS + uim_ptr->atr.atr_tc1_byte;
    }
  }

  /* !! UART constraint: UART supports only 254 etus as guard time !! */
  if(uim_guard_time > UIM_MAX_POSSIBLE_GUARD_TIME)
  {
    uim_guard_time = UIM_MAX_POSSIBLE_GUARD_TIME;
  }

  /* Set the stop bit number*/
  UIM_UART_CNFG_SLOT(uim_ptr, UART_SIM_CFG__STOP_BIT_LEN_MASK,
                     uim_guard_time <<UART_SIM_CFG__STOP_BIT_LEN_SHFT);
  UIMDRV_MSG_LOW_1(uim_ptr->id,
      "reset gaurd time to 0x%x", uim_guard_time);

  uim_ptr->atr.pps_req_buf.request_buf = uim_ptr->atr.pps_char_buf;
  uim_ptr->atr.pps_req_buf.rsp_ptr = &uim_ptr->command.rsp_buf;
  uimdrv_iso_send_pps(&uim_ptr->atr.pps_req_buf, uim_ptr);
} /* uimgen_iso_handle_pps_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_DELAY_AFTER_ATR_ST
=============================================================================*/
void uimgen_iso_handle_delay_after_atr_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* This intentional wait is to delay sending the PPS request after
   * the reception of the ATR.  To avoid sending the first PPS byte
   * within 12 ETU of the last ATR byte received.  This could happen
   * based on when the last byte is received.. if the byte is already
   * present in the FIFO by the time the ISR is reading bytes out
   */
  uim_timed_sleep(UIM_DELAY_TIME_AFTER_ATR, uim_ptr, UIM_ALL_ZERO_SIG);
  (void)rex_set_sigs(uim_ptr->tcb_ptr,UIM_CMD_RSP_SIG);
} /* uimgen_iso_handle_delay_after_atr_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UPDATE_OP_PARAMS_ST
=============================================================================*/
void uimgen_iso_handle_update_op_params_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Fill out the operational parameters as default values */
  uim_ptr->state.op_params_buf.change_baud_rate = TRUE;
  uim_ptr->state.op_params_buf.change_clk_freq = TRUE;
  uim_ptr->state.op_params_buf.change_guardtime = TRUE;
  uim_ptr->state.op_params_buf.FI = UIM_CRCF_372_1;
  uim_ptr->state.op_params_buf.DI = UIM_BRAF_1;
  if (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq ==
                 uim_clock_frequency[UIMDRV_CLK_FREQ_3_84_MHZ])
  {
    uim_ptr->state.op_params_buf.clock_frequency =
                                        UIMDRV_CLK_FREQ_3_84_MHZ;
  }
  else if(uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq ==
                 uim_clock_frequency[UIMDRV_CLK_FREQ_4_8_MHZ])
  {
    uim_ptr->state.op_params_buf.clock_frequency =
                                        UIMDRV_CLK_FREQ_4_8_MHZ;
  }
  /* Determine if there is an interface byte to process.  This code
     operates on the ATR as well as the PPS response. */
  if (uim_ptr->command.rsp_buf.rsp.data[UIM_ATR_T0_BYTE] & (UIM_ATR_TA_PRESENT))
  {
    /* Set the op parameters from the ATR/PPS response */
    uim_ptr->state.op_params_buf.FI = (uim_clock_rate_conversion_factor_type)
      (uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX] >> UIM_FI_SHIFT_OF_TA1);
    uim_ptr->state.op_params_buf.DI = (uim_baud_rate_adjustment_factor_type)
      (uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX] & UIM_DI_MASK_OF_TA1);
  } /* end if - TA(1) does exist in the ATR/PPS. */

  if (uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_TC1_BYTE_FOR_EXTRA_GUARD_TIME, uim_ptr) == FALSE)
  {
    /* Use the default guardtime size. */
    uim_ptr->state.op_params_buf.guardtime_bits = UIM_DEFLT_GUARDTIME_BITS;
  }

#if defined( FEATURE_UIM_T_1_SUPPORT )
  if( braf_values[uim_ptr->state.op_params_buf.DI] != 0 &&
      uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq != 0 )
  {
    /* Set the waiting times for T=1 protocol */
    /* Setting Block Waiting Time */
    uim_ptr->t1.t_1_bwt =  (rex_timer_cnt_type) (( (11 +
                       (uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_EXTRA_BWT]*
                        UIM_EXTRA_BWT_MUL_FACTOR)
                       )* crcf_values[uim_ptr->state.op_params_buf.FI] * 1000) /
                    ( braf_values[uim_ptr->state.op_params_buf.DI] *
                       uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq ))
                    + (rex_timer_cnt_type) (  ((1 << uim_ptr->t1.t_1_bwi) * 960 *
                        crcf_values[UIM_CRCF_372_1] ) /
                      (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000) )
                    + UIM_CLK_MS_PER_TICK + UIM_EXTRA_BGT_TIME;

    uim_ptr->t1.t_1_bwt_etus = 11 + (((1 << uim_ptr->t1.t_1_bwi) * 960 *
                            braf_values[uim_ptr->state.op_params_buf.DI] * crcf_values[UIM_CRCF_372_1]) / crcf_values[uim_ptr->state.op_params_buf.FI]);

    /* Setting Character Waiting Time */
    uim_ptr->t1.t_1_cwt = ((11 + (1 << uim_ptr->t1.t_1_cwi) +
                    (uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_EXTRA_CWT]*
                     UIM_EXTRA_CWT_MUL_FACTOR)
                   ) * crcf_values[uim_ptr->state.op_params_buf.FI]) /
                   ((uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000) *
                    braf_values[uim_ptr->state.op_params_buf.DI]) + UIM_CLK_MS_PER_TICK;

    uim_ptr->t1.t_1_cwt_etus = 11 + (1 << uim_ptr->t1.t_1_cwi);

      /* for T=1 protocol, Block Guard Time is 22 Etu's */
    uim_ptr->t1.t_1_bgt = ((BGT_22_ETU +
                    (uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_EXTRA_BGT]*
                    UIM_EXTRA_BGT_MUL_FACTOR)
                   )* crcf_values[uim_ptr->state.op_params_buf.FI]) /
                   ((uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000) *
                    braf_values[uim_ptr->state.op_params_buf.DI]) + UIM_CLK_MS_PER_TICK;
   }
   else
   {
       UIMDRV_MSG_ERR_1(uim_ptr->id,"Either Invalid DI value 0x%x or uim_clk_freq is zero",
                                     braf_values[uim_ptr->state.op_params_buf.DI]);
     uim_ptr->flag.clear_timeout_on_response = FALSE;
     (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
     uim_ptr->flag.invalid_pps_received = TRUE;
     return;
   }

   if (uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_TC1_BYTE_FOR_EXTRA_GUARD_TIME, uim_ptr) == FALSE)
   {
     /* Determine if the guard bit time in the minimum for T=1 */
     if ((UIM_T_1_PROTOCOL == uim_ptr->state.operating_protocol) &&
         (uim_ptr->t1.use_minimum_guardtime_size))
     {
       /* Use the default guardtime size. */
       uim_ptr->state.op_params_buf.guardtime_bits = UIM_MINIMUM_T_1_GUARDTIME;
     }
   }

#endif /* FEATURE_UIM_T_1_SUPPORT */
   if((braf_values[uim_ptr->state.op_params_buf.DI] == 0 ||
      uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq == 0))
   {
     UIMDRV_MSG_ERR_0(uim_ptr->id,"Invalid DI value or uim_clk_freq is zero");

     uim_ptr->flag.clear_timeout_on_response = FALSE;
     (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
     uim_ptr->flag.invalid_pps_received = TRUE;
     return;
   }

   if (uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_TC1_BYTE_FOR_EXTRA_GUARD_TIME, uim_ptr) == TRUE)
   {
      /* Determine the guard-time based on the ATR */
      uim_ptr->state.op_params_buf.guardtime_bits = uim_calculate_guard_time(uim_ptr);
   }

  /* Add extra guard time from uim config params NV 4205 */
  uim_ptr->state.op_params_buf.guardtime_bits +=
    uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_EXTRA_GUARD_TIME];

  /* Send the operational parameters */
  uim_update_op_params( &uim_ptr->state.op_params_buf, uim_ptr);

  if (uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_EXTRA_WWT] == 0)
  {
    /* Initialize Extra Work Waiting Time to 1 to prevent multiplication by zero */
    uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_EXTRA_WWT] = 1;
  }
  if(braf_values[uim_ptr->state.op_params_buf.DI] != 0)
  {
    uim_ptr->card_cmd.work_waiting_time = ((960 * uim_ptr->state.WI) *
                          uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_EXTRA_WWT] *
                          crcf_values[uim_ptr->state.op_params_buf.FI]) /
                         (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000) +
      (480 * crcf_values[uim_ptr->state.op_params_buf.FI]) /
      (braf_values[uim_ptr->state.op_params_buf.DI] * ( uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000));
    uim_ptr->card_cmd.work_waiting_time_etus = (960 * uim_ptr->state.WI * braf_values[uim_ptr->state.op_params_buf.DI]) + 480;

  }
  else
  {
    uim_ptr->card_cmd.work_waiting_time = ((960 * uim_ptr->state.WI) *
                          uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_EXTRA_WWT] *
                          crcf_values[uim_ptr->state.op_params_buf.FI]) /
                         (uim_hw_if.clkreg[uim_ptr->id].m_simClkFreq / 1000) + UIM_CLK_MS_PER_TICK;
    uim_ptr->card_cmd.work_waiting_time_etus = (960 * uim_ptr->state.WI) + 480;
  }

  /* Extend work waiting time for some out of spec RUIM cards */
  uim_ptr->card_cmd.original_work_waiting_time =
                                  uim_ptr->card_cmd.work_waiting_time;
  uim_ptr->card_cmd.original_work_waiting_time_etus =
                                  uim_ptr->card_cmd.work_waiting_time_etus;
  uim_ptr->card_cmd.extended_work_waiting_time =
                              2 * uim_ptr->card_cmd.work_waiting_time;
  uim_ptr->card_cmd.extended_work_waiting_time_etus =
                              2 * uim_ptr->card_cmd.work_waiting_time_etus;

  /* Signal is set internally for proper operation of state machine */
  uim_timed_sleep(UIM_CARD_DELAY_TIMEOUT, uim_ptr, UIM_ALL_ZERO_SIG);
  (void)rex_set_sigs(uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG);
}/* uimgen_iso_handle_update_op_params_st */


#if defined (FEATURE_UIM_T_1_SUPPORT)
/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_IFS_NEG_ST
=============================================================================*/
void uimgen_iso_handle_ifs_neg_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* This command does not use these parameters.  Set them to a benign
     command, just in case */
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_IFS_BLOCK;

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  /* This is the IFS parameter that indicates the maximum number of bytes
     the MSM can accept in a block */
  uim_req_buf_static_ptr->req.data[0] =
    int32touint8(UIM_MAX_TRANSACTION_SIZE -
                 (UIM_T_1_NUM_BYTES_PROLOGUE + uim_ptr->t1.epilogue_field_size));

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_ifs_neg_st */
#endif /* FEATURE_UIM_T_1_SUPPORT */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_ICCID_ST
=============================================================================*/
void uimgen_iso_handle_read_iccid_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_SUPPORT_NO_ICCID,
                                     uim_ptr) == TRUE)
  {
    /* Skip read iccid if select failed. */
    if (uim_ptr->flag.iccid_select_failed)
    {
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_87,uim_ptr->id,"uim_ptr->flag.iccid_select_failed is TRUE");
      uim_ptr->flag.iccid_select_failed = FALSE;

      uim_ptr->command.generic_state_ptr++;
      if (uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_WAKE_UP_F)
      {
        if(MMGSDI_SUCCESS ==
           uim_interface_mmgsdi_get_recovery_data_sync(&uim_ptr->state.recovery_data,
                                         (uim_slot_type)(uim_ptr->id + 1)))
        {
            UIMDRV_MSG_HIGH_1(uim_ptr->id,"Recovering with protocol 0x%x", uim_ptr->command.cmd_ptr->hdr.protocol);

          uim_ptr->command.cmd_ptr->hdr.protocol = uim_ptr->state.recovery_data.protocol;
          if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
          {
            /* Proceed to recover UICC card */
            uim_ptr->command.generic_state_ptr = UIM_RECOVER_UICC_SLOT_STATES;
          }
          else
          {
            /* Proceed to recover ICC card */
            uim_ptr->command.generic_state_ptr = UIM_RECOVER_ICC_SLOT_STATES;
          }
        }
      }
      /* go to process next command */
      uim_generic_command (uim_ptr);
      return;
    }
  }

  uim_req_buf_static_ptr->apdu_hdr.instrn = READ_BINARY;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;
  uim_req_buf_static_ptr->apdu_hdr.p1 = UIM_ICCID_OFFSET;
  uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_ICCID_OFFSET;
  uim_req_buf_static_ptr->apdu_hdr.p3 = (uim_ptr->cache.iccid_buf.num_bytes > 0) ?
                                              uim_ptr->cache.iccid_buf.num_bytes :
                                              UIM_ICCID_SIZE;

  /* Specifically select the ICCID EF. */
  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UICC_INSTRUCTION_CLASS1;
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_read_iccid_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_CHECK_FOR_CDMA_DF_ST
=============================================================================*/
void uimgen_iso_handle_check_for_cdma_df_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    /* Skip the CDMA DF check if UICC protocol has been chosen */
    ++uim_ptr->command.generic_state_ptr;
    /* Process the next state. */
    uim_generic_command (uim_ptr);
    return;
  }

  /* Reset the select state counter */
  uim_ptr->command.cmd_ptr->select.path_position = 0;

  if (uim_ptr->flag.uim_auth_cdma_df_first == FALSE)
  {
    uim_ptr->command.cmd_ptr->select.path.path[0] = UIM_CDMA_DF;
  }
  else
  {
    uim_ptr->command.cmd_ptr->select.path.path[0] = UIM_GSM_DF;
  }

  uim_ptr->command.cmd_ptr->select.path.len = 1;

  /* Reset select state control to start selecting from the first DF */
  uim_ptr->command.cmd_ptr->select.path_position = 0;

  uim_select_state(uim_ptr, &uim_ptr->command.cmd_ptr->select.path, uim_req_buf_static_ptr);
} /* uimgen_iso_handle_check_for_cdma_df_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_CHECK_CHARACTERISTICS_ST
=============================================================================*/
void uimgen_iso_handle_check_characteristics_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if (!uim_ptr->flag.toggle_instrn_class ) 
  {
    /* Save the protocol as UICC - drivers layer uses the req_buf protocol
       to make decisions on apdu settings.  It must be correct from here on */
    uim_ptr->state.dfs_1.protocol                 = UIM_UICC;
    uim_req_buf_static_ptr->protocol              = UIM_UICC;
    uim_ptr->command.cmd_ptr->hdr.protocol        = UIM_UICC;

    uim_ptr->command.cmd_ptr->select.path.path[0] = 0x3F00;
    uim_ptr->command.cmd_ptr->select.path.len     = 1;
  }
  else
  {
    /* Set the protocol to ICC - drivers layer uses the req_buf protocol
       to make decisions on apdu settings.  It must be correct from here on */
    uim_ptr->command.cmd_ptr->hdr.protocol         = UIM_ICC;
    uim_ptr->command.static_cmd_buf.hdr.protocol   = UIM_ICC;
    uim_req_buf_static_ptr->protocol               = UIM_ICC;
    uim_ptr->state.dfs_1.protocol                  = UIM_ICC;

    uim_ptr->command.cmd_ptr->select.path_position = 0;

    uim_ptr->command.cmd_ptr->select.path.path[0] =
      uim_check_characteristics_get_next_df(uim_ptr, uim_ptr->command.cmd_ptr->select.path.path[0]);
  }
  uim_select_state(uim_ptr, &uim_ptr->command.cmd_ptr->select.path, uim_req_buf_static_ptr);
} /* uimgen_iso_handle_check_characteristics_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_ICCID_ST
=============================================================================*/
void uimgen_iso_handle_select_iccid_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* path position is not used in the power-up states up until this point.
     For ICC we select mf/iccid incrementally - this state is reentered
     after MF is selected.  If it's ICC and we just selected MF we should
     proceed selecting ICCID otherwise we should set the path to be selected */
  if((uim_ptr->command.cmd_ptr->select.path_position != 1)
     ||(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC))
  {
    uim_ptr->command.cmd_ptr->select.file_type    = UIM_EF;
    uim_ptr->command.cmd_ptr->select.path.path[0] = UIM_MF_ID;
    uim_ptr->command.cmd_ptr->select.path.path[1] = UIM_ICCID_EF_ID;
    uim_ptr->command.cmd_ptr->select.path.len     = 2;
  }
  uim_select_state(uim_ptr, &uim_ptr->command.cmd_ptr->select.path, uim_req_buf_static_ptr);
} /* uimgen_iso_handle_select_iccid_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_EF_DIR_ST
=============================================================================*/
void uimgen_iso_handle_select_ef_dir_st
(
  uim_instance_global_type *uim_ptr
)
{
 
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;
  
   if(uim_ptr == NULL)
  {
    return;
  }

  if(!uim_nv_is_feature_enabled(UIMDRV_FEATURE_READ_NUM_OF_VALID_APPS, uim_ptr) ||
     (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC))
  {
    uim_ptr->command.generic_state_ptr ++;
    uim_generic_command (uim_ptr);
    return;
  }  

  if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_ptr->command.cmd_ptr->select.file_type    = UIM_EF;
    uim_ptr->command.cmd_ptr->select.path.path[0] = UIM_MF_ID;
    uim_ptr->command.cmd_ptr->select.path.path[1] = UIM_DIR_EF_ID;
    uim_ptr->command.cmd_ptr->select.path.len     = 2;
    uim_select_state(uim_ptr, &uim_ptr->command.cmd_ptr->select.path, uim_req_buf_static_ptr);    
  }
} /* uimgen_iso_handle_select_ef_dir_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_EF_DIR_ST
=============================================================================*/
void uimgen_iso_handle_read_ef_dir_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if(!uim_nv_is_feature_enabled(UIMDRV_FEATURE_READ_NUM_OF_VALID_APPS, uim_ptr) ||
     (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC))
  {
    uim_ptr->command.generic_state_ptr ++;
    uim_generic_command (uim_ptr);
    return;
  }   

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.instrn = READ_RECORD;
    uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;
    uim_req_buf_static_ptr->apdu_hdr.p1 = ++uim_ptr->cache.num_apps.num_apps_read;
    uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_ABS_RECORD_MODE;
    uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->cache.num_apps.record_len;

    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UICC_INSTRUCTION_CLASS1;

    uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

    uim_send_command(uim_req_buf_static_ptr, uim_ptr);
  }
} /* uimgen_iso_handle_read_ef_dir_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_UMPC_ST
=============================================================================*/
void uimgen_iso_handle_read_umpc_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if(UIM_INTERNAL_WAKE_UP_F == uim_ptr->command.cmd_ptr->hdr.command &&
    FALSE == uim_ptr->flag.is_rel_12_uicc)
  {
    UIMDRV_MSG_LOW_0(uim_ptr->id, "Skipping read_umpc during recovery for non rel 12 card");
    uim_ptr->command.generic_state_ptr++;
    uim_generic_command (uim_ptr);
    return;
  }
  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.instrn = READ_BINARY;
    uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;
    uim_req_buf_static_ptr->apdu_hdr.p1 = (UIM_SFI_USED_BITS_B6_TO_B8 |
                  (UIM_UMPC_SFI & UIM_SFI_BITS_B1_TO_B5_MASK));
    uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_UMPC_OFFSET;
    uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_UMPC_SIZE;

    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UICC_INSTRUCTION_CLASS1;

    uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

    uim_send_command(uim_req_buf_static_ptr, uim_ptr);
  }
  else
  {
    uim_ptr->command.generic_state_ptr++;
    uim_generic_command (uim_ptr);
  }
} /* uimgen_iso_handle_read_umpc_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_FAILED_ST
=============================================================================*/
void uimgen_iso_handle_select_failed_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  uim_select_state(uim_ptr, &uim_ptr->command.cmd_ptr->select.path, uim_req_buf_static_ptr);
} /* uimgen_iso_handle_select_failed_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_STREAM_SELECT_ST
=============================================================================*/
void uimgen_iso_handle_stream_select_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr       = NULL;
  uim_path_type     path_for_stream_select_by_df;

  if(uim_ptr == NULL)
  {
    return;
  }
  
  uim_req_buf_static_ptr = &uim_ptr->command.req_buf;
  memset(&path_for_stream_select_by_df, 0x00, sizeof(uim_path_type));

  if((uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_P1_OFFSET] & UIM_USIM_SELECT_BY_DF_NAME) &&
     (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC))
  {
    /* We would simply force a select on 7fff and return
     * the result.  The response would be similar to that of
     * selection by AID */
    path_for_stream_select_by_df.path[0] = 0x3F00;
    path_for_stream_select_by_df.path[1] = 0x7FFF;
    path_for_stream_select_by_df.len     = 2;

    uim_select_state(uim_ptr, &path_for_stream_select_by_df, uim_req_buf_static_ptr);
  }
  else
  {
    /* This can be a select by path or a select by file id any
       other p1 is filtered out in the 7816_STREAM_F handling */
    uim_stream_apdu_state(uim_ptr, uim_req_buf_static_ptr);
  }
} /* uimgen_iso_handle_stream_select_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_ST
=============================================================================*/
void uimgen_iso_handle_select_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  uim_select_state(uim_ptr, &uim_ptr->command.cmd_ptr->select.path, uim_req_buf_static_ptr);
} /* uimgen_iso_handle_select_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RESELECT_CURR_DIR_ST
=============================================================================*/
void uimgen_iso_handle_reselect_curr_dir_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  uim_reselect_cur_dir_state(uim_ptr,
      &uim_ptr->state.current_path[uim_ptr->command.cmd_ptr->hdr.channel].path,
      uim_req_buf_static_ptr);
} /* uimgen_iso_handle_reselect_curr_dir_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_INIT_MANAGE_CHANNEL_ISDR_SEL_ST
=============================================================================*/
void uimgen_iso_handle_init_manage_channel_isdr_sel_st
(
  uim_instance_global_type *uim_ptr
)
{
   if(uim_ptr == NULL)
  {
    return;
  }

  /* If the card is not eUICC, exit from this state */
  if(!uim_ptr->flag.is_euicc ||
     (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC))
  {
    /* Do not execute Manage Channel in case of non-eUICC card.
	   Execute the Done state */
    uim_ptr->command.generic_state_ptr += 5;
	uim_generic_command(uim_ptr);
    return;
  }

  /* Fill in the params to open channel to select ISDR */
  uim_ptr->command.cmd_ptr->manage_channel.mc_action        = UIM_MC_OPEN;
  uim_ptr->command.cmd_ptr->manage_channel.channel          = 0x00;
  uim_ptr->command.cmd_ptr->manage_channel.op_close_channel = 0x00; /* get a channel number allocated by the eUICC */
  
  /* Indicate this is for select ISDR, so that this channel number can be cached
     and later used to close the channel  */
  uim_ptr->flag.is_select_isdr = TRUE;

  uim_ptr->command.generic_state_ptr++;
  uim_generic_command(uim_ptr);
  
  return;
} /* uimgen_iso_handle_init_manage_channel_isdr_sel_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_ISDR_ST
=============================================================================*/
void uimgen_iso_handle_select_isdr_st
(
  uim_instance_global_type *uim_ptr
)
{
  typedef struct
  {
    uint8 aid_len;
	uint8 aid[UIM_EUICC_AID_LEN];
  } uim_euicc_isdr_select_aid_type;

  uim_euicc_isdr_select_aid_type  uim_euicc_isdr_select_aid  = {16,{0xA0, 0x00, 0x00, 0x05,
                                                                0x59, 0x10, 0x10, 0xFF,
                                                                0xFF, 0xFF, 0xFF, 0x89,
																0x00, 0x00, 0x01, 0x00}};
  uim_req_buf_type *uim_req_buf_static_ptr                   = &uim_ptr->command.req_buf;

  if(uim_ptr == NULL)
  {
    return;
  }

  /* If the card is not eUICC, exit from this state */
  if(!uim_ptr->flag.is_euicc ||
     (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC))
  {
	uim_ptr->command.generic_state_ptr += 3;
	uim_generic_command(uim_ptr);
    return;
  }
  
  /* Mention the channel this command has to go through. 
     This should have been retrieved from an earlier manage channel cmd  */
  uim_ptr->command.cmd_ptr->hdr.channel         = uim_ptr->select_isdr_ch_num.channel;

  uim_ptr->command.cmd_ptr->select.file_type    = UIM_ADF;
  uim_ptr->command.cmd_ptr->select.session      = UIM_ACTIVATION_WITH_FCI;
  uim_ptr->command.cmd_ptr->select.path.path[0] = UIM_MF_ID;
  uim_ptr->command.cmd_ptr->select.path.path[1] = UIM_SEL_AID_BY_FILE_ID;
  uim_ptr->command.cmd_ptr->select.path.len     = 2;

  /* Copy the AID */
  memscpy(uim_ptr->command.cmd_ptr->select.aid.data,
	      sizeof(uim_ptr->command.cmd_ptr->select.aid.data),
		  uim_euicc_isdr_select_aid.aid,
		  uim_euicc_isdr_select_aid.aid_len);

  uim_ptr->command.cmd_ptr->select.aid.aid_length = uim_euicc_isdr_select_aid.aid_len;

  uim_select_state(uim_ptr, &uim_ptr->command.cmd_ptr->select.path, uim_req_buf_static_ptr);
} /* uimgen_iso_handle_select_isdr_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_GET_EID_ST
=============================================================================*/
void uimgen_iso_handle_get_eid_st
(
  uim_instance_global_type *uim_ptr
)
{
  uim_euicc_phase_enum_type euicc_phase;

  if(uim_ptr == NULL)
  {
    return;
  }
  
  /* If the card is not eUICC, exit from this state */
  if(!uim_ptr->flag.is_euicc ||
     (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC))
  {
	uim_ptr->command.generic_state_ptr += 2;
	uim_generic_command(uim_ptr);
    return;
  }

  memset(&euicc_phase, 0x00, sizeof(uim_euicc_phase_enum_type));

  (void)uim_get_euicc_phase(uim_ptr->logical_id, &euicc_phase);
  
  if(euicc_phase == UIM_PHASE1_EUICC ||
     euicc_phase == UIM_PHASE1_1_EUICC)
  {
    uim_send_get_data_apdu(uim_ptr);
  }
  else if(euicc_phase == UIM_PHASE2_EUICC)
  {
    uim_send_store_data_apdu(uim_ptr);
  }
} /* uimgen_iso_handle_get_eid_st */


/*=============================================================================
  FUNCTION UIM_RETRIEVE_TERMINAL_CAPABILITY
=============================================================================*/
void uim_retrieve_terminal_capability
(
  uim_slot_type   slot,
  uint8*          num_tlvs_ptr,
  uint8*          tlvs_data_len_ptr,
  uint8*          tlvs_data_ptr
)
{
  uim_instance_global_type         *uim_ptr                    = NULL;
  boolean                          is_lpdd_supported           = FALSE;
  boolean                          is_luid_supported           = FALSE;
  boolean                          is_ldsd_supported           = FALSE;
  boolean                          is_luie_supported           = FALSE;
  uint16                           i                           = 0;
  boolean                          get_additional_interface_support_from_modem =
                                     TRUE;
  boolean                          get_lpa_support_from_modem  = TRUE;
  uint16                           next_tag_index              = 0;
  uint8                            *file_data_ptr              = NULL;
  uint16                           file_data_len               = 0;
  uint8                            current_tlv_len             = 0;
  uint8                            current_tlv_tag             = 0;
  uint8                            skip_current_tlv            = 0;
  uint8                            j                           = 0;
  uint8                            default_tlvs_data_len       = 13;
  /* 5 bytes for the 'power supply support' TLV,
     2 bytes for the 'logical channels support' TLV,
     3 bytes for the 'additional interfaces support' TLV,
     3 bytes for the 'LPA support' TLV */
  uint8                            in_tlvs_data_ptr_len        = 0;

  if(tlvs_data_len_ptr == NULL ||
     tlvs_data_ptr == NULL ||
     num_tlvs_ptr == NULL ||
     *tlvs_data_len_ptr < default_tlvs_data_len ||
     *tlvs_data_len_ptr > (UIM_MAX_TRANS_SIZE_OUT - UIM_TERM_CAPABILITY_LEN_BYTE_OFFSET))
  {
    return;
  }

  in_tlvs_data_ptr_len = *tlvs_data_len_ptr;
  *num_tlvs_ptr = 0;
  *tlvs_data_len_ptr = 0;

  /* Convert logical to Physical ID */
  slot = uim_convert_logical_to_physical_slot_id(slot);

  uim_ptr = uim_get_instance_ptr((uim_instance_enum_type)(slot - 1));
  if(uim_ptr == NULL)
  {
    return;
  }

  /* Read the terminal capability TLVs that the AP wants to send to the
     card. Those ones take precedence over the modem values. First read
     the first two bytes of the file in order to get the file length */
  if(UIM_COMMON_EFS_SUCCESS == uim_common_efs_read(UIM_COMMON_EFS_UIMQMI_QMI_UIM_TERMINAL_CAPABILITY,
                                                   UIM_COMMON_EFS_REGULAR_FILE_TYPE,
                                                   (uim_common_efs_context_type)(uim_ptr->logical_id - 1),
                                                   (uint8 *)&file_data_len,
                                                   UIM_COMMON_TERMINAL_CAPABILITY_EFS_LEN_BYTES) &&
     file_data_len > UIM_COMMON_TERMINAL_CAPABILITY_EFS_LEN_BYTES)
  {
    /* tlvs_data_ptr is going to contain the entire terminal capability TLVs,
       including the ones from the EFS file and the standard/default ones,
       if not over-written in the EFS file. tlvs_data_ptr can only hold a
       max of 253 bytes (255 max APDU data minus 2 bytes for the master TC
       tag and length). Hence, this code ensures that we don't end up copying
       more than the allowed number of bytes to this data ptr */

    /* file_data_ptr will contain the data read from the EFS file */
    file_data_ptr = uim_malloc(file_data_len);
    if(file_data_ptr == NULL)
    {
      return;
    }

    UIMDRV_MSG_ERR_2(uim_ptr->id, "Reading 0x%x bytes from slot 0x%x", file_data_len, slot);
    /* Read the entire file */
    if(UIM_COMMON_EFS_SUCCESS == uim_common_efs_read(UIM_COMMON_EFS_UIMQMI_QMI_UIM_TERMINAL_CAPABILITY,
                                                     UIM_COMMON_EFS_REGULAR_FILE_TYPE,
                                                     (uim_common_efs_context_type)(uim_ptr->logical_id - 1),
                                                     file_data_ptr,
                                                     file_data_len))
    {
      i = UIM_COMMON_TERMINAL_CAPABILITY_EFS_LEN_BYTES;
      while(i < (file_data_len - 1) &&
            j < (in_tlvs_data_ptr_len - default_tlvs_data_len))
      {
        uint8          index = 0;

        current_tlv_tag = file_data_ptr[i];

        /* Move to the index containing the length */
        i++;
        if(i < file_data_len)
        {
          current_tlv_len = file_data_ptr[i];
        }
        else
        {
          /* At this point we stop peeking into the data returned from the
             file. We do not flag it as error condition and return the terminal
             capability comprising the TLVs parsed successfully thus far */
          break;
        }

        if(current_tlv_len == 0 && (i+1) < file_data_len)
        {
          skip_current_tlv = file_data_ptr[i+1];
          next_tag_index = i + 2; /* i points to "length" of current tlv... next
                                     TLV starts at i + 2 */
        }
        else if(current_tlv_len > 0 && (i+1+current_tlv_len) < file_data_len)
        {
          skip_current_tlv = file_data_ptr[i+1+current_tlv_len];
          next_tag_index = i + 2 +current_tlv_len; /* i points to "length" of
                                                      current tlv... next TLV
                                                      starts at i+2+current_tlv_len */
        }
        else
        {
          j = 0;
          break;
        }

        if(skip_current_tlv != 0x00 &&
           skip_current_tlv != 0x01)
        {
          j = 0;
          break;
        }

        if(current_tlv_tag == 0x80 ||
           current_tlv_tag == 0x81 ||
           (skip_current_tlv &&
            current_tlv_tag != UIM_ADDITIONAL_INTERFACE_SUPPORTED_TAG &&
            current_tlv_tag != 0x83))
        {
          i = next_tag_index;
          continue;
        }

        if(current_tlv_tag == UIM_ADDITIONAL_INTERFACE_SUPPORTED_TAG)
        {
          if(skip_current_tlv)
          {
            i = next_tag_index;
            get_additional_interface_support_from_modem = FALSE;
            continue;
          }
          if(current_tlv_len != 0x01)
          {
            i = next_tag_index;
            continue;
          }
          /* Set the flag which will later help in using the TLV value from the
             EFS file rather than from the modem itself */
          get_additional_interface_support_from_modem = FALSE;
        }
        else if(current_tlv_tag == 0x83)
        {
          if(skip_current_tlv)
          {
            i = next_tag_index;
            get_lpa_support_from_modem = FALSE;
            continue;
          }
          if(current_tlv_len != 0x01)
          {
            i = next_tag_index;
            continue;
          }
          /* Set the flag which will later help in using the TLV value from the
             EFS file rather than from the modem itself */
          get_lpa_support_from_modem = FALSE;
        }

        if((j + current_tlv_len + 1) >=
             (in_tlvs_data_ptr_len - default_tlvs_data_len))
        {
          /* We copied over the maximum number of TLVs we could, from the EFS
             file. */
          break;
        }
        tlvs_data_ptr[j++] = current_tlv_tag;
        tlvs_data_ptr[j] = current_tlv_len;
        for(index = 1; index <= current_tlv_len; index++)
        {
          j++;
          /* Move to the data byte(s) of the TLV in the file data ptr */
          i++;
          tlvs_data_ptr[j] = file_data_ptr[i];
        }
        i += 2; /* Skip past the remove_tlv part */
        if (i != next_tag_index)
        {
          j = 0;
          break;
        }
        j++;
        (*num_tlvs_ptr)++;
      }
    }
  }

  UIM_FREE(file_data_ptr);

  /* We have retrieved all the valid TLVs from the EFS file. Now lets
     add the values from modem side for any of the tags, as applicable */

  /* Tag - Power supply support indication */
  tlvs_data_ptr[j++] = 0x80;
  /* Length - as defined by ETSI 102 221 */
  tlvs_data_ptr[j++] = 0x03;
  /* Value 1 - for indicating the used voltage class.
              bits b1 to b6 - xxxx1x - Class B
              bits b1 to b6 - xxx1xx - Class C */
  /* Value 2 - for indicating the max power capability.
              Per 102 221, 0x3C (60mA) is the max possible value.
              Value set to 60mA as our PMIC's are capable of supporting this load.*/
  /* Value 3 - for indicating the used clk freq.
              0xFF implies that no clk freq is indicated */
  if(uim_ptr->state.current_voltage_class == UIM_VOLTAGE_CLASS_C)
  {
    tlvs_data_ptr[j++] = 0x04;
    tlvs_data_ptr[j++] = TERMINAL_MAX_POWER_CAPABILITY_CLASS_C;
  }
  else
  {
    tlvs_data_ptr[j++] = 0x02;
    tlvs_data_ptr[j++] = TERMINAL_MAX_POWER_CAPABILITY_CLASS_B;
  }
  tlvs_data_ptr[j++] = 0xFF;
  (*num_tlvs_ptr)++;

  /* Tag - Extended logical channels support indication */
  tlvs_data_ptr[j++] = 0x81;
  /* Length - as defined by ETSI 102 221 */
  tlvs_data_ptr[j++] = 0x00;
  (*num_tlvs_ptr)++;

  /* Tag - Additional interface support indication */
  if(get_additional_interface_support_from_modem)
  {
    tlvs_data_ptr[j++] = UIM_ADDITIONAL_INTERFACE_SUPPORTED_TAG;
    /* Length - as defined by ETSI 102 221 */
    tlvs_data_ptr[j++] = UIM_ADDITIONAL_INTERFACE_SUPPORTED_LENGTH;
    /* Value */
    if( TRUE == uim_ptr->flag.uim_swp_in_terminal_capability)
    {
      tlvs_data_ptr[j++] = UIM_ADDITIONAL_INTERFACE_SUPPORTED;
    }
    else
    {
     /* This value is default FALSE */
     tlvs_data_ptr[j++] = UIM_ADDITIONAL_INTERFACE_NOT_SUPPORTED;
    }
    (*num_tlvs_ptr)++;
  }

  if(uim_ptr->flag.is_euicc &&
     get_lpa_support_from_modem)
  {
    /* Tag - eUICC support */
    tlvs_data_ptr[j++] = 0x83;
    /* Length - as defined by SGP22 */
    tlvs_data_ptr[j++] = 0x01;

    /* Value 1 - for indicating LUI and LPD support.
       -   -   -   -   -   -   -   1   Local User Interface (LUId) for eUICC supported
       -   -   -   -   -   -   -   0   Local User Interface (LUId) for eUICC not supported
       -   -   -   -   -   -   1   -   Local Profile Download (LPDd) for eUICC supported
       -   -   -   -   -   -   0   -   Local Profile Download (LPDd) for eUICC not supported
       -   -   -   -   -   1   -   -   Local Discovery Service (LDSd) for eUICC supported
       -   -   -   -   -   0   -   -   Local Discovery Service (LDSd) for eUICC not supported
       -   -   -   -   1   -   -   -   LUI when LPA is in the eUICC (LUIe) based on SCWS supported
       -   -   -   -   0   -   -   -   LUI when LPA is in the eUICC (LUIe) based on SCWS not supported
       x   x   x   x   x   x   -   -   RFU
    */
    lpa_is_lpa_supported(&is_lpdd_supported, &is_luid_supported, &is_ldsd_supported, &is_luie_supported, NULL);

    tlvs_data_ptr[j++] = (((uint8)is_luie_supported << 0x03)  |
                          ((uint8)is_ldsd_supported << 0x02)  |
                          ((uint8)is_lpdd_supported << 0x01)  |
                           (uint8)is_luid_supported );
    (*num_tlvs_ptr)++;
  }

  *tlvs_data_len_ptr = j;
} /* uim_retrieve_terminal_capability */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_TERMINAL_CAPABILITY_ST
=============================================================================*/
void uimgen_iso_handle_terminal_capability_st
(
  uim_instance_global_type *uim_ptr
)
{
  uint8             num_tlvs                = 0;
  uint8             *tlvs_data_ptr          = NULL;
  uim_req_buf_type  *uim_req_buf_static_ptr = NULL;
  uint8             tlvs_data_len           = (UIM_MAX_TRANS_SIZE_OUT - UIM_TERM_CAPABILITY_LEN_BYTE_OFFSET);

  /* Allocate the pointer used for retrieving the TC TLVs. Ofcourse, the data
     to be sent out in TC can at max be 255 bytes... hence, malloc being
     done for 255 minus 2 bytes (for TC master tag 0xA9 and the length)*/
  tlvs_data_ptr = uim_malloc(tlvs_data_len);

  if(tlvs_data_ptr == NULL)
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id, "Skipping sending terminal capability");
    return;
  }

  uim_retrieve_terminal_capability(
    (uim_slot_type)(uim_ptr->logical_id),
    &num_tlvs,
    &tlvs_data_len,
    tlvs_data_ptr);

  if(tlvs_data_len > (UIM_MAX_TRANS_SIZE_OUT - UIM_TERM_CAPABILITY_LEN_BYTE_OFFSET))
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id, "Skipping sending terminal capability");
    UIM_FREE(tlvs_data_ptr);
    return;
  }

  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  uim_req_buf_static_ptr->apdu_hdr.instrn    = UIM_TERMINAL_CAPABILITY;
  uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UICC_INSTRUCTION_CLASS2;
  uim_req_buf_static_ptr->instrn_case        = UIM_INSTRN_CASE_3;
  uim_req_buf_static_ptr->apdu_hdr.p1        = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2        = 0;

  /* Initialize p3 length */
  uim_req_buf_static_ptr->apdu_hdr.p3        = 0x00;
  /* Tag - Terminal capability constructed */
  uim_req_buf_static_ptr->req.data[uim_req_buf_static_ptr->apdu_hdr.p3++]   = 0xA9;
  /* Length - Initialize length of proprietary data to 0x00 */
  uim_req_buf_static_ptr->req.data[uim_req_buf_static_ptr->apdu_hdr.p3++]   = 0x00;

  if(sizeof(uim_req_buf_static_ptr->req.data) > uim_req_buf_static_ptr->apdu_hdr.p3 &&
     tlvs_data_len > 0 &&
     num_tlvs > 0 &&
     tlvs_data_ptr)
  {
    uim_memscpy(&uim_req_buf_static_ptr->req.data[uim_req_buf_static_ptr->apdu_hdr.p3],
                sizeof(uim_req_buf_static_ptr->req.data) - uim_req_buf_static_ptr->apdu_hdr.p3,
                tlvs_data_ptr,
                tlvs_data_len);
    /* Adjusting the length of proprietary data */
    uim_req_buf_static_ptr->req.data[1] = tlvs_data_len;
    uim_req_buf_static_ptr->apdu_hdr.p3 += tlvs_data_len;
  }

  if(uim_ptr->flag.uim_swp_in_terminal_capability &&
     uim_ptr->flag.uicc_supports_swp)
  {
    uim_ptr->setting.voltage_indication_to_qmi.is_ack_needed = TRUE;
  }
  else
  {
    /* This value is default FALSE */
    uim_ptr->setting.voltage_indication_to_qmi.is_ack_needed = FALSE;
  }

  uim_req_buf_static_ptr->rsp_ptr            = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);

  UIM_FREE(tlvs_data_ptr);
} /* uimgen_iso_handle_terminal_capability_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SEND_STATUS_ST
=============================================================================*/
void uimgen_iso_handle_send_status_st
(
  uim_instance_global_type *uim_ptr
)
{
  ++uim_ptr->command.generic_state_ptr;
  uim_generic_command(uim_ptr);
} /* uimgen_iso_handle_send_status_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_STATUS_ST
=============================================================================*/
void uimgen_iso_handle_status_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Get the status (last selected DF) of the UIM card */
  uim_req_buf_static_ptr->apdu_hdr.instrn = STATUS;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
      uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS2, uim_ptr->command.cmd_ptr->hdr.channel);

    uim_req_buf_static_ptr->apdu_hdr.p1 = (int)uim_ptr->command.cmd_ptr->status.status;
    uim_req_buf_static_ptr->apdu_hdr.p2 = (int)uim_ptr->command.cmd_ptr->status.return_data;

    if ( uim_ptr->command.cmd_ptr->status.return_data == UIM_RETURN_NONE )
    {
      /* expect nothing in the response */
      uim_req_buf_static_ptr->apdu_hdr.p3 = 0;
      uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_1;
    }
    else if((uim_ptr->command.cmd_ptr->status.return_data == UIM_STATUS_P2_DF_NAME_OF_CURR_APP) &&
            (uim_ptr->command.cmd_ptr->status.aid.aid_length != 0))
    {
      uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->status.aid.aid_length + UIM_AID_TLV_LEN_OFFSET;
    }
    else
    {
      /* Maximum length of data expected in the response */
      if(uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_BOOLEAN_FLAGS] &
         UIM_RFU_MASK_STATUS_MAX_LEN_RETURN_EXP)
      {
        uim_req_buf_static_ptr->apdu_hdr.p3 = 255;
      }
      else
      {
        uim_req_buf_static_ptr->apdu_hdr.p3 = 0;
      }
    }
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

     uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
     uim_req_buf_static_ptr->apdu_hdr.p2 = 0;
     uim_req_buf_static_ptr->apdu_hdr.p3 = 22;
     /* 22 bytes are mandatory and shall be returned by the R-UIM for
     the DF */
  }

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr , uim_ptr);
} /* uimgen_iso_handle_status_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_SEND_STATUS_ST
=============================================================================*/
void uimgen_iso_handle_recovery_send_status_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if(TRUE == uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].send_status)
  {
    uim_req_buf_static_ptr->apdu_hdr.instrn = STATUS;
    uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;

    uim_req_buf_static_ptr->apdu_hdr.uim_class =
    uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS2, uim_ptr->state.recovery_data.ch_to_be_recovered);

    uim_req_buf_static_ptr->apdu_hdr.p1 = UIM_STATUS_P1_CURR_APP_INIT;
    uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_STATUS_P2_DF_NAME_OF_CURR_APP;
    uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].aid.aid_length + UIM_AID_TLV_LEN_OFFSET;
    uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

    uim_send_command(uim_req_buf_static_ptr, uim_ptr);
  }
  else
  {
    /* Do not send status comamnd and skip to next state*/
    ++uim_ptr->command.generic_state_ptr;
    uim_generic_command (uim_ptr);
  }
} /* uimgen_iso_handle_recovery_send_status_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_BIN_MEM_ST
=============================================================================*/
void uimgen_iso_handle_read_bin_mem_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* The selected file is TRANSPARENT.  Use a binary read to read
     from the file. */
  uim_req_buf_static_ptr->apdu_hdr.instrn = READ_BINARY;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;

  /* Offset into the EF - high byte, low byte */
  uim_req_buf_static_ptr->apdu_hdr.p1 = WORD_HI(uim_ptr->command.rd_wr_info.offset);
  uim_req_buf_static_ptr->apdu_hdr.p2 = WORD_LO(uim_ptr->command.rd_wr_info.offset);

  if (uim_ptr->command.cmd_ptr->read_bin.sfi != UIM_INVALID_SFI &&
      uim_req_buf_static_ptr->apdu_hdr.p1 == 0x00)
  {
    /* Can there be a case where the offset is > 256 that would need P1? If yes then we cannot use SFI but use the regular method. */
    /* SFI encoded in P1. Bits B8,B7,B6 need to be 1,0,0 respectively for SFI operation and bits B1-B5 hold the SFI.
       Offset into the EF in P2. */
    uim_req_buf_static_ptr->apdu_hdr.p1 = ( UIM_SFI_USED_BITS_B6_TO_B8 |
                                            (uim_ptr->command.cmd_ptr->read_bin.sfi & UIM_SFI_BITS_B1_TO_B5_MASK) );
    uim_req_buf_static_ptr->apdu_hdr.p2 = WORD_LO(uim_ptr->command.rd_wr_info.offset);
    UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_91,uim_ptr->id, "UIM_READ_BIN_MEM_ST - SFI used - 0x%x",
                      uim_ptr->command.cmd_ptr->read_bin.sfi);

  }
  else
  {
     /* SFI referencing is not used and so clear the MSB of p1 */
     uim_req_buf_static_ptr->apdu_hdr.p1 = uim_req_buf_static_ptr->apdu_hdr.p1 ^
         (uim_req_buf_static_ptr->apdu_hdr.p1 & UIM_SFI_NOT_USED_BR_MASK);
  }

  if (UIM_MAX_TRANSACTION_SIZE <= uim_ptr->command.rd_wr_info.num_bytes)
  {
    /* This is to read file in chunks of 255bytes */
    uim_req_buf_static_ptr->apdu_hdr.p3 = 0xFF;
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.p3 = WORD_LO (uim_ptr->command.rd_wr_info.num_bytes);
  }

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_read_bin_mem_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UPDATE_BIN_MEM_ST
=============================================================================*/
void uimgen_iso_handle_update_bin_mem_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* The selected file is TRANSPARENT.  Use a binary update to write
     to the file. */
  uim_req_buf_static_ptr->apdu_hdr.instrn = UPDATE_BINARY;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  /* Offset into the EF -- high byte, low byte */
  uim_req_buf_static_ptr->apdu_hdr.p1 = WORD_HI(  uim_ptr->command.rd_wr_info.offset );
  uim_req_buf_static_ptr->apdu_hdr.p2 = WORD_LO(  uim_ptr->command.rd_wr_info.offset );

  if (uim_ptr->command.cmd_ptr->write_bin.sfi != UIM_INVALID_SFI &&
      uim_req_buf_static_ptr->apdu_hdr.p1 == 0x00)
  {
    /* Can there be a case where the offset is > 256 that would need P1?
       If yes then we cannot use SFI but use the regular method */
    /* SFI encoded in P1. Bits B8,B7,B6 need to be 1,0,0 respectively for SFI operation and bits B1-B5 hold the SFI.
       Offset into the EF in P2 */
    uim_req_buf_static_ptr->apdu_hdr.p1 = ( UIM_SFI_USED_BITS_B6_TO_B8 |
                                            (uim_ptr->command.cmd_ptr->write_bin.sfi & UIM_SFI_BITS_B1_TO_B5_MASK) );
    uim_req_buf_static_ptr->apdu_hdr.p2 = WORD_LO(uim_ptr->command.rd_wr_info.offset);
    UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_92,uim_ptr->id, "UIM_UPDATE_BIN_MEM_ST - SFI used - 0x%x",
                      uim_ptr->command.cmd_ptr->write_bin.sfi);

  }
  else
  {
     /* SFI referencing is not used and so clear the MSB of p1 */
     uim_req_buf_static_ptr->apdu_hdr.p1 &= ~UIM_SFI_NOT_USED_BR_MASK;
  }

  if (UIM_MAX_TRANSACTION_SIZE <= uim_ptr->command.rd_wr_info.num_bytes)
  {
    uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_TRANS_SIZE_OUT;
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.p3 =
      WORD_LO (uim_ptr->command.rd_wr_info.num_bytes);
  }

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  uim_memscpy(uim_req_buf_static_ptr->req.data,
         sizeof(uim_req_buf_static_ptr->req.data),
         uim_ptr->command.rd_wr_info.data_ptr,
         uim_req_buf_static_ptr->apdu_hdr.p3);

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_update_bin_mem_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_REC_MEM_ST
=============================================================================*/
void uimgen_iso_handle_read_rec_mem_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* The selected file is a record format.  Use a record read to read
     from the file. */
  uim_req_buf_static_ptr->apdu_hdr.instrn = READ_RECORD;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  if (uim_ptr->command.cmd_ptr->read_rec.sfi != UIM_INVALID_SFI)
  {
    UIMDRV_MSG_MED_2(uim_ptr->id, "UIM_READ_REC_MEM_ST - UIM_NEXT - SFI used - 0x%x for record mode 0x%x",
                     uim_ptr->command.cmd_ptr->read_rec.sfi,uim_ptr->command.cmd_ptr->read_rec.recmode );
  }

  /* Fill out APDU parameters based on the request mode */
  switch (uim_ptr->command.cmd_ptr->read_rec.recmode)
  {
    case UIM_CURRENT:
      {
        uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
        uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_ABS_RECORD_MODE;
        /* SFI referencing cannot be used if we are trying to read a record
           from the currently selected EF, so clear the MS 5 bits of p2 */
        uim_req_buf_static_ptr->apdu_hdr.p2 = uim_req_buf_static_ptr->apdu_hdr.p2 ^
        (uim_req_buf_static_ptr->apdu_hdr.p2 & UIM_SFI_NOT_USED_RR_MASK);

      } /* end case - UIM_CURRENT */
      break;

    case UIM_NEXT:
      {
        uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
        if (uim_ptr->command.cmd_ptr->read_rec.sfi != UIM_INVALID_SFI)
        {
          /* SFI encoded in P2. Bits B8,B7,B6,B5,B4 denote the SFI and bits B1-B3 hold the mode for READ_RECORD. */
          uim_req_buf_static_ptr->apdu_hdr.p2 = ( UIM_P2_NEXT_RECORD_MODE |
                                                  ((uim_ptr->command.cmd_ptr->read_rec.sfi << UIM_SHIFT_SFI_BITS_TO_B4_TO_B8)
                                                   & UIM_SFI_BITS_B4_TO_B8_MASK) );

        }
        else
        {
        uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_NEXT_RECORD_MODE;
        }
      } /* end case - UIM_NEXT */
      break;

    case UIM_PREVIOUS:
      {
        uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
        if (uim_ptr->command.cmd_ptr->read_rec.sfi != UIM_INVALID_SFI)
        {
          /* SFI encoded in P2. Bits B8,B7,B6,B5,B4 denote the SFI and bits B1-B3 hold the mode for READ_RECORD. */
          uim_req_buf_static_ptr->apdu_hdr.p2 = ( UIM_P2_PREV_RECORD_MODE |
                                                  ((uim_ptr->command.cmd_ptr->read_rec.sfi << UIM_SHIFT_SFI_BITS_TO_B4_TO_B8)
                                                    & UIM_SFI_BITS_B4_TO_B8_MASK) );
        }
        else
        {
        uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_PREV_RECORD_MODE;
      }
      }
      break;

    case UIM_ABSOLUTE:
      {
        uim_req_buf_static_ptr->apdu_hdr.p1 = uim_ptr->command.cmd_ptr->read_rec.rec_num;
        if (uim_ptr->command.cmd_ptr->read_rec.sfi != UIM_INVALID_SFI)
        {
          /* SFI encoded in P2. Bits B8,B7,B6,B5,B4 denote the SFI and bits B1-B3 hold the mode for READ_RECORD. */
          uim_req_buf_static_ptr->apdu_hdr.p2 = ( UIM_P2_ABS_RECORD_MODE |
                                                  ((uim_ptr->command.cmd_ptr->read_rec.sfi << UIM_SHIFT_SFI_BITS_TO_B4_TO_B8)
                                                   & UIM_SFI_BITS_B4_TO_B8_MASK) );
        }
        else
        {
        uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_ABS_RECORD_MODE;
      }
      }
      break;

    default:
      break;

  } /* end switch - uim_ptr->command.cmd_ptr->read_rec.rec_mode */

  /* Number of bytes to be read */
  uim_req_buf_static_ptr->apdu_hdr.p3 = uint16touint8(uim_ptr->command.rd_wr_info.num_bytes);

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_read_rec_mem_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UPDATE_REC_MEM_ST
=============================================================================*/
void uimgen_iso_handle_update_rec_mem_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* The last selected file is a record structure.  Use update record
     to write to this file */
  uim_req_buf_static_ptr->apdu_hdr.instrn = UPDATE_RECORD;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  if (uim_ptr->command.cmd_ptr->write_rec.sfi != UIM_INVALID_SFI)
  {
     UIMDRV_MSG_HIGH_2(uim_ptr->id, "UIM_UPDATE_REC_MEM_ST - UIM_NEXT - SFI used - 0x%x  for rec mode 0x%x",
                            uim_ptr->command.cmd_ptr->write_rec.sfi , uim_ptr->command.cmd_ptr->write_rec.recmode);
  }
  /* Fill out APDU parameters based on the request mode */
  switch (uim_ptr->command.cmd_ptr->write_rec.recmode)
  {
    case UIM_CURRENT:
      {
        uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
        uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_ABS_RECORD_MODE;
        /* SFI referencing cannot be used if we are trying to update a record
           from the currently selected EF, so clear the MS 5 bits of p2 */
        uim_req_buf_static_ptr->apdu_hdr.p2 = uim_req_buf_static_ptr->apdu_hdr.p2 ^
        (uim_req_buf_static_ptr->apdu_hdr.p2 & UIM_SFI_NOT_USED_RR_MASK);

      } /* end case - UIM_CURRENT */
      break;

    case UIM_NEXT:
      {
        uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
        if (uim_ptr->command.cmd_ptr->write_rec.sfi != UIM_INVALID_SFI)
        {
          /* SFI encoded in P2. Bits B8,B7,B6,B5,B4 denote the SFI and bits B1-B3 hold the mode for UPDATE_RECORD. */
          uim_req_buf_static_ptr->apdu_hdr.p2 = ( UIM_P2_NEXT_RECORD_MODE |
                                                  ((uim_ptr->command.cmd_ptr->write_rec.sfi << UIM_SHIFT_SFI_BITS_TO_B4_TO_B8)
                                                    & UIM_SFI_BITS_B4_TO_B8_MASK) );
        }
        else
        {
        uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_NEXT_RECORD_MODE;
        }
      } /* end case - UIM_NEXT */
      break;

    case UIM_PREVIOUS:
      {
        uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
                      if (uim_ptr->command.cmd_ptr->write_rec.sfi != UIM_INVALID_SFI)
        {
          /* SFI encoded in P2. Bits B8,B7,B6,B5,B4 denote the SFI and bits B1-B3 hold the mode for UPDATE_RECORD. */
          uim_req_buf_static_ptr->apdu_hdr.p2 = ( UIM_P2_PREV_RECORD_MODE |
                                                  ((uim_ptr->command.cmd_ptr->write_rec.sfi << UIM_SHIFT_SFI_BITS_TO_B4_TO_B8)
                                                    & UIM_SFI_BITS_B4_TO_B8_MASK) );
        }
        else
        {
        uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_PREV_RECORD_MODE;
        }
      } /* end case - UIM_PREVIOUS */
      break;

    case UIM_ABSOLUTE:
      {
        uim_req_buf_static_ptr->apdu_hdr.p1 = uim_ptr->command.cmd_ptr->write_rec.rec_num;
                      if (uim_ptr->command.cmd_ptr->write_rec.sfi != UIM_INVALID_SFI)
        {
          /* SFI encoded in P2. Bits B8,B7,B6,B5,B4 denote the SFI and bits B1-B3 hold the mode for UPDATE_RECORD. */
          uim_req_buf_static_ptr->apdu_hdr.p2 = ( UIM_P2_ABS_RECORD_MODE |
                                                  ((uim_ptr->command.cmd_ptr->write_rec.sfi << UIM_SHIFT_SFI_BITS_TO_B4_TO_B8)
                                                   & UIM_SFI_BITS_B4_TO_B8_MASK) );
        }
        else
        {
        uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_ABS_RECORD_MODE;
        }
      } /* end case - UIM_ABSOLUTE */
      break;

    default:
      break;

  } /* end switch - uim_ptr->command.cmd_ptr->write_rec.rec_mode */

  /* Number of bytes to write */
  uim_req_buf_static_ptr->apdu_hdr.p3 = uint16touint8(uim_ptr->command.rd_wr_info.num_bytes);

  uim_memscpy(uim_req_buf_static_ptr->req.data,
         sizeof(uim_req_buf_static_ptr->req.data),
         uim_ptr->command.rd_wr_info.data_ptr,
         uim_req_buf_static_ptr->apdu_hdr.p3);

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_update_rec_mem_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_INC_MEM_ST
=============================================================================*/
void uimgen_iso_handle_inc_mem_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build an increase APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = INCREASE;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_4;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
      uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS2, uim_ptr->command.cmd_ptr->hdr.channel);
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  uim_req_buf_static_ptr->apdu_hdr.p3 = uint16touint8(uim_ptr->command.rd_wr_info.num_bytes);

  uim_memscpy( uim_req_buf_static_ptr->req.data,
           sizeof(uim_req_buf_static_ptr->req.data),
           uim_ptr->command.rd_wr_info.data_ptr,
           uim_ptr->command.rd_wr_info.num_bytes);

  if (uim_ptr->command.cmd_ptr->inc.sfi != UIM_INVALID_SFI)
  {
    /* SFI encoded in P1. Bits B8,B7,B6 need to be 1,0,0
       respectively for SFI operation and bits B1-B5 hold the SFI. */
    uim_req_buf_static_ptr->apdu_hdr.p1 = ( UIM_SFI_USED_BITS_B6_TO_B8 |
                                            (uim_ptr->command.cmd_ptr->inc.sfi &
                                             UIM_SFI_BITS_B1_TO_B5_MASK) );
    UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_93,uim_ptr->id, "UIM_INC_MEM_ST - SFI used - 0x%x",
                      uim_ptr->command.cmd_ptr->inc.sfi);
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  }

  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_inc_mem_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_VERIFY_CHV1_ST
=============================================================================*/
void uimgen_iso_handle_verify_chv1_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Check if CHV1 has been cache'd or this is a command to verify CHV */
  if (uim_ptr->cache.chv1.use_chv)
  {
    /* Build a verify CHV APDU */
    uim_req_buf_static_ptr->apdu_hdr.instrn = VERIFY_CHV;
    uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

    uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

    uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV1;

    /* bytes of data sent in the command */
    uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS;

    uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

    /* The CHV digits are copied from the cache'd buffer so that CHV1
       can be verified on wake up of the UIM (no explicit command). */
    uim_memscpy( (byte*) uim_req_buf_static_ptr->req.verify_chv.chv,
            sizeof(uim_req_buf_static_ptr->req.verify_chv.chv),
            uim_ptr->cache.chv1.chv, UIM_MAX_CHV_DIGITS);

    uim_send_command(uim_req_buf_static_ptr, uim_ptr);
  }
  else /* CHV 1 value is not cache'd */
  {
    /* Skip this state */
    ++uim_ptr->command.generic_state_ptr;
    /* Call this function to process the next state. */
    uim_generic_command (uim_ptr);
  } /* end if - the CHV 1 value is cache'd */
} /* uimgen_iso_handle_verify_chv1_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_VERIFY_CHV2_ST
=============================================================================*/
void uimgen_iso_handle_verify_chv2_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build a verify CHV or a VERIFY PIN APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = VERIFY_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

  if (uim_ptr->command.cmd_ptr->hdr.protocol != UIM_UICC)
  {
    if (!uim_ptr->cache.chv2.use_chv)
    {
      /* Do not verify CHV2 and skip this state */
      ++uim_ptr->command.generic_state_ptr;
      /* Call this function to process the next state. */
      uim_generic_command (uim_ptr);
      return; /* exit immediately */
    }

    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
    uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV2;  /* CHV2 */
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
      uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);

    uim_set_uicc_pin_param_p2( uim_ptr->command.cmd_ptr->verify_chv.chv,
                              (byte*) &(uim_req_buf_static_ptr->apdu_hdr.p2),
                               uim_ptr);
  }

  /* If the command is being sent just to get the number of re-tries
     remaining, do not include the data */
  if ((uim_ptr->command.cmd_ptr->verify_chv.chv_digits[0] == '\0') &&
    (uim_ptr->command.cmd_ptr->hdr.command == UIM_VERIFY_CHV_F))
  {
    uim_req_buf_static_ptr->apdu_hdr.p3 = 0;
    uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_1;
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS;

    uim_memscpy( (byte*) uim_req_buf_static_ptr->req.verify_chv.chv,
                    sizeof(uim_req_buf_static_ptr->req.verify_chv.chv),
                    uim_ptr->cache.chv2.chv, UIM_MAX_CHV_DIGITS);
  }

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_verify_chv2_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UNBLOCK_CHV1_ST
=============================================================================*/
void uimgen_iso_handle_unblock_chv1_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build an unblock CHV APDU */
  uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  uim_req_buf_static_ptr->apdu_hdr.instrn = UNBLOCK_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;     /* weird but TRUE,!= UIM_CHV1*/
  /* bytes of data sent in cmd */
  uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS * 2;

  /* The unblock digits are provided in the command buffer by the
     client task. */
  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.unblk_chv.chv,
          sizeof(uim_req_buf_static_ptr->req.unblk_chv.chv),
          uim_ptr->command.cmd_ptr->unblk_chv.chv_digits, UIM_MAX_CHV_DIGITS);

  /* The new CHV1 digits are provided in the command buffer by the
     client task. These digits are sent in the APDU.  The digits
     are copied to the cache buffer if the command completes
     successfully. */
  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.unblk_chv.new_chv,
          sizeof(uim_req_buf_static_ptr->req.unblk_chv.new_chv),
          uim_ptr->command.cmd_ptr->unblk_chv.new_chv_digits, UIM_MAX_CHV_DIGITS);

  /* Format the CHV digits in the request buffer */
  uim_format_chv( (byte*) uim_req_buf_static_ptr->req.unblk_chv.new_chv,
                  uim_ptr);

  /* Format the CHV digits in the request buffer */
  uim_format_chv( (byte*) uim_req_buf_static_ptr->req.unblk_chv.chv,
                  uim_ptr);

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_unblock_chv1_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UNBLOCK_CHV2_ST
=============================================================================*/
void uimgen_iso_handle_unblock_chv2_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build an unblock CHV or Unblock PIN APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = UNBLOCK_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

  if (uim_ptr->command.cmd_ptr->unblk_chv.chv == UIM_CHV2)
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

     uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV2;  /* CHV2 */
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);

     uim_set_uicc_pin_param_p2( uim_ptr->command.cmd_ptr->unblk_chv.chv,
                                (byte*) &(uim_req_buf_static_ptr->apdu_hdr.p2),
                                uim_ptr);
  }

  /* If the command is being sent just to get the number of re-tries
     remaining, do not include the data */
  if (uim_ptr->command.cmd_ptr->unblk_chv.chv_digits[0] == '\0')
  {
      uim_req_buf_static_ptr->apdu_hdr.p3 = 0;
      uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_1;
  }
  else
  {
     /* bytes of data sent in cmd */
     uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS * 2;

     /* The unblock digits are supplied in the command buffer by the
        client task. */
     uim_memscpy( (byte*) uim_req_buf_static_ptr->req.unblk_chv.chv,
            sizeof(uim_req_buf_static_ptr->req.unblk_chv.chv),
            uim_ptr->command.cmd_ptr->unblk_chv.chv_digits, UIM_MAX_CHV_DIGITS);

     /* Get the CHV digits from the command buffer.  The digits are not
        saved in a cache buffer for CHV2 since CHV2 is only verified
        by an explicit call from a client (not on wakeup of the UIM). */
     uim_memscpy( (byte*) uim_req_buf_static_ptr->req.unblk_chv.new_chv,
             sizeof(uim_req_buf_static_ptr->req.unblk_chv.new_chv),
             uim_ptr->command.cmd_ptr->unblk_chv.new_chv_digits, UIM_MAX_CHV_DIGITS);

     /* Format the CHV digits for the new CHV */
     uim_format_chv((byte*) uim_req_buf_static_ptr->req.unblk_chv.new_chv,
                    uim_ptr);

     /* Format the CHV digits for Unblock CHV */
     uim_format_chv((byte*) uim_req_buf_static_ptr->req.unblk_chv.chv,
                    uim_ptr);
  }

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_unblock_chv2_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_CHANGE_CHV1_ST
=============================================================================*/
void uimgen_iso_handle_change_chv1_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build an change CHV APDU */
  uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  uim_req_buf_static_ptr->apdu_hdr.instrn = CHANGE_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = (int)UIM_CHV1; /* CHV1 */
  /* bytes of data sent in cmd */
  uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS * 2;

  /* The old CHV1 digits are supplied in the command buffer by the
     client task.  In this case, we do not use the cache buffer as
     the cache buffer may not contain the proper digits. */
  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.change_chv.chv,
          sizeof(uim_req_buf_static_ptr->req.change_chv.chv),
          uim_ptr->command.cmd_ptr->change_chv.chv_digits, UIM_MAX_CHV_DIGITS);

  /* The client task supplies the new CHV digits in the command buffer.
     The digits will be copied to the CHV1 cache buffer if the command
     completes successfully. */
  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.change_chv.new_chv,
          sizeof(uim_req_buf_static_ptr->req.change_chv.new_chv),
          uim_ptr->command.cmd_ptr->change_chv.new_chv_digits, UIM_MAX_CHV_DIGITS);

  /* Format the CHV digits */
  uim_format_chv((byte*) uim_req_buf_static_ptr->req.change_chv.chv,
                    uim_ptr);
  uim_format_chv((byte*) uim_req_buf_static_ptr->req.change_chv.new_chv,
                    uim_ptr);

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_change_chv1_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_CHANGE_CHV2_ST
=============================================================================*/
void uimgen_iso_handle_change_chv2_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build an change CHV APDU or CHANGE PIN APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = CHANGE_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

  if (uim_ptr->command.cmd_ptr->change_chv.chv == UIM_CHV2)
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

     uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV2;  /* CHV2 */
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);

     uim_set_uicc_pin_param_p2( uim_ptr->command.cmd_ptr->change_chv.chv,
                                (byte*) &(uim_req_buf_static_ptr->apdu_hdr.p2),
                                uim_ptr);
  }

  /* bytes of data sent in cmd */
  uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS * 2;

  /* The old CHV2 digits are supplied in the command buffer by the
     client task.  The CHV2 digits are not kept in a cache buffer. */
  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.change_chv.chv,
          sizeof(uim_req_buf_static_ptr->req.change_chv.chv),
          uim_ptr->command.cmd_ptr->change_chv.chv_digits, UIM_MAX_CHV_DIGITS);

  /* The client task supplies the new CHV digits in the command buffer.
     The digits are not copied to a cache buffer as there is no need
     for a cache buffer for CHV2. */
  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.change_chv.new_chv,
          sizeof(uim_req_buf_static_ptr->req.change_chv.new_chv),
          uim_ptr->command.cmd_ptr->change_chv.new_chv_digits, UIM_MAX_CHV_DIGITS);

  /* Format the CHV digits */
  uim_format_chv((byte*) uim_req_buf_static_ptr->req.change_chv.chv,
                    uim_ptr);
  uim_format_chv((byte*) uim_req_buf_static_ptr->req.change_chv.new_chv,
                    uim_ptr);

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_change_chv2_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_ENABLE_CHV1_ST
=============================================================================*/
void uimgen_iso_handle_enable_chv1_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build an enable CHV APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = ENABLE_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

  if (uim_ptr->command.cmd_ptr->enable_chv.chv == UIM_CHV1)
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

     uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV1;  /* CHV2 */
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);

     uim_set_uicc_pin_param_p2( uim_ptr->command.cmd_ptr->enable_chv.chv,
                                (byte*) &(uim_req_buf_static_ptr->apdu_hdr.p2),
                                uim_ptr);
  }

  uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS;
                /* bytes of data sent in the command */

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  /* The CHV1 digits are provided in the command buffer by the
     client task. These digits are sent in the APDU.  The digits
     are copied to the cache buffer if the command completes
     successfully. */
  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.enable_chv.chv,
          sizeof(uim_req_buf_static_ptr->req.enable_chv.chv),
          uim_ptr->command.cmd_ptr->enable_chv.chv_digits, UIM_MAX_CHV_DIGITS);

  uim_format_chv((byte*) uim_req_buf_static_ptr->req.enable_chv.chv,
                    uim_ptr);

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_enable_chv1_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_ENABLE_CHV2_ST
=============================================================================*/
void uimgen_iso_handle_enable_chv2_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  uim_req_buf_static_ptr->apdu_hdr.instrn = ENABLE_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV2;
  uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS;
                /* bytes of data sent in the command */

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.enable_chv.chv,
          sizeof(uim_req_buf_static_ptr->req.enable_chv.chv),
          uim_ptr->command.cmd_ptr->enable_chv.chv_digits, UIM_MAX_CHV_DIGITS);

  uim_format_chv((byte*) uim_req_buf_static_ptr->req.enable_chv.chv,
                    uim_ptr);

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_enable_chv2_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_DISABLE_CHV1_ST
=============================================================================*/
void uimgen_iso_handle_disable_chv1_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build a disable CHV APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = DISABLE_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

  if (uim_ptr->command.cmd_ptr->disable_chv.chv == UIM_CHV1)
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

     uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV1;  /* CHV2 */
  }
  else
  {
     uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);

     uim_set_uicc_pin_param_p2( uim_ptr->command.cmd_ptr->disable_chv.chv,
                                (byte*) &(uim_req_buf_static_ptr->apdu_hdr.p2),
                                uim_ptr);

     if(uim_ptr->command.cmd_ptr->disable_chv.param_p2 != 0)
     {
        uim_req_buf_static_ptr->apdu_hdr.p1 = 0x80 | UIM_UNIVERSAL_PIN_VALUE;
     }
  }

  uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS;
                /* bytes of data sent in the command */

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  /* The CHV1 digits are provided in the command buffer by the
     client task. These digits are sent in the APDU.  The digits
     are copied to the cache buffer if the command completes
     successfully. */
  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.disable_chv.chv,
          sizeof(uim_req_buf_static_ptr->req.disable_chv.chv),
          uim_ptr->command.cmd_ptr->disable_chv.chv_digits, UIM_MAX_CHV_DIGITS);

  uim_format_chv((byte*) uim_req_buf_static_ptr->req.disable_chv.chv,
                    uim_ptr);

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_disable_chv1_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_DISABLE_CHV2_ST
=============================================================================*/
void uimgen_iso_handle_disable_chv2_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build a disable CHV APDU */
  uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  uim_req_buf_static_ptr->apdu_hdr.instrn = DISABLE_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV2;
  uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS;
                /* bytes of data sent in the command */

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.disable_chv.chv,
          sizeof(uim_req_buf_static_ptr->req.disable_chv.chv),
          uim_ptr->command.cmd_ptr->disable_chv.chv_digits, UIM_MAX_CHV_DIGITS);

  uim_format_chv((byte*) uim_req_buf_static_ptr->req.disable_chv.chv,
                    uim_ptr);

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_disable_chv2_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_INVALIDATE_ST
=============================================================================*/
void uimgen_iso_handle_invalidate_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build an invalidate APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = INVALIDATE;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_1;

  /* P1 = P2 = 0 and data field is empty in UICC implies that the
     command applies on the current EF */
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p3 = 0;
              /* bytes of data sent in the command */

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
      uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_invalidate_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_REHABILITATE_ST
=============================================================================*/
void uimgen_iso_handle_rehabilitate_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build a rehabilitate APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = REHABILITATE;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_1;

  /* P1 = P2 = 0 and data field is empty in UICC implies that the
     command applies on the current EF */
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p3 = 0;
              /* bytes of data sent in the command */

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
      uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_rehabilitate_st */

/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SEEK_ST
=============================================================================*/
void uimgen_iso_handle_seek_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build a seek APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = SEEK;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_4;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
      uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);
    if (uim_ptr->command.cmd_ptr->seek.sfi != UIM_INVALID_SFI)
    {
      UIMDRV_MSG_HIGH_2(uim_ptr->id, "UIM_SEEK_ST - UIM_SIMPLE_SRCH - SFI used - 0x%x for Search mode 0x%x",
                        uim_ptr->command.cmd_ptr->seek.sfi, uim_ptr->command.cmd_ptr->seek.srch_mode);
    }
    /* Simple search or enhanced search */
    switch (uim_ptr->command.cmd_ptr->seek.srch_mode)
    {
      case  UIM_SIMPLE_SRCH:

        uim_req_buf_static_ptr->apdu_hdr.p1 = uim_ptr->command.cmd_ptr->seek.rec_number;
        if (uim_ptr->command.cmd_ptr->seek.sfi != UIM_INVALID_SFI)
        {
          /* SFI encoded in P2. Bits B8,B7,B6,B5,B4 denote the SFI and bits B1-B3 hold the mode for SEARCH_RECORD. */
          uim_req_buf_static_ptr->apdu_hdr.p2 = ( uim_ptr->command.cmd_ptr->seek.srch_dir |
                                                  ((uim_ptr->command.cmd_ptr->seek.sfi << UIM_SHIFT_SFI_BITS_TO_B4_TO_B8)
                                                   & UIM_SFI_BITS_B4_TO_B8_MASK) );
        }
        else
        {
        uim_req_buf_static_ptr->apdu_hdr.p2 = (int)uim_ptr->command.cmd_ptr->seek.srch_dir;
        }

        /* Number of bytes of data sent in the command */
        uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->seek.num_bytes;

        /* Copy the seek patern from the command */
        uim_memscpy( uim_req_buf_static_ptr->req.data,
                sizeof(uim_req_buf_static_ptr->req.data),
                uim_ptr->command.cmd_ptr->seek.srch_pattern,
                uim_ptr->command.cmd_ptr->seek.num_bytes );

      break; /* UIM_SIMPLE_SRCH  */

      case UIM_ENHANCED_SRCH:

        /* See if search is based on  rec # in P1 or current rec */
        if ((int)uim_ptr->command.cmd_ptr->seek.srch_rec_mode_type & UIM_SRCH_MODE_MASK )
          uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
        else
          uim_req_buf_static_ptr->apdu_hdr.p1 = uim_ptr->command.cmd_ptr->seek.rec_number;

                      if (uim_ptr->command.cmd_ptr->seek.sfi != UIM_INVALID_SFI)
        {
          /* SFI encoded in P2. Bits B8,B7,B6,B5,B4 denote the SFI and bits B1-B3 hold the mode for SEARCH_RECORD. */
          uim_req_buf_static_ptr->apdu_hdr.p2 = ( UIM_P2_SRCH_ENHANCED |
                                                  ((uim_ptr->command.cmd_ptr->seek.sfi << UIM_SHIFT_SFI_BITS_TO_B4_TO_B8)
                                                   & UIM_SFI_BITS_B4_TO_B8_MASK) );
        }
        else
        {
        uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_P2_SRCH_ENHANCED;
        }

        /* Number of bytes of data sent in the command */
        uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->seek.num_bytes +
                                        UIM_SRCH_IND_BYTES;

        /* Search indicator bytes */
        uim_req_buf_static_ptr->req.data[0] = (int)uim_ptr->command.cmd_ptr->seek.offset_type
                                     | (int)uim_ptr->command.cmd_ptr->seek.srch_rec_mode_type;

        uim_req_buf_static_ptr->req.data[1] = uim_ptr->command.cmd_ptr->seek.offset;

        /* Copy the seek patern from the command */
        uim_memscpy( &uim_req_buf_static_ptr->req.data[UIM_SRCH_IND_BYTES],
                (sizeof(uim_req_buf_static_ptr->req.data) - UIM_SRCH_IND_BYTES),
                uim_ptr->command.cmd_ptr->seek.srch_pattern, uim_ptr->command.cmd_ptr->seek.num_bytes );

      break; /* UIM_ENHANCED_SRCH */

      default:
      break;
    } /* switch (uim_ptr->command.cmd_ptr->seek.srch_mode) */
  } /* if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC) */

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

    uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

    uim_req_buf_static_ptr->apdu_hdr.p2 = (int)uim_ptr->command.cmd_ptr->seek.seek_type
                                 | (int)uim_ptr->command.cmd_ptr->seek.mode;

    /* Number of bytes of data sent in the command */
    uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->seek.num_bytes;

    /* Copy the seek patern from the command */
    uim_memscpy( uim_req_buf_static_ptr->req.data,
            sizeof(uim_req_buf_static_ptr->req.data),
            uim_ptr->command.cmd_ptr->seek.srch_pattern,
            uim_ptr->command.cmd_ptr->seek.num_bytes );
  }

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_seek */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_TERMINAL_PROFILE_ST
=============================================================================*/
void uimgen_iso_handle_terminal_profile_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if ( ((uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_WAKE_UP_F) &&
        (uim_ptr->cache.int_term_profile_data.num_bytes != 0)) ||
       (uim_ptr->command.cmd_ptr->hdr.command == UIM_TERMINAL_PROFILE_F)
     )
  {

    /* Build a Terminal Profile APDU */
    uim_req_buf_static_ptr->apdu_hdr.instrn = TERMINAL_PROFILE;
    uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

    /* P1 = P2 = 0 and data field is empty in UICC implies that the
    command applies on the current EF */
    uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
    uim_req_buf_static_ptr->apdu_hdr.p2 = 0;

    if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
    {
      uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UICC_INSTRUCTION_CLASS2;
    }
    else
    {
      uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
    }

    if (uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_WAKE_UP_F)
    {
      /* Number of bytes of Terminal Profile */
      uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->cache.int_term_profile_data.num_bytes;

      /* Copy the data from the internal buffer */
      uim_memscpy (uim_req_buf_static_ptr->req.data,
              sizeof(uim_req_buf_static_ptr->req.data),
              uim_ptr->cache.int_term_profile_data.data,
              uim_ptr->cache.int_term_profile_data.num_bytes );
    }
    else
    {
      /* Number of bytes of Terminal Profile */
      uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->terminal_profile.num_bytes;

      /* Copy the data from the command */
      uim_memscpy(uim_req_buf_static_ptr->req.data,
             sizeof(uim_req_buf_static_ptr->req.data),
             uim_ptr->command.cmd_ptr->terminal_profile.data,
             uim_ptr->command.cmd_ptr->terminal_profile.num_bytes );
      /* Clear TP data cached by UIM */
      uim_ptr->cache.int_term_profile_data.num_bytes = 0;
    }

    uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

    if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_MIN_TPL_ICCID_SUPPORT,
                                 uim_ptr) == TRUE)
    {
      /* Get the ICCID value and TPL from NV  */
      if (NULL == uim_ptr->state.iccid_tpl_ptr)
      {
        /* Allocate memory */
        uim_ptr->state.iccid_tpl_ptr = uim_malloc(sizeof(nv_uim_iccid_tpl_type));
        if(NULL != uim_ptr->state.iccid_tpl_ptr)
        {
          /* Read NV Item */
          if (FALSE == uim_get_nv(
                        NV_UIM_ICCID_TPL_I,
                        (nv_item_type *)uim_ptr->state.iccid_tpl_ptr,
                        uim_ptr)
              )
          {
            /* NV item is not supported, freeing memory */
            UIM_FREE(uim_ptr->state.iccid_tpl_ptr);
          }
        }
      }
      /* Compare the uim iccid and nv iccid value, if both are same then
         update the uim terminal profile length to nv terminal profile
         length if it is minimum, that will lead the uim to send the
         terminal profile length with minmum value. */
      if ((uim_ptr->command.cmd_ptr->hdr.command == UIM_TERMINAL_PROFILE_F) && (NULL != uim_ptr->state.iccid_tpl_ptr))
      {
        if ((0 == memcmp(uim_ptr->cache.iccid_buf.data ,uim_ptr->state.iccid_tpl_ptr->iccid_data,
                         uim_ptr->cache.iccid_buf.num_bytes)) &&
            (uim_ptr->command.req_buf.apdu_hdr.p3 > uim_ptr->state.iccid_tpl_ptr->terminal_profile_len))
        {
          uim_ptr->command.req_buf.apdu_hdr.p3 = (byte)uim_ptr->state.iccid_tpl_ptr->terminal_profile_len;
        }
        else
        {
          /* If uim iccid and nv iccid is not same then take the uim iccid
             and uim terminal profile length that will be write
             back to NV if we get a error on terminal profile response */
          uim_memscpy(uim_ptr->state.iccid_tpl_ptr->iccid_data,
                 sizeof(uim_ptr->state.iccid_tpl_ptr->iccid_data),
                 uim_ptr->cache.iccid_buf.data ,
                 uim_ptr->cache.iccid_buf.num_bytes);
          uim_ptr->state.iccid_tpl_ptr->terminal_profile_len =
                                       uim_ptr->command.cmd_ptr->terminal_profile.num_bytes;
        }
      }
    } /* endif uim_nv_is_feature_enabled */

    uim_send_command(uim_req_buf_static_ptr, uim_ptr);
  } /* end if command needs to be sent */
  else
  {
    /* Signal is set internally for proper operation of state machine */
    uim_timed_sleep(UIM_CARD_DELAY_TIMEOUT, uim_ptr, UIM_ALL_ZERO_SIG);
    (void)rex_set_sigs(uim_ptr->tcb_ptr,UIM_CMD_RSP_SIG);
  }
} /* uimgen_iso_handle_terminal_profile_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_ENVELOPE_ST
=============================================================================*/
void uimgen_iso_handle_envelope_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build an Envelope APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = ENVELOPE;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_4;

  /* P1 = P2 = 0 and data field is empty in UICC implies that the
     command applies on the current EF */
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UICC_INSTRUCTION_CLASS2;
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  /* Number of bytes of Envelope command */
  uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->envelope.num_bytes;

  uim_memscpy(uim_req_buf_static_ptr->req.data,
         sizeof(uim_req_buf_static_ptr->req.data),
         uim_ptr->command.cmd_ptr->envelope.data + uim_ptr->command.cmd_ptr->envelope.offset,
         uim_ptr->command.cmd_ptr->envelope.num_bytes );

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_envelope_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_FETCH_ST
=============================================================================*/
void uimgen_iso_handle_fetch_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build a Fetch APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = FETCH;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;

  /* P1 = P2 = 0 and data field is empty in UICC implies that the
     command applies on the current EF */
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UICC_INSTRUCTION_CLASS2;
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  /* Number of bytes of Fetch command */
  uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->fetch.command_size;

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_fetch_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_TERMINAL_RESPONSE_ST
=============================================================================*/
void uimgen_iso_handle_terminal_response_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build a Terminal Response APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = TERMINAL_RESPONSE;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  /* P1 = P2 = 0 and data field is empty in UICC implies that the
     command applies on the current EF */
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UICC_INSTRUCTION_CLASS2;
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  /* Number of bytes of Terminal Response */
  uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->terminal_response.num_bytes;

  uim_memscpy(uim_req_buf_static_ptr->req.data,
         sizeof(uim_req_buf_static_ptr->req.data),
         uim_ptr->command.cmd_ptr->terminal_response.data,
         uim_ptr->command.cmd_ptr->terminal_response.num_bytes );

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_terminal_response_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_POLLING_INTERVAL_ST
=============================================================================*/
void uimgen_iso_handle_polling_interval_st
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr == NULL)
  {
    return;
  }

  switch (uim_ptr->command.cmd_ptr->polling.command_type)
  {
    case POLLING_INTERVAL:
      {
        uim_ptr->poll.is_proactive_polling_enabled = TRUE;
        uim_ptr->setting.proactive_time_delay =
          uim_ptr->command.cmd_ptr->polling.polling_interval_in_ms;

        if (uim_ptr->setting.proactive_time_delay
            < UIM_MIN_POLL_DURATION)
        {
          uim_ptr->setting.proactive_time_delay =
            UIM_MIN_POLL_DURATION;
        }

        /* Subtract UIM_POLL_OFFSET from the time_delay that is used by
           the server */
        uim_ptr->setting.proactive_time_delay -= UIM_POLL_OFFSET;
      }
      break;

    case POLLING_OFF:
      {
        uim_ptr->poll.is_proactive_polling_enabled = FALSE;
        /* Set the proactive polling duration to the maximum allowed */
        uim_ptr->setting.proactive_time_delay
          = (dword)UIM_MAX_POLL_DURATION;
        if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_UIM_POLLING_ONLY_AT_POLLING_TIMER_EXPIRY,
                                       uim_ptr) == TRUE)
        {
          /* Set the poll timer to the default polling time */
          uim_set_poll_timer( UIM_POLL_TIME, uim_ptr->setting.wakeup_poll_offset, uim_ptr);
        }
        uim_reset_poll_timer_val(uim_ptr);
      }
      break;

    default:
      break;
  } /* switch */

  /* Call the UIM power control function to use the new Proactive
     polling value.  Do not affect the power on voting. */
  uim_ptr->command.rpt_buf.rpt.polling_interval.actual_uim_polling_interval =
    uim_power_control_ds(uim_ptr->logical_id, UIM_NONE, TRUE, UIM_CALL_TYPE_NOT_APPLICABLE );

  /* Add the UIM_POLL_OFFSET back so that the negotiated time does not
     include the poll_offset as poll_offset is for internal purposes
     only */
  if (uim_ptr->command.cmd_ptr->polling.command_type != POLLING_OFF)
  {
    uim_ptr->command.rpt_buf.rpt.polling_interval.actual_uim_polling_interval +=
                                                       UIM_POLL_OFFSET;
  }
  /* Signal is set internally for proper operation of state machine */
  uim_timed_sleep(UIM_CARD_DELAY_TIMEOUT, uim_ptr, UIM_ALL_ZERO_SIG);
  (void)rex_set_sigs(uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG);
} /* uimgen_iso_handle_polling_interval_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_STREAM_APDU_ST
=============================================================================*/
void uimgen_iso_handle_stream_apdu_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  uim_stream_apdu_state(uim_ptr, uim_req_buf_static_ptr);
} /* uimgen_iso_handle_stream_apdu_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_COMPLETE_ST
=============================================================================*/
void uimgen_iso_handle_recovery_complete_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Switch the mode back to the saved previous mode */
  uim_ptr->command.mode = uim_ptr->state.save_mode;

  /* set the response as done */
  uim_ptr->command.rsp_buf.cmd_status = UIM_DONE;

  /* Nothing is to be done set the response signal */
  (void)rex_set_sigs(uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG);
} /* uimgen_iso_handle_recovery_complete_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_DONE_ST
=============================================================================*/
void uimgen_iso_handle_done_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Process the response of the command. */
  if(UIM_CARD_SAM  == uim_ptr->card_mode)
  {
    uim_sam_process_response(&uim_ptr->command.rsp_buf, uim_ptr);
  }
  else
  {
    uim_process_generic_response(&uim_ptr->command.rsp_buf, uim_ptr);
  }
} /* uimgen_iso_handle_done_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_FLUSH_MANAGE_CHANNEL_ST
=============================================================================*/
void uimgen_iso_handle_recovery_flush_manage_channel_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Check the command queue for any Manage Channel command
     Flush the command if found. Proceed to next state pointer
  */
  q_linear_delete(&uim_ptr->task.cmd_q,
                 uim_compare_manage_ch_cmd,
                 uim_ptr,
                 uim_remove_manage_ch_cmd);
  UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_100,uim_ptr->id,"Flushing manage channel command");
  ++uim_ptr->command.generic_state_ptr;
  uim_generic_command(uim_ptr);
} /* uimgen_iso_handle_recovery_flush_manage_channel_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_VERIFY_CHV1_ST
=============================================================================*/
void uimgen_iso_handle_recovery_verify_chv1_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if(0 == uim_ptr->state.recovery_data.num_channels)
  {
    /* GSDI has already cleared its cache */
    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_101,uim_ptr->id,"No information in GSDI cache, move to Done state");
    /* Finish recovery, move to done state */
    uim_ptr->mode.generic_states[0] = UIM_DONE_ST;
    uim_ptr->command.generic_state_ptr = uim_ptr->mode.generic_states;

    /* Call this function to process the next state. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }
  /* Determine which CHV is to be verified. */
  if ((uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin1_data.chv == UIM_CHV1) &&
           (uim_ptr->state.recovery_data.protocol == UIM_ICC))
  {
    UIMDRV_MSG_MED_0(uim_ptr->id,"Verifying PIN1 on ICC");
    /* Format the CHV digits in the cache buffer. */
    uim_format_chv(uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin1_data.chv_digits,
                   uim_ptr);

    /* Set the instruction class for the APDU */
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

    /* Set the CHV type */
    uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV1;  /* CHV1 */

  }
  else if ((uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin1_data.chv > UIM_CHV2) &&
           (uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin1_data.chv < UIM_CHV_MAX) &&
           (uim_ptr->state.recovery_data.protocol == UIM_UICC))
  {
    UIMDRV_MSG_MED_1(uim_ptr->id,"Verifying PIN1 on UICC for channel- %d",
                     uim_ptr->state.recovery_data.ch_to_be_recovered);
    /* Format the CHV digits in the cache buffer. */
    uim_format_chv(uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin1_data.chv_digits,
                   uim_ptr);

    /* Set the instruction class for the APDU */
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1,
         uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].channel);

    /* Set the CHV type */
    uim_set_uicc_pin_param_p2( uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin1_data.chv,
                              (byte*) &(uim_req_buf_static_ptr->apdu_hdr.p2),
                               uim_ptr);
  }
  else
  {
    UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_102,uim_ptr->id,"Wrong CHV1/PIN1 %d recd for %d protocol",
             uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin1_data.chv,
                      uim_ptr->state.recovery_data.protocol);
    /* Do not verify CHV1 and skip this state */
    ++uim_ptr->command.generic_state_ptr;
    /* Call this function to process the next state. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }

  /* Build a verify CHV APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = VERIFY_CHV;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

  /* bytes of data sent in the command */
  uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS;

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.verify_chv.chv,
                    sizeof(uim_req_buf_static_ptr->req.verify_chv.chv),
                    uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin1_data.chv_digits,
                    UIM_MAX_CHV_DIGITS);

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_recovery_verify_chv1_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_VERIFY_CHV2_ST
=============================================================================*/
void uimgen_iso_handle_recovery_verify_chv2_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Determine which CHV is to be verified. */
  if ((uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin2_data.chv == UIM_CHV2) &&
           (uim_ptr->state.recovery_data.protocol == UIM_ICC))
  {
    UIMDRV_MSG_MED_0(uim_ptr->id,"Verifying PIN2 on ICC");
    /* Format the CHV digits in the cache buffer. */
    uim_format_chv(uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin2_data.chv_digits,
                   uim_ptr);

    /* Set the instruction class for the APDU */
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

    /* Set the CHV type */
    uim_req_buf_static_ptr->apdu_hdr.p2 = (byte) UIM_CHV2;  /* CHV2 */

  }
  else if ((uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin2_data.chv > UIM_CHV2) &&
           (uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin2_data.chv < UIM_CHV_MAX) &&
           (uim_ptr->state.recovery_data.protocol == UIM_UICC))
  {
    UIMDRV_MSG_MED_1(uim_ptr->id,"Verifying PIN2 on UICC for channel- %d",
                     uim_ptr->state.recovery_data.ch_to_be_recovered);
    /* Format the CHV digits in the cache buffer. */
    uim_format_chv(uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin2_data.chv_digits,
                    uim_ptr);

    /* Set the instruction class for the APDU */
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1,
         uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].channel);

    /* Set the CHV type */
    uim_set_uicc_pin_param_p2( uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin2_data.chv,
                              (byte*) &(uim_req_buf_static_ptr->apdu_hdr.p2),
                               uim_ptr);

  }
  else
  {
    UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_103,uim_ptr->id,"Wrong CHV2/PIN2 %d recd for %d protocol",
             uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin2_data.chv,
                      uim_ptr->state.recovery_data.protocol);

   /* Do not verify CHV2 and skip this state */
   ++uim_ptr->command.generic_state_ptr;

    /* Call this function to process the next state. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }

  /* Build a verify CHV or a VERIFY PIN APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = VERIFY_CHV;

  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

  uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS;

  /* Copy the CHV digits from data provided by MMGSDI */
  uim_memscpy( (byte*) uim_req_buf_static_ptr->req.verify_chv.chv,
                    sizeof(uim_req_buf_static_ptr->req.verify_chv.chv),
                    uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].pin2_data.chv_digits,
                    UIM_MAX_CHV_DIGITS);

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_recovery_verify_chv2_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_PERFORM_TP_ST
=============================================================================*/
void uimgen_iso_handle_recovery_perform_tp_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if(0 == uim_ptr->cache.int_term_profile_data.num_bytes)
  {
    /* No Terminal Profile in UIM cache, move to done state */
    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_104,uim_ptr->id,"No Terminal Profile data in UIM cache");
    
    if(uim_ptr->setting.work_arounds_for_non_spec_cards & UIM_CONTINUE_RECOVERY_WITHOUT_TP)
    {
      /* Move to the next state */
      uim_ptr->command.generic_state_ptr ++;
      UIMDRV_MSG_HIGH_0(uim_ptr->id, "Continue Recovery Procedure without Terminal Profile");
    }
    else
    {
      /* Do not verify CHV2 and skip this state */
      uim_ptr->mode.generic_states[0] = UIM_DONE_ST;
      uim_ptr->command.generic_state_ptr = uim_ptr->mode.generic_states;
    }
    /* Call this function to process the next state. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }

  /* Remove Terminal Response commands on this slot from UIM queue */
  q_linear_delete(&uim_ptr->task.cmd_q,
                  uim_compare_toolkit_cmds,
                  uim_ptr,
                  uim_remove_toolkit_cmds);

  /* Build a Terminal Profile APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = TERMINAL_PROFILE;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  /* P1 = P2 = 0 and data field is empty in UICC implies that the
  command applies on the current EF */
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;

  if (uim_ptr->state.recovery_data.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UICC_INSTRUCTION_CLASS2;
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  UIMDRV_MSG_MED_1(uim_ptr->id,"Sending Terminal Profile for protocol 0x%x", uim_ptr->state.recovery_data.protocol);
  /* Number of bytes of Terminal Profile */
  uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->cache.int_term_profile_data.num_bytes;

  /* Copy the data from the internal buffer */
  uim_memscpy (uim_req_buf_static_ptr->req.data,
         sizeof(uim_req_buf_static_ptr->req.data),
         uim_ptr->cache.int_term_profile_data.data,
         uim_ptr->cache.int_term_profile_data.num_bytes );

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_recovery_perform_tp_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_CHECK_REHAB_REQD_ST
=============================================================================*/
void uimgen_iso_handle_recovery_check_rehab_reqd_st
(
  uim_instance_global_type *uim_ptr
)
{
  if((UIM_NUM_REHAB_FILES == uim_ptr->state.recovery_data.rehab_file_index) ||
     (!uim_ptr->state.recovery_data.rehab_data[uim_ptr->state.recovery_data.rehab_file_index].rehab_reqd))
  {
    /* No more file to Rehabilitate */
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"No more Files to Rehabilitate");
    /* Do not Rehabilitate any more file */
    uim_ptr->command.generic_state_ptr += 3;

    /* Call this function to process the next state. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }

  /* path position and path are all that is needed for ICC select populate
     command here and transition to the select state to handle the select */
  uim_ptr->command.cmd_ptr->select.path_position = 0;

  uim_ptr->command.cmd_ptr->select.path =
    uim_ptr->state.recovery_data.rehab_data[uim_ptr->state.recovery_data.rehab_file_index].path;

  /* Move to select state */
  ++uim_ptr->command.generic_state_ptr;
  /* Call this function to process the next state. */
  uim_generic_command (uim_ptr);
} /* uimgen_iso_handle_recovery_check_rehab_reqd_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_REHAB_ST
=============================================================================*/
void uimgen_iso_handle_recovery_rehab_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Build a rehabilitate APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = REHABILITATE;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_1;

  /* P1 = P2 = 0 and data field is empty in UICC implies that the
     command applies on the current EF */
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p3 = 0;

  uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_recovery_rehab_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_CHECK_EF_TST_UPDATE_REQD_ST
=============================================================================*/
void uimgen_iso_handle_recovery_check_ef_tst_update_reqd_st
(
  uim_instance_global_type *uim_ptr
)
{
  uint8 ch_no = uim_ptr->state.recovery_data.ch_to_be_recovered;

  if(0 == uim_ptr->state.recovery_data.app_data[ch_no].ef_tst_data.length)
  {
    /* EF-TST update is not required */
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"EF-tst update not required");
    if(uim_ptr->state.recovery_data.protocol == UIM_UICC)
    {
      /* Increment channel index */
      ++uim_ptr->state.recovery_data.ch_to_be_recovered;
      /* Move to next channel*/
      uim_ptr->command.generic_state_ptr -= 7;
    }
    else
    {
      uim_ptr->mode.generic_states[0] = UIM_DONE_ST;
      uim_ptr->command.generic_state_ptr = uim_ptr->mode.generic_states;
    }

    /* Call this function to process the next state. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }

  /* path position and path needs to be populated for select
     command here and transition to the select state to handle the select */
  if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_ptr->command.cmd_ptr->select.path.path[0]        = UIM_MF_ID;
    uim_ptr->command.cmd_ptr->select.path.path[1]        = 0x7FFF;
    uim_ptr->command.cmd_ptr->select.path.path[2]        = 0x7F66;
    uim_ptr->command.cmd_ptr->select.path.path[3]        = 0x6FD2;

    uim_ptr->command.cmd_ptr->select.path.len            = 4;
  }
  else
  {
    uim_ptr->command.cmd_ptr->select.path.path[0]        = UIM_MF_ID;
    uim_ptr->command.cmd_ptr->select.path.path[1]        = 0x7F66;
    uim_ptr->command.cmd_ptr->select.path.path[2]        = 0x6FD2;

    uim_ptr->command.cmd_ptr->select.path.len            = 3;
  }
  uim_ptr->command.cmd_ptr->select.path_position       = 0;
  uim_ptr->command.cmd_ptr->select.file_type           = UIM_EF;
  uim_ptr->command.cmd_ptr->select.aid.aid_length      = 0;

  uim_ptr->command.cmd_ptr->hdr.channel                = ch_no;

  /* Move to select EF-TST state */
  ++uim_ptr->command.generic_state_ptr;
  /* Call this function to process the next state. */
  uim_generic_command (uim_ptr);
} /* uimgen_iso_handle_recovery_check_ef_tst_update_reqd_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_SELECT_EF_TST_ST
=============================================================================*/
void uimgen_iso_handle_recovery_select_ef_tst_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  uim_select_state(uim_ptr, &uim_ptr->command.cmd_ptr->select.path, uim_req_buf_static_ptr);
}/*uimgen_iso_handle_recovery_select_ef_tst_st*/


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_WRITE_EF_TST_ST
=============================================================================*/
void uimgen_iso_handle_recovery_write_ef_tst_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;
  uint8 ch_no = uim_ptr->state.recovery_data.ch_to_be_recovered;

  /* Build a update binary APDU */
  uim_req_buf_static_ptr->apdu_hdr.instrn = UPDATE_BINARY;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

  /* Offset into the EF -- high byte, low byte */
  uim_req_buf_static_ptr->apdu_hdr.p1 = 00;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 00;

  uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->state.recovery_data.app_data[ch_no].ef_tst_data.length;

  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
       uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, ch_no);
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_INSTRUCTION_CLASS;
  }

  uim_memscpy(uim_req_buf_static_ptr->req.data,
         sizeof(uim_req_buf_static_ptr->req.data),
         uim_ptr->state.recovery_data.app_data[ch_no].ef_tst_data.data,
         uim_ptr->state.recovery_data.app_data[ch_no].ef_tst_data.length);

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_recovery_write_ef_tst_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_MANAGE_CHANNEL_FOR_UICC_ST
=============================================================================*/
void uimgen_iso_handle_recovery_manage_channel_for_uicc_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /*All the channels till the last channel which was opened before recovery needs to be opened*/
  if(uim_ptr->state.recovery_data.ch_to_be_recovered > uim_max_ch_num_opened(uim_ptr))
  {
    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_105,uim_ptr->id,"No more channels to recover");
    uim_ptr->state.recovery_data.ch_to_be_recovered=0;
    uim_ptr->command.generic_state_ptr ++;

    /* Call this function to process the same state for next channel. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }
  else if(!uim_ptr->state.recovery_data.ch_to_be_recovered)
  {
    ++uim_ptr->state.recovery_data.ch_to_be_recovered;
    /* Call this function to process the same state(Manage Channel)
       for next channel. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }

  /* Build a Manage Channel APDU to open channel */
  uim_req_buf_static_ptr->apdu_hdr.instrn = MANAGE_CHANNEL;
  uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UTIL_INSTRUCTION_CLASS2;

  /* For open channel P2 = 0 */
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0;
  uim_req_buf_static_ptr->apdu_hdr.p3 = 1;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;
  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_recovery_manage_channel_for_uicc_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_CLOSE_CHANNEL_FOR_UICC_ST
=============================================================================*/
void uimgen_iso_handle_recovery_close_channel_for_uicc_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if(uim_ptr->state.recovery_data.ch_to_be_recovered > uim_max_ch_num_opened(uim_ptr))
  {
    /* All channels recovered, move to UPIN state */
    uim_ptr->command.generic_state_ptr += 10;
    UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_107,uim_ptr->id,"No more channels to be closed. Processing state 0x%ld",
                      *uim_ptr->command.generic_state_ptr);
    /* Call this function to process the same state for next channel. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }
  else if(!uim_ptr->state.recovery_data.ch_to_be_recovered)
  {
    /* Skip Manage Channel for default channel */
    UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_108,uim_ptr->id,"Process further states for default channel =%d ",
                    uim_ptr->state.recovery_data.ch_to_be_recovered);
    /* Move to check for select state */
    ++uim_ptr->command.generic_state_ptr;
    /* Call this function to process the same state for next channel. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }
  else if(TRUE == (uim_ptr->state.is_ch_open[uim_ptr->state.recovery_data.ch_to_be_recovered] &&
           uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].is_ch_valid &&
          !uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].do_not_recover))
  {
    UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_109,uim_ptr->id,"Process further states for Channel=%d",
                      uim_ptr->state.recovery_data.ch_to_be_recovered);
    /* Move to next state */
    ++uim_ptr->command.generic_state_ptr;
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }
  else
  {
    UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_110,uim_ptr->id,"Channel=%d is being closed, as per MMGSDI request- 0x%x",
                        uim_ptr->state.recovery_data.ch_to_be_recovered,
                        uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].do_not_recover);

     /* Build a Manage Channel APDU to Close channel */
    uim_req_buf_static_ptr->apdu_hdr.instrn = MANAGE_CHANNEL;
    uim_req_buf_static_ptr->apdu_hdr.uim_class = UIM_UTIL_INSTRUCTION_CLASS2;

    uim_req_buf_static_ptr->apdu_hdr.p1 = 0x80;
    uim_req_buf_static_ptr->apdu_hdr.p2 =
               uim_ptr->state.recovery_data.ch_to_be_recovered;
    uim_req_buf_static_ptr->apdu_hdr.p3 = 0x0;
    uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_1;

    uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;
    uim_send_command(uim_req_buf_static_ptr, uim_ptr);
  }
} /* uimgen_iso_handle_recovery_close_channel_for_uicc_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_CHECK_FOR_SELECT_AID_ST
=============================================================================*/
void uimgen_iso_handle_recovery_check_for_select_aid_st
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].is_aid_valid == FALSE)
  {
    /* No AID present */
    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_112,uim_ptr->id,"AID is invalid, moving to next channel");
    /* Move to next channel */
    ++uim_ptr->state.recovery_data.ch_to_be_recovered;
    /* Move to Manage Channel state for next channel */
    --uim_ptr->command.generic_state_ptr;
    /* Call this function to process the same state for next channel. */
    uim_generic_command (uim_ptr);
    return; /* exit immediately */
  }
  UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_113,uim_ptr->id,"Send Select AID for channel=%d",
                    uim_ptr->state.recovery_data.ch_to_be_recovered);

  uim_ptr->command.cmd_ptr->select.file_type    = UIM_ADF;
  uim_ptr->command.cmd_ptr->select.path.path[0] = 0x3F00;
  uim_ptr->command.cmd_ptr->select.path.path[1] = 0x7FFF;
  uim_ptr->command.cmd_ptr->select.path.len     = 2;
  uim_ptr->command.cmd_ptr->hdr.channel         = uim_ptr->state.recovery_data.ch_to_be_recovered;

  uim_ptr->command.cmd_ptr->select.session =
    uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].session_control;
  uim_ptr->command.cmd_ptr->select.aid     =
    uim_ptr->state.recovery_data.app_data[uim_ptr->state.recovery_data.ch_to_be_recovered].aid;

  /* Move to Select State */
  ++uim_ptr->command.generic_state_ptr;
  /* Call this function to process the select state. */
  uim_generic_command (uim_ptr);
} /* uimgen_iso_handle_recovery_check_for_select_aid_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_VERIFY_UPIN_FOR_UICC_ST
=============================================================================*/
void uimgen_iso_handle_recovery_verify_upin_for_uicc_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if ((uim_ptr->state.recovery_data.upin_data.chv == UIM_UNIVERSAL_PIN) &&
      (uim_ptr->state.recovery_data.protocol == UIM_UICC))
  {
    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_114,uim_ptr->id,"Send UPIN verification on default channel");
    /* Format the CHV digits in the cache buffer. */
    uim_format_chv(uim_ptr->state.recovery_data.upin_data.chv_digits,
                   uim_ptr);

    /* Set the instruction class for the APDU */
    uim_req_buf_static_ptr->apdu_hdr.uim_class =  UIM_UICC_INSTRUCTION_CLASS1 ;

    /* Set the CHV type */
    uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_UNIVERSAL_PIN_VALUE;

    /* Build a verify CHV APDU */
    uim_req_buf_static_ptr->apdu_hdr.instrn = VERIFY_CHV;
    uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

    uim_req_buf_static_ptr->apdu_hdr.p1 = 0;

    /* bytes of data sent in the command */
    uim_req_buf_static_ptr->apdu_hdr.p3 = UIM_MAX_CHV_DIGITS;

    uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

    uim_memscpy( (byte*) uim_req_buf_static_ptr->req.verify_chv.chv,
                      sizeof(uim_req_buf_static_ptr->req.verify_chv.chv),
                      uim_ptr->state.recovery_data.upin_data.chv_digits,
                      UIM_MAX_CHV_DIGITS);

    uim_send_command(uim_req_buf_static_ptr, uim_ptr);
  }
  else
  {
    /* UPIN not available, move to next state*/
    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_115,uim_ptr->id,"UPIN not available");
    /* Move to select state */
    ++uim_ptr->command.generic_state_ptr;
    /* Call this function to process the next state. */
    uim_generic_command (uim_ptr);
  }
} /* uimgen_iso_handle_recovery_verify_upin_for_uicc_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_GET_RESPONSE_ST
=============================================================================*/
void uimgen_iso_handle_get_response_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Sending Get Response");
  if ((uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC) ||
      (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_WCDMA))
  {
    /*
      In case of streaming APDU do not mask bit 8 and bit 5 as some proprietary application
      do want GET RESPONSE with exact same CLA byte value as the one in original
      C-APDU header. If CLA is 0xFF which is a reserved value then print error and
      let card generate the error for transaction
    */
    if(uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_CLASS_OFFSET] == 0xFF)
    {
      UIMDRV_MSG_ERR_1(uim_ptr->id,"Invalid CLA byte 0x%x",
        uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_CLASS_OFFSET]);
    }
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
      uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_CLASS_OFFSET];
    
    /* Keep the uim_class of the extended APDU request*/
    if(uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes > UIM_MAX_STREAM_ISO7816_APDU_TRANSACTION_SIZE)
    {
      uim_req_buf_static_ptr->apdu_hdr.uim_class = uim_req_buf_static_ptr->apdu_hdr.uim_class & 0x0F;
    }

    if ( UIM_PASSIVE_MODE == uim_ptr->command.mode)
    {
      /* In passive mode, UIM will support only ETSI CLA mask */
      uim_req_buf_static_ptr->apdu_hdr.uim_class =
         uim_req_buf_static_ptr->apdu_hdr.uim_class & UIM_GET_RESPONSE_CLA_MASK;
    }
  }
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.uim_class = 0xA0;
  }
  uim_req_buf_static_ptr->apdu_hdr.instrn = GET_RESPONSE;
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;
  uim_req_buf_static_ptr->apdu_hdr.p1 = 0x00;
  uim_req_buf_static_ptr->apdu_hdr.p2 = 0x00;

  /* When we get SW 62 XX for Case 4 instruction, we are supposed to force
     GET RESPONSE with P3 = 0. For 62 83 we force uim_get_resp_sw1 to
     value SW1_END_RESP. Hence same value is used here for sake of comparison */
  uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->card_cmd.resp_buf_ptr->sw2;

  /* Move to Stream APDU state to process the response */
  ++uim_ptr->command.generic_state_ptr;
  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uimgen_iso_handle_get_response_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SILENT_RECOVER_UICC_ST
=============================================================================*/
void uimgen_iso_handle_silent_recover_uicc_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Clear out the command response timer as we are setting the timeout signal */
  uim_ptr->flag.clear_timeout_on_response = FALSE;

  /* Set the command response timeout signal so that UIM task can initiate
     the powerdown procedure */
  (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
} /* uimgen_iso_handle_silent_recover_uicc_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_MANAGE_CHANNEL_ST
=============================================================================*/
void uimgen_iso_handle_manage_channel_st
(
  uim_instance_global_type *uim_ptr
)
{
  /* Build a Manage Channel APDU */
  uim_ptr->command.req_buf.apdu_hdr.instrn = MANAGE_CHANNEL;
  /* This should always be 0 from mmgsdi but pass what they send along */
  uim_ptr->command.req_buf.apdu_hdr.uim_class =
    uim_util_get_uim_class(UIM_UTIL_INSTRUCTION_CLASS2,
                           uim_ptr->command.cmd_ptr->manage_channel.channel);

  if(uim_ptr->command.cmd_ptr->manage_channel.mc_action == UIM_MC_OPEN)
  {
    if( TRUE == uim_ptr->state.channel_info.manage_channel_allowed &&
        0xFF != uim_ptr->state.channel_info.unallocated_channel)
    {
      UIMDRV_MSG_MED_1(uim_ptr->id, "Donot send the command to card.Channel 0x%x already opened internally.", uim_ptr->state.channel_info.unallocated_channel);
      uim_ptr->command.rsp_buf.cmd_status = UIM_DONE;
      uim_ptr->command.rsp_buf.cmd_rsp_size = 1;
      uim_ptr->command.rsp_buf.rsp.data[0] = uim_ptr->state.channel_info.unallocated_channel;
      uim_ptr->command.rsp_buf.sw1 = SW1_NORMAL_END;
      uim_ptr->command.rsp_buf.sw2 = SW2_NORMAL_END;
      uim_ptr->state.channel_info.unallocated_channel = 0xFF;
      (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG );
      return;
    }
    uim_ptr->command.req_buf.apdu_hdr.p1 = 0;
    uim_ptr->command.req_buf.apdu_hdr.p2 =
      uim_ptr->command.cmd_ptr->manage_channel.op_close_channel;
    if(uim_ptr->command.cmd_ptr->manage_channel.op_close_channel == 0x0)
    {
      uim_ptr->command.req_buf.apdu_hdr.p3 = 1;
      uim_ptr->command.req_buf.instrn_case = UIM_INSTRN_CASE_2;
    }
    else
    {
      uim_ptr->command.req_buf.apdu_hdr.p3 = 0;
      uim_ptr->command.req_buf.instrn_case = UIM_INSTRN_CASE_1;
    }
  }
  else
  {
    uim_ptr->command.req_buf.apdu_hdr.p1 = 0x80;
    uim_ptr->command.req_buf.apdu_hdr.p2 =
    uim_ptr->command.cmd_ptr->manage_channel.op_close_channel;
    uim_ptr->command.req_buf.apdu_hdr.p3 = 0x0;
    uim_ptr->command.req_buf.instrn_case = UIM_INSTRN_CASE_1;
  }

  uim_ptr->command.req_buf.rsp_ptr = &uim_ptr->command.rsp_buf;
  uim_send_command(&uim_ptr->command.req_buf, uim_ptr);
} /* uimgen_iso_handle_manage_channel_st */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SET_APDU_BEHAVIOR_ST
=============================================================================*/
void uimgen_iso_handle_set_apdu_behavior_st
(
  uim_instance_global_type *uim_ptr
)
{
  uim_channel_type          channel_id  = 0;
  boolean                   ret_status  = FALSE;

  if (uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_WAKE_UP_F)
  {
    channel_id = uim_ptr->state.recovery_data.ch_to_be_recovered;
    uim_ptr->state.ch_apdu_behavior[channel_id] =
        uim_ptr->state.recovery_data.app_data[channel_id].apdu_behavior;
  }
  else
  {
    channel_id = uim_ptr->command.cmd_ptr->set_apdu_behavior.hdr.channel;
    if(channel_id < UIM_MAX_CHANNELS)
    {
      if(uim_ptr->state.is_ch_open[channel_id] == TRUE)
      {
        if(0 != uim_ptr->state.current_path[channel_id].curr_app.aid_length)
        {
          if(uim_is_telecom_aid(uim_ptr->state.current_path[channel_id].curr_app) == FALSE)
          {
            uim_ptr->state.ch_apdu_behavior[channel_id] =
                uim_ptr->command.cmd_ptr->set_apdu_behavior.apdu_behavior;
            ret_status = TRUE;
          }
        }
      }
    }

    if(ret_status)
    {
      uim_ptr->command.rsp_buf.cmd_status = UIM_DONE;
    }
    else
    {
      uim_ptr->command.rsp_buf.cmd_status = UIM_PROBLEM;
    }
  }

  (void)rex_set_sigs(uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG);
} /* uimgen_iso_handle_set_apdu_behavior_st */

/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_NOT_PUP_IF_ALREADY_ST
=============================================================================*/
void uimgen_iso_handle_not_pup_if_already_st
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr == NULL)
  {
    return;
  }  
  (void)rex_set_sigs(uim_ptr->tcb_ptr,UIM_CMD_RSP_SIG);
}

/*=============================================================================
  GENERIC RESPONSE STATES
==============================================================================*/
/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_POWER_DOWN_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_power_down_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  ++uim_ptr->command.generic_state_ptr;
  *completed = UIM_CMD_COMPLETED;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_power_down_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_POWER_UP_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_power_up_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Get the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_power_up_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RESET_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_reset_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  byte i = 0;

  uim_clock_rate_conversion_factor_type  FI = UIM_CRCF_372_1; /* CRCF index */
  uim_baud_rate_adjustment_factor_type   DI = UIM_BRAF_1; /* BRAF index */
  /* Boolean variable to specific mode check */
  boolean skip_specific_mode_check = FALSE;
  /* Boolean variable to check for mode changeability */
  boolean skip_mode_changeability_check = FALSE;
  byte   j       = 0;
  uint32 tempATR[8];

  /* Holds the UI (voltage class indicator) value found in the first TA for T=15 */
  uim_uicc_voltage_class_type voltage_class_indicator;

  /* IF TA2 is present and PPS is required then check for global specif mode */
  uim_global_specific_mode_type global_specific_mode;

  if( uim_ptr->setting.phy_layer != UIM_ISO )
  {
    /* Need not parse ATR as we need only historical char data
       which is obtained from non ISO interface */
    ++uim_ptr->command.generic_state_ptr;
    return UIM_CMD_SUCCESS;
  }
  /* variable is static so re-initialize */
  uim_ptr->flag.dont_skip_PPS = FALSE;
  uim_ptr->flag.is_euicc      = FALSE;

  /* Reset the protocols */
  uim_ptr->state.prot_sup_by_card.t_0 = FALSE;
#if defined( FEATURE_UIM_T_1_SUPPORT )
  uim_ptr->state.prot_sup_by_card.t_1 = FALSE;
#endif /* FEATURE_UIM_T_1_SUPPORT */


  memset(tempATR, 0x00, sizeof(tempATR));

  /* Print any data received from the card before it's examined */
  for( i=0; (((4*i) < (rsp_ptr->cmd_rsp_size)) || ((4*i) < (UIM_MAX_ATR_CHARS))) ; i++)
  {
    for ( j = 0; (j < 4)&&((4*i+j) < rsp_ptr->cmd_rsp_size); j++ )
    {
      tempATR[i] = (tempATR[i]<<8)|(rsp_ptr->rsp.data[4*i + j]);
    }
  }
  UIMDRV_MSG_MED_8(uim_ptr->id, "atr bytes: %x %x %x %x %x %x %x %x",
                   tempATR[0], tempATR[1], tempATR[2],tempATR[3],tempATR[4],tempATR[5] ,tempATR[6],tempATR[7]);

  if(rsp_ptr->cmd_rsp_size == UIM_MAX_ATR_CHARS)
  {
    UIMDRV_MSG_MED_1(uim_ptr->id,"Last byte (33rd byte) of ATR is %x",rsp_ptr->rsp.data[UIM_MAX_ATR_CHARS -1]);
  }

  /* Reset this global to NV value upon ATR reception */
  uim_ptr->hotswap.retry_power_up_count = uim_ptr->extended_recovery.recovery_config.no_of_extended_power_up_attempts;

  /* Set is_first_atr_received to TRUE */
  uim_ptr->atr.is_first_atr_received = TRUE;

  /* Check to see if there was a convention error */
  if (rsp_ptr->cmd_status == UIM_CONVENTION_ERROR)
  {
      /* If the number of times we are switching between direct & inverse
         convention is greater than the max number of attempts */
      if (++uim_ptr->debug.convention_change_count > UIM_MAX_NUM_ATTEMPTS)
      {
        /* Return a cmd error after setting the command count to MAX */
        uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
        return(UIM_CMD_ERR);
      }
      else
      {
        /* Return a convention changes error that will not increment the
        wake-up or power-up command count */
        return(UIM_CONVENTION_CHANGE);
      }
  }
  /* Check if there was a problem with the Reset
     also return error if by some chance the response size becomes 0 */
  if (rsp_ptr->cmd_status != UIM_DONE ||
      0 == rsp_ptr->cmd_rsp_size)
  {
    return(UIM_CMD_ERR);
  } /* end if - there was a problem with the Reset */
  
  UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
  
  uim_ptr->atr.atr_buf.num_bytes = (byte)rsp_ptr->cmd_rsp_size;

  /*if the atr is being recieved from RX_ISR (local sim)
    then there is no issue, as there are already checks present there for max length.
    the check hereis for precaution, if the ATR is recieved from a different source (like remote sim)*/
  if(UIM_MAX_ATR_CHARS < uim_ptr->atr.atr_buf.num_bytes)
  {
    uim_ptr->atr.atr_buf.num_bytes = UIM_MAX_ATR_CHARS;
  }
  uim_memscpy(uim_ptr->atr.atr_buf.data,
         sizeof(uim_ptr->atr.atr_buf.data),
         rsp_ptr->rsp.data,
         rsp_ptr->cmd_rsp_size);
  
  UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);

  /* Validate the ATR here */
  /* Check for an intelligible start byte */

  /* Parsing and populating ATR bytes, TCK Byte are also checked here*/
  if(!uim_util_atr_parse_and_populate(uim_ptr,
                                      (uint8)rsp_ptr->cmd_rsp_size,
                                      (uint8 *)&rsp_ptr->rsp.data[0]))
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"Error in ATR parsing");
    return(UIM_CMD_ERR);
  }

  /* Format byte T0  and Generic Check */
  if (!uim_util_is_format_byte_valid_in_atr(uim_ptr) ||
      !uim_is_protocol_valid_in_atr(uim_ptr))
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"Error in ATR parsing");
    return(UIM_CMD_ERR);
  }

  /* The number of historical byes received in ATR */
  uim_ptr->atr.hist_char_buf.length = uim_util_get_historical_bytes_len_from_atr(uim_ptr);
  
  /* populate formal argument with historical bytes */
  if(uim_util_get_historical_bytes_from_atr(uim_ptr, (uint8 *)&uim_ptr->atr.hist_char_buf.data))
  {
    if(!uim_util_is_mf_present_indicated_in_atr(uim_ptr) &&
        uim_ptr->card_mode != UIM_CARD_SAM               &&
        uim_is_feature_sam_enabled())
    {
      /* No MF is indicated by ATR
         Switch the card mode to SAM mode */
      return (UIM_SWITCH_MODE);
    } 
  }

  /* First check that ATR have any specific voltage or not */
  /*  The first TA (TA3 \ TA4) for T=15 encodes the clock stop indicator(X)
      and the class indicator (Y). */
  uim_ptr->flag.voltage_class_known_from_atr =
                 uim_util_get_voltage_support_from_atr(uim_ptr,&voltage_class_indicator);

  if( uim_ptr->flag.voltage_class_known_from_atr )
  {
      if((voltage_class_indicator.only_class_b_supported) &&
         (voltage_class_indicator.only_class_c_supported))
      {
        uim_ptr->flag.voltage_class_c_and_b_supported = TRUE;
      }
      /* Determine the current voltage on the interface */
      switch (uim_ptr->state.current_voltage_class)
      {
        case UIM_VOLTAGE_CLASS_C:
          {
            /* Check if the card does not support this voltage */
            if(!voltage_class_indicator.only_class_c_supported)
            {
              UIMDRV_MSG_LOW_0(uim_ptr->id, "Card does not support 1.8V");
              /* Check if card supports other supported voltages */
              if(voltage_class_indicator.only_class_b_supported)
              {
                if (uim_ptr->hardware.uim_ldo.customize_ldo_setting &&
                    uim_ptr->hardware.uim_ldo.desired_voltage_class == UIM_VOLTAGE_CLASS_C)
                {
                  UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_118,uim_ptr->id,"LDO behaviour modified to work at specific voltage class");
                  return(UIM_VOLTAGE_MISMATCH);
                }
                /* Switch to different voltage */
                uim_power_down(uim_ptr);
                uim_timed_sleep(UIM_ISO7816_VOLTAGE_SWITCH_PWR_DOWN_DELAY,
                                uim_ptr,
                                UIM_ALL_ZERO_SIG);
                /* Try the indicated voltage class */
                uim_ptr->state.current_voltage_class = UIM_VOLTAGE_CLASS_B;
                UIMDRV_MSG_LOW_0(uim_ptr->id, "Card supports 3V mode");
                return(UIM_SWITCH_VOLTAGE);
              }
              else /* No voltages supported */
              {
                UIMDRV_MSG_LOW_0(uim_ptr->id, "Card does not support 3V mode either");
                return(UIM_VOLTAGE_MISMATCH);
              } /* end if - card supports other voltage */
            } /* end if - card does not support this voltage */
          } /* end case - UIM_VOLTAGE_1_8V */
          break;

        case UIM_VOLTAGE_CLASS_B:
          {
            /* Check if the card does not support this voltage */
            if(!voltage_class_indicator.only_class_b_supported)
            {
              UIMDRV_MSG_LOW_0(uim_ptr->id, "Card does not support 3V");
              return(UIM_VOLTAGE_MISMATCH);
            } /* end if - card does not support this voltage */
          } /* end case - UIM_VOLTAGE_3V*/
          break;

        default:
          {
            UIMDRV_MSG_ERR_0(uim_ptr->id, "Bad UIM voltage class" );
            return(UIM_VOLTAGE_MISMATCH);
          } /* end case - default */
      } /* end switch - voltage class */

    /* Check if FI value is > 0 and if clk freq needs to be changed */
    if ( UIM_IS_CLK_FREQ_AT_4_8_MHZ(uim_ptr->id) &&
         UIM_ATR_FI_VALUE_IS_ZERO_OR_INVALID(uim_ptr->id) &&
         uim_ptr->decoded_atr.interface_byte.TAx[0].is_TA_byte_present )
    {
      UIMDRV_MSG_LOW_0(uim_ptr->id,"UIM_RESET_ST FI is 0");
      uim_power_down(uim_ptr);
      uim_hotswap_reset_slot(uim_ptr);
      UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ(uim_ptr->id);
      SET_UIM_BAUD_RATE_SLOT ( uim_ptr->id, UIM_CRCF_372_1, UIM_BRAF_1 );
      return(UIM_CLK_FREQ_CHANGE_NEEDED);
    }
  } /* uim_util_get_voltage_support_from_atr */

  if (!uim_util_get_clock_stop_indicator_from_atr(uim_ptr, &uim_ptr->state.clock_stop_cntrl))
  {
    /* Set default clock stop control to CLOCK_STOP_LOW*/
    uim_ptr->state.clock_stop_cntrl = UIM_CLOCK_STOP_LOW;
  }

  /* Checking the protocol value of the ATR and device preference */
  /*  TDi Structural, encodes TAi+1, TBi+1, TCi+1. TD+1 and preferred protocol */
  uim_ptr->state.prot_sup_by_card.t_0 = uim_util_is_T0_supported_in_atr(uim_ptr);

#if defined( FEATURE_UIM_T_1_SUPPORT )
  uim_ptr->state.prot_sup_by_card.t_1 = uim_util_is_T1_supported_in_atr(uim_ptr);
  /* If the preferred protocol is T = 1 */
  if (uim_ptr->t1.nv_pref_protocol == NV_UIM_PREF_PROTOCOL_T_1)
  {
    /* If the card supports T=1 protocol */
    if (uim_ptr->state.prot_sup_by_card.t_1)
    {
      /* Set the current protocol to T = 1 */
      uim_ptr->state.operating_protocol = UIM_T_1_PROTOCOL;
      /* Determine if T=0 is supported also */
      if (uim_ptr->state.prot_sup_by_card.t_0)
      {
        /* Do not skip the PPS since there are two protocols supported
           and we will use T=1 */
        uim_ptr->flag.dont_skip_PPS = TRUE;
      }/* end if - T=0 is also supported */
    }
    else if (uim_ptr->state.prot_sup_by_card.t_0)
    {
      /* Set the current protocol to T = 0 */
      uim_ptr->state.operating_protocol = UIM_T_0_PROTOCOL;
    }
    else
    {
      /* Return an error */
      UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
      uim_ptr->atr.atr_buf.num_bytes = 0;
      UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
      return (UIM_CMD_ERR);
    }
  }
  /* Else if the preferred protocol is T=0 or No preference */
  else if ((uim_ptr->t1.nv_pref_protocol == NV_UIM_PREF_PROTOCOL_T_0) ||
           (uim_ptr->t1.nv_pref_protocol == NV_UIM_PREF_PROTOCOL_NONE))
  {
    if (uim_ptr->state.prot_sup_by_card.t_0)
    {
      /* Set the current protocol to T = 0 */
      uim_ptr->state.operating_protocol = UIM_T_0_PROTOCOL;
    }
    else if (uim_ptr->state.prot_sup_by_card.t_1)
    {
      /* Set the current protocol to T = 1 */
      uim_ptr->state.operating_protocol = UIM_T_1_PROTOCOL;
    }
    else
    {
      /* Return an error for ATR */
      UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
      uim_ptr->atr.atr_buf.num_bytes = 0;
      UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);	  
      return (UIM_CMD_ERR);
    }
  }
#else
  if (!uim_ptr->state.prot_sup_by_card.t_0)
  {
    UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
    uim_ptr->atr.atr_buf.num_bytes = 0;
    UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
    return (UIM_CMD_ERR);
  }
#endif /* FEATURE_UIM_T_1_SUPPORT */
  /*  TA2 Global, specific mode byte */
  if(!uim_util_get_specific_mode_info_from_atr(uim_ptr, &global_specific_mode))
  {
    global_specific_mode.uim_atr_ta2_byte_present = FALSE;
  }

  if (uim_util_is_pps_to_be_performed_in_atr(uim_ptr))
  {
    /* Do not skip PPS.  Use these parameters */
    uim_ptr->flag.dont_skip_PPS = TRUE;
    /*  TA1 Global, encodes Fi and Di */
    (void)uim_util_get_fi_di_from_atr(uim_ptr,&FI,&DI);
    /* Place parameters back into the ATR for processing of the
       PPS */
    uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX] =
      (uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX] & (~UIM_DI_MASK_OF_TA1))
      | (int)DI;
    } /* check for TA1 */
    else
    {
      /* Skip the PPS */
      uim_ptr->flag.dont_skip_PPS = FALSE;
      /* Skip the subsequent specific mode check */
      skip_specific_mode_check = TRUE;
    }

    /* Enter the specific mode check */
  if(!skip_specific_mode_check )
    {
      /* Is SIM in specific mode? */
      if( global_specific_mode.uim_atr_ta2_byte_present )
      {
        /* Don't perform PPS as the SIM is in specific mode */
        uim_ptr->flag.dont_skip_PPS = FALSE;

        /* Is TA1 supported by the platform? */
        if( uim_is_fi_di_supported(uim_ptr, FI, DI) )
        {
          /* Check if b5 bit is set in TA2 */
          if(!global_specific_mode.uim_atr_b5_bit_ta2)
          {
            skip_mode_changeability_check = TRUE;
          }
          else
          {
            /* Use implicit values for F and D.  Cause this by
            indicating TA1 is not in the ATR */
            uim_ptr->command.rsp_buf.rsp.data[UIM_ATR_T0_BYTE] &= ~UIM_ATR_TA_PRESENT;
          }
        }

        /* Enter the mode changeability check */
        if(skip_mode_changeability_check != TRUE)
        {
          /* Check if mode change is possible */
          if(!global_specific_mode.uim_atr_b8_bit_ta2)
          {
            /* Check if we had done warm reset earlier within the same powerup */
            if(uim_ptr->debug.warm_reset_count > 0)
            {
              /* Set this to 0 as this is a static variable */
              uim_ptr->debug.warm_reset_count = 0;

              /* Is Implicit baud-rate possible? */
              if(global_specific_mode.uim_atr_b5_bit_ta2)
              {
                /* Use implicit values for F and D.  Cause this by
                indicating TA1 is not in the ATR */
                uim_ptr->command.rsp_buf.rsp.data[UIM_ATR_T0_BYTE] &= ~UIM_ATR_TA_PRESENT;
              }
              else
              {
                /* Return error as this SIM is not useable */
                /* Convert this to MSG_ERROR, if need be */
                UIMDRV_MSG_HIGH_0(uim_ptr->id, "THIS SIM IS NOT USEABLE BY OUR TERMINAL DEVICE");
                UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
                uim_ptr->atr.atr_buf.num_bytes = 0;
                UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
                return (UIM_CMD_ERR);
              }
            }
            else
            {
              if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_ENABLE_SIM_MODE_CHANGE_VIA_WARM_RESET,
                                           uim_ptr))
              {
                uim_ptr->debug.warm_reset_count++;
              }
              else
              {
                if(global_specific_mode.uim_atr_b5_bit_ta2)
                {
                  /* Use implicit values for F and D.  Cause this by
                     indicating TA1 is not in the ATR */
                  uim_ptr->command.rsp_buf.rsp.data[UIM_ATR_T0_BYTE] &= ~UIM_ATR_TA_PRESENT;
                }
                else
                {
                  /* Return error as this SIM is not useable */
                  UIMDRV_MSG_HIGH_0(uim_ptr->id,"THIS SIM IS NOT USEABLE BY OUR TERMINAL DEVICE");
                  UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
                  uim_ptr->atr.atr_buf.num_bytes = 0;
                  UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
                  return (UIM_CMD_ERR);
                }
              } /* UIMDRV_FEATURE_ENABLE_SIM_MODE_CHANGE_VIA_WARM_RESET */
            }
          }
          else /* Mode change is not possible */
          {
            /* Reset the warm reset count to 0 */
            uim_ptr->debug.warm_reset_count = 0;

            /* Is B5 bit in TA2 byte set? */
            if(global_specific_mode.uim_atr_b5_bit_ta2 )
            {
              /* Use implicit values for F and D.  Cause this by
              indicating TA1 is not in the ATR */
              uim_ptr->command.rsp_buf.rsp.data[UIM_ATR_T0_BYTE] &= ~UIM_ATR_TA_PRESENT;
            }
            else
            {
              /* Return error as this SIM is not useable */
              UIMDRV_MSG_HIGH_0(uim_ptr->id,"THIS SIM IS NOT USEABLE BY OUR TERMINAL DEVICE");
              UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
              uim_ptr->atr.atr_buf.num_bytes = 0;
              UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
              return (UIM_CMD_ERR);
            }
          }
        } /* skip_mode_changeability_check */
        else
        {
          uim_ptr->debug.warm_reset_count = 0;
        }
      }
      else /* SIM is not in specific mode */
      {
        uim_ptr->debug.warm_reset_count = 0;
        uim_ptr->flag.dont_skip_PPS = TRUE;

        /* Is TA1 supported by the platform? */
        if( uim_is_fi_di_supported(uim_ptr, FI, DI) )
        {
          uim_ptr->flag.dont_skip_PPS = TRUE;
        }
        else /* TA1 not supported by the platform */
        {
          /* Fall back to 0x96 */
          DI = UIM_BRAF_32;
          /* Place parameters back into the ATR for processing of the
             PPS */
          uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX] =
            (uim_ptr->command.rsp_buf.rsp.data[UIM_TA1_INDEX] & (~UIM_DI_MASK_OF_TA1))
            | (int)DI;

          /* Perform PPS */
          uim_ptr->flag.dont_skip_PPS = TRUE;
        }

      } /* else */
    } /* if skip_specific_mode_check ... */
    else
    {
      /* set the warm reset count to 0 */
      uim_ptr->debug.warm_reset_count = 0;
    }

    /* According to 51.011, the terminal shall reject a SIM card with TC1 value 
       other than 0 or 255. For this reason, enforcing the check only for telecom 
       cards and based on ICC protocol */
    if((uim_ptr->card_mode != UIM_CARD_SAM) &&
       ((uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_TC1_BYTE_FOR_EXTRA_GUARD_TIME, uim_ptr) == FALSE) ||
        (uim_ptr->flag.toggle_instrn_class)))
    {
      uint8 tc1_byte;
      /* If TC1 is present, check to see if it is within acceptable
         values */
      if (uim_util_get_tc1_byte_from_atr(uim_ptr, &tc1_byte))
      {
        /* Check if TC1 is acceptable */
        if (tc1_byte != UIM_TC1_IS_0)
        {
          if (tc1_byte !=  UIM_TC1_IS_255)
          {
            /* Fail the ATR from the SIM */
            UIM_ENTER_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
            uim_ptr->atr.atr_buf.num_bytes = 0;
            UIM_LEAVE_CRIT_SECTION(&uim_ptr->crit_sect.atr_ctl,uim_ptr);
            return (UIM_CMD_ERR);
          }
          else /* TC1 is the minimum value */
          {
            /* Use the minimum guardtime size */
            uim_ptr->t1.use_minimum_guardtime_size = TRUE;
          } /* end if - TC1 is not within the acceptable range */
        } /* end if - TC1 is not 0 */
      } /* end if - TC1 is present */
    }
    else
    {
      uint8 tc1_byte;
      /* Check for TC1 byte presence in ATR */
      if( uim_util_get_tc1_byte_from_atr(uim_ptr, &tc1_byte))
      {
        /* store the TC1 byte value */
        uim_ptr->atr.atr_tc1_byte = tc1_byte;
      } /* end if - TC1 is present */
    }

      /* Check if TA2 is present by checking TD1 */
  if( global_specific_mode.uim_atr_ta2_byte_present )
  {
#if defined( FEATURE_UIM_T_1_SUPPORT )
        /* Set the current protocol to the specific mode */
    uim_ptr->state.operating_protocol =  global_specific_mode.protocol& UIM_T_FIELD;
#endif /* FEATURE_UIM_T_1_SUPPORT */
  } /* end if - TA2 is present */

      /* Check if TC2 is present by checking TD1 */
        /* Save the work waiting time */
  uim_ptr->state.WI = uim_util_get_waiting_time_from_atr(uim_ptr);

#if defined( FEATURE_UIM_T_1_SUPPORT )
      /* Set initial values of send sequence numbers */
      uim_ptr->t1.me_send_sequence_num = UIM_T_1_INIT_SEQ_NUM;
      uim_ptr->t1.card_send_sequence_num = UIM_T_1_INIT_SEQ_NUM;

      /* Set default values of IFS */
      uim_ptr->t1.t_1_ifsc = UIM_T_1_DEFAULT_IFS;
      uim_ptr->t1.t_1_ifsd = UIM_T_1_DEFAULT_IFS;

      /* Set the default waiting times indeces for T=1 protocol */
      uim_ptr->t1.t_1_bwi = UIM_T_1_DEFAULT_BWI;
      uim_ptr->t1.t_1_cwi = UIM_T_1_DEFAULT_CWI;

      /* Set the default EDC controls */
      uim_ptr->t1.t_1_edc = UIM_T_1_EDC_LRC;
      uim_ptr->t1.epilogue_field_size = UIM_T_1_LRC_SIZE;
          /* Get the protocol specific parameters */
      uim_util_update_t_1_params_from_atr(uim_ptr);
#endif /* FEATURE_UIM_T_1_SUPPORT */

  uim_ptr->flag.uicc_supports_swp = uim_util_is_swp_supported_in_atr(uim_ptr);
  if (uim_util_is_euicc_supported_in_atr(uim_ptr))
  {
    uim_ptr->flag.is_euicc = TRUE;
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"This is an eUICC card");
  }

  /* Check if this is a card that needs more delay for response bytes */
  if (uim_ptr->atr.hist_char_buf.length > UIM_HIST_CHAR_CMP_SIZE)
  {
    if (!memcmp(uim_ptr->atr.hist_char_buf.data,
                UIM_MORE_DELAY_HIST_CHARS,
                UIM_HIST_CHAR_CMP_SIZE))
    {
      if ((uim_ptr->atr.hist_char_buf.data[UIM_HIST_CHAR_CMP_SIZE] >=
           UIM_MIN_MINOR_REV_MORE_DELAY) &&
          (uim_ptr->atr.hist_char_buf.data[UIM_HIST_CHAR_CMP_SIZE] <=
           UIM_MAX_MINOR_REV_MORE_DELAY))
      {
        /* Increase the response byte delay in the driver */
        uim_ptr->setting.response_byte_delay = UIM_RESPONSE_BYTE_DELAY_MORE;
      } /* end if - check the minor revision */
    } /* end if - the historical bytes match the delay more test
         string */
  } /* end if - there are enough historical bytes to check for more
       delay for the response byte */

  /* Indicate that we have already received the ATR so that we
   * will not try next voltage leavel when MULTIPLE_VOLTAGE is supported*/
  /* Move to the adjacent state if only we did not require a warm reset */
  if(uim_ptr->debug.warm_reset_count == 0)
  {
    uim_ptr->atr.atr_received = TRUE;
    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_reset_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_PPS_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_pps_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Check if there was a problem with the Reset */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    return(UIM_CMD_ERR);
  } /* end if - there was a problem with the Reset */

  /* Set the flag to TRUE */
  uim_ptr->atr.atr_pps_done = TRUE;

  /* Resetting the global as we received the PPS response */
  uim_ptr->atr.no_pps_attempts = 0;

  /* Process the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_pps_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_DELAY_AFTER_ATR_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_delay_after_atr_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* This is just to create a delay.  The delay has been done just
   * Get the next state as nothing needs to be done here
   */
  /* Adjust the generic state pointer according */
  if (FALSE == uim_ptr->flag.dont_skip_PPS)
  {
    /* Set the flag to TRUE */
    uim_ptr->atr.atr_pps_done = TRUE;
    uim_ptr->command.generic_state_ptr += 2;
  }
  else
  {
    ++uim_ptr->command.generic_state_ptr;
  } /* end if - skip the PPS */
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_delay_after_atr_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UPDATE_OP_PARAMS_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_update_op_params_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
#if defined( FEATURE_UIM_T_1_SUPPORT )
  /* Set the current protocol to the operating protocol determined
     in the ATR processing */
  uim_ptr->state.current_protocol = uim_ptr->state.operating_protocol;

  /* Check if the current protocol is not T=1 */
  if (UIM_T_1_PROTOCOL != uim_ptr->state.current_protocol)
  {
    /* Skip the IFS negotiation */
    ++uim_ptr->command.generic_state_ptr;
  } /* end if - the protocol is not T=1 */
#endif /* FEATURE_UIM_T_1_SUPPORT */

  /* Process the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
}/* uimgen_iso_handle_update_op_params_st_rsp */


#if defined (FEATURE_UIM_T_1_SUPPORT)
/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_IFS_NEG_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_ifs_neg_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Process the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_ifs_neg_st_rsp */
#endif /* FEATURE_UIM_T_1_SUPPORT */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_ICCID_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_read_iccid_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  byte i = 0;

  /* Check if there was a problem with the read */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    if (uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_WAKE_UP_F)
    {
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_124,uim_ptr->id,"ICCID read failure during recovery");
      uim_clear_recovery_params(uim_ptr);
    }

    if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_HANDLE_ICCID_READ_FAILURE,
                                 uim_ptr) == TRUE)
    {
      if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
      {
        if(!uim_ptr->flag.recovery_triggered)
        {
          /* Toggle the instruction class */
          UIMDRV_MSG_HIGH_0(uim_ptr->id, "ICCID read failure, toggling the inst class ");
          uim_ptr->flag.toggle_instrn_class = !uim_ptr->flag.toggle_instrn_class;
        }
        return(UIM_INSTRN_CHANGE);
      }
      else if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC)
      {
       /* If there was a read error for ICCID
        * and the class is already in GSM return
        * error
        */
        return(UIM_CMD_ERR);
      }
    }
    else
    {
      return(UIM_CMD_ERR);
    }
  } /* end if - there was a problem with the APDU */

  /* Process the ICCID data according to the command received. */
  if ( (uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_ME_PUP_F) ||
       (uim_ptr->command.cmd_ptr->hdr.command == UIM_HOTSWAP_CARD_INS_F)||
       (uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_F)
     )
  {
    /* Store the ICCID */
    for (i = 0; i < uim_ptr->cache.iccid_buf.num_bytes; ++i)
    {
      uim_ptr->cache.iccid_buf.data[i] = rsp_ptr->rsp.data[i];
    } /* end for - store the ICCID */

    /* Do not execute wrokarounds when the GCF card is used */
    if(TRUE == uim_is_test_iccid(uim_ptr))
    {
      uim_ptr->setting.work_arounds_for_non_spec_cards = 0;
    }
    /* Get the next state. */
    ++uim_ptr->command.generic_state_ptr;

  }
  else if (uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_WAKE_UP_F)
  {
    /* Compare the ICCID with the stored ICCID */
    for (i = 0; i < uim_ptr->cache.iccid_buf.num_bytes; ++i)
    {
      if (uim_ptr->cache.iccid_buf.data[i] != rsp_ptr->rsp.data[i])
      {
        /* Indicate an ICCID mismatch */
        UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_126,uim_ptr->id,"ICCID MISMATCH");
        uim_clear_recovery_params(uim_ptr);
        return (UIM_ICCID_MISMATCH);
      }
    } /* end for - compare the ICCID */
    /* Move to recovery states */
    if(MMGSDI_SUCCESS ==
        uim_interface_mmgsdi_get_recovery_data_sync(&uim_ptr->state.recovery_data,
                                      (uim_slot_type)(uim_ptr->id + 1)))
    {

      UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_127, uim_ptr->id,"Recovering with protocol 0x%x", uim_ptr->command.cmd_ptr->hdr.protocol);
      uim_ptr->command.cmd_ptr->hdr.protocol = uim_ptr->state.recovery_data.protocol;
      if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
      {
        /* Proceed to recover UICC card */
        uim_ptr->command.generic_state_ptr = UIM_RECOVER_UICC_SLOT_STATES;
      }
      else
      {
       /* Proceed to recover ICC card */
         uim_ptr->command.generic_state_ptr = UIM_RECOVER_ICC_SLOT_STATES;
      }
    }
    else
    {
      /* In case if fails to recover the data from MMGSDI during recovery
         proceed to final state (UIM_DONE_ST) to notify MMGSDI
         of recovery complete */
      uim_ptr->mode.generic_states[0]    = UIM_DONE_ST;
      uim_ptr->command.generic_state_ptr = uim_ptr->mode.generic_states; 
    }
  }
  else
  {
    /* Something is wrong.  We should not be in this state
       while processing this command. */
    UIMDRV_MSG_LOW_0(uim_ptr->id,"UIM_READ_ICCID_ST outside of PUP or WAKE_UP");
    return (UIM_CMD_ERR);
  } /* end if - command is a power-up command. */
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_read_iccid_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_CHECK_FOR_CDMA_DF_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_check_for_cdma_df_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Check if there was a problem with selecting the CDMA DF */
  if (rsp_ptr->cmd_status == UIM_DONE)
  {
    /* update the path directly instead of calling uim_update_current_dir_icc
       use current_path[0] (default channel) for ICC */
    uim_ptr->state.current_path[0].path.path[0] = 0x3F00;
    uim_ptr->state.current_path[0].path.path[1] = uim_ptr->command.cmd_ptr->select.path.path[0];
    uim_ptr->state.current_path[0].is_ef        = FALSE;
    uim_ptr->state.current_path[0].path.len     = 2;

    /* uim_ptr->command.cmd_ptr->select.path.path[0] is set in the beginning of this state
       according to MMGSDI_FEATURE_CDMA_DF_SELECT_BEFORE_TP if it is true path[0] would be
       GSM_DF if it is false path[0] would be CDMA_DF */
    if (uim_ptr->flag.uim_auth_cdma_df_first == FALSE)
    {
      uim_ptr->state.dfs_1.cdma_df_present = TRUE;
    }
    else
    {
      uim_ptr->state.dfs_1.gsm_df_present = TRUE;
    }

    /* Due to some out of spec, commercialized RUIM cards we have to
       extend our work waiting timer value */
    uim_ptr->card_cmd.work_waiting_time =
      uim_ptr->card_cmd.extended_work_waiting_time;
    uim_ptr->card_cmd.work_waiting_time_etus =
      uim_ptr->card_cmd.extended_work_waiting_time_etus;
  } /* end if - the CDMA DF was selected */
   /*clearing of EF is required as some cards are clearing their EF pointer after the
     CDMA DF failure*/
  else if (rsp_ptr->cmd_status != UIM_DONE &&
           uim_ptr->command.rpt_buf.sw1 == SW1_REFERENCE &&
           uim_ptr->command.rpt_buf.sw2 == SW2_NOT_FOUND
          )
  {
    UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_123,uim_ptr->id,"Select CDMA failed Command status: %x",
                      rsp_ptr->cmd_status);
  }
  /* Go to the next state.The current path points to previous selected path. */
  /* 3F00 2FE2 is the last selected file at this point during power-up or recovery */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_check_for_cdma_df_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_CHECK_CHARACTERISTICS_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_check_characteristics_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Does the card support the terminal capability command */
  uint8 uim_send_terminal_capability = 0x00;

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
   #error code not present
#endif

  if((UIM_PROBLEM == rsp_ptr->cmd_status) &&
     (FALSE == uim_ptr->flag.reattempted_select_mf_upon_tech_prob))
  {
    /* Do not increment state pointer incase 6F 00 is received,
       select MF will be attempted again */
    uim_ptr->flag.reattempted_select_mf_upon_tech_prob = TRUE;
    return UIM_CMD_SUCCESS;
  }
  else
  {
    uim_ptr->flag.reattempted_select_mf_upon_tech_prob = FALSE;
  }

  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_RECOVERY_ON_BAD_STATUS_WORD_ON_MF_SELECT, uim_ptr) &&
     uim_ptr->flag.bad_status_words_error                                                        &&
     uim_ptr->command.static_cmd_buf.hdr.cmd_count < UIM_MAX_NUM_ATTEMPTS)
  {
    /* Trigger recovery in case we receive bad status word
       for Select MF */
    UIMDRV_MSG_ERR_0(uim_ptr->id,"received bad status word. for select MF forcing recovery");
    uim_force_recovery(uim_ptr);
    return UIM_CMD_SUCCESS;
  }

  /* If the class of the command was incorrect, set the flag to change
  the instruction class and return a CMD ERROR */
  if ((rsp_ptr->cmd_status == UIM_WRONG_CLASS) ||
      (rsp_ptr->cmd_status == UIM_UNKNOWN_INS) ||
      (rsp_ptr->cmd_status == UIM_PROBLEM &&
       (uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_BOOLEAN_FLAGS] &
        UIM_RFU_MASK_CHANGE_INST_CLASS_IF_TECH_PROB)
      ) ||
      (rsp_ptr->cmd_status == UIM_INCORRECT_PARAM_P3) ||
      (rsp_ptr->cmd_status == UIM_INCORRECT_PARAM_P1_2) ||
      (rsp_ptr->cmd_status == UIM_INCORRECT_PARAMS_P1_P2) ||
      (rsp_ptr->cmd_status == UIM_INCORRECT_PARAM_IN_DATA_FIELD) ||
      ((rsp_ptr->cmd_status == UIM_DONE) &&
       (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC) &&
       (rsp_ptr->rsp.data[0] != UIM_FCP_TEMPLATE_TAG))  ||
       /* It's possible this was a failure on the MF it's
       unlikely but we handle it by switching to ICC.
       As some buggy cards can work well with ICC protocol*/
      ((rsp_ptr->cmd_status != UIM_DONE) &&
       (uim_ptr->command.cmd_ptr->select.path.path[0] == UIM_MF_ID))
     )
  {
    /* Toggle the instruction class if not in recovery mode */
    if(!uim_ptr->flag.recovery_triggered)
    {
      uim_ptr->flag.toggle_instrn_class = !uim_ptr->flag.toggle_instrn_class;
    }

    if (++uim_ptr->card_cmd.instrn_toggle_count > UIM_MAX_CLA_TOGGLE_ATTEMPTS)
    {
      uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
      return(UIM_CMD_ERR);
    }
    else
    {
       uim_ptr->flag.bad_status_words_error = FALSE;
       return(UIM_INSTRN_CHANGE);
    }
  }
  else
  /* Check if there was a problem with the select */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    /* Check to see if there are more directories to attempt.  If there are stay in the
       check_characteristics_st which will try to select the next DF */
    if(0x0000 == uim_check_characteristics_get_next_df(uim_ptr, uim_ptr->command.cmd_ptr->select.path.path[0]))
    {
      /* Since the SELECT failed, we do not know the preferred clock
         stop mode for this card. But, let us set it to LOW, as most
         cards supports it, to make sure the driver stops the clock
         and votes for sleep. */
      uim_ptr->state.clock_stop_cntrl = UIM_CLOCK_STOP_LOW;
      /* No more DFs to try.  Continue with the power-up */
      /* Skip EF-UMPC and terminal capability state and continue to select and read the ICCID */
      uim_ptr->command.generic_state_ptr+=3;
    } /* end if - no more directories to select */
  }
  else /* The select was success.  Check the characteristics. */
  {
    /* Initialize the file characteristics byte */
    uim_ptr->state.file_char_byte = 0;

    if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC)
    {
      /* update the path directly instead of calling uim_update_current_dir_icc
         use current_path[0] for default channel for ICC */
      uim_ptr->state.current_path[0].path.path[0] = 0x3F00;
      uim_ptr->state.current_path[0].path.path[1] = uim_ptr->command.cmd_ptr->select.path.path[0];
      uim_ptr->state.current_path[0].is_ef        = FALSE;
      uim_ptr->state.current_path[0].path.len     = 2;

      if (uim_ptr->command.cmd_ptr->select.path.path[0] == UIM_GSM_DF)
      {
        uim_ptr->state.dfs_1.gsm_df_present = TRUE;
      }
      else if(uim_ptr->command.cmd_ptr->select.path.path[0]== UIM_CDMA_DF)
      {
        uim_ptr->state.dfs_1.cdma_df_present = TRUE;
      }

      /*Extract the file characteristic byte from the response */
      uim_ptr->state.file_char_byte = rsp_ptr->rsp.data[UIM_FILE_CHAR_INDEX];

      if(!IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
      {
        if(((uim_ptr->state.file_char_byte & UIM_1_8_SIM_GSM)
          &&(uim_ptr->state.file_char_byte & UIM_3V_SIM_GSM)))
        {
          uim_ptr->flag.voltage_class_c_and_b_supported = TRUE;
        }

        /* Determine the current voltage on the interface */
        switch (uim_ptr->state.current_voltage_class)
        {
          case UIM_VOLTAGE_CLASS_C:
            {
              /* Check if the card does not support this voltage */
              if ((uim_ptr->state.file_char_byte & UIM_1_8_SIM_GSM) !=
                  UIM_1_8_SIM_GSM)
              {
                /* Check if card supports other supported voltages */
                if ((uim_ptr->state.file_char_byte & UIM_3V_SIM_GSM) ==
                    UIM_3V_SIM_GSM)
                {
                  if (uim_ptr->hardware.uim_ldo.customize_ldo_setting &&
                      uim_ptr->hardware.uim_ldo.desired_voltage_class == UIM_VOLTAGE_CLASS_C)
                  {
                    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_121,uim_ptr->id,"LDO behaviour modified to work at specific voltage class");
                    return(UIM_VOLTAGE_MISMATCH);
                  }
                  /* Switch to different voltage */
                  uim_power_down(uim_ptr);
                  uim_timed_sleep(UIM_ISO7816_VOLTAGE_SWITCH_PWR_DOWN_DELAY,
                                  uim_ptr,
                                  UIM_ALL_ZERO_SIG);
                  /* Try the indicated voltage class */
                  uim_ptr->state.current_voltage_class = UIM_VOLTAGE_CLASS_B;
                  return(UIM_SWITCH_VOLTAGE);
                }
                else if (!(uim_ptr->setting.work_arounds_for_non_spec_cards & UIM_IGNORE_VOLTAGE_CLASS_ERROR_IN_MF_SELECTION))
                {
                  /* No voltages supported */
                  UIMDRV_MSG_ERR_1(uim_ptr->id, "No voltage class supported: 0x%x", uim_ptr->state.file_char_byte);
                  return(UIM_VOLTAGE_MISMATCH);
                } /* end if - card supports other voltage */
              } /* end if - card does not support this voltage */
            } /* end case - UIM_VOLTAGE_1_8V */
            break;

          case UIM_VOLTAGE_CLASS_B:
            {
              /* Check if the card does not support this voltage */
              if (((uim_ptr->state.file_char_byte & UIM_3V_SIM_GSM) != UIM_3V_SIM_GSM) && 
                    !(uim_ptr->setting.work_arounds_for_non_spec_cards & UIM_IGNORE_VOLTAGE_CLASS_ERROR_IN_MF_SELECTION))
              {
                UIMDRV_MSG_ERR_1(uim_ptr->id, "No voltage class supported: 0x%x", uim_ptr->state.file_char_byte);
                return(UIM_VOLTAGE_MISMATCH);
              } /* end if - card does not support this voltage */
            } /* end case - UIM_VOLTAGE_3V*/
            break;

          default:
            {
              UIMDRV_MSG_ERR_0(uim_ptr->id, "Bad UIM voltage class" );
              return(UIM_VOLTAGE_MISMATCH);
            } /* end case - default */
        } /* end switch - voltage class */
      }

      /* Skip EF-UMPC and terminal capability state for ICC cards */
      uim_ptr->command.generic_state_ptr+=2;
    }/* protocol is GSM */
    else if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
    {
      /* this is a response for select of MF */
      uim_ptr->state.current_path[uim_ptr->card_cmd.cur_channel].path = uim_ptr->command.cmd_ptr->select.path;

      /* call the function to parse the response from the UICC
         select for an MF select */
      uim_parse_uicc_select_response(
        uim_ptr,
        rsp_ptr->rsp.data,
        rsp_ptr->cmd_rsp_size,
        NULL,
        &uim_ptr->state.file_char_byte,
        &uim_send_terminal_capability,
        NULL);

      if(!(uim_send_terminal_capability & UIM_TERMINAL_CAP_BYTE_MASK))
      {
        /* Skip EF-UMPC and terminal capability */
        uim_ptr->command.generic_state_ptr+=2;
      }

      if(!IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
      {
        if(((uim_ptr->state.file_char_byte & UIM_1_8_UIM_UICC)
          &&(uim_ptr->state.file_char_byte & UIM_3V_UIM_UICC)))
        {
          uim_ptr->flag.voltage_class_c_and_b_supported = TRUE;
        }
        /* Determine the current voltage on the interface */
        switch (uim_ptr->state.current_voltage_class)
        {
          case UIM_VOLTAGE_CLASS_C:
            {
              /* Check if the card does not support this voltage */
              if ((uim_ptr->state.file_char_byte & UIM_1_8_UIM_UICC) !=
                  UIM_1_8_UIM_UICC)
              {
                /* Check if card supports other supported voltages */
                if ((uim_ptr->state.file_char_byte & UIM_3V_UIM_UICC) ==
                    UIM_3V_UIM_UICC)
                {
                  if (uim_ptr->hardware.uim_ldo.customize_ldo_setting &&
                      uim_ptr->hardware.uim_ldo.desired_voltage_class == UIM_VOLTAGE_CLASS_C)
                  {
                    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_122,uim_ptr->id,"LDO behaviour modified to work at specific voltage class");
                    return(UIM_VOLTAGE_MISMATCH);
                  }
                  /* Switch to different voltage */
                  uim_power_down(uim_ptr);
                  uim_timed_sleep(UIM_ISO7816_VOLTAGE_SWITCH_PWR_DOWN_DELAY,
                                  uim_ptr,
                                  UIM_ALL_ZERO_SIG);
                  /* Try the indicated voltage class */
                  uim_ptr->state.current_voltage_class = UIM_VOLTAGE_CLASS_B;
                  return(UIM_SWITCH_VOLTAGE);
                }
                else if (!(uim_ptr->setting.work_arounds_for_non_spec_cards & UIM_IGNORE_VOLTAGE_CLASS_ERROR_IN_MF_SELECTION)) 
                {
                  /* No voltages supported */
                  UIMDRV_MSG_ERR_1(uim_ptr->id, "No voltage class supported: 0x%x", uim_ptr->state.file_char_byte);
                  return(UIM_VOLTAGE_MISMATCH);
                } /* end if - card supports other voltage */
              } /* end if - card does not support this voltage */
            } /* end case - UIM_VOLTAGE_1_8V */
            break;

          case UIM_VOLTAGE_CLASS_B:
            {
              /* Check if the card does not support this voltage */
              if (((uim_ptr->state.file_char_byte & UIM_3V_UIM_UICC) != UIM_3V_UIM_UICC) && 
                    !(uim_ptr->setting.work_arounds_for_non_spec_cards & UIM_IGNORE_VOLTAGE_CLASS_ERROR_IN_MF_SELECTION))
              {
                UIMDRV_MSG_ERR_1(uim_ptr->id, "No voltage class supported: 0x%x", uim_ptr->state.file_char_byte);
                return(UIM_VOLTAGE_MISMATCH);
              } /* end if - card does not support this voltage */
            } /* end case - UIM_VOLTAGE_3V*/
            break;

          default:
            {
              UIMDRV_MSG_ERR_0(uim_ptr->id, "Bad UIM voltage class" );
              return(UIM_VOLTAGE_MISMATCH);
            } /* end case - default */
        } /* end switch - voltage class */
      }/* !IS_BTSAP_HANDLE_ACTIVE(uim_ptr)  */
    } /* protocol is UICC */
    /* Check if the UIM clock can be stopped */
    if ((uim_ptr->state.file_char_byte & UIM_CLOCK_STOP_ALL_BITS) == 0)
    {
      /* Do not allow the UIM clock to be stopped */
      uim_ptr->state.clock_stop_cntrl = UIM_CLOCK_STOP_NOT_ALLOWED;
    }
    else /* the card supports stopping the UIM clock */
    {
      /* Determine the recommended level to stop the clock */
      if (uim_ptr->state.file_char_byte & UIM_CLOCK_STOP_B3)
      {
        /* Clock must be stopped at high level */
        uim_ptr->state.clock_stop_cntrl = UIM_CLOCK_STOP_HIGH;
      }
      else
      {
        /* Clock must be stopped at low level */
        uim_ptr->state.clock_stop_cntrl = UIM_CLOCK_STOP_LOW;
      } /* end if - determine clock stop level */
    } /* end if - check UIM clock stop. */

    /* A DF was processed.  Continue with the power-up */
    ++uim_ptr->command.generic_state_ptr;

  } /* end if - there was a problem with the APDU */
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_check_characteristics_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_ICCID_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_select_iccid_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_SUPPORT_NO_ICCID, uim_ptr) &&
     (rsp_ptr->cmd_status != UIM_DONE))
  {
    if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
    {
     uim_ptr->flag.iccid_select_failed = TRUE;
     rsp_ptr->cmd_status = UIM_DONE;
    }
    else if((uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC) &&
            (uim_ptr->command.cmd_ptr->select.path_position == 1))
    {
      /* For an ICC we set the select path to 3F00/2FE2.  If we get a response
         and the path position is 1 then we know we failed selecting 2FE2(ICCID) */
      uim_ptr->flag.iccid_select_failed = TRUE;
      rsp_ptr->cmd_status               = UIM_DONE;
      }
  } /* support no iccid*/

  if(UIM_ICC == uim_ptr->command.cmd_ptr->hdr.protocol  &&
     0 == uim_ptr->command.cmd_ptr->select.path_position &&
     UIM_DONE == rsp_ptr->cmd_status)
  {
    /* For ICC we select incrementally.  If the path position is 0
       this is the response for MF increment path position and
       select again */
    uim_ptr->command.cmd_ptr->select.path_position++;
    return UIM_CMD_SUCCESS;
  }
  uim_ptr->cache.iccid_buf.num_bytes = (byte)uim_util_get_ef_file_size (uim_ptr,rsp_ptr);

  /* Limit the ICCID to 10bytes even if card (non-spec complaint) has more bytes */
  if(uim_ptr->cache.iccid_buf.num_bytes > UIM_ICCID_SIZE)
  {
    uim_ptr->cache.iccid_buf.num_bytes = UIM_ICCID_SIZE;
  }

  uim_ptr->state.current_path[0].path.path[0] = UIM_MF_ID;
  uim_ptr->state.current_path[0].path.path[1] = UIM_ICCID_EF_ID;
  uim_ptr->state.current_path[0].is_ef        = TRUE;
  uim_ptr->state.current_path[0].path.len     = 2;
  ++uim_ptr->command.generic_state_ptr;

  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_select_iccid_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_EF_DIR_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_select_ef_dir_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  if(rsp_ptr->cmd_status != UIM_DONE)
  {
    ++uim_ptr->command.generic_state_ptr;
    return UIM_CMD_SUCCESS;  
  }

  uim_parse_uicc_select_response(uim_ptr,
                                 &rsp_ptr->rsp.data[0],
                                 rsp_ptr->cmd_rsp_size,
                                 NULL,
                                 NULL,
                                 NULL,
                                 &(uim_ptr->cache.num_apps));

  uim_ptr->state.current_path[0].path.path[0] = UIM_MF_ID;
  uim_ptr->state.current_path[0].path.path[1] = UIM_DIR_EF_ID;
  uim_ptr->state.current_path[0].is_ef        = TRUE;
  uim_ptr->state.current_path[0].path.len     = 2;
  ++uim_ptr->command.generic_state_ptr;

  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_select_ef_dir_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_EF_DIR_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_read_ef_dir_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  if(rsp_ptr->cmd_status != UIM_DONE)
  {
    ++uim_ptr->command.generic_state_ptr;
    return UIM_CMD_SUCCESS;  
  }

  if(uim_util_is_aid_valid(rsp_ptr))
  {
    uim_ptr->cache.num_apps.num_apps_valid++;
  }

  if(uim_ptr->cache.num_apps.num_apps_present == uim_ptr->cache.num_apps.num_apps_read)
  {
    ++uim_ptr->command.generic_state_ptr;
  }

  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_read_ef_dir_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_UMPC_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_read_umpc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  uint32 terminal_max_power_capability = 0;

  if((rsp_ptr->cmd_status != UIM_DONE))
  {
    uim_ptr->flag.is_rel_12_uicc = FALSE;
    memset(&uim_ptr->cache.ef_umpc_data, 0x00, sizeof (uim_ef_umpc_data_type));
    rsp_ptr->cmd_status = UIM_DONE;
  }
  else
  {
    uim_ptr->flag.is_rel_12_uicc = TRUE;
    uim_ptr->cache.ef_umpc_data.uicc_max_power_consumption      = rsp_ptr->rsp.data[0];
    uim_ptr->cache.ef_umpc_data.operator_defined_timeout        = rsp_ptr->rsp.data[1];
    uim_ptr->cache.ef_umpc_data.increased_idle_current_required = rsp_ptr->rsp.data[2];
    UIMDRV_MSG_MED_3(uim_ptr->id, "ef_umpc - umpc: 0x%x, t_op: 0x%x, idle_current: 0x%x",
                  uim_ptr->cache.ef_umpc_data.uicc_max_power_consumption,
                  uim_ptr->cache.ef_umpc_data.operator_defined_timeout,
                  uim_ptr->cache.ef_umpc_data.increased_idle_current_required);

    uim_ptr->state.current_path[0].path.path[0] = UIM_MF_ID;
    uim_ptr->state.current_path[0].path.path[1] = UIM_UMPC_EF_ID;
    uim_ptr->state.current_path[0].is_ef        = TRUE;
    uim_ptr->state.current_path[0].path.len     = 2;

    if (uim_ptr->state.current_voltage_class == UIM_VOLTAGE_CLASS_C)
    {
      terminal_max_power_capability = TERMINAL_MAX_POWER_CAPABILITY_CLASS_C;
    }
    else
    {
      terminal_max_power_capability = TERMINAL_MAX_POWER_CAPABILITY_CLASS_B;
    }
    /*if EF-UMPC power is greater than TERMINAL_MAX_POWER_CAPABILITY
      modify transaction timer to be atleast t_op (from UMPC) */
    if(uim_ptr->cache.ef_umpc_data.uicc_max_power_consumption > terminal_max_power_capability)
    {
      /*increase trans timer to alteast t_op*/
      if((1000*uim_ptr->cache.ef_umpc_data.operator_defined_timeout) >
         uim_ptr->setting.uim_feature_busy_response.uim_trans_timer_val_ms)
      {
        uim_ptr->setting.uim_feature_busy_response.uim_trans_timer_val_ms =
              (1000*uim_ptr->cache.ef_umpc_data.operator_defined_timeout);
      }
    }
  }
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_read_umpc_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_FAILED_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_select_failed_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  if(rsp_ptr->cmd_status == UIM_DONE)
  {
    if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC)
    {
      uim_ptr->command.cmd_ptr->select.path_position++;

      /* Select the next file ID by incrementing the sel state counter */
      if(uim_ptr->command.cmd_ptr->select.path_position < uim_ptr->command.cmd_ptr->select.path.len)
      {
        uim_update_current_dir_icc(uim_ptr,
                                 FALSE,
                                &uim_ptr->command.cmd_ptr->select.path,
                                 uim_ptr->command.cmd_ptr->select.path_position);
        return UIM_CMD_SUCCESS;
      }
      else if(uim_ptr->command.cmd_ptr->select.path_position == uim_ptr->command.cmd_ptr->select.path.len)
      {
        boolean temp_ef = rsp_ptr->rsp.ef.file_type == EF ? TRUE : FALSE;

#ifdef FEATURE_UIM_TEST_FRAMEWORK
      #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

        uim_update_current_dir_icc(uim_ptr,
                                 temp_ef,
                                &uim_ptr->command.cmd_ptr->select.path,
                                 uim_ptr->command.cmd_ptr->select.path_position);
      }
    }
    else if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
    {
      boolean temp_ef = FALSE;
      uim_parse_uicc_select_response(
        uim_ptr,
       &rsp_ptr->rsp.data[0],
        rsp_ptr->cmd_rsp_size,
       &temp_ef,
        NULL,
        NULL,
        NULL);

      uim_update_current_dir_uicc(uim_ptr, temp_ef, &uim_ptr->command.cmd_ptr->select, curr_ch);
    }
  }
  else
  {
    /* We failed the last selected dir select attempt. Clear the current
       path and report - Something is very wrong with the card*/
    uim_clear_current_path(uim_ptr, curr_ch);
  }

  uim_ptr->command.rpt_buf.sw1 = uim_ptr->command.cmd_ptr->select.reselect.sw1;
  uim_ptr->command.rpt_buf.sw2 = uim_ptr->command.cmd_ptr->select.reselect.sw2;
  rsp_ptr->cmd_status          = uim_ptr->command.cmd_ptr->select.reselect.cmd_status;
  uim_ptr->command.cmd_ptr->select.reselect.is_failed_select_reselect = FALSE;
  uim_ptr->command.cmd_ptr->select.is_intermediate_sel = FALSE;

  uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
  if(*completed == UIM_CMD_FETCH)
  {
    uim_ptr->flag.fetch_pending = TRUE;
  }
  return UIM_CMD_ERR;
} /* uimgen_iso_handle_select_failed_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_STREAM_SELECT_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_stream_select_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  uint8 dir_level     = 0;
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* this state is only entered when streaming selects on the default channel */
  byte *apdu_data_ptr = uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data;
  byte  apdu_p1       = apdu_data_ptr[UIM_7816_APDU_P1_OFFSET];
  uint8 cmd_path_len  = apdu_data_ptr[UIM_CASE3_4_7816_LC_OFFSET];
  uint8 i             = 0;

  uim_current_path_type *default_ch_curr_path_ptr = &uim_ptr->state.current_path[UIM_DEFAULT_CHANNEL];

  /* cmd_path_len is in bytes - each path entry is two bytes so divide by two */
  cmd_path_len /= 2;

  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    uim_prepare_select_failed_st(uim_ptr, 0, rsp_ptr->cmd_status);

    uim_ptr->mode.generic_states[0]    = UIM_SELECT_FAILED_ST;
    uim_ptr->mode.generic_states[1]    = UIM_DONE_ST;
    uim_ptr->command.generic_state_ptr = uim_ptr->mode.generic_states;
    return UIM_CMD_SUCCESS;
  } /* end of error handling */


  if ( ((UIM_ICC  == uim_ptr->state.dfs_1.protocol) ||
        (UIM_UICC == uim_ptr->state.dfs_1.protocol)) &&
       ((SW1_NORMAL_END    != rsp_ptr->sw1) &&
        (SW1_END_FETCH     != rsp_ptr->sw1) &&
        (SW1_END_RESP      != rsp_ptr->sw1) &&
        (SW1_UTIL_END_RESP !=rsp_ptr->sw1))
     )
  {
    /* Do not update the current path as Streaming command for (U)ICC is not executed successfully
     * If current PATH is updated then subsequent POLL would fail
     */
    uim_stream_apdu_success(uim_ptr,
                            uim_req_buf_static_ptr,
                            rsp_ptr);
    UIMDRV_MSG_HIGH_3(uim_ptr->id,
               "STREAMING Protocol %d SW1 %d SW2 %d",
                uim_ptr->state.dfs_1.protocol,
                rsp_ptr->sw1,
                rsp_ptr->sw2);
    return(UIM_CMD_SUCCESS);
  }
  if ( apdu_p1 & UIM_USIM_SELECT_BY_DF_NAME )
  {
    default_ch_curr_path_ptr->path.path[0] = 0x3F00;
    default_ch_curr_path_ptr->path.path[1] = 0x7FFF;
    default_ch_curr_path_ptr->path.len     = 2;
    default_ch_curr_path_ptr->is_ef        = FALSE;
  }
  else if(apdu_p1 & UIM_USIM_SELECT_BY_PATH_FROM_MF &&
          cmd_path_len < UIM_DIRMAIN_MAX_PATH_ELEMENTS)
  {
    default_ch_curr_path_ptr->path.path[0] = 0x3F00;
    default_ch_curr_path_ptr->path.len     = 1;

    /* set the data pointer equal to the path */
    apdu_data_ptr = &apdu_data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 1];
    for(i = 1; i <= cmd_path_len; i++)
    {
      default_ch_curr_path_ptr->path.path[i] = (apdu_data_ptr[0] << 8) | apdu_data_ptr[1];
      apdu_data_ptr += 2;
      default_ch_curr_path_ptr->path.len++;
    }
    /* The type of selected file is unknown at this point.
       We need to get file type returned from card by GET RESPONSE*/
  }
  else if(apdu_p1 == UIM_USIM_SELECT_BY_FILE_ID &&
          default_ch_curr_path_ptr->path.len < UIM_DIRMAIN_MAX_PATH_ELEMENTS)
  {
    if(((apdu_data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 1] << 8) |
          apdu_data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 2]) == 0x3F00)
    {
      default_ch_curr_path_ptr->path.len     = 1;
      default_ch_curr_path_ptr->path.path[0] = 0x3F00;
      default_ch_curr_path_ptr->is_ef        = FALSE;
    }
    else if(default_ch_curr_path_ptr->path.len != 0)
    {
      /* The command was successful. The apdu data has the file id selected as the
         first two command data bytes.  Take them from this apdu and store them */
      dir_level = (default_ch_curr_path_ptr->path.path[default_ch_curr_path_ptr->path.len - 1] & UIM_FILE_ID_LEVEL_MASK) >> UIM_FILE_ID_LEVEL_SHIFT;
      if((dir_level == UIM_EF_UNDER_MF) || (dir_level == UIM_EF_UNDER_2ND_DF) || (dir_level == UIM_EF_UNDER_1ST_DF))
      {
        default_ch_curr_path_ptr->path.path[default_ch_curr_path_ptr->path.len - 1] =
          (apdu_data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 1] << 8) | apdu_data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 2];
				
        default_ch_curr_path_ptr->is_ef = TRUE;
      }
      else
      {
        default_ch_curr_path_ptr->path.path[default_ch_curr_path_ptr->path.len] =
          (apdu_data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 1] << 8) | apdu_data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 2];
 
        default_ch_curr_path_ptr->path.len++;
      }
      /* The type of selected file is unknown at this point.
       We need to get file type returned from card by GET RESPONSE*/
    }
  }

  /* this copies apdu response data and increments the generic state ptr
     it's possible that it will build new states for get rsp if needed */
  uim_stream_apdu_success(uim_ptr, uim_req_buf_static_ptr, rsp_ptr);
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_stream_select_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_select_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  /* Did the command execute properly. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    if (uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_WAKE_UP_F)
    {
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_131,uim_ptr->id,"Select failure during Recovery");
      /* If internal wake_up_f, try to recover */
      (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
      /* Go to the next state so that we are not stuck in infinite
         SELECT */
      ++uim_ptr->command.generic_state_ptr;
      return UIM_CMD_SUCCESS;
    }
    else if( (UIM_SELECT_F             == uim_ptr->command.cmd_ptr->hdr.command)          &&
            ((UIM_NEXT_OCCURRENCE      == uim_ptr->command.cmd_ptr->select.adf_selection) ||
             (UIM_LAST_OCCURRENCE      == uim_ptr->command.cmd_ptr->select.adf_selection) ||
             (UIM_PREVIOUS_OCCURRENCE  == uim_ptr->command.cmd_ptr->select.adf_selection)) )
   {
     /* Do not clear the last selected ADF because
        Upon un-successful selection of given partial AID for given channel the
        already selected Applet stays selected.
         Therefore no need to issue MF Selection upon Failure.
       */
       uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_CMD_ATTEMPTS(uim_ptr);
       uim_ptr->command.rpt_buf.sw1 = uim_ptr->command.rsp_buf.sw1;
       uim_ptr->command.rpt_buf.sw2 = uim_ptr->command.rsp_buf.sw2;
       UIMDRV_MSG_HIGH_3(uim_ptr->id,"Select for Next Occurrence Failed and not going to re-attempt"
                    " sw1 0x%x sw2 0x%x cmd_count 0x%x",
                    uim_ptr->command.rpt_buf.sw1,uim_ptr->command.rpt_buf.sw2,
                    uim_ptr->command.cmd_ptr->hdr.cmd_count);
       return (UIM_CMD_ERR);
   }
   else if((uim_ptr->card_cmd.get_resp_sw1 == SW1_WARNINGS1) ||
	   (uim_ptr->card_cmd.get_resp_sw1 == SW1_WARNINGS2))
   {
     uim_ptr->card_cmd.resp_buf_ptr->cmd_status = UIM_DONE;
     uim_ptr->command.rpt_buf.sw1 = uim_ptr->card_cmd.get_resp_sw1;
     uim_ptr->command.rpt_buf.sw2 = uim_ptr->card_cmd.get_resp_sw2;
     ++uim_ptr->command.generic_state_ptr;
     return UIM_CMD_SUCCESS;
   }
   /* Do not send select MF on failure for non-telecom app */
   else if((FALSE == uim_is_telecom_aid(uim_ptr->state.current_path[curr_ch].curr_app)) &&
	   (uim_ptr->state.current_path[curr_ch].curr_app.aid_length > 0))
   {
     ++uim_ptr->command.generic_state_ptr;
     return UIM_CMD_SUCCESS;
   }

    uim_prepare_select_failed_st(uim_ptr, curr_ch, rsp_ptr->cmd_status);

    uim_ptr->mode.generic_states[0]    = UIM_SELECT_FAILED_ST;
    uim_ptr->mode.generic_states[1]    = UIM_DONE_ST;
    uim_ptr->command.generic_state_ptr = uim_ptr->mode.generic_states;
    return UIM_CMD_SUCCESS;
  } /* end of error handling */


  if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    boolean temp_ef = FALSE;

    uim_parse_uicc_select_response(
      uim_ptr,
     &rsp_ptr->rsp.data[0],
      rsp_ptr->cmd_rsp_size,
     &temp_ef,
      NULL,
      NULL,
      NULL);

    uim_update_current_dir_uicc(uim_ptr, temp_ef, &uim_ptr->command.cmd_ptr->select, curr_ch);

    /* Setting the default behavior for non_telecom applications */
    if(uim_ptr->command.cmd_ptr->select.file_type == UIM_ADF)
    {
      if(uim_is_telecom_aid(uim_ptr->command.cmd_ptr->select.aid) == FALSE)
      {
        uim_ptr->state.ch_apdu_behavior[uim_ptr->command.cmd_ptr->hdr.channel] =
          UIM_APDU_BEHAVIOR_NO_GET_RESPONSE_FOR_WARNING;

        /* Update status word with warning status word for non-telecom cards */
        if((uim_ptr->card_cmd.get_resp_sw1 == SW1_WARNINGS1) ||
           (uim_ptr->card_cmd.get_resp_sw1 == SW1_WARNINGS2))
        {
          uim_ptr->command.rpt_buf.sw1 = uim_ptr->card_cmd.get_resp_sw1;
          uim_ptr->command.rpt_buf.sw2 = uim_ptr->card_cmd.get_resp_sw2;
        }
      }
      else
      {
        uim_ptr->state.ch_apdu_behavior[uim_ptr->command.cmd_ptr->hdr.channel] =
          UIM_APDU_BEHAVIOR_FOR_TELECOM_APP;
      }
    }

    ++uim_ptr->command.generic_state_ptr;
  }
  else if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC)
  {
    uim_ptr->command.cmd_ptr->select.path_position++;

    /* Select the next file ID by incrementing the sel state counter */
    if(uim_ptr->command.cmd_ptr->select.path_position < uim_ptr->command.cmd_ptr->select.path.len)
    {
      uim_update_current_dir_icc(uim_ptr,
                                 FALSE,
                                &uim_ptr->command.cmd_ptr->select.path,
                                 uim_ptr->command.cmd_ptr->select.path_position);
    }
    else
    {
      boolean temp_ef = rsp_ptr->rsp.ef.file_type == EF ? TRUE : FALSE;

#ifdef FEATURE_UIM_TEST_FRAMEWORK
      #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

      UIMDRV_PUP_MSG_ERR_2(UIMLOG_MSG_132,uim_ptr->id, "updating current directory icc - temp_ef:0x%x - file type:0x%x",temp_ef,rsp_ptr->rsp.ef.file_type);

      uim_update_current_dir_icc(uim_ptr,
                                 temp_ef,
                                &uim_ptr->command.cmd_ptr->select.path,
                                 uim_ptr->command.cmd_ptr->select.path_position);

        /* The file selection is finished.  Get to the next state. */
      ++uim_ptr->command.generic_state_ptr;
    }
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_select_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RESELECT_CURR_DIR_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_reselect_curr_dir_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  /* Did the command execute properly. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
     /* We failed the last selected dir select attempt. Clear the current
           path and report - Something is very wrong with the card*/
    uim_clear_current_path(uim_ptr, curr_ch);

    uim_ptr->state.mf_selection.required = TRUE;
    uim_ptr->state.mf_selection.channel  = 0;

    uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
    return (UIM_CMD_ERR);
  }
  else
  {
    /* The file selection is finished.  Get to the next state. */
    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_reselect_curr_dir_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_TERMINAL_CAPABILITY_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_terminal_capability_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
   /* We sent the terminal capability and printed
     the card response. Go to the next state */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_terminal_capability_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SEND_STATUS_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_send_status_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_send_status_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_STATUS_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_status_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  uim_aid_type status_adf     = {{0}};
  uim_dir_type status_file_id = 0;
  uint16       current_dir    = 0x0000;
  int          idx            = 0;

  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  if(uim_ptr->poll.flaky_polling_state == UIM_FLAKY_POLLING_ATTEMPT_2)
  {
    uim_ptr->setting.poll_time_delay  = uim_ptr->poll.orig_poll_time;
    uim_ptr->poll.flaky_polling_state = UIM_FLAKY_POLLING_OFF;
  }

  /* Response to status is the same as Select MF/DF */
  /* Determine which command got us here. */
  if (uim_ptr->command.cmd_ptr->hdr.command == UIM_INTERNAL_POLL_F)
  {
    /* Send second poll command irrespective of command status */
    if(uim_ptr->poll.flaky_polling_state == UIM_FLAKY_POLLING_ATTEMPT_1)
    {
      uim_ptr->poll.orig_poll_time = uim_ptr->setting.poll_time_delay;
      uim_set_poll_timer(uim_ptr->poll.poll_time_for_flaky,
                         uim_ptr->setting.wakeup_poll_offset,
                         uim_ptr);
      uim_ptr->poll.flaky_polling_state = UIM_FLAKY_POLLING_ATTEMPT_2;
      ++uim_ptr->command.generic_state_ptr;
      return UIM_CMD_SUCCESS;
    }

    if (rsp_ptr->cmd_status != UIM_DONE)
    {
      if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_RPT_BAD_SW_ON_POLL,
                                   uim_ptr) == TRUE)
      {
        if ( uim_ptr->command.mode != UIM_PASSIVE_MODE )
        {
          UIMDRV_MSG_ERR_3(uim_ptr->id, "Un-successful polling - SW1, SW2, cmd_status",
                           rsp_ptr->sw1,
                           rsp_ptr->sw2,
                           rsp_ptr->cmd_status);
          return(UIM_POLL_ERROR);
        }
      }
      else
      {
        UIMDRV_MSG_ERR_1(uim_ptr->id, "Polling failed while uim_mode is 0x%x, ignoring",
                         uim_ptr->command.mode);
        /* Internal poll command failed for some odd reason
         * let us continue instead of comparing data with the
         * stale data in the response pointer
         */
        ++uim_ptr->command.generic_state_ptr;
        return UIM_CMD_SUCCESS;
      }
    }

    if ( uim_ptr->command.mode == UIM_PASSIVE_MODE ||
         uim_ptr->command.cmd_ptr->status.return_data == UIM_RETURN_NONE  || rsp_ptr->cmd_rsp_size == 0)
    {
      /* We are either in in passive mode or not in traffic.  It is
       * enough if we get a response, we need not compare the
       * directory maintainence just process the next state
       */
      ++uim_ptr->command.generic_state_ptr;
      return UIM_CMD_SUCCESS;
    }
    status_adf.aid_length = 0;

    /* get the currently select file id */
    if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
    {
      uim_parse_uicc_status_response ( rsp_ptr->rsp.data,
                                     rsp_ptr->cmd_rsp_size,
                                     &status_file_id,
                                     &status_adf,
                                     uim_ptr);
    }
    else
    {
      status_file_id = FLIPW(rsp_ptr->rsp.mf_df.file_id);
    }

    /* Get the last selected directory to compare
       against the currently selected FID from above*/
    if(uim_ptr->state.current_path[curr_ch].is_ef)
    {
      current_dir = uim_ptr->state.current_path[curr_ch].path.path[uim_ptr->state.current_path[curr_ch].path.len - 2];
    }
    else
    {
      current_dir = uim_ptr->state.current_path[curr_ch].path.path[uim_ptr->state.current_path[curr_ch].path.len - 1];
    }
    
    if (!mcfg_gcf_nv_get_status() && 
        (uim_ptr->setting.work_arounds_for_non_spec_cards
         & UIM_IGNORE_POLL_ERROR) )
    {
      ++uim_ptr->command.generic_state_ptr;
      return(UIM_CMD_SUCCESS);
    }

    /* Compare the two values - the current directory
       and the directory identified by the status*/
    if(current_dir == 0x7FFF)
    {
        if (memcmp(status_adf.data,
                uim_ptr->state.current_path[curr_ch].curr_app.data,
                uim_ptr->state.current_path[curr_ch].curr_app.aid_length) != 0)
        {
          UIMDRV_MSG_HIGH_0(uim_ptr->id, "Mismatch in ADF - UICC protocol");
          return(UIM_POLL_ERROR);
        }
    }
    else if(current_dir != status_file_id)
    {
      /* if the current DF doesn't match the one returned by the
         status it is only ok if it's UICC, the current dir is MF, and
         the status returned the AID currently selected on this channel*/
      if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC &&
         current_dir == UIM_MF_ID  &&
         status_adf.aid_length > 0 &&
              status_adf.aid_length <= UIM_AID_MAX_NAME_LENGTH )
      {
        if (memcmp(status_adf.data,
            uim_ptr->state.current_path[curr_ch].curr_app.data,
            status_adf.aid_length ) != 0)
        {
          UIMDRV_MSG_HIGH_0 ( uim_ptr->id, "Mismatch in AID - UICC protocol");
          /* Log shall be printed */
          for (idx = 0; idx < status_adf.aid_length; idx++)
          {
            UIMDRV_MSG_HIGH_2 ( uim_ptr->id, " status_adf.data[%d] 0x%x",
                                idx, status_adf.data[idx]);
          }
          return(UIM_POLL_ERROR);
        }
        /* if the aid matches we break out here and increment to the next state*/
      }
      else
      {
        UIMDRV_MSG_ERR_1(uim_ptr->id, "Recd file ID in STATUS: 0x%x Current Path:",
                     status_file_id);
        uim_print_curr_path(&uim_ptr->state.current_path[curr_ch].path,uim_ptr);
        if(uim_ptr->setting.work_arounds_for_non_spec_cards
           & UIM_IGNORE_POLL_ERROR_UNDER_7F10_DIRECTORY)
        { 
          /* Workaround for a non-spec complaint card that sends 3F00 or 103A even under 7F10 */
          if( uim_ptr->state.current_path[curr_ch].path.path[1] == 0x7F10 && 
              (status_file_id == 0x3F00 || status_file_id == 0x103A) )
          {
            ++uim_ptr->command.generic_state_ptr;
            return(UIM_CMD_SUCCESS);
          }
        }
        /* The selected file ID has changed */
        return(UIM_POLL_ERROR);
      }
    }
  } /* end if - the command was a poll command */
  else
  {
    if (rsp_ptr->cmd_status != UIM_DONE)
    {
      return(UIM_CMD_ERR);
    }
  }
  /* Process the next state */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_status_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_SEND_STATUS_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_send_status_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Restart recovery if command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    return (UIM_CMD_ERR);
  }
  else
  {
    /*Move to next state*/
    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_send_status_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_BIN_MEM_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_read_bin_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  byte mnc1 = 0, mnc2 = 0;
  byte mcc1 = 0, mcc2 = 0, mcc3 = 0;

  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  /* Return an error status if the command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
              /* Check if SFI was used and the command failed.
       If yes then retry the command without using SFI. */
    if (uim_ptr->command.cmd_ptr->read_bin.sfi != UIM_INVALID_SFI)
    {
      /* 1. Blank out the SFI so its not used while re-attempting the command.
         2. Blank out the directory structure as we are uncertain of the current directory
            pointer in the card and while re-attempting the command we would like to
            start from MF */
      uim_ptr->command.cmd_ptr->read_bin.sfi = UIM_INVALID_SFI;
      uim_clear_current_path(uim_ptr, curr_ch);
      return UIM_CMD_ERR_WITH_SFI;
    }
    return (UIM_CMD_ERR);
  }
  else
  {
    /* Temporary variable to hold the number of bytes actually read */
    word tmp_num_bytes_read;

    if ( uim_req_buf_static_ptr->apdu_hdr.p3 == 0 )
    {
      tmp_num_bytes_read = (word) UIM_MAX_TRANSACTION_SIZE;
    }
    else
    {
      tmp_num_bytes_read = (word) uim_req_buf_static_ptr->apdu_hdr.p3;
    }

    /* decrement the number of bytes left to read by the number read*/
    uim_ptr->command.rd_wr_info.num_bytes -= tmp_num_bytes_read;


    /* store the total number of bytes read up-to now */
    uim_ptr->command.rd_wr_info.num_bytes_rsp += tmp_num_bytes_read;

    /* Increment the offset so that next read would get
     * the right data
     */
    uim_ptr->command.rd_wr_info.offset += tmp_num_bytes_read;

    /* Copy over the data to the buffer provided each time we have
     * a peice of data returned
     */
    uim_memscpy(uim_ptr->command.rd_wr_info.data_ptr ,tmp_num_bytes_read, rsp_ptr->rsp.data,
           tmp_num_bytes_read);

    /* Increment the pointer to point to the next
     * location to copy the data
     */
    uim_ptr->command.rd_wr_info.data_ptr = uim_ptr->command.rd_wr_info.data_ptr +
      tmp_num_bytes_read;

    /* if no more bytes to read go to the next step*/
    if (uim_ptr->command.rd_wr_info.num_bytes == 0)
    {
      if (uim_ptr->command.cmd_ptr->hdr.command == UIM_READ_BIN_CMD_F)
      {
        /* Check MCC and MNC */
        /* -----------------------------------------------------------
          Parse MCC from the IMSI:
          B0     B1       B2       B3       B4     B5   B6   B7    B8
          LEN  MCC1PB   MCC3MCC2 MNC2MNC1 D1MNC3  D3D2 D5D4 D7D6  D9D8
        -------------------------------------------------------------*/
        mcc1 = rsp_ptr->rsp.data[1] & UIM_USIM_IMSI_HIGH_BYTE_MASK;
        mcc1 = mcc1 >> 4;
        mcc2 = rsp_ptr->rsp.data[2] & UIM_USIM_IMSI_LOW_BYTE_MASK;
        mcc3 = rsp_ptr->rsp.data[2] & UIM_USIM_IMSI_HIGH_BYTE_MASK;
        mcc3 = mcc3 >> 4;

        mnc1 = rsp_ptr->rsp.data[3] & UIM_USIM_IMSI_LOW_BYTE_MASK;
        mnc2 = rsp_ptr->rsp.data[3] & UIM_USIM_IMSI_HIGH_BYTE_MASK;
        mnc2 = mnc2 >> 4;

        if ((mcc1 == UIM_USIM_CARD_IMSI_MCC1_VAL) &&
            (mcc2 == UIM_USIM_CARD_IMSI_MCC2_VAL) &&
            (mcc3 == UIM_USIM_CARD_IMSI_MCC3_VAL) &&
            (mnc1 == UIM_USIM_CARD_IMSI_MNC1_VAL) &&
            (mnc2 == UIM_USIM_CARD_IMSI_MNC2_VAL))
        {
          uim_ptr->flag.check_card_problem = TRUE;
        }
        else
        {
          uim_ptr->flag.check_card_problem = FALSE;
        }
        UIMDRV_MSG_HIGH_1(uim_ptr->id,"Check Card Problem MCC/MNC match 0x%x",uim_ptr->flag.check_card_problem);
      }
      /*
       * This workaround is under the assumption that
       * IMSI is read before IMSI_M
       */
      if ((uim_ptr->command.cmd_ptr->hdr.command == UIM_READ_BIN_CMD_F) &&
          (uim_ptr->flag.check_card_problem))
      {
          /* Check if IMSI programmed */
          if((rsp_ptr->rsp.data[UIM_CSIM_IMSI_M_ADDR_OFFSET] & 0x80) == 0)
          {
            /* Disable return FCP for STATUS */
            uim_ptr->flag.disable_return_df_fcp_inpoll = TRUE;
          }
          else
          {
            uim_ptr->flag.disable_return_df_fcp_inpoll = FALSE;
          }
          UIMDRV_MSG_MED_1(uim_ptr->id,"IMSI_M_PROGRAMMED bit is 0x%x",
                                         uim_ptr->flag.disable_return_df_fcp_inpoll);
      }
      /* Skip to the next state. */

      /* To update the last_sel_dir and the last_sel_ef if SFI was used */
      if(uim_ptr->command.cmd_ptr->read_bin.sfi != UIM_INVALID_SFI)
      {
        UIMDRV_MSG_MED_0(uim_ptr->id,"UIM_READ_BIN_MEM_ST - SFI used successfully");
        uim_update_current_dir_sfi(uim_ptr, &uim_ptr->command.cmd_ptr->read_bin.path, curr_ch);
      }

      ++uim_ptr->command.generic_state_ptr;
    }
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_read_bin_mem_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UPDATE_BIN_MEM_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_update_bin_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  /* Return an error status if the command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    if (rsp_ptr->cmd_status == UIM_MEMORY_PROBLEM &&
        uim_is_ef_gsm_loci(&(uim_ptr->command.cmd_ptr->write_bin.path)))
    {
       /* Set the command count to MAX so as to not repeat this cmd */
       uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
    }

    /* Check if SFI was used and the command failed.
       If yes then retry the command without using SFI. */
    if (uim_ptr->command.cmd_ptr->write_bin.sfi != UIM_INVALID_SFI)
    {
      /* 1. Blank out the SFI so its not used while re-attempting the command.
         2. Blank out the directory structure as we are uncertain of the current directory
            pointer in the card and while re-attempting the command we would like to
            start from MF */
      uim_ptr->command.cmd_ptr->write_bin.sfi = UIM_INVALID_SFI;
      uim_clear_current_path(uim_ptr, curr_ch);
      return UIM_CMD_ERR_WITH_SFI;
    }

    return (UIM_CMD_ERR);
  }
  else
  {
    /* Decrement the number of bytes sent in the transaction */
    uim_ptr->command.rd_wr_info.num_bytes -= uim_req_buf_static_ptr->apdu_hdr.p3;

    /* store the total number of bytes read up-to now */
    uim_ptr->command.rd_wr_info.num_bytes_rsp  += uim_req_buf_static_ptr->apdu_hdr.p3;

    /* Check if we are done sending all the bytes
     * If the number of bytes requested is greater than the file size, then
     *  P3 is filled with the file size as the number of bytes to write
     */
    if (uim_ptr->command.rd_wr_info.num_bytes)
    {
      /* Change the offset for the next write */
      uim_ptr->command.rd_wr_info.offset += uim_req_buf_static_ptr->apdu_hdr.p3;

      /* Change the data pointer for the next write */
      uim_ptr->command.rd_wr_info.data_ptr += uim_req_buf_static_ptr->apdu_hdr.p3;
    }
    else
    {
      /* Skip to the next state. */


      /* To update the last_sel_dir and the last_sel_ef if SFI was used */
      if(uim_ptr->command.cmd_ptr->write_bin.sfi != UIM_INVALID_SFI)
      {
        UIMDRV_MSG_HIGH_0(uim_ptr->id,"UIM_UPDATE_BIN_MEM_ST - SFI used successfully");
        uim_update_current_dir_sfi(uim_ptr, &uim_ptr->command.cmd_ptr->write_bin.path, curr_ch);
      }

      ++uim_ptr->command.generic_state_ptr;
    }
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_update_bin_mem_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_READ_REC_MEM_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_read_rec_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  /* Return an error status if the command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
              /* Check if SFI was used and the command failed.
       If yes then retry the command without using SFI. */
    if (uim_ptr->command.cmd_ptr->read_rec.sfi != UIM_INVALID_SFI)
    {
      /* 1. Blank out the SFI so its not used while re-attempting the command.
         2. Blank out the directory structure as we are uncertain of the current directory
            pointer in the card and while re-attempting the command we would like to
            start from MF */
      uim_ptr->command.cmd_ptr->read_rec.sfi = UIM_INVALID_SFI;
      uim_clear_current_path(uim_ptr, curr_ch);
      return UIM_CMD_ERR_WITH_SFI;
    }

    return (UIM_CMD_ERR);
  }
  else
  {
    /* Need to copy the data here */
    word tmp_num_bytes_read;

    if ( uim_req_buf_static_ptr->apdu_hdr.p3 == 0 )
    {
      tmp_num_bytes_read = (word) UIM_MAX_TRANSACTION_SIZE;
    }
    else
    {
      tmp_num_bytes_read = (word) uim_req_buf_static_ptr->apdu_hdr.p3;
    }

    /* Copy over the data to the buffer provided each time we have
     * a peice of data returned
     */
    uim_memscpy(uim_ptr->command.rd_wr_info.data_ptr ,
           tmp_num_bytes_read,
           rsp_ptr->rsp.data,
           tmp_num_bytes_read);

    /* Set the number of response bytes received */
    uim_ptr->command.rd_wr_info.num_bytes_rsp = tmp_num_bytes_read;

    /* To update the last_sel_dir and the last_sel_ef if SFI was used */
    if(uim_ptr->command.cmd_ptr->read_rec.sfi != UIM_INVALID_SFI)
    {
      UIMDRV_MSG_MED_0(uim_ptr->id,"UIM_READ_REC_MEM_ST - SFI used successfully");
      uim_update_current_dir_sfi(uim_ptr, &uim_ptr->command.cmd_ptr->read_rec.path, curr_ch);
    }

    /* Skip to the next state. */
    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_read_rec_mem_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UPDATE_REC_MEM_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_update_rec_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  /* Return an error status if the command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    /* Check if SFI was used and the command failed.
       If yes then retry the command without using SFI. */
    if (uim_ptr->command.cmd_ptr->write_rec.sfi != UIM_INVALID_SFI)
    {
      /* 1. Blank out the SFI so its not used while re-attempting the command.
         2. Blank out the directory structure as we are uncertain of the current directory
            pointer in the card and while re-attempting the command we would like to
            start from MF */
      uim_ptr->command.cmd_ptr->write_rec.sfi = UIM_INVALID_SFI;
      uim_clear_current_path(uim_ptr, curr_ch);
      return UIM_CMD_ERR_WITH_SFI;
    }

    return (UIM_CMD_ERR);
  }
  else
  {
    uim_ptr->command.rd_wr_info.num_bytes_rsp  += uim_req_buf_static_ptr->apdu_hdr.p3;
    /* Skip to the next state. */
    /* To update the last_sel_dir and the last_sel_ef if SFI was used */
    if(uim_ptr->command.cmd_ptr->write_rec.sfi != UIM_INVALID_SFI)
    {
      UIMDRV_MSG_MED_0(uim_ptr->id,"UIM_UPDATE_REC_MEM_ST - SFI used successfully");
      uim_update_current_dir_sfi(uim_ptr, &uim_ptr->command.cmd_ptr->write_rec.path, curr_ch);
    }

    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_update_rec_mem_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_INC_MEM_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_inc_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  /* Return an error status if the command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    /* Check if SFI was used and the command failed.
       If yes then retry the command without using SFI. */
    if (uim_ptr->command.cmd_ptr->inc.sfi != UIM_INVALID_SFI)
    {
      /* 1. Blank out the SFI so its not used while re-attempting the command.
         2. Blank out the directory structure as we are uncertain of the current directory
            pointer in the card and while re-attempting the command we would like to
            start from MF */
      uim_ptr->command.cmd_ptr->inc.sfi = UIM_INVALID_SFI;
      uim_clear_current_path(uim_ptr, curr_ch);
      return UIM_CMD_ERR_WITH_SFI;
    }

    return (UIM_CMD_ERR);
  }
  else
  {
    /* Skip to the next state. */
    /* To update the last_sel_dir and the last_sel_ef if SFI was used */
    if(uim_ptr->command.cmd_ptr->inc.sfi != UIM_INVALID_SFI)
    {
      UIMDRV_MSG_MED_0(uim_ptr->id,"UIM_INCREMENT_ST - SFI used successfully");
      uim_update_current_dir_sfi(uim_ptr, &uim_ptr->command.cmd_ptr->inc.path, curr_ch);
    }

    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_inc_mem_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_VERIFY_CHV1_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_verify_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* If the response is good, then set the use flag to TRUE */
  if (rsp_ptr->cmd_status == UIM_DONE)
  {
    uim_ptr->cache.chv1.use_chv = TRUE;
  }
  else
  {
    /* Do not verify CHV1 until the verify succeeds. */
    uim_ptr->cache.chv1.use_chv = FALSE;
  } /* end if - the UIM command completed successfully. */

  /* Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_verify_chv1_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_VERIFY_CHV2_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_verify_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* If the response is good, then set the use flag to TRUE */
  if (rsp_ptr->cmd_status == UIM_DONE)
  {
    uim_ptr->cache.chv2.use_chv = TRUE;
  }
  else
  {
    /* Do not verify CHV1 until the verify succeeds. */
    uim_ptr->cache.chv2.use_chv = FALSE;

  } /* end if - the UIM command completed successfully. */
  /* Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_verify_chv2_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UNBLOCK_CHV1_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_unblock_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* If the response is good, then set the use flag to TRUE */
  if (rsp_ptr->cmd_status == UIM_DONE)
  {
    /* Keep the new CHV value in the cache buffer.  The cache buffer
       is used to verify CHV1 on wake up. */
    uim_memscpy( uim_ptr->cache.chv1.chv,
            sizeof(uim_ptr->cache.chv1.chv),
            uim_ptr->command.cmd_ptr->unblk_chv.new_chv_digits,
            UIM_MAX_CHV_DIGITS);

    /* Format the CHV digits in the cache buffer. */
    uim_format_chv(uim_ptr->cache.chv1.chv,uim_ptr);

    uim_ptr->cache.chv1.use_chv = TRUE;
  }
  else
  {
    /* Do not verify CHV1 again until a CHV command succeeds. */
    uim_ptr->cache.chv1.use_chv = FALSE;
  } /* end if - the UIM command completed successfully. */

  /* Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_unblock_chv1_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UNBLOCK_CHV2_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_unblock_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_unblock_chv2_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_CHANGE_CHV1_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_change_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* If the response is good, then set the use flag to TRUE */
  if (rsp_ptr->cmd_status == UIM_DONE)
  {
    /* Keep the new CHV value in the cache buffer.  The cache buffer
       is used to verify CHV1 on wake up. */
    uim_memscpy( uim_ptr->cache.chv1.chv,
            sizeof(uim_ptr->cache.chv1.chv),
            uim_ptr->command.cmd_ptr->change_chv.new_chv_digits,
            UIM_MAX_CHV_DIGITS);

    /* Format the CHV digits in the cache buffer. */
    uim_format_chv(uim_ptr->cache.chv1.chv, uim_ptr);

    uim_ptr->cache.chv1.use_chv = TRUE;
  }
  else
  {
    /* Do not verify CHV1 again until the verify succeeds. */
    uim_ptr->cache.chv1.use_chv = FALSE;
  } /* end if - the UIM command completed successfully. */

  /* Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_change_chv1_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_CHANGE_CHV2_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_change_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Since we are not caching the CHV2, Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_change_chv2_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_ENABLE_CHV1_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_enable_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* If the response is good, then set the use flag to TRUE */
  if (rsp_ptr->cmd_status == UIM_DONE)
  {
    /* Keep the new CHV value in the cache buffer.  The cache buffer
       is used to verify CHV1 on wake up. */
    uim_memscpy( uim_ptr->cache.chv1.chv,
            sizeof(uim_ptr->cache.chv1.chv),
            uim_ptr->command.cmd_ptr->enable_chv.chv_digits,
            UIM_MAX_CHV_DIGITS);

    /* Format the CHV digits in the cache buffer. */
    uim_format_chv(uim_ptr->cache.chv1.chv, uim_ptr);

    uim_ptr->cache.chv1.use_chv = TRUE;
  }
  else
  {
    /* Do not verify CHV1 again until a CHV command succeeds. */
    uim_ptr->cache.chv1.use_chv = FALSE;
  } /* end if - the UIM command completed successfully. */

  /* Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_enable_chv1_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_ENABLE_CHV2_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_enable_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Since we are not caching the CHV2, Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_enable_chv2_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_DISABLE_CHV1_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_disable_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
   /* If this command was successful, then the CHV was disabled
     successfully, so we do not have to verify CHV upon power up.

     If the command failed with SW1 as 98, then the CHV presented
     is incorrect or already disabled or blocked, then the use
     flag is set to FALSE */

  /* If the response is good, then copy the CHV1 digits to the cache
     buffer. */
  if (rsp_ptr->cmd_status == UIM_DONE)
  {
    /* Keep the new CHV value in the cache buffer.  The cache buffer
       is used to verify CHV1 on wake up. */
    uim_memscpy( uim_ptr->cache.chv1.chv,
            sizeof(uim_ptr->cache.chv1.chv),
            uim_ptr->command.cmd_ptr->unblk_chv.new_chv_digits,
            UIM_MAX_CHV_DIGITS);

    /* Format the CHV digits in the cache buffer. */
    uim_format_chv(uim_ptr->cache.chv1.chv, uim_ptr);

  }
  /* Whether the command succeeds, there is no reason the verify CHV1.
     If the command fails, the CHV1 digits do not match the card.  In
     either case, CHV1 is not to be verified. */
  uim_ptr->cache.chv1.use_chv = FALSE;

  /* if the sw indicates that there is some other error, then we
     would like to continue verifying CHV on power up with the
     old CHV and not from the CHV in the disable command */
  if (rsp_ptr->sw1 != UIM_SW1_SECURITY)
  {
     uim_ptr->cache.chv1.use_chv = TRUE;
  } /* end if - sw1 indicates a problem other than security management.*/

  /* Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_disable_chv1_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_DISABLE_CHV2_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_disable_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Since we are not caching the CHV2, Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_disable_chv2_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_INVALIDATE_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_invalidate_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Check if there was a problem with Invalidation */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    return (UIM_CMD_ERR);
  } /* end if - there was a problem with the APDU */

  /* Get the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_invalidate_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_REHABILITATE_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_rehabilitate_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Check if there was a problem with Rehab */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    return (UIM_CMD_ERR);
  } /* end if - there was a problem with the APDU */

  /* Get the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_rehabilitate_st_rsp */

/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SEEK_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_seek_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Create a ptr to current channel to increase code readability */
  uim_channel_type curr_ch = uim_ptr->command.cmd_ptr->hdr.channel;

  /* Return an error status if the command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    /* Check if SFI was used and the command failed.
       If yes then retry the command without using SFI. */
    if (uim_ptr->command.cmd_ptr->seek.sfi != UIM_INVALID_SFI)
    {
      if(uim_ptr->card_cmd.resp_buf_ptr->cmd_status != UIM_RECORD_NOT_FOUND)
      {
        /* 1. Blank out the SFI so its not used while re-attempting the command.
           2. Blank out the directory structure as we are uncertain of the current directory
              pointer in the card and while re-attempting the command we would like to
              start from MF */
        uim_ptr->command.cmd_ptr->seek.sfi = UIM_INVALID_SFI;
        uim_clear_current_path(uim_ptr, curr_ch);
        return UIM_CMD_ERR_WITH_SFI;
      }
      else
      {
        /* If seek failed due to 6A 83 then do not re-attempt
            Update the last_sel_ef and return UIM_CMD_ERR */
        uim_update_current_dir_sfi(uim_ptr, &uim_ptr->command.cmd_ptr->seek.path, curr_ch);
        uim_ptr->state.last_selected.required = TRUE;
        uim_ptr->state.last_selected.channel  = uim_ptr->command.cmd_ptr->hdr.channel;
      }
    }
    return (UIM_CMD_ERR);
  }
  else
  {
    /* Skip to the next state. */
    /* To update the last_sel_dir and the last_sel_ef if SFI was used */
    if(uim_ptr->command.cmd_ptr->seek.sfi != UIM_INVALID_SFI)
    {
      UIMDRV_MSG_MED_0(uim_ptr->id,"UIM_SEEK_ST - SFI used successfully");
      uim_update_current_dir_sfi(uim_ptr, &uim_ptr->command.cmd_ptr->seek.path, curr_ch);
    }

    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_seek_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_TERMINAL_PROFILE_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_terminal_profile_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* We need to cache the terminal profile value even if there was
     error in the transaction. GSTK will not send terminal profile
     again if we hit recovery during terminal profile APDU */
  if (uim_ptr->command.cmd_ptr->terminal_profile.num_bytes <= UIM_TERMINAL_PROFILE_DATA_LENGTH)
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"Cache Terminal Profile");
    /* Store the number of bytes that was sent in the command */
    uim_ptr->cache.int_term_profile_data.num_bytes =
                                  uim_ptr->command.cmd_ptr->terminal_profile.num_bytes;
    /* Copy the data from the command into internal buffer */
    uim_memscpy(uim_ptr->cache.int_term_profile_data.data,
             sizeof(uim_ptr->cache.int_term_profile_data.data),
             uim_ptr->command.cmd_ptr->terminal_profile.data,
             uim_ptr->command.cmd_ptr->terminal_profile.num_bytes);
  }
  /* Return an error status if the command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    return (UIM_CMD_ERR);
  }
  else
  {
    /* Skip to the next state. */
    ++uim_ptr->command.generic_state_ptr;

    if (uim_ptr->command.cmd_ptr->hdr.command == UIM_TERMINAL_PROFILE_F)
    {
      if (uim_nv_is_feature_enabled(UIMDRV_FEATURE_MIN_TPL_ICCID_SUPPORT,
                                    uim_ptr) &&
          NULL != uim_ptr->state.iccid_tpl_ptr)
      {
        /* Terminal Profile is working fine, No need to update ICCID and TPL */
        /* No need to continue uim_ptr->state.iccid_tpl_ptr further, freeing memory here */
        UIM_FREE(uim_ptr->state.iccid_tpl_ptr);
        /* After that memory will be allocate in next terminal profile
           command */
      }
    }
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_terminal_profile_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_ENVELOPE_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_envelope_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Return an error status if the command failed. */
  if ((rsp_ptr->cmd_status != UIM_DONE) &&
      (rsp_ptr->cmd_status != UIM_TOOLKIT_BUSY))
  {
    return (UIM_CMD_ERR);
  }
  else
  {
    /* Skip to the next state. */
    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_envelope_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_FETCH_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_fetch_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  if ((uim_ptr == NULL) || (rsp_ptr == NULL))
  {
    return (UIM_CMD_ERR);
  }

  if((UIM_PROBLEM == rsp_ptr->cmd_status) &&
     (uim_ptr->setting.work_arounds_for_non_spec_cards &
                  UIM_IGNORE_TECH_PROBLEM_ERROR_FOR_FETCH_COMMAND))
  {
    /* Ignore the Technical UIM problem error for fetch command */
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"Ignoring tech problem error for fetch command");
    uim_ptr->command.generic_state_ptr++;
    return UIM_CMD_SUCCESS;
  }
  
  /* Return an error status if the command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    return (UIM_CMD_ERR);
  }
  else
  {
    /* Skip to the next state. */
    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_fetch_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_TERMINAL_RESPONSE_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_terminal_response_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Return an error status if the command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    return (UIM_CMD_ERR);
  }
  else
  {

    if ( *completed == UIM_CMD_FETCH)
    {
      /* Tell GSTK a proactive command pending */
      uim_ptr->command.rpt_buf.rpt.terminal_response.proactive_cmd_pending = TRUE;
    }

    /* Skip to the next state. */
    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_terminal_response_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_POLLING_INTERVAL_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_polling_interval_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Skip to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_polling_interval_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_STREAM_APDU_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_stream_apdu_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Pointer to the Global UIM Request buffer */
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  /* Return an error status if the command failed and 
     update the status words if first status words is 
     warning status words */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    if((uim_ptr->card_cmd.get_resp_sw1 == SW1_WARNINGS1) ||
       (uim_ptr->card_cmd.get_resp_sw1 == SW1_WARNINGS2))
    {
      uim_ptr->card_cmd.resp_buf_ptr->cmd_status = UIM_DONE;
      uim_ptr->command.rpt_buf.sw1 = uim_ptr->card_cmd.get_resp_sw1;
      uim_ptr->command.rpt_buf.sw2 = uim_ptr->card_cmd.get_resp_sw2;
      return UIM_CMD_SUCCESS;
    }
   
    uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
    return (UIM_CMD_ERR);
  }

  /* Is extended APDU data chunks is still pending to be sent to card then return SUCCESS */
  if(uim_ptr->card_cmd.is_ext_cmd_transaction_done == FALSE && 
    uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes > UIM_MAX_STREAM_ISO7816_APDU_TRANSACTION_SIZE)
  {
    /* For 9000 and 6883 status words return SUCCESS */
    if((rsp_ptr->sw1 == SW1_NORMAL_END && rsp_ptr->sw2 == SW2_NORMAL_END) ||
       (rsp_ptr->sw1 == SW1_CLA_NOT_SUPPORTED && rsp_ptr->sw2 == SW2_LAST_CMD_OF_CHAIN_EXP))
    {
      return UIM_CMD_SUCCESS;
    }
    /* If status word is 61XX then terminate the session and send get response */
    else if(rsp_ptr->sw1 == SW1_UTIL_END_RESP)
    {
      uim_ptr->card_cmd.is_ext_cmd_transaction_done = TRUE;
      uim_ptr->card_cmd.ext_cmd_data_offset = uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes;
    }
    else
    {
      uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
      rsp_ptr->cmd_status = UIM_PROBLEM;
      return UIM_CMD_ERR;
    }
  }
    
  if(UIM_CARD_SAM  == uim_ptr->card_mode)
  {
    uim_sam_stream_apdu_success(uim_ptr, uim_req_buf_static_ptr, rsp_ptr);
  }
  else
  {
    uim_stream_apdu_success(uim_ptr, uim_req_buf_static_ptr, rsp_ptr);
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_stream_apdu_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_COMPLETE_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_complete_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Just skip to the next state */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_complete_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_DONE_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_done_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Do nothing */
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_done_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_FLUSH_MANAGE_CHANNEL_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_flush_manage_channel_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  UIMDRV_MSG_MED_0(uim_ptr->id,"Should not reach here!!");
  return UIM_CMD_ERR;
} /* uimgen_iso_handle_recovery_flush_manage_channel_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_VERIFY_CHV2_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_verify_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Restart recovery if command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    UIMDRV_MSG_MED_0(uim_ptr->id,"Pin1 Verification failed during recovery");
    return (UIM_CMD_ERR);
  }
  else
  {
    /* Move to the next state. */
    ++uim_ptr->command.generic_state_ptr;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_verify_chv1_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_VERIFY_CHV2_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_verify_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Restart recovery if command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    UIMDRV_MSG_MED_0(uim_ptr->id,"Pin2 Verification failed during recovery");
    return (UIM_CMD_ERR);
  }
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_verify_chv2_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_PERFORM_TP_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_perform_tp_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    UIMDRV_MSG_ERR_1(uim_ptr->id,"CMD Status = 0x%x, Continuing with recovery !!",
                     rsp_ptr->cmd_status);
  }
  /* Move to the next state. */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_perform_tp_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_CHECK_REHAB_REQD_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_check_rehab_reqd_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  UIMDRV_MSG_ERR_0(uim_ptr->id, "RECOVERY CHECK REHAB REQD is not a valid response state");
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_check_rehab_reqd_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_REHAB_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_rehab_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* The item has been rehabilitated if it's get response has been
   * cached.... remove it from the cache so that the next select
   * on the item is forced to do a get response
   */

  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    UIMDRV_MSG_MED_1(uim_ptr->id,"Recovery Rehabilitate on file_index= %d returned error",
                     uim_ptr->state.recovery_data.rehab_file_index);
    return (UIM_CMD_ERR);
  } /* end if - there was a problem with the APDU */
  /* Increment file index */
  ++uim_ptr->state.recovery_data.rehab_file_index;
  /* Move to check for rehab state for next file */
  uim_ptr->command.generic_state_ptr -=2;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_rehab_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_CHECK_EF_TST_UPDATE_REQD_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_check_ef_tst_update_reqd_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  UIMDRV_MSG_ERR_0(uim_ptr->id, "RECOVERY CHECK EF_TST_UPDATE REQD is not a valid response state");
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_check_ef_tst_update_reqd_st_rsp */



/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_SELECT_EF_TST_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_select_ef_tst_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  uint8 ch_no = uim_ptr->state.recovery_data.ch_to_be_recovered;

  if(rsp_ptr->cmd_status != UIM_DONE)
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"select of EF-TST failed. skip write EF-tst and move to next state");
    rsp_ptr->cmd_status               = UIM_DONE;

    /*select failed there might be a mismatch in directory.
      to avoid that queue a select MF command to reset the directory*/
    uim_ptr->state.mf_selection.required  = TRUE;
    uim_ptr->state.mf_selection.channel   = ch_no;
    uim_explicit_reselect_mf_aid(uim_ptr);

    if(uim_ptr->state.recovery_data.protocol == UIM_UICC)
    {
      /* Increment channel index */
      ++uim_ptr->state.recovery_data.ch_to_be_recovered;
      /* Move to next channel*/
      uim_ptr->command.generic_state_ptr -= 8;
    }
    else
    {
      uim_ptr->command.generic_state_ptr +=2;
    }

    return UIM_CMD_SUCCESS;
  }

  if(UIM_ICC == uim_ptr->command.cmd_ptr->hdr.protocol  &&
     (uim_ptr->command.cmd_ptr->select.path_position < 2) &&
     UIM_DONE == rsp_ptr->cmd_status)
  {
    /* For ICC we select incrementally*/
    uim_ptr->command.cmd_ptr->select.path_position++;
    return UIM_CMD_SUCCESS;
  }

  /*update the current path to EF-tst*/
  if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
  {
    uim_ptr->state.current_path[ch_no].path.path[0] = UIM_MF_ID;
    uim_ptr->state.current_path[ch_no].path.path[1] = 0x7FFF;
    uim_ptr->state.current_path[ch_no].path.path[2] = 0x7F66;
    uim_ptr->state.current_path[ch_no].path.path[3] = 0x6FD2;

    uim_ptr->state.current_path[ch_no].path.len     = 4;
    uim_ptr->state.current_path[ch_no].is_ef        = TRUE;

  }
  else
  {
    uim_ptr->state.current_path[0].path.path[0] = UIM_MF_ID;
    uim_ptr->state.current_path[0].path.path[1] = 0x7F66;
    uim_ptr->state.current_path[0].path.path[2] = 0x6FD2;

    uim_ptr->state.current_path[0].path.len     = 3;
    uim_ptr->state.current_path[0].is_ef        = TRUE;
  }
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_select_ef_tst_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_UIM_RECOVERY_WRITE_EF_TST_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_write_ef_tst_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  if(rsp_ptr->cmd_status != UIM_DONE)
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"write of EF-TST failed. ignore the failure");
    rsp_ptr->cmd_status               = UIM_DONE;
  }

  if(uim_ptr->state.recovery_data.protocol == UIM_UICC)
  {
    /* Increment channel index */
    ++uim_ptr->state.recovery_data.ch_to_be_recovered;
    /* Move to next channel*/
    uim_ptr->command.generic_state_ptr -= 9;
  }
  else
  {
    ++uim_ptr->command.generic_state_ptr;
  }

  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_write_ef_tst_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_MANAGE_CHANNEL_FOR_UICC_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_manage_channel_for_uicc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Restart recovery if command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    UIMDRV_MSG_MED_1(uim_ptr->id,"Channel=%d failed to open during Recovery",
                     uim_ptr->state.recovery_data.ch_to_be_recovered);
    return (UIM_CMD_ERR);
  } /* end if - there was a problem with the APDU */
  else
  {
    UIMDRV_MSG_MED_1(uim_ptr->id,"Channel=%d opened during Recovery",
                     uim_ptr->state.recovery_data.ch_to_be_recovered);
    ++uim_ptr->state.recovery_data.ch_to_be_recovered;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_manage_channel_for_uicc_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_CLOSE_CHANNEL_FOR_UICC_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_close_channel_for_uicc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  UIMDRV_MSG_MED_2(uim_ptr->id,"Channel=%d closed during Recovery with cmd_status - 0x%x",
                     uim_ptr->state.recovery_data.ch_to_be_recovered, rsp_ptr->cmd_status);
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    /* Clear out the command response timer as we are setting the timeout signal */
    (void) rex_clr_timer( &uim_ptr->command.cmd_rsp_timer);
    /* UIM should try to recover */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
  } /* end if - there was a problem with the APDU */
  else
  {
     /*Set this flag to FASLE for every closed channel*/
     uim_ptr->state.is_ch_open[uim_ptr->state.recovery_data.ch_to_be_recovered] = FALSE;
     /*Set this flag to default for every closed channel*/
     uim_ptr->state.ch_apdu_behavior[uim_ptr->state.recovery_data.ch_to_be_recovered] = UIM_APDU_BEHAVIOR_FOR_TELECOM_APP;
    /* Process next channel */
    ++uim_ptr->state.recovery_data.ch_to_be_recovered;
  }
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_close_channel_for_uicc_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_CHECK_FOR_SELECT_AID_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_check_for_select_aid_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  UIMDRV_MSG_ERR_0(uim_ptr->id, "RECOVERY CHECK FOR SELECT AID is not a valid response state");
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_check_for_select_aid_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_RECOVERY_VERIFY_UPIN_FOR_UICC_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_recovery_verify_upin_for_uicc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  /* Restart recovery if command failed. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
     UIMDRV_MSG_MED_0(uim_ptr->id,"UPIN verification failed");
     return (UIM_CMD_ERR);
  } /* end if - there was a problem with the APDU */
  /* Move to next state */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_recovery_verify_upin_for_uicc_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_GET_RESPONSE_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_get_response_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  UIMDRV_MSG_ERR_0(uim_ptr->id, "GET RESPONSE is not a valid response state");
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_get_response_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SILENT_RECOVER_UICC_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_silent_recover_uicc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  UIMDRV_MSG_ERR_0(uim_ptr->id, "SILENT RECOVER UICC is not a valid response state");
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_silent_recover_uicc_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_MANAGE_CHANNEL_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_manage_channel_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  int i = 0;

  /* Force uim status to success when card returns 62 00 */
  if(rsp_ptr->cmd_status == UIM_NO_INFO_AND_NV_UNCHANGED)
  {
    rsp_ptr->cmd_status = UIM_DONE;
  }
  /* Determine if the UIM command responded with a problem. */
  if (rsp_ptr->cmd_status != UIM_DONE)
  {
    /* Check if Manage Channel is failed with SW1 0x6E SW2 0x00 */
    if ( ( UIM_WRONG_CLASS == rsp_ptr->cmd_status &&
           SW1_CLA_BAD     == rsp_ptr->sw1    &&
           SW2_NORMAL_END  == rsp_ptr->sw2 ) ||
         ( UIM_MC_CLOSE == uim_ptr->command.cmd_ptr->manage_channel.mc_action )
       )
    {
      uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_CMD_ATTEMPTS(uim_ptr);
      UIMDRV_MSG_HIGH_3(uim_ptr->id,
                       "Manage Channel is not being re-tried for SW1 0x%x SW2 0x%x status %d",
                       rsp_ptr->sw1,
                       rsp_ptr->sw2,
                       rsp_ptr->cmd_status);
    }

    if ( (uim_ptr->command.cmd_ptr->manage_channel.mc_action == UIM_MC_CLOSE) &&
         (TRUE == uim_ptr->state.aram_app.select_aram_failed) &&
         (uim_ptr->command.cmd_ptr->manage_channel.op_close_channel == uim_ptr->state.aram_app.channel) )
    {
      (void)uim_trigger_silent_recovery((uim_slot_type)(uim_ptr->id+1),
                                               uim_ptr->command.cmd_ptr,
                                               NULL);
      uim_ptr->state.aram_app.select_aram_failed = FALSE;
      uim_ptr->state.aram_app.channel            = 0xFF;
    }
    /* Indicate a problem with the command. */
    return (UIM_CMD_ERR);
  }
  else /* The command completed successfully. */
  {
    if ((uim_ptr->command.cmd_ptr->manage_channel.mc_action == UIM_MC_CLOSE) &&
        (TRUE == uim_ptr->state.aram_app.select_aram_failed) &&
        (uim_ptr->command.cmd_ptr->manage_channel.op_close_channel == uim_ptr->state.aram_app.channel))
    {
      if(uim_ptr->setting.work_arounds_for_non_spec_cards &
         UIM_ADF_SELECTION_ON_OTHER_CHANNELS_UPON_ARAM_APP_FAILURE)
      {
        UIMDRV_MSG_HIGH_0(uim_ptr->id,"Queueing MF/ADF on other channels, after close channel on which ARA-M app failed");
        for(i=0; i<UIM_MAX_CHANNELS; i++)
        {
          if((uim_ptr->state.is_ch_open[i]== TRUE) &&
             (i != uim_ptr->state.aram_app.channel ))
          {
            if(0 != uim_ptr->state.current_path[i].curr_app.aid_length)
            {
              if(uim_is_telecom_aid(uim_ptr->state.current_path[i].curr_app))
              {
                uim_ptr->state.adf_selection.required  = TRUE;
                uim_ptr->state.adf_selection.channel   = i;
              }
            }
            else
            {
              uim_ptr->state.mf_selection.required  = TRUE;
              uim_ptr->state.mf_selection.channel   = i;
            }
            uim_explicit_reselect_mf_aid(uim_ptr);
          }
        }
      }
      uim_ptr->state.aram_app.select_aram_failed = FALSE;
      uim_ptr->state.aram_app.channel            = 0xFF;
    }
    /* In case of SELECT ISDR, we would need to keep track of
       the channel number that the cmd is sent on and the subsequent
       GET EID cmd */
	if(TRUE == uim_ptr->flag.is_select_isdr)
	{
      uim_ptr->select_isdr_ch_num.channel = rsp_ptr->rsp.data[0];
	}
    /* Get the next state. */
    uim_ptr->command.generic_state_ptr++;
  } /* end if - the UIM command responded with a problem. */
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_manage_channel_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SET_APDU_BEHAVIOR_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_set_apdu_behavior_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  uim_ptr->command.generic_state_ptr++;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_set_apdu_behavior_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_SELECT_ISDR_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_select_isdr_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed_ptr
)
{
  uim_select_isdr_resp_type      select_isdr_resp;
  uim_return_type                status;

  if(rsp_ptr == NULL ||
     uim_ptr == NULL)
  {
    return UIM_CMD_ERR;
  }

  if(rsp_ptr->cmd_status != UIM_DONE)
  {
    /* If the select ISDR fails, do not attempt to
       send GET EID command. Move to the manage channel state and indicate that
       the current command succeeded to complete the power up sequence */
    /* Fill in the params to close the channel that was used to select ISDR */
    uim_ptr->command.cmd_ptr->manage_channel.mc_action        = UIM_MC_CLOSE;
    uim_ptr->command.cmd_ptr->manage_channel.channel          = 0x0;
    uim_ptr->command.cmd_ptr->manage_channel.op_close_channel = uim_ptr->select_isdr_ch_num.channel;
    uim_ptr->command.generic_state_ptr += 2;
	uim_ptr->flag.is_select_isdr        = FALSE;
	return UIM_CMD_SUCCESS;
  }
  
  status = uim_decode_select_isdr_resp(rsp_ptr->cmd_rsp_size,
                                       rsp_ptr->rsp.data,
							           &select_isdr_resp);
  if(status == UIM_SUCCESS)
  {
	status = uim_set_svn(uim_ptr->logical_id,
                         sizeof(select_isdr_resp.svn),
			             select_isdr_resp.svn);
  }
	
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_select_isdr_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_GET_EID_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_get_eid_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed_ptr
)
{
  uint16 rsp_size                           = 0;
  uim_return_type status                    = UIM_SUCCESS;
  
  if(uim_ptr == NULL ||
     rsp_ptr == NULL)
  {
    return UIM_CMD_ERR;
  }
  
  rsp_size = rsp_ptr->cmd_rsp_size;
  uim_ptr->cache.eid_buf.num_bytes = rsp_size;
  
  status = uim_parse_eid_resp(rsp_size,
                              rsp_ptr->rsp.data,
                              &uim_ptr->cache.eid_buf);
  if(status == UIM_SUCCESS)
  {
    uim_ptr->cache.eid_buf.num_bytes = UIM_MAX_EID_SIZE;  
  }
  
  /* Terminate the currently selected application and move to the next state */
  /* Fill in the params to close the channel that was used to select ISDR */
  uim_ptr->command.cmd_ptr->manage_channel.mc_action        = UIM_MC_CLOSE;
  uim_ptr->command.cmd_ptr->manage_channel.channel          = 0x0;
  uim_ptr->command.cmd_ptr->manage_channel.op_close_channel = uim_ptr->select_isdr_ch_num.channel;
  
  uim_ptr->flag.is_select_isdr        = FALSE;
  uim_ptr->select_isdr_ch_num.channel = 0;
  
  /* Goto next state */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
} /* uimgen_iso_handle_get_eid_st_rsp */


/*=============================================================================
  FUNCTION UIMGEN_ISO_HANDLE_NOT_PUP_IF_ALREADY_ST_RSP
=============================================================================*/
uim_cmd_status_type uimgen_iso_handle_not_pup_if_already_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed_ptr
)
{
  (void)rsp_ptr;
  (void)completed_ptr;

  /* Move to next state */
  ++uim_ptr->command.generic_state_ptr;
  return UIM_CMD_SUCCESS;
}

/*=============================================================================
  INTERNAL FUNCTION DEFINITIONS
==============================================================================*/
/*===========================================================================

FUNCTION UIM_SET_UICC_PIN_PARAM_P2

DESCRIPTION
  This procedure sets the parameter p2 in the apdu header based on the USIM
  PIN.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if a valid PIN type is recd. FALSE otherwise.

SIDE EFFECTS
  Fills the p2 in the apdu header.
===========================================================================*/
static void uim_set_uicc_pin_param_p2
(
  uim_chv_type chv,
  byte *p2,
  uim_instance_global_type *uim_ptr
)
{
  if (p2 == NULL)
  {
    return;
  }
  switch (chv)
  {
    case UIM_PIN1_APP1:
       *p2 = UIM_PIN1_APP1_VALUE;
    break;

    case UIM_PIN1_APP2:
       *p2 = UIM_PIN1_APP2_VALUE;
    break;

    case UIM_PIN1_APP3:
       *p2 = UIM_PIN1_APP3_VALUE;
    break;

    case UIM_PIN1_APP4:
       *p2 = UIM_PIN1_APP4_VALUE;
    break;

    case UIM_PIN1_APP5:
       *p2 = UIM_PIN1_APP5_VALUE;
    break;

    case UIM_PIN1_APP6:
       *p2 = UIM_PIN1_APP6_VALUE;
    break;

    case UIM_PIN1_APP7:
       *p2 = UIM_PIN1_APP7_VALUE;
    break;

    case UIM_PIN1_APP8:
       *p2 = UIM_PIN1_APP8_VALUE;
    break;

    case UIM_PIN2_APP1:
       *p2 = UIM_PIN2_APP1_VALUE;
    break;

    case UIM_PIN2_APP2:
       *p2 = UIM_PIN2_APP2_VALUE;
    break;

    case UIM_PIN2_APP3:
       *p2 = UIM_PIN2_APP3_VALUE;
    break;

    case UIM_PIN2_APP4:
       *p2 = UIM_PIN2_APP4_VALUE;
    break;

    case UIM_PIN2_APP5:
       *p2 = UIM_PIN2_APP5_VALUE;
    break;

    case UIM_PIN2_APP6:
       *p2 = UIM_PIN2_APP6_VALUE;
    break;

    case UIM_PIN2_APP7:
       *p2 = UIM_PIN2_APP7_VALUE;
    break;

    case UIM_PIN2_APP8:
       *p2 = UIM_PIN2_APP8_VALUE;
    break;

    case UIM_UNIVERSAL_PIN:
       *p2 = UIM_UNIVERSAL_PIN_VALUE;
    break;

    default:
       *p2 = 0;
       UIMDRV_MSG_ERR_1(uim_ptr->id,"Incorrect chv_type 0x%x", chv);
    break;
  }
} /* uim_set_uicc_pin_param_p2 */


/*===========================================================================
FUNCTION UIM_COMPARE_MANAGE_CH_CMD

DESCRIPTION
  UIM Action function callback that is called by q_linear_delete when the
  compare callback function returns TRUE. This function will return TRUE only
  when the command id is that of a MANAGE CHANNEL in case of Processing
Recovery while in ICC mode
  These commands need to be removed from UIM queue before starting recovery.

PARAMETER
  item_ptr  [Input]:  ptr to queue item
  param_ptr [Input]:  no use as of now. This will be populated as NULL
                      by q_linear_delete()

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if there is a match
  FALSE - if there is no match

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
int uim_compare_manage_ch_cmd
(
  void *item_ptr,
  void *param_ptr
)
{
  uim_cmd_type          *cmd_ptr      = NULL;

  cmd_ptr = (uim_cmd_type*)item_ptr;

  if (!cmd_ptr ||
      cmd_ptr->hdr.slot == UIM_SLOT_NONE ||
      cmd_ptr->hdr.slot >= UIM_SLOT_AUTOMATIC)
  {
    return 0;
  }

  switch((uim_cmd_name_type)cmd_ptr->hdr.command)
  {
    case UIM_MANAGE_CHANNEL_F:
      UIMDRV_MSG_MED_0((cmd_ptr->hdr.slot - 1), "MANAGE CHANNEL command matched");
      return 1;
    default:
      return 0;
  }/* switch */

} /* uim_compare_toolkit_cmds */


/*===========================================================================
FUNCTION UIM_REMOVE_MANAGE_CH_CMD

DESCRIPTION
  UIM Action function callback that is called by q_linear_delete when the
  compare callback function returns TRUE. This function puts the command
  removed from the command queue into the free queue.

PARAMETER
  item_ptr  [Input]:  ptr to queue item
  param_ptr [Input]:  no use as of now. This will be populated as NULL
                      by q_linear_delete()

DEPENDENCIES
  None

RETURN VALUE
  void

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None

===========================================================================*/
void uim_remove_manage_ch_cmd
(
  void  *item_ptr,
  void  *param_ptr
)
{
  uim_cmd_type             *cmd_ptr          = NULL;
  uim_instance_global_type *uim_ptr = (uim_instance_global_type*)param_ptr;

  if (uim_ptr == NULL)
  {
    return;
  }

  if(item_ptr == NULL)
  {
    return;
  }

  cmd_ptr = (uim_cmd_type*)item_ptr;

  /* return an invalid status in the header to all the commands */
  cmd_ptr->hdr.status = UIM_ERR_S;
  if (cmd_ptr->hdr.cmd_hdr.task_ptr != NULL)
  {
    (void) rex_set_sigs( cmd_ptr->hdr.cmd_hdr.task_ptr,
                             cmd_ptr->hdr.cmd_hdr.sigs );
  }

  /* If always reporting & have a report function, report failure */
  if (((int)cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0)
  {
    uim_send_generic_err_rpt(cmd_ptr, uim_ptr);
  }
  else
  {  /* return buffer to free queue */
    UIMDRV_MSG_HIGH_1(uim_ptr->id, "Not Sending message error report 0x%x ", cmd_ptr->hdr.command);
    if ( cmd_ptr->hdr.cmd_hdr.done_q_ptr != NULL )
    {
      /* place command on requested queue */
      q_put( cmd_ptr->hdr.cmd_hdr.done_q_ptr,
             &cmd_ptr->hdr.cmd_hdr.link );
     }
  }
} /* uim_remove_toolkit_cmds */


/*===========================================================================

FUNCTION UIM_PARSE_UICC_SELECT_RESPONSE

DESCRIPTION
  This procedure parses the UICC select response for an EF and fills the
  required fields of last selected EF structure if required and fils the
  file characteristic byte.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Fills the file characteristic byte and elements of the last selected EF
  structure when the file selected is an EF that is under an ADF.
===========================================================================*/
static void uim_parse_uicc_select_response
(
  uim_instance_global_type     *uim_ptr,
  byte const                   *data,
  word                          num_bytes,
  boolean                      *ef_rsp_ptr,
  byte                         *file_char,
  uint8                        *supported_cmd_present,
  uim_num_apps_data_type       *num_apps_ptr
)
{
  uint16 i = 0; /* index variable */
  uint16 j = 0; /* Temp storage for length */
  uint16 k = 0; /* Innerloop variable for proprietary information */

  if (uim_ptr == NULL)
  {
    return;
  }

  if (data == NULL)
  {
    UIMDRV_MSG_LOW_0(uim_ptr->id,"uim_parse_uicc_select_response data is NULL or num_bytes is Zero");
    return;
  }

  /* If FCP, FCI, FCM template tag is invalid, select MF After
     So that current directory in UIMDRV is in sync with CARD.
     Since, polling is only happening on channel - 0, So add 
     check for channel - 0*/
  if ((uim_ptr->command.cmd_ptr->hdr.channel == 0) &&
      (num_bytes > 0) &&
      (data[0] != UIM_FCP_TEMPLATE_TAG  &&
       data[0] != UIM_FCI_TEMPLATE_TAG  &&
       data[0] != UIM_FCM_TEMPLATE_TAG))
  {
    uim_ptr->state.mf_reselect_count_upon_invalid_fcp_template++;
    if(uim_ptr->state.mf_reselect_count_upon_invalid_fcp_template < UIM_MF_RESELECT_COUNT_UPON_INVALID_FCP_TEMPLATE)
  {
    uim_ptr->state.mf_selection.required = TRUE;
    uim_ptr->state.mf_selection.channel  = uim_ptr->command.cmd_ptr->hdr.channel;
  }
  }

  if (num_bytes > UIM_MAX_CHARS)
  {
    UIMDRV_MSG_LOW_0(uim_ptr->id,
                     "uim_parse_uicc_status_response cmd rsp size is greater than rsp buf size");
    return;
  }

  /* Process all the bytes in the receive FIFO */
  while (i < num_bytes)
  {
    switch (data[i++])
    {
      case UIM_FCP_TEMPLATE_TAG:
        i++;  /* Move past the length field */
        break;

      case UIM_FILE_DESCRIPTOR_TAG:
        if (i >= num_bytes)
        {
          return;
        }
        /* store the length to advance */
        j = data[i++];
        if ((i + j) >= num_bytes)
        {
          return;
        }
        switch (data[i] & UIM_UICC_EF_STRUCT_MASK)
        {
          case UIM_UICC_TRANSPARENT_EF:
          case UIM_UICC_LINEAR_EF:
          case UIM_UICC_CYCLIC_EF:
            if(ef_rsp_ptr != NULL)
            {
              *ef_rsp_ptr = TRUE;
            }
            break;
          default:
            break;
        }
        switch (data[i] & UIM_UICC_EF_STRUCT_MASK)
        {
          case UIM_UICC_LINEAR_EF:
          case UIM_UICC_CYCLIC_EF:
            if(num_apps_ptr != NULL && (i + 4) <= num_bytes)
            {
              num_apps_ptr->record_len = data[i + 3];
              num_apps_ptr->num_apps_present = data[i + 4];
            }
            break;
          default:
            break;
        }        
        /* Skip j bytes */
        i+=j;
        break;

      case UIM_PROPRIETARY_TAG:
        if (i >= num_bytes)
        {
          return;
        }
        k = i + data[i];
        i++;
        while(i < k)
        {
          if (i >= num_bytes)
          {
            return;
          }
          switch(data[i++])
          {
            case UIM_CHARACTERISTICS_TAG:
            /* check here if i is a good index for "j" and the file_char */
            if ((i + 1) >= num_bytes)
            {
              return;
            }
            j = data[i++];
            /* Get the file characteristics byte */
            if (i >= num_bytes)
            {
              return;
            }
            if(file_char != NULL)
            {
              *file_char = data[i];
            }
            else
            {
              UIMDRV_MSG_LOW_0(uim_ptr->id,"uim_parse_uicc_select_response data is NULL");
              return;
            }
            i+=j;
            break;

            case UIM_SUPPORTED_CMD_TAG:
              /* check here if i is a good index for "j" and supported_cmd_present */
              if ((i + 1) >= num_bytes)
              {
                return;
              }

              j = data[i++];

              if (i >= num_bytes)
              {
                return;
              }
              if(supported_cmd_present != NULL)
              {
                *supported_cmd_present = data[i];
              }
              i+=j;
              break;

            default:
              if (i >= num_bytes)
              {
                return;
              }
              j = data[i++];
              i+=j;
              break;
          }
        }
        break;

      case UIM_FILE_ID_TAG:
      case UIM_FILE_SIZE_TAG:
      case UIM_DF_NAME_TAG:
      case UIM_SECURITY_ATTR_TAG1:
      case UIM_SECURITY_ATTR_TAG2:
      case UIM_SECURITY_ATTR_TAG3:
      case UIM_SECURITY_ATTR_TAG4:
      case UIM_LIFE_CYCLE_STATUS_TAG:
      case UIM_TOTAL_FILE_SIZE_TAG:
      case UIM_SFI_TAG:
      /* Ignore the tags that are not needed */
      default:
        if (i >= num_bytes)
        {
          return;
        }     /* Get the length */
        j = data[i++];
        /* Skip j bytes */
        i+=j;
        break;
    }
  }
} /* uim_parse_uicc_select_response */


/*===========================================================================

FUNCTION UIM_PARSE_STATUS_RESPONSE

DESCRIPTION
  This procedure parses the UICC response to a STATUS command

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Fills the data into the parameters provided.
===========================================================================*/
static void uim_parse_uicc_status_response
(
  byte const *data,
  word num_bytes,
  uim_dir_type *file_id,
  uim_aid_type *aid,
  uim_instance_global_type *uim_ptr
)
{
  uint16 i = 0; /* index variable */
  uint16 j = 0; /* Temp storage for length */

  if(file_id == NULL || aid == NULL)
  {
    return;
  }

  if (num_bytes > UIM_MAX_CHARS)
  {
    UIMDRV_MSG_LOW_0(uim_ptr->id,
                     "uim_parse_uicc_status_response cmd rsp size is greater than rsp buf size");
    return;
  }

  /* Process all the bytes in the receive FIFO */
  while (i < num_bytes)
  {
     if (data == NULL)
     {
       UIMDRV_MSG_LOW_0(uim_ptr->id,"uim_parse_uicc_status_response data is NULL");
       return;
     }
     switch (data[i++])
     {
       case UIM_FCP_TEMPLATE_TAG:
         i++;  /* Move past the length field */
       break;

       case UIM_DF_NAME_TAG:
       {
         if (i >= num_bytes)
         {
           return;
         }
         /* Store the length of the aid data */
         aid->aid_length = data[i++];
         if((aid->aid_length + i) > num_bytes)
         {
           return;
         }
         uim_memscpy( aid->data, sizeof(aid->data),data+i, aid->aid_length);
         i+= aid->aid_length;
       }
       break;

       case UIM_FILE_ID_TAG:
       {
         if (i >= num_bytes)
         {
           return;
         }
         /* Move past the length field */
         j = data[i++];
         if ((i + j) > num_bytes)
         {
           return;
         }
         /* Get the file id */
         *file_id = (word)FLIPW(data+i);
         /* Move past the file id */
         i+=j;
       }
       break;

       /* Ignore the remaining tags */
       default:
       {
         if (i >= num_bytes)
         {
           return;
         }     /* Get the length */
         j = data[i++];
         /* Skip j bytes */
         i+=j;
       }
       break;
    } /* switch */
  }  /* while loop */
} /* uim_parse_status_response */


/*===========================================================================

FUNCTION UIM_UPDATE_CURRENT_DIR_UICC

DESCRIPTION
  Update the current directory after a UICC select command

===========================================================================*/
static void uim_update_current_dir_uicc
(
  uim_instance_global_type *uim_ptr,
  boolean                   is_ef,
  uim_select_cmd_type      *sel_cmd_ptr,
  uim_channel_type          curr_ch
)
{
  uim_print_curr_path_ext(&uim_ptr->state.current_path[curr_ch].path,uim_ptr);


  uim_ptr->state.current_path[curr_ch].path  = sel_cmd_ptr->path;
  uim_ptr->state.current_path[curr_ch].is_ef = is_ef;

  UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_30,uim_ptr->id, "Path after on channel 0x%x:", curr_ch);
  uim_print_curr_path(&uim_ptr->state.current_path[curr_ch].path,uim_ptr);

  if(UIM_ADF == uim_ptr->command.cmd_ptr->select.file_type)
  {
    if(uim_ptr->command.cmd_ptr->select.session == UIM_TERMINATION)
    {
      /* Clear the current path structure - intentionally overwriting the path we set above
         This returns each state(is_ef, aid, path and empty app) to its default/empty state */
      uim_clear_current_path(uim_ptr, curr_ch);
      if(0 == curr_ch)
      {
        /* This is to make sure MF is selected on channel0
           after the previous path is cleared */
        uim_ptr->state.mf_selection.required = TRUE;
        uim_ptr->state.mf_selection.channel  = 0;
      }
    }
    else
    {
      uim_ptr->state.current_path[curr_ch].curr_app = sel_cmd_ptr->aid;
      if(sel_cmd_ptr->aid.aid_length == 0)
      {
        uim_ptr->state.current_path[curr_ch].empty_aid_is_active = TRUE;
      }
    }
  }
} /* uim_update_current_dir_uicc */


/*===========================================================================

FUNCTION UIM_UPDATE_CURRENT_DIR_ICC

DESCRIPTION
  Update the current directory after an ICC select command

===========================================================================*/
static void uim_update_current_dir_icc
(
  uim_instance_global_type *uim_ptr,
  boolean                   is_ef,
  uim_path_type            *path_ptr,
  uint8                     path_position
)
{
  int i = 0;

  uim_print_curr_path_ext(&uim_ptr->state.current_path[0].path,uim_ptr);

  memset(&uim_ptr->state.current_path[0].path, 0x00, sizeof(uim_path_type));

  uim_ptr->state.current_path[0].path.len = 0;
  for(i = 0; i < path_position; i++)
  {
    uim_ptr->state.current_path[0].path.path[i] = path_ptr->path[i];
    uim_ptr->state.current_path[0].path.len++;
  }
  uim_ptr->state.current_path[0].is_ef = is_ef;

  uim_print_curr_path_ext(&uim_ptr->state.current_path[0].path,uim_ptr);
} /* uim_update_current_dir_icc */


/*===========================================================================

FUNCTION UIM_UPDATE_CURRENT_DIR_SFI

DESCRIPTION
  Update the current directory after an SFI command

===========================================================================*/
static void uim_update_current_dir_sfi
(
  uim_instance_global_type *uim_ptr,
  uim_path_type            *path_ptr,
  uim_channel_type          curr_ch
)
{
  uim_print_curr_path_ext(&uim_ptr->state.current_path[curr_ch].path,uim_ptr);

  uim_ptr->state.current_path[curr_ch].path  = *path_ptr;
  uim_ptr->state.current_path[curr_ch].is_ef = TRUE;

  uim_print_curr_path_ext(&uim_ptr->state.current_path[curr_ch].path,uim_ptr);
} /* uim_update_current_dir_sfi */


/*===========================================================================
FUNCTION UIM_COMPARE_TOOLKIT_CMDS

DESCRIPTION
  UIM Action function callback that is called by q_linear_delete when the
  compare callback function returns TRUE. This function will return TRUE only
  when the command id is that of a Terminal Response.
  These commands need to be removed from UIM queue before starting recovery.

PARAMETER
  item_ptr  [Input]:  ptr to queue item
  param_ptr [Input]:  no use as of now. This will be populated as NULL
                      by q_linear_delete()

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if there is a match
  FALSE - if there is no match

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
int uim_compare_toolkit_cmds
(
  void *item_ptr,
  void *param_ptr
)
{
  uim_cmd_type             *cmd_ptr          = NULL;

  if(item_ptr == NULL)
  {
    return 0;
  }
  cmd_ptr = (uim_cmd_type*)item_ptr;

  /* Clear Terminal Response commands sent by GSTK before recovery */

  switch((uim_cmd_name_type)cmd_ptr->hdr.command)
  {
    case UIM_TERMINAL_RESPONSE_F:
    case UIM_ENVELOPE_F:
    case UIM_SILENT_RECOVER_UICC_F:
      return 1;
    default:
      return 0;
  }/* switch */

  return 0;
} /* uim_compare_toolkit_cmds */


/*===========================================================================
FUNCTION UIM_REMOVE_TOOLKIT_CMDS

DESCRIPTION
  UIM Action function callback that is called by q_linear_delete when the
  compare callback function returns TRUE. This function puts the command
  removed from the command queue into the free queue.

PARAMETER
  item_ptr  [Input]:  ptr to queue item
  param_ptr [Input]:  no use as of now. This will be populated as NULL
                      by q_linear_delete()

DEPENDENCIES
  None

RETURN VALUE
  void

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
void uim_remove_toolkit_cmds
(
  void *item_ptr,
  void *param_ptr
)
{
  uim_cmd_type          *cmd_ptr      = NULL;
  uim_instance_global_type *uim_ptr   = NULL;

  if(item_ptr == NULL)
  {
    UIM_MSG_HIGH_0("item_ptr is NULL");
    return;
  }
  if(param_ptr == NULL)
  {
    return;
  }

  cmd_ptr = (uim_cmd_type*)item_ptr;
  uim_ptr = (uim_instance_global_type*)param_ptr;

  /* return an invalid status in the header to all the commands */
  cmd_ptr->hdr.status = UIM_ERR_S;
  if (cmd_ptr->hdr.cmd_hdr.task_ptr != NULL)
  {
    (void) rex_set_sigs( cmd_ptr->hdr.cmd_hdr.task_ptr,
                             cmd_ptr->hdr.cmd_hdr.sigs );
  }

  /* If always reporting & have a report function, report failure */
  if (((int)cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0)
  {
    uim_send_generic_err_rpt(cmd_ptr, uim_ptr);
  }
  else
  {  /* return buffer to free queue */
    UIMDRV_MSG_HIGH_1(uim_ptr->id, "Not sending  error report for toolkit command 0x%x",cmd_ptr->hdr.command);
    if ( cmd_ptr->hdr.cmd_hdr.done_q_ptr != NULL )
    {
      /* place command on requested queue */
      q_put( cmd_ptr->hdr.cmd_hdr.done_q_ptr,
             &cmd_ptr->hdr.cmd_hdr.link );
     }
  }
} /* uim_remove_toolkit_cmds */


/*===========================================================================
FUNCTION UIM_SAVE_CURRENT_REQ_BUF

DESCRIPTION
  This function saves the current request buffer.

PARAMETER
  None

DEPENDENCIES
  None

RETURN VALUE
  void

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
void uim_save_current_req_buf(uim_instance_global_type *uim_ptr)
{
  uim_ptr->state.saved_req_buf.apdu_hdr.uim_class = uim_ptr->command.req_buf.apdu_hdr.uim_class;
  uim_ptr->state.saved_req_buf.apdu_hdr.instrn    = uim_ptr->command.req_buf.apdu_hdr.instrn;
  uim_ptr->state.saved_req_buf.instrn_case        = uim_ptr->command.req_buf.instrn_case;
  uim_ptr->state.saved_req_buf.apdu_hdr.p1        = uim_ptr->command.req_buf.apdu_hdr.p1;
  uim_ptr->state.saved_req_buf.apdu_hdr.p2        = uim_ptr->command.req_buf.apdu_hdr.p2;
  uim_ptr->state.saved_req_buf.apdu_hdr.p3        = uim_ptr->command.req_buf.apdu_hdr.p3;
} /* uim_save_current_req_buf */


/*===========================================================================
FUNCTION UIM_RESTORE_CURRENT_REQ_BUF

DESCRIPTION
  This function restores the current request buffer.

PARAMETER
  None

DEPENDENCIES
  None

RETURN VALUE
  void

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
void uim_restore_current_req_buf(uim_instance_global_type *uim_ptr)
{
  uim_ptr->command.req_buf.apdu_hdr.uim_class = uim_ptr->state.saved_req_buf.apdu_hdr.uim_class;
  uim_ptr->command.req_buf.apdu_hdr.instrn    = uim_ptr->state.saved_req_buf.apdu_hdr.instrn;
  uim_ptr->command.req_buf.instrn_case        = uim_ptr->state.saved_req_buf.instrn_case;
  uim_ptr->command.req_buf.apdu_hdr.p1        = uim_ptr->state.saved_req_buf.apdu_hdr.p1;
  uim_ptr->command.req_buf.apdu_hdr.p2        = uim_ptr->state.saved_req_buf.apdu_hdr.p2;
  uim_ptr->command.req_buf.apdu_hdr.p3        = uim_ptr->state.saved_req_buf.apdu_hdr.p3;
} /* uim_restore_current_req_buf */


/*===========================================================================

FUNCTION UIM_CALCULATE_GUARD_TIME

DESCRIPTION
  This procedure calculates the guard time.

DEPENDENCIES

RETURN VALUE
  Guard time

SIDE EFFECTS

===========================================================================*/
uint8 uim_calculate_guard_time(uim_instance_global_type *uim_ptr)
{
  uint16 uim_guard_time     = 0;
  uint8  uim_min_guard_time = 0;

  uim_min_guard_time = UIM_MINIMUM_GUARD_TIME;

  /* TC1 byte is present in ATR */
  if(uim_ptr->atr.atr_buf.data[UIM_ATR_T0_BYTE] & (UIM_ATR_TC_PRESENT))
  {
    /* If TC1 byte is 0 or 255, only minimum guard time is possible */
    if( uim_ptr->atr.atr_tc1_byte == UIM_TC1_IS_0 ||
        uim_ptr->atr.atr_tc1_byte == UIM_TC1_IS_255 )
    {
      uim_guard_time = uim_min_guard_time;
    }
    else /* TC1 value is added to the minimum guard time */
    {
      uim_guard_time = uim_min_guard_time + uim_ptr->atr.atr_tc1_byte;
    }
  }
  else /* If tc1 byte is absent, only minimum guard time is possible */
  {
    uim_guard_time = uim_min_guard_time;
  }

  /* !! UART constraint: UART supports only 254 etus as guard time !! */
  if(uim_guard_time > UIM_MAX_POSSIBLE_GUARD_TIME)
  {
    uim_guard_time = UIM_MAX_POSSIBLE_GUARD_TIME;
  }

  return (uint8)uim_guard_time;
} /* uim_calculate_guard_time */


/*===========================================================================

FUNCTION UIM_STREAM_APDU_STATE

DESCRIPTION
  This procedure executes the stream apdu state

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void uim_stream_apdu_state
(
  uim_instance_global_type *uim_ptr,
  uim_req_buf_type         *uim_req_buf_static_ptr
)
{
  uint16            apdu_data_len       = 0;  
      
  if (uim_ptr->command.cmd_ptr->hdr.command == UIM_STREAM_APDU_F)
  {
    /* Build an APDU to stream to the card */
    uim_req_buf_static_ptr->apdu_hdr.instrn = uim_ptr->command.cmd_ptr->stream_apdu.instrn;
    uim_req_buf_static_ptr->apdu_hdr.uim_class = uim_ptr->command.cmd_ptr->stream_apdu.uim_class;
    uim_req_buf_static_ptr->apdu_hdr.p1 = uim_ptr->command.cmd_ptr->stream_apdu.p1;
    uim_req_buf_static_ptr->apdu_hdr.p2 = uim_ptr->command.cmd_ptr->stream_apdu.p2;
    uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->stream_apdu.p3;

    /* We are ONLY going to deal with APDUs that have cmd data for now */
    uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;

    if (uim_ptr->command.cmd_ptr->stream_apdu.p3 != 0)
    {
      uim_memscpy(uim_req_buf_static_ptr->req.data,
             sizeof(uim_req_buf_static_ptr->req.data),
             uim_ptr->command.cmd_ptr->stream_apdu.data,
             uim_ptr->command.cmd_ptr->stream_apdu.p3);
    }
  }
  else
  {
    /* If P3 byte is ZERO then calculate the length of extended APDU data*/
    if ( uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE2_7816_LE_OFFSET] == 0)
    {
      apdu_data_len = uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE3_4_7816_LC_OFFSET+1] << 8;
      apdu_data_len |= uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE3_4_7816_LC_OFFSET+2];      
    }

    /* Init the status words */
    uim_ptr->card_cmd.get_resp_sw1 = (uim_sw1_type) 0;
    uim_ptr->card_cmd.get_resp_sw2 = (uim_sw2_type) 0;

    /* Init the get resp status words */
    uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw1 = uim_ptr->card_cmd.get_resp_sw1;
    uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw2 = uim_ptr->card_cmd.get_resp_sw2;

    /* Build an APDU to stream to the card */
    uim_req_buf_static_ptr->apdu_hdr.uim_class =
      uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_CLASS_OFFSET];
    uim_req_buf_static_ptr->apdu_hdr.instrn =
      uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_INSTRN_OFFSET];
    uim_req_buf_static_ptr->apdu_hdr.p1 =
      uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_P1_OFFSET];
    uim_req_buf_static_ptr->apdu_hdr.p2 =
      uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_P2_OFFSET];
    
    if (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes > UIM_MAX_STREAM_ISO7816_APDU_TRANSACTION_SIZE)
    {
      uint16   remaining_byte_need_to_send = 0;

      uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_4;

      /* Extended APDU envelope */
      uim_req_buf_static_ptr->apdu_hdr.uim_class =
        uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_CLASS_OFFSET] & 0x0F;
      uim_req_buf_static_ptr->apdu_hdr.instrn    = ENVELOPE;
      uim_req_buf_static_ptr->apdu_hdr.p1        = 0x00;
      uim_req_buf_static_ptr->apdu_hdr.p2        = 0x00;

      remaining_byte_need_to_send = uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes - uim_ptr->card_cmd.ext_cmd_data_offset;

      if(remaining_byte_need_to_send >= 0xFF)
      {
        uim_req_buf_static_ptr->apdu_hdr.p3 = 0xFF;
      }
      else if(remaining_byte_need_to_send == 0)
      {
        uim_req_buf_static_ptr->apdu_hdr.p3 = 0x00;
        uim_ptr->card_cmd.is_ext_cmd_transaction_done = TRUE;        
      }
      else if(remaining_byte_need_to_send < 0xFF)
      {
        uim_req_buf_static_ptr->apdu_hdr.p3 = remaining_byte_need_to_send;
      }

      /* Copy the command bytes into the request buffer */
      uim_memscpy(uim_req_buf_static_ptr->req.data,
             sizeof(uim_req_buf_static_ptr->req.data),
             uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data + uim_ptr->card_cmd.ext_cmd_data_offset,
             uim_req_buf_static_ptr->apdu_hdr.p3);
      uim_ptr->card_cmd.ext_cmd_data_offset += uim_req_buf_static_ptr->apdu_hdr.p3;
    }    
    /* Figure out the case for the commands */
    else if (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes ==
                                               UIM_CASE1_7816_APDU_SIZE)
    {
      /* No input / no output */
      uim_req_buf_static_ptr->apdu_hdr.p3 = 0;
      /*  Marking case as case 2 because we always transmit p3 as 0x0 hence
          making it a de facto case 2. And in certain cases such as non-telecom
          apps it is possible that we get a 0x6C 0xXX response even for
          a case 1 command so for the response handling to occur correctly
          marking case as case 2*/
      uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;

    }
    else if ((uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes == UIM_CASE2_7816_APDU_SIZE) ||
             (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes == UIM_CASE2_7816_APDU_SIZE + 2 &&
              uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE2_7816_LE_OFFSET] == 0))
    {
      /* Case 2 - no input / output of known or unknown length */
      uim_req_buf_static_ptr->apdu_hdr.p3 =
        uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE2_7816_LE_OFFSET];
      uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_2;
      uim_ptr->card_cmd.stream_apdu_rpt_data_length = uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[
                    (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes - 1)] ;
                    
      if((uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes == UIM_CASE2_7816_APDU_SIZE + 2 &&
                uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE2_7816_LE_OFFSET] == 0))
      {
        if(apdu_data_len > 0xFF)
        {
          uim_req_buf_static_ptr->apdu_hdr.p3 = 0x00;
        }
        else
        {
          uim_req_buf_static_ptr->apdu_hdr.p3 = (uint8)apdu_data_len;        
        }
        uim_ptr->card_cmd.stream_apdu_rpt_data_length = apdu_data_len;
      }

      if(uim_ptr->card_cmd.stream_apdu_rpt_data_length == 0)
      {
        uim_ptr->card_cmd.stream_apdu_rpt_data_length  = 0xFFFF;
      }

      UIMDRV_MSG_HIGH_1(uim_ptr->id, "For case 2 command - value of the rpt_data_length is 0x%x",
                                             uim_ptr->card_cmd.stream_apdu_rpt_data_length );
    }
    else if ((uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes ==
             (UIM_CASE2_7816_APDU_SIZE + uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE3_4_7816_LC_OFFSET])) ||             
             (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE2_7816_LE_OFFSET] == 0 && 
              uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes == apdu_data_len + UIM_CASE2_7816_APDU_SIZE + 2))
    {
      /* Case 3 - input / no ouput */
      uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data
                                             [UIM_CASE3_4_7816_LC_OFFSET];

      /* Copy the command bytes into the request buffer */
      uim_memscpy(uim_req_buf_static_ptr->req.data,
             sizeof(uim_req_buf_static_ptr->req.data),
             uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data +
                                                UIM_CASE2_7816_APDU_SIZE,
             uim_ptr->command.cmd_ptr-> stream_iso7816_apdu.cmd_data
                                          [UIM_CASE3_4_7816_LC_OFFSET]);
                                          
      if ( uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE2_7816_LE_OFFSET] == 0)
      {
        if (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes == (UIM_CASE2_7816_APDU_SIZE + apdu_data_len + 2))
        {
          uim_req_buf_static_ptr->apdu_hdr.p3 = (uint8)apdu_data_len;
          /* Copy the command bytes into the request buffer */
          uim_memscpy(uim_req_buf_static_ptr->req.data,
                 sizeof(uim_req_buf_static_ptr->req.data),
                 uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data + UIM_CASE2_7816_APDU_SIZE + 2,
                 apdu_data_len);          
        }
      }

      /*For SAM card set the instruction case to 3*/
      if(UIM_CARD_SAM  == uim_ptr->card_mode)
      {
        uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;
      }
      /* SELECT will be a case-4 instruction if it requests for
         FCP template */
      else if(uim_req_buf_static_ptr->apdu_hdr.instrn == SELECT)
      {
        if (uim_req_buf_static_ptr->apdu_hdr.p2 == UIM_UICC_RETURN_NO_DATA)
        {
          /* Case 3 - input / output of known or unknown length */
          uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;
        }
        else
        {
          /* Case 4 for select case */
          uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_4;
        }
      } /* Non-SELECT APDUs */
      else if (STATUS == uim_req_buf_static_ptr->apdu_hdr.instrn)
      {
        /* Always allow GetStatus as Case4 command */
        uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_4;
      }/* GET STATUS APDUs */
      else
      {
        uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_3;
      }
      UIMDRV_MSG_MED_1(uim_ptr->id, "For instruction case 3 - value of the rpt_data_length is 0x%x",
                                             uim_ptr->card_cmd.stream_apdu_rpt_data_length );
      
    }
    else
    {
      /* Case 4 - input / output of known or unknown length */
      uim_req_buf_static_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data
                                             [UIM_CASE3_4_7816_LC_OFFSET];

      uim_ptr->card_cmd.stream_apdu_rpt_data_length = uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[
                    (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes - 1)] ;

      /* Copy the command bytes into the request buffer */
      uim_memscpy(uim_req_buf_static_ptr->req.data,
             sizeof(uim_req_buf_static_ptr->req.data),
             uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data +
                                                UIM_CASE2_7816_APDU_SIZE,
             uim_ptr->command.cmd_ptr-> stream_iso7816_apdu.cmd_data
                                          [UIM_CASE3_4_7816_LC_OFFSET]);

      if (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_CASE2_7816_LE_OFFSET] == 0)
      {
        uim_req_buf_static_ptr->apdu_hdr.p3 = (uint8)apdu_data_len;

        uim_ptr->card_cmd.stream_apdu_rpt_data_length = 
                uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[(uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes - 2)] << 8;
        uim_ptr->card_cmd.stream_apdu_rpt_data_length |= 
                uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[(uim_ptr->command.cmd_ptr->stream_iso7816_apdu.num_cmd_bytes - 1)];      
        
        /* Copy the command bytes into the request buffer */
        uim_memscpy(uim_req_buf_static_ptr->req.data,
               sizeof(uim_req_buf_static_ptr->req.data),
               uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data + UIM_CASE2_7816_APDU_SIZE + 2,
               apdu_data_len);            
      }                                            

      uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_4;

      UIMDRV_MSG_MED_1(uim_ptr->id, "For instruction case 4 - value of the rpt_data_length is 0x%x",
                                             uim_ptr->card_cmd.stream_apdu_rpt_data_length );
      if(uim_ptr->card_cmd.stream_apdu_rpt_data_length == 0)
      {
        uim_ptr->card_cmd.stream_apdu_rpt_data_length  = 0xFFFF;
      }
    }
  }

  /*For SAM card no protcol settign is not required*/
  if(UIM_CARD_TELECOM  == uim_ptr->card_mode)
  {
    /* Set the protocol based on the class of the command */
    if (uim_req_buf_static_ptr->apdu_hdr.uim_class != UIM_INSTRUCTION_CLASS)
    {
      uim_req_buf_static_ptr->protocol = UIM_UICC;
      uim_ptr->command.cmd_ptr->hdr.protocol = UIM_UICC;
    }
    else
    {
      uim_req_buf_static_ptr->protocol = UIM_ICC;
      uim_ptr->command.cmd_ptr->hdr.protocol = UIM_ICC;
    }
  }
  /* Set rd wr buffer length to zero */
  uim_ptr->command.rd_wr_info.num_bytes = 0;
  uim_ptr->command.rd_wr_info.num_bytes_rsp = 0;

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uim_stream_apdu_state */


/*===========================================================================

FUNCTION UIM_GET_START_PATH_INDEX

DESCRIPTION
  This procedure gets the index we want to start at of the path to select

DEPENDENCIES
  Only used in ICC cases

RETURN VALUE
  The position of the new path to start selecting from

SIDE EFFECTS

===========================================================================*/
static uint8 uim_get_start_path_index
(
  uim_instance_global_type *uim_ptr,
  uim_path_type            *select_path_ptr
)
{
  uim_path_type curr_dir_path = {0};
  uim_path_type *curr_dir_path_ptr = &curr_dir_path;

  if(uim_ptr->state.current_path[0].is_ef)
  {
    uim_memscpy(curr_dir_path_ptr->path,
                sizeof(curr_dir_path_ptr->path),
                uim_ptr->state.current_path[0].path.path,
                (uim_ptr->state.current_path[0].path.len - 1) * 2);
    curr_dir_path_ptr->len = uim_ptr->state.current_path[0].path.len - 1;
  }
  else
  {
    uim_memscpy(curr_dir_path_ptr->path,
                sizeof(curr_dir_path_ptr->path),
                uim_ptr->state.current_path[0].path.path,
                uim_ptr->state.current_path[0].path.len * 2);
    curr_dir_path_ptr->len = uim_ptr->state.current_path[0].path.len;
  }


  /* if it's a select for MF or some path we don't
     recognize start at the beginning of the path */
  if(select_path_ptr->len <= 1 || curr_dir_path_ptr->len == 0)
  {
    return 0;
  }

  /* current directory is MF start selecting from
     the second select path element */
  if(curr_dir_path_ptr->len == 1)
  {
    return 1;
  }

  /* at this point the curr dir path length and the
     select path length are both at least two */

  /* is the target a child of the current directory
     or is it the current directory */
  if(memcmp(select_path_ptr->path,
            curr_dir_path_ptr->path,
            (curr_dir_path_ptr->len * 2)) ==0)
  {
    if(select_path_ptr->len > curr_dir_path_ptr->len)
    {
      return curr_dir_path_ptr->len;
    }
    else if(select_path_ptr->len == curr_dir_path_ptr->len)
    {
      return curr_dir_path_ptr->len - 1;
    }
    /* if we reach here the select path is shorter than the current directory
       path but somehow the memcmp returned true - should not be possible */
  }
  /* Is the next select a DF that is a child of the current
     selected DF's parent (sibling of the current DF)
     An explicit select for a sibling directory would be the same length
     as the current dir path length, however we don't know before issuing
     a select if it's a DF or EF - EF selection is not allowed this way
     so we can only issue the select this way if we know it's a DF(additional
     path elements after the sibling element assures us it's a DF)*/
  if(select_path_ptr->len > curr_dir_path_ptr->len &&
     memcmp(select_path_ptr->path,
            curr_dir_path_ptr->path,
            (curr_dir_path_ptr->len - 1) * 2) == 0)
  {
    return curr_dir_path_ptr->len - 1;
  }
  /* Does it make sense to select the parent directory or
     is the select explicitly for the parent dir */
  if((select_path_ptr->len == curr_dir_path_ptr->len ||
      select_path_ptr->len == curr_dir_path_ptr->len - 1) &&
     memcmp(select_path_ptr->path,
            curr_dir_path_ptr->path,
            (curr_dir_path_ptr->len - 1) *2) == 0)
  {
    return curr_dir_path_ptr->len - 2;
  }

  /* we found no clever way for selecting so select from MF */
  return 0;
} /* uim_get_start_path_index */


/*===========================================================================

FUNCTION UIM_SELECT_STATE

DESCRIPTION
  This procedure executes the select state

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void uim_select_state
(
  uim_instance_global_type *uim_ptr,
  uim_path_type            *target_path_ptr,
  uim_req_buf_type         *uim_req_buf_ptr
)
{
  uint8 session_type = UIM_ACTIVATION_WITH_NO_DATA;
  uint8 i            = 0;
  uint8 j            = 0;

  if(uim_ptr == NULL || target_path_ptr == NULL || uim_req_buf_ptr == NULL)
  {
    return;
  }

  uim_req_buf_ptr->apdu_hdr.instrn = SELECT;
  uim_req_buf_ptr->instrn_case     = UIM_INSTRN_CASE_4;

/* ICC CODE */
  if(uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC)
  {
    if(uim_ptr->command.cmd_ptr->select.path_position == 0)
    {
      uim_ptr->command.cmd_ptr->select.path_position = uim_get_start_path_index(uim_ptr, target_path_ptr);
    }
    uim_req_buf_ptr->apdu_hdr.uim_class     = UIM_INSTRUCTION_CLASS;
    uim_req_buf_ptr->apdu_hdr.p1            = 0;
    uim_req_buf_ptr->apdu_hdr.p2            = 0;
    uim_req_buf_ptr->apdu_hdr.p3            = 2;

    uim_req_buf_ptr->req.mf_df_ef.high_byte = WORD_HI (target_path_ptr->path[uim_ptr->command.cmd_ptr->select.path_position]);
    uim_req_buf_ptr->req.mf_df_ef.low_byte  = WORD_LO (target_path_ptr->path[uim_ptr->command.cmd_ptr->select.path_position]);

    uim_req_buf_ptr->rsp_ptr                = &uim_ptr->command.rsp_buf;

    uim_send_command(uim_req_buf_ptr, uim_ptr);
    return;
  }
/* ICC CODE since we return in the ICC code everything below only is executed for UICC case */

  uim_req_buf_ptr->apdu_hdr.uim_class =
    uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);

  /* if it's a select MF do this */
  if(target_path_ptr->path[0] == 0x3F00 &&
     target_path_ptr->len     == 1)
  {
    uim_req_buf_ptr->apdu_hdr.p1 = UIM_USIM_SELECT_BY_FILE_ID;
    uim_req_buf_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_FCP_DATA;
    uim_req_buf_ptr->req.data[0] = WORD_HI(target_path_ptr->path[0]);
    uim_req_buf_ptr->req.data[1] = WORD_LO(target_path_ptr->path[0]);
    uim_req_buf_ptr->apdu_hdr.p3 = 2;
  } /* SELECT MF */
  else if(UIM_ADF == uim_ptr->command.cmd_ptr->select.file_type)
  {
    uim_req_buf_ptr->apdu_hdr.p1 = UIM_USIM_SELECT_BY_DF_NAME;

    session_type = uim_ptr->command.cmd_ptr->select.session;

    /* Bit 2, 3 and 4 of P2 determine the type of select response
       solicited from the card. Based on type of activation the
       bit pattern for P2 is setup */
    switch(session_type)
    {
      case UIM_ACTIVATION_WITH_NO_DATA:
        uim_req_buf_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_NO_DATA;
        break;
      case UIM_ACTIVATION_WITH_FMD:
        uim_req_buf_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_FMD_DATA;
        break;
      case UIM_ACTIVATION_WITH_FCI:
        uim_req_buf_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_FCI_DATA;
        break;
      case UIM_ACTIVATION_WITH_FCI_INTERFACES:
        uim_req_buf_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_FCI_W_INTERFACE;
        break;
      case UIM_ACTIVATION_OR_RESET:
        uim_req_buf_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_FCP_DATA;
        break;
      case UIM_TERMINATION:
        uim_req_buf_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_NO_DATA | UIM_USIM_SESSION_TERMINATION;
        break;
      /* No default case to ensure all types requested types are addressed */
    }
    /* Select Next on partial Aid */
    if(UIM_NEXT_OCCURRENCE == uim_ptr->command.cmd_ptr->select.adf_selection)
    {
      uim_req_buf_ptr->apdu_hdr.p2 |= UIM_NEXT_OCCURRENCE;
    }
    else if(UIM_LAST_OCCURRENCE == uim_ptr->command.cmd_ptr->select.adf_selection)
    {
      uim_req_buf_ptr->apdu_hdr.p2 |= UIM_LAST_OCCURRENCE;
    }
    else if(UIM_PREVIOUS_OCCURRENCE == uim_ptr->command.cmd_ptr->select.adf_selection)
    {
      uim_req_buf_ptr->apdu_hdr.p2 |= UIM_PREVIOUS_OCCURRENCE;
    }
    else
    {
      /* Bits 1 and 2 of P2 are setup for first occurrence */
      uim_req_buf_ptr->apdu_hdr.p2 |= UIM_FIRST_OR_ONLY_OCCURRENCE;
    }
    uim_memscpy(uim_req_buf_ptr->req.data,
                sizeof(uim_req_buf_ptr->req.data),
                uim_ptr->command.cmd_ptr->select.aid.data,
                uim_ptr->command.cmd_ptr->select.aid.aid_length);

    uim_req_buf_ptr->apdu_hdr.p3 = uim_ptr->command.cmd_ptr->select.aid.aid_length;

    /* Check for empty AID and  */
    if (uim_req_buf_ptr->apdu_hdr.p3 == 0)
    {
      uim_req_buf_ptr->instrn_case = UIM_INSTRN_CASE_2;
    }
  } /* Select by AID */
  else
  {
    uim_req_buf_ptr->apdu_hdr.p1 = UIM_USIM_SELECT_BY_PATH_FROM_MF;
    uim_req_buf_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_FCP_DATA;
    uim_req_buf_ptr->apdu_hdr.p3 = 0;

    if(target_path_ptr->len > UIM_DIRMAIN_MAX_PATH_ELEMENTS)
    {
      /* target_path_ptr max length is 5 */
      UIMDRV_MSG_LOW_0(uim_ptr->id, "Target path lengh is greater than max 5. Setting to 5");
      target_path_ptr->len = UIM_DIRMAIN_MAX_PATH_ELEMENTS;
    }

    for(i = 1, j = 0; i < target_path_ptr->len; i++)
    {
      uim_req_buf_ptr->req.data[j++] = WORD_HI(target_path_ptr->path[i]);
      uim_req_buf_ptr->req.data[j++] = WORD_LO(target_path_ptr->path[i]);

      /* Set the param3 as the total number of bytes */
      uim_req_buf_ptr->apdu_hdr.p3 += 2;
    }
  } /* UICC Select - performed by path */

  uim_req_buf_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_ptr, uim_ptr);
} /* uim_select_state */


/*===========================================================================

FUNCTION UIM_PREPARE_SELECT_FAILED_ST

DESCRIPTION
  This procedure saves the current select results(which will be
  restored in the SELECT_FAILED_ST response) and sets up a new
  select command to select the last successfully selected directory

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void uim_prepare_select_failed_st
(
  uim_instance_global_type *uim_ptr,
  uim_channel_type          curr_ch,
  uim_apdu_status_type      cmd_status
)
{
  int i = 0;

  /* For buggy cards a MF select on channel 0 is performed
     if there is a AID selection on non-zero channel*/
  if( uim_ptr->setting.work_arounds_for_non_spec_cards
      & UIM_MF_SELECTION_AFTER_AID_SELECTION_FAILURE_ON_NON_ZERO_CHANNEL)
  {
    if ((UIM_ADF == uim_ptr->command.cmd_ptr->select.file_type) && ( 0 != curr_ch))
    {
      uim_ptr->state.mf_selection.required  = TRUE;
      uim_ptr->state.mf_selection.channel   = 0;
    }
  }


  if ( /*is workaround for aram app failure enabled*/
      (uim_ptr->setting.work_arounds_for_non_spec_cards
      & UIM_ADF_SELECTION_ON_OTHER_CHANNELS_UPON_ARAM_APP_FAILURE) &&
       /*is aram-app selection*/
      (UIM_ADF == uim_ptr->command.cmd_ptr->select.file_type) &&
      (uim_is_aram_aid(uim_ptr->command.cmd_ptr->select.aid)))
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"aram_app_selected failed, Queueing ADF/MF selection on the other channels");

    for(i=0; i<UIM_MAX_CHANNELS; i++)
    {
      if((uim_ptr->state.is_ch_open[i]== TRUE) &&
         (i != curr_ch))
      {
        if(0 != uim_ptr->state.current_path[i].curr_app.aid_length)
        {
          if(uim_is_telecom_aid(uim_ptr->state.current_path[i].curr_app))
          {
            uim_ptr->state.adf_selection.required  = TRUE;
            uim_ptr->state.adf_selection.channel   = i;
            uim_explicit_reselect_mf_aid(uim_ptr);
          }
        }
        else
        {
          uim_ptr->state.mf_selection.required  = TRUE;
          uim_ptr->state.mf_selection.channel   = i;
          uim_explicit_reselect_mf_aid(uim_ptr);
        }
      }
    }
  }

  if ((UIM_ADF == uim_ptr->command.cmd_ptr->select.file_type) &&
      (uim_is_aram_aid(uim_ptr->command.cmd_ptr->select.aid))
    )
  {
    uim_ptr->state.aram_app.select_aram_failed = TRUE;
    uim_ptr->state.aram_app.channel            = curr_ch;
  }

  if(
      /*if MF selection failed*/
      (1 == uim_ptr->command.cmd_ptr->select.path.len &&
       UIM_MF_ID ==uim_ptr->command.cmd_ptr->select.path.path[0]) ||
      /*if 7FFF selection failed*/
      (UIM_ADF != uim_ptr->command.cmd_ptr->select.file_type &&
       0 != uim_ptr->state.current_path[curr_ch].curr_app.aid_length &&
       (TRUE == uim_is_telecom_aid(uim_ptr->state.current_path[curr_ch].curr_app)) &&
       2 == uim_ptr->command.cmd_ptr->select.path.len &&
       UIM_MF_ID ==uim_ptr->command.cmd_ptr->select.path.path[0] &&
       UIM_SEL_AID_BY_FILE_ID == uim_ptr->command.cmd_ptr->select.path.path[1])
    )
  {
    /* If the external AUTH command is to be retried after 7FFF selection failure
       do not trigger recovery  */
    if(uim_ptr->flag.uim_do_not_flush_ext_cmd == FALSE)
    {
      UIMDRV_MSG_HIGH_0(uim_ptr->id,"triggering recovery as MF or 7FFF selection failed");
      (void)uim_trigger_silent_recovery((uim_slot_type)(uim_ptr->id+1),
                                        uim_ptr->command.cmd_ptr,
                                        NULL);
    }
    else
    {
      if(uim_ptr->setting.work_arounds_for_non_spec_cards & UIM_IMSI_SELECTION_BEFORE_AUTH_COMMAND)
      {
        uim_ptr->command.cmd_ptr->select.path.path[0] = 0x3F00;
        uim_ptr->command.cmd_ptr->select.path.path[1] = 0x7FFF;
        uim_ptr->command.cmd_ptr->select.path.path[2] = 0x6F07;
        uim_ptr->command.cmd_ptr->select.path.len     = 3;

        uim_ptr->command.cmd_ptr->select.file_type =
		    uim_get_file_type(uim_ptr->command.cmd_ptr->select.path);

        UIM_MSG_HIGH_0("Selecting EF-IMSI before AUTH CMD");
        return;
      }
    }
  }


  /* When a select fails try to reselect the last selected directory this
     is done in the select failed state - set up the select here and
     save the sw from the original failure to restore once we've reselected */
  uim_ptr->command.cmd_ptr->select.path_position = 0;

  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Saving the actual path received in external cmd");
  uim_ptr->command.cmd_ptr->select.reselect.original_select_path.len = uim_ptr->command.cmd_ptr->select.path.len;
  uim_ptr->command.cmd_ptr->select.reselect.original_file_type       = uim_ptr->command.cmd_ptr->select.file_type;
  for(i = 0; i < uim_ptr->command.cmd_ptr->select.path.len; i++)
  {
    uim_ptr->command.cmd_ptr->select.reselect.original_select_path.path[i] = uim_ptr->command.cmd_ptr->select.path.path[i];
  }

  for(i = 0; i < uim_ptr->state.current_path[curr_ch].path.len; i++)
  {
    uim_ptr->command.cmd_ptr->select.path.path[i] = uim_ptr->state.current_path[curr_ch].path.path[i];
  }
  uim_ptr->command.cmd_ptr->select.path.len = uim_ptr->state.current_path[curr_ch].path.len;

  uim_ptr->command.cmd_ptr->select.file_type =
  uim_get_file_type(uim_ptr->command.cmd_ptr->select.path);

  if(uim_ptr->state.current_path[curr_ch].path.len == 0)
  {
    uim_ptr->command.cmd_ptr->select.path.path[0]        = UIM_MF_ID;
    uim_ptr->command.cmd_ptr->select.path.len            = 1;
    uim_ptr->command.cmd_ptr->select.file_type           = UIM_MF;
    uim_ptr->command.cmd_ptr->select.aid.aid_length      = 0;
  }

  uim_ptr->command.cmd_ptr->select.reselect.is_failed_select_reselect = TRUE;
  uim_ptr->command.cmd_ptr->select.reselect.sw1        = uim_ptr->command.rpt_buf.sw1;
  uim_ptr->command.cmd_ptr->select.reselect.sw2        = uim_ptr->command.rpt_buf.sw2;
  uim_ptr->command.cmd_ptr->select.reselect.cmd_status = cmd_status;
  uim_ptr->command.cmd_ptr->select.is_intermediate_sel = TRUE;
} /* uim_prepare_select_failed_st */


/*===========================================================================

FUNCTION UIM_STREAM_APDU_SUCCESS

DESCRIPTION
  This procedure handles a successful stream apdu response

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void uim_stream_apdu_success
(
  uim_instance_global_type *uim_ptr,
  uim_req_buf_type         *uim_req_buf_static_ptr,
  uim_rsp_buf_type         *rsp_ptr
)
{
  word    num_bytes_to_alloc = 0;
  byte   *temp_data_ptr      = NULL;
  boolean temp_ef            = FALSE;

  num_bytes_to_alloc = rsp_ptr->cmd_rsp_size +
    uim_ptr->command.rd_wr_info.num_bytes_rsp;

  UIMDRV_MSG_HIGH_2(uim_ptr->id,"New data received = 0x%x, Data already present = 0x%x",
                    rsp_ptr->cmd_rsp_size,
                    uim_ptr->command.rd_wr_info.num_bytes_rsp);

  /*Now both of the size values added above are sizeof(word).
   As a result, for large values, this can wrap and result in a
   small value for num_bytes_to_alloc.
   If this happens then there will be a buffer overflow in the following processing.
   So truncate the response to just 0xFFFF bytes*/
  if(0xFFFF < ((dword)(uim_ptr->command.rd_wr_info.num_bytes_rsp)
                       + (dword)(rsp_ptr->cmd_rsp_size)))
  {
     /*the num_bytes_to_alloc variable has overflown. set it to FFFF bytes*/
    num_bytes_to_alloc = 0xFFFF;

     /*truncate cmd_rsp_size*/
    rsp_ptr->cmd_rsp_size = num_bytes_to_alloc - uim_ptr->command.rd_wr_info.num_bytes_rsp;

    UIMDRV_MSG_HIGH_0(uim_ptr->id,"New data received + Data already present exceeds 0xFFFF, truncating it to 0xFFFF bytes");
  }

  /* Add received data to rd/wr buffer */
  if(num_bytes_to_alloc > 0)
  {
    /* Merge new data if data is already present in rd/wr buffer */
    if(uim_ptr->command.rd_wr_info.num_bytes_rsp > 0)
    {
      /* Allocate new buffer for total data */
      temp_data_ptr = uim_malloc(num_bytes_to_alloc);
      if(temp_data_ptr ==  NULL)
      {
        uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
        return;
      }

      /* Copy over the already present data to the new buffer */
      uim_memscpy(temp_data_ptr,
                  num_bytes_to_alloc,
                  uim_ptr->command.rd_wr_info.data_ptr,
                  uim_ptr->command.rd_wr_info.num_bytes_rsp);

      /* Copy over the already present data to the buffer */
      uim_memscpy(temp_data_ptr + uim_ptr->command.rd_wr_info.num_bytes_rsp,
                 (num_bytes_to_alloc - uim_ptr->command.rd_wr_info.num_bytes_rsp),
                  rsp_ptr->rsp.data,
                  rsp_ptr->cmd_rsp_size);
    }

    /* Try to allocate mem and reset all the variables */
    if ( !uim_alloc_set_rd_wr_info(num_bytes_to_alloc, uim_ptr) )
    {
      uim_ptr->command.cmd_ptr->hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
      UIM_FREE(temp_data_ptr);
      return;
    }

    /* Merge new data if data is already present in rd/wr buffer */
    if(num_bytes_to_alloc > rsp_ptr->cmd_rsp_size && temp_data_ptr != NULL)
    {
      /* Copy over the merged data to the buffer provided each time we have
       * a peice of data returned
       */
      uim_memscpy(uim_ptr->command.rd_wr_info.data_ptr,
                  num_bytes_to_alloc,
                  temp_data_ptr,
                  num_bytes_to_alloc);
    }
    else
    {
      /* Copy over the data to the buffer provided each time we have
       * a peice of data returned
       */
      uim_memscpy(uim_ptr->command.rd_wr_info.data_ptr,
                  num_bytes_to_alloc,
                  rsp_ptr->rsp.data,
                  num_bytes_to_alloc);
    }

    uim_ptr->command.rd_wr_info.num_bytes_rsp = num_bytes_to_alloc;
    UIM_FREE(temp_data_ptr);
    temp_data_ptr = NULL;
  }

  /* Send Get Response if intermediate SW1 is 0x61 */
  if((rsp_ptr->sw1 == SW1_USIM_END_RESP) &&
     (0xFFFF > uim_ptr->command.rd_wr_info.num_bytes_rsp))
  {
    if(uim_req_buf_static_ptr->apdu_hdr.instrn != GET_RESPONSE)
    {
      uim_save_current_req_buf(uim_ptr);
    }
    /* Add state to do get response */
    uim_ptr->mode.generic_states[0] = UIM_GET_RESPONSE_ST;
    uim_ptr->mode.generic_states[1] = UIM_STREAM_APDU_ST;
    uim_ptr->mode.generic_states[2] = UIM_DONE_ST;
    uim_ptr->command.generic_state_ptr = uim_ptr->mode.generic_states;
  }
  else
  {
    /* If it is a select on the default channel we need to update curr path */
    if((uim_ptr->command.cmd_ptr->stream_iso7816_apdu.is_select_default_ch)
       && (rsp_ptr->cmd_rsp_size))
    {
      /* UICC and ICC have different response structure*/
      /* At this point, response from the card contains file type */
      if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_UICC)
      {
        uim_parse_uicc_select_response(
        uim_ptr,
        uim_ptr->command.rd_wr_info.data_ptr,
        uim_ptr->command.rd_wr_info.num_bytes_rsp,
       &temp_ef,
        NULL,
        NULL,
        NULL);

        uim_ptr->state.current_path[UIM_DEFAULT_CHANNEL].is_ef = temp_ef;
      }
      else if (uim_ptr->command.cmd_ptr->hdr.protocol == UIM_ICC)
      {
        temp_ef = rsp_ptr->rsp.ef.file_type == EF ? TRUE : FALSE;

#ifdef FEATURE_UIM_TEST_FRAMEWORK
        #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
        uim_ptr->state.current_path[UIM_DEFAULT_CHANNEL].is_ef = temp_ef;
      }
    }

    if(uim_req_buf_static_ptr->apdu_hdr.instrn == GET_RESPONSE)
    {
      uim_restore_current_req_buf(uim_ptr);
    }
    /* Skip to the next state. */
    ++uim_ptr->command.generic_state_ptr;
  }
} /* uim_stream_apdu_success */


/*===========================================================================

FUNCTION UIM_RESELECT_CURRENT_DIRECTORY_STATE

DESCRIPTION
  This procedure executes the re-selection of current directory

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void uim_reselect_cur_dir_state
(
  uim_instance_global_type *uim_ptr,
  uim_path_type            *target_path_ptr,
  uim_req_buf_type         *uim_req_buf_static_ptr
)
{
  int i;
  int j;

  uim_req_buf_static_ptr->apdu_hdr.instrn = SELECT;
  uim_req_buf_static_ptr->instrn_case     = UIM_INSTRN_CASE_3;

  uim_req_buf_static_ptr->apdu_hdr.uim_class =
    uim_util_get_uim_class(UIM_UICC_INSTRUCTION_CLASS1, uim_ptr->command.cmd_ptr->hdr.channel);

  /* if it's a select MF do this */
  if(target_path_ptr->path[0] == 0x3F00 &&
     target_path_ptr->len     == 1)
  {
    uim_req_buf_static_ptr->apdu_hdr.p1 = UIM_USIM_SELECT_BY_FILE_ID;
    uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_NO_DATA;
    uim_req_buf_static_ptr->req.data[0] = WORD_HI(target_path_ptr->path[0]);
    uim_req_buf_static_ptr->req.data[1] = WORD_LO(target_path_ptr->path[0]);
    uim_req_buf_static_ptr->apdu_hdr.p3 = 2;
  } /* SELECT MF */
  else
  {
    uim_req_buf_static_ptr->apdu_hdr.p1 = UIM_USIM_SELECT_BY_PATH_FROM_MF;
    uim_req_buf_static_ptr->apdu_hdr.p2 = UIM_UICC_RETURN_NO_DATA;
    uim_req_buf_static_ptr->apdu_hdr.p3 = 0;

    if(target_path_ptr->len > UIM_DIRMAIN_MAX_PATH_ELEMENTS)
    {
      /* target_path_ptr max length is 5 */
      UIMDRV_MSG_LOW_0(uim_ptr->id, "Target path lengh is greater than max 5. Setting to 5");
      target_path_ptr->len = UIM_DIRMAIN_MAX_PATH_ELEMENTS;
    }

    for(i = 1, j = 0; i < target_path_ptr->len; i++)
    {
      uim_req_buf_static_ptr->req.data[j++] = WORD_HI(target_path_ptr->path[i]);
      uim_req_buf_static_ptr->req.data[j++] = WORD_LO(target_path_ptr->path[i]);

      /* Set the param3 as the total number of bytes */
      uim_req_buf_static_ptr->apdu_hdr.p3 += 2;
    }
  } /* UICC Select - performed by path */

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uim_reselect_cur_dir_state */


/*===========================================================================

FUNCTION UIM_CHECK_CHARACTERISTICS_GET_NEXT_DF

DESCRIPTION
  We want to attempt to select GSM GSM_DCS_1800(if present) and CDMA  DFs
   (if CDMA_DF_SELECT_BEFORE_TP is active then we try to select CDMA first)
   If any of these selects are successful we'll stop attempting to select an
   ICC DF, but if they fail we want to keep selecting until one is a success

   This function takes the path that was just attempted and returns the next DF
   to select. In a normal case it would be called once and the first DF would
  return success - it is only called multiple times if there are failures

DEPENDENCIES

RETURN VALUE
  Next DF to select during ICC power-up

SIDE EFFECTS

===========================================================================*/
static uint16 uim_check_characteristics_get_next_df
(
  uim_instance_global_type *uim_ptr,
  uint16 curr_df
)
{
  uint16                     dfs_to_select[3] = {0x0000, 0x0000, 0x0000};
  int                        index            = 0;
  int                        i                = 0;
  mmgsdi_return_enum_type    mmgsdi_status    = MMGSDI_SUCCESS;
  mmgsdi_cached_nv_item_data nv_item;

  memset(&nv_item, 0x00, sizeof(mmgsdi_cached_nv_item_data));

  if (uim_ptr->flag.uim_auth_cdma_df_first)
  {
    dfs_to_select[index++] = UIM_CDMA_DF;
  }

  if(!(uim_ptr->setting.config_params.future_use[UIM_RFU_INDEX_BOOLEAN_FLAGS] &
      UIM_RFU_MASK_NOT_SELECT_GSM_DF))
  {
    dfs_to_select[index++] = UIM_GSM_DF;
  }

  nv_item.nv_item_enum = MMGSDI_NV_UIM_GSM_DCS_1800_I;
  mmgsdi_status        = uim_interface_mmgsdi_get_cached_nv_item_ext(&nv_item, (uint8)uim_ptr->id);

  if(mmgsdi_status == MMGSDI_SUCCESS &&
     nv_item.nv_item_data.uim_gsm_dcs_1800 == TRUE)
  {
    dfs_to_select[index++] = UIM_DCS_1800_DF;
  }

  if(uim_ptr->flag.uim_auth_cdma_df_first == FALSE)
  {
    dfs_to_select[index++] = UIM_CDMA_DF;
  }

  if( index == 0 || curr_df == dfs_to_select[index-1])
  {
    /* if index is 0
       or the current df was the last in the list to select
       we don't have a next df return an invalid ID */
    return 0x0000;
  }

  for(i = 0; i < index-1; i++)
  {
    if(i < 2 && curr_df == dfs_to_select[i])
    {
      return dfs_to_select[i + 1];
    }
  }
  return dfs_to_select[0];
} /* uim_check_characteristics_get_next_df */


/*===========================================================================

FUNCTION UIM_IS_FI_DI_SUPPORTED

DESCRIPTION
  This procedure determines if the platform can support any {FI, DI}
  combination.

DEPENDENCIES
  None

RETURN VALUE
  BOOL

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_is_fi_di_supported
(
  uim_instance_global_type              *uim_ptr,
  uim_clock_rate_conversion_factor_type  FI, /* CRCF index */
  uim_baud_rate_adjustment_factor_type   DI /* BRAF index */
)
{
  return is_fi_di_supported(uim_ptr->id, FI, DI);
} /* uim_is_fi_di_supported */


/*===========================================================================

FUNCTION UIM_SET_SVN

DESCRIPTION
  This procedure sets the eUICC version number global

DEPENDENCIES
  None

RETURN VALUE
  uim_return_type

SIDE EFFECTS
  None

===========================================================================*/
uim_return_type uim_set_svn 
(
  uim_slot_type          slot_id,
  uint8                  svn_len,
  uint8                 *svn_ptr
)
{
  if(slot_id >= UIM_MAX_NUM_SLOTS ||
     svn_ptr == NULL ||
     svn_len != UIM_SVN_LEN)
  {
    return UIM_GENERIC_ERROR;
  }

  memscpy(uim_svn[slot_id],
          sizeof(uim_svn[slot_id]),
          svn_ptr,
          svn_len);

  return UIM_SUCCESS;
} /* uim_set_svn */


/*===========================================================================

FUNCTION UIM_GET_EUICC_PHASE

DESCRIPTION
  This procedure returns the eUICC phase

DEPENDENCIES
  None

RETURN VALUE
  uim_return_type

SIDE EFFECTS
  None

===========================================================================*/
uim_return_type uim_get_euicc_phase
(
  uim_slot_type               slot_id,
  uim_euicc_phase_enum_type   *euicc_phase_ptr
)
{
  uim_return_type  status = UIM_SUCCESS;

  if(slot_id > UIM_MAX_NUM_SLOTS ||
     euicc_phase_ptr == NULL)
  {
    return UIM_GENERIC_ERROR;
  }
  
  switch(uim_svn[slot_id][0])
  {
    case 0x01:
	  if(uim_svn[slot_id][1] == 0x00)
	  {
        *euicc_phase_ptr = UIM_PHASE1_EUICC;
	  }
	  else if(uim_svn[slot_id][1] == 0x01)
	  {
        *euicc_phase_ptr = UIM_PHASE1_1_EUICC;
	  }
	  else
	  {
        status = UIM_GENERIC_ERROR;
	  }
	  break;
    case 0x02:
	  *euicc_phase_ptr = UIM_PHASE2_EUICC;
	  break;
    default:
      status = UIM_GENERIC_ERROR;
	  break;
  }
  
  UIM_MSG_HIGH_1("eUICC phase 0x%x ", *euicc_phase_ptr);
  
  return status;
} /* uim_get_euicc_phase */


/*===========================================================================

FUNCTION UIM_SEND_GET_DATA_APDU

DESCRIPTION
  This procedure prepares and sends a GET DATA APDU to GET EID

DEPENDENCIES
  None

RETURN VALUE
  uim_return_type

SIDE EFFECTS
  None

===========================================================================*/
void uim_send_get_data_apdu
(
  uim_instance_global_type *uim_ptr
)
{
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;

  if(uim_ptr == NULL)
  {
    return;
  }
  
  uim_req_buf_static_ptr->protocol              = UIM_UICC;

  uim_req_buf_static_ptr->apdu_hdr.instrn       = 0xCA;
  uim_req_buf_static_ptr->apdu_hdr.uim_class    = (0x80 | uim_ptr->select_isdr_ch_num.channel);
  uim_req_buf_static_ptr->apdu_hdr.p1           = 0xBF;
  uim_req_buf_static_ptr->apdu_hdr.p2           = 0x30;
  uim_req_buf_static_ptr->apdu_hdr.p3           = 0x03;
  
  /* Prepare the command data which indicates 5C 01 5A GET EID data object, 
     where 5A identifies the EID */
  uim_req_buf_static_ptr->req.data[0]           = 0x5C;
  uim_req_buf_static_ptr->req.data[1]           = 0x01;
  uim_req_buf_static_ptr->req.data[2]           = 0x5A;
  
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_4;
  
  /* Initialize the SW */
  uim_ptr->card_cmd.get_resp_sw1 = (uim_sw1_type) 0;
  uim_ptr->card_cmd.get_resp_sw2 = (uim_sw2_type) 0;
  
  uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw1 = uim_ptr->card_cmd.get_resp_sw1;
  uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw2 = uim_ptr->card_cmd.get_resp_sw2;
  
  uim_ptr->command.rd_wr_info.num_bytes = 0;
  uim_ptr->command.rd_wr_info.num_bytes_rsp = 0;

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uim_send_get_data_apdu */


/*===========================================================================

FUNCTION UIM_SEND_STORE_DATA_APDU

DESCRIPTION
  This procedure prepares and sends a STORE DATA APDU to GET EID

DEPENDENCIES
  None

RETURN VALUE
  uim_return_type

SIDE EFFECTS
  None

===========================================================================*/
void uim_send_store_data_apdu
(
  uim_instance_global_type *uim_ptr
)
{
  uim_req_buf_type *uim_req_buf_static_ptr = &uim_ptr->command.req_buf;
	
  if(uim_ptr == NULL)
  {
    return;
  }
  
  uim_req_buf_static_ptr->protocol              = UIM_UICC;

  uim_req_buf_static_ptr->apdu_hdr.instrn       = 0xE2;
  uim_req_buf_static_ptr->apdu_hdr.uim_class    = (0x80 | uim_ptr->select_isdr_ch_num.channel);
  uim_req_buf_static_ptr->apdu_hdr.p1           = 0x91;
  uim_req_buf_static_ptr->apdu_hdr.p2           = 0x00;
  uim_req_buf_static_ptr->apdu_hdr.p3           = 0x06;
  
  /* Prepare the command data */
  uim_req_buf_static_ptr->req.data[0]           = 0xBF;
  uim_req_buf_static_ptr->req.data[1]           = 0x3E;
  uim_req_buf_static_ptr->req.data[2]           = 0x03;
  uim_req_buf_static_ptr->req.data[3]           = 0x5C;
  uim_req_buf_static_ptr->req.data[4]           = 0x01;
  uim_req_buf_static_ptr->req.data[5]           = 0x5A;
  
  uim_req_buf_static_ptr->instrn_case = UIM_INSTRN_CASE_4;
  
  /* Initialize the SW */
  uim_ptr->card_cmd.get_resp_sw1 = (uim_sw1_type) 0;
  uim_ptr->card_cmd.get_resp_sw2 = (uim_sw2_type) 0;
  
  uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw1 = uim_ptr->card_cmd.get_resp_sw1;
  uim_ptr->command.rpt_buf.rpt.stream_iso7816_apdu.get_resp_sw2 = uim_ptr->card_cmd.get_resp_sw2;
  
  uim_ptr->command.rd_wr_info.num_bytes = 0;
  uim_ptr->command.rd_wr_info.num_bytes_rsp = 0;

  uim_req_buf_static_ptr->rsp_ptr = &uim_ptr->command.rsp_buf;

  uim_send_command(uim_req_buf_static_ptr, uim_ptr);
} /* uim_send_store_data_apdu */


/*===========================================================================

FUNCTION UIM_PARSE_EID_RESP

DESCRIPTION
  This procedure parses the response from GET EID command

DEPENDENCIES
  None

RETURN VALUE
  uim_return_type

SIDE EFFECTS
  None

===========================================================================*/
uim_return_type uim_parse_eid_resp
(
  uint32                          data_len,
  uint8                          *data_ptr,
  uim_eid_buf_type               *resp_ptr
)
{
  /*  GetEuiccDataResponse ::= [62] SEQUENCE { -- Tag 'BF3E'
      eidValue [APPLICATION 26] Octet16 -- tag '5A'
      }
  */
  uint32              offset  = 0;
  uim_asn1_cmds_type  cmds;

  if(data_ptr == NULL ||
     resp_ptr == NULL)
  {
    return UIM_INVALID_PARAMS;
  }

  memset(&cmds, 0x00, sizeof(uim_asn1_cmds_type));
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = UIM_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = UIM_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED;
  cmds.tlv_tag[0].tag[1]  = UIM_ASN1_GEN_TAG_CONTEXT_SPECIFIC62_SECOND;

  if(NULL == data_ptr ||
     UIM_SUCCESS != uim_util_asn1_decode_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   UIM_ASN1_TLV_SINGLE_TAG_MIN_LEN +
                                                   UIM_MAX_EID_SIZE,
                                                   UIM_ASN1_TLV_SINGLE_TAG_MIN_LEN +
                                                   UIM_MAX_EID_SIZE,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return UIM_GENERIC_ERROR;
  }

  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = UIM_ASN1_GEN_TAG_APPLICATION26;

  if(UIM_SUCCESS != uim_util_asn1_decode_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   UIM_MAX_EID_SIZE,
                                                   UIM_MAX_EID_SIZE,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return UIM_GENERIC_ERROR;
  }

  memscpy((uint8 *)(resp_ptr->data),
          UIM_MAX_EID_SIZE,
          (const uint8 *)(data_ptr + offset),
          UIM_MAX_EID_SIZE);

  return UIM_SUCCESS;
} /* uim_parse_eid_resp */


/*===========================================================================

FUNCTION UIM_DECODE_SELECT_ISDR_RESP

DESCRIPTION
  This procedure parses the response from select ISDR

DEPENDENCIES
  None

RETURN VALUE
  uim_return_type

SIDE EFFECTS
  None

===========================================================================*/
uim_return_type uim_decode_select_isdr_resp 
(
  uint32                                 data_len,
  uint8                                 *data_ptr,
  uim_select_isdr_resp_type             *resp_ptr
)
{
  uint32                 offset           = 0;
  uint32                 current_tlv_len  = 0;
  uim_asn1_tlv_type_type current_tlv_type = UNKNOWN_TLV_TYPE;
  uim_asn1_cmds_type     cmds;

  if (NULL == data_ptr || NULL == resp_ptr)
  {
    return UIM_GENERIC_ERROR;
  }

  memset(&cmds, 0x00, sizeof(uim_asn1_cmds_type));
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = UIM_ASN1_TAG_SELECT_ISDR;

  if(UIM_SUCCESS != uim_util_asn1_decode_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   0,
                                                   0,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    resp_ptr->svn[0] = 2;
    resp_ptr->svn[1] = 0;
    resp_ptr->svn[2] = 0;
    return UIM_SUCCESS;
  }

  /* This is a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 2;
  cmds.tlv_type[0]        = INTERNAL_PRIVATE0;
  cmds.tlv_tag[0].tag_len = UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = UIM_ASN1_GEN_TAG_PRIVATE0;
  cmds.tlv_type[1]        = ANY_TLV_TYPE;
  cmds.tlv_tag[1].tag_len = UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[1].tag[0]  = UIM_ASN1_TAG_ANY_LOW_TAG_NUMBER_FORM;

  while(offset < data_len)
  {
    if(UIM_SUCCESS != uim_util_asn1_decode_parse_tlv(data_len,
                                                     data_ptr,
                                                     offset,
                                                     cmds,
                                                     0,
                                                     0,
                                                     &offset,
                                                     &current_tlv_len,
                                                     &current_tlv_type,
                                                     TRUE))
    {
      return UIM_GENERIC_ERROR;
    }

    if(ANY_TLV_TYPE == current_tlv_type)
    {
      /* Found some TLV that we don't care,
         skip the value field*/
      offset += current_tlv_len;
    }
    else if(INTERNAL_PRIVATE0 == current_tlv_type)
    {
      /* Found ISDRProprietaryApplicationTemplate,
         proceed */
      break;
    }
    else
    {
      /* This should never happen */
      return UIM_GENERIC_ERROR;
    }
  }

  if(offset >= data_len)
  {
    /* Didn't find ISDRProprietaryApplicationTemplate
       in the ISDR response. Assume this is a non-phase2
       card, and set svn to phase 1 for now */
    resp_ptr->svn[0] = 1;
    resp_ptr->svn[1] = 1;
    resp_ptr->svn[2] = 0;

    return UIM_SUCCESS;
  }

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_tag[0].tag_len = UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = UIM_ASN1_GEN_TAG_CONTEXT_SPECIFIC2;

  if(UIM_SUCCESS != uim_util_asn1_decode_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   UIM_SVN_LEN,
                                                   UIM_SVN_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   FALSE))
  {
    return UIM_GENERIC_ERROR;
  }

  memscpy((uint8 *)(resp_ptr->svn),
          UIM_SVN_LEN,
          (const uint8 *)(data_ptr + offset),
          UIM_SVN_LEN);

  offset += UIM_SVN_LEN;
  if (offset >= data_len)
  {
    return UIM_SUCCESS;
  }

  /* This is not a CHOICE, hence only one possible cmd */
  cmds.num_of_tlv_types   = 1;
  cmds.tlv_type[0]        = INTERNAL_BIT_STRING;
  cmds.tlv_tag[0].tag_len = UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM;
  cmds.tlv_tag[0].tag[0]  = UIM_ASN1_GEN_TAG_BIT_STRING;

  if(UIM_SUCCESS != uim_util_asn1_decode_parse_tlv(data_len,
                                                   data_ptr,
                                                   offset,
                                                   cmds,
                                                   UIM_ASN1_SUPPORT_LEN,
                                                   UIM_ASN1_SUPPORT_LEN,
                                                   &offset,
                                                   NULL,
                                                   NULL,
                                                   TRUE))
  {
    return UIM_GENERIC_ERROR;
  }
  if(INTERNAL_BIT_STRING == current_tlv_type)
  {
    /* lpaeSupport present */
    resp_ptr->uim_e_using_cat  = ((data_ptr[offset] & UIM_ASN1_USING_CAT_MASK) > 0);
    resp_ptr->uim_e_using_cat  = ((data_ptr[offset] & UIM_ASN1_USING_SCWS_MASK)> 0);
  }

  return UIM_SUCCESS;
} /* uim_decode_select_isdr_resp */
/*============================================================================
  FILE:         uim_powerup_logging.c

  OVERVIEW:     The file defines the powerup logging module

  DEPENDENCIES: N/A

                Copyright (c) 2014 - 2017, 2021 QUALCOMM Technologies, Inc(QTI).
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/common/src/uim_powerup_logging.c#3 $

when       who      what, where, why
--------   ---      -----------------------------------------------------------
01/21/21   ykr      Habanero compilation fix
01/19/17   vdc      Added wrapper function to commit log message
04/25/16   sam      Added feature to print full F3s of powerup logs instead of
                    just powerup tags
03/28/16   sam      Move critical section definition from header file to source file
03/09/16   sam      Addition of version and index to LOG_UIM_PUP_MSG_C
10/27/15   hyo      Avoid writing enum to EFS
09/06/15   sam      Conversion of the UIM powerup messages from F3 messages 
                    to Log packets
06/29/15   lm       Enable memory leak detection
08/04/15   sam      Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#ifdef FEATURE_UIM_POWERUP_LOGGING
#include "uim_powerup_logging.h"
#include "modem_mem.h"
#include "uimutil.h"
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
#include "uim_msg.h"
#include "uim_common_lib.h"

/*Structure of uim_powerup_logging_config NV for EFS file interface*/
typedef PACKED struct PACKED_POST {
  boolean              disable_pup_logging[NO_OF_INSTANCES];
  uint16               no_of_msgs_per_slot;
  uint16               timer_value_in_ms;
  uint8                pup_logging_type;
} uim_pup_logging_config_efs_type;

/*critical section for intialising the module*/
rex_crit_sect_type  pup_log_crit_sec;

/*Boolean to check if the intialization is done or not*/
boolean pup_globals_init = FALSE;

/*Ptr to the UIM logging module globals*/
uim_pup_log_global_type *uim_pup_ptr = NULL;


#ifdef UIM_PUP_LOGGING_PRINT_F3
/**
*API to map the enum tag to the corresponding F3 and print it out using the
 Qsrink Message macros.
*
*  @param uim_pup_enum  tag    The tag of the msg string
*  @param uint32  p1                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p2                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p3                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p4                    The first parameter for the printf style
*                                                 msg string.
*/
void uim_map_powerup_tag_to_f3
(
  uim_pup_enum pup_tag,
  uint32 p1,
  uint32 p2,
  uint32 p3,
  uint32 p4
)
{
  switch (pup_tag)
  {
    case   UIMLOG_MSG_1:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Resetting necessary UIM globals after notifying clients of error", p1);
    }
    break;

    case   UIMLOG_MSG_2:
    {
      UIM_MSG_HIGH_4("#PUP UIM_%d: Flushing_Type 0x%xFlushing 0x%x commands, uim status is 0x%x", p1, p2,p3,p4);
    }
    break;

    case   UIMLOG_MSG_3:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: After flushing Total CMD count 0x%x , Removed count 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_4:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: Response status %d, uim_cmd_mode 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_5:
    {
      UIM_MSG_HIGH_4("#PUP UIM_%d: SW1 0x%x,SW2 0x%x, Response data length 0x%x ", p1, p2,p3,p4);
    }
    break;

    case   UIMLOG_MSG_6:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Internal command to Reset the UIM", p1);
    }
    break;

    case   UIMLOG_MSG_7:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: uim_reset_uim is not allowed, card is either not inserted or not detected on this slot", p1);
    }
    break;

    case   UIMLOG_MSG_8:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM link established with card over USB UICC interface", p1);
    }
    break;

    case   UIMLOG_MSG_9:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM link established with card over legacy ISO interface", p1);
    }
    break;

    case   UIMLOG_MSG_10:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: In uim_process_cmd_err()", p1);
    }
    break;

    case   UIMLOG_MSG_11:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_12:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Process cmd error in external command", p1);
    }
    break;

    case   UIMLOG_MSG_13:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: cmd count maxed out", p1);
    }
    break;

    case   UIMLOG_MSG_14:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: uim_command_requeued = %d", p1, p2);
    }
    break;

    case   UIMLOG_MSG_15:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Process cmd error in internal command", p1);
    }
    break;

    case   UIMLOG_MSG_16:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Processing response for the reset command", p1);
    }
    break;

    case   UIMLOG_MSG_17:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Requeue the Reset command due to UIM_INSTRN_CHANGE", p1);
    }
    break;

    case   UIMLOG_MSG_18:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Requeue the Reset command due to UIM_CONVENTION_CHANGE", p1);
    }
    break;

    case   UIMLOG_MSG_19:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: SFI operation failed - Retrying command without using SFI", p1);
    }
    break;

    case   UIMLOG_MSG_20:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Requeue the Reset command due to UIM_SWITCH_VOLTAGE", p1);
    }
    break;

    case   UIMLOG_MSG_21:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_22:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Task clean up completed", p1);
    }
    break;

    case   UIMLOG_MSG_23:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: select path[%d]:0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_24:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Stream on default with hdr->channel != 0 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_25:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Intermediate select required for path:", p1);
    }
    break;

    case   UIMLOG_MSG_26:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: Path[%d]:0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_27:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: AID is present: aid_length:0x%d", p1, p2);
    }
    break;

    case   UIMLOG_MSG_28:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: No need for intermediate select on channel 0x%x. Curr Path:", p1, p2);
    }
    break;

    case   UIMLOG_MSG_29:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Updating the current directory for UICC on channel 0x%x. Path before:", p1, p2);
    }
    break;

    case   UIMLOG_MSG_30:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Path after:", p1);
    }
    break;

    case   UIMLOG_MSG_31:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Updating the current directory for ICC - default channel. Path before", p1);
    }
    break;

    case   UIMLOG_MSG_32:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Path after:", p1);
    }
    break;

    case   UIMLOG_MSG_33:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Updating path for SFI on channel 0x%x. Path before:", p1, p2);
    }
    break;

    case   UIMLOG_MSG_34:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Path after:", p1);
    }
    break;

    case   UIMLOG_MSG_35:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: uim_notify_busy(TRUE, slot)", p1);
    }
    break;

    case   UIMLOG_MSG_36:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: uim_notify_busy(FALSE, slot)", p1);
    }
    break;

    case   UIMLOG_MSG_37:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: DISABLE_RECOVERY_UPON_INFINITE_NULL.NULL bytes Maxed. UIM wll power down", p1);
    }
    break;

    case   UIMLOG_MSG_38:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM will force recovery due to trans timer timeout", p1);
    }
    break;

    case   UIMLOG_MSG_39:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Reporting error for CDMA command 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_40:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Received cdma command 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_41:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Reporting result for CDMA command 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_42:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM in BTSAP client mode.Need not do Vcc power-up", p1);
    }
    break;

    case   UIMLOG_MSG_43:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: uim power up @ 1.8 v", p1);
    }
    break;

    case   UIMLOG_MSG_44:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: uim power up @ 3 v", p1);
    }
    break;

    case   UIMLOG_MSG_45:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: Wrong value of uim_DI_current = 0x%x/uim_FI_current = 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_46:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Function uim_update_op_params should not be called", p1);
    }
    break;

    case   UIMLOG_MSG_47:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM in BTSAP client mode.Calling BTSAP API's to send cmd to remote card", p1);
    }
    break;

    case   UIMLOG_MSG_48:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM in USB mode.Calling remote API's to send cmd", p1);
    }
    break;

    case   UIMLOG_MSG_49:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: uim_send_command", p1);
    }
    break;

    case   UIMLOG_MSG_50:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: uim power down @ 1.8 v", p1);
    }
    break;

    case   UIMLOG_MSG_51:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: uim power down @ 3 v", p1);
    }
    break;

    case   UIMLOG_MSG_52:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: In Software work waiting timer expiry cb", p1);
    }
    break;

    case   UIMLOG_MSG_53:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: In Software work waiting timer expiry cb", p1);
    }
    break;

    case   UIMLOG_MSG_54:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: uim_bytes_waiting_in_rxfifo 0x%x  wait cnt 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_55:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: No ATR in 40,000 clk cycles", p1);
    }
    break;

    case   UIMLOG_MSG_56:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Force Recovery", p1);
    }
    break;

    case   UIMLOG_MSG_57:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Reporting error for generic command 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_58:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Received generic command 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_59:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Received internal USB UICC powerup command 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_60:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Received internal Powerup command 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_61:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Received internal USB UICC wakeup command 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_62:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Received internal Wakeup command, UIM Entering Recovery 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_63:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Changing the voltage class to 3V during 0x%x recovery", p1, p2);
    }
    break;

    case   UIMLOG_MSG_64:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Received Reset command for UIM_HOTSWAP_CARD_INS_F", p1);
    }
    break;

    case   UIMLOG_MSG_65:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Received Reset command 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_66:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: USB UICC :Received reset command to icc", p1);
    }
    break;

    case   UIMLOG_MSG_67:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM is in BTSAP client mode and cannot be switched to PASSIVE MODE. ", p1);
    }
    break;

    case   UIMLOG_MSG_68:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Received reset to make UIM USB UICC passive 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_69:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Received Reset to make UIM passive 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_70:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Received power down command", p1);
    }
    break;

    case   UIMLOG_MSG_71:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Successfully mask hot swap interrupt for power down command", p1);
    }
    break;

    case   UIMLOG_MSG_72:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Streaming UICC on default channel", p1);
    }
    break;

    case   UIMLOG_MSG_73:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Not allowed to active other app on default channel", p1);
    }
    break;

    case   UIMLOG_MSG_74:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_75:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: SFI selection is not supported", p1);
    }
    break;

    case   UIMLOG_MSG_76:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: SFI selection is not supported", p1);
    }
    break;

    case   UIMLOG_MSG_77:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Reporting UIM generic command 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_78:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM Recovery Complete", p1);
    }
    break;

    case   UIMLOG_MSG_79:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM was not able to open the channel", p1);
    }
    break;

    case   UIMLOG_MSG_80:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Calling QMI remote API to power down the remote SIM", p1);
    }
    break;

    case   UIMLOG_MSG_81:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: GSM instruction class  - clk freq change required to 3.8MHz", p1);
    }
    break;

    case   UIMLOG_MSG_82:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM in BTSAP client mode.Call BTSAP API's to power-up remote card", p1);
    }
    break;

    case   UIMLOG_MSG_83:
    {
      UIM_MSG_HIGH_4("#PUP UIM_%d: usb enabled 0x%x usb mode supported 0x%x fallback occured 0x%x", p1, p2,p3,p4);
    }
    break;

    case   UIMLOG_MSG_84:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: usb has remote connection 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_85:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Sending USB PPS", p1);
    }
    break;

    case   UIMLOG_MSG_86:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Sending Default PPS", p1);
    }
    break;

    case   UIMLOG_MSG_87:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: uim_ptr->flag.iccid_select_failed is TRUE", p1);
    }
    break;

    case   UIMLOG_MSG_88:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_89:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_90:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM_RECOVERY_SEND_STATUS_COMMAND_ST", p1);
    }
    break;

    case   UIMLOG_MSG_91:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: UIM_READ_BIN_MEM_ST - SFI used - 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_92:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: UIM_UPDATE_BIN_MEM_ST - SFI used - 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_93:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: UIM_INC_MEM_ST - SFI used - 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_94:
    {
      UIM_MSG_HIGH_0("#PUP Turning off UIM USB UICC with QMI_UIM_REMOTE_POWER_DOWN_CARD");
    }
    break;

    case   UIMLOG_MSG_95:
    {
      UIM_MSG_HIGH_0("#PUP Turning off UIM USB UICC with QMI_UIM_REMOTE_POWER_DOWN_TELECOM_INTERFACE");
    }
    break;

    case   UIMLOG_MSG_96:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Waiting for AP client to connect....", p1);
    }
    break;

    case   UIMLOG_MSG_97:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Turning on UIM USB UICC", p1);
    }
    break;

    case   UIMLOG_MSG_98:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_99:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_100:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Flushing manage channel command", p1);
    }
    break;

    case   UIMLOG_MSG_101:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: No information in GSDI cache, move to Done state", p1);
    }
    break;

    case   UIMLOG_MSG_102:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: Wrong CHV1/PIN1 %d recd for %d protocol", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_103:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: Wrong CHV2/PIN2 %d recd for %d protocol", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_104:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: No Terminal Profile data in UIM cache", p1);
    }
    break;

    case   UIMLOG_MSG_105:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: No more channels to recover", p1);
    }
    break;

    case   UIMLOG_MSG_106:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Skip Manage Channel open for default Channel=%d", p1, p2);
    }
    break;

    case   UIMLOG_MSG_107:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: No more channels to be closed. Processing state 0x%ld", p1, p2);
    }
    break;

    case   UIMLOG_MSG_108:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Process further states for default channel =%d ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_109:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Process further states for Channel=%d", p1, p2);
    }
    break;

    case   UIMLOG_MSG_110:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Channel=%d is being closed, as per MMGSDI request", p1, p2);
    }
    break;

    case   UIMLOG_MSG_111:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Channel=%d is being closed now", p1, p2);
    }
    break;

    case   UIMLOG_MSG_112:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: AID is invalid, moving to next channel", p1);
    }
    break;

    case   UIMLOG_MSG_113:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Send Select AID for channel=%d", p1, p2);
    }
    break;

    case   UIMLOG_MSG_114:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Send UPIN verification on default channel", p1);
    }
    break;

    case   UIMLOG_MSG_115:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UPIN not available", p1);
    }
    break;

    case   UIMLOG_MSG_116:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: generic_state_ptr 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_117:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Returning error since cmd_status = UIM_PARITY", p1);
    }
    break;

    case   UIMLOG_MSG_118:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: LDO behaviour modified to work at specific voltage class", p1);
    }
    break;

    case   UIMLOG_MSG_119:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: The card supports USB UICC", p1);
    }
    break;

    case   UIMLOG_MSG_120:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: The card supports USB UICC", p1);
    }
    break;

    case   UIMLOG_MSG_121:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: LDO behaviour modified to work at specific voltage class", p1);
    }
    break;

    case   UIMLOG_MSG_122:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: LDO behaviour modified to work at specific voltage class", p1);
    }
    break;

    case   UIMLOG_MSG_123:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Select CDMA failed Command status: %x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_124:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: ICCID read failure during recovery", p1);
    }
    break;

    case   UIMLOG_MSG_125:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_126:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: ICCID MISMATCH", p1);
    }
    break;

    case   UIMLOG_MSG_127:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Recovering UICC", p1);
    }
    break;

    case   UIMLOG_MSG_128:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Recovering ICC", p1);
    }
    break;

    case   UIMLOG_MSG_129:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_130:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_131:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Select failure during Recovery", p1);
    }
    break;

    case   UIMLOG_MSG_132:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: updating current directory icc - temp_ef:0x%x - file type:0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_133:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: MCC/MNC match. It is a problematic card", p1);
    }
    break;

    case   UIMLOG_MSG_134:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: IMSI_M_PROGRAMMED bit is 0.IMSI_M not programmed", p1);
    }
    break;

    case   UIMLOG_MSG_135:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Removing command=%d from UIM queue", p1, p2);
    }
    break;

    case   UIMLOG_MSG_136:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Reporting error for USIM command 0x%x ", p1, p2);
    }
    break;

    case   UIMLOG_MSG_137:
    {
      UIM_MSG_HIGH_0("#PUP Curent envelop recieved is a location status envelope");
    }
    break;

    case   UIMLOG_MSG_138:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d:   current path[%d]: 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_139:
    {
      UIM_MSG_HIGH_0("#PUP NON-TELECOM AID found");
    }
    break;

    case   UIMLOG_MSG_140:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: HOTSWAP: Drop Card inserted signal as card is already removed or me is in power down state", p1);
    }
    break;

    case   UIMLOG_MSG_141:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: HOTSWAP: Reporting err for current cmd: 0x%x status: 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_142:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: HOTSWAP:executing UIM_HOTSWAP_CARD_INS_F", p1);
    }
    break;

    case   UIMLOG_MSG_143:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: HOTSWAP: Drop Card removed signal as card is inserted or ME is in power down state", p1);
    }
    break;

    case   UIMLOG_MSG_144:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: HOTSWAP: Reporting err for current cmd: 0x%x status: 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_145:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: HOTSWAP: executing UIM_HOTSWAP_CARD_REM_F", p1);
    }
    break;

    case   UIMLOG_MSG_146:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: HOTSWAP: Start, For CARD INSERTED Interrupt ", p1);
    }
    break;

    case   UIMLOG_MSG_147:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: HOTSWAP: Start, For CARD REMOVED Interrupt", p1);
    }
    break;

    case   UIMLOG_MSG_148:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Disabled hotswap", p1);
    }
    break;

    case   UIMLOG_MSG_149:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: HOTSWAP: Debounce logic End, Unstable status: %d [0=REMOVED 1=INSERTED]", p1, p2);
    }
    break;

    case   UIMLOG_MSG_150:
    {
      UIM_MSG_HIGH_0("#PUP Uim internal hotswap detection");
    }
    break;

    case   UIMLOG_MSG_151:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Starting to log the timeout Information to UimReset.Txt", p1);
    }
    break;

    case   UIMLOG_MSG_152:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_153:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM_NV_INIT completed", p1);
    }
    break;

    case   UIMLOG_MSG_154:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Invalid resp received setting command resp time out sig", p1);
    }
    break;

    case   UIMLOG_MSG_155:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Connection Unavailable event triggered.No need to call powerdown API", p1);
    }
    break;

    case   UIMLOG_MSG_156:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Could powerdown the remote card", p1);
    }
    break;

    case   UIMLOG_MSG_157:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: Sending the qmi remote connect API with timer set to 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_158:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Could connect to the remote card", p1);
    }
    break;

    case   UIMLOG_MSG_159:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Couldn't powerup the remote card", p1);
    }
    break;

    case   UIMLOG_MSG_160:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Could reset the remote card", p1);
    }
    break;

    case   UIMLOG_MSG_161:
    {
      UIM_MSG_HIGH_4("#PUP UIM_%d: Err instrn %%case=0x%x.", p1, p2,p3,p4);
    }
    break;

    case   UIMLOG_MSG_162:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Received get response command. Issuing new command", p1);
    }
    break;

    case   UIMLOG_MSG_163:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: No response from remote sim for the command", p1);
    }
    break;

    case   UIMLOG_MSG_164:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: RX state machine not invoked - Error in rx sub state", p1);
    }
    break;

    case   UIMLOG_MSG_165:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: RX received with parity errors", p1);
    }
    break;

    case   UIMLOG_MSG_166:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: RX received with parity errors while waiting for ATR", p1);
    }
    break;

    case   UIMLOG_MSG_167:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: T=1: RX received with parity errors, to send R block", p1);
    }
    break;

    case   UIMLOG_MSG_168:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: RX state moved to overrun state to log RX bytes", p1);
    }
    break;

    case   UIMLOG_MSG_169:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: RX sub state invalid", p1);
    }
    break;

    case   UIMLOG_MSG_170:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Error processing PPS", p1);
    }
    break;

    case   UIMLOG_MSG_171:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Process Unknown procedure byte", p1);
    }
    break;

    case   UIMLOG_MSG_172:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Received Rx break error, setting timeout signal", p1);
    }
    break;

    case   UIMLOG_MSG_173:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: parity error in IDLE", p1);
    }
    break;

    case   UIMLOG_MSG_174:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Maxed the parity error count. timeout", p1);
    }
    break;

    case   UIMLOG_MSG_175:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Falling back to legacy ISO from IC USB interface", p1);
    }
    break;

    case   UIMLOG_MSG_176:
    {
      UIM_MSG_HIGH_4("#PUP UIM_%d: Reporting err for current cmd: 0x%x slot: 0x%x status: 0x%x", p1, p2,p3,p4);
    }
    break;

    case   UIMLOG_MSG_177:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Received remote wake up event from USB UICC", p1);
    }
    break;

    case   UIMLOG_MSG_178:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: CLOCK OFF when processing command", p1);
    }
    break;

    case   UIMLOG_MSG_179:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: SIM in power down state", p1);
    }
    break;

    case   UIMLOG_MSG_180:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: STREAMING COMMAND is not allowed for channel 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_181:
    {
      UIM_MSG_HIGH_0("#PUP Invalid cmd UIM in passive mode ret error");
    }
    break;

    case   UIMLOG_MSG_182:
    {
      UIM_MSG_HIGH_4("#PUP UIM_%d: CMD_RSP Sig Rcvd uim_status=0x%x, uim_st_bf_us=0x%x, cmd_ptr=0x%x", p1, p2,p3,p4);
    }
    break;

    case   UIMLOG_MSG_183:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Received timeout signal when remote client is active", p1);
    }
    break;

    case   UIMLOG_MSG_184:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Received timeout signal when remote client is active", p1);
    }
    break;

    case   UIMLOG_MSG_185:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_186:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Attempting power up at 3V", p1);
    }
    break;

    case   UIMLOG_MSG_187:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: HOTSWAP: card_status removed during cmd_rsp_timeout", p1);
    }
    break;

    case   UIMLOG_MSG_188:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: Reporting err for current cmd: 0x%x Status: 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_189:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: HOTSWAP: UIM received response time out signal  0x%x 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_190:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Shut down UIM due to the infinite NULL responses", p1);
    }
    break;

    case   UIMLOG_MSG_191:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Timed out on the command response", p1);
    }
    break;

    case   UIMLOG_MSG_192:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Processing UART Overrun Error in UIM task", p1);
    }
    break;

    case   UIMLOG_MSG_193:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Processing reception of bad status words. not triggering recovery", p1);
    }
    break;

    case   UIMLOG_MSG_194:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_195:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_196:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Reset due to overrun errors", p1);
    }
    break;

    case   UIMLOG_MSG_197:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Reset due to bad status words", p1);
    }
    break;

    case   UIMLOG_MSG_198:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Ignoring the spurious timeout signal for T=0", p1);
    }
    break;

    case   UIMLOG_MSG_199:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Timeout on an internal fetch", p1);
    }
    break;

    case   UIMLOG_MSG_200:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM timeout in external command", p1);
    }
    break;

    case   UIMLOG_MSG_201:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: UIM Command Re-queue did not happen for cmd = 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_202:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Reset_F timeout-Hotswap-Enabled,Card-Inserted,Apply 3V", p1);
    }
    break;

    case   UIMLOG_MSG_203:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Reset_F timeout out when hotswap disabled. report to client and try a fresh poweru", p1);
    }
    break;

    case   UIMLOG_MSG_204:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM Recovery during AUTHENTICATION Command or VERIFY/CHANGE/DISABLE/ENABLE/UNBLOCK Pin command", p1);
    }
    break;

    case   UIMLOG_MSG_205:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM SILENT Recovery ", p1);
    }
    break;

    case   UIMLOG_MSG_206:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: Reset after timeout Rx-state 0x%x Tx-state 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_207:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM timeout in internal command", p1);
    }
    break;

    case   UIMLOG_MSG_208:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: USB UICC: Timeout handling for cmd 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_209:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: ATR_PPS Done", p1);
    }
    break;

    case   UIMLOG_MSG_210:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Timeout happened on interface error", p1);
    }
    break;

    case   UIMLOG_MSG_211:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Toggling INST CLA and dropping freq to 3.8MHz", p1);
    }
    break;

    case   UIMLOG_MSG_212:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Trying 3.84MHz CLK freq as a safety measure for old cards", p1);
    }
    break;

    case   UIMLOG_MSG_213:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Attempting power up at 3V", p1);
    }
    break;

    case   UIMLOG_MSG_214:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Bad PPS in WAKE_UP_F -> Resetting + UIM POLL timer reset", p1);
    }
    break;

    case   UIMLOG_MSG_215:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Entering UIM_MAX_NUM_ATTEMPTS", p1);
    }
    break;

    case   UIMLOG_MSG_216:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: Cold Reset after timeout Rx-state 0x%x Tx-state 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_217:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: UIM received the RESET F command with protocol 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_218:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Reset command come after the power down, UIM does not know the current card status", p1);
    }
    break;

    case   UIMLOG_MSG_219:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: UIM current card status 0x%x  HS status 0x%x", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_220:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Card is already removed, UIM does not need to RESET the card", p1);
    }
    break;

    case   UIMLOG_MSG_221:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: WARNING!!! TRANSACTION RESPONSE TIMED-OUT in T=1 protocol", p1);
    }
    break;

    case   UIMLOG_MSG_222:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: !!FAILED TO OBTAIN NPA HANDLE FOR /CLK/MSS/CONFIG_BUS", p1);
    }
    break;

    case   UIMLOG_MSG_223:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: UIM CLK is at %ld Hz", p1, p2);
    }
    break;

    case   UIMLOG_MSG_224:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ", p1);
    }
    break;

    case   UIMLOG_MSG_225:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Failed to register with TLMM DAL", p1);
    }
    break;

    case   UIMLOG_MSG_226:
    {
      UIM_MSG_HIGH_1("#PUP UIM_%d: Failed to register with Interrupt Controller", p1);
    }
    break;

    case   UIMLOG_MSG_227:
    {
      UIM_MSG_HIGH_0("#PUP ");
    }
    break;

    case   UIMLOG_MSG_228:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: uim_evaluate_req_ldo_voltage_mod - Evaluating req - 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_229:
    {
      UIM_MSG_HIGH_2("#PUP UIM_%d: UIMDRV received ACK from QMI - 0x%x", p1, p2);
    }
    break;

    case   UIMLOG_MSG_230:
    {
      UIM_MSG_HIGH_4("#PUP UIM_%d: UICC set shutdown upon card removal for slot is %d. m_uiccShutCardRemoval = %d, m_hs_enable = %d", p1, p2,p3,p4);
    }
    break;

    case   UIMLOG_MSG_231:
    {
      UIM_MSG_HIGH_3("#PUP UIM_%d: UICC set shutdown upon batt removal for slot is %d. m_uiccShutBattRemoval = %d", p1, p2,p3);
    }
    break;

    case   UIMLOG_MSG_232:
    {
      UIM_MSG_HIGH_4("#PUP UIM_%d: Path length is 0x%x, Path 0x%x 0x%x ", p1, p2,p3,p4);
    }
    break;

    case   UIMLOG_MSG_233:
    {
      UIM_MSG_HIGH_4("#PUP UIM_%d: path 0x%x 0x%x 0x%x", p1, p2,p3,p4);
    }
    break;

    default:
    UIM_MSG_HIGH_0("#PUP wrong tag");
    break;
  }
}/*uim_map_powerup_tag_to_f3*/
#endif /*UIM_PUP_LOGGING_PRINT_F3*/


/**
*This function adds an entry to the  circular buffer based on the tcb_ptr.
*
*  @param uim_pup_enum  tag    The tag of the msg string
*  @param uint32  p1                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p2                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p3                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p4                    The first parameter for the printf style
*                                                 msg string.
*  @param uint8   no_of_args         The number of printf style parametrs/arguments
*                                                 present in the msg string.
*/
void uim_pup_log_add
(
  uim_pup_enum tag,
  uint32 p1,
  uint32 p2,
  uint32 p3,
  uint32 p4,
  uint8 no_of_args
  )
{
  int  i = 0;

  /*this will point to the memroy slot to which the current log will be pushed to */
  uim_pup_msg_type *write_index_ptr = NULL;

  /*temp variable to hold current task's tcb_ptr*/
  rex_tcb_type  *temp_tcb_ptr;

  temp_tcb_ptr = rex_self();

  /*Check if the current task has registered for powerup logging.
      if the task has registered it will be present in the memory block table*/
  for (i=0;i<uim_pup_ptr->total_blocks;i++)
  {
    /*cross verfiy with the tcb_ptr stored in the memory block table*/
    if(temp_tcb_ptr == uim_pup_ptr->block[i]->tcb_ptr)
    {
      if((TRUE == uim_pup_ptr->block[i]->block_full) &&
         (STOP_LOGGING_WHEN_BUFFER_FULL == uim_pup_ptr->config.pup_logging_type))
      {
        return;
      }

      /*tcb_ptr  match found. retrieve the current message slot  from matched block 'i'*/
      write_index_ptr =uim_pup_ptr->block[i]->msg_array_ptr + uim_pup_ptr->block[i]->current_index;

      /*incrment the current index to next msg slot in the block*/
      ++uim_pup_ptr->block[i]->current_index;

      /*check if we have reached the last msg_slot of the block
        if yes , move to the first_slot/head/start of the memory block.  */
      if(uim_pup_ptr->block[i]->current_index >= uim_pup_ptr->block[i]->max_no_of_logs)
      {
        uim_pup_ptr->block[i]->current_index = 0;

        /*True the block_full flag. this is used to update the flush index*/
        uim_pup_ptr->block[i]->block_full = TRUE;
      }

      /*if the memory block gets overwritten, then the flush index also needs to be updated
        to always start the flushing from oldest log*/
      if(uim_pup_ptr->block[i]->block_full == TRUE)
      {
         uim_pup_ptr->block[i]->flush_index = uim_pup_ptr->block[i]->current_index;
      }
      break;
    }
  }

  /*write index will be null, the task is not registered for pup logging*/
   if(NULL == write_index_ptr)
   {
     return;
   }

  /*push the current log to the msg_slot in the memory block*/
  memset(write_index_ptr, 0x00, sizeof(uim_pup_msg_type));

  write_index_ptr->p1 = p1;
  write_index_ptr->p2 = p2;
  write_index_ptr->p3 = p3;
  write_index_ptr->p4 = p4;
  write_index_ptr->tag =  tag;
  uim_get_tstamp (&write_index_ptr->tstamp);
  write_index_ptr->no_of_args = no_of_args;
}/*uim_pup_log_add*/


/**
*This function will start the process of flushing the power up log messages to QXDM.
*
* the messages will be flushed in groups of 5 at a time to avoid bus clogging.
* and between each group of messages there is a delay of 50ms.
*
* Each memory block of the block table is flusehed one by one.
*
* In each memory block the messsages are flushed from oldest to the newest logs.
*/
void uim_pup_log_flush_to_diag (void)
{
  int i =0;

  /*Boolean to indicate if to resart timer of 250ms*/
  boolean reset_timer = TRUE;

  /*this will point to the memroy slot that will be flushed to daig */
  uim_pup_msg_type *flush_index_ptr = NULL;

  /*ptr to hold the current memory block that is being  flushed*/
  uim_pup_block_type* block_ptr =  NULL;

#ifndef UIM_PUP_LOGGING_PRINT_F3
  uim_pup_log_packet_type *log_ptr  = NULL;
#endif /*UIM_PUP_LOGGING_PRINT_F3*/

  /*Change the state to flusshing to stop pup logging*/
  if(UIM_FLUSHING != uim_pup_ptr->current_state)
  {
    uim_pup_ptr->current_state = UIM_FLUSHING;
#ifndef FEATURE_UIM_TEST_FRAMEWORK
    (void)timer_set( &uim_pup_ptr->logging_timer,
                      1000, 0, T_MSEC);
#endif
    MSG(MSG_SSID_DFLT, MSG_LEGACY_MED,"waiting for 1 sec before flushing for any pendig requests to be completed");
    return;
  }


  if((NO_OF_MAX_INSTANCES <= uim_pup_ptr->current_block) ||
      NULL == uim_pup_ptr->block[uim_pup_ptr->current_block])
  {
    reset_timer = FALSE;
  }
  else
  {
    block_ptr =  uim_pup_ptr->block[uim_pup_ptr->current_block];
    /*Flush only 5 messages at a time to avoid bus clogging*/
    for(i=0; i<5; i++)
    {
      /*--> if the memory block is not yet full, then check if the
             memory block is completely flushed before printing the log.
        --> if all the memroy blocks are flushed, then set restart timer to FLASE.*/
      if(FALSE == block_ptr->block_full &&
        (block_ptr->flush_index == block_ptr->current_index))
      {
        ++uim_pup_ptr->current_block;
        if(uim_pup_ptr->current_block >= uim_pup_ptr->total_blocks)
        {
          /*If all the blocks are cmpleted, then donot restart the timer*/
          reset_timer = FALSE;
        }
        break;
      }

      flush_index_ptr =block_ptr->msg_array_ptr + block_ptr->flush_index;

      /*Flush the msg to QXDM through diag*/
#ifdef UIM_PUP_LOGGING_PRINT_F3
      uim_map_powerup_tag_to_f3(flush_index_ptr->tag, flush_index_ptr->p1, flush_index_ptr->p2, flush_index_ptr->p3, flush_index_ptr->p4);
#else
      /*allocate memory for the log packet*/
      log_ptr = (uim_pup_log_packet_type *) log_alloc (LOG_UIM_PUP_MSG_C,sizeof(uim_pup_log_packet_type));

      if (log_ptr != NULL)
      {
        /*Populate the data into the packet*/
        log_ptr->version = 0;
        log_ptr->index = block_ptr->flush_index;
        log_ptr->tstamp.hour = flush_index_ptr->tstamp.hour;
        log_ptr->tstamp.minute = flush_index_ptr->tstamp.minute;
        log_ptr->tstamp.second = flush_index_ptr->tstamp.second;
        log_ptr->tstamp.ms = flush_index_ptr->tstamp.ms;
        log_ptr->no_of_args = flush_index_ptr->no_of_args;
        log_ptr->tag = flush_index_ptr->tag;
        log_ptr->p1 = flush_index_ptr->p1;
        log_ptr->p2 = flush_index_ptr->p2;
        log_ptr->p3 = flush_index_ptr->p3;
        log_ptr->p4 = flush_index_ptr->p4;

        /*Commit the log packet*/
        uim_log_packet_commit(SYS_MODEM_AS_ID_NONE, (void *) log_ptr);
      }
      else
      {
        MSG(MSG_SSID_DFLT, MSG_LEGACY_MED,"LOG_UIM_PUP_MSG_C(0x1903) allocation failed. Print it as an F3 msg");

        memset(uim_pup_ptr->temp_string, 0x00, sizeof(uim_pup_ptr->temp_string));

        /*Flush the msg to QXDM through F3 msg*/
        (void)snprintf( uim_pup_ptr->temp_string, sizeof(uim_pup_ptr->temp_string), 
                    "UIM_PUP_MSG[0%ld]: %02d:%02d:%02d:%03d", 
                     block_ptr->flush_index,
                     flush_index_ptr->tstamp.hour,
                     flush_index_ptr->tstamp.minute,
                     flush_index_ptr->tstamp.second,
                     flush_index_ptr->tstamp.ms);

        MSG_SPRINTF_7(MSG_SSID_DFLT, MSG_LEGACY_HIGH,"%s  tag:%x  no_of_Args:0%d  p1:%x  p2:%x  p3:%x  p4:%x " , uim_pup_ptr->temp_string,
                                flush_index_ptr->tag,
                                flush_index_ptr->no_of_args,
                                flush_index_ptr->p1,
                                flush_index_ptr->p2,
                                flush_index_ptr->p3,
                                flush_index_ptr->p4);

      }
#endif /*UIM_PUP_LOGGING_PRINT_F3*/

      /*move to the next msg_slot in the current memory block*/
      ++block_ptr->flush_index;

      /*check if we need to move to the head of the memory block*/
      if(block_ptr->flush_index >= block_ptr->max_no_of_logs)
      {
        block_ptr->flush_index = 0;
      }

      /*--> if the memory block is full and overwrittenl, then check if the
            memory block is completely flushed after printing the log.
        --> if all the memroy blocks are flushed, then set restart timer to FLASE.*/
      if(TRUE == block_ptr->block_full &&
        (block_ptr->flush_index == block_ptr->current_index))
      {
        ++uim_pup_ptr->current_block;
        if(uim_pup_ptr->current_block >= uim_pup_ptr->total_blocks)
        {
          /*If all the blocks are cmpleted, then donot restart the timer*/
          reset_timer = FALSE;
        }
        break;
      }
    }
  }

  /*Restart the timer if there are still logs remaining to be flushed*/
  if (TRUE == reset_timer)
  {
#ifndef FEATURE_UIM_TEST_FRAMEWORK
    (void)timer_set( &uim_pup_ptr->logging_timer,
                       50, 0, T_MSEC);
#endif
  }
  else
  {
    /*if all the memory blocks are flushed. then undef the timer*/
    uim_pup_ptr->current_block= 0;
    timer_undef(&uim_pup_ptr->logging_timer);

    /*Based on NV either stop logging or continue to log*/
    if(CONTINUE_LOGGING_INDEFINITELY == uim_pup_ptr->config.pup_logging_type)
    {
      uim_pup_ptr->current_state = UIM_LOGGING;
    }
    else
    {
      uim_pup_ptr->current_state = UIM_STOPPED;

     /*if the logging is stopped then clean up the globals.
       free all the memory*/
     uim_pup_log_clean();
    }
  }
}/*uim_pup_log_flush_to_diag*/


/**
*Function to generate the timestamp
*
*@param uim_pup_tstamp_type *tstamp  Pointer to timestamp
*/
void uim_get_tstamp(uim_pup_tstamp_type *tstamp)
{
  time_julian_type temp_tstamp;
  qword temp_tstamp_1;
  uint64 temp_time_in_ms;

  time_get_ms(temp_tstamp_1);
  temp_time_in_ms = (((uint64)temp_tstamp_1[1]) << 32) | ((uint64)temp_tstamp_1[0]);

#ifndef FEATURE_UIM_TEST_FRAMEWORK
  time_get_julian(&temp_tstamp);
#else
  #error code not present
#endif

  tstamp->ms = (uint16) (temp_time_in_ms % 1000);
  tstamp->hour = (uint8)temp_tstamp.hour;
  tstamp->minute = (uint8)temp_tstamp.minute;
  tstamp->second = (uint8)temp_tstamp.second;
}/*uim_get_tstamp*/


/**
*This function is to:
* - to intialize the common globals of the pup module
* - to also register the caller task to pup logging.
*
* the common globals is intilaized once in the first call.
*
* whichever calls this function, it gets registered to the pup logging module.
* by assiging/mallocing a memory block and getting added to the mem block table.
*/
void uim_pup_log_init(uim_instance_enum_type instance_id)
{
  uim_pup_block_type              *block_ptr = NULL;
  uim_pup_logging_config_efs_type pup_logging_config_efs;
  uint8                           i = 0;

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif
  /*use critical section since the same globals can be accesd by the other tasks as well*/
  rex_enter_crit_sect(&pup_log_crit_sec);
  if (FALSE == pup_globals_init)
  {
    /*set the flag pup_globals_init to TRUE so that the common globals
    are not remalloced by the other task calls */
    pup_globals_init = TRUE;

    /*allocate memory for the common globals*/
    uim_pup_ptr = uim_malloc(sizeof(uim_pup_log_global_type));

    /*if the allocation fails then leave the critical section and return*/
    if(NULL == uim_pup_ptr)
    {
      rex_leave_crit_sect(&pup_log_crit_sec);
      return;
    }

    memset(uim_pup_ptr, 0x00, sizeof(uim_pup_log_global_type));
    memset(&pup_logging_config_efs, 0x00, sizeof(uim_pup_logging_config_efs_type));

    /*read the NV from EFS to the global*/
    if (UIM_COMMON_EFS_SUCCESS == uim_common_efs_read(UIM_COMMON_EFS_UIM_PUP_LOGGING_CONFIG,
                                                      UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                                     (uim_common_efs_context_type)UIM_COMMON_EFS_CONTEXT_0,
                                                     (uint8 *)&(pup_logging_config_efs),
                                                     sizeof(uim_pup_logging_config_efs_type)))
    {
      /* copy what's read in from EFS to UIMDRV struct */
      uim_memscpy(&uim_pup_ptr->config.disable_pup_logging,
                  sizeof(uim_pup_ptr->config.disable_pup_logging),
                  &pup_logging_config_efs.disable_pup_logging,
                  sizeof(pup_logging_config_efs.disable_pup_logging));
      uim_pup_ptr->config.no_of_msgs_per_slot = pup_logging_config_efs.no_of_msgs_per_slot;
      uim_pup_ptr->config.timer_value_in_ms   = pup_logging_config_efs.timer_value_in_ms;
      uim_pup_ptr->config.pup_logging_type    = (uim_pup_logging_type)pup_logging_config_efs.pup_logging_type;
      
      if(0 != uim_pup_ptr->config.no_of_msgs_per_slot)
      {
        if((UIM_PUP_LOGGING_TIMER_VALUE_MIN > uim_pup_ptr->config.timer_value_in_ms)
           || (UIM_PUP_LOGGING_TIMER_VALUE_MAX < uim_pup_ptr->config.timer_value_in_ms))
        {
          uim_pup_ptr->config.timer_value_in_ms = UIM_PUP_LOGGING_TIMER_VALUE_DEFAULT;
        }
        if(UIM_PUP_LOGGING_NO_OF_MSGS_PER_SLOT_MAX < uim_pup_ptr->config.no_of_msgs_per_slot)
        {
          uim_pup_ptr->config.no_of_msgs_per_slot = UIM_PUP_LOGGING_NO_OF_MSGS_PER_SLOT_DEFAULT;
        }
      }
      else
      {
        /*if the no_of_msgs_per_slot is 0, then the pup logging is disalbed
          free the common globals and return*/
        uim_pup_mem_free((void**)&uim_pup_ptr);
        rex_leave_crit_sect(&pup_log_crit_sec);
        return;
      }
    }
    else
    {
      /*If efs file read fails, wirte the default values and write to efs*/
      memset(&uim_pup_ptr->config, 0x0, sizeof(uim_pup_logging_config_type));
      uim_pup_ptr->config.timer_value_in_ms = UIM_PUP_LOGGING_TIMER_VALUE_DEFAULT;
      uim_pup_ptr->config.disable_pup_logging[2] = TRUE;
      uim_pup_ptr->config.no_of_msgs_per_slot = UIM_PUP_LOGGING_NO_OF_MSGS_PER_SLOT_DEFAULT;
      uim_pup_ptr->config.pup_logging_type = STOP_LOGGING_AT_TIMER_EXPIRY;

      /* Copy data to EFS structure to eliminate enum */
      memset(&pup_logging_config_efs, 0x0, sizeof(uim_pup_logging_config_efs_type));
      pup_logging_config_efs.timer_value_in_ms      = uim_pup_ptr->config.timer_value_in_ms;
      for(i = 0; i < NO_OF_INSTANCES; i++)
      {
        pup_logging_config_efs.disable_pup_logging[i] = uim_pup_ptr->config.disable_pup_logging[i];
      }
      pup_logging_config_efs.no_of_msgs_per_slot    = uim_pup_ptr->config.no_of_msgs_per_slot;
      pup_logging_config_efs.pup_logging_type       = (uint8)uim_pup_ptr->config.pup_logging_type;

     (void)uim_common_efs_write(UIM_COMMON_EFS_UIM_PUP_LOGGING_CONFIG,
                                  UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                 (uim_common_efs_context_type)UIM_COMMON_EFS_CONTEXT_0,
                                 (const char*)&(pup_logging_config_efs),
                                 sizeof(uim_pup_logging_config_efs_type));
    }

    /*Define hte powerup logging timer and set to the nv read value*/
    timer_def(&(uim_pup_ptr->logging_timer), &(uim_pup_ptr->logging_timer_group), NULL, 0,
        (timer_t1_cb_type)uim_pup_log_flush_to_diag, (timer_cb_data_type)0);
#ifndef FEATURE_UIM_TEST_FRAMEWORK
    (void)timer_set( &uim_pup_ptr->logging_timer,
                      uim_pup_ptr->config.timer_value_in_ms, 0, T_MSEC);
#endif

    /*move the current state to logging to start logging*/
    uim_pup_ptr->current_state = UIM_LOGGING;
  }

  /*Register the current task by allocating a memory block and add it to the memory block table */
  if(NULL == uim_pup_ptr || TRUE == uim_pup_ptr->config.disable_pup_logging[instance_id])
  {
    /*Either the pup globals is null due to malloc faiure or the NV is set to diable pup logging for this nstance*/
    rex_leave_crit_sect(&pup_log_crit_sec);
    return;
  }

  /*intialize the memory block parameters*/
  block_ptr = uim_malloc(sizeof(uim_pup_block_type));
  if(NULL== block_ptr)
  {
    rex_leave_crit_sect(&pup_log_crit_sec);
    return;
  }
  memset(block_ptr, 0x00, sizeof(uim_pup_block_type));

  /*store the tcb_ptr to the block. for comparison dring logging*/
  block_ptr->tcb_ptr = rex_self();

  /*Allocate memory for this memory block. the no of msg_slots in this memory block is determied throgh the NV*/
  block_ptr->max_no_of_logs =uim_pup_ptr->config.no_of_msgs_per_slot;
  block_ptr->msg_array_ptr = uim_malloc((block_ptr->max_no_of_logs) * sizeof(uim_pup_msg_type));

  if(NULL== block_ptr->msg_array_ptr)
  {
    /*if the allocation failed then free this block parameters*/
    uim_pup_mem_free((void**)&block_ptr);
    rex_leave_crit_sect(&pup_log_crit_sec);
    return;
  }

  /*set the current index/ptr to the first msg_slot/head/start of the memory block*/
  block_ptr->current_index = 0;

  /*set the flush index/ptr to the first msg_lot/head/start of the memory block*/
  block_ptr->flush_index = 0;

  block_ptr->block_full = FALSE;

  /*Add the block to the memory table*/
  uim_pup_ptr->block[uim_pup_ptr->total_blocks] = block_ptr;
  uim_pup_ptr->total_blocks += 1;
  rex_leave_crit_sect(&pup_log_crit_sec);
}


/* Function to clean the memory allocated  in the powerup module.
*   pnce the pup logging is stopped. this function is called to free all the memory.
*/
void uim_pup_log_clean(void)
{
  int i =0;

  /*Free all the memory that was allocated*/
  if(NULL == uim_pup_ptr)
  {
    return;
  }
  for(i=0; i<uim_pup_ptr->total_blocks; i++)
  {
    if(NULL == uim_pup_ptr->block[i])
    {
      continue;
    }
    uim_pup_mem_free((void**)&(uim_pup_ptr->block[i]->msg_array_ptr));
    uim_pup_mem_free((void**)&(uim_pup_ptr->block[i]));
  }
  uim_pup_mem_free((void**)&uim_pup_ptr);
//MSG(MSG_SSID_DFLT, MSG_LEGACY_HIGH,"pup log clean");
}


/**
*Pup logging API for msgs with no arguments/parametrs
*
*  @param uim_pup_enum  tag    The tag of the msg string
*/
void uim_pup_log_add_0
(
  uim_pup_enum tag
)
{
  if((NULL == uim_pup_ptr) ||
  (UIM_LOGGING != uim_pup_ptr->current_state) ||
  (0 == uim_pup_ptr->total_blocks))
  {
    return;
  }
  uim_pup_log_add(tag,0, 0, 0, 0, 0);
}


/**
*Pup logging API for msgs with 1 arguments/parametrs
*
*  @param uim_pup_enum  tag    The tag of the msg string
*  @param uint32  p1                    The first parameter for the printf style
*                                                msg string.
*/
void uim_pup_log_add_1
(
  uim_pup_enum tag,
  uint32 p1
)
{
  if((NULL == uim_pup_ptr) ||
     (UIM_LOGGING != uim_pup_ptr->current_state) ||
     (0 == uim_pup_ptr->total_blocks))
  {
    return;
  }
  uim_pup_log_add(tag,p1, 0, 0, 0, 1);
}

/**
*Pup logging API for msgs with 2 arguments/parametrs
*
*  @param uim_pup_enum  tag    The tag of the msg string
*  @param uint32  p1                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p2                    The first parameter for the printf style
*                                                msg string.
*/
void uim_pup_log_add_2
(
  uim_pup_enum tag,
  uint32 p1,
  uint32 p2
)
{
  if((NULL == uim_pup_ptr) ||
     (UIM_LOGGING != uim_pup_ptr->current_state) ||
     (0 == uim_pup_ptr->total_blocks))
  {
    return;
  }
  uim_pup_log_add(tag,p1, p2, 0, 0, 2);
}


/**
*Pup logging API for msgs with 3 arguments/parametrs
*
*  @param uim_pup_enum  tag    The tag of the msg string
*  @param uint32  p1                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p2                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p3                    The first parameter for the printf style
*                                                msg string.
*/
void uim_pup_log_add_3
(
  uim_pup_enum tag,
  uint32 p1,
  uint32 p2,
  uint32 p3
)
{
  if((NULL == uim_pup_ptr) ||
     (UIM_LOGGING != uim_pup_ptr->current_state) ||
     (0 == uim_pup_ptr->total_blocks))
  {
    return;
  }
  uim_pup_log_add(tag,p1, p2, p3, 0, 3);
}


/**
*Pup logging API for msgs with 4 arguments/parametrs
*
*  @param uim_pup_enum  tag    The tag of the msg string
*  @param uint32  p1                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p2                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p3                    The first parameter for the printf style
*                                                msg string.
*  @param uint32  p4                    The first parameter for the printf style
*                                                 msg string.
*/
void uim_pup_log_add_4
(
  uim_pup_enum tag,
  uint32 p1,
  uint32 p2,
  uint32 p3,
  uint32 p4
)
{
  if((NULL == uim_pup_ptr) ||
     (UIM_LOGGING != uim_pup_ptr->current_state) ||
   (0 == uim_pup_ptr->total_blocks))
  {
    return;
  }
  uim_pup_log_add(tag,p1, p2, p3, p4, 4);
}

/*
*Function to free the malloced memory
*  @param void* ptr   poitner to the memory block that needs to be freed
*/

void uim_pup_mem_free(void** ptr)
{
  if(NULL != *ptr)
  {
    modem_mem_free((void*)(*ptr),MODEM_MEM_CLIENT_UIM);
    *ptr = NULL;
  }
}

#endif/*FEATURE_UIM_POWERUP_LOGGING*/

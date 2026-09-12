#ifndef UIM_POWERUP_LOGGING_H
#define UIM_POWERUP_LOGGING_H
/*===========================================================================


                U I M   P O W E R U P  L O G G I N G   H E A D E R


DESCRIPTION
    This is the header file for the uim powerup logging module

   Copyright (c) 2014 - 2016 by QUALCOMM Technologies, Inc. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/common/inc/uim_powerup_logging.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
04/25/16   sam     Added feature to print full F3s of powerup logs instead of
                   just powerup tags
03/28/16   sam     Move critical section definition from header file to source file
03/09/16   sam     Addition of version and index to LOG_UIM_PUP_MSG_C
10/27/15   hyo     Avoid writing enum to EFS 
08/07/15   sam     Conversion of the UIM powerup messages from F3 messages
                   to Log packets
09/02/15   ks      Correcting the powerup logging enums
07/16/15   sam     Fix for compiler warning
06/18/15   ks      Consolidating the F3s that print the directory path
04/08/15   sam     Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"

#ifdef FEATURE_UIM_POWERUP_LOGGING
#include "comdef.h"
#include "timer.h"
#include "time_svc.h"
#include "time_jul.h"
#include "rex.h"
#include <stringl/stringl.h>
#include "msg.h"
#include "uim_common_efs.h"
#include "uim_v.h"
#include "log.h"


/*Size of the circular buffer which stores the debug msgs*/
#define NUM_LOG_ITEMS 300

/*Default timer value of the powerup_logging timer*/
#define UIM_PUP_LOGGING_TIMER_VALUE_DEFAULT 6000
#define UIM_PUP_LOGGING_TIMER_VALUE_MIN 1000
#define UIM_PUP_LOGGING_TIMER_VALUE_MAX 65000


#define UIM_PUP_LOGGING_NO_OF_MSGS_PER_SLOT_DEFAULT 150
#define UIM_PUP_LOGGING_NO_OF_MSGS_PER_SLOT_MAX 1000

/*Size of the debug message string*/
#define STRING_SIZE   90

#define NO_OF_INSTANCES 3

#define NO_OF_MAX_INSTANCES 4

typedef enum {
  STOP_LOGGING_WHEN_BUFFER_FULL,
  STOP_LOGGING_AT_TIMER_EXPIRY,
  CONTINUE_LOGGING_INDEFINITELY
} uim_pup_logging_type;

/*Structure of uim_powerup_logging_config nv*/
typedef PACKED struct PACKED_POST {
  boolean              disable_pup_logging[NO_OF_INSTANCES];    /*provision to disable logging for a specifc slot*/
  uint16               no_of_msgs_per_slot;                     /*No of messages to be logged per slot*/
  uint16               timer_value_in_ms;                       /*Value of the powerup logging timer in ms*/
  uim_pup_logging_type pup_logging_type;                        /*weather to continue logging even after powerup*/
} uim_pup_logging_config_type;


/*Enum for tags of pup logging*/
typedef enum {
  UIMLOG_NO_MSG,
  UIMLOG_MSG_1,         /*UIM_%d: Resetting necessary UIM globals after notifying clients of error*/
  UIMLOG_MSG_2,         /*UIM_%d: Flushing_Type 0x%xFlushing 0x%x commands, uim status is 0x%x*/
  UIMLOG_MSG_3,         /*UIM_%d: After flushing Total CMD count 0x%x , Removed count 0x%x*/
  UIMLOG_MSG_4,         /*UIM_%d: Response status %d, uim_cmd_mode 0x%x*/
  UIMLOG_MSG_5,         /*UIM_%d: SW1 0x%x,SW2 0x%x, Response data length 0x%x */
  UIMLOG_MSG_6,         /*UIM_%d: Internal command to Reset the UIM*/
  UIMLOG_MSG_7,         /*UIM_%d: uim_reset_uim is not allowed, card is either not inserted or not detected on this slot*/
  UIMLOG_MSG_8,         /*UIM_%d: UIM link established with card over USB UICC interface*/
  UIMLOG_MSG_9,         /*UIM_%d: UIM link established with card over legacy ISO interface*/
  UIMLOG_MSG_10,        /*UIM_%d: In uim_process_cmd_err()*/
  UIMLOG_MSG_11,        /**/
  UIMLOG_MSG_12,        /*UIM_%d: Process cmd error in external command*/
  UIMLOG_MSG_13,        /*UIM_%d: cmd count maxed out*/
  UIMLOG_MSG_14,        /*UIM_%d: uim_command_requeued = %d*/
  UIMLOG_MSG_15,        /*UIM_%d: Process cmd error in internal command*/
  UIMLOG_MSG_16,        /*UIM_%d: Processing response for the reset command*/
  UIMLOG_MSG_17,        /*UIM_%d: Requeue the Reset command due to UIM_INSTRN_CHANGE*/
  UIMLOG_MSG_18,        /*UIM_%d: Requeue the Reset command due to UIM_CONVENTION_CHANGE*/
  UIMLOG_MSG_19,        /*UIM_%d: SFI operation failed - Retrying command without using SFI*/
  UIMLOG_MSG_20,        /*UIM_%d: Requeue the Reset command due to UIM_SWITCH_VOLTAGE*/
  UIMLOG_MSG_21,        /**/
  UIMLOG_MSG_22,        /*UIM_%d: Task clean up completed*/
  UIMLOG_MSG_23,        /*UIM_%d: select path[%d]:0x%x*/
  UIMLOG_MSG_24,        /*UIM_%d: Stream on default with hdr->channel != 0 0x%x*/
  UIMLOG_MSG_25,        /*UIM_%d: Intermediate select required for path:*/
  UIMLOG_MSG_26,        /*UIM_%d: Path[%d]:0x%x*/
  UIMLOG_MSG_27,        /*UIM_%d: AID is present: aid_length:0x%d*/
  UIMLOG_MSG_28,        /*UIM_%d: No need for intermediate select on channel 0x%x. Curr Path:*/
  UIMLOG_MSG_29,        /*UIM_%d: Updating the current directory for UICC on channel 0x%x. Path before:*/
  UIMLOG_MSG_30,        /*UIM_%d: Path after:*/
  UIMLOG_MSG_31,        /*UIM_%d: Updating the current directory for ICC - default channel. Path before*/
  UIMLOG_MSG_32,        /*UIM_%d: Path after:*/
  UIMLOG_MSG_33,        /*UIM_%d: Updating path for SFI on channel 0x%x. Path before:*/
  UIMLOG_MSG_34,        /*UIM_%d: Path after:*/
  UIMLOG_MSG_35,        /*UIM_%d: uim_notify_busy(TRUE, slot)*/
  UIMLOG_MSG_36,        /*UIM_%d: uim_notify_busy(FALSE, slot)*/
  UIMLOG_MSG_37,        /*UIM_%d: DISABLE_RECOVERY_UPON_INFINITE_NULL.NULL bytes Maxed. UIM wll power down*/
  UIMLOG_MSG_38,        /*UIM_%d: UIM will force recovery due to trans timer timeout*/
  UIMLOG_MSG_39,        /*UIM_%d: Reporting error for CDMA command 0x%x */
  UIMLOG_MSG_40,        /*UIM_%d: Received cdma command 0x%x */
  UIMLOG_MSG_41,        /*UIM_%d: Reporting result for CDMA command 0x%x*/
  UIMLOG_MSG_42,        /*UIM_%d: UIM in BTSAP client mode.Need not do Vcc power-up*/
  UIMLOG_MSG_43,        /*UIM_%d: uim power up @ 1.8 v*/
  UIMLOG_MSG_44,        /*UIM_%d: uim power up @ 3 v*/
  UIMLOG_MSG_45,        /*UIM_%d: Wrong value of uim_DI_current = 0x%x/uim_FI_current = 0x%x*/
  UIMLOG_MSG_46,        /*UIM_%d: Function uim_update_op_params should not be called*/
  UIMLOG_MSG_47,        /*UIM_%d: UIM in BTSAP client mode.Calling BTSAP API's to send cmd to remote card*/
  UIMLOG_MSG_48,        /*UIM_%d: UIM in USB mode.Calling remote API's to send cmd*/
  UIMLOG_MSG_49,        /*UIM_%d: uim_send_command*/
  UIMLOG_MSG_50,        /*UIM_%d: uim power down @ 1.8 v*/
  UIMLOG_MSG_51,        /*UIM_%d: uim power down @ 3 v*/
  UIMLOG_MSG_52,        /*UIM_%d: In Software work waiting timer expiry cb*/
  UIMLOG_MSG_53,        /*UIM_%d: In Software work waiting timer expiry cb*/
  UIMLOG_MSG_54,        /*UIM_%d: uim_bytes_waiting_in_rxfifo 0x%x  wait cnt 0x%x*/
  UIMLOG_MSG_55,        /*UIM_%d: No ATR in 40,000 clk cycles*/
  UIMLOG_MSG_56,        /*UIM_%d: Force Recovery*/
  UIMLOG_MSG_57,        /*UIM_%d: Reporting error for generic command 0x%x*/
  UIMLOG_MSG_58,        /*UIM_%d: Received generic command 0x%x */
  UIMLOG_MSG_59,        /*UIM_%d: Received internal USB UICC powerup command 0x%x "*/
  UIMLOG_MSG_60,        /*UIM_%d: Received internal Powerup command 0x%x */
  UIMLOG_MSG_61,        /*UIM_%d: Received internal USB UICC wakeup command 0x%x */
  UIMLOG_MSG_62,        /*UIM_%d: Received internal Wakeup command, UIM Entering Recovery 0x%x */
  UIMLOG_MSG_63,        /*UIM_%d: Changing the voltage class to 3V during 0x%x recovery*/
  UIMLOG_MSG_64,        /*UIM_%d: Received Reset command for UIM_HOTSWAP_CARD_INS_F*/
  UIMLOG_MSG_65,        /*UIM_%d: Received Reset command 0x%x */
  UIMLOG_MSG_66,        /*UIM_%d: USB UICC :Received reset command to icc*/
  UIMLOG_MSG_67,        /*UIM_%d: UIM is in BTSAP client mode and cannot be switched to PASSIVE MODE. */
  UIMLOG_MSG_68,        /*UIM_%d: Received reset to make UIM USB UICC passive 0x%x */
  UIMLOG_MSG_69,        /*UIM_%d: Received Reset to make UIM passive 0x%x */
  UIMLOG_MSG_70,        /*UIM_%d: Received power down command*/
  UIMLOG_MSG_71,        /*UIM_%d: Successfully mask hot swap interrupt for power down command*/
  UIMLOG_MSG_72,        /*UIM_%d: Streaming UICC on default channel*/
  UIMLOG_MSG_73,        /*UIM_%d: Not allowed to active other app on default channel"*/
  UIMLOG_MSG_74,        /**/
  UIMLOG_MSG_75,        /*UIM_%d: SFI selection is not supported*/
  UIMLOG_MSG_76,        /*UIM_%d: SFI selection is not supported*/
  UIMLOG_MSG_77,        /*UIM_%d: Reporting UIM generic command 0x%x*/
  UIMLOG_MSG_78,        /*UIM_%d: UIM Recovery Complete*/
  UIMLOG_MSG_79,        /*UIM_%d: UIM was not able to open the channel*/
  UIMLOG_MSG_80,        /*UIM_%d: Calling QMI remote API to power down the remote SIM*/
  UIMLOG_MSG_81,        /*UIM_%d: GSM instruction class  - clk freq change required to 3.8MHz*/
  UIMLOG_MSG_82,        /*UIM_%d: UIM in BTSAP client mode.Call BTSAP API's to power-up remote card*/
  UIMLOG_MSG_83,        /*UIM_%d: usb enabled 0x%x usb mode supported 0x%x fallback occured 0x%x*/
  UIMLOG_MSG_84,        /*UIM_%d: usb has remote connection 0x%x*/
  UIMLOG_MSG_85,        /*UIM_%d: Sending USB PPS*/
  UIMLOG_MSG_86,        /*UIM_%d: Sending Default PPS*/
  UIMLOG_MSG_87,        /*UIM_%d: uim_ptr->flag.iccid_select_failed is TRUE*/
  UIMLOG_MSG_88,        /**/
  UIMLOG_MSG_89,        /**/
  UIMLOG_MSG_90,        /*UIM_%d: UIM_RECOVERY_SEND_STATUS_COMMAND_ST*/
  UIMLOG_MSG_91,        /*UIM_%d: UIM_READ_BIN_MEM_ST - SFI used - 0x%x*/
  UIMLOG_MSG_92,        /*UIM_%d: UIM_UPDATE_BIN_MEM_ST - SFI used - 0x%x*/
  UIMLOG_MSG_93,        /*UIM_%d: UIM_INC_MEM_ST - SFI used - 0x%x*/
  UIMLOG_MSG_94,        /*Turning off UIM USB UICC with QMI_UIM_REMOTE_POWER_DOWN_CARD*/
  UIMLOG_MSG_95,        /*Turning off UIM USB UICC with QMI_UIM_REMOTE_POWER_DOWN_TELECOM_INTERFACE*/
  UIMLOG_MSG_96,        /*UIM_%d: Waiting for AP client to connect....*/
  UIMLOG_MSG_97,        /*UIM_%d: Turning on UIM USB UICC*/
  UIMLOG_MSG_98,        /**/
  UIMLOG_MSG_99,        /**/
  UIMLOG_MSG_100,        /*UIM_%d: Flushing manage channel command*/
  UIMLOG_MSG_101,        /*UIM_%d: No information in GSDI cache, move to Done state*/
  UIMLOG_MSG_102,        /*UIM_%d: Wrong CHV1/PIN1 %d recd for %d protocol*/
  UIMLOG_MSG_103,        /*UIM_%d: Wrong CHV2/PIN2 %d recd for %d protocol*/
  UIMLOG_MSG_104,        /*UIM_%d: No Terminal Profile data in UIM cache*/
  UIMLOG_MSG_105,        /*UIM_%d: No more channels to recover*/
  UIMLOG_MSG_106,        /*UIM_%d: Skip Manage Channel open for default Channel=%d*/
  UIMLOG_MSG_107,        /*UIM_%d: No more channels to be closed. Processing state 0x%ld*/
  UIMLOG_MSG_108,        /*UIM_%d: Process further states for default channel =%d */
  UIMLOG_MSG_109,        /*UIM_%d: Process further states for Channel=%d*/
  UIMLOG_MSG_110,        /*UIM_%d: Channel=%d is being closed, as per MMGSDI request*/
  UIMLOG_MSG_111,        /*UIM_%d: Channel=%d is being closed now*/
  UIMLOG_MSG_112,        /*UIM_%d: AID is invalid, moving to next channel*/
  UIMLOG_MSG_113,        /*UIM_%d: Send Select AID for channel=%d*/
  UIMLOG_MSG_114,        /*UIM_%d: Send UPIN verification on default channel*/
  UIMLOG_MSG_115,        /*UIM_%d: UPIN not available*/
  UIMLOG_MSG_116,        /*UIM_%d: generic_state_ptr 0x%x*/
  UIMLOG_MSG_117,        /*UIM_%d: Returning error since cmd_status = UIM_PARITY*/
  UIMLOG_MSG_118,        /*UIM_%d: LDO behaviour modified to work at specific voltage class*/
  UIMLOG_MSG_119,        /*UIM_%d: The card supports USB UICC*/
  UIMLOG_MSG_120,        /*UIM_%d: The card supports USB UICC*/
  UIMLOG_MSG_121,        /*UIM_%d: LDO behaviour modified to work at specific voltage class*/
  UIMLOG_MSG_122,        /*UIM_%d: LDO behaviour modified to work at specific voltage class*/
  UIMLOG_MSG_123,        /*UIM_%d: Select CDMA failed Command status: %x*/
  UIMLOG_MSG_124,        /*UIM_%d: ICCID read failure during recovery*/
  UIMLOG_MSG_125,        /**/
  UIMLOG_MSG_126,        /*UIM_%d: ICCID MISMATCH*/
  UIMLOG_MSG_127,        /*UIM_%d: Recovering UICC*/
  UIMLOG_MSG_128,        /*UIM_%d: Recovering ICC"*/
  UIMLOG_MSG_129,        /**/
  UIMLOG_MSG_130,        /**/
  UIMLOG_MSG_131,        /*UIM_%d: Select failure during Recovery*/
  UIMLOG_MSG_132,        /*UIM_%d: updating current directory icc - temp_ef:0x%x - file type:0x%x*/
  UIMLOG_MSG_133,        /*UIM_%d: MCC/MNC match. It is a problematic card*/
  UIMLOG_MSG_134,        /*UIM_%d: IMSI_M_PROGRAMMED bit is 0.IMSI_M not programmed*/
  UIMLOG_MSG_135,        /*UIM_%d: Removing command=%d from UIM queue*/
  UIMLOG_MSG_136,        /*UIM_%d: Reporting error for USIM command 0x%x */
  UIMLOG_MSG_137,        /*Curent envelop recieved is a location status envelope*/
  UIMLOG_MSG_138,        /*UIM_%d:   current path[%d]: 0x%x*/
  UIMLOG_MSG_139,        /*NON-TELECOM AID found*/
  UIMLOG_MSG_140,        /*UIM_%d: HOTSWAP: Drop Card inserted signal as card is already removed or me is in power down state*/
  UIMLOG_MSG_141,        /*UIM_%d: HOTSWAP: Reporting err for current cmd: 0x%x status: 0x%x*/
  UIMLOG_MSG_142,        /*UIM_%d: HOTSWAP:executing UIM_HOTSWAP_CARD_INS_F*/
  UIMLOG_MSG_143,        /*UIM_%d: HOTSWAP: Drop Card removed signal as card is inserted or ME is in power down state"*/
  UIMLOG_MSG_144,        /*UIM_%d: HOTSWAP: Reporting err for current cmd: 0x%x status: 0x%x*/
  UIMLOG_MSG_145,        /*UIM_%d: HOTSWAP: executing UIM_HOTSWAP_CARD_REM_F*/
  UIMLOG_MSG_146,        /*UIM_%d: HOTSWAP: Start, For CARD INSERTED Interrupt */
  UIMLOG_MSG_147,        /*UIM_%d: HOTSWAP: Start, For CARD REMOVED Interrupt*/
  UIMLOG_MSG_148,        /*UIM_%d: Disabled hotswap*/
  UIMLOG_MSG_149,        /*UIM_%d: HOTSWAP: Debounce logic End, Unstable status: %d [0=REMOVED 1=INSERTED]*/
  UIMLOG_MSG_150,        /*Uim internal hotswap detection*/
  UIMLOG_MSG_151,        /*UIM_%d: Starting to log the timeout Information to UimReset.Txt*/
  UIMLOG_MSG_152,        /**/
  UIMLOG_MSG_153,        /*UIM_%d: UIM_NV_INIT completed"*/
  UIMLOG_MSG_154,        /*UIM_%d: Invalid resp received setting command resp time out sig*/
  UIMLOG_MSG_155,        /*UIM_%d: Connection Unavailable event triggered.No need to call powerdown API*/
  UIMLOG_MSG_156,        /*UIM_%d: Could powerdown the remote card*/
  UIMLOG_MSG_157,        /*UIM_%d: Sending the qmi remote connect API with timer set to 0x%x*/
  UIMLOG_MSG_158,        /*UIM_%d: Could connect to the remote card*/
  UIMLOG_MSG_159,        /*UIM_%d: Couldn't powerup the remote card*/
  UIMLOG_MSG_160,        /*UIM_%d: Could reset the remote card*/
  UIMLOG_MSG_161,        /*UIM_%d: Err instrn case=0x%x.*/
  UIMLOG_MSG_162,        /*UIM_%d: Received get response command. Issuing new command*/
  UIMLOG_MSG_163,        /*UIM_%d: No response from remote sim for the command*/
  UIMLOG_MSG_164,        /*UIM_%d: RX state machine not invoked - Error in rx sub state*/
  UIMLOG_MSG_165,        /*UIM_%d: RX received with parity errors*/
  UIMLOG_MSG_166,        /*UIM_%d: RX received with parity errors while waiting for ATR*/
  UIMLOG_MSG_167,        /*UIM_%d: T=1: RX received with parity errors, to send R block*/
  UIMLOG_MSG_168,        /*UIM_%d: RX state moved to overrun state to log RX bytes*/
  UIMLOG_MSG_169,        /*UIM_%d: RX sub state invalid*/
  UIMLOG_MSG_170,        /*UIM_%d: Error processing PPS"*/
  UIMLOG_MSG_171,        /*UIM_%d: Process Unknown procedure byte*/
  UIMLOG_MSG_172,        /*UIM_%d: Received Rx break error, setting timeout signal*/
  UIMLOG_MSG_173,        /*UIM_%d: parity error in IDLE*/
  UIMLOG_MSG_174,        /*UIM_%d: Maxed the parity error count. timeout*/
  UIMLOG_MSG_175,        /*UIM_%d: Falling back to legacy ISO from IC USB interface*/
  UIMLOG_MSG_176,        /*UIM_%d: Reporting err for current cmd: 0x%x slot: 0x%x status: 0x%x*/
  UIMLOG_MSG_177,        /*UIM_%d: Received remote wake up event from USB UICC*/
  UIMLOG_MSG_178,        /*UIM_%d: CLOCK OFF when processing command*/
  UIMLOG_MSG_179,        /*UIM_%d: SIM in power down state*/
  UIMLOG_MSG_180,        /*UIM_%d: STREAMING COMMAND is not allowed for channel 0x%x*/
  UIMLOG_MSG_181,        /*Invalid cmd UIM in passive mode ret error*/
  UIMLOG_MSG_182,        /*UIM_%d: CMD_RSP Sig Rcvd uim_status=0x%x, uim_st_bf_us=0x%x, cmd_ptr=0x%x*/
  UIMLOG_MSG_183,        /*UIM_%d: Received timeout signal when remote client is active*/
  UIMLOG_MSG_184,        /*UIM_%d: Received timeout signal when remote client is active*/
  UIMLOG_MSG_185,        /**/
  UIMLOG_MSG_186,        /*UIM_%d: Attempting power up at 3V*/
  UIMLOG_MSG_187,        /*UIM_%d: HOTSWAP: card_status removed during cmd_rsp_timeout*/
  UIMLOG_MSG_188,        /*UIM_%d: Reporting err for current cmd: 0x%x Status: 0x%x*/
  UIMLOG_MSG_189,        /*UIM_%d: HOTSWAP: UIM received response time out signal  0x%x 0x%x*/
  UIMLOG_MSG_190,        /*UIM_%d: Shut down UIM due to the infinite NULL responses*/
  UIMLOG_MSG_191,        /*UIM_%d: Timed out on the command response*/
  UIMLOG_MSG_192,        /*UIM_%d: Processing UART Overrun Error in UIM task*/
  UIMLOG_MSG_193,        /*UIM_%d: Processing reception of bad status words. not triggering recovery*/
  UIMLOG_MSG_194,        /**/
  UIMLOG_MSG_195,        /**/
  UIMLOG_MSG_196,        /*UIM_%d: Reset due to overrun errors*/
  UIMLOG_MSG_197,        /*UIM_%d: Reset due to bad status words*/
  UIMLOG_MSG_198,        /*UIM_%d: Ignoring the spurious timeout signal for T=0*/
  UIMLOG_MSG_199,        /*UIM_%d: Timeout on an internal fetch*/
  UIMLOG_MSG_200,        /*UIM_%d: UIM timeout in external command*/
  UIMLOG_MSG_201,        /*UIM_%d: UIM Command Re-queue did not happen for cmd = 0x%x*/
  UIMLOG_MSG_202,        /*UIM_%d: Reset_F timeout-Hotswap-Enabled,Card-Inserted,Apply 3V*/
  UIMLOG_MSG_203,        /*UIM_%d: Reset_F timeout out when hotswap disabled. report to client and try a fresh poweru*/
  UIMLOG_MSG_204,        /*UIM_%d: UIM Recovery during AUTHENTICATION Command or VERIFY/CHANGE/DISABLE/ENABLE/UNBLOCK Pin command*/
  UIMLOG_MSG_205,        /*UIM_%d: UIM SILENT Recovery */
  UIMLOG_MSG_206,        /*UIM_%d: Reset after timeout Rx-state 0x%x Tx-state 0x%x*/
  UIMLOG_MSG_207,        /*UIM_%d: UIM timeout in internal command*/
  UIMLOG_MSG_208,        /*UIM_%d: USB UICC: Timeout handling for cmd 0x%x*/
  UIMLOG_MSG_209,        /*UIM_%d: ATR_PPS Done*/
  UIMLOG_MSG_210,        /*UIM_%d: Timeout happened on interface error*/
  UIMLOG_MSG_211,        /*UIM_%d: Toggling INST CLA and dropping freq to 3.8MHz*/
  UIMLOG_MSG_212,        /*UIM_%d: Trying 3.84MHz CLK freq as a safety measure for old cards*/
  UIMLOG_MSG_213,        /*UIM_%d: Attempting power up at 3V*/
  UIMLOG_MSG_214,        /*UIM_%d: Bad PPS in WAKE_UP_F -> Resetting + UIM POLL timer reset*/
  UIMLOG_MSG_215,        /*UIM_%d: Entering UIM_MAX_NUM_ATTEMPTS*/
  UIMLOG_MSG_216,        /*UIM_%d: Cold Reset after timeout Rx-state 0x%x Tx-state 0x%x*/
  UIMLOG_MSG_217,        /*UIM_%d: UIM received the RESET F command with protocol 0x%x*/
  UIMLOG_MSG_218,        /*UIM_%d: Reset command come after the power down, UIM does not know the current card status*/
  UIMLOG_MSG_219,        /*UIM_%d: UIM current card status 0x%x  HS status 0x%x*/
  UIMLOG_MSG_220,        /*UIM_%d: Card is already removed, UIM does not need to RESET the card*/
  UIMLOG_MSG_221,        /*UIM_%d: WARNING!!! TRANSACTION RESPONSE TIMED-OUT in T=1 protocol*/
  UIMLOG_MSG_222,        /*UIM_%d: !!FAILED TO OBTAIN NPA HANDLE FOR /CLK/MSS/CONFIG_BUS*/
  UIMLOG_MSG_223,        /*UIM_%d: UIM CLK is at %ld Hz*/
  UIMLOG_MSG_224,        /*UIM_%d: UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ*/
  UIMLOG_MSG_225,        /*UIM_%d: Failed to register with TLMM DAL*/
  UIMLOG_MSG_226,        /*UIM_%d: Failed to register with Interrupt Controller*/
  UIMLOG_MSG_227,        /**/
  UIMLOG_MSG_228,        /*UIM_%d: uim_evaluate_req_ldo_voltage_mod - Evaluating req - 0x%x*/
  UIMLOG_MSG_229,        /*UIM_%d: UIMDRV received ACK from QMI - 0x%x*/
  UIMLOG_MSG_230,        /*UIM_%d: UICC set shutdown upon card removal for slot is %d. m_uiccShutCardRemoval = %d, m_hs_enable = %d*/
  UIMLOG_MSG_231,        /*UIM_%d: UICC set shutdown upon batt removal for slot is %d. m_uiccShutBattRemoval = %d*/
  UIMLOG_MSG_232,        /*UIM_%d: Path length is 0x%x, Path 0x%x 0x%x */
  UIMLOG_MSG_233,        /*UIM_%d: path 0x%x 0x%x 0x%x*/
  UIMLOG_MAX = 400
} uim_pup_enum;

/*UIM timestamp type*/
typedef PACKED struct PACKED_POST 
{
  uint8 hour;   /*Hour of day [0 through 23]. */
  uint8 minute; /*Minute of hour [0 through 59]. */
  uint8 second; /*Second of minute [0 through 59]. */
  uint16 ms;    /*Millieseconds of a second [0 through 999]. */
}uim_pup_tstamp_type;


/* Define data structure */
LOG_RECORD_DEFINE(LOG_UIM_PUP_MSG_C)
  uint16  version;
  uint32  index;
  uim_pup_tstamp_type  tstamp;
  uint16 tag;
  uint8 no_of_args;   
  uint32 p1;
  uint32 p2;
  uint32 p3;
  uint32 p4;
LOG_RECORD_END

typedef LOG_UIM_PUP_MSG_C_type uim_pup_log_packet_type;


typedef enum
{
  UIM_PUP_NOT_INTIALISED,  /*The module is not yet intialized*/
  UIM_LOGGING,             /* the module is ready to log msgs to the memory*/
  UIM_FLUSHING,            /*The sotred pup logs are being flushed*/
  UIM_STOPPED              /*No more logging into the memory*/
}uim_pup_logging_state;


/*UIM log item type*/
typedef struct {
  uim_pup_enum tag;
  uint32 p1;
  uint32 p2;
  uint32 p3;
  uint32 p4;
  uim_pup_tstamp_type  tstamp;      /*Timestamp of the debug msg*/
  uint8 no_of_args;                 /*No of printfd style arguments of the message. the extra argumets will be set to 0 */
}uim_pup_msg_type;

/*memory block type. A memory block will be created
   for each task registration*/
typedef struct{
  rex_tcb_type        *tcb_ptr;
  uim_pup_msg_type*    msg_array_ptr;          /*ptr to the head of the current block*/
  uint32               max_no_of_logs;    /*the max no of logs/msg_slots this memory block holds*/
  uint32               current_index;     /* Index of the current msg_slot in which teh next log will be added*/
  uint32               flush_index;       /* Index of the msg_slot that will be flushed next*/
  boolean               block_full;       /*Boolean to indicate if the block is filled completely and needs to be
                                          overwritten for the next msg logging*/
}uim_pup_block_type;

/*UIM powerup logging global type*/
typedef struct {
  uim_pup_logging_state       current_state;             /*Current state of the logging module*/
  uim_pup_logging_config_type config;                    /*consists of the configuration parameters (timer value)*/
  timer_group_type            logging_timer_group;       /*powerup logging timer group*/
  timer_type                  logging_timer;             /*powerup logging timer*/
  uim_pup_block_type          *block[NO_OF_MAX_INSTANCES]; /*Memory block table. with a max possible entries of 5.
                                                           Whenever a task registers the memory block will be added to this table*/
  uint32                      total_blocks;              /*Total no of registered memory_blocks*/
  uint8                       current_block;             /*Global to hold the current block being flushed*/
#ifndef UIM_PUP_LOGGING_PRINT_F3
  char                        temp_string[100];          /*Just a global to hold a temprary string(timestamp) before flushing to diag*/
#endif /*UIM_PUP_LOGGING_PRINT_F3*/
}uim_pup_log_global_type;

extern rex_crit_sect_type  pup_log_crit_sec;
extern boolean             pup_globals_init;


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
);


/**
*Pup logging API for msgs with no arguments/parametrs
*
*  @param uim_pup_enum  tag    The tag of the msg string
*/
void uim_pup_log_add_0
(
  uim_pup_enum tag
);


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
);


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
);


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
);


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
);


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
void uim_pup_log_init(uim_instance_enum_type intstance_id);


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
  uim_pup_enum tag,
  uint32 p1,
  uint32 p2,
  uint32 p3,
  uint32 p4
  );
#endif /*UIM_PUP_LOGGING_PRINT_F3*/


/* Function to clean the memory allocated  in the powerup module.
*   pnce the pup logging is stopped. this function is called to free all the memory.
*/
void uim_pup_log_clean();


/**
*This function will start the process of flushing the power up log messages to QXDM.
*
* the messages will be flushed in groups of 5 at a time to avoid bus clogging.
* and between each group of messages there is a delay of 250ms.
*
* Each memory block of the block table is flusehed one by one.
*
* In each memory block the messsages are flushed from oldest to the newest logs.
*/

void uim_pup_log_flush_to_diag (void);


/**
*Function to generate the timestamp
*
*@param uim_pup_tstamp_type *tstamp  Pointer to timestamp
*/
void uim_get_tstamp(uim_pup_tstamp_type *tstamp);

void uim_pup_mem_free( void** ptr);

#endif /*FEATURE_UIM_POWERUP_LOGGING*/
#endif /*UIM_POWERUP_LOGGING_H*/

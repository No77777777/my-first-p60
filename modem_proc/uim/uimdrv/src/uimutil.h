#ifndef UIMUTIL_H
#define UIMUTIL_H
/*===========================================================================

        E X T E R N A L   U I M  U T I L   S T A T E   M A C H I N E
                            D E F I N I T I O N S

DESCRIPTION
  This contains all the declarations for the UIM UTIL state machine.

Copyright (c) 2003, 2011-2017, 2019-2020 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE
*****************************************************************************
$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimutil.h#4 $
$DateTime: 2020/06/16 08:44:30 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/16/20   ykr     Add macros for manage channel command
02/20/20   rkg     Add new TLV in Suscription manager
09/17/19   mm      Add EID to SimSlotStatus radio config HAL
05/10/17   ku      Switch Card Mode if no MF is indicated in ATR
04/07/17   gm      Not retrying for memory problem status word
12/21/16   ks      Workaround to select MF after call control envelope command
12/07/16   ssr     ATR Parsing Optimization
03/28/16   sam     Support for SAM card
03/09/16   na      Iccid length is being filled from file size received in FCP of EF-ICCID
12/16/15   ssr     Delay local card power up in Modem SSR in BTSAP mode
11/23/15   sam     Added workaround to select MF after call connect envelope
11/02/15   sam     Added util function to check if a given AID is ARA-M aid
08/20/15   ks      Resending error notification if mmgsdi_notigy_error fails
08/07/15   ks      Updating the select file type appropriately 
06/30/15   lm      Enable memory leak detection
06/11/15   ks      New NV to protect workarounds for non-spec compliant cards
06/11/15   ks      Select MF after EF-PSLOCI update for a buggy card
04/07/15   ks      Select MF after EF-LOCI update for a buggy card
07/03/14   sam     Explicit MF selection after location status envelop when 
                   NV 'UIMDRV_FEATURE_ENABLE_EXPLICIT_SELECTION_OF_MF_OR_ADF' is enabled
06/11/14   ll      Add interruptible time delay and return value for update params
06/04/14   na/js   Handling of case 4 commands for non telecom apps
03/21/14   ll      Remove return_ptr and remove/change uim_free to UIM_FREE
01/14/14   nmb     Move MANAGE CHANNEL command to generic cmd processing
01/14/14   nmb     Remove ACCESS_F and related unused functionality
09/16/13   yk      Removed the code for old targets and for old features
08/13/31   js      UIMDRV Memory De-Couple changes
07/12/13   nmb     Global Reorganization
12/05/12   js      UIM Parallel Processing changes
02/18/12   av      Cleaned up remaining ALWAYS ON features
09/09/11   ssr     Fix to allow the reset command when switch is un-pressed where
                   client want to disable the hotswap interrupt at runtime,
                   Added new API to return the real time card status,
                   Added new API to disable the hotswap interrupt
09/01/11   ak      GPIO hotswap support for 9x15
08/11/11   nmb     Expanded Logical Channels
04/25/11   yb      Adding support for hotswap
04/03/06   pv      Lint Fixes
02/10/03   jk      Created module.  This module contains the util state
                   machine definitions.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "target.h"
#include "comdef.h"
#include "uimi.h"
#include "qmi_uim_remote.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#define UIM_FIRST_EXTENDED_CHANNEL               0x04
#define UIM_NO_LOGICAL_CHANNEL_SET               0xFF
/* Proactive command tag */
#define UIM_PROACTIVE_CMD_TAG                    0xD0
/* Command Tag for More Time */
#define PROA_MORE_TIME                           0x02

/* envelope command tags*/
#define UIM_EVT_DOWNLOAD_ENVELOPE                0xD6
#define UIM_ENVELOPE_EVENT_TAG                   0x19
#define UIM_LOCATION_STATUS_EVT                  0x03
#define UIM_CALL_CONNECT_EVT                     0x01
#define UIM_CALL_CONTROL_TAG                     0xD4
#define UIM_ENV_2_BYTE_LENGTH_FIRST_VALUE        0x81

#define UIM_MAX_NOTIFICATION_RETRIES             0x03

/* 5 millisec */
#define UIM_DELAY_BEFORE_RETRYING_ERROR_NOTIFICATION 0x05

/* Offsets & lengths used in AID check */
#define UIM_EF_DIR_TEMPLATE_TAG_OFFSET            (0)
#define UIM_EF_DIR_TEMPLATE_LEN_OFFSET            (1)
#define UIM_EF_DIR_AID_TAG_OFFSET                 (2)
#define UIM_EF_DIR_AID_LEN_OFFSET                 (3)
#define UIM_EF_DIR_LABEL_TAG_OFFSET(aid_length)   ((aid_length) + 4)
#define UIM_EF_DIR_LABEL_LEN_OFFSET(aid_length)   ((aid_length) + 5)
#define UIM_EF_DIR_LABEL_VALUE_OFFSET(aid_length) ((aid_length) + 6)
#define UIM_EF_DIR_APP_TEMPLATE_MIN_LEN           (3)
#define UIM_EF_DIR_APP_TEMPLATE_ENTRY_MIN_LEN     (UIM_EF_DIR_AID_LEN_OFFSET + 1)
#define UIM_APP_TEMPLATE_TAG                      0x61
#define UIM_APP_ID_TAG                            0x4F

/* Offsets and lengths used to parse ASN1 formatted data */
#define UIM_ASN1_GEN_TAG_LEN_LOW_TAG_NUMBER_FORM            0x01
#define UIM_ASN1_GEN_TAG_LEN_HIGH_TAG_NUMBER_FORM           0x02
#define UIM_ASN1_LEN_ZERO                                   0x00
#define UIM_ASN1_MAX_TAG_FIELD_LEN                          0x02
#define UIM_ASN1_FIRST_TAG_INDEX                            0x00
#define UIM_ASN1_SECOND_TAG_INDEX                           0x01

#define UIM_ASN1_MAX_NUM_CMD_CMD_PARSING                    0x02
#define UIM_ASN1_TLV_SINGLE_TAG_MIN_LEN                     0x02
#define UIM_ASN1_MAX_NUM_BPP_88_TLVS                        0x02
#define UIM_ASN1_LOW_NIBBLE_MASK                            0x0F
#define UIM_ASN1_GEN_TAG_CONTEXT_SPECIFIC_FIRST_CONSTRUCTED 0xBF
#define UIM_ASN1_GEN_TAG_CONTEXT_SPECIFIC62_SECOND          0x3E
#define UIM_ASN1_GEN_TAG_APPLICATION26                      0x5A

#define UIM_ONE_BYTE_LEN                                    0x80
#define UIM_NUM_LEN_BYTES_MASK                              0x7F
#define UIM_SVN_LEN                                         0x03
#define UIM_SVN_BYTE1_DEFAULT                               0x01
#define UIM_SVN_BYTE2_DEFAULT                               0x00
#define UIM_EUICC_CHALLENGE_LEN                             0x10

#define UIM_ASN1_PPR1_MASK                                  0x40
#define UIM_ASN1_PPR2_MASK                                  0x20
#define UIM_ASN1_PPR3_MASK                                  0x10
#define UIM_ASN1_USING_CAT_MASK                             0x01
#define UIM_ASN1_USING_SCWS_MASK                            0x02
#define UIM_ASN1_USER_CONSENT_REQUIRED_MASK                 0x80
#define UIM_ASN1_ONE_BYTE_BIT_STRING                        0x01
#define UIM_ASN1_TWO_BYTE_BIT_STRING                        0x02
#define UIM_ASN1_8BIT_STRING_MAX_LEN                        0x08
#define UIM_ASN1_TAG_SELECT_ISDR                            0x6F
#define UIM_ASN1_GEN_TAG_PRIVATE0                           0xE0
#define UIM_ASN1_TAG_ANY_LOW_TAG_NUMBER_FORM                0x00
#define UIM_ASN1_GEN_TAG_CONTEXT_SPECIFIC2                  0x82
#define UIM_ASN1_GEN_TAG_BIT_STRING                         0x03
#define UIM_ASN1_SUPPORT_LEN                                0x01
#define UIM_SVN_BYTE1_DEFAULT                               0x01
#define UIM_SVN_BYTE2_DEFAULT                               0x00


/* UIMDRV Manage channel command Macros */
#define UIM_MANAGE_CHANNEL_P1_OPEN                          0x00
#define UIM_MANAGE_CHANNEL_P1_CLOSE                         0x80

typedef enum {
  GET_EID,
  INTERNAL_PRIVATE0,
  INTERNAL_BIT_STRING,
  ANY_TLV_TYPE,
  UNKNOWN_TLV_TYPE
} uim_asn1_tlv_type_type;

typedef struct
{
  uint16         tag_len;
  uint8          tag[2];
} uim_asn1_tag_field_type;

typedef struct {
  uint8                                   num_of_tlv_types;
  uim_asn1_tlv_type_type                  tlv_type[2];
  uim_asn1_tag_field_type                 tlv_tag[2];
} uim_asn1_cmds_type;

typedef struct {
  uint8                                   svn[UIM_SVN_LEN];
  boolean                                 uim_e_using_cat;
  boolean                                 uim_e_using_scws;
} uim_select_isdr_resp_type;

/*===========================================================================

FUNCTION UIM_UTIL_GET_UIM_CLASS

DESCRIPTION
  Returns the appropriate standard or extended logical channel class byte.

DEPENDENCIES
  None

RETURN VALUE
  Class byte

SIDE EFFECTS
  None

===========================================================================*/
byte uim_util_get_uim_class
(
  byte             uim_ins_class,
  uim_channel_type channel
);


/*===========================================================================

FUNCTION UIM_MALLOC

DESCRIPTION
  The uim_malloc called TMC alloc with different heap_size based the size
  of the allocation required.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  a pointer is allocated.
===========================================================================*/
void * uim_malloc(dword size);

/*===========================================================================

FUNCTION UIM_MEMSCPY

DESCRIPTION
  This Function provides the wrapper to memscpy, which is a secure version of
  memory copy

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_memscpy
(
  void        *dst_buff,
  size_t       dst_len,
  const void  *src_buff,
  size_t       src_len
);


/*===========================================================================

FUNCTION UIM_FORMAT_CHV

DESCRIPTION
  This procedure memsets the trailing characters in the CHV buffer with pad
  character (0xFF)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the value of the CHV passed in as a parameter.

===========================================================================*/
void uim_format_chv
(
  byte                     *chv,   /* This points to the CHV buffer that holds the CHV digits.
                                      The digits in this buffer are formatted  */
  uim_instance_global_type *uim_ptr
);


/*========================================================================

FUNCTION UIM_PROCESSING_STREAM_APDU

DESCRIPTION
  This function returns whether UIM is processing a stream apdu command.

DEPENDENCIES
  None

RETURN VALUE
  TRUE : UIM processing STREAM APDU COMMAND
  FALSE: UIM not processing STREAM APDU COMMAND

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_processing_stream_apdu
(
  uim_instance_global_type *uim_ptr
);


/*===========================================================================

FUNCTION UIM_IS_MORE_TIME_PCMD_FETCHED

DESCRIPTION
  This procedure tells whether the fetched command is MORE TIME or not

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if the data buffer contains MORE TIME proactive command
  FALSE - if the data buffer does not contain MORE TIME proactive cmd

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_is_more_time_pcmd_fetched
(
  const uint8    *data
);

/*===========================================================================

FUNCTION uim_is_location_status_envelope

DESCRIPTION
The function verifies if the current envelop is location status envwlop or not
DEPENDENCIES
  None

RETURN VALUE
  TRUE -- if location status envelop
  FALSE -- if not a location status envelop

SIDE EFFECTS
 NONE
===========================================================================*/
boolean uim_is_location_status_envelope
(
  const uint8    *data
);


/*===========================================================================

FUNCTION uim_is_call_connect_envelope

DESCRIPTION
The function verifies if the current envelop is call connect envwlop or not
DEPENDENCIES
  None

RETURN VALUE
  TRUE -- if call connect envelop
  FALSE -- if not a call connect envelop

SIDE EFFECTS
 NONE
===========================================================================*/
boolean uim_is_call_connect_envelope
(
  const uint8    *data
);


/*===========================================================================

FUNCTION uim_is_call_control_envelope

DESCRIPTION
The function verifies if the current envelop is call control envwlop or not
DEPENDENCIES
  None

RETURN VALUE
  TRUE -- if call control envelop
  FALSE -- if not a call control envelop

SIDE EFFECTS
 NONE
===========================================================================*/
boolean uim_is_call_control_envelope
(
  const uint8    *data
);

/*===========================================================================

FUNCTION uim_get_file_type

DESCRIPTION
The function returns the file type of the path received as argument.

DEPENDENCIES
  None

RETURN VALUE
File type of the file path passed

SIDE EFFECTS
 NONE
===========================================================================*/

uim_file_type uim_get_file_type
(
  const uim_path_type path
);

/*===========================================================================

FUNCTION uim_is_mf_selection_needed_after_update_bin_command

DESCRIPTION
The function verifies if the current update operation is on location status EF
DEPENDENCIES
  None

RETURN VALUE
  TRUE -- if location status EF or EF PSLOCI is the one to be updated
  FALSE --if location status EF or EF PSLOCI is not the one to be updated

SIDE EFFECTS
 NONE

===========================================================================*/
boolean uim_is_mf_selection_needed_after_update_bin_command
(
  uim_path_type *cmd_path_ptr,
  uim_instance_global_type *uim_ptr
);


/*===========================================================================

FUNCTION uim_is_ef_gsm_loci

DESCRIPTION
  The function verifies if the current operation is on EF-LOCI

DEPENDENCIES
  None

RETURN VALUE
  TRUE -- If current file is ef_loci
  FALSE -- If curent file is not ef_loci

SIDE EFFECTS
 NONE
===========================================================================*/
boolean uim_is_ef_gsm_loci
(
  uim_path_type *cmd_path_ptr
);


/*===========================================================================

FUNCTION uim_clk_busy_wait

DESCRIPTION
  This function is a wrapper around clk_busy_wait to avoid data conversion
  issues for different targets and to validate the supplied values.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_clk_busy_wait(dword busy_wait_time);


/*===========================================================================

FUNCTION uim_timed_sleep_ext

DESCRIPTION
  This function is a wrapper around rex_timed_wait() to replace rex_sleep()
  with a non-deferrable timer with additional signals to wait for if needed.

DEPENDENCIES
  None

RETURN VALUE
  return the signals

SIDE EFFECTS
  None

===========================================================================*/
rex_sigs_type uim_timed_sleep_ext
(
  unsigned long ms,
  uim_instance_global_type *uim_ptr,
  rex_sigs_type             additional_sigs_to_wait
);

/*===========================================================================

FUNCTION uim_timed_sleep

DESCRIPTION
  This function is a wrapper around rex_timed_wait() to replace rex_sleep()
  with a non-deferrable timer with additional signals to wait for if needed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_timed_sleep
(
  unsigned long ms,
  uim_instance_global_type *uim_ptr,
  rex_sigs_type             additional_sigs_to_wait

);


/*===========================================================================
FUNCTION UIM_MAX_CH_NUM_OPENED

DESCRIPTION
  This function returns the value of the maximum channel number which was
  opened before recovery

PARAMETER
  None

DEPENDENCIES
  uim_is_ch_open

RETURN VALUE
  uim_channel_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
uim_channel_type uim_max_ch_num_opened
(
  uim_instance_global_type *uim_ptr
);

/*===========================================================================

FUNCTION UIM_START_POWER_SAVE

DESCRIPTION
  For USB cards this function informs the USB driver that the USB interface is
  not currently needed for any ICCD transaction and hence it can be suspended.
  For legacy ISO cards this function processes a request to turn the UIM clock
  off. It uses a global control variable to determine how to control the clock.

DEPENDENCIES
  For legacy ISO cards this function uses a global variable to determine
  how to control the UIM clock.

RETURN VALUE
  TRUE/FALSE.

SIDE EFFECTS
  For USB cards if all functional interfaces over the USB interface have voted
  for a suspend, the USB interface to the card will be suspended.
  For legacy ISO cards the UIM clock will either be left high or low or will be
   left on, depending on the preferences of the UIM card.

===========================================================================*/
boolean uim_start_power_save
(
  uim_instance_global_type *uim_ptr
);

/*===========================================================================

FUNCTION UIM_STOP_POWER_SAVE

DESCRIPTION
  For USB cards this function sends a request to the USB driver to resume
  the USB interface as we need to communicate with the card over the ICCD
  functional interface.
  For legacy ISO cards this function processes a request to turn the UIM clock
  back on. It uses a global control variable to determine how to control the
  clock. It also uses a global variable to determine what frequency to set
  the clock.

DEPENDENCIES
  For legacy ISO cards this function uses a global variable to determine how
  to control the UIM clock and another global variable to determine the
  clock frequency.

RETURN VALUE
  None

SIDE EFFECTS
  For USB cards the USB driver will be requested to resume the interface.
  For ISO cards the UIM clock will be turned on and set to a frequency
  previously determined.

===========================================================================*/
void uim_stop_power_save
(
  uim_instance_global_type *uim_ptr
);

/*===========================================================================
FUNCTION UIM_IS_TELECOM_AID

DESCRIPTION
 This function is used to determine apps based on AID AID could belong to
 TELECOM as well as NON-TELECOM.For NON-TELECOM app,GET RESPONSE will not be
 issued upon receipt of sw1 as 0x62 and sw2 as 0x83

DEPENDENCIES
  None

RETURN VALUE
  TRUE   if AID is TELECOM or non-ADF type file is present, also default return value.
  FALSE  if AID is NON-TELECOM.

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_is_telecom_aid
(
  uim_aid_type aid
);


/*===========================================================================
FUNCTION UIM_IS_ARAM_AID

DESCRIPTION
 This function is used to determine is the AID is an ARA-M app AID

DEPENDENCIES
  None

RETURN VALUE
  TRUE   if AID is ARA-M AID.
  FALSE  if AID is not ARA-M AID.

SIDE EFFECTS
  None

===========================================================================*/
boolean uim_is_aram_aid
(
  uim_aid_type aid
);


#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif


/*===========================================================================
FUNCTION UIM_UTIL_MMGSDI_NOTIFY_CARD_ERROR

DESCRIPTION
 This function is a wrapper function to notify error to mmgsdi

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_util_mmgsdi_notify_card_error
(
  uim_instance_global_type *uim_ptr,
  uim_status_type error_condition,
  uim_slot_type   slot,
  boolean         temporary_error
);

/*===========================================================================
FUNCTION UIM_UTIL_GET_EF_FILE_SIZE

DESCRIPTION
 This function is used to retrieve the file size from FCP parameter

DEPENDENCIES
  None

RETURN VALUE
  File size of EF

SIDE EFFECTS
  None

===========================================================================*/
unsigned int uim_util_get_ef_file_size
(
  uim_instance_global_type *uim_ptr,
  uim_rsp_buf_type         *rsp_ptr
);


/*===========================================================================
FUNCTION UIM_PROCESS_STREAM_ISO7816_APDU_F_RESPONSE

DESCRIPTION
 This function is used to determine is the AID is an ARA-M app AID

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void uim_process_stream_iso7816_apdu_f_response
(
  uim_rsp_buf_type const   *rsp_ptr,
  uim_instance_global_type *uim_ptr
);


/*===========================================================================
FUNCTION UIM_PROCESS_VOLTAGE_MISMATCH

DESCRIPTION
 This function is used to process voltage mismatch

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void uim_process_voltage_mismatch
(
  rex_sigs_type            *i_mask,    /* Mask of signals interested in */
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      status
);



/*===========================================================================
FUNCTION UIM_PROCESS_CONVENTION_MISMATCH

DESCRIPTION
 This function is used to process convention mismatch

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void uim_process_convention_mismatch
(
  rex_sigs_type            *i_mask,    /* Mask of signals interested in */
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      status
);


/**
 * This function is used to parse ATR bytes and store them into global
 *
 * @param uim_ptr           Pointer to the UIM global instance
 * @param atr_len           Length of ATR bytes
 * @param atr_bytes_ptr     ATR Bytes received from UICC/ICC
 * 
 * @return  boolean          TRUE if decoded successfully else FALSE    
 *
 */
boolean uim_util_atr_parse_and_populate
(
  uim_instance_global_type *uim_ptr,
  uint8                     atr_len, 
  uint8                    *atr_bytes_ptr
);


/**
 * This function is used to validate the format bytes
 *
 * @param  uim_ptr           Pointer to the UIM global instance
 * 
 * @return boolean           TRUE if format byte is correct else FALSE    
 *
 */
boolean uim_util_is_format_byte_valid_in_atr
(
  uim_instance_global_type *uim_ptr
);


/**
 * This function is used to validate the protocol in ATR
 *
 * @param uim_ptr      Pointer to the UIM global instance
 * 
 * @return boolean     TRUE if TD1 < TD2 and so on else FALSE
 *
 */
boolean uim_is_protocol_valid_in_atr
(
  uim_instance_global_type *uim_ptr
);


/**
 * This function is used to retrieve len of historical byte present in ATR
 *
 * @param uim_ptr          Pointer to the UIM global instance
 * 
 * @return  uint8          Available Historical bytes
 *
 */
uint8 uim_util_get_historical_bytes_len_from_atr
(
  uim_instance_global_type *uim_ptr
);


/**
 * This function is used to copy historical bytes in actual argument
 *
 * @param uim_ptr           Pointer to the UIM global instance
 * @param hist_bytes_ptr    Historical bytes are to be stored
 * 
 * @return  boolean         TRUE if historical bytes are present else FALSE    
 *
 */
boolean uim_util_get_historical_bytes_from_atr
(
  uim_instance_global_type *uim_ptr, 
  uint8                    *hist_bytes_ptr
);


/**
 * This function is used to check if MF is present in ATR or not
 *
 * @param   uim_ptr         Pointer to the UIM global instance 
 *
 * @return  boolean         TRUE if MF is present in ATR else FALSE    
 *
 */
boolean uim_util_is_mf_present_indicated_in_atr
(
  uim_instance_global_type *uim_ptr
);


/**
* This function is used to populate Fi and Di parameters
*
* @param uim_ptr                      Pointer to the UIM global instance
* @param voltage_class_supported_ptr  Pointer to voltage_class_supported_ptr
*
* @return  boolean                    Return TRUE if volatge is supported else FALSE
*
*/
boolean uim_util_get_voltage_support_from_atr
(
  uim_instance_global_type      *uim_ptr,
  uim_uicc_voltage_class_type   *voltage_class_supported_ptr
);


/**
* This function is used to populate clock indicator
*
* @param uim_ptr              Pointer to the UIM global instance
* @param clk_cntrl_ptr        Pointer to clk_cntrl_ptr
*
* @return  boolean            FALSE if validation is failed else TRUE
*
*/
boolean uim_util_get_clock_stop_indicator_from_atr
(
  uim_instance_global_type    *uim_ptr,
  uim_clock_stop_cntrl_type   *clk_cntrl_ptr
);


/**
* This function is used to check whether T=0 protocol is supported or not
*
* @param uim_ptr           Pointer to the UIM global instance
*
* @return  boolean         FALSE if it supports else TRUE
*
*/
boolean uim_util_is_T0_supported_in_atr
(
  uim_instance_global_type  *uim_ptr
);


/**
* This function is used to check whether T=1 protocol is supported or not.
*
* @param uim_ptr         Pointer to the UIM global instance
*
* @return  boolean       FALSE if it is not supported else TRUE
*
*/
#if defined( FEATURE_UIM_T_1_SUPPORT )
boolean uim_util_is_T1_supported_in_atr
(
  uim_instance_global_type *uim_ptr
);
#endif /* FEATURE_UIM_T_1_SUPPORT */

/**
* This function is used to populate specific parameters
*
* @param uim_ptr                Pointer to the UIM global instance
* @param specifi_mode_ptr       Pointer to specifi_mode_ptr
*
* @return  boolean              FALSE if validation is failed else TRUE
*
*/
boolean uim_util_get_specific_mode_info_from_atr
(
  uim_instance_global_type      *uim_ptr,
  uim_global_specific_mode_type *specifi_mode_ptr
);

/**
 * This function is used to check whether PPS to be performed or not.
 *
 * @param uim_ptr           Pointer to the UIM global instance
 * 
 * @return  boolean         TRUE if PPS to be performed else FALSE
 *
 */
boolean uim_util_is_pps_to_be_performed_in_atr
(
  uim_instance_global_type *uim_ptr
);


/**
* This function is used to populate Fi and Di parameters
*
* @param uim_ptr      Pointer to the UIM global instance
* @param FI_ptr       pointer to FI_ptr
* @param DI_ptr       pointer to DI_ptr
*
* @return  boolean    FALSE if validation failed else TRUE
*
*/
boolean uim_util_get_fi_di_from_atr
(
  uim_instance_global_type               *uim_ptr,
  uim_clock_rate_conversion_factor_type  *FI_ptr,
  uim_baud_rate_adjustment_factor_type   *DI_ptr
);


/**
* This function is used to get the TC1 from ATR 
*
* @param uim_ptr           Pointer to the UIM global instance
* @param tc1_ptr           pointer to tc1
* @param t1_guard_time     Pointer to t1 guard time
*
* @return  boolean         FALSE if it falls out of specified value else TRUE
*
*/
boolean uim_util_get_tc1_byte_from_atr
(
  uim_instance_global_type  *uim_ptr,
  uint8                     *tc1_ptr
);


/**
* This function is used to validate Waiting Time
*
* @param uim_ptr    Pointer to the UIM global instance
*
* @return  uint8    TC2 value if it is within range or default value
*
*/
uint8 uim_util_get_waiting_time_from_atr
(
  uim_instance_global_type *uim_ptr
);


/**
* This function is used to check whether T=1 is supported or not. If it is 
* supported then it retrieves IFSC, BWI, CWI and CRC
*
* @param *uim_ptr         Pointer to the UIM global instance
*
*/
void uim_util_update_t_1_params_from_atr
(
  uim_instance_global_type  *uim_ptr
); 


/**
* This function is used to check whether CLF is supported or not
*  
* @param  uim_ptr          Pointer to the UIM global instance
*
* @return  boolean          FALSE If SWP is not supported
*                           TRUE:  SWP is supported
*/
boolean uim_util_is_swp_supported_in_atr
(
  uim_instance_global_type *uim_ptr
);


/**
* This function is used to check whether eUICC is supported or not
*
* @param  uim_ptr          Pointer to the UIM global instance
*
* @return  boolean          FALSE: If eUICC is not supported
*                           TRUE:  eUICC is supported
*/
boolean uim_util_is_euicc_supported_in_atr
(
  uim_instance_global_type *uim_ptr
);

/**
* This function is used to parse the input buffer that contains the length bytes of a TLV
  and returns back a single uint32 with length derived from those bytes
  (For length values greater than 127, the length is encoded in a multiple
  bytes)
*
* @return  uim_return_type
*
*/
uim_return_type uim_util_get_len_from_tlv 
(
  uint32                              *out_len_ptr,
  uint8                               *out_num_len_bytes_ptr,
  uint32                               in_data_len,
  uint8                               *in_data_ptr
);


/**
* This function is used to parse ASN1 TLV based on input data.
*
* @return  uim_return_type
*
*/
uim_return_type uim_util_asn1_decode_parse_tlv
(
  uint32                            data_len,
  uint8                             *data_ptr,
  uint32                            offset,
  uim_asn1_cmds_type                cmds,
  uint32                            min_len,
  uint32                            max_len,
  uint32                            *offset_out_ptr,
  uint32                            *v_len_out_ptr,
  uim_asn1_tlv_type_type            *tlv_type_out_ptr,
  boolean                           is_optional
);

/**
* This function is used to check whether AID record present in EF-DIR is valid or not
*
* @param uim_rsp_buf_type           Pointer to the response data
*
* @return  boolean                  TRUE if valid else FALSE
*
*/
boolean uim_util_is_aid_valid
(
  uim_rsp_buf_type  *uim_rsp_buf_type
);
#endif /* UIMUTIL_H */

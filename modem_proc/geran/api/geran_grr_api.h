/**
  @file geran_grr_api.h

  @brief Defines GRR functions available to software outside GERAN.
*/

/*============================================================================

  Copyright (c) 2010 - 2015 Qualcomm Technologies, Inc.

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

============================================================================*/

/* $Header: //components/rel/geran.mpss/7.4.0/api/geran_grr_api.h#2 $ */

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/10   gjohn   Initial revision
===========================================================================*/

#ifndef GERAN_GRR_API_H
#define GERAN_GRR_API_H

/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/

#include "customer.h"
#include "comdef.h"
#include "geran_grr_types.h"
#include "sys.h"
#include "sys_type.h"
#include "geran_eng_mode_read_api.h"


/*===========================================================================
  Call back declaration
===========================================================================*/

typedef void (*pscan_completion_cb_fn)(word, sys_modem_as_id_e_type);

/**
  @brief Determine the band that a GERAN ARFCN belongs to.

  This function determines the band associated with a GERAN ARFCN.  If the
  ARFCN is in the range of ARFCNs in which DCS 1800 and PCS 1900 overlap (512
  to 810) then the band indicator will be used to resolve the band correctly.

  The following table defines which bands correspond to which input:

  --------------------------------------------------
  arfcn      | band_ind | band
  --------------------------------------------------
  0          | not used | SYS_SBAND_GSM_EGSM_900
  1 - 124    | not used | SYS_SBAND_GSM_PGSM_900
  128 - 251  | not used | SYS_SBAND_GSM_850
  512 - 810  | DCS_1800 | SYS_SBAND_GSM_DCS_1800
  512 - 810  | PCS_1900 | SYS_SBAND_GSM_PCS_1900
  811 - 885  | not used | SYS_SBAND_GSM_DCS_1800
  975 - 1023 | not used | SYS_SBAND_GSM_EGSM_900
  --------------------------------------------------

  @param arfcn The ARFCN for which the band is required.
  @param band_ind The band indicator associated with the ARFCN, if available.
  @param is_band_valid Pointer to boolean variable that will be set to TRUE if
                       the band returned is valid, FALSE if it is not.
  @return The band that ARFCN belongs to.

  @note For ARFCN values in the range of 1 to 124 the band is indicated as
  PGSM.  For purposes of comparison and UE support these may also be considered
  as belonging to the EGSM band as for ARFCNs in the range of 1 to 124 they are
  interchangeable.         
*/
extern sys_sband_e_type geran_grr_get_arfcn_band(
  uint16                 arfcn,
  geran_grr_band_ind_e   band_ind,
  boolean              * is_band_valid
);

extern ARFCN_T geran_grr_convert_to_arfcn_type(
  uint16 arfcn,
  geran_grr_band_ind_e band_ind
);

/**
  @brief This function replaces the macro ARFCNS_EQUAL for GERAN clients as
   the macro includes conditional compilation */
extern boolean arfcns_equal(ARFCN_T arfcn1, ARFCN_T arfcn2);

/**
  @brief Returns the frequency of the ARFCN in Hz.

  @param arfcn The ARFCN to get the frequency of.
  @return The frequency of the ARFCN in Hz.
*/
extern uint32 geran_api_grr_arfcn_frequency(const ARFCN_T arfcn);

/**
  @brief Turn DTM override on.

  This public function is called from NAS at certain times when a call 
  is established to avoid a problem where noise was heard on the audio
  which was found to be related to DTX operation. 
  
  This function allows NAS to override the DTX setting and force it
  OFF during certain signalling procedures. 
  
  geran_grr_dtx_override_off() then allows DTX to be turned on again.
*/
extern void rr_dtx_override_on(void);  // legacy
extern void geran_grr_dtx_override_on(void);
extern void geran_grr_dtx_override_on_per_subs(sys_modem_as_id_e_type as_id);

/**
  @brief Turn DTM override off.

  See rr_dtx_override_on.
*/
extern void rr_dtx_override_off(void);  // legacy
extern void geran_grr_dtx_override_off(void);
extern void geran_grr_dtx_override_off_per_subs(sys_modem_as_id_e_type as_id);

/**
  @brief Gets the maximum power output of the mobile for a given frequency.

  @param frequency - frequency required.

  @return Max output power in dBm.

  @note: Called from WCDMA (WL1).
*/
extern byte rr_get_maximum_power_output_for_frequency(ARFCN_T frequency);

/**
  @brief Starts capture of power-scan data into the provided buffer 
   
  @param memptr Pointer to a buffer to place the results 
  @param memsize Size (bytes) of the buffer 

  @return TRUE if successful; FALSE otherwise
*/
extern boolean geran_start_capture_scan_data(uint32 *memptr, uint16 memsize);

/**
  @brief Starts capture of power-scan data into the provided buffer 
   
  @param memptr Pointer to a buffer to place the results 
  @param memsize Size (bytes) of the buffer 
  @AS_ID: ASID which is passing the buffer

  @return TRUE if successful; FALSE otherwise
*/
extern boolean geran_api_start_data_capture(uint32 *memptr, uint16 memsize, sys_modem_as_id_e_type as_id);

/**
  @brief Stopss capture of power-scan data into the provided buffer 
  @paramAS_ID
  @return TRUE if successful; FALSE otherwise
*/

extern boolean geran_api_stop_data_capture(sys_modem_as_id_e_type as_id);
/**
  @brief Stops capture of power-scan data

  @return TRUE if successful; FALSE otherwise
*/
extern boolean geran_stop_capture_scan_data(void);
/**
  @brief ARFCN on which external client wants to lock 
   @param ARFCN to be locked
  @param AS_ID

  @return TRUE if successful; FALSE otherwise
*/
extern boolean geran_api_lock_channel(ARFCN_T *channel_ptr, sys_modem_as_id_e_type as_id);
/**
  @brief pointer to the function registered 
   @param call back function
  @paramAS_ID
  @return TRUE if successful; FALSE otherwise
*/
extern boolean geran_api_register_scan_complete_callback(void *callback_func_ptr, sys_modem_as_id_e_type as_id );

/**
   @brief Access fn for cfa_send_event_if_neccessary()
   
   @param message_id  RR/L2 message ID
   @param msg_ptr pointer to RR/L2 message
   
   @return TRUE if message_ID is an OTA message
   @return fpos contains FPOS of layer3_message[0] in the message
   @return isTx is TRUE if message_id is a req, FALSE if it's an ind
*/
extern boolean geran_grr_get_l3_msg_FPOS(
  const byte message_id,
  const byte *msg_ptr,
  uint32 *fpos,
  boolean *isTx
);

/*!
 * \brief API to obtain partial PLMN search results.
 * 
 * \param available_plmn_list_ptr (out)
 * 
 * \return boolean - TRUE if results valid, FALSE otherwise
 */
extern boolean rr_get_inter_rat_bplmn_prtl_result(
  sys_detailed_plmn_list_s_type *available_plmn_list_ptr
);

/*!
 * \brief Multi-SIM API to obtain partial PLMN search results.
 * 
 * \param available_plmn_list_ptr (out)
 * \param as_id (in)
 * 
 * \return boolean - TRUE if results valid, FALSE otherwise
 */
extern boolean rr_get_inter_rat_bplmn_prtl_result_sub_id(
  sys_detailed_plmn_list_s_type *available_plmn_list_ptr,
  sys_modem_as_id_e_type as_id
);

/*!
 * \brief Determines whether the opposite ASID has a GPRS data call in progress
 *
 * \param as_id (in)
 * \return boolean - TRUE if GPRS activity is detected on the opposite subscription
 */
extern boolean geran_grr_is_opposite_sub_in_data_call(
  const sys_modem_as_id_e_type as_id
);

/*!
 * \brief This API Maps Engineering Mode Band Information to correct System Band Class.
 * 
 * \param gsm_band:  Engineering Mode Band Information (in)
 * 
 * \return sys_sband_e_type: System Band Class
 */
extern sys_sband_e_type geran_map_gsm_band_to_sys_band_class(eng_mode_band_t gsm_band);
/*!
 * \brief Determines whether the opposite ASID has a GPRS data trasnsfer state or not
 *
 * \param as_id (in)
 * \return boolean - TRUE if GPRS Transfer mode activity is detected on the opposite subscription
 */
extern boolean geran_grr_is_opposite_sub_in_data_transfer_state(
  const sys_modem_as_id_e_type as_id
);

/*!
 * \brief Returns the 3GPP release version of GERAN code
 *
 * \param - none
 * \return uint8 - 3gpp release version of GERAN code
 */
extern uint8 geran_grr_get_3gpp_release_version();

/*!
 * \brief Expose encryption algo to SSGCSS module.
 *
 * \param none
 * \return none
 */
typedef struct
{
  int8                   cell_2g_enc;
  sys_modem_as_id_e_type as_id;
}rr_enc_info;

typedef void (*ssgccs_2g_cell_enc_info_msg_handler) (rr_enc_info*);

/*!
 * \Register SSGCCS 2G encryption info msg handler
 *
 * \param 2G encryption info msg handler
 *
 * \return NONE
 */
extern void register_ssgccs_2g_enc_info_msg_handler(ssgccs_2g_cell_enc_info_msg_handler msg_handler);

#endif /* GERAN_GRR_API_H */


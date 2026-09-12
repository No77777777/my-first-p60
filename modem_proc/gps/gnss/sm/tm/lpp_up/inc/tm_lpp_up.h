#ifndef _TM_LPP_UP_H
#define _TM_LPP_UP_H


/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         TM LPP Utility

GENERAL DESCRIPTION
  This file contains TM's LPP protocol utilities, which convert decoded LPP
  messages for the consumption of TmCore.  

  Copyright (c) 2011 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2014 - 2020 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
 
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* ^L<EJECT> */
/*===========================================================================
  
                         EDIT HISTORY FOR MODULE
  
   This section contains comments describing changes made to the module.
   Notice that changes are listed in reverse chronological order.
  
   $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/lpp_up/inc/tm_lpp_up.h#2 $ $DateTime: 2020/07/27 23:14:18 $ $Author: pwbldsvc $
  
   when       who     what, where, why
   --------   ---     ----------------------------------------------------------
   07/13/20   skm     Ensure we reg/dereg with TX MGR by reading correct Sub's NV for nDDS AGPS session.
   07/28/16   skm     2Timer re-arc for LPPe includes CR's(985835,1034212,1034213,1034216,1034219) 
   05/22/13   rk      Use a common data structure to store gnss time from network.
   02/27/13   rk      Enhance supl_pos_enc_lpp_error_tx to take current transation details.
                      This solution should address LPP Error with new transaction id issue [CR 456984].
===========================================================================*/
  

#include "gps_variation.h"
#include "tm_lpp_tx_mgr.h"
#include "tm_umts_up_supl.h"

#include "tm_supl_asn1.h"

/* LPP UP timer id mask. Upon timer expiry this mask is used to find whether the expired timer id
*  belongs to LPP UP module or not.
*/
#define TM_LPP_UP_TIMER_ID_DISPATCH_MASK    0x0000FF00

/* LPP UP RLI timer ids. RLI timer ids are created for each incoming LPP RLI transaction. This
*  timer is used to send a response (i.e. LPP PLI) for each LPP RLI. In LPP concurrent transaction
*  scenario more than one RLI timer ids are used from the below definitions.
*/
static uint32 const LPP_UP_RLI_TIMER_IDS[MAX_CONCUR_LPP_RLI_TRANSACTIONS] = {
  (LPP_UP_TIMER_ID_START_RANGE),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0100),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0200),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0300),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0400),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0500),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0600),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0700),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0800),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0900),
};

/* LPP UP GNSS timer ids. GNSS timer ids are used when the server request consist of either "MSA
   GNSS + OTDOA" or "GNSS + DBH" type.  */
static uint32 const LPP_UP_GNSS_TIMER_IDS[MAX_CONCUR_LPP_RLI_TRANSACTIONS] = {
  (LPP_UP_TIMER_ID_START_RANGE + 0x0A00),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0B00),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0C00),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0D00),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0E00),
  (LPP_UP_TIMER_ID_START_RANGE + 0x0F00),
  (LPP_UP_TIMER_ID_START_RANGE + 0x1000),
  (LPP_UP_TIMER_ID_START_RANGE + 0x1100),
  (LPP_UP_TIMER_ID_START_RANGE + 0x1200),
  (LPP_UP_TIMER_ID_START_RANGE + 0x1300),
}; 
#define SUPL_LPP_OTDOA_MUL_MEAS_TIMER_ID    (LPP_UP_TIMER_ID_START_RANGE + 0x1400)

/*LPP rel-12 supports 2timer callflow. LPP RLI's Eearly Response Timer ID's are created for 
  Individual RLI's with EarlyTimer Qos request .Current implementation supports maximum 2 RLI's with
  EarlyTimer Qos per SUPL session. Applicable to "GNSS+OTDOA" or "OTDOA Only" and "MSB" RLI's*/
static uint32 const LPP_EARLY_RESPONSE_TIMER_IDS[MAX_CONCUR_LPP_EARLY_TIMER_TRANSACTIONS] = {
 (LPP_UP_TIMER_ID_START_RANGE + 0x1500),
 (LPP_UP_TIMER_ID_START_RANGE + 0x1600),
};

typedef enum
{
  C_LPP_UP_STATE_GNSS_AUXI = 0x01,        /* Upon receiving LPP RLI from server, LPP UP module will enter this state
                                             to process the request & respond with measurements/position-estimate/error
                                             correctly. This C_LPP_UP_STATE_GNSS_AUXI is a sub-state of SUPL
                                             C_SUPL_CF_STATE_SUPLPOS_XCHG state */
  C_LPP_UP_STATE_OTDOA_GRT_24_MEAS = 0x02,  /* C_LPP_UP_STATE_OTDOA_GRT_24_MEAS is independent state. When more than
                                              24 OTDOA measurements are available to send, the LPP UP module will
                                              enter this state to process/send correctly. This state can coexist with
                                              C_LPP_UP_STATE_GNSS_AUXI state, when there are concurrent LPP UP
                                              transactions for GNSS and OTDOA technologies running in parallel
                                              and the measurements are delivered in certain fashion. */
  C_LPP_UP_STATE_EARLY_RESPONSE_REQ = 0x4,  /*C_LPP_UP_STATE_EARLY_RESPONSE is an independant state. Where SLP can request
                                              UE to provide Measurements/POS earlier than the final time out. UE will provide
                                              them if MEAS/POS are available or it will drop it since it is an optional RLI*/
} lpp_up_state_type;

boolean supl_lpp_gnss_otdoa_assist_proc(tm_lpp_transaction_info_s_type *p_lpp_session_status);


boolean supl_lpp_req_loc_info_proc(tm_lpp_transaction_info_s_type *p_lpp_session_status);

/* this function encodes the supl pos in suplposinit for LPP*/
boolean supl_enc_lpp_suplpos_suplposinit(
  supl_status_s_type   *p_supl_session_status,
  supl_SUPLPOSINIT*   p_Suplposinit,
  supl_Ver2_PosPayLoad_extension *p_lpp_payload
);

/*
******************************************************************************
* tm_lpp_up_suplpos_lpp_proc
*
* Function description:
*
* Process incoming SUPL POS message containing LPP payload
*
* Parameters: p_payload: pointer to SUPL POS LPP payload
*                        payload_length: Length of the payload
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/
boolean tm_lpp_up_suplpos_lpp_proc(const supl_Ver2_PosPayLoad_extension_lPPPayload *p_payload);

boolean tm_lpp_up_cb_func ( tm_lpp_tx_mgr_client_cmd_e_type  e_cmd_type,
                                      tm_lpp_transaction_info_s_type *p_lpp_tx_info);

boolean tm_lpp_prov_loc_gnss_meas_build 
( 
  const SmGnssMeasReportStructType *p_meas_reports, 
  A_GNSS_ProvideLocationInformation *p_prov_loc_info_ie, 
  boolean b_status_check_needed,
  uint8 b_ls_gnss_method_bmap 
);
  
void tm_umts_up_supl_lpp_prov_loc_info(tm_lpp_transaction_info_s_type *p_lpp_tx_info);

boolean supl_SuplPos_lpp_error_tx(const tm_lpp_transaction_info_s_type *p_lpp_tx_info_type);

/*
******************************************************************************
* tm_lpp_up_handle_final_lr_data
*
* Function description:
*
*   Handle a final LR that needs to be sent to the HSLP
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_handle_final_lr_data( supl_status_s_type *const cp_supl_session_status,
                                     tm_lpp_transaction_info_s_type * pz_lpp_tx_item_msb );

/*
******************************************************************************
* tm_lpp_up_handle_best_pos
*
* Function description:
*
*   Handle a DBH position that can be sent to the HSLP
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_handle_best_pos ( sm_FixReportStructType const *const cp_dbh_pos_report );

/*
******************************************************************************
* tm_lpp_up_init
*
* Function description:
*
*   Get LPP UP into an 'Init' state, i.e. clean all pending transactions, free
*   all resources and get ready for the next session. This can be called at 
*   bootup and subsequently as a cleanup of the LPP UP state.
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_init(void);

/*
******************************************************************************
* tm_lpp_up_handle_pa_event
*
* Function description:
*
*   Handle a configuration change by the GPS client for LPP UP.
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_handle_pa_event(prtl_pa_event_s_type *p_pa_event);

/*
******************************************************************************
* tm_lpp_up_get_lpp_cap
*
* Function description:
*
*   Return LPP UP Capabilities.
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_get_lpp_cap
(
  supl_callflow_kind_enum_type     u_callflow_type,
  supl_session_agps_mode_enum_type u_agps_mode,
  uint32                           q_supl_version,
  tm_lpp_provide_cap_s_type        *p_lpp_provide_cap
);


boolean tm_lpp_up_get_ota_reset_status( void );

void tm_lpp_up_set_ota_reset_status( boolean const cv_otdoa_ota_reset );


/*
******************************************************************************
* tm_lpp_up_reset_txtimerid_index
*
* Function description:
*   Reset the transaction timer index to zero when SUPL enters UT3 state.
*   SUPL will enter UT3 state when all the LPP transactions request (position and/or
*   measurement) is served by UE.
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_reset_txtimerid_index( void );


/*
******************************************************************************
* tm_lpp_up_handle_supl_cf_timer_expiry
*
* Function description:
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_handle_supl_cf_timer_expiry( supl_status_s_type * const p_supl_session_status );

/*
******************************************************************************
* tm_lpp_up_stop_sess_req_handler
*
* Function description:
*     This function handles the stop session request from TM-core.
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_stop_sess_req_handler( tm_sess_stop_reason_e_type const ce_StopReason,
                                      supl_status_s_type *p_supl_session_status );
/*
******************************************************************************
* tm_lpp_up_supl_handle_otoda_data
*
* Function description:
*
*   Handle OTDOA measurement data
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_handle_otdoa_data( sm_loc_lte_otdoa_meas_info_type const *p_OtdoaData,
                                  supl_status_s_type *p_supl_session_status );


/*===========================================================================

FUNCTION tm_lpp_up_handle_timer_expiry

DESCRIPTION
  This function handles all the LPP UP timer expires.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
void tm_lpp_up_handle_timer_expiry( os_TimerExpiryType const *const p_timer_param );


/*===========================================================================

FUNCTION tm_lpp_up_handle_prm_data_before_timeout

DESCRIPTION
  This function process the ready GNSS measurements before the LPP timer timeouts.

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
void tm_lpp_up_handle_prm_data_before_timeout( supl_status_s_type *const cpz_supl_session_status );

/*===========================================================================

FUNCTION tm_lpp_up_refresh_efs_nv

DESCRIPTION
  This function handles the refresh of EFS NV items.

DEPENDENCIES: none.

RETURN VALUE: none

SIDE EFFECTS: none.

===========================================================================*/
void tm_lpp_up_refresh_efs_nv(boolean v_hwRead );

/*
******************************************************************************
* tm_lpp_up_tx_mgr_reg
*
* Function description:
*
*   Register SUPL with the LPP transaction manager Module
*
* Parameters: None.
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_tx_mgr_reg(void);

/*
 ******************************************************************************
 * Function description:
 *  Encodes LPP SUPL POS INIT with providing capabilites IE and req AD
 *
 * Parameters:
 *
 * Return value:
 *
 *    TRUE/FALSE
 *
 ******************************************************************************
*/
boolean tm_lpp_up_enc_lpp_suplpos_suplposinit( supl_status_s_type   *p_supl_session_status,
                                            supl_SUPLPOSINIT *p_Suplposinit,
                                            supl_Ver2_PosPayLoad_extension *p_lpp_payload );

/*
******************************************************************************
* tm_lpp_up_set_tm_session_done
*
* Function description:
*     Set the TM session done status internally
*
* Parameters:
*   cv_TmSessDone - TM session done status
*
* Return value: None.
*
******************************************************************************
*/
void tm_lpp_up_set_tm_session_done( boolean const cv_TmSessDone );

/*
******************************************************************************
* tm_lpp_up_get_tm_session_done
*
* Function description:
*     This function return the TM session done status
*
* Parameters: None.
*
* Return value:
*   True if TM session is done otherwise false.
*
******************************************************************************
*/
boolean tm_lpp_up_get_tm_session_done( void );

/*
******************************************************************************
* tm_lpp_up_rli_received
*
* Function description:
*     This function checks if the internal LPP flag indicates RLI is received.
*
* Parameters: None.
*
* Return value:
*   True if the RLI is received, otherwise false.
*
******************************************************************************
*/
boolean tm_lpp_up_rli_received( void );

/*===========================================================================
FUNCTION lpp_up_map_agps_mode_to_tm_oper_mode

DESCRIPTION
  This function maps from SUPL agps mode type to TM operation mode

PARAMETERS: supl_session_agps_mode_enum_type e_agps_mode
   
DEPENDENCIES: none.

RETURN VALUE: tm_sess_operation_mode_e_type.

SIDE EFFECTS: none.

===========================================================================*/

tm_sess_operation_mode_e_type lpp_up_map_agps_mode_to_tm_oper_mode(supl_session_agps_mode_enum_type e_agps_mode);

#endif

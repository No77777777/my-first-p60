#ifndef TM_LPP_COMMON_UTILS_H
#define TM_LPP_COMMON_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         TM LPP Common Utility Module

GENERAL DESCRIPTION
  This file contains TM's LPP Common Utility Functions

  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc.
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved. 

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* ^L<EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/lpp_common/inc/tm_lpp_common_utils.h#1 $
 
  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  11/16/16   gk      Velocity flags not correctly stored
  08/05/15   rh      Moved carrier-specific NV 1920 (pos mode support) to TM core 
  01/14/15   rk      LPP >24 AD/Meas feature support.
  12/14/14   gk      new session events to PDAPI
  05/14/13   rk      Promoted Glonass conversion function to this LPP common module.
  02/1/12    atien   Initial version

===========================================================================*/
#include "customer.h"   /* Customer configuration file */
/*=============================================================================

                                INCLUDE FILES

=============================================================================*/
#include "comdef.h"    /* Definition for basic types and macros */
#include "msg.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "sm_nv.h"
#include "sm_nv_efs.h"

#include "tm_data.h"
#include "tm_lpp_asn1.h"
#include "tm_umts_up_supl.h"
#include "tm_lpp_tx_mgr.h"
#include "tm_lpp.h"


typedef struct
{
  uint8   mt_lr_support;                /* used by Capability: LCS value_added_lcs_supported: 0: not supported */

  /* NOTE - LPP CP enable/disable NV item is maintained in TM CORE via tm_core_get_lpp_cp_enable() */

  uint32  aagps_default_qos_time;
  uint32  aagps_default_qos_unc;
  uint32  aagps_default_ref_time_unc_50ms; 
  uint8   aagps_default_lpp_cp_timeout_secs;

} lpp_cp_nv_items_s_type;

extern lpp_cp_nv_items_s_type z_lpp_cp_nv_item_ram_copy;

/*
 ******************************************************************************
 *
 * tm_lpp_common_utils_read_nv
 *
 * Function description:
 *  This function reads NV items that are common to LPP into a structure.
 *
 * Parameters:
 *   None.
 *
 * Return value: 
 *   None.
 *
 ******************************************************************************
*/
extern void tm_lpp_common_utils_read_nv (void);

/*
 ******************************************************************************
 *
 * tm_lpp_common_utils_refresh_carrier_specific_nv
 *
 * Function description:
 *    This function refresh carrier-specific NVs after SIM hot swap
 *
 * Parameters:
 *   None.
 *
 * Return value: 
 *   None.
 *
 ******************************************************************************
*/
extern void tm_lpp_common_utils_refresh_carrier_specific_nv (void);

/*
 ******************************************************************************
 *
 * tm_lpp_common_glo_eph_svid_freq_num_map_aux 
 *
 * Function description:
 *  Associates sv ID with a frequencey number for Glonass eph. data using 
 *  information from auxilary assistance.                   
 *  
 * Parameters:
 *   p_lpp_glo_aux_info : pointer to the structure holding Glonass aux. data.
 *   p_lpp_glo_eph_freq : points to glonass eph. array struct.
 * 
 * Return value: 
 *   None.
 *
 ******************************************************************************
*/

void tm_lpp_common_glo_eph_svid_freq_num_map_aux (const GNSS_AuxiliaryInformation* const p_gnss_aux_info, prot_glo_eph_buffer_s_type p_lpp_glo_eph_freq[N_GLO_SV]);

/*
 ******************************************************************************
 *
 * tm_lpp_common_glo_acqast_svid_freq_num_map_aux 
 *
 * Function description:
 *  Associates sv ID with a frequencey number for Glonass Aquisition Assistance data using 
 *  information from auxilary assistance.                   
 *  
 * Parameters:
 *   p_lpp_glo_aux_info : pointer to the structure holding Glonass aux. data.
 *   p_lpp_glo_aa_freq : pointer to LPP Glonass aquisition assistance & auxi info struct.
 * 
 * Return value: 
 *   None.
 *
 ******************************************************************************
*/

void tm_lpp_common_glo_acqast_svid_freq_num_map_aux (const GNSS_AuxiliaryInformation* const p_gnss_aux_info,
                                                     prot_glo_acqast_buffer_s_type* p_lpp_glo_aa_freq);

/*
 ******************************************************************************
 *
 * tm_lpp_common_glo_eph_svid_freq_num_map_alm 
 *
 * Function description:
 *  Associates sv ID with a frequencey number for Glonass eph. data using 
 *  information from Almanac model for Glonass.                   
 *  
 * Parameters:
 *   p_keplerianGLONASS : pointer to the structure holding Almanac model for Glonass.
 *
 * Return value: 
 *   None.
 *
 ******************************************************************************
*/

void tm_lpp_common_glo_eph_svid_freq_num_map_alm (const AlmanacGLONASS_AlmanacSet* const p_keplerianGLONASS, prot_glo_eph_buffer_s_type p_lpp_glo_eph_freq[N_GLO_SV]);

/*
 ******************************************************************************
 *
 * tm_lpp_common_convert_to_glonass_time
 *
 * Function description:
 *  This function processes GPS offset time (deltaT) to compute the Glonass time
 *  & set the appropiate Glonass time fields in set_assist_pos_time_stamp struct.
 *  The deltaT could be from GPS UTCModel AD and/or GLONASS TimeModel AD.
 *
 * Parameters:
 *  bDeltaT [IN]: Positive GPS time offset value
 *  p_supl_session_postime [OUT]: Pointer to position time structure.
 * 
 * Return value:
 *  On success TRUE, else FALSE is return to indicate the failure.
 *
 *
 ******************************************************************************
*/
boolean tm_lpp_common_convert_to_glonass_time(const int8 bDeltaT, gnss_time_info *p_supl_session_postime);

/*
 ******************************************************************************
 *
 * tm_lpp_common_lpp_agnss_rli_proc
 *
 * FUNCTION DESCRIPTION:
 *  Process A-GNSS IEs in the LPP RLI message.
 *
 * PARAMETERS:
 *  p_req_loc_r9 : ASN.1 decoded LPP req location info IE.
 *  cp_LppErr    : LPP Error structure to populate if there is an error
 *  cm_AcceptedRliTech : Technology already accepted in this session. Used to check for
 *                       any duplicate request in concurrent call flow.
 *  cpm_RliTech : Technology currently requested by the server.
 *  cpm_RliGnssMethod: Return the GPS/GLONASS method is requested by server.
 *  cpq_SessionLppFlags: LPP session flag to indicate different request types received in the
 *                       LPP RLI from server thus far in a session.
 *  cpq_TxLppFlags : Transaction flag to indicate different request types received in the
 *                   LPP RLI from server.
 *
 * RETURN VALUE:
 *  True if there are no error, otherwise false.
 *
 * SIDE EFFECTS:
 *  No side effects
 *
 ******************************************************************************
*/
boolean tm_lpp_common_lpp_agnss_rli_proc( RequestLocationInformation_r9_IEs *p_req_loc_r9,
                                          tm_lpp_transaction_info_s_type *p_LppTxItem,
                                          rli_tx_tech_mask const cm_AcceptedRliTech,
                                          rli_tx_tech_mask *const cpm_RliTech,
                                          uint8 *const cpm_RliGnssMethod,
                                          tm_lpp_req_type *const cpq_SessionLppFlags,
                                          tm_lpp_req_type *const cpq_TxLppFlags );

/*
 ******************************************************************************
 *
 * tm_lpp_common_lpp_otdoa_rli_proc
 *
 * FUNCTION DESCRIPTION:
 *  Process OTDOA IEs in the LPP RLI message.
 *
 * PARAMETERS:
 *  p_req_loc_r9 : ASN.1 decoded LPP req location info IE.
 *  cp_LppErr    : LPP Error structure to populate if there is an error
 *  cm_AcceptedRliTech : Technology already accepted in this session. Used to check for
 *                       any duplicate request in concurrent call flow.
 *  cpm_RliTech : Technology currently requested by the server.
 *  cpq_SessionLppFlags: LPP session flag to indicate different request types received in the
 *                       LPP RLI from server thus far in a session.
 *  cpq_TxLppFlags : Transaction flag to indicate different request types received in the
 *                   LPP RLI from server.
 *
 * RETURN VALUE:
 *  True if there are no error, otherwise false.
 *
 * SIDE EFFECTS:
 *  No side effects
 *
 ******************************************************************************
*/
boolean tm_lpp_common_lpp_otdoa_rli_proc( RequestLocationInformation_r9_IEs const *const p_req_loc_r9,
                                          tm_lpp_error_type *const cp_LppErr,
                                          rli_tx_tech_mask const cm_AcceptedRliTech,
                                          rli_tx_tech_mask *const cpm_RliTech,
                                          tm_lpp_req_type *const cpq_SessionLppFlags,
                                          tm_lpp_req_type *const cpq_TxLppFlags );

/*
 ******************************************************************************
 *
 * tm_lpp_cp_check_ecid_req
 *
 * Function description:
 *  This function processes an ECID meas required request from the SMLC. The
 *  function checks whether an ECID meas required request is to be accepted,
 *  based on the current NV and LPP State Machine flags provided to it.
 *
 * Parameters:
 *  p_req_loc_r9 : ASN.1 decoded LPP req location info IE.
 *  cp_LppErr    : LPP Error structure to populate if there is an error
 *  cm_AcceptedRliTech : Technology already accepted in this session. Used to check for
 *                       any duplicate request in concurrent call flow.
 *  cpm_RliTech : Technology currently requested by the server.
 *  cpq_LppFlags : Flag to indicate different request types received in the LPP RLI from server.
 *  p_lpp_ecid_req_loc : Return the ECID measurement type
 *
 * Return value:
 *  True if there are no error, otherwise false.
 *
 * Side Effects: No side effects
 ******************************************************************************
*/
boolean tm_lpp_common_lpp_ecid_rli_proc( RequestLocationInformation_r9_IEs const *const p_req_loc_r9,
                                         tm_lpp_error_type *const cp_LppErr,
                                         rli_tx_tech_mask const cm_AcceptedRliTech,
                                         rli_tx_tech_mask *const cpm_RliTech,
                                         tm_lpp_req_type *const cpq_LppFlags,
                                         uint32 *p_lpp_ecid_req_loc );

/*
 ******************************************************************************
 *
 * tm_lpp_common_rli_loc_type_fits_pos_method
 *
 * FUNCTION DESCRIPTION:
 *  Function to validate if the requested location type match the position method.
 *
 * PARAMETERS:
 *  cz_LocType: Location type requested by server.
 *  cm_RliTech: Technology or position method requested by server.
 *
 * RETURN VALUE:
 *  True if location type match position method, otherwise false.
 *
 * SIDE EFFECTS:
 *  No side effects
 *
 * DEPENDENCIES:
 *  All location type should be resolve to either LPP_locationEstimateRequired
 *  or LPP_locationMeasurementsRequired before calling this function. The cm_RliTech should be
 *  processed by tm_lpp_common_lpp_agnss_rli_proc(), tm_lpp_common_lpp_otdoa_rli_proc,
 *  tm_lpp_common_lpp_ecid_rli_proc() function before calling this function.
 ******************************************************************************
*/
boolean tm_lpp_common_rli_loc_type_fits_pos_method( LPP_LocationInfoType const cz_LocType,
                                                   rli_tx_tech_mask const cm_RliTech );

/*
 ******************************************************************************
 *
 * tm_lpp_common_lpp_commonie_rli_proc
 *
 * FUNCTION DESCRIPTION:
 *  Process common IE in the LPP RLI message.
 *
 * PARAMETERS:
 *  p_req_loc_r9: Pointer to LPP RLI
 *  cpq_LppFlags: Flag to indicate different request types received in the LPP RLI from server.
 *  cpq_TxLppFlags: Flag to indicate different request types received in the LPP RLI tx from server.
 *
 * RETURN VALUE:
 *  None.
 *
 * SIDE EFFECTS:
 *  No side effects
 *
 ******************************************************************************
*/
void tm_lpp_common_lpp_commonie_rli_proc( RequestLocationInformation_r9_IEs const *const p_req_loc_r9, tm_lpp_req_type
                                          *const cpq_LppFlags, tm_lpp_req_type *const cpq_TxLppFlags );

/*
 ******************************************************************************
 *
 * tm_lpp_common_check_supported_rlitech_req
 *
 * FUNCTION DESCRIPTION:
 *  Check atleast one of the supported LPP technology is requested in the
 *  LPP RLI message.
 *
 * PARAMETERS:
 *  p_req_loc_r9: Pointer to LPP RLI
 *
 * RETURN VALUE:
 *  True if atleast one LPP technology is requested, otherwise FALSE.
 *
 * SIDE EFFECTS:
 *  No side effects
 *
 ******************************************************************************
*/
boolean tm_lpp_common_check_supported_rlitech_req( RequestLocationInformation_r9_IEs const *const p_req_loc_r9 );

/*
 ******************************************************************************
 *
 * tm_lpp_common_xlate_lpp_hor_qos_req
 *
 * Function description:
 *  This function processes the QOS-Time from OSYS ASN.1 decoded LPP Request
 *  Location Info and translates the horizontal accuracy QoS requested by the
 *  server into the LPP-CP value (same as PDAPI value) of horizontal QoS.
 *
 * Parameters:
 *  p_smlc_qos: pointer to QoS value sent by SMLC.
 *  p_hor_acc_meters: default value of horiz QoS accuracy
 *
 * Return value:
 *  PDAPI_HOR_QOS
 *
 * Side Effects: No side effects
 *
 ******************************************************************************
*/
uint32 tm_lpp_common_xlate_lpp_hor_qos_req( const QoS *const p_smlc_qos, const uint32 *const p_hor_acc_meters );

/*
 ******************************************************************************
 *
 * tm_lpp_common_lpp_rli_version
 *
 * FUNCTION DESCRIPTION:
 *  Verify the LPP RLI extension version is valid.
 *
 * PARAMETERS:
 *  p_req_loc_r9 : ASN.1 decoded LPP req location info IE.
 *
 * RETURN VALUE:
 *  True if there are no error, otherwise false.
 *
 * SIDE EFFECTS:
 *  No side effects
 *
 ******************************************************************************
*/
boolean tm_lpp_common_lpp_rli_version( RequestLocationInformation_criticalExtensions const *const cpcz_RLICritExt );

/*
 ******************************************************************************
 *
 * tm_lpp_common_copy_gnss_pos2tx_item
 *
 * Function description:
 *  This function copies the GNSS position estimate from TM common structure to LPP transaction
 *  item. The copy is conditional, only if the transaction item doesn't have valid GNSS position
 *  estimate already.
 *
 * Parameters:
 *  cpz_LppTransItem: pointer LPP transaction item.
 *  cpz_PendingPosToNetwork: Pointer to TM common struction containing the GNSS position.
 *
 * Return value:
 *  None
 *
 * Side Effects: No side effects
 *
 ******************************************************************************
*/
void tm_lpp_common_copy_gnss_pos2tx_item( tm_lpp_transaction_info_s_type *const cpz_LppTransItem,
                                          tm_umts_common_pos_for_net_s_type *const cpz_PendingPosToNetwork );

/*
 ******************************************************************************
 *
 * tm_lpp_common_copy_gnss_meas2tx_item
 *
 * Function description:
 *  This function copies the GNSS measurement from TM common structure to LPP transaction item. The
 *  copy is conditional, only if the transaction item doesn't have valid GNSS measurements already.
 *
 * Parameters:
 *  cpz_LppTransItem: pointer LPP transaction item.
 *  cpz_GnssTimeStamp: Pointer to GNSS time.
 *  cpz_GnssMeasReport: Pointer to TM common struction containing the GNSS measurements.
 *
 * Return value:
 *  None
 *
 * Side Effects: No side effects
 *
 ******************************************************************************
*/
void tm_lpp_common_copy_gnss_meas2tx_item( tm_lpp_transaction_info_s_type *const cpz_LppTransItem,
                                           gnss_time_info *const cpz_GnssTimeStamp,
                                           SmGnssMeasReportStructType *const cpz_GnssMeasReport );

/*
 ******************************************************************************
 *
 * tm_lpp_common_agnss_otdoa_callfow
 *
 * FUNCTION DESCRIPTION:
 *  Determine whether its A-GNSS + OTDOA hybrid call flow based on the LPP ProvAD and RLI message
 *  from server. If any of the below condition is TRUE its  A-GNSS + OTDOA hybrid call flow
 *  1. When ProvideAD sent before LPP RLI case, if only AGPS MSA AD and OTDOA AD is received.
 *  2. When ProvideAD sent after LPP RLI case, if LPP CP RLI tech is AGNSS MSA + OTDOA.
 *
 * PARAMETERS:
 *  p_req_loc_r9 : ASN.1 decoded LPP req location info IE.
 *
 * RETURN VALUE:
 *  True if there are no error, otherwise false.
 *
 * SIDE EFFECTS:
 *  No side effects
 *
 ******************************************************************************
*/
/* */
GNSS_INLINE boolean tm_lpp_common_agnss_otdoa_callfow( uint32 const cq_AGpsAdReceived,
                                                      rli_tx_tech_mask const cm_RliTech )
{
  if ( ((cq_AGpsAdReceived & C_GPS_ASSIS_ACQ) != 0) ||
       ((C_LPP_TECH_OTDOA_MSA | C_LPP_TECH_GNSS_MSA) == cm_RliTech) )
  {
    return ( TRUE );
  }
  return ( FALSE );
}

/*
 ******************************************************************************
 *
 * tm_lpp_common_set_error_code
 *
 * Function description:
 *  This function to analyze some LPP & LPPe error code & RLI to set appropiate
 *  decisive error codes.
 *
 * Parameters:
 *  cpz_LppTxErr: pointer LPP/LPPe error structure
 *  m_RliTech: RLI tech mask
 *
 * Return value:
 *  True if error is concluded & final. The LPP/LPPe function can stop processing further.
 *  otherwise false.
 *
 * Side Effects: No side effects
 *
 ******************************************************************************
*/
boolean tm_lpp_common_set_error_code( tm_lpp_error_type *const cpz_LppTxErr, rli_tx_tech_mask const m_RliTech );


/*
 ******************************************************************************
 *
 * tm_lpp_common_calc_resp_time_from_run_rli_timer
 *
 * Function description:
 *  Compute the response timeout from the running remaining RLI timer.
 *
 * Parameters:
 *  cpz_rli_timer - Pointer the RLI timer object.
 *  cq_rliTimeoutShimMsec - RLI timeout shim based on whether in OTDOA only
 *                          N/W simulation test mode or no.
 *  cq_OtdoaEngineIpcDelay - Expected IPC delay from OTDOA engine to this module.
 *
 *  Return value:
 *    Non-zero value if the response time is more than the IPC delay timeout, otherwise return zero.
 *
 ******************************************************************************
*/
uint32 tm_lpp_common_calc_resp_time_from_run_rli_timer( os_TimerMsgType *const cpz_rli_timer,
                                                        uint32 const cq_rliTimeoutShimMsec,
                                                        uint32 const cq_OtdoaEngineIpcDelay
                                                       );

/*
 ******************************************************************************
 *
 * tm_lpp_send_req_loc_type_to_pdapi
 *
 * Function description:
 *  This function sends a PDAPI event to indicate the type or Positioning request
 *  from the network
 *
 * Parameters:
 * 
 * Return value:
 *    None 
 *
 ******************************************************************************
*/
void tm_lpp_send_req_loc_type_to_pdapi(tm_lpp_data_type    z_lpp_data_type,
                                         pdsm_pd_comm_protocol_e_type e_protocol_type);

#ifdef __cplusplus
}
#endif

#endif /* TM_LPP_COMMON_UTILS_H */


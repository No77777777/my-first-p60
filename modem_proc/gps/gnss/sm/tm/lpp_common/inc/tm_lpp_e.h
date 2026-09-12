#ifndef __TM_LPP_E_H__
#define __TM_LPP_E_H__
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         TM LPP Transaction Manager module

GENERAL DESCRIPTION
This file contains TM's LPPe protocol utilities, which encode/decode LPP messages 

Copyright (c) 2016 - 2019 Qualcomm Technologies, Inc.
Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* ^L<EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/lpp_common/inc/tm_lpp_e.h#1 $ $DateTime: 2019/10/24 01:30:36 $ $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  02/22/19   mj      Location Privacy Feature
  04/11/18   sj      Fix bad DBH Punc in LPPe
  08/08/17   rk      Avoid reporting DBH for MO E911 sessions.
  02/17/17   skm     BLE and Barometer support for LPPE Phase 2
  01/30/16   sj      Initial creation.

===========================================================================*/
#include "customer.h"
#ifdef FEATURE_GNSS_LPPE
#include "comdef.h"
#include "tm_lpp_asn1.h"
#include "tm_lppe_asn1_dec.h"
#include "tm_lppe_asn1_enc.h"
#include "tm_lpp_tx_mgr.h"
#include "tm_auxtech_iface.h"
#include "tm_lpp_e_asn1util.h"

#define TM_LPPE_COMPATIBILITY_LEVEL    (0)
#define TM_LPPE_MAJ_VERSION            (1)
#define TM_LPPE_MINOR_VERSION          (0)

/*Uncertianity cap as per LPPe spec*/
#define C_TM_LPPE_UNC_CAP_METERS_EXT   (200.0)
#define C_TM_LPPE_VERT_UNC_CONF        (68)


/*************************************************************************
*  EPDUFailureCause:
*  Use this failure cause when tm_lpp_error_type.w_ErrType has
*  LPP_ERROR_TYPE_PROV_LOC_EPDU_ERROR
*************************************************************************/
typedef enum
{
  LPPe_EPDU_FAILURE_UNDEF = 0x0,
  LPPe_EPDU_COMPATIBILITY_FAILURE = 0x1,
  LPPe_EPDU_ID_INVALID = 0x2,
} LPPe_EPDUFailureCause;

/*=========================================================================== 
  tm_lpp_e_encode_pli
 
  Description: 
    LPP function used to encode LPPe provide location as an EPDU. Currently,
    a Ha3dpos and WLAN AP Measurements are encoded.
 
  Parameter:
   p_lpp_tx_type : Pointer to the right LPP transaction element from the transaction table.
 
  Return: 
    True if the encoding of provide location is successful, else return False.

===========================================================================*/
boolean tm_lpp_e_encode_pli
(
  tm_lpp_transaction_info_s_type *p_lpp_tx_type
);

/*=========================================================================== 
  tm_lpp_e_encode_provide_cap
 
  Description: 
    LPP function used to encode LPPe positioning capabilities of target 
    device (UE) as an EPDU.
 
  Parameter:
   p_lpp_tx_type : Pointer to the right LPP transaction element from the transaction table.
 
  Return: 
    True if the encoding of UE capabilities is successful, else return False.

===========================================================================*/
boolean tm_lpp_e_encode_provide_cap
(
  tm_lpp_transaction_info_s_type *p_lpp_tx_type
);

/*===========================================================================
  tm_lpp_e_decode_epdu_from_lpp_pdu

  Description:
    LPP function to look at an LPP PDU and extract an OMA LPPe EPDU from it if
    present.

  Parameters: 
      p_lpp_msg_body_decoded - UP or CP
      p_decode_status - Pointer to decode status
      cpz_lpp_err - Pointer to error structure

  Return value: decoded EPDU if successful, NULL otherwise

  Side Effects: None. No global state is changed. 
                Memory for the decoded EPDU is allocated
=============================================================================*/
tm_decode_OMA_LPPe_MessageExtension* tm_lpp_e_decode_epdu_from_lpp_pdu
(
  LPP_MessageBody_c1  *p_lpp_msg_body_decoded, 
  int                 *p_decode_status,
  tm_lpp_error_type * const cpz_lpp_err
);

/*===========================================================================
  tm_lpp_e_decode_epdu_from_lpp_pdu

  Description:
    LPP function to look at an EPDU sequence sent as part of an LPP message
    and extract an EPDU from it if present. Only EPDU-ID 1 is supported 
    (OMA LOC LPPe EPDU type). If a sequence of EPDUs is present, only the 
    first successfully decoded EPDU shall be returned, and the rest will be
    dropped silently.

  Parameters: 
      p_lpp_epdu_seq - pointer to LPP EPDU sequence structure
      p_decode_status - Pointer to decode status
      cpz_lpp_err - Pointer to error structure

  Return value: decoded EPDU successfully, NULL otherwise

  Side Effects: None. No global state is changed. 
                Memory for the decoded EPDU is allocated
=============================================================================*/
tm_decode_OMA_LPPe_MessageExtension* tm_lpp_e_decode_epdu_from_epdu_seq
(
  EPDU_Sequence *p_lpp_epdu_seq,
  int           *p_decode_status,
  tm_lpp_error_type * const cpz_lpp_err
);

/*==========================================================================
  tm_lpp_e_decode_message                                 
                                 
  Description:                     
  LPP function to process incoming LPPe message
  
  Returns                               
    TRUE if the decoding passed, FALSE otherwise    
===========================================================================*/
void*  tm_lpp_e_decode_message
( 
  const void* p_payload,  /**< encoded data */
  uint16      payload_size,
  int *decode_status_ptr
);

/*===========================================================================
  tm_lppe_fetch_best_position

  Description:
    LPPe function to fetch the best position from ALE

  Parameters:
      cpz_lpp_tx_item - pointer to LPP/LPPe transaction item

  Return value: True if request to fetch the best position is successful.

  Side Effects: None.

=============================================================================*/
GNSS_INLINE boolean tm_lppe_fetch_best_position( tm_lpp_transaction_info_s_type const *const cpz_lpp_tx_item,
                                                 tm_auxTechReqId const ce_ATReqId )
{
  if ( NULL == cpz_lpp_tx_item )
  {
    return ( FALSE );
  }

  /* Fetch DBH if requested */
  if ( (cpz_lpp_tx_item->z_lpp_data_type.z_rli.m_tech & C_LPP_TECH_HA3DPOS) != 0 )
  {
    /* CPI injection to ALE should have started when emergency call originated. Now request ALE to
       provide the best position through quxtech API */
    return ( tm_DBHPositionGet( ce_ATReqId ) );
  }

  return ( FALSE );
}

/*===========================================================================
  tm_lppe_trigger_wlan_ap_meas

  Description:
    LPPe function to trigger/start WLAN-AP measurement scan

  Parameters:

  Return value: None.

  Side Effects: None.

=============================================================================*/
GNSS_INLINE void tm_lppe_trigger_wlan_ap_meas(rli_tx_tech_mask const cm_RliTech,
                                              tm_auxTechReqId const ce_ATReqId,
                                              boolean const cv_EmergencyMode,
                                              pdsm_client_id_type const cq_PClientId)
{
  if ( (cm_RliTech & C_LPP_TECH_WLAN_AP_MEAS) != 0 )
  {
    tm_reqOnDemandWifiScan( cv_EmergencyMode, ce_ATReqId, cq_PClientId );
  }
}

/*===========================================================================
  tm_lppe_get_wlan_ap_meas

  Description:
    LPPe function to get the cached WLAN-AP measurements from AuxTech.

  Parameters:
      cpz_lpp_tx_item - pointer to LPP/LPPe transaction item

  Return value: None.

  Side Effects: None.

=============================================================================*/
GNSS_INLINE void tm_lppe_get_wlan_ap_meas( tm_lpp_transaction_info_s_type * const cpz_lpp_tx_item )
{
  if ( NULL == cpz_lpp_tx_item )
  {
    return;
  }

  /* Fetch WLAN-AP if requested by the server & copy the data to transaction item */
  if ( ( cpz_lpp_tx_item->z_lpp_data_type.z_rli.m_tech & C_LPP_TECH_WLAN_AP_MEAS) != 0 )
  {
    cpz_lpp_tx_item->z_lpp_data_type.p_wifi_ap_meas = tm_cacheWifiScanReportsGet();
  }
}

/*===========================================================================
  tm_lppe_get_srn_ap_meas

  Description:
    LPPe function to get the cached SRN measurements from AuxTech.
    Currently only BTLE is supported

  Parameters:
      cpz_lpp_tx_item - pointer to LPP/LPPe transaction item

  Return value: None.

  Side Effects: None.

=============================================================================*/
GNSS_INLINE void tm_lppe_get_srn_ap_meas( tm_lpp_transaction_info_s_type * const cpz_lpp_tx_item )
{
  if ( NULL == cpz_lpp_tx_item )
  {
    return;
  }

  /* Fetch SRN if requested by the server & copy the data to transaction item */
  if ( ( cpz_lpp_tx_item->z_lpp_data_type.z_rli.m_tech & C_LPP_TECH_SRN_MEAS) != 0 )
  {
    cpz_lpp_tx_item->z_lpp_data_type.pz_srn_ap_meas = tm_cacheSrnScanReportsGet();
  }
}

/*===========================================================================
  tm_lppe_get_sensor_baro_meas

  Description:
    LPPe function to get the cached Sensor Barometer pressure measurement from
    AUXTECH.

  Parameters:
      cpz_lpp_tx_item - pointer to LPP/LPPe transaction item

  Return value: None.

  Side Effects: None.

=============================================================================*/
GNSS_INLINE void tm_lppe_get_sensor_baro_meas( tm_lpp_transaction_info_s_type * const cpz_lpp_tx_item )
{
  if ( NULL == cpz_lpp_tx_item )
  {
    return;
  }

  /* Fetch SRN if requested by the server & copy the data to transaction item */
  if ( ( cpz_lpp_tx_item->z_lpp_data_type.z_rli.m_tech & C_LPP_TECH_BARO_MEAS) != 0 )
  {
    cpz_lpp_tx_item->z_lpp_data_type.pz_sensor_meas = tm_cacheSensorBaroMeasReportGet();
  }
}

/*==========================================================================
  tm_lppe_populate_tech_errors

  Description:
      LPPe function to populate LPPe errors in a transaction.

  Parameters:
      pz_lpp_tx_item - pointer to LPP/LPPe transaction item
      pq_location_notification_tech_mask - pointer to tech mask that 
         indicates location info tech that is send out to the server
      pq_location_notification_tech_mask_err - pointer to tech mask that
         indicates location info tech that failed to send out to the server

  Returns
===========================================================================*/
void tm_lppe_populate_tech_errors( tm_lpp_transaction_info_s_type * pz_lpp_tx_item, 
                                   uint32 *pq_location_notification_tech_mask, 
                                   uint32 *pq_location_notification_tech_mask_err);

/*===========================================================================
  tm_lppe_inform_lpp_req_cap

  Description:
    LPP module inform the LPPe Req Cap is received. The LPP payload may or may not
    have LPPe EPDU. This funcion process LPPe RC IE's 

  Parameters:
    cpz_req_cap - Pointer to LPP req cap struct.
    p_lpp_trans_item - pointer to LPP/LPPe transaction item.
    
  Return value: TRUE/FALSE.

  Side Effects: None.

=============================================================================*/
boolean tm_lppe_inform_lpp_req_cap( RequestCapabilities_r9_IEs const *const cpz_ReqCapR9,
                                    tm_lpp_transaction_info_s_type *const cpz_LppTransItem,
                                    boolean const cv_LppeEnabled );



/*===========================================================================
  tm_lppe_inform_lpp_req_loc_info

  Description:
    LPP module inform the LPP RLI is received. The LPP payload may or may not
    have LPPe EPDU. This funcion process LPPe RLI IE's & set the RLI technology mask
    if there are no duplicate request.

  Parameters:
    cpz_req_loc_r9 - Pointer to LPP RLI struct.
    p_lpp_trans_item - pointer to LPP/LPPe transaction item.
    cv_LppeEnabled - Indicate if LPPe feature is enabled
    cv_LppeEnabled - Indicate if LPPe feature is enabled.
    cq_LppeConfig  - Used to check if LPPe WLAN-AP feature is enabled.
    cv_NiSession - True if its NI session, otherwise False.

  Return value: None.

  Side Effects: None.

=============================================================================*/
boolean tm_lppe_inform_lpp_req_loc_info( RequestLocationInformation_r9_IEs const *const cpz_ReqLocInfoR9,
                                         tm_lpp_transaction_info_s_type *const cpz_LppTransItem,
                                         rli_tx_tech_mask    const cm_AcceptedRliTech,
                                         boolean const cv_LppeEnabled,
                                         boolean const cv_LppeDbhEnabled,
                                         pdsm_pa_lppe_config_type const cq_LppeConfig,
                                         boolean const cv_NiSession );

/*===========================================================================
  tm_lppe_get_lppe_cap

  Description:
    Get the LPPe capability based on the server request & UE configuration.
    When cv_LppeEnabled is TRUE, this API assumes the LPPe precondition is valid,
    LPPe bit is enabled & atleat one of the technology (DBH, WLAN-AP) is enabled.

  Parameters:
    cpcz_LppReqCap - Pointer to server's LPPe request capabilities
    cpz_LppProvCap - Return the UE supported LPPe capability
    cv_LppeEnabled - Indicate if LPPe feature is enabled
    cv_LppeEnabled - Indicate if LPPe feature is enabled
    cq_LppeConfig  - Indicate LPPe is configured to enabled DBH, WLAN-AP technologies.
    cv_NiSession - True if its NI session, otherwise False.

  Return value: None.

  Side Effects: None.

=============================================================================*/
void tm_lppe_get_lppe_cap( tm_lpp_req_cap_s_type const *const cpcz_LppReqCap,
                           tm_lpp_provide_cap_s_type  *const cpz_LppProvCap,
                           boolean const cv_LppeEnabled,
                           boolean const cv_LppeDbhEnabled,
                           pdsm_pa_lppe_config_type  const cq_LppeConfig,
                           boolean const cv_NiSession );


/*===========================================================================
  tm_lppe_check_supported_rlitech_req

  Description:
    Check if atleast one of the supported LPPe technology is requested by the server
    in the LPPe RLI.

  Parameters:
    cpcz_LppeMsg - Pointer to ASN.1 decoded LPPe message.
    cv_LppeEnabled - Indicate if LPPe feature is enabled.
    cq_LppeConfig  - Indicate LPPe is configured to enabled DBH, WLAN-AP technologies.

  Return value: True if any of the supported technology is requested, otherwise false.

  Side Effects: None.

=============================================================================*/
boolean tm_lppe_check_supported_rlitech_req( tm_decode_OMA_LPPe_MessageExtension const *const cpcz_LppeMsg,
                                             boolean const cv_LppeEnabled,
                                             pdsm_pa_lppe_config_type  const cq_LppeConfig );


/*===========================================================================
  tm_lppe_trigger_srn_ap_meas

  Description:
    LPPe function to trigger/start SRN-AP measurement. Short Range Node (SRN)
    currently supported is BlueTooth LE (Low Energy)

  Parameters:

  Return value: None.

  Side Effects: None.

=============================================================================*/
GNSS_INLINE void tm_lppe_trigger_srn_ap_meas(rli_tx_tech_mask const cm_RliTech,
                                             tm_auxTechReqId const ce_ATReqId,
                                             boolean const cv_EmergencyMode,
                                             pdsm_client_id_type const cq_PClientId,
                                             boolean const cv_Request)
{
  if ( (cm_RliTech & C_LPP_TECH_SRN_MEAS) != 0 )
  {
    tm_reqSrnScan( cv_EmergencyMode, ce_ATReqId, cq_PClientId, cv_Request );
  }
}

/*===========================================================================
  tm_lppe_trigger_sensor_meas

  Description:
    LPPe function to trigger/start Sensor measurements. Current implementation
    supports streaming Uncompromised Barometer Pressure measurements.

  Parameters:

  Return value: None.

  Side Effects: None.

=============================================================================*/
GNSS_INLINE void tm_lppe_trigger_sensor_meas(rli_tx_tech_mask const cm_RliTech,
                                             tm_auxTechReqId const ce_ATReqId,
                                             boolean const cv_Request)
{
  if ( (cm_RliTech & C_LPP_TECH_BARO_MEAS) != 0 )
  {
    tm_reqSensorBaroMeas( ce_ATReqId, cv_Request );
  }
}
#endif
#endif /* ifdef lppe */

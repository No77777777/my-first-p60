/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  TM-RRLP-Utils SUPL sub-module

GENERAL DESCRIPTION
  This file implements TM RRLP-UP SUPL sub-module.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2017 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/umts_up/supl/src/tm_rrlp_utils.h#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  02/10/17   SJ      Initial version

============================================================================*/

#ifndef TM_RRLP_UTILS_H
#define TM_RRLP_UTILS_H

#include "comdef.h"
#include "gps_variation.h"
#include "customer.h"   /* Customer configuration file */
#include "tm_rrlp_asn1_encode.h"
#include "tm_rrlp_asn1_decode.h"
#include "gnss_common.h"
#include "tm_data.h"


/*
 ******************************************************************************
 *
 * supl_glo_eph_build
 *
 * Function description:
 *  Buffers Glonass navigation model in a buffer that associates its SV ID with
 *  the corresponding frequency number
 *
 * Parameters:
 *   p_ganss_sat_element : pointer to the structure holding Glonass nav. model.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/

void supl_glo_eph_build
(
  rrlp_dec_GANSSSatelliteElement *p_ganss_sat_element,
  glo_EphStructType              *p_eph_buffer_per_sv
);

/*
 ******************************************************************************
 *
 * supl_glo_eph_svid_freq_num_map_aux
 *
 * Function description:
 *  Associates sv ID with a frequencey number for Glonass eph. data using
 *  information from auxilary assistance.
 *
 * Parameters:
 *   p_glo_aux : pointer to the structure holding Glonass aux. data.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/

void supl_glo_eph_svid_freq_num_map_aux 
(
  struct rrlp_dec_GANSS_ID3   *p_glo_aux,
  prot_glo_eph_buffer_s_type  *p_prot_glo_eph_buffer,
  uint8                       b_buf_sz
);

/*
 ******************************************************************************
 *
 * supl_glo_eph_svid_freq_num_map_alm
 *
 * Function description:
 *  Associates sv ID with a frequencey number for Glonass eph. data using
 *  information from auxilary assistance.
 *
 * Parameters:
 *   p_glo_alm_set : pointer to the structure holding Glonass alm. model.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/

void supl_glo_eph_svid_freq_num_map_alm
(
   rrlp_dec_Almanac_GlonassAlmanacSet *p_glo_alm_set,
   prot_glo_eph_buffer_s_type         *p_prot_glo_eph_buffer,
   uint8 b_buff_sz
);

/*
 ******************************************************************************
 *
 * supl_glo_acqasst_svid_freq_num_map_aux
 *
 * Function description:
 *  Associates sv ID with a frequencey number for Glonass reference measurement
 *  data using information from auxilary assistance.
 *
 * Parameters:
 *   p_glo_aux : pointer to the structure containing GLO aux. assistance;
 *   p_glo_acqasst: point to the structure that saves GLONASS acq. assist.
 *   measurement.
 *
 * Return value:
 *   None.
 *
 ******************************************************************************
*/
void supl_glo_acqasst_svid_freq_num_map_aux 
(
   struct rrlp_dec_GANSS_ID3 *p_glo_aux,
   prot_glo_acqast_buffer_s_type *p_glo_acqasst
);

/*
 ******************************************************************************
 *
 * tm_rrlp_bds_eph_post
 *
 * Function description:
 *  Translate RRLP BDS Ephemeris to a gnss_common.h format used by rest of 
 *  the engine and post it to TM Core using the session handle provided.
 *
 * Parameters:
 *   prtl_type: Protocol Type to use to post the data to TM Core.
 *   sess_handle: Session handle to use to post the data to TM Core.
 *   p_ganss_nav_model : input pointer to the structure holding RRLP Ephemeris
 *   p_tm_post_data_payload : output pointer to use to send the translated
 *                       ephemeris to TM Core.
 *
 * Side Effects:
 *   None: Posts BDS Ephmeris to TM Core
 *
 * Return value:
 *   None
 *
 ******************************************************************************
*/
void tm_rrlp_bds_eph_post
(
  tm_prtl_type                prtl_type,
  tm_sess_handle_type         sess_handle,
  rrlp_dec_GANSSNavModel     *p_ganss_nav_model,
  tm_post_data_payload_type  *p_tm_post_data_payload,
  uint16                      w_bds_week
);

/*
 ******************************************************************************
 *
 * tm_rrlp_bds_alm_post
 *
 * Function description:
 *  Translate RRLP BDS Almanac to a gnss_common.h format used by rest of 
 *  the engine and post it to TM Core using the session handle provided.
 *
 * Parameters:
 *   prtl_type: Protocol Type to use to post the data to TM Core.
 *   sess_handle: Session handle to use to post the data to TM Core.
 *   p_ganss_alm_model : input pointer to the structure holding RRLP BDS Almanac
 *   p_tm_post_data_payload : output pointer to use to send the translated BDS
 *                       Almanac to TM Core.
 *
 * Side Effects:
 *   None: Posts BDS Almanac to TM Core
 *
 * Return value:
 *   None
 *
 ******************************************************************************
*/
void tm_rrlp_bds_alm_post
(
  tm_prtl_type                prtl_type,
  tm_sess_handle_type         sess_handle,
  rrlp_dec_GANSSAlmanacModel *p_ganss_alm_model,
  tm_post_data_payload_type  *p_tm_post_data_payload
);

/*
 ******************************************************************************
 *
 * tm_rrlp_bds_rti_post
 *
 * Function description:
 *  Translate RRLP BDS RTI to a gnss_common.h format used by rest of 
 *  the engine and post it to TM Core using the session handle provided.
 *
 * Parameters:
 *   prtl_type: Protocol Type to use to post the data to TM Core.
 *   sess_handle: Session handle to use to post the data to TM Core.
 *   p_ganss_rti_model : input pointer to the structure holding RRLP BDS RTI
 *   p_tm_post_data_payload : output pointer to use to send the translated BDS
 *                       RTI to TM Core.
 *
 * Side Effects:
 *   None: Posts BDS RTI to TM Core
 *
 * Return value:
 *   None
 *
 ******************************************************************************
*/
void tm_rrlp_bds_rti_post
(
  tm_prtl_type                     prtl_type,
  tm_sess_handle_type              sess_handle,
  rrlp_dec_GANSSRealTimeIntegrity *p_ganss_rti_model,
  tm_post_data_payload_type       *p_tm_post_data_payload
);


/*
 ******************************************************************************
 *
 * tm_rrlp_bds_time_model_post
 *
 * Function description:
 *  Translate RRLP BDS Time Model to a gnss_common.h format used by rest of 
 *  the engine and post it to TM Core using the session handle provided.
 *
 * Parameters:
 *   prtl_type: Protocol Type to use to post the data to TM Core.
 *   sess_handle: Session handle to use to post the data to TM Core.
 *   p_ganss_time_model : input pointer to the structure holding RRLP BDS Time 
 *                        Model
 *   p_tm_post_data_payload : output pointer to use to send the translated BDS
 *                        Time Model to TM Core.
 *
 * Side Effects:
 *   None: Posts BDS Time Model to TM Core
 *
 * Return value:
 *   None
 *
 ******************************************************************************
*/
void tm_rrlp_bds_time_model_post
(
  tm_prtl_type                   prtl_type,
  tm_sess_handle_type            sess_handle,
  rrlp_dec_SeqOfGANSSTimeModel  *p_ganss_time_model,
  tm_post_data_payload_type     *p_tm_post_data_payload,
  uint16                        *p_w_bds_week,
  rrlp_dec_GANSSReferenceTime   *p_ganss_ref_time
);

/*
 ******************************************************************************
 *
 * tm_rrlp_bds_global_iono_post
 *
 * Function description:
 *  Translate RRLP BDS Global Iono Model to a gnss_common.h format used by rest 
 *  of the engine and post it to TM Core using the session handle provided.
 *
 * Parameters:
 *   prtl_type: Protocol Type to use to post the data to TM Core.
 *   sess_handle: Session handle to use to post the data to TM Core.
 *   p_ganss_common_data : Input pointer to the structure holding RRLP BDS Global 
 *                        Iono Model
 *   p_tm_post_data_payload : Output pointer to use to send the translated BDS
 *                        Global Iono Model to TM Core.
 *
 * Side Effects:
 *   None: Posts BDS Global Iono Model to TM Core
 *
 * Return value:
 *   None
 *
 ******************************************************************************
*/
void tm_rrlp_bds_global_iono_post
( 
  tm_prtl_type                     prtl_type,
  tm_sess_handle_type              sess_handle, 
  rrlp_dec_GANSSCommonAssistData  *p_ganss_common_data, 
  tm_post_data_payload_type       *p_tm_post_data_payload
);

#endif

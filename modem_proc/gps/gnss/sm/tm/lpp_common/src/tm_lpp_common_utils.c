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

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/lpp_common/src/tm_lpp_common_utils.c#1 $
 
  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  02/17/17   skm     BLE and Barometer support for LPPE Phase 2
  11/16/16   gk      Velocity flags not correctly stored
  09/15/16   rk      Avoid reseting the GNSS method in Tx item. In duplicate tx scenario,
                     the first GNSS method request shouldn't be overwritten.
  08/05/15   rh      Moved carrier-specific NV 1920 (pos mode support) to TM core 
  01/14/15   rk      LPP >24 AD/Meas feature support.
  12/14/14   gk      new session events to PDAPI
  08/25/14   jv      16-bit SVID Support 
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
#include "tm_lpp_common_utils.h"
#ifdef FEATURE_GNSS_LPPE
#include "tm_lpp_e.h"
#endif

lpp_cp_nv_items_s_type z_lpp_cp_nv_item_ram_copy;

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
void tm_lpp_common_utils_read_nv (void)
{
  /* NV buffer, used to read NV items */
  nv_item_type      z_nv_read_buffer;
  
  /* Default is NV_AAGPS_POSITIONING_MODES_SUPPORTED_V
     PDSM_POS_MODE_4G_UP_MS_BASED    0x0100
     PDSM_POS_MODE_4G_UP_MS_ASSISTED 0x0200
     PDSM_POS_MODE_4G_CP_MS_BASED    0x0400
     PDSM_POS_MODE_4G_CP_MS_ASSISTED 0x0800
    
     NOTE: carrier-specific NV 1920 moved to TM core */ 
  
  /* NOTE - LPP CP enable/disable NV item is maintained in TM CORE via tm_core_get_lpp_cp_enable() */

  /*
  Bit 0 - CS-MT-LR             (for CP GSM/WCDMA)
  Bit 1 - Ignore UP NI trigger (for UP GSM/WCDMA)
  Bit 2 - UP NI trigger (for UP LPP)
  Bit 3 - CP NI trigger (for CP LPP EPC-MT-LR)

  NV_AAGPS_MT_LR_CP_GSM_WCDMA_SUPPORTED             0x01 
  NV_AAGPS_MT_LR_UP_GSM_WCDMA_BLOCKED               0x02
  NV_AAGPS_MT_LR_UP_LPP_SUPPORTED                   0x04
  NV_AAGPS_MT_LR_CP_LPP_SUPPORTED                   0x08
  */
  (void)sm_nv_read( NV_AAGPS_MT_LRSUPPORT_I, (void *)&z_nv_read_buffer);
  z_lpp_cp_nv_item_ram_copy.mt_lr_support = z_nv_read_buffer.aagps_mt_lrsupport;
 

  (void) sm_nv_read( NV_AAGPS_DEFAULT_QOS_TIME_I, (void *)&z_nv_read_buffer);
  z_lpp_cp_nv_item_ram_copy.aagps_default_qos_time = z_nv_read_buffer.aagps_default_qos_time;

  (void) sm_nv_read( NV_AAGPS_DEFAULT_QOS_UNC_I, (void *)&z_nv_read_buffer);
  z_lpp_cp_nv_item_ram_copy.aagps_default_qos_unc = z_nv_read_buffer.aagps_default_qos_unc;   /* ayt_todo - not in nv_lpp.xml */

  (void) sm_nv_read( NV_AAGPS_DEFAULT_REF_TIME_UNC_I, (void *)&z_nv_read_buffer);
  z_lpp_cp_nv_item_ram_copy.aagps_default_ref_time_unc_50ms = z_nv_read_buffer.aagps_default_ref_time_unc; /* ayt_todo - not in nv_lpp.xml */
  
  (void) sm_nv_efs_reg_item_read( GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS,
      sizeof(uint8), &z_lpp_cp_nv_item_ram_copy.aagps_default_lpp_cp_timeout_secs);
  {
    /* if returned false for any reason, force it to default value */
    z_lpp_cp_nv_item_ram_copy.aagps_default_lpp_cp_timeout_secs = GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS_DEFAULT;
  }
  
} /* tm_lpp_common_utils_read_nv */


/*
 ******************************************************************************
 *
 * tm_lpp_common_utils_refresh_carrier_specific_nv
 *
 * Function description:
 *    This function refresh carrier-specific NVs after SIM hot swap
 *    sm_nv_read() will always read from HW. sm_nv_read_ex() may read from cache
 *    if available.
 *
 * Parameters:
 *   None.
 *
 * Return value: 
 *   None.
 *
 ******************************************************************************
*/
void tm_lpp_common_utils_refresh_carrier_specific_nv (void)
{
#ifdef FEATURE_MODEM_CONFIG_REFRESH
  /* Add any other carrier-specific NVs here, NV 1920 (pos mode support) moved to TM core */
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
}


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
 *   p_gnss_aux_info : pointer to the structure holding Glonass aux. data.
 *
 * Return value: 
 *   None.
 *
 ******************************************************************************
*/

void tm_lpp_common_glo_eph_svid_freq_num_map_aux (const GNSS_AuxiliaryInformation * const p_gnss_aux_info, prot_glo_eph_buffer_s_type p_lpp_glo_eph_freq[N_GLO_SV])
{
  uint8  u_index = 0;
  GNSS_ID_GLONASS_SatElement *p_lpp_glonass_sat_elem = NULL;

  if (( p_gnss_aux_info->t != T_GNSS_AuxiliaryInformation_gnss_ID_GLONASS ) || ( p_lpp_glo_eph_freq == NULL) ||
      (p_gnss_aux_info->u.gnss_ID_GLONASS == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "AuxiInfo doesn't contain Glonass parameters. Map svid & freq/channel num failed!", 0, 0, 0 );
    return;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"LPP Glonass AuxiInfo list = %u", p_gnss_aux_info->u.gnss_ID_GLONASS->n, 0, 0);

  for ( u_index = 0; u_index < p_gnss_aux_info->u.gnss_ID_GLONASS->n; ++u_index )
  {
    p_lpp_glonass_sat_elem = &p_gnss_aux_info->u.gnss_ID_GLONASS->elem[u_index];

    if ( p_lpp_glonass_sat_elem && p_lpp_glonass_sat_elem->m.channelNumberPresent )
    {
      if ( ( p_lpp_glonass_sat_elem->svID.satellite_id <= ( N_GLO_SV - 1 ) ) &&
           ( ( p_lpp_glonass_sat_elem->channelNumber >= C_PROT_GLO_FREQ_MIN ) &&
             ( p_lpp_glonass_sat_elem->channelNumber <= C_PROT_GLO_FREQ_MAX ) 
           )
         )
      {
        p_lpp_glo_eph_freq[p_lpp_glonass_sat_elem->svID.satellite_id].u_freq_num_valid = TRUE;
        p_lpp_glo_eph_freq[p_lpp_glonass_sat_elem->svID.satellite_id].z_eph_glo_data.u_FreqNum = 
          p_lpp_glonass_sat_elem->channelNumber & 0x1F; /* the MGP API expects 5 bits */

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW,"Mapped LPP Glonass SV_id=%u and channel#=%d. u_i=%u",
                 p_lpp_glonass_sat_elem->svID.satellite_id, p_lpp_glonass_sat_elem->channelNumber, u_index);
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Incorrect LPP Glonass SV_id=%u and channel#=%d, u_i=%u",
                 p_lpp_glonass_sat_elem->svID.satellite_id, p_lpp_glonass_sat_elem->channelNumber, u_index);
      }
    }
  }
} /* tm_lpp_common_glo_eph_svid_freq_num_map_aux */


/*
 ******************************************************************************
 *
 * tm_lpp_common_is_freq_sat_valid 
 *
 * Function description:
 *  Verify if the frequency number & satellite SV id is valid. Generally these
 *  information are are available in auxiliary info assistance data.                   
 *  
 * Parameters:
 *   p_gnss_glonass_sat_elem : pointer to the structure holding Glonass aux. data.
 * 
 * Return value: 
 *   None.
 *
 ******************************************************************************
*/
boolean tm_lpp_common_is_freq_sat_valid( const GNSS_ID_GLONASS_SatElement * const p_gnss_glonass_sat_elem )
{   
  if ( (p_gnss_glonass_sat_elem != NULL ) && (p_gnss_glonass_sat_elem->m.channelNumberPresent) )
  {
    // Check freqNum & svID.satellite_id (0..63) correctness. For glonass satellite_id 0..23 is valid
    if ( ( p_gnss_glonass_sat_elem->svID.satellite_id <= ( N_GLO_SV - 1 ) ) &&
         ( p_gnss_glonass_sat_elem->channelNumber >= C_PROT_GLO_FREQ_MIN ) && ( p_gnss_glonass_sat_elem->channelNumber <= GLO_HIGHEST_FREQ_NUMBER ) // ME expect FreqNum -7..6, though protocol max is 13.
       )
    {
      return TRUE;
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Incorrect LPP Glonass SV_id=%u and channel#=%d", p_gnss_glonass_sat_elem->svID.satellite_id,
                p_gnss_glonass_sat_elem->channelNumber, 0);
    }
  }

  return FALSE;
}

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
 *   p_gnss_aux_info : pointer to the structure holding Glonass aux. data.
 *   p_lpp_glo_aa_freq : pointer to LPP Glonass aquisition assistance & auxi info struct.
 * 
 * Return value: 
 *   None.
 *
 ******************************************************************************
*/
void tm_lpp_common_glo_acqast_svid_freq_num_map_aux (const GNSS_AuxiliaryInformation* const p_gnss_aux_info, 
                                                     prot_glo_acqast_buffer_s_type* p_lpp_glo_aa_freq)
{
  uint8 u_index = 0, u_sv_iter = 0, u_save_loop = 0;
  GNSS_ID_GLONASS_SatElement *p_gnss_glonass_sat_elem = NULL;
  uint8 u_glo_num_svs = 0;

  if ( ( p_lpp_glo_aa_freq == NULL ) || (p_gnss_aux_info->t != T_GNSS_AuxiliaryInformation_gnss_ID_GLONASS ) || 
       ( p_gnss_aux_info->u.gnss_ID_GLONASS == NULL ) )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "AuxiInfo doesn't contain Glonass parameters. Map svid & freq/channel num failed!", 0, 0, 0 );
    return;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"LPP Glonass AuxiInfo num of list = %u", p_gnss_aux_info->u.gnss_ID_GLONASS->n, 0, 0);

  /* Glonass Acq Assistance isn't received ? */
  if ( 0 == p_lpp_glo_aa_freq->gnss_sv_dir.u_NumSvs )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Glonass Acq Assistance isn't received yet. Glo SvId to FreqNum correlation deferred.", 0, 0, 0 );
    return;
  }
  else 
  {
    /* Glonass Acq Assistance is received already */

    /* Ensure the total SV count for SV-Dir & Steering are same. This help avoid looping twice. */
    u_glo_num_svs = p_lpp_glo_aa_freq->gnss_sv_dir.u_NumSvs;
    if( p_lpp_glo_aa_freq->gnss_sv_dir.u_NumSvs != p_lpp_glo_aa_freq->z_steering_glo_data.u_NumSvs )
    {
      if (p_lpp_glo_aa_freq->gnss_sv_dir.u_NumSvs > p_lpp_glo_aa_freq->z_steering_glo_data.u_NumSvs)
      {
        u_glo_num_svs = p_lpp_glo_aa_freq->z_steering_glo_data.u_NumSvs;
      }

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SV-Dir & Steering SV count differs %u, %u. Using only %u AA SVs", 
            p_lpp_glo_aa_freq->gnss_sv_dir.u_NumSvs, p_lpp_glo_aa_freq->z_steering_glo_data.u_NumSvs, u_glo_num_svs);
    }

    for ( u_index = 0; u_index < p_gnss_aux_info->u.gnss_ID_GLONASS->n; ++u_index )
    {
      p_gnss_glonass_sat_elem = &p_gnss_aux_info->u.gnss_ID_GLONASS->elem[u_index];

      if ( TRUE == tm_lpp_common_is_freq_sat_valid( p_gnss_glonass_sat_elem ) )
      {
        u_save_loop = 0;
        /* Loop through AA SVs & set the corresponding FreqNum */
        for ( u_sv_iter = 0; u_sv_iter < u_glo_num_svs; ++u_sv_iter )
        {
          // AA-SvDir
          // local u_Sv (65..88) & protocol p_gnss_glonass_sat_elem->svID (0..23)
          if (p_lpp_glo_aa_freq->gnss_sv_dir.z_SvAzElInfo[u_sv_iter].w_Sv == (p_gnss_glonass_sat_elem->svID.satellite_id + GLO_SLOTID_OFFSET + 1))
          {
            p_lpp_glo_aa_freq->u_freq_num_valid = TRUE;
            /* TomL: Per GLONASS ICD page 10, after year 2005, GLO SV would use frequency ID -7 to +6. Freq channel +7 to +13 would not be used. */
            p_lpp_glo_aa_freq->gnss_sv_dir.z_SvAzElInfo[u_sv_iter].b_GloFreqIndex = p_gnss_glonass_sat_elem->channelNumber; /* the MGP API expects 5 bits */
            u_save_loop |= 0x1;
          }

          // AA-Steering
          // local u_Sv (65..88) & protocol p_gnss_glonass_sat_elem->svID (0..23)
          if (p_lpp_glo_aa_freq->z_steering_glo_data.z_SvSteerInfo[u_sv_iter].w_Sv == (p_gnss_glonass_sat_elem->svID.satellite_id + GLO_SLOTID_OFFSET + 1))
          {
            p_lpp_glo_aa_freq->u_freq_num_valid = TRUE;
            p_lpp_glo_aa_freq->z_steering_glo_data.z_SvSteerInfo[u_sv_iter].b_GloFreqIndex = p_gnss_glonass_sat_elem->channelNumber;
            u_save_loop |= 0x2;
          }

          if ( u_save_loop == 3)
          {
            break; // break when both freqNum are set, else loop to max.
          }
        }

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Mapped LPP Glonass SV_id=%u and channel#=%d. u_i=%u", p_gnss_glonass_sat_elem->svID.satellite_id,
                     p_gnss_glonass_sat_elem->channelNumber, u_index);
      }
    }
  }
}

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

void tm_lpp_common_glo_eph_svid_freq_num_map_alm (const AlmanacGLONASS_AlmanacSet * const p_keplerianGLONASS, prot_glo_eph_buffer_s_type p_lpp_glo_eph_freq[N_GLO_SV])
{
  if ((p_keplerianGLONASS == NULL) || (p_keplerianGLONASS->gloAlmnA < 1) || (p_keplerianGLONASS->gloAlmnA > N_GLO_SV)) // gloAlmnA (1..24) - sv slot id
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"GLO Alm SvId invalid: %p", p_keplerianGLONASS, 0, 0 );
    return;
  }

  /* gloAlmnA is SV_ID :  gloAlmnA : INTEGER (1..24) */
  p_lpp_glo_eph_freq[p_keplerianGLONASS->gloAlmnA - 1].u_freq_num_valid = TRUE;
  // gloAlmHA (0..31)
  p_lpp_glo_eph_freq[p_keplerianGLONASS->gloAlmnA - 1].z_eph_glo_data.u_FreqNum =  p_keplerianGLONASS->gloAlmHA;
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW,"GLO Alm. SV slot id %u to FreqNum %d mapping", p_keplerianGLONASS->gloAlmnA, p_keplerianGLONASS->gloAlmHA, 0);
}

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
boolean tm_lpp_common_convert_to_glonass_time(const int8 bDeltaT, gnss_time_info *p_supl_session_postime)
{
  // the delta time (a.k.a LeapSecondOffset) for GPS to GLONASS should be positive value
  if ( ( p_supl_session_postime == NULL ) || ( p_supl_session_postime->b_deltaT < 0 ) )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_UP: SUPL: GPS time difference deltaT should be positive. %p",
               p_supl_session_postime, 0, 0 );
    return FALSE;
  }

  p_supl_session_postime->b_deltaT = bDeltaT;
  p_supl_session_postime->m.deltaTPresent = 1;

  // Convert GPS to GLONASS time
  if ( (TRUE == p_supl_session_postime->m.refTimeWeekNumValid) && (1 == p_supl_session_postime->m.refTimeTowMsValid) ) 
  {
    gps_TimeStructType z_GpsTime;
    memset(&z_GpsTime, 0, sizeof(z_GpsTime));
    memset(&p_supl_session_postime->z_GlonassTime, 0, sizeof(glo_TimeStructType));

    z_GpsTime.w_GpsWeek = p_supl_session_postime->w_ref_time_week_num;
    z_GpsTime.q_GpsMsec = p_supl_session_postime->q_ref_time_tow_ms;

    gnss_ConvertGpsTime2GloTime(&z_GpsTime, (uint8)p_supl_session_postime->b_deltaT,
                                &p_supl_session_postime->z_GlonassTime );

    p_supl_session_postime->m.glonassTimeValid = TRUE;
    MSG_6( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_LPP_UP: SUPL: GLONASS time (%u, %u, %ld) computed from GPS time (%u, %lu) offset of %d", 
           p_supl_session_postime->z_GlonassTime.u_FourYear, p_supl_session_postime->z_GlonassTime.w_Days, 
           p_supl_session_postime->z_GlonassTime.q_GloMsec, z_GpsTime.w_GpsWeek, z_GpsTime.q_GpsMsec,
           p_supl_session_postime->b_deltaT );

    return TRUE;
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_LPP_UP: SUPL: GPS Reference Time (%d, %d) isn't valid to compute Glonass time.",
               p_supl_session_postime->m.refTimeWeekNumValid, p_supl_session_postime->m.posTimeStampValid, 0 );
  }

  return FALSE;
}

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
boolean tm_lpp_common_lpp_rli_version( RequestLocationInformation_criticalExtensions const * const cpcz_RLICritExt )
{
  if ( FALSE == cpcz_RLICritExt )
  {
    return ( FALSE );
  }

  /* Only handle the supported version of REQ_LOC_INFO. For everything else,
 * send an error to the SMLC */
  if( (cpcz_RLICritExt->t != LPP_REQ_LOC_INFO_CRIT_EXT) ||
      (cpcz_RLICritExt->u.c1->t != LPP_REQ_CAP_CRIT_EXT_R9))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Unsupported critical extentsion=%d ext type%d",
              cpcz_RLICritExt->t,
              cpcz_RLICritExt->u.c1->t, 0);

    return ( FALSE );
  }

  return ( TRUE );
}


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
                                          tm_lpp_req_type *const cpq_TxLppFlags )
{
  A_GNSS_RequestLocationInformation *p_a_gnss_info;
  tm_lpp_error_type *cp_LppErr = NULL;

  if ( (NULL == p_req_loc_r9) || (NULL == p_LppTxItem) || (NULL == cpm_RliTech) ||
       (NULL == cpm_RliGnssMethod) || (NULL == cpq_SessionLppFlags) || (NULL == cpq_TxLppFlags) )
  {
    return ( FALSE );
  }

  /* AGNSS unavailable to process isn't failure case. So return TRUE. */
  if ( 0 == p_req_loc_r9->m.a_gnss_RequestLocationInformationPresent )
  {
    return ( TRUE );
  }

  cp_LppErr = &p_LppTxItem->z_lpp_data_type.z_lpp_err;

  /* commonIERequestLocationInformation is needed for processing of request-
   * LocationInformation. Reject request if this is requested */
  if ( 0 == p_req_loc_r9->m.commonIEsRequestLocationInformationPresent )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "CommonIE not present. Abort session" );
    cp_LppErr->w_ErrType |= LPP_ERROR_TYPE_LPP_ABORT;
    cp_LppErr->u_AbortErrCode  = LPP_ABORT_TARGET;
    return ( FALSE );
  }

  p_a_gnss_info = &p_req_loc_r9->a_gnss_RequestLocationInformation;

  /* Store location server's choice(s) of gnss type to report in Prov location Info message */
  if ( OSTESTBIT( p_a_gnss_info->gnss_PositioningInstructions.gnss_Methods.gnss_ids, LPP_GNSS_ID_GPS ) )
  {
    *cpm_RliGnssMethod |= C_POS_SOURCE_GPS;
  }
  if ( OSTESTBIT( p_a_gnss_info->gnss_PositioningInstructions.gnss_Methods.gnss_ids, LPP_GNSS_ID_GLONASS ) )
  { 
    /* Glonass isn't enabled, but LS request for Glo meas or Glonass is indicated by the LS but
       GPS is not. Send requestMethodNotSupported error in ProvLocInfo */
    if ( FALSE == OSTESTBIT( p_a_gnss_info->gnss_PositioningInstructions.gnss_Methods.gnss_ids, LPP_GNSS_ID_GPS ) )
    {
      cp_LppErr->w_ErrType |= LPP_ERROR_TYPE_PROV_LOC_COMMON_ERROR;
      cp_LppErr->u_CommonErrCode = LPP_FAILURE_REQMETHODNOT_SUPPORTED;
    }
    else
    {
      *cpm_RliGnssMethod |= C_POS_SOURCE_GLO;
    }
  }

  /* Check the request from server is tained before accepting the request. In concurrency sessions
  *  the duplicate request are not allowed. Respond with LPP Abort for any duplicate request.
  *  Otherwise accept the request as valid.
  */
  if ( (p_req_loc_r9->commonIEsRequestLocationInformation.locationInformationType
        == LocationInformationType_locationEstimateRequired) ||
       (p_req_loc_r9->commonIEsRequestLocationInformation.locationInformationType
        == LocationInformationType_locationEstimatePreferred ) )
  {
    /* If AGNSS MSB request is pending then send LPP Abort. Later decide to send LPP Abort or
    *  ignored based on whether its new transaction id.
    */
    if ( (cm_AcceptedRliTech & C_LPP_TECH_GNSS_MSB) != 0 )
    {
      MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "AGNSS MSB request pending 0x%X", cm_AcceptedRliTech );
      cp_LppErr->w_ErrType |= LPP_ERROR_TYPE_LPP_ABORT;
      cp_LppErr->u_AbortErrCode  = LPP_ABORT_TARGET;
      return ( FALSE );
    }
    /* The server need to provide atleat A-GPS method */
    if ( (*cpm_RliGnssMethod & C_POS_SOURCE_GPS) != 0 )
    {
      *cpm_RliTech |= C_LPP_TECH_GNSS_MSB;
      *cpq_SessionLppFlags |= TM_LPP_REQ_LOC_INFO_POS_EST_RCVD;
    }
  }
  else
  {
    /* If AGNSS MSA request is pending then send LPP Abort. Later decide to send LPP Abort or
    *  ignored based on whether its new transaction id.
    */
    if ( (cm_AcceptedRliTech & C_LPP_TECH_GNSS_MSA) != 0 )
    {
      MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "AGNSS MSA request pending 0x%X", cm_AcceptedRliTech);
      cp_LppErr->w_ErrType |= LPP_ERROR_TYPE_LPP_ABORT;
      cp_LppErr->u_AbortErrCode  = LPP_ABORT_TARGET;
      return ( FALSE );
    }
    /* The server need to provide atleat A-GPS method */
    if ( (*cpm_RliGnssMethod & C_POS_SOURCE_GPS) != 0 )
    {
      *cpm_RliTech |= C_LPP_TECH_GNSS_MSA;
      *cpq_SessionLppFlags |= TM_LPP_REQ_LOC_INFO_GNSS_MEAS_RCVD;
    }
  }

  /* If server didn't provide GNSS method send 'Undefined' back*/
  if ( 0 == *cpm_RliGnssMethod )
  {
    cp_LppErr->w_ErrType |= LPP_ERROR_TYPE_PROV_LOC_COMMON_ERROR;
    cp_LppErr->u_CommonErrCode = LPP_FAILURE_UNDEF;
  }
  MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LS ReqLocInfo for 0x%lX system.",
         p_a_gnss_info->gnss_PositioningInstructions.gnss_Methods.gnss_ids.data[0] );

  if ( p_a_gnss_info->gnss_PositioningInstructions.assistanceAvailability )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "UE can request additonal AD", 0, 0, 0 );
    *cpq_TxLppFlags |= TM_LPP_MAY_REQ_ADD_GNSS_AD;
  }
  if ( p_a_gnss_info->gnss_PositioningInstructions.fineTimeAssistanceMeasReq )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "UE to report FTA AD. Not supported", 0, 0, 0 );
  }

  return ( TRUE );
}


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
boolean tm_lpp_common_lpp_otdoa_rli_proc( RequestLocationInformation_r9_IEs const*const p_req_loc_r9,
                                          tm_lpp_error_type*const cp_LppErr,
                                          rli_tx_tech_mask const cm_AcceptedRliTech,
                                          rli_tx_tech_mask*const cpm_RliTech,
                                          tm_lpp_req_type*const cpq_SessionLppFlags,
                                          tm_lpp_req_type *const cpq_TxLppFlags)
{

  if ( (NULL == p_req_loc_r9) || (NULL == cp_LppErr) || (NULL == cpm_RliTech) || (NULL == cpq_SessionLppFlags) ||
       (NULL == cpq_TxLppFlags) )
  {
    return ( FALSE );
  }

  /* OTDOA unavailable to process isn't failure case. So return TRUE. */
  if ( 0 == p_req_loc_r9->m.otdoa_RequestLocationInformationPresent )
  {
    return ( TRUE );
  }

  /* commonIERequestLocationInformation is needed for processing of request-
   * LocationInformation. Reject request if this is requested */
  if ( 0 == p_req_loc_r9->m.commonIEsRequestLocationInformationPresent )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "CommonIE not present. Abort session" );
    cp_LppErr->w_ErrType |= LPP_ERROR_TYPE_LPP_ABORT;
    cp_LppErr->u_AbortErrCode  = LPP_ABORT_TARGET;
    return ( FALSE );
  }

  /* Check the request from server is tained before accepting the request.
  *  In concurrency sessions the duplicate or overlapping request are not allowed. LPP Abort is send
  *  for new transaction. Otherwise this transaction is ignored.
  */
  if ( (p_req_loc_r9->commonIEsRequestLocationInformation.locationInformationType
        == LocationInformationType_locationMeasurementsRequired) ||
       (p_req_loc_r9->commonIEsRequestLocationInformation.locationInformationType
        == LocationInformationType_locationMeasurementsPreferred ) )
  {
    if ( (cm_AcceptedRliTech & C_LPP_TECH_OTDOA_MSA) != 0 )
    {
      MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "OTDOA MSA request pending 0x%X", *cpm_RliTech );
      cp_LppErr->w_ErrType |= LPP_ERROR_TYPE_LPP_ABORT;
      cp_LppErr->u_AbortErrCode  = LPP_ABORT_TARGET;
      return ( FALSE );
    }
  }

  if ( p_req_loc_r9->otdoa_RequestLocationInformation.assistanceAvailability == TRUE )
  {
    *cpq_TxLppFlags |= TM_LPP_MAY_REQ_ADD_OTDOA_AD;
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_MED, "UE can request additonal OTDOA AD" );
  }

  *cpm_RliTech |= C_LPP_TECH_OTDOA_MSA;
  *cpq_SessionLppFlags |= TM_LPP_REQ_LOC_INFO_OTDOA_RCVD;
  return ( TRUE );
}

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
void tm_lpp_common_lpp_commonie_rli_proc( RequestLocationInformation_r9_IEs const * const p_req_loc_r9,
                                          tm_lpp_req_type * const cpq_LppFlags,
                                          tm_lpp_req_type * const cpq_TxLppFlags )
{
  CommonIEsRequestLocationInformation const * p_common_ie_req = NULL;

  if (( NULL == p_req_loc_r9 ) || (NULL == cpq_LppFlags) || (NULL == cpq_TxLppFlags))
  {
    return;
  }
  p_common_ie_req = &p_req_loc_r9->commonIEsRequestLocationInformation;

    /* Check if UE may return addition information (beyond what is requested by
   * the SMLC) */
  if((p_common_ie_req->m.additionalInformationPresent == 1) &&
     (p_common_ie_req->additionalInformation == 1))
  {
      *cpq_LppFlags |= TM_LPP_MAY_RET_ADD_INFO;
      *cpq_TxLppFlags |= TM_LPP_MAY_RET_ADD_INFO;
  }
}

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
boolean tm_lpp_common_lpp_ecid_rli_proc( RequestLocationInformation_r9_IEs const*const p_req_loc_r9,
                                         tm_lpp_error_type*const cp_LppErr,
                                         rli_tx_tech_mask const cm_AcceptedRliTech,
                                         rli_tx_tech_mask*const cpm_RliTech,
                                         tm_lpp_req_type*const cpq_LppFlags,
                                         uint32* p_lpp_ecid_req_loc)
{
   OSOCTET requested_ecid_data = 0;

  if ( (NULL == p_req_loc_r9) || (NULL == cp_LppErr) || (NULL == cpm_RliTech) ||
       (NULL == cpq_LppFlags) || (NULL == p_lpp_ecid_req_loc) )
  {
    return ( FALSE );
  }

  /* ECID unavailable to process isn't failure case. So return TRUE. */
  if ( 0 == p_req_loc_r9->m.ecid_RequestLocationInformationPresent )
  {
    return ( TRUE );
  }

  /* commonIERequestLocationInformation is needed for processing of request-
   * LocationInformation. Reject request if this is requested */
  if ( 0 == p_req_loc_r9->m.commonIEsRequestLocationInformationPresent )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "CommonIE not present. Abort session" );
    cp_LppErr->w_ErrType |= LPP_ERROR_TYPE_LPP_ABORT;
    cp_LppErr->u_AbortErrCode  = LPP_ABORT_TARGET;
    return ( FALSE );
  }

  /* Check the request from server is tained before accepting the request.
  *  In concurrency sessions the duplicate or overlapping request are not allowed. LPP Abort is send
  *  for new transaction. Otherwise this transaction is ignored.
  */
  if ( (p_req_loc_r9->commonIEsRequestLocationInformation.locationInformationType
        == LocationInformationType_locationMeasurementsRequired) ||
       (p_req_loc_r9->commonIEsRequestLocationInformation.locationInformationType
        == LocationInformationType_locationMeasurementsPreferred ) )
  {
    if ( (cm_AcceptedRliTech & C_LPP_TECH_ECID_MEAS) != 0 )
    {
      MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "ECID MSA request pending 0x%X", cm_AcceptedRliTech );
      cp_LppErr->w_ErrType |= LPP_ERROR_TYPE_LPP_ABORT;
      cp_LppErr->u_AbortErrCode  = LPP_ABORT_TARGET;
      return ( FALSE );
    }
  }

  /* ECID method requested by the server */
  requested_ecid_data =
    p_req_loc_r9->ecid_RequestLocationInformation.requestedMeasurements.data[0];

  if ( requested_ecid_data & (0x80 >> LPP_PROV_ECID_RSRP % 8))
  {
    *p_lpp_ecid_req_loc |= LPP_PROV_ECID_RSRP;
  }
  if ( requested_ecid_data & (0x80 >> LPP_PROV_ECID_RSRQ % 8))
  {
    *p_lpp_ecid_req_loc |= LPP_PROV_ECID_RSRQ;
  }
  if ( requested_ecid_data & (0x80 >> LPP_PROV_ECID_RXTX % 8))
  {
    *p_lpp_ecid_req_loc |= LPP_PROV_ECID_RXTX;
  }

  *cpm_RliTech |= C_LPP_TECH_ECID_MEAS;
  *cpq_LppFlags |= TM_LPP_REQ_LOC_INFO_ECID_RCVD;
  return ( TRUE );
}

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
boolean tm_lpp_common_check_supported_rlitech_req( RequestLocationInformation_r9_IEs const * const p_req_loc_r9 )
{
  /* We do not support anything other location method than LPP OTDOA, GNSS, ECID and LPPe WLAN-AP.
   * So if these are not requested, return FALSE */
  if ( (p_req_loc_r9->m.otdoa_RequestLocationInformationPresent  == 1) ||
       (p_req_loc_r9->m.a_gnss_RequestLocationInformationPresent == 1) ||
       (p_req_loc_r9->m.ecid_RequestLocationInformationPresent   == 1) )
  {
    return ( TRUE );
  }

#ifdef FEATURE_NR5G_SA
  /* We do not support WLAN, UBP, DBH in Rel13-15 IE's*/
  else if ( ( p_req_loc_r9->m.otdoa_RequestLocationInformationPresent  == 0 ) &&
            ( p_req_loc_r9->m.a_gnss_RequestLocationInformationPresent == 0 ) &&
            ( p_req_loc_r9->m.ecid_RequestLocationInformationPresent   == 0 ) && 
            ( ( p_req_loc_r9->m.bt_RequestLocationInformation_r13Present == 1 )||
              ( p_req_loc_r9->m.sensor_RequestLocationInformation_r13Present == 1 ) ||
              ( p_req_loc_r9->m.tbs_RequestLocationInformation_r13Present == 1 ) ||
              ( p_req_loc_r9->m.wlan_RequestLocationInformation_r13Present == 1 ) ) )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Req_Loc Rel13-15 : TBS/WLAN/BT/BARO not supported" );
    return ( FALSE );
  }
#endif /* FEATURE_NR5G_SA */

  return ( FALSE );
}

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
                                                    rli_tx_tech_mask const cm_RliTech )
{
  if ( ( cm_RliTech & C_LPP_TECH_GNSS_MSB ) != 0 )
  {
    return ( LPP_locationEstimateRequired == cz_LocType );
  }

  if ( ( cm_RliTech & (C_LPP_TECH_GNSS_MSA | C_LPP_TECH_OTDOA_MSA | C_LPP_TECH_ECID_MEAS)) != 0 )
  {
    return ( LPP_locationMeasurementsRequired == cz_LocType );
  }

  /* For LPPe don't bother about the LocationInfoType. Ensure if any of the LPPe position method
     is requested */
  if ( ( cm_RliTech & (C_LPP_TECH_HA3DPOS | C_LPP_TECH_WLAN_AP_MEAS | 
	                   C_LPP_TECH_SRN_MEAS | C_LPP_TECH_BARO_MEAS ) ) != 0 )
  {
    return ( TRUE );
  }

  return ( FALSE );
}

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
uint32 tm_lpp_common_xlate_lpp_hor_qos_req (  QoS const * const p_smlc_qos, uint32 const * const p_hor_acc_meters )
{
  float  f_scale   = 0;
  uint32 pdapi_hor_qos = 0;

  if ( (p_smlc_qos == NULL) || (p_hor_acc_meters == NULL) )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Null Pointer parameter", 0, 0, 0);
    return ( 0 );
  }

  if ( p_smlc_qos->m.horizontalAccuracyPresent == FALSE )
  {
    return ( *p_hor_acc_meters );
  }

  pdapi_hor_qos = tm_umts_common_utils_GADK_to_meter(TRUE, p_smlc_qos->horizontalAccuracy.accuracy);

  /* scale position unc. to that of 1 sigma confidence level */
  f_scale= tm_util_get_cep_scale_ratio_from_pct(
               (float)(p_smlc_qos->horizontalAccuracy.confidence * 0.01f),
               0.63f);
  pdapi_hor_qos = (uint32)((float)(pdapi_hor_qos) * (float)(f_scale));
  return ( pdapi_hor_qos );
}

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
                                          tm_umts_common_pos_for_net_s_type *const cpz_PendingPosToNetwork )
{
  tm_lpp_pending_pos_to_network_s_type *pz_TxPosData   = NULL;

  if ( ( NULL == cpz_PendingPosToNetwork) || (NULL == cpz_LppTransItem) )
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null ptr" );
    return;
  }
  MSG_3( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "RLITech 0x%X, LPP Flag 0x%X, Pos valid %d",
         cpz_LppTransItem->z_lpp_data_type.z_rli.m_tech, cpz_LppTransItem->z_lpp_data_type.q_flags,
         cpz_PendingPosToNetwork->u_valid );

  if ( ((cpz_LppTransItem->z_lpp_data_type.z_rli.m_tech & C_LPP_TECH_GNSS_MSB) != 0) &&
    (0 == (cpz_LppTransItem->z_lpp_data_type.q_flags & LPP_POS_DATA_VALID)) &&
    (cpz_PendingPosToNetwork->u_valid == TRUE)
    )
  {
    pz_TxPosData = &(cpz_LppTransItem->z_lpp_data_type.z_pos_info);

    pz_TxPosData->q_pos_sources = cpz_PendingPosToNetwork->z_pos_info.q_pos_sources;

    if ( ( cpz_PendingPosToNetwork->z_pos_info.q_pos_sources & C_POS_SOURCE_GLO ) && // Glonass pos source ?
      !( cpz_PendingPosToNetwork->z_pos_info.q_pos_sources & ~C_POS_SOURCE_GLO ) )  // No other pos sources are used ?
    {
      pz_TxPosData->q_GloTodMs = cpz_PendingPosToNetwork->z_pos_info.q_GloTimeMs;
    }

    pz_TxPosData->z_pos = cpz_PendingPosToNetwork->z_pos_info.z_pos;

    pz_TxPosData->f_VelEnu[0] = cpz_PendingPosToNetwork->z_pos_info.f_VelEnu[0];
    pz_TxPosData->f_VelEnu[1] = cpz_PendingPosToNetwork->z_pos_info.f_VelEnu[1];
    pz_TxPosData->f_VelEnu[2] = cpz_PendingPosToNetwork->z_pos_info.f_VelEnu[2];
    pz_TxPosData->f_Vunc3dMps = cpz_PendingPosToNetwork->z_pos_info.f_Vunc3dMps;
    pz_TxPosData->f_velVertUnc = cpz_PendingPosToNetwork->z_pos_info.f_VelVertUnc;
    pz_TxPosData->q_fix_time_gps_msec = cpz_PendingPosToNetwork->z_pos_info.q_fix_time_gps_msec;
    pz_TxPosData->q_fix_time_utc_msec = cpz_PendingPosToNetwork->z_pos_info.q_fix_time_utc_msec;
    if ( (cpz_LppTransItem->z_lpp_data_type.m_req_flags & TM_LPP_REQ_LOC_INFO_VEL_REQUESTED) ||
      (cpz_LppTransItem->z_lpp_data_type.m_req_flags & TM_LPP_MAY_RET_ADD_INFO) )
    {
      pz_TxPosData->vel_valid = cpz_PendingPosToNetwork->z_pos_info.vel_valid;
    }
    else
    {
      pz_TxPosData->vel_valid = FALSE;
    }
    pz_TxPosData->w_Alt = cpz_PendingPosToNetwork->z_pos_info.w_Alt;
    pz_TxPosData->q_Lat = cpz_PendingPosToNetwork->z_pos_info.q_Lat;
    pz_TxPosData->l_Long = cpz_PendingPosToNetwork->z_pos_info.l_Long;
    pz_TxPosData->w_GpsWeek = cpz_PendingPosToNetwork->z_pos_info.w_GpsWeek;
    pz_TxPosData->f_ClockBias = cpz_PendingPosToNetwork->z_pos_info.f_ClockBias;
    pz_TxPosData->f_ClockBiasUncMs = cpz_PendingPosToNetwork->z_pos_info.f_ClockBiasUncMs;

    cpz_LppTransItem->z_lpp_data_type.q_flags |= LPP_POS_DATA_VALID;
    cpz_LppTransItem->z_lpp_data_type.q_flags |= LPP_ENC_POS_DATA;

    MSG( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "GNSS position transferred" );
  }
  else
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can't copy GNSS position" );
  }
}


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
                                           gnss_time_info * const cpz_GnssTimeStamp,
                                           SmGnssMeasReportStructType * const cpz_GnssMeasReport )
{
  if (( NULL == cpz_GnssTimeStamp) || (NULL == cpz_LppTransItem) || (NULL == cpz_GnssMeasReport))
  {
    MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null prt" );
    return;
  }

  MSG_7( MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
         "RLI Tech 0x%X LPP valid flag 0x%X MeasRpt %u Time valid %1u %u, %u, %lu",
         cpz_LppTransItem->z_lpp_data_type.z_rli.m_tech,
         cpz_LppTransItem->z_lpp_data_type.q_flags,
         cpz_GnssMeasReport->u_meas_reports_bmap,
         cpz_GnssTimeStamp->m.refTimeWeekNumValid,
         cpz_GnssTimeStamp->w_ref_time_week_num,
         cpz_GnssMeasReport->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS].z_MeasBlk.z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.w_GpsWeek,
         cpz_GnssMeasReport->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS].z_MeasBlk.z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.q_GpsMsec );

  /* Copy the GNSS Measurement from MGP once per transaction item. After copying the transaction
     item is marked with PRM valid, after that no mew measurements will be copied again. */
  if ( ((cpz_LppTransItem->z_lpp_data_type.z_rli.m_tech & C_LPP_TECH_GNSS_MSA) != 0) &&
       (0 == (cpz_LppTransItem->z_lpp_data_type.q_flags & LPP_PRM_DATA_VALID)) &&
       ((cpz_GnssMeasReport->u_meas_reports_bmap) != 0)
      )
  {
    /* Propagate each GPS and/or GLONASS SV's measurement to the common reference point:
    *  p_prm_data->z_MeasBlk.z_GnssHeader.z_MeasClock.q_RefFCount
    */
    if ( cpz_GnssMeasReport->u_meas_reports_bmap & SM_GNSS_MEAS_REPORTS_BMAP_GPS )
    {
      gps_MeasBlkPropagate( &(cpz_GnssMeasReport->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS].z_MeasBlk) );
    }
    if ( cpz_GnssMeasReport->u_meas_reports_bmap & SM_GNSS_MEAS_REPORTS_BMAP_GLONASS )
    {
      gnss_MeasBlkPropagate( &(cpz_GnssMeasReport->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS].z_MeasBlk) );
      gnss_GloMeasBlkGGRfgdAdjust( &(cpz_GnssMeasReport->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS].z_MeasBlk) );
    }

    /* Populate the GPS msec time from Measurement clock time */
    if ( cpz_GnssTimeStamp->m.refTimeWeekNumValid )
    {
      cpz_GnssTimeStamp->t_pos_time_stamp_ms =
        tm_umts_common_utils_full_gps_ms( cpz_GnssTimeStamp->w_ref_time_week_num,
                                          cpz_GnssMeasReport->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS].z_MeasBlk.z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.q_GpsMsec );
      cpz_GnssTimeStamp->m.posTimeStampValid = TRUE;
    }

    /* Copy PRM & Mark PRM is valid */
    cpz_LppTransItem->z_lpp_data_type.p_gnss_meas_report = cpz_GnssMeasReport;
    cpz_LppTransItem->z_lpp_data_type.q_flags |= LPP_ENC_PRM_DATA;
    cpz_LppTransItem->z_lpp_data_type.q_flags |= LPP_PRM_DATA_VALID;

    MSG( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "GNSS meas transferred" );
  }
  else
  {
     MSG( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can't copy GNSS meas" );
  }
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
boolean tm_lpp_common_set_error_code( tm_lpp_error_type * const cpz_LppTxErr, rli_tx_tech_mask const m_RliTech )
{
  if ( NULL == cpz_LppTxErr )
  {
    return ( FALSE );
  }

#ifdef FEATURE_GNSS_LPPE
  /* If the EPDU is invalid or some of the IE in the LPPe EPDU isn't support, then the LPP
     transaction should be dropped if LPP position method can't be selected. */
  if ( ((cpz_LppTxErr->w_ErrType & LPP_ERROR_TYPE_PROV_LOC_EPDU_ERROR) != 0) &&
    (cpz_LppTxErr->u_EpduErrCode != LPPe_EPDU_FAILURE_UNDEF) &&
    (0 == m_RliTech) )
  {
    cpz_LppTxErr->w_ErrType |= LPP_ERROR_TYPE_IGNORE_LPP_TRANS;
    return ( TRUE );
  }
#endif /* End - FEATURE_GNSS_LPPE */

  return ( FALSE );
}

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
                                                       )
{
  uint32 q_remainingRliTimeMsec = os_TimerCheck( cpz_rli_timer );

  if ( q_remainingRliTimeMsec > cq_OtdoaEngineIpcDelay )
  {
    return ( q_remainingRliTimeMsec + cq_rliTimeoutShimMsec );
  }

  MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_MED, "Remaining RLI time %d is less than IPC delay time", q_remainingRliTimeMsec );
  return ( 0 );
}

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
void  tm_lpp_send_req_loc_type_to_pdapi(tm_lpp_data_type    z_lpp_data_type,
                                         pdsm_pd_comm_protocol_e_type e_protocol_type)
{
  boolean b_glo_requested = (z_lpp_data_type.z_rli.s_gnss_method & C_POS_SOURCE_GLO) ? TRUE : FALSE;
  boolean b_gps_requested = (z_lpp_data_type.z_rli.s_gnss_method & C_POS_SOURCE_GPS) ? TRUE : FALSE;
  uint32 sess_type = 0; 
  
  if (C_LPP_TECH_GNSS_MSB == z_lpp_data_type.z_rli.m_tech)
  {
    /* set the GPS and GLO flags based on what was requested for this session */
    if (b_glo_requested )
    {
      sess_type |= PDSM_PD_SESS_REQ_TYPE_MSB_GLO;
    }
    
    if (b_gps_requested )
    {
      sess_type |= PDSM_PD_SESS_REQ_TYPE_MSB_GPS;
    }
  }
  if (C_LPP_TECH_GNSS_MSA == z_lpp_data_type.z_rli.m_tech)
  {
    if ( b_glo_requested )
    {
      sess_type |= PDSM_PD_SESS_REQ_TYPE_MSA_GLO;
    }
    if (b_gps_requested )
    {
      sess_type |= PDSM_PD_SESS_REQ_TYPE_MSA_GPS;
    }
  }
  if (C_LPP_TECH_ECID_MEAS == z_lpp_data_type.z_rli.m_tech)
  {
    sess_type |= PDSM_PD_SESS_REQ_TYPE_ECID;
  }
  if (C_LPP_TECH_OTDOA_MSA == z_lpp_data_type.z_rli.m_tech)
  {
    sess_type |= PDSM_PD_SESS_REQ_TYPE_OTDOA;
  }
  
  MSG_2( MSG_SSID_GPSSM, MSG_LEGACY_MED, "Requested sess type %d, protocol type %d,", sess_type, e_protocol_type);
  
  if ( 0 != sess_type )
  {
    tm_send_sess_type_pdapi_event(sess_type, e_protocol_type);
  }
}

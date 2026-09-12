/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         TM RRLP Utility

GENERAL DESCRIPTION
  This file contains TM's RRLP protocol utilities, which convert decoded RRLP
  messages for the consumption of TmCore.  

  Copyright (c) 2007 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2016 Qualcomm Technologies, Inc.
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved. 

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* ^L<EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/umts_up/supl/src/tm_rrlp_utils.c#1 $ $DateTime: 2019/10/24 01:30:36 $ $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  02/10/17   sj      First Version

===========================================================================*/
#include "comdef.h"
#include "gps_variation.h"
#include "customer.h"   /* Customer configuration file */

#include "msg.h"
#include "tm_rrlp_utils.h"
#include "tm_umts_up_supl.h"

#ifdef FEATURE_ABDS_RRLP_SUPL
 
/*
 ******************************************************************************
 *
 * tm_rrlp_bds_rti_build
 *
 * Function description:
 *  Translate RRLP BDS RTI to a gnss_common.h format used by rest of the engine.
 *
 * Parameters:
 *   p_ganss_rti     : input pointer to the structure holding RRLP BDS RTI
 *   p_bds_sv_health : output pointer to the gnss_common structure
 *
 * Side Effects:
 *   None:
 *
 * Return value:
 *   boolean: Successful translation or not.
 *
 ******************************************************************************
*/
static boolean tm_rrlp_bds_rti_build
(
  rrlp_dec_GANSSRealTimeIntegrity *p_ganss_rti,
  gnss_AdvHealthStructType        *p_bds_sv_health
)
{
  uint8 u_i = 0;
  rrlp_dec_BadSignalElement *p_bad_sig;

  if((p_ganss_rti == NULL) || (p_bds_sv_health == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x", 
          p_ganss_rti, p_bds_sv_health);
    return FALSE;
  }
  
  p_bds_sv_health->e_GnssType = MGP_GNSS_TYPE_BDS;

  p_bds_sv_health->q_BadSvMask  = 0;
  p_bds_sv_health->q_BadSvMask2 = 0;
	  
  for(u_i = 0; 
      (u_i < p_ganss_rti->ganssBadSignalList.n) && 
      ((u_i < sizeof(p_bds_sv_health->q_BadSvMask))*8);
      u_i++)
  {
    p_bad_sig = &p_ganss_rti->ganssBadSignalList.elem[u_i];
    p_bds_sv_health->q_BadSvMask |= 1 << p_bad_sig->badSVID;
  }
  return TRUE;
}

/*
 ******************************************************************************
 *
 * tm_rrlp_bds_timemodel_build
 *
 * Function description:
 *  Translate RRLP BDS time model to a gnss_common.h format used by rest of 
 *  the engine.
 *
 * Parameters:
 *   p_ganss_time_model : input pointer to the structure holding RRLP BDS timemodel
 *   p_bds_time_model : output pointer to the gnss_common structure
 *
 * Side Effects:
 *   None:
 *
 * Return value:
 *   boolean: Successful translation or not.
 *
 ******************************************************************************
*/
static boolean tm_rrlp_bds_timemodel_build
( 
  rrlp_dec_GANSSTimeModelElement *p_ganss_time_model,
  gnss_TimeModelType             *p_bds_time_model,
  uint16                         *p_w_bds_week,
  rrlp_dec_GANSSReferenceTime    *p_ganss_ref_time
)
{
  if((p_bds_time_model == NULL) || (p_ganss_time_model == NULL) || (p_w_bds_week == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x 0x%x", 
          p_ganss_time_model, p_bds_time_model, p_w_bds_week);
    return FALSE;
  }

  if ( p_ganss_time_model->gnssTOID != C_RRLP_GNSS_TO_ID_GPS )
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Bad GNSS-to-id in BDS TimeModel %d", 
          p_ganss_time_model->gnssTOID);
    return FALSE;
  }

  p_bds_time_model->l_A0 = p_ganss_time_model->tA0;
  p_bds_time_model->q_GanssTimeModelRefTime = p_ganss_time_model->ganssTimeModelRefTime;
  p_bds_time_model->u_GnssToID = C_GNSS_TO_ID_GPS;
  /* until the MGP API proivdes a different way to identify, use 0xFFFF for NA */
  p_bds_time_model->w_DeltaT = C_GNSS_GANSST_DELTAT_UNKNOWN; 
  /* until the MGP API proivdes a different way to identify, use 0x8000 for NA */
  p_bds_time_model->x_A1 = C_GNSS_GANSST_A1_UNKNOWN;
  /* until the MGP API proivdes a different way to identify, use 0x8000 for NA */
  p_bds_time_model->x_A2 = C_GNSS_GANSST_A2_UNKNOWN; 
  /* until the MGP API proivdes a different way to identify, use 0xFFFF for NA */
  p_bds_time_model->w_WeekNo = C_GNSS_GANSST_WEEK_UNKNOWN;

  if (TRUE == p_ganss_time_model->m.weekNumberPresent)
  {
    p_bds_time_model->w_WeekNo = p_ganss_time_model->weekNumber;
    /* If w_bds_week is not known yet, and if ganss_ref_time is not provided,
     * then the week_number provided in bds time model is in relation to GPS 
     * Time */
    if((*p_w_bds_week == 0) && (p_ganss_ref_time == NULL))
    {
      #define C_BDS_GPS_WEEK_OFFSET    (1356)
      /* if bds_week is not set, try to convert the the given GPS week number
       * to BDS week number */
      *p_w_bds_week = p_bds_time_model->w_WeekNo - C_BDS_GPS_WEEK_OFFSET;
    }
  }
  if (TRUE == p_ganss_time_model->m.tA1Present)
  {
    p_bds_time_model->x_A1 = p_ganss_time_model->tA1;
  }
  if (TRUE == p_ganss_time_model->m.tA2Present)
  {
    p_bds_time_model->x_A2 = p_ganss_time_model->tA2;
  }

  return TRUE;
}

void tm_rrlp_bds_grid_iono_build
( 
  rrlp_dec_BDS_GridModelParameter_r12 *p_assist, 
  bds_IonoStructType                  *p_tm_post_data_payload 
)
{
  /* TODO */
}

/*
 ******************************************************************************
 *
 * tm_rrlp_bds_global_iono_build
 *
 * Function description:
 *  Translate RRLP BDS global iono to a gnss_common.h format used by rest of 
 *  the engine.
 *
 * Parameters:
 *   p_ganss_common_data : input pointer to the structure holding RRLP BDS iono
 *   p_bds_iono : output pointer to the gnss_common iono structure
 *
 * Side Effects:
 *   None:
 *
 * Return value:
 *   boolean: Successful translation or not.
 *
 ******************************************************************************
*/
static boolean tm_rrlp_bds_global_iono_build
( 
  rrlp_dec_GANSSCommonAssistData *p_ganss_common_data, 
  bds_IonoStructType       *p_bds_iono 
)
{
  rrlp_dec_GANSSAddIonosphericModel *p_ganss_add_iono_model = NULL;
  if((p_ganss_common_data == NULL) || (p_bds_iono == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x",
          p_ganss_common_data, p_bds_iono);
    return FALSE;
  }

  if(p_ganss_common_data->m.ganssAddIonosphericModelPresent != 1)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unsupported Iono model!");
    return FALSE;
  }

  p_ganss_add_iono_model = &p_ganss_common_data->ganssAddIonosphericModel;

  if((p_ganss_add_iono_model->dataID.data[0] & 0x3) == 0x3)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unsupported Iono data 0x%x!", 
          p_ganss_add_iono_model->dataID.data);
    return FALSE;
  }

  p_bds_iono->u_Alpha0 = p_ganss_add_iono_model->ionoModel.alfa0;
  p_bds_iono->u_Alpha1 = p_ganss_add_iono_model->ionoModel.alfa1;
  p_bds_iono->u_Alpha2 = p_ganss_add_iono_model->ionoModel.alfa2;
  p_bds_iono->u_Alpha3 = p_ganss_add_iono_model->ionoModel.alfa3;
  p_bds_iono->u_Beta0 = p_ganss_add_iono_model->ionoModel.beta0;
  p_bds_iono->u_Beta1 = p_ganss_add_iono_model->ionoModel.beta1;
  p_bds_iono->u_Beta2 = p_ganss_add_iono_model->ionoModel.beta2;
  p_bds_iono->u_Beta3 = p_ganss_add_iono_model->ionoModel.beta3;

  return TRUE;
}

/*
 ******************************************************************************
 *
 * tm_rrlp_bds_alm_build
 *
 * Function description:
 *  Translate RRLP BDS Almanac to a gnss_common.h format used by rest of 
 *  the engine.
 *
 * Parameters:
 *   p_ganss_alm_elt : input pointer to the structure holding RRLP Almanac
 *   p_bds_alm : output pointer to the gnss_common almanac structure
 *   u_amb_bds_wk_no: BDS week number (ambiguous)
 *
 * Side Effects:
 *   None:
 *
 * Return value:
 *   boolean: Successful translation or not.
 *
 ******************************************************************************
*/
static boolean tm_rrlp_bds_alm_build
(
  rrlp_dec_GANSSAlmanacElement *p_ganss_alm_elt,
  bds_AlmStructType            *p_bds_alm,
  uint8                         u_amb_bds_wk_no
)
{
  rrlp_dec_Almanac_BDSAlmanacSet_r12 *p_bds_alm_elt = NULL;
	
  if((p_ganss_alm_elt == NULL) || (p_bds_alm == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x", 
          p_ganss_alm_elt, p_bds_alm);
    return FALSE;
  }

  if(p_ganss_alm_elt->t != T_rrlp_dec_GANSSAlmanacElement_keplerianBDSAlmanac_r12)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Bad BDS orbit model. Type %d",
          p_ganss_alm_elt->t);
    return FALSE;
  }

  p_bds_alm_elt = p_ganss_alm_elt->u.keplerianBDSAlmanac_r12;
  memset(p_bds_alm, 0, sizeof(*p_bds_alm));

  /* Square-root of the Semi-Major axis (24-bits) */
  p_bds_alm->q_SqrtA     = p_bds_alm_elt->bdsAlmSqrtA_r12;
  /* Orbital Eccentricity (17-bits) */
  p_bds_alm->q_E         = p_bds_alm_elt->bdsAlmE_r12;
  /* Argument of Perigee (24-bits) */
  p_bds_alm->q_Omega     = p_bds_alm_elt->bdsAlmW_r12;
  /* Mean anomaly at reference time (24-bits) */
  p_bds_alm->q_M0        = p_bds_alm_elt->bdsAlmM0_r12;                
  /* Longitude of the ascending node of orbit plane at weekly epoch (24-bits) */
  p_bds_alm->q_Omega0    = p_bds_alm_elt->bdsAlmOmega0_r12;            
  /* Rate of Right Ascension (17-bits) */
  p_bds_alm->q_OmegaDot  = p_bds_alm_elt->bdsAlmOmegaDot_r12;
  /* Correction of orbit reference iclination at reference angle (16-bits) */
  p_bds_alm->w_DeltaI    = p_bds_alm_elt->bdsAlmDeltaI_r12;
  /* Clock bias coefficient (11-bits) */
  p_bds_alm->w_A0        = p_bds_alm_elt->bdsAlmA0_r12;
  /* Clock drift coefficient (11-bits) */
  p_bds_alm->w_A1        = p_bds_alm_elt->bdsAlmA1_r12;
  /* BDS week number, can be ambiguous depending on the src */
  p_bds_alm->w_BdsWeek   = u_amb_bds_wk_no;

  if(p_bds_alm_elt->m.bdsAlmToa_r12Present)
  {
    /* Reference time of Almanac (8-bits) */
    p_bds_alm->u_Toa = p_bds_alm_elt->bdsAlmToa_r12;
  }
  /* SV ID: 201 - 237 from SvID 0-63 */
  p_bds_alm->u_Sv = BDS_LOWEST_ID + p_bds_alm_elt->svID;

  return TRUE;
}

/*
 ******************************************************************************
 *
 * tm_rrlp_bds_eph_build
 *
 * Function description:
 *  Translate RRLP BDS Ephemeris to a gnss_common.h format used by rest of 
 *  the engine.
 *
 * Parameters:
 *   p_ganss_sat_element : input pointer to the structure holding RRLP Ephemeris
 *   p_bds_eph : output pointer to the gnss_common BDS ephemeris structure
 *
 * Side Effects:
 *   None:
 *
 * Return value:
 *   boolean: Successful translation or not.
 *
 ******************************************************************************
*/
static boolean tm_rrlp_bds_eph_build
(
  
  rrlp_dec_GANSSSatelliteElement *p_ganss_sat_element,
  bds_EphStructType              *p_bds_eph,
  uint16                          w_bds_week
)
{
   rrlp_dec_BDSClockModel_r12            *p_bds_clk_model   = NULL;
   rrlp_dec_NavModel_BDSKeplerianSet_r12 *p_bds_orbit_model = NULL;
	
  if((p_ganss_sat_element == NULL) || 
     (p_bds_eph == NULL) || 
     (w_bds_week == 0))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x %d", 
          p_ganss_sat_element, p_bds_eph, w_bds_week);
    return FALSE;
  }

  if(p_ganss_sat_element->ganssOrbitModel.t != 
     T_rrlp_dec_GANSSOrbitModel_bdsKeplerianSet_r12)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Bad BDS orbit model. Type %d",
          p_ganss_sat_element->ganssOrbitModel.t);
    return FALSE;
  }

  if(p_ganss_sat_element->ganssClockModel.t !=
     T_rrlp_dec_GANSSClockModel_bdsClockModel_r12)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Bad BDS clock model. Type %d",
          p_ganss_sat_element->ganssClockModel.t);
    return FALSE;
  }

  memset(p_bds_eph, 0, sizeof(*p_bds_eph));

  /* Unambiguous BDS week number, convert ganss day to ganss week */
  p_bds_eph->w_BdsWeek = w_bds_week;

  p_bds_clk_model   = p_ganss_sat_element->ganssClockModel.u.bdsClockModel_r12;
  p_bds_orbit_model = p_ganss_sat_element->ganssOrbitModel.u.bdsKeplerianSet_r12;

  /*
   * Translate Orbit Model 
   */
  /* Longitude of the ascending node of orbit plane computed according to ref. time (32-bits) */
  p_bds_eph->q_Omega0   = p_bds_orbit_model->bdsOmega0_r12;
  /* Rate of Right Ascension (24-bits) */
  p_bds_eph->q_OmegaDot = p_bds_orbit_model->bdsOmegaDot_r12;
  /* Inclination angle at reference time (32-bits) */
  p_bds_eph->q_I0   = p_bds_orbit_model->bdsI0_r12;                
  /* Rate of inclination angle (14-bits) */
  p_bds_eph->w_Idot = p_bds_orbit_model->bdsIDot_r12;
  /* Amplitude of the cosine harmonic correction term to argument of latitude (18-bits) */
  p_bds_eph->q_Cuc  = p_bds_orbit_model->bdsCuc_r12;               
  /* Amplitude of the sine harmonic correction term to argument of latitude (18-bits) */
  p_bds_eph->q_Cus  = p_bds_orbit_model->bdsCus_r12;               
  /* Amplitude of the cosine harmonic correction term to Orbit Radius (18-bits) */
  p_bds_eph->q_Crc  = p_bds_orbit_model->bdsCrc_r12;               
  /* Amplitude of the sine harmonic correction term to Orbit Radius (18-bits) */
  p_bds_eph->q_Crs  = p_bds_orbit_model->bdsCrs_r12;               
  /* Amplitude of the cosine harmonic correction term to angle of inclination (18-bits) */
  p_bds_eph->q_Cic  = p_bds_orbit_model->bdsCic_r12;               
  /* Amplitude of the sine harmonic correction term to angle of inclination (18-bits) */
  p_bds_eph->q_Cis  = p_bds_orbit_model->bdsCis_r12;               
  /* Orbital Eccentricity (32-bits) */
  p_bds_eph->q_E  = p_bds_orbit_model->bdsE_r12;
  /* Argument of Perigee (32-bits) */
  p_bds_eph->q_Omega  = p_bds_orbit_model->bdsW_r12;
  /* Mean motion difference from computed value (16-bits) */
  p_bds_eph->w_DeltaN  = p_bds_orbit_model->bdsDeltaN_r12;
  /* Mean anomaly at reference time (32-bits) */
  p_bds_eph->q_M0   = p_bds_orbit_model->bdsM0_r12;
  /* User Range Accuracy Index (4-bits) */
  p_bds_eph->u_URAI = p_bds_orbit_model->bdsURAI_r12;
  /* Reference time of Ephemeris (17-bits) */
  p_bds_eph->q_Toe   = p_bds_orbit_model->bdsToe_r12;
  /* Square-root of the Semi-Major axis (32-bits) */
  p_bds_eph->q_SqrtA = p_bds_orbit_model->bdsAPowerHalf_r12;
  /* Issue of data ephemeris tag (5-bits) */
  p_bds_eph->u_Iode  = p_bds_orbit_model->bdsAODE_r12;
  
  
  /*
   * Translate Clock Model 
   */
  /* Clock bias coefficient (24-bits) */
  p_bds_eph->q_Toc  = p_bds_clk_model->bdsToc_r12;                
  /* Clock bias coefficient (24-bits) */
  p_bds_eph->q_A0   = p_bds_clk_model->bdsA0_r12;                
  /* Clock drift coefficient (22-bits) */
  p_bds_eph->q_A1   = p_bds_clk_model->bdsA1_r12;                
  /* Clock acceleration coefficient (11-bits) */
  p_bds_eph->w_A2   = p_bds_clk_model->bdsA2_r12;                
  /* Time of Group Delay (10-bits) */
  p_bds_eph->w_Tgd  = p_bds_clk_model->bdsTgd1_r12;               
  /* Issue of data clock (5-bits) */
  p_bds_eph->u_Iodc  = p_bds_clk_model->bdsAODC_r12;
  
  /* SV ID: 201 - 237 from SvID 0-63 */
  p_bds_eph->u_Sv = BDS_LOWEST_ID + p_ganss_sat_element->svID;
  /* SV Health (1-bit): SatH1 */
  p_bds_eph->u_Health = (*(p_ganss_sat_element->svHealth.data) >> 5) & 0x1;
  return TRUE;
}

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
  tm_prtl_type                 prtl_type,
  tm_sess_handle_type          sess_handle,
  rrlp_dec_GANSSNavModel      *p_ganss_nav_model,
  tm_post_data_payload_type   *p_tm_post_data_payload,
  uint16                      w_bds_week
)
{
  uint8 u_i;

  if((p_ganss_nav_model == NULL)      || 
     (p_tm_post_data_payload == NULL) || 
     (w_bds_week == 0))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x %d", 
          p_ganss_nav_model, p_tm_post_data_payload, w_bds_week);
    return;
  }

  for(u_i = 0; u_i < p_ganss_nav_model->ganssSatelliteList.n; u_i++)
  {
     if( !tm_rrlp_bds_eph_build
         (
            &p_ganss_nav_model->ganssSatelliteList.elem[u_i],
            &p_tm_post_data_payload->eph_bds_data,
            w_bds_week
         )
       )
     {
       MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL to Tmcore BDS Eph xlation failed");
       continue;
     }

     if ( !tm_post_data
          ( 
            prtl_type,
            sess_handle,
            TM_POST_DATA_TYPE_EPH_BDS,
            p_tm_post_data_payload
	  )
	)
     {
       MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL to Tmcore data post failed");
     }
  }
}

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
)
{
  uint8 u_i;

  if((p_ganss_alm_model == NULL) || (p_tm_post_data_payload == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x", 
          p_tm_post_data_payload, p_ganss_alm_model);
    return;
  }

  for(u_i = 0; u_i < p_ganss_alm_model->ganssAlmanacList.n; u_i++)
  {
     if( !tm_rrlp_bds_alm_build
         (
            &p_ganss_alm_model->ganssAlmanacList.elem[u_i],
            &p_tm_post_data_payload->alm_bds_data,
            p_ganss_alm_model->weekNumber
         )
       )
     {
       MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL to Tmcore BDS ALM xlation failed");
       continue;
     }

     if ( !tm_post_data
          ( 
            prtl_type,
	    sess_handle,
            TM_POST_DATA_TYPE_ALM_BDS,
            p_tm_post_data_payload
	  )
	)
     {
       MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL to Tmcore alm data post failed");
     }
  }
}

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
)
{
  uint8 u_i;

  if((p_ganss_rti_model == NULL) || (p_tm_post_data_payload == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x", 
          p_tm_post_data_payload, p_ganss_rti_model);
    return;
  }

  if( !tm_rrlp_bds_rti_build
      (
         p_ganss_rti_model,
         &p_tm_post_data_payload->gnss_sv_health_info
      )
    )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL to Tmcore BDS RTI xlation failed");
    return;
  }

  if ( !tm_post_data
       ( 
         prtl_type,
         sess_handle,
         TM_POST_DATA_TYPE_GNSS_SV_HEALTH_INFO,
         p_tm_post_data_payload
       )
     )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL to Tmcore BDS RTI data post failed");
  }
  return;
}

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
  tm_prtl_type                     prtl_type,
  tm_sess_handle_type              sess_handle,
  rrlp_dec_SeqOfGANSSTimeModel    *p_ganss_time_model,
  tm_post_data_payload_type       *p_tm_post_data_payload,
  uint16                          *p_w_bds_week,
  rrlp_dec_GANSSReferenceTime     *p_ganss_ref_time
)
{
  uint8 u_i;

  if((p_ganss_time_model == NULL) || (p_tm_post_data_payload == NULL) || (p_w_bds_week == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x 0x%x", 
          p_tm_post_data_payload, p_ganss_time_model, p_w_bds_week);
    return;
  }

  for(u_i = 0; u_i < p_ganss_time_model->n; u_i++)
  {
    if(!tm_rrlp_bds_timemodel_build(
           &p_ganss_time_model->elem[u_i],
           &p_tm_post_data_payload->gnss_time_model_data, 
            p_w_bds_week, p_ganss_ref_time))
    {
       MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
           "SUPL to Tmcore BDS TimeModel xlation failed");
       continue;
    }

    if ( !tm_post_data
         ( 
           prtl_type,
           sess_handle,
           TM_POST_DATA_TYPE_TIME_MODEL_BDS,
           p_tm_post_data_payload
         )
       )
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "SUPL to Tmcore BDS TimeModel data post failed");
    }
  }
  

  return;
}

void tm_rrlp_bds_grid_iono_post
( 
  tm_prtl_type                         prtl_type,
  tm_sess_handle_type                  sess_handle, 
  rrlp_dec_BDS_GridModelParameter_r12 *p_assist, 
  bds_IonoStructType                  *p_tm_post_data_payload 
)
{
  if((p_assist == NULL) || (p_tm_post_data_payload == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x", 
          p_tm_post_data_payload, p_assist);
    return;
  }


  /* TODO */
}

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
)
{
  if((p_ganss_common_data == NULL) || (p_tm_post_data_payload == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x", 
          p_tm_post_data_payload, p_ganss_common_data);
    return;
  }

  if(!tm_rrlp_bds_global_iono_build(
         p_ganss_common_data, 
         &p_tm_post_data_payload->global_iono_bds_data ))
  {
     MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
         "RRLP Iono translation failed");
     return;
  }

  if ( !tm_post_data
       (
         prtl_type,
         sess_handle,
         TM_POST_DATA_TYPE_BDS_GLOBAL_IONO,
         p_tm_post_data_payload
       )
     )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
        "RRLP Iono post failed");
  }
  

  return;
}

#endif /* FEATURE_ABDS_RRLP_SUPL */

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
)
{
  if((p_ganss_sat_element == NULL) || (p_eph_buffer_per_sv == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x", 
          p_ganss_sat_element, p_eph_buffer_per_sv);
    return;
  }
  /* TM API uses 65-88 for Glonass SVs, RRLP decoded SV ID ranges from 0-63 though
     Glonass uses only 24 SVs. */
  if ( p_ganss_sat_element->svID >= N_GLO_SV )
  {
    return;
  }

  p_eph_buffer_per_sv->z_Mask.u_FilledMask = 0;


  p_eph_buffer_per_sv->u_SvId = p_ganss_sat_element->svID + GLO_SLOTID_OFFSET + 1; /* start at 65 */

  /* Glonass eph. reference time */
  p_eph_buffer_per_sv->u_Tb = p_ganss_sat_element->iod;

  /* Glonass ICD: 4 LSB bits in svHealth are Ft;
     API: Ft is a parameter shown the URE at time tb: 4-bit LSBs;
     RRLP: svHealth BIT STRING (SIZE(5)) */
  p_eph_buffer_per_sv->u_Ft = (*(p_ganss_sat_element->svHealth.data) >> 3) & 0xF;
  p_eph_buffer_per_sv->z_Mask.z_FilledMask.u_FtFlag = TRUE;

  /* From RRLP: Bn(MSB) is Bit 1 of the 5-bit OSS_BITSTRING svHealth
     (Bit 5 being LSB); from API: Health flag Bn: 3-bit LSBs, only check bit 2 */
  p_eph_buffer_per_sv->u_Bn = (*p_ganss_sat_element->svHealth.data) >> 5 & 0x2;

  /* Gamma_n (ICD defines as 11 bits, scale factor 2^-40 ),
     RRLP decodes into 2's complement,
     MGP API: w_FreqOff using ICD format of sign+magnitude */
  p_eph_buffer_per_sv->w_FreqOff = tm_umts_common_utils_2s_comp_to_sign_mag(
            p_ganss_sat_element->ganssClockModel.u.glonassClockModel->gloGamma, 0x400);


  /* Tau_n: (IDC defines as 22 bits, scale factor 2^-30),
     RRLP decodes into 2's complement,
     MGP API: q_ClkCorr using ICD format of sign+magnitude */
  p_eph_buffer_per_sv->q_ClkCorr = tm_umts_common_utils_2s_comp_to_sign_mag(
            p_ganss_sat_element->ganssClockModel.u.glonassClockModel->gloTau, 0x200000);


  /* Delta Tau_n: (IDC defines as 5 bits, scale factor 2^-30); MGP API: u_L1L2; RRLP - optional */
  if ( p_ganss_sat_element->ganssClockModel.u.glonassClockModel->m.gloDeltaTauPresent  )
  {
    p_eph_buffer_per_sv->u_L1L2 = tm_umts_common_utils_2s_comp_to_sign_mag(
            p_ganss_sat_element->ganssClockModel.u.glonassClockModel->gloDeltaTau, 0x10);
  }
  else
  {
    /* temp. solution until gnss_common.h is updated with added FilledMask element to support this */
    p_eph_buffer_per_sv->u_L1L2 = 0;
  }


  /* En: 5 bits. RRLP: Integer (0...31) */
  p_eph_buffer_per_sv->u_En =
    p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloEn;

  /* P1: 2 bits. RRLP: Bit string size 2 */
  p_eph_buffer_per_sv->u_P1 =
    *(p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloP1.data);
  p_eph_buffer_per_sv->u_P1 >>= 6;

  /* P2: RRLP: boolean */
  p_eph_buffer_per_sv->u_P2 =
    p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloP2;

  /* M: RRLP: Integer (0..3) */
  p_eph_buffer_per_sv->u_GloType =
    p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloM;

  p_eph_buffer_per_sv->z_Mask.z_FilledMask.u_GloTypeFlag = 1;

  /* X: RRLP: Integer (-6710864..67108863);
        API expects ICD format 27 bits: sign magnitude */
  p_eph_buffer_per_sv->q_SatPos[0] =
    tm_umts_common_utils_2s_comp_to_sign_mag(
      p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloX,
      0x04000000 );

  /* X-dot: RRLP: Integer ( -8388608..8388607)
     API expects ICD format 24 bits: sign-magnitude */
  p_eph_buffer_per_sv->q_SatVel[0] =
    tm_umts_common_utils_2s_comp_to_sign_mag(
      p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloXdot,
      0x00800000 );

  /* X-dot-dot: RRLP: Integer ( -16..15 )
     API expects ICD format 5 bits: sign-magnitude */
  p_eph_buffer_per_sv->u_Acc[0] =
    tm_umts_common_utils_2s_comp_to_sign_mag(
      p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloXdotdot,
      0x00000010 );

  /* Y: RRLP: Integer (-6710864..67108863)
     API expects ICD format 24 bits: sign-magnitude */
  p_eph_buffer_per_sv->q_SatPos[1] =
    tm_umts_common_utils_2s_comp_to_sign_mag(
      p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloY,
      0x04000000 );

  /* Y-dot: RRLP: Integer ( -8388608..6388607)
     API expects ICD format 24 bits: sign-magnitude */
  p_eph_buffer_per_sv->q_SatVel[1] =
    tm_umts_common_utils_2s_comp_to_sign_mag(
      p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloYdot,
      0x00800000 );

  /* Y-dot-dot: RRLP: Integer ( -16..15 )
     API expects ICD format 5 bits: sign-magnitude */
  p_eph_buffer_per_sv->u_Acc[1] =
    tm_umts_common_utils_2s_comp_to_sign_mag(
      p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloYdotdot,
      0x00000010 );

  /* Z: RRLP: Integer (-6710864..67108863)
     API expects ICD format 24 bits: sign-magnitude */
  p_eph_buffer_per_sv->q_SatPos[2] =
    tm_umts_common_utils_2s_comp_to_sign_mag(
      p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloZ,
      0x04000000 );

  /* Z-dot: RRLP: Integer ( -8388608..6388607)
     API expects ICD format 24 bits: sign-magnitude */
  p_eph_buffer_per_sv->q_SatVel[2] =
    tm_umts_common_utils_2s_comp_to_sign_mag(
      p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloZdot,
      0x00800000 );

  /* Y-dot-dot: RRLP: Integer ( -16..15 )
     API expects ICD format 5 bits: sign-magnitude */
  p_eph_buffer_per_sv->u_Acc[2] =
    tm_umts_common_utils_2s_comp_to_sign_mag(
      p_ganss_sat_element->ganssOrbitModel.u.glonassECEF->gloZdotdot,
      0x00000010 );

  p_eph_buffer_per_sv->z_Mask.z_FilledMask.u_LnFlag = 0;
  p_eph_buffer_per_sv->z_Mask.z_FilledMask.u_NtFlag = 0;
  p_eph_buffer_per_sv->z_Mask.z_FilledMask.u_P4Flag = 0;
  p_eph_buffer_per_sv->z_Mask.z_FilledMask.u_TimeSrcPFlag = 0;

}

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
)
{
  struct rrlp_dec_GANSS_ID3         *p_local_rrlp_aux_list = NULL;
  struct rrlp_dec_GANSS_ID3_element *p_local_rrlp_aux_elem = NULL;
  uint8                         u_i;

  if((p_glo_aux == NULL) || (p_prot_glo_eph_buffer == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x", 
          p_glo_aux, p_prot_glo_eph_buffer);
    return;    
  }
  
  p_local_rrlp_aux_list = p_glo_aux;
  p_local_rrlp_aux_elem = p_local_rrlp_aux_list->elem;

  for ( u_i = 0; u_i < p_local_rrlp_aux_list->n; u_i++, p_local_rrlp_aux_elem++)
  {
    if (  ( p_local_rrlp_aux_elem->svID <= ( b_buf_sz - 1 ) ) &&
          ( ( p_local_rrlp_aux_elem->channelNumber >= C_PROT_GLO_FREQ_MIN ) &&
            ( p_local_rrlp_aux_elem->channelNumber <= C_PROT_GLO_FREQ_MAX )
          )
       )
    {
      p_prot_glo_eph_buffer[p_local_rrlp_aux_elem->svID].u_freq_num_valid = TRUE;

      p_prot_glo_eph_buffer[p_local_rrlp_aux_elem->svID].z_eph_glo_data.u_FreqNum =
        p_local_rrlp_aux_elem->channelNumber & 0x1F; /* the MGP API expects 5 bits */
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Incorrect Glonass SV_id=%u or channel#=%d, u_i=%u",
               p_local_rrlp_aux_elem->svID, p_local_rrlp_aux_elem->channelNumber, u_i);
    }
  }
}

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
)
{
  if((p_glo_alm_set == NULL) || (p_prot_glo_eph_buffer == NULL))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointers 0x%x 0x%x", 
          p_glo_alm_set, p_prot_glo_eph_buffer);
    return;    
  }
  
  if ( (p_glo_alm_set->gloAlmnA < 1) || ( p_glo_alm_set->gloAlmnA > 24) ) // gloAlmnA INTEGER (1..24)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"GLO Alm SvID invalid: %d", p_glo_alm_set->gloAlmnA, 0, 0 );
    return;
  }

  /* n_A is SV_ID :  gloAlmnA RRLP: INTEGER (1..24) */
  if (p_glo_alm_set->gloAlmnA - 1 < b_buff_sz)
  {
    p_prot_glo_eph_buffer[p_glo_alm_set->gloAlmnA - 1].u_freq_num_valid = TRUE;
    p_prot_glo_eph_buffer[p_glo_alm_set->gloAlmnA -1 ].z_eph_glo_data.u_FreqNum =
        p_glo_alm_set->gloAlmHA;
  }
  else
  {
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"GLO Alm SvID out of bounds: %d", p_glo_alm_set->gloAlmnA, 0, 0 );
  }
}


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
)
{

  uint8 u_index = 0, u_sv_iter = 0, u_save_loop = 0;
  rrlp_dec_GANSS_ID3_element *p_gnss_glonass_sat_elem = NULL;
  uint8 u_glo_num_svs = 0;

  if ( ( p_glo_aux == NULL ) || ( p_glo_aux->elem == NULL ) || ( p_glo_acqasst == NULL ) )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid pointer(s) found, exit supl_glo_acqassit_svid_freq_num_map_aux()",
                0, 0, 0 );
    return;
  }


  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Glonass AuxiInfo num of list = %u", p_glo_aux->n, 0, 0);

  /* Glonass Acq Assistance isn't received ? */
  if (0 == p_glo_acqasst->gnss_sv_dir.u_NumSvs)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Glonass Acq Assistance isn't received yet. Glo SvId to FreqNum correlation deferred.", 0, 0, 0 );
    return;
  }
  else
  {
    /* Glonass Acq Assistance is received already */

    /* Ensure the total SV count for SV-Dir & Steering are same. This help avoid looping twice. */
    u_glo_num_svs = p_glo_acqasst->gnss_sv_dir.u_NumSvs;

    if (p_glo_acqasst->gnss_sv_dir.u_NumSvs != p_glo_acqasst->z_steering_glo_data.u_NumSvs)
    {
      if (p_glo_acqasst->gnss_sv_dir.u_NumSvs > p_glo_acqasst->z_steering_glo_data.u_NumSvs)
      {
        u_glo_num_svs = p_glo_acqasst->z_steering_glo_data.u_NumSvs;
      }

      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SV-Dir & Steering SV count differs %u, %u. Using only %u AA SVs",
            p_glo_acqasst->gnss_sv_dir.u_NumSvs, p_glo_acqasst->z_steering_glo_data.u_NumSvs, u_glo_num_svs);
    }

    p_gnss_glonass_sat_elem = p_glo_aux->elem;

    for ( u_index = 0; u_index < p_glo_aux->n; ++u_index, ++p_gnss_glonass_sat_elem )
    {

      if ( p_gnss_glonass_sat_elem == NULL )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Unexpected NULL pointer p_gnss_glonass_sat_elem !!", 0, 0, 0);
        break;
      }

      if (  ( p_gnss_glonass_sat_elem->svID <= ( N_GLO_SV - 1 ) ) &&
            ( ( p_gnss_glonass_sat_elem->channelNumber >= C_PROT_GLO_FREQ_MIN ) &&
              ( p_gnss_glonass_sat_elem->channelNumber <= C_PROT_GLO_FREQ_MAX )
            )
         )
      {
        u_save_loop = 0;

        /* Loop through AA SVs & set the corresponding FreqNum */
        for ( u_sv_iter = 0; u_sv_iter < u_glo_num_svs; ++u_sv_iter )
        {

          // AA-SvDir
          // local w_Sv (65..88) & protocol p_gnss_glonass_sat_elem->svID (0..23)
          if (p_glo_acqasst->gnss_sv_dir.z_SvAzElInfo[u_sv_iter].w_Sv == (p_gnss_glonass_sat_elem->svID + GLO_SLOTID_OFFSET + 1))
          {
            p_glo_acqasst->u_freq_num_valid = TRUE;
            /* Per GLONASS ICD page 10, after year 2005, GLO SV would use frequency ID -7 to +6. Freq channel +7 to +13 would not be used. */
            p_glo_acqasst->gnss_sv_dir.z_SvAzElInfo[u_sv_iter].b_GloFreqIndex = p_gnss_glonass_sat_elem->channelNumber; /* the MGP API expects 5 bits */
            u_save_loop |= 0x1;
          }

          // AA-Steering
          // local w_Sv (65..88) & protocol p_gnss_glonass_sat_elem->svID (0..23)
          if (p_glo_acqasst->z_steering_glo_data.z_SvSteerInfo[u_sv_iter].w_Sv == (p_gnss_glonass_sat_elem->svID + GLO_SLOTID_OFFSET + 1))
          {
            p_glo_acqasst->u_freq_num_valid = TRUE;
            p_glo_acqasst->z_steering_glo_data.z_SvSteerInfo[u_sv_iter].b_GloFreqIndex = p_gnss_glonass_sat_elem->channelNumber;
            u_save_loop |= 0x2;
          }

          if ( u_save_loop == 3)
          {
            break; // break when both freqNum are set, else loop to max.
          }
        } /* saved GLONASS AA data for-loop */

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Mapped SV_id=%u and channel#=%d. u_i=%u", p_gnss_glonass_sat_elem->svID,
                     p_gnss_glonass_sat_elem->channelNumber, u_index);

      } /* if this SV's aux. assistance data valid*/
    } /* aux. assistance data for-loop */
  } /* save GLONASS AA data available */

}



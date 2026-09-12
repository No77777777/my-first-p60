/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  TM-RRLP-UP-SUPL sub-module

GENERAL DESCRIPTION
  This file implements TM RRLP-UP SUPL sub-module.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2017, 2019 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/umts_up/supl/src/tm_rrlp_up.c#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ----------------------------------------------------------
  10/04/19   gk      new session events to PDAPI
  09/30/19   skm     Support for 911 nDDS on DSDS targets 
  02/22/19   mj      Location Privacy Feature
  07/25/18   SJ      Inject QoS for MSA only when processing MeasurePosReq messages
  02/10/17   SJ      Initial version

============================================================================*/

#include "msg.h"
#include "math.h"

#include "mgp_api.h"
#include "sm_nv.h"
#include "tm_data.h"
#include "tm_common.h"

#include "uim.h"
#include "pbmlib.h"

#include "pd_comms_api.h"
#include "tm_asn1util.h"
#include "tm_rrlp_asn1_encode.h"
#include "tm_rrlp_asn1_decode.h"
#include "tm_rrlp_utils.h"
#include "tm_rrlp_up.h"

#include "tm_umts_common_utils.h" /* this files includes tm_rrlp.h */

#include "tm_sim_utils.h"

#include "sm_api.h" /* included only for the data structures defined there */
#include "sm_log.h"

#ifdef FEATURE_WCDMA_CM_CID_UPDATE_IN_DCH /* this would be defined in custwcdma.h */
#undef FEATURE_WCDMA_GPS_CELL_ID_READING /* this would be defined in custcgps.h */
#endif

#define SUPL_MAIN_VARIABLES_DEFINED
#include "tm_umts_up_supl.h"    /* which includes "tm_supl_asn1.h" */
#undef SUPL_MAIN_VARIABLES_DEFINED

#include "tm_umts_up_supl_comm.h"
#include "pdapibuf.h"

#ifdef FEATURE_SUPL1_TDS  /* defined in custcgps.h, if .builds defines FEATURE_CGPS_USES_TDS */
#include "tdsrrclsmif.h"
#endif

#include "fs_public.h"
#include "gps_fs_api.h"
#include "tm_prtl_iface.h"
#include "geran_eng_mode_read_api.h"
#include "tm_pdapi_client.h"
#include "gnss_calendar_util.h"
#include "gnss_wwan_iface.h"
#include "tm_l1_iface.h"

#include "tm_auxtech_iface.h"

static tm_sess_operation_mode_e_type rrlp_up_map_agps_mode_to_tm_oper_mode(supl_session_agps_mode_enum_type e_agps_mode);

/*
******************************************************************************
* tm_umts_up_supl_suplpos_rrlp_mpr_err_tx
*
* Function description:
*
* Encode and transmit SUPL POS carrying RRLP-measurePosResp-error
*
* Parameters: NONE.
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/

boolean tm_umts_up_supl_suplpos_rrlp_mpr_err_tx 
( 
  supl_status_s_type       *p_supl_session_status, 
  rrlp_enc_LocErrorReason  e_error_reason 
)
{
  boolean u_ret = FALSE;
  tm_pdu_buf_type   pdu_buff; /* including length and pointer to data */
  rrlp_enc_PDU z_rrlp;
  rrlp_enc_MsrPosition_Rsp z_rrlp_MsrPosition_Rsp;
  rrlp_enc_MsrPosition_Rsp *p_mpr;
  int encode_status;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, 
        "tm_umts_up_supl_suplpos_rrlp_mpr_err_tx mpr_err reason: %d",
        e_error_reason);

  memset(&z_rrlp, 0, sizeof(z_rrlp));
  memset(&z_rrlp_MsrPosition_Rsp, 0, sizeof(z_rrlp_MsrPosition_Rsp));
  memset(&pdu_buff, 0, sizeof(tm_pdu_buf_type));

  z_rrlp.component.u.msrPositionRsp = &z_rrlp_MsrPosition_Rsp;
  p_mpr = &z_rrlp_MsrPosition_Rsp;

  /* ref. num: the same as the one came from SMLC */
  z_rrlp.referenceNumber =
    p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.b_RefNum;

  z_rrlp.component.t = rrlp_msrPositionRsp_chosen;

  p_mpr->m.locationErrorPresent = 1;
  p_mpr->locationError.m.additionalAssistanceDataPresent = 0;
  p_mpr->locationError.locErrorReason = e_error_reason;

  /* Do the ASN.1 encoding for RRLP */
  encode_status = tm_osys_asn1_encode_pdu((void *)&z_rrlp, &pdu_buff, PROTO_RRLP);
  if ( (encode_status != 0) || (pdu_buff.value == NULL ) || (pdu_buff.length == 0) )
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
      pdu_buff.value = NULL;
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }
    return FALSE;
  }

  tm_umts_up_supl_terminate_timer_cancel_cf_state( C_SUPL_CF_STATE_SUPLPOS_XCHG );

  if ( supl_SuplPos_rrlp_tx (pdu_buff.value, (uint16)pdu_buff.length, NULL) )
  {
    if ( e_error_reason != rrlp_notEnoughSats )
    {
      /* if not the final no-fix report, expect the SLP to correct its errors in
         the current SuplPos-rrlp-measurePosRequest */
      supl_set_cf_state(C_SUPL_CF_STATE_UT3_RRLP, 0);
    }
    else
    {
      /* if final no-fix report, give the SLP a chance to send fix from other sources */
      supl_set_cf_state(C_SUPL_CF_STATE_UT3, 0);
    }
    u_ret = TRUE;
  }
  else
  {
    u_ret = FALSE;
  }

  /*Free the encoded PDU*/
  tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
  pdu_buff.value = NULL;

  return u_ret;

}

/*
 ******************************************************************************
 *
 * supl_rrlp_gps_assist_proc
 *
 * Function description:
 *  This function processes the OSS ASN.1 decoded RRLP gpsAssistance data and
 *  prepares such assistance data for injecting to MGP.
 *
 * Parameters:
 *
 *  p_assist: pointer to the OSS ASN.1 decoded RRLP gpsAsssitance data.
 *
 *  b_pos_req_present: set to TRUE if a position request is present in the RRLP 
 *            message, false otherwise.
 *
 * Return value:
 *
 *
 ******************************************************************************
*/

static boolean supl_rrlp_gps_assist_proc
( 
  supl_status_s_type     *p_supl_session_status, 
  rrlp_dec_ControlHeader *p_assist,
  boolean b_pos_req_present 
)
{
  tm_post_data_payload_type    z_tm_post_data_payload;

  gps_RefTimeStructType  reftime_data; /* QQQ: temp, as we seem to need week number for eph. for now */

  if ((NULL == p_assist) || (NULL == p_supl_session_status))
  {
     MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL RRLP payload NULL");
     return FALSE;
  }

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,"supl_rrlp_gps_assist_proc");
  memset(&z_tm_post_data_payload, 0x00, sizeof(tm_post_data_payload_type));
  memset(&reftime_data, 0x00, sizeof(reftime_data));

  /************** REFERENCE TIME **********************************/

  if ( p_assist->m.referenceTimePresent )
  {
    uint32 q_GpsTow;

    /* QQQ: for possible time-stamping SET-Assisted position, until a UTC-->GPS conversion
       utility is available */
    p_supl_session_status->z_set_assist_pos_time_stamp.w_ref_time_week_num =
      gnss_GpsWeekExtend( (uint16)p_assist->referenceTime.gpsTime.gpsWeek );
    p_supl_session_status->z_set_assist_pos_time_stamp.m.refTimeWeekNumValid = TRUE;

    q_GpsTow = (uint32)p_assist->referenceTime.gpsTime.gpsTOW23b;
    q_GpsTow *= 80; /* gpsTow23b LSB = 0.08 sec */

    /* 23 bit Gps Time of Week converted to full milli-seconds of the week */
    tm_umts_common_utils_GpsMsecWeekLimit(
       (int32 *)&q_GpsTow,
       &p_supl_session_status->z_set_assist_pos_time_stamp.w_ref_time_week_num );

    p_supl_session_status->z_set_assist_pos_time_stamp.t_pos_time_stamp_ms =
        tm_umts_common_utils_full_gps_ms(
            p_supl_session_status->z_set_assist_pos_time_stamp.w_ref_time_week_num,
            q_GpsTow);

    p_supl_session_status->z_set_assist_pos_time_stamp.m.posTimeStampValid = TRUE;

    tm_rrlp_reftime_to_TmCore_post_data_payload( p_assist, &z_tm_post_data_payload);

    /* if ref time availabe, use that to set eph. week number, facilitating the process that
       otherwise can be performed by PE */
    memscpy( &reftime_data, sizeof(reftime_data),
             &z_tm_post_data_payload.reftime_data, sizeof( z_tm_post_data_payload.reftime_data ) );

    if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                        p_supl_session_status->tm_active_session_handle,
                        TM_POST_DATA_TYPE_REFTIME,
                        &z_tm_post_data_payload
                      ) == FALSE
       )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: refT posted to TmCore", 0, 0, 0);
    }


    if ( p_assist->referenceTime.m.gsmTimePresent )
    {
      /* Not supported */
    }

    if ( p_assist->referenceTime.m.gpsTowAssistPresent )
    {
      #if 0   /*** QQQ-PM: need to discuss how to handle sparse pattern match **/  /* ayt - ignore */

      lsm_raw_tow_type *p_zTow = &p_zSmlcAssist->z_SmlcTow[0];
      struct rrlp_GPSTOWAssist *p_rrlp_tow = p_assist->referenceTime.gpsTowAssist;

      /* Initialize TOW array */
      memset( p_zSmlcAssist->z_SmlcTow, 0, sizeof( p_zSmlcAssist->z_SmlcTow ) );

      while ( p_rrlp_tow != NULL )
      {
        /* SvId */
        p_zTow->w_SvId = (U8)p_rrlp_tow->value.satelliteID + 1;

        /* Tlm word */
        p_zTow->w_TlmWord = (U16)p_rrlp_tow->value.tlmWord;

        /* AntiSpoof */
        p_zTow->u_AntiSpoofFlag = (U8)p_rrlp_tow->value.antiSpoof;

        /* Alert */
        p_zTow->u_AlertFlag = (U8)p_rrlp_tow->value.alert;

        /* TlmRsvBits */
        p_zTow->u_TlmRsvBits = (U8)p_rrlp_tow->value.tlmRsvdBits;

        /* Update TOW pointer to next element in the list */
        p_zTow++;
        p_rrlp_tow = p_rrlp_tow->next;
      }

      #endif

    } /* close if-GpsToW-Assist-available */

  } /* close if-refTime-available */


  /************** REFERENCE LOCATION **********************************/

  if ( p_assist->m.refLocationPresent )
  {

    if (  tm_rrlp_refpos_to_TmCore_post_data_payload
            ( p_assist,
              NULL,
              &z_tm_post_data_payload,
              (uint8 *)&z_last_refloc_info.z_RefPos_GAD.gad_ellip_alt_unc_ellip,
              FALSE
            )
       )
    {

      uint32                q_time_stamp_sec = 0;
      pdsm_pd_event_type    q_pd_event; /* pdsm_pd_event_type is uint32 */

      if ( p_supl_session_status->z_set_assist_pos_time_stamp.m.refTimeWeekNumValid )
      {
        /* seconds since GPS birth */
        q_time_stamp_sec =  (uint32) (p_supl_session_status->z_set_assist_pos_time_stamp.t_pos_time_stamp_ms / SEC_MSECS);
      }

      (void)tm_umts_common_utils_gad_to_pdsm_pd_info(
        p_supl_session_status->q_pdapi_client_id,
        (uint8 *)&z_last_refloc_info.z_RefPos_GAD.gad_ellip_alt_unc_ellip,
        PDSM_PD_POSITION_TYPE_NEW,
        PDSM_PD_POSITION_MODE_UNKNOWN,
        PDSM_PD_POSITION_SOURCE_DEFAULT,
        q_time_stamp_sec,
        &z_supl_pd_info_buffer,
        &q_pd_event
      );

      tm_post_ext_status_ref_loc_rep_event(
        TM_PRTL_TYPE_UMTS_UP,
        p_supl_session_status->tm_active_session_handle,
        &z_supl_pd_info_buffer.pd_info.pd_data);

      if ( q_handovers_since_last_cell_id )
      {

       /* Increase the Reference Position uncertainty by # of cell changes
          times maximum cell diameter. NV item aagps_default_ref_position_unc
          represents the radius of the cell */

       z_tm_post_data_payload.seed_data.f_LocUncrtntySemiMajor +=
         (float)( q_handovers_since_last_cell_id *
                  z_umts_nv_item_ram_copy.aagps_default_ref_position_unc * 2 );

       z_tm_post_data_payload.seed_data.f_LocUncrtntySemiMinor +=
         (float) ( q_handovers_since_last_cell_id *
                   z_umts_nv_item_ram_copy.aagps_default_ref_position_unc * 2 );

      }


      if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                          p_supl_session_status->tm_active_session_handle,
                          TM_POST_DATA_TYPE_SEED,
                          &z_tm_post_data_payload
                        ) == FALSE
         )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: refL posted to TmCore", 0, 0, 0);

        /* this is for storing the reference position in the original GAD shape
           format, in case there is a need later to report this back to the
           network */
        z_last_refloc_info.z_RefPos_GAD.u_pos_valid = TRUE;
        if ( p_assist->m.referenceTimePresent )
        {
          z_last_refloc_info.z_RefPos_GAD.w_week =
          p_supl_session_status->z_set_assist_pos_time_stamp.w_ref_time_week_num;

          /* 23 bit Gps Time of Week : q_GpsTow23b LSB = 0.08 sec */
          z_last_refloc_info.z_RefPos_GAD.q_tow_ms =
           (uint32)p_assist->referenceTime.gpsTime.gpsTOW23b * 80;

          z_last_refloc_info.z_RefPos_GAD.u_timestamp_valid = TRUE;
        }

        /* associate reference position with cell info. */
        memscpy( &z_last_refloc_info.z_cell_info_refloc,
                 sizeof(z_last_refloc_info.z_cell_info_refloc),
                &z_cell_info_cached,
                sizeof (z_cell_info_cached) );
      }
    }

  } /* close "if-refLoc-available */


  /*********  DGPS *********************************************/

  if ( p_assist->m.dgpsCorrectionsPresent )
  {
    /* Not supported, ignored */
  }


  /************** Nav. Model **********************************/

  if ( p_assist->m.navigationModelPresent )
  {

    rrlp_dec_SeqOfNavModelElement  *navModelList;
    rrlp_dec_UncompressedEphemeris *p_Ephem;
    rrlp_dec_NavModelElement       *p_navModelList_elem;

    uint16 w_week; /* QQQ: for now we seem need the week number */
    uint8 u_i;

    uint32 q_eph_svs_injected = 0;

    navModelList = &p_assist->navigationModel.navModelList;
    p_navModelList_elem = navModelList->elem;
    for ( u_i=0; u_i < navModelList->n; u_i++ )
    {
      uint32 q_Word;

      /* SvId */
      q_Word = p_navModelList_elem->satelliteID + 1;

      z_tm_post_data_payload.eph_data.u_Sv = (uint8)q_Word; /* SV ID */

      /* SatStatus ChoiceTag - currently SMLC only supports
         newNavModelUC (2) */
      if ( p_navModelList_elem->satStatus.t == rrlp_oldSatelliteAndModel_chosen )
      {
        /* oldStatelliteAndModel: no encoded eph.
           values for this SV will follow */
        if ( u_i < ( navModelList->n - 1) )
          p_navModelList_elem++;
        continue;
      }

      if ( p_navModelList_elem->satStatus.t == rrlp_newSatelliteAndModelUC_chosen )
        p_Ephem = p_navModelList_elem->satStatus.u.newSatelliteAndModelUC;
      else if ( p_navModelList_elem->satStatus.t == rrlp_newNaviModelUC_chosen )
        p_Ephem = p_navModelList_elem->satStatus.u.newNaviModelUC;
      else
      {
        /* Unknown */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Unknown Sat Status",0,0,0);
        if ( u_i < ( navModelList->n - 1) )
          p_navModelList_elem++;
        continue;
      }

      if ( p_assist->m.referenceTimePresent )
      {
        w_week = tm_umts_common_utils_build_eph_week_from_ref_time ( p_Ephem->ephemToe,
                                                                     reftime_data.q_GpsMsec,
                                                                     reftime_data.w_GpsWeek );
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"no eph. week since there is no ref time", 0, 0, 0);
        w_week = 0;
      }

      p_supl_session_status->q_eph_delivered_sv_list |= 1L << p_navModelList_elem->satelliteID;

      tm_rrlp_eph_to_TmCore_post_data_payload( p_Ephem, &z_tm_post_data_payload, w_week);

      if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                          p_supl_session_status->tm_active_session_handle,
                          TM_POST_DATA_TYPE_EPH,
                          &z_tm_post_data_payload
                        ) == FALSE
         )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to TmCore eph SV %u post failed", q_Word, 0, 0);
      }
      else
      {
        q_eph_svs_injected |= 1L << p_navModelList_elem->satelliteID;
      }

      /* Go to next element in the list */
      if ( u_i < ( navModelList->n - 1) )
        p_navModelList_elem++;

    } /* for-loop for the eph. SVs in the message */

    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "SUPL: GPS eph. posted to TmCore 0x%lx, total SVs = %u",
          q_eph_svs_injected, navModelList->n );

  } /* close if-eph-available */

  /****************************** IONO ****************************************/

  if ( p_assist->m.ionosphericModelPresent )
  {
    tm_rrlp_Iono_to_TmCore_post_data_payload( p_assist, &z_tm_post_data_payload );

    if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                        p_supl_session_status->tm_active_session_handle,
                        TM_POST_DATA_TYPE_IONO,
                        &z_tm_post_data_payload
                      ) == FALSE
       )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: IONO posted to TmCore", 0, 0, 0);
    }
  }


  /****************************** UTC ****************************************/

  if ( p_assist->m.utcModelPresent )
  {
    tm_rrlp_UtcModel_to_TmCore_post_data_payload( p_assist, &z_tm_post_data_payload );

    if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                        p_supl_session_status->tm_active_session_handle,
                        TM_POST_DATA_TYPE_UTC_MODEL,
                        &z_tm_post_data_payload
                      ) == FALSE
       )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: UTC posted to TmCore", 0, 0, 0);
    }


    /* Convert to glonass time & set the z_set_assist_pos_time_stamp struct. Later used in injecting AA.
       Don't bother about return value. If conversion fails Glo steering won't be posted to TM. */

    if ( (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED) &&
         (FALSE == p_supl_session_status->z_set_assist_pos_time_stamp.m.glonassTimeValid)
        )
    {
      tm_util_convert_to_glonass_time ( p_assist->utcModel.utcDeltaTls,
                                        &p_supl_session_status->z_set_assist_pos_time_stamp);
    }


  }

  /******************* almanac ***************************************************/

  if ( p_assist->m.almanacPresent )
  {
    rrlp_dec_SeqOfAlmanacElement *almanacList = &p_assist->almanac.almanacList;
    rrlp_dec_AlmanacElement      *p_Almanac;
    uint16 w_AlmanacWeek;
    uint8  u_i;
    uint32 q_alm_svs_injected = 0;

    /* Almanac WNa: 8 LSBs of the full 10-bit week -- PE will solve the ambiguity  */
    w_AlmanacWeek = (int16)p_assist->almanac.alamanacWNa;

    /* start almanac list */
    p_Almanac = almanacList->elem;

    for ( u_i=0; u_i<almanacList->n; u_i++ )
    {
      uint16 w_SvId;

      w_SvId = (uint8)p_Almanac->satelliteID + 1; /* 6-bit unsigned */

      if ( w_SvId > N_SV )
      {
        if ( u_i < (almanacList->n - 1) )
          p_Almanac++;
        continue;
      }

      tm_rrlp_alm_to_TmCore_post_data_payload( p_Almanac, &z_tm_post_data_payload );
      z_tm_post_data_payload.alm_data.w_GpsWeek = w_AlmanacWeek;

      p_supl_session_status->q_alm_delivered_sv_list |= 1L << p_Almanac->satelliteID;

      if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                          p_supl_session_status->tm_active_session_handle,
                          TM_POST_DATA_TYPE_ALM,
                          &z_tm_post_data_payload
                        ) == FALSE
         )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to TmCore alm SV %u post failed", w_SvId, 0, 0);
      }
      else
      {
        q_alm_svs_injected |= 1L << p_Almanac->satelliteID;
      }

      /* Move to next element of the list */
      if ( u_i < (almanacList->n - 1) )
      {
        p_Almanac++;
      }

    } /* for-loop GPS alm SVs */

    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "SUPL: GPS alm. posted to TmCore 0x%lx, total SVs = %u",
          q_alm_svs_injected, almanacList->n);

  }

  /**************** acquisition assistance **********************/

  if ( p_assist->m.acquisAssistPresent )
  {
    if ( p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED )
    {

      gps_RefTimeStructType  reftime_data1;

      memset(&reftime_data1, 0, sizeof( gps_RefTimeStructType ) );
      reftime_data1.w_GpsWeek = C_GPS_WEEK_UNKNOWN;

      if(b_pos_req_present == TRUE)
      {      
        tm_umts_up_supl_inform_core_nt_assist_end(p_supl_session_status); /* QQQ: need to consider what if ref time is coming in a later RRLP */
        /* receiver is now IDLE */
      }

      if ( p_assist->m.referenceTimePresent )
      {
        memscpy( &reftime_data1, sizeof(reftime_data1), &z_tm_post_data_payload.reftime_data,
                 sizeof( z_tm_post_data_payload.reftime_data ) );
      }

      /* inject SV directions */
      tm_rrlp_AcqAssistSvDir_to_TmCore_post_data_payload( p_assist, &z_tm_post_data_payload );

      if ( q_handovers_since_last_cell_id != 0 )
      {
        uint8 u_i;

        /* Widen the Acq assistance windows by a factor of
           q_HandoversSinceLastCellID to account for the fact that the Acq
           Assistance was generated for a cell that the phone is no longer in */

        for ( u_i=0; u_i < z_tm_post_data_payload.gnss_aa_data.u_NumSvs; u_i++ )
        {
          z_tm_post_data_payload.gnss_aa_data.z_SvSteerInfo[u_i].f_SvTimeUnc *=
            q_handovers_since_last_cell_id + 1;
        }

      }

      if ( z_tm_post_data_payload.gnss_sv_dir.u_NumSvs ) /* Optional AdditionalAngle */
      {
        if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                            p_supl_session_status->tm_active_session_handle,
                            TM_POST_DATA_TYPE_SV_DIR,
                            &z_tm_post_data_payload
                          ) == FALSE
           )
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
        }
        else
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: SvDir posted to TmCore", 0, 0, 0);
        }
      }


      /* inject steering */
      tm_rrlp_AcqAssistSteering_to_TmCore_post_data_payload( p_assist, &z_tm_post_data_payload, reftime_data1.w_GpsWeek );
      if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                          p_supl_session_status->tm_active_session_handle,
                          TM_POST_DATA_TYPE_AA,
                          &z_tm_post_data_payload
                        ) == FALSE
         )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: Steering posted to TmCore", 0, 0, 0);
      }

    } /* close if-SET-Assisted */

  } /* close if-acq-assist-available */

  /**************** Real Time Integrity ************************/


  if ( p_assist->m.realTimeIntegrityPresent )
  {
    rrlp_dec_SeqOf_BadSatelliteSet *p_RTI = &p_assist->realTimeIntegrity;
    rrlp_dec_SatelliteID           *p_SatelliteID;

    uint32 q_RealTimeIntegrity0 = 0;
    uint32 q_RealTimeIntegrity1 = 0;
    uint32 u_i;

    p_SatelliteID = p_RTI->elem;

    for ( u_i=0; u_i<p_RTI->n; u_i++ )
    {
      uint32 q_Word;

      q_Word = *p_SatelliteID + 1;

      if (q_Word <= 32)
      {
        q_RealTimeIntegrity0 |= 1 << (q_Word-1);
      }
      else
      {
        q_RealTimeIntegrity1 |= 1 << (q_Word-33);
      }

      /* Move to next element of the list */
      if ( u_i < (p_RTI->n - 1) )
      {
        p_SatelliteID++;
      }
    }

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: Received RTI informationi %d", q_RealTimeIntegrity0,0,0);

    /* Determine if we have enough info. about some SVs' health and
       inform MGP about such info., if available */
    tm_util_post_unhealthy_svs ( TM_PRTL_TYPE_UMTS_UP,
                                p_supl_session_status->tm_active_session_handle,
                                q_RealTimeIntegrity0);
  }


  return ( TRUE );
}

/*
 ******************************************************************************
 *
 * supl_rrlp_bds_assist_proc
 *
 * Function description:
 *  This function processes the OSS ASN.1 decoded RRLP Ganss Generic Assistance
 *  Data element, when the ganss ID is set to BDS. The decoded BDS data is 
 *  posted to TM.
 *
 * Parameters:
 *
 *  p_supl_session_status: Global SUPL State.
 *  p_tm_post_data_payload: Pointer to data structure to post BDS data to TM.
 *  p_ganss_generic_assist_data_elem: Generis assistance data element.
 *
 * Return value: TRUE if successful, False otherwise
 *
 ******************************************************************************
*/
static boolean supl_rrlp_bds_assist_proc
( 
  supl_status_s_type          *p_supl_session_status,
  tm_post_data_payload_type   *p_tm_post_data_payload,
  rrlp_dec_GANSSGenericAssistDataElement  *p_ganss_generic_assist_data_elem,
  rrlp_dec_GANSSReferenceTime *p_ganss_ref_time
)
{
#ifdef FEATURE_ABDS_RRLP_SUPL
  uint16 w_bds_week = 0; /* Initialize to invalid value */

  if((p_supl_session_status == NULL) || 
     (p_ganss_generic_assist_data_elem == NULL) || 
     (p_tm_post_data_payload == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"NULL pointers! 0x%x 0x%x 0x%x", 
          p_supl_session_status, p_ganss_generic_assist_data_elem, 
          p_tm_post_data_payload);
    return FALSE;
  }

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,"supl_rrlp_bds_assist_proc");

  if((p_ganss_ref_time != NULL) &&
     (p_ganss_ref_time->ganssRefTimeInfo.m.ganssTimeIDPresent != 0x0) &&
     (p_ganss_ref_time->ganssRefTimeInfo.m.ganssDayPresent    != 0x0) &&
     (p_ganss_ref_time->ganssRefTimeInfo.ganssTimeID == T_rrlp_GANSSRefTime_ID_BDS))
  {
    /* Unambiguous BDS week number, convert ganss day to ganss week */
    w_bds_week = p_ganss_ref_time->ganssRefTimeInfo.ganssDay / 7;
  }
    
  /* Process GANSS Time Model first!! */
  if (p_ganss_generic_assist_data_elem->m.ganssTimeModelPresent && 
      p_ganss_generic_assist_data_elem->ganssTimeModel.n > 0 )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL Posting BDS Time Model!");
    tm_rrlp_bds_time_model_post(
      TM_PRTL_TYPE_UMTS_UP,
      p_supl_session_status->tm_active_session_handle,
      &p_ganss_generic_assist_data_elem->ganssTimeModel,
      p_tm_post_data_payload, &w_bds_week, p_ganss_ref_time);
  }

  /* Process GANSS RTI next. Otherwise we could inject eph for bad SVs and PE 
   * would not know! */
  if ( p_ganss_generic_assist_data_elem->m.ganssRealTimeIntegrityPresent == 1 )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL Posting BDS RTI!");
    tm_rrlp_bds_rti_post(
      TM_PRTL_TYPE_UMTS_UP,
      p_supl_session_status->tm_active_session_handle,
      &p_ganss_generic_assist_data_elem->ganssRealTimeIntegrity,
      p_tm_post_data_payload);
  }
   
  if ( p_ganss_generic_assist_data_elem->m.ganssNavigationModelPresent == 1 )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL Posting BDS NavModel!");
    tm_rrlp_bds_eph_post(
      TM_PRTL_TYPE_UMTS_UP,
      p_supl_session_status->tm_active_session_handle,
      &p_ganss_generic_assist_data_elem->ganssNavigationModel,
      p_tm_post_data_payload, 
      w_bds_week);
  }

  if ( p_ganss_generic_assist_data_elem->m.ganssAlmanacModelPresent == 1 )
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL Posting BDS Alm!");
    tm_rrlp_bds_alm_post(
      TM_PRTL_TYPE_UMTS_UP,
      p_supl_session_status->tm_active_session_handle,
      &p_ganss_generic_assist_data_elem->ganssAlmanacModel,
      p_tm_post_data_payload);	  
  }

#if 0
  if (p_ganss_generic_assist_data_elem->m.bdsGridModel_r12Present == 1) 
  {
    tm_rrlp_bds_grid_iono_model_post(
      TM_PRTL_TYPE_UMTS_UP,
      p_supl_session_status->tm_active_session_handle,
      p_ganss_generic_assist_data_elem->bdsGridModel_r12,
      p_tm_post_data_payload);	  
  }
#endif /* 0 */
#endif /* FEATURE_ABDS_RRLP_SUPL */

  return TRUE;
}

/*
 ******************************************************************************
 *
 * supl_rrlp_glo_assist_proc
 *
 * Function description:
 *  This function processes the OSS ASN.1 decoded RRLP Ganss Generic Assistance
 *  Data element, when the ganss ID is set to Glonass. The decoded Glo data is 
 *  posted to TM.
 *
 * Parameters:
 *
 *  p_supl_session_status: Global SUPL State.
 *  p_tm_post_data_payload: Pointer to data structure to post BDS data to TM.
 *  p_ganss_generic_assist_data_elem: Generis assistance data element.
 *
 * Return value: TRUE if successful, False otherwise
 *
 ******************************************************************************
*/
static void supl_rrlp_glo_assist_proc
( 
  supl_status_s_type          *p_supl_session_status,
  tm_post_data_payload_type   *p_tm_post_data_payload,
  rrlp_dec_GANSSGenericAssistDataElement  *p_ganss_generic_assist_data_elem
)
{
  struct rrlp_dec_SeqOfGANSSSatelliteElement  *p_ganss_sat_list = NULL;
  rrlp_dec_GANSSSatelliteElement *p_ganss_sat_element = NULL;

  if((p_supl_session_status == NULL) || 
     (p_ganss_generic_assist_data_elem == NULL) || 
     (p_tm_post_data_payload == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"NULL pointers! 0x%x 0x%x 0x%x", 
          p_supl_session_status, p_ganss_generic_assist_data_elem, 
          p_tm_post_data_payload);
    return;
  }

  if (p_ganss_generic_assist_data_elem->m.ganssTimeModelPresent && p_ganss_generic_assist_data_elem->ganssTimeModel.n > 0 )
  {
    uint8 u_i = 0;
    rrlp_dec_GANSSTimeModelElement *p_ganss_time_model;

    p_supl_session_status->z_OrigWishList.z_GloAssistData.q_WishListMask &= ~C_GNSS_ASSIST_TIME_MODELS;

    p_ganss_time_model = p_ganss_generic_assist_data_elem->ganssTimeModel.elem;
    for ( u_i=0; u_i < p_ganss_generic_assist_data_elem->ganssTimeModel.n; u_i++ )
    {
      /* interested only in GPS gnss-TO-ID time */
      if ( p_ganss_time_model != NULL )
      {
        if ( p_ganss_time_model->gnssTOID == C_RRLP_GNSS_TO_ID_GPS )
        {
          p_tm_post_data_payload->gnss_time_model_data.l_A0 = p_ganss_time_model->tA0;
          p_tm_post_data_payload->gnss_time_model_data.q_GanssTimeModelRefTime = p_ganss_time_model->ganssTimeModelRefTime;
          p_tm_post_data_payload->gnss_time_model_data.u_GnssToID = C_GNSS_TO_ID_GPS;

          p_tm_post_data_payload->gnss_time_model_data.w_DeltaT = C_GNSS_GANSST_DELTAT_UNKNOWN; /* until the MGP API proivdes a different way to identify, use 0xFFFF for NA */
          p_tm_post_data_payload->gnss_time_model_data.x_A1 = C_GNSS_GANSST_A1_UNKNOWN; /* until the MGP API proivdes a different way to identify, use 0x8000 for NA */
          p_tm_post_data_payload->gnss_time_model_data.x_A2 = C_GNSS_GANSST_A2_UNKNOWN; /* until the MGP API proivdes a different way to identify, use 0x8000 for NA */
          p_tm_post_data_payload->gnss_time_model_data.w_WeekNo = C_GNSS_GANSST_WEEK_UNKNOWN;  /* until the MGP API proivdes a different way to identify, use 0xFFFF for NA */

          if (TRUE == p_ganss_time_model->m.weekNumberPresent)
          {
            p_tm_post_data_payload->gnss_time_model_data.w_WeekNo = p_ganss_time_model->weekNumber;
          }
          if (TRUE == p_ganss_time_model->m.tA1Present)
          {
            p_tm_post_data_payload->gnss_time_model_data.x_A1 = p_ganss_time_model->tA1;
          }
          if (TRUE == p_ganss_time_model->m.tA2Present)
          {
            p_tm_post_data_payload->gnss_time_model_data.x_A2 = p_ganss_time_model->tA2;
          }

          if (FALSE == tm_post_data(TM_PRTL_TYPE_UMTS_UP,
                                    p_supl_session_status->tm_active_session_handle,
                                    TM_POST_DATA_TYPE_TIME_MODEL_GLO,
                                    p_tm_post_data_payload
                                    ))
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"TM_RRLP_UP: SUPL(Glo) TimeModel to Tmcore data post failed", 0, 0, 0);
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_RRLP_UP: SUPL(Glo): TimeModel posted to TmCore", 0, 0, 0);
          }

        } /* ganss TOID = GPS */
      } /* pointer not NULL */
      p_ganss_time_model++;
    } /* for loop going through n members of rrlp_GANSSTimeModelElement */
  } /* ganssTime model present and n>0 */


  if ( p_ganss_generic_assist_data_elem->m.ganssNavigationModelPresent == 1 )
  {
    uint8  u_sat_i = 0, u_i = 0;
    uint32 q_eph_svs_injected = 0;
    uint32 q_eph_svs_not_injected = 0;
    glo_EphStructType    *p_eph_buffer_per_sv   = NULL;

    p_ganss_sat_list =
      &p_ganss_generic_assist_data_elem->ganssNavigationModel.ganssSatelliteList;

    p_ganss_sat_element = p_ganss_sat_list->elem;

    for (u_sat_i = 0; u_sat_i < p_ganss_sat_list->n; u_sat_i++, p_ganss_sat_element++)
    {
      if ( p_ganss_sat_element->svID >= N_GLO_SV )
      {
        /* should not happen */
        continue;
      }
      p_eph_buffer_per_sv = &p_supl_session_status->z_prot_glo_eph_buffer[p_ganss_sat_element->svID].z_eph_glo_data;

      supl_glo_eph_build(p_ganss_sat_element, p_eph_buffer_per_sv);

      if ( p_supl_session_status->z_prot_glo_eph_buffer[p_ganss_sat_element->svID].u_freq_num_valid
           == TRUE)
      {

        /* if this SV already has freq. number, inject it now;
           otherwise keep data in the buffer and inject when all
           assistance have been delivered */
        tm_rrlp_glo_eph_to_TmCore_post_data_payload
          ( &p_supl_session_status->z_prot_glo_eph_buffer[p_ganss_sat_element->svID],
            p_tm_post_data_payload );

        if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                            p_supl_session_status->tm_active_session_handle,
                            TM_POST_DATA_TYPE_EPH_GLO,
                            p_tm_post_data_payload
                          ) == FALSE
           )
        {
          /* Bound check for z_rrlp_glo_eph_buffer array */
          if (u_i < N_GLO_SV)
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Glo eph. SV %u inj. to TmCore failed",
                     p_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId, 0, 0);
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Glo eph. SV inj. to TmCore failed and array z_prot_glo_eph_buffer out of bounds with index of %d", u_i, 0, 0);
            continue;
          }
        }
        else
        {
          q_eph_svs_injected |= 1L << p_ganss_sat_element->svID;
        }

        /* once processed, remove it from the buffer */
        /* 0 would be an invalid number because the api uses 65 and up for Glonass */
        p_supl_session_status->z_prot_glo_eph_buffer[p_ganss_sat_element->svID].z_eph_glo_data.u_SvId = 0;
        p_supl_session_status->z_prot_glo_eph_buffer[p_ganss_sat_element->svID].u_freq_num_valid = FALSE;

      } /* if this SV already has freq. number */
      else
      {
        q_eph_svs_not_injected |= 1L << p_ganss_sat_element->svID;
      }
    } /* for */

    if ( q_eph_svs_injected != 0 )
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "SUPL GLONASS eph posted to TmCore (eph. recv) 0x%lx, total SVs = %u",
            q_eph_svs_injected, p_ganss_sat_list->n);
    }
    if ( q_eph_svs_not_injected != 0 )
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "SUPL GLONASS eph pending posting to TmCore (eph. recv) 0x%lx, total SVs = %u",
            q_eph_svs_not_injected, p_ganss_sat_list->n);
    }

  } /* ganssNavModel present */

  if ( p_ganss_generic_assist_data_elem->m.ganssAlmanacModelPresent == 1 )
  {
    struct rrlp_dec_SeqOfGANSSAlmanacElement *p_ganss_alm_list =
      &p_ganss_generic_assist_data_elem->ganssAlmanacModel.ganssAlmanacList;

    rrlp_dec_GANSSAlmanacElement *p_ganss_almanac = p_ganss_alm_list->elem;
    uint16 w_AlmanacWeek;
    uint8  u_alm_i;
    uint32 q_alm_svs_injected = 0;

    /* ayt - how to post weekNumber to MGP ? */
    /* Almanac WNa: 8 LSBs of the full 10-bit week -- PE will solve the ambiguity  */
    w_AlmanacWeek = (int16)p_ganss_generic_assist_data_elem->ganssAlmanacModel.weekNumber;

    /* start almanac list */
    for (u_alm_i = 0; u_alm_i < p_ganss_alm_list->n; u_alm_i++, p_ganss_almanac++ )
    {
      if ( p_ganss_almanac->t == T_rrlp_dec_GANSSAlmanacElement_keplerianGLONASS )
      {
        supl_glo_eph_svid_freq_num_map_alm(
            p_ganss_almanac->u.keplerianGLONASS, 
            p_supl_session_status->z_prot_glo_eph_buffer, 
            sizeof( p_supl_session_status->z_prot_glo_eph_buffer) / 
            sizeof( p_supl_session_status->z_prot_glo_eph_buffer[0]));

        tm_rrlp_glo_alm_to_TmCore_post_data_payload
          ( p_ganss_almanac->u.keplerianGLONASS, p_tm_post_data_payload );
        p_tm_post_data_payload->alm_data.w_GpsWeek = w_AlmanacWeek;

        if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                            p_supl_session_status->tm_active_session_handle,
                            TM_POST_DATA_TYPE_ALM_GLO,
                            p_tm_post_data_payload
                          ) == FALSE
           )
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
        }
        else
        {
          q_alm_svs_injected |= 1L << ( p_ganss_almanac->u.keplerianGLONASS->gloAlmnA - 1 );
        }

      } /* if Model 5 ( GLONASS ) */
    } /* for-loop for ganssAlmanac list */

    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "SUPL GLONASS alm. posted to TmCore 0x%lx, total SVs = %u",
          q_alm_svs_injected, p_ganss_alm_list->n);

  } /* ganssAlmanac present */

  if ( ( p_ganss_generic_assist_data_elem->m.ganssAddUTCModelPresent == 1) &&
       ( p_ganss_generic_assist_data_elem->ganssAddUTCModel.t == T_rrlp_GANSSAddUTCModel_utcModel3 )
     )
  {
    tm_rrlp_glo_addutc_to_TmCore_post_data_payload
          ( p_ganss_generic_assist_data_elem, p_tm_post_data_payload );

    if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                        p_supl_session_status->tm_active_session_handle,
                        TM_POST_DATA_TYPE_UTC_MODEL_GLO,
                        p_tm_post_data_payload
                      ) == FALSE
       )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: GANSS Additional UTC posted to TmCore", 0, 0, 0);
    }
  } /* ganssAddUTC Model 3 (GLONASS) present */


  if  ( p_ganss_generic_assist_data_elem->m.ganssRefMeasurementAssistPresent )
  {

    if ( p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED )
    {

      if ( ( p_ganss_generic_assist_data_elem->ganssRefMeasurementAssist.ganssSignalID == GANSS_SIGNAL_ID_GLONASS_G1 ) &&
           ( p_ganss_generic_assist_data_elem->ganssRefMeasurementAssist.ganssRefMeasAssistList.elem != NULL ) &&
           ( p_ganss_generic_assist_data_elem->ganssRefMeasurementAssist.ganssRefMeasAssistList.n > 0 )
         )
      {
        /* Build SV directions (Azim & Elevation) & Steering information if Glonass time is available.
           Else take care of Glonass time & freqNum mapping later when GPS TimeModel & AuxiInfo AD is received */

       if (p_supl_session_status->z_set_assist_pos_time_stamp.m.glonassTimeValid)
       {
         tm_rrlp_glo_acq_assist_to_tmcore_post_data_payload (
             &p_ganss_generic_assist_data_elem->ganssRefMeasurementAssist.ganssRefMeasAssistList,
             &p_supl_session_status->z_prot_glo_acqasst_buffer,
             p_supl_session_status->z_set_assist_pos_time_stamp.z_GlonassTime.q_GloMsec);
       }
       else
       {
         MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "No Glonass time at present, preserve AA data");

         /* Store AA data */
       }

       /* Pseudo segmented AA data? Correlate SvId-to-FreqNum if AA sent in a separate transaction
            (different from AuxiInfo) */

      }  /* refMeasurement data valid */
    } /* is MSA mode */
  }  /* reference measurement assistance present */

  if ( ( p_ganss_generic_assist_data_elem->m.ganssAuxiliaryInfoPresent == 1) &&
       ( p_ganss_generic_assist_data_elem->ganssAuxiliaryInfo.t == T_rrlp_GANSSAuxiliaryInformation_ganssID3 )
     )
  {
    uint8  u_i = 0;
    uint8  u_num_eph_injected = 0;
    uint32 q_eph_injected_svs = 0;
    uint8  b_buf_sz = sizeof(p_supl_session_status->z_prot_glo_eph_buffer) / 
                      sizeof(p_supl_session_status->z_prot_glo_eph_buffer[0]);

    supl_glo_eph_svid_freq_num_map_aux ( 
        p_ganss_generic_assist_data_elem->ganssAuxiliaryInfo.u.ganssID3, 
        p_supl_session_status->z_prot_glo_eph_buffer, b_buf_sz);

    /* inject any Glonass SV eph. that didn't have freq.Num / svID association before */
    for ( u_i =0; u_i < N_GLO_SV; u_i++)
    {

      if ( ( p_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId > GLO_SLOTID_OFFSET ) &&
           ( p_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId < (N_GLO_SV+GLO_SLOTID_OFFSET+1) ) &&
           ( p_supl_session_status->z_prot_glo_eph_buffer[u_i].u_freq_num_valid == TRUE )
         )
      {
        /* inject this SV eph */
        tm_rrlp_glo_eph_to_TmCore_post_data_payload
          ( &p_supl_session_status->z_prot_glo_eph_buffer[u_i],
            p_tm_post_data_payload );

        if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                            p_supl_session_status->tm_active_session_handle,
                            TM_POST_DATA_TYPE_EPH_GLO,
                            p_tm_post_data_payload
                          ) == FALSE
           )
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Glo eph. SV %u inj. to TmCore failed",
                   p_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId, 0, 0);
        }
        else
        {
          u_num_eph_injected++;
          q_eph_injected_svs |=
            1L << ( p_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId - GLO_SLOTID_OFFSET - 1 );
        }
        /* reset these flags so that they won't get injected again later */
        p_supl_session_status->z_prot_glo_eph_buffer[u_i].z_eph_glo_data.u_SvId = 0;
        p_supl_session_status->z_prot_glo_eph_buffer[u_i].u_freq_num_valid = FALSE;
      }

    }

    if ( u_num_eph_injected !=0 )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL GLONASS eph posted to TmCore (when getting aux) 0x%lx, totoal SVs = %u",
              q_eph_injected_svs, u_num_eph_injected, 0 );
    }

  } /* rrlp_ganssAuxiliaryInfo for GLONASS present */
  else
  {
     MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"GanssAux not present or not for GLONASS: %u, %u",
              p_ganss_generic_assist_data_elem->m.ganssAuxiliaryInfoPresent,
              p_ganss_generic_assist_data_elem->ganssAuxiliaryInfo.t, 0);
  }

  /***** Inject Glonass SV directions **** */
  /* RefMeasurement is only usable for current MSA session:  match SV_ID with freq. number   */
  if ( p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED )
  {

    if ( ( p_ganss_generic_assist_data_elem->m.ganssAuxiliaryInfoPresent == 1) &&
         ( p_ganss_generic_assist_data_elem->ganssAuxiliaryInfo.t == T_rrlp_GANSSAuxiliaryInformation_ganssID3 )
       )
    {
      supl_glo_acqasst_svid_freq_num_map_aux (p_ganss_generic_assist_data_elem->ganssAuxiliaryInfo.u.ganssID3,
                                              &p_supl_session_status->z_prot_glo_acqasst_buffer);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"No valid aux. assistance available: %u,  %u",
               p_ganss_generic_assist_data_elem->m.ganssAuxiliaryInfoPresent,
               p_ganss_generic_assist_data_elem->ganssAuxiliaryInfo.t, 0);
    }

    if (TRUE == p_supl_session_status->z_prot_glo_acqasst_buffer.u_freq_num_valid)
    {
      if ((p_supl_session_status->z_prot_glo_acqasst_buffer.gnss_sv_dir.u_NumSvs > 0) &&
          (p_supl_session_status->z_prot_glo_acqasst_buffer.gnss_sv_dir.u_NumSvs <= N_GLO_SV))
      {
        // Copy SV direction into TM post struct
        memscpy(&p_tm_post_data_payload->gnss_sv_dir,
               sizeof(p_tm_post_data_payload->gnss_sv_dir),
               &p_supl_session_status->z_prot_glo_acqasst_buffer.gnss_sv_dir,
               sizeof(p_supl_session_status->z_prot_glo_acqasst_buffer.gnss_sv_dir));

        if (FALSE == tm_post_data(TM_PRTL_TYPE_UMTS_UP,
                                  p_supl_session_status->tm_active_session_handle,
                                  TM_POST_DATA_TYPE_SV_DIR_GLO,
                                  p_tm_post_data_payload
                                  ))
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL RRLP(Glo): AA SV Dir (total %u SVs) Tmcore data post failed during aux. proc",
                    p_supl_session_status->z_prot_glo_acqasst_buffer.gnss_sv_dir.u_NumSvs, 0, 0);
        }
        else
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL RRLP(Glo): AA SV Dir (total %u SVs) posted to Tmcore during aux. proc",
                   p_supl_session_status->z_prot_glo_acqasst_buffer.gnss_sv_dir.u_NumSvs, 0, 0);
        }
      }  /* if SV direction assistance available and valid */

      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL RRLP(Glo): AA SV Dir (%u) aren't qualified to post during aux. proc",
                 p_supl_session_status->z_prot_glo_acqasst_buffer.gnss_sv_dir.u_NumSvs, 0, 0);
      }

      /***** Inject Glonass Steering data if there are SVs for this data yet to be injected *****/
      if ((p_supl_session_status->z_prot_glo_acqasst_buffer.z_steering_glo_data.u_NumSvs > 0) &&
          (p_supl_session_status->z_prot_glo_acqasst_buffer.z_steering_glo_data.u_NumSvs <= N_GLO_SV))
      {
        // Copy over the Glonass time to steering struct
        if (p_supl_session_status->z_set_assist_pos_time_stamp.m.glonassTimeValid)
        {
          /* Copy Steering into TM post struct */
          memscpy(&p_tm_post_data_payload->gnss_aa_data,
                 sizeof(p_tm_post_data_payload->gnss_aa_data),
                 &p_supl_session_status->z_prot_glo_acqasst_buffer.z_steering_glo_data,
                 sizeof(p_supl_session_status->z_prot_glo_acqasst_buffer.z_steering_glo_data));

          /* Fill Glonass time fields */
          p_tm_post_data_payload->gnss_aa_data.u_GloCycleValid =
             (p_supl_session_status->z_set_assist_pos_time_stamp.z_GlonassTime.u_FourYear != C_GLO_FOUR_YEAR_UNKNOWN);
          p_tm_post_data_payload->gnss_aa_data.u_GloCalDayValid =
             (p_supl_session_status->z_set_assist_pos_time_stamp.z_GlonassTime.w_Days != C_GLO_DAY_UNKNOWN);

          p_tm_post_data_payload->gnss_aa_data.u_GloNumCycles  = p_supl_session_status->z_set_assist_pos_time_stamp.z_GlonassTime.u_FourYear;
          p_tm_post_data_payload->gnss_aa_data.w_GloCalDay     = p_supl_session_status->z_set_assist_pos_time_stamp.z_GlonassTime.w_Days;
          p_tm_post_data_payload->gnss_aa_data.q_Msec          = p_supl_session_status->z_set_assist_pos_time_stamp.z_GlonassTime.q_GloMsec;
          p_tm_post_data_payload->gnss_aa_data.u_TimeRefIsGnss = TRUE;
          p_tm_post_data_payload->gnss_aa_data.q_SvSteerSrc    = MGP_GNSS_SV_STEER_SRC_GLO;

          MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "SUPL RRLP(Glo): AA-Steering to post with Glonass Time: %d, %d, %u, %u, %d, %lu",
                p_tm_post_data_payload->gnss_aa_data.u_GloCycleValid,
                p_tm_post_data_payload->gnss_aa_data.u_GloCalDayValid,
                p_tm_post_data_payload->gnss_aa_data.u_GloNumCycles,
                p_tm_post_data_payload->gnss_aa_data.w_GloCalDay,
                p_tm_post_data_payload->gnss_aa_data.q_Msec,
                p_tm_post_data_payload->gnss_aa_data.u_TimeRefIsGnss);

          /* Post Glonass AA data to TM. This would inturn ask LM to turn MGP receiver ON */
          if (FALSE == tm_post_data(TM_PRTL_TYPE_UMTS_UP,
                                    p_supl_session_status->tm_active_session_handle,
                                    TM_POST_DATA_TYPE_AA_GLO,
                                    p_tm_post_data_payload
                                    ))
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL RRLP(Glo): AA-Steering Tmcore data post failed", 0, 0, 0);
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL RRLP(Glo): AA-Steering posted to TmCore", 0, 0, 0);

            /* For ME to generate GLONASS measurements both SV Dir & Steering should be injected successful. */
            p_supl_session_status->u_glo_meas_status = SM_GNSS_GLO_AA_INJECTED;

            /* Update flag to inform TM sess continue to set SM_GNSS_LS_REQ_GLO_MEAS in the LM message */
            p_supl_session_status->u_glo_meas_status |= SM_GNSS_LS_REQ_GLO_MEAS;


            /* reset these time & freqNum flags so that they won't get injected again later */
            p_supl_session_status->z_set_assist_pos_time_stamp.m.glonassTimeValid = FALSE;
            p_supl_session_status->z_prot_glo_acqasst_buffer.u_freq_num_valid = FALSE;
          }
        } /* GLONASS time valid */
      } /* if GLONASS steering assitance yet to be injected */
    } /* if freq. number / SVID association established */
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Not injecting GLONASS ref. meas.: lack freq. svID association", 0, 0, 0);
    }

  } /* if MSA mode */
}

/*
 ******************************************************************************
 *
 * supl_rrlp_ganss_assist_proc
 *
 * Function description:
 *  This function processes the OSS ASN.1 decoded RRLP ganssAssistance data
 *  and prepares such assistance data for injecting to MGP.
 *
 * Parameters:
 *
 *
 *  p_assist: pointer to the OSS ASN.1 decoded RRLP ganssAsssitance data.
 *
 * Return value: TRUE/FALSE
 *
 *
 ******************************************************************************
*/

static boolean supl_rrlp_ganss_assist_proc
( 
  supl_status_s_type       *p_supl_session_status,
  rrlp_dec_GANSS_ControlHeader *p_assist 
)
{
  tm_post_data_payload_type    z_tm_post_data_payload;
  struct rrlp_dec_SeqOfGANSSGenericAssistDataElement *p_ganss_generic_assist_data_list = NULL;
  rrlp_dec_GANSSReferenceTime  *p_ganss_ref_time = NULL;

  if ( p_assist->m.ganssCommonAssistDataPresent == 1 )
  {

    /* In gannCommon, only reference time and reference location are used */

    #if 0  // MGP does not process GLO ref. time at this point
    if ( p_assist->ganssCommonAssistData.m.ganssReferenceTimePresent == 1 )
    {
      tm_rrlp_glo_reftime_to_TmCore_post_data_payload( p_assist, &z_tm_post_data_payload);

      if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                          p_supl_session_status->tm_active_session_handle,
                          TM_POST_DATA_TYPE_REFTIME_GLO,
                          &z_tm_post_data_payload
                        ) == FALSE
         )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: Glonass refT posted to TmCore", 0, 0, 0);

      }

    }
    #endif

    if ( p_assist->ganssCommonAssistData.m.ganssRefLocationPresent == 1 )
    {
      /* GANSS ref. location has the same structure as that of the GPS'. Use the
         same interface to pass ref. location to MGP */
      if( tm_rrlp_refpos_to_TmCore_post_data_payload (
                NULL,
                p_assist,
                &z_tm_post_data_payload,
                (uint8 *)&z_last_refloc_info.z_RefPos_GAD.gad_ellip_alt_unc_ellip,
                FALSE))
	  {
        if ( tm_post_data ( TM_PRTL_TYPE_UMTS_UP,
                            p_supl_session_status->tm_active_session_handle,
                            TM_POST_DATA_TYPE_SEED,
                            &z_tm_post_data_payload
                          ) == FALSE
           )
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL to Tmcore data post failed", 0, 0, 0);
        }
        else
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: Glonass refL posted to TmCore", 0, 0, 0);
        }
	  }
    } /* if ganssRefL present */

    if ( p_assist->ganssCommonAssistData.m.ganssReferenceTimePresent == 1 )
    {
      p_ganss_ref_time = &p_assist->ganssCommonAssistData.ganssReferenceTime;
      /* xlate GANSS ref. time and post to TM Core */
      if(!tm_rrlp_ganss_reftime_to_tmcore_post_data_payload (
              TM_PRTL_TYPE_UMTS_UP,
              p_supl_session_status->tm_active_session_handle,
              &p_assist->ganssCommonAssistData.ganssReferenceTime,
              &z_tm_post_data_payload))
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
            "SUPL Could not xlate GANSS RefTime!");
      }
    } /* if ganssRefL present */
    #ifdef FEATURE_ABDS_RRLP_SUPL
    if ( p_assist->ganssCommonAssistData.m.ganssAddIonosphericModelPresent == 1 )
    {
      /* GANSS ref. location has the same structure as that of the GPS'. Use the
         same interface to pass ref. location to MGP */
      tm_rrlp_bds_global_iono_post(
          TM_PRTL_TYPE_UMTS_UP,
          p_supl_session_status->tm_active_session_handle, 
          &p_assist->ganssCommonAssistData, 
          &z_tm_post_data_payload);
    } /* if ganssRefL present */
    #endif
  } /* if ganssCommonAssistData present */


  if ( p_assist->m.ganssGenericAssistDataListPresent == 1 )
  {
    uint8 u_i;
    rrlp_dec_GANSSGenericAssistDataElement  *p_ganss_generic_assist_data_elem = NULL;

    p_ganss_generic_assist_data_list = &p_assist->ganssGenericAssistDataList;
    p_ganss_generic_assist_data_elem = p_ganss_generic_assist_data_list->elem;

    for ( u_i = 0; u_i < p_ganss_generic_assist_data_list->n; u_i++, p_ganss_generic_assist_data_elem++)
    {
      if ( p_ganss_generic_assist_data_elem->m.ganssIDPresent != 1)
      {
        /* Ganss ID not present indicates Galileo. We dont support */
        continue;
      }

      /* only Glonass & BDS supported */
      if ( p_ganss_generic_assist_data_elem->ganssID == GANSS_ID_3_GLONASS )
      {
        /* ganssID indicates GLONASS */
        supl_rrlp_glo_assist_proc(
            p_supl_session_status,
            &z_tm_post_data_payload,
            p_ganss_generic_assist_data_elem );
      }
      else if ( p_ganss_generic_assist_data_elem->ganssID == GANSS_ID_4_BDS)
      {
        /* ganssID indicates BDS */
        supl_rrlp_bds_assist_proc(
            p_supl_session_status,
            &z_tm_post_data_payload,
            p_ganss_generic_assist_data_elem, 
            p_ganss_ref_time);
      }
      else /* ganssID absent (Gaelio); ganssID other than Glonass(3) -- neither supported */
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"Discard ganssAssist: ganssIdPresent=%u, ganssID=%u",
                 p_ganss_generic_assist_data_elem->m.ganssIDPresent,
                 p_ganss_generic_assist_data_elem->ganssID,
                 0 );
      }
    } /* for-loop:  ganssGenericAssistDataList */

  } /* ganssGenericAssistDataList present */


  return TRUE;
}

/*
 ******************************************************************************
 *
 * supl_rrlp_msr_pos_req_qos_proc
 *
 * Function description:
 *  This function processes the OSS ASN.1 decoded RRLP-msrPosReq's
 *  PositionInstruct part, passes QoS to LM if needed, and sets RRLP response
 *  timer.
 *
 * Parameters:
 *
 *  p_mpr: pointer to the OSS ASN.1 decoded RRLP-msrPosReq.
 *
 * Return value:  none.
 *
 *
 ******************************************************************************
*/
static void supl_rrlp_msr_pos_req_qos_proc
(
  supl_status_s_type   *p_supl_session_status, 
  rrlp_dec_MsrPosition_Req *p_mpr 
)
{
  rrlp_dec_PositionInstruct *p_rrlp_pos_instruction = &p_mpr->positionInstruct;

  /* Determine whether the REL-7 responseTime is present */
  if ( ( p_mpr->m.rel7_MsrPosition_Req_extensionPresent ) &&
       ( p_mpr->rel7_MsrPosition_Req_extension.m.requiredResponseTimePresent )
     )
  {
    p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_measure_response_time_sec =
      p_mpr->rel7_MsrPosition_Req_extension.requiredResponseTime;
  }
  else
  {
    p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_measure_response_time_sec =
      (uint32)((uint32)1 << p_rrlp_pos_instruction->measureResponseTime);
  }

  if ( p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED )
  {

    /* let LM times out first, so if there is a previously disqualified position, it can be reported at LM timeout */
    if ( p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_UI )
    {
      /* for UI, if the pdapi provided QoS timer is longer than RRLP timer ( minus 2 sec. ), use the RRLP timer */
      if ( (p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec / 1000) >
             (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_measure_response_time_sec - 2)
         )
      {
        p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec =
          (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_measure_response_time_sec - 2) * 1000;
      }
    }
    else /* for NI, RRC timer (minus 2 sec.) is the only timer */
    {
      p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec =
        (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_measure_response_time_sec - 2 ) * 1000;
    }

    /* adopt the network provided accuracy threshold, if provided, though there should be no
       effect on MGP */

    /* the mode is set for msAssisted but the RRLP could have asked for either:
       msAssisted, msAssistedPref, or msBasedPref */
    if (  (p_rrlp_pos_instruction->methodType.t == rrlp_msAssisted_chosen ) &&
          ( p_rrlp_pos_instruction->methodType.u.msAssisted != NULL )
       )
    {
      if ( p_rrlp_pos_instruction->methodType.u.msAssisted->m.accuracyPresent )
      {
        p_supl_session_status->z_session_qos.q_hor_acc_meters =
        (uint32)tm_umts_common_utils_GADK_to_meter( TRUE, p_rrlp_pos_instruction->methodType.u.msAssisted->accuracy );
      }
    }
    else if (p_rrlp_pos_instruction->methodType.t == rrlp_msAssistedPref_chosen )
    {
      p_supl_session_status->z_session_qos.q_hor_acc_meters =
      (uint32)tm_umts_common_utils_GADK_to_meter( TRUE, p_rrlp_pos_instruction->methodType.u.msAssistedPref );
    }
    else if (p_rrlp_pos_instruction->methodType.t == rrlp_msBasedPref_chosen )
    {
      p_supl_session_status->z_session_qos.q_hor_acc_meters =
      (uint32)tm_umts_common_utils_GADK_to_meter( TRUE, p_rrlp_pos_instruction->methodType.u.msBased );
    }

    /* set the timer so that if no fix by its expiration, respond to the network with no-fix */
    supl_set_cf_state(C_SUPL_CF_STATE_SUPLPOS_XCHG, 0);

    /* inform TmCore of session-Continue, using QoS saved in p_supl_session_status->z_session_qos */
    tm_umts_up_supl_inform_core_nt_assist_end(p_supl_session_status);        
  } /* end if SET-Assisted */

  else if ( p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED )
  {
    if ( !p_supl_session_status->u_pdapi_fix_reported )
    {
      /* Note for SI-SB, RRLP is supposed to be of AssistanceData. However, if measurePosReq
         arrives, it will be processed here too, and the SLP provided QoP shall override
         pdapi's original QoS. */

      /* let LM times out first, so if there is a previously disqualified position, it can be reported at LM timeout */
      p_supl_session_status->z_session_qos.q_gps_session_timeout_value_msec =
        (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_measure_response_time_sec - 2) * 1000;

      /* adopt the network provided accuracy threshold */

      /* the mode is set for msBased the RRLP could have asked for either:
         msBased, msBasedPref, or msAssistedPref */

      if (p_rrlp_pos_instruction->methodType.t == rrlp_msBased_chosen )
      {
        p_supl_session_status->z_session_qos.q_hor_acc_meters =
        (uint32)tm_umts_common_utils_GADK_to_meter( TRUE, p_rrlp_pos_instruction->methodType.u.msBased );
      }
      else if (p_rrlp_pos_instruction->methodType.t == rrlp_msBasedPref_chosen )
      {
        p_supl_session_status->z_session_qos.q_hor_acc_meters =
        (uint32)tm_umts_common_utils_GADK_to_meter( TRUE, p_rrlp_pos_instruction->methodType.u.msBasedPref );
      }
      else if (p_rrlp_pos_instruction->methodType.t == rrlp_msAssistedPref_chosen )
      {
        p_supl_session_status->z_session_qos.q_hor_acc_meters =
        (uint32)tm_umts_common_utils_GADK_to_meter( TRUE, p_rrlp_pos_instruction->methodType.u.msAssistedPref );
      }


      /* set the timer so that if no fix by its expiration, respond to the network with no-fix. */
      supl_set_cf_state(C_SUPL_CF_STATE_SUPLPOS_XCHG, 0);

      /* inform TmCore of session-Continue, using QoS saved in p_supl_session_status->z_session_qos */
      tm_umts_up_supl_inform_core_nt_assist_end(p_supl_session_status);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SUPL: pos. already obtained, no action on RRLP-mspReq QoP", 0, 0, 0 );
    }

  } /* close else-if-msBased */

}


 /******************************************************************************
 *
 * supl_convert_and_send_pos_mode_to_pdapi
 *
 * Function description:
 *  This function sends a PDAPI event of the requested session type by the network
 *
 * Parameters:
 *
 *
 * Return value:
 *
 *
 ******************************************************************************
*/
void supl_convert_and_send_pos_mode_to_pdapi(supl_status_s_type *p_supl_session_status)
{
  
  uint32 session_type = 0;
  boolean b_gps_enabled = 
    (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.u_ganss_pos_methods_bitmap &
                              RRLP_GANSS_POS_METHOD_GPS) ? TRUE : FALSE; 
  
  boolean b_glo_enabled = 
    (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.u_ganss_pos_methods_bitmap &
                           RRLP_GANSS_POS_METHOD_GLONASS) ? TRUE : FALSE ;
  
  if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
  {
    if (b_gps_enabled)
    {
      session_type |= PDSM_PD_SESS_REQ_TYPE_MSB_GPS;
    }
    if (b_glo_enabled)
    {
      session_type |= PDSM_PD_SESS_REQ_TYPE_MSB_GLO;
    }
  }
  
  else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED)
  {
    if (b_gps_enabled)
    {
      session_type |= PDSM_PD_SESS_REQ_TYPE_MSA_GPS;
    }
    if (b_glo_enabled)
    {
      session_type |= PDSM_PD_SESS_REQ_TYPE_MSA_GLO;
    }
  }
  else if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_AUTO)
  {
    session_type |= PDSM_PD_SESS_REQ_TYPE_STANDALONE;
  }
  
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"AGPS Mode %d and GPS requested %d GLO requested %d,", p_supl_session_status->u_agps_mode, b_gps_enabled, b_glo_enabled); 
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"Requested sess type %d,", session_type);
  
  if ( 0 != session_type )
  {
    tm_send_sess_type_pdapi_event(session_type, PDSM_PD_COMM_PROTOCOL_UMTS_UP_SUPL);
  }
}
  

/*
 ******************************************************************************
 *
 * supl_rrlp_msr_pos_req_proc
 *
 * Function description:
 *  This function processes the OSS ASN.1 decoded RRLP-msrPosReq and prepares
 *  such data for SUPL usage.
 *
 * Parameters:
 *
 *  p_mpr: pointer to the OSS ASN.1 decoded RRLP-msrPosReq data.
 *
 * Return value:
 *
 *
 ******************************************************************************
*/
static boolean supl_rrlp_msr_pos_req_proc
(
  supl_status_s_type       *p_supl_session_status, 
  rrlp_dec_MsrPosition_Req *p_mpr 
)
{
  rrlp_dec_PositionInstruct *p_rrlp_pos_instruction = &p_mpr->positionInstruct;
  boolean u_ret = TRUE;


  if ( p_rrlp_pos_instruction->positionMethod == rrlp_dec_PositionMethod_eotd  )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"SUPL: rrlp-EOTD not supported", 0, 0, 0);
    if ( !tm_umts_up_supl_suplpos_rrlp_mpr_err_tx(
              p_supl_session_status, rrlp_methodNotSupported))
    {
      tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
      tm_umts_up_supl_session_init(FALSE);
    }
    return FALSE;
  }

  /* when RRLP measurePosReq is received, the "more-to-come" flag should
     be cleared */
  p_supl_session_status->z_network_assist.u_MoreToCome = FALSE;

  /**** SET-initiated: check consistency with regard to the gps mode */
  /* We set the GPS mode in SUPL status based on the user req and nv config */
  if  ( p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_UI )
  {

    if ( p_rrlp_pos_instruction->methodType.t == rrlp_msAssisted_chosen )
    {
      /** For SET-Initiated, check to see if the SLP still follows the previous
          agreement with the SET on methodType */
      if ( p_supl_session_status->u_agps_mode != C_SUPL_AGPS_MODE_UE_ASSISTED )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL: rrlp-msrPosReq methodType mismatch", 0, 0, 0);
        if ( !tm_umts_up_supl_suplpos_rrlp_mpr_err_tx(
                  p_supl_session_status, rrlp_methodNotSupported))
        {
          tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
          tm_umts_up_supl_session_init(FALSE);
        }
        return FALSE;
      }
    }
    else if ( p_rrlp_pos_instruction->methodType.t == rrlp_msBased_chosen )
    {
      if ( p_supl_session_status->u_agps_mode != C_SUPL_AGPS_MODE_UE_BASED )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL: rrlp-msrPosReq methodType mismatch", 0, 0, 0);
        if ( !tm_umts_up_supl_suplpos_rrlp_mpr_err_tx(
                  p_supl_session_status, rrlp_methodNotSupported))
        {
          tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
        }
        return FALSE;
      }
    } /* close else-if-rrlp_msBased_chosen */

    /* other two methodType.choice: msBasedPreferred and msAssistedPreferred should not alter
       the agreed mode */

  } /* close if-SET-Initiated */


  /**** For NI, this is where the final settlement of the gps mode takes place ***/

  else if ( p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI )
  {
    boolean u_ni_gps_method_settled = TRUE;

    /* if RRLP asks for a definitive gps mode, use that */
    if ( p_rrlp_pos_instruction->methodType.t == rrlp_msBased_chosen )
    {
      if ( TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED) ||
         ( TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET( PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED ) && 
           tm_umts_common_bds_enabled()))
      {
        /* may or may not be what SUPL_INIT has */
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
      }
      else
      {
        u_ni_gps_method_settled = FALSE;
      }
    }
    else if ( p_rrlp_pos_instruction->methodType.t == rrlp_msAssisted_chosen )
    {
      if ( TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_ASSISTED) )
      {
        /* may or may not be what SUPL_INIT has */
        p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
      }
      else
      {
        u_ni_gps_method_settled = FALSE;
      }
    }

    /* other two methodType.choice: msBasedPreferred and msAssistedPreferred should not alter
       the previously saved mode provided by SUPL_INIT. Only if agps_mode was not set during
       SUPL_INIT processing, because requested mode not supported, will it be set here */
    if ( p_rrlp_pos_instruction->methodType.t == rrlp_msBasedPref_chosen )
    {
      if (  p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UNDEF )
      {
        /* not chosen during SUPL_INIT processing, decide now */
        if ( TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED) ||
            (TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET( PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED ) && 
             tm_umts_common_bds_enabled()))
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
        }
        else if (  TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_ASSISTED) )
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
        }
        else
        {
          u_ni_gps_method_settled = FALSE;
        }
      } /* agps_mode not set during SUPL_INIT proc. */
    } /* msBasedPref */

    if ( p_rrlp_pos_instruction->methodType.t == rrlp_msAssistedPref_chosen )
    {
      if (  p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UNDEF )
      {
        /* not chosen during SUPL_INIT processing, decide now */
        if (  TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_ASSISTED) )
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_ASSISTED;
        }
        else if ( TM_CONFIG_POS_MODE_BIT_IS_SET(PDSM_POS_MODE_UP_MS_BASED) ||
                 (TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET( PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED ) && 
                  tm_umts_common_bds_enabled()))
        {
          p_supl_session_status->u_agps_mode = C_SUPL_AGPS_MODE_UE_BASED;
        }
        else
        {
          u_ni_gps_method_settled = FALSE;
        }
      } /* agps_mode not set during SUPL_INIT proc. */
    } /* msAssistedPref */

    /* if the SET is provisioned to support neither GPS msBased nor GPS msAssisted, 
     * nor BDS MS Based, no assisted session can proceed */
    if ( !( TM_CORE_CONFIG_DD(positioning_modes_supported) &
            ( PDSM_POS_MODE_UP_MS_BASED | PDSM_POS_MODE_UP_MS_ASSISTED ))  &&
         !(TM_CONFIG_ABDS_POS_MODE_BIT_IS_SET( PDSM_ENABLE_ABDS_RRLP_SUPL_MS_BASED ) && 
              tm_umts_common_bds_enabled())
       )
    {
      u_ni_gps_method_settled = FALSE;
    }

    /* if RRLP requested agps mode is not supported, end the session */
    if ( ! u_ni_gps_method_settled )
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Could not settle NI method");
      if ( !tm_umts_up_supl_suplpos_rrlp_mpr_err_tx(
                p_supl_session_status, rrlp_methodNotSupported))
      {
        tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
      }
      return FALSE;
    }

    /* if TM session was not started duing SUPL_INIT processing, because
       the SUPL_INIT requested gps_mode not provisioned, start it now */
    if (p_supl_session_status->tm_active_session_handle == TM_SESS_HANDLE_NONE)
    {
      p_supl_session_status->tm_active_session_handle = TM_SESS_HANDLE_UMTS_UP_NI;

      if ( p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED )
      {
        /* this will ask pdapi to generate pdsm_get_pos() to TmCore */
        (void )tm_umts_common_ni_client_start_session
                (TM_SESS_OPERATION_MODE_MSB, 0, TM_MAX_SESSION_TIMEOUT, 1, 0, MT_UP, FALSE);
      }
      else if ( p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED )
      {
        /* this will ask pdapi to generate pdsm_get_pos() to TmCore */
        (void )tm_umts_common_ni_client_start_session
                (TM_SESS_OPERATION_MODE_MSA, 0,TM_MAX_SESSION_TIMEOUT, 1,0, MT_UP, FALSE);
      }
    } /* TM session not started */

  } /* close if-NI */

  /* action on the RRLP provided QoS */
  supl_rrlp_msr_pos_req_qos_proc(p_supl_session_status, p_mpr);

  if ( p_mpr->m.referenceAssistDataPresent )
  {
    /* Not supported */
  }

  if ( p_mpr->m.msrAssistDataPresent )
  {
    /* Not supported */
  }

  if ( p_mpr->m.systemInfoAssistDataPresent )
  {
    /* Not supported */
  }

  if ( p_mpr->m.gps_AssistDataPresent )
  {
    u_ret = supl_rrlp_gps_assist_proc( 
                p_supl_session_status, &p_mpr->gps_AssistData.controlHeader, TRUE );
  }

  if ( p_mpr->m.extensionContainerPresent )
  {
    /* Not supported */
  }

  if ( p_mpr->m.rel98_MsrPosition_Req_extensionPresent )
  {
    /* Not supported */
  }

  if ( p_mpr->m.rel5_MsrPosition_Req_extensionPresent )
  {
    p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_present = TRUE;

    p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_ref.smlc_code =
      p_mpr->rel5_MsrPosition_Req_extension.extended_reference.smlc_code;

    p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_ref.transaction_ID =
      p_mpr->rel5_MsrPosition_Req_extension.extended_reference.transaction_ID;
  }

  if ( p_mpr->m.rel7_MsrPosition_Req_extensionPresent )
  {
    uint8 u_bad_constellation_combination = 0;
    rrlp_rel7_extension_s_type *p_rrlp_r7_ext = 
          &p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext;

    p_rrlp_r7_ext->rel7_ext_present = TRUE;

    if ( p_mpr->rel7_MsrPosition_Req_extension.m.velocityRequestedPresent )
    {
      p_rrlp_r7_ext->u_velocity_requested = TRUE;
    }

    if ( p_mpr->rel7_MsrPosition_Req_extension.m.ganssPositionMethodPresent == 1)
    {
      if ( OSTESTBIT(p_mpr->rel7_MsrPosition_Req_extension.ganssPositionMethod, 
                     rrlp_dec_GANSSPositioningMethod_rrlp_dec_gps)
         )
      {
        /* We dont allow anyone to disable GPS in NV */
        p_rrlp_r7_ext->u_ganss_pos_methods_bitmap |= RRLP_GANSS_POS_METHOD_GPS;
      }

      /* If AGlo is enabled for RRLP, and server requests AGLO, and the right mode 
       * for AGLO (MSA/MSB) is enabled, set AGLO in the pos_methods_bitmap */
      if ( tm_umts_up_supl_rrlp_assisted_glonass_enabled() &&
           OSTESTBIT(p_mpr->rel7_MsrPosition_Req_extension.ganssPositionMethod, 
                     rrlp_dec_GANSSPositioningMethod_rrlp_dec_glonass)
         )
      {
        if (((p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED ) &&
             (tm_core_pos_mode_allows_aglo_msb_up( TM_POS_PRTL_RRLP )))
            ||
           ((p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_ASSISTED ) &&
             (tm_core_pos_mode_allows_aglo_msa_up( TM_POS_PRTL_RRLP )))
            ) 
        {
          p_rrlp_r7_ext->u_ganss_pos_methods_bitmap |= RRLP_GANSS_POS_METHOD_GLONASS;
        }
      }

      /* If ABDS is enabled for RRLP, and server requests ABDS, and the right mode 
       * for ABDS (MSB) is enabled, set ABDS in the pos_methods_bitmap */
      if ( tm_umts_up_supl_rrlp_assisted_bds_enabled() &&
           OSTESTBIT(p_mpr->rel7_MsrPosition_Req_extension.ganssPositionMethod, 
                     rrlp_dec_GANSSPositioningMethod_rrlp_dec_bds)
         )
      {
        if (p_supl_session_status->u_agps_mode == C_SUPL_AGPS_MODE_UE_BASED)
        {
          p_rrlp_r7_ext->u_ganss_pos_methods_bitmap |= RRLP_GANSS_POS_METHOD_BDS;
        }
      }

      /* If no supported constellation asked for, OR
       *                                GLO Only, OR
       *                               BDS + GLO, 
       * Send an RRLP Error! We dont support. */
      if((p_rrlp_r7_ext->u_ganss_pos_methods_bitmap == 0x0) ||
         (p_rrlp_r7_ext->u_ganss_pos_methods_bitmap == RRLP_GANSS_POS_METHOD_GLONASS) ||
         (p_rrlp_r7_ext->u_ganss_pos_methods_bitmap == (RRLP_GANSS_POS_METHOD_GLONASS | RRLP_GANSS_POS_METHOD_BDS))
        )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
              "SUPL: unsupported constellation combination 0x%x. data[0] 0x%x data[1] 0x%x!", 
              p_rrlp_r7_ext->u_ganss_pos_methods_bitmap,
              p_mpr->rel7_MsrPosition_Req_extension.ganssPositionMethod.data[0],
              p_mpr->rel7_MsrPosition_Req_extension.ganssPositionMethod.data[1]);
        if ( !tm_umts_up_supl_suplpos_rrlp_mpr_err_tx(
                  p_supl_session_status, rrlp_methodNotSupported))
        {
          tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
        }
        return FALSE;
      }
    }

    if ( tm_umts_up_supl_rrlp_assisted_glonass_enabled() || 
         tm_umts_up_supl_rrlp_assisted_bds_enabled())
    {
      u_ret = supl_rrlp_ganss_assist_proc(
                  p_supl_session_status, 
                  &p_mpr->rel7_MsrPosition_Req_extension.ganss_AssistData.ganss_controlHeader);
    }
    
    supl_convert_and_send_pos_mode_to_pdapi(p_supl_session_status);
  }

  return u_ret;
}


/********************************************************************
*
* supl_send_pos_rrlp_to_slp
*
* Function description:
*   Send position in RRLP wrapped by SUPL_POS to SLP
*
* Parameters:
*   p_gad_shape_pos: pointer to the array holding the GAD shape;
*   p_velocity : pointer to a structure of the format for velocity
*                in SUPL POS
*   q_timestamp_ms : gps time of the position in msec.
*   q_pos_sources: GPS and/or GLONASS
*   q_glonass_tod_modulo_1hour_ms: GLONASS time stamp TimeOfDay
*
* Return:
*  None
*
*********************************************************************
*/
void supl_send_pos_rrlp_to_slp
( 
  supl_status_s_type           *p_supl_session_status,
  gad_ellip_alt_unc_ellip_type *p_gad_shape_pos,
  supl_Velocity *p_velocity,
  uint32 q_timestamp_ms,
  uint32 q_pos_sources,
  uint32 q_glonass_tod_modulo_1hour_ms 
)
{
  gad_velocity_hor_ver_s_type  z_gad_velocity_hor_ver;

  rrlp_enc_PDU z_rrlp;
  rrlp_enc_MsrPosition_Rsp z_rrlp_MsrPosition_Rsp;

  rrlp_enc_MsrPosition_Rsp  *p_mpr;
  rrlp_enc_LocationInfo     *p_locinfo;

  tm_pdu_buf_type   pdu_buff; /* including length and pointer to data */

  uint32 q_block_size;
  int encode_status;
  boolean ganss_pos_method = FALSE; /* TRUE if RRLP measurePosReq included GANSS posMethod in Rel7 extension */
  tm_location_notification_s_type location_notification = {0};
  
  if ( p_gad_shape_pos == NULL )
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "ERROR: p_gad_shape_pos=%lx", p_gad_shape_pos);
    return;
  }

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "supl_send_pos_rrlp_to_slp");

  /* initialize buffer for structure buffers used by OSYS ASN1 encoder */
  memset( &z_rrlp, 0, sizeof( z_rrlp ));
  memset( &z_rrlp_MsrPosition_Rsp, 0, sizeof(z_rrlp_MsrPosition_Rsp));
  memset(&pdu_buff, 0, sizeof(pdu_buff));

  z_rrlp.component.u.msrPositionRsp = &z_rrlp_MsrPosition_Rsp;
  p_mpr  = z_rrlp.component.u.msrPositionRsp;


  /* ref. num: the same as the one came from SLP */
  z_rrlp.referenceNumber =
    p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.b_RefNum;

  z_rrlp.component.t = rrlp_msrPositionRsp_chosen;


  if (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.rel7_ext_present == TRUE)
  {
    if ( (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.u_ganss_pos_methods_bitmap &
          RRLP_GANSS_POS_METHOD_GPS ) ||
         (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.u_ganss_pos_methods_bitmap &
          RRLP_GANSS_POS_METHOD_GLONASS ) ||
	 (p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.u_ganss_pos_methods_bitmap &
          RRLP_GANSS_POS_METHOD_BDS )
       )
    {
      ganss_pos_method = TRUE;
    }
  }

  if (!ganss_pos_method) /* RRLP msrPosReq does not includes REL7-extension of GANSS_posMethod */
  {

    p_locinfo  = &p_mpr->locationInfo;

    /* it is garanteed by the definition of the structures that no capping
       is needed, but just in case */
    q_block_size = memscpy(&p_locinfo->posEstimate.data[0], sizeof (p_locinfo->posEstimate.data),
                           p_gad_shape_pos, sizeof(gad_ellip_alt_unc_ellip_type));
    p_locinfo->posEstimate.numocts = q_block_size;

    p_locinfo->refFrame = 65535; /* (42432..65535) will be ignored */
    #ifndef FEATURE_CGPS_SUPL_VARIANT_SUPL_POS_NO_TIMESTAMP /* NOT defined by default */
    p_locinfo->m.gpsTOWPresent = TRUE;
    p_locinfo->gpsTOW =  q_timestamp_ms % C_FOUR_HRS_MSEC;
    #endif
    p_locinfo->fixType = rrlp_threeDFix;

    p_mpr->m.locationInfoPresent = TRUE;

  }

  else /* RRLP msrPosReq includes REL-7 extension of GANSS_posMethod: only report pos in REL-7 extension */
  {

    p_mpr->m.rel_7_MsrPosition_Rsp_ExtensionPresent = TRUE;

    p_mpr->rel_7_MsrPosition_Rsp_Extension.m.ganssLocationInfoPresent = TRUE;

    memset(p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData.data, 
           0, sizeof(p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData.data));
    p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData.numbits = 8;
    
    if ( q_pos_sources & C_POS_SOURCE_GLO )
    {
      OSSETBIT(p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData,
               rrlp_enc_PositionData_rrlp_enc_glonass_1);
    }
    if ( q_pos_sources & C_POS_SOURCE_GPS )
    {
      OSSETBIT(p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData,
               rrlp_enc_PositionData_rrlp_enc_gps_1);
    }
    if ( q_pos_sources & C_POS_SOURCE_BDS )
    {
      OSSETBIT(p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData,
               rrlp_enc_PositionData_rrlp_enc_bds_1);
    }

    if(!(( q_pos_sources & C_POS_SOURCE_GLO ) || 
         ( q_pos_sources & C_POS_SOURCE_GPS ) || 
         ( q_pos_sources & C_POS_SOURCE_BDS ))) /* default to GPS for now */
    {
      OSSETBIT(p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData,
               rrlp_enc_PositionData_rrlp_enc_gps_1);
    }
    MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Pos Sources: %lu encoded to [%d %d] nbits %d", 
          q_pos_sources, 
          p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData.data[0], 
          p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData.data[1], 
          p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posData.numbits);

    p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.fixType = rrlp_threeDFix;

    /* it is garanteed by the definition of the structures that no capping
       is needed, but just in case */
    q_block_size = memscpy(&p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posEstimate.data[0],
                           sizeof (p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posEstimate.data),
                           p_gad_shape_pos, sizeof(gad_ellip_alt_unc_ellip_type));
    p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.posEstimate.numocts = q_block_size;

    if ( q_glonass_tod_modulo_1hour_ms <= 3599999 ) /* RRLP ASN1 limit*/
    {
      p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.m.ganssTODmPresent = 1;
      p_mpr->rel_7_MsrPosition_Rsp_Extension.ganssLocationInfo.ganssTODm = q_glonass_tod_modulo_1hour_ms;
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "No GLO Timestamp: q_glonass_tod_modulo_1hour_ms=%lx",
            q_glonass_tod_modulo_1hour_ms);
    }

  }

  /* if REL-5 Extension has been included in the message sent from the network, include it
     in this uplink message too */
  if ( p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_present )
  {
    p_mpr->m.rel_5_MsrPosition_Rsp_ExtensionPresent = TRUE;

    p_mpr->rel_5_MsrPosition_Rsp_Extension.m.extended_referencePresent = TRUE;

    p_mpr->rel_5_MsrPosition_Rsp_Extension.extended_reference.smlc_code =
      p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_ref.smlc_code;

    p_mpr->rel_5_MsrPosition_Rsp_Extension.extended_reference.transaction_ID =
      p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_ref.transaction_ID;

  }

  if ( p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.rel7_ext_present )
  {
    if ( ( p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.u_velocity_requested ) &&
         ( p_velocity != NULL )
       )
    {
      p_mpr->m.rel_7_MsrPosition_Rsp_ExtensionPresent = TRUE;
      p_mpr->rel_7_MsrPosition_Rsp_Extension.m.velEstimatePresent = TRUE;

      if ( p_velocity->t == C_SUPL_ASN1_CHOICE_VELCITY_HORANDVERVEL )
      {
        /* typedef rrlp_VelocityEstimate */
        z_gad_velocity_hor_ver.u_ShapeCode_verDir_MsbBearing = 0x10; /* hor. and ver. */

        if ( p_velocity->u.horandvervel->verdirect.data[0] & 0x1 )
        {
          z_gad_velocity_hor_ver.u_ShapeCode_verDir_MsbBearing |= 0x2; /* downward */
        }
        z_gad_velocity_hor_ver.u_hor_speed0 = (uint8)(p_velocity->u.horandvervel->horspeed.data[0]);
        z_gad_velocity_hor_ver.u_hor_speed1 = (uint8)(p_velocity->u.horandvervel->horspeed.data[1]);
        z_gad_velocity_hor_ver.u_ver_speed =  (uint8)(p_velocity->u.horandvervel->verspeed.data[0]);

        /* p_velocity->u.horandvervel.bearing.length = 9 */
        if ( p_velocity->u.horandvervel->bearing.data[0] & 0x80 )
        {
          z_gad_velocity_hor_ver.u_ShapeCode_verDir_MsbBearing |= 0x1;
        }
        z_gad_velocity_hor_ver.u_hor_bearing_Lsb8 = p_velocity->u.horandvervel->bearing.data[0];
        z_gad_velocity_hor_ver.u_hor_bearing_Lsb8 <<= 1;

        z_gad_velocity_hor_ver.u_hor_bearing_Lsb8 |= (p_velocity->u.horandvervel->bearing.data[1]) >> 7;

        /* it is garanteed by the definition of the structures that no capping
           is needed, but just in case */

        q_block_size = memscpy( &p_mpr->rel_7_MsrPosition_Rsp_Extension.velEstimate.data[0],
                          sizeof (p_mpr->rel_7_MsrPosition_Rsp_Extension.velEstimate.data),
                          &z_gad_velocity_hor_ver, sizeof( z_gad_velocity_hor_ver ));

        p_mpr->rel_7_MsrPosition_Rsp_Extension.velEstimate.numocts = q_block_size;

      }
      else
      {
        /* for now, other velocity shapes not reported */
        p_mpr->rel_7_MsrPosition_Rsp_Extension.m.velEstimatePresent = FALSE;

      }

      if ( ( p_mpr->rel_7_MsrPosition_Rsp_Extension.m.ganssLocationInfoPresent == FALSE ) &&
           ( p_mpr->rel_7_MsrPosition_Rsp_Extension.m.ganssMeasureInfoPresent == FALSE ) &&
           ( p_mpr->rel_7_MsrPosition_Rsp_Extension.m.velEstimatePresent == FALSE )
         )
      {
        /* if nothing is to be included in rel7 ext. don't include it */
        p_mpr->m.rel_7_MsrPosition_Rsp_ExtensionPresent = FALSE;
      }

    } /* RRLP velocity requested and velocity available */
  } /* Rel7 Extention */

  /* ASN1 encoding */
  encode_status = tm_osys_asn1_encode_pdu((void *)&z_rrlp, &pdu_buff, PROTO_RRLP);

  if ( (encode_status != 0) || (pdu_buff.value == NULL ) || (pdu_buff.length == 0) )
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }

    if (C_SUPL_SESSION_NI == p_supl_session_status->u_callflow_kind)
    { 
      /* Construct and send location notification for FAILURE */
      location_notification.q_prtl_type = TM_PRTL_TYPE_UMTS_UP;
      location_notification.e_pdapi_client_id = p_supl_session_status->q_pdapi_client_id;
      location_notification.e_pdapi_client_type = PDSM_CLIENT_TYPE_NI;
      location_notification.e_oper_mode = rrlp_up_map_agps_mode_to_tm_oper_mode(p_supl_session_status->u_agps_mode);
      location_notification.e_sub_id = p_supl_session_status->session_sub_id;
      location_notification.e_location_info_type = PDSM_PD_LOC_INFO_GNSS_POSITION;
      location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_NO_LOC_PROVIDED;
      location_notification.v_location_privacy_override_set = p_supl_session_status->v_privacy_override_set;
      location_notification.v_emergency_bit_set = p_supl_session_status->z_emergency_data.b_emergency_ni;
    
      tm_construct_location_notification_ind(location_notification);
      
      /* Dont send any more location notification for failure cases */
      p_supl_session_status->v_send_no_more_location_notifications = TRUE;
    }
    	
    /* QQQ: return to SLP rrlp-msrPosResp-err */
    return;
  }

  if ( !supl_SuplPos_rrlp_tx (pdu_buff.value, (uint16)pdu_buff.length, p_velocity) )
  {
    tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
    tm_umts_up_supl_session_init(FALSE);
  }
  /* else, if successful, entering UT3 waiting for SUPL_END */

  if (C_SUPL_SESSION_NI == p_supl_session_status->u_callflow_kind)
  { 
    /* Construct and send location notification for SUCCESS */
    location_notification.q_prtl_type = TM_PRTL_TYPE_UMTS_UP;
    location_notification.e_pdapi_client_id = p_supl_session_status->q_pdapi_client_id;
    location_notification.e_pdapi_client_type = PDSM_CLIENT_TYPE_NI;
    location_notification.e_oper_mode = rrlp_up_map_agps_mode_to_tm_oper_mode(p_supl_session_status->u_agps_mode);
    location_notification.e_sub_id = p_supl_session_status->session_sub_id;
    location_notification.e_location_info_type = PDSM_PD_LOC_INFO_GNSS_POSITION;
    location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_LOC_PROVIDED;
    location_notification.v_location_privacy_override_set = p_supl_session_status->v_privacy_override_set;
    location_notification.v_emergency_bit_set = p_supl_session_status->z_emergency_data.b_emergency_ni;
    
    tm_construct_location_notification_ind(location_notification);    
  }
    
  /* Free the PDU */
  tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
  pdu_buff.value = NULL;

}

/*
******************************************************************************
* supl_SuplPos_RRLP_ACK_tx
*
* Function description:
*
* Encode and transmit SUPL POS carrying RRLP-ACK
*
* Parameters: NONE.
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/
static boolean supl_SuplPos_RRLP_ACK_tx
(
  supl_status_s_type   *p_supl_session_status
)
{
  boolean u_ret = FALSE;

  tm_pdu_buf_type   pdu_buff; /* including length and pointer to data */

  rrlp_enc_PDU      z_rrlp;

  int encode_status;

  /* Initialize OSS rrlp structure */
  memset( &z_rrlp, 0, sizeof(z_rrlp));

  memset(&pdu_buff, 0, sizeof(pdu_buff));

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "supl_SuplPos_RRLP_ACK_tx");
  /* ref. num: the same as the one came from SMLC */
  z_rrlp.referenceNumber =
    p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.b_RefNum;

  z_rrlp.component.t = rrlp_assistanceDataAck_chosen;

  /* Do the ASN.1 encoding for RRLP */
  encode_status = tm_osys_asn1_encode_pdu((void *)&z_rrlp, &pdu_buff, PROTO_RRLP);

  if ( (encode_status != 0) || (pdu_buff.value == NULL ) || (pdu_buff.length == 0) )
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }
    return FALSE;
  }

  if ( supl_SuplPos_rrlp_tx (pdu_buff.value, (uint16)pdu_buff.length, NULL) )
  {
    if ( ( p_supl_session_status->u_callflow_kind == C_SUPL_SESSION_NI ) &&
         ( p_supl_session_status->u_current_is_for_area_event == FALSE )
       )
    {
      /* supl_SuplPos_rrlp_tx() only sets timer to UT3_RRLP if the incoming
         RRLP assist. data contains "more_to_come"; but if NI, it should enter
         this state regardless */
      supl_set_cf_state(C_SUPL_CF_STATE_UT3_RRLP, 0);
    }
    else if (  p_supl_session_status->u_current_is_for_area_event == TRUE )
    {
      if ( p_supl_session_status->z_network_assist.u_MoreToCome == TRUE )
      {
        supl_set_cf_state(C_SUPL_CF_STATE_UT3_RRLP, 0);
      }
      else
      {
        supl_set_cf_state(C_SUPL_CF_STATE_UT3_EXPECT_SUPLREPORT, 0);
      }
    }
    u_ret = TRUE;
  }
  else
  {
    tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
    tm_umts_up_supl_session_init(FALSE);
    u_ret = FALSE;
  }

  /*Free the encoded PDU*/
  tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
  pdu_buff.value = NULL;

  return u_ret;
}

/*
******************************************************************************
* supl_SuplPos_RRLP_protocol_err_tx
*
* Function description:
*
* Encode and transmit SUPL POS carrying RRLP-protocolError
*
* Parameters: NONE.
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/

static boolean supl_SuplPos_RRLP_protocol_err_tx (int8 b_RefNum, rrlp_enc_ErrorCodes e_err_code )
{
  boolean u_ret = FALSE;
  tm_pdu_buf_type   pdu_buff; /* including length and pointer to data */
  rrlp_enc_PDU z_rrlp;
  rrlp_enc_ProtocolError z_protocol_error;
  rrlp_enc_ProtocolError *p_prot_err ;
  int encode_status;


  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "supl_SuplPos_RRLP_protocol_err_tx err code %d", e_err_code);
  memset(&z_rrlp, 0, sizeof(z_rrlp) );
  memset(&z_protocol_error, 0, sizeof(z_protocol_error) );
  memset(&pdu_buff, 0, sizeof(pdu_buff));


  z_rrlp.referenceNumber = b_RefNum;
  z_rrlp.component.t = rrlp_protocolError_chosen;

  z_rrlp.component.u.protocolError = &z_protocol_error;
  p_prot_err = z_rrlp.component.u.protocolError;

  p_prot_err->m.extensionContainerPresent = 0;
  p_prot_err->m.rel_5_ProtocolError_ExtensionPresent = 0;
  p_prot_err->errorCause = e_err_code;


  /* Do the ASN.1 encoding for RRLP */

  encode_status = tm_osys_asn1_encode_pdu((void *)&z_rrlp, &pdu_buff, PROTO_RRLP);

  if ( (encode_status != 0) || (pdu_buff.value == NULL ) || (pdu_buff.length == 0) )
  {
    /* ASN.1 encoding failed */
    if (pdu_buff.value != NULL )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. Freeing: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);

      tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
      pdu_buff.value = NULL;

    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. No alloc.: %d,%lx,%u",
            encode_status, pdu_buff.value, pdu_buff.length);
    }
    return FALSE;
  }


  if ( supl_SuplPos_rrlp_tx (pdu_buff.value, (uint16)pdu_buff.length, NULL) )
  {
    supl_set_cf_state(C_SUPL_CF_STATE_UT3_RRLP, 0);
    u_ret = TRUE;
  }
  else
  {
    tm_umts_up_supl_inform_core_stop_session(TM_STOP_REASON_GENERAL_ERROR);
    tm_umts_up_supl_session_init(FALSE);
    u_ret = FALSE;
  }

  /*Free the encoded PDU*/
  tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
  pdu_buff.value = NULL;

  return u_ret;

}


/*
 ******************************************************************************
 * Function description:
 *  Decode ASN1 encoded RRLP messages and process the assistance data
 *
 * Parameters:
 *
 *  Data_ptr: pointer to the lowest address byte in RRLP data buffer
 *
 *  w_max_len_byte: maximum possible length of the RRLP message(s), possibly
 *                  multiple RRLP messages
 *
 *  osys_decode_status_for_calling_func: pointer to OSS decoder error code
 *
 * Return value:
 *
 *    TRUE: successfully docoded; FALSE: protocol error has occured.
 *
 *
 ******************************************************************************
*/
static boolean supl_rrlp_decode_proc
( 
  supl_status_s_type   *p_supl_session_status,
  tm_network_assist_s_type  *p_tm_network_assist,
  uint8*            Data_ptr,
  uint16            w_max_len_byte,
  int*              asn1_decode_status_for_calling_func_ptr 
)
{
  rrlp_dec_PDU           z_rrlp_pdu_decoded;
  rrlp_dec_PDU *p_rrlp = &z_rrlp_pdu_decoded;
  uint32 success = TRUE;
  int asn1_decode_status =  0xFFFFFFFF;
  rrlp_dec_ErrorCodes rrlp_protocol_reason;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "supl_rrlp_decode_proc");
  memset(&z_rrlp_pdu_decoded, 0, sizeof(z_rrlp_pdu_decoded));

  tm_osys_asn1_decode_pdu( Data_ptr, w_max_len_byte, (void *)p_rrlp, PROTO_RRLP, &asn1_decode_status);

  *asn1_decode_status_for_calling_func_ptr = asn1_decode_status;

  if ( asn1_decode_status == 0 )
  {

    p_tm_network_assist->radio_link_prot_params.z_rrlp_params.b_RefNum = (int8)p_rrlp->referenceNumber;

    switch ( p_rrlp->component.t )
    {
      case rrlp_msrPositionReq_chosen:
      {
        p_tm_network_assist->radio_link_prot_params.z_rrlp_params.u_rrlp_choice = C_RRLP_CHOICE_MSRPOSREQ;

        if ( ! supl_rrlp_msr_pos_req_proc(
                   p_supl_session_status, p_rrlp->component.u.msrPositionReq ) )
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL: err RRLP-msrPosReq", 0, 0, 0);
          success = FALSE;
        }
        break;
      }

      case rrlp_msrPositionRsp_chosen:
      {
        /* We should never be receiving this */
        rrlp_protocol_reason = rrlp_dec_ErrorCodes_unDefined;
        (void)supl_SuplPos_RRLP_protocol_err_tx( p_tm_network_assist->radio_link_prot_params.z_rrlp_params.b_RefNum,
                                          rrlp_protocol_reason );
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL: Unexpected RRLP-msrPosRsp received",0,0,0);
        success = FALSE;
        break;
      }

      case rrlp_assistanceData_chosen:
      {
        rrlp_dec_AssistanceData *p_Assist = p_rrlp->component.u.assistanceData;

        p_tm_network_assist->radio_link_prot_params.z_rrlp_params.u_rrlp_choice = C_RRLP_CHOICE_ASSISTDATA;

        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL: RRLP-AD received");
        if ( p_Assist->m.gps_AssistDataPresent ) /* other types of assist. data ignored */
        {
          if ( !supl_rrlp_gps_assist_proc( 
                    p_supl_session_status, 
                    &p_rrlp->component.u.assistanceData->gps_AssistData.controlHeader, 
                    FALSE ))
          {
            success = FALSE;
          }
        }

        /* Before entering this next IF, make sure p_tm_network_assist->u_MoreToCome
           is set to FALSE because although SnapTrack SMLC always provide this
           optional sequence of "more-assistance-to-come" to indicate whether
           or not more is on the way, non-SnapTrack yet compliant SMLCs may not
           have that sequence, in which case it means no-more-to-come. */

        p_tm_network_assist->u_MoreToCome = FALSE;

        if ( p_Assist->m.moreAssDataToBeSentPresent )
        {
          if ( p_Assist->moreAssDataToBeSent == rrlp_dec_MoreAssDataToBeSent_moreMessagesOnTheWay )
          {
            p_tm_network_assist->u_MoreToCome = TRUE;
          }
        }

        if ( p_Assist->m.rel5_AssistanceData_ExtensionPresent )
        {
          p_tm_network_assist->radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_present = TRUE;

          p_tm_network_assist->radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_ref.smlc_code =
            p_Assist->rel5_AssistanceData_Extension.extended_reference.smlc_code;

          p_tm_network_assist->radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_ref.transaction_ID =
            p_Assist->rel5_AssistanceData_Extension.extended_reference.transaction_ID;
        }

        if ( (tm_umts_up_supl_rrlp_assisted_glonass_enabled() ||
              tm_umts_up_supl_rrlp_assisted_bds_enabled()) &&
             (p_Assist->m.rel7_AssistanceData_ExtensionPresent == 1)
           )
        {
          if ( p_Assist->rel7_AssistanceData_Extension.m.ganss_AssistDataPresent == 1)
          {
            success = supl_rrlp_ganss_assist_proc(
                        p_supl_session_status, 
                       &p_Assist->rel7_AssistanceData_Extension.ganss_AssistData.ganss_controlHeader);
          }
        }

        break;
      }

      case rrlp_assistanceDataAck_chosen:
      {
        /* We should never be receiving this */
        rrlp_protocol_reason = rrlp_dec_ErrorCodes_unDefined;
        (void)supl_SuplPos_RRLP_protocol_err_tx( p_tm_network_assist->radio_link_prot_params.z_rrlp_params.b_RefNum,
                                          rrlp_protocol_reason );
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL: Unexpected RRLP-AD Ack received");
        success = FALSE;
        break;
      }

      case rrlp_protocolError_chosen:
      {
        p_tm_network_assist->radio_link_prot_params.z_rrlp_params.u_rrlp_choice = C_RRLP_CHOICE_PROTCOL_ERR;
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL: SLP detects RRLP protocol error from SET", 0, 0, 0);
        tm_umts_up_supl_callflow_disrupt_handler(); /* the SET will not try to correct itself */
        success = FALSE;
        break;
      }

      /* "The RRLP Positioning Capability Transfer procedure introduced in RRLP
         Release 7 (section 2.3a in [3GPP RRLP]) SHALL NOT be used." */
      case rrlp_posCapabilityReq_chosen:
      case rrlp_posCapabilityRsp_chosen:
      {
        rrlp_protocol_reason = rrlp_dec_ErrorCodes_unDefined;
        (void)supl_SuplPos_RRLP_protocol_err_tx( p_tm_network_assist->radio_link_prot_params.z_rrlp_params.b_RefNum,
                                          rrlp_protocol_reason );
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL: Unexpected RRLP-rrlp_posCapability %u received",p_rrlp->component.t,0,0);
        success = FALSE;
        break;
      }


      default:
      {
        /* Unknown type */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW,"LSM - Unknown RRLP-choice received",0,0,0);
        success = FALSE; /* shouldn't happen as OSS ASN.1 decoder will fail */
        break;
      }
    }

    tm_osys_asn1_free_pdu (p_rrlp , PROTO_RRLP);

  }

  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"SUPL: RRLP Decode Failure",0,0,0);

    if (asn1_decode_status != C_ASN1_DECODE_BUFFER_NOT_ALLOCATED)
    {
      tm_osys_asn1_free_pdu (p_rrlp , PROTO_RRLP);
    }

    /* when this happens, there is no information about the current
       message's reference number which makes it impossible to return with RRLP-protocolError;
       Send SuplEnd, and hanle this as call flow disruption */
    (void) tm_umts_up_supl_supl_end_prepare_or_tx(&p_supl_session_status->z_session_id.z_Slp_Sess_Id,
                                           NULL,
                                           supl_StatusCode_protocolError,
                                           NULL,
                                           p_supl_session_status->q_supl_version);
    tm_umts_up_supl_callflow_disrupt_handler();

    return FALSE;
  }

  return ( success );
}


/*
******************************************************************************
* supl_SuplPos_rrlp_proc
*
* Function description:
*
* Process incoming SUPL POS message containing RRLP payload
*
* Parameters: p_payload: pointer to SUPL POS RRLP payload
*                        payload_length: Length of the payload
*
* Return value: TRUE or FALSE
*
******************************************************************************
*/
boolean supl_SuplPos_rrlp_proc
(
  supl_status_s_type   *p_supl_session_status,
  uint8 *p_payload, 
  const uint16 payload_length
)
{

  int osys_decode_status = 0xFFFFFFFF;
  boolean u_ret = TRUE;


  if(p_supl_session_status == NULL || p_payload == NULL)
  {
    return FALSE;
  }

  if ( supl_rrlp_decode_proc (
          p_supl_session_status, 
          &p_supl_session_status->z_network_assist, 
          p_payload, 
          payload_length, 
          &osys_decode_status )  )
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "RRLP choice of %u decode/proc ok",
          p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_rrlp_choice);

    /* check if a SUPL_POS-RRLP-ACK is needed to send back to the SLP */
    if ( p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_rrlp_choice
         == C_RRLP_CHOICE_ASSISTDATA
       )
    {
      if ( !supl_SuplPos_RRLP_ACK_tx(p_supl_session_status) )
      {
        u_ret = FALSE;
      }
    }


    if ( ! ( p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_UT3_RRLP ) )
    {

      /* No more assistance from the SLP; tell TmCore to send pdapi event of
         EventGpsPdConnectionDone */
      tm_umts_common_send_pdapi_comm_event
      (  PDSM_PD_COMM_PROTOCOL_UMTS_UP_SUPL,
         C_COMM_DONE,
         p_supl_session_status->tm_active_session_handle,
         p_supl_session_status->q_pdapi_client_id
      );


      /* If there is a final position pending to be sent to the network, and
         this involves RRLP-msrPosReq, report the saved position to SLP now */
      if ( ( p_supl_session_status->z_pending_new_pos_to_network.u_valid == TRUE ) &&
           ( p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.u_final == TRUE )
         )
      {
        if ( p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_rrlp_choice ==
             C_RRLP_CHOICE_MSRPOSREQ )
        {

          if ( p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.vel_valid )
          {
            supl_Velocity z_velocity;

            supl_Horandvervel z_horandvervel;

            memset(&z_velocity, 0, sizeof(supl_Velocity));
            memset(&z_horandvervel, 0, sizeof(supl_Horandvervel));
            z_velocity.u.horandvervel = &z_horandvervel;

            supl_convert_velocity ( p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.f_VelEnu[0],
                                    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.f_VelEnu[1],
                                    p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.f_VelEnu[2],
                                    &z_velocity);

            /* send the pending position out to the network */
            supl_send_pos_rrlp_to_slp
              ( p_supl_session_status,
                &p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.z_pos,
                &z_velocity,
                p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_fix_time_gps_msec % C_FOUR_HRS_MSEC,
                p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_pos_sources,
                p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_glonass_tod_modulo_1hour_ms);
          }

          else  /* no velocity info */
          {
            /* send the pending position out to the network */
            supl_send_pos_rrlp_to_slp
              ( p_supl_session_status,
                &p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.z_pos,
                NULL,
                p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_fix_time_gps_msec % C_FOUR_HRS_MSEC,
                p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_pos_sources,
                p_supl_session_status->z_pending_new_pos_to_network.z_pos_info.q_glonass_tod_modulo_1hour_ms);
          }
        }

      }

      if ( p_supl_session_status->u_agps_mode != C_SUPL_AGPS_MODE_UE_ASSISTED )
      {
        /* Determine if it has enough info. about some SVs' nonexistence and
           inform MGP about such info., if available */
        tm_util_post_non_exist_svs ( TM_PRTL_TYPE_UMTS_UP,
          p_supl_session_status->tm_active_session_handle,
          p_supl_session_status->q_alm_delivered_sv_list,
          p_supl_session_status->q_eph_delivered_sv_list );

        /* inform TM core to start session timer if not SET-Assisted; Note for
           SET-Assisted, this is done after steering has been injected */
        tm_umts_up_supl_inform_core_nt_assist_end(p_supl_session_status);
      }
    }

    return TRUE;

  } /* close if-decoded-ok */

  else
  {

    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "RRLP choice of %u decode/proc encountered error",
          p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.u_rrlp_choice);

    u_ret = FALSE;

  }

  return u_ret;
}


/*===========================================================================
FUNCTION tm_umts_up_rrlp_handle_prm_data

DESCRIPTION:
    Process the cached AGNSS measurements & report the qualified measurements to
    SUPL over RRLP server.

PARAMETERS:

DEPENDENCIES: none.

RETURN VALUE: none.

SIDE EFFECTS: none.

===========================================================================*/
boolean tm_umts_up_rrlp_handle_prm_data( supl_status_s_type *p_supl_session_status )
{
  uint8    u_i,
           u_list_len;
  uint8    u_sv_cnt;
  uint32   flags                       = 0;
  int      encode_status;
  boolean  status_check_needed         = FALSE;
  boolean  b_gps_meas_qualified        = FALSE;
  boolean  b_glonass_meas_qualified    = FALSE;
  boolean  v_tunc_to_include           = FALSE;
  boolean  v_RetVal                    = FALSE;
  tm_location_notification_s_type location_notification = {0};

  tm_pdu_buf_type              pdu_buff; /* including length and pointer to data */
  rrlp_enc_PDU                 z_rrlp;
  rrlp_enc_MsrPosition_Rsp     *p_mpr;
  gnss_MeasStructType          *p_gps_meas_per_sv;
  rrlp_enc_MsrPosition_Rsp     z_rrlp_MsrPosition_Rsp;
  rrlp_enc_GPS_MsrSetElement   z_rrlp_GPS_MsrSetElement;
  rrlp_enc_GPS_MsrElement      z_MsrElement[16]; /* max. 16 SVs */
  rrlp_enc_GANSS_MsrSetElement *p_rrlp_GANSS_MsrSetElement = NULL;
  sm_GpsMeasRptStructType      *p_gnss_prm_data            = NULL;

  if ( NULL == p_supl_session_status )
  {
    return ( FALSE );
  }

  if ( ( p_supl_session_status->u_agps_mode != C_SUPL_AGPS_MODE_UE_ASSISTED ) ||
       (0 == ( p_supl_session_status->e_cf_state & C_SUPL_CF_STATE_SUPLPOS_XCHG )) )
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SUPL RRLP: Not in right mode %u, 0x%X",
          p_supl_session_status->u_agps_mode, p_supl_session_status->e_cf_state );
    return ( FALSE );
  }

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "handle meas for rrlp msg");

  memset( &pdu_buff, 0, sizeof(tm_pdu_buf_type) );
  memset( &z_rrlp, 0, sizeof(z_rrlp));
  memset( &z_rrlp_MsrPosition_Rsp, 0, sizeof(z_rrlp_MsrPosition_Rsp) );
  memset( &z_rrlp_GPS_MsrSetElement, 0, sizeof(z_rrlp_GPS_MsrSetElement) );
  memset( &z_MsrElement[0], 0, sizeof(z_MsrElement));

  z_rrlp.component.u.msrPositionRsp = &z_rrlp_MsrPosition_Rsp;
  z_rrlp_MsrPosition_Rsp.gps_MeasureInfo.gpsMsrSetList.n = 1;
  z_rrlp_MsrPosition_Rsp.gps_MeasureInfo.gpsMsrSetList.elem = &z_rrlp_GPS_MsrSetElement;
  z_rrlp_GPS_MsrSetElement.gps_msrList.elem = &z_MsrElement[0];
  p_mpr = &z_rrlp_MsrPosition_Rsp;

  /* ref. num: the same as the one came from SLP */
  z_rrlp.referenceNumber = p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.b_RefNum;
  z_rrlp.component.t = rrlp_msrPositionRsp_chosen;
  status_check_needed = p_supl_session_status->z_gnss_meas_report.m.s_StatusCheckNeeded;

  /* Propagate each GPS and/or GLONASS SV's measurement to the common reference point:
     p_prm_data->z_MeasBlk.z_MeasClock.q_RefFCount */
  if ( p_supl_session_status->z_gnss_meas_report.u_meas_reports_bmap & SM_GNSS_MEAS_REPORTS_BMAP_GPS )
  {
    gps_MeasBlkPropagate( &(p_supl_session_status->z_gnss_meas_report.z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS].z_MeasBlk) );
  }
  if ( p_supl_session_status->z_gnss_meas_report.u_meas_reports_bmap & SM_GNSS_MEAS_REPORTS_BMAP_GLONASS )
  {
    gnss_MeasBlkPropagate( &(p_supl_session_status->z_gnss_meas_report.z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS].z_MeasBlk) );
    gnss_GloMeasBlkGGRfgdAdjust( &(p_supl_session_status->z_gnss_meas_report.z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS].z_MeasBlk) );
  }

  if ( p_supl_session_status->z_gnss_meas_report.u_meas_reports_bmap & SM_GNSS_MEAS_REPORTS_BMAP_GPS )
  {
    p_gnss_prm_data = &p_supl_session_status->z_gnss_meas_report.z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS];

    /* if this number is ever greater than 16, limit to the first 16 */
    if ( p_gnss_prm_data->z_MeasBlk.z_GnssHeader.u_NumSvs < 16 )
    {
      u_list_len = p_gnss_prm_data->z_MeasBlk.z_GnssHeader.u_NumSvs;
    }
    else
    {
      u_list_len = 16;
    }

    /* count to see if there is any non-zero CNo in the list
       Only SVs that are not blacklisted are considered */
    u_sv_cnt = 0;

    p_gps_meas_per_sv = &p_gnss_prm_data->z_MeasBlk.z_Gnss[0];
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "tm_umts_up_rrlp_handle_prm_data: Calling BlacklistFilter");
    for ( u_i = 0; u_i < u_list_len; u_i++ )
    {
      if ((p_gps_meas_per_sv->w_Cno > 0) && 
          (FALSE == gnss_SvIsBlacklisted(&tm_core_info.config_info.z_GnssPersistSV, p_gps_meas_per_sv->w_Sv)))
      {
        u_sv_cnt++;
		/* If one or more measurements do not have their SV millisecond-known bit set, then send TUNC. */
        if ((FALSE == v_tunc_to_include) && (FALSE == gps_meas_status_bits_check(p_gps_meas_per_sv->q_MeasStatus, TRUE)))
        {
          v_tunc_to_include = TRUE;
        }
      }
      p_gps_meas_per_sv++;
    }

    if ( u_sv_cnt != 0 )
    {
      if ( tm_rrlp_mprsp_gps_meas_build( p_mpr, p_gnss_prm_data,
                                         u_list_len,
                                         status_check_needed ) == 0 )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "GPS meas. build aborted: not enough qualified SVs", 0, 0, 0 );
      }
      else
      {
        b_gps_meas_qualified = TRUE;
      }
    }

  } /* if GPS available  */

  if ( p_supl_session_status->z_gnss_meas_report.u_meas_reports_bmap & SM_GNSS_MEAS_REPORTS_BMAP_GLONASS )
  {
    /* rel7 extension GANSS measurements are only for sending GLONASS measurements to the SLP.
       If both GPS and GANSS measurements are to be sent to SLP,
       the GPS measurements are sent in gps-MeasureInfo while the GLONASS measurements are sent in ganssMeasureInfo
     */

    if ( ( p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.rel7_ext_present == TRUE) &&
         ( p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel7_ext.u_ganss_pos_methods_bitmap &
            RRLP_GANSS_POS_METHOD_GLONASS
         )
        )
    {

      p_gnss_prm_data = &p_supl_session_status->z_gnss_meas_report.z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS];


      p_rrlp_GANSS_MsrSetElement = os_MemAlloc( sizeof(*p_rrlp_GANSS_MsrSetElement), OS_MEM_SCOPE_TASK );

      if ( p_rrlp_GANSS_MsrSetElement != NULL )
      {
        memset( p_rrlp_GANSS_MsrSetElement, 0, sizeof(*p_rrlp_GANSS_MsrSetElement) );

        if ( tm_rrlp_mprsp_glonass_meas_build( p_mpr, p_gnss_prm_data, status_check_needed,
                                               b_gps_meas_qualified, p_rrlp_GANSS_MsrSetElement
                                              ) == FALSE
            )
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "GLONASS meas. build aborted", 0, 0, 0 );

          /* free p_rrlp_GANSS_MsrSetElement :  */
          os_MemFree( (void **)&p_rrlp_GANSS_MsrSetElement );
        }
        else
        {
          b_glonass_meas_qualified = TRUE;
        }
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "rrlp_GANSS_MsrSetElement allocation failure, skip GLO meas. report", 0, 0, 0 );
      }

    }
  } /* if GLONASS available */

  if ( !b_gps_meas_qualified && !b_glonass_meas_qualified )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Neither GPS or GLONASS meas. qualified", 0, 0, 0 );
    return ( FALSE );
  }

  /* if REL-5 Extension has been included in the message sent from the network, include it
     in this uplink message too */
  if ( p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_present )
  {
    p_mpr->m.rel_5_MsrPosition_Rsp_ExtensionPresent = TRUE;
    p_mpr->rel_5_MsrPosition_Rsp_Extension.m.extended_referencePresent = TRUE;
    p_mpr->rel_5_MsrPosition_Rsp_Extension.extended_reference.smlc_code =
      p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_ref.smlc_code;
    p_mpr->rel_5_MsrPosition_Rsp_Extension.extended_reference.transaction_ID =
      p_supl_session_status->z_network_assist.radio_link_prot_params.z_rrlp_params.z_rrlp_rel5_ext.rel5_ext_ref.transaction_ID;
  }

  if (v_tunc_to_include)
  {
    /*Status check was not done, include time uncertainity*/
    p_mpr->m.rel_98_MsrPosition_Rsp_ExtensionPresent = TRUE;
    p_mpr->rel_98_MsrPosition_Rsp_Extension.m.timeAssistanceMeasurementsPresent = TRUE;
    p_mpr->rel_98_MsrPosition_Rsp_Extension.timeAssistanceMeasurements.m.gpsReferenceTimeUncertaintyPresent = TRUE;
    p_mpr->rel_98_MsrPosition_Rsp_Extension.timeAssistanceMeasurements.gpsReferenceTimeUncertainty =
      tm_umts_common_utils_microsec_to_GADK( p_gnss_prm_data->z_MeasBlk.z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.f_ClkTimeUncMs *1000 );
  }

  /* Do the RRLP encode */
  encode_status = tm_osys_asn1_encode_pdu( (void *)&z_rrlp, &pdu_buff, PROTO_RRLP );

  if ( (encode_status != 0) || (pdu_buff.value == NULL ) || (pdu_buff.length == 0) )
  {
    /* ASN.1 encoding failed and pdu buffer allocated  */
    if ( pdu_buff.value != NULL )
    {
      MSG_3( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. Freeing: %d,%lx,%u",
             encode_status, pdu_buff.value, pdu_buff.length );

      tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
      pdu_buff.value = NULL;

    }
    else /* ASN.1 encoding failed but pdu buffer allocated  */
    {
      MSG_3( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "RRLP ASN1 enc. err. No alloc.: %d,%lx,%u",
             encode_status, pdu_buff.value, pdu_buff.length );
    }
    return ( FALSE );
  } /* if pdu ASN1 encoding fails */
  else /* pdu ASN1 encoding successful */
  {
    /* clear timer and call flow state */
    tm_umts_up_supl_terminate_timer_cancel_cf_state( C_SUPL_CF_STATE_SUPLPOS_XCHG );

    if ( !supl_SuplPos_rrlp_tx( pdu_buff.value, (uint16)pdu_buff.length, NULL ) )
    {      
      tm_umts_up_supl_inform_core_stop_session( TM_STOP_REASON_GENERAL_ERROR );
      tm_umts_up_supl_session_init( FALSE );
      v_RetVal = FALSE;
    }
    else
    {
      if (C_SUPL_SESSION_NI == p_supl_session_status->u_callflow_kind)
      { 
        /* Construct and send location notification for SUCCESS */
        location_notification.q_prtl_type = TM_PRTL_TYPE_UMTS_UP;
        location_notification.e_pdapi_client_id = p_supl_session_status->q_pdapi_client_id;
        location_notification.e_pdapi_client_type = PDSM_CLIENT_TYPE_NI;
        location_notification.e_oper_mode = rrlp_up_map_agps_mode_to_tm_oper_mode(p_supl_session_status->u_agps_mode);
        location_notification.e_sub_id = p_supl_session_status->session_sub_id;
        location_notification.e_location_info_type = PDSM_PD_LOC_INFO_GNSS_MEAS;
        location_notification.e_location_response = PDSM_PD_LOC_RESPONSE_ACCEPTED_LOC_PROVIDED;
        location_notification.v_location_privacy_override_set = p_supl_session_status->v_privacy_override_set;
        location_notification.v_emergency_bit_set = p_supl_session_status->z_emergency_data.b_emergency_ni;
        
        tm_construct_location_notification_ind(location_notification); 
      }
      	  
      /* GPS Complete event */
      sm_log_event_report( SM_LOG_EVENT_ID_GPS_COMPLETE, 0 );

      /* QQQ: for possible time-stamping SET-Assisted position, until a UTC-->GPS conversion
         utility is available */
      if ( p_supl_session_status->z_set_assist_pos_time_stamp.m.refTimeWeekNumValid )
      {
        p_supl_session_status->z_set_assist_pos_time_stamp.t_pos_time_stamp_ms =
          tm_umts_common_utils_full_gps_ms( p_supl_session_status->z_set_assist_pos_time_stamp.w_ref_time_week_num,
                                            p_gnss_prm_data->z_MeasBlk.z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.q_GpsMsec );

        p_supl_session_status->z_set_assist_pos_time_stamp.m.posTimeStampValid = TRUE;
      }
      v_RetVal = TRUE;
    }
    /* Free the PDU */
    tm_asn1_free_osys_buf( pdu_buff.value, PROTO_RRLP );
    pdu_buff.value = NULL;
  }

  /* regardless whether pdu encoding is successful, free allocated IEs */
  if ( p_rrlp_GANSS_MsrSetElement != NULL )  /* allocated in this function */
  {
    if ( p_rrlp_GANSS_MsrSetElement->ganss_MsrElementList.elem != NULL )
    {
      if ( p_rrlp_GANSS_MsrSetElement->ganss_MsrElementList.elem->ganss_SgnTypeList.elem != NULL )
      {
        if ( p_rrlp_GANSS_MsrSetElement->ganss_MsrElementList.elem->ganss_SgnTypeList.elem->ganss_SgnList.elem != NULL )
        {
          os_MemFree( (void **)&p_rrlp_GANSS_MsrSetElement->ganss_MsrElementList.elem->ganss_SgnTypeList.elem->ganss_SgnList.elem );
        }
        os_MemFree( (void **)&p_rrlp_GANSS_MsrSetElement->ganss_MsrElementList.elem->ganss_SgnTypeList.elem );
      }
      os_MemFree( (void **)&p_rrlp_GANSS_MsrSetElement->ganss_MsrElementList.elem );
    }

    os_MemFree( (void **)&p_rrlp_GANSS_MsrSetElement ); /* allocated in this function */
  }

  return ( v_RetVal );
}

/*===========================================================================
FUNCTION rrlp_up_map_agps_mode_to_tm_oper_mode

DESCRIPTION
  This function maps from SUPL agps mode type to TM operation mode

PARAMETERS: supl_session_agps_mode_enum_type e_agps_mode
   
DEPENDENCIES: none.

RETURN VALUE: tm_sess_operation_mode_e_type.

SIDE EFFECTS: none.

===========================================================================*/

static tm_sess_operation_mode_e_type rrlp_up_map_agps_mode_to_tm_oper_mode(supl_session_agps_mode_enum_type e_agps_mode)
{
  tm_sess_operation_mode_e_type e_oper_mode;

  switch (e_agps_mode)
  {
    case C_SUPL_AGPS_MODE_UE_BASED:
        e_oper_mode = TM_SESS_OPERATION_MODE_MSB;
        break;

    case C_SUPL_AGPS_MODE_UE_ASSISTED:
        e_oper_mode = TM_SESS_OPERATION_MODE_MSA;
        break;

    case C_SUPL_OTDOA_MODE:
        e_oper_mode = TM_SESS_OPERATION_MODE_OTDOA;
        break;

    default:
        MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Privacy: Unsupported agps_mode %d", e_agps_mode );
        e_oper_mode = TM_SESS_OPERATION_MODE_NONE;
  }
  return e_oper_mode;
}


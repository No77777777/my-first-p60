/*===========================================================================
            R R C   M E A S U R E M E N T S

DESCRIPTION

   This module supports Measurement Control and Measurement Reporting
   for the RRC layer. At present, it supports the following features
   for in Idle mode and DCH state
   1. Read SIB 11 and tell L1 to setup inter-RAT meas
   based on them.
   2. Sends Meas Report to UTRAN in DCH state.
   3. Supports Measurement Control Message received from UTRAN



EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

   RRC Task must have been started. All RRC command queues must
   have been initialized. All RRC signals must have been cleared
   at task startup.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

             EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcirat.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who   what, where, why
--------   ---   ------------------------------------------------------------
12/17/12   zy      fix CR430249, and add the check of m_length and k in DMO, and check valid of all eutra frequency
10/08/12   ftp    Turn BSIC back ON
09/29/12   zy      add code of IE DMO and IE Idele Interval Info 
10/20/11   yuh   Merged LTE code
03/10/11   zwj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/23/10   zwj    Modify meas report for TDD.
11/08/10   zwj    Delete compress mode CCSA 8.4.1.3 for TDD.

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "customer.h"
#include "comdef.h"
#include "tdsrrcmeas.h"
#include "tdsrrcmeasi.h"
#include "tdsrrcsibdb.h"
#include "msg.h"
#include "tdsrrccmd_v.h"
#include "tdsl1mtask.h"
#include "tdsrrclcm.h"
#include "tdsrrcsend.h"
#include "tdsuecomdef.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcmcm.h"
#include "tdsrrccsp.h"
#include "tdsrrcasn1util.h"
#include "tdsrrcnv.h"
#include "tdsrrccommon.h"

#ifdef FEATURE_GSM_QUADBAND
/*#include "rr_rrc_if.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"
#include "geran_grr_api.h"
#endif /* FEATURE_GSM_QUADBAND */

#include "tdsrrccspi.h"
#include "tdsrrclogging.h"

#ifdef FEATURE_TDSCDMA_REL9
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#endif

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
#include <lte_rrc_ext_api.h>
#endif
// #define FEATURE_ENH_BSIC_VERIFY

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSRET_BSIC_VERIFICATION_REQD(verification) \
( \
(verification == tdsrrc_BSIC_VerificationRequired_required) ? TRUE : FALSE \
)

/* Network Color Code */
#define TDSNCC_MAX   7
/* Base Station Color Code */
#define TDSBCC_MAX   7

/* Inter-RAT Cell Id */
#define TDSMAX_INTER_RAT_CELL_ID 31

/* ARFCN */
#define TDSMAX_ARFCN 1024

/* MAX RSSI */
#define TDSMAX_RSSI 63

/* Min RSSI */
#define TDSMIN_RSSI 0

/* Max RSSI Length */
#define TDSRSSI_MAX_LENGTH 1

/* Actual RSSI Length in bits */
#define TDSRSSI_ACTUAL_LENGTH 6

/* Invalid ARFCN */
#define TDSRRC_INVALID_ARFCN_FLAG 10000


/* Inter-RAT Reporting Amount */
#define  TDSRET_INTER_RAT_REPORTING_AMOUNT(reporting_amount)  \
(  \
 ((reporting_amount == tdsrrc_ReportingAmount_ra1) ? TDSL1_RPT_AMOUNT_1 : \
  ((reporting_amount == tdsrrc_ReportingAmount_ra2) ? TDSL1_L1_RPT_AMOUNT_2 : \
  ((reporting_amount == tdsrrc_ReportingAmount_ra4) ? TDSL1_RPT_AMOUNT_4 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra8) ? TDSL1_RPT_AMOUNT_8 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra16) ? TDSL1_RPT_AMOUNT_16 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra32) ? TDSL1_RPT_AMOUNT_32 :\
  (reporting_amount == tdsrrc_ReportingAmount_ra64) ? TDSL1_RPT_AMOUNT_64 : TDSL1_RPT_AMOUNT_INFINITY \
  )))))) \
)

/*
#define RET_INTER_RAT_PERIODIC_REPORTING_INTERVAL(ri)  \
(  \
 (ri == rrc_ril0) ? 0 : \
  ((ri == rrc_ril0_25) ? TDSL1_RPT_INTERVAL_250MS : \
  ((ri == rrc_ril0_5) ? TDSL1_RPT_INTERVAL_500MS :\
  ((ri == rrc_ril1) ? TDSL1_RPT_INTERVAL_1000MS :\
  ((ri == rrc_ril2) ? TDSL1_RPT_INTERVAL_2000MS :\
  ((ri == rrc_ril3) ? TDSL1_RPT_INTERVAL_3000MS :\
  ((ri == rrc_ril4) ? TDSL1_RPT_INTERVAL_4000MS :\
  ((ri == rrc_ril6) ? TDSL1_RPT_INTERVAL_6000MS :\
  ((ri == rrc_ril8) ? TDSL1_RPT_INTERVAL_8000MS :\
  ((ri == rrc_ril12) ? TDSL1_RPT_INTERVAL_12000MS :\
  ((ri == rrc_ril16) ? TDSL1_RPT_INTERVAL_16000MS :\
  ((ri == rrc_ril20) ? TDSL1_RPT_INTERVAL_20000MS :\
  ((ri == rrc_ril24) ? TDSL1_RPT_INTERVAL_24000MS :\
  ((ri == rrc_ril28) ? TDSL1_RPT_INTERVAL_28000MS :\
  (ri == rrc_ril32) ? TDSL1_RPT_INTERVAL_32000MS : TDSL1_RPT_INTERVAL_64000MS \
  ))))))))))))) \
)
*/


/* Inter-RAT Event Type */
#define  TDSRET_INTER_RAT_EVENT_TYPE(event_type)  \
(  \
 ((event_type == T_tdsrrc_InterRATEvent_event3a) ? TDSL1_INTER_RAT_EVENT_3A : \
  ((event_type == T_tdsrrc_InterRATEvent_event3b) ? TDSL1_INTER_RAT_EVENT_3B : \
  ((event_type == T_tdsrrc_InterRATEvent_event3c) ? TDSL1_INTER_RAT_EVENT_3C : \
  TDSL1_INTER_RAT_EVENT_3D ))) \
)

/* Converts time to trigger for FDD from ASN1 format to L1 format */
#define  TDSRET_INTER_RAT_TIME_TO_TRIGGER(time_to_trigger)  \
    (  \
     (time_to_trigger == tdsrrc_TimeToTrigger_ttt0) ? 0 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt10) ? 10 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt20) ? 20 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt40) ? 40 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt60) ? 60 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt80) ? 80 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt100) ? 100 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt120) ? 120 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt160) ? 160 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt200) ? 200 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt240) ? 240 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_tt320) ? 320 : \
      ((time_to_trigger == tdsrrc_TimeToTrigger_ttt640) ? 640 : \
     ((time_to_trigger == tdsrrc_TimeToTrigger_ttt1280) ? 1280 : \
    ((time_to_trigger == tdsrrc_TimeToTrigger_ttt2560) ? 2560 : 5000 \
      )))))))))))))) \
    )


/* GSM Filter Coefficient */
#define  TDSRET_GSM_FILTER_COEFFICIENT(fc)  \
(  \
 (fc == tdsrrc_FilterCoefficient_fc0 ) ? TDSL1_GSM_FILTER_COEF_0 : \
  ((fc == tdsrrc_FilterCoefficient_fc1 ) ? TDSL1_GSM_FILTER_COEF_1 :\
  ((fc == tdsrrc_FilterCoefficient_fc2 ) ? TDSL1_GSM_FILTER_COEF_2 :\
  ((fc == tdsrrc_FilterCoefficient_fc3 ) ? TDSL1_GSM_FILTER_COEF_3 :\
  ((fc == tdsrrc_FilterCoefficient_fc4 ) ? TDSL1_GSM_FILTER_COEF_4 :\
  ((fc == tdsrrc_FilterCoefficient_fc5 ) ? TDSL1_GSM_FILTER_COEF_5 :\
  ((fc == tdsrrc_FilterCoefficient_fc6 ) ? TDSL1_GSM_FILTER_COEF_6 :\
  ((fc == tdsrrc_FilterCoefficient_fc7 ) ? TDSL1_GSM_FILTER_COEF_7 :\
  ((fc == tdsrrc_FilterCoefficient_fc8 ) ? TDSL1_GSM_FILTER_COEF_8 :\
  ((fc == tdsrrc_FilterCoefficient_fc9 ) ? TDSL1_GSM_FILTER_COEF_9 :\
  ((fc == tdsrrc_FilterCoefficient_fc11 ) ? TDSL1_GSM_FILTER_COEF_11 :\
  ((fc == tdsrrc_FilterCoefficient_fc13 ) ? TDSL1_GSM_FILTER_COEF_13 :\
  ((fc == tdsrrc_FilterCoefficient_fc15 ) ? TDSL1_GSM_FILTER_COEF_15 :\
  ((fc == tdsrrc_FilterCoefficient_fc17 ) ? TDSL1_GSM_FILTER_COEF_17 :\
  (fc == tdsrrc_FilterCoefficient_fc19 ) ? TDSL1_GSM_FILTER_COEF_19 : TDSL1_GSM_FILTER_COEF_0 \
  ))))))))))))) \
)

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
#define TDSRET_EUTRA_FILTER_COEFFICIENT(fc)\
(  \
 (fc == tdsrrc_FilterCoefficient_fc0 ) ? TDSL1_EUTRA_FILTER_COEF_0 : \
  ((fc == tdsrrc_FilterCoefficient_fc1 ) ? TDSL1_EUTRA_FILTER_COEF_1 :\
  ((fc == tdsrrc_FilterCoefficient_fc2 ) ? TDSL1_EUTRA_FILTER_COEF_2 :\
  ((fc == tdsrrc_FilterCoefficient_fc3 ) ? TDSL1_EUTRA_FILTER_COEF_3 :\
  ((fc == tdsrrc_FilterCoefficient_fc4 ) ? TDSL1_EUTRA_FILTER_COEF_4 :\
  ((fc == tdsrrc_FilterCoefficient_fc5 ) ? TDSL1_EUTRA_FILTER_COEF_5 :\
  ((fc == tdsrrc_FilterCoefficient_fc6 ) ? TDSL1_EUTRA_FILTER_COEF_6 :\
  ((fc == tdsrrc_FilterCoefficient_fc7 ) ? TDSL1_EUTRA_FILTER_COEF_7 :\
  ((fc == tdsrrc_FilterCoefficient_fc8 ) ? TDSL1_EUTRA_FILTER_COEF_8 :\
  ((fc == tdsrrc_FilterCoefficient_fc9 ) ? TDSL1_EUTRA_FILTER_COEF_9 :\
  ((fc == tdsrrc_FilterCoefficient_fc11 ) ? TDSL1_EUTRA_FILTER_COEF_11 :\
  ((fc == tdsrrc_FilterCoefficient_fc13 ) ? TDSL1_EUTRA_FILTER_COEF_13 :\
  ((fc == tdsrrc_FilterCoefficient_fc15 ) ? TDSL1_EUTRA_FILTER_COEF_15 :\
  ((fc == tdsrrc_FilterCoefficient_fc17 ) ? TDSL1_EUTRA_FILTER_COEF_17 :\
  (fc == tdsrrc_FilterCoefficient_fc19 ) ? TDSL1_EUTRA_FILTER_COEF_19 : TDSL1_EUTRA_FILTER_COEF_0 \
  ))))))))))))) \
)

#define TDSRET_EUTRA_MEAS_QUAN(meas_quan)\
( \
  (meas_quan == tdsrrc_MeasurementQuantityEUTRA_rrsp) ? TDSL1_EUTRA_QUAN_RSRP : TDSL1_EUTRA_QUAN_RSRQ \
)
#endif


/* RSSI high and Low values */
#define TDSRSSI_VAL_HIGH -48
#define TDSRSSI_VAL_LOW -110

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
extern tdsrrcmeas_substate_e_type tdsrrcmeas_current_substate;
extern tdsl1_meas_sib_parm_struct_type *tdsrrcmeas_sib11;
extern uint8 tdsrrcmeas_pending_rpt;
/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Local Object Definition
** ----------------------------------------------------------------------- */


/*-------------------------------------------------------------------
STRUCTURE: tdsinter_rat_cell_list_struct_type
Stores the inter rat cell info list [CELL_INFO_LIST variable]
--------------------------------------------------------------------*/
tdsinter_rat_cell_list_struct_type  tdsinter_rat_cell_list[TDSL1_MAX_CELL_MEAS];

/* Used for manipulation of Local Neighbours given in Meas Ctrl Msg */
tdsinter_rat_cell_list_struct_type tdslocal_neighbor_list[TDSL1_MAX_CELL_MEAS];

/* For storing temporary meas results with cells marked for deletion */
tdsl1_measured_cells_struct_type tdstmp_measured_cells[TDSL1_MAX_REPORTED_GSM_CELLS];

/* Declared to store new intra freq cell list that is being built */
static tdsl1_inter_sys_cell_list_struct_type inter_sys_sib12_new_cell_list;

/* for storing interRAT cell info ind present in MCM */
tdsinterRAT_cell_info_status_type tdsinterRAT_cell_info_status;


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */

/* Maximum No of Inter RAT measurements based on additional meas */
tdsinter_rat_meas_id_list_struct_type tdsinter_rat_meas_id_list[TDSMAX_INTER_RAT_MEAS];

/*Boolean added to track if interfrequency and interrat measurements are to be processed or not*/
extern boolean tdsprocess_interfreq_interrat_meas_rel7;



#ifdef FEATURE_TDSCDMA_TO_LTE
  /*Frequencyl list*/
  #define RRC_INVALID_EUTRA_FREQ 0xFFFFFFFF
  tdsrrcirat_eutra_freq_type tdsrrcirat_eutra_freq_list[TDSL1_MAX_EUTRA_FREQ];
  tdsrrcirat_eutra_freq_type *tdsrrcirat_eutra_freq_list_ptr = tdsrrcirat_eutra_freq_list;
#endif

  extern byte rr_get_maximum_power_output_for_frequency( ARFCN_T frequency );

/* =======================================================================
**                            Function Definitions
** ======================================================================= */

#ifdef FEATURE_GSM_QUADBAND

sys_band_T tdsconverted_gsm_band;

sys_band_T tdsrrcirat_get_gsm_band(tdsl1_gsm_cell_info_type gsm_info)
{
  if (gsm_info.band_indicator == TDSL1_DCS_1800_BAND)
  {
    return(tdsrr_get_arfcn_band_internal(gsm_info.bcch_arfcn.num, TDSRR_DCS_1800));
  }
  else
  {
    return(tdsrr_get_arfcn_band_internal(gsm_info.bcch_arfcn.num, TDSRR_PCS_1900));
  }
}
#endif /* FEATURE_GSM_QUADBAND */

#ifdef FEATURE_ENH_BSIC_VERIFY
boolean tdsrrcirat_get_cell_id_from_rat_cell_list(uint16 arfcn, uint16 * cell_id_ptr)
{
   int i = 0;
   boolean ret = FALSE;

   if (cell_id_ptr)    
   {
      for (i = 0; i < TDSL1_MAX_CELL_MEAS; i++)
      {
         if (tdsinter_rat_cell_list[i].cell_position == TDSOCCUPIED)
         {
            if (tdsinter_rat_cell_list[i].arfcn == arfcn)
            {
               *cell_id_ptr = tdsinter_rat_cell_list[i].cell_id;
               ret = TRUE;
               break;
            }
         }
      }
   }

   return ret;
}

boolean tdsrrcirat_get_next_cell_id_from_rat_cell_list(uint16 arfcn, uint16 curr_cid, uint16 * cell_id_ptr)
{
   int i = 0;
   boolean ret = FALSE;
   uint16 cid_list[TDSL1_MAX_CELL_MEAS];
   uint8 cid_len = 0;

   if (cell_id_ptr)    
   {
      /* Get the ID list for the ARFCN */
      for (i = 0; i < TDSL1_MAX_CELL_MEAS; i++)
      {
         if (tdsinter_rat_cell_list[i].cell_position == TDSOCCUPIED)
         {
            if (tdsinter_rat_cell_list[i].arfcn == arfcn)
            {
               cid_list[cid_len] = tdsinter_rat_cell_list[i].cell_id;
               cid_len++;
            }
         }
      }

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Repeated cell_id for arfcn %d,num of cells %d",arfcn,cid_len);
      
      /* Search for current CID position in the cid_list */
      if (cid_len > 1)
      {
         for (i = 0; i < cid_len; i++)
         {
            if (curr_cid == cid_list[i])
            {
               if (i < cid_len - 1)
               {
                  *cell_id_ptr = cid_list[i+1];
                  ret = TRUE;
               }
               break;
            }
         }
      }
   }

   return ret;
}

boolean tdsrrcirat_meas_id_is_bsic_verified(uint16 meas_id)
{
   int count;
   boolean ret = FALSE;

   for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
   {
      /* First check if this meas already exists */
      if ((tdsinter_rat_meas_id_list[count].meas_id == meas_id) &&
          (tdsinter_rat_meas_id_list[count].interrat_type == TDSRRCMEAS_IRAT_GSM))
      {
         ret = tdsinter_rat_meas_id_list[count].rpt_bsic_verified;
         break;
      }
   }

   return ret;
}
#endif

/*===========================================================================

FUNCTION   IRAT_READ_SIB11

DESCRIPTION

  This function reads Inter RAT Cell List from SIB 11 and
  stores it in tdsinter_rat_meas_id_list. This Inter RAT
  Cell List is used for cell reselection. When UTRAN sends a meas
  in connected DCH mode, this cell list is overwritten.

DEPENDENCIES

  Pointer to tdsrrc_InterRATCellInfoList_B must be valid.

RETURN VALUE

  FAILURE: If cell list is not in order
  SUCCESS: If able to analyze and fill Cell Info.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_read_sib11
(
  tdsl1_meas_sib_parm_struct_type* l1_inter_rat_sib_ptr
)
{
  tdsrrc_SysInfoType11 *sib11_ptr = NULL; /* Declared to store the value of SIB 11*/

  tdsrrc_SysInfoType3  *sib3_ptr = NULL;

  tdsrrc_SysInfoType4  *sib4_ptr = NULL;

  tdsrrc_NewInterRATCellList_B* inter_rat_cell_list_ptr = NULL;

  uint8 cell_count = 0;

  uint16 cell_position;

  boolean sib4_present = FALSE;

  tdsrrc_SysInfoType18 *sib18_ptr = NULL; /* Declared to store the value of SIB 18*/
  tdsrrc_PLMNsOfInterRATCellsList *tmp_rrc_PLMNsOfInterRATCellsList_ptr = NULL;
  rrc_plmn_identity_type tmp_plmn_identity;
  rrc_plmn_identity_type selected_plmn_identity;
  tdsrrc_int_cell_id_type  tmp_cell_id;
  boolean sib18_present = FALSE;

  tdsrrc_FACH_MeasurementOccasionInfo_inter_RAT_meas_ind * local_sib_inter_rat_ptr = NULL;

  tdsrrc_SysInfoType11_v590ext_IEs *sib11_rel5_ext_ptr = NULL;
  tdsrrc_SysInfoType11_v590ext_IEs_newInterRATCellInfoList_v590ext * rel5_ext_ptr = NULL;

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  uint16 prev_cell_count =0;
#endif
  uint32 idx=0,idy=0;
  uint16 arfcn;

  /* First check if dual mode is enabled */
  if (tdsrrcmcm_is_dualmode_enabled() == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IRAT:Dual Mode not enabled");
    return FAILURE;
  }

  if(!tdsprocess_interfreq_interrat_meas_rel7)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip Processing interRAT SIB11 measurements");
    l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = FALSE;
    return FAILURE;
  }

  sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB3);
  if( sib3_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB3 ptr is NULL");
    return( FAILURE );
  }

  if (((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH) || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)) &&
    ((sib3_ptr->sib4indicator == TRUE)))
  {
    sib4_ptr = (tdsrrc_SysInfoType4 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB4);

    if (sib4_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB4 ptr NULL.Use SIB3");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Use default values from SIB4");
      sib4_present = TRUE;
    }
  }

  tmp_plmn_identity.mcc[0] =  tmp_plmn_identity.mcc[1] = tmp_plmn_identity.mcc[2] = 0;
  tmp_plmn_identity.num_mnc_digits = 0;
  tmp_plmn_identity.mnc[0] =  tmp_plmn_identity.mnc[1] = 0;
   /* SIB18 */
  if((sib18_ptr = (tdsrrc_SysInfoType18*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB18)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    if(((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)))
    {
      /*
       1>if System Information Block type 11bis is scheduled on BCH:
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities for SIB 11bis"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Idle mode PLMN identities for SIB11bis";
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Connected mode PLMN identities"; 
       2>otherwise, 
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
       
       1> else
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
      */
      if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,
          connectedModePLMNIdentitiesSIB11bis)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis,
            rrc_plmnsOfInterRATCellsList)) &&
          (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            connectedModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Conn Inter RAT from bis extension ");
        sib18_present = TRUE;
      }
      else if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,idleModePLMNIdentitiesSIB11bis)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
            plmnsOfInterRATCellsList)) &&
          (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 idle Inter RAT from bis extension ");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,connectedModePLMNIdentities)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterRATCellsList)) &&
        (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Conn Inter RAT");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterRATCellsList)) &&
        (sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter RAT");
        sib18_present = TRUE;
      }
    }
    else
    {
   
      /*
       If SIB11bis is present and SIB18 bis extenstions are present, use SIB18 bis extension
       If SIB11bis is present and SIb18 bis extensions are absent, use SIB18
       If SIB11bis is absent, use SIB18
      */
      if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,idleModePLMNIdentitiesSIB11bis)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
          plmnsOfInterRATCellsList)) &&
        (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Idle Inter RAT from bis extension ");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities))
        &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterRATCellsList))
        &&
        (sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter RAT ");
        sib18_present = TRUE;
      }
    }
#else
    if(((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,connectedModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterRATCellsList))
      &&
      (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
    {
       tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList;
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 process SIB18 Conn Inte&r RAT");
       sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterRATCellsList))
      &&
      (sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 process SIB18 Idle Inter RAT");
      sib18_present = TRUE;
    }
#endif
  }
  if(tdsrrcccm_get_serv_plmn_cellid(&tmp_cell_id, &selected_plmn_identity) == TDSRRC_CELL_LOSS_NO_NEW_CELL)
  {
    sib18_present = FALSE;
  }
  else
  {
    memscpy(&tmp_plmn_identity, sizeof(rrc_plmn_identity_type), &selected_plmn_identity, sizeof(rrc_plmn_identity_type));
  }

  if(tdsrrccsp_is_ue_in_true_limited_service() == TRUE)
  {
    sib18_present = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE in limited service ignoring SIB18");
  }
  {
    tdsrrc_MasterInformationBlock *mib_ptr = NULL;/* Declared to store the value of MIB*/
    if((mib_ptr = (tdsrrc_MasterInformationBlock*)  tdsrrcsibdb_return_sib_for_srv_cell
      (tdsrrc_MIB))!= NULL)
    {
      if(tdsrrc_is_nw_shared(mib_ptr))
      {
        sib18_present = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Ignore SIB18");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: N/W not shader so consider SIB18");
      }
    }
  }




  sib11_ptr = (tdsrrc_SysInfoType11*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB11);

  /* Check if the value received is O.K. */

  if ( sib11_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 11 from SIB DB is NULL");
    return FAILURE;
  }
  else
  {

    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
    {
       /* Check for REL 5 extension ptr */
      if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib11_ptr,tdsrrc_SysInfoType11,v4b0NonCriticalExtensions)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE(sib11_ptr->v4b0NonCriticalExtensions,
        tdsrrc_SysInfoType11_v4b0NonCriticalExtensions,v590NonCriticalExtension)))
      {
        sib11_rel5_ext_ptr = &(sib11_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
          sysInfoType11_v590ext);
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib11_rel5_ext_ptr,tdsrrc_SysInfoType11_v590ext_IEs,newInterRATCellInfoList_v590ext))
        {
          rel5_ext_ptr = &sib11_rel5_ext_ptr->newInterRATCellInfoList_v590ext;
        }
      }
    }
    /* Check if the FACH Measurement Occassion Info if present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib11_ptr,
        tdsrrc_SysInfoType11,fach_MeasurementOccasionInfo))
    {
      l1_inter_rat_sib_ptr->fach_meas_incl = TRUE;

      /* Initialize the following value to Default values */
      l1_inter_rat_sib_ptr->fach.k_ind = FALSE;
      l1_inter_rat_sib_ptr->fach.inter_f_ind = FALSE;
      l1_inter_rat_sib_ptr->fach.num_inter_sys = 0;

      /* Check if fACH_meas_occasion_coeff present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib11_ptr->fach_MeasurementOccasionInfo,
        fACH_meas_occasion_coeff))
      {
        /* Set the flag */
        l1_inter_rat_sib_ptr->fach.k_ind = TRUE;
        l1_inter_rat_sib_ptr->fach.k =
          sib11_ptr->fach_MeasurementOccasionInfo.fACH_meas_occasion_coeff;
      }

      /* Check if Inter Freq indicator present */
      if ((sib11_ptr->m.v4b0NonCriticalExtensionsPresent) && 
           (sib11_ptr->v4b0NonCriticalExtensions.m.sysInfoType11_v4b0extPresent) &&
             (sib11_ptr->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.m.
                 fach_MeasurementOccasionInfo_LCR_ExtPresent) &&
              (sib11_ptr->v4b0NonCriticalExtensions.sysInfoType11_v4b0ext.
                   fach_MeasurementOccasionInfo_LCR_Ext.inter_freq_TDD128_meas_ind == TRUE))
      {
        l1_inter_rat_sib_ptr->fach.inter_f_ind = TRUE;
      }

      /* Check if Inter RAT indicator present */
      local_sib_inter_rat_ptr = &sib11_ptr->fach_MeasurementOccasionInfo.inter_RAT_meas_ind;
      idx=0;
      while (local_sib_inter_rat_ptr->n > idx)
      {
        if (local_sib_inter_rat_ptr->elem[idx] == tdsrrc_RAT_Type_gsm)
        {
          l1_inter_rat_sib_ptr->fach.num_inter_sys = 1;
          l1_inter_rat_sib_ptr->fach.inter_sys[0] = TDSL1_GSM_SYSTEM;
          break;
        }
        idx++;
      }
    }

    /* Check if HCS is in use. */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(sib11_ptr->measurementControlSysInfo.use_of_HCS,
         rrc_MeasurementControlSysInfo_use_of_HCS_hcs_used))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"HCS=>TRUE");
      if(!(sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->m.
        interRATMeasurementSysInfoPresent))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Sys info absent in SIB11");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(tdsrrcirat_read_sib11bis(l1_inter_rat_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterRATCellsList_ptr)== FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter RAT in SIB11bis Ignored");
          l1_inter_rat_sib_ptr->inter_s.num_cell_add = 0;
          l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
          return FAILURE;
        }
        return SUCCESS;
#else
        return FAILURE;
#endif

      }
      if (tdsrrcirat_fill_hcs_info(&sib11_ptr->measurementControlSysInfo.use_of_HCS.u.
        hcs_used->interRATMeasurementSysInfo, l1_inter_rat_sib_ptr, rel5_ext_ptr) == SUCCESS)
      {
        l1_inter_rat_sib_ptr->hcs_used = TRUE;
        return SUCCESS;
      }
      else
        return FAILURE;
    }
    else
    { /* HCS is NOT in use */
      if(!(sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->m.
         interRATMeasurementSysInfoPresent))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Sys info absent in SIB11");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(tdsrrcirat_read_sib11bis(l1_inter_rat_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterRATCellsList_ptr)== FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter RAT in SIB11bis Ignored");
          l1_inter_rat_sib_ptr->inter_s.num_cell_add = 0;
          l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
          return FAILURE;
        }
        return SUCCESS;
#else
        return FAILURE;
#endif
      }

      if (!(TDSRRC_MSG_COMMON_BITMASK_IE(sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->
        interRATMeasurementSysInfo,tdsrrc_InterRATMeasurementSysInfo_B,interRATCellInfoList)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT list absent in SIB11");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(tdsrrcirat_read_sib11bis(l1_inter_rat_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterRATCellsList_ptr)== FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter RAT in SIB11bis Ignored");
          l1_inter_rat_sib_ptr->inter_s.num_cell_add = 0;
          l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
          return FAILURE;
        }
        return SUCCESS;
#else
        return FAILURE;
#endif

      }

      /* Store ptr given by UTRAN */
      inter_rat_cell_list_ptr = &sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->
        interRATMeasurementSysInfo.interRATCellInfoList.newInterRATCellList;

      if (inter_rat_cell_list_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"inter_rat_cell_list_ptr is NULL");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(tdsrrcirat_read_sib11bis(l1_inter_rat_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterRATCellsList_ptr)== FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter RAT in  SIB11bis Ignored");
          l1_inter_rat_sib_ptr->inter_s.num_cell_add = 0;
          l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
          return FAILURE;
        }
        return SUCCESS;
#else
        return FAILURE;
#endif
      }
      
      idx=idy=0;
      while ( (inter_rat_cell_list_ptr->n > idx) &&
        (cell_count < TDSL1_MAX_CELL_MEAS) )
      {
      /* Let first check the type of the cell to find if we
        * really need to give this cell to L1 */
        if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.t !=
          T_tdsrrc_NewInterRATCell_B_technologySpecificInfo_gsm)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring Non GSM cell");
          arfcn = TDSMAX_ARFCN;
          if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
          {
            /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
            cell_position = (uint16)
              inter_rat_cell_list_ptr->elem[idx].interRATCellID;
          }
          else
          {
            /* Store the cell in first order position */
            cell_position = TDSAVAILABLE_CELL_POSITION;
          }
          /* Tell to get the first available cell position and
          * that would be the cell id. The called function would store
          * the cell id back in passed cell_position variable */
          if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
            return FAILURE;
          }
          idx++;
          continue;
        }
        else
        {
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].cell_info_type = TDSL1_GSM_SYSTEM;

          /* Cell reselection info is always set to TRUE. If not present in SIB 11, just
          set it to default values */
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info_incl = TRUE;

          /* Initializw q-offset1-2 Defaults as per specs */
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.qoffset1_s_n = TDSQOFFSET1;

           /* -24. Got this value from Radio Access Capability */
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.max_tx_pwr = -24;

            /* Not valid for Inter-RAT but initialize to default
          * as they are present */
          if ((sib4_present == TRUE) && (sib4_ptr != NULL))
          {
            if (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.t !=
                T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"unsupprt FDD mode");
              return(FAILURE);
            }
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.rxlev_min = (int16)
              (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
          }
          else
          {
            if (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.t !=
                T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"unsupprt FDD mode");
              return(FAILURE);
            }

            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.rxlev_min = (int16)
              (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
          }
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.hcs_info.hcs_prio = 0;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.q_hcs = 110;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.hcs_info.penalty_time = 0;

          /* Get GSM CELL Info */
          /* Check if rrc_gsm_cellSelectionReselectionInfo_present */
          if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->m.
            cellSelectionReselectionInfoPresent)
          {
            TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Reselection info present");

            /* q-Offset1S-N */
          
              /* Range is -50...50 */
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                gsm_cell.cell_sel_info.qoffset1_s_n = (int16)
                inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.q_Offset1S_N;
            


            /* Max Txit Power */
            if (TDSRRC_MSG_COMMON_BITMASK_IE(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->cellSelectionReselectionInfo,
              tdsrrc_CellSelectReselectInfoSIB_11_12,maxAllowedUL_TX_Power))
            {
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                gsm_cell.cell_sel_info.max_tx_pwr = (int16)
                inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.maxAllowedUL_TX_Power;
            }

            /* No need to Check if gsm mode present as it has already been done */

            /* Check if rxlev_min present */

            if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.modeSpecificInfo.u
              .gsm->m.q_RxlevMinPresent)
            {
              /* Store rxlev_min in L1 */
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min = (int16)
                ((inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.modeSpecificInfo.u.gsm->q_RxlevMin * 2) + 1);
            }
            /*l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
              delta_qrxlevmin_included = FALSE;  */
            if(rel5_ext_ptr != NULL)
            {
              if(TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(rel5_ext_ptr,deltaQrxlevmin))
              {
                /*l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
                  delta_qrxlevmin_included = TRUE;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
                  delta_qrxlevmin = rel5_ext_ptr->value.deltaQrxlevmin * 2; */
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Delta-Qrxlev=%d, Qrxlev=%d, ARFCN=%d",
                  (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2),
                  l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min,
                  CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn));

                /*if ((l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +
                     (rel5_ext_ptr->value.deltaQrxlevmin * 2)) < -115)
                {*/
                  l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +=
                     (int16) (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2);
                /*}*/
              }
            }

          } /* rrc_gsm_cellSelectionReselectionInfo_present */

          /* Cell Individual Offset */
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.cio = (int16)
            inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
            u.gsm->interRATCellIndividualOffset;

          /* Get BAND 1800 DCS or 1900 PCS */
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator =
            TDSRET_INTER_RAT_BAND(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
            u.gsm->frequency_band);

          /* Get bcch_ARFCN */
          CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn) =
            (uint16) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
            u.gsm->bcch_ARFCN;

          arfcn =(uint16) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bcch_ARFCN;

#ifdef FEATURE_GSM_QUADBAND

/* for SGLTE, don't need to know band enable/disable at G, just do band convertion */
#ifdef FEATURE_SGLTE
          if (tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
          {
            geran_grr_band_ind_e grr_band_ind = (l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator == TDSL1_DCS_1800_BAND) ?
				                                 GERAN_GRR_BAND_IND_DCS_1800 : GERAN_GRR_BAND_IND_PCS_1900;
			ARFCN_T converted_arfcn = geran_grr_convert_to_arfcn_type(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num, grr_band_ind);
            tdsconverted_gsm_band = converted_arfcn.band;
			TDSRRC_MSG2(MSG_LEGACY_HIGH,"SGLTE: arfcn %d,converted_gsm_band %d",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num,tdsconverted_gsm_band);
		  }
		  else
#endif /* FEATURE_SGLTE */
          {
            tdsconverted_gsm_band = tdsrrcirat_get_gsm_band(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info);
          }

		  if (tdsconverted_gsm_band != INVALID_BAND)
          {
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.band =
              tdsconverted_gsm_band;
          }
          else
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"ARFCN %d ignored bec of invalid band",inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
              u.gsm->bcch_ARFCN,0,0);
            arfcn = TDSMAX_ARFCN;
            /* Increment the pointer */
            if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
            {
              /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
              cell_position = (uint16)
                inter_rat_cell_list_ptr->elem[idx].interRATCellID;
            }
            else
            {
              /* Store the cell in first order position */
              cell_position = TDSAVAILABLE_CELL_POSITION;
            }
            /* Tell to get the first available cell position and
            * that would be the cell id. The called function would store
            * the cell id back in passed cell_position variable */
            if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
              return FAILURE;
            }
            idx++;
            continue;
          }
#endif /* FEATURE_GSM_QUADBAND */

          /* To add BSIC (Base Station Identification Code) */
          if ((inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc
            > TDSNCC_MAX) ||
            (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc
            > TDSBCC_MAX))
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"BCC %d or NCC %d invalid[0..7]",inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc,inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc);

            return FAILURE;

          } /* End of if of checking NCC and BCC validity */

          /* Store good values of bcc and ncc */
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.ncc =
            (uint8) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
            u.gsm->bsic.ncc;

          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.bcc =
            (uint8) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
            u.gsm->bsic.bcc;
        } /* End of gsm cell  */

        /* Time to get its cell id*/
        /* Cell Order has already been verified */

        if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
        {
          /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
          cell_position = (uint16)
            inter_rat_cell_list_ptr->elem[idx].interRATCellID;

        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
          /* Store the cell in first order position */
          cell_position = TDSAVAILABLE_CELL_POSITION;
        }
        /* Tell to get the first available cell position and
        * that would be the cell id. The called function would store
        * the cell id back in passed cell_position variable */
        if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
          return FAILURE;
        }
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id
          = cell_position;
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].
          measure_in_idle_pch_fach = TRUE;

        if(sib18_present)
        {
          if(tmp_rrc_PLMNsOfInterRATCellsList_ptr->n > idy)
          {
            if(tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[idy].m.plmn_IdentityPresent)
            {
              tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[idy].plmn_Identity,
                &tmp_plmn_identity);
              if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
              {
                if((tdsrrccsp_check_for_eplmn(
                  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                  tmp_plmn_identity))) == FALSE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d Non eq PLMN measure only in DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
                  /* set the flag to FALSE */
                  l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
                }
              }
            }
            else
            {
              /* use the previous value of the flag if PLMN id is not selected PLMN */
              if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
              {
                /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
                if(cell_count > 0)
                {
                  l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach =
                    l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].measure_in_idle_pch_fach;
                }
              }
            }

            idy++;
          }
          else
          {
            /* PLMN list is shorter than the cell info list set flag to true for the remaining cells */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d measured in IDLE,FACH,PCH,DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
          }
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d measured in IDLE,FACH,PCH,DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
        }
        /* Increment cell count */
        cell_count++;

        /* Increment the pointer */
        idx++;
        if ((inter_rat_cell_list_ptr->n > idx) && (cell_count == TDSL1_MAX_CELL_MEAS))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List full");
          break;
        }

      } /* End of while inter_rat_cell_list_ptr */

      /* Finally Set the cell count */
      l1_inter_rat_sib_ptr->inter_s.num_cell_add = cell_count;

      /* Now set inter sys info incl info to TRUE */
      l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
      l1_inter_rat_sib_ptr->hcs_used = FALSE;

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      prev_cell_count = l1_inter_rat_sib_ptr->inter_s.num_cell_add;
      if(tdsrrcirat_read_sib11bis(l1_inter_rat_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterRATCellsList_ptr) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading Sib11bis FAILURE");
        l1_inter_rat_sib_ptr->inter_s.num_cell_add = prev_cell_count;
      }
#endif


      TDSRRC_MSG0(MSG_LEGACY_HIGH,"IRAT CELL INFO = FALSE");
      tdsinterRAT_cell_info_status.interRAT_cell_info_present = FALSE;
      tdsinterRAT_cell_info_status.interRAT_cell_info_ind = 0;

      if (TDSRRC_CHECK_COMMON_MSG_TYPE(sib11_ptr->measurementControlSysInfo.use_of_HCS,
           rrc_MeasurementControlSysInfo_use_of_HCS_hcs_used ))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"HCS=>TRUE");
        if(sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->m.
            interRATMeasurementSysInfoPresent)
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE(sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->interRATMeasurementSysInfo,
           tdsrrc_InterRATMeasurementSysInfo,interRATCellInfoList))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IRAT CELL INFO = TRUE");
            tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
            tdsinterRAT_cell_info_status.interRAT_cell_info_ind = 0;
            return SUCCESS;
          }
        }
      }
      else
      {
        if(sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->m.
          interRATMeasurementSysInfoPresent)
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE(sib11_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->
            interRATMeasurementSysInfo,tdsrrc_InterRATMeasurementSysInfo_B,interRATCellInfoList))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"IRAT CELL INFO = TRUE");
            tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
            tdsinterRAT_cell_info_status.interRAT_cell_info_ind = 0;
            return SUCCESS;
          }
        }
      }



      return SUCCESS;

    } /* End of else of HCS not in use */


  } /* End of else of sib11_ptr = NULL */

}

/*===========================================================================

FUNCTION tdsrrcirat_process_remove_cell_list

DESCRIPTION

 This function processes the remove cell list of SIB12 and updates the inter
 rat cell list

DEPENDENCIES

 SIB12 must have been received by the camping cell.

RETURN VALUE
 None

SIDE EFFECTS

 None

===========================================================================*/
void tdsrrcirat_process_remove_cell_list
(
tdsrrc_RemovedInterRATCellList *removedInterRATCellList,
uint16 *num_removed_cells
)
{
  uint32 inter_rat_cell_id;

  uint32 idx=0;

  *num_removed_cells = 0;
  switch(removedInterRATCellList->t)
  {
    case T_tdsrrc_RemovedInterRATCellList_removeAllInterRATCells:
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"remove all cells from inter rat cells list");
         *num_removed_cells = TDSL1_MAX_CELL_MEAS;
         tdsrrcirat_initialize_cell_list();
      break;

    case T_tdsrrc_RemovedInterRATCellList_removeSomeInterRATCells:
         if((removedInterRATCellList->u.removeSomeInterRATCells == NULL)
            ||(removedInterRATCellList->u.removeSomeInterRATCells->n == 0))
         {
           TDSRRC_MSG0(MSG_LEGACY_LOW,"Assign no of removed cells to be 0");
         }
         else
         {
          
           do
           {
             inter_rat_cell_id = removedInterRATCellList->u.removeSomeInterRATCells->elem[idx];

             /* Remove cell at position given by the cell id from the inter rat cell info list */
             if(inter_rat_cell_id < TDSL1_MAX_CELL_MEAS)
             {
               tdsinter_rat_cell_list[inter_rat_cell_id].cell_position = TDSVACANT;
               (*num_removed_cells)++; /* Increment count */
             }

             /*  Get next element in the list */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"removed  cell id %d from inter rat cell list",inter_rat_cell_id);
            idx++;

           } /* Continue if cond satisfied */
           while ((removedInterRATCellList->u.removeSomeInterRATCells->n > idx) &&
            ((*num_removed_cells) < TDSL1_MAX_CELL_MEAS));
         }
      break;

    case T_tdsrrc_RemovedInterRATCellList_removeNoInterRATCells:
         /* no cells need to be removed */
         TDSRRC_MSG0(MSG_LEGACY_LOW,"Assign no of removed cells to be 0");
      break;

    default:
      /* cannot come here */
      break;
  }
}

/*===========================================================================

FUNCTION tdsrrcirat_build_composite_cell_list_from_SIB11_SIB12

DESCRIPTION

 This function builds a composite cell info list from SIB11 and SIB12 that is
 then sent down to L1

DEPENDENCIES

 SIB12 must have been received by the camping cell.

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcirat_build_composite_cell_list_from_SIB11_SIB12
(
tdsrrc_RemovedInterRATCellList *removedInterRATCellList,
tdsl1_inter_sys_cell_list_struct_type* sib11_cell_list_ptr,
tdsl1_inter_sys_cell_list_struct_type* sib12_cell_list_ptr,
uint16 num_removed_cells
)
{
/* Initialize all variables */

  /* Stores no of intra-freq cells stored in SIB11 */
  uint32 sib11_added_cells = 0;

  /* Stores no of intra-freq cells stored in SIB12 */
  uint32 sib12_added_cells = 0;

  /* Counts no of cells in SIB12 */
  uint32 sib12_count_cells = 0;

  /* Counts removed cells matched against new inter rat cells of SIB11 */
  uint32 sib12_r_cell = 0;

  /* Counts no of new intra freq cells in SIB11 and helps in comparing
  * with no fo removed cells in SIB12 */
  uint32 sib11_a_cell = 0;

  /* If removed cell is found */
  uint32 match_found = 0;

  uint16 new_cell_list = 0;
  uint16 new_cell_list_from_sib11 = 0;
  uint16 count = 0;



  /* Stores ptr to SIB12 list */
  tdsl1_inter_sys_cell_list_struct_type* new_cell_list_ptr = NULL;

  sib11_added_cells = sib11_cell_list_ptr->num_cell_add;
  /* initialize tmp cell list to satisfy lint */

  if(removedInterRATCellList->t == T_tdsrrc_RemovedInterRATCellList_removeAllInterRATCells)
  {
    /* No cells need to be picked up from SIB 11 */
    sib12_cell_list_ptr->num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
    return;
  }
  /* First remove no of removed cells in SIB12 from SIB11 and store result
  * in a temporary list i.e. inter_sys_sib12_new_cell_list */


  /* Criteria: Match removed cell of SIB12 against all cells of SIB11
  * one at a time */

  for ( sib11_a_cell=0 ; sib11_a_cell<sib11_added_cells; sib11_a_cell++ )
  {
    
    count =0;
    for ( sib12_r_cell=0; sib12_r_cell <num_removed_cells; sib12_r_cell++ )
    {
      if(removedInterRATCellList->t == T_tdsrrc_RemovedInterRATCellList_removeSomeInterRATCells)
      {
        if((removedInterRATCellList->u.removeSomeInterRATCells != NULL)
            && (removedInterRATCellList->u.removeSomeInterRATCells->n > count))
            
        {
          if(removedInterRATCellList->u.removeSomeInterRATCells->elem[count] !=
            sib11_cell_list_ptr->add_cell[sib11_a_cell].intersys_cell_id)
          {
            count++;
            continue; /* Read next value of removed cell*/
          }
        }
        match_found = 1;
        break;
      }
      if(match_found == 1)
      {
         /* Do not add this cell to the composite list */
         break;
      }
    }  /* End of for for sib12_r_cell */

    if ( match_found == 0 )
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Copying SIB 11 cell back in new list");

      inter_sys_sib12_new_cell_list.add_cell[new_cell_list].intersys_cell_id =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].intersys_cell_id;

      inter_sys_sib12_new_cell_list.add_cell[new_cell_list].measure_in_idle_pch_fach =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].measure_in_idle_pch_fach;
      inter_sys_sib12_new_cell_list.add_cell[new_cell_list].cell_info_type =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].cell_info_type;

      inter_sys_sib12_new_cell_list.add_cell[new_cell_list].u.gsm_cell =
        sib11_cell_list_ptr->add_cell[sib11_a_cell].u.gsm_cell;

      /* Increment counter */
      new_cell_list++;

    } /* End of if */

    /* Reinitialize match_found variable */
    match_found = 0;
    /* Read next SIB 11 cell */
  } /* End of for sib11_a_cell */

  new_cell_list_ptr = &inter_sys_sib12_new_cell_list;
  new_cell_list_from_sib11 = new_cell_list;

  /* Check if SIB12 new intra freq cells have id same as the one already present
   * from SIB 11 if so overwrite, if not append to the list */
  for ( sib12_added_cells =0; sib12_added_cells < sib12_cell_list_ptr->num_cell_add && new_cell_list < TDSL1_MAX_CELL_MEAS;
  sib12_added_cells++ )
  {
    for(count = 0; count < new_cell_list_from_sib11; count++)
    {
      if(sib12_cell_list_ptr->add_cell[sib12_added_cells].intersys_cell_id
         == new_cell_list_ptr->add_cell[count].intersys_cell_id)
      {
        new_cell_list_ptr->add_cell[count].measure_in_idle_pch_fach =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].measure_in_idle_pch_fach;
        new_cell_list_ptr->add_cell[count].cell_info_type =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].cell_info_type;

        new_cell_list_ptr->add_cell[count].u.gsm_cell =
          sib12_cell_list_ptr->add_cell[sib12_added_cells].u.gsm_cell;
        break;
      }
    }
    if(count == new_cell_list_from_sib11)
    {
      new_cell_list_ptr->add_cell[new_cell_list].intersys_cell_id =
        sib12_cell_list_ptr->add_cell[sib12_added_cells].intersys_cell_id;

      new_cell_list_ptr->add_cell[new_cell_list].measure_in_idle_pch_fach =
        sib12_cell_list_ptr->add_cell[sib12_added_cells].measure_in_idle_pch_fach;
      new_cell_list_ptr->add_cell[new_cell_list].cell_info_type =
        sib12_cell_list_ptr->add_cell[sib12_added_cells].cell_info_type;

      new_cell_list_ptr->add_cell[new_cell_list].u.gsm_cell =
        sib12_cell_list_ptr->add_cell[sib12_added_cells].u.gsm_cell;

      new_cell_list++;
    }
  } /* End of for */

  /* Store the no of cells */
  sib12_cell_list_ptr->num_cell_add = new_cell_list;

  TDSRRC_MSG1(MSG_LEGACY_LOW,"There are %d cells in SIB12 cell list",new_cell_list);

  /* Now copy the structure in SIB12 */
  for (sib12_count_cells =0; sib12_count_cells<new_cell_list && sib12_count_cells < TDSL1_MAX_CELL_MEAS;
   sib12_count_cells++)
  {

    sib12_cell_list_ptr->add_cell[sib12_count_cells].intersys_cell_id =
      new_cell_list_ptr->add_cell[sib12_count_cells].intersys_cell_id;

    sib12_cell_list_ptr->add_cell[sib12_count_cells].measure_in_idle_pch_fach =
      new_cell_list_ptr->add_cell[sib12_count_cells].measure_in_idle_pch_fach;
    sib12_cell_list_ptr->add_cell[sib12_count_cells].cell_info_type =
      new_cell_list_ptr->add_cell[sib12_count_cells].cell_info_type;

    sib12_cell_list_ptr->add_cell[sib12_count_cells].u.gsm_cell =
      new_cell_list_ptr->add_cell[sib12_count_cells].u.gsm_cell;
  } /* End of for */

    /* Initialize no of removed cells to 0 as SIB12 intra-freq cell info list has
  * already taken them into account */
  sib12_cell_list_ptr->num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
}

/*===========================================================================

FUNCTION tdsrrcirat_read_sib12

DESCRIPTION

 Caller must check the tdssib12_present before calling this fn.
 This function processes SIB12 IEs and build the information
 to be sent down to L1 and updates the cell info list for
 inter rat measurements.

DEPENDENCIES

 SIB12 must have been received by the camping cell.

RETURN VALUE

 SUCCESS/FAIILURE

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_read_sib12
(
tdsrrc_SysInfoType12 *sib12_ptr,
tdsl1_meas_sib_parm_struct_type* l1_inter_rat_sib_ptr
)
{
  tdsrrc_SysInfoType3  *sib3_ptr = NULL;

  tdsrrc_SysInfoType4  *sib4_ptr = NULL;

  tdsrrc_NewInterRATCellList_B* inter_rat_cell_list_ptr = NULL;

  tdsrrc_NewInterRATCellList* hcs_inter_rat_cell_list_ptr = NULL;

  uint8 cell_count = 0;

  uint16 cell_position;

  boolean sib4_present = FALSE;

  uint16 num_removed_cells;
  uint32 idx=0;
  tdsrrc_SysInfoType18 *sib18_ptr = NULL; /* Declared to store the value of SIB 18*/
  tdsrrc_PLMNsOfInterRATCellsList *tmp_rrc_PLMNsOfInterRATCellsList_ptr = NULL;
  rrc_plmn_identity_type tmp_plmn_identity;
  rrc_plmn_identity_type selected_plmn_identity;
  tdsrrc_int_cell_id_type  tmp_cell_id;
  boolean sib18_present = FALSE;


  tdsrrc_FACH_MeasurementOccasionInfo_inter_RAT_meas_ind* local_sib_inter_rat_ptr = NULL;

  tdsrrc_SysInfoType12_v590ext_IEs *sib12_rel5_ext_ptr = NULL;
  tdsrrc_SysInfoType12_v590ext_IEs_newInterRATCellInfoList_v590ext * rel5_ext_ptr = NULL;

  uint16 arfcn;

  /* First check if dual mode is enabled */
  if (tdsrrcmcm_is_dualmode_enabled() == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IRAT:Dual Mode not enabled");
    return FAILURE;
  }

  if(!tdsprocess_interfreq_interrat_meas_rel7)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip Processing interRAT SIB12 measurements");
    l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = FALSE;
    return FAILURE;
  }

  tmp_plmn_identity.mcc[0] =  tmp_plmn_identity.mcc[1] = tmp_plmn_identity.mcc[2] = 0;
  tmp_plmn_identity.num_mnc_digits = 0;
  tmp_plmn_identity.mnc[0] =  tmp_plmn_identity.mnc[1] = 0;

  if((sib18_ptr = (tdsrrc_SysInfoType18*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB18)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      /*
       1>if System Information Block type 11bis is scheduled on BCH:
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities for SIB 11bis"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Idle mode PLMN identities for SIB11bis";
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Connected mode PLMN identities"; 
       2>otherwise, 
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
       
       1> else
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
      */
    if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,connectedModePLMNIdentitiesSIB11bis)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis,
          plmnsOfInterRATCellsList)) &&
        (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
          connectedModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Conn Inter RAT from bis extension ");
      sib18_present = TRUE;
    }
    else if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,idleModePLMNIdentitiesSIB11bis)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
          rrc_plmnsOfInterRATCellsList)) &&
        (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
          idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 idle Inter RAT from bis extension ");
      sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,connectedModePLMNIdentities)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterRATCellsList)) &&
      (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Conn Inter RAT");
      sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities, plmnsOfInterRATCellsList)) &&
      (sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter RAT");
      sib18_present = TRUE;
    }
#else
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,connectedModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterRATCellsList))
      &&
      (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
    {
       tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList;
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB12 process SIB18 Conn Inter RAT");
       sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterRATCellsList))
      &&
      (sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB12 processing SIB18 Idle Inter RAT");
      sib18_present = TRUE;
    }
#endif
  }

  if(tdsrrcccm_get_serv_plmn_cellid(&tmp_cell_id, &selected_plmn_identity) == TDSRRC_CELL_LOSS_NO_NEW_CELL)
  {
    sib18_present = FALSE;
  }
  else
  {
    memscpy(&tmp_plmn_identity, sizeof(rrc_plmn_identity_type), &selected_plmn_identity, sizeof(rrc_plmn_identity_type));
  }

  if(tdsrrccsp_is_ue_in_true_limited_service() == TRUE)
  {
    sib18_present = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE in limited service ignoring SIB18");
  }
  {
    tdsrrc_MasterInformationBlock *mib_ptr = NULL;/* Declared to store the value of MIB*/
    if((mib_ptr = (tdsrrc_MasterInformationBlock*)  tdsrrcsibdb_return_sib_for_srv_cell
      (tdsrrc_MIB))!= NULL)
    {
      if(tdsrrc_is_nw_shared(mib_ptr))
      {
        sib18_present = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Ignore SIB18");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: N/W not shader so consider SIB18");
      }
    }
  }



  sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB3);
  if( sib3_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB3 ptr is NULL");
    return( FAILURE );
  }

  if (((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH) || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)) &&
    ((sib3_ptr->sib4indicator == TRUE)))
  {
    sib4_ptr = (tdsrrc_SysInfoType4 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB4);

    if (sib4_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB4 ptr NULL.Use SIB3");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Use default values from SIB4");
      sib4_present = TRUE;
    }
  }

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    /* Check for REL 5 extension ptr */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib12_ptr,tdsrrc_SysInfoType12,v4b0NonCriticalExtensions)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE(sib12_ptr->v4b0NonCriticalExtensions,
      tdsrrc_SysInfoType12_v4b0NonCriticalExtensions,v590NonCriticalExtension)))
    {
      sib12_rel5_ext_ptr = &(sib12_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.
        sysInfoType12_v590ext);
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib12_rel5_ext_ptr,tdsrrc_SysInfoType12_v590ext_IEs,newInterRATCellInfoList_v590ext))
      {
        rel5_ext_ptr = &sib12_rel5_ext_ptr->newInterRATCellInfoList_v590ext;
      }
    }
  }

  /* Check if the FACH Measurement Occassion Info if present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib12_ptr,tdsrrc_SysInfoType12,fach_MeasurementOccasionInfo))
  {
    l1_inter_rat_sib_ptr->fach_meas_incl = TRUE;

    /* Initialize the following value to Default values */
    l1_inter_rat_sib_ptr->fach.k_ind = FALSE;
    l1_inter_rat_sib_ptr->fach.inter_f_ind = FALSE;
    l1_inter_rat_sib_ptr->fach.num_inter_sys = 0;

    /* Check if fACH_meas_occasion_coeff present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib12_ptr->fach_MeasurementOccasionInfo,fACH_meas_occasion_coeff))
    {
      /* Set the flag */
      l1_inter_rat_sib_ptr->fach.k_ind = TRUE;
      l1_inter_rat_sib_ptr->fach.k =
        sib12_ptr->fach_MeasurementOccasionInfo.fACH_meas_occasion_coeff;
    }

    /* Check if Inter Freq indicator present */
    /* RRC processing IE "Inter-frequency TDD 1.28 Mcps measurement indicator" if it present in v4b0NonCriticalExtensions. */
    if ((sib12_ptr->m.v4b0NonCriticalExtensionsPresent) && 
         (sib12_ptr->v4b0NonCriticalExtensions.m.sysInfoType12_v4b0extPresent) &&
             (sib12_ptr->v4b0NonCriticalExtensions.sysInfoType12_v4b0ext.m.
                 fach_MeasurementOccasionInfo_LCR_ExtPresent) &&
          (sib12_ptr->v4b0NonCriticalExtensions.sysInfoType12_v4b0ext.
               fach_MeasurementOccasionInfo_LCR_Ext.inter_freq_TDD128_meas_ind == TRUE))
    {
      l1_inter_rat_sib_ptr->fach.inter_f_ind = TRUE;
    }
    /* Check if Inter RAT indicator present */
    local_sib_inter_rat_ptr = &sib12_ptr->fach_MeasurementOccasionInfo.inter_RAT_meas_ind;
    idx=0;
    while (local_sib_inter_rat_ptr->n > idx)
    {
      if (local_sib_inter_rat_ptr->elem[idx] == tdsrrc_RAT_Type_gsm)
      {
        l1_inter_rat_sib_ptr->fach.num_inter_sys = 1;
        l1_inter_rat_sib_ptr->fach.inter_sys[0] = TDSL1_GSM_SYSTEM;
        break;
      }
      idx++;
    }
  }
  else if(tdsrrcmeas_sib11->fach_meas_incl == TRUE)
  {
    l1_inter_rat_sib_ptr->fach_meas_incl = TRUE;

    /* Initialize the following value to Default values */
    l1_inter_rat_sib_ptr->fach.k_ind = tdsrrcmeas_sib11->fach.k_ind;
    l1_inter_rat_sib_ptr->fach.inter_f_ind = tdsrrcmeas_sib11->fach.inter_f_ind;
    l1_inter_rat_sib_ptr->fach.k =  tdsrrcmeas_sib11->fach.k;
    l1_inter_rat_sib_ptr->fach.num_inter_sys = tdsrrcmeas_sib11->fach.num_inter_sys;
    l1_inter_rat_sib_ptr->fach.inter_sys[0] = tdsrrcmeas_sib11->fach.inter_sys[0];
  }

  /* Check if HCS is in use. */
  if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib12_ptr->measurementControlSysInfo.use_of_HCS,
     rrc_MeasurementControlSysInfo_use_of_HCS_hcs_used ))
  {
    /* start HCS processing */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"HCS Processing");
    if(!(sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->m.
        interRATMeasurementSysInfoPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Sys info absent in SIB12 checking SIB11");
      if(tdsrrcmeas_sib11->inter_sys_meas_info_incl == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copying inter rat cell info from SIB 11");
        l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
        l1_inter_rat_sib_ptr->inter_s = tdsrrcmeas_sib11->inter_s;
        l1_inter_rat_sib_ptr->hcs_used = TRUE;
      }
      return SUCCESS;
    }
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE(sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->
      interRATMeasurementSysInfo,tdsrrc_InterRATMeasurementSysInfo,interRATCellInfoList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT list absent in SIB12 checking SIB11");
      if(tdsrrcmeas_sib11->inter_sys_meas_info_incl == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copying inter rat cell info from SIB 11");
        l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
        l1_inter_rat_sib_ptr->inter_s = tdsrrcmeas_sib11->inter_s;
        l1_inter_rat_sib_ptr->hcs_used = TRUE;
      }
      return SUCCESS;
    }

    /* Store ptr given by UTRAN */
    hcs_inter_rat_cell_list_ptr = &sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->
      interRATMeasurementSysInfo.interRATCellInfoList.newInterRATCellList;

    if (hcs_inter_rat_cell_list_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT list absent in SIB12 checking SIB11");
      if(tdsrrcmeas_sib11->inter_sys_meas_info_incl == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copying inter rat cell info from SIB 11");
        l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
        l1_inter_rat_sib_ptr->inter_s = tdsrrcmeas_sib11->inter_s;
        l1_inter_rat_sib_ptr->hcs_used = TRUE;
      }
      return SUCCESS;
    }

    /* Process the remove cell list of SIB12 */
    tdsrrcirat_process_remove_cell_list(&sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->
      interRATMeasurementSysInfo.interRATCellInfoList.removedInterRATCellList, &num_removed_cells);
    idx=0;
    while ( (hcs_inter_rat_cell_list_ptr->n > idx) &&
      (cell_count < TDSL1_MAX_CELL_MEAS) )
    {
      /* Let first check the type of the cell to find if we
      * really need to give this cell to L1 */
      if (hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.t !=
        T_tdsrrc_NewInterRATCell_technologySpecificInfo_gsm)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring Non GSM cell");
        arfcn = TDSMAX_ARFCN;
        if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(hcs_inter_rat_cell_list_ptr,interRATCellID))
        {
          /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
          cell_position = (uint16)
            hcs_inter_rat_cell_list_ptr->elem[idx].interRATCellID;
        }
        else
        {
          /* Store the cell in first order position */
          cell_position = TDSAVAILABLE_CELL_POSITION;
        }
        /* Tell to get the first available cell position and
        * that would be the cell id. The called function would store
        * the cell id back in passed cell_position variable */
        if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
          return FAILURE;
        }

        idx++;
        continue;
      }
      else
      {
       /* l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.delta_qrxlevmin_included = FALSE; */

        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].cell_info_type = TDSL1_GSM_SYSTEM;

        /* Cell reselection info is always set to TRUE. If not present in SIB 11, just
        set it to default values */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info_incl = TRUE;

        /* Initializw q-offset1-2 Defaults as per specs */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info.qoffset1_s_n = TDSQOFFSET1;


        /* -24. Got this value from Radio Access Capability */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info.max_tx_pwr = -24;

          /* Not valid for Inter-RAT but initialize to default
        * as they are present */
        if ((sib4_present == TRUE) && (sib4_ptr != NULL))
        {
          if (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.t !=
              T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"unsupprt FDD mode");
            return(FAILURE);
          }

          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.rxlev_min = (int16)
            (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
          /* Check for REL 5 extension ptr */
          if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
            (sib4_ptr->m.v4b0NonCriticalExtensionsPresent) &&
            (sib4_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent) &&
            (sib4_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.sysInfoType4_v590ext.m.
            cellSelectReselectInfo_v590extPresent) && (sib4_ptr->v4b0NonCriticalExtensions.
            v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.m.
            deltaQrxlevminPresent))
          {

            /*l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.delta_qrxlevmin_included = TRUE;
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
             gsm_cell.cell_sel_info.delta_qrxlevmin = sib4_ptr->v4b0NonCriticalExtensions.
             v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2; */

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB4 Delta-Qrxlev=%d, Qrxlev=%d, ARFCN=%d",
              (sib4_ptr->v4b0NonCriticalExtensions.
               v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2),
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min,
              CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn));

            /*if ((l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +
                 (sib4_ptr->v4b0NonCriticalExtensions.
                  v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2)) < -115)
            {*/
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.rxlev_min += (int16) (sib4_ptr->v4b0NonCriticalExtensions.
               v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2);
            /*}*/
          }
        }
        else
        {
          if (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.t !=
              T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"unsupprt FDD mode");
            return(FAILURE);
          }
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.rxlev_min = (int16)
            (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
          /* Check for REL 5 extension ptr */
          if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
            (sib3_ptr->m.v4b0NonCriticalExtensionsPresent) &&
            (sib3_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent) &&
            (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.sysInfoType3_v590ext.m.
            cellSelectReselectInfo_v590extPresent) && (sib3_ptr->v4b0NonCriticalExtensions.
            v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.m.
            deltaQrxlevminPresent))
          {
            /*l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.delta_qrxlevmin_included = TRUE;
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.delta_qrxlevmin = sib3_ptr->v4b0NonCriticalExtensions.
              v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2; */
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB3 Delta-Qrxlev=%d, Qrxlev=%d, ARFCN=%d",
              (sib3_ptr->v4b0NonCriticalExtensions.
                v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2),
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min,
              CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn));

            /*if ((l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +
                 (sib3_ptr->v4b0NonCriticalExtensions.
                  v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2)) < -115)
            {*/
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.rxlev_min += (int16) (sib3_ptr->v4b0NonCriticalExtensions.
                v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2);
            /*}*/
          }
        }

        /* Set default values for the HCS neighboring cell info if it is first cell
        * else use the info from the previous cell */
        if(cell_count == 0)
        {
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.hcs_prio = 0;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.q_hcs = 110;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.penalty_time = 0;
        }
        else
        {
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.hcs_prio =
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
            gsm_cell.hcs_info.hcs_prio;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.q_hcs =
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
            gsm_cell.hcs_info.q_hcs;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.penalty_time =
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
            gsm_cell.hcs_info.penalty_time;

          if(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
            gsm_cell.hcs_info.penalty_time != 0)
          {
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.hcs_info.tmp_offset1 =
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
              gsm_cell.hcs_info.tmp_offset1;
          }
        }

        /* Get GSM CELL Info */
        /* Check if rrc_gsm_cellSelectionReselectionInfo_present */
        if (hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->m.cellSelectionReselectionInfoPresent)
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Reselection info present");

          /* q-Offset1S-N */

            /* Range is -50...50 */
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.qoffset1_s_n = (int16)
              hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.q_Offset1S_N;
          

          /* Max Txit Power */
          if (hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->cellSelectionReselectionInfo.m.
            maxAllowedUL_TX_PowerPresent)
          {
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.max_tx_pwr = (int16)
              hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.maxAllowedUL_TX_Power;
          }

          /* No need to Check if gsm mode present as it has already been done */

          /* Check if rxlev_min present */

          if (hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.modeSpecificInfo.u
            .gsm->m.q_RxlevMinPresent)
          {
            /* Store rxlev_min in L1 */
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min = (int16)
              ((hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.modeSpecificInfo.u.gsm->q_RxlevMin * 2) + 1);
          }
          if(rel5_ext_ptr != NULL)
          {
            if(TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(rel5_ext_ptr,deltaQrxlevmin))
            {
              /*l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
                delta_qrxlevmin_included = TRUE;
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
                delta_qrxlevmin = rel5_ext_ptr->value.deltaQrxlevmin * 2;        */
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Delta-Qrxlev=%d, Qrxlev=%d, ARFCN=%d",
                (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2),
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min,
                CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn));

              /*if ((l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +
                   (rel5_ext_ptr->value.deltaQrxlevmin * 2)) < -115)
              {*/
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +=
                  (int16) (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2);
              /*}*/
            }
          }
          /* HCS inforamtion */
          if(TDSRRC_MSG_COMMON_BITMASK_IE(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo,
            tdsrrc_CellSelectReselectInfoSIB_11_12,hcs_NeighbouringCellInformation_RSCP))
          {
            /* Default HCS params */
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                gsm_cell.hcs_info.hcs_prio = 0;
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                  gsm_cell.hcs_info.q_hcs = 110;
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 0;

            /* HCS PRIO*/
           
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                gsm_cell.hcs_info.hcs_prio = (uint16)
                hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_PRIO;
            

            /*Q HCS */
            
              if(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.q_HCS < 90)
              {
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                  gsm_cell.hcs_info.q_hcs = (uint16) (110 -
                  hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.q_HCS);
              }
              else
              {
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                  gsm_cell.hcs_info.q_hcs = 37;
              }
            
            /* Delta Qhcs is not needed if it is not mapped from CPICH-RSCP or PCCPCH-RSCP.
             * For inter rat cells, Qhcs is always mapped from RSSI
             */

            /* PENALTY TIME*/
            if(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
              .penaltyTime.t != T_tdsrrc_PenaltyTime_RSCP_notUsed)
            {
              switch(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                .penaltyTime.t)
              {
              case T_tdsrrc_PenaltyTime_RSCP_pt10:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 10;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt10);
                break;

              case T_tdsrrc_PenaltyTime_RSCP_pt20:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 20;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt20);
                break;

              case T_tdsrrc_PenaltyTime_RSCP_pt30:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 30;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt30);
                break;

              case T_tdsrrc_PenaltyTime_RSCP_pt40:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 40;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt40);
                break;

              case T_tdsrrc_PenaltyTime_RSCP_pt50:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 50;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt50);
                break;

              case T_tdsrrc_PenaltyTime_RSCP_pt60:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 60;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt60);
                break;

              default:  /* should not happen */
                break;
              }
            }
          }

        } /* rrc_gsm_cellSelectionReselectionInfo_present */

        /* Cell Individual Offset */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.cio = (int16)
          hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->interRATCellIndividualOffset;

        /* Get BAND 1800 DCS or 1900 PCS */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator =
          TDSRET_INTER_RAT_BAND(hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->frequency_band);

        /* Get bcch_ARFCN */
        CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn) =
          (uint16) hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bcch_ARFCN;

        arfcn = (uint16) hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bcch_ARFCN;

#ifdef FEATURE_GSM_QUADBAND

/* for SGLTE, don't need to know band enable/disable at G, just do band convertion */
#ifdef FEATURE_SGLTE
        if (tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
        {
          geran_grr_band_ind_e grr_band_ind =  (l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator == TDSL1_DCS_1800_BAND) ?
				                                 GERAN_GRR_BAND_IND_DCS_1800 : GERAN_GRR_BAND_IND_PCS_1900;
		  ARFCN_T converted_arfcn = geran_grr_convert_to_arfcn_type(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num, grr_band_ind);
		  tdsconverted_gsm_band = converted_arfcn.band;
		  TDSRRC_MSG2(MSG_LEGACY_HIGH,"SGLTE: arfcn %d,converted_gsm_band %d",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num,tdsconverted_gsm_band);
        }
		else
#endif /* FEATURE_SGLTE */
        {
          tdsconverted_gsm_band = tdsrrcirat_get_gsm_band(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info);
        }

		if (tdsconverted_gsm_band != INVALID_BAND)
        {
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.band =
            tdsconverted_gsm_band;
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"ARFCN %d ignored bec of invalid band",hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
            u.gsm->bcch_ARFCN,0,0);
          arfcn = TDSMAX_ARFCN;
          /* Increment the pointer */
          if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(hcs_inter_rat_cell_list_ptr,interRATCellID))
          {
            /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
            cell_position = (uint16)
              hcs_inter_rat_cell_list_ptr->elem[idx].interRATCellID;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
            /* Store the cell in first order position */
            cell_position = TDSAVAILABLE_CELL_POSITION;
          }
          /* Tell to get the first available cell position and
          * that would be the cell id. The called function would store
          * the cell id back in passed cell_position variable */
          if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
            return FAILURE;
          }
          idx++;
          continue;
        }
#endif /* FEATURE_GSM_QUADBAND */

        /* To add BSIC (Base Station Identification Code) */
        if ((hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc
          > TDSNCC_MAX) ||
          (hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc
          > TDSBCC_MAX))
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"BCC %d or NCC %d invalid[0..7]",hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc,hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc);

          return FAILURE;

        } /* End of if of checking NCC and BCC validity */

        /* Store good values of bcc and ncc */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.ncc =
          (uint8) hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.ncc;

        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.bcc =
          (uint8) hcs_inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.bcc;
      } /* End of gsm cell  */

      /* Time to get its cell id*/
      /* Cell Order has already been verified */

      if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(hcs_inter_rat_cell_list_ptr,interRATCellID))
      {
        /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
        cell_position = (uint16)
          hcs_inter_rat_cell_list_ptr->elem[idx].interRATCellID;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
        /* Store the cell in first order position */
        cell_position = TDSAVAILABLE_CELL_POSITION;
      }
      /* Tell to get the first available cell position and
      * that would be the cell id. The called function would store
      * the cell id back in passed cell_position variable */
      if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
        return FAILURE;
      }
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id
        = cell_position;
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
      /* Increment cell count */
      cell_count++;

      /* Increment the pointer */
      idx++;
      if ((hcs_inter_rat_cell_list_ptr->n > idx) && (cell_count == TDSL1_MAX_CELL_MEAS))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List full");
        break;
      }
    } /* End of while hcs_inter_rat_cell_list_ptr */

    /* Finally Set the cell count */
    l1_inter_rat_sib_ptr->inter_s.num_cell_add = cell_count;

    /* build composite cell info list from SIB 11 and SIB 12 */
    tdsrrcirat_build_composite_cell_list_from_SIB11_SIB12(
      &sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->
      interRATMeasurementSysInfo.interRATCellInfoList.removedInterRATCellList,
      &tdsrrcmeas_sib11->inter_s, &l1_inter_rat_sib_ptr->inter_s, num_removed_cells);
    for(cell_count = 0; cell_count < l1_inter_rat_sib_ptr->inter_s.num_cell_add; cell_count++)
    {
       if(sib18_present)
       {
         if(tmp_rrc_PLMNsOfInterRATCellsList_ptr->n > cell_count)
         {
           if(tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[cell_count].m.
            plmn_IdentityPresent)
           {
             tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[cell_count].plmn_Identity,
               &tmp_plmn_identity);
             if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
             {
               if((tdsrrccsp_check_for_eplmn(
                  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                    tmp_plmn_identity))) == FALSE)
               {
                 TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d Non eq PLMN measure only in DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
                 /* set the flag to FALSE */
                 l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
               }
             }
           }
           else
           {
             /* use the previous value of the flag if PLMN id is not selected PLMN */
             if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
             {
               /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
               if(cell_count > 0) /* to satisfy lint */
               {
                 l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach =
                   l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].measure_in_idle_pch_fach;
               }
             }
           }

           
         }
         else
         {
           /* PLMN list is shorter than the cell info list set Flag to true for the remaining cells */
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d measured in IDLE,FACH,PCH,DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
         }
       }
       else
       {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d measured in IDLE,FACH,PCH,DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
       }
    }
    /* Now set inter sys info incl info to TRUE */
    l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
    l1_inter_rat_sib_ptr->hcs_used = TRUE;
    return SUCCESS;
    /*  end HCS processing */
  }
  else
  {
    /* HCS is NOT in use */
    l1_inter_rat_sib_ptr->hcs_used = FALSE;
    if(!(sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->m.
        interRATMeasurementSysInfoPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Sys info absent in SIB12 checking SIB11");
      if(tdsrrcmeas_sib11->inter_sys_meas_info_incl == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copying inter rat cell info from SIB 11");
        l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
        l1_inter_rat_sib_ptr->inter_s = tdsrrcmeas_sib11->inter_s;
      }
      return SUCCESS;
    }
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE(sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->
      interRATMeasurementSysInfo,tdsrrc_InterRATMeasurementSysInfo_B,interRATCellInfoList)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT list absent in SIB12 checking SIB11");
      if(tdsrrcmeas_sib11->inter_sys_meas_info_incl == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copying inter rat cell info from SIB 11");
        l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
        l1_inter_rat_sib_ptr->inter_s = tdsrrcmeas_sib11->inter_s;
      }
      return SUCCESS;
    }

    /* Store ptr given by UTRAN */
    inter_rat_cell_list_ptr = &sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->
      interRATMeasurementSysInfo.interRATCellInfoList.newInterRATCellList;

    if (inter_rat_cell_list_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT list absent in SIB12 checking SIB11");
      if(tdsrrcmeas_sib11->inter_sys_meas_info_incl == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copying inter rat cell info from SIB 11");
        l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
        l1_inter_rat_sib_ptr->inter_s = tdsrrcmeas_sib11->inter_s;
      }
      return SUCCESS;
    }

    /* Process the remove cell list of SIB12 */
    tdsrrcirat_process_remove_cell_list(&sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->
      interRATMeasurementSysInfo.interRATCellInfoList.removedInterRATCellList, &num_removed_cells);
    
    idx=0;
    while ( (inter_rat_cell_list_ptr->n > idx) &&
      (cell_count < TDSL1_MAX_CELL_MEAS) )
    {
    /* Let first check the type of the cell to find if we
      * really need to give this cell to L1 */
      if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.t !=
        T_tdsrrc_NewInterRATCell_technologySpecificInfo_gsm)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring Non GSM cell");
        arfcn = TDSMAX_ARFCN;
        if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,
            interRATCellID))
        {
          /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
          cell_position = (uint16)
            inter_rat_cell_list_ptr->elem[idx].interRATCellID;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
          /* Store the cell in first order position */
          cell_position = TDSAVAILABLE_CELL_POSITION;
        }
        /* Tell to get the first available cell position and
        * that would be the cell id. The called function would store
        * the cell id back in passed cell_position variable */
        if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
          return FAILURE;
        }
        idx++;
        continue;
      }
      else
      {
      /*  l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.delta_qrxlevmin_included = FALSE; */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].cell_info_type = TDSL1_GSM_SYSTEM;

        /* Cell reselection info is always set to TRUE. If not present in SIB 11, just
        set it to default values */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info_incl = TRUE;

        /* Initializw q-offset1-2 Defaults as per specs */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info.qoffset1_s_n = TDSQOFFSET1;


        /* -24. Got this value from Radio Access Capability */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info.max_tx_pwr = -24;

          /* Not valid for Inter-RAT but initialize to default
        * as they are present */
        if ((sib4_present == TRUE) && (sib4_ptr != NULL))
        {
          if (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.t !=
              T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"unsupprt FDD mode");
            return(FAILURE);
          }
          
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.rxlev_min = (int16)
            (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
          /* Check for REL 5 extension ptr */
          if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
            (sib4_ptr->m.v4b0NonCriticalExtensionsPresent) &&
            (sib4_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent) &&
            (sib4_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.sysInfoType4_v590ext.m.
            cellSelectReselectInfo_v590extPresent) && (sib4_ptr->v4b0NonCriticalExtensions.
            v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.m.
            deltaQrxlevminPresent))
          {

            /*l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.delta_qrxlevmin_included = TRUE;
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.delta_qrxlevmin = sib4_ptr->v4b0NonCriticalExtensions.
              v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2; */
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB4 Delta-Qrxlev=%d, Qrxlev=%d, ARFCN=%d",
              (sib4_ptr->v4b0NonCriticalExtensions.
               v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2),
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min,
              CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn));

            /*if ((l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +
                 (sib4_ptr->v4b0NonCriticalExtensions.
                  v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2)) < -115)
            {*/
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.rxlev_min += (int16) (sib4_ptr->v4b0NonCriticalExtensions.
                v590NonCriticalExtension.sysInfoType4_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2);
            /*}*/
          }
        }
        else
        {
          
          if (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.t !=
              T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"unsupprt FDD mode");
            return(FAILURE);
          }
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.rxlev_min = (int16)
            (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
          /* Check for REL 5 extension ptr */
          if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
            (sib3_ptr->m.v4b0NonCriticalExtensionsPresent) &&
            (sib3_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent) &&
            (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.sysInfoType3_v590ext.m.
            cellSelectReselectInfo_v590extPresent) && (sib3_ptr->v4b0NonCriticalExtensions.
            v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.m.
            deltaQrxlevminPresent))
          {
            /*l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.delta_qrxlevmin_included = TRUE;
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.delta_qrxlevmin = sib3_ptr->v4b0NonCriticalExtensions.
              v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2;*/
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB3 Delta-Qrxlev=%d, Qrxlev=%d, ARFCN=%d",
              (sib3_ptr->v4b0NonCriticalExtensions.
               v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2),
               l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min,
               CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn));

            /*if ((l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +
               (sib3_ptr->v4b0NonCriticalExtensions.
                v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2)) < -115)
            {*/
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                gsm_cell.cell_sel_info.rxlev_min += (int16) (sib3_ptr->v4b0NonCriticalExtensions.
                v590NonCriticalExtension.sysInfoType3_v590ext.cellSelectReselectInfo_v590ext.deltaQrxlevmin * 2);
            /*}*/
          }
        }

        /* Get GSM CELL Info */
        /* Check if rrc_gsm_cellSelectionReselectionInfo_present */
        if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->m.cellSelectionReselectionInfoPresent)
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Reselection info present");

          /* q-Offset1S-N */

            /* Range is -50...50 */
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.qoffset1_s_n = (int16)
              inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.q_Offset1S_N;
          


          /* Max Txit Power */
          if (TDSRRC_MSG_COMMON_BITMASK_IE(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->cellSelectionReselectionInfo,
            tdsrrc_CellSelectReselectInfoSIB_11_12,maxAllowedUL_TX_Power))
          {
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.cell_sel_info.max_tx_pwr = (int16)
              inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.maxAllowedUL_TX_Power;
          }

          /* No need to Check if gsm mode present as it has already been done */

          /* Check if rxlev_min present */

          if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.modeSpecificInfo.u
            .gsm->m.q_RxlevMinPresent)
          {
            /* Store rxlev_min in L1 */
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min = (int16)
              ((inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.modeSpecificInfo.u.gsm->q_RxlevMin * 2) + 1);
          }
          /*l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
            delta_qrxlevmin_included = FALSE;  */
          if(rel5_ext_ptr != NULL)
          {
            if(TDSRRC_MSG_LIST_BITMASK_IE_TYPE2( rel5_ext_ptr,deltaQrxlevmin))
            {
              /*l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
                delta_qrxlevmin_included = TRUE;
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
                delta_qrxlevmin = rel5_ext_ptr->value.deltaQrxlevmin * 2;        */
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Delta-Qrxlev=%d, Qrxlev=%d, ARFCN=%d",
                (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2),
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min,
                CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn));

              /*if ((l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +
                 (rel5_ext_ptr->value.deltaQrxlevmin * 2)) < -115)
              {*/
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +=
                  (int16) (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2);
              /*}*/
            }
          }
        } /* rrc_gsm_cellSelectionReselectionInfo_present */

        /* Cell Individual Offset */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.cio = (int16)
          inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->interRATCellIndividualOffset;

        /* Get BAND 1800 DCS or 1900 PCS */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator =
          TDSRET_INTER_RAT_BAND(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->frequency_band);

        /* Get bcch_ARFCN */
        CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn) =
          (uint16) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bcch_ARFCN;

        arfcn = (uint16)inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bcch_ARFCN;

#ifdef FEATURE_GSM_QUADBAND

/* for SGLTE, don't need to know band enable/disable at G, just do band convertion */
#ifdef FEATURE_SGLTE
        if (tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
        {
          geran_grr_band_ind_e grr_band_ind = (l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator == TDSL1_DCS_1800_BAND) ?
				                                 GERAN_GRR_BAND_IND_DCS_1800 : GERAN_GRR_BAND_IND_PCS_1900;
		  ARFCN_T converted_arfcn = geran_grr_convert_to_arfcn_type(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num, grr_band_ind);
		  tdsconverted_gsm_band = converted_arfcn.band;
		  TDSRRC_MSG2(MSG_LEGACY_HIGH,"SGLTE: arfcn %d,converted_gsm_band %d",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num,tdsconverted_gsm_band);
        }
		else
#endif /* FEATURE_SGLTE */
        {
          tdsconverted_gsm_band = tdsrrcirat_get_gsm_band(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info);
        }

		if (tdsconverted_gsm_band != INVALID_BAND)
        {
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.band =
            tdsconverted_gsm_band;
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"ARFCN %d ignored bec of invalid band",inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
            u.gsm->bcch_ARFCN,0,0);
          arfcn = TDSMAX_ARFCN;
          if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
          {
            /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
            cell_position = (uint16)
              inter_rat_cell_list_ptr->elem[idx].interRATCellID;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
            /* Store the cell in first order position */
            cell_position = TDSAVAILABLE_CELL_POSITION;
          }          /* Tell to get the first available cell position and
          * that would be the cell id. The called function would store
          * the cell id back in passed cell_position variable */
          if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
            return FAILURE;
          }
          /* Increment the pointer */
          idx++;
          continue;
        }
#endif /* FEATURE_GSM_QUADBAND */

        /* To add BSIC (Base Station Identification Code) */
        if ((inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc
          > TDSNCC_MAX) ||
          (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc
          > TDSBCC_MAX))
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"BCC %d or NCC %d invalid[0..7]",inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc,inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc);

          return FAILURE;

        } /* End of if of checking NCC and BCC validity */

        /* Store good values of bcc and ncc */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.ncc =
          (uint8) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.ncc;

        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.bcc =
          (uint8) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.bcc;
      } /* End of gsm cell  */

      /* Time to get its cell id*/
      /* Cell Order has already been verified */

      if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
      {
        /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
        cell_position = (uint16)
          inter_rat_cell_list_ptr->elem[idx].interRATCellID;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
        /* Store the cell in first order position */
        cell_position = TDSAVAILABLE_CELL_POSITION;
      }
      /* Tell to get the first available cell position and
      * that would be the cell id. The called function would store
      * the cell id back in passed cell_position variable */
      if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
        return FAILURE;
      }
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id
        = cell_position;
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].
        measure_in_idle_pch_fach = TRUE;

      /* Increment cell count */
      cell_count++;

      /* Increment the pointer */
      idx++;
      if ((inter_rat_cell_list_ptr->n > idx) && (cell_count == TDSL1_MAX_CELL_MEAS))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List full");
        break;
      }
    } /* End of while inter_rat_cell_list_ptr */

    /* Finally Set the cell count */
    l1_inter_rat_sib_ptr->inter_s.num_cell_add = cell_count;

    /* build composite cell info list from SIB 11 and SIB 12 */
    tdsrrcirat_build_composite_cell_list_from_SIB11_SIB12(
      &sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->
      interRATMeasurementSysInfo.interRATCellInfoList.removedInterRATCellList,
      &tdsrrcmeas_sib11->inter_s, &l1_inter_rat_sib_ptr->inter_s, num_removed_cells);

    for(cell_count = 0; cell_count < l1_inter_rat_sib_ptr->inter_s.num_cell_add; cell_count++)
    {
      if(sib18_present)
      {
        if(tmp_rrc_PLMNsOfInterRATCellsList_ptr->n > cell_count)
        {
          if(tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
          {
            tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[cell_count].plmn_Identity,
              &tmp_plmn_identity);
            if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
            {
              if((tdsrrccsp_check_for_eplmn(
                mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                tmp_plmn_identity))) == FALSE)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d Non eq PLMN measure only in DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
                /* set the flag to FALSE */
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
              }
            }
          }
          else
          {
            /* use the previous value of the flag if PLMN id is not selected PLMN */
            if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
            {
              /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
              if(cell_count > 0) /* to satisfy lint */
              {
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach =
                  l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].measure_in_idle_pch_fach;
              }
            }
          }

        }
        else
        {
          /* PLMN list is shorter than the cell info list set Flag to true for the remaining cells */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d measured in IDLE,FACH,PCH,DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d measured in IDLE,FACH,PCH,DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
      }
    }

    /* Now set inter sys info incl info to TRUE */
    l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;




     TDSRRC_MSG0(MSG_LEGACY_HIGH,"IRAT CELL INFO = FALSE");
     tdsinterRAT_cell_info_status.interRAT_cell_info_present = FALSE;
     tdsinterRAT_cell_info_status.interRAT_cell_info_ind = 0;

    if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib12_ptr->measurementControlSysInfo.use_of_HCS,
     rrc_MeasurementControlSysInfo_use_of_HCS_hcs_used ))
    {
      if((sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->m.
        interRATMeasurementSysInfoPresent))
      {
        if ((TDSRRC_MSG_COMMON_BITMASK_IE(sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->
           interRATMeasurementSysInfo,tdsrrc_InterRATMeasurementSysInfo,interRATCellInfoList)) ||
           (tdsrrcmeas_sib11->inter_sys_meas_info_incl == TRUE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IRAT CELL INFO = TRUE");
          tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
          tdsinterRAT_cell_info_status.interRAT_cell_info_ind = 0;
          return SUCCESS;
        }
      }
    }
    else
    {
      if((sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->m.
        interRATMeasurementSysInfoPresent))
      {
        if ((TDSRRC_MSG_COMMON_BITMASK_IE(sib12_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->
          interRATMeasurementSysInfo,tdsrrc_InterRATMeasurementSysInfo_B,interRATCellInfoList)) ||
          (tdsrrcmeas_sib11->inter_sys_meas_info_incl == TRUE))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"IRAT CELL INFO = TRUE");
          tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
          tdsinterRAT_cell_info_status.interRAT_cell_info_ind = 0;
          return SUCCESS;
        }
      }
    }




    return SUCCESS;

  } /* End of else of HCS not in use */
}

/*===========================================================================

  FUNCTION   RRCIRAT_RET_PERIODIC_RPT_INTVL

DESCRIPTION

     This function converts the reporting interval specified by network into the
     format required by L1


DEPENDENCIES

  None

RETURN VALUE

  Rpting Interval required by L1

SIDE EFFECTS

  None

===========================================================================*/

static tdsl1_reporting_interval_enum_type tdsrrcirat_ret_periodic_rpt_intvl
(
  tdsrrc_ReportingIntervalLong periodic_intvl
)
{
  tdsl1_reporting_interval_enum_type l1_rpt_intvl = TDSL1_RPT_INTERVAL_250MS;
  switch(periodic_intvl)
  {
  /* Since L1 did not define 0 ms, just to satisfy LINT use TDSL1_RPT_INTERVAL_250MS.
  L1 interface change required.  rrc_ril0 scenario is taken care of before this fn is called
  by specifying that no reporting is required. Also for Reporting Interval 0, spec says
    UE behavior is unspecified. */
  case tdsrrc_ReportingIntervalLong_ril0:
  case tdsrrc_ReportingIntervalLong_ril0_25:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_250MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril0_5:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_500MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril1:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_1000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril2:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_2000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril3:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_3000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril4:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_4000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril6:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_6000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril8:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_8000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril12:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_12000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril16:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_16000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril20:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_20000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril24:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_24000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril28:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_28000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril32:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_32000MS;
    break;

  case tdsrrc_ReportingIntervalLong_ril64:
    l1_rpt_intvl = TDSL1_RPT_INTERVAL_64000MS;
    break;
  }
  return l1_rpt_intvl;
}

/*===========================================================================

FUNCTION   RRCIRAT_CHECK_INTER_RAT_CELL_ORDER_MCM

DESCRIPTION

  This function processes the cell list received in a Measurement Control Message


DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If cell list is in order
  SUCCESS: If cell list is not in order

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_check_inter_rat_cell_order
(
  tdsrrc_InterRATCellInfoList_B* rrc_cell_info_list_ptr
)
{
  /* Find the cell order */

  uint32 cell_id_list[TDSL1_MAX_CELL_MEAS];
  uint16 count=0, list_count1 =0, list_count2 = 0;

  tdsrrc_NewInterRATCellList_B   *local_inter_rat_cell_list_ptr = NULL;

  local_inter_rat_cell_list_ptr = &rrc_cell_info_list_ptr->newInterRATCellList;

  for (count = 0; count <TDSL1_MAX_CELL_MEAS; count++ )
    cell_id_list[count] = 0;

  count = 0;

  /* Store cell id's in a local list and calculate total no of cells */
  while ((local_inter_rat_cell_list_ptr->n > list_count1) && (count < TDSL1_MAX_CELL_MEAS))
  {
    if (local_inter_rat_cell_list_ptr->elem[list_count1].m.interRATCellIDPresent)

    {
      /* Store cell id */
      cell_id_list[count] = local_inter_rat_cell_list_ptr->elem[list_count1].interRATCellID;
      count++;
    }
    /* Cell id was never present which is very likely */
    list_count1++;
  }

  if (count == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell list: No cell Id's");
    /* Return successfully as cells would now be stored
    in first available position */
    return SUCCESS;
  }
  if (count == 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell list: 1 cell with id");
    /* So no comparision has to be done. */
    return SUCCESS;
  }
  /* This means that there are multiple cells in the cell list and there
  order needs to be verified */

  /* Compare 1 st with 2,3,4..... Then 2nd with 3,4,5... and so on */
  for (list_count1 = 0; list_count1 < (count-1); list_count1++)
  {
    for (list_count2 = list_count1+1; list_count2 < count; list_count2++)
    {
      if (cell_id_list[list_count1] < cell_id_list[list_count2])
      {
        /* Good. It is in order */
        continue;
      }
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Inter-RAT Cell List elements %d & %d not in order",cell_id_list[list_count1],cell_id_list[list_count2]);
      return FAILURE;   /* So ignore this list in the calling fn */
    } /* End of for for list_count2 */
  }  /* End of for for list_count1 */
#ifdef TDSDEBUG_MSG
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Inter-RAT Cell list has %d cells",count);
#endif
  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCIRAT_UPDATE_INTER_RAT_CELL_LIST

DESCRIPTION

  This function updates the inter rat cell list received from SIB11

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If cell list is not in order
  SUCCESS: If cell list is in order.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_update_inter_rat_cell_list
(
 uint16* position_no_ptr,
 uint16 arfcn
)
{
  uint8 count = 0;

  /* First check if one has to add this in the first available position */
  if (*position_no_ptr == TDSAVAILABLE_CELL_POSITION)
  {
    for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
    {
      if (tdsinter_rat_cell_list[count].cell_position == TDSVACANT)
      {
        /* Store this cell in this first available vacant position */
        tdsinter_rat_cell_list[count].cell_position = TDSOCCUPIED;
        tdsinter_rat_cell_list[count].cell_id = count;
        tdsinter_rat_cell_list[count].arfcn = arfcn;
        /* Set the position_no_ptr to count */
        *position_no_ptr = count;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB: Adding cell with id = %d",count);
        return SUCCESS;
      }
    } /* End of for */
  } /* End of if of  position_no_ptr == TDSAVAILABLE_CELL_POSITION */


  /* Now check which position the cells needs to be updated */
  for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
  {
    if (count == *position_no_ptr)
    {
    /* We have reached the position in the array which is given by id
     * Check if there is already a cell in this position
     * If yes, then overwrite on this cell */


      /* Store the new cell id given by *position_no_ptr at this position */
      tdsinter_rat_cell_list[count].cell_id = *position_no_ptr;

      /* Mark the position as occupied */
      tdsinter_rat_cell_list[count].cell_position = TDSOCCUPIED;

      tdsinter_rat_cell_list[count].arfcn = arfcn;

      return SUCCESS;
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_ERROR,"InterRAT cell ID = %d not found",*position_no_ptr);
  return FAILURE;

}

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_REPORT_CRITERIA

DESCRIPTION

  This function processes the report criteria in the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_report_criteria
(
  tdsrrc_InterRATReportCriteria* reportCriteria_ptr,
  tdsl1_inter_sys_meas_ctrl_struct_type* inter_s_ptr,
  tdsrpt_crit_enum_type* mcm_rpt_crit_ptr,
  uint8 tdstransaction_id
)
{
  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* Get Reporting Criteria */
  if (reportCriteria_ptr->t == T_tdsrrc_InterRATReportCriteria_interRATReportingCriteria)
  {
    /* Set criteria to event triggered */
    if(!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(reportCriteria_ptr->u.interRATReportingCriteria,interRATEventList))
      || (reportCriteria_ptr->u.interRATReportingCriteria->interRATEventList.n == 0))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Event Criteria empty in modify set TDSL1_NO_CHANGE");
      inter_s_ptr->rpt_criteria.rpt_mode = TDSL1_NO_CHANGE;
      *mcm_rpt_crit_ptr = TDSNOT_AVAILABLE;
      return SUCCESS;
    }      
    inter_s_ptr->rpt_criteria.rpt_mode = TDSL1_EVENT_TRIGGER_RPT;

    *mcm_rpt_crit_ptr = TDSEVENT;

    /* Event Triggered Reporting Chosen */
    if (tdsrrcirat_process_event_criteria(reportCriteria_ptr->u.interRATReportingCriteria,
      &inter_s_ptr->rpt_criteria.u.evt_rpt_crit) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Event Criteria specified");
      return FAILURE;
    }
  }
  else if (reportCriteria_ptr->t == T_tdsrrc_InterRATReportCriteria_periodicalReportingCriteria)
  {
    /* Set periodic reporting */
    inter_s_ptr->rpt_criteria.rpt_mode = TDSL1_PERIODIC_RPT;

    *mcm_rpt_crit_ptr = TDSPERIODIC;
    /* Periodic Reporting Chosen */
    /* Set reporting cells */
    /* Check if reporting amount present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(reportCriteria_ptr->u.
      periodicalReportingCriteria,
      tdsrrc_PeriodicalWithReportingCellStatus,reportingCellStatus))
    {

      if ((tdsrrcmeas_fill_no_of_reporting_cells(
        &reportCriteria_ptr->u.
        periodicalReportingCriteria->reportingCellStatus,
        &inter_s_ptr->rpt_cell_status) == SUCCESS) &&
        (reportCriteria_ptr->u.
        periodicalReportingCriteria->reportingCellStatus.t ==
        T_tdsrrc_ReportingCellStatus_withinActSetOrVirtualActSet_InterRATcells))
      {
        inter_s_ptr->rpt_cell_status_incl = TRUE;
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid Rpt Cell Status %d", reportCriteria_ptr->u.periodicalReportingCriteria->
          reportingCellStatus.t,0,0);
        /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

    }
    /* Assign Default Value i.e. Infinity */
    inter_s_ptr->rpt_criteria.u.periodic_crit.rpt_amount
      = TDSL1_RPT_AMOUNT_INFINITY;

   
      /* Store reporting amount */
      inter_s_ptr->rpt_criteria.u.periodic_crit.rpt_amount =
        TDSRET_INTER_RAT_REPORTING_AMOUNT(
        reportCriteria_ptr->u.periodicalReportingCriteria->
        periodicalReportingCriteria.reportingAmount);
    

    if (reportCriteria_ptr->u.periodicalReportingCriteria->
      periodicalReportingCriteria.reportingInterval == tdsrrc_ReportingIntervalLong_ril0)
    {
    /* Since L1 does not have 0ms defined in interface, change rpt mode to no rpt
      so that L1 does not get confused. Also spec in this scenario says UE behaviour unspec */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rpt Intvl 0,No PeriodicRpting");
      inter_s_ptr->rpt_criteria.rpt_mode = TDSL1_NO_RPT;
    }
    else
    {
      /* Store reporting interval */
      inter_s_ptr->rpt_criteria.u.periodic_crit.rpt_interval =
        tdsrrcirat_ret_periodic_rpt_intvl(
        reportCriteria_ptr->u.periodicalReportingCriteria->
        periodicalReportingCriteria.reportingInterval);
    }

  } /* End of else of periodic reporting chosen */
  else
  {
    /* Set No reporting */
    inter_s_ptr->rpt_criteria.rpt_mode = TDSL1_NO_RPT;

    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(reportCriteria_ptr->u.noReporting,
      tdsrrc_ReportingCellStatusOpt,reportingCellStatus))
    {
      if ((tdsrrcmeas_fill_no_of_reporting_cells(
        &reportCriteria_ptr->u.noReporting->reportingCellStatus,
        &inter_s_ptr->rpt_cell_status) == SUCCESS) &&
        (reportCriteria_ptr->u.noReporting->reportingCellStatus.t ==
        T_tdsrrc_ReportingCellStatus_withinActSetOrVirtualActSet_InterRATcells))
      {
        inter_s_ptr->rpt_cell_status_incl = TRUE;
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid Rpt Cell Status %d", reportCriteria_ptr->u.periodicalReportingCriteria->
          reportingCellStatus.t,0,0);

        /* Send Meas Ctrl Failure Message */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;

      }

    }

    *mcm_rpt_crit_ptr = TDSADDTL;
  }
  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_CELL_LIST_MCM

DESCRIPTION

  This function processes the cell list received in a Measurement Control Message

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If Cell list is invalid
  SUCCESS: If Cell list is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_cell_list_mcm
(
  tdsrrc_InterRATCellInfoList* mcm_cell_list_ptr,
  tdsl1_inter_sys_cell_list_struct_type* l1_cell_list_ptr
)
{
/* RRC new cell info list and l1 new cell info list would have the new cell info list
through local neighbour list. This has to be done to make sure that if the Meas Ctrl
  Msg contains bad cells, our good tdsinter_rat_cell_list does not get disturbed */

  uint8 cnt = 0;

  uint8 cell_count = 0;

  uint32 cell_position = 0;

  uint16 cell_id = TDSINVALID_CELL_ID;

  /* For storing the new cell list */
  tdsrrc_NewInterRATCellList* local_rrc_new_cell_list_ptr = NULL;

  tdsrrc_CellsForInterRATMeasList* local_cells_for_meas_ptr = NULL;
  uint32 idx=0;

  uint16 arfcn = 0;
  
  /* Initializes the local neighbor list to default values */
  tdsrrcirat_initialize_local_neighbor_list();
  /* Copy Inter-RAT CELL_INFO_LIST list to local neighbor list. This is necessary
  so as to make sure that in case UTRAN messes up, our old cell list is intact */

  for (cnt=0; cnt < TDSL1_MAX_CELL_MEAS; cnt++)
  {
    memscpy(&tdslocal_neighbor_list[cnt],sizeof(tdsinter_rat_cell_list_struct_type),
	    &tdsinter_rat_cell_list[cnt],sizeof(tdsinter_rat_cell_list_struct_type));
  }

  /* In Inter-RAT meas, remove is not optional */
  /* For sanity, initialize to 0 */
  l1_cell_list_ptr->num_cell_rmv = 0;

  if (tdsrrcirat_fill_removed_inter_rat_cells(&mcm_cell_list_ptr->removedInterRATCellList,
    l1_cell_list_ptr) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Removed Inter-RAT cells");
    return FAILURE;
  }

  /* If removeAll in MCM, then always configure to L1 without additional check,
   * to make sure L1 and RRC has aligned cell list
   */
  #if 0
  /* UTRAN can ask us to remove all cells when it is possible that for that
  * measurement (First time SETUP) we may not have any cell list.
  * To avoid L1 getting confused, let us make number of cells to be
  * removed to 0 in case the cell list does not exist previously.
  * So check in the CELL_INFO_LIST */

  if ((mcm_cell_list_ptr->removedInterRATCellList.t ==
    T_tdsrrc_RemovedInterRATCellList_removeAllInterRATCells) &&
    (tdsrrcirat_check_cell_absence_in_cell_list() == SUCCESS))
  {
    /* Set No of Cells removed to 0 from oxff */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting No of cells removed to 0");
    l1_cell_list_ptr->num_cell_rmv = 0;
  }
  #endif

  /* At this time, tdslocal_neighbor_list is completely updated and has
  * the latest and the greatest information
  * Get New cells to be added. UTRAN can just set it to NULL */
  if (mcm_cell_list_ptr->newInterRATCellList.n == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:No new cells to be added");
    /* Put this info in L1 list */
    l1_cell_list_ptr->num_cell_add = 0;
  }
  else
  {
    if (tdsrrcirat_check_inter_rat_cell_order_mcm(&mcm_cell_list_ptr->newInterRATCellList) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");
      return FAILURE;
    }
    /* New cells need to be added */
    local_rrc_new_cell_list_ptr =  &mcm_cell_list_ptr->newInterRATCellList;

    /* Before verifying the cell id, make sure that cell params are correct */

    while ((local_rrc_new_cell_list_ptr->n > idx) && (cell_count < TDSL1_MAX_CELL_MEAS))
    {
    /* Let first check the type of the cell to find if we
      really need to give this cell to L1 */
      if (local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.t !=
        T_tdsrrc_NewInterRATCell_technologySpecificInfo_gsm)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Ignoring Non GSM cell");
        idx++;
        continue;
      }
      else
      {
        l1_cell_list_ptr->add_cell[cell_count].cell_info_type = TDSL1_GSM_SYSTEM;

        /* Get GSM CELL Info */

        /* No need to read cell selec/reselect info as that is for SIB11/12 */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.cell_sel_info_incl = FALSE;

        /* Cell Individual Offset */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.cio = (int16)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->interRATCellIndividualOffset;

        /* Get BAND 1800 DCS or 1900 PCS */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator =
          TDSRET_INTER_RAT_BAND(local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->frequency_band);

        /* Get bcch_ARFCN */
        CONVERT_ARFCN_TO_NUM(l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn) = (uint16)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bcch_ARFCN;

#ifdef FEATURE_ENH_BSIC_VERIFY
        arfcn = (uint16) local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bcch_ARFCN;
#endif

#ifdef FEATURE_GSM_QUADBAND

/* for SGLTE, don't need to know band enable/disable at G, just do band convertion */
#ifdef FEATURE_SGLTE
          if (tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
          {
            geran_grr_band_ind_e grr_band_ind = (l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator == TDSL1_DCS_1800_BAND) ?
				                                 GERAN_GRR_BAND_IND_DCS_1800 : GERAN_GRR_BAND_IND_PCS_1900;
			ARFCN_T converted_arfcn = geran_grr_convert_to_arfcn_type(l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num, grr_band_ind);
			tdsconverted_gsm_band = converted_arfcn.band;
			TDSRRC_MSG2(MSG_LEGACY_HIGH,"SGLTE: arfcn %d,converted_gsm_band %d",l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num,tdsconverted_gsm_band);
          }
		  else
#endif /* FEATURE_SGLTE */
        {
            tdsconverted_gsm_band = tdsrrcirat_get_gsm_band(l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info);
          }

          if (tdsconverted_gsm_band != INVALID_BAND)
          {
            l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.band =
              tdsconverted_gsm_band;
          }
          else
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"ARFCN %d rejected bec of invalid band",local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
              u.gsm->bcch_ARFCN,0,0);
            idx++;
            return FAILURE;
          }

#endif /* FEATURE_GSM_QUADBAND */

        /* To add BSIC (Base Station Identification Code) */
        if ((local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc
          > TDSNCC_MAX) ||
          (local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc
          > TDSBCC_MAX))
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"BCC %d or NCC %d invalid[0..7]",local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc,local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc);

          return FAILURE;

        } /* End of if of checking NCC and BCC validity */

        /* Store good values of bcc and ncc */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bsic.ncc = (uint8)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.ncc;

        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bsic.bcc = (uint8)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.bcc;
      } /* End of gsm cell  */

      /* Time to get its cell id*/
      /* Cell Order has already been verified */

      if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(local_rrc_new_cell_list_ptr,interRATCellID))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Inter-RAT Cell Id present");
        cell_position = local_rrc_new_cell_list_ptr->elem[idx].interRATCellID;
        l1_cell_list_ptr->add_cell[cell_count].intersys_cell_id = (uint16)
          local_rrc_new_cell_list_ptr->elem[idx].interRATCellID;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
        /* Store the cell in first order position */
        cell_position = TDSAVAILABLE_CELL_POSITION;
        /* Tell to get the first available cell position and
        * that would be the cell id. The called function would store
        * the cell id back in passed cell_position variable */
      }


      /* Now store the cell id in local neighbour rat cell list */
      if (tdsrrcirat_update_inter_rat_cell_list_mcm(&cell_position,
        &l1_cell_list_ptr->add_cell[cell_count].intersys_cell_id, &cell_id, arfcn)
        == SUCCESS)
      {
        /* set this flag to TRUE always for MCM */
        l1_cell_list_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
        /* Increment cell count as add cell successfully stored */
        cell_count ++;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCM:Invalid Inter-RAT cell list");
        return FAILURE;
      }

      /* Increment the pointer */
      /* Read Next One */
      idx++;


      if ((local_rrc_new_cell_list_ptr->n > idx) && (cell_count == TDSL1_MAX_CELL_MEAS))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List full");
        break;
      }

    } /* End of while inter_rat_cell_list_ptr */

    /* Finally Set the cell count */
    l1_cell_list_ptr->num_cell_add = cell_count;

  } /* End of else of new cells added */

  cell_count = 0;

  /* Check for cells for meas */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(mcm_cell_list_ptr,
    tdsrrc_InterRATCellInfoList,cellsForInterRATMeasList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cells for meas present");
    l1_cell_list_ptr->cells_for_meas_incl = TRUE;

    local_cells_for_meas_ptr = &mcm_cell_list_ptr->cellsForInterRATMeasList;
    idx=0;
    while (local_cells_for_meas_ptr->n > idx)
    {
      cell_id = (uint16) local_cells_for_meas_ptr->elem[idx];

      /* Check if the cell id is present */
      if( tdsrrcirat_find_cell_id(&cell_id) == FAILURE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"cell to measure with id = %d not found",cell_id);

      } /* End of if for cell id not found */
      else
      {
         /* Store cell id */
         l1_cell_list_ptr->meas_cells_list[cell_count++] = cell_id;
      }

      /* Increment Ptr */
      idx++;
    } /* End of while */

    l1_cell_list_ptr->num_cell_meas =  cell_count;

  } /* End of if of rrc_cellsForIntraFreqMeasList_present */

  return SUCCESS;
}


/*===========================================================================

FUNCTION   RRCIRAT_INIT_SIB11_PARAMS

DESCRIPTION

  This function initializes SIB11 params for Inter-RAT Meas

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If measured results are valid
  SUCCESS: If measured results are invalid.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_init_sib11_params
(
  tdsl1_meas_sib_parm_struct_type* l1_sib11_inter_rat_ptr
)
{
  l1_sib11_inter_rat_ptr->fach_meas_incl = FALSE;
  l1_sib11_inter_rat_ptr->hcs_used = FALSE;
  l1_sib11_inter_rat_ptr->inter_sys_meas_info_incl = TRUE;
  l1_sib11_inter_rat_ptr->inter_s.num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
  l1_sib11_inter_rat_ptr->inter_s.num_cell_add = 0;
  l1_sib11_inter_rat_ptr->inter_s.cells_for_meas_incl = FALSE;
}


/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_EVENT_CRITERIA

DESCRIPTION

  This function processes the event criteria received in a Measurement Control Message
    and stores in the format that is sent to L1

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If event criteria is invalid
  SUCCESS: If event criteria is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_event_criteria
(
  tdsrrc_InterRATReportingCriteria* rrc_event_criteria_ptr,
  tdsl1_inter_sys_report_crit_struct_type* l1_event_criteria_ptr
)
{
  /* Store the list in a local pointer */
  tdsrrc_InterRATEventList* rrc_local_list_ptr = NULL;
  uint32 idx=0;
  uint8 cnt = 0;

  rrc_local_list_ptr = &rrc_event_criteria_ptr->interRATEventList;

  while ((rrc_local_list_ptr->n > idx) && (cnt < TDSL1_MAX_MEAS_EVENT))
  {
    /* Get Event Id */
    l1_event_criteria_ptr->evt[cnt].evt_id =
      TDSRET_INTER_RAT_EVENT_TYPE(rrc_local_list_ptr->elem[idx].t);

    switch(l1_event_criteria_ptr->evt[cnt].evt_id)
    {
    case TDSL1_INTER_RAT_EVENT_3A:
    /* If The estimated quality of the currently used UTRAN frequency
    is below a certain threshold and the estimated quality of the
      other system is above a certain threshold.*/

      /* Get own threshold */
      l1_event_criteria_ptr->evt[cnt].own_thresh = (int16)
        rrc_local_list_ptr->elem[idx].u.event3a->thresholdOwnSystem;

      /* Get W */
      l1_event_criteria_ptr->evt[cnt].w = (uint16)
        rrc_local_list_ptr->elem[idx].u.event3a->w;

      /* Get Threshold other system */
      l1_event_criteria_ptr->evt[cnt].other_thresh = (int16)
        rrc_local_list_ptr->elem[idx].u.event3a->thresholdOtherSystem;

      /* Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        rrc_local_list_ptr->elem[idx].u.event3a->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_RAT_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[idx].u.event3a->timeToTrigger);

      l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;

      /* Check if reporting cell status present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_local_list_ptr->elem[idx].u.event3a,
        tdsrrc_Event3a,reportingCellStatus))
      {
        l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;

        if (tdsrrcmeas_fill_no_of_reporting_cells(&rrc_local_list_ptr->elem[idx].u.event3a->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No of Reporting Cells incorrect");
          return FAILURE;
        }
      }


      break;


    case TDSL1_INTER_RAT_EVENT_3B:
      /* The estimated quality of other system is below a certain threshold. */

      /* Get Threshold other system */
      l1_event_criteria_ptr->evt[cnt].other_thresh = (int16)
        rrc_local_list_ptr->elem[idx].u.event3b->thresholdOtherSystem;

      /* Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        rrc_local_list_ptr->elem[idx].u.event3b->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_RAT_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[idx].u.event3b->timeToTrigger);

      l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;

      /* Check if reporting cell status present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_local_list_ptr->elem[idx].u.event3b,
        tdsrrc_Event3b,reportingCellStatus))
      {
        l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;

        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &rrc_local_list_ptr->elem[idx].u.event3b->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No Of Reporting Cells incorrect");
          return FAILURE;
        }
      }

      break;

    case TDSL1_INTER_RAT_EVENT_3C:
      /* Event 3c: The estimated quality of other system is above a certain threshold. */

      /* Get Threshold other system */
      l1_event_criteria_ptr->evt[cnt].other_thresh = (int16)
        rrc_local_list_ptr->elem[idx].u.event3c->thresholdOtherSystem;

      /* Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        rrc_local_list_ptr->elem[idx].u.event3c->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_RAT_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[idx].u.event3c->timeToTrigger);

      l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;

      /* Check if reporting cell status present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_local_list_ptr->elem[idx].u.event3c,
        tdsrrc_Event3b,reportingCellStatus))
      {
        l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;

        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &rrc_local_list_ptr->elem[idx].u.event3c->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No Of Reporting Cells incorrect");
          return FAILURE;
        }
      }

      break;

    case TDSL1_INTER_RAT_EVENT_3D:
      /* Event 3d: Change of best cell in other system */
      /* Hysterisis */
      l1_event_criteria_ptr->evt[cnt].hyst = (uint16)
        rrc_local_list_ptr->elem[idx].u.event3d->hysteresis;

      /* Time to trigger */
      l1_event_criteria_ptr->evt[cnt].time_to_trig =
        TDSRET_INTER_RAT_TIME_TO_TRIGGER(
        rrc_local_list_ptr->elem[idx].u.event3d->timeToTrigger);

      l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = FALSE;

      /* Check if reporting cell status present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_local_list_ptr->elem[idx].u.event3d,
         tdsrrc_Event3d,reportingCellStatus))
      {
        l1_event_criteria_ptr->evt[cnt].rpt_cell_status_incl = TRUE;

        if (tdsrrcmeas_fill_no_of_reporting_cells(
          &rrc_local_list_ptr->elem[idx].u.event3d->reportingCellStatus,
          &l1_event_criteria_ptr->evt[cnt].rpt_cell_status) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No Of Reporting Cells incorrect");
          return FAILURE;
        }
      }

      break;

    } /* End of switch */

    cnt++;

    /* Increment Pointer count */
    idx++;

  } /* End of while */

  /* Store the count */
  l1_event_criteria_ptr->num_event = cnt;

  return SUCCESS;

}


/*===========================================================================

FUNCTION   RRCIRAT_CHECK_INTER_RAT_CELL_ORDER_MCM

DESCRIPTION

  This function processes the cell list received in a Measurement Control Message
  and checks if there are cells with same cell identity.


DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If 2 cells have the same cell identity
  SUCCESS: If no 2 cells have the same cell identity

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_check_inter_rat_cell_order_mcm
(
  tdsrrc_NewInterRATCellList   *local_inter_rat_cell_list_ptr
)
{
  /* Find the cell order */

  uint32 cell_id_list[TDSL1_MAX_CELL_MEAS];
  uint16 count=0, list_count1 =0, list_count2 = 0;

  /* Initialize addl meas list to satisfy lint */
  for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
  {
    cell_id_list[count] = 0;
  }

  count = 0;

  /* Store cell id's in a local list and calculate total no of cells */
  while ((local_inter_rat_cell_list_ptr->n > list_count1) &&(count < TDSL1_MAX_CELL_MEAS))
  {

    if (local_inter_rat_cell_list_ptr->elem[list_count1].m.interRATCellIDPresent)

    {
      /* Store cell id */
      cell_id_list[count] = local_inter_rat_cell_list_ptr->elem[list_count1].interRATCellID;
      count++;
    }
    /* Cell id was never present which is very likely */
    list_count1++;
  }

  if (count == 0 )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell list: No cell Id's");
    /* Return successfully as cells would now be stored
    in first available position */
    return SUCCESS;
  }
  if (count == 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell list: 1 cell with id");
    /* So no comparision has to be done. */
    return SUCCESS;
  }
  /* This means that there are multiple cells in the cell list and there
  order needs to be verified */

  /* Compare 1 st with 2,3,4..... Then 2nd with 3,4,5... and so on */
  for (list_count1 = 0; list_count1 < (count-1); list_count1++)
  {
    for (list_count2 = list_count1+1; list_count2 < count; list_count2++)
    {
      if (cell_id_list[list_count1] == cell_id_list[list_count2])
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"2 GSM cells=> Same CellId");
        /* Cell Id's match */
        return FAILURE;

      }

    } /* End of for for list_count2 */
  }  /* End of for for list_count1 */
#ifdef TDSDEBUG_MSG
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Inter-RAT Cell list has %d cells",count);
#endif
  return SUCCESS;
}


/*===========================================================================

FUNCTION   RRCIRAT_FILL_REMOVED_INTER_RAT_CELLS

DESCRIPTION

  This function processes the removed cell list received in a Measurement Control Message


DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If removed cell list is valid
  SUCCESS: If removed cell list is invalid

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_fill_removed_inter_rat_cells
(
  tdsrrc_RemovedInterRATCellList* rrc_remove_cells_ptr,
  tdsl1_inter_sys_cell_list_struct_type* l1_cell_list_ptr
)
{
  /* To store the temp pointer which reads element from the rrc__seqof3 list */
  uint8 removed_cell_count = 0;

  /* Intra Freq Cell Id */
  uint16 inter_rat_cell_id = 0;

  tdsrrc_RemovedInterRATCellList_removeSomeInterRATCells* local_rrc_removed_ptr = NULL;
  uint32 idy =0;
  switch ( rrc_remove_cells_ptr->t )
  {
  case T_tdsrrc_RemovedInterRATCellList_removeAllInterRATCells:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Remove all Inter-RAT cells");
    l1_cell_list_ptr->num_cell_rmv = TDSL1_REMOVE_ALL_CELLS;
    /* Remove all cells in tdslocal_neighbor_list for inter-RAT */
    for (removed_cell_count=0; removed_cell_count<TDSL1_MAX_CELL_MEAS; removed_cell_count++ )
    {
      tdslocal_neighbor_list[removed_cell_count].cell_id = TDSINVALID_CELL_ID;
      tdslocal_neighbor_list[removed_cell_count].cell_position = TDSVACANT;
    }
    break;

  case T_tdsrrc_RemovedInterRATCellList_removeSomeInterRATCells:
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Remove some Inter-RAT cells");

    /* Local ptr to store removed cells */
    local_rrc_removed_ptr = (rrc_remove_cells_ptr->u.removeSomeInterRATCells);


    if ( local_rrc_removed_ptr == NULL )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Inter-RAT removed cell list ptr NULL");
      return FAILURE;
    }

    do
    {
      /* Get cell id */
      inter_rat_cell_id =  (uint16) local_rrc_removed_ptr->elem[idy];
      /* Find cpi of the cell id */
      if (tdsrrcirat_find_cell_id(&inter_rat_cell_id) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCM:Inter RAT Cell to be removed not found");
        /*  Get next element in the list */
        idy++;
        continue;
      }
      else
      {
        /* Removes cell from local neighbor list */
        tdsrrcirat_remove_cell_id(&inter_rat_cell_id);

        /* Store this removed cell id for L1 */
        l1_cell_list_ptr->rmv_cell[removed_cell_count].intersys_cell_id =
          inter_rat_cell_id;
      }
      removed_cell_count++; /* Increment count */

      /*  Get next element in the list */
      idy++;


    } /* Continue if cond satisfied */

    while ( (local_rrc_removed_ptr->n > idy) &&
      (removed_cell_count < TDSL1_MAX_CELL_MEAS) );

    /* Now store the number of actual cells sent by UTRAN */

    l1_cell_list_ptr->num_cell_rmv =  removed_cell_count;

    break;

  case T_tdsrrc_RemovedInterRATCellList_removeNoInterRATCells:

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Remove 0 Inter-RAT cells");

    l1_cell_list_ptr->num_cell_rmv = 0;
    break;

  default:
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Choice");
    return FAILURE;

  } /* End of switch */

  return SUCCESS;
}


/*===========================================================================

FUNCTION   RRCIRAT_FIND_CELL_ID

DESCRIPTION

  This function processes the inter rat cell id


DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If cell id is valid
  SUCCESS: If cell id is invalid

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_find_cell_id
(
 uint16* intra_freq_cell_id_ptr
)
{
  uint8 count = 0; /* Count variable */

  for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
  {
    /* Found if cell id exists */
    if ((tdslocal_neighbor_list[count].cell_position == TDSOCCUPIED) &&
      (tdslocal_neighbor_list[count].cell_id == *intra_freq_cell_id_ptr))
    {
      return SUCCESS;
    }
  }

  return FAILURE;
}


/*===========================================================================

FUNCTION   RRCIRAT_REMOVE_CELL_ID

DESCRIPTION

  This function removes the inter rat cell id


DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_remove_cell_id
(
 uint16* inter_rat_cell_id_ptr
)
{
  uint8 count = 0; /* Count variable */
  boolean cell_found = FALSE;

  for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
  {
    if ((count == (*inter_rat_cell_id_ptr)) &&
      (tdslocal_neighbor_list[count].cell_position == TDSOCCUPIED))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Removing cell id with id %d",*inter_rat_cell_id_ptr);

      tdslocal_neighbor_list[count].cell_position = TDSVACANT;

      tdslocal_neighbor_list[count].cell_id =  TDSINVALID_CELL_ID;

      cell_found = TRUE;

    }
  } /* End of for loop */

  if (cell_found == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rem:Inter-RAT cell absent in local neighbor list");
  }

  return;
}


/*===========================================================================

FUNCTION   RRCIRAT_UPDATE_INTER_RAT_CELL_LIST_MCM

DESCRIPTION

  This function updates the inter rat cell list received in a Meas Ctrl Msg


DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If cell list is valid
  SUCCESS: If cell list is invalid

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_update_inter_rat_cell_list_mcm
(
 uint32* position_no_ptr, /* First availabble position or cell id */
 uint16* cell_id_ptr,  /* L1 value */
 uint16* cell_id_to_remove_ptr,  /* Determines if a cell has to be removed */
 uint16 arfcn
)
{
  uint16 count = 0;
  

  /* First check if one has to add this in the first available position */
  if (*position_no_ptr == TDSAVAILABLE_CELL_POSITION)
  {
    for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
    {
      if (tdslocal_neighbor_list[count].cell_position == TDSVACANT)
      {
        /* Store this cell in this first available vacant position */
        tdslocal_neighbor_list[count].cell_position = TDSOCCUPIED;
        tdslocal_neighbor_list[count].cell_id = count;
        tdslocal_neighbor_list[count].arfcn = arfcn;

        /* This is the new cell id. So store it back in l1 struct */
        *cell_id_ptr = count;

        *cell_id_to_remove_ptr = TDSINVALID_CELL_ID;

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Add:Cell added at pos %d with id = %d arfcn=%d",
                    count,*cell_id_ptr,tdslocal_neighbor_list[count].arfcn);

        return SUCCESS;
      }
    } /* End of for */
  } /* End of if of  position_no_ptr == TDSAVAILABLE_CELL_POSITION */

    /* position_no_ptr and cell_id_ptr has same value for cell
       to be stored when cell id given */
  /* Now check which position the cells needs to be updated */
  for (count = 0; count < TDSL1_MAX_CELL_MEAS; count++)
  {
    /* Check if there is already a cell in this position
       If yes, then this cell has to be removed and L1
       has to be informed about it. So store its cell id  */

    if ( (count == *position_no_ptr) &&
      (tdslocal_neighbor_list[count].cell_position == TDSOCCUPIED) )
    {

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Add:Cell Id %d at pos. %d removed[Cascading] by %d",
        tdslocal_neighbor_list[count].cell_id,count,*cell_id_ptr);


      /* Store the removed cell id */
      *cell_id_to_remove_ptr = tdslocal_neighbor_list[count].cell_id;

      /* Store the new cell id at this position */
      tdslocal_neighbor_list[count].cell_id = *cell_id_ptr;
      tdslocal_neighbor_list[count].arfcn = arfcn;

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Replace tdslocal_neighbor_list[%d].arfcn=%d",tdslocal_neighbor_list[count].arfcn);

      return SUCCESS;
    }
    else if ( (count == *position_no_ptr) &&
      (tdslocal_neighbor_list[count].cell_position == TDSVACANT) )
    {

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Add:Adding cell at pos %d with id %d",count,*cell_id_ptr);


      /* Store the new cpi at this position */
      tdslocal_neighbor_list[count].cell_id = *cell_id_ptr;

      /* Mark the position as occupied. NOT REQD */
      tdslocal_neighbor_list[count].cell_position = TDSOCCUPIED;


      tdslocal_neighbor_list[count].arfcn = arfcn;

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Add tdslocal_neighbor_list[%d].arfcn=%d",tdslocal_neighbor_list[count].arfcn);

      /* No cell id to be removes as this place is vacant */
      *cell_id_to_remove_ptr = TDSINVALID_CELL_ID;

      return SUCCESS;

    } /* End of else if */
  } /* End of for */
  TDSRRC_MSG1(MSG_LEGACY_ERROR,"Inter-RAT cell ID = %d not valid",*cell_id_ptr);
  return FAILURE;
}


/*===========================================================================

FUNCTION   RRCIRAT_INITIALIZE_CELL_LIST

DESCRIPTION

  This function initializes the inter rat cell list


DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_initialize_cell_list(void)
{
  uint8 cnt = 0;
  tdsrrcirat_initialize_local_neighbor_list();
  for (cnt =0; cnt <TDSL1_MAX_CELL_MEAS; cnt++ )
  {
    tdsinter_rat_cell_list[cnt].cell_id = TDSINVALID_CELL_ID;
    tdsinter_rat_cell_list[cnt].cell_position = TDSVACANT;
  }

  tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
  tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;

#ifdef FEATURE_TDSCDMA_TO_LTE
  tdsrrcirat_initialize_eutra_freq_list();
#endif
}


/*===========================================================================

FUNCTION   RRCIRAT_INITIALIZE_MEAS_ID_LIST

DESCRIPTION

  Handles initialization of MEASUREMNT_IDENTIY variable
  for Iner RAT Measurements.

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcirat_initialize_meas_id_list
(
 void
)
{
  uint8 count = 0, cnt = 0;
  /* Initialize Measurement Identity variable */
  TDSRRC_MSG0(MSG_LEGACY_LOW,"Initializing Inter-RAT Meas Id List");
  for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
  {
    /* Set all positions to vacant and make CPID as invalid */
    tdsinter_rat_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
    tdsinter_rat_meas_id_list[count].meas_validity = TDSSTATE_DCH;
    tdsinter_rat_meas_id_list[count].rpt_crit = TDSNOT_AVAILABLE;
    tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
    /* Also initialize additional meas */
    for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
      tdsinter_rat_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;

    tdsinter_rat_meas_id_list[count].rpt_bsic_verified = FALSE;
  }
#ifdef FEATURE_ENH_BSIC_VERIFY
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"FEATURE_ENH_BSIC_VERIFY defined");
#else
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"FEATURE_ENH_BSIC_VERIFY not defined");
#endif

}


/*===========================================================================

FUNCTION   tdsrrcirat_initialize_local_neighbor_list

DESCRIPTION

  This function initializes the local neighbor list


DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_initialize_local_neighbor_list(void)
{
  uint8 cnt = 0;
  for (cnt =0; cnt <TDSL1_MAX_CELL_MEAS; cnt++ )
  {
    tdslocal_neighbor_list[cnt].cell_id = TDSINVALID_CELL_ID;
    tdslocal_neighbor_list[cnt].cell_position = TDSVACANT;
  }
}

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
/*===========================================================================

FUNCTION   tdsrrcirat_is_meas_id_eutra_meas

DESCRIPTION
  This function returns TRUE if the meas id is EUTRA meas id

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the meas id is found in DB and is EUTRA measurement
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcirat_is_meas_id_eutra_meas
(
  uint16 meas_id
)
{
  uint8 cnt=0;
  boolean status = FALSE;
  for(cnt = 0;cnt < TDSMAX_INTER_RAT_MEAS ; cnt++)
  {
    if((meas_id == tdsinter_rat_meas_id_list[cnt].meas_id) && (tdsinter_rat_meas_id_list[cnt].interrat_type == TDSRRCMEAS_IRAT_EUTRA))
    {
      status = TRUE;
      break;
    }
  }
  return status;
}

/*===========================================================================

FUNCTION   tdsrrcirat_send_eutra_meas_report

DESCRIPTION
  This function populates EUTRA measurement report based on WL1 reports and submits the report to RLC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcirat_send_eutra_meas_report
(
  tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
)
{
  /* Stores meas report */
  tdsrrc_UL_DCCH_Message* msg_ptr = NULL;
  /* Indicates if measured results would be appended to event results */
  boolean measured_res_incl = FALSE;

  /* RLC ack or unack mode */
  uecomdef_logch_mode_e_type logical_channel_mode;

  /* Logical Channel Id */
  rlc_lc_id_type ul_lc_id;

  /* Radio bearer mode */
  tdsrrc_RB_Identity rb_mode;

  /* L2 ack status */
  tdsrrcsend_l2ack_status_e_type l2_ack_status;

  uint8 meas_id_cnt,cnt;

  /* Stores status of send chain */
  tdsrrcsend_status_e_type status;

  /*Validate the measurement results from WL1, TBD: Do we really need this, If WL1 takes care of this stuff, then not needed*/
  if(l1_meas_report_ptr->meas_type != TDSL1_INTER_SYS_EUTRA)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRC has the meas_id %d as EUTRA where as L1 indicated meas_type as %d",l1_meas_report_ptr->meas_id,l1_meas_report_ptr->meas_type);
    return;
  }

  if(0 == l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.measured_eutra_freqs)
  {
    if(l1_meas_report_ptr->rpt_crit == TDSL1_PERIODIC_RPT)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Measurement results absent");
      /* allowed empty MCR  to go eventhough no eutra freq present, done as part of cr fix */
      //return;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Measurement results absent");
    }
  }
  else
  {
    measured_res_incl = TRUE;
  }

  if ( l1_meas_report_ptr->meas_transfer_mode == TDSL1_ACKNOWLEDGED_MODE )
  {
    logical_channel_mode = UE_MODE_ACKNOWLEDGED_DATA;
    rb_mode = DCCH_AM_RADIO_BEARER_ID;
    l2_ack_status = TDSRRCSEND_L2ACK_REQUIRED;
  }
  else
  {
    logical_channel_mode = UE_MODE_UNACKNOWLEDGED;
    rb_mode = DCCH_UM_RADIO_BEARER_ID;
    l2_ack_status = TDSRRCSEND_L2ACK_NOT_REQUIRED;
  }

  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH, rb_mode,
    logical_channel_mode);


  if ( ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
  {
    ERR("RLC Logical channel not found", 0,0,0);
    return;
  }

  msg_ptr = (tdsrrc_UL_DCCH_Message *) tdsrrc_malloc (sizeof(struct tdsrrc_UL_DCCH_Message));

  if ( msg_ptr == NULL )
  {
    ERR_FATAL("Failed to allocate memory ",0,0,0);
  }

  /* Store meas report chosen */
  msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_measurementReport;


  /* First store the meas id */
  msg_ptr->message.u.measurementReport.measurementIdentity =
   (tdsrrc_MeasurementIdentity) l1_meas_report_ptr->meas_id;

  /* Initialize Meas Report bit mask */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport);

  /* First store the meas id */
  msg_ptr->message.u.measurementReport.measurementIdentity =
    l1_meas_report_ptr->meas_id;

   /* Malloc memory for MeasResult_v4b0  */
  tdsrrcmeas_set_measurement_result_v4b0ext(msg_ptr);

  TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.measurementReport,v390nonCriticalExtensions);
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.measurementReport_v390ext);
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions);

  TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions,laterNonCriticalExtensions);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions);

  TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions,v4b0NonCriticalExtensions);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions);

  TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions,
    v590NonCriticalExtensions);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.measurementReport_v590ext);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions);


  TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions,v5b0NonCriticalExtensions);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions);

  TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions,v690NonCriticalExtensions);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.measurementReport_v690ext);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions);

  TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions,v770NonCriticalExtensions);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
    measurementReport_v770ext);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions);

  TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions,v860NonCriticalExtensions);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
    v860NonCriticalExtensions.measurementReport_v860ext);
  TDSRRC_RESET_MSG_IE_PRESENT(
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
    v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.v860NonCriticalExtensions);

  if((l1_meas_report_ptr->rpt_crit == TDSL1_EVENT_TRIGGER_RPT) && 
    (0 != l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.num_freq))
  {
    tdsrrc_Eutra_EventResultList *local_event_list_ptr = NULL;
    uint8 freq_count, cell_count,num_valid_freq;

    num_valid_freq = 0;

    for(freq_count = 0;freq_count < l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.num_freq && 
          freq_count <TDSL1_MAX_REP_EUTRA_FREQ ;freq_count++)
    {
      if( 0 != l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.freqs[freq_count].num_cell)
      {
        num_valid_freq++;
      }
    }
    if(num_valid_freq !=0)
    {
      uint8 freq_idx =0;
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
        v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
        v860NonCriticalExtensions.measurementReport_v860ext,eutra_EventResults);

      tdsrrcirat_set_inter_sys_event_id(&l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.event_id,
         &msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
             v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
             v860NonCriticalExtensions.measurementReport_v860ext.eutra_EventResults.eventID
          );
      local_event_list_ptr = & msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                       v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
                       v860NonCriticalExtensions.measurementReport_v860ext.eutra_EventResults.eutra_EventResultsList;
      local_event_list_ptr->n = num_valid_freq;
      ALLOC_ASN1ARRAY1(&tdsenc_ctxt,local_event_list_ptr,tdsrrc_Eutra_EventResult);
      if(NULL == local_event_list_ptr->elem)
      {
        ERR_FATAL("Couldn't allocate memory for Event frequencies",0,0,0);
      }
      for(freq_count = 0;freq_count < l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.num_freq && 
            freq_count <TDSL1_MAX_REP_EUTRA_FREQ && freq_idx < num_valid_freq && freq_idx < TDSL1_MAX_REP_EUTRA_FREQ ;freq_count++)
      {
        tdsrrc_Eutra_EventResult_reportedCells *cell_list_ptr = NULL;
        if( 0 == l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.freqs[freq_count].num_cell)
        {
          continue;
        }
        local_event_list_ptr->elem[freq_idx].earfcn = (tdsrrc_EARFCN)l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.freqs[freq_count].earfcn;
        cell_list_ptr = &local_event_list_ptr->elem[freq_idx].reportedCells;
        cell_list_ptr->n = 
          (l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.freqs[freq_count].num_cell > TDSL1_MAX_REP_EUTRA_CELLS_PER_FREQUENCY)
          ? TDSL1_MAX_REP_EUTRA_CELLS_PER_FREQUENCY :l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.freqs[freq_count].num_cell ;
        ALLOC_ASN1ARRAY1(&tdsenc_ctxt,cell_list_ptr,tdsrrc_EUTRA_PhysicalCellIdentity);
        if(NULL == cell_list_ptr->elem)
        {
          ERR_FATAL("Unable to allocate memory for reporting LTE cells",0,0,0);
        }
        for(cell_count = 0; cell_count < l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.freqs[freq_count].num_cell &&
              cell_count < TDSL1_MAX_REP_EUTRA_CELLS_PER_FREQUENCY ; cell_count++)
        {
          cell_list_ptr->elem[cell_count] = l1_meas_report_ptr->u.inter_sys_eutra_rpt.evt_res.freqs[freq_count].phy_cell_id[cell_count];
        }
        freq_idx++;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TD2L: No LTE cells present in Event report from TDL1");
    }
  }

  if(measured_res_incl)
  {
    tdsrrc_Eutra_MeasuredResultList *local_meas_list_ptr = NULL;
    uint8 freq_count, cell_count,num_valid_freq;

    num_valid_freq = 0;
    for(freq_count = 0;freq_count < l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.measured_eutra_freqs && 
          freq_count <TDSL1_MAX_REP_EUTRA_FREQ ;freq_count++)
    {
      if( 0 != l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].num_cell)
      {
        num_valid_freq++;
      }
    }
    if(num_valid_freq != 0)
    {
      uint8 freq_idx =0;
      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(
        msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
        v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
        v860NonCriticalExtensions.measurementReport_v860ext,eutra_MeasuredResults);
      local_meas_list_ptr = & msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.
                       v590NonCriticalExtensions.v5b0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.
                       v860NonCriticalExtensions.measurementReport_v860ext.eutra_MeasuredResults.eutraMeasuredResultList;
      local_meas_list_ptr->n = num_valid_freq;
      ALLOC_ASN1ARRAY1(&tdsenc_ctxt,local_meas_list_ptr,tdsrrc_Eutra_MeasuredResult);
      if(NULL == local_meas_list_ptr->elem)
      {
        ERR_FATAL("Couldn't allocate memory for measured LTE frequencies",0,0,0);
      }
      for(freq_count = 0;freq_count < l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.measured_eutra_freqs && 
            freq_count <TDSL1_MAX_REP_EUTRA_FREQ && freq_idx < num_valid_freq && freq_idx < TDSL1_MAX_REP_EUTRA_FREQ;freq_count++)
      {
        tdsrrc_Eutra_MeasuredResult_measuredEUTRACells * cell_list_ptr = NULL;
        if( 0 == l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].num_cell)
        {
          continue;
        }
        local_meas_list_ptr->elem[freq_idx].earfcn = (tdsrrc_EARFCN)l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].earfcn;
        cell_list_ptr = & local_meas_list_ptr->elem[freq_idx].measuredEUTRACells;
        cell_list_ptr->n = 
          (l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].num_cell > TDSL1_MAX_REP_EUTRA_CELLS_PER_FREQUENCY) ?
          TDSL1_MAX_REP_EUTRA_CELLS_PER_FREQUENCY : l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].num_cell;
        ALLOC_ASN1ARRAY1(&tdsenc_ctxt,cell_list_ptr,tdsrrc_EUTRA_MeasuredCells);
        if(NULL == cell_list_ptr->elem)
        {
           ERR_FATAL("Memory allocation failed",0,0,0);
        }
        for(cell_count = 0; cell_count < l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].num_cell &&
              cell_count < TDSL1_MAX_REP_EUTRA_CELLS_PER_FREQUENCY ; cell_count++)
        {
          cell_list_ptr->elem[cell_count].physicalCellIdentity = l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].cells[cell_count].phy_cell_id;
          TDSRRC_RESET_MSG_IE_PRESENT(cell_list_ptr->elem[cell_count]);
          if(l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].cells[cell_count].rsrq_incl)
          {
            TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(cell_list_ptr->elem[cell_count],rSRQ);
            cell_list_ptr->elem[cell_count].rSRQ =  l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].cells[cell_count].rsrq;
          }
          if(l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].cells[cell_count].rsrp_incl)
          {
            TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(cell_list_ptr->elem[cell_count],rSRP);
            cell_list_ptr->elem[cell_count].rSRP =  l1_meas_report_ptr->u.inter_sys_eutra_rpt.meas_res.freqs[freq_count].cells[cell_count].rsrp;
          }
        }
        freq_idx ++;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"W2L: No LTE cells present in measured results from WL1");
    }

    /* Now check if this is the last report. Meas Id needs to be deleted if true */
    if (l1_meas_report_ptr->last_periodic_report == TRUE)
    {
      /* Delete this Measurement */
      for (meas_id_cnt =0; meas_id_cnt < TDSMAX_INTER_RAT_MEAS; meas_id_cnt++ )
      {
        if (tdsinter_rat_meas_id_list[meas_id_cnt].meas_id == l1_meas_report_ptr->meas_id)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete Meas %d",l1_meas_report_ptr->meas_id);
          tdsinter_rat_meas_id_list[meas_id_cnt].meas_id = TDSINVALID_MEAS_ID;
  
          tdsinter_rat_meas_id_list[meas_id_cnt].rpt_crit = TDSNOT_AVAILABLE;

          /* Also initialize additional meas */
          for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
            tdsinter_rat_meas_id_list[meas_id_cnt].additional_meas[cnt] = TDSINVALID_MEAS_ID;

          break;
        }
      }
    }

    /* Now delete this meas from addtl meas DB */
    if (l1_meas_report_ptr->last_periodic_report == TRUE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete %d from addtl meas DB",l1_meas_report_ptr->meas_id);
      tdsrrcmeas_delete_addtl_meas(&l1_meas_report_ptr->meas_id);
    }
  }

  /* Append Additional Measured Results if available */
  tdsrrcmeas_process_additional_measured_results(
    &l1_meas_report_ptr->meas_id, TDSINTER_SYS, &msg_ptr->message.u.measurementReport);

  /* Closed measurement_result_v4b0ext if malloc memory no use*/
  tdsrrcmeas_close_measurement_result_v4b0ext(msg_ptr);

  /* This places SDU into RLC watermark queue */
  status = tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_MCMR, (void *) msg_ptr,
    ul_lc_id, l2_ack_status);
  if ( status != TDSRRCSEND_SUCCESS )
  {
    ERR("Send chain failed to send Inter-RAT Meas Rpt", 0, 0, 0);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"EUTRA Inter-RAT Meas Rpt sent from RRC=>RLC");
    if(l1_meas_report_ptr->meas_transfer_mode == TDSL1_ACKNOWLEDGED_MODE)
    {
      tdsrrcmeas_pending_rpt++;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"pending meas rpt = %d",tdsrrcmeas_pending_rpt);
    }
  }

}

/*===========================================================================

FUNCTION tdsrrcirat_delete_mcm_due_to_irat_type_change

DESCRIPTION

 The function checks if the new meas id is already used for a different meas type
 and if so releases the measurement from the primary measurement database, from
 the additional measurement dtabase and send a release command to L1 to release
 the measurement.

DEPENDENCIES
 None

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/
static void tdsrrcirat_delete_mcm_due_to_irat_type_change
(
  uint16 tdsmeas_id
)
{
  uint8 count = 0;
  uint8 cnt = 0;
  boolean meas_found = FALSE;
  tdsl1_meas_ctrl_parm_struct_type l1_mcm;

  memset(&l1_mcm, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  for(count = 0; count < TDSMAX_INTER_RAT_MEAS; count++)
  {
    if(tdsinter_rat_meas_id_list[count].meas_id == tdsmeas_id)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Releasing earlier INTER RAT meas with id %d",tdsmeas_id);
      meas_found = TRUE;
      /* Store meas type */
      l1_mcm.meas_object.meas_type = TDSL1_INTER_SYS;
      tdsinter_rat_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;

      /* Also initialize additional meas */
      for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
        tdsinter_rat_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;

      /* Delete additional meas */
      tdsrrcmeas_delete_addtl_meas(&tdsmeas_id);
      break;
    }
  }
  
  if(meas_found)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending MCM release to L1 due to inter RAT type change when setup new MCM");
    /* With release meas, additional meas should not be present */
    /* Set meas id in L1 struct */
    l1_mcm.meas_id = tdsmeas_id;

    /* Set trans Id in var trans_id */
    l1_mcm.trans_id = TDSL1_NO_CNF_TRANSACTION_ID;//TBD
    l1_mcm.meas_object_info_included = FALSE;

    /* Set command type in L1 struct */
    l1_mcm.meas_cmd = TDSL1_MEAS_RELEASE;
    tdsrrcmeas_send_l1_cmd(NULL, &l1_mcm, NULL, TDSL1_MEAS_CTRL_PARMS);
    return;
  }
  
  return;
}
#endif

/*===========================================================================

FUNCTION   RRCIRAT_SEND_MEAS_REPORT

DESCRIPTION

  This function sends the Measurement Report to UTRAN in CELL_DCH only

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_send_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
)
{
  /* Stores meas report */
  tdsrrc_UL_DCCH_Message* msg_ptr = NULL;

  /* Used as a counter to store cell number counted */
  uint8 cell_number = 0;

  /* GSM Cell Count */
  uint8 gsm_cell_count = 0;

  /* Event Cell Count */
  uint8 event_cell_count = 0;

  /* Stores status of send chain */
  tdsrrcsend_status_e_type status;

  /* Stores RSSI value */
  uint8 rssi_value;

  uint8 meas_id_cnt = 0, cnt = 0;

  /* Indicates if measured results would be appended to event results */
  boolean measured_res_incl = FALSE;

  /* RLC ack or unack mode */
  uecomdef_logch_mode_e_type logical_channel_mode;

  /* Logical Channel Id */
  rlc_lc_id_type ul_lc_id;

  /* Radio bearer mode */
  tdsrrc_RB_Identity rb_mode;

  /* L2 ack status */
  tdsrrcsend_l2ack_status_e_type l2_ack_status;

  /* Contains ptr to the head of results of Inter-RAT Measured Results */
  tdsrrc_InterRATMeasuredResultsList* local_rrc_measured_results_ptr = NULL;

  /* Contains Ptr to the head of GSM cells in one Inter-RAT system */
  tdsrrc_GSM_MeasuredResultsList* local_rrc_gsm_list_ptr = NULL;

  tdsrrc_CellToReportList* local_cell_list_ptr = NULL;


  /* Sanity Tests before building Measurement Report */
  /* Check if reporting criteria is O.K. */
  if (l1_meas_report_ptr->rpt_crit == TDSL1_NO_RPT ||
    l1_meas_report_ptr->rpt_crit == TDSL1_NO_CHANGE )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rejecting Inter-RAT Report. R.Crit=%d",l1_meas_report_ptr->rpt_crit);
    return;
  }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
  if(tdsrrcirat_is_meas_id_eutra_meas(l1_meas_report_ptr->meas_id))
  {
    /*Call the EUTRA function to report EUTRA measurements*/
    tdsrrcirat_send_eutra_meas_report(l1_meas_report_ptr);
    return;
  }
#endif

  if (l1_meas_report_ptr->rpt_crit == TDSL1_PERIODIC_RPT)
  {
    /* Validate Measured Results */
    if (tdsrrcirat_validate_measured_results(
      &l1_meas_report_ptr->u.inter_sys_rpt.measurements) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Measured Results");
      return;
    }
    else
    {
      measured_res_incl = TRUE;
    }
  }
  else
  {
    if (l1_meas_report_ptr->rpt_crit == TDSL1_EVENT_TRIGGER_RPT)
    {
      /* Validate event results */
      if (tdsrrcirat_validate_event_results(&l1_meas_report_ptr->u.inter_sys_rpt.event_results) ==
        FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Meas Report not sent");
        return;
      }

      /* Now check if measured results are present */
      if (tdsrrcirat_validate_measured_results(
        &l1_meas_report_ptr->u.inter_sys_rpt.measurements) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore Measured Results");
        /* Nevertheless send event report only */
      }
      else
      {
        measured_res_incl = TRUE;
      }
    }
  } /* End of else of event triggered report */

  if ( l1_meas_report_ptr->meas_transfer_mode == TDSL1_ACKNOWLEDGED_MODE )
  {
    logical_channel_mode = UE_MODE_ACKNOWLEDGED_DATA;
    rb_mode = DCCH_AM_RADIO_BEARER_ID;
    l2_ack_status = TDSRRCSEND_L2ACK_REQUIRED;
  }
  else
  {
    logical_channel_mode = UE_MODE_UNACKNOWLEDGED;
    rb_mode = DCCH_UM_RADIO_BEARER_ID;
    l2_ack_status = TDSRRCSEND_L2ACK_NOT_REQUIRED;
  }

  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH, rb_mode,
    logical_channel_mode);


  if ( ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RLC Logical channel not found");
    return;
  }

  /* Allocate a buffer for an uplink DCCH Message. This buffer
  * should be released by SEND_CHAIN using OSS compiler functions */

  msg_ptr = (tdsrrc_UL_DCCH_Message *) tdsrrc_malloc (sizeof(struct tdsrrc_UL_DCCH_Message));

  if ( msg_ptr == NULL )
  {
    ERR_FATAL("Failed to allocate memory ",0,0,0);
  }

  /* Store meas report chosen */
  msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_measurementReport;


  /* First store the meas id */
  msg_ptr->message.u.measurementReport.measurementIdentity =
   (tdsrrc_MeasurementIdentity) l1_meas_report_ptr->meas_id;

  /* Initialize Meas Report bit mask */
  TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport);

   /* Malloc memory for MeasResult_v4b0  */
  tdsrrcmeas_set_measurement_result_v4b0ext(msg_ptr);

  /* Append Event Results if present */
  if (l1_meas_report_ptr->rpt_crit == TDSL1_EVENT_TRIGGER_RPT)
  {
    /* Set event results present */
    TDSRRC_MSG_COMMON_SET_BITMASK_IE(msg_ptr->message.u.measurementReport,
    tdsrrc_MeasurementReport,eventResults);

    /* Set t to Inter RAT Events Results chosen */
    TDSRRC_SET_COMMON_MSG_TYPE2(msg_ptr->message.u.measurementReport.eventResults,
    rrc_EventResults,interRATEventResults);

    /* Allocate Memory for the cell */
    msg_ptr->message.u.measurementReport.eventResults.u.interRATEventResults =
         rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_InterRATEventResults);
		 
    if (msg_ptr->message.u.measurementReport.eventResults.u.interRATEventResults == NULL)
    {
      ERR_FATAL("Unable to allocate memory for RATEventResults",0,0,0);
    }
		 
    /* Set Event ID */
    tdsrrcirat_set_inter_sys_event_id(&l1_meas_report_ptr->u.inter_sys_rpt.event_results.event_id,
      &msg_ptr->message.u.measurementReport.eventResults.u.
      interRATEventResults->eventID);

    local_cell_list_ptr =  &msg_ptr->message.u.measurementReport.eventResults.u.
        interRATEventResults->cellToReportList;

    local_cell_list_ptr->n = 
        l1_meas_report_ptr->u.inter_sys_rpt.event_results.num_cells;
    ALLOC_ASN1ARRAY1(&tdsenc_ctxt,local_cell_list_ptr,
        tdsrrc_CellToReport);

    if ( local_cell_list_ptr->elem == NULL )
    {
      ERR_FATAL("Failed to allocate memory",0,0,0);
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"l1_meas_report_ptr->u.inter_sys_rpt.event_results.num_cells=%d",l1_meas_report_ptr->u.inter_sys_rpt.event_results.num_cells);
    
    /* Set Cell on which Event occurred */
    for (event_cell_count = 0;
    event_cell_count < l1_meas_report_ptr->u.inter_sys_rpt.event_results.num_cells;
    event_cell_count++)
    {
      /* If BSIC Verified */
#ifdef FEATURE_ENH_BSIC_VERIFY
      if (tdsrrcirat_meas_id_is_bsic_verified(l1_meas_report_ptr->meas_id))
      {
        uint16 arfcn = CONVERT_ARFCN_TO_NUM(l1_meas_report_ptr->u.inter_sys_rpt.event_results.cell[event_cell_count].u.bsic_nonverified.bcch_arfcn);
        uint16 cell_id = TDSMAX_INTER_RAT_CELL_ID;
        boolean cell_id_allocated = FALSE;

        if (tdsrrcirat_get_cell_id_from_rat_cell_list(arfcn, &cell_id))
        {
            int i = 0;
            boolean cell_id_used = FALSE;
            uint16 used_cid = TDSMAX_INTER_RAT_CELL_ID;

            cell_id_allocated = TRUE;
            
            /* Check if this cell_id has been used */
            if (event_cell_count > 0)
            {
               /* Check for last used from back of the array */
               for (i = event_cell_count - 1; i >= 0; i--)
               {
                  if (local_cell_list_ptr->elem[i].bsicReported.u.verifiedBSIC == cell_id)
                  {
                     cell_id_used = TRUE;
                     break;
                  }
               }
            }

            if (cell_id_used)
            {
               used_cid = cell_id;
               if (!tdsrrcirat_get_next_cell_id_from_rat_cell_list(arfcn, used_cid, &cell_id))
               {
                  cell_id_allocated = FALSE;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"FEATURE_ENH_BSIC_VERIFY unable to allocate event cell_id %d",used_cid);
               }
            }
         }

         if (cell_id_allocated)
         {
            local_cell_list_ptr->elem[event_cell_count].bsicReported.t = T_tdsrrc_BSICReported_verifiedBSIC;


            TDSRRC_MSG1(MSG_LEGACY_HIGH,"FEATURE_ENH_BSIC_VERIFY Verified BSIC is %d",cell_id);

            /* Set cell id to Verified BSIC */
            local_cell_list_ptr->elem[event_cell_count].bsicReported.u.verifiedBSIC = cell_id;
         }
         else
         { // Treat as nonVerified
           TDSRRC_MSG1(MSG_LEGACY_ERROR,"FEATURE_ENH_BSIC_VERIFY can't locate cell_id for arfcn %d",arfcn);

           local_cell_list_ptr->elem[event_cell_count].bsicReported.t = T_tdsrrc_BSICReported_nonVerifiedBSIC;

           /* Set cell id to Non Verified BSIC */
           local_cell_list_ptr->elem[event_cell_count].bsicReported.u.nonVerifiedBSIC =
             CONVERT_ARFCN_TO_NUM(l1_meas_report_ptr->u.inter_sys_rpt.event_results.cell[event_cell_count].u.
             bsic_nonverified.bcch_arfcn);


           TDSRRC_MSG3(MSG_LEGACY_HIGH,"FEATURE_ENH_BSIC_VERIFY Non Verified BSIC is %d",
             CONVERT_ARFCN_TO_NUM(l1_meas_report_ptr->u.inter_sys_rpt.event_results.cell[event_cell_count].u.
             bsic_nonverified.bcch_arfcn),0,0);
         }
      }
#else
      if (l1_meas_report_ptr->u.inter_sys_rpt.event_results.cell[event_cell_count].bsic_choice == TDSL1_BSIC_VERIFIED)
      {
        local_cell_list_ptr->elem[event_cell_count].bsicReported.t = T_tdsrrc_BSICReported_verifiedBSIC;

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Verified BSIC is %d",
          l1_meas_report_ptr->u.inter_sys_rpt.event_results.cell[event_cell_count].u.
          bsic_verified.intersys_cell_id,0,0);

        /* Set cell id to Verified BSIC */
        local_cell_list_ptr->elem[event_cell_count].bsicReported.u.verifiedBSIC =
         (uint8) l1_meas_report_ptr->u.inter_sys_rpt.event_results.cell[event_cell_count].u.
          bsic_verified.intersys_cell_id;
      }
#endif
      else
      {
        local_cell_list_ptr->elem[event_cell_count].bsicReported.t = T_tdsrrc_BSICReported_nonVerifiedBSIC;

        /* Set cell id to Non Verified BSIC */
        local_cell_list_ptr->elem[event_cell_count].bsicReported.u.nonVerifiedBSIC =
          CONVERT_ARFCN_TO_NUM(l1_meas_report_ptr->u.inter_sys_rpt.event_results.cell[event_cell_count].u.
          bsic_nonverified.bcch_arfcn);


        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Non Verified BSIC is %d",
          CONVERT_ARFCN_TO_NUM(l1_meas_report_ptr->u.inter_sys_rpt.event_results.cell[event_cell_count].u.
          bsic_nonverified.bcch_arfcn),0,0);
        
      }
      

    } /* End of for */
  } /* End of if of Event Triggered Events */

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"l1_meas_report_ptr->.gsm.measured_gsm_cells=%d",l1_meas_report_ptr->u.inter_sys_rpt.measurements.inter_sys_msmt_results[0].u.gsm.measured_gsm_cells);

  /* Now add Measured Results */
  if ((measured_res_incl == FALSE) ||
    (l1_meas_report_ptr->u.inter_sys_rpt.measurements.
    inter_sys_msmt_results[0].u.gsm.measured_gsm_cells == 0))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MeasResult: measured_res_incl = %d",measured_res_incl);
  }
  else
  {
    
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
         v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.n = 1;
    
    /* Set Measured Results t */
    TDSRRC_SET_COMMON_MSG_TYPE2(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.
       laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext.
       additionalMeasuredResults_LCR.elem[0], 
       rrc_MeasuredResults_LCR_r4,
       interRATMeasuredResultsList);

    /* Allocate Memory */
    local_rrc_measured_results_ptr =
      rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_InterRATMeasuredResultsList);

    if (local_rrc_measured_results_ptr == NULL)
    {
      ERR_FATAL("Unable to allocate memory",0,0,0);
    }

    /* Store the head ptr */
    msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
      v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.elem[0].
      u.interRATMeasuredResultsList = local_rrc_measured_results_ptr;

    cell_number = 0;

    local_rrc_measured_results_ptr->n =1;

    ALLOC_ASN1ARRAY1(&tdsenc_ctxt,local_rrc_measured_results_ptr,tdsrrc_InterRATMeasuredResults);
    if ( local_rrc_measured_results_ptr->elem == NULL )
    {
      ERR_FATAL("Failed to allocate memory",0,0,0);
    }

    /* Set System to GSM */
    local_rrc_measured_results_ptr->elem[0].t =
      T_tdsrrc_InterRATMeasuredResults_gsm;

    local_rrc_gsm_list_ptr = rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_GSM_MeasuredResultsList);
    

    if (local_rrc_gsm_list_ptr == NULL)
    {
      ERR_FATAL("Unable to allocate memory",0,0,0);
    }
    /* Assign Head Ptr */
    local_rrc_measured_results_ptr->elem[0].u.gsm = local_rrc_gsm_list_ptr;
   
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"No of GSM cells are %d", l1_meas_report_ptr->u.inter_sys_rpt.measurements.
      inter_sys_msmt_results[cell_number].u.gsm.measured_gsm_cells,0,0);
    local_rrc_gsm_list_ptr->n =
        l1_meas_report_ptr->u.inter_sys_rpt.measurements.
      inter_sys_msmt_results[cell_number].u.gsm.measured_gsm_cells;

    ALLOC_ASN1ARRAY1(&tdsenc_ctxt,local_rrc_gsm_list_ptr,tdsrrc_GSM_MeasuredResults);
    if ( local_rrc_gsm_list_ptr->elem == NULL )
    {
      ERR_FATAL("Failed to allocate memory",0,0,0);
    }
    
    /* For First RAT, read the GSM cell */
    for (gsm_cell_count = 0;
    gsm_cell_count < l1_meas_report_ptr->u.inter_sys_rpt.measurements.
      inter_sys_msmt_results[cell_number].u.gsm.measured_gsm_cells;
    gsm_cell_count++)
    {

      TDSRRC_RESET_MSG_IE_PRESENT(local_rrc_gsm_list_ptr->elem[gsm_cell_count]);

      /* If BSIC Verified */
#ifdef FEATURE_ENH_BSIC_VERIFY
      if (tdsrrcirat_meas_id_is_bsic_verified(l1_meas_report_ptr->meas_id))
      {
        uint16 arfcn = CONVERT_ARFCN_TO_NUM(l1_meas_report_ptr->u.inter_sys_rpt.measurements.inter_sys_msmt_results[cell_number].
          u.gsm.measured_cells[gsm_cell_count].u.bsic_not_verified.bcch_arfcn);
        uint16 cell_id = TDSMAX_INTER_RAT_CELL_ID;
        boolean cell_id_allocated = FALSE;

        if (tdsrrcirat_get_cell_id_from_rat_cell_list(arfcn, &cell_id))
        {
           int i = 0;
           boolean cell_id_used = FALSE;
           uint16 used_cid = TDSMAX_INTER_RAT_CELL_ID;
  
           cell_id_allocated = TRUE;

           /* Check if this cell_id has been used */
           if (gsm_cell_count > 0)
           {
              /* Check for last used from back of the array */
              for (i = gsm_cell_count - 1; i >= 0; i--)
              {
                 if (local_rrc_gsm_list_ptr->elem[i].bsicReported.u.verifiedBSIC == cell_id)
                 {
                    cell_id_used = TRUE;
                    break;
                 }
              }
           }
  
           if (cell_id_used)
           {
              used_cid = cell_id;
              if (!tdsrrcirat_get_next_cell_id_from_rat_cell_list(arfcn, used_cid, &cell_id))
              {
                 cell_id_allocated = FALSE;
                 TDSRRC_MSG1(MSG_LEGACY_HIGH,"FEATURE_ENH_BSIC_VERIFY unable to allocate gsm cell_id %d",used_cid);
              }
           }
         }

         if (cell_id_allocated)
         {
           local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.t = T_tdsrrc_BSICReported_verifiedBSIC;

           TDSRRC_MSG1(MSG_LEGACY_HIGH,"FEATURE_ENH_BSIC_VERIFY Verified cell index is %d",cell_id);

           /* Set cell id to Verified BSIC */
           local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.u.verifiedBSIC = cell_id;
         }
         else
         { // Treat as nonVerified
           TDSRRC_MSG1(MSG_LEGACY_ERROR,"FEATURE_ENH_BSIC_VERIFY can't locate cell_id for arfcn %d",arfcn);
            
           local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.t = T_tdsrrc_BSICReported_nonVerifiedBSIC;

           /* Set cell id to Non Verified BSIC */
           local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.u.nonVerifiedBSIC =
             CONVERT_ARFCN_TO_NUM(l1_meas_report_ptr->u.inter_sys_rpt.measurements.inter_sys_msmt_results[cell_number].
             u.gsm.measured_cells[gsm_cell_count].u.bsic_not_verified.bcch_arfcn);

           TDSRRC_MSG1(MSG_LEGACY_HIGH,"FEATURE_ENH_BSIC_VERIFY Non Verified BSIC is %d",local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.u.nonVerifiedBSIC);
         }
      }
#else
      if (l1_meas_report_ptr->u.inter_sys_rpt.measurements.inter_sys_msmt_results[cell_number].
        u.gsm.measured_cells[gsm_cell_count].bsic_choice == TDSL1_BSIC_VERIFIED)
      {
        local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.t = T_tdsrrc_BSICReported_verifiedBSIC;

        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Verified cell index is %d", l1_meas_report_ptr->u.inter_sys_rpt.measurements.inter_sys_msmt_results[cell_number].
          u.gsm.measured_cells[gsm_cell_count].u.bsic_verified.inter_sys_cell_id,0,0);

        /* Set cell id to Verified BSIC */
        local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.u.verifiedBSIC =
          (uint8)l1_meas_report_ptr->u.inter_sys_rpt.measurements.inter_sys_msmt_results[cell_number].
          u.gsm.measured_cells[gsm_cell_count].u.bsic_verified.inter_sys_cell_id;
      }
#endif
      else
      {
        local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.t = T_tdsrrc_BSICReported_nonVerifiedBSIC;

        /* Set cell id to Non Verified BSIC */
        local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.u.nonVerifiedBSIC =
          CONVERT_ARFCN_TO_NUM(l1_meas_report_ptr->u.inter_sys_rpt.measurements.inter_sys_msmt_results[cell_number].
          u.gsm.measured_cells[gsm_cell_count].u.bsic_not_verified.bcch_arfcn);

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Non Verified BSIC is %d",local_rrc_gsm_list_ptr->elem[gsm_cell_count].bsicReported.u.nonVerifiedBSIC);

      }

      /* Set RSSI value */
      if (l1_meas_report_ptr->u.inter_sys_rpt.measurements.inter_sys_msmt_results[cell_number].
        u.gsm.measured_cells[gsm_cell_count].gsm_carrier_rssi_incl == TRUE)
      {
        /* Convert value from uint8 to oss bit string */
        tdsrrcirat_convert_rssi_value(
          &l1_meas_report_ptr->u.inter_sys_rpt.measurements.
          inter_sys_msmt_results[cell_number].u.gsm.measured_cells[gsm_cell_count].
          gsm_carrier_rssi, &rssi_value);

        /* Double check if value is O.K */
        if (rssi_value <= TDSMAX_RSSI)
        {
         

          /* As per specs, only right most 6 bits should be sent. Lint e734 */
          rssi_value = rssi_value << 2;     /*lint !e734 */

          /* Put Actual Value to be 6 bits */
          local_rrc_gsm_list_ptr->elem[gsm_cell_count].gsm_CarrierRSSI.numbits= TDSRSSI_ACTUAL_LENGTH;

          *(local_rrc_gsm_list_ptr->elem[gsm_cell_count].gsm_CarrierRSSI.data) = (unsigned char ) (rssi_value);


          /* Set RSSI value present in Meas Report */
          local_rrc_gsm_list_ptr->elem[gsm_cell_count].m.gsm_CarrierRSSIPresent = 1;

        }  /* End of rssi value less than TDSMAX_RSSI */

      } /* End of RSSI included */
      else
      {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"RSSI not included for cell %d",gsm_cell_count);
      }
      
      if (gsm_cell_count == (l1_meas_report_ptr->u.inter_sys_rpt.measurements.
        inter_sys_msmt_results[cell_number].u.gsm.measured_gsm_cells-1))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Thus no of GSM cells set are %d",l1_meas_report_ptr->u.inter_sys_rpt.measurements.
          inter_sys_msmt_results[cell_number].u.gsm.measured_gsm_cells,0,0);

        /* Reached the end of the gsm cell list */

        break; /* Come out of first for loop */

      }
     

    } /* End of for loop of gsm_cell_count */

    /* Since only 1 GSM RAT, initialize the next RAT to 0 */
    


    /* Now check if this is the last report. Meas Id needs to be deleted if true */
    if (l1_meas_report_ptr->last_periodic_report == TRUE)
    {
      /* Delete this Measurement */
      for (meas_id_cnt =0; meas_id_cnt < TDSMAX_INTER_RAT_MEAS; meas_id_cnt++ )
      {
        if (tdsinter_rat_meas_id_list[meas_id_cnt].meas_id == l1_meas_report_ptr->meas_id)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete Meas %d",l1_meas_report_ptr->meas_id);
          tdsinter_rat_meas_id_list[meas_id_cnt].meas_id = TDSINVALID_MEAS_ID;

          tdsinter_rat_meas_id_list[meas_id_cnt].rpt_crit = TDSNOT_AVAILABLE;

          tdsinter_rat_meas_id_list[meas_id_cnt].rpt_bsic_verified = FALSE;
          
          /* Also initialize additional meas */
          for ( cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++ )
            tdsinter_rat_meas_id_list[meas_id_cnt].additional_meas[cnt] = TDSINVALID_MEAS_ID;

          break;
        }
      }
    }

    /* Now delete this meas from addtl meas DB */
    if (l1_meas_report_ptr->last_periodic_report == TRUE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete %d from addtl meas DB",l1_meas_report_ptr->meas_id);
      tdsrrcmeas_delete_addtl_meas(&l1_meas_report_ptr->meas_id);
    }

  } /* End of else of measured_res_incl */

  /* It doesn't seem necessary for TDS to have the following entire block of code */
  if(((msg_ptr->message.u.measurementReport.m.v390nonCriticalExtensionsPresent) &&
      (msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.m.laterNonCriticalExtensionsPresent) &&
      (msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.m.
            v4b0NonCriticalExtensionsPresent) &&
      (msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
       v4b0NonCriticalExtensions.measurementReport_v4b0ext.m.additionalMeasuredResults_LCRPresent) &&
      (msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.
          v4b0NonCriticalExtensions.measurementReport_v4b0ext.additionalMeasuredResults_LCR.elem[0].t == 
          T_tdsrrc_MeasuredResults_LCR_r4_interRATMeasuredResultsList))
         || ((msg_ptr->message.u.measurementReport.m.eventResultsPresent) &&
             (msg_ptr->message.u.measurementReport.eventResults.t == T_tdsrrc_EventResults_interRATEventResults)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"measured result list present");
    if(tdsinterRAT_cell_info_status.interRAT_cell_info_present==TRUE)
    {
      //msg_ptr->message.u.measurementReport.m.v390nonCriticalExtensionsPresent =1;

      //TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions);
      //msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.m.laterNonCriticalExtensionsPresent =1;
      //TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.measurementReport_v390ext);

      //TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions);
      //msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent=1 ;

      //TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions);
      msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent =1 ;

      //TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext);


      //TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions);
      msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v5b0NonCriticalExtensionsPresent=1 ;

      //TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext);

      //TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions);

      //TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
      msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.
        measurementReport_v5b0ext.m.interRATCellInfoIndicationPresent =1 ;

      msg_ptr->message.u.measurementReport.v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.interRATCellInfoIndication =
          tdsinterRAT_cell_info_status.interRAT_cell_info_ind ;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"irat cell info ind added in meas report %d",tdsinterRAT_cell_info_status.interRAT_cell_info_ind);
    }
  }



  /* Append Additional Measured Results if available */
  tdsrrcmeas_process_additional_measured_results(
    &l1_meas_report_ptr->meas_id, TDSINTER_SYS, &msg_ptr->message.u.measurementReport);

  
  /* Closed measurement_result_v4b0ext if malloc memory no use*/
  tdsrrcmeas_close_measurement_result_v4b0ext(msg_ptr);

  /* This places SDU into RLC watermark queue */
  status =  tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_MCMR, (void *) msg_ptr,
    ul_lc_id, l2_ack_status);

  if ( status != TDSRRCSEND_SUCCESS )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Send chain failed to send Inter-RAT Meas Rpt");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Meas Rpt sent from RRC=>RLC");
    if(l1_meas_report_ptr->meas_transfer_mode == TDSL1_ACKNOWLEDGED_MODE)
    {
      tdsrrcmeas_pending_rpt++;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"pending meas rpt = %d",tdsrrcmeas_pending_rpt);
    }
  }

  return;

}

/*===========================================================================

FUNCTION    RRCMEAS_SET_INTRA_FREQ_EVENT_ID

DESCRIPTION

  Sets the event id in measurement report from L1 format to ASN1 format

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE if event no is incorrect
  SUCCESS if event no is correct

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_set_inter_sys_event_id
(
  tdsl1_inter_sys_event_enum_type* l1_event_type_ptr,
  tdsrrc_EventIDInterRAT* rrc_event_id_ptr
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Event Triggered Report");

  /* Find type of event and set it */
  switch ( *l1_event_type_ptr )
  {
    case TDSL1_INTER_RAT_EVENT_3A:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSEVENT 3A reported");
      *rrc_event_id_ptr = tdsrrc_EventIDInterRAT_e3a;
      break;

    case TDSL1_INTER_RAT_EVENT_3B:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSEVENT 3B reported");
      *rrc_event_id_ptr = tdsrrc_EventIDInterRAT_e3b;

      break;

    case TDSL1_INTER_RAT_EVENT_3C:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSEVENT 3C reported");
      *rrc_event_id_ptr = tdsrrc_EventIDInterRAT_e3c;

      break;

    case TDSL1_INTER_RAT_EVENT_3D:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSEVENT 3D reported");
      *rrc_event_id_ptr = tdsrrc_EventIDInterRAT_e3d;

      break;

  default:
    /* Not Possible */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"L1 sent a wrong event ID.");

    break;
  }  /* End of switch */

  return;
}


/*===========================================================================

FUNCTION   RRCIRAT_VALIDATE_EVENT_RESULTS

DESCRIPTION

  This function validates the event results sent by L1

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If event resuulst are valid
  SUCCESS: If event results are invalid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_validate_event_results
(
  tdsl1_inter_sys_event_results* l1_event_results_ptr
)
{
  uint8 cell_number = 0;

  /* Validate the results */
  if (l1_event_results_ptr->num_cells == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"L1 gave 0 cells for Inter-RAT Event Report");
    return FAILURE;
  }
  if (l1_event_results_ptr->num_cells > TDSL1_MAX_CELL_MEAS)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Set Inter-RAT cells to %d,L1 sent %d",TDSL1_MAX_CELL_MEAS,l1_event_results_ptr->num_cells);
    l1_event_results_ptr->num_cells = TDSL1_MAX_CELL_MEAS;
  }

  for (cell_number = 0; cell_number <l1_event_results_ptr->num_cells; cell_number++)
  {
    if (l1_event_results_ptr->
      cell[cell_number].bsic_choice == TDSL1_BSIC_VERIFIED)
    {
      if (l1_event_results_ptr->
        cell[cell_number].u.bsic_verified.intersys_cell_id > TDSMAX_INTER_RAT_CELL_ID)
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid Inter RAT cell id %d",
          l1_event_results_ptr->cell[cell_number].u.
          bsic_verified.intersys_cell_id,0,0);

        return FAILURE;
      }
    }
    else
    {

    /* Set bcch ARFCN i.e. Absolute Radio Freq Channel No. GSM carrier frequency is
      designated by ARFCN */
      if (CONVERT_ARFCN_TO_NUM(l1_event_results_ptr->cell[cell_number].u.
        bsic_nonverified.bcch_arfcn) > TDSMAX_ARFCN)
      {
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid ARFCN %d",
          CONVERT_ARFCN_TO_NUM(l1_event_results_ptr->cell[cell_number].u.
          bsic_nonverified.bcch_arfcn),0,0);

        return FAILURE;
      }
    }

  } /* End of for */
  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCIRAT_CHECK_CELL_ABSENCE_IN_CELL_LIST

DESCRIPTION

  This function checks if the cells are absent in CELL_INFO_LIST

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If cell is absent in CELL_INFO_LIST
  SUCCESS: If cell is present in CELL_INFO_LIST

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_check_cell_absence_in_cell_list
(
  void
)
{
  uint8 local_neighbor_cnt = 0;

  for (local_neighbor_cnt=0; local_neighbor_cnt < TDSL1_MAX_CELL_MEAS; local_neighbor_cnt++)
  {
    if (tdsinter_rat_cell_list[local_neighbor_cnt].cell_id != TDSINVALID_CELL_ID)
    {
      return FAILURE;
    }
  }

  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCIRAT_VALIDATE_MEASURED_RESULTS

DESCRIPTION

  This function validates the measured results sent by L1

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If measured results are valid
  SUCCESS: If measured results are invalid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_validate_measured_results
(
  tdsl1_inter_sys_meas_result_struct_type* meas_results_ptr
)
{
  uint8 cell_number=0, gsm_cell_count=0;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Validate Inter-RAT Measured Results");
  if ((meas_results_ptr->num_msmt_results == 0) || ((meas_results_ptr->num_msmt_results > 1)))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RAT Meas Results (Valid is 1) = %d",meas_results_ptr->num_msmt_results);
    return FAILURE;
  }


  if (meas_results_ptr->inter_sys_msmt_results[cell_number].u.gsm.measured_gsm_cells >
    TDSL1_MAX_REPORTED_GSM_CELLS)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Setting no of GSM cells to %d,L1 sent %d",TDSL1_MAX_REPORTED_GSM_CELLS,meas_results_ptr->inter_sys_msmt_results[cell_number].u.gsm.measured_gsm_cells);
    meas_results_ptr->inter_sys_msmt_results[cell_number].u.gsm.measured_gsm_cells =
      TDSL1_MAX_REPORTED_GSM_CELLS;
  }

  /* Delete duplicate non verified cells if any */
  tdsrrcirat_delete_dup_non_verified_cells(meas_results_ptr);


  for (cell_number = 0;
  cell_number < meas_results_ptr->num_msmt_results;
  cell_number++)
  {
    if (meas_results_ptr->inter_sys_msmt_results[cell_number].system != TDSL1_GSM)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Inter-RAT system = %d",meas_results_ptr->inter_sys_msmt_results[cell_number].system);

      return FAILURE;
    }
    /* For First RAT, read the GSM cell */
    for (gsm_cell_count = 0;
    gsm_cell_count < meas_results_ptr->inter_sys_msmt_results[cell_number].u.gsm.measured_gsm_cells;
    gsm_cell_count++)
    {
      /* Validate Cell Id */
      if (meas_results_ptr->inter_sys_msmt_results[cell_number].
        u.gsm.measured_cells[gsm_cell_count].bsic_choice == TDSL1_BSIC_VERIFIED)
      {
        if (meas_results_ptr->inter_sys_msmt_results[cell_number].
          u.gsm.measured_cells[gsm_cell_count].u.bsic_verified.inter_sys_cell_id > TDSMAX_INTER_RAT_CELL_ID)
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid Cell Id[0..31] = %d",
            meas_results_ptr->inter_sys_msmt_results[cell_number].
            u.gsm.measured_cells[gsm_cell_count].u.bsic_verified.inter_sys_cell_id,0,0);
          return FAILURE;
        }
      }
      else
      {
        /* Validate ARFCN */
        if (CONVERT_ARFCN_TO_NUM(meas_results_ptr->inter_sys_msmt_results[cell_number].
          u.gsm.measured_cells[gsm_cell_count].u.bsic_not_verified.bcch_arfcn) >= TDSMAX_ARFCN)
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid ARFCN[0..1024] = %d",
            CONVERT_ARFCN_TO_NUM(meas_results_ptr->inter_sys_msmt_results[cell_number].
            u.gsm.measured_cells[gsm_cell_count].u.bsic_not_verified.bcch_arfcn),0,0);
          return FAILURE;
        }
      }

    } /* End of for loop of gsm_cell_count */

  } /* End of for of system results */

  return SUCCESS;
}


/*===========================================================================

FUNCTION   RRCIRAT_CONVERT_RSSI_VALUE

DESCRIPTION

  This function converst the RSSI value sent by L1 into a format expected by UTRAN.
    Maximum converted value is 63 and minimum converted value is 0

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_convert_rssi_value
(
  int16* gsm_carrier_rssi_ptr,
  uint8* rssi_converted_value_ptr
)
{
/* From 45.008
The measured signal level shall be mapped to an RXLEV value between 0 and 63, as follows:
RXLEV 0 = less than -110 dBm + SCALE.
RXLEV 1 = -110 dBm + SCALE to -109 dBm + SCALE.
RXLEV 2 = -109 dBm + SCALE to -108 dBm + SCALE.
:
:
RXLEV 62 = -49 dBm + SCALE to -48 dBm + SCALE.
RXLEV 63 = greater than -48 dBm + SCALE.
where SCALE is an offset that is used in the ENHANCED MEASUREMENT REPORT and PACKET ENHANCED
MEASUREMENT REPORT messages, otherwise it is set to 0.
*/

/* Keep the original implementation as reference */
#if 0
  /* -48 .. -110 */
  int16 rssi_val_range = TDSRSSI_VAL_LOW; /* -110 */

  uint8 utran_rssi_value = 0;

  /* To Satify Lint */
  boolean value = TRUE;

  while (value)
  {
    if ((*gsm_carrier_rssi_ptr >= rssi_val_range) &&
      (*gsm_carrier_rssi_ptr < (rssi_val_range + 1)))
    {
      /* > 110, <= 109,  rssi = 1 .... */
      *rssi_converted_value_ptr = ++utran_rssi_value;
      break;
    }

    /* Increment rssi value */
    utran_rssi_value++;

    /* Increase range by 1 */
    rssi_val_range++;


    if (rssi_val_range == (TDSRSSI_VAL_HIGH+1)) /* -47 */
    {
      /* Now find if the value is beyond TDSRSSI_VAL_LOW */
      if (*gsm_carrier_rssi_ptr < TDSRSSI_VAL_LOW)
      {
        *rssi_converted_value_ptr = TDSMIN_RSSI;
      }
      else
      {
        *rssi_converted_value_ptr = TDSMAX_RSSI;
      }
      break;
    }

  } /* End of while */
#endif

  if (*gsm_carrier_rssi_ptr <= TDSRSSI_VAL_LOW)
    *rssi_converted_value_ptr = TDSMIN_RSSI;
  else if (*gsm_carrier_rssi_ptr > TDSRSSI_VAL_HIGH)
    *rssi_converted_value_ptr = TDSMAX_RSSI;
  else
    *rssi_converted_value_ptr = *gsm_carrier_rssi_ptr - TDSRSSI_VAL_LOW;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"IRAT RSSI Recvd =%d,Converted : %d",*gsm_carrier_rssi_ptr,*rssi_converted_value_ptr);
  return;
}


/*===========================================================================

FUNCTION  RRCIRAT_FILL_HCS_INFO

DESCRIPTION

 This function fills the overall HCS info for an Inter RAT Cell.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill overall HCS info correctly
 SUCCESS: If overall HCS info is filled correctly

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_fill_hcs_info
(
  tdsrrc_InterRATMeasurementSysInfo* sib_inter_rat_ptr,
  tdsl1_meas_sib_parm_struct_type* l1_sib_ptr
  ,tdsrrc_SysInfoType11_v590ext_IEs_newInterRATCellInfoList_v590ext * rel5_ext_ptr
)
{
  tdsrrc_SysInfoType3  *sib3_ptr = NULL;

  tdsrrc_SysInfoType4  *sib4_ptr = NULL;

  tdsrrc_NewInterRATCellList* inter_rat_cell_list_ptr = NULL;

  uint8 cell_count = 0;

  boolean sib4_present = FALSE;

  uint16 cell_position;
  uint32 idx=0,idy=0;

  uint16 arfcn;

  tdsrrc_SysInfoType18 *sib18_ptr = NULL; /* Declared to store the value of SIB 18*/
  tdsrrc_PLMNsOfInterRATCellsList *tmp_rrc_PLMNsOfInterRATCellsList_ptr = NULL;
  rrc_plmn_identity_type tmp_plmn_identity;
  rrc_plmn_identity_type selected_plmn_identity;
  tdsrrc_int_cell_id_type  tmp_cell_id;
  boolean sib18_present = FALSE;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
uint16 prev_cell_count=0;
#endif
  sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB3);
  if( sib3_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB3 ptr is NULL");
    return( FAILURE );
  }

  if (((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH) || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)) &&
    ((sib3_ptr->sib4indicator == TRUE)))
  {
    sib4_ptr = (tdsrrc_SysInfoType4 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB4);

    if (sib4_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB4 ptr NULL.Use SIB3");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Use default values from SIB4");
      sib4_present = TRUE;
    }
  }
  tmp_plmn_identity.mcc[0] =  tmp_plmn_identity.mcc[1] = tmp_plmn_identity.mcc[2] = 0;
  tmp_plmn_identity.num_mnc_digits = 0;
  tmp_plmn_identity.mnc[0] =  tmp_plmn_identity.mnc[1] = 0;
   /* SIB18 */
  if((sib18_ptr = (tdsrrc_SysInfoType18*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB18)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    if(((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH)))
    {
      /*
       1>if System Information Block type 11bis is scheduled on BCH:
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities for SIB 11bis"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Idle mode PLMN identities for SIB11bis";
       2>otherwise,
                 the UE shall use, in place of any missing list, when present, the corresponding one in the IE "Connected mode PLMN identities"; 
       2>otherwise, 
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
       
       1> else
       2>the PLMN lists to be considered are the ones included, when present, in the IE "Connected mode PLMN identities"; 
       2>otherwise,
                 the UE shall use, in place of any missing list, the corresponding one in the IE "Idle mode PLMN identities";
      */
      if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,connectedModePLMNIdentitiesSIB11bis)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis,
            rrc_plmnsOfInterRATCellsList)) &&
          (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.connectedModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            connectedModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Conn Inter RAT from bis extension ");
        sib18_present = TRUE;
      }
      else if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,idleModePLMNIdentitiesSIB11bis)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
            rrc_plmnsOfInterRATCellsList)) &&
          (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 idle Inter RAT from bis extension ");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,connectedModePLMNIdentities)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterRATCellsList)) &&
        (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList.n !=0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &(sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Conn Inter RAT");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterRATCellsList)) &&
        (sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter RAT");
        sib18_present = TRUE;
      }
    }
    else
    {
      /*
       If SIB11bis is present and SIB18 bis extenstions are present, use SIB18 bis extension
       If SIB11bis is present and SIb18 bis extensions are absent, use SIB18
       If SIB11bis is absent, use SIB18
      */
      if((tdsrrcsibdb_is_sib11bis_present_in_srv_cell()) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,v6b0NonCriticalExtensions)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext,idleModePLMNIdentitiesSIB11bis)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis,
          rrc_plmnsOfInterRATCellsList)) &&
        (sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->v6b0NonCriticalExtensions.sysInfoType18_v6b0ext.
            idleModePLMNIdentitiesSIB11bis.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis:SIB11 processing SIB18 Idle Inter RAT from bis extension ");
        sib18_present = TRUE;
      }
      else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities))
        &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,
        plmnsOfInterRATCellsList))
        &&
        (sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
      {
        tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11 processing SIB18 Idle Inter RAT ");
        sib18_present = TRUE;
      }
    }
#else
    if(((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_DCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)
      || (tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,connectedModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->connectedModePLMNIdentities,plmnsOfInterRATCellsList))
      &&
      (sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
    {
       tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->connectedModePLMNIdentities.plmnsOfInterRATCellsList;
       sib18_present = TRUE;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib18_ptr, tdsrrc_SysInfoType18,idleModePLMNIdentities))
      &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib18_ptr->idleModePLMNIdentities,plmnsOfInterRATCellsList))
      &&
      (sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList.n != 0))
    {
      tmp_rrc_PLMNsOfInterRATCellsList_ptr = &sib18_ptr->idleModePLMNIdentities.plmnsOfInterRATCellsList;
      sib18_present = TRUE;
    }
#endif
  }
  if(tdsrrcccm_get_serv_plmn_cellid(&tmp_cell_id, &selected_plmn_identity) == TDSRRC_CELL_LOSS_NO_NEW_CELL)
  {
    sib18_present = FALSE;
  }
  else
  {
    memscpy(&tmp_plmn_identity, sizeof(rrc_plmn_identity_type), &selected_plmn_identity, sizeof(rrc_plmn_identity_type));
  }
  if(tdsrrccsp_is_ue_in_true_limited_service() == TRUE)
  {
    sib18_present = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE in limited service ignoring SIB18");
  }
  {
    tdsrrc_MasterInformationBlock *mib_ptr = NULL;/* Declared to store the value of MIB*/
    if((mib_ptr = (tdsrrc_MasterInformationBlock*)  tdsrrcsibdb_return_sib_for_srv_cell
      (tdsrrc_MIB))!= NULL)
    {
      if(tdsrrc_is_nw_shared(mib_ptr))
      {
        sib18_present = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Ignore SIB18");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: N/W not shader so consider SIB18");
      }
    }
  }



  if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib_inter_rat_ptr,
    tdsrrc_InterRATMeasurementSysInfo,interRATCellInfoList)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List absent in SIB11");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    if(tdsrrcirat_read_sib11bis(l1_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterRATCellsList_ptr)== FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter RAT in SIB11bis Ignored");
      l1_sib_ptr->inter_s.num_cell_add = 0;
      l1_sib_ptr->inter_sys_meas_info_incl = TRUE;
      return FAILURE;
    }
    return SUCCESS;
#else
    return FAILURE;
#endif
  }

  /* Store ptr given by UTRAN */
  inter_rat_cell_list_ptr = &sib_inter_rat_ptr->interRATCellInfoList.newInterRATCellList;

  if ((inter_rat_cell_list_ptr == NULL)||(inter_rat_cell_list_ptr->n == 0))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"inter_rat_cell_list_ptr is NULL");
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    if(tdsrrcirat_read_sib11bis(l1_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterRATCellsList_ptr)== FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter RAT in SIB11bis Ignored");
      l1_sib_ptr->inter_s.num_cell_add = 0;
      l1_sib_ptr->inter_sys_meas_info_incl = TRUE;
      return FAILURE;
    }
    return SUCCESS;
#else
    return FAILURE;
#endif

  }

  while ( (inter_rat_cell_list_ptr->n > idx) &&
    (cell_count < TDSL1_MAX_CELL_MEAS) )
  {
    /* Let first check the type of the cell to find if we
    * really need to give this cell to L1 */
    if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.t !=
      T_tdsrrc_NewInterRATCell_technologySpecificInfo_gsm)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring Non GSM cell");
       arfcn = TDSMAX_ARFCN;
      if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
      {
        /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
        cell_position = (uint16)
          inter_rat_cell_list_ptr->elem[idx].interRATCellID;

      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
        /* Store the cell in first order position */
        cell_position = TDSAVAILABLE_CELL_POSITION;
      }
      /* Tell to get the first available cell position and

      * that would be the cell id. The called function would store
      * the cell id back in passed cell_position variable */
      if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
        return FAILURE;
      }
      idx++;
      continue;
    }
    else
    {
      l1_sib_ptr->inter_s.add_cell[cell_count].cell_info_type = TDSL1_GSM_SYSTEM;

      /* Cell reselection info is always set to TRUE. If not present in SIB 11, just
      set it to default values */
      l1_sib_ptr->inter_s.add_cell[cell_count].u.
        gsm_cell.cell_sel_info_incl = TRUE;

      /* Initializw q-offset1-2 Defaults as per specs */
      l1_sib_ptr->inter_s.add_cell[cell_count].u.
        gsm_cell.cell_sel_info.qoffset1_s_n = TDSQOFFSET1;


      /* -24. Got this value from Radio Access Capability */
      l1_sib_ptr->inter_s.add_cell[cell_count].u.
        gsm_cell.cell_sel_info.max_tx_pwr = -24;

      /* Set default values for the HCS neighboring cell info if it is first cell
       * else use the info from the previous cell */
      if(cell_count == 0)
      {
        l1_sib_ptr->inter_s.add_cell[cell_count].u.
           gsm_cell.hcs_info.hcs_prio = 0;
        l1_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.hcs_info.q_hcs = 110;
        l1_sib_ptr->inter_s.add_cell[cell_count].u.
         gsm_cell.hcs_info.penalty_time = 0;
      }
      else
      {
        l1_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.hcs_info.hcs_prio =
          l1_sib_ptr->inter_s.add_cell[cell_count - 1].u.
          gsm_cell.hcs_info.hcs_prio;
        l1_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.hcs_info.q_hcs =
          l1_sib_ptr->inter_s.add_cell[cell_count - 1].u.
          gsm_cell.hcs_info.q_hcs;
        l1_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.hcs_info.penalty_time =
          l1_sib_ptr->inter_s.add_cell[cell_count - 1].u.
          gsm_cell.hcs_info.penalty_time;
        
        if(l1_sib_ptr->inter_s.add_cell[cell_count - 1].u.
          gsm_cell.hcs_info.penalty_time != 0)
        {
          l1_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.tmp_offset1 =
            l1_sib_ptr->inter_s.add_cell[cell_count - 1].u.
            gsm_cell.hcs_info.tmp_offset1;
        }
      }

      if ((sib4_present == TRUE) && (sib4_ptr != NULL))
      {
      /* Not valid for Inter-RAT but initialize to default
        * as they are present */
        if (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.t !=
            T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"unsupprt FDD mode");
          return(FAILURE);
        }
        
        l1_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info.rxlev_min = (int16)
          (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
      }
      else
      {
        
        if (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.t !=
            T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"unsupprt FDD mode");
          return(FAILURE);
        }
        
        l1_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info.rxlev_min = (int16)
          (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
      }

      /* Get GSM CELL Info */
      /* Check if rrc_gsm_cellSelectionReselectionInfo_present */
      if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->m.
        cellSelectionReselectionInfoPresent)
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Reselection info present");

        /* q-Offset1S-N */
     
          /* Range is -50...50 */
          l1_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.qoffset1_s_n = (int16)
            inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.q_Offset1S_N;
        


        /* Max Txit Power */
        if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->cellSelectionReselectionInfo.m.
          maxAllowedUL_TX_PowerPresent)
        {
          l1_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.max_tx_pwr = (int16)
            inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.maxAllowedUL_TX_Power;
        }

        /* No need to Check if gsm mode present as it has already been done */

        /* Check if rxlev_min present */

        if(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
           cellSelectionReselectionInfo.modeSpecificInfo.t == 
             T_tdsrrc_CellSelectReselectInfoSIB_11_12_modeSpecificInfo_gsm)
        {
          if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.modeSpecificInfo.u
            .gsm->m.q_RxlevMinPresent)
          {
            /* Store rxlev_min in L1 */
            l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min = (int16)
              ((inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.modeSpecificInfo.u.gsm->q_RxlevMin * 2) + 1);
          }
        }
          /*l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
            delta_qrxlevmin_included = FALSE;  */
        if(rel5_ext_ptr != NULL)
        {
          if(TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(rel5_ext_ptr,deltaQrxlevmin))
          {
            /*l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
              delta_qrxlevmin_included = TRUE;
            l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.
              delta_qrxlevmin = rel5_ext_ptr->value.deltaQrxlevmin * 2; */
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Delta-Qrxlev=%d, Qrxlev=%d, ARFCN=%d",
              (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2),
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min,
              CONVERT_ARFCN_TO_NUM(l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn));

              /*if (l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +
                  (rel5_ext_ptr->value.deltaQrxlevmin * 2) < -115)
              {*/
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +=
                (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2);
            /*}*/
          }
        }
        /* HCS inforamtion */
        if(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
          cellSelectionReselectionInfo.m.hcs_NeighbouringCellInformation_RSCPPresent)
        {
          /* HCS PRIO*/
         
            l1_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.hcs_info.hcs_prio = (uint16)
              inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_PRIO;
          
          /*Q HCS */
         
            if(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.q_HCS < 90)
            {
              l1_sib_ptr->inter_s.add_cell[cell_count].u.
                gsm_cell.hcs_info.q_hcs = (uint16) (110 -
                inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.q_HCS);
            }
            else
            {
              l1_sib_ptr->inter_s.add_cell[cell_count].u.
                gsm_cell.hcs_info.q_hcs = 37;
            }
          
          /* Delta Qhcs is not needed if it is not mapped from CPICH-RSCP or PCCPCH-RSCP.
          * For inter rat cells, Qhcs is always mapped from RSSI
          */
          
          /* PENALTY TIME*/
          if(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
            .penaltyTime.t != T_tdsrrc_PenaltyTime_RSCP_notUsed)
          {
            
            switch(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
              .penaltyTime.t)
            {
            case T_tdsrrc_PenaltyTime_RSCP_pt10:
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 10;
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                .penaltyTime.u.pt10);
              break;
              
            case T_tdsrrc_PenaltyTime_RSCP_pt20:
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 20;
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                .penaltyTime.u.pt20);
              break;
              
            case T_tdsrrc_PenaltyTime_RSCP_pt30:
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 30;
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                .penaltyTime.u.pt30);
              break;
              
            case T_tdsrrc_PenaltyTime_RSCP_pt40:
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 40;
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                .penaltyTime.u.pt40);
              break;
              
            case T_tdsrrc_PenaltyTime_RSCP_pt50:
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 50;
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                .penaltyTime.u.pt50);
              break;
              
            case T_tdsrrc_PenaltyTime_RSCP_pt60:
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 60;
              l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                .penaltyTime.u.pt60);
              break;
              
            default:  /* should not happen */
              break;
            }
          }
        }
      } /* rrc_gsm_cellSelectionReselectionInfo_present */

      /* Cell Individual Offset */
      l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.cio = (int16)
        inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->interRATCellIndividualOffset;

      /* Get BAND 1800 DCS or 1900 PCS */
      l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator =
        TDSRET_INTER_RAT_BAND(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->frequency_band);

      /* Get bcch_ARFCN */
      CONVERT_ARFCN_TO_NUM(l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn) =
        (uint16) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->bcch_ARFCN;
      l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.max_tx_pwr = 
        (int16)rr_get_maximum_power_output_for_frequency(l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MAX_TX_POWR = %d",l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.max_tx_pwr);

      arfcn = (uint16) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bcch_ARFCN;

#ifdef FEATURE_GSM_QUADBAND

/* for SGLTE, don't need to know band enable/disable at G, just do band convertion */
#ifdef FEATURE_SGLTE
      if (tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
      {
        geran_grr_band_ind_e grr_band_ind = (l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator == TDSL1_DCS_1800_BAND) ?
				                                 GERAN_GRR_BAND_IND_DCS_1800 : GERAN_GRR_BAND_IND_PCS_1900;
		ARFCN_T converted_arfcn = geran_grr_convert_to_arfcn_type(l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num, grr_band_ind);
		tdsconverted_gsm_band = converted_arfcn.band;
		TDSRRC_MSG2(MSG_LEGACY_HIGH,"SGLTE: arfcn %d,converted_gsm_band %d",l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num,tdsconverted_gsm_band);
      }
	  else
#endif /* FEATURE_SGLTE */
      {
        tdsconverted_gsm_band = tdsrrcirat_get_gsm_band(l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info);
      }

      if (tdsconverted_gsm_band != INVALID_BAND)
      {
        l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.band =
          tdsconverted_gsm_band;
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"ARFCN %d ignored bec of invalid band",inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bcch_ARFCN,0,0);
        arfcn = TDSMAX_ARFCN;
        if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
        {
          /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
          cell_position = (uint16)
            inter_rat_cell_list_ptr->elem[idx].interRATCellID;

        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
          /* Store the cell in first order position */
          cell_position = TDSAVAILABLE_CELL_POSITION;
        }
        /* Tell to get the first available cell position and

        * that would be the cell id. The called function would store
        * the cell id back in passed cell_position variable */
        if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
          return FAILURE;
        }
        idx++;
        continue;
      }

#endif /* FEATURE_GSM_QUADBAND */

      /* To add BSIC (Base Station Identification Code) */
      if ((inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc
        > TDSNCC_MAX) ||
        (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc
        > TDSBCC_MAX))
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"BCC %d or NCC %d invalid[0..7]",inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc,inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc);

        return FAILURE;

      } /* End of if of checking NCC and BCC validity */

      /* Store good values of bcc and ncc */
      l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.ncc =
        (uint8) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->bsic.ncc;

      l1_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.bcc =
        (uint8) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->bsic.bcc;
    } /* End of gsm cell  */

    /* Time to get its cell id*/
    /* Cell Order has already been verified */

    if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
    {
      /* TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id present"); */
      cell_position = (uint16)
        inter_rat_cell_list_ptr->elem[idx].interRATCellID;

    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
      /* Store the cell in first order position */
      cell_position = TDSAVAILABLE_CELL_POSITION;
    }
    /* Tell to get the first available cell position and

    * that would be the cell id. The called function would store
    * the cell id back in passed cell_position variable */
    if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
      return FAILURE;
    }
    l1_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id
      = cell_position;

    /* set the measure in PCH IDLE and FACH flag to TRUE */
    l1_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
    /* Increment cell count */

       if(sib18_present)
       {
         if(tmp_rrc_PLMNsOfInterRATCellsList_ptr->n > idy)
         {
           if(tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[idy].m.plmn_IdentityPresent)
           {
             tdsrrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[idy].plmn_Identity,
               &tmp_plmn_identity);
             if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
             {
               if((tdsrrccsp_check_for_eplmn(
                  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                    tmp_plmn_identity))) == FALSE)
               {
                 TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d Non eq PLMN measure only in DCH",l1_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
                 /* set the flag to FALSE */
                 l1_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
               }
             }
           }
           else
           {
             /* use the previous value of the flag if PLMN id is not selected PLMN */
             if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
             {
               /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
               if(cell_count > 0) /* to satisfy lint */
               {
                 l1_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach =
                   l1_sib_ptr->inter_s.add_cell[cell_count - 1].measure_in_idle_pch_fach;
               }
             }
           }

           idy++;
         }
         else
         {
           /* PLMN list is shorter than the cell info list set flag to true for the remaining cells */
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d measured in IDLE,FACH,PCH,DCH",l1_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
         }
       }
       else
       {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d measured in IDLE,FACH,PCH,DCH",l1_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
       }
    cell_count++;

    /* Increment the pointer */
    idx++;
    if ((inter_rat_cell_list_ptr->n > idx) && (cell_count == TDSL1_MAX_CELL_MEAS))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List full");
      break;
    }

  } /* End of while inter_rat_cell_list_ptr */

  /* Finally Set the cell count */
  l1_sib_ptr->inter_s.num_cell_add = cell_count;

  /* Now set inter sys info incl info to TRUE */
  l1_sib_ptr->inter_sys_meas_info_incl = TRUE;
  
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
 prev_cell_count = l1_sib_ptr->inter_s.num_cell_add;
 if(tdsrrcirat_read_sib11bis(l1_sib_ptr,sib18_present,tmp_rrc_PLMNsOfInterRATCellsList_ptr) == FAILURE)
 {
   TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Reading Sib11bis FAILURE");
   l1_sib_ptr->inter_s.num_cell_add = prev_cell_count;
 }
#endif

  return SUCCESS;
}



/*===========================================================================

FUNCTION  RRCIRAT_DELETE_DUP_NON_VERIFIED_CELLS

DESCRIPTION

 This function deletes duplicate non verified cells from Meas Report.

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcirat_delete_dup_non_verified_cells
(
 tdsl1_inter_sys_meas_result_struct_type* meas_results_ptr
)
{
  uint8 count = 0, tmp_list_count = 0; /* Count variable */

  if ((meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_gsm_cells == 0) ||
    (meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_gsm_cells == 1))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Dupl non-verf BSIC purging not done");
    return;
  }

  for (count = 0; count < meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_gsm_cells; count++)
  {
    memscpy(&tdstmp_measured_cells[count], sizeof(tdsl1_measured_cells_struct_type),
      &meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_cells[count],
      sizeof(tdsl1_measured_cells_struct_type));
  }

  for (count = 0; count < meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_gsm_cells -1; count++)
  {
    for (tmp_list_count = count+1; tmp_list_count < meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_gsm_cells &&
            tmp_list_count < TDSL1_MAX_REPORTED_GSM_CELLS; tmp_list_count++)
    {
      /* Now compare the cells */
      if (meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_cells[count].bsic_choice == TDSL1_BSIC_VERIFIED)
      {
        /* Come out of for loop for tmp_list_count. Read the next cell */
        break;
      }
      else /* BSIC NOT verified case */
      {
        if (CONVERT_ARFCN_TO_NUM(meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_cells[count].u.bsic_not_verified.bcch_arfcn) == TDSRRC_INVALID_ARFCN_FLAG)
        {
          /* Come out of for loop for tmp_list_count. Read the next cell */
          break;
        }
        /* Check this cell with all celss in tdstmp_measured_cells */
        if (tdstmp_measured_cells[tmp_list_count].bsic_choice == TDSL1_BSIC_VERIFIED)
        {
          /* Check with next cell. It could be BSIC non verified */
          continue;
        }
        else /* BSIC NOT verified case */
        {
          if (CONVERT_ARFCN_TO_NUM(meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_cells[count].u.bsic_not_verified.bcch_arfcn)
            == TDSRRC_INVALID_ARFCN_FLAG)
          {
            /* Check with next cell. It could be BSIC non verified */
            continue;
          }
          if (CONVERT_ARFCN_TO_NUM(tdstmp_measured_cells[tmp_list_count].u.bsic_not_verified.bcch_arfcn) ==
            CONVERT_ARFCN_TO_NUM(meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_cells[count].u.bsic_not_verified.bcch_arfcn))
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"CellCnt %d == CellCnt %d ARF %d", count, tmp_list_count,
              CONVERT_ARFCN_TO_NUM(meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_cells[count].u.bsic_not_verified.bcch_arfcn));

            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring cell %d ARGFCN %d",tmp_list_count,CONVERT_ARFCN_TO_NUM(tdstmp_measured_cells[tmp_list_count].u.bsic_not_verified.bcch_arfcn));

            /* Mark ARFCN as invalid so that it could be deleted later */
            CONVERT_ARFCN_TO_NUM(tdstmp_measured_cells[tmp_list_count].u.bsic_not_verified.bcch_arfcn) = TDSRRC_INVALID_ARFCN_FLAG;

            /* Go to end of inner for loop */
          }
          else
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"CellCnt %d != CellCnt %d ARF %d", count, tmp_list_count,
              CONVERT_ARFCN_TO_NUM(tdstmp_measured_cells[tmp_list_count].u.bsic_not_verified.bcch_arfcn));

            /* Good this cell does not match. Go to end of inner for loop */
          }
        } /* Else of BSIC not verified case for inner loop */

      } /* Else of BSIC not verified case for outer loop */
    } /* End of for of tmp_list_count */

  } /* End of for of count */

  /* Now list has already been updated. tdstmp_measured_cells contains the deleted cells */
  /* Now copy update the original list */
  count = 0;
  for (tmp_list_count = 0;
  tmp_list_count < meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_gsm_cells; tmp_list_count++)
  {
    if (tdstmp_measured_cells[tmp_list_count].bsic_choice == TDSL1_BSIC_VERIFIED)
    {
      /* No problem Just copy back */
      memscpy(&meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_cells[count++],
	      sizeof (tdsl1_measured_cells_struct_type),
	      &tdstmp_measured_cells[tmp_list_count], sizeof (tdsl1_measured_cells_struct_type));
    }
    else
    {
      if (CONVERT_ARFCN_TO_NUM(tdstmp_measured_cells[tmp_list_count].u.bsic_not_verified.bcch_arfcn) == TDSRRC_INVALID_ARFCN_FLAG)
      {
        /* Do not copy this ARFCN */
      }
      else
      {
        memscpy(&meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_cells[count++],
		sizeof (tdsl1_measured_cells_struct_type),
		&tdstmp_measured_cells[tmp_list_count], sizeof (tdsl1_measured_cells_struct_type));
      }
    }

  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"L1 specifed %d cells",meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_gsm_cells);

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"After purge dups,RRC has %d cells",count);

  /* Set the new cell cnt */
  meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_gsm_cells = count;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"New RRC cnt is %d",meas_results_ptr->inter_sys_msmt_results[0].u.gsm.measured_gsm_cells);
}

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R5

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_r5
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the INTER-RAT Measurement Control Message */
  uint8 count = 0, cnt = 0;

  boolean cell_info_ind_present = FALSE;
  uint8 cell_info_ind = 0;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_rat_meas_params;

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_SYS;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r4_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  tdsrrc_InterRATMeasurement_r4 *irat_meas_cmd_ptr = NULL;

  boolean bsic_verify = FALSE;

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.r4.measurementControl_r4;



  if (!(rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r4_release))
  {
    if((dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.u.r4.m.
        v4d0NonCriticalExtensionsPresent) &&
          (dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.
          v590NonCriticalExtensionsPresent) &&
          (dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.m.
          v5b0NonCriticalExtensionsPresent) &&

          (dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementControl_v5b0ext.m.
          interRATCellInfoIndicationPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"irat cell info ind present");
      cell_info_ind_present=TRUE;
      cell_info_ind = dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementControl_v5b0ext.interRATCellInfoIndication;
    }

    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"irat cell info ind not present");
      cell_info_ind_present=FALSE;
    }

  }

  /* Assign default values to inter_rat_meas_params */
  memset(&l1_inter_rat_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_rat_meas_params.meas_object.meas_type = TDSL1_INTER_SYS;

  /* Set Transaction Id and Meas Id */
  l1_inter_rat_meas_params.trans_id = tdstransaction_id;
  l1_inter_rat_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;

  if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r4_setup)
  {
    /* Double check that it is INTER-RAT Meas */
    if (rrc_mcm_ptr->measurementCommand.u.setup->t !=
        T_tdsrrc_MeasurementType_r4_interRATMeasurement)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    /* Check if meas quantity,reporting quantity & Reporting criteria is received.
    If either of them is not received, then configuration incomplete ERROR */

    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r4,interRATMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r4,interRATReportingQuantity)) &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.t == T_tdsrrc_InterRATMeasQuantity_ratSpecificInfo_gsm) &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity !=
      tdsrrc_MeasurementQuantityGSM_dummy) &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATReportingQuantity.ratSpecificInfo.t ==
      T_tdsrrc_InterRATReportingQuantity_ratSpecificInfo_gsm) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/ReportCriteria/R.Mode");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    
    if((rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.t
      == T_tdsrrc_InterRATReportCriteria_interRATReportingCriteria) &&
      (!(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria->m.interRATEventListPresent) ||
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria->interRATEventList.n == 0)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid,event criteria list absent ");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    
    irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement ;
    if((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
       (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
        T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Quality Estimate is FDD,which is not supported");
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

    /* Set Meas Identity */
    l1_inter_rat_meas_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;

    /* Set Meas Quantity as Inter-sys Meas Quan present */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;

    /* Initialize Default Value to intra_freq_meas_quan_incl */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl =  FALSE;

    /* Initialize Filter Coefficient to Default Value i.e. 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;

    /* Initialize GSM Filter Coefficient to 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
    TDSL1_GSM_FILTER_COEF_0;

    /* Meas Quantity Parameter 1 i.e. Quality Estimate */
    if(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity,tdsrrc_InterRATMeasQuantity,measQuantityUTRAN_QualityEstimate))
    {
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;

      /* Meas Quan i.e ISCP/RSCP/RSSI... */
      /* 10.3.7.38  intra meas quantity type is a list */
      tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan,
                               &rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.
                             measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->intraFreqMeasQuantity_TDDList);
      /* Get Filter Coefficient too */
      /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient_present */
     
        /* Store filter coefficient */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
          TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
          interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.filterCoefficient);
      
    } /* End of quality estimate present */

    /* Set System Choice to GSM */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;

    /* Get GSM Meas Quan, Filter Coefficient, BSIC required */

    /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
    So set it to RSSI */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
      TDSL1_GSM_CARRIER_RSSI;

    
      /* GSM Filter Coefficient present */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
        TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.
        u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo.u.gsm->filterCoefficient);

#ifdef FEATURE_ENH_BSIC_VERIFY    
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
    bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);

#else
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
      TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#endif

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"R5 MCM gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);

    /* Store Reporting Quantity */
    l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;

    /* Sys Choice */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;

    /* L1 wants this to be FALSE for the first version as it does not support it */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;

    /* Ignore reading Observed time difference as no longer supported in Rel 99
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
    rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
    ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */

    /* Check if gsm_carrier_rssi wanted in Meas Report */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
      rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
      ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;

    if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      reportCriteria, &l1_inter_rat_meas_params.meas_object.u.inter_s, &mcm_rpt_crit, tdstransaction_id))
    {
      return FAILURE;
    }


    /* Store reporting mode to TRUE */
    l1_inter_rat_meas_params.rpt_mode_incl = TRUE;

    l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

    /* For L1 sake, copy reporting criteria */
    l1_inter_rat_meas_params.reporting_mode.rpt_mode =
      l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;

    /* Now set other values */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

    l1_inter_rat_meas_params.meas_object_info_included = TRUE;


    /* Time to process the cell list */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r4,interRATCellInfoList))
    {
      if (tdsrrcirat_process_cell_list_mcm_r5(&rrc_mcm_ptr->measurementCommand.
        u.setup->u.interRATMeasurement->interRATCellInfoList,
        &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list) == FAILURE)
      {
        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
       rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      /* Set Cell list is present */
      l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
    }

    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

      /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
        if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
          addtl_meas_incl, TRUE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
          &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      }
    } /* End of if of Additional Meas Present */

    if( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r4,interRATCellInfoList))
    {
      /* At this point everything has been validated and now is the time
      to update the inter rat CELL_INFO_LIST */

      for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		&tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
      }
    }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        if(tdsinter_rat_meas_id_list[count].interrat_type != TDSRRCMEAS_IRAT_GSM)
        {
          tdsrrcirat_delete_mcm_due_to_irat_type_change(l1_inter_rat_meas_params.meas_id);
        }
        break;
      }
    }
#endif

    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
        tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
        meas_exists_for_setup = TRUE;
        break;
      }
    }

    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
      {
        /* First check if this meas already exists */
        if (tdsinter_rat_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
          tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
          break;
        }
      }

    } /* End of if */

    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }

    /* Now one needs to send command to L1 */

  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r4_modify)
  {
    /* Initialize Filter Coefficient to Default Value i.e. 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;

    /* Initialize GSM Filter Coefficient to 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
    TDSL1_GSM_FILTER_COEF_0;

    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_inter_rat_meas_params.rpt_mode_incl = TRUE;

      l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    /* Set type to MODIFY */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r4_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:I.RAT Modify: MType absent");

      /* Check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
      {
        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }
    }
    else
    {
      if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType,
          rrc_MeasurementType_r4_interRATMeasurement)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:Meas Id is not Inter-RAT");
        /* Send Meas Ctrl Failure  */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

      irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement;
      if(
          ((irat_meas_cmd_ptr->m.interRATMeasQuantityPresent) &&
            ((irat_meas_cmd_ptr->interRATMeasQuantity.ratSpecificInfo.t != T_tdsrrc_InterRATMeasQuantity_ratSpecificInfo_gsm) ||
              ((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
                (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
                 T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))))
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration not supported");
        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        /* Set faliure choice */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }

      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
        interRATMeasurement,tdsrrc_InterRATMeasurement_r4,interRATMeasQuantity)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Meas Quan absent");
      }
      else
      {
        /* Set Meas Quantity to TRUE */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = FALSE;
        if (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity ==
          tdsrrc_MeasurementQuantityGSM_dummy)
        {

          TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCM:MeasQuantity PathLoss");

          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

          /* Set Error Cause */
          rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }

        /* Meas Quantity Parameter 1 i.e. Quality Estimate */
        if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity,
          tdsrrc_InterRATMeasQuantity,measQuantityUTRAN_QualityEstimate))
        {
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;

          /* Meas Quan i.e ISCP/RSCP/RSSI... */

          /* 10.3.7.38  intra meas quantity type is a list */
          tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.u.inter_s.
                        meas_quan.intra_meas_quan,
                     &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
                     interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->
                     intraFreqMeasQuantity_TDDList);
          /* Get Filter Coefficient too */

 
          /* Get Filter Coefficient too */
          
            /* Store filter coefficient */
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
              TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u
              .modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
              measQuantityUTRAN_QualityEstimate.filterCoefficient);
          
        }

        /* Set System Choice to GSM */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;

        /* Get GSM Meas Quan, Filter Coefficient, BSIC required */

        /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
        So set it to RSSI */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
          TDSL1_GSM_CARRIER_RSSI;

        
          /* GSM Filter Coefficient present */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
            TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.
            modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
            ratSpecificInfo.u.gsm->filterCoefficient);
        
#ifdef FEATURE_ENH_BSIC_VERIFY
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
        bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
            modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#else
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
          TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
            modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#endif
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"R5 MCM Modify gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);
      } /* End of else of Meas Quan present */

      /* Check if reporting quan present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r4,interRATReportingQuantity))
      {
        /* Store Reporting Quantity */
        l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;

        /* Sys Choice */
        l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;

        /* L1 wants this to be FALSE for the first version as it does not support it */
        l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;

        /* Observed time difference no longer supported in Rel 99
        l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
          rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATReportingQuantity.ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */

          /* Check if gsm_carrier_rssi wanted in Meas Report */
          l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
          rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATReportingQuantity.ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;
      }

      if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.modify->
        measurementType.u.interRATMeasurement->reportCriteria,
        &l1_inter_rat_meas_params.meas_object.u.inter_s, &mcm_rpt_crit, tdstransaction_id))
      {
        return FAILURE;
      }
      l1_inter_rat_meas_params.reporting_mode.rpt_mode =
      l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;
      
      l1_inter_rat_meas_params.meas_object_info_included = TRUE;
      if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r4_modify,measurementType))
        && (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r4,interRATCellInfoList)) )
      {
        /* Time to process the cell list */
        if (tdsrrcirat_process_cell_list_mcm_r5(&rrc_mcm_ptr->measurementCommand.
          u.modify->measurementType.u.interRATMeasurement->interRATCellInfoList,
          &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list)
          == FAILURE)
        {
          /* Send Meas Ctrl Failure Msg */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

          /* Set Error Cause */
          rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
                    /*log the protocol error*/
          tdsrrc_log_protocol_error( (uint8) (tdsrrc_get_state()),
                                  (uint8)TDSRRC_PROCEDURE_IHO,
                                  (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                                  (uint8)TDSRRCLOG_BAD_IE
                                  );

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }

        /* Set Cell list is present */
        l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
      }

      /* Check for additional measurements */
      /* Call fn which makes check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r4_IEs,additionalMeasurementList))
      {
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type,&rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }


      /* Set reporting criteria in RRC DB */
      for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
      {
        if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
        {
          if(mcm_rpt_crit != TDSNOT_AVAILABLE)
          {      
            tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
            tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
            tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reportinf Crit set in RRC DB");
          }
          break;
        }
      }
      
      if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
         tdsrrc_MeasurementCommand_r4_modify,measurementType))
        && (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r4,interRATCellInfoList)) )
      {
        /* At this point everything has been validated and now is the time
        to update the inter rat CELL_INFO_LIST */
        for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		  &tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
        }
      }

    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
  * that this meas exists. So let us delete it from tdsinter_rat_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_rat_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_rat_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;

        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = FALSE;
        
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsinter_rat_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */



  if(rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r4_setup)
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
       tdsrrc_InterRATMeasurement_r4,interRATCellInfoList))
    {
      if(cell_info_ind_present==TRUE)
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=cell_info_ind;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }

  if(rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r4_modify)
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r4_modify,measurementType)) &&
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
       tdsrrc_InterRATMeasurement_r4,interRATCellInfoList)))
    {
      if(cell_info_ind_present==TRUE)
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=cell_info_ind;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }


  if(!tdsprocess_interfreq_interrat_meas_rel7)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending interRAT measurements from MCM to L1");
    return SUCCESS;
  }

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */

  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_rat_meas_params.trans_id = alloc_idx;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Sending cmd to L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_rat_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  

  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_CELL_LIST_MCM_R5

DESCRIPTION

  This function processes the cell list received in a Measurement Control Message

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If Cell list is invalid
  SUCCESS: If Cell list is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_cell_list_mcm_r5
(
  tdsrrc_InterRATCellInfoList_r4* mcm_cell_list_ptr,
  tdsl1_inter_sys_cell_list_struct_type* l1_cell_list_ptr
)
{
/* RRC new cell info list and l1 new cell info list would have the new cell info list
through local neighbour list. This has to be done to make sure that if the Meas Ctrl
  Msg contains bad cells, our good tdsinter_rat_cell_list does not get disturbed */

  uint8 cnt = 0;

  uint8 cell_count = 0;

  uint32 cell_position = 0,idx=0;

  uint16 cell_id = TDSINVALID_CELL_ID;

  /* For storing the new cell list */
  tdsrrc_NewInterRATCellList* local_rrc_new_cell_list_ptr = NULL;

  tdsrrc_CellsForInterRATMeasList* local_cells_for_meas_ptr = NULL;
  
  uint16 arfcn = 0;
  
  /* Initializes the local neighbor list to default values */
  tdsrrcirat_initialize_local_neighbor_list();
  
  /* Copy Inter-RAT CELL_INFO_LIST list to local neighbor list. This is necessary
  so as to make sure that in case UTRAN messes up, our old cell list is intact */

  for (cnt=0; cnt < TDSL1_MAX_CELL_MEAS; cnt++)
  {
    memscpy(&tdslocal_neighbor_list[cnt], sizeof(tdsinter_rat_cell_list_struct_type),
	    &tdsinter_rat_cell_list[cnt], sizeof(tdsinter_rat_cell_list_struct_type));
  }

  /* In Inter-RAT meas, remove is not optional */
  /* For sanity, initialize to 0 */
  l1_cell_list_ptr->num_cell_rmv = 0;

  if (tdsrrcirat_fill_removed_inter_rat_cells(&mcm_cell_list_ptr->removedInterRATCellList,
    l1_cell_list_ptr) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Removed Inter-RAT cells");
    return FAILURE;
  }

  /* If removeAll in MCM, then always configure to L1 without additional check,
   * to make sure L1 and RRC has aligned cell list
   */
  #if 0
  /* UTRAN can ask us to remove all cells when it is possible that for that
  * measurement (First time SETUP) we may not have any cell list.
  * To avoid L1 getting confused, let us make number of cells to be
  * removed to 0 in case the cell list does not exist previously.
  * So check in the CELL_INFO_LIST */

  if ((mcm_cell_list_ptr->removedInterRATCellList.t ==
    T_tdsrrc_RemovedInterRATCellList_removeAllInterRATCells) &&
    (tdsrrcirat_check_cell_absence_in_cell_list() == SUCCESS))
  {
    /* Set No of Cells removed to 0 from oxff */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting No of cells removed to 0");
    l1_cell_list_ptr->num_cell_rmv = 0;
  }
  #endif

  /* At this time, tdslocal_neighbor_list is completely updated and has
  * the latest and the greatest information
  * Get New cells to be added. UTRAN can just set it to NULL */
  if ( !(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(mcm_cell_list_ptr,
    tdsrrc_InterRATCellInfoList_r4,newInterRATCellList))
    || ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(mcm_cell_list_ptr,tdsrrc_InterRATCellInfoList_r4,
    newInterRATCellList))
    && (mcm_cell_list_ptr->newInterRATCellList.n == 0)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:No new cells to be added");
    /* Put this info in L1 list */
    l1_cell_list_ptr->num_cell_add = 0;
  }
  else
  {
    if (tdsrrcirat_check_inter_rat_cell_order_mcm(&mcm_cell_list_ptr->newInterRATCellList) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");
      return FAILURE;
    }
    /* New cells need to be added */
    local_rrc_new_cell_list_ptr =  &mcm_cell_list_ptr->newInterRATCellList;

    /* Before verifying the cell id, make sure that cell params are correct */
    idx=0;
    while ((local_rrc_new_cell_list_ptr ->n > idx) && (cell_count < TDSL1_MAX_CELL_MEAS))
    {
    /* Let first check the type of the cell to find if we
      really need to give this cell to L1 */
      if (local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.t !=
        T_tdsrrc_NewInterRATCell_technologySpecificInfo_gsm)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Ignoring Non GSM cell");
        idx++;
        continue;
      }
      else
      {
        l1_cell_list_ptr->add_cell[cell_count].cell_info_type = TDSL1_GSM_SYSTEM;

        /* Get GSM CELL Info */

        /* No need to read cell selec/reselect info as that is for SIB11/12 */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.cell_sel_info_incl = FALSE;

        /* Cell Individual Offset */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.cio = (int16)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->interRATCellIndividualOffset;

        /* Get BAND 1800 DCS or 1900 PCS */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator =
          TDSRET_INTER_RAT_BAND(local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->frequency_band);

        /* Get bcch_ARFCN */
        CONVERT_ARFCN_TO_NUM(l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn) = (uint16)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bcch_ARFCN;

#ifdef FEATURE_ENH_BSIC_VERIFY
        arfcn = (uint16) local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bcch_ARFCN;
#endif

#ifdef FEATURE_GSM_QUADBAND

/* for SGLTE, don't need to know band enable/disable at G, just do band convertion */
#ifdef FEATURE_SGLTE
          if (tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
          {
            geran_grr_band_ind_e grr_band_ind = (l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator == TDSL1_DCS_1800_BAND) ?
				                                 GERAN_GRR_BAND_IND_DCS_1800 : GERAN_GRR_BAND_IND_PCS_1900;
			ARFCN_T converted_arfcn = geran_grr_convert_to_arfcn_type(l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num, grr_band_ind);
			tdsconverted_gsm_band = converted_arfcn.band;
			TDSRRC_MSG2(MSG_LEGACY_HIGH,"SGLTE: arfcn %d,converted_gsm_band %d",l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num,tdsconverted_gsm_band);
          }
		  else
#endif /* FEATURE_SGLTE */
        {
            tdsconverted_gsm_band = tdsrrcirat_get_gsm_band(l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info);
          }

          if (tdsconverted_gsm_band != INVALID_BAND)
          {
            l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.band =
              tdsconverted_gsm_band;
          }
          else
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"ARFCN %d rejected bec of invalid band",local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
              u.gsm->bcch_ARFCN,0,0);
            idx++;
            return FAILURE;
          }

#endif /* FEATURE_GSM_QUADBAND */

        /* To add BSIC (Base Station Identification Code) */
        if ((local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc
          > TDSNCC_MAX) ||
          (local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc
          > TDSBCC_MAX))
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"BCC %d or NCC %d invalid[0..7]",local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc,local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc);

          return FAILURE;

        } /* End of if of checking NCC and BCC validity */

        /* Store good values of bcc and ncc */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bsic.ncc = (uint8)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.ncc;

        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bsic.bcc = (uint8)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.bcc;
      } /* End of gsm cell  */

      /* Time to get its cell id*/
      /* Cell Order has already been verified */

      if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(local_rrc_new_cell_list_ptr,interRATCellID))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Inter-RAT Cell Id present");
        cell_position = local_rrc_new_cell_list_ptr->elem[idx].interRATCellID;
        l1_cell_list_ptr->add_cell[cell_count].intersys_cell_id = (uint16)
          local_rrc_new_cell_list_ptr->elem[idx].interRATCellID;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
        /* Store the cell in first order position */
        cell_position = TDSAVAILABLE_CELL_POSITION;
        /* Tell to get the first available cell position and
        * that would be the cell id. The called function would store
        * the cell id back in passed cell_position variable */
      }


      /* Now store the cell id in local neighbour rat cell list */
      if (tdsrrcirat_update_inter_rat_cell_list_mcm(&cell_position,
        &l1_cell_list_ptr->add_cell[cell_count].intersys_cell_id, &cell_id, arfcn)
        == SUCCESS)
      {
        /* set this flag to TRUE always for MCM */
        l1_cell_list_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
        /* Increment cell count as add cell successfully stored */
        cell_count ++;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCM:Invalid Inter-RAT cell list");
        return FAILURE;
      }

      /* Increment the pointer */
      /* Read Next One */
      idx++;


      if ((local_rrc_new_cell_list_ptr->n > idx) && (cell_count == TDSL1_MAX_CELL_MEAS))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List full");
        break;
      }

    } /* End of while inter_rat_cell_list_ptr */

    /* Finally Set the cell count */
    l1_cell_list_ptr->num_cell_add = cell_count;

  } /* End of else of new cells added */

  cell_count = 0;

  /* Check for cells for meas */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(mcm_cell_list_ptr,
    tdsrrc_InterRATCellInfoList_r4,cellsForInterRATMeasList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cells for meas present");
    l1_cell_list_ptr->cells_for_meas_incl = TRUE;

    local_cells_for_meas_ptr = &mcm_cell_list_ptr->cellsForInterRATMeasList;
    idx=0;
    while (local_cells_for_meas_ptr->n > idx)
    {
      cell_id = (uint16) local_cells_for_meas_ptr->elem[idx];

      /* Check if the cell id is present */
      if( tdsrrcirat_find_cell_id(&cell_id) == FAILURE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"cell to measure with id = %d not found",cell_id);

      } /* End of if for cell id not found */
      else
      {
        /* Store cell id */
        l1_cell_list_ptr->meas_cells_list[cell_count++] = cell_id;
      }
      /* Increment Ptr */
      idx++;
    } /* End of while */

    l1_cell_list_ptr->num_cell_meas =  cell_count;

  } /* End of if of rrc_cellsForIntraFreqMeasList_present */

  return SUCCESS;
}


/*===========================================================================

FUNCTION   tdsrrcirat_add_irat_cell_info_ind

DESCRIPTION

  This function set the irat cell info ind in meas report.


DEPENDENCIES

  None

RETURN VALUE

  none

SIDE EFFECTS

  None

===========================================================================*/



void tdsrrcirat_add_irat_cell_info_ind(tdsrrc_MeasurementReport* rpt_ptr)
{
  if(tdsinterRAT_cell_info_status.interRAT_cell_info_present==TRUE)
  {
    if(rpt_ptr->m.v390nonCriticalExtensionsPresent)
    {
      if(rpt_ptr->v390nonCriticalExtensions.m.laterNonCriticalExtensionsPresent)
      {
        if(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent)
        {
          if(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent)
          {
            if(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.
               v5b0NonCriticalExtensionsPresent)
            {
              if(!(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.m.
               v690NonCriticalExtensionsPresent))
              {
                TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions);
              }
              TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
              rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.m.
                interRATCellInfoIndicationPresent =1;
              rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.interRATCellInfoIndication =
                  tdsinterRAT_cell_info_status.interRAT_cell_info_ind ;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"irat cell info ind added in meas report %d",tdsinterRAT_cell_info_status.interRAT_cell_info_ind);
            }
            else
            {
              TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions);
              rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.
                v5b0NonCriticalExtensionsPresent =1;

              TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions);

              TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
              rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.m.
               interRATCellInfoIndicationPresent = 1;

              rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.interRATCellInfoIndication =
                tdsinterRAT_cell_info_status.interRAT_cell_info_ind ;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"irat cell info ind added in meas report %d",tdsinterRAT_cell_info_status.interRAT_cell_info_ind);
            }
          }
          else
          {
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions);
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent =1;
           // rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext.bit_mask = 0;
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions);
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.
              v5b0NonCriticalExtensionsPresent = 1;
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext);

            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions);

            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
            TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext,
               interRATCellInfoIndication) ;

            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.interRATCellInfoIndication =
              tdsinterRAT_cell_info_status.interRAT_cell_info_ind ;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"irat cell info ind added in meas report %d",tdsinterRAT_cell_info_status.interRAT_cell_info_ind);
          }
        }
        else
        {
          if(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.m.measurementReport_r3_add_extPresent)
          {
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent=1 ;
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions);
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent=1 ;
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext);

            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions);
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v5b0NonCriticalExtensionsPresent=1 ;
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext);

            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions);

            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.m.interRATCellInfoIndicationPresent=1 ;

            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.interRATCellInfoIndication =
              tdsinterRAT_cell_info_status.interRAT_cell_info_ind ;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"irat cell info ind added in meas report %d",tdsinterRAT_cell_info_status.interRAT_cell_info_ind);
          }
          else
          {
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions);
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent=1 ;
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions);
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent =1 ;
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext);

            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions);
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v5b0NonCriticalExtensionsPresent =1;
            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext);

            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions);

            TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.m.
               interRATCellInfoIndicationPresent =1;

            rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.interRATCellInfoIndication =
              tdsinterRAT_cell_info_status.interRAT_cell_info_ind ;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"irat cell info ind added in meas report %d",tdsinterRAT_cell_info_status.interRAT_cell_info_ind);
          }
        }
      }
      else
      {
        TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions);
        rpt_ptr->v390nonCriticalExtensions.m.laterNonCriticalExtensionsPresent=1;
       // rpt_ptr->v390nonCriticalExtensions.measurementReport_v390ext.bit_mask = 0;
        TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions);
        rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent=1 ;

        TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions);
        rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent=1 ;
        TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext);

        TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions);
        rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v5b0NonCriticalExtensionsPresent=1 ;
        TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext);

        TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions);

        TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
        rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.m.
          interRATCellInfoIndicationPresent =1;

        rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.interRATCellInfoIndication =
          tdsinterRAT_cell_info_status.interRAT_cell_info_ind ;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"irat cell info ind added in meas report %d",tdsinterRAT_cell_info_status.interRAT_cell_info_ind);
      }

    }
    else
    {
      rpt_ptr->m.v390nonCriticalExtensionsPresent =1;

      TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions);
      rpt_ptr->v390nonCriticalExtensions.m.laterNonCriticalExtensionsPresent =1;
      TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.measurementReport_v390ext);

      TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions);
      rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.m.v4b0NonCriticalExtensionsPresent =1;

      TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions);
      rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent=1 ;
      TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.measurementReport_v4b0ext);

      TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions);
      rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v5b0NonCriticalExtensionsPresent =1;
      TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.measurementReport_v590ext);

      TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions);

      TDSRRC_RESET_MSG_IE_PRESENT(rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext);
      rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.m.
          interRATCellInfoIndicationPresent =1;

      rpt_ptr->v390nonCriticalExtensions.laterNonCriticalExtensions.v4b0NonCriticalExtensions.v590NonCriticalExtensions.v5b0NonCriticalExtensions.measurementReport_v5b0ext.interRATCellInfoIndication =
          tdsinterRAT_cell_info_status.interRAT_cell_info_ind ;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"irat cell info ind added in meas report %d",tdsinterRAT_cell_info_status.interRAT_cell_info_ind);
    }

  }
}


/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R6

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_r6
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the INTER-RAT Measurement Control Message */
  uint8 count = 0, cnt = 0;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_rat_meas_params;

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_SYS;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r6_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  tdsrrc_InterRATMeasurement_r6* irat_meas_cmd_ptr = NULL;

  boolean bsic_verify = FALSE;
  
  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.r6.measurementControl_r6;

  /* Assign default values to inter_rat_meas_params */
  memset(&l1_inter_rat_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_rat_meas_params.meas_object.meas_type = TDSL1_INTER_SYS;

  /* Set Transaction Id and Meas Id */
  l1_inter_rat_meas_params.trans_id = tdstransaction_id;
  l1_inter_rat_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;


  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r6_setup))
  {
    /* Double check that it is INTER-RAT Meas */
    if (!TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(rrc_mcm_ptr->measurementCommand.u.setup,
        rrc_MeasurementType_r6_interRATMeasurement))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    /* Check if meas quantity,reporting quantity & Reporting criteria is received.
    If either of them is not received, then configuration incomplete ERROR */

    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r6,interRATMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r6,interRATReportingQuantity)) &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.t == T_tdsrrc_InterRATMeasQuantity_ratSpecificInfo_gsm) &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity !=
      tdsrrc_MeasurementQuantityGSM_dummy) &&
      (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATReportingQuantity.ratSpecificInfo,
      rrc_InterRATReportingQuantity_ratSpecificInfo_gsm)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/ReportCriteria/R.Mode");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria,
      rrc_InterRATReportCriteria,interRATReportingCriteria)) &&
      (!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria,interRATEventList)) ||
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria->interRATEventList.n == 0)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid,event criteria list absent ");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement ;
    if((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
       (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
        T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Quality Estimate is FDD,which is not supported");
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

    /* Set Meas Identity */
    l1_inter_rat_meas_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;

    /* Set Meas Quantity as Inter-sys Meas Quan present */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;

    /* Initialize Default Value to intra_freq_meas_quan_incl */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl =  FALSE;

    /* Initialize Filter Coefficient to Default Value i.e. 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;

    /* Initialize GSM Filter Coefficient to 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
    TDSL1_GSM_FILTER_COEF_0;

    /* Meas Quantity Parameter 1 i.e. Quality Estimate */
    if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity,tdsrrc_InterRATMeasQuantity,measQuantityUTRAN_QualityEstimate))
    {
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;

      /* Meas Quan i.e ISCP/RSCP/RSSI... */

      /* 10.3.7.38  intra meas quantity type is a list */
      tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.u.inter_s.
                  meas_quan.intra_meas_quan,
               &rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.
                 measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->
               intraFreqMeasQuantity_TDDList);

      /* Get Filter Coefficient too */
      /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient_present */
     
        /* Store filter coefficient */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
          TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.setup->u.
          interRATMeasurement->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.filterCoefficient);
     
    } /* End of quality estimate present */

    /* Set System Choice to GSM */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;

    /* Get GSM Meas Quan, Filter Coefficient, BSIC required */

    /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
    So set it to RSSI */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
      TDSL1_GSM_CARRIER_RSSI;

  
      /* GSM Filter Coefficient present */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
        TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.
        u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo.u.gsm->filterCoefficient);
    

#ifdef FEATURE_ENH_BSIC_VERIFY
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
    bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#else
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
      TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#endif

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"R6 MCM gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);

    /* Store Reporting Quantity */
    l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;

    /* Sys Choice */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;

    /* L1 wants this to be FALSE for the first version as it does not support it */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;

    /* Ignore reading Observed time difference as no longer supported in Rel 99
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
    rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
    ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */

    /* Check if gsm_carrier_rssi wanted in Meas Report */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
      rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
      ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;

    if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.setup->
      u.interRATMeasurement->reportCriteria, &l1_inter_rat_meas_params.meas_object.u.inter_s,
      &mcm_rpt_crit, tdstransaction_id))
    {
      return FAILURE;
    }

    /* Store reporting mode to TRUE */
    l1_inter_rat_meas_params.rpt_mode_incl = TRUE;

    l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

    /* For L1 sake, copy reporting criteria */
    l1_inter_rat_meas_params.reporting_mode.rpt_mode =
      l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;

    /* Now set other values */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

    l1_inter_rat_meas_params.meas_object_info_included = TRUE;


    /* Time to process the cell list */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r6,interRATCellInfoList))
    {
      if (tdsrrcirat_process_cell_list_mcm_r6(&rrc_mcm_ptr->measurementCommand.
        u.setup->u.interRATMeasurement->interRATCellInfoList,
        &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list) == FAILURE)
      {
        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
       rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      /* Set Cell list is present */
      l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
    }

    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

      /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
        if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
          addtl_meas_incl, TRUE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
          &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      }
    } /* End of if of Additional Meas Present */

    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r6,interRATCellInfoList))
    {
      /* At this point everything has been validated and now is the time
      to update the inter rat CELL_INFO_LIST */

      for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		&tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
      }
    }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        if(tdsinter_rat_meas_id_list[count].interrat_type != TDSRRCMEAS_IRAT_GSM)
        {
          tdsrrcirat_delete_mcm_due_to_irat_type_change(l1_inter_rat_meas_params.meas_id);
        }
         break;
      }
    }
#endif

    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
        tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
        meas_exists_for_setup = TRUE;
        break;
      }
    }

    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
      {
        /* First check if this meas already exists */
        if (tdsinter_rat_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
          tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
          break;
        }
      }

    } /* End of if */

    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }

    /* Now one needs to send command to L1 */

  } /* End of if of set up */
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r6_modify))
  {
    /* Initialize Filter Coefficient to Default Value i.e. 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;

    /* Initialize GSM Filter Coefficient to 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
    TDSL1_GSM_FILTER_COEF_0;

    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_inter_rat_meas_params.rpt_mode_incl = TRUE;

      l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    /* Set type to MODIFY */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r6_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:I.RAT Modify: MType absent");

      /* Check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
      {
        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }
    }
    else
    {
      if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType,
          rrc_MeasurementType_r6_interRATMeasurement)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:Meas Id is not Inter-RAT");
        /* Send Meas Ctrl Failure  */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement;
      if(
          ((irat_meas_cmd_ptr->m.interRATMeasQuantityPresent) &&
            ((irat_meas_cmd_ptr->interRATMeasQuantity.ratSpecificInfo.t != T_tdsrrc_InterRATMeasQuantity_ratSpecificInfo_gsm) ||
              ((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
                (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
                 T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))))
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration not supported");
        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        /* Set faliure choice */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
        interRATMeasurement,tdsrrc_InterRATMeasurement_r6,interRATMeasQuantity)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Meas Quan absent");
      }
      else
      {
        /* Set Meas Quantity to TRUE */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = FALSE;
        if (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity ==
          tdsrrc_MeasurementQuantityGSM_dummy)
        {

          TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCM:MeasQuantity PathLoss");

          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

          /* Set Error Cause */
          rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }

        /* Meas Quantity Parameter 1 i.e. Quality Estimate */
        if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
           interRATMeasQuantity,tdsrrc_InterRATMeasQuantity,measQuantityUTRAN_QualityEstimate))
        {
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;

          /* Meas Quan i.e ISCP/RSCP/RSSI... */

          /* 10.3.7.38  intra meas quantity type is a list */
          tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.u.inter_s.
                      meas_quan.intra_meas_quan,
                   &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
                     interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->
                   intraFreqMeasQuantity_TDDList);
          /* Get Filter Coefficient too */


          /* Get Filter Coefficient too */
          /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient_present */
         
            /* Store filter coefficient */
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
              TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u
              .modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
              measQuantityUTRAN_QualityEstimate.filterCoefficient);
         
        }

        /* Set System Choice to GSM */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;

        /* Get GSM Meas Quan, Filter Coefficient, BSIC required */

        /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
        So set it to RSSI */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
          TDSL1_GSM_CARRIER_RSSI;

      
          /* GSM Filter Coefficient present */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
            TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.
            modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
            ratSpecificInfo.u.gsm->filterCoefficient);
        

#ifdef FEATURE_ENH_BSIC_VERIFY
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
        bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
            modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#else
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
          TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
            modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#endif

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"R6 MCM Modify gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);
      } /* End of else of Meas Quan present */

      /* Check if reporting quan present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r6,interRATReportingQuantity))
      {
        /* Store Reporting Quantity */
        l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;

        /* Sys Choice */
        l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;

        /* L1 wants this to be FALSE for the first version as it does not support it */
        l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;

        /* Observed time difference no longer supported in Rel 99
        l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
          rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATReportingQuantity.ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */

          /* Check if gsm_carrier_rssi wanted in Meas Report */
          l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
          rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATReportingQuantity.ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;
      }

      if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.modify->
        measurementType.u.interRATMeasurement->reportCriteria,
        &l1_inter_rat_meas_params.meas_object.u.inter_s, &mcm_rpt_crit, tdstransaction_id))
      {
        return FAILURE;
      }
      l1_inter_rat_meas_params.reporting_mode.rpt_mode =
      l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;
      
      l1_inter_rat_meas_params.meas_object_info_included = TRUE;
      if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r6_modify,measurementType))
        && (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r6,interRATCellInfoList)) )
      {
        /* Time to process the cell list */
        if (tdsrrcirat_process_cell_list_mcm_r6(&rrc_mcm_ptr->measurementCommand.
          u.modify->measurementType.u.interRATMeasurement->interRATCellInfoList,
          &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list)
          == FAILURE)
        {
          /* Send Meas Ctrl Failure Msg */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

          /* Set Error Cause */
          rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }

        /* Set Cell list is present */
        l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
      }

      /* Check for additional measurements */
      /* Call fn which makes check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,additionalMeasurementList))
      {
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type,&rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }


      /* Set reporting criteria in RRC DB */
      for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
      {
        if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
        {
          if(mcm_rpt_crit != TDSNOT_AVAILABLE)
          {      
            tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
            tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
            tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reportinf Crit set in RRC DB");
          }
          break;
        }
      }
      
      if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
         tdsrrc_MeasurementCommand_r6_modify,measurementType))
        && (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r6,interRATCellInfoList)) )
      {
        /* At this point everything has been validated and now is the time
        to update the inter rat CELL_INFO_LIST */
        for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		  &tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
        }
      }

    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
  * that this meas exists. So let us delete it from tdsinter_rat_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_rat_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_rat_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;
        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = FALSE;
        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsinter_rat_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */


  if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r6_setup))
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
       tdsrrc_InterRATMeasurement_r6,interRATCellInfoList)))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATCellInfoList,
          interRATCellInfoIndication_r6))
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind = rrc_mcm_ptr->measurementCommand.
                                 u.setup->u.interRATMeasurement->interRATCellInfoList.interRATCellInfoIndication_r6;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }

  if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r6_modify))
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r6_modify,measurementType)) &&
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r6,interRATCellInfoList)))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATCellInfoList,
        interRATCellInfoIndication_r6))
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind = rrc_mcm_ptr->measurementCommand.
           u.modify->measurementType.u.interRATMeasurement->interRATCellInfoList.interRATCellInfoIndication_r6;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }

  if(!tdsprocess_interfreq_interrat_meas_rel7)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending interRAT measurements from MCM to L1");
    return SUCCESS;
  }

  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_rat_meas_params.trans_id = alloc_idx;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Sending cmd to L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_rat_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);
  

  return SUCCESS;
}

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_CELL_LIST_MCM_R6

DESCRIPTION

  This function processes the cell list received in a Measurement Control Message

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If Cell list is invalid
  SUCCESS: If Cell list is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_cell_list_mcm_r6
(
  tdsrrc_InterRATCellInfoList_r6* mcm_cell_list_ptr,
  tdsl1_inter_sys_cell_list_struct_type* l1_cell_list_ptr
)
{
/* RRC new cell info list and l1 new cell info list would have the new cell info list
through local neighbour list. This has to be done to make sure that if the Meas Ctrl
  Msg contains bad cells, our good tdsinter_rat_cell_list does not get disturbed */

  uint8 cnt = 0;

  uint8 cell_count = 0;

  uint32 cell_position = 0;

  uint16 cell_id = TDSINVALID_CELL_ID;

  /* For storing the new cell list */
  tdsrrc_NewInterRATCellList* local_rrc_new_cell_list_ptr = NULL;
  uint32 idx=0;
  tdsrrc_CellsForInterRATMeasList* local_cells_for_meas_ptr = NULL;

  uint16 arfcn = 0;
  
  /* Initializes the local neighbor list to default values */
  tdsrrcirat_initialize_local_neighbor_list();
  
  /* Copy Inter-RAT CELL_INFO_LIST list to local neighbor list. This is necessary
  so as to make sure that in case UTRAN messes up, our old cell list is intact */

  for (cnt=0; cnt < TDSL1_MAX_CELL_MEAS; cnt++)
  {
    memscpy(&tdslocal_neighbor_list[cnt], sizeof(tdsinter_rat_cell_list_struct_type),
	    &tdsinter_rat_cell_list[cnt], sizeof(tdsinter_rat_cell_list_struct_type));
  }

  /* In Inter-RAT meas, remove is not optional */
  /* For sanity, initialize to 0 */
  l1_cell_list_ptr->num_cell_rmv = 0;

  if (tdsrrcirat_fill_removed_inter_rat_cells(&mcm_cell_list_ptr->removedInterRATCellList,
    l1_cell_list_ptr) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Removed Inter-RAT cells");
    return FAILURE;
  }

  /* If removeAll in MCM, then always configure to L1 without additional check,
   * to make sure L1 and RRC has aligned cell list
   */
  #if 0
  /* UTRAN can ask us to remove all cells when it is possible that for that
  * measurement (First time SETUP) we may not have any cell list.
  * To avoid L1 getting confused, let us make number of cells to be
  * removed to 0 in case the cell list does not exist previously.
  * So check in the CELL_INFO_LIST */

  if ((TDSRRC_CHECK_COMMON_MSG_TYPE2(mcm_cell_list_ptr->removedInterRATCellList,
    rrc_RemovedInterRATCellList, removeAllInterRATCells)) &&
    (tdsrrcirat_check_cell_absence_in_cell_list() == SUCCESS))
  {
    /* Set No of Cells removed to 0 from oxff */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting No of cells removed to 0");
    l1_cell_list_ptr->num_cell_rmv = 0;
  }
  #endif

  /* At this time, tdslocal_neighbor_list is completely updated and has
  * the latest and the greatest information
  * Get New cells to be added. UTRAN can just set it to NULL */
  if ( !(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(mcm_cell_list_ptr,tdsrrc_InterRATCellInfoList_r6,newInterRATCellList))
    || ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(mcm_cell_list_ptr,tdsrrc_InterRATCellInfoList_r6,newInterRATCellList))
    && (mcm_cell_list_ptr->newInterRATCellList.n == 0)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:No new cells to be added");
    /* Put this info in L1 list */
    l1_cell_list_ptr->num_cell_add = 0;
  }
  else
  {
    if (tdsrrcirat_check_inter_rat_cell_order_mcm(&mcm_cell_list_ptr->newInterRATCellList) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");
      return FAILURE;
    }
    /* New cells need to be added */
    local_rrc_new_cell_list_ptr =  &mcm_cell_list_ptr->newInterRATCellList;

    /* Before verifying the cell id, make sure that cell params are correct */

    while ((local_rrc_new_cell_list_ptr->n > idx) && (cell_count < TDSL1_MAX_CELL_MEAS))
    {
    /* Let first check the type of the cell to find if we
      really need to give this cell to L1 */
      if (local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.t !=
        T_tdsrrc_NewInterRATCell_technologySpecificInfo_gsm)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Ignoring Non GSM cell");
        idx++;
        continue;
      }
      else
      {
        l1_cell_list_ptr->add_cell[cell_count].cell_info_type = TDSL1_GSM_SYSTEM;

        /* Get GSM CELL Info */

        /* No need to read cell selec/reselect info as that is for SIB11/12 */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.cell_sel_info_incl = FALSE;

        /* Cell Individual Offset */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.cio = (int16)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->interRATCellIndividualOffset;

        /* Get BAND 1800 DCS or 1900 PCS */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator =
          TDSRET_INTER_RAT_BAND(local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->frequency_band);

        /* Get bcch_ARFCN */
        CONVERT_ARFCN_TO_NUM(l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn) = (uint16)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bcch_ARFCN;

#ifdef FEATURE_ENH_BSIC_VERIFY
        arfcn = (uint16) local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bcch_ARFCN;
#endif

#ifdef FEATURE_GSM_QUADBAND

/* for SGLTE, don't need to know band enable/disable at G, just do band convertion */
#ifdef FEATURE_SGLTE
          if (tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
        {
            geran_grr_band_ind_e grr_band_ind = (l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator == TDSL1_DCS_1800_BAND) ?
				                                 GERAN_GRR_BAND_IND_DCS_1800 : GERAN_GRR_BAND_IND_PCS_1900;
			ARFCN_T converted_arfcn = geran_grr_convert_to_arfcn_type(l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num, grr_band_ind);
			tdsconverted_gsm_band = converted_arfcn.band;
			TDSRRC_MSG2(MSG_LEGACY_HIGH,"SGLTE: arfcn %d,converted_gsm_band %d",l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num,tdsconverted_gsm_band);
          }
		  else
#endif /* FEATURE_SGLTE */
          {
            tdsconverted_gsm_band = tdsrrcirat_get_gsm_band(l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info);
          }

		  if (tdsconverted_gsm_band != INVALID_BAND)
          {
            l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.band =
              tdsconverted_gsm_band;
          }
          else
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"ARFCN %d rejected bec of invalid band",local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
              u.gsm->bcch_ARFCN,0,0);
            idx++;
            return FAILURE;
          }

#endif /* FEATURE_GSM_QUADBAND */

        /* To add BSIC (Base Station Identification Code) */
        if ((local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc
          > TDSNCC_MAX) ||
          (local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc
          > TDSBCC_MAX))
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"BCC %d or NCC %d invalid[0..7]",local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc,local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc);

          return FAILURE;

        } /* End of if of checking NCC and BCC validity */

        /* Store good values of bcc and ncc */
        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bsic.ncc = (uint8)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.ncc;

        l1_cell_list_ptr->add_cell[cell_count].u.gsm_cell.gsm_info.bsic.bcc = (uint8)
          local_rrc_new_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bsic.bcc;
      } /* End of gsm cell  */

      /* Time to get its cell id*/
      /* Cell Order has already been verified */

      if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(local_rrc_new_cell_list_ptr,interRATCellID))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Inter-RAT Cell Id present");
        cell_position = local_rrc_new_cell_list_ptr->elem[idx].interRATCellID;
        l1_cell_list_ptr->add_cell[cell_count].intersys_cell_id = (uint16)
          local_rrc_new_cell_list_ptr->elem[idx].interRATCellID;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell Id absent");
        /* Store the cell in first order position */
        cell_position = TDSAVAILABLE_CELL_POSITION;
        /* Tell to get the first available cell position and
        * that would be the cell id. The called function would store
        * the cell id back in passed cell_position variable */
      }


      /* Now store the cell id in local neighbour rat cell list */
      if (tdsrrcirat_update_inter_rat_cell_list_mcm(&cell_position,
        &l1_cell_list_ptr->add_cell[cell_count].intersys_cell_id, &cell_id, arfcn)
        == SUCCESS)
      {
        /* set this flag to TRUE always for MCM */
        l1_cell_list_ptr->add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
        /* Increment cell count as add cell successfully stored */
        cell_count ++;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCM:Invalid Inter-RAT cell list");
        return FAILURE;
      }

      /* Increment the pointer */
      /* Read Next One */
      idx++;


      if ((local_rrc_new_cell_list_ptr->n > idx) && (cell_count == TDSL1_MAX_CELL_MEAS))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List full");
        break;
      }

    } /* End of while inter_rat_cell_list_ptr */

    /* Finally Set the cell count */
    l1_cell_list_ptr->num_cell_add = cell_count;

  } /* End of else of new cells added */

  cell_count = 0;

  /* Check for cells for meas */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(mcm_cell_list_ptr,
    tdsrrc_InterRATCellInfoList_r6,cellsForInterRATMeasList))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cells for meas present");
    l1_cell_list_ptr->cells_for_meas_incl = TRUE;

    local_cells_for_meas_ptr = &mcm_cell_list_ptr->cellsForInterRATMeasList;
    idx=0;
    while (local_cells_for_meas_ptr->n > idx)
    {
      cell_id = (uint16) local_cells_for_meas_ptr->elem[idx];

      /* Check if the cell id is present */
      if( tdsrrcirat_find_cell_id(&cell_id) == FAILURE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"cell to measure with id = %d not found",cell_id);

      } /* End of if for cell id not found */
      else
      {
        /* Store cell id */
        l1_cell_list_ptr->meas_cells_list[cell_count++] = cell_id;
      }
      /* Increment Ptr */
      idx++;
    } /* End of while */

    l1_cell_list_ptr->num_cell_meas =  cell_count;

  } /* End of if of rrc_cellsForIntraFreqMeasList_present */


  return SUCCESS;
}

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT

/*===========================================================================

FUNCTION tdsrrcirat_read_sib11bis

DESCRIPTION

  Reads SIB11bis from the current active serving cell for all the measurements.
  If SIB11bis information is incorrect, meas setup command is not send to L1/MAC
  and UE does not do any measurements..

DEPENDENCIES

  SIB must have been stored by the SIB procedure

RETURN VALUE

  Success if SIB11bis has been correctly read
  Failure if SIB11bis has not been correctly read

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_read_sib11bis
(
  tdsl1_meas_sib_parm_struct_type* l1_inter_rat_sib_ptr,
  boolean sib18_present,
  tdsrrc_PLMNsOfInterRATCellsList tmp_rrc_PLMNsOfInterRATCellsList_ptr
)
{
  struct tdsrrc_SysInfoType11bis *sib11bis_ptr = NULL;
  struct tdsrrc_MeasurementControlSysInfoExtensionAddon_r5 *sib11bis_rel5_ptr = NULL;
  struct tdsrrc_NewInterRATCellList *inter_rat_cell_list_ptr = NULL;
  tdsrrc_SysInfoType3  *sib3_ptr = NULL;
  tdsrrc_SysInfoType4  *sib4_ptr = NULL;
  tdsrrc_MeasurementControlSysInfoExtensionAddon_r5_newInterRATCellInfoListAddon_r5 *rel5_ext_ptr = NULL;
  uint16 cell_count =0;
  boolean sib4_present = FALSE;
  uint16 cell_position;
  boolean hcs_used = FALSE;
  uint32 idx=0;
  uint16 arfcn;

  rrc_plmn_identity_type tmp_plmn_identity;
  rrc_plmn_identity_type selected_plmn_identity;
  tdsrrc_int_cell_id_type  tmp_cell_id;
  
  tmp_plmn_identity.mcc[0] =  tmp_plmn_identity.mcc[1] = tmp_plmn_identity.mcc[2] = 0;
  tmp_plmn_identity.num_mnc_digits = 0;
  tmp_plmn_identity.mnc[0] =  tmp_plmn_identity.mnc[1] = 0;

  selected_plmn_identity.mcc[0] =  selected_plmn_identity.mcc[1] = selected_plmn_identity.mcc[2] = 0;
  selected_plmn_identity.num_mnc_digits = 0;
  selected_plmn_identity.mnc[0] =  selected_plmn_identity.mnc[1] = 0;
  
  if(!tdsrrcsibdb_is_sib11bis_present_in_srv_cell())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: SIB11bis is not present in Serving Cell");
    return FAILURE;
  }
  sib11bis_ptr = (tdsrrc_SysInfoType11bis *)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB11bis);
  if(sib11bis_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: SIB11bis ptr is NULL");
    return FAILURE;
  }
  if(!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib11bis_ptr,measurementControlSysInfo)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Mesaurement Control SysInfo is not Present in SIB11bis");
    return FAILURE;
  }
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib11bis_ptr,measurementControlSysInfoExtensionAddon_r5))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Rel5 Measurement Control info present in SIB11bis");
    sib11bis_rel5_ptr = &sib11bis_ptr->measurementControlSysInfoExtensionAddon_r5;
    if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib11bis_rel5_ptr,newInterRATCellInfoListAddon_r5))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Rel5 Cell Selection Reselection info present for Inter RAT");
      rel5_ext_ptr = &sib11bis_rel5_ptr->newInterRATCellInfoListAddon_r5;
    }
  }
  sib3_ptr = (tdsrrc_SysInfoType3 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB3);
  if( sib3_ptr == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB11bis: SIB3 ptr is NULL");
    return( FAILURE );
  }
  
  if (((tdsrrcmeas_current_substate == TDSRRCMEAS_CELL_FACH) || (tdsrrcmeas_current_substate == TDSRRCMEAS_PCH)) &&
    ((sib3_ptr->sib4indicator == TRUE)))
  {
    sib4_ptr = (tdsrrc_SysInfoType4 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB4);

    if (sib4_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB11bis: SIB4 ptr NULL.Use SIB3");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Use default values from SIB4");
      sib4_present = TRUE;
    }
  }

  if(sib18_present == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: SIB18 is Absent/Ignored");
  }
  else if(tdsrrcccm_get_serv_plmn_cellid(&tmp_cell_id, &selected_plmn_identity) != TDSRRC_CELL_LOSS_NO_NEW_CELL)
  {
    memscpy(&tmp_plmn_identity, sizeof(rrc_plmn_identity_type), &selected_plmn_identity, sizeof(rrc_plmn_identity_type));
  }
  else
  {
    sib18_present = FALSE;
  }
  if(sib11bis_ptr->measurementControlSysInfo.use_of_HCS.t == 
      T_tdsrrc_MeasurementControlSysInfoExtension_use_of_HCS_hcs_used)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: HCS Used in SIB11bis");
    hcs_used = TRUE;
    if(!(sib11bis_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->m.newInterRATCellListPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter RAT cell list is not present in SIB11bis");
      return FAILURE;
    }
    inter_rat_cell_list_ptr = &sib11bis_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_used->newInterRATCellList;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: HCS not Used in SIB11bis");
    hcs_used = FALSE;
    if(!(sib11bis_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->m.newInterRATCellListPresent))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter RAT cell list is not present in SIB11bis");
      return FAILURE;
    }
    inter_rat_cell_list_ptr = &sib11bis_ptr->measurementControlSysInfo.use_of_HCS.u.hcs_not_used->newInterRATCellList;
  }
  cell_count = l1_inter_rat_sib_ptr->inter_s.num_cell_add;
  while ( (inter_rat_cell_list_ptr->n > idx) &&
    (cell_count < TDSL1_MAX_CELL_MEAS) )
  {
    /* Let first check the type of the cell to find if we
    * really need to give this cell to L1 */
    if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.t !=
      T_tdsrrc_NewInterRATCell_technologySpecificInfo_gsm)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Ignoring Non GSM cell");
      arfcn = TDSMAX_ARFCN;
      if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
      {
        cell_position = (uint16)
          inter_rat_cell_list_ptr->elem[idx].interRATCellID;
      }
      else
      {
        cell_position = TDSAVAILABLE_CELL_POSITION;
      }
      /* Tell to get the first available cell position and
  
      * that would be the cell id. The called function would store
      * the cell id back in passed cell_position variable */
      if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inter-RAT Cell List invalid");
        return FAILURE;
      }
      idx++;
      continue;
    }
    else
    {
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].cell_info_type = TDSL1_GSM_SYSTEM;
  
      /* Cell reselection info is always set to TRUE. If not present in SIB 11, just
      set it to default values */
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
        gsm_cell.cell_sel_info_incl = TRUE;
  
      /* Initializw q-offset1-2 Defaults as per specs */
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
        gsm_cell.cell_sel_info.qoffset1_s_n = TDSQOFFSET1;
  
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
        gsm_cell.cell_sel_info.qoffset2_s_n = TDSQOFFSET2;
  
      /* -24. Got this value from Radio Access Capability */
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
        gsm_cell.cell_sel_info.max_tx_pwr = -24;
  
      /* Set default values for the HCS neighboring cell info if it is first cell
       * else use the info from the previous cell */
      if(hcs_used == TRUE)
      {
        if(cell_count == 0)
        {
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
             gsm_cell.hcs_info.hcs_prio = 0;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.q_hcs = 110;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
           gsm_cell.hcs_info.penalty_time = 0;
        }
        else
        {
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.hcs_prio =
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
            gsm_cell.hcs_info.hcs_prio;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.q_hcs =
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
            gsm_cell.hcs_info.q_hcs;
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.hcs_info.penalty_time =
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
            gsm_cell.hcs_info.penalty_time;
          
          if(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
            gsm_cell.hcs_info.penalty_time != 0)
          {
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
              gsm_cell.hcs_info.tmp_offset1 =
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].u.
              gsm_cell.hcs_info.tmp_offset1;
          }
        }
      }
      if ((sib4_present == TRUE) && (sib4_ptr != NULL))
      {
        if (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.t 
          != T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB11bis: SIB4 unsupport FDD");
          return FAILURE;
        }
      /* Not valid for Inter-RAT but initialize to default
        * as they are present */
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info.rxlev_min = (int16)
          (sib4_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
      }
      else
      {
        if (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.t 
          != T_tdsrrc_CellSelectReselectInfoSIB_3_4_modeSpecificInfo_tdd)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB11bis: SIB3 unsupport FDD");
          return FAILURE;
        }
  
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.cell_sel_info.rxlev_min = (int16)
          (sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1;
      }
  
      if(hcs_used == FALSE)
      {
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.hcs_info.hcs_prio = 0;
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.hcs_info.q_hcs = 110;
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
          gsm_cell.hcs_info.penalty_time = 0;
      }
      /* Get GSM CELL Info */
      /* Check if rrc_gsm_cellSelectionReselectionInfo_present */
      if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->m.cellSelectionReselectionInfoPresent)
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Cell Reselection info present");
  
        /* q-Offset1S-N */
     
          /* Range is -50...50 */
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.qoffset1_s_n = (int16)
            inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.q_Offset1S_N;
        
  
        /* q_Offset2S_N  -50...50 */
        if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectReselectInfo.m.q_Offset2S_NPresent)
        {
          /* Range is -50...50 */
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.qoffset2_s_n =  (int16)
            inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.q_Offset2S_N;
        }
  
        /* Max Txit Power */
        if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
          cellSelectionReselectionInfo.m.maxAllowedUL_TX_PowerPresent)
        {
          l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
            gsm_cell.cell_sel_info.max_tx_pwr = (int16)
            inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.maxAllowedUL_TX_Power;
        }
  
        /* No need to Check if gsm mode present as it has already been done */
  
        /* Check if rxlev_min present */
  
        if(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
           cellSelectionReselectionInfo.modeSpecificInfo.t == 
             T_tdsrrc_CellSelectReselectInfoSIB_11_12_modeSpecificInfo_gsm)
        {
          if (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.modeSpecificInfo.u
            .gsm->m.q_RxlevMinPresent)
          {
            /* Store rxlev_min in L1 */
            l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min = (int16)
              ((inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.modeSpecificInfo.u.gsm->q_RxlevMin * 2) + 1);
          }
        }
        if(rel5_ext_ptr != NULL)
        {
          if(TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(rel5_ext_ptr,deltaQrxlevmin))
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB11bis: Delta-Qrxlev=%d, Qrxlev=%d, ARFCN=%d",
              (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2),
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min,
              CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn));
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.rxlev_min +=
                (rel5_ext_ptr->elem[idx].deltaQrxlevmin * 2);
          }
        }
        if(hcs_used == TRUE)
        {
          /* HCS inforamtion */
          if(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
            cellSelectionReselectionInfo.m.hcs_NeighbouringCellInformation_RSCPPresent)
          {
            /* HCS PRIO*/
    
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                gsm_cell.hcs_info.hcs_prio = (uint16)
                inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_PRIO;
            
            
            /*Q HCS */
          
              if(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.q_HCS < 90)
              {
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                  gsm_cell.hcs_info.q_hcs = (uint16) (110 -
                  inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.q_HCS);
              }
              else
              {
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.
                  gsm_cell.hcs_info.q_hcs = 37;
              }
            
            /* Delta Qhcs is not needed if it is not mapped from CPICH-RSCP or PCCPCH-RSCP.
            * For inter rat cells, Qhcs is always mapped from RSSI
            */
            
            /* PENALTY TIME*/
            if(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
              cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
              .penaltyTime.t != T_tdsrrc_PenaltyTime_RSCP_notUsed)
            {
              
              switch(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                .penaltyTime.t)
              {
              case T_tdsrrc_PenaltyTime_RSCP_pt10:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 10;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt10);
                break;
                
              case T_tdsrrc_PenaltyTime_RSCP_pt20:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 20;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt20);
                break;
                
              case T_tdsrrc_PenaltyTime_RSCP_pt30:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 30;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt30);
                break;
                
              case T_tdsrrc_PenaltyTime_RSCP_pt40:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 40;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt40);
                break;
                
              case T_tdsrrc_PenaltyTime_RSCP_pt50:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 50;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt50);
                break;
                
              case T_tdsrrc_PenaltyTime_RSCP_pt60:
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.penalty_time = 60;
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.hcs_info.tmp_offset1 = (uint16)
                  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->
                  cellSelectionReselectionInfo.hcs_NeighbouringCellInformation_RSCP.hcs_CellReselectInformation
                  .penaltyTime.u.pt60);
                break;
                
              default:  /* should not happen */
                break;
              }
            }
          }
        }
      } /* rrc_gsm_cellSelectionReselectionInfo_present */
  
      /* Cell Individual Offset */
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.cio = (int16)
        inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->interRATCellIndividualOffset;
  
      /* Get BAND 1800 DCS or 1900 PCS */
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator =
        TDSRET_INTER_RAT_BAND(inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->frequency_band);
  
      /* Get bcch_ARFCN */
      CONVERT_ARFCN_TO_NUM(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn) =
        (uint16) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->bcch_ARFCN;
      if(hcs_used)
      {
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.max_tx_pwr = 
          (int16)rr_get_maximum_power_output_for_frequency(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB11bis: MAX_TX_POWR = %d",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.cell_sel_info.max_tx_pwr);
      }

      arfcn = (uint16) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bcch_ARFCN;

#ifdef FEATURE_GSM_QUADBAND
  
/* for SGLTE, don't need to know band enable/disable at G, just do band convertion */
#ifdef FEATURE_SGLTE
      if (tdsrrc_get_ue_mode()==TDSL1_MEAS_MODE_SGLTE)
      {
        geran_grr_band_ind_e grr_band_ind = (l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.band_indicator == TDSL1_DCS_1800_BAND) ?
				                                 GERAN_GRR_BAND_IND_DCS_1800 : GERAN_GRR_BAND_IND_PCS_1900;
		ARFCN_T converted_arfcn = geran_grr_convert_to_arfcn_type(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num, grr_band_ind);
		tdsconverted_gsm_band = converted_arfcn.band;
		TDSRRC_MSG2(MSG_LEGACY_HIGH,"SGLTE: arfcn %d,converted_gsm_band %d",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.num,tdsconverted_gsm_band);
      }
	  else
#endif /* FEATURE_SGLTE */
      {
        tdsconverted_gsm_band = tdsrrcirat_get_gsm_band(l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info);
      }

      if (tdsconverted_gsm_band != INVALID_BAND)
      {
        l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bcch_arfcn.band =
          tdsconverted_gsm_band;
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"ARFCN %d ignored bec of invalid band",inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
          u.gsm->bcch_ARFCN,0,0);
        arfcn = TDSMAX_ARFCN;
        if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
        {
          cell_position = (uint16)
            inter_rat_cell_list_ptr->elem[idx].interRATCellID;
        }
        else
        {
          cell_position = TDSAVAILABLE_CELL_POSITION;
        }
        /* Tell to get the first available cell position and
  
        * that would be the cell id. The called function would store
        * the cell id back in passed cell_position variable */
        if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter-RAT Cell List invalid");
          return FAILURE;
        }
        idx++;
        continue;
      }
  
#endif /* FEATURE_GSM_QUADBAND */
  
      /* To add BSIC (Base Station Identification Code) */
      if ((inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc
        > TDSNCC_MAX) ||
        (inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc
        > TDSBCC_MAX))
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"SIB11bis: BCC %d or NCC %d invalid[0..7]",inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.bcc,inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.u.gsm->bsic.ncc);
  
        return FAILURE;
  
      } /* End of if of checking NCC and BCC validity */
  
      /* Store good values of bcc and ncc */
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.ncc =
        (uint8) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->bsic.ncc;
  
      l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].u.gsm_cell.gsm_info.bsic.bcc =
        (uint8) inter_rat_cell_list_ptr->elem[idx].technologySpecificInfo.
        u.gsm->bsic.bcc;
    } /* End of gsm cell  */
  
    /* Time to get its cell id*/
    /* Cell Order has already been verified */
  
    if (TDSRRC_MSG_LIST_BITMASK_IE_TYPE2(inter_rat_cell_list_ptr,interRATCellID))
    {
      cell_position = (uint16)
        inter_rat_cell_list_ptr->elem[idx].interRATCellID;
    }
    else
    {
      cell_position = TDSAVAILABLE_CELL_POSITION;
    }
    /* Tell to get the first available cell position and
  
    * that would be the cell id. The called function would store
    * the cell id back in passed cell_position variable */
    if (tdsrrcirat_update_inter_rat_cell_list(&cell_position,arfcn) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter-RAT Cell List invalid");
      return FAILURE;
    }
    l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id
      = cell_position;
  
    /* set the measure in PCH IDLE and FACH flag to TRUE */
    l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach = TRUE;
    /* Increment cell count */

    if(sib18_present)
    {
      if((tmp_rrc_PLMNsOfInterRATCellsList_ptr != NULL) && (tmp_rrc_PLMNsOfInterRATCellsList_ptr->n > cell_count))
      {
        if(tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[cell_count].m.plmn_IdentityPresent)
        {
          rrc_translate_gsm_map_plmn_id(&tmp_rrc_PLMNsOfInterRATCellsList_ptr->elem[cell_count].plmn_Identity,
            &tmp_plmn_identity);
          if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
          {
            if((tdsrrccsp_check_for_eplmn(
              mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
              tmp_plmn_identity))) == FALSE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"cell id %d Non eq PLMN measure only in DCH",l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].intersys_cell_id);
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach = FALSE;
            }
          }
        }
        else
        {
          /* use the previous value of the flag if PLMN id is not selected PLMN */
          if(tdsrrc_bcch_and_nas_plmns_match(tmp_plmn_identity, selected_plmn_identity) == FALSE)
          {
            /* Add code to set the flag meas_pch_fach_idle to the flag value of the previous cell */
            if(cell_count > 0)
            {
              l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count].measure_in_idle_pch_fach =
                l1_inter_rat_sib_ptr->inter_s.add_cell[cell_count - 1].measure_in_idle_pch_fach;
            }
          }
        }
      }
      else
      {
        /* PLMN list is shorter than the cell info list set flag to true for the remaining cells */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN list < Cell list");
      }
    }

    cell_count++;
    /* Increment the pointer */
    idx++;
    if ((inter_rat_cell_list_ptr->n > idx) && (cell_count == TDSL1_MAX_CELL_MEAS))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter-RAT Cell List full");
      break;
    }
  }
  l1_inter_rat_sib_ptr->inter_s.num_cell_add = cell_count;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB11bis: Number of Inter RAT cells added from SIB11 and SIB11bis is %d",l1_inter_rat_sib_ptr->inter_s.num_cell_add);
  if(cell_count == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB11bis: Inter RAT,number of cells added is zero");
    l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = FALSE;
    return FAILURE;
  }
  l1_inter_rat_sib_ptr->inter_s.num_cell_add = cell_count;
  l1_inter_rat_sib_ptr->inter_sys_meas_info_incl = TRUE;
  l1_inter_rat_sib_ptr->hcs_used = hcs_used;
  return SUCCESS;
}

#endif /*FEATURE_TDSCDMA_SIB11BIS_SUPPORT*/


/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R7

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_r7
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the INTER-RAT Measurement Control Message */
  uint8 count = 0, cnt = 0;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_rat_meas_params;

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_SYS;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r7_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  tdsrrc_InterRATMeasurement_r6 *irat_meas_cmd_ptr = NULL;

  boolean bsic_verify = FALSE;

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.r7.measurementControl_r7;
  
  /* Assign default values to inter_rat_meas_params */
  memset(&l1_inter_rat_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_rat_meas_params.meas_object.meas_type = TDSL1_INTER_SYS;

  /* Set Transaction Id and Meas Id */
  l1_inter_rat_meas_params.trans_id = tdstransaction_id;
  l1_inter_rat_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;


  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r7_setup))
  {
    /* Double check that it is INTER-RAT Meas */
    if (!(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(rrc_mcm_ptr->measurementCommand.u.setup,
        rrc_MeasurementType_r7_interRATMeasurement)))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    /* Check if meas quantity,reporting quantity & Reporting criteria is received.
    If either of them is not received, then configuration incomplete ERROR */

    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r6,interRATMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r6,interRATReportingQuantity)) &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.t == T_tdsrrc_InterRATMeasQuantity_ratSpecificInfo_gsm) &&
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity !=
      tdsrrc_MeasurementQuantityGSM_dummy) &&
      (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATReportingQuantity.ratSpecificInfo,
      rrc_InterRATReportingQuantity_ratSpecificInfo_gsm)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r6_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/ReportCriteria/R.Mode");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria,
      rrc_InterRATReportCriteria,interRATReportingCriteria)) &&
      (!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria,interRATEventList)) ||
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria->interRATEventList.n == 0)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid,event criteria list absent ");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement ;
    if((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
       (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
        T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Quality Estimate is FDD,which is not supported");
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

    /* Set Meas Identity */
    l1_inter_rat_meas_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;

    /* Set Meas Quantity as Inter-sys Meas Quan present */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;

    /* Initialize Default Value to intra_freq_meas_quan_incl */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl =  FALSE;

    /* Initialize Filter Coefficient to Default Value i.e. 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;

    /* Initialize GSM Filter Coefficient to 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
    TDSL1_GSM_FILTER_COEF_0;

    /* Meas Quantity Parameter 1 i.e. Quality Estimate */
    if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity,tdsrrc_InterRATMeasQuantity,measQuantityUTRAN_QualityEstimate))
    {
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;

      /* Meas Quan i.e ISCP/RSCP/RSSI... */
      /* 10.3.7.38  intra meas quantity type is a list */
      tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.u.inter_s.
                    meas_quan.intra_meas_quan,
                 &rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
                 interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->
                 intraFreqMeasQuantity_TDDList);
      /* Get Filter Coefficient too */

      /* Get Filter Coefficient too */
      /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient_present */
      
        /* Store filter coefficient */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
          TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.setup->u.
          interRATMeasurement->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.filterCoefficient);
      
    } /* End of quality estimate present */

    /* Set System Choice to GSM */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;

    /* Get GSM Meas Quan, Filter Coefficient, BSIC required */

    /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
    So set it to RSSI */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
      TDSL1_GSM_CARRIER_RSSI;

      /* GSM Filter Coefficient present */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
        TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.
        u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo.u.gsm->filterCoefficient);

#ifdef FEATURE_ENH_BSIC_VERIFY
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
    bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#else
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
      TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#endif

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"R7 MCM gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);

    /* Store Reporting Quantity */
    l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;

    /* Sys Choice */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;

    /* L1 wants this to be FALSE for the first version as it does not support it */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;

    /* Ignore reading Observed time difference as no longer supported in Rel 99
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
    rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
    ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */

    /* Check if gsm_carrier_rssi wanted in Meas Report */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
      rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
      ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;

    if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.setup->
      u.interRATMeasurement->reportCriteria, &l1_inter_rat_meas_params.meas_object.u.inter_s,
      &mcm_rpt_crit, tdstransaction_id))
    {
      return FAILURE;
    }

    /* Store reporting mode to TRUE */
    l1_inter_rat_meas_params.rpt_mode_incl = TRUE;

    l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

    /* For L1 sake, copy reporting criteria */
    l1_inter_rat_meas_params.reporting_mode.rpt_mode =
      l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;

    /* Now set other values */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

    l1_inter_rat_meas_params.meas_object_info_included = TRUE;


    /* Time to process the cell list */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r6,interRATCellInfoList))
    {
      if (tdsrrcirat_process_cell_list_mcm_r6(&rrc_mcm_ptr->measurementCommand.
        u.setup->u.interRATMeasurement->interRATCellInfoList,
        &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list) == FAILURE)
      {
        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
       rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      /* Set Cell list is present */
      l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
    }

    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

      /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
        if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
          addtl_meas_incl, TRUE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
          &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      }
    } /* End of if of Additional Meas Present */

    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r6,interRATCellInfoList))
    {
      /* At this point everything has been validated and now is the time
      to update the inter rat CELL_INFO_LIST */

      for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
      {
        memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		&tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
      }
    }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        if(tdsinter_rat_meas_id_list[count].interrat_type != TDSRRCMEAS_IRAT_GSM)
        {
          tdsrrcirat_delete_mcm_due_to_irat_type_change(l1_inter_rat_meas_params.meas_id);
        }
        break;
      }
    }
#endif

    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
        tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
        meas_exists_for_setup = TRUE;
        break;
      }
    }

    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
      {
        /* First check if this meas already exists */
        if (tdsinter_rat_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
          tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
          break;
        }
      }

    } /* End of if */

    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }

    /* Now one needs to send command to L1 */

  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r7_modify)
  {
    /* Initialize Filter Coefficient to Default Value i.e. 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;

    /* Initialize GSM Filter Coefficient to 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
    TDSL1_GSM_FILTER_COEF_0;

    /* Check if reporting Mode present */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,measurementReportingMode))
    {
      /* Store reporting mode to TRUE */
      l1_inter_rat_meas_params.rpt_mode_incl = TRUE;

      l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
    }

    /* Set type to MODIFY */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r7_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:I.RAT Modify: MType absent");

      /* Check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
      {
        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }
    }
    else
    {
      if(!(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType,
          rrc_MeasurementType_r7_interRATMeasurement)))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:Meas Id is not Inter-RAT");
        /* Send Meas Ctrl Failure  */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
     
      irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement;
      if(
          ((irat_meas_cmd_ptr->m.interRATMeasQuantityPresent) &&
            ((irat_meas_cmd_ptr->interRATMeasQuantity.ratSpecificInfo.t != T_tdsrrc_InterRATMeasQuantity_ratSpecificInfo_gsm) ||
              ((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
                (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
                 T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))))
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration not supported");
        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        /* Set faliure choice */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }

      if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
        interRATMeasurement,tdsrrc_InterRATMeasurement_r6,interRATMeasQuantity)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Meas Quan absent");
      }
      else
      {
        /* Set Meas Quantity to TRUE */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = FALSE;
        if (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity ==
          tdsrrc_MeasurementQuantityGSM_dummy)
        {

          TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCM:MeasQuantity PathLoss");

          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

          /* Set Error Cause */
          rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }

        /* Meas Quantity Parameter 1 i.e. Quality Estimate */
        if(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
           interRATMeasQuantity,tdsrrc_InterRATMeasQuantity,measQuantityUTRAN_QualityEstimate))
        {
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;

          /* Meas Quan i.e ISCP/RSCP/RSSI... */
          /* 10.3.7.38  intra meas quantity type is a list */
          tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.
                                              u.inter_s.meas_quan.intra_meas_quan,
                     &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
                     interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->
                     intraFreqMeasQuantity_TDDList);
          /* Get Filter Coefficient too */

          /* Get Filter Coefficient too */
          /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient_present */
          
            /* Store filter coefficient */
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
              TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u
              .modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
              measQuantityUTRAN_QualityEstimate.filterCoefficient);
           
        }

        /* Set System Choice to GSM */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;

        /* Get GSM Meas Quan, Filter Coefficient, BSIC required */

        /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
        So set it to RSSI */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
          TDSL1_GSM_CARRIER_RSSI;

       
          /* GSM Filter Coefficient present */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
            TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.
            modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
            ratSpecificInfo.u.gsm->filterCoefficient);
       
#ifdef FEATURE_ENH_BSIC_VERIFY
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
        bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
            modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#else
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
          TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
            modify->measurementType.u.interRATMeasurement->
          interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#endif

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"R7 MCM Modify gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);
      } /* End of else of Meas Quan present */

      /* Check if reporting quan present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r6,interRATReportingQuantity))
      {
        /* Store Reporting Quantity */
        l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;

        /* Sys Choice */
        l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;

        /* L1 wants this to be FALSE for the first version as it does not support it */
        l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;

        /* Observed time difference no longer supported in Rel 99
        l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
          rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATReportingQuantity.ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */

          /* Check if gsm_carrier_rssi wanted in Meas Report */
          l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
          rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
          interRATReportingQuantity.ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;
      }

      if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.modify->
        measurementType.u.interRATMeasurement->reportCriteria,
        &l1_inter_rat_meas_params.meas_object.u.inter_s, &mcm_rpt_crit, tdstransaction_id))
      {
        return FAILURE;
      }
      l1_inter_rat_meas_params.reporting_mode.rpt_mode =
      l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;
      
      l1_inter_rat_meas_params.meas_object_info_included = TRUE;
      if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r7_modify,measurementType))
        && (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r6,interRATCellInfoList)) )
      {
        /* Time to process the cell list */
        if (tdsrrcirat_process_cell_list_mcm_r6(&rrc_mcm_ptr->measurementCommand.
          u.modify->measurementType.u.interRATMeasurement->interRATCellInfoList,
          &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list)
          == FAILURE)
        {
          /* Send Meas Ctrl Failure Msg */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;

          /* Set Error Cause */
          rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }

        /* Set Cell list is present */
        l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
      }

      /* Check for additional measurements */
      /* Call fn which makes check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
      {
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type,&rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }


      /* Set reporting criteria in RRC DB */
      for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
      {
        if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
        {
          if(mcm_rpt_crit != TDSNOT_AVAILABLE)
          {      
            tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
            tdsinter_rat_meas_id_list[count].interrat_type = TDSRRCMEAS_IRAT_GSM;
            tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reportinf Crit set in RRC DB");
          }
          break;
        }
      }
      
      if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        & tdsrrc_MeasurementCommand_r7_modify,measurementType))
        && (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
        & tdsrrc_InterRATMeasurement_r6,interRATCellInfoList) ))
      {
        /* At this point everything has been validated and now is the time
        to update the inter rat CELL_INFO_LIST */
        for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		  &tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
        }
      }

    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
  * that this meas exists. So let us delete it from tdsinter_rat_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_rat_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_rat_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;

        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = FALSE;

        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsinter_rat_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */


  if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r7_setup))
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
       tdsrrc_InterRATMeasurement_r6,interRATCellInfoList)))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATCellInfoList,
          tdsrrc_InterRATCellInfoList_r6,interRATCellInfoIndication_r6))
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind = rrc_mcm_ptr->measurementCommand.
                                 u.setup->u.interRATMeasurement->interRATCellInfoList.interRATCellInfoIndication_r6;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }

  if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r7_modify))
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
         tdsrrc_MeasurementCommand_r7_modify,measurementType)) &&
       (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
         tdsrrc_InterRATMeasurement_r6,interRATCellInfoList)))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATCellInfoList,
                   tdsrrc_InterRATCellInfoList_r6,interRATCellInfoIndication_r6))
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind = rrc_mcm_ptr->measurementCommand.
           u.modify->measurementType.u.interRATMeasurement->interRATCellInfoList.interRATCellInfoIndication_r6;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }

  if(!tdsprocess_interfreq_interrat_meas_rel7)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending interRAT measurements from MCM to L1");
    return SUCCESS;
  }

  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_rat_meas_params.trans_id = alloc_idx;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Sending cmd to L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_rat_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);

  return SUCCESS;
}


#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================

FUNCTION   tdsrrcirat_initialize_eutra_freq_list

DESCRIPTION 
  This function initializes EUTRA frequency list stored

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcirat_initialize_eutra_freq_list
(
  void
)
{
  uint32 cnt;
  for(cnt = 0; cnt < TDSL1_MAX_EUTRA_FREQ;cnt++)
  {
    tdsrrcirat_eutra_freq_list_ptr[cnt].earfcn = RRC_INVALID_EUTRA_FREQ;
  }
}

/*===========================================================================

FUNCTION   tdsrrcirat_update_eutra_freq_list

DESCRIPTION 
  This function updates stored EUTRA frequency with EUTRA freq received in the argument

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS : If EUTRA frequency is updated in the DB
  FAILURE   : Otherwise

SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcirat_update_eutra_freq_list
(
  uint32 earfcn
)
{
  uint32 cnt;
  for(cnt = 0; cnt < TDSL1_MAX_EUTRA_FREQ;cnt++)
  {
    if((tdsrrcirat_eutra_freq_list_ptr[cnt].earfcn == RRC_INVALID_EUTRA_FREQ) ||
       (earfcn == tdsrrcirat_eutra_freq_list_ptr[cnt].earfcn))
    {
      break;
    }
  }
  if(cnt != TDSL1_MAX_EUTRA_FREQ)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSTOL: Updated Freq %d @ idx %d",earfcn,cnt);
    tdsrrcirat_eutra_freq_list_ptr[cnt].earfcn = earfcn;
    return SUCCESS;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't store earfcn %d,MAX frequencies reached",earfcn);
    return FAILURE;
  }
}
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
/*===========================================================================

FUNCTION   tdsrrcirat_remove_eutra_freq

DESCRIPTION 
  This function removes EUTRA freq from Stored DB

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS : If EUTRA frequency is removed from the DB
  FAILURE   : Otherwise

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcirat_remove_eutra_freq
(
  uint32 earfcn
)
{
  uint32 cnt;
  for(cnt=0;cnt<TDSL1_MAX_EUTRA_FREQ;cnt++)
  {
    if(earfcn == tdsrrcirat_eutra_freq_list_ptr[cnt].earfcn)
    {
      tdsrrcirat_eutra_freq_list_ptr[cnt].earfcn = RRC_INVALID_EUTRA_FREQ;
      break;
    }
  }
  if(TDSL1_MAX_EUTRA_FREQ != cnt)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Removed freq %d from idx ",earfcn,cnt);
    return SUCCESS;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Freq %d not present in the list",earfcn);
    return FAILURE;
  }
}

/*===========================================================================

FUNCTION   tdsrrcirat_process_eutra_freq_list_mcm

DESCRIPTION 
  This function processes EUTRA freq list from MCM

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS : If EUTRA frequency list is successfully processed
  FAILURE   : Otherwise

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcirat_process_eutra_freq_list_mcm
(
  tdsrrc_EUTRA_FrequencyList *eutra_freq_list,
  tdsl1_inter_sys_eutra_freq_list_struct_type *l1_freq_list
)
{
  struct tdsrrc_EUTRA_FrequencyRemoval_removeSomeFrequencies *rmv_freq_ptr = NULL;
  uint8 freq_count =0;
  uint32 earfcn;
  memset(l1_freq_list,0,sizeof(tdsl1_inter_sys_eutra_freq_list_struct_type));
  // for KW only
  if (eutra_freq_list == NULL) 
  {
    ERR_FATAL("eutra_freq_list is NULL!",0,0,0);
  }
  switch(TDSRRC_GET_MSG_TYPE(eutra_freq_list->eutraFrequencyRemoval))
  {
    case T_tdsrrc_EUTRA_FrequencyRemoval_removeAllFrequencies:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Remove all freqs");
      l1_freq_list->num_freq_rmv = TDSL1_REMOVE_ALL_EUTRA_FREQ;
      tdsrrcirat_initialize_eutra_freq_list();
      break;
    case T_tdsrrc_EUTRA_FrequencyRemoval_removeNoFrequencies:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Remove None");
      l1_freq_list->num_freq_rmv = 0;
      break;
    case T_tdsrrc_EUTRA_FrequencyRemoval_removeSomeFrequencies:
      rmv_freq_ptr = eutra_freq_list->eutraFrequencyRemoval.u.removeSomeFrequencies;
      freq_count = 0;
      if(rmv_freq_ptr != NULL)
      {
        uint8 idx = 0;
        do
        {
          earfcn = (uint32)rmv_freq_ptr->elem[idx];
          if(FAILURE == tdsrrcirat_remove_eutra_freq(earfcn))
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"EARFCN %d not found",earfcn);
            idx++;
            continue;
          }
          l1_freq_list->rmv_freq[freq_count] = earfcn;
          freq_count++;
          idx++;
        } while((freq_count < TDSL1_MAX_EUTRA_FREQ) && (rmv_freq_ptr->n > idx));
      }
      l1_freq_list->num_freq_rmv = freq_count;
      break;
    default :
      /*shouldn't happen*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Choice");
      break;
  }
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(eutra_freq_list,eutraNewFrequencies))
  {
    tdsrrc_EUTRA_FrequencyInfoList *freq_list_ptr = NULL;
    uint8 blc_idx=0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"New Frequency list present");
    freq_list_ptr = &eutra_freq_list->eutraNewFrequencies;
    freq_count = 0;
    if(freq_list_ptr != NULL)
    {
      uint8 idx = 0;
      do
      {
        earfcn = (uint32)freq_list_ptr->elem[idx].earfcn;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
        if(!lte_rrc_earfcn_is_supported(tdsrrc_get_as_id(),earfcn,tdsrrccsp_get_lte_band_pref()))
#else
        if(!lte_rrc_earfcn_is_supported(earfcn,tdsrrccsp_get_lte_band_pref()))
#endif
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"EARFCN %d not supported by UE",earfcn);
          idx++;
          continue;
        }
        l1_freq_list->add_freq_list.eutra_frequency[freq_count].EARFCN = earfcn;
        l1_freq_list->add_freq_list.eutra_frequency[freq_count].meas_band_width = TDSL1_MBW6;
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"EARFCN %d meas_band_width %d added to the list",earfcn,
                    l1_freq_list->add_freq_list.eutra_frequency[freq_count].meas_band_width);
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(freq_list_ptr->elem[idx],measurementBandwidth))
        {
          l1_freq_list->add_freq_list.eutra_frequency[freq_count].meas_band_width = 
               TDSRRCMEAS_GET_EUTRA_MEAS_BANDWIDTH(freq_list_ptr->elem[idx].measurementBandwidth);
        }
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(freq_list_ptr->elem[idx],eutra_blackListedCellList))
        {
          uint8 cell_count =0;
          struct tdsrrc_EUTRA_BlacklistedCellPerFreqList *cell_list_ptr = &freq_list_ptr->elem[idx].eutra_blackListedCellList;
          if(cell_list_ptr != NULL)
          {
            do
            {
              l1_freq_list->add_freq_list.eutra_frequency[freq_count].blacklisted_cells[cell_count] = 
                cell_list_ptr->elem[cell_count].physicalCellIdentity;
              cell_count++;
            }while((cell_count < TDSL1_MAX_EUTRA_CELLS_PER_FREQUENCY) && (cell_list_ptr->n > cell_count));
            l1_freq_list->add_freq_list.eutra_frequency[freq_count].num_blacklisted_cells = cell_count;
          }
        }
        if(FAILURE == tdsrrcirat_update_eutra_freq_list(earfcn))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"'EARFCN %d cannot be updated in freq list",earfcn);
          return FAILURE;
        }
        for(blc_idx =0;blc_idx< l1_freq_list->add_freq_list.eutra_frequency[freq_count].num_blacklisted_cells && 
          blc_idx < TDSL1_MAX_EUTRA_CELLS_PER_FREQUENCY;blc_idx++)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Blacklisted PCI %d ",l1_freq_list->add_freq_list.eutra_frequency[freq_count].blacklisted_cells[blc_idx]);
        }
        idx++;
        freq_count++;
      }while((freq_count < TDSL1_MAX_EUTRA_FREQ) && (freq_list_ptr->n > idx));
      l1_freq_list->add_freq_list.num_eutra_frequencies = freq_count;
    }
  }
  return SUCCESS;
}

/*===========================================================================

FUNCTION   tdsrrcirat_get_idleInterval_Info_r8

DESCRIPTION

  This function processes the IE "Idle interval for measurement"

DEPENDENCIES

  None.

RETURN VALUE


SIDE EFFECTS

  None

===========================================================================*/
static void tdsrrcirat_get_idleInterval_Info_r8( tdsrrc_InterRATMeasurement_r8* tdsrrc_irat,
                                         tdsl1_meas_ctrl_parm_struct_type* l1_irat_meas_params)
{
  if(TRUE == tdsrrc_irat->m.idleIntervalInfoPresent)
  {
    l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info_incl = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: idle_interval_info present");

    /* k: Integer (2..3), This IE is the coefficient parameter to calculate the idle interval period. */
    if(TRUE == tdsrrc_irat->idleIntervalInfo.m.kPresent)
    {
       l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.k = tdsrrc_irat->idleIntervalInfo.k;
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8: idle_interval_info.k = %d",(l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.k));
    }
    else
    {
    /* specification 25v331-10.3.7.12a , the coefficient parameter to calculate the idle interval period.
           Default value is 2. Since it is common for SRB to use TTI of 40ms, use k of 2 can cause UE not 
		   able to send or receive signaling with TTI 40ms. Per systems requirement, use default value 3.
		   The actual idle interval period equal to 2^k radio frames. */
       l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.k = 3;
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"RELL8: idle_interval_info.k = 3,default value");
    }

    /* Offset: The idle interval position in the period. */
    if(TRUE == tdsrrc_irat->idleIntervalInfo.m.offsetPresent)
    {
       l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.Offset = tdsrrc_irat->idleIntervalInfo.offset;
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"RELL8: idle_interval_info.Offset = %d",(l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.Offset));
    }
    else
    {
    /* specification 25v331-10.3.7.12a , The idle interval position in the period.Default value is 0. */
       l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.Offset = 0;
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"RELL8: idle_interval_info.Offset = 0,default value");
    }
  }
  else
  {
    l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info_incl = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: idle_interval_info is not present");
  }
}

/*===========================================================================

FUNCTION   tdsrrcirat_process_mcm_setup_modify_eutra

DESCRIPTION 
  This function processes setup or modify of EUTRA IRAT measurement IE from MCM

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS : If EUTRA measurement IE from MCM is successfully processed
  FAILURE   : Otherwise

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcirat_process_mcm_setup_modify_eutra
(
  tdsrrc_InterRATMeasurement_r8 *mcm_irat_ptr,
  tdsl1_meas_ctrl_parm_struct_type *l1_inter_rat_meas_params,
  uint32 transaction_id,
  tdsrpt_crit_enum_type *mcm_rpt_crit_ptr
)
{
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  tdsl1_inter_sys_meas_ctrl_struct_type temp_inter_s_meas_ctrl;

  memset(&temp_inter_s_meas_ctrl,0,sizeof(tdsl1_inter_sys_meas_ctrl_struct_type));

  l1_inter_rat_meas_params->meas_object_info_included = TRUE;
  l1_inter_rat_meas_params->meas_object.meas_type = TDSL1_INTER_SYS_EUTRA;

  /*Fill the measurement quantity*/
  if(!TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mcm_irat_ptr,interRATMeasQuantity))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Measurement Object Absent");
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_meas_quantity_incl = FALSE;
  }
  else
  {
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_meas_quantity_incl = TRUE;
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.system_choice = TDSL1_EUTRA_SYSTEM;

    /* Meas Quantity Parameter 1 i.e. Quality Estimate */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(mcm_irat_ptr->interRATMeasQuantity,
          measQuantityUTRAN_QualityEstimate)) &&
         (TDSRRC_CHECK_COMMON_MSG_TYPE(mcm_irat_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo,
         rrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd)))
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.intra_meas_quan_incl = TRUE;

      /* Meas Quan i.e ISCP/RSCP/RSSI... */
      /* 10.3.7.38  intra meas quantity type is a list */
      tdsrrcmeas_set_intra_meas_quan_info(
        &l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.intra_meas_quan,
        &mcm_irat_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->intraFreqMeasQuantity_TDDList);

      /* Store filter coefficient */
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.intra_meas_quan.filter_coef = (uint16)
        TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(mcm_irat_ptr->interRATMeasQuantity.
          measQuantityUTRAN_QualityEstimate.filterCoefficient);
    }

    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.filter_coef = TDSL1_EUTRA_FILTER_COEF_0;
/*    if(RRC_MSG_COMMON_BITMASK_IE(mcm_irat_ptr->interRATMeasQuantity.ratSpecificInfo.u.e_UTRA,))*/
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.filter_coef =
          TDSRET_EUTRA_FILTER_COEFFICIENT(mcm_irat_ptr->interRATMeasQuantity.ratSpecificInfo.u.e_UTRA->filterCoefficient);
    }
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.meas_quan =
      TDSRET_EUTRA_MEAS_QUAN(mcm_irat_ptr->interRATMeasQuantity.ratSpecificInfo.u.e_UTRA->measurementQuantity);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"EUTRA meas quan %d,filter_coeff %d",l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.meas_quan,l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.filter_coef);
  }

  /*Reporting Quantity*/
  if(!TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mcm_irat_ptr,interRATReportingQuantity))
  {
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_rpt_quantity_incl = FALSE;
  }
  else
  {
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_rpt_quantity_incl = TRUE;
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.utran_est_qual = FALSE;
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.sys_choice = TDSL1_EUTRA_SYSTEM;
  
    if(tdsrrc_InterRATReportingQuantity_r8_reportingQuantity_measurementQuantity == mcm_irat_ptr->interRATReportingQuantity.ratSpecificInfo.u.eutra->reportingQuantity)
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.u.eutra_rpt_quan = 
          l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.meas_quan;
    }
    else
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.u.eutra_rpt_quan = TDSL1_EUTRA_QUAN_BOTH;
    }
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"EUTRA reporting Quan %d",l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.u.eutra_rpt_quan);
  }

  /*Fill Reporting Criteria*/
  if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&mcm_irat_ptr->reportCriteria,&temp_inter_s_meas_ctrl,
    mcm_rpt_crit_ptr, transaction_id))
  {
    return FAILURE;
  }

  l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_cell_status_incl =
    temp_inter_s_meas_ctrl.rpt_cell_status_incl;
  memscpy(&l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_cell_status, sizeof(tdsl1_meas_rpt_cell_status_struct_type),
    &temp_inter_s_meas_ctrl.rpt_cell_status,sizeof(tdsl1_meas_rpt_cell_status_struct_type));

  memscpy(&l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_criteria, sizeof(tdsl1_inter_sys_reporting_criteria_struct_type),
    &temp_inter_s_meas_ctrl.rpt_criteria,sizeof(tdsl1_inter_sys_reporting_criteria_struct_type));

  l1_inter_rat_meas_params->reporting_mode.rpt_mode = 
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_criteria.rpt_mode;

  /*Measurement Objects*/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mcm_irat_ptr,interRATMeasurementObjects)) &&
    (TDSRRC_CHECK_COMMON_MSG_TYPE(mcm_irat_ptr->interRATMeasurementObjects,
      rrc_InterRATMeasurement_r8_interRATMeasurementObjects_eutra_FrequencyList)))
  {
    if(FAILURE == tdsrrcirat_process_eutra_freq_list_mcm(mcm_irat_ptr->interRATMeasurementObjects.u.eutra_FrequencyList,
        &l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list))
    {
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = transaction_id;
      /* Set error choice */
     rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

    /* RRC need to send MCF if all the EUTRAN freq from NW is invalid, rather than just skip them */
    if( (l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list.add_freq_list.num_eutra_frequencies > 0) ||
		(l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list.num_freq_rmv >0) )
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list_incl = TRUE;
    }
    /* if no valid eutra freq to add, reply MCF to NW */
	if ( (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mcm_irat_ptr->interRATMeasurementObjects.u.eutra_FrequencyList,eutraNewFrequencies)) &&
		 (l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list.add_freq_list.num_eutra_frequencies == 0) )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"num of valid EUTRA freq to add: %d, invalid config,send MCF",l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list.add_freq_list.num_eutra_frequencies);

      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = transaction_id;
      /* Set error choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }
  }

  tdsrrcirat_get_idleInterval_Info_r8(
                                    mcm_irat_ptr,
                                    l1_inter_rat_meas_params
                                    );

  return SUCCESS;
}

/*===========================================================================

FUNCTION   tdsrrcirat_process_eutra_freq_list_mcm_r9

DESCRIPTION 
  This function processes EUTRA freq list from MCM

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS : If EUTRA frequency list is successfully processed
  FAILURE   : Otherwise

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcirat_process_eutra_freq_list_mcm_r9
(
  tdsrrc_EUTRA_FrequencyList_r9 *eutra_freq_list,
  tdsl1_inter_sys_eutra_freq_list_struct_type *l1_freq_list
)
{
  struct tdsrrc_EUTRA_FrequencyRemoval_removeSomeFrequencies *rmv_freq_ptr = NULL;
  uint8 freq_count =0;
  uint32 earfcn;
  memset(l1_freq_list,0,sizeof(tdsl1_inter_sys_eutra_freq_list_struct_type));
  // for KW only
  if (eutra_freq_list == NULL) 
  {
    ERR_FATAL("eutra_freq_list is NULL!",0,0,0);
  }
  switch(TDSRRC_GET_MSG_TYPE(eutra_freq_list->eutraFrequencyRemoval))
  {
    case T_tdsrrc_EUTRA_FrequencyRemoval_removeAllFrequencies:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Remove all freqs");
      l1_freq_list->num_freq_rmv = TDSL1_REMOVE_ALL_EUTRA_FREQ;
      tdsrrcirat_initialize_eutra_freq_list();
      break;
    case T_tdsrrc_EUTRA_FrequencyRemoval_removeNoFrequencies:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Remove None");
      l1_freq_list->num_freq_rmv = 0;
      break;
    case T_tdsrrc_EUTRA_FrequencyRemoval_removeSomeFrequencies:
      rmv_freq_ptr = eutra_freq_list->eutraFrequencyRemoval.u.removeSomeFrequencies;
      freq_count = 0;
      if(rmv_freq_ptr != NULL)
      {
        uint8 idx = 0;
        do
        {
          earfcn = (uint32)rmv_freq_ptr->elem[idx];
          if(FAILURE == tdsrrcirat_remove_eutra_freq(earfcn))
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"EARFCN %d not found",earfcn);
            idx++;
            continue;
          }
          l1_freq_list->rmv_freq[freq_count] = earfcn;
          freq_count++;
          idx++;
        } while((freq_count < TDSL1_MAX_EUTRA_FREQ) && (rmv_freq_ptr->n > idx));
      }
      l1_freq_list->num_freq_rmv = freq_count;
      break;
    default :
      /*shouldn't happen*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid Choice");
      break;
  }

  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(eutra_freq_list,eutraNewFrequencies))
  {
    tdsrrc_EUTRA_FrequencyInfoList *freq_list_ptr = NULL;
    uint8 blc_idx=0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"New Frequency list present");
    freq_list_ptr = &eutra_freq_list->eutraNewFrequencies;
    freq_count = 0;
    if(freq_list_ptr != NULL)
    {
      uint8 idx = 0;
      do
      {
        earfcn = (uint32)freq_list_ptr->elem[idx].earfcn;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
        if(!lte_rrc_earfcn_is_supported(tdsrrc_get_as_id(), earfcn,tdsrrccsp_get_lte_band_pref()))
#else
        if(!lte_rrc_earfcn_is_supported(earfcn,tdsrrccsp_get_lte_band_pref()))
#endif
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"EARFCN %d not supported by UE",earfcn);
          idx++;
          continue;
        }
        l1_freq_list->add_freq_list.eutra_frequency[freq_count].EARFCN = earfcn;
        l1_freq_list->add_freq_list.eutra_frequency[freq_count].meas_band_width = TDSL1_MBW6;
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"EARFCN %d meas_band_width %d added to the list",earfcn,
                    l1_freq_list->add_freq_list.eutra_frequency[freq_count].meas_band_width);
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(freq_list_ptr->elem[idx],measurementBandwidth))
        {
          l1_freq_list->add_freq_list.eutra_frequency[freq_count].meas_band_width = 
               TDSRRCMEAS_GET_EUTRA_MEAS_BANDWIDTH(freq_list_ptr->elem[idx].measurementBandwidth);
        }
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(freq_list_ptr->elem[idx],eutra_blackListedCellList))
        {
          uint8 cell_count =0;
          struct tdsrrc_EUTRA_BlacklistedCellPerFreqList *cell_list_ptr = &freq_list_ptr->elem[idx].eutra_blackListedCellList;
          if(cell_list_ptr != NULL)
          {
            do
            {
              l1_freq_list->add_freq_list.eutra_frequency[freq_count].blacklisted_cells[cell_count] = 
                cell_list_ptr->elem[cell_count].physicalCellIdentity;
              cell_count++;
            }while((cell_count < TDSL1_MAX_EUTRA_CELLS_PER_FREQUENCY) && (cell_list_ptr->n > cell_count));
            l1_freq_list->add_freq_list.eutra_frequency[freq_count].num_blacklisted_cells = cell_count;
          }
        }
        if(FAILURE == tdsrrcirat_update_eutra_freq_list(earfcn))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"'EARFCN %d cannot be updated in freq list",earfcn);
          return FAILURE;
        }
        for(blc_idx =0;blc_idx< l1_freq_list->add_freq_list.eutra_frequency[freq_count].num_blacklisted_cells && 
          blc_idx < TDSL1_MAX_EUTRA_CELLS_PER_FREQUENCY;blc_idx++)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Blacklisted PCI %d ",l1_freq_list->add_freq_list.eutra_frequency[freq_count].blacklisted_cells[blc_idx]);
        }
        idx++;
        freq_count++;
      }while((freq_count < TDSL1_MAX_EUTRA_FREQ) && (freq_list_ptr->n > idx));
      l1_freq_list->add_freq_list.num_eutra_frequencies = freq_count;
    }
  }

/* eutra_freq_list->eutraSIAcquisition is FDD IE, unsupport */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(eutra_freq_list,eutraSIAcquisition))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"'eutra SIAcquisition is FDD,should not present");
    return FAILURE;
  }

  return SUCCESS;
}

/*===========================================================================

FUNCTION   tdsrrcirat_get_idleInterval_Info_r9

DESCRIPTION

  This function processes the IE "Idle interval for measurement"

DEPENDENCIES

  None.

RETURN VALUE


SIDE EFFECTS

  None

===========================================================================*/
static void tdsrrcirat_get_idleInterval_Info_r9( tdsrrc_InterRATMeasurement_r9* tdsrrc_irat,
                                         tdsl1_meas_ctrl_parm_struct_type* l1_irat_meas_params)
{
  if(TRUE == tdsrrc_irat->m.idleIntervalInfoPresent)
  {
    l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info_incl = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: idle_interval_info present");

    /* k: Integer (2..3), This IE is the coefficient parameter to calculate the idle interval period. */
    if(TRUE == tdsrrc_irat->idleIntervalInfo.m.kPresent)
    {
       l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.k = tdsrrc_irat->idleIntervalInfo.k;
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9: idle_interval_info.k = %d",(l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.k));
    }
    else
    {
    /* specification 25v331-10.3.7.12a , the coefficient parameter to calculate the idle interval period.
           Default value is 2. Since it is common for SRB to use TTI of 40ms, use k of 2 can cause UE not 
		   able to send or receive signaling with TTI 40ms. Per systems requirement, use default value 3.
		   The actual idle interval period equal to 2^k radio frames. */
       l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.k = 3;
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"RELL9: idle_interval_info.k = 3,default value");
    }

    /* Offset: The idle interval position in the period. */
    if(TRUE == tdsrrc_irat->idleIntervalInfo.m.offsetPresent)
    {
       l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.Offset = tdsrrc_irat->idleIntervalInfo.offset;
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"RELL9: idle_interval_info.Offset = %d",(l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.Offset));
    }
    else
    {
    /* specification 25v331-10.3.7.12a , The idle interval position in the period.Default value is 0. */
       l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info.Offset = 0;
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"RELL9: idle_interval_info.Offset = 0,default value");
    }
  }
  else
  {
    l1_irat_meas_params->meas_object.u.inter_s_eutra.idle_interval_info_incl = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: idle_interval_info is not present");
  }
}

/*===========================================================================

FUNCTION   tdsrrcirat_process_mcm_setup_modify_eutra_R9

DESCRIPTION 
  This function processes setup or modify of EUTRA IRAT measurement IE from MCM

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS : If EUTRA measurement IE from MCM is successfully processed
  FAILURE   : Otherwise

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcirat_process_mcm_setup_modify_eutra_r9
(
  tdsrrc_InterRATMeasurement_r9 *mcm_irat_ptr,
  tdsl1_meas_ctrl_parm_struct_type *l1_inter_rat_meas_params,
  uint32 transaction_id,
  tdsrpt_crit_enum_type *mcm_rpt_crit_ptr
)
{
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;
  tdsl1_inter_sys_meas_ctrl_struct_type temp_inter_s_meas_ctrl;

  memset(&temp_inter_s_meas_ctrl,0,sizeof(tdsl1_inter_sys_meas_ctrl_struct_type));

  l1_inter_rat_meas_params->meas_object_info_included = TRUE;
  l1_inter_rat_meas_params->meas_object.meas_type = TDSL1_INTER_SYS_EUTRA;

  /*Fill the measurement quantity*/
  if(!TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mcm_irat_ptr,interRATMeasQuantity))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Measurement Object Absent");
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_meas_quantity_incl = FALSE;
  }
  else
  {
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_meas_quantity_incl = TRUE;
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.system_choice = TDSL1_EUTRA_SYSTEM;

    /* Meas Quantity Parameter 1 i.e. Quality Estimate */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(mcm_irat_ptr->interRATMeasQuantity,
          measQuantityUTRAN_QualityEstimate)) &&
         (TDSRRC_CHECK_COMMON_MSG_TYPE(mcm_irat_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo,
         rrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd)))
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.intra_meas_quan_incl = TRUE;

      /* Meas Quan i.e ISCP/RSCP/RSSI... */
      /* 10.3.7.38  intra meas quantity type is a list */
      tdsrrcmeas_set_intra_meas_quan_info(
        &l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.intra_meas_quan,
        &mcm_irat_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->intraFreqMeasQuantity_TDDList);

      /* Store filter coefficient */
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.intra_meas_quan.filter_coef = (uint16)
        TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(mcm_irat_ptr->interRATMeasQuantity.
          measQuantityUTRAN_QualityEstimate.filterCoefficient);
    }

    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.filter_coef = TDSL1_EUTRA_FILTER_COEF_0;
/*    if(RRC_MSG_COMMON_BITMASK_IE(mcm_irat_ptr->interRATMeasQuantity.ratSpecificInfo.u.e_UTRA,))*/
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.filter_coef =
          TDSRET_EUTRA_FILTER_COEFFICIENT(mcm_irat_ptr->interRATMeasQuantity.ratSpecificInfo.u.e_UTRA->filterCoefficient);
    }
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.meas_quan =
      TDSRET_EUTRA_MEAS_QUAN(mcm_irat_ptr->interRATMeasQuantity.ratSpecificInfo.u.e_UTRA->measurementQuantity);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"EUTRA meas quan %d,filter_coeff %d",l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.meas_quan,l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.filter_coef);
  }

  /*Reporting Quantity*/
  if(!TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mcm_irat_ptr,interRATReportingQuantity))
  {
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_rpt_quantity_incl = FALSE;
  }
  else
  {
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_rpt_quantity_incl = TRUE;
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.utran_est_qual = FALSE;
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.sys_choice = TDSL1_EUTRA_SYSTEM;
  
    if(tdsrrc_InterRATReportingQuantity_r8_reportingQuantity_measurementQuantity == mcm_irat_ptr->interRATReportingQuantity.ratSpecificInfo.u.eutra->reportingQuantity)
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.u.eutra_rpt_quan = 
          l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.meas_quan.u.eutra_meas_quan.meas_quan;
    }
    else
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.u.eutra_rpt_quan = TDSL1_EUTRA_QUAN_BOTH;
    }
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"EUTRA reporting Quan %d",l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_quan.u.eutra_rpt_quan);
  }

  /*Fill Reporting Criteria*/
  if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&mcm_irat_ptr->reportCriteria,&temp_inter_s_meas_ctrl,
    mcm_rpt_crit_ptr, transaction_id))
  {
    return FAILURE;
  }

  l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_cell_status_incl =
    temp_inter_s_meas_ctrl.rpt_cell_status_incl;
  memscpy(&l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_cell_status, sizeof(tdsl1_meas_rpt_cell_status_struct_type),
    &temp_inter_s_meas_ctrl.rpt_cell_status,sizeof(tdsl1_meas_rpt_cell_status_struct_type));

  memscpy(&l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_criteria, sizeof(tdsl1_inter_sys_reporting_criteria_struct_type),
    &temp_inter_s_meas_ctrl.rpt_criteria,sizeof(tdsl1_inter_sys_reporting_criteria_struct_type));

  l1_inter_rat_meas_params->reporting_mode.rpt_mode = 
    l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.rpt_criteria.rpt_mode;

  /*Measurement Objects*/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mcm_irat_ptr,interRATMeasurementObjects)) &&
    (TDSRRC_CHECK_COMMON_MSG_TYPE(mcm_irat_ptr->interRATMeasurementObjects,
      rrc_InterRATMeasurement_r9_interRATMeasurementObjects_eutra_FrequencyList)))
  {
    if(FAILURE == tdsrrcirat_process_eutra_freq_list_mcm_r9(mcm_irat_ptr->interRATMeasurementObjects.u.eutra_FrequencyList,
        &l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list))
    {
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = transaction_id;
      /* Set error choice */
     rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

    /* RRC need to send MCF if all the EUTRAN freq from NW is invalid, rather than just skip them */
    if( (l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list.add_freq_list.num_eutra_frequencies > 0) ||
		(l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list.num_freq_rmv >0) )
    {
      l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list_incl = TRUE;
    }
    /* if no valid eutra freq to add, reply MCF to NW */
	if ( (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mcm_irat_ptr->interRATMeasurementObjects.u.eutra_FrequencyList,eutraNewFrequencies)) &&
		 (l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list.add_freq_list.num_eutra_frequencies == 0) )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"num of valid EUTRA freq to add: %d, invalid config,send MCF",l1_inter_rat_meas_params->meas_object.u.inter_s_eutra.eutra_freq_list.add_freq_list.num_eutra_frequencies);

      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = transaction_id;
      /* Set error choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

  }

  tdsrrcirat_get_idleInterval_Info_r9(
                                    mcm_irat_ptr,
                                    l1_inter_rat_meas_params
                                    );

  return SUCCESS;
}
#endif /* defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) */
#endif /* FEATURE_TDSCDMA_TO_LTE */

#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R8

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/
/*RRC_GKG: This function needs to be reviewed because there're some typos here.*/
uecomdef_status_e_type tdsrrcirat_process_mcm_r8
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
/* zhaoweijia del for rel8 begin compile */
  /* Process the INTER-RAT Measurement Control Message */
  uint8 count = 0, cnt = 0;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_rat_meas_params;

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_SYS;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r8_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  tdsrrc_InterRATMeasurement_r8 *irat_meas_cmd_ptr = NULL;

  boolean bsic_verify = FALSE;

  rrcmeas_interrat_type interrat_type = TDSRRCMEAS_IRAT_GSM;
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
  tdsrrcirat_eutra_freq_type temp_freq_list[TDSL1_MAX_EUTRA_FREQ];
#endif

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8.measurementControl_r8;
  
  /* Assign default values to inter_rat_meas_params */
  memset(&l1_inter_rat_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_rat_meas_params.meas_object.meas_type = TDSL1_INTER_SYS;

  /* Set Transaction Id and Meas Id */
  l1_inter_rat_meas_params.trans_id = tdstransaction_id;
  l1_inter_rat_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;


  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r8_setup))
  {
    /* Double check that it is INTER-RAT Meas */
    if (!(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(rrc_mcm_ptr->measurementCommand.u.setup,
        rrc_MeasurementType_r8_interRATMeasurement)))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
	if(!TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL)
	{
#endif
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
           tdsrrc_InterRATMeasurement_r8,idleIntervalInfo))
      {
#ifdef FEATURE_TDSCDMA_REL9_NO_T2L_CONN_MEAS
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: REL9_NO_T2L_CONN_MEAS enabled,ignore R9 idle interval config");
#else
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"idleIntervalInfo for EUTRA UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
#endif
      }
#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
	}
#endif 

    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r8,interRATMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r8,interRATReportingQuantity)) &&
      (
        ((TDSRRC_CHECK_COMMON_MSG_TYPE
           (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
             rrc_InterRATMeasQuantity_r8_ratSpecificInfo_gsm)) &&
           (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
             interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity !=
             tdsrrc_MeasurementQuantityGSM_dummy))
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
           ||
           (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
             rrc_InterRATMeasQuantity_r8_ratSpecificInfo_e_UTRA))
#endif
      ) &&
#if !(defined (FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined (FEATURE_TDSCDMA_DMO_SUPPORT))
      (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATReportingQuantity.ratSpecificInfo,
      rrc_InterRATReportingQuantity_r8_ratSpecificInfo_gsm) 
      ) &&
#endif
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/ReportCriteria/R.Mode");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria,
      rrc_InterRATReportCriteria,interRATReportingCriteria)) &&
      (!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria,interRATEventList)) ||
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria->interRATEventList.n == 0)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid,event criteria list absent ");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement ;
    if((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
       (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
        T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Quality Estimate is FDD,which is not supported");
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    /*Check if one element is LTE IRAT and other are GSM, then send a failure with cause invalid configuration*/
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(
         rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
         rrc_InterRATMeasQuantity_r8_ratSpecificInfo_e_UTRA))
    {
      interrat_type = TDSRRCMEAS_IRAT_EUTRA;
    }
    if((TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL == FALSE) &&
       (TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO == FALSE) &&
       (interrat_type == TDSRRCMEAS_IRAT_EUTRA))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL and TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO are not enabled");
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }
    if(
        ((TDSRRCMEAS_IRAT_EUTRA == interrat_type) &&
          (((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
              interRATMeasurementObjects)) && 
             (!TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasurementObjects,
               rrc_InterRATMeasurement_r8_interRATMeasurementObjects_eutra_FrequencyList))) ||
             (!TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
               ratSpecificInfo, rrc_InterRATReportingQuantity_r8_ratSpecificInfo_eutra)))) ||
        ((TDSRRCMEAS_IRAT_GSM == interrat_type) &&
        (((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
             interRATMeasurementObjects)) && 
             (!TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasurementObjects,
               rrc_InterRATMeasurement_r8_interRATMeasurementObjects_interRATCellInfoList))) ||
           (!TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
               ratSpecificInfo, rrc_InterRATReportingQuantity_r8_ratSpecificInfo_gsm))))
       )
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"LTOTD: Incompatible choice of RAT choice in objects, meas quantity,reporting quantity");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

#else

    if((irat_meas_cmd_ptr->m.interRATMeasurementObjectsPresent) 
        && (irat_meas_cmd_ptr->interRATMeasurementObjects.t == 
           T_tdsrrc_InterRATMeasurement_r8_interRATMeasurementObjects_eutra_FrequencyList))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupport EUTRA in interRATMeasurementObjects");
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }
#endif
  }
  else
  {

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
      /*Derive if the measurement type is of LTE IRAT based on the stored measurement ID list*/
      /* Store meas in MEASUREMENT_IDENTITY List */
      if(tdsrrcirat_is_meas_id_eutra_meas((uint16) rrc_mcm_ptr->measurementIdentity))
      {
        interrat_type = TDSRRCMEAS_IRAT_EUTRA;
      }
      if((TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL == FALSE) &&
         (TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO == FALSE) &&
         (interrat_type == TDSRRCMEAS_IRAT_EUTRA))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL and TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO are not enabled");
        /* Send Meas Ctrl Failure Message */
  
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
        /* Set error choice */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
  
        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
  
      /*Validations for measurement modify*/
      if((TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r8_modify)) &&
          (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.modify,measurementType)))
      {
        /*
          As of now If interrat rat type is EUTRA, then, no other type should be GSM
          TBD: Can N/W switch EUTRA<->GSM with modify ?
        */
        if(
            ((TDSRRCMEAS_IRAT_EUTRA == interrat_type) &&
             (((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                  (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATMeasurementObjects)) &&
              (!TDSRRC_CHECK_COMMON_MSG_TYPE(
                rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects,
                rrc_InterRATMeasurement_r8_interRATMeasurementObjects_eutra_FrequencyList))) ||
             ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                  (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATMeasQuantity)) &&
              (!TDSRRC_CHECK_COMMON_MSG_TYPE(
                rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
                rrc_InterRATMeasQuantity_r8_ratSpecificInfo_e_UTRA ))) ||
             ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                  (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATReportingQuantity)) &&
              (!TDSRRC_CHECK_COMMON_MSG_TYPE(
                rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATReportingQuantity.ratSpecificInfo,
                rrc_InterRATReportingQuantity_r8_ratSpecificInfo_eutra ))))) ||
            ((TDSRRCMEAS_IRAT_GSM == interrat_type) &&
             (((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                  (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATMeasurementObjects)) &&
              (!TDSRRC_CHECK_COMMON_MSG_TYPE(
                rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects,
                rrc_InterRATMeasurement_r8_interRATMeasurementObjects_interRATCellInfoList))) ||
             ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                  (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATMeasQuantity)) &&
              (!TDSRRC_CHECK_COMMON_MSG_TYPE(
                rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
                rrc_InterRATMeasQuantity_r8_ratSpecificInfo_gsm ))) ||
             ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                  (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATReportingQuantity)) &&
              (!TDSRRC_CHECK_COMMON_MSG_TYPE(
                rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATReportingQuantity.ratSpecificInfo,
                rrc_InterRATReportingQuantity_r8_ratSpecificInfo_gsm )))))
          )
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"LTOTD: Incompatible choice of RAT choice in objects, meas quantity,reporting quantity");
    
          /* Tr Id */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
    
          /* Set faliure choice */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
    
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
          return FAILURE;
        }
      }
#endif

    if((rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r8_modify) &&
        (rrc_mcm_ptr->measurementCommand.u.modify->m.measurementTypePresent))
    {
      irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement;
      if(
          ((irat_meas_cmd_ptr->m.interRATMeasQuantityPresent) &&
            (
#ifndef FEATURE_TDSCDMA_TO_LTE
              (irat_meas_cmd_ptr->interRATMeasQuantity.ratSpecificInfo.t != T_tdsrrc_InterRATMeasQuantity_r8_ratSpecificInfo_gsm) ||
#endif
              ((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
                (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
                 T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))))
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration not supported");
        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        /* Set faliure choice */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }

#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
	  if(!TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL)
	  {
#endif
        if(irat_meas_cmd_ptr->m.idleIntervalInfoPresent)
        {
#ifdef FEATURE_TDSCDMA_REL9_NO_T2L_CONN_MEAS
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: REL9_NO_T2L_CONN_MEAS enabled,ignore R9 idle interval config");
#else
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"idleIntervalInfo for EUTRA UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
        /* Send Meas Ctrl Failure Message */

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
#endif
        }
#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
	  }
#endif

#if !(defined (FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined (FEATURE_TDSCDMA_DMO_SUPPORT))
      if((irat_meas_cmd_ptr->m.interRATMeasurementObjectsPresent) 
          && (irat_meas_cmd_ptr->interRATMeasurementObjects.t == 
             T_tdsrrc_InterRATMeasurement_r8_interRATMeasurementObjects_eutra_FrequencyList))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupport EUTRA in interRATMeasurementObjects");
        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        /* Set faliure choice */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
#endif
    }
  }

  if (((TDSRRCMEAS_IRAT_GSM == interrat_type) && (tdsrrcmcm_is_dualmode_enabled() == FALSE)) 
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
       ||((TDSRRCMEAS_IRAT_EUTRA == interrat_type) && (tdsrrcmcm_is_lte_mode_enabled() == FALSE))
#endif
       )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:IRAT:Mode not enabled,IRAT_Type %d",interrat_type);
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Send Meas Ctrl Failure Msg");
    /* Send Meas Ctrl Failure Message */
  
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
    /* Set error choice */
    rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
  
    /* Send MCF msg to UTRAN */
    tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
    return FAILURE;
  }

  /* Set Transaction Id and Meas Id */
  l1_inter_rat_meas_params.trans_id = tdstransaction_id;
  l1_inter_rat_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;


  /* Assign default values to inter_rat_meas_params */
  l1_inter_rat_meas_params.meas_object_info_included = FALSE;
  l1_inter_rat_meas_params.num_add_meas = 0;
  l1_inter_rat_meas_params.rpt_mode_incl = FALSE;

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
  if(TDSRRCMEAS_IRAT_EUTRA == interrat_type)
  {
    l1_inter_rat_meas_params.meas_object.meas_type = TDSL1_INTER_SYS_EUTRA;
    memset(&l1_inter_rat_meas_params.meas_object.u.inter_s_eutra,0,sizeof(tdsl1_inter_sys_eutra_meas_ctrl_struct_type));
  }
  else
#endif
  {
    l1_inter_rat_meas_params.meas_object.meas_type = TDSL1_INTER_SYS;
    l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = FALSE;
    l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list.num_cell_add = 0;
    l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list.num_cell_meas = 0;
    l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list.num_cell_rmv = 0;
    l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list.cells_for_meas_incl = FALSE;
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = FALSE;
    l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_cell_status_incl = FALSE;
    l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = FALSE;
    /* The following is not supported in Rel 99 anymore. So set it to FALSE */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff = FALSE;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, rrc_MeasurementControl_r8_IEs,
      measurementReportingMode))
  {
    /* Store reporting mode to TRUE */
    l1_inter_rat_meas_params.rpt_mode_incl = TRUE;
  
    l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
  }

  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r8_setup))
  {
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    if(TDSRRCMEAS_IRAT_EUTRA == interrat_type)
    {
      uecomdef_status_e_type status;
      /*Take a back up to temp list*/
      memscpy(temp_freq_list, TDSL1_MAX_EUTRA_FREQ * sizeof(tdsrrcirat_eutra_freq_type),
	      tdsrrcirat_eutra_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(tdsrrcirat_eutra_freq_type));
      tdsrrcirat_eutra_freq_list_ptr = temp_freq_list;

      /*Call the function passing the MCM setup structure that updates LTE info to the l1_inter_rat_meas_params*/
      status = tdsrrcirat_process_mcm_setup_modify_eutra(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
                             &l1_inter_rat_meas_params,tdstransaction_id,&mcm_rpt_crit);
      tdsrrcirat_eutra_freq_list_ptr = tdsrrcirat_eutra_freq_list;

      if(FAILURE == status)
      {
        return status;
      }
    }
    else
#endif
    {
      /* Set Meas Identity */
      l1_inter_rat_meas_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
  
      /* Set Meas Quantity as Inter-sys Meas Quan present */
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;
  
      /* Initialize Default Value to intra_freq_meas_quan_incl */
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl =  FALSE;
  
      /* Initialize Filter Coefficient to Default Value i.e. 0 */
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;
  
      /* Initialize GSM Filter Coefficient to 0 */
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
      TDSL1_GSM_FILTER_COEF_0;
  
      /* Meas Quantity Parameter 1 i.e. Quality Estimate */
      if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
        interRATMeasQuantity,tdsrrc_InterRATMeasQuantity_r8,measQuantityUTRAN_QualityEstimate))
      {
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;
  
        /* Meas Quan i.e ISCP/RSCP/RSSI... */
        /* 10.3.7.38  intra meas quantity type is a list */
        tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.u.inter_s.
                     meas_quan.intra_meas_quan,
                 &rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.
                 measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->intraFreqMeasQuantity_TDDList);
         /* Get Filter Coefficient too */
  
        /* Get Filter Coefficient too */
        /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient_present */

        /* Store filter coefficient */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
          TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.setup->u.
          interRATMeasurement->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.filterCoefficient);
      } /* End of quality estimate present */

      /* Set System Choice to GSM */
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;

      /* Get GSM Meas Quan, Filter Coefficient, BSIC required */

      /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
      So set it to RSSI */
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
        TDSL1_GSM_CARRIER_RSSI;

      /* GSM Filter Coefficient present */
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
          TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.
          u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo.u.gsm->filterCoefficient);

#ifdef FEATURE_ENH_BSIC_VERIFY
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
      bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
        interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#else
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
        TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
        interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#endif

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"R8 MCM gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);

      /* Store Reporting Quantity */
      l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;

      /* Sys Choice */
      l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;

      /* L1 wants this to be FALSE for the first version as it does not support it */
      l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;

      /* Ignore reading Observed time difference as no longer supported in Rel 99
      l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
      rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
      ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */

      /* Check if gsm_carrier_rssi wanted in Meas Report */
      l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
        rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
        ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;

      if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.setup->
        u.interRATMeasurement->reportCriteria, &l1_inter_rat_meas_params.meas_object.u.inter_s,
        &mcm_rpt_crit, tdstransaction_id))
      {
        return FAILURE;
      }

      /* Store reporting mode to TRUE */
      l1_inter_rat_meas_params.rpt_mode_incl = TRUE;

      l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
        rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

      /* For L1 sake, copy reporting criteria */
      l1_inter_rat_meas_params.reporting_mode.rpt_mode =
        l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;

      /* Now set other values */
      l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

      l1_inter_rat_meas_params.meas_object_info_included = TRUE;

      /* Time to process the cell list */
      if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
        interRATMeasurementObjects)) && (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasurementObjects,
          rrc_InterRATMeasurement_r8_interRATMeasurementObjects,interRATCellInfoList)))
      {
        if (tdsrrcirat_process_cell_list_mcm_r6(rrc_mcm_ptr->measurementCommand.
          u.setup->u.interRATMeasurement->interRATMeasurementObjects.u.interRATCellInfoList,
          &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list) == FAILURE)
        {
          /* Send Meas Ctrl Failure Msg */
          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
         rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;
        }
        /* Set Cell list is present */
        l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
      }
    }

    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

      /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
        if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
          addtl_meas_incl, TRUE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
          &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      }
    } /* End of if of Additional Meas Present */

    if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      interRATMeasurementObjects))
    {
      if(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasurementObjects.t ==
        T_tdsrrc_InterRATMeasurement_r8_interRATMeasurementObjects_interRATCellInfoList)
      {
        /* At this point everything has been validated and now is the time
        to update the inter rat CELL_INFO_LIST */
  
        for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		  &tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
        }
      }
    }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    if(TDSRRCMEAS_IRAT_EUTRA == interrat_type)
    {
      /*Copy back the temp freq list to orig freq list*/
      memscpy(tdsrrcirat_eutra_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(tdsrrcirat_eutra_freq_type),
	      temp_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(tdsrrcirat_eutra_freq_type));
    }
#endif

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        if(tdsinter_rat_meas_id_list[count].interrat_type != interrat_type)
        {
          tdsrrcirat_delete_mcm_due_to_irat_type_change(l1_inter_rat_meas_params.meas_id);
        }
        break;
      }
    }
#endif

    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
        tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsinter_rat_meas_id_list[count].interrat_type = interrat_type;
        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
        meas_exists_for_setup = TRUE;
        break;
      }
    }

    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
      {
        /* First check if this meas already exists */
        if (tdsinter_rat_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsinter_rat_meas_id_list[count].interrat_type = interrat_type;
          tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
          break;
        }
      }
    } /* End of if */

    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }

    /* Now one needs to send command to L1 */

  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r8_modify)
  {
    /* Set type to MODIFY */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r8_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:I.RAT Modify: MType absent");

      /* Check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r8_IEs,additionalMeasurementList))
      {
        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }
    }
    else
    {
      if (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.t != 
          T_tdsrrc_MeasurementType_r8_interRATMeasurement)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:Meas Id is not Inter-RAT");
        /* Send Meas Ctrl Failure  */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
      if(TDSRRCMEAS_IRAT_EUTRA == interrat_type)
      {
        uecomdef_status_e_type status;
        /*Take a back up to temp list*/
        memscpy(temp_freq_list, TDSL1_MAX_EUTRA_FREQ * sizeof(rrcirat_eutra_freq_type),
		tdsrrcirat_eutra_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(rrcirat_eutra_freq_type));
        tdsrrcirat_eutra_freq_list_ptr = temp_freq_list;

        /*Call the function passing the MCM modify structure that updates LTE info to the l1_inter_rat_meas_params*/
        status = tdsrrcirat_process_mcm_setup_modify_eutra(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
                               &l1_inter_rat_meas_params,tdstransaction_id,&mcm_rpt_crit);
        tdsrrcirat_eutra_freq_list_ptr = tdsrrcirat_eutra_freq_list;
        if(FAILURE == status)
        {
          return status;
        }
      }
      else
#endif
      {
        /* Initialize Filter Coefficient to Default Value i.e. 0 */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;
  
        /* Initialize GSM Filter Coefficient to 0 */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
          TDSL1_GSM_FILTER_COEF_0;
        if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
          interRATMeasurement,tdsrrc_InterRATMeasurement_r6,interRATMeasQuantity)))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Meas Quan absent");
        }
        else
        {
          /* Set Meas Quantity to TRUE */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = FALSE;
          if (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
            interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity ==
            tdsrrc_MeasurementQuantityGSM_dummy)
          {
  
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCM:MeasQuantity PathLoss");
  
            /* Tr Id */
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
  
            /* Set Error Cause */
            rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
  
            return FAILURE;
          }
  
          /* Meas Quantity Parameter 1 i.e. Quality Estimate */
          if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
            interRATMeasQuantity,tdsrrc_InterRATMeasQuantity,measQuantityUTRAN_QualityEstimate))
          {
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;
  
            /* Meas Quan i.e ISCP/RSCP/RSSI... */
            if (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
                       interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t == 
                       T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_fdd)
            {
              /* check FDD mode */            
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport FDD mode in measQuantityUTRAN_QualityEstimate");
              /* Send Meas Ctrl Failure  */
              
              rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
              
              /* Set error t */
              rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
              
              /* Send MCF msg to UTRAN */
              tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
              
              return FAILURE;
            }
            else
            {
              /* 10.3.7.38  intra meas quantity type is a list */
              tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.u.inter_s.
                          meas_quan.intra_meas_quan,
                       &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
                       interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->
                       intraFreqMeasQuantity_TDDList);
              /* Get Filter Coefficient too */
            }
            /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient_present */
          
            /* Store filter coefficient */
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
              TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u
              .modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
              measQuantityUTRAN_QualityEstimate.filterCoefficient);
            
          }
  
          /* Set System Choice to GSM */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;
  
          /* Get GSM Meas Quan, Filter Coefficient, BSIC required */
  
          /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
          So set it to RSSI */
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
            TDSL1_GSM_CARRIER_RSSI;
  
            /* GSM Filter Coefficient present */
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
              TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.
              modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
              ratSpecificInfo.u.gsm->filterCoefficient);
          
  #ifdef FEATURE_ENH_BSIC_VERIFY 
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
          bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
              modify->measurementType.u.interRATMeasurement->
            interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
  #else
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
            TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
              modify->measurementType.u.interRATMeasurement->
            interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
  #endif
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"R8 MCM Modify gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);
  
        } /* End of else of Meas Quan present */

        /* Check if reporting quan present */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
           tdsrrc_InterRATMeasurement_r6,interRATReportingQuantity))
        {
          /* Store Reporting Quantity */
          l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;
  
          /* Sys Choice */
          l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;
  
          /* L1 wants this to be FALSE for the first version as it does not support it */
          l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;
  
          /* Observed time difference no longer supported in Rel 99
          l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
            rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
            interRATReportingQuantity.ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */
  
            /* Check if gsm_carrier_rssi wanted in Meas Report */
            l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
            rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
            interRATReportingQuantity.ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;
        }

        if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.modify->
          measurementType.u.interRATMeasurement->reportCriteria,
          &l1_inter_rat_meas_params.meas_object.u.inter_s, &mcm_rpt_crit, tdstransaction_id))
        {
          return FAILURE;
        }
        l1_inter_rat_meas_params.reporting_mode.rpt_mode =
        l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;
        
        l1_inter_rat_meas_params.meas_object_info_included = TRUE;
        if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
          tdsrrc_MeasurementCommand_r8_modify,measurementType))
          && (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects,
           rrc_InterRATMeasurement_r8_interRATMeasurementObjects,interRATCellInfoList)) )
        {
          /* Time to process the cell list */
          if (tdsrrcirat_process_cell_list_mcm_r6(rrc_mcm_ptr->measurementCommand.
            u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects.u.interRATCellInfoList,
            &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list)
            == FAILURE)
          {
            /* Send Meas Ctrl Failure Msg */
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
  
            /* Set Error Cause */
            rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
  
            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
  
            return FAILURE;
          }
  
          /* Set Cell list is present */
          l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
        }
      }
      /* Check for additional measurements */
      /* Call fn which makes check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r7_IEs,additionalMeasurementList))
      {
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type,&rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }


      /* Set reporting criteria in RRC DB */
      for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
      {
        if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
        {
          if(mcm_rpt_crit != TDSNOT_AVAILABLE)
          {      
            tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
            tdsinter_rat_meas_id_list[count].interrat_type = interrat_type;
            tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reportinf Crit set in RRC DB");
          }
          break;
        }
      }
      
      if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
         tdsrrc_MeasurementCommand_r8_modify,measurementType))
        && (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects,
         rrc_InterRATMeasurement_r8_interRATMeasurementObjects,interRATCellInfoList)) )
      {
        /* At this point everything has been validated and now is the time
        to update the inter rat CELL_INFO_LIST */
        for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		  &tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
        }
      }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
      if(TDSRRCMEAS_IRAT_EUTRA == interrat_type)
      {
        /*Copy the updated frequency list*/
        memscpy(tdsrrcirat_eutra_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(rrcirat_eutra_freq_type),
	        temp_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(rrcirat_eutra_freq_type));
      }
#endif
    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
  * that this meas exists. So let us delete it from tdsinter_rat_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_rat_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_rat_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;

        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = FALSE;

        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsinter_rat_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */


  if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r8_setup))
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
        interRATMeasurementObjects)))
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
          interRATMeasurementObjects,rrc_InterRATMeasurement_r8_interRATMeasurementObjects,interRATCellInfoList))
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind = rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
          interRATMeasurementObjects.u.interRATCellInfoList->interRATCellInfoIndication_r6;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }

  if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r8_modify))
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r8_modify,measurementType)) &&
       (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
        interRATMeasurementObjects)))
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
        interRATMeasurementObjects,rrc_InterRATMeasurement_r8_interRATMeasurementObjects,interRATCellInfoList))
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind = rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
          interRATMeasurement->interRATMeasurementObjects.u.interRATCellInfoList->interRATCellInfoIndication_r6;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }

  if(!tdsprocess_interfreq_interrat_meas_rel7)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending interRAT measurements from MCM to L1");
    return SUCCESS;
  }

  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_rat_meas_params.trans_id = alloc_idx;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Sending cmd to L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_rat_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);

  return SUCCESS;
}

#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R9

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/
/*RRC_GKG: This function needs to be reviewed because there're some typos here.*/
uecomdef_status_e_type tdsrrcirat_process_mcm_r9
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
)
{
  /* Process the INTER-RAT Measurement Control Message */
  uint8 count = 0, cnt = 0;

  /* Declare MCM config structure that goes to L1 */
  tdsl1_meas_ctrl_parm_struct_type  l1_inter_rat_meas_params;

  /* Stores meas ctrl failure parameters */
  tdsrrcmeas_mcf_struct_type rrcmeas_mcf;

  /* Declared to store rpt criteria in RRC DB */
  tdsrpt_crit_enum_type mcm_rpt_crit = TDSNOT_AVAILABLE;

  /* Declared to store meas type */
  tdsrrcmeas_layer_e_type primary_meas_type = TDSINTER_SYS;

  /* This var is set to TRUE in case addtl meas is successfully validated */
  boolean addtl_meas_incl = FALSE;

  /* This is used if for SETUP, meas id does not exist in DB */
  boolean meas_exists_for_setup = FALSE;

  tdsrrc_MeasurementControl_r9_IEs* rrc_mcm_ptr = NULL;

  tdsrrc_DL_DCCH_Message* dcch_msg_ptr = NULL;

  uint8 alloc_idx = TDSMAXIMUM_TRANSACTIONS;

  tdsrrc_InterRATMeasurement_r9 *irat_meas_cmd_ptr = NULL;

  boolean bsic_verify = FALSE;

  rrcmeas_interrat_type interrat_type = TDSRRCMEAS_IRAT_GSM;
  
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
  tdsrrcirat_eutra_freq_type temp_freq_list[TDSL1_MAX_EUTRA_FREQ];
#endif

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.
    downlink_sdu_ind.decoded_msg;

  /* Store actual mcm ptr in a local variable */
  rrc_mcm_ptr = &dcch_msg_ptr->message.u.measurementControl.u.
    later_than_r3.criticalExtensions.u.later_than_r4.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.measurementControl_r9;
  
  /* Assign default values to inter_rat_meas_params */
  memset(&l1_inter_rat_meas_params, 0, sizeof(tdsl1_meas_ctrl_parm_struct_type));

  l1_inter_rat_meas_params.meas_object.meas_type = TDSL1_INTER_SYS;

  /* Set Transaction Id and Meas Id */
  l1_inter_rat_meas_params.trans_id = tdstransaction_id;
  l1_inter_rat_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;


  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r9_setup))
  {
    /* Double check that it is INTER-RAT Meas */
    if (!(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(rrc_mcm_ptr->measurementCommand.u.setup,
        rrc_MeasurementType_r9_interRATMeasurement)))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
	if(!TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL)
	{
#endif
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
        tdsrrc_InterRATMeasurement_r9,idleIntervalInfo))
  
      {
#ifdef FEATURE_TDSCDMA_REL9_NO_T2L_CONN_MEAS
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: REL9_NO_T2L_CONN_MEAS enabled,ignore R9 idle interval config");
#else
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"idleIntervalInfo for EUTRA UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
#endif
      }
#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
	}
#endif
    if (!((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r9,interRATMeasQuantity)) &&
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      tdsrrc_InterRATMeasurement_r9,interRATReportingQuantity)) &&
      (
        ((TDSRRC_CHECK_COMMON_MSG_TYPE
           (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
             rrc_InterRATMeasQuantity_r8_ratSpecificInfo_gsm)) &&
           (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
             interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity !=
             tdsrrc_MeasurementQuantityGSM_dummy))
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
           ||
           (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
             rrc_InterRATMeasQuantity_r8_ratSpecificInfo_e_UTRA))
#endif      
      ) &&
#if !(defined (FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined (FEATURE_TDSCDMA_DMO_SUPPORT))
      (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATReportingQuantity.ratSpecificInfo,
      rrc_InterRATReportingQuantity_r8_ratSpecificInfo_gsm) 
      ) &&
#endif
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,measurementReportingMode))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid MeasQuan/ReportQuan/ReportCriteria/R.Mode");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    if((TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria,
      rrc_InterRATReportCriteria,interRATReportingCriteria)) &&
      (!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria,interRATEventList)) ||
      (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->reportCriteria.
      u.interRATReportingCriteria->interRATEventList.n == 0)))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SETUP:Invalid,event criteria list absent ");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationIncomplete;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }

    irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement ;
    if((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
       (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
        T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Quality Estimate is FDD,which is not supported");
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    /*Check if one element is LTE IRAT and other are GSM, then send a failure with cause invalid configuration*/
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(
         rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
         rrc_InterRATMeasQuantity_r8_ratSpecificInfo_e_UTRA))
    {
      interrat_type = TDSRRCMEAS_IRAT_EUTRA;
    }
    if((TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL == FALSE) &&
       (TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO == FALSE) &&
       (interrat_type == TDSRRCMEAS_IRAT_EUTRA))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL and TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO are not enabled");
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }
    if(
        ((TDSRRCMEAS_IRAT_EUTRA == interrat_type) &&
          (((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
              interRATMeasurementObjects)) && 
             (!TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasurementObjects,
               rrc_InterRATMeasurement_r8_interRATMeasurementObjects_eutra_FrequencyList))) ||
             (!TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
               ratSpecificInfo, rrc_InterRATReportingQuantity_r8_ratSpecificInfo_eutra)))) ||
        ((TDSRRCMEAS_IRAT_GSM == interrat_type) &&
        (((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
             interRATMeasurementObjects)) && 
             (!TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasurementObjects,
               rrc_InterRATMeasurement_r8_interRATMeasurementObjects_interRATCellInfoList))) ||
           (!TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
               ratSpecificInfo, rrc_InterRATReportingQuantity_r8_ratSpecificInfo_gsm))))
       )
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"LTOTD: Incompatible choice of RAT choice in objects, meas quantity,reporting quantity");

      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

#else

    if((irat_meas_cmd_ptr->m.interRATMeasurementObjectsPresent) 
        && (irat_meas_cmd_ptr->interRATMeasurementObjects.t == 
           T_tdsrrc_InterRATMeasurement_r9_interRATMeasurementObjects_eutra_FrequencyList))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupport EUTRA in interRATMeasurementObjects");
      /* Tr Id */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
      /* Set faliure choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }
#endif

  }
  else
  {

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    /*Derive if the measurement type is of LTE IRAT based on the stored measurement ID list*/
    /* Store meas in MEASUREMENT_IDENTITY List */
    if(tdsrrcirat_is_meas_id_eutra_meas((uint16) rrc_mcm_ptr->measurementIdentity))
    {
      interrat_type = TDSRRCMEAS_IRAT_EUTRA;
    }
    if((TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL == FALSE) && 
      (TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO == FALSE) &&
      (interrat_type == TDSRRCMEAS_IRAT_EUTRA))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL and TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO are not enabled");
      /* Send Meas Ctrl Failure Message */

      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error choice */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
      return FAILURE;
    }

    /*Validations for measurement modify*/
    if((TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r8_modify)) &&
        (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.modify,measurementType)))
    {
      /*
        As of now If interrat rat type is EUTRA, then, no other type should be GSM
        TBD: Can N/W switch EUTRA<->GSM with modify ?
      */
      if(
          ((TDSRRCMEAS_IRAT_EUTRA == interrat_type) &&
           (((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATMeasurementObjects)) &&
            (!TDSRRC_CHECK_COMMON_MSG_TYPE(
              rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects,
              rrc_InterRATMeasurement_r8_interRATMeasurementObjects_eutra_FrequencyList))) ||
           ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATMeasQuantity)) &&
            (!TDSRRC_CHECK_COMMON_MSG_TYPE(
              rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
              rrc_InterRATMeasQuantity_r8_ratSpecificInfo_e_UTRA ))) ||
           ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATReportingQuantity)) &&
            (!TDSRRC_CHECK_COMMON_MSG_TYPE(
              rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATReportingQuantity.ratSpecificInfo,
              rrc_InterRATReportingQuantity_r8_ratSpecificInfo_eutra ))))) ||
          ((TDSRRCMEAS_IRAT_GSM == interrat_type) &&
           (((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATMeasurementObjects)) &&
            (!TDSRRC_CHECK_COMMON_MSG_TYPE(
              rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects,
              rrc_InterRATMeasurement_r8_interRATMeasurementObjects_interRATCellInfoList))) ||
           ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATMeasQuantity)) &&
            (!TDSRRC_CHECK_COMMON_MSG_TYPE(
              rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo,
              rrc_InterRATMeasQuantity_r8_ratSpecificInfo_gsm ))) ||
           ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR
                (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,interRATReportingQuantity)) &&
            (!TDSRRC_CHECK_COMMON_MSG_TYPE(
              rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATReportingQuantity.ratSpecificInfo,
              rrc_InterRATReportingQuantity_r8_ratSpecificInfo_gsm )))))
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"LTOTD: Incompatible choice of RAT choice in objects, meas quantity,reporting quantity");
  
        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
        /* Set faliure choice */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
  
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
    }
#endif

    if((rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r9_modify) &&
        (rrc_mcm_ptr->measurementCommand.u.modify->m.measurementTypePresent))
    {
      irat_meas_cmd_ptr = rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement;
      if(
          ((irat_meas_cmd_ptr->m.interRATMeasQuantityPresent) &&
            (
#ifndef FEATURE_TDSCDMA_TO_LTE
              (irat_meas_cmd_ptr->interRATMeasQuantity.ratSpecificInfo.t != T_tdsrrc_InterRATMeasQuantity_r8_ratSpecificInfo_gsm) ||
#endif
              ((irat_meas_cmd_ptr->interRATMeasQuantity.m.measQuantityUTRAN_QualityEstimatePresent) &&
                (irat_meas_cmd_ptr->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t !=
                 T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_tdd))))
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Configuration not supported");
        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        /* Set faliure choice */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
	  if(!TDSCHECK_OPTIONAL_FEATURE_ENABLE_IDLEINTERVAL)
	  {
#endif
      if(irat_meas_cmd_ptr->m.idleIntervalInfoPresent)
      {
#ifdef FEATURE_TDSCDMA_REL9_NO_T2L_CONN_MEAS
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: REL9_NO_T2L_CONN_MEAS enabled,ignore R9 idle interval config");
#else
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"idleIntervalInfo for EUTRA UnSupported Meas %d",rrc_mcm_ptr->measurementIdentity);
        /* Send Meas Ctrl Failure Message */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
#endif
      }
#ifdef FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT
	  }
#endif

#if !(defined (FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined (FEATURE_TDSCDMA_DMO_SUPPORT))
      if((irat_meas_cmd_ptr->m.interRATMeasurementObjectsPresent) 
          && (irat_meas_cmd_ptr->interRATMeasurementObjects.t == 
             T_tdsrrc_InterRATMeasurement_r9_interRATMeasurementObjects_eutra_FrequencyList))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupport EUTRA in interRATMeasurementObjects");
        /* Tr Id */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
        /* Set faliure choice */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;
      }
#endif
    }
  }

  if (((TDSRRCMEAS_IRAT_GSM == interrat_type) && (tdsrrcmcm_is_dualmode_enabled() == FALSE)) 
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
           ||((TDSRRCMEAS_IRAT_EUTRA == interrat_type) && (tdsrrcmcm_is_lte_mode_enabled() == FALSE))
#endif
       )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:IRAT:Mode not enabled,IRAT_Type %d",interrat_type);
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Send Meas Ctrl Failure Msg");
    /* Send Meas Ctrl Failure Message */
  
    rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
    /* Set error choice */
    rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;
  
    /* Send MCF msg to UTRAN */
    tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
    return FAILURE;
  }

  /* Set Transaction Id and Meas Id */
  l1_inter_rat_meas_params.trans_id = tdstransaction_id;
  l1_inter_rat_meas_params.meas_id =  (uint16) rrc_mcm_ptr->measurementIdentity;


  /* Assign default values to inter_rat_meas_params */
  l1_inter_rat_meas_params.meas_object_info_included = FALSE;
  l1_inter_rat_meas_params.num_add_meas = 0;
  l1_inter_rat_meas_params.rpt_mode_incl = FALSE;
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    if(TDSRRCMEAS_IRAT_EUTRA == interrat_type)
    {
      l1_inter_rat_meas_params.meas_object.meas_type = TDSL1_INTER_SYS_EUTRA;
      memset(&l1_inter_rat_meas_params.meas_object.u.inter_s_eutra,0,sizeof(tdsl1_inter_sys_eutra_meas_ctrl_struct_type));
    }
    else
#endif
    {

  l1_inter_rat_meas_params.meas_object.meas_type = TDSL1_INTER_SYS;
  l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = FALSE;
  l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list.num_cell_add = 0;
  l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list.num_cell_meas = 0;
  l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list.num_cell_rmv = 0;
  l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list.cells_for_meas_incl = FALSE;
  l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = FALSE;
  l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_cell_status_incl = FALSE;
  l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = FALSE;
  /* The following is not supported in Rel 99 anymore. So set it to FALSE */
  l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff = FALSE;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, rrc_MeasurementControl_r8_IEs,
      measurementReportingMode))
  {
    /* Store reporting mode to TRUE */
    l1_inter_rat_meas_params.rpt_mode_incl = TRUE;
  
    l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);
  }

  if (TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,
    rrc_MeasurementCommand_r9_setup))
  {
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_SETUP;
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    if(TDSRRCMEAS_IRAT_EUTRA == interrat_type)
    {
      uecomdef_status_e_type status;
      /*Take a back up to temp list*/
      memscpy(temp_freq_list, TDSL1_MAX_EUTRA_FREQ * sizeof(tdsrrcirat_eutra_freq_type),
	      tdsrrcirat_eutra_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(tdsrrcirat_eutra_freq_type));
      tdsrrcirat_eutra_freq_list_ptr = temp_freq_list;

      /*Call the function passing the MCM setup structure that updates LTE info to the l1_inter_rat_meas_params*/
      status = tdsrrcirat_process_mcm_setup_modify_eutra_r9(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
                             &l1_inter_rat_meas_params,tdstransaction_id,&mcm_rpt_crit);
      tdsrrcirat_eutra_freq_list_ptr = tdsrrcirat_eutra_freq_list;

      if(FAILURE == status)
      {
        return status;
      }
    }
    else
#endif
    {      /* Set Meas Identity */
    l1_inter_rat_meas_params.meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;

    /* Set Meas Quantity as Inter-sys Meas Quan present */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;

    /* Initialize Default Value to intra_freq_meas_quan_incl */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl =  FALSE;

    /* Initialize Filter Coefficient to Default Value i.e. 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;

    /* Initialize GSM Filter Coefficient to 0 */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
    TDSL1_GSM_FILTER_COEF_0;

    /* Meas Quantity Parameter 1 i.e. Quality Estimate */
    if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity,tdsrrc_InterRATMeasQuantity_r8,measQuantityUTRAN_QualityEstimate))
    {
      l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;

      /* Meas Quan i.e ISCP/RSCP/RSSI... */
      /* 10.3.7.38  intra meas quantity type is a list */
      tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.u.inter_s.
                   meas_quan.intra_meas_quan,
               &rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasQuantity.
               measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd-> intraFreqMeasQuantity_TDDList);
       /* Get Filter Coefficient too */


      /* Get Filter Coefficient too */
      /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient_present */
      
        /* Store filter coefficient */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
          TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.setup->u.
          interRATMeasurement->interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.filterCoefficient);
      
    } /* End of quality estimate present */

    /* Set System Choice to GSM */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;

    /* Get GSM Meas Quan, Filter Coefficient, BSIC required */

    /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
    So set it to RSSI */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
      TDSL1_GSM_CARRIER_RSSI;

  
    /* GSM Filter Coefficient present */
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
        TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.
        u.setup->u.interRATMeasurement->interRATMeasQuantity.ratSpecificInfo.u.gsm->filterCoefficient);

#ifdef FEATURE_ENH_BSIC_VERIFY
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
    bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#else
    l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
      TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
      interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
#endif

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"R9 MCM gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);

    /* Store Reporting Quantity */
    l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;

    /* Sys Choice */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;

    /* L1 wants this to be FALSE for the first version as it does not support it */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;

    /* Ignore reading Observed time difference as no longer supported in Rel 99
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
    rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
    ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */

    /* Check if gsm_carrier_rssi wanted in Meas Report */
    l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
      rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATReportingQuantity.
      ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;

    if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.setup->
      u.interRATMeasurement->reportCriteria, &l1_inter_rat_meas_params.meas_object.u.inter_s,
      &mcm_rpt_crit, tdstransaction_id))
    {
      return FAILURE;
    }

    /* Store reporting mode to TRUE */
    l1_inter_rat_meas_params.rpt_mode_incl = TRUE;

    l1_inter_rat_meas_params.reporting_mode.rpt_trans_mode = TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(
      rrc_mcm_ptr->measurementReportingMode.measurementReportTransferMode);

    /* For L1 sake, copy reporting criteria */
    l1_inter_rat_meas_params.reporting_mode.rpt_mode =
      l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;

    /* Now set other values */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_SETUP;

    l1_inter_rat_meas_params.meas_object_info_included = TRUE;


    /* Time to process the cell list */
    if ((rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->m.interRATMeasurementObjectsPresent) &&
         (rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasurementObjects.t ==
           T_tdsrrc_InterRATMeasurement_r9_interRATMeasurementObjects_interRATCellInfoList))
    {
      if (tdsrrcirat_process_cell_list_mcm_r6(rrc_mcm_ptr->measurementCommand.
        u.setup->u.interRATMeasurement->interRATMeasurementObjects.u.interRATCellInfoList,
        &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list) == FAILURE)
      {
        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
       rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }
      /* Set Cell list is present */
      l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
    }
}

    /* Now check Additional Measurements */
    /* Process Additional Meas */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MCM:Addtl Meas given for Id %d",rrc_mcm_ptr->measurementIdentity);

      /* Call fn which makes check for additional measurements */
      if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
        &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
      {
        /* Called function has already sent Meas Ctrl Failure */
        return FAILURE;
      }
      else
      {
        addtl_meas_incl = TRUE;
        /* Can't set additional meas in DB now as meas id has not yet been set */
        if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
          addtl_meas_incl, TRUE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
          &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
        {
          /* As UTRAN tried to configure more than 4 Addtl Meas */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

          rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

          /* Set error t */
          rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

          /* Send MCF msg to UTRAN */
          tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

          return FAILURE;

        }
      }
    } /* End of if of Additional Meas Present */

    if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
      interRATMeasurementObjects))
    {
      if(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->interRATMeasurementObjects.t ==
          T_tdsrrc_InterRATMeasurement_r9_interRATMeasurementObjects_interRATCellInfoList)
      {
        /* At this point everything has been validated and now is the time
        to update the inter rat CELL_INFO_LIST */
  
        for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		  &tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
        }
      }
    }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        if(tdsinter_rat_meas_id_list[count].interrat_type != interrat_type)
        {
          tdsrrcirat_delete_mcm_due_to_irat_type_change(l1_inter_rat_meas_params.meas_id);
        }
        break;
      }
    }
#endif

    /* Store meas in MEASUREMENT_IDENTITY List */
    for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
    {
      /* First check if this meas already exists */
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
        tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
        tdsinter_rat_meas_id_list[count].interrat_type = interrat_type;
        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
        meas_exists_for_setup = TRUE;
        break;
      }
    }

    /* Check if meas already exists. If not set the crit */
    if (meas_exists_for_setup == FALSE)
    {
      /* Set this meas in Meas Id List */
      for ( count = 0; count < TDSMAX_INTER_RAT_MEAS; count++ )
      {
        /* First check if this meas already exists */
        if (tdsinter_rat_meas_id_list[count].meas_id == TDSINVALID_MEAS_ID)
        {
          tdsinter_rat_meas_id_list[count].meas_id = (uint16) rrc_mcm_ptr->measurementIdentity;
          tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
          tdsinter_rat_meas_id_list[count].interrat_type = interrat_type;
          tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
          break;
        }
      }

    } /* End of if */

    if (addtl_meas_incl == TRUE)
    {
      tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
        rrc_mcm_ptr->measurementIdentity);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
    }

    /* Now one needs to send command to L1 */

  } /* End of if of set up */
  else if (rrc_mcm_ptr->measurementCommand.t == T_tdsrrc_MeasurementCommand_r9_modify)
  {
    /* Set type to MODIFY */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_MODIFY;


    /* Check if Meas Object Present */
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
      tdsrrc_MeasurementCommand_r9_modify,measurementType)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:I.RAT Modify: MType absent");

      /* Check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
      {
        /* Call fn which makes check for additional measurements */
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type, &rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }
    }
    else
    {
      if (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.t !=
            T_tdsrrc_MeasurementType_r9_interRATMeasurement)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Modify:Meas Id is not Inter-RAT");
        /* Send Meas Ctrl Failure  */

        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

        return FAILURE;
      }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
      if(TDSRRCMEAS_IRAT_EUTRA == interrat_type)
      {
        uecomdef_status_e_type status;
        /*Take a back up to temp list*/
        memscpy(temp_freq_list, TDSL1_MAX_EUTRA_FREQ * sizeof(rrcirat_eutra_freq_type),
		tdsrrcirat_eutra_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(rrcirat_eutra_freq_type));
        tdsrrcirat_eutra_freq_list_ptr = temp_freq_list;
 
        /*Call the function passing the MCM modify structure that updates LTE info to the l1_inter_rat_meas_params*/
        status = tdsrrcirat_process_mcm_setup_modify_eutra_r9(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
                               &l1_inter_rat_meas_params,tdstransaction_id,&mcm_rpt_crit);
        tdsrrcirat_eutra_freq_list_ptr = tdsrrcirat_eutra_freq_list;
        if(FAILURE == status)
        {
          return status;
        }
      }
      else
#endif
      {
         /* Initialize Filter Coefficient to Default Value i.e. 0 */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = 0;
  
        /* Initialize GSM Filter Coefficient to 0 */
        l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
          TDSL1_GSM_FILTER_COEF_0;
        if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
          interRATMeasurement,tdsrrc_InterRATMeasurement_r6,interRATMeasQuantity)))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Meas Quan absent");
        }
        else
        {
          /* Set Meas Quantity to TRUE */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan_incl = TRUE;
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = FALSE;
          if (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
            interRATMeasQuantity.ratSpecificInfo.u.gsm->measurementQuantity ==
            tdsrrc_MeasurementQuantityGSM_dummy)
          {
  
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCM:MeasQuantity PathLoss");
  
            /* Tr Id */
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
  
            /* Set Error Cause */
            rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending meas ctrl failure message");
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
  
            return FAILURE;
          }
  
          /* Meas Quantity Parameter 1 i.e. Quality Estimate */
          if (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
            interRATMeasQuantity,tdsrrc_InterRATMeasQuantity,measQuantityUTRAN_QualityEstimate))
          {
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan_incl = TRUE;
  
            /* Meas Quan i.e ISCP/RSCP/RSSI... */
            if (rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
                       interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.t == 
                       T_tdsrrc_IntraFreqMeasQuantity_modeSpecificInfo_fdd)
            {
              /* check FDD mode */            
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport FDD mode in measQuantityUTRAN_QualityEstimate");
              /* Send Meas Ctrl Failure  */
              
              rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
              
              /* Set error t */
              rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_unsupportedMeasurement;
              
              /* Send MCF msg to UTRAN */
              tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
              
              return FAILURE;
            }
            else
            {
              /* 10.3.7.38  intra meas quantity type is a list */
              tdsrrcmeas_set_intra_meas_quan_info(&l1_inter_rat_meas_params.meas_object.u.inter_s.
                          meas_quan.intra_meas_quan,
                       &rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
                       interRATMeasQuantity.measQuantityUTRAN_QualityEstimate.modeSpecificInfo.u.tdd->
                       intraFreqMeasQuantity_TDDList);
              /* Get Filter Coefficient too */
            }
            /* Check if rrc_IntraFreqMeasQuantity_filterCoefficient_present */
          
            /* Store filter coefficient */
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.intra_meas_quan.filter_coef = (uint16)
              TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u
              .modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
              measQuantityUTRAN_QualityEstimate.filterCoefficient);
            
          }
  
          /* Set System Choice to GSM */
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.system_choice = TDSL1_GSM_SYSTEM;
  
          /* Get GSM Meas Quan, Filter Coefficient, BSIC required */
  
          /* Acc to specs, this quan can never be GSM_PATHLOSS(Error already taken care of.
          So set it to RSSI */
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.meas_quan =
            TDSL1_GSM_CARRIER_RSSI;
  
            /* GSM Filter Coefficient present */
            l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.filter_coef =
              TDSRET_GSM_FILTER_COEFFICIENT(rrc_mcm_ptr->measurementCommand.u.
              modify->measurementType.u.interRATMeasurement->interRATMeasQuantity.
              ratSpecificInfo.u.gsm->filterCoefficient);
          
  
  #ifdef FEATURE_ENH_BSIC_VERIFY
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req = FALSE;
          bsic_verify = TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
              modify->measurementType.u.interRATMeasurement->
            interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
  #else
          l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req =
            TDSRET_BSIC_VERIFICATION_REQD(rrc_mcm_ptr->measurementCommand.u.
              modify->measurementType.u.interRATMeasurement->
            interRATMeasQuantity.ratSpecificInfo.u.gsm->bsic_VerificationRequired);
  #endif
  
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"R9 MCM Modify gsm_meas_quan.bsic_ver_req=%d",l1_inter_rat_meas_params.meas_object.u.inter_s.meas_quan.u.gsm_meas_quan.bsic_ver_req);
        } /* End of else of Meas Quan present */

        /* Check if reporting quan present */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
           tdsrrc_InterRATMeasurement_r6,interRATReportingQuantity))
        {
          /* Store Reporting Quantity */
          l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_quan_incl = TRUE;
  
          /* Sys Choice */
          l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.sys_choice = TDSL1_GSM_SYSTEM;
  
          /* L1 wants this to be FALSE for the first version as it does not support it */
          l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.utran_est_qual = FALSE;
  
          /* Observed time difference no longer supported in Rel 99
          l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.time_diff =
            rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
            interRATReportingQuantity.ratSpecificInfo.u.gsm->observedTimeDifferenceGSM; */
  
            /* Check if gsm_carrier_rssi wanted in Meas Report */
            l1_inter_rat_meas_params.meas_object.u.inter_s.inter_sys_quan.gsm_rpt_quan.rssi_rpt_ind =
            rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
            interRATReportingQuantity.ratSpecificInfo.u.gsm->gsm_Carrier_RSSI;
        }

        if(FAILURE == tdsrrcirat_process_mcm_report_criteria(&rrc_mcm_ptr->measurementCommand.u.modify->
          measurementType.u.interRATMeasurement->reportCriteria,
          &l1_inter_rat_meas_params.meas_object.u.inter_s, &mcm_rpt_crit, tdstransaction_id))
        {
          return FAILURE;
        }
        l1_inter_rat_meas_params.reporting_mode.rpt_mode =
        l1_inter_rat_meas_params.meas_object.u.inter_s.rpt_criteria.rpt_mode;
        
        l1_inter_rat_meas_params.meas_object_info_included = TRUE;
        if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
          tdsrrc_MeasurementCommand_r9_modify,measurementType))
          && (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects,
           rrc_InterRATMeasurement_r9_interRATMeasurementObjects,interRATCellInfoList)) )
        {
          /* Time to process the cell list */
          if (tdsrrcirat_process_cell_list_mcm_r6(rrc_mcm_ptr->measurementCommand.
            u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects.u.interRATCellInfoList,
            &l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list)
            == FAILURE)
          {
            /* Send Meas Ctrl Failure Msg */
            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;
  
            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_protocolError;
  
            /* Set Error Cause */
            rrcmeas_mcf.error_cause  = tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended;
  
            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
  
            return FAILURE;
          }
  
          /* Set Cell list is present */
          l1_inter_rat_meas_params.meas_object.u.inter_s.cell_list_incl = TRUE;
        }
      }

      /* Check for additional measurements */
      /* Call fn which makes check for additional measurements */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr, tdsrrc_MeasurementControl_r9_IEs,additionalMeasurementList))
      {
        if (tdsrrcmeas_validate_additional_meas(&rrc_mcm_ptr->additionalMeasurementList, tdstransaction_id,
          &primary_meas_type, rrc_mcm_ptr->measurementIdentity) == FALSE)
        {
          /* Called function has already sent Meas Ctrl Failure */
          return FAILURE;
        }
        else
        {
          if (tdsrrcmeas_check_mcm_for_setup_modify_additional(
            TRUE, FALSE, TDSINTER_SYS, rrc_mcm_ptr->measurementIdentity,
            &rrc_mcm_ptr->additionalMeasurementList) == FALSE)
          {
            /* As UTRAN tried to configure more than 4 Addtl Meas */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending Meas Ctrl Failure");

            rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

            /* Set error t */
            rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

            /* Send MCF msg to UTRAN */
            tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

            return FAILURE;

          }
          tdsrrcmeas_set_additional_meas_in_db(&primary_meas_type,&rrc_mcm_ptr->additionalMeasurementList,
            rrc_mcm_ptr->measurementIdentity);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Additional Meas set in RRC DB");
        }
      }


      /* Set reporting criteria in RRC DB */
      for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
      {
        if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
        {
          if(mcm_rpt_crit != TDSNOT_AVAILABLE)
          {      
            tdsinter_rat_meas_id_list[count].rpt_crit = mcm_rpt_crit;
            tdsinter_rat_meas_id_list[count].interrat_type = interrat_type;
            tdsinter_rat_meas_id_list[count].rpt_bsic_verified = bsic_verify;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reportinf Crit set in RRC DB");
          }
          break;
        }
      }
      
      if( (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
         tdsrrc_MeasurementCommand_r9_modify,measurementType))
        && (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->interRATMeasurementObjects,
         rrc_InterRATMeasurement_r8_interRATMeasurementObjects,interRATCellInfoList)) )
      {
        /* At this point everything has been validated and now is the time
        to update the inter rat CELL_INFO_LIST */
        for (count=0; count < TDSL1_MAX_CELL_MEAS; count++)
        {
          memscpy(&tdsinter_rat_cell_list[count], sizeof(tdsinter_rat_cell_list_struct_type),
		  &tdslocal_neighbor_list[count], sizeof(tdsinter_rat_cell_list_struct_type));
        }
      }

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
      if(TDSRRCMEAS_IRAT_EUTRA == interrat_type)
      {
        /*Copy the updated frequency list*/
        memscpy(tdsrrcirat_eutra_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(tdsrrcirat_eutra_freq_type),
		temp_freq_list,TDSL1_MAX_EUTRA_FREQ * sizeof(tdsrrcirat_eutra_freq_type));
      }
#endif

    } /* End of else of meas type present */

  } /* End of else if of modify */
  else
  {
  /* Delete chosen. It has already been verified in rrcmeas_find_owner
  * that this meas exists. So let us delete it from tdsinter_rat_meas_id_list
    * Meas Identity, Transaction Id already set */
    l1_inter_rat_meas_params.meas_cmd = TDSL1_MEAS_RELEASE;

    /* Delete addtl meas if any from additional DB */
    tdsrrcmeas_delete_addtl_meas(&l1_inter_rat_meas_params.meas_id);

    /* Clear Meas Id from tdsinter_rat_meas_id_list List */
    for (count=0; count < TDSMAX_INTER_RAT_MEAS; count++)
    {
      if (tdsinter_rat_meas_id_list[count].meas_id == l1_inter_rat_meas_params.meas_id)
      {
        tdsinter_rat_meas_id_list[count].meas_id = TDSINVALID_MEAS_ID;

        tdsinter_rat_meas_id_list[count].rpt_bsic_verified = FALSE;

        /* Also remove any Addtl Meas associated with it */
        for (cnt = 0; cnt < TDSMAX_ADDITIONAL_MEAS; cnt++)
        {
          tdsinter_rat_meas_id_list[count].additional_meas[cnt] = TDSINVALID_MEAS_ID;
        }
        break; /* Come out of for */
      }
    }
  } /* End of else of release */

    /* Transaction Id would be stored in transaction list by the calling function
  if success is returned. Measurement Id has already been stored */


  if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r9_setup))
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement,
        interRATMeasurementObjects)))
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
          interRATMeasurementObjects,rrc_InterRATMeasurement_r9_interRATMeasurementObjects,interRATCellInfoList))
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind = rrc_mcm_ptr->measurementCommand.u.setup->u.interRATMeasurement->
          interRATMeasurementObjects.u.interRATCellInfoList->interRATCellInfoIndication_r6;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }

  if(TDSRRC_CHECK_COMMON_MSG_TYPE(rrc_mcm_ptr->measurementCommand,rrc_MeasurementCommand_r9_modify))
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_mcm_ptr->measurementCommand.u.modify,
        tdsrrc_MeasurementCommand_r9_modify,measurementType)) &&
       (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement,
        interRATMeasurementObjects)))
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE2(rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.interRATMeasurement->
        interRATMeasurementObjects,rrc_InterRATMeasurement_r9_interRATMeasurementObjects,interRATCellInfoList))
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present = TRUE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind = rrc_mcm_ptr->measurementCommand.u.modify->measurementType.u.
          interRATMeasurement->interRATMeasurementObjects.u.interRATCellInfoList->interRATCellInfoIndication_r6;
      }
      else
      {
        tdsinterRAT_cell_info_status.interRAT_cell_info_present=FALSE;
        tdsinterRAT_cell_info_status.interRAT_cell_info_ind=0;
      }
    }
  }


#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
  if(TDSCHECK_OPTIONAL_FEATURE_ENABLE_DMO)
  {
    if(TRUE == rrc_mcm_ptr->m.cellDCHMeasOccasionInfo_TDD128Present)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: cellDCHMeasOccasionInfo_TDD128 IE present and supported!");

      switch(tdsrrcllc_get_dmo_r9(&rrc_mcm_ptr->cellDCHMeasOccasionInfo_TDD128,
                            &l1_inter_rat_meas_params.meas_occasion_info))
      {
        case TDSDMO_mlength_k_chk_Unsupported_Config:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: tdsrrcllc_get_dmo_r9 return unsupport failure");

        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;

      case TDSDMO_mlength_k_chk_Invalid_Config:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: tdsrrcllc_get_dmo_r9 return invalid failure");

        /* Send Meas Ctrl Failure Msg */
        rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

        /* Set error t */
        rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_invalidConfiguration;

        /* Send MCF msg to UTRAN */
        tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);
        return FAILURE;

      case TDSDMO_mlength_k_chk_SUCCESS:
        break;
      default:
        break;
      }

      l1_inter_rat_meas_params.meas_occasion_info_incl = TRUE;
      /* save in CC, in order to revert old config, send to L1 old config */
      tdscurrent_config_ptr->meas_occasion_info_incl = TRUE;

      memscpy(&tdscurrent_config_ptr->meas_occasion_info, sizeof(tdsl1_meas_occasion_info_struct_type),
              &l1_inter_rat_meas_params.meas_occasion_info, sizeof(tdsl1_meas_occasion_info_struct_type));
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: cellDCHMeasOccasionInfo_TDD128 IE is not present!");
      l1_inter_rat_meas_params.meas_occasion_info_incl = FALSE;
      tdscurrent_config_ptr->meas_occasion_info_incl = FALSE;
    }
  }
  else
  {
#endif
        if(TDSRRCRB_R9_MSG_IE_PRESENT(rrc_mcm_ptr,tdsrrc_MeasurementControl_r9_IEs,cellDCHMeasOccasionInfo_TDD128))
    {
#ifdef FEATURE_TDSCDMA_REL9_NO_T2L_CONN_MEAS
      /* Return failure when conditional ignore DMO NV is set and some statusFlag are active.
         Ignore DMO config in other cases. */
      if((TDSCHECK_OPTIONAL_FEATURE_CONDITIONAL_IGNORE_DMO) &&
       (tdsrrcmeas_check_all_statusFlag_deact_in_DMO_config(&rrc_mcm_ptr->cellDCHMeasOccasionInfo_TDD128) == FALSE))
    {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: FEATURE_CONDITIONAL_IGNORE_DMO enabled and some statusFlag in DMO config are activate");
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: cellDCHMeasOccasionInfo_TDD128 IE not supported yet!");
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
    }
    else
    {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: CONDITIONAL_IGNORE_DMO disabled or all statusFlag deactivate,ignore R9 DMO config");
      }
#else
      /* FEATURE_TDSCDMA_REL9_NO_T2L_CONN_MEAS not defined, return failure directly as before */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: cellDCHMeasOccasionInfo_TDD128 IE not supported yet!");
      /* Send Meas Ctrl Failure Msg */
      rrcmeas_mcf.rrc_transaction_id = tdstransaction_id;

      /* Set error t */
      rrcmeas_mcf.error_choice = T_tdsrrc_FailureCauseWithProtErr_configurationUnsupported;

      /* Send MCF msg to UTRAN */
      tdsrrcmeas_send_meas_ctrl_failure_msg(&rrcmeas_mcf);

      return FAILURE;
#endif
    }
#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
  }
#endif

  if(!tdsprocess_interfreq_interrat_meas_rel7)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rel7: Skip sending interRAT measurements from MCM to L1");
    return SUCCESS;
  }

  alloc_idx = tdsrrcmeas_get_int_trans_index_for_trans_id(tdstransaction_id);
  l1_inter_rat_meas_params.trans_id = alloc_idx;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MCM:Sending cmd to L1");
  tdsrrcmeas_send_l1_cmd(NULL, &l1_inter_rat_meas_params, NULL, TDSL1_MEAS_CTRL_PARMS);

  return SUCCESS;
}

 #endif/*#ifdef FEATURE_TDSCDMA_REL9*/



/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
            G E R A N  A R B I T R A T O R  C X M  I N T E R F A C E

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_arbitrator_cxm.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
25/04/18   rv        CR2225983 - GERAN F3 and log pkt Reduction TA.3.0
11/04/17   rv        CR2031827 Excessive F3 log reduction for Oppo
24/08/16    sp       CR1057931: Use dynamic memory allocation for cxm_timing_info_s
07/04/16   snjv      CR1000205 Retrieve Rx/Tx Channel Id based on Rx/Tx client id
29/10/17   km        CR2133917 GL1 support for L+L
12/05/16   rv        CR998395: Excessive F3 logging in  GERAN L1 
06/11/15   ab        CR927639 : CXM In QDTA QDA use desense_band_id as freq id to avoid X blanking.
23/07/15   pg        CR877745 Reorder multisim FW RF init to reduce critical path through RF wakeup 
22/07/15   ap        CR877707: Use TRM_RX_BEST/TRM_RX_ANY/TRM_TX_BEST instead of TRM_RXTX_ANY and TRM_RXTX_BEST 
17/07/15   mc        CR871205 : Incorrect CXM Channel ID assigned to W2G activities in W+W mode
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
03/07/15   mc        CR855818 : COEX:MCS_CXM_WWAN_TECH_STATE_IND registrations for X2G IRAT need to indicate CXM_LNK_TYPE_IRAT
23/06/15   shr       CR859975  GL1 should not use Default Device RFM_DEVICE_0 when obtaining channel_id
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
27/05/15   mc        CR.840262 : COEX handling for Dual-Multimode DR-DSDS
07/05/15   mc        CR830988 : COEX Channel ID selection for X2G IRAT measurements does not support TRM_UMTS2
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
07/04/15   shr       CR808932: COEX support for DTM
07/04/15   og        CR814507. Further stack reduction in the GSM L1 ISR.
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
27/02/15   am        CR646584 WLAN coex
18/12/14   cgc       CR766576 FR23109 Adaptive Power monitor during CS in DSDA G + TDD ( TDS/LTE TDD)
15/01/15   shr       CR765883 COEX: Fix "No CXM freqid available for DL ARFCN" log 
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
03/12/14   npt       CR759467 Fix compiler warnings
13/11/14   npt       CR746180 Remove KW errors 
26/09/14   sjv       CR731259 Correct device id for G2X IRAT, trm_get_info is a union, make sure to use 1 member of this
24/09/14   cah      CR729408: Updates to reduce number of TRM API calls in Gl1. 
12/09/14   am        CR642120 Use right client in trm_get_device_mapping()
23/08/14   mc        CR707745 : [BOLT2.0][SG+G]: Incorrect CXM priority assignment policy in use for SGLTE+G
08/05/14   ws        CR661576 GL1 Support for sending MCS_CXM_BAND_AVOID_PWR_IND
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
26/06/14   ab        CR 682000 - COEX - GL1 to update ARFCN if they are not registered when start start is next radio block
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
10/06/14   jj        CR 668965 merge ASDIV from DI.3.0 to BO 2.0
18/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
11/04/14   pg        CR645875 Fix Bolt multisim compilation errors
29/03/14   mc/ab     CR640829 : COEX : Use correct Channel ID for X2G IRAT, DSDS
26/03/14   ab        CR639679: Correct CR 639086 :Ensure in DSDS/GBTA that CXM channel IDs get setup.
26/03/14   dv        CR639086: Ensure in GBTA that CXM channel IDs get setup.
03/03/14   ab        CR625312: COEX - GL1 to update MCS/COEX ARFCN/Band Info for Power Monitors.
13/03/14   mc        CR602368: COEX : Support for Frequency Hopping over large ARFCN lists
10/03/14   ab        CR629382 Unknown Freq Id in FIND_BCCH State leads to C/W blanking
19/02/14   ap        CR600310 GSM L1 to send channel Id to GSM FW corresponding to the primary RF device
14/02/14   mc        CR.572523 : COEX : Ensure all DL bands are registered with CXM on transition to FIND_BCCH
14/01/14   og        CR589868. T+G DSDA support.
20/01/14   ab        CR582408: DSDA - Update Rx Power in IDLE and Do not Update Power when RX OFF
19/11/13   og        CR552366. Use the correct rf device id.
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
04/11/13   mc        CR556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
07/11/13   og        CR565881. Reduce the number of power scan monitors per frame
                     from 7 to 2 when WCDMA is in dedicated mode.
01/11/13   ab        CR.568623 : COEX : DSDA - update Rx Tx power for COEX with a faster convergence
07/10/13   ab        CR 554820 DSDA GL1 - Update Tx power as per slot transmission for RF COEX
11/10/13   pjr       CR.555300 : Changes to correctly register freqid during monitors  and acq
07/11/13   og        CR565881. Reduce the number of power scan monitors per frame
                     from 7 to 2 when WCDMA is in dedicated mode.
11/10/13   pjr       CR.555300 : Changes to correctly register freqid during monitors  and acq
11/14/13   ab        CR 554820 DSDA GL1 - Update Tx power as per slot transmission for RF COEX
04/10/13   ab        CR.553892 : DSDA COEX - Deregister with MCS-MCS_CXM_WWAN_TECH_STATE_IND when reg_pending=TRUE
01/10/13   sk        CR552259 CR sync from 1.0/1.1
23/09/13   sk        2.0 syncup from 1.1
15/08/13   sk        CR524039 GBTA bringup changes
29/07/13   sk        CR519663 Partial QBTA bringup changes
24/05/13   cs        TSTS Updates
05/04/13   mc        CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13   mc        CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "gl1_arbitrator_cxm.h"

#ifdef FEATURE_GSM_COEX_SW_CXM

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "gl1_arbitrator_utils.h"
#include "gl1_arbitrator_types.h"
#include "gl1_arbitrator_tables.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "l1_utils.h"
#include "l1_isr.h"
#include "gpl1_dual_sim.h"
#include "gpl1_grm_intf.h"
#include "l1_task.h"
#include "gpl1_grm_intf.h"
#include "gprs_mem.h"
/*===========================================================================

                            LOCAL DEFINES

===========================================================================*/
#define CXM_BAND_REG_LINKTYPE      ( CXM_LNK_TYPE_POWER_MONITOR )
#define CXM_BAND_REG_LINKTYPE_IRAT ( CXM_LNK_TYPE_POWER_MONITOR + CXM_LNK_TYPE_IRAT )

extern garb_arbitrated_gas_t arbitrated_gas[NUM_GERAN_DATA_SPACES];

/*===========================================================================

                     PROTOTYPES FOR LOCAL FUNCTIONS

===========================================================================*/
static void garb_cxm_priority_table_callback(cxm_tech_type tech_id, cxm_activity_table_s activity_tbl[MAX_ACTIVITY_TIERS]);

/*===========================================================================

FUNCTION      garb_cxm_register_priority_table_callback

DESCRIPTION   Registers the priority callback function with CXM. See the
              callback funtion for more info.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_register_priority_table_callback(gas_id_t gas_id)
{
#ifdef FEATURE_GARB_QTF /* GARB TODO : Implement QTF stubs for SW CXM */
   cxm_activity_table_s activity_tbl[MAX_ACTIVITY_TIERS] = { {ACTIVITY_TIER_3, 1000}, {ACTIVITY_TIER_5, 500}, {ACTIVITY_TIER_10, 100},
      {ACTIVITY_TIER_15, 80}, {ACTIVITY_TIER_20, 10}, {ACTIVITY_TIER_25, 5} };
   garb_cxm_priority_table_callback(geran_map_gas_id_to_cxm_id( gas_id ), activity_tbl);
#endif /* FEATURE_GARB_QTF */
   cxm_register_prio_table_cb( geran_map_gas_id_to_cxm_id( gas_id ), garb_cxm_priority_table_callback );
}

/*===========================================================================

FUNCTION      garb_cxm_deregister_priority_table_callback

DESCRIPTION   De-registers the priority callback function with CXM. See
              the callback funtion for more info.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_deregister_priority_table_callback(gas_id_t gas_id)
{
   cxm_register_prio_table_cb( geran_map_gas_id_to_cxm_id( gas_id ), NULL );
}

/*===========================================================================

FUNCTION      garb_cxm_priority_table_callback

DESCRIPTION   This callback is used by CXM to notify GL1 of an update to its
              priority table. Updates can be triggered dynamically, for e.g.
              if an active voice call is placed on hold GL1 may be required
              to use lower CXM priorities.

RETURN VALUE  None

===========================================================================*/
static void garb_cxm_priority_table_callback(cxm_tech_type tech_id, cxm_activity_table_s activity_tbl[MAX_ACTIVITY_TIERS])
{
   gas_id_t gas_id = gl1_tech_id_to_gas_id( tech_id );

   if(gas_id!=GERAN_ACCESS_STRATUM_ID_UNDEFINED)
   {
      arbitrated_gas[gas_id].cxm_activity_table[ACTIVITY_TIER_3]  = activity_tbl[ACTIVITY_TIER_3];
      arbitrated_gas[gas_id].cxm_activity_table[ACTIVITY_TIER_5]  = activity_tbl[ACTIVITY_TIER_5];
      arbitrated_gas[gas_id].cxm_activity_table[ACTIVITY_TIER_10] = activity_tbl[ACTIVITY_TIER_10];
      arbitrated_gas[gas_id].cxm_activity_table[ACTIVITY_TIER_15] = activity_tbl[ACTIVITY_TIER_15];
      arbitrated_gas[gas_id].cxm_activity_table[ACTIVITY_TIER_20] = activity_tbl[ACTIVITY_TIER_20];
      arbitrated_gas[gas_id].cxm_activity_table[ACTIVITY_TIER_25] = activity_tbl[ACTIVITY_TIER_25];

      MSG_GERAN_HIGH_3_G("GARB <- CXM : CXM PRIORITY CB T10=%d T15=%d T20=%d",
         activity_tbl[ACTIVITY_TIER_10].priority, activity_tbl[ACTIVITY_TIER_15].priority, activity_tbl[ACTIVITY_TIER_20].priority);
   }
}

/*===========================================================================

FUNCTION      garb_cxm_arfcn_indication

DESCRIPTION   Notify CXM of our frequency list. The CXM supports a maximum
              of 24 registered ARFCNs at any time. Note that for each ARFCN
              UL and DL are handled separately and are assigned different
              CXM freq IDs.

              For CS and PS assignments, if there are 10 or less ARFCNs in our
              assigned frequency list then we can do a one-shot registration
              with CXM which will be good for the lifetime of the assignment.
              If however there are more than 10 ARFCNs in the assigned
              frequency list then we need to periodically update CXM with a
              new list of registered ARFCNs.

              This function sends MCS_CXM_WWAN_TECH_STATE_IND to CXM.

RETURN VALUE  None

===========================================================================*/
void  garb_cxm_arfcn_indication( ARFCN_T*                 arfcns,
                                 uint8                    num_arfcns,
                                 cxm_tech_link_direction  direction,
                                 boolean                  register_bands,
                                 gas_id_t                 gas_id
                                )
{
   cxm_wwan_tech_state_ind_s *msg = NULL;
   /*this is a union*/
   trm_get_info_data         get_info_input;
   grm_client_enum_t         requested_client_id;
   cxm_rfdev_type            rf_device;

   errno_enum_type           msgr_error;

#ifdef FEATURE_GARB_QTF /* GARB TODO : Implement QTF stubs for SW CXM */
   cxm_freqid_info_ind_s cxm_freqid_info_ind;
#endif /* FEATURE_GARB_QTF */

   int i, num_link_info_sets = 0;

   msg = GPRS_MEM_MALLOC(sizeof(cxm_wwan_tech_state_ind_s));
  if(msg == NULL) 
  {    
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_FATAL("mem alloc failed",0,0,0);
  }

   
   memset(msg, 0, sizeof(cxm_wwan_tech_state_ind_s));

   msgr_init_hdr((msgr_hdr_struct_type*)msg, MSGR_GERAN_GL1,  MCS_CXM_WWAN_TECH_STATE_IND);

   msg->tech_id = geran_map_gas_id_to_cxm_id( gas_id );
   /* For full DR-DSDS support need to scan through all ARCN's in list and
    * add them into band_mapping_info data
    */
   get_info_input.band_mapping_info.band[0]   = SYS_BAND_GSM_EGSM_900;
   get_info_input.band_mapping_info.num_bands = 1;

   if (msg->tech_id == CXM_TECH_GSM1)
   {
      requested_client_id =  GRM_GSM1;

#if defined(FEATURE_SGLTE_DUAL_SIM ) || defined(FEATURE_SVLTE_DUAL_SIM )
      get_info_input.band_mapping_info.resource = TRM_RX_BEST_POSSIBLE;
#else
      get_info_input.band_mapping_info.resource =  TRM_RX_BEST;
#endif
   }
   else
   {
     if (msg->tech_id == CXM_TECH_GSM2)
     {
       requested_client_id = GRM_GSM2;
     }
#ifdef FEATURE_TRIPLE_SIM
     else if (msg->tech_id == CXM_TECH_GSM3)
     {
       requested_client_id = GRM_GSM3;
     }
#endif     
     else
     {
       requested_client_id =  GRM_GSM1;
       MSG_GERAN_ERROR_1_G("tech_id %d not supported", msg->tech_id);
     }

#if defined(FEATURE_SGLTE_DUAL_SIM ) || defined(FEATURE_SVLTE_DUAL_SIM )
      get_info_input.band_mapping_info.resource =  TRM_RX_BEST_POSSIBLE;
#else
      get_info_input.band_mapping_info.resource =  TRM_RXTX_BEST_SMODE;
#endif
   }

   rf_device = grm_get_device_mapping( requested_client_id, &get_info_input, gas_id );

   msgr_init_hdr((msgr_hdr_struct_type*)msg, MSGR_GERAN_GL1,  MCS_CXM_WWAN_TECH_STATE_IND);

   msg->tech_id = geran_map_gas_id_to_cxm_id( gas_id );

   /* Register Serving Cells ARFCNs*/
   for(i=0; (i<MAX_CXM_FREQIDS) && (i < num_arfcns); i++)
   {
      if( (direction==CXM_LNK_DRCTN_UL) || (direction==CXM_LNK_DRCTN_UL_AND_DL) )
      {
         msg->tech_data.link_list[num_link_info_sets].frequency = garb_arfcn_to_centre_freq_kHz(arfcns[i], GARB_TX, gas_id);
         msg->tech_data.link_list[num_link_info_sets].bandwidth = 200000; /* 200 kHz */;
         msg->tech_data.link_list[num_link_info_sets].band      = gl1_map_gsm_band_to_sys_band(arfcns[i].band);
         msg->tech_data.link_list[num_link_info_sets].channel   = arfcns[i].num;
         msg->tech_data.link_list[num_link_info_sets].rf_device = rf_device;
         msg->tech_data.link_list[num_link_info_sets].direction = CXM_LNK_DRCTN_UL;

         num_link_info_sets++;
      }

      if( (direction==CXM_LNK_DRCTN_DL) || (direction==CXM_LNK_DRCTN_UL_AND_DL) )
      {
         msg->tech_data.link_list[num_link_info_sets].frequency = garb_arfcn_to_centre_freq_kHz(arfcns[i], GARB_RX, gas_id);
         msg->tech_data.link_list[num_link_info_sets].bandwidth = 200000; /* 200 kHz */;
         msg->tech_data.link_list[num_link_info_sets].band      = gl1_map_gsm_band_to_sys_band(arfcns[i].band);
         msg->tech_data.link_list[num_link_info_sets].channel   = arfcns[i].num;
         msg->tech_data.link_list[num_link_info_sets].rf_device = rf_device;
         msg->tech_data.link_list[num_link_info_sets].direction = CXM_LNK_DRCTN_DL;

         num_link_info_sets++;
      }
   }

   /* Register GSM bands for NCELLS and ACQ */
   if(register_bands)
   {
      cxm_tech_link_type link_type = arbitrated_gas[gas_id].irat ? CXM_BAND_REG_LINKTYPE_IRAT : CXM_BAND_REG_LINKTYPE;

      /* SYS_BAND_DCS_1800 */
      msg->tech_data.link_list[num_link_info_sets].frequency = 1842500; /* 1842.5 MHz */
      msg->tech_data.link_list[num_link_info_sets].bandwidth = 74600000; /* 74.6 MHz */
      msg->tech_data.link_list[num_link_info_sets].band      = SYS_BAND_GSM_DCS_1800;
      msg->tech_data.link_list[num_link_info_sets].channel   = GARB_UNKNOWN_ARFCN;

      msg->tech_data.link_list[num_link_info_sets].rf_device = rf_device;
      msg->tech_data.link_list[num_link_info_sets].direction = CXM_LNK_DRCTN_DL;
      msg->tech_data.link_list[num_link_info_sets].type      = link_type;
      num_link_info_sets++;

      /* SYS_BAND_PCS_1900 */
      msg->tech_data.link_list[num_link_info_sets].frequency = 1960000; /* 19600 MHz */
      msg->tech_data.link_list[num_link_info_sets].bandwidth = 59600000; /* 59.6 MHz */
      msg->tech_data.link_list[num_link_info_sets].band      = SYS_BAND_GSM_PCS_1900;
      msg->tech_data.link_list[num_link_info_sets].channel   = GARB_UNKNOWN_ARFCN;
      msg->tech_data.link_list[num_link_info_sets].rf_device = rf_device;
      msg->tech_data.link_list[num_link_info_sets].direction = CXM_LNK_DRCTN_DL;
      msg->tech_data.link_list[num_link_info_sets].type      = link_type;
      num_link_info_sets++;

      /* SYS_BAND_CELL_850 */
      msg->tech_data.link_list[num_link_info_sets].frequency = 881700; /* 881.7 MHz */
      msg->tech_data.link_list[num_link_info_sets].bandwidth = 25000000; /* 25.0 MHz */
      msg->tech_data.link_list[num_link_info_sets].band      = SYS_BAND_GSM_850;
      msg->tech_data.link_list[num_link_info_sets].channel   = GARB_UNKNOWN_ARFCN;
      msg->tech_data.link_list[num_link_info_sets].rf_device = rf_device;
      msg->tech_data.link_list[num_link_info_sets].direction = CXM_LNK_DRCTN_DL;
      msg->tech_data.link_list[num_link_info_sets].type      = link_type;
      num_link_info_sets++;

      /* SYS_BAND_EGSM_900 */
      msg->tech_data.link_list[num_link_info_sets].frequency = 942500; /* 942.5 MHz */
      msg->tech_data.link_list[num_link_info_sets].bandwidth = 35000000; /* 35.0 MHz */
      msg->tech_data.link_list[num_link_info_sets].band      = SYS_BAND_GSM_EGSM_900;
      msg->tech_data.link_list[num_link_info_sets].channel   = GARB_UNKNOWN_ARFCN;
      msg->tech_data.link_list[num_link_info_sets].rf_device = rf_device;
      msg->tech_data.link_list[num_link_info_sets].direction = CXM_LNK_DRCTN_DL;
      msg->tech_data.link_list[num_link_info_sets].type      = link_type;
      num_link_info_sets++;

      MSG_GERAN_HIGH_2_G("GARB -> CXM : FN=%d CXM FREQ IND for %d SCELL ARFCNS and 4 DL Bands", GSTMR_GET_FN_GERAN(gas_id), num_arfcns);
   }
   else
   {
      MSG_GERAN_HIGH_2_G("GARB -> CXM : FN=%d CXM FREQ IND for %d SCELL ARFCNS and 0 DL Bands", GSTMR_GET_FN_GERAN(gas_id), num_arfcns);
   }

   msg->tech_data.num_link_info_sets = num_link_info_sets;

   msgr_error = msgr_send( (msgr_hdr_struct_type *) msg, sizeof(cxm_wwan_tech_state_ind_s) );

   if (msgr_error != E_SUCCESS)
   {
      MSG_GERAN_ERROR_0_G("GARB->CXM MSGR send failed");
   }

#ifdef FEATURE_GARB_QTF /* GARB TODO : Implement QTF stubs for SW CXM */
   msgr_error = E_SUCCESS;

   for(i=0;i<msg->tech_data.num_link_info_sets;i++)
   {
      cxm_freqid_info_ind.freq_data.link_list[i].link_info.direction = msg->tech_data.link_list[i].direction;
      cxm_freqid_info_ind.freq_data.link_list[i].link_info.band      = msg->tech_data.link_list[i].band;
      cxm_freqid_info_ind.freq_data.link_list[i].link_info.channel   = msg->tech_data.link_list[i].channel;
      cxm_freqid_info_ind.freq_data.link_list[i].freqid              = msg->tech_data.link_list[i].channel;
   }

   cxm_freqid_info_ind.freq_data.num_link_info_sets = msg->tech_data.num_link_info_sets;

   garb_cxm_notify_freqid_list(&cxm_freqid_info_ind, gas_id);
#endif /* FEATURE_GARB_QTF */

   GPRS_MEM_FREE(msg);
   msg = NULL;
}

/*===========================================================================

FUNCTION      garb_cxm_notify_freqid_list

DESCRIPTION   Notifies GL1 of the list of CXM freqids to be associated with
              the reported ARFCNs.

              This function is called from L1 Task on receipt of an
              MCS_CXM_FREQID_LIST_GSM sent by CXM in response to a
              MCS_CXM_WWAN_TECH_STATE_IND.

              MCS_CXM_COEX_POWER_IND is sent to notify CXM of the expected
              power on the registerered ARFCNs

RETURN VALUE  None

===========================================================================*/
void garb_cxm_notify_freqid_list(cxm_freqid_info_ind_s* msg, gas_id_t gas_id)
{
   sys_band_T band;

   int link;

   memset(&arbitrated_gas[gas_id].ul_arfcn_table, 0, sizeof(garb_arfcn_table_t));
   memset(&arbitrated_gas[gas_id].dl_arfcn_table, 0, sizeof(garb_arfcn_table_t));
   memset(&arbitrated_gas[gas_id].dl_band_table,  0, sizeof(uint32[INVALID_BAND]));

   for(link=0;link<msg->freq_data.num_link_info_sets;link++)
   {
      band = gl1_map_sys_band_to_gsm_band(msg->freq_data.link_list[link].link_info.band);

      if((msg->freq_data.link_list[link].link_info.channel   == GARB_UNKNOWN_ARFCN) &&
         (msg->freq_data.link_list[link].link_info.direction == CXM_LNK_DRCTN_DL) && (band < INVALID_BAND))
      {
         /* get dl freq id (band) */
         arbitrated_gas[gas_id].dl_band_table[band] = msg->freq_data.link_list[link].freqid;

         MSG_GERAN_DEBUG_OPT_2_G("GARB : CXM DL Band=%d Freq ID = %u", msg->freq_data.link_list[link].link_info.band, msg->freq_data.link_list[link].freqid);
      }
      else if( (msg->freq_data.link_list[link].link_info.direction==CXM_LNK_DRCTN_UL) && (band < INVALID_BAND) )
      {
         /* get ul freq id (arfcn) */
         garb_arfcn_table_t* arfcn_table_ptr = &arbitrated_gas[gas_id].ul_arfcn_table;

         arfcn_table_ptr->arfcns[arfcn_table_ptr->num_arfcns].arfcn      = msg->freq_data.link_list[link].link_info.channel;
         arfcn_table_ptr->arfcns[arfcn_table_ptr->num_arfcns].cxm_freqid = msg->freq_data.link_list[link].freqid;

         MSG_GERAN_DEBUG_OPT_2_G("GARB : CXM UL ARFCN=%d Freq ID = %u", msg->freq_data.link_list[link].link_info.channel, msg->freq_data.link_list[link].freqid);

         arfcn_table_ptr->num_arfcns++;
      }
      else if( (msg->freq_data.link_list[link].link_info.direction==CXM_LNK_DRCTN_DL) && (band < INVALID_BAND) )
      {
         /* get dl freq id (arfcn) */
         garb_arfcn_table_t* arfcn_table_ptr = &arbitrated_gas[gas_id].dl_arfcn_table;

         arfcn_table_ptr->arfcns[arfcn_table_ptr->num_arfcns].arfcn      = msg->freq_data.link_list[link].link_info.channel;
         arfcn_table_ptr->arfcns[arfcn_table_ptr->num_arfcns].cxm_freqid = msg->freq_data.link_list[link].freqid;

         MSG_GERAN_DEBUG_OPT_2_G("GARB : CXM DL ARFCN=%d Freq ID = %u", msg->freq_data.link_list[link].link_info.channel, msg->freq_data.link_list[link].freqid);

         arfcn_table_ptr->num_arfcns++;
      }
   }

   garb_cxm_power_indication(gas_id);

   /* Band Power Udpate for BAND Registration*/
   switch ( arbitrated_gas[gas_id].state)
   {
      case GARB_IDLE:
        /* For QDTA , band id is used for PCH/Monitors , thus having an unknown power ,results 
         * in X TX blanking in the 1st DRX period. Reporting SCELL PCH power serves the purpose.
           Accurate band power UPDATE is performed at the time of RX/MON scheduling.
         */
        garb_cxm_band_power_ind(arbitrated_gas[gas_id].params_idle.frequency_list.channel[0].band,
                                (arbitrated_gas[gas_id].params_idle.rx_power_dbm10[0] << 4)/10,
                                 gas_id);

        /* check if interband cells present */
        if (arbitrated_gas[gas_id].params_idle.interband != INVALID_BAND)
        {
          /* Report -100dbm to avoid TX blanking  as a result of UNKNOWN_POWER */
          garb_cxm_band_power_ind(arbitrated_gas[gas_id].params_idle.interband,
                                  SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16_COEX,
                                  gas_id);
        }
        break;
     
       default:
        break;
    }

   MSG_GERAN_LOW_2_G("GARB <- CXM FN=%d CXM NOTIFY FREQ LIST for %d ARFCNS", GSTMR_GET_FN_GERAN(gas_id), msg->freq_data.num_link_info_sets);
}

/*===========================================================================

FUNCTION      garb_cxm_power_indication_arfcn

DESCRIPTION   Notify the CXM of our updated average UL and DL power for
              each serving cell FOR ONLY one ARFCN

              This function sends MCS_CXM_COEX_POWER_IND to CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_power_indication_arfcn(ARFCN_T arfcn,int32 rx_power_dbm10,gas_id_t gas_id)
{
   cxm_coex_power_ind_s *msg = NULL;
   errno_enum_type msgr_error;
   boolean freq_id_match = FALSE;
   uint8 i;

   for(i=0;i<arbitrated_gas[gas_id].dl_arfcn_table.num_arfcns;i++)
   {
     if(arbitrated_gas[gas_id].dl_arfcn_table.arfcns[i].arfcn == arfcn.num)
     {
       freq_id_match = TRUE;
       break; // i is the index to fetch freq id
     }
   }

   if (freq_id_match)
   {
     msg = GPRS_MEM_MALLOC(sizeof(cxm_coex_power_ind_s));
     if(msg == NULL) 
     {   
#ifdef FEATURE_QSH_MDUMP
          QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
          ERR_FATAL("mem alloc failed",0,0,0);
     }


     memset(msg, 0, sizeof(cxm_coex_power_ind_s));

     msgr_init_hdr((msgr_hdr_struct_type*)msg, MSGR_GERAN_GL1,  MCS_CXM_COEX_POWER_IND);

     msg->tech_id = geran_map_gas_id_to_cxm_id( gas_id );
     
     msg->power_data.link_list[0].rx_power  = rx_power_dbm10;
     msg->power_data.link_list[0].direction = CXM_LNK_DRCTN_DL;
     msg->power_data.link_list[0].freqid    = arbitrated_gas[gas_id].dl_arfcn_table.arfcns[i].cxm_freqid;
     
     msg->power_data.num_link_info_sets = 1;

     if (msg->power_data.num_link_info_sets)
     {
        msgr_error = msgr_send( (msgr_hdr_struct_type *) msg, sizeof(cxm_coex_power_ind_s) );

        if (msgr_error != E_SUCCESS)
        {
           MSG_GERAN_ERROR_0_G("GARB->CXM MSGR send failed");
        }

        MSG_GERAN_LOW_2_G("GARB -> CXM : MCS_CXM_COEX_POWER_IND DL=%ddBm10 linksets=%d",  rx_power_dbm10, msg->power_data.num_link_info_sets );
     }
     GPRS_MEM_FREE(msg);
     msg = NULL;
   }
}

/*===========================================================================

FUNCTION      garb_cxm_band_power_ind

DESCRIPTION   Notify the CXM of our updated average DL power at BAND_LEVEL
              

RETURN VALUE  None

===========================================================================*/
void garb_cxm_band_power_ind(sys_band_T band, int32 pwr_dBm_x16, gas_id_t gas_id)
{
   cxm_coex_power_ind_s *msg = NULL;
   errno_enum_type msgr_error;
   int32 rx_power_dBm_x10 = (pwr_dBm_x16 == CXM_UNKNOWN_POWER) ? 
                                CXM_UNKNOWN_POWER:((pwr_dBm_x16*10)>>4);

   if (band == INVALID_BAND)
   {
     return;
   }

   msg = GPRS_MEM_MALLOC(sizeof(cxm_coex_power_ind_s));
  if(msg == NULL) 
  {   
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_FATAL("mem alloc failed",0,0,0);
  }


   memset(msg, 0, sizeof(cxm_coex_power_ind_s));

   msgr_init_hdr((msgr_hdr_struct_type*)msg, MSGR_GERAN_GL1,  MCS_CXM_COEX_POWER_IND);
   msg->tech_id = geran_map_gas_id_to_cxm_id( gas_id );

   msg->power_data.link_list[msg->power_data.num_link_info_sets].rx_power  = rx_power_dBm_x10;
   msg->power_data.link_list[msg->power_data.num_link_info_sets].direction = CXM_LNK_DRCTN_DL;
   msg->power_data.link_list[msg->power_data.num_link_info_sets].freqid    = arbitrated_gas[gas_id].dl_band_table[band];
   msg->power_data.num_link_info_sets = 1;

   if (msg->power_data.num_link_info_sets)
   {
      msgr_error = msgr_send( (msgr_hdr_struct_type *) msg, sizeof(cxm_coex_power_ind_s) );

      if (msgr_error != E_SUCCESS)
      {
         MSG_GERAN_ERROR_0_G("GARB->CXM MSGR send failed");
      }

      MSG_GERAN_HIGH_3_G("GARB -> CXM : MCS_CXM_COEX_POWER_IND Band %d DL =%ddBm10 linksets=%d", 
                         band,rx_power_dBm_x10, msg->power_data.num_link_info_sets);
   }
   GPRS_MEM_FREE(msg);
   msg = NULL;
}

/*===========================================================================

FUNCTION      garb_cxm_power_indication

DESCRIPTION   Notify the CXM of our updated average UL and DL power for
              each serving cell ARFCN.

              This function sends MCS_CXM_COEX_POWER_IND to CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_power_indication(gas_id_t gas_id)
{
   static cxm_coex_power_ind_s  msg[3];

   errno_enum_type msgr_error;

   int32 rx_power = CXM_UNKNOWN_POWER, tx_power = CXM_UNKNOWN_POWER;

   uint8 i = 0;

   if ( arbitrated_gas[gas_id].state == GARB_FIND_BCCH)
   {
     rx_power = garb_get_rx_power_in_find_bcch(gas_id);
   }
   else if (gpl1_get_rx_pwr_avg_active_state(gas_id) || (arbitrated_gas[gas_id].state == GARB_IDLE))
   {
     rx_power = gpl1_get_rx_pwr_avg_dbm10_coex(gas_id);
     arbitrated_gas[gas_id].params_idle.rx_power_dbm10[0] = rx_power; // Serving cell power.
   }
      
   if ((rx_power == CXM_UNKNOWN_POWER) || (rx_power ==  0))
   {
     /* No RX Yet , Just enetered Idle CXM will use previous power report or acquiring bcch  ,  */
      return;
   } 
   
   if(gpl1_get_tx_pwr_avg_active_state(gas_id)) (tx_power = gpl1_get_avg_tx_slot_pwr_db_10(gas_id) );
   
   memset(&msg[gas_id], 0, sizeof(cxm_coex_power_ind_s));

   msgr_init_hdr((msgr_hdr_struct_type*)&msg[gas_id], MSGR_GERAN_GL1,  MCS_CXM_COEX_POWER_IND);

   msg[gas_id].tech_id = geran_map_gas_id_to_cxm_id( gas_id );

   msg[gas_id].power_data.num_link_info_sets = 0;

   for(i=0;i<arbitrated_gas[gas_id].ul_arfcn_table.num_arfcns;i++)
   {
      msg[gas_id].power_data.link_list[msg[gas_id].power_data.num_link_info_sets].tx_power  = tx_power;
      msg[gas_id].power_data.link_list[msg[gas_id].power_data.num_link_info_sets].direction = CXM_LNK_DRCTN_UL;
      msg[gas_id].power_data.link_list[msg[gas_id].power_data.num_link_info_sets].freqid    = arbitrated_gas[gas_id].ul_arfcn_table.arfcns[i].cxm_freqid;

      msg[gas_id].power_data.num_link_info_sets++;
   }

   for(i=0;i<arbitrated_gas[gas_id].dl_arfcn_table.num_arfcns;i++)
   {
      if ( arbitrated_gas[gas_id].state == GARB_FIND_BCCH)
      {
        msg[gas_id].power_data.link_list[msg[gas_id].power_data.num_link_info_sets].rx_power  =
                             arbitrated_gas[gas_id].params_find_bcch.rx_power_dbm10[i];
      }
      else if (arbitrated_gas[gas_id].state == GARB_IDLE)
      {
        msg[gas_id].power_data.link_list[msg[gas_id].power_data.num_link_info_sets].rx_power  = 
                             arbitrated_gas[gas_id].params_idle.rx_power_dbm10[i];
      }
      else 
      {
        msg[gas_id].power_data.link_list[msg[gas_id].power_data.num_link_info_sets].rx_power  = rx_power;
      }
      
      msg[gas_id].power_data.link_list[msg[gas_id].power_data.num_link_info_sets].direction = CXM_LNK_DRCTN_DL;
      msg[gas_id].power_data.link_list[msg[gas_id].power_data.num_link_info_sets].freqid    = arbitrated_gas[gas_id].dl_arfcn_table.arfcns[i].cxm_freqid;

      msg[gas_id].power_data.num_link_info_sets++;
   }

   if (msg[gas_id].power_data.num_link_info_sets)
   {
      msgr_error = msgr_send( (msgr_hdr_struct_type *) &msg[gas_id], sizeof(cxm_coex_power_ind_s) );

      if (msgr_error != E_SUCCESS)
      {
         MSG_GERAN_ERROR_0_G("GARB->CXM MSGR send failed");
      }

      MSG_GERAN_LOW_3_G("GARB -> CXM : MCS_CXM_COEX_POWER_IND UL=%ddBm10 DL=%ddBm10 linksets=%d", tx_power, rx_power, msg[gas_id].power_data.num_link_info_sets );
   }
}

#if defined (FEATURE_GSM_BAND_AVOIDANCE)
/*===========================================================================
FUNCTION      garb_cxm_band_avoid_pwr_ind

DESCRIPTION   Notify the CXM of our updated average UL and DL power for
              TCH ARFCN for Band avoidance purposes

              This function sends MCS_CXM_BAND_AVOID_PWR_IND to CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_band_avoid_pwr_ind(gas_id_t gas_id)
{
   cxm_coex_ba_pwr_ind_s *msg=NULL;

   errno_enum_type msgr_error;

   int32 rx_power = CXM_UNKNOWN_POWER, tx_power = CXM_UNKNOWN_POWER;

   msg = GPRS_MEM_MALLOC(sizeof(cxm_coex_ba_pwr_ind_s));
   if(msg == NULL) 
   {   
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_FATAL("mem alloc failed",0,0,0);
   }

   memset(msg, 0, sizeof(cxm_coex_ba_pwr_ind_s));

   msgr_init_hdr((msgr_hdr_struct_type*)msg, MSGR_GERAN_GL1,  MCS_CXM_BAND_AVOID_PWR_IND);

   msg->serv_pwr_data.tech_id = geran_map_gas_id_to_cxm_id( gas_id );

   
   /* Use same Rx and Tx power for all TCH ARFCN's */
   msg->serv_pwr_data.handle_flag = BA_PWR_LIST_FILL_ALL;
   msg->serv_pwr_data.num_entries = 2;

   if( gpl1_get_rx_pwr_avg_active_state(gas_id) )
   {
     rx_power = gpl1_get_rx_pwr_avg_dbm10_coex(gas_id);
   }

   if(gpl1_get_tx_pwr_avg_active_state(gas_id)) 
   {
     tx_power = gpl1_get_avg_tx_slot_pwr_db_10(gas_id);
   }

   /* Seperated the TX/RX power into seperate entries */
   msg->serv_pwr_data.pwr_list[0].direction = CXM_LNK_DRCTN_DL;
   msg->serv_pwr_data.pwr_list[0].rx_pwr = rx_power;
   msg->serv_pwr_data.pwr_list[1].tx_pwr = CXM_UNKNOWN_POWER;
   
   msg->serv_pwr_data.pwr_list[1].direction = CXM_LNK_DRCTN_UL;
   msg->serv_pwr_data.pwr_list[1].rx_pwr = CXM_UNKNOWN_POWER;
   msg->serv_pwr_data.pwr_list[1].tx_pwr = tx_power;

   msgr_error = msgr_send( (msgr_hdr_struct_type *) msg, sizeof(cxm_coex_ba_pwr_ind_s) );

   MSG_GERAN_LOW_2_G("GARB -> CXM : MCS_CXM_BAND_AVOID_PWR_IND UL=%ddBm10 DL=%ddBm10 linksets=%d", tx_power, rx_power);
   
   GPRS_MEM_FREE(msg);
   msg = NULL;
}
#endif

#ifdef FEATURE_DUAL_SIM
#if defined(FEATURE_GSM_COEX)
/*===========================================================================

FUNCTION      garb_cxm_activity_timeline_indication

DESCRIPTION   Notify CXM of our activity timeline. This is only reported to
              CXM when requested via MCS_CXM_REQUEST_ACTIVITY_INFO_IND. During
              CS or PS calls this is sent initially as a one-shot report with
              subsequent updates sent for e.g. if timing drifts beyond that
              which can be absorbed by the guard period.

              For CS(TCH) and PS the indication is updated every
              GARB_CXM_IND_UPDATE_PERIOD frames to correct for any
              USTMR drift. For RACH and CS(SDCCH) the notification is sent
              single-shot for each UL or DL burst.

              This function sends MCS_CXM_SET_ACTIVITY_TIMELINE_IND to
              CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_activity_timeline_indication(uint8 read_t_index,gas_id_t gas_id)
{
   cxm_activity_timeline_s msg;

   errno_enum_type msgr_error = E_FAILURE;

   if( SUB_FN(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn, arbitrated_gas[gas_id].cxm_time_ind.last_ind_fn) > GARB_CXM_IND_UPDATE_PERIOD )
   {
      if((arbitrated_gas[gas_id].cxm_time_ind.indication==CXM_LNK_DRCTN_DL) ||
         (arbitrated_gas[gas_id].cxm_time_ind.indication==CXM_LNK_DRCTN_UL_AND_DL) )
      {
         if(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.configured)
         {
            memset(&msg, 0, sizeof(cxm_activity_timeline_s));

            msgr_init_hdr((msgr_hdr_struct_type*)&msg, MSGR_GERAN_GL1, MCS_CXM_SET_ACTIVITY_TIMELINE_IND);

            msg.tech_id = geran_map_gas_id_to_cxm_id( gas_id );

            msg.direction  = CXM_LNK_DRCTN_DL;
            msg.start_time = arbitrated_gas[gas_id].timeline_rx[read_t_index][GARB_TIMELINE_THIS_FRAME].s_time;
            msg.end_time   = arbitrated_gas[gas_id].timeline_rx[read_t_index][GARB_TIMELINE_THIS_FRAME].e_time;
            msg.period     = USTMR_PER_FRAME;

            msgr_error = msgr_send( (msgr_hdr_struct_type *) &msg, sizeof(cxm_activity_timeline_s) );

#ifdef FEATURE_GARB_QTF /* GARB TODO : Implement QTF stubs for SW CXM */
            msgr_error = E_SUCCESS;
#endif /* FEATURE_GARB_QTF */

            MSG_GERAN_HIGH_2_G("GARB -> CXM : MCS_CXM_SET_ACTIVITY_TIMELINE_IND for DL start=%d end=%d", msg.start_time, msg.end_time);
         }
      }

      if((arbitrated_gas[gas_id].cxm_time_ind.indication==CXM_LNK_DRCTN_UL) ||
         (arbitrated_gas[gas_id].cxm_time_ind.indication==CXM_LNK_DRCTN_UL_AND_DL))
      {
         if(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.configured)
         {
            memset(&msg, 0, sizeof(cxm_activity_timeline_s));

            msgr_init_hdr((msgr_hdr_struct_type*)&msg, MSGR_GERAN_GL1, MCS_CXM_SET_ACTIVITY_TIMELINE_IND);

            msg.tech_id = geran_map_gas_id_to_cxm_id( gas_id );

            msg.direction  = CXM_LNK_DRCTN_UL; 
            msg.start_time = arbitrated_gas[gas_id].timeline_tx[read_t_index][GARB_TIMELINE_THIS_FRAME].s_time;
            msg.end_time   = arbitrated_gas[gas_id].timeline_tx[read_t_index][GARB_TIMELINE_THIS_FRAME].e_time;
            msg.period     = USTMR_PER_FRAME;

            msgr_error = msgr_send( (msgr_hdr_struct_type *) &msg, sizeof(cxm_activity_timeline_s) );

#ifdef FEATURE_GARB_QTF /* GARB TODO : Implement QTF stubs for SW CXM */
            msgr_error = E_SUCCESS;
#endif /* FEATURE_GARB_QTF */

            MSG_GERAN_LOW_2_G("GARB -> CXM : MCS_CXM_SET_ACTIVITY_TIMELINE_IND for UL start=%d end=%d", msg.start_time, msg.end_time);
         }
      }
   }

   if(msgr_error==E_SUCCESS) { arbitrated_gas[gas_id].cxm_time_ind.last_ind_fn = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn; }

}

/*===========================================================================

FUNCTION      garb_cxm_notify_request_activity_info

DESCRIPTION   Notification from CXM that GL1 is requested to send
              MCS_CXM_SET_ACTIVITY_TIMELINE_IND reports.

              This function is called from L1 Task on receipt of an
              MCS_CXM_REQUEST_ACTIVITY_INFO_GSM_IND.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_notify_request_activity_info(cxm_request_activity_info_s* cxm_request_activity_info, gas_id_t gas_id)
{
   if( (cxm_request_activity_info->reg_flag==TRUE) && (cxm_request_activity_info->direction!=CXM_LNK_DRCTN_INVLD) )
   {
     /* The CXM has registered for MCS_CXM_SET_ACTIVITY_TIMELINE_IND
      * reports from GL1 - set the direction (UL, DL, UL and DL) flag
      * to indicate which reports should be generated
      */
      arbitrated_gas[gas_id].cxm_time_ind.indication = cxm_request_activity_info->direction;
   }
   else
   {
     /* The CXM has unregistered for MCS_CXM_SET_ACTIVITY_TIMELINE_IND
      * reports from GL1 - clear the direction (UL, DL, UL and DL) flag
      * and associated data
      */
      arbitrated_gas[gas_id].cxm_time_ind.indication = CXM_LNK_DRCTN_INVLD;
   }
   MSG_GERAN_HIGH_2_G("GARB <- CXM : FN=%d CXM NOTIFY REQ ACTIVITY INFO direction=%d", GSTMR_GET_FN_GERAN(gas_id), arbitrated_gas[gas_id].cxm_time_ind.indication );
}


/*===========================================================================

FUNCTION      garb_cxm_notify_coex_bcast_ind

DESCRIPTION   Notification from CXM to notify TECH RX/TX state.

              This function is called from L1 Task on receipt of an
              MCS_CXM_COEX_TECH_STATE_BCAST_IND


RETURN VALUE  None

===========================================================================*/
void garb_cxm_notify_coex_bcast_ind(cxm_coex_tech_state_bcast_ind_s* cxm_coex_tech_state_bcast_info)
{
   /* Maps each TECH ID to a ptr to the corresponding TECH STATE INFO entry for fast access */
   cxm_coex_tech_data* map_tech_to_tech_state_info[CXM_TECH_MAX] = {NULL};
   uint16   cxm_techs_rx_on    = 0; /* Bitmap of TECHs that have registered Rx ON */
   uint16   cxm_techs_tx_on    = 0; /* Bitmap of TECHs that have registered Rx ON */
   boolean  peer_x_tech_is_tdd = FALSE; /* Non-GSM TDD TECHs have registered (TDSCDMA or TDD LTE) */
   uint8    i                  = 0;
   gas_id_t gas_id;

   /* Loop through all the TECH STATE INFO entries. Each TECH ID is not expected to appear in the list more than once */
   for (i = 0;i < cxm_coex_tech_state_bcast_info->num_active_techs; i++ )
   {
      /* TECH ID and ASID for the current iteration */
      cxm_tech_type          tech_id = cxm_coex_tech_state_bcast_info->tech_state[i].tech_id;
      sys_modem_as_id_e_type as_id   = cxm_coex_tech_state_bcast_info->tech_state[i].as_id;

      /* TECH ID range check */
      if(!( (tech_id >= 0 ) && (tech_id < CXM_TECH_MAX) ))
      {
         MSG_GERAN_HIGH_1("GARB <- CXM :MCS_CXM_COEX_TECH_STATE_BCAST_IND tech_id=%d is invalid", tech_id);
         continue;
      }

      /* TECH IDs that have been registered 1for IRAT are ignored since we only care about active TECHs */
      if(cxm_coex_tech_state_bcast_info->tech_state[i].dl_link_type_mask & CXM_LNK_TYPE_IRAT)
      {
         MSG_GERAN_HIGH_2("GARB <- CXM :MCS_CXM_COEX_TECH_STATE_BCAST_IND tech_id=%d on ASID=%d registed for IRAT", tech_id, as_id);
         continue;
      }

      /* If TECH is Rx ON then set the TECH ID in the Rx ON list */
      if(cxm_coex_tech_state_bcast_info->tech_state[i].rx_active)
      {
         cxm_techs_rx_on |= (1 << tech_id);
      }

      /* If TECH is Tx ON then set the TECH ID in the Tx ON list */
      if(cxm_coex_tech_state_bcast_info->tech_state[i].tx_active)
      {
         cxm_techs_tx_on |= (1 << tech_id);
      }

      /* If TECH is TDD then set the TECH ID in the Tx ON list */
      if ( (tech_id == CXM_TECH_TDSCDMA || tech_id == CXM_TECH_LTE || tech_id == CXM_TECH_LTE2 ) && cxm_coex_tech_state_bcast_info->tech_state[i].tx_active)
      {
        //peer_x_tech_is_tdd = cxm_coex_tech_state_bcast_info->tech_state[i].is_tech_tdd;
        if ( cxm_coex_tech_state_bcast_info->tech_state[i].tech_id == CXM_TECH_TDSCDMA)
        {
           peer_x_tech_is_tdd = TRUE;;
        }
        /* LTE TDD BAND 3gpp spec : BAND 33 to BSN 43 (sys.h), BAND 44 not supported by RFC */
        else if (   (cxm_coex_tech_state_bcast_info->tech_state[i].tech_id == CXM_TECH_LTE
                     || cxm_coex_tech_state_bcast_info->tech_state[i].tech_id == CXM_TECH_LTE2 ) &&
                  ( (cxm_coex_tech_state_bcast_info->tech_state[i].ul_bands[0] >= SYS_BAND_LTE_EUTRAN_BAND33 ) &&
                    (cxm_coex_tech_state_bcast_info->tech_state[i].ul_bands[0] <= SYS_BAND_LTE_EUTRAN_BAND43 )
                  )
                )
        {
           peer_x_tech_is_tdd = TRUE;
        }
      }

      /* TECH ID to BCAST message entry mapping */
      map_tech_to_tech_state_info[tech_id] = &(cxm_coex_tech_state_bcast_info->tech_state[i]);

      MSG_GERAN_LOW_4("GARB <- CXM :MCS_CXM_COEX_TECH_STATE_BCAST_IND tech_id=%d rx_active%d tx_active %d tech_is_tdd %d",
                        cxm_coex_tech_state_bcast_info->tech_state[i].tech_id,
                        cxm_coex_tech_state_bcast_info->tech_state[i].rx_active,
                        cxm_coex_tech_state_bcast_info->tech_state[i].tx_active,
                        peer_x_tech_is_tdd);
                        //cxm_coex_tech_state_bcast_info->tech_state[i].is_tech_tdd);
   }


   /* Loop around each GAS and set the peer TECH info */
   for(gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
   {
      cxm_tech_type tech_id = geran_map_gas_id_to_cxm_id(gas_id);

      /* TECH ID range check */
      if( (tech_id >= 0 ) && (tech_id < CXM_TECH_MAX) )
      {
         if(map_tech_to_tech_state_info[tech_id] != NULL )
         {
            /* Take the list of Rx ON TECHs and remove our TECH ID to determine if any of our peers are Rx ON */
            boolean peer_tech_rx_on = (( cxm_techs_rx_on & (~(1<< tech_id)) ) !=0 );
            /* Take the list of Tx ON TECHs and remove our TECH ID to determine if any of our peers are Tx ON */
            boolean peer_tech_tx_on = (( cxm_techs_tx_on & (~(1<< tech_id)) ) !=0 );

            /* Take the list of Rx ON TECHs and remove all other TECH IDs to determine if this GAS is Rx ON */
            boolean this_tech_rx_on = (( cxm_techs_rx_on & (1<< tech_id) ) !=0 );
            /* Take the list of Tx ON TECHs and remove all other TECH IDs to determine if this GAS is Tx ON */
            boolean this_tech_tx_on = (( cxm_techs_tx_on & (1<< tech_id) ) !=0 );

            /* To get the peer TECHs for this GAS ID amalgamate the Rx ON and Tx ON lists and remove our TECH ID */
            arbitrated_gas[gas_id].cxm_peer_tech        = ( ( cxm_techs_tx_on | cxm_techs_rx_on) & (~(1<< tech_id)) );
            arbitrated_gas[gas_id].gas_is_coex_victim   = this_tech_rx_on && peer_tech_tx_on;
            arbitrated_gas[gas_id].gas_is_coex_agressor = this_tech_tx_on && peer_tech_rx_on;
            arbitrated_gas[gas_id].peer_x_tech_is_tdd   = peer_x_tech_is_tdd;

            MSG_GERAN_HIGH_4_G("GARB <- CXM :MCS_CXM_COEX_TECH_STATE_BCAST_IND cxm_peer_techs=%d gas_is_coex_victim=%d gas_is_coex_agressor=%d peer_x_tech_is_tdd %d",
                                arbitrated_gas[gas_id].cxm_peer_tech,
                                arbitrated_gas[gas_id].gas_is_coex_victim,
                                arbitrated_gas[gas_id].gas_is_coex_agressor,
                                arbitrated_gas[gas_id].peer_x_tech_is_tdd );
         }
      }
      else
      {
         MSG_GERAN_HIGH_1("GARB <- CXM :MCS_CXM_COEX_TECH_STATE_BCAST_IND tech_id=%d is invalid", tech_id);
      }
   }
}

#endif /* FEATURE_GSM_COEX */
#endif /* FEATURE_DUAL_SIM */
#ifdef FEATURE_GSM_COEX_FW_CXM
/*===========================================================================

FUNCTION      garb_return_cxm_channel_id

DESCRIPTION   return the cxm channel ID
  
RETURN VALUE  cxm channel type

===========================================================================*/
cxm_channel_out_type garb_return_cxm_channel_id(grm_client_enum_t grm_client, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  cxm_channel_in_type           cxm_channel_in  ={0};
  cxm_channel_out_type          cxm_channel_out ={0};
  cxm_rfdev_type                rf_device       = gl1_hw_get_gsm_rf_id(grm_client, gas_id );
  
  if ( rf_device == RFM_INVALID_DEVICE )
  {
#ifdef FEATURE_DUAL_SIM
    if( gl1_msg_get_multi_sim_standby_mode() && garb_is_g_plus_g() )
    {
       /* This happens in GBTA where IDLE client is not granted TRM */
       /* Other sub may not own the lock at this time and hence one cannot derive from peer gas_id */
       rf_device = grm_get_other_g_rf_device(gas_id);

       if ( rf_device == RFM_INVALID_DEVICE )  // other sub not initialised
       {
          rf_device = RFM_DEVICE_0;
          MSG_GERAN_HIGH_1_G( "Invalid RF device %d set RFM_DEVICE_0 during INIT", rf_device);
       }
       else
       {
        /* FOR use case when G1 SUB is getting START_GSM + G 2 SUB has RF device
           RF Device has no sigificance but cannot be RFM_INVALID
        */
         MSG_GERAN_HIGH_1_G( "Invalid RF device %d set RFM_DEVICE_OTHER_SUB", rf_device);
       }
       cxm_channel_in.techid    = geran_map_gas_id_to_cxm_id( gas_id );
       cxm_channel_in.rf_device = rf_device;
       cxm_channel_out          = cxm_get_channel_id(cxm_channel_in );
       return ( cxm_channel_out );
    }
    else
#endif /* FEATURE_DUAL_SIM */
    {
       /* This GAS does not own a TRM grant and this is not GBTA - so this is assumed to be CM IRAT */

       cxm_channel_in.rf_device = (uint8)gl1_hw_cm_get_rf_device(gas_id);
       cxm_channel_in.techid    = gl1_rf_mode_type_to_cxm_tech_id(  gl1_hw_cm_get_cm_rat_type(gas_id) );

       if( (cxm_channel_in.rf_device!=RFM_INVALID_DEVICE) && (cxm_channel_in.techid!=CXM_TECH_DFLT_INVLD) )
       {
          cxm_channel_out = cxm_get_channel_id(cxm_channel_in);

          MSG_GERAN_HIGH_3_G("CM RAT type = %d CM RF device = %d CXM channel id = %d", cxm_channel_in.techid, cxm_channel_in.rf_device, cxm_channel_out.rx_channel_id );
       }
       else
       {
          MSG_GERAN_HIGH_2_G("Unable to determine CXM channel ID rf_device=%d tech_id=%d", cxm_channel_in.rf_device, cxm_channel_in.techid);
       }

       return ( cxm_channel_out );
    }
  }

  /* This can only happen during start GSM mode or when X sub does not own TRM.
  ** On subsequent call when Sub's are active - we should not come here.
  */
  if ( rf_device == RFM_INVALID_DEVICE )
  {
    rf_device = RFM_DEVICE_0;

    MSG_GERAN_HIGH_3_G( "RF device still INVALID  %d tech_id %d force to RFM_DEVICE_0 devmode %d",
                         RFM_DEVICE_0, geran_map_gas_id_to_cxm_id( gas_id ), gl1_msg_get_multi_sim_sys_mode()); 
  }

  cxm_channel_in.techid    = geran_map_gas_id_to_cxm_id( gas_id );
  cxm_channel_in.rf_device = rf_device;
  cxm_channel_out          = cxm_get_channel_id( cxm_channel_in );

  MSG_GERAN_LOW_1_G( "MCS channel ID %d",  cxm_channel_out.rx_channel_id);

  return ( cxm_channel_out );
}
#endif /* FEATURE_GSM_COEX_FW_CXM */

/*===========================================================================

FUNCTION      garb_cxm_get_freqid
 
DESCRIPTION   This function returns the CXM freqid associated with the 
              requested ARFCN.
 
RETURN VALUE  The CXM freqid associated with the requested band.
 
===========================================================================*/
uint32 garb_cxm_get_freqid(ARFCN_T arfcn, garb_rxtx_t rxtx, gas_id_t gas_id)
{
   uint32 i = 0;

   if( (rxtx==GARB_RX) && (arfcn.band < INVALID_BAND ) )
   {
      /* Look for the requested ARFCN in the DL table */
      for(i=0;i<arbitrated_gas[gas_id].dl_arfcn_table.num_arfcns;i++)
      {
         if(arbitrated_gas[gas_id].dl_arfcn_table.arfcns[i].arfcn == arfcn.num)
         {
            return arbitrated_gas[gas_id].dl_arfcn_table.arfcns[i].cxm_freqid;
         }
      }

      MSG_GERAN_LOW_1_G("GARB : No CXM freqid available for DL ARFCN %d", arfcn.num);

      /* Fall-back to back a band registered DL Freq ID if none was found for the ARFCN */
      return arbitrated_gas[gas_id].dl_band_table[arfcn.band];
   }
   else
   if( (rxtx==GARB_TX) && (arfcn.band < INVALID_BAND ) )
   {
      /* Look for the requested ARFCN in the UL table */
      for(i=0;i<arbitrated_gas[gas_id].ul_arfcn_table.num_arfcns;i++)
      {
         if(arbitrated_gas[gas_id].ul_arfcn_table.arfcns[i].arfcn == arfcn.num)
         {
            return arbitrated_gas[gas_id].ul_arfcn_table.arfcns[i].cxm_freqid;
         }
      }

      MSG_GERAN_HIGH_1_G("GARB : No CXM freqid available for UL ARFCN %d", arfcn.num);
   }

   return CXM_UNKNOWN_FREQID;
}

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
/*===========================================================================

FUNCTION      garb_cxm_get_prio_from_activity_tier

DESCRIPTION   This function retrieves the priority associated with the 
              specified CXM activity tier. If the specified activity tier is
              invalid then a priority of zero is returned.

RETURN VALUE  CXM priority
 
===========================================================================*/
uint32 garb_cxm_get_prio_from_activity_tier(cxm_activity_type tier, gas_id_t gas_id)
{
   uint32 prio = 0;

   if(tier!=MAX_ACTIVITY_TIERS)
   {
      prio = arbitrated_gas[gas_id].cxm_activity_table[tier].priority;
   }

   return prio;
}
/*===========================================================================

FUNCTION      garb_cxm_set_slot_activity_timeline

DESCRIPTION   Notifies CXM of GSM timeline information. This provides a much
              more detailed timeline report than the more primative
              MCS_CXM_SET_ACTIVITY_TIMELINE_IND and will likely obsolete it.

              This UMID is used to provide a rolling 4 frame timeline window
              to MCS. This function is called on every frame to build the 4
              frame timeline.

              This information is used by TECH L1s and WLAN to avoid
              conflicts with GSM.

              This function sends MCS_CXM_SET_SLOT_ACTIVITY_TL_IND to
              CXM.

RETURN VALUE  None

===========================================================================*/
void garb_cxm_set_slot_activity_timeline(uint8 read_t_index,gas_id_t gas_id)
{
 if(  ( arbitrated_gas[gas_id].cxm_policy.active_cxm_policy & CXM_POLICY_WCI2_OUTGNG_TYPE6_TX_ADV_NOTICE ) )
 {
   cxm_timing_info_s *msg=NULL;

   errno_enum_type msgr_error = E_FAILURE;

   int i = 0;

   /* Enhanced Timeline - Currenly only supported in TCH, SDCCH and PS */
   if( (arbitrated_gas[gas_id].state!=GARB_CS) &&
       (arbitrated_gas[gas_id].state!=GARB_CS_SDCCH) &&
       (arbitrated_gas[gas_id].state!=GARB_PS) &&
       (arbitrated_gas[gas_id].state!=GARB_CS_PS) )
   {
      return;
   }

   msg = GPRS_MEM_MALLOC(sizeof(cxm_timing_info_s));
   if(msg == NULL) 
   {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_FATAL("mem alloc failed",0,0,0);
   }


   memset(msg, 0, sizeof(cxm_timing_info_s));

   msgr_init_hdr((msgr_hdr_struct_type*)msg, MSGR_GERAN_GL1, MCS_CXM_SET_SLOT_ACTIVITY_TL_IND);

   msg->tech_id = geran_map_gas_id_to_cxm_id( gas_id );


   /* Metrics Info */
   //msg->tech_metrics.gsm_metrics.call_type      = garb_get_wwcoex_call_type(gas_id);

   /* Timeline Entries */
   for(i = 0; i < arbitrated_gas[gas_id].timeline_num_entries[read_t_index]; i++)
   {
      if(arbitrated_gas[gas_id].timeline[read_t_index][i].rxtx==GARB_RX)
      {
         msg->slot_list[msg->num_entries].band       = gl1_map_gsm_band_to_sys_band(arbitrated_gas[gas_id].timeline[read_t_index][i].arfcn.band);
         msg->slot_list[msg->num_entries].bandwidth  = 200000; /* 200 kHz */;
         msg->slot_list[msg->num_entries].direction  = CXM_LNK_DRCTN_DL;
         msg->slot_list[msg->num_entries].start_time = arbitrated_gas[gas_id].timeline[read_t_index][i].s_time;
         msg->slot_list[msg->num_entries].end_time   = arbitrated_gas[gas_id].timeline[read_t_index][i].e_time;
         //msg->slot_list[msg->num_entries].freqid     = garb_cxm_get_freqid(arbitrated_gas[gas_id].timeline[i].arfcn, GARB_RX, gas_id);
         msg->slot_list[msg->num_entries].frequency  = garb_arfcn_to_centre_freq_kHz(arbitrated_gas[gas_id].timeline[read_t_index][i].arfcn, GARB_RX, gas_id);
         msg->slot_list[msg->num_entries].link_type  = arbitrated_gas[gas_id].timeline[read_t_index][i].link_type;
         msg->slot_list[msg->num_entries].micro_prio = garb_cxm_get_prio_from_activity_tier(arbitrated_gas[gas_id].timeline[read_t_index][i].cxm_tier, gas_id);
         msg->slot_list[msg->num_entries].pwr        = arbitrated_gas[gas_id].timeline[read_t_index][i].power;
         msg->slot_list[msg->num_entries].wlan_prio  = garb_map_channel_to_wlan_priority[arbitrated_gas[gas_id].timeline[read_t_index][i].channel_type];

         msg->num_entries++;
      }
      else
      if(arbitrated_gas[gas_id].timeline[read_t_index][i].rxtx==GARB_TX)
      {
         msg->slot_list[msg->num_entries].band       = gl1_map_gsm_band_to_sys_band(arbitrated_gas[gas_id].timeline[read_t_index][i].arfcn.band);
         msg->slot_list[msg->num_entries].bandwidth  = 200000; /* 200 kHz */;
         msg->slot_list[msg->num_entries].direction  = CXM_LNK_DRCTN_UL;
         msg->slot_list[msg->num_entries].start_time = arbitrated_gas[gas_id].timeline[read_t_index][i].s_time;
         msg->slot_list[msg->num_entries].end_time   = arbitrated_gas[gas_id].timeline[read_t_index][i].e_time;
         //msg->slot_list[msg->num_entries].freqid     = garb_cxm_get_freqid(arbitrated_gas[gas_id].timeline[write_t_index][i].arfcn, GARB_TX, gas_id);
         msg->slot_list[msg->num_entries].frequency  = garb_arfcn_to_centre_freq_kHz(arbitrated_gas[gas_id].timeline[read_t_index][i].arfcn, GARB_TX, gas_id);
         msg->slot_list[msg->num_entries].link_type  = 0;
         msg->slot_list[msg->num_entries].micro_prio = garb_cxm_get_prio_from_activity_tier(arbitrated_gas[gas_id].timeline[read_t_index][i].cxm_tier, gas_id);
         msg->slot_list[msg->num_entries].pwr        = 10 * arbitrated_gas[gas_id].timeline[read_t_index][i].power;
         msg->slot_list[msg->num_entries].wlan_prio  = garb_map_channel_to_wlan_priority[arbitrated_gas[gas_id].timeline[read_t_index][i].channel_type];

         msg->num_entries++;
      }
   }

   msgr_error = msgr_send( (msgr_hdr_struct_type *) msg, sizeof(cxm_timing_info_s) );
   
   if (msgr_error != E_SUCCESS)
   {
      MSG_GERAN_ERROR_0_G("GARB->CXM MCS_CXM_SET_SLOT_ACTIVITY_TL_IND send failed");
   }

#ifdef FEATURE_GARB_ENHANCED_TIMELINE_DEBUG
   for(i=0;i<msg->num_entries;i++)
   {
      if(msg->slot_list[i].direction==CXM_LNK_DRCTN_DL)
      {
         MSG_GERAN_HIGH_5_G("QTF_CXM_ENHANCED_TIMELINE : dir=DL : fn=%d : s_time=%d : e_time=%d : wlanprio=%d : channel_type=%d",
            arbitrated_gas[gas_id].timeline[read_t_index][i].fn,
            msg->slot_list[i].start_time,
            msg->slot_list[i].end_time,
            msg->slot_list[i].wlan_prio,
            arbitrated_gas[gas_id].timeline[read_t_index][i].channel_type
         );
         MSG_GERAN_HIGH_5_G("QTF_CXM_ENHANCED_TIMELINE : dir=DL : micro_prio=%9d : arfcn=%d : band=%d : link_type=%d : freqid=%d",
            msg->slot_list[i].micro_prio,
            arbitrated_gas[gas_id].timeline[read_t_index][i].arfcn.num,
            msg->slot_list[i].band ,
            msg->slot_list[i].link_type,
            msg->slot_list[i].freqid
         );

      }
      else
      if(msg->slot_list[i].direction==CXM_LNK_DRCTN_UL)
      {
         MSG_GERAN_HIGH_5_G("QTF_CXM_ENHANCED_TIMELINE : dir=UL : pwr=%9d : s_time=%9d : e_time=%9d : wlanprio=%3d : freqid=%3d",
            msg->slot_list[i].pwr ,
            msg->slot_list[i].start_time,
            msg->slot_list[i].end_time,
            msg->slot_list[i].wlan_prio,
            msg->slot_list[i].freqid
         );
         MSG_GERAN_HIGH_5_G("QTF_CXM_ENHANCED_TIMELINE : dir=UL : micro_prio=%9d : arfcn=%d : band=%d : link_type=%d : fn=%d",
            msg->slot_list[i].micro_prio,
            arbitrated_gas[gas_id].timeline[read_t_index][i].arfcn.num,
            msg->slot_list[i].band ,
            msg->slot_list[i].link_type,
            arbitrated_gas[gas_id].timeline[read_t_index][i].fn
         );

      }
   }
#endif /* FEATURE_GARB_ENHANCED_TIMELINE_DEBUG */

   GPRS_MEM_FREE(msg);
   msg = NULL;
 }
}
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

#ifdef FEATURE_WLAN_COEX_SW_CXM
/*===========================================================================

FUNCTION      garb_cxm_reset_coex_active_policy

DESCRIPTION   This function re-sets the coex active policies to NONE

===========================================================================*/
void garb_cxm_reset_coex_active_policy( gas_id_t gas_id )
{
  arbitrated_gas[gas_id].cxm_policy_updated = FALSE;
  arbitrated_gas[gas_id].cxm_policy.active_cxm_policy = CXM_POLICY_NONE;
}

/*===========================================================================

FUNCTION      garb_cxm_reset_coex_metrics

DESCRIPTION   This function re-sets the action required for coex metrics 

===========================================================================*/
void garb_cxm_reset_coex_metrics( gas_id_t gas_id )
{
  arbitrated_gas[gas_id].cxm_metrics.action = CXM_ACTION_UNINIT;
}

/*===========================================================================

FUNCTION      garb_cxm_coex_active_policy

DESCRIPTION   This function store active cxm policy from CXM

===========================================================================*/
void garb_cxm_coex_active_policy(cxm_coex_active_policy_ind_s* msg, gas_id_t gas_id)
{
  GL1_ISR_LOCK(gas_id);
  arbitrated_gas[gas_id].cxm_policy = msg->active_config;
  arbitrated_gas[gas_id].cxm_policy_updated = TRUE;
  GL1_ISR_UNLOCK(gas_id);

  MSG_GERAN_HIGH_1_G("Coex policy received: active_cxm_policy %x",arbitrated_gas[gas_id].cxm_policy.active_cxm_policy);
}

/*===========================================================================

FUNCTION      garb_cxm_coex_metrics_rsp

DESCRIPTION   This function sends coex metrics response to CXM
      filt_metric is the filtered SNR
      metric_cnt is the number of samples used for filtering SNR
      status is E_SUCCESS is SNR is available otherwise E_NOT_AVAILABLE

===========================================================================*/
void garb_cxm_coex_metrics_rsp( gas_id_t gas_id )
{
  boolean result;
  uint32 filt_metric = 0, metric_cnt = 0;
  cxm_coex_metrics_rsp_s msg;

  memset(&msg,0,sizeof(cxm_coex_metrics_rsp_s));
  result = gpl1_read_avg_snr(&filt_metric, &metric_cnt, gas_id);

  if(result)
  {
    msg.status = E_SUCCESS;
  }
  else
  {
    msg.status = E_NOT_AVAILABLE;
  }
  msg.metric_cnt = metric_cnt;
  msg.filt_metric = filt_metric;
  msg.msg_id = arbitrated_gas[gas_id].cxm_metrics.read_params.msg_id;
  msg.tech_id = geran_map_gas_id_to_cxm_id(gas_id);

  msgr_init_hdr(&msg.msg_hdr, MSGR_GERAN_GL1, MCS_CXM_COEX_TECH_METRICS_RSP);

  (void) msgr_send( &msg.msg_hdr, sizeof(cxm_coex_metrics_rsp_s) );

  if(result)
  {
    MSG_GERAN_HIGH_2_G("GARB -> CXM : MCS_CXM_COEX_TECH_METRICS_RSP SINR %d count %d",filt_metric,metric_cnt);
  }
  else
  {
    MSG_GERAN_HIGH_0_G("GARB -> CXM : MCS_CXM_COEX_TECH_METRICS_RSP metrics not available");
  }
}

/*===========================================================================

FUNCTION      garb_cxm_coex_metrics_req

DESCRIPTION   This function handles the coex metrics request from CXM.
       COEX sets the action required for metrics, action field is
       CXM_ACTION_START - start taking the samples for coex metrcis (SNR)
       CXM_ACTION_STOP - stop taking the samples for coex metrcis (SNR) 
       CXM_ACTION_READ - send the filtered SNR to CXM

===========================================================================*/
void garb_cxm_coex_metrics_req(cxm_coex_metrics_req_s* msg, gas_id_t gas_id)
{
  cxm_action_e action = msg->action;

  arbitrated_gas[gas_id].cxm_metrics.action = action;
  arbitrated_gas[gas_id].cxm_metrics.read_params.msg_id = msg->payload.read_params.msg_id;

  if( action == CXM_ACTION_START )
  {
    if ( ( msg->payload.start_params.alpha > 0 ) && ( msg->payload.start_params.alpha <= 256 ) )
    {
      arbitrated_gas[gas_id].cxm_metrics.alpha = msg->payload.start_params.alpha;
    }
    else
    {
      arbitrated_gas[gas_id].cxm_metrics.alpha = 256;
    }
  }

  MSG_GERAN_HIGH_2_G("Coex metrics received: action %d alpha %d",action,arbitrated_gas[gas_id].cxm_metrics.alpha);

  if( action == CXM_ACTION_START )
  {
    gpl1_enable_snr_avg(gas_id);
  }
  else if( action == CXM_ACTION_STOP )
  {
    gpl1_stop_snr_avg(gas_id);
  }
  else if( action == CXM_ACTION_READ )
  {
    garb_cxm_coex_metrics_rsp(gas_id);
  }
}

/*===========================================================================

FUNCTION      garb_cxm_coex_metrics_alpha

DESCRIPTION   This function stores the alpha for the filtering coex metrics, value of alpha is assigned
      by COEX

===========================================================================*/
uint32 garb_cxm_coex_metrics_alpha( gas_id_t gas_id )
{
  return (arbitrated_gas[gas_id].cxm_metrics.alpha);
}

/*===========================================================================

FUNCTION      garb_cxm_fw_priority_fail_report

DESCRIPTION   This function sends cxm_fw_priority_fail report to cxm
           cxm_fw_priority_fail report is prepared by GFW and GL1 just bypass this report to CXM

===========================================================================*/
void garb_cxm_fw_priority_fail_report( GfwWlanCxmConfigRsp* config_rsp, gas_id_t gas_id )
{
  cxm_coex_tech_tx_frm_dnl_report_ind_s msg;

  msg.stats = config_rsp->response;
  msg.tech_id = geran_map_gas_id_to_cxm_id(gas_id);

  msgr_init_hdr(&msg.msg_hdr, MSGR_GERAN_GL1,	MCS_CXM_COEX_TECH_TX_FRM_DNL_REPORT_IND);

  (void) msgr_send( &msg.msg_hdr, sizeof(cxm_coex_tech_tx_frm_dnl_report_ind_s) );

  MSG_GERAN_HIGH_0_G("GARB -> CXM : MCS_CXM_COEX_TECH_TX_FRM_DNL_REPORT_IND");
}

/*===========================================================================

FUNCTION      garb_cxm_send_sleep_duration

DESCRIPTION   This function sendssend sleep duration to CXM.
  boolean is_going_to_sleep - if going to sleep
  duration - ms (rounded down)
       when going to sleep then duration is sleep duartion
       when weaking-up then duartion is the duration when RF is going to be used, this is set to
                          4ms (rounded down) one GSM frame

===========================================================================*/
void garb_cxm_send_sleep_duration( boolean sleep, uint32 duration, gas_id_t gas_id)
{
  if(  ( arbitrated_gas[gas_id].cxm_policy.active_cxm_policy & CXM_POLICY_WCI2_OUTGNG_TYPE3_INACTVTY_DURN ) )
  {
    cxm_coex_tech_sleep_wakeup_duration_ind_s msg;
    msg.duration = duration;
    msg.is_going_to_sleep = sleep;
    msg.tech_id = geran_map_gas_id_to_cxm_id(gas_id);
    
    msgr_init_hdr(&msg.msg_hdr, MSGR_GERAN_GL1, MCS_CXM_COEX_TECH_SLEEP_WAKEUP_IND);
    
    (void) msgr_send( &msg.msg_hdr, sizeof(cxm_coex_tech_sleep_wakeup_duration_ind_s) );
    
    MSG_GERAN_HIGH_2_G("GARB -> CXM : MCS_CXM_COEX_TECH_SLEEP_WAKEUP_IND sleep %d duration %d",sleep, duration);
  }
}

/*===========================================================================

FUNCTION      garb_cxm_high_priority_event_ind

DESCRIPTION   This function indicates the high priority event and their start and end timeing in ustmr

===========================================================================*/
void garb_cxm_high_priority_event_ind( uint32 start_fn, uint32 frames_required, cxm_high_prio_oper_e op_id, gas_id_t gas_id )
{
  cxm_coex_high_prio_ind_s msg;

  memset(&msg,0,sizeof(cxm_coex_high_prio_ind_s));
  msg.tech_id = geran_map_gas_id_to_cxm_id(gas_id);

  if( frames_required == 0)
  {
    msg.start = 0;
    msg.end = 0;
    msg.op_id = CXM_HIGH_PRIO_UNKNOWN;
  }
  else
  {
    if( !(IS_FRAME_NUM_LATER(start_fn,GSTMR_GET_FN_GERAN(gas_id))) )
    {
      msg.start = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart;
    }
    else
    {
      msg.start = USTMR_ADD(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart, (SUB_FN(start_fn, GSTMR_GET_FN_GERAN(gas_id))*USTMR_PER_FRAME) );
    }

    if( frames_required >= GL1_DEFS_INVALID_FN )
    {
      msg.end = 0xFFFFFFFF;
    }
    else
    {
      msg.end = USTMR_ADD(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart, (frames_required*USTMR_PER_FRAME) );
    }
    msg.op_id = op_id;
  }
  msgr_init_hdr(&msg.msg_hdr, MSGR_GERAN_GL1, MCS_CXM_COEX_HIGH_PRIORITY_IND);

  (void) msgr_send( &msg.msg_hdr, sizeof(cxm_coex_high_prio_ind_s) );

  MSG_GERAN_LOW_3_G("GARB -> CXM : MCS_CXM_COEX_HIGH_PRIORITY_IND start %d end %d op_id %d",msg.start, msg.end,op_id);
}
#endif /* FEATURE_WLAN_COEX_SW_CXM */

#endif /* FEATURE_GSM_COEX_SW_CXM */

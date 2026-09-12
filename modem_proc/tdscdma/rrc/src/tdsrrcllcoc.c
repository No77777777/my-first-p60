/*===========================================================================

                    L3 - RRC Lower Layer Controller Ordered Config

DESCRIPTION
  This file contains functions necessary for the RRC Lower Layer Controller
  Ordered Config Processing.

  The functions that are to be called from external modules are:

  - tdsrrcllc_set_ordered_config(tdsrrc_state_e_type next_state,
                              uint8 dl_sdu_num, void *msg_ptr)

    This function takes a pointer to the ASN.1 decoded message, the next
    state, and the downlink SDU number as parameters. It then fills in the
    ordered config database. After the order config data base is filled in,
    status is then returned to the calling function as to the success
    or failure to fill in the ordered config database.

    The return type is of type tdsrrcllc_oc_status_e_type. This indicates
    if TDSORDERED_CONFIG is set and if not, what the current failure
    status is. The calling procedure then needs to act accordingly.

    Once TDSORDERED_CONFIG is set, rrcllc_channel_config() is called with
    LLC using the newly programed data to configure the lower layers with
    TDSORDERED_CONFIG.


    * Idle-Disconnected to Idle-Connecting

    ************ Procedure calls rrcllc_chan_config_req() *************
    1) Copy SIB5 data to TDSCURRENT_CONFIG (Note SIB5 data is already in
       TDSCURRENT_CONFIG, courtesy the PCH setup), unless a SIB change
       has occurred.
    2) Config Lower Layers (via primitives with CURRENT_CONFIG_PTR),
       note this is for PRACH/RACH/CCCH Uplink only, while enabling
       FACH Transport blocks, i.e. RLC-UL, MAC-UL, and PHYCHAN-PRACH.
    3) Wait for Lower Layer confirms
    4) Send Channel Confirm to RRC Connection Establishment

    * Idle-Connecting to CELL_DCH

    ************ Procedure calls tdsrrcllc_set_ordered_config() **********
    1) TDSORDERED_CONFIG = TDSCURRENT_CONFIG
    2) Extract OTA Msg IEs and overwrite those values included in Msg.
    3) tdsordered_config.set_status = OC_SET_FOR..
    ************ Procedure calls rrcllc_chan_config_req() *************
    1) Config Lower Layers (via primitives with ORDERED_CONFIG_PTR)
    2) Wait for Lower Layer confirms
    3) Change State (to CELL_DCH)
    4) Send Channel Confirm to RRC Connection Establishment
    *********** Procedure calls tdsrrcllc_clear_ordered_config() *********
    1) tdsordered_config.set_status = TDSOC_NOT_SET
    2) CURRENT_CONFIG_PTR = ORDERED_CONFIG_PTR
    ************ Receive an RB Setup msg & stay in CELL_DCH ***********
    ************ Procedure calls tdsrrcllc_set_ordered_config() **********
    1) TDSORDERED_CONFIG = TDSCURRENT_CONFIG
    2) Extract OTA Msg IEs and overwrite those values included in Msg.
    3) tdsordered_config.set_status = OC_SET_FOR..
    ************ Procedure calls rrcllc_chan_config_req() *************
    1) Config Lower Layers (via primitives with ORDERED_CONFIG_PTR)
    2) Wait for Lower Layer confirms
    3) Change State (to CELL_DCH)
    4) Send Channel Confirm to RB Setup
    *********** Procedure calls tdsrrcllc_clear_ordered_config() *********
    1) tdsordered_config.set_status = TDSOC_NOT_SET
    2) CURRENT_CONFIG_PTR = ORDERED_CONFIG_PTR


    * Idle-Connecting to CELL_FACH

    ************ Procedure calls tdsrrcllc_set_ordered_config() **********
    1) TDSORDERED_CONFIG = TDSCURRENT_CONFIG
    2) If SIB change Overwrite TDSORDERED_CONFIG with SIB5 updates.
    3) Extract OTA Msg IEs and overwrite those values included in Msg.
    4) tdsordered_config.set_status = OC_SET_FOR..
    ************ Procedure calls rrcllc_chan_config_req() *************
    1) Config Lower Layers (via primitives with ORDERED_CONFIG_PTR)
    2) Wait for Lower Layer confirms
    3) Change State (to CELL_FACH)
    4) Send Channel Confirm to RRC Connection Establishment
    *********** Procedure calls tdsrrcllc_clear_ordered_config() *********
    1) tdsordered_config.set_status = TDSOC_NOT_SET
    2) CURRENT_CONFIG_PTR = ORDERED_CONFIG_PTR

  - tdsrrcllc_clear_ordered_config(void)

    This function will clear(reset) the ordered_config_status flag. It also
    swaps the tdsordered_config_ptr and tdscurrent_config_ptr.

    The following steps occur:
    1) tdsordered_config.set_status = TDSOC_NOT_SET
    2) temp_ptr = tdscurrent_config_ptr
    3) tdscurrent_config_ptr = tdsordered_config_ptr
    4) tdsordered_config_ptr = temp_ptr

  Functions that can be called from other rrcllc modules are:

  - rrcllc_init_ordered_config()

    This function is called by tdsrrcllc_init(). This function initializes
    the ordered config and current config pointers and data inside
    the ordered config and current config data bases.

  - tdsrrcllc_copy_sib_to_oc(tdsrrc_SysInfoType5 *sib5_ptr,
                          tdsrrc_state_e_type next_state)

    This function copies data from the SIB data base to the ordered
    config data base. It also performs the SCCPCH, PRACH and the
    Access Service Class Establishment selection that is required in RRC.

   *********************************************************************
   ***************** Other Assumptions in the code *********************
   *********************************************************************
   - There is no support for TDD, DRAC, PDSCH, or CPCH.

   - CELL_PCH and URA_PCH states are not supported.

   - Ciphering and Integrity Protection IEs are not processed now.

   - Dynamic TTIs are only for TDD,; however it is not clear in the
     spec; hence, we will error fatal in the code.

   - Only the first PRACH in the SIB 5 PRACH list is supported.

   - Only the first SCCPCH in the SIB 5 SCCPCH list is supported.

   - No PRACH and SCCPCH selection algorithms are used. In other words
     they have been coded but not tested.

   - Since only 1 PRACH and SCCPCH are supported, there is no support for
     reading the info from the previous entry in the list for the various
     IEs as stated in 25.331. 25.331 indicates that for optional present
     IEs, the first is mandatory, and if not present for others in the loop
     (1 to MAX_PRACH or 1 to MAX_SCCPCH) use the previous value. Since only
     the first is used, the remaining has not been implemented.

   - Only TDSUE_MAX_TFC, TDSUE_MAX_TF, and TDSUE_MAX_TRCH are supported. All values
     not in these ranges are ignored.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcllcoc.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
03/14/12   as       Added code to support feature NB/WB handover support
03/13/12   zwj     Initialize tstd_indicator to TRUE.
02/28/12   mkh      Fix compiler warning
02/09/12   ysh     when w_freq change, need reconfig HSDPA
01/12/12   ysh     add init cpid_present in tdsrrcllc_init_before_msg_processing()
12/10/11   ttl      Set TPC Step size w/ the value from OTA otherwise set to TDSL1_TPC_STEP_SIZE_0DB if missed
                    from OTA.  
10/31/11   ysh      Add save SIB3's uppch_shift.
10/25/11   ysh      Modify req_mak in cphy_setup_req.
10/17/11   zwj      Modify default value of FPACH and upPCH. 
10/13/11   mkh      Now using correct macro. Earlier it was coming from W code.
09/17/11   zwj      Fixed UL TB size mac header size.
09/08/11   mkh      Events renamed to TDS
08/26/11   yzh      Add add_queue_bitmask in order to check multiple delete mac_queue info in OTA message.
                    Modify bug about remove old mapping if mac_queue is available in other mac_dflow,
                    And check old mapping when setup not recofigure mac_dflow.
06/16/11   bj       Added code to handle REL9 DPA action.
06/16/11   mkh      Changes to pass pval update ind to MAC only if nASC > 0 and
                    if SIB6 has PRACH_systemInfoList present (for connected mode)
06/14/11   yzh      Made changes from FDD to TDD in R8
06/10/11   ttl      Have AMR to use GHDI MVS TDSCDMA version functions.
06/09/11   mkh      Added support to pass pval to MAC on reading SIB7 in FACH 
03/10/11   zwj      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/11/11   bj       (3GPP: 8.5.28)Modify actions related to.E_DCH_TRANSMISSION variable from FDD to TDD.
01/06/11   ysh      IE "E-DCH Transmission Time Interval" delete. 3GPP-8.6.5.5(3.6.16).
                    Modify The relation between HSUPA categories and rate.
11/12/10   yzh      (3GPP: 8.5.25&10.3.6.23a)Modify Actions related to HS_DSCH_RECEPTION variable
                    and Downlink HS-PDSCH Information(FDD->TDD).
11/18/10   ysh      Transport Format Set. 3GPP8.6.5.1(3.2.32)
                    rach_TransportFormatSet -> rach_TransportFormatSet_LCR
11/11/10   yzh      (3GPP: 8.6.6.27)Del tdsrrcllcpcie_initialise_sync_a_post_veri_info() and modify IEs
                    when IE Downlink information common for all radio links(FDD->TDD).
11/11/10   yzh      (3GPP: 8.6.6.28a)Del Downlink F-DPCH info common for all radio links(FDD).
                    and (3GPP: 10.3.6.18)Del Downlink rate matching restriction information(FDD).
11/11/10   yzh      (3GPP: 10.3.6.39a)Add code for r4\r5-add-ext.
11/05/10   ysh      C-RNTI and H-RNTI remove condition.
                    New function for get rnti and cpid present, and judge. 3GPP-8.2.2.3(3.2.3).
11/04/10   yzh      (3GPP 10.3.6.21) modify Downlink DPCH info for each RL from FDD to TDD.
11/01/10   yzh      (3GPP 8.6.6.6) modify Uplink DPCH info from FDD to TDD
11/05/10   yzh      (3GPP: 8.6.6.15)Delete DPCH Compressed mode info(FDD only, under FEATURE_CM_SUPPORTED)(3.2.20).
11/03/10   ysh      IE "RLC size index" of RB mapping info, FDD->TDD, means change.  3GPP-8.6.4.8(3.2.1).
11/03/10   zwj      Added code to handle REL4 OTA Message. 
10/29/10   bj       Made changes to Open loop power control.Delete FDD 
                    IE "Primary CPICH Tx power", "Constant value" and "UL 
                    interference". Add TDD IE "Primary CCPCH Tx Power".
                    3GPP:8.5.7. 
10/25/10   zwj      Added code to process SCCPCHinfolist and SCCPCHinfolist-LCR-R4 for TDD (3GPP 10.3.6.71).
10/24/10   yzh      According to 3GPP25.331v716 8.6.5.12a, delete IE  "Additional RACH TFCS for CCCH" (FDD only)(3.2.11).
10/10/10   ysh      FDD only IE"DPCH frame offset" delete. 3G99-8.6.6.14(3.2.4)

===========================================================================*/


/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "err.h"
#include "tdsl1rrcif.h"
#include "tdsl1mtask.h"
#include "tdsmacrrcif.h"
#include "msg.h"
#include "tdsrlcrrcif.h"
#include "tdsrrcasn1.h"
#include "tdsrrccsp.h"
#include "tdsrrcdata.h"
#include "tdsrrcdata_v.h"
#include "tdsrrccommon.h"
#include "tdsrrcllc.h"
#include "tdsrrcllci.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsibdb.h"
#include "tdsuecomdef.h"
#include "tdsrrclcm.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcdt.h"
#include "tdsrrcccm.h"
#include "tdsrrcsmc.h"
#if (defined FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM) || (defined FEATURE_TDSCDMA_RRC_RB_PRIORITY_OPT)
#include "tdsrrciho.h"
#endif

#include "tdsrrcnv.h"
#include "tdsseq.h"
#include "event.h"

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
#include "tdsrrcrbe.h"
#include "tdsrrcrbreconfig.h"
#include "tdsrrcrbr.h"
#include "tdsrrctcreconfig.h"
#include "tdsrrcpcreconfig.h"

extern tdsrrcrbe_int_data_type tdsrrc_rbe;
extern tdsrrcrbrc_int_data_type tdsrrc_rbrc;
extern tdsrrcrbr_int_data_type tdsrrc_rbr;
extern tdsrrctcrc_int_data_type tdsrrc_tcrc;
extern tdsrrcpcrc_int_data_type tdsrrc_pcrc;
#endif

#ifdef FEATURE_TDSCDMA_RRC_RB_PRIORITY_OPT
extern tdsrrciho_g2td_info_type g2td_info;
#endif

/*===================================================================
                        FORWARD DECLARATIONS
=====================================================================*/

/*===================================================================
                        DATA DECLARATIONS
=====================================================================*/

tdsrrcllc_transition_config_type  tdstransition_config;
tdsordered_config_type *tdsordered_config_ptr;
tdsordered_config_type *tdscurrent_config_ptr;

tdsrrc_state_e_type  tdsrrc_ordered_state = TDSRRC_STATE_MAX;

/* Used for configuration of lower layers during cell reselection */
tdsordered_config_type *tdsreselection_config_ptr;

/* This has complete status of Ordered Config */
tdsrrcllc_oc_complete_status_type  tdsordered_config;

tdsrrcllc_config_e_type tdsconfig_ptr_in_use;

boolean tds_is_sib5_change_in_progress = FALSE;


tdsrrc_hsdpa_msg_struct_type tdshsdpa_msg_params;

boolean tdstmp_dl_dlfow_index_in_use[UE_MAX_MAC_D_FLOW];

tdsmac_dflow_struct_type    tdstmp_mac_dflow_info[UE_MAX_MAC_D_FLOW];

tdsmac_hs_queue_struct_type tdstmp_queue_info[UE_MAX_HS_QUEUE];
/*extern tdsrrcllc_rrc_cmd_process_data_type  tdsrrcllc_cmd_under_process;

extern tdsrrcllc_rrc_cmd_process_data_type  tdsrrcllc_cmd_under_process;*/

#ifdef FEATURE_TDSCDMA_MIMO
tdsrrc_mimo_status_enum_type tdsrrc_mimo_status = TDSRRC_MIMO_INACTIVE;
#endif /*FEATURE_TDSCDMA_MIMO*/

#ifdef FEATURE_TDS_DC_HSDPA
tdsrrc_dc_hsdpa_status_enum_type tdsrrc_dc_hsdpa_status = TDSRRC_DC_HSDPA_INACTIVE;
#endif /*FEATURE_TDSCDMA_DC_HSDPA*/


/*tdsrrc_rb_list_mapped_to_dch is used to store the list of RBs that are 
mapped to DCH and for whcih RLC PDU size has changed. This list is 
used by procedures while sending channel config req to LLC.*/
tdsrrcllc_rb_list_mapped_to_dch_type tdsrrc_rb_list_mapped_to_dch;

/* Booleans kepts for checking of invalid config as per 25.331 8.6.5.2
If the IE "Transport format combination set" is not included for either the uplink or the downlink 
and for that direction after the reconfiguration there is one or more DCH; and
1> if no transport format combination set is stored in the UE; or
1> if transport channels are added or removed in the message; or
1> if any transport channel is reconfigured in the message such that the size of the 
   transport format set is changed: 
the UE shall:
1> set the variable TDSL1_INVALID_CONFIGURATION to TRUE.
*/
boolean tdsul_tfcs_needed = FALSE;
boolean tdsdl_tfcs_needed = FALSE;
boolean tdsul_tfcs_rcvd = FALSE;
boolean tdsdl_tfcs_rcvd = FALSE;

/********************************************************************
* End TDSORDERED_CONFIG definition
*********************************************************************/

/*===================================================================
                        CONSTANTS
=====================================================================*/
/*
* The following constants are used when determining the Logical
* channel mapping information for the vocoder AMR Modes and Classes.
* Some of these are not actually used in the code anywhere, but it's
* nice to have them here for completeness.  The unused ones are
* commented out to prevent lint complaints.
*/
/* #define WVS_AMR_MODE_CLASS_C_0_BIT_SIZE     0 */

/* AMR 4.75 kbps speech */
#define TDSWVS_AMR_MODE_475_CLASS_A_BIT_SIZE  42
#define TDSWVS_AMR_MODE_475_CLASS_B_BIT_SIZE  53
/* #define WVS_AMR_MODE_475_CLASS_C_BIT_SIZE  WVS_AMR_MODE_CLASS_C_0_BIT_SIZE */

/* AMR 5.15 kbps speech */
#define TDSWVS_AMR_MODE_515_CLASS_A_BIT_SIZE  49
#define TDSWVS_AMR_MODE_515_CLASS_B_BIT_SIZE  54
/* #define WVS_AMR_MODE_515_CLASS_C_BIT_SIZE  WVS_AMR_MODE_CLASS_C_0_BIT_SIZE */

/* AMR 5.90 kbps speech */
#define TDSWVS_AMR_MODE_590_CLASS_A_BIT_SIZE  55
#define TDSWVS_AMR_MODE_590_CLASS_B_BIT_SIZE  63
/* #define WVS_AMR_MODE_590_CLASS_C_BIT_SIZE  WVS_AMR_MODE_CLASS_C_0_BIT_SIZE */

/* AMR 6.70 kbps speech */
#define TDSWVS_AMR_MODE_670_CLASS_A_BIT_SIZE  58
#define TDSWVS_AMR_MODE_670_CLASS_B_BIT_SIZE  76
/* #define WVS_AMR_MODE_670_CLASS_C_BIT_SIZE  WVS_AMR_MODE_CLASS_C_0_BIT_SIZE */

/* AMR 7.40 kbps speech */
#define TDSWVS_AMR_MODE_740_CLASS_A_BIT_SIZE  61
#define TDSWVS_AMR_MODE_740_CLASS_B_BIT_SIZE  87
/* #define WVS_AMR_MODE_740_CLASS_C_BIT_SIZE  WVS_AMR_MODE_CLASS_C_0_BIT_SIZE */

/* AMR 7.95 kbps speech */
#define TDSWVS_AMR_MODE_795_CLASS_A_BIT_SIZE  75
#define TDSWVS_AMR_MODE_795_CLASS_B_BIT_SIZE  84
/* #define WVS_AMR_MODE_795_CLASS_C_BIT_SIZE  WVS_AMR_MODE_CLASS_C_0_BIT_SIZE */

/* AMR 10.2 kbps speech */
#define TDSWVS_AMR_MODE_102_CLASS_A_BIT_SIZE  65
#define TDSWVS_AMR_MODE_102_CLASS_B_BIT_SIZE  99
#define TDSWVS_AMR_MODE_102_CLASS_C_BIT_SIZE  40

/* AMR 12.2 kbps speech */
#define TDSWVS_AMR_MODE_122_CLASS_A_BIT_SIZE  81
#define TDSWVS_AMR_MODE_122_CLASS_B_BIT_SIZE 103
#define TDSWVS_AMR_MODE_122_CLASS_C_BIT_SIZE  60

/* AMR Mode 8 (SID) */
#define TDSRRCLLC_AMR_MODE_8_CLASS_A_BIT_SIZE   39
/* #define RRCLLC_AMR_MODE_8_CLASS_B_BIT_SIZE    0 */
/* #define RRCLLC_AMR_MODE_8_CLASS_C_BIT_SIZE    WVS_AMR_MODE_CLASS_C_0_BIT_SIZE */

/* AMR Mode 15 (SID) */
/* #define RRCLLC_AMR_MODE_15_CLASS_A_BIT_SIZE   0 */
/* #define RRCLLC_AMR_MODE_15_CLASS_B_BIT_SIZE   0 */
/* #define RRCLLC_AMR_MODE_15_CLASS_C_BIT_SIZE   WVS_AMR_MODE_CLASS_C_0_BIT_SIZE */

#define TDSRRCLLC_AMR_WB_CLASS_NO_DATA_FRAME_SIZE 0

#define TDSRRCLLC_AMR_WB_CLASS_A_BIT_SIZE  40


#define  TDSWVS_AMR_WB_MODE_475_TOTAL_BITS  (TDSWVS_AMR_MODE_475_CLASS_A_BIT_SIZE + \
                                                                                 TDSWVS_AMR_MODE_475_CLASS_B_BIT_SIZE)
#define  TDSWVS_AMR_MODE_515_TOTAL_BITS (TDSWVS_AMR_MODE_515_CLASS_A_BIT_SIZE + \
                                                                          TDSWVS_AMR_MODE_515_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_MODE_590_TOTAL_BITS (TDSWVS_AMR_MODE_590_CLASS_A_BIT_SIZE + \
                                                                          TDSWVS_AMR_MODE_590_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_MODE_670_TOTAL_BITS (TDSWVS_AMR_MODE_670_CLASS_A_BIT_SIZE + \
                                                                          TDSWVS_AMR_MODE_670_CLASS_B_BIT_SIZE) 
#define TDSWVS_AMR_MODE_740_TOTAL_BITS (TDSWVS_AMR_MODE_740_CLASS_A_BIT_SIZE + \
                                                                         TDSWVS_AMR_MODE_740_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_MODE_795_TOTAL_BITS (TDSWVS_AMR_MODE_795_CLASS_A_BIT_SIZE + \
                                                                         TDSWVS_AMR_MODE_795_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_MODE_102_TOTAL_BITS (TDSWVS_AMR_MODE_102_CLASS_A_BIT_SIZE + \
             TDSWVS_AMR_MODE_102_CLASS_B_BIT_SIZE + TDSWVS_AMR_MODE_102_CLASS_C_BIT_SIZE)
#define TDSWVS_AMR_MODE_122_TOTAL_BITS (TDSWVS_AMR_MODE_122_CLASS_A_BIT_SIZE + \
             TDSWVS_AMR_MODE_122_CLASS_B_BIT_SIZE + TDSWVS_AMR_MODE_122_CLASS_C_BIT_SIZE)


/* WB_AMR  6.60 kbps Speech carries 132 bits in total */
#define TDSWVS_AMR_WB_MODE_660_CLASS_A_BIT_SIZE  54
#define TDSWVS_AMR_WB_MODE_660_CLASS_B_BIT_SIZE  78

/* WB_AMR  8.85 kbps Speech carries 177 bits in total */
#define TDSWVS_AMR_WB_MODE_885_CLASS_A_BIT_SIZE  64
#define TDSWVS_AMR_WB_MODE_885_CLASS_B_BIT_SIZE  113

/* WB_AMR  12.65, 4.25. 15.85, 18.25, 19.85. 23.05,23.85 
 * kbps Speech 
 */
#define TDSWVS_AMR_WB_MODE_1265_TO_2385_CLASS_A_BIT_SIZE 72

/* WB_AMR  12.65 kbps Speech carries 253 bits in total */
#define TDSWVS_AMR_WB_MODE_1265_CLASS_B_BIT_SIZE 181

/* WB_AMR  14.25 kbps Speech carries 285 bits in total */
#define TDSWVS_AMR_WB_MODE_1425_CLASS_B_BIT_SIZE 213

/* WB_AMR  15.85 kbps Speech carries 317 bits in total */
#define TDSWVS_AMR_WB_MODE_1585_CLASS_B_BIT_SIZE 245

/* WB_AMR  18.25 kbps Speech carries 365 bits in total */
#define TDSWVS_AMR_WB_MODE_1825_CLASS_B_BIT_SIZE 293

/* WB_AMR  19.85 kbps Speech carries 397 bits in total */
#define TDSWVS_AMR_WB_MODE_1985_CLASS_B_BIT_SIZE 325

/* WB_AMR  23.05 kbps Speech carries 461 bits in total */
#define TDSWVS_AMR_WB_MODE_2305_CLASS_B_BIT_SIZE 389

/* WB_AMR  23.85 kbps Speech carries 477 bits in total */
#define TDSWVS_AMR_WB_MODE_2385_CLASS_B_BIT_SIZE 405

#define TDSWVS_AMR_WB_MODE_660_TOTAL_BITS (TDSWVS_AMR_WB_MODE_660_CLASS_A_BIT_SIZE + \
                                                                            TDSWVS_AMR_WB_MODE_660_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_WB_MODE_885_TOTAL_BITS (TDSWVS_AMR_WB_MODE_885_CLASS_A_BIT_SIZE + \
                                                                            TDSWVS_AMR_WB_MODE_885_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_WB_MODE_1265_TOTAL_BITS (TDSWVS_AMR_WB_MODE_1265_TO_2385_CLASS_A_BIT_SIZE + \
                                                                           TDSWVS_AMR_WB_MODE_1265_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_WB_MODE_1425_TOTAL_BITS (TDSWVS_AMR_WB_MODE_1265_TO_2385_CLASS_A_BIT_SIZE+  \
                                                                           TDSWVS_AMR_WB_MODE_1425_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_WB_MODE_1585_TOTAL_BITS (TDSWVS_AMR_WB_MODE_1265_TO_2385_CLASS_A_BIT_SIZE + \
                                                                           TDSWVS_AMR_WB_MODE_1585_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_WB_MODE_1825_TOTAL_BITS (TDSWVS_AMR_WB_MODE_1265_TO_2385_CLASS_A_BIT_SIZE +  \
                                                                           TDSWVS_AMR_WB_MODE_1825_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_WB_MODE_1985_TOTAL_BITS (TDSWVS_AMR_WB_MODE_1265_TO_2385_CLASS_A_BIT_SIZE +  \
                                                                           TDSWVS_AMR_WB_MODE_1985_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_WB_MODE_2305_TOTAL_BITS (TDSWVS_AMR_WB_MODE_1265_TO_2385_CLASS_A_BIT_SIZE +  \
                                                                         TDSWVS_AMR_WB_MODE_2305_CLASS_B_BIT_SIZE)
#define TDSWVS_AMR_WB_MODE_2385_TOTAL_BITS (TDSWVS_AMR_WB_MODE_1265_TO_2385_CLASS_A_BIT_SIZE +  \
                                                                         TDSWVS_AMR_WB_MODE_2385_CLASS_B_BIT_SIZE)

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
#define TDSRRCLLC_INVALID_TRCH_ID                0xFFFF
#define TDSRRCLLC_INVALID_LC_ID                0xFFFF
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

#define TDSRRC_AMR_RATE_SIZE 9
#define TDSRRC_NO_IMSI_DRX_LENGTH 256

/*===================================================================
                        FORWARD DECLARATIONS
=====================================================================*/


/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this  
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */



/*====================================================================
FUNCTION: tdsrrc_query_crnti()

DESCRIPTION:
  This function returns the C-RNTI if it is valid in the Current Config.
  If Current Config does not have a valid CRNTI but a valid CRNTI exists
  in Ordered Config, that is understood as a valid CRNTI status.

DEPENDENCIES:
  This will be called by L1 to do GSM measurementse in CONNECTED state.

RETURN VALUE:
  boolean - TRUE indicates C-RNTI value will be filled in the c-rnti pointer
            FALSE indicates no C-RNTI

SIDE EFFECTS:
  None.
====================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
boolean
tdsrrc_query_crnti(uint16 *c_rnti)
{

  /* If lower layers are already configured with OC, check OC before
     Checking CC */

  tdsrrcllc_oc_set_status_e_type oc_status = tdsrrcllc_get_ordered_config_status();

  if ((oc_status != TDSOC_NOT_SET ) &&
      (tdsordered_config.process_state == TDSLL_CONFIG_WITH_OC) &&
      ((tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSCRNTI_VALID)||
       (tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID)) )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Valid C-RNTI in OC. c-rnti: %d",
             tdsordered_config_ptr->mac_dl_parms.rnti_info.crnti);
    *c_rnti = tdsordered_config_ptr->mac_dl_parms.rnti_info.crnti;
    return TRUE;
  }
  else if ((tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSCRNTI_VALID)||
           (tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
  {

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Valid C-RNTI in CC. c-rnti: %d",
             tdscurrent_config_ptr->mac_dl_parms.rnti_info.crnti);
    *c_rnti = tdscurrent_config_ptr->mac_dl_parms.rnti_info.crnti;
    return TRUE;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No Valid C-RNTI in CC or OC");
    return FALSE;
  }  /* No valid C_RNTI in Current Config */
} /* tdsrrc_query_crnti */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*====================================================================
FUNCTION: tdsrrcllc_get_ordered_config_status()

DESCRIPTION:
  This function checks if TDSORDERED_CONFIG is set. If so, it
  returns TRUE. If it's not set it returns FALSE.

DEPENDENCIES:
  None.

RETURN VALUE:
  Returns a value that indicates if Ordered Config is set for
  an RRC state or not set.

SIDE EFFECTS:
  None.
====================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

tdsrrcllc_oc_set_status_e_type tdsrrcllc_get_ordered_config_status( void )
{
  TDSRRC_MSG2(MSG_LEGACY_MED,"Get OC status %d proc %d",
           tdsordered_config.set_status, tdsordered_config.set_by_proc);
  /* return the status of tdsordered_config.set_status variable */
  return tdsordered_config.set_status;

} /* end tdsrrcllc_get_ordered_config_status() */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*====================================================================
FUNCTION: tdsrrcllc_get_drx_info()

DESCRIPTION:
  This function returns the shorter of the PS or CS CN DRX Cycle
  Length.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32 of the DRX Cycle length, per 25.304.

SIDE EFFECTS:
  This function should only be called after the following function
  calls:
  - tdsrrcllc_copy_sib_to_oc() or
  - tdsrrcllc_set_ordered_config().
====================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

uint32 tdsrrcllc_get_drx_info(void)
{
   uint32 cs_drx = 0;
   uint32 ps_drx = 0;
   uint32 utran_drx = 0;
   tdsrrcllc_oc_set_status_e_type oc_status;
 
   uint32 cc_pbp = TDSRRC_CONV_PICH_REP_LEN(tdscurrent_config_ptr->l1_dl_chan_parms.pich.repetition_period_length);
   uint32 oc_pbp = TDSRRC_CONV_PICH_REP_LEN(tdsordered_config_ptr->l1_dl_chan_parms.pich.repetition_period_length);
   uint64 tmp_imsi_id = tdsrrc_convert_gsm_map_imsi_into_int_value(tdsrrc_imsi);

   TDSRRC_MSG3(MSG_LEGACY_HIGH, "IMSI is valid %d (1:TRUE;0:FALSE),IMSI value is %d and %d",tdsrrc_imsi_valid,QWORD_HIGH(tmp_imsi_id),QWORD_LOW(tmp_imsi_id));
   if (FALSE == tdsrrc_imsi_valid)
   {
     /*Per 25.304 clause 8.3 , if NO IMSI , take IMSI=0 and DRX length=256(2.56s) */
     TDSRRC_MSG1(MSG_LEGACY_HIGH, "No IMSI return DRX length %d",TDSRRC_NO_IMSI_DRX_LENGTH);
     return TDSRRC_NO_IMSI_DRX_LENGTH;
   }
   /* Initialize TDD CS PS DRX in similar way as in FDD */
   cs_drx = MAX(tdscurrent_config_ptr->cs_drx_cycle_length, cc_pbp);
   ps_drx = MAX(tdscurrent_config_ptr->ps_drx_cycle_length, cc_pbp);
  
   oc_status = tdsrrcllc_get_ordered_config_status();
   /*get the UTRAN DRX cycle length coef only if the OC is
   set for Cell_PCH or URA_PCH state transition */
   if((oc_status == TDSOC_SET_FOR_DCH_CELL_PCH_TRANS) ||
      (oc_status == TDSOC_SET_FOR_DCH_URA_PCH_TRANS) 
      ||(tdstransition_config.toc_usage == TDSTOC_FOR_OOS)
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
       || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITHOUT_DCH_INFO)
       || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITH_DCH_INFO)
       || (tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH)
       || (tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH_WITH_DCH_INFO)
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      )
   {
     uint32 tc_pbp = TDSRRC_CONV_PICH_REP_LEN(tdstransition_config.toc_ptr->l1_dl_chan_parms.pich.repetition_period_length);
     
     /* Read the UTRAN DRX cycle from TOC */
     /* This will take care of DCH->PCH state transition */
     utran_drx = MAX(tdstransition_config.toc_ptr->utran_drx_cycle_length, tc_pbp);
     cs_drx = MAX(tdsordered_config_ptr->cs_drx_cycle_length, oc_pbp);
     ps_drx = MAX(tdsordered_config_ptr->ps_drx_cycle_length, oc_pbp);
   }
   else if((oc_status == TDSOC_SET_FOR_FACH_CELL_PCH_TRANS) ||
      (oc_status == TDSOC_SET_FOR_FACH_URA_PCH_TRANS))
   {
     /* This will take care of FACH->PCH state transition with a message
   that sets ordered_reconfig variable */
     utran_drx = MAX(tdsordered_config_ptr->utran_drx_cycle_length, oc_pbp);
   }
   else
   {
      /* TDS: exclude idle state condition */
      if ((tdscurrent_config_ptr->utran_drx_cycle_length == 0) && 
          (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
         utran_drx = 0;
      }
      else
      {
     /* this will take care of FACH->PCH state transition
   with a message that does not set the ordered_reconfig variable*/
     utran_drx = MAX(tdscurrent_config_ptr->utran_drx_cycle_length, cc_pbp);
      }
   }

  /* We may not have a UTRAN DRX cycle length, so we must not consider
     it in this case.*/

   TDSRRC_MSG3(MSG_LEGACY_HIGH,"DRX cs:%d, ps:%d, utran:%d", cs_drx,ps_drx, utran_drx);
  if(utran_drx == 0)
  {
    /* Return the DRX Cycle Length */
    if (cs_drx < ps_drx)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Using cs drx %d", cs_drx);
      return cs_drx;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Using ps drx %d", ps_drx);
      return ps_drx;
    }
  }
  else /* this is non idle case so must be cell_PCH/URA_PCH case */
  {
    /* find out the un-established domain.*/
     if ((tdsrrcscr_get_signalling_connection_status (RRC_PS_DOMAIN_CN_ID) == FALSE) &&
         (tdsrrcscr_get_signalling_connection_status (RRC_CS_DOMAIN_CN_ID) == TRUE))
     {
       if (ps_drx < utran_drx)
       {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS & UTRAN Using PS drx %d", ps_drx);
         return ps_drx;
       }
       else
       {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS & UTRAN Using utran drx %d", utran_drx);
         return utran_drx;
       }
       //return (ps_drx<utran_drx?ps_drx:utran_drx);
     }
     else if ((tdsrrcscr_get_signalling_connection_status (RRC_CS_DOMAIN_CN_ID) == FALSE) &&
              (tdsrrcscr_get_signalling_connection_status (RRC_PS_DOMAIN_CN_ID) == TRUE))
     {
       //return (cs_drx<utran_drx?cs_drx:utran_drx);
       if (cs_drx < utran_drx)
       {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS & UTRAN Using CS drx %d", cs_drx);
         return cs_drx;
       }
       else
       {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS & UTRAN Using utran drx %d", utran_drx);
         return utran_drx;
       }
     }
     else if ((tdsrrcscr_get_signalling_connection_status (RRC_CS_DOMAIN_CN_ID) == TRUE) &&
              (tdsrrcscr_get_signalling_connection_status (RRC_PS_DOMAIN_CN_ID) == TRUE))
     {
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS & PS connected. Using utran drx %d", utran_drx);
       return (utran_drx);
     }
     else /*((tdsrrcscr_get_signalling_connection_status (RRC_CS_DOMAIN_CN_ID) == FALSE) &&
              (tdsrrcscr_get_signalling_connection_status (RRC_PS_DOMAIN_CN_ID) == FALSE))*/
     {
       if(cs_drx <= ps_drx && cs_drx <= utran_drx)
       {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"No PS or CS Using CS drx %d", cs_drx);
         return cs_drx;
       }
       else if(ps_drx <= cs_drx && ps_drx <= utran_drx)
       {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"No PS or CS Using ps drx %d", ps_drx);
         return ps_drx;
       }
       else
       {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"No PS or CS Using utran drx %d", utran_drx);
         return utran_drx;
       }
     }
  }
} /* end tdsrrcllc_get_drx_info() */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/ /* end tdsrrcllc_get_drx_info() */

#if defined(FEATURE_3GPP_FAST_DORMANCY)
/*====================================================================
FUNCTION: tdsrrcllc_get_drx_info_values()

DESCRIPTION:
  This function returns the values of the PS/CS CN DRX Cycle and DRX
  cycle in use Length.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
  This function should only be called after the following function
  calls:
  - tdsrrcllc_copy_sib_to_oc() or
  - tdsrrcllc_set_ordered_config().
====================================================================*/
void tdsrrcllc_get_drx_info_values
(
  uint32 *cs_drx_ptr,
  uint32 *ps_drx_ptr,
  uint32 *drx_in_use
)
{
  uint32 cs_drx = tdscurrent_config_ptr->cs_drx_cycle_length;
  uint32 ps_drx = tdscurrent_config_ptr->ps_drx_cycle_length;
  tdsrrcllc_oc_set_status_e_type oc_status;

  oc_status = tdsrrcllc_get_ordered_config_status();
  
  /* Get the UTRAN DRX cycle length coef only if the OC is
     * set for Cell_PCH or URA_PCH state transition */
  if((oc_status == TDSOC_SET_FOR_DCH_CELL_PCH_TRANS) 
     || (oc_status == TDSOC_SET_FOR_DCH_URA_PCH_TRANS) 
     || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS)
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
     || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITHOUT_DCH_INFO)
     || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITH_DCH_INFO)
     || (tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH)
     || (tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH_WITH_DCH_INFO)
#endif 
    )
  {
    /* This will take care of DCH->PCH state transition */
    cs_drx = tdsordered_config_ptr->cs_drx_cycle_length;
    ps_drx = tdsordered_config_ptr->ps_drx_cycle_length;
  }

  *cs_drx_ptr = cs_drx;
  *ps_drx_ptr = ps_drx;
  *drx_in_use = tdsrrcllc_get_drx_info();

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"DRX cs:%d, ps:%d, in_use:%d", cs_drx, ps_drx, *drx_in_use);
}
#endif 
/* jb 3.4.10 begin , this function is called by L1 , shoule be deleted */
/*====================================================================
FUNCTION: tdsrrc_ul_tx_pwr_ind()

DESCRIPTION:
  This function places the values of cpich tx power, ul interference,
  and a constant value defined in PRACH sys info, into the addresses
  passed into this function.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_ul_tx_pwr_ind
(
  int8 *cpich_tx_pwr,
  int8 *ul_if,
  int8 *const_val
)
{

  return;

} /* end tdsrrc_ul_tx_pwr_ind() */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/* jb 3.4.10 end */

/*====================================================================
FUNCTION: tdsrrcllc_process_get_cpid_r4

DESCRIPTION:
  This function get cpid present and cpid same as current of r4 msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_process_get_cpid_r4
(
  tdsrrc_DL_InformationPerRL_List_r4 * dl_InformationPerRL_List,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg   /* either new CELL PARAMETER ID is present and different with current */
)
{

  /* Get the CELL PARAMETER ID */
  if ((dl_InformationPerRL_List->elem[0].modeSpecificInfo.t == 
       T_tdsrrc_DL_InformationPerRL_r4_modeSpecificInfo_tdd)&&
       (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->t == 
               T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
  {
    if (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      *cpid_flg = TRUE;
      if (tdsrrcccm_get_curr_camped_cpid() != dl_InformationPerRL_List->elem[0].
                 modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
      {
        *cpid_change_flg = TRUE;
      }
    }
  }
  return SUCCESS;
} /* tdsrrcllc_process_get_cpid_r4 */

/*====================================================================
FUNCTION: tdsrrcllc_process_get_cpid_r5

DESCRIPTION:
  This function get cpid present and cpid same as current of r5 msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_process_get_cpid_r5
(
  tdsrrc_DL_InformationPerRL_List_r5 * dl_InformationPerRL_List,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg   /* either new CELL PARAMETER ID is present and different with current */
)
{

  /* Get the CELL PARAMETER ID */
  if ((dl_InformationPerRL_List->elem[0].modeSpecificInfo.t == 
       T_tdsrrc_DL_InformationPerRL_r5_modeSpecificInfo_tdd)&&
       (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->t == 
               T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
  {
    if (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      *cpid_flg = TRUE;
      if (tdsrrcccm_get_curr_camped_cpid() != dl_InformationPerRL_List->elem[0].
                 modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
      {
        *cpid_change_flg = TRUE;
      }
    }
  }
  return SUCCESS;
} /* tdsrrcllc_process_get_cpid_r5 */

/*====================================================================
FUNCTION: tdsrrcllc_process_get_cpid_r6

DESCRIPTION:
  This function get cpid present and cpid same as current of r6 msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_process_get_cpid_r6
(
  tdsrrc_DL_InformationPerRL_List_r6 * dl_InformationPerRL_List,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg  /* either new CELL PARAMETER ID is present and different with current */
)
{

  /* Get the CELL PARAMETER ID */
  if ((dl_InformationPerRL_List->elem[0].modeSpecificInfo.t == 
       T_tdsrrc_DL_InformationPerRL_r6_modeSpecificInfo_tdd)&&
       (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->t == 
               T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
  {
    if (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      *cpid_flg = TRUE;
      if (tdsrrcccm_get_curr_camped_cpid() != dl_InformationPerRL_List->elem[0].
                 modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
      {
        *cpid_change_flg = TRUE;
      }
    }
  }
  return SUCCESS;
} /* tdsrrcllc_process_get_cpid_r6 */

/*====================================================================
FUNCTION: tdsrrcllc_process_get_cpid_r7

DESCRIPTION:
  This function get cpid present and cpid same as current of r7 msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_process_get_cpid_r7
(
  tdsrrc_DL_InformationPerRL_List_r7 * dl_InformationPerRL_List,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg  /* either new CELL PARAMETER ID is present and different with current */
)
{

  /* Get the CELL PARAMETER ID */
  if ((dl_InformationPerRL_List->elem[0].modeSpecificInfo.t == 
       T_tdsrrc_DL_InformationPerRL_r7_modeSpecificInfo_tdd)&&
       (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->t == 
               T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
  {
    if (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      *cpid_flg = TRUE;
      if (tdsrrcccm_get_curr_camped_cpid() != dl_InformationPerRL_List->elem[0].
                 modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
      {
        *cpid_change_flg = TRUE;
      }
    }
  }
  return SUCCESS;
} /* tdsrrcllc_process_get_cpid_r7 */
#ifdef FEATURE_TDSCDMA_REL8
/*====================================================================
FUNCTION: tdsrrcllc_process_get_cpid_r8

DESCRIPTION:
  This function get cpid present and cpid same as current of r8 msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_process_get_cpid_r8
(
  tdsrrc_DL_InformationPerRL_List_r8 * dl_InformationPerRL_List,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg  /* either new CELL PARAMETER ID is present and different with current */
)
{

  /* Get the CELL PARAMETER ID */
  if ((dl_InformationPerRL_List->elem[0].modeSpecificInfo.t == 
       T_tdsrrc_DL_InformationPerRL_r8_modeSpecificInfo_tdd)&&
       (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->t == 
               T_tdsrrc_PrimaryCCPCH_Info_r4_tdd))
  {
    if (dl_InformationPerRL_List->elem[0].modeSpecificInfo.u.tdd->u.tdd->m.cellParametersIDPresent)
    {
      *cpid_flg = TRUE;
      if (tdsrrcccm_get_curr_camped_cpid() != dl_InformationPerRL_List->elem[0].
                 modeSpecificInfo.u.tdd->u.tdd->cellParametersID)
      {
        *cpid_change_flg = TRUE;
      }
    }
  }
  return SUCCESS;
} /* tdsrrcllc_process_get_cpid_r8 */
#endif

/*====================================================================
FUNCTION: tdsrrcllc_process_ccch_cu_get_msg_ie_flg

DESCRIPTION:
  This function processes the Cell Update Confirm msg  received on CCCH based on the REL version and
  get CELL PARAMETER ID, NEW C_RNTI, NEW H_RNTI present by Cell Update  msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_process_ccch_cu_get_msg_ie_flg
(
  void *msg_ptr,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg,  /* either new CELL PARAMETER ID is present and different with current */
  boolean *crnti_flg,   /* NEW C_RNTI present flag */
  boolean *hrnti_flg,   /* NEW H_RNTI present flag */
  boolean *ernti_flg    /* NEW E_RNTI present flag */
)
{
  tdsrrc_DL_CCCH_Message *ccch_ptr;

  tdsrrc_CellUpdateConfirm_r4_IEs   *ccch_ptr_r4;
  tdsrrc_CellUpdateConfirm_r5_IEs   *ccch_ptr_r5;
  tdsrrc_CellUpdateConfirm_r6_IEs   *ccch_ptr_r6;
  tdsrrc_CellUpdateConfirm_r7_IEs   *ccch_ptr_r7;
#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_CellUpdateConfirm_r8_IEs   *ccch_ptr_r8;
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  tdsrrc_CellUpdateConfirm_r9_IEs   *ccch_ptr_r9;
#endif /* FEATURE_TDSCDMA_REL9 */

  uecomdef_status_e_type status = SUCCESS;
  ccch_ptr = (tdsrrc_DL_CCCH_Message *)msg_ptr;

  if ((NULL == cpid_flg)||(NULL == cpid_change_flg)||(NULL == crnti_flg)||(NULL == hrnti_flg)||(NULL == ernti_flg))
  {
    ERR_FATAL("Point is NULL", 0,0,0);
  }
  *cpid_flg = FALSE;
  *cpid_change_flg = FALSE;
  *crnti_flg = FALSE;
  *hrnti_flg = FALSE;
  *ernti_flg = FALSE;

  if(SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm, TDSMSG_REL4))
  {
    ccch_ptr_r4 = &ccch_ptr->message.u.cellUpdateConfirm.u.
                later_than_r3.criticalExtensions.u.r4.cellUpdateConfirm_r4;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r4, tdsrrc_CellUpdateConfirm_r4_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r4,
        tdsrrc_CellUpdateConfirm_r4_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r4(&ccch_ptr_r4->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if(SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm, TDSMSG_REL5))
  {
    ccch_ptr_r5 = &ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                   criticalExtensions.u.criticalExtensions.
                    u.r5.cellUpdateConfirm_r5;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r5, tdsrrc_CellUpdateConfirm_r5_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r5, tdsrrc_CellUpdateConfirm_r5_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r5,
        tdsrrc_CellUpdateConfirm_r5_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r5(&ccch_ptr_r5->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm, TDSMSG_REL6))
  {
    ccch_ptr_r6 = &ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                          criticalExtensions.u.criticalExtensions.u.criticalExtensions.
                          u.r6.cellUpdateConfirm_r6;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r6, tdsrrc_CellUpdateConfirm_r6_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r6, tdsrrc_CellUpdateConfirm_r6_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r6, tdsrrc_CellUpdateConfirm_r6_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r6,
        tdsrrc_CellUpdateConfirm_r6_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r6(&ccch_ptr_r6->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm, TDSMSG_REL7))
  {
    ccch_ptr_r7 = &ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                    criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
                            u.r7.cellUpdateConfirm_r7;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r7, tdsrrc_CellUpdateConfirm_r7_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r7, tdsrrc_CellUpdateConfirm_r7_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r7, tdsrrc_CellUpdateConfirm_r7_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r7,
        tdsrrc_CellUpdateConfirm_r7_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r7(&ccch_ptr_r7->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
    else if (SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
             T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm, TDSMSG_REL8))
    {
      ccch_ptr_r8 = &ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
                              u.criticalExtensions.u.r8.cellUpdateConfirm_r8;
      /* Get the C-RNTI */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r8, tdsrrc_CellUpdateConfirm_r8_IEs, new_C_RNTI))
      {
        *crnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r8, tdsrrc_CellUpdateConfirm_r8_IEs, new_H_RNTI))
      {
        *hrnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r8, tdsrrc_CellUpdateConfirm_r8_IEs, newPrimary_E_RNTI))
      {
        *ernti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r8,
          tdsrrc_CellUpdateConfirm_r8_IEs, dl_InformationPerRL_List))
      {
        /* Get the CELL PARAMETER ID */
        tdsrrcllc_process_get_cpid_r8(&ccch_ptr_r8->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
      }
    }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  else if (SUCCESS == tdsrrc_ccch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm, TDSMSG_REL9))
  {
    ccch_ptr_r9 = &ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                    criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
                            u.criticalExtensions.u.criticalExtensions.u.r9.cellUpdateConfirm_r9;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r9, tdsrrc_CellUpdateConfirm_r7_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r9, tdsrrc_CellUpdateConfirm_r7_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r9, tdsrrc_CellUpdateConfirm_r7_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ccch_ptr_r9,
        tdsrrc_CellUpdateConfirm_r9_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r8(&ccch_ptr_r9->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#endif /* FEATURE_TDSCDMA_REL9 */
  else
  {
    ERR_FATAL("Invalid CCCH Message %d rcv'd", ccch_ptr->message.t,0,0);
  }

  return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_pcrc_get_msg_ie_flg

DESCRIPTION:
  This function processes the Physical Channel Reconfig msg based on the REL version and
  get CELL PARAMETER ID, NEW C_RNTI, NEW H_RNTI present by Physical Channel Reconfig msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_pcrc_get_msg_ie_flg
(
  void *msg_ptr,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg,  /* either new CELL PARAMETER ID is present and different with current */
  boolean *crnti_flg,   /* NEW C_RNTI present flag */
  boolean *hrnti_flg,   /* NEW H_RNTI present flag */
  boolean *ernti_flg    /* NEW E_RNTI present flag */
)
{
  /* local variables */
  uecomdef_status_e_type  status = SUCCESS;

  tdsrrc_PhysicalChannelReconfiguration_r4_IEs   *dcch_ptr_r4;
  tdsrrc_PhysicalChannelReconfiguration_r5_IEs   *dcch_ptr_r5;
  tdsrrc_PhysicalChannelReconfiguration_r6_IEs   *dcch_ptr_r6;
  tdsrrc_PhysicalChannelReconfiguration_r7_IEs   *dcch_ptr_r7;
#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_PhysicalChannelReconfiguration_r8_IEs   *dcch_ptr_r8;
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  tdsrrc_PhysicalChannelReconfiguration_r9_IEs   *dcch_ptr_r9;
#endif /* FEATURE_TDSCDMA_REL9 */

  tdsrrc_DL_DCCH_Message *dcch_ptr;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if ((NULL == cpid_flg)||(NULL == cpid_change_flg)||(NULL == crnti_flg)||(NULL == hrnti_flg)||(NULL == ernti_flg))
  {
    ERR_FATAL("Point is NULL", 0,0,0);
  }
  *cpid_flg = FALSE;
  *cpid_change_flg = FALSE;
  *crnti_flg = FALSE;
  *hrnti_flg = FALSE;
  *ernti_flg = FALSE;

  if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL4))
  {
    dcch_ptr_r4 = &dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
           criticalExtensions.u.r4.physicalChannelReconfiguration_r4;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4, tdsrrc_PhysicalChannelReconfiguration_r4_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4, 
        tdsrrc_PhysicalChannelReconfiguration_r4_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r4(&dcch_ptr_r4->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL5))
  {
    dcch_ptr_r5 = &dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.
                         u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_PhysicalChannelReconfiguration_r5_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_PhysicalChannelReconfiguration_r5_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5,
        tdsrrc_PhysicalChannelReconfiguration_r5_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r5(&dcch_ptr_r5->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL6))
  {
    dcch_ptr_r6 = &dcch_ptr->message.u.physicalChannelReconfiguration.
                            u.later_than_r3.criticalExtensions.
                            u.criticalExtensions.
                            u.criticalExtensions.
                            u.r6.physicalChannelReconfiguration_r6;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_PhysicalChannelReconfiguration_r6_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_PhysicalChannelReconfiguration_r6_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_PhysicalChannelReconfiguration_r6_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6,
        tdsrrc_PhysicalChannelReconfiguration_r6_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r6(&dcch_ptr_r6->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL7))
  {
    dcch_ptr_r7 = &dcch_ptr->message.u.physicalChannelReconfiguration.
                                u.later_than_r3.criticalExtensions.
                                u.criticalExtensions.
                                u.criticalExtensions.
                                u.criticalExtensions.
                                u.r7.physicalChannelReconfiguration_r7;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_PhysicalChannelReconfiguration_r7_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_PhysicalChannelReconfiguration_r7_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_PhysicalChannelReconfiguration_r7_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7,
        tdsrrc_PhysicalChannelReconfiguration_r7_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r7(&dcch_ptr_r7->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
    else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL8))
    {
      dcch_ptr_r8 = &dcch_ptr->message.u.physicalChannelReconfiguration.
                                  u.later_than_r3.criticalExtensions.
                                  u.criticalExtensions.
                                  u.criticalExtensions.
                                  u.criticalExtensions.
                                  u.criticalExtensions.
                                  u.r8.physicalChannelReconfiguration_r8;
      /* Get the C-RNTI */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_PhysicalChannelReconfiguration_r8_IEs, new_C_RNTI))
      {
        *crnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_PhysicalChannelReconfiguration_r8_IEs, new_H_RNTI))
      {
        *hrnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_PhysicalChannelReconfiguration_r8_IEs, newPrimary_E_RNTI))
      {
        *ernti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8,
          tdsrrc_PhysicalChannelReconfiguration_r8_IEs, dl_InformationPerRL_List))
      {
        /* Get the CELL PARAMETER ID */
        tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r8->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
      }
    }
#endif /*FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL9))
  {
    dcch_ptr_r9 = &dcch_ptr->message.u.physicalChannelReconfiguration.
                                u.later_than_r3.criticalExtensions.
                                u.criticalExtensions.
                                u.criticalExtensions.
                                u.criticalExtensions.
                                u.criticalExtensions.
                                u.criticalExtensions.u.r9.physicalChannelReconfiguration_r9;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_PhysicalChannelReconfiguration_r9_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_PhysicalChannelReconfiguration_r9_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_PhysicalChannelReconfiguration_r9_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9,
        tdsrrc_PhysicalChannelReconfiguration_r9_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r9->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#endif /* FEATURE_TDSCDMA_REL9 */

  else
  {
    ERR_FATAL("Invalid DCCH Message %d rcv'd", dcch_ptr->message.t,0,0);
  }

  return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_rbrc_get_msg_ie_flg

DESCRIPTION:
  This function processes the Radio Bearer Reconfig msg based on the REL version and
  get CELL PARAMETER ID, NEW C_RNTI, NEW H_RNTI present by TRadio Bearer Reconfig msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_process_rbrc_get_msg_ie_flg
(
  void *msg_ptr,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg,  /* either new CELL PARAMETER ID is present and different with current */
  boolean *crnti_flg,   /* NEW C_RNTI present flag */
  boolean *hrnti_flg,   /* NEW H_RNTI present flag */
  boolean *ernti_flg    /* NEW E_RNTI present flag */
)
{
  /* local variables */
  tdsrrc_DL_DCCH_Message *dcch_ptr;

  tdsrrc_RadioBearerReconfiguration_r4_IEs   *dcch_ptr_r4;
  tdsrrc_RadioBearerReconfiguration_r5_IEs   *dcch_ptr_r5;
  tdsrrc_RadioBearerReconfiguration_r6_IEs   *dcch_ptr_r6;
  tdsrrc_RadioBearerReconfiguration_r7_IEs   *dcch_ptr_r7;
#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_RadioBearerReconfiguration_r8_IEs   *dcch_ptr_r8;
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  tdsrrc_RadioBearerReconfiguration_r9_IEs   *dcch_ptr_r9;
#endif /* FEATURE_TDSCDMA_REL9 */

  uecomdef_status_e_type  status = SUCCESS;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if ((NULL == cpid_flg)||(NULL == cpid_change_flg)||(NULL == crnti_flg)||(NULL == hrnti_flg)||(NULL == ernti_flg))
  {
    ERR_FATAL("Point is NULL", 0,0,0);
  }
  *cpid_flg = FALSE;
  *cpid_change_flg = FALSE;
  *crnti_flg = FALSE;
  *hrnti_flg = FALSE;
  *ernti_flg = FALSE;

  if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL4))
  {
    dcch_ptr_r4 = &dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.
          criticalExtensions.u.r4.radioBearerReconfiguration_r4;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4, tdsrrc_RadioBearerReconfiguration_r4_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4,
        tdsrrc_RadioBearerReconfiguration_r4_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r4(&dcch_ptr_r4->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if(SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL5))
  {
    dcch_ptr_r5 = &dcch_ptr->message.u.radioBearerReconfiguration.u.
                later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerReconfiguration_r5;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_RadioBearerReconfiguration_r5_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_RadioBearerReconfiguration_r5_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5,
        tdsrrc_RadioBearerReconfiguration_r5_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r5(&dcch_ptr_r5->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL6))
  {
    dcch_ptr_r6 = &dcch_ptr->message.u.radioBearerReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.
                      u.r6.radioBearerReconfiguration_r6;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_RadioBearerReconfiguration_r6_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_RadioBearerReconfiguration_r6_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_RadioBearerReconfiguration_r6_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6,
        tdsrrc_RadioBearerReconfiguration_r6_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r6(&dcch_ptr_r6->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL7))
  {
    dcch_ptr_r7 = &dcch_ptr->message.u.radioBearerReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.
                      u.r7.radioBearerReconfiguration_r7;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_RadioBearerReconfiguration_r7_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_RadioBearerReconfiguration_r7_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_RadioBearerReconfiguration_r7_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7,
        tdsrrc_RadioBearerReconfiguration_r7_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r7(&dcch_ptr_r7->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
    else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL8))
    {
      dcch_ptr_r8 = &dcch_ptr->message.u.radioBearerReconfiguration.
                        u.later_than_r3.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.criticalExtensions.
                        u.r8.radioBearerReconfiguration_r8;
      /* Get the C-RNTI */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_RadioBearerReconfiguration_r8_IEs, new_C_RNTI))
      {
        *crnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_RadioBearerReconfiguration_r8_IEs, new_H_RNTI))
      {
        *hrnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_RadioBearerReconfiguration_r8_IEs, newPrimary_E_RNTI))
      {
        *ernti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8,
          tdsrrc_RadioBearerReconfiguration_r8_IEs, dl_InformationPerRL_List))
      {
        /* Get the CELL PARAMETER ID */
        tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r8->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
      }
    }
#endif
#ifdef FEATURE_TDSCDMA_REL9
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration, TDSMSG_REL9))
  {
    dcch_ptr_r9 = &dcch_ptr->message.u.radioBearerReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.
                      u.r9.radioBearerReconfiguration_r9;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_RadioBearerReconfiguration_r9_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_RadioBearerReconfiguration_r9_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_RadioBearerReconfiguration_r9_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9,
        tdsrrc_RadioBearerReconfiguration_r9_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r9->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#endif /* FEATURE_TDSCDMA_REL9 */

  else
  {
    ERR_FATAL("Invalid DCCH Message %d rcv'd", dcch_ptr->message.t,0,0);
  }

  return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_rbr_get_msg_ie_flg

DESCRIPTION:
  This function processes the Radio Bearer Release msg based on the REL version and
  get CELL PARAMETER ID, NEW C_RNTI, NEW H_RNTI present by TRadio Bearer Release msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_rbr_get_msg_ie_flg
(
  void *msg_ptr,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg,  /* either new CELL PARAMETER ID is present and different with current */
  boolean *crnti_flg,   /* NEW C_RNTI present flag */
  boolean *hrnti_flg,   /* NEW H_RNTI present flag */
  boolean *ernti_flg    /* NEW E_RNTI present flag */
)
{
  /* local variables */
  tdsrrc_DL_DCCH_Message *dcch_ptr;

  tdsrrc_RadioBearerRelease_r4_IEs   *dcch_ptr_r4;
  tdsrrc_RadioBearerRelease_r5_IEs   *dcch_ptr_r5;
  tdsrrc_RadioBearerRelease_r6_IEs   *dcch_ptr_r6;
  tdsrrc_RadioBearerRelease_r7_IEs   *dcch_ptr_r7;
#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_RadioBearerRelease_r8_IEs   *dcch_ptr_r8;
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  tdsrrc_RadioBearerRelease_r9_IEs   *dcch_ptr_r9;
#endif /* FEATURE_TDSCDMA_REL9 */

  uecomdef_status_e_type  status = SUCCESS;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if ((NULL == cpid_flg)||(NULL == cpid_change_flg)||(NULL == crnti_flg)||(NULL == hrnti_flg)||(NULL == ernti_flg))
  {
    ERR_FATAL("Point is NULL", 0,0,0);
  }
  *cpid_flg = FALSE;
  *cpid_change_flg = FALSE;
  *crnti_flg = FALSE;
  *hrnti_flg = FALSE;
  *ernti_flg = FALSE;

  if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL4))
  {
    dcch_ptr_r4 = &dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.
                  criticalExtensions.u.r4.radioBearerRelease_r4;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4, tdsrrc_RadioBearerRelease_r4_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4,
        tdsrrc_RadioBearerRelease_r4_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r4(&dcch_ptr_r4->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL5))
  {
    dcch_ptr_r5 = &dcch_ptr->message.u.radioBearerRelease.u.
            later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerRelease_r5;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_RadioBearerRelease_r5_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_RadioBearerRelease_r5_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5,
        tdsrrc_RadioBearerRelease_r5_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r5(&dcch_ptr_r5->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL6))
  {
    dcch_ptr_r6 = &dcch_ptr->message.u.radioBearerRelease.
              u.later_than_r3.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.r6.radioBearerRelease_r6;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_RadioBearerRelease_r6_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_RadioBearerRelease_r6_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_RadioBearerRelease_r6_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6,
        tdsrrc_RadioBearerRelease_r6_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r6(&dcch_ptr_r6->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL7))
  {
    dcch_ptr_r7 = &dcch_ptr->message.u.radioBearerRelease.
              u.later_than_r3.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.r7.radioBearerRelease_r7;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_RadioBearerRelease_r7_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_RadioBearerRelease_r7_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_RadioBearerRelease_r7_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7,
        tdsrrc_RadioBearerRelease_r7_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r7(&dcch_ptr_r7->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
    else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL8))
    {
      dcch_ptr_r8 = &dcch_ptr->message.u.radioBearerRelease.
                u.later_than_r3.criticalExtensions.
                u.criticalExtensions.
                u.criticalExtensions.
                u.criticalExtensions.
                u.criticalExtensions.
                u.r8.radioBearerRelease_r8;
      /* Get the C-RNTI */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_RadioBearerRelease_r8_IEs, new_C_RNTI))
      {
        *crnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_RadioBearerRelease_r8_IEs, new_H_RNTI))
      {
        *hrnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_RadioBearerRelease_r8_IEs, newPrimary_E_RNTI))
      {
        *ernti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8,
          tdsrrc_RadioBearerRelease_r8_IEs, dl_InformationPerRL_List))
      {
        /* Get the CELL PARAMETER ID */
        tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r8->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
      }
    }
#endif
#ifdef FEATURE_TDSCDMA_REL9
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL9))
  {
    dcch_ptr_r9 = &dcch_ptr->message.u.radioBearerRelease.
              u.later_than_r3.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.r9.radioBearerRelease_r9;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_RadioBearerRelease_r9_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_RadioBearerRelease_r9_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_RadioBearerRelease_r9_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9,
        tdsrrc_RadioBearerRelease_r9_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r9->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#endif  /* FEATURE_TDSCDMA_REL9 */

  else
  {
    ERR_FATAL("Invalid DCCH Message %d rcv'd", dcch_ptr->message.t,0,0);
  }

  return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_rbe_get_msg_ie_flg

DESCRIPTION:
  This function processes the Radio Bearer Setup msg based on the REL version and
  get CELL PARAMETER ID, NEW C_RNTI, NEW H_RNTI present by TRadio Bearer Setup msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_process_rbe_get_msg_ie_flg
(
  void *msg_ptr,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg,  /* either new CELL PARAMETER ID is present and different with current */
  boolean *crnti_flg,   /* NEW C_RNTI present flag */
  boolean *hrnti_flg,   /* NEW H_RNTI present flag */
  boolean *ernti_flg    /* NEW E_RNTI present flag */
)
{
  /* local variables */
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  tdsrrc_RadioBearerSetup_r4_IEs   *dcch_ptr_r4;
  tdsrrc_RadioBearerSetup_r5_IEs   *dcch_ptr_r5;
  tdsrrc_RadioBearerSetup_r6_IEs   *dcch_ptr_r6;
  tdsrrc_RadioBearerSetup_r7_IEs   *dcch_ptr_r7;
#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_RadioBearerSetup_r8_IEs   *dcch_ptr_r8;
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  tdsrrc_RadioBearerSetup_r9_IEs   *dcch_ptr_r9;
#endif /* FEATURE_TDSCDMA_REL9 */

  uecomdef_status_e_type  status = SUCCESS;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if ((NULL == cpid_flg)||(NULL == cpid_change_flg)||(NULL == crnti_flg)||(NULL == hrnti_flg)||(NULL == ernti_flg))
  {
    ERR_FATAL("Point is NULL", 0,0,0);
  }
  *cpid_flg = FALSE;
  *cpid_change_flg = FALSE;
  *crnti_flg = FALSE;
  *hrnti_flg = FALSE;
  *ernti_flg = FALSE;

  if(SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL4))
  {
    dcch_ptr_r4 = &dcch_ptr->message.u.radioBearerSetup.u.
        later_than_r3.criticalExtensions.u.r4.radioBearerSetup_r4;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4, tdsrrc_RadioBearerSetup_r4_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4,
        tdsrrc_RadioBearerSetup_r4_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r4(&dcch_ptr_r4->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if( SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL5))
  {
    dcch_ptr_r5 = &dcch_ptr->message.u.radioBearerSetup.u.
            later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerSetup_r5;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_RadioBearerSetup_r5_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_RadioBearerSetup_r5_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5,
        tdsrrc_RadioBearerSetup_r5_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r5(&dcch_ptr_r5->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL6))
  {
    dcch_ptr_r6 = &dcch_ptr->message.u.radioBearerSetup.
              u.later_than_r3.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.u.r6.radioBearerSetup_r6;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_RadioBearerSetup_r6_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_RadioBearerSetup_r6_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_RadioBearerSetup_r6_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6,
        tdsrrc_RadioBearerSetup_r6_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r6(&dcch_ptr_r6->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL7))
  {
    dcch_ptr_r7 = &dcch_ptr->message.u.radioBearerSetup.
             u.later_than_r3.criticalExtensions.u.
             criticalExtensions.u.criticalExtensions.
             u.criticalExtensions.u.r7.radioBearerSetup_r7;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_RadioBearerSetup_r7_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_RadioBearerSetup_r7_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_RadioBearerSetup_r7_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7,
        tdsrrc_RadioBearerSetup_r7_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r7(&dcch_ptr_r7->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
    else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL8))
    {
      dcch_ptr_r8 = &dcch_ptr->message.u.radioBearerSetup.
               u.later_than_r3.criticalExtensions.u.
               criticalExtensions.u.criticalExtensions.
               u.criticalExtensions.u.criticalExtensions.
               u.r8.radioBearerSetup_r8;
      /* Get the C-RNTI */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_RadioBearerSetup_r8_IEs, new_C_RNTI))
      {
        *crnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_RadioBearerSetup_r8_IEs, new_H_RNTI))
      {
        *hrnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_RadioBearerSetup_r8_IEs, newPrimary_E_RNTI))
      {
        *ernti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8,
          tdsrrc_RadioBearerSetup_r8_IEs, dl_InformationPerRL_List))
      {
        /* Get the CELL PARAMETER ID */
        tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r8->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
      }
    }
#endif
#ifdef FEATURE_TDSCDMA_REL9
    else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL9))
    {
      dcch_ptr_r9 = &dcch_ptr->message.u.radioBearerSetup.
               u.later_than_r3.criticalExtensions.u.
               criticalExtensions.u.criticalExtensions.
               u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.radioBearerSetup_r9;
      /* Get the C-RNTI */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_RadioBearerSetup_r9_IEs, new_C_RNTI))
      {
        *crnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_RadioBearerSetup_r9_IEs, new_H_RNTI))
      {
        *hrnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_RadioBearerSetup_r9_IEs, newPrimary_E_RNTI))
      {
        *ernti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9,
          tdsrrc_RadioBearerSetup_r9_IEs, dl_InformationPerRL_List))
      {
        /* Get the CELL PARAMETER ID */
        tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r9->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
      }
    }
#endif /* FEATURE_TDSCDMA_REL9 */

  else
  {
    ERR_FATAL("Invalid DCCH Message %d rcv'd", dcch_ptr->message.t,0,0);
  }

  return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_tcrc_get_msg_ie_flg

DESCRIPTION:
  This function processes the Transport Channel Reconfig msg based on the REL version and
  get CELL PARAMETER ID, NEW C_RNTI, NEW H_RNTI present by Transport Channel Reconfig msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_process_tcrc_get_msg_ie_flg
(
  void *msg_ptr,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg,  /* either new CELL PARAMETER ID is present and different with current */
  boolean *crnti_flg,   /* NEW C_RNTI present flag */
  boolean *hrnti_flg,   /* NEW H_RNTI present flag */
  boolean *ernti_flg    /* NEW E_RNTI present flag */
)
{
  /* local variables */
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  tdsrrc_TransportChannelReconfiguration_r4_IEs   *dcch_ptr_r4;
  tdsrrc_TransportChannelReconfiguration_r5_IEs   *dcch_ptr_r5;
  tdsrrc_TransportChannelReconfiguration_r6_IEs   *dcch_ptr_r6;
  tdsrrc_TransportChannelReconfiguration_r7_IEs   *dcch_ptr_r7;
#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_TransportChannelReconfiguration_r8_IEs   *dcch_ptr_r8;
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  tdsrrc_TransportChannelReconfiguration_r9_IEs   *dcch_ptr_r9;
#endif /* FEATURE_TDSCDMA_REL9 */

  uecomdef_status_e_type  status = SUCCESS;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if ((NULL == cpid_flg)||(NULL == cpid_change_flg)||(NULL == crnti_flg)||(NULL == hrnti_flg)||(NULL == ernti_flg))
  {
    ERR_FATAL("Point is NULL", 0,0,0);
  }
  *cpid_flg = FALSE;
  *cpid_change_flg = FALSE;
  *crnti_flg = FALSE;
  *hrnti_flg = FALSE;
  *ernti_flg = FALSE;

  if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL4))
  {
    dcch_ptr_r4 = &dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
        criticalExtensions.u.r4.transportChannelReconfiguration_r4;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4, tdsrrc_TransportChannelReconfiguration_r4_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4,
        tdsrrc_TransportChannelReconfiguration_r4_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r4(&dcch_ptr_r4->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL5))
  {
    dcch_ptr_r5 = &dcch_ptr->message.u.transportChannelReconfiguration.u.
                       later_than_r3.criticalExtensions.u.criticalExtensions.u
                        .r5.transportChannelReconfiguration_r5;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_TransportChannelReconfiguration_r5_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_TransportChannelReconfiguration_r5_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5,
        tdsrrc_TransportChannelReconfiguration_r5_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r5(&dcch_ptr_r5->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL6))
  {
    dcch_ptr_r6 = &dcch_ptr->message.u.transportChannelReconfiguration.
                          u.later_than_r3.criticalExtensions.
                          u.criticalExtensions.
                          u.criticalExtensions.
                          u.r6.transportChannelReconfiguration_r6;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_TransportChannelReconfiguration_r6_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_TransportChannelReconfiguration_r6_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_TransportChannelReconfiguration_r6_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6,
        tdsrrc_TransportChannelReconfiguration_r6_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r6(&dcch_ptr_r6->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL7))
  {
    dcch_ptr_r7 = &dcch_ptr->message.u.transportChannelReconfiguration.
            u.later_than_r3.criticalExtensions.u.
             criticalExtensions.u.criticalExtensions.
             u.criticalExtensions.u.r7.transportChannelReconfiguration_r7;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_TransportChannelReconfiguration_r7_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_TransportChannelReconfiguration_r7_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_TransportChannelReconfiguration_r7_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7,
        tdsrrc_TransportChannelReconfiguration_r7_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r7(&dcch_ptr_r7->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
    else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL8))
    {
      dcch_ptr_r8 = &dcch_ptr->message.u.transportChannelReconfiguration.
              u.later_than_r3.criticalExtensions.u.
               criticalExtensions.u.criticalExtensions.
               u.criticalExtensions.u.criticalExtensions.
               u.r8.transportChannelReconfiguration_r8;
      /* Get the C-RNTI */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_TransportChannelReconfiguration_r8_IEs, new_C_RNTI))
      {
        *crnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_TransportChannelReconfiguration_r8_IEs, new_H_RNTI))
      {
        *hrnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_TransportChannelReconfiguration_r8_IEs, newPrimary_E_RNTI))
      {
        *ernti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8,
          tdsrrc_TransportChannelReconfiguration_r8_IEs, dl_InformationPerRL_List))
      {
        /* Get the CELL PARAMETER ID */
        tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r8->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
      }
    }
#endif/*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL9))
  {
    dcch_ptr_r9 = &dcch_ptr->message.u.transportChannelReconfiguration.
            u.later_than_r3.criticalExtensions.u.
             criticalExtensions.u.criticalExtensions.
             u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9.transportChannelReconfiguration_r9;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_TransportChannelReconfiguration_r9_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_TransportChannelReconfiguration_r9_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_TransportChannelReconfiguration_r9_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9,
        tdsrrc_TransportChannelReconfiguration_r9_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r9->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#endif/* FEATURE_TDSCDMA_REL9 */

  else
  {
    ERR_FATAL("Invalid DCCH Message %d rcv'd", dcch_ptr->message.t,0,0);
  }

  return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_dcch_cu_get_msg_ie_flg

DESCRIPTION:
  This function processes the Cell Update Confirm received on DCCH based on the REL version and
  get CELL PARAMETER ID, NEW C_RNTI, NEW H_RNTI present by CU

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_process_dcch_cu_get_msg_ie_flg
(
  void *msg_ptr,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg,  /* either new CELL PARAMETER ID is present and different with current */
  boolean *crnti_flg,   /* NEW C_RNTI present flag */
  boolean *hrnti_flg,   /* NEW H_RNTI present flag */
  boolean *ernti_flg    /* NEW E_RNTI present flag */
)
{
  /* local variables */
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  tdsrrc_CellUpdateConfirm_r4_IEs   *dcch_ptr_r4;
  tdsrrc_CellUpdateConfirm_r5_IEs   *dcch_ptr_r5;
  tdsrrc_CellUpdateConfirm_r6_IEs   *dcch_ptr_r6;
  tdsrrc_CellUpdateConfirm_r7_IEs   *dcch_ptr_r7;
#ifdef FEATURE_TDSCDMA_REL8
  tdsrrc_CellUpdateConfirm_r8_IEs   *dcch_ptr_r8;
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
  tdsrrc_CellUpdateConfirm_r9_IEs   *dcch_ptr_r9;
#endif /* FEATURE_TDSCDMA_REL9 */

  uecomdef_status_e_type  status = SUCCESS;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if ((NULL == cpid_flg)||(NULL == cpid_change_flg)||(NULL == crnti_flg)||(NULL == hrnti_flg)||(NULL == ernti_flg))
  {
    ERR_FATAL("Point is NULL", 0,0,0);
  }
  *cpid_flg = FALSE;
  *cpid_change_flg = FALSE;
  *crnti_flg = FALSE;
  *hrnti_flg = FALSE;
  *ernti_flg = FALSE;

  if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm, TDSMSG_REL4))
  {
    dcch_ptr_r4 = &dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                          criticalExtensions.u.r4.cellUpdateConfirm_r4;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4, tdsrrc_CellUpdateConfirm_r4_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r4,
        tdsrrc_CellUpdateConfirm_r4_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r4(&dcch_ptr_r4->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm, TDSMSG_REL5))
  {
    dcch_ptr_r5 = &dcch_ptr->message.u.cellUpdateConfirm.u.
                         later_than_r3.criticalExtensions.u.criticalExtensions.u
                         .r5.cellUpdateConfirm_r5;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_CellUpdateConfirm_r5_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5, tdsrrc_CellUpdateConfirm_r5_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r5,
        tdsrrc_CellUpdateConfirm_r5_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r5(&dcch_ptr_r5->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm, TDSMSG_REL6))
  {
    dcch_ptr_r6 = &dcch_ptr->message.u.cellUpdateConfirm.u.
                      later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                      r6.cellUpdateConfirm_r6;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_CellUpdateConfirm_r6_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_CellUpdateConfirm_r6_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6, tdsrrc_CellUpdateConfirm_r6_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r6,
        tdsrrc_CellUpdateConfirm_r6_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r6(&dcch_ptr_r6->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm, TDSMSG_REL7))
  {
    dcch_ptr_r7 = &dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
                      u.r7.cellUpdateConfirm_r7;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_CellUpdateConfirm_r7_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_CellUpdateConfirm_r7_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7, tdsrrc_CellUpdateConfirm_r7_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r7,
        tdsrrc_CellUpdateConfirm_r7_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r7(&dcch_ptr_r7->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
    else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
           T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm, TDSMSG_REL8))
    {
      dcch_ptr_r8 = &dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                        criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
                        u.criticalExtensions.u.r8.cellUpdateConfirm_r8;
      /* Get the C-RNTI */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_CellUpdateConfirm_r8_IEs, new_C_RNTI))
      {
        *crnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_CellUpdateConfirm_r8_IEs, new_H_RNTI))
      {
        *hrnti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8, tdsrrc_CellUpdateConfirm_r8_IEs, newPrimary_E_RNTI))
      {
        *ernti_flg = TRUE;
      }
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r8,
          tdsrrc_CellUpdateConfirm_r8_IEs, dl_InformationPerRL_List))
      {
        /* Get the CELL PARAMETER ID */
        tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r8->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
      }
    }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  else if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm, TDSMSG_REL9))
  {
    dcch_ptr_r9 = &dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                      criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
                      u.criticalExtensions.u.criticalExtensions.u.r9.cellUpdateConfirm_r9;
    /* Get the C-RNTI */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_CellUpdateConfirm_r9_IEs, new_C_RNTI))
    {
      *crnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_CellUpdateConfirm_r9_IEs, new_H_RNTI))
    {
      *hrnti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9, tdsrrc_CellUpdateConfirm_r9_IEs, newPrimary_E_RNTI))
    {
      *ernti_flg = TRUE;
    }
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(dcch_ptr_r9,
        tdsrrc_CellUpdateConfirm_r9_IEs, dl_InformationPerRL_List))
    {
      /* Get the CELL PARAMETER ID */
      tdsrrcllc_process_get_cpid_r8(&dcch_ptr_r9->dl_InformationPerRL_List, cpid_flg, cpid_change_flg);
    }
  }
#endif /*FEATURE_TDSCDMA_REL9*/

  else
  {
    ERR_FATAL("Invalid DCCH Message %d rcv'd", dcch_ptr->message.t,0,0);
  }

  return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_get_msg_rnti_cpid_flg

DESCRIPTION:
  This function figures out which RRC message was received and
  get either crnti, hrnti, cpid IE.present, and either cpid same as current

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if msg is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_msg_rnti_cpid_flg
(
  uint32 dl_sdu_num,
  void *msg_ptr,
  boolean *cpid_flg,         /* CELL PARAMETER ID present flag */
  boolean *cpid_change_flg,  /* either new CELL PARAMETER ID is present and different with current */
  boolean *crnti_flg,   /* NEW C_RNTI present flag */
  boolean *hrnti_flg,   /* NEW H_RNTI present flag */
  boolean *ernti_flg    /* NEW E_RNTI present flag */
)
{
  /* local variables */
  tdsrrc_DL_CCCH_Message *ccch_ptr;
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  uecomdef_status_e_type  status = SUCCESS;

  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;
  ccch_ptr = (tdsrrc_DL_CCCH_Message *)msg_ptr;

  if(msg_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL message ptr passed to OC");
    return(FAILURE);
  }

  /* Determine if it was rcv'd on the DL CCCH */
  if(dl_sdu_num == tdsrrc_DL_CCCH_Message_PDU)
  {
    /* Since it was a DL CCCH Msg, figure out the Message Type */
    switch(ccch_ptr->message.t)
    {
      case T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CellUpdateCnf updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_ccch_cu_get_msg_ie_flg(msg_ptr, 
             cpid_flg, cpid_change_flg, crnti_flg, hrnti_flg, ernti_flg)== FAILURE)
        {
          status = FAILURE;
        }
        break;

      case T_tdsrrc_DL_CCCH_MessageType_uraUpdateConfirm:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CCCH Message %d not supported yet for TDSORDERED_CONFIG init", ccch_ptr->message.t);
        status = FAILURE;
        break;

      case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject:
      case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease:
      case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup:
      /* March2002Rel changes : replaced with spare values
       * case rrc_DL_CCCH_MessageType_extension_chosen:
       */
      case T_tdsrrc_DL_CCCH_MessageType_spare2:
      case T_tdsrrc_DL_CCCH_MessageType_spare1:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CCCH Message %d updating OC", ccch_ptr->message.t);
        status = FAILURE;
        break;

      default:
        ERR_FATAL("Invalid CCCH Message %d rcv'd", ccch_ptr->message.t,0,0);
        break;
    } /* end CCCH msg switch */
  } /* end DL CCCH msg */

  /* It was rcv'd on the DL DCCH */
  else if(dl_sdu_num == tdsrrc_DL_DCCH_Message_PDU)
  {
    /* Since it was a DL DCCH Msg, figure out the Message Type */
    switch(dcch_ptr->message.t)
    {
      case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PhyChan Reconfig updating TDSORDERED_CONFIG");

        if(tdsrrcllc_process_pcrc_get_msg_ie_flg(msg_ptr,
               cpid_flg, cpid_change_flg, crnti_flg, hrnti_flg, ernti_flg)== FAILURE)
        {
          status = FAILURE;
        }
        break;

      case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Reconfig updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_rbrc_get_msg_ie_flg(msg_ptr,
                  cpid_flg, cpid_change_flg, crnti_flg, hrnti_flg, ernti_flg)== FAILURE)
        {
          status = FAILURE;
        }
        break;

      case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Release updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_rbr_get_msg_ie_flg(msg_ptr,
                    cpid_flg, cpid_change_flg, crnti_flg, hrnti_flg, ernti_flg)== FAILURE)
        {
          status = FAILURE;
        }
        break;

      case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Setup updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_rbe_get_msg_ie_flg(msg_ptr,
                  cpid_flg, cpid_change_flg, crnti_flg, hrnti_flg, ernti_flg)== FAILURE)
        {
          status = FAILURE;
        }
       break;

      case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TrCH reconfig updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_tcrc_get_msg_ie_flg(msg_ptr,
                cpid_flg, cpid_change_flg, crnti_flg, hrnti_flg, ernti_flg)== FAILURE)
        {
          status = FAILURE;
        }
        break;

      case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CellUpdateCnf updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_dcch_cu_get_msg_ie_flg(msg_ptr,
                  cpid_flg, cpid_change_flg, crnti_flg, hrnti_flg, ernti_flg)== FAILURE)
        {
          status = FAILURE;
        }
        break;

      case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM:
      case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_CDMA2000:
      case T_tdsrrc_DL_DCCH_MessageType_securityModeCommand:
      case T_tdsrrc_DL_DCCH_MessageType_transportFormatCombinationControl:
      case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DCCH Message %d not supported yet for OC", dcch_ptr->message.t);
        status = FAILURE;
        break;

      case T_tdsrrc_DL_DCCH_MessageType_rrcConnectionRelease:
      case T_tdsrrc_DL_DCCH_MessageType_downlinkDirectTransfer:
      case T_tdsrrc_DL_DCCH_MessageType_counterCheck:
      case T_tdsrrc_DL_DCCH_MessageType_measurementControl:
      case T_tdsrrc_DL_DCCH_MessageType_pagingType2:
      case T_tdsrrc_DL_DCCH_MessageType_signallingConnectionRelease:
      case T_tdsrrc_DL_DCCH_MessageType_ueCapabilityEnquiry:
      case T_tdsrrc_DL_DCCH_MessageType_ueCapabilityInformationConfirm:
      case T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation:
      /* March2002Rel changes : replaced with spare values
       * case rrc_DL_DCCH_MessageType_extension_chosen:
       */
      case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GERANIu:
      case T_tdsrrc_DL_DCCH_MessageType_mbmsModifiedServicesInformation:
      case T_tdsrrc_DL_DCCH_MessageType_spare3:
      case T_tdsrrc_DL_DCCH_MessageType_spare2:
      case T_tdsrrc_DL_DCCH_MessageType_spare1:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid DCCH Message %d updating OC", dcch_ptr->message.t);
        status = FAILURE;
        break;

      case T_tdsrrc_DL_DCCH_MessageType_physicalSharedChannelAllocation:
      case T_tdsrrc_DL_DCCH_MessageType_uplinkPhysicalChannelControl:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DL TDD Message %d will never be supported", dcch_ptr->message.t);
        status = FAILURE;
        break;

      default:
        ERR_FATAL("Invalid DCCH Message %d rcv'd", dcch_ptr->message.t,0,0);
        break;
    } /* end DCCH msg switch */
  } /* end DL DCCH msg */

  /* No other downlink SDU type is valid for this case */
  else
  {
    ERR_FATAL("Invalid SDU %d received", dl_sdu_num,0,0);
  }
  return status;
} /* end tdsrrcllc_get_msg_rnti_cpid_flg() */

#ifdef FEATURE_TDSCDMA_MIMO
/*============================================================================
FUNCTION: tdsrrcllc_get_mimo_action_in_config()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

tdsl1_dl_hs_mimo_action_enum_type 
  tdsrrcllc_get_mimo_action_in_config
(
  void
)
{
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
   return (tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action);
  }
  else
  {
    return( tdscurrent_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action);
  }
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*============================================================================
FUNCTION: tdsrrcllc_get_mimo_status()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

tdsrrc_mimo_status_enum_type tdsrrcllc_get_mimo_status
(
   void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO:MIMO Status is  %d",tdsrrc_mimo_status);
  return tdsrrc_mimo_status;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

#endif

/*===================================================================
*                       FUNCTION PROTOTYPES
====================================================================*/

/*===================================================================
                        EXTERNAL FUNCTIONS
=====================================================================*/
/* The declarations for these functions are in tdsrrcllc.h */

/*====================================================================
FUNCTION: tdsrrcllc_allocate_dflow_index()

DESCRIPTION:
  Allocates a Dflow Id.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL DCH TrCH Index that was already allocated to
           given transport channel ID.
  TDSRRCLLC_INVALID_TRCH_IDX -> Given Transport channel ID does not
           have an allocated DCH transport channel index.

SIDE EFFECTS:
====================================================================*/
uint8  tdsrrcllc_allocate_dflow_index()
{

  uint8 cnt = UE_MAX_MAC_D_FLOW;
  /* An index with the same DFLOW ID is NOT in use.
  * Allocate a new index */
  for (cnt =0; cnt <UE_MAX_MAC_D_FLOW; cnt++)
  {
    if ((tdsordered_config_ptr->dl_dflow_index_in_use[cnt] == FALSE) &&
      (tdsordered_config_ptr->mac_dflow_info[cnt].mac_dflow_id == TDSINVALID_DFLOW_ID))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:Assign Index %d for new DFLOW", cnt);
      tdsordered_config_ptr->dl_dflow_index_in_use[cnt] = TRUE;
      break;
    }
  }
  return cnt;

} /* tdsrrcllc_allocate_dflow_index */

/*====================================================================
FUNCTION: tdsrrcllc_allocate_queue_index()

DESCRIPTION:
  Allocates a Queue Index for the queue.

DEPENDENCIES:
  None.

RETURN VALUE:
  Queue Index if found/allocated else UE_MAX_HS_QUEUE

SIDE EFFECTS:
====================================================================*/
static uint8  tdsrrcllc_allocate_queue_index(uint8 dflow_index, uint32 queue_id)
{
  uint8 cnt = UE_MAX_HS_QUEUE;
  /* An index with the same DFLOW ID is NOT in use. Allocate a new index */

  if (tdsordered_config_ptr->mac_dflow_info[dflow_index].no_of_queues == UE_MAX_HS_QUEUE)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCHS:Num Queues %d with dflow %d",tdsordered_config_ptr->mac_dflow_info[dflow_index].no_of_queues
        ,tdsordered_config_ptr->mac_dflow_info[dflow_index].mac_dflow_id);

    return cnt;
  }
  for (cnt =0; cnt <UE_MAX_HS_QUEUE; cnt++)
  {
    if (tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[cnt].queue_id == TDSINVALID_QUEUE_ID)
    {
      TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:Assign Index %d for queue %d", cnt,queue_id);
      tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[cnt].queue_id = (uint8) queue_id;
      tdsordered_config_ptr->mac_dflow_info[dflow_index].no_of_queues++;
      return cnt;
    }
  }
  return cnt;

} /* tdsrrcllc_allocate_queue_index */


/*====================================================================
FUNCTION: rrcllc_find_dflow_idx()

DESCRIPTION:
  This function finds the dflow index for the DFLOW. If not found, it allocates
  a new index and return to the index number.

DEPENDENCIES:
  None.

RETURN VALUE:
  DFLOW Index if found
  Else UE_MAX_MAC_D_FLOW.

SIDE EFFECTS:
====================================================================*/
uint8  tdsrrcllc_find_dflow_index
(
  uint8  dflow_id
)
{
  uint8 index = UE_MAX_MAC_D_FLOW;

  for(index = 0; index < UE_MAX_MAC_D_FLOW; index++)
  {
    if((tdsordered_config_ptr->dl_dflow_index_in_use[index] == TRUE) &&
      (tdsordered_config_ptr->mac_dflow_info[index].mac_dflow_id == dflow_id))
    {
      TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:DFLOWidx %d, Id %d",index, dflow_id);

      break;
    }
  }
  return(index);
}

/*====================================================================
FUNCTION: tdsrrcllc_initialize_queue_info()

DESCRIPTION:
  This function deallocates all the queues associated with a DFLOW

DEPENDENCIES:
  None.

RETURN VALUE:
  None.
  
SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_initialize_queue_info
(
  uint8 dflow_index
)
{
  uint8 cnt = 0;

  for (cnt = 0; cnt < tdsordered_config_ptr->mac_dflow_info[dflow_index].no_of_queues; cnt++)
  {
    tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[cnt].queue_id = TDSINVALID_QUEUE_ID;
    tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[cnt].no_of_pdu_sizes = 0;
    tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[cnt].release_timer = 0;
    tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[cnt].window_size = 0;
  }
  tdsordered_config_ptr->mac_dflow_info[dflow_index].no_of_queues = 0;
} /* end tdsrrcllc_initialize_queue_info() */




/*====================================================================
FUNCTION: tdsrrcllc_get_dflow_index()

DESCRIPTION:
  This function gets a DFLOW Index for the specified DFLOW ID. If the
  DFLOW already existed, its index is returned. Otherwise a new index is
  allocated for the DFLOW ID. If a new DFLOW index is allocated,
  DFLOW count in MAC structure is updated.

DEPENDENCIES:
  None.

RETURN VALUE:
  Index value if successful else
  UE_MAX_MAC_D_FLOW if no index is available

SIDE EFFECTS:

====================================================================*/

uint8 tdsrrcllc_get_dflow_index
(
  uint8  dflow_id
)
{
  /* To indicate if DFLOW Index has been founf */
  boolean       dflow_index_found = FALSE;

  uint8 index = UE_MAX_MAC_D_FLOW;

  for(index = 0; index < UE_MAX_MAC_D_FLOW; index++)
  {
    if((tdsordered_config_ptr->dl_dflow_index_in_use[index] == TRUE) &&
      (tdsordered_config_ptr->mac_dflow_info[index].mac_dflow_id == dflow_id))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:DFLOWidx %d, Id %d",index, dflow_id);

      dflow_index_found = TRUE;

      break;
    }
  }

  if(dflow_index_found)
  {
    return(index);
  }
  else  /* DFLOW index not found, Allocate new */
  {
    index = tdsrrcllc_allocate_dflow_index();
    if(index == UE_MAX_MAC_D_FLOW)
    {
      /* Failure to allocate a new DFLOW Index */
      return(UE_MAX_MAC_D_FLOW);
    }
    else  /* A valid new DFLOW index has been allocated */
    {

      /* Now Save DFLOW ID in MAC structure */
      tdsordered_config_ptr->mac_dflow_info[index].mac_dflow_id = dflow_id;
      tdsrrcllc_semi_permanent_data.mac_dflow_info[index].mac_dflow_id = dflow_id;

      /* Increment no of DFLOWs */
      tdsordered_config_ptr->mac_dl_parms.num_dflow++;

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:New dflow cnt %d in MAC struct", tdsordered_config_ptr->mac_dl_parms.num_dflow);

      return(index);

    } /* A valid new DFLOW index allocated */
  } /* DFLOW index not found, Allocate new */
} /* tdsrrcllc_get_dflow_index */


/*====================================================================
FUNCTION: tdsrrcllc_remove_dflow_queue_mapping()

DESCRIPTION:

  This function removes the association of former dflow associated
  with the queue. The dflow associated with this queue in the passed
  ptr is the new DFLOW

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:

====================================================================*/
void tdsrrcllc_remove_dflow_queue_mapping
(
  tdsrrc_MAC_hs_AddReconfQueue* queue_ptr
)
{
  /* transport Channel Index */
  uint8  dflow_index = 0;

  uint32  queue_index = 0;

  /* Check if queue is is already present */
  for(dflow_index = 0; dflow_index < tdsordered_config_ptr->mac_dl_parms.num_dflow; dflow_index++)
  {
    if ((tdsordered_config_ptr->mac_dflow_info[dflow_index].mac_dflow_id == TDSINVALID_DFLOW_ID) ||
        (tdsordered_config_ptr->dl_dflow_index_in_use[dflow_index]  == FALSE))

      continue;

    for(queue_index = 0; queue_index < tdsordered_config_ptr->mac_dflow_info[dflow_index].no_of_queues;
         queue_index++)
    {
      if (tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].queue_id == TDSINVALID_QUEUE_ID)
        continue;

      /* Find if queue Id is same but dflow id is different */
      if ((tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].queue_id ==
          queue_ptr->mac_hsQueueId) &&
          (tdsordered_config_ptr->mac_dflow_info[dflow_index].mac_dflow_id != queue_ptr->mac_dFlowId))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCHS:Remove Queue %d from old DFLOW %d, new DFLOW %d",
                 queue_ptr->mac_hsQueueId, tdsordered_config_ptr->mac_dflow_info[dflow_index].mac_dflow_id,
                 queue_ptr->mac_dFlowId);
        /* Take the backup of the RB-Mapping , as the DFlow is been deleted */
        tdsrrcllc_backup_rbmapping_info(dflow_index,TDSMAC_D_RB_MAPPING );
        tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].queue_id = TDSINVALID_QUEUE_ID;
        tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes = 0;
        tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].release_timer = 0;
        tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].window_size = 0;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Remove Queue index is %d in old DFLOW",
                 queue_index);

        /* If delete old mapping between mac_dflow and mac_queue, mac_queue list generate holes. So RRC need to 
                  rearrange queue index */
        tdsrrcllc_rearrange_queue_index();
      }
    }
  }
}


/*====================================================================
FUNCTION: tdsrrcllc_get_queue_index()

DESCRIPTION:

  This function gets a Queue Index for the specified DFLOW ID. If the
  Queue already existed, its index is returned. Otherwise a new queue index is
  allocated for the DFLOW ID. The queue count for that index is also
  incremented. The fn also fills the passed queue already present indicator
  if the queue is already present.

DEPENDENCIES:
  None.

RETURN VALUE:
  Queue Index if successfully allocated else UE_MAX_HS_QUEUE

SIDE EFFECTS:

====================================================================*/
uint8 tdsrrcllc_get_queue_index
(
  uint8  dflow_index,
  uint32 queue_id,
  boolean* queue_present_ptr
)
{
  /* transport Channel Index */
  uint8         index = 0;

  /* Initialize by default indicating that queue is absent */
  *queue_present_ptr = FALSE;

  /* Check if queue is is already present */
  for(index = 0; index < tdsordered_config_ptr->mac_dflow_info[dflow_index].no_of_queues; index++)
  {
    if(tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[index].queue_id == queue_id)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:Queue id %d already present for Flow Id %d",index,
        tdsordered_config_ptr->mac_dflow_info[dflow_index].mac_dflow_id);

        /* This is the case when queue is already present and Network wants to
        reconfigure this queue again. Set a boolean indicating this condition */

      *queue_present_ptr = TRUE;

      break;
    }
  }

  if(*queue_present_ptr == TRUE)
  {
    /* Return queue index. */
    return(index);
  }
  else  /* Queue index not found, allocate new */
  {
    /* An index with the same Queue Id is NOT in use.Need to allocate a new index. */

    TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:Alloc new queue idx for FlowIdx %d,QueueId %d", dflow_index, queue_id);
    index = tdsrrcllc_allocate_queue_index(dflow_index, queue_id);

    return index;

  } /* Existing transport channel index couldn't be found, allocated new */
} /* tdsrrcllc_get_queue_index */


/*====================================================================
FUNCTION: tdsrrcllc_determine_hsdpa_action_r6()

DESCRIPTION:

  This function determines the HSDPA action and sets the parameters in L1
  structure accordingly.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  Sucess if HSDPA action and parameters have been set
  Failure Otherwise

SIDE EFFECTS:
  
====================================================================*/
uecomdef_status_e_type tdsrrcllc_determine_hsdpa_action_r6
(
  tdsrrc_DL_HSPDSCH_Information_r6* hspdsch_info_ptr
)
{
  uint8 reconfig_bit_mask = 0;
  boolean hsscch_codes_changed = FALSE;
  boolean hspdsch_midamble_changed = FALSE;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  uint8 i = 0;
#endif

  /* HSDPA params have already been set in rrc_hsdpa_params.
  Check if HSDPA is already ACTIVE */
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
   tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type = TDSMAC_HS_NO_TRANSITION;
#endif

  /* Save UE category info */
  if (tdsnv_hsdpa_category > 15)
  {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if (0 == tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue)
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
#endif
    }
    else
    {
      for(i=0;i< tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue;i++)
      {
        if ((tdsordered_config_ptr->dl_macehs_logchan_info[i].queue_id != TDSINVALID_QUEUE_ID) &&
          (tdsordered_config_ptr->mac_ehs_lc_id_in_use[i] == TRUE ))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 23;
          break;
        }
        else
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
          if (i == (tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue - 1))
          {
            tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
          }
#endif
        }
      }
    }
#else
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#endif
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = tdsnv_hsdpa_category;
  }

  if (tdsrrc_get_hsdpa_status() == TDSHSDPA_INACTIVE)
  {
    /* HSDPA is currently inactive */
    /* Check if network wants to activate HSDPA */
    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {
      if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
      {
        /* Check if channelization codes and feedback info present */
        if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r6,hs_scch_Info))  
        {
          if (tdsrrcllc_set_hsscch_info_r6(&hspdsch_info_ptr->hs_scch_Info) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid HS-SCCH info");
            return FAILURE;
          }
        }

                        
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r6,measurement_feedback_Info))
        {
          /* Set Meas Feedback Info */
          if (tdsrrcllc_set_meas_feedback_info(&hspdsch_info_ptr->measurement_feedback_Info) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid Meas Feedback info");
            return FAILURE;
          }
        }
        /* Check if HS-PDSCH Midamble Configuration present */
        if (TDSRRC_CHECK_MSG_TYPE(hspdsch_info_ptr->modeSpecificInfo,
            T_tdsrrc_DL_HSPDSCH_Information_r6_modeSpecificInfo_tdd))
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(hspdsch_info_ptr->modeSpecificInfo.u.tdd,
              rrc_DL_HSPDSCH_Information_r6_tdd_tdd128))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                hs_PDSCH_Midamble_Configuration_tdd128))
            {
              tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                  hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                  ,FALSE
#endif
                   );
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive midamble configuration tdd128");
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
          return FAILURE;
        }
      }
    } /* End of if of hsdpa message params absent */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_reset_mac_ehs_params(tdsordered_config_ptr);
#endif
      return SUCCESS;
    }

    /* Set Queue Info */
    if (tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set Queue Info");
      return FAILURE;
    }

    /* Final Check to make sure that everything is present */
    if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
      return SUCCESS;
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTART");
    tdsrrc_set_hsdpa_action(TDSHSDPA_START);

    /* Set Bit Mask */
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = TDSL1_CPHY_HS_DSCH_CFG_INCL | 
      TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;


    TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:HS Bit Mask is (Dec) %d (Hex) %x", tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask,
             tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);

    /* In case UTRAN messes up, set HS-RESET to FALSE */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Force HS-RESET to FALSE");
    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = FALSE;
    return SUCCESS;
    
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSDPA already active");
    /* HSDPA is already ACTIVE */
    
    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {

      /* Set bit mask for HO */
      if ((tdsordered_config_ptr->is_hho== TRUE)
        ||(tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:frequency change primary freq %d working freq %d Set HS-DSCH in bit_mask ",
               tdsordered_config_ptr->p_freq,tdsordered_config_ptr->w_freq);
       
        reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
      }

      if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
      {
        if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r6,hs_scch_Info))
        {
          /* Check if HS-SCCH Info has changed */
          if (tdsrrcllc_check_if_hsscch_changed_r6(&hspdsch_info_ptr->hs_scch_Info) == TRUE)
          {
            hsscch_codes_changed = TRUE;
          }

          if (tdsrrcllc_set_hsscch_info_r6(&hspdsch_info_ptr->hs_scch_Info) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid HS-SCCH info");
            return FAILURE;
          }
          else
          {
            if (hsscch_codes_changed == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:SCCH Info changed");
              reconfig_bit_mask =reconfig_bit_mask | TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL;
            }

          }
        }


        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(
            hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r6,
            measurement_feedback_Info))
        {
          if (tdsrrcllc_set_meas_feedback_info(
            &hspdsch_info_ptr->measurement_feedback_Info) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid Meas Feedback info");
            return FAILURE;
          }
        }
        /* Check if HS-PDSCH Midamble Configuration present */
        if (TDSRRC_CHECK_MSG_TYPE(hspdsch_info_ptr->modeSpecificInfo,
            T_tdsrrc_DL_HSPDSCH_Information_r6_modeSpecificInfo_tdd))
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd,
            rrc_DL_HSPDSCH_Information_r6_tdd_tdd128))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                hs_PDSCH_Midamble_Configuration_tdd128))
            {
              /* Check if HS-PDSCH Midamble Configuration has changed */
              if (tdsrrcllc_check_if_hspdsch_midamble_changed(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                  , FALSE
#endif
                ) == TRUE)
              {
                hspdsch_midamble_changed = TRUE;
              }
              tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                  ,FALSE
#endif
                );
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive midamble configuration tdd128");

              if (hspdsch_midamble_changed == TRUE)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:hspdsch midamble configuration changed");
                reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;
              }
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
          return FAILURE;
        }
      }

      /* Final Check to make sure that everything is present */
      if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTOP");
        tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
        return SUCCESS;
      }
      /* Set the queue information */
      if (tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set HSDPA info in L1");
        return FAILURE;
      }
      else
      {
        if ((tdshsdpa_msg_params.hsdsch_info_present == TRUE) ||
            ((tdshsdpa_msg_params.hrnti_present == TRUE) && (tdshsdpa_msg_params.hrnti_changed == TRUE)))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:TDSL1_CPHY_HS_DSCH_CFG_INCL set");
          reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
        }

      }
      /* no matter what freq change */
      if ((reconfig_bit_mask == 0) && 
          (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE) &&
          (tdsordered_config_ptr->is_hho== FALSE)&&
          (tdsordered_config_ptr->w_freq == tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Change action RECONFIG to NOOP");
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
        tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
        return SUCCESS;
      }
      

      tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = reconfig_bit_mask;

      /* Set Action to RECONFIG */
     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:HSRECONFIG bit mask %d,HS-RESET %d",reconfig_bit_mask,
               tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
      tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
      
      return SUCCESS;
      
    } /* End of if of hsdpa msg params present */
    else
    {
      /* Final Check to make sure that everything is present */
      if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTOP");
        tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
        return SUCCESS;
      }
      /* no matter what freq change, should reconfig HSDPA */
      if ((tdsordered_config_ptr->is_hho== TRUE)||
        (tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq change,HS Action RECONFIG");
        tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
        return SUCCESS;
      }

      /* Means No Action */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
      return SUCCESS;
    }
  }
}


#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
/*====================================================================
FUNCTION: tdsrrcllc_multicarrier_present_per_version_r7()

DESCRIPTION:

  RRC needs to check multi-carrier present
  
DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:

====================================================================*/
uint8 tdsrrcllc_multicarrier_present_per_version_r7
(
  tdsrrc_DL_HSPDSCH_Information_r7* hspdsch_info_ptr
)
{
  uint8 status = FALSE;
  if (TDSRRC_CHECK_MSG_TYPE(hspdsch_info_ptr->modeSpecificInfo,
      T_tdsrrc_DL_HSPDSCH_Information_r7_modeSpecificInfo_tdd))
  {
    if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
      hspdsch_info_ptr->modeSpecificInfo.u.tdd,
      rrc_DL_HSPDSCH_Information_r7_tdd_tdd128))
    {
      /* Check if DL Multi-Carrier Information present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
          hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
          dl_MultiCarrier_Information))
      {
        status = TRUE;
      }
    }
  }
  return status;
}
/*====================================================================
FUNCTION: tdsrrcllc_multicarrier_present_per_version_r8()

DESCRIPTION:

  RRC needs to check multi-carrier present
  
DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:

====================================================================*/
uint8 tdsrrcllc_multicarrier_present_per_version_r8
(
  tdsrrc_DL_HSPDSCH_Information_r8* hspdsch_info_ptr
)
{
  uint8 status = FALSE;
  if (TDSRRC_CHECK_MSG_TYPE(hspdsch_info_ptr->modeSpecificInfo,
      T_tdsrrc_DL_HSPDSCH_Information_r8_modeSpecificInfo_tdd))
  {
    if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
      hspdsch_info_ptr->modeSpecificInfo.u.tdd,
      rrc_DL_HSPDSCH_Information_r8_tdd_tdd128))
    {
      /* Check if DL Multi-Carrier Information present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
          hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
          dl_MultiCarrier_Information))
      {
        status = TRUE;
      }
    }
  }
  return status;
}
/*====================================================================
FUNCTION: tdsrrcllc_multicarrier_present_per_version_r9()

DESCRIPTION:

  RRC needs to check multi-carrier present
  
DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:

====================================================================*/
uint8 tdsrrcllc_multicarrier_present_per_version_r9
(
  tdsrrc_DL_HSPDSCH_Information_r9* hspdsch_info_ptr
)
{
  uint8 status = FALSE;
  if (TDSRRC_CHECK_MSG_TYPE(hspdsch_info_ptr->modeSpecificInfo,
      T_tdsrrc_DL_HSPDSCH_Information_r9_modeSpecificInfo_tdd))
  {
    if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
      hspdsch_info_ptr->modeSpecificInfo.u.tdd,
      rrc_DL_HSPDSCH_Information_r9_tdd_tdd128))
    {
      /* Check if DL Multi-Carrier Information present */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
          hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
          dl_MultiCarrier_Information))
      {
        status = TRUE;
      }
    }
  }
  return status;
}

/*====================================================================
FUNCTION      tdsrrcllc_check_multicarrier_hsscch_change_per_version()

DESCRIPTION   This function checks if HS-SCCH Info has changed.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
boolean tdsrrcllc_check_multicarrier_hsscch_change_per_version
(
  tdsrrc_DL_HSPDSCH_MultiCarrier_Information_hs_SCCH_TDD128_MultiCarrier *hs_scch_multiCarrier
)
{
  uint8 cnt = 0;
  tdsrrc_HS_SCCH_Info_tdd128   *local_scch_ptr = NULL;

  /* Check hsscch num */
  if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich != 
      hs_scch_multiCarrier->n)
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"num_hs_scch_sich is different");
    return TRUE;
  }

  for (cnt = 0; cnt < hs_scch_multiCarrier->n; cnt++)
  {
    /* Check timeslotNumber */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].ts_num_scch !=
      hs_scch_multiCarrier->elem[cnt].timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"timeslotNumber is different");
      return TRUE;
    }

    /* Check firstChannelisationCode */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_1st !=
      hs_scch_multiCarrier->elem[cnt].firstChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"firstChannelisationCode is different");
      return TRUE;
    }

    /* Check secondChannelisationCode */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].scch_chcode_2nd !=
      hs_scch_multiCarrier->elem[cnt].secondChannelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"secondChannelisationCode is different");
      return TRUE;
    }

    /* Check midambleAllocationMode */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.allocation_mode !=
          tdsMidambleAllocMode[hs_scch_multiCarrier->elem[cnt].midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"midambleAllocationMode is different");
      return TRUE;
    }

    /* Check midamble_shift */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(hs_scch_multiCarrier->elem[cnt].midambleAllocationMode,
        rrc_HS_SCCH_TDD128_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.shift != 
            hs_scch_multiCarrier->elem[cnt].midambleAllocationMode.u.ueSpecificMidamble)
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"midamble_shift is different");
        return TRUE;
      }
    }

    /* Check midambleConfiguration */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].hs_scch_midamble_cfg.config !=
          tdsMidambleConfiguration[hs_scch_multiCarrier->elem[cnt].midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"midambleConfiguration is different");
      return TRUE;
    }

    /* Check bler_target */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].bler_target !=
          hs_scch_multiCarrier->elem[cnt].bler_target)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"bler_target is different");
      return TRUE;
    }

    /* Check HS-SICH configuration */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.ts_num_sich !=
          hs_scch_multiCarrier->elem[cnt].hs_sich_configuration.timeslotNumber)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"hs_sich_configuration timeslotNumber is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.sich_chcode !=
          hs_scch_multiCarrier->elem[cnt].hs_sich_configuration.channelisationCode)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"hs_sich_configuration channelisationCode is different");
      return TRUE;
    }

    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.allocation_mode !=
          tdsMidambleAllocMode[hs_scch_multiCarrier->elem[cnt].hs_sich_configuration.midambleAllocationMode.t])
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"hs_sich_configuration midambleAllocationMode is different");
      return TRUE;
    }

    /* Check midamble_shift */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(hs_scch_multiCarrier->elem[cnt].hs_sich_configuration.midambleAllocationMode,
        rrc_HS_SCCH_TDD128_midambleAllocationMode_ueSpecificMidamble))
    {
      if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.shift != 
            hs_scch_multiCarrier->elem[cnt].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift)
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"hs_sich_configuration midamble_shift is different");
        return TRUE;
      }
    }
    
    /* Check midambleConfiguration */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.midamble_config_sich.config !=
          tdsMidambleConfiguration[hs_scch_multiCarrier->elem[cnt].hs_sich_configuration.midambleConfiguration])
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"midambleConfiguration is different");
      return TRUE;
    }

    /* Check nack_ack_power_offset */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.ack_nack_powoffset !=
          hs_scch_multiCarrier->elem[cnt].hs_sich_configuration.nack_ack_power_offset)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"nack_ack_power_offset is different");
      return TRUE;
    }

    /* Check power_level_HSSICH */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.prx_sich !=
          hs_scch_multiCarrier->elem[cnt].hs_sich_configuration.power_level_HSSICH)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"power_level_HSSICH is different");
      return TRUE;
    }

    /* Check tpc_step_size */
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].l1_sich_config.tpc_ssize !=
          (hs_scch_multiCarrier->elem[cnt].hs_sich_configuration.tpc_step_size+1))
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"tpc_step_size is different");
      return TRUE;
    }
  }

  return FALSE;
}

/*====================================================================
FUNCTION      tdsrrcllc_multicarrier_hsscch_info_per_version()

DESCRIPTION   This function sets the Channelization code required
              for HSPDSCH in L1 structure in OC. IEs refer to 10.3.6.36a.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_multicarrier_hsscch_info_per_version
(
  tdsrrc_DL_HSPDSCH_MultiCarrier_Information_hs_SCCH_TDD128_MultiCarrier *hs_scch_multiCarrier,
  boolean rel5_flag
)
{
  uint8 i = 0;

  /** Set common_scch_sich_params_present info 
         TRUE indicate the current IE using shared ck_nack_powoffset,etc */
  tdsordered_config_ptr->l1_hsdpa_info.common_scch_sich_params_present = FALSE;

  /* Get HS-SCCH Set Configuration list(R5) */
  tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich =
    hs_scch_multiCarrier->n;

  for (i = 0; i < hs_scch_multiCarrier->n; i++)
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].ts_num_scch =
      hs_scch_multiCarrier->elem[i].timeslotNumber;

    /** OC: scch_chcode_1st = 0, L1_SF16_CH_CODE_1;
              ASN1: firstChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_1st =
      hs_scch_multiCarrier->elem[i].firstChannelisationCode;

    /** OC: scch_chcode_2nd = 0, L1_SF16_CH_CODE_1;
               ASN1: secondChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_2nd =
      hs_scch_multiCarrier->elem[i].secondChannelisationCode;

    /* Get midamble info */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.allocation_mode =
      tdsMidambleAllocMode[hs_scch_multiCarrier->elem[i].midambleAllocationMode.t];

    if (hs_scch_multiCarrier->elem[i].midambleAllocationMode.t == 
          T_tdsrrc_HS_SCCH_TDD128_MultiCarrier_midambleAllocationMode_ueSpecificMidamble)
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.shift =
          hs_scch_multiCarrier->elem[i].midambleAllocationMode.u.ueSpecificMidamble;
    }

    /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
               ASN1: midambleConfiguration   Integer (1...8), */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.config =
      tdsMidambleConfiguration[hs_scch_multiCarrier->elem[i].midambleConfiguration];

    /* Get HS-SICH configuration info */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.ts_num_sich =
      hs_scch_multiCarrier->elem[i].hs_sich_configuration.timeslotNumber;

    /* OC: sich_chcode = 0, L1_SF16_CH_CODE_1;
               ASN1: channelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1*/
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.sich_chcode =
      hs_scch_multiCarrier->elem[i].hs_sich_configuration.channelisationCode;

    /* Get midamble info */
    switch (hs_scch_multiCarrier->elem[i].hs_sich_configuration.midambleAllocationMode.t)
    {
      case T_tdsrrc_HS_SICH_Configuration_TDD128_midambleAllocationMode_defaultMidamble:
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
            TDSL1_DEFAULT_MIDAMBLE;
        break;

      case T_tdsrrc_HS_SICH_Configuration_TDD128_midambleAllocationMode_ueSpecificMidamble:
        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
            TDSL1_UE_SPECIFIC_MIDAMBLE;

          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.shift =
            hs_scch_multiCarrier->elem[i].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
        break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"hs_sich_configuration.midambleAllocationMode is invalid");
        return FAILURE;
    }

    /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
               ASN1: midambleConfiguration   Integer (1...8), */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.config =
      tdsMidambleConfiguration[hs_scch_multiCarrier->elem[i].hs_sich_configuration.midambleConfiguration];

    /* Get Ack-Nack Power Offset info(R6) */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.ack_nack_powoffset =
      hs_scch_multiCarrier->elem[i].hs_sich_configuration.nack_ack_power_offset;

    /* Get PRXHS-SICH info(R6) */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.prx_sich =
      hs_scch_multiCarrier->elem[i].hs_sich_configuration.power_level_HSSICH;

    /* Get TPC step size(R6) info */
    if (hs_scch_multiCarrier->elem[i].hs_sich_configuration.tpc_step_size < tdsrrc_HS_SCCH_Info_r7_tpc_step_size_spare1)
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.tpc_ssize =
        hs_scch_multiCarrier->elem[i].hs_sich_configuration.tpc_step_size + 1;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TPC Step Size is absent in OTA message");
      tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.tpc_ssize = TDSL1_TPC_STEP_SIZE_0DB;
    }

    /* Get BLER target(R6) info,
      10.3.6.28a: The UE shall use the BLER target signalled in the first occurrence of the HS-SCCH Set Configuration.
    */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].bler_target =
            hs_scch_multiCarrier->elem[i].bler_target;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Set bler_target = %d", hs_scch_multiCarrier->elem[i].bler_target);
  }

  /* The two IEs in Rel5 Msg is present, and out of IE "dl_Multicarrier", so no need this code. Rel6/7/8/9 is absent */
  if (FALSE == rel5_flag)
  {
    /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;

    /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */
    tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = FALSE;
  }
  return SUCCESS;

}


/*====================================================================
FUNCTION: tdsrrcllc_check_multicarrier_support_per_version()

DESCRIPTION:

  RRC needs to check
  a.  Consistence of the multi-carrier SCCH freq and working freq. The two frequency should be same
  b.  Consistence of HARQ Number of Process in multi-carrier IE and dl-AddReconfTransChInfoList IE. The two number should be same.
  If any of above does not match, then RRC will return failure to NW.
  
DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:

====================================================================*/
uecomdef_status_e_type tdsrrcllc_check_multicarrier_support_per_version
(
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr,
  uint8 *reconfig_bit_mask,
  boolean rel5_flag
)
{
  uint8 i = 0;
  uecomdef_status_e_type status = SUCCESS;
  tdsrrc_freq_type work_freq = tdsordered_config_ptr->w_freq;
  uint8 num_proc = tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"muliticarrier check and save");

  if (multiCarrier_info_ptr->m.tsn_LengthPresent)
  {
    if (multiCarrier_info_ptr->tsn_Length == tdsrrc_DL_MultiCarrier_Information_tsn_Length_tsn_9bits)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"muliticarrier tsn_Length is 9, not support");
      status = FAILURE;
    }
  }

  if ((status == SUCCESS)&&(multiCarrier_info_ptr->m.dl_HSPDSCH_MultiCarrier_InformationPresent))
  {
    if (multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.n > 1)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"muliticarrier number >1, not signal");
      status = FAILURE;
    }
    else if (multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.n == 1)
    {
      /* HARQ Number of Process in multi-carrier IE and dl-AddReconfTransChInfoList IE should be same */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"muliticarrier:check HARQ Number");
      if (multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.
            elem[0].m.harqInfoPresent)
      {
        if (tdsordered_config_ptr->harq_present)
        {
          if(num_proc != multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.
              elem[0].harqInfo.numberOfProcesses)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"muliticarrier:harq number not match");
            status = FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"muliticarrier:save HARQ info");
          tdsrrcllc_set_harq_information(&multiCarrier_info_ptr->
            dl_HSPDSCH_MultiCarrier_Information.elem[0].harqInfo);
        }
      }

      /* the multi-carrier SCCH freq and working freq should be same */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"muliticarrier:check w_freq and multi-carrier SCCH freq");
      if((status == SUCCESS)&&(work_freq != multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.
            elem[0].uarfcn_Carrier))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"muliticarrier:uarfcn_Carrier not match");
        status = FAILURE;
      }
      if((status == SUCCESS)&&(multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.
            elem[0].m.hs_SCCH_TDD128_MultiCarrierPresent))
      {
        for(i = 0; i < multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.
          elem[0].hs_SCCH_TDD128_MultiCarrier.n; i++)
        {
          if(work_freq != multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.
                elem[0].hs_SCCH_TDD128_MultiCarrier.elem[i].uarfcn_HS_SCCH_Rx)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"muliticarrier:uarfcn_HS_SCCH_Rx not match");
            status = FAILURE;
            break;
          }
        }
        
        if ((tdsrrc_get_hsdpa_status() != TDSHSDPA_INACTIVE)&&(reconfig_bit_mask != NULL))
        {
          if (tdsrrcllc_check_multicarrier_hsscch_change_per_version(&multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.
            elem[0].hs_SCCH_TDD128_MultiCarrier) == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"muliticarrier:SCCH Info changed");
            *reconfig_bit_mask = (*reconfig_bit_mask) | TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL;
          }
        }
        if(tdsrrcllc_multicarrier_hsscch_info_per_version(&multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.
            elem[0].hs_SCCH_TDD128_MultiCarrier, rel5_flag) != SUCCESS)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"muliticarrier:hsscch_info failed");
          status = FAILURE;
        }
      }
      if (multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.elem[0].m.hs_PDSCH_Midamble_ConfigurationPresent)
      {
        if ((tdsrrc_get_hsdpa_status() != TDSHSDPA_INACTIVE)&&(reconfig_bit_mask != NULL))
        {
          if (tdsrrcllc_check_if_hspdsch_midamble_changed(&multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.elem[0].
            hs_PDSCH_Midamble_Configuration
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                              , FALSE
#endif
            ) == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"muliticarrier:SCCH Info changed");
            *reconfig_bit_mask = (*reconfig_bit_mask) | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;
          }
        }
        tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
          &multiCarrier_info_ptr->dl_HSPDSCH_MultiCarrier_Information.elem[0].
            hs_PDSCH_Midamble_Configuration
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                            ,FALSE
#endif
            );
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"muliticarrier:Receive midamble configuration tdd128");
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"muliticarrier number = 0");
    }
  }

  return status;

}

#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION

/*====================================================================
FUNCTION: tdsrrcllc_determine_hsdpa_action()

DESCRIPTION:

  This function determines the HSDPA action and sets the parameters in L1
  structure accordingly.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  Sucess if HSDPA action and parameters have been set
  Failure Otherwise

SIDE EFFECTS:
  
====================================================================*/
uecomdef_status_e_type tdsrrcllc_determine_hsdpa_action
(
  tdsrrc_DL_HSPDSCH_Information* hspdsch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,uint8 multicarrier_flg,
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr
#endif
)
{
  uint8 reconfig_bit_mask = 0;
  boolean hsscch_codes_changed = FALSE;
  boolean hspdsch_midamble_changed = FALSE;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS 
  uint8 i = 0;
#endif

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  if(tdsrrcllc_set_macehs_or_machs_and_check_if_coexist()== TRUE)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: MAC EHS %d and MAC HS %d cant co-exist ",
                        tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue, 
                        tdsordered_config_ptr->mac_dl_parms.num_dflow);
    return FAILURE;
  }

  tdsrrcllc_set_machs_ehs_transition();

  if(((tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_HS_TO_EHS )
        ||(tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_EHS_TO_HS ))
        && (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE))
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"MAC-HS <->  MAC-EHS transition %d, and Mac Reset Ind not set %d, reject config",
            tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type,
            tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
    return FAILURE;
  }
#endif
 
#ifdef FEATURE_TDSCDMA_64QAM
  if(tdshsdpa_msg_params.hspdsch_info_present )
  {
    if(tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured == TRUE)
    {
      tdshsdpa_msg_params.hsdsch_info_present = TRUE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set HS-DSCh bitmask as 64 QAM is stopped");
    }
  }
#endif

  /* Save UE category info */
  if (tdsnv_hsdpa_category > 15)
  {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if (0 == tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue)
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
#endif
    }
    else
    {
      for(i=0;i< tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue;i++)
      {
        if ((tdsordered_config_ptr->dl_macehs_logchan_info[i].queue_id != TDSINVALID_QUEUE_ID) &&
          (tdsordered_config_ptr->mac_ehs_lc_id_in_use[i] == TRUE ))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 23;
          break;
        }
        else
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
          if (i == (tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue - 1))
          {
            tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
          }
#endif
        }
      }
    }
#else
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#endif
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = tdsnv_hsdpa_category;
  }

  /* HSDPA params have already been set in rrc_hsdpa_params.
  Check if HSDPA is already ACTIVE */
  if (tdsrrc_get_hsdpa_status() == TDSHSDPA_INACTIVE)
  {
    /* HSDPA is currently inactive */
    /* Check if network wants to activate HSDPA */
    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {
      if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
      {
        /* Check if channelization codes and feedback info present */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,
            tdsrrc_DL_HSPDSCH_Information,hs_scch_Info))
        {
          if (FAILURE == tdsrrcllc_set_hsscch_info(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                      , multicarrier_flg
#endif
            ))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid HS-SCCH info");
            return FAILURE;
          }
        }

        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,
            tdsrrc_DL_HSPDSCH_Information,
            measurement_feedback_Info))
        {
          /* Set Meas Feedback Info */
          if (FAILURE == tdsrrcllc_set_meas_feedback_info(
            &hspdsch_info_ptr->measurement_feedback_Info))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid Meas Feedback info");
            return FAILURE;
          }
        }
        /* Check if HS-PDSCH Midamble Configuration present */
        if (TDSRRC_CHECK_MSG_TYPE(hspdsch_info_ptr->modeSpecificInfo,
            T_tdsrrc_DL_HSPDSCH_Information_modeSpecificInfo_tdd))
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd,
            rrc_DL_HSPDSCH_Information_tdd_tdd128))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                hs_PDSCH_Midamble_Configuration_tdd128))
            {
              tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                                , multicarrier_flg
#endif
                 );
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive midamble configuration tdd128");
            }
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
            if((multicarrier_flg == TRUE)&&(FAILURE == tdsrrcllc_check_multicarrier_support_per_version
                  (multiCarrier_info_ptr, NULL, TRUE)))
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
              return FAILURE;
            }
#endif
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
          return FAILURE;
        }
      }
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
      else if (TRUE == multicarrier_flg)
      {
        if(FAILURE == tdsrrcllc_check_multicarrier_support_per_version(multiCarrier_info_ptr, NULL, TRUE))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
          return FAILURE;
        }
      }
#endif
    } /* End of if of hsdpa message params absent */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_reset_mac_ehs_params(tdsordered_config_ptr);

#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
      
      return SUCCESS;
    }

    /* Set Queue Info */
    if (tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set Queue Info");
      return FAILURE;
    }

    /* Final Check to make sure that everything is present */
    if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
      return SUCCESS;
    }


    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTART");
    tdsrrc_set_hsdpa_action(TDSHSDPA_START);

    /* Set Bit Mask */
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = TDSL1_CPHY_HS_DSCH_CFG_INCL | 
      TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL| TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;


    TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:HS Bit Mask is (Dec) %d (Hex) %x", tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask,
             tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);

    /* In case UTRAN messes up, set HS-RESET to FALSE */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Force HS-RESET to FALSE");
    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = FALSE;
    return SUCCESS;
    
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSDPA already active");
    /* HSDPA is already ACTIVE */
    
    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {

      /* Set bit mask for HO */
      if ((tdsordered_config_ptr->is_hho== TRUE)||
        (tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:hho, dpa need reconfig ");
        reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
      }


      if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
      {
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information,hs_scch_Info))
        {
          /* Check if HS-SCCH Info has changed */
          if (tdsrrcllc_check_if_hsscch_changed_r5(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                              , multicarrier_flg
#endif
            ) == TRUE)
          {
            hsscch_codes_changed = TRUE;
          }
          if (tdsrrcllc_set_hsscch_info(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                      , multicarrier_flg
#endif
            ) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid HS-SCCH info");
            return FAILURE;
          }
          else
          {
            if (hsscch_codes_changed == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:SCCH Info changed");
              reconfig_bit_mask =reconfig_bit_mask | TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL;
            }
          }
        }
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information,measurement_feedback_Info))
        {
          if (tdsrrcllc_set_meas_feedback_info(&hspdsch_info_ptr->measurement_feedback_Info) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid Meas Feedback info");
            return FAILURE;
          }
        }
        /* Check if HS-PDSCH Midamble Configuration present */
        if (TDSRRC_CHECK_MSG_TYPE(
            hspdsch_info_ptr->modeSpecificInfo,
            T_tdsrrc_DL_HSPDSCH_Information_modeSpecificInfo_tdd))
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd,
            rrc_DL_HSPDSCH_Information_tdd_tdd128))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                hs_PDSCH_Midamble_Configuration_tdd128))
            {
              /* Check if HS-PDSCH Midamble Configuration has changed */
              if (tdsrrcllc_check_if_hspdsch_midamble_changed(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                  , multicarrier_flg
#endif
                ) == TRUE)
              {
                hspdsch_midamble_changed = TRUE;
              }
              tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                                , multicarrier_flg
#endif
                );
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive midamble configuration tdd128");
              if (hspdsch_midamble_changed == TRUE)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:hspdsch midamble configuration changed");
                reconfig_bit_mask =reconfig_bit_mask | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;
              }
            }
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
            if((multicarrier_flg == TRUE)&&(FAILURE == tdsrrcllc_check_multicarrier_support_per_version
                  (multiCarrier_info_ptr, &reconfig_bit_mask, TRUE)))
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
              return FAILURE;
            }
#endif    
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
          return FAILURE;
        }
      }
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
      else if (TRUE == multicarrier_flg)
      {
        if(FAILURE == tdsrrcllc_check_multicarrier_support_per_version(multiCarrier_info_ptr, &reconfig_bit_mask, TRUE))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
          return FAILURE;
        }
      }
#endif
      /* Final Check to make sure that everything is present */
      if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTOP");
        tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
        return SUCCESS;
      }


      /* Set the queue information */
      if (tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set HSDPA info in L1");
        return FAILURE;
      }
      else
      {
        if ((tdshsdpa_msg_params.hsdsch_info_present == TRUE) ||
            ((tdshsdpa_msg_params.hrnti_present == TRUE) && (tdshsdpa_msg_params.hrnti_changed == TRUE)))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:TDSL1_CPHY_HS_DSCH_CFG_INCL set");
          reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
        }
      }
      /* no matter what freq change */
      if ((reconfig_bit_mask == 0) && 
          (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE) &&
          (tdsordered_config_ptr->is_hho== FALSE)&&
          (tdsordered_config_ptr->w_freq == tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Change action RECONFIG to NOOP");
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
        tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
        return SUCCESS;
      }


      tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = reconfig_bit_mask;

      /* Set Action to RECONFIG */
     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:HSRECONFIG bit mask %d,HS-RESET %d",reconfig_bit_mask,
               tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
      tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
      
      return SUCCESS;
      
    } /* End of if of hsdpa msg params present */
    else
    {
      /* Final Check to make sure that everything is present */
      if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTOP");
        tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
        return SUCCESS;
      }
      /* no matter what freq change, should reconfig HSDPA */
      if ((tdsordered_config_ptr->is_hho== TRUE)
        ||(tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq change,HS Action RECONFIG");
        tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
        return SUCCESS;
      }

      /* Means No Action */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
      return SUCCESS;
    }
  }
}


/*====================================================================
FUNCTION: tdsrrcllc_init_hsdpa_mac_ptr()

DESCRIPTION:
  This function initializes the DFLOW Pointer in MAC.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:

====================================================================*/
static void  tdsrrcllc_init_hsdpa_mac_ptr(tdsordered_config_type *config_ptr)
{
  uint8  ix;
  TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Init HS MAC Ptr");
  for(ix = 0 ; ix < UE_MAX_MAC_D_FLOW; ix++)
  {
    config_ptr->mac_dl_parms.hsdpa_info[ix] = &config_ptr->mac_dflow_info[ix];
  }
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  for(ix = 0 ; ix < UE_MAX_HS_QUEUE; ix++)
  {
    config_ptr->mac_dl_parms.hsdpa_ehs_queue_info[ix] = &config_ptr->mac_ehs_queue_info[ix];
  }

  for(ix = 0 ; ix < TDSUE_MAX_DL_LOGICAL_CHANNEL; ix++)
  {
    config_ptr->mac_dl_parms.dl_macehs_logchan_info[ix] = &config_ptr->dl_macehs_logchan_info[ix];
  }
#endif
}


#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
/*====================================================================
FUNCTION: tdsrrcllc_check_if_rb_mapped_on_macehs_q

DESCRIPTION:
  This function checks if a RB is mapped to  MACEHS passed
  config ptr.

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE if RB is found to be mapped on a MACEHS else FALSE

SIDE EFFECTS:
  None.
====================================================================*/
static boolean tdsrrcllc_check_if_rb_mapped_on_macehs_q
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_RB_Identity  rb_id
)
{
  uint32 queue_cnt=0;
  uint32 num_queue=0;
  uint32 nlchl_cnt=0;
  num_queue =config_ptr->mac_dl_parms.num_mac_ehs_queue;
  for(queue_cnt=0;queue_cnt < num_queue;queue_cnt++)
  {
    if ((config_ptr->mac_ehs_queue_info[queue_cnt].queue_id != TDSINVALID_QUEUE_ID)
        && (config_ptr->mac_ehs_queue_index_in_use[queue_cnt] == TRUE))
    {
      for(nlchl_cnt =0; (nlchl_cnt <config_ptr->mac_dl_parms.ndlchan_macehs);nlchl_cnt++)
      {
        if ((config_ptr->mac_ehs_lc_id_in_use[nlchl_cnt] == TRUE) 
           && (config_ptr->dl_macehs_logchan_info[nlchl_cnt].queue_id == 
           config_ptr->mac_ehs_queue_info[queue_cnt].queue_id)&&
            (config_ptr->dl_macehs_logchan_info[nlchl_cnt].rb_id == rb_id)
            && (config_ptr->dl_macehs_logchan_info[nlchl_cnt].chan_type == UE_LOGCHAN_DTCH))
        {
          return TRUE;
        }
      }
    }
  }
  return (FALSE);
}

/*===========================================================================

FUNCTION tdsrrcllc_set_hsdpa_action_in_machs_macehs

DESCRIPTION


  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcllc_set_hsdpa_action_in_machs_macehs
(
  void
)
{
  /*The additional check for HSDPA_STOP is done to avoid over-writing HSDPA action in the case of
  MAC-EHS -> MAC-HS mapping change alongwith HSDPA STOP*/
  if((tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_HS_TO_EHS) && 
    (tdsrrc_get_hsdpa_action() != TDSHSDPA_STOP))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS: Set HS Action to reconfig, for HS to EHS transtion");
    tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = TDSHSDPA_RECONFIG;
    tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
  }
  /*The additional check for HSDPA_STOP is done to avoid over-writing HSDPA action in the case of
  MAC-HS -> MAC-EHS mapping change alongwith HSDPA STOP*/  
  else if((tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type == TDSMAC_EHS_TO_HS) && 
    (tdsrrc_get_hsdpa_action() != TDSHSDPA_STOP))
  {
    tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = TDSHSDPA_RECONFIG;
    tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS: Set HS Action to reconfig, for EHS to HS transtion");
  }
  else
  {
    tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = tdsrrc_get_hsdpa_action();

    if((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE ) ||
        (tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_START) ||
        (tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action == TDSHSDPA_STOP))
    {
      tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type = TDSMAC_HS_NO_TRANSITION;
    }

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: Set HS Action to %d Transition type %d ",
                                             tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action,
                                             tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type);
  }
}

/*===========================================================================

FUNCTION tdsrrcllc_reset_mac_ehs_params

DESCRIPTION


  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcllc_reset_mac_ehs_params
(
  tdsordered_config_type * config_ptr
)
{
  if(tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE)
  {
     config_ptr->mac_dl_parms.mac_hsdpa_action = TDSHSDPA_STOP;
                /* HSDPA to be stopped */
     tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
  }
  else
  {
    config_ptr->mac_dl_parms.mac_hs_or_ehs = MAC_HS_TYPE_INVALID;
    config_ptr->mac_dl_parms.mac_hs_transition_type= TDSMAC_HS_INVALID_TRANSITION;
    config_ptr->l1_hsdpa_info.hs_dsch_info.mac_hs_or_ehs = MAC_HS_TYPE_INVALID;

  }
}

/*===========================================================================

FUNCTION tdsrrcllc_set_machs_ehs_transition

DESCRIPTION


  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcllc_set_machs_ehs_transition
(
void
)
{
    tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type = TDSMAC_HS_NO_TRANSITION;
    if(tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE )
    {
      if((tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS) &&
          (tdscurrent_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_EHS))
      {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Transition from MAC EHS to MAC HS");
          tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type = TDSMAC_EHS_TO_HS;
      }
      else if((tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_EHS) &&
          (tdscurrent_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS))
      {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Transition from MAC HS to MAC EHS");
          tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type = TDSMAC_HS_TO_EHS;
      }
    }
    return;
}
/*====================================================================
FUNCTION: tdsrrcllc_get_macehs_queue_index()

DESCRIPTION:

DEPENDENCIES:
  None.

RETURN VALUE:


SIDE EFFECTS:

====================================================================*/
uint8  tdsrrcllc_allocate_lc_index_macehs
( 
  tdsordered_config_type *config_ptr,
  tdsrrc_LogicalChannelIdentity logicalChannelIdentity
)
{

  uint8 cnt = TDSUE_MAX_DL_LOGICAL_CHANNEL;
  /* An index with the same DFLOW ID is NOT in use.
  * Allocate a new index */
  for (cnt =0; cnt <TDSUE_MAX_DL_LOGICAL_CHANNEL; cnt++)
  {
    if ((tdsordered_config_ptr->mac_ehs_lc_id_in_use[cnt] == FALSE) &&
      (tdsordered_config_ptr->dl_macehs_logchan_info[cnt].mac_id == TDSINVALID_LOGICAL_CHANNEL_ID))
    {
      /*give it to MAC as LC id -1 as recieved in MAC EHS header*/
      tdsordered_config_ptr->dl_macehs_logchan_info[cnt].mac_id= logicalChannelIdentity -1;

      /* Increment no of DFLOWs */
      tdsordered_config_ptr->mac_dl_parms.ndlchan_macehs ++;
      
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS:Assign Index %d for new LC ID %d", 
         cnt,logicalChannelIdentity);
      tdsordered_config_ptr->mac_ehs_lc_id_in_use[cnt] = TRUE;
      break;
    }
  }
  return cnt;

}
/*====================================================================
FUNCTION: tdsrrcllc_get_macehs_queue_index()

DESCRIPTION:

DEPENDENCIES:
  None.

RETURN VALUE:


SIDE EFFECTS:

====================================================================*/
uint8 tdsrrcllc_find_lc_index_for_macehs
( 
  tdsordered_config_type *config_ptr,
  tdsrrc_LogicalChannelIdentity logicalChannelIdentity,
  tdsrrc_RB_Identity rb_id
)
{

  uint8 index = TDSUE_MAX_DL_LOGICAL_CHANNEL;

  for(index = 0; index < TDSUE_MAX_DL_LOGICAL_CHANNEL; index++)
  {
    if((tdsordered_config_ptr->mac_ehs_lc_id_in_use[index] == TRUE) &&
      (tdsordered_config_ptr->dl_macehs_logchan_info[index].mac_id == (logicalChannelIdentity -1)))
    {
      if(rb_id != config_ptr->dl_macehs_logchan_info[index].rb_id )
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS:LCH Id index %d, RB Id %d",logicalChannelIdentity-1,rb_id);
        continue;
      }
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS:LC Id index %d, Id %d",index, logicalChannelIdentity);

      return index;
    }
  }

  return TDSUE_MAX_DL_LOGICAL_CHANNEL;
  
}
   
/*====================================================================
FUNCTION: tdsrrcllc_init_hsdpa_mac_ehs_info()

DESCRIPTION:
  This function initializes the HSDPA Information in the configuration
  database.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllc_init_hsdpa_mac_ehs_info(tdsordered_config_type  *config_ptr)
{
  uint8 idx = 0;
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS: Reset MAC-EHS parameters");
  
  tdsrrcllc_reset_mac_ehs_params(config_ptr);


  config_ptr->mac_dl_parms.num_mac_ehs_queue = 0;

  for(idx = 0; idx < UE_MAX_HS_QUEUE ; idx++)
  {
    config_ptr->mac_ehs_queue_info[idx].queue_id = TDSINVALID_QUEUE_ID;
    config_ptr->mac_ehs_queue_info[idx].release_timer = 0;
    config_ptr->mac_ehs_queue_info[idx].window_size = 0;
    config_ptr->mac_ehs_queue_index_in_use[idx] = FALSE;
  }
  
  config_ptr->mac_dl_parms.ndlchan_macehs = 0;
  for(idx = 0; idx < TDSUE_MAX_DL_LOGICAL_CHANNEL; idx++)
  {
    config_ptr->mac_ehs_lc_id_in_use[idx] = FALSE;
    config_ptr->dl_macehs_logchan_info[idx].queue_id = TDSINVALID_QUEUE_ID;
    config_ptr->dl_macehs_logchan_info[idx].mac_id = TDSINVALID_LOGICAL_CHANNEL_ID;
    config_ptr->dl_macehs_logchan_info[idx].rb_id = TDSRRC_INVALID_RB_ID;
    config_ptr->dl_macehs_logchan_info[idx].rlc_id = 0;
    config_ptr->dl_macehs_logchan_info[idx].chan_type = UE_LOGCHAN_NONE;
    config_ptr->dl_macehs_logchan_info[idx].rlc_mode = UE_MODE_MAX_NUM;
  }
}
/*====================================================================
FUNCTION: tdsrrcllc_rearrange_macehs_queue_index()

DESCRIPTION:
  This function deletes any holes
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/

void tdsrrcllc_rearrange_macehs_queue_index
(
  tdsordered_config_type *config_ptr
)
{
  uint32  ix, iy;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Rearranging queue index");

  if (tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:MAC EHS 0");
    return;
  }
 /* Put all the values in tmp structure. No there are no holes in tmp structure */
  for(ix = UE_MAX_HS_QUEUE; ix > 0; ix--)
  {
    /* Check if valid data */
    if(tdsordered_config_ptr->mac_ehs_queue_info[ix-1].queue_id!= TDSINVALID_QUEUE_ID)
    {
      for(iy = 0; iy < (ix - 1); iy++)
      {
        /* Check if valid data */
        if(tdsordered_config_ptr->mac_ehs_queue_info[iy].queue_id== TDSINVALID_QUEUE_ID)
        {
          tdsordered_config_ptr->mac_ehs_queue_index_in_use[iy] =  
                    tdsordered_config_ptr->mac_ehs_queue_index_in_use[ix-1];
          memscpy(&tdsordered_config_ptr->mac_ehs_queue_info[iy], sizeof(tdsmac_ehs_queue_struct_type),
                 &tdsordered_config_ptr->mac_ehs_queue_info[ix-1], 
                 sizeof(tdsmac_ehs_queue_struct_type));
          tdsordered_config_ptr->mac_ehs_queue_info[ix-1].queue_id= TDSINVALID_QUEUE_ID;
          tdsordered_config_ptr->mac_ehs_queue_info[ix-1].release_timer = 0;
          tdsordered_config_ptr->mac_ehs_queue_info[ix-1].window_size = 0;
          tdsordered_config_ptr->mac_ehs_queue_index_in_use[ix-1] = FALSE;
          break;
        }
      }
      if (iy == (ix-1)) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"MACEHS: Rearranging MAC EHS Q complete. Total %d",
                  config_ptr->mac_dl_parms.num_mac_ehs_queue);
        break;
      }
    }
  } /* End of for (...) */

}

/*====================================================================
FUNCTION: tdsrrcllc_allocate_macehs_q_index()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A EDCH MAC-d flow Index.
  UE_MAX_MAC_D_FLOW -> Given EDCH Mac-d flow does not
           have an allocated DCH transport channel index.

SIDE EFFECTS:
====================================================================*/
uint8  tdsrrcllc_allocate_macehs_q_index
(
  uint8  mac_ehs_q_id
)
{

  uint8 cnt = UE_MAX_HS_QUEUE;
  /* An index with the same DFLOW ID is NOT in use.
  * Allocate a new index */
  for (cnt =0; cnt <UE_MAX_HS_QUEUE; cnt++)
  {
    if ((tdsordered_config_ptr->mac_ehs_queue_index_in_use[cnt] == FALSE) &&
      (tdsordered_config_ptr->mac_ehs_queue_info[cnt].queue_id== TDSINVALID_QUEUE_ID ))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MACEHS:Assign Index %d for new MACEHS Q", cnt);
      memset(&tdsordered_config_ptr->mac_ehs_queue_info[cnt], 0, sizeof(tdsmac_ehs_queue_struct_type));
      tdsordered_config_ptr->mac_ehs_queue_info[cnt].queue_id =mac_ehs_q_id;
      
      tdsordered_config_ptr->mac_ehs_queue_index_in_use[cnt] = TRUE;

      break;
    }
  }
  return cnt;

}
/*====================================================================
FUNCTION: tdsrrcllc_get_macehs_queue_index()

DESCRIPTION:

DEPENDENCIES:
  None.

RETURN VALUE:


SIDE EFFECTS:

====================================================================*/
uint8 tdsrrcllc_get_macehs_queue_index
(
  uint8  mac_ehs_q_id
)
{
  uint8 index = UE_MAX_HS_QUEUE;

  for(index = 0; index < UE_MAX_HS_QUEUE; index++)
  {
    if((tdsordered_config_ptr->mac_ehs_queue_index_in_use[index] == TRUE) &&
      (tdsordered_config_ptr->mac_ehs_queue_info[index].queue_id == mac_ehs_q_id))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: MAC EHS Q Id %d at index %d", mac_ehs_q_id,index);
      return(index);
    }
  }
  
  /* MAC EHS Q index not found, Allocate new */
  
  index = tdsrrcllc_allocate_macehs_q_index(mac_ehs_q_id);
  if(index == UE_MAX_HS_QUEUE)
  {
    /* Failure to allocate a new DFLOW Index */
    return(UE_MAX_HS_QUEUE);
  }
  else  /* A valid new DFLOW index has been allocated */
  {
  
    /* Now Save Q ID in MAC structure */
    tdsordered_config_ptr->mac_ehs_queue_info[index].queue_id = mac_ehs_q_id;
  
    /* Increment no of MAC EHS Qs */
    tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue ++;
  
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MACEHS:New MAC EHS cnt %d in MAC struct", tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue);
  
    return(index);
  
  } /* A valid new MAC EHS Q index allocated */
  
}

/*====================================================================
FUNCTION: tdsrrcllc_get_ehs_log_chan_mode()

DESCRIPTION:
  This function gets a the log_chan_mode for the rb id specified if the rb_id is mapped to EHS
  queues

DEPENDENCIES:
  None.

RETURN VALUE:
  Logical Channel mode of the rb_id    if rb_id is found mapped to a EHS queue
  UE_MODE_MAX_NUM     otherwise

SIDE EFFECTS:

====================================================================*/
uecomdef_logch_mode_e_type tdsrrcllc_get_ehs_log_chan_mode
(
  tdsrrc_RB_Identity rb_id
)
{
  uint32 queue_cnt=0;
  uint32 num_queue=0;
  uint32 nlchl_cnt=0;
  num_queue =tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue;
  for(queue_cnt=0;queue_cnt < num_queue;queue_cnt++)
  {
    if (tdsordered_config_ptr->mac_ehs_queue_info[queue_cnt].queue_id != TDSINVALID_QUEUE_ID)
    {
      for(nlchl_cnt =0; (nlchl_cnt <tdsordered_config_ptr->mac_dl_parms.ndlchan_macehs);nlchl_cnt++)
      {
        if (((tdsordered_config_ptr->dl_macehs_logchan_info[nlchl_cnt].queue_id) == 
           (tdsordered_config_ptr->mac_ehs_queue_info[queue_cnt].queue_id))&&
            (tdsordered_config_ptr->dl_macehs_logchan_info[nlchl_cnt].rb_id == rb_id))
        {
          return (tdsordered_config_ptr->dl_macehs_logchan_info[nlchl_cnt].rlc_mode);
        }
      }
    }
  }
  return (UE_MODE_MAX_NUM);
}

#endif/*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

/*====================================================================
FUNCTION      tdsrrcllc_set_meas_feedback_info_r7()

DESCRIPTION   This function sets the Meas Feedback Information in
              L1 structure in OC.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_set_meas_feedback_info_r7
(
  tdsrrc_Measurement_Feedback_Info_r7* meas_feedback_ptr
)
{
  if (TDSRRC_CHECK_MSG_TYPE(meas_feedback_ptr->modeSpecificInfo,
                 T_tdsrrc_Measurement_Feedback_Info_r7_modeSpecificInfo_tdd))
  {
    /* choice TDD, Measurement Feedback Info no data */
    /* tdsrrc_UL_DPCH_PowerControlInfo_r5 : ACK/NACK/REPITITON factor from Power Control */
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid Meas Feedback Info");
    return FAILURE;
  }
  return SUCCESS;

}
/*====================================================================
FUNCTION      tdsrrcllc_set_hsscch_info_r7()

DESCRIPTION   This function sets the Channelization code required
              for HSPDSCH in L1 structure in OC. IEs refer to 10.3.6.36a.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_set_hsscch_info_r7
(
  tdsrrc_HS_SCCH_Info_r7* hsscch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg
#endif
)
{
  struct tdsrrc_HS_SCCH_Info_r7_tdd128 *tmp_ptr = NULL;
  uint8 i = 0;

#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  if (multicarrier_flg)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MultiCarrier present, ignore hsscch info");
    return SUCCESS;
  }
#endif

  if (TDSRRC_CHECK_MSG_TYPE(hsscch_info_ptr->modeSpecificInfo,
        T_tdsrrc_HS_SCCH_Info_r7_modeSpecificInfo_tdd))
  {
    /* Get HSSCCH info */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(hsscch_info_ptr->modeSpecificInfo.u.tdd,
          rrc_HS_SCCH_Info_r7_tdd_tdd128))
    {
       tmp_ptr = hsscch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128;
       
       /** Set common_scch_sich_params_present info 
                TRUE indicate the current IE using shared ck_nack_powoffset,etc */
       tdsordered_config_ptr->l1_hsdpa_info.common_scch_sich_params_present = TRUE;

       /* Get Ack-Nack Power Offset info(R6) */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.ack_nack_powoffset =
         tmp_ptr->nack_ack_power_offset;

       /* Get PRXHS-SICH info(R6) */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.prx_sich =
          tmp_ptr->power_level_HSSICH;

       /* Get TPC step size(R6) info */
       if (tmp_ptr->tpc_step_size < tdsrrc_HS_SCCH_Info_r7_tpc_step_size_spare1)
       {
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize =
           tmp_ptr->tpc_step_size + 1;
       }
       else
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"TPC Step Size is absent in OTA message");
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize = TDSL1_TPC_STEP_SIZE_0DB;
       }

       /* Get BLER target(R6) info */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.bler_target =
          tmp_ptr->bler_target;
	     TDSRRC_MSG1(MSG_LEGACY_HIGH,"Set bler_target = %d", tmp_ptr->bler_target);

       /* Get Power Control GAP(R7) info */
       if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ptr, powerControlGAP))
       {
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap =
          tmp_ptr->powerControlGAP;
       }
       else
       {
         /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;
       }

       /* Get Pathloss compensation switch(R7) info */
       if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ptr,
            pathlossCompensationSwitch))
       {
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
            tmp_ptr->pathlossCompensationSwitch;
       }
       else
       {
		   if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(tmp_ptr, powerControlGAP))
		   {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set the do_pathloss_compens to 1, because powerControlGAP IE exists");
			        tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
              TRUE;

		   }
		   else
		   {
         /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
           FALSE;
       }
       }

       /* Get HS-SCCH Set Configuration list(R5) */
       tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich =
         tmp_ptr->hS_SCCH_SetConfiguration.n;

       for (i = 0; i < tmp_ptr->hS_SCCH_SetConfiguration.n; i++)
       {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].ts_num_scch =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].timeslotNumber;

          /** OC: scch_chcode_1st = 0, L1_SF16_CH_CODE_1;
                    ASN1: firstChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_1st =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].firstChannelisationCode;

          /** OC: scch_chcode_2nd = 0, L1_SF16_CH_CODE_1;
                     ASN1: secondChannelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1 */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].scch_chcode_2nd =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].secondChannelisationCode;

          /* Get midamble info */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.allocation_mode =
            tdsMidambleAllocMode[tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleAllocationMode.t];

          if (TDSRRC_CHECK_COMMON_MSG_TYPE(tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleAllocationMode,
                rrc_HS_SCCH_TDD128_r6_midambleAllocationMode_ueSpecificMidamble))
          {
            tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.shift =
                tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleAllocationMode.u.ueSpecificMidamble;
          }

          /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
                     ASN1: midambleConfiguration   Integer (1...8), */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].hs_scch_midamble_cfg.config =
            tdsMidambleConfiguration[tmp_ptr->hS_SCCH_SetConfiguration.elem[i].midambleConfiguration];

          /* Get HS-SICH configuration info */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.ts_num_sich =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.timeslotNumber;

          /* OC: sich_chcode = 0, L1_SF16_CH_CODE_1;
                     ASN1: channelisationCode = 0, tdsrrc_HS_ChannelisationCode_LCR_cc16_1*/
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.sich_chcode =
            tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.channelisationCode;

          /* Get midamble info */
          switch (tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.midambleAllocationMode.t)
          {
            case T_tdsrrc_HS_SICH_Configuration_TDD128_r6_midambleAllocationMode_defaultMidamble:
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
                  TDSL1_DEFAULT_MIDAMBLE;
                break;
          
            case T_tdsrrc_HS_SICH_Configuration_TDD128_r6_midambleAllocationMode_ueSpecificMidamble:
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.allocation_mode =
                  TDSL1_UE_SPECIFIC_MIDAMBLE;
          
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.shift =
                  tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.midambleAllocationMode.u.ueSpecificMidamble->midambleShift;
                break;
            default:
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"hs_sich_configuration.midambleAllocationMode is invalid");
              return FAILURE;
          }

          /* OC: midamble_config = 0, L1_MIDAMBLE_CONFIG_2;
                     ASN1: midambleConfiguration   Integer (1...8), */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[i].l1_sich_config.midamble_config_sich.config =
            tdsMidambleConfiguration[tmp_ptr->hS_SCCH_SetConfiguration.elem[i].hs_sich_configuration.midambleConfiguration];
       }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo.u.tdd choice is not tdd128");
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"modeSpecificInfo choice FDD, not TDD");
    return FAILURE;
  }
  return SUCCESS;

}


/*====================================================================
FUNCTION: tdsrrcllc_determine_hsdpa_action_r7()

DESCRIPTION:

  This function determines the HSDPA action and sets the parameters in L1
  structure accordingly.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  Sucess if HSDPA action and parameters have been set
  Failure Otherwise

SIDE EFFECTS:
  
====================================================================*/
uecomdef_status_e_type tdsrrcllc_determine_hsdpa_action_r7
(
  tdsrrc_DL_HSPDSCH_Information_r7* hspdsch_info_ptr
)
{
  uint8 reconfig_bit_mask = 0;
  
  boolean hsscch_codes_changed = FALSE;
  boolean hspdsch_midamble_changed = FALSE;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS 
  uint8 i = 0;
#endif

#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  multicarrier_flg = tdsrrcllc_multicarrier_present_per_version_r7(hspdsch_info_ptr);
#endif

  /* Save UE category info */
  if (tdsnv_hsdpa_category > 15)
  {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if (0 == tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue)
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
#endif
    }
    else
    {
      for(i=0;i< tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue;i++)
      {
        if ((tdsordered_config_ptr->dl_macehs_logchan_info[i].queue_id != TDSINVALID_QUEUE_ID) &&
          (tdsordered_config_ptr->mac_ehs_lc_id_in_use[i] == TRUE ))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 23;
          break;
        }
        else
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
          if (i == (tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue - 1))
          {
            tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
          }
#endif
        }
      }
    }
#else
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#endif
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = tdsnv_hsdpa_category;
  }

  /* HSDPA params have already been set in rrc_hsdpa_params.
  Check if HSDPA is already ACTIVE */
  if (tdsrrc_get_hsdpa_status() == TDSHSDPA_INACTIVE)
  {
    /* HSDPA is currently inactive */
    /* Check if network wants to activate HSDPA */
    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {
      if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
      {
        /* Check if hs scch and feedback info present */
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,
            tdsrrc_DL_HSPDSCH_Information_r7,hs_scch_Info))
        {
          if (FAILURE == tdsrrcllc_set_hsscch_info_r7(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                          , multicarrier_flg
#endif
                         ))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid HS-SCCH info");
            return FAILURE;
          }
        }

        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,
            tdsrrc_DL_HSPDSCH_Information_r7,
            measurement_feedback_Info)) 
        {
          /* Check Meas Feedback Info */
          if (FAILURE ==tdsrrcllc_set_meas_feedback_info_r7(
              &hspdsch_info_ptr->measurement_feedback_Info))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid Meas Feedback info");
            return FAILURE;
          }
        }

        if (TDSRRC_CHECK_MSG_TYPE(hspdsch_info_ptr->modeSpecificInfo,
            T_tdsrrc_DL_HSPDSCH_Information_r7_modeSpecificInfo_tdd))
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd,
            rrc_DL_HSPDSCH_Information_r7_tdd_tdd128))
          {
            /* Check if HS-PDSCH Midamble Configuration present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                hs_PDSCH_Midamble_Configuration_tdd128))
            {
              tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                  , multicarrier_flg
#endif
                );
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive midamble configuration tdd128");
            }
            /* Check if DL Multi-Carrier Information present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                dl_MultiCarrier_Information))
            {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
              if(FAILURE == tdsrrcllc_check_multicarrier_support_per_version(&hspdsch_info_ptr->
                  modeSpecificInfo.u.tdd->u.tdd128->dl_MultiCarrier_Information, NULL, FALSE))
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
                return FAILURE;
              }
#else
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport IE DL Multi-Carrier Information reveived");
              return FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
          return FAILURE;
        }

      }
    } /* End of if of hsdpa message params absent */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_reset_mac_ehs_params(tdsordered_config_ptr);
#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
      return SUCCESS;
    }
    /* Set Queue Info */
    if (tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set Queue Info");
      return FAILURE;
    }

    /* Final Check to make sure that everything is present */
    if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
    {
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_reset_mac_ehs_params(tdsordered_config_ptr);
#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
      return SUCCESS;
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTART");
    tdsrrc_set_hsdpa_action(TDSHSDPA_START);

    /* Set Bit Mask */
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = TDSL1_CPHY_HS_DSCH_CFG_INCL | 
      TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:HS Bit Mask is (Dec) %d (Hex) %x", tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask,
             tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);

    /* In case UTRAN messes up, set HS-RESET to FALSE */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Force HS-RESET to FALSE");
    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = FALSE;
    return SUCCESS;
    
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSDPA already active");
    /* HSDPA is already ACTIVE */
    
#ifdef FEATURE_TDSCDMA_CPC_DTX
    if(tdshsdpa_msg_params.rl_link_present == TRUE)
    {
      if(tdshsdpa_msg_params.new_rl_link != tdsordered_config_ptr->l1_hsdpa_info.hs_pdsch_rl_cpid)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCCPC:Serving HS-SCCH cell change,old cpid=%d new cpid=%d", 
          tdsordered_config_ptr->l1_hsdpa_info.hs_pdsch_rl_cpid, 
          tdshsdpa_msg_params.new_rl_link);
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCCPC:Set hs_scch_order_action as TDSL1_HS_SCCH_ORDER_ACTION_DISCARD"); 
        tdsordered_config_ptr->hs_scch_order_action = TDSL1_HS_SCCH_ORDER_ACTION_DISCARD;
      }
    }
#endif
    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {
      /* Set bit mask for HO */
      if ((tdsordered_config_ptr->is_hho== TRUE)
        ||(tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:hho, need reconfig dpa ");
        reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
      }
      if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
      {
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r7,hs_scch_Info))
        {
          /* Check if HS-SCCH Info has changed */
          if (tdsrrcllc_check_if_hsscch_changed_r7(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                                  , multicarrier_flg
#endif
            ) == TRUE)
          {
            hsscch_codes_changed = TRUE;
          }
          if (tdsrrcllc_set_hsscch_info_r7(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                      , multicarrier_flg
#endif
                                     ) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid HS-SCCH info");
            return FAILURE;
          }
          else
          {
            if (hsscch_codes_changed == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:SCCH Info changed");
              reconfig_bit_mask =reconfig_bit_mask | TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL;
            }
          }
        }
        
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r7,measurement_feedback_Info))
        {
          if (tdsrrcllc_set_meas_feedback_info_r7(&hspdsch_info_ptr->measurement_feedback_Info) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid Meas Feedback info");
            return FAILURE;
          }
        }

        
        if (TDSRRC_CHECK_MSG_TYPE(
            hspdsch_info_ptr->modeSpecificInfo,
            T_tdsrrc_DL_HSPDSCH_Information_r7_modeSpecificInfo_tdd))
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd,
            rrc_DL_HSPDSCH_Information_r7_tdd_tdd128))
          {
            /* Check if HS-PDSCH Midamble Configuration present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                hs_PDSCH_Midamble_Configuration_tdd128))
            {
              /* Check if HS-PDSCH Midamble Configuration has changed */
              if (tdsrrcllc_check_if_hspdsch_midamble_changed(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                  , multicarrier_flg
#endif
                ) == TRUE)
              {
                hspdsch_midamble_changed = TRUE;
              }
              tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                , multicarrier_flg
#endif
                );
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive midamble configuration tdd128");

              if (hspdsch_midamble_changed == TRUE)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:hspdsch midamble configuration changed");
                reconfig_bit_mask =reconfig_bit_mask | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;
              }
            }
            
            /* Check if DL Multi-Carrier Information present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                dl_MultiCarrier_Information))
            {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
              if(FAILURE == tdsrrcllc_check_multicarrier_support_per_version(&hspdsch_info_ptr->
                  modeSpecificInfo.u.tdd->u.tdd128->dl_MultiCarrier_Information, &reconfig_bit_mask, FALSE))
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
                return FAILURE;
              }
#else
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport IE DL Multi-Carrier Information reveived");
              return FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
          return FAILURE;
        }

      }
#ifdef FEATURE_TDSCDMA_64QAM
      if(tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured !=
               tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured)
      {
        tdshsdpa_msg_params.hsdsch_info_present = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set HS-DSCh bitmask as 64 QAM config is changed ");
      }
#endif

      /* Final Check to make sure that everything is present */
      if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
      {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTOP");
       tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);

        return SUCCESS;
      }


      /* Set the queue information */
      if (tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set HSDPA info in L1");
        return FAILURE;
      }
      else
      {
        if ((tdshsdpa_msg_params.hsdsch_info_present == TRUE) ||
            ((tdshsdpa_msg_params.hrnti_present == TRUE) && (tdshsdpa_msg_params.hrnti_changed == TRUE)))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:TDSL1_CPHY_HS_DSCH_CFG_INCL set");
          reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
        }
      }

      /* no matter what freq change */
      if ((reconfig_bit_mask == 0) && 
          (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE) &&
          (tdsordered_config_ptr->is_hho== FALSE)&&
          (tdsordered_config_ptr->w_freq == tdscurrent_config_ptr->w_freq))
      {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Change action RECONFIG to NOOP");
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
        tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
        return SUCCESS;
      }
      

      tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = reconfig_bit_mask;

      /* Set Action to RECONFIG */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:HSRECONFIG bit mask %d,HS-RESET %d",reconfig_bit_mask,
               tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
       tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
      
      return SUCCESS;
      
    } /* End of if of hsdpa msg params present */
    else
    {
      /* Final Check to make sure that everything is present */
      if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTOP");
         tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);

        return SUCCESS;
      }
      /* no matter what freq change, should reconfig HSDPA */
      if ((tdsordered_config_ptr->is_hho== TRUE)
        ||(tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq change,HS Action RECONFIG");
        tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
        return SUCCESS;
      }

      /* Means No Action */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSNOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);

      return SUCCESS;
    }
  }
}
#ifdef FEATURE_TDSCDMA_MIMO

/*============================================================================
FUNCTION: tdsrrcllc_check_if_mimo_can_be_started()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcllc_check_if_mimo_can_be_started
(
  tdsrrc_RRC_StateIndicator rrc_state
)
{
  uint32 i;
  tdshsdpa_action_enum_type hs_action =tdsrrc_get_hsdpa_action() ;
  /*
  The MIMO_STATUS variable shall be set to TRUE only when all the following conditions are met:
    "   The UE is in CELL_DCH state.
    "   The variable MIMO_PARAMS contains a value for the IE "MIMO N_cqi_typeA/M_cqi ratio". and
    "   The variable MIMO_PARAMS contains a value for the IE "MIMO pilot configuration".

  */

  
  if(( rrc_state != tdsrrc_RRC_StateIndicator_cell_DCH ) ||
     (tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio == TDSL1_MIMO_N_TO_M_RATIO_INVALID ) ||
     (tdsordered_config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern == TDSL1_MIMO_INVALID_PILOT_PATTERN)
     )
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Mimo status false, rrc state %d, N-M ratio %d, CIPH pattern %d ",rrc_state,
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio,
          tdsordered_config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern);
    return FALSE;
  }
 /*
   *  Section 8.6.6.41  MIMO parameters
   *  1> If the IE "Primary CPICH usage for Channel Estimation" is set to "FALSE":
   *       2>   the UE behavior is undefined.
   *  1> If the IE "S-CPICH Info" is included and if  the IE "Channelisation code" in MIMO pilot configuration 
   *        is different from  the IE "Channelisation Code" in Secondary CPICH info
   *        2>  the UE behavior is undefined.
   *
   */
  for(i=0;i< tdsordered_config_ptr->l1_dl_chan_parms.num_rl;i++)
  {
    if( tdsordered_config_ptr->l1_dl_chan_parms.dl_per_rl[i].dpch_per_rl.use_pri_cpich == TDSL1DLPHYCFG_DO_NOT_USE_PRI_CPICH)
    {
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"MIMO:MIMO cannot be started as Primary CIPCH for rl %d cant use Channel estimation",
           i);
       return FALSE;
    }

    if(tdsordered_config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern == TDSL1_MIMO_DIVERSITY_PATTERN)
    {
        if( tdsordered_config_ptr->l1_dl_chan_parms.dl_per_rl[i].dpch_per_rl.opt_parms_present_mask
        & DL_DPCH_INFO_PER_RL_SEC_CPICH_INFO_PRESENT)
        {           
          if( tdsordered_config_ptr->l1_dl_chan_parms.dl_per_rl[i].dpch_per_rl.sec_cpich_ch_code != 
           tdsordered_config_ptr->l1_hsdpa_info.mimo_info.channelisation_code)
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"MIMO:MIMO cannot be started as rl %d sec chann code %d is not same mimo chann code %d",
                i,tdsordered_config_ptr->l1_dl_chan_parms.dl_per_rl[i].dpch_per_rl.sec_cpich_ch_code,
                tdsordered_config_ptr->l1_hsdpa_info.mimo_info.channelisation_code);
            return FALSE;
          }
        }
    }
  }
  if((TDSCHECK_WCDMA_OPTIONAL_FEATURE_MIMO_SUPPORTED ) && 
       ((hs_action == TDSHSDPA_START) ||
         ((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE) && (hs_action != TDSHSDPA_STOP)) )
     )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:MIMO can be started");
    return TRUE;
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"MIMO:MIMO cannot be started HS Active %d HS Status %d MIMO supported %d",
           hs_action,tdsrrc_get_hsdpa_status(),TDSCHECK_WCDMA_OPTIONAL_FEATURE_MIMO_SUPPORTED);
    return FALSE;
    
  }
   
}


/*============================================================================
FUNCTION: tdsrrcllc_update_mimo_status()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_mimo_status
(
  tdsrrc_mimo_status_enum_type mimo_status
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO:Set MIMO Status to %d",mimo_status);
  tdsrrc_mimo_status = mimo_status;
}
/*============================================================================
FUNCTION: tdsrrcllc_init_mimo_params()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_init_mimo_params
(
  tdsordered_config_type *config_ptr
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIMO:Init MIMO Params");
  tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask &= ~TDSL1_CPHY_HS_MIMO_CFG_INCL;
  config_ptr->l1_hsdpa_info.mimo_info.mimo_n_m_ratio = TDSL1_MIMO_N_TO_M_RATIO_INVALID;
  config_ptr->l1_hsdpa_info.mimo_info.second_cpich_pattern = TDSL1_MIMO_INVALID_PILOT_PATTERN;
  config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action = TDSL1_MIMO_NOOP;
  tdsrrcllc_update_mimo_status(TDSRRC_MIMO_INACTIVE);

#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
  config_ptr->l1_hsdpa_info.mimo_info.s_cpich_po = tdsrrc_mimo_s_cpich_po;  
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/
  
#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
  config_ptr->l1_hsdpa_info.mimo_info.mimo_codebook_restriction_present= FALSE;  
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/
  
}
#endif /*FEATURE_TDSCDMA_MIMO*/



#ifdef FEATURE_TDSCDMA_CPC_DTX
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_dtx_req_mask_for_stop()

DESCRIPTION:
  This function updates the l1_cpc_dtx_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related UL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_dtx_req_mask_for_stop
(
  boolean dtx_info_present
)
{
  if (TRUE == dtx_info_present)
  {
    /* set the reqeest mask with stop, because DTX evaluation has set 
     * DTX-DRX-status = FALSE */

    tdsordered_config_ptr->l1_cpc_dtx_req_mask |= L1_CPC_DTX_STOP;
  }
}
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_dtx_req_mask_for_reconfig()

DESCRIPTION:
  This function updates the l1_cpc_dtx_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related UL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_dtx_req_mask_for_reconfig
(
  boolean dtx_info_present
)
{
  if (TRUE == dtx_info_present)
  {
    /* set the reqeest mask with stop, because DTX evaluation has set 
     * DTX-DRX-status = FALSE */

    tdsordered_config_ptr->l1_cpc_dtx_req_mask |= L1_CPC_DTX_RECONFIG;
  }
}
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_dtx_req_mask_for_start()

DESCRIPTION:
  This function updates the l1_cpc_dtx_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related UL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_dtx_req_mask_for_start
(
  boolean dtx_info_present
)
{
  if (TRUE == dtx_info_present)
  {
    /* set the reqeest mask with stop, because DRX evaluation has set 
     * DTX-DRX-status = FALSE */

    tdsordered_config_ptr->l1_cpc_dtx_req_mask |= L1_CPC_DTX_START;
  }
}
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_dtx_req_mask_for_no_op()

DESCRIPTION:
  This function updates the l1_cpc_dtx_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related UL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_dtx_req_mask_for_no_op
(
  boolean dtx_info_present
)
{
  if (TRUE == dtx_info_present)
  {
    /* set the reqeest mask with NO-OP*/

    tdsordered_config_ptr->l1_cpc_dtx_req_mask |= L1_CPC_DTX_NO_OP;
  }
}
#endif
#ifdef FEATURE_TDSCDMA_CPC_DRX
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_drx_req_mask_for_stop()

DESCRIPTION:
  This function updates the l1_cpc_drx_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related UL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_drx_req_mask_for_stop
(
  tdsordered_config_type *config_ptr,
  boolean drx_info_present
)
{
  if (TRUE == drx_info_present)
  {
    /* set the reqeest mask with stop, because DRX evaluation has set 
     * DTX-DRX-status = FALSE */

    config_ptr->l1_cpc_drx_req_mask |= L1_CPC_DRX_STOP;
  }
}
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_drx_req_mask_for_reconfig()

DESCRIPTION:
  This function updates the l1_cpc_drx_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related UL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_drx_req_mask_for_reconfig
(
  tdsordered_config_type *config_ptr,
  boolean drx_info_present
)
{
  if (TRUE == drx_info_present)
  {
    /* set the reqeest mask with stop, because DRX evaluation has set 
     * DTX-DRX-status = FALSE */

    config_ptr->l1_cpc_drx_req_mask |= L1_CPC_DRX_RECONFIG;
  }
}
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_drx_req_mask_for_start()

DESCRIPTION:
  This function updates the l1_cpc_drx_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related UL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_drx_req_mask_for_start
(
  tdsordered_config_type *config_ptr,
  boolean drx_info_present
)
{
  if (TRUE == drx_info_present)
  {
    /* set the reqeest mask with start, because DRX evaluation has set 
     * DTX-DRX-status = TRUE */

    config_ptr->l1_cpc_drx_req_mask |= L1_CPC_DRX_START;
  }
}
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_drx_req_mask_for_no_op()

DESCRIPTION:
  This function updates the l1_cpc_drx_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related UL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_drx_req_mask_for_no_op
(
 tdsordered_config_type *config_ptr,
  boolean drx_info_present
)
{
  if (TRUE == drx_info_present)
  {
    /* set the reqeest mask with NO-OP*/

    config_ptr->l1_cpc_drx_req_mask |= L1_CPC_DRX_NO_OP;
  }
}
/*============================================================================
FUNCTION: tdsrrcllc_get_l1_cpc_drx_status()

DESCRIPTION:
  This function retuens the l1_cpc_drx_status.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcllc_get_l1_cpc_drx_status
(
 tdsordered_config_type *config_ptr
)
{
  return (config_ptr->cpc_drx_status);
}
#endif


#ifdef FEATURE_TDSCDMA_HSUPA

/*============================================================================
FUNCTION: tdsrrcllc_update_l1_e_req_mask_for_e_ul_info()

DESCRIPTION:
  This function updates the e_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related UL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_e_req_mask_for_e_ul_info
(
  boolean e_ul_info_present
)
{
  if (TRUE == e_ul_info_present)
  {
    /* set the reqeest mask with reconfig, because EUL is already up */
    /* if E-DCH transmission is START or STOP then this request mask needs to be
       modified accordingly*/    
    tdsordered_config_ptr->l1_e_req_mask |= TDSRRCLLC_EUL_RECFG_UL;
  }
}

/*============================================================================
FUNCTION: tdsrrcllc_update_l1_e_req_mask_for_e_dl_info()

DESCRIPTION:
  This function updates the e_req_mask to be sent to layer1.
  This function is called whenever any of the EUL related DL physical layer 
  parameters are modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_l1_e_req_mask_for_e_dl_info
(
  boolean e_dl_info_present
)
{
  if (TRUE == e_dl_info_present)
  {
    /* set the reqeest mask with reconfig, because EUL is already up */
    /* if E-DCH transmission is START or STOP then this request mask needs to be
       modified accordingly*/
    tdsordered_config_ptr->l1_e_req_mask |= TDSRRCLLC_EUL_RECFG_DL;
  }
}

/*============================================================================
FUNCTION: tdsrrcllc_update_mac_eul_action()

DESCRIPTION:
  This function updates the mac_eul_action to be sent to MAC.
  This function is called whenever any of the EUL related parameters used by MAC 
  is modified by reconfig message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_mac_eul_action
(
  boolean mac_e_es_info_present
)
{
  if (TRUE == mac_e_es_info_present)
  {
    /* set the reqeest mask with reconfig, because EUL is already up */
    /* if E-DCH transmission is START or STOP then this action needs to be
       modified accordingly*/
    tdsordered_config_ptr->mac_eul_action = TDSMAC_EUL_CONFIG;
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_mac_ul_edch_info_ie()

DESCRIPTION:
  This function initializes 'mac_e_config.e_dch_info' variables (i.e. UL-EDCH
  physical layer parameters).
  This function should be called whenever UE leaves DCH state.  
DEPENDENCIES:


RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_mac_ul_edch_info_ie
(
  tdsordered_config_type *config_ptr
)
{
  config_ptr->mac_e_config.e_rucch_info.mmax = 0;;
  config_ptr->mac_e_config.e_rucch_info.nASC = 0;
  config_ptr->mac_e_config.e_rucch_info.t_rucch_tmr = 0;

  config_ptr->mac_e_config.eul_si_info.ernti_change = FALSE;
  config_ptr->mac_e_config.eul_si_info.ext_est_window = 0xFF;

  config_ptr->mac_e_config.eul_si_info.srv_cell_or_work_freq_change = FALSE;
  config_ptr->mac_e_config.eul_si_info.t_non_Sched_periodic_si_tmr = 0;
}

/*====================================================================
FUNCTION: tdsrrcllc_init_mac_e_info()

DESCRIPTION:
  This function initializes mac e info stored in rrc data-base
  
DEPENDENCIES:
  Function is only called before processing the DL OTA Msg.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_mac_e_info(tdsordered_config_type *config_ptr)
{
  uint32 count, idx;

  config_ptr->mac_eul_action = TDSMAC_EUL_NOOP;

  config_ptr->mac_e_reset_indicator = FALSE;

  config_ptr->mac_e_tsn_action = TDSMAC_E_TSN_ACTION_BACKUP;

  tdsrrcllc_init_mac_ul_edch_info_ie(config_ptr);

  config_ptr->mac_e_config.num_e_mac_d_flow = 0;

  for (count=0; count<TDSMAX_E_DCH_MAC_D_FLOW; count++)
  {
    config_ptr->mac_e_config.e_mac_d_flow_info[count].
      e_mac_d_flow_id = TDSINVALID_DFLOW_ID;

    config_ptr->mac_e_config.e_mac_d_flow_info[count].
      e_mac_d_flow_power_offset = 0xFF;

    config_ptr->mac_e_config.e_mac_d_flow_info[count].
      e_mac_d_flow_max_num_of_retx = 0xFF;

    config_ptr->mac_e_config.e_mac_d_flow_info[count].
      e_mac_d_flow_mux_list = 0xFF;

    config_ptr->mac_e_config.e_mac_d_flow_info[count].
      e_tx_grant_info = TDSMAC_E_TX_GRANT_NOT_DEFINED;

    config_ptr->mac_e_config.e_mac_d_flow_info[count].
      num_logch = 0;

    for (idx=0; idx < MAX_DED_LOGCHAN_PER_TRCH; idx++)
    {
      config_ptr->mac_e_config.e_mac_d_flow_info[count].
        e_ul_logch_info[idx].include_in_scheduling_info = FALSE;
      config_ptr->mac_e_config.e_mac_d_flow_info[count].
        e_ul_logch_info[idx].num_rlc_size_ddi_info = 0;
    }

    config_ptr->mac_e_config.e_mac_d_flow_info[count].
      non_sched_tx_grant.non_sched_tx_grant_harq_allocation_2ms = 0xFF;

    config_ptr->mac_e_config.e_mac_d_flow_info[count].
      non_sched_tx_grant.max_mac_e_pdu_size_in_bits = 0;
  }
}
/*====================================================================
FUNCTION: rrcllc_init_l1_e_ul_info_going_to_idle()

DESCRIPTION:
  This function initializes l1_e_ul_info variables.
  Some of the variables needs to be touched only if UE is in idle state or
  transitioning to idle state.

DEPENDENCIES:


RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_l1_e_ul_info
(
  boolean is_idle,
  tdsordered_config_type *config_ptr  
)
{
  uint8 count = 0;
  config_ptr->e_dch_transmission = FALSE;

  config_ptr->l1_e_info.e_ul_info.e_ul_info_mask = 0;
  config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.num_asc = 0;
  config_ptr->l1_e_info.e_ul_info.e_rucch_info.num_prach_fpach = 0;

  config_ptr->l1_e_info.e_ul_info.e_puch_info.num_epuch_ts = 0;
  for (count = 0; count < TDSL1_MAX_EPUCH_TS; count++)
  {
    config_ptr->l1_e_info.e_ul_info.e_puch_info.epuch_ts_cfg_list[count].
      midamble_info.shift = 0xFF;
  }
  config_ptr->l1_e_info.e_ul_info.e_puch_info.num_ref_e_tfci_16qam = 0;
  config_ptr->l1_e_info.e_ul_info.e_puch_info.num_ref_e_tfci_qpsk = 0;
 
  config_ptr->l1_e_info.e_ul_info.e_puch_info.snpl_rep_type = TDSL1_SNPL_REP_TYPE_NONE;

  if (is_idle == TRUE)
  {
    config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_NOT_PRESENT;
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_l1_e_dl_info()

DESCRIPTION:
  This function initializes l1_e_dl_info variables.
  Some of the variables needs to be touched only if UE is in idle state or
  transitioning to idle state.
  
DEPENDENCIES:


RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_l1_e_dl_info
(
  tdsordered_config_type *config_ptr  
)
{
  uint32 count;

  tdsordered_config_ptr->e_rnti_present = FALSE;
  config_ptr->e_dch_transmission = FALSE;
  
  config_ptr->l1_e_info.e_dl_info.e_dl_channel_mask = 0;
  config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.n_e_hich = 0;
  
  config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_agch_info.num_eagch = 0;
  for (count = 0; count < TDSL1_EDL_MAX_NUM_EAGCH; count++)
  {
    config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_agch_info.
      eagch_slot_info[count].eagch_midamble.shift = 0xFF;
  }

  config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.num_ehich = 0;
  for (count = 0; count < TDSL1_EDL_MAX_NUM_SCH_EHICH; count++)
  {
    config_ptr->l1_e_info.e_dl_info.e_dl_per_rl_info.e_hich_info.
      sched_e_hich_slot_info[count].ehich_midamble.shift = 0xFF;
  }
}
/*====================================================================
FUNCTION: tdsrrcllc_init_l1_non_sched_grant_info()

DESCRIPTION:
  This function initializes l1_non_sched_grant_info variables.
  Some of the variables needs to be touched only if UE is in idle state or
  transitioning to idle state.
  
DEPENDENCIES:


RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_l1_non_sched_grant_info
(
  tdsordered_config_type *config_ptr  
)
{

  config_ptr->l1_e_info.non_sched_grant_info.prri = 0;
  config_ptr->l1_e_info.non_sched_grant_info.crri = 0;
  config_ptr->l1_e_info.non_sched_grant_info.trri = 0;

  config_ptr->l1_e_info.non_sched_grant_info.num_e_ucch = 0;
  config_ptr->l1_e_info.non_sched_grant_info.rep_length = 0;
  config_ptr->l1_e_info.non_sched_grant_info.rep_period = 0;

  config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.
    chan_code = TDSL1_EDL_INVALID_CH_CODE;
  config_ptr->l1_e_info.non_sched_grant_info.non_sched_e_hich_info.
    mid_code.shift = 0xFF;

}

/*====================================================================
FUNCTION: tdsrrcllc_reset_mac_and_l1_e_info_on_llc_cfg_cnf()

DESCRIPTION:
  This function resets the MAC and L1 variables on receipt of LLC CONFIG CNF 
  by any procedure
  
DEPENDENCIES:
  None
  
RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_reset_mac_and_l1_e_info_on_llc_cfg_cnf(void)
{
  tdsl1_e_dl_info_struct_type *e_dl_info_ptr;
  tdsordered_config_type  *config_ptr;
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }
  e_dl_info_ptr = &config_ptr->l1_e_info.e_dl_info;


  config_ptr->l1_e_req_mask = TDSRRCLLC_EUL_NOOP;
  config_ptr->mac_eul_action = TDSMAC_EUL_NOOP;

  e_dl_info_ptr->e_dl_channel_mask = 0;
  config_ptr->l1_e_info.e_ul_info.e_ul_info_mask = 0;


  config_ptr->mac_e_reset_indicator = FALSE;

  config_ptr->mac_e_tsn_action = TDSMAC_E_TSN_ACTION_BACKUP;

}

/*====================================================================
FUNCTION: tdsrrcllc_init_e_dch_rbmapping_info()

DESCRIPTION:
  This function initializes E-DCH mac-d flow information.  To initialize
  all info, pass second parameter as true.  Else, pass index to be
  initialized

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:

====================================================================*/
void tdsrrcllc_init_e_dch_rbmapping_info
(
  tdsordered_config_type  *config_ptr,
  uint8  e_mac_d_flow_idx
)
{

  /* Initialize some of the mac-d flow values*/
  config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
    e_mac_d_flow_id = TDSINVALID_DFLOW_ID;
  config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
    num_logch = 0;
  config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
    e_mac_d_flow_max_num_of_retx = 0xFF;
  config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
    e_mac_d_flow_mux_list = 0xFF;
  config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
    non_sched_tx_grant.non_sched_tx_grant_harq_allocation_2ms = 0xFF;
  config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
    e_tx_grant_info = TDSMAC_E_TX_GRANT_NOT_DEFINED;
  config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
    non_sched_tx_grant.max_mac_e_pdu_size_in_bits = 0;

  if (config_ptr->mac_e_config.num_e_mac_d_flow > 0)
  {
    config_ptr->mac_e_config.num_e_mac_d_flow--;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: trying to delete e-mac-d-flow-id: %d when num_e_mac_d_flow is 0", 
      e_mac_d_flow_idx);
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCEUL: Init idx %d e_mac_d_flow info, updated num_e_mac_d_flow: %d",
    e_mac_d_flow_idx, config_ptr->mac_e_config.num_e_mac_d_flow);

}
/*====================================================================
FUNCTION: tdsrrcllc_update_e_dch_mapping_info_with_log_chl_id()

DESCRIPTION:
  This function updates the Uplink Mapping Info in EDCH RB Mapping
  Info with the passed Logical Channel ID.
  The correct entity is selected based on the passed RB ID.
  The EDCH RB mapping info in the passed Config data is updated.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_e_dch_mapping_info_with_log_chl_id
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_RB_Identity  rb_id,
  rlc_lc_id_type   rlc_ul_log_chl_id
)
{
  boolean rb_found = FALSE;
  int ix, iy;
  TDSRRC_MSG0(MSG_LEGACY_MED,"RRCEUL: Update edch mapping info with log chl id");
  for(ix = 0; (!rb_found) && (ix < config_ptr->mac_e_config.num_e_mac_d_flow); ix++)
  {
    if(config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_mac_d_flow_id != TDSINVALID_DFLOW_ID)
    {
      for(iy = 0; (!rb_found) && (iy < config_ptr->mac_e_config.e_mac_d_flow_info[ix].num_logch); iy++)
      {
        if (config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_ul_logch_info[iy].rb_id == rb_id) 
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCEUL: rb_id %d lc_id %d", rb_id, rlc_ul_log_chl_id);
          rb_found = TRUE;
          config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_ul_logch_info[iy].rlc_id = rlc_ul_log_chl_id;
          break;
        }
      }
    }
  }
}
/*====================================================================
FUNCTION: tdsrrcllc_get_e_mac_dflow_idx()

DESCRIPTION:
  This function gets a EDCH mac-d flow Index for the specified mac-d flow. 
  If the the mac-d flow already existed, its index is returned. Otherwise a 
  new index is allocated for the mac-d ID. If a new mac-d flowindex is allocated,
  e_mac_d_flow count in MAC structure is updated.

DEPENDENCIES:
  None.

RETURN VALUE:
  Index value if successful else
  UE_MAX_MAC_D_FLOW if no index is available

SIDE EFFECTS:

====================================================================*/
uint8 tdsrrcllc_get_e_mac_dflow_idx
(
  uint8  e_mac_d_flow_id
)
{
  uint8 index = TDSMAX_E_DCH_MAC_D_FLOW;

  for(index = 0; index < tdsordered_config_ptr->mac_e_config.num_e_mac_d_flow; index++)
  {
    if(tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_mac_d_flow_id == e_mac_d_flow_id)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCEUL:DFLOWidx %d, Id %d",index, e_mac_d_flow_id);
      return(index);
    }
  }

  /* E-MAC-d flow index not found, Allocate new */
  index = tdsrrcllc_allocate_e_mac_d_flow_index();
  if(index != TDSMAX_E_DCH_MAC_D_FLOW)
  {  
    /* Now Save the MAC-d flow ID in MAC structure */
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_mac_d_flow_id = e_mac_d_flow_id;
    
    /* Increment no of DFLOWs */
    tdsordered_config_ptr->mac_e_config.num_e_mac_d_flow++;
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL:New e-dch dflow cnt %d in MAC struct",tdsordered_config_ptr->mac_e_config.num_e_mac_d_flow );  
  } /* A valid new DFLOW index allocated */

  return(index);   
} /* tdsrrcllc_get_dflow_index */

/*====================================================================
FUNCTION: tdsrrcllc_find_e_mac_d_flow_idx()

DESCRIPTION:
  This function finds the edch mac-d flow index for the E-MAC-d FLOW. 
  If not found, it allocates a new index and return to the index number.

DEPENDENCIES:
  None.

RETURN VALUE:
  E-MAC-d FLOW Index if found
  Else UE_MAX_MAC_D_FLOW.

SIDE EFFECTS:
====================================================================*/
uint8  tdsrrcllc_find_e_mac_d_flow_idx
(
  uint8  e_mac_dflow_id
)
{
  uint8 index = TDSMAX_E_DCH_MAC_D_FLOW;

  for(index = 0; index < TDSMAX_E_DCH_MAC_D_FLOW; index++)
  {
    if(tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_mac_d_flow_id == e_mac_dflow_id)
    {
      TDSRRC_MSG2(MSG_LEGACY_MED,"RRCEUL:E_MAC_D FLOW idx %d, Id %d",index, e_mac_dflow_id);
      break;
    }
  }
  return(index);
}
/*====================================================================
FUNCTION: rrcllc_allocate_e_mac_d_flow_idx()

DESCRIPTION:
  Allocates a EDCH MAC-d flow Id.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A EDCH MAC-d flow Index.
  UE_MAX_MAC_D_FLOW -> Given EDCH Mac-d flow does not
           have an allocated DCH transport channel index.

SIDE EFFECTS:
====================================================================*/
uint8  tdsrrcllc_allocate_e_mac_d_flow_index(void)
{
  uint8 cnt = TDSMAX_E_DCH_MAC_D_FLOW;
  /* An index with the same EDCH Mac-d FLOW ID is NOT in use.
  * Allocate a new index */
  for (cnt =0; cnt < TDSMAX_E_DCH_MAC_D_FLOW; cnt++)
  {
    if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[cnt].e_mac_d_flow_id == TDSINVALID_DFLOW_ID)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL:Assign Index %d for new EDCH Mac-D flow", cnt);    
      break;
    }
  }
  return cnt;
}

/*====================================================================
FUNCTION: tdsrrcllc_fill_holes_in_e_mac_d_flow_idx()

DESCRIPTION:
  This function deletes any holes in the MAC edch mac-d flow structure.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_fill_holes_in_e_mac_d_flow_idx
(
  void
)
{
  uint8  ix, iy;

  if (tdsordered_config_ptr->mac_e_config.num_e_mac_d_flow == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL:Num EDCH MAC-d flow is 0");
    return;
  }

  /* Put all the values in tmp structure. No there are no holes in tmp structure */
  for(ix = TDSMAX_E_DCH_MAC_D_FLOW; ix > 0; ix--)
  {
    /* Check if valid data */
    if(tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[ix-1].e_mac_d_flow_id != TDSINVALID_DFLOW_ID)
    {
      for(iy = 0; iy < (ix - 1); iy++)
      {
        /* Check if valid data */
        if(tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[iy].e_mac_d_flow_id == TDSINVALID_DFLOW_ID)
        {
          memscpy(&tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[iy], sizeof(tdsmac_e_mac_d_flow_info_s_type),
                 &tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[ix-1], 
                 sizeof(tdsmac_e_mac_d_flow_info_s_type));
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[ix-1].e_mac_d_flow_id = TDSINVALID_DFLOW_ID;
          /*Set num_logch for this index as Zero*/
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[ix-1].num_logch = 0;
          break;
        }
      }
      if (iy == (ix-1)) 
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: Rearranging e_mac d flow complete");
        break;
      }
    }
  }
}
/*====================================================================
FUNCTION: tdsrrcllc_get_e_dch_log_chan_mode()

DESCRIPTION:
  This function gets a the log_chan_mode for the rb id specified

DEPENDENCIES:
  None.

RETURN VALUE:
  Index value if successful else
  UE_MAX_MAC_D_FLOW if no index is available

SIDE EFFECTS:

====================================================================*/
uecomdef_logch_mode_e_type tdsrrcllc_get_e_dch_log_chan_mode(uint32 rb_id)
{
  int ix, iy;
  for(ix = 0; (ix < tdsordered_config_ptr->mac_e_config.num_e_mac_d_flow); ix++)
  {
    if(tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_mac_d_flow_id != TDSINVALID_DFLOW_ID)
    {
      for(iy = 0; (iy < tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[ix].num_logch); iy++)
      {
        if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_ul_logch_info[iy].rb_id == rb_id) 
        {
          return (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_ul_logch_info[iy]).rlc_mode;
        }
      }
    }
  }
  return UE_MODE_MAX_NUM;
}

#endif /*FEATURE_TDSCDMA_HSUPA*/

/*============================================================================
FUNCTION: tdsrrcllc_reset_mac_and_l1_for_hsdpa()

DESCRIPTION:
  This funtion resets MAC-HS actions. It also resets the hs_req_mask for L1.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
=============================================================================*/
void tdsrrcllc_reset_mac_and_l1_for_hsdpa
(
  void
)
{
  tdsordered_config_type  *config_ptr;
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Reset MAC-HS action %d and L1 hs_req_mask %d",config_ptr->mac_dl_parms.mac_hsdpa_action,
    config_ptr->l1_hsdpa_info.hs_req_mask);

  config_ptr->l1_hsdpa_info.hs_req_mask = 0;
  config_ptr->mac_dl_parms.mac_hsdpa_action = TDSHSDPA_NOOP;
  /*Reset the reset indicator since configuration to MAC is done*/
  config_ptr->mac_dl_parms.mac_hs_reset_indicator = FALSE;

#if defined (FEATURE_TDSCDMA_HSDPA_MAC_EHS)
  config_ptr->mac_dl_parms.mac_hs_transition_type = TDSMAC_HS_NO_TRANSITION;
#endif

  return;
}
/*====================================================================
FUNCTION: tdsrrcllc_init_hsdpa_mac_info()

DESCRIPTION:
  This function initializes the HSDPA Information in the configuration
  database.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_hsdpa_mac_info(tdsordered_config_type  *config_ptr)
{
  uint8 idx = 0, idy = 0, idz = 0;
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Init HSDPA MAC Info");
  }
  config_ptr->mac_dl_parms.mac_hsdpa_action = TDSHSDPA_NOOP;
  config_ptr->mac_dl_parms.num_dflow = 0;
  config_ptr->mac_dl_parms.mac_hs_reset_indicator = FALSE;

  config_ptr->mac_dl_parms.mac_hs_restore_tsns = FALSE;

  for(idx = 0; idx < UE_MAX_MAC_D_FLOW; idx++)
  {
    config_ptr->mac_dflow_info[idx].no_of_queues = 0;
    config_ptr->mac_dflow_info[idx].ndlchan = 0;


    config_ptr->dl_dflow_index_in_use[idx] = FALSE;
    config_ptr->mac_dflow_info[idx].mac_dflow_id = TDSINVALID_DFLOW_ID;
    tdsrrcllc_semi_permanent_data.mac_dflow_info[idx].mac_dflow_id = TDSINVALID_DFLOW_ID;

    for(idz = 0; idz < UE_MAX_LOGCHAN_PER_DFLOW; idz++)
    {
      config_ptr->mac_dflow_info[idx].dlchan_info[idz].mac_id = 0;
      config_ptr->mac_dflow_info[idx].dlchan_info[idz].rb_id = 0;
      config_ptr->mac_dflow_info[idx].dlchan_info[idz].rlc_id = 0;
    }


    for(idy = 0; idy < UE_MAX_HS_QUEUE; idy++)
    {
      config_ptr->mac_dflow_info[idx].queue_info[idy].queue_id = TDSINVALID_QUEUE_ID;
      config_ptr->mac_dflow_info[idx].queue_info[idy].no_of_pdu_sizes = 0;
      for(idz = 0; idz < UE_MAX_HS_NO_OF_PDU_SIZE_PER_QUEUE; idz++)
      {
        config_ptr->mac_dflow_info[idx].queue_info[idy].pdu_info[idz].mac_pdu_index = TDSINVALID_PDU_INDEX_ID;
      }
      config_ptr->mac_dflow_info[idx].queue_info[idy].release_timer = 0;
      config_ptr->mac_dflow_info[idx].queue_info[idy].window_size = 0;
    }
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_hsdpa_l1_queue()

DESCRIPTION:
  This function initializes the HSDPA Information in the configuration
  database.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_hsdpa_l1_queue
(
  tdsordered_config_type  *config_ptr
)
{
  uint8 idx = 0, idy = 0;

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Init HSDPA L1 Queue");
  }
  
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  for(idx = 0; idx < TDSL1_DL_CCTRCHCFG_MAX_LOGICAL_CHANNEL; idx++)
  {
    config_ptr->l1_hsdpa_info.hs_dsch_info.mac_ehs_dlchan_q_info[idx] = TDSINVALID_QUEUE_ID;
  }
#endif

  config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.num_mac_hs_q = 0;

  for(idx = 0; idx < UE_MAX_HS_QUEUE; idx++)
  {
    config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.mac_hs_q_info[idx].mac_hs_q_id = TDSINVALID_QUEUE_ID;
    config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.mac_hs_q_info[idx].num_mac_d_pdu_sz = 0;
    for(idy = 0; idy < TDSL1_DL_MACD_PDU_SZ_PER_Q_MAX_COUNT; idy++)
    {
      config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.mac_hs_q_info[idx].mac_d_pdu_sz_info[idy].pdu_sz = 0;
      config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.mac_hs_q_info[idx].mac_d_pdu_sz_info[idy].pdu_sz_idx = 0;
    }
  }
}


/*====================================================================
FUNCTION: tdsrrcllc_init_hsdpa_msg_params_struct()

DESCRIPTION:
  This function initializes the HSDPA Message Parameter structure

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_hsdpa_msg_params_struct
(
  void
)
{
  tdshsdpa_msg_params.msg_params_present = FALSE;
  tdshsdpa_msg_params.hrnti_present = FALSE;
  tdshsdpa_msg_params.hspdsch_info_present = FALSE;
  tdshsdpa_msg_params.hsdsch_info_present = FALSE;

  tdshsdpa_msg_params.hrnti_changed = FALSE;

  tdshsdpa_msg_params.mac_hs_restore_tsns = FALSE;

}


/*====================================================================
FUNCTION: tdsrrcllc_init_hsdpa_info_before_msg_processing()

DESCRIPTION:
  This function initializes the HSDPA Action in MAC structure, forces
  the MAC Reset variable to FALSE. Also, makes sure that the HSDPA
  Action variable is set to NOOP and also initializes the HSDPA
  message param structure and rlc size per rb structure

DEPENDENCIES:
  Function is only called before processing the DL OTA Msg.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_hsdpa_info_before_msg_processing
(
  void
)
{
  /* Initialize HSDPA action forcibly to NOOP */
  tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
  tdsrrcllc_init_hsdpa_msg_params_struct();
  tdsrrc_initialize_rlc_size_per_rb();

  /* Set MAC Action variable to NOOP */
  tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = TDSHSDPA_NOOP;

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  tdsordered_config_ptr->mac_dl_parms.mac_hs_transition_type = TDSMAC_HS_NO_TRANSITION;
#endif
  
  tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = FALSE;

}


/*====================================================================
FUNCTION: tdsrrcllc_init_hsdpa_l1_info()

DESCRIPTION:
  This function initializes the HSDPA Information in the configuration
  database.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllc_init_hsdpa_l1_info(tdsordered_config_type  *config_ptr)
{
  uint8 cnt = 0;
  config_ptr->l1_hsdpa_info.hs_req_mask = 0;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  config_ptr->l1_hsdpa_info.hs_dsch_info.mac_hs_or_ehs = MAC_HS_TYPE_INVALID;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Reset to use the Bit aligned TFRI table ");
  tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hsdsch_tb_size_alignment_type = 
               TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_UNALIGNED;
#endif
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Init HSDPA L1 Info");
  }

  config_ptr->l1_hsdpa_info.hs_scchset_info.tpc_ssize = TDSL1_TPC_STEP_SIZE_0DB;
  config_ptr->l1_hsdpa_info.hs_scchset_info.num_hs_scch_sich = 0;
  for (cnt = 0; cnt < TDS_MAX_HS_SCCH_SICH_COUNT; cnt++)
  {
    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].ts_num_scch = 0;
    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].
        scch_chcode_1st = TDSL1_DL_INVALID_CH_CODE;
    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].
        scch_chcode_2nd = TDSL1_DL_INVALID_CH_CODE;
    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].
        hs_scch_midamble_cfg.config = TDSL1_NUM_MIDAMBLE_CONFIG;
    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].
        hs_scch_midamble_cfg.shift = 0xFF;

    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].
        l1_sich_config.midamble_config_sich.config = TDSL1_NUM_MIDAMBLE_CONFIG;
    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].
        l1_sich_config.midamble_config_sich.shift = 0xFF;
    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].
        l1_sich_config.midamble_config_sich.config = TDSL1_NUM_MIDAMBLE_CONFIG;
    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].
        l1_sich_config.midamble_config_sich.shift = 0xFF;
    config_ptr->l1_hsdpa_info.hs_scchset_info.l1_dl_hs_scch_sich_config[cnt].
        l1_sich_config.tpc_ssize = TDSL1_TPC_STEP_SIZE_0DB;
  }

  config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_part_info_valid = FALSE;
  config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc = 0;
#ifdef FEATURE_TDSCDMA_MIMO
  config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mimo_additional_mem_part_info_valid = FALSE;
#endif
  config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.num_mac_hs_q = 0;

  config_ptr->l1_hsdpa_info.hs_req_mask = 0;

#ifdef FEATURE_TDSCDMA_64QAM
  config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
#endif

  config_ptr->l1_hsdpa_info.hs_pdsch_midamble_cfg.config = TDSL1_NUM_MIDAMBLE_CONFIG;
  config_ptr->l1_hsdpa_info.hs_pdsch_midamble_cfg.shift = 0xff;

  tdsrrcllc_init_hsdpa_l1_queue(config_ptr);
}



/*====================================================================
FUNCTION: tdsrrcllc_init_before_msg_processing()

DESCRIPTION:
  This function initializes global variables which need to be initialized before message
  processing.

DEPENDENCIES:
  Function is only called before processing the DL OTA Msg.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_before_msg_processing (void)
{


  tdsordered_config_ptr->is_hho = FALSE;
  tdsordered_config_ptr->p_freq_present = FALSE;
  tdsordered_config_ptr->s_freq_present = FALSE;
  tdsordered_config_ptr->cpid_present = FALSE;
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  tdsordered_config_ptr->harq_present = FALSE;
#endif

  tdsrrcllc_init_tfcs_needed_flags();

  /* Initialize HSDPA parameters */
  tdsrrcllc_init_hsdpa_info_before_msg_processing();

  tdsrrc_initialize_rlc_size_per_rb();

#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrcllc_init_e_dch_info_before_msg_processing(tdsordered_config_ptr);
#endif  /*FEATURE_TDSCDMA_HSUPA*/

  /*Intialize the global variable tdsrrc_rb_list_mapped_to_dch */
  tdsrrcllc_init_rb_list_mapped_to_dch();


  tdsrrc_initialize_rlc_size_change();

#ifdef FEATURE_TDSCDMA_CPC_DTX
  tdsrrcllc_init_cpc_dtx_info_before_msg_processing(tdsordered_config_ptr);
#endif/*FEATURE_TDSCDMA_CPC_DTX*/

#ifdef FEATURE_TDSCDMA_CPC_DRX
  tdsrrcllc_init_cpc_drx_info_before_msg_processing(tdsordered_config_ptr);
#endif

  /* Initialize the L1 bitmask for the parameters in the "Downlink
   * information for all radio links" IE.  Since the IE is optional,
   * we want to make sure we don't signal L1 erroneously that we have
   * received information when we haven't.
   */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Init dl common opt parm present bit mask"); 
  tdsordered_config_ptr->l1_dl_chan_parms.dpch.opt_parm_presence_bitmask = 0;
}

/*====================================================================
FUNCTION: tdsrrcllc_init_rb_list_mapped_to_dch()

DESCRIPTION:
  This function initializes global variable tdsrrc_rb_list_mapped_to_dch

DEPENDENCIES:
  None
  
RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_rb_list_mapped_to_dch
(
  void
)
{
  uint8 rb_idx=0;
  
  tdsrrc_rb_list_mapped_to_dch.num_of_RBs = 0;
  
  for(rb_idx = 0; rb_idx < TDS_MAX_RB; rb_idx++)
  {
    tdsrrc_rb_list_mapped_to_dch.rrc_rb_mapped_to_dch[rb_idx] = TDSRRC_INVALID_RB_ID;
  }
}

#ifdef FEATURE_UMTS_PDCP
/*====================================================================
FUNCTION: tdsrrcllc_print_pdcp_info()

DESCRIPTION:
  This function prints the PDCP information in the ordered config pointer

DEPENDENCIES:
  None.
RETURN VALUE:
  None
SIDE EFFECTS:
  None
====================================================================*/
void tdsrrcllc_print_pdcp_info(tdsordered_config_type *config_ptr)
{
  uint32 x;

  TDSRRC_MSG1(MSG_LEGACY_HIGH," Num RBs with PDCP: %d", config_ptr->pdcp_parms.num_rbs);

  for (x = 0; x < config_ptr->pdcp_parms.num_rbs; x++) 
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"PDCP RB_ID:%d, in use: %d, info_changed %d",
    config_ptr->pdcp_parms.pdcp_config_req[x].rb_id,
    config_ptr->pdcp_parms.pdcp_config_req[x].in_use,
    config_ptr->pdcp_parms.pdcp_config_req[x].pdcp_info_changed);
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"PDCP action:%d, rb_id: %d, srns_reloc %d",
             config_ptr->pdcp_parms.pdcp_config_req[x].pdcp_cfg_req.action,
             config_ptr->pdcp_parms.pdcp_config_req[x].pdcp_cfg_req.rb_id,
             config_ptr->pdcp_parms.pdcp_config_req[x].pdcp_cfg_req.srns_reloc_flag);
  }
}
#endif /* FEATURE_UMTS_PDCP*/

/*====================================================================
FUNCTION: tdsrrcllc_print_mac_info()

DESCRIPTION:
  This function prints the MAC information in the ordered config pointer

DEPENDENCIES:
  None.
RETURN VALUE:
  None
SIDE EFFECTS:
  None
====================================================================*/
void tdsrrcllc_print_mac_info(tdsordered_config_type *config_ptr)
{
  int ix,iy;

  TDSRRC_MSG1(MSG_LEGACY_MED,"MAC DL nbch:%d", config_ptr->mac_dl_parms.num_bchs);
  for(ix = 0; ix < TDSMAC_DL_MAX_CCTRCH; ix++)
  {
    TDSRRC_MSG2(MSG_LEGACY_MED,"MAC BCH bcch_id:%d,cctrch_id:%d",
              config_ptr->mac_dl_parms.bch_info[ix].bcch_id,
              config_ptr->mac_dl_parms.bch_info[ix].cctrch_id);
  }

  TDSRRC_MSG1(MSG_LEGACY_MED,"MAC DL npch:%d", config_ptr->mac_dl_parms.num_pchs);
  for(ix = 0; ix < TDSMAC_DL_MAX_CCTRCH; ix++)
  {
    TDSRRC_MSG2(MSG_LEGACY_MED,"MAC PCH pcch_id:%d,cctrch_id:%d",
              config_ptr->mac_dl_parms.pch_info[ix].pcch_id,
              config_ptr->mac_dl_parms.pch_info[ix].cctrch_id);
  }
  TDSRRC_MSG1(MSG_LEGACY_MED,"MAC DL nfach:%d", config_ptr->mac_dl_parms.num_fachs);
  for(ix = 0; ix < TDSUE_MAX_TRCH; ix++)
  {
    TDSRRC_MSG2(MSG_LEGACY_MED,"MAC FACH bcch_en:%d,bcch_id:%d",
              config_ptr->mac_dl_parms.fach_info[ix]->bcch_enable,
              config_ptr->mac_dl_parms.fach_info[ix]->bcch_id);
    TDSRRC_MSG3(MSG_LEGACY_MED,"MAC FACH ccch_en:%d,ccch_id:%d,ndlchan:%d",
              config_ptr->mac_dl_parms.fach_info[ix]->ccch_enable,
              config_ptr->mac_dl_parms.fach_info[ix]->ccch_id,
              config_ptr->mac_dl_parms.fach_info[ix]->ndlchan
              );
  }

  TDSRRC_MSG1(MSG_LEGACY_MED,"MAC DL ndch:%d", config_ptr->mac_dl_parms.num_dchs);
  for(ix = 0; ix < TDSUE_MAX_TRCH; ix++)
  {
    TDSRRC_MSG3(MSG_LEGACY_MED,"MAC DCH trch_id:%d,cctrch_id:%d,ndlchan:%d",
              config_ptr->mac_dl_parms.dch_info[ix]->trch_id,
              config_ptr->mac_dl_parms.dch_info[ix]->cctrch_id,
              config_ptr->mac_dl_parms.dch_info[ix]->ndlchan
              );
    for (iy=0; iy<config_ptr->mac_dl_parms.dch_info[ix]->ndlchan; iy++)
    {
      TDSRRC_MSG3(MSG_LEGACY_MED,"DCH dlchan mac_id:%d,rb_id:%d,rlc_id:%d",
                config_ptr->mac_dl_parms.dch_info[ix]->dlchan_info[iy].mac_id,
                config_ptr->mac_dl_parms.dch_info[ix]->dlchan_info[iy].rb_id,
                config_ptr->mac_dl_parms.dch_info[ix]->dlchan_info[iy].rlc_id
                );
    }
  }
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
  {
    TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:Num HS-DSCH:%d", config_ptr->mac_dl_parms.num_dflow);

    for(ix = 0; ix < config_ptr->mac_dl_parms.num_dflow; ix++)
    {
      TDSRRC_MSG3(MSG_LEGACY_MED,"Dflow Id:%d,cctrch_id:%d,ndlchan:%d",
               config_ptr->mac_dl_parms.hsdpa_info[ix]->mac_dflow_id,
               config_ptr->mac_dl_parms.hsdpa_info[ix]->cctrch_id,
               config_ptr->mac_dl_parms.hsdpa_info[ix]->ndlchan
               );
      for (iy=0; iy<config_ptr->mac_dl_parms.hsdpa_info[ix]->ndlchan; iy++)
      {
        TDSRRC_MSG3(MSG_LEGACY_MED,"MAC Id:%d,rb_id:%d,rlc_id:%d",
                 config_ptr->mac_dl_parms.hsdpa_info[ix]->dlchan_info[iy].mac_id,
                 config_ptr->mac_dl_parms.hsdpa_info[ix]->dlchan_info[iy].rb_id,
                 config_ptr->mac_dl_parms.hsdpa_info[ix]->dlchan_info[iy].rlc_id
                 );
      }
    }
  }
  TDSRRC_MSG3(MSG_LEGACY_MED,"MAC RNTI info type:%d crnti:0x%x,urnti:0x%x",
            config_ptr->mac_dl_parms.rnti_info.rnti_valid,
            config_ptr->mac_dl_parms.rnti_info.crnti,
            config_ptr->mac_dl_parms.rnti_info.urnti
            );

  TDSRRC_MSG3(MSG_LEGACY_MED,"MAC UL cctrch_type:%d, cfg_chg:%d, wf1sig:%d",
           config_ptr->mac_ul_parms.cctrch_ptr->cctrch_type,
           config_ptr->mac_ul_parms.mac_config_changed,
           config_ptr->mac_ul_parms.wait_for_l1_sig
           );
  TDSRRC_MSG3(MSG_LEGACY_MED,"MAC RACH ccch_en:%d,ccch_id:%d,trch_id:%d",
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_enable,
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_id,
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.trch_id
            );
  TDSRRC_MSG2(MSG_LEGACY_MED,"MAC RACH ccch_asc:%d,ndlchan:%d",
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_asc,
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ndlchan);
  for(ix = 0; ix < config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ndlchan; ix++)
  {
    TDSRRC_MSG3(MSG_LEGACY_MED,"RACH ndlch rb_id:%d,rlc_id:%d,rlc_mode:%d",
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].rb_id,
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].rlc_id,
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].rlc_mode
            );
    TDSRRC_MSG3(MSG_LEGACY_MED,"RACH ndlch mac_id:%d,pri:%d,ch_type:%d",
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].mac_id,
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].priority,
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].chan_type
            );
    TDSRRC_MSG2(MSG_LEGACY_MED,"RACH ndlch all_tfi_valid:%d,n_tfi:%d",
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].rlc_size_info.all_tfi_valid,
            config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].rlc_size_info.ntfi_per_lc
            );
    TDSRRC_MSG3(MSG_LEGACY_MED," RACH dlchan tfi0:%d,tfi1:%d,tfi2:%d",
             config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].rlc_size_info.lc_tfi_list[0],
             config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].rlc_size_info.lc_tfi_list[1],
             config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[ix].rlc_size_info.lc_tfi_list[2]
            );
  }
  /* mac dch info */
  TDSRRC_MSG1(MSG_LEGACY_MED,"MAC DCH ndchs:%d",
            config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs
            );

  for(ix=0; ix<config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs; ix++)
  {
    TDSRRC_MSG3(MSG_LEGACY_MED,"RACH ndlch rb_id:%d,rlc_id:%d,rlc_mode:%d",
            config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].cctrch_id,
            config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].trch_id,
            config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].ndlchan
            );

    for (iy=0; iy<config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].ndlchan; iy++)
    {
      TDSRRC_MSG3(MSG_LEGACY_MED,"UL DCH dlchan chan_type:%d,mac_id:%d,pri:%d",
                config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].chan_type,
                config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].mac_id,
                config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].priority
                );
      TDSRRC_MSG3(MSG_LEGACY_MED,"UL DCH dlchan rb_id:%d,rlc_id:%d,rlc_mode:%d",
                config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].rb_id,
                config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].rlc_id,
                config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].rlc_mode
                );
      TDSRRC_MSG2(MSG_LEGACY_MED,"UL DCH dlchan all_tfi_val:%d,ntfi:%d",
                config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].rlc_size_info.all_tfi_valid,
                config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].rlc_size_info.ntfi_per_lc
              );
      TDSRRC_MSG3(MSG_LEGACY_MED," UL DCH dlchan tfi0:%d,tfi1:%d,tfi2:%d",
               config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].rlc_size_info.lc_tfi_list[0],
               config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].rlc_size_info.lc_tfi_list[1],
               config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[ix].dlchan_info[iy].rlc_size_info.lc_tfi_list[2]
              );
    }
  }

}
/*====================================================================
FUNCTION: tdsrrcllc_print_l1_info()
DESCRIPTION:
  This function prints the L1 information in the ordered config pointer
DEPENDENCIES:
  None.
RETURN VALUE:
  None
SIDE EFFECTS:
  None
====================================================================*/
void tdsrrcllc_print_l1_info(tdsordered_config_type *config_ptr)
{
 int ix, iy;
  TDSRRC_MSG2(MSG_LEGACY_MED,"L1UL CCTRCH  chan:%d,ntfci:%d",
           config_ptr->l1_ul_cctrch_parms.chan,
           config_ptr->ul_tfcs.ntfc
           );
#if 0
  for (ix=0;ix<config_ptr->l1_ul_cctrch_parms.num_trch; ix++)
  {
    TDSRRC_MSG3(MSG_LEGACY_MED,"Semist info  chan:%d,ntrch:%d,ntfci:%d",
             config_ptr->l1_ul_cctrch_parms.semi_static_parms_ptr[ix]->coding,
             config_ptr->l1_ul_cctrch_parms.semi_static_parms_ptr[ix]->trch_id,
             config_ptr->l1_ul_cctrch_parms.semi_static_parms_ptr[ix]->tti
             );
  }
#endif
  TDSRRC_MSG3(MSG_LEGACY_MED,"L1 UL DPCH ul_sync_step_size:%d,ul_sync_freq:%d,ul_sync_parm_present:%d",
           config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size,
           config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq,
           config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present
          );

  TDSRRC_MSG3(MSG_LEGACY_MED,"L1 UL DPCH prx_dpch_des:%d,act_time:%d,tfci_coding:%d",
           config_ptr->l1_ul_chan_parms.dpch_cfg.prx_dpch_des,
           config_ptr->l1_ul_chan_parms.dpch_cfg.act_time,
           config_ptr->l1_ul_chan_parms.dpch_cfg.common_ts_info.tfci_coding
          );

  TDSRRC_MSG3(MSG_LEGACY_MED,"L1 UL DPCH tpc_step_size:%d,pccpch_tx_pwr:%d, beacon_pl_est:%d",
           config_ptr->l1_ul_chan_parms.dpch_cfg.tpc_step_size,
           config_ptr->l1_ul_chan_parms.dpch_cfg.pccpch_tx_pwr,
           config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est
          );

   TDSRRC_MSG1(MSG_LEGACY_MED,"L1 PRACH pccpch_tx_pwr:%d",
              config_ptr->l1_ul_chan_parms.prach_cfg.pccpch_tx_pwr
             );
  TDSRRC_MSG1(MSG_LEGACY_MED,"L1 PRACH num_asc:%d",
           config_ptr->l1_ul_chan_parms.prach_cfg.asc_cfg.num_asc
          );
  /*downlink L1 parms */
  TDSRRC_MSG3(MSG_LEGACY_MED,"L1DL CCTRCH  cctrch_id:%d,ntrch:%d,trchbf:0x%x",
           config_ptr->l1_dl_cctrch_parms.cctrch_id,
           config_ptr->l1_dl_cctrch_parms.num_trch,
           (config_ptr->l1_dl_cctrch_parms.trch_enable_bf|
           config_ptr->l1_dl_cctrch_parms.trch_ref_type_bf)
           );
  for (ix=0; ix<config_ptr->l1_dl_cctrch_parms.num_trch; ix++)
  {
    TDSRRC_MSG3(MSG_LEGACY_MED,"L1DL CCTRCH blerqval:%d,coding:%d,crc:%d",
             config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->bler_quality_val,
             config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->coding_type,
             config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->crc_length
             );
    TDSRRC_MSG3(MSG_LEGACY_MED,"L1DL CCTRCH ntf:%d,trchid:%d,tti:%d",
             config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->num_tf,
             config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->trch_id,
             config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->tti_value
             );
    for (iy=0; iy<config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->num_tf; iy++)
    {
      TDSRRC_MSG2(MSG_LEGACY_MED,"L1DL CCTRCH num_tb:%d,tb_size:%d",
               config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->tf_info_array[iy]->num_tb,
               config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->tf_info_array[iy]->tb_size
               );
    }

  }
  for (ix=0; ix<TDSL1_DL_MAX_SCCPCH; ix++)
  {
    TDSRRC_MSG3(MSG_LEGACY_MED,"L1 SCCPCH tfci coding:%d,punct_limit:%d,repetition_period:%d",
             config_ptr->l1_dl_chan_parms.sccpch[ix].common_ts_info.tfci_coding,
             config_ptr->l1_dl_chan_parms.sccpch[ix].common_ts_info.punct_limit,
             config_ptr->l1_dl_chan_parms.sccpch[ix].common_ts_info.repetition_period
            );
    TDSRRC_MSG3(MSG_LEGACY_MED,"L1 SCCPCH offset:%d,repetition_length:%d,ts_num:%d",
             config_ptr->l1_dl_chan_parms.sccpch[ix].offset,
             config_ptr->l1_dl_chan_parms.sccpch[ix].common_ts_info.repetition_length,
             config_ptr->l1_dl_chan_parms.sccpch[ix].individual_ts_info.ts_num
             );
    TDSRRC_MSG3(MSG_LEGACY_MED,"L1 SCCPCH additional_ss_tpc_symbols:%d,chan_code_bitmap:%d,shift:%d",
             config_ptr->l1_dl_chan_parms.sccpch[ix].individual_ts_info.additional_ss_tpc_symbols,
             config_ptr->l1_dl_chan_parms.sccpch[ix].individual_ts_info.chan_code_bitmap,
             config_ptr->l1_dl_chan_parms.sccpch[ix].individual_ts_info.midamble_ss_burst_type.shift
            );
  }
  TDSRRC_MSG3(MSG_LEGACY_MED,"L1 PICH paging_indicator_length:%d,imsi_dev_8192:%d,imsi_div_pch_count:%d",
           config_ptr->l1_dl_chan_parms.pich.paging_indicator_length,
           config_ptr->l1_dl_chan_parms.pich.imsi_div_8192,
           config_ptr->l1_dl_chan_parms.pich.imsi_div_pch_count
          );

  TDSRRC_MSG2(MSG_LEGACY_MED,"L1 PCCPCH tstd:%d,use_pich:%d",
           config_ptr->l1_dl_chan_parms.pccpch.tstd_indicator,
           config_ptr->l1_dl_chan_parms.use_pich
          );

  TDSRRC_MSG3(MSG_LEGACY_MED,"L1 DL_COMMON doff:%d, timing_ind:%d,tpc_step_size:%d",
           config_ptr->l1_dl_chan_parms.dpch.doff,
           config_ptr->l1_dl_chan_parms.dpch.dpch_common.timing_ind,
           config_ptr->l1_dl_chan_parms.dpch.dpch_common.tpc_step_size
          );

  for (iy=0; iy < TDSL1_DL_MAX_NUM_TS; iy++)
  {
    TDSRRC_MSG3(MSG_LEGACY_MED,"L1 DL_PER_RL ts_num:%d,ss_tpc_symbols:%d,additional_ss_tpc_symbols:%d",
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].ts_num,
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].ss_tpc_symbols,
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].additional_ss_tpc_symbols);

    TDSRRC_MSG3(MSG_LEGACY_MED,"L1 DL_PER_RL allocation_mode:%d,config:%d,shift:%d",
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].midamble_ss_burst_type.allocation_mode,
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].midamble_ss_burst_type.config,
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].midamble_ss_burst_type.shift);

    TDSRRC_MSG1(MSG_LEGACY_MED,"L1 DL_PER_RL chan_code_bitmap:%d",
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].chan_code_bitmap);
  }

}

/*====================================================================
FUNCTION: tdsrrcllc_set_utran_drx_info()

DESCRIPTION:
  This function sets the utran_drx_coef in to tdsordered_config
  data structure. This api is used when a procedure does not have
  to set tdsordered_config but needs to configure it with utran_drx_info

DEPENDENCIES:
  None.

RETURN VALUE:
  void

SIDE EFFECTS:
====================================================================*/
void tdsrrcllc_set_utran_drx_info( uint32 utran_drx_coef )
{
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    tdsordered_config_ptr->utran_drx_cycle_length =
      (1 << utran_drx_coef);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"drx cycle set in OC %d",tdsordered_config_ptr->utran_drx_cycle_length);
    tdscurrent_config_ptr->utran_drx_cycle_length =
      (1 << utran_drx_coef);
  }
  else
  {
    tdscurrent_config_ptr->utran_drx_cycle_length =
      (1 << utran_drx_coef);
    tdsordered_config_ptr->utran_drx_cycle_length =
      (1 << utran_drx_coef);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"drx cycle set in CC %d",tdscurrent_config_ptr->utran_drx_cycle_length);
  }

} /* end tdsrrcllc_set_utran_drx_info() */


/*====================================================================
FUNCTION: tdsrrcllc_update_sib1_data()

DESCRIPTION:
  This function reads SIB1 for the Current Serving Cell and updates
  both Current and Ordered Config databases with DRX cycle values
  for all available CN domains.
  If a domain is not present (CS or PS), the DRX cycle length for
  that domain is initialized to the one for the other domain to
  prevent any wrong calculations elsewhere.

DEPENDENCIES:
  The caller of this function has to make sure Cell Reselection is
  not in progress when this is called. Otherwise it may lead to
  storing DRX values of the old cell.

RETURN VALUE:
  Success/Failure to update.

SIDE EFFECTS:
  Look under dependency.
====================================================================*/
uecomdef_status_e_type  tdsrrcllc_update_sib1_data(void)
{
  tdsrrc_SysInfoType1    *sib1_ptr = NULL;

  /* Pointer & counter to loop through the CN System Information List */
  tdsrrc_CN_DomainSysInfoList * cn_ptr = NULL;
  uint32     cn_cnt  = 0;
  uint32     ps_drx_cycle_length  = 0;
  uint32 cs_drx_cycle_length =0;


  /* Get an index to SIB1 */
  sib1_ptr = (tdsrrc_SysInfoType1 *)
               tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1);
  if (sib1_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 1 not yet in SIB data base");
     return FAILURE;
  }

  /* Get the CS and PS DRX Cycle Lengths */
  cn_ptr = &sib1_ptr->cn_DomainSysInfoList;

  while((cn_cnt < cn_ptr->n) && (cn_cnt < UE_MAX_CN_DOMAINS))
  {
    if((cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff >= TDSRRCLLC_CN_DOMAIN_MIN_DRX_CYCLE_LENGTH) &&
       (cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff <= TDSRRCLLC_CN_DOMAIN_MAX_DRX_CYCLE_LENGTH))
    {
      switch(cn_ptr->elem[cn_cnt].cn_DomainIdentity)
      {
        case tdsrrc_CN_DomainIdentity_cs_domain:
          cs_drx_cycle_length =
            (uint32) (1 << cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff);
          break;
        case tdsrrc_CN_DomainIdentity_ps_domain:
          ps_drx_cycle_length =
            (uint32) (1 << cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff);
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CN 0x%x for DRX", cn_ptr->elem[cn_cnt].cn_DomainIdentity);
          return FAILURE;
      } /* end CN Identity switch */
    } /* end drx cycle length check */
    else
    {
      /* A SIB with invalid DRX length coefficient is unacceptable */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid DRX len value %d, CN domain %d",
          cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff, cn_cnt);
      return FAILURE;
    }
    /* Next CN Info */
    cn_cnt++;
  } /* end while Core Network Info */

  /*
   * Check to make sure both Core Networks have good DRX info
   * Will always get CS DRX info, may not get PS. If no PS info, copy
   * the CS DRX into the PS DRX info.
  */
  if(ps_drx_cycle_length == 0)
  {
    /* This indicates PS domain not present */
    if(cs_drx_cycle_length > 0)
    {
      ps_drx_cycle_length = cs_drx_cycle_length;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Both PS and CS DRX info missing");
      return FAILURE;
    }
  }

  if(cs_drx_cycle_length == 0)
  {
    /* This indicates CS domain not present */
    if(ps_drx_cycle_length > 0)
    {
      cs_drx_cycle_length = ps_drx_cycle_length;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Both PS and CS DRX info missing");
      return FAILURE;
    }
  }

  /* Update Ordered and Current Config now */
  tdscurrent_config_ptr->ps_drx_cycle_length =
  tdsordered_config_ptr->ps_drx_cycle_length =
    ps_drx_cycle_length;

  tdscurrent_config_ptr->cs_drx_cycle_length =
  tdsordered_config_ptr->cs_drx_cycle_length =
    cs_drx_cycle_length;

  return SUCCESS;
} /* tdsrrcllc_update_sib1_data */

/*====================================================================
FUNCTION: tdsrrcllc_calculate_ul_tm_ccch_rlc_size()

DESCRIPTION:
  This is called when the RLC size for an uplink Transparent Mode
  Channel is required. 
  This function returns the rlc size removing the MAC header size from the tansport block size

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The size of the RLC Transparent Mode buffer.

SIDE EFFECTS:
  Note that currently all logical channels mapped to a Transport
  channel must have the same TrBLK size. Therefore, by extracting
  the RLC size of the first Transport Format, this will apply to
  all Transport Formats for all logical channels and types mapped to
  this TrCH.
====================================================================*/
uint32 tdsrrcllc_calculate_ul_tm_ccch_rlc_size
(
  void
)
{
  uint32 rlc_size;
  tdsordered_config_type  *config_ptr;
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }

  if(config_ptr->ul_tfcs.tfs_info[0].tf_info[0].tb_size <
     TDSRRCLLC_COM_LOG_CHL_RACH_MAC_HDR_SIZE)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"TB Size invalid %d",
          config_ptr->ul_tfcs.tfs_info[0].tf_info[0].tb_size);
    return(TDSRRCLLC_INVALID_RLC_SIZE);
  }

  rlc_size = config_ptr->ul_tfcs.tfs_info[0].tf_info[0].tb_size -
    TDSRRCLLC_COM_LOG_CHL_RACH_MAC_HDR_SIZE;
  
  return rlc_size;

} 

/*====================================================================
FUNCTION: tdsrrcllc_get_amr_mapping()

DESCRIPTION:
  This function returns takes a pointer to a data structure of type
  tdsrrcllc_amr_mapping_info_type, fills in the RLC LC IDs of the Uplink
  and Downlink AMR Modes (A, B, C), as well as the AMR and SRC modes\
  in use.

DEPENDENCIES:
  None.

RETURN VALUE:
  FAILURE: If either the AMR mode is not understood (based on TFs)
           Or info on Class A and B is not available. For 12.2 and
           10.2 Kbps AMR modes, Class C info is also needed.
  SUCCESS: If info on AMR mode and all classes is available.

SIDE EFFECTS:
  The uplink and downlink TrCH IDs and RLC Sizes are assumed to be
  the same.

  Since the UE only supports Logical Channels mapped to a TrCH
  where all Logical Channels mapped support all Transport Formats,
  and since this is not the case for AMR Modes, there can only be
  1 Logical channel on this TrCH.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_get_amr_mapping( tdsrrcllc_amr_mapping_info_type *ptr )
{
  /* local index variables */
  uint8 ix;

  boolean found_1x39_class_a = FALSE;
  boolean found_0_class_a = FALSE;
  boolean found_0_class_b = FALSE;
  boolean found_0_class_c = FALSE;
  
  uint32  codec_info;
  /* Local RB ids for AMR Mode Classes */
  tdsrrc_RB_Identity amr_a_rb_id = TDSRRCLLC_INVALID_RB_ID;
  tdsrrc_RB_Identity amr_b_rb_id = TDSRRCLLC_INVALID_RB_ID;
  tdsrrc_RB_Identity amr_c_rb_id = TDSRRCLLC_INVALID_RB_ID;

  /* Indexes of the DCHs for AMR Mode Classes */
  uint8 amr_a_dch_idx = TDSUE_MAX_TRCH;
  uint8 amr_b_dch_idx = TDSUE_MAX_TRCH;
  uint8 amr_c_dch_idx = TDSUE_MAX_TRCH;

  tdsdch_id_mapping dch_mapping[TDSMAX_DCH_VOICE_CHANNEL];
  

  /* Disable SCR(DTX), aka Background Noise suppression */
  ptr->scr_mode = FALSE;

  /* Set AMR mode to invalid value */
  ptr->amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Get AMR Mapping info");
  
  /*
  * Get the RLC Logical Channel IDs for UL, and use
  * RB IDs extracted here to get Logical Channel IDs
  * for DL
  */
  
  ptr->amr_mode=  tdsrrcllc_derive_amr_rate(&codec_info, dch_mapping);
    
  if(codec_info != TDSCDMA_IVOCODER_ID_AMR )
  {
    return FAILURE;
  }
  if(dch_mapping[0].transport_channel_index != TDSUE_MAX_TRCH)
  {
    /* update the rlc_id, rb_id for Class A*/
    ptr->ul_amr_a_id = dch_mapping[0].rlc_id;
  
    /* Remember the RB ID for class A  so can easily get the DL AMR channel */
    amr_a_rb_id = dch_mapping[0].rb_id;
  
    /* Remember the DCH idx for class A so we can check for DTX activation later */
    amr_a_dch_idx = dch_mapping[0].transport_channel_index;
  }
  
  if(dch_mapping[1].transport_channel_index != TDSUE_MAX_TRCH)
  {
    /* update the rlc_id, rb_id for Class B*/
    ptr->ul_amr_b_id = dch_mapping[1].rlc_id;
  
    /* Remember the RB ID for class B so can easily get the DL AMR channel */
    amr_b_rb_id = dch_mapping[1].rb_id;
  
    /* Remember the DCH idx for class B  so we can check for DTX activation later */
    amr_b_dch_idx = dch_mapping[1].transport_channel_index;
  }
  if(dch_mapping[2].transport_channel_index != TDSUE_MAX_TRCH)
  {
    /* update the rlc_id for Class C*/
    ptr->ul_amr_c_id = dch_mapping[2].rlc_id;
    
    /* Remember the RB ID for class c  so can easily get the DL AMR channel */
    amr_c_rb_id = dch_mapping[2].rb_id;
    
    /* Remember the DCH idx for class c so we can check for DTX activation later */
    amr_c_dch_idx = dch_mapping[2].transport_channel_index;
  }

  
  /* Check whether to enable DTX (Source Controlled Rate).  DTX should be
     enabled if both of the following conditions are met:

       * There is a TFC that allows a SID frame (i.e. 1x39 for Class A)
       * There is a TFC that allows no data on each of Class A, B, and C

     This check is for Uplink only.
     Note: For UMTS: SCR and DTX are the same.
  */
  if(amr_a_dch_idx < TDSUE_MAX_TRCH)
  {
    for(ix = 0; ix < tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].ntf; ix++)
    {
      if(tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].tf_info[ix].tb_size ==
         TDSRRCLLC_AMR_MODE_8_CLASS_A_BIT_SIZE &&
         tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].tf_info[ix].ntblks ==
         1)
      {
        /* This is a 1x39 TFC */
        found_1x39_class_a = TRUE;
      }
      else if(tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].tf_info[ix].tb_size *
              tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].tf_info[ix].ntblks
              == 0)
      {
        /* This is a TFC that allows no data */
        found_0_class_a = TRUE;
      }

      if(found_1x39_class_a && found_0_class_a)
      {
        break;
      }
    }
  }

  if(amr_b_dch_idx < TDSUE_MAX_TRCH)
  {
    for(ix = 0; ix < tdsordered_config_ptr->ul_tfcs.tfs_info[amr_b_dch_idx].ntf; ix++)
    {
      if(tdsordered_config_ptr->ul_tfcs.tfs_info[amr_b_dch_idx].tf_info[ix].tb_size *
         tdsordered_config_ptr->ul_tfcs.tfs_info[amr_b_dch_idx].tf_info[ix].ntblks
         == 0)
      {
        /* This is a TFC that allows no data */
        found_0_class_b = TRUE;
        break;
      }
    }
  }

  if(amr_c_dch_idx < TDSUE_MAX_TRCH)
  {
    for(ix = 0; ix < tdsordered_config_ptr->ul_tfcs.tfs_info[amr_c_dch_idx].ntf; ix++)
    {
      if(tdsordered_config_ptr->ul_tfcs.tfs_info[amr_c_dch_idx].tf_info[ix].tb_size *
         tdsordered_config_ptr->ul_tfcs.tfs_info[amr_c_dch_idx].tf_info[ix].ntblks
         == 0)
      {
        /* This is a TFC that allows no data */
        found_0_class_c = TRUE;
        break;
      }
    }
  }

  if(found_1x39_class_a && found_0_class_a && found_0_class_b && tdsrrc_nv_enable_audio_scr_mode)
  {
    /* Provisionally switch on DTX.  If we find later that we needed a Class C
       and didn't find one, we'll switch it back off again. */
    ptr->scr_mode = TRUE;
  }

  if(ptr->amr_mode == TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown AMR mode");
    return(FAILURE);
  }

  /* Check if TrCHs for Class A and B data have been
   * found. For 12.2 and 10.2 AMR modes, also check for
   * channel for Class C data.
   */
  if((amr_a_rb_id == TDSRRCLLC_INVALID_RB_ID) ||
     (amr_b_rb_id == TDSRRCLLC_INVALID_RB_ID))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not all UL TrCHs for AMR found");
    return(FAILURE);
  }
  else
  {
    if((ptr->amr_mode == TDSCDMA_IVOCAMR_CODEC_MODE_1220) ||
       (ptr->amr_mode == TDSCDMA_IVOCAMR_CODEC_MODE_1020))
    {
      /* Only 12.2 and 10.2 modes have Class C data */
      if(amr_c_rb_id == TDSRRCLLC_INVALID_RB_ID)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not all UL TrCHs for AMR found");
        return(FAILURE);
      }

      if(!found_0_class_c)
      {
        /* If we previously turned on DTX, we need to turn it off now, since
           we didn't find a cooperative Class C TFC. */
        ptr->scr_mode = FALSE;
      }
    }
  }  /* If Class A and B TrCH have been found */
  TDSRRC_MSG2(MSG_LEGACY_MED,"Audio DTX scr_mode=%d, nv_enabled=%d",ptr->scr_mode, tdsrrc_nv_enable_audio_scr_mode);
  
  /* Go through the DL RB Mapping info to get the DL RLC IDs */
  for(ix = 0; ix < tdsordered_config_ptr->mac_dl_parms.num_dchs; ix++)
  {
    if(tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rb_id == amr_a_rb_id)
    {
      ptr->dl_amr_a_id = tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rlc_id;
      amr_a_rb_id = TDSRRCLLC_INVALID_RB_ID;
    }
    else if(tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rb_id == amr_b_rb_id)
    {
      ptr->dl_amr_b_id = tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rlc_id;
      amr_b_rb_id = TDSRRCLLC_INVALID_RB_ID;
    }
    else if(tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rb_id == amr_c_rb_id)
    {
      ptr->dl_amr_c_id = tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rlc_id;
      amr_c_rb_id = TDSRRCLLC_INVALID_RB_ID;
    }
    /* Break the loop if all Logical Channel IDs have been established */
    if((amr_a_rb_id == TDSRRCLLC_INVALID_RB_ID) &&
       (amr_b_rb_id == TDSRRCLLC_INVALID_RB_ID))
    {
      if((ptr->amr_mode == TDSCDMA_IVOCAMR_CODEC_MODE_1220) ||
         (ptr->amr_mode == TDSCDMA_IVOCAMR_CODEC_MODE_1020))
      {
        /* For 12.2 and 10.2 AMR modes, make sure Class C
         * info has been obtained.
         */
        if(amr_c_rb_id == TDSRRCLLC_INVALID_RB_ID)
        {
          break;
        }
      }  /* AMR mode of 12.2 or 10.2 kbps */
      else /* All other AMR modes do not have Class C data */
      {
        break;
      } /* All other AMR modes that have no Class C bits */
    } /* Info for Class A and B logical channel has been obtained */
  }

  /* Check if TrCHs for Class A and B data have been
   * found. For 12.2 and 10.2 AMR modes, also check for
   * channel for Class C data.
   */
  if((amr_a_rb_id != TDSRRCLLC_INVALID_RB_ID) ||
     (amr_b_rb_id != TDSRRCLLC_INVALID_RB_ID))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not all DL TrCHs for AMR found");
    return(FAILURE);
  }
  else
  {
    if((ptr->amr_mode == TDSCDMA_IVOCAMR_CODEC_MODE_1220) ||
       (ptr->amr_mode == TDSCDMA_IVOCAMR_CODEC_MODE_1020))
    {
      /* Only 12.2 and 10.2 modes have Class C data */
      if(amr_c_rb_id != TDSRRCLLC_INVALID_RB_ID)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not all DL TrCHs for AMR found");
        return(FAILURE);
      }
    }
  }  /* If Class A and B TrCH have been found */

  tdsrrcllc_derive_dl_amr_mode(ptr);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"AMR Mode for DL = %0x",ptr->dl_amr_mode);
 
  return(SUCCESS);
}



/*====================================================================
FUNCTION: tdsrrcllc_get_amr_wb_mapping()

DESCRIPTION:
  This function returns takes a pointer to a data structure of type
  rrcllc_amr_wb_mapping_info_type, fills in the RLC LC IDs of the Uplink
  and Downlink AMR Modes (A, B, C), as well as the AMR and SRC modes\
  in use.

DEPENDENCIES:
  None.

RETURN VALUE:
  FAILURE: If either the AMR mode is not understood (based on TFs)
           Or info on Class A and B is not available. 
  SUCCESS: If info on AMR mode and all classes is available.

SIDE EFFECTS:
  The uplink and downlink TrCH IDs and RLC Sizes are assumed to be
  the same.

  Since the UE only supports Logical Channels mapped to a TrCH
  where all Logical Channels mapped support all Transport Formats,
  and since this is not the case for AMR Modes, there can only be
  1 Logical channel on this TrCH.
====================================================================*/

uecomdef_status_e_type
tdsrrcllc_get_amr_wb_mapping( tdsrrcllc_amr_mapping_info_type *ptr )
{
  /* local index variables */
  uint8 ix;

  boolean found_1x40_class_a = FALSE;
  boolean found_0_class_a = FALSE;
  boolean found_0_class_b = FALSE;
  
  /* Local RB ids for AMR Mode Classes */
  tdsrrc_RB_Identity amr_a_rb_id = TDSRRCLLC_INVALID_RB_ID;
  tdsrrc_RB_Identity amr_b_rb_id = TDSRRCLLC_INVALID_RB_ID;

  /* Indexes of the DCHs for AMR Mode Classes */
  uint8 amr_a_dch_idx = TDSUE_MAX_TRCH;
  uint8 amr_b_dch_idx = TDSUE_MAX_TRCH;

  tdsdch_id_mapping dch_mapping[TDSMAX_DCH_VOICE_CHANNEL];

  /* Disable SCR(DTX), aka Background Noise suppression */
  uint32 codec_info;
  ptr->scr_mode = FALSE;

  /* Set AMR mode to invalid value */ 
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Get WB-AMR Mapping info");


  ptr->amr_mode=  tdsrrcllc_derive_amr_rate(&codec_info,dch_mapping);
    
  if(codec_info != TDSCDMA_IVOCODER_ID_AMRWB )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"is not WB AMR mode");
    return FAILURE;
  }
     
  if(dch_mapping[0].transport_channel_index != TDSUE_MAX_TRCH)
  {
    /* update the rlc_id, rb_id for Class A*/
    ptr->ul_amr_a_id = dch_mapping[0].rlc_id;
    
    /* Remember the RB ID so can easily get the DL AMR channel */
    amr_a_rb_id = dch_mapping[0].rb_id;
    
    /* Remember the DCH idx so we can check for DTX activation later */
    amr_a_dch_idx = dch_mapping[0].transport_channel_index;
  }
  
  if(dch_mapping[1].transport_channel_index != TDSUE_MAX_TRCH)
  {
    /* update the rlc_id, rb_id for Class B*/
    ptr->ul_amr_b_id = dch_mapping[1].rlc_id;
    
    /* Remember the RB ID so can easily get the DL AMR channel */
    amr_b_rb_id = dch_mapping[1].rb_id;
    
    /* Remember the DCH idx so we can check for DTX activation later */
    amr_b_dch_idx = dch_mapping[1].transport_channel_index;
  }
  /*  Check whether to enable DTX (Source Controlled Rate).  DTX should be
  *  enabled if both of the following conditions are met:
  *
  *    * There is a TFC that allows a SID frame (i.e. 1x40 for Class A)
  *    * There is a TFC that allows no data on each of Class A and C
  *
  *  This check is for Uplink only.
  *  Note: For UMTS: SCR and DTX are the same.
  */
  if(amr_a_dch_idx < TDSUE_MAX_TRCH)
  {
    for(ix = 0; ix < tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].ntf; ix++)
    {
      if(tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].tf_info[ix].tb_size ==
         TDSRRCLLC_AMR_WB_CLASS_A_BIT_SIZE &&
         tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].tf_info[ix].ntblks ==
         1)
      {
        /* This is a 1x40 TFC */
        found_1x40_class_a = TRUE;
      }
      else if(tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].tf_info[ix].tb_size *
              tdsordered_config_ptr->ul_tfcs.tfs_info[amr_a_dch_idx].tf_info[ix].ntblks
              == 0)
      {
        /* This is a TFC that allows no data */
        found_0_class_a = TRUE;
      }
      /* Exit the loop if the TFC for a SID frame and TFC with no data is found */
      if(found_1x40_class_a && found_0_class_a)
      {
        break;
      }
    }
  }

  if(amr_b_dch_idx < TDSUE_MAX_TRCH)
  {
    for(ix = 0; ix < tdsordered_config_ptr->ul_tfcs.tfs_info[amr_b_dch_idx].ntf; ix++)
    {
      if(tdsordered_config_ptr->ul_tfcs.tfs_info[amr_b_dch_idx].tf_info[ix].tb_size *
         tdsordered_config_ptr->ul_tfcs.tfs_info[amr_b_dch_idx].tf_info[ix].ntblks
         == 0)
      {
        /* This is a TFC that allows no data */
        found_0_class_b = TRUE;
        break;
      }
    }
  }

  /* DTX can be enabled is TFC with SID frame and 
   * TFC with no data(for Class A & Class B) is found
   */
  if(found_1x40_class_a && found_0_class_a && found_0_class_b && tdsrrc_nv_enable_audio_scr_mode)
  {
    /* Provisionally switch on DTX.  */
    ptr->scr_mode = TRUE;
  }


  if(ptr->amr_mode == TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown WB AMR mode");
    return(FAILURE);
  }

  /* Check if TrCHs for Class A and B data have been
   * found. 
   */
  if((amr_a_rb_id == TDSRRCLLC_INVALID_RB_ID) ||
     (amr_b_rb_id == TDSRRCLLC_INVALID_RB_ID))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not all UL TrCHs for AMR found");
    return(FAILURE);
  }
 
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Audio DTX scr_mode=%d, nv_enabled=%d",ptr->scr_mode, tdsrrc_nv_enable_audio_scr_mode);
  /* Go through the DL RB Mapping info to get the DL RLC IDs */
  for(ix = 0; ix < tdsordered_config_ptr->mac_dl_parms.num_dchs; ix++)
  {
    if(tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rb_id == amr_a_rb_id)
    {
      /* Update The RLC-ID, transport channel id and CCTrCh ID for 
       * for Class A the DL Channel also
       */
      ptr->dl_amr_a_id = tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rlc_id;
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"amr_a_rb_id made invalid");
      amr_a_rb_id = TDSRRCLLC_INVALID_RB_ID;
    }
    else if(tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rb_id == amr_b_rb_id)
    {
      /* Update The RLC-ID, transport channel id and CCTrCh ID for 
       * for Class B the DL Channel also
       */
      ptr->dl_amr_b_id = tdsordered_config_ptr->dl_dch_info[ix].dlchan_info[0].rlc_id;
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"amr_b_rb_id made invalid");
      amr_b_rb_id = TDSRRCLLC_INVALID_RB_ID;
    }
    
    /* Break the loop if all Logical Channel IDs have been established */
    if((amr_a_rb_id == TDSRRCLLC_INVALID_RB_ID) &&
       (amr_b_rb_id == TDSRRCLLC_INVALID_RB_ID))
    {
      break;     
    } /* Info for Class A and B logical channel has been obtained */
  }
 
  /* Check if TrCHs for Class A and B data have been
   * found. 
   */
  if((amr_a_rb_id != TDSRRCLLC_INVALID_RB_ID) ||
     (amr_b_rb_id != TDSRRCLLC_INVALID_RB_ID))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not all DL TrCHs for AMR found");
    return(FAILURE);
  }

  /* If Class A and B TrCH have been found */
  return(SUCCESS);
}

/*====================================================================
FUNCTION: tdsrrcllc_derive_amr_rate()

DESCRIPTION: 
  This function derives the AMR rate. And differentaites if a Wb-AMR 
  codec or a NB-AMR codecis used to configured. 

DEPENDENCIES:
  None.

RETURN VALUE:
 None

SIDE EFFECTS:
  
====================================================================*/
uint32 tdsrrcllc_derive_amr_rate
( 
  uint32 * codec_info ,
  tdsdch_id_mapping * dch_mapping
)
{
  /* local index variables */
  uint8 ix, iy,iz;
  uint8 index=0;
  uint16   tfi;

  uint32  current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
  uint32  previous_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
  uint16 total_bits;
  boolean found  = TRUE;
  boolean found_rate = FALSE;
  tds_ul_amr_rate_event_type start_event_log;
  /* Disable SCR(DTX), aka Background Noise suppression */

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Derive AMR mode and rate");
  start_event_log.num_rates = 0;
  start_event_log.codec_type = TDSRRC_NO_CODEC; /*In valid*/
  for(ix = 0; ix < TDSRRC_AMR_RATE_SIZE; ix++)
  {
    start_event_log.amr_rate[ix] = TDSMAC_AMR_MODE_UNDEF;
  }
  /*
  * Get the RLC Logical Channel IDs for UL, and use
  * RB IDs extracted here to get Logical Channel IDs
  * for DL
  */
  for(ix =0;ix< TDSMAX_DCH_VOICE_CHANNEL; ix++ )
  {
    dch_mapping[ix].transport_channel_index = TDSUE_MAX_TRCH;
    dch_mapping[ix].amr_class_size = 0;
    dch_mapping[ix].rlc_id= 0xFF;
    dch_mapping[ix].rb_id = TDSRRCLLC_INVALID_RB_ID;
  }
    
  for (ix=0; ix < tdsordered_config_ptr->ul_tfcs.ntfc; ix++)
  {
    index =0;
    /* Since for every rate it iterated, reset the values  */
    dch_mapping[0].amr_class_size =0;
    dch_mapping[1].amr_class_size=0;
    dch_mapping[2].amr_class_size=0;
    /* Check if the TFC is allowed*/
    if (tdsordered_config_ptr->ul_tfcs.tfc_info[ix].control_flag == TDSTFC_ALLOWED)
    {  
      /* Go through the number of transport channels configured*/
      for (iy=0; iy < tdsordered_config_ptr->ul_tfcs.nchan; iy++)
      {
         /* Only check for DTCHs, only 1 DTCH per DCH */
        if((tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[iy].dlchan_info[0].chan_type == UE_LOGCHAN_DTCH)
            && (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[iy].dlchan_info[0].rlc_mode== UE_MODE_TRANSPARENT))
        {
          /* Since we allocated for only 3 RBs for voice */
          if(index == TDSMAX_DCH_VOICE_CHANNEL )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"only 3 transport channel allowed index> %d so break ",index);
            break;
          }
          /* Get the transport format index for TFC "ix" and TFI corresponding to Transoprt channel "iy"*/
          tfi = tdsordered_config_ptr->ul_tfcs.tfc_info[ix].tfi[iy];
         
          /* If the number of transport blocks are > 0 */
          if (tdsordered_config_ptr->ul_tfcs.tfs_info[iy].tf_info[tfi].ntblks > 0)
          {
            /* Check the TB_size*/
            dch_mapping[index].amr_class_size = tdsordered_config_ptr->ul_tfcs.tfs_info[iy].tf_info[tfi].tb_size;
                     /* update the rlc_id, rb_id for Class A*/
            dch_mapping[index].rlc_id = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[iy].dlchan_info[0].rlc_id;
        
            /* Remember the RB ID so can easily get the DL AMR channel */
            dch_mapping[index].rb_id = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[iy].dlchan_info[0].rb_id;
        
            dch_mapping[index].transport_channel_index = iy;
            index++;                            
          }
        }
      }
      
           
      total_bits = dch_mapping[0].amr_class_size + dch_mapping[1].amr_class_size +dch_mapping[2].amr_class_size;
                
      switch(total_bits)
      {
        case TDSWVS_AMR_WB_MODE_660_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_0660;
          TDSRRC_MSG0(MSG_LEGACY_MED," found WB-AMR rate 6.60 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_885_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_0885;
          TDSRRC_MSG0(MSG_LEGACY_MED,"found WB-AMR rate 8.85 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1265_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1265;
          TDSRRC_MSG0(MSG_LEGACY_MED," found WB-AMR rate 12.65 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1425_TOTAL_BITS :
        { 
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1425;          
          TDSRRC_MSG0(MSG_LEGACY_MED," found WB-AMR rate 14.25 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1585_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1585;
          TDSRRC_MSG0(MSG_LEGACY_MED," found WB-AMR rate 15.85 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1825_TOTAL_BITS : 
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1825;
          TDSRRC_MSG0(MSG_LEGACY_MED," found WB-AMR rate 18.25 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1985_TOTAL_BITS : 
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1985;
          TDSRRC_MSG0(MSG_LEGACY_MED," found WB-AMR rate 19.85 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_2305_TOTAL_BITS : 
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_2305;
          TDSRRC_MSG0(MSG_LEGACY_MED," found WB-AMR rate 23.05 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_2385_TOTAL_BITS : 
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_2385;
          TDSRRC_MSG0(MSG_LEGACY_MED," found WB-AMR rate 23.85 K");
         }
         break;
         case TDSWVS_AMR_MODE_122_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_1220;
          TDSRRC_MSG0(MSG_LEGACY_MED," found NB-AMR rate 12.20 K");
        }
        break;
        case TDSWVS_AMR_MODE_102_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_1020;
          TDSRRC_MSG0(MSG_LEGACY_MED," found NB-AMR rate 10.20 K");
        }
        break;
        case TDSWVS_AMR_MODE_795_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0795;
          TDSRRC_MSG0(MSG_LEGACY_MED," found NB-AMR rate 7.95 K");
        }
        break;
        case TDSWVS_AMR_MODE_740_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0740;
          TDSRRC_MSG0(MSG_LEGACY_MED," found NB-AMR rate 7.40 K");
        }
        break;
        case TDSWVS_AMR_MODE_670_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0670;
          TDSRRC_MSG0(MSG_LEGACY_MED," found NB-AMR rate 6.70 K");
        }
        break;
        case TDSWVS_AMR_MODE_590_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0590;
          TDSRRC_MSG0(MSG_LEGACY_MED," found NB-AMR rate 5.90 K");
        }
        break;
        case TDSWVS_AMR_MODE_515_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0515;
          TDSRRC_MSG0(MSG_LEGACY_MED," found NB-AMR rate 5.15 K");
        }  
        break;
        case TDSWVS_AMR_WB_MODE_475_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
          TDSRRC_MSG0(MSG_LEGACY_MED," found NB-AMR rate 4.75 K");
        }
        break;         
        default:
          if((total_bits == TDSRRCLLC_AMR_WB_CLASS_NO_DATA_FRAME_SIZE)
             || (total_bits == TDSRRCLLC_AMR_WB_CLASS_A_BIT_SIZE)
             ||(total_bits == TDSRRCLLC_AMR_MODE_8_CLASS_A_BIT_SIZE))
          {
            TDSRRC_MSG1(MSG_LEGACY_MED,"SID frames found or No dat frames  %d found",
                              total_bits);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_MED,"wrong total size %d for  AMR",
                              total_bits);
          }
          found = FALSE;
        break;
      }
      if(found == TRUE)
      {
        boolean rate_present_in_event = FALSE;
        found_rate = TRUE;
        TDSRRC_MSG2(MSG_LEGACY_MED,"Total bits %d and  amr mode is %0x", total_bits, current_amr_mode);
        for(iz =0;(iz < TDSRRC_AMR_RATE_SIZE) && (iz<start_event_log.num_rates);iz++)
        {
          if(start_event_log.amr_rate[iz] == ((tdsrrc_event_ul_amr_rate_type)current_amr_mode) )
          {
            rate_present_in_event = TRUE;
            break;
          }
        }
        if((rate_present_in_event == FALSE) && (start_event_log.num_rates < TDSRRC_AMR_RATE_SIZE))
        {
          start_event_log.amr_rate[ start_event_log.num_rates] =
              (tdsrrc_event_ul_amr_rate_type) current_amr_mode;
          start_event_log.num_rates++;
        }

        if(previous_amr_mode < current_amr_mode)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Multirate wb-amr lower amr rate %0x higher rate %0x ", 
            previous_amr_mode, current_amr_mode);
          previous_amr_mode = current_amr_mode;
        }
      }
      found = TRUE;
    }
  }
      
  if(found_rate == TRUE )
  {
    if( (previous_amr_mode >= TDSCDMA_IVOCAMRWB_CODEC_MODE_0660) &&
       (previous_amr_mode < TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF))
    {
      *codec_info = TDSCDMA_IVOCODER_ID_AMRWB;
      start_event_log.codec_type = TDSRRC_WB_AMR_CODEC;
    }
    else
    {
      *codec_info = TDSCDMA_IVOCODER_ID_AMR;
      start_event_log.codec_type = TDSRRC_NB_AMR_CODEC;
    }
    event_report_payload(EVENT_TDSCDMA_MAC_UL_AMR_RATE,
                       sizeof(start_event_log),
                       (void *)&start_event_log);
  }
  else
  {
    *codec_info = TDSCDMA_IVOCODER_ID_NONE;
  }   
  return previous_amr_mode;
}


/*====================================================================
FUNCTION: tdsrrcllc_derive_dl_amr_mode()

DESCRIPTION:

  This function returns AMR mode for DL. This function uses DL CTFCs 
  to derive the AMR mode.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  
====================================================================*/
void tdsrrcllc_derive_dl_amr_mode(tdsrrcllc_amr_mapping_info_type *ptr)
{
  uint8 ix, iy, index=0, tfi;
  uint16 total_bits;
  tdsdch_id_mapping dch_mapping[TDSMAX_DCH_VOICE_CHANNEL];
 
  boolean found  = TRUE;
  uint32  current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
  uint32  previous_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Derive AMR mode for DL using CTFCs");


  for(ix =0;ix< TDSMAX_DCH_VOICE_CHANNEL; ix++ )
  {
    dch_mapping[ix].transport_channel_index = TDSUE_MAX_TRCH;
    dch_mapping[ix].amr_class_size = 0;
    dch_mapping[ix].rlc_id= 0xFF;
  }
    
  
  /*Loop through all the CTFCs for DL*/
  for (ix=0; ix < tdsordered_config_ptr->dl_ctfc.num_ctfc_entry ;ix++)
  {
    index =0;
    dch_mapping[0].amr_class_size =0; /*Class A*/
    dch_mapping[1].amr_class_size =0; /*Class B*/
    dch_mapping[2].amr_class_size =0; /*Class C*/
    
    /*Check whether the CTFC is allowed or not*/
    if (tdsordered_config_ptr->dl_ctfc.ctfc_entry_data[ix].state== TDSALLOWED)
    {
      /*Loop through all the DL DCH TrCHs*/
      for (iy=0; iy < tdsordered_config_ptr->mac_dl_parms.num_dchs; iy++)
      {
        /*Check whether the Logical channel mapped to DCH is of DTCH type and of TM RLC mode*/
        if((tdsordered_config_ptr->mac_dl_parms.dch_info[iy]->dlchan_info[0].chan_type == UE_LOGCHAN_DTCH)
              && (tdsordered_config_ptr->mac_dl_parms.dch_info[iy]->dlchan_info[0].rlc_mode== UE_MODE_TRANSPARENT))
        {
          if(index == TDSMAX_DCH_VOICE_CHANNEL)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Only 3 transport channel allowed (index = %d) so break ",index);
            break;
          }
          /*TF index given by DL CTFCs*/
          tfi = tdsordered_config_ptr->dl_ctfc.ctfc_entry_data[ix].tfi_table[iy];

          if ((tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[iy]->tf_info_array[tfi]->num_tb > 0)
            &&(index < TDSMAX_DCH_VOICE_CHANNEL))
          {
            dch_mapping[index].rlc_id = tdsordered_config_ptr->mac_dl_parms.dch_info[iy]->dlchan_info[0].rlc_id;
            dch_mapping[index].transport_channel_index = (uint8)tdsordered_config_ptr->dl_dch_info[iy].trch_id;

            dch_mapping[index].amr_class_size = (tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[iy]->tf_info_array[tfi]->tb_size);
            index++;
          }
        }
      }
      total_bits = dch_mapping[0].amr_class_size + dch_mapping[1].amr_class_size + dch_mapping[2].amr_class_size;
    
      switch(total_bits)
      {
        case TDSWVS_AMR_WB_MODE_660_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_0660;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found WB-AMR rate 6.60 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_885_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_0885;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found WB-AMR rate 8.85 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1265_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1265;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found WB-AMR rate 12.65 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1425_TOTAL_BITS :
        { 
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1425;          
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found WB-AMR rate 14.25 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1585_TOTAL_BITS :
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1585;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found WB-AMR rate 15.85 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1825_TOTAL_BITS : 
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1825;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found WB-AMR rate 18.25 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_1985_TOTAL_BITS : 
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_1985;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found WB-AMR rate 19.85 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_2305_TOTAL_BITS : 
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_2305;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found WB-AMR rate 23.05 K");
        }
        break;
        case TDSWVS_AMR_WB_MODE_2385_TOTAL_BITS : 
        {
          current_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_2385;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found WB-AMR rate 23.85 K");
        }
        break;
        case TDSWVS_AMR_MODE_122_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_1220;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found NB-AMR rate 12.20 K");
        }
        break;
        case TDSWVS_AMR_MODE_102_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_1020;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found NB-AMR rate 10.20 K");
        }
        break;
        case TDSWVS_AMR_MODE_795_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0795;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found NB-AMR rate 7.95 K");
        }
        break;
        case TDSWVS_AMR_MODE_740_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0740;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found NB-AMR rate 7.40 K");
        }
        break;
        case TDSWVS_AMR_MODE_670_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0670;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found NB-AMR rate 6.70 K");
        }
        break;
        case TDSWVS_AMR_MODE_590_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0590;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found NB-AMR rate 5.90 K");
        }
        break;
        case TDSWVS_AMR_MODE_515_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0515;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found NB-AMR rate 5.15 K");
        }  
        break;
        case TDSWVS_AMR_WB_MODE_475_TOTAL_BITS  :
        {
          current_amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Found NB-AMR rate 4.75 K");
        }
        break;         
        default:
          if((total_bits == TDSRRCLLC_AMR_WB_CLASS_NO_DATA_FRAME_SIZE)
              || (total_bits == TDSRRCLLC_AMR_WB_CLASS_A_BIT_SIZE)
              ||(total_bits == TDSRRCLLC_AMR_MODE_8_CLASS_A_BIT_SIZE))
          {
            TDSRRC_MSG1(MSG_LEGACY_MED,"SID frames found or No data frames %d found",
                              total_bits);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_MED,"Wrong total size %d for AMR",
                              total_bits);
          }
          found = FALSE;
        break;
      }
      if(found == TRUE)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Total bits %d and AMR mode %0x",total_bits, current_amr_mode);
        /* Comparison is done to find the larger of the tow amr modes */
        if(previous_amr_mode < current_amr_mode)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Multirate_AMR, lower AMR mode %0x higher AMR mode %0x ",
            previous_amr_mode, current_amr_mode);
          previous_amr_mode = current_amr_mode;
        }
      } /* if(found == TRUE) */
      found = TRUE;
    }
  }
  
  ptr->dl_amr_mode = previous_amr_mode;

  ptr->dl_amr_a_id = dch_mapping[0].rlc_id;
  ptr->dl_amr_b_id = dch_mapping[1].rlc_id;
  ptr->dl_amr_c_id = dch_mapping[2].rlc_id;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DL AMR Mode %0x",ptr->dl_amr_mode);
  return;
}

/*====================================================================
FUNCTION: tdsrrcllc_set_ordered_config

DESCRIPTION:
  This is called when the TDSORDERED_CONFIG variable needs to be set.
  It checks to see whether or not it is set or not, and returns
  the status of the check.

  It also takes a pointer to the asn1 decoded message stream and
  places it into the tdsordered_config database.

  The procedure name is required so that the message asn1 decoded
  message can be properly typecast so as to populate the
  TDSORDERED_CONFIG database.

DEPENDENCIES:
  CELL_DCH is the only supported state. Now Cell_FACH/Cell_PCH/URA_PCH
  are also supported.

RETURN VALUE:
  tdsrrcllc_oc_status_e_type - indicates whether the TDSORDERED_CONFIG
                            variable could be successfully set or
                            what the failure was.

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type tdsrrcllc_set_ordered_config
(
  /*
  * This variable indicates the PDU number as defined in the ASN.1
  * header file. This information can be used to map the to correct
  * logical channel received on and then down to the message type.
  * The message contents are in msg_ptr, and need to be typecasted.
  */
  tdsrrc_proc_e_type  proc_id,
  tdsrrc_state_e_type  next_state,
  uint32            dl_sdu_num,
  void             *msg_ptr
)
{
  /* local copy of current state */
  tdsrrc_state_e_type current_state;

  tdsrrcllc_oc_status_e_type  oc_status;

  uint8 idx=0;

  tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSMAX_NUM_OF_PROCEDURES);

  /* Check to see if TDSORDERED_CONFIG is set */
  if (tdsordered_config.set_status != TDSOC_NOT_SET
      || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS)
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITHOUT_DCH_INFO)
      || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITH_DCH_INFO)
      || (tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH)
      || (tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH_WITH_DCH_INFO)
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
      )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incompatible Simultaneous Reconfig");
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"OC status %d, proc %d",
                tdsordered_config.set_status, tdsordered_config.set_by_proc);
    return (TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG);
  }

  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG &&
    tdsrrcllc_check_if_chan_config_in_progress_for_sib5_change())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC is configuring lower layers for SIB5 change");
    return (TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG);
  }
  
  /*Copy the current Rb-Mapping to the ordered RB-Mapping database
  * As the OTA contents will updated in the Ordered RB-mapping database
  */
  if( (proc_id == TDSRRC_PROCEDURE_RBE)||
    (proc_id == TDSRRC_PROCEDURE_RBR)||
    (proc_id == TDSRRC_PROCEDURE_RBRC)||
    (proc_id == TDSRRC_PROCEDURE_CU)||
    (proc_id == TDSRRC_PROCEDURE_TCR)
    )
  {
    memscpy(tdsrrc_ordered_hanging_rb_mapping,(sizeof(tdsrb_mapping_info_type)*TDSMAX_HANGING_RB_MAPPING_INFO), tdsrrc_current_hanging_rb_mapping,
                              (sizeof(tdsrb_mapping_info_type)*TDSMAX_HANGING_RB_MAPPING_INFO));
  }
  /*
  * If made it this far, check the state to determine how to
  * setup TDSORDERED_CONFIG
  */
  if (next_state == TDSRRC_STATE_DISCONNECTED)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cannot set TDSORDERED_CONFIG in Idle-Disconnected State");
    return (TDSORDERED_CONFIG_NOT_SET_OTHER);
  }

  /* Get the current state */
  current_state = tdsrrc_get_state();
  tdsrrc_ordered_state = next_state;

  /*Initialization of tdscell_id_per_rl is done for nonDCH -> DCH states.*/
  if((next_state == TDSRRC_STATE_CELL_DCH) && (current_state !=TDSRRC_STATE_CELL_DCH))
  {
    for(idx=0; idx < TDSL1_MAX_RL; idx++)
    {
      tdscell_id_per_rl[idx] = TDSINVALID_CELL_ID_OTA;
    }
  }  

  switch (next_state)
  {
    case TDSRRC_STATE_CELL_DCH:
    {

      switch(current_state)
      {
        case TDSRRC_STATE_CELL_DCH:
          oc_status = tdsrrcllc_update_oc_in_dch(dl_sdu_num, msg_ptr);
          break;

        case TDSRRC_STATE_CONNECTING:
          oc_status = tdsrrcllc_update_oc_from_connecting_to_dch(dl_sdu_num, msg_ptr);
          break;

        case TDSRRC_STATE_CELL_FACH:
          oc_status = tdsrrcllc_update_oc_from_fach_to_dch(dl_sdu_num, msg_ptr);
          break;


#if defined(FEATURE_INTERRAT_HANDOVER_G2TD)||defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
        case TDSRRC_STATE_DISCONNECTED:
          oc_status = tdsrrcllc_update_oc_from_disconnected_to_dch(dl_sdu_num, msg_ptr);
          break;
#endif /* FEATURE_INTERRAT_HANDOVER_G2TD */

        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"State %d to Cell_DCH transition not supported", current_state);
          oc_status = TDSORDERED_CONFIG_CONFIGURATION_INVALID;
          break;
      }
    }
    break;

    case TDSRRC_STATE_CELL_FACH:
    {
      switch(current_state)
      {
        case TDSRRC_STATE_CELL_FACH:
          oc_status = tdsrrcllc_update_oc_in_fach_with_msg(dl_sdu_num, msg_ptr);
          break;

        case TDSRRC_STATE_CONNECTING:
          oc_status = tdsrrcllc_update_oc_from_connecting_to_fach(dl_sdu_num, msg_ptr);
          break;

        case TDSRRC_STATE_CELL_DCH:
          oc_status = tdsrrcllc_update_oc_from_dch_to_fach_with_msg(dl_sdu_num, msg_ptr);
          break;


        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"State %d to Cell_FACH transition not supported", current_state);
          oc_status = TDSORDERED_CONFIG_CONFIGURATION_INVALID;
          break;
      }
    }
    break;


  case TDSRRC_STATE_CELL_PCH:
  case TDSRRC_STATE_URA_PCH:
      switch(current_state)
      {
        case TDSRRC_STATE_CELL_FACH:
          oc_status = tdsrrcllc_update_oc_from_fach_to_pch_with_msg(dl_sdu_num,
                                                                 msg_ptr,
                                                                 next_state);
          break;

        case TDSRRC_STATE_CELL_DCH:
          oc_status = tdsrrcllc_update_oc_from_dch_to_pch_with_msg(dl_sdu_num,
                                                                msg_ptr,
                                                                next_state);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"State %d to PCH transition not supported", current_state);
          oc_status = TDSORDERED_CONFIG_CONFIGURATION_INVALID;
          break;
      }
      break;


    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid next state %d for TDSORDERED_CONFIG", next_state);
      oc_status = TDSORDERED_CONFIG_CONFIGURATION_INVALID;
      break;

  } /* end switch(next_state) */

  if((oc_status != TDSORDERED_CONFIG_SET) &&
     (oc_status != TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG))
  {
    /* This implies Ordered Config is not being used - update the
     * config pointer in use
     */
    tdsconfig_ptr_in_use = TDSCURRENT_CONFIG;
  }


  if(oc_status == TDSORDERED_CONFIG_SET)
  {
    tdsordered_config.set_by_proc = proc_id;
    /* Update OC process state */
    tdsordered_config.process_state = TDSLL_NOT_CONFIG_WITH_OC;
    /* This variable indicates that SMC needs to be notified when
     * a reconfiguration is completed.
     */
    tdsordered_config.notify_smc = FALSE;

#ifdef FEATURE_TDSCDMA_TVM_BLOCK
  /* Block TVM if Handover */
  if((proc_id == TDSRRC_PROCEDURE_RBE) ||
     (proc_id == TDSRRC_PROCEDURE_RBRC) ||
     (proc_id == TDSRRC_PROCEDURE_RBR) ||
     (proc_id == TDSRRC_PROCEDURE_TCR) ||
     (proc_id == TDSRRC_PROCEDURE_PCR)
  )
  {
    if ((TRUE == tdsordered_config_ptr->is_hho) &&
        (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH) &&
        (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH) &&
        (TRUE == tdsrrctvm_tvm_identity_exist()))
    {
      uint16 time_interval = 0;
      time_interval = tdsrrc_calculate_block_tvm_time();
      tdsrrctvm_send_block_tvm_cmd(time_interval);
    }
  }
#endif
    TDSRRC_MSG1(MSG_LEGACY_MED,"OC set by proc %d", proc_id);
  }
  if(oc_status != TDSORDERED_CONFIG_SET)
  {
    /* Ordered config is not used,  so initialise the ordered RB-Mapping database*/
    tdsrrcllc_init_ordered_hanging_rb_mapping_info();
    /* In case of OC NOT set reset SVTHHO */
    tdsrrcsmc_reset_start_value_to_transmit_cs_hho(TRUE);
  }

  tdsrrc_ordered_state = TDSRRC_STATE_MAX;
  return (oc_status);

} /* end tdsrrcllc_set_ordered_config() */

/*====================================================================
FUNCTION: tdsrrcllc_swap_oc_and_cc

DESCRIPTION:
  This function swaps tdsordered_config_ptr and current_config_ptrs
  without altering the tdsordered_config.set_status.
  This could be called in place of tdsrrcllc_clear_ordered_config
  when the tdsordered_config.set_status needs to be preserved.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_swap_oc_and_cc(void)
{
  tdsordered_config_type *tmp_config_ptr;
  /* Make Ordered Config as Current Config and
   * Current Config as Ordered Config - so that at the next
   * reconfiguration, the data pointed to by the tdsordered_config
   * before calling this function is not disturbed.
   */
  tmp_config_ptr = tdscurrent_config_ptr;
  tdscurrent_config_ptr = tdsordered_config_ptr;
  tdsordered_config_ptr = tmp_config_ptr;
  tdsconfig_ptr_in_use = TDSCURRENT_CONFIG;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Swap OC and CC");

}

/*====================================================================
FUNCTION: tdsrrcllc_clear_ordered_config

DESCRIPTION:
  This is called when the TDSORDERED_CONFIG variable needs to be cleared.
  That means the Ordered Config and Current Config pointers are swapped.

  This function puts control of TDSORDERED_CONFIG in the hands of the
  individual procedures. Each procedure can then decide whether or not
  they wish to wait for a layer 2 ack before clearing or just clearing
  after reciept of an lower layer configuration message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_clear_ordered_config
(
  void
)
{
  /* Check with LLC if this is the right time to clear ordered config */
  if(tdsrrcllc_can_oc_be_cleared_now())
  {
    tdsrrcllc_clear_ordered_config_now();
  }
  /* If this is not the correct time to clear ordered config,
   * LLC will do it when it is done with the command under process
   */

  return;
} /* end tdsrrcllc_clear_ordered_config() */

/*====================================================================
FUNCTION: tdsrrcllc_get_toc_usage()

DESCRIPTION:
  This function checks returns the toc_usage status

DEPENDENCIES:
  None.

RETURN VALUE:
  toc_usage status

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_toc_usage_e_type tdsrrcllc_get_toc_usage(void)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TOC usage %d", tdstransition_config.toc_usage);
  return tdstransition_config.toc_usage;
} /*tdsrrcllc_get_toc_usage */
 /*====================================================================
 FUNCTION: tdsrrcllc_get_ordered_config_process_state()
 
 DESCRIPTION:
   This function returns info on whether or not lower layers have been
   configured with the information in odered config.
 
 DEPENDENCIES:
   None.
 
 RETURN VALUE:
   Returns a value that indicates if lower layers have been set with the
   info in ordered config.
 
 SIDE EFFECTS:
   None.
 ====================================================================*/
 tdsrrcllc_oc_process_state_e_type tdsrrcllc_get_ordered_config_process_state( void )
 {
   TDSRRC_MSG2(MSG_LEGACY_HIGH,"Get OC process state %d proc %d",
            tdsordered_config.process_state, tdsordered_config.set_by_proc);
   /* return the status of ordered_config.process_state variable */
   return tdsordered_config.process_state;
 
 } /* end rrcllc_get_ordered_process_state() */

 /*====================================================================
 FUNCTION: tdsrrcllc_reconfig_in_progress()
 
 DESCRIPTION:
   This function returns info on whether or not a reconfiguration is
   currently in progress.
 
 DEPENDENCIES:
   None.
 
 RETURN VALUE:
   Returns a value that indicates if a reconfiguration is in progress.
 
 SIDE EFFECTS:
   None.
 ====================================================================*/
 boolean tdsrrcllc_reconfig_in_progress( void )
 {
   TDSRRC_MSG2(MSG_LEGACY_HIGH,"Get reconfig in progress %d proc %d",
            tdsordered_config.reconfig_in_progress, tdsordered_config.set_by_proc);
   /* return the status of ordered_config.process_state variable */
   return tdsordered_config.reconfig_in_progress;
 
 } /* end rrcllc_reconfig_in_progress() */


 /*====================================================================
FUNCTION: tdsrrcllc_get_ordered_config_state_and_proc()

DESCRIPTION:
  This function returns oc status. It also indicates which process
  has set the OC and process status.

DEPENDENCIES:
  None.

RETURN VALUE:
  Returns a value that indicates if Ordered Config is set for
  an RRC state or not set.

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_set_status_e_type tdsrrcllc_get_ordered_config_state_and_proc(
                                                   tdsrrc_proc_e_type *proc_ptr,
                                                   tdsrrcllc_oc_process_state_e_type *process_status_ptr
                                                    )
{

  TDSRRC_MSG3(MSG_LEGACY_MED,"Get OC set_status %d proc %d proc_st %d",
           tdsordered_config.set_status, tdsordered_config.set_by_proc,
           tdsordered_config.process_state);
  *proc_ptr           = tdsordered_config.set_by_proc;
  *process_status_ptr = tdsordered_config.process_state;
  /* return the status of tdsordered_config.process_state variable */
  return tdsordered_config.set_status;

} /* end tdsrrcllc_get_ordered_config_state_and_proc() */



 /*====================================================================
FUNCTION: tdsrrcllc_reset_ordered_config()

DESCRIPTION:
  This function is used to reset the OC

DEPENDENCIES:
  None.

RETURN VALUE:
  Returns a value that indicates if Ordered Config is set for
  an RRC state or not set.

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_set_status_e_type tdsrrcllc_reset_ordered_config(
                                                   tdsrrc_proc_e_type proc_id
                                                   ,boolean reset_toc

                                                    )
{

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Reset OC state %d proc %d proc_st %d",
           tdsordered_config.set_status, tdsordered_config.set_by_proc,
           tdsordered_config.process_state);

  if (TDSOC_NOT_SET == tdsordered_config.set_status)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC is not set");
  }
  if (tdsordered_config.process_state != TDSLL_CONFIG_WITH_OC)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reseting OC while ll configured");
  }
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Reset OC status:%d, proc:%d requested proc:%d",
           tdsordered_config.set_status,tdsordered_config.set_by_proc, proc_id);
  tdsordered_config.set_status    = TDSOC_NOT_SET;
  tdsordered_config.process_state = TDSLL_NOT_CONFIG_WITH_OC;
  tdsordered_config.set_by_proc   = TDSRRC_PROCEDURE_NONE;
  /* Clear the boolean */
  tdsordered_config.notify_smc    = FALSE;

  if ( TDSRRC_STATE_CELL_DCH == tdsrrc_get_state()
      && reset_toc                                        
       ) 
  {
     tdstransition_config.toc_usage = TDSTOC_INVALID;
  }
  else if (!reset_toc) 
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting TOC usage for OOS");

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU

    if( tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH) 
    {
      tdstransition_config.toc_usage = TDSTOC_FOR_OOS_WITHOUT_DCH_INFO;
    }
    else if(tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH_WITH_DCH_INFO)
    {
      tdstransition_config.toc_usage = TDSTOC_FOR_OOS_WITH_DCH_INFO;
    }
    else
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
    {
      tdstransition_config.toc_usage = TDSTOC_FOR_OOS;
    }
  }
  tdsconfig_ptr_in_use = TDSCURRENT_CONFIG;
  return tdsordered_config.set_status;
} /* end tdsrrcllc_reset_ordered_config() */

/*===================================================================
                         INTERNAL FUNCTIONS
=====================================================================*/
/* These functions are declared in tdsrrcllcoc.h */

/*====================================================================
FUNCTION: tdsrrcllc_init_rach_logical_channel_lists()

DESCRIPTION:
  This function initializes the RACH logical channel lists in the RLC
  size restriction info of the given database.
  Pointer to the database is passed as a parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void  tdsrrcllc_init_rach_logical_channel_lists(tdsordered_config_type *config_ptr)
{
  int i;
  tdsrrcllcoc_logical_channel_list_type *rec = NULL;

  for(i = 0; i < TDSUE_MAX_TF; i++)
  {
    rec = &(config_ptr->rach_rlc_size_restriction_info.logical_channel_list[i]);
    rec->restriction_type = TDSRLC_SIZES_UNCONFIGURED;
    rec->count = 0;
    memset(rec->rb_id, TDSRRC_INVALID_RB_ID, sizeof(rec->rb_id));
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_rach_rlc_size_lists()

DESCRIPTION:
  This function initializes the RACH rlc size lists in the RLC
  size restriction info of the given database.
  Pointer to the database is passed as a parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void  tdsrrcllc_init_rach_rlc_size_lists(tdsordered_config_type *config_ptr)
{
  int i;
  tdsrrcllcoc_rlc_size_list_type *rec = NULL;

  for(i = 0; i < MAX_DED_LOGCHAN_PER_TRCH; i++)
  {
    rec = &(config_ptr->rach_rlc_size_restriction_info.rlc_size_list[i]);
    rec->rb_id = TDSRRC_INVALID_RB_ID;
    rec->restriction_type = TDSRLC_SIZES_UNCONFIGURED;
    rec->count = 0;
    memset(rec->size_idx, 0xff, sizeof(rec->size_idx));
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_reset_rach_rlc_size_lists()

DESCRIPTION:
  This function initializes the RACH rlc size lists in the RLC
  size restriction info of the given database.
  Pointer to the database is passed as a parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_reset_rach_rlc_size_lists(
  tdsrrcllcoc_rlc_size_list_type *rec,
  uint8                        entry_to_remove,
  boolean remove_all
)
{
  int i;

  for(i = 0; i < MAX_DED_LOGCHAN_PER_TRCH; i++)
  {
    /*Restore a particular RB*/
    if((remove_all == FALSE) && (i != entry_to_remove))
    {
      continue;
    }
    rec[i].rb_id = TDSRRC_INVALID_RB_ID;
    rec[i].restriction_type = TDSRLC_SIZES_UNCONFIGURED;
    rec[i].count = 0;
    memset(rec[i].size_idx, 0xff, sizeof(rec[i].size_idx));
  }
}

#ifdef FEATURE_UMTS_PDCP
/*====================================================================
FUNCTION: tdsrrcllc_init_pdcp_parms()

DESCRIPTION:
  This function initializes all PDCP related information.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_pdcp_parms(tdsordered_config_type *config_ptr)
{
  uint32 i;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initializing PDCP parms");
  config_ptr->pdcp_parms.num_rbs = 0;

  for(i=0; i < MAX_RAB_TO_SETUP; i++)
  {
    config_ptr->pdcp_parms.pdcp_config_req[i].in_use = FALSE;
    config_ptr->pdcp_parms.pdcp_config_req[i].pdcp_info_changed = FALSE;
    config_ptr->pdcp_parms.pdcp_config_req[i].rb_id = TDSRRC_INVALID_RB_ID;
    memset(&(config_ptr->pdcp_parms.pdcp_config_req[i].pdcp_cfg_req), 0, sizeof(pdcp_cfg_req_data_type)); 
    config_ptr->pdcp_parms.pdcp_config_req[i].pdcp_cfg_req.action = MAX_ACTION_PDCP;
    config_ptr->pdcp_parms.pdcp_config_req[i].pdcp_cfg_req.rb_id = TDSRRC_INVALID_RB_ID;
  }
}
#endif /* FEATURE_UMTS_PDCP*/

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION        tdsrrcllc_init_srb5_info()

DESCRIPTION     This function initializes SRB#5 data structures 
                with default values.

DEPENDENCIES    NONE

RETURN VALUE    void

SIDE EFFECTS
====================================================================*/
void tdsrrcllc_init_srb5_info(tdsordered_config_type *config_ptr)
{
  memset((void *)&(config_ptr->srb5_info), 0, sizeof(tdsrrcllc_srb5_info_type));
  config_ptr->srb5_info.srb5_rb_exists   = FALSE;
  config_ptr->srb5_info.rb_id            = TDSRRCLLC_INVALID_RB_ID;
  config_ptr->srb5_info.srb5_trch_exists = FALSE;
  config_ptr->srb5_info.trch_id          = TDSRRCLLC_INVALID_TRCH_ID;
  config_ptr->srb5_info.lc_id            = TDSRRCLLC_INVALID_LC_ID;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cleared SRB#5 info in CC/OC");
} /* tdsrrcllc_init_srb5_info */

/*====================================================================
FUNCTION        tdsrrcllc_has_config_srb5()

DESCRIPTION     This function tells if the passed RB Id maps to SRB#5 
                or not.

DEPENDENCIES    NONE

RETURN VALUE    boolean
                TRUE: If RB Id is same as SRB#5 Id.
                FALSE: otherwise

SIDE EFFECTS
====================================================================*/
boolean tdsrrcllc_has_config_srb5
(
  tdsrrcllc_config_e_type config_type, 
  uint16 rb_id
)
{
  boolean srb5_status = FALSE;
  tdsordered_config_type *config_ptr = NULL;

  if(TDSORDERED_CONFIG == config_type)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else if(TDSCURRENT_CONFIG ==  config_type)
  {
    config_ptr = tdscurrent_config_ptr;
  }
  else if(TDSTRANSITION_CONFIG == config_type)
  {
    config_ptr = tdstransition_config.toc_ptr;
  }

  if(config_ptr != NULL)
  {
    if(config_ptr->srb5_info.srb5_rb_exists == TRUE &&
       config_ptr->srb5_info.rb_id == rb_id)
    {
      srb5_status = TRUE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Config has SRB#5");
    }
  }

  return srb5_status; 
} /* tdsrrcllc_has_config_srb5 */
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

#ifdef FEATURE_TDSCDMA_CPC_DTX
/*====================================================================
FUNCTION: tdsrrcllc_init_cpc_dtx_params()

DESCRIPTION:
  This function initializes the CPC DTX config database.
  Pointer to the database is passed as a parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_cpc_dtx_params
(
  tdsordered_config_type *config_ptr
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:Clear dtx config from config ptr = 0x%x",config_ptr);
  config_ptr->l1_cpc_dtx_req_mask= L1_CPC_DTX_NO_OP;
  tdsrrcllc_init_dtx_params(config_ptr);
}
#endif
#ifdef FEATURE_TDSCDMA_CPC_DRX
/*====================================================================
FUNCTION: tdsrrcllc_init_cpc_drx_params()

DESCRIPTION:
  This function initializes the CPC DRX config database.
  Pointer to the database is passed as a parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_cpc_drx_params
(
  tdsordered_config_type *config_ptr
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:Clear drx config from config ptr = 0x%x",config_ptr);
  config_ptr->l1_cpc_drx_req_mask= L1_CPC_DRX_NO_OP;
  tdsrrcllc_init_drx_info_before_processing(config_ptr);
}
#endif

/*====================================================================
FUNCTION: tdsrrcllc_init_config_data()

DESCRIPTION:
  This function initializes the config database.
  Pointer to the database is passed as a parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_config_data(
  tdsordered_config_type *config_ptr,
  tdsrrc_state_e_type  rrc_state
  )
{

  if(config_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CC/OC/TOC already freed.");
    return;
  }

  /* Initialize all pointers in L1, MAC and RLC structures */
  tdsrrcllc_init_mac_dl_ptrs(config_ptr);
  tdsrrcllc_init_mac_ul_ptrs(config_ptr);
  tdsrrcllc_init_l1_dl_ptrs(config_ptr);
  tdsrrcllc_init_l1_ul_ptrs(config_ptr);
  tdsrrcllc_init_rlc_ptrs(config_ptr);

  /* Initialize RNTI (C-RNTI and U-RNTI) info */
  tdsrrcllc_init_rnti_info(config_ptr);

  /*** Initialize Downlink Info ***/
  /* Initialize BCH Info */
  tdsrrcllc_init_bch_info(config_ptr);
  /* Initialize PCH Info */
  tdsrrcllc_init_pch_info(config_ptr);
  /* Initialize FACH Info */
  tdsrrcllc_init_fach_info(config_ptr, TRUE);
  /* Initialize DL DCH  Info */
  tdsrrcllc_init_dl_dch_info(config_ptr);
  /* Initialize Downlink CCTrCH Info */
  tdsrrcllc_init_dl_cctrch_info(config_ptr);
  /* Initialize Downlink CTFC Info */
  tdsrrcllc_init_dl_ctfcs(config_ptr);
#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrcllc_init_dl_ctfcs_for_sccpch_with_ctch(config_ptr);
  tdsrrcllc_init_cbs_info(config_ptr);
#endif
#endif
  /* Initialize Downlink DPCH Channel Info */
  tdsrrcllc_init_l1_dl_dpch_chan_info(config_ptr);
  /* Initialize Downlink SCCPCH Channel Info */
  tdsrrcllc_init_l1_sccpch_chan_info(config_ptr);
  /* Initialize Downlink PCCPCH Channel Info */
  config_ptr->l1_dl_chan_parms.pccpch.tstd_indicator = TRUE;
    

  /*** Initialize Uplink Info ***/
  /* Initialize RACH Info */
  tdsrrcllc_init_rach_info(config_ptr, TRUE);
  /* Initialize Uplink DCH Info */
  tdsrrcllc_init_ul_dch_info(config_ptr);
  /* Initialize Uplink DPCH Channel Info */
  tdsrrcllc_l1_ul_dpch_chan_info(config_ptr);
  /*** Initialize RLC Logical Channel Info ***/
  /* Initialize RLC Info */
  if(rrc_state == TDSRRC_STATE_CONNECTING)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Current DL TM in OC nchan %d and DL TM nchan with LCM is %d",
                  config_ptr->rlc_dl_tm_parms.nchan,tdsrrclcm_get_nchan_dl_tm());
  }
  tdsrrcllc_init_rlc_parms(config_ptr);
  if(rrc_state == TDSRRC_STATE_CONNECTING)
  {
    config_ptr->rlc_dl_tm_parms.nchan = tdsrrclcm_get_nchan_dl_tm();
  }
  /* Initialize the Activation Time */
  tdsrrcllc_init_activation_time(config_ptr);

  /* Invalidate RM Restriction Info */

  /* Initaialize Reconfig needed */
  tdsrrcllc_init_reconfig_needed(config_ptr);

  /***************************************************/
  /* Initialize Transport Channel Counts for UL & DL */
  /***************************************************/
  /* The ordered RRC State */
  config_ptr->rrc_state_indicator = rrc_state;
  /*initialize the destination freq */
  config_ptr->p_freq_present = FALSE;
  config_ptr->s_freq_present = FALSE;
  config_ptr->is_hho = FALSE;
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  config_ptr->harq_present = FALSE;
#endif

  /* Initialize the CS and PS Drx Cycle Length */
  config_ptr->cs_drx_cycle_length = 0;
  config_ptr->ps_drx_cycle_length = 0;
  config_ptr->utran_drx_cycle_length = 0;


  /* Initialize RLC size restriction info */
  tdsrrcllc_init_rach_rlc_size_lists(config_ptr);
  tdsrrcllc_init_rach_logical_channel_lists(config_ptr);


  tdsrrcllc_init_hsdpa_l1_info(config_ptr);
  tdsrrcllc_init_hsdpa_mac_info(config_ptr);
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Set HRNTI TDSINVALID");
  }
  tdsrrcllc_set_hrnti_status(config_ptr,TDSHRNTI_INVALID);

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  tdsrrcllc_init_hsdpa_mac_ehs_info(config_ptr);
#endif

#ifdef FEATURE_TDSCDMA_HSUPA
  if (TDSRRC_STATE_DISCONNECTED == rrc_state)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: State: TDSRRC_STATE_DISCONNECTED, hence resetting mac-e, e-dl, e-ul information");
    tdsrrcllc_init_mac_e_info(config_ptr);
    tdsrrcllc_init_l1_e_ul_info(TRUE, config_ptr);
    tdsrrcllc_init_l1_e_dl_info(config_ptr);
    tdsrrcllc_init_l1_non_sched_grant_info(config_ptr);
    
    if ((config_ptr->l1_e_req_mask != TDSRRCLLC_EUL_NOOP) || (config_ptr->mac_eul_action != TDSMAC_EUL_NOOP))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCEUL: DEBUG l1_e_req_mask: %d, mac_eul_action: %d", 
        config_ptr->l1_e_req_mask, config_ptr->mac_eul_action);
    }
    config_ptr->l1_e_req_mask = TDSRRCLLC_EUL_NOOP;
    config_ptr->hsupa_start_mask = 0;
    config_ptr->mac_eul_action = TDSMAC_EUL_NOOP;    
    /* Reset the current codec to the default mode */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"reset the AMR codec status");
    tdsrrc_set_current_codec_mode(TDSCDMA_IVOCODER_ID_AMR);
    tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCEUL: State: %d <not IDLE>, resetting selective eul information, cc(e_dch_tx)=%d", 
      rrc_state, tdscurrent_config_ptr->e_dch_transmission);
    tdsrrcllc_init_mac_e_info(config_ptr);
    tdsrrcllc_init_l1_e_ul_info(FALSE, config_ptr);
    tdsrrcllc_init_l1_e_dl_info(config_ptr);
    tdsrrcllc_init_l1_non_sched_grant_info(config_ptr);
  }
#endif /* FEATURE_TDSCDMA_HSUPA */

#ifdef FEATURE_UMTS_PDCP
  tdsrrcllc_init_pdcp_parms(config_ptr);
#endif /* FEATURE_UMTS_PDCP */

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT 
  tdsrrcllc_init_srb5_info(config_ptr);
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

#ifdef FEATURE_TDSCDMA_MIMO
  tdsrrcllc_init_mimo_params(config_ptr);
#endif

#ifdef FEATURE_TDS_DC_HSDPA
  /*Notice that we will reset the global tdsrrc_dc_hsdpa_status also*/
  tdsrrcllc_init_dc_hsdpa_params(config_ptr);
#endif /*FEATURE_TDSCDMA_DC_HSDPA*/

  config_ptr->mac_ul_parms.new_tfcs_list_present = FALSE;

#ifdef FEATURE_TDSCDMA_CPC_DTX
/* init cpc-DTX-DRX ptrs here*/
tdsrrcllc_init_cpc_dtx_params(config_ptr);
#endif
#ifdef FEATURE_TDSCDMA_CPC_DRX
  tdsrrcllc_init_cpc_drx_params(config_ptr);
#endif

#ifdef FEATURE_TDSCDMA_REL9
#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
  config_ptr->meas_occasion_info_incl = FALSE;
  memset(&config_ptr->meas_occasion_info, 0, sizeof (tdsl1_meas_occasion_info_struct_type));
#endif
#endif

} /* tdsrrcllc_init_config_data */

/*====================================================================
FUNCTION: tdsrrcllc_init_ordered_config_status_and_data()

DESCRIPTION:
  This function initializes the tdsordered_config database. It sets up
  the tdsordered_config_ptr and tdscurrent_config_ptr, as well as
  initializing all data.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_ordered_config_status_and_data(
  tdsrrc_state_e_type  rrc_state
  )
{
  uint8 i;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initializing the TDSORDERED_CONFIG Data Base");

  /* Initialize the ordered_reconfiguration to FALSE, i.e. no pending
  * ORDERED_RECONFIGURATION.
  */
  tdsordered_config.set_status = TDSOC_NOT_SET;
  tdsordered_config.notify_smc = FALSE;
  tdsordered_config.process_state = TDSLL_NOT_CONFIG_WITH_OC;
  tdsordered_config.set_by_proc = TDSRRC_PROCEDURE_NONE;
  tdsconfig_ptr_in_use = TDSCURRENT_CONFIG;

  /* Init the Rb_mapping database with default values*/
  for(i=0;i<TDSMAX_HANGING_RB_MAPPING_INFO;i++)
  {
    memset(&tdsrrc_current_hanging_rb_mapping[i], 0, sizeof(tdsrb_mapping_info_type));
    tdsrrc_current_hanging_rb_mapping[i].in_use = FALSE;  
  }
  tdsrrcllc_init_ordered_hanging_rb_mapping_info();

  if(tdsordered_config_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC already freed.");
  }
  else
  {
    tdsrrcllc_init_config_data(tdsordered_config_ptr, rrc_state);
  }
  return;
} /* end tdsrrcllc_init_ordered_config_status_and_data() */

/*====================================================================
FUNCTION: tdsrrcllc_init_ordered_config_mac_info()

DESCRIPTION:
  This function initializes the MAC Info in Config database.
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_ordered_config_mac_info
(
  tdsrrcllc_config_e_type config_type
)
{
  tdsordered_config_type  *config_ptr;
  /* This function effectively sets all kinds of
   * Downlink and Uplink Transport Channels in MAC
   * to 0. It also invalidates RNTI info.
   */
  if(config_type == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }

  tdsrrcllc_init_mac_dl_ptrs(config_ptr);
  tdsrrcllc_init_mac_ul_ptrs(config_ptr);
  /* Initialize RNTI (C-RNTI and U-RNTI) info */
  tdsrrcllc_init_rnti_info(config_ptr);

  /*** Initialize Downlink Info ***/
  /* Initialize BCH Info */
  tdsrrcllc_init_bch_info(config_ptr);
  /* Initialize PCH Info */
  tdsrrcllc_init_pch_info(config_ptr);
  /* Initialize FACH Info */
  tdsrrcllc_init_fach_info(config_ptr, TRUE);
  /* Initialize DL DCH  Info */
  tdsrrcllc_init_dl_dch_info(config_ptr);


  tdsrrcllc_init_hsdpa_mac_info(config_ptr);


#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  tdsrrcllc_init_hsdpa_mac_ehs_info(config_ptr);
#endif

  /*** Initialize Uplink Info ***/
  /* Initialize RACH Info */
  tdsrrcllc_init_rach_info(config_ptr, TRUE);
  /* Initialize Uplink DCH Info */
  tdsrrcllc_init_ul_dch_info(config_ptr);

#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrcllc_init_mac_e_info(config_ptr);
#endif /* FEATURE_TDSCDMA_HSUPA */

} /* rrcllc_init_ordered_config */

/*====================================================================
FUNCTION: tdsrrcllc_init_rlc_parms()

DESCRIPTION:
  This function initializes the Number of channels and Number of
  entities in RLC parameters to 0.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_rlc_parms(tdsordered_config_type  *config_ptr)
{
  uint8  ix;
  /* Init the local AM data pointers, DL & UL channel counts */
  config_ptr->rlc_am_parms.nentity = 0;

  for(ix = 0; ix < TDSUE_MAX_AM_ENTITY; ix++)
  {
    config_ptr->am_config[ix].ul_nchan =
    config_ptr->am_config[ix].dl_nchan = 0;
    config_ptr->am_config[ix].rlc_ul_am_config.rlc_info.rlc_info_incl =
    config_ptr->am_config[ix].rlc_dl_am_config.rlc_info.rlc_info_incl = FALSE;

    config_ptr->am_config[ix].reestab_entity = TDSRLC_RE_ESTABLISH_NONE;

    tdsrrcllc_init_am_dl_rlc_size(config_ptr,ix);

#ifdef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
  config_ptr->am_config[ix].rlc_ul_am_config.special_value_he_config = 
  config_ptr->am_config[ix].rlc_dl_am_config.special_value_he_config = FALSE;
#endif /*FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE*/
  }

  config_ptr->rlc_dl_tm_parms.nchan = 0;
  config_ptr->rlc_dl_um_parms.nchan = 0;
  config_ptr->rlc_ul_tm_parms.nchan = 0;
  config_ptr->rlc_ul_um_parms.nchan = 0;
  
  for(ix =0; ix < TDSUE_MAX_UL_UM_CHANNEL; ix++)
  {
    config_ptr->rlc_ul_um_parms.chan_info[ix].rb_id = TDSRRCLLC_INVALID_RB_ID ;
    config_ptr->rlc_ul_um_parms.chan_info[ix].timer_discard = 0;
    config_ptr->rlc_ul_um_parms.chan_info[ix].alternate_e_bit = FALSE;
  }
  
  for(ix =0; ix < TDSUE_MAX_DL_UM_CHANNEL; ix++)
  {
    config_ptr->rlc_dl_um_parms.chan_info[ix].rb_id = TDSRRCLLC_INVALID_RB_ID ;
    
    config_ptr->rlc_dl_um_parms.chan_info[ix].alternate_e_bit = FALSE;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"nchan: UL UM set to %d and DL UM %d",
      tdsordered_config_ptr->rlc_ul_um_parms.nchan, 
      tdsordered_config_ptr->rlc_dl_um_parms.nchan);

  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"nchan: UL TM set to %d and DL TM set to %d",
      tdsordered_config_ptr->rlc_ul_tm_parms.nchan, 
      tdsordered_config_ptr->rlc_dl_tm_parms.nchan);

}

/*====================================================================
FUNCTION: tdsrrcllc_init_rnti_info()

DESCRIPTION:
  This function initializes the RNTI info in Uplink and Downlink
  MAC parameter data to NO RNTI VALID.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_rnti_info(tdsordered_config_type  *config_ptr)
{
  config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
  config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
}

/*====================================================================
FUNCTION: tdsrrcllc_init_activation_time()

DESCRIPTION:
  This function initializes the Activation Time to Avtivation Time
  NONE. This is equivalent to saying the Avtivation Time is "Now".

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_activation_time(tdsordered_config_type  *config_ptr)
{
  config_ptr->act_time_type = TDSL1_ACTIVATION_TIME_NONE;
}

/*====================================================================
FUNCTION: tdsrrcllc_init_reconfig_needed()

DESCRIPTION:
  This function initializes the Reconfig Needed structure.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_reconfig_needed(tdsordered_config_type  *config_ptr)
{
  config_ptr->reconfig_needed.downlink_l1 = FALSE;
  config_ptr->reconfig_needed.downlink_mac = FALSE;
  config_ptr->reconfig_needed.uplink_l1 = FALSE;
  config_ptr->reconfig_needed.uplink_mac = FALSE;
}

/*====================================================================
FUNCTION: tdsrrcllc_init_l1_dl_dpch_chan_info()

DESCRIPTION:
  This function initializes the Downlink DPCH related channel info
  that goes to L1.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_l1_dl_dpch_chan_info(tdsordered_config_type  *config_ptr)
{
  uint8  rl_count;

  config_ptr->l1_dl_chan_parms.dpch.dpch_common.opt_parm_present_mask = 0;
  config_ptr->l1_dl_chan_parms.dpch.dpch_common.timing_ind =
    TDSL1_DL_PHYCFG_TIMING_IND_MAINTAIN;
  config_ptr->l1_dl_chan_parms.dpch.dpch_common.tpc_step_size = 
    TDSL1_TPC_STEP_SIZE_0DB;
  config_ptr->l1_dl_chan_parms.dpch.dpch_common.tstd_indicator = TRUE;
  /* Initialize the Downlink RL information. */
  /* Number of PhyChan Radio Links (RLs) */

  for (rl_count = 0; rl_count < TDSUE_MAX_RL; rl_count++)
  {
    tdsrrcllc_init_dl_rl_info(config_ptr, rl_count);
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_l1_sccpch_chan_info()

DESCRIPTION:
  This function initializes the SCCPCH related channel info
  that goes to L1.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_l1_sccpch_chan_info(tdsordered_config_type  *config_ptr)
{

  config_ptr->l1_dl_chan_parms.use_pich = FALSE;

}

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT 
/*====================================================================
FUNCTION: tdsrrcllc_init_l1_sccpch_for_ctch_chan_info()

DESCRIPTION:
  This function initializes the SCCPCH (used for ctch) related channel info
  that goes to L1.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_l1_sccpch_for_ctch_chan_info(tdsordered_config_type  *config_ptr)
{
     config_ptr->l1_dl_chan_parms.sccpch[1].opt_parm_present_mask = 0;

     config_ptr->l1_dl_chan_parms.use_pich = FALSE;

}
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

/*====================================================================
FUNCTION: tdsrrcllc_init_bch_info()

DESCRIPTION:
  This function initializes the BCH transport channel related info.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_bch_info(tdsordered_config_type *config_ptr)
{
  config_ptr->mac_dl_parms.num_bchs = 0;
}

/*====================================================================
FUNCTION: tdsrrcllc_init_pch_info()

DESCRIPTION:
  This function initializes the PCH transport channel related info.
  This also includes releasing the PCH TrCH index and setting "PCH
  Included" to FALSE for LLC.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_pch_info(tdsordered_config_type *config_ptr)
{
  config_ptr->mac_dl_parms.num_pchs = 0;
  tdsrrcllc_init_dl_pch_trch_idx(config_ptr);
  /* Reflect the status of no PCH in LLC */
  tdsrrcllc_set_pch_included(FALSE);
}

/*====================================================================
FUNCTION: tdsrrcllc_init_fach_info()

DESCRIPTION:
  This function initializes the FACH transport channel related info.
  This also includes releasing all FACH TrCH indices, setting
  "FACH Included" to FALSE for LLC and initializing the FACH RB
  Mapping Info.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_fach_info(tdsordered_config_type *config_ptr, boolean init_rb_mapping_info)
{
  uint8  trch_index;

  config_ptr->mac_dl_parms.num_fachs = 0;


  tdsrrcllc_init_dl_fach_trch_idx_info(config_ptr);

  /* Reflect the status of no FACH in LLC */
  tdsrrcllc_set_fach_included(FALSE);

  if(init_rb_mapping_info)
  {
    for(trch_index = 0; trch_index < TDSUE_MAX_TRCH; trch_index ++)
    {
      tdsrrcllc_init_fach_rb_mapping_info(config_ptr, trch_index);
    }
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_dl_dch_info()

DESCRIPTION:
  This function initializes the DL DCH transport channel related info.
  This also includes releasing all DL DCH TrCH indices and
  initializing the DL DCH RB Mapping Info.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_dch_info(tdsordered_config_type *config_ptr)
{
  uint8  trch_index;
  config_ptr->mac_dl_parms.num_dchs = 0;

  tdsrrcllc_init_dl_dch_trch_idx_info(config_ptr);

  for(trch_index = 0; trch_index < TDSUE_MAX_TRCH; trch_index ++)
  {
    tdsrrcllc_init_dl_dch_rb_mapping_info(config_ptr, trch_index);
  }
}

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT 
/*====================================================================
FUNCTION: tdsrrcllc_init_cbs_info()

DESCRIPTION:
  This function initializes the CBS related info.  Sets the flag that
  indicated to L1 that ctch info is present to FALSE.


DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_cbs_info(tdsordered_config_type *config_ptr)
{
  uint8  sccpch_count;
  for(sccpch_count = 0; sccpch_count < TDSL1_DL_MAX_SCCPCH; sccpch_count ++)
  {
    config_ptr->l1_dl_chan_parms.sccpch[sccpch_count].cbs_sched.ctch_info_present = FALSE;
  }

}


/*====================================================================
FUNCTION: tdsrrcllc_init_dl_cctrch_info_for_sccpch_with_ctch()

DESCRIPTION:
  This function initializes the DL CCTrCH Info for L1.
  This includes setting the number of transport channels for this
  CCTrCH to 0 and initializing the Transport Format Set.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_cctrch_info_for_sccpch_with_ctch(tdsordered_config_type *config_ptr)
{
  uint8  trch_index;
  config_ptr->l1_dl_cctrch_parms_for_sccpch_with_ctch.num_trch = 0;

  for(trch_index = 0; trch_index < TDSL1_DL_MAX_SCCPCH; trch_index ++)
  {
    tdsrrcllc_init_dl_tfs_table(config_ptr, trch_index);
  }
}


/*====================================================================
FUNCTION: tdsrrcllc_init_dl_ctfcs_for_sccpch_with_ctch()

DESCRIPTION:
  This function initializes the Downlink TFI table. This is
  called whenever the DL CCTrCH type changes (i.e. a state change),
  or when a TrCH is deleted..

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_ctfcs_for_sccpch_with_ctch(tdsordered_config_type *config_ptr)
{
  /* local variables */
  uint8 ix, iy;

  config_ptr->l1_dl_ctfc_parms_for_sccpch_with_ctch.num_ctfc_entry = 0;

  /* Initialize the array count */
  config_ptr->dl_ctfc_for_sccpch_with_ctch.num_ctfc_entry = 0;

  /* Clear out the downlink ctfc */
  for(ix = 0; ix < TDSUE_MAX_TFC; ix++)
  {
    /* Clear out the ctfc array */
    config_ptr->dl_ctfc_for_sccpch_with_ctch.ctfc_entry_data[ix].ctfc_value = 0;
    config_ptr->dl_ctfc_for_sccpch_with_ctch.ctfc_entry_data[ix].state = TDSINVALID;

    for(iy = 0; iy < TDSUE_MAX_TRCH; iy++)
    {
      config_ptr->dl_ctfc_for_sccpch_with_ctch.ctfc_entry_data[ix].tfi_table[iy] = 0;
    }

    /* clear out valid ctfc field */
    config_ptr->l1_dl_ctfc_parms_for_sccpch_with_ctch.ctfc_entry_array[ix]->valid_ctfc = FALSE;
    config_ptr->l1_dl_ctfc_parms_for_sccpch_with_ctch.ctfc_entry_array[ix]->tfi_table = NULL;
  }

  return;
} /* end rrcllc_init_dl_ctfcs_for_sccpch_with_ctch */
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT 
#endif

/*====================================================================
FUNCTION: tdsrrcllc_init_dl_cctrch_info()

DESCRIPTION:
  This function initializes the DL CCTrCH Info for L1.
  This includes setting the number of transport channels for this
  CCTrCH to 0 and initializing the Transport Format Set.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_cctrch_info(tdsordered_config_type *config_ptr)
{
  uint8  trch_index;
  config_ptr->l1_dl_cctrch_parms.num_trch = 0;

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  config_ptr->l1_dl_cctrch_parms_for_sccpch_with_ctch.num_trch = 0;
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

  for(trch_index = 0; trch_index < TDSL1_DL_MAX_TRCH; trch_index ++)
  {
    tdsrrcllc_init_dl_tfs_table(config_ptr, trch_index);
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_dl_ctfcs()

DESCRIPTION:
  This function initializes the Downlink TFI table. This is
  called whenever the DL CCTrCH type changes (i.e. a state change),
  or when a TrCH is deleted..

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_ctfcs(tdsordered_config_type *config_ptr)
{
  /* local variables */
  uint8 ix, iy;

  config_ptr->l1_dl_ctfc_parms.num_ctfc_entry = 0;

  /* Initialize the array count */
  config_ptr->dl_ctfc.num_ctfc_entry = 0;

  /* Clear out the downlink ctfc */
  for(ix = 0; ix < TDSUE_MAX_TFC; ix++)
  {
    /* Clear out the ctfc array */
    config_ptr->dl_ctfc.ctfc_entry_data[ix].ctfc_value = 0;
    config_ptr->dl_ctfc.ctfc_entry_data[ix].state = TDSINVALID;

    for(iy = 0; iy < TDSUE_MAX_TRCH; iy++)
    {
      config_ptr->dl_ctfc.ctfc_entry_data[ix].tfi_table[iy] = 0;
    }

    /* clear out valid ctfc field */
    config_ptr->l1_dl_ctfc_parms.ctfc_entry_array[ix]->valid_ctfc = FALSE;
    config_ptr->l1_dl_ctfc_parms.ctfc_entry_array[ix]->tfi_table = NULL;
  }

  return;
} /* end tdsrrcllc_init_dl_ctfcs */
/*====================================================================
FUNCTION: tdsrrcllc_l1_ul_dpch_chan_info()

DESCRIPTION:
  This function initializes the UL DPCH Physical Channel Related Info.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_l1_ul_dpch_chan_info(tdsordered_config_type  *config_ptr)
{
  config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est= FALSE;
  config_ptr->l1_ul_chan_parms.dpch_cfg.tpc_step_size = TDSL1_TPC_STEP_SIZE_0DB;
  config_ptr->l1_ul_chan_parms.dpch_cfg.prx_dpch_des= 0;
  config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_parm_present = FALSE;
  config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_present = FALSE;
  config_ptr->l1_ul_chan_parms.dpch_cfg.ul_timing_advance_ctrl_enabled = FALSE;
  config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_freq = 0;
  config_ptr->l1_ul_chan_parms.dpch_cfg.ul_sync_step_size = 0;
  config_ptr->l1_ul_chan_parms.dpch_cfg.num_ts = 0;
}

/*====================================================================
FUNCTION: tdsrrcllc_init_rach_info()

DESCRIPTION:
  This function initializes the RACH Transport Channel Related Info.
  Since there can be only one Uplink Physical Channel at any time,
  initializing RACH info also results in initialization of:
  - MAC UL CCTrCH type to TDSINVALID
  - Number of TrCHs in Uplink TFCS
  - Number of TrCHs in L1 Uplink CCTrCH parameters
  - Uplink TFCS table
  - Uplink TFS for all TrCHs.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_rach_info(tdsordered_config_type *config_ptr, boolean init_rb_map_info)
{
  uint8  trch_index;

  config_ptr->mac_ul_parms.cctrch_ptr->cctrch_type = TDSINVALID_UL_CCTRCH;

  /* Since RACH Info is being initialized, initialize number of
   * Transport channels in TFCS Info and L1 UL CCTrCH parameters to 0
   */
  config_ptr->mac_ul_parms.tfcs_ptr->nchan = 0;
  /* config_ptr->l1_ul_cctrch_parms.num_trch = 0; */

  /* Release the RACH TrCH Index and initialize local info */
  tdsrrcllc_init_ul_rach_trch_idx_info(config_ptr);
  if (init_rb_map_info)
  {
    /* Initialize RACH RB Mapping Info */
    tdsrrcllc_init_rach_rb_mapping_info(config_ptr);
  }

  /* Initialize the TFCS Info */
  tdsrrcllc_init_ul_tfi_table(config_ptr);
  
  /* Initialize the UL TFS info for all transport channels */
  for(trch_index = 0; trch_index < TDSUE_MAX_TRCH; trch_index ++)
  {
    tdsrrcllc_init_ul_tfs_table(config_ptr, trch_index);
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_ul_dch_info()

DESCRIPTION:
  This function initializes the UL DCH Transport Channel Related Info.
  Since there can be only one Uplink Physical Channel at any time,
  initializing UL DCH info also results in initialization of:
  - MAC UL CCTrCH type to TDSINVALID
  - Number of TrCHs in Uplink TFCS
  - Number of TrCHs in L1 Uplink CCTrCH parameters
  - Uplink TFCS table
  - Uplink TFS for all TrCHs.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_ul_dch_info(tdsordered_config_type *config_ptr)
{
  uint8  trch_index;

  config_ptr->mac_ul_parms.cctrch_ptr->cctrch_type = TDSINVALID_UL_CCTRCH;
  /* Initialize number of UL DCHs to 0 */
  config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs = 0;

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  /* Initialize loopback mode */
  config_ptr->mac_ul_parms.cctrch_ptr->dch_info.loopback_test_mode =
    TDSMAC_LOOPBACK_NA;
#endif

  /* Initialize Loopback mode 2 info */
#ifdef FEATURE_DUMMY_SIGNALING
  #error code not present
#endif
  /* Set the number of channels in TFCS to 0 */
  config_ptr->mac_ul_parms.tfcs_ptr->nchan = 0;
  /* Set number of TrCHs in L1 to 0 */
  /* config_ptr->l1_ul_cctrch_parms.num_trch = 0; */
  config_ptr->ul_tfcs.nchan = 0;

  config_ptr->rrc_ul_cctrch_params.num_trch =0;


  /* Release all UL DCH TrCH indices and initialize the local info */
  tdsrrcllc_init_ul_dch_trch_idx_info(config_ptr);

  /* Initialize RB Mapping info for all transport channels */
  for(trch_index = 0; trch_index < TDSUE_MAX_TRCH; trch_index ++)
  {
    tdsrrcllc_init_ul_dch_rb_mapping_info(config_ptr, trch_index);
  }

  /* Initialize the TFCS Info */
  tdsrrcllc_init_ul_tfi_table(config_ptr);

  /* Initialize the UL TFS info for all transport channels */
  for(trch_index = 0; trch_index < TDSUE_MAX_TRCH; trch_index ++)
  {
    tdsrrcllc_init_ul_tfs_table(config_ptr, trch_index);
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_dl_rl_info()

DESCRIPTION:
  This function initializes the Downlink Radio Link Info.
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_rl_info(tdsordered_config_type *config_ptr, uint8 ix)
{
  /* local loop variable */
  uint8 iy;


  /*Since we remove the CPID at index ix, we need to remove the Cell_id also at same index ix.*/
  tdscell_id_per_rl[ix] = TDSINVALID_CELL_ID_OTA;    


  /* RL info */
  config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.common_ts_info.second_interleaving_mode = TDSL1_SEC_INTLV_FRAME_BASED;
  config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.common_ts_info.tfci_coding = 0;
  config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.common_ts_info.punct_limit = 0;
  config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.common_ts_info.repetition_period = 0;
  config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.common_ts_info.repetition_length = 0;
  config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.time_info.activation_time = 0;
  config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.num_ts = 0;

  /* Channelization code info */
  for (iy=0; iy < TDSL1_DL_MAX_NUM_TS; iy++)
  {
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].ts_num = 0;
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].midamble_ss_burst_type.allocation_mode = TDSL1_DEFAULT_MIDAMBLE;
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].midamble_ss_burst_type.config = TDSL1_NUM_MIDAMBLE_CONFIG;
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].midamble_ss_burst_type.shift = 0xFF;
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].ss_tpc_symbols = TDSL1_NUM_TPC_SYMBOL_CFG;
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].additional_ss_tpc_symbols = 0;
    config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.individual_ts_info[iy].chan_code_bitmap = 0;
  }

  return;
} /* end tdsrrcllc_init_dl_rl_info() */

/*====================================================================
FUNCTION: tdsrrcllc_init_dl_tfs_table()

DESCRIPTION:
  This function initializes the Downlink TrCH TFS table. This
  is called whenever the UL CCTrCH type changes (i.e. a state change),
  or when a TrCH is deleted.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_tfs_table(tdsordered_config_type *config_ptr, uint8 ix)
{
  /* local variables */
  uint8 iy;

  config_ptr->dl_trch_info[ix].num_tf = 0;
  config_ptr->dl_trch_info[ix].trch_id = 0;
  config_ptr->dl_trch_info[ix].tti_value = TDSL1_NUM_TTI;
  config_ptr->dl_trch_info[ix].crc_length = TDSL1_NUM_CRC_LENGTH;
  config_ptr->dl_trch_info[ix].coding_type = TDSL1_DL_UNCODED;
  config_ptr->dl_trch_info[ix].rate_match_attrib = 0;
  config_ptr->dl_trch_info[ix].ref_type = TDSL1_DL_TRCH_INVALID;
#ifdef FEATURE_TC
  config_ptr->dl_trch_info[ix].loopback_mode = TDSL1_LOOPBACK_NA;
#endif

  for(iy = 0; iy < TDSUE_MAX_TF; iy++)
  {
    config_ptr->dl_trch_info[ix].tf_info_array[iy]->num_tb  = 0;
    config_ptr->dl_trch_info[ix].tf_info_array[iy]->tb_size = 0;
  }
#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/* clear the trch info for sccpch with ctch */
  config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].num_tf = 0;
  config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].trch_id = 0;
  config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].tti_value = TDSL1_NUM_TTI;
  config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].crc_length = TDSL1_NUM_CRC_LENGTH;
  config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].coding_type = TDSL1_DL_UNCODED;
  config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].rate_match_attrib = 0;
  config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].ref_type = TDSL1_DL_TRCH_INVALID;
  config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].loopback_mode = TDSL1_LOOPBACK_NA;

  for(iy = 0; iy < TDSL1_DL_MAX_TF; iy++)
  {
    config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].tf_info_array[iy]->num_tb  = 0;
    config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].tf_info_array[iy]->tb_size = 0;
    config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].tf_info_array[iy]->allowed = FALSE;
  }
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif
  return;
} /* end rrcllc_dl_tfs_table() */

/*====================================================================
FUNCTION: tdsrrcllc_init_ul_tfs_table()

DESCRIPTION:
  This function initializes the Uplink TFS table. This is
  called whenever the UL CCTrCH type changes (i.e. a state change),
  or when a TrCH is deleted.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_ul_tfs_table(tdsordered_config_type *config_ptr, uint8 ix)
{
  /* local variables */
  uint8 iy;

  config_ptr->mac_ul_parms.tfcs_ptr->tfs_info[ix].ntf      = 0;
  config_ptr->mac_ul_parms.tfcs_ptr->tfs_info[ix].tti_info = TDSL1_NUM_TTI;
  config_ptr->mac_ul_parms.tfcs_ptr->tfs_info[ix].mac_header_size = 0;
  for(iy = 0; iy < TDSUE_MAX_TF; iy++)
  {
    config_ptr->mac_ul_parms.tfcs_ptr->tfs_info[ix].tf_info[iy].ntblks  = 0;
    config_ptr->mac_ul_parms.tfcs_ptr->tfs_info[ix].tf_info[iy].tb_size = 0;
  }
  return;
} /* end tdsrrcllc_init_ul_tfs_table */

/*====================================================================
FUNCTION: tdsrrcllc_init_ul_tfi_table()

DESCRIPTION:
  This function initializes the Uplink TFI table. This needs to be
  called whenever the DL CCTrCH type changes (i.e. a state change),
  or when a TrCH is deleted.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_ul_tfi_table(tdsordered_config_type *config_ptr)
{
  /* local variables */
  uint8 ix, iy;

  /* Indicate no Transport Format Combinations
     in tdsordered_config for MAC and L1 */
  config_ptr->mac_ul_parms.tfcs_ptr->ntfc = 0;
  config_ptr->rrc_ul_cctrch_params.num_tfci = 0;

  /* Clear out the number of removed TFCIs */
  config_ptr->num_removed_tfcis = 0;

  /* Clear out the uplink ctfc & tfis */
  for(ix = 0; ix < TDSUE_MAX_TFC; ix++)
  {
    config_ptr->mac_ul_parms.tfcs_ptr->tfc_info[ix].control_flag = TDSTFC_NOT_ALLOWED;
    config_ptr->mac_ul_parms.tfcs_ptr->tfc_info[ix].ctfc = 0;

    /* Clear out the Removed TFCI list and number */
    config_ptr->removed_tfci_list[ix] = 0;

    for(iy = 0; iy < TDSUE_MAX_TRCH; iy++)
    {
      config_ptr->mac_ul_parms.tfcs_ptr->tfc_info[ix].tfi[iy] = 0;
    }
  }

  return;
} /* end tdsrrcllc_init_ul_tfi_table */

/*====================================================================
FUNCTION: tdsrrcllc_init_dl_dch_rb_mapping_info()

DESCRIPTION:
  This function initializes the Downlink RB Mapping Info table. This is
  called whenever the DL CCTrCH type changes (i.e. a state change), or
  a TrCH is deleted.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_dch_rb_mapping_info
(
  tdsordered_config_type  *config_ptr,
  uint8  ix
)
{
  /* local loop variable */
  uint8 iy;

  /* Initialize the UL DCH RB Mapping info for a given TrCH Index */
  config_ptr->dl_dch_info[ix].trch_id = 0;

  /* No Dedicated Logical Channels now */
  config_ptr->dl_dch_info[ix].ndlchan = 0;

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  /* Initialize loopback mode */
  config_ptr->dl_dch_info[ix].loopback_test_mode = TDSMAC_LOOPBACK_NA;
#endif

  /* Clear out all Logical Channel info for this TrCH */
  for(iy = 0; iy < MAX_DED_LOGCHAN_PER_TRCH; iy++)
  {
    config_ptr->dl_dch_info[ix].dlchan_info[iy].mac_id = 0;
    /* LLC will clear out the RB ID and RLC ID */
    /* Don't care whether it is TM,UM,AM or DCCH or DTCH */
  }

  return;

} /* end tdsrrcllc_init_dl_dch_rb_mapping_info */

/*====================================================================
FUNCTION: tdsrrcllc_init_fach_rb_mapping_info()

DESCRIPTION:
  This function initializes the Downlink RB Mapping Info table. This is
  called whenever the DL CCTrCH type changes (i.e. a state change), or
  a TrCH is deleted.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_fach_rb_mapping_info
(
  tdsordered_config_type  *config_ptr,
  uint8  ix
)
{
  /* local loop variable */
  uint8 iy;

  /* Initialize the UL DCH RB Mapping info for a given TrCH Index */
  config_ptr->fach_info[ix].trch_id = 0;

  /* No Dedicated Logical Channels now */
  config_ptr->fach_info[ix].ndlchan = 0;

  config_ptr->cell_fach_rb_mapping_info.dl_ndlchan = 0;

  /* Clear out all Logical Channel info for this TrCH */
  for(iy = 0; iy < MAX_DED_LOGCHAN_PER_TRCH; iy++)
  {
    config_ptr->fach_info[ix].dlchan_info[iy].mac_id = 0;
    /* LLC will clear out the RB ID and RLC ID */
    /* Don't care whether it is TM,UM,AM or DCCH or DTCH */
  }

  /* No BCCH, CCCH, or CTCH either on this FACH */
  config_ptr->fach_info[ix].bcch_enable = FALSE;
  config_ptr->fach_info[ix].bcch_id     = 0;
  config_ptr->fach_info[ix].ccch_enable = FALSE;
  config_ptr->fach_info[ix].ccch_id     = 0;
  config_ptr->fach_info[ix].ctch_enable = FALSE;
  config_ptr->fach_info[ix].ctch_id     = 0;

  return;

} /* end tdsrrcllc_init_fach_rb_mapping_info */

/*====================================================================
FUNCTION: tdsrrcllc_init_ul_dch_rb_mapping_info()

DESCRIPTION:
  This function initializes the Uplink RB Mapping Info table. This is
  called whenever the DL CCTrCH type changes (i.e. a state change), or
  a TrCH is deleted.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_ul_dch_rb_mapping_info
(
  tdsordered_config_type  *config_ptr,
  uint8  ix
)
{
  /* local loop variable */
  uint8 iy;


  /* Initialize the UL DCH RB Mapping info for a given TrCH Index */
  config_ptr->ul_cctrch.dch_info.dch_info[ix].trch_id = 0;

  /* No Dedicated Logical Channels now */
  config_ptr->ul_cctrch.dch_info.dch_info[ix].ndlchan = 0;


  /* Clear out all Logical Channel info for this TrCH */
  for(iy = 0; iy < MAX_DED_LOGCHAN_PER_TRCH; iy++)
  {
    config_ptr->ul_cctrch.dch_info.dch_info[ix].dlchan_info[iy].mac_id = 0;
    config_ptr->ul_cctrch.dch_info.dch_info[ix].dlchan_info[iy].priority = 0;
    config_ptr->ul_cctrch.dch_info.dch_info[ix].dlchan_info[iy].rlc_size_info.all_tfi_valid = TRUE;
    config_ptr->ul_cctrch.dch_info.dch_info[ix].dlchan_info[iy].rlc_size_info.ntfi_per_lc = 0;
    /* LLC will clear out the RB ID and RLC ID */
    /* Don't care whether it is TM,UM,AM or DCCH or DTCH */
  }


  return;

} /* end tdsrrcllc_init_ul_dch_rb_mapping_info */

/*====================================================================
FUNCTION: tdsrrcllc_init_rach_rb_mapping_info()

DESCRIPTION:
  This function initializes the Uplink RB Mapping Info table. This is
  called whenever the DL CCTrCH type changes (i.e. a state change), or
  a TrCH is deleted.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_rach_rb_mapping_info(tdsordered_config_type  *config_ptr)
{
  /* local loop variable */
  uint8 iy;


  /* Initialize the UL DCH RB Mapping info for a given TrCH Index */
  config_ptr->ul_cctrch.rach_info.trch_id = 0;

  /* No Dedicated Logical Channels now */
  config_ptr->ul_cctrch.rach_info.ndlchan = 0;

  config_ptr->cell_fach_rb_mapping_info.ul_ndlchan = 0;


  /* Clear out all Logical Channel info for this TrCH */
  for(iy = 0; iy < MAX_DED_LOGCHAN_PER_TRCH; iy++)
  {
    config_ptr->ul_cctrch.rach_info.dlchan_info[iy].mac_id = 0;
    config_ptr->ul_cctrch.rach_info.dlchan_info[iy].priority = 0;
    config_ptr->ul_cctrch.rach_info.dlchan_info[iy].rlc_size_info.all_tfi_valid = TRUE;
    config_ptr->ul_cctrch.rach_info.dlchan_info[iy].rlc_size_info.ntfi_per_lc = 0;
    /* LLC will clear out the RB ID and RLC ID */
    /* Don't care whether it is TM,UM,AM or DCCH or DTCH */
  }


  /* No CCCH either on this RACH */
  config_ptr->ul_cctrch.rach_info.ccch_enable = FALSE;
  config_ptr->ul_cctrch.rach_info.ccch_id     = 0;
  config_ptr->ul_cctrch.rach_info.ccch_asc    = 0;

  return;

} /* end tdsrrcllc_init_rach_rb_mapping_info */

/*====================================================================
FUNCTION: tdsrrcllc_init_dl_pch_trch_idx()

DESCRIPTION:
  This function initializes the RRC DL PCH TRCH Data base. All DL
  PCH TRCH IDs are marked as available for use.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_pch_trch_idx(tdsordered_config_type *config_ptr)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"Initializing the DL PCH TrCH Index Data Base");
  config_ptr->dl_pch_trch_idx_in_use   = FALSE;
  return;
} /* end tdsrrcllc_init_dl_pch_trch_idx() */

/*====================================================================
FUNCTION: rrcllc_init_dl_fach_trch_idx()

DESCRIPTION:
  This function initializes the RRC DL FACH TRCH Data base. All DL
  FACH TRCH IDs are marked as available for use.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_fach_trch_idx_info(tdsordered_config_type *config_ptr)
{
  int trch_idx; /* local loop variable */

  TDSRRC_MSG0(MSG_LEGACY_MED,"Initializing the DL FACH TrCH Index Data Base");

  /* check for a valid DL TrCH ID */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    config_ptr->dl_fach_trch_idx_in_use[trch_idx] = FALSE;
    /* Initialize the rrcllcoc FACH TrCH database */
    config_ptr->dl_fach_trch_info[trch_idx].trch_id     = 0;
    config_ptr->dl_fach_trch_info[trch_idx].tf_cnt      = 0;
    config_ptr->dl_fach_trch_info[trch_idx].lg_ch_cnt   = 0;
    config_ptr->dl_fach_trch_info[trch_idx].dch_present = FALSE;

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    config_ptr->dl_fach_trch_idx_in_use_for_sccpch_with_ctch[trch_idx] = FALSE;
    /* Initialize the rrcllcoc FACH TrCH database */
    config_ptr->dl_fach_trch_info_for_sccpch_with_ctch[trch_idx].trch_id     = 0;
    config_ptr->dl_fach_trch_info_for_sccpch_with_ctch[trch_idx].tf_cnt      = 0;
    config_ptr->dl_fach_trch_info_for_sccpch_with_ctch[trch_idx].lg_ch_cnt   = 0;
    config_ptr->dl_fach_trch_info_for_sccpch_with_ctch[trch_idx].dch_present = FALSE;
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif
  }
  return;
} /* end rrcllc_init_dl_fach_trch_idx() */

/*====================================================================
FUNCTION: rrcllc_init_dl_dch_trch_idx()

DESCRIPTION:
  This function initializes the RRC DL DCH TRCH Data base. All DL
  DCH TRCH IDs are marked as available for use.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_dch_trch_idx_info(tdsordered_config_type *config_ptr)
{
  int trch_idx; /* local loop variable */

  TDSRRC_MSG0(MSG_LEGACY_MED,"Initializing the DL TDSDCH TrCH Index Data Base");

  /* check for a valid DL TrCH ID */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    config_ptr->dl_dch_trch_idx_in_use[trch_idx] = FALSE;
    /* Initialize the rrcllcoc DCH TrCH database */
    config_ptr->dl_dch_trch_info[trch_idx].trch_id     = 0;
    config_ptr->dl_dch_trch_info[trch_idx].tf_cnt      = 0;
    config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt   = 0;
    config_ptr->dl_dch_trch_info[trch_idx].dch_present = FALSE;

    /* These two flags track the number of logic channels mapped into one
     * Transport channel in DCH and decide when should the TB size be
     * adjusted.
     */
    config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt_down = FALSE;
    config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt_up = FALSE;
  }

  return;
} /* end rrcllc_init_dl_dch_trch_idx() */

/*====================================================================
FUNCTION: rrcllc_init_ul_rach_trch_idx()

DESCRIPTION:
  This function initializes the RRC UL RACH TRCH Data base. All UL
  RACH TRCH IDs are marked as available for use.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_ul_rach_trch_idx_info(tdsordered_config_type *config_ptr)
{
  uint8 trch_idx; /* local loop variable */

  TDSRRC_MSG0(MSG_LEGACY_MED,"Initializing the UL RACH Index Data Base");

  /* check for a valid DL TrCH ID */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    config_ptr->ul_rach_trch_idx_in_use[trch_idx] = FALSE;
    /* Initialize the DCH TrCH Info */
    config_ptr->ul_rach_trch_info[trch_idx].trch_id     = 0;
    config_ptr->ul_rach_trch_info[trch_idx].tf_cnt      = 0;
    config_ptr->ul_rach_trch_info[trch_idx].lg_ch_cnt   = 0;
    config_ptr->ul_rach_trch_info[trch_idx].dch_present = FALSE;
  }

  return;
} /* end rrcllc_init_ul_rach_trch_idx() */

/*====================================================================
FUNCTION: rrcllc_init_ul_dch_trch_idx()

DESCRIPTION:
  This function initializes the RRC UL RACH TRCH Data base. All UL
  RACH TRCH IDs are marked as available for use.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_ul_dch_trch_idx_info(tdsordered_config_type *config_ptr)
{
  uint8 trch_idx; /* local loop variable */

  TDSRRC_MSG0(MSG_LEGACY_MED,"Initializing the UL DCH Index Data Base");

  /* check for a valid DL TrCH ID */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    config_ptr->ul_dch_trch_idx_in_use[trch_idx] = FALSE;
    /* Initialize the DCH TrCH Info */
    config_ptr->ul_dch_trch_info[trch_idx].trch_id     = 0;
    config_ptr->ul_dch_trch_info[trch_idx].tf_cnt      = 0;
    config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt   = 0;
    config_ptr->ul_dch_trch_info[trch_idx].dch_present = FALSE;
  /* These two flags track the number of logic channels mapped into one
   * Transport channel in DCH and decide when should the TB size be
   * adjusted.
   */
    config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt_down = FALSE;
    config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt_up = FALSE;
  }
  return;
} /* end rrcllc_init_ul_dch_trch_idx() */

/*====================================================================
  The RLC size saga:
    For dedicated TrCHs:
      UTRAN gives RLC size in Transport format set.
      In Uplink:
        OC stores RLC size in TFS,
        MAC calculates TB Size from RLC size.
      In Downlink:
        OC stores TB size in TFS.

    For Common TrCHs
      UTRAN gives TB size in TFS.
        In Uplink and Downlink:
          OC stores TB size in TFS

    Calculation of RLC size:
      For dedicated TrCHs (DCHs):
        In Uplink:
          Find the TrCH that has the RB mapped to it.
          RLC size = MAX(TB size in OC)
        In Downlink:
          Find the TrCH that has the RB mapped to it.
          Determine MAC header length based on multiple/single
           logical channels on that TrCH.
          RLC size = MAX(RLC size in OC - MAC header size)

      For Common TrCHs (FACH/RACH):
        In Downlink:
          Determine MAC header length based on multiple/single
           logical channels on FACHes.
          For each FACH:
            RLC size = MAX(RLC size in OC - MAC header size)
          RLC size = MAX(RLC sizes from each FACH)
        In Uplink:
          Determine MAC header length based on multiple/single
            logical channels on RACH.
          RLC size = MAX(RLC size in OC - MAC header size)
=====================================================================*/

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_um_ccch_rlc_size()

DESCRIPTION:
  This is called when the RLC size for an downlink Unacknowledged Mode
  CCCH Logical Channel is required.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The size of the RLC UM channel.

SIDE EFFECTS:
  Note that currently all logical channels mapped to a Transport
  channel must have the same TrBLK size. Therefore, by extracting
  the RLC size of the first Transport Format, this will apply to
  all Transport Formats for all logical channels and types mapped to
  this TrCH, except for PCH/FACH, where it is the second index.
====================================================================*/
uint32 tdsrrcllc_get_dl_um_ccch_rlc_size(void)
{
  uint32  rlc_size = 0;
  uint32  trch_count;
  uint32  tf_count;

  for(trch_count = 0;
      trch_count < tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch;
      trch_count ++)
  {
    /* The Common Transport Channel could either be a PCH
     * or a FACH.
     */
    if(tdsordered_config_ptr->l1_dl_cctrch_parms.
        trch_info_array[trch_count]->ref_type == TDSL1_DL_TRCH_FACH_TYPE)
    {
      for(tf_count = 0;
          tf_count < tdsordered_config_ptr->l1_dl_cctrch_parms.
                     trch_info_array[trch_count]->num_tf;
          tf_count ++)
      {
        if((uint32)(tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
              tf_info_array[tf_count]->tb_size -
              TDSRRCLLC_COM_LOG_CHL_FACH_MAC_HDR_SIZE) >
            rlc_size)
        {
          rlc_size = tdsordered_config_ptr->l1_dl_cctrch_parms.
                       trch_info_array[trch_count]->
                       tf_info_array[tf_count]->tb_size -
                     TDSRRCLLC_COM_LOG_CHL_FACH_MAC_HDR_SIZE;
        }
      } /* For all TFs in a TRCH */
    } /* If TrCH type is a FACH */
  }  /* For all TrCHs */

  if(rlc_size == 0)
  {
    rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;
  }

  return(rlc_size);
} /* end tdsrrcllc_get_dl_um_ccch_rlc_size() */

/*===========================================================================

FUNCTION RRCLLC_CONVERT_TTI_TO_MS

DESCRIPTION
  This function converts the l1_tti_enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None
===========================================================================*/
static uint32 tdsrrcllc_convert_tti_to_ms
(
  tdsl1_tti_enum_type tti                    
)
{
  uint32    timer_value;                         /* Timer Value in milli seconds */

  switch (tti)
  {
    case TDSL1_TTI_10MS:
      timer_value = 10;          
      break;
    case TDSL1_TTI_20MS:
      timer_value = 20;
      break;      
    case TDSL1_TTI_40MS:
      timer_value = 40;
      break;
    case TDSL1_TTI_80MS:
      timer_value = 80;
      break;
    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unknown Timer value received ");
      timer_value = 0xEFFFFFFF;         
      break;
  }
  return (timer_value);
} /* RRCLLC_CONVERT_TTI_TO_MS */


/*====================================================================
FUNCTION: tdsrrcllc_print_trans_chnl_rate()

DESCRIPTION:
  This is called to print the rate of each transport channel in CC and OC
  in order to see how rate has changed.

DEPENDENCIES:
  None.

RETURN VALUE:
  void.

SIDE EFFECTS:

====================================================================*/
void tdsrrcllc_print_trans_chnl_rate(void)
{
  uint32  data_size = 0;
  uint32  rate = 0;
  uint32  tti_in_ms = 0;
  uint32  trch_count;
  uint32  tf_count;
  tdsrrcllc_oc_set_status_e_type oc_status;
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type   *l1_dl_cctrch_parms_p;
  tdsordered_config_type  *db_ptr = tdscurrent_config_ptr;
  tdsrrc_state_e_type  rrc_state = tdsrrc_get_state();
  

  l1_dl_cctrch_parms_p = &(db_ptr->l1_dl_cctrch_parms);

  /* dont want to print old rate in connecting */
  if (rrc_state != TDSRRC_STATE_CONNECTING)
  {
    for(trch_count = 0;
         trch_count < l1_dl_cctrch_parms_p->num_trch;
         trch_count ++)
    {
      for(tf_count = 0;
           tf_count < l1_dl_cctrch_parms_p->
           trch_info_array[trch_count]->num_tf;
           tf_count ++)
      {
        if((uint32)(l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tf_info_array[tf_count]->tb_size *
            l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tf_info_array[tf_count]->num_tb) >
           data_size)
        {
          data_size = l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tf_info_array[tf_count]->tb_size *
            l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tf_info_array[tf_count]->num_tb;
        }
      } /* For all TFs in a TRCH */
      tti_in_ms = tdsrrcllc_convert_tti_to_ms(l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tti_value); 
      rate = (data_size * 1000)/tti_in_ms;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Old rates for DL ==> Tr_id: %d  rate: %d tti_in_ms:%d",
               l1_dl_cctrch_parms_p->trch_info_array[trch_count]->trch_id, 
               rate, tti_in_ms);
      
      data_size = 0;  
    }  /* For all TrCHs */
    
    for(trch_count = 0; (trch_count < db_ptr->ul_tfcs.nchan); trch_count ++)
    {
      for(tf_count = 0; tf_count < db_ptr->ul_tfcs.tfs_info[trch_count].ntf; tf_count ++)
      {
        if(((uint32)(db_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].tb_size *
            db_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks))  > data_size)
        { 
          data_size =  db_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].tb_size * 
            db_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks;
        }
      }  /* For all TFs of a TrCH */
      tti_in_ms = tdsrrcllc_convert_tti_to_ms(db_ptr->ul_tfcs.tfs_info[trch_count].tti_info);
      rate = (data_size * 1000)/tti_in_ms;
      if (trch_count == 0 && (rrc_state == TDSRRC_STATE_CONNECTING || rrc_state == TDSRRC_STATE_CELL_FACH) )
      { 
        
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Old rates for UL ==> Tr_id: %d  rate: %d tti_in_ms:%d",
                 db_ptr->mac_ul_parms.cctrch_ptr->rach_info.trch_id, 
                 rate, tti_in_ms);
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Old rates for UL ==> Tr_id: %d  rate: %d tti_in_ms:%d",
                 db_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].trch_id, 
                 rate, tti_in_ms);
      }
      data_size = 0;  
    }
  }
  
  db_ptr = tdsordered_config_ptr;
  l1_dl_cctrch_parms_p = &(db_ptr->l1_dl_cctrch_parms);
  
  data_size = 0;
  oc_status = tdsrrcllc_get_ordered_config_status();

  if(oc_status == TDSOC_NOT_SET)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Getting rate from OC while it is not set");
  }
  for(trch_count = 0;
       trch_count < l1_dl_cctrch_parms_p->num_trch;
       trch_count ++)
  {
    for(tf_count = 0;
         tf_count < l1_dl_cctrch_parms_p->
         trch_info_array[trch_count]->num_tf;
         tf_count ++)
    {
      if((uint32)(l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tf_info_array[tf_count]->tb_size *
          l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tf_info_array[tf_count]->num_tb) >
         data_size)
      {
        data_size = l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tf_info_array[tf_count]->tb_size *
          l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tf_info_array[tf_count]->num_tb;
      }
    } /* For all TFs in a TRCH */
    tti_in_ms = tdsrrcllc_convert_tti_to_ms(l1_dl_cctrch_parms_p->trch_info_array[trch_count]->tti_value);
    rate = (data_size * 1000)/tti_in_ms;
    
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"New rates for DL ==> Tr_id: %d  rate: %d tti_in_ms:%d",
             l1_dl_cctrch_parms_p->trch_info_array[trch_count]->trch_id, 
             rate, tti_in_ms);
    data_size = 0;
  }  
  
  for(trch_count = 0; (trch_count < db_ptr->ul_tfcs.nchan); trch_count ++)
  {
    for(tf_count = 0; tf_count < db_ptr->ul_tfcs.tfs_info[trch_count].ntf; tf_count ++)
    {
      if(((uint32)(db_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].tb_size *
          db_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks))  > data_size)
      {
        
        data_size =  db_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].tb_size * 
          db_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks;
      }
    } 
    tti_in_ms = tdsrrcllc_convert_tti_to_ms(db_ptr->ul_tfcs.tfs_info[trch_count].tti_info); 
    rate = (data_size * 1000)/tti_in_ms;
    if (trch_count == 0 && (rrc_state == TDSRRC_STATE_CONNECTING || rrc_state == TDSRRC_STATE_CELL_FACH) )
    { 
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"New rates for UL ==> Tr_id: %d  rate: %d tti_in_ms:%d",
               db_ptr->mac_ul_parms.cctrch_ptr->rach_info.trch_id, 
               rate, tti_in_ms);
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"New rates for UL ==> Tr_id: %d  rate: %d tti_in_ms:%d",
               db_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].trch_id, rate,
               tti_in_ms);
    }
    data_size = 0;  
  }
  
} /* tdsrrcllc_print_trans_chnl_rate() */

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_get_ctch_enabled_status()

DESCRIPTION:
  This is called from other modules to check whether CTCH-FACH-SCCPCH
  pipe is currently set up.

DEPENDENCIES:
  None.

RETURN VALUE:
  boolean

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_get_ctch_enabled_status(void)
{

  tdsordered_config_type  *config_ptr;
  uint8 i;
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    /* If Ordered Config is SET and next state is Cell_FACH, look into
     * the Ordered Config. Otherwise look into Current Config.
     */
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }

  for(i = 0; i < config_ptr->mac_dl_parms.num_fachs; i ++)
  {
    if (config_ptr->mac_dl_parms.fach_info[i]->ctch_enable == TRUE)
    {
      return TRUE;
    }
  }

  return FALSE;
}
/*====================================================================
FUNCTION: tdsrrcllc_get_ctch_drx_info()

DESCRIPTION:
  This is called when DRX info for CTCH is required.

DEPENDENCIES:
  None.

RETURN VALUE:


SIDE EFFECTS:

====================================================================*/
void tdsrrcllc_get_ctch_drx_info(uint8 *frame_offset, uint16 *alloc_period)
{

  tdsordered_config_type  *config_ptr;

  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    /* If Ordered Config is SET and next state is Cell_FACH, look into
     * the Ordered Config. Otherwise look into Current Config.
     */
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }

  if (config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.ctch_info_present)
  {
    *frame_offset = (uint8)config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.cbs_frame_offset;
    *alloc_period = (uint16)config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.ctch_allocation_period;
  }
  else
  {
    *frame_offset = (uint8)config_ptr->l1_dl_chan_parms.sccpch[1].cbs_sched.cbs_frame_offset;
    *alloc_period = (uint16)config_ptr->l1_dl_chan_parms.sccpch[1].cbs_sched.ctch_allocation_period;
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_get_ctch_rlc_size()

DESCRIPTION:
  This is called when the RLC size for an downlink Unacknowledged Mode
  CTCH Logical Channel is required.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The size of the RLC UM channel.

SIDE EFFECTS:
  Note that currently all logical channels mapped to a Transport
  channel must have the same TrBLK size. Therefore, by extracting
  the RLC size of the first Transport Format, this will apply to
  all Transport Formats for all logical channels and types mapped to
  this TrCH, except for PCH/FACH, where it is the second index.
====================================================================*/
uint32 tdsrrcllc_get_ctch_rlc_size(void)
{
  uint32  rlc_size = 0;
  uint32  trch_count;
  uint32   fach_count = 0;
  uint32  tf_count;
  tdsrrcllc_ctch_status_e_type ctch_status;
  l1_dl_cctrch_info_struct_type   *l1_dl_cctrch_parms_p;
  tdsordered_config_type  *config_ptr;

  ctch_status = tdsrrcllc_semi_permanent_data.dl_phy_chan.ctch_status;


  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    /* If Ordered Config is SET and next state is Cell_FACH, look into
     * the Ordered Config. Otherwise look into Current Config.
     */
    config_ptr = tdsordered_config_ptr;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Using OC");
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Using CC");
  }


  if (ctch_status == TDSRRCLLC_CTCH_WITH_PCCH)
  {
    l1_dl_cctrch_parms_p = &(config_ptr->l1_dl_cctrch_parms);
  }
  else if (ctch_status == TDSRRCLLC_CTCH_ON_DIF_SCCPCH)
  {
    l1_dl_cctrch_parms_p = &(config_ptr->l1_dl_cctrch_parms_for_sccpch_with_ctch);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"There is no CTCH");
    return  (TDSRRCLLC_INVALID_RLC_SIZE);
  }

  for(trch_count = 0;
      trch_count < l1_dl_cctrch_parms_p->num_trch;
      trch_count ++)
  {
    /* The Common Transport Channel could either be a PCH
     * or a FACH.
     */
    if((fach_count < UE_MAX_TRCH) && (l1_dl_cctrch_parms_p->trch_info_array[trch_count]->ref_type == L1_DL_TRCH_FACH_TYPE))
    {
      if (config_ptr->mac_dl_parms.fach_info[fach_count]->ctch_enable == TRUE)
      {
        for(tf_count = 0;
             tf_count < l1_dl_cctrch_parms_p->
             trch_info_array[trch_count]->num_tf;
             tf_count ++)
        {
          if((l1_dl_cctrch_parms_p->trch_info_array[trch_count]->
              tf_info_array[tf_count]->tb_size -
              TDSRRCLLC_COM_LOG_CHL_FACH_MAC_HDR_SIZE) >
             rlc_size)
          {
            rlc_size = l1_dl_cctrch_parms_p->
                       trch_info_array[trch_count]->
                       tf_info_array[tf_count]->tb_size -
                       TDSRRCLLC_COM_LOG_CHL_FACH_MAC_HDR_SIZE;
          }
        } /* For all TFs in a TRCH */
        return rlc_size;
      }
      fach_count++;
    } /* If TrCH type is a FACH */
  }  /* For all TrCHs */

  if(rlc_size == 0)
  {
    rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;
  }

  return(rlc_size);
} /* end tdsrrcllc_get_ctch_rlc_size() */
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

/*====================================================================
FUNCTION: tdsrrcllc_get_am_rlc_size_for_rb()

DESCRIPTION:
 This function retrieves AM RLC PDU size from CC for a given RB rb_id.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The size of the RLC PDU.

SIDE EFFECTS:
  None
===================================================================*/
uint32 tdsrrcllc_get_am_rlc_size_for_rb
(
  tdsrrc_RB_Identity  rb_id, 
  tdsrrc_direction_enum_type direction
)
{
  uint8 am_idx = 0;
  uint32 rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;

  /*Loop thru all the AM entities in CC */
  for(am_idx = 0; am_idx < TDSUE_MAX_AM_ENTITY; am_idx++)
  {
    if(tdscurrent_config_ptr->am_config[am_idx].common_config.rb_id == rb_id)
    {
      /* RLC size is retrieved for UL*/
      if(direction == TDSUL)
      {
        rlc_size = tdscurrent_config_ptr->am_config[am_idx].rlc_ul_am_config.ul_rlc_size;
      }
      /* RLC size is retrieved for DL*/   
      else if (direction == TDSDL)
      {
        rlc_size = tdscurrent_config_ptr->am_config[am_idx].rlc_dl_am_config.dl_rlc_size;
      }
      break;
    }
  }
  return rlc_size;
}

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_ded_rlc_size()

DESCRIPTION:
  This is called when the RLC size for a downlink dedicated RLC
  entity is required. This is needed since the RLC sap requires the
  rlc size for channel setups. The input parameter is the next
  state.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The size of the RLC channel.

SIDE EFFECTS:
  None
===================================================================*/
uint32 tdsrrcllc_get_dl_ded_rlc_size
(
  tdsrrc_state_e_type  rrc_state,
  tdsrrc_RB_Identity  rb_id
)
{
  uint32 rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;
  uint32 trch_count;
  uint32 tf_count;
  uint32 dlch_count;
  uint8 mac_hdr_size;
  boolean  rb_found = FALSE;
  tdsordered_config_type  *config_ptr;

  if((rrc_state == TDSRRC_STATE_CELL_PCH) || (rrc_state == TDSRRC_STATE_URA_PCH))
  {
    if (TDSRRC_STATE_CELL_FACH == tdsrrc_get_state())
    {
      config_ptr = tdsordered_config_ptr;
      /*make rrc_state as CELL_FACH */
      rrc_state =  TDSRRC_STATE_CELL_FACH;
    }
    else
    {
      /* it is dch->pch transition. Check whether transition config has
      dcch_fach mapping in this case use the tdsordered_config to populate the
      rlc size (cell_fach mapping is already copied to oc from toc by this time
      change the next state as CELL_FACH to reuse the same code */
      if(TRUE == tdsrrcllc_check_dcch_fach_mapping(tdstransition_config.toc_ptr))
      {
        config_ptr = tdsordered_config_ptr;
        /*make rrc_state as CELL_FACH */
        rrc_state =  TDSRRC_STATE_CELL_FACH;
      }
      else
      {
        /*dcch_fach mapping was not present in toc. This means UE is transitioning from
        DCH to PCH without CELL_FACH mapping. Use the dch info to populate the RLC
        */
        config_ptr = tdstransition_config.toc_ptr;
        /*make rrc_state as CELL_FACH */
        rrc_state =  TDSRRC_STATE_CELL_DCH;
      }
    }
  }
  else
  {
    config_ptr = tdsordered_config_ptr;
  }

  if (rrc_state == TDSRRC_STATE_CELL_DCH)
  {
    rlc_size = 0;
    /* Look for the DCH that has the same RB mapped to it. Then calculate
    * RLC size from TrBlk size by substracting MAC header
    */
    for(
        trch_count = 0;
        (trch_count < config_ptr->l1_dl_cctrch_parms.num_trch) &&
          (rb_found == FALSE);
        trch_count ++
       )
    {
      for(
          dlch_count = 0;
          (dlch_count < config_ptr->mac_dl_parms.
                        dch_info[trch_count]->ndlchan) &&
            (rb_found == FALSE);
          dlch_count ++
         )
      {
        if(config_ptr->mac_dl_parms.dch_info[trch_count]->
            dlchan_info[dlch_count].rb_id == rb_id)
        {
          rb_found = TRUE;
          /* This is the TrCH */
          if(config_ptr->mac_dl_parms.dch_info[trch_count]->ndlchan > 1)
          {
            /* Multiple dedicated logical channels mapped on DCH */
            mac_hdr_size = TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
          }
          else
          {
            /* Single dedicated logical channel on DCH */
            mac_hdr_size = TDSRRCLLC_DED_SINGLE_LOG_CHL_DCH_MAC_HDR_SIZE;
          }

          for(
              tf_count = 0;
              tf_count < config_ptr->l1_dl_cctrch_parms.
                         trch_info_array[trch_count]->num_tf;
              tf_count ++
             )
          {
            if(config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                  tf_info_array[tf_count]->tb_size != 0)
            {
              /* If the tb_size less the mac_hdr_size is bigger than the
                 rlc_size, then we need to adjust the rlc_size up.  This
                 comparison is written with respect to zero to avoid a lint
                 warning about comparing signed and unsigned values, since a
                 subtraction always results in a signed value and the rlc_size
                 is an unsigned value. */
              if((config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                    tf_info_array[tf_count]->tb_size - mac_hdr_size - rlc_size)
                  > 0)
              {
                /*RLC size has to be octet aligned so dont subtract by 4 if 
                  already octet aligned */
                if((config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                             tf_info_array[tf_count]->tb_size % 8 ) == 0 )
                {
                  mac_hdr_size = TDSRRCLLC_DED_SINGLE_LOG_CHL_DCH_MAC_HDR_SIZE;
                }
                
                rlc_size = config_ptr->l1_dl_cctrch_parms.
                             trch_info_array[trch_count]->
                             tf_info_array[tf_count]->tb_size -
                           mac_hdr_size;
              }
            } /* Consider only those TFs that have TB size > 0 */
          } /* For all TFs in a TRCH */
        } /* If RB is found */
      } /* For all dedicated logical channels on this TrCH */
    }  /* For all TrCHs */
  } /* If state is Cell_DCH */

  else if(rrc_state == TDSRRC_STATE_CELL_FACH)
  {
    /* For FACH, the MAC header for dedicated logical channel is 24 bits
     * if C-RNTI is used. If U-RNTI is used, it is 40 bits. Based on C-RNTI,
     * subtract 24 from TB Size to get RLC size for DCCH mapped on FACH
     */
    rlc_size = 0;
    for(trch_count = 0;
        trch_count < config_ptr->l1_dl_cctrch_parms.num_trch;
        trch_count ++)
    {
      /* The Common Transport Channel could either be a PCH
       * or a FACH.
       */
      if(config_ptr->l1_dl_cctrch_parms.
          trch_info_array[trch_count]->ref_type == TDSL1_DL_TRCH_FACH_TYPE)
      {
#if 0
        /*Here the index used for FACh in MAC DB might be incorrect as L1 DB has 
           PCH also*/
        if(config_ptr->mac_dl_parms.fach_info[trch_count]->ndlchan > 1)
        {
          /* Multiple dedicated logical channels on FACH */
          mac_hdr_size = TDSRRCLLC_DED_MULTI_LOG_CHL_FACH_CRNTI_MAC_HDR_SIZE;
        }
        else
        {
          /* Single dedicated logical channel */
          mac_hdr_size = TDSRRCLLC_DED_SINGLE_LOG_CHL_FACH_CRNTI_MAC_HDR_SIZE;
        }
#endif

        /* Since the RLC size needs to be byte aligned, and the Tb size received in 
             FACH Trch is always Octet aligned, subtract by MAX FACH header size ,
             always. In any case all FACh shld have multiple
             Rbs, as we always map all Rbs to all the FACh */
        mac_hdr_size = TDSRRCLLC_DED_MULTI_LOG_CHL_FACH_CRNTI_MAC_HDR_SIZE;
  
        for(tf_count = 0;
            tf_count < config_ptr->l1_dl_cctrch_parms.
                       trch_info_array[trch_count]->num_tf;
            tf_count ++)
        {
          if(config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                tf_info_array[tf_count]->tb_size > 0)
          {
            /* If the tb_size less the mac_hdr_size is bigger than the
               rlc_size, then we need to adjust the rlc_size up.  This
               comparison is written with respect to zero to avoid a lint
               warning about comparing signed and unsigned values, since a
               subtraction always results in a signed value and the rlc_size
               is an unsigned value. */
            if((config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                  tf_info_array[tf_count]->tb_size - mac_hdr_size - rlc_size) >
                0)
            {
              rlc_size = config_ptr->l1_dl_cctrch_parms.
                           trch_info_array[trch_count]->
                           tf_info_array[tf_count]->tb_size -
                         mac_hdr_size;
            }
          }  /* If TB size > 0 */
        } /* For all TFs in a TRCH */
      } /* If TrCH type is a FACH */
    }  /* For all TrCHs */
  } /* State is Cell_FACH */
  if(rlc_size == 0)
  {
    rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;
  }
  if (rlc_size == TDSRRCLLC_INVALID_RLC_SIZE)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid DL RLC size for RBid:%d, curr_rrc_st:%d next_rrc_st:%d",
    rb_id,tdsrrc_get_state(),rrc_state);
  }
  if((rlc_size % 8 ) != 0)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid DL RLC size shld be byte aligned for RBid:%d, Size:%d next_rrc_st:%d",
    rb_id,rlc_size,rrc_state);
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_MED," DL RLC size for RBid:%d, Size:%d next_rrc_st:%d",
    rb_id,rlc_size,rrc_state);
  }
  return rlc_size;
} /* end tdsrrcllc_get_dl_ded_rlc_size() */

/*====================================================================
FUNCTION: tdsrrcllc_get_ul_ded_rlc_size()

DESCRIPTION:
  This is called when the RLC size for an uplink dedicated RLC
  entity is required. This is needed since the RLC sap requires the
  rlc size for channel setups. The input parameter is the next
  state.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The size of the RLC channel.

SIDE EFFECTS:
  None
===================================================================*/
uint32 tdsrrcllc_get_ul_ded_rlc_size
(
  tdsrrc_state_e_type  rrc_state,
  tdsrrc_RB_Identity  rb_id
)
{
  uint32 rlc_size = 0;
  uint32 trch_count;
  uint32 tf_count;
  uint32 tfi_count;
  uint32 tf_idx;
  uint32 dlch_count;
  uint8 mac_hdr_size;
  boolean  rb_found = FALSE;

  tdsordered_config_type  *config_ptr;
#ifdef FEATURE_TDSCDMA_HSUPA
  uint32 ix = 0, iy = 0, iz = 0;
#endif /* FEATURE_TDSCDMA_HSUPA */

  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }
  if((rrc_state == TDSRRC_STATE_CELL_PCH) || (rrc_state == TDSRRC_STATE_URA_PCH))
  {
    if (TDSRRC_STATE_CELL_FACH == tdsrrc_get_state())
    {
      /*this is cell_FACH->PCH transition */
      config_ptr = tdsordered_config_ptr;
      /*make rrc_state as CELL_FACH */
      rrc_state =  TDSRRC_STATE_CELL_FACH;
    }
    else
    {
      /* it is dch->pch transition. Check whether transition config has
      dcch_fach mapping in this case use the tdsordered_config to populate the
      rlc size (cell_fach mapping is already copied to oc from toc by this time
      change the next state as CELL_FACH to reuse the same code */
      if(TRUE == tdsrrcllc_check_dcch_fach_mapping(tdstransition_config.toc_ptr))
      {
        config_ptr = tdsordered_config_ptr;
        /*make rrc_state as CELL_FACH */
        rrc_state =  TDSRRC_STATE_CELL_FACH;
      }
      else
      {
        /*dcch_fach mapping was not present in toc. This means UE is transitioning from
        DCH to PCH without CELL_FACH mapping. Use the dch info to populate the RLC
        */
        config_ptr = tdstransition_config.toc_ptr;
        /*make rrc_state as CELL_FACH */
        rrc_state =  TDSRRC_STATE_CELL_DCH;
      }
    }
  }

  /* Note: UL UM could only be a dedicated logical channel */
  if(rrc_state == TDSRRC_STATE_CELL_DCH)
  {
    for(trch_count = 0;
        (trch_count < config_ptr->ul_tfcs.nchan) &&
          (rb_found == FALSE);
        trch_count ++)
    {
      for(
          dlch_count = 0;
          (dlch_count < config_ptr->mac_ul_parms.cctrch_ptr->dch_info.
            dch_info[trch_count].ndlchan) &&
            (rb_found == FALSE);
          dlch_count ++
         )
      {
        if(config_ptr->mac_ul_parms.cctrch_ptr->dch_info.
           dch_info[trch_count].dlchan_info[dlch_count].rb_id == rb_id)
        {
          rlc_size = 0;
          rb_found = TRUE;
          /* Get the RLC size */
          for(tf_count = 0;
              tf_count < config_ptr->ul_tfcs.tfs_info[trch_count].ntf;
              tf_count ++)
          {
            if(config_ptr->ul_tfcs.tfs_info[trch_count].
                tf_info[tf_count].tb_size  > rlc_size)
            {
              /* In Uplink the Transport format set in Ordered Config is
               * in terms of RLC PDU size as sent in RRC message.
               */
              rlc_size =  config_ptr->ul_tfcs.tfs_info[trch_count].
                          tf_info[tf_count].tb_size;
            }
          }  /* For all TFs of a TrCH */
        }  /* If this TrCH has the same RB ID mapped to it */
      } /* For all dedicated logical channels */
    } /* For all Transport Channels */
#ifdef FEATURE_TDSCDMA_HSUPA
    /* Check in EDCH MAC structure if there is a higher RLC size */
    rb_found = FALSE;
    /* Compute EDCH rlc size only if EDCH transmission is TRUE - CR#754844 
       If both EDCH/DCH transport channels are added, pick either RLC size of dch/EDCH 
       based on e_dch_transmisson 
    */
    if(TRUE == config_ptr->e_dch_transmission)
    {
      for (ix = 0; (!rb_found) && (ix < TDSMAX_E_DCH_MAC_D_FLOW) ; ix++) 
      {
        for (iy = 0; (!rb_found) && (iy < config_ptr->mac_e_config.e_mac_d_flow_info[ix].num_logch); iy++)
        {
          if (config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_ul_logch_info[iy].rb_id == rb_id)
          { 
            rb_found = TRUE;
            for (iz = 0; iz < config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_ul_logch_info[iy].num_rlc_size_ddi_info; iz++) 
            {
              if (rlc_size < config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_ul_logch_info[iy].rlc_size_ddi_info[iz].rlc_pdu_size_in_bits) 
              {
                rlc_size = config_ptr->mac_e_config.e_mac_d_flow_info[ix].e_ul_logch_info[iy].rlc_size_ddi_info[iz].rlc_pdu_size_in_bits;
              }
            }
          }
        }
      }
    }
#endif /* FEATURE_TDSCDMA_HSUPA */

  } /* State is DCH */
  else if(rrc_state == TDSRRC_STATE_CELL_FACH)
  {
    rlc_size = 0;

    /* When multiple logical channels are mapped to RACH, we need extra 4 bits in MAC header */
    
    if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ndlchan > 1)
    {
      mac_hdr_size = TDSRRCLLC_DED_MULTI_LOG_CHL_RACH_MAC_HDR_SIZE;
    }
    else
    {
      mac_hdr_size = TDSRRCLLC_DED_SINGLE_LOG_CHL_RACH_MAC_HDR_SIZE;
    }

    /*We need to loop through all the logical channels that are mapped to RACH 
        and find out which logical channel has RB id same as rb_id*/
    
    for(dlch_count = 0;
     (dlch_count < config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ndlchan) &&
          (rb_found == FALSE);
        dlch_count ++)
    {
      if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rb_id == rb_id)
      {
        rb_found = TRUE;
        
        /* Check for whether all the TFs are allowed */
        if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rlc_size_info.all_tfi_valid == TRUE)
        {
          /* Since there is only one RACH Channel, we use index 0 instead of looping through all the TrCHs 
           We loop through all the TFs of the RACH channel to find the max TB size. */
          TDSRRC_MSG0(MSG_LEGACY_MED,"All TFs are allowed.");
          for(tf_count =0;
               tf_count < config_ptr->ul_tfcs.tfs_info[0].ntf;
               tf_count ++)
          {
            /* We need to subtract MAC hdr from TB size to get RLC PDU size. */
            if((config_ptr->ul_tfcs.tfs_info[0].tf_info[tf_count].tb_size - mac_hdr_size - rlc_size) > 0)
            {
              rlc_size =  (config_ptr->ul_tfcs.tfs_info[0].tf_info[tf_count].tb_size - mac_hdr_size);
            }
          }
        } /*If all the TFs are allowed*/
        
        /*This case will arise when explicit list of TFs is given*/
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"Explicit list of TFs are given.");
          /* We loop through all the TFs of the RACH channel to find the max TB size. */
          
          for(tfi_count =0; 
               tfi_count < config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rlc_size_info.ntfi_per_lc; 
                tfi_count++)
          {
            /* tf_idx is the TF index that is given in explicit list of TFs for RACH Channel */
            
            tf_idx = config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rlc_size_info.lc_tfi_list[tfi_count];
            
            if((config_ptr->ul_tfcs.tfs_info[0].tf_info[tf_idx].tb_size - mac_hdr_size - rlc_size) > 0)
            {
              rlc_size =  (config_ptr->ul_tfcs.tfs_info[0].tf_info[tf_idx].tb_size - mac_hdr_size);
            }
          } /* For all the TFIs that are given for the logical channel dlch_count */
        } /*If all the TFs are not allowed*/
      } /*If this TrCH has the same RB ID mapped to it*/
    }/* For all dedicated logical channels mapped to CCTrCH*/
  } /* State is FACH */
  if(rlc_size == 0)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid UL RLC size for RBid:%d, curr_rrc_st:%d next_rrc_st:%d",
    rb_id,tdsrrc_get_state(),rrc_state);
    /*Check if CC has a valid RLC Size, if then use it in OC*/
    for(dlch_count = 0; dlch_count< TDSUE_MAX_AM_ENTITY; dlch_count++)
    {
      if(tdscurrent_config_ptr->am_config[dlch_count].common_config.rb_id == rb_id)
      {
        rlc_size = tdscurrent_config_ptr->am_config[dlch_count].rlc_ul_am_config.ul_rlc_size;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Use valid RLC Size from CC");
        break;
      }
    }
  }
  if(rlc_size == 0)
  {
    rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid UL RLC size for RBid:%d, curr_rrc_st:%d next_rrc_st:%d",
    rb_id,tdsrrc_get_state(),rrc_state);
  }
  TDSRRC_MSG2(MSG_LEGACY_MED,"RLC Size for RB id %d is %d",rb_id,rlc_size);
  return rlc_size;
} /* end tdsrrcllc_get_ul_ded_rlc_size() */
/*====================================================================
FUNCTION: tdsrrcllc_get_dl_ded_rate()

DESCRIPTION:
  This is called when dl rate for a particular rb is needed

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The uplink rate of the rb passed in.

SIDE EFFECTS:
  None
===================================================================*/
uint32 tdsrrcllc_get_dl_ded_rate
(
  tdsrrc_state_e_type  rrc_state,
  tdsrrc_RB_Identity  rb_id
)
{
  uint32 data_size = TDSRRCLLC_INVALID_RLC_SIZE;
  uint32 trch_count;
  uint32 tf_count;
  uint32 dlch_count;
  uint8 mac_hdr_size = 0;
  uint8 rlc_hdr_size = 0;
  boolean  rb_found = FALSE;
  tdsordered_config_type  *config_ptr;
  uint32 tti_in_ms = 0;
  uint32 rate = 0;
#ifdef FEATURE_TDSCDMA_MIMO
 tdsl1_dl_hs_mimo_action_enum_type mimo_action 
                    =tdsrrcllc_get_mimo_action_in_config();            
#endif          
       
#ifdef FEATURE_TDS_DC_HSDPA
  tdsl1_hsdpa_dc_hsdpa_action_enum_type dc_hsdpa_action =
                  tdsrrcllc_get_dc_hsdpa_action_in_config();
#endif /*FEATURE_TDSCDMA_DC_HSDPA*/
            
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }

  if(NULL == config_ptr)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Config ptr is NULL, returing zero");
    return 0;
  }
  /* If state is ura_pch or cell_pch, and prev state is DCH, look into TOC */
  if ((rrc_state == TDSRRC_STATE_CELL_PCH) || (rrc_state == TDSRRC_STATE_URA_PCH)) 
  {
    if (TDSRRC_STATE_CELL_DCH == tdsrrc_get_prev_state()) 
    {
      config_ptr = tdstransition_config.toc_ptr;
      rrc_state =  TDSRRC_STATE_CELL_DCH;
    }
    else
    {
      /*make rrc_state as CELL_FACH to to look at dtch-fach mapping*/
      rrc_state =  TDSRRC_STATE_CELL_FACH;
    }
  }

  if (rrc_state == TDSRRC_STATE_CELL_DCH)
  {
    /* Look for the DCH that has the same RB mapped to it. Then calculate
    * RLC size from TrBlk size by substracting MAC header
    */
    for(
        trch_count = 0;
        (trch_count < config_ptr->l1_dl_cctrch_parms.num_trch) &&
          (rb_found == FALSE);
        trch_count ++
       )
    {
      for( dlch_count = 0;
           (dlch_count < config_ptr->mac_dl_parms.dch_info[trch_count]->ndlchan) &&
            (rb_found == FALSE);
           dlch_count ++
         )
      {
        if(config_ptr->mac_dl_parms.dch_info[trch_count]->
            dlchan_info[dlch_count].rb_id == rb_id)
        {
          rb_found = TRUE;
          data_size = 0;
          /*Set RLC header size based on RLC mode of the corresponding RB.*/
          if((config_ptr->mac_dl_parms.dch_info[trch_count]->
            dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED) ||
          (config_ptr->mac_dl_parms.dch_info[trch_count]->
            dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_DATA) ||
          (config_ptr->mac_dl_parms.dch_info[trch_count]->
            dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_CONTROL))
          {
            rlc_hdr_size = 16;
          }
          else if(config_ptr->mac_dl_parms.dch_info[trch_count]->
            dlchan_info[dlch_count].rlc_mode == UE_MODE_UNACKNOWLEDGED)
          {
            rlc_hdr_size = 8;
          }
          else if(config_ptr->mac_dl_parms.dch_info[trch_count]->
            dlchan_info[dlch_count].rlc_mode == UE_MODE_TRANSPARENT)
          {
            rlc_hdr_size = 0;
          }
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"Wrong RLC Mode %d for RB %d",config_ptr->
              mac_dl_parms.dch_info[trch_count]->dlchan_info[dlch_count].rlc_mode,rb_id);
          }          
          /* This is the TrCH */
          if(config_ptr->mac_dl_parms.dch_info[trch_count]->ndlchan > 1)
          {
            /* Multiple dedicated logical channels mapped on DCH */
            mac_hdr_size = TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
          }
          else
          {
            /* Single dedicated logical channel on DCH */
            mac_hdr_size = TDSRRCLLC_DED_SINGLE_LOG_CHL_DCH_MAC_HDR_SIZE;
          }

          for(
              tf_count = 0;
              tf_count < config_ptr->l1_dl_cctrch_parms.
                         trch_info_array[trch_count]->num_tf;
              tf_count ++
             )
          {
            if(config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                  tf_info_array[tf_count]->tb_size != 0)
            {
              /* If the tb_size less the mac_hdr_size is bigger than the
                 rlc_size, then we need to adjust the rlc_size up.  This
                 comparison is written with respect to zero to avoid a lint
                 warning about comparing signed and unsigned values, since a
                 subtraction always results in a signed value and the rlc_size
                 is an unsigned value. */
              if(
                 ((uint32)((config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                    tf_info_array[tf_count]->tb_size - (rlc_hdr_size +  mac_hdr_size)) * 
                   config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                    tf_info_array[tf_count]->num_tb) > data_size))
              {
                data_size = (config_ptr->l1_dl_cctrch_parms.
                  trch_info_array[trch_count]->tf_info_array[tf_count]->tb_size - 
                  (rlc_hdr_size + mac_hdr_size) )* config_ptr->l1_dl_cctrch_parms.
                  trch_info_array[trch_count]->tf_info_array[tf_count]->num_tb;
                tti_in_ms = tdsrrcllc_convert_tti_to_ms(config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->tti_value); 
                rate = ((data_size * 1000)/tti_in_ms);
              }
            } /* Consider only those TFs that have TB size > 0 */
          } /* For all TFs in a TRCH */
        } /* If RB is found */
      } /* For all dedicated logical channels on this TrCH */
    }  /* For all TrCHs */

    /* Check for higher rates in HS.  Look for the rb_id in HS database */
    /* Look into all DFLOWs to get to the one mapped to the same RB ID */
    if (((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE) &&
        (tdsrrc_get_hsdpa_action() != TDSHSDPA_STOP)) ||
        ((tdsrrc_get_hsdpa_status() == TDSHSDPA_INACTIVE) &&
          (tdsrrc_get_hsdpa_action() == TDSHSDPA_START)))
    {
      if (tdsrrcllc_check_if_rb_mapped_on_dflow(config_ptr, rb_id)
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
       || (tdsrrcllc_check_if_rb_mapped_on_macehs_q(config_ptr, rb_id))
#endif
         )
      {
        TDSRRC_MSG1(MSG_LEGACY_MED,"rb_id %d is HS rb - set rate based on cat",rb_id);
        /* set data seize to non-zero value so that it doesn't return invalid below*/
        data_size = 1;
        /* Here return based on the HS category as HS rates are dynamic */
        
        /* The relation between HSDPA category and rate are taken from 25.306 8.10.0
                  (Table 5.1c: 1.28 Mcps TDD HS-DSCH physical layer categories) */  
        /*Here 1Mbps = (1000 * 1000) bps*/      
        switch(tdsnv_hsdpa_category)
        {
          case 1:
          case 2:
          case 3:
            /* 0.55 Mbps*/
            rate = 550000;
            break;
          case 4:
          case 5:
          case 6:
            /* 1.1 Mbps*/
            rate = 1100000;
            break;
          case 7:
          case 8:
          case 9:
            /* 1.6 Mbps*/
            rate = 1600000;
            break;
          case 10:
          case 11:
          case 12:
            /* 2.2 Mbps*/
            rate = 2200000;
            break;
          case 13:
          case 14:
          case 15:
            /* 2.8 Mbps*/
            rate = 2800000;
            break;
          case 16:
          case 17:
          case 18:
            /* 2.5 Mbps*/
            rate = 2500000;
            break;
          case 19:
          case 20:
          case 21:
            /* 3.3 Mbps*/
            rate = 3300000;
            break;
          case 22:
          case 23:
          case 24:
            /* 4.2 Mbps*/
            rate = 4200000;
            break;
          default:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid rate for HS rab");
            break;

        }
      }
    }
  } /* If state is Cell_DCH */

  else if(rrc_state == TDSRRC_STATE_CELL_FACH)
  {
    /* For FACH, the MAC header for dedicated logical channel is 24 bits
     * if C-RNTI is used. If U-RNTI is used, it is 40 bits. Based on C-RNTI,
     * subtract 24 from TB Size to get RLC size for DCCH mapped on FACH
     */
    for(trch_count = 0;
        trch_count < config_ptr->l1_dl_cctrch_parms.num_trch && rb_found == FALSE;
        trch_count ++)
    {
      /* The Common Transport Channel could either be a PCH
       * or a FACH.
       */
      if(config_ptr->l1_dl_cctrch_parms.
          trch_info_array[trch_count]->ref_type == TDSL1_DL_TRCH_FACH_TYPE)
      {
        if(config_ptr->mac_dl_parms.fach_info[trch_count]->ndlchan > 1)
        {
          /* Multiple dedicated logical channels on FACH */
          mac_hdr_size = TDSRRCLLC_DED_MULTI_LOG_CHL_FACH_CRNTI_MAC_HDR_SIZE;
        }
        else
        {
          /* Single dedicated logical channel */
          mac_hdr_size = TDSRRCLLC_DED_SINGLE_LOG_CHL_FACH_CRNTI_MAC_HDR_SIZE;
        }

        for (
             dlch_count = 0;
             (dlch_count < config_ptr->mac_dl_parms.fach_info[trch_count]->ndlchan) && 
              (rb_found == FALSE);
              dlch_count ++ )
        {
          if (config_ptr->mac_dl_parms.fach_info[trch_count]->dlchan_info[dlch_count].rb_id == rb_id) 
          {
            rb_found = TRUE;
            data_size = 0;
            /*RLC header is updated based on the RLC mode of the RB*/
            if((config_ptr->mac_dl_parms.fach_info[trch_count]->
              dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED) ||
            (config_ptr->mac_dl_parms.fach_info[trch_count]->
              dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_DATA) ||
            (config_ptr->mac_dl_parms.fach_info[trch_count]->
              dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_CONTROL))
            {
              rlc_hdr_size = 16;
            }
            else if(config_ptr->mac_dl_parms.fach_info[trch_count]->
              dlchan_info[dlch_count].rlc_mode == UE_MODE_UNACKNOWLEDGED)
            {
              rlc_hdr_size = 8;
            }
            else if(config_ptr->mac_dl_parms.fach_info[trch_count]->
              dlchan_info[dlch_count].rlc_mode == UE_MODE_TRANSPARENT)
            {
              rlc_hdr_size = 0;
            }
            else
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Wrong RLC Mode %d for RB %d",config_ptr->
                mac_dl_parms.fach_info[trch_count]->dlchan_info[dlch_count].rlc_mode,rb_id);
            }
            
            for(tf_count = 0;
                 tf_count < config_ptr->l1_dl_cctrch_parms.
                 trch_info_array[trch_count]->num_tf;
                 tf_count ++)
            {
              if(config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                 tf_info_array[tf_count]->tb_size > 0 && 
                 config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                 tf_info_array[tf_count]->num_tb > 0)
              {
                /* If the tb_size less the mac_hdr_size is bigger than the
                rlc_size, then we need to adjust the rlc_size up.  This
                comparison is written with respect to zero to avoid a lint
                warning about comparing signed and unsigned values, since a
               subtraction always results in a signed value and the rlc_size
               is an unsigned value. */
                if((uint32)((config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                    tf_info_array[tf_count]->tb_size - mac_hdr_size - rlc_hdr_size)* 
                   config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                    tf_info_array[tf_count]->num_tb) > data_size)
                {
                  data_size = (config_ptr->l1_dl_cctrch_parms.
                    trch_info_array[trch_count]->
                    tf_info_array[tf_count]->tb_size -
                    mac_hdr_size - rlc_hdr_size) * config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                    tf_info_array[tf_count]->num_tb;
                  tti_in_ms = tdsrrcllc_convert_tti_to_ms(config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->tti_value); 
                  rate = ((data_size * 1000)/tti_in_ms);
                }
              }  /* If TB size > 0 */
            } /* For all TFs in a TRCH */ 
          } /* if RB_ID is found */
        } /* For all dedicated logical channels on this TrCH */
      } /* If TrCH type is a FACH */
    }  /* For all TrCHs */
  } /* State is Cell_FACH */
  if(data_size == 0)
  {
    data_size = TDSRRCLLC_INVALID_RLC_SIZE;
  }
  if (data_size == TDSRRCLLC_INVALID_RLC_SIZE)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Rate not determined for RBid:%d, curr_rrc_st:%d next_rrc_st:%d",
    rb_id,tdsrrc_get_state(),rrc_state);
  }

  TDSRRC_MSG4(MSG_LEGACY_HIGH,"DL Rate for rb_id %d is %d, data size %d tti %d ",
                               rb_id, rate, data_size, tti_in_ms);
  return rate;
} /* end tdsrrcllc_get_dl_ded_rate() */
/*====================================================================
FUNCTION: tdsrrcllc_get_ul_ded_rate()

DESCRIPTION:
  This is called when the uplink rate for a particular rb is needed

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The uplink rate of the rb passed in.

SIDE EFFECTS:
  None
===================================================================*/
uint32 tdsrrcllc_get_ul_ded_rate
(
  tdsrrc_state_e_type  rrc_state,
  tdsrrc_RB_Identity  rb_id
)
{
  uint32 data_size = TDSRRCLLC_INVALID_RLC_SIZE;
  uint32 trch_count;
  uint32 tf_count;
  uint32 dlch_count;
  uint8 rlc_hdr_size = 0;
  boolean  rb_found = FALSE;
  uint32 rate = 0;
  uint32 tti_in_ms = 0;
  uint8 mac_hdr_size = 0;

  tdsordered_config_type  *config_ptr;

  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }
  if(NULL == config_ptr)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Config ptr is NULL, returing zero");
    return 0;
  }

  /* If state is ura_pch or cell_pch, and prev state is DCH, look into TOC */
  if ((rrc_state == TDSRRC_STATE_CELL_PCH) || (rrc_state == TDSRRC_STATE_URA_PCH)) 
  {
    if (TDSRRC_STATE_CELL_DCH == tdsrrc_get_prev_state()) 
    {
      config_ptr = tdstransition_config.toc_ptr;
      rrc_state =  TDSRRC_STATE_CELL_DCH;
    }
    else
    {
      /*make rrc_state as CELL_FACH to to look at dtch-fach mapping*/
      rrc_state =  TDSRRC_STATE_CELL_FACH;
    }
  }

  /* Note: UL UM could only be a dedicated logical channel */
  if(rrc_state == TDSRRC_STATE_CELL_DCH)
  {
    for(trch_count = 0;
        (trch_count < config_ptr->ul_tfcs.nchan) &&
          (rb_found == FALSE);
        trch_count ++)
    {
      for(
          dlch_count = 0;
          (dlch_count < config_ptr->mac_ul_parms.cctrch_ptr->dch_info.
            dch_info[trch_count].ndlchan) &&
            (rb_found == FALSE);
          dlch_count ++
         )
      {
        mac_hdr_size = 0;
        if ( config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].ndlchan > 1)
        {
          /*Multiple dedicated logical channels mapped on DCH */  
          mac_hdr_size = TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
        }
        else
        {
          /*Single dedicated logical channel on DCH */
          mac_hdr_size = TDSRRCLLC_DED_SINGLE_LOG_CHL_DCH_MAC_HDR_SIZE;
        }
        if(config_ptr->mac_ul_parms.cctrch_ptr->dch_info.
           dch_info[trch_count].dlchan_info[dlch_count].rb_id == rb_id)
        {
          data_size = 0;
          rb_found = TRUE;
          /*RLC header is updated based on the RLC mode of the RB*/
          if((config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].
            dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED) ||
          (config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].
            dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_DATA) ||
          (config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].
            dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_CONTROL))
          {
            rlc_hdr_size = 16;
          }
          else if(config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].
            dlchan_info[dlch_count].rlc_mode == UE_MODE_UNACKNOWLEDGED)
          {
            rlc_hdr_size = 8;
          }
          else if(config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].
            dlchan_info[dlch_count].rlc_mode == UE_MODE_TRANSPARENT)
          {
            rlc_hdr_size = 0;
          }          
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"Wrong RLC Mode %d for RB %d",config_ptr->mac_ul_parms.cctrch_ptr->dch_info.
              dch_info[trch_count].dlchan_info[dlch_count].rlc_mode,rb_id);
          }         
          
          /* Get the RLC size */
          for(tf_count = 0;
              tf_count < config_ptr->ul_tfcs.tfs_info[trch_count].ntf;
              tf_count ++)
          {
            if((uint32)((config_ptr->ul_tfcs.tfs_info[trch_count].
                tf_info[tf_count].tb_size - rlc_hdr_size - mac_hdr_size) *  
                config_ptr->ul_tfcs.tfs_info[trch_count].
                tf_info[tf_count].ntblks) > data_size)
            {
              /* In Uplink the Transport format set in Ordered Config is
               * in terms of RLC PDU size as sent in RRC message.
               */
              data_size =  (config_ptr->ul_tfcs.tfs_info[trch_count].
                          tf_info[tf_count].tb_size - rlc_hdr_size - mac_hdr_size) * 
                config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks ;
              tti_in_ms = tdsrrcllc_convert_tti_to_ms(config_ptr->ul_tfcs.tfs_info[trch_count].tti_info); 
              rate = ((data_size * 1000)/tti_in_ms);
            }
          }  /* For all TFs of a TrCH */
        }  /* If this TrCH has the same RB ID mapped to it */
      } /* For all dedicated logical channels */
    } /* For all Transport Channels */

#ifdef FEATURE_TDSCDMA_HSUPA
    /* Check for higher rates in HS.  Look for the rb_id in EDCH database */
    if (TRUE == tdsrrc_is_eul_active())
    {
      rb_found = FALSE;
      for(trch_count = 0;
           (trch_count < config_ptr->mac_e_config.num_e_mac_d_flow) &&
           (rb_found == FALSE);
           trch_count ++)
      {
        for (dlch_count=0;
             dlch_count < config_ptr->mac_e_config.e_mac_d_flow_info[trch_count].num_logch; 
             dlch_count++)
        {
          if (config_ptr->mac_e_config.e_mac_d_flow_info[trch_count].e_ul_logch_info[dlch_count].rb_id == rb_id)
          {
            TDSRRC_MSG1(MSG_LEGACY_MED,"rb_id %d is EUL rb - set rate based on cat",rb_id);
            /* set data seize to non-zero value so that it doesn't return invalid below*/
            data_size = 1;
            rb_found = TRUE;

            /*The relation between HSUPA categories and rate is taken from Spec 25.306 7.80 
            (Table 5.1m: TDD E-DCH physical layer categories). Here 1 Mbps  = (1000 * 1000) bps*/           
            switch (tds_nv_hsupa_category) 
            {
              /* In TDD, UPA tti is 5ms. rate = (bits number * 1000) /5 */
              case 1:
                /* 0.5508 Mbps */
                rate = 550800;
                break;

              case 2:
                /* 0.8324 Mbps */
                rate = 832400;
                break;

              case 3:
                /* 1.1064 Mpbs */
                rate = 1106400;
                break;

              case 4:
                /* 1.6696 Mpbs */
                rate = 1669600;
                break;

              case 5:
                /* 2.2320 Mbps */
                rate = 2232000;
                break;

              case 6:
                /* 2.2320 Mbps */
                rate = 2232000;
                break;

            default:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid rate for EUL rab");
              break;
            }
            break;
          }
        }       
      }
    }
#endif /* FEATURE_TDSCDMA_HSUPA */
  } /* State is DCH */
  else if(rrc_state == TDSRRC_STATE_CELL_FACH)
  {
    for(trch_count = 0;
        trch_count < config_ptr->ul_tfcs.nchan && rb_found == FALSE;
        trch_count ++)
    {
      for(
          dlch_count = 0;
          (dlch_count < config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ndlchan) &&
           (rb_found == FALSE);
          dlch_count ++
            )
      {
        if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rb_id == rb_id)
        {
          data_size = 0;
          rb_found = TRUE;
          if((config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].
            rlc_mode == UE_MODE_ACKNOWLEDGED) ||
          (config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].
            rlc_mode == UE_MODE_ACKNOWLEDGED_DATA) ||
          (config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].
            rlc_mode == UE_MODE_ACKNOWLEDGED_CONTROL))
          {
            rlc_hdr_size = 16;
          }
          else if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].
            rlc_mode == UE_MODE_UNACKNOWLEDGED)
          {
            rlc_hdr_size = 8;
          }
          else if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].
            rlc_mode == UE_MODE_TRANSPARENT)
          {
            rlc_hdr_size = 0;
          }          
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"Wrong RLC Mode %d for RB %d",config_ptr->mac_ul_parms.cctrch_ptr->
              rach_info.dlchan_info[dlch_count].rlc_mode,rb_id);
          }             
          for(tf_count = 0;
               tf_count < config_ptr->ul_tfcs.tfs_info[trch_count].ntf;
               tf_count ++)
          {
            /* If the tb_size less the mac_hdr_size is bigger than the
            rlc_size, then we need to adjust the rlc_size up.  This
            comparison is written with respect to zero to avoid a lint
            warning about comparing signed and unsigned values, since a
            subtraction always results in a signed value and the rlc_size
            is an unsigned value. */
            if((uint32)((config_ptr->ul_tfcs.tfs_info[trch_count].
                tf_info[tf_count].tb_size - rlc_hdr_size) * 
               config_ptr->ul_tfcs.tfs_info[trch_count].
                tf_info[tf_count].ntblks) > data_size)
            {
              data_size =  (config_ptr->ul_tfcs.tfs_info[trch_count].
                           tf_info[tf_count].tb_size - rlc_hdr_size) * 
                config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks;
              tti_in_ms = tdsrrcllc_convert_tti_to_ms(config_ptr->ul_tfcs.tfs_info[trch_count].tti_info);
              rate = ((data_size * 1000)/tti_in_ms);
            }
          }  /* For all TFs of a TrCH */
        } /* if rb_id is found */
      } /* for all dedicated logical channel*/
    } /* For all Transport Channels */
  } /* State is FACH */
  if(data_size == 0)
  {
    data_size = TDSRRCLLC_INVALID_RLC_SIZE;
  }
  if (data_size == TDSRRCLLC_INVALID_RLC_SIZE)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Rate not determined for RBid:%d, curr_rrc_st:%d next_rrc_st:%d",
    rb_id,tdsrrc_get_state(),rrc_state);
  }

  TDSRRC_MSG4(MSG_LEGACY_HIGH,"UL Rate for rb_id %d is %d, data size %d tti %d",
                               rb_id, rate,data_size, tti_in_ms);
  return rate;
} /* end tdsrrcllc_get_ul_ded_rlc_size() */
/*====================================================================
FUNCTION: tdsrrcllc_translate_n313

DESCRIPTION:
  This function translates the enum for timer N313 into an int16 value.

DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
uint16 tdsrrcllc_translate_n313(tdsrrc_N_313 val)
{
  switch (val)
  {
    case tdsrrc_N_313_s1:
      return 1;
    case tdsrrc_N_313_s2:
      return 2;
    case tdsrrc_N_313_s4:
      return 4;
    case tdsrrc_N_313_s10:
      return 10;
    case tdsrrc_N_313_s20:
      return 20;
    case tdsrrc_N_313_s50:
      return 50;
    case tdsrrc_N_313_s100:
      return 100;
    case tdsrrc_N_313_s200:
      return 200;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid N313 = %d", val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
}

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER) 
/*====================================================================
FUNCTION: tdsrrcllc_get_timers_and_const_in_conn_mode_for_handover

DESCRIPTION:
  This function initializes default connected mode timers and
  constants for GtoW handover.  Defaults are used here because the
  UE does not have access to SIB1 at this point.

DEPENDENCIES:
  None

RETURN VALUE:
  None.

SIDE EFFECTS:
====================================================================*/
void
tdsrrcllc_get_timers_and_const_in_conn_mode_for_handover(void)
{
  //initialize timers and constants to mandatory default params
  tdsrrc_initialize_ue_conn_timers_and_constants();

  tdsordered_config_ptr->l1_dl_chan_parms.sync_det.N312 = (uint16)TDSRRC_GET_N312();
  tdsordered_config_ptr->l1_dl_chan_parms.sync_det.N313 =
    tdsrrcllc_translate_n313(TDSRRC_GET_N313());
  tdsordered_config_ptr->l1_dl_chan_parms.sync_det.N315 = (uint16)TDSRRC_GET_N315();

  tdsordered_config_ptr->l1_dl_chan_parms.sync_det.T312 = (uint8)TDSRRC_GET_T312();
  tdsordered_config_ptr->l1_dl_chan_parms.sync_det.T313 = (uint8)TDSRRC_GET_T313();

}
#endif

/*====================================================================
FUNCTION: tdsrrcllc_get_timers_and_const_in_conn_mode

DESCRIPTION:
  This function takes a pointer to the serving cell SIB 1 and
  copies all timers and constant values that are needed by L1
  into Ordered Config. If an element is mandatory default, the
  default value is supplied if it is not present in the SIB.

DEPENDENCIES:
  None

RETURN VALUE:
  None.

SIDE EFFECTS:
====================================================================*/
static uecomdef_status_e_type
tdsrrcllc_get_timers_and_const_in_conn_mode
(
  tdsordered_config_type *config_ptr,
  tdsrrc_SysInfoType1 *sib1_ptr
)
{
  if(sib1_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Passed NULL SIB1 pointer");
    return(FAILURE);
  }
  /* Get the Physical Channel Establishment Criteria counter value */
  config_ptr->l1_dl_chan_parms.sync_det.N312 = (uint16)TDSRRC_GET_N312();

  /* Get the Radio Link Failure Criteria counter value. */
  config_ptr->l1_dl_chan_parms.sync_det.N313 =
    tdsrrcllc_translate_n313(TDSRRC_GET_N313());

  /* Get the Radio Link Re-establishment Criteria counter value. */
  config_ptr->l1_dl_chan_parms.sync_det.N315 = (uint16)TDSRRC_GET_N315();

  /* Get the Physical Channel establishment timer T312 */
  config_ptr->l1_dl_chan_parms.sync_det.T312 =
      (uint8)TDSRRC_GET_T312();

  /* Get the Radio Link Failure timer T313 */
  config_ptr->l1_dl_chan_parms.sync_det.T313 =
    (uint8)TDSRRC_GET_T313();

  /* Indicate that L1 Downlink needs to be reconfigured */
  config_ptr->reconfig_needed.downlink_l1 = TRUE;
  /* Although no new parameters for MAC have been received, MAC
  * still needs to be reconfigured so as to get the new CCTrCH
  * ID whenever Downlink L1 is reconfigured. This is due to the
  * interface requirements.
  */
  config_ptr->reconfig_needed.downlink_mac = TRUE;

  return(SUCCESS);
} /*  tdsrrcllc_get_timers_and_const_in_conn_mode */

/*====================================================================
FUNCTION: tdsrrcllc_use_connected_mode_sibs

DESCRIPTION:
  This function checks if connected mode SIBs are to be used.

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE : If connected mode sibs to be used 
  FALSE: Otherwise 

SIDE EFFECTS:
====================================================================*/
static boolean tdsrrcllc_use_connected_mode_sibs(tdsrrc_state_e_type state)
{
  boolean use_conn_mode_sibs = FALSE;

  /* Check the state to find out what initializations to perform on OC */
  switch(state)
  {
    case TDSRRC_STATE_CELL_DCH:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid state to read sibs");
      /* No need to set use_conn_mode_sibs to FALSE as it is already initialized to FALSE */
      break;

    case TDSRRC_STATE_DISCONNECTED:
    case TDSRRC_STATE_CONNECTING:
      /* No need to set use_conn_mode_sibs to FALSE as it is already initialized to FALSE */
      break;

    case TDSRRC_STATE_CELL_FACH:
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      use_conn_mode_sibs = TRUE;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RRC state %d passed", state);
      /* No need to set use_conn_mode_sibs to FALSE as it is already initialized to FALSE */

  } /* end switch */

  return (use_conn_mode_sibs);
}
/*====================================================================
FUNCTION: tdsrrcllc_copy_sib_to_oc

DESCRIPTION:
  This function takes a pointer to the serving cell data base for
  SIB 5 and copies all applicable IEs for RLC, MAC and L1 and places
  it into an internal format from the format received and processed
  over-the-air.

DEPENDENCIES:
  SIB 6 is currently not supported.

RETURN VALUE:
  None.

SIDE EFFECTS:
  If SIB 6 is available, SIB 6 data will instead be used in place of
  SIB 5 data. In other words, the correct SIB data will be used.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_copy_sib_to_oc(tdsrrc_state_e_type next_rrc_state)
{
  tdsrrc_SysInfoType1    *sib1_ptr = NULL;
  tdsrrc_SysInfoType3    *sib3_ptr = NULL;
  tdsrrc_SysInfoType4    *sib4_ptr = NULL;
  tdsrrc_SysInfoType5    *sib5_ptr = NULL;
  tdsrrc_SysInfoType6    *sib6_ptr = NULL;
  tdsrrc_SysInfoType7    *sib7_ptr = NULL;
  tdsrrc_MappingFunctionParameterList *sib_3_4_mapping_v4b0ext_ptr = NULL;

  /* Pointer & counter to loop through the CN System Information List */
  tdsrrc_CN_DomainSysInfoList * cn_ptr = NULL;
  uint8 cn_cnt                    = 0;

  /* Pointer & counter to loop through the Dynamic Persistence level values */
  tdsrrc_DynamicPersistenceLevelList *dpl_ptr = NULL;
  uint8 dpl_cnt                                   = 0;
  /* Added a parameter to distinguish between getting sccpchinfo from sib5 or sib6, beacuse for future add new 
     SCCPCHinfo list extension, we change code handle from SCCPCHinfolist and SCCPCHinfolist-LCR-R4 to upper 
     structure which is sib5(or sib6). 
     sib6_present_flg is ture means get sccpch info from sib6, 
     sib6_present_flg is false means get sccpch info from sib5. */
  uint8 sib6_present_flg = FALSE;              

    /* If we are going to a connected mode state, we must use the connected mode
     specific SIBs if they are available. */
  boolean use_conn_mode_sibs = tdsrrcllc_use_connected_mode_sibs(next_rrc_state);

  /************************/
  /* Get SIB 1 data first */
  /************************/
  sib1_ptr = (tdsrrc_SysInfoType1 *)
               tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1);
  if (sib1_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 1 not yet in SIB data base");
     return FAILURE;
  }

  /* Get the CS and PS DRX Cycle Lengths */
  cn_ptr = &sib1_ptr->cn_DomainSysInfoList;

  while((cn_cnt < cn_ptr->n ) && (cn_cnt < UE_MAX_CN_DOMAINS))
  {
    if((cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff >= TDSRRCLLC_CN_DOMAIN_MIN_DRX_CYCLE_LENGTH) &&
       (cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff <= TDSRRCLLC_CN_DOMAIN_MAX_DRX_CYCLE_LENGTH))
    {
      switch(cn_ptr->elem[cn_cnt].cn_DomainIdentity)
      {
        case tdsrrc_CN_DomainIdentity_cs_domain:
          tdsordered_config_ptr->cs_drx_cycle_length =
            (uint32) (1 << cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff);
          break;
        case tdsrrc_CN_DomainIdentity_ps_domain:
          tdsordered_config_ptr->ps_drx_cycle_length =
            (uint32) (1 << cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff);
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CN 0x%x for DRX", cn_ptr->elem[cn_cnt].cn_DomainIdentity);
          return FAILURE;
      } /* end CN Identity switch */
    } /* end drx cycle length check */
    else
    {
      /* A SIB with invalid DRX length coefficient is unacceptable */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid DRX len value %d, CN domain %d",
          cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff, cn_cnt);
      return FAILURE;
    }
    /* Next CN Info */
    cn_cnt++;
  } /* end while Core Network Info */

  /* Check to make sure both Core Networks have good DRX info */
  if(tdsordered_config_ptr->ps_drx_cycle_length == 0)
  {
    /* This indicates PS domain not present */
    if(tdsordered_config_ptr->cs_drx_cycle_length > 0)
    {
      tdsordered_config_ptr->ps_drx_cycle_length =
        tdsordered_config_ptr->cs_drx_cycle_length;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Both PS and CS DRX info missing");
      return FAILURE;
    }
  }

  if(tdsordered_config_ptr->cs_drx_cycle_length == 0)
  {
    /* This indicates CS domain not present */
    if(tdsordered_config_ptr->ps_drx_cycle_length > 0)
    {
      tdsordered_config_ptr->cs_drx_cycle_length =
        tdsordered_config_ptr->ps_drx_cycle_length;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Both PS and CS DRX info missing");
      return FAILURE;
    }
  }

  /* Only T312 and N312 in Idle mode timers and constants are needed
   * by L1. There is no need to read them in Idle mode since they are
   * going to be used only in Connected mode.
   */
  if(tdsrrcllc_get_timers_and_const_in_conn_mode(tdsordered_config_ptr, sib1_ptr) == FAILURE)
  {
    return(FAILURE);
  }

  /************************/
  /* Get SIB 3 data next  */
  /************************/
  sib3_ptr = (tdsrrc_SysInfoType3 *)
             tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB3);
  if (sib3_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 3 not yet in SIB data base");
     return FAILURE;
  }

  if(use_conn_mode_sibs && (sib3_ptr->sib4indicator == TRUE) && ((sib4_ptr = (tdsrrc_SysInfoType4 *)
                                                                  tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB4)) != NULL))
  {
    if((sib4_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power >= TDSRRCLLC_MIN_ALLOW_UL_TX_PWR) &&
       (sib4_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power <= TDSRRCLLC_MAX_ALLOW_UL_TX_PWR))
    {
      tdsordered_config_ptr->max_tx_pwr =
        (int8) sib4_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power;
      /* Indicate that L1 UL needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcv'd invalid UL TX PWR %d", sib4_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power);
      return FAILURE;
    }

    /* Set UpPCH_shift as default value at first */
    tdsordered_config_ptr->l1_ul_chan_parms.prach_cfg.upPch_shift = 0;

    /* Check if SIB4 has configured UpPCH_shift */
    if (sib4_ptr->m.v4b0NonCriticalExtensionsPresent)
    {
      if (sib4_ptr->v4b0NonCriticalExtensions.sysInfoType4_v4b0ext.m.mapping_LCRPresent)
      {
        sib_3_4_mapping_v4b0ext_ptr = 
           &(sib4_ptr->v4b0NonCriticalExtensions.sysInfoType4_v4b0ext.mapping_LCR.mappingFunctionParameterList);

        if (sib_3_4_mapping_v4b0ext_ptr->elem[0].m.mapParameter1Present)
        {
          tdsordered_config_ptr->l1_ul_chan_parms.prach_cfg.upPch_shift =
            sib_3_4_mapping_v4b0ext_ptr->elem[0].mapParameter1;
        }
      }
    }
  } /* If next state == Cell_FACH AND SIB 4 is present */
  else
  {
    if((sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power >= TDSRRCLLC_MIN_ALLOW_UL_TX_PWR) &&
       (sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power <= TDSRRCLLC_MAX_ALLOW_UL_TX_PWR)
      )
    {
      tdsordered_config_ptr->max_tx_pwr =
        (int8) sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power;
      /* Indicate that L1 UL needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcv'd invalid UL TX PWR %d", sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power);
      return FAILURE;
    }

    /* Set UpPCH_shift as default value at first */
    tdsordered_config_ptr->l1_ul_chan_parms.prach_cfg.upPch_shift = 0;

    /* Check if SIB3 has configured UpPCH_shift */
    if (sib3_ptr->m.v4b0NonCriticalExtensionsPresent)
    {
      if (sib3_ptr->v4b0NonCriticalExtensions.sysInfoType3_v4b0ext.m.mapping_LCRPresent)
      {
        sib_3_4_mapping_v4b0ext_ptr = 
           &(sib3_ptr->v4b0NonCriticalExtensions.sysInfoType3_v4b0ext.mapping_LCR.mappingFunctionParameterList);

        if (sib_3_4_mapping_v4b0ext_ptr->elem[0].m.mapParameter1Present)
        {
          tdsordered_config_ptr->l1_ul_chan_parms.prach_cfg.upPch_shift =
            sib_3_4_mapping_v4b0ext_ptr->elem[0].mapParameter1;
        }
      }
    }
  }

  /************************/
  /* Get SIB 7 data next  */
  /************************/
  sib7_ptr = (tdsrrc_SysInfoType7 *)
             tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB7);
  if (sib7_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 7 not yet in SIB data base");
     return FAILURE;
  }


  if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib7_ptr->modeSpecificInfo, rrc_SysInfoType7_modeSpecificInfo_fdd))
  {

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"FDD mode choice received");
    return FAILURE;
  }

  /* We have to wait until we read SIB 5 before we can get the Dynamic
     Persistence level so we know whether to get the values for SIB 5 or
     SIB 6. */

  /************************/
  /* Get SIB 5 data next  */
  /************************/
  sib5_ptr = (tdsrrc_SysInfoType5 *)
             tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB5);
  if (sib5_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 5 not yet in SIB data base");
     return FAILURE;
  }
  if(use_conn_mode_sibs && (sib5_ptr->sib6indicator == TRUE))
  {
    sib6_ptr = (tdsrrc_SysInfoType6 *)
               tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB6);
    if (sib6_ptr == NULL)
    {
       /* SIB 6 may not be present. That is acceptable */
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 6 not in SIB data base");
    }
  }

  /* Get the Dynamic Persistence level */
  if(use_conn_mode_sibs && (sib5_ptr->sib6indicator == TRUE))
  {
    dpl_ptr = &sib7_ptr->prach_Information_SIB6_List;
  }
  else
  {
    dpl_ptr = &sib7_ptr->prach_Information_SIB5_List;
  }
  while((dpl_cnt < dpl_ptr->n ) && (dpl_cnt < UE_MAX_PRACH))
  {
    tdsordered_config_ptr->dpl = dpl_ptr->elem[dpl_cnt];
    dpl_cnt++;
  }

  /* Get the PICH Power offset */
  if(sib6_ptr && use_conn_mode_sibs)
  {
    if(((int8)sib6_ptr->pich_PowerOffset >= TDSRRCLLC_MIN_PICH_PWR_OFFSET) &&
       ((int8)sib6_ptr->pich_PowerOffset <= TDSRRCLLC_MAX_PICH_PWR_OFFSET))
    {
      tdsordered_config_ptr->l1_dl_chan_parms.pich.pwr_offset =
        (int8)sib6_ptr->pich_PowerOffset;
      /* Indicate that L1 DL needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
       * still needs to be reconfigured so as to get the new CCTrCH
       * ID whenever Downlink L1 is reconfigured. This is due to the
       * interface requirements.
       */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"invalid PICH Power Offset %d", (int8)sib6_ptr->pich_PowerOffset);
      return(FAILURE);
    }
  }
  else
  {
    if(((int8)sib5_ptr->pich_PowerOffset >= TDSRRCLLC_MIN_PICH_PWR_OFFSET) &&
       ((int8)sib5_ptr->pich_PowerOffset <= TDSRRCLLC_MAX_PICH_PWR_OFFSET))
    {
      tdsordered_config_ptr->l1_dl_chan_parms.pich.pwr_offset =
        (int8)sib5_ptr->pich_PowerOffset;
      /* Indicate that L1 DL needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
       * still needs to be reconfigured so as to get the new CCTrCH
       * ID whenever Downlink L1 is reconfigured. This is due to the
       * interface requirements.
       */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"invalid PICH Power Offset %d", (int8)sib5_ptr->pich_PowerOffset);
      return(FAILURE);
    }
  } /* SIB 6 ptr is NULL */

  /* Get the Primary CCPCH Tx Power */
  if(sib6_ptr && use_conn_mode_sibs)
  {
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib6_ptr->modeSpecificInfo, rrc_SysInfoType6_modeSpecificInfo_tdd))
    {
      if(((int8) sib6_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power >=
          TDSRRCLLC_MIN_PRI_CCPCH_TX_PWR) &&
         ((int8) sib6_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power <=
          TDSRRCLLC_MAX_PRI_CCPCH_TX_PWR))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.prach_cfg.pccpch_tx_pwr =
          (int8) sib6_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power;
        /* Indicate that L1 UL needs to be reconfigured */
        tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
        /* Although no new parameters for MAC have been received, MAC
         * still needs to be reconfigured so as to get the new CCTrCH
         * ID whenever Uplink L1 is reconfigured. This is due to the
         * interface requirements.
         */
        tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d Primary CCPCH Tx Power",
            sib6_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power);
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cell TDD info absent");
      return FAILURE;
    }
  }
  else
  {
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib5_ptr->modeSpecificInfo, rrc_SysInfoType5_modeSpecificInfo_tdd))
    {
      if(((int8) sib5_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power >=
            TDSRRCLLC_MIN_PRI_CCPCH_TX_PWR) &&
         ((int8) sib5_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power <=
            TDSRRCLLC_MAX_PRI_CCPCH_TX_PWR))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.prach_cfg.pccpch_tx_pwr  =
          (int8) sib5_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power;
        /* Indicate that L1 UL needs to be reconfigured */
        tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
        /* Although no new parameters for MAC have been received, MAC
         * still needs to be reconfigured so as to get the new CCTrCH
         * ID whenever Uplink L1 is reconfigured. This is due to the
         * interface requirements.
         */
        tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d Primary CCPCH Tx Power",
            sib5_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power);
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cell TDD info absent");
      return FAILURE;
    }
  }  /* SIB 6 ptr is NULL */

  /* Check to see if Primary PCCPCH info is present. If so, save it. */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib5_ptr,tdsrrc_SysInfoType5,primaryCCPCH_Info))
  {
    /* This information is present */
    /* Note, TDSORDERED_CONFIG is always filled out for the serving cell */
    // new function call here due to modularization
    // rrcllc_set_pccpch(sib5_ptr->primaryCCPCH_Info.u.fdd.tx_DiversityIndicator);
  }
  /* else this IE is not present */
  /* add tstd_indicator indicator from SIB5\6 */
  if(sib6_ptr && use_conn_mode_sibs)
  {
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib6_ptr, v4b0NonCriticalExtensions))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib6_ptr->v4b0NonCriticalExtensions,sysInfoType6_v4b0ext))
      {
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext,
            tdd128SpecificInfo))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.tdd128SpecificInfo,
            pCCPCH_LCR_Extensions))
          {
            tdsordered_config_ptr->l1_dl_chan_parms.pccpch.tstd_indicator =
                sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.tdd128SpecificInfo.pCCPCH_LCR_Extensions.tstd_Indicator;
          }
        }
      }
    }
    /* Get from SIB5 */
    else if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib5_ptr, v4b0NonCriticalExtensions))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib5_ptr->v4b0NonCriticalExtensions, sysInfoType5_v4b0ext))
      {
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext,
            tdd128SpecificInfo))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.tdd128SpecificInfo,
            pCCPCH_LCR_Extensions))
          {
            tdsordered_config_ptr->l1_dl_chan_parms.pccpch.tstd_indicator =
                sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.tdd128SpecificInfo.pCCPCH_LCR_Extensions.tstd_Indicator;
          }
        }
      }
    }
  }
  else
  {
    /* GET from SIB5 */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib5_ptr, v4b0NonCriticalExtensions))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib5_ptr->v4b0NonCriticalExtensions, sysInfoType5_v4b0ext))
      {
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext,
            tdd128SpecificInfo))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.tdd128SpecificInfo,
            pCCPCH_LCR_Extensions))
          {
            tdsordered_config_ptr->l1_dl_chan_parms.pccpch.tstd_indicator =
                sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.tdd128SpecificInfo.pCCPCH_LCR_Extensions.tstd_Indicator;
          }
        }
      }
    }
  }

  /* Now get the PRACH system information */
  if(sib6_ptr && use_conn_mode_sibs)
  {
    if((sib6_ptr->m.prach_SystemInformationListPresent)&&
       (sib6_ptr->m.v4b0NonCriticalExtensionsPresent)&&
       (sib6_ptr->v4b0NonCriticalExtensions.m.sysInfoType6_v4b0extPresent)&&
       (sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.m.prach_SystemInformationList_LCR_r4Present))
    {
      if(tdsrrcllc_get_prach_sys_info(tdsordered_config_ptr, sib6_ptr, TRUE) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating PRACH Info");
        return FAILURE;
      }
    } /* If PRACH Info present in SIB6 */
    else  /* Read it from SIB5 */ /* CHECK_VN */
    {

      if(tdsrrcllc_get_prach_sys_info(tdsordered_config_ptr, sib5_ptr, FALSE) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating PRACH Info");
        return FAILURE;
      }
    }
  }
  else
  {

    if(tdsrrcllc_get_prach_sys_info(tdsordered_config_ptr, sib5_ptr, FALSE) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating PRACH Info");
      return FAILURE;
    }
  }


  /* Clause 8.6.5.1 of 25.331 specifies that we must ignore this SIB if the
     logical channel restriction type is set to explicit list and this is
     a RACH TFS.  We accomplish this (if necessary) by resetting the
     mac_update_needed flag for our RACH restriction information to FALSE.
     Although the tdsrrcllc_populate_mac_rlc_size_restriction_info() function
     still gets called, nothing is done with the MAC structures, since this
     flag is set to FALSE. */
  if(tdsordered_config_ptr->rach_rlc_size_restriction_info.mac_update_needed)
  {
    int i;
    for(i = 0; i < MAX_DED_LOGCHAN_PER_TRCH; i++)
    {
      if(tdsordered_config_ptr->
         rach_rlc_size_restriction_info.rlc_size_list[i].restriction_type ==
         TDSRLC_SIZES_EXPLICIT_LIST)
      {
        tdsordered_config_ptr->rach_rlc_size_restriction_info.mac_update_needed =
          FALSE;
      }
    }
  }

  /* Fill in the RLC size restriction information for MAC */
  (void)tdsrrcllc_populate_mac_rlc_size_restriction_info(tdsordered_config_ptr,
                                                      tdsordered_config_ptr->
                                                      rrc_state_indicator);


  /* Now get the SCCPCH system information */
  if(use_conn_mode_sibs)
  {
    if ((tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSNO_RNTI_VALID) ||
       (tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSCRNTI_VALID))
    {
     /* We are going to be in one of Connected mode states and a valid URNTI
       * is NOT present - return from here - do not process SCCPCH info.
       */
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"RNTI not valid %d",tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid);
      /* Set the Config Pointer in Use */
      tdsconfig_ptr_in_use = TDSORDERED_CONFIG;

      return(SUCCESS);
    }
  }

  if(sib6_ptr && use_conn_mode_sibs)
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib6_ptr,sCCPCH_SystemInformationList)) &&
        (TDSRRCLLC_SIB6_SCCPCH_LCR_EXT_PRESENT(sib6_ptr)))
    {
      if(tdsrrcllc_verify_sccpch_exists(sib6_ptr,
                              next_rrc_state)
         == TRUE)
      {
        /* If SCCPCH Info present in SIB6, read info from there */
        /* Notify function tdsrrcllc_get_sccpch_sys_info() should get sccpchinfo from sib6 */
        sib6_present_flg = TRUE;
        if(tdsrrcllc_get_sccpch_sys_info(tdsordered_config_ptr,
                               sib6_ptr,
                               sib6_present_flg,
                               next_rrc_state)
         == FAILURE)
        {
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
         return FAILURE;
        }
      } /* SCCPCH with FACH exists */
      else
      {
        if(tdsrrcllc_get_sccpch_sys_info(tdsordered_config_ptr,
                               sib5_ptr,
                               sib6_present_flg,
                               next_rrc_state)
         == FAILURE)
        {
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
         return FAILURE;
        }
      } /* SCCPCH with FACH does NOT exist in SIB6 */
    } /* SCCPCH system info list present in SIB 6 */
    else
    {
      /* SCCPCH INfo not present in SIB6, read it from SIB 5 */
      /* CHECK_VN */
      if(tdsrrcllc_get_sccpch_sys_info(tdsordered_config_ptr,
                             sib5_ptr,
                             sib6_present_flg,
                             next_rrc_state)
       == FAILURE)
      {
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
       return FAILURE;
      }
    }
  } /* SIB6 present and next state is CELL_FACH, CELL_PCH, URA_PCH */
  else /* SCCPCH system info in SIB 5 has to be used */
  {
    if(tdsrrcllc_get_sccpch_sys_info(tdsordered_config_ptr,
                           sib5_ptr,
                           sib6_present_flg,
                           next_rrc_state)
       == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
      return FAILURE;
    }
  }

  /*
  * Note that since CTCH is not initially supported the CBS DRX
  * Level 1 information is not decoded here.
  */

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  /* Getting CBS related info.  Get the Period of CTCH allocation (N) and
     CBS frame offset (K)  */
  if (next_rrc_state == TDSRRC_STATE_DISCONNECTED || next_rrc_state == TDSRRC_STATE_CELL_PCH ||
      next_rrc_state == TDSRRC_STATE_URA_PCH )
  {
    if (tdsrrcllc_semi_permanent_data.dl_phy_chan.ctch_status == TDSRRCLLC_CTCH_WITH_PCCH)
    {
      tdstordered_config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.ctch_info_present = TRUE;
      tdsordered_config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.only_ctch_on_sccpch = FALSE;
/*When UE is in connected mode, it ignores any CTCH info present in SIB6 and only uses info in SIB5*/
      tdsordered_config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.cbs_frame_offset =
        (uint16)sib5_ptr->cbs_DRX_Level1Information.cbs_FrameOffset;
      tdsordered_config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.ctch_allocation_period =
        (uint16)sib5_ptr->cbs_DRX_Level1Information.ctch_AllocationPeriod;
    }
  }
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

  /* Set the Config Pointer in Use */
  tdsconfig_ptr_in_use = TDSORDERED_CONFIG;


  return SUCCESS;
} /* end tdsrrcllc_copy_sib_to_oc() */

/*====================================================================
FUNCTION: tdsrrcllc_copy_cc_to_oc

DESCRIPTION:
  This function copies the current config and places it in ordered
  config. It also updates the tdsordered_config pointer.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_copy_cc_to_oc
(
  void
)
{
  memscpy(tdsordered_config_ptr, sizeof(tdsordered_config_type),tdscurrent_config_ptr, sizeof(tdsordered_config_type));
  /* Now initialize all pointers in interface structures within
   * current config.
   * TBD: Remove these pointers in interface structures.
   */
  /* Initialize all pointers in L1, MAC and RLC structures */
  tdsrrcllc_init_mac_dl_ptrs(tdsordered_config_ptr);
  tdsrrcllc_init_mac_ul_ptrs(tdsordered_config_ptr);
  tdsrrcllc_init_l1_dl_ptrs(tdsordered_config_ptr);
  tdsrrcllc_init_l1_ul_ptrs(tdsordered_config_ptr);
  tdsrrcllc_init_rlc_ptrs(tdsordered_config_ptr);

  /* Initialize Reconfig needed */
  tdsrrcllc_init_reconfig_needed(tdsordered_config_ptr);

  tdsconfig_ptr_in_use = TDSORDERED_CONFIG;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copy CC to OC");

  tdsordered_config_ptr->mac_ul_parms.new_tfcs_list_present = FALSE;

  return;
} /* end tdsrrcllc_copy_cc_to_oc() */

/*====================================================================
FUNCTION: tdsrrcllc_copy_oc_to_cc

DESCRIPTION:
  This function copies the Ordered config and places it in Current
  config. It also updates the current_config pointer.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_copy_oc_to_cc
(
  void
)
{
  memscpy(tdscurrent_config_ptr,sizeof(tdsordered_config_type), tdsordered_config_ptr, sizeof(tdsordered_config_type));
  /* Now initialize all pointers in interface structures within
   * current config.
   * TBD: Remove these pointers in interface structures.
   */
  /* Initialize all pointers in L1, MAC and RLC structures */
  tdsrrcllc_init_mac_dl_ptrs(tdscurrent_config_ptr);
  tdsrrcllc_init_mac_ul_ptrs(tdscurrent_config_ptr);
  tdsrrcllc_init_l1_dl_ptrs(tdscurrent_config_ptr);
  tdsrrcllc_init_l1_ul_ptrs(tdscurrent_config_ptr);
  tdsrrcllc_init_rlc_ptrs(tdscurrent_config_ptr);

  /* Initialize Reconfig needed */
  tdsrrcllc_init_reconfig_needed(tdscurrent_config_ptr);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copy OC to CC");
  return;
} /* end tdsrrcllc_copy_oc_to_cc() */

/*====================================================================
FUNCTION: tdsrrcllc_copy_oc_to_toc

DESCRIPTION:
  This function copies the Ordered config to Transitional Ordered
  Config during a CELL_DCH to CELL_FACH state transition.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllc_copy_oc_to_toc
(
  void
)
{
  memscpy(tdstransition_config.toc_ptr,sizeof(tdsordered_config_type), tdsordered_config_ptr, sizeof(tdsordered_config_type));
  /* Now initialize all pointers in interface structures within
   * current config.
   * TBD: Remove these pointers in interface structures.
   */
  /* Initialize all pointers in L1, MAC and RLC structures */
  tdsrrcllc_init_mac_dl_ptrs(tdstransition_config.toc_ptr);
  tdsrrcllc_init_mac_ul_ptrs(tdstransition_config.toc_ptr);
  tdsrrcllc_init_l1_dl_ptrs(tdstransition_config.toc_ptr);
  tdsrrcllc_init_l1_ul_ptrs(tdstransition_config.toc_ptr);
  tdsrrcllc_init_rlc_ptrs(tdstransition_config.toc_ptr);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copy OC to TOC");

#ifdef FEATURE_UMTS_PDCP
  tdsrrcllc_print_pdcp_info(tdstransition_config.toc_ptr);
#endif /* FEATURE_UMTS_PDCP*/

  return;
} /* end tdsrrcllc_copy_oc_to_toc() */


/*====================================================================
FUNCTION: tdsrrcllc_process_non_critcal_trch_reconfig_ie_r5

DESCRIPTION:
  This function acts on the NON Critical IE for Trch Reconfig R5

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllc_process_non_critcal_trch_reconfig_ie_r5
(
  tdsrrc_DL_DCCH_Message *dcch_ptr
)
{
  tdsrrc_TransportChannelReconfiguration_r5 *rrc_trch_reconfig = &dcch_ptr->message.u.transportChannelReconfiguration.u.
            later_than_r3.criticalExtensions.u.criticalExtensions.u.r5;

  if(rrc_trch_reconfig->m.v690NonCriticalExtensionsPresent)
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_trch_reconfig->v690NonCriticalExtensions.transportChannelReconfiguration_v690ext,
                 tdsrrc_TransportChannelReconfiguration_v690ext_IEs,
                 delayRestrictionFlag))
    {
      if((TDSRRC_MSG_COMMON_BITMASK_IE(rrc_trch_reconfig->transportChannelReconfiguration_r5,
                  tdsrrc_TransportChannelReconfiguration_r5_IEs,
                  ul_AddReconfTransChInfoList ))
          ||(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_trch_reconfig->transportChannelReconfiguration_r5,
                  tdsrrc_TransportChannelReconfiguration_r5_IEs,
                  dl_AddReconfTransChInfoList )
      ))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Trch Ie touched bitmask , so Delay restriction cannot be applied");
      }
      else
      {
        tdsrrcllc_update_delay_restriction(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_trch_reconfig->transportChannelReconfiguration_r5,
                   tdsrrc_TransportChannelReconfiguration_r5_IEs,
                   activationTime),
             TRUE,
             rrc_trch_reconfig->transportChannelReconfiguration_r5.activationTime,
             rrc_trch_reconfig->transportChannelReconfiguration_r5.rrc_StateIndicator
          );
      }
    }
  }
 
}


/*====================================================================
FUNCTION: tdsrrcllc_process_non_critcal_rb_reconfig_ie_r5

DESCRIPTION:
  This function acts on the NON Critical IE for RB Reconfig R5

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllc_process_non_critcal_rb_reconfig_ie_r5
(
  tdsrrc_DL_DCCH_Message *dcch_ptr
)
{
  tdsrrc_RadioBearerReconfiguration_r5 *rrc_rb_reconfig = &dcch_ptr->message.u.radioBearerReconfiguration.u.
            later_than_r3.criticalExtensions.u.criticalExtensions.u.r5;
  tdsrrc_RadioBearerReconfiguration_r5_IEs *r5_rb_reconfig;

  r5_rb_reconfig = &rrc_rb_reconfig->radioBearerReconfiguration_r5;
  if((rrc_rb_reconfig->m.v5d0NonCriticalExtenstionsPresent)
          && (rrc_rb_reconfig->v5d0NonCriticalExtenstions.m.v690NonCriticalExtensionsPresent))
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_rb_reconfig->v5d0NonCriticalExtenstions.v690NonCriticalExtensions.radioBearerReconfiguration_v690ext,
          tdsrrc_RadioBearerReconfiguration_v690ext_IEs,delayRestrictionFlag))
    {
      if((TDSRRC_CHECK_COMMON_MSG_TYPE(r5_rb_reconfig->specificationMode,rrc_RadioBearerReconfiguration_r5_IEs_specificationMode_complete ))
          && (  (r5_rb_reconfig->specificationMode.u.complete.m.dl_AddReconfTransChInfoListPresent )
          ||(r5_rb_reconfig->specificationMode.u.complete.m.ul_AddReconfTransChInfoListPresent )
         ||(r5_rb_reconfig->specificationMode.u.complete.m.ul_deletedTransChInfoListPresent )
         ||(r5_rb_reconfig->specificationMode.u.complete.m.dl_DeletedTransChInfoListPresent ))
     
       )
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Trch Ie touched , so Delay restriction cannot be applied");
      }
      else
      {
        tdsrrcllc_update_delay_restriction(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_rb_reconfig->radioBearerReconfiguration_r5,
                tdsrrc_RadioBearerReconfiguration_r5_IEs,activationTime),
                 TRUE,
                 rrc_rb_reconfig->radioBearerReconfiguration_r5.activationTime,
                 rrc_rb_reconfig->radioBearerReconfiguration_r5.rrc_StateIndicator
              );
      }
    }
  }
 
}

/*====================================================================
FUNCTION: tdsrrcllc_process_non_critcal_phys_reconfig_ie_r5

DESCRIPTION:
  This function acts on the NON Critical IE for Phys Reconfig R5

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_process_non_critcal_phys_reconfig_ie_r5
(
  tdsrrc_DL_DCCH_Message *dcch_ptr
)
{
  tdsrrc_PhysicalChannelReconfiguration_r5 *rrc_phys_chan_reconfig = &dcch_ptr->message.u.physicalChannelReconfiguration.u.
          later_than_r3.criticalExtensions.u.criticalExtensions.u.r5;
  
  if(rrc_phys_chan_reconfig->m.v690NonCriticalExtensionsPresent)
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_phys_chan_reconfig->v690NonCriticalExtensions.
        physicalChannelReconfiguration_v690ext,tdsrrc_PhysicalChannelReconfiguration_v690ext_IEs,
        delayRestrictionFlag))
    {
      tdsrrcllc_update_delay_restriction(TDSRRC_MSG_COMMON_BITMASK_IE(
           rrc_phys_chan_reconfig->physicalChannelReconfiguration_r5,
              tdsrrc_PhysicalChannelReconfiguration_r5_IEs,activationTime),
              TRUE,
              rrc_phys_chan_reconfig->physicalChannelReconfiguration_r5.activationTime,
              rrc_phys_chan_reconfig->physicalChannelReconfiguration_r5.rrc_StateIndicator
           );
    }
  }
 
}

/*====================================================================
FUNCTION: tdsrrcllc_process_conn_setup_msg

DESCRIPTION:
  This function processes the RRC Connection Setup msg  received on CCCH based on the REL version and
  updates tdsordered_config with the latest IEs recv'd by RRC Connection Setup msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_conn_setup_msg
(
  uint32 dl_sdu_num,
  void *msg_ptr

)
{
  tdsrrc_DL_CCCH_Message *ccch_ptr;
  boolean                         fpach_freq_present = FALSE; 
  boolean                         uppch_positionInfo_present = FALSE; 
  uecomdef_status_e_type status = SUCCESS;
  ccch_ptr = (tdsrrc_DL_CCCH_Message *)msg_ptr;

  if ((TDSRRC_CHECK_COMMON_MSG_TYPE(ccch_ptr->message.u.rrcConnectionSetup,rrc_RRCConnectionSetup_later_than_r3)) &&
        (TDSRRC_CHECK_COMMON_MSG_TYPE(ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions,
            rrc_RRCConnectionSetup_criticalExtensions_6_r4)))
  {
    if(tdsrrcllc_rrc_conn_setup_ie_r4(&ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
      criticalExtensions.u.r4) == FAILURE)
    {
      status = FAILURE;
    }

    if (status == SUCCESS)
    {
      /* add ext value save process */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.r4,
          v4d0NonCriticalExtensions))
      {
        //save beaconPLEst
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
            v590NonCriticalExtensions))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
              criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions,
              v690NonCriticalExtensions))
          {
            tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                  ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.r4.
                  v4d0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.rrcConnectionSetup_v690ext,
                  beaconPLEst))
            {
              tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
            }
          }
        }
        //save r4_add_ext
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
          rrcConnectionSetup_r4_add_ext))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.rrcConnectionSetup_r4_add_ext,
              rrcConnectionSetup_r4_TDD128_add_ext))
          {
            if (FAILURE == tdsrrcget_add_ext_info(
              ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.rrcConnectionSetup_r4_add_ext.
                rrcConnectionSetup_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent,
              ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.rrcConnectionSetup_r4_add_ext.
                rrcConnectionSetup_r4_TDD128_add_ext.fPachFrequencyInfo,
              ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.rrcConnectionSetup_r4_add_ext.
                rrcConnectionSetup_r4_TDD128_add_ext.m.upPCHpositionInfoPresent,
              ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.rrcConnectionSetup_r4_add_ext.
                rrcConnectionSetup_r4_TDD128_add_ext.upPCHpositionInfo
              ))
            {
              status = FAILURE;
            }
            fpach_freq_present = ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.rrcConnectionSetup_r4_add_ext.
                rrcConnectionSetup_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent;
            uppch_positionInfo_present = ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.rrcConnectionSetup_r4_add_ext.
                rrcConnectionSetup_r4_TDD128_add_ext.m.upPCHpositionInfoPresent;
          }
        }
      }
    }
    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_rrc_conn_setup_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }

  else if (TDSRRCRB_R6_CRITICAL_EXT_PRESENT(
      ccch_ptr->message.u.rrcConnectionSetup,rrc_RRCConnectionSetup_))
  {
    if (FAILURE == tdsrrcllc_rrc_conn_setup_ie_r6(&ccch_ptr->message.u.rrcConnectionSetup.
      u.later_than_r3.criticalExtensions.
      u.criticalExtensions.
      u.criticalExtensions.
      u.r6.rrcConnectionSetup_r6
      ))
    {
      status = FAILURE;
    }
  }
  else if (TDSRRCRB_R7_CRITICAL_EXT_PRESENT(
      ccch_ptr->message.u.rrcConnectionSetup,rrc_RRCConnectionSetup_))
  {
    if (FAILURE == tdsrrcllc_rrc_conn_setup_ie_r7(&ccch_ptr->message.u.rrcConnectionSetup.
      u.later_than_r3.criticalExtensions.
      u.criticalExtensions.
      u.criticalExtensions.
       u.criticalExtensions.
      u.r7))
    {
      status = FAILURE;
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
  else if (TDSRRCRB_R8_CRITICAL_EXT_PRESENT(
      ccch_ptr->message.u.rrcConnectionSetup,rrc_RRCConnectionSetup_))
  {
    if (FAILURE == tdsrrcllc_rrc_conn_setup_ie_r8(&ccch_ptr->message.u.rrcConnectionSetup.
      u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r8))
    {
      status = FAILURE;
    }
  }
#endif/*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
    else if (TDSRRCRB_R9_CRITICAL_EXT_PRESENT(
        ccch_ptr->message.u.rrcConnectionSetup,rrc_RRCConnectionSetup_))
    {
      if (FAILURE == tdsrrcllc_rrc_conn_setup_ie_r9(&ccch_ptr->message.u.rrcConnectionSetup.
        u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.
        u.criticalExtensions.u.criticalExtensions.u.r9))
      {
        status = FAILURE;
      }
    }
#endif/*FEATURE_TDSCDMA_REL9*/

  else
  {
    if(tdsrrcllc_rrc_conn_setup_ie_r5(&ccch_ptr->message.u.rrcConnectionSetup.u.
         later_than_r3.criticalExtensions.u.
         criticalExtensions.u.r5)
                == FAILURE)
    {
      status = FAILURE;
    }

    if (status == SUCCESS)
    {
      //save beaconPLEst
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5,
        v690NonCriticalExtensions))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
        v690NonCriticalExtensions.rrcConnectionSetup_v690ext,
        beaconPLEst))
        {
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
        }
      }

      /* add ext value save process */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5,
        rrcConnectionSetup_r5_add_ext))
      {
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.rrcConnectionSetup_r5_add_ext,
            rrcConnectionSetup_r5_TDD128_add_ext))
        {
          if (FAILURE == tdsrrcget_add_ext_info(
            ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.rrcConnectionSetup_r5_add_ext.rrcConnectionSetup_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent,
            ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.rrcConnectionSetup_r5_add_ext.rrcConnectionSetup_r5_TDD128_add_ext.fPachFrequencyInfo,
            ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.rrcConnectionSetup_r5_add_ext.rrcConnectionSetup_r5_TDD128_add_ext.m.upPCHpositionInfoPresent,
            ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.rrcConnectionSetup_r5_add_ext.rrcConnectionSetup_r5_TDD128_add_ext.upPCHpositionInfo
            ))
          {
            status = FAILURE;
          }
          fpach_freq_present = ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.rrcConnectionSetup_r5_add_ext.rrcConnectionSetup_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent;
          uppch_positionInfo_present = ccch_ptr->message.u.rrcConnectionSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.rrcConnectionSetup_r5_add_ext.rrcConnectionSetup_r5_TDD128_add_ext.m.upPCHpositionInfoPresent;
        }
      }
    }
    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_rrc_conn_setup_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }
 return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_ccch_cell_update_cnf

DESCRIPTION:
  This function processes the Cell Update  msg  received on CCCH based on the REL version and
  updates tdsordered_config with the latest IEs recv'd by Cell Update  msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_ccch_cell_update_cnf
(
  uint32 dl_sdu_num,
  void *msg_ptr

)
{
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr = NULL;
#endif
  boolean                         fpach_freq_present = FALSE;
  boolean                         uppch_positionInfo_present = FALSE; 
  uecomdef_status_e_type status = SUCCESS;
  tdsrrc_DL_CCCH_Message *ccch_ptr;
  ccch_ptr = (tdsrrc_DL_CCCH_Message *)msg_ptr;

  if((TDSRRC_CHECK_COMMON_MSG_TYPE(ccch_ptr->message.u.cellUpdateConfirm,
           rrc_CellUpdateConfirm_CCCH_later_than_r3)) &&
       (TDSRRC_CHECK_COMMON_MSG_TYPE(ccch_ptr->message.u.cellUpdateConfirm.
             u.later_than_r3.criticalExtensions, rrc_CellUpdateConfirm_CCCH_criticalExtensions_6_r4))
       )
  {
    if ( ccch_ptr->message.u.cellUpdateConfirm.u.
                  later_than_r3.criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent)
    {
      if(tdsrrcllc_cell_update_confirm_ie_r4(&ccch_ptr->message.u.cellUpdateConfirm.u.
                  later_than_r3.criticalExtensions.u.r4.cellUpdateConfirm_r4, TRUE, 
                  &ccch_ptr->message.u.cellUpdateConfirm.u.
                  later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions, NULL)
            == FAILURE)
      {
        status = FAILURE;
      }
    }
    else
    {
      if(tdsrrcllc_cell_update_confirm_ie_r4(&ccch_ptr->message.u.cellUpdateConfirm.u.
                  later_than_r3.criticalExtensions.u.r4.cellUpdateConfirm_r4, TRUE, 
                  NULL, NULL)
            == FAILURE)
      {
        status = FAILURE;
      }
    }
    if (status == SUCCESS)
    {
      /* add ext value save process */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4,
          v4d0NonCriticalExtensions))
      {
        //save beaconPLEst
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
            v590NonCriticalExtensions))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions,
                v5d0NonCriticalExtenstions))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5d0NonCriticalExtenstions,
                v690NonCriticalExtensions))
            {
              tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
              if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
                    v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.cellUpdateConfirm_v690ext,
                    beaconPLEst))
              {
                tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
              }
            }
          }
        }

        //save r4_TDD128_add_ext
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
            r4.v4d0NonCriticalExtensions,
          cellUpdateConfirm_CCCH_r4_add_ext))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
                r4.v4d0NonCriticalExtensions.cellUpdateConfirm_CCCH_r4_add_ext,
              cellUpdateConfirm_CCCH_r4_TDD128_add_ext))
          {
            if (FAILURE == tdsrrcget_add_ext_info(
              ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
                r4.v4d0NonCriticalExtensions.cellUpdateConfirm_CCCH_r4_add_ext.
                cellUpdateConfirm_CCCH_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent,
              ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
                r4.v4d0NonCriticalExtensions.cellUpdateConfirm_CCCH_r4_add_ext.
                cellUpdateConfirm_CCCH_r4_TDD128_add_ext.fPachFrequencyInfo,
              ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
                r4.v4d0NonCriticalExtensions.cellUpdateConfirm_CCCH_r4_add_ext.
                cellUpdateConfirm_CCCH_r4_TDD128_add_ext.m.upPCHpositionInfoPresent,
              ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
                r4.v4d0NonCriticalExtensions.cellUpdateConfirm_CCCH_r4_add_ext.
                cellUpdateConfirm_CCCH_r4_TDD128_add_ext.upPCHpositionInfo
              ))
            {
              status = FAILURE;
            }
            fpach_freq_present = ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
                r4.v4d0NonCriticalExtensions.cellUpdateConfirm_CCCH_r4_add_ext.
                cellUpdateConfirm_CCCH_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent;
            uppch_positionInfo_present = ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
                r4.v4d0NonCriticalExtensions.cellUpdateConfirm_CCCH_r4_add_ext.
                cellUpdateConfirm_CCCH_r4_TDD128_add_ext.m.upPCHpositionInfoPresent;
          }
        }
      }
    }
    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_cell_update_confirm_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }
  else if ((T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_5_criticalExtensions == 
    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_4_criticalExtensions == 
    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_4_r6 == 
    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))
  {
    if(tdsrrcllc_cell_update_confirm_ie_r6(&ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                                      criticalExtensions.u.criticalExtensions.u.criticalExtensions.
                                      u.r6.cellUpdateConfirm_r6
                                  )
     == FAILURE)
    {
      status = FAILURE;
    }
  }
  else if ((T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_5_criticalExtensions == 
    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_4_criticalExtensions == 
    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_3_criticalExtensions == 
     ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)
     && (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_3_r7 & 
     ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))
  {
    if(tdsrrcllc_cell_update_confirm_ie_r7(dl_sdu_num,msg_ptr)
     == FAILURE)
    {
      status = FAILURE;
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
  else if (TDSRRCRB_R8_CU_CRITICAL_EXT_PRESENT_PTR(ccch_ptr,rrc_CellUpdateConfirm_CCCH))
  {
    if(tdsrrcllc_cell_update_confirm_ie_r8(dl_sdu_num,msg_ptr) == FAILURE)
    {
      status = FAILURE;
    }
  }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  else if ((T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_6_criticalExtensions == 
    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_5_criticalExtensions == 
    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_4_criticalExtensions == 
    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_3_criticalExtensions == 
    ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_2_criticalExtensions == 
     ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
        criticalExtensions.u.criticalExtensions.t)
     && (T_tdsrrc_CellUpdateConfirm_CCCH_criticalExtensions_1_r9 & 
     ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
       criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))
  {
    if(tdsrrcllc_cell_update_confirm_ie_r9(dl_sdu_num,msg_ptr)
     == FAILURE)
    {
      status = FAILURE;
    }
  }
#endif /*FEATURE_TDSCDMA_REL9*/
  else
  {
    //save beaconPLEst
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5,
        v5d0NonCriticalExtenstions))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions,
        v690NonCriticalExtensions))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions.
        v690NonCriticalExtensions.cellUpdateConfirm_v690ext,
        beaconPLEst))
        {
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
        }
      }
    }
    /* add ext value save process */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
      ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
        criticalExtensions.u.criticalExtensions.u.r5,
      cellUpdateConfirm_CCCH_r5_add_ext))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_CCCH_r5_add_ext,
          cellUpdateConfirm_CCCH_r5_TDD128_add_ext))
      {
        if (FAILURE == tdsrrcget_add_ext_info(
          ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_CCCH_r5_add_ext.cellUpdateConfirm_CCCH_r5_TDD128_add_ext.
            m.fPachFrequencyInfoPresent,
          ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_CCCH_r5_add_ext.cellUpdateConfirm_CCCH_r5_TDD128_add_ext.
            fPachFrequencyInfo,
          ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_CCCH_r5_add_ext.cellUpdateConfirm_CCCH_r5_TDD128_add_ext.
            m.upPCHpositionInfoPresent,
          ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_CCCH_r5_add_ext.cellUpdateConfirm_CCCH_r5_TDD128_add_ext.
            upPCHpositionInfo
          ))
        {
          status = FAILURE;
        }
        fpach_freq_present = ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_CCCH_r5_add_ext.cellUpdateConfirm_CCCH_r5_TDD128_add_ext.
            m.fPachFrequencyInfoPresent;
        uppch_positionInfo_present = ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_CCCH_r5_add_ext.cellUpdateConfirm_CCCH_r5_TDD128_add_ext.
            m.upPCHpositionInfoPresent;

        /* save power control gap and pathloss compensation switch */
        if (TRUE == ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.cellUpdateConfirm_CCCH_r5_add_ext.
            cellUpdateConfirm_CCCH_r5_TDD128_add_ext.m.powerControlGapPresent)
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap =
           ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
           criticalExtensions.u.r5.cellUpdateConfirm_CCCH_r5_add_ext.
           cellUpdateConfirm_CCCH_r5_TDD128_add_ext.powerControlGap;
        }
        else
        {
          /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;
        }
        
        if (TRUE == ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.
          u.criticalExtensions.u.r5.cellUpdateConfirm_CCCH_r5_add_ext.
          cellUpdateConfirm_CCCH_r5_TDD128_add_ext.m.pathlossCompensationSwitchPresent)
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
           ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
           criticalExtensions.u.r5.cellUpdateConfirm_CCCH_r5_add_ext.
           cellUpdateConfirm_CCCH_r5_TDD128_add_ext.pathlossCompensationSwitch;
        }
        else
        {
          /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */ 
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = FALSE;
        }

        /* Because UE does not support MultiCarrier, if received  messages contain 
                           the IE "DL-MultiCarrier-Information", set the variable INVALID_CONFIGURATION to TRUE. */
        if (tdsrrc_RRC_StateIndicator_cell_DCH == ccch_ptr->message.u.cellUpdateConfirm.u.
          later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.cellUpdateConfirm_r5.
          rrc_StateIndicator)
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_CCCH_r5_add_ext.cellUpdateConfirm_CCCH_r5_TDD128_add_ext,
            dl_MultiCarrier_Information))
          {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
            multicarrier_flg = TRUE;
            multiCarrier_info_ptr = &ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                 criticalExtensions.u.criticalExtensions.u.r5.
                 cellUpdateConfirm_CCCH_r5_add_ext.
                 cellUpdateConfirm_CCCH_r5_TDD128_add_ext.dl_MultiCarrier_Information;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Multi-Carrier Information present");
#else
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport configuration received DL Multi-Carrier Information reveived");
            status = FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
          }
        }
      }
    }
    if (status == SUCCESS)
    {
      if ( ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
              criticalExtensions.u.criticalExtensions.
                 u.r5.m.cellUpdateConfirm_CCCH_r5_add_extPresent)
      {
        if (tdsrrcllc_cell_update_confirm_ie_r5(&ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                        criticalExtensions.u.criticalExtensions.
                         u.r5.cellUpdateConfirm_r5, TRUE, 
                &ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                      criticalExtensions.u.criticalExtensions.
                         u.r5.cellUpdateConfirm_CCCH_r5_add_ext, NULL
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                         , multicarrier_flg, multiCarrier_info_ptr
#endif
                         ) == FAILURE)
        {
          status = FAILURE;
        }
      }
      else
      {
        if (tdsrrcllc_cell_update_confirm_ie_r5(&ccch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                        criticalExtensions.u.criticalExtensions.
                         u.r5.cellUpdateConfirm_r5, TRUE, NULL, NULL
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                           , multicarrier_flg, multiCarrier_info_ptr
#endif
                         ) == FAILURE)
        {
          status = FAILURE;
        }
      }
    }

    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_cell_update_confirm_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }
  return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_pcrc_msg

DESCRIPTION:
  This function processes the Physical Channel Reconfig msg based on the REL version and
  updates tdsordered_config with the latest IEs recv'd by Physical Channel Reconfig msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_pcrc_msg
(
  uint32 dl_sdu_num,
  void *msg_ptr

)
{
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr = NULL;
#endif
  /* local variables */
  uecomdef_status_e_type  status = SUCCESS;
  boolean                         fpach_freq_present = FALSE; 
  boolean                         uppch_positionInfo_present = FALSE; 
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;
         
  if ((TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.physicalChannelReconfiguration,
       rrc_PhysicalChannelReconfiguration_later_than_r3)) &&
     (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions,
          rrc_PhysicalChannelReconfiguration_criticalExtensions_6_r4)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 4 Msg");
    if(tdsrrcllc_phychan_reconfig_ie_r4(&dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
           criticalExtensions.u.r4) == FAILURE)
    {
       status = FAILURE;
    }
    else /*Here we update the delayRestriction that is coming in no-critical extension of the R4 OTA.*/
    {  
       tdsrrcllc_process_non_critcal_phys_reconfig_ie_r4(dcch_ptr);
    }

    /* add ext value save process */
    if (status == SUCCESS)
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.r4,
          v4d0NonCriticalExtensions))
      {
              //save beaconPLEst
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
            v590NonCriticalExtenstions))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
              criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtenstions,
              v690NonCriticalExtensions))
          {
            tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                  dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.u.r4.
                  v4d0NonCriticalExtensions.v590NonCriticalExtenstions.v690NonCriticalExtensions.physicalChannelReconfiguration_v690ext,
                  beaconPLEst))
            {
              tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
            }
          }
        }
        //save r4_add_ext
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
          physicalChannelReconfiguration_r4_add_ext))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.physicalChannelReconfiguration_r4_add_ext,
              physicalChannelReconfiguration_r4_TDD128_add_ext))
          {
            if (FAILURE == tdsrrcget_add_ext_info(
              dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.physicalChannelReconfiguration_r4_add_ext.
                physicalChannelReconfiguration_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent,
              dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.physicalChannelReconfiguration_r4_add_ext.
                physicalChannelReconfiguration_r4_TDD128_add_ext.fPachFrequencyInfo,
              dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.physicalChannelReconfiguration_r4_add_ext.
                physicalChannelReconfiguration_r4_TDD128_add_ext.m.upPCHpositionInfoPresent,
              dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.physicalChannelReconfiguration_r4_add_ext.
                physicalChannelReconfiguration_r4_TDD128_add_ext.upPCHpositionInfo
              ))
            {
              status = FAILURE;
            }
            fpach_freq_present = dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.physicalChannelReconfiguration_r4_add_ext.
                physicalChannelReconfiguration_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent;
            uppch_positionInfo_present = dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.physicalChannelReconfiguration_r4_add_ext.
                physicalChannelReconfiguration_r4_TDD128_add_ext.m.upPCHpositionInfoPresent;
          }
        }
      }
    }
    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_phychan_reconfig_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
 }
         else if (TDSRRCRB_R6_CRITICAL_EXT_PRESENT(
                  dcch_ptr->message.u.physicalChannelReconfiguration, 
                  rrc_PhysicalChannelReconfiguration_))
         {
           
           if (FAILURE == tdsrrcllc_phychan_reconfig_ie_r6(&dcch_ptr->message.u.physicalChannelReconfiguration.
                                                        u.later_than_r3.criticalExtensions.
                                                        u.criticalExtensions.
                                                        u.criticalExtensions.
                                                        u.r6.physicalChannelReconfiguration_r6
                                                        ))
           {
             status = FAILURE;
           }
         }
                 else if (TDSRRCRB_R7_CRITICAL_EXT_PRESENT(
                          dcch_ptr->message.u.physicalChannelReconfiguration, 
                          rrc_PhysicalChannelReconfiguration_))
                 {
                   if (FAILURE == tdsrrcllc_phychan_reconfig_ie_r7(
                                                                &dcch_ptr->message.u.physicalChannelReconfiguration.
                                                                u.later_than_r3.criticalExtensions.
                                                                u.criticalExtensions.
                                                                u.criticalExtensions.
                                                                u.criticalExtensions.
                                                                u.r7
                                                                ))
                   {
                     status = FAILURE;
                   }
                 }
#ifdef FEATURE_TDSCDMA_REL8
                 else if (TDSRRCRB_R8_CRITICAL_EXT_PRESENT(
                          dcch_ptr->message.u.physicalChannelReconfiguration, 
                          rrc_PhysicalChannelReconfiguration_
                          ))
                 {
                   if (FAILURE == tdsrrcllc_phychan_reconfig_ie_r8(
                                                                &dcch_ptr->message.u.physicalChannelReconfiguration.
                                                                u.later_than_r3.criticalExtensions.
                                                                u.criticalExtensions.
                                                                u.criticalExtensions.
                                                                u.criticalExtensions.
                                                                u.criticalExtensions.                                                       
                                                                u.r8
                                                                ))
                   {
                     status = FAILURE;
                   }
                 }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
        else if (TDSRRCRB_R9_CRITICAL_EXT_PRESENT(
             dcch_ptr->message.u.physicalChannelReconfiguration, 
             rrc_PhysicalChannelReconfiguration_
             ))
        {
          if (FAILURE == tdsrrcllc_phychan_reconfig_ie_r9(
                               &dcch_ptr->message.u.physicalChannelReconfiguration.
                               u.later_than_r3.criticalExtensions.
                               u.criticalExtensions.
                               u.criticalExtensions.
                               u.criticalExtensions.
                               u.criticalExtensions.
                               u.criticalExtensions.                           
                               u.r9
                               ))
          {
          status = FAILURE;
          }
        }
#endif /*FEATURE_TDSCDMA_REL9*/
         else
         {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 5 Msg");
           //save beaconPLEst
           if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
             dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5,
             v690NonCriticalExtensions))
           {
             tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
             if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
             dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
             v690NonCriticalExtensions.physicalChannelReconfiguration_v690ext,
             beaconPLEst))
             {
               tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
             }
           }
           //save r5_add_ext
           if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
             dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
               criticalExtensions.u.criticalExtensions.u.r5,
             physicalChannelReconfiguration_r5_add_ext))
           {
             if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                 dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                   criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext,
                 physicalChannelReconfiguration_r5_TDD128_add_ext))
             {
               if (FAILURE == tdsrrcget_add_ext_info(
                 dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                   criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                   physicalChannelReconfiguration_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent,
                 dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                   criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                   physicalChannelReconfiguration_r5_TDD128_add_ext.fPachFrequencyInfo,
                 dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                   criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                   physicalChannelReconfiguration_r5_TDD128_add_ext.m.upPCHpositionInfoPresent,
                 dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                   criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                   physicalChannelReconfiguration_r5_TDD128_add_ext.upPCHpositionInfo
                   ))
               {
                 status = FAILURE;
               }
               fpach_freq_present = dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                   criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                   physicalChannelReconfiguration_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent;
               uppch_positionInfo_present = dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                   criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                   physicalChannelReconfiguration_r5_TDD128_add_ext.m.upPCHpositionInfoPresent;

               /* save power control gap and pathloss compensation switch */
               if (TRUE == dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.
                  u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                  physicalChannelReconfiguration_r5_TDD128_add_ext.m.powerControlGapPresent)
               {
                 tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap =
                   dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.u.
                   criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                   physicalChannelReconfiguration_r5_TDD128_add_ext.powerControlGap;
               }
               else
               {
                 /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
                 tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;
               }

               if (TRUE == dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.
                  u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                  physicalChannelReconfiguration_r5_TDD128_add_ext.m.pathlossCompensationSwitchPresent)
               {
                 tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
                   dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.u.
                   criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                   physicalChannelReconfiguration_r5_TDD128_add_ext.pathlossCompensationSwitch;
               }
               else
               {
                 /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */ 
                 tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = FALSE;
               }

               /* Because UE does not support MultiCarrier, if received  messages contain 
                                  the IE "DL-MultiCarrier-Information", set the variable INVALID_CONFIGURATION to TRUE. */
               if (tdsrrc_RRC_StateIndicator_cell_DCH == dcch_ptr->message.u.physicalChannelReconfiguration.
                u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5.rrc_StateIndicator)
               {
                 if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                   dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                   criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                   physicalChannelReconfiguration_r5_TDD128_add_ext, dl_MultiCarrier_Information))
                 {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                   multicarrier_flg = TRUE;
                   multiCarrier_info_ptr = &dcch_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                       criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5_add_ext.
                       physicalChannelReconfiguration_r5_TDD128_add_ext.
                       dl_MultiCarrier_Information;
                   TDSRRC_MSG0(MSG_LEGACY_HIGH,"Multi-Carrier Information present");
#else
                   TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport configuration received DL Multi-Carrier Information reveived");
                   status = FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                 }
               }
             }
           }
           if ((status == SUCCESS)&&(tdsrrcllc_phychan_reconfig_ie_r5(&dcch_ptr->message.u.
                                         physicalChannelReconfiguration.u.later_than_r3.criticalExtensions.
                                         u.criticalExtensions.u.r5
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                         , multicarrier_flg, multiCarrier_info_ptr
#endif
                                         )
             == FAILURE))
           {
             status = FAILURE;
           }
           else
           {
              tdsrrcllc_process_non_critcal_phys_reconfig_ie_r5(dcch_ptr);
           }
           /* add ext value save process */

           /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_phychan_reconfig_ie_r7\8\9 */
           if (status == SUCCESS)
           {
             tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
           }
         }


return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_rbrc_msg

DESCRIPTION:
  This function processes the Radio Bearer Reconfig msg based on the REL version and
  updates tdsordered_config with the latest IEs recv'd by TRadio Bearer Reconfig msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_rbrc_msg
(
  uint32 dl_sdu_num,
  void *msg_ptr

)
{
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr = NULL;
#endif
  /* local variables */
  uecomdef_status_e_type  status = SUCCESS;
  boolean                         fpach_freq_present = FALSE; 
  boolean                         uppch_positionInfo_present = FALSE; 
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if ((TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.radioBearerReconfiguration,
       rrc_RadioBearerReconfiguration_later_than_r3 )) && 
       (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions,
        rrc_RadioBearerReconfiguration_criticalExtensions_6_r4 )))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 4 Msg");
    if(tdsrrcllc_rb_reconfig_ie_r4(&dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.
          criticalExtensions.u.r4) == FAILURE)
    {
      status = FAILURE;
    }
    else /*Here we update the tdscell_id_per_rl that is coming in no-critical extension of the R99 OTA.*/
    {
        tdsrrcllc_process_non_critcal_rb_reconfig_ie_r4(dcch_ptr);
    }

    /* add ext value save process */
    if (status == SUCCESS)
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.r4,
          v4d0NonCriticalExtensions))
      {
        //save beaconPLEst
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
            v590NonCriticalExtensions))
        {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                  dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.
                  criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions,
                  v5d0NonCriticalExtenstions))
            {
              if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                  dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.
                  criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5d0NonCriticalExtenstions,
                  v690NonCriticalExtensions))
              {
                tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
                if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                      dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.r4.
                      v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5d0NonCriticalExtenstions.
                      v690NonCriticalExtensions.radioBearerReconfiguration_v690ext,
                      beaconPLEst))
                {
                  tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
                }
              }
            }
        }
        //r4_TDD128_add_ext
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.r4.
            v4d0NonCriticalExtensions,
          radioBearerReconfiguration_r4_add_ext))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.radioBearerReconfiguration_r4_add_ext,
              radioBearerReconfiguration_r4_TDD128_add_ext))
          {
            if (FAILURE == tdsrrcget_add_ext_info(
              dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.radioBearerReconfiguration_r4_add_ext.
                radioBearerReconfiguration_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent,
              dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.radioBearerReconfiguration_r4_add_ext.
                radioBearerReconfiguration_r4_TDD128_add_ext.fPachFrequencyInfo,
              dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.radioBearerReconfiguration_r4_add_ext.
                radioBearerReconfiguration_r4_TDD128_add_ext.m.upPCHpositionInfoPresent,
              dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.radioBearerReconfiguration_r4_add_ext.
                radioBearerReconfiguration_r4_TDD128_add_ext.upPCHpositionInfo
              ))
            {
              status = FAILURE;
            }
            fpach_freq_present = dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.radioBearerReconfiguration_r4_add_ext.
                radioBearerReconfiguration_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent;
            uppch_positionInfo_present = dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.radioBearerReconfiguration_r4_add_ext.
                radioBearerReconfiguration_r4_TDD128_add_ext.m.upPCHpositionInfoPresent;
          }
        }
      }
    }
    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_rb_reconfig_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }
  else if (TDSRRCRB_R6_CRITICAL_EXT_PRESENT(
          dcch_ptr->message.u.radioBearerReconfiguration, 
          rrc_RadioBearerReconfiguration_))
  {
    if (FAILURE == tdsrrcllc_rb_reconfig_ie_r6(&dcch_ptr->message.u.radioBearerReconfiguration.
                                            u.later_than_r3.criticalExtensions.
                                            u.criticalExtensions.
                                            u.criticalExtensions.
                                            u.r6.radioBearerReconfiguration_r6

    ))
    {
      status = FAILURE;          
    }
  }
  else if (TDSRRCRB_R7_CRITICAL_EXT_PRESENT(
          dcch_ptr->message.u.radioBearerReconfiguration, 
          rrc_RadioBearerReconfiguration_))
  {
    if (FAILURE == tdsrrcllc_rb_reconfig_ie_r7(&dcch_ptr->message.u.radioBearerReconfiguration.
                                            u.later_than_r3.criticalExtensions.
                                            u.criticalExtensions.
                                            u.criticalExtensions.
                                            u.criticalExtensions.
                                            u.r7))
    {
      status = FAILURE;          
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
  else if (TDSRRCRB_R8_CRITICAL_EXT_PRESENT(
          dcch_ptr->message.u.radioBearerReconfiguration, 
          rrc_RadioBearerReconfiguration_))
  {
    if (FAILURE == tdsrrcllc_rb_reconfig_ie_r8(&dcch_ptr->message.u.radioBearerReconfiguration.
                                            u.later_than_r3.criticalExtensions.
                                            u.criticalExtensions.
                                            u.criticalExtensions.
                                            u.criticalExtensions.
                                            u.criticalExtensions.                                                  
                                            u.r8))
    {
      status = FAILURE;          
    }
  }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  else if (TDSRRCRB_R9_CRITICAL_EXT_PRESENT(
      dcch_ptr->message.u.radioBearerReconfiguration, 
      rrc_RadioBearerReconfiguration_))
  {
  if (FAILURE == tdsrrcllc_rb_reconfig_ie_r9(&dcch_ptr->message.u.radioBearerReconfiguration.
                      u.later_than_r3.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.
                      u.criticalExtensions.                          
                      u.criticalExtensions.
                      u.r9))
  {
    status = FAILURE;      
  }
  }
#endif /*FEATURE_TDSCDMA_REL9*/
  else
  {
    //save beaconPLEst
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5,
        v5d0NonCriticalExtenstions))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions,
        v690NonCriticalExtensions))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions.
        v690NonCriticalExtensions.radioBearerReconfiguration_v690ext,
        beaconPLEst))
        {
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
        }
      }
    }
    //r5_add_ext
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
      dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
        u.criticalExtensions.u.r5,
      radioBearerReconfiguration_r5_add_ext))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext,
          radioBearerReconfiguration_r5_TDD128_add_ext))
      {
        if (FAILURE == tdsrrcget_add_ext_info(
          dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.
            radioBearerReconfiguration_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent,
          dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.
            radioBearerReconfiguration_r5_TDD128_add_ext.fPachFrequencyInfo,
          dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.
            radioBearerReconfiguration_r5_TDD128_add_ext.m.upPCHpositionInfoPresent,
          dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.
            radioBearerReconfiguration_r5_TDD128_add_ext.upPCHpositionInfo
          ))
        {
          status = FAILURE;
        }
        fpach_freq_present = dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.
            radioBearerReconfiguration_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent;
        uppch_positionInfo_present = dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.
            radioBearerReconfiguration_r5_TDD128_add_ext.m.upPCHpositionInfoPresent;

       /* save power control gap and pathloss compensation switch */
       if (TRUE == dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.
          criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.radioBearerReconfiguration_r5_TDD128_add_ext.
          m.powerControlGapPresent)
       {
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap =
          dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
          r5.radioBearerReconfiguration_r5_add_ext.radioBearerReconfiguration_r5_TDD128_add_ext.powerControlGap;
       }
       else
       {
         /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;
       }

       if (TRUE == dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.
          criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.radioBearerReconfiguration_r5_TDD128_add_ext.
          m.pathlossCompensationSwitchPresent)
       {
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
          dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.
          r5.radioBearerReconfiguration_r5_add_ext.radioBearerReconfiguration_r5_TDD128_add_ext.pathlossCompensationSwitch;
       }
       else
       {
         /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */ 
         tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = FALSE;
       }

       /* Because UE does not support MultiCarrier, if received  messages contain 
                          the IE "DL-MultiCarrier-Information", set the variable INVALID_CONFIGURATION to TRUE. */
       if (tdsrrc_RRC_StateIndicator_cell_DCH == dcch_ptr->message.u.radioBearerReconfiguration.u.
        later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.radioBearerReconfiguration_r5.rrc_StateIndicator)
       {
         if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.radioBearerReconfiguration_r5_TDD128_add_ext, dl_MultiCarrier_Information))
         {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
           multicarrier_flg = TRUE;
           multiCarrier_info_ptr = &dcch_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.criticalExtensions.
               u.criticalExtensions.u.r5.radioBearerReconfiguration_r5_add_ext.radioBearerReconfiguration_r5_TDD128_add_ext.
               dl_MultiCarrier_Information;
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Multi-Carrier Information present");
#else
           TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport configuration received DL Multi-Carrier Information reveived");
           status = FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
          }
        }
      }
    }
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 5 Msg");
    if ((status == SUCCESS)&&(tdsrrcllc_rb_reconfig_ie_r5(&dcch_ptr->message.u.radioBearerReconfiguration.u.
             later_than_r3.criticalExtensions.u.criticalExtensions.u.r5
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
             , multicarrier_flg, multiCarrier_info_ptr
#endif
             )
          == FAILURE))
    {
      status = FAILURE;
    }
    else
    {
      tdsrrcllc_process_non_critcal_rb_reconfig_ie_r5(dcch_ptr);
    }

    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_rb_reconfig_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }

return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_rbr_msg

DESCRIPTION:
  This function processes the Radio Bearer Release msg based on the REL version and
  updates tdsordered_config with the latest IEs recv'd by TRadio Bearer Release msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_rbr_msg
(
  uint32 dl_sdu_num,
  void *msg_ptr

)
{
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr = NULL;
#endif
  /* local variables */
  uecomdef_status_e_type  status = SUCCESS;
  boolean                         fpach_freq_present = FALSE; 
  boolean                         uppch_positionInfo_present = FALSE; 
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if ((TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.radioBearerRelease,
          rrc_RadioBearerRelease_later_than_r3)) &&
         (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions,
           rrc_RadioBearerRelease_criticalExtensions_6_r4)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 4 Msg");
    if(tdsrrcllc_rb_release_ie_r4(&dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.
                      criticalExtensions.u.r4) == FAILURE)
    {
      status = FAILURE;
    }

    /* add ext value save process */
    if (status == SUCCESS)
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4,
          v4d0NonCriticalExtensions))
      {
        //save beaconPLEst
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
            v590NonCriticalExtensions))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.
              criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions,
              v690NonCriticalExtensions))
          {
            tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                  dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.
                  v4d0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
                  radioBearerRelease_v690ext, beaconPLEst))
            {
              tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
            }
          }
        }
        //r4_add_ext
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.
            v4d0NonCriticalExtensions,
          radioBearerRelease_r4_add_ext))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerRelease_r4_add_ext,
              radioBearerRelease_r4_TDD128_add_ext))
          {
            if (FAILURE == tdsrrcget_add_ext_info(
              dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerRelease_r4_add_ext.
                radioBearerRelease_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent,
              dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerRelease_r4_add_ext.
                radioBearerRelease_r4_TDD128_add_ext.fPachFrequencyInfo,
              dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerRelease_r4_add_ext.
                radioBearerRelease_r4_TDD128_add_ext.m.upPCHpositionInfoPresent,
              dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerRelease_r4_add_ext.
                radioBearerRelease_r4_TDD128_add_ext.upPCHpositionInfo
              ))
            {
              status = FAILURE;
            }
            fpach_freq_present = dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerRelease_r4_add_ext.
                radioBearerRelease_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent;
            uppch_positionInfo_present = dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerRelease_r4_add_ext.
                radioBearerRelease_r4_TDD128_add_ext.m.upPCHpositionInfoPresent;
          }
        }
      }
    }
    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_rb_release_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }
        else if (TDSRRCRB_R6_CRITICAL_EXT_PRESENT(
                dcch_ptr->message.u.radioBearerRelease, 
                rrc_RadioBearerRelease_))
        {
          if (FAILURE == tdsrrcllc_rb_release_ie_r6(&dcch_ptr->message.u.radioBearerRelease.
              u.later_than_r3.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.r6.radioBearerRelease_r6
              ))
          {
            status = FAILURE;
          }         
        }

        else if (TDSRRCRB_R7_CRITICAL_EXT_PRESENT(
                dcch_ptr->message.u.radioBearerRelease, 
                rrc_RadioBearerRelease_))
        {
          if (FAILURE == tdsrrcllc_rb_release_ie_r7(&dcch_ptr->message.u.radioBearerRelease.
              u.later_than_r3.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.r7))
          {
            status = FAILURE;
          }            
        }
#ifdef FEATURE_TDSCDMA_REL8
        else if (TDSRRCRB_R8_CRITICAL_EXT_PRESENT(
                dcch_ptr->message.u.radioBearerRelease, 
                rrc_RadioBearerRelease_))
        {
          if (FAILURE == tdsrrcllc_rb_release_ie_r8(&dcch_ptr->message.u.radioBearerRelease.
              u.later_than_r3.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.              
              u.r8))
          {
            status = FAILURE;
          }            
        }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
        else if (TDSRRCRB_R9_CRITICAL_EXT_PRESENT(
                dcch_ptr->message.u.radioBearerRelease, 
                rrc_RadioBearerRelease_))
        {
          if (FAILURE == tdsrrcllc_rb_release_ie_r9(&dcch_ptr->message.u.radioBearerRelease.
              u.later_than_r3.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.              
              u.criticalExtensions.
              u.r9))
          {
            status = FAILURE;
          }            
        }
#endif /*FEATURE_TDSCDMA_REL9*/
        else
        {

          //save beaconPLEst
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5,
            v690NonCriticalExtensions))
          {
            tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
            v690NonCriticalExtensions.radioBearerRelease_v690ext,
            beaconPLEst))
            {
              tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
            }
          }
          //r5_TDD128_add_ext
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.r5,
              radioBearerRelease_r5_add_ext))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.r5.radioBearerRelease_r5_add_ext,
              radioBearerRelease_r5_TDD128_add_ext))
            {
                if (FAILURE == tdsrrcget_add_ext_info(
                  dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                    radioBearerRelease_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent,
                  dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                    radioBearerRelease_r5_TDD128_add_ext.fPachFrequencyInfo,
                  dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                    radioBearerRelease_r5_TDD128_add_ext.m.upPCHpositionInfoPresent,
                  dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                    radioBearerRelease_r5_TDD128_add_ext.upPCHpositionInfo
                  ))
                {
                  status = FAILURE;
                }
                fpach_freq_present = dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                    radioBearerRelease_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent;
                uppch_positionInfo_present = dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                    radioBearerRelease_r5_TDD128_add_ext.m.upPCHpositionInfoPresent;

                /* save power control gap and pathloss compensation switch */
                if (TRUE == dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                    radioBearerRelease_r5_TDD128_add_ext.m.powerControlGapPresent)
                {
                  tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap =
                   dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                   criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                   radioBearerRelease_r5_TDD128_add_ext.powerControlGap;
                }
                else
                {
                  /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
                  tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;
                }
                
                if (TRUE == dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.
                    u.criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                    radioBearerRelease_r5_TDD128_add_ext.m.pathlossCompensationSwitchPresent)
                {
                  tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
                   dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                   criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.
                   radioBearerRelease_r5_TDD128_add_ext.pathlossCompensationSwitch;
                }
                else
                {
                  /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */ 
                  tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = FALSE;
                }

                /* Because UE does not support MultiCarrier, if received  messages contain 
                                   the IE "DL-MultiCarrier-Information", set the variable INVALID_CONFIGURATION to TRUE. */
                if (tdsrrc_RRC_StateIndicator_cell_DCH == dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                  criticalExtensions.u.r5.radioBearerRelease_r5.rrc_StateIndicator)
                {
                  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                    dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.radioBearerRelease_r5_TDD128_add_ext, dl_MultiCarrier_Information))
                  {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                    multicarrier_flg = TRUE;
                    multiCarrier_info_ptr = &dcch_ptr->message.u.radioBearerRelease.u.later_than_r3.criticalExtensions.u.
                      criticalExtensions.u.r5.radioBearerRelease_r5_add_ext.radioBearerRelease_r5_TDD128_add_ext.
                       dl_MultiCarrier_Information;
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Multi-Carrier Information present");
#else
                    TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport configuration received DL Multi-Carrier Information reveived");
                    status = FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                  }
                }
            }
          }
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 5 Msg");
          if ((status == SUCCESS)&&(tdsrrcllc_rb_release_ie_r5(&dcch_ptr->message.u.radioBearerRelease.u.
            later_than_r3.criticalExtensions.u.criticalExtensions.u.r5
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                       , multicarrier_flg, multiCarrier_info_ptr
#endif
            )
            == FAILURE))
          {
            status = FAILURE;
          }

          /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_rb_release_ie_r7\8\9 */
          if (status == SUCCESS)
          {
            tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
          }
        }

return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_rbe_msg

DESCRIPTION:
  This function processes the Radio Bearer Setup msg based on the REL version and
  updates tdsordered_config with the latest IEs recv'd by TRadio Bearer Setup msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_rbe_msg
(
  uint32 dl_sdu_num,
  void *msg_ptr

)
{
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr = NULL;
#endif
  /* local variables */
  uecomdef_status_e_type  status = SUCCESS;
  boolean                         fpach_freq_present = FALSE; 
  boolean                         uppch_positionInfo_present = FALSE; 
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if((TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.radioBearerSetup,
            rrc_RadioBearerSetup_later_than_r3)) &&
  (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions,
            rrc_RadioBearerSetup_criticalExtensions_6_r4)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 4 Msg");
    if(tdsrrcllc_rb_setup_ie_r4(&dcch_ptr->message.u.radioBearerSetup.u.
        later_than_r3.criticalExtensions.u.r4)
        == FAILURE)
    {
      status = FAILURE;
    }
    /* add ext value save process */
    if (status == SUCCESS)
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4,
          v4d0NonCriticalExtensions))
      {
        //save beaconPLEst
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
            v590NonCriticalExtensions))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions,
                v5d0NonCriticalExtenstions))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5d0NonCriticalExtenstions,
                v690NonCriticalExtensions))
            {
              tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
              if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                    dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.
                    v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
                    radioBearerSetup_v690ext, beaconPLEst))
              {
                tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
              }
            }
          }
        }
        //r4_add_ext
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.
            v4d0NonCriticalExtensions,
          radioBearerSetup_r4_add_ext))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerSetup_r4_add_ext,
              radioBearerSetup_r4_TDD128_add_ext))
          {
            if (FAILURE == tdsrrcget_add_ext_info(
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerSetup_r4_add_ext.
                radioBearerSetup_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent,
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerSetup_r4_add_ext.
                radioBearerSetup_r4_TDD128_add_ext.fPachFrequencyInfo,
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerSetup_r4_add_ext.
                radioBearerSetup_r4_TDD128_add_ext.m.upPCHpositionInfoPresent,
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerSetup_r4_add_ext.
                radioBearerSetup_r4_TDD128_add_ext.upPCHpositionInfo
              ))
            {
              status = FAILURE;
            }
            fpach_freq_present = dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerSetup_r4_add_ext.
                radioBearerSetup_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent;
            uppch_positionInfo_present = dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.radioBearerSetup_r4_add_ext.
                radioBearerSetup_r4_TDD128_add_ext.m.upPCHpositionInfoPresent;
          }
        }
      }
    }
    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_rb_setup_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }
        else if (TDSRRCRB_R6_CRITICAL_EXT_PRESENT(
                dcch_ptr->message.u.radioBearerSetup, 
                rrc_RadioBearerSetup_))
        {
          
          if (FAILURE == tdsrrcllc_rb_setup_ie_r6(&dcch_ptr->message.u.radioBearerSetup.
              u.later_than_r3.criticalExtensions.
              u.criticalExtensions.
              u.criticalExtensions.u.r6.radioBearerSetup_r6
              ))
          {
            status = FAILURE;
          }
        }
        else if (TDSRRCRB_R7_CRITICAL_EXT_PRESENT(
                dcch_ptr->message.u.radioBearerSetup, 
                rrc_RadioBearerSetup_))
        {
          if (FAILURE == tdsrrcllc_rb_setup_ie_r7(&dcch_ptr->message.u.radioBearerSetup.
             u.later_than_r3.criticalExtensions.u.
             criticalExtensions.u.criticalExtensions.
             u.criticalExtensions.u.r7))
          {
            status = FAILURE;
          }
        }
#ifdef FEATURE_TDSCDMA_REL8
        else if (TDSRRCRB_R8_CRITICAL_EXT_PRESENT(
                dcch_ptr->message.u.radioBearerSetup, 
                rrc_RadioBearerSetup_))
        {
          if (FAILURE == tdsrrcllc_rb_setup_ie_r8(&dcch_ptr->message.u.radioBearerSetup.
             u.later_than_r3.criticalExtensions.u.
             criticalExtensions.u.criticalExtensions.
             u.criticalExtensions.u.criticalExtensions.u.r8))
          {
            status = FAILURE;
          }
        }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
        else if (TDSRRCRB_R9_CRITICAL_EXT_PRESENT(
                dcch_ptr->message.u.radioBearerSetup, 
                rrc_RadioBearerSetup_))
        {
          if (FAILURE == tdsrrcllc_rb_setup_ie_r9(&dcch_ptr->message.u.radioBearerSetup.
             u.later_than_r3.criticalExtensions.u.
             criticalExtensions.u.criticalExtensions.
             u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9))
          {
            status = FAILURE;
          }
        }
#endif /*FEATURE_TDSCDMA_REL9*/

        else
        {

          //save beaconPLEst
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5,
              v5d0NonCriticalExtenstions))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions,
              v690NonCriticalExtensions))
            {
              tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
              if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions.
              v690NonCriticalExtensions.radioBearerSetup_v690ext,
              beaconPLEst))
              {
                tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
              }
            }
          }
          //save r5_TDD128_add_ext
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.r5,
              radioBearerSetup_r5_add_ext))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.r5.radioBearerSetup_r5_add_ext,
              radioBearerSetup_r5_TDD128_add_ext))
            {
              if (FAILURE == tdsrrcget_add_ext_info(
                  dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                    radioBearerSetup_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent,
                  dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                    radioBearerSetup_r5_TDD128_add_ext.fPachFrequencyInfo,
                  dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                    radioBearerSetup_r5_TDD128_add_ext.m.upPCHpositionInfoPresent,
                  dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                    radioBearerSetup_r5_TDD128_add_ext.upPCHpositionInfo
                  ))
                {
                  status = FAILURE;
                }
              fpach_freq_present = dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                    radioBearerSetup_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent;
              uppch_positionInfo_present = dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                    criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                    radioBearerSetup_r5_TDD128_add_ext.m.upPCHpositionInfoPresent;

              /* save power control gap and pathloss compensation switch */
              if (TRUE == dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                  criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                  radioBearerSetup_r5_TDD128_add_ext.m.powerControlGapPresent)
              {
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap =
                 dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                 criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                 radioBearerSetup_r5_TDD128_add_ext.powerControlGap;
              }
              else
              {
                /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;
              }
              
              if (TRUE == dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.
                u.criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                radioBearerSetup_r5_TDD128_add_ext.m.pathlossCompensationSwitchPresent)
              {
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
                 dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                 criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                 radioBearerSetup_r5_TDD128_add_ext.pathlossCompensationSwitch;
              }
              else
              {
                /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */ 
                tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = FALSE;
              }

              /* Because UE does not support MultiCarrier, if received  messages contain 
                                 the IE "DL-MultiCarrier-Information", set the variable INVALID_CONFIGURATION to TRUE. */
              if (tdsrrc_RRC_StateIndicator_cell_DCH == dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                criticalExtensions.u.r5.radioBearerSetup_r5.rrc_StateIndicator)
              {
                if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                  dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                  criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                  radioBearerSetup_r5_TDD128_add_ext, dl_MultiCarrier_Information))
                {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                  multicarrier_flg = TRUE;
                  multiCarrier_info_ptr = &dcch_ptr->message.u.radioBearerSetup.u.later_than_r3.criticalExtensions.u.
                       criticalExtensions.u.r5.radioBearerSetup_r5_add_ext.
                       radioBearerSetup_r5_TDD128_add_ext.
                       dl_MultiCarrier_Information;
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Multi-Carrier Information present");
#else
                  TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport configuration received DL Multi-Carrier Information reveived");
                  status = FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                }
              }
            }
          }
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 5 Msg");
          if ((status == SUCCESS)&&(tdsrrcllc_rb_setup_ie_r5(&dcch_ptr->message.u.radioBearerSetup.u.
            later_than_r3.criticalExtensions.u.criticalExtensions.u.r5
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                       , multicarrier_flg, multiCarrier_info_ptr
#endif
            ))
            == FAILURE)
          {
            status = FAILURE;
          }

          /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_rb_setup_ie_r7\8\9 */
          if (status == SUCCESS)
          {
            tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
          }
        }

return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_tcrc_msg

DESCRIPTION:
  This function processes the Transport Channel Reconfig msg based on the REL version and
  updates tdsordered_config with the latest IEs recv'd by Transport Channel Reconfig msg

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_tcrc_msg
(
  uint32 dl_sdu_num,
  void *msg_ptr

)
{
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr = NULL;
#endif
  /* local variables */
  uecomdef_status_e_type  status = SUCCESS;
  boolean                         fpach_freq_present = FALSE; 
  boolean                         uppch_positionInfo_present = FALSE; 
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;
          
  if ((TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.transportChannelReconfiguration,
      rrc_TransportChannelReconfiguration_later_than_r3)) &&
      (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions,
        rrc_TransportChannelReconfiguration_criticalExtensions_6_r4)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 4 Msg");
    if(tdsrrcllc_trch_reconfig_ie_r4(&dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
        criticalExtensions.u.r4) == FAILURE)
    {
      status = FAILURE;
    }
    else /*Here we update the tdscell_id_per_rl that is coming in no-critical extension of the R99 OTA.*/
    {
      tdsrrcllc_process_non_critcal_trch_reconfig_ie_r4(dcch_ptr);
    }  

    if (status == SUCCESS)
    {
      /* add ext value save process */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
          criticalExtensions.u.r4, v4d0NonCriticalExtensions))
      {
        //save beaconPLEst
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
            v590NonCriticalExtensions))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
              criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtensions,
              v690NonCriticalExtensions))
          {
            tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                  dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.u.r4.
                  v4d0NonCriticalExtensions.v590NonCriticalExtensions.v690NonCriticalExtensions.
                  transportChannelReconfiguration_v690ext, beaconPLEst))
            {
              tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
            }
          }
        }
        //save r4_add_ext
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.r4.v4d0NonCriticalExtensions,
            transportChannelReconfiguration_r4_add_ext))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.transportChannelReconfiguration_r4_add_ext,
              transportChannelReconfiguration_r4_TDD128_add_ext))
          {
            if (FAILURE == tdsrrcget_add_ext_info(
              dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.transportChannelReconfiguration_r4_add_ext.
                transportChannelReconfiguration_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent,
              dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.transportChannelReconfiguration_r4_add_ext.
                transportChannelReconfiguration_r4_TDD128_add_ext.fPachFrequencyInfo,
              dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.transportChannelReconfiguration_r4_add_ext.
                transportChannelReconfiguration_r4_TDD128_add_ext.m.upPCHpositionInfoPresent,
              dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.transportChannelReconfiguration_r4_add_ext.
                transportChannelReconfiguration_r4_TDD128_add_ext.upPCHpositionInfo
              ))
            {
              status = FAILURE;
            }
            fpach_freq_present = dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.transportChannelReconfiguration_r4_add_ext.
                transportChannelReconfiguration_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent;
            uppch_positionInfo_present = dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.
                u.r4.v4d0NonCriticalExtensions.transportChannelReconfiguration_r4_add_ext.
                transportChannelReconfiguration_r4_TDD128_add_ext.m.upPCHpositionInfoPresent;
          }
        }
      }    
    }
    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_trch_reconfig_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }
          else if (TDSRRCRB_R6_CRITICAL_EXT_PRESENT(
                  dcch_ptr->message.u.transportChannelReconfiguration, 
                  rrc_TransportChannelReconfiguration_))
          {
            
            if (FAILURE == tdsrrcllc_trch_reconfig_ie_r6(&dcch_ptr->message.u.transportChannelReconfiguration.
                                                    u.later_than_r3.criticalExtensions.
                                                    u.criticalExtensions.
                                                    u.criticalExtensions.
                                                    u.r6.transportChannelReconfiguration_r6
  
            ))
            {
              status = FAILURE;
            }
          }
          else if (TDSRRCRB_R7_CRITICAL_EXT_PRESENT(
                  dcch_ptr->message.u.radioBearerSetup, 
                  rrc_TransportChannelReconfiguration_))
          {
            if (FAILURE == tdsrrcllc_trch_reconfig_ie_r7(&dcch_ptr->message.u.transportChannelReconfiguration.
               u.later_than_r3.criticalExtensions.u.
               criticalExtensions.u.criticalExtensions.
               u.criticalExtensions.u.r7))
            {
              status = FAILURE;
            }
          }
#ifdef FEATURE_TDSCDMA_REL8
          else if (TDSRRCRB_R8_CRITICAL_EXT_PRESENT(
                  dcch_ptr->message.u.radioBearerSetup, 
                  rrc_TransportChannelReconfiguration_))
          {
            if (FAILURE == tdsrrcllc_trch_reconfig_ie_r8(&dcch_ptr->message.u.transportChannelReconfiguration.
               u.later_than_r3.criticalExtensions.u.
               criticalExtensions.u.criticalExtensions.
               u.criticalExtensions.u.criticalExtensions.u.r8))
            {
              status = FAILURE;
            }
          }
#endif/*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
      else if (TDSRRCRB_R9_CRITICAL_EXT_PRESENT(
          dcch_ptr->message.u.radioBearerSetup, 
          rrc_TransportChannelReconfiguration_))
      {
      if (FAILURE == tdsrrcllc_trch_reconfig_ie_r9(&dcch_ptr->message.u.transportChannelReconfiguration.
         u.later_than_r3.criticalExtensions.u.
         criticalExtensions.u.criticalExtensions.
         u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.r9))
      {
        status = FAILURE;
      }
      }
#endif/*FEATURE_TDSCDMA_REL9*/
  else
  {

    //save beaconPLEst
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
      dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5,
      v690NonCriticalExtensions))
    {
      tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
      dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
      v690NonCriticalExtensions.transportChannelReconfiguration_v690ext,
      beaconPLEst))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
      }
    }
    //save r5_add_ext
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
      dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
      criticalExtensions.u.criticalExtensions.u.r5,
      transportChannelReconfiguration_r5_add_ext))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
          criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext,
          transportChannelReconfiguration_r5_TDD128_add_ext))
      {
        if (FAILURE == tdsrrcget_add_ext_info(
          dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
            transportChannelReconfiguration_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent,
          dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
            transportChannelReconfiguration_r5_TDD128_add_ext.fPachFrequencyInfo,
          dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
            transportChannelReconfiguration_r5_TDD128_add_ext.m.upPCHpositionInfoPresent,
          dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
            transportChannelReconfiguration_r5_TDD128_add_ext.upPCHpositionInfo
            ))
        {
          status = FAILURE;
        }
        fpach_freq_present = dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
            transportChannelReconfiguration_r5_TDD128_add_ext.m.fPachFrequencyInfoPresent;
        uppch_positionInfo_present = dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
            transportChannelReconfiguration_r5_TDD128_add_ext.m.upPCHpositionInfoPresent;

        /* save power control gap and pathloss compensation switch */
        if (TRUE == dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
            transportChannelReconfiguration_r5_TDD128_add_ext.m.powerControlGapPresent)
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap =
           dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.
           u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
           transportChannelReconfiguration_r5_TDD128_add_ext.powerControlGap;
        }
        else
        {
          /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;
        }
        
        if (TRUE == dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
            transportChannelReconfiguration_r5_TDD128_add_ext.m.pathlossCompensationSwitchPresent)
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
           dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.criticalExtensions.
           u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
           transportChannelReconfiguration_r5_TDD128_add_ext.pathlossCompensationSwitch;
        }
        else
        {
          /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */ 
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = FALSE;
        }


        /* Because UE does not support MultiCarrier, if received  messages contain 
                           the IE "DL-MultiCarrier-Information", set the variable INVALID_CONFIGURATION to TRUE. */
        if (tdsrrc_RRC_StateIndicator_cell_DCH == dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5.rrc_StateIndicator)
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
              criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
              transportChannelReconfiguration_r5_TDD128_add_ext, dl_MultiCarrier_Information))
          {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
            multicarrier_flg = TRUE;
            multiCarrier_info_ptr = &dcch_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
              criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5_add_ext.
              transportChannelReconfiguration_r5_TDD128_add_ext.
               dl_MultiCarrier_Information;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Multi-Carrier Information present");
#else
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport configuration received DL Multi-Carrier Information reveived");
            status = FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
          }
        }
      }
    }
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 5 Msg");
    if ((status == SUCCESS)&&(tdsrrcllc_trch_reconfig_ie_r5(&dcch_ptr->message.u.transportChannelReconfiguration.u.
                               later_than_r3.criticalExtensions.u.criticalExtensions.u
                               .r5
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                 , multicarrier_flg, multiCarrier_info_ptr
#endif
                               ) == FAILURE))
    {
      status = FAILURE;
    }
    else
    {
      tdsrrcllc_process_non_critcal_trch_reconfig_ie_r5(dcch_ptr);
    }

    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_trch_reconfig_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }

  return status;
}

/*====================================================================
FUNCTION: tdsrrcllc_process_dcch_cu_msg

DESCRIPTION:
  This function processes the CU m,sg received on DCCH based on the REL version and
  updates tdsordered_config with the latest IEs recv'd by CU

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/

static uecomdef_status_e_type tdsrrcllc_process_dcch_cu_msg
(
  uint32 dl_sdu_num,
  void *msg_ptr

)
{
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr = NULL;
#endif
  /* local variables */
  uecomdef_status_e_type  status = SUCCESS;
  boolean                         fpach_freq_present = FALSE; 
  boolean                         uppch_positionInfo_present = FALSE; 
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

  if ((TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.cellUpdateConfirm,
           rrc_CellUpdateConfirm_later_than_r3)) &&
        (TDSRRC_CHECK_COMMON_MSG_TYPE(dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions,
                  rrc_CellUpdateConfirm_criticalExtensions_6_r4)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 4 Msg");
    if ( dcch_ptr->message.u.cellUpdateConfirm.u.
                  later_than_r3.criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent)
    {
      if(tdsrrcllc_cell_update_confirm_ie_r4(&dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                            criticalExtensions.u.r4.cellUpdateConfirm_r4, FALSE/*DCCH */, NULL,
                            &dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                            criticalExtensions.u.r4.v4d0NonCriticalExtensions)
         == FAILURE)
      {
        status = FAILURE;
      
      }
    }
    else
    {
      if(tdsrrcllc_cell_update_confirm_ie_r4(&dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                            criticalExtensions.u.r4.cellUpdateConfirm_r4, FALSE/*DCCH */, NULL,
                            NULL)
         == FAILURE)
      {
        status = FAILURE;
      
      }
    }

    if (status == SUCCESS)
    {
    /* add ext value save process */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4,
        v4d0NonCriticalExtensions))
      {
        //save beaconPLEst
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions,
            v590NonCriticalExtenstions))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtenstions,
                v5d0NonCriticalExtenstions))
          {
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
                criticalExtensions.u.r4.v4d0NonCriticalExtensions.v590NonCriticalExtenstions.v5d0NonCriticalExtenstions,
                v690NonCriticalExtensions))
            {
              tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
              if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
                    dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
                    v4d0NonCriticalExtensions.v590NonCriticalExtenstions.v5d0NonCriticalExtenstions.v690NonCriticalExtensions.
                    cellUpdateConfirm_v690ext, beaconPLEst))
              {
                tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
              }
            }
          }
        }
        //save r4_add_ext
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
            v4d0NonCriticalExtensions,
          cellUpdateConfirm_r4_add_ext))
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
              dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
              v4d0NonCriticalExtensions.cellUpdateConfirm_r4_add_ext,
              cellUpdateConfirm_r4_TDD128_add_ext))
          {
            if (FAILURE == tdsrrcget_add_ext_info(
              dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.cellUpdateConfirm_r4_add_ext.
                cellUpdateConfirm_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent,
              dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.cellUpdateConfirm_r4_add_ext.
                cellUpdateConfirm_r4_TDD128_add_ext.fPachFrequencyInfo,
              dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.cellUpdateConfirm_r4_add_ext.
                cellUpdateConfirm_r4_TDD128_add_ext.m.upPCHpositionInfoPresent,
              dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.cellUpdateConfirm_r4_add_ext.
                cellUpdateConfirm_r4_TDD128_add_ext.upPCHpositionInfo
              ))
            {
              status = FAILURE;
            }
            fpach_freq_present = dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.cellUpdateConfirm_r4_add_ext.
                cellUpdateConfirm_r4_TDD128_add_ext.m.fPachFrequencyInfoPresent;
            uppch_positionInfo_present = dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.r4.
                v4d0NonCriticalExtensions.cellUpdateConfirm_r4_add_ext.
                cellUpdateConfirm_r4_TDD128_add_ext.m.upPCHpositionInfoPresent;
          }
        }
      }
    }
    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_cell_update_confirm_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }
  else if ((T_tdsrrc_CellUpdateConfirm_criticalExtensions_5_criticalExtensions == 
    dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_criticalExtensions_4_criticalExtensions == 
      dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && 
      (T_tdsrrc_CellUpdateConfirm_criticalExtensions_4_r6 ==
       dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))
  {
    if(tdsrrcllc_cell_update_confirm_ie_r6(&dcch_ptr->message.u.cellUpdateConfirm.u.
               later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                  r6.cellUpdateConfirm_r6
        )
           == FAILURE)
    {
      status = FAILURE;          
    }
  }
    else if ((T_tdsrrc_CellUpdateConfirm_criticalExtensions_5_criticalExtensions ==
    dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.t) && 
    (T_tdsrrc_CellUpdateConfirm_criticalExtensions_4_criticalExtensions == 
      dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) && 
      (T_tdsrrc_CellUpdateConfirm_criticalExtensions_3_criticalExtensions == 
        dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.
        u.criticalExtensions.u.criticalExtensions.t)
       && (T_tdsrrc_CellUpdateConfirm_criticalExtensions_3_r7 == 
       dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
       criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) )
  {
    if(tdsrrcllc_cell_update_confirm_ie_r7(dl_sdu_num,msg_ptr)
       == FAILURE)
    {
      status = FAILURE;          
    }
  }
#ifdef FEATURE_TDSCDMA_REL8
  else if (TDSRRCRB_R8_CU_CRITICAL_EXT_PRESENT_PTR(dcch_ptr,rrc_CellUpdateConfirm))
  {
    if(tdsrrcllc_cell_update_confirm_ie_r8(dl_sdu_num,msg_ptr)
       == FAILURE)
    {
      status = FAILURE;          
    }
  }
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
  else if ((T_tdsrrc_CellUpdateConfirm_criticalExtensions_6_criticalExtensions == 
    dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.t) 
    && (T_tdsrrc_CellUpdateConfirm_criticalExtensions_5_criticalExtensions == 
    dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.t) 
    && (T_tdsrrc_CellUpdateConfirm_criticalExtensions_4_criticalExtensions == 
     dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) 
     && (T_tdsrrc_CellUpdateConfirm_criticalExtensions_3_criticalExtensions == 
     dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t) 
     && (T_tdsrrc_CellUpdateConfirm_criticalExtensions_2_criticalExtensions == 
     dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t)
     && (T_tdsrrc_CellUpdateConfirm_criticalExtensions_1_r9 == 
     dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.criticalExtensions.t))
  {
    if(tdsrrcllc_cell_update_confirm_ie_r9(dl_sdu_num,msg_ptr)
     == FAILURE)
    {
      status = FAILURE;
    }
  }
#endif /*FEATURE_TDSCDMA_REL9*/

  else
  {
    //save beaconPLEst
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5,
          v5d0NonCriticalExtenstions))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.v5d0NonCriticalExtenstions,
        v690NonCriticalExtensions))
      {
        tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = FALSE;
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
        dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.criticalExtensions.u.r5.
        v5d0NonCriticalExtenstions.v690NonCriticalExtensions.cellUpdateConfirm_v690ext,
        beaconPLEst))
        {
          tdsordered_config_ptr->l1_ul_chan_parms.dpch_cfg.beacon_pl_est = TRUE;
        }
      }
    }
    //save r5_add_ext
    if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
      dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
        criticalExtensions.u.criticalExtensions.u.r5,
      cellUpdateConfirm_r5_add_ext))
    {
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
          dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_r5_add_ext,
          cellUpdateConfirm_r5_TDD128_add_ext))
      {
        if (FAILURE == tdsrrcget_add_ext_info(
          dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_r5_add_ext.cellUpdateConfirm_r5_TDD128_add_ext.
            m.fPachFrequencyInfoPresent,
          dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_r5_add_ext.cellUpdateConfirm_r5_TDD128_add_ext.
            fPachFrequencyInfo,
          dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_r5_add_ext.cellUpdateConfirm_r5_TDD128_add_ext.
            m.upPCHpositionInfoPresent,
          dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_r5_add_ext.cellUpdateConfirm_r5_TDD128_add_ext.
            upPCHpositionInfo
            ))
        {
          status = FAILURE;
        }
        fpach_freq_present = dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_r5_add_ext.cellUpdateConfirm_r5_TDD128_add_ext.
            m.fPachFrequencyInfoPresent;
        uppch_positionInfo_present = dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.
            cellUpdateConfirm_r5_add_ext.cellUpdateConfirm_r5_TDD128_add_ext.
            m.upPCHpositionInfoPresent;

        /* save power control gap and pathloss compensation switch */
        if (TRUE == dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.
            u.criticalExtensions.u.r5.cellUpdateConfirm_r5_add_ext.
            cellUpdateConfirm_r5_TDD128_add_ext.m.powerControlGapPresent)
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap =
           dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
           criticalExtensions.u.r5.cellUpdateConfirm_r5_add_ext.
           cellUpdateConfirm_r5_TDD128_add_ext.powerControlGap;
        }
        else
        {
          /* (10.3.3.36a) Power Control GAP: Unit: Number of subframes, Default value is 1. */
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.pow_control_gap = 1;
        }
        
        if (TRUE == dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.
          u.criticalExtensions.u.r5.cellUpdateConfirm_r5_add_ext.
          cellUpdateConfirm_r5_TDD128_add_ext.m.pathlossCompensationSwitchPresent)
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens =
           dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.criticalExtensions.u.
           criticalExtensions.u.r5.cellUpdateConfirm_r5_add_ext.
           cellUpdateConfirm_r5_TDD128_add_ext.pathlossCompensationSwitch;
        }
        else
        {
          /* (10.3.6.36a)Pathloss compensation switch: Default value is FALSE. */ 
          tdsordered_config_ptr->l1_hsdpa_info.hs_scchset_info.do_pathloss_compens = FALSE;
        }

        /* Because UE does not support MultiCarrier, if received  messages contain 
                           the IE "DL-MultiCarrier-Information", set the variable INVALID_CONFIGURATION to TRUE. */
        if (tdsrrc_RRC_StateIndicator_cell_DCH == dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
            criticalExtensions.u.criticalExtensions.u.r5.cellUpdateConfirm_r5.rrc_StateIndicator)
        {
          if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(
            dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
              criticalExtensions.u.criticalExtensions.u.r5.
              cellUpdateConfirm_r5_add_ext.cellUpdateConfirm_r5_TDD128_add_ext,
              dl_MultiCarrier_Information))
          {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
            multicarrier_flg = TRUE;
            multiCarrier_info_ptr = &dcch_ptr->message.u.cellUpdateConfirm.u.later_than_r3.
              criticalExtensions.u.criticalExtensions.u.r5.
              cellUpdateConfirm_r5_add_ext.cellUpdateConfirm_r5_TDD128_add_ext.
               dl_MultiCarrier_Information;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Multi-Carrier Information present");
#else
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"unsupport configuration received DL Multi-Carrier Information reveived");
            status = FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
          }
        }
      }
    }
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC:Rel 5 Msg");
    if (status == SUCCESS)
    {
      if (dcch_ptr->message.u.cellUpdateConfirm.u.
                later_than_r3.criticalExtensions.u.criticalExtensions.u
               .r5.m.cellUpdateConfirm_r5_add_extPresent)
      {
        if(tdsrrcllc_cell_update_confirm_ie_r5(&dcch_ptr->message.u.cellUpdateConfirm.u.
                  later_than_r3.criticalExtensions.u.criticalExtensions.u
                 .r5.cellUpdateConfirm_r5, FALSE/*DCCH */, NULL,
                                &dcch_ptr->message.u.cellUpdateConfirm.u.
                  later_than_r3.criticalExtensions.u.criticalExtensions.u
                      .r5.cellUpdateConfirm_r5_add_ext
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                     , multicarrier_flg, multiCarrier_info_ptr
#endif
                      ) == FAILURE)
        {
          status = FAILURE;
        }
      }
      else
      {
        if(tdsrrcllc_cell_update_confirm_ie_r5(&dcch_ptr->message.u.cellUpdateConfirm.u.
                  later_than_r3.criticalExtensions.u.criticalExtensions.u
                 .r5.cellUpdateConfirm_r5, FALSE/*DCCH */, NULL,
                                NULL
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                     , multicarrier_flg, multiCarrier_info_ptr
#endif
                                ) == FAILURE)
        {
          status = FAILURE;
        }
      }
    }
    /* add ext value save process */

    /* For R7\R8\R9, handle IE "FPACH freq" and "upPCH shift" in function tdsrrcllc_cell_update_confirm_ie_r7\8\9 */
    if (status == SUCCESS)
    {
      tdsrrcllc_set_default_value_fpach_and_uppch(fpach_freq_present, uppch_positionInfo_present);
    }
  }

  return status;
}

/*====================================================================
FUNCTION: rrcllc_copy_oc_with_msg_ie

DESCRIPTION:
  This function figures out which RRC message was received and
  updates tdsordered_config with the latest IEs recv'd by one of
  the RRC procedures (i.e. an over-the-air message). This is valid
  only for Downlink DCCH and CCCH messages.

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS if everything is valid, FAILURE otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_copy_msg_ie_to_oc
(
  uint32 dl_sdu_num,
  void *msg_ptr
)
{
  /* local variables */
  uecomdef_status_e_type  status = SUCCESS;
  tdsrrc_DL_CCCH_Message *ccch_ptr;
  tdsrrc_DL_DCCH_Message *dcch_ptr;
  uint32 codec_info;
  tdsdch_id_mapping  dch_mapping[TDSMAX_DCH_VOICE_CHANNEL];
  uint32 ordered_codec_info;
  uint32 current_codec_info;
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
    boolean ordered_codec_status_flag = FALSE;
#endif

  if(msg_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL message ptr passed to OC");
    return(FAILURE);
  }

  /* Determine if it was rcv'd on the DL CCCH */
  if(dl_sdu_num == tdsrrc_DL_CCCH_Message_PDU)
  {
    ccch_ptr = (tdsrrc_DL_CCCH_Message *)msg_ptr;

    /* Since it was a DL CCCH Msg, figure out the Message Type */
    switch(ccch_ptr->message.t)
    {
      case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Conn Setup updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_conn_setup_msg(dl_sdu_num,msg_ptr)== FAILURE)
          {
            status = FAILURE;
          }


        break;

      case T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CellUpdateCnf updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_ccch_cell_update_cnf(dl_sdu_num,msg_ptr)== FAILURE)
          {
            status = FAILURE;
          }

        break;

      case T_tdsrrc_DL_CCCH_MessageType_uraUpdateConfirm:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CCCH Message %d not supported yet for TDSORDERED_CONFIG init", ccch_ptr->message.t);
        status = FAILURE;
        break;

      case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject:
      case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease:
      /*March2002Rel changes : replaced with spare values
      case rrc_DL_CCCH_MessageType_extension_chosen:
    */
    case T_tdsrrc_DL_CCCH_MessageType_spare2:
    case T_tdsrrc_DL_CCCH_MessageType_spare1:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid CCCH Message %d updating OC", ccch_ptr->message.t);
        status = FAILURE;
        break;

      default:
        ERR_FATAL("Invalid CCCH Message %d rcv'd", ccch_ptr->message.t,0,0);
        break;
    } /* end CCCH msg switch */
  } /* end DL CCCH msg */

  /* It was rcv'd on the DL DCCH */
  else if(dl_sdu_num == tdsrrc_DL_DCCH_Message_PDU)
  {
    dcch_ptr = (tdsrrc_DL_DCCH_Message *)msg_ptr;

    /* Since it was a DL DCCH Msg, figure out the Message Type */
    switch(dcch_ptr->message.t)
    {
      case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PhyChan Reconfig updating TDSORDERED_CONFIG");

        if(tdsrrcllc_process_pcrc_msg(dl_sdu_num,msg_ptr)== FAILURE)
          {
            status = FAILURE;
          }
  


        break;

      case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Reconfig updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_rbrc_msg(dl_sdu_num,msg_ptr)== FAILURE)
          {
            status = FAILURE;
          }

        

        break;

      case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Release updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_rbr_msg(dl_sdu_num,msg_ptr)== FAILURE)
          {
            status = FAILURE;
          }
          
        
        break;

      case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Setup updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_rbe_msg(dl_sdu_num,msg_ptr)== FAILURE)
          {
            status = FAILURE;
          }


       break;

      case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TrCH reconfig updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_tcrc_msg(dl_sdu_num,msg_ptr)== FAILURE)
          {
            status = FAILURE;
          }


        break;

      case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CellUpdateCnf updating TDSORDERED_CONFIG");
        if(tdsrrcllc_process_dcch_cu_msg(dl_sdu_num,msg_ptr)== FAILURE)
          {
            status = FAILURE;
          }


        break;

      case T_tdsrrc_DL_DCCH_MessageType_activeSetUpdate:
      case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM:
      case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_CDMA2000:
      case T_tdsrrc_DL_DCCH_MessageType_securityModeCommand:
      case T_tdsrrc_DL_DCCH_MessageType_transportFormatCombinationControl:
      case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
      case T_tdsrrc_DL_DCCH_MessageType_uplinkPhysicalChannelControl:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DCCH Message %d not supported yet for OC", dcch_ptr->message.t);
        status = FAILURE;
        break;

      case T_tdsrrc_DL_DCCH_MessageType_rrcConnectionRelease:
      case T_tdsrrc_DL_DCCH_MessageType_downlinkDirectTransfer:
      case T_tdsrrc_DL_DCCH_MessageType_counterCheck:
      case T_tdsrrc_DL_DCCH_MessageType_measurementControl:
      case T_tdsrrc_DL_DCCH_MessageType_pagingType2:
      case T_tdsrrc_DL_DCCH_MessageType_signallingConnectionRelease:
      case T_tdsrrc_DL_DCCH_MessageType_ueCapabilityEnquiry:
      case T_tdsrrc_DL_DCCH_MessageType_ueCapabilityInformationConfirm:
      case T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation:
      /*March2002Rel changes : replaced with spare values
      case rrc_DL_DCCH_MessageType_extension_chosen:
      */
      case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GERANIu:
      case T_tdsrrc_DL_DCCH_MessageType_mbmsModifiedServicesInformation:
      case T_tdsrrc_DL_DCCH_MessageType_spare3:
      case T_tdsrrc_DL_DCCH_MessageType_spare2:
      case T_tdsrrc_DL_DCCH_MessageType_spare1:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid DCCH Message %d updating OC", dcch_ptr->message.t);
        status = FAILURE;
        break;

      case T_tdsrrc_DL_DCCH_MessageType_physicalSharedChannelAllocation:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DL TDD Message %d will never be supported", dcch_ptr->message.t);
        status = FAILURE;
        break;

      default:
        ERR_FATAL("Invalid DCCH Message %d rcv'd", dcch_ptr->message.t,0,0);
        break;
    } /* end DCCH msg switch */
  } /* end DL DCCH msg */

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined (FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
  /* It is a Handover to UTRAN Command */
  else if(dl_sdu_num == tdsrrc_HandoverToUTRANCommand_PDU)
  {
    if(tdsrrcllc_handover_to_utran_ie((tdsrrc_HandoverToUTRANCommand *)msg_ptr) == FAILURE)
    {
      status = FAILURE;
    }
  }
#endif /* FEATURE_INTERRAT_HANDOVER_G2TD */

  /* No other downlink SDU type is valid for this case */
  else
  {
    ERR_FATAL("Invalid SDU %d received", dl_sdu_num,0,0);
  }

  ordered_codec_info = tdsrrc_get_ordered_codec_mode();
  current_codec_info = tdsrrc_get_current_codec_mode();
  
  if((status == SUCCESS ) && (ordered_codec_info != TDSCDMA_IVOCODER_ID_NONE) && (ordered_codec_info != current_codec_info))
  {
    (void) tdsrrcllc_derive_amr_rate(&codec_info,dch_mapping);
    TDSRRC_MSG3(MSG_LEGACY_HIGH, " derived codec mode = %d current codec mode = %d ordered codec mode = %d", codec_info,current_codec_info,ordered_codec_info);
    if(codec_info != ordered_codec_info)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Derived code mode is different from Ordered codec mode");
      status = FAILURE;
    }
  }
  if(status == SUCCESS)
  {
    tdsconfig_ptr_in_use = TDSORDERED_CONFIG;
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
    if(!tdsrrciho_is_g2td_ho_active())
#endif
    {
      if((TRUE == tdsordered_config_ptr->is_hho) && (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH))
      {
        /*Send HHO start to CM*/
        tdsrrc_send_hho_cmd_to_cm(SYS_HO_START);
      }
      ordered_codec_status_flag = tdsrrc_get_ordered_codec_status();
      if((tdsordered_config_ptr->act_time_type == TDSL1_ACTIVATION_TIME_NONE) && (TRUE == ordered_codec_status_flag)) /*Add the activation_time check also if needed*/
      {
        if(FALSE == tdsrrc_is_voice_rab_present() || (ordered_codec_info != current_codec_info))
        {
          /*Sending codec info to CM*/
          tdsrrc_send_codec_info_to_cm(ordered_codec_info, TDSRRC_SAMPLING_RATE_NB);
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"is_voice_rab %d, ordered codec %d current codec %d",tdsrrc_is_voice_rab_present(),ordered_codec_info,current_codec_info);
        }
        tdsrrc_set_ordered_codec_status(FALSE);
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_MED,"Activation type is %d, ordered config status %d",tdsordered_config_ptr->act_time_type,ordered_codec_status_flag);
      }
    }
#endif
  }


  return status;
} /* end tdsrrcllc_copy_msg_ie_to_oc() */

/*====================================================================
FUNCTION: rrcllc_get_dl_trch_idx()

DESCRIPTION:
  This function manages DL PCH TrCH Indexes for the UE.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL PCH TrCH Index that can be used.

SIDE EFFECTS:
  None.
====================================================================*/
uint8 tdsrrcllc_get_dl_pch_trch_idx(
  tdsordered_config_type  *config_ptr,
  tr_ch_id_type transportChannelIdentity)
{
  /* The PCH TrCH index is always fixed as 0 since there is only
   * 1 PCH
   */
  uint8  pch_trch_idx = 0;

  //  tdsordered_config_ptr->dl_trch_cnt++;
  /* Allocate an index and update structures if an index is not being used */
  if(config_ptr->dl_pch_trch_idx_in_use == FALSE)
  {
    config_ptr->dl_pch_trch_idx_in_use = TRUE;
    /* Update the L1 count of transport channels */
    config_ptr->l1_dl_cctrch_parms.num_trch++;
  }

  /* Number of PCHs can not be more than 1. Ensure it is 1 whenever
   * SCCPCH has a PCH selected. Update MAC count
   */
  config_ptr->mac_dl_parms.num_pchs = 1;

  /* Save the Transport Channel ID in Local and L1 info */
  /* Note: MAC doesn't care for PCH Transport channel ID */
  config_ptr->dl_pch_trch_info.trch_id  =
  config_ptr->l1_dl_cctrch_parms.trch_info_array[pch_trch_idx]->trch_id =
    (uint8) transportChannelIdentity;

  /* Indicate to L1 that this reference TrCH type is a PCH */
  config_ptr->l1_dl_cctrch_parms.trch_info_array[pch_trch_idx]->
    ref_type = TDSL1_DL_TRCH_PCH_TYPE;

  return (pch_trch_idx);
} /* end tdsrrcllc_get_dl_pch_trch_idx() */

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: rrcllc_get_dl_pch_trch_idx_for_sccpch_with_ctch()

DESCRIPTION:
  This function manages DL PCH TrCH Indexes for the UE.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL PCH TrCH Index that can be used.

SIDE EFFECTS:
  None.
====================================================================*/
uint8 rrcllc_get_dl_pch_trch_idx_for_sccpch_with_ctch(
  ordered_config_type  *config_ptr,
  tr_ch_id_type transportChannelIdentity)
{
  /* The PCH TrCH index is always fixed as 0 since there is only
   * 1 PCH
   */
  uint8  pch_trch_idx = 0;

  //  ordered_config_ptr->dl_trch_cnt++;
  /* Allocate an index and update structures if an index is not being used */
  if(config_ptr->dl_pch_trch_idx_in_use_for_sccpch_with_ctch == FALSE)
  {
    config_ptr->dl_pch_trch_idx_in_use_for_sccpch_with_ctch = TRUE;
    /* Update the L1 count of transport channels */
    config_ptr->l1_dl_cctrch_parms_for_sccpch_with_ctch.num_trch++;
  }

  config_ptr->l1_dl_cctrch_parms_for_sccpch_with_ctch.trch_info_array[pch_trch_idx]->trch_id =
    (uint8) transportChannelIdentity;

  /* Indicate to L1 that this reference TrCH type is a PCH */
  config_ptr->l1_dl_cctrch_parms_for_sccpch_with_ctch.trch_info_array[pch_trch_idx]->
    ref_type = L1_DL_TRCH_PCH_TYPE;

  return (pch_trch_idx);
} /* end rrcllc_get_dl_pch_trch_idx() */


/*====================================================================
FUNCTION: rrcllc_allocate_dl_fach_trch_idx_for_sccpch_with_ctch()

DESCRIPTION:
  This function allocates a DL FACH TrCH Index.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL FACH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL FACH TrCH Indexes are available.
====================================================================*/
uint8 rrcllc_allocate_dl_fach_trch_idx_for_sccpch_with_ctch(ordered_config_type *config_ptr)
{
  uint8 trch_idx; /* local loop variable */

  /* check for a valid DL TrCH Index */
  for(trch_idx = 0; trch_idx < UE_MAX_TRCH; trch_idx++)
  {
    if (config_ptr->dl_fach_trch_idx_in_use_for_sccpch_with_ctch[trch_idx] == FALSE)
    {
      /*
      * If this case holds true, indicate that this DL TrCH Index is used,
      * break the for loop and use this index as the DL TrCH Index.
      */
      config_ptr->dl_fach_trch_idx_in_use_for_sccpch_with_ctch[trch_idx] = TRUE;
      break;
    }
  }
  /* If we reached the max => no index was found */
  if (trch_idx == UE_MAX_TRCH)
  {
    /* no available unused TrCH index */
    trch_idx = RRCLLC_INVALID_TRCH_IDX;
    ERR("No available DL FACH TrCH Index");
  }
  /* always return a DL TrCH Index */
  return (trch_idx);
} /* end rrcllc_allocate_dl_fach_trch_idx_for_sccpch_with_ctch() */
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

/*====================================================================
FUNCTION: tdsrrcllc_allocate_dl_fach_trch_idx()

DESCRIPTION:
  This function allocates a DL FACH TrCH Index.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL FACH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL FACH TrCH Indexes are available.
====================================================================*/
static uint8 tdsrrcllc_allocate_dl_fach_trch_idx(tdsordered_config_type *config_ptr)
{
  uint8 trch_idx; /* local loop variable */

  /* check for a valid DL TrCH Index */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if (config_ptr->dl_fach_trch_idx_in_use[trch_idx] == FALSE)
    {
      /*
      * If this case holds true, indicate that this DL TrCH Index is used,
      * break the for loop and use this index as the DL TrCH Index.
      */
      config_ptr->dl_fach_trch_idx_in_use[trch_idx] = TRUE;
      break;
    }
  }
  /* If we reached the max => no index was found */
  if (trch_idx == TDSUE_MAX_TRCH)
  {
    /* no available unused TrCH index */
    trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No available DL FACH TrCH Index");
  }
  /* always return a DL TrCH Index */
  return (trch_idx);
} /* end tdsrrcllc_allocate_dl_fach_trch_idx() */

/*====================================================================
FUNCTION: tdsrrcllc_find_dl_fach_trch_idx()

DESCRIPTION:
  This function looks for and returns the 1st allocated transport
  channel index for FACH.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL FACH TrCH Index that was already allocated.
  TDSRRCLLC_INVALID_TRCH_IDX -> No index allocated to a FACH.

SIDE EFFECTS:
====================================================================*/
uint8  tdsrrcllc_find_dl_fach_trch_idx(void)
{
  uint8 trch_idx;

  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if((tdsordered_config_ptr->dl_fach_trch_info[trch_idx].trch_id != 0) &&
       (tdsordered_config_ptr->dl_fach_trch_idx_in_use[trch_idx] == TRUE))
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      break;
    }
  }
  if(trch_idx == TDSUE_MAX_TRCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL FACH TrCH idx not found");
    trch_idx =  TDSRRCLLC_INVALID_TRCH_IDX;
  }

  return(trch_idx);
}

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_fach_trch_idx()

DESCRIPTION:
  This function gets a DL FACH TrCH Index for a given TrCH ID. If an
  index already exists, it is returned. Otherwise a new one is
  allocated for this TrCH ID.
  If a new TrCH index is allocated, TrCH count in MAC, L1 and local
  data for TrCH ID and reference type is updated. The number of
  DL transport channels is also incremented in local, L1 and MAC
  structures.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL FACH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL FACH TrCH Indexes are available.
====================================================================*/
uint8 tdsrrcllc_get_dl_fach_trch_idx(
  tdsordered_config_type  *config_ptr,
  tr_ch_id_type  tdstrch_id,
  uint8 pch_trch_idx
)
{
  /* transport Channel Index */
  uint8         trch_idx;

  /* Indicate if already have this TrCH Id */
  boolean       trch_idx_found = FALSE;

  /* First look if this Transport Channel ID is already present
   * in an index that is being IN Use.
   */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if((config_ptr->dl_fach_trch_info[trch_idx].trch_id == tdstrch_id) &&
       (config_ptr->dl_fach_trch_idx_in_use[trch_idx] == TRUE))
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      trch_idx_found = TRUE;
      break;
    }
  }

  if(trch_idx_found)
  {
    /* Since a Transport Channel Index that is in use has been found,
     * return this index. No need to change any counters or initialize
     * transport channel ID.
     */
    return(trch_idx);
  }
  else  /* TrCH index not found, allocate new */
  {
    /* An index with the same transport channel ID is NOT in use.
     * Need to allocate a new index.
     */
    trch_idx = tdsrrcllc_allocate_dl_fach_trch_idx(config_ptr);
    
    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      /* Failure to allocate a new TrCH Index */
      return(trch_idx);
    }
    else  /* A valid new index has been allocated */
    {
      /* Now we've got a new TrCH index - need to initialize TrCH
       * data in local, MAC and L1 structures.
       */
      /* Update the Local info */
      config_ptr->dl_fach_trch_info[trch_idx].trch_id     = tdstrch_id;
      config_ptr->dl_fach_trch_info[trch_idx].dch_present = FALSE;

      /* Save Transport Channel ID in MAC Configuration Data */
      config_ptr->fach_info[trch_idx].trch_id = tdstrch_id;

      /* Save the TrCH type and TrCH ID to L1 data */
      config_ptr->dl_trch_info[pch_trch_idx+trch_idx].ref_type =
        TDSL1_DL_TRCH_FACH_TYPE;
      config_ptr->dl_trch_info[pch_trch_idx+trch_idx].trch_id =
        (unsigned char)tdstrch_id;

      /* MAC count */
      config_ptr->mac_dl_parms.num_fachs++;
      /* L1 count */
      config_ptr->l1_dl_cctrch_parms.num_trch++;

      return(trch_idx);

    } /* A valid new index allocated */
  } /* Existing transport channel index couldn't be found, allocated new */
} /* tdsrrcllc_get_dl_fach_trch_idx */

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: rrcllc_get_dl_fach_trch_idx_for_sccpch_with_ctch()

DESCRIPTION:
  This function gets a DL FACH TrCH Index for a given TrCH ID. If an
  index already exists, it is returned. Otherwise a new one is
  allocated for this TrCH ID.
  If a new TrCH index is allocated, TrCH count in MAC, L1 and local
  data for TrCH ID and reference type is updated. The number of
  DL transport channels is also incremented in local, L1 and MAC
  structures.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL FACH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL FACH TrCH Indexes are available.
====================================================================*/
uint8 rrcllc_get_dl_fach_trch_idx_for_sccpch_with_ctch(
  ordered_config_type  *config_ptr,
  tr_ch_id_type  trch_id,
  uint8 pch_trch_idx
)
{
  /* transport Channel Index */
  uint8         trch_idx;

  /* Indicate if already have this TrCH Id */
  boolean       trch_idx_found = FALSE;

  /* First look if this Transport Channel ID is already present
   * in an index that is being IN Use.
   */
  for(trch_idx = 0; trch_idx < UE_MAX_TRCH; trch_idx++)
  {
    if((config_ptr->dl_fach_trch_info_for_sccpch_with_ctch[trch_idx].trch_id == trch_id) &&
       (config_ptr->dl_fach_trch_idx_in_use_for_sccpch_with_ctch[trch_idx] == TRUE))
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      trch_idx_found = TRUE;
      break;
    }
  }

  if(trch_idx_found)
  {
    /* Since a Transport Channel Index that is in use has been found,
     * return this index. No need to change any counters or initialize
     * transport channel ID.
     */
    return(trch_idx);
  }
  else  /* TrCH index not found, allocate new */
  {
    /* An index with the same transport channel ID is NOT in use.
     * Need to allocate a new index.
     */
    trch_idx = rrcllc_allocate_dl_fach_trch_idx_for_sccpch_with_ctch(config_ptr);
    if(trch_idx >= UE_MAX_TRCH)
    {
      /* Failure to allocate a new TrCH Index */
      return(trch_idx);
    }
    else  /* A valid new index has been allocated */
    {
      /* Now we've got a new TrCH index - need to initialize TrCH
       * data in local, MAC and L1 structures.
       */
      /* Update the Local info */
      config_ptr->dl_fach_trch_info_for_sccpch_with_ctch[trch_idx].trch_id     = trch_id;
      config_ptr->dl_fach_trch_info_for_sccpch_with_ctch[trch_idx].dch_present = FALSE;

      /* Save Transport Channel ID in MAC Configuration Data */
      config_ptr->fach_info[config_ptr->mac_dl_parms.num_fachs].trch_id = trch_id;

      /* Save the TrCH type and TrCH ID to L1 data */
      config_ptr->dl_trch_info_for_sccpch_with_ctch[pch_trch_idx+trch_idx].ref_type =
        L1_DL_TRCH_FACH_TYPE;
      config_ptr->dl_trch_info_for_sccpch_with_ctch[pch_trch_idx+trch_idx].trch_id =
        (unsigned char)trch_id;

      /* MAC count */
      config_ptr->mac_dl_parms.num_fachs++;
      /* L1 count */
      config_ptr->l1_dl_cctrch_parms_for_sccpch_with_ctch.num_trch++;

      return(trch_idx);

    } /* A valid new index allocated */
  } /* Existing transport channel index couldn't be found, allocated new */
} /* rrcllc_get_dl_fach_trch_idx_for_sccpch_with_ctch */
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

/*====================================================================
FUNCTION: tdsrrcllc_allocate_dl_dch_trch_idx()

DESCRIPTION:
  This function allocates a DL DCH TrCH Index.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL DCH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL DCH TrCH Indexes are available.
====================================================================*/
uint8 tdsrrcllc_allocate_dl_dch_trch_idx(void)
{
  uint8 trch_idx; /* local loop variable */

  /* check for a valid DL TrCH Index */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if (tdsordered_config_ptr->dl_dch_trch_idx_in_use[trch_idx] == FALSE)
    {
      /*
      * If this case holds true, indicate that this DL TrCH Index is used,
      * break the for loop and use this index as the DL TrCH Index.
      */
      tdsordered_config_ptr->dl_dch_trch_idx_in_use[trch_idx] = TRUE;
      break;
    }
  }
  /* If we reached the max => no index was found */
  if (trch_idx == TDSUE_MAX_TRCH)
  {
    /* no available unused TrCH index */
    trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No available DL TDSDCH TrCH Indexes");
  }
  /* always return a DL TrCH Index */
  return (trch_idx);
} /* end tdsrrcllc_allocate_dl_dch_trch_idx() */

/*====================================================================
FUNCTION: tdsrrcllc_find_dl_dch_trch_idx()

DESCRIPTION:
  This function looks for and returns a transport channel index
  for a given DCH transport channel ID. It returns a valid index only
  if it was already assigned to the given DCH transport channel ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL DCH TrCH Index that was already allocated to
           given transport channel ID.
  TDSRRCLLC_INVALID_TRCH_IDX -> Given Transport channel ID does not
           have an allocated DCH transport channel index.

SIDE EFFECTS:
====================================================================*/
uint8  tdsrrcllc_find_dl_dch_trch_idx(tr_ch_id_type  tdstrch_id)
{
  uint8 trch_idx;

  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if((tdsordered_config_ptr->dl_dch_trch_info[trch_idx].dch_present) &&
       (tdsordered_config_ptr->dl_dch_trch_info[trch_idx].trch_id == tdstrch_id) &&
       (tdsordered_config_ptr->dl_dch_trch_idx_in_use[trch_idx] == TRUE))
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      break;
    }
  }
  if(trch_idx == TDSUE_MAX_TRCH)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DL TDSDCH TrCH idx not found: ID %d", tdstrch_id);
    trch_idx =  TDSRRCLLC_INVALID_TRCH_IDX;
  }

  return(trch_idx);
}

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_dch_trch_idx()

DESCRIPTION:
  This function gets a DL DCH TrCH Index for a given TrCH ID. If an
  index already exists, it is returned. Otherwise a new one is
  allocated for this TrCH ID.
  If a new TrCH index is allocated, TrCH count in MAC, L1 and local
  data for TrCH ID and reference type is updated. The number of
  DL transport channels is also incremented in local, L1 and MAC
  structures.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL DCH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL DCH TrCH Indexes are available.
====================================================================*/
uint8 tdsrrcllc_get_dl_dch_trch_idx(tr_ch_id_type  tdstrch_id)
{
  /* transport Channel Index */
  uint8         trch_idx;

  /* Indicate if already have this TrCH Id */
  boolean       trch_idx_found = FALSE;

  /* First look if this Transport Channel ID is already present
   * in an index that is being IN Use.
   */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if((tdsordered_config_ptr->dl_dch_trch_info[trch_idx].dch_present) &&
       (tdsordered_config_ptr->dl_dch_trch_info[trch_idx].trch_id == tdstrch_id) &&
       (tdsordered_config_ptr->dl_dch_trch_idx_in_use[trch_idx] == TRUE))
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      trch_idx_found = TRUE;
      break;
    }
  }

  if(trch_idx_found)
  {
    /* Since a Transport Channel Index that is in use has been found,
     * return this index. No need to change any counters or initialize
     * transport channel ID.
     */
    return(trch_idx);
  }
  else  /* TrCH index not found, allocate new */
  {
    /* An index with the same transport channel ID is NOT in use.
     * Need to allocate a new index.
     */
    trch_idx = tdsrrcllc_allocate_dl_dch_trch_idx();

    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      /* Failure to allocate a new TrCH Index */
      return(trch_idx);
    }
    else  /* A valid new index has been allocated */
    {
      /* Now we've got a new TrCH index - need to initialize TrCH
       * data in local, MAC and L1 structures.
       */
      /* Update the Local info */
      tdsordered_config_ptr->dl_dch_trch_info[trch_idx].dch_present = TRUE;
      tdsordered_config_ptr->dl_dch_trch_info[trch_idx].trch_id     = tdstrch_id;

      /* Save Transport Channel ID in MAC Configuration Data */
      tdsordered_config_ptr->dl_dch_info[trch_idx].trch_id = tdstrch_id;

      /* Save the TrCH type and TrCH ID to L1 data */
      tdsordered_config_ptr->dl_trch_info[trch_idx].ref_type = TDSL1_DL_TRCH_DCH_TYPE;
      tdsordered_config_ptr->dl_trch_info[trch_idx].trch_id = (unsigned char)tdstrch_id;

      /* MAC count */
      tdsordered_config_ptr->mac_dl_parms.num_dchs++;
      /* L1 count */
      tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch++;
     /* set tdsdl_tfcs_needed flag to true.  Later if common info is not preset,
      * UE needs to reject the message 
      */
      tdsdl_tfcs_needed = TRUE;
      return(trch_idx);

    } /* A valid new index allocated */
  } /* Existing transport channel index couldn't be found, allocated new */
} /* tdsrrcllc_get_dl_dch_trch_idx */


/*====================================================================
FUNCTION: tdsrrcllc_deallocate_dl_dch_trch_idx()

DESCRIPTION:
  This function deallocates a DL DCH TrCH Index for later UE use.

DEPENDENCIES:
  None.

RETURN VALUE:
  ue_comdef_status_e_type -> SUCCESS indicates a DL DCH TrCH Index
                             was cleared
                             FAILURE indicates a DL DCH TrCH Index
                             was not cleared
SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_deallocate_dl_dch_trch_idx(uint8 trch_idx)
{
  uecomdef_status_e_type status = SUCCESS;

  /*
  * Check to make sure that the DL TrCH Index is in range
  */
  if(trch_idx < TDSUE_MAX_TRCH)
  {
    tdsordered_config_ptr->dl_dch_trch_idx_in_use[trch_idx] = FALSE;
  }
  else
  {
    /* Bad TrCH Index */
    TDSRRC_MSG0(MSG_LEGACY_MED,"Cannot free an invalid DL FACH TrCH Index");
    status = FAILURE;
  }

  return status;
} /* end tdsrrcllc_deallocate_dl_dch_trch_idx() */

/*====================================================================
FUNCTION: tdsrrcllc_allocate_ul_rach_trch_idx()

DESCRIPTION:
  This function allocates UL RACH TrCH Indexes for the UE.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A UL RACH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no UL RACH TrCH Indexes are available.
====================================================================*/
static uint8 tdsrrcllc_allocate_ul_rach_trch_idx
(
  boolean  *ul_rach_trch_idx_in_use
)
{
  uint8 trch_idx; /* local loop variable */

  /* check for a valid UL TrCH Index */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if (ul_rach_trch_idx_in_use[trch_idx] == FALSE)
    {
      /*
      * If this case holds true, indicate that this UL TrCH Index is used,
      * break the for loop and use this index as the UL TrCH Index.
      */
      ul_rach_trch_idx_in_use[trch_idx] = TRUE;
      break;
    }
  }
  if (trch_idx == TDSUE_MAX_TRCH)
  {
    /* no available TrCHs index's */
    trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
    TDSRRC_MSG0(MSG_LEGACY_MED,"No available UL RACH TrCH Indexes");
  }

  /* always return a UL TrCH Index */
  return trch_idx;

} /* end tdsrrcllc_allocate_ul_rach_trch_idx() */

/*====================================================================
FUNCTION: tdsrrcllc_find_ul_rach_trch_idx()

DESCRIPTION:
  This function looks for an already allocated transport channel index
  for an UL RACH transport channel.

DEPENDENCIES:
  None.

RETURN VALUE:
  Transport Channel Index.
  If no index found, returns TDSRRCLLC_INVALID_TRCH_IDX.

SIDE EFFECTS:
  None.
====================================================================*/
uint8  tdsrrcllc_find_ul_rach_trch_idx(void)
{
  uint8 trch_idx;
  /* Indicate if already have this TrCH Id */
  boolean       trch_idx_found = FALSE;

  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if(tdsordered_config_ptr->ul_rach_trch_idx_in_use[trch_idx] == TRUE)
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      trch_idx_found = TRUE;
      break;
    }
  }
  if(trch_idx_found)
  {
    return(trch_idx);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"UL TrCH idx not found for ID %d", trch_idx);
    return(TDSRRCLLC_INVALID_TRCH_IDX);
  }
}  /* tdsrrcllc_find_ul_rach_trch_idx */

/*====================================================================
FUNCTION: tdsrrcllc_get_ul_rach_trch_idx_for_prach_db()

DESCRIPTION:
  This function gets a UL DCH TrCH Index for a given TrCH ID. If an
  index already exists, it is returned. Otherwise a new one is
  allocates for this TrCH ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A UL RACH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL RACH TrCH Indexes are available.
  If a new TrCH index is allocated, TrCH count in MAC, L1 and local
  data is incremented.
====================================================================*/
uint8 tdsrrcllc_get_ul_rach_trch_idx_for_prach_db
(
  tdsprach_config_type *prach_config_ptr,
  tdsrrcllc_prach_sel_info_type * prach_sel_info_ptr,
  tr_ch_id_type  tdstrch_id
)
{
  /* transport Channel Index */
  uint8         trch_idx;

  /* Indicate if already have this TrCH Id */
  boolean       trch_idx_found = FALSE;

  /* First look if this Transport Channel ID is already present
   * in an index that is being IN Use.
   */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if((prach_config_ptr->ul_rach_trch_info[trch_idx].trch_id == tdstrch_id) &&
       (prach_config_ptr->ul_rach_trch_idx_in_use[trch_idx] == TRUE))
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      trch_idx_found = TRUE;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Found RACH trch_idx = %d",trch_idx);
      break;
    }
  }

  if(trch_idx_found)
  {
    /* Since a Transport Channel Index that is in use has been found,
     * return this index. No need to change any counters or initialize
     * transport channel ID.
     */
    return(trch_idx);
  }
  else  /* TrCH index not found, allocate new */
  {
    /* An index with the same transport channel ID is NOT in use.
     * Need to allocate a new index.
     */
    trch_idx = tdsrrcllc_allocate_ul_rach_trch_idx(prach_config_ptr->ul_rach_trch_idx_in_use);

    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      /* Failure to allocate a new TrCH Index */
      return(trch_idx);
    }
    else  /* A valid new index has been allocated */
    {
      /* Now we've got a new TrCH index - need to initialize TrCH
       * data in local, MAC and L1 structures.
       */
      /* Update the Local info */
      prach_config_ptr->ul_rach_trch_info[trch_idx].dch_present = FALSE;
      prach_config_ptr->ul_rach_trch_info[trch_idx].trch_id     = tdstrch_id;

      /* MAC TrCH count in TFCS */
      prach_sel_info_ptr->ul_tfcs.nchan++;

      /* L1 count */
#if 0
      prach_sel_info_ptr->l1_ul_cctrch_parms.num_trch ++;
      if (prach_sel_info_ptr->l1_ul_cctrch_parms.num_trch > 1) 
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"PRACH shall not have more that 1 TrCH");
      }
#endif
      return(trch_idx);
    }/* A valid new index allocated */
  } /* Existing transport channel index couldn't be found, allocated new */

}

/*====================================================================
FUNCTION: tdsrrcllc_get_ul_rach_trch_idx()

DESCRIPTION:
  This function gets a UL DCH TrCH Index for a given TrCH ID. If an
  index already exists, it is returned. Otherwise a new one is
  allocates for this TrCH ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A UL RACH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL RACH TrCH Indexes are available.
  If a new TrCH index is allocated, TrCH count in MAC, L1 and local
  data is incremented.
====================================================================*/
uint8 tdsrrcllc_get_ul_rach_trch_idx
(
  tdsordered_config_type *config_ptr,
  tr_ch_id_type  tdstrch_id
)
{
  /* transport Channel Index */
  uint8         trch_idx;

  /* Indicate if already have this TrCH Id */
  boolean       trch_idx_found = FALSE;

  /* First look if this Transport Channel ID is already present
   * in an index that is being IN Use.
   */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if((config_ptr->ul_rach_trch_info[trch_idx].trch_id == tdstrch_id) &&
       (config_ptr->ul_rach_trch_idx_in_use[trch_idx] == TRUE))
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      trch_idx_found = TRUE;
      break;
    }
  }

  if(trch_idx_found)
  {
    /* Since a Transport Channel Index that is in use has been found,
     * return this index. No need to change any counters or initialize
     * transport channel ID.
     */
    return(trch_idx);
  }
  else  /* TrCH index not found, allocate new */
  {
    /* An index with the same transport channel ID is NOT in use.
     * Need to allocate a new index.
     */
    trch_idx = tdsrrcllc_allocate_ul_rach_trch_idx(config_ptr->ul_rach_trch_idx_in_use);

    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      /* Failure to allocate a new TrCH Index */
      return(trch_idx);
    }
    else  /* A valid new index has been allocated */
    {
      /* Now we've got a new TrCH index - need to initialize TrCH
       * data in local, MAC and L1 structures.
       */
      /* Update the Local info */
      config_ptr->ul_rach_trch_info[trch_idx].dch_present = FALSE;
      config_ptr->ul_rach_trch_info[trch_idx].trch_id     = tdstrch_id;

      /* Save Transport Channel ID in MAC Configuration Data */
      config_ptr->ul_cctrch.rach_info.trch_id = tdstrch_id;
      config_ptr->ul_cctrch.cctrch_type = TDSRACH;

      /* Save Transport Channel ID in L1 UL configuration */
      config_ptr->l1_ul_chan_parms.prach_cfg.trch_id =
        (uint8) tdstrch_id;

      /* Local count */
      //tdsordered_config_ptr->ul_trch_cnt ++;

      /* No MAC count for RACH - only 1 permissible */

      /* MAC TrCH count in TFCS */
      config_ptr->ul_tfcs.nchan++;

      /* L1 count */
#if 0
      config_ptr->l1_ul_cctrch_parms.num_trch ++;
      if (config_ptr->l1_ul_cctrch_parms.num_trch > 1) 
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"PRACH shall not have more that 1 TrCH");
      }
#endif
      return(trch_idx);
    } /* A valid new index allocated */
  } /* Existing transport channel index couldn't be found, allocated new */
} /* tdsrrcllc_get_ul_rach_trch_idx */


/*====================================================================
FUNCTION: tdsrrcllc_allocate_ul_dch_trch_idx()

DESCRIPTION:
  This function manages UL DCH TrCH Indexes for the UE.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A UL DCH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no UL DCH TrCH Indexes are available.
====================================================================*/
uint8 tdsrrcllc_allocate_ul_dch_trch_idx(void)
{
  uint8 trch_idx; /* local loop variable */

  /* check for a valid UL TrCH Index */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if (tdsordered_config_ptr->ul_dch_trch_idx_in_use[trch_idx] == FALSE)
    {
      /*
      * If this case holds true, indicate that this UL TrCH Index is used,
      * break the for loop and use this index as the UL TrCH Index.
      */
      tdsordered_config_ptr->ul_dch_trch_idx_in_use[trch_idx] = TRUE;
      break;
    }
  }
  if(trch_idx == TDSUE_MAX_TRCH)
  {
    /* no available TrCHs index's */
    trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No available UL TDSDCH TrCH Indexes");
  }
  /* always return a UL TrCH Index */
  return trch_idx;
} /* end tdsrrcllc_allocate_ul_dch_trch_idx() */

/*====================================================================
FUNCTION: tdsrrcllc_find_ul_dch_trch_idx()

DESCRIPTION:
  This function looks for and returns a transport channel index for a
  given UL DCH transport channel ID. It returns a valid index only
  if it was already assigned to the given DCH transport channel ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A UL DCH TrCH Index that was already allocated to
           given transport channel ID.
  TDSRRCLLC_INVALID_TRCH_IDX -> Given Transport channel ID does not
           have an allocated DCH transport channel index.

SIDE EFFECTS:
====================================================================*/
uint8  tdsrrcllc_find_ul_dch_trch_idx(tdsordered_config_type *config_ptr,
                                   tr_ch_id_type  tdstrch_id)
{
  uint8 trch_idx;
  /* Indicate if already have this TrCH Id */
  boolean       trch_idx_found = FALSE;

  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if((config_ptr->ul_dch_trch_info[trch_idx].dch_present) &&
       (config_ptr->ul_dch_trch_info[trch_idx].trch_id == tdstrch_id) &&
       (config_ptr->ul_dch_trch_idx_in_use[trch_idx] == TRUE))
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      trch_idx_found = TRUE;
      break;
    }
  }
  if(trch_idx_found)
  {
    return(trch_idx);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TrCH idx not found for ID %d", tdstrch_id);
    return(TDSRRCLLC_INVALID_TRCH_IDX);
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_get_ul_dch_trch_idx()

DESCRIPTION:
  This function gets a UL DCH TrCH Index for a given TrCH ID. If an
  index already exists, it is returned. Otherwise a new one is
  allocates for this TrCH ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A UL DCH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL DCH TrCH Indexes are available.
  If a new TrCH index is allocated, TrCH count in MAC, L1 and local
  data is incremented.
====================================================================*/
uint8 tdsrrcllc_get_ul_dch_trch_idx(tr_ch_id_type  tdstrch_id)
{
  /* transport Channel Index */
  uint8         trch_idx;

  /* Indicate if already have this TrCH Id */
  boolean       trch_idx_found = FALSE;

  /* First look if this Transport Channel ID is already present
   * in an index that is being IN Use.
   */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if((tdsordered_config_ptr->ul_dch_trch_info[trch_idx].dch_present) &&
       (tdsordered_config_ptr->ul_dch_trch_info[trch_idx].trch_id == tdstrch_id) &&
       (tdsordered_config_ptr->ul_dch_trch_idx_in_use[trch_idx] == TRUE))
    {
      /* Get Transport channel mapping index, since
       * it is already in database
       */
      trch_idx_found = TRUE;
      break;
    }
  }

  if(trch_idx_found)
  {
    /* Since a Transport Channel Index that is in use has been found,
     * return this index. No need to change any counters or initialize
     * transport channel ID.
     */
    return(trch_idx);
  }
  else  /* TrCH index not found, allocate new */
  {
    /* An index with the same transport channel ID is NOT in use.
     * Need to allocate a new index.
     */
    trch_idx = tdsrrcllc_allocate_ul_dch_trch_idx();

    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      /* Failure to allocate a new TrCH Index */
      return(trch_idx);
    }
    else  /* A valid new index has been allocated */
    {
      /* Now we've got a new TrCH index - need to initialize TrCH
       * data in local, MAC and L1 structures.
       */
      /* Update the Local info */
      tdsordered_config_ptr->ul_dch_trch_info[trch_idx].dch_present = TRUE;
      tdsordered_config_ptr->ul_dch_trch_info[trch_idx].trch_id     = tdstrch_id;

      /* Save Transport Channel ID in MAC Configuration Data */
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].trch_id = tdstrch_id;
      tdsordered_config_ptr->ul_cctrch.cctrch_type = TDSDCH;

      /* MAC count */
      tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs ++;
      /* MAC TrCH count in TFCS */
      tdsordered_config_ptr->ul_tfcs.nchan++;
      /* L1 count */

      tdsordered_config_ptr->rrc_ul_cctrch_params.num_trch ++;
      
      /* set tdsul_tfcs_needed flag to true.  Later if common info is not preset,
       * UE needs to reject the message 
       */
      tdsul_tfcs_needed = TRUE;
      return(trch_idx);
    } /* A valid new index allocated */
  } /* Existing transport channel index couldn't be found, allocated new */
} /* tdsrrcllc_get_ul_dch_trch_idx */

/*====================================================================
FUNCTION: tdsrrcllc_deallocate_ul_dch_trch_idx()

DESCRIPTION:
  This function clears a UL DCH TrCH Index for later UE use.

DEPENDENCIES:
  None.

RETURN VALUE:
  ue_comdef_status_e_type -> SUCCESS indicates a UL DCH TrCH Index was cleared
                             FAILURE indicates a UL DCH TrCH Index was not cleared

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_deallocate_ul_dch_trch_idx(uint8 trch_idx)
{
  uecomdef_status_e_type status = SUCCESS;

  /*
  * Check to make sure that the UL TrCH Index is in range
  */
  if(trch_idx < TDSUE_MAX_TRCH)
  {
    tdsordered_config_ptr->ul_dch_trch_idx_in_use[trch_idx] = FALSE;
  }
  else
  {
    /* Bad TrCH Index */
    TDSRRC_MSG0(MSG_LEGACY_MED,"Cannot free an invalid UL TDSDCH TrCH Index");
    status = FAILURE;
  }

  return status;
} /* end tdsrrcllc_deallocate_ul_dch_trch_idx() */


/*====================================================================
FUNCTION: tdsrrcllc_check_ul_tfs_cnt()

DESCRIPTION:
  This function verifies that the number of Transport Format sets
  received in an OTA Message is within the UE Capabilities.

DEPENDENCIES:
  None.

RETURN VALUE:
  ue_comdef_status_e_type -> SUCCESS indicates # of UL TFS < UE Cap
                             FAILURE indicates # of UL TFS > UE Cap

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_check_ul_tfs_cnt(void)
{
  /* local variables */
  uint8  ix;
  uint32 tfs_cnt = 0;

  /* First get a count of all UL TFS into ordered config */
  for(ix = 0; ix < tdsordered_config_ptr->mac_ul_parms.tfcs_ptr->nchan; ix++)
  {
    tfs_cnt += tdsordered_config_ptr->mac_ul_parms.tfcs_ptr->tfs_info[ix].ntf;
  }

  /* Check to make sure it is less than UE Capability */
  if (tfs_cnt <= TDSUE_MAX_TF)
  {
    TDSRRC_MSG1(MSG_LEGACY_LOW,"UL TFS Cnt = %d", tfs_cnt);
    return SUCCESS;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"UL TFS Cnt %d > UE Cap of 32", tfs_cnt);
    return FAILURE;
  }
} /* end tdsrrcllc_check_ul_tfs_cnt() */

/*====================================================================
FUNCTION: tdsrrcllc_check_dl_tfs_cnt()

DESCRIPTION:
  This function verifies that the number of Transport Format sets
  received in an OTA Message is within the UE Capabilities.

DEPENDENCIES:
  None.

RETURN VALUE:
  ue_comdef_status_e_type -> SUCCESS indicates # of DL TFS < UE Cap
                             FAILURE indicates # of DL TFS > UE Cap

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_check_dl_tfs_cnt(void)
{
  /* local variables */
  uint8  ix;
  uint32 tfs_cnt = 0;

  /* First get a count of all DL TFS into ordered config */
  for(ix = 0; ix < tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch; ix++)
  {
    tfs_cnt += tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[ix]->num_tf;
  }

  /* Check to make sure it is less than UE Capability */
  if (tfs_cnt <= TDSUE_MAX_TF)
  {
    TDSRRC_MSG1(MSG_LEGACY_LOW,"DL TFS Cnt = %d", tfs_cnt);
    return SUCCESS;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL TFS Cnt %d > UE Cap of 64", tfs_cnt);
    return FAILURE;
  }
} /* end tdsrrcllc_check_dl_tfs_cnt() */



/*====================================================================
FUNCTION: tdsrrcllc_rearrange_dflow_index()

DESCRIPTION:
  This function deletes any hoes in dl_dflow_index_in_use structure
  and the MAC DFLOW structure.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/

void tdsrrcllc_rearrange_dflow_index
(
  void
)
{
  uint8  ix;
  uint8  count = 0;

  if (tdsordered_config_ptr->mac_dl_parms.num_dflow == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Num DFLOW 0");
    return;
  }
  /* Put all the values in tmp structure. No there are no holes in tmp structure */
  for(ix = 0; ix < tdsordered_config_ptr->mac_dl_parms.num_dflow; ix ++)
  {
    /* Check if valid data */
    if((tdsordered_config_ptr->mac_dflow_info[ix].mac_dflow_id != TDSINVALID_DFLOW_ID) &&
      (tdsordered_config_ptr->dl_dflow_index_in_use[ix] == TRUE))
    {
      memscpy(&tdstmp_dl_dlfow_index_in_use[count],sizeof(boolean), &tdsordered_config_ptr->dl_dflow_index_in_use[ix],
        sizeof(boolean));

      memscpy(&tdstmp_mac_dflow_info[count], sizeof(tdsmac_dflow_struct_type),&tdsordered_config_ptr->mac_dflow_info[ix], sizeof(tdsmac_dflow_struct_type));

      /* Now reinitialize the dflow index to indicate that it is avaliable */
      memset(&tdsordered_config_ptr->dl_dflow_index_in_use[ix], 0, sizeof(tdsordered_config_ptr->dl_dflow_index_in_use[ix]));

      memset(&tdsordered_config_ptr->mac_dflow_info[ix].mac_dflow_id, TDSINVALID_DFLOW_ID, sizeof(tdsordered_config_ptr->mac_dflow_info[ix].mac_dflow_id));

      count++;

    }  /* If this index is not being used */
  } /* Outer for (...) */

  /* Now copy back the tmp structure into the original structure. */
  for(ix = 0; ix < count; ix ++)
  {
    memscpy(&tdsordered_config_ptr->dl_dflow_index_in_use[ix],sizeof(boolean), &tdstmp_dl_dlfow_index_in_use[ix], sizeof(boolean));
    memscpy(&tdsordered_config_ptr->mac_dflow_info[ix],sizeof(tdsmac_dflow_struct_type), &tdstmp_mac_dflow_info[ix], sizeof(tdsmac_dflow_struct_type));
  }

}


/*============================================================================
FUNCTION: tdsrrcllc_rearrange_queue_index()

DESCRIPTION:
  This function rearranges the queue for a particular DFLOW so that the hole
  which may have been created earlier is filled up

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_rearrange_queue_index
(
  void
)
{
  uint8  ix, iy;
  uint8  count;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Rearranging queue index");

  if (tdsordered_config_ptr->mac_dl_parms.num_dflow == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:DFLOW 0");
    return;
  }
  /* Put all the values in tmp structure. No there are no holes in tmp structure */
  for(ix = 0; ix < tdsordered_config_ptr->mac_dl_parms.num_dflow; ix ++)
  {
    /* Check if valid data */
    if((tdsordered_config_ptr->mac_dflow_info[ix].mac_dflow_id != TDSINVALID_DFLOW_ID) &&
      (tdsordered_config_ptr->dl_dflow_index_in_use[ix] == TRUE))
    {
      count = 0;
      for(iy = 0; iy < tdsordered_config_ptr->mac_dflow_info[ix].no_of_queues; iy ++)
      {
        if (tdsordered_config_ptr->mac_dflow_info[ix].queue_info[iy].queue_id != TDSINVALID_QUEUE_ID)
        {
          memscpy(&tdstmp_queue_info[count], sizeof(tdsmac_hs_queue_struct_type),&tdsordered_config_ptr->mac_dflow_info[ix].queue_info[iy], sizeof(tdsmac_hs_queue_struct_type));
          /* Make existing one as TDSINVALID */
          tdsordered_config_ptr->mac_dflow_info[ix].queue_info[iy].queue_id = TDSINVALID_QUEUE_ID;
          count++;
        }
      }
      /* Now copy back the queue */
      if (count == 0)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:No copy back");
        tdsordered_config_ptr->mac_dflow_info[ix].no_of_queues = count;
        continue;
      }
      else
      {
        for(iy = 0; iy < count; iy ++)
        {
          /* Copy back the queue in first locn */
          memscpy(&tdsordered_config_ptr->mac_dflow_info[ix].queue_info[iy],sizeof(tdsmac_hs_queue_struct_type), &tdstmp_queue_info[iy], sizeof(tdsmac_hs_queue_struct_type));
        }
        tdsordered_config_ptr->mac_dflow_info[ix].no_of_queues = count;
      }
      /* Holes removed for this DFLOW Id */
    }  /* If this index is being used */
  } /* End of for (...) */

}



/*====================================================================
FUNCTION: tdsrrcllc_fill_holes_in_dl_dch_trch_idx()

DESCRIPTION:
  This function goes over the local transport channel index database
  for downlink and looks for any holes in the index allocation.
  If an index is found un-allocated, it is allocated and all
  transport channel data from a later index is copied to the
  local, MAC and L1 structures pointed to by the 1st index. The
  later downlink index is deleted and de-allocated.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_fill_holes_in_dl_dch_trch_idx(void)
{

  uint8  ix;
  uint8  iy;
  uint8  new_idx;

  for(ix = 0; ix < 
       (TDSRRCLLC_DOWNLINK_TRCH_COUNT(tdsordered_config_ptr))
       ; ix ++)
  {
    /* Move data to fill holes */
    if(tdsordered_config_ptr->dl_dch_trch_idx_in_use[ix] == FALSE)
    {
      /* Look for a index in use in the rest of list */
      for(iy = ix+1; iy < TDSUE_MAX_TRCH; iy ++)
      {
        if(tdsordered_config_ptr->dl_dch_trch_idx_in_use[iy] == TRUE)
        {
          /* Copy all data for TrCH iy to a temp buffer */
          tdsrrcllc_copy_dl_dch_trch_info_to_buffer(iy);

          /* Delete transport channel index iy - this also clears
           * all data associated with index iy
           */
          (void)tdsrrcllc_delete_dl_dch_trch_index(iy);

          /* Allocate a new TrCH index */
          if( (new_idx = tdsrrcllc_get_dl_dch_trch_idx(tdsordered_config_ptr->
                   mac_dl_parms.dch_info[iy]->trch_id)) < TDSUE_MAX_TRCH)
          {
                                               

            /* Copy data from buffer to the new index just allocated. */
            tdsrrcllc_copy_dl_dch_trch_info_from_buffer(new_idx);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"new_idx for DL %d exceeds TDSUE_MAX_TRCH ",new_idx);
          }

          break;
        }
      }  /* for(.... ) */
    }  /* If this index is not being used */
  } /* Outer for (...) */
} /* tdsrrcllc_fill_holes_in_dl_dch_trch_idx */

/*====================================================================
FUNCTION: tdsrrcllc_fill_holes_in_ul_dch_trch_idx()

DESCRIPTION:
  This function goes over the local transport channel index database
  for uplink and looks for any holes in the index allocation.
  If an index is found un-allocated, it is allocated and all
  transport channel data from a later index is copied to the
  local, MAC and L1 structures pointed to by the 1st index. The
  later uplink index is deleted and de-allocated.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_fill_holes_in_ul_dch_trch_idx(void)
{
  uint8  ix;
  uint8  iy;
  uint8  new_idx;

  for(ix = 0; ix < (TDSRRCLLC_UPLINK_TRCH_COUNT(tdsordered_config_ptr)); ix ++)
  {
    /* Move data to fill holes */
    if(tdsordered_config_ptr->ul_dch_trch_idx_in_use[ix] == FALSE)
    {
      /* Look for a index in use in the rest of list */
      for(iy = ix+1; iy < TDSUE_MAX_TRCH; iy ++)
      {
        if(tdsordered_config_ptr->ul_dch_trch_idx_in_use[iy] == TRUE)
        {

          /* Copy data from iy to the temporary buffer. */
          tdsrrcllc_copy_ul_dch_trch_info_to_buffer(iy);

          /* Delete transport channel index iy - this also clears
           * all data associated with index iy
           */
          (void)tdsrrcllc_delete_ul_dch_trch_index(iy);

          /* Allocate a new index - should be ix */
          if(( new_idx = tdsrrcllc_get_ul_dch_trch_idx(tdsordered_config_ptr->
                mac_ul_parms.cctrch_ptr->dch_info.dch_info[iy].trch_id)
                ) < TDSUE_MAX_TRCH)
          {

            /* Copy data to new index */
            tdsrrcllc_copy_ul_dch_trch_info_from_buffer(new_idx);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"new_idx for UL %d exceeds TDSUE_MAX_TRCH ",new_idx);
          }
          break;
        }
      }  /* for(.... ) */
    }  /* If this index is not being used */
  } /* Outer for (...) */
} /* tdsrrcllc_fill_holes_in_ul_dch_trch_idx */

/*====================================================================
FUNCTION: tdsrrcllc_check_if_rlc_mode_is_am()

DESCRIPTION:
 This function queries CC and finds out whether a given RB rb_id is of 
 AM RLC mode or not.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - RB rb_id is of AM RLC mode.
  FALSE - RB rb_id isn't of AM RLC mode.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_check_if_rlc_mode_is_am
(
  tdsrrc_RB_Identity rb_id
)
{
  uint8 am_idx = 0;
  boolean rb_found = FALSE;
  
  for(am_idx = 0; am_idx < TDSUE_MAX_AM_ENTITY; am_idx++)
  {
    if(tdscurrent_config_ptr->am_config[am_idx].common_config.rb_id == rb_id)
    {
      rb_found = TRUE;
      break;
    }
  }
  return rb_found;
}
/*====================================================================
FUNCTION: tdsrrcllc_check_if_rlc_mode_is_um()

DESCRIPTION:
 This function queries CC and finds out whether a given RB rb_id is of 
 UM RLC mode or not.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - RB rb_id is of UM RLC mode.
  FALSE - RB rb_id isn't of UM RLC mode.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_check_if_rlc_mode_is_um
(
  tdsrrc_RB_Identity rb_id
)
{
  uint8 um_idx = 0;
  boolean rb_found = FALSE;
  
  for(um_idx = 0; um_idx < UE_MAX_UL_UM_CHANNEL; um_idx++)
  {
    if(tdscurrent_config_ptr->rlc_ul_um_parms.chan_info[um_idx].rb_id == rb_id)
    {
      rb_found = TRUE;
      return rb_found;
    }
  }
  for(um_idx = 0; um_idx < UE_MAX_DL_UM_CHANNEL; um_idx++)
  {
    if(tdscurrent_config_ptr->rlc_dl_um_parms.chan_info[um_idx].rb_id == rb_id)
    {
      rb_found = TRUE;
      return rb_found;
    }
  }	
  return rb_found;
}
/*====================================================================
FUNCTION: tdsrrcllc_fill_rb_list_mapped_to_dch()

DESCRIPTION:
  This function fills the given RB rb_id in the global structure 
  tdsrrc_rb_list_mapped_to_dch. It does so only if the given RB rb_id 
  isn't already present in tdsrrc_rb_list_mapped_to_dch.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_fill_rb_list_mapped_to_dch
(
  tdsrrc_RB_Identity rb_id
)
{
  uint8 rb_idx = 0;
  boolean rb_found = FALSE;

  /*To make sure that no. of RBs don't exceed in array rrc_rb_mapped_to_dch*/
  if((tdsrrc_rb_list_mapped_to_dch.num_of_RBs < TDS_MAX_RB) && (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
  {
    for(rb_idx = 0; rb_idx < tdsrrc_rb_list_mapped_to_dch.num_of_RBs; rb_idx++)
    {
      if(tdsrrc_rb_list_mapped_to_dch.rrc_rb_mapped_to_dch[rb_idx] == rb_id)
      {
        rb_found = TRUE;
        break; 
      }
    }
    /*Only if rb_id isn't already present in rrc_rb_mapped_to_dch*/ 
    if(rb_found == FALSE)
    {
      if(tdsrrc_rb_list_mapped_to_dch.num_of_RBs < TDS_MAX_RB)  
      {
        tdsrrc_rb_list_mapped_to_dch.rrc_rb_mapped_to_dch[tdsrrc_rb_list_mapped_to_dch.num_of_RBs] = rb_id;
      }
      tdsrrc_rb_list_mapped_to_dch.num_of_RBs++;      
    }
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"No.%d of RBs in tdsrrc_rb_list_mapped_to_dch. And RB-Id to be added is %d",
            tdsrrc_rb_list_mapped_to_dch.num_of_RBs,rb_id);
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_get_rb_for_rb_list_mapped_to_dch()

DESCRIPTION:
  This function calls tdsrrcllc_fill_rb_list_mapped_to_dch() to fill the 
  given RB in tdsrrc_rb_list_mapped_to_dch, if RLC PDU size for that RB has 
  changed. The third parameter is to notify whether RLC PDU size will be 
  checked for UL or DL.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
====================================================================*/
void tdsrrcllc_get_rb_for_rb_list_mapped_to_dch
(
  tr_ch_id_type tdstrch_id, 
  tdsrrc_state_e_type next_state, 
  tdsrrc_direction_enum_type direction
)
{
  uint8 trch_idx=0, rb_idx = 0;
  uint32 rlc_size_old = 0, rlc_size_new = 0;
  
  tdsrrc_RB_Identity   rb_id = TDS_MAX_RB;
  
  if(tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)
  {
    /*If trch_id is given for UL*/
    if(direction == TDSUL)
    {
      /*Loop thru all the UL DCH TrCH in OC*/
      for(trch_idx = 0; trch_idx < tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->
              dch_info.ndchs;trch_idx++)
      {
        if(tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_idx].trch_id == tdstrch_id)
        {
          /*UL DCH TrCH trch_id was found in OC. Now loop thru all the RBs mapped to trch_id TrCH*/
          for(rb_idx = 0;rb_idx < tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->
                dch_info.dch_info[trch_idx].ndlchan; rb_idx++)
          {
            if(tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_idx].
                  dlchan_info[rb_idx].rlc_mode == UE_MODE_ACKNOWLEDGED_DATA)
            {
              rb_id = tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_idx].
                      dlchan_info[rb_idx].rb_id;
              rlc_size_old = tdsrrcllc_get_am_rlc_size_for_rb(rb_id, direction);
              rlc_size_new = tdsrrcllc_get_ul_ded_rlc_size(next_state, rb_id);
            
              /*We need to add the RB in rb_list_mapped_to_dch only when RLC size has changed.*/              
              if((rlc_size_old != TDSRRCLLC_INVALID_RLC_SIZE) && (rlc_size_new != TDSRRCLLC_INVALID_RLC_SIZE) 
                      && (rlc_size_old != rlc_size_new))
              {
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Old & New UL RLC size for RB %d are %u and %u",rb_id, rlc_size_old, rlc_size_new);   
                tdsrrcllc_fill_rb_list_mapped_to_dch(rb_id);
              }
            }
          }
        }
      }
    }
    /*If trch_id is given for DL*/
    else if(direction == TDSDL)
    {
      /*Loop thru all the DL DCH TrCH in OC*/
      for(trch_idx = 0; trch_idx < tdsordered_config_ptr->mac_dl_parms.num_dchs; trch_idx++)
      {
        if(tdsordered_config_ptr->mac_dl_parms.dch_info[trch_idx]->trch_id == tdstrch_id)
        {
          /*DL DCH TrCH trch_id was found in OC. Now loop thru all the RBs mapped to trch_id TrCH*/      
          for(rb_idx = 0;rb_idx < tdsordered_config_ptr->mac_dl_parms.dch_info[trch_idx]->ndlchan; rb_idx++)
          {
            if((tdsordered_config_ptr->mac_dl_parms.dch_info[trch_idx]->dlchan_info[rb_idx].rlc_mode == 
                    UE_MODE_ACKNOWLEDGED_DATA)
                && (tdsrrcllc_get_am_dl_rlc_size_ota(tdsordered_config_ptr->mac_dl_parms.dch_info[trch_idx]->dlchan_info[rb_idx].rb_id)
                    == TDSRRCLLC_INVALID_RLC_SIZE)

                    )
            {
              rb_id =  tdsordered_config_ptr->mac_dl_parms.dch_info[trch_idx]->dlchan_info[rb_idx].rb_id;  
              rlc_size_old = tdsrrcllc_get_am_rlc_size_for_rb(rb_id, direction);
              rlc_size_new = tdsrrcllc_get_dl_ded_rlc_size(next_state, rb_id);
    
              /*We need to add the RB in rb_list_mapped_to_dch only when RLC size has changed.*/          
              if((rlc_size_old != TDSRRCLLC_INVALID_RLC_SIZE) && (rlc_size_new != TDSRRCLLC_INVALID_RLC_SIZE) 
                      && (rlc_size_old != rlc_size_new))
              {
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Old & New DL RLC size for RB %d are %u and %u",rb_id, rlc_size_old, rlc_size_new);            
                tdsrrcllc_fill_rb_list_mapped_to_dch(rb_id);
              }
            }
          }
        }
      }
    }
  }
}


/*====================================================================
FUNCTION: tdsrrcllc_update_lbt_mode_for_dchs()

DESCRIPTION:
  This function updates the Loopback Test mode in CCTrCH Info for
  L1 in the Ordered Config.
  It updates the mode for those transport channels that have a DTCH
  logical channel mapped.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
#ifdef FEATURE_TC
void tdsrrcllc_update_lbt_mode_for_dchs(
  tdsl1_loop_back_mode_type_enum_type tdsloopback_test_mode
#ifdef FEATURE_DUMMY_SIGNALING
  #error code not present
#endif
)
{
  uint32 dch_idx;
  uint32 dlch_count;

  /* Check for all Transport Channels */
  for( dch_idx = 0;
       dch_idx < tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch;
       dch_idx ++
      )
  {
    if(tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[dch_idx]->
        ref_type == TDSL1_DL_TRCH_DCH_TYPE)
    {
      for( dlch_count = 0;
           dlch_count < tdsordered_config_ptr->mac_dl_parms.dch_info[dch_idx]->ndlchan;
           dlch_count ++)
      {

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
        switch(tdsloopback_test_mode)
        {
          case TDSL1_LOOPBACK_MODE_1:
            tdsordered_config_ptr->mac_dl_parms.dch_info[dch_idx]->loopback_test_mode
              = TDSMAC_LOOPBACK_MODE_1;
            break;
          case TDSL1_LOOPBACK_MODE_2:
            tdsordered_config_ptr->mac_dl_parms.dch_info[dch_idx]->loopback_test_mode
              = TDSMAC_LOOPBACK_MODE_2;
            break;
          default:
            tdsordered_config_ptr->mac_dl_parms.dch_info[dch_idx]->loopback_test_mode
              = TDSMAC_LOOPBACK_NA;
            break;
        }
#endif /* FEATURE_TDSCDMA_TC_CONSTANT_DELAY */

        if(tdsordered_config_ptr->mac_dl_parms.dch_info[dch_idx]->
            dlchan_info[dlch_count].chan_type
              == UE_LOGCHAN_DTCH)
        {
          /* This DCH is mapped to a DTCH - yes, update the mode */
          tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[dch_idx]->
            loopback_mode = tdsloopback_test_mode;

          /* If loopback mode 1 is chosen, determine whether to indicate LOOPBACK_MODE_1 with
           * TM or NON_TM bearers to L1. IF TM RBs are setup for LB, L1 passes up the blocks
           * to RLC regardless of CRC.  Else for non-tm, L1 will discard blocks that don't pass CRC
           */
          if (tdsloopback_test_mode == TDSL1_LOOPBACK_MODE_1) 
          {
            if (tdsordered_config_ptr->mac_dl_parms.dch_info[dch_idx]->
            dlchan_info[dlch_count].rlc_mode == UE_MODE_TRANSPARENT) 
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting L1 loopback mode to TDSL1_LOOPBACK_MODE_1 for TM RB idx:%d",dch_idx);
              tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[dch_idx]->non_tm_rb = FALSE;
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting L1 loopback mode to TDSL1_LOOPBACK_MODE_1 for Non-TM RB idx: %d",dch_idx);
              tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[dch_idx]->non_tm_rb = TRUE;
            }
          }
          /* Move to the next transport channel */
          break;
        } /* Is Logical Channel type a DTCH ? */
      }  /* Loop for all available Dedicated Logical channels mapped on this DCH */
    } /* This TrCH is a DCH */
  } /* Loop for all available DCHs */

  if(tdsloopback_test_mode == TDSL1_LOOPBACK_MODE_2)
  {
    /* Additionally, for loopback mode 2, zero out the CRC length
     * for all user plane UL TrCHs
     */
    for(dch_idx = 0;
        dch_idx < tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs;
        dch_idx ++)
    {
      for(dlch_count = 0;
          dlch_count < tdsordered_config_ptr->mac_ul_parms.
                       cctrch_ptr->dch_info.dch_info[dch_idx].ndlchan;
          dlch_count ++)
      {
        if(tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.
            dch_info[dch_idx].dlchan_info[dlch_count].chan_type
           == UE_LOGCHAN_DTCH)
        {
#ifndef FEATURE_TM_LB
          tdsordered_config_ptr->l1_ul_cctrch_parms.
            semi_static_parms_ptr[dch_idx]->crc = TDSL1_CRC_LENGTH_0;
#endif 
          tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
          break;
        }
      } /* For all dedicated logical channels on this DCH */
    } /* For all DCHs */
  } /* If this is Loopback mode 2 */

#ifdef FEATURE_DUMMY_SIGNALING
    #error code not present
#endif

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  switch(tdsloopback_test_mode)
  {
    case TDSL1_LOOPBACK_MODE_1:
      tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.loopback_test_mode
        = TDSMAC_LOOPBACK_MODE_1;
      break;
    case TDSL1_LOOPBACK_MODE_2:
      tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.loopback_test_mode
        = TDSMAC_LOOPBACK_MODE_2;
      break;
    default:
      tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.loopback_test_mode
        = TDSMAC_LOOPBACK_NA;
      break;
  }

  tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
#endif /* FEATURE_TDSCDMA_TC_CONSTANT_DELAY */

  /* Till a solution is deviced to fix the lower layer out-of-sync issue
   * with activation time, copy the Ordered Config to Current Config
   * Note: For LBT always keep CC and OC in sync since Physical channels
   * are not Dropped and Added for LBT.
   */
  tdsrrcllc_copy_oc_to_cc();

  return;
} /* tdsrrcllc_update_lbt_mode_for_dchs */
#endif

/*====================================================================
FUNCTION: tdsrrcllc_update_lbt_mode_for_fachs()

DESCRIPTION:
  This function updates the Loopback Test mode in CCTrCH Info for
  L1 in the Ordered Config.
  It updates the mode for those transport channels that have a DTCH
  logical channel mapped.

DEPENDENCIES:
  This function would update the test mode only for LOOPBACK mode 1.
  Loopback mode 2 is not applicable for FACH since constnat delay
  can't be applied in RACH.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
#ifdef FEATURE_TC
void tdsrrcllc_update_lbt_mode_for_fachs(
  tdsl1_loop_back_mode_type_enum_type tdsloopback_test_mode
)
{
  uint32 fach_idx;
  uint32 dlch_count;

  /* Check for all Transport Channels */
  for( fach_idx = 0;
       fach_idx < tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch;
       fach_idx ++
      )
  {
    if(tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[fach_idx]->
        ref_type == TDSL1_DL_TRCH_FACH_TYPE)
    {
      for( dlch_count = 0;
           dlch_count < tdsordered_config_ptr->mac_dl_parms.fach_info[fach_idx]->ndlchan;
           dlch_count ++)
      {

        if(tdsordered_config_ptr->mac_dl_parms.fach_info[fach_idx]->
            dlchan_info[dlch_count].chan_type
              == UE_LOGCHAN_DTCH)
        {
          /* This FACH is mapped to a DTCH - yes, update the mode */
          tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[fach_idx]->
            loopback_mode = tdsloopback_test_mode;

          /* If loopback mode 1 is chosen, determine whether to indicate LOOPBACK_MODE_1 with
           * TM or NON_TM bearers to L1. IF TM RBs are setup for LB, L1 passes up the blocks
           * to RLC regardless of CRC.  Else for non-tm, L1 will discard blocks that don't pass CRC
           */
          if (tdsloopback_test_mode == TDSL1_LOOPBACK_MODE_1) 
          {
            if (tdsordered_config_ptr->mac_dl_parms.fach_info[fach_idx]->
                dlchan_info[dlch_count].rlc_mode == UE_MODE_TRANSPARENT) 
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting L1 loopback mode to TDSL1_LOOPBACK_MODE_1 for TM RB idx:%d",fach_idx);
              tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[fach_idx]->non_tm_rb = FALSE;
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting L1 loopback mode to TDSL1_LOOPBACK_MODE_1 for non-TM RB idx:%d",fach_idx);
              tdsordered_config_ptr->l1_dl_cctrch_parms.trch_info_array[fach_idx]->non_tm_rb = TRUE;
            }
          }
          
          /* Move to the next transport channel */
          break;
        } /* Is Logical Channel type a DTCH ? */
      }  /* Loop for all available Dedicated Logical channels mapped on this FACH */
    } /* This TrCH is a FACH*/
  } /* Loop for all available DCHs */

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
  switch(tdsloopback_test_mode)
  {
    case TDSL1_LOOPBACK_MODE_1:
      tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->rach_info.loopback_test_mode
        = TDSMAC_LOOPBACK_MODE_1;
      break;
    case TDSL1_LOOPBACK_MODE_2:
      tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->rach_info.loopback_test_mode
        = TDSMAC_LOOPBACK_NA;
      break;
    default:
      tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.loopback_test_mode
        = TDSMAC_LOOPBACK_NA;
      break;
  }

  tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
#endif /* FEATURE_TDSCDMA_TC_CONSTANT_DELAY */

  /* Till a solution is deviced to fix the lower layer out-of-sync issue
   * with activation time, copy the Ordered Config to Current Config
   * Note: For LBT always keep CC and OC in sync since Physical channels
   * are not Dropped and Added for LBT.
   */
  tdsrrcllc_copy_oc_to_cc();

  return;
} /* tdsrrcllc_update_lbt_mode_for_dchs */
#endif


/*====================================================================
FUNCTION: tdsrrcllc_init_dl_common_info()
DESCRIPTION:
  This function is responsible for initializing DL Common Info
  for DPCH. The only field that needs to be init is the Timing
  Initialization type. It is set to "Timing Maintained".
DEPENDENCIES:
  None
RETURN VALUE:
  None
SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_common_info(tdsordered_config_type *config_ptr)
{
  /* Initialize to Timing Maintained */
  config_ptr->l1_dl_chan_parms.dpch.dpch_common.timing_ind = TDSL1_DL_PHYCFG_TIMING_IND_MAINTAIN;
}
/*====================================================================
FUNCTION: tdsrrcllc_timing_init_in_progress()
DESCRIPTION:
  This function returns true if timing init HHO is ongoing.
DEPENDENCIES:
  None
RETURN VALUE:
  None
SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_timing_init_in_progress(void)
{
  tdsordered_config_type *config_ptr = NULL;

  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }

  if (config_ptr->l1_dl_chan_parms.dpch.dpch_common.timing_ind == TDSL1_DL_PHYCFG_TIMING_IND_INIT) 
  {
    return TRUE;
  }
  else
  { 
    return FALSE;
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_init_mac_dl_ptrs()

DESCRIPTION:
  This function is responsible for initializing pointers to FACH, DCH
  and DSCH info in MAC DL Parameter structure to the storage
  space for FACH, DCH and DSCH info respectively.
  NOTE: Once tdsmac_dl_config_type is changed to have the data
  structures in place of pointers, this function will not be
  needed anymore.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_mac_dl_ptrs(tdsordered_config_type *config_ptr)
{
  uint8  ix;
  for(ix = 0 ; ix < TDSUE_MAX_TRCH; ix++)
  {
    /* MAC FACH, DCH, and DSCH data pointers */
    config_ptr->mac_dl_parms.fach_info[ix] = &config_ptr->fach_info[ix];
    config_ptr->mac_dl_parms.dch_info[ix]  = &config_ptr->dl_dch_info[ix];
    config_ptr->mac_dl_parms.dsch_info[ix] = &config_ptr->dsch_info[ix];
  }

  TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Init MAC HS Ptr");
  tdsrrcllc_init_hsdpa_mac_ptr(config_ptr);

}

/*====================================================================
FUNCTION: tdsrrcllc_init_mac_ul_ptrs()

DESCRIPTION:
  This function is responsible for initializing pointers to
  TFCS and CCTrCH configuration in MAC UL Parameter structure to the
  TFCS and CCTrCH Config info in tdsordered_config_type respectively.
  NOTE: Once tdsmac_ul_config_type is changed to have the data
  structures in place of pointers, this function will not be
  needed anymore.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_mac_ul_ptrs(tdsordered_config_type *config_ptr)
{
  /* Save the address of the MAC Uplink TFCS info */
  config_ptr->mac_ul_parms.tfcs_ptr = &config_ptr->ul_tfcs;

  /* Save the address of the MAC Uplink CCTrCH info */
  config_ptr->mac_ul_parms.cctrch_ptr = &config_ptr->ul_cctrch;

#ifdef FEATURE_TDSCDMA_HSUPA
  config_ptr->mac_ul_parms.mac_e_config_ptr = &config_ptr->mac_e_config;
#endif /* FEATURE_TDSCDMA_HSUPA*/

}

/*====================================================================
FUNCTION: tdsrrcllc_init_l1_dl_ptrs()

DESCRIPTION:
  This function is responsible for initializing pointers to
  transport channel info (for each TrCH) in l1_dl_cctrch_parms
  to dedicated or common TrCH info in ordered config type
  depending on the passed RRC state. It also initializes the
  pointers to Transport format info inside dedicated or common TrCH info
  to dedicated or common transport format info respectively.
  This also initializes pointers to CTFC info in l1_dl_ctfc_parms
  to ctfc_info in tdsordered_config_type for all CTFCs. The pointers
  to TFI table inside ctfc_info are initialized to TFI table inside
  the local dl_ctfc in tdsordered_config_type.

  NOTE: Once l1_dl_cctrch_parms, tdsl1_dl_cctrchcfg_trch_info_struct_type,
  l1_dl_ctfc_parms and tdsl1dlcctrchcfg_dl_ctfc_info_struct_type are changed
  to have the data structures in place of pointers, this function will
  not be needed anymore.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_l1_dl_ptrs
(
  tdsordered_config_type *config_ptr
)
{
  uint8  ix;
  uint8  iy;
  uint16  tdsnum_tfc;

  for(ix = 0 ; ix < TDSL1_DL_MAX_TRCH; ix++)
  {
    /* In Cell_DCH state, initialize Transport Channel Info array
     * to point to Dedicated Transport Channel Info array elements.
     */
    config_ptr->l1_dl_cctrch_parms.trch_info_array[ix] =
      &config_ptr->dl_trch_info[ix];

    /* Init the DL Transport Format data pointers, indicate common at first */
    for (iy = 0; iy < TDSL1_DL_MAX_TF_PER_TRCH; iy++)
    {
      config_ptr->dl_trch_info[ix].tf_info_array[iy] = &config_ptr->dl_tf_info[ix][iy];
    } /* end TF initialization */
  }
#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  for(ix = 0 ; ix < TDSL1_DL_MAX_TRCH; ix++)
  {
    /* In Cell_DCH state, initialize Transport Channel Info array
     * to point to Dedicated Transport Channel Info array elements.
     */
    config_ptr->l1_dl_cctrch_parms_for_sccpch_with_ctch.trch_info_array[ix] =
      &config_ptr->dl_trch_info_for_sccpch_with_ctch[ix];

    /* Init the DL Transport Format data pointers, indicate common at first */
    for (iy = 0; iy < TDSL1_DL_MAX_TF; iy++)
    {
      config_ptr->dl_trch_info_for_sccpch_with_ctch[ix].tf_info_array[iy] =
        &config_ptr->dl_tf_info_for_sccpch_with_ctch[ix][iy];
    } /* end TF initialization */
  }
  for(tdsnum_tfc = 0; tdsnum_tfc < TDSUE_MAX_TFC; tdsnum_tfc++)
  {
    /* Init the DL Transport Format Combination data pointers */
    config_ptr->l1_dl_ctfc_parms_for_sccpch_with_ctch.ctfc_entry_array[tdsnum_tfc] =
      &config_ptr->ctfc_info_for_sccpch_with_ctch[tdsnum_tfc];

    /* Initialize the pointer to tfi_table in CTFC Info */
    config_ptr->ctfc_info_for_sccpch_with_ctch[tdsnum_tfc].tfi_table = config_ptr->dl_ctfc_for_sccpch_with_ctch.ctfc_entry_data[tdsnum_tfc].tfi_table;

  }
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif
  for(tdsnum_tfc = 0; tdsnum_tfc < TDSUE_MAX_TFC; tdsnum_tfc++)
  {
    /* Init the DL Transport Format Combination data pointers */
    config_ptr->l1_dl_ctfc_parms.ctfc_entry_array[tdsnum_tfc] = &config_ptr->ctfc_info[tdsnum_tfc];
    /* Initialize the pointer to tfi_table in CTFC Info */
    config_ptr->ctfc_info[tdsnum_tfc].tfi_table = config_ptr->dl_ctfc.ctfc_entry_data[tdsnum_tfc].tfi_table;
  }


}  /* tdsrrcllc_init_l1_dl_ptrs */

/*====================================================================
FUNCTION: tdsrrcllc_init_l1_ul_ptrs()

DESCRIPTION:
  This function is responsible for initializing pointers to
  semi static and gain factor info in L1 UL CCTrCH Parameter structure
  to the info stored locally in tdsordered_config_type. It also
  initializes pointers to ASC Info in L1 UL Channel Parameters.

  NOTE: Once l1_ul_cctrch_parms and l1_ul_chan_parms are changed
  to have the data structures in place of pointers, this function will
  not be needed anymore.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_l1_ul_ptrs(tdsordered_config_type *config_ptr)
{
  uint8  ix;

  for(ix = 0 ; ix < TDSUE_MAX_TRCH; ix++)
  {

    /* L1 UL Semi static parameter data pointers */
    config_ptr->l1_ul_cctrch_parms.semi_static_parms_ptr[ix] =
      &config_ptr->rrc_ul_cctrch_params.semi_static_parms[ix];
  }
  for(ix = 0; ix < TDSL1_UL_TFC_MAX; ix++)
  {

    /* Init the UL Gain Factors that are TFC specific */
    config_ptr->l1_ul_cctrch_parms.gain_factor_parms_ptr[ix] =
      &config_ptr->rrc_ul_cctrch_params.gain_factor_parms[ix];

  } /* end TFC initialization */

  /* Init the ASC data pointers */
  for(ix = 0; ix < MAX_ASC; ix ++)
  {
    config_ptr->l1_ul_chan_parms.prach_cfg.asc_cfg.asc_info_ptr[ix] =
      &config_ptr->asc_info[ix];
  } /* end ASC initialization */
  
#ifdef FEATURE_TDSCDMA_HSUPA
  for(ix = 0; ix < MAX_ASC; ix ++)
  {
    config_ptr->l1_e_info.e_ul_info.e_rucch_info.asc_cfg.asc_info_ptr[ix] =
      &config_ptr->erucch_asc_info[ix];
  } /* end E-RUCCH ASC initialization */
#endif /* FEATURE_TDSCDMA_HSUPA*/
} /* tdsrrcllc_init_l1_ul_ptrs */

/*====================================================================
FUNCTION: tdsrrcllc_init_rlc_ptrs()

DESCRIPTION:
  This function is responsible for initializing pointers to
  AM Info in RLC AM parameter structure to the AM Configuration
  stored locally in tdsordered_config_type.

  NOTE: Once tdsrlc_am_config_req_type is changed to have the data
  structures in place of pointers, this function will not be
  needed anymore.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_rlc_ptrs(tdsordered_config_type *config_ptr)
{
  uint8  ix;
  /* Init the local AM data pointers, DL & UL channel counts */
  for(ix = 0; ix < TDSUE_MAX_AM_ENTITY; ix++)
  {
    config_ptr->rlc_am_parms.am_info[ix] = &config_ptr->am_config[ix];
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_copy_bch_info_from_cc_to_oc()

DESCRIPTION:
  This function is responsible for copying all BCH related info from
  Current Config to Ordered Config.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_copy_bch_info_from_cc_to_oc(void)
{
  uint8  bch_count;

  /* Copy the BCH count from Current to Ordered Config */
  tdsordered_config_ptr->mac_dl_parms.num_bchs =
    tdscurrent_config_ptr->mac_dl_parms.num_bchs;

  /* Copy BCH info for all active BCHs from current to ordered config */
  for(bch_count=0;
      bch_count < tdsordered_config_ptr->mac_dl_parms.num_bchs;
      bch_count++)
  {
    tdsordered_config_ptr->mac_dl_parms.bch_info->bcch_id =
     tdscurrent_config_ptr->mac_dl_parms.bch_info->bcch_id;
    tdsordered_config_ptr->mac_dl_parms.bch_info->cctrch_id =
     tdscurrent_config_ptr->mac_dl_parms.bch_info->cctrch_id;
  }
}  /* tdsrrcllc_copy_bch_info_from_cc_to_oc */

/*====================================================================
FUNCTION: tdsrrcllc_copy_ccch_info_from_cc_to_oc()

DESCRIPTION:
  This function is responsible for copying all CCCH related info from
  Current Config to Ordered Config.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_copy_ccch_info_from_cc_to_oc(void)
{
  uint32  count, num_fach;
  /* Copy CCCH info in Uplink and Downlink */

  if(tdsordered_config_ptr->mac_dl_parms.num_fachs == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Number of FACHes in OC zero");
    return;
  }
  if(tdscurrent_config_ptr->mac_dl_parms.num_fachs == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Number of FACHes in CC zero");
    return;
  }

  /* Update Downlink first */
  /* Copy for all FACHes in the Ordered Config. Since there
   * is atleast one FACH in Current Config, copy from the data
   * for the first FACH.
   */
  num_fach = tdsordered_config_ptr->mac_dl_parms.num_fachs;
  for(count = 0; count < num_fach; count ++)
  {
    tdsordered_config_ptr->mac_dl_parms.fach_info[count]->ccch_enable =
      tdscurrent_config_ptr->mac_dl_parms.fach_info[0]->ccch_enable;
    tdsordered_config_ptr->mac_dl_parms.fach_info[count]->ccch_id =
      tdscurrent_config_ptr->mac_dl_parms.fach_info[0]->ccch_id;
  }

  /* Update Uplink next */
  tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_asc =
    tdscurrent_config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_asc;

  tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_enable =
    tdscurrent_config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_enable;

  tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_id =
    tdscurrent_config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ccch_id;

  /* Since RLC info is not kept track of, no point of copying it */
} /* tdsrrcllc_copy_ccch_info_from_cc_to_oc */



/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_pch_for_pcch_reconfig()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  in cell_pch or URA pch state.

DEPENDENCIES:
  For a PCH add/reconfig operation, all SIBs should be read before
  calling this function.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type  tdsrrcllc_update_oc_in_pch_for_pcch_reconfig
(
  tdsordered_config_type *config_ptr,
  tdsrrc_state_e_type state
)
{
  uecomdef_status_e_type  oc_status = SUCCESS;

  /* Set the Activation Time to "Now" */
  tdsrrcllc_init_activation_time(config_ptr);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Init OC before updating with SIB");

  /* Initialize PCH Info */
  tdsrrcllc_init_pch_info(config_ptr);
  /* Do not delete RB Mapping Info in any other RRC state */
  tdsrrcllc_init_fach_info(config_ptr, FALSE);
  /* Initialize L1 SCCPCH Channel Info */
  tdsrrcllc_init_l1_sccpch_chan_info(config_ptr);
  /* Initialize L1 Downlink CCTrCH Info that includes Transport Format Set */
  tdsrrcllc_init_dl_cctrch_info(config_ptr);
  /* Initialize Downlink TFCS */
  tdsrrcllc_init_dl_ctfcs(config_ptr);
#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrcllc_init_dl_ctfcs_for_sccpch_with_ctch(config_ptr);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif
  oc_status = tdsrrcllc_copy_sib_to_oc(state);

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  if (tdsrrcllc_semi_permanent_data.dl_phy_chan.ctch_status == TDSRRCLLC_CTCH_ON_DIF_SCCPCH )
  {
    (void)rrcllc_copy_sib_to_oc_for_sccpch_with_ctch(state);
  }
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

  return oc_status;
}

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_disconnected()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  in Disconnected state. It is based on the PCH and BCH operation type
  that is passed as a parameter.
  It starts with copying Current Config to Ordered Config and then
  updating Ordered Config data. For a PCH add/reconfig, OC is updated
  with SIBs and in the event of a failure, the Ordered Config data is
  reverted back to Current Config data.

DEPENDENCIES:
  For a PCH add/reconfig operation, all SIBs should be read before
  calling this function.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type  tdsrrcllc_update_oc_in_disconnected
(
  tdsordered_config_type *config_ptr,
  tdsrrcllc_bch_op_e_type  bch_op,
  tdsrrcllc_nbch_op_e_type  nbch_op,
  tdsrrcllc_pch_op_e_type  pch_op
)
{
  uecomdef_status_e_type  oc_status = SUCCESS;

  /* Set the Activation Time to "Now" */
  tdsrrcllc_init_activation_time(config_ptr);

  /* Always process the drop of BCH and NBCH before processing
     the addition.  This is to ensure that num_bch always remain
     at 0 or 1.  
   */
  if(bch_op == TDSRRCLLC_DROP_BCH)
  {
    /* Set number of BCHs in MAC correctly */
    if(config_ptr->mac_dl_parms.num_bchs)
    {
      config_ptr->mac_dl_parms.num_bchs --;
    }

    /* Update number of transport channels on PCCPCH in L1 CCTrCH Parameters */
    tdsrrcllc_semi_permanent_data.dl_phy_chan.pccpch.num_trch = 0;

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;
  }
  else if(nbch_op == TDSRRCLLC_DROP_NBCH)
  {
    /* Set number of BCHs in MAC correctly */
    if(config_ptr->mac_dl_parms.num_bchs)
    {
      config_ptr->mac_dl_parms.num_bchs --;
    }

    /* Update number of transport channels on PCCPCH in L1 CCTrCH Parameters */
    tdsrrcllc_semi_permanent_data.dl_phy_chan.n_pccpch.num_trch = 0;

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;
  }

  if(bch_op == TDSRRCLLC_ADD_BCH)
  {
    /* Set number of BCHs in MAC correctly */
    if(config_ptr->mac_dl_parms.num_bchs)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Number of BCHS : %d not Zero",config_ptr->mac_dl_parms.num_bchs);
    }
    else
    {
      config_ptr->mac_dl_parms.num_bchs ++;
    }

    /* Update number of transport channels on PCCPCH in L1 CCTrCH Parameters */
    tdsrrcllc_semi_permanent_data.dl_phy_chan.pccpch.num_trch = 1;

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;
  }
  else if(nbch_op == TDSRRCLLC_ADD_NBCH)
  {
    /* Set number of BCHs in MAC correctly */
    if(config_ptr->mac_dl_parms.num_bchs)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Number of BCHS : %d not Zero",config_ptr->mac_dl_parms.num_bchs);
    }
    else
    {
      config_ptr->mac_dl_parms.num_bchs ++;
    }

    /* Update number of transport channels on PCCPCH in L1 CCTrCH Parameters */
    tdsrrcllc_semi_permanent_data.dl_phy_chan.n_pccpch.num_trch = 1;

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;
  }


  if((pch_op == TDSRRCLLC_ADD_PCH) || (pch_op == TDSRRCLLC_RECONFIG_PCH))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Init OC before updating with SIB");

    /* Initialize PCH Info */
    tdsrrcllc_init_pch_info(config_ptr);
    /* Do not delete RB Mapping Info in any other RRC state */
    tdsrrcllc_init_fach_info(config_ptr, FALSE);
    /* Initialize L1 SCCPCH Channel Info */
    tdsrrcllc_init_l1_sccpch_chan_info(config_ptr);
    /* Initialize L1 Downlink CCTrCH Info that includes Transport Format Set */
    tdsrrcllc_init_dl_cctrch_info(config_ptr);
    /* Initialize Downlink TFCS */
    tdsrrcllc_init_dl_ctfcs(config_ptr);

    oc_status = tdsrrcllc_copy_sib_to_oc(TDSRRC_STATE_DISCONNECTED);

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
#if 0
    if (tdsrrcllc_semi_permanent_data.dl_phy_chan.ctch_status == TDSRRCLLC_CTCH_ON_DIF_SCCPCH )
    {
      (void)tdsrrcllc_copy_sib_to_oc_for_sccpch_with_ctch(TDSRRC_STATE_DISCONNECTED);
    }
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
    /* In Idle Disconnected state, Ordered Config is never set */
    tdsordered_config.set_status = TDSOC_NOT_SET;
  }

  /* Return the Ordered Config status and let the calling fuinction handle it */
  return(oc_status);

}  /* tdsrrcllc_update_oc_in_disconnected */

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_fach_with_sibs()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  in Disconnected state. It is based on the PCH and BCH operation type
  that is passed as a parameter.
  It starts with copying Current Config to Ordered Config and then
  updating Ordered Config data. For a PCH add/reconfig, OC is updated
  with SIBs and in the event of a failure, the Ordered Config data is
  reverted back to Current Config data.

DEPENDENCIES:
  For a PCH add/reconfig operation, all SIBs should be read before
  calling this function.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type  tdsrrcllc_update_oc_in_fach_with_sibs
(
  tdsordered_config_type *config_ptr
)
{
  uecomdef_status_e_type  oc_status = SUCCESS;


  /* Set the Activation Time to "Now" */
  tdsrrcllc_init_activation_time(config_ptr);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Init OC before updating with SIB");

  /* Initialize PCH Info */
  tdsrrcllc_init_pch_info(config_ptr);
  /* Do not delete RB Mapping Info in any other RRC state */
  tdsrrcllc_init_fach_info(config_ptr, FALSE);
  /* Initialize L1 SCCPCH Channel Info */
  tdsrrcllc_init_l1_sccpch_chan_info(config_ptr);
  /* Initialize L1 Downlink CCTrCH Info that includes Transport Format Set */
  tdsrrcllc_init_dl_cctrch_info(config_ptr);
  /* Initialize Downlink TFCS */
  tdsrrcllc_init_dl_ctfcs(config_ptr);

  oc_status = tdsrrcllc_copy_sib_to_oc(TDSRRC_STATE_CELL_FACH);

  /* In Idle Disconnected state, Ordered Config is never set */
  //ordered_config.set_status = TDSOC_NOT_SET;

  /* Return the Ordered Config status and let the calling fuinction handle it */
  return(oc_status);

}  /* tdsrrcllc_update_oc_in_fach_with_sibs */

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_for_serving_neighbor_bch_in_connected()

DESCRIPTION:
  This function is responsible for updating the Ordered Config
  for Serving and/or Neighbor BCH in Connected mode states.
  It is based on the BCH and NBCH operation type
  that is passed as a parameter.

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type  tdsrrcllc_update_oc_for_serving_neighbor_bch_in_connected
(
  tdsordered_config_type  *config_ptr,
  tdsrrcllc_bch_op_e_type  bch_op,
  tdsrrcllc_nbch_op_e_type  nbch_op
)
{
  uecomdef_status_e_type  oc_status = SUCCESS;

  /* Set the Activation Time to "Now" */
  tdsrrcllc_init_activation_time(config_ptr);

  /* Always process the drop of BCH and NBCH before processing
     the addition.  This is to ensure that num_bch always remain
     at 0 or 1.  
   */
  if((bch_op == TDSRRCLLC_DROP_BCH)&&(tdsrrcllc_semi_permanent_data.dl_phy_chan.pccpch_is_up))
  {
    /* Set number of BCHs in MAC correctly */
    if(config_ptr->mac_dl_parms.num_bchs)
    {
      config_ptr->mac_dl_parms.num_bchs --;
    }

    /* Update number of transport channels on PCCPCH in L1 CCTrCH Parameters */
    tdsrrcllc_semi_permanent_data.dl_phy_chan.pccpch.num_trch = 0;
  }
  else if((nbch_op == TDSRRCLLC_DROP_NBCH)&&(tdsrrcllc_semi_permanent_data.dl_phy_chan.n_pccpch_is_up))
  {
    /* Set number of BCHs in MAC correctly */
    if(config_ptr->mac_dl_parms.num_bchs)
    {
      config_ptr->mac_dl_parms.num_bchs --;
    }

    /* Update number of transport channels on PCCPCH in L1 CCTrCH Parameters */
    tdsrrcllc_semi_permanent_data.dl_phy_chan.n_pccpch.num_trch = 0;

    
  }

  if(bch_op == TDSRRCLLC_ADD_BCH)
  {
    /* Set number of BCHs in MAC correctly */
    if(config_ptr->mac_dl_parms.num_bchs)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Number of BCHS : %d not Zero",config_ptr->mac_dl_parms.num_bchs);
    }
    else
    {
      config_ptr->mac_dl_parms.num_bchs ++;
    }

    /* Update number of transport channels on PCCPCH in L1 CCTrCH Parameters */
    tdsrrcllc_semi_permanent_data.dl_phy_chan.pccpch.num_trch = 1;

    
  }
  else if(nbch_op == TDSRRCLLC_ADD_NBCH)
  {
    /* Set number of BCHs in MAC correctly */
    if(config_ptr->mac_dl_parms.num_bchs)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Number of BCHS : %d not Zero",config_ptr->mac_dl_parms.num_bchs);
    }
    else
    {
      config_ptr->mac_dl_parms.num_bchs ++;
    }

    /* Update number of transport channels on PCCPCH in L1 CCTrCH Parameters */
    tdsrrcllc_semi_permanent_data.dl_phy_chan.n_pccpch.num_trch = 1;

  }


  /* Return the Ordered Config status and let the calling fuinction handle it */
  return(oc_status);
} /* tdsrrcllc_update_oc_for_serving_neighbor_bch_in_connected */

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_disconnected_to_connecting()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  mving from Disconnected to Connecting state.
  It starts with initializing Ordered Config and then
  updating Ordered Config data with SIBs
  In the event of a failure, the Ordered Config data is
  reverted back to Current Config data and a FAILURE status is returned.

DEPENDENCIES:
  All SIBs should be read before calling this function.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_from_disconnected_to_connecting(void)
{
  uecomdef_status_e_type  oc_status = SUCCESS;

  /* Since changing a state, start with initialized Ordered Config */
  tdsrrcllc_init_ordered_config_status_and_data(TDSRRC_STATE_CONNECTING);


  tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);


  oc_status = tdsrrcllc_copy_sib_to_oc(TDSRRC_STATE_CONNECTING);

  /* Restore BCH info from current to ordered config
   */
  if(oc_status ==  SUCCESS)
  {
    tdsrrcllc_copy_bch_info_from_cc_to_oc();
  }
  else
  {
    tdsrrcllc_discard_oc();
  }

#if 0 
  if ((tdsrrcllc_cmd_under_process.next_rrc_state == TDSRRC_STATE_CONNECTING)&&(oc_status ==  SUCCESS))
  {
    if (tdsrrc_check_L1_accumulated_rscp() <= -850)
    {
      if(tdsordered_config_ptr != NULL)
      {
         TDSRRC_MSG3(MSG_LEGACY_HIGH,"RSCP<=-85dbm. MMAX from NW: %d, New MMAX value: %d",tdsordered_config_ptr->mac_ul_parms.rach_tx_cntl_info.mmax, TDSRRCLLC_MAX_MMAX);
         tdsordered_config_ptr->mac_ul_parms.rach_tx_cntl_info.mmax = TDSRRCLLC_MAX_MMAX;
      }
    }
  }
#endif

  /* Return the Ordered Config status and let the calling fuinction handle it */
  return(oc_status);

}  /* tdsrrcllc_update_oc_from_disconnected_to_connecting */

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_disconnected_to_dch()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  moving from Disconnected to Cell DCH state.
  It starts with initializing Ordered Config and then
  updating Ordered Config data with message IEs.
  In the event of a failure, the Ordered Config data is
  reverted back to Current Config data and a FAILURE status is returned.

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_from_disconnected_to_dch(
  uint32            dl_sdu_num,
  void             *msg_ptr
)
{
  uint8 idx = 0;
  /* Since changing a state, start with initialized Ordered Config */
  tdsrrcllc_init_ordered_config_status_and_data(TDSRRC_STATE_CELL_DCH);

  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY );

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"set oc from_disconnected_to_dch, init add_queues_bitmask");
  for (idx = 0; idx < UE_MAX_MAC_D_FLOW; idx++)
  {
    tdsrrcllc_semi_permanent_data.mac_dflow_info[idx].add_queues_bitmask = 0;
  }

  /* Update with new IEs received in msg */
  if(tdsrrcllc_copy_msg_ie_to_oc(dl_sdu_num, msg_ptr) == SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG updated for CELL_DCH");

    /* Ordered Config now set */
    tdsordered_config.set_status = TDSOC_SET_FOR_CELL_DCH;

    return (TDSORDERED_CONFIG_SET);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG update failed for CELL_DCH");

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;

    tdsrrcllc_discard_oc();

    return (TDSORDERED_CONFIG_CONFIGURATION_INVALID);
  }
} /* tdsrrcllc_update_oc_from_disconnected_to_dch */
#endif /* FEATURE_INTERRAT_HANDOVER_G2TD */

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_connecting_to_dch()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  moving from Connecting to Cell_DCH state.
  It starts with initializing Ordered Config and then
  updating Ordered Config data with Message IEs.
  In the event of a failure, the Ordered Config data is
  reverted back to Current Config data and a NOT_SUPPORTED
  status is returned.
  On successful updation, tdsordered_config.set_status is set to TRUE.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_from_connecting_to_dch(
  uint32            dl_sdu_num,
  void             *msg_ptr
)
{
  uint8 idx = 0;
  /* Since changing a state, start with initialized Ordered Config */
  tdsrrcllc_init_ordered_config_status_and_data(TDSRRC_STATE_CELL_DCH);


  tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"set oc from_connecting_to_dch");

  /* Check if Serving BCCH is up */
  if( (tdsrrclcm_check_dl_rlc_lc_id( UE_LOGCHAN_BCCH,
                                  BCCH_S_RADIO_BEARER_ID,
                                  UE_MODE_TRANSPARENT ))
       != TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
  {
    /* S-BCCH is up. Increment the number of DL TM channels by 1 */
    TDSRRC_MSG0(MSG_LEGACY_MED,"S-BCCH up for the cell");
    tdsordered_config_ptr->rlc_dl_tm_parms.nchan++;
  }

  /* Check if Neighbor BCCH is up */
  if( (tdsrrclcm_check_dl_rlc_lc_id( UE_LOGCHAN_BCCH,
                                  BCCH_N_RADIO_BEARER_ID,
                                  UE_MODE_TRANSPARENT ))
       != TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
  {
    /* N-BCCH is up. Increment the number of DL TM channels by 1 */
    TDSRRC_MSG0(MSG_LEGACY_MED,"N-BCCH up");
    tdsordered_config_ptr->rlc_dl_tm_parms.nchan++;
  }

  /* Check if PCCH is up */
  if( (tdsrrclcm_check_dl_rlc_lc_id( UE_LOGCHAN_PCCH,
                                  PCCH_RADIO_BEARER_ID,
                                  UE_MODE_TRANSPARENT ))
       != TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
  {
    /* PCCH is up. Increment the number of DL TM channels by 1 */
    TDSRRC_MSG0(MSG_LEGACY_MED,"PCCH up");
    tdsordered_config_ptr->rlc_dl_tm_parms.nchan++;
  }

  /* Check if CCCH is up */
  if( (tdsrrclcm_check_ul_rlc_lc_id( UE_LOGCHAN_CCCH,
                                  CCCH_RADIO_BEARER_ID,
                                  UE_MODE_TRANSPARENT ))
       != TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
  {
    /* CCCH is up. Increment the number of DL TM channels by 1 */
    TDSRRC_MSG0(MSG_LEGACY_MED,"CCCH up");
    tdsordered_config_ptr->rlc_ul_tm_parms.nchan++;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"nchan: CONN->DCH CCCH up UL TM %d after increment",
        tdsordered_config_ptr->rlc_ul_tm_parms.nchan);
  }
  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"from_connecting_to_dch, init add_queues_bitmask");
  for (idx = 0; idx < UE_MAX_MAC_D_FLOW; idx++)
  {
    tdsrrcllc_semi_permanent_data.mac_dflow_info[idx].add_queues_bitmask = 0;
  }

  /* Update with new IEs received in CCCH/DCCH msg */
  if(tdsrrcllc_copy_msg_ie_to_oc(dl_sdu_num, msg_ptr) == SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG updated for CELL_DCH");

    /* Ordered Config now set */
    tdsordered_config.set_status = TDSOC_SET_FOR_CELL_DCH;

    return (TDSORDERED_CONFIG_SET);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG update failed for CELL_DCH");

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;

    tdsrrcllc_discard_oc();

    return (TDSORDERED_CONFIG_CONFIGURATION_INVALID);
  }
} /* tdsrrcllc_update_oc_from_connecting_to_dch */

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_dch()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  in Cell_DCH state.
  It starts with copying Current Config to Ordered Config and then
  updating Ordered Config data with Message IEs.
  In the event of a failure, the Ordered Config data is
  reverted back to Current Config data and a NOT_SUPPORTED
  status is returned.
  On successful updation, tdsordered_config.set_status is set to TRUE.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_in_dch(
  uint32            dl_sdu_num,
  void             *msg_ptr
)
{
  boolean   cpid_flg;         /* CELL PARAMETER ID present flag */
  boolean   cpid_change_flg;  /* either new CELL PARAMETER ID is present and different with current */
  boolean   crnti_flg;        /* NEW C_RNTI present flag */
  boolean   hrnti_flg;        /* NEW H_RNTI present flag */
  boolean   ernti_flg;        /* NEW E_RNTI present flag */
  tdsmac_rnti_valid_e_type   temp_ul_rnti_valid;   /* if msg save failure, recover it */
  uint32                     temp_ul_urnti;
  tdsmac_rnti_valid_e_type   temp_dl_rnti_valid;   /* if msg save failure, recover it */
  uint32                     temp_dl_urnti;
  tdsrrc_hsdpa_hrnti_enum_type   temp_hrnti_status;
  boolean                        temp_hrnti_present;

  temp_dl_rnti_valid = tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid;
  temp_dl_urnti = tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti;
  temp_ul_rnti_valid = tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid;
  temp_ul_urnti = tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti;
  temp_hrnti_status = tdsordered_config_ptr->hrnti_status;
  temp_hrnti_present = tdshsdpa_msg_params.hrnti_present;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"set oc from_dch_to_dch");

  /* Start with copying Current Config to Ordered Config */
  tdsrrcllc_copy_cc_to_oc();

  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);

  /* if "Primary CCPCH Info" is included indicating a new target cell and "New C-RNTI" is not specified:
   * remove any C-RNTI from MAC
   * if "Primary CCPCH Info" is included indicating a new target cell and "New H-RNTI" is not specified:
   * remove any H-RNTI from MAC
   */
  if (SUCCESS == tdsrrcllc_get_msg_rnti_cpid_flg(dl_sdu_num, msg_ptr, &cpid_flg, 
             &cpid_change_flg, &crnti_flg, &hrnti_flg, &ernti_flg))
  {
    if ((TRUE == cpid_flg)&&(TRUE == cpid_change_flg))
    {
      /* "New C-RNTI" is not specified */
      if (FALSE == crnti_flg)
      {
        if((tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSURNTI_VALID) ||
           (tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
        {
          tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
          tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti =
            tdscurrent_config_ptr->mac_dl_parms.rnti_info.urnti;
        }
        else
        {
          tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
        }

        if((tdscurrent_config_ptr->mac_ul_parms.rnti_info.rnti_valid == TDSURNTI_VALID) ||
           (tdscurrent_config_ptr->mac_ul_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
        {
          tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
          tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti =
            tdscurrent_config_ptr->mac_ul_parms.rnti_info.urnti;
        }
        else
        {
          tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
        }
      }
      if (FALSE == hrnti_flg)
      {
        tdsordered_config_ptr->hrnti_status = TDSHRNTI_INVALID;
        tdshsdpa_msg_params.hrnti_present = FALSE;
      }

#ifdef FEATURE_TDSCDMA_HSUPA
      if (FALSE == ernti_flg)
      {
        tdsordered_config_ptr->e_rnti_present = FALSE;
      }
#endif
    }
  }

  if(tdsrrcllc_copy_msg_ie_to_oc(dl_sdu_num, msg_ptr) == SUCCESS)
  {
    /* Copy OC back to CC. This is so as to keep MAC and L1 in
     * sync. Once the RRC-MAC-L1 interface is fixed for all
     * out-of-sync scenarios, this must go away. This is TBD
     */

    TDSRRC_MSG0(MSG_LEGACY_MED,"TDSORDERED_CONFIG updated for CELL_DCH");

    /* Ordered Config now set */
    tdsordered_config.set_status = TDSOC_SET_FOR_CELL_DCH;

    return (TDSORDERED_CONFIG_SET);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG update failed for CELL_DCH");
    tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = temp_dl_rnti_valid;
    tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti = temp_dl_urnti;
    tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = temp_ul_rnti_valid;
    tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti = temp_ul_urnti;
    tdsordered_config_ptr->hrnti_status = temp_hrnti_status;
    tdshsdpa_msg_params.hrnti_present = temp_hrnti_present;

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;
    return (TDSORDERED_CONFIG_CONFIGURATION_INVALID);
  }
}  /* tdsrrcllc_update_oc_in_dch */



/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_dch_to_fach_with_msg()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  transitioning from Cell_DCH to Cell_FACH state.
  This function updates OC with the message contents of the message
  that transitions UE from Cell_DCH to Cell_FACH.
  The updated OC is copied to TOC to be used later in Cell_FACH and
  when coming back to Cell_DCH from Cell_FACH.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_from_dch_to_fach_with_msg(
  uint32            dl_sdu_num,
  void             *msg_ptr
)
{
  /* Copy Current Config to Ordered Config */
  tdsrrcllc_copy_cc_to_oc();
  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);

  if(tdsrrcllc_copy_msg_ie_to_oc(dl_sdu_num, msg_ptr) == SUCCESS)
  {
    /* Copy OC back to CC. This is so as to keep MAC and L1 in
     * sync. Once the RRC-MAC-L1 interface is fixed for all
     * out-of-sync scenarios, this must go away. This is TBD
     */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC and TOC updated for DCH->FACH");

    /* Ordered Config now set */
    tdsordered_config.set_status = TDSOC_SET_FOR_DCH_FACH_TRANS;

    /* Now copy OC to TOC */
    tdsrrcllc_copy_oc_to_toc();
    /* Inducate that TOC could be used both for DCH and FACH in the future */
    tdstransition_config.toc_usage = TDSTOC_FOR_DCH_AND_FACH;

    return (TDSORDERED_CONFIG_SET);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC update failed for DCH->FACH");

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;
    return (TDSORDERED_CONFIG_CONFIGURATION_INVALID);
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_dch_to_fach_with_sibs()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  transitioning from Cell_DCH to Cell_FACH state.
  This function reads the SIBs and updates OC with SIB info.
  The BCH info is preserved from the CC to OC.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_from_dch_to_fach_with_sibs(void)
{
  uecomdef_status_e_type  oc_status = SUCCESS;

  /* Start with copying CC to OC */
  tdsrrcllc_copy_cc_to_oc();

  /* Now copy RNTI Info from TOC to OC */
  tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
  tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti =
    tdstransition_config.toc_ptr->mac_dl_parms.rnti_info.urnti;

  tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
  tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti =
    tdstransition_config.toc_ptr->mac_dl_parms.rnti_info.urnti;

  oc_status = tdsrrcllc_copy_sib_to_oc(TDSRRC_STATE_CELL_FACH);

  /* Restore BCH info from current to ordered config
   */
  if(oc_status ==  SUCCESS)
  {
    tdsrrcllc_copy_bch_info_from_cc_to_oc();
  }

  if(oc_status == SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update OC with SIBs DCH->FACH");
    /* Update OC with next state as CELL_FACH */
    tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_FACH;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure updating OC with SIBs DCH->FACH");
  }

  /* Return the Ordered Config status and let the calling fuinction handle it */
  return(oc_status);
} /* tdsrrcllc_update_oc_from_dch_to_fach_with_sibs */




/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_pch_to_fach()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  transitioning from Cell_PCH to Cell_FACH state.
  This function reads the SIBs and updates OC with SIB info.
  The BCH info is preserved from the CC to OC.

DEPENDENCIES:
  None

RETURN VALUE:
  uecomdef_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_from_pch_to_fach(void)
{
  uecomdef_status_e_type  oc_status = SUCCESS;
  int i, j;

  /* Start with copying CC to OC */
  tdsrrcllc_copy_cc_to_oc();


  tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);




  /* Now copy RNTI Info from TOC to OC */
  tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
  /*TOC may not have valid RNTI now*/
  /*
  tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti =
    tdstransition_config.toc_ptr->mac_dl_parms.rnti_info.urnti;
    */

  tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
  /*
  tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti =
    tdstransition_config.toc_ptr->mac_dl_parms.rnti_info.urnti;
    */
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"After update the rnti valid flag:");
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"PCH->FACH OC UL rnti info: valid=%d, u-rnti=0x%X, c-rnti=0x%X", 
              tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid, tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti,
              tdsordered_config_ptr->mac_ul_parms.rnti_info.crnti);
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"PCH->FACH OC DL rnti info: valid=%d, u-rnti=0x%X, c-rnti=0x%X", 
              tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid, tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti,
              tdsordered_config_ptr->mac_dl_parms.rnti_info.crnti);

  /* Initialize PCH Info */
  tdsrrcllc_init_pch_info(tdsordered_config_ptr);
  /* Do not delete RB Mapping Info in any other RRC state */
  tdsrrcllc_init_fach_info(tdsordered_config_ptr, FALSE);
  /* Initialize L1 SCCPCH Channel Info */
  tdsrrcllc_init_l1_sccpch_chan_info(tdsordered_config_ptr);
  /* Initialize L1 Downlink CCTrCH Info that includes Transport Format Set */
  tdsrrcllc_init_dl_cctrch_info(tdsordered_config_ptr);
  /* Initialize Downlink TFCS */
  tdsrrcllc_init_dl_ctfcs(tdsordered_config_ptr);

  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);


  /* we need to copy SIB info again because of the SCCPCH selection
  based on fach */
  oc_status = tdsrrcllc_copy_sib_to_oc(TDSRRC_STATE_CELL_FACH);

  /* Restore BCH info from current to ordered config
   */
  if(oc_status ==  SUCCESS)
  {
    if(tdsordered_config_ptr->mac_dl_parms.num_fachs > 1)
    {
      for(i = 0; i < tdsordered_config_ptr->mac_dl_parms.num_fachs; i ++)
      {
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->ccch_enable =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->ccch_enable;
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->ccch_id =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->ccch_id;
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->ndlchan =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->ndlchan;

        for(j = 0; j < MAX_DED_LOGCHAN_PER_TRCH; j ++)
        {
          tdsordered_config_ptr->mac_dl_parms.fach_info[i]->dlchan_info[j] =
            tdsordered_config_ptr->mac_dl_parms.fach_info[0]->dlchan_info[j];
        }
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->bcch_enable =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->bcch_enable;
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->bcch_id =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->bcch_id;
      }
    } /* If number of FACHs > 1 */
    tdsrrcllc_copy_bch_info_from_cc_to_oc();
  }

  if(oc_status == SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update OC with SIBs PCH->FACH");
    /* Update OC with next state as CELL_FACH */
    tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_FACH;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure updating OC with SIBs PCH->FACH");
  }

  /* Return the Ordered Config status and let the calling fuinction handle it */
  return(oc_status);
} /* rrcllc_update_oc_from_pch_to_fach_with_sibs */


/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_connecting_to_fach()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  going from Connecting to Cell_FACH state.
  It starts with initializing Ordered Config and then
  updating Ordered Config data with Message IEs.
  In the event of a failure, the Ordered Config data is
  reverted back to Current Config data and a NOT_SUPPORTED
  status is returned.
  On successful updation, tdsordered_config.set_status is set to TRUE.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_from_connecting_to_fach(
  uint32            dl_sdu_num,
  void             *msg_ptr
)
{
  tdsrrcllc_oc_status_e_type  oc_status;

  /* Since changing a state, start with initialized Ordered Config */
  tdsrrcllc_init_ordered_config_status_and_data(TDSRRC_STATE_CELL_FACH);
  /* Restore BCH info from current to ordered config */
  tdsrrcllc_copy_bch_info_from_cc_to_oc();

  /* Update OC with SIB Info */
  if(tdsrrcllc_copy_sib_to_oc(TDSRRC_STATE_CELL_FACH) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG update failed for CELL_FACH");

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;
    tdsrrcllc_discard_oc();

    oc_status = TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED;
  }
  else /* Successfully updated with SIBs */
  {
    tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);

    /* Update with new IEs received in CCCH/DCCH msg */
    if(tdsrrcllc_copy_msg_ie_to_oc(dl_sdu_num, msg_ptr) == SUCCESS)
    {
      /* Restore the CCCH info from Current Config to Ordered Config */
      tdsrrcllc_copy_ccch_info_from_cc_to_oc();


      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG updated for CELL_FACH");

      /* Ordered Config now set */
      tdsordered_config.set_status = TDSOC_SET_FOR_CELL_FACH;

      oc_status = TDSORDERED_CONFIG_SET;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG update failed for CELL_FACH");

      /* Ordered Config NOT set */
      tdsordered_config.set_status = TDSOC_NOT_SET;
      oc_status = TDSORDERED_CONFIG_CONFIGURATION_INVALID;
    }
  }

  return(oc_status);
} /* tdsrrcllc_update_oc_from_connecting_to_fach */

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_dch_to_pch_with_msg()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  transitioning from Cell_DCH to Cell_PCH or URA_PCH state.
  This function updates OC with the message contents of the message
  that transitions UE from Cell_DCH to Cell_PCH or URA_PCH.
  The updated OC is copied to TOC to be used later when coming back to
  Cell_FACH from Cell_PCH or URA_PCH.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_from_dch_to_pch_with_msg(
  uint32            dl_sdu_num,
  void             *msg_ptr,
  tdsrrc_state_e_type  next_rrc_state
)
{
  /* Copy Current Config to Ordered Config */
  tdsrrcllc_copy_cc_to_oc();
  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);
  
  /* Copy U-RNTI from Current Config to Ordered Config. Note that C-RNTI
   * is not applicable in CELL_PCH, and so is not copied.
   */
  if((tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSURNTI_VALID) ||
     (tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
  {
    tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
    tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti =
      tdscurrent_config_ptr->mac_dl_parms.rnti_info.urnti;
  }
  else
  {
    tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No URNTI DCH->PCH");
  }

  if((tdscurrent_config_ptr->mac_ul_parms.rnti_info.rnti_valid == TDSURNTI_VALID) ||
     (tdscurrent_config_ptr->mac_ul_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
  {
    tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
    tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti =
      tdscurrent_config_ptr->mac_ul_parms.rnti_info.urnti;
  }
  else
  {
    tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No URNTI DCH->PCH");
  }
  if(tdsrrcllc_copy_msg_ie_to_oc(dl_sdu_num, msg_ptr) == SUCCESS)
  {
    /* Copy OC back to CC. This is so as to keep MAC and L1 in
     * sync. Once the RRC-MAC-L1 interface is fixed for all
     * out-of-sync scenarios, this must go away. This is TBD
     */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC and TOC updated for DCH->PCH");

    /* Ordered Config now set */
    switch(next_rrc_state)
    {
      case TDSRRC_STATE_CELL_PCH:
        tdsordered_config.set_status = TDSOC_SET_FOR_DCH_CELL_PCH_TRANS;
        break;
      case TDSRRC_STATE_URA_PCH:
        tdsordered_config.set_status = TDSOC_SET_FOR_DCH_URA_PCH_TRANS;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid state %d for DCH->PCH OC update", next_rrc_state);
        tdsordered_config.set_status = TDSOC_NOT_SET;
        return TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED;
    }

    /* Now copy OC to TOC */
    tdsrrcllc_copy_oc_to_toc();
    /* Indicate that TOC could be used both for DCH and FACH in the future */
    tdstransition_config.toc_usage = TDSTOC_FOR_DCH_AND_FACH;

    return (TDSORDERED_CONFIG_SET);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC update failed for DCH->CELL_PCH");

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;
    return (TDSORDERED_CONFIG_CONFIGURATION_INVALID);
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_dch_to_pch_with_sibs()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  transitioning from Cell_DCH to Cell_PCH or URA_PCH state.
  This function reads the SIBs and updates OC with SIB info.
  The BCH info is preserved from the CC to OC.

DEPENDENCIES:
  None

RETURN VALUE:
  uecomdef_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_from_dch_to_pch_with_sibs(tdsrrc_state_e_type next_rrc_state)
{
  uecomdef_status_e_type  oc_status = SUCCESS;

  /* Start with copying CC to OC */
  tdsrrcllc_copy_cc_to_oc();

  /* Now copy U-RNTI Info from TOC to OC */
  tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
  tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti =
    tdstransition_config.toc_ptr->mac_dl_parms.rnti_info.urnti;

  tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
  tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti =
    tdstransition_config.toc_ptr->mac_dl_parms.rnti_info.urnti;

  oc_status = tdsrrcllc_copy_sib_to_oc(next_rrc_state);
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
#if 0
    if (tdsrrcllc_semi_permanent_data.dl_phy_chan.ctch_status == TDSRRCLLC_CTCH_ON_DIF_SCCPCH )
    {
      (void)tdsrrcllc_copy_sib_to_oc_for_sccpch_with_ctch(next_rrc_state);
    }
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
  /* Restore BCH info from current to ordered config
   */
  if(oc_status ==  SUCCESS)
  {
    tdsrrcllc_copy_bch_info_from_cc_to_oc();
  }

  if(oc_status == SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update OC with SIBs DCH->PCH");
    /* Update OC with next state. */
    tdsordered_config_ptr->rrc_state_indicator = next_rrc_state;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure updating OC with SIBs DCH->PCH");
  }

  /* Return the Ordered Config status and let the calling fuinction handle it */
  return(oc_status);
} /* tdsrrcllc_update_oc_from_dch_to_fach_with_sibs */



/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_fach_to_cell_pch_or_ura_pch()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  transitioning from Cell_FACH to Cell_PCH or URA_PCH state.
  This function reads the SIBs and updates OC with SIB info.
  The BCH info is preserved from the CC to OC.

DEPENDENCIES:
  None

RETURN VALUE:
  uecomdef_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_from_fach_to_cell_pch_or_ura_pch(tdsrrc_state_e_type next_rrc_state)
{
  int i,j;
  uecomdef_status_e_type  oc_status = SUCCESS;

  /* Initialize PCH Info */
  tdsrrcllc_init_pch_info(tdsordered_config_ptr);
  /* Do not delete RB Mapping Info in any other RRC state */
  tdsrrcllc_init_fach_info(tdsordered_config_ptr, FALSE);
  /* Initialize L1 SCCPCH Channel Info */
  tdsrrcllc_init_l1_sccpch_chan_info(tdsordered_config_ptr);
  /* Initialize L1 Downlink CCTrCH Info that includes Transport Format Set */
  tdsrrcllc_init_dl_cctrch_info(tdsordered_config_ptr);
  /* Initialize Downlink TFCS */
  tdsrrcllc_init_dl_ctfcs(tdsordered_config_ptr);

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrcllc_init_dl_ctfcs_for_sccpch_with_ctch(tdsordered_config_ptr);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT

  oc_status = tdsrrcllc_copy_sib_to_oc(next_rrc_state);

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    if (tdsrrcllc_semi_permanent_data.dl_phy_chan.ctch_status == TDSRRCLLC_CTCH_ON_DIF_SCCPCH )
    {
      (void)tdsrrcllc_copy_sib_to_oc_for_sccpch_with_ctch(next_rrc_state);
    }
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

  /* Restore BCH info from current to ordered config
   */
  if(oc_status ==  SUCCESS)
  {
    if(tdsordered_config_ptr->mac_dl_parms.num_fachs > 1)
    {
      for(i = 0; i < tdsordered_config_ptr->mac_dl_parms.num_fachs; i ++)
      {
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->ccch_enable =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->ccch_enable;
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->ccch_id =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->ccch_id;
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->ndlchan =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->ndlchan;

        for(j = 0; j < MAX_DED_LOGCHAN_PER_TRCH; j ++)
        {
          tdsordered_config_ptr->mac_dl_parms.fach_info[i]->dlchan_info[j] =
            tdsordered_config_ptr->mac_dl_parms.fach_info[0]->dlchan_info[j];
        }
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->bcch_enable =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->bcch_enable;
        tdsordered_config_ptr->mac_dl_parms.fach_info[i]->bcch_id =
          tdsordered_config_ptr->mac_dl_parms.fach_info[0]->bcch_id;
      }
    } /* If number of FACHs > 1 */
    tdsrrcllc_copy_bch_info_from_cc_to_oc();
    tdsrrcllc_copy_bch_info_from_cc_to_oc();
  }

  if(oc_status == SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update OC with SIBs FACH->PCH");
    /* Update OC with next state. */
    tdsordered_config_ptr->rrc_state_indicator = next_rrc_state;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure updating OC with SIBs FACH->PCH");
  }

  /* Return the Ordered Config status and let the calling function handle it */
  return(oc_status);
} /* tdsrrcllc_update_oc_from_fach_to_cell_pch_or_ura_pch */

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_fach_to_pch_with_msg()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  transitioning from Cell_FACH to Cell_PCH or URA_PCH state.
  It starts with copying Current Config to Ordered Config and then
  updating Ordered Config data with Message IEs.
  In the event of a failure, the Ordered Config data is
  reverted back to Current Config data and a NOT_SUPPORTED
  status is returned.
  On successful updation, tdsordered_config.set_status is set to TRUE.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_from_fach_to_pch_with_msg(
  uint32            dl_sdu_num,
  void             *msg_ptr,
  tdsrrc_state_e_type  next_rrc_state
)
{
  /* Start with copying Current Config to Ordered Config */
  tdsrrcllc_copy_cc_to_oc();
  
 /* Copy U-RNTI from Current Config to Ordered Config. Note that C-RNTI
   * is not applicable in CELL_PCH, and so is not copied.
   */
  if((tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSURNTI_VALID) ||
     (tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
  {
    tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
    tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti =
      tdscurrent_config_ptr->mac_dl_parms.rnti_info.urnti;
  }
  else
  {
    tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No URNTI FACH->PCH");
  }

  if((tdscurrent_config_ptr->mac_ul_parms.rnti_info.rnti_valid == TDSURNTI_VALID) ||
     (tdscurrent_config_ptr->mac_ul_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
  {
    tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
    tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti =
      tdscurrent_config_ptr->mac_ul_parms.rnti_info.urnti;
  }
  else
  {
    tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No URNTI FACH->PCH");
  }

  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);


  if(tdsrrcllc_copy_msg_ie_to_oc(dl_sdu_num, msg_ptr) == SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG updated for CELL_FACH->PCH");

    /* Ordered Config now set */
    switch(next_rrc_state)
    {
      case TDSRRC_STATE_CELL_PCH:
        tdsordered_config.set_status = TDSOC_SET_FOR_FACH_CELL_PCH_TRANS;
        break;
      case TDSRRC_STATE_URA_PCH:
        tdsordered_config.set_status = TDSOC_SET_FOR_FACH_URA_PCH_TRANS;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid state %d for FACH->PCH OC update", next_rrc_state);
        tdsordered_config.set_status = TDSOC_NOT_SET;
        return TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED;
    }
    /* Now copy OC to TOC */
   // tdsrrcllc_copy_oc_to_toc();
    /* Indicate that TOC could be used both for DCH and FACH in the future */
    //transition_config.toc_usage = TDSTOC_FOR_DCH_AND_FACH;

    return (TDSORDERED_CONFIG_SET);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG update failed for CELL_FACH->PCH");

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;

    return (TDSORDERED_CONFIG_CONFIGURATION_INVALID);
  }
}  /* tdsrrcllc_update_oc_from_fach_to_pch_with_msg */




/*====================================================================
FUNCTION: tdsrrcllc_copy_harq_info()

DESCRIPTION:
  This function is responsible for copying all HARQ related data from
  source to destination config.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_copy_harq_info
(
  tdsordered_config_type *dest_config,
  tdsordered_config_type *src_config
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Copy HS-DSCH Info");
  dest_config->l1_hsdpa_info.hs_dsch_info.harq_info =
    src_config->l1_hsdpa_info.hs_dsch_info.harq_info;

}





#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_copy_srb5_info()

DESCRIPTION:
  This function is responsible for copying all SRB#5 related data from
  source to destination config.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllc_copy_srb5_info
(
  tdsordered_config_type *dest_config,
  tdsordered_config_type *src_config
)
{
  memscpy ((void *)&(dest_config->srb5_info),sizeof(tdsrrcllc_srb5_info_type), (void *)&(src_config->srb5_info), 
           sizeof(tdsrrcllc_srb5_info_type));
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Copied SRB#5 data from TOC  to OC");
} /* tdsrrcllc_copy_srb5_info */
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

/*====================================================================
FUNCTION: tdsrrcllc_copy_trch_info()

DESCRIPTION:
  This function is responsible for copying all TrCH related data from
  source to destination config.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
static void tdsrrcllc_copy_trch_info
(
  tdsordered_config_type *dest_config,
  tdsordered_config_type *src_config
)
{
  int i, j;
  /* Copy all L1 data */
  dest_config->l1_dl_cctrch_parms = src_config->l1_dl_cctrch_parms;
  dest_config->l1_dl_ctfc_parms = src_config->l1_dl_ctfc_parms;
  dest_config->l1_ul_cctrch_parms = src_config->l1_ul_cctrch_parms;

  /* Copy MAC data */
  dest_config->mac_dl_parms = src_config->mac_dl_parms;
  dest_config->mac_ul_parms = src_config->mac_ul_parms;
  dest_config->rrc_ul_cctrch_params = src_config->rrc_ul_cctrch_params;


  /* Copy local data structures that may be pointed to by L1/MAC structures */
  for(i=0; i<TDSUE_MAX_TFC; i++)
  {
    dest_config->ctfc_info[i] = src_config->ctfc_info[i];
  }

  dest_config->dl_ctfc = src_config->dl_ctfc;

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
  {
    for(i=0; i<UE_MAX_MAC_D_FLOW; i++)
    {
      dest_config->mac_dflow_info[i] = src_config->mac_dflow_info[i];
      dest_config->dl_dflow_index_in_use[i] = src_config->dl_dflow_index_in_use[i];
    }
  }

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  for(i=0; i< UE_MAX_HS_QUEUE; i++)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"QUEUE_ID %d",src_config->mac_ehs_queue_info[i].queue_id);
    dest_config->mac_ehs_queue_info[i] = src_config->mac_ehs_queue_info[i];
    dest_config->mac_ehs_queue_index_in_use[i] = src_config->mac_ehs_queue_index_in_use[i];
  }
  
  for(i=0; i< TDSUE_MAX_DL_LOGICAL_CHANNEL; i++)
  {
    dest_config->dl_macehs_logchan_info[i] = src_config->dl_macehs_logchan_info[i];
    dest_config->mac_ehs_lc_id_in_use[i] = src_config->mac_ehs_lc_id_in_use[i];
  }
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/


#ifdef FEATURE_TDSCDMA_HSUPA
  dest_config->mac_e_config = src_config->mac_e_config;
  dest_config->l1_e_info.e_ul_info.harq_rv_config = 
   src_config->l1_e_info.e_ul_info.harq_rv_config;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL:harq_rv_config=%d",
    src_config->l1_e_info.e_ul_info.harq_rv_config);

#endif /* FEATURE_TDSCDMA_HSUPA */

  for(i=0; i<TDSUE_MAX_TRCH; i++)
  {
    dest_config->dl_dch_info[i] = src_config->dl_dch_info[i];
    dest_config->dl_dch_trch_idx_in_use[i] = src_config->dl_dch_trch_idx_in_use[i];
    dest_config->dl_dch_trch_info[i] = src_config->dl_dch_trch_info[i];
    for(j=0; j<TDSUE_MAX_TF; j++)
    {
      dest_config->dl_tf_info[i][j] = src_config->dl_tf_info[i][j];
    }
    dest_config->dl_trch_info[i] = src_config->dl_trch_info[i];
    dest_config->rrc_ul_cctrch_params.semi_static_parms[i] = 
            src_config->rrc_ul_cctrch_params.semi_static_parms[i];
    dest_config->ul_dch_trch_idx_in_use[i] = src_config->ul_dch_trch_idx_in_use[i];
    dest_config->ul_dch_trch_info[i] = src_config->ul_dch_trch_info[i];
  }

  dest_config->dl_trch_cnt = src_config->dl_trch_cnt;
  dest_config->ul_trch_cnt = src_config->ul_trch_cnt;

  dest_config->ul_cctrch = src_config->ul_cctrch;
  dest_config->ul_tfcs = src_config->ul_tfcs;


  for(i=0; i<TDSL1_UL_TFC_MAX; i++)
  {
    dest_config->rrc_ul_cctrch_params.gain_factor_parms[i] =
            src_config->rrc_ul_cctrch_params.gain_factor_parms[i];
  }

  dest_config->reconfig_needed.downlink_l1 = TRUE;
  dest_config->reconfig_needed.uplink_l1 = TRUE;
  dest_config->reconfig_needed.downlink_mac = TRUE;
  dest_config->reconfig_needed.uplink_mac = TRUE;

  dest_config->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;

  dest_config->num_removed_tfcis = 0;

  /* Initialize all pointers in L1, MAC and RLC structures */
  tdsrrcllc_init_mac_dl_ptrs(dest_config);
  tdsrrcllc_init_mac_ul_ptrs(dest_config);
  tdsrrcllc_init_l1_dl_ptrs(dest_config);
  tdsrrcllc_init_l1_ul_ptrs(dest_config);
  tdsrrcllc_init_rlc_ptrs(dest_config);

} /* tdsrrcllc_copy_trch_info */

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_fach_to_dch()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  transitioning from Cell_FACH to Cell_DCH state.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_from_fach_to_dch(
  uint32            dl_sdu_num,
  void             *msg_ptr
)
{
  boolean cpid_flg;         /* CELL PARAMETER ID present flag */
  boolean cpid_change_flg;  /* either new CELL PARAMETER ID is present and different with current */
  boolean crnti_flg;          /* NEW C_RNTI present flag */
  boolean hrnti_flg;          /* NEW H_RNTI present flag */
  boolean ernti_flg;          /* NEW E_RNTI present flag */
  uint8 idx = 0;

  /* Initialize Ordered Config */
  tdsrrcllc_init_config_data(tdsordered_config_ptr, TDSRRC_STATE_CELL_DCH);
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"set oc from_fach_to_dch");

  /* Check if Serving BCCH is up */
  if( (tdsrrclcm_check_dl_rlc_lc_id( UE_LOGCHAN_BCCH,
                                  BCCH_S_RADIO_BEARER_ID,
                                  UE_MODE_TRANSPARENT ))
       != TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
  {
    /* S-BCCH is up. Increment the number of DL TM channelsu by 1 */
    TDSRRC_MSG0(MSG_LEGACY_MED,"S-BCCH up for the cell");
    tdsordered_config_ptr->rlc_dl_tm_parms.nchan++;
  }

  /* Check if Neighbor BCCH is up */
  if( (tdsrrclcm_check_dl_rlc_lc_id( UE_LOGCHAN_BCCH,
                                  BCCH_N_RADIO_BEARER_ID,
                                  UE_MODE_TRANSPARENT ))
       != TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
  {
    /* N-BCCH is up. Increment the number of DL TM channels by 1 */
    TDSRRC_MSG0(MSG_LEGACY_MED,"N-BCCH up");
    tdsordered_config_ptr->rlc_dl_tm_parms.nchan++;
  }

#ifdef FEATURE_UMTS_PDCP
  tdsrrcllc_copy_pdcp_parms(tdsordered_config_ptr, tdscurrent_config_ptr);
#endif /* FEATURE_UMTS_PDCP*/


  /* Copy RLC size restrictions for FACH from CC to OC */
  /* The "RLC size list" within "RLC size restrictions" should be cleared first */

  /* Copy RLC Info from Current Config */
  tdsrrcllc_copy_rlc_parms(tdsordered_config_ptr, tdscurrent_config_ptr);

  /* Copy Cell FACH RB Mapping Info from Current to Ordered Config. This
   * will be needed on subsequent transitions between CELL_DCH and CELL_FACH
   */
  tdsrrcllc_copy_cell_fach_rb_mapping_info(tdsordered_config_ptr, tdscurrent_config_ptr);

  /* copy p_freq from Current to Ordered Config.
   * If OTA msg have new freq info, will save in tdsrrcllc_copy_msg_ie_to_oc */
  tdsordered_config_ptr->p_freq = tdscurrent_config_ptr->p_freq;

  /* Copy all TrCH related data from TOC to OC if TOC is valid */
  if(tdstransition_config.toc_usage != TDSTOC_INVALID)
  {
    tdsrrcllc_copy_trch_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    tdsrrcllc_copy_srb5_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
    {
      /* Copy HARQ Information to L1 */
      tdsrrcllc_copy_harq_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);
    }
  }
  /* if "Primary CCPCH Info" is included indicating a new target cell and "New C-RNTI" is not specified:
   * remove any C-RNTI from MAC,
   */
  if (SUCCESS == tdsrrcllc_get_msg_rnti_cpid_flg(dl_sdu_num, msg_ptr, &cpid_flg,
                &cpid_change_flg, &crnti_flg, &hrnti_flg, &ernti_flg))
  {
    if ((TRUE == cpid_flg)&&(TRUE == cpid_change_flg)&&(FALSE == crnti_flg))
    {
      /* Copy U-RNTI from Current Config to Ordered Config. Note that C-RNTI
       * is not applicable in CELL_DCH, and so is not copied.
       */
      if((tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSURNTI_VALID) ||
         (tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
      {
        tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
        tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti =
          tdscurrent_config_ptr->mac_dl_parms.rnti_info.urnti;
      }
      else
      {
        tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No URNTI FACH->DCH");
      }

      if((tdscurrent_config_ptr->mac_ul_parms.rnti_info.rnti_valid == TDSURNTI_VALID) ||
         (tdscurrent_config_ptr->mac_ul_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
      {
        tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSURNTI_VALID;
        tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti =
          tdscurrent_config_ptr->mac_ul_parms.rnti_info.urnti;
      }
      else
      {
        tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = TDSNO_RNTI_VALID;
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No URNTI FACH->DCH");
      }
    }
    /* not fulfil remove C_RNTI condition */
    else
    {
      /* Copy RNTI from Current Config to Ordered Config. */
      tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid = 
          tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid;
      tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti =
          tdscurrent_config_ptr->mac_dl_parms.rnti_info.urnti;

      tdsordered_config_ptr->mac_ul_parms.rnti_info.rnti_valid = 
          tdscurrent_config_ptr->mac_ul_parms.rnti_info.rnti_valid;
      tdsordered_config_ptr->mac_ul_parms.rnti_info.urnti =
          tdscurrent_config_ptr->mac_ul_parms.rnti_info.urnti;
    }
  }

  /* Copy RLC size restrictions for DCH from TOC to OC
   * if TOC is not invalid
   */
  /* Copy RLC Size Info list from Current Config - Current
   * Config has the latest.
   */
  tdsrrcllc_copy_cell_fach_rlc_size_list(tdsordered_config_ptr,
                                      tdscurrent_config_ptr);


  /* Now Ordered Config is in a state where it can be updated
   * with Message Contents.
   */
  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"from fach_to_dch, init add_queues_bitmask");
  for (idx = 0; idx < UE_MAX_MAC_D_FLOW; idx++)
  {
    tdsrrcllc_semi_permanent_data.mac_dflow_info[idx].add_queues_bitmask = 0;
  }

  if(tdsrrcllc_copy_msg_ie_to_oc(dl_sdu_num, msg_ptr) == SUCCESS)
  {
    /* Copy OC back to CC. This is so as to keep MAC and L1 in
     * sync. Once the RRC-MAC-L1 interface is fixed for all
     * out-of-sync scenarios, this must go away. This is TBD
     */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC updated for FACH->DCH");

    /* Ordered Config now set */
    tdsordered_config.set_status = TDSOC_SET_FOR_CELL_DCH;


    return (TDSORDERED_CONFIG_SET);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"OC update failed for FACH->DCH");

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;

    return (TDSORDERED_CONFIG_CONFIGURATION_INVALID);
  }
}


/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_fach_with_msg()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  in Cell_FACH state.
  It starts with copying Current Config to Ordered Config and then
  updating Ordered Config data with Message IEs.
  In the event of a failure, the Ordered Config data is
  reverted back to Current Config data and a NOT_SUPPORTED
  status is returned.
  On successful updation, tdsordered_config.set_status is set to TRUE.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_in_fach_with_msg(
  uint32            dl_sdu_num,
  void             *msg_ptr
)
{
  /* Start with copying Current Config to Ordered Config */
  tdsrrcllc_copy_cc_to_oc();

  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);

  if(tdsrrcllc_copy_msg_ie_to_oc(dl_sdu_num, msg_ptr) == SUCCESS)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG updated for CELL_FACH");

    /* Ordered Config now set */
    tdsordered_config.set_status = TDSOC_SET_FOR_CELL_FACH;

    return (TDSORDERED_CONFIG_SET);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSORDERED_CONFIG update failed for CELL_FACH");

    /* Ordered Config NOT set */
    tdsordered_config.set_status = TDSOC_NOT_SET;

    return (TDSORDERED_CONFIG_CONFIGURATION_INVALID);
  }
}  /* tdsrrcllc_update_oc_in_fach_with_msg */

#ifdef FEATURE_UMTS_PDCP

/*====================================================================
FUNCTION: tdsrrcllc_copy_pdcp_parms()

DESCRIPTION:
  This function is responsible for copying the PDCP
  parms from Source to Destination Config data.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_copy_pdcp_parms
(
  tdsordered_config_type  *dest_config,
  tdsordered_config_type  *src_config
)
{
  dest_config->pdcp_parms = src_config->pdcp_parms;
}
#endif /* FEATURE_UMTS_PDCP */



/*====================================================================
FUNCTION: tdsrrcllc_copy_rlc_parms()

DESCRIPTION:
  This function is responsible for copying the RLC AM, UM and TM
  parms from Source to Destination Config data.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_copy_rlc_parms(
  tdsordered_config_type  *dest_config,
  tdsordered_config_type  *src_config
  )
{
  uint32  i;
  /* Copy RLC AM Parameter Info from Source to Destination */
  dest_config->rlc_am_parms = src_config->rlc_am_parms;
  memscpy(dest_config->rb_am_rlc_size_ota,(sizeof(tdsrrc_rb_dl_am_rlc_size_info))*TDSUE_MAX_AM_ENTITY,
                src_config->rb_am_rlc_size_ota,
                (sizeof(tdsrrc_rb_dl_am_rlc_size_info))*TDSUE_MAX_AM_ENTITY);
  
  for(i = 0; i < TDSUE_MAX_AM_ENTITY; i ++)
  {
    /* Copy AM Config Info from Source to Destination */
    dest_config->am_config[i] = src_config->am_config[i];
    /* Re-init destination AM Config pointers */
    dest_config->rlc_am_parms.am_info[i] = &dest_config->am_config[i];
  }

  /* Copy DL TM Config Info */
  dest_config->rlc_dl_tm_parms = src_config->rlc_dl_tm_parms;
  /* Copy DL UM Config Info */
  dest_config->rlc_dl_um_parms = src_config->rlc_dl_um_parms;
  /* Copy UL TM Config Info */
  dest_config->rlc_ul_tm_parms = src_config->rlc_ul_tm_parms;
  /* Copy UL UM Config Info */
  dest_config->rlc_ul_um_parms = src_config->rlc_ul_um_parms;

  tdsrrcllc_init_rlc_ptrs(dest_config);
  return;
} /* tdsrrcllc_copy_rlc_parms */

/*====================================================================
FUNCTION: tdsrrcllc_copy_cell_fach_rb_mapping_info()

DESCRIPTION:
  This function is responsible for copying all RB Mapping Info stored
  for CELL_FACH state in the Source Config, to the RB Mapping Info for
  FACH and RACH in the Destination Config.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_copy_cell_fach_rb_mapping_info(
  tdsordered_config_type  *dest_config,
  tdsordered_config_type  *src_config
  )
{
  uint32  i;
  uint32  j;

  /* Copy for all FACHes */
  for( i = 0; i < TDSUE_MAX_TRCH; i ++)
  {
    /* Copy number of DL DEdicated logical channels on FACH */
    dest_config->fach_info[i].ndlchan = src_config->cell_fach_rb_mapping_info.dl_ndlchan;
    for ( j = 0; j < MAX_DED_LOGCHAN_PER_TRCH; j ++ )
    {
      /* Copy mapping info for DL dedicated logical channels */
      dest_config->fach_info[i].dlchan_info[j] =
        src_config->cell_fach_rb_mapping_info.dl_dlchan_info[j];
    }
  }

  /* Copy number of UL Dedicated logical channels on RACH */
  dest_config->mac_ul_parms.cctrch_ptr->rach_info.ndlchan =
    src_config->cell_fach_rb_mapping_info.ul_ndlchan;

  /* Copy mapping info for UL dedicated logical channels */
  for ( j = 0; j < MAX_DED_LOGCHAN_PER_TRCH; j ++ )
  {
    dest_config->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[j] =
      src_config->cell_fach_rb_mapping_info.ul_dlchan_info[j];
  }

  /* Finally copy Cell FACH RB Mapping INfo from Source to Destination */
  dest_config->cell_fach_rb_mapping_info = src_config->cell_fach_rb_mapping_info;

} /* tdsrrcllc_copy_cell_fach_rb_mapping_info */

/*====================================================================
FUNCTION: tdsrrcllc_copy_cell_fach_rlc_size_list()

DESCRIPTION:
  This function is responsible for copying all RLC Size Restriction Info
  stored for CELL_FACH state in the Source Config, to the RLC Size
  Restriction Info for FACH and RACH in the Destination Config.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_copy_cell_fach_rlc_size_list(
  tdsordered_config_type  *dest_config,
  tdsordered_config_type  *src_config
  )
{
  memscpy(dest_config->rach_rlc_size_restriction_info.rlc_size_list,sizeof(tdsrrcllcoc_rlc_size_list_type)*MAX_DED_LOGCHAN_PER_TRCH,
         src_config->rach_rlc_size_restriction_info.rlc_size_list,
         sizeof(tdsrrcllcoc_rlc_size_list_type)*MAX_DED_LOGCHAN_PER_TRCH);
} /* tdsrrcllc_copy_cell_fach_rlc_size_list */



/*====================================================================
FUNCTION: tdsrrcllc_update_oc_for_pch_to_fach_with_toc()

DESCRIPTION:
  This function is responsible for updating OC with TOC parameters
  immediately after transitioning to CELL_FACH.
  The CC is copied to OC and then OC is updated with RLC and RB Mapping
  Info.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_oc_for_pch_to_fach_with_toc(
  void
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"update_oc_for_pch_to_fach_with_toc toc_usage");
  ///* Copy all RLC Config data for AM channels */
  //rrcllc_copy_rlc_parms(tdsordered_config_ptr, tdstransition_config.toc_ptr);
  /* Increment DL UM and UL TM Nchans to accomodate the already
   * existing PCCH.
   */
 // tdsordered_config_ptr->rlc_dl_tm_parms.nchan ++;

  /* Re-map all channels previously mapped on DCHs to FACHs and PRACH */
  //rrcllc_copy_cell_fach_rb_mapping_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);


   tdsordered_config_ptr->rach_rlc_size_restriction_info.mac_update_needed = TRUE;
  /* Copy over the RB-oriented side of RLC size restrictions */
  tdsrrcllc_copy_cell_fach_rlc_size_list(tdsordered_config_ptr, tdstransition_config.toc_ptr);

  /* Update the MAC layer with the newly copied RLC size restriction info */
  (void)tdsrrcllc_populate_mac_rlc_size_restriction_info(tdsordered_config_ptr, TDSRRC_STATE_CELL_FACH);


  TDSRRC_MSG0(MSG_LEGACY_HIGH,"update_oc_for_pch_to_fach_with_toc");
} /* tdsrrcllc_update_oc_in_fach_with_toc */


/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_fach_with_toc()

DESCRIPTION:
  This function is responsible for updating OC with TOC parameters
  immediately after transitioning to CELL_FACH.
  The CC is copied to OC and then OC is updated with RLC and RB Mapping
  Info.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_oc_in_fach_with_toc(
  void
)
{
  /* Start with copying CC to OC */
  tdsrrcllc_copy_cc_to_oc();

#ifdef FEATURE_UMTS_PDCP
  /* Copy all pdcp info into TOC */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Printing PDCP info from TOC");
  tdsrrcllc_print_pdcp_info(tdstransition_config.toc_ptr);
  tdsrrcllc_copy_pdcp_parms(tdsordered_config_ptr, tdstransition_config.toc_ptr);
#endif /* FEATURE_UMTS_PDCP*/


  /* Copy all RLC Config data for AM channels */
  tdsrrcllc_copy_rlc_parms(tdsordered_config_ptr, tdstransition_config.toc_ptr);

  /* Re-map all channels previously mapped on DCHs to FACHs and PRACH */
  tdsrrcllc_copy_cell_fach_rb_mapping_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);

  /* Also copy the RNTI Info from TOC to OC */
  tdsordered_config_ptr->mac_ul_parms.rnti_info =
    tdstransition_config.toc_ptr->mac_ul_parms.rnti_info;
  tdsordered_config_ptr->mac_dl_parms.rnti_info =
    tdstransition_config.toc_ptr->mac_dl_parms.rnti_info;

  if ((tdstransition_config.toc_usage == TDSTOC_FOR_OOS)
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITHOUT_DCH_INFO)
      || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITH_DCH_INFO)
      ||(tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH  )
      ||(tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH_WITH_DCH_INFO )
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  ) 
  {
    tdsordered_config_ptr->rach_rlc_size_restriction_info.mac_update_needed = TRUE;
  }
  /* Copy over the RB-oriented side of RLC size restrictions */
  tdsrrcllc_copy_cell_fach_rlc_size_list(tdsordered_config_ptr, tdstransition_config.toc_ptr);

  /* Update the MAC layer with the newly copied RLC size restriction info */
  (void)tdsrrcllc_populate_mac_rlc_size_restriction_info(tdsordered_config_ptr, TDSRRC_STATE_CELL_FACH);


  TDSRRC_MSG0(MSG_LEGACY_HIGH,"update_oc_in_fach_with_toc");
} /* tdsrrcllc_update_oc_in_fach_with_toc */



/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_pch_with_toc()

DESCRIPTION:
  This function is responsible for updating OC with TOC parameters
  immediately after transitioning to CELL_PCH or URA_PCH.
  The CC is copied to OC and then OC is updated with RLC info.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_oc_in_pch_with_toc(
  void
)
{
  /* Start with copying CC to OC */
  tdsrrcllc_copy_cc_to_oc();

#ifdef FEATURE_UMTS_PDCP
  /* Copy all pdcp info into TOC */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Printing PDCP info from TOC");
  tdsrrcllc_print_pdcp_info(tdstransition_config.toc_ptr);
  tdsrrcllc_copy_pdcp_parms(tdsordered_config_ptr, tdstransition_config.toc_ptr);
#endif /* FEATURE_UMTS_PDCP*/


  /* Copy all RLC Config data for AM channels */
  tdsrrcllc_copy_rlc_parms(tdsordered_config_ptr, tdstransition_config.toc_ptr);
  /* Increment DL UM and UL TM Nchans to accomodate the already
   * existing PCCH.
   */
  tdsordered_config_ptr->rlc_dl_tm_parms.nchan ++;

  /* Re-map all channels previously mapped on DCHs to FACHs and PRACH */
  tdsrrcllc_copy_cell_fach_rb_mapping_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);

  /* Also copy the RNTI Info from TOC to OC */
  tdsordered_config_ptr->mac_ul_parms.rnti_info =
    tdstransition_config.toc_ptr->mac_ul_parms.rnti_info;
  tdsordered_config_ptr->mac_dl_parms.rnti_info =
    tdstransition_config.toc_ptr->mac_dl_parms.rnti_info;

  if ((tdstransition_config.toc_usage == TDSTOC_FOR_OOS)
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITHOUT_DCH_INFO)
      || (tdstransition_config.toc_usage == TDSTOC_FOR_OOS_WITH_DCH_INFO)
      || (tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH)
      || (tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH_WITH_DCH_INFO)
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

  ) 
  {
    tdsordered_config_ptr->rach_rlc_size_restriction_info.mac_update_needed = TRUE;
  }

  tdsordered_config_ptr->utran_drx_cycle_length =  tdstransition_config.toc_ptr->utran_drx_cycle_length;
  /* Copy over the RB-oriented side of RLC size restrictions */
  tdsrrcllc_copy_cell_fach_rlc_size_list(tdsordered_config_ptr, tdstransition_config.toc_ptr);

  /* Update the MAC layer with the newly copied RLC size restriction info */
  (void)tdsrrcllc_populate_mac_rlc_size_restriction_info(tdsordered_config_ptr, TDSRRC_STATE_CELL_FACH);



  TDSRRC_MSG0(MSG_LEGACY_HIGH,"update_oc_in_pch_with_toc");
} /* tdsrrcllc_update_oc_in_pch_with_toc */

/*====================================================================
FUNCTION: tdsrrcllc_copy_sccpch_info_from_sib_to_oc_in_connected_mode()

DESCRIPTION:
  This function is responsible for getting the SCCPCH info from SIB
  and updating of relevant Ordered Config data structures.

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS/FAILURE
  (For now, SUCCESS is always returned)

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_copy_sccpch_info_from_sib_to_oc_in_connected_mode
(
  tdsrrc_state_e_type  curr_rrc_state,
  tdsrrc_state_e_type  next_rrc_state
)
{
    /* Added a parameter to distinguish between getting sccpchinfo from sib5 or sib6, beacuse for future add new 
     SCCPCHinfo list extension, we change code handle from SCCPCHinfolist and SCCPCHinfolist-LCR-R4 to upper 
     structure which is sib5(or sib6). 
     sib6_present_flg is ture means get sccpch info from sib6, 
     sib6_present_flg is false means get sccpch info from sib5. */
  uint8    sib6_present_flg = FALSE;
  tdsrrc_SysInfoType5    *sib5_ptr = NULL;
  tdsrrc_SysInfoType6    *sib6_ptr = NULL;

  if((next_rrc_state == TDSRRC_STATE_DISCONNECTED) ||
     (next_rrc_state == TDSRRC_STATE_CONNECTING))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC state %d not handled here", next_rrc_state);
    return(FAILURE);
  }

  sib5_ptr = (tdsrrc_SysInfoType5 *)
              tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB5);

  if (sib5_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 5 not yet in SIB data base");
     return FAILURE;
  }

  if(sib5_ptr->sib6indicator == TRUE)
  {
    sib6_ptr = (tdsrrc_SysInfoType6 *)
                tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB6);
    if(sib6_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB6 not present in SIB DB");
    }
  } /* SIB6 is present as per SIB5 */

  /* Before updating with SCCPCH info, initialize all relevant Ordered
   * Config data structures.
   */
  /* Initialize PCH Info */
  tdsrrcllc_init_pch_info(tdsordered_config_ptr);

  /* Initialize FACH Info */
  /* If this function is called when next states is Cell_FACH and
   * Current state is either Cell FACH or Connecting,
   * do not initialize RB mapping Info. This is because some RBs would be
   * present if the current state is Cell_FACH.
   */
  if(((curr_rrc_state == TDSRRC_STATE_CELL_FACH) ||
      (curr_rrc_state == TDSRRC_STATE_CONNECTING))&&
     ((next_rrc_state == TDSRRC_STATE_CELL_FACH) ||
      (next_rrc_state == TDSRRC_STATE_CELL_PCH) ||
      (next_rrc_state == TDSRRC_STATE_URA_PCH)))
  {
    tdsrrcllc_init_fach_info(tdsordered_config_ptr, FALSE);
  }
  else
  {
    tdsrrcllc_init_fach_info(tdsordered_config_ptr, TRUE);
  }
  /* Initialize L1 SCCPCH Channel Info */
  tdsrrcllc_init_l1_sccpch_chan_info(tdsordered_config_ptr);
  /* Initialize L1 Downlink CCTrCH Info that includes Transport Format Set */
  tdsrrcllc_init_dl_cctrch_info(tdsordered_config_ptr);
  /* Initialize Downlink TFCS */
  tdsrrcllc_init_dl_ctfcs(tdsordered_config_ptr);

    /* Now get the SCCPCH system information */
  if(sib6_ptr)
  {
    if(TDSRRC_CHECK_MSG_TYPE_IE_PTR(sib6_ptr, 
             sCCPCH_SystemInformationListPresent) &&
        (TDSRRCLLC_SIB6_SCCPCH_LCR_EXT_PRESENT(sib6_ptr)))
    {
      if(tdsrrcllc_verify_sccpch_exists(sib6_ptr,
                              next_rrc_state)
         == TRUE)
      {
        /* Notify function tdsrrcllc_get_sccpch_sys_info() should get sccpchinfo from sib6 */
        sib6_present_flg = TRUE;
        if(tdsrrcllc_get_sccpch_sys_info(tdsordered_config_ptr,
                               sib6_ptr,
                               sib6_present_flg,
                               next_rrc_state)
            == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
          return FAILURE;
        }
      } /* SCCPCH with FACH exists in SIB6 */
      else
      {
        if(tdsrrcllc_get_sccpch_sys_info( tdsordered_config_ptr,
                                sib5_ptr,
                                sib6_present_flg,
                                next_rrc_state)
            == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
          return FAILURE;
        }
      } /* SCCPCH with FACH DOES NOT exist in SIB6 */
    } /* SCCPCH info in SIB6 is present */
    else
    {
      if(tdsrrcllc_get_sccpch_sys_info( tdsordered_config_ptr,
                              sib5_ptr,
                              sib6_present_flg,
                              next_rrc_state)
          == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
        return FAILURE;
      }
    } /* SCCPCH info in SIB6 not present */
  } /* SIB6 is present */
  else
  {
    if(tdsrrcllc_get_sccpch_sys_info( tdsordered_config_ptr,
                            sib5_ptr,
                            sib6_present_flg,
                            next_rrc_state)
        == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
      return FAILURE;
    }
  } /* SIB6 is NOT present */

  return(SUCCESS);
}  /* tdsrrcllc_copy_sccpch_info_from_sib_to_oc_in_connected_mode */

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
#if 0
/*====================================================================
FUNCTION: tdsrrcllc_copy_sib_to_oc_for_sccpch_with_ctch()

DESCRIPTION:
  This function is responsible for getting the SCCPCH info from SIB
  and updating of relevant Ordered Config data structures.

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS/FAILURE
  (For now, SUCCESS is always returned)

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_copy_sib_to_oc_for_sccpch_with_ctch
(
  tdsrrc_state_e_type  next_rrc_state
)
{
  rrc_SysInfoType5    *sib5_ptr = NULL;
  rrc_SysInfoType6    *sib6_ptr = NULL;

  /* Boolean value to indicate if sib 5 is in use.  If set to false, then
     sib 6 is in use */
  sib5_ptr = (tdsrrc_SysInfoType5 *)
              tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB5);

  if (sib5_ptr == NULL)
  {
     ERR_FATAL("SIB 5 not yet in SIB data base",0,0,0);
     return FAILURE;
  }

  if(sib5_ptr->sib6indicator == TRUE)
  {
    sib6_ptr = (tdsrrc_SysInfoType6 *)
                tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB6);
    if(sib6_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB6 not present in SIB DB");
    }
  } /* SIB6 is present as per SIB5 */

  /* Before updating with SCCPCH info, initialize all relevant Ordered
   * Config data structures.
   */

    /* Now get the SCCPCH system information */
  if(((next_rrc_state == TDSRRC_STATE_CELL_PCH) ||(next_rrc_state == TDSRRC_STATE_URA_PCH)) &&
      (sib6_ptr))
  {
    if(sib6_ptr->m.sCCPCH_SystemInformationListPresent)
    {
      if(tdsrrcllc_verify_sccpch_exists(&sib6_ptr->sCCPCH_SystemInformationList,
                              next_rrc_state)
         == TRUE)
      {
        if(tdsrrcllc_get_sys_info_for_sccpch_with_ctch(tdsordered_config_ptr,
                               &(sib6_ptr->sCCPCH_SystemInformationList),
                               next_rrc_state)
            == FAILURE)
        {
          ERR_FATAL("Failure updating SCCPCH Info", 0, 0, 0);
          return FAILURE;
        }
      } /* SCCPCH with FACH exists in SIB6 */
      else
      {
        if(tdsrrcllc_get_sys_info_for_sccpch_with_ctch( tdsordered_config_ptr,
                                &(sib5_ptr->sCCPCH_SystemInformationList),
                                next_rrc_state)
            == FAILURE)
        {
          ERR_FATAL("Failure updating SCCPCH Info", 0, 0, 0);
          return FAILURE;
        }
      } /* SCCPCH with FACH DOES NOT exist in SIB6 */
    } /* SCCPCH info in SIB6 is present */
    else
    {
      if(tdsrrcllc_get_sys_info_for_sccpch_with_ctch( tdsordered_config_ptr,
                              &(sib5_ptr->sCCPCH_SystemInformationList),
                              next_rrc_state)
          == FAILURE)
      {
        ERR_FATAL("Failure updating SCCPCH Info", 0, 0, 0);
        return FAILURE;
      }
    } /* SCCPCH info in SIB6 not present */
  } /* SIB6 is present */

  else  /* sib 6 is not present */
  {
    if(tdsrrcllc_get_sys_info_for_sccpch_with_ctch( tdsordered_config_ptr,
                            &(sib5_ptr->sCCPCH_SystemInformationList),
                            next_rrc_state)
        == FAILURE)
    {
      ERR_FATAL("Failure updating SCCPCH Info", 0, 0, 0);
      return FAILURE;
    }
  } /* SIB6 is NOT present */
  /* Getting CBS related info.  Get the Period of CTCH allocation (N) and
     CBS frame offset (K)  */
  if (next_rrc_state == TDSRRC_STATE_DISCONNECTED || next_rrc_state == TDSRRC_STATE_CELL_PCH ||
      next_rrc_state == TDSRRC_STATE_URA_PCH )
  {
    if (tdsrrcllc_semi_permanent_data.dl_phy_chan.ctch_status == TDSRRCLLC_CTCH_ON_DIF_SCCPCH)
    {
      tdsordered_config_ptr->l1_dl_chan_parms.sccpch[1].cbs_sched.ctch_info_present = TRUE;
      tdsordered_config_ptr->l1_dl_chan_parms.sccpch[1].cbs_sched.only_ctch_on_sccpch = TRUE;
      /*When UE is in connected mode, it ignores any CTCH info present in SIB6 and only uses info in SIB5*/
      tdsordered_config_ptr->l1_dl_chan_parms.sccpch[1].cbs_sched.cbs_frame_offset =
        (uint16)sib5_ptr->cbs_DRX_Level1Information.cbs_FrameOffset;
      tdsordered_config_ptr->l1_dl_chan_parms.sccpch[1].cbs_sched.ctch_allocation_period =
        (uint16)sib5_ptr->cbs_DRX_Level1Information.ctch_AllocationPeriod;
      
    }
  }

  return(SUCCESS);
}  /* tdsrrcllc_copy_sccpch_info_from_sib_to_oc_in_connected_mode */
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT

/*====================================================================
FUNCTION: tdsrrcllc_get_current_crnti_status()

DESCRIPTION:
  This function is responsible for returning the C-RNTI status as it
  exists in the Current Config. If Current Config does not have a valid
  CRNTI but a valid CRNTI exists in Ordered Config, that is understood
  as a valid CRNTI status.

DEPENDENCIES:
  The procedure calling this function should have called either
  set_ordered_config or should have updated C-RNTI thru' RNTI UPDATE REQ
  before a call to this function is made.

RETURN VALUE:
  tdsrrcllc_crnti_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_crnti_status_e_type
tdsrrcllc_get_current_crnti_status(void)
{
  if ((tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSCRNTI_VALID)||
      (tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Valid C-RNTI in CC");
    return TDSC_RNTI_VALID;
  }
  else /* No valid C_RNTI in Current Config */
  {
    if ((tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSCRNTI_VALID)||
        (tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Valid C-RNTI in OC");
      return TDSC_RNTI_VALID;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"No Valid C-RNTI in CC or OC");
      return TDSC_RNTI_NOT_VALID;
    }
  }  /* No valid C_RNTI in Current Config */
} /* tdsrrcllc_get_current_crnti_status */

/*====================================================================
FUNCTION: tdsrrcllc_get_current_urnti()

DESCRIPTION:
  This function is responsible for returning the U-RNTI status as it
  exists in the Current Config. If Current Config does not have a valid
  URNTI, then it return TDSU_RNTI_NOT_PRESENT.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_urnti_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_urnti_status_e_type
tdsrrcllc_get_current_urnti(uint32* urnti)
{
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    if ((tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSURNTI_VALID)||
        (tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"U-RNTI present in OC");
      *urnti = tdsordered_config_ptr->mac_dl_parms.rnti_info.urnti;
      return TDSU_RNTI_PRESENT;
    }
    else /* No valid U_RNTI in Ordered Config */
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"U_RNTI is not present in OC");

      return TDSU_RNTI_NOT_PRESENT;
    }  /* No valid U_RNTI in Ordered Config */
  }
  else  /* Ordered Config NOT set */
  {
    if ((tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSURNTI_VALID)||
        (tdscurrent_config_ptr->mac_dl_parms.rnti_info.rnti_valid == TDSBOTH_VALID))
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"U-RNTI present in CC");
      *urnti = tdscurrent_config_ptr->mac_dl_parms.rnti_info.urnti;
      return TDSU_RNTI_PRESENT;
    }
    else /* No valid U_RNTI in Current Config */
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"U_RNTI is not present in CC");

      return TDSU_RNTI_NOT_PRESENT;
    }  /* No valid U_RNTI in Current Config */
  }
} /* tdsrrcllc_get_current_urnti */

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_for_resel()

DESCRIPTION:
  This function is responsible for updating Ordered Config due to a
  Cell reselection in all RRC states.
  It initializes all SCCPCH Info, CTFCs and L1 CCTrCH Info.
  The PCH info is not initialized so that the PCCH logical channel ID
  could be maintained. In Connecting and Connected modes, FACH
  info is not initialized
  The SCCPCH Info on new cell is read and all
  info is updated. The PCH Transport Channel ID is also updated.

DEPENDENCIES:
  Serving Cell index in SIB should be initialized before calling this
  function. Otherwise this may read SIBs from old cell.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_for_resel
(
  tdsrrc_state_e_type  curr_rrc_state
)
{
  tdsrrc_SysInfoType1    *sib1_ptr = NULL;
  tdsrrc_SysInfoType3    *sib3_ptr = NULL;
  tdsrrc_SysInfoType4    *sib4_ptr = NULL;
  tdsrrc_SysInfoType5    *sib5_ptr = NULL;
  tdsrrc_SysInfoType6    *sib6_ptr = NULL;
  tdsrrc_SysInfoType7    *sib7_ptr = NULL;
  tdsrrc_MappingFunctionParameterList *sib_3_4_mapping_v4b0ext_ptr = NULL;

  /* Pointer & counter to loop through the CN System Information List */
  tdsrrc_CN_DomainSysInfoList * cn_ptr = NULL;
  uint8 cn_cnt                    = 0;
  uint32     i;
  uint32     j;
  /* Added a parameter to distinguish between getting sccpchinfo from sib5 or sib6, beacuse for future add new 
     SCCPCHinfo list extension, we change code handle from SCCPCHinfolist and SCCPCHinfolist-LCR-R4 to upper 
     structure which is sib5(or sib6). 
     sib6_present_flg is ture means get sccpch info from sib6, 
     sib6_present_flg is false means get sccpch info from sib5. */
  uint8   sib6_present_flg = FALSE;

  /* Update ROC only in all cases. */
  /* Before updating with SCCPCH info, initialize all relevant
   * Reselection Config data structures.
   */
  /* Initialize PCH Info */
  tdsrrcllc_init_pch_info(tdsreselection_config_ptr);
  /* Initialize FACH Info */
  if(curr_rrc_state == TDSRRC_STATE_DISCONNECTED)
  {
    tdsrrcllc_init_fach_info(tdsreselection_config_ptr, TRUE);
  }
  else
  {
    /* Do not delete RB Mapping Info in any other RRC state */
    tdsrrcllc_init_fach_info(tdsreselection_config_ptr, FALSE);
  }
  /* Initialize L1 SCCPCH Channel Info */
  tdsrrcllc_init_l1_sccpch_chan_info(tdsreselection_config_ptr);
  /* Initialize L1 Downlink CCTrCH Info that includes Transport Format Set */
  tdsrrcllc_init_dl_cctrch_info(tdsreselection_config_ptr);
  /* Initialize Downlink TFCS */
  tdsrrcllc_init_dl_ctfcs(tdsreselection_config_ptr);

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrcllc_init_dl_ctfcs_for_sccpch_with_ctch(tdsreselection_config_ptr);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

  /************************/
  /* Get SIB 1 data first */
  /************************/
  sib1_ptr = (tdsrrc_SysInfoType1 *)
               tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1);
  if (sib1_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 1 not yet in SIB data base");
     return FAILURE;
  }

  /* Get the CS and PS DRX Cycle Lengths */
  cn_ptr = &sib1_ptr->cn_DomainSysInfoList;

  while((cn_cnt < cn_ptr->n ) && (cn_cnt < UE_MAX_CN_DOMAINS))
  {
    if((cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff >= TDSRRCLLC_CN_DOMAIN_MIN_DRX_CYCLE_LENGTH) &&
       (cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff <= TDSRRCLLC_CN_DOMAIN_MAX_DRX_CYCLE_LENGTH))
    {
      switch(cn_ptr->elem[cn_cnt].cn_DomainIdentity)
      {
        case tdsrrc_CN_DomainIdentity_cs_domain:
          tdsreselection_config_ptr->cs_drx_cycle_length =
            (uint32) (1 << cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff);
          break;
        case tdsrrc_CN_DomainIdentity_ps_domain:
          tdsreselection_config_ptr->ps_drx_cycle_length =
            (uint32) (1 << cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff);
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CN 0x%x for DRX", cn_ptr->elem[cn_cnt].cn_DomainIdentity);
          return FAILURE;
      } /* end CN Identity switch */
    } /* end drx cycle length check */
    else
    {
      /* A SIB with invalid DRX length coefficient is unacceptable */
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid DRX len value %d, CN domain %d",
          cn_ptr->elem[cn_cnt].cn_DRX_CycleLengthCoeff, cn_cnt);
      return FAILURE;
    }
    /* Next CN Info */
    cn_cnt++;
  } /* end while Core Network Info */


  /* Check to make sure both Core Networks have good DRX info */
  if(tdsreselection_config_ptr->ps_drx_cycle_length == 0)
  {
    /* This indicates PS domain not present */
    if(tdsreselection_config_ptr->cs_drx_cycle_length > 0)
    {
      tdsreselection_config_ptr->ps_drx_cycle_length =
        tdsreselection_config_ptr->cs_drx_cycle_length;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Both PS and CS DRX info missing");
      return FAILURE;
    }
  }

  if(tdsreselection_config_ptr->cs_drx_cycle_length == 0)
  {
    /* This indicates CS domain not present */
    if(tdsreselection_config_ptr->ps_drx_cycle_length > 0)
    {
      tdsreselection_config_ptr->cs_drx_cycle_length =
        tdsreselection_config_ptr->ps_drx_cycle_length;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Both PS and CS DRX info missing");
      return FAILURE;
    }
  }

  /* Only T312 and N312 in Idle mode timers and constants are needed
   * by L1. There is no need to read them in Idle mode since they are
   * going to be used only in Connected mode.
   */
  if(tdsrrcllc_get_timers_and_const_in_conn_mode(tdsreselection_config_ptr, sib1_ptr) == FAILURE)
  {
    return(FAILURE);
  }

  /************************/
  /* Get SIB 3 data next  */
  /************************/
  sib3_ptr = (tdsrrc_SysInfoType3 *)
             tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB3);
  if (sib3_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 3 not yet in SIB data base");
     return FAILURE;
  }

  if(((curr_rrc_state == TDSRRC_STATE_CELL_FACH) || (curr_rrc_state == TDSRRC_STATE_CELL_PCH) ||
      (curr_rrc_state == TDSRRC_STATE_URA_PCH)) && (sib3_ptr->sib4indicator == TRUE) && 
     ((sib4_ptr = (tdsrrc_SysInfoType4 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB4)) != NULL ))
  {
    if((sib4_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power >= TDSRRCLLC_MIN_ALLOW_UL_TX_PWR) &&
       (sib4_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power <= TDSRRCLLC_MAX_ALLOW_UL_TX_PWR))
    {
      tdsreselection_config_ptr->max_tx_pwr =
        (int8) sib4_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power;
      /* Indicate that L1 UL needs to be reconfigured */
      tdsreselection_config_ptr->reconfig_needed.uplink_l1 = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcv'd invalid UL TX PWR %d", sib4_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power);
      return FAILURE;
    }
    /* Set UpPCH_shift as default value at first */
    tdsreselection_config_ptr->l1_ul_chan_parms.prach_cfg.upPch_shift = 0;

    /* Check if SIB4 has configured UpPCH_shift */
    if (sib4_ptr->m.v4b0NonCriticalExtensionsPresent)
    {
      if (sib4_ptr->v4b0NonCriticalExtensions.sysInfoType4_v4b0ext.m.mapping_LCRPresent)
      {
        sib_3_4_mapping_v4b0ext_ptr = 
           &(sib4_ptr->v4b0NonCriticalExtensions.sysInfoType4_v4b0ext.mapping_LCR.mappingFunctionParameterList);

        if (sib_3_4_mapping_v4b0ext_ptr->elem[0].m.mapParameter1Present)
        {
          tdsreselection_config_ptr->l1_ul_chan_parms.prach_cfg.upPch_shift =
            sib_3_4_mapping_v4b0ext_ptr->elem[0].mapParameter1;
        }
      }
    }
  } /* If next state == Cell_FACH AND SIB 4 is present */
  else
  {
    if((sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power >= TDSRRCLLC_MIN_ALLOW_UL_TX_PWR) &&
       (sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power <= TDSRRCLLC_MAX_ALLOW_UL_TX_PWR)
      )
    {
      tdsreselection_config_ptr->max_tx_pwr =
        (int8) sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power;
      /* Indicate that L1 UL needs to be reconfigured */
      tdsreselection_config_ptr->reconfig_needed.uplink_l1 = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcv'd invalid UL TX PWR %d", sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power);
      return FAILURE;
    }
    /* Set UpPCH_shift as default value at first */
    tdsreselection_config_ptr->l1_ul_chan_parms.prach_cfg.upPch_shift = 0;

    /* Check if SIB3 has configured UpPCH_shift */
    if (sib3_ptr->m.v4b0NonCriticalExtensionsPresent)
    {
      if (sib3_ptr->v4b0NonCriticalExtensions.sysInfoType3_v4b0ext.m.mapping_LCRPresent)
      {
        sib_3_4_mapping_v4b0ext_ptr = 
           &(sib3_ptr->v4b0NonCriticalExtensions.sysInfoType3_v4b0ext.mapping_LCR.mappingFunctionParameterList);

        if (sib_3_4_mapping_v4b0ext_ptr->elem[0].m.mapParameter1Present)
        {
          tdsreselection_config_ptr->l1_ul_chan_parms.prach_cfg.upPch_shift =
            sib_3_4_mapping_v4b0ext_ptr->elem[0].mapParameter1;
        }
      }
    }
  }

  /************************/
  /* Get SIB 7 data next  */
  /************************/
  sib7_ptr = (tdsrrc_SysInfoType7 *)
             tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB7);
  if (sib7_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 7 not yet in SIB data base");
     return FAILURE;
  }

  if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib7_ptr->modeSpecificInfo,
   rrc_SysInfoType7_modeSpecificInfo_fdd))
  {

    TDSRRC_MSG0(MSG_LEGACY_ERROR,"FDD mode choice received");
    return FAILURE;
  }

  /************************/
  /* Get SIB 5 data next  */
  /************************/

  sib5_ptr = (tdsrrc_SysInfoType5 *)
             tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB5);
  if (sib5_ptr == NULL)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 5 not yet in SIB data base");
     return FAILURE;
  }
  if(((curr_rrc_state == TDSRRC_STATE_CELL_FACH)|| (curr_rrc_state == TDSRRC_STATE_CELL_PCH) ||
          (curr_rrc_state == TDSRRC_STATE_URA_PCH)) && (sib5_ptr->sib6indicator == TRUE))
  {
    sib6_ptr = (tdsrrc_SysInfoType6 *)
               tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB6);
    if (sib6_ptr == NULL)
    {
       /* SIB 6 may not be present. That is acceptable */
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 6 not in SIB data base");
    }
  }

  /* Get the PICH Power offset */
  if(sib6_ptr && ((curr_rrc_state == TDSRRC_STATE_CELL_FACH)|| (curr_rrc_state == TDSRRC_STATE_CELL_PCH) ||
          (curr_rrc_state == TDSRRC_STATE_URA_PCH)))
  {
    if(((int8)sib6_ptr->pich_PowerOffset >= TDSRRCLLC_MIN_PICH_PWR_OFFSET) &&
       ((int8)sib6_ptr->pich_PowerOffset <= TDSRRCLLC_MAX_PICH_PWR_OFFSET))
    {
      tdsreselection_config_ptr->l1_dl_chan_parms.pich.pwr_offset =
        (int8)sib6_ptr->pich_PowerOffset;
      /* Indicate that L1 DL needs to be reconfigured */
      tdsreselection_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
       * still needs to be reconfigured so as to get the new CCTrCH
       * ID whenever Downlink L1 is reconfigured. This is due to the
       * interface requirements.
       */
      tdsreselection_config_ptr->reconfig_needed.downlink_mac = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"invalid PICH Power Offset %d", (int8)sib6_ptr->pich_PowerOffset);
      return(FAILURE);
    }
  }
  else
  {
    if(((int8)sib5_ptr->pich_PowerOffset >= TDSRRCLLC_MIN_PICH_PWR_OFFSET) &&
       ((int8)sib5_ptr->pich_PowerOffset <= TDSRRCLLC_MAX_PICH_PWR_OFFSET))
    {
      tdsreselection_config_ptr->l1_dl_chan_parms.pich.pwr_offset =
        (int8)sib5_ptr->pich_PowerOffset;
      /* Indicate that L1 DL needs to be reconfigured */
      tdsreselection_config_ptr->reconfig_needed.downlink_l1 = TRUE;
      /* Although no new parameters for MAC have been received, MAC
       * still needs to be reconfigured so as to get the new CCTrCH
       * ID whenever Downlink L1 is reconfigured. This is due to the
       * interface requirements.
       */
      tdsreselection_config_ptr->reconfig_needed.downlink_mac = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"invalid PICH Power Offset %d", (int8)sib5_ptr->pich_PowerOffset);
      return(FAILURE);
    }
  } /* SIB 6 ptr is NULL */

  /* Get the Primary CCPCH Tx Power */
  if(sib6_ptr && ((curr_rrc_state == TDSRRC_STATE_CELL_FACH) || (curr_rrc_state == TDSRRC_STATE_CELL_PCH) ||
          (curr_rrc_state == TDSRRC_STATE_URA_PCH)))
  {
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib6_ptr->modeSpecificInfo,
    rrc_SysInfoType6_modeSpecificInfo_tdd))
    {
      if(((int8) sib6_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power >=
            TDSRRCLLC_MIN_PRI_CCPCH_TX_PWR) &&
         ((int8) sib6_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power <=
            TDSRRCLLC_MAX_PRI_CCPCH_TX_PWR))
      {
        tdsreselection_config_ptr->l1_ul_chan_parms.prach_cfg.pccpch_tx_pwr =
          (int8) sib6_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power;
        /* Indicate that L1 UL needs to be reconfigured */
        tdsreselection_config_ptr->reconfig_needed.uplink_l1 = TRUE;
        /* Although no new parameters for MAC have been received, MAC
         * still needs to be reconfigured so as to get the new CCTrCH
         * ID whenever Uplink L1 is reconfigured. This is due to the
         * interface requirements.
         */
        tdsreselection_config_ptr->reconfig_needed.uplink_mac = TRUE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d Primary CCPCH Tx Power",
            sib6_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power);
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cell TDD info absent");
      return FAILURE;
    }
  }
  else
  {
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(sib5_ptr->modeSpecificInfo,
    rrc_SysInfoType5_modeSpecificInfo_tdd))
    {
      if(((int8) sib5_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power >=
            TDSRRCLLC_MIN_PRI_CCPCH_TX_PWR) &&
         ((int8) sib5_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power <=
            TDSRRCLLC_MAX_PRI_CCPCH_TX_PWR))
      {
        tdsreselection_config_ptr->l1_ul_chan_parms.prach_cfg.pccpch_tx_pwr =
          (int8) sib5_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power;
        /* Indicate that L1 UL needs to be reconfigured */
        tdsreselection_config_ptr->reconfig_needed.uplink_l1 = TRUE;
        /* Although no new parameters for MAC have been received, MAC
         * still needs to be reconfigured so as to get the new CCTrCH
         * ID whenever Uplink L1 is reconfigured. This is due to the
         * interface requirements.
         */
        tdsreselection_config_ptr->reconfig_needed.uplink_mac = TRUE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value %d Primary CCPCH Tx Power",
            sib5_ptr->modeSpecificInfo.u.tdd->openLoopPowerControl_TDD.primaryCCPCH_TX_Power);
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cell TDD info absent");
      return FAILURE;
    }
  }  /* SIB 6 ptr is NULL */

  /* Check to see if Primary PCCPCH info is present. If so, save it. */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib5_ptr,tdsrrc_SysInfoType5,primaryCCPCH_Info))
  {
    /* This information is present */
    /* Note, TDSORDERED_CONFIG is always filled out for the serving cell */
    // new function call here due to modularization
    // rrcllc_set_pccpch(sib5_ptr->primaryCCPCH_Info.u.fdd.tx_DiversityIndicator);
  }
  /* else this IE is not present */

  /* Now get the PRACH system information */
  if(sib6_ptr && ((curr_rrc_state == TDSRRC_STATE_CELL_FACH) || (curr_rrc_state == TDSRRC_STATE_CELL_PCH) ||
          (curr_rrc_state == TDSRRC_STATE_URA_PCH)))
  {
    if((sib6_ptr->m.prach_SystemInformationListPresent)&&
       (sib6_ptr->m.v4b0NonCriticalExtensionsPresent)&&
       (sib6_ptr->v4b0NonCriticalExtensions.m.sysInfoType6_v4b0extPresent)&&
       (sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.m.prach_SystemInformationList_LCR_r4Present))
    {
      if(tdsrrcllc_get_prach_sys_info(tdsreselection_config_ptr, sib6_ptr, TRUE) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating PRACH Info");
        return FAILURE;
      }
    }
    else  /* Read PRACH Info from SIB 5 */
    {
      if(tdsrrcllc_get_prach_sys_info(tdsreselection_config_ptr, sib5_ptr, FALSE) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating PRACH Info");
        return FAILURE;
      }
    }
  }
  else
  {

    if(tdsrrcllc_get_prach_sys_info(tdsreselection_config_ptr, sib5_ptr, FALSE) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating PRACH Info");
      return FAILURE;
    }
  }

  (void)tdsrrcllc_populate_mac_rlc_size_restriction_info(tdsreselection_config_ptr,
                                                      tdsreselection_config_ptr->rrc_state_indicator);


  if(sib6_ptr && ((curr_rrc_state == TDSRRC_STATE_CELL_FACH)|| (curr_rrc_state == TDSRRC_STATE_CELL_PCH) ||
          (curr_rrc_state == TDSRRC_STATE_URA_PCH)))
  {
    if((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sib6_ptr,sCCPCH_SystemInformationList)) &&
        (TDSRRCLLC_SIB6_SCCPCH_LCR_EXT_PRESENT(sib6_ptr)))      
    {
      if(tdsrrcllc_verify_sccpch_exists(sib6_ptr,
                              curr_rrc_state)
         == TRUE)
      {
        /* Notify function tdsrrcllc_get_sccpch_sys_info() should get sccpchinfo from sib6 */
        sib6_present_flg = TRUE;
        if(tdsrrcllc_get_sccpch_sys_info(tdsreselection_config_ptr,
                               sib6_ptr,
                               sib6_present_flg,
                               curr_rrc_state)
           == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
          return FAILURE;
        }
      } /* SCCPCH with FACH present in SIB6 */
      else
      {
        if(tdsrrcllc_get_sccpch_sys_info(tdsreselection_config_ptr,
                               sib5_ptr,
                               sib6_present_flg,
                               curr_rrc_state)
           == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
          return FAILURE;
        }
      } /* SCCPCH with FACH NOT present in SIB6 */
    } /* SCCPCH Info present in SIB6 */
    else /* Read it from SIB5 */
    {
      if(tdsrrcllc_get_sccpch_sys_info(tdsreselection_config_ptr,
                             sib5_ptr,
                             sib6_present_flg,
                             curr_rrc_state)
         == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
        return FAILURE;
      }
    }
  }
  else /* SCCPCH system info in SIB 5 has to be used */
  {
    if(tdsrrcllc_get_sccpch_sys_info(tdsreselection_config_ptr,
                           sib5_ptr,
                           sib6_present_flg,
                           curr_rrc_state)
       == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failure updating SCCPCH Info");
      return FAILURE;
    }
  }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
#if 0
  /* Getting CBS related info.  Get the Period of CTCH allocation (N) and
     CBS frame offset (K)  */
  if (curr_rrc_state == RRC_STATE_DISCONNECTED || curr_rrc_state == RRC_STATE_CELL_PCH ||
      curr_rrc_state == RRC_STATE_URA_PCH )
  {
    if (rrcllc_semi_permanent_data.dl_phy_chan.ctch_status == RRCLLC_CTCH_WITH_PCCH)
    {
      tdsreselection_config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.ctch_info_present = TRUE;
      tdsreselection_config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.only_ctch_on_sccpch = FALSE;
      /*When UE is in connected mode, it ignores any CTCH info present in SIB6 and only uses info in SIB5*/
      tdsreselection_config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.cbs_frame_offset =
        (uint16)sib5_ptr->cbs_DRX_Level1Information.cbs_FrameOffset;
      tdsreselection_config_ptr->l1_dl_chan_parms.sccpch[0].cbs_sched.ctch_allocation_period =
        (uint16)sib5_ptr->cbs_DRX_Level1Information.ctch_AllocationPeriod;
    }
  }
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
  /* If this is in Connecting/CELL_FACH/CELL_PCH/URA_PCH state, copy the Mapping
   * info to all available FACHes
   */
  if((curr_rrc_state == TDSRRC_STATE_CONNECTING) ||
     (curr_rrc_state == TDSRRC_STATE_CELL_FACH) ||
   (curr_rrc_state == TDSRRC_STATE_CELL_PCH) ||
   (curr_rrc_state == TDSRRC_STATE_URA_PCH))
  {
    if(tdsreselection_config_ptr->mac_dl_parms.num_fachs > 1)
    {
      for(i = 0; i < tdsordered_config_ptr->mac_dl_parms.num_fachs; i ++)
      {
        tdsreselection_config_ptr->mac_dl_parms.fach_info[i]->ccch_enable =
          tdsreselection_config_ptr->mac_dl_parms.fach_info[0]->ccch_enable;
        tdsreselection_config_ptr->mac_dl_parms.fach_info[i]->ccch_id =
          tdsreselection_config_ptr->mac_dl_parms.fach_info[0]->ccch_id;
        tdsreselection_config_ptr->mac_dl_parms.fach_info[i]->ndlchan =
          tdsreselection_config_ptr->mac_dl_parms.fach_info[0]->ndlchan;

        for(j = 0; j < MAX_DED_LOGCHAN_PER_TRCH; j ++)
        {
          tdsreselection_config_ptr->mac_dl_parms.fach_info[i]->dlchan_info[j] =
            tdsreselection_config_ptr->mac_dl_parms.fach_info[0]->dlchan_info[j];
        }
        tdsreselection_config_ptr->mac_dl_parms.fach_info[i]->bcch_enable =
          tdsreselection_config_ptr->mac_dl_parms.fach_info[0]->bcch_enable;
        tdsreselection_config_ptr->mac_dl_parms.fach_info[i]->bcch_id =
          tdsreselection_config_ptr->mac_dl_parms.fach_info[0]->bcch_id;
      }
    } /* If number of FACHs > 1 */
  } /* If current state is CELL_FACH or Connecting */

  return(SUCCESS);
}  /* tdsrrcllc_update_oc_for_resel */

/*====================================================================
FUNCTION: tdsrrcllc_update_cell_fach_dl_mapping_info_with_log_chl_id()

DESCRIPTION:
  This function updates the Downlink Mapping Info in Cell FACH RB Mapping
  Info with the passed Logical Channel ID.
  The correct entity is selected based on the passed RB ID.
  The Cell FACH RB mapping info in the passed Config data is updated.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_cell_fach_dl_mapping_info_with_log_chl_id
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_RB_Identity  rb_id,
  rlc_lc_id_type   rlc_dl_log_chl_id
)
{
  int i;
  for(i=0; i<config_ptr->cell_fach_rb_mapping_info.dl_ndlchan; i++)
  {
    if(config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[i].rb_id == rb_id)
    {
      config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[i].rlc_id = rlc_dl_log_chl_id;
      break;
    }
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_update_cell_fach_ul_mapping_info_with_log_chl_id()

DESCRIPTION:
  This function updates the Uplink Mapping Info in Cell FACH RB Mapping
  Info with the passed Logical Channel ID.
  The correct entity is selected based on the passed RB ID.
  The Cell FACH RB mapping info in the passed Config data is updated.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_cell_fach_ul_mapping_info_with_log_chl_id
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_RB_Identity  rb_id,
  rlc_lc_id_type   rlc_ul_log_chl_id
)
{
  int i;
  for(i=0; i<config_ptr->cell_fach_rb_mapping_info.ul_ndlchan; i++)
  {
    if(config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[i].rb_id == rb_id)
    {
      config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[i].rlc_id = rlc_ul_log_chl_id;
      break;
    }
  }
}


/*====================================================================
FUNCTION: tdsrrcllc_check_dcch_fach_mapping(tdsordered_config_type *config_ptr)

DESCRIPTION:
  This function checks whether DCCH mapping on FACH exists in
  ordered config.

DEPENDENCIES:
  None
RETURN VALUE:
  booean
SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_check_dcch_fach_mapping(tdsordered_config_type *config_ptr)
{
  uint8 dl_rb_count = 0;
  uint8 ul_rb_count = 0;
  uint8 i;

  for(i = 0; i < config_ptr->cell_fach_rb_mapping_info.dl_ndlchan; i++)
  {
    if(config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[i].rb_id >=
       DCCH_UM_RADIO_BEARER_ID &&
       config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[i].rb_id <=
       DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
    {
      dl_rb_count++;
    }
  }
  for(i = 0; i < config_ptr->cell_fach_rb_mapping_info.ul_ndlchan; i++)
  {
    if(config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[i].rb_id >=
       DCCH_UM_RADIO_BEARER_ID &&
       config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[i].rb_id <=
       DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
    {
      ul_rb_count++;
    }
  }

  if((dl_rb_count >= 3) && (ul_rb_count >= 3))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"DCCH mapping on FACH present dlrb:%d ulrb:%d",dl_rb_count, ul_rb_count);
    return (TRUE);
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"No DCCH mapping on FACH  dlrb:%d ulrb:%d",dl_rb_count, ul_rb_count);
    return(FALSE);
  }
}

/*====================================================================
FUNCTION: rrcllc_update_oc_for__fach_with_toc_for_rl_failure()

DESCRIPTION:
  This function is responsible for updating OC with TOC parameters
  immediately after transitioning to CELL_FACH.
  The CC is copied to OC and then OC is updated with RLC and RB Mapping
  Info.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_oc_in_fach_with_toc_for_rl_failure(
  void
)
{
  /* Start with copying CC to OC */
  tdsrrcllc_copy_cc_to_oc();

#ifdef FEATURE_UMTS_PDCP
  /* Copy all pdcp info into TOC */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Printing PDCP info from TOC");
  tdsrrcllc_print_pdcp_info(tdstransition_config.toc_ptr);
  tdsrrcllc_copy_pdcp_parms(tdsordered_config_ptr, tdstransition_config.toc_ptr);
#endif /* FEATURE_UMTS_PDCP*/

  tdsrrcllc_init_reestablish_entity(TDSORDERED_CONFIG_ONLY);

  /* Copy all RLC Config data for AM channels */
  tdsrrcllc_copy_rlc_parms(tdsordered_config_ptr, tdstransition_config.toc_ptr);
  /* Increment DL UM and UL TM Nchans to accomodate the already
   * existing CCCH.
   */
  if(tdsrrcllc_get_rlc_dl_um_info(CCCH_RADIO_BEARER_ID, TRUE, tdsordered_config_ptr) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not update CCCH RB in OC.");
  }
  
  tdsordered_config_ptr->rlc_ul_tm_parms.nchan ++;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"nchan: FACH RLF CCCH up UL TM %d after increment",
      tdsordered_config_ptr->rlc_ul_tm_parms.nchan);

  /* Re-map all channels previously mapped on DCHs to FACHs and PRACH */
  tdsrrcllc_copy_cell_fach_rb_mapping_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);

  /* Also copy the RNTI Info from TOC to OC */
  tdsordered_config_ptr->mac_ul_parms.rnti_info =
    tdstransition_config.toc_ptr->mac_ul_parms.rnti_info;
  tdsordered_config_ptr->mac_dl_parms.rnti_info =
    tdstransition_config.toc_ptr->mac_dl_parms.rnti_info;

  /* Copy over the RB-oriented side of RLC size restrictions */
  tdsrrcllc_copy_cell_fach_rlc_size_list(tdsordered_config_ptr, tdstransition_config.toc_ptr);

  /* Update the MAC layer with the newly copied RLC size restriction info */
  (void)tdsrrcllc_populate_mac_rlc_size_restriction_info(tdsordered_config_ptr, TDSRRC_STATE_CELL_FACH);


#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT  
  tdsrrcllc_copy_srb5_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */  

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"update_oc_in_fach_with_toc_for_rl_failure");

} /* tdsrrcllc_update_oc_in_fach_with_toc_for_rl_failure */

/*====================================================================
FUNCTION: tdsrrcllc_update_cc_in_fach_with_toc_for_oos_ind()

DESCRIPTION:
  This function is responsible for updating CC with TOC parameters
  immediately after transitioning to CELL_FACH.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_cc_in_fach_with_toc_for_oos_ind
(
  void
)
{
#ifdef FEATURE_UMTS_PDCP
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Printing PDCP info from TOC");
  tdsrrcllc_print_pdcp_info(tdstransition_config.toc_ptr);
  tdsrrcllc_copy_pdcp_parms(tdscurrent_config_ptr, tdstransition_config.toc_ptr);
#endif /* FEATURE_UMTS_PDCP*/

  tdsrrcllc_init_reestablish_entity(TDSCURRENT_CONFIG_ONLY);

  /* Copy all RLC Config data for AM channels */
  tdsrrcllc_copy_rlc_parms(tdscurrent_config_ptr, tdstransition_config.toc_ptr);

  /* Increment DL UM and UL TM nchans to accomodate the already
   * existing CCCH. */
  if(tdsrrcllc_get_rlc_dl_um_info(CCCH_RADIO_BEARER_ID, TRUE, tdscurrent_config_ptr) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not update CCCH RB in CC.");
  }
  
  tdscurrent_config_ptr->rlc_ul_tm_parms.nchan ++;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"nchan: FACH RLF CCCH up UL TM %d after increment",
      tdscurrent_config_ptr->rlc_ul_tm_parms.nchan);

  /*Re-map all channels previously mapped on DCHs to FACHs and PRACH*/
  tdsrrcllc_copy_cell_fach_rb_mapping_info(tdscurrent_config_ptr, tdstransition_config.toc_ptr);

  /* Also copy the RNTI Info from TOC to CC */
  tdscurrent_config_ptr->mac_ul_parms.rnti_info = tdstransition_config.toc_ptr->mac_ul_parms.rnti_info;
  tdscurrent_config_ptr->mac_dl_parms.rnti_info = tdstransition_config.toc_ptr->mac_dl_parms.rnti_info;

  /* Copy over the RB-oriented side of RLC size restrictions */
  tdsrrcllc_copy_cell_fach_rlc_size_list(tdscurrent_config_ptr, tdstransition_config.toc_ptr);

  /* Update the MAC layer with the newly copied RLC size restriction info */
  (void)tdsrrcllc_populate_mac_rlc_size_restriction_info(tdscurrent_config_ptr, TDSRRC_STATE_CELL_FACH);

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT  
  tdsrrcllc_copy_srb5_info(tdscurrent_config_ptr, tdstransition_config.toc_ptr);
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */  

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcllc_update_cc_in_fach_with_toc_for_oos_ind");
  return;
} /* tdsrrcllc_update_oc_in_fach_with_toc_for_rl_failure */

/*====================================================================
FUNCTION: tdsrrcllc_update_toc_ul_dch_info_with_log_chl_id()

DESCRIPTION:
  This function updates the MAC UL Info in TOC with LOgical Channel ID.
  The correct entity is selected based on the passed RB ID.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_update_toc_ul_dch_info_with_log_chl_id
(
  tdsrrc_RB_Identity  rb_id,
  rlc_lc_id_type  rlc_ul_log_chl_id
)
{
  int i, j;
  boolean  log_chl_found = FALSE;

  for(i=0; i<tdstransition_config.toc_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs; i++)
  {
    for(j=0; j<tdstransition_config.toc_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[i].ndlchan; j++)
    {
      if(tdstransition_config.toc_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[i].dlchan_info[j].rb_id == rb_id)
      {
        tdstransition_config.toc_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[i].dlchan_info[j].rlc_id = rlc_ul_log_chl_id;
        log_chl_found = TRUE;
        break;
      }
    }

    /* The same logical channel can not be mapped to two TrCHs */
    if(log_chl_found)
    {
      break;
    }
  }

#ifdef FEATURE_TDSCDMA_HSUPA
  for(i=0; i < tdstransition_config.toc_ptr->mac_e_config.num_e_mac_d_flow; i++)
  {
    for(j=0; j < tdstransition_config.toc_ptr->mac_e_config.e_mac_d_flow_info[i].num_logch;j++)
    {
      if(tdstransition_config.toc_ptr->mac_e_config.e_mac_d_flow_info[i].e_ul_logch_info[j].rb_id == rb_id)
      {
        tdstransition_config.toc_ptr->mac_e_config.e_mac_d_flow_info[i].e_ul_logch_info[j].rlc_id = rlc_ul_log_chl_id;
        log_chl_found = TRUE;
        break;
      }
    }
    /* The same logical channel can not be mapped to two TrCHs */
    if(log_chl_found)
    {
      break;
    }
  }
#endif /*FEATURE_TDSCDMA_HSUPA*/

}

/*====================================================================
FUNCTION: tdsrrcllc_update_toc_dl_dch_info_with_log_chl_id()

DESCRIPTION:
  This function updates the MAC DL Info in TOC with LOgical Channel ID.
  The correct entity is selected based on the passed RB ID.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_update_toc_dl_dch_info_with_log_chl_id
(
  tdsrrc_RB_Identity  rb_id,
  rlc_lc_id_type  rlc_dl_log_chl_id
)
{
  int i, j;
  boolean  log_chl_found = FALSE;

  for(i=0; i<tdstransition_config.toc_ptr->mac_dl_parms.num_dchs; i++)
  {
    for(j=0; j<tdstransition_config.toc_ptr->mac_dl_parms.dch_info[i]->ndlchan;   j++)
    {
      if(tdstransition_config.toc_ptr->mac_dl_parms.dch_info[i]->dlchan_info[j].rb_id == rb_id)
      {
        tdstransition_config.toc_ptr->mac_dl_parms.dch_info[i]->dlchan_info[j].rlc_id = rlc_dl_log_chl_id;
        log_chl_found = TRUE;
        break;
      }
    }
    /* The same logical channel can not be mapped to two TrCHs */
    if(log_chl_found)
    {
      break;
    }
  } /* for(i=0; i<tdstransition_config.toc_ptr->mac_dl_parms.num_dchs; i++) */

}




/*====================================================================
FUNCTION: rrcllc_populate_mac_size_restriction_info()

DESCRIPTION:
  This function validates the information stored in the global
  rlc_size_restriction_info[] array and fills in the appropriate MAC
  data structures in ordered config.

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  MAC ordered config parameters relating to RLC size restriction are
  updated.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_populate_mac_rlc_size_restriction_info
(
  tdsordered_config_type *config_ptr,
  tdsrrc_state_e_type state
)
{
  tr_ch_id_type idx, max_idx;
  uint8 logch_idx;
  uint16 i;
  uint16 tf_idx;
  uint8 trch_idx;
  uint8 ndlchan;
  uint8 rlc_size_idx = 1;
  boolean is_rach = FALSE;

  tdsmac_ul_ded_logch_config_type *dlchan_info;
  tdsrrcllcoc_rlc_size_restriction_info_type *rec;
  tdsrrcllcoc_rlc_size_restriction_info_type *rrc_rlc_size_restriction_info;
  tdsmac_ul_dch_trch_config_type *dch_info;
  tdsmac_ul_rach_cctrch_config_type *rach_info;
  tdsmac_ul_tfs_type *tfs_info;
  tdsmac_ul_rlc_size_info_type *sz_inf;

  switch(state)
  {
    case TDSRRC_STATE_CELL_DCH:
      TDSRRC_MSG0(MSG_LEGACY_MED,"RLC size restrictions not supported in DCH");
      return SUCCESS;
    case TDSRRC_STATE_DISCONNECTED:
    case TDSRRC_STATE_CONNECTING:
    case TDSRRC_STATE_CELL_FACH:
      rrc_rlc_size_restriction_info =
        &config_ptr->rach_rlc_size_restriction_info;
      max_idx = 1;
      is_rach = TRUE;
      break;
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      rrc_rlc_size_restriction_info =
        &config_ptr->rach_rlc_size_restriction_info;
      max_idx = 1;
      is_rach = TRUE;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unsupported state %d.", config_ptr->rrc_state_indicator);
      return FAILURE;
  }

  for(idx = 0; idx < max_idx; idx++)
  {
    /* We will need the TFS info for this TrCH later in a couple of places. */
    tfs_info = &(config_ptr->mac_ul_parms.tfcs_ptr->tfs_info[idx]);

    rec = &(rrc_rlc_size_restriction_info[idx]);

    if(!rec->mac_update_needed)
    {
      continue;
    }
    rec->mac_update_needed = FALSE;

    switch(state)
    {
      case TDSRRC_STATE_CELL_DCH:
        trch_idx = tdsrrcllc_find_ul_dch_trch_idx(config_ptr, idx);
        if(trch_idx >= TDSUE_MAX_TRCH)
        {
          return FAILURE;
        }
        dch_info =
          &(config_ptr->ul_cctrch.dch_info.dch_info[trch_idx]);
        ndlchan = dch_info->ndlchan;
        dlchan_info = &(dch_info->dlchan_info[0]);
        break;
      case TDSRRC_STATE_DISCONNECTED:
      case TDSRRC_STATE_CONNECTING:
      case TDSRRC_STATE_CELL_FACH:
      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_URA_PCH:
        rach_info = &(config_ptr->ul_cctrch.rach_info);
        ndlchan = rach_info->ndlchan;
        dlchan_info = &(rach_info->dlchan_info[0]);
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unsupported state %d.", config_ptr->rrc_state_indicator);
        return FAILURE;
    }

    for(logch_idx = 0; logch_idx < ndlchan; logch_idx++)
    {
      sz_inf = &(dlchan_info[logch_idx].rlc_size_info);
      switch(rec->rlc_size_list[logch_idx].restriction_type)
      {
        case TDSRLC_SIZES_CONFIGURED:
          /* Skip this entry for now.  We will process it later as we loop
          through the logical_channel_list in this record. */
          break;
        case TDSRLC_SIZES_ALL:
          for(tf_idx = 0; tf_idx < tfs_info->ntf; tf_idx++)
          {
            /* The Logical Channel List for all TFs in this record must be set
               to CONFIGURED, since MAC is free to choose any of them. */
            if(rec->logical_channel_list[tf_idx].restriction_type !=
               TDSRLC_SIZES_CONFIGURED)
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad RLC size restr combo: tf_idx = %d", tf_idx);
              return FAILURE;
            }
          }
          sz_inf->all_tfi_valid = TRUE;
          sz_inf->ntfi_per_lc = 0;
          break;
        case TDSRLC_SIZES_EXPLICIT_LIST:
          sz_inf->all_tfi_valid = FALSE;
          sz_inf->ntfi_per_lc = 0;
          /* For TDD in this release there is a single TF within the RACH TFS */
          /* RB mapping save in rach_rlc_size_restriction_info.rlc_size_list */
          for (i = 0; i < rec->rlc_size_list[logch_idx].count; i++)
          {
            /* Validate the configuration.  The logical_channel_list[] entries are stored by
             * TF index(TDD, only one).  Therefore, logical_channel_list[0] must have a
             * restriction type of CONFIGURED, since it corresponds to this
             * rlc_size_list[] entry.
             */
            /* The rec->rlc_size_list[logch_idx].size_idx[] array contains a
             * list of RLC size indices that are valid.  We don't store or use
             * RLC size indices, so these indices must be converted to groups
             * of TF indices instead.  For example:

                 RLC size index     tb_size      ntblks     TF index
                       1              136           1          0
                       1              136           2          1
                       2              360           1          2
                       2              360           2          3

             * If RLC size index 2 were specified in the size_idx[] array, that
             * would mean that TF indices 2 and 3 are valid.
             */
            if(rec->logical_channel_list[0].restriction_type != TDSRLC_SIZES_CONFIGURED)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Bad RLC size restr combo");
              return FAILURE;
            }
            sz_inf->lc_tfi_list[sz_inf->ntfi_per_lc++] = 0;
          }

          if(sz_inf->ntfi_per_lc == 0 && !is_rach)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"No valid TF indices found for RLC size index %d", rlc_size_idx);
            return FAILURE;
          }
          break;
        case TDSRLC_SIZES_UNCONFIGURED:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unconfigured entry %d in LC list.  ntf = %d", logch_idx,
                   tfs_info->ntf);
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RLC size restr type %d",
              rec->rlc_size_list[logch_idx].restriction_type);
          return FAILURE;
      }
    }

    /* Now process the Logical Channel List for this record. */
    for(tf_idx = 0; tf_idx < tfs_info->ntf; tf_idx++)
    {
      switch(rec->logical_channel_list[tf_idx].restriction_type)
      {
        case TDSRLC_SIZES_CONFIGURED:
          /* Nothing to do here but validation.  Any actual processing should
             have already been taken care of when we processed the RLC Size
             List. */
          for(logch_idx = 0; logch_idx < ndlchan; logch_idx++)
          {
            if(rec->rlc_size_list[logch_idx].restriction_type ==
               TDSRLC_SIZES_CONFIGURED)
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad RLC size restr combo: logch_idx = %d", logch_idx);
              return FAILURE;
            }
          }
          break;
        case TDSRLC_SIZES_ALL:
          /* All logical channels mapped on this transport channel may use
             this tf_idx. */
          for(logch_idx = 0; logch_idx < ndlchan; logch_idx++)
          {
            /* If any of the RLC sizes for this transport channel is set to
               ALL, then all the RLC Size List entries for any logical channel
               mapped onto this transport channel must be set to CONFIGURED. */
            if(rec->rlc_size_list[logch_idx].restriction_type !=
               TDSRLC_SIZES_CONFIGURED)
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad RLC size restr combo: logch_idx = %d", logch_idx);
              return FAILURE;
            }

            /* Add this TF index to the list of allowed TF indices for this
               logical channel in the MAC configuration. */
            sz_inf = &(dlchan_info[logch_idx].rlc_size_info);
            if(sz_inf->all_tfi_valid)
            {
              sz_inf->all_tfi_valid = FALSE;
              sz_inf->ntfi_per_lc = 0;
            }
            sz_inf->lc_tfi_list[sz_inf->ntfi_per_lc++] = tf_idx;
          }
          break;
        case TDSRLC_SIZES_EXPLICIT_LIST:
          for(i = 0; i < rec->logical_channel_list[tf_idx].count; i++)
          {
            /* The list we are given is a list of RBs, so we must search
               through the logical channels mapped onto this transport channel
               for the one whose RB id matches. */
            for(logch_idx = 0; logch_idx < ndlchan; logch_idx++)
            {
              if(dlchan_info[logch_idx].rb_id ==
                 rec->logical_channel_list[tf_idx].rb_id[i])
              {
                break;
              }
            }

            /* The value of RLC Size List for this logical channel must be set
               to CONFIGURED. */
            if(rec->rlc_size_list[logch_idx].restriction_type !=
               TDSRLC_SIZES_CONFIGURED)
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Bad RLC size restr combo: logch_idx = %d",
                  logch_idx);
              return FAILURE;
            }

            /* Add this TF index to the list of allowed TF indices for this
               logical channel in the MAC configuration. */
            sz_inf = &(dlchan_info[logch_idx].rlc_size_info);
            if(sz_inf->all_tfi_valid)
            {
              sz_inf->all_tfi_valid = FALSE;
              sz_inf->ntfi_per_lc = 0;
            }
            sz_inf->lc_tfi_list[sz_inf->ntfi_per_lc++] = tf_idx;
          }
          break;
        case TDSRLC_SIZES_UNCONFIGURED:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Unconfigured entry %d in LC list.  ntf = %d", tf_idx,
                   tfs_info->ntf);
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RLC size restr type %d",
              rec->logical_channel_list[tf_idx].restriction_type);
          return FAILURE;
      }
    }
  }

  return SUCCESS;
} /* rrcllc_populate_mac_size_restriction_info */


/*============================================================================
FUNCTION: tdsrrcllc_register_for_reconfig_cnf()

DESCRIPTION:
  This function is called by SMC if Ordered Config is set for reconfiguration.
  This function sets a boolean to notify SMC when the reconfiguration is
  completed.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_register_for_reconfig_cnf(void)
{
  tdsordered_config.notify_smc = TRUE;
  return;
}


/*============================================================================
FUNCTION: tdsrrcllc_discard_oc()

DESCRIPTION:
  This function is called to discard the Ordered Config. This in effect
  sets the tdsconfig_ptr_in_use to CURRENT CONFIG.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_discard_oc(void)
{
  tdsconfig_ptr_in_use = TDSCURRENT_CONFIG;
}



/*============================================================================
FUNCTION: tdsrrcllc_return_freq_for_fach_pch()

DESCRIPTION:
  This function returns the frequency that UE is directed to when going to
  either one of Cell_FACH, Cell_PCH or URA_PCH

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE
  FAILURE indicates redirection frequency is NOT available.

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_return_freq_for_fach_pch(tdsrrc_freq_type  *freq)
{
  /* Return a Failure if Ordered Config NOT set for DCH to FACH
   * OR if Transition Config is somehow invalid.
   */
  if(
     (
      (tdsordered_config.set_status != TDSOC_SET_FOR_DCH_FACH_TRANS) &&
      (tdsordered_config.set_status != TDSOC_SET_FOR_DCH_CELL_PCH_TRANS) &&
      (tdsordered_config.set_status != TDSOC_SET_FOR_DCH_URA_PCH_TRANS)
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      &&
      (tdsordered_config.set_status != TDSOC_SET_FOR_FACH_CELL_PCH_TRANS) &&
      (tdsordered_config.set_status != TDSOC_SET_FOR_FACH_URA_PCH_TRANS)&&
      (tdsordered_config.set_status != TDSOC_SET_FOR_CELL_FACH)
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

     ) ||
     (tdstransition_config.toc_usage == TDSTOC_INVALID))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No freq to pickup ");
    return(FAILURE);
  }
  else
  {
    /* Check if the the directed frequency info is present in OTA message 
     *in  DCH-> FACH,or FACH->PCH or DCH->PCH or FACH->FACH.
     * This info is present in  TOC */
    /* non-DCH state,  not care w_freq */
    if(tdstransition_config.toc_ptr->p_freq_present == FALSE)
    {
      /* Destination freq not present. Treat the current freq
      as destination frequency*/
      *freq = tdsrrcccm_get_curr_camped_freq();
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"using current camped freq:%d", *freq);
      return(SUCCESS);
    }
    else
    {
      /* this function is for fach/pch, so use p_freq */
      *freq = tdstransition_config.toc_ptr->p_freq;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"using freq from msg, freq:%d", *freq);
      return(SUCCESS);
    }
  }
} /* tdsrrcllc_return_freq_for_fach_pch */


/*============================================================================
FUNCTION: tdsrrcllc_return_psc_for_fach_pch()

DESCRIPTION:
  This function returns the Primary Scvrambling Code that UE is directed to
  when going from to either one of Cell_FACH, Cell_PCH or URA_PCH

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE
  FAILURE indicates redirection CPID is NOT available.

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_return_psc_for_fach_pch(tdsrrc_cpid_type  *cpid)
{
  /* Return a Failure if Ordered Config NOT set for DCH to FACH */
  if ((tdsordered_config.set_status != TDSOC_SET_FOR_DCH_FACH_TRANS)&&
      (tdsordered_config.set_status != TDSOC_SET_FOR_DCH_CELL_PCH_TRANS) &&
      (tdsordered_config.set_status != TDSOC_SET_FOR_DCH_URA_PCH_TRANS)
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
      &&
      (tdsordered_config.set_status != TDSOC_SET_FOR_FACH_CELL_PCH_TRANS) &&
      (tdsordered_config.set_status != TDSOC_SET_FOR_FACH_URA_PCH_TRANS)&&
      (tdsordered_config.set_status != TDSOC_SET_FOR_CELL_FACH)
#endif /*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

      )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No CPID to pickup ");
    return(FAILURE);
  }
  else
  {
     /* Check if the the directed CPID is present in OTA message 
      *  in  DCH-> FACH,or FACH->PCH or DCH->PCH or FACH->FACH.
      * This info is present in  TOC 
      */
    if(tdstransition_config.toc_ptr->cpid_present == FALSE)
    {
      /* Destination freq not present */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No CPID in TOC ");
      return(FAILURE);
    }
    else
    {
      *cpid = tdstransition_config.toc_ptr->cell_param_id;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CPID in TOC %d ", *cpid);
      return(SUCCESS);
    }
  }
} /* tdsrrcllc_return_psc_for_fach_pch */


/*====================================================================
FUNCTION: tdsrrcllc_set_ordered_config_for_rl_failure

DESCRIPTION:
  This is called when the TDSORDERED_CONFIG variable needs to be set.
  It checks to see whether or not it is set or not, and returns
  the status of the check.

  It also takes a pointer to the asn1 decoded message stream and
  places it into the tdsordered_config database.

  The procedure name is required so that the message asn1 decoded
  message can be properly typecast so as to populate the
  TDSORDERED_CONFIG database.

DEPENDENCIES:
  CELL_DCH is the only supported state.

RETURN VALUE:
  tdsrrcllc_oc_status_e_type - indicates whether the TDSORDERED_CONFIG
                            variable could be successfully set or
                            what the failure was.

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type tdsrrcllc_set_ordered_config_for_rl_failure
(
  /*
  * This variable indicates the PDU number as defined in the ASN.1
  * header file. This information can be used to map the to correct
  * logical channel received on and then down to the message type.
  * The message contents are in msg_ptr, and need to be typecasted.
  */
  tdsrrc_proc_e_type  proc_id
)
{
  tdsrrcllc_oc_status_e_type  oc_status;

  /* Check to see if TDSORDERED_CONFIG is set */
  if (tdsordered_config.set_status != TDSOC_NOT_SET)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Incompatible Simultaneous Reconfig");
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"OC status %d, proc %d",
                tdsordered_config.set_status, tdsordered_config.set_by_proc);
    return (TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG);
  }

  /*
  * If made it this far, check the state to determine how to
  * setup TDSORDERED_CONFIG
  */
    /* This implies Ordered Config is not being used - update the
     * config pointer in use
     */

  tdscurrent_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_FACH;
  tdsrrcllc_copy_cc_to_oc();

  /* Ordered Config now set */
  tdsordered_config.set_status = TDSOC_SET_FOR_DCH_FACH_TRANS;
  /* Now copy OC to TOC */
  tdsrrcllc_copy_oc_to_toc();
  /* Inducate that TOC could be used both for DCH and FACH in the future */
  tdstransition_config.toc_usage = TDSTOC_FOR_DCH_AND_FACH;
  oc_status = TDSORDERED_CONFIG_SET;

  /* Update the Procedure ID */
  tdsordered_config.set_by_proc = proc_id;
  /* Update OC process state */
  tdsordered_config.process_state = TDSLL_NOT_CONFIG_WITH_OC;
  /* This variable indicates that SMC needs to be notified when
   * a reconfiguration is completed.
  */
  tdsordered_config.notify_smc = FALSE;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"OC set by proc %d", proc_id);

  return (oc_status);

} /* end tdsrrcllc_set_ordered_config_for_rl_failure() */

/*===========================================================================

FUNCTION tdsrrcllc_get_dl_amr_mode

DESCRIPTION

 Given the TB size, this function returns the AMR mode associated with this 
 TB size

DEPENDENCIES
 None

RETURN VALUE

 uint32: AMR mode

SIDE EFFECTS

 None

===========================================================================*/
uint32 tdsrrcllc_get_dl_amr_mode(uint32 tb_size)
{
  uint32 amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;

  switch(tb_size)
  {
  case TDSWVS_AMR_MODE_122_CLASS_A_BIT_SIZE:
  case TDSWVS_AMR_MODE_122_CLASS_B_BIT_SIZE:
  case TDSWVS_AMR_MODE_122_CLASS_C_BIT_SIZE:
    amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_1220;
    break;
 
  case TDSWVS_AMR_MODE_102_CLASS_A_BIT_SIZE:
  case TDSWVS_AMR_MODE_102_CLASS_B_BIT_SIZE:
  case TDSWVS_AMR_MODE_102_CLASS_C_BIT_SIZE:
    amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_1020;
    break;
  
  case TDSWVS_AMR_MODE_795_CLASS_A_BIT_SIZE:
  case TDSWVS_AMR_MODE_795_CLASS_B_BIT_SIZE:
    amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0795;
    break;
   
  case TDSWVS_AMR_MODE_740_CLASS_A_BIT_SIZE:
  case TDSWVS_AMR_MODE_740_CLASS_B_BIT_SIZE:
    amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0740;
    break;
  
  case TDSWVS_AMR_MODE_670_CLASS_A_BIT_SIZE:
  case TDSWVS_AMR_MODE_670_CLASS_B_BIT_SIZE:
    amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0670;
    break;

  case TDSWVS_AMR_MODE_590_CLASS_A_BIT_SIZE:
  case TDSWVS_AMR_MODE_590_CLASS_B_BIT_SIZE:
    amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0590;
    break;
    
  case TDSWVS_AMR_MODE_515_CLASS_A_BIT_SIZE:
  case TDSWVS_AMR_MODE_515_CLASS_B_BIT_SIZE:
    amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0515;
    break;

  case TDSWVS_AMR_MODE_475_CLASS_A_BIT_SIZE:
  case TDSWVS_AMR_MODE_475_CLASS_B_BIT_SIZE:
    amr_mode = TDSCDMA_IVOCAMR_CODEC_MODE_0475;
    break;

  default:
    amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;
    break;
  }

  return amr_mode;
} /* tdsrrcllc_get_dl_amr_mode */

/*===========================================================================

FUNCTION tdsrrcllc_get_dl_amr_mode_for_rb

DESCRIPTION

 Given RB Id, returns the AMR mode associated with it

DEPENDENCIES
 None

RETURN VALUE

 uint32: AMR mode

SIDE EFFECTS

 None

===========================================================================*/
uint32 tdsrrcllc_get_dl_amr_mode_for_rb
(
  tdsrrc_RB_Identity  rb_id, 
  tdsordered_config_type *config_ptr
)
{
  uint32 trch_count = 0;
  uint32 dlch_count = 0;
  uint32 index_ctfc = 0;
  uint8 this_trch_tf_in_tfc = 0;
  uint32 dl_amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF, new_dl_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;
  boolean rate_initialised= FALSE;

  for(
      trch_count = 0;
      (trch_count < config_ptr->l1_dl_cctrch_parms.num_trch);
      trch_count ++
     )
  {
    for(
        dlch_count = 0;
        (dlch_count < config_ptr->mac_dl_parms.
                      dch_info[trch_count]->ndlchan);
        dlch_count ++
       )
    {
      if(config_ptr->mac_dl_parms.dch_info[trch_count]->
          dlchan_info[dlch_count].rb_id == rb_id)
      {
        for (index_ctfc = 0;
             index_ctfc < config_ptr->dl_ctfc.num_ctfc_entry;
             index_ctfc++)
        { 
          /* The check against validity of CTFC state is particularly useful where multirate 
             configuration is given but one AMR rate is marked as allowed through
             CTFCs */
          if(config_ptr->dl_ctfc.ctfc_entry_data[index_ctfc].state == TDSALLOWED)
          {
            this_trch_tf_in_tfc = config_ptr->dl_ctfc.ctfc_entry_data[index_ctfc].tfi_table[trch_count];
            /* Only if no. of TBs is greater than ZERO, we run through CCRTrCH info to get the right TF */
            if(config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->tf_info_array[this_trch_tf_in_tfc]->num_tb > 0)
            {
              new_dl_mode = tdsrrcllc_get_dl_amr_mode(
                  config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->tf_info_array[this_trch_tf_in_tfc]->tb_size);
              
              /* Since the AMR mode enum has high value for TDSINVALID mode,
                 the following logic ensures that right values are picked up
                 after finding first correct AMR mode.
                 At the end of the loop, it is expected that higher mode is
                 chosen in case of multirate AMR configuration.
              */
              if( (new_dl_mode != TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF) &&(rate_initialised == FALSE))
              {
                dl_amr_mode = new_dl_mode;
                rate_initialised = TRUE;
              }
              
              if((new_dl_mode != TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF) && (new_dl_mode > dl_amr_mode))
              {
                dl_amr_mode = new_dl_mode;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"overwriting with=%d",dl_amr_mode);        
              }
            } 
          } 
        }
        return dl_amr_mode; /* No need to iterate as intended RB ID is found */
      }
    } 
  } 
  return dl_amr_mode;
} /* tdsrrcllc_get_dl_amr_mode_for_rb */


/*===========================================================================

FUNCTION tdsrrcllc_get_cm_hsdpa_nv_item

DESCRIPTION
  This  function gets NV item NV_HSDPA_COMPRESSED_MODE_ENABLED_I and stores it in the global variable

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcllc_get_cm_hsdpa_nv_item
(
void
)
{
  uint8 nv_hsdpa_cm_enabled = TRUE;

  /* read HSDPA + CM Variable from NV */
  if ( NV_DONE_S ==  (tdsrrc_get_nv( NV_HSDPA_COMPRESSED_MODE_ENABLED_I, 
                                  (nv_item_type *) &nv_hsdpa_cm_enabled)))
  {
    if (nv_hsdpa_cm_enabled == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NVHSDPACM disabled");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"NVHSDPACM ENABLED");
    }
  }
  else
  { 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"NVHSDPACM not read.Force enabled");
  }
}


/*===========================================================================

FUNCTION tdsrrcllc_set_hrnti_status

DESCRIPTION

 Sets HRNTI status in the passed pointer

DEPENDENCIES
 None

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcllc_set_hrnti_status
(
  tdsordered_config_type* config_ptr,
  tdsrrc_hsdpa_hrnti_enum_type hrnti_status
)
{
  config_ptr->hrnti_status = hrnti_status;
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
  {
    TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:HRNTIstatus after setting %d", config_ptr->hrnti_status);
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_check_if_rb_mapped_on_dflow()

DESCRIPTION:
  This function checks if a RB is mapped to a DFLOW in the passed
  config ptr.

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE if RB is found to be mapped on a DFLOW else FALSE

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_check_if_rb_mapped_on_dflow
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_RB_Identity  rb_id
)
{
  uint8 idx =0, idz =0;

  for (idx = 0; idx < config_ptr->mac_dl_parms.num_dflow; idx++)
  {
    if ((config_ptr->dl_dflow_index_in_use[idx] == TRUE) &&
      (config_ptr->mac_dflow_info[idx].mac_dflow_id != TDSINVALID_DFLOW_ID))
    {

      for(idz = 0; idz < UE_MAX_LOGCHAN_PER_DFLOW; idz++)
      {
        if ((config_ptr->mac_dflow_info[idx].dlchan_info[idz].rb_id == rb_id) &&
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
#if 0
           ((config_ptr->mac_dflow_info[idx].dlchan_info[idz].chan_type == UE_LOGCHAN_DTCH) ||
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
          (config_ptr->mac_dflow_info[idx].dlchan_info[idz].chan_type == UE_LOGCHAN_DTCH) &&
          (config_ptr->mac_dflow_info[idx].dlchan_info[idz].rlc_mode != UE_MODE_TRANSPARENT))
        {
          TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:Found RB Id %d mapped on DFLOW %d",rb_id,
            config_ptr->mac_dflow_info[idx].mac_dflow_id);
          return TRUE;
        }

      }
    }
  }
  return FALSE;
}

/*====================================================================
FUNCTION: tdsrrcllc_init_reestablish_entity()

DESCRIPTION:
  This function re-initializes Re-establish entity with in OC/CC.

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE if RLC Size Changed

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_reestablish_entity
(
  tdsrrcllc_reestab_config_e_type config_type
)
{
  uint8 ix = 0;
  if(config_type == TDSORDERED_CONFIG_ONLY )
  {
    for(ix = 0; ix < TDSUE_MAX_AM_ENTITY; ix++)
    {
      tdsordered_config_ptr->am_config[ix].reestab_entity =  TDSRLC_RE_ESTABLISH_NONE;
#ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
      tdsordered_config_ptr->rlc_one_sided_reest[ix] = FALSE;
#endif
    }
  }
  else if(config_type == TDSCURRENT_CONFIG_ONLY )
  {
    for(ix = 0; ix < TDSUE_MAX_AM_ENTITY; ix++)
    {
      tdscurrent_config_ptr->am_config[ix].reestab_entity = TDSRLC_RE_ESTABLISH_NONE;
#ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
      tdscurrent_config_ptr->rlc_one_sided_reest[ix] = FALSE;
#endif
    }
  }
  else
  {
    for(ix = 0; ix < TDSUE_MAX_AM_ENTITY; ix++)
    {
      tdsordered_config_ptr->am_config[ix].reestab_entity = 
       tdscurrent_config_ptr->am_config[ix].reestab_entity = TDSRLC_RE_ESTABLISH_NONE;
#ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
      tdscurrent_config_ptr->rlc_one_sided_reest[ix] = FALSE;
      tdsordered_config_ptr->rlc_one_sided_reest[ix] = FALSE;
#endif
    }
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_compare_rlc_size_change()

DESCRIPTION:
  This function compares the new size with old RLC Size. At present,
  we are comparing with 24 bits. [16 bits URNTI-CRNTI
  difference, 6 bits difference between channel mapping
  i.e CCCH/DCCH/DTCH over FACH. 2 bits difference for C/T field]
  This [24 bits difference comparison] would be removed later. TBD

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE if RLC Size Changed

SIDE EFFECTS:
  None.
====================================================================*/

boolean tdsrrcllc_compare_rlc_size_change(uint32 old_size, uint32 new_size)
{
  if (old_size == new_size)
  {
    return FALSE;
  }
  else if (old_size > new_size)
  {
    /* Check if new size is within 24 bits of the old size */
    if ((old_size - new_size) <= 24)
    {
      return FALSE;
    }
    else
    {
      return TRUE;
    }
  }
  else 
  {
    if ((new_size - old_size) <= 24)
    {
      return FALSE;
    }
    else
    {
      return TRUE;
    }
  }
  
}


#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
/*====================================================================
FUNCTION: tdsrrcllc_update_dpl_from_sib7_info_in_fach

DESCRIPTION:
  This is called when the dynamic persistence value
  needs to be updated with the latest value received by SIB7.

DEPENDENCIES:
  None

RETURN VALUE:
  FAILURE -> If new pval sent to MAC
  SUCCESS -> Otherwise.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_update_dpl_from_sib7_info_in_fach
(
  tdsrrc_SysInfoType7    *sib7_ptr
)
{
    tdsrrc_SysInfoType5 *sib5_ptr = NULL;    
    tdsrrc_DynamicPersistenceLevel dpl = 0;             /* DPL used to calculate the ASC Establishment */    
    uint32 psf[MAX_ASC_PERSIST];                        /* Persistence Scaling factors used to calculate the ASC Establishment */    
    tdsrrc_DynamicPersistenceLevelList *dpl_ptr = NULL; /* Pointer to loop through the DPL values */
    uint8 dpl_cnt = 0;                                  /* Counter to loop through the DPL values */
    tdsl2_ul_cmd_type  *l2_ul_cmd_ptr = NULL;           /* Pointer which carries info about the command */
    tdsordered_config_type *current_cfg_ptr = NULL;     /* Current config pointer in use */
    uint32  pval = 0;                                   /* Persistence value */
    uint8 asc_idx = 0;                                  /* Counter to loop through the PSFs */

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Extracting Persitence Value from sib7.");

    /* We have to read SIB 5 before we can get the Dynamic
       Persistence level so we know whether to get the values for SIB 5 or
       SIB 6.
    */

    /************************/
    /* Get SIB 5 data next  */
    /************************/
    sib5_ptr = (tdsrrc_SysInfoType5 *)
               tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB5);
    /* We should have all SIBs read by now as we are in FACH state.
       This check is not required but just to be on safe side */
    if (sib5_ptr == NULL) 
    {
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB 5 not yet in SIB data base");
       return FAILURE;
    }
    else
    { 
      /* Get the current state */
      tdsrrc_state_e_type current_state = tdsrrc_get_state();
  
      boolean use_conn_mode_sibs = tdsrrcllc_use_connected_mode_sibs(current_state);
  
      /* use SIB6 only if SIB5 has sib6indicator TRUE and RRC is in connected state */
      if(use_conn_mode_sibs && (sib5_ptr->sib6indicator == TRUE))
      {
        tdsrrc_SysInfoType6    *sib6_ptr = NULL;

        sib6_ptr = (tdsrrc_SysInfoType6 *)
                   tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB6);

        /* Look for PRACH system info list in SIB6 */
        if (sib6_ptr && sib6_ptr->m.prach_SystemInformationListPresent &&
              (sib6_ptr->m.v4b0NonCriticalExtensionsPresent &&
               sib6_ptr->v4b0NonCriticalExtensions.m.sysInfoType6_v4b0extPresent&&
               sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.m.prach_SystemInformationList_LCR_r4Present))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Using SIB 6 for DPL.");
          /* Get the persistence level from SIB 6 */
          dpl_ptr = &sib7_ptr->prach_Information_SIB6_List;
        }
        else
        {
           /* SIB 6 may not be present. That is acceptable */
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 6 not in SIB data base or PRACH info is not present in SIB6; Using SIB5 for DPL.");
           /* Get the persistence level from SIB 5 */
           dpl_ptr = &sib7_ptr->prach_Information_SIB5_List;
        }
      }
      else /* use SIB5 for DPL */
      {
        /* SIB 6 may not be present. That is acceptable */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 6 not in SIB data base; Using SIB5 for DPL.");
        /* Get the persistence level from SIB 5 */
        dpl_ptr = &sib7_ptr->prach_Information_SIB5_List;
      }

      while((dpl_cnt < dpl_ptr->n ) && (dpl_cnt < UE_MAX_PRACH))
      {
        dpl = dpl_ptr->elem[dpl_cnt];
        dpl_cnt++;
      }

      if (dpl_cnt == 0)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"DPL count (nASC) is 0.");
        return FAILURE;
      }
    }

    /* send command to MAC */

    /* get PSF from current config */
    current_cfg_ptr = tdsrrcllc_get_config_ptr_in_use();
    memscpy(psf, sizeof(uint32)*MAX_ASC_PERSIST,current_cfg_ptr->psf, sizeof(uint32)*MAX_ASC_PERSIST);

    /* Get L2 DL command buffer */
    if ((l2_ul_cmd_ptr = tdsl2_ul_get_cmd_buf()) == NULL)
  {
      ERR_FATAL("NULL CMD Ptr when sending TDSMAC_UL_PERSISTENCE_VAL_CHANGE_IND ",0,0,0);
    }

    /* Initialize the LLC command header first */
    l2_ul_cmd_ptr->cmd_hdr.cmd_id= TDSMAC_UL_PERSISTENCE_VAL_CHANGE_IND;

    /* Indicate the number of Access Service Classes supported */
    l2_ul_cmd_ptr->cmd_data.persistence_info.nASC = dpl_cnt;

    /* Setup the Persistence values for all ASCs */
    for(asc_idx = 0; ((asc_idx < dpl_cnt)&& (asc_idx < MAX_ASC)); asc_idx++)
    {
      switch(asc_idx)
      {
        case 0:
          pval = TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR;
          break;
        case 1:
          pval = (TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR >>
                  (dpl - 1));
          break;
        default:
          if((asc_idx - 2) >=0)
          {
            pval = ((TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR >>
                    (dpl - 1)) * psf[asc_idx - 2])/TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL; 
                    /*lint !e676 */
                    /* lint mistakenly believes there is a possibility for a
                       negative subscript here. */
          }
          break;
      }
      /* Note that since there is a multiplication by the Persistence Scaling
         Factor in the above calculation, the actual multiplier for the
         Persistence value is now TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR * 10.
         Now we need to make the value fit into 16 bits, so we divide it
         by ((TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR * 10) / 0xffff) + 1 before
         sending it on to MAC.  (The 1 is added to make sure we round up
         instead of truncating.) */
  {
        l2_ul_cmd_ptr->cmd_data.persistence_info.asc[asc_idx].pval = pval;
      }
  }

    /* Send the command to L2 UL task */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending pval update indication to MAC");
    tdsl2_ul_put_cmd( l2_ul_cmd_ptr );

    return SUCCESS;
} /* tdsrrcllc_update_configs_with_sib7_info_in_fach */
#endif /*FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH*/
/*====================================================================
FUNCTION: tdsrrcllc_get_dch_fach_act_time_in_ms()

DESCRIPTION:
  This function returns activation time in ms for dch->fach transitions.
  returns 0 if no act time or if this function is called during non dch-fach
  transitions.

DEPENDENCIES:
  None

RETURN VALUE:
  uin16.

SIDE EFFECTS:
  None.
====================================================================*/
uint16 tdsrrcllc_get_dch_fach_act_time_in_ms(void)
{
  uint16 ans = 0;
  uint16 curr_cfn = 0;
  
  if ((TDSOC_SET_FOR_DCH_FACH_TRANS == tdsrrcllc_get_ordered_config_status()) &&
      ((tdstransition_config.toc_ptr->activation_time != TDSRRCLLC_ACTIVATION_TIME_NOW) &&
       (tdstransition_config.toc_ptr->act_time_type == TDSL1_ACTIVATION_TIME_CFN))) 
  {
    curr_cfn =tdsseq_get_cfn();
    /*convert cfn to ms*/ 
    ans = (((256+(tdstransition_config.toc_ptr->activation_time )-(curr_cfn)) & 0xFF) *10);
  }
  return ans;
}

/*====================================================================
FUNCTION: tdsrrcllc_dl_dchs_not_standalone()

DESCRIPTION:
  This function returns TRUE if there's at least one DL DCH TrCH that's not standalone. 
  Essentially, it means that a DL DCH is in use.

DEPENDENCIES:
  None

RETURN VALUE:
  boolean

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_dl_dchs_not_standalone
(
  void
)
{
  uint32 cnt,trch_idx;

  for(trch_idx=0;trch_idx< TDSUE_MAX_TRCH;trch_idx++)
  {
    if((tdsordered_config_ptr->dl_dch_trch_info[trch_idx].dch_present) &&
      (tdsordered_config_ptr->dl_dch_trch_idx_in_use[trch_idx] == TRUE) )
    {
      for (cnt = 0; cnt < tdsordered_config_ptr->dl_dch_info[trch_idx].ndlchan;cnt++) 
      {      
        if (tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[cnt].rb_id != 0)
        {
          /* This IE must have already been provided as part of mapping info. Just return success */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"DL Trch mapped to RB %d ",tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[cnt].rb_id);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

#ifdef FEATURE_TDSCDMA_HSUPA
/*====================================================================
FUNCTION: tdsrrcllc_ul_dchs_not_standalone()

DESCRIPTION:
  This function returns TRUE if there's at least one UL DCH TrCH that's not standalone. 
  Essentially, it means that a UL DCH is in use.

DEPENDENCIES:
  None

RETURN VALUE:
  boolean

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_ul_dchs_not_standalone
(
  void
)
{
  uint32 cnt,trch_idx;

  for(trch_idx=0;trch_idx< TDSUE_MAX_TRCH;trch_idx++)
  {
    if((tdsordered_config_ptr->ul_dch_trch_info[trch_idx].dch_present) &&
        (tdsordered_config_ptr->ul_dch_trch_idx_in_use[trch_idx] == TRUE))
    {
      for (cnt = 0; cnt < tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan; cnt++) 
      {      
        if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnt].rb_id != 0)
        {
          /* This IE must have already been provided as part of mapping info. Just return success */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"UL Trch mapped to RB %d ",tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnt].rb_id);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}
#endif /*FEATURE_TDSCDMA_HSUPA*/

/*====================================================================
FUNCTION: tdsrrcllc_init_tfcs_needed_flags()

DESCRIPTION:
  This functions initializes flags tdsul_tfcs_needed and tdsdl_tfcs_needed.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_tfcs_needed_flags(void)
{
  tdsul_tfcs_needed = FALSE;
  tdsdl_tfcs_needed = FALSE;
  tdsul_tfcs_rcvd = FALSE;
  tdsdl_tfcs_rcvd = FALSE;
}
/*====================================================================
FUNCTION: tdsrrcllc_is_tfcs_provided_on_add_delete_trch()

DESCRIPTION:
  This function returns TRUE if UE was expecting to receive some TFCS
  info, but never received it.  Else returns FALSE.  Only enforce this
  rule is DCH and relax this rule in other states.

DEPENDENCIES:
  None

RETURN VALUE:
  boolean

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_is_tfcs_provided_on_add_delete_trch(tdsrrc_RRC_StateIndicator dest_state)
{
  tdsordered_config_type  *config_ptr = tdsordered_config_ptr;

  if (dest_state == tdsrrc_RRC_StateIndicator_cell_DCH) 
  {
    if((tdsdl_tfcs_needed == TRUE) && (tdsdl_tfcs_rcvd == FALSE) &&
       (config_ptr->mac_dl_parms.num_dchs > 0))
    {
      /*The following check makes sure that there's not any standalone DL DCH*/
      if(tdsrrcllc_dl_dchs_not_standalone())
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"DL TFCS info not present: Invalid %d %d",tdsdl_tfcs_needed,tdsdl_tfcs_rcvd);
        return FALSE;
      }
    }
    else if((tdsul_tfcs_needed == TRUE) && (tdsul_tfcs_rcvd == FALSE) &&
            (config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs > 0))
    {
#ifdef FEATURE_TDSCDMA_HSUPA
      if(tdsrrcllc_ul_dchs_not_standalone())
#endif /*FEATURE_TDSCDMA_HSUPA*/
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"UL TFCS info not present: Invalid %d %d",tdsul_tfcs_needed, tdsul_tfcs_rcvd);
        return FALSE;
      }
    }
  }
  if(tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH || dest_state == tdsrrc_RRC_StateIndicator_cell_DCH)
  {
    if(config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs == 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initing UL TFCS");
      tdsrrcllc_init_ul_tfi_table(config_ptr);
      config_ptr->mac_ul_parms.new_tfcs_list_present = TRUE;
    }
    if(config_ptr->mac_dl_parms.num_dchs == 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initing DL TFCS");
      tdsrrcllc_init_dl_ctfcs(config_ptr);
    }
  }    
  return TRUE;
}

/*===========================================================================

FUNCTION tdsrrc_change_codec_configuration

DESCRIPTION 
 This function is called if there is a codec change in a ongoing call.
 In this case, the old codec is released. And then the TDS Voice Adapter 
 is configured with the new codec parameters. And then the new is acquired.
 
DEPENDENCIES
 None.

RETURN VALUE
 None

===========================================================================*/
void tdsrrc_change_codec_configuration
(
  uint8 rab_idx
)
{
  uint32 current_mode;
  tdsrrcllc_amr_mapping_info_type amr_info = {0};

  current_mode = tdsrrc_get_current_codec_mode();

  if( current_mode == TDSCDMA_IVOCODER_ID_AMRWB)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"change config to AMR_WB");
    amr_info.mode_type = TDSCDMA_IVOCODER_ID_AMRWB;
    (void) tdsrrcllc_get_amr_wb_mapping(&(amr_info));
  }
  else
  {    
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"change config to AMR");
    amr_info.mode_type = TDSCDMA_IVOCODER_ID_AMR;
    (void) tdsrrcllc_get_amr_mapping(&(amr_info)); 
  }

  tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_STOP);
  tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE);
  tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_SCR_MODE);
  tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS);
  tdsrrc_configure_voice_adapter(&amr_info, TRUE, TRUE, TDSCDMA_IVOICE_EVENT_REQUEST_START);
}

/*===========================================================================

FUNCTION tdsrrcllcoc_update_oc_orderd_codec_mode

DESCRIPTION This function updates the OC with the codec mode which needs to be
                      applied.
 
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcllcoc_update_oc_orderd_codec_mode
(
  uint32 codec_mode_ordered
)
{
  tdsordered_config_ptr->ul_tfcs.orderd_codec_mode = codec_mode_ordered;
}


#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION tdsrrcllcoc_update_toc_with_oc_from_fach


DESCRIPTION This function updates the TOC with the OC value. 
            It also sets the TOC.
            It also handles the case where TOC originally had some DCH info.
 
DEPENDENCIES
  None.

RETURN VALUE
 None

===========================================================================*/
void tdsrrcllcoc_update_toc_with_oc_from_fach
(
  void
)
{
  tdsrrcllc_toc_usage_e_type toc_usage_type ;
  tdsmac_rnti_config_type      backup_rnti_info;
  backup_rnti_info.rnti_valid = TDSNO_RNTI_VALID;
 /* Now copy OC to TOC */
  if( tdstransition_config.toc_usage == TDSTOC_INVALID )
  {
    toc_usage_type = TDSTOC_FOR_FACH_AND_PCH;
  }
  else
  {
    toc_usage_type =TDSTOC_FOR_FACH_AND_PCH_WITH_DCH_INFO;
  }
  
  /* Since TOC had some valid DCH info, backup the DCH info into OC 
   * So that when TOC is updated with OC, the existing TOC contents are also available.
   */     
  if(tdstransition_config.toc_usage != TDSTOC_INVALID)
  {
    //backup_rnti_info from OC,as TOC may no have proper value
    if(tdsordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid != TDSNO_RNTI_VALID)
    {
      backup_rnti_info = tdsordered_config_ptr->mac_dl_parms.rnti_info;
    }  

    tdsrrcllc_copy_trch_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);
    /* If OC has valid RNTIs, restore the RNTIs back to OC
       TOC RNTIs should be used only if OC doesn't have valid RNTIs */
    if(backup_rnti_info.rnti_valid != TDSNO_RNTI_VALID)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RNTIinfo:Restore RNTI into OC");
      tdsordered_config_ptr->mac_dl_parms.rnti_info = backup_rnti_info;
      tdsordered_config_ptr->mac_ul_parms.rnti_info = backup_rnti_info;
    }

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    tdsrrcllc_copy_srb5_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

    if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
    {
      /* Copy HARQ Information to L1 */
      tdsrrcllc_copy_harq_info(tdsordered_config_ptr, tdstransition_config.toc_ptr);
    }

    
  }
  
  /* Now copy OC to TOC */
  tdsrrcllc_copy_oc_to_toc();  

  tdstransition_config.toc_ptr->rlc_ul_tm_parms.nchan = 0;
  tdstransition_config.toc_ptr->rlc_dl_tm_parms.nchan = 0;	

  /* Indicate that TOC is used for FACH->FACH or FACH->PCH transition  */
  tdstransition_config.toc_usage = toc_usage_type;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TOC Usage set as %d",toc_usage_type);
}

#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/


/*============================================================================
FUNCTION: tdsrrcllc_check_if_new_tfcs_present()

DESCRIPTION:
 This Function returns TRUE if UL TFCs is set in ordered config,
 FALSE otherwise

DEPENDENCIES:
  None

RETURN VALUE:
TRUE    if new UL TFCS is present in ordered config
FALSE  otherwise
=============================================================================*/
boolean tdsrrcllc_check_if_new_tfcs_present
(
  void
)
{
  return tdsordered_config_ptr->mac_ul_parms.new_tfcs_list_present;
}


/*==========================================================
FUNCTION  :   RRCLLC_GET_CONFIG_PTR_IN_USE()

DESCRIPTION : This function returns the config ptr in use.

DEPENDENCIES: None.

RETURN VALUE: See description.

SIDE EFFECTS: None.
============================================================*/
tdsordered_config_type *tdsrrcllc_get_config_ptr_in_use(void)
{

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"config ptr in use 0x%x [0:OC/1:CC/2:TOC]",
           tdsconfig_ptr_in_use);
  if (tdsconfig_ptr_in_use == TDSORDERED_CONFIG) 
  {
    return tdsordered_config_ptr;
  }
  else if(tdsconfig_ptr_in_use == TDSCURRENT_CONFIG)
  {
    return tdscurrent_config_ptr;
  }
  else if (tdsconfig_ptr_in_use == TDSTRANSITION_CONFIG) 
  {
    return tdstransition_config.toc_ptr;
  }

  return NULL;
}/*tdsrrcllc_get_config_ptr_in_use*/

/*====================================================================
FUNCTION: tdsrrcllc_get_rach_cctrch_ptr()

DESCRIPTION:
  This function returns rach cctrch ptr from current config

DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:
  None.

=====================================================================*/
tdsmac_ul_rach_cctrch_config_type* tdsrrcllc_get_rach_cctrch_ptr
(
  void
)
{
  return (&tdscurrent_config_ptr->mac_ul_parms.cctrch_ptr->rach_info);
}

/*===========================================================================

FUNCTION tdsrrc_deconfigure_voice_adapter

DESCRIPTION
  This function deconfigures TDS Voice Adapter for deregistration of UL / DL 
  AMR RLC queues. It also resets the RRC global variable tdsrrc_amr_info.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrrc_deconfigure_voice_adapter
(
  void
)
{
  tdscdma_ivoice_event_set_logical_channels_t rrc_amr_log_chnl_info = {0};

  rrc_amr_log_chnl_info.direction = TRUE;
  rrc_amr_log_chnl_info.lc_class_a = 0;
  rrc_amr_log_chnl_info.lc_class_b = 0;
  rrc_amr_log_chnl_info.lc_class_c = 0;
  rrc_amr_log_chnl_info.class_type = TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_NONE;

  if(tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func != NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Deconfigure the vocoder in UL/DL.");

    /* Deconfigure UL */
    tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
      tdsrrc_get_as_id(),
      TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS,
      &rrc_amr_log_chnl_info,
      sizeof(rrc_amr_log_chnl_info)
      );

    /* Deconfigure DL */
    rrc_amr_log_chnl_info.direction = FALSE;
    tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
      tdsrrc_get_as_id(),
      TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS,
      &rrc_amr_log_chnl_info,
      sizeof(rrc_amr_log_chnl_info)
      );

    /* Vocoder needs to be stopped if there is any change in the codec before we configuring the logical channels */
    tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
      tdsrrc_get_as_id(),
      TDSCDMA_IVOICE_EVENT_REQUEST_STOP,
      NULL,
      0
      );;
  }
  
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
  /*Send  WtoG hand over failure   */
  tdsrrc_send_hho_cmd_to_cm(SYS_HO_FAIL);
  tdsrrc_set_ordered_codec_status(FALSE);
#endif

  return;
}

/*===========================================================================

FUNCTION tdsrrc_configure_voice_adapter

DESCRIPTION
  This function configures TDS Voice Adapter for registeration of AMR RLC 
  queues in UL/DL. For WB-AMR, only 2 classes are registered in UL /DL. 
  For NB-AMR, the no. of classes to be registered in UL / DL is taken based 
  on no. of RBs / DL AMR rate.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrrc_configure_voice_adapter
(
  tdsrrcllc_amr_mapping_info_type * amr_info,
  boolean rrc_ul_direction,
  boolean rrc_dl_direction,
  uint32 rrc_voice_adapter_event
)
{
  uint8 num_of_RBs = 0, voice_rab_idx = 0;
  tdscdma_ivoice_event_request_start_t rrc_mode_type = {0};
  tdscdma_ivoice_event_request_codec_mode_t rrc_amr_codec_mode = {0};
  tdscdma_ivoice_event_request_scr_mode_t rrc_amr_scr_mode = {0};
  tdscdma_ivoice_event_set_logical_channels_t rrc_amr_log_chnl_info = {0};

  for(voice_rab_idx = 0; voice_rab_idx < MAX_RAB_TO_SETUP; voice_rab_idx++)
  {
    if(tdsrrc_est_rabs.rabs[voice_rab_idx].rab_type == TDSRRCRB_CS_VOICE_RAB)
    {
      break;
    }
  }

  if(voice_rab_idx >= MAX_RAB_TO_SETUP)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: No CS RAB found");
    return;
  }

  if(tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func == NULL)
  {
    ERR_FATAL("TV_TRRC: failed to register with TDS Voice Adapter", 0, 0, 0);
    return;
  }

  switch(rrc_voice_adapter_event)
  {
    case TDSCDMA_IVOICE_EVENT_SELECT_OWNER:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Sending select owner event TDSCDMA_IVOICE_EVENT_SELECT_OWNER");
      tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
        tdsrrc_get_as_id(),
        TDSCDMA_IVOICE_EVENT_SELECT_OWNER,
        NULL,
        0
        );
      break;

    case TDSCDMA_IVOICE_EVENT_REQUEST_STOP:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Sending stop event TDSCDMA_IVOICE_EVENT_REQUEST_STOP");
      tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
        tdsrrc_get_as_id(),
        TDSCDMA_IVOICE_EVENT_REQUEST_STOP,
        NULL,
        0
        );
      break;

    case TDSCDMA_IVOICE_EVENT_REQUEST_START:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: Sending start event TDSCDMA_IVOICE_EVENT_REQUEST_START, codec_mode_type:%0x", 
        amr_info->mode_type);
      rrc_mode_type.vocoder_id = amr_info->mode_type;
      tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
        tdsrrc_get_as_id(),
        TDSCDMA_IVOICE_EVENT_REQUEST_START,
        &rrc_mode_type,
        sizeof(rrc_mode_type)
        );
      break;

    case TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: Sending codec mode event TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE, codec_mode:%0x", 
        amr_info->amr_mode);
      rrc_amr_codec_mode.codec_mode = amr_info->amr_mode;
      tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
        tdsrrc_get_as_id(),
        TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE,
        &rrc_amr_codec_mode,
        sizeof(rrc_amr_codec_mode)
        );
      break;

    case TDSCDMA_IVOICE_EVENT_REQUEST_SCR_MODE:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: Sending scr mode event TDSCDMA_IVOICE_EVENT_REQUEST_SCR_MODE, scr_mode:%0x", 
        amr_info->scr_mode);
      rrc_amr_scr_mode.enable_flag = amr_info->scr_mode;
      tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
        tdsrrc_get_as_id(),
        TDSCDMA_IVOICE_EVENT_REQUEST_SCR_MODE,
        &rrc_amr_scr_mode,
        sizeof(rrc_amr_scr_mode)
        );
      break;

    case TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS:
      if((num_of_RBs = (uint8)tdsrrc_est_rabs.rabs[voice_rab_idx].num_rbs_for_rab) > TDSMAX_DCH_VOICE_CHANNEL)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: Invalid no. of RBs = %d. Skipping the configuration of TDS Voice Adapter.",num_of_RBs);
        return;
      }
 
      /*The decision of registering for AMC Class C in UL is done based on no. of RBs.*/        
      if(num_of_RBs == 3)
      {
        amr_info->ul_amr_c_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)tdsrrc_est_rabs.rabs[voice_rab_idx].rb_for_rab[2].rb_id);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: LC id for AMR Class C = %d",amr_info->ul_amr_c_id);         
      }

      if(TDSCDMA_IVOCODER_ID_AMRWB == amr_info->mode_type)
      {
        rrc_amr_log_chnl_info.direction = TRUE;
        rrc_amr_log_chnl_info.lc_class_a = amr_info->ul_amr_a_id;
        rrc_amr_log_chnl_info.lc_class_b = amr_info->ul_amr_b_id;
        rrc_amr_log_chnl_info.lc_class_c = amr_info->ul_amr_c_id;
        rrc_amr_log_chnl_info.class_type = TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_AB;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Sending event TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS with 2 WB-AMR Classes in UL");  
        tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
          tdsrrc_get_as_id(),
          TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS,
          &rrc_amr_log_chnl_info,
          sizeof(rrc_amr_log_chnl_info)
          );

        rrc_amr_log_chnl_info.direction = FALSE;
        rrc_amr_log_chnl_info.lc_class_a = amr_info->dl_amr_a_id;
        rrc_amr_log_chnl_info.lc_class_b = amr_info->dl_amr_b_id;
        rrc_amr_log_chnl_info.lc_class_c = amr_info->dl_amr_c_id;
        rrc_amr_log_chnl_info.class_type = TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_AB;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Sending event TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS with 2 WB-AMR Classes in DL");  
        tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
          tdsrrc_get_as_id(),
          TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS,
          &rrc_amr_log_chnl_info,
          sizeof(rrc_amr_log_chnl_info)
          );

      }
      else if(TDSCDMA_IVOCODER_ID_AMR == amr_info->mode_type)
      {
        if(TRUE == rrc_ul_direction) /* during the AMR NB reconfiguration explicitly configured the logical channels for both UL and DL */
        {
          if(num_of_RBs == 3)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Sending event TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS with 3 AMR Classes in UL.");
            rrc_amr_log_chnl_info.direction = TRUE;
            rrc_amr_log_chnl_info.lc_class_a = amr_info->ul_amr_a_id;
            rrc_amr_log_chnl_info.lc_class_b = amr_info->ul_amr_b_id;
            rrc_amr_log_chnl_info.lc_class_c = amr_info->ul_amr_c_id;
            rrc_amr_log_chnl_info.class_type = TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_ABC;
            tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
              tdsrrc_get_as_id(),
              TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS,
              &rrc_amr_log_chnl_info,
              sizeof(rrc_amr_log_chnl_info)
              );
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Sending event TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS with 2 AMR Classes in UL.");  
            rrc_amr_log_chnl_info.direction = TRUE;
            rrc_amr_log_chnl_info.lc_class_a = amr_info->ul_amr_a_id;
            rrc_amr_log_chnl_info.lc_class_b = amr_info->ul_amr_b_id;
            rrc_amr_log_chnl_info.lc_class_c = amr_info->ul_amr_c_id;
            rrc_amr_log_chnl_info.class_type = TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_AB;
            tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
              tdsrrc_get_as_id(),
              TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS,
              &rrc_amr_log_chnl_info,
              sizeof(rrc_amr_log_chnl_info)
              );
          }
        }

        if(TRUE == rrc_dl_direction)
        {
          if(amr_info->dl_amr_mode >= TDSCDMA_IVOCAMR_CODEC_MODE_1020)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Sending event TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS with 3 AMR Classes in DL.");
            rrc_amr_log_chnl_info.direction = FALSE;
            rrc_amr_log_chnl_info.lc_class_a = amr_info->dl_amr_a_id;
            rrc_amr_log_chnl_info.lc_class_b = amr_info->dl_amr_b_id;
            rrc_amr_log_chnl_info.lc_class_c = amr_info->dl_amr_c_id;
            rrc_amr_log_chnl_info.class_type = TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_ABC;
            tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
              tdsrrc_get_as_id(),
              TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS,
              &rrc_amr_log_chnl_info,
              sizeof(rrc_amr_log_chnl_info)
              );
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TV_TRRC: Sending event TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS with 2 AMR Classes in DL.");  
            rrc_amr_log_chnl_info.direction = FALSE;
            rrc_amr_log_chnl_info.lc_class_a = amr_info->dl_amr_a_id;
            rrc_amr_log_chnl_info.lc_class_b = amr_info->dl_amr_b_id;
            rrc_amr_log_chnl_info.lc_class_c = amr_info->dl_amr_c_id;
            rrc_amr_log_chnl_info.class_type = TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_AB;
            tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func(
              tdsrrc_get_as_id(),
              TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS,
              &rrc_amr_log_chnl_info,
              sizeof(rrc_amr_log_chnl_info)
              );
          }
        }
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: UNKNOWN EVENT SELECT %0x", rrc_voice_adapter_event);
      break;
  }

}

/*====================================================================
FUNCTION: tdsrrcllc_get_log_chan_mode_ul_dch()

DESCRIPTION:
  This function gets a the log_chan_mode for the rb id specified if the rb_id is UL DCH

DEPENDENCIES:
  None.

RETURN VALUE:
  Logical Channel mode of the rb_id    if rb_id is found mapped to a UL DCH
  UE_MODE_MAX_NUM     otherwise

SIDE EFFECTS:

====================================================================*/
uecomdef_logch_mode_e_type tdsrrcllc_get_log_chan_mode_ul_dch
(
  tdsrrc_RB_Identity rb_id
)
{
  uint32 dch_count = 0;
  uint32 num_dlchan = 0;
  uint32 num_dch=0;
  uint32 dlchl_count=0;
  /* Get the logical channel mode if a new logical channel is to be established */
  num_dch = tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs;
  /* Look into all DCHs to get to the one mapped to the same RB ID */
  for(dch_count=0;(dch_count < num_dch);dch_count++)
  {
    num_dlchan = tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->
           dch_info.dch_info[dch_count].ndlchan;
    for(dlchl_count=0; dlchl_count< num_dlchan; dlchl_count++)
    {
      if(tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->
        dch_info.dch_info[dch_count].dlchan_info[dlchl_count].rb_id == rb_id)
      {
        /* This DCH is mapped to the same RB ID as is being setup.
         */
         return (tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->
          dch_info.dch_info[dch_count].dlchan_info[dlchl_count].rlc_mode);
      }
    }
  }
  return (UE_MODE_MAX_NUM);
}

/*====================================================================
FUNCTION: tdsrrcllc_get_hs_log_chan_mode()

DESCRIPTION:
  This function gets a the log_chan_mode for the rb id specified if the rb_id is mapped to HS dflow

DEPENDENCIES:
  None.

RETURN VALUE:
  Logical Channel mode of the rb_id    if rb_id is found mapped to a HS dflow
  UE_MODE_MAX_NUM     otherwise

SIDE EFFECTS:

====================================================================*/
uecomdef_logch_mode_e_type tdsrrcllc_get_hs_log_chan_mode
(
  tdsrrc_RB_Identity rb_id
)
{
  uint32    tdsnum_dflow = 0;
  uint32    dflow_count = 0;
  uint32    dlchl_count;

  tdsnum_dflow= tdsordered_config_ptr->mac_dl_parms.num_dflow;
  
  /* Look into all DFLOWs to get to the one mapped to the same RB ID */
  for(dflow_count=0;(dflow_count < tdsnum_dflow);dflow_count++)
  {
    /* Preventive check */
    if (tdsordered_config_ptr->mac_dflow_info[dflow_count].mac_dflow_id == TDSINVALID_DFLOW_ID)
    {
      continue;
    }
    TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:DFLOW %d, No DL Ch %d",
             tdsordered_config_ptr->mac_dflow_info[dflow_count].mac_dflow_id,
             tdsordered_config_ptr->mac_dflow_info[dflow_count].ndlchan);
    
    /* Check no of DL Dedicated Channels */
    
    for(dlchl_count=0; dlchl_count< tdsordered_config_ptr->mac_dflow_info[dflow_count].ndlchan; dlchl_count++)
    {
      if(tdsordered_config_ptr->mac_dflow_info[dflow_count].dlchan_info[dlchl_count].rb_id == rb_id)
      {
        TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:RB Id %d mapped to dflow %d",
                 rb_id,tdsordered_config_ptr->mac_dflow_info[dflow_count].mac_dflow_id);
        
          /* This DFLOW is mapped to the same RB ID as is being setup.
        */
        TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:RLC Mode(0:T:1:U:2/3:A)for above DFLOW %d",
                 tdsordered_config_ptr->mac_dflow_info[dflow_count].dlchan_info[dlchl_count].rlc_mode);
        return(tdsordered_config_ptr->mac_dflow_info[dflow_count].dlchan_info[dlchl_count].rlc_mode);
      }
    }
  }
  return (UE_MODE_MAX_NUM);
}

/*====================================================================
FUNCTION: tdsrrcllc_check_if_dflow_exist_and_update_rlc_id()

DESCRIPTION:
  This function gets a the log_chan_mode for the rb id specified if the rb_id is mapped to dflow,
  if mapped to dflow, updates the dflow info the logicah channel id

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE   if RB id is mapped to dflow
  FALSE     otherwise

SIDE EFFECTS:

====================================================================*/
boolean tdsrrcllc_check_if_dflow_exist_and_update_rlc_id
(
  tdsrrc_RB_Identity rb_id,
  rlc_lc_id_type dl_lc_id
)
{
  uint32    tdsnum_dflow = 0;
  uint32    dflow_count = 0;
  uint32    dlchl_count;
/* Check in DFLOW List */
  /* Get the logical channel mode if a new logical channel is to be established */
  tdsnum_dflow= tdsordered_config_ptr->mac_dl_parms.num_dflow;

  /* Look into all DFLOWs to get to the one mapped to the same RB ID */
  for(dflow_count=0; (dflow_count < tdsnum_dflow); dflow_count++)
  {
    if (tdsordered_config_ptr->mac_dflow_info[dflow_count].mac_dflow_id == TDSINVALID_DFLOW_ID)
    {
      continue;
    }
    TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:DFLOW %d, No DL Ch %d",
             tdsordered_config_ptr->mac_dflow_info[dflow_count].mac_dflow_id,
             tdsordered_config_ptr->mac_dflow_info[dflow_count].ndlchan);

    /* Check no of DL Dedicated Channels */

    for(dlchl_count=0; dlchl_count< tdsordered_config_ptr->mac_dflow_info[dflow_count].ndlchan; dlchl_count++)
    {
      if(tdsordered_config_ptr->mac_dflow_info[dflow_count].dlchan_info[dlchl_count].rb_id ==rb_id)
      {
        TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:RB Id %d mapped to dflow %d",
                 rb_id, tdsordered_config_ptr->mac_dflow_info[dflow_count].mac_dflow_id);
        
        /* This DFLOW is mapped to the same RB ID as is being setup.*/

        TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:RLC Mode(0:T:1:U:2/3:A)for above DFLOW %d",
                 tdsordered_config_ptr->mac_dflow_info[dflow_count].dlchan_info[dlchl_count].rlc_mode);
        tdsordered_config_ptr->mac_dflow_info[dflow_count].dlchan_info[dlchl_count].rlc_id =
          dl_lc_id;
        return TRUE;
      }
    }  /* for(...) */
  }
  return FALSE;
}
/*====================================================================
FUNCTION: tdsrrcllc_check_if_ul_dch_exist_and_update_rlc_id()

DESCRIPTION:
  This function gets a the log_chan_mode for the rb id specified if the rb_id is mapped to UL DCH,
  if mapped to DCH, updates the UL DCH info the logicah channel id

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE   if RB id is mapped to UL DCH
  FALSE     otherwise

SIDE EFFECTS:

====================================================================*/
boolean tdsrrcllc_check_if_ul_dch_exist_and_update_rlc_id
(
  tdsrrc_RB_Identity rb_id,
  rlc_lc_id_type ul_lc_id,
  uecomdef_logchan_e_type log_chl_type
)
{
  uint32 dch_count = 0;
  uint32 num_dlchan = 0;
  uint32 num_dch=0;
  uint32 dlchl_count=0;
  
  num_dch = tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs;
  /* Look into all DCHs to get to the one mapped to the same RB ID */
  for(dch_count=0;(dch_count < num_dch); dch_count++)
  {
    num_dlchan = tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->
                 dch_info.dch_info[dch_count].ndlchan;
    for(dlchl_count=0; dlchl_count< num_dlchan; dlchl_count++)
    {
      if((tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->
            dch_info.dch_info[dch_count].dlchan_info[dlchl_count].rb_id == rb_id) &&
          (tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->
          dch_info.dch_info[dch_count].dlchan_info[dlchl_count].chan_type == log_chl_type))
      {
        /* This DCH is mapped to the same RB ID and the same logical
         * Channel type as is being setup.
         */
        tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->
          dch_info.dch_info[dch_count].dlchan_info[dlchl_count].rlc_id =  ul_lc_id;
        return (TRUE);
      }
    }  /* for(...) */
  }
  return (FALSE);
}

/*====================================================================
FUNCTION: tdsrrcllc_check_if_dl_dch_exist_and_update_rlc_id()

DESCRIPTION:
  This function gets a the log_chan_mode for the rb id specified if the rb_id is mapped to UL DCH,
  if mapped to DCH, updates the UL DCH info the logicah channel id

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE   if RB id is mapped to UL DCH
  FALSE     otherwise

SIDE EFFECTS:

====================================================================*/
boolean tdsrrcllc_check_if_dl_dch_exist_and_update_rlc_id
(
  tdsrrc_RB_Identity rb_id,
  rlc_lc_id_type dl_lc_id,
  uecomdef_logchan_e_type log_chl_type
)
{
  uint32 dch_count = 0;
  uint32 num_dlchan = 0;
  uint32 num_dch=0;
  uint32 dlchl_count=0;

 num_dch = tdsordered_config_ptr->mac_dl_parms.num_dchs;
 
  /* Look into all DCHs to get to the one mapped to the same RB ID */
  for(dch_count=0;(dch_count < num_dch);dch_count++)
  {
    num_dlchan = tdsordered_config_ptr->mac_dl_parms.dch_info[dch_count]->ndlchan;
    for(dlchl_count=0; dlchl_count< num_dlchan; dlchl_count++)
    {
      if((tdsordered_config_ptr->mac_dl_parms.dch_info[dch_count]->
          dlchan_info[dlchl_count].rb_id  == rb_id) &&
         (tdsordered_config_ptr->mac_dl_parms.dch_info[dch_count]->dlchan_info[dlchl_count].
            chan_type == log_chl_type))
      {
        /* This logical channel is of the same RB ID and the same logical
         * Channel type as is being setup.
         */
        tdsordered_config_ptr->mac_dl_parms.dch_info[dch_count]->
          dlchan_info[dlchl_count].rlc_id = dl_lc_id;
        return (TRUE);
      }
    }  /* for(...) */
  }  /* for(...) */
  return (FALSE);
}

/*====================================================================
FUNCTION: tdsrrcllc_allocate_mem_for_prach_sel_info()

DESCRIPTION:
  This function initializes global tdsprach_sel_info dynamically/statically.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.
  
SIDE EFFECTS:
  None.

=====================================================================*/
void tdsrrcllc_allocate_mem_for_prach_sel_info
(
  uint8 rrc_num_of_prach
)
{
  uint8 idx = 0;


  if(rrc_num_of_prach > 0 && rrc_num_of_prach <= UE_MAX_PRACH)
  {
    for(idx = 0; idx < rrc_num_of_prach; idx++)
    {
      tdsprach_sel_info[idx] = (tdsrrcllc_prach_sel_info_type *) modem_mem_calloc(1,sizeof(tdsrrcllc_prach_sel_info_type),
            MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT);

      if(tdsprach_sel_info[idx] == NULL)
      {
        ERR_FATAL("PRACH_OPT: Couldn't alloc mem for idx %d for num %d PRACHs",idx,rrc_num_of_prach,0);
      }
    }
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"PRACH_OPT: Allocated %d PRACHs in tdsprach_sel_info",rrc_num_of_prach);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"PRACH_OPT: Invalid no. of prachs %d",rrc_num_of_prach);
  }

}

/*====================================================================
FUNCTION: tdsrrcllc_allocate_mem_for_prach_sel_info()

DESCRIPTION:
  This function resets global tdsprach_sel_info.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.
  
SIDE EFFECTS:
  None.

=====================================================================*/
void tdsrrcllc_deallocate_mem_for_prach_sel_info
(
  uint8 rrc_num_of_prach
)
{
  uint8 idx = 0;
  

  for(idx = 0; idx < rrc_num_of_prach; idx++)
  {
    if(tdsprach_sel_info[idx] != NULL)
    {
      modem_mem_free((void *)tdsprach_sel_info[idx], MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PRACH_OPT: Something wrong happened.");
    }
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"PRACH_OPT: De-Allocated %d PRACHs in tdsprach_sel_info with %d PRACHs",idx,rrc_num_of_prach);

}



/*====================================================================
FUNCTION: tdsrrcllc_update_drx_coef_length()

DESCRIPTION:
  This function is called when T319 timer expires(or is stopped) and updates the UTRAN DRX Cycle Coef Length

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/

void tdsrrcllc_update_drx_coef_length
(
  void
)
{
  if((tdsutran_drx_cycle_info.utran_drx_cycle_length.length>= TDSRRCLLC_UTRAN_MIN_DRX_CYCLE_LENGTH) && 
    (tdsutran_drx_cycle_info.utran_drx_cycle_length.length <= TDSRRCLLC_UTRAN_MAX_DRX_CYCLE_LENGTH)
    )
  {
    if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
    {
      tdsordered_config_ptr->utran_drx_cycle_length =
        (uint32) (1 << tdsutran_drx_cycle_info.utran_drx_cycle_length.length);
    }
    else if(tdsconfig_ptr_in_use == TDSCURRENT_CONFIG)
    {
      tdscurrent_config_ptr->utran_drx_cycle_length =
        (uint32) (1 << tdsutran_drx_cycle_info.utran_drx_cycle_length.length);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Cycle Length Coefficient %d is not valid",
      tdsutran_drx_cycle_info.utran_drx_cycle_length.length);
    return;
  }
  if((tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH) ||(tdsrrc_get_state() == TDSRRC_STATE_URA_PCH) )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"UTRAN DRX:send_l1_drx_req, cycle_length=%d",
      (uint32) (1 << tdsutran_drx_cycle_info.utran_drx_cycle_length.length));
    /*Send a request to L1 to update the UTRAN DRX Cycle Coef Length*/
    (void)tdsrrccsp_send_l1_drx_req_for_multiple_drx();
  }
}
/*====================================================================
FUNCTION: tdsrrcllc_get_drx_info_for_multiple_drx()

DESCRIPTION:
  This function returns the shorter of the PS or CS CN DRX Cycle
  Length for multiple DRX support feature.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32 of the DRX Cycle length, per 25.304.

SIDE EFFECTS:
  None.
====================================================================*/
uint32 tdsrrcllc_get_drx_info_for_multiple_drx(void)
{
  uint32 cs_drx = 0;
  uint32 ps_drx = 0;
  uint32 utran_drx = 0;
 
  uint32 cc_pbp = TDSRRC_CONV_PICH_REP_LEN(tdscurrent_config_ptr->l1_dl_chan_parms.pich.repetition_period_length);
  uint32 oc_pbp = TDSRRC_CONV_PICH_REP_LEN(tdsordered_config_ptr->l1_dl_chan_parms.pich.repetition_period_length);
 
  /* Initialize TDD CS PS DRX in similar way as in FDD */
  cs_drx = MAX(tdscurrent_config_ptr->cs_drx_cycle_length, cc_pbp);
  ps_drx = MAX(tdscurrent_config_ptr->ps_drx_cycle_length, cc_pbp);
   
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    utran_drx = MAX(tdsordered_config_ptr->utran_drx_cycle_length, oc_pbp);
    cs_drx = MAX(tdsordered_config_ptr->cs_drx_cycle_length, oc_pbp);
    ps_drx = MAX(tdsordered_config_ptr->ps_drx_cycle_length, oc_pbp);
  }
  else if(tdsconfig_ptr_in_use == TDSCURRENT_CONFIG)
  {
    utran_drx = MAX(tdscurrent_config_ptr->utran_drx_cycle_length, cc_pbp);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"UTRAN DRX Cycle Length Coefficient %d is not valid",
      tdsutran_drx_cycle_info.utran_drx_cycle_length.length);
    /* ZZTBD: Why return DRX COEF here ? */
    return tdsutran_drx_cycle_info.utran_drx_cycle_length.length;
  }

  /* We may not have a UTRAN DRX cycle length, so we must not consider
     it in this case.*/

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"DRX cs:%d, ps:%d, utran:%d", cs_drx,ps_drx, utran_drx);
  if(utran_drx == 0)
  {
    /* Return the DRX Cycle Length */
    if (cs_drx < ps_drx)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Using cs drx %d", cs_drx);
      return cs_drx;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Using ps drx %d", ps_drx);
      return ps_drx;
    }
  }
  else /* this is non idle case so must be cell_PCH/URA_PCH case */
  {
    /* find out the un-established domain.*/
    if ((tdsrrcscr_get_signalling_connection_status (RRC_PS_DOMAIN_CN_ID) == FALSE) &&
        (tdsrrcscr_get_signalling_connection_status (RRC_CS_DOMAIN_CN_ID) == TRUE))
    {
      if (ps_drx < utran_drx)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS & UTRAN Using PS drx %d", ps_drx);
        return ps_drx;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS & UTRAN Using utran drx %d", utran_drx);
        return utran_drx;
      }
      //return (ps_drx<utran_drx?ps_drx:utran_drx);
    }
    else if ((tdsrrcscr_get_signalling_connection_status (RRC_CS_DOMAIN_CN_ID) == FALSE) &&
             (tdsrrcscr_get_signalling_connection_status (RRC_PS_DOMAIN_CN_ID) == TRUE))
    {
      //return (cs_drx<utran_drx?cs_drx:utran_drx);
      if (cs_drx < utran_drx)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS & UTRAN Using CS drx %d", cs_drx);
        return cs_drx;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS & UTRAN Using utran drx %d", utran_drx);
        return utran_drx;
      }
    }
    else if ((tdsrrcscr_get_signalling_connection_status (RRC_CS_DOMAIN_CN_ID) == TRUE) &&
             (tdsrrcscr_get_signalling_connection_status (RRC_PS_DOMAIN_CN_ID) == TRUE))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS & PS connected. Using utran drx %d", utran_drx);
      return (utran_drx);
    }
    else /*((tdsrrcscr_get_signalling_connection_status (RRC_CS_DOMAIN_CN_ID) == FALSE) &&
            (tdsrrcscr_get_signalling_connection_status (RRC_PS_DOMAIN_CN_ID) == FALSE))*/
    {
      if(cs_drx <= ps_drx && cs_drx <= utran_drx)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"No PS or CS Using CS drx %d", cs_drx);
        return cs_drx;
      }
      else if(ps_drx <= cs_drx && ps_drx <= utran_drx)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"No PS or CS Using ps drx %d", ps_drx);
        return ps_drx;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"No PS or CS Using utran drx %d", utran_drx);
        return utran_drx;
      }
    }
  }
} /* end tdsrrcllc_get_drx_info_for_multiple_drx() */ 

/*====================================================================
FUNCTION: tdsrrcllc_set_am_dl_rlc_size_ota()

DESCRIPTION:
  This function sets the PDU type and size for AM RB .
  If the PDU tpe is 
    TDSRRC_PDU_SIZE_NOT_SET then the OTA has not provided the RLC size explicitly, and
     it should be calculated from the TF sizes of Trch
  TDSRRC_FIXED_PDU_SIZE_SET the OTA had explicitly given the rlc size 
  TDSRRC_FLEXIBLE_PDU_SIZE_SET OTA has set the RB as Flexible PDU
  
DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/

void tdsrrcllc_set_am_dl_rlc_size_ota
(
  tdsrrc_RB_Identity rb_id,
  uint32 dl_rlc_am_pdu_size,
  tdsrrc_am_rlc_pdu_enum_type pdu_type

)
{
  uint32 entity_idx =0;
  
  if((entity_idx = tdsrrc_get_am_index(rb_id)) != TDSUE_MAX_AM_ENTITY)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Set PDU type as %d for rb id %d at idx %d",pdu_type,rb_id,entity_idx);
    tdsordered_config_ptr->rb_am_rlc_size_ota[entity_idx].rrc_pdu_type = pdu_type;

    tdsordered_config_ptr->rb_am_rlc_size_ota[entity_idx].dl_rlc_am_pdu_size = dl_rlc_am_pdu_size;
    tdsordered_config_ptr->rb_am_rlc_size_ota[entity_idx].rb_id = rb_id;
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_get_am_dl_rlc_size_ota()

DESCRIPTION:
  This function returns  the RLC size if it is provided by the N\W  for AM RB .
 
  
DEPENDENCIES:
  None.

RETURN VALUE:
  RLC size if the OTA provided the rlcc size, 
  0 if it is flexible pdu,
  TDSRRCLLC_INVALID_RLC_SIZE if rlc size is not provided in ota
  
SIDE EFFECTS:

====================================================================*/
uint32 tdsrrcllc_get_am_dl_rlc_size_ota
(
  tdsrrc_RB_Identity rb_id
)
{
  uint32 entity_idx;
  tdsordered_config_type * config_ptr;
  
  if((tdstransition_config.toc_usage == TDSTOC_FOR_DCH_AND_FACH)
      ||(tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH)
      ||(tdstransition_config.toc_usage == TDSTOC_FOR_FACH_AND_PCH_WITH_DCH_INFO))
  {
    entity_idx = tdsrrc_get_am_index_in_toc(rb_id);
    config_ptr= tdstransition_config.toc_ptr;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TOC in use %d",tdstransition_config.toc_usage);
  }
  else
  {
    entity_idx = tdsrrc_get_am_index(rb_id);
    config_ptr= tdsordered_config_ptr;
    if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG )
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"OC in use ");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"CC in use unexpected to call this function");
    }
  }
  
  if( entity_idx != TDSUE_MAX_AM_ENTITY)
  {
    if(config_ptr->rb_am_rlc_size_ota[entity_idx].rb_id == rb_id)
    {
      if(  config_ptr->rb_am_rlc_size_ota[entity_idx].rrc_pdu_type == TDSRRC_PDU_SIZE_NOT_SET)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"No  RLC size for RB id %d from OTA",rb_id);
        return TDSRRCLLC_INVALID_RLC_SIZE;
      }
      else if (  config_ptr->rb_am_rlc_size_ota[entity_idx].rrc_pdu_type == TDSRRC_FIXED_PDU_SIZE_SET)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH," RLC size for RB id %d from OTA %d",rb_id,
             config_ptr->rb_am_rlc_size_ota[entity_idx].dl_rlc_am_pdu_size);
        return  config_ptr->rb_am_rlc_size_ota[entity_idx].dl_rlc_am_pdu_size;
      }
      else
      {  
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"Flexible PDU  RLC size for RB id %d from OTA",rb_id);
        return 0;
      }
    }
  
  }
  TDSRRC_MSG1(MSG_LEGACY_MED,"No rb entry in RLC size for RB id %d",rb_id);
  return TDSRRCLLC_INVALID_RLC_SIZE;
  
}

/*====================================================================
FUNCTION: tdsrrcllc_init_am_dl_rlc_size()

DESCRIPTION:
  This function inits the db which contains rlc size from ota .
 
  
DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcllc_init_am_dl_rlc_size
(
  tdsordered_config_type *config_ptr,
  uint32 entity_idx
)
{
   config_ptr->rb_am_rlc_size_ota[entity_idx].rrc_pdu_type = TDSRRC_PDU_SIZE_NOT_SET;
   config_ptr->rb_am_rlc_size_ota[entity_idx].dl_rlc_am_pdu_size = TDSRRCLLC_INVALID_RLC_SIZE;
   config_ptr->rb_am_rlc_size_ota[entity_idx].rb_id = TDSRRCLLC_INVALID_RB_ID;
}
 void
tdsrrcllc_init_config_ptrs
(
  void
)
{

  if(tdscurrent_config_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CC/OC/TOC already freed.");
    return;
  }

  tdsrrcllc_init_ordered_config_status_and_data(TDSRRC_STATE_DISCONNECTED);
  tdsrrcllc_init_config_data(tdscurrent_config_ptr, TDSRRC_STATE_DISCONNECTED);

  tdsrrcllc_init_config_data(tdstransition_config.toc_ptr, TDSRRC_STATE_DISCONNECTED);
}


#ifdef FEATURE_TDSCDMA_CPC_DTX
/*====================================================================
FUNCTION: tdsrrcllc_reset_l1_cpc_dtx_info_on_llc_cfg_cnf()

DESCRIPTION:
  This function resets the L1 variables related to CPC-DTX on receipt of LLC CONFIG CNF 
  by any procedure
  
DEPENDENCIES:
  None
  
RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_reset_l1_cpc_dtx_info_on_llc_cfg_cnf(void)
{
  tdsordered_config_type  *config_ptr;
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:Reset CPC-DTX  masks<Prev: l1_cpc_dtx_req_mask: %d,",
    config_ptr->l1_cpc_dtx_req_mask, 0);
  
  config_ptr->l1_cpc_dtx_req_mask = L1_CPC_DTX_NO_OP;
#ifdef FEATURE_TDSCDMA_CPC_DRX
TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCCPC:Reset CPC-DRX  masks<Prev: l1_cpc_drx_req_mask: %d,",
  config_ptr->l1_cpc_drx_req_mask);
  
  config_ptr->l1_cpc_drx_req_mask = L1_CPC_DRX_NO_OP;
#endif
}
#endif


#ifdef FEATURE_TDSCDMA_REL8
/*====================================================================
FUNCTION: tdsrrcllc_determine_hsdpa_action_r8()

DESCRIPTION:
  This function determines the HSDPA action and sets the parameters in L1
  structure accordingly.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  Sucess if HSDPA action and parameters have been set
  Failure Otherwise

SIDE EFFECTS:
====================================================================*/
uecomdef_status_e_type tdsrrcllc_determine_hsdpa_action_r8
(
  tdsrrc_DL_HSPDSCH_Information_r8* hspdsch_info_ptr
)
{
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  uint8 i = 0;
#endif
  
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  multicarrier_flg = tdsrrcllc_multicarrier_present_per_version_r8(hspdsch_info_ptr);
#endif
  /* Save UE category info */
  if (tdsnv_hsdpa_category > 15)
  {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if (0 == tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue)
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
#endif
    }
    else
    {
      for(i=0;i< tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue;i++)
      {
        if ((tdsordered_config_ptr->dl_macehs_logchan_info[i].queue_id != TDSINVALID_QUEUE_ID) &&
          (tdsordered_config_ptr->mac_ehs_lc_id_in_use[i] == TRUE ))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 23;
          break;
        }
        else
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
          if (i == (tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue - 1))
          {
            tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
          }
#endif
        }
      }
    }
#else
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#endif
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = tdsnv_hsdpa_category;
  }

  /* HSDPA params have already been set in rrc_hsdpa_params.
  Check if HSDPA is already ACTIVE */
  if (tdsrrc_get_hsdpa_status() == TDSHSDPA_INACTIVE)
  {
    /* HSDPA is currently inactive */
    /* Check if network wants to activate HSDPA */
    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {
      if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
      {
        /* Check if channelization codes and feedback info present */
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r8,hs_scch_Info)) 
        {
          if (tdsrrcllc_set_hsscch_info_r7(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                      , multicarrier_flg
#endif
            ) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: Invalid HS-SCCH info.");
            return FAILURE;
          }
        }
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r8,measurement_feedback_Info))
        {
          /* Set Meas Feedback Info */
          if (tdsrrcllc_set_meas_feedback_info_r7(&hspdsch_info_ptr->measurement_feedback_Info) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Invalid Meas Feedback info for HS.");
            return FAILURE;
          }
        }

        if (TDSRRC_CHECK_MSG_TYPE(hspdsch_info_ptr->modeSpecificInfo,
            T_tdsrrc_DL_HSPDSCH_Information_r8_modeSpecificInfo_tdd))
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd,
            rrc_DL_HSPDSCH_Information_r8_tdd_tdd128))
          {
            /* Check if HS-PDSCH Midamble Configuration present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                hs_PDSCH_Midamble_Configuration_tdd128))
            {
              tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                , multicarrier_flg
#endif
                );
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive midamble configuration tdd128");
            }
            /* Check if DL Multi-Carrier Information present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                dl_MultiCarrier_Information))
            {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
              if(FAILURE == tdsrrcllc_check_multicarrier_support_per_version(&hspdsch_info_ptr->
                  modeSpecificInfo.u.tdd->u.tdd128->dl_MultiCarrier_Information, NULL, FALSE))
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
                return FAILURE;
              }
#else
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport IE DL Multi-Carrier Information reveived");
              return FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
          return FAILURE;
        }
      }
    } /* End of if of hsdpa message params absent */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:TDSHSDPA_NOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_reset_mac_ehs_params(tdsordered_config_ptr);
#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
      return SUCCESS;
    }

    /* Set Queue Info */
    if (tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Unable to set Queue Info");
      return FAILURE;
    }

    /* Final Check to make sure that everything is present */
    if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
    {
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_reset_mac_ehs_params(tdsordered_config_ptr);
#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:TDSHSDPA_NOOP");
      return SUCCESS;
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:TDSHSDPA_START");
    tdsrrc_set_hsdpa_action(TDSHSDPA_START);

    /* Set Bit Mask */
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = TDSL1_CPHY_HS_DSCH_CFG_INCL | 
      TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL8:hs_req_mask 0X%x", tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);

    /* In case UTRAN messes up, set HS-RESET to FALSE */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:Force HS-RESET to FALSE");
    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = FALSE;
    return SUCCESS;
  }
  /*The following deals with the scenarios when HSDPA is already ACTIVE and HS action needs to be decided.*/
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:HSDPA already active");

    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {
      return tdsrrcllc_determine_hsdpa_action_for_hsdpa_active_r8(hspdsch_info_ptr);
    }
    else
    {
      /*It means that HSDPA ia already ACTIVE but OTA hasn't provided the HSDPA parameters*/
      /* So Final Check to make sure that everything is present */
      if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: TDSHSDPA_STOP");
        tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
        return SUCCESS;
      }
      /* no matter what freq change, should reconfig HSDPA */
      if ((tdsordered_config_ptr->is_hho== TRUE)
        ||(tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: Freq change,HS Action RECONFIG");
        tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
        return SUCCESS;
      }

      /* Means No Action has to be taken for HSDPA. i.e.*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:TDSHSDPA_NOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);

      return SUCCESS;
    }
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_determine_hsdpa_action_for_hsdpa_active_r8()

DESCRIPTION:
  This function determines the HSDPA action when HSDPA ia already ACTIVE. 
  
DEPENDENCIES:
  None.

RETURN VALUE:
  Sucess if HSDPA action and parameters have been set
  Failure Otherwise

SIDE EFFECTS:
====================================================================*/
uecomdef_status_e_type tdsrrcllc_determine_hsdpa_action_for_hsdpa_active_r8
(
  tdsrrc_DL_HSPDSCH_Information_r8* hspdsch_info_ptr
)
{
    uint8 reconfig_bit_mask = 0;
    boolean hsscch_codes_changed = FALSE;
    boolean hspdsch_midamble_changed = FALSE;
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  multicarrier_flg = tdsrrcllc_multicarrier_present_per_version_r8(hspdsch_info_ptr);
#endif

  /* Set bit mask for HO */
  if ((tdsordered_config_ptr->is_hho== TRUE)
    ||(tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:hho, need reconfig dpa ");
    reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
  }
  if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
  {
    /*Process HS-SCCH Info*/
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r8,hs_scch_Info))
    {
      /* Check if HS-SCCH Info has changed */
      if (tdsrrcllc_check_if_hsscch_changed_r7(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                                          , multicarrier_flg
#endif
        ) == TRUE)
      {
        hsscch_codes_changed = TRUE;
      }

      if (tdsrrcllc_set_hsscch_info_r7(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                  , multicarrier_flg
#endif
        ) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Invalid HS-SCCH info");
        return FAILURE;
      }
      else
      {
        if (hsscch_codes_changed == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:SCCH Info changed");
          reconfig_bit_mask =reconfig_bit_mask | TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL;
        }
      }
    }

    /*Process Measurement Feedback Info.*/ 
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r8,measurement_feedback_Info))
    {
      if (tdsrrcllc_set_meas_feedback_info_r7(&hspdsch_info_ptr->measurement_feedback_Info) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: Invalid Meas Feedback info for HS.");
        return FAILURE;
      }
    }

    if (TDSRRC_CHECK_MSG_TYPE(
        hspdsch_info_ptr->modeSpecificInfo,
        T_tdsrrc_DL_HSPDSCH_Information_r8_modeSpecificInfo_tdd))
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
        hspdsch_info_ptr->modeSpecificInfo.u.tdd,
        rrc_DL_HSPDSCH_Information_r8_tdd_tdd128))
      {
        /* Check if HS-PDSCH Midamble Configuration present */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
            hs_PDSCH_Midamble_Configuration_tdd128))
        {
          /* Check if HS-PDSCH Midamble Configuration has changed */
          if (tdsrrcllc_check_if_hspdsch_midamble_changed(
            &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
            hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                              , multicarrier_flg
#endif
            ) == TRUE)
          {
            hspdsch_midamble_changed = TRUE;
          }
          tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
            &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
            hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                            , multicarrier_flg
#endif
            );
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive midamble configuration tdd128");

          if (hspdsch_midamble_changed == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:hspdsch midamble configuration changed");
            reconfig_bit_mask =reconfig_bit_mask | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;
          }
        }
        
        /* Check if DL Multi-Carrier Information present */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
            dl_MultiCarrier_Information))
        {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
          if(FAILURE == tdsrrcllc_check_multicarrier_support_per_version(&hspdsch_info_ptr->
              modeSpecificInfo.u.tdd->u.tdd128->dl_MultiCarrier_Information, &reconfig_bit_mask, FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
            return FAILURE;
          }
#else
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport IE DL Multi-Carrier Information reveived");
          return FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
      return FAILURE;
    }
  }
#ifdef FEATURE_TDSCDMA_64QAM
  if(tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured !=
           tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured)
  {
    tdshsdpa_msg_params.hsdsch_info_present = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set HS-DSCh bitmask as 64 QAM config is changed ");
  }
#endif

  /* Final Check to make sure that everything is present */
  if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:TDSHSDPA_STOP");
    tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
    return SUCCESS;
  }


  /* Set the queue information */
  if(tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8:Unable to set HSDPA info in L1");
    return FAILURE;
  }
  else
  {
    if((tdshsdpa_msg_params.hsdsch_info_present == TRUE) || 
      ((tdshsdpa_msg_params.hrnti_present == TRUE) && (tdshsdpa_msg_params.hrnti_changed == TRUE)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:TDSL1_CPHY_HS_DSCH_CFG_INCL set");
      reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
    }
  }

  /* no matter what freq change */
  if ((reconfig_bit_mask == 0) && 
      (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE) &&
      (tdsordered_config_ptr->is_hho== FALSE)&&
      (tdsordered_config_ptr->w_freq == tdscurrent_config_ptr->w_freq))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8:TDSHSDPA_NOOP");
    tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
    return SUCCESS;
  }

  tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = reconfig_bit_mask;

  /* Set Action to RECONFIG */
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL8:TDSHSDPA_RECONFIG bit mask %d,HS-RESET IND %d",reconfig_bit_mask,
           tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
  tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
  
  return SUCCESS;
}

#ifdef FEATURE_TDS_DC_HSDPA
/*============================================================================
FUNCTION: tdsrrcllc_init_dc_hsdpa_params()

DESCRIPTION: This function resets DC-HSDPA parameters in Config DB (CC/OC). It also
  resets global status of DC-HSDPA.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_init_dc_hsdpa_params
(
  tdsordered_config_type *config_ptr
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:Initing Params in Config Ptr.");

  /*The following is done for the sake of better readability*/
  if(config_ptr->l1_hsdpa_info.hs_req_mask & TDSL1_CPHY_HS_DC_HSDPA_CFG_INCL)
  {
    config_ptr->l1_hsdpa_info.hs_req_mask &= ~TDSL1_CPHY_HS_DC_HSDPA_CFG_INCL;    
  }

  config_ptr->l1_hsdpa_info.hs_secondary_info.dc_hsdpa_action = TDSL1_DC_HSDPA_NOOP;
  /*There's no valid need to reset H-RNTI because it's mandatory field and its validation is done
  while storing it.*/
#ifdef FEATURE_TDSCDMA_64QAM  

  config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_64_qam_configured = FALSE;
#endif

#ifdef FEATURE_TDSCDMA_DC_HSDPA_DIFF_TXD_MODE
  config_ptr->l1_hsdpa_info.hs_secondary_info.diff_txd_mode = FALSE;
#endif /*FEATURE_TDSCDMA_DC_HSDPA_DIFF_TXD_MODE*/

  config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_tb_size_alignment_type = 
    TDSL1_DL_CCTRCHCFG_HS_DSCH_TB_TABLE_OCTET_UNALIGNED;

  config_ptr->l1_hsdpa_info.hs_secondary_info.pri_cpich_cpid = TDSRRCLLC_INVALID_CPICH_CPID;

  config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.sec_scr_code_valid = FALSE;
  config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.sec_cpid = (uint8)TDSRRCLLC_INVALID_CPICH_CPID;
  config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.num_ch_code = 0;
  /*Since num_ch_code is 0, no need to init each SCCH channelisation codes*/

  config_ptr->l1_hsdpa_info.hs_secondary_info.po_hs_dsch_sec_cell = TDSRRCLLC_INVALID_MEAS_PWR_OFFSET;
  config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_dl_freq = TDSRRCLLC_INVALID_DL_FREQ;

  tdsrrcllc_update_dc_hsdpa_status(TDSRRC_DC_HSDPA_INACTIVE);  
  return;
}

/*============================================================================
FUNCTION: tdsrrcllc_update_dc_hsdpa_status()

DESCRIPTION:
  This function update the global status of DC-HSDPA.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_update_dc_hsdpa_status
(
  tdsrrc_dc_hsdpa_status_enum_type dc_hsdpa_status
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA:Set status to %d (0:Inactive 1:Active)",dc_hsdpa_status);
  tdsrrc_dc_hsdpa_status = dc_hsdpa_status;
  return;
}

/*============================================================================
FUNCTION: tdsrrcllc_get_dc_hsdpa_status()

DESCRIPTION:
  This function retrieves DC-HSDPA status from global status.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
tdsrrc_dc_hsdpa_status_enum_type tdsrrcllc_get_dc_hsdpa_status
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA:Get status %d (0:Inactive 1:Active)",tdsrrc_dc_hsdpa_status);
  return tdsrrc_dc_hsdpa_status;
}

/*============================================================================
FUNCTION: tdsrrcllc_check_if_dc_hsdpa_can_be_started()

DESCRIPTION: When this function returns FALSE, the Reconfig Failure isn't sent in the OTA. 
  This function does the following validations:
  1. UE is going to Cell_DCH state.
  2. HSDPA will be active with DC-HSDPA.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcllc_check_if_dc_hsdpa_can_be_started
(
  tdsrrc_RRC_StateIndicator rrc_state
)
{
  tdshsdpa_action_enum_type hs_action =tdsrrc_get_hsdpa_action();

  /*Next state should be Cell_DCH*/
  if(rrc_state != tdsrrc_RRC_StateIndicator_cell_DCH )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DC-HSDPA: Not started bcoz next RRC state %d",rrc_state);
    return FALSE;
  }

  /*DC-HSDPA can be started if
  1. HSDPA is being started
  2. Os if HSDPA ia already active, it's not being stopped.*/
  if(!((hs_action == TDSHSDPA_START) || ((tdsrrc_get_hsdpa_status() == TDSHSDPA_ACTIVE) && (hs_action != TDSHSDPA_STOP))))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA: Not started bcoz HSDPA will not be active.");
    return FALSE;
  }

  return TRUE;
}

/*============================================================================
FUNCTION: tdsrrcllc_validate_dc_hsdpa_config()

DESCRIPTION: When this function returns FALSE, the Reconfug Failure is sent in the OTA.
  The following validations are done here.
  1. Make sure MIMO is not active with DC-HSDPA.
  2. Total no. of HS-SCCH codes doesn't exceed 6

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcllc_validate_dc_hsdpa_config
(
  void
)
{
  /*Make sure that DC-HSDPA isn't active with MIMO.*/
#ifdef FEATURE_TDSCDMA_MIMO
  if((tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action == TDSL1_MIMO_START) ||
    ((tdsordered_config_ptr->l1_hsdpa_info.mimo_info.l1_mimo_action != TDSL1_MIMO_STOP) && 
    (tdsrrcllc_get_mimo_status() == TDSRRC_MIMO_ACTIVE )))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DC-HSDPA:Not started bcoz MIMO will also be active.");
    return FALSE;
  }
#endif /*FEATURE_TDSCDMA_MIMO*/

#ifdef FEATURE_TDSCDMA_64QAM
  if((tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_HS) &&
    (tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_64_qam_configured))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DC-HSDPA:Not started bcoz MAC header type is MAC-hs + 64QAM.");
    return FALSE;
  }
#endif /*FEATURE_TDSCDMA_64QAM*/

  /*Make check for no. of HS-SCCH Codes.*/
  if((tdsordered_config_ptr->l1_hsdpa_info.hs_scch_info.num_ch_code +
      tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.num_ch_code) > 
      TDSMAX_HS_SCCH_CODES_WITH_DUAL_CELL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DC-HSDPA:Exceeded max no. of HS-SCCH codes. P %d and S%",
      tdsordered_config_ptr->l1_hsdpa_info.hs_scch_info.num_ch_code,
      tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.hs_sec_cell_scch_info.num_ch_code);
    return FALSE;
  }

  if(((tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc < TDSRRC_MIN_HARQ_PROC_FOR_DC_HSDPA) ||
    (tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc > TDSRRC_MAX_HARQ_PROC_FOR_DC_HSDPA))&&
    (tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_part_info_valid == FALSE))
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"DC-HSDPA:No. of HARQ processes %d out of range (%d to %d)",
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc,
      TDSRRC_MIN_HARQ_PROC_FOR_DC_HSDPA, TDSRRC_MAX_HARQ_PROC_FOR_DC_HSDPA);
    return FALSE;
  }

  if(tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_part_info_valid == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:HARQ info has Explicit memory partitioning ");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DC-HSDPA:HARQ info has Implicit memory partitioning");
  }

  return TRUE;
}

/*============================================================================
FUNCTION: tdsrrcllc_get_dc_hsdpa_action_in_config()

DESCRIPTION: This function returns DC-HSDPA action that's st in Config DB.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
tdsl1_hsdpa_dc_hsdpa_action_enum_type tdsrrcllc_get_dc_hsdpa_action_in_config
(
  void
)
{
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    return (tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.dc_hsdpa_action);
  }
  else
  {
    return (tdsordered_config_ptr->l1_hsdpa_info.hs_secondary_info.dc_hsdpa_action);
  }
}
#endif /*FEATURE_TDSCDMA_DC_HSDPA*/
#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
/*====================================================================
FUNCTION: tdsrrcllc_determine_hsdpa_action_r9()

DESCRIPTION:
  This function determines the HSDPA action and sets the parameters in L1
  structure accordingly.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  Sucess if HSDPA action and parameters have been set
  Failure Otherwise

SIDE EFFECTS:
====================================================================*/
uecomdef_status_e_type tdsrrcllc_determine_hsdpa_action_r9
(
  tdsrrc_DL_HSPDSCH_Information_r9* hspdsch_info_ptr
)
{
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  uint8 i = 0;
#endif
  
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  multicarrier_flg = tdsrrcllc_multicarrier_present_per_version_r9(hspdsch_info_ptr);
#endif

  /* Save UE category info */
  if (tdsnv_hsdpa_category > 15)
  {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if (0 == tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue)
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
#endif
    }
    else
    {
      for(i=0;i< tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue;i++)
      {
        if ((tdsordered_config_ptr->dl_macehs_logchan_info[i].queue_id != TDSINVALID_QUEUE_ID) &&
          (tdsordered_config_ptr->mac_ehs_lc_id_in_use[i] == TRUE ))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 23;
          break;
        }
        else
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#ifdef FEATURE_TDSCDMA_64QAM
          if (i == (tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue - 1))
          {
            tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = FALSE;
          }
#endif
        }
      }
    }
#else
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = 15;
#endif
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_cat = tdsnv_hsdpa_category;
  }
  
  /* HSDPA params have already been set in rrc_hsdpa_params.
  Check if HSDPA is already ACTIVE */
  if (tdsrrc_get_hsdpa_status() == TDSHSDPA_INACTIVE)
  {
    /* HSDPA is currently inactive */
    /* Check if network wants to activate HSDPA */
    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {
      if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
      {
        /* Check if channelization codes and feedback info present */
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r9,hs_scch_Info)) 
        {
          if (tdsrrcllcpcie_set_hsscch_info_r9(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                              , multicarrier_flg
#endif
            ) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: Invalid HS-SCCH info.");
            return FAILURE;
          }
        }
        if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r9,measurement_feedback_Info))
        {
          /* Set Meas Feedback Info */
          if (tdsrrcllc_set_meas_feedback_info_r7(&hspdsch_info_ptr->measurement_feedback_Info) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Invalid Meas Feedback info for HS.");
            return FAILURE;
          }
        }
        
        if (TDSRRC_CHECK_MSG_TYPE(hspdsch_info_ptr->modeSpecificInfo,
            T_tdsrrc_DL_HSPDSCH_Information_r9_modeSpecificInfo_tdd))
        {
          if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd,
            rrc_DL_HSPDSCH_Information_r9_tdd_tdd128))
          {
            /* Check if HS-PDSCH Midamble Configuration present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                hs_PDSCH_Midamble_Configuration_tdd128))
            {
              tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
                &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
                hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                  , multicarrier_flg
#endif
                );
              TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Receive midamble configuration tdd128");
            }
            /* Check if DL Multi-Carrier Information present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                dl_MultiCarrier_Information))
            {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
              if(FAILURE == tdsrrcllc_check_multicarrier_support_per_version(&hspdsch_info_ptr->
                  modeSpecificInfo.u.tdd->u.tdd128->dl_MultiCarrier_Information, NULL, FALSE))
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
                return FAILURE;
              }
#else
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport IE DL Multi-Carrier Information reveived");
              return FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
            }
            
            /* Check if TS0 Indicator present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                tS0_Indicator))
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport IE TS0 Indicator reveived");
              return FAILURE;
            }

            /* Check if DL Out of Sync window present */
            if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
                hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
                outofSyncWindow))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive IE Out of Sync window");
              //TDSRRC_MSG3(MSG_LEGACY_ERROR,"RRCHS:unsupport IE Out of Sync window reveived",0,0,0);
              //return FAILURE;
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
          return FAILURE;
        }
      }
    } /* End of if of hsdpa message params absent */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:TDSHSDPA_NOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_reset_mac_ehs_params(tdsordered_config_ptr);
#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
      return SUCCESS;
    }

    /* Set Queue Info */
    if (tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Unable to set Queue Info");
      return FAILURE;
    }

#ifdef FEATURE_TDSCDMA_64QAM
   if(tdscurrent_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured !=
           tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured)
   {
     tdshsdpa_msg_params.hsdsch_info_present = TRUE;
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set HS-DSCh bitmask as 64 QAM config is changed ");
   }
#endif

    /* Final Check to make sure that everything is present */
    if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
    {
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      tdsrrcllc_reset_mac_ehs_params(tdsordered_config_ptr);
#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:TDSHSDPA_NOOP");
      return SUCCESS;
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:TDSHSDPA_START");
    tdsrrc_set_hsdpa_action(TDSHSDPA_START);

    /* Set Bit Mask */
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = TDSL1_CPHY_HS_DSCH_CFG_INCL | 
      TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;

    /* In case UTRAN messes up, set HS-RESET to FALSE */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"REL9:Force HS-RESET to FALSE, hs_req_mask 0X%x",
                                 tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask);
    tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator = FALSE;
    return SUCCESS;
  }
  /*The following deals with the scenarios when HSDPA is already ACTIVE and HS action needs to be decided.*/
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:HSDPA already active");

    if (tdshsdpa_msg_params.msg_params_present == TRUE)
    {
      return tdsrrcllc_determine_hsdpa_action_for_hsdpa_active_r9(hspdsch_info_ptr);
    }
    else
    {
      /*It means that HSDPA ia already ACTIVE but OTA hasn't provided the HSDPA parameters*/
      /* So Final Check to make sure that everything is present */
      if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: TDSHSDPA_STOP");
        tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
        return SUCCESS;
      }

      /* no matter what freq change, should reconfig HSDPA */
      if ((tdsordered_config_ptr->is_hho== TRUE)
        ||(tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9: FORCED Freq change with TDSHSDPA_RECONFIG");
        tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
        return SUCCESS;
      }

      /* Means No Action has to be taken for HSDPA. i.e.*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:TDSHSDPA_NOOP");
      tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);

      return SUCCESS;
    }
  }
}
/*====================================================================
FUNCTION: tdsrrcllc_determine_hsdpa_action_for_hsdpa_active_r9()

DESCRIPTION:
  This function determines the HSDPA action when HSDPA ia already ACTIVE. 
  
DEPENDENCIES:
  None.

RETURN VALUE:
  Sucess if HSDPA action and parameters have been set
  Failure Otherwise

SIDE EFFECTS:
====================================================================*/
uecomdef_status_e_type tdsrrcllc_determine_hsdpa_action_for_hsdpa_active_r9
(
  tdsrrc_DL_HSPDSCH_Information_r9* hspdsch_info_ptr
)
{
  uint8 reconfig_bit_mask = 0;
  boolean hsscch_codes_changed = FALSE;
  boolean hspdsch_midamble_changed = FALSE;  

#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  uint8 multicarrier_flg = FALSE;
  multicarrier_flg = tdsrrcllc_multicarrier_present_per_version_r9(hspdsch_info_ptr);
#endif

  /*InterF or IntraF HO is happening */
  if ((tdsordered_config_ptr->is_hho== TRUE)
    ||(tdsordered_config_ptr->w_freq != tdscurrent_config_ptr->w_freq))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:frequency change primary freq %d working freq &d Set HS-DSCH in bit_mask ",
           tdsordered_config_ptr->p_freq,tdsordered_config_ptr->w_freq);
    
    reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
  }

  if (tdshsdpa_msg_params.hspdsch_info_present == TRUE)
  {
    /*Process HS-SCCH Info*/
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r9,hs_scch_Info))
    {
      /* Check if HS-SCCH Info has changed */
      if (tdsrrcllcpcie_check_if_hsscch_changed_r9(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                                  , multicarrier_flg
#endif
        ) == TRUE)
      {
        hsscch_codes_changed = TRUE;
      }

      if (tdsrrcllcpcie_set_hsscch_info_r9(&hspdsch_info_ptr->hs_scch_Info
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                                          , multicarrier_flg
#endif
        ) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Invalid HS-SCCH info");
        return FAILURE;
      }
      else
      {
        if (hsscch_codes_changed == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:SCCH Info changed");
          reconfig_bit_mask =reconfig_bit_mask | TDSL1_CPHY_HS_SCCH_SICH_CFG_INCL;
        }
      }
    }

    /*Process Measurement Feedback Info.*/ 
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(hspdsch_info_ptr,tdsrrc_DL_HSPDSCH_Information_r9,measurement_feedback_Info))
    {
      if (tdsrrcllc_set_meas_feedback_info_r7(&hspdsch_info_ptr->measurement_feedback_Info) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9: Invalid Meas Feedback info for HS.");
        return FAILURE;
      }
    }
    if (TDSRRC_CHECK_MSG_TYPE(
        hspdsch_info_ptr->modeSpecificInfo,
        T_tdsrrc_DL_HSPDSCH_Information_r9_modeSpecificInfo_tdd))
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(
        hspdsch_info_ptr->modeSpecificInfo.u.tdd,
        rrc_DL_HSPDSCH_Information_r9_tdd_tdd128))
      {
        /* Check if HS-PDSCH Midamble Configuration present */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
            hs_PDSCH_Midamble_Configuration_tdd128))
        {
          /* Check if HS-PDSCH Midamble Configuration has changed */
          if (tdsrrcllc_check_if_hspdsch_midamble_changed(
            &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
            hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                              , multicarrier_flg
#endif
            ) == TRUE)
          {
            hspdsch_midamble_changed = TRUE;
          }
          tdsrrcllc_set_hspdsch_midamble_configuration_tdd128(
            &hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128->
            hs_PDSCH_Midamble_Configuration_tdd128
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
                              , multicarrier_flg
#endif
            );
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive midamble configuration tdd128");
    
          if (hspdsch_midamble_changed == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:hspdsch midamble configuration changed");
            reconfig_bit_mask =reconfig_bit_mask | TDSL1_CPHY_HS_PDSCH_MIDAMBLE_CFG_INCL;
          }
        }
        
        /* Check if DL Multi-Carrier Information present */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
            dl_MultiCarrier_Information))
        {
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
          if(FAILURE == tdsrrcllc_check_multicarrier_support_per_version(&hspdsch_info_ptr->
              modeSpecificInfo.u.tdd->u.tdd128->dl_MultiCarrier_Information, &reconfig_bit_mask, FALSE))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Multi-Carrier Information not match");
            return FAILURE;
          }
#else
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport IE DL Multi-Carrier Information reveived");
          return FAILURE;
#endif //FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
        }
        
        /* Check if TS0 Indicator present */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
            tS0_Indicator))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport IE TS0 Indicator reveived");
          return FAILURE;
        }
        
        /* Check if DL Out of Sync window present */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(
            hspdsch_info_ptr->modeSpecificInfo.u.tdd->u.tdd128,
            outofSyncWindow))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Receive IE Out of Sync window");
      //TDSRRC_MSG3(MSG_LEGACY_ERROR,"RRCHS:unsupport IE Out of Sync window reveived",0,0,0);
          //return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:unsupport chip rate received");
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD mode choice received");
      return FAILURE;
    }
    
  }/*HS-PDSCH Info is present in tdshsdpa_msg_params*/


  /* Final Check to make sure that everything is present */
  if (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:TDSHSDPA_STOP");
    tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
    return SUCCESS;
  }

  /* Set the queue information */
  if(tdsrrcllc_set_hsdpa_l1_queue_info() == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL9:Unable to set HSDPA info in L1");
    return FAILURE;
  }
  else
  {
    if((tdshsdpa_msg_params.hsdsch_info_present == TRUE) || 
      ((tdshsdpa_msg_params.hrnti_present == TRUE) && (tdshsdpa_msg_params.hrnti_changed == TRUE)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:TDSL1_CPHY_HS_DSCH_CFG_INCL set");
      reconfig_bit_mask = reconfig_bit_mask | TDSL1_CPHY_HS_DSCH_CFG_INCL;
    }
  }

  if ((reconfig_bit_mask == 0) && 
      (tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator == FALSE) &&
      (tdsordered_config_ptr->is_hho== FALSE)&&
      (tdsordered_config_ptr->w_freq == tdscurrent_config_ptr->w_freq))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:TDSHSDPA_NOOP");
    tdsrrc_set_hsdpa_action(TDSHSDPA_NOOP);
    return SUCCESS;
  }

  tdsordered_config_ptr->l1_hsdpa_info.hs_req_mask = reconfig_bit_mask;

  /* Set Action to RECONFIG */
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"REL9:TDSHSDPA_RECONFIG bit mask %d,HS-RESET IND %d",reconfig_bit_mask,
           tdsordered_config_ptr->mac_dl_parms.mac_hs_reset_indicator);
  tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);

  return SUCCESS;
}

#endif /*FEATURE_TDSCDMA_REL9*/

/*====================================================================
FUNCTION: tdsrrcllc_set_sib5_change_in_progress_flag()

DESCRIPTION:
  This function set sib5_change_in_progress_flag.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_set_sib5_change_in_progress_flag
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"set tds_is_sib5_change_in_progress flag to TRUE, curr val=%d",
      tds_is_sib5_change_in_progress);

  tds_is_sib5_change_in_progress = TRUE;
}
/*====================================================================
FUNCTION: tdsrrcllc_clear_sib5_change_in_progress_flag()

DESCRIPTION:
  This function clear sib5_change_in_progress_flag.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_clear_sib5_change_in_progress_flag
(
  void
)
{
  if(tds_is_sib5_change_in_progress)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"set is_sib5_change_in_progress flag to TRUE,curr val=%d",
       tds_is_sib5_change_in_progress);
  
    tds_is_sib5_change_in_progress = FALSE;
  }
}


/*====================================================================
FUNCTION: tdsrrcllc_get_sib5_change_in_progress_flag()

DESCRIPTION:
  This function clear sib5_change_in_progress_flag.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_get_sib5_change_in_progress_flag
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tds_is_sib5_change_in_progress val=%d",
     tds_is_sib5_change_in_progress);

  return (tds_is_sib5_change_in_progress);
}


/*====================================================================
FUNCTION: tdsrrcllc_set_r4_hsdpa_action()

DESCRIPTION:

  This function sets the HSDPA Action based on the contents of R4 Message.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  
====================================================================*/
void tdsrrcllc_set_r4_hsdpa_action
(
  void
)
{
  /* Check to make sure that everything is present */
  if (((tdshsdpa_msg_params.msg_params_present == TRUE)  &&
       (tdsordered_config_ptr->is_hho== TRUE)) || 
      (tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured(tdsordered_config_ptr) == FALSE))
  {
    tdsrrc_set_hsdpa_action(TDSHSDPA_STOP);
    tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = TDSHSDPA_STOP;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSSTOP");
  }
  else
  {
    
    if (tdsordered_config_ptr->is_hho== TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq change. HSAction RECONFIG");
      tdsrrc_set_hsdpa_action(TDSHSDPA_RECONFIG);
      tdsordered_config_ptr->mac_dl_parms.mac_hsdpa_action = TDSHSDPA_RECONFIG;
    }
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    tdsrrcllcpcie_update_hs_tfri_table_type_non_r7_ota();
#endif
  }
}

/*====================================================================
FUNCTION: tdsrrcllc_process_non_critcal_trch_reconfig_ie_r4

DESCRIPTION:
  This function acts on the NON Critical IE for RB Reconfig R4

  The functio is from tdsrrcllc_process_non_critcal_trch_reconfig_ie_r5

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_process_non_critcal_trch_reconfig_ie_r4
(
  tdsrrc_DL_DCCH_Message *dcch_ptr
)
{
   tdsrrc_TransportChannelReconfiguration_r4 *rrc_trch_reconfig = &dcch_ptr->message.u.transportChannelReconfiguration.u.
             later_than_r3.criticalExtensions.u.r4;
 
   if((rrc_trch_reconfig->m.v4d0NonCriticalExtensionsPresent) &&
        (rrc_trch_reconfig->v4d0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent) && 
          (rrc_trch_reconfig->v4d0NonCriticalExtensions.v590NonCriticalExtensions.m.
          v690NonCriticalExtensionsPresent))
   {
     if(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_trch_reconfig->v4d0NonCriticalExtensions.v590NonCriticalExtensions.
                                   v690NonCriticalExtensions.transportChannelReconfiguration_v690ext,
                                   tdsrrc_TransportChannelReconfiguration_v690ext_IEs,
                                   delayRestrictionFlag))
     {
       if((TDSRRC_MSG_COMMON_BITMASK_IE(rrc_trch_reconfig->transportChannelReconfiguration_r4,
             tdsrrc_TransportChannelReconfiguration_r4_IEs,
             ul_AddReconfTransChInfoList ))
           ||(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_trch_reconfig->transportChannelReconfiguration_r4,
                   tdsrrc_TransportChannelReconfiguration_r4_IEs,
                   dl_AddReconfTransChInfoList )
       ))
       {
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"Trch Ie touched bitmask , so Delay restriction cannot be applied");
       }
       else
       {
         tdsrrcllc_update_delay_restriction(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_trch_reconfig->transportChannelReconfiguration_r4,
           tdsrrc_TransportChannelReconfiguration_r4_IEs,
            activationTime),
              TRUE,
              rrc_trch_reconfig->transportChannelReconfiguration_r4.activationTime,
              rrc_trch_reconfig->transportChannelReconfiguration_r4.rrc_StateIndicator
           );
       }
     }
   }
}
/*====================================================================
FUNCTION: tdsrrcllc_process_non_critcal_phys_reconfig_ie_r4

DESCRIPTION:
  This function acts on the NON Critical IE for Phys Reconfig R5

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_process_non_critcal_phys_reconfig_ie_r4
(
  tdsrrc_DL_DCCH_Message *dcch_ptr
)
{
  tdsrrc_PhysicalChannelReconfiguration_r4 *rrc_phys_chan_reconfig = &dcch_ptr->message.u.physicalChannelReconfiguration.u.
          later_than_r3.criticalExtensions.u.r4;
  
  if((rrc_phys_chan_reconfig->m.v4d0NonCriticalExtensionsPresent) && 
      (rrc_phys_chan_reconfig->v4d0NonCriticalExtensions.m.v590NonCriticalExtenstionsPresent) &&
       (rrc_phys_chan_reconfig->v4d0NonCriticalExtensions.v590NonCriticalExtenstions.m.
          v690NonCriticalExtensionsPresent))
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_phys_chan_reconfig->v4d0NonCriticalExtensions.
          v590NonCriticalExtenstions.v690NonCriticalExtensions.
         physicalChannelReconfiguration_v690ext,tdsrrc_PhysicalChannelReconfiguration_v690ext_IEs,
                delayRestrictionFlag))
    {
      tdsrrcllc_update_delay_restriction(TDSRRC_MSG_COMMON_BITMASK_IE(
           rrc_phys_chan_reconfig->physicalChannelReconfiguration_r4,
              tdsrrc_PhysicalChannelReconfiguration_r4_IEs,activationTime),
              TRUE,
              rrc_phys_chan_reconfig->physicalChannelReconfiguration_r4.activationTime,
              rrc_phys_chan_reconfig->physicalChannelReconfiguration_r4.rrc_StateIndicator
           );
    }
  }
}
/*====================================================================
FUNCTION: tdsrrcllc_process_non_critcal_rb_reconfig_ie_r4

DESCRIPTION:
  This function acts on the NON Critical IE for RB Reconfig R4

  The function is copy from tdsrrcllc_process_non_critcal_rb_reconfig_ie_r5

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_process_non_critcal_rb_reconfig_ie_r4
(
  tdsrrc_DL_DCCH_Message *dcch_ptr
)
{
  tdsrrc_RadioBearerReconfiguration_r4 *rrc_rb_reconfig = &dcch_ptr->message.u.radioBearerReconfiguration.u.
           later_than_r3.criticalExtensions.u.r4;
 
  if((rrc_rb_reconfig->m.v4d0NonCriticalExtensionsPresent) &&
        (rrc_rb_reconfig->v4d0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent) && 
         (rrc_rb_reconfig->v4d0NonCriticalExtensions.v590NonCriticalExtensions.m.v5d0NonCriticalExtenstionsPresent) &&
          (rrc_rb_reconfig->v4d0NonCriticalExtensions.v590NonCriticalExtensions.v5d0NonCriticalExtenstions.m.
          v690NonCriticalExtensionsPresent))
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_rb_reconfig->v4d0NonCriticalExtensions.v590NonCriticalExtensions.
                                 v5d0NonCriticalExtenstions.v690NonCriticalExtensions.radioBearerReconfiguration_v690ext,
                                   tdsrrc_RadioBearerReconfiguration_v690ext_IEs,
                                   delayRestrictionFlag))
    {      
      if( (TDSRRC_MSG_COMMON_BITMASK_IE(rrc_rb_reconfig->radioBearerReconfiguration_r4,
          tdsrrc_RadioBearerReconfiguration_r4_IEs,dl_AddReconfTransChInfoList))
           ||(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_rb_reconfig->radioBearerReconfiguration_r4,
          tdsrrc_RadioBearerReconfiguration_r4_IEs,ul_AddReconfTransChInfoList))
          ||(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_rb_reconfig->radioBearerReconfiguration_r4,
          tdsrrc_RadioBearerReconfiguration_r4_IEs,ul_deletedTransChInfoList))
          ||(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_rb_reconfig->radioBearerReconfiguration_r4,
          tdsrrc_RadioBearerReconfiguration_r4_IEs,dl_DeletedTransChInfoList))
          )
     
      {
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"Trch Ie touched , so Delay restriction cannot be applied");
      }
      else
      {
        tdsrrcllc_update_delay_restriction(TDSRRC_MSG_COMMON_BITMASK_IE(rrc_rb_reconfig->radioBearerReconfiguration_r4,
          tdsrrc_RadioBearerReconfiguration_r4_IEs,activationTime),
                TRUE,
                rrc_rb_reconfig->radioBearerReconfiguration_r4.activationTime,
                rrc_rb_reconfig->radioBearerReconfiguration_r4.rrc_StateIndicator
             );
      }
    }
  }
}
#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/*====================================================================
FUNCTION: tdsrrcllc_get_target_state

DESCRIPTION:
  This function to get the current target state per each state.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrc_state_e_type tdsrrcllc_get_target_state(void)
{

  tdsrrc_proc_e_type proc_id = 0;
  tdsrrcllc_oc_process_state_e_type process_status = 0;
  
  tdsrrc_state_e_type target_state = TDSRRC_STATE_MAX;

  (void)tdsrrcllc_get_ordered_config_state_and_proc(&proc_id,&process_status );
  switch (proc_id)
  {
    case TDSRRC_PROCEDURE_RBE:
      target_state = tdsrrc_rbe.state_from_msg;
      break;
    case TDSRRC_PROCEDURE_RBRC:
      target_state = tdsrrc_rbrc.state_indicated_by_msg;
      break;
    case TDSRRC_PROCEDURE_RBR:
      target_state = tdsrrc_rbr.state_indicated_by_msg;
      break;
    case TDSRRC_PROCEDURE_TCR:
      target_state = tdsrrc_tcrc.state_indicated_by_msg;
      break;
    case TDSRRC_PROCEDURE_PCR:
      target_state = tdsrrc_pcrc.state_indicated_by_msg;
      break;
    default:
        target_state = TDSRRC_STATE_MAX;
      
  }
  return target_state;
}
#endif

#ifdef FEATURE_TDSCDMA_RRC_RB_PRIORITY_OPT
/*==========================================================
FUNCTION  : tdsrrcllc_find_cn_domain_from_rb_id()

DESCRIPTION : Find CN domain from RB id.

DEPENDENCIES: None.

RETURN VALUE: CN domain

SIDE EFFECTS: None.
============================================================*/
tdsrrcllc_cn_domain_enum_type tdsrrcllc_find_cn_domain_from_rb_id
(
  uint32 num_rabs_in_msg,
  tdsrrc_est_rab_info_type *rabs_in_msg,
  uint8 rb_id
)
{
  uint8 rb_count;
  uint8 rab_count;
  tdsrrcllc_cn_domain_enum_type cn_domain = TDSRRC_CN_DOMAIN_NONE;

  /* Check whether this RB exists in established_rab db */
  for (rab_count = 0; (rab_count < MAX_RAB_TO_SETUP) && (rab_count < num_rabs_in_msg); rab_count++)
  {
    /* Compare each rb-id in this established rab with the rb-id received in the message */
    for (rb_count = 0; (rb_count < MAX_RB_PER_RAB) && (rb_count < rabs_in_msg[rab_count].num_rbs_for_rab); rb_count++)
    {
      if (rabs_in_msg[rab_count].rb_for_rab[rb_count].rb_id == rb_id)
      {
        return (tdsrrcllc_cn_domain_enum_type)(rabs_in_msg[rab_count].cn_domain); 
      }
    }/* rb_count loop */
  }/* rab_count loop */
  return cn_domain;
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_cn_domain_from_rb_id()

DESCRIPTION : Get CN domain from RB id based on proc.

DEPENDENCIES: None.

RETURN VALUE: CN domain

SIDE EFFECTS: None.
============================================================*/
tdsrrcllc_cn_domain_enum_type tdsrrcllc_get_cn_domain_from_rb_id
(
  uint8 rb_id
)
{
  rrc_cn_domain_identity_e_type cn_domain;

  switch (tdsordered_config.set_by_proc) 
  {
    case TDSRRC_PROCEDURE_RBE:
        if(tdsrrc_rbe.rabs.num_rabs_in_msg <= TDSRRCRB_MAX_RAB_PER_MSG)
        {
            return tdsrrcllc_find_cn_domain_from_rb_id(tdsrrc_rbe.rabs.num_rabs_in_msg,tdsrrc_rbe.rabs.rabs_in_msg,rb_id);
        }
        else
        {
            return tdsrrcllc_find_cn_domain_from_rb_id(TDSRRCRB_MAX_RAB_PER_MSG,tdsrrc_rbe.rabs.rabs_in_msg,rb_id);
        }
    case TDSRRC_PROCEDURE_RBRC:
    case TDSRRC_PROCEDURE_RBR:
      if (tdsrrc_find_cn_domain_id_for_rb_in_est_rabs(rb_id, &cn_domain) == TRUE)
      {
	return (tdsrrcllc_cn_domain_enum_type)(cn_domain);
      }
      else
      {
	TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't find rb_id %d in existing RABs",rb_id);
      }
      break;
#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_PROCEDURE_IHO:
      return tdsrrcllc_find_cn_domain_from_rb_id(g2td_info.number_of_est_rabs,
					         g2td_info.est_rabs,
					         rb_id);
#endif
    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"No priority adjustment for proc %d", tdsordered_config.set_by_proc);
      break;
  }
  return TDSRRC_CN_DOMAIN_NONE;
}

/*==========================================================
FUNCTION  : tdsrrcllc_adjust_oc_urb_priority()

DESCRIPTION : Adjust User RB MAC logical channel priority in OC.

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
void tdsrrcllc_adjust_oc_urb_priority
(
  uint8 rb_id,
  uint8 *priority_ptr
)
{
  tdsrrcllc_cn_domain_enum_type cn_domain;

  /* For user RBs, check cn_domain and set priority */
  cn_domain = tdsrrcllc_get_cn_domain_from_rb_id(rb_id);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Current processing cn_domain %d (0-CS, 1-PS, 2-NONE)",cn_domain);
  switch (cn_domain) 
  {
    case TDSRRC_CN_DOMAIN_CS:
      *priority_ptr = TDSRRC_CS_RB_PRIORITY;
      break;
    case TDSRRC_CN_DOMAIN_PS:
      if (*priority_ptr <= TDSRRC_MAX_MAC_LOGICAL_CHAN_PRIORITY) 
      {
	*priority_ptr += TDSRRC_PS_RB_PRIORITY_OFFSET;
      }
      break;
    default:
      break;
  }
}

/*==========================================================
FUNCTION  : tdsrrcllc_adjust_oc_srb_priority()

DESCRIPTION : Adjust SRB MAC logical channel priority in OC.

DEPENDENCIES: None.

RETURN VALUE: SRB priority

SIDE EFFECTS: None.
============================================================*/
void tdsrrcllc_adjust_oc_srb_priority
(
  uint8 rb_id,
  uint8 *priority_ptr
)
{
  /* SRB priority can be set directly based on rb_id */
  switch (rb_id)
  {
    case DCCH_UM_RADIO_BEARER_ID:
      *priority_ptr = (uint8)TDSRRC_SRB1_PRIORITY;
      break;
    case DCCH_AM_RADIO_BEARER_ID:
      *priority_ptr = (uint8)TDSRRC_SRB2_PRIORITY;
      break;
    case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
      *priority_ptr = (uint8)TDSRRC_SRB3_PRIORITY;
      break;
    case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
      *priority_ptr = (uint8)TDSRRC_SRB4_PRIORITY;
      break;
    default:
      break;
  }
}

/*==========================================================
FUNCTION  : tdsrrcllc_adjust_mac_logical_channel_priority()

DESCRIPTION : Adjust MAC logical channel priority in OC.

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
void tdsrrcllc_adjust_oc_mac_logical_channel_priority
(
  void
)
{
  uint8 rb_id;
  uint8 trch_idx;
  uint8 logch_idx;
  uint8 *priority_ptr = NULL;
  tdsmac_ul_cctrch_config_type *ul_cctrch_ptr = &tdsordered_config_ptr->ul_cctrch;

  /* Adjust dch_info in ul_cctrch */
  for (trch_idx = 0;(trch_idx < ul_cctrch_ptr->dch_info.ndchs) && (trch_idx < TDSUE_MAX_TRCH);trch_idx++) 
  {
    for (logch_idx = 0;(logch_idx < ul_cctrch_ptr->dch_info.dch_info[trch_idx].ndlchan) && (logch_idx < MAX_DED_LOGCHAN_PER_TRCH);logch_idx++) 
    {
      rb_id = ul_cctrch_ptr->dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rb_id;
      priority_ptr = &ul_cctrch_ptr->dch_info.dch_info[trch_idx].dlchan_info[logch_idx].priority;
      if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
	      tdsrrcllc_adjust_oc_srb_priority(rb_id, priority_ptr);
      }
      else
      {
	      tdsrrcllc_adjust_oc_urb_priority(rb_id, priority_ptr);
      }
      TDSRRC_MSG2(MSG_LEGACY_MED,"Adjust rb_id %d priority to %d in dch_info",rb_id,*priority_ptr);
    }
  }

  /* Adjust rach_info in ul_cctrch */
  for (logch_idx = 0;(logch_idx < ul_cctrch_ptr->rach_info.ndlchan) && (logch_idx < MAX_DED_LOGCHAN_PER_TRCH);logch_idx++) 
  {
    rb_id = ul_cctrch_ptr->rach_info.dlchan_info[logch_idx].rb_id;
    priority_ptr = &ul_cctrch_ptr->rach_info.dlchan_info[logch_idx].priority;
    if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
    {
      tdsrrcllc_adjust_oc_srb_priority(rb_id, priority_ptr);
    }
    else
    {
      tdsrrcllc_adjust_oc_urb_priority(rb_id, priority_ptr);
    }
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adjust rb_id %d priority to %d in rach_info",rb_id,*priority_ptr);
  }
}
#endif

/*====================================================================
FUNCTION: tdsrrcllc_get_ul_ps_data_rate()

DESCRIPTION:
  This is called when the uplink rate for a particular rb is needed
  by DS

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The uplink rate (bps) of the rb passed in.

SIDE EFFECTS:
  None
===================================================================*/
uint32 tdsrrcllc_get_ul_ps_data_rate
(
  tdsrrc_state_e_type  rrc_state,
  tdsrrc_RB_Identity  rb_id
)
{
  uint32 data_size = TDSRRCLLC_INVALID_RLC_SIZE;
  uint32 trch_count;
  uint32 tf_count;
  uint32 dlch_count;
  uint8 rlc_hdr_size = 0;
  boolean  rb_found = FALSE;
  uint32 rate = 0;
  uint32 tti_in_ms = 0;
  uint8 mac_hdr_size = 0;

  tdsordered_config_type  *config_ptr;

  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }
  if(NULL == config_ptr)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Config ptr is NULL, returing zero");
    return 0;
  }

  /* If state is ura_pch or cell_pch, and prev state is DCH, look into TOC */
  if ((rrc_state == TDSRRC_STATE_CELL_PCH) || (rrc_state == TDSRRC_STATE_URA_PCH)) 
  {
    if (TDSRRC_STATE_CELL_DCH == tdsrrc_get_prev_state()) 
    {
      config_ptr = tdstransition_config.toc_ptr;
      rrc_state =  TDSRRC_STATE_CELL_DCH;
    }
    else
    {
      /*make rrc_state as CELL_FACH to to look at dtch-fach mapping*/
      rrc_state =  TDSRRC_STATE_CELL_FACH;
    }
  }

  /* Note: UL UM could only be a dedicated logical channel */
  if(rrc_state == TDSRRC_STATE_CELL_DCH)
  {
#ifdef FEATURE_TDSCDMA_HSUPA
    if (TRUE == tdsrrc_is_eul_active())
    {
      /* For UPA, get rate from L1 API */
      rate = tdsl1_max_allowed_upa_rate();
    }
    else
#endif /* FEATURE_TDSCDMA_HSUPA */
    {
    for(trch_count = 0;
        (trch_count < config_ptr->ul_tfcs.nchan) && (rb_found == FALSE);
        trch_count ++)
    {
      for(dlch_count = 0;
          (dlch_count < config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].ndlchan) && (rb_found == FALSE);
          dlch_count ++)
      {
        if (config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].ndlchan > 1)
        {
          /*Multiple dedicated logical channels mapped on DCH */  
          mac_hdr_size = TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
        }
        else
        {
          /*Single dedicated logical channel on DCH */
          mac_hdr_size = TDSRRCLLC_DED_SINGLE_LOG_CHL_DCH_MAC_HDR_SIZE;
        }

        if(config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].dlchan_info[dlch_count].rb_id == rb_id)
        {
          data_size = 0;
          rb_found = TRUE;
          /*RLC header is updated based on the RLC mode of the RB*/
          if((config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED) ||
	     (config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_DATA) ||
	     (config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_CONTROL))
          {
            rlc_hdr_size = 16;
          }
          else if(config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].dlchan_info[dlch_count].rlc_mode == UE_MODE_UNACKNOWLEDGED)
          {
            rlc_hdr_size = 8;
          }
          else if(config_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[trch_count].dlchan_info[dlch_count].rlc_mode == UE_MODE_TRANSPARENT)
          {
            rlc_hdr_size = 0;
          }          
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"Wrong RLC Mode %d for RB %d",config_ptr->mac_ul_parms.cctrch_ptr->dch_info.
              dch_info[trch_count].dlchan_info[dlch_count].rlc_mode,rb_id);
          }         
          
          /* Get the RLC size */
          for(tf_count = 0; tf_count < config_ptr->ul_tfcs.tfs_info[trch_count].ntf; tf_count ++)
          {
            if((uint32)((config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].tb_size - rlc_hdr_size - mac_hdr_size) * 
			 config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks) > data_size)
            {
              /* In Uplink the Transport format set in Ordered Config is
               * in terms of RLC PDU size as sent in RRC message.
               */
              data_size = (uint32)(config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].tb_size - rlc_hdr_size - mac_hdr_size) * 
		config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks ;
              tti_in_ms = tdsrrcllc_convert_tti_to_ms(config_ptr->ul_tfcs.tfs_info[trch_count].tti_info); 
              rate = ((data_size * 1000)/tti_in_ms);
            }
          }  /* For all TFs of a TrCH */
        }  /* If this TrCH has the same RB ID mapped to it */
      } /* For all dedicated logical channels */
    } /* For all Transport Channels */
    }/* UPA not active*/
  } /* State is DCH */
  else if(rrc_state == TDSRRC_STATE_CELL_FACH)
  {
    for(trch_count = 0; (trch_count < config_ptr->ul_tfcs.nchan) && (rb_found == FALSE); trch_count ++)
    {
      for(dlch_count = 0; (dlch_count < config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ndlchan) && (rb_found == FALSE); dlch_count ++)
      {
	/* MAC header size should be 26 bits for DTCH-RACH */  
        mac_hdr_size = 26;

	/* Get RLC header size */
        if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rb_id == rb_id)
        {
          data_size = 0;
          rb_found = TRUE;
          if((config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED) ||
	     (config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_DATA) ||
	     (config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rlc_mode == UE_MODE_ACKNOWLEDGED_CONTROL))
          {
            rlc_hdr_size = 16;
          }
          else if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rlc_mode == UE_MODE_UNACKNOWLEDGED)
          {
            rlc_hdr_size = 8;
          }
          else if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rlc_mode == UE_MODE_TRANSPARENT)
          {
            rlc_hdr_size = 0;
          }          
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"Wrong RLC Mode %d for RB %d",config_ptr->mac_ul_parms.cctrch_ptr->
              rach_info.dlchan_info[dlch_count].rlc_mode,rb_id);
          }
	            
          for(tf_count = 0; tf_count < config_ptr->ul_tfcs.tfs_info[trch_count].ntf; tf_count ++)
          {
            /* If the tb_size less the mac_hdr_size is bigger than the
            rlc_size, then we need to adjust the rlc_size up.  This
            comparison is written with respect to zero to avoid a lint
            warning about comparing signed and unsigned values, since a
            subtraction always results in a signed value and the rlc_size
            is an unsigned value. */
            if((uint32)((config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].tb_size - rlc_hdr_size - mac_hdr_size) * 
			 config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks) > data_size)
            {
              data_size = (uint32)(config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].tb_size - rlc_hdr_size - mac_hdr_size) * 
                config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_count].ntblks;
              tti_in_ms = tdsrrcllc_convert_tti_to_ms(config_ptr->ul_tfcs.tfs_info[trch_count].tti_info);
              rate = ((data_size * 1000)/tti_in_ms);
            }
          }  /* For all TFs of a TrCH */
        } /* if rb_id is found */
      } /* for all dedicated logical channel*/
    } /* For all Transport Channels */
  } /* State is FACH */

  if(data_size == 0)
  {
    TDSRRC_MSG3(MSG_LEGACY_ERROR,"Rate not determined for RBid:%d, curr_rrc_st:%d next_rrc_st:%d", rb_id,tdsrrc_get_state(),rrc_state);
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"UL Rate for rb_id %d is %d",rb_id, rate);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"data size %d tti %d", data_size, tti_in_ms);
  return rate;
}

#ifndef __MCPM_API_H__
#define __MCPM_API_H__

/*=========================================================================

           M O D E M   C L O C K   A N D   P O W E R   M A N A G E R

                E X T E R N A L   H E A D E R   F I L E


  GENERAL DESCRIPTION
    This file contains the external interface functions and definitions for
    Modem Clock and Power Manager (MCPM).

  EXTERNALIZED FUNCTIONS
    MCPM_Config_Modem

  INITIALIZATION AND SEQUENCING REQUIREMENTS
    Invoke the MCPM_Init function to initialize the MCPM.

        Copyright (c) 2021 by QUALCOMM Technologies, Inc.  All Rights Reserved.


==========================================================================*/

/*==========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mpower.mpss/13.1/api/mcpm_api.h#17 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------
11/15/19   sb      concurrency manager changes
10/14/19   xd      restore the enum of mcpm_request_type 
05/31/19   dk      MCVS params changes to support clk_plan2.0
07/26/17   sk      support L+L
07/18/16   sw      add MCPM_MCVS_MPPS_Q6_UPDATE in MCVS_CONFIG update_info to indicate whether mpps_q6 is requested.
12/18/15   hg      Added new GPS requests for common code changes.
11/27/15   pp      Added Support for W/G/TDS INIT Feature
08/26/15   dk      Updating QDSS event mapping, adding API to check if QDSS is enabled
07/15/15   ne      Added enums for W1 requests
05/15/15   sz      Added W21X and T21X support.
04/13/15   vr      MCPM GPIO Debug API  fix for TA  builds
10/21/14   pc      Addition of Tabasco specific resources
10/17/14   mh      Remove WCDMA EDRX request enums
10/16/14   cl      Adding support for Q6 345.6MHz and 370MHz
10/14/14   mh      Fixed merge issues
09/04/14   sc      Added support for CMCC power optimization feature.
08/11/14   ss      Map LTE data to LTE FDD data
07/17/14   ss      Add MCVS parameters for Scalar and Vector Q6 processors
06/19/14   cl      Add API to support W-L1 team query for Q6 speed
05/04/14   bd      GPIO Protection API Support
05/05/14   cl      First Jolokia official release branch off Bolt tip
04/25/14   ls      FW_VPE MCVS client to support LTE NLIC.
03/21/14   sr      G + G DSDS multi-sim support.
02/13/14   ls      LTE VoLTE and CDRX support.
01/17/14   ls      Support for LTE's MCPM request for TDD and FDD DATA on Bolt.
09/27/13   sr      Corrected formatting of some comments.
09/03/13   ls      Added a new clk_mdm freq enum.
07/31/13   sr      Removed GSM apis
07/26/13   sr      Added enum MCPM_GSM_DL_NONEV.
07/08/13   ls      Added more APIs (request type, DL/UL/BW data rate).
02/11/13   vs      Ported from Dime release branch.


==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/


#include "customer.h"
#include "comdef.h"
#include "msg.h"
#include "npa.h"
#include "icbarb.h"
#include "rfm_mode_types.h"
#include "mplm_api.h"
#include "voltage_level.h"

/*=========================================================================
      Macros for the MCPM clk/pll dump
==========================================================================*/

#define MCPM_DUMP_CLK 0x00000001
#define MCPM_DUMP_BLK 0x00000002
#define MCPM_DUMP_PWRUP 0x00000004


#define MCPM_DO_NOP_REQ            MCPM_DO_PARMS_UPDATE_REQ
#define MCPM_DO_START_DATA_TX_REQ  MCPM_DO_START_DATA_REQ
#define MCPM_DO_STOP_DATA_TX_REQ   MCPM_DO_STOP_DATA_REQ

/*
 * Bit 0: "RECEIVER_CONFIG" field is updated
 * Bit 1: "NEIGHBOR_MEAS" field is updated
 * Bit 2: "DL_DATA_RATE" field is updated
 * Bit 3: "UL_DATA_RATE" field is updated
 * Bit 4: "RF_BANDWIDTH" field is updated
 */
#define MCPM_NO_UPDATE                 0x0000
#define MCPM_RECEIVER_CONFIG_UPDATE    0x0001
#define MCPM_NEIGHBOR_MEAS_UPDATE      0x0002
#define MCPM_DL_DATA_RATE_UPDATE       0x0004
#define MCPM_UL_DATA_RATE_UPDATE       0x0008
#define MCPM_RF_BANDWIDTH_UPDATE       0x0010
#define MCPM_WKTIME_RF_UPDATE          0x0020
#define MCPM_WKTIME_RSRC_UPDATE        0x0040
#define MCPM_FCW_DCO_RESET_UPDATE      0x0080
#define MCPM_MODEM_PERF_MODE_UPDATE    0x0100
#define MCPM_MCVS_SCALE_UPDATE         0x0200
#define MCPM_NUM_PORT_UPDATE           0x0400
#define MCPM_TM_UPDATE                 0x0800
#define MCPM_DUPLEX_CFG_UPDATE         0x1000


/*
 * MCVS Param update info
 */
#define MCPM_MCVS_VPE_UPDATE           0x001
#define MCPM_MCVS_CLKBUS_UPDATE        0x002
/* Requests for Scalar Q6 clock change */
#define MCPM_MCVS_Q6_CLK_UPDATE           0x004
#define MCPM_MCVS_SCALAR_PROC_CLK_UPDATE  MCPM_MCVS_Q6_CLK_UPDATE 
/* Requests for Vector Q6 (Silver) clock change */
#define MCPM_MCVS_VECTOR_PROC_CLK_UPDATE 0x008
#define MCPM_MCVS_SILVER_CLK_UPDATE MCPM_MCVS_VECTOR_PROC_CLK_UPDATE
#define MCPM_MCVS_FW_VPE_UPDATE          0x010 //temporary for compilation - check
/* request for mpps_q6 change */
//#define MCPM_MCVS_MPPS_Q6_UPDATE         0x020
/* request for scenario info sharing*/
#define MCPM_MCVS_SCENARIO_INFO_UPDATE          0x040
#define MCPM_MCVS_CLIENT_SCENARIO_INFO_UPDATE   0x080
#define MCPM_MCVS_BOOST_SCENARIO_BITS           0xFFFFFF00
#define MCPM_MCVS_BOOST_SCENARIO_BIT_SHIFTS     8
//#define MCPM_MCVS_TIMED_SCENARIO_UPDATE         0x80

/*
 * RECEIVER_CONFIG
 * Bit 0: RXD (ON/OFF)
 * Bit 1: EQ (ON/OFF)
 * Bit 2: QICE (ON/OFF)
 * Bit 3: W-MIMO (ON/OFF)
 */

#define MCPM_RECEIVER_CONFIG_RXD       0x01  /* 1 rxd ON, 0 rxd OFF */
#define MCPM_RECEIVER_CONFIG_EQ        0x02  /* 1 EQ ON, 0 EQ OFF */
#define MCPM_RECEIVER_CONFIG_QICE      0x04  /* 1 QICE ON, 0 QICE OFF */
#define MCPM_RECEIVER_CONFIG_W_MIMO    0x08  /* 1 W MIMO CALL enabled */


#define MCPM_RECEIVER_CONFIG_RXD_SHIFT    0
#define MCPM_RECEIVER_CONFIG_EQ_SHIFT     1
#define MCPM_RECEIVER_CONFIG_QICE_SHIFT   2
#define MCPM_RECEIVER_CONFIG_W_MIMO_SHIFT 3

#define MCPM_MODEM_WCDMA_PERF_NONE      0x0
#define MCPM_MODEM_WCDMA_HIGH_PERF_MODE 0x1 /* OFFLINE clock to 144 for Nikel */


/*=========================================================================
      Typedefs
==========================================================================*/

/*
 * mcpm_request_type
 *
 * Requests to MCPM
 *
 * START_REQ should be sent when a technology is activated.
 *
 * STOP_REQ should be sent when a technology is deactivated.
 *
 * WAKE_UP_REQ should be sent when coming out sleep to decode a page indicator
 *
 * GO_TO_SLEEP_REQ should be sent when going into sleep.
 *
 * GO_TO_PSEUDO_SLEEP should be sent by 1X tech if RTC needs to be kept ON during sleep
 *
 * IDLE_REQ should be sent when the UE moves from page indicator decode
 * to setup paging channel. 1X technology also calls this to move from
 * 1X VOICE and 1X DATA calls.
 *
 * VOICE_START_REQ should be sent on start of a voice call.
 *
 * VOICE_STOP_REQ should be sent on end of a voice call.
 *
 * DATA_START_REQ should be sent on start of a data call
 *
 * DATA_STOP_REQ should be sent on end of a data call
 *
 */

typedef enum
{
  /* 1X Requests */
  MCPM_1X_START_REQ                                   = 0,
  MCPM_1X_STOP_REQ                                    = 1,
  MCPM_1X_WAKE_UP_REQ                                 = 2,
  MCPM_1X_GO_TO_SLEEP_REQ                             = 3,
  MCPM_1X_GO_TO_PSEUDO_SLEEP_REQ                      = 4,
  MCPM_1X_IDLE_REQ                                    = 5,
  MCPM_1X_VOICE_REQ                                   = 6,
  MCPM_1X_DATA_REQ                                    = 7,
  MCPM_1X_PARMS_UPDATE_REQ                            = 8,

  /* DO Requests */
  MCPM_DO_START_REQ                                   = 9,
  MCPM_DO_STOP_REQ                                    = 10,
  MCPM_DO_WAKE_UP_REQ                                 = 11,
  MCPM_DO_GO_TO_SLEEP_REQ                             = 12,
  MCPM_DO_IDLE_REQ                                    = 13,
  MCPM_DO_START_DATA_REQ                              = 14,
  MCPM_DO_STOP_DATA_REQ                               = 15,
  MCPM_DO_PARMS_UPDATE_REQ                            = 16,

  /* GERAN Requests */
  MCPM_GERAN_START_REQ                                = 17,
  MCPM_GERAN_STOP_REQ                                 = 18,
  MCPM_GERAN_INIT_REQ                                 = 19,
  MCPM_GERAN_WAKE_UP_REQ                              = 20,
  MCPM_GERAN_GO_TO_SLEEP_REQ                          = 21,
  MCPM_GERAN_IDLE_REQ                                 = 22,
  MCPM_GERAN_VOICE_START_REQ                          = 23,
  MCPM_GERAN_VOICE_STOP_REQ                           = 24,
  MCPM_GERAN_DATA_START_REQ                           = 25,
  MCPM_GERAN_DATA_STOP_REQ                            = 26,
  MCPM_GERAN_PARMS_UPDATE_REQ                         = 27,

  /* GERAN1 Requests */
  MCPM_GERAN1_START_REQ                               = 28,
  MCPM_GERAN1_STOP_REQ                                = 29,
  MCPM_GERAN1_INIT_REQ                                = 30,
  MCPM_GERAN1_WAKE_UP_REQ                             = 31,
  MCPM_GERAN1_GO_TO_SLEEP_REQ                         = 32,
  MCPM_GERAN1_IDLE_REQ                                = 33,
  MCPM_GERAN1_VOICE_START_REQ                         = 34,
  MCPM_GERAN1_VOICE_STOP_REQ                          = 35,
  MCPM_GERAN1_DATA_START_REQ                          = 36,
  MCPM_GERAN1_DATA_STOP_REQ                           = 37,
  MCPM_GERAN1_PARMS_UPDATE_REQ                        = 38,

  /* LTE Requests */
  MCPM_LTE_START_REQ                                  = 39,
  MCPM_LTE_STOP_REQ                                   = 40,
  MCPM_LTE_INIT_REQ                                   = 41,
  MCPM_LTE_ACQ_REQ                                    = 42,
  MCPM_LTE_WAKE_UP_REQ                                = 43,
  MCPM_LTE_GO_TO_SLEEP_REQ                            = 44,
  MCPM_LTE_IDLE_REQ                                   = 45,
  MCPM_LTE_DATA_START_REQ                             = 46,
  MCPM_LTE_FDD_DATA_START_REQ                         = 47,
  MCPM_LTE_TDD_DATA_START_REQ                         = 48,
  MCPM_LTE_FDD_VOLTE_DATA_START_REQ                   = 49,
  MCPM_LTE_TDD_VOLTE_DATA_START_REQ                   = 50,
  MCPM_LTE_GO_TO_LIGHT_SLEEP_REQ                      = 51,
  MCPM_LTE_GO_TO_LONG_LIGHT_SLEEP_REQ                 = 52,
  MCPM_LTE_GO_TO_LIGHT_SLEEP_NO_MODEM_FREEZE_REQ      = 53,
  MCPM_LTE_DATA_STOP_REQ                              = 54,
  MCPM_LTE_PARMS_UPDATE_REQ                           = 55,

  /* TDSCDMA Requests */
  MCPM_TDSCDMA_START_REQ                              = 56,
  MCPM_TDSCDMA_STOP_REQ                               = 57,
  MCPM_TDSCDMA_INIT_REQ                               = 58,
  MCPM_TDSCDMA_ACQ_REQ                                = 59,
  MCPM_TDSCDMA_WAKE_UP_REQ                            = 60,
  MCPM_TDSCDMA_GO_TO_SLEEP_REQ                        = 61,
  MCPM_TDSCDMA_IDLE_REQ                               = 62,
  MCPM_TDSCDMA_VOICE_START_REQ                        = 63,
  MCPM_TDSCDMA_VOICE_STOP_REQ                         = 64,
  MCPM_TDSCDMA_DATA_START_REQ                         = 65,
  MCPM_TDSCDMA_DATA_STOP_REQ                          = 66,
  MCPM_TDSCDMA_PARMS_UPDATE_REQ                       = 67,

  /* WCDMA Requests */
  MCPM_WCDMA_START_REQ                                = 68,
  MCPM_WCDMA_STOP_REQ                                 = 69,
  MCPM_WCDMA_INIT_REQ                                 = 70,
  MCPM_WCDMA_WAKE_UP_REQ                              = 71,
  MCPM_WCDMA_GO_TO_SLEEP_REQ                          = 72,
  MCPM_WCDMA_IDLE_REQ                                 = 73,
  MCPM_WCDMA_CDRX_GO_TO_LIGHT_SLEEP_REQ               = 74,
  MCPM_WCDMA_VOICE_START_REQ                          = 75,
  MCPM_WCDMA_VOICE_STOP_REQ                           = 76,
  MCPM_WCDMA_DATA_START_REQ                           = 77,
  MCPM_WCDMA_DATA_STOP_REQ                            = 78,
  MCPM_WCDMA_PARMS_UPDATE_REQ                         = 79,

  /* WCDMA Requests */
  MCPM_WCDMA1_START_REQ                               = 80,
  MCPM_WCDMA1_STOP_REQ                                = 81,
  MCPM_WCDMA1_INIT_REQ                                = 82,
  MCPM_WCDMA1_WAKE_UP_REQ                             = 83,
  MCPM_WCDMA1_GO_TO_SLEEP_REQ                         = 84,
  MCPM_WCDMA1_IDLE_REQ                                = 85,
  MCPM_WCDMA1_VOICE_START_REQ                         = 86,
  MCPM_WCDMA1_VOICE_STOP_REQ                          = 87,
  MCPM_WCDMA1_DATA_START_REQ                          = 88,
  MCPM_WCDMA1_CDRX_GO_TO_LIGHT_SLEEP_REQ              = 89,
  MCPM_WCDMA1_DATA_STOP_REQ                           = 90,
  MCPM_WCDMA1_PARMS_UPDATE_REQ                        = 91,

  /* GPS Requests */
  MCPM_GPS_INIT_REQ                                   = 92,
  MCPM_GPS_ON_REQ                                     = 93,
  MCPM_GPS_OFF_REQ                                    = 94,
  MCPM_GPS_STOP_REQ                                   = 95,
  MCPM_GPS_ACQ_REQ                                    = 96,
  MCPM_GPS_NON_DPO_REQ                                = 97,
  MCPM_GPS_DPO_ON_REQ                                 = 98,
  MCPM_GPS_PARMS_UPDATE_REQ                           = 99,

/* RF Requests */
  MCPM_RF_START_REQ                                   = 100,
  MCPM_RF_STOP_REQ                                    = 101,

  /* A2 Requests */
  MCPM_A2_START_REQ                                   = 102,
  MCPM_A2_STOP_REQ                                    = 103,

  /* GSM Ciphering Requests */
  MCPM_GSM_CIPHERING_START_REQ                        = 104,
  MCPM_GSM_CIPHERING_STOP_REQ                         = 105,

  /* GSM Ciphering1 Requests */
  MCPM_GSM_CIPHERING1_START_REQ                       = 106,
  MCPM_GSM_CIPHERING1_STOP_REQ                        = 107,

  /* RF CAL Requests */
  MCPM_RF_CAL_START_REQ                               = 108,
  MCPM_RF_CAL_STOP_REQ                                = 109,


  MCPM_LTE_FDD_DATA_PDCCH_REQ                         = 110,
  MCPM_LTE1_FDD_DATA_PDCCH_REQ                        = 111,

  /* RF WLAN Requests */
  MCPM_RF_WLAN_START_REQ                              = 112,
  MCPM_RF_WLAN_STOP_REQ                               = 113,

  /* LTE1 Requests */
  MCPM_LTE1_START_REQ                                 = 114,
  MCPM_LTE1_STOP_REQ                                  = 115,
  MCPM_LTE1_INIT_REQ                                  = 116,
  MCPM_LTE1_ACQ_REQ                                   = 117,
  MCPM_LTE1_WAKE_UP_REQ                               = 118,
  MCPM_LTE1_GO_TO_SLEEP_REQ                           = 119,
  MCPM_LTE1_IDLE_REQ                                  = 120,
  MCPM_LTE1_DATA_START_REQ                            = 121,
  MCPM_LTE1_FDD_DATA_START_REQ                        = 122,
  MCPM_LTE1_TDD_DATA_START_REQ                        = 123,
  MCPM_LTE1_FDD_VOLTE_DATA_START_REQ                  = 124,
  MCPM_LTE1_TDD_VOLTE_DATA_START_REQ                  = 125,
  MCPM_LTE1_GO_TO_LIGHT_SLEEP_REQ                     = 126,
  MCPM_LTE1_GO_TO_LONG_LIGHT_SLEEP_REQ                = 127,
  MCPM_LTE1_GO_TO_LIGHT_SLEEP_NO_MODEM_FREEZE_REQ     = 128,
  MCPM_LTE1_DATA_STOP_REQ                             = 129,
  MCPM_LTE1_PARMS_UPDATE_REQ                          = 130,



  MCPM_TECH_MAX_REQ
} mcpm_request_type;
/*
 * mcpm_query_resrc
 * MCPM resource query enum
 * Currently ML1 queries Q6 and BIMC resources
 */
typedef enum
{
  MCPM_QUERY_CLKQ6  = 0,
  MCPM_QUERY_CLKBIMC,
  MCPM_NUM_QUERY_RESRC
}mcpm_query_resrc;

/*
 * mcpm_query_result
 *
 * For Q6 or BIMC query request,send a return value
 * of this type depending on if query is successful 
 * or not
 */
typedef enum
{
  MCPM_QUERY_SUCCESS = 0,
  MCPM_QUERY_RESRC_NOT_SUPPORTED,  
  MCPM_QUERY_NO_SCHEDULED_VAL,
  MCPM_QUERY_UNSUPPORTED_QUERY_TYPE
}mcpm_query_result;


/* mcpm_query_type 
 * 
 * MCPM resource query client type enum 
 * MCPM_QUERY_BASE_CLIENT       - base column value of a tech
 * MCPM_QUERY_BASE_MCVS_CLIENT  - base + MCVS muxed value of a tech
 * MCPM_QUERY_SCHEDULED_CLIENT  - scheduled value of a tech
 */

typedef enum
{  
  MCPM_QUERY_BASE_CLIENT = 0,  
  MCPM_QUERY_BASE_MCVS_CLIENT,  
  MCPM_QUERY_SCHEDULED_CLIENT,  
  MCPM_QUERY_NUM_CLIENT
}mcpm_query_type;


/*----------------------------------------------------------------------------
  MODEM block register restore callback function prototype
----------------------------------------------------------------------------*/

typedef void (*mcpm_block_restore_callback_type) (void);


/*----------------------------------------------------------------------------
  L1 techs register sleep timeline callback function prototype
  Default 1X and DO will extend timeline
  TRUE in param implies Optimized timeline
  FALSE in param implies Extended timeline
----------------------------------------------------------------------------*/

typedef void (*mcpm_npa_slp_tmln_callback_type) (boolean);


/*----------------------------------------------------------------------------
  MODEM clock speed change callback function prototype
----------------------------------------------------------------------------*/

typedef void (*mcpm_clk_speed_change_callback_type) (void);

/*----------------------------------------------------------------------------
  MODEM sleep entry callback function prototype
----------------------------------------------------------------------------*/

typedef void (*mcpm_sleep_entry_callback_type) (void);

/*
 * mcpm_nbr_meas_type
 *
 * The neighbor measurement type
 */

typedef enum
{
  /* 1X Neighbors */
  MCPM_1X2L_MEAS_START,
  MCPM_1X2L_MEAS_STOP,

  /* DO Neighbors */
  MCPM_DO2L_MEAS_START,
  MCPM_DO2L_MEAS_STOP,
  MCPM_DO2G_MEAS_START,
  MCPM_DO2G_MEAS_STOP,

  /* GERAN and GERAN1 Neighbors */
  MCPM_G2L_MEAS_START,
  MCPM_G2L_MEAS_STOP,
  MCPM_G2W_MEAS_START,
  MCPM_G2W_MEAS_STOP,
  MCPM_G2TDS_MEAS_START,
  MCPM_G2TDS_MEAS_STOP,

  /* LTE Neighbors */
  MCPM_L2G_MEAS_START,
  MCPM_L2G_MEAS_STOP,
  MCPM_L2W_MEAS_START,
  MCPM_L2W_MEAS_STOP,
  MCPM_L21XDO_MEAS_START,
  MCPM_L21XDO_MEAS_STOP,
  MCPM_L2TDSCDMA_MEAS_START,
  MCPM_L2TDSCDMA_MEAS_STOP,

  /* TDSCDMA Neighbors */
  MCPM_TDSCDMA2G_MEAS_START,
  MCPM_TDSCDMA2G_MEAS_STOP,
  MCPM_TDSCDMA2L_MEAS_START,
  MCPM_TDSCDMA2L_MEAS_STOP,
  MCPM_TDSCDMA21XDO_MEAS_START,
  MCPM_TDSCDMA21XDO_MEAS_STOP,

  /* WCDMA Neighbors */
  MCPM_W2G_MEAS_START,
  MCPM_W2G_MEAS_STOP,
  MCPM_W2L_MEAS_START,
  MCPM_W2L_MEAS_STOP,
  MCPM_W21XDO_MEAS_START,
  MCPM_W21XDO_MEAS_STOP

} mcpm_nbr_meas_type;


/*
 * mcpm_rf_bandwidth_type
 *
 * The configured RF bandwidth
 */

typedef enum
{
  /* DO */
  MCPM_DO,
  MCPM_MCDO,

  /* LTE */
  MCPM_LTE_1p4MHZ,
  MCPM_LTE_3MHZ,
  MCPM_LTE_5MHZ,
  MCPM_LTE_10MHZ,
  MCPM_LTE_15MHZ,
  MCPM_LTE_20MHZ,
  MCPM_LTE_5MHZ_5MHZ,
  MCPM_LTE_10MHZ_10MHZ,
  MCPM_LTE_20MHZ_20MHZ,
  MCPM_LTED_TDD_20MHZ,
  MCPM_LTED_FDD_20MHZ,
  /* WCDMA */
  MCPM_WCDMA,
  MCPM_DC_WCDMA,
  MCPM_3C_WCDMA,
  MCPM_4C_WCDMA,
  MCPM_BW_NA,
  MCPM_NUM_BW

} mcpm_rf_bandwidth_type;


/*
 * MCPM_dl_datarate_type
 *
 * Downlink data rate.
 */

typedef enum
{

  /* 1X */

  /* DO */
  MCPM_DO_DL_2P4_MBPS,
  MCPM_DO_DL_3P1_MBPS,
  MCPM_DO_DL_9P3_MBPS,
  MCPM_DO_DL_14P7_MBPS,

  /* GERAN and GERAN1*/
  MCPM_GSM_DL_EV,
  MCPM_GSM_DL_NONEV,

  /* GPS */

  /* LTE */
  MCPM_LTE_DL_RATE_L10,
  MCPM_LTE_DL_RATE_L36,
  MCPM_LTE_DL_RATE_L50,
  MCPM_LTE_DL_RATE_L73,
  MCPM_LTE_DL_RATE_L100,
  MCPM_LTE_DL_RATE_L110,
  MCPM_LTE_DL_RATE_L146,
  MCPM_LTE_DL_RATE_L150,
  MCPM_LTE_DL_RATE_L300,
  MCPM_LTE_DL_RATE_L400,

  /* TDSCDMA */

  /* WCDMA */
  MCPM_W_DL_R99DATA,
  MCPM_W_DL_3p6_MBPS,
  MCPM_W_DL_7p2_MBPS,
  MCPM_W_DL_14p4_MBPS,
  MCPM_W_DL_21_MBPS,
  MCPM_W_DL_28_MBPS,
  MCPM_W_DL_42_MBPS,
  MCPM_W_DL_63_MBPS,
  MCPM_W_DL_84_MBPS,
  MCPM_DL_RATE_NA,
  MCPM_NUM_DL_RATES

} MCPM_dl_datarate_type;


/*
 * mcpm_ul_datarate_type
 *
 * uplink datarate
 */

typedef enum
{
  /* 1X */

  /* DO */
  MCPM_DO_UL_153KBPS,
  MCPM_DO_UL_1P8MBPS,
  MCPM_DO_UL_5P4MBPS,

  /* GERAN and GERAN1*/
  MCPM_GSM_UL_EV,

  /* GPS */

  /* LTE */
  MCPM_LTE_UL_RATE_L5,
  MCPM_LTE_UL_RATE_L11,
  MCPM_LTE_UL_RATE_L13,
  MCPM_LTE_UL_RATE_L23,
  MCPM_LTE_UL_RATE_L25,
  MCPM_LTE_UL_RATE_L40,
  MCPM_LTE_UL_RATE_L50,
  MCPM_LTE_UL_RATE_L100,
  MCPM_LTE_UL_RATE_L150,

  /* TDSCDMA */

  /* WCDMA */
  MCPM_W_UL_R99DATA,
  MCPM_W_UL_1P46_MBPS,
  MCPM_W_UL_2MBPS,
  MCPM_W_UL_2P93_MBPS,
  MCPM_W_UL_5P76_MBPS,
  MCPM_W_UL_11P5_MBPS,
  MCPM_W_UL_22_MBPS,
  MCPM_UL_RATE_NA,
  MCPM_NUM_UL_RATES

} mcpm_ul_datarate_type;


/*
 * mcvs_request_type
 *
 * mcvs request type
 */

typedef enum
{
  MCVS_REQUEST_NONE = 0,
  MCVS_PRE_SCALE_REQUEST,  /* to prescale Cx/Mx based on clock requests to come in future */
  MCVS_FULL_REQUEST,       /* to update clocks - clk bus/VPE/Q6 - as requested possibly even causing RPM interaction */
  MCVS_FAST_CAP_REQUEST,   /* to update clocks as far as possible but return quickly - No RPM interaction*/
  MCVS_RELEASE_REQUEST,     /* To release any request made in past */
} mcvs_request_type;


/* mcpm_scenario_ctrl_type_e */
typedef enum
{
  MCPM_SCENARIO_CLIENT_VPE,
  MCPM_SCENARIO_CLIENT_Q6,
  MCPM_SCENARIO_CLIENT_BOOST,
  MCPM_SCENARIO_CLIENT_DYN_VPE,
  MCPM_SCENARIO_CLIENT_DYN_Q6,
  MCPM_SCENARIO_CLIENT_MAX
} mcpm_scenario_ctrl_type_e;

/* mcpm_scenario_dynamic_lvl_type */
typedef enum
{
  MCPM_SCENARIO_DYNAMIC_LVL_0,
  MCPM_SCENARIO_DYNAMIC_LVL_1,
  MCPM_SCENARIO_DYNAMIC_LVL_2,
  MCPM_SCENARIO_DYNAMIC_LVL_3,
  MCPM_SCENARIO_DYNAMIC_LVL_MAX
} mcpm_scenario_dynamic_lvl_type;

/* mcpm_voltage_level_e*/
typedef enum
{
  MCPM_OFF,
//  MCPM_MIN_SVS,
//  MCPM_LOW_SVS,
  MCPM_SVS2,
  MCPM_SVS,
  MCPM_SVS_PLUS,
//  MCPM_SVS_L1,
  MCPM_NOM,
  MCPM_NOM_PLUS,
  MCPM_TURBO,
  MCPM_SUPER_TURBO,
  /*MAX value*/
  MCPM_MAX_LEVEL
} mcpm_voltage_level_e;

/* client_scenario_info_type*/
typedef struct
{
  mcpm_scenario_ctrl_type_e client_type_e; // (static/dynamic - q6_static, vpe_static, dynamic,dynamic_aggregation)
  uint32 client_name; //lte_ml1_mcvs_q6_vote_reason_e or lte_ml1_mcvs_vpe_vote_reason_e
  uint32 scenario_name; // enum within a client
  mcpm_voltage_level_e dynamic_floor;
  mcpm_voltage_level_e dynamic_ceil;
  mcpm_scenario_dynamic_lvl_type dynamic_level;
} client_scenario_info_type;

/*
 * mcpm_mcvsrequest_parms_type
 *
 *  Structure containing the MCPM MCVS Request and Request Attributes.
 *  mcvs_req_type                 - Indicates if request is pre-scale (0) - slow (1) or fast(2), other fileds reserved for future
 *  mcvs_update_info              - Mask that indicates which of the attrbutes are updated.
 *
 *  modem_vpe_KHz                 - Provides the MP update Info
 *  modem_clk_bus_KHz             - Indicates the Clk bus update
 *  q6_clock_KHz                  - Indicates Q6 clock update
 *  mpps_q6                       - Indicates MPPS for Q6 clock update
 *  scenario_info                 - pointer to scenario info of type uint32
 *                                  Would use scenario_info_type as W can send timestamp info as well
 */

typedef struct
{
  mcvs_request_type      mcvs_req_type;
  uint32                 mcvs_update_info;
  uint32                 modem_vpe_KHz;//Not in use on HA
  uint32                 modem_clk_bus_KHz;//Not in use on HA
  uint32                 q6_clock_KHz;
  uint32                 mpps_q6;//Not in use on HA
  uint32 *               scenario_info;
  client_scenario_info_type  *client_info;
  uint32                 num_client_info_elements;
} mcpm_mcvsrequest_parms_type;


typedef enum{
  MCPM_PORT_NA,
  MCPM_TWO_PORTS,
  MCPM_FOUR_PORTS,
  MCPM_NUM_PORT_MAX
}mcpm_number_of_ports_e;

typedef enum{
  MCPM_TM_NA,
  MCPM_TM_1_TO_6,
  MCPM_TM_7_to_p,
  MCPM_NUM_TM_MODE_MAX
}mcpm_transmission_mode_e;

typedef enum{
  MCPM_CFG_NA,
  MCPM_FDD_CFG,
  MCPM_TDD_CFG1_OR_CFG2,
  MCPM_TDD_OTHR_CFGS,
  MCPM_NUM_CFG_MAX
}mcpm_duplex_config_e;

/*
 * mcpm_request_parms_type
 *
 * Structure containing the MCPM Request and Request Attributes.
 *
 *  update_info        - Indicates which of the attrbutes are updated.
 *  receiver_config    - Provides the receiver config information.
 *  neighbor_meas      - Indiactes the type of neighbor measurement
 *  dl_datarate        - Provides the downlink data rate.
 *  ul_datarate        - Provides the uplink data rate.
 *  rf_bandwidth       - Provides the RF bandwidth.
 *  wkup_time_for_rf   - Time when RF resource needed (in usec)
 *  wkup_time          - Time when bus or any other resource request needed.(in usec)
 *  modem_perf_mode    - Modem performance mode
 *  mcvs_request - L1s can send an MCVS request along with other parameter updates
 *                 MCVS request could be to pre-scale voltage rails based on clock frequency, or make immediate clk bus/Q6/VPE updates
 *  number_of_ports    - number_of_ports;
 *  transmission_mode  - transmission_mode;
 *  duplex_config      - config;
 */
typedef struct
{

  uint32                        update_info;
  uint32                        receiver_config;
  mcpm_nbr_meas_type            neighbor_meas;
  MCPM_dl_datarate_type         dl_datarate;
  mcpm_ul_datarate_type         ul_datarate;
  mcpm_rf_bandwidth_type        rf_bandwidth;
  uint64                        wkup_time_for_rf;
  uint64                        wkup_time;
  uint32                        modem_perf_mode;
  mcpm_mcvsrequest_parms_type   mcvs_request;
  mcpm_number_of_ports_e        number_of_ports;
  mcpm_transmission_mode_e      transmission_mode;
  mcpm_duplex_config_e          duplex_config;
} mcpm_request_parms_type;

/*
 * mcpm_modem_clk_type
 *
 * modem clock type.
 */

typedef enum
{

  /* MCPM Scalar Q6 Clk */
  MCPM_CLK_Q6,

  /* Clk Modem AXI */
  MCPM_CLK_MODEM_AXI,

  /* Clk bus */
  MCPM_CLK_BUS,

  /* MCPM TDEC clk */
  MCPM_CLK_MODEM_TDEC,

  /* MCPM Vector Q6 Clk */
  MCPM_CLK_SILVER,

  /* Clk bus internal to MODEM */
  MCPM_CLK_MODEM_MSSBUS,

  /* Clk Modem CCS */
  MCPM_CLK_MODEM_CCS,

  /* Clk Modem MTC FAST */
  MCPM_CLK_MODEM_MTC,

  /* MCPM total number of clocks */
  MCPM_NUM_CLK

} mcpm_modem_clk_type;

/*
 * mcpm_resource_type
 *
 * modem resrc type.
 */

typedef enum
{
  /* MCPM Scalar Q6 Clk */
  MCPM_RESRC_Q6,

  /* MCPM Vector Q6 Clk */
  MCPM_RESRC_SILVER,

  /* Clk bus internal to MODEM - modemAHb */
  MCPM_RESRC_MODEM_MSSBUS,

  /* MCPM total number of resrcs */
  MCPM_RESRC_MAX

}mcpm_resource_type;

/*
 * mcpm_clk_callback_trigger_type
 *
 * clock callback trigger type.
 */

typedef enum
{

  /* Trigger call back after clock change in MCPM */
  MCPM_CB_Trigger_AFTER_CLK_Change,

  /* Trigger call back before clock change in MCPM */
  MCPM_CB_Trigger_BEFORE_CLK_Change,

} mcpm_clk_callback_trigger_type;


/*
 * mcpm_tech_type
 *
 * MCPM supported technologies.
 */

typedef enum
{
  /* MCPM 1X technology definition */
  MCPM_1X_TECH,

  /* MCPM DO technology definition */
  MCPM_DO_TECH,

  /* MCPM WCDMA technology definition */
  MCPM_WCDMA_TECH,

  /* MCPM WCDMA technology definition */
  MCPM_WCDMA1_TECH,

  /* MCPM LTE technology definition */
  MCPM_LTE_TECH,

  /* MCPM LTE1 technology definition */
  MCPM_LTE1_TECH,

  /* MCPM TDSCDMA technology definition */
  MCPM_TDSCDMA_TECH,

  /* MCPM GERAN technology definition */
  MCPM_GERAN_TECH,

  /* MCPM GERAN1 technology definition */
  MCPM_GERAN1_TECH,

  /* Number of RAT technologies */
  MCPM_NUM_RAT_TECH,

  /* MCPM GPS technology definition */
  MCPM_GPS_TECH = MCPM_NUM_RAT_TECH,

  /* MCPM RF technology definition */
  MCPM_RF_TECH,

  /* MCPM A2 technology definition */
  MCPM_A2_TECH,

  /* MCPM GSM CIPHERING definition */
  MCPM_GSM_CIPHERING_TECH,

  /* MCPM GSM CIPHERING1 definition */
  MCPM_GSM_CIPHERING1_TECH,

  /* MCPM RF CAL technology definition */
  MCPM_RF_CAL_TECH,

  /* Number of total techs used for boundary checks */
  MCPM_NUM_TECH
} mcpm_tech_type;


/*
 * MCPM_RCM_TRIGGER
 *
 * Supported RCM Triggers
 */

typedef enum
{
  /* Primary RCM Debug Toggle */
  RCM_TRIGGER_1,
  /* Secondary RCM Debug Toggle */
  RCM_TRIGGER_2,
  /* Tertiary RCM Debug Toggle */
  RCM_TRIGGER_3 = RCM_TRIGGER_1,
  /* RCM Debug toggle for L1 breakdowns */
  RCM_TRIGGER_4 = RCM_TRIGGER_2,
  /* Max number of RCM triggers */
  RCM_TRIGGER_MAX
} MCPM_RCM_TRIGGER;


/*----------------------------------------------------------------------------
  A2 resources need be setup as fast as possible. Pulling out of A2 from the
  normal path of muxing.
----------------------------------------------------------------------------*/
#define MCPMDRV_NUM_MUX_TECH  (MCPM_A2_TECH)

/*----------------------------------------------------------------------------
  Typedef of modem blocks.
----------------------------------------------------------------------------*/

typedef enum
{

  /* MCPM EDGE block */
  MCPM_EDGE_BLOCK,

  /* MCPM EDGE1 block */
  MCPM_EDGE_G1_BLOCK,

  /* MCPM EDGE2 block */
  MCPM_EDGE_G2_BLOCK,

  /* MCPM TX block */
  MCPM_TX_BLOCK,

  /* MCPM Turbo decoder block */
  MCPM_TDEC_BLOCK,

  /* MCPM DemBack block */
  MCPM_DEMBACK_BLOCK,

  /* MCPM VPE block */
  MCPM_VPE_BLOCK,

  /* MCPM MEMSS block */
  MCPM_MEMSS_BLOCK,

  /* MCPM DEMSS block */
  MCPM_DEMSS_BLOCK,

  /* MCPM RXFE block */
  MCPM_RXFE_BLOCK,

  /* MCPM RXFE block */
  MCPM_STMR_BLOCK,

  /* MCPM total number of blocks */
  MCPM_NUM_BLOCK

} mcpm_modem_block_type;

/* Return values to check for by RF/other SW modules using MCPM_NPA */
typedef enum
{
  MCPMNPA_STAT_NULL,
  MCPMNPA_STAT_OPT_SUCCESS = 1,
  MCPMNPA_STAT_OPT_FAILURE = 2,
  MCPMNPA_STAT_OPT_IMM_REQ_SENT = 3,
  MCPMNPA_STAT_OPT_ENABLE = 4
} MCPMNPA_StatusType;


/*
 * MCPM_ClkSpeedType
 *
 * MCPM clock speed type which controls the Q6 Clk speed for HA.
 * TODO:Ensure that aMCPMModemClkSilverTable is populated correctly.
 *
 */

typedef enum
{
  MCPM_CLK_NULL,
  MCPM_CLK_19_2M,
  MCPM_CLK_SILVER_403M,
  MCPM_CLK_SILVER_595M,
  MCPM_CLK_SILVER_729M,
  MCPM_CLK_SILVER_864M,
  MCPM_CLK_SILVER_921M,
  MCPM_CLK_SILVER_998M,
  MCPM_NUM_CLKSPEED
} MCPM_ClkSpeedType;
/*
 * format in which clock info would be populated and sent back to calling tech
 */
typedef struct
{
  mcpm_tech_type tech;
  uint32 mcpm_column_id;
  uint32 tdec_khz;
  uint32 ccs_khz;
  uint32 axi_khz;
  uint32 mtc_fast_khz;
}mcpm_clks_type;
/*----------------------------------------------------------------------------
  Typedef of core cpu vdd vote.
----------------------------------------------------------------------------*/

typedef enum
{
  MCPM_MODEM_PLUS_Q6_PC_VOTE,      /*vote = 0*/
  MCPM_MODEM_PLUS_Q6_NOT_PC_VOTE,   /*vote = 1*/
  MCPM_Q6_ONLY_PC_VOTE            /*MODEM FREEZE - vote =2*/
} MCPM_core_cpuvdd_vote_type;

#define MCPM_BIMC_BOOST_MASK              0x001
#define MCPM_CPU_BOOST_MASK               0x002
#define MCPM_CPU_RETAIN_MASK              0x004
//#define MCPM_VECTOR_CPU_BOOST_MASK        0x004

typedef void (*mcpm_deboost_callback)(void);
typedef void (*mcpm_boost_callback)(void);
typedef void (*mcpm_tech_scen_denial_cb_type)(uint32* scenario);


/*==========================================================================
               FUNCTION DECLARATIONS FOR MODULE
==========================================================================*/

/* =========================================================================
 **  Function : MCPM_Service_Boost_Req
 ** =========================================================================*/
/**
  This function is the main alternate API (wrapper function) called by L1 techs to request for boost/deboost.

  @param client         [in] Current technology mcpm_tech.
  @param boostRequest         [in] uses 2 LSB bits as flags.
                                  MCPM_BIMC_BOOST_MASK(0x1) and MCPM_CPU_BOOST_MASK (0x2)
  @param callbackFunctionPtr  [in] function pointer to call, when deboost is complete.

  @return
  REturn 0 if request honoured else 1

  @dependencies
  TBD.

 */
uint32 MCPM_Service_Boost_Req
(
  mcpm_tech_type tech,
  uint32                 boost_request,
  mcpm_deboost_callback  fun_ptr_deboost_cb
);


/* =========================================================================
 **  Function : MCPM_Service_Boost_Request
 ** =========================================================================*/
/**
  This function is the main API called by L1 techs to request for boost/deboost..


  @param client         [in] Current technology mcpm_tech.
  @param boostRequest         [in] uses 2 LSB bits as flags.
                                  MCPM_BIMC_BOOST_MASK(0x1) and MCPM_CPU_BOOST_MASK (0x2)
  @param callbackFunctionPtr  [in] function pointer to call, when deboost is complete.
  @param CpuBoostCbfcnPtr     [in] function pointer to call, when CPU boost is complete.
  @param l1boostTime          [in] client request for boost time... max 20000 usec allowed.

  @return
  REturn 0 if request honoured else 1

  @dependencies
  TBD.

 */
uint32 MCPM_Service_Boost_Request
(
  mcpm_tech_type tech,
  uint32                 boost_request,
  mcpm_deboost_callback  fun_ptr_deboost_cb,
  mcpm_boost_callback    fun_ptr_boost_cb,
  uint32                 boost_time
);

/*==========================================================================

  FUNCTION      MCPM_CONFIG_MODEM

  DESCRIPTION   This function is the main interface function.
                Modem SW should call this function when the modem mode is
                changed. MCPM expects the call made for the following mode
                changes.
                - Activate and deactivate the Portocal stack
                - Start and stop of voice call
                - Start and stop of data call
                - Change in RF bandwidth
                - Start and Stop of nbr measurement
                - Rxd/qice/EQ start and stop.

  PARAMETERS    req      - Modem SW mode change request.
                parms    - Attributes associated with req.

  DEPENDENCIES  MCPM must be initialized.

  RETURN VALUE  None.

  SIDE EFFECTS  Clocks will be enabled and disabled during the test.

==========================================================================*/

void MCPM_Config_Modem(mcpm_request_type req, const mcpm_request_parms_type *parms);


/*==========================================================================

  FUNCTION      MCPM_MCVSConfig_Modem

  DESCRIPTION   This function is the interface function for MCVS requests.
                Modem SW should call this function when the modem mode mcvs update is required.
                MCPM expects the call made for the following mode
                changes.
                - VPE Clock update
                - CLK BUS Clock update
                - Q6 Clock update

  PARAMETERS    tech      - Modem SW tech type.
                parms    - Attributes associated with req.

  DEPENDENCIES  MCPM must be initialized.

  RETURN VALUE  None.

  SIDE EFFECTS  Clocks will be Updated during this without change in Config. If the request attribute type in parms is
                set to type immediate - then no RPM requests will be sent out to scale the voltages. If voltages have
                not been pre-scaled, then frequencies will be capped.If request type is type slow - this call will
                block if (mx,cx) voltage scaling is required and may take longer to finish. This call needs to be
                accompanied by a call to Release the MCVS Request

==========================================================================*/

void MCPM_MCVSConfig_Modem(mcpm_tech_type tech, const mcpm_mcvsrequest_parms_type *parms);


/*==========================================================================

  FUNCTION      MCPM_MCVSConfig_Release
  DESCRIPTION   This function is the interface function for releasing the MCVS requests .
                Modem SW should call this function when the modem mode mcvs update request has been made.
                MCPM expects the call made for the following mode
                changes.
                - VPE Clock update
                - CLK BUS Clock update
                - Q6 Clock update

  PARAMETERS    tech      - Modem SW tech type.


  DEPENDENCIES  MCPM must be initialized. A prior call to MCPM_MCVSConfig_Modem from same tech type should have been made.


  RETURN VALUE  NONE.

  SIDE EFFECTS : This will bring down the clock configs to pre-mcvs level.

==========================================================================*/

void MCPM_MCVSConfig_Release(mcpm_tech_type tech);


/*============================================================================

FUNCTION MCPM_SET_BLOCK_RESTORE_CALLBACK

DESCRIPTION
  This function sets the callback function to call when modem block is bought
  out of power gating (power collapse)

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

============================================================================*/

void MCPM_Set_Block_Restore_Callback
(
  /* The tech that is registering the block */
  mcpm_tech_type tech,

  /* Modem block  */
  mcpm_modem_block_type block,

  /* register restore callback */
  mcpm_block_restore_callback_type restore_cb
);

/*============================================================================

FUNCTION MCPM_Set_Clock_Change_Callback

DESCRIPTION
  This function sets the callback function to call when modem clock state
  changes based on the trigger type.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

============================================================================*/

void MCPM_Set_Clock_Change_Callback
(
  /* The tech that is registering the call-back */
  mcpm_tech_type tech,

  /* Modem clk  */
  mcpm_modem_clk_type clk_type,

  /* register clock change callback */
  mcpm_clk_speed_change_callback_type clk_speed_change_cb
);

/*============================================================================

FUNCTION MCPMDRV_CFG_Dump

DESCRIPTION
  This function dumps all the real time MCPM clk/registers values to F3 for
  now.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

============================================================================*/

void MCPMDRV_CFG_Dump( uint32 mcpm_dump_val);


/*==========================================================================

  FUNCTION      MCPM_Send_RF_Req

  DESCRIPTION   This function can be called by RF drivers to send the RF requests
                to MCPM.

  PARAMETERS    mode                - Tech/RF mode that is making this request.
                npa_id              - PAM Id that MCPM will use in NPA request
                npa_handle          - NPA handle that MCPM should use to
                pass this request to PMIC
                time_rsrc_needed    - Time when this resource is needed (not used for now)
                sleep_req           - Bool to indicate if this is a sleep or awake request

  DEPENDENCIES  MCPM must be initialized.

  RETURN VALUE  None.

  SIDE EFFECTS  None.

==========================================================================*/
MCPMNPA_StatusType MCPM_Send_RF_Req
(
  /* Tech that is making this request. */
  rfm_mode_enum_type mode,
  /* NPA Id sent by RF driver */
  uint8 npa_id,
  /* NPA handle that is sent by RF driver */
  npa_client_handle npa_handle,
  /* Time when this resource is needed. */
  uint64 time_rsrc_needed,
  /* Sleep or Awake request. */
  boolean sleep_req
);


/*============================================================================

FUNCTION MCPM_NPA_SET_SLEEP_TIMELINE_CALLBACK

DESCRIPTION
  This function sets the callback function to call when sleep timeline has to
  changed from optimized to extended and vice versa

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

============================================================================*/

void MCPM_NPA_Set_Sleep_Timeline_Callback
(
  /* The tech that is registering the block */
  mcpm_tech_type tech,

  /* register sleep timeline callback */
  mcpm_npa_slp_tmln_callback_type slp_tmln_cb
);


/*==========================================================================
FUNCTION MCPMDRV_Get_Modem_Offline_Clock

DESCRIPTION:
    MCPM API to returen offline clock setting to the caller

DEPENDENCIES
  None.

RETURN VALUE
  clock speed type (enum).

SIDE EFFECTS
  None.

============================================================================*/

MCPM_ClkSpeedType MCPMDRV_Get_Modem_Offline_Clock(mcpm_tech_type current_tech);


/*===========================================================================
FUNCTION     MCPM_Get_PwrOpt_Level

DESCRIPTION  This function is the interface function for L1 SW to get the
             level of power optimization from Modem SW.

DEPENDENCIES MCPM must be initialized.

PARAMETERS   eTech  - mcpm_tech_type.

RETURN VALUE The level of power optimization for that tech.

SIDE EFFECTS None.
===========================================================================*/

uint8 MCPM_Get_PwrOpt_Level(mcpm_tech_type eTech);

/*==========================================================================
FUNCTION MCPMDRV_Get_Resource_State

DESCRIPTION:
    MCPM API to return requested nResrcId setting to the caller.
    This is not the final resource state in the NPA node, but it's only the
    state that's voted by a particular tech indicated by eTech.

DEPENDENCIES
  None.

RETURN VALUE
  uint32 - for clocks - KHz

SIDE EFFECTS
  None.

============================================================================*/

uint32 MCPMDRV_Get_Resource_State (mcpm_tech_type tech, mcpm_resource_type eResrc, void * futureUse);

/*==========================================================================

  FUNCTION      mcpm_gpio_profile_pwr_strobe_toggle

  DESCRIPTION   Used to toggle power strobe at profile points. Should not be used
                directly. Instead should be used through macro MCPM_GPIO_TOGGLE.

  PARAMETERS    strobe      - The RCM power strobe to toggle

  DEPENDENCIES  MCPM must be initialized, MCPM GPIO feature must be enabled

  RETURN VALUE  None.

  SIDE EFFECTS  Toggles a specific dedicated debug strobe on RCM HW

==========================================================================*/

extern void MCPM_Gpio_Profile_Pwr_Strobe_Toggle(MCPM_RCM_TRIGGER strobe);

/*
 * Macro for toggling a specific RCM debug strobe
 */

#define MCPM_GPIO_TOGGLE(RCM_STROBE) MCPM_Gpio_Profile_Pwr_Strobe_Toggle(RCM_STROBE);

/*==========================================================================

  FUNCTION      MCPM_Gpio_Profile_RF_Pwr_Strobe_Toggle

  DESCRIPTION   Used to toggle power strobe at profile points. Should not be used
                directly. Instead should be used through macro MCPM_RF_GPIO_TOGGLE.

  PARAMETERS    strobe      - The RCM power strobe to toggle

  DEPENDENCIES  MCPM must be initialized, MCPM GPIO feature must be enabled

  RETURN VALUE  None.

  SIDE EFFECTS  Toggles a specific dedicated debug strobe on RCM HW

==========================================================================*/
extern void MCPM_Gpio_Profile_RF_Pwr_Strobe_Toggle();

#define MCPM_RF_GPIO_TOGGLE MCPM_Gpio_Profile_RF_Pwr_Strobe_Toggle();

/*============================================================================

FUNCTION MCPM_Get_Data_MPLM

DESCRIPTION
  This function copies MCPM data into MPLM structure for modem power logging.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

============================================================================*/

void MCPM_Get_Data_MPLM
(
  /* The tech that is registering the block */
  MPLM_MCPM_Buffer_Type* MPLMBuffer
);

/*==========================================================================

  FUNCTION      MCPM_is_qdss_en

  DESCRIPTION   Used to check if QDSS modem events are enabled for MCPM

  PARAMETERS    None.

  DEPENDENCIES  MCPM must be initialized,

  RETURN VALUE  None.

  SIDE EFFECTS  None.

==========================================================================*/
boolean MCPM_is_qdss_en(void);

/*============================================================================

FUNCTION MCPM_Set_Mcpm_Sleep_Entry_Callback

DESCRIPTION
  This function sets the callback function to call when mcpm enter sleep/power down or Lightsleep States

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

============================================================================*/

void MCPM_Set_Mcpm_Sleep_Entry_Callback
(
  /* register sleep entry callback */
  mcpm_sleep_entry_callback_type sleep_entry_cb,
  void * futureUse
);

/*============================================================================
FUNCTION mcpm_query_ccs_state

DESCRIPTION
  This function returns the current state of RFCCS for the requested TECH

DEPENDENCIES
  None.

RETURN VALUE
  MCPM_RM_Resrc_StateType 

SIDE EFFECTS
  None.

============================================================================*/

boolean mcpm_query_ccs_state
(
  mcpm_tech_type eTech
);

/*============================================================================

FUNCTION MCPM_Query_Client_Resource_Value

DESCRIPTION
  This API queries a client state for the queried resource for given tech.
  Currently, resources from the enum mcpm_query_resrc_id are supported, and MCPM returns the
  FMAX, and based on the voted clocks, ML1 comes up with the RF warmup budget at wakeup.
  No MCVS/Boost clients are considered, as they should not exist at the query time.

PARAMETERS
  eTech                [in]  Current technology mcpm_tech.
  resource             [in]  The resource ML1 queries
                             Only resources from the enum mcpm_query_resrc_id are supported.
  query_type           [in]  This parameter is unused at this moment.
                             ML1 currently passing 0 while calling this API.
  data                 [in]  Pointer to data from ML1 can be used to specify params(UL, DL, RF BW)
                             to determine a new column. This value will be no NULL only for query
                             client type MCPM_QUERY_NEW_COLUMN_CLIENT which is currently unsupported
  data_size            [in]  Size of the data passed in
  resrc_val            [out] Query return value


DEPENDENCIES
  TBD.

RETURN VALUE
  Returns the value indicating if the query was successful or not

SIDE EFFECTS
  None.

============================================================================*/

mcpm_query_result  MCPM_Query_Client_Resource_Value
(
  mcpm_tech_type                eTech,
  mcpm_query_resrc              resrc_id,
  mcpm_query_type               query_type,
  void                          *data,
  uint32                        data_size,
  uint32                        *resrc_val
);



/*==========================================================================

  FUNCTION      MCPM_Toggle_Demback_And_Tdec
  
  DESCRIPTION
    This function toggles DEMBACK and TDEC HW . WA for HDR
  DEPENDENCIES
    None.

  RETURN VALUE
    None

  SIDE EFFECTS
    None.

==========================================================================*/


void MCPM_Toggle_Demback_And_Tdec();

/*============================================================================

  FUNCTION      mcpm_get_modem_clks_for_given_config

  DESCRIPTION   This function would give out clock info which would be applied
                based on current state and the requested input parameters.

                LTE would be using this for supporting the TDEC half iteration
                borrowing feature. FW would need to know TDEC clk at least 2msec
                before the column switch to lower TDEC clk can happen. so that
                they can schedule TDEC tasks appropriately. Hence the ask for
                this new interface.

============================================================================*/
boolean mcpm_get_modem_clks_for_given_config(mcpm_request_type request,
                                        mcpm_request_parms_type * mcpm_param,
                                        mcpm_clks_type * mcpm_clks);

#endif /* __MCPM_API_H__ */


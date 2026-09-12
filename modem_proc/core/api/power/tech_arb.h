#ifndef _TECH_ARB_H_
#define _TECH_ARB_H_

/*===========================================================================

FILE: 
   tech_arb.h

BRIEF DESCRIPTION:
   Tech Arbiter client interface

DESCRIPTION:
   This file contains the state definitions and interface for tech arb clients.
   
                Copyright (c) 2016 Qualcomm Technologies Incorporated.
                All Right Reserved.
                Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/api/power/tech_arb.h#1 $
$Datetime$

when       who     what, where, why
--------   ---     ---------------------------------------------------------------------------------
 3/23/16   AB      Add enum of mux types, table structure for mux logic and resrc mapping and mca vector structure for query
 1/26/16   AB      Initial verson of technology arbiter public header file
===================================================================================================*/
#include "coremca.h"                         /* Includes MCA key definitions */
#include "com_dtypes.h"                      /* Includes basic data type boolean */


/*=========================================================================
      COMMON Definitions
==========================================================================*/

/* supported common keys for tech arb vector requests */
#define TECH_REQ_KEY           0x54454348    /* 'T' 'E' 'C' 'H'  Units: MCPM_Tech_Info or CCPM_MAC_Info.     */                                 
#define TECH_STATE_REQ_KEY     0x53544154    /* 'S' 'T' 'A' 'T'  Units: MCPM_Tech_State_Info or CCPM_MAC_Power_Modes. */
#define DL_RATE_REQ_KEY        0x444C0000    /* 'D' 'L'          Units: See Tech_Arb_DL_Rate_Info. */
#define UL_RATE_REQ_KEY        0x554C0000    /* 'U' 'L'          Units: See Tech_Arb_UL_Rate_Info. */
#define RF_BW_REQ_KEY          0x52464257    /* 'R' 'F' 'B' 'W'  Units: See Tech_Arb_RF_BW_Info. */
#define RF_CA_REQ_KEY          0x52464341    /* 'R' 'F' 'C' 'A'  Units: See Tech_Arb_RF_CA_Info. */
#define SKIP_LUT_REQ_KEY       0x534B4950    /* 'S' 'K' 'I' 'P'  Units: 1 - skip LUT, 0 - peform LU operation. */


/* Data size of mca value field*/
#define MPPS_DATA_SIZE         sizeof(unsigned)
#define CPP_DATA_SIZE          sizeof(unsigned)
#define Q6_CLK_DATA_SIZE       sizeof(unsigned)
#define AB_DATA_SIZE           sizeof(unsigned)
#define IB_DATA_SIZE           sizeof(unsigned)
#define LAT_DATA_SIZE          sizeof(unsigned)
#define TECH_REQ_SIZE          sizeof(unsigned)
#define TECH_STATE_REQ_SIZE    sizeof(unsigned)
#define DL_RATE_REQ_SIZE       sizeof(unsigned)
#define UL_RATE_REQ_SIZE       sizeof(unsigned)
#define RF_BW_REQ_SIZE         sizeof(unsigned)
#define RF_CA_REQ_SIZE         sizeof(unsigned)
#define SKIP_LUT_REQ_SIZE      sizeof(unsigned)


/* Tech info can be equal to TECH_INFO_UNDEF, to skip LUT operation */
#define TECH_INFO_UNDEF     0x7fffffff //UL

/*
 *  Common Tech arbiter DL rate.
 *  Note: For now, keeping it simple - LOW, MED and HIGH.
 *  
 *  TECHARB_DL_NA         - DL rate not applicable or don't care (DC).
 *  TECHARB_DL_LOW        - Low DL rate.
 *  TECHARB_DL_MED        - Medium DL rate.
 *  TECHARB_DL_HIGH       - High DL rate.
 *  
 */
typedef enum
{
  TECHARB_DL_NA,
  TECHARB_DL_LOW,
  TECHARB_DL_MED,
  TECHARB_DL_HIGH
}TechArb_DL_Rate_Info;

/*
 *  Common Tech arbiter UL rate.
 *  Note: For now, keeping it simple - LOW, MED and HIGH.
 *   
 *  TECHARB_UL_NA         - UL rate not applicable or don't care (DC).
 *  TECHARB_UL_LOW        - Low UL rate.
 *  TECHARB_UL_MED        - Medium UL rate.
 *  TECHARB_UL_HIGH       - High UL rate.
 *  
 */
typedef enum
{
  TECHARB_UL_NA,
  TECHARB_UL_LOW,
  TECHARB_UL_MED,
  TECHARB_UL_HIGH
}TechArb_UL_Rate_Info;

/*
 *  Common Tech arbiter RF Bandwidth.
 *  
 *  TECHARB_RF_BW_NA,     -  RF BW not applicable or don't care (DC).
 *  TECHARB_RF_BW_5Mhz    -  RF BW 5Mhz.
 *  TECHARB_RF_BW_10Mhz   -  RF BW 10Mhz.
 *  TECHARB_RF_BW_20Mhz   -  RF BW 20Mhz.
 *  
 */
typedef enum
{
  TECHARB_RF_BW_NA,
  TECHARB_RF_BW_5Mhz,
  TECHARB_RF_BW_10Mhz,
  TECHARB_RF_BW_20Mhz,
  TECHARB_RF_BW_40Mhz,
  TECHARB_RF_BW_80Mhz,
  TECHARB_RF_BW_160Mhz
}TechArb_RF_BW_Info;

/*
 *  Common Tech arbiter RF Carrier aggregation.
 *  
 *  TECHARB_RF_CA_NA,     -  RF CA not applicable or don't care (DC).
 *  TECHARB_RF_CA_1       -  RF single carrier aggregation (CA).
 *  TECHARB_RF_CA_2       -  RF 2 CAs.
 *  TECHARB_RF_CA_3       -  RF 3 CAs.
 *  TECHARB_RF_CA_4       -  RF 4 CAs.
 *  TECHARB_RF_CA_5       -  RF 5 CAs.
 */
typedef enum
{
  TECHARB_RF_CA_NA,
  TECHARB_RF_CA_1,
  TECHARB_RF_CA_2,
  TECHARB_RF_CA_3,
  TECHARB_RF_CA_4,
  TECHARB_RF_CA_5
}TechArb_RF_CA_Info;


/*
 *  Common Tech arbiter query id.
 *  
 *  TECHARB_NPA_QUERY_CPU_CLK      -  tech arb query for CPU CLK.
 *  TECHARB_NPA_QUERY_MPPS         -  tech arb query for MPPS.
 *  TECHARB_NPA_QUERY_CPP          -  tech arb query for CPP.
 *  TECHARB_NPA_QUERY_LATENCY      -  tech arb query for LATENCY.
 *  TECHARB_NPA_QUERY_AB           -  tech arb query for AB.
 *  TECHARB_NPA_QUERY_IB           -  tech arb query for IB.
 *  TECHARB_NPA_QUERY_ALL          -  tech arb query for all params.
 */
typedef enum {
    TECHARB_NPA_QUERY_CPU_CLK = 0,
	TECHARB_NPA_QUERY_MPPS,
	TECHARB_NPA_QUERY_CPP,
	TECHARB_NPA_QUERY_LATENCY,
	TECHARB_NPA_QUERY_AB,
	TECHARB_NPA_QUERY_IB,
	TECHARB_NPA_QUERY_ALL,
	TECHARB_NPA_QUERY_LUT_MATCH,
	TECHARB_NPA_QUERY_NUM_ACTIVE_CLIENTS,
	TECHARB_NPA_QUERY_CLIENTS_VOTE,
	TECHARB_NPA_QUERY_TECH_UC_STATE,
} techarb_npa_query_id;


/*
 *  Common Tech arbiter mux type of a resource.
 *  
 *  TECHARB_MUX_NONE          -  tech arb no mux logic.
 *  TECHARB_MUX_MAX           -  tech arb max of resource states.
 *  TECHARB_MUX_MIN           -  tech arb max of resource states.
 *  TECHARB_MUX_SUM           -  tech arb sum of resource states.
 *  TECHARB_MUX_FUDGE_FACTOR  -  tech arb fudge factor to resource states.
 */
typedef enum
{
	TECHARB_MUX_NONE = 0,
	TECHARB_MUX_MAX,
	TECHARB_MUX_MIN,
	TECHARB_MUX_SUM,
	TECHARB_MUX_FUDGE_FACTOR,
	TECHARB_MUX_CUSTOM,
} techarb_mux_id;

/*
 *  Mux type mapping with resource
 *
 *  id                        -  techarb resource index
 *  techarb_resource          -  tech arb resource key 
 *  mux_type                  -  Mux type of resource
 *  aggregate                 - function pointer for specific mux type
 */
typedef struct
{
	uint32           techarb_resrc_key;
	techarb_mux_id   mux_type;
	uint32 (*aggregate)(uint32 active, uint32 pending);
	
}techarb_mux_mapping;

/*
 *  techarb_mca_vec_query.
 *  
 *  mpps     -   mpps value in tech arb node                              
 *  cpp      -   cpp value in tech arb node 
 *  clk      -   Q6 clk value in tech arb node 
 *  ab       -   ab value in tech arb node 
 *  ib       -   ib value in tech arb node        
 *  ddr_lat  -   ddr latency value in tech arb node                         
 *  
 */

typedef struct
{
  unsigned mpps; 	                                    
  unsigned cpp; 	                                     	                                    
  unsigned clk; 	                                     	                                     
  unsigned ab; 	                                    	                                    
  unsigned ib; 	                                                                    
  unsigned ddr_lat;
} techarb_mca_vec_query;


/*
 *  tech arb use case data type.
 *  
 *  tech_info     -   tech id info from client                             
 *  tech_state    -   client tech state info 
 *  ul_dt_rate    -   UL data rate of the tech
 *  dl_dt_rate    -   DL data rate of the tech 
 *  rf_bw         -   RF bandwidth of the tech        
 *  rf_ca         -   RF carrier aggregation of tech                        
 *  skip_lut      -   Skip LUT info of client
 */
typedef struct
{
   uint32 tech_info;                                         /* Technology info */
   uint32 tech_state;                                        /* current sttae of tech */ 
   uint32 ul_dt_rate;                                        /* uplinq data rate level info */
   uint32 dl_dt_rate;                                        /* downlink data rate level info */
   uint32 rf_bw;                                             /* RF bandwidth */
   uint32 rf_ca;                                             /* RF carrier aggregation */
   uint32 skip_lut;
}techarb_uc_info_query;



/*=========================================================================
      MCPM Tech-Arbiter Definitions
==========================================================================*/
/*
 * Supported MCPM keys for tech arb vector requests
 */

/*
 *  MCPM techarb techs.
 *  
 *  MCPM_TECHARB_1X_TECH,           -   1X
 *  MCPM_TECHARB_DO_TECH,           -   DO
 *  MCPM_TECHARB_WCDMA_TECH,        -   WCDMA
 *  MCPM_TECHARB_WCDMA1_TECH,       -   WCDMA1
 *  MCPM_TECHARB_LTE_TECH,          -   LTE
 *  MCPM_TECHARB_TDSCDMA_TECH,      -   TDS
 *  MCPM_TECHARB_GERAN_TECH,        -   GSM1
 *  MCPM_TECHARB_GERAN1_TECH,       -   GSM2
 *  MCPM_TECHARB_TECH_MAX           -   MAX
 */
typedef enum
{
  MCPM_TECHARB_1X_TECH,
  MCPM_TECHARB_DO_TECH,
  MCPM_TECHARB_WCDMA_TECH,
  MCPM_TECHARB_WCDMA1_TECH,
  MCPM_TECHARB_LTE_TECH,
  MCPM_TECHARB_LTE1_TECH,
  MCPM_TECHARB_TDSCDMA_TECH,
  MCPM_TECHARB_GERAN_TECH,
  MCPM_TECHARB_GERAN1_TECH,
  MCPM_TECHARB_TECH_MAX = MCPM_TECHARB_GERAN1_TECH,
} MCPM_TechArb_Tech_Info;

/*
 *  MCPM techarb states.
 *  
 *  MCPM_TECHARB_STATE_POWER_DOWN   -   Tech is in power down state.
 *  MCPM_TECHARB_STATE_ACQ          -   Tech is in acquisition state. 
 *  MCPM_TECHARB_STATE_IDLE         -   Tech is in idle state.
 *  MCPM_TECHARB_STATE_SLEEP        -   Tech is in sleep state.
 *  MCPM_TECHARB_STATE_VOICE        -   Tech is in voice state.
 *  MCPM_TECHARB_STATE_DATA         -   Tech is in data state.
 *  MCPM_TECHARB_STATE_VOLTE_DATA   -   Tech is in VoLTE data state.
 *  MCPM_TECHARB_STATE_LIGHT_SLEEP  -   Tech is in light sleep state.
 *  MCPM_TECHARB_STATE_MAX          -   Tech max state.
 *  
 */
typedef enum
{
   MCPM_TECHARB_STATE_POWER_DOWN,
   MCPM_TECHARB_STATE_ACQ,
   MCPM_TECHARB_STATE_IDLE,
   MCPM_TECHARB_STATE_SLEEP,
   MCPM_TECHARB_STATE_VOICE,
   MCPM_TECHARB_STATE_DATA,
   MCPM_TECHARB_STATE_VOLTE_DATA,
   MCPM_TECHARB_STATE_LIGHT_SLEEP,
   MCPM_TECHARB_STATE_MAX
}MCPM_TechArb_State_Info;


/*=========================================================================
      CCPM Definitions
==========================================================================*/

/*
 *  CCPM MACs.
 *  
 *  CCPM_TECHARB_MAC0,           -   MAC0
 *  CCPM_TECHARB_MAC1,           -   MAC1
 *  
 */
typedef enum
{
  CCPM_TECHARB_MAC0 = MCPM_TECHARB_TECH_MAX + 1,
  CCPM_TECHARB_MAC1,
  TOTAL_TECH_MAX = CCPM_TECHARB_MAC1
} CCPM_MAC_Info;

/*
 *  CCPM MAC power modes.
 *  
 *  CCPM_TECHARB_WLAN_DISABLED = 0,     -   WLAN is disabled.                                
 *  CCPM_TECHARB_WLAN_IMPS,             -   WLAN in IMPS active.
 *  CCPM_TECHARB_WLAN_PERF,             -   WLAN 20MHz mode is enabled and baseband is active.
 *  CCPM_TECHARB_WLAN_NETWORK_SLEEP,    -   WLAN in network sleep mode PS_PLL w/PM=1.
 *  CCPM_TECHARB_WLAN_UAPSD,            -   WLAN in UAPSD triggering mode.        
 *  CCPM_TECHARB_WLAN_QPOWER,           -   WLAN in QPower mode.                   
 *  CCPM_TECHARB_WLAN_HW_DTIM,          -   WLAN in network sleep agree to HW DTIM.
 *  CCPM_TECHARB_WLAN_SW_DTIM,          -   WLAN in network sleep agree to SW DTIM.          
 *  
 */

typedef enum
{
   CCPM_TECHARB_WLAN_DISABLED = 0,
   CCPM_TECHARB_WLAN_IMPS,
   CCPM_TECHARB_WLAN_PERF,
   CCPM_TECHARB_WLAN_NETWORK_SLEEP,
   CCPM_TECHARB_WLAN_UAPSD,
   CCPM_TECHARB_WLAN_QPOWER,
   CCPM_TECHARB_WLAN_HW_DTIM,	
   CCPM_TECHARB_WLAN_SW_DTIM,	
} CCPM_MAC_Power_Modes;

/* tech arb table sum of all the tech states in a row and corresponding MCA vote */
typedef struct
{
   uint32   tech_hash_sum;                                  /* Sum of all tech states in a given row*/
   uint32   tech_state[TOTAL_TECH_MAX+1];                   /* Array of tech state */
   uint32   cpu_clk;                                        /* Q6 clk */
   uint32   mpps;                                           /* mpps */
   uint32   cpp;                                            /* cpp */
   uint32   ib;                                             /* instantaneous bus BW */
   uint32   ab;                                             /* average bus BW */
   uint32   ddr_latency;                                    /* latency */
}techarb_tbl_entries;


#endif /* _TECH_ARB_H_ */




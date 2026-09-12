#ifndef TDSRRCLLCOC_H
#define TDSRRCLLCOC_H
/*===========================================================================

                    L3 - RRC Lower Layer Controller Ordered Config

DESCRIPTION
  This file contains data structures, variables, enums and function prototypes
  needed to be used within Lower Layer Controller and Ordered Config.


Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcllcoc.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------   
12/17/12   zy      add the check of m_length and k in DMO
09/29/12   zy      add code of IE DMO and IE Idele Interval Info 
10/25/11   ysh      Modify req_mak in cphy_setup_req.
10/17/11   zwj   Modify default value of FPACH and upPCH. 
10/13/11   mkh     Now using correct macro. Earlier it was coming from W code.
08/26/11   yzh       Add add_queue_bitmask in order to check multiple delete mac_queue info in OTA message.
                            Modify bug about remove old mapping if mac_queue is available in other mac_dflow,
                            And check old mapping when setup not recofigure mac_dflow.
08/20/11   mkh     Compile issue on target. API is static in src file.
07/27/11   ttl     Correct naming of ghdi_mvs functions for TDSCDMA.
06/14/11   yzh     Made changes from FDD to TDD in R8
06/10/11   ttl     Use mvssup_umts_blk_size_cb_set_type for TDSCDMA.
06/09/11   mkh     Added support to pass pval to MAC on reading SIB7 in FACH   
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
03/09/11   su      Made changes to mainline code under FEATURE_WCDMA_HSUPA_2MS_TTI.
03/02/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.92.50
02/18/11   ad      Added changes for one sided UL RLC reestablishment
02/16/11   as      Fixed compilation error
02/15/11   kp      Added function to restore eFACH mapping. 
02/15/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.91.50
02/03/11   as      Made changes to do rnti validation for dch to fach/pch
                   transition when efach is enabled 
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/06/11   as      Made changes to delete commonn queue info while moving
                   to non EFACH cell or DCH state
01/12/11   rl      Migrated to VU_MODEM_WCDMA_RRC.01.88.50
01/04/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.87.50
01/12/11   bj       (3GPP: 10.3.6.55)Added code to store IE "e-RUCCH-Sync-UL-Codes-Bitmap" of 
                          "PRACH-RACH-Info-LCR-v770ext".
01/11/11   bj       (3GPP: 8.5.28)Modify actions related to.E_DCH_TRANSMISSION variable from FDD to TDD.
01/17/11   zwj      3GPP: 8.6.6.37 &10.3.6.97. Add E-DCH info for HSUPA.
11/12/10   yzh      (3GPP: 10.3.6.23a)Modify tdsl1_hs_info_struct_type to tdsl1_hs_info_struct_type
                   when Downlink HS-PDSCH Information(FDD->TDD).
11/18/10   ysh     Transport Format Set. 3GPP8.6.5.1(3.2.32)
                                 rach_TransportFormatSet -> rach_TransportFormatSet_LCR
11/18/10   ysh     Frequency info save. 3GPP-8.6.6.1(3.4.2).
11/11/10   yzh     (3GPP: 8.6.6.27&10.3.6.24)Modify default DPCH offset Value(FDD MAX: 599, TDD MAX: 7)
                   and Del tdsrrcllcpcie_initialise_sync_a_post_veri_info(), and Del Downlink rate
                   matching restriction information(FDD), and Del Downlink F-DPCH info common for all 
                   radio links(FDD) when IE Downlink information common for all radio links(FDD->TDD).
11/04/10   yzh     (3GPP 10.3.6.21) modify Downlink DPCH info for each RL from FDD to TDD.
11/01/10   yzh     (3GPP 8.6.6.6) modify Uplink DPCH info from FDD to TDD
11/05/10   yzh      (3GPP: 8.6.6.15)Delete DPCH Compressed mode info(FDD only, under FEATURE_CM_SUPPORTED)(3.2.20).
10/30/10   ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/29/10   bj       Made changes to Open loop power control.Delete FDD IE "Primary 
                          CPICH Tx power","Constant value" and "UL interference". Add 
                          TDD IE "Primary CCPCH Tx Power".3GPP:8.5.7. 
10/25/10   zwj     Added code to handle REL4 OTA Message. 
10/25/10   zwj     Added code for SCCPCHinfolist-LCR(3GPP 10.3.6.71)
10/24/10   yzh     According to 3GPP25.331v716 8.6.5.12a, delete IE  "Additional RACH TFCS for CCCH" (FDD only)(3.2.11).
10/22/10   ysh     When Physical channel reconfiguration, compressed mode runtime error, reconfig failure,
                                 this function is FDD only, should delete. 3Gpp-8.2.11.2(3.2.7)

===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsl1rrcif.h"
#include "tdsmacrrcif.h"
#include "tdsrlcrrcif.h"
#include "tdsrrcllci.h"
#include "tdsuecomdef.h"
#include "tdsrrciho.h"
#include "tdsrrccu.h"
#include "tdsrrcrbe.h"

#include "tdsrrccspi.h"
#ifdef FEATURE_UMTS_PDCP

#ifdef FEATURE_CMI
#include "pdcp.h"
#include "comptask_api.h"
#else/*FEATURE_CMI*/
#include "pdcprrcif.h"
#include "comptask.h"
#endif /*FEATURE_CMI*/

#endif /* FEATURE_UMTS_PDCP */

#include "modem_mem.h"
#include "tdsrrc_dependancy.h"

/*===================================================================
                        CONSTANTS
=====================================================================*/

#ifdef FEATURE_TDSCDMA_HSUPA

/* Not used currently */
#define TDSL1_EUL_RECFG_GRANT_CONFIG_DL 0x10

#define TDSRRC_INVALID_TPC_INDEX 0xFF

#endif

/* This defines the Activation Time min and max and now values. */
/* Now is an implementation specific value. */
#define TDSRRCLLC_ACTIVATION_TIME_NOW       0xFFFF
#define TDSRRCLLC_MIN_ACTIVATION_TIME       0x0000
#define TDSRRCLLC_MAX_ACTIVATION_TIME       0x00FF

/* This defines an that no valid C_RNTI has been received. */
/* Note that the UE must at least have been assigned a U_RNTI
   in the RRC Connection Establishment procedure. */
#define TDSRRCLLC_NO_VALID_C_RNTI           0xFFFFFFFF

/* Serving RNC ID bit length */
#define TDSRRCLLC_SRNC_ID_BIT_LENGTH        12

/* Serving RNTI bit length */
#define TDSRRCLLC_SRNTI_BIT_LENGTH          20

/* This defines the UTRAN DRX Cylce Length min and max values. */
#define TDSRRCLLC_UTRAN_MIN_DRX_CYCLE_LENGTH      3
#define TDSRRCLLC_UTRAN_MAX_DRX_CYCLE_LENGTH      9

/* This defines the CN domain specific DRX cycle length min and max values */
#define TDSRRCLLC_CN_DOMAIN_MIN_DRX_CYCLE_LENGTH   6
#define TDSRRCLLC_CN_DOMAIN_MAX_DRX_CYCLE_LENGTH   9

/* This defines the Max Allowed UL TX Power min and max values. */
#define TDSRRCLLC_MIN_ALLOW_UL_TX_PWR       (-50)
#define TDSRRCLLC_MAX_ALLOW_UL_TX_PWR       (33)

/* This defines the PICH power offset min and max values. */
#define TDSRRCLLC_MIN_PICH_PWR_OFFSET       (-10)
#define TDSRRCLLC_MAX_PICH_PWR_OFFSET       (5)

/* This defines the Primary CCPCH Tx Power range min and max values. */
#define TDSRRCLLC_MIN_PRI_CCPCH_TX_PWR       (6)
#define TDSRRCLLC_MAX_PRI_CCPCH_TX_PWR       (43)

/* This defines the DRX Cycle range min and max values. */
#define TDSRRCLLC_MIN_DRX_CYCLE             3
#define TDSRRCLLC_MAX_DRX_CYCLE             12

/* This defines the Puncturing Limit range min and max values
   multiplied by 100. */
#define TDSRRCLLC_MIN_PUNCTURING_LIMIT      40
#define TDSRRCLLC_MAX_PUNCTURING_LIMIT      100

/* This defines the Max available signiture bit string size */
#define TDSRRCLLC_MAX_AVAILABLE_SIGNATURE   16

/* This defines the Max available sub-channel # bit string size */
#define TDSRRCLLC_MAX_AVAILABLE_SUBCHAN     12

/* This defines the ASC range min and max values */
#define TDSRRCLLC_MIN_AC_TO_ASC_MAPPING     0
#define TDSRRCLLC_MAX_AC_TO_ASC_MAPPING     7


/* This defines the minimum and maximum values for Transport Channel IDs */
#define TDSRRCLLC_TRCH_ID_MIN_VALUE    1
#define TDSRRCLLC_TRCH_ID_MAX_VALUE    32

/* This defines the Maximum number of Preamble cycles range min
   and max values */
#define TDSRRCLLC_MIN_MMAX                  1
#define TDSRRCLLC_MAX_MMAX                  32

/* This defines the Lower Bound for random backoff range min
   and max values */

/* This defines the ul scrambling code range min and max values */
#define TDSRRCLLC_MIN_UL_SCR_CODE           0
#define TDSRRCLLC_MAX_UL_SCR_CODE           16777215

/* Minimun number of DPDCH allowed */
#define TDSRRCLLC_MIN_NUM_UL_DPDCH_ALLOWED  2
#define TDSRRCLLC_DEFAULT_NUM_DPDCH_ALLOWED 1

#define TDSRRCLLC_EUL_MIN_NUM_UL_DPDCH_ALLOWED 0

#define TDSRRCLLC_UL_DPCCH_PWR_OFFSET_MIN   (-82)
#define TDSRRCLLC_UL_DPCCH_PWR_OFFSET_MAX   (-3)

/* This defines the minimum and maximum rate matching attributes */
#define TDSRRCLLC_MIN_HI_RM                 1
#define TDSRRCLLC_MAX_HI_RM                 256

#define TDSRRC_INTRA_HHO     0
#define TDSRRC_INTER_HHO     1

/*
* This defines the BLER range min and max values Value is (x10)
* Example: MIN value is -6.3; therefore, MIN value is -6.3*10 = -63
* This is per a note in ASN.1 source
*/
#define TDSRRCLLC_MIN_BLER                  (-63)
#define TDSRRCLLC_MAX_BLER                  (0)
#define TDSRRCLLC_BLER_NA                   (-127)

/* This define indicates an invalid Number of Transport Blocks */
#define  TDSRRCLLC_INVALID_NUM_TB           0xFF

/* This define indicates when there is an invalid UL/DL TrCH Index */
#define  TDSRRCLLC_INVALID_TRCH_IDX         0xFF

/* This define indicates an invalid SCCPCH index condition */
#define  TDSRRCLLC_INVALID_SCCPCH_IDX       0x00

/* This defines the maximum preamble scrambling code */
#define TDSRRCLLC_MAX_PREAMBLE_SCRAMBLING_CODE 15

/* Defines an invalid window size */
#define TDSRRCLLC_INVALID_WINDOW_SIZE       0xFFFF

/* Defines an invalid Discard value for all RLC discard types */
#define TDSRRCLLC_INVALID_RLC_DISCARD       0xFFFF

/* This defines the minimum and maximum rate matching attributes */
#define TDSRRCLLC_MIN_CPICH_SCR_CODE        0
#define TDSRRCLLC_MAX_CPICH_SCR_CODE        511

/* Defines an invalid Primary Scrambling code for Downlink. */
#define TDSRRCLLC_INVALID_CPICH_CPID    0xFF

/* Defines an invalid Channelization code for Downlink. */
#define TDSRRCLLC_INVALID_CHAN_CODE         0xFFFF

/* Defines an invalid Channelization code for Downlink. */
#define TDSRRCLLC_INVALID_SEC_CPID      0xFF


#define TDSRRCLLC_MAX_SCCH_CHAN_CODE   127


#define TDSRRC_CONV_PICH_REP_LEN(pbp) \
    ((pbp == TDSL1_PICH_REPEAT_4_2) ? 4 :\
      ((pbp == TDSL1_PICH_REPEAT_8_2) ? 8 :\
        ((pbp == TDSL1_PICH_REPEAT_8_4) ? 8: \
          ((pbp == TDSL1_PICH_REPEAT_16_2) ? 16: \
            ((pbp == TDSL1_PICH_REPEAT_16_4) ? 16 : \
              ((pbp == TDSL1_PICH_REPEAT_32_2) ? 32 : \
                ((pbp == TDSL1_PICH_REPEAT_32_4) ? 32 : \
                  ((pbp == TDSL1_PICH_REPEAT_64_2) ? 64 : \
                    (pbp == TDSL1_PICH_REPEAT_64_4) ? 64 : 0 \
                  )\
                )\
              )\
            )\
          )\
        )\
      )\
    )


#ifdef FEATURE_TDS_DC_HSDPA
typedef enum
{
  TDSRRC_DC_HSDPA_INACTIVE,
  TDSRRC_DC_HSDPA_ACTIVE
}tdsrrc_dc_hsdpa_status_enum_type;

extern tdsrrc_dc_hsdpa_status_enum_type tdsrrc_dc_hsdpa_status;

#define TDSMAX_HS_SCCH_CODES_WITH_DUAL_CELL 6
#define TDSRRCLLC_INVALID_MEAS_PWR_OFFSET 0xFF
#define TDSRRCLLC_INVALID_DL_FREQ 0xFFFF

#define TDSRRC_MAX_HARQ_PROC_FOR_DC_HSDPA 8
#define TDSRRC_MIN_HARQ_PROC_FOR_DC_HSDPA 6

#endif /*FEATURE_TDSCDMA_DC_HSDPA*/


#ifdef FEATURE_TDSCDMA_HSUPA

#define TDSRRCLLC_MAX_E_MAC_D_FLOW_PWR_OFFSET 6
#define TDSRRCLLC_MAX_E_MAC_D_FLOW_NUM_RETX   15
#define TDSRRCLLC_MAX_NUM_OF_RLC_PDU_SIZE_PER_LOGCHAN 32


#define   TDSRRCLLC_EUL_NOOP                 0x00
#define   TDSRRCLLC_EUL_START                0x01
#define   TDSRRCLLC_EUL_RECFG_UL             0x02
#define   TDSRRCLLC_EUL_RECFG_DL             0x04
#define   TDSRRCLLC_EUL_STOP                 0x08
#define   TDSRRCLLC_NON_SCHED_GRANT_START        0x10

#define   TDSRRC_SCHED_START            1
#define   TDSRRC_NON_SCHED_START        2

#endif /* FEATURE_TDSCDMA_HSUPA */

/*
* The following define the MAC Header sizes.
*/
/* Common logical channel mapped on a RACH */
#define TDSRRCLLC_COM_LOG_CHL_RACH_MAC_HDR_SIZE               2
/* Multiple dedicated logical channels on a RACH */
#define TDSRRCLLC_DED_MULTI_LOG_CHL_RACH_MAC_HDR_SIZE         26
/* Single dedicated logical channels on a RACH */
#define TDSRRCLLC_DED_SINGLE_LOG_CHL_RACH_MAC_HDR_SIZE         26

/* Common logical channel on a FACH */
#define TDSRRCLLC_COM_LOG_CHL_FACH_MAC_HDR_SIZE               3

/* Multiple Dedicated logical channels on a FACH with CRNTI */
#define TDSRRCLLC_DED_MULTI_LOG_CHL_FACH_CRNTI_MAC_HDR_SIZE   27
/* Single Dedicated logical channel on a FACH with CRNTI */
#define TDSRRCLLC_DED_SINGLE_LOG_CHL_FACH_CRNTI_MAC_HDR_SIZE  27

/* Multiple dedicated logical channels on a FACH with URNTI */
#define TDSRRCLLC_DED_MULTI_LOG_CHL_FACH_URNTI_MAC_HDR_SIZE   43
/* Single dedicated logical channels on a FACH with URNTI */
#define TDSRRCLLC_DED_SINGLE_LOG_CHL_FACH_URNTI_MAC_HDR_SIZE  43

/* Multiple dedicated logical channels on a DL DCH */
#define TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE         4
/* Single dedicated logical channel on a DL DCH */
#define TDSRRCLLC_DED_SINGLE_LOG_CHL_DCH_MAC_HDR_SIZE        0

/* Default values as given in the spec */
/* All persistence scaling factor values in RRC as actual value * 10.
 * So a default value of 1 becomes 10.
 */
#define TDSRRCLLC_DEFAULT_PERSISTENCE_SCALING_FACTOR_VAL 10

/* Defines multiplication factor for Persistence values */
/* #define TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR   10000000 */ // TDS value
#define TDSRRCLLC_PVAL_MULTIPLICATION_FACTOR   0xFFFF //WCDMA value

/* Defines multiplication factor for default DPCH Offset value */
#define TDSRRCLLC_DOFF_MULTIPLICATION_FACTOR   512

/* Defines multiplication factor for TAU DPCH */
#define TDSRRCLLC_TAU_DPCH_MULTIPLICATION_FACTOR   256

/* Defines the scr code multi factor (L1 representation) */
#define TDSRRCLLC_SCR_CODE_MULTIPLICATION_FACTOR     16

/* Max Doff value in ASN1 */
#define TDSRRCLLC_ASN1_DOFF_MAX_VAL  7

/* Default value for ASC0 PRACH Signature End Index */
#define  TDSRRCLLC_ASC0_DEFAULT_SIGNATURE_END_INDEX    15
/* Default value for ASC0 PRACH Signature Start Index */
#define  TDSRRCLLC_ASC0_DEFAULT_SIGNATURE_START_INDEX   0
/* Default value for ASC0 Assigned subchannel mask */
#define TDSRRCLLC_ASC0_DEFAULT_ASSIGNED_SUBCHAN_MASK    0x0f

#ifdef FEATURE_UMTS_PDCP
/* definition of default values for IEs related to RFC 2507 */

#define TDSRFC_2507_F_MAX_PERIOD     256
#define TDSRFC_2507_F_MAX_TIME       5
#define TDSRFC_2507_MAX_HEADER       168
#define TDSRFC_2507_TCP_SPACE        15
#define TDSRFC_2507_NON_TCP_SPACE    15

/* definition of default values for IEs related to RFC 3095 [ROHC] */

#define TDSRFC_3095_MAX_CID          15
#define TDSRFC_3095_REVERSE_DECOMPRESSION_DEPTH   0
#define TDSRFC_3095_MAX_ROHC_PROFILES    3
#define TDSRFC_3095_MAX_PCKT_SIZE_LIST      3
#endif /* FEATURE_UMTS_PDCP*/

#define TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT 0
/* MAX EDCH RL in Active set allowed */
#define TDSRRC_EUL_MAX_RL 4

#define TDSRRC_INVALID_CPID 128
#define TDSRRC_ASC_SUB_CHAN_SIZE_1   1
#define TDSRRC_ASC_SUB_CHAN_SIZE_2   2
#define TDSRRC_ASC_SUB_CHAN_SIZE_4   4
#define TDSRRC_ASC_SUB_CHAN_SIZE_8   8


/*Value in ms */
#define TDSRRC_DEFAULT_T_RUCCH_TIMER   120  
#define TDSRRC_DEFAULT_T_WAIT_TIMER   40  
#define TDSRRC_DEFAULT_T_SI_TIMER   40 

#define TDSRRC_DEFAULT_N_RUCCH         3    
#define TDSRRC_INVALID_EXT_EST_WINDOW     0xFF  
#define TDSRRC_MAX_NUMBITS_8               8
#define TDSRRC_MAX_NUMBITS_4               4
#define TDSRRC_MAX_NUMBITS_5               5
#define TDSRRC_DEFAULT_P_BASE_GAP                1
#define TDSRRC_DEFAULT_UL_SYNC_STEP              1
#define TDSRRC_DEFAULT_UL_SYNC_FREQ              1
#define TDSRRC_DEFAULT_N_E_UCCH     1

/* This macro computes the totalk number of Downlink transport channels
 * active in the configuration pointed to by the passed pointer. The
 * number is the sum of number of BCHs, PCHs, FACHs and DCHs.
 * Note: Number of DCHs in any state other than Cell_DCH should be 0
 *       Number of FACHs in any state other than Cell_FACH and Connecting
 *       should be 0.
 */

#define  TDSRRCLLC_DOWNLINK_TRCH_COUNT(config_ptr)  \
         (                                        \
            config_ptr->mac_dl_parms.num_bchs +   \
            config_ptr->mac_dl_parms.num_pchs +   \
            config_ptr->mac_dl_parms.num_fachs +  \
            config_ptr->mac_dl_parms.num_dchs     \
         )
/* This macro computes the total number of Uplink Transport Channels
 * active in the Configuration, a pointer to which is passed. If the
 * CCTrCH type is invalid, number of transport channels is 0, for RACH
 * it is fixed to 1 (only 1 RACH supported) and for DCH, the number is the
 * total number of DCHs present in MAC Uplink parameter structure
 */
#define  TDSRRCLLC_UPLINK_TRCH_COUNT(config_ptr)     \
         (                                        \
            (config_ptr->mac_ul_parms.cctrch_ptr->cctrch_type           \
               == TDSINVALID_UL_CCTRCH) ? 0 :        \
              (                                   \
               (config_ptr->mac_ul_parms.cctrch_ptr->cctrch_type == TDSRACH) ? 1 : \
                (                                 \
                  config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs     \
                )                                 \
              )                                   \
         )

#define TDSRRCLLC_INVALID_RB_ID                  0xFF

#define TDSRRCLLC_SIB6_SCCPCH_LCR_EXT_PRESENT(sib6_ptr)  \
    ( \
    (sib6_ptr->m.v4b0NonCriticalExtensionsPresent) && \
    (sib6_ptr->v4b0NonCriticalExtensions.m.sysInfoType6_v4b0extPresent) && \
    (sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.m.tdd128SpecificInfoPresent) &&  \
    (sib6_ptr->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.tdd128SpecificInfo.m.sCCPCH_LCR_ExtensionsListPresent))

#define TDSRRCLLC_SIB5_SCCPCH_LCR_EXT_PRESENT(sib5_ptr)  \
    ( \
    (sib5_ptr->m.v4b0NonCriticalExtensionsPresent) && \
    (sib5_ptr->v4b0NonCriticalExtensions.m.sysInfoType5_v4b0extPresent) && \
    (sib5_ptr->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.m.tdd128SpecificInfoPresent))




#ifdef FEATURE_TDSCDMA_CPC_DTX
#define TDSUE_MAX_DTX_DRX_OFFSET 159
#endif

#define TDSRRC_DL_INVALID_CTFC_VALUE 0xFFFF
#define TDSRRC_DL_MAX_TRANSPORT_BLOCKS 32

#define TDSRRC_SRB1_PRIORITY 1
#define TDSRRC_SRB2_PRIORITY 2
#define TDSRRC_SRB3_PRIORITY 3
#define TDSRRC_SRB4_PRIORITY 4
#define TDSRRC_CS_RB_PRIORITY 5
#define TDSRRC_PS_RB_PRIORITY_OFFSET TDSRRC_CS_RB_PRIORITY
#define TDSRRC_MAX_MAC_LOGICAL_CHAN_PRIORITY 8

#if defined(FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM)
#define TDSRRC_SAMPLING_RATE_NB 8000
#define TDSRRC_SAMPLING_RATE_WB 16000
#endif


/*-------------------------------------------------------------------
ENUM: tdsrrcllc_dl_ctfc_entry_state_enum_type

TFC restriction state enum type. This is based on the RM restriction
information.
--------------------------------------------------------------------*/
typedef enum
{
  TDSINVALID, /* available to be used */
  TDSALLOWED, /* Allowed and should be accounted for rate matching */
  TDSRESTRICTED /* Some TFI in this TFC is not allowed. Hence should
                should not be used in RM calculation */
} tdsrrcllc_dl_ctfc_entry_state_enum_type;

typedef enum
{
  TDSORDERED_CONFIG,
  TDSCURRENT_CONFIG,
  TDSTRANSITION_CONFIG
} tdsrrcllc_config_e_type;


typedef enum
{
  TDSRRCLLC_ADD_BCH,
  TDSRRCLLC_DROP_BCH,
  TDSRRCLLC_NO_OP_BCH
} tdsrrcllc_bch_op_e_type;

typedef enum
{
  TDSRRCLLC_ADD_NBCH,
  TDSRRCLLC_DROP_NBCH,
  TDSRRCLLC_NO_OP_NBCH
}tdsrrcllc_nbch_op_e_type;

typedef enum
{
  TDSRRCLLC_ADD_PCH,
  TDSRRCLLC_RECONFIG_PCH,
  TDSRRCLLC_NO_OP_PCH
} tdsrrcllc_pch_op_e_type;

typedef enum
{
  TDSRRCLLC_ADD_CCCH,
  TDSRRCLLC_RECONFIG_CCCH,
  TDSRRCLLC_NO_OP_CCCH
} tdsrrcllc_ccch_op_e_type;

/* This enumerates the possible status values that
 * processing of Downlink Info per RL can assume
 */
typedef enum
{
  TDSRRCLLCOC_DL_INFO_PER_RL_SUCCESS,
  TDSRRCLLCOC_DL_INFO_PER_RL_FAILURE,
  TDSRRCLLCOC_DL_INFO_PER_RL_IGNORED
}tdsrrcllcoc_dl_info_per_rl_status_e_type;


#define TDSRRC_INVALID_URA_ID   0xFFFFFFFF

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
typedef enum
{
  TDSRRCLLC_NO_CTCH, /* no CTCH, one SCCPCH with paging */
  TDSRRCLLC_CTCH_WITH_PCCH, /*  Only one SCCPCH with CTCH and PCCH*/
  TDSRRCLLC_CTCH_ON_DIF_SCCPCH /*CTCH is on different SCCPCH */
} tdsrrcllc_ctch_status_e_type;
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT

#ifdef FEATURE_TDSCDMA_CPC_DTX
/* This enum indicate what action L1 needs to take on received HS-SCCH orders*/
typedef enum
{
  /* L1 to save & continue using any previously received HS-SCCH orders*/
  TDSL1_HS_SCCH_ORDER_ACTION_BACKUP,
  /*L1 to restore any previously received HS-SCCH orders which where Acd*/
  TDSL1_HS_SCCH_ORDER_ACTION_RESTORE,
  /*L1 to act as if HS-SCCH orders where never  received*/
  TDSL1_HS_SCCH_ORDER_ACTION_DISCARD
}tdsl1_dtx_drx_hs_scch_order_info_enum_type;

#endif

#ifdef FEATURE_TDSCDMA_MIMO
#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
extern int8 tdsrrc_mimo_s_cpich_po;
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/
#endif /*FEATURE_TDSCDMA_MIMO*/

/* This enum indicate CN domain type, NONE is initial value */
typedef enum
{
  TDSRRC_CN_DOMAIN_CS,
  TDSRRC_CN_DOMAIN_PS,
  TDSRRC_CN_DOMAIN_NONE
}tdsrrcllc_cn_domain_enum_type;

/*declared as staic in tdsrrcllc.c*/
/*extern tdsrrcllc_rrc_cmd_process_data_type  rrcllc_cmd_under_process;*/
/*===================================================================
                        DATA STRUCTURES
====================================================================*/
/*___________________________________________________________________
STRUCTURE: rrcllcoc_pdcp_param_type

This stucture contains PDCP related info needed by the PDCP layer for 
a particular RB.
--------------------------------------------------------------------*/

#ifdef FEATURE_UMTS_PDCP
#define TDSRRC_INVALID_MAX_CS_DELAY 0xFF
typedef enum
{
  TDSCPDCP_SETUP,
  TDSCPDCP_RECONFIG,
  TDSCPDCP_RELEASE
} tdsrrcllcoc_pdcp_action_etype;

typedef struct
{
  tdsrrc_RB_Identity rb_id;
  boolean pdcp_info_changed;
  boolean in_use;
  pdcp_cfg_req_data_type pdcp_cfg_req;
}tdsrrcllcoc_pdcp_config_req_type;

typedef struct
{
  uint32 num_rbs;
  tdsrrcllcoc_pdcp_config_req_type pdcp_config_req[MAX_RAB_TO_SETUP];
  //pdcp_sn_info_type pdcp_sn_info;
  /*Since only one PDCP entity will have AMR Rate associated with it. So keeping
  ul_amr_rate variable outside so that it's easier to retrieve.*/
}tdsrrcllcoc_pdcp_parms_type;

#endif /* FEATURE_UMTS_PDCP */


/*-------------------------------------------------------------------
STRUCTURE: tdsrrcllcoc_trch_info_type

This structure is used to store Transport Channel information.
--------------------------------------------------------------------*/
typedef struct
{
  /* The Transport Channel(TrCH) ID */
  tr_ch_id_type               trch_id;

  /* Number of Transport Formats(TF) supported */
  uint32                      tf_cnt;

  /* Number of Logical Channels supported */
  uint32                      lg_ch_cnt;

  /* Set to TRUE when a DCH is mapped to a TrCH */
  boolean                     dch_present;

  /* TRUE when the logical channel mapped to this transport channel
     dropped from 2(multiple) to 1. */
  boolean                     lg_ch_cnt_down;

  /* TRUE when the lc mapped to this TrCH increased from 1 to 2. */
  boolean                     lg_ch_cnt_up;

} tdsrrcllcoc_trch_info_type;

/*-------------------------------------------------------------------
STRUCTURE: tdsrrcllc_dl_ctfc_entry_info_struct_type

This strcuture is for each TFC information
--------------------------------------------------------------------*/
typedef struct
{
  /* CTFC entry state valid/allowed/restricted */
  tdsrrcllc_dl_ctfc_entry_state_enum_type state;

  /* CTFC value representing the TFI for this TFC */
  uint32 ctfc_value;

  /* TFI for each TrCh in this TrCh */
  uint8 tfi_table[TDSUE_MAX_TRCH];
} tdsrrcllc_dl_ctfc_entry_info_struct_type;

/*-------------------------------------------------------------------
STRUCTURE: tdsrrcllc_dl_ctfc_info_struct_type

This structure definition is for storing all the TFC information in
the TFCS
--------------------------------------------------------------------*/
typedef struct
{
  /* last CTFC or TFC index. */
  uint16  num_ctfc_entry;

  /* Pointer to the array of TFC or CTFC entries. */
  tdsrrcllc_dl_ctfc_entry_info_struct_type  ctfc_entry_data[TDSUE_MAX_TFC];

} tdsrrcllc_dl_ctfc_info_struct_type;

/*-------------------------------------------------------------------
STRUCTURE: tdsrrcllc_rm_restricted_trch_info_struct_type

This structure definition is for storing the RM restriction
information for a given TrCh
--------------------------------------------------------------------*/
typedef struct
{
  /* TrCh Id of the restricted TrCh */
  uint8 trch_id;

  /* number of TF info entries */
  uint8 num_tf;

  /* allowed TF indicies per TrCH */
  boolean tf_allowed[TDSUE_MAX_TF];
} tdsrrcllc_rm_restricted_trch_info_struct_type;

/*-------------------------------------------------------------------
STRUCTURE: tdsrrcllc_rm_restrict_info_struct_type

This structure definition is for storing the RM restriction
information for all given TrChs
--------------------------------------------------------------------*/
typedef struct
{
  /* indicate if the RM restriction info exists or NOT */
  boolean restriction_info_exist;

  /* number of TrCh in rm restriction info */
  uint8 num_trch;

  /* restrcited TrCh info */
  tdsrrcllc_rm_restricted_trch_info_struct_type restricted_trch_info[TDSUE_MAX_TRCH];
} tdsrrcllc_rm_restrict_info_struct_type;


/*-------------------------------------------------------------------
STRUCTURE: tdsrrcllc_dl_dch_info_buffer_type

This structure holds all DL DCH transport channel specific data that
is of use to L1 or MAC. Note that this is a temporary storage for
ordered config operations.
--------------------------------------------------------------------*/
typedef struct
{
  tdsl1_dl_cctrchcfg_trch_info_struct_type   l1_info;
  tdsl1_dl_cctrchcfg_tf_info_struct_type     l1_tf_info[TDSUE_MAX_TF];
  tdsmac_dl_dch_trch_config_type   mac_info;
  tdsrrcllcoc_trch_info_type       local_info;
}tdsrrcllc_dl_dch_info_buffer_type;

/*-------------------------------------------------------------------
STRUCTURE: tdsrrcllc_ul_dch_info_buffer_type

This structure holds all UL DCH transport channel specific data that
is of use to L1 or MAC. Note that this is a temporary storage for
ordered config operations.
--------------------------------------------------------------------*/
typedef struct
{
  tdsl1_ul_semi_static_parms_struct_type   l1_semi_static_parms;
  tdsmac_ul_tfs_type                       mac_tfs_info;
  tdsmac_ul_dch_trch_config_type           mac_dch_info;
  tdsrrcllcoc_trch_info_type               local_info;
}tdsrrcllc_ul_dch_info_buffer_type;


/* This stores data needed by LLC */
typedef struct
{
  /* Indicates if PCCPCH is up currently */
  boolean   pccpch_is_up;
  /* Indicates that N_PCCPCH is up currently */
  boolean   n_pccpch_is_up;
  /* Indicates if SCCPCH is up currently */
  boolean   sccpch_is_up;
  /* Indicates if DL DPCH is up currently */
  boolean   dl_dpch_is_up;
  /* CCTrCH info for PCCPCH */
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type   pccpch;
  /* CCTrCH info for N_PCCPCH */
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type   n_pccpch;
  /* PCCPCH parameters */
  tdsl1_dl_phycfg_pccpch_parms_struct_type     pccpch_parms;
  /* N_PCCPCH parameters */
  tdsl1_dl_phycfg_pccpch_parms_struct_type     n_pccpch_parms;
  /* CCTrCH info for SCCPCH */
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type   sccpch;
  /* CCTrCH info for DPCH */
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type   dl_dpch;
  /* Indicates if FACH transport blocks are present on SCCPCH */
  boolean   fach_included;
  /* Indicates if PCH transport blocks are present on SCCPCH */
  boolean   pch_included;

  /*below is code that not supported by current TDS */
#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  /* Indicates if SCCPCH is up currently */
  boolean   sccpch_with_ctch_is_up;

  /* Indicates if SCCPCH with ctch is up currently */
  tdsrrcllc_ctch_status_e_type  ctch_status;

  /* Indicates if PCH transport blocks are present on SCCPCH that
  has been selected for  CTCH */
  boolean   pch_included_for_sccpch_with_ctch;

  /* CCTrCH info for SCCPCH with CTCH*/
  tdsl1_dl_cctrch_info_struct_type   sccpch_with_ctch;
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

}tdsrrcllc_dl_phy_chan_data_type;


typedef struct
{
  /* Indicates if PRACH is up currently */
  boolean   prach_is_up;
  /* Indicates if UL DPCH is up currently */
  boolean   ul_dpch_is_up;

  cctrch_id_type  ul_dpch_cctrch_id;
  cctrch_id_type  prach_cctrch_id;
}tdsrrcllc_ul_phy_chan_data_type;

/* This struct need by save mac_hd queue id in RRC */
#define TDSRRC_MAC_DFLOW_QUEUEID_INVAILD    0
typedef struct
{
  /* MAC-d Flow Id. main with tdsordered_config_ptr->mac_dflow_info[idx].mac_dflow_id */
  uint8 mac_dflow_id;

  /*RRC use, to flag that mac-hsQueueId has been added. */
  uint8 add_queues_bitmask;
}tdsrrcllc_dl_mac_dflow_type;


/*-------------------------------------------------------------------
This structure holds semi-permanent data pertaining to LLC Uplink
and Downlink Physical channels as well as Uplink MAC Config flag
--------------------------------------------------------------------*/
typedef struct
{
  tdsrrcllc_dl_phy_chan_data_type  dl_phy_chan;
  tdsrrcllc_ul_phy_chan_data_type  ul_phy_chan;
  boolean                       send_mac_uplink_config;
  tdsrrcllc_dl_mac_dflow_type   mac_dflow_info[UE_MAX_MAC_D_FLOW];
}tdsrrcllc_semi_permanent_data_type;



/*-------------------------------------------------------------------
This structure indicates need for reconfiguring UL and DL MAC and L1.
This is determined based on presence of corresponding IEs.
--------------------------------------------------------------------*/
typedef struct
{
  boolean  uplink_mac;
  boolean  downlink_mac;
  boolean  uplink_l1;
  boolean  downlink_l1;
}tdsrrcllc_lower_layer_reconfig_need_type;

typedef struct
{
  /* Number of dedicated RLC logical channels mapped to TrCh */
  uint8             dl_ndlchan;
  uint8             ul_ndlchan;
  /* Info of each dedicated logical channel mapped */
  tdsmac_ul_ded_logch_config_type    ul_dlchan_info[MAX_DED_LOGCHAN_PER_TRCH];
  /* Configuration of each dedicated logical channel mapped to */
  /* this transport channel */
  tdsmac_dl_ded_logch_config_type    dl_dlchan_info[MAX_DED_LOGCHAN_PER_TRCH];

  /* Future place for storing RLC size restrictions */

}tdsrrcllc_rb_mapping_info;

/*-------------------------------------------------------------------
STRUCTURE: tdsrrcllc_rb_list_mapped_to_dch_type

This structure holds num of RBs in rrc_rb_mapped_to_dch. 
rrc_rb_mapped_to_dch contains those RBs for which RLC PDU size has changed.
--------------------------------------------------------------------*/
typedef struct {
  uint8 num_of_RBs;
  tdsrrc_RB_Identity rrc_rb_mapped_to_dch[TDS_MAX_RB];
} tdsrrcllc_rb_list_mapped_to_dch_type;

typedef enum {
  TDSUL,
  TDSDL
} tdsrrc_direction_enum_type;

/*-------------------------------------------------------------------
STRUCTURE: rrcllc_rlc_size_list_type

This structure holds data extracted from an RLC Size List IE.
--------------------------------------------------------------------*/
typedef struct
{
  tdsrrc_RB_Identity rb_id;
  tdsrrc_rlc_size_restriction_config_e_type restriction_type;
  uint8 count;
  uint16 size_idx[TDSUE_MAX_TF];
}tdsrrcllcoc_rlc_size_list_type;

/*-------------------------------------------------------------------
STRUCTURE: rrcllc_logical_channel_list_type

This structure holds data extracted from a Logical Channel List IE.
--------------------------------------------------------------------*/
typedef struct
{
  tdsrrc_rlc_size_restriction_config_e_type restriction_type;
  uint8 count;
  tdsrrc_RB_Identity rb_id[15];
}tdsrrcllcoc_logical_channel_list_type;

/*-------------------------------------------------------------------
STRUCTURE: rrcllc_rlc_size_restriction_info_type

This structure contains the raw RLC size restriction info for a
particular channel.  This data is later validated and copied into
the MAC's data structures.
--------------------------------------------------------------------*/
typedef struct
{
  tdsrrcllcoc_rlc_size_list_type rlc_size_list[MAX_DED_LOGCHAN_PER_TRCH];
  tdsrrcllcoc_logical_channel_list_type logical_channel_list[TDSUE_MAX_TF];
  boolean mac_update_needed;
}tdsrrcllcoc_rlc_size_restriction_info_type;




typedef enum restriction_choice_e {
  TDSTFC_CHOICE_NOT_VALID   = -1,
  TDSMINIMUM_ALLOWED_TFC  = 0,
  TDSALLOWED_TFC_LIST     = 1,
  TDSNON_ALLOWED_TFC_LIST = 2,
  TDSRESTRICTED_TRCH_LIST = 3,
  TDSTFCS_FULL            = 4
} tdsrestriction_choice_enum_type;

typedef struct allowed_tfc_list_s {
  uint16 total_allowed_tfc_count; /* Total count of allowed TFCs; can be more than TDSUE_MAX_TFC */
  uint16 allowed_tfc_number[TDSUE_MAX_TFC]; /* List of allowed TFC numbers */
} tdsallowed_tfc_list_type;

typedef struct non_allowed_tfc_list_s {
  uint16 total_non_allowed_tfc_count; /* Total count of non allowed TFCs; can be more than TDSUE_MAX_TFC */
  uint16 non_allowed_tfc_number[TDSUE_MAX_TFC]; /* List of disallowed TFC numbers */
} tdsnon_allowed_tfc_list_type;

typedef struct allowed_tfi_info_s {
  uint16 no_of_allowed_tfis; /* Total number of allowed TFIs; can be more than UE_MAX_NUM_TF_64 */
  uint8 tfi[TDSUE_MAX_TF]; /* List of TFIs which are allowed for TFC selection */
} tdsallowed_tfi_info_type;

typedef struct trch_info_s {
  tdsallowed_tfi_info_type  allowed_tfi_info; 
  uint32                 trch_id;     /* Transport channel ID signalled */
  boolean                dch_present; /* TRUE if DCH transport channel type is signalled */
  boolean                allowed_tfi_info_present; /* TRUE indicates presence */
} tdstrch_info_type;

typedef struct restricted_trch_list_s {
  uint8                 no_of_transport_channels; /* Number of restricted transport channels; can be more than TDSUE_MAX_TRCH */
  tdstrch_info_type         trch_info[TDSUE_MAX_TRCH]; /* Info related to each restricteed transport channel */
} tdsrestricted_trch_list_type;

typedef union tfc_choice_info_u {
  uint32                     minimum_allowed_tfc_number; /* Minimum number below which all the TFCs are valid */
  tdsallowed_tfc_list_type      allowed_tfc_list; /* Info on list of allowed TFCs */
  tdsnon_allowed_tfc_list_type  non_allowed_tfc_list; /* Info on list of disallowed TFCs */
  tdsrestricted_trch_list_type  restricted_trch_list; /* Info on list of restricted TFCs based on transport channels */
} tdstfc_choice_info_u_type;

typedef struct tfc_subset_info_s {
  tdsrestriction_choice_enum_type restriction_choice; /* TFC restriction choice */
  tdstfc_choice_info_u_type       tfc_choice_info;    /* Info related to the selected restriction choice */
} tdstfc_subset_info_type;


#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
typedef struct rrcllc_oc_tfc_subset_list_s {
  uint32                no_of_subset_lists; /* The number of TFCsubset lists stored; can be more than UE_MAX_TFC_SUBSET */
  tdstfc_subset_info_type  tfc_subset_info[UE_MAX_TFC_SUBSET]; /* Each subsets' list info */
} tdsrrcllc_oc_tfc_subset_list_type;

typedef struct {
  boolean srb5_rb_exists;  /* TRUE indicates presence of SRB#5 */
  tdsrrc_RB_Identity rb_id;           /* SRB#5 RB ID */
  boolean srb5_trch_exists;/* TRUE indicates Transport Channel for this RB exists*/
  uint32  trch_id;         /* Transport channel ID on which SRB#5 is mapped */
  uint16  lc_id;           /* Logical channel ID that is allocated for this */
} tdsrrcllc_srb5_info_type;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

typedef struct
{
  tdsl1_ul_prach_cfg_struct_type prach_cfg; /*PRACH config parameters*/
  tdsrrcllcoc_trch_info_type   ul_rach_trch_info[TDSUE_MAX_TRCH];  /*RACH TrCH Info*/
  boolean                        ul_rach_trch_idx_in_use[TDSUE_MAX_TRCH];
  tdsl1_ul_asc_struct_type                 asc_info[MAX_ASC];  
} tdsprach_config_type;

typedef struct
{

  /* number of transport channels valid for this CCTrCH */
  uint8                                 num_trch;
  /* number of TFCIs */
   uint8                                 num_tfci;
  /* Semi-static parameters for Uplink Transport channels */
  tdsl1_ul_semi_static_parms_struct_type   semi_static_parms[TDSUE_MAX_TRCH];
   /* Map of Gain factor parameters for Uplink */
  tdsl1_ul_gain_factor_parms_struct_type   gain_factor_parms[TDSL1_UL_TFC_MAX];
}tdsrrc_ul_cctrch_struct_type;


typedef struct {
  rlc_lc_id_type lc_id;          /* Unique identifier for the Downlink RLC logical channel */
  tdsrrc_RB_Identity rb_id;               /*rb-id provided in OTA*/
  uecomdef_logchan_e_type lc_type;      /* Type of Downlink UM logical channel */
  
  tdsrlc_li_e_type li_size;        /* Network can explicitly specify li size in Release 5 */

  boolean alternate_e_bit;              /* TRUE - Alternate E-bit interpretation */
                                        /* FALSE- Normal E-bit interpretation    */
} tdsrrc_rlc_dl_um_channel_config_type;

typedef struct {
  uint8  nchan;          /* Number of transparent mode DL logical channels to configure */ 
  tdsrrc_rlc_dl_um_channel_config_type chan_info[TDSUE_MAX_DL_UM_CHANNEL];
                                        /* Downlink UM RLC configuration for each channel*/ 
} tdsrrc_rlc_dl_um_config_type;

typedef struct {
  rlc_lc_id_type lc_id;      /* Unique identifier for the Uplink RLC logical channel */
  tdsrrc_RB_Identity rb_id;          /*rb-id provided in OTA*/
  uecomdef_logchan_e_type lc_type;   /* Type of Downlink UM logical channel */
  uint16 timer_discard;       /* Elapsed time in milliseconds before an SDU is discarded at the transmitter*/
  
  boolean alternate_e_bit;              /* TRUE - Alternate E-bit interpretation */
                                        /* FALSE- Normal E-bit interpretation    */
} tdsrrc_rlc_ul_um_channel_config_type;

typedef struct {
  uint8  nchan;    /* Number of transparent mode UL logical channels to configure */ 
  tdsrrc_rlc_ul_um_channel_config_type chan_info[TDSUE_MAX_UL_UM_CHANNEL];
                                        /* Uplink UM RLC configuration for each channel*/
} tdsrrc_rlc_ul_um_config_type;
/* Indicates if the RLC is provided in OTA or not.
   And if provided ,if it is fixed or flexible*/
typedef enum
{
  TDSRRC_PDU_SIZE_NOT_SET,
  TDSRRC_FIXED_PDU_SIZE_SET,
  TDSRRC_FLEXIBLE_PDU_SIZE_SET
}tdsrrc_am_rlc_pdu_enum_type;
/* Stores if the RLC is provided in OTA or not . 
And stores the rlc size indicated by the OTA */
typedef struct {

 tdsrrc_am_rlc_pdu_enum_type rrc_pdu_type;

 tdsrrc_RB_Identity  rb_id;
 uint32 dl_rlc_am_pdu_size;
}tdsrrc_rb_dl_am_rlc_size_info;
/*-------------------------------------------------------------------
STRUCTURE: rrc_ordered_config_type

This structure defines the variable TDSORDERED_CONFIG as defined in
the RRC spec 25.331.
--------------------------------------------------------------------*/
typedef struct
{
  /* Physical Layer Ordered Config Data */
  /* CCTrCH info */
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type   l1_dl_cctrch_parms;
  tdsl1_dl_cctrchcfg_ctfc_info_struct_type     l1_dl_ctfc_parms;
  tdsl1_ul_cctrch_cfg_struct_type l1_ul_cctrch_parms;

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  /* CCTrCH info for sccpch carrying CTCH*/
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type   l1_dl_cctrch_parms_for_sccpch_with_ctch;
  tdsl1_dl_cctrchcfg_ctfc_info_struct_type     l1_dl_ctfc_parms_for_sccpch_with_ctch;
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

  /* Phychan info */
  tdsl1_dl_phycfg_phychan_db_struct_type    l1_dl_chan_parms;
  tdsl1_ul_phych_cfg_struct_type   l1_ul_chan_parms;

  /* Record UL TFCS ID, Only support one TFCS ID current.
        define array for future */
  uint8  ul_tfcs_id[L1_MAX_CCTRCH];

  /* This is RRC information to indicate the queue/s used by dflow */
  boolean dl_dflow_index_in_use[UE_MAX_MAC_D_FLOW];

  /* L1 HSDPA Information */
  tdsl1_hs_info_struct_type l1_hsdpa_info;

  /* MAC storage space for HS flow Id */
  tdsmac_dflow_struct_type    mac_dflow_info[UE_MAX_MAC_D_FLOW];

  tdsrrc_hsdpa_hrnti_enum_type hrnti_status;

  uint16 hrnti;

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS

  /* This is RRC information to indicate the queue/s used */
  boolean mac_ehs_lc_id_in_use[TDSUE_MAX_DL_LOGICAL_CHANNEL];

  /* This is RRC information to indicate the queue/s used */
  boolean mac_ehs_queue_index_in_use[UE_MAX_HS_QUEUE];

  /* Info of dedicated logical channels mapped */
  tdsmac_dl_ehs_logch_config_type dl_macehs_logchan_info[TDSUE_MAX_DL_LOGICAL_CHANNEL];
  
  /* MAC storage space for EHS Q Id */
  tdsmac_ehs_queue_struct_type    mac_ehs_queue_info[UE_MAX_HS_QUEUE];

#endif /* FEATURE_TDSCDMA_HSDPA_MAC_EHS */

#ifdef FEATURE_TDSCDMA_HSUPA
  /* E_DCH_TRANSMISSION variable to be evaluated after processing config message */
  boolean e_dch_transmission;

  /* To be indicator weather sync ul bitmap present in sib 770ext.
       TRUE means "E-RUCCH Sync UL code bitmap" present.*/
  boolean sync_code_erucch_770ext_indicator;

  /* set to MAC_EUL_REVERT_TO_OLD_TSN whenever UE has to reverting back to old
     config. MAC-E should process this information first(before mac-e-reset
     indicator). 
     If it is set to MAC_EUL_REVERT_TO_OLD_TSN then MAC-E is expected to
     revert to the old TSN information which was stored on receipt of last
     CMAC_UL_CONFIG_REQ.
     If it is set to MAC_EUL_REVERT_TO_OLD_CONFIG_BACKUP_NEEDED then back up of
     current config(like TSN) is needed. */
  tdsmac_e_tsn_action_e_type   mac_e_tsn_action;
  
 /* Indicates L1 to flush all HARQs.
    RRC will set this to TRUE under following conditions:
      - NW explicitly indicate in the reconfig message
      - whenever TTI reconfiguration happens 

    When this variable is set to TRUE, then all the HARQ processes
    will be flushed in Firmware. MAC will use this variable to flush all
    HARQ process related information that is maintained internally.
    Thsi information needs to be flushed so that happy bit is computed
    correctly in MAC */
 boolean mac_e_reset_indicator;
 /* req mask for EUL configuration */
 /* This request mask will outline the action related to HSUPA UL and DL */
 /* bitmask:
      TDSRRCLLC_EUL_NOOP                 0x00
      TDSRRCLLC_EUL_START                0x01
      TDSRRCLLC_EUL_RECFG_UL             0x02
      TDSRRCLLC_EUL_RECFG_DL             0x04
      TDSRRCLLC_EUL_STOP                 0x08 */

 uint8 l1_e_req_mask;

  /**
 TDSRRC_SCHED_START    1
 TDSRRC_NON_SCHED_START 2
*/
uint8       hsupa_start_mask;

 tdsl1_e_info_struct_type l1_e_info;

 tdsmac_eul_action_e_type               mac_eul_action;

 /* MAC E-DCH config info - MAC should look into this pointer only if
    eul_action is MAC_EUL_CONFIG */
 tdsmac_e_config_s_type                 mac_e_config;

 boolean e_rnti_present;
#endif /* FEATURE_TDSCDMA_HSUPA */

#ifdef FEATURE_TDSCDMA_CPC_DTX
/* DTX_DRX_STATUS variable to be evaluated after processing config message */
/*L1 to not look at this */
boolean cpc_dtx_drx_status;

/* req mask for DTX-DRX configuration */
/* This request mask will outline the action related to DTX*/
/* bitmask:
  L1_CPC_DTX_NO_OP                      0x00
  L1_CPC_DTX_START                      0x01
  L1_CPC_DTX_RECONFIG                   0x02
  L1_CPC_DTX_STOP                       0x04*/
uint8 l1_cpc_dtx_req_mask;

#ifdef FEATURE_TDSCDMA_CPC_DRX
/* DRX_STATUS variable to be evaluated after processing config message */
/*L1 to not look at this */
boolean cpc_drx_status;

/* This request mask will outline the action related to DRX*/
/* bitmask:
  L1_CPC_DRX_NO_OP                      0x00
  L1_CPC_DRX_START                      0x01
  L1_CPC_DRX_RECONFIG                   0x02
  L1_CPC_DRX_STOP                       0x04*/
uint8 l1_cpc_drx_req_mask;
#endif

/* This enum indicate what action L1 needs to take on received HS-SCCH orders*/
#ifdef FEATURE_TDSCDMA_CPC_DRX
  tdsl1_dtx_drx_hs_scch_order_info_enum_type hs_scch_order_action;
#endif
/*Timing info needed to start DTX/DRX at L1 for CPC*/
l1_dtx_drx_timing_info_struct_type dtx_drx_timing_info;

/*All the info needed for DTX operation at L1 for CPC*/
l1_dtx_info_struct_type dtx_info;

#ifdef FEATURE_TDSCDMA_CPC_DRX
/*All the info needed for DRX operation at L1 for CPC*/
  l1_drx_info_struct_type drx_info;
#endif

#endif

  /* RLC size restriction data */
  tdsrrcllcoc_rlc_size_restriction_info_type rach_rlc_size_restriction_info;


  /* MAC Ordered Config Data */
  tdsmac_dl_config_type              mac_dl_parms;
  tdsmac_ul_config_type              mac_ul_parms;

  /* RLC Ordered Config Data */
  /* TM and UM Uplink */
  tdsrlc_ul_tm_config_type           rlc_ul_tm_parms;
  tdsrrc_rlc_ul_um_config_type       rlc_ul_um_parms;

  /* TM & UM Downlink */
  tdsrlc_dl_tm_config_type           rlc_dl_tm_parms;
  tdsrrc_rlc_dl_um_config_type       rlc_dl_um_parms;

  /* AM mode only has one type for both uplink and downlink */
  tdsrlc_am_config_req_type          rlc_am_parms;

#ifdef FEATURE_UMTS_PDCP
  tdsrrcllcoc_pdcp_parms_type        pdcp_parms;
#endif /* FEATURE_UMTS_PDCP */

  /* Start: Storage space for data needed by L1 */
  /* Downlink Transport channel data */
  tdsl1_dl_cctrchcfg_trch_info_struct_type           dl_trch_info[TDSUE_MAX_TRCH];
#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsl1_dl_cctrchcfg_trch_info_struct_type           dl_trch_info_for_sccpch_with_ctch[TDSUE_MAX_TRCH];
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif
  /* Downlink Dedicate Transport Format (TF) data per Transport Channel */
  tdsl1_dl_cctrchcfg_tf_info_struct_type             dl_tf_info[TDSUE_MAX_TRCH][TDSUE_MAX_TF];
#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsl1_dl_cctrchcfg_tf_info_struct_type             dl_tf_info_for_sccpch_with_ctch[TDSUE_MAX_TRCH][TDSUE_MAX_TF];
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif
  /* Downlink Transport Format Combinations (TFC) */
  tdsl1_dl_cctrchcfg_ctfc_entry_info_struct_type     ctfc_info[TDSUE_MAX_TFC];
#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsl1_dl_cctrchcfg_ctfc_entry_info_struct_type     ctfc_info_for_sccpch_with_ctch[TDSUE_MAX_TFC];
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif
  tdsrrc_ul_cctrch_struct_type rrc_ul_cctrch_params;
  /* Access Service Class Info per ASC */
  tdsl1_ul_asc_struct_type                 asc_info[MAX_ASC];
  
  /* Access Service Class Info per ASC for E-RUCCH */
  tdsl1_ul_asc_struct_type               erucch_asc_info[MAX_ASC];

  /* Needed for calculations - also store parameters in a slightly
   * different form.
   */
  /* CTFC management data used to calculate the TFCS */
  tdsrrcllc_dl_ctfc_info_struct_type       dl_ctfc;

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT

 /* CTFC management data used to calculate the TFCS
    for sccpch that carries CTCH */
  tdsrrcllc_dl_ctfc_info_struct_type       dl_ctfc_for_sccpch_with_ctch;

  /* Rate Matching info for DL Blind Rate Detection */
  tdsrrcllc_rm_restrict_info_struct_type   rm_restrict_for_sccpch_with_ctch;
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
  /* End: Storage space for data needed by L1 */

  /* Start: Storage space for data needed by MAC */
  /* FACH transport channel info */
  tdsmac_dl_fach_trch_config_type          fach_info[TDSUE_MAX_TRCH];

  /* TDSDCH Transport channel information */
  tdsmac_dl_dch_trch_config_type           dl_dch_info[TDSUE_MAX_TRCH];

  /* DSCH Transport channel information */
  tdsmac_dl_dsch_trch_config_type          dsch_info[TDSUE_MAX_TRCH];

  /* MAC Uplink TFCS information */
  tdsmac_ul_tfcs_type                      ul_tfcs;

  /* List of removed TFCIs, sorted at all times based on TFCI value */
  uint32                                removed_tfci_list[TDSUE_MAX_TFC];

  /* The number of removed TFCIs is */
  /* - set to zero initially  and also when UE receives complete TFCS configuration */
  /* - incremented by one for each TFCI removal and the TFCI is stored in the table */
  /* - decremented by one (if number of removed TFCIs > zero) when a TFCI is added */
  uint16                                num_removed_tfcis;

  /* MAC uplink CCTrCH information */
  tdsmac_ul_cctrch_config_type             ul_cctrch;

  /* End: Storage space for data needed by MAC */

  /* Start: Storage space for data needed by RLC */
  /* RLC-AM information */
  tdsrlc_am_config_type                    am_config[TDSUE_MAX_AM_ENTITY];
  /* End: Storage space for data needed by RLC */

  /*
  * Persistence Scaling factors used to calculate the Access
  * Service Class Establishment
  */
  uint32                          psf[MAX_ASC_PERSIST];
  /*
  * Persistence Scaling factors used to calculate the Access
  * Service Class Establishment for E-RUCCH
  */
  uint8                          erucch_psf[MAX_ASC_PERSIST];
     
  /*
  * Dynamic Persistence level used to calculate the Access
  * Service Class Establishment
  */
  tdsrrc_DynamicPersistenceLevel     dpl;

    /* Total count of all active UL and DL TrCHs */
  uint8                           dl_trch_cnt;
  uint8                           ul_trch_cnt;

  /* Data structures for Transport channel indexing */
  /* This is RRC information to indicate which TrCH indicies are in use. */
  boolean                         dl_pch_trch_idx_in_use;
  boolean                         dl_fach_trch_idx_in_use[TDSUE_MAX_TRCH];
  boolean                         dl_dch_trch_idx_in_use[TDSUE_MAX_TRCH];
  boolean                         ul_rach_trch_idx_in_use[TDSUE_MAX_TRCH];
  boolean                         ul_dch_trch_idx_in_use[TDSUE_MAX_TRCH];
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  boolean                         dl_pch_trch_idx_in_use_for_sccpch_with_ctch;
  boolean                         dl_fach_trch_idx_in_use_for_sccpch_with_ctch[TDSUE_MAX_TRCH];
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
  /* This stores the TrCH ID to TrCH Idx mapping info.*/
  tdsrrcllcoc_trch_info_type         dl_pch_trch_info;
  tdsrrcllcoc_trch_info_type         dl_fach_trch_info[TDSUE_MAX_TRCH];
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrcllcoc_trch_info_type         dl_fach_trch_info_for_sccpch_with_ctch[TDSUE_MAX_TRCH];
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrcllcoc_trch_info_type         dl_dch_trch_info[TDSUE_MAX_TRCH];
  tdsrrcllcoc_trch_info_type         ul_rach_trch_info[TDSUE_MAX_TRCH];
  tdsrrcllcoc_trch_info_type         ul_dch_trch_info[TDSUE_MAX_TRCH];

  /* End: Data structures for Transport channel indexing */

  /* This structure indicates the need for reconfiguring
   * Uplink and Downlink MAC and L1.
   */
  tdsrrcllc_lower_layer_reconfig_need_type  reconfig_needed;

  /* Access Class to Access Service Class Mapping value */
  uint8                           ac_to_asc_mapping[MAX_ASC_MAP];

  /* Store the Activation Time and DRX Cycle Lengths. */
    /* the type of action time.  NONE, SFN or CFN */
  tdsl1_act_time_enum_type           act_time_type;
  uint16                          activation_time;
  uint16                          sdu_time;
  tdsrrc_state_e_type                rrc_state_indicator;

  /* ZZTBD: for now in TDD, following 3 fields 
   * are not real DRX cycle length. Rather, they are
   * the value of 2^k (section 8.3 of 25.304)
   * Real DRX cycle length in TDD is calculated in
   * functions tdsrrcllc_get_drx_info() and
   * tdsrrcllc_get_drx_info_for_multiple_drx().
   * So these 3 fields are not to be used 
   * for direct DRX cycle length.
   */
  uint32                          cs_drx_cycle_length;
  uint32                          ps_drx_cycle_length;
  uint32                          utran_drx_cycle_length;

  tdsrrcllc_rb_mapping_info          cell_fach_rb_mapping_info;

  /* Indicates if Destination Primray Frequency is present in OTA msg*/
  boolean         p_freq_present;
  /* Destination Primray Frequency  */
  uint16          p_freq;
  /* Indicates if Destination second Frequency is present in OTA msg*/
  boolean         s_freq_present;
  /* Destination Working Frequency  */
  uint16          w_freq;
  /* Indicates if Destination cell_param_id is present in OTA msg */
  boolean         cpid_present;
  /* the parameter save cpid from OTA msg, just use when DCH for DPCH */
  uint8           cell_param_id;

  /* Maximum allowed Tx power in dBm (-50..33)
   * See 25.331 v9.1.0 10.3.6.39*/
  int8   max_tx_pwr;

 /* RRC will set the bit based on what type of call is active and 
     what type of call is being setup.
  bit7(MSB) bit6 bit5 .........bit0(LSB)
  bit0 ...................1 Voice call  
  bit1 ...................1 Release 99 PS data call
  bit2 ...................1 HS call 
  bit3-7 ...................reserved */
  uint8 call_type_mask;

  /* boolean flag to indicate that hard handover is in progress */
  boolean is_hho;
/* When is_hho is TRUE, hho_type is valid */
  uint8   hho_type;
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrcllc_oc_tfc_subset_list_type ul_tfc_subset_list;
  tdsrrcllc_srb5_info_type  srb5_info;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
   tdsrrc_rb_dl_am_rlc_size_info rb_am_rlc_size_ota[TDSUE_MAX_AM_ENTITY];
#ifdef FEATURE_TDSCDMA_REL9
#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
/* If OTA msg have this IE, set TRUE, other condition(include revert) set FALSE */
  boolean                        meas_occasion_info_incl;

/* cellDCHMeasOccasionInfo-TDD128 CellDCHMeasOccasionInfo-TDD128-r9 OPTIONAL */
  tdsl1_meas_occasion_info_struct_type    meas_occasion_info;
#endif
#endif
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  boolean   harq_present;
#endif

#ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
  /*Structure to maintain the rlc_OneSidedReEst per AM entity*/
  boolean rlc_one_sided_reest[TDSUE_MAX_AM_ENTITY];
#endif

} tdsordered_config_type;


typedef struct {
  /* MAC Uplink TFCS information */
  tdsmac_ul_tfcs_type                      ul_tfcs;
  tdsl1_ul_cctrch_cfg_struct_type    l1_ul_cctrch_parms;

  /* Semi-static parameters for Uplink Transport channels */
  tdsl1_ul_semi_static_parms_struct_type   semi_static_parms[TDSUE_MAX_TRCH];

    /* Map of Gain factor parameters for Uplink */
  tdsl1_ul_gain_factor_parms_struct_type   gain_factor_parms[TDSL1_UL_TFC_MAX];
  tdsrrc_PersistenceScalingFactorList  *persistence_scaling_factor_list;  
  tdsrrc_AC_To_ASC_MappingTable        *ac_to_asc_mapping_table; 
  tdsrrc_PRACH_RACH_Info_LCR_r4               *rach_info;
  tdsrrc_TransportChannelIdentity              transport_channel_id;
  tdsrrc_TransportFormatSet_LCR               *rach_tfs;  
  tdsrrc_PRACH_Partitioning_LCR_r4            *prach_partitioning;
#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrc_PRACH_RACH_Info_LCR_v770ext           *prach_RACH_Info_LCR;
#endif /*FEATURE_TDSCDMA_HSUPA*/
} tdsrrcllc_prach_sel_info_type;

typedef struct
{
  tdsrrcllc_toc_usage_e_type   toc_usage;
  tdsordered_config_type       *toc_ptr;
}tdsrrcllc_transition_config_type;

extern tdsrrc_state_e_type  tdsrrc_ordered_state;

/* This structure has all data to provide complete status of OC */
typedef struct
{
  /*This enum indicates the status of Ordered Config */
  tdsrrcllc_oc_set_status_e_type   set_status;
  /* This records which one of the RRC procedures last set the OC */
  tdsrrc_proc_e_type               set_by_proc;
  /* This records the current OC processing state if OC is set
   * Note that process_state is valid only when set_status != TDSOC_NOT_SET
   */
  tdsrrcllc_oc_process_state_e_type  process_state;
  /* This variable indicates that SMC needs to be notified when
   * a reconfiguration is completed.
   */
  boolean              notify_smc;

/* upmerge interface */
/* This variable indicates that a reconfiguration message is currently
 * being processed.
 */
  boolean reconfig_in_progress;
}tdsrrcllc_oc_complete_status_type;


#ifdef FEATURE_TDSCDMA_HSUPA

/* NOT USED CURRENTLY */

typedef struct e_dch_transmission_s
{
  boolean curr_value;                /* EDCH transmission current value */
  boolean next_value;                /* EDCH transmission value after processing OTA msg */
  boolean rnti_info_stored;          /* Indicates whether Primary or Secondary is present*/
  boolean edpcch_info_stored;        /* Indicates whether E-DPCCH info stored */
  boolean edpdch_info_stored;        /* Indicates whether E-DPDCH info stored */
  boolean serving_cell_info_stored;  /* Indicates whether serving cell info stored */
  boolean log_chan_to_edch;          /* Indicates whether logical channel to RB mapping exists */
  boolean e_mac_info_changed;        /* Indicates whether mac info is included in OTA msg */
  boolean e_l1_dl_info_changed;      /* Indicates whether L1 dl info is included in OTA msg */
  boolean e_l1_ul_info_changed;      /* Indicates whether L1 ul info is included in OTA msg */
}tdse_dch_transmission_type;
#endif /* FEATURE_TDSCDMA_HSUPA */

#define TDSMAX_HANGING_RB_MAPPING_INFO 5

/* This enum defines the RB-Mapping for the  type of Trch channel used */
typedef enum
{
  TDSINVALID_RB_MAPPING,
  TDSUL_DCH_RB_MAPPING,
  TDSDL_DCH_RB_MAPPING,
  TDSRACH_RB_MAPPING,
  TDSFACH_RB_MAPPING,
  TDSE_DCH_RB_MAPPING,
  TDSMAC_D_RB_MAPPING,
  TDSMAC_EHS_RB_MAPPING,
  TDSALL_RB_MAPPING  
} tdsrb_mapping_chan_enum_type;

/* RB-Mapping info for the UL DCH Trch channel */
typedef struct
{
  boolean valid; /* indicates if the database is valid */
  tr_ch_id_type tr_id; /* Trch channel id for the UL DCH for which the RB-Mapping is stored*/
  uint8 log_channel_id;/* Logical channel id received in the OTA message in RB-Mapping  */
  tdsrrcllcoc_rlc_size_list_type size_rest_info;/* RLC size info */
  uint8 mac_log_priority;/* MAC Priority received in the OTA message in RB-Mapping*/
}tdsul_rb_dch_mapping_info_type;

/* RB-Mapping info for the RACH Trch channel */
typedef struct
{
  boolean valid; /* indicates if the database is valid */
  tr_ch_id_type tr_id; /*none for now */
  uint8 log_channel_id;/* Logical channel id received in the OTA message in RB-Mapping  */
  tdsrrcllcoc_rlc_size_list_type size_rest_info;/* RLC size info */
  uint8 mac_log_priority;/* MAC Priority received in the OTA message in RB-Mapping*/
}tdsul_rb_rach_mapping_info_type;


#ifdef FEATURE_TDSCDMA_HSUPA
  /* RB-Mapping info for the HSUPA channel */
typedef struct
{
  boolean valid;/* indicates if the database is valid */
  uint8 mac_e_flow_id;
  uint8 mac_log_priority;/* MAC Priority received in the OTA message in RB-Mapping*/
   /* Mapping of rlc-pdu-size  to ddi. Valid number of fields in following array is
     equal to 'num_rlc_size_ddi_info' */
       /* logical channel identifier (1-15) - as received from NW */
  /* To be used while sending Scheduling Information to NW */
  uint8      log_channel_id;/* Logical channel id received in the OTA message in RB-Mapping  */
  boolean in_sched_info;
  uint8                              num_rlc_size_ddi_info; /* 0..32, 0 is valid only when action is NOOP or STOP */
  /* Mapping of rlc-pdu-size  to ddi. Valid number of fields in following array is
     equal to 'num_rlc_size_ddi_info' */
  tdsmac_e_rlc_size_ddi_info_s_type     rlc_size_ddi_info[MAX_RLC_PDU_SIZE_PER_LOGCHAN];  
}tdsul_rb_mac_e_mapping_info_type;
#endif /*FEATURE_TDSCDMA_HSUPA*/

typedef struct
{

  tdsul_rb_dch_mapping_info_type  ul_dch_info;
  tdsul_rb_rach_mapping_info_type  rach_info;
#ifdef FEATURE_TDSCDMA_HSUPA
  tdsul_rb_mac_e_mapping_info_type ul_mace;
#endif /*FEATURE_TDSCDMA_HSUPA*/

}tdsul_mapping_info_type;



typedef struct
{
  boolean valid;/* indicates if the database is valid */
  tr_ch_id_type tr_id; 
      /* MAC logical channel identifier (1-15) */
  /* Distinguishes dedicated logical channels when more than one */
  /* is mapped onto same transport channel */
  uint8               log_channel_id;/* Logical channel id received in the OTA message in RB-Mapping  */
   
}tdsdl_rb_dch_mapping_info_type;

typedef struct
{
  boolean valid;/* indicates if the database is valid */
  tr_ch_id_type fach_id;
      /* MAC logical channel identifier (1-15) */
  /* Distinguishes dedicated logical channels when more than one */
  /* is mapped onto same transport channel */
  uint8               log_channel_id;/* Logical channel id received in the OTA message in RB-Mapping  */

}tdsdl_rb_fach_mapping_info_type;

typedef struct
{
  boolean valid;/* indicates if the database is valid */
  uint8 mac_d_flow_id;
  /* MAC logical channel identifier (1-15) */
  /* Distinguishes dedicated logical channels when more than one */
  /* is mapped onto same transport channel */
  uint8               log_channel_id; /* Logical channel id received in the OTA message in RB-Mapping  */
    /* Radio Bearer Identifier (0-31) */

}tdsdl_rb_macd_mapping_info_type;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
typedef struct
{
  boolean  valid;/* indicates if the database is valid */
  uint8  queue_id; /* Queue id received in the OTA message in RB-Mapping  */
  uint8  log_channel_id; /* Logical channel id received in the OTA message in RB-Mapping  */

}tdsdl_rb_macehs_mapping_info_type;
#endif/*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
typedef struct
{
  tdsdl_rb_dch_mapping_info_type dch_map_info;
  tdsdl_rb_fach_mapping_info_type fach_map_info;
  tdsdl_rb_macd_mapping_info_type macd_map_info;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  tdsdl_rb_macehs_mapping_info_type macehs_map_info;
#endif/*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

}tdsdl_mapping_info_type;


typedef struct
{
  boolean in_use; /*TRUE for being in use and FASLE otherwise */
  tdsrrc_RB_Identity rb_id;/*rb id for which mapping is maintained */
  uecomdef_logch_mode_e_type    logch_mode;
  tdsul_mapping_info_type ul_mapping_info;
  tdsdl_mapping_info_type dl_mapping_info;
}tdsrb_mapping_info_type;


/* defines that max no of DCh channels required for one voice rab */
#define TDSMAX_DCH_VOICE_CHANNEL 3
#define TDSMIN_DCH_VOICE_CHANNEL 2
/*-------------------------------------------------------------------
STRUCTURE: tdsrrcllc_amr_mapping_info_type

This structure defines the uplink and downlink RLC Logical Channel IDs
mapping for the AMR Mode commands.
--------------------------------------------------------------------*/
typedef struct
{
  rlc_lc_id_type     ul_amr_a_id;
  rlc_lc_id_type     ul_amr_b_id;
  rlc_lc_id_type     ul_amr_c_id;
  rlc_lc_id_type     dl_amr_a_id;
  rlc_lc_id_type     dl_amr_b_id;
  rlc_lc_id_type     dl_amr_c_id;

  uint32  amr_mode;
  boolean scr_mode;
  uint32  mode_type;   /* contains the codec mode that TDS Voice Adapter needs to be configured with*/
  uint32  dl_amr_mode; /* contains DL AMR mode */
} tdsrrcllc_amr_mapping_info_type;

/*This stucture is used for internal use*/
/* This structure conatins the transport channel if, RB-ID , RLC -ID and the Transport block sizes
  * associated with each voice class.
  */
typedef struct
{
  uint8   transport_channel_index; 
  uint16  amr_class_size;
  /* Radio Bearer Identifier (0-31) */
  tdsrrc_RB_Identity                          rb_id;
  /* RLC logical channel buffer identifier (0-20) */
  rlc_lc_id_type                  rlc_id;
}tdsdch_id_mapping;

#ifdef FEATURE_TDSCDMA_MIMO

typedef enum
{
  TDSRRC_MIMO_INACTIVE,
  TDSRRC_MIMO_ACTIVE
}tdsrrc_mimo_status_enum_type;
#endif

/* Stores parameters required to setup HSDPA Action */
extern tdsrrc_hsdpa_msg_struct_type tdshsdpa_msg_params;

extern tdsrlc_size_per_rb_struct_type tdsrlc_size_per_rb;

extern tdsrlc_size_change_struct_type tdsrlc_size_change_in_progress;
extern tdsrb_mapping_info_type tdsrrc_ordered_hanging_rb_mapping[TDSMAX_HANGING_RB_MAPPING_INFO];
extern tdsrb_mapping_info_type tdsrrc_current_hanging_rb_mapping[TDSMAX_HANGING_RB_MAPPING_INFO];

  /*Variable to store the UTRAN DRX CYCLE INFO*/
  extern tdsutran_drx_cycle_type tdsutran_drx_cycle_info;
/*===================================================================
                        DATA DECLARATIONS
=====================================================================*/
extern  tdsordered_config_type  tdsconfig_data[2];

/********************************************************************
* TDSORDERED_CONFIG definition
*********************************************************************/
extern tdsordered_config_type *tdsordered_config_ptr;
extern tdsordered_config_type *tdscurrent_config_ptr;
extern tdsordered_config_type *tdsreselection_config_ptr;

extern tdsrrcllc_oc_complete_status_type tdsordered_config;

extern tdsrrcllc_config_e_type tdsconfig_ptr_in_use;
extern tdsrrcllc_transition_config_type  tdstransition_config;


/********************************************************************
* End TDSORDERED_CONFIG definition
*********************************************************************/

/********************************************************************
* Transport Channel Mapping info
*********************************************************************/
extern tdsrrcllcoc_trch_info_type dl_pch_trch_info;
extern tdsrrcllcoc_trch_info_type dl_fach_trch_info[TDSUE_MAX_TRCH];
extern tdsrrcllcoc_trch_info_type dl_dch_trch_info[TDSUE_MAX_TRCH];
extern tdsrrcllcoc_trch_info_type ul_rach_trch_info[TDSUE_MAX_TRCH];
extern tdsrrcllcoc_trch_info_type ul_dch_trch_info[TDSUE_MAX_TRCH];

/********************************************************************
* End Transport Channel Mapping info
*********************************************************************/

/********************************************************************
* Local TFS/Rate Matching Info for TFCS calc in RRCLLCOC
*********************************************************************/
/* Uplink */
extern uint16                                num_removed_tfcis;
/********************************************************************
* End Local TFS/Rate Matching Info for TFCS calc in RRCLLCOC
*********************************************************************/
/* This stores semi permamnent data for various Physical channels */
extern  tdsrrcllc_semi_permanent_data_type  tdsrrcllc_semi_permanent_data;

extern tdsrrcllc_rb_list_mapped_to_dch_type tdsrrc_rb_list_mapped_to_dch;


/* ESTABLISHED_RABS */
extern tdsrrc_established_rabs_type tdsrrc_est_rabs;

extern tdsrrcllc_prach_sel_info_type *  tdsprach_sel_info[UE_MAX_PRACH];


/*===================================================================
*                       FUNCTION PROTOTYPES
====================================================================*/
/*====================================================================
FUNCTION: tdsrrcllc_dequeue_cmd()

DESCRIPTION:
  This function dequeues commands fromLLC command queue
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.

=====================================================================*/
void tdsrrcllc_dequeue_cmd
(
  void
);
/*====================================================================
FUNCTION       tdsrrc_get_mac_d_hfn_cucnf()

DESCRIPTION    This function takes a pointer to the cu_confirm_ptr for R5
                         and extract mac_d_hfn field.

DEPENDENCIES   None

RETURN VALUE   value of mac_d_hfn if present else, invalid value

====================================================================*/
uint32 tdsrrc_get_mac_d_hfn_cucnf(tdsrrc_CellUpdateConfirm_r5_IEs *cu_confirm_ptr);

/*====================================================================
FUNCTION       get_mac_d_hfn()

DESCRIPTION    This function takes a pointer to the dl_dpch_info
                         and extract mac_d_hfn field.

DEPENDENCIES   None

RETURN VALUE   Success or failure if validation fails or succeds,
                        allocation of pointer is the responsibility of caller.

====================================================================*/

uecomdef_status_e_type  get_mac_d_hfn(tdsrrc_DL_DPCH_InfoCommon_r4 *dpch_ie_ptr, 
                                      uint32 *mac_d_val);



/*
* SCCPCH TrBlk delivery enable functions.
*/
/*====================================================================
FUNCTION: tdsrrcllc_set_fach_included()

DESCRIPTION:
  This function sets the fach_included flag in
  tdsrrcllc_semi_permanent_data to the boolean value as indicated
  by the passed parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
extern  void  tdsrrcllc_set_fach_included(boolean status);

/*====================================================================
FUNCTION: tdsrrcllc_set_pch_included()

DESCRIPTION:
  This function sets the pch_included flag in
  tdsrrcllc_semi_permanent_data to the boolean value as indicated
  by the passed parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
extern  void  tdsrrcllc_set_pch_included
(
  boolean status
);

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_set_ctch_included()

DESCRIPTION:
  This function sets the pch_included flag in
  rrcllc_semi_permanent_data to the rrcllc_ctch_status_e_type value as indicated
  by the passed parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_set_ctch_included
(
  tdsrrcllc_ctch_status_e_type status
);
/*====================================================================
FUNCTION: tdsrrcllc_set_pch_included_for_sccpch_with_ctch()

DESCRIPTION:
  This function sets the pch_included_for_sccpch_with_ctch flag in
  rrcllc_semi_permanent_data to the boolean value as indicated
  by the passed parameter.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_set_pch_included_for_sccpch_with_ctch
(
  boolean status
);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_rb_setup_ie_r4()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd
  in the Radio Bearer Setup Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_setup_ie_r4
(
  tdsrrc_RadioBearerSetup_r4 *msg_ptr
);




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
void tdsrrcllc_init_pdcp_parms(tdsordered_config_type *config_ptr);

#endif /* FEATURE_UMTS_PDCP*/

/*
* Ordered Config functions.
*/
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
);

void tdsrrcllc_init_ordered_config_mac_info(
  tdsrrcllc_config_e_type config_type
);

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
void tdsrrcllc_init_before_msg_processing (void);

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
);

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
);

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
uecomdef_status_e_type tdsrrcllc_copy_sib_to_oc(tdsrrc_state_e_type next_rrc_state);

/*============================================================================
FUNCTION: tdsrrcllc_update_oc_with_tfc_subset()

DESCRIPTION:
  This function takes UL common Transport channel
  List and updates OC with TFC subset restrictions.
      
DEPENDENCIES:
  This function assumes that mode specific info is of type FDD.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_with_tfc_subset
(
  void *ie_ptr, 
  tdsrrc_msg_ver_enum_type rel_ver
);


#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*============================================================================
FUNCTION: tdsrrcllc_update_oc_with_tfc_subset_list()

DESCRIPTION:
  This function takes UL common Transport channel
  List and maintains TFC subset list restrictions.
      
DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_with_tfc_subset_list(void  *ie_ptr);

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_with_srb5()

DESCRIPTION:
  This function updates tdsordered_config with SRB#5 information such as 
  RB ID & transport channel ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_with_srb5
(
  tdsrrc_SRB_InformationSetup_r5 * srb_setup_r5_ptr,
  tdsrrc_RB_Identity                  rb_id
);


/*====================================================================
FUNCTION: tdsrrcllc_update_oc_with_srb5_rel6_ie()

DESCRIPTION:
  This function updates tdsordered_config with SRB#5 information such as 
  RB ID & transport channel ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE if SRB#5 is found & OC is successful cases.
  FALSE otherwise

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_with_srb5_rel6_ie
(
  tdsrrc_SRB_InformationSetup_r6 * srb_setup_r6_ptr, 
  tdsrrc_RB_Identity rb_id
);

/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_dl_common_info_srb5

DESCRIPTION: This function updates the DL common info received in Handover to 
             Utran message and the SRB5 Default config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type 
tdsrrcllc_get_predefined_dl_common_info_srb5
(
  tdsrrc_DL_CommonInformation        *dl_common_info_ptr,
  tdsrrc_DL_CommonInformationPost    *dl_common_info_post_ptr,
  tdsrrc_DL_DPCH_InfoCommon_r4 *predef_phych,
  tdsrrc_U_RNTI_Short *new_urnti_ptr
);  

/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_ul_dpch_info_srb5

DESCRIPTION: This function updates the UL DPCH info received in Handover to 
             Utran message and the SRB5 Default config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type 
tdsrrcllc_get_predefined_ul_dpch_info_srb5
(
  tdsrrc_UL_DPCH_Info *ul_dpch_info_ptr,
  tdsrrc_UL_DPCH_InfoPostFDD *ul_dpch_info_post_ptr,
  tdsrrc_UL_DPCH_Info_r5 *predef_phych
) ;

#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: rrcllc_copy_sib_to_oc_for_sccpch_with_ctch()

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
rrcllc_copy_sib_to_oc_for_sccpch_with_ctch
(
  rrc_state_e_type  next_rrc_state
);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#endif

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
uecomdef_status_e_type
tdsrrcllc_update_dpl_from_sib7_info_in_fach
(
  tdsrrc_SysInfoType7    *sib7_ptr
);
#endif  /*FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH*/


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
);

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
uint32 tdsrrcllc_get_dl_um_ccch_rlc_size(void);

/*====================================================================
FUNCTION: tdsrrcllc_print_trans_chnl_rate()

DESCRIPTION:
  This is called to print the rate of each transport channel in CC and OC
  in order to see how rate has changed.

DEPENDENCIES:
  None.

RETURN VALUE:
  void.
====================================================================*/
void tdsrrcllc_print_trans_chnl_rate(void);

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_get_ctch_drx_info()

DESCRIPTION:
  This is called when DRX info for CTCH is required.

DEPENDENCIES:
  None.

RETURN VALUE:


SIDE EFFECTS:

====================================================================*/
void tdsrrcllc_get_ctch_drx_info
(
  uint8 *frame_offset,
  uint16 *alloc_period
);

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
uint32 tdsrrcllc_get_ctch_rlc_size(void);


/*====================================================================
FUNCTION: tdsrrcllc_get_ctch_enabled_status()

DESCRIPTION:
  Ckecks whether CTCH is enabled in mac info and returns boolean
  value based on that.
DEPENDENCIES:
  None.

RETURN VALUE:
  boolean

SIDE EFFECTS:
====================================================================*/
boolean tdsrrcllc_get_ctch_enabled_status(void);

#endif //FEATURE_TDSCDMA_SRB5_SUPPORT

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
);

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
);
/*====================================================================
FUNCTION: tdsrrcllc_get_dl_ded_rate()

DESCRIPTION:
  This is called when dl rate for a particular rb is needed

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The downlink rate of the rb passed in.

SIDE EFFECTS:
  None
===================================================================*/
uint32 tdsrrcllc_get_dl_ded_rate
(
  tdsrrc_state_e_type  rrc_state,
  tdsrrc_RB_Identity  rb_id
);
/*====================================================================
FUNCTION: tdsrrcllc_get_ul_ded_rate()

DESCRIPTION:
  This is called when ul rate for a particular rb is needed

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
);

/*
* The following prototypes don't have the rrcllc prefix, since they
* are simply to copy IE's from messages into the L1, MAC and RLC
* data structures. These functions get IEs out of SIBs 5 and 6.
* They may call other IE processign functions listed below.
*/

/*
* Physical Channel IE processing functions
*/

/* Uplink */
/*====================================================================
FUNCTION       tdsrrcllc_get_prach_sys_info()

DESCRIPTION    This function takes a pointer to the PRACH system
               information list and copies all applicable IEs for
               MAC, RLC, and L1 into the TDSORDERED_CONFIG database
               (internal format).

DEPENDENCIES   None

RETURN VALUE   SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS   The current contents of TDSORDERED_CONFIG will be
               overwritten.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_prach_sys_info
(
  tdsordered_config_type *config_ptr,
  void                   *sib5_6ptr,
  uint8   sib6_prach_present
);

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_prach_rach_info()

DESCRIPITON     This function takes a pointer to the PRACH to RACH
                info IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS    The current Ordered Config data is overwritten.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_prach_rach_info
(
  tdsl1_ul_prach_cfg_struct_type * prach_cfg_ptr,
  tdsrrc_PRACH_RACH_Info_LCR_r4 *ie_lcr_ptr
);

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_prach_partitioning()

DESCRIPITON     This function takes a pointer to the PRACH Partitioning
                info IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS    The current Ordered Config data is overwritten.
                The num_of_asc is updated to reflect the number of
                asc's rcv'd over the air.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_prach_partitioning
(
  tdsl1_ul_asc_struct_type  *tdsasc_info,
  tdsl1_ul_prach_cfg_struct_type * prach_cfg_ptr,
  tdsrrc_PRACH_Partitioning_LCR_r4   *ie_ptr,
  uint8                    *num_of_asc
);

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_persist_scaling_factors()

DESCRIPITON     This function takes a pointer to the Persistence Scaling
                Factor info IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS    Dynamic Persistence Values from SIB 7 are then needed
                to correctly calculate the Persistence Value needed
                by MAC.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_persist_scaling_factors
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_PersistenceScalingFactorList  *ie_ptr
);

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_ac_to_asc_mapping()

DESCRIPITON     This function takes a pointer to the AC to ASC mapping
                info IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES    None

RETURN VALUE    SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS    Previous values in Ordered Config are overwritten.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ac_to_asc_mapping
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_AC_To_ASC_MappingTable        *ie_ptr
);

uecomdef_status_e_type tdsrrcllc_get_max_allow_ul_tx_pwr
(
  tdsrrc_MaxAllowedUL_TX_Power max_ul_tx_pwr
);
uecomdef_status_e_type tdsrrcllc_get_ul_dpch_info
(
  tdsrrc_UL_DPCH_Info *ie_ptr
);
uecomdef_status_e_type tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info
(
  tdsrrc_UL_DPCH_PowerControlInfo *ie_ptr
);
/* Downlink */

/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_r5()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r5
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r5 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);


/*====================================================================
FUNCTION       tdsrrcllc_verify_sccpch_exists()

DESCRIPTION    This function takes a pointer to the SCCPCH system
               information list and RRC state, checks
               to see if at least one SCCPCH exists that has a FACH
               (for RRC states CELL_FACH and CONNECTING) or a PCH
               (for RRC states DISCONNECTED, CELL_PCH or URA_PCH)
               mapped to it.

DEPENDENCIES   None

RETURN VALUE   TRUE: SCCPCH with right type of TrCH exists.
               FALSE: SCCPCH with right kind of TrCH does NOT exist.

SIDE EFFECTS   None
====================================================================*/
boolean tdsrrcllc_verify_sccpch_exists
(
  tdsrrc_SysInfoType6  *sib6_ptr,
  tdsrrc_state_e_type  next_rrc_state
);
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION       tdsrrcllc_get_sys_info_for_sccpch_with_ctch()

DESCRIPTION    This function takes a pointer to the SCCPCH system
               information list for SIB 5, determines the state, checks
               to see if SIB 6 is also available, and selects the
               applicable SCCPCH information to use, and copies all
               applicable IEs for MAC, RLC, and L1 into the
               ORDERED_CONFIG database (internal format).

               SIB 5 must be there or else the UE cannot get pages or
               enter connected mode. This SIB contains the Inormation
               necessary to setup the SCCPCH an PRACH physical channels,
               and the PCH, FACH and RACH transport channels.

               SIB 6 is optional, and if present is only used to get
               SCCPCH information while in connected mode, i.e.
               CELL_FACH, CELL_PCH and URA_PCH.

DEPENDENCIES   rrcllc_init_ordered_config should always be called
               before a call to this function.

RETURN VALUE   SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS   This function presumes that Ordered Config is initialized
               before a call to this function. It updates all ordered
               config structures based on this assumption.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_sys_info_for_sccpch_with_ctch
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_SCCPCH_SystemInformationList *sccpch_sys_info_ptr,
  tdsrrc_state_e_type  next_rrc_state
);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION       tdsrrcllc_get_sccpch_sys_info()

DESCRIPTION    This function takes a pointer to the SCCPCH system
               information list for SIB 5, determines the state, checks
               to see if SIB 6 is also available, and selects the
               applicable SCCPCH information to use, and copies all
               applicable IEs for MAC, RLC, and L1 into the
               TDSORDERED_CONFIG database (internal format).

               SIB 5 must be there or else the UE cannot get pages or
               enter connected mode. This SIB contains the Inormation
               necessary to setup the SCCPCH an PRACH physical channels,
               and the PCH, FACH and RACH transport channels.

               SIB 6 is optional, and if present is only used to get
               SCCPCH information while in connected mode, i.e.
               CELL_FACH, CELL_PCH and URA_PCH.

DEPENDENCIES   rrcllc_init_ordered_config should always be called
               before a call to this function.

RETURN VALUE   SUCCESS if everything is valid, FAILURE otherwise

SIDE EFFECTS   This function presumes that Ordered Config is initialized
               before a call to this function. It updates all ordered
               config structures based on this assumption.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_sccpch_sys_info
(
  tdsordered_config_type  *config_ptr,

  void           *sib5_6ptr,
  uint8           sib6_present,                       /* true means sib6 valid, false means sib5 valid  */
  tdsrrc_state_e_type  next_rrc_state
);

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_tfcs_and_fach_pch_info()

DESCRIPTION     This function will return the tfcs and fach_pch
                info list to use for the sccpch identitfied by the
                index.

RETURN VALUE    SUCCESS or FAILURE

====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_tfcs_and_fach_pch_info
(
  tdsrrc_SCCPCH_SystemInformationList                      sccpch_sys_info_ptr,
  tdsrrc_SCCPCH_SystemInformationList_LCR_r4_ext           sccpch_sys_lcr_info_ptr,
  tdsrrc_FACH_PCH_InformationList **fach_pch_infolist_to_use,
  tdsrrc_TFCS *tfcs_to_use,
  uint8 index
);

/*====================================================================
FUNCTION        tdsrrcllc_get_sccpch_index()

DESCRIPTION     This function will determine which SCCPCH to read from
                SIB 5, and returns this index.

DEPENDENCIES    The U-RNTI should have been read and filled in
                Ordered Config

RETURN VALUE    The index (starting at 1) of the SCCPCH to read. An
                index of zero indicates a problem reading the
                SCCPCH_SYS_INFO IE from Sib 5 or Sib 6.

SIDE EFFECTS    This function also sets the imsi_div_pch_cnt(IMSI/K)
                parameter for L1 to perform the DRX calculations.
====================================================================*/
int tdsrrcllc_get_sccpch_index
(
  tdsordered_config_type  *config_ptr,

  void                  *sib5_6ptr,
  uint8                  sib6_present,             /* true means sib6 valid, false means sib5 valid  */
  tdsrrc_state_e_type                next_rrc_state
);
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION        tdsrrcllcpcie_get_sccpch_index_for_sccpch_with_ctch()

DESCRIPTION     This function will determine which SCCPCH to read for
                ctch from SIB 5/6, and returns this index.

DEPENDENCIES
RETURN VALUE    The index (starting at 1) of the SCCPCH to read. An
                index of zero indicates a problem reading the
                SCCPCH_SYS_INFO IE from Sib 5 or Sib 6.

SIDE EFFECTS
====================================================================*/
uint8 tdsrrcllcpcie_get_sccpch_index_for_sccpch_with_ctch
(
  tdsrrc_SCCPCH_SystemInformationList *ie_ptr
);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT

uecomdef_status_e_type tdsrrcllcpcie_get_sccpch_info
(
  tdsrrc_SecondaryCCPCH_Info       *ie_ptr,

  tdsrrc_SecondaryCCPCH_Info_LCR_r4_ext       *ie_lcr_ptr,

  tdsl1_dl_phycfg_sccpch_parms_struct_type   *psccpch,
  tdsl1_dl_cctrchcfg_ctfc_info_struct_type   *ctfc_info,
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type *cctrch_info
);
/*====================================================================
FUNCTION        tdsrrcllc_copy_ul_tfcs_info_to_dl_tfcs()

DESCRIPTION     This function copied TFCS information from Uplink TFCS
                in Current Config to Downlink TFCS information in Ordered
                Config

DEPENDENCIES
RETURN VALUE    None

SIDE EFFECTS
====================================================================*/
void tdsrrcllc_copy_ul_tfcs_info_to_dl_tfcs
(
  void
);


uecomdef_status_e_type tdsrrcllcpcie_get_pich_info
(

  tdsrrc_PICH_Info_LCR_r4   *ie_lcr_ptr,

  tdsl1_dl_phycfg_pich_parms_struct_type *ppich
);

uecomdef_status_e_type tdsrrcllcpcie_get_dl_dpch_info_for_each_rl
(
  tdsrrc_DL_DPCH_InfoPerRL *ie_ptr,
  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan,
  uint8 rl_idx,
  boolean is_new_rl
);
uecomdef_status_e_type tdsrrcllc_get_dl_info_common_for_all_rl
(
  tdsrrc_DL_CommonInformation *ie_ptr,
  tdsl1_dl_phycfg_dpch_parms_struct_type *dl_common
);

tdsrrcllcoc_dl_info_per_rl_status_e_type tdsrrcllc_get_dl_info_for_each_rl
(
  tdsrrc_DL_InformationPerRL *ie_ptr,
  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan,
  uint8 rl_idx,
  boolean is_new_rl
);
void tdsrrcllc_update_rm_restriction_info
(
  tdsl1_dl_cctrchcfg_cctrch_info_struct_type *cctrch_info,
  tdsrrcllc_dl_ctfc_info_struct_type *dl_tfcs_info,
  tdsl1_dl_cctrchcfg_ctfc_info_struct_type *tfcs_info_rrc_l1_inf
);

void tdsrrcllc_init_dl_rl_info
(
  tdsordered_config_type *config_ptr,
  uint8 idx
);

/*
* Transport Channel IE processing functions
*/
/* Uplink */
uint8 tdsrrcllc_get_tfs_num_tb
(
  tdsrrc_NumberOfTransportBlocks *ie_ptr
);
uint32 tdsrrcllc_get_tfs_ded_rlc_size
(
  tdsrrc_DedicatedDynamicTF_Info *ie_ptr
);
uint32 tdsrrcllc_get_tfs_com_rlc_size
(
    tdsrrc_CommonDynamicTF_Info    *ie_ptr
);
uecomdef_status_e_type tdsrrcllc_get_ul_info_common_for_all_trch
(
  tdsrrc_UL_CommonTransChInfo *ie_ptr,
  tdsrrc_state_e_type  next_rrc_state
);
uecomdef_status_e_type tdsrrcllc_get_ul_added_reconfig_trch
(
  tdsrrc_UL_AddReconfTransChInformation * ie_ptr
);
uecomdef_status_e_type tdsrrcllc_get_ul_deleted_trch
(
  tdsrrc_UL_TransportChannelIdentity * ie_ptr
);

/*====================================================================
FUNCTION: FIND_TRCH_ID_IN_CONFIG_TRCH_LIST_R5

DESCRIPTION:

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_trch_id_in_config_trch_list_r5
(
  tdsrrc_DL_AddReconfTransChInfoList_r5 * dl_trch_add_reconfig_ptr,
  tr_ch_id_type tdstrch_id
);

/*====================================================================
FUNCTION: FIND_TRCH_ID_IN_CONFIG_TRCH_LIST_R4

DESCRIPTION:

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsfind_trch_id_in_config_trch_list_r4
(
  tdsrrc_DL_AddReconfTransChInfoList_r4 * dl_trch_add_reconfig_ptr,
  tr_ch_id_type tdstrch_id
);


/*================================================================================================
FUNCTION    :   tdsrrcllc_get_ul_transport_format_set()

DESCRIPTION :   Processes the TFS IE of a transport channel. If the IE contents are valid
              this function saves the information in MAC Ordered Configuration Database.

DEPENDENCIES:   None.

RETURN VALUE:   SUCCESS if the IE contents are valid, FAILURE otherwise

SIDE EFFECTS:   None.
===============================================================================================*/
uecomdef_status_e_type  tdsrrcllc_get_ul_transport_format_set
(
    tdsordered_config_type       *config_ptr,
    /* Transport Format Set IE Pointer */
    tdsrrc_TransportFormatSet    *ie_ptr,
    /* Transport Format Set LCR IE Pointer, for prach */
    tdsrrc_TransportFormatSet_LCR    *ie_lcr_ptr,

    /* Transport channel index (0 to 7) */
    uint8                     trch_idx
);

/*================================================================================================
FUNCTION: tdsrrcllc_get_ul_tfcs_info()

DESCRIPTION:
  This function takes a pointer to the FDD only portion of the
  Transport Format Set IE and copies it to TDSORDERED_CONFIG.

  NOTE: This is on a per Transport Channel basis, and there can only
        be 1 TTI type per Transport Channel.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS: None.
===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_tfcs_info
(
    tdsordered_config_type  *config_ptr,
    /* Transport Format Combination Set IE Pointer */
    tdsrrc_TFCS        *ie_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_get_ul_gain_factor_info()

DESCRIPTION:
  This function get the UL gain factor information. The gain factor
  information is of 2 types.
  Signalled Type
  Computed Type

  In the Signalled Mode the gain factor information for this TFC Id
  (passed as the parameter to function) can be assigned a reference
  TFC Id. Note that this Reference TFC Id is different from the real
  TFC Id.
  This reference TFC Id is used in Computed Gain factor type in which
  rather than giving all the gain factors as in signalled mode only
  reference TFC Id is given. In this case the UL is supposed to use the
  Computed Gain factors from the TFC Id that had the reference TFC Id
  as already indicated in signalled mode TFC Id.
  There can be maximum 4 Reference TFC Id

  Example:

  -----------------------------------------------------
  TFCI Id   Mode    Gain Factors     Reference TFC Id
                     Beta C  Beta D
  -----------------------------------------------------
     0      SIG        X0     Y0            -
     1      SIG        X1     Y1            -
     2      SIG        X2     Y2            0
     ........
     25     SIG        X25    Y25           1
     ........
     37     COMPUTED    -      -            0 // Use gain factor of TFC Id 2
     ........
     41     SIG        X41    Y41           2
     ........
     62     COMPUTED    -      -            1 // Use gain factor of TFC Id 25
     63     COMPUTED    -      -            2 // Use gain factor of TFC Id 41
  -----------------------------------------------------


  Reference TFC Id list (MAX 4 Rreference TFC Id)

  -----------------------------------
  Ref Id     Index Valid   UL TFC Id
  -----------------------------------
     0         TRUE           2
     1         TRUE           25
     2         TRUE           41
     3         FALSE          -
  -----------------------------------

DEPENDENCIES:
  Needs function get_ul_gain_factor_info_struct_ptr to return the
  valid structure pointers to store the UL power offset info.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_gain_factor_info
(
  /* the last ul tfc in this OTA msg */
  boolean  last_tfc_flg,
  /* PowerOffsetInformation present flag */
  unsigned  power_offset_present,
  tdsl1_ul_gain_factor_parms_struct_type   *gain_factor_parms,
  /* RRC Power offset inforamtion IE struct pointer */
  tdsrrc_PowerOffsetInformation *ie_ptr,
  /* This UL TFC Id */
  uint8 tfc_id
);

uecomdef_status_e_type tdsrrcllc_get_ul_semi_static_transport_format_info
(
  tdsrrc_SemistaticTF_Information        *ie_ptr,
  tdsl1_ul_semi_static_parms_struct_type *oc_ssp_ptr
);
void    tdsrrcllc_compute_ul_tfis_from_ctfc
(
    tdsmac_ul_tfcs_type    *tfcs_ptr
);
void tdsrrcllc_init_ul_tfi_table
(
  tdsordered_config_type *config_ptr
);
void tdsrrcllc_init_ul_tfs_table
(
  tdsordered_config_type *config_ptr,
  uint8 index
);
void tdsrrcllc_init_rlc_size_info
(
  tr_ch_id_type tdstrch_id
);
/* Downlink */
uecomdef_status_e_type tdsrrcllc_get_dl_semistatic_tf_info
(
  tdsrrc_SemistaticTF_Information *ie_ptr,
  tdsl1_dl_cctrchcfg_trch_info_struct_type  *dl_semistatic_tf_info
);

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_tfcs_explicit_configuration()

DESCRIPTION:
  This function get the CTFC information for the DL from the
  Explicit TFCS information IE in RRC. This function needs the
  DL CTFC information pointer passed as a paramater.
  The CTFC structure has array of CTFC entries. The elements of
  the array are expected to be valid CTFC entry structure pointers
  up to MAX TFC as defined by macro TDSUE_MAX_TFC. The caller of this
  function must initialize the CTFC entry array pointer with pointers
  to the valid structures.

  There are 4 type of CTFC explicit configurations:
  1. Complete reconfiguration
  2. Addtion of CTFC
  3. Removal of CTFC
  4. Replacement of CTFCs (Removal and addtion)

  In the addition configuration the least TFC index available (never
  used or freed in previous removal) is occupied.
  In the replace the TFCIs removed may not be equal to the TFCI to be
  added.

  References: 25.331 (2000-12) 10.3.5.13, 10.3.5.15, 10.3.5.16

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_tfcs_explicit_configuration
(
  tdsordered_config_type  *config_ptr,
  /* RRC TFCS explicit configuration IE structure pointer */
  tdsrrc_ExplicitTFCS_Configuration  *ie_ptr,
  /* DL CTFC structure pointer */
  tdsrrcllc_dl_ctfc_info_struct_type *dl_tfcs_info
);

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_tfcs_info()

DESCRIPTION:
  This function get the CTFC information for the DL from the
  Explicit TFCS information IE in RRC. This function needs the
  DL CTFC information pointer passed as a paramater. This parameter
  is passed to the further lower level functions.
  For this parameter requirement see function:
    tdsrrcllc_get_dl_tfcs_explicit_configuration

  References: 25.331 (2000-12) 10.3.5.20

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_tfcs_info
(
  tdsordered_config_type  *config_ptr,
  /* RRC TFCS configuration IE structure pointer */
  tdsrrc_TFCS                        *ie_ptr,
  /* DL CTFC structure pointer */
  tdsrrcllc_dl_ctfc_info_struct_type *dl_tfcs_info
);

uecomdef_status_e_type tdsrrcllc_get_dl_tfs_dedicated_trch
(
  tdsrrc_DedicatedTransChTFS     *ie_ptr,
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info,
  boolean                      multiple_logical_ch
);
uecomdef_status_e_type tdsrrcllc_get_dl_tfs_common_trch
(
  tdsrrc_CommonTransChTFS        *ie_ptr,
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info
);
uecomdef_status_e_type tdsrrcllc_get_dl_transport_format_set
(
  tdsrrc_TransportFormatSet      *ie_ptr,
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_ded_tfs_info,
  boolean                      multiple_logical_ch,
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_com_tfs_info
);
uecomdef_status_e_type tdsrrcllc_get_dl_info_common_for_all_trch
(
  tdsrrc_DL_CommonTransChInfo *ie_ptr,
  tdsrrc_UL_CommonTransChInfo *ul_ptr,
  tdsrrc_state_e_type  next_rrc_state
);
uecomdef_status_e_type tdsrrcllc_get_dl_added_reconfig_trch
(
  tdsrrc_DL_AddReconfTransChInformation * ie_ptr,
  tdsrrc_UL_AddReconfTransChInfoList * ul_ptr,
  boolean                         tm_signalling_allowed
);

uecomdef_status_e_type tdsrrcllc_get_dl_deleted_trch
(
  tdsrrc_DL_TransportChannelIdentity * ie_ptr
);
void tdsrrcllc_init_dl_tfs_table
(
  tdsordered_config_type  *config_ptr,
  uint8 index
);

/*
* Transport channel mapping prototypes.
*/

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
void tdsrrcllc_init_dl_pch_trch_idx(tdsordered_config_type* config_ptr);

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
  tr_ch_id_type transportChannelIdentity);
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_get_dl_pch_trch_idx_for_sccpch_with_ctch()

DESCRIPTION:
  This function manages DL PCH TrCH Indexes for the UE.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL PCH TrCH Index that can be used.

SIDE EFFECTS:
  None.
====================================================================*/
uint8 tdsrrcllc_get_dl_pch_trch_idx_for_sccpch_with_ctch(
  tdsordered_config_type  *config_ptr,
  tr_ch_id_type transportChannelIdentity);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
/* FACH */
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
void tdsrrcllc_init_dl_fach_trch_idx_info(tdsordered_config_type *config_ptr);

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
);
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_get_dl_fach_trch_idx_for_sccpch_with_ctch()

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
uint8 tdsrrcllc_get_dl_fach_trch_idx_for_sccpch_with_ctch(
  tdsordered_config_type  *config_ptr,
  tr_ch_id_type  trch_id,
  uint8 pch_trch_idx
);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
/* RACH */
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
void tdsrrcllc_init_ul_rach_trch_idx_info(tdsordered_config_type *config_ptr);

/*====================================================================
FUNCTION: tdsrrcllc_get_ul_rach_trch_idx()

DESCRIPTION:

DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:
====================================================================*/
uint8 tdsrrcllc_get_ul_rach_trch_idx_for_prach_db
(
  tdsprach_config_type *prach_config_ptr,
  tdsrrcllc_prach_sel_info_type * prach_sel_info_ptr,
  tr_ch_id_type  tdstrch_id
);

/*================================================================================================
FUNCTION    :   tdsrrcllc_get_ul_transport_format_set_for_prach_db

DESCRIPTION :   Processes the TFS IE of a transport channel. If the IE contents are valid
              this function saves the information in MAC Ordered Configuration Database.

DEPENDENCIES:   None.

RETURN VALUE:   SUCCESS if the IE contents are valid, FAILURE otherwise

SIDE EFFECTS:   None.
===============================================================================================*/
uecomdef_status_e_type  tdsrrcllc_get_ul_transport_format_set_for_prach_db
(
    tdsrrcllc_prach_sel_info_type * prach_sel_info_ptr, /* Transport Format Set IE Pointer */
    tdsrrc_TransportFormatSet_LCR  *ie_ptr, /* Transport channel index (0 to 7) */
    uint8                     trch_idx
);


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
);

/* DL DCH */
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
void tdsrrcllc_init_dl_dch_trch_idx_info(tdsordered_config_type *config_ptr);

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
uint8 tdsrrcllc_allocate_dl_dch_trch_idx(void);

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
uecomdef_status_e_type tdsrrcllc_deallocate_dl_dch_trch_idx(uint8 trch_idx);

/* UL DCH */
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
void tdsrrcllc_init_ul_dch_trch_idx_info(tdsordered_config_type *config_ptr);

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
uint8 tdsrrcllc_allocate_ul_dch_trch_idx(void);

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
uecomdef_status_e_type tdsrrcllc_deallocate_ul_dch_trch_idx(uint8 trch_idx);

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
void tdsrrcllc_init_rlc_parms(tdsordered_config_type  *config_ptr);

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
void  tdsrrcllc_init_rnti_info(tdsordered_config_type  *config_ptr);

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
void  tdsrrcllc_init_l1_dl_dpch_chan_info(tdsordered_config_type  *config_ptr);

#ifdef FEATURE_TDSCDMA_HSUPA

/*===========================================================================

FUNCTION          tdsrrcllcpcie_convert_asn1_oss_bitstring_to_uint8

DESCRIPTION       
  This function translates oss-bitstring to uint8 and retruned the computed uint8 value. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint8 tdsrrcllcpcie_convert_asn1_oss_bitstring_to_uint8
(
  ASN1BitStr32  *bitstring_ptr
);

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
);
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
);

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
);

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
void tdsrrcllc_init_mac_e_info(tdsordered_config_type *config_ptr);
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
);
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
);

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
);

/*====================================================================
FUNCTION: tdsrrcllc_init_l1_e_ul_info()

DESCRIPTION:
  This function initializes l1_e_ul_info variables
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
);
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
void tdsrrcllc_reset_mac_and_l1_e_info_on_llc_cfg_cnf(void);
/*====================================================================
FUNCTION: rrcllc_init_e_mac_dflow()

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
);
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
);
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
);

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
);

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
uint8  tdsrrcllc_allocate_e_mac_d_flow_index(void);


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
);

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
uecomdef_logch_mode_e_type tdsrrcllc_get_e_dch_log_chan_mode(uint32 rb_id);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_e_dch_information()

DESCRIPTION:
  This function updates IE "ul edch informaion" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_e_dch_information
(
  uint32 e_dch_info_present,
  tdsrrc_UL_EDCH_Information_r6 *e_dch_info_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*====================================================================
FUNCTION: tdsrrcllc_init_e_dch_info_before_msg_processing()

DESCRIPTION:
  This function resets needed E-DCH variables before processing any config message

DEPENDENCIES:
  Function is only called before processing the DL OTA Msg.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_e_dch_info_before_msg_processing
(
  tdsordered_config_type *config_ptr
);

#endif /* FEATURE_TDSCDMA_HSUPA*/

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
);



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
);


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
uint8  tdsrrcllc_allocate_dflow_index(void);

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
void tdsrrcllc_init_hsdpa_mac_info(tdsordered_config_type  *config_ptr);


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
);


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
);

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
);

/*====================================================================
FUNCTION: tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured()

DESCRIPTION:
  This function checks if HSDPA can be started/reconfigured by looking at the fact that
  at least one HS Queue, one DFLOW, a HS Radio Link, a PS RAB is mapped onto
  one DFLOW and H-RNTI is present

DEPENDENCIES:
  None.

RETURN VALUE:
  True/False.

SIDE EFFECTS:
  None.
====================================================================*/

boolean tdsrrcllc_check_if_hsdpa_can_be_started_reconfigured
(
  tdsordered_config_type *config_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_copy_harq_info()

DESCRIPTION:
  This function copies the HARQ information.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_copy_harq_info
(
  tdsordered_config_type *dest_config,
  tdsordered_config_type *src_config
);

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
  , uint8 multicarrier_flg,
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr
#endif
);

/*====================================================================
FUNCTION      tdsrrcllc_set_hsscch_info()

DESCRIPTION   This function sets the Channelization code required
              for HSPDSCH in L1 structure in OC.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_set_hsscch_info
(
  tdsrrc_HS_SCCH_Info* hsscch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  , uint8 multicarrier_flg
#endif
);


/*====================================================================
FUNCTION      tdsrrcllc_set_meas_feedback_info()

DESCRIPTION   This function sets the Meas Feedback Information in
              L1 structure in OC.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/

uecomdef_status_e_type tdsrrcllc_set_meas_feedback_info
(
  tdsrrc_Measurement_Feedback_Info* meas_feedback_ptr
);


/*================================================================================================
FUNCTION: tdsrrcllc_set_hsdpa_l1_queue_info()

DESCRIPTION:
  This function checks all the MAC D Flows in MAC structure and gets
  the index to MAC DFLOW and then sets the get queue info.

DEPENDENCIES:
  None.

RETURN VALUE:
  Success/Failure.

SIDE EFFECTS:

===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_set_hsdpa_l1_queue_info
(
  void
);



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
);

/*====================================================================
FUNCTION: tdsrrcllc_initialize_queue_info()

DESCRIPTION:
  This function deallocates a DFLOW Index specified by passed pointer
  so that it can be used Later

DEPENDENCIES:
  None.

RETURN VALUE:
  ue_comdef_status_e_type -> SUCCESS/FAILURE
SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_initialize_queue_info
(
  uint8 dflow_index
);


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
);


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
);


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
);

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
);

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
);


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
);

/*============================================================================
FUNCTION: tdsrrcllc_set_harq_information()

DESCRIPTION:
  This function sets the HARQ information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_set_harq_information
(
  tdsrrc_HARQ_Info* harq_info_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_get_hs_window_size()

DESCRIPTION:
  
DEPENDENCIES:
  Gets the window size associated with the queue.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  
====================================================================*/
/*declared as static in tdsrrctrchie.c*/
/*
uint8 tdsrrcllc_get_hs_window_size
(
  rrc_MAC_hs_WindowSize mac_hsWindowSize
);
*/

/*====================================================================
FUNCTION: tdsrrcllc_get_hs_release_timer()

DESCRIPTION:
  
DEPENDENCIES:
  Gets the Release Timer associated with the queue.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  
====================================================================*/
/*Became static and moved to tdsrrcllctrchie.c*/
/*
uint16 tdsrrcllc_get_hs_release_timer
(
  rrc_T1_ReleaseTimer reorderingReleaseTimer
);
*/


/*====================================================================
FUNCTION      tdsrrcllc_set_hspdsch_midamble_configuration_tdd128()

DESCRIPTION   This function get hspdsch_midamble_configuration info.

DEPENDENCIES  None

RETURN VALUE  void

SIDE EFFECTS  None.
====================================================================*/

void tdsrrcllc_set_hspdsch_midamble_configuration_tdd128
(
  tdsrrc_HS_PDSCH_Midamble_Configuration_TDD128 *midamble_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,uint8 multiCarrier_flg
#endif
);
/*====================================================================
FUNCTION      tdsrrcllc_check_if_hspdsch_midamble_changed()

DESCRIPTION   This function check hspdsch_midamble_configuration info.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/

boolean tdsrrcllc_check_if_hspdsch_midamble_changed
(
  tdsrrc_HS_PDSCH_Midamble_Configuration_TDD128 *midamble_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,uint8 multicarrier_flg
#endif
);

/*====================================================================
FUNCTION      tdsrrcllc_check_if_hsscch_changed_r5()

DESCRIPTION   This function checks if HS-SCCH Info has changed.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
boolean tdsrrcllc_check_if_hsscch_changed_r5
(
  tdsrrc_HS_SCCH_Info* scch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  , uint8 multicarrier_flg
#endif

);

/*====================================================================
FUNCTION      tdsrrcllc_check_if_hsscch_changed_r6()

DESCRIPTION   This function checks if HS-SCCH Info has changed.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
boolean tdsrrcllc_check_if_hsscch_changed_r6
(
  tdsrrc_HS_SCCH_Info_r6* scch_info_ptr
);

/*====================================================================
FUNCTION      tdsrrcllc_check_if_hsscch_changed_r7()

DESCRIPTION   This function checks if HS-SCCH Info has changed.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
boolean tdsrrcllc_check_if_hsscch_changed_r7
(
  tdsrrc_HS_SCCH_Info_r7* scch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  , uint8 multicarrier_flg
#endif
);

/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r4()

DESCRIPTION:
  This function takes up deleted and added DL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs. For HSDSCH, it adds, deletes the DFLOW and rearranges
  the DFLOW so that missing holes because of deleted DFLOWs are
  removed

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r4
(
  /* local DL TrCH Add Reconfig pointer */
  tdsrrc_DL_AddReconfTransChInfoList_r4 * dl_trch_add_reconfig_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList    * dl_trch_delete_ptr
);
/*====================================================================
FUNCTION: tdsrrcllc_process_non_critcal_trch_reconfig_ie_r4

DESCRIPTION:
  This function acts on the NON Critical IE for RB Reconfig R4

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
);
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
);
/*====================================================================
FUNCTION: tdsrrcllc_process_non_critcal_rb_reconfig_ie_r4

DESCRIPTION:
  This function acts on the NON Critical IE for RB Reconfig R4

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
);



/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r5()

DESCRIPTION:
  This function takes up deleted and added DL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs. For HSDSCH, it adds, deletes the DFLOW and rearranges
  the DFLOW so that missing holes because of deleted DFLOWs are
  removed

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r5
(
  /* local DL TrCH Add Reconfig pointer */
  tdsrrc_DL_AddReconfTransChInfoList_r5 * dl_trch_add_reconfig_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList_r5    * dl_trch_delete_ptr
);


/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_reconfig_r5()

DESCRIPTION : Get Radio Bearer Information to reconfig.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_reconfig_r5
(
  tdsrrc_RB_InformationReconfig_r5 *ie_ptr
);


/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_affected_r5()

DESCRIPTION : Get Radio Bearer Information to be affected.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_affected_r5
(
  tdsrrc_RB_InformationAffected_r5 *ie_ptr
);


/*================================================================================================
FUNCTION: tdsrrcllc_get_ul_info_common_for_all_trch_r5()

DESCRIPTION:
  This function takes a pointer to the Uplink Common Information for all
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS: None.
===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_info_common_for_all_trch_r5
(
  tdsrrc_UL_CommonTransChInfo_r4 *ie_ptr,
  tdsrrc_state_e_type  next_rrc_state
);


/*=============================================================================
FUNCTION: tdsrrcllc_get_dl_info_common_for_all_trch_r5()

DESCRIPTION:
  This function takes a pointer to the Uplink Common Information for all
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
===============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_common_for_all_trch_r5
(
  tdsrrc_DL_CommonTransChInfo_r4 *ie_ptr,
  tdsrrc_UL_CommonTransChInfo_r4 *ul_ptr,
  tdsrrc_state_e_type  next_rrc_state
);


/*================================================================================================
FUNCTION: tdsrrcllc_get_dl_added_reconfig_trch_r5()

DESCRIPTION:
  This function takes a pointer to the Uplink Added or Reconfigured
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  CELL_DCH additions are the only item supported now.
===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_added_reconfig_trch_r5
(
  tdsrrc_DL_AddReconfTransChInformation_r5 * ie_ptr,
  tdsrrc_UL_AddReconfTransChInfoList * ul_ptr,
  boolean                         tm_signalling_allowed
);

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
);

/*====================================================================
FUNCTION      tdsrrcllc_get_ul_dpch_info_r5()

DESCRIPTION   This function translates the Uplink DPCH info common for
              all RL (radio links) IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_dpch_info_r5
(
  tdsrrc_UL_DPCH_Info_r5 *ie_ptr
);


/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_common_for_all_rl_r5()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_common_for_all_rl_r5
(
  /* Pointer to the DL CommonInformation IE */
  tdsrrc_DL_CommonInformation_r5 *ie_ptr,

  /* Pointer to the desirec DL PhyChan common information structure */
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
);





/*===========================================================================
FUNCTION        tdsrrcllc_get_dl_info_per_rl_r5()

DESCRIPTION     This function validates and stores Downlink Information Per
                Radio Link for reconfiguration messages.

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise

SIDE EFFECTS    Ordered config is updated with the new RL info.
===========================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_per_rl_r5
(
  tdsrrc_DL_InformationPerRL_List_r5 *ie_ptr,
  tdsrrc_RRC_StateIndicator next_state
);


/*==========================================================
FUNCTION  : tdsrrcllc_get_srb_info_to_setup_r5()

DESCRIPTION : Get Signalling Radio Bearer Information to
                            setup.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_srb_info_to_setup_r5
(
  tdsrrc_SRB_InformationSetup_r5     *ie_ptr,
  tdsrrc_SRB_InformationSetupList_r5 * srb_setup_head_ptr,
  tdsrrc_RB_Identity                   rb_id
);


/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_setup_r5()

DESCRIPTION : Get Radio Bearer Information to setup.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_setup_r5
(
  tdsrrc_RB_InformationSetup_r5       *ie_ptr,
#ifdef FEATURE_UMTS_PDCP
tdsrrc_CN_DomainIdentity cn_Domain,
#endif /* FEATURE_UMTS_PDCP*/
  tdsrrc_RB_InformationSetupList_r5   * rb_setup_head_ptr
);




/*====================================================================
FUNCTION: tdsrrcllc_rb_reconfig_ie_r5()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd
  in the Radio Bearer Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_reconfig_ie_r5
(
  tdsrrc_RadioBearerReconfiguration_r5 *r5_msg_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg,
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr
#endif
);


/*====================================================================
FUNCTION: tdsrrcllc_rb_release_ie_r5()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd
  in the Release 5 Radio Bearer Release Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_release_ie_r5
(
  tdsrrc_RadioBearerRelease_r5 *msg_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg,
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr
#endif
);


/*====================================================================
FUNCTION: tdsrrcllc_rb_setup_ie_r5()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd
  in the Radio Bearer Setup Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_setup_ie_r5
(
  tdsrrc_RadioBearerSetup_r5 *msg_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg,
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr
#endif
);

/*====================================================================
FUNCTION: tdsrrcllc_rrc_conn_setup_ie_r5()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd
  in R5 RRC Connection Setup Message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rrc_conn_setup_ie_r5
(
  tdsrrc_RRCConnectionSetup_r5 *msg_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_trch_reconfig_ie_r5()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd
  in the Release 5 Transport Channel Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_trch_reconfig_ie_r5
(
  tdsrrc_TransportChannelReconfiguration_r5 *msg_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg,
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr
#endif
);

/*====================================================================
FUNCTION: tdsrrcllc_phychan_reconfig_ie_r5()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd
  in the Physical Channel Reconfiguration Message for Rel 5

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_phychan_reconfig_ie_r5
(
  tdsrrc_PhysicalChannelReconfiguration_r5 *msg_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg,
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr
#endif
);

/*====================================================================
FUNCTION: tdsrrcllc_cell_update_confirm_ie_r5()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd
  in the Cell Update Confirm Message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_cell_update_confirm_ie_r5
(
  tdsrrc_CellUpdateConfirm_r5_IEs *msg_ptr,
  boolean    is_ccch_flag,
  tdsrrc_CellUpdateConfirm_CCCH_r5_add_ext_IEs *cellUpdateConfirm_CCCH_r5_add_ext,
  tdsrrc_CellUpdateConfirm_r5_add_ext_IEs *cellUpdateConfirm_r5_add_ext
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg,
  tdsrrc_DL_MultiCarrier_Information *multiCarrier_info_ptr
#endif
);

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
);

/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info2_r5()

DESCRIPTION:
  This function takes up deleted and added DL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs.
  NOTE: This function is similar to
  tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info except that this takes
  tdsrrc_DL_AddReconfTransChInfo2List as a parameter as opposed to
  tdsrrc_DL_AddReconfTransChInfoList. There are subtle differences between
  tdsrrc_DL_AddReconfTransChInfo2List and tdsrrc_DL_AddReconfTransChInfoList.

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info2_r5
(
  /* local DL TrCH Add Reconfig pointer */
  tdsrrc_DL_AddReconfTransChInfoList_r4 *  dl_trch_add_reconfig2_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList    * dl_trch_delete_ptr
);


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
);

/*================================================================================================
FUNCTION: tdsrrcllc_get_dl_added_reconfig_trch_2_r5()

DESCRIPTION:
  This function takes a pointer to the Rel 5 Uplink Added or Reconfigured
  Transport Channels IE and copies it to TDSORDERED_CONFIG. This function
  will not allow for the option of setting up a TM DCCH entity
  implicitly.
  NOTE: This function is similar to tdsrrcllc_get_dl_added_reconfig_trch except
  that this function takes tdsrrc_DL_AddReconfTransChInfo2List as
  argument as against tdsrrc_DL_AddReconfTransChInfoList taken by
  tdsrrcllc_get_dl_added_reconfig_trch. There are subtle differences between
  tdsrrc_DL_AddReconfTransChInfo2List and tdsrrc_DL_AddReconfTransChInfoList.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  CELL_DCH additions are the only item supported now.
===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_added_reconfig_trch_2_r5
(
  tdsrrc_DL_AddReconfTransChInformation_r4 * ie_ptr,
  tdsrrc_UL_AddReconfTransChInfoList  * ul_ptr
);




/*====================================================================
FUNCTION: tdsrrcllc_process_rb_info_release_list()

DESCRIPTION:
  This function the IE tdsrrc_RB_InformationReleaseList and updates the ordered
  config accordingly

REUSABILITY: R99/R5/R6

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rb_info_release_list 
(
  tdsrrc_RB_InformationReleaseList * rb_info_rel_list_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_rb_release_ie_r6()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Release 5 Radio Bearer Release Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_release_ie_r6
(
  tdsrrc_RadioBearerRelease_r6_IEs *msg_ptr

);

/*====================================================================
FUNCTION: tdsrrcllc_rb_setup_ie_r6()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Setup Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_setup_ie_r6
(
  tdsrrc_RadioBearerSetup_r6_IEs *msg_ptr

);
/*====================================================================
FUNCTION: tdsrrcllc_find_trch_id_in_ul_trch_list_r6

DESCRIPTION:
     The function is to find TRCH ID whether in current OTA message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_find_trch_id_in_ul_trch_list_r6
(
  tdsrrc_UL_AddReconfTransChInfoList_r6 * ul_trch_add_reconfig_ptr,
  tr_ch_id_type tdstrch_id
);

/*====================================================================
FUNCTION: tdsrrcllc_rb_reconfig_ie_r6()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_reconfig_ie_r6
(
  tdsrrc_RadioBearerReconfiguration_r6_IEs *msg_ptr

);
/*====================================================================
FUNCTION: tdsrrcllc_trch_reconfig_ie_r6()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Release 6 Transport Channel Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_trch_reconfig_ie_r6
(
  tdsrrc_TransportChannelReconfiguration_r6_IEs *msg_ptr

);
/*====================================================================
FUNCTION: tdsrrcllc_phychan_reconfig_ie_r6()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Physical Channel Reconfiguration Message for Rel 6

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_phychan_reconfig_ie_r6
(
  tdsrrc_PhysicalChannelReconfiguration_r6_IEs *msg_ptr

);
/*====================================================================
FUNCTION: tdsrrcllc_cell_update_confirm_ie_r6()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Cell Update Confirm Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_cell_update_confirm_ie_r6
(
  tdsrrc_CellUpdateConfirm_r6_IEs *msg_ptr

);
/*====================================================================
FUNCTION: tdsrrcllc_rrc_conn_setup_ie_r6()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in R5 RRC Connection Setup Message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rrc_conn_setup_ie_r6
(
  tdsrrc_RRCConnectionSetup_r6_IEs *msg_ptr

);

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
);

/*====================================================================
FUNCTION      tdsrrcllc_set_hsscch_info_r6()

DESCRIPTION   This function sets the Channelization code required
              for HSPDSCH in L1 structure in OC.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_set_hsscch_info_r6
(
  tdsrrc_HS_SCCH_Info_r6* hsscch_info_ptr
);

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
  boolean *cpid_change_flg   /* either new CELL PARAMETER ID is present and different with current */
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_dpch_info_r6()

DESCRIPTION:
  This function updates IE "ul dpch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_dpch_info_r6
(
  uint32 ul_dpch_info_present,
  tdsrrc_UL_DPCH_Info_r6 *ul_dpch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_validate_transition_to_cell_dch()

DESCRIPTION:
  This function validates whether DL common Info and DL per RL info is present or not,
  when UE is transitioned from non-cell DCH to cell-DCH in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_validate_transition_to_cell_dch
(
  uint32 ul_dpch_info_present,
  uint32 dl_common_info_present,
  uint32 dl_per_rl_info_list_present,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_common_info_r6()

DESCRIPTION:
  This function updates IE "dl common info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_common_info_r6
(
  uint32 dl_common_info_present,
  tdsrrc_DL_CommonInformation_r6 *dl_common_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_info_per_rl_list_r6()

DESCRIPTION:
  This function updates IE "dl per rl info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_info_per_rl_list_r6
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r6 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: rrcllcpcie_process_dl_hspdsch_info_and_update_hsdpa_msg_params()

DESCRIPTION:
  This function updates IE "dl hspdsch info " in ordered config and update
  hsdpa_msg_param variable
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_hspdsch_info_and_upd_hsdpa_msg_params
(
  uint32 dl_hspdsch_info_present,
  tdsrrc_DL_HSPDSCH_Information_r6 *dl_hspdsch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_6()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r6
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r6 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info_r6()

DESCRIPTION:
  This function takes up deleted and added UL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info_r6
(
  uint32 ul_add_recfg_trch_info_list_present,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r6  * ul_trch_add_reconfig_ptr,
  uint32 ul_deleted_trch_info_list_present,
  tdsrrc_UL_DeletedTransChInfoList_r6 * ul_deleted_trch_list_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_process_srb_info_setup_list_r6()

DESCRIPTION:
  This function process IE tdsrrc_SRB_InformationSetupList_r6 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_srb_info_setup_list_r6
(
  uint32 srb_info_setup_list_present,
  tdsrrc_SRB_InformationSetupList_r6 * src_info_setup_list_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_process_rab_info_setup_list_r6()

DESCRIPTION:
  This function process IE tdsrrc_RB_InformationAffectedList_r6 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rab_info_setup_list_r6
(
  uint32 rab_info_setup_list_present,
  tdsrrc_RAB_InformationSetupList_r6 *rab_info_setup_list_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_process_rb_info_affected_list_r6()

DESCRIPTION:
  This function process IE tdsrrc_RB_InformationAffectedList_r6 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rb_info_affected_list_r6
(
  uint32 rb_info_affected_list_present,
  tdsrrc_RB_InformationAffectedList_r6 * rb_info_affected_list_ptr
) ;

/*============================================================================
FUNCTION: tdsrrcllc_process_rb_info_reconfig_list_r6()

DESCRIPTION:
  This function process IE tdsrrc_RB_InformationReconfigList_r6 and store the 
  received information in OC

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rb_info_reconfig_list_r6
(
  uint32 rb_info_reconfig_list_present,
  tdsrrc_RB_InformationReconfigList_r6 * rb_info_reconfig_list_ptr
);
/*============================================================================
FUNCTION: tdsrrcllc_process_ul_common_trch_info_r5_r6()

DESCRIPTION:
  This function takes a pointer to the Uplink Common Information for all
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

REUSABILITY: R5/R6

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_ul_common_trch_info_r5_r6
(
  uint32 ul_common_trch_info_present,
  tdsrrc_UL_CommonTransChInfo_r4 *ie_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_store_ul_add_reconf_trch_info_list_r6()

DESCRIPTION:
  This function updates IE "ul added reconfig trch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_store_ul_add_reconf_trch_info_list_r6
(
  uint32 ul_add_recfg_trch_info_list_present,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r6 * ul_trch_add_reconfig_ptr  
);

/*============================================================================
FUNCTION: tdsrrcllc_store_dl_add_reconf_trch_info_list_r6()

DESCRIPTION:
  This function updates IE "dl added reconfig trch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_store_dl_add_reconf_trch_info_list_r6
(
  uint32 dl_add_reconf_trch_info_list_present,
  tdsrrc_DL_AddReconfTransChInfoList_r5 * dl_add_reconf_trch_info_list_ptr,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r6  * ul_add_reconf_trch_info_list_ptr  
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_srb_info_to_setup_r6()

DESCRIPTION : Get Signalling Radio Bearer Information to setup.

REUSABILITY: R6

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_srb_info_to_setup_r6
(
  tdsrrc_SRB_InformationSetup_r6     *ie_ptr,
  tdsrrc_SRB_InformationSetupList_r6 * srb_setup_head_ptr,
  tdsrrc_RB_Identity                   rb_id
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_reconfig_r6()

DESCRIPTION : Get Radio Bearer Information to reconfig.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_reconfig_r6
(
tdsrrc_RB_InformationReconfig_r6 *ie_ptr
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_affected_r6()

DESCRIPTION : Get Radio Bearer Information to be affected.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_affected_r6
(
tdsrrc_RB_InformationAffected_r6 *ie_ptr
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_setup_r6()

DESCRIPTION : Get Radio Bearer Information to setup.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_setup_r6
(
tdsrrc_RB_InformationSetup_r6       *ie_ptr,
#ifdef FEATURE_UMTS_PDCP
  tdsrrc_CN_DomainIdentity cn_Domain,
#endif /* FEATURE_UMTS_PDCP*/
tdsrrc_RB_InformationSetupList_r6   * rb_setup_head_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_rab_info

DESCRIPTION:

      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_predefined_rab_info
(
  tdsrrc_RAB_Info_Post rab_Info,
  tdsrrc_PredefinedRB_Configuration  *predef_rb, 
  struct tdsrrc_RAB_InformationSetupList *rab_setup_ptr
); 
/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_ul_dpch_info

DESCRIPTION:

      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_predefined_ul_dpch_info
(
  tdsrrc_UL_DPCH_Info *ul_dpch_info_ptr,
  tdsrrc_UL_DPCH_InfoPostFDD *ul_dpch_info_post_ptr,
  tdsrrc_PreDefPhyChConfiguration *predef_phych
);
/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_dl_common_info

DESCRIPTION:

      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_predefined_dl_common_info
(
  tdsrrc_DL_CommonInformation        *dl_common_info_ptr,
  tdsrrc_DL_CommonInformationPost    *dl_common_info_post_ptr,
  tdsrrc_PreDefPhyChConfiguration    *predef_phych,
  tdsrrc_U_RNTI_Short *new_urnti_ptr
);
/*============================================================================
FUNCTION: tdsrrcllc_get_predefined_dl_rl_list

DESCRIPTION:

      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_predefined_dl_rl_list
(
  struct tdsrrc_DL_InformationPerRL_List    **dl_rl_ptr,
  tdsrrc_DL_InformationPerRL_ListPostFDD  *dl_rl_post_ptr,
  tdsrrc_DL_CommonInformation        dl_common_info
);  
/*==========================================================
FUNCTION  : tdsrrcllc_get_srb_info_to_setup_preconfig_r5()

DESCRIPTION : Get Signalling Radio Bearer Information to 
              setup for Rb-Setup with preconfiguration info.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_srb_info_to_setup_preconfig_r5
(
  tdsrrc_SRB_InformationSetup_r5     *ie_ptr,
  tdsrrc_SRB_InformationSetupList_r5 * srb_setup_head_ptr,
  tdsrrc_RB_Identity                   rb_id
);


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
void  tdsrrcllc_init_l1_sccpch_chan_info(tdsordered_config_type  *config_ptr);

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
void  tdsrrcllc_init_l1_sccpch_for_ctch_chan_info(tdsordered_config_type  *config_ptr);
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
void tdsrrcllc_init_bch_info(tdsordered_config_type *config_ptr);

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
void tdsrrcllc_init_pch_info(tdsordered_config_type *config_ptr);

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
void tdsrrcllc_init_fach_info(tdsordered_config_type *config_ptr, boolean init_rb_mapping_info);

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
void tdsrrcllc_init_dl_dch_info(tdsordered_config_type *config_ptr);

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
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
void tdsrrcllc_init_dl_cctrch_info_for_sccpch_with_ctch(tdsordered_config_type *config_ptr);
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
void tdsrrcllc_init_dl_ctfcs_for_sccpch_with_ctch(tdsordered_config_type *config_ptr);
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
void tdsrrcllc_init_dl_cctrch_info(tdsordered_config_type *config_ptr);

/*====================================================================
FUNCTION: tdsrrcllc_init_dl_ctfcs()

DESCRIPTION:
  This function initializes the Global Downlink TFI table. This is
  called whenever the DL CCTrCH type changes (i.e. a state change),
  or when a TrCH is deleted..

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dl_ctfcs(tdsordered_config_type *config_ptr);

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
void tdsrrcllc_l1_ul_dpch_chan_info(tdsordered_config_type  *config_ptr);

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
void tdsrrcllc_init_rach_info(tdsordered_config_type *config_ptr, boolean init_rb_map_info);

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
void tdsrrcllc_init_ul_dch_info(tdsordered_config_type *config_ptr);

/* TrCH Reconfig */
/*====================================================================
FUNCTION: tdsrrcllc_ul_trch_reconfig()

DESCRIPTION:
  This checks for UL TrCH reconfigurations, and then executes them.

DEPENDENCIES:
  This uses the Add Reconfig Info List structure.

RETURN VALUE:
  ue_comdef_status_e_type -> SUCCESS indicates either no reconfig,
                             or reconfig is successful
                             FAILURE indicates an unsuccessful reconfig

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_ul_trch_reconfig
(
  tdsrrc_UL_AddReconfTransChInfoList ie_ptr
);


/* TFS Ue Capability Check */
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
uecomdef_status_e_type tdsrrcllc_check_ul_tfs_cnt(void);

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
uecomdef_status_e_type tdsrrcllc_check_dl_tfs_cnt(void);

/*
* Radio Bearer IE processing functions
*/
uecomdef_status_e_type tdsrrcllc_get_srb_info_to_setup
(
  tdsrrc_SRB_InformationSetup   *ie_ptr,
  tdsrrc_SRB_InformationSetupList2 * srb_setup_head_ptr,
  tdsrrc_RB_Identity                  rb_id
);

uecomdef_status_e_type tdsrrcllc_get_rb_info_to_setup
(
  tdsrrc_RB_InformationSetup    *ie_ptr,
#ifdef FEATURE_UMTS_PDCP
  tdsrrc_CN_DomainIdentity cn_Domain,
#endif /* FEATURE_UMTS_PDCP*/
  tdsrrc_RB_InformationSetupList * rb_setup_head_ptr
);

uecomdef_status_e_type tdsrrcllc_get_rb_info_affected
(
  tdsrrc_RB_InformationAffected *ie_ptr
);
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_release
(
  tdsrrc_RB_Identity            *ie_ptr
);

void tdsrrcllc_init_fach_rb_mapping_info
(
  tdsordered_config_type  *config_ptr,
  uint8 index
);
void tdsrrcllc_init_dl_dch_rb_mapping_info
(
  tdsordered_config_type  *config_ptr,
  uint8 index
);
void tdsrrcllc_init_rach_rb_mapping_info
(
  tdsordered_config_type  *config_ptr
);
void tdsrrcllc_init_ul_dch_rb_mapping_info
(
  tdsordered_config_type  *config_ptr,
  uint8 index
);
void tdsrrcllc_unmap_rb
(
  tdsrrc_RB_Identity rb_id,
  tdsrrcllc_unmap_rb_e_type config_db
);
void tdsrrcllc_remove_dl_ded_logch_entry
(
  tdsmac_dl_ded_logch_config_type *ded_logch_array,
  uint8                         entry_to_remove
);
void tdsrrcllc_remove_ul_ded_logch_entry
(
  tdsmac_ul_ded_logch_config_type *ded_logch_array,
  uint8                         entry_to_remove
);
void tdsrrcllc_remove_rlc_size_list_entry
(
  tdsrrcllcoc_rlc_size_list_type *rlc_size_list_array,
  uint8                        entry_to_remove
);

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
);
/*====================================================================
FUNCTION        tdsrrcllcpcie_select_asc_from_ac()

DESCRIPITON     This function selects the ASC from the UE's access
                service class. It uses the AC to ASC mapping to come up
                with the chosen ASC.

DEPENDENCIES    AC to ASC mapping should be present.
                UE's Access class should be available.

RETURN VALUE    None

SIDE EFFECTS    None
====================================================================*/
void  tdsrrcllcpcie_select_asc_from_ac(tdsordered_config_type *config_ptr);

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_persist_scaling_default_factors()

DESCRIPITON     This function sets default values of persistence
                scaling factors for all ASCs.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
====================================================================*/
void  tdsrrcllcpcie_get_persist_scaling_default_factors(tdsordered_config_type *config_ptr);

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_dch_trch_idx()

DESCRIPTION:
  This function gets a DL DCH TrCH Index for a given TrCH ID. If an
  index already exists, it is returned. Otherwise a new one is
  allocates for this TrCH ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL DCH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL DCH TrCH Indexes are available.
  If a new TrCH index is allocated, TrCH count in MAC, L1 and local
  data is incremented.
====================================================================*/
uint8 tdsrrcllc_get_dl_dch_trch_idx(tr_ch_id_type  tdstrch_id);


/*====================================================================
FUNCTION: tdsrrcllc_get_ul_dch_trch_idx()

DESCRIPTION:
  This function gets a DL DCH TrCH Index for a given TrCH ID. If an
  index already exists, it is returned. Otherwise a new one is
  allocates for this TrCH ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A DL DCH TrCH Index that can be used.

SIDE EFFECTS:
  trch_idx = 0xFF -> indicates no DL DCH TrCH Indexes are available.
  If a new TrCH index is allocated, TrCH count in MAC, L1 and local
  data is incremented.
====================================================================*/
uint8 tdsrrcllc_get_ul_dch_trch_idx(tr_ch_id_type  tdstrch_id);


void  tdsrrcllc_fill_holes_in_dl_dch_trch_idx(void);

void tdsrrcllc_copy_dl_dch_trch_info_from_one_to_next_index(
      uint8 dest_trch_idx,
      uint8 src_trch_idx
);

uecomdef_status_e_type tdsrrcllc_delete_dl_dch_trch_index
(
  uint8   trch_idx
);

uint8  tdsrrcllc_find_dl_fach_trch_idx(void);
uint8  tdsrrcllc_find_dl_dch_trch_idx(tr_ch_id_type  tdstrch_id);

/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info()

DESCRIPTION:
  This function takes up deleted and added DL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs.

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info
(
  /* local DL TrCH Add Reconfig pointer */
  tdsrrc_DL_AddReconfTransChInfoList * dl_trch_add_reconfig_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList   * dl_trch_delete_ptr
);

void  tdsrrcllc_fill_holes_in_ul_dch_trch_idx(void);

/*====================================================================
FUNCTION: tdsrrcllc_check_if_rlc_mode_is_am()

DESCRIPTION:
  This function checks whether given rb_id is of AM RLC type or not.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - rb_id is of AM RLC type
  FALSE - rb_id isn't of AM RLC type

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_check_if_rlc_mode_is_am
(
  tdsrrc_RB_Identity rb_id
);

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
);
/*====================================================================
FUNCTION: tdsrrcllc_init_rb_list_mapped_to_dch()

DESCRIPTION:
  This function initializes global variables tdsrrc_rb_list_mapped_to_dch

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
);

/*====================================================================
FUNCTION: tdsrrcllc_fill_rb_list_mapped_to_dch()

DESCRIPTION:
  This function fills the given RB-id in the global structure 
  tdsrrc_rb_list_mapped_to_dch. It does so only if the given RB-id isn't 
  already present in tdsrrc_rb_list_mapped_to_dch.

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
);

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
  None.
====================================================================*/
void tdsrrcllc_get_rb_for_rb_list_mapped_to_dch
(
  tr_ch_id_type tdstrch_id, 
  tdsrrc_state_e_type next_state, 
  tdsrrc_direction_enum_type direction
);

/*====================================================================
FUNCTION: tdsrrcllc_get_am_rlc_size_for_rb()

DESCRIPTION:
  This function returns RLC PDU size for a given rb_id and direction.
  It used OC database for it.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The size of the RLC channel.

SIDE EFFECTS:
  None
===================================================================*/
uint32 tdsrrcllc_get_am_rlc_size_for_rb
(
  tdsrrc_RB_Identity  rb_id, 
  tdsrrc_direction_enum_type direction
);

void tdsrrcllc_copy_ul_dch_trch_info_from_one_to_next_index(
      uint8 dest_trch_idx,
      uint8 src_trch_idx
);

uecomdef_status_e_type tdsrrcllc_delete_ul_dch_trch_index
(
  uint8   trch_idx
);

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
                                   tr_ch_id_type  tdstrch_id);

uint8  tdsrrcllc_find_ul_rach_trch_idx(void);

/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info()

DESCRIPTION:
  This function takes up deleted and added UL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs.

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info
(
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList * ul_trch_add_reconfig_ptr,
  tdsrrc_UL_DeletedTransChInfoList   * ul_trch_delete_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_init_mac_dl_ptrs()

DESCRIPTION:
  This function is responsible for initializing pointers to FACH, DCH
  and DSCH info in MAC DL Parameter structure to the storage
  space for FACH, DCH and DSCH info respectively.
  NOTE: Once mac_dl_config_type is changed to have the data
  structures in place of pointers, this function will not be
  needed anymore.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_mac_dl_ptrs(tdsordered_config_type  *config_ptr);

/*====================================================================
FUNCTION: tdsrrcllc_init_mac_ul_ptrs()

DESCRIPTION:
  This function is responsible for initializing pointers to
  TFCS and CCTrCH configuration in MAC UL Parameter structure to the
  TFCS and CCTrCH Config info in tdsordered_config_type respectively.
  NOTE: Once mac_ul_config_type is changed to have the data
  structures in place of pointers, this function will not be
  needed anymore.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_mac_ul_ptrs(tdsordered_config_type  *config_ptr);

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
  l1_dl_ctfc_parms and tdsl1_dl_cctrchcfg_ctfc_entry_info_struct_type are changed
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
);

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
void  tdsrrcllc_init_l1_ul_ptrs(tdsordered_config_type  *config_ptr);

/*====================================================================
FUNCTION: tdsrrcllc_init_rlc_ptrs()

DESCRIPTION:
  This function is responsible for initializing pointers to
  AM Info in RLC AM parameter structure to the AM Configuration
  stored locally in tdsordered_config_type.

  NOTE: Once rlc_am_config_req_type is changed to have the data
  structures in place of pointers, this function will not be
  needed anymore.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_init_rlc_ptrs(tdsordered_config_type  *config_ptr);

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
void  tdsrrcllc_init_activation_time(tdsordered_config_type  *config_ptr);

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
void tdsrrcllc_init_reconfig_needed(tdsordered_config_type  *config_ptr);

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
  On successful updation, ordered_config_status is set to TRUE.

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
);

void  tdsrrcllc_update_oc_from_fach_to_disconnected(
  uint32            dl_sdu_num,
  void             *msg_ptr
);

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
);


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
  On successful updation, ordered_config_set is set to TRUE.

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
);


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
);

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_from_cell_pch_to_fach()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  transitioning from Cell_PCH to Cell_FACH state.
  This function reads the SIBs and updates OC with SIB info.
  The BCH info is preserved from the CC to OC.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_from_cell_pch_to_fach(void);


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
  uecomdef_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_from_dch_to_fach_with_sibs(void);

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_fach_with_toc()

DESCRIPTION:
  This function is responsible for updating OC with TOC parameters
  immediately after transitioning to CELL_FACH.
  The CC is sopied to OC and then OC is updated with RLC and RB Mapping
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
);


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
boolean tdsrrcllc_check_dcch_fach_mapping(tdsordered_config_type *config_ptr);

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
);

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
);

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
  On successful updation, ordered_config_set is set to TRUE.

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
);

/*===========================================================================
FUNCTION   tdsrrcllc_chan_config_dch_to_fach

DESCRIPTION
  This function is resposible for determining all actions on RLC, MAC
  and L1 when going from Cell_DCH to Cell_FACH state.
  Note: This function will only setup CCCH and the rest of RLC channels
  will be hooked to the pipes later, thru' explicit commands.
  This also expects a BCCH Drop request in Channel Config Request.

DEPENDENCIES
  None.

RETURN VALUE


SIDE EFFECTS
  None.
===========================================================================*/
uecomdef_status_e_type
tdsrrcllc_chan_config_dch_to_fach(tdsrrc_cmd_type *cmd_ptr);


/*===========================================================================
FUNCTION   tdsrrcllc_chan_config_dch_to_pch

DESCRIPTION
  This function is resposible for determining all actions on RLC, MAC
  and L1 when going from Cell_DCH to Cell_PCH or URA_PCH state.
  Note: This function will only setup CCCH and the rest of RLC channels
  will be hooked to the pipes later, thru' explicit commands.
  This also expects a BCCH Drop request in Channel Config Request.

DEPENDENCIES
  None.

RETURN VALUE


SIDE EFFECTS
  None.
===========================================================================*/
uecomdef_status_e_type
tdsrrcllc_chan_config_dch_to_pch(tdsrrc_cmd_type *cmd_ptr);


/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_fach()

DESCRIPTION:
  This function is responsible for updating the Ordered Config when
  in Cell_FACH state.
  It starts with copying Current Config to Ordered Config and then
  updating Ordered Config data with Message IEs.
  In the event of a failure, the Ordered Config data is
  reverted back to Current Config data and a NOT_SUPPORTED
  status is returned.
  On successful updation, ordered_config_set is set to TRUE.

DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type
tdsrrcllc_update_oc_in_fach(
  uint32            dl_sdu_num,
  void             *msg_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_update_oc_in_fach_with_sibs

DESCRIPTION:


DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_oc_status_e_type

SIDE EFFECTS:
  None.
====================================================================*/

uecomdef_status_e_type  tdsrrcllc_update_oc_in_fach_with_sibs
(
  tdsordered_config_type *config_ptr
);

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
  On successful updation, ordered_config_set is set to TRUE.

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
);

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
);

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
tdsrrcllc_update_oc_from_disconnected_to_connecting(void);

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
);
#endif /* FEATURE_INTERRAT_HANDOVER_G2TD */

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
);

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
);

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
);

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
void tdsrrcllc_copy_ccch_info_from_cc_to_oc(void);

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
);

/*==========================================================
FUNCTION  : get_lc_type_by_rb_id()

DESCRIPTION :   
  This function returns logical channel type based on RB id.

DEPENDENCIES: None.

RETURN VALUE: 
  Logical Channel Type.

SIDE EFFECTS: None.
============================================================*/
uecomdef_logchan_e_type tdsrrcllc_get_lc_type_by_um_rb_id
(
  tdsrrc_RB_Identity rb_id
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_1st_hole_for_rlc_ul_um

DESCRIPTION : 
  Find the first unused rlc_ul_um_parms block in OC.
  
DEPENDENCIES: 
  None.

RETURN VALUE:
  entity index

SIDE EFFECTS:
  None.
============================================================*/
uint8 tdsrrcllc_get_1st_hole_for_rlc_ul_um
(
  void
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_1st_hole_for_rlc_dl_um

DESCRIPTION : 
  Find the first unused rlc_dl_um_parms block in Config DB.
  
DEPENDENCIES: 
  None.

RETURN VALUE:
  entity index

SIDE EFFECTS:
  None.
============================================================*/
uint8 tdsrrcllc_get_1st_hole_for_rlc_dl_um
(
  tdsordered_config_type * config_ptr
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_dl_um_info ()

DESCRIPTION : 
  Processes RLC Info IE for DL UM

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rlc_dl_um_info
(
  tdsrrc_RB_Identity    rb_id,
  boolean       allow_new_channels,
  tdsordered_config_type * config_ptr
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_ul_um_info ()

DESCRIPTION : 
  Processes RLC Info IE for UL UM RBs.

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rlc_ul_um_info
(
  tdsrrc_RB_Identity    rb_id,
  boolean       allow_new_channels 
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_um_index()

DESCRIPTION : Returns the index of the UL UM channel config
              info associated with the given logical channel id,
              or TDSUE_MAX_UL_UM_CHANNEL if no entry exists.

DEPENDENCIES: None.

RETURN VALUE: See description.

SIDE EFFECTS: None.
============================================================*/
uint8 tdsrrcllc_get_ul_um_index
(
  tdsrrc_RB_Identity rb_id
);
/*==========================================================
FUNCTION  : tdsrrcllc_get_um_ul_index()

DESCRIPTION : Returns the index of the UL UM channel config
              info associated with the given logical channel id,
              or UE_MAX_UL_UM_CHANNEL if no entry exists.

DEPENDENCIES: None.

RETURN VALUE: See description.

SIDE EFFECTS: None.
============================================================*/
uint8 tdsrrcllc_get_um_ul_index
(
  tdsrrc_RB_Identity rb_id,
  tdsordered_config_type * config_ptr
);
/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_um_index()

DESCRIPTION : Returns the index of the DL UM channel config
              info associated with the given logical channel id,
              or TDSUE_MAX_DL_UM_CHANNEL if no entry exists.

DEPENDENCIES: None.

RETURN VALUE: See description.

SIDE EFFECTS: None.
============================================================*/
uint8 tdsrrcllc_get_dl_um_index
(
  tdsrrc_RB_Identity rb_id,
  tdsordered_config_type * config_ptr
);

/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_am_info()

DESCRIPTION : Deletes RLC AM info and re-arranges the RLC
              AM Info array to cover any holes.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_am_info(tdsrrc_RB_Identity rb_id);

/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_dl_tm_info()

DESCRIPTION : Decrements the count of DL TM logical channels.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_dl_tm_info(rlc_lc_id_type lc_id);

/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_dl_um_info()

DESCRIPTION : Decrements the count of DL UM logical channels.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_dl_um_info(rlc_lc_id_type lc_id);

/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_ul_tm_info()

DESCRIPTION : Decrements the count of UL TM logical channels.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_ul_tm_info(rlc_lc_id_type lc_id);

/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_ul_um_info()

DESCRIPTION : Decrements the count of UL UM logical channels.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_ul_um_info(rlc_lc_id_type lc_id);

/*============================================================================
FUNCTION: tdsrrcllc_copy_ul_dch_trch_info_from_buffer()

DESCRIPTION:
  This function copies all Uplink DCH transport channel related info from
  a local buffer to L1, MAC and local data structures indexed by the passed
  index.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
void  tdsrrcllc_copy_ul_dch_trch_info_from_buffer(uint32 trch_idx);

/*============================================================================
FUNCTION: tdsrrcllc_copy_ul_dch_trch_info_to_buffer()

DESCRIPTION:
  This function copies all Uplink DCH transport channel related info to
  a local buffer from L1, MAC and local data structures indexed by the passed
  index.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
void  tdsrrcllc_copy_ul_dch_trch_info_to_buffer(uint32 trch_idx);

/*============================================================================
FUNCTION: tdsrrcllc_copy_dl_dch_trch_info_from_buffer()

DESCRIPTION:
  This function copies all Downlink DCH transport channel related info from
  a local buffer to L1, MAC and local data structures indexed by the passed
  index.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
void  tdsrrcllc_copy_dl_dch_trch_info_from_buffer(uint32 trch_idx);

/*============================================================================
FUNCTION: tdsrrcllc_copy_dl_dch_trch_info_to_buffer()

DESCRIPTION:
  This function copies all Downlink DCH transport channel related info to
  a local buffer from L1, MAC and local data structures indexed by the passed
  index.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
void  tdsrrcllc_copy_dl_dch_trch_info_to_buffer(uint32 trch_idx);

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
);

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
);

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
);

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
);

/*====================================================================
FUNCTION: tdsrrcllc_populate_mac_rlc_size_restriction_info()

DESCRIPTION:
  This function populates the MAC ordered config structures with the
  RLC size restriction data already stored outside ordered config.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  Updates MAC data structures in ordered config.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_populate_mac_rlc_size_restriction_info
(
  tdsordered_config_type *config_ptr,
  tdsrrc_state_e_type state
);


/*====================================================================
FUNCTION: tdsrrcllc_init_dch_rlc_size_info()

DESCRIPTION:
  This function initializes the DCH RLC size restriction info for the
  given transport channel identity.  Any existing RLC size restriction
  information is cleared out.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dch_rlc_size_info(tr_ch_id_type tdstrch_id);

/*============================================================================
FUNCTION: rrcllc_register_for_reconfig_complete()

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
void tdsrrcllc_register_for_reconfig_cnf(void);

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
void tdsrrcllc_discard_oc(void);


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
  );



#if defined (FEATURE_INTERRAT_HANDOVER_G2TD) || defined (FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
/*====================================================================
FUNCTION: tdsrrcllc_handover_to_utran_ie()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd
  in the Handover to UTRAN Message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_handover_to_utran_ie
(
  tdsrrc_HandoverToUTRANCommand *msg_ptr
);

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
tdsrrcllc_get_timers_and_const_in_conn_mode_for_handover(void);

#endif

/*====================================================================
FUNCTION: tdsrrcllc_clear_ordered_config_now

DESCRIPTION:
  This is called when the TDSORDERED_CONFIG variable needs to be cleared.
  That means the Ordered Config and Current Config pointers are swapped.

DEPENDENCIES:
  Make sure no command is under process when this is called.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_clear_ordered_config_now
(
  void
);

boolean  tdsrrcllc_is_cmd_under_process_for_solo_bch_operation(void);

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
void  tdsrrcllc_copy_bch_info_from_cc_to_oc(void);




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
);

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
tdsrrcllc_update_oc_from_dch_to_pch_with_sibs
(
  tdsrrc_state_e_type next_rrc_state
);

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
tdsrrcllc_update_oc_from_fach_to_cell_pch_or_ura_pch
(
  tdsrrc_state_e_type next_rrc_state
);

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
);

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
void tdsrrcllc_update_oc_for_pch_to_fach_with_toc
(
  void
);

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
uecomdef_status_e_type tdsrrcllc_update_oc_from_pch_to_fach
(
  void
);

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
void tdsrrcllc_update_oc_in_pch_with_toc
(
  void
);





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
void tdsrrcllc_init_dl_common_info(tdsordered_config_type *config_ptr);

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
boolean tdsrrcllc_timing_init_in_progress(void);

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
void tdsrrcllc_print_pdcp_info(tdsordered_config_type *config_ptr);
#endif /* FEATURE_UMTS_PDCP */

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
void tdsrrcllc_print_mac_info(tdsordered_config_type *config_ptr);

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
void tdsrrcllc_print_l1_info(tdsordered_config_type *config_ptr);


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
void tdsrrcllc_copy_pdcp_parms(
  tdsordered_config_type  *dest_config,
  tdsordered_config_type  *src_config
  );

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
  );


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
  );

/******************************/
/* End of Function prototypes */
/******************************/

/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_for_each_rl_r5()

DESCRIPTION     This function translates the Downlink information for
                each radio link IE to the RRC config database

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
tdsrrcllcoc_dl_info_per_rl_status_e_type tdsrrcllc_get_dl_info_for_each_rl_r5
(
  tdsrrc_DL_InformationPerRL_r5 *ie_ptr,

  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan
);
#ifdef FEATURE_UMTS_PDCP
/*==========================================================
FUNCTION  : tdsrrcllc_get_pdcp_info_later_than_r3()

DESCRIPTION : Get PDCP info IE

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_pdcp_info_later_than_r3
(
tdsrrcllcoc_pdcp_parms_type   *pdcp_parms_ptr,
tdsrrc_PDCP_Info_r4           *ie_ptr,
tdsrrc_RB_Identity                 rb_id,
tdsrrcllcoc_pdcp_action_etype pdcp_action
);
#endif
/*==========================================================
FUNCTION  : tdsrrc_get_am_index()

DESCRIPTION : Returns the index of the am_config info
              associated with the given RB id, or
              TDSUE_MAX_AM_ENTITY if no entry exists.

DEPENDENCIES: None.

RETURN VALUE: This function returns AM index in OC for a given
              rb_id.

SIDE EFFECTS: None.
============================================================*/
uint8 tdsrrc_get_am_index
(
  tdsrrc_RB_Identity rb_id
);

/*==========================================================
FUNCTION  : tdsrrc_get_am_index_in_toc()

DESCRIPTION : Returns the index of the am_config info
              associated with the given RB id, or
              TDSUE_MAX_AM_ENTITY if no entry exists.

DEPENDENCIES: None.

RETURN VALUE: See description.

SIDE EFFECTS: None.
============================================================*/
uint8 tdsrrc_get_am_index_in_toc
(
  tdsrrc_RB_Identity rb_id
);

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
uint32 tdsrrcllc_get_dl_amr_mode(uint32 tb_size);

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
);

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION        tdsrrcllc_init_srb5_info()

DESCRIPTION     This function initializes SRB#5 data structures 
                with default values.

DEPENDENCIES    NONE

RETURN VALUE    void

SIDE EFFECTS
====================================================================*/
void tdsrrcllc_init_srb5_info(tdsordered_config_type *config_ptr);

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
);

#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
/*====================================================================
FUNCTION: tdsrrcllc_get_rlc_size_change_status()

DESCRIPTION:
  This function checks if RLC Size Change is in progress from OC and returns
  TRUE or FALSE accordingly

DEPENDENCIES:
  None

RETURN VALUE:
  RLC Size

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_get_rlc_size_change_status(void);
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
);

/*====================================================================
FUNCTION: tdsrrcllc_compare_rlc_size_change()

DESCRIPTION:
  This function compares the new with old RLC Size

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE if RLC Sioze Changed

SIDE EFFECTS:
  None.
====================================================================*/

boolean tdsrrcllc_compare_rlc_size_change(uint32 old_size, uint32 new_size);


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
uint16 tdsrrcllc_get_dch_fach_act_time_in_ms(void);

/*==========================================================
FUNCTION  : tdsrrcllc_check_if_rbid_present_in_chan_config()

DESCRIPTION : This function checks if the RB-Id specified is already present in Chan_config

DEPENDENCIES: None.

RETURN VALUE: boolean: returns TRUE if Rb Id is presrn in  Channel Config Req. It also 
     returns the index at which the RB-Id is present in Chan Config
     else returns FALSE

SIDE EFFECTS: None
============================================================*/
uint8 tdsrrcllc_check_if_rbid_present_in_chan_config
(
  tdsrrc_channel_config_req_type    * chan_config_req, 
  tdsrrc_RB_Identity rb_id 
);



/*==========================================================
FUNCTION  : tdsrrcllc_check_ordered_current_mapping_same()

DESCRIPTION : This function checks if the RB Mapping is same in the current RB-Mapping 
 database, and the Ordered RB-Mapping database.

DEPENDENCIES: None.

RETURN VALUE: boolean: returns TRUE if  RB Mapping is same in the current RB-Mapping 
 database and the Ordered RB-Mapping database.
 else returns FALSE

SIDE EFFECTS: None
============================================================*/
boolean tdsrrcllc_check_ordered_current_mapping_same
(
  uint8 index
);
/*==========================================================
FUNCTION  : tdsrrcllc_init_ordered_hanging_rb_mapping_info()

DESCRIPTION : initialise the tdsrrc_ordered_hanging_rb_mapping info
DEPENDENCIES: None.

RETURN VALUE: None                        

SIDE EFFECTS: None
============================================================*/
extern void tdsrrcllc_init_ordered_hanging_rb_mapping_info
(
  void
 );

/*==========================================================
FUNCTION  : tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current()

DESCRIPTION : copies the tdsrrc_ordered_hanging_rb_mapping to tdsrrc_current_hanging_rb_mapping
DEPENDENCIES: None.

RETURN VALUE: None                        

SIDE EFFECTS: None
============================================================*/
 void tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current
 ( 
   void
 );

/*==========================================================
FUNCTION  : tdsrrcllc_find_free_hanging_rb_mapping_info()

DESCRIPTION : Finds a free slot in the tdsrrc_ordered_hanging_rb_mapping database

DEPENDENCIES: None.

RETURN VALUE: SUCCESS, if it succefully finds a free slot in the Backup database
                        In this case it also, return the index of the free slot in the database
                        else FAILURE is there is no more any free slot.                   

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type tdsrrcllc_find_free_hanging_rb_mapping_info
(
  uint8 *index
);
/*==========================================================
FUNCTION  : tdsrrcllc_free_hanging_rb_mapping_info_index()

DESCRIPTION : this function frees the rb_mapping info for the RB-ID passed in 
                        the Rb- mapping backup info

DEPENDENCIES: None.

RETURN VALUE: None       

SIDE EFFECTS: None
============================================================*/
void tdsrrcllc_free_hanging_rb_mapping_info_index
(
  tdsrrc_RB_Identity rb_id,
  tdsrb_mapping_chan_enum_type chan_type 
);
/*==========================================================
FUNCTION  : tdsrrcllc_check_rb_id_exist_rb_mapping()

DESCRIPTION : this function checks if the RB_Mapping info for the RB-ID passed is present in
                       the RB-Mapping Backup info.

DEPENDENCIES: None.

RETURN VALUE: Of type boolean . Returns true if the RB-ID exists in the RB-mappign Backup info
                       else, returns FALSE

SIDE EFFECTS: None
============================================================*/
 boolean tdsrrcllc_check_rb_id_exist_rb_mapping
 (
   tdsrrc_RB_Identity rb_id , 
   uint8 *index
 );
/*==========================================================
FUNCTION  : tdsrrcllc_backup_rbmapping_info()

DESCRIPTION : This function stores the Rb_mapping info for the transport channel id passed 
                         in the RB-mapping Backup info

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None
============================================================*/
 void tdsrrcllc_backup_rbmapping_info
 (
   uint8 trch_index,
   tdsrb_mapping_chan_enum_type chan_type 
 );

/*==========================================================
FUNCTION  :   tdsrrcllc_restore_ul_dch_rb_mapping_info()

DESCRIPTION : This function restores the Rb_mapping info from
              the RB-mapping backup info to the Ordered config
              for UL DCh.

DEPENDENCIES: None.

RETURN VALUE: FAILURE: if the Rb-mapping info in the Rb-Mapping 
              backup info is invalid
              else 
              returns  SUCCESS

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type tdsrrcllc_restore_ul_dch_rb_mapping_info
(
   uint8  index 
);

/*==========================================================
FUNCTION  :   tdsrrcllc_restore_dl_dch_rb_mapping_info()

DESCRIPTION : This function restores the Rb_mapping info from
              the RB-mapping backup info to the Ordered config
              for DL DCh.

DEPENDENCIES: None.

RETURN VALUE: FAILURE: if the Rb-mapping info in the Rb-Mapping 
              backup info is invalid
              else 
              returns  SUCCESS

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type tdsrrcllc_restore_dl_dch_rb_mapping_info
( 
  tr_ch_id_type index
);
/*==========================================================
FUNCTION  : tdsrrcllc_restore_macd_rb_mapping_info()

DESCRIPTION : This function restores the Rb_mapping info 
              from the RB-mapping backup info to the 
              Ordered config for MAC-D flow id.

DEPENDENCIES: None.

RETURN VALUE: FAILURE: if the Rb-mapping info in the Rb-Mapping backup info is invalid
              else 
              returns  SUCCESS

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type tdsrrcllc_restore_macd_rb_mapping_info
( 
  uint8 index
);

#ifdef FEATURE_TDSCDMA_HSUPA
/*==========================================================
FUNCTION  : tdsrrcllc_restore_ul_emac_rb_mapping_info()

DESCRIPTION : This function restores the Rb_mapping info from 
              the RB-mapping backup info to the Ordered config 
              for E-DCH MAC-d flowid.

DEPENDENCIES: None.

RETURN VALUE: FAILURE: if the Rb-mapping info in the Rb-Mapping 
                       backup info is invalid
              else 
              returns SUCCESS

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type tdsrrcllc_restore_ul_emac_rb_mapping_info
( 
  uint8 index
);
/*==========================================================
FUNCTION  : tdsrrcllc_backup_eul_from_ota()

DESCRIPTION : This function backs up the Rb-Mapping info 
              received in the OTA message for EULH. 
              This function is called when the OTA message 
              provides the RB-Mapping info, but the Trch 
              info mapped to that RB is not provided.

DEPENDENCIES: None.

RETURN VALUE: boolean

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type tdsrrcllc_backup_eul_from_ota
(
  tdsrrc_UL_LogicalChannelMapping_r6 *ie_ptr,  
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
);

/*===========================================================================

FUNCTION          tdsrrc_is_eul_active


DESCRIPTION       

  This function returns whether HSUPA / EUL is active or not at a given moment.
  for input, if OC is not set then use tdscurrent_config_ptr, else use tdsordered_config_ptr.

DEPENDENCIES


RETURN VALUE
                  
  TRUE/FALSE

SIDE EFFECTS
                  
  None

===========================================================================*/
boolean tdsrrc_is_eul_active
(
  void
);
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
);

#endif /*FEATURE_TDSCDMA_HSUPA*/
/*==========================================================
FUNCTION  : tdsrrcllc_backup_ul_dch_from_ota_r6()

DESCRIPTION : This function backs up the Rb-Mapping info 
              received in the REL6 OTA message for UL DCH.
              This function is called when the OTA message 
              provides the RB-Mapping info, 
              but the Trch info mapped to that RB is not provided.

DEPENDENCIES: None.

RETURN VALUE: boolean

SIDE EFFECTS: None
============================================================*/
 uecomdef_status_e_type tdsrrcllc_backup_ul_dch_from_ota_r6
(
  tdsrrc_UL_LogicalChannelMapping_r6 *ie_ptr,  
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
);
/*==========================================================
FUNCTION  : tdsrrcllc_backup_dl_dch_from_ota()

DESCRIPTION : This function backs up the Rb-Mapping info 
              received in the OTA message for DL DCH. 
              This function is called when the OTA message 
              provides the RB-Mapping info, but the Trch 
              info mapped to that RB is not provided.

DEPENDENCIES: None.

RETURN VALUE: boolean

SIDE EFFECTS: None
============================================================*/
 uecomdef_status_e_type tdsrrcllc_backup_dl_dch_from_ota
(
  tr_ch_id_type               tdstrch_id,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode,
  uint8 mac_id
);
/*==========================================================
FUNCTION  : tdsrrcllc_backup_ul_dch_from_ota()

DESCRIPTION : This function backs up the Rb-Mapping info received
              in the REL5/R99 OTA message for UL DCH. This function 
              is called when the OTA message provides the RB-Mapping 
              info, but the Trch info mapped to that RB is not provided.


DEPENDENCIES: None.

RETURN VALUE: boolean

SIDE EFFECTS: None
============================================================*/
 uecomdef_status_e_type tdsrrcllc_backup_ul_dch_from_ota
(
   tdsrrc_UL_LogicalChannelMapping *ie_ptr,  
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
);
/*====================================================================
FUNCTION: tdsrrcllc_init_tfcs_needed_flags()

DESCRIPTION:
  This functions initializes flags tdsul_tfcs_needed and tdsdl_tfcs_needed.

DEPENDENCIES:
  None

RETURN VALUE:
  uin16.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_tfcs_needed_flags(void);
/*====================================================================
FUNCTION: tdsrrcllc_is_tfcs_provided_on_add_delete_trch()

DESCRIPTION:
  This function returns TRUE if UE was expecting to receive some TFCS
  info, but never received it.  Else returns FALSE

DEPENDENCIES:
  None

RETURN VALUE:
  boolean

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_is_tfcs_provided_on_add_delete_trch(tdsrrc_RRC_StateIndicator dest_state);
/*==========================================================
FUNCTION  : tdsrrcllc_validate_rb_mapping_info()

DESCRIPTION : This functions validates the RB mapping information
              for R99, R5 and R6.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE.

SIDE EFFECTS: 
============================================================*/
uecomdef_status_e_type tdsrrcllc_validate_rb_mapping_info(void);

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
void tdsrrcllc_derive_dl_amr_mode
(
  tdsrrcllc_amr_mapping_info_type * amr_info
);

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
);

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
);


/*============================================================================
FUNCTION: tdsrrcllc_set_harq_information_r7()

DESCRIPTION:
  This function sets the HARQ information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_set_harq_information_r7
(
  tdsrrc_HARQ_Info_r7* harq_info_ptr
);

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
);


/*============================================================================
FUNCTION: tdsrrcllc_process_rb_info_reconfig_list_r7()

DESCRIPTION:
  This function process IE tdsrrc_RB_InformationReconfigList_r6 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rb_info_reconfig_list_r7
(
  uint32 rb_info_reconfig_list_present,
  tdsrrc_RB_InformationReconfigList_r7 * rb_info_reconfig_list_ptr
)  ;
/*============================================================================
FUNCTION: tdsrrcllc_process_rb_info_affected_list_r7()

DESCRIPTION:
  This function process IE tdsrrc_RB_InformationAffectedList_r7 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rb_info_affected_list_r7
(
  uint32 rb_info_affected_list_present,
  tdsrrc_RB_InformationAffectedList_r7 * rb_info_affected_list_ptr
)  ;
/*============================================================================
FUNCTION: tdsrrcllc_process_srb_info_setup_list_r7()

DESCRIPTION:
  This function process IE tdsrrc_SRB_InformationSetupList_r7 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_srb_info_setup_list_r7
(
  uint32 srb_info_setup_list_present,
  tdsrrc_SRB_InformationSetupList_r7 * src_info_setup_list_ptr
);
/*============================================================================
FUNCTION: tdsrrcllc_store_dl_add_reconf_trch_info_list_r6()

DESCRIPTION:
  This function updates IE "dl added reconfig trch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_store_dl_add_reconf_trch_info_list_r7
(
  uint32 dl_add_reconf_trch_info_list_present,
  tdsrrc_DL_AddReconfTransChInfoList_r7 *dl_add_reconf_trch_info_list_ptr,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r7  *ul_add_reconf_trch_info_list_ptr  
);
/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r7()

DESCRIPTION:
  This function takes up deleted and added DL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs. For HSDSCH, it adds, deletes the DFLOW and rearranges
  the DFLOW so that missing holes because of deleted DFLOWs are
  removed
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r7
(
  /* local DL TrCH Add Reconfig pointer */
  tdsrrc_DL_AddReconfTransChInfoList_r7 * dl_trch_add_reconfig_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList_r7    * dl_trch_delete_ptr
);
/*====================================================================
FUNCTION: FIND_TRCH_ID_IN_CONFIG_TRCH_LIST_R7

DESCRIPTION:

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_trch_id_in_config_trch_list_r7
(
  tdsrrc_DL_AddReconfTransChInfoList_r7 * dl_trch_add_reconfig_ptr,
  tr_ch_id_type tdstrch_id
);
/*====================================================================
FUNCTION: tdsrrcllc_rb_release_ie_r7()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Release 7 Radio Bearer Release Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_release_ie_r7
(
  tdsrrc_RadioBearerRelease_r7 * rb_release_r7_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_rb_setup_ie_r7()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Setup Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_setup_ie_r7
(
  tdsrrc_RadioBearerSetup_r7 * rb_setup_r7_ota
);
/*====================================================================
FUNCTION: tdsrrcllc_rb_reconfig_ie_r7()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_reconfig_ie_r7
(
  tdsrrc_RadioBearerReconfiguration_r7 * rb_reconfig_r7_ota
);
/*====================================================================
FUNCTION: tdsrrcllc_trch_reconfig_ie_r7()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Release 6 Transport Channel Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_trch_reconfig_ie_r7
(
  tdsrrc_TransportChannelReconfiguration_r7 * trch_reconfig_r7_ota
);
/*====================================================================
FUNCTION: tdsrrcllc_phychan_reconfig_ie_r6()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Physical Channel Reconfiguration Message for Rel 6

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_phychan_reconfig_ie_r7
(
 tdsrrc_PhysicalChannelReconfiguration_r7 * phys_chan_reconfig_r7_ota
);
/*====================================================================
FUNCTION: tdsrrcllc_cell_update_confirm_ie_r7()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Cell Update Confirm Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_cell_update_confirm_ie_r7
(
  uint32 dl_sdu_num,
  void * cell_update_r7_msg_ptr
);
/*====================================================================
FUNCTION: tdsrrcllc_rrc_conn_setup_ie_r7()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in R5 RRC Connection Setup Message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rrc_conn_setup_ie_r7
(
  tdsrrc_RRCConnectionSetup_r7 *rrc_conn_setup_r7_ota
);

/*============================================================================
FUNCTION: tdsrrcllc_store_ul_add_reconf_trch_info_list_r7()

DESCRIPTION:
  This function updates IE "ul added reconfig trch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_store_ul_add_reconf_trch_info_list_r7
(
  uint32 ul_add_recfg_trch_info_list_present,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r7 *  ul_trch_add_reconfig_ptr  
);
/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info_r7()

DESCRIPTION:
  This function takes up deleted and added UL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type 
tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info_r7
(
  uint32 ul_add_recfg_trch_info_list_present,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r7 * ul_trch_add_reconfig_ptr,
  uint32 ul_deleted_trch_info_list_present,
  tdsrrc_UL_DeletedTransChInfoList_r6 * ul_deleted_trch_list_ptr
);


#ifdef FEATURE_TDSCDMA_HSUPA
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_e_dch_information_r7()

DESCRIPTION:
  This function updates IE "ul e dch information" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_e_dch_information_r7
(
  uint32 e_dch_info_present,
  tdsrrc_UL_EDCH_Information_r7 *e_dch_info_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

#endif

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_dpch_info_r7()

DESCRIPTION:
  This function updates IE "ul dpch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_dpch_info_r7
(
  uint32 ul_dpch_info_present,
  tdsrrc_UL_DPCH_Info_r7 *ul_dpch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);
/*====================================================================
FUNCTION: tdsrrcllc_find_trch_id_in_ul_trch_list_r7

DESCRIPTION:
   The function is to find TRCH ID whether in current OTA message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_find_trch_id_in_ul_trch_list_r7
(
  tdsrrc_UL_AddReconfTransChInfoList_r7 * ul_trch_add_reconfig_ptr,
  tr_ch_id_type tdstrch_id
);


#ifdef FEATURE_TDSCDMA_HSUPA
/*============================================================================
FUNCTION: tdsrrcllc_ul_edch_information()

DESCRIPTION:
  This function sets UL E-DCH information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_ul_edch_information
(
  tdsrrc_UL_EDCH_Information_ext* edch_info_ptr
);



/*============================================================================
FUNCTION: tdsrrcllcpcie_e_rucch_asc_update()

DESCRIPTION:
  This function updates the E-RUCCH AccessServiceClass to be sent to MAC and L1.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_rucch_asc_update
(
  uint32         e_rucch_asc_present,
  tdsrrc_E_RUCCH_AccessServiceClass  *e_rucch_asc_ptr
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_get_erucch_pval()

DESCRIPTION:
  This function caulate Persistence values for all ASCs

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_get_erucch_pval(void);
/*============================================================================
FUNCTION: tdsrrcllcpcie_e_rucch_psf_update()

DESCRIPTION:
  This function updates the E-RUCCH PersistenceScalingFactorList to OC.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_rucch_psf_update
(
  tdsrrc_PersistenceScalingFactorList  *e_rucch_psf_ptr
);


/*============================================================================
FUNCTION: tdsrrcllcpcie_e_rucch_ul_syn_update()

DESCRIPTION:
  This function updates the E-RUCCH UL sync info to OC.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_rucch_ul_syn_update
(
  tdsrrc_SYNC_UL_InfoForE_RUCCH  *e_rucch_syn_ptr
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_e_rucch_prach_info_update()

DESCRIPTION:
  This function updates the E-RUCCH PRACH info to OC.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_rucch_prach_info_update
(
  tdsrrc_PRACH_Information_LCR_List  *e_rucch_prach_ptr
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_erucch_info()

DESCRIPTION:
  This function updates IE "E-RUCCH Info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type  tdsrrcllcpcie_process_e_rucch_info
(
  uint32 e_rucch_info_present,
  tdsrrc_E_RUCCH_Info *e_rucch_info_ptr
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_e_puch_tscfg_update()

DESCRIPTION:
  This function updates IE "E-RUCCH Info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_e_puch_tscfg_update
(
  tdsrrc_E_PUCH_Info_e_PUCH_TS_ConfigurationList_1 e_rucch_ts_cfg
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_e_puch_info()

DESCRIPTION:
  This function updates IE "E-PUCH Info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type  tdsrrcllcpcie_process_e_puch_info
(
  uint32 e_puch_info_present,
  tdsrrc_E_PUCH_Info *e_puch_info_ptr
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_non_sched_info()

DESCRIPTION:
  This function updates IE "Non-scheduled transmission grant info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type  tdsrrcllcpcie_process_non_sched_info
(
  uint32 e_non_sched_info_present,
  tdsrrc_Non_ScheduledTransGrantInfoTDD *e_non_si_ptr
);
#endif /*FEATURE_TDSCDMA_HSUPA*/


/*============================================================================
FUNCTION: rrcllc_process_rab_info_setup_list_r7()

DESCRIPTION:
  This function process IE tdsrrc_RB_InformationAffectedList_r6 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rab_info_setup_list_r7
(
  uint32 rab_info_setup_list_present,
  tdsrrc_RAB_InformationSetupList_r7 * rab_info_setup_list_ptr
);


/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_r7()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r7
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r7 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*====================================================================
FUNCTION: tdsrrcllc_process_get_cpid_r7

DESCRIPTION:
  This function get cpid present and cpid same as current of r4 msg

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
  boolean *cpid_change_flg   /* either new CELL PARAMETER ID is present and different with current */
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_tim_adv_contrl_r7()

DESCRIPTION   This function translates the Uplink Timing Advance Control
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.96 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_tim_adv_contrl_r7
(
  tdsrrc_UL_TimingAdvanceControl_r7 *ie_ptr
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r7()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r7
(
  tdsrrc_UL_TS_ChannelisationCodeList_r7 *src_ptr,
  tdsl1_ul_dpch_ts_struct_type *dst_cfg
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_r7()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.94a in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_r7
(
  tdsrrc_UL_CCTrCH_r7_tdd128 *ie_ptr,
  tdsl1_ul_dpch_cfg_struct_type *dpch_cfg
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_common_info_r7()

DESCRIPTION:
  This function updates IE "dl common info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_common_info_r7
(
  uint32 dl_common_info_present,
  tdsrrc_DL_CommonInformation_r7 *dl_common_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_info_per_rl_list_r7)

DESCRIPTION:
  This function updates IE "dl per rl info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_info_per_rl_list_r7
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r7 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: rrcllcpcie_process_dl_hspdsch_info_and_update_hsdpa_msg_params_r7()

DESCRIPTION:
  This function updates IE "dl hspdsch info " in ordered config and update
  hsdpa_msg_param variable
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_hspdsch_info_and_upd_hsdpa_msg_params_r7
(
  uint32 dl_hspdsch_info_present,
  tdsrrc_DL_HSPDSCH_Information_r7 *dl_hspdsch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_update_oc_with_srb5_rel7_ie()

DESCRIPTION:
  This function updates tdsordered_config with SRB#5 information such as 
  RB ID & transport channel ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE if SRB#5 is found & OC is successful cases.
  FALSE otherwise

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_with_srb5_rel7_ie
(
  tdsrrc_SRB_InformationSetup_r7 * srb_setup_r7_ptr, 
  tdsrrc_RB_Identity rb_id
);
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
/*============================================================================
FUNCTION: tdsrrcllcpcie_update_hs_tfri_table_type_non_r7_ota()

DESCRIPTION:
  This function updates the TRFI table to be used for MAC-HS or MAC-EHS for Non-R7 OTA
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_update_hs_tfri_table_type_non_r7_ota
(
  void
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_update_hs_tfri_table_type()

DESCRIPTION:
  This function updates the TRFI table to be used for MAC-HS or MAC-EHS
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_update_hs_tfri_table_type
(
  boolean hs_tb_size_octet_aligned
);
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
);
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
);
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
);
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
);
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
);
/*====================================================================
FUNCTION: tdsrrcllc_set_macehs_or_machs_and_check_if_coexist()

DESCRIPTION:
  
DEPENDENCIES:


RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  
====================================================================*/
boolean tdsrrcllc_set_macehs_or_machs_and_check_if_coexist
(
  void
);
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
);

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
);

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
);


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
);

#endif

#ifdef FEATURE_TDSCDMA_MIMO
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
);

/*============================================================================
FUNCTION: tdsrrcllc_get_mimo_action_in_config()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
tdsl1_dl_hs_mimo_action_enum_type 
  tdsrrcllc_get_mimo_action_in_config
(
  void
);

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
);
/*============================================================================
FUNCTION: tdsrrcllc_get_mimo_status()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
tdsrrc_mimo_status_enum_type tdsrrcllc_get_mimo_status
(
   void
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params()

DESCRIPTION:
  This function updates IE "Mimo Params" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type    tdsrrcllcpcie_process_mimo_params
(
  tdsrrc_MIMO_Parameters_r7 * mimo_params,
  tdsrrc_RRC_StateIndicator rrc_state,
  uint32 mimo_params_present_ota
);

#ifdef FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT
/*============================================================================
FUNCTION: tdsrrcllcpcie_update_mimo_scpich_po()

DESCRIPTION:
  This function updates IE "s-cpich-PowerOffset-Mimo" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_update_mimo_scpich_po
(
  tdsrrc_MIMO_Parameters_v7f0ext * mimo_params_ptr
);
#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT*/

#ifdef FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION
/*============================================================================
FUNCTION: tdsrrcllcpcie_update_mimo_codebook_restriction()

DESCRIPTION:
  This function updates IE "precodingWeightSetRestriction" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_update_mimo_codebook_restriction
(
  boolean val
);
#endif /*FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/

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
);
#endif

#if defined(FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT) || defined(FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION)
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_asu_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 ASU.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_asu_r7
(
  tdsrrc_ActiveSetUpdate_r7 * active_set_update_r7_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_asu_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 ASU.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_asu_r8
(
  tdsrrc_ActiveSetUpdate_r8 * active_set_update_r8_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_cu_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 CU.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_cu_r7
(
  uint32 dl_sdu_num,
  void * cell_update_r7_msg_ptr
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_cu_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 CU.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_cu_r8
(
  uint32 dl_sdu_num,
  void * cell_update_r8_msg_ptr
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_tcrc_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 TCRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_tcrc_r7
(
  tdsrrc_TransportChannelReconfiguration_r7 * trch_reconfig_r7_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_tcrc_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 TCRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_tcrc_r8
(
  tdsrrc_TransportChannelReconfiguration_r8 * trch_reconfig_r8_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbrc_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 RBRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbrc_r7
(
  tdsrrc_RadioBearerReconfiguration_r7 * rb_reconfig_r7_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbrc_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 RBRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbrc_r8
(
  tdsrrc_RadioBearerReconfiguration_r8 * rb_reconfig_r8_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbr_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 RBR.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbr_r7
(
  tdsrrc_RadioBearerRelease_r7 * rb_release_r7_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbr_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 RBR.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbr_r8
(
  tdsrrc_RadioBearerRelease_r8 * rb_release_r8_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbe_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 RBE.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbe_r7
(
  tdsrrc_RadioBearerSetup_r7 * rb_setup_r7_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_rbe_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 RBE.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_rbe_r8
(
  tdsrrc_RadioBearerSetup_r8 * rb_setup_r8_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_pcrc_r7()

DESCRIPTION:
  This function updates the addional MIMO params for R7 PCRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_pcrc_r7
(
  tdsrrc_PhysicalChannelReconfiguration_r7 * phys_chan_reconfig_r7_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_ext_pcrc_r8()

DESCRIPTION:
  This function updates the addional MIMO params for R8 PCRC.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllcpcie_process_mimo_params_ext_pcrc_r8
(
  tdsrrc_PhysicalChannelReconfiguration_r8 * phys_chan_reconfig_r8_ota
);

#endif /*FEATURE_TDSCDMA_SCPICH_MIMO_MODE_PRESENT || FEATURE_TDSCDMA_MIMO_CODEBOOK_RESTRICTION*/



/*===========================================================================

FUNCTION     tdsrrcllc_check_trch_mapped_to_srb_present

DESCRIPTION  This function checks if the Trch mapped to SRBs in 
               CELL DCH state exist
 
DEPENDENCIES
  None.

RETURN VALUE
 SUCCESS or FAILURE

===========================================================================*/
uecomdef_status_e_type tdsrrcllc_check_trch_mapped_to_srb_present
(
  void
);

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION tdsrrcllcoc_update_toc_with_oc_from_fach


DESCRIPTION This function updates the TOC with the OC value. 
                      It also sets the TOC.
 
DEPENDENCIES
  None.

RETURN VALUE
 None

===========================================================================*/
void  tdsrrcllcoc_update_toc_with_oc_from_fach
(
  void
);

#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

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
);

/*============================================================================
FUNCTION: tdsrrcllc_check_if_tfi_present_in_allowed_list_in_tfcc()

DESCRIPTION:
  This function checks if the TFI for a Trch is present
  in the allowed list list in Restricted transport channel list
  It takes a Transport channel index and  TFCI index and the 
  Restricted transport channel list from the TFCC message with which MAC UL is configured
DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS : If the TFI is present in the Allowed list  or the Trch ID
            itself was not present in the Restricted transport channel list
  else   FAILURE
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_check_if_tfi_present_in_allowed_list_in_tfcc
(
  tdsordered_config_type* config_ptr,
  tdsmac_tfc_subset_restriction_info_type* restricted_info,
  int tfci_index,
  int trch_index
);

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
);

/*===========================================================================

FUNCTION   RRCLLC_UPDATE_TFCC_RESTRICTION_IN_CC_OC

DESCRIPTION
  This function updates the TFCC restriction in RRC config db
  based on state.
  If RRC state is FACH, restriction info is updated in TOC
  If RRC state is DCH, restriction info is updated in CC
  If RRC state is DCH and OC is set for DCH and OC doesn't have the 
  new UL TFCS list from OTA, restriction info is also updated in OC
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/  
void tdsrrcllc_update_tfcc_restriction_in_cc_oc
(
  tdsmac_tfc_subset_restriction_info_type* restricted_info
);


/*====================================================================
FUNCTION: tdsrrcllc_get_cctrch_id()

DESCRIPTION:
  This function manages CCTrCH Id's for the UE.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint8 -> A CCTrCH ID that can be used.

SIDE EFFECTS:
  cctrch_id = 0xFF -> indicates no CCTrCHs are available.
====================================================================*/
uint8 tdsrrcllc_get_cctrch_id(void);

/*====================================================================
FUNCTION: tdsrrcllc_clear_cctrch_id()

DESCRIPTION:
  This function clears a CCTrCH Id for later UE use.

DEPENDENCIES:
  None.

RETURN VALUE:
  ue_comdef_status_e_type -> SUCCESS indicates a CCTrCH ID was cleared
                             FAILURE indicates a CCTrCH ID was not cleared

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_clear_cctrch_id(uint8 cctrch_id);

/*==========================================================
FUNCTION  :   RRCLLC_GET_CONFIG_PTR_IN_USE()

DESCRIPTION : This function checks

DEPENDENCIES: None.

RETURN VALUE: See description.

SIDE EFFECTS: None.
============================================================*/
tdsordered_config_type *tdsrrcllc_get_config_ptr_in_use(void);

/*==========================================================
FUNCTION  :   tdsrrcllc_update_l1_with_ul_trch()

DESCRIPTION : This function updates the L1 info in Config ptr with 
              UL Trch and UL TFCS info if UL DPDCH is present

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
void 
tdsrrcllc_update_l1_with_ul_trch
(
  tdsordered_config_type * config_ptr
);

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
tdsrrcllc_get_amr_mapping( tdsrrcllc_amr_mapping_info_type *ptr );


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

uecomdef_status_e_type tdsrrcllc_get_amr_wb_mapping
(
  tdsrrcllc_amr_mapping_info_type *ptr 
);

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
);


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
);

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
);

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
);

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
  rlc_lc_id_type ul_lc_id
);

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
);

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
);

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
);

/*===========================================================================
FUNCTION tdsrrcllc_update_delay_restriction
DESCRIPTION
This function updates the activation time based on delay restriction flag received in the OTA

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcllc_update_delay_restriction
(
  uint32 act_time_present, 
  uint32 delay_restriction_present,
  tdsrrc_ActivationTime act_time,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);


/*====================================================================
FUNCTION: tdsrrcllc_allocate_mem_for_prach_sel_info()

DESCRIPTION:
  This function initializes global tdsprach_sel_info dynamically/statically .

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
);

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
void tdsrrcllc_allocate_mem_for_prach_sel_info
(
  uint8 rrc_num_of_prach
);


/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_setup_r7()

DESCRIPTION : Get Radio Bearer Information to setup.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_setup_r7
(
  tdsrrc_RB_InformationSetup_r7       *ie_ptr,
#ifdef FEATURE_UMTS_PDCP
  tdsrrc_CN_DomainIdentity cn_Domain,
#endif /* FEATURE_UMTS_PDCP*/
  tdsrrc_RB_InformationSetupList_r7  * rb_setup_head_ptr
);
#ifdef FEATURE_TDSCDMA_HSUPA

#endif /*FEATURE_TDSCDMA_HSUPA*/
/*====================================================================
FUNCTION: tdsrrcllc_update_drx_coef_length()

DESCRIPTION:
  This function is called when T319 timer expires(or is stopped) and updates the UTRAN DRX Cycle Coef Length

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS : If process is successfull
  FAILURE : Otherwise
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcllc_update_drx_coef_length
(
  void
);
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
uint32 tdsrrcllc_get_drx_info_for_multiple_drx
(
  void
);


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

);

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
);

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
);


 void
tdsrrcllc_init_config_ptrs
(
  void
);

/*==========================================================
FUNCTION  : tdsrrcllc_ul_um_timer_discard()

DESCRIPTION : 
  SDU discard -- timer discard infomation interpretation.
  See 9.7.3.2 in 25.322 for the details of the bahaviour of this timer.
  See 10.3.4.25 in 25.331 for the details of the value (in ms)

DEPENDENCIES:
  None.

RETURN VALUE:
  Time, in MS, that should elapse before an SDU is discarded for RLC.

SIDE EFFECTS:
  None.
============================================================*/
uint16 tdsrrcllc_ul_um_timer_discard
(
  tdsrrc_NoExplicitDiscard noExplicitDiscard
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_um_timer_discard_for_rb_id()

DESCRIPTION : 
  This function returns UL UM SDU Discard Timer value for a given RB-Id.

DEPENDENCIES:
  None.

RETURN VALUE:
  Time, in MS, that should elapse before an SDU is discarded for RLC.

SIDE EFFECTS:
  None.
============================================================*/
uint16 tdsrrcllc_get_ul_um_timer_discard_for_rb_id
(
  tdsrrc_RB_Identity rb_id
);
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*============================================================================
FUNCTION: tdsrrcllc_shld_ctch_be_setup()

DESCRIPTION:
  This function indicates if the present cell support CTCH or not.
   CSP calls this function to find out if CTCH can be setup or not after BPLMN search is done
      
DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE/FASLE

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcllc_shld_ctch_be_setup
(
  void
);
#endif //FEATURE_TDSCDMA_SRB5_SUPPORT
#ifdef FEATURE_TDSCDMA_CPC_DTX
/*====================================================================
FUNCTION: tdsrrcllc_check_and_stop_dtx()

DESCRIPTION:

  This function will be called by all the pre-Rel7 messages which are capable of DTX operation 
  It will check if DTX operation is ongoing, 
    - if yes, then it will stop DTX 
    - otherwise, it will not do anything.
    
DEPENDENCIES:
  Function should be called only by the pre-Rel7 messages which are capable of DTX operation.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_check_and_stop_dtx
(
  rrc_RRC_StateIndicator rcvd_state_indicator,
  rrc_msg_ver_enum_type     rel_ver
);

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
);
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
);
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
);
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
void tdsrrcllc_update_l1_drx_req_mask_for_stop
(
 tdsordered_config_type *config_ptr,
  boolean drx_info_present
);
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_drx_req_mask_for_reconfig()

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
void tdsrrcllc_update_l1_drx_req_mask_for_reconfig
(
  tdsordered_config_type *config_ptr,
  boolean drx_info_present
);
/*============================================================================
FUNCTION: tdsrrcllc_update_l1_drx_req_mask_for_start()

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
void tdsrrcllc_update_l1_drx_req_mask_for_start
(
 tdsordered_config_type *config_ptr,
  boolean drx_info_present
);
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
void tdsrrcllc_reset_l1_cpc_dtx_info_on_llc_cfg_cnf
(
  void
);
/*====================================================================
FUNCTION: tdsrrcllc_init_cpc_dtx_info_before_msg_processing()

DESCRIPTION:
  This function resets needed CPC-DTX variables before processing any config message

DEPENDENCIES:
  Function is only called before processing the DL OTA Msg.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_cpc_dtx_info_before_msg_processing
(
  tdsordered_config_type *config_ptr
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_calculate_dpcch_slot_format()

DESCRIPTION:
  This function calculate dpcch_slot_format
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uint8  tdsrrcllcpcie_calculate_dpcch_slot_format
(
  void
);
/*====================================================================
FUNCTION: tdsrrcllc_init_dtx_params()

DESCRIPTION:
  This function evaluates the value of variable tdsrrcllc_is_dtx_params_variable_set.

DEPENDENCIES:
  Function is only called after processing of all the IEs in a config message is completed.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_dtx_params
(
  tdsordered_config_type *config_ptr  
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dtx_drx_params()

DESCRIPTION:
  This function processes 
  1) DTX-DRX timing info 
  2) dtx_drx_Info &
  3) evaluates DTX-DRX status in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type    tdsrrcllcpcie_process_dtx_drx_params
(
  tdsrrc_DTX_DRX_TimingInfo_r7 *dtx_drx_timingInfo,
  uint32 dtx_drx_timingInfo_present_ota,
  tdsrrc_DTX_DRX_Info_r7 *dtx_drx_Info,
  uint32 dtx_drx_Info_present_ota,
  tdsrrc_RRC_StateIndicator rrc_state
);
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
);
l1_ue_dtx_cycle1_info_enum_type  tdsrrcllcpcie_get_dtx_cycle1
(
  void    *ue_dtx_cycle1,
  tdsl1_e_tti_enum_type e_tti
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_dtx_cycle2()

DESCRIPTION:
  This function processes DTX info in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_ue_dtx_cycle2_info_enum_type  tdsrrcllcpcie_get_dtx_cycle2
(
  void    *ue_dtx_cycle2,
  tdsl1_e_tti_enum_type e_tti
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_mac_dtx_cycle()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_mac_dtx_cycle_info_enum_type tdsrrcllcpcie_get_mac_dtx_cycle
(
  void    *mac_dtx_cycle,
  tdsl1_e_tti_enum_type e_tti
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_ue_dpcch_burst_1_info()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_ue_dpcch_burst_1_info_enum_type tdsrrcllcpcie_get_ue_dpcch_burst_1_info
(
    tdsrrc_UE_DPCCH_Burst *ue_dpcch_burst_1_info
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_ue_dpcch_burst_2_info()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_ue_dpcch_burst_2_info_enum_type tdsrrcllcpcie_get_ue_dpcch_burst_2_info
(
    tdsrrc_UE_DPCCH_Burst *ue_dpcch_Burst2
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_ue_dtx_long_preamble_length()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_dtx_long_preamble_length_info_enum_type tdsrrcllcpcie_get_ue_dtx_long_preamble_length
(
  tdsrrc_UE_DTX_long_preamble_length *ue_dtx_long_preamble_length
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_get_inact_thrshd_dtx_cycle2()

DESCRIPTION:
  This function converts MAC DTX info to internal form 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
l1_inactivity_threshold_for_ue_dtx_cycle2_info_enum_type tdsrrcllcpcie_get_inact_thrshd_dtx_cycle2
(
  tdsrrc_UE_DTX_Cycle2InactivityThreshold *ue_dtx_cycle2_inactiv_thrshd
);

/*====================================================================
FUNCTION: tdsrrcllc_evaluate_dtx_drx_status_var_and_set_lower_layers_action()

DESCRIPTION:

  This function calls 
    - the function that evaluates the value of variable dtx_drx_status.
    - the function that updates the l1_cpc_dtx_req_mask for L1
    - the function updates the various masks in l1 and other information based on 
      evaluated value of dtx_drx_status variable

DEPENDENCIES:
  Function is only called after processing of all the IEs in a config message is 
  completed.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_evaluate_dtx_drx_status_var_and_set_lower_layers_action
(
  tdsrrc_RRC_StateIndicator rcvd_state_indicator,
  uint32 dtx_drx_timingInfo_present_ota
);
#endif

#ifdef FEATURE_TDSCDMA_REL8
/*====================================================================
FUNCTION: tdsrrcllc_process_get_cpid_r8

DESCRIPTION:
  This function get cpid present and cpid same as current of r4 msg

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
  boolean *cpid_change_flg   /* either new CELL PARAMETER ID is present and different with current */
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_rl_addition_info_list_r8()

DESCRIPTION:
  This function updates IE "tdsrrc_RL_AdditionInformationList_r8 " in ordered config
  and update e_dch parameters

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_rl_addition_info_list_r8
(
  uint32 rl_additon_info_list_present,
  tdsrrc_RL_AdditionInformationList_r8 *rl_add_ptr
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_r8()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r8
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r8 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_common_info_r8()

DESCRIPTION:
  This function updates IE "dl common info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_common_info_r8
(
  uint32 dl_common_info_present,
  tdsrrc_DL_CommonInformation_r8 *dl_common_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dl_info_per_rl_list_r8

DESCRIPTION:
  This function updates IE "dl per rl info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dl_info_per_rl_list_r8
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r8 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_e_dch_information_r8()

DESCRIPTION:
  This function updates IE "ul e dch information" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_e_dch_information_r8
(
  uint32 e_dch_info_present,
  tdsrrc_UL_EDCH_Information_r8 *e_dch_info_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: rrcllcpcie_process_dl_hspdsch_info_and_update_hsdpa_msg_params_r8()

DESCRIPTION:
  This function updates IE "dl hspdsch info " in ordered config and update
  hsdpa_msg_param variable
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_hspdsch_info_and_upd_hsdpa_msg_params_r8
(
  uint32 dl_hspdsch_info_present,
  tdsrrc_DL_HSPDSCH_Information_r8 *dl_hspdsch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info_r8()

DESCRIPTION:
  This function takes up deleted and added UL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type 
tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info_r8
(
  uint32 ul_add_recfg_trch_info_list_present,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r8 * ul_trch_add_reconfig_ptr,
  uint32 ul_deleted_trch_info_list_present,
  tdsrrc_UL_DeletedTransChInfoList_r6 * ul_deleted_trch_list_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_process_srb_info_setup_list_r8()

DESCRIPTION:
  This function process IE tdsrrc_SRB_InformationSetupList_r8 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_srb_info_setup_list_r8
(
  uint32 srb_info_setup_list_present,
  tdsrrc_SRB_InformationSetupList_r8 * src_info_setup_list_ptr
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_srb_info_to_setup_r8()

DESCRIPTION : Get Signalling Radio Bearer Information to setup.

REUSABILITY: R6

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_srb_info_to_setup_r8
(
  tdsrrc_SRB_InformationSetup_r8     *ie_ptr,
  tdsrrc_SRB_InformationSetupList_r8 * srb_setup_head_ptr,
  tdsrrc_RB_Identity                   rb_id
);


/*============================================================================
FUNCTION: tdsrrcllc_store_ul_add_reconf_trch_info_list_r8()

DESCRIPTION:
  This function updates IE "ul added reconfig trch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_store_ul_add_reconf_trch_info_list_r8
(
  uint32 ul_add_recfg_trch_info_list_present,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r8 * ul_trch_add_reconfig_ptr  
);

/*============================================================================
FUNCTION: tdsrrcllc_process_rab_info_setup_list_r8()

DESCRIPTION:
  This function process IE tdsrrc_RB_InformationAffectedList_r8 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rab_info_setup_list_r8
(
  uint32 rab_info_setup_list_present,
  tdsrrc_RAB_InformationSetupList_r8 * rab_info_setup_list_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_process_rb_info_reconfig_list_r8()

DESCRIPTION:
  This function process IE tdsrrc_RB_InformationReconfigList_r6 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rb_info_reconfig_list_r8
(
  uint32 rb_info_reconfig_list_present,
  tdsrrc_RB_InformationReconfigList_r8 * rb_info_reconfig_list_ptr
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_reconfig_r8()

DESCRIPTION : Get Radio Bearer Information to reconfig.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_reconfig_r8
(
tdsrrc_RB_InformationReconfig_r8 *ie_ptr
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_setup_r8()

DESCRIPTION : Get Radio Bearer Information to setup.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_setup_r8
(
  tdsrrc_RB_InformationSetup_r8       *ie_ptr,
#ifdef FEATURE_UMTS_PDCP
  tdsrrc_CN_DomainIdentity cn_Domain,
#endif /* FEATURE_UMTS_PDCP*/
  tdsrrc_RB_InformationSetupList_r8  * rb_setup_head_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_process_rb_info_affected_list_r8()

DESCRIPTION:
  This function process IE tdsrrc_RB_InformationAffectedList_r8 and store the 
  received information in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_process_rb_info_affected_list_r8
(
  uint32 rb_info_affected_list_present,
  tdsrrc_RB_InformationAffectedList_r8 * rb_info_affected_list_ptr
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_affected_r8()

DESCRIPTION : Get Radio Bearer Information to be affected.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_affected_r8
(
tdsrrc_RB_InformationAffected_r8*ie_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_store_dl_add_reconf_trch_info_list_r8()

DESCRIPTION:
  This function updates IE "dl added reconfig trch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_store_dl_add_reconf_trch_info_list_r8
(
  uint32 dl_add_reconf_trch_info_list_present,
  tdsrrc_DL_AddReconfTransChInfoList_r7* dl_add_reconf_trch_info_list_ptr,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r8 * ul_add_reconf_trch_info_list_ptr  
);

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_update_oc_with_srb5_rel8_ie()

DESCRIPTION:
  This function updates tdsordered_config with SRB#5 information such as 
  RB ID & transport channel ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE if SRB#5 is found & OC is successful cases.
  FALSE otherwise

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_with_srb5_rel8_ie
(
  tdsrrc_SRB_InformationSetup_r8 * srb_setup_r8_ptr, 
  tdsrrc_RB_Identity rb_id
);
#endif /*FEATURE_TDSCDMA_SRB5_SUPPORT*/

/*==========================================================
FUNCTION  : tdsrrcllc_backup_ul_dch_from_ota_r8()

DESCRIPTION : This function backs up the Rb-Mapping info 
              received in the REL8 OTA message for UL DCH.
              This function is called when the OTA message 
              provides the RB-Mapping info, 
              but the Trch info mapped to that RB is not provided.

DEPENDENCIES: None.

RETURN VALUE: boolean

SIDE EFFECTS: None
============================================================*/
 uecomdef_status_e_type tdsrrcllc_backup_ul_dch_from_ota_r8
(
  tdsrrc_UL_LogicalChannelMapping_r8*ie_ptr,  
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_edch_mapping_info_r8()

DESCRIPTION : Processes EDCH RB Mapping info contents and saves 
              them in MAC Ordered Configuration Data  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_edch_mapping_info_r8
(
tdsrrc_UL_LogicalChannelMapping_r8 *ie_ptr,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode
,boolean one_sided_reest_present
,boolean one_sided_reest
,boolean allow_new_channels
);

#ifdef FEATURE_TDSCDMA_HSUPA
/*==========================================================
FUNCTION  : tdsrrcllc_backup_eul_from_ota()

DESCRIPTION : This function backs up the Rb-Mapping info 
              received in the OTA message for EULH. 
              This function is called when the OTA message 
              provides the RB-Mapping info, but the Trch 
              info mapped to that RB is not provided.

DEPENDENCIES: None.

RETURN VALUE: boolean

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type tdsrrcllc_backup_eul_from_ota_r8
(
  tdsrrc_UL_LogicalChannelMapping_r8*ie_ptr,  
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
);
#endif /*FEATURE_TDSCDMA_HSUPA*/

#ifdef FEATURE_TDSCDMA_MIMO
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_mimo_params_r8()

DESCRIPTION:
  This function updates IE "Mimo Params" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type    tdsrrcllcpcie_process_mimo_params_r8
(
  tdsrrc_MIMO_Parameters_r8 * mimo_params,
  tdsrrc_RRC_StateIndicator rrc_state,
  uint32 mimo_params_present_ota
);
#endif /*FEATURE_TDSCDMA_MIMO*/

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
);

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
);

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
);

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
);

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
);

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
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_process_dc_hsdpa_params()

DESCRIPTION:
  This function updates IE "DL_SecondaryCellInfoFDD" in ordered config for R8 OTAs.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_dc_hsdpa_params
(
  tdsrrc_DL_SecondaryCellInfoFDD * dc_hsdpa_params,
  tdsrrc_RRC_StateIndicator rrc_state,
  uint32 dc_hsdpa_params_present_ota
);

/*============================================================================
FUNCTION: tdsrrcllcpcie_update_l1_action_for_dc_hsdpa_params()

DESCRIPTION:
  This function updates L1 action/configuration for DC-HSDPA. By this time, OC is already set with
  DC-HSDPA content.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
boolean tdsrrcllcpcie_update_l1_action_for_dc_hsdpa_params
(
  tdsrrc_DL_SecondaryCellInfoFDD * dc_hsdpa_params,
  tdsrrc_RRC_StateIndicator rrc_state,
  uint32 dc_hsdpa_params_present_ota  
);

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
);

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
);
#endif /*FEATURE_TDSCDMA_DC_HSDPA*/


/*====================================================================
FUNCTION: tdsrrcllc_rrc_conn_setup_ie_r8()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in R8 RRC Connection Setup Message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rrc_conn_setup_ie_r8
(
  tdsrrc_RRCConnectionSetup_r8 *rrc_conn_setup_r8_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_cell_update_confirm_ie_r8()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Cell Update Confirm Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_cell_update_confirm_ie_r8
(
  uint32 dl_sdu_num,
  void * cell_update_r8_msg_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_phychan_reconfig_ie_r8()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Physical Channel Reconfiguration Message for Rel 8

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_phychan_reconfig_ie_r8
(
  tdsrrc_PhysicalChannelReconfiguration_r8 * phys_chan_reconfig_r8_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_rb_reconfig_ie_r8()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_reconfig_ie_r8
(
  tdsrrc_RadioBearerReconfiguration_r8 * rb_reconfig_r8_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_rb_setup_ie_r8()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Setup Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_setup_ie_r8
(
  tdsrrc_RadioBearerSetup_r8 * rb_setup_r8_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_rb_release_ie_r8()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Release 8 Radio Bearer Release Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_release_ie_r8
(
  tdsrrc_RadioBearerRelease_r8 * rb_release_r8_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_trch_reconfig_ie_r8()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Release 8 Transport Channel Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_trch_reconfig_ie_r8
(
  tdsrrc_TransportChannelReconfiguration_r8 * trch_reconfig_r8_ota
);

#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
#ifdef FEATURE_TDSCDMA_DMO_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_get_dmo_r9()

DESCRIPTION:
  This function processes the IE "CELL_DCH measurement occasion info LCR"

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_mlength_k_chk_e_type tdsrrcllc_get_dmo_r9
(
  tdsrrc_CellDCHMeasOccasionInfo_TDD128_r9* dmo_ptr,
  tdsl1_meas_occasion_info_struct_type* target_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_deactivate_dmo_r9()

DESCRIPTION:
  This function deactivate all DMO

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_deactivate_dmo_r9
(
  tdsl1_meas_occasion_info_struct_type* target_ptr
);
#endif
/*====================================================================
FUNCTION: tdsrrcllc_rrc_conn_setup_ie_r9()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in R8 RRC Connection Setup Message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rrc_conn_setup_ie_r9
(
  tdsrrc_RRCConnectionSetup_r9 *rrc_conn_setup_r9_ota
);
/*====================================================================
FUNCTION: tdsrrcllc_cell_update_confirm_ie_r9()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Cell Update Confirm Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_cell_update_confirm_ie_r9
(
  uint32 dl_sdu_num,
  void * cell_update_r9_msg_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_phychan_reconfig_ie_r9()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Physical Channel Reconfiguration Message for Rel 9

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_phychan_reconfig_ie_r9
(
  tdsrrc_PhysicalChannelReconfiguration_r9 * phys_chan_reconfig_r9_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_rb_reconfig_ie_r9()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_reconfig_ie_r9
(
  tdsrrc_RadioBearerReconfiguration_r9 * rb_reconfig_r9_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_rb_setup_ie_r9()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Setup Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_setup_ie_r9
(
  tdsrrc_RadioBearerSetup_r9 * rb_setup_r9_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_rb_release_ie_r9()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Release9 Radio Bearer Release Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_release_ie_r9
(
  tdsrrc_RadioBearerRelease_r9 * rb_release_r9_ota
);

/*====================================================================
FUNCTION: tdsrrcllc_trch_reconfig_ie_r9()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Release 9Transport Channel Reconfiguration Message

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_trch_reconfig_ie_r9
(
  tdsrrc_TransportChannelReconfiguration_r9 * trch_reconfig_r9_ota
);
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
);
/*====================================================================
FUNCTION      tdsrrcllcpcie_check_if_hsscch_changed_r9()

DESCRIPTION   This function checks if HS-SCCH Info has changed.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
boolean tdsrrcllcpcie_check_if_hsscch_changed_r9
(
  tdsrrc_HS_SCCH_Info_r9* scch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  , uint8 multicarrier_flg
#endif
);
/*====================================================================
FUNCTION      tdsrrcllcpcie_set_hsscch_info_r9()

DESCRIPTION   This function sets the Channelization code required
              for HSPDSCH in L1 structure in OC. IEs refer to 10.3.6.36a.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire operation is successful,
              FAILURE otherwise

SIDE EFFECTS  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_set_hsscch_info_r9
(
  tdsrrc_HS_SCCH_Info_r9* hsscch_info_ptr
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  ,
  uint8 multicarrier_flg
#endif
);

/*====================================================================
FUNCTION: FIND_TRCH_ID_IN_CONFIG_TRCH_LIST_R9

DESCRIPTION:

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_trch_id_in_config_trch_list_r9
(
  tdsrrc_DL_AddReconfTransChInfoList_r9 * dl_trch_add_reconfig_ptr,
  tr_ch_id_type tdstrch_id
);
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
);
/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info_r9()

DESCRIPTION:
  This function takes up deleted and added UL DCH Transport channel
  List and updates local, MAC and L1 data structures with transport
  channel IDs and count. It also re-arranges transport channel info
  in local, MAC and L1 structures so that it is arranged and sorted
  by TrCH IDs.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r9
(
  /* local DL TrCH Add Reconfig pointer */
  tdsrrc_DL_AddReconfTransChInfoList_r9 * dl_trch_add_reconfig_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList_r7    * dl_trch_delete_ptr
);
/*============================================================================
FUNCTION: tdsrrcllc_store_dl_add_reconf_trch_info_list_r9()

DESCRIPTION:
  This function updates IE "dl added reconfig trch info" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_store_dl_add_reconf_trch_info_list_r9
(
  uint32 dl_add_reconf_trch_info_list_present,
  tdsrrc_DL_AddReconfTransChInfoList_r9 *dl_add_reconf_trch_info_list_ptr,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r8  *ul_add_reconf_trch_info_list_ptr  
);
/*============================================================================
FUNCTION: tdsrrcllcpcie_process_ul_e_dch_information_r9()

DESCRIPTION:
  This function updates IE "ul e dch information" in ordered config
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_ul_e_dch_information_r9
(
  uint32 e_dch_info_present,
  tdsrrc_UL_EDCH_Information_r9 *e_dch_info_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);
/*============================================================================
FUNCTION: rrcllcpcie_process_dl_hspdsch_info_and_update_hsdpa_msg_params_r9()

DESCRIPTION:
  This function updates IE "dl hspdsch info " in ordered config and update
  hsdpa_msg_param variable
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_process_hspdsch_info_and_upd_hsdpa_msg_params_r9
(
  uint32 dl_hspdsch_info_present,
  tdsrrc_DL_HSPDSCH_Information_r9 *dl_hspdsch_info_ptr,
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);

#endif /* FEATURE_TDSCDMA_REL9 */

/*===========================================================================
FUNCTION tdsrrcllc_update_utran_drx_cycle_length_coeff_in_oc

DESCRIPTION
This function updates the utran DRX cycle length coefficient to OC received in
any reconfig message.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS or FAILURE
===========================================================================*/

uecomdef_status_e_type tdsrrcllc_update_utran_drx_cycle_length_coeff_in_oc_r7
(
  uint32 utran_drx_cycle_len_coeff_present, 
  tdsrrc_UTRAN_DRX_CycleLengthCoefficient_r7 * utran_drx_cycle_len_coeff
);

#ifdef FEATURE_TDSCDMA_CPC_DRX
/*====================================================================
FUNCTION: tdsrrcllc_init_cpc_drx_info_before_msg_processing()

DESCRIPTION:
  This function resets needed CPC-DRX variables before processing any config message

DEPENDENCIES:
  Function is only called before processing the DL OTA Msg.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init_cpc_drx_info_before_msg_processing
(
  tdsordered_config_type *config_ptr
);

/*============================================================================
FUNCTION: tdsrrcllc_init_drx_info_before_processing()

DESCRIPTION:
  This function will init stored DRX info before processing 
  DRX_info I.E
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
void  tdsrrcllc_init_drx_info_before_processing
(
  tdsordered_config_type *config_ptr
);
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
);
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
);
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
);
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
);
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
);

/*====================================================================
FUNCTION: tdsrrcllc_is_drx_params_variable_set()

DESCRIPTION:
  This function evaluates the value of variable tdsrrcllc_is_drx_params_variable_set.

DEPENDENCIES:
  Function is only called after processing of all the IEs in a config message is completed.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_is_drx_params_variable_set
(
  void
);
/*====================================================================
FUNCTION: tdsrrcllc_is_drx_params_variable_set()

DESCRIPTION:
  This function evaluates the value of variable tdsrrcllc_is_drx_params_variable_set.

DEPENDENCIES:
  Function is only called after processing of all the IEs in a config message is completed.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_is_drx_params_variable_set
(
  void
);

#endif
/*================================================================================================
FUNCTION: tdsrrcllc_get_dl_added_reconfig_trch_r4()

DESCRIPTION:
  This function takes a pointer to the Uplink Added or Reconfigured 
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  CELL_DCH additions are the only item supported now.
===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_added_reconfig_trch_r4
(
  tdsrrc_DL_AddReconfTransChInformation_r4 * ie_ptr,
  tdsrrc_UL_AddReconfTransChInfoList * ul_ptr
);
/*====================================================================
FUNCTION      tdsrrcllc_get_ul_dpch_info_r4()

DESCRIPTION   This function translates the Uplink DPCH info common for
              all RL (radio links) IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
 uecomdef_status_e_type tdsrrcllc_get_ul_dpch_info_r4
 (
   tdsrrc_UL_DPCH_Info_r4 *ie_ptr
 );

/*============================================================================
FUNCTION: tdsrrcllcpcie_is_hho_r4()

DESCRIPTION:
  This function returns boolean value based on whether inter-freq or intra-freq
  HHO is ongoing. 

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - HHO in progress
  FALSE - No HHO

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_is_hho_r4
(
  uint32 dl_info_per_rl_list_present,
  tdsrrc_DL_InformationPerRL_List_r4 *dl_info_per_rl_list_ptr,  
  tdsrrc_RRC_StateIndicator rcvd_state_indicator
);
/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_common_for_all_rl_r4()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_common_for_all_rl_r4
(
  /* Pointer to the DL CommonInformation IE */
  tdsrrc_DL_CommonInformation_r4 *ie_ptr,

  /* Pointer to the desirec DL PhyChan common information structure */
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
);

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_reconfig_r4()

DESCRIPTION : Get Radio Bearer Information to reconfig.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_reconfig_r4
(
  tdsrrc_RB_InformationReconfig_r4 *ie_ptr
);
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
);
/*====================================================================
FUNCTION: tdsrrcllc_rrc_conn_setup_ie_r4()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the RRC Connection Setup Message.

  The function is copy from tdsrrcllc_rrc_conn_setup_ie

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rrc_conn_setup_ie_r4
(
  tdsrrc_RRCConnectionSetup_r4 *msg_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_find_trch_id_in_ul_trch_list

DESCRIPTION: 
    The function is to find TRCH ID whether in current OTA message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_find_trch_id_in_ul_trch_list
(
  tdsrrc_UL_AddReconfTransChInfoList * ul_trch_add_reconfig_ptr,
  tr_ch_id_type tdstrch_id
);

/*====================================================================
FUNCTION: tdsrrcllc_cell_update_confirm_ie_r4()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Cell Update Confirm Message.

  The function is copy from tdsrrcllc_cell_update_confirm_ie

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_cell_update_confirm_ie_r4
(
  tdsrrc_CellUpdateConfirm_r4_IEs *msg_ptr,
  boolean    ccch_dcch_flg,
  tdsrrc_CellUpdateConfirm_CCCH_v4d0NonCriticalExtensions *vccch4d0NonCriticalExtensions,
  tdsrrc_CellUpdateConfirm_v4d0NonCriticalExtensions *v4d0NonCriticalExtensions
);

/*====================================================================
FUNCTION: tdsrrcllc_phychan_reconfig_ie_r4()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Physical Channel Reconfiguration Message

  The function is copy from tdsrrcllc_phychan_reconfig_ie

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_phychan_reconfig_ie_r4
(
  tdsrrc_PhysicalChannelReconfiguration_r4 *msg_ptr
);
/*====================================================================
FUNCTION: tdsrrcllc_rb_reconfig_ie_r4()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Reconfiguration Message

  The function is copy from tdsrrcllc_rb_reconfig_ie

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_reconfig_ie_r4
(
  tdsrrc_RadioBearerReconfiguration_r4 *msg_ptr
);
/*====================================================================
FUNCTION: tdsrrcllc_rb_release_ie_r4()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Radio Bearer Release Message

  The function is copy from tdsrrcllc_rb_release_ie

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_rb_release_ie_r4
(
  tdsrrc_RadioBearerRelease_r4 *msg_ptr
);
/*====================================================================
FUNCTION: tdsrrcllc_trch_reconfig_ie_r4()

DESCRIPTION:
  This function updates tdsordered_config with the latest IEs recv'd 
  in the Transport Channel Reconfiguration Message

  The function is copy from tdsrrcllc_trch_reconfig_ie

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_trch_reconfig_ie_r4
(
  tdsrrc_TransportChannelReconfiguration_r4 *msg_ptr
);
/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r4()

DESCRIPTION   This function translates the Uplink DPCH Power Control
              info common for all RL (radio links) IE into the RRC
              config database. IEs can refer to 10.3.6.91 in 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_dpch_pwr_ctrl_info_r4
(
  tdsrrc_UL_DPCH_PowerControlInfo_r4_tdd *ie_ptr
);


/*====================================================================
FUNCTION       tdsrrc_get_mac_d_hfn_cucnf_r4()

DESCRIPTION    This function takes a pointer to the cu_confirm_ptr for R4
                         and extract mac_d_hfn field.

DEPENDENCIES   None

RETURN VALUE   value of mac_d_hfn if present else, invalid value

====================================================================*/
uint32 tdsrrc_get_mac_d_hfn_cucnf_r4(tdsrrc_CellUpdateConfirm_r4_IEs *cu_confirm_ptr);
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*====================================================================
FUNCTION: tdsrrcllc_update_oc_with_srb5_r4()

DESCRIPTION:
  This function updates tdsordered_config with SRB#5 information such as 
  RB ID & transport channel ID.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE if SRB#5 is found & OC is successful cases.
  FALSE otherwise

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type
tdsrrcllc_update_oc_with_srb5_r4
(
  tdsrrc_SRB_InformationSetup * srb_setup_r5_ptr, 
  tdsrrc_RB_Identity rb_id
);
#endif
/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_setup_r4()

DESCRIPTION : Get Radio Bearer Information to setup.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_setup_r4
(
tdsrrc_RB_InformationSetup_r4       *ie_ptr,
#ifdef FEATURE_UMTS_PDCP
tdsrrc_CN_DomainIdentity cn_Domain,
#endif /* FEATURE_UMTS_PDCP*/
tdsrrc_RB_InformationSetupList_r4   * rb_setup_ptr
);



/*===========================================================================
FUNCTION        tdsrrcllc_get_dl_info_per_rl_r4()

DESCRIPTION     This function validates and stores Downlink Information Per
                Radio Link for reconfiguration messages.

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise

SIDE EFFECTS    Ordered config is updated with the new RL info.
===========================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_per_rl_r4
(
  tdsrrc_DL_InformationPerRL_List_r4 *ie_ptr,
  tdsrrc_RRC_StateIndicator next_state
);
/*====================================================================
FUNCTION        tdsrrcllc_get_dl_info_for_each_rl_r4()

DESCRIPTION     This function translates the Downlink information for
                each radio link IE to the RRC config database

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
tdsrrcllcoc_dl_info_per_rl_status_e_type tdsrrcllc_get_dl_info_for_each_rl_r4
(
  tdsrrc_DL_InformationPerRL_r4 *ie_ptr,
  tdsl1_dl_phycfg_phychan_db_struct_type *dl_phychan
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_fpach_info_r4()

DESCRIPTION   This function translates the FPACH
              info common for all RL (radio links) IE into the RRC
              config database. IE refer to 10.3.6.35a in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_fpach_info_r4
(
  tdsrrc_FPACH_Info_r4 *ie_ptr
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_tim_adv_contrl_r4()

DESCRIPTION   This function translates the Uplink Timing Advance Control
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.96 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_tim_adv_contrl_r4
(
  tdsrrc_UL_TimingAdvanceControl_r4 *ie_ptr
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_com_ts_info_r4()

DESCRIPTION   This function translates the Common Timeslot
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.10 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_com_ts_info_r4
(
  tdsrrc_CommonTimeslotInfo *ie_ptr,
  tdsl1_common_ts_info_struct_type *comts_ptr,
  boolean dl_indicator
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_indiv_r4
(
  tdsrrc_IndividualTimeslotInfo_LCR_r4 *src_ptr,
  tdsl1_ul_dpch_ts_struct_type *dst_cfg
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_channelcode_r4
(
  tdsrrc_UL_TS_ChannelisationCodeList *src_ptr,
  tdsl1_ul_dpch_ts_struct_type *dst_cfg
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_ts_and_code_r4()

DESCRIPTION   This function translates the Uplink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.94a in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_ts_and_code_r4
(
  tdsrrc_UL_CCTrCH_r4_tdd128 *ie_ptr,
  tdsl1_ul_dpch_cfg_struct_type *dpch_cfg
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_ul_cctrch_list_r4()

DESCRIPTION   This function translates the Uplink CCTrCH List
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.88 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_ul_cctrch_list_r4
(
  tdsrrc_UL_CCTrCHList_r4 *ie_ptr
);


/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_ts_and_code_indiv_r4()

DESCRIPTION   This function translates the Downlink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_ts_and_code_indiv_r4
(
  tdsrrc_IndividualTimeslotInfo_LCR_r4 *src_ptr,
  tdsl1_dl_phycfg_individual_ts_info_struct_type *dst_ptr
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_ts_and_code_channelcode_r4()

DESCRIPTION   This function translates the Downlink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.37 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_ts_and_code_channelcode_r4
(
  tdsrrc_DL_TS_ChannelisationCodesShort_codesRepresentation *src_ptr,
  tdsl1_dl_phycfg_individual_ts_info_struct_type *dst_ptr
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_ts_and_code_r4()

DESCRIPTION   This function translates the Downlink Timeslot and code
              info for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.32 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_ts_and_code_r4
(
  tdsrrc_DL_CCTrCh_r4_tdd128 *ie_ptr,
  tdsl1_dl_phycfg_cctrch_list_struct_type *cctrch_ptr
);

/*====================================================================
FUNCTION      tdsrrcllcpcie_get_dl_cctrch_list_r4()

DESCRIPTION   This function translates the Downlink CCTrCH List
              info common for all RL (radio links) IE into the RRC
              config database. IEs refer to 10.3.6.21 in 3GPP 25331v716.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_cctrch_list_r4
(
  tdsrrc_DL_CCTrCh_r4 *ie_ptr,
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch
);

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
);
/*====================================================================
FUNCTION: tdsrrcllc_clear_sib5_change_in_progress_flag()

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
);

#ifdef FEATURE_TDSCDMA_HSUPA

/*====================================================================
FUNCTION: tdsrrcllc_get_ul_rlc_size_for_eul()

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
uint32 tdsrrcllc_get_ul_rlc_size_for_eul
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_RB_Identity  rb_id
);
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
);
/*====================================================================
FUNCTION: tdsrrcllc_set_default_value_fpach_and_uppch

DESCRIPTION:
   The function is to set default FPACH freq and uppch position info 
   when they are absent in current OTA message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_set_default_value_fpach_and_uppch
(
  boolean fpach_freq_present,
  boolean uppch_positionInfo_present
);

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
/*====================================================================
FUNCTION      tdsrrcllc_get_ul_dpch_info_r5_iho()

DESCRIPTION   This function translates the Uplink DPCH info common for
              all RL (radio links) IE into the RRC config database.

DEPENDENCIES  None

RETURN VALUE  SUCCESS if the entire translation operation is successful,
              FAILURE otherwise

SIDE EFFECTS  The RRC config database page pointed to by the input
              parameter ord_cfg is overwritten with all IE elements
              that are present.  Those IE elements that are not present
              do NOT overwrite the database values with, say, default
              values.  Any optional parameters that are present also
              cause parameter presence bits in the database to be set.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_dpch_info_r5_iho
(
  tdsrrc_UL_DPCH_InfoPostTDD_LCR_r4 *ie_ptr
);

/*===========================================================================
FUNCTION        tdsrrcllc_get_dl_info_per_rl_r5_iho()

DESCRIPTION     This function validates and stores Downlink Information Per
                Radio Link for reconfiguration messages.

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise

SIDE EFFECTS    Ordered config is updated with the new RL info.
===========================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_info_per_rl_r5_iho
(
  tdsrrc_DL_InformationPerRL_PostTDD_LCR_r4 *ie_ptr
);

/*====================================================================
FUNCTION        tdsrrcllcpcie_get_dl_info_common_for_all_rl_iho()

DESCRIPTION     This function translates the Downlink information common
                for all radio links IE into the RRC config database.

DEPENDENCIES    NONE

RETURN VALUE    SUCCESS if all goes well, FAILURE otherwise.

SIDE EFFECTS
====================================================================*/
uecomdef_status_e_type tdsrrcllcpcie_get_dl_info_common_for_all_rl_iho
(
  tdsl1_dl_phycfg_dpch_parms_struct_type *dpch,
  tdsrrc_U_RNTI_Short *new_urnti_ptr
);
#endif
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
tdsrrc_state_e_type tdsrrcllc_get_target_state
(
void
);
#endif/* FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE */


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
);


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
);


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
);


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
);

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
);

#endif /*FEATURE_TDSCDMA_RRC_RB_PRIORITY_OPT*/

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
);

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
uint16 tdsrrcllc_translate_n313
(
tdsrrc_N_313 val
);
#endif /* TDSRRCLLCOC_H */


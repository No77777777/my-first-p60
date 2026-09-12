#ifndef MAC_UL_HS_H
#define MAC_UL_HS_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              U P L I N K   M A C   H S U P A   H E A D E R    F I L E

DESCRIPTION
   MAC HSUPA specific type declarations.


Copyright (c) 2001, 2002, 2003 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2006-2009 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/mac/inc/maculhs.h#3 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who      what, where, why
--------   -------- --------------------------------------------------------
04/22/16  kc        Separting out SI timers for grant and no grant cases
10/19/15   kc       API to indicate if EUL is active in DCH state
09/28/15   as       MAC to delay event 6D trigger by limiting the ETFCI selection in UL inorder to avoid 
                    UE hitting MTPL due to data traffic.
09/22/15   kv       Redesign 0x4322/23 logging to remove race condition in double buffer
09/14/15   kv       Update L2 structure with ETFCI reason and expose in l1macdata.h
08/18/15   aa       MAC to pass correct values to UL for secondary carrier EUL logging
07/22/15   kv       FR 22377: RAVE-API
07/01/15   kv       FR 26237: DSDS with Rel.8 E-FACH-(HSRACH+MSIM) L2-Changes 
06/26/15   aa       Removing extern declarations from L2 code
06/23/15   as       Fix compilation errors when DCHSUPA is undefined
06/08/15   as       W+W MAC UL/DL HS task variable doubling.
06/04/15   kv       Compiler Warnings 
06/01/15   as       In FACH state MAC should send CMAC_STATUS_IND only for TM CCCH UL data.
05/29/15   as       Added a new API to trigger SI whenever UE comes out of LTA or QTA gap.
05/26/15   kc       Fix to not set MAC-EHS decode flag for 1st row of 0x4314 packet, when HS-RACH resource is not yet acquired
03/03/15   as       SW enhancement to avoid reporting of event 6D due to persistent higher ETFCIs selected on both the carriers
                    based upon L1 operating power and delay 6D trigger by limiting number of bits to be transmitted.
04/25/15   scv      W+W Changes to have messages support SubID
04/10/15   kv       Moved Qxdm Support for MAC debug F3s from L1 UL to MAC as part of WpW cleanup
04/07/15   kv       MAC to pass valid PO for ReTx if SG or HARQ is de-activated after First TX
04/06/15   kv       Fix to log missing sample in 0x430E during buffer swap
03/30/15   aa       Changes to 0x430E to include ERNTI in header and not count SIs in sched payload calculation
03/10/15   aa       Duplicated L2 task related globals and removed AS_ID hardcodings
01/29/15   aa       L2 global reorg for W+W
10/21/14   ts       SW enhancement to avoid reporting of event 6D due to persistent higher ETFCIs selected on both the carriers
07/30/14   ts       16QAM: Updated the validation check for the ETFCI table passed by RRC, in the config handler function
07/02/14   ts       Fix to STOP SI timers on Secondary Carrier Deactivation per spec CR 25.321_CR0624
05/15/14   kc       Changes for enabling RxD for FACH Signaling when HS-RACH is configured
05/13/14   ymu      Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
03/19/14   kc       Changing FEATURE for heap corruption issue.
03/18/14   ts       Fix for heap memory corruption issue, to allocate and free heap memory fo rlog buffers in l2ultask itself
02/24/14   ts       Changes for 0x430E log packet redesign
02/05/13   ts       DC-HSUPA: Fix for incorrect transmission of preambles on Primary Carrier when Secondary alone is capable of clearing data available
12/13/13   kc       Changes for Happy Bit computation code redesign
11/07/13   ts       DC-HSUPA: Changes for SI timer redesign by keeping a single base timer and reducing signals
11/06/13   as       Fix to consider all the restrictions in EUL new data transfer irrespective of SI trigger 
                    condition and should not trigger immediately when we 
06/12/13   as       HSRACH - Fix to clear TM mode partial info at MAC as well during all scenarios
                    while flushing RLC TM water mark and partial info.
05/28/13   kc       Change to save PO computed in Non-Sched PO API & use it during ETFCI selection
05/22/13   kc       Added DC-HSUPA Changes
05/03/13   kc       Remove CM interleaving workaround
04/26/13   as       MAC-I: MAC-I: MAC not to error fatal during RRC config if there is HS UL type transition 
                    without MAC-es/e reset indicator flag set to TRUE.
12/03/12   mrg      HSRACH - When EDCH resource is rleased - commit the 0x430E log packet with available log samples
10/12/12   kc       HS_RACH MAC F3's moved under debug variable and x4314 log packet issue
07/16/12   mrg      MAC_I & HS-RACH Compilation Warnings fixed
08/22/12   kc       New log packet 0x4314 for HS RACH MAC timer info
08/13/12   mrg      HS-RACH & MAC-I Feature updated code check-in.
07/24/12   geg      Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/05/12   mrg      HS-RACH & MAC-I Feature code integrated
01/02/12   grk      Feature cleanup
10/22/11   sk       NikeL UL CM workaround under feature FEATURE_WCDMA_UL_CM_INTLV_WORKAROUND
12/17/10   mrg      Changes for LOW GRANT support when the UE is in DTX Call and 
                    SRB delay condition added in L1 data query interface function 
12/06/10   mrg      Reverting the CR-265483 changes 
12/03/10   mrg      LOW_GRANT and SRB_OMIT conditions are added to the L1 data query funciton
06/01/10   mrg      IRQ split changes
10/12/09   mrg      MAC_HS_UL_MAX_NUM_RLC_PDUS_PER_LC define is added
05/07/09   ssg      Updated Copyright Information
04/29/09   av       Fixed compiler warnings in L2 
04/15/09   mrg      Removing extra prototype of the funciton 
                    l1_phy_edch_get_mac_hs_ul_mac_d_si_log_info()
10/17/08   grk      Merged taxis changes to mainline.
06/03/08   kvk      Added support for new reason fields
05/02/08   sk       If the SG bits is in the range of 0 and (min_sched_rlc_pdu_size+hdr) treat it as 0
                    for purposes of quantization.
03/13/08   sk       Added support to serve equally 2 lcs with the same priority in a round robin fashion
                    across tti's
03/03/08   sk       Moved some F3's under Debug flag. Also changed the array size for
                    the prev_tx_info to 8
02/19/08   sk       Chnaged the MAC_HS_UL_MAX_HARQ_ID to 8
10/07/07   sk       Added min_rlc_pdu_size to mac_hs_ul_si_happybit_parameters_config_type
02/04/07   sk       Added support for the 0x4311 Log packet
12/20/06   am       Changed return type of mac_d_si_log_info func
12/06/06   sk       Made changes to the 4309 log packet. Added the enum fo rthe etfci retsrtcion reason
11/15/06   sk       Added HHO support
01/02/06   sk       Initial revision
09/07/06   sk       Merged all IOT fixes
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#ifdef FEATURE_WCDMA_HSUPA

#include "macrrcif_v.h"
#include "macinternal.h"
#include "l1macdata.h"
#include "rlcul.h"

/*===========================================================================

                     DEFINES AND MACROS FOR MODULE

===========================================================================*/
/*--------------------------------------------------------------------------
Number of MAC-d flows configured
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MIN_MAC_D_FLOWS                  1
#define MAC_HS_UL_MAX_MAC_D_FLOWS                  MAX_E_DCH_MAC_D_FLOW

/*--------------------------------------------------------------------------
Ranges for MAC-d flow Id
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MIN_MAC_D_FLOW_ID                0
#define MAC_HS_UL_MAX_MAC_D_FLOW_ID                (MAX_E_DCH_MAC_D_FLOW - 1)

/*--------------------------------------------------------------------------
Max TSN  value
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_TSN                          63

/*--------------------------------------------------------------------------
E-DCH MAC-d flow mpower offset range
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MIN_POWER_OFFSET      0
#define MAC_HS_UL_MAX_POWER_OFFSET      6

/*--------------------------------------------------------------------------
E-DCH MAC-d flow maximum number of  retransmissions
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAC_D_FLOW_MAX_NUM_RETX          15

/*--------------------------------------------------------------------------
Max MAC-e PDU contents size: max number of non scheduled data
     in bits that can be part of MAC-e PDU
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_NON_SCHED_DATA_SIZE          19982

/*--------------------------------------------------------------------------
Since there is no header for MAC-D pdus in EUL, so rlc_pdu_size is
same as mac_d_pdu_size. The pdu size should be between 16 and 5000 bits
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MIN_MAC_D_PDU_SIZE              16
#define MAC_HS_UL_MAX_MAC_D_PDU_SIZE              5000

/*--------------------------------------------------------------------------
The size of the Si is 18bits.
--------------------------------------------------------------------------*/
#define MAC_HS_UL_SI_SIZE                           18

/*--------------------------------------------------------------------------
Data Description Identifier (6 bits), it identifies the logical channel,
MAC-d flow and size of the MAC-d PDUs concatenated into the associated
MAC-es PDU. 0-62 is the valid range. 63 is reserved
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_DDI_VALUE                   63

/*--------------------------------------------------------------------------
Number of valid fields in mapping of rlc pdu size to ddi values. 32 is the
max
--------------------------------------------------------------------------*/
#define MAC_HS_UL_NUM_RLC_PDU_SIZE_DDI            32

/*--------------------------------------------------------------------------
Maximum Number of RLC PDUS allowed in one TTI
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
There are 32 entries in the TEBS value table.
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_TEBS_VALUES                 32

/*--------------------------------------------------------------------------
There are 15 entries in the HLBS value table.
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_HLBS_VALUES                 16

/*--------------------------------------------------------------------------
MAximum value for the HARQ_ID
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_HARQ_ID                     7

/*--------------------------------------------------------------------------
Max number of HARQ processes in case of 2ms TTI configured by RRC
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_NUM_HARQ_PROCESSES_2MS      8

#define MAC_HS_UL_BYTES_TO_BITS_COVERT             8

/*--------------------------------------------------------------------------
Max number of HARQ processes in case of 10ms TTI configured by RRC
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_NUM_HARQ_PROCESSES_10MS     4

/*--------------------------------------------------------------------------
Sec 11.8.1.4 specifies that the UE use maximum number of
HARQ transmissions of 8 for a HARQ profile for 'Control-only' transmissions
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_NUM_RETX_CONTROL_ONLY       7

/*--------------------------------------------------------------------------
Is there is room after E-TFCI selection adn as a result of quantization,
we can attach a special header consisting of all 1s, as in 111111
--------------------------------------------------------------------------*/
#define MAC_HS_UL_SI_SPL_HEADER                   0x3F

/*--------------------------------------------------------------------------
Each uint32 can contain the following
DDI1*N1*DDI2*N2*DDI3*N3-partial
Hence it can completely contain 2 sets of DDI,N and one complete ddi
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_HDR_SIZE                    32

/*  If Grant is not enough to build even 1 PDU,SI will be sent and timer will be started to re-send the SI */
#define MAC_HS_UL_LOW_GRANT_TIMER_VALUE 100

#ifdef FEATURE_WCDMA_DC_HSUPA
/* DC-HSUPA uses extended TSN field of 14 bits instead of 6 bits */
#define   MAC_HS_UL_DC_TSN_EXTRA_ETXN_BITS   8
#define   MAC_HS_UL_MAX_NUM_CARRIER_ALLOWED  2

#define   MAC_HS_UL_SET_SI_TIMER_STATE_INVALID(index)                          \
do                                                                             \
{                                                                              \
  eul_table_ptr->mac_hs_ul_si_timer_info[0].si_state.prev_state = MAC_HS_UL_SI_STATE_INVALID; \
  eul_table_ptr->mac_hs_ul_si_timer_info[1].si_state.prev_state = MAC_HS_UL_SI_STATE_INVALID; \
}while(0);

#define   MAC_HS_UL_GET_INDEX_SI_TIMER_INFO(si_timer_info_ptr, index)          \
do                                                                             \
{                                                                              \
  if(index == EUL_INVALID_CARR_IDX)                                            \
  {                                                                            \
    WMAC_ERR_FATAL(eul_table_ptr->as_id, "MAC_HS_DC: si_timer Invalid carrier index %d", index, 0,0 );    \
  }                                                                            \
  else                                                                         \
  {                                                                            \
    si_timer_info_ptr = &(eul_table_ptr->mac_hs_ul_si_timer_info[index]);                     \
  }                                                                            \
}while(0);
#define   MAC_HS_UL_GET_INDEX_FOR_POWR_OFFSET_DS(si_timer_info_ptr, prev_harq_tx_info_ptr, harq_id, index)\
do                                                                             \
{                                                                              \
  if(index == EUL_INVALID_CARR_IDX)                                            \
  {                                                                            \
    WMAC_ERR_FATAL(eul_table_ptr->as_id, "MAC_HS_DC: PO Invalid index of the carrier %d", index, 0,0 );   \
  }                                                                            \
  else                                                                         \
  {                                                                            \
    si_timer_info_ptr     = &(eul_table_ptr->mac_hs_ul_si_timer_info[index]);                 \
    prev_harq_tx_info_ptr = &(eul_table_ptr->mac_hs_ul_prev_harq_tx_info[harq_id][index]);    \
  }                                                                            \
}while(0);

#define   MAC_HS_UL_GET_INDEX_FOR_ETFCI_DS1(etfc_common_info_ptr, prev_harq_tx_ptr, mac_edch_control_info_ptr, harq_id, index)\
do                                                                             \
{                                                                              \
  if(index == EUL_INVALID_CARR_IDX)                                            \
  {                                                                            \
    WMAC_ERR_FATAL(eul_table_ptr->as_id, "MAC_HS_DC: ETFC Invalid index of the carrier %d", index, 0,0 ); \
  }                                                                            \
  else                                                                         \
  {                                                                            \
    etfc_common_info_ptr      = &eul_table_ptr->mac_hs_ul_etfc_common_info[index];            \
    prev_harq_tx_ptr          = &eul_table_ptr->mac_hs_ul_prev_harq_tx_info[harq_id][index];  \
    mac_edch_control_info_ptr = &eul_table_ptr->mac_hs_ul_edch_control_info[index];           \
    memset(etfc_common_info_ptr, 0x00, sizeof(mac_hs_ul_etfc_common_info_type));  \
    etfc_common_info_ptr->bfi_rlc_info_ptr = &(eul_table_ptr->mac_hs_ul_rlc_pdu_ddi_info[index]);  \
    etfc_common_info_ptr->si_timer_info_ptr = &eul_table_ptr->mac_hs_ul_si_timer_info[index];   \
  }                                                                            \
}while(0);

#ifdef FEATURE_WCDMA_EVT6D_ENH
#define MAC_EV_6D_OPT_TSHLD_TABLE0                   100
#define MAC_EV_6D_OPT_TSHLD_TABLE1                   60
#define MAC_EV_6D_OPT_TSHLD_TABLE2                   87
#define MAC_EV_6D_OPT_TSHLD_TABLE3                   50
#define MAC_EV_6D_OPT_TSHLD_TABLE4                   81
#define MAC_EUL_MAX_E_TFCI_ARRAY_INDEX               5
#endif

#else
#define   MAC_HS_UL_SET_SI_TIMER_STATE_INVALID(index)                          \
do                                                                             \
{                                                                              \
  eul_table_ptr->mac_hs_ul_si_timer_info.si_state.prev_state = MAC_HS_UL_SI_STATE_INVALID; \
}while(0);
#define   MAC_HS_UL_GET_INDEX_SI_TIMER_INFO(si_timer_info_ptr, index)          \
do                                                                             \
{                                                                              \
  si_timer_info_ptr = &(eul_table_ptr->mac_hs_ul_si_timer_info);            \
}while(0);
#define   MAC_HS_UL_GET_INDEX_FOR_POWR_OFFSET_DS(si_timer_info_ptr, prev_harq_tx_info_ptr, harq_id, index)\
do                                                                             \
{                                                                              \
  si_timer_info_ptr     = &(eul_table_ptr->mac_hs_ul_si_timer_info);                 \
  prev_harq_tx_info_ptr = &(eul_table_ptr->mac_hs_ul_prev_harq_tx_info[harq_id][0]);    \
}while(0);
#define   MAC_HS_UL_GET_INDEX_FOR_ETFCI_DS1(etfc_common_info_ptr, prev_harq_tx_ptr, mac_edch_control_info_ptr, harq_id, index)\
do                                                                             \
{                                                                               \
  etfc_common_info_ptr      = &eul_table_ptr->mac_hs_ul_etfc_common_info[0];            \
  prev_harq_tx_ptr          = &eul_table_ptr->mac_hs_ul_prev_harq_tx_info[harq_id][0];  \
  mac_edch_control_info_ptr = &eul_table_ptr->mac_hs_ul_edch_control_info[0];           \
  memset(etfc_common_info_ptr, 0x00, sizeof(mac_hs_ul_etfc_common_info_type));  \
  etfc_common_info_ptr->bfi_rlc_info_ptr = &(eul_table_ptr->mac_hs_ul_rlc_pdu_ddi_info[0]);  \
  etfc_common_info_ptr->si_timer_info_ptr = &eul_table_ptr->mac_hs_ul_si_timer_info;   \
}while(0);

#define   MAC_HS_UL_MAX_NUM_CARRIER_ALLOWED  1
#endif

/* Table 9.2.5.3.2.1 TEBS values (ceil in bits) */

#define MAC_E_TEBS_INDEX_0 0

#define MAC_E_TEBS_INDEX_1 10<<3

#define MAC_E_TEBS_INDEX_2 14<<3

#define MAC_E_TEBS_INDEX_3 18<<3

#define MAC_E_TEBS_INDEX_4 24<<3

#define MAC_E_TEBS_INDEX_5 32<<3

#define MAC_E_TEBS_INDEX_6 42<<3

#define MAC_E_TEBS_INDEX_7 55<<3

#define MAC_E_TEBS_INDEX_8 73<<3

#define MAC_E_TEBS_INDEX_9 97<<3

#define MAC_E_TEBS_INDEX_10 129<<3

#define MAC_E_TEBS_INDEX_11 171<<3

#define MAC_E_TEBS_INDEX_12 228<<3

#define MAC_E_TEBS_INDEX_13 302<<3

#define MAC_E_TEBS_INDEX_14 401<<3

#define MAC_E_TEBS_INDEX_15 533<<3

#define MAC_E_TEBS_INDEX_16 708<<3

#define MAC_E_TEBS_INDEX_17 940<<3

#define MAC_E_TEBS_INDEX_18 1248<<3

#define MAC_E_TEBS_INDEX_19 1658<<3

#define MAC_E_TEBS_INDEX_20 2202<<3

#define MAC_E_TEBS_INDEX_21 2925<<3

#define MAC_E_TEBS_INDEX_22 3884<<3

#define MAC_E_TEBS_INDEX_23 5160<<3

#define MAC_E_TEBS_INDEX_24 6853<<3

#define MAC_E_TEBS_INDEX_25 9103<<3

#define MAC_E_TEBS_INDEX_26 12092<<3

#define MAC_E_TEBS_INDEX_27 16062<<3

#define MAC_E_TEBS_INDEX_28 21335<<3

#define MAC_E_TEBS_INDEX_29 28339<<3

#define MAC_E_TEBS_INDEX_30 37642<<3

#define MAC_E_TEBS_INDEX_31 50000<<3

/*------------------------------------------------------------------------
 If 2ms TTI - Number of status reports to collect are 40 
 If 10ms TTI - Number of status reports to collect are 20 
------------------------------------------------------------------------*/
#define MAC_HS_UL_NUM_REPORTS_IN_STATUS_LOG_PKT                             40
#define MAC_HS_UL_NUM_REPORTS_IN_STATUS_LOG_PKT_10MS_TTI                    20

#define MAC_HS_UL_NUM_MAC_ES_HEADERS_IN_LOG_PKT                            16

#define MAC_HS_UL_NUM_MACI_IS_STATUS_LOG_PKT                                40

#define MAC_HS_UL_CHECK_LOG_STATUS_AFTER_N_CFN_COUNT                          25

#ifdef FEATURE_WCDMA_HS_RACH
#define MAC_HS_RACH_MAX_SAMPLES_MAC_TIMER_LOG_PKT                     40
#define MAC_HS_RACH_MISC_VAL_BSHIFT                                    6
#endif

/*===========================================================================

                     KEY MACROS

===========================================================================*/
#define HARQ_MASK   0xFF

/*===========================================================================

                     KEY MACROS

===========================================================================*/
#define HARQ_MASK   0xFF

/*--------------------------------------------------------------------------
harq_restriction is 8 bit bit-string. The LSB of this bit string corresponds
to the harq_id 0, the MSB corresponds to harq_id 7
Harq_id's can range from 0-7
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAC_D_FLOW_IS_ALLOWED_ON_HARQ(harq_id, harq_restriction) \
                 ((1 << (MAC_HS_UL_MAX_HARQ_ID-harq_id)) & harq_restriction)

/*--------------------------------------------------------------------------

--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAC_D_FLOW_IS_ALLOWED_BY_MUX_LIST(flow_id, mux_list) \
                 ((128 >> flow_id) & mux_list)

/*===========================================================================

                     GENERAL PURPOSE MACROS

===========================================================================*/

/*--------------------------------------------------------------------------
Check for the validity of the TSN
--------------------------------------------------------------------------*/
#define MAC_HS_UL_TSN_IS_INVALID(tsn)\
      (tsn > MAC_HS_UL_MAX_TSN)

/*--------------------------------------------------------------------------
Check on the number of MAC-d flows configured
--------------------------------------------------------------------------*/
#define MAC_HS_UL_NUM_MAC_D_FLOW_IS_INVALID(num_flows)\
      ((num_flows < MAC_HS_UL_MIN_MAC_D_FLOWS) || \
       (num_flows > MAC_HS_UL_MAX_MAC_D_FLOWS))

/*--------------------------------------------------------------------------
Check on the range of the MAC-d flow id
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAC_D_FLOW_ID_IS_INVALID(flow_id)\
       (flow_id > MAC_HS_UL_MAX_MAC_D_FLOW_ID)

/*--------------------------------------------------------------------------
Check on the range for the max number of retransmissions
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAX_NUM_OF_RETX_IS_INVALID(num_retx)\
       (num_retx > MAC_HS_UL_MAC_D_FLOW_MAX_NUM_RETX)

/*--------------------------------------------------------------------------
Check on the MAC-d flow power offset values
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAC_D_FLOW_POWER_OFFSET_IS_INVALID(pwr_offset)\
       (pwr_offset > MAC_HS_UL_MAX_POWER_OFFSET)

/*--------------------------------------------------------------------------
Check to see if the configured number of bits for non-sched data is within
the valid range
--------------------------------------------------------------------------*/
#define MAC_HS_UL_NON_SCHED_DATA_SIZE_IS_INVALID(size)\
       (size > MAC_HS_UL_MAX_NON_SCHED_DATA_SIZE)

/*--------------------------------------------------------------------------
Check to see if the PDU size lies in the valid range.
--------------------------------------------------------------------------*/
#define MAC_HS_UL_MAC_D_PDU_SIZE_IS_INVALID(size)\
        ((size > MAC_HS_UL_MAX_MAC_D_PDU_SIZE) &&\
         (size < MAC_HS_UL_MIN_MAC_D_PDU_SIZE))

/*--------------------------------------------------------------------------
DDI value can't exceed 63.
--------------------------------------------------------------------------*/
#define MAC_HS_UL_DDI_VALUE_IS_INVALID(ddi_val)\
        (ddi_val > MAC_HS_UL_MAX_DDI_VALUE)

/*--------------------------------------------------------------------------
Check on the number of RLC PDU sizes to the DDI values dont exceed 32.
--------------------------------------------------------------------------*/
#define MAC_HS_UL_NUM_RLC_PDU_SIZE_DDI_IS_INVALID(size)\
        (size > MAC_HS_UL_NUM_RLC_PDU_SIZE_DDI)

/*--------------------------------------------------------------------------
Ensure that the E-TFCI table index is either 0 or 1
--------------------------------------------------------------------------*/
#ifdef FEATURE_WCDMA_16_QAM
#define MAC_HS_UL_E_TFCI_TABLE_INDEX_IS_INVALID(index) \
        ((eul_table_ptr->mac_hs_ul_tti_type == E_TTI_2MS)      ?      \
         (index >= L1_EUL_NUM_ETFC_TAB_FOR_2MS) :      \
         (index >= L1_EUL_NUM_ETFC_TAB_PER_TTI))
#else
#define MAC_HS_UL_E_TFCI_TABLE_INDEX_IS_INVALID(index)\
        (index >= L1_EUL_NUM_ETFC_TAB_PER_TTI)
#endif /* FEATURE_WCDMA_16_QAM */

/*--------------------------------------------------------------------------
Ensure that the E-TFCI is in the range of 0 to ntfc
--------------------------------------------------------------------------*/
#define MAC_HS_UL_E_TFC_INDEX_IS_INVALID(etfc_index, ntfc)\
        (etfc_index >= ntfc)

/*--------------------------------------------------------------------------
Ensure that the HARQ Id is between 1-8
--------------------------------------------------------------------------*/
#define MAC_HS_UL_HARQ_ID_IS_INVALID(harq_id)\
        (harq_id > MAC_HS_UL_MAX_HARQ_ID)

#ifdef  FEATURE_MAC_I
#define MAC_I_CHECK_SDU_SEG_NOT_PRESENT(mac_hs_ul_logical_ch_mac_d_flow_mapping, rlc_id) (FALSE == mac_hs_ul_logical_ch_mac_d_flow_mapping[rlc_id].rlc_ul_partial_info.mac_is_sdu_seg_present)
#define MAC_I_MSG_ERROR   0x01
#define MAC_I_MSG_MED     0x02
#define MAC_I_MSG_LOW     0x04
#define MAC_I_MSG_ERR_FATAL 0x08
#define MSG_HIGH_MAC_I  MSG_HIGH
#endif /*   FEATURE_MAC_I */

/*===========================================================================

                     TYPE DEFINITIONS FOR MODULE

===========================================================================*/

#ifdef FEATURE_MAC_I

typedef enum
{
  MAC_NO_TRANSITION = 0,
  MAC_E_TO_I,
  MAC_I_TO_E,
  MAC_E_INVALID_TRANSITION
} mac_e_transition_e_type;

typedef struct
{
  /* 4 Pc of X and 1 Pc of Y Size 
   *          - num_complete_pdu = 4+1 ---> Total = 5, (X,4), (Y, 1)
   * 4 Pc of X and 1 Ps of Z bytes from Y bytes PDU 
   *          - num_complete_pdu = 4+1 ---> Total = 5, (X,4), (Y, 1), partial_present = TRUE, partial_size = Z 
   */
  uint32 num_total_pdu;
  uint32 num_ctrl_pdu;
  uint32 num_retx_pdu;
  uint32 num_new_data_pdu;
  uint32 complete_pdu_idx;
  uint16 num_complete_pdu[MAC_HS_UL_MAX_NUM_RLC_PDUS_PER_LC];
  uint16 complete_pdu_size[MAC_HS_UL_MAX_NUM_RLC_PDUS_PER_LC];
  uint16 retx_sn[MAC_HS_UL_MAX_NUM_RLC_PDUS_PER_LC];
  boolean partial_present;
  uint32 partial_size;
  uint32 total_lc_pdu_size;
} mac_i_flex_pdu_size_chosen;

#endif  /* FEATURE_MAC_I  */

typedef   enum
{
  SINGLE_CARRIER_CONFIG_TYPE =0,
  DUAL_CARRIER_CONFIG_TYPE
}mac_ul_hs_carrier_config_type;

typedef struct
{
/* Number of HARQ processes that are active. MAC will need this for triggering SI */
  uint8     num_harq_active;
  
/*  This identifies the state of the HARQ process used for the upcoming transmission  */
  boolean   current_harq_active;
  
/* This identifies the Active and deactive HARQ process.MAC will need this for HappyBit computation */
  uint8   active_harq_bit_mask;
}mac_hs_ul_harq_active_info_type;


/*-----------------------------------------------------------------------------------------------------
There will be 3 levels of checking. Level 1 takes the highest priority over 2 and 3.
All reasons are grouped and then checked across each group as follows:

1st Level: 
  If any of the following holds true then reason will be set to that 
  given that SG is enough to build at least 1 scheduled data PDU.
     HQR
     HQD
     MUX
     RWS
2nd Level:
     Min(BO, MP, SG)  // BO takes priority over SG

3rd level:
     Min(MP, MSF, MIS, LFC) // These are listed in priority order in case more than one reason applies
---------------------------------------------------------------------------------------------------------*/
typedef struct
{
  boolean        level1_etfci_reason_flag;
  boolean        level2_etfci_reason_flag;
  
  mac_hs_ul_etfci_restriction_reason_e_type   reason;
  mac_hs_ul_etfci_restriction_reason_e_type   level1_etfci_reason;
  mac_hs_ul_etfci_restriction_reason_e_type   level2_etfci_reason;
}log_etfci_restriction_reason_info_type;

/*----------------------------------------------------------------------------
This structure gives a mapping between the priority levels and the number of
logical channels mapped at that priority level and their rlc_id's
----------------------------------------------------------------------------*/

typedef struct
{
  /* Number of logical channels at this priority level */
  uint8                     num_logical_channels;

  /* Start index for the round robin scheduling*/
  uint8                     start_idx;

  /* RLC logical channel buffer identifier (0-20) */
  rlc_lc_id_type            rlc_id[MAX_DED_LOGCHAN_PER_TRCH];

}mac_hs_ul_priority_lc_mapping_info_type;

/*----------------------------------------------------------------------------
This structure gives all the information about a MAC-d flow ID.
----------------------------------------------------------------------------*/

typedef struct
{
  /* TRUE indicates that this MAC-d flow is enabled, FALSE, otherwise */
  boolean                                mac_d_flow_enabled;

  /* E-DCH MAC-d flow power offset */
  uint8                                  power_offset; /* 0..6 (unit in dB) */

  /* E-DCH MAC-d flow maximum number of  retransmissions */
  uint8                                  max_num_of_retx; /* 0..15 */

  /* E-DCH MAC-d flow multiplexing list:
     Indicates, if this is the first MAC-d flow for which PDU's are placed in
     the MAC-e PDU, the other MAC-d flows from which MAC-d PDU's
     are allowed to be included in the same MAC-e PDU.
     Bit 0 is for MAC-d flow 0, Bit 1 is for MAC-d flow 1, …
     Value '1' for a bit means multiplexing is allowed.
     Bit 0 is Most Significant Bit(Left most bit) in the bitstring
     NOTE: The bit that corresponds to the MAC-d flow itself is ignored */
  uint8                                  mux_list; /* BITSTRING */

  /* transmission grant type */
  mac_e_tx_grant_info_e_type             tx_grant_info;

  /* below field is valid only if mac_e_tx_grant_info_e_type
     indicates NON_SCHEDULED */
  mac_non_sched_tx_grant_info_s_type     non_sched_tx_grant;

  /* Harq process restriction valied for only 2ms TTI case */
  uint8                                  harq_restriction;

}mac_hs_ul_mac_d_flow_info_type;

/*----------------------------------------------------------------------------
This structure gives the mapping between the logical channel ID and MAC-d flow
id.
----------------------------------------------------------------------------*/
typedef struct
{
  /* TRUE Indicates if this logical channel is enabled or not */
  boolean                               logical_ch_enabled;

  /* logical channel id sent by the NW - to be used in SI */
  uint8                                 logical_ch_id;

  /* E-DCH MAC-d flow identified */
  uint8                                  mac_d_flow_id; /* 0..7 */

  /* Radio Bearer Identifier (0-31) */
  rb_id_type                             rb_id;

  /* RLC mode (UM or AM) */
  uecomdef_logch_mode_e_type             rlc_mode; /* UM or AM */

  /* Logical channel type (DCCH or DTCH) */
  uecomdef_logchan_e_type                chan_type;

  /* number of valid fields in rlc_size_ddi_info[] */
  /* 0..32, 0 is valid only when action is NOOP or STOP */
  uint8                                  num_rlc_size_ddi_info;

  /* RLC Hdr size = 8 bits for UM and 16 bits for AM */
  uint8                                  rlc_hdr_length;

  /* Indicates if this channel has to be looked into for computing
  scheduling info */
  boolean                                include_in_sched_info;

  /* Priority */
  uint8                                  priority;

#ifdef FEATURE_MAC_I
  rlc_pdu_size_type  rlc_pdu_type;         /* Flexible or Fixed PDU type */
  rlc_li_e_type      ul_li_size;           /* LI size, only for AM       */
  uint16             ul_flex_min_pdu_size; /* Minimum PDU size           */
  uint16             ul_flex_max_pdu_size; /* Minimum PDU size           */
  rlc_ul_partial_info_type              rlc_ul_partial_info;
  
#endif    /* FEATURE_MAC_I */

  /* Mapping of rlc-pdu-size  to ddi. Valid number of fields in following array is
  equal to 'num_rlc_size_ddi_info' */
  mac_e_rlc_size_ddi_info_s_type         rlc_size_ddi_info[MAX_RLC_PDU_SIZE_PER_LOGCHAN];

}mac_hs_ul_logical_ch_mac_d_flow_info_type;

/*----------------------------------------------------------------------------
This structure gives the config parameters for computing the happy bit
and the scheduling information.
----------------------------------------------------------------------------*/
typedef struct
{
  /* Happy bit delay condition */
  uint16                           happy_bit_delay_in_ms; /* in ms */

  /* Periodicity for Scheduling Info - no grant: uint16.
     It can have following values:
               0: indicates that no report(scheduling information) needs to be sent,
       otherwise: periodically send scheduling information as per its value */
  uint16                           no_grant_periodicity_of_si; /* in ms */

  /* Periodicity for Scheduling Info - grant.
     It can have following values:
               0: indicates that no report(scheduling information) needs to be sent,
       otherwise: periodically send scheduling information as per its value */
  uint16                           grant_periodicity_of_si; /* in ms */

  uint8                            power_offset_of_si; /* 0..6 (in dB) */

  uint32                           min_rlc_pdu_size;

  /* Number of logical channels for which reporting has been requested */
  uint16                           num_log_ch;

  /* Identifies the logical channels Id's for which reporting has been requested*/
  rlc_lc_id_type                   rlc_id[MAX_DED_LOGCHAN_PER_TRCH];

}mac_hs_ul_si_happybit_parameters_config_type;

/*----------------------------------------------------------------------------
This structure gives the config parameters related to E-TFCI
----------------------------------------------------------------------------*/
typedef struct
{
  /* Index into the E-TFCI table */
  uint8                             e_tfci_index;

  /* This means E-TFCI in the range 0 - e_tfci_min_set_index is allowed*/
  uint8                             e_tfci_min_set_index;

  /* number of E-TFCI */
  uint32                            ntfc;

  /* The E-TFCI table pointer for the current TTI */
  const  uint16                            *etfci_ptr;

}mac_hs_ul_e_tfci_info_type;

typedef enum
{
  NON_SCHED_GRANT_LC,
  SCHED_GRANT_LC,
  BOTH_NSG_SG_LC
} mac_dchsupa_lc_type_po_e_type;

/*----------------------------------------------------------------------------
This structure stores the various parameters as a result of the power offset
computation. These results will be needed for E-TFCI selection and determining
if SI needs to be sent or not.
----------------------------------------------------------------------------*/
typedef struct
{
  /* Bitstring which identifies the MAC-d flows whose data may be present
  in the current MAC-e PDU */
  uint8                     mux_list;
  
  /* Indicates the power offset that will be sent to L1 after E-TFCI selection */
  uint8                     power_offset;
}mac_hs_ul_pwr_offset_profile;


typedef struct
{
 /* index1-PO Secondary , Index2- PO Primary, Index3-PO NSG */
 mac_hs_ul_pwr_offset_profile   po_profile[3];
 
 /* Total Scheduled Buffer Status*/
 uint32                    sched_buf_status;
 
 /* Total Non-Scheduled Buffer Status*/
 uint32                    non_sched_buf_status;
 
 /* Gives the highest priority scheduled flow's PDU size */
 uint32                    min_sched_rlc_pdu_size;

}mac_dchsupa_po_compn_result_type;


typedef struct
{

  #ifdef FEATURE_WCDMA_DC_HSUPA
  /* TRUE : DC_HSUPA is active at Layer 1; FALSE: DC_HSUPA is configured, not active*/
  boolean   is_dc_hsupa_active;
  #endif
  
  /* TRUE indicates if SI needs to be sent or not */
  boolean   si_send_status;
  
  /*True Indicates new Tx, False indicates Re-Tx */
  boolean new_tx;
  
  /* TRUE indicates if the SI is sent alone without higher layer data */
  boolean   si_sent_alone;
  
  /* Bitstring which identifies the MAC-d flows whose data may be present
  in the current MAC-e PDU */
  uint8                     mux_list;
  
  /* Indicates the power offset that will be sent to L1 after E-TFCI selection */
  uint8                     power_offset;
  
  /* Scheduled Buffer Status after considering include_in_si flag - In BITS */
  uint32                    sched_buf_status;

  /* Total Scheduled Buffer Status - In BITS */
  uint32                    total_sched_buf_status;

  /* Total Non-Scheduled Buffer Status - In BITS */
  uint32                    non_sched_buf_status;

  /* Gives the highest priority scheduled flow's PDU size */
  uint32                    min_sched_rlc_pdu_size;
  
  mac_hs_ul_harq_active_info_type            harq_l1_info;
  
  /*reason for selecting the PO as 0xFF*/
  mac_hs_ul_etfci_restriction_reason_e_type  mac_hs_ul_pwr_offset_reason;

}mac_hs_ul_pwr_offset_compn_result_type;

/*----------------------------------------------------------------------------
This structure stores the SI timer blocks and a boolean to indicate
if any timer is active or not.
----------------------------------------------------------------------------*/
typedef struct
{
  /* Timer to report SI in case of NO Grant/Low Grant/Periodic Grant condition */
  rex_timer_type                        mac_hs_ul_si_no_grant_timer_blk;
  rex_timer_type                        mac_hs_ul_si_grant_timer_blk; 
}mac_hs_ul_si_tmr_info_type;

/*----------------------------------------------------------------------------
This enum will identify what state the UE is in currently. This information
will be needed so that we dont keep triggering SI when we are on zero sg
and no timers are configured.
----------------------------------------------------------------------------*/
typedef enum
{
  MAC_HS_UL_SI_STATE_INVALID=0,
  /* There is no initial serving grant */
  MAC_HS_UL_SI_STATE_ZERO_SG,
  /* There is initial serving grant */
  MAC_HS_UL_SI_STATE_NON_ZERO_SG

}mac_hs_ul_si_state_e_type;

typedef struct
{
  /* State in the previous TTI */
  mac_hs_ul_si_state_e_type prev_state;
  /* State in the curr TTI */
  mac_hs_ul_si_state_e_type curr_state;

}mac_hs_ul_si_state_type;

typedef struct
{
  /* To start the Timer with rex_set_timer()*/
  boolean       start_timer;
  /* To keep track of timer if it is active or not */
  boolean       timer_active;
  /* To keep track if the timer is expired */
  boolean       timer_expired;
  /* Configured timer value stored here */
  uint16        config_count;
}mac_hsupa_si_tmr_count_info_type;

typedef struct
{
  boolean                             si_triggered;
  boolean                             si_padded;
  boolean                             si_sent_alone;
  boolean                             mac_trig_si_qta_end;
  /* Highest priority data that is obtained from RLC in the previous transmission */
  uint8                               prev_priority_lc_data;
  /* Indicates If in this TTI data from a high priority LC arrived - valid only in NSG state */
  boolean                             higher_priority_data_arrived;

  mac_hs_ul_si_state_type             si_state;
  mac_hs_ul_si_tmr_info_type          si_timer_blk;
  
  /* TSI_G */  
  mac_hsupa_si_tmr_count_info_type    si_grant_timer;
  /* TSI_NG */
  mac_hsupa_si_tmr_count_info_type      si_no_grant_timer;
  /* TSI_LG */
  mac_hsupa_si_tmr_count_info_type      si_low_grant_timer;
}mac_hsupa_si_tmr_info_type;

/*----------------------------------------------------------------------------
This structure stores the E-TFC payload in bits, Power Offset and the
serving grant that was used in the current TTI.
----------------------------------------------------------------------------*/
typedef struct
{
  /* Indicates the power offset that was used in the current transmission */
  uint8         power_offset;

  /* Indicates the maximum number of retransmissions */
  uint8         max_num_retx;

  /* Indicates the E-TFCI selected for this TTI */
  uint8         etfci;

  /* Serving grant available in the current tti*/
  uint32        serving_grant_payload_txed;

  /* Total payload that can be transmitted based on the available UE Tx power */
  uint32        total_ue_tx_pwr_payload;

  /* This was the serving grant available from L1 */
  uint32        serving_grant_bits;

  /* TRUE indicates SI was sent with some higher layer data */
  boolean       si_sent_with_data;

}mac_hs_ul_prev_harq_tx_info_type;

/*----------------------------------------------------------------------------
This structure stores results of the E-TFCI selection.
----------------------------------------------------------------------------*/
typedef struct
{
  /* TRUE indicates if SI needs to be sent or not */
  boolean   si_send_status;
  
  /* TRUE indicates if SI is padded in a TB */
  boolean   si_padded;
  
  /* TRUE indicates if the special SI header has to be included */
  boolean   special_si_header;

  /* TRUE indicates if the SI is sent alone without higher layer data */
  boolean   si_sent_alone;

  /* TRUE indicates that start the no grant periodicity timer. This
  timer is actually started when the E-TFCI selection is done. */
  boolean   si_start_no_grant_tmr;

  /* TRUE indicates that start the grant periodicity timer. This
  timer is actually started when the E-TFCI selection is done. */
  boolean   si_start_grant_tmr;

  /* Indicates the amount of bits to be padded */
  uint32    padding;

}mac_hs_ul_etfci_seln_results_type;

typedef struct
{
  boolean   cur_harq_active;
  mac_hs_ul_si_state_e_type   si_curr_state;
  uint8     cur_rrc_sched_harq_restriction;
  uint8     cur_active_harq_bit_mask;
  uint8     etfc_max_sf_index;
  uint32    cur_serving_grant;
  
  #ifdef  FEATURE_WCDMA_DC_HSUPA
  uint8     prim_car_rrc_harq_restriction;
  uint8     prim_car_active_harq_bit_mask;
  uint32    prim_car_serving_grant;
  
  uint8     sec_car_rrc_harq_restriction;
  uint8     sec_car_active_harq_bit_mask;
  uint32    sec_car_serving_grant;
  #endif
  
  /* Actual amount of data transmitted this tti */
  uint32    scheduled_data_txed;
  
  /* E-TFCI selected this tti. In case of re-transmission this will
  refer to the E-TFCI chosen for the previous transmission for the
  same HARQ process */
  uint32    etfci_bits;
  
  /* Maximum bits that can be Tx this TTI based on available UE tx power */
  uint32    ue_total_tx_pwr_payload;
}mac_hs_ul_happy_bit_params_type;

/*----------------------------------------------------------------------------
This structure stores information about the DDI and the corresponding
RLC PDU size and the number of PDUs that has to be built for each
logical channel.
----------------------------------------------------------------------------*/
typedef struct
{
#ifdef FEATURE_MAC_I 
  /* This field indicates the length of the mac_e_hdr_info. 
   * This is needed in case of MAC-I, as multiple (LC,L) combinations possible for same LC
   * This is NOT needed in case of MAC-e, as only one (DDI,N) combination exists for a given LC
   */
  uint16                    mac_e_hdr_info_len;
#endif
  /* Indicates the DDI that has to be attached to the MAC header */
  uint8                     mac_e_hdr_info[MAX_MAC_E_HDR_SIZE_BYTES];
  /* Information about the number of PDUs and the PDU size for each logical channel */
  rlc_ul_frame_format_type  rlc_pdu_info;

}mac_hs_ul_rlc_pdu_ddi_info_type;

#ifdef FEATURE_WCDMA_DC_HSUPA
typedef struct
{
  boolean                      sec_carrier_active;
  boolean                      tsn_field_extn_flag;
  uint8                        e_tfci_min_set;
  uint8                        harq_restriction;
  l1_eul_carr_idx_enum_type    l1_carrier_indx;
  mac_sec_eul_action_e_type    eul_action;
  uint32                       non_sched_buf_status;
  uint32                       total_non_sched_grant;
  /* Holds PO for Non-sched mac-d flows when BOTH carriers are active */
  uint8                        non_sched_harq_po;
  /* Used to save HB computation params on secondary carrier */
  mac_hs_ul_happy_bit_params_type  hb_params;
  /*Holds Happy Bit on Sec carrier -  If predetermined to be TRUE during
    secondary carrier ETFCI selection */
  boolean                        hb_on_sec_carr;
  uint32                         hb_total_buf_status;
  uint32                         hb_pdu_size;
}mac_hs_ul_sec_carrier_info_type;
#endif /*  FEATURE_WCDMA_DC_HSUPA  */

/*----------------------------------------------------------------------------
This structure will contain the loop limits based on the configuration
that MAC gets
----------------------------------------------------------------------------*/

typedef struct
{
  /* Loop start index when looping over priority*/
  uint16    min_mlp;

  /* Loop end index when looping over priority*/
  uint16    max_mlp;

  /* Loop start index when looping over logical channels*/
  uint16    min_rlc_id;

  /* Loop end index when looping over logical channels*/
  uint16    max_rlc_id;

}mac_hs_ul_loop_limits_cfg;

typedef  struct
{
  /* Bitstring : To identifies MAC-d flows whose data presentin in current MAC-e PDU */
  uint8                     mux_list;
  
  /* Indicates the power offset that will be sent to L1 after E-TFCI selection */
  uint8                     power_offset;
  
  /* Total Scheduled Buffer Status - In BITS */
  uint32                    sched_buf_status;
  
  /* Gives the highest priority scheduled flow's PDU size */
  uint32                    min_sched_rlc_pdu_size;
  
   /* Reason for selecting Invalid PO - 0xFF */
   mac_hs_ul_etfci_restriction_reason_e_type  pwr_offset_reason;
}mac_hs_ul_store_po_result_info_type;

typedef struct
{
  uint8    carrier_index;
  uint8    harq_id;
  uint8    etfc_idx;
  uint8    etfc_min_set_index;
  uint8    etfc_max_sf_index;
  uint32   remaining_avl_payload;
  uint32   scheduled_bits_available;
  uint32   total_ns_payload;
  uint32   size_of_si;
  #ifdef FEATURE_HSUPA_NEW_ETFCI_ALGORITHM
  uint32   quantization_loss;
  #endif
  uint32   mac_e_pdu_size;
  mac_hs_ul_etfci_seln_results_type      etfci_si_result_info;
  mac_hs_ul_store_po_result_info_type    po_result_info;
  mac_hs_ul_happy_bit_params_type        happybit_param_info;
  l1_eul_serving_grant_update_info_type  *l1_sg_info_ptr;
  mac_hs_ul_rlc_pdu_ddi_info_type        *bfi_rlc_info_ptr;
  mac_hsupa_si_tmr_info_type             *si_timer_info_ptr;
}mac_hs_ul_etfc_common_info_type;

typedef struct
{
  boolean    pdu_built_flag;
  boolean    tmp_log_harq_r;
  boolean    tmp_log_harq_d;
  boolean    tmp_log_harq_mux;
  boolean    tmp_log_rws;
} mac_hs_ul_etfci_reason_info_type;

#if defined( FEATURE_L1_LOGGING_ENABLED ) && defined( FEATURE_HSUPA_L1_LOGGING_ENABLED)

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct 
#endif /* __GNUC__ */
{
     /*NUM_POW_LIMITED:Number of TTIs transmission is limited by power*/
 uint8 num_tx_limited_by_pwr;

 /*NUM_SG_LIMITED:Number of TTIs transmission is limited by serving grant*/
 uint8 num_tx_limited_by_sg;

 /*NUM_BUF_LIMITED:Number of TTIs transmission is limited by available buffer
   status*/
 uint8 num_buf_limited;

 /*NUM_HARQ_RESTRICTION: Number of TTIs transmission is limited by HARQ restriction*/
 uint8  num_harq_restriction;

 /*NUM_HARQ_DEACT: Number of TTIs transmission is limited by HARQ Deactivated*/
 uint8  num_harq_deactivated;

 /*NUM_IRQ_FC: Number of TTIs transmission is limited by IRQ FC*/
 uint8  num_irq_fc;

 /*NUM_MUX_RESTRICTION: Number of TTIs transmission is limited by MUX restriction*/
 uint8 num_mux_restriction;

 /*NUM_RLC_WIN_STALL: Number of TTIs transmission is limited by RLC window stall*/
 uint8  num_rlc_win_stall;

 /*NUM_MAX_SF_CFG: Number of TTIs transmission is limited by MAX SF cfg*/
 uint8 num_max_sf_cfg;

 /*NUM_MIN_SET: Number of TTIs transmission is limited by MIN set*/
 uint8 num_min_set;

 /*NUM_BUF_EMPTY:Number of TTIs both sched + non-sched buffers were empty*/
 uint8 num_buf_empty;

 /* NUM_S_BUF_EMPTY:Number of TTIs total scheduled buffer was empty*/
 uint8 num_sched_buf_empty;

 /*NUM_NS_BUF_EMPTY:Number of TTIs total non-scheduled buffer was empty*/
 uint8 num_non_sched_buf_empty;

 /*SUM_S_BUF:SUM of schedule buffer bytes status for newtx  divided by 64 is sent
   to diag and Mean scheduled buffer status in bytes (value of 0xFFFF means
   more than 65535 bytes) is displayed
 */
 uint32 sum_sched_buf_bytes;

 /*SUM_NS_BUF:SUM of non schedule buffer bytes status for newtx  divided by 64 is
   sent to diag and Mean non-scheduled buffer status in bytes (total over all NS flows)
   is displayed*/
 uint32 sum_non_sched_buf_bytes;

 /* NUM_SI:Number of SIs sent */
 uint8  num_si_sent;
}mac_hs_ul_l1_mac_stats_info_struct_type;

/*----------------------------------------------------------------------------
This structure stores the values needed for logging the 0x4311 Log packet.
The accumulated log packet is logged at L1. MAC provides all the following
information to L1.
----------------------------------------------------------------------------*/

typedef struct
{
/*MAC_RESETS    1   Number of MAC-e resets*/
uint8    num_mac_e_resets;

/*Sum of the number of transmitted scheduled bits per TTI, divided by 64, counting only new transmissions*/
uint32    sum_sched_bits_txed;

/*Sum of the number of transmitted non-scheduled bits per TTI, divided by 64, counting only new transmissions*/
uint32    sum_non_sched_bits_txed;

/*Sum of the number of bits that could have been transmitted per TTI based on available buffer status (scheduled flows only) divided by 64,
counting only new transmissions NOTE While summing up over all TTIs: For all TTIs for which the buffer status is greater than the max TB size,
use the max TB size instead of the actual buffer status in the summation.*/
uint32   sum_buf_bits;

/*mac continuous data as in the 4311 logpkt*/
mac_hs_ul_l1_mac_stats_info_struct_type continuous_mac_info;

}mac_hs_ul_log_mac_stats_struct_type;
#ifdef FEATURE_WCDMA_HS_RACH

typedef enum
{
 MAC_HS_RACH_COLLISION_RESOLUTION_PHASE = 0,
 MAC_HS_RACH_COLLISION_RESOLVED_PHASE,
 MAC_HS_RACH_COLLISION_INVALID_PHASE
}mac_hsrach_collision_phase_e_type;


typedef struct
{
  boolean  ernti_hdr_included;
  boolean  tebs0_si_only_triggered;
  #ifdef FEATURE_WCDMA_EFACH_MSIM
  boolean  hsrach_force_trig_si0;
  #endif
  boolean  hsrach_dxch_tebs0_flag;
  boolean  lc_include_in_si_current_tti;
}mac_hs_rach_etfci_common_info_type;


typedef struct
{

  /*additional-E-DCH-TransmitBackoff   INTEGER   (0 .  . 15 )*/
  uint8                                add_e_dch_tx_backoff_config_value;
  
  /*Maximum E-DCH resource allocation for CCCH, values 8, 12, 16, 24, 32, 40, 80, 120 In terms of TTIs*/
  boolean                             ccch_timer_configuerd_flag;
  l1_mac_hsrach_eul_timer_status_enum ccch_timer_status;
  uint8                               max_e_dch_res_alloc_ccch_config_value;
  uint8                               max_e_dch_res_alloc_ccch_current_count;


  /*Maximum period for collision resolution phase, Integer (8..24) In terms of TTIs.*/
  boolean                             agch_coll_resol_configuerd_flag;
  l1_mac_hsrach_eul_timer_status_enum agch_coll_resol_timer_status;
  uint8                               agch_max_collision_resol_config_value;
  uint8                               agch_max_collision_resol_current_count;
 
  /*E-DCH transmission continuation back off, value Enumerated (0, 8, 16, 24, 40, 80, 120, infinity)
  *In terms of TTIs. If set to "infinity", implicit common E-DCH resource release is disabled.
  */
  boolean                              tebs_zero_timer_configuerd_flag;
  l1_mac_hsrach_eul_timer_status_enum  tebs_zero_timer_status;
  l1_e_dch_tx_cont_backoff_enum_type   edch_tx_tebs_zero_timer_config_value; 
  uint8                                edch_tx_tebs_zero_timer_current_count;
  
} mac_hs_ul_hsach_timers_type;

typedef struct
{
  boolean                           hs_rach_active;
  uint8                             hs_rach_ccch_asc;  
  boolean                           primary_e_rnti_present;
  uint16                            primary_e_rnti;
  uint16                            backoff_val;
  uint8                             t2_timer_count;
  uint32                            ccch_buff_status;/* Total CCCH Buffer Status in Bytes*/
  uint32                            dxch_buff_status;/* Total CCCH Buffer Status in Bytes*/
  
  mac_ul_rach_txcntl_config_type      hs_rach_cntl_info;
  
  mac_hsrach_collision_phase_e_type   mac_hsrach_collision_phase_info;
  
  l1_mac_hs_rach_trans_type_enum_type  mac_hsrach_logch_tx_type;
  
  rlc_lc_id_type                       mac_hsrach_ccch_rlc_id;

  mac_hs_ul_hsach_timers_type          mac_hsrach_timers_info;
  
  mac_hs_rach_etfci_common_info_type   mac_hsrach_etfci_common_info;

  boolean                                     mac_hsrach_edch_rsrc_rel_cause_valid;
  l1_mac_hs_rach_edch_rsrc_rel_cause_enum_type mac_hsrach_edch_rsrc_rel_cause;  
  
}mac_ul_hs_rach_cfg_info_type;

#endif /*FEATURE_WCDMA_HS_RACH*/

#endif
/*--------------------------------------------------------------------------
 MAC HS Header Type
 --------------------------------------------------------------------------*/
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  /*Data Description Indicator. Range: 0..62*/
  uint8 ddi;

  /* Transmit Sequence Number. Range: 0..63*/
  uint8 tsn;

  /* N = number of MAC-d PDUs in this MAC-es PDU. Range: 0..63 */
  uint16 npdus;

}mac_hs_ul_log_hdr_type;


#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  /*Indicates the type of transmission 0: new transmission 1: retransmission
  2: no transmission */
  uint8         tx_type;

  /* ETFCI used this tti */
  uint8         etfci;

  /*Whether mobile is happy (1) or unhappy (0) with the current SG*/
  uint8         happy_bit;

  /* Indicates whether the next fields related to SI are present
  0: SI not present1: SI present */
  boolean       si_present;

  /*Whether SI was triggered by buffer status or periodic trigger
  0: SI was not triggered by buffer status or periodic trigger
  1: SI was triggered due to either buffer status or periodic trigger*/
  boolean       si_triggered;

  /*Whether SI was added due to available padding 0: SI was not appended
  due to available 1: SI was appended due to available */
  boolean       si_padding;

  /* HLID, TEBS, HEBS,.. */
  uint8      tebs;
  uint8      uph;
  uint8      hlid;
  uint8      hlbs;

}mac_hs_ul_log_hdr_ctrl_info_type;

/*--------------------------------------------------------------------------
 Header info per TTI
 --------------------------------------------------------------------------*/
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{

  /*Number of MAC-es headers in this MAC-e PDU. Range: 0..62*/
  uint8                                 num_mac_es_hdrs;

  /*information related to si */
  mac_hs_ul_log_hdr_ctrl_info_type      ctrl_info;

  /* Header data */
  mac_hs_ul_log_hdr_type                hdr_data[MAC_HS_UL_NUM_MAC_ES_HEADERS_IN_LOG_PKT];

}mac_hs_ul_log_header_info_per_tti_type;

/*--------------------------------------------------------------------------
 Header info per TTI
 --------------------------------------------------------------------------*/
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  /*NUM_SAMP        Number of MAC-e headers in this log packet. Range: 0..50*/
  uint8         num_samples;

  /* TTI 0 - 10ms, 1 - 2ms */
  uint8         tti;

  /*START_FN        Starting EUL Frame number or Subframe number for E-DPCH.
  Range: 0..255. Frame number if TTI = 10ms Range: 0..1279. Subframe number if TTI = 2ms*/
  /*START_HARQ_ID       Indicates which HARQ process ID corresponds to the
  first frame or subframe number in this log packet. Range: 0..7 */
  uint16        start_fn;

  uint8         harq_id;

  /* ETFCI Table IDx */
  uint8         etfci_table_idx;

  /* dirty_bit is uesd to make sure that there will be no over writting of the log samples */
  boolean                                       dirty_bit;

  /* Complete header information for all samples*/
  mac_hs_ul_log_header_info_per_tti_type    hdr_info[MAC_HS_UL_NUM_REPORTS_IN_STATUS_LOG_PKT];

}mac_hs_ul_log_header_uncompressed_pkt_type;

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  //Size of Mac-is SDU in the Mac-is PDU (choosen PDU size)
  uint16        maci_sdu_size;
  
  /* 0000 to 1101 : Logical Channel ID 1 to 14
    * 1110         : Identification of CCCH (SRB0)
    *1111         : Identification of E-RNTI being included.
    */
   uint8        logch_id;
  
  /* Number of consecutive Mac-is SDU’s of given SDU_SIZE of Mac-is PDU(range is 0 to 63) */ 
  uint8 n_sdu;
  
  /* Flag bit to indicate if any more fields are present in Mac-i header.
    * 0 : Followed by another Mac-i header
    * 1 : Followed by Mac-is PDU
  */
  boolean       flag_bit;
  
  /*Segmentation Type: 0- PC, 1 - PS, 2-PM, 3-PE */
  uint8    ss;
  
  /* Transmit Sequence Number. Range: 0..63*/
  #ifdef FEATURE_WCDMA_DC_HSUPA
  uint16         tsn;
  #else
  uint8         tsn;
  #endif
  
  /* 0 : Primary ERNTI (P), 1 : Secondary ERNTI (S) */
  uint8         ernti;
  
}mac_hs_ul_maci_hdr_logch_type;

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  
  /*Number of MAC-es headers in this MAC-e PDU. Range: 0..62*/
  uint8                               num_mac_i_hdrs;
  
  /*information related to si */
  mac_hs_ul_log_hdr_ctrl_info_type    ctrl_info;
  
  /* Available payload in bits summed over all logical channels configured on scheduled Mac-d flows.
   * The value 65535 means available  payload is greater or equal to 65536 bits.
   */
  uint16    sched_buf_status;
  
  /* Available payload in bits summed over all logical   channels configured on non-scheduled Mac-d flows.
   * The value 65535 means   available payload is greater or equal to 65536 bits.
   */
  uint16    non_sched_buf_status;
  
  /*Final sum of non-scheduled payloads after E-TFC selection. Range: 0..20000 */
  uint16    ns_payload;
  
  /*Final sum of scheduled payloads after E-TFC selection. Range: 0..20000 */
  uint16    sched_payload;
  
  uint8     carrier_index;
  uint8     carrier_config_type;
  uint16    l1_sup_payload;
  uint16    l1_sg_payload;

  
  uint8     etfci_restricted_reason;
  
}mac_hs_ul_maci_pkt_per_tti_type;

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  /*Number of MAC-I headers in this log packet. Range: 0..50*/
  uint8         log_pkt_num_samples;

  /* Total number of Cont case samples */
  uint8         num_continue;
  
  /* TTI 0 - 10ms, 1 - 2ms */
  uint8         tti;
  
  /* Starting EUL Frame number or Subframe number for E-DPCH.
   *  Range: 0..255. Frame number if TTI = 10ms
   * Range: 0..1279. Subframe number if TTI = 2ms
   */
  uint16        start_frame_num;
  
  /* Indicates which HARQ process ID corresponds to the  first frame 
   * or subframe number in this log packet. Range: 0..7 
   */
  uint8         harq_id;
  
  /* ETFCI Table IDx */
  uint8         etfci_table_idx;
  
  /* RRC State Indicator- 0 : CELL_DCH, 1 : CELL_FACH,2 : IDLE,3 : N/A */
  uint8         rrc_state;
  
  uint8         log_pakt_version;
  
  /* dirty_bit is uesd to make sure that there will be no over writting of the log samples */
  boolean       dirty_bit;
  
  uint8   num_tti_logged;
  
  mac_hs_ul_maci_hdr_logch_type      maci_logch_hdr_info[MAC_HS_UL_NUM_MACI_IS_STATUS_LOG_PKT + 1];
  
  mac_hs_ul_maci_pkt_per_tti_type    maci_per_tti_info[MAC_HS_UL_NUM_MACI_IS_STATUS_LOG_PKT];
  
}mac_hs_ul_maci_logpkt_uncompressed_pkt_type;

typedef struct
{
  boolean dcupa_active;

  /* Indicates which HARQ process ID corresponds to the  first frame 
   * or subframe number in this log packet. Range: 0..7 
   */
  uint8         harq_id;

  mac_hs_ul_maci_pkt_per_tti_type maci_per_tti_info_record;

  mac_hs_ul_maci_hdr_logch_type   maci_hdr_info_record[MAC_HS_UL_MAX_NUM_RLC_PDUS_PER_LC];
  
}mac_hs_ul_maci_log_pkt_record_per_carrier_type;

/*=========================================================================

  MAC HS PDU STTATUS LOG PACKET - LOG CODE 4323

========================================================================*/

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  /*SI_TRIG     Whether SI was triggered by buffer status or periodic trigger*/
  /* 0: SI was not triggered by buffer status or periodic trigger
  1: SI was triggered due to either buffer status or periodic trigger*/
  uint8     si_trigger;

  /*SI_PAD      Whether SI was added due to available padding
  0: SI was not appended due to available
  1: SI was appended due to available */
  uint8     si_pad;

  /*HAPPY_BIT       Whether mobile is happy (1) or unhappy (0) with the current SG*/
  boolean   happy_bit;

  /*TX_TYPE     Indicates the type of transmission
  0: new transmission
  1: retransmission
  2: no transmission */
  uint8     tx_type;

  /*ETFCI       Final E-TFCI selected. Range: 0..127 */
  uint8     etfci;

    /*  S_BUF_STATUS  2   Available payload in bits summed over all logical
  channels configured on scheduled Mac-d flows. The value 65535 means available
  payload is greater or equal to 65536 bits */
  uint16    sched_buf_status;

  /*NS_BUF_STATUS   2   Available payload in bits summed over all logical
  channels configured on non-scheduled Mac-d flows. The value 65535 means
  available payload is greater or equal to 65536 bits.*/
  uint16    non_sched_buf_status;

}mac_hs_ul_log_etfci_status_ctrl_info_per_tti_type;


/*--------------------------------------------------------------------------
E-TFCI Info per tti
--------------------------------------------------------------------------*/
#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{

  /*L1_SUP_PAYLOAD  2   Max supported payload per L1 E-TFC restriction
  calculated in L1. Range: 0..20000 */
  uint16    l1_sup_payload;

  /*    2   SG payload calculated in L1. Range: 0..20000 */
  uint16    l1_sg_payload;

  /*NS_PAYLOAD  2   Final sum of non-scheduled payloads after
  E-TFC selection. Range: 0..20000 */
  uint16    ns_payload;

  /*S_PAYLOAD   2   Final sum of scheduled payloads after E-TFC
  selection. Range: 0..20000 */
  uint16    sched_payload;

}mac_hs_ul_log_etfci_status_pkt_per_tti_type;

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
  uint16                                        num_samples;
  /* TTI 0 - 10ms, 1 - 2ms */
  uint8                                         tti;

  /*START_FN        Starting EUL Frame number or Subframe number for E-DPCH.
  Range: 0..255. Frame number if TTI = 10ms Range: 0..1279. Subframe number if TTI = 2ms*/
  /*START_HARQ_ID       Indicates which HARQ process ID corresponds to the
  first frame or subframe number in this log packet. Range: 0..7 */
  uint16                                        start_fn;

  uint8                                         harq_id;

/* dirty_bit is uesd to make sure that there will be no over writting of the log samples */
  boolean                                       dirty_bit;

  /* ETFCI Table IDx */
  uint8                                         etfci_table_idx;

  mac_hs_ul_log_etfci_status_ctrl_info_per_tti_type    ctrl_info[MAC_HS_UL_NUM_REPORTS_IN_STATUS_LOG_PKT];

  mac_hs_ul_log_etfci_status_pkt_per_tti_type   etfci_status_info[MAC_HS_UL_NUM_REPORTS_IN_STATUS_LOG_PKT];

} mac_hs_ul_log_etfci_status_uncompressed_pkt_type;

#ifdef FEATURE_WCDMA_HS_RACH

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
 /* CFN/SuFN value of each sample */
  uint16        cfn_sfn;

  /* TEBS buffer value for CCCH (in bytes) */
  uint8          ccch_tebs_val;

  /*  Bit 0     : Indicates if EDCH resource is acquired by UE or not
       Bit 1-7  : Indicates the countdown timer value for Maximum
                      EDCH resource allocation for CCCH data (in TTIs)  */
  uint8          rsrc_avail_ccch_timer_val;

  /* Indicates the countdown timer value for Maximum period 
      for Collision Resolution (in TTIs) */
  uint8          agch_coll_resol_timer_val;

  /* indicates the countdown timer value for TB timer (in TTIs) */
  uint8          tb_timer_val;

  /* TEBS buffer value for DXCH (in bytes) */
  uint8          dxch_tebs_val;
  
  /*  Bit 0     :  indicates if SI0 is padded or independent
      Bit 1     :  indicates if AGCH is decoded within Maximum period for Collision Resolution
      Bit 2     :  indicates if MAC-EHS data is decoded on the DL or not, if yes then reset the TB timer 
      Bit 3     :  if EDCH data transmission is ongoing or not
      Bit 4     :  indicates if TB timer is frozen
      Bit 5     :  indicates if Force SI-0 is triggered
      Bit 6-9   :  indicates the cause of EDCH resource release 
      Bit 10-15 :  Reserved
  */      
  uint16         misc_vals;  
}mac_hs_ul_hsrach_mac_timer_info_type;

#ifdef __GNUC__
typedef PACKED struct PACKED_POST
#else /* __GNUC__*/
typedef PACKED struct
#endif /* __GNUC__ */
{
 uint8          log_pkt_version;
 
 /*Number of MAC timer samples in this log packet. Range: 0..40*/
 uint8         log_pkt_num_samples;
 
 /* Starting EUL Frame number or Subframe number.
  *  Range: 0..255. Frame number if TTI = 10ms
  * Range: 0..1279. Subframe number if TTI = 2ms
  */
 uint16        start_frame_num;
 
 mac_hs_ul_hsrach_mac_timer_info_type hsrach_mac_timer_info[MAC_HS_RACH_MAX_SAMPLES_MAC_TIMER_LOG_PKT];

}mac_hs_ul_hsrach_mac_timer_log_pkt_type;
#endif

#ifdef FEATURE_DATA_RAVE_SUPPORT 
typedef struct
{
  rex_timer_type   mac_ul_rave_timer_blk;     /* timer block for RAVE Timer */
} mac_ul_rave_tmr_info;

/* ENUM for Alpha Params for FIR filter */
typedef enum
{
  MAC_RAVE_FIR_MIN_ALPHA = 0,
  MAC_RAVE_FIR_USE_ALPHA = 90,
  MAC_RAVE_FIR_MAX_ALPHA = 100,
  MAC_RAVE_FIR_INV_ALPHA = 0xFF
} e_alpha_enum_type; 

/* Confidence reporting Enum for RAVE */
typedef enum
{
  CAPACITY_CONF_RST = 0,
  CAPACITY_CONF_LOW = 1,  
  CAPACITY_CONF_HGH = 7,
  CAPACITY_CONF_INV = 0xFF
} e_conf_enum_type;

/* Intializing Rave params based on below enum */
typedef enum
{
  RAVE_EUL_INIT     = 0,
  RAVE_EUL_RECONFIG = 1,
  RAVE_DISABLE      = 2  
} e_rave_init_enum_type;

typedef struct
{
  boolean              enabled_at_w;            /* Rave enabled at - W. Default - TRUE                 */
  boolean              enabled_by_ds;           /* RAVE enabled or disabled from higher layer          */  
  boolean              ps_on_eul;               /* If PS RABS are on EUL                               */
  rb_id_type           ps_highest_prio_rb_id;   /* PS RAB with highest priority                        */
  e_alpha_enum_type    alpha;                   /* Scaling parameter for FIR filter                    */
  uint32               intvl_conf;              /* Configured periodicity by DS                        */
  uint32               agg_time;                /* Aggregated time for L2-TX/ReTx                      */
  uint32               agg_capacity;            /* 1) - Aggregated Capacity in the current period
                                                   2) - Stores the smoothened filter o/p given to DS   */
  uint32               total_capacity;          /* Total capacity upto the start of current period     */
  mac_ul_rave_tmr_info rave_tmr_blk;            /* RAVE timer - intvl_conf value                       */
} mac_eul_rave_param_type;
#endif

typedef struct
{
  uint16                           mac_hs_ul_etfci_log_pkt_cnt;
  uint16                           mac_hs_ul_hdr_log_pkt_cnt;

  /*  set the 0x4323 and 0x4322 log packet status  */
  boolean                          mac_hs_ul_etfci_log_pacekt_status;
  boolean                          mac_hs_ul_hdr_log_pacekt_status;
  boolean                          mac_hs_ul_log_etfci_flag;
  boolean                          mac_hs_ul_log_hdr_flag;
  boolean                          maci_is_hdr_continues;

/*  Number of status reports collected (If 2ms then max = 40 else max =20)  */
  uint16                           mac_hs_ul_max_status_pkt_cnt;
  uint8                     mac_hs_ul_log_start_harq_id;
  
#ifdef FEATURE_MAC_HS_UL_QXDM_LOGGING
  mac_hs_ul_log_header_uncompressed_pkt_type        *mac_hs_ul_log_pkt_ptr;
  mac_hs_ul_log_header_info_per_tti_type            *mac_hs_ul_log_hdr_ptr;
#ifdef FEATURE_MAC_I

  boolean                          mac_hs_ul_maci_is_log_pacekt_status;
  boolean                          mac_hs_ul_maci_is_log_pacekt_enabled;
  boolean                          mac_hs_ul_maci_current_log_buf_flag;
  uint16                           mac_hs_ul_maci_is_newTx_ReTx_dTx_cnt;
  uint16                           mac_hs_ul_maci_is_hdr_cnt;

  /*   MAC-I_IS Log Packet  varaiables    */
  mac_hs_ul_maci_logpkt_uncompressed_pkt_type    mac_hs_ul_maci_is_log_packet_info;
  mac_hs_ul_maci_logpkt_uncompressed_pkt_type    mac_hs_ul_maci_is_prev_log_packet_info;
  mac_hs_ul_maci_logpkt_uncompressed_pkt_type    *mac_hs_ul_maci_is_log_packet_info_ptr;
  mac_hs_ul_maci_hdr_logch_type                  *maci_logch_hdr_info_log_ptr;
  mac_hs_ul_maci_pkt_per_tti_type                *maci_per_tti_info_log_ptr;

  mac_hs_ul_maci_log_pkt_record_per_carrier_type*  maci_log_record_per_carr; /* Global Record for 430E - [0] for Secondary Carrier, [1] for Primary Carrier */
  mac_hs_ul_maci_log_pkt_record_per_carrier_type* maci_log_record_ptr; /* Pointer to a row of Global Record */

#endif
  /*Every TTI 0x4323 and 0x4322 info will be stored and it will 
   *finally be flushed in task context to diag services.
   */
  mac_hs_ul_log_etfci_status_uncompressed_pkt_type    mac_hs_ul_etfci_status_log_info;
  mac_hs_ul_log_etfci_status_uncompressed_pkt_type    mac_hs_ul_etfci_status_prev_log_info;
  mac_hs_ul_log_etfci_status_uncompressed_pkt_type    *mac_hs_ul_etfci_log_ptr;

  mac_hs_ul_log_header_uncompressed_pkt_type         mac_hs_ul_header_log_info;
  mac_hs_ul_log_header_uncompressed_pkt_type         mac_hs_ul_header_prev_log_info;
  mac_hs_ul_log_header_uncompressed_pkt_type         *mac_hs_ul_header_log_ptr;

  mac_hs_ul_log_etfci_status_ctrl_info_per_tti_type   *mac_hs_ul_etfci_status_ctrl_log_ptr;
  mac_hs_ul_log_etfci_status_pkt_per_tti_type         *mac_hs_ul_etfci_status_pkt_log_ptr;

  /*-----------------------------------------------------------------------------
  If mac_hs_ul_current_etfci_log_buf_flag = TRUE
      - mac_hs_ul_etfci_status_log_info  buffer is used to log the log_packetsamples.
  Else
      - mac_hs_ul_etfci_status_prev_log_info buffer is used to log the log_packet samples.
    
  If mac_hs_ul_current_hdr_log_buf_flag = TRUE
      - mac_hs_ul_header_log_info buffer is used to log the log_packetsamples.
  Else
      - mac_hs_ul_header_prev_log_info buffer is used to log the log_packet samples.
  ---------------------------------------------------------------------------  */
  boolean                                             mac_hs_ul_current_etfci_log_buf_flag;
  boolean                                             mac_hs_ul_current_hdr_log_buf_flag;
#ifdef FEATURE_WCDMA_HS_RACH
  /* Double buffer for MAC timer info */
  mac_hs_ul_hsrach_mac_timer_log_pkt_type  hs_rach_mac_timers_log_pkt_info, hs_rach_mac_timers_prev_log_pkt_info;

  /* Pointer to mac timer buffer being used -- will start with cur log buf */
  mac_hs_ul_hsrach_mac_timer_log_pkt_type  *hs_rach_mac_timers_log_pkt_info_ptr;

  /* True - Using hs_rach_mac_timers_log_pkt_info buffer 
     False - Using hs_rach_mac_timers_prev_log_pkt_info buffer */
  boolean hs_rach_mac_timers_cur_log_buf_flag;

  /* Index to mac timer info -- also used to track number of samples */
  uint8 mac_timer_pkt_idx;

  /* Boolean indicating if this log packet - 0x4314 is enabled or not */
  boolean hs_rach_mac_timers_log_pkt_status;
  
  /* Boolean indicating when to start 0x4314 logging - This would control
   updating log packet global with stale values */
  boolean hs_rach_start_logging;
#endif  /*  FEATURE_WCDMA_HS_RACH  */
#endif
#if defined( FEATURE_L1_LOGGING_ENABLED ) && defined( FEATURE_HSUPA_L1_LOGGING_ENABLED)

/* This gives the list of MAC-d flows that are present in this transmission
  For example, bit 0 (LSB) indicates whether MAC-d flow 0 is present in this transport block.
  0: this Mac-d flow is absent
  1: this Mac-d flow was present */
 uint8        mac_hs_ul_log_macd_present_bitstr;
 /*
 0 = No SI present
 1 = SI present, triggered by event/timer
 2 = SI present, triggered by padding
 3 = Reserved
 */
 uint8        mac_hs_ul_log_si_status;
 boolean      mac_hs_ul_nex_tx;
 
  /*  Accumulated log packet collecting stats for the L1-MAC combined log packet 0x4311  */
  mac_hs_ul_log_mac_stats_struct_type         mac_hs_ul_l1_mac_accum_stats_info[MAC_HS_UL_MAX_NUM_CARRIER_ALLOWED]; 
#endif
  uint32    mac_eul_enable_debug_f3s; 
  log_etfci_restriction_reason_info_type    log_etfci_reason;
}mac_eul_logging_info;

typedef struct
{
  sys_modem_as_id_e_type as_id;
  boolean mac_hs_rach_rxd_enable; 
  boolean mac_eul_enable; /* indicates if EUL is enabled in DCH */
  boolean   mac_hs_ul_reconfig_in_progress; /*  Indicates if reconfig is in progress or not  */  
  boolean   mac_hs_ul_mac_d_flow_carries_ns_data[MAX_E_DCH_MAC_D_FLOW]; /*  Indicates if this MAC_d flow carries Non-Scheduled data  */
#ifdef FEATURE_WCDMA_DC_HSUPA
  boolean   srbs_on_non_sch_flows;
#endif
  /*  Active log channles of the previous config is stored here  */
  boolean              mac_hs_ul_log_ch_enabeld[UE_MAX_UL_LOGICAL_CHANNEL];

  boolean mac_hs_ul_serv_cell_change; /*  This indicates if the serving cell has changed or not  */
  boolean                                      sched_non_zero_bo;

  /*  This is to store the HARQ restriction information in 2ms TTI  */
  uint8                             mac_hs_ul_rrc_sched_harq_restrictions;
  
  /* Use this global when "periodicityOfSchedInfo-Grant" IE is set to "everyEDCHTTI" Holds 2 or 10 depending on 2msec TTI or 10msec TTI call */
  uint8                               mac_hs_ul_tti_config_value;

  /* These varaible are used to store the previous active config information */
  uint8     mac_hs_ul_prev_num_mac_d_flow;
  uint8     mac_hs_ul_prev_num_logch;

  uint8                              global_harq_id;
  
  uint32     mac_hs_ul_tsn_back_up[UE_MAX_UL_LOGICAL_CHANNEL]; /*Back up of TSN which is maintained for each LC(RRC will give indication for Bachup TSN) */
  boolean    mac_hs_ul_tsn_backed_up_flag; /*  TRUE whenever we back up TSN either through L1 or MAC call  */  
  
  uint32     mac_hs_ul_tsn[UE_MAX_UL_LOGICAL_CHANNEL]; /*TSN which is maintained for each LC.The TSN is a number between 0 and 63  */

  uint32     bits_avail_sec;


#ifdef FEATURE_MAC_I
  boolean                             mac_partial_pdu_support;
  mac_ul_pdu_hdr_e_type               mac_hs_ul_pdu_type;  /* MAC-e/es or MAC-i/is PDU */
  mac_e_transition_e_type             mac_hs_ul_transition_type;  /* MAC-e or MAC-i PDU */ 
  mac_i_seg_status                    mac_current_segment;
  mac_i_flex_pdu_size_chosen mac_i_flex_pdu_size_chosen_info;
 #endif

  /* Populated during the config time, this will contain the min mlp id, max mlp id, min rlc id and 
     max rlc id configured. This will be used to retsrict the number of times a loop is executed */
  mac_hs_ul_loop_limits_cfg    mac_hs_ul_loop_limit;

  mac_hs_ul_etfc_common_info_type       mac_hs_ul_etfc_common_info[MAC_HS_UL_MAX_NUM_CARRIER_ALLOWED];

  /* This is an array of structure which stores {E-TFC, power offset,SG} info for each of HARQ's.
   * This will be updated with the latest info corresponding to that HARQ ID.
   * In case of retransmissions this structure will be used to determine the happy bit setting */
  mac_hs_ul_prev_harq_tx_info_type  mac_hs_ul_prev_harq_tx_info[8][MAC_HS_UL_MAX_NUM_CARRIER_ALLOWED];
  /*---------------------------------------------------------------------------
  This structure provides L1 with the E-TFCI selected for this TTI and how much 
  of scheduled data was transmitted this TTI. This is populated by MAC and 
  returned after processing of the l1_edch_req_control_info function
  ---------------------------------------------------------------------------*/
  l1_eul_edch_control_info_type         mac_hs_ul_edch_control_info[MAC_HS_UL_MAX_NUM_CARRIER_ALLOWED];

  /*---------------------------------------------------------------------------
  This structure contains information about the number of PDU's that
  can built on each logical channel. This structure will be valid
  after E-TFCI selection is done.
  ---------------------------------------------------------------------------*/
  mac_hs_ul_rlc_pdu_ddi_info_type       mac_hs_ul_rlc_pdu_ddi_info[MAC_HS_UL_MAX_NUM_CARRIER_ALLOWED];

  /*  Structure to store the results of power offset computation  */
  mac_hs_ul_pwr_offset_compn_result_type mac_hs_ul_pwr_offset_compn_results;
  
  /*  Gives the MAC-d flow specific information(MAX_E_DCH_MAC_D_FLOW = 8)  */
  mac_hs_ul_mac_d_flow_info_type   mac_hs_ul_mac_d_flow_info[MAX_E_DCH_MAC_D_FLOW];

  /*  Gives the mapping of priority to logical channels(MAC_UL_MAX_MLPS = 8)  */
  /* Makes a mapping of LCs mapped onto MAC_d flows based on priority.
   *This is done for scheduled mac-d flows and Non-sched mac-d flows separately. */
  mac_hs_ul_priority_lc_mapping_info_type   mac_hs_ul_priority_lc_mapping[MAC_UL_MAX_MLPS+1];
  mac_hs_ul_priority_lc_mapping_info_type   mac_hs_ul_sched_lc_mapping[MAC_UL_MAX_MLPS+1];
  mac_hs_ul_priority_lc_mapping_info_type   mac_hs_ul_non_sched_lc_mapping[MAC_UL_MAX_MLPS+1];
  mac_hs_ul_etfci_restriction_reason_e_type                 pwr_offset_log_reason;
#ifdef FEATURE_WCDMA_DC_HSUPA  
  mac_hs_ul_sec_carrier_info_type                          mac_hs_ul_sec_car_info;
  mac_hsupa_si_tmr_info_type  mac_hs_ul_si_timer_info[MAC_HS_UL_MAX_NUM_CARRIER_ALLOWED]; /*  SI timer related information  */
#else
  mac_hsupa_si_tmr_info_type  mac_hs_ul_si_timer_info;
#endif

  /*  Gives the mapping of logical channel to MAC-d flow(MAX_DED_LOGCHAN_PER_TRCH = 15)  */
  mac_hs_ul_logical_ch_mac_d_flow_info_type
               mac_hs_ul_logical_ch_mac_d_flow_mapping[UE_MAX_UL_LOGICAL_CHANNEL];

  /* Only few variables of this structure are used - remove others ? */
  rlc_ul_buf_status_type                               mac_hs_ul_si_lc_buf_status;

  /*Identifies the index to the E-TFCI table tobe used for the current
   *scenario. and also gives the minimum set for the EDCH
   */
  mac_hs_ul_e_tfci_info_type                         mac_hs_ul_e_tfci_info;

  /*  Contains the list of logical channels for which MAC needs Buffer status  */
  rlc_ul_lc_info_type                                           mac_hs_ul_lc_info;

  /*  Gives the buffer status for the logical channels mapped onto E-DCH  */
  rlc_ul_buf_status_type                                  mac_hs_ul_lc_buf_status;

  /*  Gives the information necessary to trigger SI and happy bit  */
  mac_hs_ul_si_happybit_parameters_config_type
                                              mac_hs_ul_si_happybit_config_info;
  /*  E-DCH Transmission Time Interval  */
  e_tti_enum_type                                       mac_hs_ul_tti_type;                                              
#ifdef FEATURE_WCDMA_HS_RACH
  mac_ul_hs_rach_cfg_info_type  mac_hs_rach_info;
#endif

#ifdef FEATURE_WCDMA_EVT6D_ENH
  uint8   mac_eul_6d_sup_tshld[MAC_EUL_MAX_E_TFCI_ARRAY_INDEX];
  uint8   mac_eul_6d_sup_thru_qxdm;
  uint8   mac_eul_6d_consec_tti;
  uint8   mac_eul_6d_sup_reductn;
  uint8   mac_eul_6d_max_skip_count;
  uint32  mac_eul_6d_reinit_timer;
  mac_evt_6d_opt_e_type  mac_eul_6d_enable;
#endif

/* RAVE Param for EUL */
#ifdef FEATURE_DATA_RAVE_SUPPORT 
  mac_eul_rave_param_type    mac_eul_rave_info;
#endif

  mac_eul_logging_info  eul_log_info;
}mac_eul_table_type;

extern mac_eul_table_type mac_eul_table[]; 
#define GET_MAC_EUL_TABLE_PTR(as_id) &(mac_eul_table[as_id])

#ifdef FEATURE_DATA_RAVE_SUPPORT 
/* MACRO used to RESET RAVE Params after periodcity expires */
#define MAC_UL_RESET_RAVE_PARAMS(mac_ul_rave_info_ptr)\
  do\
  { \
    mac_ul_rave_info_ptr->agg_time        = 0;\
    mac_ul_rave_info_ptr->agg_capacity    = 0;\
  }\
  while(0)
#endif

/*===========================================================================

                         GLOBAL FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION: MAC_HS_UL_CONFIG_REQ_HANDLER

DESCRIPTION:
This function is called when some HSUPA related configuration is given by
RRC to MAC.  The uplink configuration is absolute.  Both HSUPA and Non-HS
configuration is obtained in the same command.  RRC will specify if HS
configuration has changed or not.  If HS configuration has changed then
this function will be called to apply the configuration. By defualt whenever
MAC receives the CMAC_UL_CONFIG_REQ it will apply the new confirguration
for Non-HS channels.

DEPENDENCIES:
  If at this point any validations fail, MAC err_fatals so that these
  can be corrected.

RETURN VALUE:
  None

SIDE EFFECTS:

  MAC internal tables are populated.
  Hence if any configuration validation fails MAC HSUPA config will not
  be reliable. Its upto RRC to make MAC come to a sane state.
===========================================================================*/
void mac_hs_ul_config_req_handler
(
  mac_ul_config_type    *cfgptr, sys_modem_as_id_e_type as_id
);

/*===========================================================================
FUNCTION: mac_hs_ul_init

DESCRIPTION:
  This function performs the intialization of some global data and creates
  timers for the triggers needed for scheduling information. This function is
  called when the mac ul task is created

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  All internal variables maintained by MAC are initialized
===========================================================================*/
void mac_hs_ul_perform_init(boolean is_power_up, sys_modem_as_id_e_type as_id);

#ifdef FEATURE_MAC_HS_UL_QXDM_LOGGING
/*===========================================================================
FUNCTION: mac_eul_allocate_mem_log_records

DESCRIPTION:
This function dynamically allocates memory for EUL 0x430E log record pointer
===========================================================================*/
void mac_eul_allocate_mem_log_records(mac_eul_table_type *eul_table_ptr);
#endif

 /*===========================================================================
 FUNCTION mac_hs_ul_rlc_pdu_size_sort_comp_fn
 
 DESCRIPTION
   This function is the comparison function for the bubble sort utility in L1
   math utils. It takes void pointer, casts them to mac_e_rlc_size_ddi_info_s_type 
   struct type and compares the rlc_pdu_sizes.
 
 DEPENDENCIES
   None
 
 RETURN VALUE
   1 - if argument1 pdu size > argument2 pdu size
   0 - otherwise
 
 SIDE EFFECTS
   None
 ===========================================================================*/
 
int8 mac_hs_ul_rlc_pdu_size_sort_comp_fn(
  /* The two rlc_size_ddi_info to be compared */
  const void *rlc_size_ddi_info1, const void *rlc_size_ddi_info2);

 boolean    mac_hs_ul_query_si_triggered( l1_eul_pwr_offset_req_struct_type  pwr_offset_info,
                                          mac_hsupa_si_tmr_info_type            *si_state_info_ptr,
                                          mac_hs_ul_prev_harq_tx_info_type      *prev_harq_tx_info_ptr,
                                          uint32                            sched_buf_status,
                                          boolean                           bfi_query,
                                          mac_eul_table_type *eul_table_ptr);

void mac_hs_ul_trigger_si_due_to_ta(sys_modem_as_id_e_type as_id, si_trigger type_of_trigger);

/*===============================================================================================
 FUNCTION NAME  : mac_hs_ul_ignore_ul_srbs

 DESCRIPTION  : This function clears the buffer status of UL Signal Radio Bearers

 RETURN VALUE:
  Num of Non-schd bytes mapped on SRBs at present TTI
===============================================================================================*/
uint32    mac_hs_ul_ignore_ul_srbs
(
  /* Uplink Buffer Status Pointer */
  rlc_ul_buf_status_type        *rlc_ul_buf_status,
  mac_eul_table_type          *eul_table_ptr
);

/*===============================================================================================
 FUNCTION NAME  : mac_hs_ul_logpacket_info

 DESCRIPTION  : This function 
===============================================================================================*/
#ifdef FEATURE_MAC_HS_UL_QXDM_LOGGING
void mac_hs_ul_logpacket_info
(
  uint8   harq_id
);

void  mac_hs_ul_mace_etfci_log_buf_change(uint16  num_samples, mac_eul_table_type *eul_table_ptr);
void  mac_hs_ul_mace_hdr_log_buf_change(uint16  num_samples, mac_eul_table_type *eul_table_ptr);

#endif

/*===============================================================================================
 FUNCTION NAME  : buffer_status_info_logging

 DESCRIPTION  : This function 
===============================================================================================*/

void buffer_status_info_logging(void);

#ifdef FEATURE_WCDMA_HS_RACH
rlc_ul_lc_info_type *get_mac_hs_ul_lc_info_ptr(mac_eul_table_type *eul_table_ptr);
mac_hs_ul_logical_ch_mac_d_flow_info_type *get_mac_hs_logch_macd_flow_mapping_ptr(mac_eul_table_type *eul_table_ptr);

#ifdef FEATURE_MAC_HS_UL_QXDM_LOGGING
  /*===========================================================================
  FUNCTION mac_hsrach_submit_timer_log_pkt
  
  DESCRIPTION This function enqueues a command to L2 UL task 
                command queue to send mac timer log info to Diag buffers.
  ===========================================================================*/
void mac_hsrach_submit_timer_log_pkt(mac_eul_table_type *eul_table_ptr);
/*==========================================================================
FUNCTION: mac_update_hs_rach_log_pkt_info

DESCRIPTION: Fields of HS RACH log packet are updated here.
==========================================================================*/
void mac_update_hs_rach_log_pkt_info 
( 
  boolean rsrc_avail, 
  l1_mac_hs_rach_edch_rsrc_rel_cause_enum_type rel_cause,
  mac_eul_table_type *eul_table_ptr
);

#endif /* FEATURE_MAC_HS_UL_QXDM_LOGGING */

#endif /* FEATURE_WCDMA_HS_RACH */

void mac_hs_ul_eul_action_stop_handler
(
#ifdef FEATURE_WCDMA_HS_RACH 
  mac_hs_rach_action_e_type   hs_rach_action,
#endif
  sys_modem_as_id_e_type as_id
);
/*===========================================================================
FUNCTION    MAC_HS_UL_GET_ETFCI

DESCRIPTION
This function will do a binary search into the E-TFCI table and
return the E-TFCI corresponding to the number of bits that will
be transmitted this TTI

If we have found a E-TFCI then
Map the mac_e_pdu_size to the closest E-TFC payload from the table.
Also get the index of the E-TFCI which maps to this E-TFC Payload
This will require a binary search.

DEPENDENCIES


RETURN VALUE
  Chosen E-TFCI and the E-TFCI corresponding to the scheduled data
  txed will be updated in the structure
  mac_hs_ul_edch_control_info.eul_etfci_info

SIDE EFFECTS

===========================================================================*/
uint32  mac_hs_ul_get_etfci
(
  /* Total MAC e PDU size for this TTI */
  uint32    mac_e_pdu_size,
  /* Number of values in the table */
  uint8     nvalues,

  mac_eul_table_type *eul_table_ptr
);

void   mac_hs_ul_init_si_timers_powerup(mac_eul_table_type *eul_table_ptr,  mac_hsupa_si_tmr_info_type*  si_timer_info_ptr);
void   mac_hs_ul_init_sec_car_si_timers_powerup(mac_eul_table_type *eul_table_ptr,  mac_hsupa_si_tmr_info_type*  si_timer_info_ptr);

void   mac_hs_ul_init_si_timers( mac_hsupa_si_tmr_info_type*  si_timer_info_ptr, boolean stop_si_timer);
void   mac_hs_ul_set_si_timer_conifg_values( mac_eul_table_type *eul_table_ptr, 
                                             uint16 t_si_grant,
                                             uint16 t_si_no_grant,
                                             mac_hsupa_si_tmr_info_type*  si_timer_info_ptr);
  
void   mac_hs_ul_initialize_globals_before_etfci_selection( l1_eul_serving_grant_update_info_type *sg_info_ptr ,
                                                   mac_hs_ul_etfc_common_info_type       *etfc_common_info_ptr,
                                                   l1_eul_etfci_info_type                 *eul_etfci_info_ptr,
                                                   mac_hs_ul_pwr_offset_compn_result_type  *po_compn_result_ptr,
                                                   uint8                                  carrier_index, mac_eul_table_type *eul_table_ptr);

#ifdef FEATURE_MAC_I 

/* Clear all the partial related information for each logical channel
 * Useful during MAC-I to MAC-E transition
 */
void mac_hs_ul_clear_mac_i_partial_info( mac_eul_table_type *eul_table_ptr );

void   mac_hs_ul_maci_log_buf_change(mac_eul_table_type *eul_table_ptr);

/*===========================================================================
FUNCTION      mac_hs_ul_mac_i_config_handler

DESCRIPTION   Following actions are completed
              1. If the current config is RECONFIG
                  - Update the mac_hs_ul_transition_type based on current  
                    and reconfig pdu type.
                  - Check MAC HS UL Reset as the queues need to be flushed
                    (25.331 - 8.6.6.37 E-DCH Info)
                     
              2. Update the Current pdu type
===========================================================================*/
void mac_hs_ul_mac_i_config_handler(mac_ul_config_type    *cfgptr, mac_eul_table_type *eul_table_ptr);

/*-------------------------------------------------------------------
FUNCTION MAC_HS_UL_CLEAR_RLC_ID_PARTIAL_INFO

DESCRIPTION
  API to clear stored partial info for the passed RLC - ID
  
DEPENDENCIES
  
RETURN VALUE
  none

SIDE EFFECTS
  
--------------------------------------------------------------------*/
void mac_hs_ul_clear_rlc_id_partial_info(rlc_lc_id_type rlc_id, sys_modem_as_id_e_type as_id);

#endif /* FEATURE_MAC_I */

#ifdef FEATURE_WCDMA_HS_RACH
void mac_hsrach_reset_tsn_during_rsr_release(mac_eul_table_type *eul_table_ptr);

rlc_ul_lc_info_type *get_mac_hs_ul_lc_info_ptr(mac_eul_table_type *eul_table_ptr);

mac_hs_ul_logical_ch_mac_d_flow_info_type *get_mac_hs_logch_macd_flow_mapping_ptr(mac_eul_table_type *eul_table_ptr);

void mac_hsrach_config_timers_init(uint8 add_e_dch_tx_backoff, uint8 max_collision_resol,
                                   uint8 max_e_dch_res_alloc_ccch ,
                                   l1_e_dch_tx_cont_backoff_enum_type   l1_e_dch_tx_cont_backoff,
                                   mac_eul_table_type *eul_table_ptr);

void mac_hsrach_config_timers_stop(mac_eul_table_type *eul_table_ptr);

void l1_mac_hsrach_get_mac_config_params(l1_mac_hs_rach_config_params *config_params, sys_modem_as_id_e_type wsub_id);


void mac_hsrach_phychan_estblish_failure_handler(mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr);

void mac_hsrach_edch_res_release_cleanup(mac_eul_table_type *eul_table_ptr);

void mac_hs_ul_fill_ernti_special_mac_i_hdr(mac_hs_ul_rlc_pdu_ddi_info_type  *bfi_rlc_info, mac_eul_table_type *eul_table_ptr);

//Tebs timer count is zero and enough SG to include SI0 with data
void mac_hsrach_tebs_timer_expiry_during_build_pdu(mac_eul_table_type *eul_table_ptr);

boolean mac_hsrach_tebs0_si_trrigger_query(mac_eul_table_type *eul_table_ptr);

/* Buffer status computation and also determines the LC_TYPE in HS_RACH */
boolean  mac_hsrach_buffer_status(boolean *data_present_on_srb,mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr);

void   mac_hsrach_tebs_timer_update_during_po(uecomdef_logchan_e_type   chan_type,rlc_lc_id_type  rlc_id, mac_eul_table_type *eul_table_ptr);

void mac_hsrach_edch_tx_backoff_timer_update(l1_mac_hsrach_eul_timer_status_enum status, mac_eul_table_type *eul_table_ptr);

uint32 mac_hsrach_si0_content(uint32    uph, mac_eul_table_type *eul_table_ptr, uint8 index);
uint8  mac_hsrach_pick_mlp(mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr);

#endif /* FEATURE_WCDMA_HS_RACH */
void mac_hs_ul_hb_computation
(
  uint8 index,
  l1_eul_edch_control_info_type *mac_control_info_ptr,
  mac_hs_ul_happy_bit_params_type  *hapy_bit_params_ptr,
  mac_eul_table_type *eul_table_ptr
);
void l1_edch_update_mac_log_packets_during_reconfig(mac_eul_table_type *eul_table_ptr);

#endif /* FEATURE_WCDMA_HSUPA */
#endif /* MAC_HS_UL_H */

#ifndef     MACUL_H
#define     MACUL_H
/*=========================================================================================

                        U P L I N K   M A C  H E A D E R   F I L E


Copyright (c) 2001 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2002-2009 by Qualcomm Technologies, Inc.  All Rights Reserved.

========================================================================================*/

/*========================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macul.h_v   1.5   10 Jul 2002 10:04:24   skrishna  $
$Header: //components/rel/wcdma.mpss/8.4.0/mac/inc/macul.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------
06/26/15   aa      Removing extern declarations from L2 code
05/22/15   scv     WL2 Compiler Warnings Fix
04/08/15   as      W+W MVS L2 new interface changes.
03/19/15   aa      RRC-L2 interface changes for passing AS ID as part of L2 commands
01/29/15   kc      L2 global reorg for W+W
01/15/15   vr      Wrapping up some UL globals used by L2 under MACROs
08/21/14   ts      FR22375: Fixed Medium compiler warnings in WCDMA on Bolt 2.0/2.1
03/19/14   kc      Changing FEATURE for heap corruption issue.
03/18/14   ts      Fix for heap memory corruption issue, to allocate and free heap memory fo rlog buffers in l2ultask itself
10/11/12   mrg     TVM optimization for MRAB
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/05/12   mrg     HS-RACH & MAC-I Feature code integrated
03/06/12   mrg     RACH staus info from L1 to MAC indicaiton with signal based approch enhanced with double buffer.
01/02/12   grk     Feature cleanup
12/06/11   ksr     Fix to handle back to back RACH ABORT and RACH NO ACK to MAC from L1
                   Moving to command based interface from signal based approach.
10/22/11   sk      NikeL UL CM workaround under feature FEATURE_WCDMA_UL_CM_INTLV_WORKAROUND
06/02/11   kc      Added support for logical channel to transport channel mapping info
11/11/10   mrg     MAC DL HS ZI memory optimization changes
09/16/10   mrg     Moved auido(MVS) dependency under the feature flag FEATURE_MVS
06/28/10   mrg     MAC-R99 ZI memory reduction changes
10/07/09   mrg     Fixed Klocwork warnings
05/07/09   ssg     Updated Copyright Information
04/29/09    av     Fixed compiler warnings in L2 
03/13/08    sk     Added MAC_CHECK_HSUPA_CONFIG_STATUS declaration
10/17/07    ps     Logic for deriving AMR rate based on sum of class A,
                   Class B and Class C bits.
03/20/07    sk     Added extern uint32 mac_get_cnt_c_val under FEATURE_WCDMA_CNT_C_SFN_DIFF
03/03/07    sk     Added support for WB-AMR under FEATURE_VOC_AMR_WB
10/16/06    sk     Fixed compiler warning
09/21/05   gsc     Added support for Rel-5 spec CR 2272.
07/20/05   gsc     Added support for new interface between L1/MAC for CFN reporting
                   to MAC by L1. This new mechanism in enabled when
                   FEATURE_WCDMA_HFN_UPDATE is not defined.
05/05/05    sk     Added protoype for mac_asc_selection()
03/09/05    sk     Removed ## from the macro definitions.
12/30/04    sk     Fixed lint errors
06/05/05    sk     Added MAC_UL_TFC_MAX to line it with L1_UL_TFC_MAX that L1 has for
                   number of TFC's for which the excess power state is computed
04/26/04    sk     Removed the number of ded logical channel constraints for the minimum
                   in the macro UL_DCCH_DTCH_NUM_IS_INVALID definition
04/14/04    sk     Added a macro GET_UL_TRCH_TF_INFO_PTR
12/26/03    sk     Added support for TFCS selection based on logical channel prioritites
                   under FEATURE_TFCS_LC_PRIORITIES
09/04/02    sk     Added extern declarations for some global variables under feature
                   definition
06/08/02    sk     Removed FEATURE_TRAFFIC_VOLUME_MEAS
05/13/02    sk     Defined macro GET_UL_TRCH_ID_TO_IDX_PTR. This macro would give the
                   transport channel parameters for a given transport channel id. This
                   addition is made under the FEATURE_TRAFFIC_VOLUME_MEAS
02/12/02    ps     Deleted UL U-RNTI related definitions and declarations
06/19/01    ps     Converted UL TX Status and Build Frame Indications from commands to signals
06/18/01    ps     declared ul_cctrch_info as external

=========================================================================================*/

#include "wcdma_variation.h"
#include "macrlcif.h"
#include "mactraffic.h"
#include "maculhs.h"
#include "stringl.h"

#ifdef FEATURE_WCDMA_MAC_UL_ZI_REDUCTION
#error code not present
#endif /* FEATURE_WCDMA_MAC_UL_ZI_REDUCTION */

#define MAC_UL_TFC_MAX 64

extern mac_dl_table_type mac_dl_table[];

extern mac_rlc_data_table_type    mac_data_table[];
#define     GET_MAC_RLC_DATA_TABLE_PTR(as_id)    &mac_data_table[as_id]

/*========================================================================================
 FUNCTION NAME  proc_ul_cipher_info

 DESCRIPTION
    This function processes the UL Ciphering information received from RRC

=========================================================================================*/
void    proc_ul_cipher_info
(
    /* Pointer to UL MAC Ciphering information */
    mac_cipher_config_type   *cipherptr,
    mac_ul_table_type *ul_table_ptr
);

#define     GET_UL_TRCH_ID_TO_IDX_PTR(trch_id) &ul_table_ptr->ul_trch_id_to_idx_table[trch_id-MAC_UL_MIN_TRCH_ID]

/* MACRO to check Trch id_to_idx is valid or not */
#define   TRCH_ID_TO_IDX_IS_INVALID(trch_id)\
            ((trch_id < MAC_UL_MIN_TRCH_ID) || (trch_id > MAX_TRCH))

#define     UL_MAC_CRNTI_IS_VALID   (ul_table_ptr->ul_mac_crnti_valid)

#define     GET_UL_MAC_CRNTI        (ul_table_ptr->ul_mac_crnti)

/* Selected TFCI */
#define     GET_SELECTED_TFCI           (ul_table_ptr->int_ul_cctrch_info.selected_tfci)
#define     GET_UL_CCTRCH_TYPE          (ul_table_ptr->int_ul_cctrch_info.cctrch_type)
#define     SET_SELECTED_TFCI(tfci)      ul_table_ptr->int_ul_cctrch_info.selected_tfci = tfci;

/*===========================================================================================
FUNCTION NAME   proc_ul_rnti_info

PURPOSE         Process the UL RNTI information received from RRC

PARAMETERS      rntiptr : Pointer to the RNTI information

DESCRIPTION     If the RNTI info is valid, the received RNTI information
                is saved in MAC's local memory
===========================================================================================*/
void    proc_ul_rnti_info
(
  /* Pointer to the RNTI information */
  mac_rnti_config_type   *rntiptr,
  
  mac_ul_table_type *ul_table_ptr
);

#define     GET_UL_LC_INFO_PTR(rlc_id)      &ul_table_ptr->int_ul_lc_info[rlc_id - MAC_UL_MIN_RLC_ID]

#define     GET_UL_TRCH_INFO_PTR(trch_idx)  &ul_table_ptr->int_ul_trch_info[trch_idx]

#define     GET_UL_TRCH_PRIORITY_INFO_PTR(trch_idx, priority) (&ul_table_ptr->int_ul_trch_priority_info[trch_idx][priority - MAC_UL_MIN_MLP_ID])

#define   GET_UL_TRCH_TF_INFO_PTR(trch_idx, tf) (&ul_table_ptr->int_ul_trch_tf_info[trch_idx][tf])

/* MACRO to check the UL Logical Channel Priority */
#define     UL_LC_PRIORITY_IS_INVALID(priority)\
            ((priority < MAC_UL_MIN_MLP_ID ) || (priority > MAC_UL_MAX_MLP_ID ))

/* MACRO to check the UL ASC validity */
#define     UL_ASC_IS_INVALID(asc)          (asc > MAC_UL_MAX_ASC_ID)

/* MACRO to check UL RLC ID */
#define     UL_RLC_ID_IS_INVALID(rlc_id)    (rlc_id > MAC_UL_MAX_RLC_ID)

/* MACRO to check UL RB ID */
#define     UL_RB_ID_IS_INVALID(rb_id)      (rb_id > MAC_UL_MAX_RB_ID)

/* MACRO to check UL Transport Channel ID */
#define     UL_TRCH_ID_IS_INVALID(trch_id)\
            ((trch_id < MAC_UL_MIN_TRCH_ID) || (trch_id > MAC_UL_MAX_TRCH_ID))

/* MACRO to check RLC Mode */
#define     UL_RLC_MODE_IS_INVALID(rlc_mode)\
            ((rlc_mode >= UE_MODE_MAX_NUM))

/* MACRO to check MAC ID */
#define     UL_MAC_ID_IS_INVALID(mac_id)\
            ((mac_id < MAC_MIN_MAC_ID ) || (mac_id > MAC_MAX_MAC_ID))

/* Condition to check invalid number of DCCH/DTCHs */
#define     UL_DCCH_DTCH_NUM_IS_INVALID(ndlchan)\
            (ndlchan > MAX_DED_LOGCHAN_PER_TRCH)

/*
* The following constants are used when determining the Logical
* channel mapping information for the vocoder AMR Modes and Classes.
* Some of these are not actually used in the code anywhere, but it's
* nice to have them here for completeness.  The unused ones are
* commented out to prevent lint complaints.
*/
/* #define WVS_AMR_MODE_CLASS_C_0_BIT_SIZE     0 */

/* AMR 4.75 kbps speech */
#define AMR_MODE_475_CLASS_A_BIT_SIZE  42
#define AMR_MODE_475_CLASS_B_BIT_SIZE  53

/* AMR 5.15 kbps speech */
#define AMR_MODE_515_CLASS_A_BIT_SIZE  49
#define AMR_MODE_515_CLASS_B_BIT_SIZE  54

/* AMR 5.90 kbps speech */
#define AMR_MODE_590_CLASS_A_BIT_SIZE  55
#define AMR_MODE_590_CLASS_B_BIT_SIZE  63

/* AMR 6.70 kbps speech */
#define AMR_MODE_670_CLASS_A_BIT_SIZE  58
#define AMR_MODE_670_CLASS_B_BIT_SIZE  76

/* AMR 7.40 kbps speech */
#define AMR_MODE_740_CLASS_A_BIT_SIZE  61
#define AMR_MODE_740_CLASS_B_BIT_SIZE  87

/* AMR 7.95 kbps speech */
#define AMR_MODE_795_CLASS_A_BIT_SIZE  75
#define AMR_MODE_795_CLASS_B_BIT_SIZE  84

/* AMR 10.2 kbps speech */
#define AMR_MODE_102_CLASS_A_BIT_SIZE  65
#define AMR_MODE_102_CLASS_B_BIT_SIZE  99
#define AMR_MODE_102_CLASS_C_BIT_SIZE  40

/* AMR 12.2 kbps speech */
#define AMR_MODE_122_CLASS_A_BIT_SIZE  81
#define AMR_MODE_122_CLASS_B_BIT_SIZE 103
#define AMR_MODE_122_CLASS_C_BIT_SIZE  60

/* AMR Mode 8 (SID) */
#define AMR_MODE_8_CLASS_A_BIT_SIZE   39

#define AMR_CLASS_NO_DATA_FRAME_SIZE 0

#define  AMR_MODE_475_TOTAL_BITS  (AMR_MODE_475_CLASS_A_BIT_SIZE + \
                                                                                 AMR_MODE_475_CLASS_B_BIT_SIZE)
#define  AMR_MODE_515_TOTAL_BITS (AMR_MODE_515_CLASS_A_BIT_SIZE + \
                                                                          AMR_MODE_515_CLASS_B_BIT_SIZE)
#define AMR_MODE_590_TOTAL_BITS (AMR_MODE_590_CLASS_A_BIT_SIZE + \
                                                                          AMR_MODE_590_CLASS_B_BIT_SIZE)
#define AMR_MODE_670_TOTAL_BITS (AMR_MODE_670_CLASS_A_BIT_SIZE + \
                                                                          AMR_MODE_670_CLASS_B_BIT_SIZE)
#define AMR_MODE_740_TOTAL_BITS (AMR_MODE_740_CLASS_A_BIT_SIZE + \
                                                                         AMR_MODE_740_CLASS_B_BIT_SIZE)
#define AMR_MODE_795_TOTAL_BITS (AMR_MODE_795_CLASS_A_BIT_SIZE + \
                                                                         AMR_MODE_795_CLASS_B_BIT_SIZE)
#define AMR_MODE_102_TOTAL_BITS (AMR_MODE_102_CLASS_A_BIT_SIZE + \
             AMR_MODE_102_CLASS_B_BIT_SIZE + AMR_MODE_102_CLASS_C_BIT_SIZE)
#define AMR_MODE_122_TOTAL_BITS (AMR_MODE_122_CLASS_A_BIT_SIZE + \
             AMR_MODE_122_CLASS_B_BIT_SIZE + AMR_MODE_122_CLASS_C_BIT_SIZE)

#define AMR_WB_CLASS_A_BIT_SIZE  40

#define AMR_WB_MODE_660_TOTAL_BITS (MVS_AMR_WB_MODE_660_CLASS_A_BIT_SIZE + \
                                                                            MVS_AMR_WB_MODE_660_CLASS_B_BIT_SIZE)
#define AMR_WB_MODE_885_TOTAL_BITS (MVS_AMR_WB_MODE_885_CLASS_A_BIT_SIZE + \
                                                                            MVS_AMR_WB_MODE_885_CLASS_B_BIT_SIZE)
#define AMR_WB_MODE_1265_TOTAL_BITS (MVS_AMR_WB_MODE_1265_CLASS_A_BIT_SIZE + \
                                                                           MVS_AMR_WB_MODE_1265_CLASS_B_BIT_SIZE)
#define AMR_WB_MODE_1425_TOTAL_BITS (MVS_AMR_WB_MODE_1425_CLASS_A_BIT_SIZE+  \
                                                                           MVS_AMR_WB_MODE_1425_CLASS_B_BIT_SIZE)
#define AMR_WB_MODE_1585_TOTAL_BITS (MVS_AMR_WB_MODE_1585_CLASS_A_BIT_SIZE + \
                                                                           MVS_AMR_WB_MODE_1585_CLASS_B_BIT_SIZE)
#define AMR_WB_MODE_1825_TOTAL_BITS (MVS_AMR_WB_MODE_1825_CLASS_A_BIT_SIZE +  \
                                                                           MVS_AMR_WB_MODE_1825_CLASS_B_BIT_SIZE)
#define AMR_WB_MODE_1985_TOTAL_BITS (MVS_AMR_WB_MODE_1985_CLASS_A_BIT_SIZE +  \
                                                                           MVS_AMR_WB_MODE_1985_CLASS_B_BIT_SIZE)
#define AMR_WB_MODE_2305_TOTAL_BITS (MVS_AMR_WB_MODE_2305_CLASS_A_BIT_SIZE +  \
                                                                         MVS_AMR_WB_MODE_2305_CLASS_B_BIT_SIZE)
#define AMR_WB_MODE_2385_TOTAL_BITS (MVS_AMR_WB_MODE_2385_CLASS_A_BIT_SIZE +  \
                                                                         MVS_AMR_WB_MODE_2385_CLASS_B_BIT_SIZE)

/* WB-AMR 6.60 kbps bit size */
#define MVS_AMR_WB_MODE_660_CLASS_A_BIT_SIZE    54
#define MVS_AMR_WB_MODE_660_CLASS_B_BIT_SIZE    78

/* WB-AMR 8.85 kbps bit size */
#define MVS_AMR_WB_MODE_885_CLASS_A_BIT_SIZE    64
#define MVS_AMR_WB_MODE_885_CLASS_B_BIT_SIZE    113

/* WB-AMR 12.65 kbps bit size */
#define MVS_AMR_WB_MODE_1265_CLASS_A_BIT_SIZE   72
#define MVS_AMR_WB_MODE_1265_CLASS_B_BIT_SIZE   181

/* WB-AMR 14.25 kbps bit size */
#define MVS_AMR_WB_MODE_1425_CLASS_A_BIT_SIZE   72
#define MVS_AMR_WB_MODE_1425_CLASS_B_BIT_SIZE   213

/* WB-AMR 15.85 kbps bit size */
#define MVS_AMR_WB_MODE_1585_CLASS_A_BIT_SIZE   72
#define MVS_AMR_WB_MODE_1585_CLASS_B_BIT_SIZE   245

/* WB-AMR 18.25 kbps bit size */
#define MVS_AMR_WB_MODE_1825_CLASS_A_BIT_SIZE   72
#define MVS_AMR_WB_MODE_1825_CLASS_B_BIT_SIZE   293

/* WB-AMR 19.85 kbps bit size */
#define MVS_AMR_WB_MODE_1985_CLASS_A_BIT_SIZE   72
#define MVS_AMR_WB_MODE_1985_CLASS_B_BIT_SIZE   325

/* WB-AMR 23.05 kbps bit size */
#define MVS_AMR_WB_MODE_2305_CLASS_A_BIT_SIZE   72
#define MVS_AMR_WB_MODE_2305_CLASS_B_BIT_SIZE   389

/* WB-AMR 23.85 kbps bit size */
#define MVS_AMR_WB_MODE_2385_CLASS_A_BIT_SIZE   72
#define MVS_AMR_WB_MODE_2385_CLASS_B_BIT_SIZE   405


#ifdef FEATURE_WCDMA_MRAB_TVM_MAC_OPT
#error code not present
#endif /* FEATURE_WCDMA_MRAB_TVM_MAC_OPT */


/*=====================================================================

 FUNCTION NAME  config_ul_mac

 PURPOSE        Processes UL MAC Configuration Data

 PARAMETERS     cfgptr - Pointer to the UL MAC Configuration data

 DESCRIPTION    This function performs the following functions:
                - saves configuration related information in local memory
                - if configured for RACH, indicates RLC to notify
                  when there is an SDU in its uplink queue
                - updates uplink logical channel information table
=====================================================================*/
void    config_ul_mac
(
    /* Pointer to UL MAC Configuration Data */
    mac_ul_config_type *cfgptr, 
    mac_ul_table_type *ul_table_ptr,
    mac_eul_table_type *eul_table_ptr
);
#ifdef FEATURE_MAC_HS_UL_QXDM_LOGGING
/*===========================================================================
FUNCTION: mac_eul_deallocate_mem_log_records

DESCRIPTION:
This function frees up dynamic memory allocated for EUL 0x430E log record pointer
===========================================================================*/
void mac_eul_deallocate_mem_log_records(mac_eul_table_type *eul_table_ptr);
#endif

#ifdef FEATURE_WCDMA_MULTIRATE_AMR
/*====================================================================
FUNCTION: mac_ul_compute_amr_rate()

DESCRIPTION:
  This function looks at the uplink tfcs table and computes the new
  rate based on what TFCI's are allowed at this point. Based on the
  allowed TFCI's the rate is computed and vocoder is configured with
  this new rate.

  This function is used to change rates for only the AMR. No effect
  on the PS.

DEPENDENCIES:
  VOCODER has to support multiple rates. RRC will have to support
  multiple rates for AMR.

RETURN VALUE:

  The function determines the rate with which MVS should be configured.
  It determines the lowest rate defined based on the tfcs table.
  It also determines if its a single_rate_amr or multi_rate_amr.

SIDE EFFECTS:
  There is a worst case delay of 40ms and we can lose upto one frame
  in the process of siwtching the rates

====================================================================*/
void
mac_ul_compute_amr_rate
(
  mac_amr_mode_info_type *mac_amr_mode_info_ptr, mac_ul_table_type *ul_table_ptr
);

/*====================================================================
FUNCTION: mac_ul_compute_amr_rate()

DESCRIPTION:
  This function calls the mvs function to set the rate

DEPENDENCIES:


RETURN VALUE:



SIDE EFFECTS:


====================================================================*/
void
mac_ul_set_amr_rate
(
  uint32 amr_mode, mac_ul_table_type *ul_table_ptr
);

#endif

/*===============================================================================================
FUNCTION NAME mac_asc_selection

DESCRIPTION   Selects an Access Service Class

RETURNS     0 - 7 Depending on the selected ASC
===============================================================================================*/
uint8 mac_asc_selection (rlc_ul_frame_format_type *chan_fmt, mac_ul_table_type *ul_table_ptr);

/*==============================================================================================
 FUNCTION NAME  void mac_update_ciphering_params

 DESCRIPTION : 1. MAC maintains 2 configurations for ciphering. Current Ciphering Parameters (cur_ul_cipher_info)
 and New Ciphering Parameters (new_ul_cipher_info).

 2. If there is an activation time pending, then new_ul_cipher_info is used. The parameters
 are copied into cur_ul_cipher_info at activation time.

 3. cfn_hfn_table is a map of the CFN and the corresponding HFN. CFN serves as the
 index of this array and the value at an index is the HFN. DL MAC reads from this
 table to get the HFN associated with a CFN. UL MAC maintains this table.

 4. When MAC starts ciphering for the first time and an activation time is pending,
   the sequence to increment the HFN is Reach Activation Time, ROLL OVER.
   To illustrate lets consider 2 cases

   Case 1: ciphering_activation_time = 12 and CFN = 30. Now the CFN has to roll over and then reach the
   activation time of 12. HFN would be incremented only after CFN rolls over after the activation time.

   Case 2: ciphering_activation_time = 220 and CFN = 5. Ciphering Activation time is reached before the
   CFN roll over and hence not treated as a special case.

   The flag first_time_rolled_over serves to address the case 1 above for us not to increment HFN
   if there is a CFN roll over and ciphering_activation_time is not reached.

 5. Subsequently every time there is a roll over MAC would increment the HFN.
 roll over is detected by the following piece of code :
 if ( cfn < prev_cfn). MAC maintains the last CFN value. This code is generic and would
 be able to handle even couple of build frame indications get missed for some reason.

 6. In HO_STEP1_CIPHER_CFG : the ciphering_activation_pending flag would be set to FALSE.
 The HFN would not be incremented even if we reach activation time and there are roll overs.
 This is a temporary state when WCDMA has received a request for Handover from GSM and WCDMA
 side has not yet sent the confirm. The same is applicable for hard handovers.

 7. In HO_STEP2_CIPHER_CFG : This configuration is received when WCDMA has sent a confirm.
 On receiving this configuration, MAC would increment the HFN by 1 when it hits activation
 time without waiting for the roll over. After this, the behaviour is as in the case of
 NON_HO_CIPHER_CFG.

===============================================================================================*/

void mac_update_ciphering_params
(
  uint8 cfn,
  sys_modem_as_id_e_type as_id
);

#ifdef FEATURE_WCDMA_CNT_C_SFN_DIFF

/*-------------------------------------------------------------------
FUNCTION uint32 mac_get_cnt_c_val()

DESCRIPTION
This function will be called by L1 to get teh count_c value

DEPENDENCIES
  None

RETURN VALUE
  The count_c value correspondign the current cfn will be returned to L1

SIDE EFFECTS
  None
--------------------------------------------------------------------*/
uint32 mac_get_cnt_c_val(uint8 cfn, sys_modem_as_id_e_type wsub_id);

#endif /* FEATURE_WCDMA_CNT_C_SFN_DIFF */

#ifdef FEATURE_WCDMA_HSUPA
/*-------------------------------------------------------------------
FUNCTION boolean MAC_CHECK_HSUPA_CONFIG_STATUS()

DESCRIPTION
This function will look to see if ul configuration is pending and
if yes then whether mac config has changed or not and whether
mac_reset_indicator is set to true.

DEPENDENCIES
  None

RETURN VALUE
  Boolean indicates that mac configuration has chnaged and mac_reset_indicator
  is set to TRUE

SIDE EFFECTS
  None
--------------------------------------------------------------------*/
mac_ul_config_type* mac_check_hsupa_config_status(mac_ul_table_type *ul_table_ptr);

#endif

/*================================================================================================
 FUNCTION NAME  init_ul_mac_ciphering_params

 DESCRIPTION  This function initialises UL MAC ciphering parameters
================================================================================================*/
void  init_ul_mac_ciphering_params
(
  mac_ul_table_type *ul_table_ptr
);


/*=================================================================================================
 FUNCTION NAME  proc_ul_mac_tfcc_command

 PURPOSE    This function process UL TFCC config Commands
================================================================================================*/

void proc_ul_mac_tfcc_command
(
  mac_tfc_subset_restriction_info_type *tfcc_config_ptr, mac_ul_table_type *ul_table_ptr
);

/*=================================================================================================
 FUNCTION NAME    proc_ul_cctrch_info

 DESCRIPTION    This function processes uplink CCTrCh information
=================================================================================================*/
void    proc_ul_cctrch_info
(
  /* Pointer to ordered config data of UL CCTrCh */
  mac_ul_cctrch_config_type *cctrch_ptr,
  mac_ul_table_type *ul_table_ptr
);


#ifdef FEATURE_WCDMA_MAC_UL_ZI_REDUCTION
#error code not present
#endif /* FEATURE_WCDMA_MAC_UL_ZI_REDUCTION */

/*===============================================================================================
FUNCTION NAME mac_ul_get_trch_type

DESCRIPTION   This function returns the up-link transport channel type corresponding to given RLC & RB IDs
===============================================================================================*/

log_wcdma_trch_type mac_ul_get_trch_type
  (
   /* RLC logical channel buffer identifier (0-20) */
   rlc_lc_id_type rlc_id, 

   /* Radio Bearer Identifier (0-31) */
   rb_id_type rb_id,

   mac_ul_table_type *ul_table_ptr, mac_eul_table_type *eul_table_ptr
  );


/*===============================================================================================
FUNCTION NAME mac_ul_check_edch_trch

DESCRIPTION   This function checks if given rlc-id and rb-id pair map to a UL-EDCH transport channel
===============================================================================================*/

log_wcdma_trch_type mac_ul_check_edch_trch
(
   /* RLC logical channel buffer identifier (0-20) */
   rlc_lc_id_type rlc_id, 

   /* Radio Bearer Identifier (0-31) */
   rb_id_type rb_id, mac_eul_table_type *eul_table_ptr
);

#ifdef FEATURE_QSH_MDUMP

QSH_MDUMP_FN_ATTR
void wmac_qsh_mdump_collect(sys_modem_as_id_e_type as_id, qsh_client_cb_params_s* qsh_cb_params_ptr);
QSH_MDUMP_FN_ATTR
void wmac_qsh_mac_ul_table_mdump_collect(sys_modem_as_id_e_type as_id);
QSH_MDUMP_FN_ATTR
void wmac_qsh_mac_dl_table_mdump_collect(sys_modem_as_id_e_type as_id);
QSH_MDUMP_FN_ATTR
void wmac_qsh_mac_data_table_mdump_collect(sys_modem_as_id_e_type as_id);
QSH_MDUMP_FN_ATTR
void wmac_qsh_mac_eul_table_mdump_collect(sys_modem_as_id_e_type as_id);

#endif

#endif


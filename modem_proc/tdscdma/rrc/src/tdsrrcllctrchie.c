/*===========================================================================

                L3 - RRC Lower Layer Controller Transport Channel IEs

DESCRIPTION
  This file contains functions necessary for the RRC Lower Layer Controller
  Ordered Config Transport Channel Information Element Processing.

Copyright (c) 2000-2012 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcllctrchie.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------   ---     ---------------------------------------------------------
14/11/12   yzh     CR 414921 fixed.
17/04/12   ysh     CR 352204 fixed.
01/26/12   mkh     KW fix
11/10/11   zwj     Modified OTA TFCI according to the way we stored .
10/13/11   mkh     Now using correct macro. Earlier it was coming from W code.
09/17/11   zwj     Fixed UL TB size mac header size.
08/26/11   yzh       Add add_queue_bitmask in order to check multiple delete mac_queue info in OTA message.
                            Modify bug about remove old mapping if mac_queue is available in other mac_dflow,
                            And check old mapping when setup not recofigure mac_dflow.
08/17/11   zwj    fix bug:add judgement to get.tfc_subset
08/04/11   ysh    delete save pdsch info.
07/26/11   ttl     mainline FEATURE_DL_BLER_QUAL_CTL.
07/26/11   ysh    modify bug: 7.2.3.15 RBsetup fail due to wrong nTrch from RRC to L1 ul.
06/16/11   bj      Added R9 version IE: DL Add Reconfig TrCH info list.
06/14/11   yzh     Made changes from FDD to TDD in R8
06/06/11   ysh     Modified OTA TFCI according to the way we stored.
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
02/07/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/28/11   rl      Added the not operator for RRC_CHECK_COMMON_MSG_TYPE_PTR 
                   found during testing
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
01/06/11   as      Made changes to delete commonn queue info while moving
                   to non EFACH cell or DCH state
01/17/11   zwj     Added e-DCH info for HSUPA.
01/06/11   ysh     IE "E-DCH Transmission Time Interval" delete. 3GPP-8.6.5.5(3.6.16).
11/12/10   yzh     (3GPP: 8.5.25&10.3.6.23a)Modify Actions related to HS_DSCH_RECEPTION variable
                   and Downlink HS-PDSCH Information(FDD->TDD).
11/09/10   ysh     RLC size of Transport Format Set calculate method FDD->TDD. 3GPP-8.6.5.1/11.1(3.2.32)
11/11/10   yzh     (3GPP: 10.3.5.8)Del IE “PowerOffsetPp-m”(FDD) and IE “gainFactorBetaC”(FDD)
                   in Power Offset Information and (3GPP: 8.6.6.28a)Del Downlink F-DPCH info
                   common for all radio links(FDD).
11/04/10   yzh     (3GPP 10.3.6.21) modify Downlink DPCH info for each RL from FDD to TDD.
11/01/10   yzh     (3GPP 8.6.6.6) modify Uplink DPCH info from FDD to TDD
11/02/10   zwj     Added code to handle REL4 OTA Message.
11/01/10   ysh     CCTrCH list is TDD only, Transport channel information common save modify.
                              3GPP-8.6.5.9/8.6.5.10(3.2.8)

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
  #include "tdsmacl1rrcif.h"
#include "msg.h"
#include "tdsrlcrrcif.h"
#include "tdsrrcasn1.h"
#include "tdsrrcdata.h"
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcsibdb.h"
#include "tdsuecomdef.h"
#include "tdsrrcscmgr.h"
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
#include "tdsrrcmeasi.h"
#endif

#include "tdsrrcnv.h"
#include "tdsrrcrbcommon.h"
/*===================================================================
                        DATA DECLARATIONS
=====================================================================*/

/* This is a temporary storage buffer for all downlink DCH specific info */
tdsrrcllc_dl_dch_info_buffer_type  tdsdl_dch_info_buffer; 

/* This is a temporary storage buffer for all uplink DCH info */
tdsrrcllc_ul_dch_info_buffer_type  tdsul_dch_info_buffer;

tdsrb_mapping_info_type tdsrrc_ordered_hanging_rb_mapping[TDSMAX_HANGING_RB_MAPPING_INFO];
tdsrb_mapping_info_type tdsrrc_current_hanging_rb_mapping[TDSMAX_HANGING_RB_MAPPING_INFO];
extern boolean tdsul_tfcs_needed;
extern boolean tdsdl_tfcs_needed;
extern boolean tdsul_tfcs_rcvd; 
extern boolean tdsdl_tfcs_rcvd; 
extern boolean tdsrrcllc_find_trch_id_in_ul_trch_list_r8
(
  tdsrrc_UL_AddReconfTransChInfoList_r8 * ul_trch_add_reconfig_ptr,
  tr_ch_id_type tdstrch_id
);
/*===================================================================
                        CONSTANTS
=====================================================================*/

/*===================================================================
                        FORWARD DECLARATIONS
=====================================================================*/

/*===================================================================
*                       FUNCTION PROTOTYPES
====================================================================*/

/*===================================================================
                            FUNCTIONS
=====================================================================*/

/*===================================================================
FUNCTION: tdsrrcllc_get_ul_trch_dynamic_tfs_ie_ptr()

DESCRIPTION :   Gets pointer to Transport Format Set information in the 
              received IE Saves TTI type in MAC Ordered Config Database
              Sets 'rv' to 'FAILURE' or 'SUCCESS' depending on whether
              the chosen TTI types is supported or not supported or 
              invalid.

DEPENDENCIES:   None

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_trch_dynamic_tfs_ie_ptr
(
  tdsrrc_DedicatedTransChTFS            *ie_ptr,
  tdsrrc_DedicatedDynamicTF_InfoList   **tfs_ptr,
  tdsmac_ul_tfs_type                    *oc_tfs_ptr
)
{
  uecomdef_status_e_type        rv = FAILURE;

  /* Pick TFS pointer based on the TTI type */
  switch (ie_ptr->tti.t){

    /* 10 msec TTI */
    case T_tdsrrc_DedicatedTransChTFS_tti_tti10:
      /* Pointer to 10 msec TFC information */
      *tfs_ptr = ie_ptr->tti.u.tti10;
      /* Save TTI type info in UL MAC TFCS structure */
      oc_tfs_ptr->tti_info = TDSL1_TTI_10MS;
      /* Set Return value to SUCCESS */
      rv = SUCCESS;
      break;

    /* 20 msec TTI */
    case T_tdsrrc_DedicatedTransChTFS_tti_tti20:
      /* Pointer to 10 msec TFC information */
      *tfs_ptr = ie_ptr->tti.u.tti20;
      /* Save TTI type info in UL MAC TFCS structure */
      oc_tfs_ptr->tti_info = TDSL1_TTI_20MS;
      /* Set Return value to SUCCESS */
      rv = SUCCESS;
      break;

    /* 40 msec TTI */
    case T_tdsrrc_DedicatedTransChTFS_tti_tti40:
      /* Pointer to 10 msec TFC information */
      *tfs_ptr = ie_ptr->tti.u.tti40;
      /* Save TTI type info in UL MAC TFCS structure */
      oc_tfs_ptr->tti_info = TDSL1_TTI_40MS;
      /* Set Return value to SUCCESS */
      rv = SUCCESS;
      break;

    /* 80 msec TTI */
    case T_tdsrrc_DedicatedTransChTFS_tti_tti80:
      /* Pointer to 10 msec TFC information */
      *tfs_ptr = ie_ptr->tti.u.tti80;
      /* Save TTI type info in UL MAC TFCS structure */
      oc_tfs_ptr->tti_info = TDSL1_TTI_80MS;
      /* Set Return value to SUCCESS */
      rv = SUCCESS;
      break;

    /* Dynamic TTI */
    case T_tdsrrc_DedicatedTransChTFS_tti_dynamic:
      /* Log an error message */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Dynamic TTI's are not supported in TDD");
      /* Set Return value to FAILURE */
      rv = FAILURE;
      break;

    /* Invalid TTI type */
    default:
      /* Log an error message */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid TTI type %d rcv'd in TFS info",oc_tfs_ptr->tti_info);
      /* Set Return value to Failure */
      rv = FAILURE;
      break;
  }

  return rv;

}


/*===================================================================
FUNCTION       : tdsrrcllc_get_ul_ctfc_bit_info

DESCRIPTION : Gets CTFC info from IE and saves it in UL MAC Data 
              Structure

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
=====================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_ctfc_bit_info
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_TFCS_ReconfAdd  *ie_ptr,
  uint32 n
)
{
  uecomdef_status_e_type  rv  =   SUCCESS;

  /* Pointer to the first item in the list */
  tdsrrc_TFCS_ReconfAdd_ctfc2Bit     *ie_ctfc2_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc4Bit     *ie_ctfc4_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc6Bit     *ie_ctfc6_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc8Bit     *ie_ctfc8_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc12Bit    *ie_ctfc12_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc16Bit    *ie_ctfc16_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc24Bit    *ie_ctfc24_ptr;

  /* TFC index, needed by TDSGET_UL_CTFC_BIT_INFO macro */
  uint32  tfc_idx = 0;
  uint32  i = 0;
  uint32  idx=0;

  /* Pointer to TFC info in Ordered Config Data Structure */
  tdsmac_ul_tfc_type     *oc_tfc_ptr;

  unsigned power_present = 0;
  tdsrrc_PowerOffsetInformation *power_info;

    while ((n >idx) && (tfc_idx < TDSL1_UL_TFC_MAX)){

      /* Get TFC index */
      /* when type is T_tdsrrc_ExplicitTFCS_Configuration_replacement, config_ptr-> removed_tfci_list  could present  */
      if ((config_ptr->num_removed_tfcis > 0) &&  
         (config_ptr->num_removed_tfcis < TDSUE_MAX_TFC)){ 

        /* Get the first available position */ 
        tfc_idx = config_ptr->removed_tfci_list[0];

        /* Decrement the removed TFCI count */
        config_ptr->num_removed_tfcis--;

        /* Adjust the removed tfc list */

        for (i=0; i < config_ptr->num_removed_tfcis; i++)
          config_ptr->removed_tfci_list[i] = config_ptr->removed_tfci_list[i+1];
      } 
      /* There are no holes in the TFCS Table */
      else {
        /* TFC is added at the end */
        tfc_idx = config_ptr->ul_tfcs.ntfc;
      }

      /* TFC Index is within maximum value */
      if (tfc_idx < TDSL1_UL_TFC_MAX) {

        /* Pointer to TFC info info OC data */
        oc_tfc_ptr = &config_ptr->ul_tfcs.tfc_info[tfc_idx];

        /* Set TFC Control Flag to allowed */
        oc_tfc_ptr->control_flag = TDSTFC_ALLOWED;

        /* Save the CTFC value */
        switch (ie_ptr->ctfcSize.t)
        {
          /* 2 Bit CTFC */
          case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc2Bit:
            ie_ctfc2_ptr = ie_ptr->ctfcSize.u.ctfc2Bit;
            oc_tfc_ptr->ctfc = (uint32)(ie_ctfc2_ptr->elem[idx].ctfc2);
            power_present = ie_ctfc2_ptr->elem[idx].m.powerOffsetInformationPresent;
            power_info = &ie_ctfc2_ptr->elem[idx].powerOffsetInformation;
            break;

          /* 4 Bit CTFC */
          case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc4Bit:
            ie_ctfc4_ptr = ie_ptr->ctfcSize.u.ctfc4Bit;
            oc_tfc_ptr->ctfc = (uint32)(ie_ctfc4_ptr->elem[idx].ctfc4);
            power_present = ie_ctfc4_ptr->elem[idx].m.powerOffsetInformationPresent;
            power_info = &ie_ctfc4_ptr->elem[idx].powerOffsetInformation;
            break;

          /* 6 Bit CTFC */
          case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc6Bit:
            ie_ctfc6_ptr = ie_ptr->ctfcSize.u.ctfc6Bit;
            oc_tfc_ptr->ctfc = (uint32)(ie_ctfc6_ptr->elem[idx].ctfc6);
            power_present = ie_ctfc6_ptr->elem[idx].m.powerOffsetInformationPresent;
            power_info = &ie_ctfc6_ptr->elem[idx].powerOffsetInformation;
            break;

          /* 8 Bit CTFC */
          case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc8Bit:
            ie_ctfc8_ptr = ie_ptr->ctfcSize.u.ctfc8Bit;
            oc_tfc_ptr->ctfc = (uint32)(ie_ctfc8_ptr->elem[idx].ctfc8);
            power_present = ie_ctfc8_ptr->elem[idx].m.powerOffsetInformationPresent;
            power_info = &ie_ctfc8_ptr->elem[idx].powerOffsetInformation;
            break;

          /* 12 Bit CTFC */
          case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc12Bit:
            ie_ctfc12_ptr = ie_ptr->ctfcSize.u.ctfc12Bit;
            oc_tfc_ptr->ctfc = (uint32)(ie_ctfc12_ptr->elem[idx].ctfc12);
            power_present = ie_ctfc12_ptr->elem[idx].m.powerOffsetInformationPresent;
            power_info = &ie_ctfc12_ptr->elem[idx].powerOffsetInformation;
            break;

          /* 16 Bit CTFC */
          case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc16Bit:
            ie_ctfc16_ptr = ie_ptr->ctfcSize.u.ctfc16Bit;
            oc_tfc_ptr->ctfc = (uint32)(ie_ctfc16_ptr->elem[idx].ctfc16);
            power_present = ie_ctfc16_ptr->elem[idx].m.powerOffsetInformationPresent;
            power_info = &ie_ctfc16_ptr->elem[idx].powerOffsetInformation;
            break;

          /* 24 Bit CTFC */
          case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc24Bit:
            ie_ctfc24_ptr = ie_ptr->ctfcSize.u.ctfc24Bit;
            oc_tfc_ptr->ctfc = (uint32)(ie_ctfc24_ptr->elem[idx].ctfc24);
            power_present = ie_ctfc24_ptr->elem[idx].m.powerOffsetInformationPresent;
            power_info = &ie_ctfc24_ptr->elem[idx].powerOffsetInformation;
            break;

          /* Invalid CTFC size */
          default:
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd invalid CTFC bit size");
            rv = FAILURE;
            return (rv);
        }

        /* Save the Power Offset Information */
        if ((n- 1) == idx) {
        rv = tdsrrcllc_get_ul_gain_factor_info (
                         TRUE,
                         power_present,
                         config_ptr->rrc_ul_cctrch_params.gain_factor_parms,
                         power_info,(uint8)tfc_idx);
        }
        else {
          rv = tdsrrcllc_get_ul_gain_factor_info (
                         FALSE,
                         power_present,
                         config_ptr->rrc_ul_cctrch_params.gain_factor_parms,
                         power_info,(uint8)tfc_idx);
        }

        /* Get the pointer to the next TFC */
        idx++;

        /*Increment TFC count */
        if(tfc_idx == config_ptr->ul_tfcs.ntfc) { 
          config_ptr->ul_tfcs.ntfc++;
          config_ptr->rrc_ul_cctrch_params.num_tfci++; 
        }
      } 
      /* TFC index has exceeded the maximum value */ 
      else { 
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid number of Transport Formats");
        rv = FAILURE;
        break;
      }
    }
    return (rv);
}

/*====================================================================
FUNCTION       : tdsrrcllc_get_dl_tfcs_explicit_cfg_recfg

DESCRIPTION:
  This macro is gets the CTFC values from the TFCS information in
  case of the TFCS explicit complete reconfiguration.
  
  DEPENDENCIES:
    None.
  
  RETURN VALUE:
    SUCCESS/FAILURE
  
  SIDE EFFECTS:
    None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_tfcs_explicit_cfg_recfg
(
  tdsrrc_TFCS_ReconfAdd *tfc_add_cfg_info,
  tdsrrcllc_dl_ctfc_info_struct_type *dl_tfcs_info,
  uint32 n
) 
{
  uint8 cur_ctfc_entry_index = 0;
  tdsrrcllc_dl_ctfc_entry_info_struct_type *this_ctfc_entry;
  uint32 idx =0;

  while (n > idx) 
  { 
    if (cur_ctfc_entry_index >= TDSUE_MAX_TFC) 
    { 
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Too many TFC,Exp cfg complete recfg"); 
      return FAILURE; 
    } 
    this_ctfc_entry = &dl_tfcs_info->ctfc_entry_data[cur_ctfc_entry_index];
    this_ctfc_entry->state = TDSALLOWED; 
    switch(tfc_add_cfg_info->ctfcSize.t)
    {
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc2Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc2Bit->elem[idx].ctfc2);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc4Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc4Bit->elem[idx].ctfc4);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc6Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc6Bit->elem[idx].ctfc6);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc8Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc8Bit->elem[idx].ctfc8);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc12Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc12Bit->elem[idx].ctfc12);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc16Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc16Bit->elem[idx].ctfc16);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc24Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc24Bit->elem[idx].ctfc24);
        break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC DL Explicit TFCS cfg CTFC size");
        return FAILURE;
    }

    idx++; 
    cur_ctfc_entry_index++; 
  }

  dl_tfcs_info->num_ctfc_entry = cur_ctfc_entry_index;

  return SUCCESS;

}

/*====================================================================
FUNCTION       : tdsrrcllc_get_dl_tfcs_explicit_cfg_add

DESCRIPTION:
  This macro is gets the CTFC values from the TFCS information in
  case of the TFCS explicit TFC ADD.
  
  DEPENDENCIES:
    None.
  
  RETURN VALUE:
    SUCCESS/FAILURE
  
  SIDE EFFECTS:
    None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_tfcs_explicit_cfg_add
(
  tdsrrc_TFCS_ReconfAdd *tfc_add_cfg_info,
  tdsrrcllc_dl_ctfc_info_struct_type *dl_tfcs_info,
  uint32 n
)
{
  uint8 cur_ctfc_entry_index = 0;
  tdsrrcllc_dl_ctfc_entry_info_struct_type *this_ctfc_entry;
  uint32 idx =0;

  while (n > idx)
  {
    if (cur_ctfc_entry_index >= TDSUE_MAX_TFC)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not enough free TFCI,Exp cfg ADD");
      return FAILURE;
    }
    while (dl_tfcs_info->ctfc_entry_data[cur_ctfc_entry_index].state != TDSINVALID)
    {
      cur_ctfc_entry_index++;
      if (cur_ctfc_entry_index >= TDSUE_MAX_TFC)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not enough free TFCI,Exp cfg ADD");
        return FAILURE;
      }
    }

    if (cur_ctfc_entry_index >= TDSUE_MAX_TFC)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not enough free TFCI,Exp cfg ADD");
      return FAILURE;
    }

    this_ctfc_entry = &dl_tfcs_info->ctfc_entry_data[cur_ctfc_entry_index];
    this_ctfc_entry->state = TDSALLOWED;
    switch(tfc_add_cfg_info->ctfcSize.t)
    {
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc2Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc2Bit->elem[idx].ctfc2);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc4Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc4Bit->elem[idx].ctfc4);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc6Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc6Bit->elem[idx].ctfc6);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc8Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc8Bit->elem[idx].ctfc8);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc12Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc12Bit->elem[idx].ctfc12);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc16Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc16Bit->elem[idx].ctfc16);
        break;
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc24Bit:
        this_ctfc_entry->ctfc_value = (uint32) (tfc_add_cfg_info->ctfcSize.u.ctfc24Bit->elem[idx].ctfc24);
        break;
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC DL Explicit TFCS cfg CTFC size");
        return FAILURE;
    }

    idx++;
    cur_ctfc_entry_index++;
  }
 
  if (cur_ctfc_entry_index  > dl_tfcs_info->num_ctfc_entry)
  {
    dl_tfcs_info->num_ctfc_entry = cur_ctfc_entry_index;
  }

  return SUCCESS;
}

/*====================================================================
FUNCTION: tdsrrcllc_get_ul_trch_dynamic_tfs_ie_common()

DESCRIPTION:
  Gets pointer to Transport Format Set information in the 
              received IE Saves TTI type in MAC Ordered Config Database
              Sets 'rv' to 'FAILURE' or 'SUCCESS' depending on whether
              the chosen TTI types is supported or not supported or 
              invalid.
    
DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_trch_dynamic_tfs_ie_common
(
  /* Pointer to the ASN.1 Dedicated Transport Channel TFS information */
  tdsrrc_CommonTransChTFS_LCR            *ie_ptr,
  tdsrrc_CommonDynamicTF_InfoList        *tfs_ptr,

  /* Pointer to Transport Channel TFS information pointer in OC */
  tdsmac_ul_tfs_type                     *oc_tfs_ptr,

  /* Pointer to the Semi static parms information pointer in OC */
  tdsl1_ul_semi_static_parms_struct_type *oc_ssp_ptr
)
{
  uecomdef_status_e_type        rv = FAILURE;
  
  /* Pick TFS pointer based on the TTI type */
  switch (ie_ptr->tti.t){

    /* 5 msec TTI */
    case T_tdsrrc_CommonTransChTFS_LCR_tti_tti5:

      /* Pointer to 5 msec TFC information */
      memscpy(tfs_ptr, sizeof(tdsrrc_CommonDynamicTF_InfoList),ie_ptr->tti.u.tti5, sizeof(tdsrrc_CommonDynamicTF_InfoList)); 

      /* Save TTI type info in UL MAC TFCS structure */
      oc_tfs_ptr->tti_info = TDSL1_TTI_5MS;

      /* Set Return value to SUCCESS */
      rv = SUCCESS;

      break;

    /* 10 msec TTI */
    case T_tdsrrc_CommonTransChTFS_LCR_tti_tti10:

      /* Pointer to 10 msec TFC information */
      memscpy(tfs_ptr, sizeof(tdsrrc_CommonDynamicTF_InfoList),ie_ptr->tti.u.tti10, sizeof(tdsrrc_CommonDynamicTF_InfoList)); 

      /* Save TTI type info in UL MAC TFCS structure */
      oc_tfs_ptr->tti_info = TDSL1_TTI_10MS;

      /* Set Return value to SUCCESS */
      rv = SUCCESS;

      break;

    /* 20 msec TTI */
    case T_tdsrrc_CommonTransChTFS_LCR_tti_tti20:

      /* Pointer to 10 msec TFC information */
      memscpy(tfs_ptr,sizeof(tdsrrc_CommonDynamicTF_InfoList), ie_ptr->tti.u.tti20, sizeof(tdsrrc_CommonDynamicTF_InfoList)); 

      /* Save TTI type info in UL MAC TFCS structure */
      oc_tfs_ptr->tti_info = TDSL1_TTI_20MS;

      /* Set Return value to SUCCESS */
      rv = SUCCESS;

      break;

    /* 40 msec TTI */
    case T_tdsrrc_CommonTransChTFS_LCR_tti_tti40:

      /* Pointer to 10 msec TFC information */
      memscpy(tfs_ptr,sizeof(tdsrrc_CommonDynamicTF_InfoList), ie_ptr->tti.u.tti40, sizeof(tdsrrc_CommonDynamicTF_InfoList)); 

      /* Save TTI type info in UL MAC TFCS structure */
      oc_tfs_ptr->tti_info = TDSL1_TTI_40MS;

      /* Set Return value to SUCCESS */
      rv = SUCCESS;

      break;

    /* 80 msec TTI */
    case T_tdsrrc_CommonTransChTFS_LCR_tti_tti80:

      /* Pointer to 10 msec TFC information */
      memscpy(tfs_ptr, sizeof(tdsrrc_CommonDynamicTF_InfoList),ie_ptr->tti.u.tti80, sizeof(tdsrrc_CommonDynamicTF_InfoList)); 

      /* Save TTI type info in UL MAC TFCS structure */
      oc_tfs_ptr->tti_info = TDSL1_TTI_80MS;

      /* Set Return value to SUCCESS */
      rv = SUCCESS;

      break;

    /* Dynamic TTI */
    case T_tdsrrc_CommonTransChTFS_LCR_tti_dynamic:

      /* Log an error message */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Dynamic TTI's are not supported in FDD");

      /* Set Return value to FAILURE */
      rv = FAILURE;

      break;

    /* Invalid TTI type */
    default: 

      /* Log an error message */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid TTI type %d rcv'd in TFS info",oc_tfs_ptr->tti_info);

      /* Set Return value to Failure */
      rv = FAILURE;
      break;
  }
  return rv;
}

/*====================================================================
FUNCTION: tdsrrcllc_get_tfs_num_tb()

DESCRIPTION:
  This function gets the number of TB from the TF info. It is a
  utility function for getting the part of TFS information.
    
DEPENDENCIES:
  None

RETURN VALUE:
  number of transport blocks.
  0xFF in case of error.

SIDE EFFECTS:
  None.
====================================================================*/
uint8 tdsrrcllc_get_tfs_num_tb
(
  /* RRC Dedicated TFS Dynamic TF info */
  tdsrrc_NumberOfTransportBlocks *ie_ptr
)
{
  /* function local variables */
  /* ------------------------ */

  uint8 num_tb = 0;

  switch (ie_ptr->t)
  {
    case T_tdsrrc_NumberOfTransportBlocks_zero:
      num_tb = 0;
      break;
    case T_tdsrrc_NumberOfTransportBlocks_one: 
      num_tb = 1;
      break;
    case T_tdsrrc_NumberOfTransportBlocks_small_:
      if (TDSRRC_DL_MAX_TRANSPORT_BLOCKS < ie_ptr->u.small_)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC get TB num is %d,current UE capability is TDSRRC_DL_MAX_TRANSPORT_BLOCKS",ie_ptr->u.small_);
        return TDSRRCLLC_INVALID_NUM_TB;
      }

      num_tb = (uint8)ie_ptr->u.small_;
      break;
    case T_tdsrrc_NumberOfTransportBlocks_large_:
      if (TDSRRC_DL_MAX_TRANSPORT_BLOCKS < ie_ptr->u.large_)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC get TB num is %d,current UE capability is TDSRRC_DL_MAX_TRANSPORT_BLOCKS",ie_ptr->u.large_);
        return TDSRRCLLC_INVALID_NUM_TB;
      }

      num_tb = (uint8)ie_ptr->u.large_;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC DL number of TB type %d",ie_ptr->t);
      return TDSRRCLLC_INVALID_NUM_TB;
  }

  return num_tb;
} /* END tdsrrcllc_get_tfs_num_tb() */

/*====================================================================
FUNCTION: tdsrrcllc_dch_usch_add_trch_info()

DESCRIPTION:
  

DEPENDENCIES:
  None

RETURN VALUE:


SIDE EFFECTS:
  None.
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_dch_usch_add_trch_info
(
  tr_ch_id_type tdstrch_id,
  tdsrrc_TransportFormatSet * transport_format_set)
{
    /* local transport channel variables */
     uint8 trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;

    /*
    * Use the Transport Channel Index and next state to place the TFS in 
    * the correct slot in the MAC TFS array.
    */
    if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
    {
      /* Get the transport channel index for this transport channel ID.
       * Note: The index should already be there.
       */
      trch_idx = tdsrrcllc_find_ul_dch_trch_idx(tdsordered_config_ptr,
                                             tdstrch_id);
      if((trch_idx == TDSRRCLLC_INVALID_TRCH_IDX) ||
        (trch_idx == TDSUE_MAX_TRCH)
        )
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"UL TDSDCH TrCH ID %d,no idx found",tdstrch_id);
        return(FAILURE);
      }
      if(tdsrrcllc_restore_ul_dch_rb_mapping_info(trch_idx) == FAILURE)
      {
        /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"UL TDSDCH TrCH ID %d,info not correct in Standalone RB-Mapping",tdstrch_id);
      }

      /* get the Transport Format Set for this TrCH */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get UL TFS for TrCH ID %d",tdstrch_id);
  
      if(tdsrrcllc_get_ul_transport_format_set(tdsordered_config_ptr, 
                                     transport_format_set, NULL,
                                     (uint8)trch_idx)
          == FAILURE)
      {
        return FAILURE;
      }
    /*Find Rbs mapped to this DCH and fill them in rb_list_mapped_to_dch if their RLC
    PDU size has changed.*/ 

      tdsrrcllc_get_rb_for_rb_list_mapped_to_dch(tdstrch_id, 
                tdsordered_config_ptr->rrc_state_indicator, TDSUL);
    
      /* Indicate that Uplink MAC and L1 need to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
      tdsordered_config_ptr->reconfig_needed.uplink_l1 = TRUE;
    }
    else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH)
    {
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_FACH not supported for TrCH");
       return FAILURE;
    }
    else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH)
    {
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_PCH not supported for TrCH");
       return FAILURE;
    }
    else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
    {
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"URA_PCH not supported for TrCH");
       return FAILURE;
    }
    else
    {
       ERR_FATAL("Unknown state not supported for TrCH",0,0,0);
       return FAILURE; /*lint !e527  We've taken care of all possible enum values
                                     in the if/else clauses, but we want to leave
                                     this here just in case. */
    }
    return SUCCESS;
  }
/*============================================================================
FUNCTION: tdsrrcllc_convert_num_harq_process()

DESCRIPTION:
  
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/

static uint8 tdsrrcllc_convert_num_harq_process
(
 tdsrrc_HARQ_Info_r7* harq_info
)
{
    uint8 num_harq_procs=0;
    switch(harq_info->numberOfProcesses)
    {
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n1 :
        num_harq_procs= 1;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n2 :
        num_harq_procs= 2;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n3:
        num_harq_procs= 3;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n4 :
        num_harq_procs= 4;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n5:
        num_harq_procs= 5;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n6:
        num_harq_procs= 6;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n7:
        num_harq_procs= 7;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n8:
        num_harq_procs= 8;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n12:
        num_harq_procs= 12;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n14:
        num_harq_procs= 14;
        break;
      case tdsrrc_HARQ_Info_r7_numberOfProcesses_n16:
        num_harq_procs= 16;
        break;
        
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid value for Num Harq processes %d",harq_info->numberOfProcesses);    
        break;
    }

    return num_harq_procs;
}
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
)
{
  /* Set HARQ Information */
  tdsrrc_HARQ_Info_r7_memorySize* harq_head_ptr = NULL;

  uint8 cnt = 0;
#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  tdsordered_config_ptr->harq_present = TRUE;
#endif
  TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Set HARQ info in OC");

  /*Indicate that HSDSCH Info is present which would be used later to set bit mask when HSDPA
      is already ACTIVE */
  tdshsdpa_msg_params.msg_params_present = TRUE;
  tdshsdpa_msg_params.hsdsch_info_present = TRUE;
  
  tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc = 
    tdsrrcllc_convert_num_harq_process( harq_info_ptr);
  
  TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:No of processes %d",harq_info_ptr->numberOfProcesses); 

  if (TDSRRC_CHECK_COMMON_MSG_TYPE(harq_info_ptr->memoryPartitioning,
    rrc_HARQ_Info_r7_memoryPartitioning_implicit))
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_part_info_valid = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Implicit Mem Partition"); 
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_part_info_valid = TRUE;
    
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Explicit Mem Partition"); 
    
    if ((harq_head_ptr = &harq_info_ptr->memoryPartitioning.u.explicit_->memorySize) == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:HARQ Mem Part Ptr NULL");
      return FAILURE;
    }
#ifdef FEATURE_TDSCDMA_MIMO
    while ((harq_head_ptr->n > cnt) && (cnt < TDSL1_DL_CCTRCHCFG_HS_DSCH_HARQ_MAX_PROC_NO_MIMO))
#else
    while ((harq_head_ptr->n > cnt) && (cnt < TDSL1_DL_CCTRCHCFG_HS_DSCH_HARQ_MAX_PROC))
#endif
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_size_type[cnt] = 
        (tdsl1_dl_hs_dsch_harq_mem_sz_enum_type) harq_head_ptr->elem[cnt];
      
      cnt++;
    }
#ifdef FEATURE_TDSCDMA_MIMO
    if((harq_info_ptr->memoryPartitioning.u.explicit_->m.additionalMemorySizesForMIMOPresent) &&
         (harq_info_ptr->memoryPartitioning.u.explicit_->additionalMemorySizesForMIMO.n > 0 ))
    {
        uint32 idx =0;
        tdsrrc_HARQ_Info_r7_additionalMemorySizesForMIMO * add_harq_info_head_ptr = NULL;
        add_harq_info_head_ptr = &harq_info_ptr->memoryPartitioning.u.explicit_->additionalMemorySizesForMIMO;

        tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mimo_additional_mem_part_info_valid = TRUE;
        while ((add_harq_info_head_ptr->n > idx) && (cnt < TDSL1_DL_CCTRCHCFG_HS_DSCH_HARQ_MAX_PROC_MIMO))
        {
          tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_size_type[cnt++] = 
            (tdsl1_dl_hs_dsch_harq_mem_sz_enum_type) add_harq_info_head_ptr->elem[idx];
          
          idx++;          
        }
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIMO+: Additioanl memory for Mimo num harq %d",tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc);    
    }
    else if(tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc > TDSL1_DL_CCTRCHCFG_HS_DSCH_HARQ_MAX_PROC_NO_MIMO )
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHSPA+: Additional memory for Mimo not provided for num process %d > 8 ",tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc);
        return FAILURE;
    }
#else /*FEATURE_TDSCDMA_MIMO*/
    if(harq_info_ptr->memoryPartitioning.u.explicit_->m.additionalMemorySizesForMIMOPresent)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: Additioanl memory for Mimo not supported");
    }
#endif /*FEATURE_TDSCDMA_MIMO*/
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:No of HARQ mem sizes %d",cnt);
  }
  
  return SUCCESS;

}


/*====================================================================
FUNCTION: tdsrrcllc_get_tfs_ded_rlc_size()

DESCRIPTION:
  This function gets the RLC size for the deicated channel. The RLC size
  returned by this function is RLC PDU size. It is a utility function
  for getting the TFS info parts.
  
  See the 25.331 'Message and Information Element abstract syntax
  (with ASN.1)' PDU definition for TrCh IEs 2000-12 V3.5.0 Page 492-498
  for getting the RLC size for Bitmode or Octet mode parts.
    
DEPENDENCIES:
  None

RETURN VALUE:
  16 bit RLC size.
  0xFFFF in case of error

SIDE EFFECTS:
  None.
====================================================================*/
uint32 tdsrrcllc_get_tfs_ded_rlc_size
(
  /* RRC Dedicated TFS Dynamic TF info */
  tdsrrc_DedicatedDynamicTF_Info *ie_ptr
)
{
  /* function local variables */
  /* ------------------------ */

  /* initialize the RLC size to 0 */
  uint32 rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;

  switch (ie_ptr->rlc_Size.t)
  {
    case T_tdsrrc_DedicatedDynamicTF_Info_rlc_Size_bitMode:
      switch (ie_ptr->rlc_Size.u.bitMode->t)
      {
        case T_tdsrrc_BitModeRLC_SizeInfo_sizeType1:
          rlc_size = ie_ptr->rlc_Size.u.bitMode->u.sizeType1;
          break;
        case T_tdsrrc_BitModeRLC_SizeInfo_sizeType2:
          rlc_size = (ie_ptr->rlc_Size.u.bitMode->u.sizeType2->part1 * 8) + 128;
          if (ie_ptr->rlc_Size.u.bitMode->u.sizeType2->m.part2Present)
          {
            rlc_size += ie_ptr->rlc_Size.u.bitMode->u.sizeType2->part2;
          }
          break;
        case T_tdsrrc_BitModeRLC_SizeInfo_sizeType3:
          rlc_size = (ie_ptr->rlc_Size.u.bitMode->u.sizeType3->part1 * 16) + 256;
          if (ie_ptr->rlc_Size.u.bitMode->u.sizeType3->m.part2Present)
          {
            rlc_size += ie_ptr->rlc_Size.u.bitMode->u.sizeType3->part2;
          }
          break;
        case T_tdsrrc_BitModeRLC_SizeInfo_sizeType4:
          rlc_size = (ie_ptr->rlc_Size.u.bitMode->u.sizeType4->part1 * 64) + 1024;
          if (ie_ptr->rlc_Size.u.bitMode->u.sizeType4->m.part2Present)
          {
            rlc_size += ie_ptr->rlc_Size.u.bitMode->u.sizeType4->part2;
          }
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC TF type in BitMode %d",ie_ptr->rlc_Size.u.bitMode->t);
          break;
      }
      break;

    case T_tdsrrc_DedicatedDynamicTF_Info_rlc_Size_octetModeType1:
      switch (ie_ptr->rlc_Size.u.octetModeType1->t)
      {
        case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType1:
          rlc_size = (ie_ptr->rlc_Size.u.octetModeType1->u.sizeType1 * 8) + 16;
          break;
        case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType2:
          rlc_size = (ie_ptr->rlc_Size.u.octetModeType1->u.sizeType2->part1 * 32) + 272;
          if (ie_ptr->rlc_Size.u.octetModeType1->u.sizeType2->m.part2Present)
          {
            rlc_size += (ie_ptr->rlc_Size.u.octetModeType1->u.sizeType2->part2 * 8);
          }
          break;
        case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType3:
          rlc_size = (ie_ptr->rlc_Size.u.octetModeType1->u.sizeType3->part1 * 64) + 1040;
          if (ie_ptr->rlc_Size.u.octetModeType1->u.sizeType3->m.part2Present)
          {
            rlc_size += (ie_ptr->rlc_Size.u.octetModeType1->u.sizeType3->part2 * 8);
          }
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC TF type in OctetMode %d",ie_ptr->rlc_Size.u.octetModeType1->t);
          break;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC RLC size mode %d",ie_ptr->rlc_Size.t);
      break;
  }

  return rlc_size;
} /* END tdsrrcllc_get_tfs_ded_rlc_size() */

/*====================================================================
FUNCTION: tdsrrcllc_get_tfs_com_rlc_size()

DESCRIPTION:
  This function gets the RLC size for the common channel. The RLC size
  returned by this function is always equal to the TB size. It is a
  utility function for getting the TFS info parts.
  
  See the 25.331 'Message and Information Element abstract syntax
  (with ASN.1)' PDU definition for TrCh IEs 2000-12 V3.5.0 Page 492-498
  for getting the RLC size for Bitmode or Octet mode parts.
    
DEPENDENCIES:
  None

RETURN VALUE:
  16 bit RLC size (This is always the TB size)
  0xFFFF in case of error

SIDE EFFECTS:
  None.
====================================================================*/
uint32 tdsrrcllc_get_tfs_com_rlc_size
(
  /* RRC Dedicated TFS Dynamic TF info */
  tdsrrc_CommonDynamicTF_Info *ie_ptr
)
{
  /* function local variables */
  /* ------------------------ */
  
  uint32 rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;
  /*tdsrrc_OctetModeRLC_SizeInfoType2 *rlc_size_info;

  rlc_size_info = &(ie_ptr->rlc_Size.u.fdd->octetModeRLC_SizeInfoType2);*/
  
  /* get RLC size baed on the FDD or TDD mode */
  switch (ie_ptr->rlc_Size.t)
  {
    case T_tdsrrc_CommonDynamicTF_Info_rlc_Size_fdd:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL TFS cfg: Com RLCsize in FDD mode");
      break;

    case T_tdsrrc_CommonDynamicTF_Info_rlc_Size_tdd:
      /* Get the RLC size in TDD mode */
      switch (ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.t)
      {
        case T_tdsrrc_CommonDynamicTF_Info_commonTDD_Choice_bitModeRLC_SizeInfo:
          switch (ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->t)
          {
            case T_tdsrrc_BitModeRLC_SizeInfo_sizeType1:
              rlc_size = ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType1;
              break;
            case T_tdsrrc_BitModeRLC_SizeInfo_sizeType2:
              rlc_size = (ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType2->part1 * 8) + 128;
              if (ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType2->m.part2Present)
              {
                rlc_size = rlc_size + ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType2->part2;
              }
              break;
            case T_tdsrrc_BitModeRLC_SizeInfo_sizeType3:
              rlc_size = (ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType3->part1 * 16) + 256;
              if(ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType3->m.part2Present)
              {
                 rlc_size = rlc_size + ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType3->part2;
              }
              break;
            case T_tdsrrc_BitModeRLC_SizeInfo_sizeType4:
              rlc_size = (ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType4->part1 * 64) + 1024;
              if(ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType4->m.part2Present)
              {
                 rlc_size = rlc_size + ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.bitModeRLC_SizeInfo->u.sizeType4->part2;
              }
              break;
            default:
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL TFS cfg: Unknown TF size type");
              break;
          } /* End switch-case, size types bitModeRLC for RLC size in TDD mode  */
          break;
        case T_tdsrrc_CommonDynamicTF_Info_commonTDD_Choice_octetModeRLC_SizeInfoType1:
          switch (ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.octetModeRLC_SizeInfoType1->t)
          {
            case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType1:
              rlc_size = ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.octetModeRLC_SizeInfoType1->u.sizeType1 * 8 + 16;
              break;
            case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType2:
              rlc_size = ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.octetModeRLC_SizeInfoType1->u.sizeType2->part1 * 32 + 272;
              if(ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.octetModeRLC_SizeInfoType1->u.sizeType2->m.part2Present == 1)
              {
                 rlc_size = rlc_size + (ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.octetModeRLC_SizeInfoType1->u.sizeType2->part2 * 8);
              }
              break;
            case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType3:
              rlc_size = ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.octetModeRLC_SizeInfoType1->u.sizeType3->part1 * 64 + 1040;
              if(ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.octetModeRLC_SizeInfoType1->u.sizeType3->m.part2Present == 1)
              {
                 rlc_size = rlc_size + (ie_ptr->rlc_Size.u.tdd->commonTDD_Choice.u.octetModeRLC_SizeInfoType1->u.sizeType3->part2 * 8);
              }
              break;
            default:
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL TFS cfg: Unknown TF size type");
              break;
          } /* End switch-case, size types OctetModeRLC for RLC size in TDD mode  */
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL TFS cfg: Unknown TF size type");
          break;
      } /* End switch-case, size types for RLC size in TDD mode  */
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL TFS cfg: Unknown RLCsize type");
      break;

  } /* End switch-case, RLC size mode TDD/FDD */

  return rlc_size;
} /* END tdsrrcllc_get_tfs_com_rlc_size() */

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
)
{
  /* function local variables */
  /* ------------------------ */
  int i = 0;
  /* UL power offset information */
  tdsl1_ul_gain_factor_parms_struct_type *ul_power_offset_info;
  
  /* Gain factor structure pointer for signalled mode */
  tdsrrc_SignalledGainFactors *sig_gain_fact;

  /* Reference TFC Id for signalled mode */
  tdsrrc_ReferenceTFC_ID sig_mode_ref_tfc_id;

  uint8    signaled_num = 0;
  uint8    computed_num = 0;
  uint8    none_num = 0;
  boolean  signaled_refID = FALSE;
  uint8     ref_tfc_id_num = 0;
  uint8    ref_tfc_id[TDSL1_UL_TFC_MAX];
  uint8    signalledTFC_ID_set[4];  /*ReferenceTFC-ID ::= INTEGER (0 . . 3 ) */

  /* Get the pointer to the UL gain factor information structures/arrays */
  ul_power_offset_info = &gain_factor_parms[tfc_id];

  if (0 == power_offset_present)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No PowerOffsetInfo present,set gainfactor according to TC5-WG9-2010-204Q-MTK-V3");
    ul_power_offset_info->algo = TDSL1_UL_GF_NONE; 
    ul_power_offset_info->BetaD = DEFAULT_BATAD_NO_PWROFFSETINFO_EXIST;
  }
  else
  {
    /* Get the gain factor information based on the mode */
    switch (ie_ptr->gainFactorInformation.t)
    {
      case T_tdsrrc_GainFactorInformation_signalledGainFactors:
        ul_power_offset_info->algo = TDSL1_UL_GF_SIGNALLED;

        sig_gain_fact =
          (ie_ptr->gainFactorInformation.u.signalledGainFactors);

        if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(sig_gain_fact,referenceTFC_ID))
        {
          sig_mode_ref_tfc_id =
              ie_ptr->gainFactorInformation.u.signalledGainFactors->referenceTFC_ID;

          if (sig_mode_ref_tfc_id > (TDSL1_UL_REF_TFC_MAX - 1))
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC pwr ofst: Sig mode ref tfc id %d",sig_mode_ref_tfc_id);
            return FAILURE;
          }
          else
          {
            ul_power_offset_info->ref_tfc_id = (uint8)sig_mode_ref_tfc_id; 
            ul_power_offset_info->ref_tfc_id_valid = TRUE;
          }
        }
        else
        {
          ul_power_offset_info->ref_tfc_id_valid = FALSE;
        }

        ul_power_offset_info->BetaD = (uint8) sig_gain_fact->gainFactorBetaD;
        switch (sig_gain_fact->modeSpecificInfo.t)
        {
          case T_tdsrrc_SignalledGainFactors_modeSpecificInfo_fdd:
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC Pwr ofst:Gain fact BetaC in FDD mode");
            return FAILURE;
            break;
          case T_tdsrrc_SignalledGainFactors_modeSpecificInfo_tdd:
            /* TDD mode is no darta */
            break;
          default:
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC Pwr ofst:Invalid gain fact mode %d",sig_gain_fact->modeSpecificInfo.t);
            return FAILURE;
        }

        break;
      case T_tdsrrc_GainFactorInformation_computedGainFactors:
        ul_power_offset_info->algo = TDSL1_UL_GF_CALCULATED;
        ul_power_offset_info->ref_tfc_id = (uint8)
          ie_ptr->gainFactorInformation.u.computedGainFactors;
        break;
      default:
        break;
    }
  }
  /* all tfc have saved, begin check GainFactors */
  if (TRUE == last_tfc_flg)
  {
    /*init signalledTFC_ID_set*/
    for(i = 0; i < 4; i++) 
    {
        signalledTFC_ID_set[i] = 0;
    }
    /* find PowerOffsetInformation number. */
    for (i = 0; i <= tfc_id; i++)
    {
      if (gain_factor_parms[i].algo == TDSL1_UL_GF_SIGNALLED)
      {
        signaled_num++;
        if (gain_factor_parms[i].ref_tfc_id_valid == TRUE)
        {
          signaled_refID = TRUE;
          if ((ref_tfc_id_num >= TDSL1_UL_TFC_MAX) || (gain_factor_parms[i].ref_tfc_id > 3))
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"ref_tfc_id_num or ref_tfc_id over MAX");
            return FAILURE;
          }
          /* ref_tfc_id[0] is used only when signaled_num == 1 */
          ref_tfc_id[ref_tfc_id_num++] = gain_factor_parms[i].ref_tfc_id;
          signalledTFC_ID_set[gain_factor_parms[i].ref_tfc_id]++;
        }
      }
      else if (gain_factor_parms[i].algo == TDSL1_UL_GF_CALCULATED)
      {
        computed_num++;
      }
      else if (gain_factor_parms[i].algo == TDSL1_UL_GF_NONE)
      {
        none_num++;
      }
    }

    /* If there is no computed TFC, we allow signaled TFC share Ref ID. Otherwise, we treat this case as INVALID */

    /* The check has been combined into checking the computed referenceTFC-ID match the signaled referenceTFC-ID set */
    /* In that case, we allow non-referred but signalled ID share one Ref ID. */
    /* If more strict check is needed, use the following code. Otherwise, remove following code after review.*/

/*
    if (computed_num > 0)                                                                                         
    {                                                                                                             
        for (i = 0; i < 4; i++)                                                                                   
        {                                                                                                         
          if(signalledTFC_ID_set[i]>1)                                                                            
          {                                                                                                       
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"there is computed TFC,signaled TFC share Ref ID. This is INVALID case");                                                                                   
            return FAILURE;                                                                                       
          }                                                                                                       
        }                                                                                                         
     }                                                                                                            
                                                                                                                  
*/

    /* check All of the TFC have PowerOffsetInformation (GainFactorInformation) set to Computed */
    if ((signaled_num == 0)&&(computed_num > 0))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"All of the TFC have PowerOffsetInformation (GainFactorInformation) set to Computed. This is INVALID case");
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"We will do fault tolerance for all computed case");
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Gain Factor 1 will be assgined to each TFC");

      for (i = 0; i <= tfc_id; i++)
      {
          gain_factor_parms[i].algo = TDSL1_UL_GF_SIGNALLED;      
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"set all TFC as signalled with gainFactor = 1");
          gain_factor_parms[i].ref_tfc_id_valid = FALSE;
          gain_factor_parms[i].ref_tfc_id = 0;
          gain_factor_parms[i].BetaD = 1;
    
      }
      return SUCCESS;

    }
    /* The PowerOffsetInformation for one TFC is signaled and no value assigned to referenceTFC-ID (optional).
    All of the other TFC has PowerOffsetInformation (GainFactorInformation) set to Computed, 
    and the corresponding referenceTFC-ID is set to be a arbitrary value (0~3). 
    According to Ming’s email, this config is correct and should be support. 
    And all gainfactor for Computed TFC should refer to the signaled one.*/
    if ((signaled_num == 1) && (FALSE == signaled_refID))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"The PowerOffsetInformation for signaled TFC no value assigned to referenceTFC-ID");
      for (i = 0; i <= tfc_id; i++)
      {
        if (gain_factor_parms[i].algo == TDSL1_UL_GF_SIGNALLED)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"set signaled one a arbitrary value (0~3)");
          gain_factor_parms[i].ref_tfc_id_valid = TRUE;
          gain_factor_parms[i].ref_tfc_id = 0;
          signalledTFC_ID_set[0] = 1;     /*signalledTFC_ID_set[i!=0] may still carry original values, but it does not matter*/
        }
        else if(gain_factor_parms[i].algo == TDSL1_UL_GF_CALCULATED)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"all gainfactor for Computed TFC should refer to the signaled one");
          gain_factor_parms[i].ref_tfc_id = 0;
        }
      }
    }
    /* Only one TFC is signaled with the PowerOffsetInformation IE, and a Ref ID = A is assigned to the TFC.
    All of the computed TFCs are assigned with random Ref ID = A, B, or C… (a, b, or c has a range 0~3). 
    Should we disregard the value for A, B, or C… and assume that all gainfactor for Computed 
    TFC should refer to the only signaled TFC with Ref ID = A */
    if (ref_tfc_id_num == 1)
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"Only one TFC is signaled with the PowerOffsetInformation IE,and a Ref ID");
      for (i = 0; i <= tfc_id; i++)
      {
        if (gain_factor_parms[i].algo == TDSL1_UL_GF_CALCULATED)
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"all gainfactor for Computed TFC should refer to the only signaled TFC with Ref ID");
          gain_factor_parms[i].ref_tfc_id = ref_tfc_id[0];
        }
      }
    }
    /*No need the following loop if the ref_tfc_id_num == 1*/
    else
    {
      /* check the computed referenceTFC-ID match the signaled referenceTFC-ID set. */
      for (i = 0; i <= tfc_id; i++)
      {
        /* Mis-match. the computed referenceTFC-ID set cannot fit into the signaled referenceTFC-ID set. INVALID case. */
        if (gain_factor_parms[i].algo == TDSL1_UL_GF_CALCULATED)
        {
          /* look for signaled GF */
          if(signalledTFC_ID_set[gain_factor_parms[i].ref_tfc_id] != 1) 
          {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Mis-match. the computed referenceTFC-ID set cannot fit into the signaled referenceTFC-ID set. INVALID case.");
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Mis-matched reference ID is signalled %d times",signalledTFC_ID_set[gain_factor_parms[i].ref_tfc_id]);
              return FAILURE;
          }
  
        }
      }
     }
  }

  return SUCCESS;
} /* END tdsrrcllc_get_ul_gain_factor_info() */
  
/*================================================================================================
FUNCTION    :   tdsrrcllc_get_ul_semi_static_transport_format_info()

DESCRIPTION :   Processes the dynamic part of TFS IE of a transport channel
              If the IE contents are valid, saves the information in MAC Ordered Configuration 
              Database.

DEPENDENCIES:   None.

RETURN VALUE:   SUCCESS if the IE conenets are valid FAILURE otherwise

SIDE EFFECTS:   None.
===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_semi_static_transport_format_info
(
  tdsrrc_SemistaticTF_Information        *ie_ptr,
  tdsl1_ul_semi_static_parms_struct_type *oc_ssp_ptr
)
{
  /* get the semi-static Transport Format Information */
  switch(ie_ptr->channelCodingType.t)
  {
    case T_tdsrrc_ChannelCodingType_noCoding:
      oc_ssp_ptr->coding = TDSL1_UNCODED;
      break;

    case T_tdsrrc_ChannelCodingType_turbo:
      oc_ssp_ptr->coding = TDSL1_TURBO_CODING; 
      break;

    case T_tdsrrc_ChannelCodingType_convolutional:
      if(ie_ptr->channelCodingType.u.convolutional == tdsrrc_CodingRate_half)
      {
        oc_ssp_ptr->coding = TDSL1_CONV_1_BY_2_CODING;
      }
      else if(ie_ptr->channelCodingType.u.convolutional == tdsrrc_CodingRate_third)
      {
        oc_ssp_ptr->coding = TDSL1_CONV_1_BY_3_CODING;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Convolutional code rate %d is not valid",ie_ptr->channelCodingType.u.convolutional);
        return FAILURE;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Coding type %d rcv'd in TFS info",ie_ptr->channelCodingType.t);
      return FAILURE;
  } /* end switch channel coding type */
    
  /* Get the CRC information */
  switch(ie_ptr->crc_Size)
  {
    case tdsrrc_CRC_Size_crc0:
      oc_ssp_ptr->crc = TDSL1_CRC_LENGTH_0;      
      break;

    case tdsrrc_CRC_Size_crc8:
      oc_ssp_ptr->crc = TDSL1_CRC_LENGTH_8;
      break;

    case tdsrrc_CRC_Size_crc12:
      oc_ssp_ptr->crc = TDSL1_CRC_LENGTH_12;
      break;

    case tdsrrc_CRC_Size_crc16:
      oc_ssp_ptr->crc = TDSL1_CRC_LENGTH_16;
      break;

    case tdsrrc_CRC_Size_crc24:
      oc_ssp_ptr->crc = TDSL1_CRC_LENGTH_24;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CRC %d rcv'd in TFS info",ie_ptr->crc_Size);
      return FAILURE;
  } /* end switch on crc type */

  /* Get the rate matching attribute */
  if((ie_ptr->rateMatchingAttribute <= TDSRRCLLC_MAX_HI_RM) && 
     (ie_ptr->rateMatchingAttribute >= TDSRRCLLC_MIN_HI_RM))
  {
    oc_ssp_ptr->staticRM = (uint16) ie_ptr->rateMatchingAttribute;
  }
  else
  { 
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"UL Rate Matching Attribute %d is out of range",ie_ptr->rateMatchingAttribute);
    return FAILURE;
  }

  /* Good things if made it this far */
  return SUCCESS;
} /* end tdsrrcllc_get_ul_semi_static_transport_format_info() */

/*============================================================================
FUNCTION: tdsrrcllc_get_logical_channel_list()

DESCRIPTION:
  This function takes a Logical Channel List IE and stores its data
  in the global rlc_size_restriction_info[] array.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  The rlc_size_restriction_info[] array is updated.
=============================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_logical_channel_list
(
  tdsrrc_LogicalChannelList        *ie_ptr,
  tdsrrcllcoc_logical_channel_list_type *rec
)
{
  uecomdef_status_e_type rv = SUCCESS;
  tdsrrc_LogicalChannelList_explicitList *lc_list = NULL;

  switch(ie_ptr->t)
  {
    case T_tdsrrc_LogicalChannelList_allSizes:
        rec->restriction_type = TDSRLC_SIZES_ALL;
      break;

    case T_tdsrrc_LogicalChannelList_configured:
        rec->restriction_type = TDSRLC_SIZES_CONFIGURED;
      break;

    case T_tdsrrc_LogicalChannelList_explicitList:
        rec->restriction_type = TDSRLC_SIZES_EXPLICIT_LIST;
        rec->count = 0;
        lc_list = ie_ptr->u.explicitList;
        if(lc_list == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Logical channel list is empty.");
          rv = FAILURE;
          break;
        }
        while(lc_list->n > rec->count)
        {
          rec->rb_id[rec->count] = lc_list->elem[rec->count].rb_Identity;
          rec->count++;
        }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CHOICE for logical channel list IE: %d",ie_ptr->t);
      rv = FAILURE;
      break;
  }

  return rv;
} /* tdsrrcllc_get_logical_channel_list */


/*================================================================================================
FUNCTION:       tdsrrcllc_get_ul_ded_trch_dynamic_tfs_info()

DESCRIPTION:    Processes the dynamic part of TFS IE of dedicated transport channel
                If the IE contents are valid, saves the information in MAC Ordered Configuration 
                Database.

DEPENDENCIES:   None.

RETURN VALUE:   SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:   None.
===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_ded_trch_dynamic_tfs_info
(
  tdsordered_config_type                     *config_ptr,
  /* Pointer to the ASN.1 Dedicated Transport Channel TFS information */
  tdsrrc_DedicatedTransChTFS                 *ie_ptr,

  /* Pointer to Transport Channel TFS information pointer in OC */
  tdsmac_ul_tfs_type                         *oc_tfs_ptr,

  /* Pointer to the Semi static parms information pointer in OC */
  tdsl1_ul_semi_static_parms_struct_type *oc_ssp_ptr,

  /* TrCH identity, used for storing Logical Channel List information */
  tr_ch_id_type                         tdstrch_id

  ,boolean                               multiple_ded_log_chan
)
{
  /* Return value type  */
  uecomdef_status_e_type           rv = FAILURE;

  /* Pointer to TFS information, initialized in MACRO call */
  tdsrrc_DedicatedDynamicTF_InfoList  * tfs_ptr = NULL;

  /* Pointer to TF information in ordered config data */
  tdsmac_ul_tf_type                      *oc_tf_ptr = NULL;

  /* Transport Format index */
  /* This ensures the previous Transport Format set is trashed
   * and the new one is stored.
   */
  uint16 tf_idx = 0;

  /* Pointer to Number of Transport Blocks Linked List */
  tdsrrc_DedicatedDynamicTF_Info_numberOfTbSizeList * num_tb_ptr;
  uint32 idx =0,idy=0;

  /* Get the TFS pointer based on TTI type */
  rv = tdsrrcllc_get_ul_trch_dynamic_tfs_ie_ptr(ie_ptr, &tfs_ptr, oc_tfs_ptr);

  /*
  * If the TTI type was invalid  
  * then bail, since probably have a NULL pointer
  */
  if(rv == FAILURE)
  {
    return FAILURE;
  }

  /* Get Dynamic Transport Format information */
  while((tfs_ptr->n > idy) && (tf_idx < TDSUE_MAX_TF))
  {
    /* Get the Transport Block size pointer */
    num_tb_ptr = &tfs_ptr->elem[idy].numberOfTbSizeList;
    idx =0;
    /* Get the RLC size and Number of Transport blocks */
    while((num_tb_ptr->n  > idx) && (tf_idx < TDSUE_MAX_TF))
    {
      /* TF info pointer in ordered config data */
      oc_tf_ptr = &oc_tfs_ptr->tf_info[tf_idx];

      /* Save Number of TBlks info in OC data structure */
      if ((oc_tf_ptr->ntblks =
           tdsrrcllc_get_tfs_num_tb(&num_tb_ptr->elem[idx])) == 0xFF)
      {
        return FAILURE;
      }

      /* Save RLC Size */
      oc_tf_ptr->tb_size = (uint16) tdsrrcllc_get_tfs_ded_rlc_size(&tfs_ptr->elem[idy]);

      /* Set MAC headers for tb size, 'tb_size' is only present RLC size in UL interface */
      if((oc_tf_ptr->tb_size != 0) && (multiple_ded_log_chan))
      {
        oc_tfs_ptr->mac_header_size = TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
      }
      /* Get the next item in Number of TBs list & increment TF index */
      idx++;
      tf_idx++;

    } /* end while still have TFs to process */

    /* Pointer to the next item in Dynamic TF info */
    idy++;
  } /* end of TF loop */

  /* Save the semi-static parameters */
  if(tdsrrcllc_get_ul_semi_static_transport_format_info(&ie_ptr->semistaticTF_Information, oc_ssp_ptr)
    == FAILURE)
  {
    return FAILURE;
  }
  oc_ssp_ptr->trch_id = (uint8) tdstrch_id;

    /* Save Number of Transport Formats info, and indicate all is well */
  oc_tfs_ptr->ntf = tf_idx;
    return SUCCESS;

} /* end tdsrrcllc_get_ul_ded_trch_dynamic_tfs_info() */ /*lint !e715 */

/*================================================================================================
FUNCTION    :   tdsrrcllc_get_ul_common_trch_dynamic_tfs_info()

DESCRIPTION :   Processes the dynamic part of TFS IE of common transport channel
                If the IE contents are valid, saves the information in MAC Ordered Configuration 
                Database.

DEPENDENCIES:   None.

RETURN VALUE:   SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:   None.
===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_common_trch_dynamic_tfs_info
(
  tdsordered_config_type                 *config_ptr,
  /* Pointer to the ASN.1 Dedicated Transport Channel TFS information */
  tdsrrc_CommonTransChTFS_LCR            *ie_ptr,

  /* Pointer to Transport Channel TFS information pointer in OC */
  tdsmac_ul_tfs_type                     *oc_tfs_ptr,

  /* Pointer to the Semi static parms information pointer in OC */
  tdsl1_ul_semi_static_parms_struct_type *oc_ssp_ptr
)
{
  /* Return value type */
  uecomdef_status_e_type        rv = FAILURE;

  /* Pointer to TFS information */
  tdsrrc_CommonDynamicTF_InfoList   tfs_ptr;

  /* Pointer to TF information in ordered config data */
  tdsmac_ul_tf_type               *oc_tf_ptr = NULL;

  /* Transport Format index */
  uint16 tf_idx = 0;

  /* Pointer to Number of Transport Blocks Linked List */
  tdsrrc_CommonDynamicTF_Info_numberOfTbSizeList * num_tb_ptr;

  tdsrrcllcoc_logical_channel_list_type *rec;
  uint32 idx =0, idy=0;


  /* Get the TFS pointer based on TTI type */
  rv = tdsrrcllc_get_ul_trch_dynamic_tfs_ie_common(ie_ptr, &tfs_ptr, oc_tfs_ptr, oc_ssp_ptr);

  /*
  * If the TTI type was invalid   
  *  then bail, since probably have a NULL pointer
  */
  if(rv == FAILURE)
  {
    return FAILURE;
  }

  /* Get Dynamic Transport Format information */
  while((tfs_ptr.n > idy) && (tf_idx < TDSUE_MAX_TF))
  {
    /* Get the Transport Block size pointer */
    num_tb_ptr = &tfs_ptr.elem[idy].numberOfTbSizeList;
    idx=0;
    /* Get the RLC size and Number of Transport blocks */
    while((num_tb_ptr->n > idx) && (tf_idx < TDSUE_MAX_TF))
    {
      /* TF info pointer in ordered config data */
      oc_tf_ptr = &oc_tfs_ptr->tf_info[tf_idx];

      /* Save Number of TBlks info in OC data structure */
      if ((oc_tf_ptr->ntblks =
           tdsrrcllc_get_tfs_num_tb(&num_tb_ptr->elem[idx])) == 0xFF)
      {
        return FAILURE;
      }

      /* Save RLC Size */
      oc_tf_ptr->tb_size = (uint16) tdsrrcllc_get_tfs_com_rlc_size(&tfs_ptr.elem[idy]);

      if(config_ptr != NULL)
      {
        /* Save Logical Channel List */
        config_ptr->rach_rlc_size_restriction_info.mac_update_needed = TRUE;
        rec = &(config_ptr->rach_rlc_size_restriction_info.logical_channel_list[tf_idx]);
        if (tdsrrcllc_get_logical_channel_list(&tfs_ptr.elem[idy].logicalChannelList,
                                   rec) == FAILURE)
        {
          return FAILURE;
        }
      }


      /* Get the next item in Number of TBs list & increment TF index */
      idx++;;
      tf_idx++;

    } /* end while still have TFs to process */

    /* Pointer to the next item in Dynamic TF info */
    idy++;
  } /* end of TF loop */

  /* Save the semi-static parameters */
  if(tdsrrcllc_get_ul_semi_static_transport_format_info(&ie_ptr->semistaticTF_Information, oc_ssp_ptr) ==
    FAILURE)
  {
    rv = FAILURE;
  }

  /* Save Number of Transport Formats info, and indicate all is well */
  oc_tfs_ptr->ntf = tf_idx;
  return SUCCESS;

} /* end get_common_trch_dynamic_tfs_info() */

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
)
{
  uecomdef_status_e_type      rv = FAILURE;
  tr_ch_id_type tdstrch_id;
  tdsmac_ul_tfs_type     *oc_tfs_ptr;
  tdsl1_ul_semi_static_parms_struct_type *oc_ssp_ptr;

  /* Boolean used to indicate if multiple logical channels are mapped to a given TrCH. */
  boolean       multiple_ded_log_chan = FALSE;

  if(trch_idx >= TDSUE_MAX_TRCH)
  {
    return (rv);
  }

  /* TrCH identity, used for storing Logical Channel List information for TDSDCH */
  tdstrch_id = config_ptr->ul_dch_trch_info[trch_idx].trch_id;

  /* Pointer to the TFS info in Ordered Config */
  oc_tfs_ptr = &config_ptr->ul_tfcs.tfs_info[trch_idx];

  /* Pointer to the Semi Static parms in Ordered Config */
  oc_ssp_ptr = &config_ptr->rrc_ul_cctrch_params.semi_static_parms[trch_idx];
  /* Get the pointer for this Transport Channel's Semi Static Parameters */
  config_ptr->l1_ul_cctrch_parms.semi_static_parms_ptr[trch_idx] =
    &config_ptr->rrc_ul_cctrch_params.semi_static_parms[trch_idx];

  /* Determine if multiple dedicated logical channels are present on this TrCH id */
  if(tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt > 1)
  {
    multiple_ded_log_chan = TRUE;
  }

  /* Get TFS info based on channel type */
  if (ie_ptr != NULL)
  {
    switch (ie_ptr->t)
    {
    /* Dedicated Transport Channel */
    case T_tdsrrc_TransportFormatSet_dedicatedTransChTFS:
      rv = tdsrrcllc_get_ul_ded_trch_dynamic_tfs_info (config_ptr,
                                               ie_ptr->u.dedicatedTransChTFS,
                                               oc_tfs_ptr, oc_ssp_ptr, tdstrch_id
                                               ,multiple_ded_log_chan
                                               );
      break;

    /* Common Transport Channel */
    /* Common is only for tfs from SIB5/6. */
    case T_tdsrrc_TransportFormatSet_commonTransChTFS:
    /* Invalid Transport Channel */
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd invalid TFS");
      rv = FAILURE;
      break;
    }
  }
  else if (ie_lcr_ptr != NULL)
  {
    switch (ie_lcr_ptr->t)
    {
    /* Common Transport Channel */
    /* Common is only for tfs from SIB5/6. */
    case T_tdsrrc_TransportFormatSet_commonTransChTFS:
      rv = tdsrrcllc_get_ul_common_trch_dynamic_tfs_info (config_ptr,
                                                  (ie_lcr_ptr->u.commonTransChTFS_LCR),
                                                  oc_tfs_ptr, oc_ssp_ptr);
      break;

    /* Invalid Transport Channel */
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd invalid TFS");
      rv = FAILURE;
      break;
    }
  }

  return (rv);
} /* end tdsrrcllc_get_ul_transport_format_set() */

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
    tdsrrc_TransportFormatSet_LCR *ie_ptr, /* Transport channel index (0 to 7) */
    uint8  trch_idx
)
{
  uecomdef_status_e_type      rv = FAILURE;
  tdsmac_ul_tfs_type     *oc_tfs_ptr;
  tdsl1_ul_semi_static_parms_struct_type *oc_ssp_ptr;
  uint8 ix;

  if(trch_idx >= TDSUE_MAX_TRCH)
  {
    return (rv);
  }  
  /* Pointer to the TFS info in Ordered Config */
  oc_tfs_ptr = &prach_sel_info_ptr->ul_tfcs.tfs_info[trch_idx];

  /* Pointer to the Semi Static parms in Ordered Config */
  oc_ssp_ptr = &prach_sel_info_ptr->semi_static_parms[trch_idx];
  
  /* Get the pointer for this Transport Channel's Semi Static Parameters */
  prach_sel_info_ptr->l1_ul_cctrch_parms.semi_static_parms_ptr[trch_idx] =
    &prach_sel_info_ptr->semi_static_parms[trch_idx];

  /* Get TFS info based on channel type */
  switch (ie_ptr->t)
  {
    /* Common Transport Channel */
    case T_tdsrrc_TransportFormatSet_LCR_commonTransChTFS_LCR:
        rv = tdsrrcllc_get_ul_common_trch_dynamic_tfs_info (NULL,
                                                  ie_ptr->u.commonTransChTFS_LCR,
                                                  oc_tfs_ptr, oc_ssp_ptr);
        break;

    /* Invalid Transport Channel */
    default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd invalid TFS");
        rv = FAILURE;
        break;
  }
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"For trch_idx %d,TTI = %d",trch_idx,prach_sel_info_ptr->ul_tfcs.tfs_info[trch_idx].tti_info);
  
  for(ix=0; ix < prach_sel_info_ptr->ul_tfcs.tfs_info[trch_idx].ntf; ix++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"ntblks = %d,tb_size = %d",prach_sel_info_ptr->ul_tfcs.tfs_info[trch_idx].tf_info[ix].ntblks,prach_sel_info_ptr->ul_tfcs.tfs_info[trch_idx].tf_info[ix].tb_size);
  }
  return (rv);  
  
}

/*================================================================================================
FUNCTION    :   tdsrrcllc_get_ul_ctfc_info ()

DESCRIPTION :   Processes CTFC info based on CTFC Bit Size. This is for a complete
              reconfiguration or an an addition or the addition portion of the
              replacement.

DEPENDENCIES:   None.

RETURN VALUE:   SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:   None.
===============================================================================================*/
static uecomdef_status_e_type  tdsrrcllc_get_ul_ctfc_info
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_TFCS_ReconfAdd  *ie_ptr
)
{
  uecomdef_status_e_type  rv  =   SUCCESS;

  /* Pointer to the first item in the list */
  tdsrrc_TFCS_ReconfAdd_ctfc2Bit     *ie_ctfc2_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc4Bit     *ie_ctfc4_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc6Bit     *ie_ctfc6_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc8Bit     *ie_ctfc8_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc12Bit    *ie_ctfc12_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc16Bit    *ie_ctfc16_ptr;
  tdsrrc_TFCS_ReconfAdd_ctfc24Bit    *ie_ctfc24_ptr;

  uint32  n;

  /* Get the info based on CTFC Size type */
  switch (ie_ptr->ctfcSize.t)
  {
    /* 2 Bit CTFC */
    case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc2Bit:
      ie_ctfc2_ptr = ie_ptr->ctfcSize.u.ctfc2Bit;
      n = ie_ctfc2_ptr->n;
      break;

    /* 4 Bit CTFC */
    case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc4Bit:
      ie_ctfc4_ptr = ie_ptr->ctfcSize.u.ctfc4Bit;
      n = ie_ctfc4_ptr->n;
      break;

    /* 6 Bit CTFC */
      case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc6Bit:
      ie_ctfc6_ptr = ie_ptr->ctfcSize.u.ctfc6Bit;
      n = ie_ctfc6_ptr->n;
      break;

    /* 8 Bit CTFC */
    case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc8Bit:
      ie_ctfc8_ptr = ie_ptr->ctfcSize.u.ctfc8Bit;
      n = ie_ctfc8_ptr->n;
      break;

    /* 12 Bit CTFC */
    case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc12Bit:
      ie_ctfc12_ptr = ie_ptr->ctfcSize.u.ctfc12Bit;
      n = ie_ctfc12_ptr->n;
      break;

    /* 16 Bit CTFC */
    case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc16Bit:
      ie_ctfc16_ptr = ie_ptr->ctfcSize.u.ctfc16Bit;
      n = ie_ctfc16_ptr->n;
      break;

    /* 24 Bit CTFC */
    case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc24Bit:
      ie_ctfc24_ptr = ie_ptr->ctfcSize.u.ctfc24Bit;
      n = ie_ctfc24_ptr->n;
      break;

    /* Invalid CTFC size */
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd invalid CTFC bit size");
      rv = FAILURE;
      return (rv);
  }

  rv = tdsrrcllc_get_ul_ctfc_bit_info(config_ptr, ie_ptr, n);

  return (rv);

} /* tdsrrcllc_get_ul_ctfc_info() */

/*================================================================================================
FUNCTION  : tdsrrcllc_get_ul_add_tfcs_info() 

DESCRIPTION : Adds TFC info to the MAC Ordered Config Database

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS: None.
===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_add_tfcs_info
(
  tdsordered_config_type *config_ptr,
  tdsrrc_TFCS_ReconfAdd *ie_ptr
)
{
  /* CTFC size choice is same as the one used in complete configuration */
  return tdsrrcllc_get_ul_ctfc_info(config_ptr, ie_ptr);

} /* end tdsrrcllc_get_ul_tfcs_info() */

/*================================================================================================
FUNCTION  : tdsrrcllc_get_ul_remove_tfcs_info

DESCRIPTION : This function removes a TFCI from MAC Ordered Config Database

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS: None.
===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_remove_tfcs_info
(
  tdsordered_config_type *config_ptr,
  tdsrrc_TFCS_RemovalList * ie_ptr
)
{
  /* TFCI to be removed */
  int32   removed_tfci = 0;
  uint32  index_for_sorting;
  uint32  to_be_compared_index =0;
  uint32 idx =0;
  /* Go over each element of the Removal List */
  while(ie_ptr->n > idx)
  {
    /* 25.331-10.3.5.16 "In TDD 0 is a reserved value " */
    if(ie_ptr->elem[idx].tfci == 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TFCI = 0 is a invalid value for TDD remove TFCI");
      idx++;
      continue;
    }
    // for KW
    if ((ie_ptr->elem[idx].tfci - 1) >= 0)
    {
      /* Get TFCI value. RRC internal use index start value is 0, OTA start value is 1 */
      removed_tfci = ie_ptr->elem[idx].tfci - 1;
    }

    /* TFCI value is valid.
       The variable removed_tfci will always be a positive value (as the data type is unsigned int) */
    // 2012/09/10 KW
	if((removed_tfci >= 0) && (removed_tfci < config_ptr->ul_tfcs.ntfc) && (removed_tfci < TDSUE_MAX_TFC))
    {
      /* Mark the TFC as removed */
      config_ptr->ul_tfcs.tfc_info[removed_tfci].control_flag = TDSTFC_REMOVED;

      /* Save the removed TFCI in a table and increment the removed TFCI count */
      config_ptr->removed_tfci_list[config_ptr->num_removed_tfcis] = removed_tfci;
      config_ptr->num_removed_tfcis ++;
    }
    /* TFCI value is invalid */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid TFCI removal");
      return FAILURE;
    }

    /* Get pointer to the next item in the list */  \
    idx++;
  } /* end removed list loop */

  /* Now sort the Removed TFCI List here */
  for(index_for_sorting = 1; 
      ((index_for_sorting < config_ptr->num_removed_tfcis)&&
        (index_for_sorting < TDSUE_MAX_TFC)); 
      index_for_sorting ++)
  {
    /* If the TFCI for the index for sort is NOT lower than the TFCI
     * for the index one lower than the one being sorted, do NOT do anything.
     * This is because all indices further below have already been sorted.
     */
    if(config_ptr->removed_tfci_list[index_for_sorting] < 
        config_ptr->removed_tfci_list[index_for_sorting-1])
    {
      /* The TFCI for sorted index is lower than TFCI for index before
       * This is time to move all indices below till a space is found for 
       * data at current sorted index.
       */
      removed_tfci = config_ptr->removed_tfci_list[index_for_sorting];

      to_be_compared_index = index_for_sorting;

      while( (to_be_compared_index > 0) && 
             (removed_tfci < 
              config_ptr->removed_tfci_list[to_be_compared_index-1])
           )
      {
         config_ptr->removed_tfci_list[to_be_compared_index] =
           config_ptr->removed_tfci_list[to_be_compared_index-1];
        
         to_be_compared_index --;
      }
      config_ptr->removed_tfci_list[to_be_compared_index] = removed_tfci;
    }
  } /* for(...) */

  /* everything good if made it here */
  return SUCCESS;
} /* end tdsrrcllc_get_ul_remove_tfcs_info() */

/*================================================================================================
FUNCTION :  tdsrrcllc_get_ul_normal_tfci_info() 

DESCRIPTION :   This function Processes NORMAL TFCI signaling mode

DEPENDENCIES:   None.

RETURN VALUE:   SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:   None.
===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_normal_tfci_info
(
  tdsordered_config_type  *config_ptr,
  tdsrrc_ExplicitTFCS_Configuration * ie_ptr
)
{
  /* Get info based TFCS Representation type */
  switch (ie_ptr->t)
  {
    case T_tdsrrc_ExplicitTFCS_Configuration_complete:
      /* Initialize the Uplink TFI table first */
      tdsrrcllc_init_ul_tfi_table(config_ptr);

      /* Get and save the CTFC chosen */
      if (tdsrrcllc_get_ul_ctfc_info(config_ptr, ie_ptr->u.complete) == FAILURE)
      {
        return FAILURE;
      }
      break;

    case T_tdsrrc_ExplicitTFCS_Configuration_addition:
      if(tdsrrcllc_get_ul_add_tfcs_info(config_ptr, ie_ptr->u.addition) == FAILURE)
      { 
        return FAILURE;
      } 
      break;

    case T_tdsrrc_ExplicitTFCS_Configuration_removal:
      if(tdsrrcllc_get_ul_remove_tfcs_info(config_ptr, ie_ptr->u.removal) == FAILURE)
      {
        return FAILURE;
      }
      break;

    case T_tdsrrc_ExplicitTFCS_Configuration_replacement:
      /* First remove the selected TFC.... */
      if(tdsrrcllc_get_ul_remove_tfcs_info(config_ptr, &ie_ptr->u.replacement->tfcsRemoval)
          == FAILURE)
      {
        return FAILURE;
      }

      /* Then add the selected TFC.... */
      if(tdsrrcllc_get_ul_add_tfcs_info(config_ptr, &ie_ptr->u.replacement->tfcsAdd) == FAILURE)
      {
        return FAILURE;
      }
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcvd invalid TFCS representation");
      return FAILURE;

  } /* end explicit config choice() */

  /* everything good if got here */
  /* Indicate that both Uplink MAC and L1 need a reconfig */
  config_ptr->reconfig_needed.uplink_mac = TRUE;
  config_ptr->reconfig_needed.uplink_l1 = TRUE;

  return SUCCESS;

} /* end tdsrrcllc_get_ul_normal_tfci_info() */

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
)
{
  uecomdef_status_e_type  rv = SUCCESS;
  /* Set flag indicating that ul_tfcs has been rcvd*/
  tdsul_tfcs_rcvd = TRUE;
  /* Get TFCS based on TFCI Signaling type */
  switch (ie_ptr->t)
  {
      /* Normal TFCI Signaling - Explicit TFCS Info */
      case T_tdsrrc_TFCS_normalTFCI_Signalling:
          rv = tdsrrcllc_get_ul_normal_tfci_info(config_ptr, ie_ptr->u.normalTFCI_Signalling);
          break;
  
      /* Split TFCI Signaling */
      case T_tdsrrc_TFCS_dummy:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd Split TFCI info");
          rv = FAILURE;
          break;
  
      /* Invalid TFCI Signaling */
      default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcvd invalid TFCI signaling");
          rv = FAILURE;
          break;
    }

    return (rv);
} /* end tdsrrcllc_get_ul_tfcs_info() */

/*================================================================================================
FUNCTION: tdsrrcllc_get_ul_info_common_for_all_trch()

DESCRIPTION:
  This function takes a pointer to the Uplink Common Information for all
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS: None.
===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_info_common_for_all_trch
(
  tdsrrc_UL_CommonTransChInfo *ie_ptr,
  tdsrrc_state_e_type  next_rrc_state
)
{
  /* Now, this function is used by SIB16, preconfiguration, and handover_to_utran. */
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL Common info for all TrCH");

  /* Check if a new TFC Subset is required. */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_UL_CommonTransChInfo,tfc_Subset))
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"TFC subset IE is present: Using subset");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"Use existing UL TFC Subset");
  }

  /* Check if PRACH TFCS is presanet */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_UL_CommonTransChInfo,prach_TFCS))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd PRACH TFCS info");
    return FAILURE;
  }

  if(next_rrc_state == TDSRRC_STATE_CELL_DCH)
  {
    /* Check for the FDD specific info if it is included */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_UL_CommonTransChInfo,modeSpecificInfo))
    {
      switch(ie_ptr->modeSpecificInfo.t)
      {
        case T_tdsrrc_UL_CommonTransChInfo_modeSpecificInfo_tdd:
          if(ie_ptr->modeSpecificInfo.u.tdd->m.individualUL_CCTrCH_InfoListPresent)
          {
            if(ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.n > 1)
            {
              /* For first version, only support one cctrch, When OTA msg cctrch of CCTrCH List(more than one),
               * RRC consider this OTA msg is invalid.
               */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"More cctrch not supported for UL Common TRCH Info");
            return FAILURE;
          }
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Get New UL DCH TFCS");
            if(tdsrrcllc_get_ul_tfcs_info(tdsordered_config_ptr, &ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.elem[0].ul_TFCS) == FAILURE)
            {
              return FAILURE;
            }            
            /* Save TFCS ID from CCTRCH list for UE receive TFCC OTA message, RRC can judge the OTA message wheather legal.
                        Only support one cctrch current. */
            if ((ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.elem[0].ul_TFCS_Identity.tfcs_ID < 1) ||
                (ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.elem[0].ul_TFCS_Identity.tfcs_ID > 8))
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd invalid tfcs id");
              return FAILURE;
            }
            tdsordered_config_ptr->ul_tfcs_id[0] = ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.elem[0].ul_TFCS_Identity.tfcs_ID;
          }
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC: TFCS list is Changed ");
          tdsordered_config_ptr->mac_ul_parms.new_tfcs_list_present = TRUE;
          break;
        case T_tdsrrc_UL_CommonTransChInfo_modeSpecificInfo_fdd:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"FDD Mode not supported for UL Common TRCH Info");
          return FAILURE;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Mode %d rcv'd in UL Common TRCH Info IE",ie_ptr->modeSpecificInfo.t);
          return FAILURE;
      } /* End mode switch */
    } /* end Mode specific info if present */
  }  /* Next RRC state is Cell_DCH */

  return SUCCESS;
} /* end tdsrrcllc_get_ul_info_common_for_all_trch() */

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
)
{
  /* Check if PRACH TFCS is presanet */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_UL_CommonTransChInfo_r4,prach_TFCS))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd PRACH TFCS info");
    return FAILURE;
  }

  if(next_rrc_state == TDSRRC_STATE_CELL_DCH)
  {
    /* Check for the FDD specific info if it is included */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_UL_CommonTransChInfo_r4,modeSpecificInfo))
    {
      switch(ie_ptr->modeSpecificInfo.t)
      {
        case T_tdsrrc_UL_CommonTransChInfo_r4_modeSpecificInfo_tdd:
          if(ie_ptr->modeSpecificInfo.u.tdd->m.individualUL_CCTrCH_InfoListPresent)
          {
            if(ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.n > 1)
            {
              /* For first version, only support one cctrch, When OTA msg cctrch of CCTrCH List(more than one),
               * RRC consider this OTA msg is invalid.
               */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"More cctrch not supported for UL Common TRCH Info");
              return FAILURE;
            }
            TDSRRC_MSG0(MSG_LEGACY_MED,"Get New UL DCH TFCS");
            if(tdsrrcllc_get_ul_tfcs_info(tdsordered_config_ptr, &ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.elem[0].ul_TFCS) == FAILURE)
            {
              return FAILURE;
            }            
            /* Save TFCS ID from CCTRCH list for UE receive TFCC OTA message, RRC can judge the OTA message wheather legal.
                        Only support one cctrch current. */
            if ((ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.elem[0].ul_TFCS_Identity.tfcs_ID < 1) ||
                (ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.elem[0].ul_TFCS_Identity.tfcs_ID > 8))
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd invalid tfcs id");
              return FAILURE;
            }
            tdsordered_config_ptr->ul_tfcs_id[0] = ie_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.elem[0].ul_TFCS_Identity.tfcs_ID;
          }
          TDSRRC_MSG0(MSG_LEGACY_MED,"TFCC: TFCS list is Changed ");
          tdsordered_config_ptr->mac_ul_parms.new_tfcs_list_present = TRUE;
          break;
        case T_tdsrrc_UL_CommonTransChInfo_r4_modeSpecificInfo_fdd:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"FDD Mode not supported for UL Common TRCH Info");
          return FAILURE;
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Mode %d rcv'd in UL Common TRCH Info IE",ie_ptr->modeSpecificInfo.t);
          return FAILURE;
      } /* End mode switch */
    } /* end Mode specific info if present */
  }  /* Next RRC state is Cell_DCH */

  return SUCCESS;
} /* end tdsrrcllc_get_ul_info_common_for_all_trch_r5() */

/*================================================================================================
FUNCTION: tdsrrcllc_process_ul_deleted_trch_info_dch_usch()

DESCRIPTION:
  This function process deleted trCh information provided for dch_usch.

  In the TFS it resets all information for this TrCH.

REUSABILITY: R6/R5/R99.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  CELL_DCH deletions are the only item supported now.
===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_process_ul_deleted_trch_info_dch_usch
(
  tdsrrc_UL_TrCH_Type ul_TransportChannelType,
  tdsrrc_TransportChannelIdentity ul_TransportChannelIdentity
)
{
  /* TrCH Index */
  uint8 ix;

  if(tdsrrc_UL_TrCH_Type_dch == ul_TransportChannelType)
  {
    ix = tdsrrcllc_find_ul_dch_trch_idx(tdsordered_config_ptr,
                                     (tr_ch_id_type)ul_TransportChannelIdentity);
    if(ix >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"No index allocated for TrCH ID %d",ul_TransportChannelIdentity);
      return(SUCCESS);
    }
    tdsrrcllc_backup_rbmapping_info(ix,TDSUL_DCH_RB_MAPPING );
    /* Clear the RB Mapping info for this TrCH */
    tdsrrcllc_init_ul_dch_rb_mapping_info(tdsordered_config_ptr, ix);


    /* Update UL TrCH counts now */
    /* MAC count */
    tdsordered_config_ptr->ul_cctrch.dch_info.ndchs--;
    /* Indicate that the old index is now available */
    if(tdsrrcllc_deallocate_ul_dch_trch_idx(ix) == FAILURE)
    {
      return FAILURE;
    }

  }
  else
  {
    ix = tdsrrcllc_find_ul_rach_trch_idx();

    if(ix >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"No index allocated for TrCH ID %d",ul_TransportChannelIdentity);
      return(FAILURE);
    }
    tdsrrcllc_backup_rbmapping_info(ix,TDSRACH_RB_MAPPING);

    tdsrrcllc_init_rach_rb_mapping_info(tdsordered_config_ptr);
  }

  /* RRCEUL-query why is this cleared ir-respective of DCH or RACH trch 
     delete information */
  /* Clear the TFS for this TrCH */
  tdsrrcllc_init_ul_tfs_table(tdsordered_config_ptr, ix);

  tdsordered_config_ptr->ul_tfcs.nchan--;

  /* L1 count */
  tdsordered_config_ptr->rrc_ul_cctrch_params.num_trch--; 

  TDSRRC_MSG1(MSG_LEGACY_MED,"Delete UL TrCH %d",ul_TransportChannelIdentity);

  return(SUCCESS);
}

/*================================================================================================
FUNCTION: tdsrrcllc_get_ul_deleted_trch_info_list_r6()

DESCRIPTION:
  This function takes a pointer to the R6 Uplink Deleted 
  Transport Channel IE and Deletes if from Ordered Config.

REUSABILITY: R6

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  CELL_DCH deletions are the only item supported now.
===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_deleted_trch_info_list_r6
(
  tdsrrc_UL_TransportChannelIdentity_r6 * ie_ptr
)
{
  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_TransportChannelIdentity_r6_dch_usch))
  {
    /* set tdsul_tfcs_needed flag to true.  Later if common info is not present,
     * UE needs to reject the message 
     */
    tdsul_tfcs_needed = TRUE;
    return (tdsrrcllc_process_ul_deleted_trch_info_dch_usch(
      ie_ptr->u.dch_usch->ul_TransportChannelType,
      ie_ptr->u.dch_usch->ul_TransportChannelIdentity));
  }
#ifdef FEATURE_TDSCDMA_HSUPA
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_TransportChannelIdentity_r6_e_dch))
  {
    uint8 e_mac_d_flow_idx = 0;
    e_mac_d_flow_idx =
      tdsrrcllc_find_e_mac_d_flow_idx((uint8)ie_ptr->u.e_dch);

    if (e_mac_d_flow_idx >= UE_MAX_MAC_D_FLOW) 
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: NW deleting mac-d flow id %d that never existed",ie_ptr->u.e_dch);
      return(SUCCESS);
    }
    else
    {
      tdsrrcllc_backup_rbmapping_info(e_mac_d_flow_idx,TDSE_DCH_RB_MAPPING );
      tdsrrcllc_init_e_dch_rbmapping_info(tdsordered_config_ptr, e_mac_d_flow_idx);
      tdsrrcllc_update_mac_eul_action(TRUE);
    }
  }
    /* have this to be on safer side - in case later someone removes return 
     statement from one of the paths */
  return(SUCCESS);
#else
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: R6 UL Trch Deleted Info(e-mac-d-flow id) not yet supported");
    return(FAILURE);
  }
#endif /* FEATURE_TDSCDMA_HSUPA */

} /* end tdsrrcllc_get_ul_deleted_trch_info_list_r6() */

/*================================================================================================
FUNCTION: tdsrrcllc_get_ul_deleted_trch()

DESCRIPTION:
  This function takes a pointer to the Uplink Deleted 
  Transport Channel IE and Deletes if from Ordered Config.

  In the TFS it resets all information for this TrCH.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  CELL_DCH deletions are the only item supported now.
===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_deleted_trch
(
  tdsrrc_UL_TransportChannelIdentity * ie_ptr
)
{
  /* TrCH Index */
  uint8 ix;

  if(ie_ptr->ul_TransportChannelType == tdsrrc_UL_TrCH_Type_dch)
  {
    ix = tdsrrcllc_find_ul_dch_trch_idx(tdsordered_config_ptr,
                                     (tr_ch_id_type)ie_ptr->ul_TransportChannelIdentity);
    if(ix >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"No index allocated for TrCH ID %d",ie_ptr->ul_TransportChannelIdentity);
      return(SUCCESS);
    }
  }
  else
  {
    ix = tdsrrcllc_find_ul_rach_trch_idx();
    if(ix >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"No index allocated for TrCH ID %d",ie_ptr->ul_TransportChannelIdentity);
      return(FAILURE);
    }
  }
  /* Clear the RB Mapping info for this TrCH */
  if(ie_ptr->ul_TransportChannelType == tdsrrc_UL_TrCH_Type_dch)
  {

    tdsrrcllc_backup_rbmapping_info(ix,TDSUL_DCH_RB_MAPPING);

    tdsrrcllc_init_ul_dch_rb_mapping_info(tdsordered_config_ptr, ix);
  }
  else /* must be CELL_FACH */
  {
    tdsrrcllc_backup_rbmapping_info(ix,TDSRACH_RB_MAPPING);

    tdsrrcllc_init_rach_rb_mapping_info(tdsordered_config_ptr);
  }

  /* Clear the TFS for this TrCH */
  tdsrrcllc_init_ul_tfs_table(tdsordered_config_ptr, ix);

  /* Update UL TrCH counts now */
  /* MAC count */
  if(ie_ptr->ul_TransportChannelType == tdsrrc_UL_TrCH_Type_dch)
  {
    tdsordered_config_ptr->ul_cctrch.dch_info.ndchs--;
    /* Indicate that the old index is now available */
    if(tdsrrcllc_deallocate_ul_dch_trch_idx(ix) == FAILURE)
    {
      return FAILURE;
    }

  }
  tdsordered_config_ptr->ul_tfcs.nchan--;
  /* L1 count */
  tdsordered_config_ptr->rrc_ul_cctrch_params.num_trch--; 

  TDSRRC_MSG1(MSG_LEGACY_MED,"Delete UL TrCH %d",ie_ptr->ul_TransportChannelIdentity);
  return(SUCCESS);
} /* end tdsrrcllc_get_ul_deleted_trch() */

/*================================================================================================
FUNCTION: tdsrrcllc_get_ul_added_reconfig_trch()

DESCRIPTION:
  This function takes a pointer to the Uplink Added or Reconfigured 
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:
  CELL_DCH additions are the only item supported now.
===============================================================================================*/
uecomdef_status_e_type tdsrrcllc_get_ul_added_reconfig_trch
(
  tdsrrc_UL_AddReconfTransChInformation * ie_ptr
)
{

  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL Add/Reconf TrCH");


    if(tdsrrcllc_dch_usch_add_trch_info((tr_ch_id_type)ie_ptr->transportChannelIdentity,
                              &ie_ptr->transportFormatSet) == FAILURE)
      {
        return FAILURE;
      }

  return SUCCESS;
} /* end tdsrrcllc_get_ul_added_reconfig_trch() */

/*================================================================================================
FUNCTION    :   tdsrrcllc_compute_ul_tfis_from_ctfc 

DESCRIPTION :   For each TFCI, compute TFIs from CTFC

                First compute "P" value for each Transport Channel (P[i]) using the following :

                P[i] = (Number of TFs of "i"th TrCh) * P[i-1], where i=1,2,3,...I

                where P[0] = 1, and "I" is the number of Transport Channels

                Then for each CTFC, compute the TFs using the following algorithm :

                m = CTFC;
                i = I;          (where "I" is number of Transport Channels)
                while (i >0) {
                    TFI[
                }

DEPENDENCIES:   None.

RETURN VALUE:   None.

SIDE EFFECTS:   None.
===============================================================================================*/
void    tdsrrcllc_compute_ul_tfis_from_ctfc
(
    /* Pointer to TFCS Ordered Config Data */
    tdsmac_ul_tfcs_type    *tfcs_ptr
)
{
    /* Transport Format Combination Index */
    uint8   tfc_idx;

    /* Transport Channel Index */
    int8    trch_idx;

    /* P - values -- one for each transport channel */
    uint32  p_value[TDSUE_MAX_TRCH];

    /* M-Value */
    uint32  m_value;

    /* Pointer to TFC data in MAC Ordered Config Database */
    tdsmac_ul_tfc_type     *tfc_ptr;

    /* P[0] = 1 */
    p_value[0] = 1;

    /* Compute P- vales of all the transport channels */
    /* P[i] = P[i-1] * L[i-1], where "L" is Number of TFs of (i-1) Transport Channel */
    for (trch_idx = 1; trch_idx <tfcs_ptr->nchan && trch_idx < TDSUE_MAX_TRCH; trch_idx++){
        p_value[trch_idx] = (tfcs_ptr->tfs_info[trch_idx-1].ntf) * p_value[trch_idx-1];             
    }

    /* Derive Transport Formats from each CTFC */
    for (tfc_idx=0; tfc_idx < tfcs_ptr->ntfc && tfc_idx < TDSUE_MAX_TFC; tfc_idx++)
    {

      /* Pointer to Transport Format Combination in MAC ordered config database */
      tfc_ptr = &tfcs_ptr->tfc_info[tfc_idx];

      /* if the CTFC is not valid then skip the calculation. */
      if (tfc_ptr->control_flag == TDSTFC_NOT_ALLOWED)
        continue;
      /* Initialy "m" value = CTFC value */
      m_value = tfc_ptr->ctfc;

      /* Compute TFI for each Transport channel */
      for (trch_idx = (int8)(tfcs_ptr->nchan-1); trch_idx >= 0 && trch_idx < TDSUE_MAX_TRCH; trch_idx--)
      {

          /* TFI value */
          tfc_ptr->tfi[trch_idx] = (uint8)(m_value / p_value[trch_idx]);
          
          /* Update m-value to be used to compute TFI for next Transport Channel */
          m_value = (m_value % p_value[trch_idx]);
      }
    }
} /* end tdsrrcllc_compute_ul_tfis_from_ctfc() */

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_semistatic_tf_info()

DESCRIPTION:
  This is called from the transport format set info update function.
  This updates the Semistatic TF info for DL TrChs.
  
  References: 25.331 (2000-12) 10.3.5.11

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_semistatic_tf_info
(
  /* RRC TF semistatic TF info IE struct pointer */
  tdsrrc_SemistaticTF_Information *ie_ptr,
  /* DL TF semistatic param info struct pointer */
  tdsl1_dl_cctrchcfg_trch_info_struct_type  *dl_semistatic_tf_info
)
{
  /* Get the channel coding type */
  switch (ie_ptr->channelCodingType.t)
  {
    case T_tdsrrc_ChannelCodingType_noCoding:
      dl_semistatic_tf_info->coding_type = TDSL1_DL_UNCODED;
      break;
    case T_tdsrrc_ChannelCodingType_turbo:
      dl_semistatic_tf_info->coding_type = TDSL1_DL_TURBO_CODING;
      break;
    case T_tdsrrc_ChannelCodingType_convolutional:
      /* for convolutional coding get the DL coding type based on rate */
      if (ie_ptr->channelCodingType.u.convolutional == tdsrrc_CodingRate_half)
        dl_semistatic_tf_info->coding_type = TDSL1_DL_CONV_1_BY_2_CODING;
      else
        dl_semistatic_tf_info->coding_type = TDSL1_DL_CONV_1_BY_3_CODING;

      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC DL coding type %d",ie_ptr->channelCodingType.t);
      return FAILURE;
  } /* end switch-case, TrCh coding type */

  /* Get the rate matching attribute */
  dl_semistatic_tf_info->rate_match_attrib = 
    (uint16) (ie_ptr->rateMatchingAttribute);
    
  /* get the CRC size used for this TrCh */
  switch (ie_ptr->crc_Size)
  {
    case tdsrrc_CRC_Size_crc0:
      dl_semistatic_tf_info->crc_length = TDSL1_CRC_LENGTH_0;
      break;
    case tdsrrc_CRC_Size_crc8:
      dl_semistatic_tf_info->crc_length = TDSL1_CRC_LENGTH_8;
      break;
    case tdsrrc_CRC_Size_crc12:
      dl_semistatic_tf_info->crc_length = TDSL1_CRC_LENGTH_12;
      break;
    case tdsrrc_CRC_Size_crc16:
      dl_semistatic_tf_info->crc_length = TDSL1_CRC_LENGTH_16;
      break;
    case tdsrrc_CRC_Size_crc24:
      dl_semistatic_tf_info->crc_length = TDSL1_CRC_LENGTH_24;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC DL CRC value %d",ie_ptr->crc_Size);
      return FAILURE;
  } /* end switch-case, CRE size type */

  return SUCCESS;
} /* end tdsrrcllc_get_dl_semistatic_tf_info() */

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
)
{
  /* function local variables */
  /* ------------------------ */
  


  tdsrrc_TFCS_ReconfAdd *tfc_add_cfg_info;
  tdsrrc_TFCS_RemovalList * tfc_remove_info;

  /* next TFC pointer */
  tdsrrc_TFCS_ReconfAdd_ctfc2Bit  *tfc_ctfc2bit;
  tdsrrc_TFCS_ReconfAdd_ctfc4Bit  *tfc_ctfc4bit;
  tdsrrc_TFCS_ReconfAdd_ctfc6Bit  *tfc_ctfc6bit;
  tdsrrc_TFCS_ReconfAdd_ctfc8Bit  *tfc_ctfc8bit;
  tdsrrc_TFCS_ReconfAdd_ctfc12Bit *tfc_ctfc12bit;
  tdsrrc_TFCS_ReconfAdd_ctfc16Bit *tfc_ctfc16bit;
  tdsrrc_TFCS_ReconfAdd_ctfc24Bit *tfc_ctfc24bit;
  uint32 idx =0;
  uint32 n =0;
  uecomdef_status_e_type rv = FAILURE;
  /*** function code starts here ***/

  /* Get the CTFC information based on the type of CTFC update operation */
  switch (ie_ptr->t)
  {

    /* Complete reconfiguration */
    /* ------------------------ */
    case T_tdsrrc_ExplicitTFCS_Configuration_complete:
      /* First initialize all DL CTFCs in L1 and local structures */ 
          tdsrrcllc_init_dl_ctfcs(config_ptr);

      /* get the pointer to the complete reconfig of TFCS info.
        Initialize the CTFC entry (TFC) indedx to 0 */
      tfc_add_cfg_info = (ie_ptr->u.complete);
      

      /* get the all CTFC values (all TFC) based on the CTFC size */
      switch(ie_ptr->u.complete->ctfcSize.t)
      {
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc2Bit:
          tfc_ctfc2bit = tfc_add_cfg_info->ctfcSize.u.ctfc2Bit;
          if (tfc_ctfc2bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc2bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc2bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc4Bit:
          tfc_ctfc4bit = tfc_add_cfg_info->ctfcSize.u.ctfc4Bit;
          if (tfc_ctfc4bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc4bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc4bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc6Bit:
          tfc_ctfc6bit = tfc_add_cfg_info->ctfcSize.u.ctfc6Bit;
          if (tfc_ctfc6bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc6bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc6bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc8Bit:
          tfc_ctfc8bit = tfc_add_cfg_info->ctfcSize.u.ctfc8Bit;
          if (tfc_ctfc8bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc8bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc8bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc12Bit:
          tfc_ctfc12bit = tfc_add_cfg_info->ctfcSize.u.ctfc12Bit;
          if (tfc_ctfc12bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc12bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc12bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc16Bit:
          tfc_ctfc16bit = tfc_add_cfg_info->ctfcSize.u.ctfc16Bit;
          if (tfc_ctfc16bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc16bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc16bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc24Bit:
          tfc_ctfc24bit = tfc_add_cfg_info->ctfcSize.u.ctfc24Bit;
          if (tfc_ctfc24bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc24bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc24bit->n;
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC DL Explicit TFCS cfg CTFC size");
          return FAILURE;
      }
      rv = tdsrrcllc_get_dl_tfcs_explicit_cfg_recfg(tfc_add_cfg_info, dl_tfcs_info, n);
      if (rv != SUCCESS){
        return FAILURE;
      }
      break;

    /* CTFC addition */
    /* ------------- */
    case T_tdsrrc_ExplicitTFCS_Configuration_addition:
      /* get the pointer to the add TFCS info and initialize the
         CTFC entry (TFC) index to 0 */
      tfc_add_cfg_info = (ie_ptr->u.addition);
      
      
      /* find the least available TFCIs and update the TFC info at those
        indicies from the TFCS add info based on the CTFC value size */
      switch (ie_ptr->u.addition->ctfcSize.t)
      {
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc2Bit:
          tfc_ctfc2bit = tfc_add_cfg_info->ctfcSize.u.ctfc2Bit;
          if (tfc_ctfc2bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc2bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc2bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc4Bit:
          tfc_ctfc4bit = tfc_add_cfg_info->ctfcSize.u.ctfc4Bit;
          if (tfc_ctfc4bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc4bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc4bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc6Bit:
          tfc_ctfc6bit = tfc_add_cfg_info->ctfcSize.u.ctfc6Bit;
          if (tfc_ctfc6bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc6bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc6bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc8Bit:
          tfc_ctfc8bit = tfc_add_cfg_info->ctfcSize.u.ctfc8Bit;
          if (tfc_ctfc8bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc8bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc8bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc12Bit:
          tfc_ctfc12bit = tfc_add_cfg_info->ctfcSize.u.ctfc12Bit;
          if (tfc_ctfc12bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc12bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc12bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc16Bit:
          tfc_ctfc16bit = tfc_add_cfg_info->ctfcSize.u.ctfc16Bit;
          if (tfc_ctfc16bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc16bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc16bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc24Bit:
          tfc_ctfc24bit = tfc_add_cfg_info->ctfcSize.u.ctfc24Bit;
          if (tfc_ctfc24bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc24bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc24bit->n;
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC DL Explicit TFCS add CTFC size");
          return FAILURE;
      }
      rv = tdsrrcllc_get_dl_tfcs_explicit_cfg_add(tfc_add_cfg_info, dl_tfcs_info, n);
      if (rv != SUCCESS){
        return FAILURE;
      }
      break;

    /* CTFC or TFC is removed */
    /* ---------------------- */
    case T_tdsrrc_ExplicitTFCS_Configuration_removal:
      /* get the TFCI to be removed value and mark its state to be TDSINVALID */
      tfc_remove_info = ie_ptr->u.removal;
      while (tfc_remove_info->n > idx)
      {
        /* 25.331-10.3.5.16 "In TDD 0 is a reserved value " */
        if(tfc_remove_info->elem[idx].tfci == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TFCI = 0 is a invalid value for TDD remove TFCI");
          idx++;
          continue;
        }

        if (tfc_remove_info->elem[idx].tfci >= TDSUE_MAX_TFC)
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRC DL EXP TFCI RM idx %d > MAX idx %d",tfc_remove_info->elem[idx].tfci,TDSUE_MAX_TFC);
          return FAILURE;
        }

        /* RRC internal use index start value is 0, OTA start value is 1.
           So remove tfci 1 from OTA means remove index 0 internally */
        dl_tfcs_info->ctfc_entry_data[tfc_remove_info->elem[idx].tfci - 1].state = TDSINVALID;
        idx++;
      }
      break;

    /* CTFC replacement */
    /* ---------------- */
    case T_tdsrrc_ExplicitTFCS_Configuration_replacement:
      /* Remove the TFC and then ...... */
      tfc_remove_info = &ie_ptr->u.replacement->tfcsRemoval;
      while (tfc_remove_info->n > idx )
      {
        /* 25.331-10.3.5.16 "In TDD 0 is a reserved value " */
        if(tfc_remove_info->elem[idx].tfci == 0)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"TFCI = 0 is a invalid value for TDD remove TFCI");
          idx++;
          continue;
        }

        if (tfc_remove_info->elem[idx].tfci>= TDSUE_MAX_TFC)
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRC DL EXP TFCI RP idx %d > MAX idx %d",tfc_remove_info->elem[idx].tfci,TDSUE_MAX_TFC);
          return FAILURE;
        }

        /* RRC internal use index start value is 0, OTA start value is 1.
           So remove tfci 1 from OTA means remove index 0 internally */
        dl_tfcs_info->ctfc_entry_data[tfc_remove_info->elem[idx].tfci - 1].state = TDSINVALID;
        idx++;
      }

      /* ...... Add the new TFC */
      tfc_add_cfg_info = &(ie_ptr->u.replacement->tfcsAdd);

      idx =0;
      switch (ie_ptr->u.replacement->tfcsAdd.ctfcSize.t)
      {
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc2Bit:
          tfc_ctfc2bit = tfc_add_cfg_info->ctfcSize.u.ctfc2Bit;
          if (tfc_ctfc2bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc2bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc2bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc4Bit:
          tfc_ctfc4bit = tfc_add_cfg_info->ctfcSize.u.ctfc4Bit;
          if (tfc_ctfc4bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc4bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc4bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc6Bit:
          tfc_ctfc6bit = tfc_add_cfg_info->ctfcSize.u.ctfc6Bit;
          if (tfc_ctfc6bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc6bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc6bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc8Bit:
          tfc_ctfc8bit = tfc_add_cfg_info->ctfcSize.u.ctfc8Bit;
          if (tfc_ctfc8bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc8bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc8bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc12Bit:
          tfc_ctfc12bit = tfc_add_cfg_info->ctfcSize.u.ctfc12Bit;
          if (tfc_ctfc12bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc12bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc12bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc16Bit:
          tfc_ctfc16bit = tfc_add_cfg_info->ctfcSize.u.ctfc16Bit;
          if (tfc_ctfc16bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc16bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc16bit->n;
          break;
        case T_tdsrrc_TFCS_ReconfAdd_ctfcSize_ctfc24Bit:
          tfc_ctfc24bit = tfc_add_cfg_info->ctfcSize.u.ctfc24Bit;
          if (tfc_ctfc24bit == NULL){
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"tfc_ctfc24bit NULL"); 
            return FAILURE; 
          }
          n = tfc_ctfc24bit->n;
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC DL Explicit TFCS add CTFC size");
          return FAILURE;
      }
      rv = tdsrrcllc_get_dl_tfcs_explicit_cfg_add(tfc_add_cfg_info, dl_tfcs_info, n);
      if (rv != SUCCESS){
        return FAILURE;
      }
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid RRC Explicit TFCS cfg req type");
      return FAILURE;
  } /* end ctfc action switch */

  return SUCCESS;
} /* end tdsrrcllc_get_dl_tfcs_explicit_configuration() */

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
)
{
  /* Set flag indicating that dl_tfcs has been rcvd */
  tdsdl_tfcs_rcvd = TRUE;
  switch (ie_ptr->t)
  {
    case T_tdsrrc_TFCS_normalTFCI_Signalling:
      if(tdsrrcllc_get_dl_tfcs_explicit_configuration(config_ptr,
                                            (ie_ptr->u.normalTFCI_Signalling),
                                            dl_tfcs_info) == FAILURE)
      {
        return FAILURE;
      }
      break;

    case T_tdsrrc_TFCS_dummy:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL cfg,Split mode cfg not supported");
      return FAILURE;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC DL cfg,Unknown cfg type %d",ie_ptr->t);
      return FAILURE;
  }

  /* Indicate that Downlink L1 needs to be reconfigured */
  config_ptr->reconfig_needed.downlink_l1 = TRUE;
  /* Although no new parameters for MAC have been received, MAC 
   * still needs to be reconfigured so as to get the new CCTrCH
   * ID whenever Downlink L1 is reconfigured. This is due to the
   * interface requirements.
   */
  config_ptr->reconfig_needed.downlink_mac = TRUE;

  return SUCCESS;
} /* end tdsrrcllc_get_dl_tfcs_info() */

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_tfs_dedicated_trch()

DESCRIPTION:
  This function get the transport format set information for the
  dedicated transport channel. It gets both semistaic and dynamic
  transport formart information. This function needs a parameter of type
  tdsl1_dl_cctrchcfg_trch_info_struct_type that has array of pointers to TF information.
  Ther caller of this function is required to initialize the pointers
  of TF info structures to the valid ones up to the MAX TF per TrCh
  possible.
  
  The RLC size in the dedicated TrCh TFS info is RLC PDU. To get the TB
  size from RLC size (RLC PDU), the MAC header should be added to the
  RLC PDU size.
  TB Size = MAC Header + RLC PDU
  MAC header in case of Dedicated channel depends on the number of logical
  channel mapped to the TrCh. In case of multiple logical channel mapped
  to this TrCh (represented by this TFS info) the MAC header size is 4
  otherwise it is 0.
  
  In the Dynamic transport format info the information is in 2 loops.
  The first loop is for the RLC size (TB size info). For this TB size info
  there can be list of number of TBs for that size.
  Each pair (TB size, number of TB) constitute one TF. For example
  RLC Size = X
     Num TB size list [0, 1, 2]
  RLC Size = Y
     Num TB size list [3, 4]
  The TFI and list will be as following (each index has num_TB x TB size):
    TFI 0 : 0 x X
    TFI 1 : 1 x X
    TFI 2 : 2 x X
    TFI 3 : 3 x Y
    TFI 4 : 4 x Y
      
  References: 25.331 (2000-12) 10.3.5.23

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_tfs_dedicated_trch
(
  /* RRC TFS configuration IE structure pointer */
  tdsrrc_DedicatedTransChTFS     *ie_ptr,
  /* DL trch channel info pointer */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info,
  /* indicate if there are multiple logical channel in this TrCh */
  boolean                      multiple_logical_ch
)
{
  /* function local variables */
  /* ------------------------ */
  
  uint16 tb_size;
  uint32 tmp_tb_size;
  uint32 idx=0,idy=0;
  /* This ensures the previous TFS is trashed */
  uint8 cur_tf_idx = 0;

  tdsrrc_DedicatedDynamicTF_InfoList * tf_info_list;
  tdsrrc_DedicatedDynamicTF_Info_numberOfTbSizeList * num_tb_list;

  /* get the semistatic information */
  if(tdsrrcllc_get_dl_semistatic_tf_info(&(ie_ptr->semistaticTF_Information), dl_tfs_info)
     == FAILURE)
  {
    return FAILURE;
  }
  
  /* get the TTI type and pointer to the Dynamic transport information */
  switch (ie_ptr->tti.t)
  {
    case T_tdsrrc_DedicatedTransChTFS_tti_tti10:
      dl_tfs_info->tti_value = TDSL1_TTI_10MS;
      tf_info_list = ie_ptr->tti.u.tti10;
      break;

    case T_tdsrrc_DedicatedTransChTFS_tti_tti20:
      dl_tfs_info->tti_value = TDSL1_TTI_20MS;
      tf_info_list = ie_ptr->tti.u.tti20;
      break;

    case T_tdsrrc_DedicatedTransChTFS_tti_tti40:
      dl_tfs_info->tti_value = TDSL1_TTI_40MS;
      tf_info_list = ie_ptr->tti.u.tti40;
      break;

    case T_tdsrrc_DedicatedTransChTFS_tti_tti80:
      dl_tfs_info->tti_value = TDSL1_TTI_80MS;
      tf_info_list = ie_ptr->tti.u.tti80;
      break;

    case T_tdsrrc_DedicatedTransChTFS_tti_dynamic:
      tf_info_list = NULL;
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL TFS cfg: Dyn. tti not expected");
      return FAILURE;

    default:
      tf_info_list = NULL;
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL TFS cfg: Unknown TTI type");
      return FAILURE;

  } /* TTI Type switch */

  /* Get the Dynamic transport format information */
  /* -------------------------------------------- */
  while (tf_info_list->n > idx)
  {
    /* Get the RLC size for this TF info and adjust that to TB size */
    tmp_tb_size = tdsrrcllc_get_tfs_ded_rlc_size(&(tf_info_list->elem[idx]));
    if(tmp_tb_size == TDSRRCLLC_INVALID_RLC_SIZE)
    {
      return FAILURE;
    }
    tb_size = (uint16)tmp_tb_size;

    /* tb size for MAC headers */
    if((tb_size != 0) && (multiple_logical_ch))
    {
      tb_size += TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
    }
    idy =0;
    /* get all of the number of TB for this TB size */
    num_tb_list = &tf_info_list->elem[idx].numberOfTbSizeList;
    while (num_tb_list->n > idy)
    {
      /* Check if we have reached the max TRCH that we can accomodate */ 
      if (cur_tf_idx >= TDSUE_MAX_TF)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC DL TFS Ded > %d TFs",cur_tf_idx);
        return FAILURE;
      }

      if ((dl_tfs_info->tf_info_array[cur_tf_idx]->num_tb =
           tdsrrcllc_get_tfs_num_tb(&(num_tb_list->elem[idy]))) == 0xFF)
      {
        return FAILURE;
      }

      dl_tfs_info->tf_info_array[cur_tf_idx]->tb_size = tb_size;
      cur_tf_idx++;
      idy++;

    } /* End while, get complete number of TB list for this TB size */

    idx++;
  } /* End while, Get complete TB size list */

  /* update the number of TF obtained in the above Dynamic transport
     format information. */
  dl_tfs_info->num_tf = cur_tf_idx;

  return SUCCESS;
} /* end tdsrrcllc_get_dl_tfs_dedicated_trch() */

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_tfs_common_trch()

DESCRIPTION:
  This function get the transport format set information for the
  common transport channel. It gets both semistaic and dynamic
  transport formart information. This function needs a parameter of type
  tdsl1_dl_cctrchcfg_trch_info_struct_type that has array of pointers to TF information.
  Ther caller of this function is required to initialize the pointers
  of TF info structures to the valid ones up to the MAX TF per TrCh
  possible.
  
  The RLC size in the comomn TrCh TFS info is alwaysRLC PDU TB size.
  
  For information on how the TFI are set in the Dunamic TF info see
  description of function tdsrrcllc_get_dl_tfs_dedicated_trch.
  
  References: 25.331 (2000-12) 10.3.5.23

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_tfs_common_trch
(
  /* RRC TFS configuration IE structure pointer */
  tdsrrc_CommonTransChTFS        *ie_ptr,
  /* DL trch channel info pointer */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info
)
{
  /* function local variables */
  /* ------------------------ */
  
  uint16 tb_size;
  uint32 tmp_tb_size;

  /* This ensures the previous TFS is trashed */
  uint8 cur_tf_idx = 0;
  uint32 idx =0,idy=0;
  tdsrrc_CommonDynamicTF_InfoList * tf_info_list;
  tdsrrc_CommonDynamicTF_Info_numberOfTbSizeList *  num_tb_list;
  
  /* get the semistatic information */
  if(tdsrrcllc_get_dl_semistatic_tf_info(&(ie_ptr->semistaticTF_Information), dl_tfs_info)
     == FAILURE)
  {
    return FAILURE;
  }

  /* get the TTI type and pointer to the Dynamic transport information */
  switch (ie_ptr->tti.t)
  {
    case T_tdsrrc_CommonTransChTFS_tti_tti10:
      dl_tfs_info->tti_value = TDSL1_TTI_10MS;
      tf_info_list = ie_ptr->tti.u.tti10;
      break;
    case T_tdsrrc_CommonTransChTFS_tti_tti20:
      dl_tfs_info->tti_value = TDSL1_TTI_20MS;
      tf_info_list = ie_ptr->tti.u.tti20;
      break;
    case T_tdsrrc_CommonTransChTFS_tti_tti40:
      dl_tfs_info->tti_value = TDSL1_TTI_40MS;
      tf_info_list = ie_ptr->tti.u.tti40;
      break;
    case T_tdsrrc_CommonTransChTFS_tti_tti80:
      dl_tfs_info->tti_value = TDSL1_TTI_80MS;
      tf_info_list = ie_ptr->tti.u.tti80;
      break;
    case T_tdsrrc_CommonTransChTFS_tti_dynamic:
      tf_info_list = NULL;
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL TFS cfg: Dyn. tti not expected");
      return FAILURE;
    default:
      tf_info_list = NULL;
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC DL TFS cfg: Unknown TTI type");
      return FAILURE;
  } /* end TTI type switch */

  /* Get the Dynamic transport format information */
  /* -------------------------------------------- */
  while (tf_info_list->n > idx)
  {
    /* Get TB size. Here RLC size = TB size */
    tmp_tb_size = tdsrrcllc_get_tfs_com_rlc_size(&(tf_info_list->elem[idx]));
    if(tmp_tb_size == TDSRRCLLC_INVALID_RLC_SIZE)
    {
      return FAILURE;
    }
    tb_size = (uint16)tmp_tb_size;
    idy=0;
    /* get all of the number of TB for this TB size */
    num_tb_list = &tf_info_list->elem[idx].numberOfTbSizeList;
    while (num_tb_list->n > idy)
    {
      if (cur_tf_idx >= TDSUE_MAX_TF)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC DL TFS Common > %d TFs",cur_tf_idx);
        return (FAILURE);
      }

      if ((dl_tfs_info->tf_info_array[cur_tf_idx]->num_tb =
           tdsrrcllc_get_tfs_num_tb(&(num_tb_list->elem[idy]))) == 0xFF)
      {
        return FAILURE;
      }

      dl_tfs_info->tf_info_array[cur_tf_idx]->tb_size = tb_size;
      cur_tf_idx++;
      idy++;
    } /* End while, get complete number of TB list for this TB size */

    idx++;
  } /* End while, Get complete TB size list */

  /* update the number of TF obtained in the above Dynamic transport
     format information. */
  dl_tfs_info->num_tf = cur_tf_idx;

  return SUCCESS;
} /* end tdsrrcllc_get_dl_tfs_common_trch() */

/*====================================================================
FUNCTION: tdsrrcllc_get_dl_transport_format_set()

DESCRIPTION:
  This function get transport format set information for dedicated or
  common transport channel based on the choce of the transport channel
  type. It calls the appropriate function to get the information.
  
  References: 25.331 (2000-12) 10.3.5.23

DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_transport_format_set
(
  /* RRC TFS configuration IE structure pointer */
  tdsrrc_TransportFormatSet      *ie_ptr,
  /* DL trch channel info pointer */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_ded_tfs_info,
  /* indicate the multiple logical channel in this TrCh */
  boolean                      multiple_logical_ch,
  /* DL common trch channel info pointer */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_com_tfs_info
)
{
  switch (ie_ptr->t)
  {
    case T_tdsrrc_TransportFormatSet_dedicatedTransChTFS:
      if(dl_ded_tfs_info == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL pointer for DL Ded TFS Info");
        return(FAILURE);
      }
      if(tdsrrcllc_get_dl_tfs_dedicated_trch((ie_ptr->u.dedicatedTransChTFS),
                                   dl_ded_tfs_info, multiple_logical_ch)
         == FAILURE)
      {
        return FAILURE;
      }
      break;

    case T_tdsrrc_TransportFormatSet_commonTransChTFS:
      if(dl_com_tfs_info == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL pointer for DL Common TFS Info");
        return(FAILURE);
      }
      if(tdsrrcllc_get_dl_tfs_common_trch((ie_ptr->u.commonTransChTFS), dl_com_tfs_info)
         == FAILURE)
      {
        return FAILURE;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC DL TFS t %d",ie_ptr->t);
      return FAILURE;
  }

  return SUCCESS;
} /* end tdsrrcllc_get_dl_transport_format_set() */

/*=============================================================================
FUNCTION: tdsrrcllc_get_dl_info_common_for_all_trch()

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
uecomdef_status_e_type tdsrrcllc_get_dl_info_common_for_all_trch
(
  tdsrrc_DL_CommonTransChInfo *ie_ptr,
  tdsrrc_UL_CommonTransChInfo *ul_ptr,
  tdsrrc_state_e_type  next_rrc_state
)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Common info for all TrCH");

  /* Check if SCCPCH TFCS is present */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_CommonTransChInfo,sccpch_TFCS))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd SCCPCH TFCS info,Not applicable to CELL_DCH");
    return FAILURE;
  }

  if(next_rrc_state == TDSRRC_STATE_CELL_DCH)
  {
    /* Check for the FDD specific info if it is included */
    switch(ie_ptr->modeSpecificInfo.t)
    {
      /*
      * If FDD TFCS is signalled, it will either be included here, or else grab
      * it from the Uplink TFCS
      */
      case T_tdsrrc_DL_CommonTransChInfo_modeSpecificInfo_tdd:
        if (ie_ptr->modeSpecificInfo.u.tdd->m.individualDL_CCTrCH_InfoListPresent)
        {
          if (ie_ptr->modeSpecificInfo.u.tdd->individualDL_CCTrCH_InfoList.n > 1)
          {
            /* For first version, only support one cctrch, When OTA msg cctrch of CCTrCH List(more than one),
             * RRC consider this OTA msg is invalid.
             */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"More cctrch not supported for UL Common TRCH Info");
            return FAILURE;
          }
          switch(ie_ptr->modeSpecificInfo.u.tdd->individualDL_CCTrCH_InfoList.elem[0].tfcs_SignallingMode.t)
          {
            /* DL TFCS Included in this message */
            case T_tdsrrc_IndividualDL_CCTrCH_Info_tfcs_SignallingMode_explicit_config:
              TDSRRC_MSG0(MSG_LEGACY_LOW,"Get New DL TFCS");
        
              if(tdsrrcllc_get_dl_tfcs_info(tdsordered_config_ptr,
                  ie_ptr->modeSpecificInfo.u.tdd->
                    individualDL_CCTrCH_InfoList.elem[0].tfcs_SignallingMode.u.explicit_config,
                                  &tdsordered_config_ptr->dl_ctfc) == FAILURE)
              {
                return FAILURE;
              }
              break;
              
            /* Get the TFCS from the Uplink IE */
            case T_tdsrrc_IndividualDL_CCTrCH_Info_tfcs_SignallingMode_sameAsUL:
              /* Determine if Uplink Data was included */
              if((ul_ptr != NULL) && (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(
                    ul_ptr,tdsrrc_UL_CommonTransChInfo,modeSpecificInfo)))
              {
                switch(ul_ptr->modeSpecificInfo.t)
                {
                  /* Got a good UL TFCS */
                  case T_tdsrrc_UL_CommonTransChInfo_modeSpecificInfo_tdd:
                    TDSRRC_MSG0(MSG_LEGACY_LOW,"Use UL TFCS as New DL TFCS");
            
                    if(ul_ptr->modeSpecificInfo.u.tdd->m.individualUL_CCTrCH_InfoListPresent)
                    {
                    if(tdsrrcllc_get_dl_tfcs_info(tdsordered_config_ptr,
                                        &ul_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.
                                        elem[0].ul_TFCS,
                                        &tdsordered_config_ptr->dl_ctfc) == FAILURE)
                    {
                      return FAILURE;
                    }
                    }
                    else
                    {
                      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd no UL TFCS; Cannot get DL TFCS");
                      return FAILURE;
                    }
                    break;
                  /* Either FDD or invalid value */
                default:
                  TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd no UL TFCS; Cannot get DL TFCS");
                  return FAILURE;
                } /* End of uplink switch */
              } /* end of UL TFCS info */
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rcv'd no UL TFCS,DL same as UL");
                tdsrrcllc_copy_ul_tfcs_info_to_dl_tfcs();
              }
              break;
  
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid DL TFCS Signalled %d",ie_ptr->modeSpecificInfo.u.fdd->dl_Parameters.t);
              return FAILURE;
  
          } /* end TFCS switch */
        } /* end TFCS signalling mode present check */
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"Using existing DL TFCS");
        }
        break; /* end FDD TFCS chosen */
  
      case T_tdsrrc_DL_CommonTransChInfo_modeSpecificInfo_fdd:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"FDD Mode not supported for DL Common TRCH Info");
        return FAILURE;
  
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Mode %d rcv'd in DL Common TRCH Info IE",ie_ptr->modeSpecificInfo.t);
        return FAILURE;
    } /* End mode switch */
  }  /* next RRC state is Cell DCH */

  return SUCCESS;
} /* end tdsrrcllc_get_dl_info_common_for_all_trch() */


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
)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Common info for all TrCH");

  /* Check if SCCPCH TFCS is present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
      tdsrrc_DL_CommonTransChInfo_r4,sccpch_TFCS))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd SCCPCH TFCS info,Not applicable to CELL_DCH");
    return FAILURE;
  }

  if(next_rrc_state == TDSRRC_STATE_CELL_DCH)
  {
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_CommonTransChInfo_r4,
                   modeSpecificInfo))
    {
      /* Check for the FDD specific info if it is included */
      switch (ie_ptr->modeSpecificInfo.t)
      {
        /*
        * If FDD TFCS is signalled, it will either be included here, or else grab
        * it from the Uplink TFCS
        */
      case T_tdsrrc_DL_CommonTransChInfo_r4_modeSpecificInfo_tdd:
          if (ie_ptr->modeSpecificInfo.u.tdd->m.individualDL_CCTrCH_InfoListPresent)
          {
            if (ie_ptr->modeSpecificInfo.u.tdd->individualDL_CCTrCH_InfoList.n > 1)
            {
              /* For first version, only support one cctrch, When OTA msg cctrch of CCTrCH List(more than one),
               * RRC consider this OTA msg is invalid.
               */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"More cctrch not supported for UL Common TRCH Info");
              return FAILURE;
            }
            switch(ie_ptr->modeSpecificInfo.u.tdd->individualDL_CCTrCH_InfoList.elem[0].tfcs_SignallingMode.t)
            {
              /* DL TFCS Included in this message */
              case T_tdsrrc_IndividualDL_CCTrCH_Info_tfcs_SignallingMode_explicit_config:
                TDSRRC_MSG0(MSG_LEGACY_LOW,"Get New DL TFCS");

                if(tdsrrcllc_get_dl_tfcs_info(tdsordered_config_ptr,
                    ie_ptr->modeSpecificInfo.u.tdd->
                      individualDL_CCTrCH_InfoList.elem[0].tfcs_SignallingMode.u.explicit_config,
                    &tdsordered_config_ptr->dl_ctfc) == FAILURE)
                {
                  return FAILURE;
                }
                break;
                
              /* Get the TFCS from the Uplink IE */
              case T_tdsrrc_IndividualDL_CCTrCH_Info_tfcs_SignallingMode_sameAsUL:
                /* Determine if Uplink Data was included */
                if((ul_ptr != NULL) && (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(
                      ul_ptr,tdsrrc_UL_CommonTransChInfo_r4,modeSpecificInfo)))
                {
                  switch(ul_ptr->modeSpecificInfo.t)
                  {
                    /* Got a good UL TFCS */
                    case T_tdsrrc_UL_CommonTransChInfo_r4_modeSpecificInfo_tdd:
                      TDSRRC_MSG0(MSG_LEGACY_LOW,"Use UL TFCS as New DL TFCS");

                      if(ul_ptr->modeSpecificInfo.u.tdd->m.individualUL_CCTrCH_InfoListPresent)
                      {
                        if(tdsrrcllc_get_dl_tfcs_info(tdsordered_config_ptr,
                                          &ul_ptr->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.
                                          elem[0].ul_TFCS,
                                          &tdsordered_config_ptr->dl_ctfc) == FAILURE)
                        {
                          return FAILURE;
                        }
                      }
                      else
                      {
                        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd no UL TFCS; Cannot get DL TFCS");
                        return FAILURE;
                      }
                      break;
                    /* Either FDD or invalid value */
                    default:
                      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd no UL TFCS; Cannot get DL TFCS");
                      return FAILURE;
                  } /* End of uplink switch */
                } /* end of UL TFCS info */
                else
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rcv'd no UL TFCS,DL same as UL");
                  tdsrrcllc_copy_ul_tfcs_info_to_dl_tfcs();
                }
                break;

              default:
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid DL TFCS Signalled %d",ie_ptr->modeSpecificInfo.u.fdd->dl_Parameters.t);
                return FAILURE;

            } /* end TFCS switch */
          } /* end TFCS signalling mode present check */
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Using existing DL TFCS");
          }
          break; /* end FDD TFCS chosen */

        case T_tdsrrc_DL_CommonTransChInfo_r4_modeSpecificInfo_fdd:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"FDD Mode not supported for DL Common TRCH Info");
          return FAILURE;
    
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Mode %d rcv'd in DL Common TRCH Info IE",ie_ptr->modeSpecificInfo.t);
          return FAILURE;
      } /* End mode switch */
    }
  }  /* next RRC state is Cell DCH */

  return SUCCESS;
} /* end tdsrrcllc_get_dl_info_common_for_all_trch_r5() */

/*=============================================================================
FUNCTION: tdsrrcllc_get_dl_deleted_trch()

DESCRIPTION:
  This function takes a pointer to the Downlink Deleted 
  Transport Channel IE and Deletes if from Ordered Config.
  In the TFS it resets all information for this TrCH.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
===============================================================================*/
uecomdef_status_e_type tdsrrcllc_get_dl_deleted_trch
(
  tdsrrc_DL_TransportChannelIdentity * ie_ptr
)
{
  /* TrCH Index */
  uint8 ix;

  if(ie_ptr->dl_TransportChannelType == tdsrrc_DL_TrCH_Type_dch)
  {
    ix = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_TransportChannelIdentity);
    if(ix >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"No DL DCH index for TrCH ID %d",ie_ptr->dl_TransportChannelIdentity);
      return(SUCCESS);
    }

    tdsrrcllc_backup_rbmapping_info(ix,TDSDL_DCH_RB_MAPPING );
    /* Clear the RB Mapping info for this TrCH */
    tdsrrcllc_init_dl_dch_rb_mapping_info(tdsordered_config_ptr, ix);

    /* Clear the TFS for this TrCH */
    tdsrrcllc_init_dl_tfs_table(tdsordered_config_ptr, ix);

    /* Update MAC count */
    tdsordered_config_ptr->mac_dl_parms.num_dchs--;
    /* Initialize local data */
    tdsordered_config_ptr->dl_dch_trch_info[ix].dch_present = FALSE;
    tdsordered_config_ptr->dl_dch_trch_info[ix].trch_id     = 0;
  
    /* Indicate that the old index is now available */
    if(tdsrrcllc_deallocate_dl_dch_trch_idx(ix) == FAILURE)
    {
      return FAILURE;
    }
  }
  else  /* Must be FACH */
  {
    // ix = tdsrrcllc_find_dl_fach_trch_idx(ie_ptr->value.dl_TransportChannelIdentity); 
    ix = tdsrrcllc_find_dl_fach_trch_idx();
    if(ix >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"No FACH index for TrCH ID %d",ie_ptr->dl_TransportChannelIdentity);
      return(FAILURE);
    }
    tdsrrcllc_backup_rbmapping_info(ix,TDSFACH_RB_MAPPING );

    /* Clear the RB Mapping info for this TrCH */
    tdsrrcllc_init_fach_rb_mapping_info(tdsordered_config_ptr, ix);

    /* Clear the TFS for this TrCH */
    tdsrrcllc_init_dl_tfs_table(tdsordered_config_ptr, ix);

    /* Update MAC count */
    tdsordered_config_ptr->mac_dl_parms.num_fachs--;
  }

  /* Update L1 count */
  tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch--;
  
  TDSRRC_MSG1(MSG_LEGACY_MED,"Deleted DL TrCH %d",ie_ptr->dl_TransportChannelIdentity);

  return SUCCESS;
} /* end tdsrrcllc_get_dl_deleted_trch() */

/*============================================================================
FUNCTION: tdsrrcllc_delete_dl_dch_trch_index()

DESCRIPTION:
  This function takes a Transport channel index as a
  parameter and clears local, L1 and MAC data at the
  given index. It also decrements count of total number
  of transport channels in local, MAC and L1 data.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  CELL_DCH deletions are the only item supported now.
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_delete_dl_dch_trch_index
(
  uint8   trch_idx
)
{
  if(trch_idx < TDSUE_MAX_TRCH)
  {
    /* Clear the RB Mapping info (MAC) for this TrCH */
    tdsrrcllc_init_dl_dch_rb_mapping_info(tdsordered_config_ptr, trch_idx);

    /* Clear the TFS (L1) for this TrCH */
    tdsrrcllc_init_dl_tfs_table(tdsordered_config_ptr, trch_idx);

    /* Initialize local data */
    tdsordered_config_ptr->dl_dch_trch_info[trch_idx].dch_present = FALSE;
    tdsordered_config_ptr->dl_dch_trch_info[trch_idx].trch_id     = 0;

    /* Free the TrCH index for further use */
    if(tdsrrcllc_deallocate_dl_dch_trch_idx(trch_idx) == FAILURE)
    {
      return FAILURE;
    }

    /* One less TrCH - update local, MAC and L1 data structures */
    //tdsordered_config_ptr->dl_trch_cnt--;
    tdsordered_config_ptr->mac_dl_parms.num_dchs--;
    tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch--;

    TDSRRC_MSG1(MSG_LEGACY_MED,"Deleted DL TrCH Idx %d",trch_idx);
    return(SUCCESS);
  }
  else
  {
    return(FAILURE);
  }
} /* end tdsrrcllc_delete_dl_dch_trch_index() */

/*================================================================================================
FUNCTION: tdsrrcllc_get_dl_added_reconfig_trch()

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
uecomdef_status_e_type tdsrrcllc_get_dl_added_reconfig_trch
(
  tdsrrc_DL_AddReconfTransChInformation * ie_ptr,
  tdsrrc_UL_AddReconfTransChInfoList *  ul_ptr,
  boolean                         tm_signalling_allowed
)
{
  /* local transport channel variables */
  tr_ch_id_type trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
  tr_ch_id_type ul_trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Pointers to UL and DL TFS info for use when copying existing config */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info = NULL;
  tdsl1_ul_semi_static_parms_struct_type *ul_tfs_ss_info = NULL;
  tdsmac_ul_tfs_type *ul_tfs_dyn_info = NULL;
  uint32 idx=0;
  /* Loop variables for copying UL TFS info */
  uint8 tf_idx = 0;
  /*lint -e578 Declaration of symbol 'rrc_dl_tf_info' hides symbol 'rrc_dl_tf_info'*/
  tdsl1_dl_cctrchcfg_tf_info_struct_type *rrc_dl_tf_info = NULL;
  tdsmac_ul_tf_type *ul_tf_info = NULL;

  /* Boolean used to indicate if multiple logical channels are mapped to
     a given TrCH. */
  boolean       multiple_ded_log_chan = FALSE;

  /* local variable indicating whether or not an uplink TrCH ID was found */
  boolean       got_one = FALSE;

  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Add/Reconf TrCH");

  /*
  * Use the Transport Channel Index and next state to place the TFS in 
  * the correct slot in the MAC TFS array.
  */
  if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
  {
    /* Get the transport channel index for this transport channel ID.
     * Note: The index should already be there.
     */
    trch_idx = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_transportChannelIdentity);

    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,no idx found",ie_ptr->dl_transportChannelIdentity);
      return(FAILURE);
    }
    if(tdsrrcllc_restore_dl_dch_rb_mapping_info(trch_idx) == FAILURE)
    {
      /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,info not correct in Standalone RB-Mapping",ie_ptr->dl_transportChannelIdentity);
    }
    
    /* Determine if multiple dedicated logical channels are present on this TrCH id */
    if(tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt > 1)
    {
      multiple_ded_log_chan = TRUE;
    }

    /*
    * Get whether or not the TFS for a given TRCH is included or if it needs
    * to come from an uplink TrCH ID.*/
    switch(ie_ptr->tfs_SignallingMode.t)
    {
      /* TFS explicitly given for this Transport Channel */
      case T_tdsrrc_DL_AddReconfTransChInformation2_tfs_SignallingMode_explicit_config:
        TDSRRC_MSG1(MSG_LEGACY_LOW,"Get TFS for DL TrCH Id %d",ie_ptr->dl_transportChannelIdentity);

        /* get the Transport Format Set for this TrCH */
        if(tdsrrcllc_get_dl_transport_format_set(ie_ptr->tfs_SignallingMode.u.explicit_config,
                                       &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
                                       multiple_ded_log_chan, /* mulitple logical channels? */
                                       &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
                                      ) == FAILURE)
        {
          return FAILURE;
        }
        break; /* end explicit TFS signalling for DL TrCH */

      case T_tdsrrc_DL_AddReconfTransChInformation2_tfs_SignallingMode_sameAsULTrCH:
        /*
        * Make sure the UL Info was included, and if it was get the TFS for 
        * the TrCH ID that was included in this message.
        */ 
        if (ul_ptr == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"invalid:UL TFS absent when DL TFS choice 'same as UL'");
          return(FAILURE);
        }
        while(ul_ptr->n > idx)
        {
          if(ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
             ul_TransportChannelIdentity == ul_ptr->elem[idx].transportChannelIdentity)
          { 
            /* Got a match with Uplink TrCH ID */
            TDSRRC_MSG2(MSG_LEGACY_MED,"Get TFS for DL TrCH Id %d using UL TrCH ID %d",ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity,ul_ptr->elem[idx].transportChannelIdentity);
            if(tdsrrcllc_get_dl_transport_format_set(&ul_ptr->elem[idx].transportFormatSet,
                                           &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
                                           multiple_ded_log_chan, /* mulitple logical channels? */
                                           &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
                                          ) == FAILURE)
            {
              return FAILURE;
            }

            /* Indicate that found an UL TrCH ID, and break the while loop */
            got_one = TRUE;
            break;
          } /* end UL TrCH match */

          /* no match, keep looking */
          else 
          {
            idx++;
          }
        } /* end while */

        /* Make sure we found the UL TrCH, before continuing */
        if(!got_one)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No matching UL TFS in msg,look up from OC");
          /* Get the transport channel index for the UL transport channel ID.
           * Note: The index should already be there.
           */
          ul_trch_idx = tdsrrcllc_find_ul_dch_trch_idx(
              tdsordered_config_ptr,
              (tr_ch_id_type)ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
                ul_TransportChannelIdentity);
          if(ul_trch_idx >= TDSUE_MAX_TRCH)
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"No DL TFS for TrCH %d, to use UL TrCH %d for TFS",
                ie_ptr->dl_transportChannelIdentity,
                ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
                  ul_TransportChannelIdentity, 0);
            return(FAILURE);
          }

          /* Set up convenience pointers */
          dl_tfs_info = &tdsordered_config_ptr->dl_trch_info[trch_idx];

          ul_tfs_ss_info = &tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[ul_trch_idx];
          ul_tfs_dyn_info = &tdsordered_config_ptr->ul_tfcs.tfs_info[ul_trch_idx];

          /* Copy semistatic info from uplink */
          switch(ul_tfs_ss_info->coding)
          {
            case TDSL1_UNCODED:
                    dl_tfs_info->coding_type = TDSL1_DL_UNCODED;
                    break;
            case TDSL1_CONV_1_BY_2_CODING:
                    dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_2_CODING;
                    break;
            case TDSL1_CONV_1_BY_3_CODING:
                    dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_3_CODING;
                    break;
            case TDSL1_TURBO_CODING:
                    dl_tfs_info->coding_type = TDSL1_DL_TURBO_CODING;
                    break;
            default:break; 
          }
          dl_tfs_info->rate_match_attrib = ul_tfs_ss_info->staticRM;
          switch(ul_tfs_ss_info->crc)
          {
            case TDSL1_CRC_LENGTH_0:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_0;
              break;
            case TDSL1_CRC_LENGTH_8:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_8;
              break;
            case TDSL1_CRC_LENGTH_12:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_12;
              break;
            case TDSL1_CRC_LENGTH_16:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_16;
              break;
            case TDSL1_CRC_LENGTH_24:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_24;
              break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CRC length %d in UL",ul_tfs_ss_info->crc);
              return FAILURE;
          }

          /* Copy dynamic info from uplink */
          dl_tfs_info->tti_value = ul_tfs_dyn_info->tti_info;
          dl_tfs_info->num_tf = (uint8)ul_tfs_dyn_info->ntf;
          for(tf_idx = 0; tf_idx < dl_tfs_info->num_tf; tf_idx++)
          {
            rrc_dl_tf_info = dl_tfs_info->tf_info_array[tf_idx];
            ul_tf_info = &ul_tfs_dyn_info->tf_info[tf_idx];

            rrc_dl_tf_info->num_tb = (uint8)ul_tf_info->ntblks;
            rrc_dl_tf_info->tb_size = ul_tf_info->tb_size;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"TB Size: %d,multiple_ded_log_chan: %d",rrc_dl_tf_info->tb_size,multiple_ded_log_chan);
            
            /* If number of logical channel is > 1, inc tb_size by 4 to accomodate MAC-header */
            if ((rrc_dl_tf_info->tb_size > 0)&&(multiple_ded_log_chan == TRUE))
            {
              rrc_dl_tf_info->tb_size += TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"New TB Size %d",rrc_dl_tf_info->tb_size);
            }
          }
        }
        break; /* end use of previous signalled UL TrCH TFS for DL TrCH */ 

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"No DL TFCS was included for TrCH ID %d",ie_ptr->dl_transportChannelIdentity);
        return FAILURE;

    } /* end TFS selection switch */

    /*Find Rbs mapped to this DCH and fill them in rb_list_mapped_to_dch if their RLC
    PDU size has changed.*/ 
    tdsrrcllc_get_rb_for_rb_list_mapped_to_dch((tr_ch_id_type)ie_ptr->dl_transportChannelIdentity, 
              tdsordered_config_ptr->rrc_state_indicator, TDSDL);   
  } /* end CELL_DCH check */

  /* States not supported */
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_FACH not supported for TrCH");
     return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_PCH not supported for TrCH");
     return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"URA_PCH not supported for TrCH");
     return FAILURE;
  }
  else
  {
     ERR_FATAL("Unknown state not supported for TrCH",0,0,0);
  }

  /***************************************************************/
  /* Transport Channel Count has been updated in RB Mapping info */
  /***************************************************************/

  /* Check for the BLER */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation,
    dch_QualityTarget))
  {
    /* Save the BLER if found, and is in range */
    if((ie_ptr->dch_QualityTarget.bler_QualityValue >= TDSRRCLLC_MIN_BLER) &&
       (ie_ptr->dch_QualityTarget.bler_QualityValue <= TDSRRCLLC_MAX_BLER)
      )
    {
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val =
        (int8)ie_ptr->dch_QualityTarget.bler_QualityValue;
    }

    /* BLER is out of range */
    else
    {
      /* According to L1 I/F, must set an invalid value of -127 */
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL BLER %d is out of range",ie_ptr->dch_QualityTarget.bler_QualityValue);
      return FAILURE;
    }
  } /* end if BLER present */
  else
  {
    /* IE omitted. Set the target to -127 which would result 1% of BLER target.
     * This is good UE behaviour though the spec is ambiguous on what UE
     * behavior should be in this case.  
     */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DCH quality target not present. Setting to -127");
    tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
  }

  /* Now check to see if allowed and is time to set up TM signalling info */
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation,dummy)) &&
     (tm_signalling_allowed)
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TM Signalling on DCCH not currently supported");
    /* don't need to return a failure now */
  }
  else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation,dummy)) &&
          (!tm_signalling_allowed)
         ) 
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TM Signalling on DCCH not supported for this message");
    return FAILURE;  /* something is messed up in msg proccessing land */
  }
  else if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation,dummy)) &&
          (tm_signalling_allowed)
         ) 
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"TM Signalling on DCCH not required for this message config");
  }
  else if(!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation,dummy)) &&
          !(tm_signalling_allowed)
         ) 
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"No TM Signalling on DCCH");
  }
  else
  {
    ERR_FATAL("Something special happened(Bad)",0,0,0);
  }

  /* Indicate that DL L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;

  return SUCCESS;
  /*lint +e578 Declaration of symbol 'rrc_dl_tf_info' hides symbol 'rrc_dl_tf_info'*/
} /* end tdsrrcllc_get_dl_added_reconfig_trch() */


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
)
{
  /* local transport channel variables */
  tr_ch_id_type trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
  tr_ch_id_type ul_trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Pointers to UL and DL TFS info for use when copying existing config */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info = NULL;
  tdsl1_ul_semi_static_parms_struct_type *ul_tfs_ss_info = NULL;
  tdsmac_ul_tfs_type *ul_tfs_dyn_info = NULL;
  uint32 idx=0;
  /* Loop variables for copying UL TFS info */
  /*lint -e578 */
  uint8 tf_idx = 0;
  tdsl1_dl_cctrchcfg_tf_info_struct_type *rrc_dl_tf_info = NULL;
  tdsmac_ul_tf_type *ul_tf_info = NULL;
  /*lint +e578 */

  /* Boolean used to indicate if multiple logical channels are mapped to
     a given TrCH. */
  boolean       multiple_ded_log_chan = FALSE;

  /* local variable indicating whether or not an uplink TrCH ID was found */
  boolean       got_one = FALSE;

  /*
  * Use the Transport Channel Index and next state to place the TFS in 
  * the correct slot in the MAC TFS array.
  */
  if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
  {
    /* Get the transport channel index for this transport channel ID.
     * Note: The index should already be there.
     */
    trch_idx = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_transportChannelIdentity);
    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,no idx found",ie_ptr->dl_transportChannelIdentity);
      return(FAILURE);
    }
    if(tdsrrcllc_restore_dl_dch_rb_mapping_info(trch_idx) == FAILURE)
    {
      /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,not correct in Standalone RB-Mapping",ie_ptr->dl_transportChannelIdentity);
    }
    
    /* Determine if multiple dedicated logical channels are present on this TrCH id */
    if(tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt > 1)
    {
      multiple_ded_log_chan = TRUE;
    }

    /*
    * Get whether or not the TFS for a given TRCH is included or if it needs
    * to come from an uplink TrCH ID.
    */
    switch(ie_ptr->tfs_SignallingMode.t)
    {
      /* TFS explicitly given for this Transport Channel */
      case T_tdsrrc_DL_AddReconfTransChInformation2_tfs_SignallingMode_explicit_config:
        
        /* get the Transport Format Set for this TrCH */
        if(tdsrrcllc_get_dl_transport_format_set(ie_ptr->tfs_SignallingMode.u.explicit_config,
                                       &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
                                       multiple_ded_log_chan, /* mulitple logical channels? */
                                       &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
                                      ) == FAILURE)
        {
          return FAILURE;
        }
        break; /* end explicit TFS signalling for DL TrCH */

      case T_tdsrrc_DL_AddReconfTransChInformation2_tfs_SignallingMode_sameAsULTrCH:
        /*
        * Make sure the UL Info was included, and if it was get the TFS for 
        * the TrCH ID that was included in this message.
        */
        if (ul_ptr == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"invalid:UL TFS absent when DL TFS choice 'same as UL'");
          return(FAILURE);
        }
        while(ul_ptr->n > idx)
        {
          if(ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
             ul_TransportChannelIdentity == ul_ptr->elem[idx].transportChannelIdentity)
          { 
            /* Got a match with Uplink TrCH ID */
            TDSRRC_MSG2(MSG_LEGACY_MED,"Get TFS for DL TrCH Id %d using UL TrCH ID %d",ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity,ul_ptr->elem[idx].transportChannelIdentity);
            if(tdsrrcllc_get_dl_transport_format_set(&ul_ptr->elem[idx].transportFormatSet,
                                           &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
                                           multiple_ded_log_chan, /* mulitple logical channels? */
                                           &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
                                          ) == FAILURE)
            {
              return FAILURE;
            }

            /* Indicate that found an UL TrCH ID, and break the while loop */
            got_one = TRUE;
            break;
          } /* end UL TrCH match */

          /* no match, keep looking */
          else 
          {
            idx++;
          }
        } /* end while */

        /* Make sure we found the UL TrCH, before continuing */
        if(!got_one)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No matching UL TFS in msg,look up from OC");
          /* Get the transport channel index for the UL transport channel ID.
           * Note: The index should already be there.
           */
          ul_trch_idx = tdsrrcllc_find_ul_dch_trch_idx(
              tdsordered_config_ptr,
              (tr_ch_id_type)ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
                ul_TransportChannelIdentity);
          if(ul_trch_idx >= TDSUE_MAX_TRCH)
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"No DL TFS to use UL TrCH %d for TFS",
                ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
                  ul_TransportChannelIdentity, 0, 0);
            return(FAILURE);
          }
          /* Set up convenience pointers */
          dl_tfs_info = &tdsordered_config_ptr->dl_trch_info[trch_idx];
          ul_tfs_ss_info = &tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[ul_trch_idx];
          ul_tfs_dyn_info = &tdsordered_config_ptr->ul_tfcs.tfs_info[ul_trch_idx];
          /* Copy semistatic info from uplink */
          switch(ul_tfs_ss_info->coding)
          {
            case TDSL1_UNCODED:
                    dl_tfs_info->coding_type = TDSL1_DL_UNCODED;
                    break;
            case TDSL1_CONV_1_BY_2_CODING:
                    dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_2_CODING;
                    break;
            case TDSL1_CONV_1_BY_3_CODING:
                    dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_3_CODING;
                    break;
            case TDSL1_TURBO_CODING:
                    dl_tfs_info->coding_type = TDSL1_DL_TURBO_CODING;
                    break;
            default:break; 
          }
          dl_tfs_info->rate_match_attrib = ul_tfs_ss_info->staticRM;
          switch(ul_tfs_ss_info->crc)
          {
            case TDSL1_CRC_LENGTH_0:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_0;
              break;
            case TDSL1_CRC_LENGTH_8:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_8;
              break;
            case TDSL1_CRC_LENGTH_12:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_12;
              break;
            case TDSL1_CRC_LENGTH_16:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_16;
              break;
            case TDSL1_CRC_LENGTH_24:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_24;
              break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CRC length %d in UL",ul_tfs_ss_info->crc);
              return FAILURE;
          }
          /* Copy dynamic info from uplink */
          dl_tfs_info->tti_value = ul_tfs_dyn_info->tti_info;
          dl_tfs_info->num_tf = (uint8)ul_tfs_dyn_info->ntf;
          for(tf_idx = 0; tf_idx < dl_tfs_info->num_tf; tf_idx++)
          {
            rrc_dl_tf_info = dl_tfs_info->tf_info_array[tf_idx];
            ul_tf_info = &ul_tfs_dyn_info->tf_info[tf_idx];
            rrc_dl_tf_info->num_tb = (uint8)ul_tf_info->ntblks;
            rrc_dl_tf_info->tb_size = ul_tf_info->tb_size;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"TB Size: %d,multible_ded_log_chan: %d",rrc_dl_tf_info->tb_size,multiple_ded_log_chan);

            /* If number of logical channel is > 1, inc tb_size by 4 to accomodate MAC-header */
            if ((rrc_dl_tf_info->tb_size > 0)&&(multiple_ded_log_chan == TRUE))
            {
              rrc_dl_tf_info->tb_size += TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"New TB Size %d",rrc_dl_tf_info->tb_size);
            }
          }
        }
        break; /* end use of previous signalled UL TrCH TFS for DL TrCH */ 

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"No DL TFCS was included for TrCH ID");
        return FAILURE;

    } /* end TFS selection switch */

    /*Find Rbs mapped to this DCH and fill them in rb_list_mapped_to_dch if their RLC
    PDU size has changed.*/ 
    tdsrrcllc_get_rb_for_rb_list_mapped_to_dch((tr_ch_id_type)ie_ptr->dl_transportChannelIdentity, 
              tdsordered_config_ptr->rrc_state_indicator, TDSDL);   
  } /* end CELL_DCH check */

  /* States not supported */
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_FACH not supported for TrCH");
     return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_PCH not supported for TrCH");
     return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"URA_PCH not supported for TrCH");
     return FAILURE;
  }
  else
  {
     ERR_FATAL("Unknown state not supported for TrCH",0,0,0);
  }

  /* Check for the BLER */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation_r4,
    dch_QualityTarget))
  {
    /* Save the BLER if found, and is in range */
    if((ie_ptr->dch_QualityTarget.bler_QualityValue >= TDSRRCLLC_MIN_BLER) &&
       (ie_ptr->dch_QualityTarget.bler_QualityValue <= TDSRRCLLC_MAX_BLER)
      )
    {
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val =
        (int8)ie_ptr->dch_QualityTarget.bler_QualityValue;
    }

    /* BLER is out of range */
    else
    {
      /* According to L1 I/F, must set an invalid value of -127 */
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL BLER %d is out of range",ie_ptr->dch_QualityTarget.bler_QualityValue);
      return FAILURE;
    }
  } /* end if BLER present */
  else
  {
    /* IE omitted. Set the target to -127 which would result 1% of BLER target.
     * This is good UE behaviour though the spec is ambiguous on what UE
     * behavior should be in this case.  
     */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DCH quality target not present. Setting to -127");
    tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
  }


  /* Indicate that DL L1 and MAC needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;

  return SUCCESS;
} /* end tdsrrcllc_get_dl_added_reconfig_trch_2_r5() */

/*============================================================================
FUNCTION: tdsrrcllc_copy_dl_dch_mac_info()

DESCRIPTION:
  This function copies all Downlink DCH transport channel related info 
  relevant to MAC from source data structure to destination data structure.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE.

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type
tdsrrcllc_copy_dl_dch_mac_info
(
  tdsmac_dl_dch_trch_config_type *dest_data,
  tdsmac_dl_dch_trch_config_type *src_data
)
{
  int i;
  if((dest_data == NULL)||(src_data == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"NULL pointer passed");
    return(FAILURE);
  }

  dest_data->cctrch_id = src_data->cctrch_id;
  dest_data->trch_id   = src_data->trch_id;
  dest_data->ndlchan   = src_data->ndlchan;

  /* Copy data for all dedicated channels */
  for(i=0; i<MAX_DED_LOGCHAN_PER_TRCH; i++)
  {
    dest_data->dlchan_info[i] = src_data->dlchan_info[i];
  }

  return(SUCCESS);
}  /* tdsrrcllc_copy_dl_dch_mac_info */

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
void  tdsrrcllc_copy_dl_dch_trch_info_to_buffer(uint32 trch_idx)
{
  /* Copies all Downlink DCH Transport Channel related data to a 
   * temporary buffer
   */ 
  uint32  tf_count;
  /* Copy the L1 data first */
  /* Note: the mapping between transport channel data and the corresponding
   * transport format data is kept unchanged. ie a transport channel at index 
   * "i" will still point to transport format data at index "i".
   */
  tdsdl_dch_info_buffer.l1_info = tdsordered_config_ptr->dl_trch_info[trch_idx];

  for(tf_count = 0; tf_count < TDSUE_MAX_TF; tf_count ++)
  {
    /* Copy each transport format data from src to dest index */
    tdsdl_dch_info_buffer.l1_tf_info[tf_count] = 
                tdsordered_config_ptr->dl_tf_info[trch_idx][tf_count];
    /* Point L1 data at dest_trch_idx to corresponding transport format data */
    tdsdl_dch_info_buffer.l1_info.tf_info_array[tf_count] = 
              &tdsdl_dch_info_buffer.l1_tf_info[tf_count];
  }

  /* Copy MAC data next */
  (void)tdsrrcllc_copy_dl_dch_mac_info(&tdsdl_dch_info_buffer.mac_info, 
                              &tdsordered_config_ptr->dl_dch_info[trch_idx]);

  /* Copy local data now */
  tdsdl_dch_info_buffer.local_info = tdsordered_config_ptr->dl_dch_trch_info[trch_idx];
} /* tdsrrcllc_copy_dl_dch_trch_info_to_buffer */

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
void  tdsrrcllc_copy_dl_dch_trch_info_from_buffer(uint32 trch_idx)
{
  /* Copies all Downlink DCH Transport Channel related data from
   * a buffer to the various local and non-local data structures.
   */
  uint32  tf_count;

  /* Copy the L1 data first */
  /* Note: the mapping between transport channel data and the corresponding
   * transport format data is kept unchanged. ie a transport channel at index 
   * "i" will still point to transport format data at index "i".
   */
  tdsordered_config_ptr->dl_trch_info[trch_idx] = tdsdl_dch_info_buffer.l1_info;

  for(tf_count = 0; tf_count < TDSUE_MAX_TF; tf_count ++)
  {
    /* Copy each transport format data from src to dest index */
    tdsordered_config_ptr->dl_tf_info[trch_idx][tf_count] =
      tdsdl_dch_info_buffer.l1_tf_info[tf_count];
    /* Point L1 data at dest_trch_idx to corresponding transport format data */
    tdsordered_config_ptr->dl_trch_info[trch_idx].tf_info_array[tf_count] =
      &tdsordered_config_ptr->dl_tf_info[trch_idx][tf_count];
  }

  /* Copy MAC data next */
  (void)tdsrrcllc_copy_dl_dch_mac_info(&tdsordered_config_ptr->dl_dch_info[trch_idx],
                              &tdsdl_dch_info_buffer.mac_info); 

  /* Copy local data now */
  tdsordered_config_ptr->dl_dch_trch_info[trch_idx] = tdsdl_dch_info_buffer.local_info;
} /* tdsrrcllc_copy_dl_dch_trch_info_from_buffer */


/*====================================================================
FUNCTION        tdsrrcllc_copy_ul_tfcs_info_to_dl_tfcs()

DESCRIPTION     This function copied TFCS information from Uplink TFCS
                in Ordered Config to Downlink TFCS information in Ordered
                Config

DEPENDENCIES
RETURN VALUE    None

SIDE EFFECTS
====================================================================*/
void tdsrrcllc_copy_ul_tfcs_info_to_dl_tfcs
(
  void
)
{
    /* local variables */
  uint8 ix=0, iy=0;


  /* Set flag indicating that dl_tfcs has been rcvd */
  tdsdl_tfcs_rcvd = TRUE;

  tdsrrcllc_init_dl_ctfcs(tdsordered_config_ptr);

  for(ix = 0; ix < TDSUE_MAX_TFC; ix++)
  {
    /* Init the DL Transport Format Combination data pointers */
    tdsordered_config_ptr->l1_dl_ctfc_parms.ctfc_entry_array[ix] = &tdsordered_config_ptr->ctfc_info[ix];
    /* Initialize the pointer to tfi_table in CTFC Info */
    tdsordered_config_ptr->ctfc_info[ix].tfi_table = tdsordered_config_ptr->dl_ctfc.ctfc_entry_data[ix].tfi_table;
  }

  /* Now copy the ctfcs from UL to DL */

  if(tdsordered_config_ptr->ul_tfcs.ntfc != 0)
  {
  tdsordered_config_ptr->dl_ctfc.num_ctfc_entry = 
    tdsordered_config_ptr->l1_dl_ctfc_parms.num_ctfc_entry = 
     tdsordered_config_ptr->ul_tfcs.ntfc;
  }
  else
  {
    tdsordered_config_ptr->dl_ctfc.num_ctfc_entry = 
      tdsordered_config_ptr->l1_dl_ctfc_parms.num_ctfc_entry = 0;  
  }


  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CTFC NUMBER L1_DL_CTFC_PARMS in OC %d",tdsordered_config_ptr->l1_dl_ctfc_parms.num_ctfc_entry);


  /* Set TFCI Existence */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"NO OF UL TFCs %d",tdsordered_config_ptr->ul_tfcs.ntfc);


  /* Get the tfc */
  for(ix = 0; ix < tdsordered_config_ptr->ul_tfcs.ntfc; ix++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"ULTFC No %d,Flag [0:A,1NA,2M,3RM] %d",ix,tdsordered_config_ptr->ul_tfcs.tfc_info[ix].control_flag);
  
    if (tdsordered_config_ptr->ul_tfcs.tfc_info[ix].control_flag == TDSTFC_NOT_ALLOWED)
    {
      tdsordered_config_ptr->dl_ctfc.ctfc_entry_data[ix].state = TDSRESTRICTED;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DLTFC No %d,State Restricted",ix);
    }
    else if ((tdsordered_config_ptr->ul_tfcs.tfc_info[ix].control_flag == TDSTFC_ALLOWED) ||
             (tdsordered_config_ptr->ul_tfcs.tfc_info[ix].control_flag == TDSTFC_MANDATORY))
    {
      tdsordered_config_ptr->dl_ctfc.ctfc_entry_data[ix].state = TDSALLOWED;
      tdsordered_config_ptr->l1_dl_ctfc_parms.ctfc_entry_array[ix]->valid_ctfc = TRUE;

       TDSRRC_MSG1(MSG_LEGACY_HIGH,"DLTFC No %d,State Allowed",ix);

      tdsordered_config_ptr->dl_ctfc.ctfc_entry_data[ix].ctfc_value = 
        tdsordered_config_ptr->ul_tfcs.tfc_info[ix].ctfc;
    }
    else
    {
      tdsordered_config_ptr->dl_ctfc.ctfc_entry_data[ix].state = TDSINVALID;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DLTFC No %d,State Invalid",ix);
    }

    /* Set in l1_dl_ctfc_parms*/

    for(iy = 0; iy < TDSUE_MAX_TRCH; iy++)
    {
      tdsordered_config_ptr->l1_dl_ctfc_parms.ctfc_entry_array[ix]->tfi_table[iy] = 
        tdsordered_config_ptr->ul_tfcs.tfc_info[ix].tfi[iy];
    }
  }

}


/*============================================================================
FUNCTION: tdsrrcllc_copy_dl_dch_trch_info_from_one_to_next_index()

DESCRIPTION:
  This function copies all Downlink DCH transport channel related info from
  L1, MAC and local data structures indexed by the passed source index to
  L1, MAC and local data structures indexed by the passed destination index.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
void tdsrrcllc_copy_dl_dch_trch_info_from_one_to_next_index(
      uint8 dest_trch_idx, 
      uint8 src_trch_idx
)
{
  /* Copies all DL DCH related Transport Channel data from Source
   * Transport Channel Index to Destination Transport Channel Index.
   */
  uint32  tf_count;

  /* Copy the L1 data first */
  /* Note: the mapping between transport channel data and the corresponding
   * transport format data is kept unchanged. ie a transport channel at index 
   * "i" will still point to transport format data at index "i".
   */
  tdsordered_config_ptr->dl_trch_info[dest_trch_idx] = tdsordered_config_ptr->dl_trch_info[src_trch_idx];
  
  for(tf_count = 0; tf_count < TDSUE_MAX_TF; tf_count ++)
  {
    /* Copy each transport format data from src to dest index */
    tdsordered_config_ptr->dl_tf_info[dest_trch_idx][tf_count] = 
                tdsordered_config_ptr->dl_tf_info[src_trch_idx][tf_count];
    /* Point L1 data at dest_trch_idx to corresponding transport format data */
    tdsordered_config_ptr->dl_trch_info[dest_trch_idx].tf_info_array[tf_count] = 
              &tdsordered_config_ptr->dl_tf_info[dest_trch_idx][tf_count];
  }

  /* Copy MAC data next */
  (void)tdsrrcllc_copy_dl_dch_mac_info(&tdsordered_config_ptr->dl_dch_info[dest_trch_idx], 
                              &tdsordered_config_ptr->dl_dch_info[src_trch_idx]);
  
  /* Copy local data now */
  tdsordered_config_ptr->dl_dch_trch_info[dest_trch_idx] = tdsordered_config_ptr->dl_dch_trch_info[src_trch_idx];
  
} /* tdsrrcllc_copy_dl_dch_trch_info_from_one_to_next_index */


/*============================================================================
FUNCTION: tdsrrcllc_sort_dl_dch_trch_index_array()

DESCRIPTION:
  It sorts and re-arranges all Transport channel related 
  downlink info based on Transport channel IDs.
      
DEPENDENCIES:
  This function presumes that there are no "holes" in transport channel
  index array. Any holes have to be removed before a call to this function.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
static void  tdsrrcllc_sort_dl_dch_trch_index_array(
  tdsrrcllcoc_trch_info_type *dch_trch_info_ptr, 
  uint32  num_index
)
{
  uint8  index_for_sorting;
  uint8  to_be_compared_index;

  /* Make sure num_index doesn't read beyond the end of dch_trch_info_ptr_array */
  if (num_index > TDSUE_MAX_TRCH) 
  {
    ERR_FATAL("num_idx %d greater than TDSUE_MAX_TRCH %d",num_index,TDSUE_MAX_TRCH,0);
  }

  for(index_for_sorting = 1; index_for_sorting < num_index; index_for_sorting ++)
  {
    /* If the Transport Channel ID for the index for sort is NOT lower than the Transport
     * Channel ID for the index one lower than the one being sorted, do NOT do anything.
     * This is because all indices further below have already been sorted.
     */
    if((dch_trch_info_ptr[index_for_sorting].trch_id < 
          dch_trch_info_ptr[index_for_sorting-1].trch_id) &&
       ( dch_trch_info_ptr[index_for_sorting].trch_id != 0))
    {
      /* The TrCH ID for sorted index is lower than TrCH ID for index before
       * This is time to move all indices below till a space is found for 
       * data at current sorted index.
       */
      tdsrrcllc_copy_dl_dch_trch_info_to_buffer(index_for_sorting);
      to_be_compared_index = index_for_sorting;

      while( (to_be_compared_index > 0) && 
             (tdsdl_dch_info_buffer.local_info.trch_id < 
               dch_trch_info_ptr[to_be_compared_index-1].trch_id)
           )
      {
        dch_trch_info_ptr[to_be_compared_index] = 
          dch_trch_info_ptr[to_be_compared_index - 1];
        tdsrrcllc_copy_dl_dch_trch_info_from_one_to_next_index(
                                     to_be_compared_index,
                                     to_be_compared_index-1);
        to_be_compared_index --;
      }
  
      tdsrrcllc_copy_dl_dch_trch_info_from_buffer(to_be_compared_index);
    }
  } /* for(...) */
} /* tdsrrcllc_sort_dl_dch_trch_index_array */


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
  tdsrrc_DL_DeletedTransChInfoList    * dl_trch_delete_ptr
)
{
  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;
  uint32 idx = 0, idy = 0;
  tdsrrc_DL_DeletedTransChInfoList *temp_dl_trch_delete_ptr = dl_trch_delete_ptr;
  tdsrrc_DL_AddReconfTransChInfoList  *temp_dl_trch_add_reconfig_ptr = dl_trch_add_reconfig_ptr;
  
  if((temp_dl_trch_delete_ptr != NULL) && (temp_dl_trch_add_reconfig_ptr != NULL))
  {
    while(temp_dl_trch_delete_ptr->n > idx)
    {
      if(tdsrrc_DL_TrCH_Type_dch == temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType)
      {
        idy = 0;
        while(temp_dl_trch_add_reconfig_ptr->n > idy)
        {
          if(tdsrrc_DL_TrCH_Type_dch == temp_dl_trch_add_reconfig_ptr->elem[idy].dl_TransportChannelType)
          {
            if(temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelIdentity == temp_dl_trch_add_reconfig_ptr->elem[idy].dl_transportChannelIdentity)
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Same Trch ID found in add/reconfig list %d and deleted list %d ",temp_dl_trch_add_reconfig_ptr->elem[idy].dl_transportChannelIdentity,temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelIdentity);
              return FAILURE;
            }
          }
          idy++;
        }
      }   
      idx++;
    }
  }
  idx = 0;
  while((dl_trch_delete_ptr != NULL) && (dl_trch_delete_ptr->n > idx))
  {
    /* Remove this transport channel from MAC, L1 and local data.
     * and update transport channel count in L1, MAC and local data.
     */
    if(tdsrrcllc_get_dl_deleted_trch(&dl_trch_delete_ptr->elem[idx]) == FAILURE)
    {
      return FAILURE;
    }
    /* set tdsdl_tfcs_needed flag to true.  Later if common info is not preset,
     * UE needs to reject the message 
     */
    tdsdl_tfcs_needed = TRUE;
    /* get next TrCH Info */
    idx++;
  } /* end while */
  idx =0;
  /* Get the DL Added/Reconfigured Info */
  while((dl_trch_add_reconfig_ptr !=NULL) && (dl_trch_add_reconfig_ptr->n > idx) )
  {
    tdstrch_id = (tr_ch_id_type)dl_trch_add_reconfig_ptr->elem[idx].dl_transportChannelIdentity;

    if((tdsrrcllc_get_dl_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't find idx,TrCH ID %d",tdstrch_id);
      return(FAILURE);
    }
    idx++;
  } /* while(...) */

  /* At this point, the transport channel list -
   *  - may have holes due to deleted channels.
   *  - transport channel info may not be sorted by TrCH IDs.
   */ 

  /* Fill holes and put all Transport channels in the first
   * tdsordered_config_ptr->dl_trch_cnt indices.
   */
  tdsrrcllc_fill_holes_in_dl_dch_trch_idx();

  /* Now is the time and place for sorting all transport channel
   * info based on TrCH IDs. Call the right function.
   */

  tdsrrcllc_sort_dl_dch_trch_index_array(&tdsordered_config_ptr->dl_dch_trch_info[0], 
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING)
                                     tdsordered_config_ptr->mac_dl_parms.num_dchs
#else
                                      (TDSRRCLLC_DOWNLINK_TRCH_COUNT(tdsordered_config_ptr))
#endif 
                                      );
  return(SUCCESS);
} /* tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info */




/*=============================================================================
FUNCTION: tdsrrcllc_get_dl_deleted_trch_r5()

DESCRIPTION:
  This function takes a pointer to the Downlink Deleted 
  Transport Channel IE and Deletes if from Ordered Config.
  In the TFS it resets all information for this TrCH.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
===============================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_deleted_trch_r5
(
  tdsrrc_DL_TransportChannelIdentity_r5 * ie_ptr
)
{
  /* TrCH Index */
  uint8 ix;
  uint8 i;

  if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,rrc_DL_TrCH_TypeId2_r5_dch))
  {
    ix = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch);
    if(ix >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"No DL DCH index for TrCH ID %d",ie_ptr->dl_TransportChannelType.u.dch);
      return(SUCCESS);
    }
    tdsrrcllc_backup_rbmapping_info(ix,TDSDL_DCH_RB_MAPPING );

    /* Clear the RB Mapping info for this TrCH */
    tdsrrcllc_init_dl_dch_rb_mapping_info(tdsordered_config_ptr, ix);
    
    /* Clear the TFS for this TrCH */
    tdsrrcllc_init_dl_tfs_table(tdsordered_config_ptr, ix);
    
    /* Update MAC count */
    tdsordered_config_ptr->mac_dl_parms.num_dchs--;
    /* Initialize local data */
    tdsordered_config_ptr->dl_dch_trch_info[ix].dch_present = FALSE;
    tdsordered_config_ptr->dl_dch_trch_info[ix].trch_id     = 0;
    /* Indicate that the old index is now available */
    if(tdsrrcllc_deallocate_dl_dch_trch_idx(ix) == FAILURE)
    {
      return FAILURE;
    }
    /* Update L1 count */
    tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch--;
    /* set tdsdl_tfcs_needed flag to true.  Later if common info is not preset,
     * UE needs to reject the message 
     */
    tdsdl_tfcs_needed = TRUE;
  }
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
     rrc_DL_TrCH_TypeId2_r5_hsdsch))
  {
    TDSRRC_MSG1(MSG_LEGACY_LOW,"RRCHS:Delete DFLOW %d",ie_ptr->dl_TransportChannelType.u.hsdsch);
    if ((ix = tdsrrcllc_find_dflow_index((uint8)ie_ptr->dl_TransportChannelType.u.hsdsch)) >= UE_MAX_MAC_D_FLOW)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Unable to index for DFLOW %d",ie_ptr->dl_TransportChannelType.u.hsdsch);
      /* Allow the Reconfig Msg to be processed since spec allows it implicitly */
      return(SUCCESS);
    }
    /* Clear the Queue Information associated with the DFLOW*/

    tdsrrcllc_backup_rbmapping_info(ix,TDSMAC_D_RB_MAPPING );
    
    TDSRRC_MSG1(MSG_LEGACY_LOW,"RRCHS:Initialize Queue for DFLOW %d",ie_ptr->dl_TransportChannelType.u.hsdsch);
    /* Initialize the queue info */
    tdsrrcllc_initialize_queue_info(ix);

    tdshsdpa_msg_params.msg_params_present = TRUE;

    tdshsdpa_msg_params.hsdsch_info_present = TRUE;
   /* If the Flow id was actually not deleted, that is the Rb-Mapping for this flow id is still present in the 
    * ordered config then delete it from Backup info
    */
    if ((ix = tdsrrcllc_find_dflow_index((uint8)ie_ptr->dl_TransportChannelType.u.hsdsch)) == UE_MAX_MAC_D_FLOW)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Mac-d flow id %d got deleted ",(ie_ptr->dl_TransportChannelType.u.hsdsch));
    }
    else
    {
      for(i=0;i<tdsordered_config_ptr->mac_dflow_info[ix].ndlchan;i++)
      {
        tdsrrcllc_free_hanging_rb_mapping_info_index(tdsordered_config_ptr->mac_dflow_info[ix].dlchan_info[i].rb_id, TDSMAC_D_RB_MAPPING);
      }
    }

  }
  
  else  /* Must be FACH */
  {
    // ix = tdsrrcllc_find_dl_fach_trch_idx(ie_ptr->value.dl_TransportChannelIdentity); 
    ix = tdsrrcllc_find_dl_fach_trch_idx();
    if(ix >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No FACH index for TrCH ID");
      return(FAILURE);
    }
    tdsrrcllc_backup_rbmapping_info(ix,TDSFACH_RB_MAPPING );

    /* Clear the RB Mapping info for this TrCH */
    tdsrrcllc_init_fach_rb_mapping_info(tdsordered_config_ptr, ix);
    
    /* Clear the TFS for this TrCH */
    tdsrrcllc_init_dl_tfs_table(tdsordered_config_ptr, ix);
    
    /* Update MAC count */
    tdsordered_config_ptr->mac_dl_parms.num_fachs--;

    /* Update L1 count */
    tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch--;
  }
  
  return SUCCESS;
} /* end tdsrrcllc_get_dl_deleted_trch_r5() */



/*============================================================================
FUNCTION: tdsrrcllc_copy_mac_ul_dch_mapping_info()

DESCRIPTION:
  This function copies all Uplink DCH transport channel mapping info 
  relevant to MAC from source data structure to destination data structure.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
static void  tdsrrcllc_copy_mac_ul_dch_mapping_info(
  tdsmac_ul_dch_trch_config_type *ul_dch_dest_info_ptr, 
  tdsmac_ul_dch_trch_config_type *ul_dch_src_info_ptr 
)
{
  uint32   i;
  /* First copy all mapping info */
  ul_dch_dest_info_ptr->cctrch_id = ul_dch_src_info_ptr->cctrch_id;

  ul_dch_dest_info_ptr->trch_id = ul_dch_src_info_ptr->trch_id;

  /* Copy all info about dedicated logical channels */
  ul_dch_dest_info_ptr->ndlchan = ul_dch_src_info_ptr->ndlchan;

  for(i = 0; i < MAX_DED_LOGCHAN_PER_TRCH; i ++)
  {
    ul_dch_dest_info_ptr->dlchan_info[i] = 
      ul_dch_src_info_ptr->dlchan_info[i];
  }
} /* tdsrrcllc_copy_mac_ul_dch_mapping_info */

/*============================================================================
FUNCTION: tdsrrcllc_copy_mac_ul_dch_tfs_info()

DESCRIPTION:
  This function copies all Uplink DCH transport channel transport format info 
  relevant to MAC from source data structure to destination data structure.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
static void tdsrrcllc_copy_mac_ul_dch_tfs_info(
  tdsmac_ul_tfs_type   *dest_tfs_ptr,
  tdsmac_ul_tfs_type   *src_tfs_ptr
  )
{
  uint32  i;  

  dest_tfs_ptr->ntf = src_tfs_ptr->ntf;
  dest_tfs_ptr->tti_info  = src_tfs_ptr->tti_info;
  dest_tfs_ptr->mac_header_size  = src_tfs_ptr->mac_header_size;

  /* copy the Transport format set info */
  for(i = 0; i < TDSUE_MAX_TF; i ++)
  {
    dest_tfs_ptr->tf_info[i] = src_tfs_ptr->tf_info[i];
  }
}  /* tdsrrcllc_copy_mac_ul_dch_tfs_info */

/*============================================================================
FUNCTION: tdsrrcllc_copy_ul_dch_trch_info_from_one_to_next_index()

DESCRIPTION:
  This function copies all Uplink DCH transport channel related info from
  L1, MAC and local data structures indexed by the passed source index to
  L1, MAC and local data structures indexed by the passed destination index.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
void  tdsrrcllc_copy_ul_dch_trch_info_from_one_to_next_index(
      uint8 dest_trch_idx, 
      uint8 src_trch_idx
      )      
{
  /* Copies all UL DCH related Transport Channel data from Source
   * Transport Channel Index to Destination Transport Channel Index.
   */
  /* Copy the L1 data first */
  tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[dest_trch_idx] = 
        tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[src_trch_idx];
  /* Copy MAC data next */
  tdsrrcllc_copy_mac_ul_dch_mapping_info(&tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[dest_trch_idx],
                                      &tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[src_trch_idx]);

  tdsrrcllc_copy_mac_ul_dch_tfs_info(&tdsordered_config_ptr->ul_tfcs.tfs_info[dest_trch_idx], 
                                  &tdsordered_config_ptr->ul_tfcs.tfs_info[src_trch_idx]);
  
  /* Copy local data now */
  tdsordered_config_ptr->ul_dch_trch_info[dest_trch_idx] = tdsordered_config_ptr->ul_dch_trch_info[src_trch_idx];
  
} /* tdsrrcllc_copy_ul_dch_trch_info_from_one_to_next_index */

/*============================================================================
FUNCTION: tdsrrcllc_delete_ul_dch_trch_index()

DESCRIPTION:
  This function takes a Transport channel index as a
  parameter and clears local, L1 and MAC data at the
  given index. It also decrements count of total number
  of transport channels in local, MAC and L1 data.
  
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE.

SIDE EFFECTS:
  CELL_DCH deletions are the only item supported now.
=============================================================================*/
uecomdef_status_e_type tdsrrcllc_delete_ul_dch_trch_index
(
  uint8   trch_idx
)
{
  if(trch_idx < TDSUE_MAX_TRCH)
  {
    /* Clear the RB Mapping info (MAC) for this TrCH */
    tdsrrcllc_init_ul_dch_rb_mapping_info(tdsordered_config_ptr, trch_idx);

    /* Clear the TFS (L1) for this TrCH */
    tdsrrcllc_init_ul_tfs_table(tdsordered_config_ptr, trch_idx);

    /* Initialize local data */
    tdsordered_config_ptr->ul_dch_trch_info[trch_idx].dch_present = FALSE;
    tdsordered_config_ptr->ul_dch_trch_info[trch_idx].trch_id     = 0;

    /* Free the TrCH index for further use */
    if(tdsrrcllc_deallocate_ul_dch_trch_idx(trch_idx) == FAILURE)
    {
      return FAILURE;
    }

    /* One less TrCH - update local, MAC and L1 data structures */
    tdsordered_config_ptr->mac_ul_parms.cctrch_ptr->dch_info.ndchs --;
    tdsordered_config_ptr->mac_ul_parms.tfcs_ptr->nchan --;

    tdsordered_config_ptr->rrc_ul_cctrch_params.num_trch--; 

    TDSRRC_MSG1(MSG_LEGACY_MED,"Deleted UL TrCH Idx %d",trch_idx);
    return(SUCCESS);
  }
  else
  {
    return(FAILURE);
  }
} /* end tdsrrcllc_delete_ul_dch_trch_index() */

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
void  tdsrrcllc_copy_ul_dch_trch_info_to_buffer(uint32 trch_idx)
{
  /* Copies all Uplink DCH Transport Channel related data to a 
   * temporary buffer
   */ 
  /* Copy the L1 data first */
  tdsul_dch_info_buffer.l1_semi_static_parms = 
          tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[trch_idx];
  /* Copy MAC data next */
  tdsrrcllc_copy_mac_ul_dch_mapping_info(&tdsul_dch_info_buffer.mac_dch_info,
                                      &tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx]);

  tdsrrcllc_copy_mac_ul_dch_tfs_info(&tdsul_dch_info_buffer.mac_tfs_info, 
                                  &tdsordered_config_ptr->ul_tfcs.tfs_info[trch_idx]);

  /* Copy local data now */
  tdsul_dch_info_buffer.local_info = tdsordered_config_ptr->ul_dch_trch_info[trch_idx];
} /* tdsrrcllc_copy_ul_dch_trch_info_to_buffer */

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
void  tdsrrcllc_copy_ul_dch_trch_info_from_buffer(uint32 trch_idx)
{
  /* Copies all Downlink DCH Transport Channel related data from
   * a buffer to the various local and non-local data structures.
   */

  /* Copy the L1 data first */
  tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[trch_idx] = tdsul_dch_info_buffer.l1_semi_static_parms;
  /* Copy MAC data next */
  tdsrrcllc_copy_mac_ul_dch_mapping_info(&tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx],
                                      &tdsul_dch_info_buffer.mac_dch_info);

  tdsrrcllc_copy_mac_ul_dch_tfs_info(&tdsordered_config_ptr->ul_tfcs.tfs_info[trch_idx],
                                  &tdsul_dch_info_buffer.mac_tfs_info);

  /* Copy local data now */
  tdsordered_config_ptr->ul_dch_trch_info[trch_idx] = tdsul_dch_info_buffer.local_info;
} /* tdsrrcllc_copy_ul_dch_trch_info_from_buffer */


/*============================================================================
FUNCTION: tdsrrcllc_sort_ul_dch_trch_index_array()

DESCRIPTION:
  It sorts and re-arranges all Transport channel related 
  info based on Transport channel IDs.
      
DEPENDENCIES:
  This function presumes that there are no "holes" in transport channel
  index array. For correct results, all holes HAVE to be removed before
  calling this function.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
static void  tdsrrcllc_sort_ul_dch_trch_index_array(
  tdsrrcllcoc_trch_info_type *dch_trch_info_ptr, 
  uint32  num_index
)
{
  uint8  index_for_sorting;
  uint8  to_be_compared_index;

  /* Make sure num_index doesn't read beyond the end of dch_trch_info_ptr_array */
  if (num_index > TDSUE_MAX_TRCH) 
  {
    ERR_FATAL("num_idx %d greater than TDSUE_MAX_TRCH %d",num_index,TDSUE_MAX_TRCH,0);
  }

  for(index_for_sorting = 1; index_for_sorting < num_index; index_for_sorting ++)
  {
    /* If the Transport Channel ID for the index for sort is NOT lower than the Transport
     * Channel ID for the index one lower than the one being sorted, do NOT do anything.
     * This is because all indices further below have already been sorted.
     */
    if(dch_trch_info_ptr[index_for_sorting].trch_id < 
        dch_trch_info_ptr[index_for_sorting-1].trch_id)
    {
      /* The TrCH ID for sorted index is lower than TrCH ID for index before
       * This is time to move all indices below till a space is found for 
       * data at current sorted index.
       */
      tdsrrcllc_copy_ul_dch_trch_info_to_buffer(index_for_sorting);

      to_be_compared_index = index_for_sorting;

      while( (to_be_compared_index > 0) && 
             (tdsul_dch_info_buffer.local_info.trch_id < 
                dch_trch_info_ptr[to_be_compared_index-1].trch_id)
           )
      {
         tdsrrcllc_copy_ul_dch_trch_info_from_one_to_next_index(
                                       to_be_compared_index, 
                                       to_be_compared_index-1);
        to_be_compared_index --;
      }
  
      tdsrrcllc_copy_ul_dch_trch_info_from_buffer(to_be_compared_index);
    }
  } /* for(...) */
} /* tdsrrcllc_sort_ul_dch_trch_index_array */


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
  tdsrrc_UL_AddReconfTransChInfoList  * ul_trch_add_reconfig_ptr,
  tdsrrc_UL_DeletedTransChInfoList   * ul_trch_delete_ptr
)
{

  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;
  uint32 idx =0, idy=0;
  tdsrrc_UL_DeletedTransChInfoList* temp_ul_trch_delete_ptr = ul_trch_delete_ptr;
  tdsrrc_UL_AddReconfTransChInfoList* temp_ul_trch_add_reconfig_ptr = ul_trch_add_reconfig_ptr;

  if((temp_ul_trch_delete_ptr != NULL) && (temp_ul_trch_add_reconfig_ptr != NULL))
  {
    while(temp_ul_trch_delete_ptr->n > idx)
    {
      if(tdsrrc_UL_TrCH_Type_dch == temp_ul_trch_delete_ptr->elem[idx].ul_TransportChannelType)
     {
       idy = 0;
        while(temp_ul_trch_add_reconfig_ptr->n > idy)
        {
          if(tdsrrc_UL_TrCH_Type_dch == temp_ul_trch_add_reconfig_ptr->elem[idy].ul_TransportChannelType)
          {
            if(temp_ul_trch_delete_ptr->elem[idx].ul_TransportChannelIdentity == temp_ul_trch_add_reconfig_ptr->elem[idy].transportChannelIdentity)
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Same Trch ID found in add/reconfig list %d and deleted list %d ",temp_ul_trch_add_reconfig_ptr->elem[idy].transportChannelIdentity,temp_ul_trch_delete_ptr->elem[idx].ul_TransportChannelIdentity);
              return FAILURE;
            }
          }
          idy++;
       }
     }   
      idx++;
    }
  }
  idx = 0;
  while((ul_trch_delete_ptr !=NULL )&& (ul_trch_delete_ptr->n > idx))
  {
    /* Remove this transport channel from MAC, L1 and local data.
     * and update transport channel count in L1, MAC and local data.
     */
    if(tdsrrcllc_get_ul_deleted_trch(&ul_trch_delete_ptr->elem[idx]) == FAILURE)
    {
      return FAILURE;
    }
    /* set tdsul_tfcs_needed flag to true.  Later if common info is not present,
     * UE needs to reject the message 
     */
    tdsul_tfcs_needed = TRUE;
    /* get next TrCH Info */
    idx++;
  } /* end while */
  idx =0;
  /* Get the UL Added/Reconfigured Info */
  while((ul_trch_add_reconfig_ptr != NULL ) && (ul_trch_add_reconfig_ptr->n > idx) )
  {
    tdstrch_id = (tr_ch_id_type)ul_trch_add_reconfig_ptr->elem[idx].transportChannelIdentity;

    if((tdsrrcllc_get_ul_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't find idx,TrCH ID %d",tdstrch_id);
      return(FAILURE);
    }
    idx++;
  } /* while(...) */

  /* At this point, the transport channel list -
   *  - may have holes due to deleted channels.
   *  - transport channel info may not be sorted by TrCH IDs.
   */ 

  /* Fill holes and put all Transport channels in the first
   * tdsordered_config_ptr->ul_trch_cnt indices.
   */
  tdsrrcllc_fill_holes_in_ul_dch_trch_idx();

  /* Now is the time and place for sorting all transport channel
   * info based on TrCH IDs. Call the right function.
   */
  tdsrrcllc_sort_ul_dch_trch_index_array(&tdsordered_config_ptr->ul_dch_trch_info[0], 
                                      (TDSRRCLLC_UPLINK_TRCH_COUNT(tdsordered_config_ptr)));
  return(SUCCESS);
} /* tdsrrcllc_add_delete_and_arrange_ul_dch_trch_info */

/*============================================================================
FUNCTION: tdsrrcllc_check_if_tfi_present_in_allowed_list()

DESCRIPTION:
  This function checks if the TFI for a Trch is present
  in the allowed list list in Restricted transport channel list
  It takes a Transport channel index and  TFCI index and the 
  OTA Restricted transport channel list as   parameters.
DEPENDENCIES:
  None

RETURN VALUE:
  SUCCESS : If the TFI is present in the Allowed list  or the Trch ID
            itself was not present in the Restricted transport channel list
  else   FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type 
tdsrrcllc_check_if_tfi_present_in_allowed_list
(
  tdsrrc_RestrictedTrChInfoList * restricted_trch_tfc_ptr,
  uint32 tfci_index,
  uint32 trch_index 
  
)
{
  tdsrrc_AllowedTFI_List * temp_tfi_ptr;
  boolean trch_id_in_restricted_list = FALSE;
  uint8 tfi;
  uint32 tdstrch_id ;
  uint32 idx=0;
  uint32 idy =0;
  while(restricted_trch_tfc_ptr->n > idx)
  {
    if(restricted_trch_tfc_ptr->elem[idx].ul_TransportChannelType == tdsrrc_UL_TrCH_Type_dch)
    {
      tdstrch_id = restricted_trch_tfc_ptr->elem[idx].restrictedTrChIdentity;
      if((tdsordered_config_ptr->ul_dch_trch_info[trch_index].dch_present) &&
       ( (tdsordered_config_ptr->ul_dch_trch_info[trch_index].trch_id) == tdstrch_id ))
      {
        /* The Trch id is present in the Restriched Trch List in OTA */
        trch_id_in_restricted_list = TRUE;
        if(restricted_trch_tfc_ptr->elem[idx].m.allowedTFI_ListPresent)
        {
          temp_tfi_ptr = &restricted_trch_tfc_ptr->elem[idx].allowedTFI_List;
          idy =0;
          while(temp_tfi_ptr->n > idy)
          {
            /* Since the TFI is present in the allowed TFI list in OTA, return success */
            if( tdsordered_config_ptr->ul_tfcs.tfc_info[tfci_index].tfi[trch_index] == temp_tfi_ptr->elem[idy])
            {
              return SUCCESS;
            }
            idy++;
          }
        }
        else
        {
          /* If the Allowed List for this Trch is not present, 
            * then TB size 0 shld be treated as TDSALLOWED, all other TFIs should be
            * NOT TDSALLOWED
            */
          tfi =tdsordered_config_ptr->ul_tfcs.tfc_info[tfci_index].tfi[trch_index];
                
          if(tdsordered_config_ptr->ul_tfcs.tfs_info[trch_index].tf_info[tfi].ntblks *
            tdsordered_config_ptr->ul_tfcs.tfs_info[trch_index].tf_info[tfi].tb_size == 0)
          { 
           /*No allowed list, but TDSALLOWED TFI as TB Size =0 */
            return SUCCESS;
          } 
        }
      }
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"recieved  NON DCH type %d for trch id %d ",restricted_trch_tfc_ptr->elem[idx].ul_TransportChannelType,restricted_trch_tfc_ptr->elem[idx].restrictedTrChIdentity);
    }
    if(trch_id_in_restricted_list == TRUE )
    {
      break;
    }
    idx++;
  }
  /* We return success as, the Trch ID was not present in the 
    * Restriched Trch List in OTA, so all TFI for that Trch is allowed
    */  
  if(trch_id_in_restricted_list == FALSE)
  {
    return SUCCESS;
  }
  else
  {
    /* We return FAILURE as, the Trch ID was present in the 
      * Restriched Trch List in OTA, but not in the allowed TFI list
      */  
    return FAILURE;
  }
}
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
  void                     *ie_ptr, 
  tdsrrc_msg_ver_enum_type     rel_ver
)
{
  tdsrrc_TFC_Subset             *tfc_subset_ptr = NULL;
  tdsrrc_AllowedTFC_List        * allowed_tfc_ptr = NULL;
  tdsrrc_Non_allowedTFC_List     * non_allowed_tfc_ptr = NULL;
  tdsrrc_RestrictedTrChInfoList  * restricted_trch_tfc_ptr = NULL;
  uint32  tr_ch_indx[TDSUE_MAX_TRCH] = {0};
  uint32  no_of_tr_ch_ids = 0;
  uint32 allowed_tfc_number = TDSUE_MAX_TFC + 1;
  uint32 i = 0, j = 0, k = 0;
  boolean transport_channel_id_present = FALSE;
  tdsrrc_AllowedTFI_List * temp_tfi_ptr;
  uint32 idx=0;
  /* As the IE's of our interest are same for Rel-99 & Rel-5,
     point a local pointer to the embedded TFC subset field */

  /*Doing a check for RRC NV rel indicator here doesn't make sense. Because it should
  have been done earlier itself. But keeping it as per the legacy code.*/
  if((rel_ver == TDSMSG_REL4)
    ||(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5 && rel_ver == TDSMSG_REL5)
    ||(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6 && rel_ver == TDSMSG_REL6)
    ||(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7 && rel_ver == TDSMSG_REL7)
#ifdef FEATURE_TDSCDMA_REL8
    ||(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8 && rel_ver == TDSMSG_REL8)
#endif /* FEATURE_TDSCDMA_REL8 */  
#ifdef FEATURE_TDSCDMA_REL9
    ||(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9 && rel_ver == TDSMSG_REL9)
#endif /* FEATURE_TDSCDMA_REL9 */  
    )
  {
    /* if mode is FDD, will return failure in tdsrrcllc_get_ul_info_common_for_all_trch_r5 */
    if(((tdsrrc_UL_CommonTransChInfo_r4 *)ie_ptr)->m.modeSpecificInfoPresent)
    {
      if (((tdsrrc_UL_CommonTransChInfo_r4 *)ie_ptr)->modeSpecificInfo.t == T_tdsrrc_UL_CommonTransChInfo_r4_modeSpecificInfo_tdd)
      {
        if (((tdsrrc_UL_CommonTransChInfo_r4 *)ie_ptr)->modeSpecificInfo.u.tdd->m.individualUL_CCTrCH_InfoListPresent)
        {
          tfc_subset_ptr = &((tdsrrc_UL_CommonTransChInfo_r4 *)ie_ptr)->modeSpecificInfo.u.tdd->individualUL_CCTrCH_InfoList.elem[0].tfc_Subset;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"TFC subset not present");
          return SUCCESS;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unsupport FDD mode in UL_CommonTransChInfo ");
        return FAILURE;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"TFC subset not present");
      return SUCCESS;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TFC subset restriction for unsupported release ... rejecting");
    return FAILURE;
  }

  /* Apply the TFC subset restrictions on top of the existing OC's TFCS table */
  TDSRRC_MSG1(MSG_LEGACY_MED,"TFC subset restriction type=%d, and TFCS list Changed",tfc_subset_ptr->t);

  tdsordered_config_ptr->mac_ul_parms.new_tfcs_list_present = TRUE;

  switch(tfc_subset_ptr->t)
  {
  case T_tdsrrc_TFC_Subset_minimumAllowedTFC_Number:

    /* all the TFC indices below & including this index are allowed for selection */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Minimum allowed TFC number choosen");

    allowed_tfc_number = tfc_subset_ptr->u.minimumAllowedTFC_Number;
    /* Since we do not reserve the TFCI0, we have the TFCI value less one than the passed OTA */
    if (allowed_tfc_number > 0)
    {
      allowed_tfc_number --;
    }
    else 
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd OTA TFCI is %d in Min_allowedTFC",allowed_tfc_number);
    }

    if(allowed_tfc_number < tdsordered_config_ptr->ul_tfcs.ntfc)
    {
      i = 0;
      while(i <= allowed_tfc_number)
      {
        /* Not even a single TFC should have been removed earlier */
        if(tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag 
                                                                == TDSTFC_REMOVED)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Found a TFC which is set to TDSTFC_REMOVED");
          return FAILURE;
        }
        i++;
      }

      /* Disallow all the combinations except the REMOVED ones
         & allow only the ones given by the network */
      i = 0;
      while(i < tdsordered_config_ptr->ul_tfcs.ntfc)
      {
        if(tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag != TDSTFC_REMOVED)
        {
          tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_NOT_ALLOWED;
        }
        i++;
      }
      
      i = 0;
      while(i <= allowed_tfc_number)
      {
        tdsordered_config_ptr->ul_tfcs.tfc_info[i++].control_flag = TDSTFC_ALLOWED;
      }
    }
    else
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Allowed TFC number=%d exceeds UE's capability=%d"
               "/current combinations=%d", allowed_tfc_number, 
               TDSUE_MAX_TFC, tdsordered_config_ptr->ul_tfcs.ntfc);
      return FAILURE;
    }
    break;
  
  case T_tdsrrc_TFC_Subset_allowedTFC_List:
    if(tfc_subset_ptr->u.allowedTFC_List != NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Allowed TFC list choosen");
      
      allowed_tfc_ptr = tfc_subset_ptr->u.allowedTFC_List;
      
      while(allowed_tfc_ptr->n >  idx)
      {
        /* Since we do not reserve the TFCI0, we have the TFCI value less one than the passed OTA */
        if (allowed_tfc_ptr->elem[idx] > 0)
        {
          allowed_tfc_ptr->elem[idx] --;
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd OTA TFCI is %d in allowedTFC",allowed_tfc_ptr->elem[idx]);
        }
        if((allowed_tfc_ptr->elem[idx] >= tdsordered_config_ptr->ul_tfcs.ntfc) ||
           (tdsordered_config_ptr->ul_tfcs.tfc_info[allowed_tfc_ptr->elem[idx]].control_flag == TDSTFC_REMOVED))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Found a TFC which is set to TDSTFC_REMOVED or allowed TFC number exceeded current combinations");
          return FAILURE;
        }
        idx++;
      }

      /* Disallow all the combinations except the REMOVED ones
         & allow only the ones given by the network */
      
      i = 0;
      while(i < tdsordered_config_ptr->ul_tfcs.ntfc)
      {
        if(tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag != TDSTFC_REMOVED)
        {
          tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_NOT_ALLOWED;
        }
        i++;
      }
      idx =0;
      allowed_tfc_ptr = tfc_subset_ptr->u.allowedTFC_List;
      while(allowed_tfc_ptr->n > idx)
      {
        tdsordered_config_ptr->ul_tfcs.tfc_info[allowed_tfc_ptr->elem[idx]].control_flag = TDSTFC_ALLOWED;
        idx++;
      }
    }
    break;

  case T_tdsrrc_TFC_Subset_non_allowedTFC_List:
    if(tfc_subset_ptr->u.non_allowedTFC_List != NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Non-allowed TFC list choosen");
      non_allowed_tfc_ptr = tfc_subset_ptr->u.non_allowedTFC_List;

      while(non_allowed_tfc_ptr->n > idx)
      {
        /* Since we do not reserve the TFCI0, we have the TFCI value less one than the passed OTA */
        if (non_allowed_tfc_ptr->elem[idx] > 0)
        {
          non_allowed_tfc_ptr->elem[idx] --;
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd OTA TFCI is %d in allowedTFC",non_allowed_tfc_ptr->elem[idx]);
        }
        if((non_allowed_tfc_ptr->elem[idx] >= tdsordered_config_ptr->ul_tfcs.ntfc) ||
             (tdsordered_config_ptr->ul_tfcs.tfc_info[non_allowed_tfc_ptr->elem[idx]].control_flag == TDSTFC_REMOVED))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Found a TFC which is set to TDSTFC_REMOVED or allowed TFC number exceeded current combinations");
          return FAILURE;
        }
        idx++;
      }

      /* Allow all the combinations except the REMOVED ones
         & allow only the ones given by the network */

      i = 0;
      while(i < tdsordered_config_ptr->ul_tfcs.ntfc)
      {
        if(tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag != TDSTFC_REMOVED)
        {
          tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_ALLOWED;
        }
        i++;
      }
      idx =0;
      non_allowed_tfc_ptr = tfc_subset_ptr->u.non_allowedTFC_List;
      while(non_allowed_tfc_ptr->n > idx)
      {
        tdsordered_config_ptr->ul_tfcs.tfc_info[non_allowed_tfc_ptr->elem[idx]].control_flag = TDSTFC_NOT_ALLOWED;
        idx++;
      }
    }
    break;

  case T_tdsrrc_TFC_Subset_restrictedTrChInfoList:
    if(tfc_subset_ptr->u.restrictedTrChInfoList != NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Restricted transport channel choosen");
      restricted_trch_tfc_ptr = tfc_subset_ptr->u.restrictedTrChInfoList;    

      /* First verify whether the restricted transport channel type & 
         transport channel identity exists in OC. If not, return FAILURE */
     
      while(restricted_trch_tfc_ptr->n > idx)
      {
        if(restricted_trch_tfc_ptr->elem[idx].ul_TransportChannelType == tdsrrc_UL_TrCH_Type_dch)
        {
          transport_channel_id_present = FALSE;
          
          i = 0;
          while(i < TDSUE_MAX_TRCH)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"OC Info: Index=%d,TrChID=%d",tdsordered_config_ptr->ul_dch_trch_idx_in_use[i],tdsordered_config_ptr->ul_dch_trch_info[i].trch_id);
            if(restricted_trch_tfc_ptr->elem[idx].restrictedTrChIdentity == tdsordered_config_ptr->ul_dch_trch_info[i].trch_id)
            {
              if(tdsordered_config_ptr->ul_dch_trch_info[i].dch_present == TRUE)
              {
                transport_channel_id_present = TRUE;

                /* Record the transport channel IDs restricted in "tr_ch_ids" array */                  
                tr_ch_indx[no_of_tr_ch_ids] = tdsrrcllc_find_ul_dch_trch_idx(tdsordered_config_ptr, tdsordered_config_ptr->ul_dch_trch_info[i].trch_id);

                if(tr_ch_indx[no_of_tr_ch_ids] == TDSRRCLLC_INVALID_TRCH_IDX)
                {
                  TDSRRC_MSG0(MSG_LEGACY_ERROR,"Found invalid TrCh index .. rejecting");
                }

                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Index=%d, TDSDCH Index=%d, Restricting Transport Channel ID=%d", i, tdsordered_config_ptr->ul_dch_trch_idx_in_use[i], tr_ch_indx[no_of_tr_ch_ids]);
                
                if(restricted_trch_tfc_ptr->elem[idx].m.allowedTFI_ListPresent)
                {
                  uint32 idy =0;
                  /* Record the TFI's pointers allowed for the restricted transport channel ID */
                  temp_tfi_ptr = &restricted_trch_tfc_ptr->elem[idx].allowedTFI_List;
                  
                  while(temp_tfi_ptr->n > idy)
                  {
                    if(temp_tfi_ptr->elem[idx]>= TDSUE_MAX_TF)
                    {
                      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Found TFI value exceeding UE's capability for transport channel ID=%d.. rejecting",tr_ch_indx[no_of_tr_ch_ids]);
                      return FAILURE;
                    }
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSALLOWED TFI=%d",temp_tfi_ptr->elem[idy]);
                    idy++;
                  }
                }
                no_of_tr_ch_ids++;
                break;
              }
            }
            i++;
          }

          if(transport_channel_id_present == FALSE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Restricted transport channel Id not present ... rejecting");
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Restricted transport channel type non-dch choosen ... rejecting");
          return FAILURE;
        }

        idx++;
      }

      /* If we made this far, there is atleast one transport channel to apply restriction on. 
         Make all TFCs as NOT TDSALLOWED */
      /* j indicates the tfci index */
      for(j=0;j<tdsordered_config_ptr->ul_tfcs.ntfc && (j < TDSUE_MAX_TFC);j++)
      {
        boolean tfci_allowed = TRUE;
        /* k represents each Trch index */
        for(k=0;k< tdsordered_config_ptr->ul_tfcs.nchan && (k < TDSUE_MAX_TRCH); k++)
        {
          if(tdsrrcllc_check_if_tfi_present_in_allowed_list(restricted_trch_tfc_ptr,j,k)
               == FAILURE)
          {
            tfci_allowed = FALSE;
            break;
          }     
        }
          
        if(tdsordered_config_ptr->ul_tfcs.tfc_info[j].control_flag != TDSTFC_REMOVED)
        { 
          if(tfci_allowed == FALSE )
          {
            tdsordered_config_ptr->ul_tfcs.tfc_info[j].control_flag = TDSTFC_NOT_ALLOWED;  
          }
          else
          {
            tdsordered_config_ptr->ul_tfcs.tfc_info[j].control_flag = TDSTFC_ALLOWED;
          }
        }    
      }
    }
    break;

  case T_tdsrrc_TFC_Subset_fullTFCS:
    TDSRRC_MSG0(MSG_LEGACY_MED,"Full TFC subset restriction choosen");

    /* Relax all the restrictions if any.
       Allow all the combinations except the REMOVED ones */

    i = 0;
    while(i < tdsordered_config_ptr->ul_tfcs.ntfc)
    {
      if(tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag != TDSTFC_REMOVED)
      {
        tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_ALLOWED;
      }
      i++;
    }
    break;
  
  default:
    /* Ignoring this t */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupported .. rejecting");
    return FAILURE;
  }

  j = 0;
  while(j < tdsordered_config_ptr->ul_tfcs.ntfc)
  {
    if(tdsordered_config_ptr->ul_tfcs.tfc_info[j].control_flag == TDSTFC_ALLOWED)
    {
      TDSRRC_MSG2(MSG_LEGACY_MED,"Allowed TFCI=%d,CTFC=%d",j,tdsordered_config_ptr->ul_tfcs.tfc_info[j].ctfc);      
    }
    j++;
  }
  return SUCCESS;
} /* tdsrrcllc_update_oc_with_tfc_subset */


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
tdsrrcllc_update_oc_with_tfc_subset_list(void  *ie_ptr)
{
  uint32 i, j , k;
  uint32 no_of_subset_lists = 0;
  tdsrrcllc_oc_tfc_subset_list_type *ul_tfcsubsetlist_info_ptr = NULL;
  tdsrrc_AllowedTFC_List         *allowed_tfc_ptr = NULL;
  tdsrrc_Non_allowedTFC_List     *non_allowed_tfc_ptr = NULL;
  tdsrrc_RestrictedTrChInfoList  * restricted_trch_tfc_ptr = NULL;
  uint32  tr_ch_indx[TDSUE_MAX_TRCH] = {0};
  uint32  no_of_tr_ch_ids = 0;
  uint32 allowed_tfc_number = TDSUE_MAX_TFC + 1;  
  boolean transport_channel_id_present = FALSE;
  tdsrrc_AllowedTFI_List * temp_tfi_ptr;
  uecomdef_status_e_type status = FAILURE;
  uint32 idx =0;
  uint32 idy =0;
  uint32 idz =0;
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(((tdsrrc_UL_CommonTransChInfo_r4 *)ie_ptr),
      tfc_SubsetList))
  {
    if(!(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) )
    {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRC NV %d not REL5 capable so reject TFC subset list ",tdsrrc_nv_rel_indicator);
        return status;
    }
  }
  if((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&     
     (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(((tdsrrc_UL_CommonTransChInfo_r4 *)ie_ptr),
     tfc_SubsetList)))
  { 
    tdsrrc_TFC_SubsetList * tfc_subset_list_ptr = NULL;

    uint32 no_of_subset_list_index = 0;

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCSubset List received .. ");

    memset((void *)&tdsordered_config_ptr->ul_tfc_subset_list, 0, sizeof(tdsordered_config_ptr->ul_tfc_subset_list));
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC: TFCS list is Changed ");
  tdsordered_config_ptr->mac_ul_parms.new_tfcs_list_present = TRUE;
    tfc_subset_list_ptr = &((tdsrrc_UL_CommonTransChInfo_r4 *)ie_ptr)->tfc_SubsetList;

    ul_tfcsubsetlist_info_ptr = &tdsordered_config_ptr->ul_tfc_subset_list;


    /* All the information is basically stored as-is. NO checks performed.
       It is upto TFCC procedure (received on RLC-TM) to validate the data & use it */

    while(tfc_subset_list_ptr->n > idx)
    {
      if(TDSRRC_CHECK_COMMON_MSG_TYPE(tfc_subset_list_ptr->elem[idx].modeSpecificInfo,
           rrc_TransportFormatCombinationControl_modeSpecificInfo_tdd))
      {
        if(no_of_subset_list_index >= UE_MAX_TFC_SUBSET) 
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Found a TFC which is set to TDSTFC_REMOVED");
          return status;
        }

        /* Please refer tdsrrcllcoc.h for what each field of UL TFCSubsetList database [global var: ul_tfc_subset_list] means */
        switch(tfc_subset_list_ptr->elem[idx].tfc_Subset.t)
        {
        case T_tdsrrc_TFC_Subset_minimumAllowedTFC_Number:          

          if(tfc_subset_list_ptr->elem[idx].tfc_Subset.u.minimumAllowedTFC_Number < 
                        tdsordered_config_ptr->ul_tfcs.ntfc)
          {
            i = 0;
            while(i <= tfc_subset_list_ptr->elem[idx].tfc_Subset.u.minimumAllowedTFC_Number)
            {
              /* Not even a single TFC should have been removed earlier */
              if(tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag 
                                                                      == TDSTFC_REMOVED)
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"Found a TFC which is set to TDSTFC_REMOVED");
                return status;
              }
              i++;
            }
          }
          else
          { 
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Allowed TFC number=%d exceeds UE's capability=%d"
               "/current combinations=%d", allowed_tfc_number, 
               TDSUE_MAX_TFC, tdsordered_config_ptr->ul_tfcs.ntfc);
            return status;
          }

          ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].restriction_choice = TDSMINIMUM_ALLOWED_TFC;
          ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].tfc_choice_info.minimum_allowed_tfc_number
                = tfc_subset_list_ptr->elem[idx].tfc_Subset.u.minimumAllowedTFC_Number;
          break;

        case T_tdsrrc_TFC_Subset_allowedTFC_List:
          idy =0;
          allowed_tfc_ptr = tfc_subset_list_ptr->elem[idx].tfc_Subset.u.allowedTFC_List;

          while(allowed_tfc_ptr->n > idy)
          {
            if((allowed_tfc_ptr->elem[idy] >= tdsordered_config_ptr->ul_tfcs.ntfc) ||
               (tdsordered_config_ptr->ul_tfcs.tfc_info[allowed_tfc_ptr->elem[idy]].control_flag == TDSTFC_REMOVED))
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Found a TFC which is set to TDSTFC_REMOVED or allowed TFC number exceeded current combinations");
              return status;
            }
            idy++;
          }

          ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].restriction_choice = TDSALLOWED_TFC_LIST;
          allowed_tfc_ptr = tfc_subset_list_ptr->elem[idx].tfc_Subset.u.allowedTFC_List;

          i = 0;
          idy =0;
          while(allowed_tfc_ptr->n > idy)
          {
            if(i < TDSUE_MAX_TFC)
            {
              ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
                 tfc_choice_info.allowed_tfc_list.allowed_tfc_number[i++] = 
                (uint16)allowed_tfc_ptr->elem[idy];
            }

            ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
               tfc_choice_info.allowed_tfc_list.total_allowed_tfc_count++;

            idy++;
          }
          break;

        case T_tdsrrc_TFC_Subset_non_allowedTFC_List:
          idy =0;
          non_allowed_tfc_ptr = tfc_subset_list_ptr->elem[idx].tfc_Subset.u.non_allowedTFC_List;
          while(non_allowed_tfc_ptr->n > idy)
          {
            if((non_allowed_tfc_ptr->elem[idy] >= tdsordered_config_ptr->ul_tfcs.ntfc) ||
               (tdsordered_config_ptr->ul_tfcs.tfc_info[non_allowed_tfc_ptr->elem[idy]].control_flag == TDSTFC_REMOVED))
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Found a TFC which is set to TDSTFC_REMOVED or allowed TFC number exceeded current combinations");
              return status;
            }
            idy++;
          }
        
          ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].restriction_choice = TDSNON_ALLOWED_TFC_LIST;
          non_allowed_tfc_ptr = tfc_subset_list_ptr->elem[idx].tfc_Subset.u.non_allowedTFC_List;

          idy =0;
          i = 0;
          while(non_allowed_tfc_ptr->n > idy)
          {
            if(i < TDSUE_MAX_TFC)
            {
              ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
                  tfc_choice_info.non_allowed_tfc_list.non_allowed_tfc_number[i++]= 
                 (uint16)non_allowed_tfc_ptr->elem[idy];
            }

            ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
                   tfc_choice_info.non_allowed_tfc_list.total_non_allowed_tfc_count++;

            idy++;
          }
          break;

        case T_tdsrrc_TFC_Subset_restrictedTrChInfoList:
          idy =0;

          restricted_trch_tfc_ptr = tfc_subset_list_ptr->elem[idx].tfc_Subset.u.restrictedTrChInfoList;
          while(restricted_trch_tfc_ptr->n > idy)
          {
            transport_channel_id_present = FALSE;
            
            i = 0;
            while(i < TDSUE_MAX_TRCH)
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"OC Info: Index=%d,TrChID=%d",tdsordered_config_ptr->ul_dch_trch_idx_in_use[i],tdsordered_config_ptr->ul_dch_trch_info[i].trch_id);

              if(restricted_trch_tfc_ptr->elem[idy].restrictedTrChIdentity == tdsordered_config_ptr->ul_dch_trch_info[i].trch_id)
              {
                if(tdsordered_config_ptr->ul_dch_trch_info[i].dch_present == TRUE)
                {
                  transport_channel_id_present = TRUE;

                  if(tdsrrcllc_find_ul_dch_trch_idx(tdsordered_config_ptr, 
                           tdsordered_config_ptr->ul_dch_trch_info[i].trch_id) == TDSRRCLLC_INVALID_TRCH_IDX)
                  {
                    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Found invalid TrCh index .. rejecting");
                    return status;
                  }

                  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Index=%d, TDSDCH Index=%d, Restricting Transport Channel ID=%d", i, tdsordered_config_ptr->ul_dch_trch_idx_in_use[i], tr_ch_indx[no_of_tr_ch_ids]);
                  
                  if(restricted_trch_tfc_ptr->elem[idy].m.allowedTFI_ListPresent)
                  {
                    /* Record the TFI's pointers allowed for the restricted transport channel ID */
                    temp_tfi_ptr = &restricted_trch_tfc_ptr->elem[idy].allowedTFI_List;
                    idz =0;
                    while(temp_tfi_ptr->n > idz)
                    {
                      if(temp_tfi_ptr->elem[idz]>= TDSUE_MAX_TF)
                      {
                        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Found TFI value exceeding UE's capability for transport channel ID=%d.. rejecting",tr_ch_indx[no_of_tr_ch_ids]);
                        return status;
                      }
                      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSALLOWED TFI=%d",temp_tfi_ptr->elem[idz]);
                      idz++;
                    }
                  }
                  break;
                }
              }
              i++;
            }

            if(transport_channel_id_present == FALSE)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Restricted transport channel Id not present ... rejecting");
              return status;
            }
            idy++;
          }

          ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].restriction_choice = TDSRESTRICTED_TRCH_LIST;
          restricted_trch_tfc_ptr = tfc_subset_list_ptr->elem[idx].tfc_Subset.u.restrictedTrChInfoList;

          i = 0;
          idy =0;
          while(restricted_trch_tfc_ptr->n > idy)
          {
            if(i < TDSUE_MAX_TRCH)
            {
              if(restricted_trch_tfc_ptr->elem[idy].ul_TransportChannelType == tdsrrc_UL_TrCH_Type_dch)
              {
                ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
                    tfc_choice_info.restricted_trch_list.trch_info[i].dch_present = TRUE;
              }

              ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
                 tfc_choice_info.restricted_trch_list.trch_info[i].trch_id = 
                    restricted_trch_tfc_ptr->elem[idy].restrictedTrChIdentity;

              if(restricted_trch_tfc_ptr->elem[idy].m.allowedTFI_ListPresent)
              {
                tdsrrc_AllowedTFI_List * allowed_tfi_list_ptr = NULL;
                uint32              m;

                ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].tfc_choice_info.restricted_trch_list.trch_info[i].allowed_tfi_info_present = TRUE;

                allowed_tfi_list_ptr = &restricted_trch_tfc_ptr->elem[idy].allowedTFI_List;
                idz =0;

                m = 0;
                while(allowed_tfi_list_ptr->n > idz)
                {
                  if(m < TDSUE_MAX_TF)
                  {
                    ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
                          tfc_choice_info.restricted_trch_list.trch_info[i].allowed_tfi_info.tfi[m++] = 
                           (uint8)allowed_tfi_list_ptr->elem[idz];
                  }

                  ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
                    tfc_choice_info.restricted_trch_list.trch_info[i].allowed_tfi_info.no_of_allowed_tfis++;

                  idz++;
                }
              }

              ++i;
            }
            ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].tfc_choice_info.restricted_trch_list.no_of_transport_channels++;

            idy++;
          }
          break;

        case T_tdsrrc_TFC_Subset_fullTFCS:
          ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
          restriction_choice = TDSTFCS_FULL;
          break;

        default:
          ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_list_index].
          restriction_choice = TDSTFC_CHOICE_NOT_VALID;
          break;
        } /* end of tfcsubset switch */

         no_of_subset_list_index++;

        idx++;
      }
      else
      {
        /* In case one of the sequences in the middle TFC subset list has TDD option, we may end up
           with inconsistent TFCSubsetlist database. So trash the message */

        TDSRRC_MSG0(MSG_LEGACY_ERROR,"TFCSubset List received for non-FDD mode .. rejecting ");
        return status;
      }
    }

    ul_tfcsubsetlist_info_ptr = &tdsordered_config_ptr->ul_tfc_subset_list;  ul_tfcsubsetlist_info_ptr->no_of_subset_lists = no_of_subset_list_index;
    no_of_subset_lists = ul_tfcsubsetlist_info_ptr->no_of_subset_lists;
    while(no_of_subset_lists)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Restriction choice at index %d is: %d",(no_of_subset_lists-1),ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].restriction_choice);

      switch(ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].restriction_choice)
      {
      case TDSMINIMUM_ALLOWED_TFC:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Min. allowed:%d",ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.minimum_allowed_tfc_number);
        break;

      case TDSALLOWED_TFC_LIST:
        i = 0; 
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Allowed TFC list is:");
        while(i < ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.allowed_tfc_list.total_allowed_tfc_count)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"%d",ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.allowed_tfc_list.allowed_tfc_number[i]);
          i++;
        }
        break;

      case TDSNON_ALLOWED_TFC_LIST:
        i = 0; 
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Non-allowed TFC list is:");
        while(i < ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.non_allowed_tfc_list.total_non_allowed_tfc_count)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"%d",ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.non_allowed_tfc_list.non_allowed_tfc_number[i]);
          i++;
        }
        break;

      case TDSRESTRICTED_TRCH_LIST:
        i = ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.restricted_trch_list.no_of_transport_channels;
        if(i)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"No. of restricted transport channels are:%d",i);
          j = 0;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Restricted transport channel IDs are/is:");
          while((j < i) && (j < TDSUE_MAX_TRCH ))
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"%d",ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.restricted_trch_list.trch_info[j].trch_id);
            if(ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.restricted_trch_list.trch_info[j].allowed_tfi_info_present)
            {
              k = 0;

              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Allowed TFIs for this Tr Channel are:");

              while(k < ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.restricted_trch_list.trch_info[j].allowed_tfi_info.no_of_allowed_tfis)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"%d",ul_tfcsubsetlist_info_ptr->tfc_subset_info[no_of_subset_lists-1].tfc_choice_info.restricted_trch_list.trch_info[j].allowed_tfi_info.tfi[k]);
                k++;
              }
            }
            j++;
          }
        }
        break;

      case TDSTFCS_FULL:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"All restrictions will be lifted for this Subset Id");
        break;

      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid restriction type received");
      }

      no_of_subset_lists--;
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCSubset List stored for TFCC's[RLC-TM] use later");
  }
   
  return SUCCESS;
} /* tdsrrcllc_update_oc_with_tfc_subset_list */
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info2()

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
  tdsrrc_DL_AddReconfTransChInfoList_r4 * dl_trch_add_reconfig2_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList    * dl_trch_delete_ptr
)
{
  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;
  uint32 idx =0, idy=0;
  tdsrrc_DL_DeletedTransChInfoList *temp_dl_trch_delete_ptr = dl_trch_delete_ptr;
  tdsrrc_DL_AddReconfTransChInfoList_r4  *temp_dl_trch_add_reconfig_ptr = dl_trch_add_reconfig2_ptr;

  if((temp_dl_trch_delete_ptr != NULL) && (temp_dl_trch_add_reconfig_ptr != NULL))
  {
    while(temp_dl_trch_delete_ptr->n > idx)
    {
      if(tdsrrc_DL_TrCH_Type_dch == temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType)
      {
        idy = 0;
        while(temp_dl_trch_add_reconfig_ptr->n > idy)
        {
          if(tdsrrc_DL_TrCH_Type_dch == temp_dl_trch_add_reconfig_ptr->elem[idy].dl_TransportChannelType)
          {
            if(temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelIdentity == temp_dl_trch_add_reconfig_ptr->elem[idy].dl_transportChannelIdentity)
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Same Trch ID found in add/reconfig list %d and deleted list %d ",temp_dl_trch_add_reconfig_ptr->elem[idy].dl_transportChannelIdentity,temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelIdentity);
              return FAILURE;
            }
          }
          idy++;
       }
      }     
      idx++;
    }
  }
  idx = 0;
  while((dl_trch_delete_ptr !=NULL) && (dl_trch_delete_ptr->n > idx))
  {
    /* Remove this transport channel from MAC, L1 and local data.
     * and update transport channel count in L1, MAC and local data.
     */
    if(tdsrrcllc_get_dl_deleted_trch(&dl_trch_delete_ptr->elem[idx]) == FAILURE)
    {
      return FAILURE;
    }
    /* set tdsdl_tfcs_needed flag to true.  Later if common info is not preset,
     * UE needs to reject the message 
     */
    tdsdl_tfcs_needed = TRUE;
    /* get next TrCH Info */
    idx++;
  } /* end while */
  idx =0;
  /* Get the DL Added/Reconfigured Info */
  while((dl_trch_add_reconfig2_ptr!=NULL) && (dl_trch_add_reconfig2_ptr->n > idx) )
  {
    tdstrch_id = (tr_ch_id_type)dl_trch_add_reconfig2_ptr->elem[idx].dl_transportChannelIdentity;

    if((tdsrrcllc_get_dl_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't find idx,TrCH ID %d",tdstrch_id);
      return(FAILURE);
    }
    idx++;
  } /* while(...) */

  /* At this point, the transport channel list -
   *  - may have holes due to deleted channels.
   *  - transport channel info may not be sorted by TrCH IDs.
   */ 

  /* Fill holes and put all Transport channels in the first
   * tdsordered_config_ptr->dl_trch_cnt indices.
   */
  tdsrrcllc_fill_holes_in_dl_dch_trch_idx();

  /* Now is the time and place for sorting all transport channel
   * info based on TrCH IDs. Call the right function.
   */

  tdsrrcllc_sort_dl_dch_trch_index_array(&tdsordered_config_ptr->dl_dch_trch_info[0], 
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING)
                                      tdsordered_config_ptr->mac_dl_parms.num_dchs
#else
                                      (TDSRRCLLC_DOWNLINK_TRCH_COUNT(tdsordered_config_ptr))
#endif 
                                      );
  return(SUCCESS);
} /* tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info2_r5 */

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
  tdsrrc_DL_AddReconfTransChInfoList_r5  * dl_trch_add_reconfig_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList_r5    * dl_trch_delete_ptr
)
{
  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;

  uint8 dflow_index;
  tdsrrc_MAC_hs_AddReconfQueue_List* dflow_queue_head_ptr = NULL;
  uint32 idx =0,idy =0;
  tdsrrc_DL_DeletedTransChInfoList_r5 *temp_dl_trch_delete_ptr = dl_trch_delete_ptr;
  tdsrrc_DL_AddReconfTransChInfoList_r5  *temp_dl_trch_add_reconfig_ptr = dl_trch_add_reconfig_ptr;

  if((temp_dl_trch_delete_ptr != NULL) && (temp_dl_trch_add_reconfig_ptr != NULL))
  {
    while(temp_dl_trch_delete_ptr->n > idx)
    {
      if(T_tdsrrc_DL_TrCH_TypeId2_r5_dch == temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.t)
      {
        idy = 0;
        while(temp_dl_trch_add_reconfig_ptr->n > idy)
        {
          if(T_tdsrrc_DL_TrCH_TypeId1_r5_dch == temp_dl_trch_add_reconfig_ptr->elem[idy].dl_TransportChannelType.t)
          {
            if(temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.u.dch == temp_dl_trch_add_reconfig_ptr->elem[idy].dl_TransportChannelType.u.dch)
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Same Trch ID found in add/reconfig list %d and deleted list %d ",temp_dl_trch_add_reconfig_ptr->elem[idy].dl_TransportChannelType.u.dch,temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.u.dch);
              return FAILURE;
            }
          }
          idy++;
       }
      }     
      idx++;
    }
  }
  idx = 0;
  idy = 0;
  while((dl_trch_delete_ptr != NULL) && (dl_trch_delete_ptr->n > idx))
  {
  /* Remove this transport channel from MAC, L1 and local data.
  * and update transport channel count in L1, MAC and local data.
    */
    if(tdsrrcllc_get_dl_deleted_trch_r5(&dl_trch_delete_ptr->elem[idx]) == FAILURE)
    {
      return FAILURE;
    }
    /* get next TrCH Info */
    idx++;
  } /* end while */
  idx =0;
  /* Get the DL Added/Reconfigured Info */
  while((dl_trch_add_reconfig_ptr !=NULL) && (dl_trch_add_reconfig_ptr->n > idx) )
  {
    /* Check if channel type is HSDPA */
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(dl_trch_add_reconfig_ptr->elem[idx].dl_TransportChannelType, 
      rrc_DL_TrCH_TypeId1_r5_hsdsch))
    {
      /* At least one HARQ/DFLOW has to be present */
      if ((dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.t 
        == T_tdsrrc_DL_AddReconfTransChInformation_r5_tfs_SignallingMode_hsdsch)
        && ((dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->m.addOrReconfMAC_dFlowPresent)
        || (dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->m.harqInfoPresent)))
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Valid DFLOW specified");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:HARQ,DFLOW both ABSent ");
        return FAILURE;
      }
      
      /* Now find the DFLOW Index for ADD/RECONFIG.  */
      if ((dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.m.
             mac_hs_AddReconfQueue_ListPresent) &&
        (dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.mac_hs_AddReconfQueue_List.n > 0))
      {
        dflow_queue_head_ptr = 
          &dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->
          addOrReconfMAC_dFlow.mac_hs_AddReconfQueue_List;
        idy=0;
        while (dflow_queue_head_ptr->n  > idy)
        {
          if ((dflow_queue_head_ptr->elem[idy].mac_dFlowId > UE_MAX_MAC_D_FLOW-1) ||
              (dflow_queue_head_ptr->elem[idy].mac_hsQueueId > UE_MAX_HS_QUEUE-1))
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCHS:DFLOW Id %d, Queue Id %d,Range 0..7",dflow_queue_head_ptr->elem[idy].mac_dFlowId,dflow_queue_head_ptr->elem[idy].mac_hsQueueId);
            return FAILURE;
          }

          /* If DFLOW Index found then return it Else allocate a new index */
          if ((dflow_index = tdsrrcllc_get_dflow_index(
            (uint8)dflow_queue_head_ptr->elem[idy].mac_dFlowId)) 
            == UE_MAX_MAC_D_FLOW)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:DFLOW index %d absent",dflow_index);
            return FAILURE;
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:DFlow Index is %d",dflow_index); 
            idy++;
          }
        } /* End of while */       
        
      } /* End of if of add queue present */
    } /* End of if of rrc_DL_TrCH_TypeId1_r5_hsdsch chosen */
    else if (dl_trch_add_reconfig_ptr->elem[idx].dl_TransportChannelType.t == 
      T_tdsrrc_DL_TrCH_TypeId1_r5_dch)
    {
      tdstrch_id = (tr_ch_id_type)dl_trch_add_reconfig_ptr->elem[idx].dl_TransportChannelType.u.dch;
      
      if((tdsrrcllc_get_dl_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't find idx,TrCH ID %d",tdstrch_id);
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSCH unsupported");
      return FAILURE;
    }
    idx++;
  } /* while(...) */
  
    /* At this point, the transport channel list -
     *  - may have holes due to deleted channels.
     *  - transport channel info may not be sorted by TrCH IDs.
     */ 
  
  /* Fill holes and put all Transport channels in the first
   * tdsordered_config_ptr->dl_trch_cnt indices.
   */

  /* Now Remove holes in DFLOW Index */
  tdsrrcllc_rearrange_dflow_index();
  
  tdsrrcllc_fill_holes_in_dl_dch_trch_idx();
  
  /* Now is the time and place for sorting all transport channel
  * info based on TrCH IDs. Call the right function.
  */

  tdsrrcllc_sort_dl_dch_trch_index_array(&tdsordered_config_ptr->dl_dch_trch_info[0], 
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING)
                                     tdsordered_config_ptr->mac_dl_parms.num_dchs 
#else
                                     (TDSRRCLLC_DOWNLINK_TRCH_COUNT(tdsordered_config_ptr)) 
#endif 
                                      );
  
  
  return(SUCCESS);
} /* tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r5 */


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
)
{
  /* Set HARQ Information */
  tdsrrc_HARQ_Info_explicit_* harq_head_ptr = NULL;
  uint32 idx =0;
  uint8 cnt = 0;

#ifdef FEATURE_TDSCDMA_MULTICARRIER_PRE_VERSION
  tdsordered_config_ptr->harq_present = TRUE;
#endif

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Set HARQ info in OC");
  
  tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.num_proc = (uint8)harq_info_ptr->numberOfProcesses;
  
  TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:No of processes %d",harq_info_ptr->numberOfProcesses); 
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(harq_info_ptr->memoryPartitioning,
    rrc_HARQ_Info_memoryPartitioning_implicit))
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_part_info_valid = FALSE;
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Implicit Mem Partition"); 
  }
  else
  {
    tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_part_info_valid = TRUE;
    
    TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:Explicit Mem Partition"); 
    
    if ((harq_head_ptr = harq_info_ptr->memoryPartitioning.u.explicit_) == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:HARQ Mem Part Ptr NULL");
      return FAILURE;
    }
#ifdef FEATURE_TDSCDMA_MIMO
    while ((harq_head_ptr->n > idx) && (cnt < TDSL1_DL_CCTRCHCFG_HS_DSCH_HARQ_MAX_PROC_NO_MIMO))
#else
    while ((harq_head_ptr->n > idx) && (cnt < TDSL1_DL_CCTRCHCFG_HS_DSCH_HARQ_MAX_PROC))
#endif
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.harq_info.mem_size_type[cnt++] = 
        (tdsl1_dl_hs_dsch_harq_mem_sz_enum_type) harq_head_ptr->elem[idx];
      
      idx++;
      
    }
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:No of HARQ mem sizes %d",cnt);
  }
  
  return SUCCESS;

}


/*============================================================================
FUNCTION: tdsrrcllc_delete_queue_from_dflow()

DESCRIPTION:
  This function deletes the queue from a DFLOW. Once the queue has been
  deleted, a hole has been created which would be filled later by another
  function
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllc_delete_queue_from_dflow
(
  uint32 deleted_queue_id
)
{
  uint32 cnt = 0, queue_cnt = 0;

  boolean deleted_queue_found = FALSE;

  for (cnt = 0; cnt < tdsordered_config_ptr->mac_dl_parms.num_dflow; cnt++)
  {
    if ((tdsordered_config_ptr->dl_dflow_index_in_use[cnt] == TRUE) && 
      (tdsordered_config_ptr->mac_dflow_info[cnt].mac_dflow_id != TDSINVALID_DFLOW_ID))
    {
      /* Check if it has queue to be deleted */
      for (queue_cnt = 0; queue_cnt < tdsordered_config_ptr->mac_dflow_info[cnt].no_of_queues; queue_cnt++)
      {
        if (tdsordered_config_ptr->mac_dflow_info[cnt].queue_info[queue_cnt].queue_id == deleted_queue_id)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:Queue %d del from Dflow %d",deleted_queue_id,tdsordered_config_ptr->mac_dflow_info[cnt].mac_dflow_id);

          tdsordered_config_ptr->mac_dflow_info[cnt].queue_info[queue_cnt].queue_id = TDSINVALID_QUEUE_ID;
          
          tdsordered_config_ptr->mac_dflow_info[cnt].queue_info[queue_cnt].no_of_pdu_sizes = 0;

           TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:DFLOW %d Orig Queue cnt %d",tdsordered_config_ptr->mac_dflow_info[cnt].mac_dflow_id,tdsordered_config_ptr->mac_dflow_info[cnt].no_of_queues);
          
          /* Update the queue count */
          tdsordered_config_ptr->mac_dflow_info[cnt].no_of_queues--;

          TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:DFLOW %d Orig Queue cnt %d",tdsordered_config_ptr->mac_dflow_info[cnt].mac_dflow_id,tdsordered_config_ptr->mac_dflow_info[cnt].no_of_queues);

          deleted_queue_found = TRUE;
        
          break;
        }
      } /* End of for of no_of_queues */

      if (TDSRRC_MAC_DFLOW_QUEUEID_INVAILD != 
            (tdsrrcllc_semi_permanent_data.mac_dflow_info[cnt].add_queues_bitmask & (1 << deleted_queue_id)))
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"queue_id %d have been set in DFLOW %d",deleted_queue_id,tdsordered_config_ptr->mac_dflow_info[cnt].mac_dflow_id);
        deleted_queue_found = TRUE;
      }
    } /* End of if of dflow found */
    if (deleted_queue_found == TRUE)
    {
      return SUCCESS;
    }
  } /* End of for of no of dflow */
  
  TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:No deleted queue %d found",deleted_queue_id);
  return FAILURE;
}


/*====================================================================
FUNCTION: tdsrrcllc_get_hs_window_size()

DESCRIPTION:
  
DEPENDENCIES:
  Gets the window size associated with the queue.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  
====================================================================*/
static uint8 tdsrrcllc_get_hs_window_size
(
  tdsrrc_MAC_hs_WindowSize mac_hsWindowSize
)
{
  uint8 w_size;

  switch (mac_hsWindowSize)
  {
    case tdsrrc_MAC_hs_WindowSize_mws4 : 
      w_size = 4;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_mws6 :
      w_size = 6;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_mws8:
      w_size = 8;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_mws12:
      w_size = 12;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_mws16:
      w_size = 16;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_mws24:
      w_size = 24;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_mws32:
      w_size = 32;
      break;
      
    default: /* Not Possible */
      w_size = 0;
      break;
      
  }
  TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:WindowSize %d",w_size);
  return w_size;
}

/*====================================================================
FUNCTION: tdsrrcllc_get_hs_release_timer()

DESCRIPTION:
  
DEPENDENCIES:
  Gets the Release Timer associated with the queue.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  
====================================================================*/
static uint16 tdsrrcllc_get_hs_release_timer
(
  tdsrrc_T1_ReleaseTimer reorderingReleaseTimer
)
{
  uint16 release_timer;

  switch (reorderingReleaseTimer)
  {
    case tdsrrc_T1_ReleaseTimer_rt10 : 
      release_timer = 10;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt20 :
      release_timer = 20;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt30:
      release_timer = 30;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt40:
      release_timer = 40;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt50:
      release_timer = 50;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt60:
      release_timer = 60;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt70:
      release_timer = 70;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt80:
      release_timer = 80;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt90:
      release_timer = 90;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt100:
      release_timer = 100;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt120:
      release_timer = 120;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt140:
      release_timer = 140;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt160:
      release_timer = 160;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt200:
      release_timer = 200;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt300:
      release_timer = 300;
      break;
      
    case tdsrrc_T1_ReleaseTimer_rt400:
      release_timer = 400;
      break;    
      
    default: /* Not Possible */
      release_timer = 0;
      break;
      
  }
  TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:ReleaseTimer %d",release_timer);
  return release_timer;
}

/*====================================================================
FUNCTION: tdsrrcllc_set_queue_info()

DESCRIPTION:
  
DEPENDENCIES:
  Sets the Release Timer, PDU Size, Release Timer, Window Size associated with this queue
  in MAC structure. The index passed is the new DFLOW index. The mapping between old DFLOW 
  index and this queue is removed.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_set_queue_info
(
  uint8  dflow_index,
  uint32 queue_index,
  tdsrrc_MAC_hs_AddReconfQueue* rrc_queue_info_ptr,
  boolean queue_already_present
)
{
  /* Flow Id and Queue Id has already been stored */
  
  tdsrrc_MAC_d_PDU_SizeInfo_List* pdu_list_ptr = NULL;
  
  uint8 pdu_size_count = 0, cnt = 0;
  uint32 idx =0;
  boolean pdu_index_present = FALSE;
  
  /* Check if the queue is already present. In this case don't look at queue index */
  if (queue_already_present)
  {
    TDSRRC_MSG2(MSG_LEGACY_LOW,"RRCHS:DFLOWId %d,Queue %d exists already",rrc_queue_info_ptr->mac_dFlowId,rrc_queue_info_ptr->mac_hsQueueId);
    
    /* Check if PDU SIZE mapping provided */
    if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_queue_info_ptr,mac_d_PDU_SizeInfo_List)) && 
      (rrc_queue_info_ptr->mac_d_PDU_SizeInfo_List.n >0))
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Mapping provided");
      pdu_list_ptr = &rrc_queue_info_ptr->mac_d_PDU_SizeInfo_List;
      
      while ((pdu_list_ptr->n >idx) && (pdu_size_count < UE_MAX_HS_NO_OF_PDU_SIZE_PER_QUEUE)) 
      {
        /* Check if pdu size index exists. If yes, overwrite the pdu size in that index else store size in new index */
        for (pdu_size_count=0; pdu_size_count < 
          tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes;
        pdu_size_count++)
        {
          if (tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].pdu_info[pdu_size_count].mac_pdu_index
            == pdu_list_ptr->elem[idx].mac_d_PDU_Index)
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:PDU Index %d existing in queue %d",pdu_list_ptr->elem[idx].mac_d_PDU_Index,rrc_queue_info_ptr->mac_hsQueueId);
            
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCHS:Overwrite PDUSize %d with %d,PduIdx %d", 
              tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].
              pdu_info[pdu_size_count].mac_pdu_size, 
              pdu_list_ptr->elem[idx].mac_d_PDU_Size, pdu_list_ptr->elem[idx].mac_d_PDU_Index);
            
            tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].pdu_info[pdu_size_count].mac_pdu_size = 
              (uint16) pdu_list_ptr->elem[idx].mac_d_PDU_Size;
            
            pdu_index_present = TRUE;
          }
        }
        
        /* Now check if pdu index has already been ovewritten */
        if (pdu_index_present == FALSE)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:PDU Index %d abs in PDUSIZELIST. Add new %d",pdu_list_ptr->elem[idx].mac_d_PDU_Index);

          if (tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes < 
              (UE_MAX_HS_NO_OF_PDU_SIZE_PER_QUEUE-1))
          {
            cnt = tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes;

            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:PDU size space available");

            TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:Set PDU index %d,Size %d",pdu_list_ptr->elem[idx].mac_d_PDU_Index,pdu_list_ptr->elem[idx].mac_d_PDU_Size);

            tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].pdu_info[cnt].mac_pdu_index = 
              (uint8) pdu_list_ptr->elem[idx].mac_d_PDU_Index;
            
            tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].pdu_info[cnt].mac_pdu_size = 
              (uint16) pdu_list_ptr->elem[idx].mac_d_PDU_Size;
            
            tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes++;
            
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:No of PDU Sizes %d",tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes); 
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:No of PDU sizes %d (MAX 8)",tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes);
            return FAILURE;
          }
        }
        else
        {
          /* Set index to FALSE forcibly */
          pdu_index_present = FALSE;
        }
        
        /* The PDU size has already been written */
        /* Get the next pdu size */
        idx++;
      } /* End of while */
    } /* End of if of PDU SIZE Info present */
    else
    {
      /* No mapping provided. Use the old one */
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Queue,No PDU Mapping provided.Use Curr");
      
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Use No. of existing PDU Sizes i.e %d",tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes);
    }

    /* Set Window Size & Release Timer */
    tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].window_size = 
    
     tdsrrcllc_get_hs_window_size(rrc_queue_info_ptr->mac_hsWindowSize);

    tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].release_timer = 
    
     tdsrrcllc_get_hs_release_timer(rrc_queue_info_ptr->reorderingReleaseTimer);

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"rrchs:WindowSize %d,RelTimer %d ms",tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].window_size,tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].release_timer);

  } /* End of if of queue already present */
  else
  {
    /* Queue not present. Queue Index has already been allocated. Set PDU info, Release Timer etc */
    TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:DFLOWId %d,New Queue %d",rrc_queue_info_ptr->mac_dFlowId,rrc_queue_info_ptr->mac_hsQueueId);
    
    pdu_list_ptr = &rrc_queue_info_ptr->mac_d_PDU_SizeInfo_List;
    
    while ((pdu_list_ptr->n > idx) && (pdu_size_count < UE_MAX_HS_NO_OF_PDU_SIZE_PER_QUEUE)) 
    {
      TDSRRC_MSG3(MSG_LEGACY_MED,"RRCHS:PDUIndex %d, Size %d set in Queue %d", 
        pdu_list_ptr->elem[idx].mac_d_PDU_Index, pdu_list_ptr->elem[idx].mac_d_PDU_Size,rrc_queue_info_ptr->mac_hsQueueId);
      
      tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].pdu_info[pdu_size_count].mac_pdu_size = 
        (uint16) pdu_list_ptr->elem[idx].mac_d_PDU_Size;
      
      tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].pdu_info[pdu_size_count].mac_pdu_index = 
        (uint8) pdu_list_ptr->elem[idx].mac_d_PDU_Index;
      
      pdu_size_count++;
      
      idx++;
    } /* End of while of pdu_list_ptr */
    
    tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes = pdu_size_count;
    
    tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].window_size = 

     tdsrrcllc_get_hs_window_size(rrc_queue_info_ptr->mac_hsWindowSize);
    tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].release_timer = 

     tdsrrcllc_get_hs_release_timer(rrc_queue_info_ptr->reorderingReleaseTimer);
    TDSRRC_MSG3(MSG_LEGACY_MED,"RRCHS:No PduSizes %d,Queue %d,No of queues are %d",
            pdu_size_count,rrc_queue_info_ptr->mac_hsQueueId, tdsordered_config_ptr->mac_dflow_info[dflow_index].no_of_queues);
    
  } /* End of else of queue not present */
  
  return SUCCESS;
} /* tdsrrcllc_set_queue_info */
/*============================================================================
FUNCTION: tdsrrcllc_set_dflow_info()

DESCRIPTION:
  This function sets the DFLOW information information in MAC structure in OC
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllc_set_dflow_info(tdsrrc_AddOrReconfMAC_dFlow * rrc_hsdsch)
{
    uint8 dflow_index;
  
  uint8 queue_index;
  
  boolean queue_present = FALSE;
  uint32 idx =0;
  tdsrrc_MAC_hs_AddReconfQueue_List* queue_dflow_head_ptr = NULL;
  
  tdsrrc_MAC_hs_DelQueue_List * deleted_queue_list_ptr = NULL;
  
  /* Get the deleted Queue Information */
  if ( (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_hsdsch,
    mac_hs_DelQueue_List)) &&
    (rrc_hsdsch->mac_hs_DelQueue_List.n > 0))
  {
    
    deleted_queue_list_ptr = &rrc_hsdsch->mac_hs_DelQueue_List;
    
    while (deleted_queue_list_ptr->n > idx)
    {
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Delete Queue Present");

      /* Check which queue is being deleted and find dflow associated with it */
      if (tdsrrcllc_delete_queue_from_dflow(deleted_queue_list_ptr->elem[idx].mac_hsQueueId) == SUCCESS)
      {
        idx++;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Delete queue %d not found",deleted_queue_list_ptr->elem[idx].mac_hsQueueId);
        /* CR414921:NW send message: delete mac-hs dflow in IE "dl_DeletedTransChaInfoList",and delete mac-hsQueueId mapping on 
                 mac-hs dflow in IE "dl-AddReconfTransChInfoList". Before we cannot find the mac-hs queue, then return failure to NW.
                 Now need to process with this message */
        //return FAILURE;
        idx++;
      }
    }
    /* Now rearrange queues again so as to fill out the holes */
    tdsrrcllc_rearrange_queue_index();
  }

  /* Get the add/reconfig MAC D FLOW Info */
  if ( (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_hsdsch,mac_hs_AddReconfQueue_List)) &&
      (rrc_hsdsch->mac_hs_AddReconfQueue_List.n > 0))
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Add/Reconfig Queue/DFLOW Present");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Add/Reconfig Queue/DFLOW absent");
    return SUCCESS;
  }
  /* Parse the Queue associated to a DFLOW */
  queue_dflow_head_ptr = &rrc_hsdsch->mac_hs_AddReconfQueue_List;
  idx=0;
  /* DFLOW ID is already present in MAC structure & RRC structure */
  while (queue_dflow_head_ptr->n > idx)
  {
    /* Get the dflow index which has already been created */
    if ((dflow_index = tdsrrcllc_find_dflow_index((uint8)queue_dflow_head_ptr->elem[idx].mac_dFlowId)) == UE_MAX_MAC_D_FLOW)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Index not found for flow Id %d",queue_dflow_head_ptr->elem[idx].mac_dFlowId);
      return FAILURE;
    }
    if(tdsrrcllc_restore_macd_rb_mapping_info(dflow_index) ==FAILURE)
    {
      /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Index not correct for flow Id %d in Standalone Rb-mapping backup",queue_dflow_head_ptr->elem[idx].mac_dFlowId);
    }

    /* Now get the queue index as this queue may already be available. If not available, then store
    allocate a new queue index and store the index in it. Just store this queue and dflow id */

    if ((queue_index = tdsrrcllc_get_queue_index(dflow_index, queue_dflow_head_ptr->elem[idx].mac_hsQueueId, &queue_present)) == UE_MAX_HS_QUEUE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Unable to process Queue with ID %d",queue_dflow_head_ptr->elem[idx].mac_hsQueueId);
      return FAILURE;
    }
    
    /* Set queue information in queue index */
    if (tdsrrcllc_set_queue_info(dflow_index, queue_index, &queue_dflow_head_ptr->elem[idx], queue_present) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid queue parameters");
      return FAILURE;
    }

    /* set bitmask to flag that mac-hsQueueId has been configured. Queue id is (0~7) */
    TDSRRC_MSG2(MSG_LEGACY_MED,"queue_id %d set in DFLOW %d",queue_dflow_head_ptr->elem[idx].mac_hsQueueId,tdsordered_config_ptr->mac_dflow_info[dflow_index].mac_dflow_id);
    tdsrrcllc_semi_permanent_data.mac_dflow_info[dflow_index].add_queues_bitmask |= 
        1 << (queue_dflow_head_ptr->elem[idx].mac_hsQueueId);

    /* At this time, the queue and dflow have already been stored */
    /* Now check if this queue is available in some other dflow. If yes, then that old mapping association
      needs to be removed */

    tdsrrcllc_remove_dflow_queue_mapping(&queue_dflow_head_ptr->elem[idx]);
    /* Get the next queue */
    idx++;
    
  }
  return SUCCESS;
}
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
/*====================================================================
FUNCTION: tdsrrcllc_add_macehs()

DESCRIPTION:
  
DEPENDENCIES:


RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_add_macehs
(
 tdsrrc_AddOrReconfMAC_ehs_ReordQ * mac_ehs_info
)
{
  tdsrrc_MAC_ehs_AddReconfReordQ_List * macehs_queue_head_ptr;
  uint8 macehs_q_id;
  uint32 idx =0;
    /* Now find the MAC EHS Index for ADD/RECONFIG.  */
  if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mac_ehs_info,mac_ehs_AddReconfQueue_List))
     &&(mac_ehs_info->mac_ehs_AddReconfQueue_List.n > 0))
  {
    macehs_queue_head_ptr = &mac_ehs_info->mac_ehs_AddReconfQueue_List;
    while (macehs_queue_head_ptr->n > idx)
    {
      if (macehs_queue_head_ptr->elem[idx].mac_ehs_QueueId > UE_MAX_HS_QUEUE-1)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"MACEHS:MAC EHS Q Id %d,Range 0..7",macehs_queue_head_ptr->elem[idx].mac_ehs_QueueId);
        return FAILURE;
      }

      /* If Queue Index found then return it Else allocate a new index */
      if ((macehs_q_id = tdsrrcllc_get_macehs_queue_index((uint8)macehs_queue_head_ptr->elem[idx].mac_ehs_QueueId)) 
        == UE_MAX_HS_QUEUE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"MACEHS: MAC EHS Q index %d absent",macehs_q_id);
        return FAILURE;
      }
      else
      {
      
      /* End of while of pdu_list_ptr */            
      
      tdsordered_config_ptr->mac_ehs_queue_info[macehs_q_id].window_size = 
        tdsrrcllc_get_hs_window_size(macehs_queue_head_ptr->elem[idx].mac_ehsWindowSize);
      
      tdsordered_config_ptr->mac_ehs_queue_info[macehs_q_id].release_timer = 
        tdsrrcllc_get_hs_release_timer(macehs_queue_head_ptr->elem[idx].reorderingReleaseTimer);

         /*PSR TBD  reorderingResetTimer   in 7.6       */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"MACEHS: MAC EHS Q index is %d",macehs_q_id); 
        idx++;
      }
    } /* End of while */       
    
  }
  return SUCCESS;
}

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
)
{
  uint32 i;
  boolean machs_present = FALSE;
  boolean mac_ehs_present = FALSE;
  
  for(i=0;i< tdsordered_config_ptr->mac_dl_parms.num_dflow ;i++)
  {
       /* Check that there is at least one MAC-D FLow */
    if ((tdsordered_config_ptr->mac_dflow_info[i].mac_dflow_id != TDSINVALID_DFLOW_ID) &&
        (tdsordered_config_ptr->mac_dflow_info[i].ndlchan != 0) &&
        (tdsordered_config_ptr->mac_dflow_info[i].no_of_queues != 0)&&
        (tdsordered_config_ptr->dl_dflow_index_in_use[i] == TRUE ))
    {
       TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHSPA+: MAC-HS  present DFLOW id %d mapped to rb_id %d",tdsordered_config_ptr->mac_dflow_info[i].mac_dflow_id,tdsordered_config_ptr->mac_dflow_info[i].dlchan_info[0].rb_id);

       machs_present = TRUE;
       tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs = MAC_HS;
       tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.mac_hs_or_ehs = MAC_HS;
       break;
    }
  }
  
  for(i=0;i< tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue;i++)
  {
    /* Check that there is at least one MAC-D FLow */
    if ((tdsordered_config_ptr->dl_macehs_logchan_info[i].queue_id != TDSINVALID_QUEUE_ID) &&
      (tdsordered_config_ptr->mac_ehs_lc_id_in_use[i] == TRUE ))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHSPA+: MAC-EHS present queue id %d mapped to lc_id %d",tdsordered_config_ptr->dl_macehs_logchan_info[i].queue_id,tdsordered_config_ptr->dl_macehs_logchan_info[i].mac_id);
       
      tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs = MAC_EHS;
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.mac_hs_or_ehs = MAC_EHS;
      mac_ehs_present = TRUE;
      break;
    }
  }
  
  if(mac_ehs_present && machs_present )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: Both MAC HS and MAC-Ehs co-exist");
     return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/*============================================================================
FUNCTION: tdsrrcllc_delete_queue_from_macehs()

DESCRIPTION:
  This function deletes the MACEHS queue . Once the queue has been
  deleted, a hole has been created which would be filled later by another
  function
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllc_delete_queue_from_macehs
(
  uint32 deleted_queue_id
)
{
  uint32 cnt = 0;

  for (cnt = 0; cnt < UE_MAX_HS_QUEUE; cnt++)
  {
    if ((tdsordered_config_ptr->mac_ehs_queue_index_in_use[cnt] == TRUE) && 
      (tdsordered_config_ptr->mac_ehs_queue_info[cnt].queue_id != TDSINVALID_QUEUE_ID))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"EFACH:Searching for qid %d ",deleted_queue_id);
      if (tdsordered_config_ptr->mac_ehs_queue_info[cnt].queue_id == deleted_queue_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHSPA+:Queue %d del from ",deleted_queue_id);
  
        tdsordered_config_ptr->mac_ehs_queue_info[cnt].queue_id  = TDSINVALID_QUEUE_ID;
  
        tdsordered_config_ptr->mac_ehs_queue_index_in_use[cnt] = FALSE;
        /* Update the queue count */
        tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue--;
  
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:MACEHS Orig Queue cnt %d",tdsordered_config_ptr->mac_dl_parms.num_mac_ehs_queue);
  
        return SUCCESS;
      }
    } /* End of if of dflow found */
  } /* End of for of no of dflow */
  
  TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:No deleted queue %d found",deleted_queue_id);
  return FAILURE;
}
/*============================================================================
FUNCTION: set_macehs_q_info()

DESCRIPTION:
  
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllc_macehs_add_or_del_q_info
( 
  tdsrrc_AddOrReconfMAC_ehs_ReordQ * mac_ehs_info
)
{  

  
  /* Get the deleted Queue Information */
  if ( TDSRRC_MSG_COMMON_BITMASK_IE_PTR(mac_ehs_info,
    tdsrrc_AddOrReconfMAC_ehs_ReordQ,dummy))
      {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"AddOrReconfMAC_ehs_ReordQ_dummy_present present");
    /* Now rearrange queues again so as to fill out the holes */
  }

  /* Get the add/reconfig MAC D FLOW Info */
  if ( (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mac_ehs_info,
     mac_ehs_AddReconfQueue_List)) &&
      (mac_ehs_info->mac_ehs_AddReconfQueue_List.n > 0))
  {
    if(tdsrrcllc_add_macehs(mac_ehs_info) == FAILURE)
    {
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Add/Reconfig Queue absent");
    return SUCCESS;
  }
    
  return SUCCESS;
}

#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/




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
)
{
  /* local transport channel variables */
  tr_ch_id_type trch_idx = 0;
  tr_ch_id_type ul_trch_idx = 0;
  
  /* Pointers to UL and DL TFS info for use when copying existing config */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info = NULL;
  tdsl1_ul_semi_static_parms_struct_type *ul_tfs_ss_info = NULL;
  tdsmac_ul_tfs_type *ul_tfs_dyn_info = NULL;
  uint32 idx =0,idy=0;
  /* Loop variables for copying UL TFS info */
  /*lint -e578 */
  uint8 tf_idx = 0;
  tdsl1_dl_cctrchcfg_tf_info_struct_type *rrc_dl_tf_info = NULL;
  tdsmac_ul_tf_type *ul_tf_info = NULL;
  /*lint +e578 */
  
  /* Boolean used to indicate if multiple logical channels are mapped to
  a given TrCH. */
  boolean       multiple_ded_log_chan = FALSE;
  
  /* local variable indicating whether or not an uplink TrCH ID was found */
  boolean       got_one = FALSE;
  
  tdsrrc_MAC_hs_AddReconfQueue_List* add_queue_list_ptr = NULL;
  tdsrrc_MAC_hs_DelQueue_List* delete_queue_list_ptr = NULL;
  
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Add/Reconf TrCH");
  
  /*
  * Use the Transport Channel Index and next state to place the TFS in 
  * the correct slot in the MAC TFS array.
  */
  if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
  {
  /* Get the transport channel index for this transport channel ID.
    * Note: The index should already be there. */
    
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_hsdsch))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HS TrCh to be added");
    }
    else
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_dch))
      {
        
        trch_idx = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch);
        
        if(trch_idx >= TDSUE_MAX_TRCH)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,no idx found",ie_ptr->dl_TransportChannelType.u.dch);
          return(FAILURE);
        }

        if(tdsrrcllc_restore_dl_dch_rb_mapping_info(trch_idx) == FAILURE)
        {
          /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,not correct in Standalone RB-Mapping",ie_ptr->dl_TransportChannelType.u.dch);
        }

      }
      
      /* Determine if multiple dedicated logical channels are present on this TrCH id */
      if(tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt > 1)
      {
        multiple_ded_log_chan = TRUE;
      }
    }
    /*
    * Get whether or not the TFS for a given TRCH is included or if it needs
    * to come from an uplink TrCH ID.
    */
    switch(ie_ptr->tfs_SignallingMode.t)
    {
      /* TFS explicitly given for this Transport Channel */
      case T_tdsrrc_DL_AddReconfTransChInformation_r5_tfs_SignallingMode_explicit_config:
        
        /* get the Transport Format Set for this TrCH */
        if(tdsrrcllc_get_dl_transport_format_set(ie_ptr->tfs_SignallingMode.u.explicit_config,
          &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
          multiple_ded_log_chan, /* mulitple logical channels? */
          &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
          ) == FAILURE)
        {
          return FAILURE;
        }
        break; /* end explicit TFS signalling for DL TrCH */
        
      case T_tdsrrc_DL_AddReconfTransChInformation_r5_tfs_SignallingMode_sameAsULTrCH:
        if (ul_ptr == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"invalid:UL TFS absent when DL TFS choice 'same as UL'");
          return(FAILURE);        
        }
        while(ul_ptr->n > idx)
        {
          if(ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
            ul_TransportChannelIdentity == ul_ptr->elem[idx].transportChannelIdentity)
          { 
            /* Got a match with Uplink TrCH ID */
            TDSRRC_MSG2(MSG_LEGACY_LOW,"Get TFS for DL TrCH Id %d using UL TrCH ID %d",ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity,ul_ptr->elem[idx].transportChannelIdentity);
            if(tdsrrcllc_get_dl_transport_format_set(&ul_ptr->elem[idx].transportFormatSet,
              &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
              multiple_ded_log_chan, /* mulitple logical channels? */
              &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
              ) == FAILURE)
            {
              return FAILURE;
            }
            
            /* Indicate that found an UL TrCH ID, and break the while loop */
            got_one = TRUE;
            break;
          } /* end UL TrCH match */
          
          /* no match, keep looking */
          else 
          {
            idx++;
          }
        } /* end while */
        
        /* Make sure we found the UL TrCH, before continuing */
        if(!got_one)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No matching UL TFS in msg,look up from OC");
          /* Get the transport channel index for the UL transport channel ID.
          * Note: The index should already be there.
          */
          ul_trch_idx = tdsrrcllc_find_ul_dch_trch_idx(
            tdsordered_config_ptr,
            (tr_ch_id_type)ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
            ul_TransportChannelIdentity);
          if(ul_trch_idx >= TDSUE_MAX_TRCH)
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"No DL TFS for TrCH %d, to use UL TrCH %d for TFS",
              ie_ptr->dl_TransportChannelType.u.dch,
              ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
              ul_TransportChannelIdentity, 0);
            return(FAILURE);
          }
          
          /* Set up convenience pointers */
          dl_tfs_info = &tdsordered_config_ptr->dl_trch_info[trch_idx];
          ul_tfs_ss_info = &tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[ul_trch_idx];
          ul_tfs_dyn_info = &tdsordered_config_ptr->ul_tfcs.tfs_info[ul_trch_idx];
          
          /* Copy semistatic info from uplink */
          switch(ul_tfs_ss_info->coding)
          {
          case TDSL1_UNCODED:
            dl_tfs_info->coding_type = TDSL1_DL_UNCODED;
            break;
          case TDSL1_CONV_1_BY_2_CODING:
            dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_2_CODING;
            break;
          case TDSL1_CONV_1_BY_3_CODING:
            dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_3_CODING;
            break;
          case TDSL1_TURBO_CODING:
            dl_tfs_info->coding_type = TDSL1_DL_TURBO_CODING;
            break;
          default:
            break; 
          }
          
          
          dl_tfs_info->rate_match_attrib = ul_tfs_ss_info->staticRM;
          switch(ul_tfs_ss_info->crc)
          {
          case TDSL1_CRC_LENGTH_0:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_0;
            break;
          case TDSL1_CRC_LENGTH_8:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_8;
            break;
          case TDSL1_CRC_LENGTH_12:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_12;
            break;
          case TDSL1_CRC_LENGTH_16:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_16;
            break;
          case TDSL1_CRC_LENGTH_24:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_24;
            break;
          default:
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CRC length %d in UL",ul_tfs_ss_info->crc);
            return FAILURE;
          }
          
          /* Copy dynamic info from uplink */
          dl_tfs_info->tti_value = ul_tfs_dyn_info->tti_info;
          dl_tfs_info->num_tf = (uint8) ul_tfs_dyn_info->ntf;
          for(tf_idx = 0; tf_idx < dl_tfs_info->num_tf; tf_idx++)
          {
            rrc_dl_tf_info = dl_tfs_info->tf_info_array[tf_idx];
            ul_tf_info = &ul_tfs_dyn_info->tf_info[tf_idx];
            
            rrc_dl_tf_info->num_tb = (uint8)ul_tf_info->ntblks;
            rrc_dl_tf_info->tb_size = ul_tf_info->tb_size;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"TB Size: %d,multiple_ded_log_chan: %d",rrc_dl_tf_info->tb_size,multiple_ded_log_chan);

            /* If number of logical channel is > 1, inc tb_size by 4 to accomodate MAC-header */
            if ( (rrc_dl_tf_info->tb_size > 0)&&(multiple_ded_log_chan == TRUE) )
            {
              rrc_dl_tf_info->tb_size += TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"New TB Size %d",rrc_dl_tf_info->tb_size);
            }
          }
        }
        break; /* end use of previous signalled UL TrCH TFS for DL TrCH */ 
        
    case T_tdsrrc_DL_AddReconfTransChInformation_r5_tfs_SignallingMode_hsdsch:
      /* The DFLOW Index has already been added. So get the index */
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSDSCH chosen");
      if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr->tfs_SignallingMode.u.hsdsch,
        addOrReconfMAC_dFlow))
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:DFLOW present for Add/Reconfig Dflow");
        
      
      
      /* Check if add & delete queue both are present and network specifies the same 
      queue id in both. This needs to be rejected */
      if ((ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.m.mac_hs_AddReconfQueue_ListPresent) &&
        (ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.mac_hs_AddReconfQueue_List.n > 0) &&
        (ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.m.mac_hs_DelQueue_ListPresent) &&
        (ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.mac_hs_DelQueue_List.n >0))
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Add&Delete queue present");
        /* Check if UTRAN messed up by specifying the same queue id */
        delete_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.mac_hs_DelQueue_List;
        idx =0;
        while (delete_queue_list_ptr->n > idx)
        {
          add_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.mac_hs_AddReconfQueue_List;
          idy=0;
          while (add_queue_list_ptr->n > idy)
          {
            if (delete_queue_list_ptr->elem[idx].mac_hsQueueId == 
                add_queue_list_ptr->elem[idy].mac_hsQueueId)
            {
              /* Invalid Config */
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Queue %d simul added & deleted",delete_queue_list_ptr->elem[idx].mac_hsQueueId);
              return FAILURE;
            }
            idy++;
          }
          
          idx++;
        }
        
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Queue check passed");
      }

      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->tfs_SignallingMode.u.hsdsch,
        rrc,addOrReconfMAC_dFlow) )
      {
        if (tdsrrcllc_set_dflow_info(&ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid DFLOW info");
          return FAILURE;
        }
      }
      }
      
      /* SET HARQ info in L1 structure */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->tfs_SignallingMode.u.hsdsch,
        tdsrrc_HSDSCH_Info,harqInfo))
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:HARQ info present");
        if (tdsrrcllc_set_harq_information(&ie_ptr->tfs_SignallingMode.u.hsdsch->harqInfo) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set HARQ information");
          return FAILURE;
        }
      }
      
      /* Indicate that HSDSCH Info is present which would be used later to set bit mask when HSDPA
      is already ACTIVE */
      tdshsdpa_msg_params.hsdsch_info_present = TRUE;
      
      tdshsdpa_msg_params.msg_params_present = TRUE;
      
      break;
    default:
      return FAILURE;
      
    } /* end TFS selection switch */

    /*Find Rbs mapped to this DCH and fill them in rb_list_mapped_to_dch if their RLC
    PDU size has changed.*/ 
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_dch))
    {
      tdsrrcllc_get_rb_for_rb_list_mapped_to_dch((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch, 
                tdsordered_config_ptr->rrc_state_indicator, TDSDL);  
    }       
  } /* end CELL_DCH check */
  
  /* States not supported */
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_FACH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_PCH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"URA_PCH not supported for TrCH");
    return FAILURE;
  }
  else
  {
    ERR_FATAL("Unknown state not supported for TrCH",0,0,0);
  }
  
  /***************************************************************/
  /* Transport Channel Count has been updated in RB Mapping info */
  /***************************************************************/

  if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->tfs_SignallingMode,
     rrc_DL_AddReconfTransChInformation_r5_tfs_SignallingMode_hsdsch)) )
  {
    /* Check for the BLER */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
        tdsrrc_DL_AddReconfTransChInformation_r5,dch_QualityTarget))
    {
      /* Save the BLER if found, and is in range */
      if((ie_ptr->dch_QualityTarget.bler_QualityValue >= TDSRRCLLC_MIN_BLER) &&
         (ie_ptr->dch_QualityTarget.bler_QualityValue <= TDSRRCLLC_MAX_BLER))
      {
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val =
          (int8)ie_ptr->dch_QualityTarget.bler_QualityValue;
      }
    
    /* BLER is out of range */
      else
      {
      /* According to L1 I/F, must set an invalid value of -127 */
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL BLER %d is out of range",ie_ptr->dch_QualityTarget.bler_QualityValue);
        return FAILURE;
      }
    } /* end if BLER present */
    else
    {
     /* IE omitted. Set the target to -127 which would result 1% of BLER target.
      * This is good UE behaviour though the spec is ambiguous on what UE
      * behavior should be in this case.  
      */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DCH quality target not present. Setting to -127");
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
    } 
  }
  else
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
        tdsrrc_DL_AddReconfTransChInformation_r5,dch_QualityTarget))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring dch quality target for HS");
    }
  }
  
  
  /* Indicate that DL L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  
  return SUCCESS;
} /* end tdsrrcllc_get_dl_added_reconfig_trch_r5() */


/*================================================================================================
FUNCTION: tdsrrcllc_copy_pdu_info_per_queue()

DESCRIPTION:
  This function copies the PDU Information associated with a queue from MAC OC structure
  to L1 structure

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
  
===============================================================================================*/
static void tdsrrcllc_copy_pdu_info_per_queue(uint8 dflow_index, uint8 queue_index)
{
  uint8 index = 0;
  
  uint8 pdu_size_index = 0;
  /* Get the first available index in L1 structure and put the queue info there */
  
  for (index = 0; index < TDSL1_DL_CCTRCHCFG_MAC_HS_Q_MAX_COUNT; index++)
  {
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.
      mac_hs_q_info[index].mac_hs_q_id == TDSINVALID_QUEUE_ID)
    {
      /* Got the place to store index. First set Queue Id */
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.mac_hs_q_info[index].mac_hs_q_id = 
        tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].queue_id;
      
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.mac_hs_q_info[index].num_mac_d_pdu_sz = 
        tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes;
      
      /* Increment no of queues in L1 */
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.num_mac_hs_q++;
      
      
      TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:L1 queues after increment %d",tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.num_mac_hs_q);
      
      for (pdu_size_index = 0; pdu_size_index < 
        tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].no_of_pdu_sizes; pdu_size_index++)
      {
        /* PDU SIZE INDEX */
        tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.
          mac_hs_q_info[index].mac_d_pdu_sz_info[pdu_size_index].pdu_sz_idx = 
          tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].
          pdu_info[pdu_size_index].mac_pdu_index;
        
        /* PDU SIZE */
        tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.
          mac_hs_q_info[index].mac_d_pdu_sz_info[pdu_size_index].pdu_sz = 
          tdsordered_config_ptr->mac_dflow_info[dflow_index].queue_info[queue_index].
          pdu_info[pdu_size_index].mac_pdu_size;
      }
      
      break;
    }
  }
}


/*================================================================================================
FUNCTION: tdsrrcllc_check_if_queue_exists()

DESCRIPTION:
  This function checks if the passed queue id is already present in L1 structure.  

DEPENDENCIES:
  None.

RETURN VALUE:
  True if queue found else False

SIDE EFFECTS:
  
===============================================================================================*/
static boolean tdsrrcllc_check_if_queue_exists(uint8 queue_id)
{
  uint8 index = 0;
  for (index = 0; index < UE_MAX_HS_QUEUE; index++)
  {
    if (tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.mac_hs_q_info[index].mac_hs_q_id == queue_id)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Queue %d already present. Don't update");
      return TRUE;
    }
  }
  return FALSE;
}

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
)
{
  uint8 cnt = 0;
  uint8 queue_cnt = 0;
  
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  if ((tdsordered_config_ptr->mac_dl_parms.num_dflow == 0)
    && (tdsordered_config_ptr->mac_dl_parms.ndlchan_macehs == 0)
    )
#else
  if (tdsordered_config_ptr->mac_dl_parms.num_dflow == 0)
#endif
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No of Dflow and MAC EHS queue id are 0");
    return SUCCESS;
  }
  /* Initialize the queue in L1 structure */
  tdsrrcllc_init_hsdpa_l1_queue(tdsordered_config_ptr);
  
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  if(tdsordered_config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_EHS )
  {
    for(cnt = 0; cnt < tdsordered_config_ptr->mac_dl_parms.ndlchan_macehs; cnt++)
    {
      tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.mac_ehs_dlchan_q_info[tdsordered_config_ptr->dl_macehs_logchan_info[cnt].mac_id] 
            = (uint8)tdsordered_config_ptr->dl_macehs_logchan_info[cnt].queue_id;

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: Queue ID %d mapped to LC id %d",tdsordered_config_ptr->dl_macehs_logchan_info[cnt].queue_id,tdsordered_config_ptr->dl_macehs_logchan_info[cnt].mac_id);
    }
  }
  else 
#endif
  {
  for (cnt = 0; cnt < tdsordered_config_ptr->mac_dl_parms.num_dflow && (cnt < UE_MAX_MAC_D_FLOW); cnt++)
  {
    if ((tdsordered_config_ptr->dl_dflow_index_in_use[cnt] == TRUE) && 
      (tdsordered_config_ptr->mac_dflow_info[cnt].mac_dflow_id != TDSINVALID_DFLOW_ID))
    {
      /* Got the index and dflow id */
      /* Get the queue id */
      
      for (queue_cnt = 0; queue_cnt < tdsordered_config_ptr->mac_dflow_info[cnt].no_of_queues 
        && (queue_cnt < UE_MAX_HS_QUEUE); queue_cnt++)
      {
        if (tdsordered_config_ptr->mac_dflow_info[cnt].queue_info[queue_cnt].queue_id != TDSINVALID_QUEUE_ID)
        {
          /* Preventive check : If queue is already existing in L1 structure. */
          if (tdsrrcllc_check_if_queue_exists(tdsordered_config_ptr->mac_dflow_info[cnt].queue_info[queue_cnt].
            queue_id) == TRUE)
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCHS:Queue %d already Exists",tdsordered_config_ptr->mac_dflow_info[cnt].queue_info[queue_cnt].
              queue_id,0,0);
            continue;
          }
          /* Call a function to store this queue value in L1 structure */
          tdsrrcllc_copy_pdu_info_per_queue(cnt, queue_cnt);
          
          /* Preventive check */
          if (tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.macd_info.num_mac_hs_q == 
            TDSL1_DL_CCTRCHCFG_MAC_HS_Q_MAX_COUNT)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:L1 queue max reached. Break");
            break;
          }
        }
      }
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCHS:Serious Error. DFLOW Id %d DFLOW Index %d ",tdsordered_config_ptr->mac_dflow_info[cnt].mac_dflow_id,tdsordered_config_ptr->dl_dflow_index_in_use[cnt]);

      return FAILURE;
    }
  }
  }
  return SUCCESS;
}


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
)
{

  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;
  uint32 idx =0,idy=0;
  tdsrrc_UL_DeletedTransChInfoList_r6 *temp_ul_trch_delete_ptr = ul_deleted_trch_list_ptr;
  tdsrrc_UL_AddReconfTransChInfoList_r6 *temp_ul_trch_add_reconfig_ptr = ul_trch_add_reconfig_ptr;

  if((temp_ul_trch_delete_ptr != NULL) && (temp_ul_trch_add_reconfig_ptr != NULL))
  {
    while(temp_ul_trch_delete_ptr->n > idx)
    {
      if(T_tdsrrc_UL_TransportChannelIdentity_r6_dch_usch == temp_ul_trch_delete_ptr->elem[idx].t)
      {
        idy = 0;
        while(temp_ul_trch_add_reconfig_ptr->n > idy)
        {
          if(T_tdsrrc_UL_AddReconfTransChInformation_r6_dch_usch == temp_ul_trch_add_reconfig_ptr->elem[idy].t)
          {
            if(temp_ul_trch_delete_ptr->elem[idx].u.dch_usch->ul_TransportChannelIdentity == temp_ul_trch_add_reconfig_ptr->elem[idy].u.dch_usch->transportChannelIdentity)
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Same Trch ID found in add/reconfig list %d and deleted list %d ",temp_ul_trch_add_reconfig_ptr->elem[idy].u.dch_usch->transportChannelIdentity,temp_ul_trch_delete_ptr->elem[idx].u.dch_usch->ul_TransportChannelIdentity);
              return FAILURE;
            }
          }
          idy++;
       }
      }     
      idx++;
    }
  }
  idx = 0;
  idy = 0;
  // KW error
  if ((ul_deleted_trch_info_list_present)&&(ul_deleted_trch_list_ptr != NULL))
  {
    while (ul_deleted_trch_list_ptr->n >idx)
    {
      /* Remove this transport channel from MAC, L1 and local data.
       * and update transport channel count in L1, MAC and local data.
       */
      if(tdsrrcllc_get_ul_deleted_trch_info_list_r6(&ul_deleted_trch_list_ptr->elem[idx]) == FAILURE)
      {
        return FAILURE;
      }
      /* get next TrCH Info */
      idx++;
    } /* end while */
    
#ifdef FEATURE_TDSCDMA_HSUPA
        idx = 0;
        /*Fill the holes only when Ul EDCH Trch is deleted*/
        while(ul_deleted_trch_list_ptr->n > idx) 
        {
          if (T_tdsrrc_UL_TransportChannelIdentity_r6_e_dch == ul_deleted_trch_list_ptr->elem[idx].t)
          {
            tdsrrcllc_fill_holes_in_e_mac_d_flow_idx();
            break;
          }
          idx++;
        }
#endif
  }
  idx =0;
  // KW error
  if ((ul_add_recfg_trch_info_list_present)&&(ul_trch_add_reconfig_ptr != NULL))
  {
    /* Get the UL Added/Reconfigured Info */
    while(ul_trch_add_reconfig_ptr->n >idx ) 
    {
      if (T_tdsrrc_UL_AddReconfTransChInformation_r6_dch_usch ==
          ul_trch_add_reconfig_ptr->elem[idx].t)
      {
        tdstrch_id = (tr_ch_id_type)
                  ul_trch_add_reconfig_ptr->elem[idx].u.dch_usch->transportChannelIdentity;
  
        if((tdsrrcllc_get_ul_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't find idx,TrCH ID %d",tdstrch_id);
          return(FAILURE);
        }
      }
#ifdef FEATURE_TDSCDMA_HSUPA
      else if ((T_tdsrrc_UL_AddReconfTransChInformation_r6_e_dch == ul_trch_add_reconfig_ptr->elem[idx].t) && 
               (ul_trch_add_reconfig_ptr->elem[idx].u.e_dch->m.addReconf_MAC_d_FlowListPresent)) 
      {
        tdsrrc_E_DCH_AddReconf_MAC_d_FlowList * e_dch_mac_d_flow_list_ptr = NULL;

        e_dch_mac_d_flow_list_ptr = &ul_trch_add_reconfig_ptr->elem[idx].u.e_dch->addReconf_MAC_d_FlowList;
        idy =0;
        while (e_dch_mac_d_flow_list_ptr->n > idy)
        {
          if((tdsrrcllc_get_e_mac_dflow_idx((uint8)e_dch_mac_d_flow_list_ptr->elem[idy].
              mac_d_FlowIdentity)) == TDSMAX_E_DCH_MAC_D_FLOW)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: Couldn't find idx for e-dch MAC-D flow: %d",e_dch_mac_d_flow_list_ptr->elem[idy].mac_d_FlowIdentity);
            return(FAILURE);
          }

          idy++;
        }

        tdsrrcllc_update_mac_eul_action(TRUE);
      }
#else
      else /* tdsrrc_UL_AddReconfTransChInformation_r6_e_dch */
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: R6 UL Trch Add Reconfig for E-DCH not yet supported");
      }
#endif /* FEATURE_TDSCDMA_HSUPA*/
      idx++;
    } /* while(...) */
  }

  /* At this point, the transport channel list -
   *  - may have holes due to deleted channels.
   *  - transport channel info may not be sorted by TrCH IDs.
   */ 

  /* Fill holes and put all Transport channels in the first
   * tdsordered_config_ptr->ul_trch_cnt indices.
   */
  tdsrrcllc_fill_holes_in_ul_dch_trch_idx();

#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrcllc_fill_holes_in_e_mac_d_flow_idx();
#endif /* FEATURE_TDSCDMA_HSUPA */

  /* Now is the time and place for sorting all transport channel
   * info based on TrCH IDs. Call the right function.
   */
  tdsrrcllc_sort_ul_dch_trch_index_array(&tdsordered_config_ptr->ul_dch_trch_info[0], 
                                      (TDSRRCLLC_UPLINK_TRCH_COUNT(tdsordered_config_ptr)));
  return(SUCCESS);
}


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
)
{
  if (ul_common_trch_info_present)
  {
    if(tdsrrcllc_get_ul_info_common_for_all_trch_r5(ie_ptr,
      tdsordered_config_ptr->rrc_state_indicator)
      == FAILURE)
    {
      return FAILURE;
    } 
  }
  return(SUCCESS);
}

#ifdef FEATURE_TDSCDMA_HSUPA

/*================================================================================================
FUNCTION: tdsrrcllc_process_added_reconfig_e_dch_info()

DESCRIPTION:
  This function takes a pointer to the EDCH Added or Reconfigured 
  mac-d flow info and copies it to TDSORDERED_CONFIG.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:

===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_process_added_reconfig_e_dch_info
(
  tdsrrc_UL_AddReconfTransChInformation_r6 * ie_ptr
)
{

  /* local transport channel variables */
  uint8 e_mac_d_flow_idx = TDSMAX_E_DCH_MAC_D_FLOW;
  tdsrrc_E_DCH_AddReconf_MAC_d_FlowList * e_mac_d_flow_list_ptr = NULL;
  uint32 idx =0;
  if ( !(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_AddReconfTransChInformation_r6_e_dch)))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Un-wanted call to function to process e_dch_info. choice: %d",ie_ptr->t);
    return(FAILURE);
  }

  /*We store harq_rv_config which are trch i.e in L1 data base,
  *hence init them when previously EUL was not active
  */
  if(tdscurrent_config_ptr->e_dch_transmission == FALSE)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL:init in l1 db,prv harq_rv_config=%d",tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config);
    tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_NOT_PRESENT;
  }

  /* Update the harq info if present */
  if (ie_ptr->u.e_dch->harq_Info == tdsrrc_UL_AddReconfTransChInformation_r6_harq_Info_rv0)
  {
    tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_0;
  }
  else
  {
    /* Default value is 'rvtable', in which case the UE shall use an RSN
       based RV index as specified in 25.212*/
    tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_TABLE;
  }

  if (tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config !=
      tdscurrent_config_ptr->l1_e_info.e_ul_info.harq_rv_config) 
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: Setting UL-L1 action cuz Harq Info RECONFIG");

    tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_HARQ_RV_INFO_INCL;
    tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);
  }
  
  /* Update Add-Recong mac-d flow info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->u.e_dch,
    tdsrrc_UL_AddReconfTransChInformation_r6_e_dch,addReconf_MAC_d_FlowList))
  {       
    e_mac_d_flow_list_ptr = &ie_ptr->u.e_dch->addReconf_MAC_d_FlowList;
  }

  /* Update Add-Recong mac-d flow info */
  while ((e_mac_d_flow_list_ptr != NULL) && (e_mac_d_flow_list_ptr->n > idx))
  {       
    if ((e_mac_d_flow_idx = tdsrrcllc_find_e_mac_d_flow_idx(
           (uint8)e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowIdentity)) == TDSMAX_E_DCH_MAC_D_FLOW)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: E-DCH mac-d flow ID %d,no idx found",e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowIdentity);
      return FAILURE;
    }
    else
    {
      if(tdsrrcllc_restore_ul_emac_rb_mapping_info(e_mac_d_flow_idx) == FAILURE)
      {
        /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: E-DCH mac-d flow ID %d,not correct info found in Stanalone rb-mapping info",e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowIdentity);
      }
      tdsrrcllc_update_mac_eul_action(TRUE);
      /* Only allowed to be absent when already defined for this E-DCH MAC-d flow*/
      if (TDSRRC_MSG_COMMON_BITMASK_IE(e_mac_d_flow_list_ptr->elem[idx],
         tdsrrc_E_DCH_AddReconf_MAC_d_Flow,mac_d_FlowPowerOffset))
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
          e_mac_d_flow_power_offset = e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowPowerOffset;
      }
      else
      {
        if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
            e_mac_d_flow_power_offset > TDSRRCLLC_MAX_E_MAC_D_FLOW_PWR_OFFSET)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d flow pwr offset needs to be provided");
          return FAILURE;
        }
      }
      /* Only allowed to be absent when already defined for this E-DCH MAC-d flow */
      if (TDSRRC_MSG_COMMON_BITMASK_IE(e_mac_d_flow_list_ptr->elem[idx],
        tdsrrc_E_DCH_AddReconf_MAC_d_Flow,mac_d_FlowMaxRetrans))
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
          e_mac_d_flow_max_num_of_retx = e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowMaxRetrans;
      }
      else
      {
        if(tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
           e_mac_d_flow_max_num_of_retx > TDSRRCLLC_MAX_E_MAC_D_FLOW_NUM_RETX)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d max num of retx needs to be provided");
          return FAILURE;
        }
      }
      
      /* if no mux list is provided, use the previous one.  If there is no previous then allow multiplexing of 
         MAC-d PDU's from the E-DCH MAC-d flow indicated in the IE "E-DCH MAC-d flow identity" with MAC-d PDU's 
         from any other E-DCH MAC-d flow in the same MAC-e PDU.*/
      if (TDSRRC_MSG_COMMON_BITMASK_IE(e_mac_d_flow_list_ptr->elem[idx],
        tdsrrc_E_DCH_AddReconf_MAC_d_Flow,mac_d_FlowMultiplexingList))
      {
        /* Only look at first byte of bitstring as max e_mac_d flow is 8*/
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_mac_d_flow_mux_list =
          tdsrrcllcpcie_convert_asn1_oss_bitstring_to_uint8(
            &e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowMultiplexingList);
      }
      else
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_mac_d_flow_mux_list = 0xFF;
      }
      
      if (TDSRRC_MSG_COMMON_BITMASK_IE(e_mac_d_flow_list_ptr->elem[idx],
        tdsrrc_E_DCH_AddReconf_MAC_d_Flow,transmissionGrantType))
      {
        if (e_mac_d_flow_list_ptr->elem[idx].transmissionGrantType.t == 
            T_tdsrrc_E_DCH_AddReconf_MAC_d_Flow_transmissionGrantType_non_ScheduledTransGrantInfo)
        {
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
            e_tx_grant_info = TDSMAC_E_TX_GRANT_NON_SCHEDULED;
        }
        else /* rrc_scheduledTransmissionGrantInfo */
        {
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_tx_grant_info = TDSMAC_E_TX_GRANT_SCHEDULED;
        }
      }
    }

    idx++;
  }

  return SUCCESS;
}

#endif /* FEATURE_TDSCDMA_HSUPA */


/*================================================================================================
FUNCTION: tdsrrcllc_get_ul_added_reconfig_trch()

DESCRIPTION:
  This function takes a pointer to the Uplink Added or Reconfigured 
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:
  CELL_DCH additions are the only item supported now.
===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_added_reconfig_trch_r6
(
  tdsrrc_UL_AddReconfTransChInformation_r6 * ie_ptr
)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL Add/Reconf TrCH");

  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_AddReconfTransChInformation_r6_dch_usch))
  {

    if(tdsrrcllc_dch_usch_add_trch_info((tr_ch_id_type)ie_ptr->u.dch_usch->transportChannelIdentity,
                              &ie_ptr->u.dch_usch->transportFormatSet) == FAILURE)
      {
        return FAILURE;
      }
  }
#ifdef FEATURE_TDSCDMA_HSUPA
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_AddReconfTransChInformation_r6_e_dch)) 
  {
    if (FAILURE == tdsrrcllc_process_added_reconfig_e_dch_info(ie_ptr))
    {
      return FAILURE;
    }
  }
#else
  else /* tdsrrc_UL_AddReconfTransChInformation_r6_e_dch */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: tdsrrc_UL_AddReconfTransChInformation_r6_e_dch not yet supported");
  }
#endif /* FEATURE_TDSCDMA_HSUPA */
  return SUCCESS;
} /* end tdsrrcllc_get_ul_added_reconfig_trch_r6() */

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
)
{
  uint8 idex;
  if(ul_add_recfg_trch_info_list_present)
  {
    uint32 idx = 0;
    
    /* Save info for each additional Transport Channel */
    while((idx <= TDSUE_MAX_TRCH) && (ul_trch_add_reconfig_ptr->n > idx))
    {
      if(tdsrrcllc_get_ul_added_reconfig_trch_r6(&ul_trch_add_reconfig_ptr->elem[idx]) == FAILURE)
      {
        return FAILURE;
      }
      
      /* get next TrCH Info */
      idx++;

    } /* end while */
  } /* end UL TrCH Add/Reconfig */

  /* Adjust MAC header  on every UL TrCH if necessary. */ 
  for (idex=0; idex<TDSUE_MAX_TRCH; idex++)
  {
  /* The number of logical channel mapped to this transport channel 
  increases from 1 to 2. Needs to adjsut the tb_szie to
    reflect the adding of the MAC header.                         */
    if (tdsordered_config_ptr->ul_dch_trch_info[idex].dch_present &&
      (tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up ||
      tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down))
    {
      if (!(ul_add_recfg_trch_info_list_present &&
         tdsrrcllc_find_trch_id_in_ul_trch_list_r6(ul_trch_add_reconfig_ptr,
        tdsordered_config_ptr->ul_dch_trch_info[idex].trch_id)))
      {
        /* Added MAC head size to the current TB size.  */
        if (tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up &&
          !tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Muti logic channel map to one transport channel ");
          tdsordered_config_ptr->ul_tfcs.tfs_info[idex].mac_header_size =
             TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
        }
        /* Substract MAC head size from the current TB size.  */
        else if (!tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up &&
          tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"One logic channel map to one transport channel");
          tdsordered_config_ptr->ul_tfcs.tfs_info[idex].mac_header_size = 
               TDSRRCLLC_DED_SINGLE_LOG_CHL_DCH_MAC_HDR_SIZE;
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clean TB size adjust flag in RBSetup");
      /* Reset lg_ch_cnt_up and lg_ch_cnt_down. */
      tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up = FALSE;
      tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down = FALSE;
    }
    else if (!tdsordered_config_ptr->ul_dch_trch_info[idex].dch_present)
    {
    /* Since the activated transport channels are sorted before stored in
      the memory, break the looping after reaching the 1st invalid block.  */
      break;
    }
  }
  
  /* Check to make sure UL TFS count is within UE Capabilities */
  if(tdsrrcllc_check_ul_tfs_cnt() == FAILURE)
  {
    return FAILURE;
  }

  return(SUCCESS);
}


/*================================================================================================
FUNCTION: tdsrrcllc_get_dl_added_reconfig_trch_r6()

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
static uecomdef_status_e_type tdsrrcllc_get_dl_added_reconfig_trch_r6
(
  tdsrrc_DL_AddReconfTransChInformation_r5 * ie_ptr,
  tdsrrc_UL_AddReconfTransChInfoList_r6 * ul_ptr
)
{
  /* local transport channel variables */
  tr_ch_id_type trch_idx = 0;
  tr_ch_id_type ul_trch_idx = 0;
  uint32 idx=0,idy=0;
  /* Pointers to UL and DL TFS info for use when copying existing config */
  /*lint -e578 */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info = NULL;
  tdsl1_ul_semi_static_parms_struct_type *ul_tfs_ss_info = NULL;
  tdsmac_ul_tfs_type *ul_tfs_dyn_info = NULL;
  
  /* Loop variables for copying UL TFS info */
  uint8 tf_idx = 0;
  tdsl1_dl_cctrchcfg_tf_info_struct_type *rrc_dl_tf_info = NULL;
  tdsmac_ul_tf_type *ul_tf_info = NULL;
  /*lint +e578 */
  
  /* Boolean used to indicate if multiple logical channels are mapped to
  a given TrCH. */
  boolean       multiple_ded_log_chan = FALSE;
  
  /* local variable indicating whether or not an uplink TrCH ID was found */
  boolean       got_one = FALSE;
  
  tdsrrc_MAC_hs_AddReconfQueue_List * add_queue_list_ptr = NULL;
  
  tdsrrc_MAC_hs_DelQueue_List * delete_queue_list_ptr = NULL;
  
  
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Add/Reconf TrCH");
  
  /*
  * Use the Transport Channel Index and next state to place the TFS in 
  * the correct slot in the MAC TFS array.
  */
  if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
  {
  /* Get the transport channel index for this transport channel ID.
    * Note: The index should already be there. */
    
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_hsdsch))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HS TrCh to be added");
    }
    else
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_dch))
      {
        
        trch_idx = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch);
        
        if(trch_idx >= TDSUE_MAX_TRCH)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,no idx found",ie_ptr->dl_TransportChannelType.u.dch);
          return(FAILURE);
        }

        if(tdsrrcllc_restore_dl_dch_rb_mapping_info(trch_idx) == FAILURE)
        {
          /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,not correct in Standalone RB-Mapping",ie_ptr->dl_TransportChannelType.u.dch);
        } 

      }
      
      /* Determine if multiple dedicated logical channels are present on this TrCH id */
      if(tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt > 1)
      {
        multiple_ded_log_chan = TRUE;
      }
    }
    /*
    * Get whether or not the TFS for a given TRCH is included or if it needs
    * to come from an uplink TrCH ID.
    */
    switch(ie_ptr->tfs_SignallingMode.t)
    {
      /* TFS explicitly given for this Transport Channel */
      case T_tdsrrc_DL_AddReconfTransChInformation_r5_tfs_SignallingMode_explicit_config:
        
        /* get the Transport Format Set for this TrCH */
        if(tdsrrcllc_get_dl_transport_format_set(ie_ptr->tfs_SignallingMode.u.explicit_config,
          &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
          multiple_ded_log_chan, /* mulitple logical channels? */
          &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
          ) == FAILURE)
        {
          return FAILURE;
        }
        break; /* end explicit TFS signalling for DL TrCH */
        
      case T_tdsrrc_DL_AddReconfTransChInformation_r5_tfs_SignallingMode_sameAsULTrCH:
        idx =0;
        if (ul_ptr == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"invalid:UL TFS absent when DL TFS choice 'same as UL'");
          return(FAILURE);
        }
        while(ul_ptr->n > idx)
        {
          /* check whether trCh is of type tdsrrc_UL_AddReconfTransChInformation_r6_dch_usch */
          if((T_tdsrrc_UL_AddReconfTransChInformation_r6_dch_usch == ul_ptr->elem[idx].t) &&
             (ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity == 
              ul_ptr->elem[idx].u.dch_usch->transportChannelIdentity))
          { 
            /* Got a match with Uplink TrCH ID */
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Get TFS for DL TrCH Id %d using UL TrCH ID %d",ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity,ul_ptr->elem[idx].u.dch_usch->transportChannelIdentity);
            if(tdsrrcllc_get_dl_transport_format_set(&ul_ptr->elem[idx].u.dch_usch->transportFormatSet,
              &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
              multiple_ded_log_chan, /* mulitple logical channels? */
              &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
              ) == FAILURE)
            {
              return FAILURE;
            }
            
            /* Indicate that found an UL TrCH ID, and break the while loop */
            got_one = TRUE;
            break;
          } /* end UL TrCH match */
          
          /* no match, keep looking */
          else 
          {
            idx++;
          }
        } /* end while */
        
        /* Make sure we found the UL TrCH, before continuing */
        if(!got_one)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No matching UL TFS in msg,look up from OC");
          /* Get the transport channel index for the UL transport channel ID.
          * Note: The index should already be there.
          */
          ul_trch_idx = tdsrrcllc_find_ul_dch_trch_idx(
            tdsordered_config_ptr,
            (tr_ch_id_type)ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
            ul_TransportChannelIdentity);
          if(ul_trch_idx >= TDSUE_MAX_TRCH)
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"No DL TFS for TrCH %d, to use UL TrCH %d for TFS",
              ie_ptr->dl_TransportChannelType.u.dch,
              ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
              ul_TransportChannelIdentity, 0);
            return(FAILURE);
          }
          
          /* Set up convenience pointers */
          dl_tfs_info = &tdsordered_config_ptr->dl_trch_info[trch_idx];
          ul_tfs_ss_info = &tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[ul_trch_idx];
          ul_tfs_dyn_info = &tdsordered_config_ptr->ul_tfcs.tfs_info[ul_trch_idx];
          
          /* Copy semistatic info from uplink */
          switch(ul_tfs_ss_info->coding)
          {
          case TDSL1_UNCODED:
            dl_tfs_info->coding_type = TDSL1_DL_UNCODED;
            break;
          case TDSL1_CONV_1_BY_2_CODING:
            dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_2_CODING;
            break;
          case TDSL1_CONV_1_BY_3_CODING:
            dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_3_CODING;
            break;
          case TDSL1_TURBO_CODING:
            dl_tfs_info->coding_type = TDSL1_DL_TURBO_CODING;
            break;
          default:
            break; 
          }
          
          
          dl_tfs_info->rate_match_attrib = ul_tfs_ss_info->staticRM;
          switch(ul_tfs_ss_info->crc)
          {
          case TDSL1_CRC_LENGTH_0:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_0;
            break;
          case TDSL1_CRC_LENGTH_8:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_8;
            break;
          case TDSL1_CRC_LENGTH_12:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_12;
            break;
          case TDSL1_CRC_LENGTH_16:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_16;
            break;
          case TDSL1_CRC_LENGTH_24:
            dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_24;
            break;
          default:
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CRC length %d in UL",ul_tfs_ss_info->crc);
            return FAILURE;
          }
          
          /* Copy dynamic info from uplink */
          dl_tfs_info->tti_value = ul_tfs_dyn_info->tti_info;
          dl_tfs_info->num_tf = (uint8) ul_tfs_dyn_info->ntf;
          for(tf_idx = 0; tf_idx < dl_tfs_info->num_tf; tf_idx++)
          {
            rrc_dl_tf_info = dl_tfs_info->tf_info_array[tf_idx];
            ul_tf_info = &ul_tfs_dyn_info->tf_info[tf_idx];
            
            rrc_dl_tf_info->num_tb = (uint8)ul_tf_info->ntblks;
            rrc_dl_tf_info->tb_size = ul_tf_info->tb_size;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"TB Size: %d,multiple_ded_log_chan: %d",rrc_dl_tf_info->tb_size,multiple_ded_log_chan);

            /* If number of logical channel is > 1, inc tb_size by 4 to accomodate MAC-header */
            if ( (rrc_dl_tf_info->tb_size > 0)&&(multiple_ded_log_chan == TRUE) )
            {
              rrc_dl_tf_info->tb_size += TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"New TB Size %d",rrc_dl_tf_info->tb_size);
            }
          }
        }
        break; /* end use of previous signalled UL TrCH TFS for DL TrCH */ 
        
    case T_tdsrrc_DL_AddReconfTransChInformation_r5_tfs_SignallingMode_hsdsch:
      /* The DFLOW Index has already been added. So get the index */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HSDSCH chosen");
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->tfs_SignallingMode.u.hsdsch,
        rrc,addOrReconfMAC_dFlow))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:DFLOW present for Add/Reconfig Dflow");
        
      
      
      /* Check if add & delete queue both are present and network specifies the same 
      queue id in both. This needs to be rejected */
      if ((ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.m.
        mac_hs_AddReconfQueue_ListPresent) &&
        (ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.mac_hs_AddReconfQueue_List.n > 0) &&
        (ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.m.
        mac_hs_DelQueue_ListPresent) &&
        (ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.mac_hs_DelQueue_List.n >0 ))
      { 
        idx =0;
        TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Add&Delete queue present");
        /* Check if UTRAN messed up by specifying the same queue id */
        delete_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.mac_hs_DelQueue_List;
        
        while (delete_queue_list_ptr->n > idx)
        {
          add_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow.mac_hs_AddReconfQueue_List;
          idy =0;
          while (add_queue_list_ptr->n > idy)
          {
            if (delete_queue_list_ptr->elem[idx].mac_hsQueueId == 
                add_queue_list_ptr->elem[idy].mac_hsQueueId)
            {
              /* Invalid Config */
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Queue %d simul added & deleted",delete_queue_list_ptr->elem[idx].mac_hsQueueId);
              return FAILURE;
            }
           idy++;
          }
          
          idx++;
        }
        
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Queue check passed");
      }

      if(ie_ptr->tfs_SignallingMode.u.hsdsch->m.addOrReconfMAC_dFlowPresent )
      {
        if (tdsrrcllc_set_dflow_info(&ie_ptr->tfs_SignallingMode.u.hsdsch->addOrReconfMAC_dFlow) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid DFLOW info");
          return FAILURE;
        }
      }
      }
      
      /* SET HARQ info in L1 structure */
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->tfs_SignallingMode.u.hsdsch,
        tdsrrc_HSDSCH_Info,harqInfo))
      {
        TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:HARQ info present");
        if (tdsrrcllc_set_harq_information(&ie_ptr->tfs_SignallingMode.u.hsdsch->harqInfo) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set HARQ information");
          return FAILURE;
        }
      }
      
      /* Indicate that HSDSCH Info is present which would be used later to set bit mask when HSDPA
      is already ACTIVE */
      tdshsdpa_msg_params.hsdsch_info_present = TRUE;
      
      tdshsdpa_msg_params.msg_params_present = TRUE;
      break;
      
    default:
      return FAILURE;
      
    } /* end TFS selection switch */

    /*Find Rbs mapped to this DCH and fill them in rb_list_mapped_to_dch if their RLC
    PDU size has changed.*/ 
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_dch))
    {
      tdsrrcllc_get_rb_for_rb_list_mapped_to_dch((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch, 
                tdsordered_config_ptr->rrc_state_indicator, TDSDL);  
    }       
  } /* end CELL_DCH check */
  
  /* States not supported */
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_FACH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_PCH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"URA_PCH not supported for TrCH");
    return FAILURE;
  }
  else
  {
    ERR_FATAL("Unknown state not supported for TrCH",0,0,0);
  }
  
  /***************************************************************/
  /* Transport Channel Count has been updated in RB Mapping info */
  /***************************************************************/
  if (!(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->tfs_SignallingMode,rrc_DL_AddReconfTransChInformation_r5_tfs_SignallingMode_hsdsch)))
  {
    /* Check for the BLER */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
        tdsrrc_DL_AddReconfTransChInformation_r5,dch_QualityTarget))
    {
      /* Save the BLER if found, and is in range */
      if((ie_ptr->dch_QualityTarget.bler_QualityValue >= TDSRRCLLC_MIN_BLER) &&
        (ie_ptr->dch_QualityTarget.bler_QualityValue <= TDSRRCLLC_MAX_BLER))
      {
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val =
          (int8)ie_ptr->dch_QualityTarget.bler_QualityValue;
      }
      
      /* BLER is out of range */
      else
      {
        /* According to L1 I/F, must set an invalid value of -127 */
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL BLER %d is out of range",ie_ptr->dch_QualityTarget.bler_QualityValue);
        return FAILURE;
      }
    } /* end if BLER present */
    else
    {
      /* IE omitted. Set the target to -127 which would result 1% of BLER target.
       * This is good UE behaviour though the spec is ambiguous on what UE
       * behavior should be in this case.  
       */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DCH quality target not present. Setting to -127");
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
    }
  }
  else
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
        tdsrrc_DL_AddReconfTransChInformation_r5,dch_QualityTarget))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring dch quality target for HS");
    }
  }
  /* Indicate that DL L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  
  return SUCCESS;
} /* end tdsrrcllc_get_dl_added_reconfig_trch_r6() */

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
  tdsrrc_DL_AddReconfTransChInfoList_r5 * dl_trch_add_reconfig_ptr,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r6  * ul_trch_add_reconfig_ptr  
)
{
  uint32 idx, tf_idx;
  /* Get the DL Added or Reconfigured Transport Channel information */
  if(dl_add_reconf_trch_info_list_present)
  {

    idx = 0;
    
    /* Save info for each additional Transport Channel */
    while((idx <= TDSUE_MAX_TRCH) && (dl_trch_add_reconfig_ptr->n > idx))
    {
      if(tdsrrcllc_get_dl_added_reconfig_trch_r6(&dl_trch_add_reconfig_ptr->elem[idx],
        ul_trch_add_reconfig_ptr) == FAILURE)
      {
        return FAILURE;
      }
      /* get next TrCH Info */
      idx++;

    } /* end while */
  } 
  
  /* At this point store the Queue Information in L1 structure. To add later if HSDSCH config changed */
  
  /* Adjust TB size on every TrCH if necessary. */ 
  for (idx=0; idx<TDSUE_MAX_TRCH; idx++)
  {
  /* The number of logical channel mapped to this transport channel 
  increases from 1 to 2. Needs to adjsut the tb_szie to
    reflect the adding of the MAC header.                         */
    if (tdsordered_config_ptr->dl_dch_trch_info[idx].dch_present &&
      (tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up ||
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down))
    {
      /* TrCH IE is present and the TrCH ID is appear in the TrCH info table. */
      if (!(dl_add_reconf_trch_info_list_present &&
        tdsrrcllc_trch_id_in_config_trch_list_r5(dl_trch_add_reconfig_ptr,
        tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id)))
      {
        /* Added MAC head size to the current TB size.  */
        if (tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up &&
          !tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Increasing TrCH %d TB size,%d by MAC hdr size",tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id,tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[0]->tb_size);
          for (tf_idx=0; tf_idx<tdsordered_config_ptr->dl_trch_info[idx].num_tf; tf_idx++)
          {
              if (tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size > 0)
          		{
                tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size += 
                  TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
          		}
            
          }
        }
        /* Substract MAC head size from the current TB size.  */
        else if (!tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up &&
          tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Decreasing TrCH %d TB size,%d by MAC hdr size",tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id,tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[0]->tb_size);
          for (tf_idx=0; tf_idx<tdsordered_config_ptr->dl_trch_info[idx].num_tf; tf_idx++)
          {
            if (tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size > 0)
            {
              tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size -= 
                TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
            }
          }
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clean TB size adjust flag in RBSetup");
      /* Reset lg_ch_cnt_up and lg_ch_cnt_down. */
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up = FALSE;
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down = FALSE;
    }
    else if (!tdsordered_config_ptr->dl_dch_trch_info[idx].dch_present)
    {
    /* Since the activated transport channels are sorted before stored in
      the memory, break the looping after reaching the 1st invalid block.  */
      break;
    }
  }
  
  /* Check to make sure DL TFS count is within UE Capabilities */
  if(tdsrrcllc_check_dl_tfs_cnt() == FAILURE)
  {
    return FAILURE;
  }

  return(SUCCESS);
}

/*====================================================================
FUNCTION: tdsrrc_validate_ul_tfc_info()

DESCRIPTION:
  This function validates the tfc info
  
DEPENDENCIES:
  None.

RETURN VALUE:
Returns failure if SRB - 1/2/3 TFCS are not valid 

SIDE EFFECTS:
====================================================================*/
static uecomdef_status_e_type tdsrrc_validate_ul_tfc_info(void)
{
  uint32 size = TDSRRCLLC_INVALID_RLC_SIZE;
  uint32 trch_count;
  uint32 tfc_count;
  uint32 dlch_count;
  boolean  rb_found = FALSE;
  uint8 rb_id;
  uint8 tf_index = TDSUE_MAX_TF;
  tdsordered_config_type  *config_ptr = tdsordered_config_ptr;

  /* Skip validation if E-DCH transmission is true */
#ifdef FEATURE_TDSCDMA_HSUPA
  if(tdsordered_config_ptr->e_dch_transmission == TRUE)
  {
    return SUCCESS;
  }
#endif
  
  /*Loop over all SRBs to find if any TFCs are valid */     
  for(rb_id = DCCH_UM_RADIO_BEARER_ID; rb_id <= DCCH_DT_HIGH_PRI_RADIO_BEARER_ID; rb_id++)
  {
    rb_found = FALSE;
    
    /* Get the transport channel mapped onto this RB_ID */
    for(trch_count = 0;
        (trch_count < config_ptr->ul_tfcs.nchan) && (rb_found == FALSE);
        trch_count ++)
    {
      for(dlch_count = 0;
          (dlch_count < config_ptr->mac_ul_parms.cctrch_ptr->dch_info.
            dch_info[trch_count].ndlchan) && (rb_found == FALSE);
          dlch_count ++)
      {
     		
        if(config_ptr->mac_ul_parms.cctrch_ptr->dch_info.
           dch_info[trch_count].dlchan_info[dlch_count].rb_id == rb_id)
        {
          size = 0;
          rb_found = TRUE;         
          
          for(tfc_count = 0;
              tfc_count < config_ptr->ul_tfcs.ntfc;
              tfc_count ++)
          {  
             tf_index = TDSUE_MAX_TF;
             /* Consider only Allowed TFCs */
             if(config_ptr->ul_tfcs.tfc_info[tfc_count].control_flag == TDSTFC_ALLOWED)
             {
               /* Get the TF index from TFI table */
               tf_index = config_ptr->ul_tfcs.tfc_info[tfc_count].tfi[trch_count];
               if(tf_index < TDSUE_MAX_TF)
               {
                 TDSRRC_MSG2(MSG_LEGACY_MED,"TB size %d NT Blocks %d",config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_index].tb_size,config_ptr->ul_tfcs.tfs_info[trch_count].tf_info[tf_index].ntblks);
                 /* Use the tf_index to get the size */
                 size +=  config_ptr->ul_tfcs.tfs_info[trch_count].
                      tf_info[tf_index].tb_size *  
                      config_ptr->ul_tfcs.tfs_info[trch_count].
                      tf_info[tf_index].ntblks;
                 if(size > 0)
                 {
                   /* Break here as size is valid */
                   break;
                 }
               }
             }
          }
          /* Reject the config if there are no TFCs with size more than 0 */
          if(size == 0)
          {            
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reject config:Invalid TFCs configured");
            return FAILURE;
          }
        }
      }
    }
  }        
  return SUCCESS;
}
/*====================================================================
FUNCTION: tdsrrc_validate_dl_tfc_info()

DESCRIPTION:
  This function validates the tfc info
  
DEPENDENCIES:
  None.

RETURN VALUE:
Returns failure if SRB - 1/2/3 TFCS are not valid 

SIDE EFFECTS:
====================================================================*/
static uecomdef_status_e_type tdsrrc_validate_dl_tfc_info(void)
{

  uint32 size = TDSRRCLLC_INVALID_RLC_SIZE;
  uint32 trch_count;
  uint32 tfc_count;
  uint32 dlch_count;
  uint8 rb_id;
  uint8 tf_index = TDSL1_DL_MAX_TF;
  boolean  rb_found = FALSE;
  tdsordered_config_type  *config_ptr = tdsordered_config_ptr;
  tdsrrc_hsdpa_status_enum_type hs_status = tdsrrc_get_hsdpa_status();
  tdshsdpa_action_enum_type hs_action = tdsrrc_get_hsdpa_action();
  
  /* Skip validation if HSDPA is started/Active */
  if (((hs_status == TDSHSDPA_ACTIVE) &&(hs_action != TDSHSDPA_STOP)) ||
      ((hs_status == TDSHSDPA_INACTIVE) &&(hs_action == TDSHSDPA_START)))
  {
    return SUCCESS;
  }

  /*Loop over all SRBs to find if any TFCs are valid */     
  for(rb_id = DCCH_UM_RADIO_BEARER_ID; rb_id <= DCCH_DT_HIGH_PRI_RADIO_BEARER_ID; rb_id++)
  {
    rb_found = FALSE;
    for(trch_count = 0;
        (trch_count < config_ptr->l1_dl_cctrch_parms.num_trch) && (rb_found == FALSE);
        trch_count ++)
    {
      for(dlch_count = 0;
           (dlch_count < config_ptr->mac_dl_parms.dch_info[trch_count]->ndlchan) && (rb_found == FALSE);
           dlch_count ++ )
      {
        /* Get the transport channel mapped onto this RB_ID */
        if(config_ptr->mac_dl_parms.dch_info[trch_count]->
            dlchan_info[dlch_count].rb_id == rb_id)
        {
          rb_found = TRUE;
          size = 0;
         
          for(tfc_count = 0;
              tfc_count < config_ptr->dl_ctfc.num_ctfc_entry;
              tfc_count ++)
          {
            tf_index = TDSL1_DL_MAX_TF;
            /* Consider only Allowed TFCs */
            if(config_ptr->dl_ctfc.ctfc_entry_data[tfc_count].state == TDSALLOWED)
            {
              /* Get the TF index from TFI table */
              tf_index = config_ptr->dl_ctfc.ctfc_entry_data[tfc_count].tfi_table[trch_count];
              if(tf_index < TDSL1_DL_MAX_TF)
              {
                /* Use the tf_index to get the size */
                size += config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                   tf_info_array[tf_index]->tb_size * 
                  config_ptr->l1_dl_cctrch_parms.trch_info_array[trch_count]->
                   tf_info_array[tf_index]->num_tb;
                if(size > 0)
                {
                  /* Break here as size is valid */
                  break;
                }
              }
            } 
          } /* For all TFCs */
          if(size == 0)
          { 
            /* Reject the config if there are no TFCs with size more than 0 */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reject config:Invalid TFCs configured");
            return FAILURE;
          }
        } /* If RB is found */
      } /* For all dedicated logical channels on this TrCH */
    }  /* For all TrCHs */
  }
  return SUCCESS;
}

/*===========================================================================

FUNCTION    tdsrrcllc_check_trch_mapped_to_srb_present

DESCRIPTION This function checks if the Trch mapped to SRBs in 
             CELL DCH state exist
 
DEPENDENCIES
  None.

RETURN VALUE
 SUCCESS or FAILURE

===========================================================================*/
uecomdef_status_e_type tdsrrcllc_check_trch_mapped_to_srb_present
(
void
)
{
  uint32 cnt,trch_idx;
#ifdef FEATURE_TDSCDMA_HSUPA
  uint32 index;
#endif
  boolean dl_trch_info_present = FALSE;
  boolean ul_trch_info_present = FALSE;

#ifndef TEST_FRAMEWORK
  if((FAILURE == tdsrrc_validate_dl_tfc_info()) || (FAILURE == tdsrrc_validate_ul_tfc_info()))
  {
    return FAILURE;
  }
#endif

  for(trch_idx=0;trch_idx< TDSUE_MAX_TRCH;trch_idx++)
  {
    if((tdsordered_config_ptr->dl_dch_trch_info[trch_idx].dch_present) &&
        (tdsordered_config_ptr->dl_dch_trch_idx_in_use[trch_idx] == TRUE) )
    {
      for (cnt = 0; cnt < tdsordered_config_ptr->dl_dch_info[trch_idx].ndlchan;cnt++) 
      {      
        if (tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[cnt].rb_id < DCCH_DT_LOW_PRI_RADIO_BEARER_ID )
        {
          /* this IE must have already been provided as part of mapping info. Just
             return success
           */
          TDSRRC_MSG0(MSG_LEGACY_MED,"DL Trch mapped to SRb exists ");
          dl_trch_info_present = TRUE;
          break;
        }
      }
    }

    if((tdsordered_config_ptr->ul_dch_trch_info[trch_idx].dch_present) &&
        (tdsordered_config_ptr->ul_dch_trch_idx_in_use[trch_idx] == TRUE))
    {
      for (cnt = 0; cnt < tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan; cnt++) 
      {      
        if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnt].rb_id < DCCH_DT_LOW_PRI_RADIO_BEARER_ID )
        {
          /* this IE must have already been provided as part of mapping info. Just
             return success
           */
          TDSRRC_MSG0(MSG_LEGACY_MED,"UL Trch mapped to SRb exists ");
          ul_trch_info_present = TRUE;
          break;
        }
      }
    }
    if( (dl_trch_info_present == TRUE ) && (ul_trch_info_present == TRUE )) 
    {
        return (SUCCESS);
    }
  }
#ifdef FEATURE_TDSCDMA_SRB_MAPPING_TO_HSDPA
  if(dl_trch_info_present == FALSE ) 
  {
    for(index = 0; index < UE_MAX_MAC_D_FLOW;  index++)
    {
      if(ordered_config_ptr->dl_dflow_index_in_use[index] == TRUE)
      {
        for (cnt = 0; cnt < ordered_config_ptr->mac_dflow_info[index].ndlchan; cnt++) 
        {
          if((ordered_config_ptr->mac_dflow_info[index].dlchan_info[cnt].rb_id < DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
          {
            /* this IE must have already been provided as part of mapping info. Just
               return success
             */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACD  mapped to SRB exists ");
            dl_trch_info_present = TRUE;
            break;
          }          
        }
        if(dl_trch_info_present == TRUE )
        {
          break;
          
        }
      }
    }
  }
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  /*We check for only mapping for MAC-ehs for one SRB only.*/
  if(dl_trch_info_present == FALSE) 
  {
    /*Loop through all the DL logical channels and find out the ones that are for SRBs*/
    for(index = 0; index < UE_MAX_DL_LOGICAL_CHANNEL;  index++)
    {
      if((ordered_config_ptr->mac_ehs_lc_id_in_use[index] == TRUE) &&
        (ordered_config_ptr->dl_macehs_logchan_info[index].rb_id < DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
      {
        /*Now we got the DL logical channel for some SRB*/
        for (cnt = 0; cnt < UE_MAX_HS_QUEUE; cnt++) 
        {
          /*Check whether Q-ID for this SRB is present or not.*/
          if((ordered_config_ptr->mac_ehs_queue_index_in_use[cnt] == TRUE) &&
            (ordered_config_ptr->dl_macehs_logchan_info[index].queue_id == 
            ordered_config_ptr->mac_ehs_queue_info[cnt].queue_id))
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"SRB %d is mapped to MAC-ehs Reordering Q %d",ordered_config_ptr->dl_macehs_logchan_info[index].rb_id,ordered_config_ptr->mac_ehs_queue_info[cnt].queue_id);

            dl_trch_info_present = TRUE;
            break;
          }          
        }
        if(dl_trch_info_present == TRUE )
        {
          break;
        }
      }
    }
  }
#endif //FEATURE_TDSCDMA_HSDPA_MAC_EHS
  if( (dl_trch_info_present == TRUE ) && (ul_trch_info_present == TRUE )) 
  {
      return (SUCCESS);
  }
#endif //FEATURE_TDSCDMA_SRB_MAPPING_TO_HSDPA
#ifdef FEATURE_TDSCDMA_HSUPA
  if(ul_trch_info_present == FALSE)
  {
    for(index = 0; index < tdsordered_config_ptr->mac_e_config.num_e_mac_d_flow;  index++)
    {
      for (cnt = 0; cnt < tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].num_logch; cnt++) 
      {
        if(tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[cnt].rb_id < DCCH_DT_LOW_PRI_RADIO_BEARER_ID)   
        {
          /* this IE must have already been provided as part of mapping info. Just
             return success
           */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MAC-E mapped to SRb exists ");
          ul_trch_info_present = TRUE;
          break;
        }
      }
      if(ul_trch_info_present == TRUE )
      {
        break;
      }
    }
  }
#endif/* FEATURE_TDSCDMA_HSUPA*/


  if( (dl_trch_info_present == TRUE ) && (ul_trch_info_present == TRUE )) 
  {
    return (SUCCESS);
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"dl_trch_info_present %d ul_trch_info_present %d",dl_trch_info_present,ul_trch_info_present);
    return (FAILURE);
  }
}

/*============================================================================
FUNCTION: tdsrrcllc_check_if_tfi_present_in_allowed_list_in_tfcc()

DESCRIPTION:
  This function checks if the TFI for a Trch is present
  in the allowed list in Restricted transport channel list
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
)
{
  int i,j;
  boolean trch_id_in_restricted_list = FALSE;
  uint8 tfi;
  i=j=0;
  while(i<restricted_info->restriction_info.trch_restriction_info_list.ntrch)
  {
    if((config_ptr->ul_dch_trch_info[trch_index].dch_present) &&
      (restricted_info->restriction_info.trch_restriction_info_list.trch_restriction_info[i].trch_type == TDSDCH)&&
      (config_ptr->ul_dch_trch_info[trch_index].trch_id == 
         restricted_info->restriction_info.trch_restriction_info_list.trch_restriction_info[i].trch_id))
    {
      /* The Trch id is present in the Restriched Trch List in OTA */
      trch_id_in_restricted_list = TRUE;
      if(restricted_info->restriction_info.trch_restriction_info_list.trch_restriction_info[i].ntf)
      {
        j=0;
        while(j< restricted_info->restriction_info.trch_restriction_info_list.trch_restriction_info[i].ntf)
        {
          /* Since the TFI is present in the allowed TFI list in OTA, return success */
          if( config_ptr->ul_tfcs.tfc_info[tfci_index].tfi[trch_index] == 
              restricted_info->restriction_info.trch_restriction_info_list.trch_restriction_info[i].tfi[j])
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"TFCC: Found the Allowed TF index %d for trch_idx %d in TFCI %d",j,trch_index,tfci_index);
            return SUCCESS;
          }
          j++;
        }
      }
      else
      {
        /* If the Allowed List for this Trch is not present, 
          * then TB size 0 shld be treated as TDSALLOWED, all other TFIs should be
          * NOT TDSALLOWED
          */
        tfi =config_ptr->ul_tfcs.tfc_info[tfci_index].tfi[trch_index];
              
        if(config_ptr->ul_tfcs.tfs_info[trch_index].tf_info[tfi].ntblks *
          config_ptr->ul_tfcs.tfs_info[trch_index].tf_info[tfi].tb_size == 0)
        { 
         /*No allowed list, but TDSALLOWED TFI as TB Size =0 */
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCC: No allowed TF's for trch_idx %d,Allowed as zero",trch_index);
          return SUCCESS;
        } 
      }
    }
    if(trch_id_in_restricted_list == TRUE )
    {
      break;
    }
    i++;
  }
  if(trch_id_in_restricted_list == FALSE)
  {
    return SUCCESS;
  }
  else
  {
    /* We return FAILURE as, the Trch ID was present in the 
      * Restriched Trch List in OTA, but not in the allowed TFI list
      */  
    return FAILURE;
  }
}


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
)
{

  int i,j=0;
  tdsordered_config_type * config_ptr;
  if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
  {
    /*TFCC message is received in FACH state*/
    if(tdstransition_config.toc_usage == TDSTOC_FOR_DCH)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC: Updating the TFCC restriction info to TOC");
      config_ptr= tdstransition_config.toc_ptr;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCC: TOC usage is %d,do not update in TOC",tdstransition_config.toc_usage);
      return;
    }
  }
  else
  {
    /*TFCC message is received in DCH state*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC: Updating the TFCC restriction info to CC");
    config_ptr = tdscurrent_config_ptr;
  }

  switch(restricted_info->tfci_restriction_choice)
  {
    case TDSMIN_ALLOWED_TFCI:
      for(i=0;i<config_ptr->ul_tfcs.ntfc;i++)
      {
        if(config_ptr->ul_tfcs.tfc_info[i].control_flag != TDSTFC_REMOVED)
        {
          config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_NOT_ALLOWED;
        }
      }
      for(i=0;i<=restricted_info->restriction_info.tfci;i++)
      {
        config_ptr->ul_tfcs.tfc_info[i].control_flag =TDSTFC_ALLOWED;
      }
      break;
    case TDSALLOWED_TFCI:
      i = 0;
      while(i < config_ptr->ul_tfcs.ntfc)
      {
        if(config_ptr->ul_tfcs.tfc_info[i].control_flag != TDSTFC_REMOVED)
        {
          config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_NOT_ALLOWED;
        }
        i++;
      }
      i=0;
      while(i<restricted_info->restriction_info.tfci_restriction_info.ntfc)
      {
        config_ptr->ul_tfcs.tfc_info[restricted_info->restriction_info.tfci_restriction_info.tfci_list[i]].
            control_flag = TDSTFC_ALLOWED;
        i++;
      }
      break;
    case TDSNOT_ALLOWED_TFCI:
      i = 0;
      while(i < config_ptr->ul_tfcs.ntfc)
      {
        if(config_ptr->ul_tfcs.tfc_info[i].control_flag != TDSTFC_REMOVED)
        {
          config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_ALLOWED;
        }
        i++;
      }
      i=0;
      while(i<restricted_info->restriction_info.tfci_restriction_info.ntfc)
      {
        config_ptr->ul_tfcs.tfc_info[restricted_info->restriction_info.tfci_restriction_info.tfci_list[i]].
            control_flag = TDSTFC_NOT_ALLOWED;
        i++;
      }
      break;
    case TDSRESTRICTED_TFCI:
      /* i indicates the tfci index */
      for(i=0;i<config_ptr->ul_tfcs.ntfc && (i < TDSUE_MAX_TFC);i++)
      {
        boolean tfci_allowed = TRUE;
        /* j represents each Trch index */
        for(j=0;j< config_ptr->ul_tfcs.nchan && (j < TDSUE_MAX_TRCH); j++)
        {
          if(tdsrrcllc_check_if_tfi_present_in_allowed_list_in_tfcc(config_ptr,restricted_info,i,j)
               == FAILURE)
          {
            tfci_allowed = FALSE;
            break;
          }     
        }
          
        if(config_ptr->ul_tfcs.tfc_info[i].control_flag != TDSTFC_REMOVED)
        { 
          if(tfci_allowed == FALSE )
          {
            config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_NOT_ALLOWED;  
          }
          else
          {
            config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_ALLOWED;
          }
        }
      }
      break;
    case TDSFULL_TFCI:
      i = 0;
      while(i < config_ptr->ul_tfcs.ntfc)
      {
        if(config_ptr->ul_tfcs.tfc_info[i].control_flag != TDSTFC_REMOVED)
        {
          config_ptr->ul_tfcs.tfc_info[i].control_flag = TDSTFC_ALLOWED;
        }
        i++;
      }
      break;
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC: Allowed TFCI's are");
  for(i=0;i<config_ptr->ul_tfcs.ntfc;i++)
  {
    if(config_ptr->ul_tfcs.tfc_info[i].control_flag == TDSTFC_ALLOWED)
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCC: %d",i);
  }
  /*
    If a OTA is received during TFCC handling taking UE from DCH to DCH,
    and the OTA doesnt change TFCS, then update OC with the restrcition info of TFCC
  */
  if((tdsconfig_ptr_in_use == TDSORDERED_CONFIG)&&
    (tdsrrc_get_state()==TDSRRC_STATE_CELL_DCH)&&
    (tdsordered_config.set_status == TDSOC_SET_FOR_CELL_DCH) &&
    (!tdsordered_config_ptr->mac_ul_parms.new_tfcs_list_present))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC:OC is Set,Copy Restriction info from CC to OC");
    memscpy(&tdsordered_config_ptr->ul_tfcs,sizeof(tdsmac_ul_tfcs_type),&config_ptr->ul_tfcs, sizeof(tdsmac_ul_tfcs_type));
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TFCC: Allowed TFCI's in OC are");
    for(i=0;i<tdsordered_config_ptr->ul_tfcs.ntfc;i++)
    {
      if(tdsordered_config_ptr->ul_tfcs.tfc_info[i].control_flag == TDSTFC_ALLOWED)
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TFCC: %d",i);
    }
  }
}

/*==========================================================
FUNCTION  :   tdsrrcllc_update_l1_with_ul_trch()

DESCRIPTION : This function updates the L1 info in Config ptr with 
              UL Trch info

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
void tdsrrcllc_update_l1_with_ul_trch
(
  tdsordered_config_type * config_ptr
)
{
   TDSRRC_MSG1(MSG_LEGACY_MED,"Update l1 with UL Trch info num trch %d",config_ptr->rrc_ul_cctrch_params.num_trch);
   
   /* config_ptr->l1_ul_cctrch_parms.num_trch = config_ptr->rrc_ul_cctrch_params.num_trch; */
}

/*================================================================================================
FUNCTION: tdsrrcllc_get_dl_added_reconfig_trch_r7()

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
static uecomdef_status_e_type tdsrrcllc_get_dl_added_reconfig_trch_r7
(
  tdsrrc_DL_AddReconfTransChInformation_r7 * ie_ptr,
  tdsrrc_UL_AddReconfTransChInfoList_r7 * ul_ptr_r7
 
)
{
  /* local transport channel variables */
  tr_ch_id_type trch_idx = 0;
  tr_ch_id_type ul_trch_idx = 0;
  uint32 idx =0,idy=0;

  /* Pointers to UL and DL TFS info for use when copying existing config */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info = NULL;
  tdsl1_ul_semi_static_parms_struct_type *ul_tfs_ss_info = NULL;
  tdsmac_ul_tfs_type *ul_tfs_dyn_info = NULL;
  
  /* Loop variables for copying UL TFS info */
  uint8 tf_idx = 0;
  /*lint -e578 Declaration of symbol 'rrc_dl_tf_info' hides symbol 'rrc_dl_tf_info'*/
  tdsl1_dl_cctrchcfg_tf_info_struct_type *rrc_dl_tf_info = NULL;
  tdsmac_ul_tf_type *ul_tf_info = NULL;
  
  /* Boolean used to indicate if multiple logical channels are mapped to
  a given TrCH. */
  boolean       multiple_ded_log_chan = FALSE;
  
  /* local variable indicating whether or not an uplink TrCH ID was found */
  boolean       got_one = FALSE;
  
  tdsrrc_MAC_hs_AddReconfQueue_List * add_queue_list_ptr = NULL;
  
  tdsrrc_MAC_hs_DelQueue_List * delete_queue_list_ptr = NULL;
  
  
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Add/Reconf TrCH");
  
  /*
  * Use the Transport Channel Index and next state to place the TFS in 
  * the correct slot in the MAC TFS array.
  */
  if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
  {
    /* Get the transport channel index for this transport channel ID.
      * Note: The index should already be there. */
    
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_hsdsch))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HS TrCh to be added");
    }
    else
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_dch))
      {
        
        trch_idx = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch);
        
        if(trch_idx == TDSRRCLLC_INVALID_TRCH_IDX)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,no idx found",ie_ptr->dl_TransportChannelType.u.dch);
          return(FAILURE);
        }

        if(tdsrrcllc_restore_dl_dch_rb_mapping_info(trch_idx) == FAILURE)
        {
          /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,not correct in Standalone RB-Mapping",ie_ptr->dl_TransportChannelType.u.dch);
        } 

      }
      
      /* Determine if multiple dedicated logical channels are present on this TrCH id */
      if(tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt > 1)
      {
        multiple_ded_log_chan = TRUE;
      }
    }
    /*
      * Get whether or not the TFS for a given TRCH is included or if it needs
      * to come from an uplink TrCH ID.
      */
    switch(ie_ptr->tfs_SignallingMode.t)
    {
      /* TFS explicitly given for this Transport Channel */
      case T_tdsrrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_explicit_config:
        
        /* get the Transport Format Set for this TrCH */
        if(tdsrrcllc_get_dl_transport_format_set(ie_ptr->tfs_SignallingMode.u.explicit_config,
          &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
          multiple_ded_log_chan, /* mulitple logical channels? */
          &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
          ) == FAILURE)
        {
          return FAILURE;
        }
        break; /* end explicit TFS signalling for DL TrCH */
        
      case T_tdsrrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_sameAsULTrCH:

        if (ul_ptr_r7 == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"invalid:UL TFS absent when DL TFS choice 'same as UL'");
          return(FAILURE);
        }

        while(ul_ptr_r7->n > idx)
        {
          /* check whether trCh is of type tdsrrc_UL_AddReconfTransChInformation_r6_dch_usch */
          if((TDSRRC_CHECK_LIST_MSG_TYPE(ul_ptr_r7,tdsrrc_UL_AddReconfTransChInformation_r7_dch_usch)) &&
             (ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity == 
              ul_ptr_r7->elem[idx].u.dch_usch->transportChannelIdentity))
          { 
            /* Got a match with Uplink TrCH ID */
            TDSRRC_MSG2(MSG_LEGACY_LOW,"Get TFS for DL TrCH Id %d using UL TrCH ID %d",ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity,ul_ptr_r7->elem[idx].u.dch_usch->transportChannelIdentity);
             if(tdsrrcllc_get_dl_transport_format_set(&ul_ptr_r7->elem[idx].u.dch_usch->transportFormatSet,
              &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
              multiple_ded_log_chan, /* mulitple logical channels? */
              &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
              ) == FAILURE)
            {
              return FAILURE;
            }
            
            /* Indicate that found an UL TrCH ID, and break the while loop */
            got_one = TRUE;
            break;
          } /* end UL TrCH match */
          
          /* no match, keep looking */
          else 
          {
            idx++;
          }
        }
               
        /* Make sure we found the UL TrCH, before continuing */
        if(!got_one)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No matching UL TFS in msg,look up from OC");
          /* Get the transport channel index for the UL transport channel ID.
          * Note: The index should already be there.
          */
          ul_trch_idx = tdsrrcllc_find_ul_dch_trch_idx(
            tdsordered_config_ptr,
            (tr_ch_id_type)ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
            ul_TransportChannelIdentity);
          if(ul_trch_idx == TDSRRCLLC_INVALID_TRCH_IDX)
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"No DL TFS for TrCH %d, to use UL TrCH %d for TFS",
              ie_ptr->dl_TransportChannelType.u.dch,
              ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
              ul_TransportChannelIdentity, 0);
            return(FAILURE);
          }
          
          /* Set up convenience pointers */
          dl_tfs_info = &tdsordered_config_ptr->dl_trch_info[trch_idx];
          ul_tfs_ss_info = &tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[ul_trch_idx];
          ul_tfs_dyn_info = &tdsordered_config_ptr->ul_tfcs.tfs_info[ul_trch_idx];
          
          /* Copy semistatic info from uplink */
          switch(ul_tfs_ss_info->coding)
          {
            case TDSL1_UNCODED:
              dl_tfs_info->coding_type = TDSL1_DL_UNCODED;
              break;
            case TDSL1_CONV_1_BY_2_CODING:
              dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_2_CODING;
              break;
            case TDSL1_CONV_1_BY_3_CODING:
              dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_3_CODING;
              break;
            case TDSL1_TURBO_CODING:
              dl_tfs_info->coding_type = TDSL1_DL_TURBO_CODING;
              break;
            default:
              break; 
          }
          
          
          dl_tfs_info->rate_match_attrib = ul_tfs_ss_info->staticRM;
          switch(ul_tfs_ss_info->crc)
          {
            case TDSL1_CRC_LENGTH_0:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_0;
              break;
            case TDSL1_CRC_LENGTH_8:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_8;
              break;
            case TDSL1_CRC_LENGTH_12:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_12;
              break;
            case TDSL1_CRC_LENGTH_16:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_16;
              break;
            case TDSL1_CRC_LENGTH_24:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_24;
              break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CRC length %d in UL",ul_tfs_ss_info->crc);
              return FAILURE;
          }
          
          /* Copy dynamic info from uplink */
          dl_tfs_info->tti_value = ul_tfs_dyn_info->tti_info;
          dl_tfs_info->num_tf = (uint8) ul_tfs_dyn_info->ntf;
          for(tf_idx = 0; tf_idx < dl_tfs_info->num_tf; tf_idx++)
          {
            rrc_dl_tf_info = dl_tfs_info->tf_info_array[tf_idx];
            ul_tf_info = &ul_tfs_dyn_info->tf_info[tf_idx];
            
            rrc_dl_tf_info->num_tb = (uint8)ul_tf_info->ntblks;
            rrc_dl_tf_info->tb_size = ul_tf_info->tb_size;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"TB Size: %d,multiple_ded_log_chan: %d",rrc_dl_tf_info->tb_size,multiple_ded_log_chan);

            /* If number of logical channel is > 1, inc tb_size by 4 to accomodate MAC-header */
            if ( (rrc_dl_tf_info->tb_size > 0)&&(multiple_ded_log_chan == TRUE) )
            {
              rrc_dl_tf_info->tb_size += TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"New TB Size %d",rrc_dl_tf_info->tb_size);
            }
          }
        }
        break; /* end use of previous signalled UL TrCH TFS for DL TrCH */ 
        
      case T_tdsrrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_hsdsch:
        /* The DFLOW Index has already been added. So get the index */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->tfs_SignallingMode.u.hsdsch,
            rrc,dl_MAC_HeaderType))
        {
          if (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r7_dl_MAC_HeaderType_mac_hs)
          {
  
            TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:DFLOW present for Add/Reconfig Dflow");
              
            
            
            /* Check if add & delete queue both are present and network specifies the same 
            queue id in both. This needs to be rejected */
            if ((ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->m.
                mac_hs_AddReconfQueue_ListPresent) &&
              (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List.n > 0) &&
              (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->m. 
                mac_hs_DelQueue_ListPresent) &&
              (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_DelQueue_List.n > 0))
            {
              TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Add&Delete queue present");
              /* Check if UTRAN messed up by specifying the same queue id */
              delete_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_DelQueue_List;
              idx =0;
              while (delete_queue_list_ptr->n > idx)
              {
                add_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List;
                idy =0;
                while (add_queue_list_ptr->n > idy)
                {
                  if (delete_queue_list_ptr->elem[idx].mac_hsQueueId == add_queue_list_ptr->elem[idy].mac_hsQueueId)
                  {
                    /* Invalid Config */
                    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Queue %d simul added & deleted",delete_queue_list_ptr->elem[idx].mac_hsQueueId);
                    return FAILURE;
                  }
                  idy++;
                }
                
                idx++;
              }
              
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Queue check passed");
            }
            
            if (tdsrrcllc_set_dflow_info(ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs) == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid DFLOW info");
              return FAILURE;
            }
          }
          else if (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r7_dl_MAC_HeaderType_mac_ehs)
          {
  #ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS: MACEHS info received");
  #else
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported ");
  #endif
          }
        }
        /* SET HARQ info in L1 structure */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->tfs_SignallingMode.u.hsdsch,
            tdsrrc_HSDSCH_Info_r7,harqInfo))
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:HARQ info present");
          if (tdsrrcllc_set_harq_information_r7(&ie_ptr->tfs_SignallingMode.u.hsdsch->harqInfo) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set HARQ information");
            return FAILURE;
          }
        }
        
        /* Indicate that HSDSCH Info is present which would be used later to set bit mask when HSDPA
        is already ACTIVE */
        tdshsdpa_msg_params.hsdsch_info_present = TRUE;
        
        tdshsdpa_msg_params.msg_params_present = TRUE;
        
        break;
        
      default:
        return FAILURE;
      
    } /* end TFS selection switch */

    /*Find Rbs mapped to this DCH and fill them in rb_list_mapped_to_dch if their RLC
        PDU size has changed.*/ 
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,rrc_DL_TrCH_TypeId1_r5_dch))
    {
      tdsrrcllc_get_rb_for_rb_list_mapped_to_dch((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch, 
                tdsordered_config_ptr->rrc_state_indicator, TDSDL);  
    }
  } /* end CELL_DCH check */
  
  /* States not supported */
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_FACH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_PCH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"URA_PCH not supported for TrCH");
    return FAILURE;
  }
  else
  {
    ERR_FATAL("Unknown state not supported for TrCH",0,0,0);
  }
  
  /***************************************************************/
  /* Transport Channel Count has been updated in RB Mapping info */
  /***************************************************************/
  if (!TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->tfs_SignallingMode,
    rrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_hsdsch) )
  {
    /* Check for the BLER */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation_r7,dch_QualityTarget))
    {
      /* Save the BLER if found, and is in range */
      if((ie_ptr->dch_QualityTarget.bler_QualityValue >= TDSRRCLLC_MIN_BLER) &&
        (ie_ptr->dch_QualityTarget.bler_QualityValue <= TDSRRCLLC_MAX_BLER))
      {
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val =
          (int8)ie_ptr->dch_QualityTarget.bler_QualityValue;
      }
      
      /* BLER is out of range */
      else
      {
        /* According to L1 I/F, must set an invalid value of -127 */
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL BLER %d is out of range",ie_ptr->dch_QualityTarget.bler_QualityValue);
        return FAILURE;
      }
    } /* end if BLER present */
    else
    {
      /* IE omitted. Set the target to -127 which would result 1% of BLER target.
       * This is good UE behaviour though the spec is ambiguous on what UE
       * behavior should be in this case.  
       */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DCH quality target not present. Setting to -127");
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
    }
  }
  else
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation_r7,dch_QualityTarget))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring dch quality target for HS");
    }
  }
  /* Indicate that DL L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  
  return SUCCESS;
}
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
  tdsrrc_DL_AddReconfTransChInfoList_r7 *dl_trch_add_reconfig_ptr,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r7  * ul_trch_add_reconfig_ptr  
)
{
  uint32 idx, tf_idx;
  /* Get the DL Added or Reconfigured Transport Channel information */
  if(dl_add_reconf_trch_info_list_present)
  {
      /* local UL TrCH Add Reconfig pointer */
  
    idx = 0;
    
    /* Save info for each additional Transport Channel */
    while((idx <= TDSUE_MAX_TRCH) && (dl_trch_add_reconfig_ptr->n > idx))
    {
      if(tdsrrcllc_get_dl_added_reconfig_trch_r7(&dl_trch_add_reconfig_ptr->elem[idx],
          ul_trch_add_reconfig_ptr
        ) == FAILURE)
      {
        return FAILURE;
      }
      /* get next TrCH Info */
      idx++;
    } /* end while */
  } 
  
  /* At this point store the Queue Information in L1 structure. To add later if HSDSCH config changed */
  
  /* Adjust TB size on every TrCH if necessary. */ 
  for (idx=0; idx<TDSUE_MAX_TRCH; idx++)
  {
  /* The number of logical channel mapped to this transport channel 
  increases from 1 to 2. Needs to adjsut the tb_szie to
    reflect the adding of the MAC header.                         */
    if (tdsordered_config_ptr->dl_dch_trch_info[idx].dch_present &&
      (tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up ||
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down))
    {
      /* TrCH IE is present and the TrCH ID is appear in the TrCH info table. */
      if (!(dl_add_reconf_trch_info_list_present &&
        tdsrrcllc_trch_id_in_config_trch_list_r7(dl_trch_add_reconfig_ptr,
        tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id)))
      {
        /* Added MAC head size to the current TB size.  */
        if (tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up &&
          !tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Increasing TrCH %d TB size,%d by MAC hdr size",tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id,tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[0]->tb_size);
          for (tf_idx=0; tf_idx<tdsordered_config_ptr->dl_trch_info[idx].num_tf; tf_idx++)
          {
            if (tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size > 0)
            {
              tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size += 
               TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
            }
            
          }
        }
        /* Substract MAC head size from the current TB size.  */
        else if (!tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up &&
          tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Decreasing TrCH %d TB size,%d by MAC hdr size",tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id,tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[0]->tb_size);
          for (tf_idx=0; tf_idx<tdsordered_config_ptr->dl_trch_info[idx].num_tf; tf_idx++)
          {
            if (tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size > 0)
            {
              tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size -= 
                TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
            }
          }
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clean TB size adjust flag in RBSetup");
      /* Reset lg_ch_cnt_up and lg_ch_cnt_down. */
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up = FALSE;
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down = FALSE;
    }
    else if (!tdsordered_config_ptr->dl_dch_trch_info[idx].dch_present)
    {
    /* Since the activated transport channels are sorted before stored in
      the memory, break the looping after reaching the 1st invalid block.  */
      break;
    }
  }
  
  /* Check to make sure DL TFS count is within UE Capabilities */
  if(tdsrrcllc_check_dl_tfs_cnt() == FAILURE)
  {
    return FAILURE;
  }

  return(SUCCESS);
}
  
/*=============================================================================
FUNCTION: tdsrrcllc_get_dl_deleted_trch_r7()

DESCRIPTION:
  This function takes a pointer to the Downlink Deleted 
  Transport Channel IE and Deletes if from Ordered Config.
  In the TFS it resets all information for this TrCH.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
===============================================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_deleted_trch_r7
(
  tdsrrc_DL_TransportChannelIdentity_r7* ie_ptr
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
,uint8 *dl_delete_Mac_ehs_Queue_Id
#endif
)
{
  /* TrCH Index */
  uint8 ix;
  uint8 i;
  if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,rrc_DL_TrCH_TypeId2_r7_dch))
  {
    ix = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch);
    if(ix == TDSRRCLLC_INVALID_TRCH_IDX)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"No DL DCH index for TrCH ID %d",ie_ptr->dl_TransportChannelType.u.dch);
      return(SUCCESS);
    }
    tdsrrcllc_backup_rbmapping_info(ix,TDSDL_DCH_RB_MAPPING );

    /* Clear the RB Mapping info for this TrCH */
    tdsrrcllc_init_dl_dch_rb_mapping_info(tdsordered_config_ptr, ix);
    
    /* Clear the TFS for this TrCH */
    tdsrrcllc_init_dl_tfs_table(tdsordered_config_ptr, ix);
    
    /* Update MAC count */
    tdsordered_config_ptr->mac_dl_parms.num_dchs--;
    /* Initialize local data */
    tdsordered_config_ptr->dl_dch_trch_info[ix].dch_present = FALSE;
    tdsordered_config_ptr->dl_dch_trch_info[ix].trch_id     = 0;
  
    /* Indicate that the old index is now available */
    if(tdsrrcllc_deallocate_dl_dch_trch_idx(ix) == FAILURE)
    {
      return FAILURE;
    }
    /* Update L1 count */
    tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch--;
    /* set tdsdl_tfcs_needed flag to true.  Later if common info is not preset,
     * UE needs to reject the message 
     */
    tdsdl_tfcs_needed = TRUE;
  }
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,rrc_DL_TrCH_TypeId2_r7_hsdsch))
  {
    if(ie_ptr->dl_TransportChannelType.u.hsdsch->t == T_tdsrrc_DL_TrCH_TypeId2_r7_hsdsch_mac_hs )
    {
      TDSRRC_MSG1(MSG_LEGACY_LOW,"RRCHS:Delete DFLOW %d",ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_hs);
      if ((ix = tdsrrcllc_find_dflow_index((uint8)ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_hs)) >= UE_MAX_MAC_D_FLOW)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Unable to index for DFLOW %d",ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_hs);
        /* Allow the Reconfig Msg to be processed since spec allows it implicitly */
        return(SUCCESS);
      }
      /* Clear the Queue Information associated with the DFLOW*/
      tdsrrcllc_backup_rbmapping_info(ix,TDSMAC_D_RB_MAPPING );
      
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Initialize Queue for DFLOW %d",ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_hs);
      /* Initialize the queue info */
      tdsrrcllc_initialize_queue_info(ix);
  
      tdshsdpa_msg_params.msg_params_present = TRUE;
  
      tdshsdpa_msg_params.hsdsch_info_present = TRUE;
     /* If the Flow id was actually not deleted, that is the Rb-Mapping for this flow id is still present in the 
      * ordered config then delete it from Backup info
      */
      if ((ix = tdsrrcllc_find_dflow_index((uint8)ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_hs)) == UE_MAX_MAC_D_FLOW)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Mac-d flow id %d got deleted ",(ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_hs));
      }
      else
      {
        for(i=0;i<tdsordered_config_ptr->mac_dflow_info[ix].ndlchan;i++)
        {
          tdsrrcllc_free_hanging_rb_mapping_info_index(tdsordered_config_ptr->mac_dflow_info[ix].dlchan_info[i].rb_id, TDSMAC_D_RB_MAPPING);
        }
      }
    }
    else if(ie_ptr->dl_TransportChannelType.u.hsdsch->t == T_tdsrrc_DL_TrCH_TypeId2_r7_hsdsch_mac_ehs )
    {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS

       tdshsdpa_msg_params.msg_params_present = TRUE;
  
      tdshsdpa_msg_params.hsdsch_info_present = TRUE;

      *dl_delete_Mac_ehs_Queue_Id |= 1 << (ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_ehs);

      if (tdsrrcllc_delete_queue_from_macehs(ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_ehs) == FAILURE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"MACEHS:Delete queue %d not found",ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_ehs);
        return SUCCESS;
      }
#else
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported  ");
        return(FAILURE);
#endif
    }
    
  }  
  else  /* Must be FACH */
  {
    // ix = tdsrrcllc_find_dl_fach_trch_idx(ie_ptr->value.dl_TransportChannelIdentity); 
    ix = tdsrrcllc_find_dl_fach_trch_idx();
    if(ix == TDSRRCLLC_INVALID_TRCH_IDX)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No FACH index for TrCH ID ");
      return(FAILURE);
    }
    tdsrrcllc_backup_rbmapping_info(ix,TDSFACH_RB_MAPPING );

    /* Clear the RB Mapping info for this TrCH */
    tdsrrcllc_init_fach_rb_mapping_info(tdsordered_config_ptr, ix);
    
    /* Clear the TFS for this TrCH */
    tdsrrcllc_init_dl_tfs_table(tdsordered_config_ptr, ix);
    
    /* Update MAC count */
    tdsordered_config_ptr->mac_dl_parms.num_fachs--;

    /* Update L1 count */
    tdsordered_config_ptr->l1_dl_cctrch_parms.num_trch--;
  }
  
  return SUCCESS;
}

#ifdef FEATURE_TDSCDMA_HSUPA
/*================================================================================================
FUNCTION: tdsrrcllc_process_added_reconfig_e_dch_info_r7()

DESCRIPTION:
  This function takes a pointer to the EDCH Added or Reconfigured 
  mac-d flow info and copies it to TDSORDERED_CONFIG.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:

===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_process_added_reconfig_e_dch_info_r7
(
  tdsrrc_UL_AddReconfTransChInformation_r7 * ie_ptr
)
{

  /* local transport channel variables */
  uint8 e_mac_d_flow_idx = TDSMAX_E_DCH_MAC_D_FLOW;
  tdsrrc_E_DCH_AddReconf_MAC_d_FlowList_r7 * e_mac_d_flow_list_ptr = NULL;
  uint32 idx=0;
  if (!(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_AddReconfTransChInformation_r7_e_dch)))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Un-wanted call to function to process e_dch_info. t: %d",ie_ptr->t);
    return(FAILURE);
  }

  if(ie_ptr->u.e_dch->modeSpecific.t == T_tdsrrc_UL_AddReconfTransChInformation_r7_modeSpecific_tdd )
  {
    /*We store e-tti & harq_rv_config which are trch i.e in L1 data base,
    *hence init them when previously EUL was not active
    */
    if(tdscurrent_config_ptr->e_dch_transmission == FALSE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCEUL:init in l1 db,prv harq_rv_config=%d",tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config);

      tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_NOT_PRESENT;
    }

  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-DCH mode specific choise is FDD mode");
    return FAILURE;
  }
  /* Update the harq info if present */
  if (ie_ptr->u.e_dch->harq_Info == tdsrrc_UL_AddReconfTransChInformation_r7_harq_Info_rv0)
  {
    tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_0;
  }
  else
  {
    /* Default value is 'rvtable', in which case the UE shall use an RSN
       based RV index as specified in 25.212*/
    tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_TABLE;
  }

  if (tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config !=
      tdscurrent_config_ptr->l1_e_info.e_ul_info.harq_rv_config) 
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCEUL: Setting UL-L1 action cuz Harq Info RECONFIG");

    tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_HARQ_RV_INFO_INCL;
    tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);
  }
  
  /* Update Add-Recong mac-d flow info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->u.e_dch,tdsrrc_UL_AddReconfTransChInformation_r7_e_dch,addReconf_MAC_d_FlowList))
  {       
    e_mac_d_flow_list_ptr = &ie_ptr->u.e_dch->addReconf_MAC_d_FlowList;
  }
  idx=0;
  /* Update Add-Recong mac-d flow info */
  while ((e_mac_d_flow_list_ptr != NULL) && (e_mac_d_flow_list_ptr->n > idx))
  {       
    if ((e_mac_d_flow_idx = tdsrrcllc_find_e_mac_d_flow_idx(
           (uint8)e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowIdentity)) == TDSMAX_E_DCH_MAC_D_FLOW)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: E-DCH mac-d flow ID %d,no idx found",e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowIdentity);
      return FAILURE;
    }
    else
    {
      if(tdsrrcllc_restore_ul_emac_rb_mapping_info(e_mac_d_flow_idx) == FAILURE)
      {
        /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: E-DCH mac-d flow ID %d,not correct info found in Stanalone rb-mapping info",e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowIdentity);
      }
      tdsrrcllc_update_mac_eul_action(TRUE);
      /* Only allowed to be absent when already defined for this E-DCH MAC-d flow*/
      if (e_mac_d_flow_list_ptr->elem[idx].m.mac_d_FlowPowerOffsetPresent)
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
          e_mac_d_flow_power_offset = e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowPowerOffset;
      }
      else
      {
        if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
            e_mac_d_flow_power_offset > TDSRRCLLC_MAX_E_MAC_D_FLOW_PWR_OFFSET)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d flow pwr offset needs to be provided");
          return FAILURE;
        }
      }
      /* Only allowed to be absent when already defined for this E-DCH MAC-d flow */
      if (e_mac_d_flow_list_ptr->elem[idx].m.mac_d_FlowMaxRetransPresent)
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
          e_mac_d_flow_max_num_of_retx = e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowMaxRetrans;
      }
      else
      {
        if(tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
           e_mac_d_flow_max_num_of_retx > TDSRRCLLC_MAX_E_MAC_D_FLOW_NUM_RETX)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d max num of retx needs to be provided");
          return FAILURE;
        }
      }

      /* This IE is mandatory present for 1.28Mcps TDD, */
      if (e_mac_d_flow_list_ptr->elem[idx].m.mac_d_FlowRetransTimerPresent)
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_mac_d_flow_retx_tmr = 
            tdsRetransTimerForSI[e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowRetransTimer];
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d Flow Retrans Timer absent,This IE is mandatory present for 1.28Mcps TDD");
        return FAILURE;
      }
      
      /* if no mux list is provided, use the previous one.  If there is no previous then allow multiplexing of 
         MAC-d PDU's from the E-DCH MAC-d flow indicated in the IE "E-DCH MAC-d flow identity" with MAC-d PDU's 
         from any other E-DCH MAC-d flow in the same MAC-e PDU.*/
      if (e_mac_d_flow_list_ptr->elem[idx].m.mac_d_FlowMultiplexingListPresent)
      {
        /* Only look at first byte of bitstring as max e_mac_d flow is 8*/
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_mac_d_flow_mux_list =
          tdsrrcllcpcie_convert_asn1_oss_bitstring_to_uint8(
            &e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowMultiplexingList);
      }
      else
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_mac_d_flow_mux_list = 0xFF;
      }
      
      if (e_mac_d_flow_list_ptr->elem[idx].m.transmissionGrantTypePresent)
      {
        if (e_mac_d_flow_list_ptr->elem[idx].transmissionGrantType.t == 
            T_tdsrrc_E_DCH_AddReconf_MAC_d_Flow_r7_transmissionGrantType_non_ScheduledTransGrantInfo)
        {
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
            e_tx_grant_info = TDSMAC_E_TX_GRANT_NON_SCHEDULED;

          if(e_mac_d_flow_list_ptr->elem[idx].transmissionGrantType.u.non_ScheduledTransGrantInfo->modeSpecificInfo.t ==
            T_tdsrrc_E_DCH_AddReconf_MAC_d_Flow_r7_modeSpecificInfo_fdd)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d unsupprt FDD mode");
            return FAILURE;
          }
        }
        else /* rrc_scheduledTransmissionGrantInfo */
        {
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_tx_grant_info = TDSMAC_E_TX_GRANT_SCHEDULED;
        }
      }
    }

    idx++;  
  }

  return SUCCESS;
}


#endif /*FEATURE_TDSCDMA_HSUPA*/
/*================================================================================================
FUNCTION: tdsrrcllc_get_ul_added_reconfig_trch_r7()

DESCRIPTION:
  This function takes a pointer to the Uplink Added or Reconfigured 
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:
  CELL_DCH additions are the only item supported now.
===============================================================================================*/
static uecomdef_status_e_type 
tdsrrcllc_get_ul_added_reconfig_trch_r7
(
  tdsrrc_UL_AddReconfTransChInformation_r7 * ie_ptr
)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL Add/Reconf TrCH");

  if(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_AddReconfTransChInformation_r7_dch_usch))
  {
      if(tdsrrcllc_dch_usch_add_trch_info((tr_ch_id_type)ie_ptr->u.dch_usch->transportChannelIdentity,
                              &ie_ptr->u.dch_usch->transportFormatSet) == FAILURE)
      {
        return FAILURE;
      }
   
    }
#ifdef FEATURE_TDSCDMA_HSUPA
  else if(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_AddReconfTransChInformation_r7_e_dch) )
  {
    if (FAILURE == tdsrrcllc_process_added_reconfig_e_dch_info_r7(ie_ptr))
    {
      return FAILURE;
    }
  }
#else
  else /* tdsrrc_UL_AddReconfTransChInformation_r6_e_dch */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: tdsrrc_UL_AddReconfTransChInformation_r6_e_dch not yet supported");
  }
#endif /* FEATURE_TDSCDMA_HSUPA */
  return SUCCESS;
}
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
)
{
  uint32 idx =0,idy=0;
  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;
  
  tdsrrc_UL_DeletedTransChInfoList_r6 *temp_ul_trch_delete_ptr = ul_deleted_trch_list_ptr;
  tdsrrc_UL_AddReconfTransChInfoList_r7  *temp_ul_trch_add_reconfig_ptr = ul_trch_add_reconfig_ptr;

  if((temp_ul_trch_delete_ptr != NULL) && (temp_ul_trch_add_reconfig_ptr != NULL))
  {
    while(temp_ul_trch_delete_ptr->n > idx)
    {
      if(T_tdsrrc_UL_TransportChannelIdentity_r6_dch_usch == temp_ul_trch_delete_ptr->elem[idx].t)
      {
        idy = 0;
        while(temp_ul_trch_add_reconfig_ptr->n > idy)
        {
          if(T_tdsrrc_UL_AddReconfTransChInformation_r7_dch_usch == temp_ul_trch_add_reconfig_ptr->elem[idy].t)
          {
            if(temp_ul_trch_delete_ptr->elem[idx].u.dch_usch->ul_TransportChannelIdentity == temp_ul_trch_add_reconfig_ptr->elem[idy].u.dch_usch->transportChannelIdentity)
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Same Trch ID found in add/reconfig list %d and deleted list %d ",temp_ul_trch_add_reconfig_ptr->elem[idy].u.dch_usch->transportChannelIdentity,temp_ul_trch_delete_ptr->elem[idx].u.dch_usch->ul_TransportChannelIdentity);
              return FAILURE;
            }
          }
          idy++;
        }
      }   
      idx++;
    }
  }
  idx = 0;
  idy = 0;
  // KW error
  if ((ul_deleted_trch_info_list_present)&&(ul_deleted_trch_list_ptr != NULL))
  {
    while (ul_deleted_trch_list_ptr->n > idx)
    {
      /* Remove this transport channel from MAC, L1 and local data.
       * and update transport channel count in L1, MAC and local data.
       */
      if(tdsrrcllc_get_ul_deleted_trch_info_list_r6(&ul_deleted_trch_list_ptr->elem[idx]) == FAILURE)
      {
        return FAILURE;
      }
      /* get next TrCH Info */
      idx++;
    } /* end while */
    
#ifdef FEATURE_TDSCDMA_HSUPA
        idx = 0;
        /*Fill the holes only when Ul EDCH Trch is deleted*/
        while(ul_deleted_trch_list_ptr->n > idx) 
        {
          if (T_tdsrrc_UL_TransportChannelIdentity_r6_e_dch == ul_deleted_trch_list_ptr->elem[idx].t)
          {
            tdsrrcllc_fill_holes_in_e_mac_d_flow_idx();
            break;
          }
          idx++;
        }
#endif
  }
  idx =0;
  // KW error
  if ((ul_add_recfg_trch_info_list_present)&&(ul_trch_add_reconfig_ptr != NULL))
  {
    /* Get the UL Added/Reconfigured Info */
    while(ul_trch_add_reconfig_ptr->n > idx) 
    {
      if (T_tdsrrc_UL_AddReconfTransChInformation_r7_dch_usch ==
          ul_trch_add_reconfig_ptr->elem[idx].t)
      {
        tdstrch_id = (tr_ch_id_type)
                  ul_trch_add_reconfig_ptr->elem[idx].u.dch_usch->transportChannelIdentity;
  
        if((tdsrrcllc_get_ul_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't find idx,TrCH ID %d",tdstrch_id);
          return(FAILURE);
        }
      }
#ifdef FEATURE_TDSCDMA_HSUPA
      else if ((T_tdsrrc_UL_AddReconfTransChInformation_r7_e_dch == 
        ul_trch_add_reconfig_ptr->elem[idx].t) && 
               (ul_trch_add_reconfig_ptr->elem[idx].u.e_dch->m.addReconf_MAC_d_FlowListPresent)) 
      {
        tdsrrc_E_DCH_AddReconf_MAC_d_FlowList_r7 * e_dch_mac_d_flow_list_ptr = NULL;
        idy =0;

        e_dch_mac_d_flow_list_ptr = &ul_trch_add_reconfig_ptr->elem[idx].u.e_dch->addReconf_MAC_d_FlowList;

        while (e_dch_mac_d_flow_list_ptr->n > idy)
        {
          if((tdsrrcllc_get_e_mac_dflow_idx((uint8)e_dch_mac_d_flow_list_ptr->elem[idy].
              mac_d_FlowIdentity)) == TDSMAX_E_DCH_MAC_D_FLOW)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: Couldn't find idx for e-dch MAC-D flow: %d",e_dch_mac_d_flow_list_ptr->elem[idy].mac_d_FlowIdentity);
            return(FAILURE);
          }

          idy++;
        }

        tdsrrcllc_update_mac_eul_action(TRUE);
      }
#else
      else /* tdsrrc_UL_AddReconfTransChInformation_r6_e_dch */
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: R6 UL Trch Add Reconfig for E-DCH not yet supported");
      }
#endif /* FEATURE_TDSCDMA_HSUPA*/
      idx++;
    } /* while(...) */
  }

  /* At this point, the transport channel list -
   *  - may have holes due to deleted channels.
   *  - transport channel info may not be sorted by TrCH IDs.
   */ 

  /* Fill holes and put all Transport channels in the first
   * tdsordered_config_ptr->ul_trch_cnt indices.
   */
  tdsrrcllc_fill_holes_in_ul_dch_trch_idx();

#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrcllc_fill_holes_in_e_mac_d_flow_idx();
#endif /* FEATURE_TDSCDMA_HSUPA */

  /* Now is the time and place for sorting all transport channel
   * info based on TrCH IDs. Call the right function.
   */
  tdsrrcllc_sort_ul_dch_trch_index_array(&tdsordered_config_ptr->ul_dch_trch_info[0], 
                                      (TDSRRCLLC_UPLINK_TRCH_COUNT(tdsordered_config_ptr)));
  return(SUCCESS);
}

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
  tdsrrc_UL_AddReconfTransChInfoList_r7  * ul_trch_add_reconfig_ptr  
)
{
  uint8 idex;
  if(ul_add_recfg_trch_info_list_present)
  {
    uint32 idx = 0;
    
    /* Save info for each additional Transport Channel */
    while((idx <= TDSUE_MAX_TRCH) && (ul_trch_add_reconfig_ptr->n > idx))
    {
      if(tdsrrcllc_get_ul_added_reconfig_trch_r7(&ul_trch_add_reconfig_ptr->elem[idx]) == FAILURE)
      {
        return FAILURE;
      }
      
      /* get next TrCH Info */
      idx++;
    } /* end while */
  } /* end UL TrCH Add/Reconfig */
  
  /* Adjust MAC header  on every UL TrCH if necessary. */ 
  for (idex=0; idex<TDSUE_MAX_TRCH; idex++)
  {
  /* The number of logical channel mapped to this transport channel 
  increases from 1 to 2. Needs to adjsut the tb_szie to
    reflect the adding of the MAC header.                         */
    if (tdsordered_config_ptr->ul_dch_trch_info[idex].dch_present &&
      (tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up ||
      tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down))
    {
      if (!(ul_add_recfg_trch_info_list_present &&
         tdsrrcllc_find_trch_id_in_ul_trch_list_r7(ul_trch_add_reconfig_ptr,
        tdsordered_config_ptr->ul_dch_trch_info[idex].trch_id)))
      {
        /* Added MAC head size to the current TB size.  */
        if (tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up &&
          !tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Muti logic channel map to one transport channel ");
          tdsordered_config_ptr->ul_tfcs.tfs_info[idex].mac_header_size =
             TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
        }
        /* Substract MAC head size from the current TB size.  */
        else if (!tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up &&
          tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"One logic channel map to one transport channel");
          tdsordered_config_ptr->ul_tfcs.tfs_info[idex].mac_header_size = 
               TDSRRCLLC_DED_SINGLE_LOG_CHL_DCH_MAC_HDR_SIZE;
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clean TB size adjust flag in RBSetup");
      /* Reset lg_ch_cnt_up and lg_ch_cnt_down. */
      tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up = FALSE;
      tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down = FALSE;
    }
    else if (!tdsordered_config_ptr->ul_dch_trch_info[idex].dch_present)
    {
    /* Since the activated transport channels are sorted before stored in
      the memory, break the looping after reaching the 1st invalid block.  */
      break;
    }
  }

  /* Check to make sure UL TFS count is within UE Capabilities */
  if(tdsrrcllc_check_ul_tfs_cnt() == FAILURE)
  {
    return FAILURE;
  }

  return(SUCCESS);
}

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
)
{
  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;

  uint8 dflow_index;
  tdsrrc_MAC_hs_AddReconfQueue_List * dflow_queue_head_ptr = NULL;
  uint32 idx =0,idy =0;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  uint32 idz = 0;
  uint8 dl_delete_Mac_ehs_Queue_Id = 0;
  tdsrrc_HSDSCH_Info_r7 *temp_hsdsch;
  tdsrrc_MAC_ehs_AddReconfReordQ_List *temp_MAC_ehs_AddReconfReordQ_List;
#endif
  tdsrrc_DL_DeletedTransChInfoList_r7 *temp_dl_trch_delete_ptr = dl_trch_delete_ptr;
  tdsrrc_DL_AddReconfTransChInfoList_r7  *temp_dl_trch_add_reconfig_ptr = dl_trch_add_reconfig_ptr;

  if((temp_dl_trch_delete_ptr != NULL) && (temp_dl_trch_add_reconfig_ptr != NULL))
  {
    while(temp_dl_trch_delete_ptr->n > idx)
    {
      if(T_tdsrrc_DL_TrCH_TypeId2_r7_dch == temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.t)
      {
        idy = 0;
        while(temp_dl_trch_add_reconfig_ptr->n > idy)
        {
          if(T_tdsrrc_DL_TrCH_TypeId1_r5_dch == temp_dl_trch_add_reconfig_ptr->elem[idy].dl_TransportChannelType.t)
          {
            if(temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.u.dch == temp_dl_trch_add_reconfig_ptr->elem[idy].dl_TransportChannelType.u.dch)
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Same Trch ID found in add/reconfig list %d and deleted list %d ",temp_dl_trch_add_reconfig_ptr->elem[idy].dl_TransportChannelType.u.dch,temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.u.dch);
              return FAILURE;
            }
          }
          idy++;
        }
      }
      idx++;
    }
  }

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  /* 8.6.5.23  Added or reconfigured MAC-ehs reordering queue
  1>  if a MAC-ehs queue (identified by the IE "MAC-ehs queue Id") is included in both the IE "MAC-ehs queue to 
      add or reconfigure list" and the IE "DL HS-DSCH MAC-ehs reordering queue" within the IE "Deleted DL TrCH 
      information": 
      2>  the UE behaviour is unspecified. 
  */
  if((temp_dl_trch_delete_ptr != NULL) && (temp_dl_trch_add_reconfig_ptr != NULL))
  {
    for(idx=0;idx < temp_dl_trch_delete_ptr->n ;idx++)
    {
      if(temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.t != 
          T_tdsrrc_DL_TrCH_TypeId2_r7_hsdsch )
        continue;
      if(temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.u.hsdsch->t !=
          T_tdsrrc_DL_TrCH_TypeId2_r7_hsdsch_mac_ehs)
        continue;

      for(idy = 0;temp_dl_trch_add_reconfig_ptr->n > idy ;idy++)
      {
        if(temp_dl_trch_add_reconfig_ptr->elem[idy].tfs_SignallingMode.t != 
            T_tdsrrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_hsdsch)
          continue;
        
        temp_hsdsch = temp_dl_trch_add_reconfig_ptr->elem[idy].tfs_SignallingMode.u.hsdsch;
        if(!temp_hsdsch->m.dl_MAC_HeaderTypePresent)
          continue;
        if(temp_hsdsch->dl_MAC_HeaderType.t != T_tdsrrc_HSDSCH_Info_r7_dl_MAC_HeaderType_mac_ehs)
          continue;
        if(!temp_hsdsch->dl_MAC_HeaderType.u.mac_ehs->m.mac_ehs_AddReconfQueue_ListPresent)
          continue;
        temp_MAC_ehs_AddReconfReordQ_List = &temp_hsdsch->dl_MAC_HeaderType.u.mac_ehs->mac_ehs_AddReconfQueue_List;

        for(idz=0;temp_MAC_ehs_AddReconfReordQ_List->n > idz;idz++)
        {
          if(temp_MAC_ehs_AddReconfReordQ_List->elem[idz].mac_ehs_QueueId ==
             temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.u.hsdsch->u.mac_ehs)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Same MAC-ehs queue Id found in add/reconfig list and deleted list ",temp_dl_trch_delete_ptr->elem[idx].dl_TransportChannelType.u.hsdsch->u.mac_ehs);
            return FAILURE;
          }
        }
      }
    }
  }
#endif

  idx =0;
  idy =0;
  while((dl_trch_delete_ptr !=NULL) &&( dl_trch_delete_ptr->n > idx))
  {
  /* Remove this transport channel from MAC, L1 and local data.
  * and update transport channel count in L1, MAC and local data.
    */
    if(tdsrrcllc_get_dl_deleted_trch_r7(&dl_trch_delete_ptr->elem[idx]
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      ,&dl_delete_Mac_ehs_Queue_Id
#endif
    ) == FAILURE)
    {
      return FAILURE;
    }
    /* get next TrCH Info */
    idx++;
  } /* end while */
  idx =0;
  /* Get the DL Added/Reconfigured Info */
  while((dl_trch_add_reconfig_ptr !=NULL) && (
    dl_trch_add_reconfig_ptr->n > idx) )
  {
    /* Check if channel type is HSDPA */
    if (dl_trch_add_reconfig_ptr->elem[idx].dl_TransportChannelType.t == 
      T_tdsrrc_DL_TrCH_TypeId1_r5_hsdsch)
    {
      /* At least one HARQ/DFLOW has to be present */
      if ((dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.t == 
        T_tdsrrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_hsdsch)
        && ((dl_trch_add_reconfig_ptr->elem[idx].
        tfs_SignallingMode.u.hsdsch->m.dl_MAC_HeaderTypePresent)
        || (dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.
        u.hsdsch->m.harqInfoPresent)))
      {
        
        TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHSPA+:Valid DFLOW/MACEhs specified");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+:HARQ,DFLOW both ABS.");
        return FAILURE;
      }
      if(dl_trch_add_reconfig_ptr->elem[idx].
        tfs_SignallingMode.u.hsdsch->m.dl_MAC_HeaderTypePresent)
      {
        if(dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.
            hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r7_dl_MAC_HeaderType_mac_hs)
        {
          /* Now find the DFLOW Index for ADD/RECONFIG.  */
          if ((dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->
            dl_MAC_HeaderType.u.mac_hs->m.mac_hs_AddReconfQueue_ListPresent) &&
            (dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->
            dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List.n > 0))
          {
            dflow_queue_head_ptr = 
              &dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->
              dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List;
            idy =0;
            while (dflow_queue_head_ptr->n > idy)
            {
              if ((dflow_queue_head_ptr->elem[idy].mac_dFlowId > UE_MAX_MAC_D_FLOW-1) ||
                  (dflow_queue_head_ptr->elem[idy].mac_hsQueueId > UE_MAX_HS_QUEUE-1))
              {
                TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCHS:DFLOW Id %d, Queue Id %d,Range 0..7",dflow_queue_head_ptr->elem[idy].mac_dFlowId,dflow_queue_head_ptr->elem[idy].mac_hsQueueId);
                return FAILURE;
              }
    
              /* If DFLOW Index found then return it Else allocate a new index */
              if ((dflow_index = tdsrrcllc_get_dflow_index((uint8)dflow_queue_head_ptr->elem[idy].mac_dFlowId)) 
                == UE_MAX_MAC_D_FLOW)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:DFLOW index %d absent",dflow_index);
                return FAILURE;
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:DFlow Index is %d",dflow_index); 
                idy++;
              }
            } /* End of while */       
            
          } /* End of if of add queue present */
        }
        else if(dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.
            hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r7_dl_MAC_HeaderType_mac_ehs)
        {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
        if(tdsrrcllc_macehs_add_or_del_q_info(dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_ehs) == FAILURE)            
        {
           return FAILURE;
        }
#else
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported ");
#endif
        }
      }
    } /* End of if of rrc_DL_TrCH_TypeId1_r5_hsdsch chosen */
    else if (dl_trch_add_reconfig_ptr->elem[idx].dl_TransportChannelType.t == 
      T_tdsrrc_DL_TrCH_TypeId1_r5_dch)
    {
      tdstrch_id = (tr_ch_id_type)dl_trch_add_reconfig_ptr->elem[idx].dl_TransportChannelType.u.dch;
      
      if((tdsrrcllc_get_dl_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't find idx,TrCH ID %d",tdstrch_id);
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSCH unsupported");
      return FAILURE;
    }
    idx++;
  } /* while(...) */
  
    /* At this point, the transport channel list -
     *  - may have holes due to deleted channels.
     *  - transport channel info may not be sorted by TrCH IDs.
     */ 
  
  /* Fill holes and put all Transport channels in the first
   * tdsordered_config_ptr->dl_trch_cnt indices.
   */

  /* Now Remove holes in DFLOW Index */
  tdsrrcllc_rearrange_dflow_index();
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  tdsrrcllc_rearrange_macehs_queue_index(tdsordered_config_ptr);

#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
  
  tdsrrcllc_fill_holes_in_dl_dch_trch_idx();
  
  /* Now is the time and place for sorting all transport channel
  * info based on TrCH IDs. Call the right function.
  */
  tdsrrcllc_sort_dl_dch_trch_index_array(&tdsordered_config_ptr->dl_dch_trch_info[0], 
    (TDSRRCLLC_DOWNLINK_TRCH_COUNT(tdsordered_config_ptr)));
  
  
  return(SUCCESS);
}

#ifdef FEATURE_TDSCDMA_REL8
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
  tdsrrc_UL_AddReconfTransChInfoList_r8 *  ul_trch_add_reconfig_ptr,
  uint32 ul_deleted_trch_info_list_present,
  tdsrrc_UL_DeletedTransChInfoList_r6 * ul_deleted_trch_list_ptr
)
{
  uint32 idx =0,idy=0;
  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;
  
  tdsrrc_UL_DeletedTransChInfoList_r6 *temp_ul_trch_delete_ptr = ul_deleted_trch_list_ptr;
  tdsrrc_UL_AddReconfTransChInfoList_r8 *temp_ul_trch_add_reconfig_ptr = ul_trch_add_reconfig_ptr;

  if((temp_ul_trch_delete_ptr != NULL) && (temp_ul_trch_add_reconfig_ptr != NULL))
  {
    while(temp_ul_trch_delete_ptr->n > idx)
    {
      if(T_tdsrrc_UL_TransportChannelIdentity_r6_dch_usch == temp_ul_trch_delete_ptr->elem[idx].t)
      {
        idy = 0;
        while(temp_ul_trch_add_reconfig_ptr->n > idy)
        {
          if(T_tdsrrc_UL_AddReconfTransChInformation_r8_dch_usch == temp_ul_trch_add_reconfig_ptr->elem[idy].t)
          {
            if(temp_ul_trch_delete_ptr->elem[idx].u.dch_usch->ul_TransportChannelIdentity == temp_ul_trch_add_reconfig_ptr->elem[idy].u.dch_usch->transportChannelIdentity)
            {
              TDSRRC_MSG2(MSG_LEGACY_ERROR,"Same Trch ID found in add/reconfig list %d and deleted list %d ",temp_ul_trch_add_reconfig_ptr->elem[idy].u.dch_usch->transportChannelIdentity,temp_ul_trch_delete_ptr->elem[idx].u.dch_usch->ul_TransportChannelIdentity);
              return FAILURE;
            }
          }
          idy++;
       }
      }   
      idx++;
    }
  }
  idx =0;
  idy=0;
  // 2012/09/10 KW
  if ((ul_deleted_trch_info_list_present)&&(ul_deleted_trch_list_ptr != NULL))
  {
    while (ul_deleted_trch_list_ptr->n > idx)
    {
      /* Remove this transport channel from MAC, L1 and local data.
       * and update transport channel count in L1, MAC and local data.
       */
      if(tdsrrcllc_get_ul_deleted_trch_info_list_r6(&ul_deleted_trch_list_ptr->elem[idx]) == FAILURE)
      {
        return FAILURE;
      }
      /* get next TrCH Info */
      idx++;
    } /* end while */
#ifdef FEATURE_TDSCDMA_HSUPA
    idx = 0;
    /*Fill the holes only when Ul EDCH Trch is deleted*/
    while(ul_deleted_trch_list_ptr->n > idx) 
    {
      if (T_tdsrrc_UL_TransportChannelIdentity_r6_e_dch == ul_deleted_trch_list_ptr->elem[idx].t)
      {
        tdsrrcllc_fill_holes_in_e_mac_d_flow_idx();
        break;
      }
      idx++;
    }
#endif
  }
  idx =0;
  // 2012/09/10 KW
  if ((ul_add_recfg_trch_info_list_present)&&(ul_trch_add_reconfig_ptr != NULL))
  {
    /* Get the UL Added/Reconfigured Info */
    while(ul_trch_add_reconfig_ptr->n > idx) 
    {
      if (T_tdsrrc_UL_AddReconfTransChInformation_r8_dch_usch ==
          ul_trch_add_reconfig_ptr->elem[idx].t)
      {
        tdstrch_id = (tr_ch_id_type)
                  ul_trch_add_reconfig_ptr->elem[idx].u.dch_usch->transportChannelIdentity;
  
        if((tdsrrcllc_get_ul_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8: Couldn't find idx,TrCH ID %d",tdstrch_id);
          return(FAILURE);
        }
      }
#ifdef FEATURE_TDSCDMA_HSUPA
      else if (TDSRRC_CHECK_LIST_MSG_TYPE(ul_trch_add_reconfig_ptr,tdsrrc_UL_AddReconfTransChInformation_r8_e_dch))
      {
        if(ul_trch_add_reconfig_ptr->elem[idx].u.e_dch->m.ul_MAC_HeaderTypePresent)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: IE ul_MAC_HeaderType not supported yet!");
          return FAILURE;
        }      
        if(ul_trch_add_reconfig_ptr->elem[idx].u.e_dch->m.addReconf_MAC_d_FlowListPresent)
        {
          tdsrrc_E_DCH_AddReconf_MAC_d_FlowList_r7 * e_dch_mac_d_flow_list_ptr = NULL;

          e_dch_mac_d_flow_list_ptr = &ul_trch_add_reconfig_ptr->elem[idx].u.e_dch->addReconf_MAC_d_FlowList;
          idy =0;
          while (e_dch_mac_d_flow_list_ptr->n > idy)
          {
            if((tdsrrcllc_get_e_mac_dflow_idx((uint8)e_dch_mac_d_flow_list_ptr->elem[idy].
                mac_d_FlowIdentity)) == TDSMAX_E_DCH_MAC_D_FLOW)
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"REL8: Couldn't find idx for e-dch MAC-D flow: %d",e_dch_mac_d_flow_list_ptr->elem[idy].mac_d_FlowIdentity);
              return(FAILURE);
            }
            idy++;
          }
          tdsrrcllc_update_mac_eul_action(TRUE);
        }
      }
#else /*FEATURE_TDSCDMA_HSUPA*/
      else /* tdsrrc_UL_AddReconfTransChInformation_r6_e_dch */
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: R6 UL Trch Add Reconfig for E-DCH not yet supported");
      }
#endif /* FEATURE_TDSCDMA_HSUPA*/
      idx++;
    } /* while(...) */
  }

  /* At this point, the transport channel list -
   *  - may have holes due to deleted channels.
   *  - transport channel info may not be sorted by TrCH IDs.
   */ 

  /* Fill holes and put all Transport channels in the first
   * tdsordered_config_ptr->ul_trch_cnt indices.
   */
  tdsrrcllc_fill_holes_in_ul_dch_trch_idx();

#ifdef FEATURE_TDSCDMA_HSUPA
  tdsrrcllc_fill_holes_in_e_mac_d_flow_idx();
#endif /* FEATURE_TDSCDMA_HSUPA */

  /* Now is the time and place for sorting all transport channel
   * info based on TrCH IDs. Call the right function.
   */
  tdsrrcllc_sort_ul_dch_trch_index_array(&tdsordered_config_ptr->ul_dch_trch_info[0], 
                                      (TDSRRCLLC_UPLINK_TRCH_COUNT(tdsordered_config_ptr)));
  return(SUCCESS);
}

/*================================================================================================
FUNCTION: tdsrrcllc_process_added_reconfig_e_dch_info_r8()

DESCRIPTION:
  This function takes a pointer to the EDCH Added or Reconfigured 
  mac-d flow info and copies it to TDSORDERED_CONFIG.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:

===============================================================================================*/
static uecomdef_status_e_type tdsrrcllc_process_added_reconfig_e_dch_info_r8
(
  tdsrrc_UL_AddReconfTransChInformation_r8 * ie_ptr
)
{

  /* local transport channel variables */
  uint8 e_mac_d_flow_idx = TDSMAX_E_DCH_MAC_D_FLOW;
  tdsrrc_E_DCH_AddReconf_MAC_d_FlowList_r7 * e_mac_d_flow_list_ptr = NULL;
  uint32 idx =0;
  if (!(TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_AddReconfTransChInformation_r8_e_dch)))
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Un-wanted call to function to process e_dch_info. t: %d",ie_ptr->t);
    return(FAILURE);
  }

  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr->u.e_dch,ul_MAC_HeaderType))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: IE ul_MAC_HeaderType is not supported yet!");
    return FAILURE;
  }

  /*Update the TTI info in MAC and L1 struct. 10ms or 2ms */
  /* Check with stored TTI value before indicating reconfig to L1/MAC*/
  if(ie_ptr->u.e_dch->modeSpecific.t == T_tdsrrc_UL_AddReconfTransChInformation_r8_modeSpecific_tdd )
  {
    /*We store e-tti & harq_rv_config which are trch i.e in L1 data base,
    *hence init them when previously EUL was not active
    */
    if(tdscurrent_config_ptr->e_dch_transmission == FALSE)
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"RRCEUL:init in l1 db,prv harq_rv_config=%d",tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config);

      tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_NOT_PRESENT;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-DCH mode specific choise is FDD mode");
    return FAILURE;
  }
  /* Update the harq info if present */
  if (ie_ptr->u.e_dch->harq_Info == tdsrrc_UL_AddReconfTransChInformation_r8_harq_Info_rv0)
  {
    tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_0;
  }
  else
  {
    /* Default value is 'rvtable', in which case the UE shall use an RSN
       based RV index as specified in 25.212*/
    tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config = TDSL1_RV_TABLE;
  }

  if (tdsordered_config_ptr->l1_e_info.e_ul_info.harq_rv_config !=
      tdscurrent_config_ptr->l1_e_info.e_ul_info.harq_rv_config) 
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"REL8: Setting UL-L1 action cuz Harq Info RECONFIG");

    tdsordered_config_ptr->l1_e_info.e_ul_info.e_ul_info_mask |= TDSL1_HARQ_RV_INFO_INCL;

    tdsrrcllc_update_l1_e_req_mask_for_e_ul_info(TRUE);
  }

  /* Update Add-Recong mac-d flow info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->u.e_dch,
    tdsrrc_UL_AddReconfTransChInformation_r8_e_dch,addReconf_MAC_d_FlowList))
  {       
    e_mac_d_flow_list_ptr = &ie_ptr->u.e_dch->addReconf_MAC_d_FlowList;
  }
  idx =0;
  /* Update Add-Recong mac-d flow info */
  while ((e_mac_d_flow_list_ptr != NULL) && (e_mac_d_flow_list_ptr->n > idx))
  {       
    if ((e_mac_d_flow_idx = tdsrrcllc_find_e_mac_d_flow_idx(
           (uint8)e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowIdentity)) == TDSMAX_E_DCH_MAC_D_FLOW)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: E-DCH mac-d flow ID %d,no idx found",e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowIdentity);
      return FAILURE;
    }
    else
    {
      if(tdsrrcllc_restore_ul_emac_rb_mapping_info(e_mac_d_flow_idx) == FAILURE)
      {
        /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCEUL: E-DCH mac-d flow ID %d,not correct info found in Stanalone rb-mapping info",e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowIdentity);
      }
      tdsrrcllc_update_mac_eul_action(TRUE);
      /* Only allowed to be absent when already defined for this E-DCH MAC-d flow*/
      if (TDSRRC_MSG_LIST_BITMASK_IE(e_mac_d_flow_list_ptr,
        tdsrrc_E_DCH_AddReconf_MAC_d_Flow_r7,mac_d_FlowPowerOffset))
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
          e_mac_d_flow_power_offset = e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowPowerOffset;
      }
      else
      {
        if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
            e_mac_d_flow_power_offset > TDSRRCLLC_MAX_E_MAC_D_FLOW_PWR_OFFSET)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d flow pwr offset needs to be provided");
          return FAILURE;
        }
      }
      /* Only allowed to be absent when already defined for this E-DCH MAC-d flow */
      if (TDSRRC_MSG_LIST_BITMASK_IE(e_mac_d_flow_list_ptr,tdsrrc_E_DCH_AddReconf_MAC_d_Flow_r7,
        mac_d_FlowMaxRetrans))
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
          e_mac_d_flow_max_num_of_retx = e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowMaxRetrans;
      }
      else
      {
        if(tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
           e_mac_d_flow_max_num_of_retx > TDSRRCLLC_MAX_E_MAC_D_FLOW_NUM_RETX)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d max num of retx needs to be provided");
          return FAILURE;
        }
      }

      /* This IE is mandatory present for 1.28Mcps TDD, */
      if (e_mac_d_flow_list_ptr->elem[idx].m.mac_d_FlowRetransTimerPresent)
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_mac_d_flow_retx_tmr = 
            tdsRetransTimerForSI[e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowRetransTimer];
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d Flow Retrans Timer absent,This IE is mandatory present for 1.28Mcps TDD");
        return FAILURE;
      }

      /* if no mux list is provided, use the previous one.  If there is no previous then allow multiplexing of 
         MAC-d PDU's from the E-DCH MAC-d flow indicated in the IE "E-DCH MAC-d flow identity" with MAC-d PDU's 
         from any other E-DCH MAC-d flow in the same MAC-e PDU.*/
      if (TDSRRC_MSG_LIST_BITMASK_IE(e_mac_d_flow_list_ptr,
        tdsrrc_E_DCH_AddReconf_MAC_d_Flow_r7,mac_d_FlowMultiplexingList))
      {
        /* Only look at first byte of bitstring as max e_mac_d flow is 8*/
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_mac_d_flow_mux_list =
          tdsrrcllcpcie_convert_asn1_oss_bitstring_to_uint8(
            &e_mac_d_flow_list_ptr->elem[idx].mac_d_FlowMultiplexingList);
      }
      else
      {
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_mac_d_flow_mux_list = 0xFF;
      }
      
      if (TDSRRC_MSG_LIST_BITMASK_IE(e_mac_d_flow_list_ptr,
        tdsrrc_E_DCH_AddReconf_MAC_d_Flow_r7,transmissionGrantType))
      {
        if (e_mac_d_flow_list_ptr->elem[idx].transmissionGrantType.t == 
            T_tdsrrc_E_DCH_AddReconf_MAC_d_Flow_r7_transmissionGrantType_non_ScheduledTransGrantInfo)
        {
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].
            e_tx_grant_info = TDSMAC_E_TX_GRANT_NON_SCHEDULED;
          
          if(e_mac_d_flow_list_ptr->elem[idx].transmissionGrantType.u.non_ScheduledTransGrantInfo->modeSpecificInfo.t ==
            T_tdsrrc_E_DCH_AddReconf_MAC_d_Flow_r7_modeSpecificInfo_fdd)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d unsupprt FDD mode");
            return FAILURE;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: E-MAC-d TDD mode is NULL");
          }
        }
        else /* rrc_scheduledTransmissionGrantInfo */
        {
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[e_mac_d_flow_idx].e_tx_grant_info = TDSMAC_E_TX_GRANT_SCHEDULED;
        }
      }
    }

    idx++;
  }

  return SUCCESS;
}

/*================================================================================================
FUNCTION: tdsrrcllc_get_ul_added_reconfig_trch_r8()

DESCRIPTION:
  This function takes a pointer to the Uplink Added or Reconfigured 
  Transport Channels IE and copies it to TDSORDERED_CONFIG.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS if the IE conenets are valid
                FALSE   otherwise

SIDE EFFECTS:
  CELL_DCH additions are the only item supported now.
===============================================================================================*/
static uecomdef_status_e_type 
tdsrrcllc_get_ul_added_reconfig_trch_r8
(
  tdsrrc_UL_AddReconfTransChInformation_r8 * ie_ptr
)
{
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get UL Add/Reconf TrCH");

  if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_AddReconfTransChInformation_r8_dch_usch))
  {
      if(tdsrrcllc_dch_usch_add_trch_info((tr_ch_id_type)ie_ptr->u.dch_usch->transportChannelIdentity,
                              &ie_ptr->u.dch_usch->transportFormatSet) == FAILURE)
      {
        return FAILURE;
      }
   
    }
#ifdef FEATURE_TDSCDMA_HSUPA
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE_PTR(ie_ptr,rrc_UL_AddReconfTransChInformation_r8_e_dch)) 
  {
    if (FAILURE == tdsrrcllc_process_added_reconfig_e_dch_info_r8(ie_ptr))
    {
      return FAILURE;
    }
  }
#else
  else /* tdsrrc_UL_AddReconfTransChInformation_r6_e_dch */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: tdsrrc_UL_AddReconfTransChInformation_r8_e_dch not yet supported");
  }
#endif /* FEATURE_TDSCDMA_HSUPA */
  return SUCCESS;
}

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
)
{
  uint8 idex;
  if(ul_add_recfg_trch_info_list_present)
  {
    uint32 idx = 0;
    
    /* Save info for each additional Transport Channel */
    while((idx <= TDSUE_MAX_TRCH) && (ul_trch_add_reconfig_ptr->n > idx))
    {
      if(tdsrrcllc_get_ul_added_reconfig_trch_r8(&ul_trch_add_reconfig_ptr->elem[idx]) == FAILURE)
      {
        return FAILURE;
      }
      
      /* get next TrCH Info */
      idx++;
      
    } /* end while */
  } /* end UL TrCH Add/Reconfig */
  
  /* Adjust MAC header  on every UL TrCH if necessary. */ 
  for (idex=0; idex<TDSUE_MAX_TRCH; idex++)
  {
  /* The number of logical channel mapped to this transport channel 
  increases from 1 to 2. Needs to adjsut the tb_szie to
    reflect the adding of the MAC header.                         */
    if (tdsordered_config_ptr->ul_dch_trch_info[idex].dch_present &&
      (tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up ||
      tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down))
    {
      if (!(ul_add_recfg_trch_info_list_present &&
         tdsrrcllc_find_trch_id_in_ul_trch_list_r8(ul_trch_add_reconfig_ptr,
        tdsordered_config_ptr->ul_dch_trch_info[idex].trch_id)))
      {
        /* Added MAC head size to the current TB size.  */
        if (tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up &&
          !tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Muti logic channel map to one transport channel ");
          tdsordered_config_ptr->ul_tfcs.tfs_info[idex].mac_header_size =
             TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
        }
        /* Substract MAC head size from the current TB size.  */
        else if (!tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up &&
          tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"One logic channel map to one transport channel");
          tdsordered_config_ptr->ul_tfcs.tfs_info[idex].mac_header_size = 
               TDSRRCLLC_DED_SINGLE_LOG_CHL_DCH_MAC_HDR_SIZE;
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clean TB size adjust flag in RBSetup");
      /* Reset lg_ch_cnt_up and lg_ch_cnt_down. */
      tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_up = FALSE;
      tdsordered_config_ptr->ul_dch_trch_info[idex].lg_ch_cnt_down = FALSE;
    }
    else if (!tdsordered_config_ptr->ul_dch_trch_info[idex].dch_present)
    {
    /* Since the activated transport channels are sorted before stored in
      the memory, break the looping after reaching the 1st invalid block.  */
      break;
    }
  }

  /* Check to make sure UL TFS count is within UE Capabilities */
  if(tdsrrcllc_check_ul_tfs_cnt() == FAILURE)
  {
    return FAILURE;
  }

  return(SUCCESS);
}

/*================================================================================================
FUNCTION: tdsrrcllc_get_dl_added_reconfig_trch_r8()

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
static uecomdef_status_e_type tdsrrcllc_get_dl_added_reconfig_trch_r8
(
  tdsrrc_DL_AddReconfTransChInformation_r7 * ie_ptr,
  tdsrrc_UL_AddReconfTransChInfoList_r8 * ul_ptr_r8
 
)
{
  /* local transport channel variables */
  tr_ch_id_type trch_idx = 0;
  tr_ch_id_type ul_trch_idx = 0;
  uint32 idx=0,idy=0;


  /* Pointers to UL and DL TFS info for use when copying existing config */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info = NULL;
  tdsl1_ul_semi_static_parms_struct_type *ul_tfs_ss_info = NULL;
  tdsmac_ul_tfs_type *ul_tfs_dyn_info = NULL;
  
  /* Loop variables for copying UL TFS info */
  uint8 tf_idx = 0;
  tdsl1_dl_cctrchcfg_tf_info_struct_type *rrc_dl_tf_info = NULL;
  tdsmac_ul_tf_type *ul_tf_info = NULL;
  
  /* Boolean used to indicate if multiple logical channels are mapped to
  a given TrCH. */
  boolean       multiple_ded_log_chan = FALSE;
  
  /* local variable indicating whether or not an uplink TrCH ID was found */
  boolean       got_one = FALSE;
  
  tdsrrc_MAC_hs_AddReconfQueue_List* add_queue_list_ptr = NULL;
  
  tdsrrc_MAC_hs_DelQueue_List* delete_queue_list_ptr = NULL;
  
  
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Add/Reconf TrCH");
  
  /*
  * Use the Transport Channel Index and next state to place the TFS in 
  * the correct slot in the MAC TFS array.
  */
  if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
  {
    /* Get the transport channel index for this transport channel ID.
      * Note: The index should already be there. */
    
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_hsdsch))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:HS TrCh to be added");
    }
    else
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_dch))
      {
        
        trch_idx = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch);
        
        if(trch_idx == TDSRRCLLC_INVALID_TRCH_IDX)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,no idx found",ie_ptr->dl_TransportChannelType.u.dch);
          return(FAILURE);
        }

        if(tdsrrcllc_restore_dl_dch_rb_mapping_info(trch_idx) == FAILURE)
        {
          /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,not correct in Standalone RB-Mapping",ie_ptr->dl_TransportChannelType.u.dch);
        } 

      }
      
      /* Determine if multiple dedicated logical channels are present on this TrCH id */
      if(tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt > 1)
      {
        multiple_ded_log_chan = TRUE;
      }
    }
    /*
      * Get whether or not the TFS for a given TRCH is included or if it needs
      * to come from an uplink TrCH ID.
      */
    switch(ie_ptr->tfs_SignallingMode.t)
    {
      /* TFS explicitly given for this Transport Channel */
      case T_tdsrrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_explicit_config:
        
        /* get the Transport Format Set for this TrCH */
        if(tdsrrcllc_get_dl_transport_format_set(ie_ptr->tfs_SignallingMode.u.explicit_config,
          &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
          multiple_ded_log_chan, /* mulitple logical channels? */
          &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
          ) == FAILURE)
        {
          return FAILURE;
        }
        break; /* end explicit TFS signalling for DL TrCH */
        
      case T_tdsrrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_sameAsULTrCH:
        idx =0;
        if (ul_ptr_r8 == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"invalid:UL TFS absent when DL TFS choice 'same as UL'");
          return(FAILURE);
        }
        while(ul_ptr_r8->n > idx)
        {
          /* check whether trCh is of type tdsrrc_UL_AddReconfTransChInformation_r6_dch_usch */
          if((TDSRRC_CHECK_LIST_MSG_TYPE(ul_ptr_r8,tdsrrc_UL_AddReconfTransChInformation_r8_dch_usch)) &&
             (ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity == 
              ul_ptr_r8->elem[idx].u.dch_usch->transportChannelIdentity))
          { 
            /* Got a match with Uplink TrCH ID */
            TDSRRC_MSG2(MSG_LEGACY_LOW,"Get TFS for DL TrCH Id %d using UL TrCH ID %d",ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity,ul_ptr_r8->elem[idx].u.dch_usch->transportChannelIdentity);
            if(tdsrrcllc_get_dl_transport_format_set(&ul_ptr_r8->elem[idx].u.dch_usch->transportFormatSet,
              &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
              multiple_ded_log_chan, /* mulitple logical channels? */
              &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
              ) == FAILURE)
            {
              return FAILURE;
            }
            
            /* Indicate that found an UL TrCH ID, and break the while loop */
            got_one = TRUE;
            break;
          } /* end UL TrCH match */
          
          /* no match, keep looking */
          else 
          {
            idx++;
          }
        }
               
        /* Make sure we found the UL TrCH, before continuing */
        if(!got_one)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No matching UL TFS in msg,look up from OC");
          /* Get the transport channel index for the UL transport channel ID.
          * Note: The index should already be there.
          */
          ul_trch_idx = tdsrrcllc_find_ul_dch_trch_idx(
            tdsordered_config_ptr,
            (tr_ch_id_type)ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
            ul_TransportChannelIdentity);
          if(ul_trch_idx == TDSRRCLLC_INVALID_TRCH_IDX)
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"No DL TFS for TrCH %d, to use UL TrCH %d for TFS",
              ie_ptr->dl_TransportChannelType.u.dch,
              ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
              ul_TransportChannelIdentity, 0);
            return(FAILURE);
          }
          
          /* Set up convenience pointers */
          dl_tfs_info = &tdsordered_config_ptr->dl_trch_info[trch_idx];
          ul_tfs_ss_info = &tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[ul_trch_idx];
          ul_tfs_dyn_info = &tdsordered_config_ptr->ul_tfcs.tfs_info[ul_trch_idx];
          
          /* Copy semistatic info from uplink */
          switch(ul_tfs_ss_info->coding)
          {
            case TDSL1_UNCODED:
              dl_tfs_info->coding_type = TDSL1_DL_UNCODED;
              break;
            case TDSL1_CONV_1_BY_2_CODING:
              dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_2_CODING;
              break;
            case TDSL1_CONV_1_BY_3_CODING:
              dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_3_CODING;
              break;
            case TDSL1_TURBO_CODING:
              dl_tfs_info->coding_type = TDSL1_DL_TURBO_CODING;
              break;
            default:
              break; 
          }
          
          
          dl_tfs_info->rate_match_attrib = ul_tfs_ss_info->staticRM;
          switch(ul_tfs_ss_info->crc)
          {
            case TDSL1_CRC_LENGTH_0:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_0;
              break;
            case TDSL1_CRC_LENGTH_8:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_8;
              break;
            case TDSL1_CRC_LENGTH_12:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_12;
              break;
            case TDSL1_CRC_LENGTH_16:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_16;
              break;
            case TDSL1_CRC_LENGTH_24:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_24;
              break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CRC length %d in UL",ul_tfs_ss_info->crc);
              return FAILURE;
          }
          
          /* Copy dynamic info from uplink */
          dl_tfs_info->tti_value = ul_tfs_dyn_info->tti_info;
          dl_tfs_info->num_tf = (uint8) ul_tfs_dyn_info->ntf;
          for(tf_idx = 0; tf_idx < dl_tfs_info->num_tf; tf_idx++)
          {
            rrc_dl_tf_info = dl_tfs_info->tf_info_array[tf_idx];
            ul_tf_info = &ul_tfs_dyn_info->tf_info[tf_idx];
            
            rrc_dl_tf_info->num_tb = (uint8)ul_tf_info->ntblks;
            rrc_dl_tf_info->tb_size = ul_tf_info->tb_size;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"TB Size: %d,multiple_ded_log_chan: %d",rrc_dl_tf_info->tb_size,multiple_ded_log_chan);

            /* If number of logical channel is > 1, inc tb_size by 4 to accomodate MAC-header */
            if ( (rrc_dl_tf_info->tb_size > 0)&& (multiple_ded_log_chan == TRUE) )
            {
              rrc_dl_tf_info->tb_size += TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"New TB Size %d",rrc_dl_tf_info->tb_size);
            }
          }
        }
        break; /* end use of previous signalled UL TrCH TFS for DL TrCH */ 
        
      case T_tdsrrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_hsdsch:
        /* The DFLOW Index has already been added. So get the index */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->tfs_SignallingMode.u.hsdsch,
            rrc,dl_MAC_HeaderType))
        {
          if (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r7_dl_MAC_HeaderType_mac_hs)
          {
  
            TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:DFLOW present for Add/Reconfig Dflow");
              
            
            
            /* Check if add & delete queue both are present and network specifies the same 
            queue id in both. This needs to be rejected */
            if ((ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->m.
               mac_hs_AddReconfQueue_ListPresent) &&
              (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List.n > 0) &&
              (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->m.
              mac_hs_DelQueue_ListPresent) &&
              (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_DelQueue_List.n > 0))
            {
              TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Add&Delete queue present");
              /* Check if UTRAN messed up by specifying the same queue id */
              delete_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_DelQueue_List;
              idx =0;
              while (delete_queue_list_ptr->n > idx)
              {
                add_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List;
                idy =0;
                while (add_queue_list_ptr->n > idy)
                {
                  if (delete_queue_list_ptr->elem[idx].mac_hsQueueId == add_queue_list_ptr->elem[idy].mac_hsQueueId)
                  {
                    /* Invalid Config */
                    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Queue %d simul added & deleted",delete_queue_list_ptr->elem[idx].mac_hsQueueId);
                    return FAILURE;
                  }
                  idy++;
                }
                
                idx++;
              }
              
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Queue check passed");
            }
            
            if (tdsrrcllc_set_dflow_info(ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs) == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid DFLOW info");
              return FAILURE;
            }
          }
          else if (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r7_dl_MAC_HeaderType_mac_ehs)
          {
  #ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS: MACEHS info received");
  #else
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported ");
  #endif
          }
        }
        /* SET HARQ info in L1 structure */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->tfs_SignallingMode.u.hsdsch,
            tdsrrc_HSDSCH_Info_r7,harqInfo))
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:HARQ info present");
          if (tdsrrcllc_set_harq_information_r7(&ie_ptr->tfs_SignallingMode.u.hsdsch->harqInfo) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set HARQ information");
            return FAILURE;
          }
        }
        
        /* Indicate that HSDSCH Info is present which would be used later to set bit mask when HSDPA
        is already ACTIVE */
        tdshsdpa_msg_params.hsdsch_info_present = TRUE;
        
        tdshsdpa_msg_params.msg_params_present = TRUE;
        
        break;
        
      default:
        return FAILURE;
      
    } /* end TFS selection switch */

    /*Find Rbs mapped to this DCH and fill them in rb_list_mapped_to_dch if their RLC
        PDU size has changed.*/ 
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,rrc_DL_TrCH_TypeId1_r5_dch))
    {
      TDSRRC_MSG1(MSG_LEGACY_LOW,"REL8: DL DCH TrCH id %d",(tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch);
      tdsrrcllc_get_rb_for_rb_list_mapped_to_dch((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch, 
                tdsordered_config_ptr->rrc_state_indicator, TDSDL);  
    }
  } /* end CELL_DCH check */
  
  /* States not supported */
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_FACH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_PCH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"URA_PCH not supported for TrCH");
    return FAILURE;
  }
  else
  {
    ERR_FATAL("Unknown state not supported for TrCH",0,0,0);
  }
  
  /***************************************************************/
  /* Transport Channel Count has been updated in RB Mapping info */
  /***************************************************************/
  if (!TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->tfs_SignallingMode,
    rrc_DL_AddReconfTransChInformation_r7_tfs_SignallingMode_hsdsch) )
  {
    /* Check for the BLER */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
        tdsrrc_DL_AddReconfTransChInformation_r7,dch_QualityTarget))
    {
      /* Save the BLER if found, and is in range */
      if((ie_ptr->dch_QualityTarget.bler_QualityValue >= TDSRRCLLC_MIN_BLER) &&
        (ie_ptr->dch_QualityTarget.bler_QualityValue <= TDSRRCLLC_MAX_BLER))
      {
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val =
          (int8)ie_ptr->dch_QualityTarget.bler_QualityValue;
      }
      
      /* BLER is out of range */
      else
      {
        /* According to L1 I/F, must set an invalid value of -127 */
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL BLER %d is out of range",ie_ptr->dch_QualityTarget.bler_QualityValue);
        return FAILURE;
      }
    } /* end if BLER present */
    else
    {
      /* IE omitted. Set the target to -127 which would result 1% of BLER target.
       * This is good UE behaviour though the spec is ambiguous on what UE
       * behavior should be in this case.  
       */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DCH quality target not present. Setting to -127");
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
    }
  }
  else
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation_r7,dch_QualityTarget))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring dch quality target for HS");
    }
  }
  /* Indicate that DL L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  
  return SUCCESS;
}

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
  tdsrrc_DL_AddReconfTransChInfoList_r7 * dl_trch_add_reconfig_ptr,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r8  * ul_trch_add_reconfig_ptr  
)
{
  uint32 idx, tf_idx;
  /* Get the DL Added or Reconfigured Transport Channel information */
  if(dl_add_reconf_trch_info_list_present)
  {
      
    idx = 0;
    
    /* Save info for each additional Transport Channel */
    while((idx <= TDSUE_MAX_TRCH) && (dl_trch_add_reconfig_ptr->n > idx))
    {
      if(tdsrrcllc_get_dl_added_reconfig_trch_r8(&dl_trch_add_reconfig_ptr->elem[idx],
          ul_trch_add_reconfig_ptr
        ) == FAILURE)
      {
        return FAILURE;
      }
      /* get next TrCH Info */
      idx++;
    } /* end while */
  } 
  
  /* At this point store the Queue Information in L1 structure. To add later if HSDSCH config changed */
  
  /* Adjust TB size on every TrCH if necessary. */ 
  for (idx=0; idx<TDSUE_MAX_TRCH; idx++)
  {
  /* The number of logical channel mapped to this transport channel 
  increases from 1 to 2. Needs to adjsut the tb_szie to
    reflect the adding of the MAC header.                         */
    if (tdsordered_config_ptr->dl_dch_trch_info[idx].dch_present &&
      (tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up ||
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down))
    {
      /* TrCH IE is present and the TrCH ID is appear in the TrCH info table. */
      if (!(dl_add_reconf_trch_info_list_present &&
        tdsrrcllc_trch_id_in_config_trch_list_r7(dl_trch_add_reconfig_ptr,
        tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id)))
      {
        /* Added MAC head size to the current TB size.  */
        if (tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up &&
          !tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Increasing TrCH %d TB size,%d by MAC hdr size",tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id,tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[0]->tb_size);
          for (tf_idx=0; tf_idx<tdsordered_config_ptr->dl_trch_info[idx].num_tf; tf_idx++)
          {
            if (tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size > 0)
            {
              tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size += 
              TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
            }
            
          }
        }
        /* Substract MAC head size from the current TB size.  */
        else if (!tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up &&
          tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Decreasing TrCH %d TB size,%d by MAC hdr size",tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id,tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[0]->tb_size);
          for (tf_idx=0; tf_idx<tdsordered_config_ptr->dl_trch_info[idx].num_tf; tf_idx++)
          {
            if (tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size > 0)
            {
              tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size -= 
                TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
            }
          }
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clean TB size adjust flag in RBSetup");
      /* Reset lg_ch_cnt_up and lg_ch_cnt_down. */
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up = FALSE;
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down = FALSE;
    }
    else if (!tdsordered_config_ptr->dl_dch_trch_info[idx].dch_present)
    {
    /* Since the activated transport channels are sorted before stored in
      the memory, break the looping after reaching the 1st invalid block.  */
      break;
    }
  }
  
  /* Check to make sure DL TFS count is within UE Capabilities */
  if(tdsrrcllc_check_dl_tfs_cnt() == FAILURE)
  {
    return FAILURE;
  }

  return(SUCCESS);
}

#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
/*====================================================================
FUNCTION: tdsrrcllc_get_hs_window_size_r9()

DESCRIPTION:
  
DEPENDENCIES:
  Gets the window size associated with the queue.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  
====================================================================*/
static uint8 tdsrrcllc_get_hs_window_size_r9
(
  tdsrrc_MAC_hs_WindowSize_r9 mac_hsWindowSize_r9
)
{
  uint8 w_size = 0;

  switch (mac_hsWindowSize_r9)
  {
    case tdsrrc_MAC_hs_WindowSize_r9_mws4 : 
      w_size = 4;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_r9_mws6 :
      w_size = 6;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_r9_mws8:
      w_size = 8;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_r9_mws12:
      w_size = 12;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_r9_mws16:
      w_size = 16;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_r9_mws24:
      w_size = 24;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_r9_mws32:
      w_size = 32;
      break;
      
    case tdsrrc_MAC_hs_WindowSize_r9_mw64:
    case tdsrrc_MAC_hs_WindowSize_r9_mw128:
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:WindowSize does not support ");
       break;
    default: /* Not Possible */
      w_size = 0;
      break;
      
  }
  TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:WindowSize %d",w_size);
  return w_size;
}

/*====================================================================
FUNCTION: tdsrrcllc_add_macehs_r9()

DESCRIPTION:
  
DEPENDENCIES:


RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
  
====================================================================*/
static uecomdef_status_e_type tdsrrcllc_add_macehs_r9
(
 tdsrrc_AddOrReconfMAC_ehs_ReordQ_r9 * mac_ehs_info_r9,
 uint8 dl_delete_Mac_ehs_Queue_Id
)
{
  tdsrrc_MAC_ehs_AddReconfReordQ_List_r9 * macehs_queue_head_ptr = NULL;
  uint8 macehs_q_id = UE_MAX_HS_QUEUE;
  uint32 idx =0;
  
  if (mac_ehs_info_r9 == NULL)
  {
    ERR_FATAL("Null pointer received",0,0,0);
    return FAILURE;
  }
    /* Now find the MAC EHS Index for ADD/RECONFIG.  */
  if ((TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mac_ehs_info_r9,mac_ehs_AddReconfQueue_List))
     &&(mac_ehs_info_r9->mac_ehs_AddReconfQueue_List.n > 0))
  {
    macehs_queue_head_ptr = &mac_ehs_info_r9->mac_ehs_AddReconfQueue_List;
    for (idx = 0; idx < macehs_queue_head_ptr->n; idx++)
    {
      if (macehs_queue_head_ptr->elem[idx].mac_ehs_QueueId > UE_MAX_HS_QUEUE-1)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"MACEHS:MAC EHS Q Id %d,Range 0..7",macehs_queue_head_ptr->elem[idx].mac_ehs_QueueId);
        return FAILURE;
      }

      /* If Queue Index found then return it Else allocate a new index */
      if ((macehs_q_id = tdsrrcllc_get_macehs_queue_index((uint8)macehs_queue_head_ptr->elem[idx].mac_ehs_QueueId)) 
        == UE_MAX_HS_QUEUE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"MACEHS: MAC EHS Q index %d absent",macehs_q_id);
        return FAILURE;
      }
      else
      {
        /* Check if the IE "Deleted DL TrCH information" is included in the received message; and 
              a MAC-ehs queue is included in both the IE "MAC-ehs queue to add or reconfigure list" 
              and the IE "DL HS-DSCH MAC-ehs reordering queue" within the IE "Deleted DL TrCH information":
              return failure */
        if (dl_delete_Mac_ehs_Queue_Id & (0x01 << tdsordered_config_ptr->mac_ehs_queue_info[macehs_q_id].queue_id))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS: MAC-ehs queue Id %d simul added & deleted",dl_delete_Mac_ehs_Queue_Id);
          return FAILURE;
        }
        
        if ((tdsrrc_MAC_hs_WindowSize_r9_mw64 == macehs_queue_head_ptr->elem[idx].mac_ehsWindowSize)
        || (tdsrrc_MAC_hs_WindowSize_r9_mw128 == macehs_queue_head_ptr->elem[idx].mac_ehsWindowSize))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"MACEHS: MAC EHS window size does not support %d",macehs_queue_head_ptr->elem[idx].mac_ehsWindowSize);
          return FAILURE;
        }
#ifdef FEATURE_TDSCDMA_64QAM
        tdsordered_config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL9:64QAM is being started/continued");
#endif

        tdsordered_config_ptr->mac_ehs_queue_info[macehs_q_id].window_size = 
          tdsrrcllc_get_hs_window_size_r9(macehs_queue_head_ptr->elem[idx].mac_ehsWindowSize);
      
        tdsordered_config_ptr->mac_ehs_queue_info[macehs_q_id].release_timer = 
          tdsrrcllc_get_hs_release_timer(macehs_queue_head_ptr->elem[idx].reorderingReleaseTimer);

         /*PSR TBD  reorderingResetTimer   in 7.6       */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"MACEHS: MAC EHS Q index is %d",macehs_q_id); 
      }
    } /* End of for */       
    
  }
  return SUCCESS;
}

/*============================================================================
FUNCTION: tdsrrcllc_macehs_add_or_del_q_info_r9()

DESCRIPTION:
  
      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllc_macehs_add_or_del_q_info_r9
( 
  tdsrrc_AddOrReconfMAC_ehs_ReordQ_r9 * mac_ehs_info_r9,
  uint8 dl_delete_Mac_ehs_Queue_Id
)
{  
  if (mac_ehs_info_r9 == NULL)
  {
    ERR_FATAL("Null pointer received",0,0,0);
    return FAILURE;
  }
  /* Get the add/reconfig MAC D FLOW Info */
  if ( (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(mac_ehs_info_r9,
     mac_ehs_AddReconfQueue_List)) &&
      (mac_ehs_info_r9->mac_ehs_AddReconfQueue_List.n > 0))
  {
    if(tdsrrcllc_add_macehs_r9(mac_ehs_info_r9, dl_delete_Mac_ehs_Queue_Id) == FAILURE)
    {
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Add/Reconfig Queue absent");
    return SUCCESS;
  }
    
  return SUCCESS;
}
#endif

/*============================================================================
FUNCTION: tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r9()

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
tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r9
(
  /* local DL TrCH Add Reconfig pointer */
  tdsrrc_DL_AddReconfTransChInfoList_r9 * dl_trch_add_reconfig_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList_r7    * dl_trch_delete_ptr
)
{
  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;

  uint8 dflow_index;
  tdsrrc_MAC_hs_AddReconfQueue_List * dflow_queue_head_ptr = NULL;
  uint32 idx =0,idy =0;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  uint8  dl_delete_Mac_ehs_Queue_Id = 0;
#endif

  while((dl_trch_delete_ptr !=NULL) &&( dl_trch_delete_ptr->n > idx))
  {
  /* Remove this transport channel from MAC, L1 and local data.
  * and update transport channel count in L1, MAC and local data.
    */
    if(tdsrrcllc_get_dl_deleted_trch_r7(&dl_trch_delete_ptr->elem[idx]
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      ,&dl_delete_Mac_ehs_Queue_Id
#endif
      ) == FAILURE)
    {
      return FAILURE;
    }
    /* get next TrCH Info */
    idx++;
  } /* end while */
  idx =0;
  /* Get the DL Added/Reconfigured Info */
  while((dl_trch_add_reconfig_ptr !=NULL) && (
    dl_trch_add_reconfig_ptr->n > idx) )
  {
    /* Check if channel type is HSDPA */
    if (dl_trch_add_reconfig_ptr->elem[idx].dl_TransportChannelType.t == 
      T_tdsrrc_DL_TrCH_TypeId1_r5_hsdsch)
    {
      /* At least one HARQ/DFLOW has to be present */
      if ((dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.t == 
        T_tdsrrc_DL_AddReconfTransChInformation_r9_tfs_SignallingMode_hsdsch)
        && ((dl_trch_add_reconfig_ptr->elem[idx].
        tfs_SignallingMode.u.hsdsch->m.dl_MAC_HeaderTypePresent)
        || (dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.
        u.hsdsch->m.harqInfoPresent)))
      {
        
        TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHSPA+:Valid DFLOW/MACEhs specified");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+:HARQ,DFLOW both ABS.");
        return FAILURE;
      }
      if(dl_trch_add_reconfig_ptr->elem[idx].
        tfs_SignallingMode.u.hsdsch->m.dl_MAC_HeaderTypePresent)
      {
        if(dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.
          u.hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r9_dl_MAC_HeaderType_mac_hs)
        {
          /* Now find the DFLOW Index for ADD/RECONFIG.  */
          if ((dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->
            dl_MAC_HeaderType.u.mac_hs->m.mac_hs_AddReconfQueue_ListPresent) &&
            (dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->
            dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List.n > 0))
          {
            dflow_queue_head_ptr = 
              &dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->
              dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List;
            idy =0;
            while (dflow_queue_head_ptr->n > idy)
            {
              if ((dflow_queue_head_ptr->elem[idy].mac_dFlowId > UE_MAX_MAC_D_FLOW-1) ||
                  (dflow_queue_head_ptr->elem[idy].mac_hsQueueId > UE_MAX_HS_QUEUE-1))
              {
                TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCHS:DFLOW Id %d, Queue Id %d,Range 0..7",dflow_queue_head_ptr->elem[idy].mac_dFlowId,dflow_queue_head_ptr->elem[idy].mac_hsQueueId);
                return FAILURE;
              }
    
              /* If DFLOW Index found then return it Else allocate a new index */
              if ((dflow_index = tdsrrcllc_get_dflow_index((uint8)dflow_queue_head_ptr->elem[idy].mac_dFlowId)) 
                == UE_MAX_MAC_D_FLOW)
              {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:DFLOW index %d absent",dflow_index);
                return FAILURE;
              }
              else
              {
                TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:DFlow Index is %d",dflow_index); 
                idy++;
              }
            } /* End of while */       
            
          } /* End of if of add queue present */
        }
        else if(dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.
            hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r9_dl_MAC_HeaderType_mac_ehs)
        {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
        if(tdsrrcllc_macehs_add_or_del_q_info_r9(dl_trch_add_reconfig_ptr->elem[idx].tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_ehs,
               dl_delete_Mac_ehs_Queue_Id) == FAILURE)            
        {
           return FAILURE;
        }
#else
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported ");
#endif
        }
      }
    } /* End of if of rrc_DL_TrCH_TypeId1_r5_hsdsch chosen */
    else if (dl_trch_add_reconfig_ptr->elem[idx].dl_TransportChannelType.t == 
      T_tdsrrc_DL_TrCH_TypeId1_r5_dch)
    {
      tdstrch_id = (tr_ch_id_type)dl_trch_add_reconfig_ptr->elem[idx].dl_TransportChannelType.u.dch;
      
      if((tdsrrcllc_get_dl_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't find idx,TrCH ID %d",tdstrch_id);
        return(FAILURE);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSCH unsupported");
      return FAILURE;
    }
    idx++;
  } /* while(...) */
  
    /* At this point, the transport channel list -
     *  - may have holes due to deleted channels.
     *  - transport channel info may not be sorted by TrCH IDs.
     */ 
  
  /* Fill holes and put all Transport channels in the first
   * tdsordered_config_ptr->dl_trch_cnt indices.
   */

  /* Now Remove holes in DFLOW Index */
  tdsrrcllc_rearrange_dflow_index();
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  tdsrrcllc_rearrange_macehs_queue_index(tdsordered_config_ptr);

#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/
  
  tdsrrcllc_fill_holes_in_dl_dch_trch_idx();
  
  /* Now is the time and place for sorting all transport channel
  * info based on TrCH IDs. Call the right function.
  */
  tdsrrcllc_sort_dl_dch_trch_index_array(&tdsordered_config_ptr->dl_dch_trch_info[0], 
    (TDSRRCLLC_DOWNLINK_TRCH_COUNT(tdsordered_config_ptr)));
  
  
  return(SUCCESS);
}
/*================================================================================================
FUNCTION: tdsrrcllc_get_dl_added_reconfig_trch_r9()

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
static uecomdef_status_e_type tdsrrcllc_get_dl_added_reconfig_trch_r9
(
  tdsrrc_DL_AddReconfTransChInformation_r9 * ie_ptr,
  tdsrrc_UL_AddReconfTransChInfoList_r8 * ul_ptr_r8
 
)
{
  /* local transport channel variables */
  tr_ch_id_type trch_idx = 0;
  tr_ch_id_type ul_trch_idx = 0;
  uint32 idx=0,idy=0;


  /* Pointers to UL and DL TFS info for use when copying existing config */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info = NULL;
  tdsl1_ul_semi_static_parms_struct_type *ul_tfs_ss_info = NULL;
  tdsmac_ul_tfs_type *ul_tfs_dyn_info = NULL;
  
  /* Loop variables for copying UL TFS info */
  uint8 tf_idx = 0;
  tdsl1_dl_cctrchcfg_tf_info_struct_type *rrc_dl_tf_info = NULL;
  tdsmac_ul_tf_type *ul_tf_info = NULL;
  
  /* Boolean used to indicate if multiple logical channels are mapped to
  a given TrCH. */
  boolean       multiple_ded_log_chan = FALSE;
  
  /* local variable indicating whether or not an uplink TrCH ID was found */
  boolean       got_one = FALSE;
  
  tdsrrc_MAC_hs_AddReconfQueue_List* add_queue_list_ptr = NULL;
  
  tdsrrc_MAC_hs_DelQueue_List* delete_queue_list_ptr = NULL;
  
  
  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Add/Reconf TrCH");
  
  /*
  * Use the Transport Channel Index and next state to place the TFS in 
  * the correct slot in the MAC TFS array.
  */
  if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
  {
    /* Get the transport channel index for this transport channel ID.
      * Note: The index should already be there. */
    
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_hsdsch))
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:HS TrCh to be added");
    }
    else
    {
      if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,
        rrc_DL_TrCH_TypeId1_r5_dch))
      {
        
        trch_idx = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch);
        
        if(trch_idx == TDSRRCLLC_INVALID_TRCH_IDX)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,no idx found",ie_ptr->dl_TransportChannelType.u.dch);
          return(FAILURE);
        }

        if(tdsrrcllc_restore_dl_dch_rb_mapping_info(trch_idx) == FAILURE)
        {
          /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,not correct in Standalone RB-Mapping",ie_ptr->dl_TransportChannelType.u.dch);
        } 

      }
      
      /* Determine if multiple dedicated logical channels are present on this TrCH id */
      if(tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt > 1)
      {
        multiple_ded_log_chan = TRUE;
      }
    }
    /*
      * Get whether or not the TFS for a given TRCH is included or if it needs
      * to come from an uplink TrCH ID.
      */
    switch(ie_ptr->tfs_SignallingMode.t)
    {
      /* TFS explicitly given for this Transport Channel */
      case T_tdsrrc_DL_AddReconfTransChInformation_r9_tfs_SignallingMode_explicit_config:
        
        /* get the Transport Format Set for this TrCH */
        if(tdsrrcllc_get_dl_transport_format_set(ie_ptr->tfs_SignallingMode.u.explicit_config,
          &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
          multiple_ded_log_chan, /* mulitple logical channels? */
          &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
          ) == FAILURE)
        {
          return FAILURE;
        }
        break; /* end explicit TFS signalling for DL TrCH */
        
      case T_tdsrrc_DL_AddReconfTransChInformation_r9_tfs_SignallingMode_sameAsULTrCH:
        idx =0;
        if (ul_ptr_r8 == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"invalid:UL TFS absent when DL TFS choice 'same as UL'");
          return(FAILURE);
        }
        while(ul_ptr_r8->n > idx)
        {
          /* check whether trCh is of type tdsrrc_UL_AddReconfTransChInformation_r6_dch_usch */
          if((TDSRRC_CHECK_LIST_MSG_TYPE(ul_ptr_r8,tdsrrc_UL_AddReconfTransChInformation_r8_dch_usch)) &&
             (ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity == 
              ul_ptr_r8->elem[idx].u.dch_usch->transportChannelIdentity))
          { 
            /* Got a match with Uplink TrCH ID */
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Get TFS for DL TrCH Id %d using UL TrCH ID %d",ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity,ul_ptr_r8->elem[idx].u.dch_usch->transportChannelIdentity);
            if(tdsrrcllc_get_dl_transport_format_set(&ul_ptr_r8->elem[idx].u.dch_usch->transportFormatSet,
              &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
              multiple_ded_log_chan, /* mulitple logical channels? */
              &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
              ) == FAILURE)
            {
              return FAILURE;
            }
            
            /* Indicate that found an UL TrCH ID, and break the while loop */
            got_one = TRUE;
            break;
          } /* end UL TrCH match */
          
          /* no match, keep looking */
          else 
          {
            idx++;
          }
        }
               
        /* Make sure we found the UL TrCH, before continuing */
        if(!got_one)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No matching UL TFS in msg,look up from OC");
          /* Get the transport channel index for the UL transport channel ID.
          * Note: The index should already be there.
          */
          ul_trch_idx = tdsrrcllc_find_ul_dch_trch_idx(
            tdsordered_config_ptr,
            (tr_ch_id_type)ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
            ul_TransportChannelIdentity);
          if(ul_trch_idx == TDSRRCLLC_INVALID_TRCH_IDX)
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"No DL TFS for TrCH %d, to use UL TrCH %d for TFS",
              ie_ptr->dl_TransportChannelType.u.dch,
              ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
              ul_TransportChannelIdentity, 0);
            return(FAILURE);
          }
          
          /* Set up convenience pointers */
          dl_tfs_info = &tdsordered_config_ptr->dl_trch_info[trch_idx];
          ul_tfs_ss_info = &tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[ul_trch_idx];
          ul_tfs_dyn_info = &tdsordered_config_ptr->ul_tfcs.tfs_info[ul_trch_idx];
          
          /* Copy semistatic info from uplink */
          switch(ul_tfs_ss_info->coding)
          {
            case TDSL1_UNCODED:
              dl_tfs_info->coding_type = TDSL1_DL_UNCODED;
              break;
            case TDSL1_CONV_1_BY_2_CODING:
              dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_2_CODING;
              break;
            case TDSL1_CONV_1_BY_3_CODING:
              dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_3_CODING;
              break;
            case TDSL1_TURBO_CODING:
              dl_tfs_info->coding_type = TDSL1_DL_TURBO_CODING;
              break;
            default:
              break; 
          }
          
          
          dl_tfs_info->rate_match_attrib = ul_tfs_ss_info->staticRM;
          switch(ul_tfs_ss_info->crc)
          {
            case TDSL1_CRC_LENGTH_0:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_0;
              break;
            case TDSL1_CRC_LENGTH_8:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_8;
              break;
            case TDSL1_CRC_LENGTH_12:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_12;
              break;
            case TDSL1_CRC_LENGTH_16:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_16;
              break;
            case TDSL1_CRC_LENGTH_24:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_24;
              break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CRC length %d in UL",ul_tfs_ss_info->crc);
              return FAILURE;
          }
          
          /* Copy dynamic info from uplink */
          dl_tfs_info->tti_value = ul_tfs_dyn_info->tti_info;
          dl_tfs_info->num_tf = (uint8) ul_tfs_dyn_info->ntf;
          for(tf_idx = 0; tf_idx < dl_tfs_info->num_tf; tf_idx++)
          {
            rrc_dl_tf_info = dl_tfs_info->tf_info_array[tf_idx];
            ul_tf_info = &ul_tfs_dyn_info->tf_info[tf_idx];
            
            rrc_dl_tf_info->num_tb = (uint8)ul_tf_info->ntblks;
            rrc_dl_tf_info->tb_size = ul_tf_info->tb_size;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"TB Size: %d,multiple_ded_log_chan: %d",rrc_dl_tf_info->tb_size,multiple_ded_log_chan);

            /* If number of logical channel is > 1, inc tb_size by 4 to accomodate MAC-header */
            if ( (rrc_dl_tf_info->tb_size >0)&&(multiple_ded_log_chan == TRUE) )
            {
              rrc_dl_tf_info->tb_size += TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"New TB Size %d",rrc_dl_tf_info->tb_size);
            }
          }
        }
        break; /* end use of previous signalled UL TrCH TFS for DL TrCH */ 
        
      case T_tdsrrc_DL_AddReconfTransChInformation_r9_tfs_SignallingMode_hsdsch:
        /* The DFLOW Index has already been added. So get the index */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->tfs_SignallingMode.u.hsdsch,
            tdsrrc_HSDSCH_Info_r9,dl_MAC_HeaderType))
        {
          if (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r9_dl_MAC_HeaderType_mac_hs)
          {
  
            TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:DFLOW present for Add/Reconfig Dflow");
              
            
              
            /* Check if add & delete queue both are present and network specifies the same 
            queue id in both. This needs to be rejected */
            if ((ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->m.
               mac_hs_AddReconfQueue_ListPresent) &&
              (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List.n > 0) &&
              (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->m.
              mac_hs_DelQueue_ListPresent) &&
              (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_DelQueue_List.n > 0))
            {
              TDSRRC_MSG0(MSG_LEGACY_LOW,"RRCHS:Add&Delete queue present");
              /* Check if UTRAN messed up by specifying the same queue id */
              delete_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_DelQueue_List;
              idx =0;
              while (delete_queue_list_ptr->n > idx)
              {
                add_queue_list_ptr = &ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs->mac_hs_AddReconfQueue_List;
                idy =0;
                while (add_queue_list_ptr->n > idy)
                {
                  if (delete_queue_list_ptr->elem[idx].mac_hsQueueId == add_queue_list_ptr->elem[idy].mac_hsQueueId)
                  {
                    /* Invalid Config */
                    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Queue %d simul added & deleted",delete_queue_list_ptr->elem[idx].mac_hsQueueId);
                    return FAILURE;
                  }
                  idy++;
                }
                
                idx++;
              }
              
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCHS:Queue check passed");
            }
            
            if (tdsrrcllc_set_dflow_info(ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.u.mac_hs) == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid DFLOW info");
              return FAILURE;
            }
          }
          else if (ie_ptr->tfs_SignallingMode.u.hsdsch->dl_MAC_HeaderType.t == T_tdsrrc_HSDSCH_Info_r9_dl_MAC_HeaderType_mac_ehs)
          {
  #ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS: MACEHS info received");
  #else
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported ");
  #endif
          }
        }
        /* SET HARQ info in L1 structure */
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(ie_ptr->tfs_SignallingMode.u.hsdsch,
            tdsrrc_HSDSCH_Info_r9,harqInfo))
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"RRCHS:HARQ info present");
          if (tdsrrcllc_set_harq_information_r7(&ie_ptr->tfs_SignallingMode.u.hsdsch->harqInfo) == FAILURE)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Unable to set HARQ information");
            return FAILURE;
          }
        }
        
        /* Indicate that HSDSCH Info is present which would be used later to set bit mask when HSDPA
        is already ACTIVE */
        tdshsdpa_msg_params.hsdsch_info_present = TRUE;
        
        tdshsdpa_msg_params.msg_params_present = TRUE;
        
        break;
        
      default:
        return FAILURE;
      
    } /* end TFS selection switch */

    /*Find Rbs mapped to this DCH and fill them in rb_list_mapped_to_dch if their RLC
        PDU size has changed.*/ 
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_TransportChannelType,rrc_DL_TrCH_TypeId1_r5_dch))
    {
      TDSRRC_MSG1(MSG_LEGACY_LOW,"REL9: DL DCH TrCH id %d",(tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch);
      tdsrrcllc_get_rb_for_rb_list_mapped_to_dch((tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch, 
                tdsordered_config_ptr->rrc_state_indicator, TDSDL);  
    }
  } /* end CELL_DCH check */
  
  /* States not supported */
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_FACH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_PCH not supported for TrCH");
    return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"URA_PCH not supported for TrCH");
    return FAILURE;
  }
  else
  {
    ERR_FATAL("Unknown state not supported for TrCH",0,0,0);
  }
  
  /***************************************************************/
  /* Transport Channel Count has been updated in RB Mapping info */
  /***************************************************************/
  if (!TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->tfs_SignallingMode,
    rrc_DL_AddReconfTransChInformation_r9_tfs_SignallingMode_hsdsch) )
  {
    /* Check for the BLER */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,
        tdsrrc_DL_AddReconfTransChInformation_r9,dch_QualityTarget))
    {
      /* Save the BLER if found, and is in range */
      if((ie_ptr->dch_QualityTarget.bler_QualityValue >= TDSRRCLLC_MIN_BLER) &&
        (ie_ptr->dch_QualityTarget.bler_QualityValue <= TDSRRCLLC_MAX_BLER))
      {
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val =
          (int8)ie_ptr->dch_QualityTarget.bler_QualityValue;
      }
      
      /* BLER is out of range */
      else
      {
        /* According to L1 I/F, must set an invalid value of -127 */
        tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL BLER %d is out of range",ie_ptr->dch_QualityTarget.bler_QualityValue);
        return FAILURE;
      }
    } /* end if BLER present */
    else
    {
      /* IE omitted. Set the target to -127 which would result 1% of BLER target.
       * This is good UE behaviour though the spec is ambiguous on what UE
       * behavior should be in this case.  
       */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DCH quality target not present. Setting to -127");
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
    }
  }
  else
  {
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation_r9,dch_QualityTarget))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring dch quality target for HS");
    }
  }
  /* Indicate that DL L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
  
  return SUCCESS;
}


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
  tdsrrc_DL_AddReconfTransChInfoList_r9 * dl_trch_add_reconfig_ptr,
  /* local UL TrCH Add Reconfig pointer */
  tdsrrc_UL_AddReconfTransChInfoList_r8  * ul_trch_add_reconfig_ptr  
)
{
  uint32 idx, tf_idx;
  /* Get the DL Added or Reconfigured Transport Channel information */
  if(dl_add_reconf_trch_info_list_present)
  {
      
    idx = 0;
    
    /* Save info for each additional Transport Channel */
    while((idx <= TDSUE_MAX_TRCH) && (dl_trch_add_reconfig_ptr->n > idx))
    {
      if(tdsrrcllc_get_dl_added_reconfig_trch_r9(&dl_trch_add_reconfig_ptr->elem[idx],
          ul_trch_add_reconfig_ptr
        ) == FAILURE)
      {
        return FAILURE;
      }
      /* get next TrCH Info */
      idx++;
    } /* end while */
  } 
  
  /* At this point store the Queue Information in L1 structure. To add later if HSDSCH config changed */
  
  /* Adjust TB size on every TrCH if necessary. */ 
  for (idx=0; idx<TDSUE_MAX_TRCH; idx++)
  {
  /* The number of logical channel mapped to this transport channel 
  increases from 1 to 2. Needs to adjsut the tb_szie to
    reflect the adding of the MAC header.                         */
    if (tdsordered_config_ptr->dl_dch_trch_info[idx].dch_present &&
      (tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up ||
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down))
    {
      /* TrCH IE is present and the TrCH ID is appear in the TrCH info table. */
      if (!(dl_add_reconf_trch_info_list_present &&
        tdsrrcllc_trch_id_in_config_trch_list_r9(dl_trch_add_reconfig_ptr,
        tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id)))
      {
        /* Added MAC head size to the current TB size.  */
        if (tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up &&
          !tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Increasing TrCH %d TB size,%d by MAC hdr size",tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id,tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[0]->tb_size);
          for (tf_idx=0; tf_idx<tdsordered_config_ptr->dl_trch_info[idx].num_tf; tf_idx++)
          {
            if(tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size > 0)
            {
              tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size += 
              TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
            }
            
          }
        }
        /* Substract MAC head size from the current TB size.  */
        else if (!tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up &&
          tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Decreasing TrCH %d TB size,%d by MAC hdr size",tdsordered_config_ptr->dl_dch_trch_info[idx].trch_id,tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[0]->tb_size);
          for (tf_idx=0; tf_idx<tdsordered_config_ptr->dl_trch_info[idx].num_tf; tf_idx++)
          {
            if (tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size > 0)
            {
              tdsordered_config_ptr->dl_trch_info[idx].tf_info_array[tf_idx]->tb_size -= 
                TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
            }
          }
        }
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clean TB size adjust flag in RBSetup");
      /* Reset lg_ch_cnt_up and lg_ch_cnt_down. */
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_up = FALSE;
      tdsordered_config_ptr->dl_dch_trch_info[idx].lg_ch_cnt_down = FALSE;
    }
    else if (!tdsordered_config_ptr->dl_dch_trch_info[idx].dch_present)
    {
    /* Since the activated transport channels are sorted before stored in
      the memory, break the looping after reaching the 1st invalid block.  */
      break;
    }
  }
  
  /* Check to make sure DL TFS count is within UE Capabilities */
  if(tdsrrcllc_check_dl_tfs_cnt() == FAILURE)
  {
    return FAILURE;
  }

  return(SUCCESS);
}

#endif /*FEATURE_TDSCDMA_REL9*/

/*================================================================================================
FUNCTION: tdsrrcllc_get_dl_added_reconfig_trch_r4()

DESCRIPTION:
  This function takes a pointer to the Uplink Added or Reconfigured 
  Transport Channels IE and copies it to TDSORDERED_CONFIG.
  The function is copy from tdsrrcllc_get_dl_added_reconfig_trch:
   1.R4 is less than R3 a IE "tdsrrc_TM_SignallingInfo dummy".
   2.IE"tfs_SignallingMode" struct is different between R3 and R4.

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
)
{
  /* local transport channel variables */
  tr_ch_id_type trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
  tr_ch_id_type ul_trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
  
  /* Pointers to UL and DL TFS info for use when copying existing config */
  tdsl1_dl_cctrchcfg_trch_info_struct_type *dl_tfs_info = NULL;
  tdsl1_ul_semi_static_parms_struct_type *ul_tfs_ss_info = NULL;
  tdsmac_ul_tfs_type *ul_tfs_dyn_info = NULL;
  uint32 idx =0;
  /* Loop variables for copying UL TFS info */
  /*lint -e578 */
  uint8 tf_idx = 0;
  tdsl1_dl_cctrchcfg_tf_info_struct_type *rrc_dl_tf_info = NULL;
  tdsmac_ul_tf_type *ul_tf_info = NULL;
  /*lint +e578 */
  
  /* Boolean used to indicate if multiple logical channels are mapped to
  a given TrCH. */
  boolean       multiple_ded_log_chan = FALSE;
  
  /* local variable indicating whether or not an uplink TrCH ID was found */
  boolean       got_one = FALSE;
  

  TDSRRC_MSG0(MSG_LEGACY_MED,"Get DL Add/Reconf TrCH");
  /*
  * Use the Transport Channel Index and next state to place the TFS in 
  * the correct slot in the MAC TFS array.
  */
  if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
  {
    /* Get the transport channel index for this transport channel ID.
     * Note: The index should already be there.
     */
    trch_idx = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)ie_ptr->dl_transportChannelIdentity);

    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,no idx found",ie_ptr->dl_transportChannelIdentity);
      return(FAILURE);
    }
    if(tdsrrcllc_restore_dl_dch_rb_mapping_info(trch_idx) == FAILURE)
    {
      /* Validation was done prior to adding it to the Standalone RB-Mapping backup info */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,info not correct in Standalone RB-Mapping",ie_ptr->dl_transportChannelIdentity);
    }
    
    /* Determine if multiple dedicated logical channels are present on this TrCH id */
    if(tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt > 1)
    {
      multiple_ded_log_chan = TRUE;
    }

    /*
    * Get whether or not the TFS for a given TRCH is included or if it needs
    * to come from an uplink TrCH ID.*/
    switch(ie_ptr->tfs_SignallingMode.t)
    {
      /* TFS explicitly given for this Transport Channel */
      case T_tdsrrc_DL_AddReconfTransChInformation_r4_tfs_SignallingMode_explicit_config:
        TDSRRC_MSG1(MSG_LEGACY_LOW,"Get TFS for DL TrCH Id %d",ie_ptr->dl_transportChannelIdentity);

        /* get the Transport Format Set for this TrCH */
        if(tdsrrcllc_get_dl_transport_format_set(ie_ptr->tfs_SignallingMode.u.explicit_config,
                                       &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
                                       multiple_ded_log_chan, /* mulitple logical channels? */
                                       &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
                                      ) == FAILURE)
        {
          return FAILURE;
        }
        break; /* end explicit TFS signalling for DL TrCH */

      case T_tdsrrc_DL_AddReconfTransChInformation_r4_tfs_SignallingMode_sameAsULTrCH:
        /*
        * Make sure the UL Info was included, and if it was get the TFS for 
        * the TrCH ID that was included in this message.
        */
        if (ul_ptr == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"invalid:UL TFS absent when DL TFS choice 'same as UL'");
          return(FAILURE);
        }
        while(ul_ptr->n > idx)
        {
          if(ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
             ul_TransportChannelIdentity == ul_ptr->elem[idx].transportChannelIdentity)
          { 
            /* Got a match with Uplink TrCH ID */
            TDSRRC_MSG2(MSG_LEGACY_MED,"Get TFS for DL TrCH Id %d using UL TrCH ID %d",ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->ul_TransportChannelIdentity,ul_ptr->elem[idx].transportChannelIdentity);
            if(tdsrrcllc_get_dl_transport_format_set(&ul_ptr->elem[idx].transportFormatSet,
                                           &tdsordered_config_ptr->dl_trch_info[trch_idx],  /* dedicated trch */
                                           multiple_ded_log_chan, /* mulitple logical channels? */
                                           &tdsordered_config_ptr->dl_trch_info[trch_idx]   /* common trch */
                                          ) == FAILURE)
            {
              return FAILURE;
            }

            /* Indicate that found an UL TrCH ID, and break the while loop */
            got_one = TRUE;
            break;
          } /* end UL TrCH match */

          /* no match, keep looking */
          else 
          {
            idx++;
          }
        } /* end while */

        /* Make sure we found the UL TrCH, before continuing */
        if(!got_one)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No matching UL TFS in msg,look up from OC");
          /* Get the transport channel index for the UL transport channel ID.
           * Note: The index should already be there.
           */
          ul_trch_idx = tdsrrcllc_find_ul_dch_trch_idx(
              tdsordered_config_ptr,
              (tr_ch_id_type)ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
                ul_TransportChannelIdentity);
          if(ul_trch_idx >= TDSUE_MAX_TRCH)
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"No DL TFS for TrCH %d, to use UL TrCH %d for TFS",
                ie_ptr->dl_transportChannelIdentity,
                ie_ptr->tfs_SignallingMode.u.sameAsULTrCH->
                  ul_TransportChannelIdentity, 0);
            return(FAILURE);
          }

          /* Set up convenience pointers */
          dl_tfs_info = &tdsordered_config_ptr->dl_trch_info[trch_idx];

          ul_tfs_ss_info = &tdsordered_config_ptr->rrc_ul_cctrch_params.semi_static_parms[ul_trch_idx];
          ul_tfs_dyn_info = &tdsordered_config_ptr->ul_tfcs.tfs_info[ul_trch_idx];

          /* Copy semistatic info from uplink */
          switch(ul_tfs_ss_info->coding)
          {
            case TDSL1_UNCODED:
                    dl_tfs_info->coding_type = TDSL1_DL_UNCODED;
                    break;
            case TDSL1_CONV_1_BY_2_CODING:
                    dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_2_CODING;
                    break;
            case TDSL1_CONV_1_BY_3_CODING:
                    dl_tfs_info->coding_type = TDSL1_DL_CONV_1_BY_3_CODING;
                    break;
            case TDSL1_TURBO_CODING:
                    dl_tfs_info->coding_type = TDSL1_DL_TURBO_CODING;
                    break;
            default:break; 
          }
          dl_tfs_info->rate_match_attrib = ul_tfs_ss_info->staticRM;
          switch(ul_tfs_ss_info->crc)
          {
            case TDSL1_CRC_LENGTH_0:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_0;
              break;
            case TDSL1_CRC_LENGTH_8:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_8;
              break;
            case TDSL1_CRC_LENGTH_12:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_12;
              break;
            case TDSL1_CRC_LENGTH_16:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_16;
              break;
            case TDSL1_CRC_LENGTH_24:
              dl_tfs_info->crc_length = TDSL1_CRC_LENGTH_24;
              break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid CRC length %d in UL",ul_tfs_ss_info->crc);
              return FAILURE;
          }

          /* Copy dynamic info from uplink */
          dl_tfs_info->tti_value = ul_tfs_dyn_info->tti_info;
          dl_tfs_info->num_tf = (uint8)ul_tfs_dyn_info->ntf;
          for(tf_idx = 0; tf_idx < dl_tfs_info->num_tf; tf_idx++)
          {
            rrc_dl_tf_info = dl_tfs_info->tf_info_array[tf_idx];
            ul_tf_info = &ul_tfs_dyn_info->tf_info[tf_idx];

            rrc_dl_tf_info->num_tb = (uint8)ul_tf_info->ntblks;
            rrc_dl_tf_info->tb_size = ul_tf_info->tb_size;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"TB Size: %d,multiple_ded_log_chan: %d",rrc_dl_tf_info->tb_size,multiple_ded_log_chan);
            
            /* If number of logical channel is > 1, inc tb_size by 4 to accomodate MAC-header */
            if (multiple_ded_log_chan == TRUE)
            {
              rrc_dl_tf_info->tb_size += TDSRRCLLC_DED_MULTI_LOG_CHL_DCH_MAC_HDR_SIZE;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"New TB Size %d",rrc_dl_tf_info->tb_size);
            }
          }
        }
        break; /* end use of previous signalled UL TrCH TFS for DL TrCH */ 

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"No DL TFCS was included for TrCH ID %d",ie_ptr->dl_transportChannelIdentity);
        return FAILURE;

    } /* end TFS selection switch */

    /*Find Rbs mapped to this DCH and fill them in rb_list_mapped_to_dch if their RLC
    PDU size has changed.*/ 
    tdsrrcllc_get_rb_for_rb_list_mapped_to_dch((tr_ch_id_type)ie_ptr->dl_transportChannelIdentity, 
              tdsordered_config_ptr->rrc_state_indicator, TDSDL);   
  } /* end CELL_DCH check */

  /* States not supported */
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_FACH not supported for TrCH");
     return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"CELL_PCH not supported for TrCH");
     return FAILURE;
  }
  else if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"URA_PCH not supported for TrCH");
     return FAILURE;
  }
  else
  {
     ERR_FATAL("Unknown state not supported for TrCH",0,0,0);
  }

  /***************************************************************/
  /* Transport Channel Count has been updated in RB Mapping info */
  /***************************************************************/

  /* Check for the BLER */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_AddReconfTransChInformation,
    dch_QualityTarget))
  {
    /* Save the BLER if found, and is in range */
    if((ie_ptr->dch_QualityTarget.bler_QualityValue >= TDSRRCLLC_MIN_BLER) &&
       (ie_ptr->dch_QualityTarget.bler_QualityValue <= TDSRRCLLC_MAX_BLER)
      )
    {
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val =
        (int8)ie_ptr->dch_QualityTarget.bler_QualityValue;
    }

    /* BLER is out of range */
    else
    {
      /* According to L1 I/F, must set an invalid value of -127 */
      tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL BLER %d is out of range",ie_ptr->dch_QualityTarget.bler_QualityValue);
      return FAILURE;
    }
  } /* end if BLER present */
  else
  {
    /* IE omitted. Set the target to -127 which would result 1% of BLER target.
     * This is good UE behaviour though the spec is ambiguous on what UE
     * behavior should be in this case.  
     */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DCH quality target not present. Setting to -127");
    tdsordered_config_ptr->dl_trch_info[trch_idx].bler_quality_val = TDSRRCLLC_BLER_NA;
  }

  /* Indicate that DL L1 needs to be reconfigured */
  tdsordered_config_ptr->reconfig_needed.downlink_l1 = TRUE;
  tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;

  return SUCCESS;
} /* end tdsrrcllc_get_dl_added_reconfig_trch_r4() */
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

  The function is copy from tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info
      
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
  tdsrrc_DL_AddReconfTransChInfoList_r4  * dl_trch_add_reconfig_ptr,
    /* local DL TrCH Drop Pointer */
  tdsrrc_DL_DeletedTransChInfoList    * dl_trch_delete_ptr
)
{
   /* Transport Channel ID */
   tr_ch_id_type   tdstrch_id;
   uint32 idx=0;
   
   while((dl_trch_delete_ptr != NULL) && (dl_trch_delete_ptr->n > idx))
   {
     /* Remove this transport channel from MAC, L1 and local data.
      * and update transport channel count in L1, MAC and local data.
      */
     if(tdsrrcllc_get_dl_deleted_trch(&dl_trch_delete_ptr->elem[idx]) == FAILURE)
     {
       return FAILURE;
     }
     /* set tdsdl_tfcs_needed flag to true.  Later if common info is not preset,
      * UE needs to reject the message 
      */
     tdsdl_tfcs_needed = TRUE;
     /* get next TrCH Info */
     idx++;
   } /* end while */
   idx =0;
   /* Get the DL Added/Reconfigured Info */
   while((dl_trch_add_reconfig_ptr !=NULL) && (dl_trch_add_reconfig_ptr->n > idx) )
   {
     tdstrch_id = (tr_ch_id_type)dl_trch_add_reconfig_ptr->elem[idx].dl_transportChannelIdentity;
 
     if((tdsrrcllc_get_dl_dch_trch_idx(tdstrch_id)) == TDSRRCLLC_INVALID_TRCH_IDX)
     {
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"Couldn't find idx,TrCH ID %d",tdstrch_id);
       return(FAILURE);
     }
     idx++;
   } /* while(...) */
 
   /* At this point, the transport channel list -
    *  - may have holes due to deleted channels.
    *  - transport channel info may not be sorted by TrCH IDs.
    */ 
 
   /* Fill holes and put all Transport channels in the first
    * tdsordered_config_ptr->dl_trch_cnt indices.
    */
   tdsrrcllc_fill_holes_in_dl_dch_trch_idx();
 
   /* Now is the time and place for sorting all transport channel
    * info based on TrCH IDs. Call the right function.
    */
 
   tdsrrcllc_sort_dl_dch_trch_index_array(&tdsordered_config_ptr->dl_dch_trch_info[0], 
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING)
                                      tdsordered_config_ptr->mac_dl_parms.num_dchs
#else
                                       (TDSRRCLLC_DOWNLINK_TRCH_COUNT(tdsordered_config_ptr))
#endif 
                                       );
   return(SUCCESS);
} /* tdsrrcllc_add_delete_and_arrange_dl_dch_trch_info_r5 */



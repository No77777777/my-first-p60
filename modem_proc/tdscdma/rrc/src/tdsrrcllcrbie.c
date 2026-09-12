/*===========================================================================

             L3 - RRC Lower Layer Controller Radio Bearer IEs

DESCRIPTION
  This file contains functions necessary for the RRC Lower Layer Controller
  Ordered Config Radio Bearer Information Element processing.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcllcrbie.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
05/02/13   hx      Fix pdcp rab index was wrongly used as rb_cnt
09/17/11   zwj     Fixed UL TB size mac header size.
08/26/11   yzh       Add add_queue_bitmask in order to check multiple delete mac_queue info in OTA message.
                            Modify bug about remove old mapping if mac_queue is available in other mac_dflow,
                            And check old mapping when setup not recofigure mac_dflow.
06/14/11   yzh    Made changes from FDD to TDD in R8
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
03/02/11   rl       Merged with VU_MODEM_WCDMA_RRC.01.92.50
02/24/11   ad       Fixed compilation warnings
02/18/11   ad       Added changes for one sided UL RLC reestablishment 
02/15/11   kp       Modified rrcllc_restore_rach_rb_mapping to restore SRB1 only info. 
02/15/11   rl       Merged with VU_MODEM_WCDMA_RRC.01.91.50
02/14/11   as       Made changes to use memmove instead of memcpy when source
                    and destination buffers overlap
02/07/11   rl       Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/28/11   rl       Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad       Added changes for RRC SW decoupling
01/12/11   rl       Migrated to VU_MODEM_WCDMA_RRC.01.88.50
01/04/11   rl       Merged with VU_MODEM_WCDMA_RRC.01.87.50
12/17/10   prk      Made changes to indicate RLC if network configures DL PDU 
                    sizes for AM.
12/10/10   prk      Made changes to restore mappping from ordered hanging RB.
12/31/10   rl       Merged with VU_MODEM_WCDMA_RRC.01.86.50
12/01/10   prk      Featurization of code, this avoids rejecting dch-ehs mapping
                    if EHS feature is enabled.
11/23/10   kp       Set mac_id in l1 db while restoring mac-ehs q info
11/22/10   prk      Modified function definition such that it restore a given RB,
                    from Hanging rb to OC.
11/29/10   rl       Merged with VU_MODEM_WCDMA_RRC.01.85.50
11/02/10   zwj      Added code to handle REL4 OTA Message.

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
#include "tdsrrcdata.h"
#include "tdsrrclcm.h"
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsibdb.h"
#include "tdsuecomdef.h"
#include "tdsrrccommon.h"
#ifdef FEATURE_TDSCDMA_SRB_RESET
#include "tdsrrcnv.h"
#endif

/*===================================================================
                        DATA DECLARATIONS
====================================================================*/
#ifdef FEATURE_UMTS_PDCP
extern boolean tdsrrc_pdcp_enabled;
#endif /* FEATURE_UMTS_PDCP */

#ifdef FEATURE_TDSCDMA_HSUPA
/*Flag to check if one sided validation check can be done for ul reestablishment*/
    boolean tdsone_sided_validation_check=TRUE;
#endif
/*===================================================================
                        CONSTANTS
====================================================================*/

/*===================================================================
                        FORWARD DECLARATIONS
====================================================================*/

/*===================================================================
*                       FUNCTION PROTOTYPES
====================================================================*/
#ifdef FEATURE_TDSCDMA_CHANGE_SRB4_PRIRORITY
void tdsrrcllc_check_and_set_srb3_srb4_priority(void);
#endif
/*===================================================================
*                            FUNCTIONS
====================================================================*/

/*-------------------------------------------------------------------
This macro returns true if passed log chan type is DCCH or DTCH.  Else
returns false.
--------------------------------------------------------------------*/
#ifndef FEATURE_TDSCDMA_SRB5_SUPPORT
#define TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(log_chan_type) \
    ((log_chan_type == UE_LOGCHAN_DCCH) || \
     (log_chan_type == UE_LOGCHAN_DTCH) ) ? TRUE : FALSE 
#else

#define TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(log_chan_type) \
    ((log_chan_type == UE_LOGCHAN_DCCH) || \
     (log_chan_type == UE_LOGCHAN_DTCH) || \
     (log_chan_type == UE_LOGCHAN_TM_DCCH_DL)) ? TRUE : FALSE 

#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

/*lint -e818 */
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
)
{
  uint32 rlc_size = 0;
  uint8 ix,iy,iz;
  boolean  rb_found = FALSE;

  /* Check in EDCH MAC structure if there is a higher RLC size */
  rb_found = FALSE;
  for (ix = 0; (!rb_found) && (ix < MAX_E_DCH_MAC_D_FLOW) ; ix++) 
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

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCEUL:RLC Size for RB id %d is %d",rb_id,rlc_size);
  return rlc_size;
}
#endif

/*==========================================================
FUNCTION  : get_lc_type_by_rb_id()

DESCRIPTION :   
  This function returns logical channel type based on UM RB id.

DEPENDENCIES: None.

RETURN VALUE: 
  Logical Channel Type.

SIDE EFFECTS: None.
============================================================*/
uecomdef_logchan_e_type tdsrrcllc_get_lc_type_by_um_rb_id
(
  tdsrrc_RB_Identity rb_id
)
{
  if(rb_id == CCCH_RADIO_BEARER_ID)
  {
    return UE_LOGCHAN_CCCH;
  }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  else if(rb_id == CTCH_RADIO_BEARER_ID)
  {
    return UE_LOGCHAN_CTCH;
  }
#endif
  else if(rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    return UE_LOGCHAN_DCCH;
  }
  else
  {
    return UE_LOGCHAN_DTCH;  	
  }
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_size_list()

DESCRIPTION : Processes the RLC Size List IE contents and saves 
              them in the RLC size restriction database.  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: The RLC size restriction database is updated.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rlc_size_list
(
  tdsrrc_UL_LogicalChannelMapping *ie_ptr,
  tdsrrcllcoc_rlc_size_list_type  *rlc_size_rec,
  tdsrrc_RB_Identity                    rb_id
)
{
  tdsrrc_RLC_SizeExplicitList *rlc_size_list = NULL;

  rlc_size_rec->rb_id = rb_id;
  switch(ie_ptr->rlc_SizeList.t)
  {
    case T_tdsrrc_UL_LogicalChannelMapping_rlc_SizeList_allSizes:
      rlc_size_rec->restriction_type = TDSRLC_SIZES_ALL;
      break;

    case T_tdsrrc_UL_LogicalChannelMapping_rlc_SizeList_configured:
      rlc_size_rec->restriction_type = TDSRLC_SIZES_CONFIGURED;
      break;

    case T_tdsrrc_UL_LogicalChannelMapping_rlc_SizeList_explicitList:
      rlc_size_rec->restriction_type = TDSRLC_SIZES_EXPLICIT_LIST;
      rlc_size_list = ie_ptr->rlc_SizeList.u.explicitList;
      if(rlc_size_list == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RLC Size List empty");
        return(FAILURE);
      }
      rlc_size_rec->count = 0;
      while(rlc_size_rec->count < rlc_size_list->n)
      {
        rlc_size_rec->size_idx[rlc_size_rec->count] = (uint16)rlc_size_list->elem[rlc_size_rec->count].rlc_SizeIndex;
		rlc_size_rec->count++;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RLC Size List CHOICE: %d",ie_ptr->rlc_SizeList.t);
      return(FAILURE);
  }

  return SUCCESS;
} /* end tdsrrcllc_get_rlc_size_list() */


/*===========================================================================
FUNCTION tdsrrcllc_validate_srb_id

DESCRIPTION
This function validates the SRBID against the RLC mode of the RBID

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS or FAILURE
===========================================================================*/
static uecomdef_status_e_type tdsrrcllc_validate_srb_id
(
  tdsrrc_RB_Identity rb_id
)
{
  uecomdef_status_e_type status = SUCCESS;
  switch(rb_id)
  {
    case DCCH_UM_RADIO_BEARER_ID:
      /*Check the RB ID against UM*/
      if((UE_MAX_DL_UM_CHANNEL == tdsrrcllc_get_dl_um_index(rb_id, tdsordered_config_ptr)) ||
         (UE_MAX_UL_UM_CHANNEL == tdsrrcllc_get_ul_um_index(rb_id)))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Mode for RB ID %d is not UM ",rb_id);
        status = FAILURE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"UM mode for id %d",rb_id);
      }
      break;
    case DCCH_AM_RADIO_BEARER_ID:
    case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
    case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
      if(UE_MAX_AM_ENTITY == tdsrrc_get_am_index(rb_id))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Mode for RB ID %d is not AM ",rb_id);
        status = FAILURE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"AM SRB for id %d",rb_id);
      }
      break;
    default:
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      /*If the RB mode is AM or UM or UL TM (TBD: Check for UL TM), then it is not SRB5 too, so return failure*/
      if((UE_MAX_DL_UM_CHANNEL != tdsrrcllc_get_dl_um_index(rb_id, tdsordered_config_ptr)) ||
         (UE_MAX_UL_UM_CHANNEL != tdsrrcllc_get_ul_um_index(rb_id)) ||
         (UE_MAX_AM_ENTITY != tdsrrc_get_am_index(rb_id)))
      {
        status = FAILURE;
      }
#else
      status = FAILURE;
#endif
      break;
  }
  return status;
}

/*==========================================================
FUNCTION  : tdsrrcllc_update_dl_dch_ota()

DESCRIPTION :

DEPENDENCIES: None.

RETURN VALUE: 

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_update_dl_dch_ota
(
 tdsrrc_RB_Identity    rb_id,
 uecomdef_logch_mode_e_type       logch_mode,
 tr_ch_id_type   tdstrch_id,
  tdsrrc_LogicalChannelIdentity logical_channel_id
)

{
      /* transport Channel Index */
  uint8         trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
  
  /* Get the logical channel index */
  uint8         logch_idx;
  rlc_lc_id_type   rlc_lc_id;
  uint8 cnt;

      /* Get the transport channel index for this transport channel ID.
    * Note: The index should already be there.
  */
  trch_idx = tdsrrcllc_find_dl_dch_trch_idx(tdstrch_id);
  
  if (trch_idx == TDSRRCLLC_INVALID_TRCH_IDX)
  {
   /* Store the RB-Mapping info */
  return(tdsrrcllc_backup_dl_dch_from_ota( tdstrch_id,rb_id,logch_mode,(uint8) logical_channel_id));
 
  }
  
  /* Get the Logical Channel Index */
  logch_idx = tdsordered_config_ptr->dl_dch_info[trch_idx].ndlchan;

  for (cnt = 0; cnt < tdsordered_config_ptr->dl_dch_info[trch_idx].ndlchan; cnt++) 
  {
    if (rb_id == tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[cnt].rb_id)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB_ID %d already present",rb_id);
      return (SUCCESS);
    }
  } 

  /* If Logical Channel ID is present, save MAC Logical Channel Identity */
  tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[logch_idx].mac_id =
      (uint8)logical_channel_id;
  

  /* Save Radio Bearer ID */
  tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[logch_idx].rb_id = rb_id;
  
  /* Determine the type of Logical Channel mapped to DCH TrCH */
  if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
  } 
  else
  {
    tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
  }
  
  if (logch_mode == UE_MODE_MAX_NUM)
  {
  /* This is the case when RB mapping info is coming 
  * in "Affected List". At this point, we need to update the Logical Channel mode 
  * and Logical Channel ID in Cell FACH RB Mapping Info - 
  * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
    */
    rlc_lc_id = tdsrrclcm_get_dl_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if (rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid DL LC ID for RB %d",rb_id);
      return(FAILURE);
    }
    
    if (tdsrrclcm_get_dl_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get DL Log Chl mode for LC %d",rlc_lc_id);
      return(FAILURE);
    }
    /* Also update the Logical Channel ID here */
    tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[logch_idx].rlc_id = rlc_lc_id;
  }
  
  /* Save the logical channel mode, if a setup */
  tdsordered_config_ptr->dl_dch_info[trch_idx].dlchan_info[logch_idx].rlc_mode = logch_mode;
  
  /* Increment number of logical channels */
  tdsordered_config_ptr->dl_dch_info[trch_idx].ndlchan++;
  /* The logic channel mapped to this TrCH increases from 1 to 2. */
  /* Need to update the TB size to reflect this change.           */
  if (tdsordered_config_ptr->dl_dch_info[trch_idx].ndlchan == 2 &&
    tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt == 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Need to adjust TB size");
    tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt_up = TRUE;
  }
  tdsordered_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt = 
    tdsordered_config_ptr->dl_dch_info[trch_idx].ndlchan;
  
  tdsrrcllc_free_hanging_rb_mapping_info_index(rb_id,TDSDL_DCH_RB_MAPPING);

  return SUCCESS;
}

/*==========================================================
FUNCTION  : tdsrrcllc_process_dl_pdu_size()

DESCRIPTION :

DEPENDENCIES: None.

RETURN VALUE: 

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_process_dl_pdu_size
(
  tdsrrc_OctetModeRLC_SizeInfoType1 * rrc_dl_rlc_pdu_sz,
  uint8               entity_idx,
  boolean       allow_new_channels,
  tdsrlc_reestablish_e_type reestablish_entity

)
{
  uint32 rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;
    
  switch (rrc_dl_rlc_pdu_sz->t)
  {
  case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType1:
    
    /* -- Actual size = (8 * sizeType1) + 16   sizeType1 INTEGER (0..31) */
    if (rrc_dl_rlc_pdu_sz->u.sizeType1 > 31)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RLC Size %d (0..31)",rrc_dl_rlc_pdu_sz->u.sizeType1);
      return FAILURE;
    }
    rlc_size = (rrc_dl_rlc_pdu_sz->u.sizeType1* 8) + 16;
    break;
    
  case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType2:
  /* -- Actual size = (32 * part1) + 272 + (part2 * 8) part1 INTEGER (0..23),
    part2 INTEGER (1..3) */
    if (rrc_dl_rlc_pdu_sz->u.sizeType2->part1 > 23)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Part1 RLC Size %d (0..23)",rrc_dl_rlc_pdu_sz->u.sizeType2->part1);
      return FAILURE;
    }
    rlc_size = (rrc_dl_rlc_pdu_sz->u.sizeType2->part1 * 32) + 272;
    if (rrc_dl_rlc_pdu_sz->u.sizeType2->m.part2Present)
    {
      if (!((rrc_dl_rlc_pdu_sz->u.sizeType2->part2 > 0) && (rrc_dl_rlc_pdu_sz->u.sizeType2->part2 <4)))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Invalid Part1 RLC Size %d (1..3)",rrc_dl_rlc_pdu_sz->u.sizeType2->part2);
        return FAILURE;
      }
      rlc_size += (rrc_dl_rlc_pdu_sz->u.sizeType2->part2 * 8);
    }
    break;
  case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType3:
  /* -- Actual size = (64 * part1) + 1040 + (part2 * 8) part1 INTEGER (0..61),
    part2 INTEGER (1..7) */
    if (rrc_dl_rlc_pdu_sz->u.sizeType3->part1 > 61)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Part1 RLC Size %d (0..61)",rrc_dl_rlc_pdu_sz->u.sizeType2->part1);
      return FAILURE;
    }
    
    rlc_size = (rrc_dl_rlc_pdu_sz->u.sizeType3->part1 * 64) + 1040;
    if (rrc_dl_rlc_pdu_sz->u.sizeType3->m.part2Present)
    {
      
      if (!((rrc_dl_rlc_pdu_sz->u.sizeType3->part2 > 0) && (rrc_dl_rlc_pdu_sz->u.sizeType3->part2 <8)))
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Part1 RLC Size %d (1..7)",rrc_dl_rlc_pdu_sz->u.sizeType2->part2);
        return FAILURE;
      }
      rlc_size += (rrc_dl_rlc_pdu_sz->u.sizeType3->part2 * 8);
    }
    break;
  default:
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RLC DL PDU Size %d",rrc_dl_rlc_pdu_sz->t);
    return FAILURE;
  }
  

  TDSRRC_MSG2(MSG_LEGACY_MED,"rlcsizeindicated: RLC Size %d,NewCh %d",rlc_size,allow_new_channels);

  tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size = rlc_size;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_pdu_size_flexbile = FALSE; 
#endif

  if (allow_new_channels == FALSE)
  {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
 
    if(tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_pdu_size_flexbile == TRUE ) 
    {
      tdsordered_config_ptr->am_config[entity_idx].reestab_entity = reestablish_entity;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: Re-establish dl am enity %d PDU type changed from  flexible to fixed for rb-id %d ",entity_idx,tdsordered_config_ptr->am_config[entity_idx].common_config.rb_id);
    }
    else
#endif
    if (tdsrrcllc_compare_rlc_size_change(tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size,
      tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size) == TRUE)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"rlcsizeindicated: OldSize %d,NewSize %d,Reestab[0:B/1:DL] %d ", tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size,
        tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size,reestablish_entity);
      
      tdsordered_config_ptr->am_config[entity_idx].reestab_entity = reestablish_entity;
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"rlcsizeindicated: Old Size %d,New Size %d",tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size,tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size);
      tdsordered_config_ptr->am_config[entity_idx].reestab_entity = TDSRLC_RE_ESTABLISH_NONE;
    }
  }
  
  return SUCCESS;
  
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_dch_mapping_info()

DESCRIPTION : Processes DL DCH Mapping IE contents and saves 
              them in MAC Ordered Configuration Data  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_dch_mapping_info
(
  tdsrrc_DL_LogicalChannelMapping *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
)
{


  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;
  /* Get the DL DCH TrCH ID if it exists */
  switch(ie_ptr->dl_TransportChannelType.t)
  {
    case T_tdsrrc_DL_TransportChannelType_dch:
      tdstrch_id = (tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch;
      break;

    case T_tdsrrc_DL_TransportChannelType_fach:
    case T_tdsrrc_DL_TransportChannelType_dsch:
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Expecting DL DCH TrCH ID");
      return FAILURE;
  } /* end DL DCH TrCH ID switch */


  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,& tdsrrc_DL_LogicalChannelMapping,logicalChannelIdentity))
  {
    return tdsrrcllc_update_dl_dch_ota(rb_id,logch_mode,tdstrch_id, ie_ptr->logicalChannelIdentity);
  }
  else
  {
    return tdsrrcllc_update_dl_dch_ota(rb_id,logch_mode,tdstrch_id, TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT);
  }
} /* end tdsrrcllc_get_dl_dch_mapping_info() */

/*==========================================================
FUNCTION  : tdsrrcllc_update_dl_ded_log_chl_info_r5()

DESCRIPTION : Processes DL Logical channel info and fills it
              in passed pointer to MAC I/F struct.

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
static void  tdsrrcllc_update_dl_ded_log_chl_info_r5
(
tdsrrc_DL_LogicalChannelMapping_r5 *ie_ptr, 
tdsmac_dl_ded_logch_config_type  *dlchan_info,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode
)
{
  /* If Logical Channel ID is present, save MAC Logical Channel Identity */ 
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_LogicalChannelMapping_r5,logicalChannelIdentity))
  {
    dlchan_info->mac_id =
      (uint8)ie_ptr->logicalChannelIdentity;
  }
  else
  {
    dlchan_info->mac_id =
      TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT;
  }
  /* Save Radio Bearer ID */
  dlchan_info->rb_id = rb_id;
  
  /* Determine the type of Logical Channel mapped to DCH TrCH */
  if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    dlchan_info->chan_type = UE_LOGCHAN_DCCH;
  } 
  else
  {
    dlchan_info->chan_type = UE_LOGCHAN_DTCH;
  }
  
  /* Save the logical channel mode, if a setup */
  if (logch_mode != UE_MODE_MAX_NUM)
  {
    dlchan_info->rlc_mode = logch_mode;
  }
}  /* tdsrrcllc_update_dl_ded_log_chl_info_r5 */


/*==========================================================
FUNCTION  : tdsrrcllc_update_dl_ded_log_chl_info()

DESCRIPTION : Processes DL Logical channel info and fills it
              in passed pointer to MAC I/F struct.

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
static void  tdsrrcllc_update_dl_ded_log_chl_info
 (
  tdsrrc_LogicalChannelIdentity logicalChannelIdentity,
  tdsmac_dl_ded_logch_config_type  *dlchan_info,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode

 )
{

  dlchan_info->mac_id =
      (uint8)logicalChannelIdentity;
  /* Save Radio Bearer ID */
  dlchan_info->rb_id = rb_id;

  /* Determine the type of Logical Channel mapped to DCH TrCH */
  if(rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    dlchan_info->chan_type = UE_LOGCHAN_DCCH;
  }
  else
  {
    dlchan_info->chan_type = UE_LOGCHAN_DTCH;
  }

  /* Save the logical channel mode, if a setup */
  if(logch_mode != UE_MODE_MAX_NUM)
  {
    dlchan_info->rlc_mode = logch_mode;
  }
}  /* tdsrrcllc_update_dl_ded_log_chl_info */


/*==========================================================
FUNCTION  : tdsrrcllc_get_fach_mapping_info()

DESCRIPTION : Processes FACH mapping IE contents and saves 
              it in MAC Ordered Configuration Database.
              If the next state is CELL_DCH, the data is 
              stored in local data structure in OC.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_fach_mapping_info
(
  tdsrrc_LogicalChannelIdentity logical_chan_id,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode,
  tdsrrc_state_e_type             next_rrc_state
)
{
  /* transport Channel Index */
  uint8   trch_idx  = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Get the logical channel index */
  uint8   logch_idx, i;
 
  /* Temporary storage for TrCH ID */
  tr_ch_id_type  tmp_trch_id;

  tdsmac_dl_ded_logch_config_type  *dlchan_info;
  rlc_lc_id_type    rlc_lc_id = (uint8)TDSRRCLCM_RLC_LC_ID_NOT_FOUND;

  /* TBD: Although all existing FACHs would be mapped to each of the RBs,
   * fill up only for the 1st FACH - till MAC interface is clarified/updated
   */

  boolean update_fach_rlc_id = FALSE;

  uint8 cnt;

  /* Start with updating Cell FACH RB Mapping Info when the next state is
   * either CELL_DCH or CELL_FACH
   */
  logch_idx = tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan;
  dlchan_info = &tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[logch_idx];

  if(logch_mode == UE_MODE_MAX_NUM)
  {
    /* This is the case when RB mapping info applicable to CELL_FACH is coming 
    * in "Affected List". At this point, we need to update the Logical Channel mode 
    * and Logical Channel ID in Cell FACH RB Mapping Info - 
    * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
    */
    rlc_lc_id = tdsrrclcm_get_dl_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if(rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid DL LC ID for RB %d",rb_id);
      return(FAILURE);
    }

    if(tdsrrclcm_get_dl_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get DL Log Chl mode for LC %d",rlc_lc_id);
      return(FAILURE);
    }
    /* Also update the Logical Channel ID here */
    dlchan_info->rlc_id = rlc_lc_id;
    update_fach_rlc_id = TRUE;
  }

  for (cnt = 0; cnt < tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan ; cnt++) 
  {            
    if(rb_id == tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[cnt].rb_id)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB_ID %d already present",rb_id);
      return (SUCCESS);
    }    
  }

/* If Logical Channel ID is present, save MAC Logical Channel Identity */ 

  tdsrrcllc_update_dl_ded_log_chl_info(logical_chan_id,dlchan_info,rb_id,logch_mode);
  tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan ++;
  
   
  /* Now update the FACH Info ONLY IF the next state is CELL_FACH */
  if(next_rrc_state == TDSRRC_STATE_CELL_FACH ||
     next_rrc_state == TDSRRC_STATE_CELL_PCH ||
     next_rrc_state == TDSRRC_STATE_URA_PCH)
  {
    if((trch_idx = tdsrrcllc_find_dl_fach_trch_idx()) >= TDSUE_MAX_TRCH)
    {
      return FAILURE;
    }
    logch_idx = tdsordered_config_ptr->fach_info[trch_idx].ndlchan;
    dlchan_info = &tdsordered_config_ptr->fach_info[trch_idx].dlchan_info[logch_idx];
    if(update_fach_rlc_id)
    {
      dlchan_info->rlc_id = rlc_lc_id;
    }

    /* If Logical Channel ID is present, save MAC Logical Channel Identity */ 

    tdsrrcllc_update_dl_ded_log_chl_info(logical_chan_id,dlchan_info,rb_id,logch_mode);

    tdsordered_config_ptr->fach_info[trch_idx].ndlchan++;
    tdsordered_config_ptr->dl_fach_trch_info[trch_idx].lg_ch_cnt = 
      tdsordered_config_ptr->fach_info[trch_idx].ndlchan;
      
    /* Since each available FACH is mapped to all RBs, copy the FACH info
    *  from the 1st FACH to all FACHs. Copy everything except for the
    * Transport Channel ID.
    */
    for(i = 1; i < tdsordered_config_ptr->mac_dl_parms.num_fachs; i ++)
    {
      tmp_trch_id = tdsordered_config_ptr->mac_dl_parms.fach_info[i]->trch_id;
      
      *tdsordered_config_ptr->mac_dl_parms.fach_info[i] = 
      *tdsordered_config_ptr->mac_dl_parms.fach_info[0];
      
      tdsordered_config_ptr->mac_dl_parms.fach_info[i]->trch_id = tmp_trch_id;
      
      tdsordered_config_ptr->dl_fach_trch_info[i].lg_ch_cnt =
      tdsordered_config_ptr->dl_fach_trch_info[0].lg_ch_cnt;
    }
  }  /* If next state is CELL_FACH, update the FACH Info */
   
  return SUCCESS;

} /* end tdsrrcllc_get_fach_mapping_info() */


/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_dch_mapping_info()

DESCRIPTION : Process UL DCH Mapping IE's contents and saves 
              them in the MAC Ordered Configuration Database.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_dch_mapping_info
(
  tdsrrc_UL_LogicalChannelMapping *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
)
{
  /* transport Channel Index */
  uint8         trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id = 0;

  /* Get the logical channel index */
  uint8         logch_idx;
  rlc_lc_id_type   rlc_lc_id;
  uint8 cnt = 0;



  /* Get the UL DCH TrCH ID if it exists */
  switch(ie_ptr->ul_TransportChannelType.t)
  {
    case T_tdsrrc_UL_TransportChannelType_dch:
      tdstrch_id = (tr_ch_id_type)ie_ptr->ul_TransportChannelType.u.dch;
      break;

    case T_tdsrrc_UL_TransportChannelType_rach:
    case T_tdsrrc_UL_TransportChannelType_dummy:
    case T_tdsrrc_UL_TransportChannelType_usch:
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Expecting UL DCH TrCH ID");
      return FAILURE;
  } /* end UL DCH TrCH ID switch */

  /* Get the transport channel index for this transport channel ID.
   * Note: The index should already be there.
   */
  trch_idx = tdsrrcllc_find_ul_dch_trch_idx(tdsordered_config_ptr, tdstrch_id);

  if(trch_idx >= TDSUE_MAX_TRCH)
  {
    /* Store the RB-Mapping info */
    return(tdsrrcllc_backup_ul_dch_from_ota(ie_ptr,rb_id,logch_mode));
  }

  /* Get the Logical Channel Index */
  logch_idx = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan;

  /* If network specifies same UL DCH for the RB, do not store it again */
  for (cnt = 0; cnt < tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan; cnt++)
  {
    if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnt].rb_id == rb_id)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC:Ignore UL DCH Info for rb %d",rb_id);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC:UL RLC Mapping for UL DCH %d not overwritten",tdstrch_id);
      return SUCCESS;
    }
    
  }

  /* If Logical Channel ID is present, save MAC Logical Channel Identity */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_UL_LogicalChannelMapping,logicalChannelIdentity))
  {
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].mac_id =
      (uint8)ie_ptr->logicalChannelIdentity;
  }
  else
  {
     tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].mac_id =
       TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT;
  }
  /* Initialize the MAC data structures to allow all RLC sizes, just in case. */
  tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rlc_size_info.all_tfi_valid = TRUE;




  /* MAC Logical Channel Priority */
  tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].priority =
    (uint8) ie_ptr->mac_LogicalChannelPriority;

  /* Save Radio Bearer ID */
  tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rb_id = rb_id;

  /* Determine the type of Logical Channel mapped to DCH TrCH */
  if(rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
  }
  else
  {
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
  }

  if(logch_mode == UE_MODE_MAX_NUM)
  {
    /* This is the case when RB mapping info applicable to CELL_FACH is coming 
     * in "Affected List". At this point, we need to update the Logical Channel mode 
     * and Logical Channel ID in Cell FACH RB Mapping Info - 
     * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
     */
    rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if(rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid UL LC ID for RB %d",rb_id);
      return(FAILURE);
    }
  
    if(tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get UL Log Chl mode for LC %d",rlc_lc_id);
    }

    /* Also update the Logical Channel ID here */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rlc_id = rlc_lc_id;
  }
  
  /* Save the logical channel mode, if a setup */
  tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rlc_mode = logch_mode;

  /* other wise it is just a reconfig of RB Mapping info */

  /* Increment number of logical channels */
  tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan++;

  if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan == 2 &&
    tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt == 1)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Need to adjust TB size");
    tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt_up = TRUE;
  }
  tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan;

  tdsrrcllc_free_hanging_rb_mapping_info_index(rb_id,TDSUL_DCH_RB_MAPPING);

  return SUCCESS;

} /* end tdsrrcllc_get_ul_dch_mapping_info() */


/*==========================================================
FUNCTION  : tdsrrcllc_update_ul_ded_log_chl_info()

DESCRIPTION : Processes UL Logical channel info and fills it
              in passed pointer to MAC I/F struct.

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
static void  tdsrrcllc_update_ul_ded_log_chl_info
 (
  tdsrrc_UL_LogicalChannelMapping *ie_ptr, 
  tdsmac_ul_ded_logch_config_type  *dlchan_info,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
 )
{
  /* If Logical Channel ID is present, save MAC Logical Channel Identity */ 
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_UL_LogicalChannelMapping,logicalChannelIdentity))
  {
    dlchan_info->mac_id =
      (uint8)ie_ptr->logicalChannelIdentity;
  }
  else
  {
    dlchan_info->mac_id = 
      TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT;
  }
  /* MAC Logical Channel Priority */
  dlchan_info->priority =
    (uint8)ie_ptr->mac_LogicalChannelPriority;

  /* Save Radio Bearer ID */
  dlchan_info->rb_id = rb_id;

  /* Determine the type of Logical Channel mapped to DCH TrCH */
  if(rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    dlchan_info->chan_type = UE_LOGCHAN_DCCH;
  }
  else
  {
    dlchan_info->chan_type = UE_LOGCHAN_DTCH;
  }

  /* Save the logical channel mode, if a setup */
  if(logch_mode != UE_MODE_MAX_NUM)
  {
    dlchan_info->rlc_mode = logch_mode;
  }
}  /* tdsrrcllc_update_ul_ded_log_chl_info */


/*==========================================================
FUNCTION  : tdsrrcllc_get_rach_mapping_info()

DESCRIPTION : Process RACH Mapping IE's contents and saves 
              them in the MAC Ordered Configuration Database.
              If next RRC state is CELL_DCH, save the RACH
              mapping contents in a local data structure in
              OC.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rach_mapping_info
(
  tdsrrc_UL_LogicalChannelMapping *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode,
  tdsrrc_state_e_type             next_rrc_state
)
{
  /* transport Channel Index */
  uint8   trch_idx  = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Get the logical channel index */
  uint8   logch_idx;
 
  tdsmac_ul_ded_logch_config_type  *dlchan_info;
  rlc_lc_id_type   rlc_lc_id = (uint8)TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
  /* TBD: Although all existing FACHs would be mapped to each of the RBs,
   * fill up only for the 1st FACH - till MAC interface is clarified/updated
   */

  boolean update_rach_rlc_id = FALSE;

  uint8 cnt;

  /* Temporary pointer for processing the RLC Size List */
  tdsrrcllcoc_rlc_size_list_type *rlc_size_rec = NULL;


  /* Start with updating Cell FACH RB Mapping Info when the next state is
   * either CELL_DCH or CELL_FACH
   */
  logch_idx = tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan;
  dlchan_info = &tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[logch_idx];


  /* Save the RLC Size List information */
  tdsordered_config_ptr->rach_rlc_size_restriction_info.mac_update_needed = TRUE;
  rlc_size_rec =
    &(tdsordered_config_ptr->rach_rlc_size_restriction_info.rlc_size_list[logch_idx]);
  if(tdsrrcllc_get_rlc_size_list(ie_ptr, rlc_size_rec, rb_id) == FAILURE)
  {
    return FAILURE;
  }


  if(logch_mode == UE_MODE_MAX_NUM)
  {
    /* This is the case when RB mapping info applicable to CELL_FACH is coming 
    * in "Affected List". At this point, we need to update the Logical Channel mode 
    * and Logical Channel ID in Cell FACH RB Mapping Info - 
    * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
    */
    rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if(rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid UL LC ID for RB %d",rb_id);
      return(FAILURE);
    }
    
    if(tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get UL Log Chl mode for LC %d",rlc_lc_id);
    }
    /* Also update the Logical Channel ID here */
    dlchan_info->rlc_id = rlc_lc_id;
    update_rach_rlc_id = TRUE;
  }
  else
  {
    rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if(rlc_lc_id  != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      /* Also update the Logical Channel ID here */
      dlchan_info->rlc_id = rlc_lc_id;
      update_rach_rlc_id = TRUE;
    }
  }
  
  for (cnt = 0; cnt < tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan ; cnt++) 
  {
    if(rb_id == tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[cnt].rb_id)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB_ID %d already present",rb_id);  
      return (SUCCESS); 
    }
  }


  tdsrrcllc_update_ul_ded_log_chl_info(ie_ptr, dlchan_info, rb_id, logch_mode);
  tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan ++;  
   
  /* Now update the FACH Info ONLY IF the next state is CELL_FACH */
  if(next_rrc_state == TDSRRC_STATE_CELL_FACH ||
     next_rrc_state == TDSRRC_STATE_CELL_PCH ||
     next_rrc_state == TDSRRC_STATE_URA_PCH)
  {
    if((trch_idx = tdsrrcllc_find_ul_rach_trch_idx()) >= TDSUE_MAX_TRCH)
    {
      return FAILURE;
    }
    logch_idx = tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan;
    dlchan_info = &tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx];
    if(update_rach_rlc_id)
    {
      dlchan_info->rlc_id = rlc_lc_id;
    }
    tdsrrcllc_update_ul_ded_log_chl_info(ie_ptr, dlchan_info, rb_id, logch_mode);

    tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan++;
    tdsordered_config_ptr->ul_rach_trch_info[trch_idx].lg_ch_cnt = 
      tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan;

  }  /* If next state is CELL_FACH, update the FACH Info */
 
  return SUCCESS;

} /* end tdsrrcllc_get_rach_mapping_info() */


/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_single_logical_channel_info()

DESCRIPTION : Process one UL Logical Channel Mapping IE's 
              contents and saves them in the MAC Ordered 
              Configuration Database.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_single_logical_channel_info
(
  tdsrrc_UL_LogicalChannelMapping *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
)
{
  tdsordered_config_type *tmp_config_ptr = NULL;


  /* Transport channel type */
  switch (ie_ptr->ul_TransportChannelType.t)
  {
    case T_tdsrrc_UL_TransportChannelType_dch:
      if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
      {
        if (tdsrrcllc_get_ul_dch_mapping_info(ie_ptr, rb_id, logch_mode) == FAILURE)
        {
          return FAILURE;
        }
      }
      else
      {
          if(tdstransition_config.toc_usage == TDSTOC_INVALID)
          {
            tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
          }

          tmp_config_ptr = tdsordered_config_ptr;
          tdsordered_config_ptr = tdstransition_config.toc_ptr;

          /* Processing functions must think we are going to Cell_DCH state. */
          tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;

          if(tdsrrcllc_get_ul_dch_mapping_info(ie_ptr, rb_id, logch_mode) == FAILURE)
          {
            tdsordered_config_ptr = tmp_config_ptr;
            tmp_config_ptr = NULL;
            return FAILURE;
          }

          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
      }
      break;

    case T_tdsrrc_UL_TransportChannelType_rach:
      if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH ||
         tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH ||
         tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
      {
        if (tdsrrcllc_get_rach_mapping_info(ie_ptr, rb_id, logch_mode,
                            tdsordered_config_ptr->rrc_state_indicator) == FAILURE)
        {
          return FAILURE;
        }
      }
      else
      {
        if (tdsrrcllc_get_rach_mapping_info(ie_ptr, rb_id, logch_mode, TDSRRC_STATE_CELL_DCH) == FAILURE)
        {
          return FAILURE;
        }
      }
      break;

    case T_tdsrrc_UL_TransportChannelType_dummy:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DUMMY chosen by NW");
      return FAILURE;

    case T_tdsrrc_UL_TransportChannelType_usch:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"USCH not supported");
      return FAILURE;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd invalid UL TrCh type %d",ie_ptr->ul_TransportChannelType.t);
      return FAILURE;
  }

  return SUCCESS;

} /* end tdsrrcllc_get_ul_single_logical_channel_info() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_logical_channel_info()

DESCRIPTION : Get UL logical Channel info.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_logical_channel_info
(
  tdsrrc_UL_LogicalChannelMappings * ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
)
{

  /* Process IE info based on number of UL Logical Channels */
  switch (ie_ptr->t)
  {
    /* One UL Logical Channel */
    case T_tdsrrc_UL_LogicalChannelMappings_oneLogicalChannel:
      if(tdsrrcllc_get_ul_single_logical_channel_info(ie_ptr->u.oneLogicalChannel, rb_id, logch_mode)
         == FAILURE)
      {
        return FAILURE;
      }
      break;

    /* Two logical channels, currently not supported by RRC-RLC I/F */
    case T_tdsrrc_UL_LogicalChannelMappings_twoLogicalChannels:
      /* TRUE indicates that first logical channel is for Data PDUs and second  
         logical channel is for control PDUs */
      /* FALSE indicates that Data and Control PDUs can be sent on either of them */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Support only 1 Log CH per AM");
        return FAILURE;

      /* Invalid number of logical channels */
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid number of UL logical channels",ie_ptr->t );
        return FAILURE;
  } /* end switch */

  return SUCCESS;

} /* end tdsrrcllc_get_ul_logical_channel_info() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_logical_channel_info()

DESCRIPTION : Get DL logical Channel info.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_logical_channel_info
(
  tdsrrc_DL_LogicalChannelMappingList * ie_ptr,
  tdsrrc_RB_Identity                       rb_id,
  uecomdef_logch_mode_e_type       logch_mode
)
{
  /*
  * Count to make sure that don't exceed number of Logical
  * channels that can be Mapped to a Transport Channel.
  */
  uint8 lc_tc_cnt = 0;

  tdsordered_config_type *tmp_config_ptr = NULL;


  /* Process each item of the list */
  while((ie_ptr->n > lc_tc_cnt) && (lc_tc_cnt < MAX_DED_LOGCHAN_PER_TRCH))
  {
    /* Transport channel type */
    switch (ie_ptr->elem[lc_tc_cnt].dl_TransportChannelType.t)
    {
      case T_tdsrrc_DL_TransportChannelType_dch:
        if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
        {
          if(tdsrrcllc_get_dl_dch_mapping_info(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
          {
            return FAILURE;
          }
        }
        else
        {

          if(tdstransition_config.toc_usage == TDSTOC_INVALID)
          {
            tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
          }

          tmp_config_ptr = tdsordered_config_ptr;
          tdsordered_config_ptr = tdstransition_config.toc_ptr;

          /* Processing functions must think we are going to Cell_DCH state. */
          tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;

          if(tdsrrcllc_get_dl_dch_mapping_info(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
          {
            tdsordered_config_ptr = tmp_config_ptr;
            tmp_config_ptr = NULL;
            return FAILURE;
          }

          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
        }
        break;

      case T_tdsrrc_DL_TransportChannelType_fach:
        if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH ||
           tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH ||
           tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
        {

         /* If Logical Channel ID is present, save MAC Logical Channel Identity */ 
           if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[lc_tc_cnt],tdsrrc_DL_LogicalChannelMapping,logicalChannelIdentity))
           {
             if(tdsrrcllc_get_fach_mapping_info(ie_ptr->elem[lc_tc_cnt].logicalChannelIdentity, rb_id, logch_mode,
                                      tdsordered_config_ptr->rrc_state_indicator) == FAILURE)
             {
               return FAILURE;
             }
           }
           else
           {
             if(tdsrrcllc_get_fach_mapping_info(TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT, rb_id, logch_mode,
                                      tdsordered_config_ptr->rrc_state_indicator) == FAILURE)
             {
               return FAILURE;
             }
           }

          
        }
        else
        {
          /* FACH mapping info can come in DCH state. Accept and update
           * local mapping info structure.
           */
         /* If Logical Channel ID is present, save MAC Logical Channel Identity */ 
           if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[lc_tc_cnt],tdsrrc_DL_LogicalChannelMapping,logicalChannelIdentity))
           {
             if(tdsrrcllc_get_fach_mapping_info(ie_ptr->elem[lc_tc_cnt].logicalChannelIdentity, rb_id, logch_mode,
                                      TDSRRC_STATE_CELL_DCH) == FAILURE)
             {
               return FAILURE;
             }
           }
           else
           {
             if(tdsrrcllc_get_fach_mapping_info(TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT, rb_id, logch_mode,
                                      TDSRRC_STATE_CELL_DCH) == FAILURE)
             {
               return FAILURE;
             }
           }
        }
        break;

      case T_tdsrrc_DL_TransportChannelType_dsch:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSCH TrCH not supported");
        return FAILURE;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcv'd invalid DL TrCh type",ie_ptr->elem[lc_tc_cnt].dl_TransportChannelType.t);
        return FAILURE;
    } /* end Logical channel type switch */

    /* Get pointer to the next item in the list */
    lc_tc_cnt++;
  } /* end while ie_ptr != NULL */

  return SUCCESS;

} /* end tdsrrcllc_get_dl_logical_channel_info() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_mapping_info()

DESCRIPTION : Get Radio Bearer Mapping Info IE

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rb_mapping_info
(
  tdsrrc_RB_MappingInfo         * ie_ptr,
  tdsrrc_RB_Identity                 rb_id,
  uecomdef_logch_mode_e_type logch_mode
)
{
  /*
  * Count to make sure that don't exceed the maximum number 
  * of RB Mux options
  */
  uint8 rb_mux_cnt = 0;

  /* Process the RB Mux option IE in the linked list */
  while((ie_ptr->n > rb_mux_cnt) && (rb_mux_cnt < MAX_RB_MUX_OPTIONS))
  {
    /* Check for DL Channel Mapping List */
    if(TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[rb_mux_cnt],tdsrrc_RB_MappingOption,dl_LogicalChannelMappingList))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for DL",rb_id);
      if(tdsrrcllc_get_dl_logical_channel_info(&ie_ptr->elem[rb_mux_cnt].dl_LogicalChannelMappingList,
                                     rb_id, logch_mode) == FAILURE)
      {
        return FAILURE;
      }
      /* Here indicate that MAC Downlink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
    } /* end dl Log Ch mapping list */

    if(TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[rb_mux_cnt],tdsrrc_RB_MappingOption,ul_LogicalChannelMappings))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for UL",rb_id);
      if(tdsrrcllc_get_ul_logical_channel_info(&ie_ptr->elem[rb_mux_cnt].ul_LogicalChannelMappings,
                                     rb_id, logch_mode) == FAILURE)
      {
        return FAILURE;
      }

      /* Here indicate that MAC Uplink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
    } /* end ul Log Ch mapping list */

    /* Pointer to the next item in the IE list */
  
    rb_mux_cnt++;

  } /* end loop of RB Mapping info list */

  return SUCCESS;

} /* end tdsrrcllc_get_rb_mapping_info() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_maxmrw

DESCRIPTION : 
  SDU Discard -- Max_MRW infomation interpretation.
  See 10.3.4.25 in 25.331.

DEPENDENCIES:
  None.

RETURN VALUE:
  The max value for the number of retransmissions of a MRW 
  command for RLC.

SIDE EFFECTS:
  None.
============================================================*/
static uint32 tdsrrcllc_get_maxmrw
(
  tdsrrc_MaxMRW maxmrw
)
{
  /* Init MaxMRW */
  uint32 rlc_maxmrw = TDSRRCLLC_INVALID_RLC_DISCARD;

  switch (maxmrw)
  {
    case tdsrrc_MaxMRW_mm1:
      rlc_maxmrw = 1;
      break;
    case tdsrrc_MaxMRW_mm4:
      rlc_maxmrw = 4;
      break;
    case tdsrrc_MaxMRW_mm6:
      rlc_maxmrw = 6;
      break;
    case tdsrrc_MaxMRW_mm8:
      rlc_maxmrw = 8;
      break;
    case tdsrrc_MaxMRW_mm12:
      rlc_maxmrw = 12;
      break;
    case tdsrrc_MaxMRW_mm16:
      rlc_maxmrw = 16;
      break;
    case tdsrrc_MaxMRW_mm24:
      rlc_maxmrw = 24;
      break;
    case tdsrrc_MaxMRW_mm32:
      rlc_maxmrw = 32;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Max_MRW type",maxmrw);
      break;
  } /* end MaxMRW choice */

  return (rlc_maxmrw);

} /* end tdsrrcllc_get_maxmrw() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_timer_mrw()

DESCRIPTION : 
  SDU discard -- Timer MRW infomation interpretation.
  See 10.3.4.25 in 25.331.

DEPENDENCIES:
  None.

RETURN VALUE:
  Timer MRW in SDU discard information for RLC.

SIDE EFFECTS:
  None.
============================================================*/
static uint16 tdsrrcllc_get_timer_mrw
(
  tdsrrc_TimerMRW timer_mrw
)
{
  /* Init Timer_MRW */
  uint16 rlc_timer_mrw = (uint16)TDSRRCLLC_INVALID_RLC_DISCARD;

  switch (timer_mrw)
  {
    case tdsrrc_TimerMRW_te50:
      rlc_timer_mrw = 50;
      break;
    case tdsrrc_TimerMRW_te60:
      rlc_timer_mrw = 60;
      break;
    case tdsrrc_TimerMRW_te70:
      rlc_timer_mrw = 70;
      break;
    case tdsrrc_TimerMRW_te80:
      rlc_timer_mrw = 80;
      break;
    case tdsrrc_TimerMRW_te90:
      rlc_timer_mrw = 90;
      break;
    case tdsrrc_TimerMRW_te100:
      rlc_timer_mrw = 100;
      break;
    case tdsrrc_TimerMRW_te120:
      rlc_timer_mrw = 120;
      break;
    case tdsrrc_TimerMRW_te140:
      rlc_timer_mrw = 140;
      break;
    case tdsrrc_TimerMRW_te160:
      rlc_timer_mrw = 160;
      break;
    case tdsrrc_TimerMRW_te180:
      rlc_timer_mrw = 180;
      break;
    case tdsrrc_TimerMRW_te200:
      rlc_timer_mrw = 200;
      break;
    case tdsrrc_TimerMRW_te300:
      rlc_timer_mrw = 300;
      break;
    case tdsrrc_TimerMRW_te400:
      rlc_timer_mrw = 400;
      break;
    case tdsrrc_TimerMRW_te500:
      rlc_timer_mrw = 500;
      break;
    case tdsrrc_TimerMRW_te700:
      rlc_timer_mrw = 700;
      break;
    case tdsrrc_TimerMRW_te900:
      rlc_timer_mrw = 900;
      break;
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown Timer MRW type");
      break;
  } /* end Timer_MRW choice */

  return (rlc_timer_mrw);

} /* end tdsrrcllc_get_timer_mrw() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_max_dat()

DESCRIPTION : 
  SDU discard -- Max_DAT infomation interpretation.
  See 10.3.4.25 in 25.331.

DEPENDENCIES:
  None.

RETURN VALUE:
  Number of retransmissions of a PU before a SDU is discarded 
  for RLC.

SIDE EFFECTS:
  None.
============================================================*/
static uint32 tdsrrcllc_get_max_dat
(
  tdsrrc_MaxDAT max_dat
)
{
  /* Init Max_DAT */
  uint32 rlc_max_dat = TDSRRCLLC_INVALID_RLC_DISCARD;

  switch (max_dat)
  {
    case tdsrrc_MaxDAT_dat1:
      rlc_max_dat = 1;
      break;
    case tdsrrc_MaxDAT_dat2:
      rlc_max_dat = 2;
      break;
    case tdsrrc_MaxDAT_dat3:
      rlc_max_dat = 3;
      break;
    case tdsrrc_MaxDAT_dat4:
      rlc_max_dat = 4;
      break;
    case tdsrrc_MaxDAT_dat5:
      rlc_max_dat = 5;
      break;
    case tdsrrc_MaxDAT_dat6:
      rlc_max_dat = 6;
      break;
    case tdsrrc_MaxDAT_dat7:
      rlc_max_dat = 7;
      break;
    case tdsrrc_MaxDAT_dat8:
      rlc_max_dat = 8;
      break;
    case tdsrrc_MaxDAT_dat9:
      rlc_max_dat = 9;
      break;
    case tdsrrc_MaxDAT_dat10:
      rlc_max_dat = 10;
      break;
    case tdsrrc_MaxDAT_dat15:
      rlc_max_dat = 15;
      break;
    case tdsrrc_MaxDAT_dat20:
      rlc_max_dat = 20;
      break;
    case tdsrrc_MaxDAT_dat25:
      rlc_max_dat = 25;
      break;
    case tdsrrc_MaxDAT_dat30:
      rlc_max_dat = 30;
      break;
    case tdsrrc_MaxDAT_dat35:
      rlc_max_dat = 35;
      break;
    case tdsrrc_MaxDAT_dat40:
      rlc_max_dat = 40;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Max_DAT %d",max_dat);
      break;
  } /* end Max_DAT choice */

  return (rlc_max_dat);

} /* end tdsrrcllc_get_max_dat() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_timer_discard()

DESCRIPTION : 
  SDU discard -- timer discard infomation interpretation.
  See 10.3.4.25 in 25.331.

DEPENDENCIES:
  None.

RETURN VALUE:
  Time, in MS, that should elapse before an SDU is discarded for RLC.

SIDE EFFECTS:
  None.
============================================================*/
static uint32 tdsrrcllc_get_timer_discard
(
  tdsrrc_TimerDiscard timerDiscard
)
{
  /* Init Max_DAT */
  uint32 rlc_timer_discard = TDSRRCLLC_INVALID_RLC_DISCARD;

  switch (timerDiscard)
  {
    case tdsrrc_TimerDiscard_td0_1:
      rlc_timer_discard = 100;
      break;
    case tdsrrc_TimerDiscard_td0_25:
      rlc_timer_discard = 250;
      break;
    case tdsrrc_TimerDiscard_td0_5:
      rlc_timer_discard = 500;
      break;
    case tdsrrc_TimerDiscard_td0_75:
      rlc_timer_discard = 750;
      break;
    case tdsrrc_TimerDiscard_td1:
      rlc_timer_discard = 1000;
      break;
    case tdsrrc_TimerDiscard_td1_25:
      rlc_timer_discard = 1250;
      break;
    case tdsrrc_TimerDiscard_td1_5:
      rlc_timer_discard = 1500;
      break;
    case tdsrrc_TimerDiscard_td1_75:
      rlc_timer_discard = 1750;
      break;
    case tdsrrc_TimerDiscard_td2:
      rlc_timer_discard = 2000;
      break;
    case tdsrrc_TimerDiscard_td2_5:
      rlc_timer_discard = 2500;
      break;
    case tdsrrc_TimerDiscard_td3:
      rlc_timer_discard = 3000;
      break;
    case tdsrrc_TimerDiscard_td3_5:
      rlc_timer_discard = 3500;
      break;
    case tdsrrc_TimerDiscard_td4:
      rlc_timer_discard = 4000;
      break;
    case tdsrrc_TimerDiscard_td4_5:
      rlc_timer_discard = 4500;
      break;
    case tdsrrc_TimerDiscard_td5:
      rlc_timer_discard = 5000;
      break;
    case tdsrrc_TimerDiscard_td7_5:
      rlc_timer_discard = 7500;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown TimerDiscard %d",timerDiscard);
      break;
  } /* end TimerDiscard choice */

  return (rlc_timer_discard);

} /* end tdsrrcllc_get_timer_discard() */

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
)
{
  uint8 ul_um_idx = 0;
  tdsordered_config_type * config_ptr = NULL;
  
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }
  
  for(ul_um_idx = 0; ul_um_idx < config_ptr->rlc_ul_um_parms.nchan; ul_um_idx++)
  {
    if(config_ptr->rlc_ul_um_parms.chan_info[ul_um_idx].rb_id == rb_id)
    {
      return config_ptr->rlc_ul_um_parms.chan_info[ul_um_idx].timer_discard;
    }
  }
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL UM SDU Discard not found in OC");
  return 0;
}

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
)
{
  /* Init with zero. Basically, if we set it to zero, RLC will not start timer for UL UM SDU.*/
  uint16 rlc_timer_discard = 0;

  switch (noExplicitDiscard)
  {
    case tdsrrc_NoExplicitDiscard_dt10:
      rlc_timer_discard = 10;
      break;
    case tdsrrc_NoExplicitDiscard_dt20:
      rlc_timer_discard = 20;
      break;
    case tdsrrc_NoExplicitDiscard_dt30:
      rlc_timer_discard = 30;
      break;
    case tdsrrc_NoExplicitDiscard_dt40:
      rlc_timer_discard = 40;
      break;
    case tdsrrc_NoExplicitDiscard_dt50:
      rlc_timer_discard = 50;
      break;
    case tdsrrc_NoExplicitDiscard_dt60:
      rlc_timer_discard = 60;
      break;
    case tdsrrc_NoExplicitDiscard_dt70:
      rlc_timer_discard = 70;
      break;
    case tdsrrc_NoExplicitDiscard_dt80:
      rlc_timer_discard = 80;
      break;
    case tdsrrc_NoExplicitDiscard_dt90:
      rlc_timer_discard = 90;
      break;
    case tdsrrc_NoExplicitDiscard_dt100:
      rlc_timer_discard = 100;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown NoExplicitDiscard %d",noExplicitDiscard);
      break;
  } /* end tdsrrc_NoExplicitDiscard choice */
  return (rlc_timer_discard);
} /* end tdsrrcllc_ul_um_timer_discard() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_xmit_rlc_discard

DESCRIPTION : 
  Config the SDU discard informtion for RLC.

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_xmit_rlc_discard
(
  tdsrlc_ul_info_type            *rlc_ul_info,
  tdsrrc_TransmissionRLC_Discard *ie_ptr
)
{
  /* temporary storage for various IE values */
  uint32 temp_val = TDSRRCLLC_INVALID_RLC_DISCARD;

  /* indicated that there is information for RLC */
  rlc_ul_info->discard_info.tx_discard_incl = TRUE;

  switch (ie_ptr->t)
  {

    case T_tdsrrc_TransmissionRLC_Discard_timerBasedNoExplicit:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"UL RLC TM No Explicit Timer Discard not supported");
      return FAILURE;

    case T_tdsrrc_TransmissionRLC_Discard_timerBasedExplicit:
      /* Save the Discard Mode */
      rlc_ul_info->discard_info.discard_mode = TDSRLC_TBE;
      
      /* Get the timer discard value */
      temp_val = tdsrrcllc_get_timer_discard(ie_ptr->u.timerBasedExplicit->timerDiscard);
      if(temp_val == TDSRRCLLC_INVALID_RLC_DISCARD)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Get timer value is TDSRRCLLC_INVALID_RLC_DISCARD");
        return FAILURE;
      }
      else
      {
        rlc_ul_info->discard_info.timer_discard = (uint16)temp_val;
      }

      /* Get the Timer_MRW */
      temp_val = tdsrrcllc_get_timer_mrw(ie_ptr->u.timerBasedExplicit->timerMRW);
      if(temp_val == TDSRRCLLC_INVALID_RLC_DISCARD)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Get timer value is TDSRRCLLC_INVALID_RLC_DISCARD");
        return FAILURE;
      }
      else
      {
        rlc_ul_info->discard_info.timer_mrw = (uint16)temp_val;
      }

      /* Get the MaxMRW */
      temp_val = tdsrrcllc_get_maxmrw(ie_ptr->u.timerBasedExplicit->maxMRW);
      if(temp_val == TDSRRCLLC_INVALID_RLC_DISCARD)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Get timer value is TDSRRCLLC_INVALID_RLC_DISCARD");
        return FAILURE;
      }
      else
      {
        rlc_ul_info->discard_info.maxmrw = (uint8)temp_val;
      }

      break;


    case T_tdsrrc_TransmissionRLC_Discard_maxDAT_Retransmissions:
      /* Save the Discard Mode */
      rlc_ul_info->discard_info.discard_mode = TDSRLC_MAXDAT;
      
      /* Get the Max_DAT */
      temp_val = tdsrrcllc_get_max_dat(ie_ptr->u.maxDAT_Retransmissions->maxDAT);
      if(temp_val == TDSRRCLLC_INVALID_RLC_DISCARD)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Get timer value is TDSRRCLLC_INVALID_RLC_DISCARD");
        return FAILURE;
      }
      else
      {
        rlc_ul_info->discard_info.maxdat = (uint8)temp_val;
      }

      /* Get the Timer_MRW */
      temp_val = tdsrrcllc_get_timer_mrw(ie_ptr->u.maxDAT_Retransmissions->timerMRW);
      if(temp_val == TDSRRCLLC_INVALID_RLC_DISCARD)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Get timer value is TDSRRCLLC_INVALID_RLC_DISCARD");
        return FAILURE;
      }
      else
      {
        rlc_ul_info->discard_info.timer_mrw = (uint16)temp_val;
      }

      /* Get the MaxMRW */
      temp_val = tdsrrcllc_get_maxmrw(ie_ptr->u.maxDAT_Retransmissions->maxMRW);
      if(temp_val == TDSRRCLLC_INVALID_RLC_DISCARD)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Get timer value is TDSRRCLLC_INVALID_RLC_DISCARD");
        return FAILURE;
      }
      else
      {
        rlc_ul_info->discard_info.maxmrw = (uint8)temp_val;
      }
    
      break; /* End Max DAT retransmissions case */

    case T_tdsrrc_TransmissionRLC_Discard_noDiscard:
      /* Save the Discard Mode */
      rlc_ul_info->discard_info.discard_mode = TDSRLC_NO_DISCARD;
      
      /* Get the Max_DAT */
      temp_val = tdsrrcllc_get_max_dat(ie_ptr->u.noDiscard);
      if ((temp_val == TDSRRCLLC_INVALID_RLC_DISCARD) || (temp_val == 1))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Get timer value is TDSRRCLLC_INVALID_RLC_DISCARD");
        return FAILURE;
      }
      else
      {
        rlc_ul_info->discard_info.maxdat = (uint8)temp_val;
      }
      break; /* End No Discard case */
    
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Discard mode %d",ie_ptr->t);
      return FAILURE;
  }

  /* everything is okay */
  return SUCCESS;

} /* end tdsrrcllc_get_xmit_rlc_discard() */
 
/*==========================================================
FUNCTION  : tdsrrcllc_get_polling_info()

DESCRIPTION : 
  Config the polling information for RLC.

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_polling_info
(
  tdsrlc_ul_info_type *rlc_ul_info,
  tdsrrc_PollingInfo  *ie_ptr
)
{
  /* indicate that polling info is included */
  rlc_ul_info->poll_info.poll_info_incl = TRUE;
              
  /*------------------------------------------------- 
    Mandatory parameters.
  ---------------------------------------------------*/
  
  /* assign the boolean value of last_tx_pu_poll  */
  rlc_ul_info->poll_info.last_tx_pdu_poll = ie_ptr->lastTransmissionPDU_Poll;
              
  /* assign the boolean value of last_retx_pu_poll  */
  rlc_ul_info->poll_info.last_retx_pdu_poll = ie_ptr->lastRetransmissionPDU_Poll;

  /*------------------------------------------------- 
    Optional parameters
  ---------------------------------------------------*/

  /*
  * Timer Poll Prohibit. If the timer poll prohibit value is between 10 and 500, 
  * the step size is 10. If the Timer poll prohibit value is between 600 and 1000
  * the step size is 50. All other values are out of range.
  */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,timerPollProhibit))
  {
    /* Get the minimum time between polls in ms, for RLC UL AM */
    if(ie_ptr->timerPollProhibit <= tdsrrc_TimerPollProhibit_tpp550)
    {
      rlc_ul_info->poll_info.poll_prohibit = ((((uint16)ie_ptr->timerPollProhibit) + 1) * 10);
    }
    /*lint -save -e685 */
    else if((ie_ptr->timerPollProhibit >= tdsrrc_TimerPollProhibit_tpp550) && (ie_ptr->timerPollProhibit <= tdsrrc_TimerPollProhibit_tpp1000))
    {
      rlc_ul_info->poll_info.poll_prohibit = (600 + (((uint16)ie_ptr->timerPollProhibit - (uint16)tdsrrc_TimerPollProhibit_tpp600)) * 50);
    }
    /*lint -restore */
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Timer poll prohibit is out of bounds % d",ie_ptr->timerPollProhibit);
      return FAILURE;
    }
  } /* end timer poll prohibit check */
  else
  {
    /* A value of 0 indicates that Timer Poll Prohibit is absent */
    rlc_ul_info->poll_info.poll_prohibit = 0;
  }

  /*
  * Timer Poll. If the timer poll  value is between 10 and 500, the step size
  * is 10. If the Timer poll value is between 600 and 1000, the step size is 50.
  * All other values are out of range.
  */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,timerPoll))
  {
    if(ie_ptr->timerPoll <= tdsrrc_TimerPoll_tp550)
    {
      rlc_ul_info->poll_info.timer_poll = (((uint16)ie_ptr->timerPoll + 1) * 10);
    }
    /*lint -save -e685 */
    else if((ie_ptr->timerPoll >= tdsrrc_TimerPoll_tp550) && (ie_ptr->timerPoll <= tdsrrc_TimerPoll_tp1000))
    {
      rlc_ul_info->poll_info.timer_poll = (uint16)(600 + ((uint16)ie_ptr->timerPoll - (uint16)tdsrrc_TimerPoll_tp600) * 50);
    }
    /*lint -restore */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Timer poll is out of bound");
      return FAILURE;
    }
  } /* end timer poll check */
  else
  {
    /* A value of 0 indicates Timer Poll is absent */
    rlc_ul_info->poll_info.timer_poll = 0;
  }

  /* Number of PU's interval between polling */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,poll_PDU))
  {
    switch (ie_ptr->poll_PDU)
    {
      case tdsrrc_Poll_PDU_pdu1:
        rlc_ul_info->poll_info.poll_pdu = 1;
        break;
      case tdsrrc_Poll_PDU_pdu2:
        rlc_ul_info->poll_info.poll_pdu = 2;
        break;
      case tdsrrc_Poll_PDU_pdu4:
        rlc_ul_info->poll_info.poll_pdu = 4;
        break;
      case tdsrrc_Poll_PDU_pdu8:
        rlc_ul_info->poll_info.poll_pdu = 8;
        break;
      case tdsrrc_Poll_PDU_pdu16:
        rlc_ul_info->poll_info.poll_pdu = 16;
        break;
      case tdsrrc_Poll_PDU_pdu32:
        rlc_ul_info->poll_info.poll_pdu = 32;
        break;
      case tdsrrc_Poll_PDU_pdu64:
        rlc_ul_info->poll_info.poll_pdu = 64;
        break;
      case tdsrrc_Poll_PDU_pdu128:
        rlc_ul_info->poll_info.poll_pdu = 128;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Pull_PU type %d",ie_ptr->poll_PDU);
        return FAILURE;
    } /* end poll pu choice */
  } /* end poll pu check */
  else
  {
    /* A value of 0 indicates Poll PDU is absent */
    rlc_ul_info->poll_info.poll_pdu = 0;
  }

  /* # of SDUs interval between polling  */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,poll_SDU)) 
  {
    switch (ie_ptr->poll_SDU)
    {
      case tdsrrc_Poll_SDU_sdu1:
        rlc_ul_info->poll_info.poll_sdu = 1;
        break;
      case tdsrrc_Poll_SDU_sdu4:
        rlc_ul_info->poll_info.poll_sdu = 4;
        break;
      case tdsrrc_Poll_SDU_sdu16:
        rlc_ul_info->poll_info.poll_sdu = 16;
        break;
      case tdsrrc_Poll_SDU_sdu64:
        rlc_ul_info->poll_info.poll_sdu = 64;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Pull_SDU type %d",ie_ptr->poll_SDU);
        return FAILURE;
    } /* end poll sdu choice */
  } /* end poll sdu check */
  else
  {
    /* A value of 0 indicates Poll SDU is absent */
    rlc_ul_info->poll_info.poll_sdu = 0;
  }

  /* % of transmission window for polling */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,pollWindow)) 
  {
    switch (ie_ptr->pollWindow)
    {
      case tdsrrc_PollWindow_pw50:
        rlc_ul_info->poll_info.poll_window = 50;
        break;
      case tdsrrc_PollWindow_pw60:
        rlc_ul_info->poll_info.poll_window = 60;
        break;
      case tdsrrc_PollWindow_pw70:
        rlc_ul_info->poll_info.poll_window = 70;
        break;
      case tdsrrc_PollWindow_pw80:
        rlc_ul_info->poll_info.poll_window = 80;
        break;
      case tdsrrc_PollWindow_pw85:
        rlc_ul_info->poll_info.poll_window = 85;
        break;
      case tdsrrc_PollWindow_pw90:
        rlc_ul_info->poll_info.poll_window = 90;
        break;
      case tdsrrc_PollWindow_pw95:
        rlc_ul_info->poll_info.poll_window = 95;
        break;
      case tdsrrc_PollWindow_pw99:
        rlc_ul_info->poll_info.poll_window = 99;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown pollWindow type %d",ie_ptr->pollWindow);
        return FAILURE;
    } /* end poll window choice */
  } /* end poll window check */
  else
  {
    rlc_ul_info->poll_info.poll_window = 0;
  }

  /* Tiem in ms timer for Timer for Periodic polling */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,timerPollPeriodic)) 
  {
    switch (ie_ptr->timerPollPeriodic)
    {
      case tdsrrc_TimerPollPeriodic_tper100:
        rlc_ul_info->poll_info.timer_poll_periodic = 100;
        break;
      case tdsrrc_TimerPollPeriodic_tper200:
        rlc_ul_info->poll_info.timer_poll_periodic = 200;
        break;
      case tdsrrc_TimerPollPeriodic_tper300:
        rlc_ul_info->poll_info.timer_poll_periodic = 300;
        break;
      case tdsrrc_TimerPollPeriodic_tper400:
        rlc_ul_info->poll_info.timer_poll_periodic = 400;
        break;
      case tdsrrc_TimerPollPeriodic_tper500:
        rlc_ul_info->poll_info.timer_poll_periodic = 500;
        break;
      case tdsrrc_TimerPollPeriodic_tper750:
        rlc_ul_info->poll_info.timer_poll_periodic = 750;
        break;
      case tdsrrc_TimerPollPeriodic_tper1000:
        rlc_ul_info->poll_info.timer_poll_periodic = 1000;
        break;
      case tdsrrc_TimerPollPeriodic_tper2000:
        rlc_ul_info->poll_info.timer_poll_periodic = 2000;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Timer Poll Periodic type %d",ie_ptr->timerPollPeriodic);
        return FAILURE;
    } /* end timer poll periodic choice */
  } /* end timer poll periodic check */
  else
  {
    rlc_ul_info->poll_info.timer_poll_periodic = 0;
  }

  /* If made it this far, then all things are a go */
  return SUCCESS;

} /* end tdsrrcllc_get_polling_info() */

/*==========================================================
FUNCTION  : tdsrrcllc_rx_window_size()

DESCRIPTION : 
  Return the Window Size for receiving.

DEPENDENCIES: 
  None.

RETURN VALUE:
  Window size for receiving for RLC.

SIDE EFFECTS:
  None.
============================================================*/
uint16 tdsrrcllc_rx_window_size
(
  tdsrrc_ReceivingWindowSize rrc_rx_window_size
)
{
  /* Initialize the window size  */
  uint16 rlc_rx_win_size = (uint16)TDSRRCLLC_INVALID_WINDOW_SIZE;

  switch (rrc_rx_window_size)
  {
    case tdsrrc_ReceivingWindowSize_rw1:
      rlc_rx_win_size = 1;
      break;
    case tdsrrc_ReceivingWindowSize_rw8:
      rlc_rx_win_size = 8;
      break;
    case tdsrrc_ReceivingWindowSize_rw16:
      rlc_rx_win_size = 16;
      break;
    case tdsrrc_ReceivingWindowSize_rw32:
      rlc_rx_win_size = 32;
      break;
    case tdsrrc_ReceivingWindowSize_rw64:
      rlc_rx_win_size = 64;
      break;
    case tdsrrc_ReceivingWindowSize_rw128:
      rlc_rx_win_size = 128;
      break;
    case tdsrrc_ReceivingWindowSize_rw256:
      rlc_rx_win_size = 256;
      break;
    case tdsrrc_ReceivingWindowSize_rw512:
      rlc_rx_win_size = 512;
      break;
    case tdsrrc_ReceivingWindowSize_rw768:
      rlc_rx_win_size = 768;
      break;
    case tdsrrc_ReceivingWindowSize_rw1024:
      rlc_rx_win_size = 1024;
      break;
    case tdsrrc_ReceivingWindowSize_rw1536:
      rlc_rx_win_size = 1536;
      break;
    case tdsrrc_ReceivingWindowSize_rw2047:
      rlc_rx_win_size = 2047;
      break;
    case tdsrrc_ReceivingWindowSize_rw2560:
      rlc_rx_win_size = 2560;
      break;
    case tdsrrc_ReceivingWindowSize_rw3072:
      rlc_rx_win_size = 3072;
      break;
    case tdsrrc_ReceivingWindowSize_rw3584:
      rlc_rx_win_size = 3584;
      break;
    case tdsrrc_ReceivingWindowSize_rw4095:
      rlc_rx_win_size = 4095;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Window Size %d",rrc_rx_window_size); 
      break;
  } /* end window size choice */

  return rlc_rx_win_size;

} /* end tdsrrcllc_rx_window_size() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_window_size()

DESCRIPTION : 
  Return the Window Size for transmitting.

DEPENDENCIES: 
  None.

RETURN VALUE:
  Window size for transmitting RLC.

SIDE EFFECTS:
  None.
============================================================*/
static uint16 tdsrrcllc_get_window_size
(
  tdsrrc_TransmissionWindowSize rrc_tx_window_size
)
{
  /* Initialize the window size  */
  uint16 rlc_tx_win_size = (uint16)TDSRRCLLC_INVALID_WINDOW_SIZE;

  switch (rrc_tx_window_size)
  {
    case tdsrrc_TransmissionWindowSize_tw1:
      rlc_tx_win_size = 1;
      break;
    case tdsrrc_TransmissionWindowSize_tw8:
      rlc_tx_win_size = 8;
      break;
    case tdsrrc_TransmissionWindowSize_tw16:
      rlc_tx_win_size = 16;
      break;
    case tdsrrc_TransmissionWindowSize_tw32:
      rlc_tx_win_size = 32;
      break;
    case tdsrrc_TransmissionWindowSize_tw64:
      rlc_tx_win_size = 64;
      break;
    case tdsrrc_TransmissionWindowSize_tw128:
      rlc_tx_win_size = 128;
      break;
    case tdsrrc_TransmissionWindowSize_tw256:
      rlc_tx_win_size = 256;
      break;
    case tdsrrc_TransmissionWindowSize_tw512:
      rlc_tx_win_size = 512;
      break;
    case tdsrrc_TransmissionWindowSize_tw768:
      rlc_tx_win_size = 768;
      break;
    case tdsrrc_TransmissionWindowSize_tw1024:
      rlc_tx_win_size = 1024;
      break;
    case tdsrrc_TransmissionWindowSize_tw1536:
      rlc_tx_win_size = 1536;
      break;
    case tdsrrc_TransmissionWindowSize_tw2047:
      rlc_tx_win_size = 2047;
      break;
    case tdsrrc_TransmissionWindowSize_tw2560:
      rlc_tx_win_size = 2560;
      break;
    case tdsrrc_TransmissionWindowSize_tw3072:
      rlc_tx_win_size = 3072;
      break;
    case tdsrrc_TransmissionWindowSize_tw3584:
      rlc_tx_win_size = 3584;
      break;
    case tdsrrc_TransmissionWindowSize_tw4095:
      rlc_tx_win_size = 4095;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Window Size %d",rrc_tx_window_size); 
      break;
  } /* end window size choice */

  return rlc_tx_win_size;

} /* end tdsrrcllc_get_window_size() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_1st_hole_for_rlc_am

DESCRIPTION : 
  Find the first am_config block that is not used.
  
DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uint8 tdsrrcllc_get_1st_hole_for_rlc_am( void )
{
  uint8 am_entity_idx = TDSUE_MAX_AM_ENTITY;
  int i;

  for(i = 0; i < TDSUE_MAX_AM_ENTITY; i++)
  {
    /* Since we reset the am_config to 0 once it is not used, 
       the ul_nchan and dl_nchan should be 0 if it is a hole.
     */
    if(tdsordered_config_ptr->rlc_am_parms.am_info[i]->ul_nchan == 0 && 
       tdsordered_config_ptr->rlc_am_parms.am_info[i]->dl_nchan == 0)
    {
      am_entity_idx = (uint8)i;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
     tdsordered_config_ptr->rlc_am_parms.am_info[i]->rlc_dl_am_config.am_pdu_size_flexbile = FALSE;
#endif

      break;
    }
  }
  return am_entity_idx;
}

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
)
{
  uint8 ul_um_entity_idx = TDSUE_MAX_UL_UM_CHANNEL;
  int i;

  for(i = 0; i < TDSUE_MAX_UL_UM_CHANNEL; i++)
  {
    if(tdsordered_config_ptr->rlc_ul_um_parms.chan_info[i].rb_id == TDSRRCLLC_INVALID_RB_ID )
    {
      ul_um_entity_idx = (uint8)i;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Got empty RLC UL UM Info slot %d. (8 = TDSUE_MAX_UL_UM_CHANNEL)",i);
      break;
    }
  }
  return ul_um_entity_idx;
}

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
)
{
  uint8 dl_um_entity_idx = TDSUE_MAX_DL_UM_CHANNEL;
  int i;
  for(i = 0; i < TDSUE_MAX_DL_UM_CHANNEL; i++)
  {
    if(config_ptr->rlc_dl_um_parms.chan_info[i].rb_id == TDSRRCLLC_INVALID_RB_ID )
    {
      dl_um_entity_idx = (uint8)i;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Got empty RLC DL UM Info slot %d. (8 = TDSUE_MAX_DL_UM_CHANNEL)",i);
      break;
    }
  }
  return dl_um_entity_idx;
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_am_rlc_info()

DESCRIPTION : 
  Read the information for Uplink AM RLC from the received 
  structure and write them into one of the pre-allocated 
  AM memory block.

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_am_rlc_info
(
  tdsrrc_UL_AM_RLC_Mode *ie_ptr,
  uint8               entity_idx,
  tdsrrc_RB_Identity    rb_id
)
{
  tdsrlc_ul_info_type *rlc_ul_info;

  /* No am_config index being assigned. */
  if(entity_idx == TDSUE_MAX_AM_ENTITY)
  {
    return FAILURE;
  }

  rlc_ul_info = 
    &tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.rlc_info;

  /* Turn on the information included flag  */
  rlc_ul_info->rlc_info_incl = TRUE;

  /* config the SDU/PDU discard scheme parameters */
  if(tdsrrcllc_get_xmit_rlc_discard(rlc_ul_info, &ie_ptr->transmissionRLC_Discard) == FAILURE)
  {
    return FAILURE;
  }



  /* Get the maximum number of RLC PDUs sent without requiring an ack  */
  if((rlc_ul_info->tx_window_size = tdsrrcllc_get_window_size(ie_ptr->transmissionWindowSize))
    == TDSRRCLLC_INVALID_WINDOW_SIZE)
  {
    return FAILURE;
  }

  /* Get the timer value used to detect the loss of reset ack pdu */
  switch(ie_ptr->timerRST)
  {
    case tdsrrc_TimerRST_tr50:
      rlc_ul_info->timer_rst = 50;
      break;
    case tdsrrc_TimerRST_tr100:
      rlc_ul_info->timer_rst = 100;
      break;
    case tdsrrc_TimerRST_tr150:
      rlc_ul_info->timer_rst = 150;
      break;
    case tdsrrc_TimerRST_tr200:
      rlc_ul_info->timer_rst = 200;
      break;
    case tdsrrc_TimerRST_tr250:
      rlc_ul_info->timer_rst = 250;
      break;
    case tdsrrc_TimerRST_tr300:
      rlc_ul_info->timer_rst = 300;
      break;
    case tdsrrc_TimerRST_tr350:
      rlc_ul_info->timer_rst = 350;
      break;
    case tdsrrc_TimerRST_tr400:
      rlc_ul_info->timer_rst = 400;
      break;
    case tdsrrc_TimerRST_tr450:
      rlc_ul_info->timer_rst = 450;
      break;
    case tdsrrc_TimerRST_tr500:
      rlc_ul_info->timer_rst = 500;
      break;
    case tdsrrc_TimerRST_tr550:
      rlc_ul_info->timer_rst = 550;
      break;
    case tdsrrc_TimerRST_tr600:
      rlc_ul_info->timer_rst = 600;
      break;
    case tdsrrc_TimerRST_tr700:
      rlc_ul_info->timer_rst = 700;
      break;
    case tdsrrc_TimerRST_tr800:
      rlc_ul_info->timer_rst = 800;
      break;
    case tdsrrc_TimerRST_tr900:
      rlc_ul_info->timer_rst = 900;
      break;
    case tdsrrc_TimerRST_tr1000:
      rlc_ul_info->timer_rst = 1000;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Timer RST type %d",ie_ptr->timerRST);
      return FAILURE;
  } /* end Timer_RST */

  /* Get the Maximum number of retransmissions of the the RESET_PDU */
  switch (ie_ptr->max_RST)
  {
    case tdsrrc_MaxRST_rst1:
      rlc_ul_info->maxrst = 1;
      break;
    case tdsrrc_MaxRST_rst4:
      rlc_ul_info->maxrst = 4;
      break;
    case tdsrrc_MaxRST_rst6:
      rlc_ul_info->maxrst = 6;
      break;
    case tdsrrc_MaxRST_rst8:
      rlc_ul_info->maxrst = 8;
      break;
    case tdsrrc_MaxRST_rst12:
      rlc_ul_info->maxrst = 12;
      break;
    case tdsrrc_MaxRST_rst16:
      rlc_ul_info->maxrst = 16;
      break;
    case tdsrrc_MaxRST_rst24:
      rlc_ul_info->maxrst = 24;
      break;
    case tdsrrc_MaxRST_rst32:
      rlc_ul_info->maxrst = 32;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown MAX RST type %d",ie_ptr->max_RST);
      return FAILURE;
  } /* end Max_RST */

  /*Per 25.331 CR 1030: UE shall use Max_RST value equal to 1 for SRBs, regardless of what is sent by UTRAN;*/
  if((rb_id >= DCCH_AM_RADIO_BEARER_ID) && 
           (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID))
  {
#ifdef FEATURE_TDSCDMA_SRB_RESET
  if (TDS_NV_OPTIONAL_UECI_SRB_RESET_DISABLE)
  {
    rlc_ul_info->maxrst = 1;
  }
  else
  {
    if(ie_ptr->max_RST > tdsrrc_MaxRST_rst4)
    {
      rlc_ul_info->maxrst = 4;
    }

  }
TDSRRC_MSG3(MSG_LEGACY_HIGH,"FEATURE_TDSCDMA_SRB_RESET:UTRAN signaled MaxRST and SRB_RESET enable %d on SRB%d. Change the value to %d", ie_ptr->max_RST, rb_id, rlc_ul_info->maxrst);
#else
    rlc_ul_info->maxrst = 1;
#endif
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"UTRAN signaled MaxRST %d on SRB%d. Change the value to %d", ie_ptr->max_RST, rb_id, rlc_ul_info->maxrst);
  }
  /* Configure Poll scheme parameters  */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,pollingInfo))
  {
    if(tdsrrcllc_get_polling_info(rlc_ul_info, &ie_ptr->pollingInfo) == FAILURE)
    {
      return FAILURE;
    }
  }
  else
  {
    rlc_ul_info->poll_info.poll_info_incl = FALSE; 
  }

  return SUCCESS;
} /* end tdsrrcllc_get_ul_am_rlc_info() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_um_rlc_info

DESCRIPTION : 
  Read the information for Uplink UM RLC from the received 
  structure and write them into one of the pre-allocated 
  UM memory block.

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_um_rlc_info
(
  tdsrrc_UL_UM_RLC_Mode *ie_ptr,
  uint8               entity_idx
)
{
  /* local copy of TM pointer */
  if(entity_idx == TDSUE_MAX_UL_UM_CHANNEL)
  {
    entity_idx = tdsordered_config_ptr->rlc_ul_um_parms.nchan;
  }

  /*
  * For RLC UL UM only Timer based no explicit discard is allowed. This is not
  * supported by the UE.
  */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_UL_UM_RLC_Mode,transmissionRLC_Discard))
  {
    switch (ie_ptr->transmissionRLC_Discard.t)
    {
      case T_tdsrrc_TransmissionRLC_Discard_timerBasedNoExplicit:
        {
          tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].timer_discard = 
              tdsrrcllc_ul_um_timer_discard(ie_ptr->transmissionRLC_Discard.u.timerBasedNoExplicit);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RLC UL UM: Timer based no explicit %d",tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].timer_discard);
        }
        break;

      case T_tdsrrc_TransmissionRLC_Discard_timerBasedExplicit:
      case T_tdsrrc_TransmissionRLC_Discard_maxDAT_Retransmissions:
      case T_tdsrrc_TransmissionRLC_Discard_noDiscard:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"UL RLC UM %d Retrans not allowed",ie_ptr->transmissionRLC_Discard.t);
        return FAILURE;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown UL RLC TM Discard mode %d",ie_ptr->transmissionRLC_Discard.t);
        return FAILURE;
    } /* end rlc discard choice */
  } /* end timer discard check */
  else
  {
    tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].timer_discard = 0;
  }

  /* everything okay if here */
  return SUCCESS;

} /* end tdsrrcllc_get_ul_um_rlc_info() */


/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_rlc_status_info

DESCRIPTION : 
  Config the AM status information in Downlink.

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_rlc_status_info
(
  tdsrlc_dl_info_type      *rlc_dl_info,
  tdsrrc_DL_RLC_StatusInfo *ie_ptr 
)
{
  /* indicate that have some info for DL status */
  rlc_dl_info->status_info.status_info_incl = TRUE;

 /*-------------------------------------------------------- 
    Mandatory parameters.
  ---------------------------------------------------------*/

  /* True indicates that the UE should send a status report for each detected missing PU */
  rlc_dl_info->status_info.missing_pdu = ie_ptr->missingPDU_Indicator;
  
 /*-------------------------------------------------------- 
    Optional parameters.
  ---------------------------------------------------------*/

  /* Optional parameters that are not included indicate that the corresponding
   * function should be disabled in RLC, so initialize those functions to
   * off here. (25.331 8.6.4.9)
   */

  rlc_dl_info->status_info.status_prohibit = 0;
  rlc_dl_info->status_info.timer_epc = 0;
  rlc_dl_info->status_info.status_periodic = 0;

  /*
  * Get the minimum time in ms between status reports. The values range from 10 to
  * 50, in step sizes of 10. Note that the ASN.1 enum allows for for up to 10,000.
  */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,timerStatusProhibit))
  {
    if(ie_ptr->timerStatusProhibit <= tdsrrc_TimerStatusProhibit_tsp550)
    {
      rlc_dl_info->status_info.status_prohibit = (((uint16)ie_ptr->timerStatusProhibit + 1) * 10);
    }
    /*lint -save -e685 */
    else if((ie_ptr->timerStatusProhibit > tdsrrc_TimerStatusProhibit_tsp550) && 
            (ie_ptr->timerStatusProhibit <= tdsrrc_TimerStatusProhibit_tsp1000))
    {
      rlc_dl_info->status_info.status_prohibit = (550 + 
                                                          (((uint16)ie_ptr->timerStatusProhibit - (uint16)tdsrrc_TimerStatusProhibit_tsp550) * 50));
    }
    /*lint -restore */
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Timer Status Prohibit %d",ie_ptr->timerStatusProhibit);
      return FAILURE;
    }
  }
  
  /* Periodic Timer status, in ms */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,timerStatusPeriodic))
  {
    switch (ie_ptr->timerStatusPeriodic)
    {
      case tdsrrc_TimerStatusPeriodic_tsp100:
        rlc_dl_info->status_info.status_periodic = 100;
        break;
      case tdsrrc_TimerStatusPeriodic_tsp200:
        rlc_dl_info->status_info.status_periodic = 200;
        break;
      case tdsrrc_TimerStatusPeriodic_tsp300:
        rlc_dl_info->status_info.status_periodic = 300;
        break;
      case tdsrrc_TimerStatusPeriodic_tsp400:
        rlc_dl_info->status_info.status_periodic = 400;
        break;
      case tdsrrc_TimerStatusPeriodic_tsp500:
        rlc_dl_info->status_info.status_periodic = 500;
        break;
      case tdsrrc_TimerStatusPeriodic_tsp750:
        rlc_dl_info->status_info.status_periodic = 750;
        break;
      case tdsrrc_TimerStatusPeriodic_tsp1000:
        rlc_dl_info->status_info.status_periodic = 1000;
        break;
      case tdsrrc_TimerStatusPeriodic_tsp2000:
        rlc_dl_info->status_info.status_periodic = 2000;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Timer status Periodic type %d",ie_ptr->timerStatusPeriodic);
        return FAILURE;
    } /* end periodic timer status choice */
  } /* end periodic timer status check */
  
  /* if got here, everything is okay */
  return SUCCESS;

} /* end get_rlc_dl_status_info() */


/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_am_rlc_info_r5()

DESCRIPTION : 
  Read the information for Downlink AM RLC from the 
  received structure and write them into one of the 
  pre-allocated AM memory block.

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_am_rlc_info_r5
(
tdsrrc_DL_AM_RLC_Mode_r5 *ie_ptr,
uint8               entity_idx,
tdsrrc_RB_Identity          rb_id,
boolean       allow_new_channels,
tdsrlc_reestablish_e_type reestablish_entity
)
{
  tdsrlc_dl_info_type *rlc_dl_info;
  
  /* local copy of poitner */
  if (entity_idx == TDSUE_MAX_AM_ENTITY)
  {
    entity_idx = tdsordered_config_ptr->rlc_am_parms.nentity;
  }
  rlc_dl_info = 
    &tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.rlc_info;
  
  /* Turn on the information included flag  */
  rlc_dl_info->rlc_info_incl = TRUE;
  
  /* indicates if in sequence delivery is required */
  rlc_dl_info->in_sequence = ie_ptr->inSequenceDelivery;
  
  /* Get the maximum number of RLC PUs allowed to be rcv'd  */
  if ((rlc_dl_info->rx_window_size = tdsrrcllc_rx_window_size(ie_ptr->receivingWindowSize))
    == TDSRRCLLC_INVALID_WINDOW_SIZE)
  {
    return FAILURE;
  }
  
  /* Get the downlink status info */
  if (tdsrrcllc_get_dl_rlc_status_info(rlc_dl_info, &ie_ptr->dl_RLC_StatusInfo) == FAILURE)
  {
    return FAILURE;
  }
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing DL RLC PDU Size");


  if(tdsrrcllc_process_dl_pdu_size(&ie_ptr->dl_RLC_PDU_size, entity_idx,allow_new_channels,reestablish_entity)
    == SUCCESS)
  {
      /* Set this RLC Size in the global structure tdsrlc_size_per_rb */
    if (tdsrrc_set_dl_am_rlc_pdu_size(rb_id, tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to set DL AM PDU Size Info in rlc_per_rb");
      return FAILURE;
    }
  }
  else
  {
    return FAILURE;
  }
  
  /* everything is a go */
  return SUCCESS;

} /* end tdsrrcllc_get_dl_am_rlc_info_r5() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_am_rlc_info()

DESCRIPTION : 
  Read the information for Downlink AM RLC from the 
  received structure and write them into one of the 
  pre-allocated AM memory block.

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_am_rlc_info
(
  tdsrrc_DL_AM_RLC_Mode *ie_ptr,
  uint8               entity_idx
)
{
  tdsrlc_dl_info_type *rlc_dl_info;

  /* LLC processes the UL AM info first, the entity idx should be available
     when processing the DL AM info.
   */
  if(entity_idx == TDSUE_MAX_AM_ENTITY)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"the entity idx is not available");
    return FAILURE;
  }
  rlc_dl_info = &tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.rlc_info;

  /* Turn on the information included flag  */
  rlc_dl_info->rlc_info_incl = TRUE;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_pdu_size_flexbile = FALSE;  
    if(tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_pdu_size_flexbile == TRUE ) 
    {
      tdsordered_config_ptr->am_config[entity_idx].reestab_entity = TDSRLC_RE_ESTABLISH_DL_UL;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: Re-establish dl am enity %d PDU type changed from  flexible to fixed for rb-id %d ",entity_idx,tdsordered_config_ptr->am_config[entity_idx].common_config.rb_id);
    }
#endif

  /* indicates if in sequence delivery is required */
  rlc_dl_info->in_sequence = ie_ptr->inSequenceDelivery;

  /* Get the maximum number of RLC PUs allowed to be rcv'd  */
  if((rlc_dl_info->rx_window_size = tdsrrcllc_rx_window_size(ie_ptr->receivingWindowSize))
    == TDSRRCLLC_INVALID_WINDOW_SIZE)
  {
    return FAILURE;
  }
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  if((tdsordered_config_ptr->rb_am_rlc_size_ota[entity_idx].rrc_pdu_type
      == TDSRRC_PDU_SIZE_NOT_SET )
     ||(tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_pdu_size_flexbile == TRUE )
      )
#else
  if(tdsordered_config_ptr->rb_am_rlc_size_ota[entity_idx].rrc_pdu_type
      == TDSRRC_PDU_SIZE_NOT_SET )
#endif
  {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if(tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_pdu_size_flexbile)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Change from Flexible PDU to fixed using r3 OTA,so calculate size from Trch config");
    }
#endif
    tdsrrcllc_set_am_dl_rlc_size_ota(tdsordered_config_ptr->am_config[entity_idx].common_config.rb_id,
           TDSRRCLLC_INVALID_RLC_SIZE,TDSRRC_PDU_SIZE_NOT_SET);     
  }

  /* Get the downlink status info */
  if(tdsrrcllc_get_dl_rlc_status_info(rlc_dl_info, &ie_ptr->dl_RLC_StatusInfo) == FAILURE)
  {
    return FAILURE;
  }
  
  /* everything is a go */
  return SUCCESS;

} /* end tdsrrcllc_get_dl_am_rlc_info() */

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
)
{
  uint8 i;
  uint8 result = TDSUE_MAX_AM_ENTITY;
  tdsordered_config_type * config_ptr = tdstransition_config.toc_ptr;
  
  for(i = 0; i < TDSUE_MAX_AM_ENTITY; i++)
  {
    /* This function will be called by tdsrrcllc_get_rlc_info() when fills in RLC UL info and
       RLC DL info. Since RRC fills in UL first, the ul_nchan will be non-zero when
       fills RLC DL info and calls this function. 
       Otherwise, for an empty am_info block, the ul_nchan and dl_nchan should be zero.
     */
    if((config_ptr->rlc_am_parms.am_info[i]->common_config.rb_id == rb_id) &&
       (config_ptr->rlc_am_parms.am_info[i]->ul_nchan ||
          config_ptr->rlc_am_parms.am_info[i]->dl_nchan)
       )
    {
      result = i;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Found RLC AM for RB %d,idx %d",rb_id,i);
      break;
    }
  }

  return result;
}

/*==========================================================
FUNCTION  : tdsrrc_get_am_index()

DESCRIPTION : Returns the index of the am_config info
              associated with the given RB id, or
              TDSUE_MAX_AM_ENTITY if no entry exists.

DEPENDENCIES: None.

RETURN VALUE: See description.

SIDE EFFECTS: None.
============================================================*/
uint8 tdsrrc_get_am_index
(
  tdsrrc_RB_Identity rb_id
)
{
  uint8 i;
  uint8 result = TDSUE_MAX_AM_ENTITY;

  for(i = 0; i < TDSUE_MAX_AM_ENTITY; i++)
  {
    /* This function will be called by tdsrrcllc_get_rlc_info() when fills in RLC UL info and
       RLC DL info. Since RRC fills in UL first, the ul_nchan will be non-zero when
       fills RLC DL info and calls this function. 
       Otherwise, for an empty am_info block, the ul_nchan and dl_nchan should be zero.
     */
    if((tdsordered_config_ptr->rlc_am_parms.am_info[i]->common_config.rb_id == rb_id) &&
       (tdsordered_config_ptr->rlc_am_parms.am_info[i]->ul_nchan ||
          tdsordered_config_ptr->rlc_am_parms.am_info[i]->dl_nchan)
       )
    {
      result = i;
      TDSRRC_MSG2(MSG_LEGACY_MED,"Found RLC AM for RB %d,idx %d",rb_id,i);
      break;
    }
  }

  return result;
} /* end tdsrrc_get_am_index */

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
)
{
  uint8 i;
  uint8 result = TDSUE_MAX_UL_UM_CHANNEL;

  if(rb_id != TDSRRCLLC_INVALID_RB_ID)
  {
    for(i = 0; i < TDSUE_MAX_UL_UM_CHANNEL; i++)
    {
      if(tdsordered_config_ptr->rlc_ul_um_parms.chan_info[i].rb_id== rb_id)
      {
        result = i;
        break;
      }
    }
  }
  return result;
} /* end tdsrrcllc_get_ul_um_index */
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
)
{
  uint8 i;
  uint8 result = TDSUE_MAX_UL_UM_CHANNEL;

  if(rb_id != TDSRRCLLC_INVALID_RB_ID)
  {
    for(i = 0; i < TDSUE_MAX_UL_UM_CHANNEL; i++)
    {
      if(config_ptr->rlc_ul_um_parms.chan_info[i].rb_id == rb_id)
      {
        result = i;
        break;
      }
    }
  }
  return result;
} 
/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_tm_index()

DESCRIPTION : Returns the index of the UL TM channel config
              info associated with the given logical channel id,
              or UE_MAX_UL_TM_CHANNEL if no entry exists.

DEPENDENCIES: None.

RETURN VALUE: See description.

SIDE EFFECTS: None.
============================================================*/
static uint8 tdsrrcllc_get_ul_tm_index
(
  rlc_lc_id_type lc_id
)
{
  uint8 i;
  uint8 result = UE_MAX_UL_TM_CHANNEL;

  if(lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    for(i = 0; i < tdsordered_config_ptr->rlc_ul_tm_parms.nchan; i++)
    {
      if(tdsordered_config_ptr->rlc_ul_tm_parms.chan_info[i].lc_id == lc_id)
      {
        result = i;
        break;
      }
    }
  }

  return result;
} /* end tdsrrcllc_get_ul_tm_index */

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
)
{
  uint8 i;
  uint8 result = TDSUE_MAX_DL_UM_CHANNEL;

  if(rb_id != TDSRRCLLC_INVALID_RB_ID)
  {
    for(i = 0; i < TDSUE_MAX_DL_UM_CHANNEL; i++)
    {
      if(config_ptr->rlc_dl_um_parms.chan_info[i].rb_id == rb_id)
      {
        result = i;
        break;
      }
    }
  }
  return result;
} /* end tdsrrcllc_get_dl_um_index */

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_tm_index()

DESCRIPTION : Returns the index of the DL TM channel config
              info associated with the given logical channel id,
              or UE_MAX_DL_TM_CHANNEL if no entry exists.

DEPENDENCIES: None.

RETURN VALUE: See description.

SIDE EFFECTS: None.
============================================================*/
static uint8 tdsrrcllc_get_dl_tm_index
(
  rlc_lc_id_type lc_id
)
{
  uint8 i;
  uint8 result = UE_MAX_DL_TM_CHANNEL;

  if(lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    for(i = 0; i < tdsordered_config_ptr->rlc_dl_tm_parms.nchan; i++)
    {
      if(tdsordered_config_ptr->rlc_dl_tm_parms.chan_info[i].lc_id == lc_id)
      {
        result = i;
        break;
      }
    }
  }

  return result;
} /* end tdsrrcllc_get_dl_tm_index */

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
)
{
  uint8 entity_idx;
  rlc_lc_id_type lc_id;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Get UL UM RLC Info for RB %d",rb_id);
  
  entity_idx = tdsrrcllc_get_ul_um_index(rb_id);
      
  if(entity_idx >= TDSUE_MAX_UL_UM_CHANNEL)
  {
    if (!allow_new_channels)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not allowed to add new UL UM RLC entity");
      return FAILURE;
    }
    else
    {
      entity_idx = tdsrrcllc_get_1st_hole_for_rlc_ul_um();
  
      if (entity_idx >= TDSUE_MAX_UL_UM_CHANNEL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"unable to get empty RLC UL UM Info slot");
        return FAILURE;
      }

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Allocate RLC UL UM slot %d for RB %d",entity_idx,rb_id);
      
      tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].rb_id = rb_id;
      
      tdsordered_config_ptr->rlc_ul_um_parms.nchan++;
      
      /* Make sure UE has not exceeded the maximum number of UL UM Log CH's */
      if (tdsordered_config_ptr->rlc_ul_um_parms.nchan > TDSUE_MAX_UL_UM_CHANNEL)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too Many UL UM Log Ch %d",tdsordered_config_ptr->rlc_ul_um_parms.nchan);
        return FAILURE;
      }
    }
  }

  tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].lc_type = 
      tdsrrcllc_get_lc_type_by_um_rb_id(rb_id);
    
  lc_id = tdsrrclcm_get_ul_rlc_lc_id(rb_id);
      
  
  if (lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"LC id for RB id %d = %d",rb_id,lc_id);
    tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].lc_id =  lc_id;
  }
  return SUCCESS;
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_dl_um_info ()

DESCRIPTION : 
  Processes RLC Info IE for DL UM RBs.

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
)
{
  uint8 entity_idx;
  rlc_lc_id_type lc_id;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Get DL UM RLC Info for RB %d",rb_id);
  
  entity_idx = tdsrrcllc_get_dl_um_index(rb_id, config_ptr);
      
  if(entity_idx >= TDSUE_MAX_DL_UM_CHANNEL)
  {
    if (!allow_new_channels)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not allowed to add new DL UM RLC entity");
      return FAILURE;
    }
    else
    {
      entity_idx = tdsrrcllc_get_1st_hole_for_rlc_dl_um(config_ptr);
  
      if (entity_idx >= TDSUE_MAX_DL_UM_CHANNEL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"unable to get empty RLC DL UM Info slot");
        return FAILURE;
      }

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Allocate RLC DL UM slot %d for RB %d",entity_idx,rb_id);
      
      config_ptr->rlc_dl_um_parms.chan_info[entity_idx].rb_id = rb_id;
      
      config_ptr->rlc_dl_um_parms.nchan++;
      
      /* Make sure UE has not exceeded the maximum number of DL UM Log CH's */
      if(config_ptr->rlc_dl_um_parms.nchan > TDSUE_MAX_DL_UM_CHANNEL)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too Many DL UM Log Ch %d",config_ptr->rlc_dl_um_parms.nchan);
        return FAILURE;
      }
    }
  }
  
  config_ptr->rlc_dl_um_parms.chan_info[entity_idx].lc_type = 
      tdsrrcllc_get_lc_type_by_um_rb_id(rb_id);

  lc_id = tdsrrclcm_get_dl_rlc_lc_id(rb_id);
      
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"LC id for RB id %d = %d",rb_id,lc_id);
  
  if (lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    config_ptr->rlc_dl_um_parms.chan_info[entity_idx].lc_id =  lc_id;
  }
  return SUCCESS;
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_rlc_info()

DESCRIPTION :

DEPENDENCIES: None.

RETURN VALUE: 

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_rlc_info
(
  tdsrrc_UL_RLC_Mode *rrc_ul_RLC_Mode,
  tdsrrc_RB_Identity    rb_id,
  boolean       allow_new_channels,
  boolean * new_rlc_am_entity
)
{
      uint8 entity_idx;
  rlc_lc_id_type lc_id;

    switch (rrc_ul_RLC_Mode->t)
    {
    case T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode:
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get UL AM RLC Info for RB %d",rb_id);
      
      entity_idx = tdsrrc_get_am_index(rb_id);
      if(entity_idx == TDSUE_MAX_AM_ENTITY)
      {
        if (!allow_new_channels)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not allowed to add new UL AM RLC entity");
          return FAILURE;
        }
        else
        {
          /* Find an unused am_config block.  */
          entity_idx = tdsrrcllc_get_1st_hole_for_rlc_am();

          if (entity_idx == TDSUE_MAX_AM_ENTITY)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"unable to get empty RLC AM Info slot");
            return FAILURE;
          }
          *new_rlc_am_entity = TRUE;
          
          /* It always assume to be the shared channel (Control&Data) 
          as we support now. */  
          
          tdsordered_config_ptr->rlc_am_parms.am_info[entity_idx]->ul_nchan = 1;
        }
      }
      
      if (tdsrrcllc_get_ul_am_rlc_info(rrc_ul_RLC_Mode->u.ul_AM_RLC_Mode, entity_idx, rb_id) == FAILURE)
      {
        return FAILURE;
      }
      
      /* It's no harm to re-fill in the rb_id again, if it does exist.  */
      tdsordered_config_ptr->rlc_am_parms.am_info[entity_idx]->common_config.rb_id = rb_id;
      
      break;
      
    case T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode:

      if(tdsrrcllc_get_rlc_ul_um_info(rb_id, allow_new_channels) == FAILURE)
      {
        return FAILURE;
      }

      entity_idx = tdsrrcllc_get_ul_um_index(rb_id);
    
      if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rrc_ul_RLC_Mode->u.ul_UM_RLC_Mode,tdsrrc_UL_UM_RLC_Mode,transmissionRLC_Discard))
      {
        if (tdsrrcllc_get_ul_um_rlc_info(rrc_ul_RLC_Mode->u.ul_UM_RLC_Mode, entity_idx) == FAILURE)
        {
          return FAILURE;
        }
      }
      break;
      
    case T_tdsrrc_UL_RLC_Mode_ul_TM_RLC_Mode: 
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get UL TM RLC Info for RB %d",rb_id);
      
      lc_id = tdsrrclcm_get_ul_rlc_lc_id(rb_id);
      entity_idx = tdsrrcllc_get_ul_tm_index(lc_id);
      
      
      if (entity_idx == UE_MAX_UL_TM_CHANNEL)
      {
        if (lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
        {
        /* There was no logical channel ID for the given radio bearer,
          so we need to add a new RLC channel. */
          tdsordered_config_ptr->rlc_ul_tm_parms.nchan++;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"nchan: UL TM %d in tdsrrcllc_get_rlc_info() after alloc",tdsordered_config_ptr->rlc_ul_tm_parms.nchan);
        } 
        else
        {
        /* We found a logical channel ID for the given radio bearer, so
          make a note of that in the RLC info. */
          entity_idx = tdsordered_config_ptr->rlc_ul_tm_parms.nchan;
          tdsordered_config_ptr->rlc_ul_tm_parms.chan_info[entity_idx].lc_id =
            lc_id;
        }
        
        /* Make sure UE has not exceeded the maximum number of UL TM Log CH's */
        if (tdsordered_config_ptr->rlc_ul_tm_parms.nchan > UE_MAX_UL_TM_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too Many UL TM Log Ch %d",tdsordered_config_ptr->rlc_ul_tm_parms.nchan);
          return FAILURE;
        }
      }
      break;
      
    case T_tdsrrc_UL_RLC_Mode_spare:
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown UL RLC mode %d",rrc_ul_RLC_Mode->t);
      return FAILURE;
    } /* end UL RLC choice */

	return SUCCESS;
  }

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_rlc_tm_info()

DESCRIPTION :

DEPENDENCIES: None.

RETURN VALUE: 

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_rlc_tm_info
 (
  boolean     rrc_segmentationIndication,
  tdsrrc_RB_Identity    rb_id,
  boolean       allow_new_channels
 )
{
  uint8 entity_idx;
  rlc_lc_id_type lc_id;
  TDSRRC_MSG1(MSG_LEGACY_MED,"Get DL TM RLC Info for RB %d",rb_id);
  
  lc_id = tdsrrclcm_get_dl_rlc_lc_id(rb_id);
  entity_idx = tdsrrcllc_get_dl_tm_index(lc_id);
  
  if(entity_idx > UE_MAX_DL_TM_CHANNEL)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid entity_idx %d",entity_idx);
    return FAILURE;
  }
  /*
  * Indicate if segmentation is performed
  * Segmentation is currently not supported in RLC
  */
  if (entity_idx == UE_MAX_DL_TM_CHANNEL)
  {
    
    
    if (lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      /* There was no logical channel ID for the given radio bearer,
       so we need to add a new RLC channel. */
       tdsordered_config_ptr->rlc_dl_tm_parms.nchan++;
    } 
    else
    {
     /* We found a logical channel ID for the given radio bearer, so
      make a note of that in the RLC info. */
      entity_idx = tdsordered_config_ptr->rlc_dl_tm_parms.nchan;
      tdsordered_config_ptr->rlc_dl_tm_parms.chan_info[entity_idx].lc_id =
        lc_id;
    }
    
    /* Make sure UE has not exceeded the maximum number of DL TM Log CH's */
    if (tdsordered_config_ptr->rlc_dl_tm_parms.nchan > UE_MAX_DL_TM_CHANNEL)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too Many DL TM Log Ch %d",tdsordered_config_ptr->rlc_dl_tm_parms.nchan);
      return FAILURE;
    }
  } 
  else
  {
    tdsordered_config_ptr->rlc_dl_tm_parms.chan_info[entity_idx].seg_ind = rrc_segmentationIndication;
  }

  return SUCCESS;
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_rlc_um_info_r6()

DESCRIPTION :

DEPENDENCIES: None.

RETURN VALUE: 

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_rlc_um_info_r6
 (
    tdsrrc_DL_UM_RLC_Mode_r6 * rrc_dl_UM_RLC_Mode,
  tdsrrc_RB_Identity    rb_id,
  boolean       allow_new_channels
  , boolean alt_e_bit_present
)
{
  uint8 entity_idx;
 
  if(tdsrrcllc_get_rlc_dl_um_info(rb_id, allow_new_channels, tdsordered_config_ptr) == FAILURE)
  {
    return FAILURE;
  }
        
  if (tdsrrc_set_dl_um_li(rb_id, 
              rrc_dl_UM_RLC_Mode->dl_UM_RLC_LI_size) == FAILURE)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCHS:Unable to set LI %d for RB %d",rrc_dl_UM_RLC_Mode->dl_UM_RLC_LI_size,rb_id);
    return FAILURE;
  }

  entity_idx = tdsrrcllc_get_dl_um_index(rb_id, tdsordered_config_ptr);
  if(entity_idx == TDSUE_MAX_DL_UM_CHANNEL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"The corresponding DL UM info not present");
    return FAILURE;
  }
  tdsordered_config_ptr->rlc_dl_um_parms.chan_info[entity_idx].alternate_e_bit = alt_e_bit_present;

  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(rrc_dl_UM_RLC_Mode,dl_Reception_Window_Size))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS+: R6-ASN1 DL UM reception window size not yet supported");
  }

  return SUCCESS;
}


/*==========================================================
FUNCTION  : tdsrrcllc_update_machs_ota()

DESCRIPTION :

DEPENDENCIES: None.

RETURN VALUE: 

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_update_machs_ota
(
tdsrrc_LogicalChannelIdentity logicalChannelIdentity,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode,
  uint32         dflow_id
)
{
  /* Get the logical channel index */
  uint8         logch_idx;
  rlc_lc_id_type   rlc_lc_id;
  
  uint8 mac_dflow_index;
  uint8 cnt;
  
    mac_dflow_index = tdsrrcllc_find_dflow_index((uint8)dflow_id);
    /* If DFLOW Index does not exist, then create a new one */
    if (mac_dflow_index == UE_MAX_MAC_D_FLOW)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DFLOW Index for DFLOW %d,not found",dflow_id);
  
      /* Create a new one */
      mac_dflow_index = tdsrrcllc_allocate_dflow_index();
      if(mac_dflow_index == UE_MAX_MAC_D_FLOW)
      {
        /* Failure to allocate a new DFLOW Index */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:DFLOW Index not available");
        return FAILURE;
      }
      else  /* A valid new DFLOW index has been allocated */
      {
        /* Now Save DFLOW ID in MAC structure */
        tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].mac_dflow_id = (uint8) dflow_id;
        tdsrrcllc_semi_permanent_data.mac_dflow_info[mac_dflow_index].mac_dflow_id = (uint8) dflow_id;
        /* Increment no of DFLOWs */
        tdsordered_config_ptr->mac_dl_parms.num_dflow++;
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:New dflow cnt %d in MAC struct",tdsordered_config_ptr->mac_dl_parms.num_dflow);
        
      } /* A valid new DFLOW index allocated */
    }
    
    /* Get the Logical Channel Index */
    logch_idx = tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].ndlchan;
  
  
    for (cnt = 0; cnt < tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].ndlchan; cnt++) 
    {
      if (rb_id == tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].dlchan_info[cnt].rb_id)
      {  
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB_ID %d already present",rb_id);
        return (SUCCESS);     
      } 
    } 
  
    /* If Logical Channel ID is present, save MAC Logical Channel Identity */

   tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].dlchan_info[logch_idx].mac_id =
        (uint8)logicalChannelIdentity;
   
    /* Save Radio Bearer ID */
    tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].dlchan_info[logch_idx].rb_id = rb_id;
    
    /* Determine the type of Logical Channel mapped to DCH TrCH */
    if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
    {
      tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
    } 
    else
    {
      tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
    }
    
    if (logch_mode == UE_MODE_MAX_NUM)
    {
    /* This is the case when RB mapping info is coming 
    * in "Affected List". At this point, we need to update the Logical Channel mode 
    * and Logical Channel ID in Cell FACH RB Mapping Info - 
    * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
      */
      rlc_lc_id = tdsrrclcm_get_dl_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
      if (rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid DL LC ID for RB %d",rb_id);
        return(FAILURE);
      }
      
      if (tdsrrclcm_get_dl_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get DL Log Chl mode for LC %d",rlc_lc_id);
        return(FAILURE);
      }
      /* Also update the Logical Channel ID here */
      tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].dlchan_info[logch_idx].rlc_id = rlc_lc_id;
    }
    
    /* Save the logical channel mode, if a setup */
    tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].dlchan_info[logch_idx].rlc_mode = logch_mode;
    
    /* Increment number of logical channels */
    tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].ndlchan++;
    
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:DFLOW %d,No of dedicated Ch %d",tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].mac_dflow_id,tdsordered_config_ptr->mac_dflow_info[mac_dflow_index].ndlchan);
    
  
    tdsrrcllc_free_hanging_rb_mapping_info_index(rb_id,TDSMAC_D_RB_MAPPING);

	return SUCCESS;
  }



/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_info ()

DESCRIPTION : 
  Processes RLC Info IE

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rlc_info
(
  tdsrrc_RLC_Info *ie_ptr,
  tdsrrc_RB_Identity    rb_id,
  boolean       allow_new_channels
)
{

  uint8 entity_idx;
  boolean new_rlc_am_entity = FALSE;

  /* Uplink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info,ul_RLC_Mode))
  {
    if(tdsrrcllc_get_ul_rlc_info(&ie_ptr->ul_RLC_Mode,rb_id,allow_new_channels,&new_rlc_am_entity) == FAILURE)
    {
     return FAILURE;
    }

    if(ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode )
    {
      entity_idx = tdsrrcllc_get_ul_um_index(rb_id);

      TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is not present");
      if(entity_idx < TDSUE_MAX_UL_UM_CHANNEL)
      {
        tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].alternate_e_bit = FALSE;
      }
    }

#ifdef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
    if(ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode )
    {
      entity_idx = tdsrrc_get_am_index(rb_id);

      if(entity_idx == TDSUE_MAX_AM_ENTITY)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"The corresponding UL AM info not present");
        return FAILURE;
      }
      
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Special HE field is not present in OTA.");        
      tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.special_value_he_config = FALSE;
    }
#endif  

  } /* end if UL RLC present */

  /* Downlink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info,dl_RLC_Mode))
  {
    switch (ie_ptr->dl_RLC_Mode.t)
    {
      case T_tdsrrc_DL_RLC_Mode_dl_AM_RLC_Mode:
        TDSRRC_MSG1(MSG_LEGACY_MED,"Get DL AM RLC Info for RB %d",rb_id);

        entity_idx = tdsrrc_get_am_index(rb_id);

        /* LLC processes the UL AM info first, it should find the corresponding
           entity index for DL AM info to be filled in.
         */  
        if(entity_idx == TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"The corresponding UL AM info not present");
          return FAILURE;
        }

        if(tdsrrcllc_get_dl_am_rlc_info(ie_ptr->dl_RLC_Mode.u.dl_AM_RLC_Mode, entity_idx) == FAILURE)
        {
          return FAILURE;
        }

        /* This flag is set when processing UL AM info. */
        if(new_rlc_am_entity)
        {
          /* Indicate that the AM entity has been filled up. LLC does not increase this
             number of entities when process UL AM info.
           */
          tdsordered_config_ptr->rlc_am_parms.nentity++;

          /* Make sure UE has not exceeded the maximum number of AM Entities */
          if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too Many AM Entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
            return FAILURE;
          }
          
          /* It always assume to be the shared channel (Control&Data) 
             as we support now.
           */  
          tdsordered_config_ptr->rlc_am_parms.am_info[entity_idx]->dl_nchan = 1;
          
        }

#ifdef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
        tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.special_value_he_config = FALSE;
#endif              

        break;  /* DL AM */
 
      case T_tdsrrc_DL_RLC_Mode_dl_UM_RLC_Mode:
          
        if(tdsrrcllc_get_rlc_dl_um_info(rb_id, allow_new_channels, tdsordered_config_ptr) == FAILURE)
        {
          return FAILURE;
        }

        entity_idx = tdsrrcllc_get_dl_um_index(rb_id, tdsordered_config_ptr);
    
        TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is not present");
        if(entity_idx < TDSUE_MAX_DL_UM_CHANNEL)
        {
          tdsordered_config_ptr->rlc_dl_um_parms.chan_info[entity_idx].alternate_e_bit = FALSE;
        }
 
        break;

      case T_tdsrrc_DL_RLC_Mode_dl_TM_RLC_Mode:
        if(tdsrrcllc_get_dl_rlc_tm_info(ie_ptr->dl_RLC_Mode.u.dl_TM_RLC_Mode->segmentationIndication,rb_id,allow_new_channels)
             == FAILURE)
        {
          return FAILURE;
        }
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown DL RLC mode %d",ie_ptr->dl_RLC_Mode.t);
        return FAILURE;
    } /* end DL RLC choice */
  } /* end if DL RLC present */

  /* Got here, everything is a go */
  return SUCCESS;

} /* end tdsrrcllc_get_rlc_info() */

/*====================================================================
FUNCTION: tdsrrcllc_get_rlc_size_change_status()

DESCRIPTION:
  This function gets the RLC Size Change Status from Ordered Config

DEPENDENCIES:
  None

RETURN VALUE:
  RLC Size

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrcllc_get_rlc_size_change_status(void)
{
  uint8 i;
  for(i = 0; i < TDSUE_MAX_AM_ENTITY; i++)
  {
    if ((tdsordered_config_ptr->rlc_am_parms.am_info[i]->ul_nchan ||
         tdsordered_config_ptr->rlc_am_parms.am_info[i]->dl_nchan)
      && ((tdsordered_config_ptr->rlc_am_parms.am_info[i]->reestab_entity == 
      TDSRLC_RE_ESTABLISH_DL) ||
      (tdsordered_config_ptr->rlc_am_parms.am_info[i]->reestab_entity 
      == TDSRLC_RE_ESTABLISH_DL_UL)||
      (tdsordered_config_ptr->rlc_am_parms.am_info[i]->reestab_entity == 
       TDSRLC_RE_ESTABLISH_UL)))
    {
      return TRUE;
    }
  }
  return FALSE;  
}


/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_info_r5 ()

DESCRIPTION : 
  Processes RLC Info IE

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rlc_info_r5
(
tdsrrc_RLC_Info_r5 *ie_ptr,
tdsrrc_RB_Identity    rb_id,
boolean       allow_new_channels
)
{
  uint8 entity_idx;
  boolean new_rlc_am_entity = FALSE;
  
  tdsrlc_reestablish_e_type reestablish_entity = TDSRLC_RE_ESTABLISH_NONE;
  /* Uplink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r5,ul_RLC_Mode))
  {
    if(tdsrrcllc_get_ul_rlc_info(&ie_ptr->ul_RLC_Mode,rb_id,allow_new_channels,&new_rlc_am_entity) == FAILURE)
    {
        return FAILURE;
    }
    if(ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode )
    {
      entity_idx = tdsrrcllc_get_ul_um_index(rb_id);

      TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is not present");
      if(entity_idx < TDSUE_MAX_UL_UM_CHANNEL)
      {
        tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].alternate_e_bit = FALSE;
      }
    }

#ifdef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
    if(ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode)
    {
      entity_idx = tdsrrc_get_am_index(rb_id);
      if(entity_idx == TDSUE_MAX_AM_ENTITY)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"The corresponding UL AM info not present");
        return FAILURE;
      }

      TDSRRC_MSG0(MSG_LEGACY_LOW,"Special HE field is not present in OTA.");        
      tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.special_value_he_config = FALSE;
    }
#endif  
  } /* end if UL RLC present */
  
  /* Downlink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r5,dl_RLC_Mode))
  {
    switch (ie_ptr->dl_RLC_Mode.t)
    {
      case T_tdsrrc_DL_RLC_Mode_r5_dl_AM_RLC_Mode: /* rrc_dl_AM_RLC_Mode_r5_chosen */
        TDSRRC_MSG1(MSG_LEGACY_LOW,"RRCHS:Get DL AM RLC Info for RB %d",rb_id);
        
        entity_idx = tdsrrc_get_am_index(rb_id);
        
        /* LLC processes the UL AM info first, it should find the corresponding
        entity index for DL AM info to be filled in. */
        if (entity_idx == TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not allowed to add new DL AM RLC entity");
          return FAILURE;
        }
        
        if (ie_ptr->rlc_OneSidedReEst == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish DL only");
          reestablish_entity = TDSRLC_RE_ESTABLISH_DL;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish DL & UL");
          reestablish_entity = TDSRLC_RE_ESTABLISH_DL_UL;
        }
        if (tdsrrcllc_get_dl_am_rlc_info_r5(ie_ptr->dl_RLC_Mode.u.dl_AM_RLC_Mode, 
                                entity_idx, rb_id,
                                allow_new_channels, reestablish_entity
                                ) == FAILURE)
        {
          return FAILURE;
        }
        
        /* This flag is set when processing UL AM info. */
        if(new_rlc_am_entity)
        {
          /* Indicate that the AM entity has been filled up. LLC does not increase this
          number of entities when process UL AM info.
          */
          tdsordered_config_ptr->rlc_am_parms.nentity++;
          
          /* Make sure UE has not exceeded the maximum number of AM Entities */
          if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too Many AM Entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
            return FAILURE;
          }
          
          /* It always assume to be the shared channel (Control&Data) 
          as we support now.
          */  
          tdsordered_config_ptr->rlc_am_parms.am_info[entity_idx]->dl_nchan = 1;
          
        }
      
#ifdef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
        tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.special_value_he_config = FALSE;
#endif        
      
      break;  /* DL AM */
      
    case T_tdsrrc_DL_RLC_Mode_r5_dl_UM_RLC_Mode: /* rrc_dl_UM_RLC_r5_Mode_chosen NULL */

      if(tdsrrcllc_get_rlc_dl_um_info(rb_id, allow_new_channels, tdsordered_config_ptr) == FAILURE)
      {
        return FAILURE;
      }

      if (tdsrrc_set_dl_um_li(rb_id, 
                           ie_ptr->dl_RLC_Mode.u.dl_UM_RLC_Mode->dl_UM_RLC_LI_size) == FAILURE)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRC:Unable to set LI %d for RB %d",ie_ptr->dl_RLC_Mode.u.dl_UM_RLC_Mode->dl_UM_RLC_LI_size,rb_id);
        return FAILURE;
      }

      entity_idx = tdsrrcllc_get_dl_um_index(rb_id, tdsordered_config_ptr);   

      TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is not present");
      if(entity_idx < TDSUE_MAX_DL_UM_CHANNEL)
      {
        tdsordered_config_ptr->rlc_dl_um_parms.chan_info[entity_idx].alternate_e_bit = FALSE;
      }

      break;
      
    case T_tdsrrc_DL_RLC_Mode_r5_dl_TM_RLC_Mode:
      if(tdsrrcllc_get_dl_rlc_tm_info(ie_ptr->dl_RLC_Mode.u.dl_TM_RLC_Mode->segmentationIndication,rb_id,allow_new_channels)
        == FAILURE)
      {
        return FAILURE;
      }
      break;
      
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown DL RLC mode %d",ie_ptr->dl_RLC_Mode.t);
      return FAILURE;
    } /* end DL RLC choice */
  } /* end if DL RLC present */
  
  /* Got here, everything is a go */
  return SUCCESS;

} /* end tdsrrcllc_get_rlc_info_r5() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_mode()

DESCRIPTION : Gets the RLC mode for a given logical channel.

DEPENDENCIES: None.

RETURN VALUE: uecomdef_logch_mode_e_type indicate whether or 
              not it is TM, UM or AM.

SIDE EFFECTS: None.
============================================================*/
static uecomdef_logch_mode_e_type tdsrrcllc_get_rlc_mode
(
  tdsrrc_RLC_Info *ie_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  uecomdef_logch_mode_e_type dl_logch_mode = UE_MODE_MAX_NUM;

  /* Uplink RLC info */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info,ul_RLC_Mode))
  {
    switch (ie_ptr->ul_RLC_Mode.t)
    {
      case T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode:
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
        break;

      case T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode:
        logch_mode = UE_MODE_UNACKNOWLEDGED;
        break;

      case T_tdsrrc_UL_RLC_Mode_ul_TM_RLC_Mode: 
        logch_mode = UE_MODE_TRANSPARENT;
        break;

      case T_tdsrrc_UL_RLC_Mode_spare:
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown UL RLC mode %d",ie_ptr->ul_RLC_Mode.t);
        logch_mode = UE_MODE_MAX_NUM;
        break;
    } /* end UL RLC choice */
  } /* end if UL RLC present */

  /* Downlink RLC info */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info,dl_RLC_Mode))
  {
    switch (ie_ptr->dl_RLC_Mode.t)
    {
      case T_tdsrrc_DL_RLC_Mode_dl_AM_RLC_Mode:
        dl_logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
        break;

      case T_tdsrrc_DL_RLC_Mode_dl_UM_RLC_Mode:
        dl_logch_mode = UE_MODE_UNACKNOWLEDGED;
        break;

      case T_tdsrrc_DL_RLC_Mode_dl_TM_RLC_Mode: 
        dl_logch_mode = UE_MODE_TRANSPARENT;
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown DL RLC mode %d",ie_ptr->ul_RLC_Mode.t);
        dl_logch_mode = UE_MODE_MAX_NUM;
        break;
    } /* end DL RLC choice */

    /* If both DL and UL mode is present, check to make sure they are the same */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info,ul_RLC_Mode))
    {
      if (logch_mode != dl_logch_mode) 
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"UL %d and DL %d lc mode not a match.  Invalid config",logch_mode,dl_logch_mode);
        logch_mode = UE_MODE_MAX_NUM;
      }
    }
    else
    {
      logch_mode = dl_logch_mode;
    }
  } /* end if DL RLC present */

  /* Need to know RLC type in either UL or DL */
  if (!((ie_ptr->m.ul_RLC_ModePresent) ||
                            (ie_ptr->m.dl_RLC_ModePresent)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Neither DL nor UL RLC mode present!");
  }

  return logch_mode;

} /* end tdsrrcllc_get_rlc_mode */

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
/*====================================================================
FUNCTION: tdsrrcllc_rearrange_macehs_lc_index()

DESCRIPTION:
  This function deletes any holes
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/

static void tdsrrcllc_rearrange_macehs_lc_index
(
  tdsordered_config_type   *config_ptr
)
{
  uint8  ix, iy;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:Rearranging queue index");

  if (config_ptr->mac_dl_parms.ndlchan_macehs == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS:MAC EHS LC ids 0");
    return;
  }
 /* Put all the values in tmp structure. No there are no holes in tmp structure */
  for(ix = TDSUE_MAX_DL_LOGICAL_CHANNEL; ix > 0; ix--)
  {
    /* Check if valid data */
    if(config_ptr->dl_macehs_logchan_info[ix-1].queue_id!= TDSINVALID_QUEUE_ID)
    {
      for(iy = 0; iy < (ix - 1); iy++)
      {
        /* Check if valid data */
        if(config_ptr->dl_macehs_logchan_info[iy].queue_id== TDSINVALID_QUEUE_ID)
        {
          config_ptr->mac_ehs_lc_id_in_use[iy] =  
                    config_ptr->mac_ehs_lc_id_in_use[ix-1];
          memscpy(&config_ptr->dl_macehs_logchan_info[iy], sizeof(tdsmac_dl_ehs_logch_config_type), 
                  &config_ptr->dl_macehs_logchan_info[ix-1], sizeof(tdsmac_dl_ehs_logch_config_type));
          config_ptr->dl_macehs_logchan_info[ix-1].queue_id= TDSINVALID_QUEUE_ID;
          config_ptr->dl_macehs_logchan_info[ix-1].rb_id = TDSRRC_INVALID_RB_ID;
          config_ptr->dl_macehs_logchan_info[ix-1].rlc_id = 0;
          config_ptr->dl_macehs_logchan_info[ix-1].mac_id = TDSINVALID_LOGICAL_CHANNEL_ID;
          config_ptr->dl_macehs_logchan_info[ix-1].chan_type = UE_LOGCHAN_NONE;
          config_ptr->dl_macehs_logchan_info[ix-1].rlc_mode = UE_MODE_MAX_NUM;
          config_ptr->mac_ehs_lc_id_in_use[ix-1] = FALSE;  
          break;
        }
      }
      if (iy == (ix-1)) 
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MACEHS: Rearranging MAC EHS Q complete");
        break;
      }
    }
  } /* End of for (...) */

}
  /*==========================================================
FUNCTION  : tdsrrcllc_process_flexible_pdu_dl_am()

DESCRIPTION :

DEPENDENCIES: None.

RETURN VALUE: 

SIDE EFFECTS: None.
============================================================*/
static void tdsrrcllc_process_flexible_pdu_dl_am
(
   tdsrlc_li_e_type  rrc_dl_rlc_pdu,
   uint8               entity_idx,
   boolean       allow_new_channels,
tdsrlc_reestablish_e_type reestablish_entity

)
{

  tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_pdu_size_flexbile = TRUE;
  tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_li_type = rrc_dl_rlc_pdu;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS:LI type %d for RB-ID %d ",rrc_dl_rlc_pdu,tdsordered_config_ptr->am_config[entity_idx].common_config.rb_id);

  tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size = TDSRRCLLC_INVALID_RLC_SIZE;
  
  if(allow_new_channels == FALSE )
  {
    if(tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_pdu_size_flexbile == FALSE ) 
    {
       if(rrc_dl_rlc_pdu == TDSLI_15_BITS )
       {
         tdsordered_config_ptr->am_config[entity_idx].reestab_entity = reestablish_entity;
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: Re-establish dl am enity %d as PDU type changed from fixed to flexible for rb-id %d with LI 15 bits",entity_idx,tdsordered_config_ptr->am_config[entity_idx].common_config.rb_id);
       }
       else
       {
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: MACEHS: DL AM am enity modified %d,PDU type changed from fixed to flexible for rb-id %d with LI 7 bits",entity_idx,tdsordered_config_ptr->am_config[entity_idx].common_config.rb_id);
          tdsordered_config_ptr->am_config[entity_idx].reestab_entity = TDSRLC_RE_ESTABLISH_NONE;
       }
    }
    else if(((tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_li_type == TDSLI_7_BITS)
           && (rrc_dl_rlc_pdu == TDSLI_15_BITS )) )
    {
      tdsordered_config_ptr->am_config[entity_idx].reestab_entity = reestablish_entity;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: Re-establish dl am enity %d as LI size changes from 7 bits to 15 bitsfor rb-id %d ",entity_idx,tdsordered_config_ptr->am_config[entity_idx].common_config.rb_id);
    }
    else if(((tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_li_type == TDSLI_15_BITS)
           && rrc_dl_rlc_pdu == TDSLI_7_BITS ) )
    {
       tdsordered_config_ptr->am_config[entity_idx].reestab_entity = reestablish_entity;
       TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: Re-establish dl am enity %d as LI sizes changes from 15 bits to 7 bitsfor rb-id %d ",entity_idx,tdsordered_config_ptr->am_config[entity_idx].common_config.rb_id);
    }
    else
    {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"MACEHS: L1 size indicated: Old Size %d,New Size %d",tdscurrent_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_li_type,tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.am_li_type);
        tdsordered_config_ptr->am_config[entity_idx].reestab_entity = TDSRLC_RE_ESTABLISH_NONE;
    }
  }
}
#endif /*FEATURE_TDSCDMA_HSDPA_MAC_EHS*/



/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_mode_r5()

DESCRIPTION : Gets the RLC mode for a given logical channel.

DEPENDENCIES: None.

RETURN VALUE: uecomdef_logch_mode_e_type indicate whether or 
              not it is TM, UM or AM.

SIDE EFFECTS: None.
============================================================*/
static uecomdef_logch_mode_e_type tdsrrcllc_get_rlc_mode_r5
(
tdsrrc_RLC_Info_r5 *ie_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  uecomdef_logch_mode_e_type dl_logch_mode = UE_MODE_MAX_NUM;

  /* Uplink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r5,ul_RLC_Mode))
  {
    switch (ie_ptr->ul_RLC_Mode.t)
    {
      case T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode:
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
        break;

      case T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode:
        logch_mode = UE_MODE_UNACKNOWLEDGED;
        break;

      case T_tdsrrc_UL_RLC_Mode_ul_TM_RLC_Mode: 
        logch_mode = UE_MODE_TRANSPARENT;
        break;

      case T_tdsrrc_UL_RLC_Mode_spare:
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown UL RLC mode %d",ie_ptr->ul_RLC_Mode.t);
        logch_mode = UE_MODE_MAX_NUM;
        break;
    }/* end UL RLC choice */
  }/* end if UL RLC present */

  /* Downlink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r5,dl_RLC_Mode))
  {
    switch (ie_ptr->dl_RLC_Mode.t)
    {
      case T_tdsrrc_DL_RLC_Mode_r5_dl_AM_RLC_Mode:
        dl_logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
        break;

      case T_tdsrrc_DL_RLC_Mode_r5_dl_UM_RLC_Mode:
        dl_logch_mode = UE_MODE_UNACKNOWLEDGED;
        break;

      case T_tdsrrc_DL_RLC_Mode_r5_dl_TM_RLC_Mode: 
        dl_logch_mode = UE_MODE_TRANSPARENT;
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown DL RLC mode %d",ie_ptr->ul_RLC_Mode.t);
        dl_logch_mode = UE_MODE_MAX_NUM;
        break;
    } /* end DL RLC choice */

    /* If both DL and UL mode is present, check to make sure they are the same */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r5,ul_RLC_Mode))
    {
      if (logch_mode != dl_logch_mode) 
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"UL %d and DL %d lc mode not a match.  Invalid config",logch_mode,dl_logch_mode);
        logch_mode = UE_MODE_MAX_NUM;
      }
    }
    else
    {
      logch_mode = dl_logch_mode;
    }
  }/* end if DL RLC present */

  /* Need to know RLC type in either UL or DL */
  if (!((ie_ptr->m.ul_RLC_ModePresent) ||
                            (ie_ptr->m.dl_RLC_ModePresent)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Neither DL nor UL RLC mode present!");
  }

  return logch_mode;

} /* end tdsrrcllc_get_rlc_mode_r5 */



/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_dch_mapping_info_r5()

DESCRIPTION : Processes DL DCH Mapping IE contents and saves 
              them in MAC Ordered Configuration Data  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_dch_mapping_info_r5
(
tdsrrc_DL_LogicalChannelMapping_r5 *ie_ptr,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode
)
{

  
  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id;
  /* Get the DL DCH TrCH ID if it exists */
  switch (ie_ptr->dl_TransportChannelType.t)
  {
  case T_tdsrrc_DL_TransportChannelType_r5_dch:
    tdstrch_id = (tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch;
    break;

  case T_tdsrrc_DL_TransportChannelType_r5_dch_and_hsdsch:
    tdstrch_id = (tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->dch_transport_ch_id;
    break;
    
  case T_tdsrrc_DL_TransportChannelType_fach:
  case T_tdsrrc_DL_TransportChannelType_dsch:
  default:
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Expecting DL DCH TrCH ID");
    return FAILURE;
  } /* end DL DCH TrCH ID switch */
  

  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_LogicalChannelMapping,logicalChannelIdentity))
  {
    return tdsrrcllc_update_dl_dch_ota(rb_id,logch_mode,tdstrch_id, ie_ptr->logicalChannelIdentity);
  }
  else
  {
    return tdsrrcllc_update_dl_dch_ota(rb_id,logch_mode,tdstrch_id, TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT);
  }
} /* end tdsrrcllc_get_dl_dch_mapping_info_r5() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_hsdsch_mapping_info_r5()

DESCRIPTION : Processes HSDSCH Mapping IE contents and saves 
              them in MAC Ordered Configuration Data  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_hsdsch_mapping_info_r5
(
tdsrrc_DL_LogicalChannelMapping_r5 *ie_ptr,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode
)
{
  /* DFLOW   Id */
  uint32         dflow_id;
  switch (ie_ptr->dl_TransportChannelType.t)
  {
  case T_tdsrrc_DL_TransportChannelType_r5_dch_and_hsdsch:
    TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:DFLOW Id is %d",ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->hsdsch_mac_d_flow_id); 

    dflow_id = ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->hsdsch_mac_d_flow_id;
    break;

  case T_tdsrrc_DL_TransportChannelType_r5_hsdsch:
    TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:DFLOW Id is %d",ie_ptr->dl_TransportChannelType.u.hsdsch); 

    dflow_id = ie_ptr->dl_TransportChannelType.u.hsdsch;
    break;

  default:
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Invalid  Channel Type %d",ie_ptr->dl_TransportChannelType.t);
    return FAILURE;
  }

 /* If Logical Channel ID is present, save MAC Logical Channel Identity */
   if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_LogicalChannelMapping_r5,logicalChannelIdentity))
   {
    return tdsrrcllc_update_machs_ota(ie_ptr->logicalChannelIdentity,rb_id,logch_mode,dflow_id);
   }
   else 
   {
    return  tdsrrcllc_update_machs_ota(TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT,rb_id,logch_mode,dflow_id);
   }
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_fach_mapping_info_r5()

DESCRIPTION : Processes FACH mapping IE contents and saves 
              it in MAC Ordered Configuration Database.
              If the next state is CELL_DCH, the data is 
              stored in local data structure in OC.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_fach_mapping_info_r5
(
tdsrrc_DL_LogicalChannelMapping_r5 *ie_ptr,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode,
tdsrrc_state_e_type             next_rrc_state
)
{
  /* transport Channel Index */
  uint8   trch_idx  = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Get the logical channel index */
  uint8   logch_idx, i;

  /* Temporary storage for TrCH ID */
  tr_ch_id_type  tmp_trch_id;

  tdsmac_dl_ded_logch_config_type  *dlchan_info;
  rlc_lc_id_type    rlc_lc_id = (uint8)TDSRRCLCM_RLC_LC_ID_NOT_FOUND;

  /* TBD: Although all existing FACHs would be mapped to each of the RBs,
   * fill up only for the 1st FACH - till MAC interface is clarified/updated
   */

  boolean update_fach_rlc_id = FALSE;

  uint8 cnt;

  /* Start with updating Cell FACH RB Mapping Info when the next state is
   * either CELL_DCH or CELL_FACH
   */
  logch_idx = tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan;
  dlchan_info = &tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[logch_idx];

  if (logch_mode == UE_MODE_MAX_NUM)
  {
    /* This is the case when RB mapping info applicable to CELL_FACH is coming 
    * in "Affected List". At this point, we need to update the Logical Channel mode 
    * and Logical Channel ID in Cell FACH RB Mapping Info - 
    * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
    */
    rlc_lc_id = tdsrrclcm_get_dl_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if (rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid DL LC ID for RB %d",rb_id);
      return(FAILURE);
    }

    if (tdsrrclcm_get_dl_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get DL Log Chl mode for LC %d",rlc_lc_id);
      return(FAILURE);
    }
    /* Also update the Logical Channel ID here */
    dlchan_info->rlc_id = rlc_lc_id;
    update_fach_rlc_id = TRUE;
  }

  for (cnt = 0; cnt < tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan ; cnt++) 
  {      
    if (rb_id == tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[cnt].rb_id ) 
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB_ID %d already present",rb_id);
      return (SUCCESS);        
    }
  }

  tdsrrcllc_update_dl_ded_log_chl_info_r5(ie_ptr, dlchan_info, rb_id, logch_mode);
  tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan ++;
  

  /* Now update the FACH Info ONLY IF the next state is CELL_FACH */
  if (next_rrc_state == TDSRRC_STATE_CELL_FACH ||
      next_rrc_state == TDSRRC_STATE_CELL_PCH ||
      next_rrc_state == TDSRRC_STATE_URA_PCH)
  {
    if ((trch_idx = tdsrrcllc_find_dl_fach_trch_idx()) >= TDSUE_MAX_TRCH)
    {
      return FAILURE;
    }
    logch_idx = tdsordered_config_ptr->fach_info[trch_idx].ndlchan;
    dlchan_info = &tdsordered_config_ptr->fach_info[trch_idx].dlchan_info[logch_idx];
    if (update_fach_rlc_id)
    {
      dlchan_info->rlc_id = rlc_lc_id;
    }
    tdsrrcllc_update_dl_ded_log_chl_info_r5(ie_ptr, dlchan_info, rb_id, logch_mode);
    
    tdsordered_config_ptr->fach_info[trch_idx].ndlchan++;
    tdsordered_config_ptr->dl_fach_trch_info[trch_idx].lg_ch_cnt = 
      tdsordered_config_ptr->fach_info[trch_idx].ndlchan;

    /* Since each available FACH is mapped to all RBs, copy the FACH info
    *  from the 1st FACH to all FACHs. Copy everything except for the
    * Transport Channel ID.
    */
    for (i = 1; i < tdsordered_config_ptr->mac_dl_parms.num_fachs; i ++)
    {
      tmp_trch_id = tdsordered_config_ptr->mac_dl_parms.fach_info[i]->trch_id;
      
      *tdsordered_config_ptr->mac_dl_parms.fach_info[i] = 
      *tdsordered_config_ptr->mac_dl_parms.fach_info[0];

      tdsordered_config_ptr->mac_dl_parms.fach_info[i]->trch_id = tmp_trch_id;

      tdsordered_config_ptr->dl_fach_trch_info[i].lg_ch_cnt =
      tdsordered_config_ptr->dl_fach_trch_info[0].lg_ch_cnt;
    }
  } /* If next state is CELL_FACH, update the FACH Info */

  return SUCCESS;

} /* end tdsrrcllc_get_fach_mapping_info_r5() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_logical_channel_info_r5()

DESCRIPTION : Get DL logical Channel info.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_logical_channel_info_r5
(
tdsrrc_DL_LogicalChannelMappingList_r5 * ie_ptr,
tdsrrc_RB_Identity                       rb_id,
uecomdef_logch_mode_e_type       logch_mode
)
{
  /*
  * Count to make sure that don't exceed number of Logical
  * channels that can be Mapped to a Transport Channel.
  */
  uint8 lc_tc_cnt = 0;
  
  tdsordered_config_type *tmp_config_ptr = NULL;

  
  /* Process each item of the list */
  while ((ie_ptr->n > lc_tc_cnt) && (lc_tc_cnt < MAX_DED_LOGCHAN_PER_TRCH))
  {
    /* Transport channel type */
    switch (ie_ptr->elem[lc_tc_cnt].dl_TransportChannelType.t)
    {

    case T_tdsrrc_DL_TransportChannelType_r5_hsdsch:
      /* Check if logical channel is not TM */
      if (logch_mode == UE_MODE_TRANSPARENT)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:TM Log Channel mapped on HSDSCH");
        return FAILURE;
      }
      if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID )
      {
        /* Insane network. Mapping SRBs on HSDSCH */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:RB cannot be mapped on HSDSCH");
        return FAILURE;
      }
      if (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
      {
        if (tdsrrcllc_get_hsdsch_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          return FAILURE;
        }
      } 
      else
      {
        if (tdstransition_config.toc_usage == TDSTOC_INVALID)
        {
          tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
        }
        
        tmp_config_ptr = tdsordered_config_ptr;
        tdsordered_config_ptr = tdstransition_config.toc_ptr;
        
        /* Processing functions must think we are going to Cell_DCH state. */
        tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
        
        if (tdsrrcllc_get_hsdsch_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
          return FAILURE;
        }
        
        tdsordered_config_ptr = tmp_config_ptr;
        tmp_config_ptr = NULL;
      }
      break;
    case T_tdsrrc_DL_TransportChannelType_r5_dch:
      if (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
      {
        if (tdsrrcllc_get_dl_dch_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          return FAILURE;
        }
      } 
      else
      {
        if (tdstransition_config.toc_usage == TDSTOC_INVALID)
        {
          tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
        }
        
        tmp_config_ptr = tdsordered_config_ptr;
        tdsordered_config_ptr = tdstransition_config.toc_ptr;
        
        /* Processing functions must think we are going to Cell_DCH state. */
        tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
        
        if (tdsrrcllc_get_dl_dch_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
          return FAILURE;
        }
        
        tdsordered_config_ptr = tmp_config_ptr;
        tmp_config_ptr = NULL;
      }
      break;
      
    case T_tdsrrc_DL_TransportChannelType_r5_fach:
      if (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH ||
        tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH ||
        tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
      {
        if (tdsrrcllc_get_fach_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode,
          tdsordered_config_ptr->rrc_state_indicator) == FAILURE)
        {
          return FAILURE;
        }
      } 
      else
      {
      /* FACH mapping info can come in DCH state. Accept and update
      * local mapping info structure.
        */
        if (tdsrrcllc_get_fach_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode, TDSRRC_STATE_CELL_DCH) == FAILURE)
        {
          return FAILURE;
        }
      }
      break;
      
    case T_tdsrrc_DL_TransportChannelType_r5_dsch:
    case T_tdsrrc_DL_TransportChannelType_r5_dch_and_dsch:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSCH TrCH not supported");
      return FAILURE;
      
    case T_tdsrrc_DL_TransportChannelType_r5_dch_and_hsdsch:

      if (rb_id < DCCH_DT_LOW_PRI_RADIO_BEARER_ID )
      {
        /* Insane network. Mapping SRBs on HSDSCH */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:RB cannot be mapped on HSDSCH");
        return FAILURE;
      }

      if (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
      {
        if (tdsrrcllc_get_dl_dch_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          return FAILURE;
        }
        if (tdsrrcllc_get_hsdsch_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          return FAILURE;
        }
      } 
      else
      {
        if (tdstransition_config.toc_usage == TDSTOC_INVALID)
        {
          tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
        }
        
        tmp_config_ptr = tdsordered_config_ptr;
        tdsordered_config_ptr = tdstransition_config.toc_ptr;
        
        /* Processing functions must think we are going to Cell_DCH state. */
        tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
        
        if (tdsrrcllc_get_dl_dch_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
          return FAILURE;
        }
        
        if (tdsrrcllc_get_hsdsch_mapping_info_r5(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
          return FAILURE;
        }
        
        tdsordered_config_ptr = tmp_config_ptr;
        tmp_config_ptr = NULL;
      }
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rcv'd invalid DL TrCh type");
      return FAILURE;
    } /* end Logical channel type switch */
    
    /* Get pointer to the next item in the list */
    lc_tc_cnt++;
  } /* end while ie_ptr != NULL */
  
  return SUCCESS;

} /* end tdsrrcllc_get_dl_logical_channel_info_r5() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_mapping_info_r5()

DESCRIPTION : Get Radio Bearer Mapping Info IE

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rb_mapping_info_r5
(
tdsrrc_RB_MappingInfo_r5      * ie_ptr,
tdsrrc_RB_Identity                 rb_id,
uecomdef_logch_mode_e_type logch_mode
)
{
  /*
  * Count to make sure that don't exceed the maximum number 
  * of RB Mux options
  */
  uint8 rb_mux_cnt = 0;

  /* Process the RB Mux option IE in the linked list */
  while ((ie_ptr->n > rb_mux_cnt) && (rb_mux_cnt < MAX_RB_MUX_OPTIONS_REL5))
  {
    /* Check for DL Channel Mapping List */
    if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[rb_mux_cnt],tdsrrc_RB_MappingOption_r5,dl_LogicalChannelMappingList))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for DL",rb_id);
      if (tdsrrcllc_get_dl_logical_channel_info_r5(&ie_ptr->elem[rb_mux_cnt].dl_LogicalChannelMappingList,
                                         rb_id, logch_mode) == FAILURE)
      {
        return FAILURE;
      }
      /* Here indicate that MAC Downlink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
    } /* end dl Log Ch mapping list */

    if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[rb_mux_cnt],tdsrrc_RB_MappingOption_r5,ul_LogicalChannelMappings))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for UL",rb_id);
      if (tdsrrcllc_get_ul_logical_channel_info(&ie_ptr->elem[rb_mux_cnt].ul_LogicalChannelMappings,
                                      rb_id, logch_mode) == FAILURE)
      {
        return FAILURE;
      }

      /* Here indicate that MAC Uplink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
    } /* end ul Log Ch mapping list */

    /* Pointer to the next item in the IE list */
  
    rb_mux_cnt++;

  } /* end loop of RB Mapping info list */

  return SUCCESS;

} /* end tdsrrcllc_get_rb_mapping_info_r5() */

#ifdef FEATURE_UMTS_PDCP
/*==========================================================
FUNCTION  : tdsrrcllc_init_rfc_3095_profiles()

DESCRIPTION : initializes the rohc profiles to uncompressed as 
              uncompressed should always be supported by the UE

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
static void tdsrrcllc_init_rfc_3095_profiles(rohc_profiles_e_type *profile_list)
{
  uint32 index = 0;

  for (index = 0; index < TDSRFC_3095_MAX_ROHC_PROFILES; index++) 
  {
    profile_list[index] = ROHC_PROFILE_UNCOMPRESSED;
  }
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_pdcp_info()

DESCRIPTION : Get PDCP info IE

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_pdcp_info
(
tdsrrc_PDCP_Info           *ie_ptr,
tdsrrc_RB_Identity                 rb_id,
tdsrrcllcoc_pdcp_action_etype pdcp_action
)
{
  uint32 idx = 0;      
  uint32 index = 0;

  tdsrrc_HeaderCompressionInfoList *hdrCmprssnInfoList = {0};  

  /*Get the next free index in pdcp_config_req */
  if (pdcp_action == TDSCPDCP_SETUP) 
  {
    while (idx < MAX_RAB_TO_SETUP) 
    {
      if (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use == FALSE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"PDCP_RRC config req index is %d",idx);
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
        /* Get the RB_ID information */
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].rb_id = rb_id;
        tdsordered_config_ptr->pdcp_parms.num_rbs++;
        break;
      }
      else
      {
        idx++;
      }
    }
  }
  else if (pdcp_action == TDSCPDCP_RECONFIG) 
  {
    while (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].rb_id != rb_id) 
    {
      idx++;
    }
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"PDCP_RRC config req index is %d",idx);

  if (idx == MAX_RAB_TO_SETUP) 
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP_RRC This shouldn't happen");
    return FAILURE;
  }

  /* save the PDCP action */
  if (pdcp_action == TDSCPDCP_SETUP) 
  {
    tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.action = SETUP_PDCP;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"PDCP_RRC setup action for rb_id %d",rb_id);
  }
  else if (pdcp_action == TDSCPDCP_RECONFIG) 
  {
    /* initialize pdcp params as PDCP_INFO provides a snapshot */

    tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_info_changed = TRUE;
    memset(&(tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info), 0, sizeof(pdcp_info_type)); 

    tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.action = RECONFIG_PDCP;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"PDCP_RRC reconfig action for rb_id %d",rb_id);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP_RRC Shouldn't come here");
    return FAILURE;
  }

  tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.is_mcast_call = FALSE;
  
  /* IE support for SNRS relocation is manditory present when logch_mode is AM, 
   * in sequence delivery and no discard.
   */
  index = tdsrrc_get_am_index(rb_id);
  if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_PDCP_Info,losslessSRNS_RelocSupport)) )
  {
    if (index!= TDSUE_MAX_AM_ENTITY && 
        ((tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_ul_am_config.rlc_info.discard_info.discard_mode == TDSRLC_NO_DISCARD) 
         &&
         (tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_dl_am_config.rlc_info.in_sequence == TRUE)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP_RRC SRNS reloc IE needs to be present");
      return FAILURE;
    }
  }
 
  tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_info_valid = TRUE;

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_PDCP_Info,losslessSRNS_RelocSupport)) 
  {
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)) 
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP IE SRNS reloc supported");
      tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.support_for_lossless = TRUE;
      switch(ie_ptr->losslessSRNS_RelocSupport.u.supported)
      {
        case tdsrrc_MaxPDCP_SN_WindowSize_sn255:
          tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_sn_win_size = SN_255;
          break;

        case tdsrrc_MaxPDCP_SN_WindowSize_sn65535:
          tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_sn_win_size = SN_65535;
          break;
      }
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"PDCP SN win size %d", tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].
               pdcp_cfg_req.pdcp_info.pdcp_sn_win_size,0,0);
    }
    else
    {

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP IE SRNS reloc not supported");
      tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.support_for_lossless = FALSE;
    }
  }

  /* Populate wheter PDCP PDU header is existent or not */
  if(ie_ptr->pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present)
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"PDCP pdu header present");
    tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_pdu_hdr = PRESENT;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"PDCP pdu header absent");
    tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_pdu_hdr = ABSENT;
  }

  /* IF IPHC is configured on AM RLC , make sure in-sequence delivery and no-discard is configured.  
   * No need to have same check for UM as UM is always in sequence.
   */
   //?? why loop through??
  if (index!= TDSUE_MAX_AM_ENTITY && 
      (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_PDCP_Info,headerCompressionInfoList)))
  {
    uint32 ix =0;
      hdrCmprssnInfoList = &ie_ptr->headerCompressionInfoList;
      while (hdrCmprssnInfoList->n > ix) 
      {
        if ((tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_ul_am_config.rlc_info.discard_info.discard_mode != TDSRLC_NO_DISCARD) 
            ||
            (tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_dl_am_config.rlc_info.in_sequence == FALSE)) 
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"PDCP_RRC Bad IPHC config discard mode: %d in sequence %d ",tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_ul_am_config.rlc_info.discard_info.discard_mode,tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_dl_am_config.rlc_info.in_sequence);
          return FAILURE;
        }
        ix++;
    }
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_PDCP_Info,headerCompressionInfoList)) 
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"PDCP header compression info list present");
    hdrCmprssnInfoList = &ie_ptr->headerCompressionInfoList;

    index = 0;
    while (hdrCmprssnInfoList->n > index && (index < MAX_COMP_ENG_TYPES))
    {
      /* Process RFC 2507 related info*/
      if (hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.t == 
          T_tdsrrc_AlgorithmSpecificInfo_rfc2507_Info)
      {
        TDSRRC_MSG1(MSG_LEGACY_LOW,"RFC 2507 info present ind %d",index);

        /* Indicates that RFC 2507 info is present */
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
          iphc_info.comp_eng_type = PDCP_IPHC;

        /* Largest number of compressed non-TCP headers that may be sent without sending a full header. */
                 tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_f_max_period = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->f_MAX_PERIOD;
         /* default TDSRFC_2507_F_MAX_PERIOD;*/
        
        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_F_MAX_PERIOD %d",
                 tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_f_max_period, 0,0);

        /* Compressed headers may not be sent more than F_MAX_TIME seconds after sending last full header. */

          tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_f_max_time = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->f_MAX_TIME;
        
        /* default TDSRFC_2507_F_MAX_TIME;*/
        
        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_F_MAX_TIME %d",
                 tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_f_max_time,0,0);

        /* The largest header size in octets that may be compressed */
      
          tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_max_header = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->max_HEADER;
         /* default TDSRFC_2507_MAX_HEADER;*/
        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_F_MAX_TIME %d",
                 tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_max_header,0,0);

        /* Maximum CID value for TCP connections.*/
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_tcp_space = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->tcp_SPACE;
        
        /* default TDSRFC_2507_TCP_SPACE;*/
        
        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_TCP_SPACE %d",
                 tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_tcp_space,0,0);

       /* Maximum CID value for non-TCP connections. */
        
          tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_non_tcp_space = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->non_TCP_SPACE;
        /* default  TDSRFC_2507_NON_TCP_SPACE;*/
        
        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_NON_TCP_SPACE %d",
                 tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_non_tcp_space,0,0);

        /* Whether the algorithm shall reorder PDCP SDUs or not.*/
        if (hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->expectReordering == tdsrrc_ExpectReordering_reorderingNotExpected) 
        {
          tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.expect_reorder = REORDERING_NOT_EXPECTED;
        }
        else if (hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->expectReordering == tdsrrc_ExpectReordering_reorderingExpected) 
        {
          tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.expect_reorder = REORDERING_EXPECTED;
        }
        TDSRRC_MSG3(MSG_LEGACY_LOW,"RFC_2507_EXPECTED_REORDERING %d",
                 tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.expect_reorder,0,0);

      } /*if (hdrCmprssnInfoList->value.algorithmSpecificInfo.choice & rrc_AlgorithmSpecificInfo_r4_rfc2507_Info_chosen)*/
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Should not come here");
        return FAILURE;
      }
      index++;
      
    } /* while (hdrCmprssnInfoList) */

    /* Call PDCP function to futher validate PDCP info */
    if (pdcp_validate_params(tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info,FALSE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP validation is OK");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"pdcp validation failed");
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"PDCP header compression info list not present");
  }
  return SUCCESS;

} /* end tdsrrcllc_get_pdcp_info() */


/*==========================================================
FUNCTION  : get_pdcp_info_r5()

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
)
{
  uint32 idx = 0;      
  uint32 index = 0;
  uint32 count = 0;

  tdsrrc_HeaderCompressionInfoList_r4 *hdrCmprssnInfoList = {0};  
  tdsrrc_ROHC_ProfileList_r4 *rohcProflList;

/*If RBE is setting up CS over HSPA RAB, then make sure to do the validation of following checks.*/
/*From 25.331 v.8.4, clause 8.6.4.10, the following IEs shouldn't be present for CS over HSPA RAB.*/
  
  /*Get the next free index in pdcp_config_req */
  if (pdcp_action == TDSCPDCP_SETUP) 
  {
    while (idx < MAX_RAB_TO_SETUP) 
    {
      if (pdcp_parms_ptr->pdcp_config_req[idx].in_use == FALSE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_MED,"PDCP_RRC config req index is %d",idx);
        pdcp_parms_ptr->pdcp_config_req[idx].in_use = TRUE;
        /* Get the RB_ID information */
        pdcp_parms_ptr->pdcp_config_req[idx].rb_id = rb_id;
        pdcp_parms_ptr->num_rbs++;
        break;
      }
      else
      {
        idx++;
      }
    }
  }
  else if (pdcp_action == TDSCPDCP_RECONFIG) 
  {
    while ((pdcp_parms_ptr->pdcp_config_req[idx].rb_id != rb_id) && 
           (idx < pdcp_parms_ptr->num_rbs)) 
    {
      idx++;
    }
    
    if (idx == pdcp_parms_ptr->num_rbs) 
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"PDCP info to reconfig not found %d: fail",rb_id);
      return FAILURE;
    }
  }

  TDSRRC_MSG1(MSG_LEGACY_MED,"PDCP_RRC config req index is %d",idx);

  if (idx == MAX_RAB_TO_SETUP) 
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP_RRC This shouldn't happen");
    return FAILURE;
  }

  /* save the PDCP action */
  if (pdcp_action == TDSCPDCP_SETUP) 
  {
    pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.action = SETUP_PDCP;
    TDSRRC_MSG1(MSG_LEGACY_MED,"PDCP_RRC setup action for rb_id %d",rb_id);
  }
  else if (pdcp_action == TDSCPDCP_RECONFIG) 
  {
    /* initialize pdcp params as PDCP_INFO provides a snapshot */

    pdcp_parms_ptr->pdcp_config_req[idx].pdcp_info_changed = TRUE;
    memset(&(tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info), 0, sizeof(pdcp_info_type)); 

    pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.action = RECONFIG_PDCP;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"PDCP_RRC reconfig action for rb_id %d",rb_id);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP_RRC Shouldn't come here");
  }

  pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.is_mcast_call = FALSE;

  

  {
    /* If IE support for SNRS relocation is set, make sure that logch_mode is AM, 
     * in sequence delivery and no discard.
     */
    index = tdsrrc_get_am_index(rb_id);
    if (!(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)))
    {
      if (index!= TDSUE_MAX_AM_ENTITY && 
          ((tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_ul_am_config.rlc_info.discard_info.discard_mode == TDSRLC_NO_DISCARD) 
           &&
           (tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_dl_am_config.rlc_info.in_sequence == TRUE)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP_RRC SRNS reloc IE needs to be present");
        return FAILURE;
      }
    }
    /* IF ROCH is configured on AM RLC , make sure in-sequence delivery and no-discard is configured.  
     * No need to have same check for UM as UM is always in sequence.
     */
    if (index!= TDSUE_MAX_AM_ENTITY && 
        (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)))
    {
       
      {
        uint32 ix =0;
        hdrCmprssnInfoList = &ie_ptr->headerCompressionInfoList;
        while (hdrCmprssnInfoList->n > ix) 
        {
          if ((tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_ul_am_config.rlc_info.discard_info.discard_mode != TDSRLC_NO_DISCARD) 
              ||
              (tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_dl_am_config.rlc_info.in_sequence == FALSE)) 
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"PDCP_RRC Bad ROHC IPHC config discard mode: %d in sequence %d ",tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_ul_am_config.rlc_info.discard_info.discard_mode,tdsordered_config_ptr->rlc_am_parms.am_info[index]->rlc_dl_am_config.rlc_info.in_sequence);
            return FAILURE;
          }
          ix++;
        }
      }
    }
  }
 
  pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_info_valid = TRUE;

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) 
  {
    if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)) 
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP IE SRNS reloc supported");
      pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.support_for_lossless = TRUE;
      switch(ie_ptr->losslessSRNS_RelocSupport.u.supported)
      {
        case tdsrrc_MaxPDCP_SN_WindowSize_sn255:
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_sn_win_size = SN_255;
          break;

        case tdsrrc_MaxPDCP_SN_WindowSize_sn65535:
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_sn_win_size = SN_65535;
          break;
        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unexpected SN win size value");
          break;
      }
      TDSRRC_MSG3(MSG_LEGACY_LOW,"PDCP SN win size %d", pdcp_parms_ptr->pdcp_config_req[idx].
               pdcp_cfg_req.pdcp_info.pdcp_sn_win_size,0,0);
    }
    else
    {

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP IE SRNS reloc not supported");
      pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.support_for_lossless = FALSE;
    }
  }
  /* Populate wheter PDCP PDU header is existent or not */
  if(ie_ptr->pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present)
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"PDCP pdu header present");
    pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_pdu_hdr = PRESENT;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"PDCP pdu header absent");
    pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.pdcp_pdu_hdr = ABSENT;
  }

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) 
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"PDCP header compression info list present");
    hdrCmprssnInfoList = &ie_ptr->headerCompressionInfoList;

    index = 0;
    while (hdrCmprssnInfoList->n > index && (index < MAX_COMP_ENG_TYPES))
    {
      /* Process RFC 2507 related info*/
      if (hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.t == 
          T_tdsrrc_AlgorithmSpecificInfo_r4_rfc2507_Info)
      {
        TDSRRC_MSG1(MSG_LEGACY_LOW,"RFC 2507 info present ind %d",index);

        /* Indicates that RFC 2507 info is present */
        pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
          iphc_info.comp_eng_type = PDCP_IPHC;

        /* Largest number of compressed non-TCP headers that may be sent without sending a full header. */
    
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_f_max_period = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->f_MAX_PERIOD;
       /* defau;t  TDSRFC_2507_F_MAX_PERIOD;*/
        
        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_F_MAX_PERIOD %d",
                 pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_f_max_period, 0,0);

        /* Compressed headers may not be sent more than F_MAX_TIME seconds after sending last full header. */
      
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_f_max_time = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->f_MAX_TIME;
         /*  default       TDSRFC_2507_F_MAX_TIME */
        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_F_MAX_TIME %d",
                 pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_f_max_time,0,0);

        /* The largest header size in octets that may be compressed */
      
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_max_header = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->max_HEADER;
       /* default   TDSRFC_2507_MAX_HEADER;*/
       
        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_F_MAX_TIME %d",
                 pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_max_header,0,0);

        /* Maximum CID value for TCP connections.*/
    
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_tcp_space = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->tcp_SPACE;
       /* default  TDSRFC_2507_TCP_SPACE;*/
        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_TCP_SPACE %d",
                 pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_tcp_space,0,0);

        /* Maximum CID value for non-TCP connections. */
       
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.iphc_non_tcp_space = (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->non_TCP_SPACE;
        /*  default  TDSRFC_2507_NON_TCP_SPACE;*/

        TDSRRC_MSG3(MSG_LEGACY_LOW,"TDSRFC_2507_NON_TCP_SPACE %d",
                 pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.iphc_non_tcp_space,0,0);

        /* Whether the algorithm shall reorder PDCP SDUs or not.*/
        if (hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->expectReordering == tdsrrc_ExpectReordering_reorderingNotExpected) 
        {
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.expect_reorder = REORDERING_NOT_EXPECTED;
        }
        else if (hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc2507_Info->expectReordering == tdsrrc_ExpectReordering_reorderingExpected) 
        {
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            iphc_info.expect_reorder = REORDERING_EXPECTED;
        }
        TDSRRC_MSG3(MSG_LEGACY_LOW,"RFC_2507_EXPECTED_REORDERING %d",
                 pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                 iphc_info.expect_reorder,0,0);

      } /*if (hdrCmprssnInfoList->value.algorithmSpecificInfo.choice & rrc_AlgorithmSpecificInfo_r4_rfc2507_Info_chosen)*/

      else if (hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.t == T_tdsrrc_AlgorithmSpecificInfo_r4_rfc3095_Info) 
      {
        TDSRRC_MSG1(MSG_LEGACY_LOW,"RFC 3095 info present ind %d",index);
        /* Indicates that RFC 3095 info is present */
        pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
          rohc_info.comp_eng_type = PDCP_ROHC;


        /* Profiles supported by both compressor and decompressor in both UE and UTRAN. Profile 0 shall always be supported. */
        tdsrrcllc_init_rfc_3095_profiles(pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.
                               comp_eng_info[index].rohc_info.rohc_profiles);

        rohcProflList = &hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc3095_Info->rohcProfileList;
        count = 0;
        while((rohcProflList->n > count) && count < TDSRFC_3095_MAX_ROHC_PROFILES)
        {
          if(rohcProflList->elem[count] == 1)
          {
            pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
              rohc_info.rohc_profiles[count] = ROHC_PROFILE_RTP;
            TDSRRC_MSG1(MSG_LEGACY_LOW,"RFC_3095 rohc profile count %d value 1 = ROHC_PROFILE_RTP",count );
          }
          else if(rohcProflList->elem[count] == 2)
          {
            pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
              rohc_info.rohc_profiles[count] = ROHC_PROFILE_UDP;

            TDSRRC_MSG1(MSG_LEGACY_LOW,"RFC_3095 rohc profile count %d value 2 = ROHC_PROFILE_UDP",count );
          }

          count++;
        } // while(rohcProflList)
        /* Always set the last profile to uncompressed as uncompressed should 
         * always be supported by the UE
         */
        pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
          rohc_info.rohc_profiles[count] = ROHC_PROFILE_UNCOMPRESSED;
        TDSRRC_MSG1(MSG_LEGACY_LOW,"RFC_3095 rohc profile count %d to uncompressed",count );
        
        /* Process Uplink ROHC info if present*/
        if (hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc3095_Info->m.ul_RFC3095Present) 
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"RFC 3095 UL info present");
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            rohc_info.rohc_ul_info_present = TRUE;

          /* Highest context ID number to be used by the UE compressor */

            pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
              rohc_info.rohc_ul_max_cid =
               (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc3095_Info->ul_RFC3095.max_CID;
          /* defau;t  TDSRFC_3095_MAX_CID;*/
          

          /*  as per section 8.6.4.10, for RFC 3095:
              the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as
              indicated in the IE "PDCP Capability".
          */
          if (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
              rohc_info.rohc_ul_max_cid > 4) 
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid config.  MAX_CID is > than advertized max num ROHC context sessions",
                tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                rohc_info.rohc_ul_max_cid,0,0);
            return FAILURE;
          }

          TDSRRC_MSG3(MSG_LEGACY_LOW,"RFC 3095 ul max_cid %d",
                   pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                   rohc_info.rohc_ul_max_cid,0,0);

        }

        /* Process downlink ROHC info if present*/
        if (hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc3095_Info->m.dl_RFC3095Present) 
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"RFC 3095 DL info present");
          pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
            rohc_info.rohc_dl_info_present = TRUE;

          /* Highest context ID number to be used by the UE compressor */

            pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
              rohc_info.rohc_dl_max_cid =
               (uint16)hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.max_CID;
          /* defaULT  TDSRFC_3095_MAX_CID;*/
          

          /*  as per section 8.6.4.10, for RFC 3095:
              the chosen MAX_CID shall not be greater than the value "Maximum number of ROHC context sessions" as
              indicated in the IE "PDCP Capability".
          */
          if (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
              rohc_info.rohc_dl_max_cid > 4) 
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid config.  MAX_CID is > than advertized max num ROHC context sessions",
                tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                rohc_info.rohc_ul_max_cid,0,0);
            return FAILURE;
          }

          TDSRRC_MSG3(MSG_LEGACY_LOW,"RFC 3095 dl max_cid %d",
                   pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                   rohc_info.rohc_dl_max_cid,0,0);


          /* Determines whether reverse decompression should be used or not and the maximum number of packets that can be reverse
             decompressed by the UE decompressor.*/
       
            pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
              rohc_info.rohc_dl_rev_decomp_depth = 
              hdrCmprssnInfoList->elem[index].algorithmSpecificInfo.u.rfc3095_Info->dl_RFC3095.reverseDecompressionDepth;
         /*  DEFAULT TDSRFC_3095_REVERSE_DECOMPRESSION_DEPTH;*/
          

          TDSRRC_MSG3(MSG_LEGACY_LOW,"RFC 3095 reverse decompression depth%d",
                   pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info.comp_eng_info[index].
                   rohc_info.rohc_dl_rev_decomp_depth,0,0);

        }

      } /*else if (hdrCmprssnInfoList->value.algorithmSpecificInfo.choice & rrc_rfc3095_Info_chosen) */
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Should not come here");
      }
      index++;
     
    }/* while (hdrCmprssnInfoList) */

    /* Call PDCP function to futher validate PDCP info */
    if (pdcp_validate_params(pdcp_parms_ptr->pdcp_config_req[idx].pdcp_cfg_req.pdcp_info,
      FALSE
       ))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP validation is OK");
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"pdcp validation failed");
      return FAILURE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"PDCP header compression info list not present");
  }

  return SUCCESS;

} /* end tdsrrcllc_get_pdcp_info_later_than_r3() */

#endif /* FEATURE_UMTS_PDCP */

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
tdsrrc_RB_InformationSetupList_r5   * rb_setup_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type    logch_mode = UE_MODE_MAX_NUM;
  
  /*initialize a counter to check rb count*/
  uint8                         rb_cnt  = 0;
  
#ifdef FEATURE_UMTS_PDCP
  uint8 idx = 0;
#endif /* FEATURE_UMTS_PDCP */

  /*place holder for rb_id we get originally*/
  tdsrrc_RB_Identity                    local_rb_id = (tdsrrc_RB_Identity)ie_ptr->rb_Identity;
  
  /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_RB_InformationSetup_r5     *temp_ie_ptr = ie_ptr;

  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
  *rlc_Info for the rb_id specified.  Walk thru the rb_setup_head_ptr
  *list until we find a rb_id that matches
  */
  if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r5_same_as_RB))
  {
    if (rb_setup_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"rb_setup_ptr is NULL");  
      return FAILURE;
    }
    while ((rb_setup_ptr->n > rb_cnt) && (rb_cnt < MAX_RB_PER_RAB))
    {
      if (ie_ptr->rlc_InfoChoice.u.same_as_RB == 
        rb_setup_ptr->elem[rb_cnt].rb_Identity)
      {
        if(rb_setup_ptr->elem[rb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r5_rlc_Info)
        {
           temp_ie_ptr = &(rb_setup_ptr->elem[rb_cnt]);
           break;
        }
      }
      
      rb_cnt++;
    } /* end RB per RAB setup */
    
    if ((rb_setup_ptr->n == 0) || (rb_cnt >= MAX_RB_PER_RAB) || (rb_cnt >= rb_setup_ptr->n))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to match rb_id to same_as_RB");  
      return FAILURE;
    }
  } /* if RLC info choice is "Same as RB" type */
  
    /* Now we have either the RLC Info explicitly meant for this RB OR
    * the RLC Info that is the same as another RB.Get the RLC mode */
  
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r5_rlc_Info) &&
       ((logch_mode = tdsrrcllc_get_rlc_mode_r5(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
     )
    {
      return FAILURE;
    }
    
    /* Save off the RLC Info */
  if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r5_rlc_Info) &&
      (tdsrrcllc_get_rlc_info_r5(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, 
        local_rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }

  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info_r5(&ie_ptr->rb_MappingInfo, local_rb_id, logch_mode)
    == FAILURE)
  {
    return FAILURE;
  }
  
#ifdef FEATURE_UMTS_PDCP

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r5,pdcp_Info))
  {
  if (tdsrrc_pdcp_enabled)  
  { 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDPC info present in rb setup");
    if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,
                                    &ie_ptr->pdcp_Info, 
                                    local_rb_id, 
                                    TDSCPDCP_SETUP
                                    ) == FAILURE)
    {
      return FAILURE;
    }
    return SUCCESS;
  }
  else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r5,pdcp_Info)) &&
      ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
       (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
       ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
        (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)))))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP NV not enabled.  PDCP info not handled");
    return FAILURE;
  }
  }

  /* if fall-thru happens, PDCP layer has to be setup in transparent [pass thru] mode */
  if (cn_Domain == tdsrrc_CN_DomainIdentity_ps_domain) 
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting up PDCP in transparent mode rb_id %d",local_rb_id);
    /*Get the next free index in pdcp_config_req */
    while (idx < MAX_RAB_TO_SETUP) 
    {
      if (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use == FALSE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"pdcp config req index is %d",idx);
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].rb_id = local_rb_id;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.action = SETUP_PDCP;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.is_mcast_call = FALSE;

        tdsordered_config_ptr->pdcp_parms.num_rbs++;
        break;
      }
      else
      {
        idx++;
      }
    }
    /* CPDCP action will be setup, but pdcp info will be null */
    //tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
  }
#else
  /* Ignore the PDCP info for now */
  /********************************/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r5,pdcp_Info)) &&
      ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
       (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
       ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
        (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)))))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }
#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

} /* end tdsrrcllc_get_rb_info_to_setup_r5() */


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
tdsrrc_SRB_InformationSetupList_r5 * srb_setup_ptr,
tdsrrc_RB_Identity                   rb_id
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  
  /* Local count of # of SRB's to setup */
  uint8                           srb_cnt = 0;
  
  /* temp rb id which we shall compute if rb_id is not part of
  * srb_setup_ptr when we loop thru the list to get a match
  */
  tdsrrc_RB_Identity                      temp_rb_id = 0;
  

  
  /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_SRB_InformationSetup_r5     *temp_ie_ptr = ie_ptr;
  
  /* Entity indexes in case we need to look up RLC info from
  * our internal database instead of processing an IE.
  */
  uint8 entity_idx, new_entity_idx;
  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
  *rlc_Info for the rb_id specified.  Walk thru the srb_setup_head_ptr
  *list until we find a rb_id that matches
  */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r5_same_as_RB))
  {
    while ((srb_setup_ptr != NULL) && (srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      
      /* Check to see if the RB ID is there ... */
      if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        temp_rb_id = (tdsrrc_RB_Identity)srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }
      
      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        temp_rb_id++;
      }
      
      /*check if the rb_id matches*/
      if (ie_ptr->rlc_InfoChoice.u.same_as_RB == temp_rb_id)
      {
        if(srb_setup_ptr->elem[srb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r5_rlc_Info)
        {
           temp_ie_ptr = &(srb_setup_ptr->elem[srb_cnt]);
           break;
        }
      }
      
      
      srb_cnt++;
    } /* end while... */
    
    if (((srb_setup_ptr != NULL) && (srb_setup_ptr->n == 0)) ||
        (srb_cnt >= UE_MAX_SRB_SETUP) || 
        ((srb_setup_ptr != NULL) && (srb_cnt >= srb_setup_ptr->n)))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"same_as_RB %d not in msg,lookup req'd",ie_ptr->rlc_InfoChoice.u.same_as_RB);
      temp_ie_ptr = NULL;
      temp_rb_id = (tdsrrc_RB_Identity)ie_ptr->rlc_InfoChoice.u.same_as_RB;
      
      if ((entity_idx = tdsrrc_get_am_index(temp_rb_id)) != TDSUE_MAX_AM_ENTITY)
      {
      /* This RB is in AM mode. And its reference config is found in OC.
        */
        if ((new_entity_idx = tdsrrcllc_get_1st_hole_for_rlc_am()) == TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't find space for new rb %d",rb_id);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_ul_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->common_config.rb_id = rb_id;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan >
          TDSUE_MAX_UL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many UL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_dl_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan >
          TDSUE_MAX_DL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many DL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->rlc_am_parms.nentity++;
        if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many AM entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
          return FAILURE;
        }
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
      }
      if ((entity_idx = tdsrrcllc_get_ul_um_index(temp_rb_id))
        != TDSUE_MAX_UL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_ul_um_info(rb_id, TRUE) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      if ((entity_idx = tdsrrcllc_get_dl_um_index(temp_rb_id, tdsordered_config_ptr))     
        != TDSUE_MAX_DL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_dl_um_info(rb_id, TRUE, tdsordered_config_ptr) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      
      /* SRBs cannot be TM.  If we haven't found it by now, we have
      * a problem. */
      if (logch_mode == UE_MODE_MAX_NUM)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"same_as_RB %d not found in msg or OC",ie_ptr->rlc_InfoChoice.u.same_as_RB);
        return FAILURE;
      }
    }
  } /* end RLC choice == Same as RB */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_r5_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,ul_RLC_Mode)) ||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,dl_RLC_Mode)&&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,ul_RLC_Mode)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL or UL or Both RLC mode info not provided for rb id %d",rb_id);
      return FAILURE;
    }
  
  }
  /* Now we have either the RLC Info explicitly meant for this RB OR
  * the RLC Info that is the same as another RB.  If tmp_ie_ptr is NULL,
  * then we've already copied the RLC Info from our internal database.
  */
  if (temp_ie_ptr != NULL)
  {
    /* Get the RLC mode */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r5_rlc_Info) && 
         ((logch_mode = tdsrrcllc_get_rlc_mode_r5(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
       )
    {
      return FAILURE;
    }
    
    /* Get the RLC info */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r5_rlc_Info) && 
        (tdsrrcllc_get_rlc_info_r5(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }
  }
  
  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info_r5(&ie_ptr->rb_MappingInfo, rb_id, logch_mode) == FAILURE)
  {
    return FAILURE;
  }
#ifdef FEATURE_TDSCDMA_CHANGE_SRB4_PRIRORITY
  tdsrrcllc_check_and_set_srb3_srb4_priority(); 
#endif 

  if(FAILURE == tdsrrcllc_validate_srb_id(rb_id))
  {
    return FAILURE;
  }

  /* everything is configured */
  return SUCCESS;

} /* end tdsrrcllc_get_srb_info_to_setup_r5() */



/*==========================================================
FUNCTION  : tdsrrcllc_update_ul_ded_log_chl_info_r6()

DESCRIPTION : Processes UL Logical channel info and fills it
              in passed pointer to MAC I/F struct.

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
static void  tdsrrcllc_update_ul_ded_log_chl_info_r6
 (
  tdsrrc_UL_LogicalChannelMapping_r6 *ie_ptr, 
  tdsmac_ul_ded_logch_config_type  *dlchan_info,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
 )
{
  /* If Logical Channel ID is present, save MAC Logical Channel Identity */ 
  if(ie_ptr->ul_TrCH_Type.u.dch_rach_usch->m.logicalChannelIdentityPresent)
  {
    dlchan_info->mac_id =
      (uint8)ie_ptr->ul_TrCH_Type.u.dch_rach_usch->logicalChannelIdentity;
  }
  else
  {
    dlchan_info->mac_id = 
      TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT;
  }

  /* MAC Logical Channel Priority */
  dlchan_info->priority =
    (uint8)ie_ptr->mac_LogicalChannelPriority;

  /* Save Radio Bearer ID */
  dlchan_info->rb_id = rb_id;

  /* Determine the type of Logical Channel mapped to DCH TrCH */
  if(rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    dlchan_info->chan_type = UE_LOGCHAN_DCCH;
  }
  else
  {
    dlchan_info->chan_type = UE_LOGCHAN_DTCH;
  }

  /* Save the logical channel mode, if a setup */
  if(logch_mode != UE_MODE_MAX_NUM)
  {
    dlchan_info->rlc_mode = logch_mode;
  }
}  /* tdsrrcllc_update_ul_ded_log_chl_info */


/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_size_list_r6()

DESCRIPTION : Processes the RLC Size List IE contents and saves 
              them in the RLC size restriction database.  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: The RLC size restriction database is updated.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rlc_size_list_r6
(
  tdsrrc_UL_LogicalChannelMapping_r6 *ie_ptr,
  tdsrrcllcoc_rlc_size_list_type  *rlc_size_rec,
  tdsrrc_RB_Identity                    rb_id
)
{
  tdsrrc_RLC_SizeExplicitList *rlc_size_list = NULL;

  rlc_size_rec->rb_id = rb_id;
  switch(ie_ptr->ul_TrCH_Type.u.dch_rach_usch->rlc_SizeList.t)
  {
    case T_tdsrrc_UL_LogicalChannelMapping_r6_rlc_SizeList_allSizes:
      rlc_size_rec->restriction_type = TDSRLC_SIZES_ALL;
      break;

    case T_tdsrrc_UL_LogicalChannelMapping_r6_rlc_SizeList_configured:
      rlc_size_rec->restriction_type = TDSRLC_SIZES_CONFIGURED;
      break;

    case T_tdsrrc_UL_LogicalChannelMapping_r6_rlc_SizeList_explicitList:
      rlc_size_rec->restriction_type = TDSRLC_SIZES_EXPLICIT_LIST;
      rlc_size_list = ie_ptr->ul_TrCH_Type.u.dch_rach_usch->rlc_SizeList.u.explicitList;
      if(rlc_size_list == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RLC Size List empty");
        return(FAILURE);
      }
      rlc_size_rec->count = 0;
      while(rlc_size_list->n > rlc_size_rec->count)
      {
        rlc_size_rec->size_idx[rlc_size_rec->count] = 
			(uint16)rlc_size_list->elem[rlc_size_rec->count].rlc_SizeIndex;
        rlc_size_rec->count++;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RLC Size List CHOICE: %d",ie_ptr->ul_TrCH_Type.u.dch_rach_usch->rlc_SizeList.t);
      return(FAILURE);
  }

  return SUCCESS;
} /* end tdsrrcllc_get_rlc_size_list_r6() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rach_mapping_info_r6()

DESCRIPTION : Process RACH Mapping IE's contents and saves 
              them in the MAC Ordered Configuration Database.
              If next RRC state is CELL_DCH, save the RACH
              mapping contents in a local data structure in
              OC.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rach_mapping_info_r6
(
  tdsrrc_UL_LogicalChannelMapping_r6 *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode,
  tdsrrc_state_e_type             next_rrc_state
)
{
  /* transport Channel Index */
  uint8   trch_idx  = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Get the logical channel index */
  uint8   logch_idx;
 
  tdsmac_ul_ded_logch_config_type  *dlchan_info;
  rlc_lc_id_type   rlc_lc_id = (uint8)TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
  /* TBD: Although all existing FACHs would be mapped to each of the RBs,
   * fill up only for the 1st FACH - till MAC interface is clarified/updated
   */

  boolean update_rach_rlc_id = FALSE;

  /* Temporary pointer for processing the RLC Size List */
  tdsrrcllcoc_rlc_size_list_type *rlc_size_rec = NULL;


  /* Start with updating Cell FACH RB Mapping Info when the next state is
   * either CELL_DCH or CELL_FACH
   */
  logch_idx = tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan;
  dlchan_info = &tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[logch_idx];

  /* Save the RLC Size List information */
  tdsordered_config_ptr->rach_rlc_size_restriction_info.mac_update_needed = TRUE;
  rlc_size_rec =
    &(tdsordered_config_ptr->rach_rlc_size_restriction_info.rlc_size_list[logch_idx]);
  if(tdsrrcllc_get_rlc_size_list_r6(ie_ptr, rlc_size_rec, rb_id) == FAILURE)
  {
    return FAILURE;
  }


  if(logch_mode == UE_MODE_MAX_NUM)
  {
    /* This is the case when RB mapping info applicable to CELL_FACH is coming 
    * in "Affected List". At this point, we need to update the Logical Channel mode 
    * and Logical Channel ID in Cell FACH RB Mapping Info - 
    * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
    */
    rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if(rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid UL LC ID for RB %d",rb_id);
      return(FAILURE);
    }
    
    if(tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get UL Log Chl mode for LC %d",rlc_lc_id);
    }
    /* Also update the Logical Channel ID here */
    dlchan_info->rlc_id = rlc_lc_id;
    update_rach_rlc_id = TRUE;
  }
  else
  {
    rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if(rlc_lc_id  != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      /* Also update the Logical Channel ID here */
      dlchan_info->rlc_id = rlc_lc_id;
      update_rach_rlc_id = TRUE;
    }
  }
  
  tdsrrcllc_update_ul_ded_log_chl_info_r6(ie_ptr, dlchan_info, rb_id, logch_mode);
  tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan ++;  
   
  /* Now update the FACH Info ONLY IF the next state is CELL_FACH */
  if(next_rrc_state == TDSRRC_STATE_CELL_FACH ||
     next_rrc_state == TDSRRC_STATE_CELL_PCH ||
     next_rrc_state == TDSRRC_STATE_URA_PCH)
  {
    if((trch_idx = tdsrrcllc_find_ul_rach_trch_idx()) >= TDSUE_MAX_TRCH)
    {
      return FAILURE;
    }
    logch_idx = tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan;
    dlchan_info = &tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx];
    if(update_rach_rlc_id)
    {
      dlchan_info->rlc_id = rlc_lc_id;
    }
    tdsrrcllc_update_ul_ded_log_chl_info_r6(ie_ptr, dlchan_info, rb_id, logch_mode);

    tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan++;
    tdsordered_config_ptr->ul_rach_trch_info[trch_idx].lg_ch_cnt = 
      tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan;

  }  /* If next state is CELL_FACH, update the FACH Info */
 
  return SUCCESS;

} /* end tdsrrcllc_get_rach_mapping_info_r6() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_dch_mapping_info_r6()

DESCRIPTION : Process UL DCH Mapping IE's contents and saves 
              them in the MAC Ordered Configuration Database.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_dch_mapping_info_r6
(
  tdsrrc_UL_LogicalChannelMapping_r6 *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
)
{
  /* transport Channel Index */
  uint8         trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id = 0;

  /* Get the logical channel index */
  uint8         logch_idx;
  rlc_lc_id_type   rlc_lc_id;
  uint8 cnt = 0;


  if(TDSRRC_CHECK_COMMON_MSG_TYPE( ie_ptr->ul_TrCH_Type,rrc_UL_LogicalChannelMapping_r6_ul_TrCH_Type_dch_rach_usch))
  {
    /* Get the UL DCH TrCH ID if it exists */
    switch (ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.t)
    {
      case T_tdsrrc_UL_TransportChannelType_dch:
        tdstrch_id = (tr_ch_id_type)
          ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.u.dch;
        break;
  
      case T_tdsrrc_UL_TransportChannelType_rach:
      case T_tdsrrc_UL_TransportChannelType_dummy:
      case T_tdsrrc_UL_TransportChannelType_usch:
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Expecting UL DCH TrCH ID");
        return FAILURE;
    } /* end UL DCH TrCH ID switch */
  
    /* Get the transport channel index for this transport channel ID.
     * Note: The index should already be there.
     */
    trch_idx = tdsrrcllc_find_ul_dch_trch_idx(tdsordered_config_ptr, tdstrch_id);
    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      return(tdsrrcllc_backup_ul_dch_from_ota_r6(ie_ptr,rb_id,logch_mode));
    }
  
    /* Get the Logical Channel Index */
    logch_idx = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan;
  
    for (cnt = 0; cnt < tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan; cnt++) 
    {
      if (rb_id == tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnt].rb_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB_ID %d already present",rb_id);
        return (SUCCESS); 
      }
    }

    /* If Logical Channel ID is present, save MAC Logical Channel Identity */
    if (ie_ptr->ul_TrCH_Type.u.dch_rach_usch->m.logicalChannelIdentityPresent)
    {
      
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].mac_id =
        (uint8)ie_ptr->ul_TrCH_Type.u.dch_rach_usch->logicalChannelIdentity;
    }
    else 
    {
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].mac_id =
        TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT;
    }

    /* Initialize the MAC data structures to allow all RLC sizes, just in case. */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rlc_size_info.all_tfi_valid = TRUE;

    /* MAC Logical Channel Priority */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].priority =
      (uint8) ie_ptr->mac_LogicalChannelPriority;
  
    /* Save Radio Bearer ID */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rb_id = rb_id;
  
    /* Determine the type of Logical Channel mapped to DCH TrCH */
    if(rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
    {
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
    }
    else
    {
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
    }
  
    if(logch_mode == UE_MODE_MAX_NUM)
    {
      /* This is the case when RB mapping info applicable to CELL_FACH is coming 
       * in "Affected List". At this point, we need to update the Logical Channel mode 
       * and Logical Channel ID in Cell FACH RB Mapping Info - 
       * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
       */
      rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
      if(rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid UL LC ID for RB %d",rb_id);
        return(FAILURE);
      }
    
      if(tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get UL Log Chl mode for LC %d",rlc_lc_id);
      }
  
      /* Also update the Logical Channel ID here */
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rlc_id = rlc_lc_id;
    }
    
    /* Save the logical channel mode, if a setup */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rlc_mode = logch_mode;
  
    /* other wise it is just a reconfig of RB Mapping info */
  
    /* Increment number of logical channels */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan++;
    
    /* The logic channel mapped to this TrCH increases from 1 to 2. */
    /* Need to update the TB size to reflect this change.           */
    if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan == 2 &&
      tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt == 1)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TrCH %d Need to increase TB size",tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].trch_id);
      tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt_up = TRUE;
    }
    tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan;
  }
  else /* rrc_ul_TrCH_Type_e_dch_chosen */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: R6 ASN1 rrc_ul_TrCH_Type_e_dch_chosen not yet supported");
  }
  tdsrrcllc_free_hanging_rb_mapping_info_index(rb_id,TDSUL_DCH_RB_MAPPING);

  return SUCCESS;

} /* end tdsrrcllc_get_ul_dch_mapping_info() */

/*==========================================================
FUNCTION  : tdsrrcllc_check_fixed_ul_rlc_size_change()

DESCRIPTION : 
  This function compares the RLC size when fixed size is 
  configured and then sets the RLC reestablishment 
  type.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_check_fixed_ul_rlc_size_change
(
tdsrrc_RB_Identity rb_id,
boolean one_sided_reest_present,
boolean one_sided_reest,
uint8 mac_dflow_index,
uint8 logch_idx
)
{
  tdsrlc_reestablish_e_type reestablish_entity = TDSRLC_RE_ESTABLISH_NONE;
  uint8 entity_idx = 0;
  uint32 rlc_size_old = 0, rlc_size_new = 0;

    entity_idx = tdsrrc_get_am_index(rb_id);
    
    /* LLC processes the UL AM info first, it should find the corresponding
           entity index for DL AM info to be filled in. */
    if (entity_idx == TDSUE_MAX_AM_ENTITY)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not allowed to add new UL AM RLC entity");
      return FAILURE;
    }

    rlc_size_old = tdsrrcllc_get_am_rlc_size_for_rb(rb_id, TDSUL);
    rlc_size_new = tdsrrcllc_get_ul_ded_rlc_size(tdsordered_config_ptr->rrc_state_indicator, rb_id);


    if((rlc_size_old != TDSRRCLLC_INVALID_RLC_SIZE) && (rlc_size_new != TDSRRCLLC_INVALID_RLC_SIZE) 
            && (rlc_size_old != rlc_size_new))
    {
    
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"rlcsizeindicatedEUL: OldSize %d,NewSize %d,Reestab[0:B/1:DL] %d ", rlc_size_old,
          rlc_size_new,reestablish_entity);
  
      if ((one_sided_reest_present == TRUE) && (one_sided_reest == TRUE))
      {
        if(tdsordered_config_ptr->am_config[entity_idx].reestab_entity== TDSRLC_RE_ESTABLISH_DL)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Both UL and DL size changed when rlc_OneSidedReEst is TRUE");
           return FAILURE;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish UL only");
          reestablish_entity = TDSRLC_RE_ESTABLISH_UL;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicatedEUL Reestablish DL & UL");
        reestablish_entity = TDSRLC_RE_ESTABLISH_DL_UL;
      }

        tdsordered_config_ptr->am_config[entity_idx].reestab_entity = reestablish_entity;
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"rlcsizeindicatedEUL: Old Size %d,New Size %d",rlc_size_old,rlc_size_new);
    }

  return SUCCESS;
}

#ifdef FEATURE_TDSCDMA_HSUPA

/*==========================================================
FUNCTION  : tdsrrcllc_get_edch_mapping_info_r6()

DESCRIPTION : Processes EDCH RB Mapping info contents and saves 
              them in MAC Ordered Configuration Data  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_edch_mapping_info_r6
(
tdsrrc_UL_LogicalChannelMapping_r6 *ie_ptr,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode
,boolean one_sided_reest_present
,boolean one_sided_reest
,boolean allow_new_channels
)
{
  /* EDCH MAC-D FLOW Id */
  uint32         dflow_id;
  /* Get the logical channel index */
  uint8         logch_idx;
  rlc_lc_id_type   rlc_lc_id;
  uint8 mac_dflow_index;

  tdsrrc_RLC_PDU_SizeList *rlc_pdu_sizelist;
  uint8 ddi = 0;
  uint32 cnt = 0;
  uint16 rlc_size = 0;
  uint32 index = 0;


  tdsrlc_reestablish_e_type reestablish_entity = TDSRLC_RE_ESTABLISH_NONE;
  uint8 entity_idx = 0;

  dflow_id = ie_ptr->ul_TrCH_Type.u.e_dch->e_DCH_MAC_d_FlowIdentity;
  
  mac_dflow_index = tdsrrcllc_find_e_mac_d_flow_idx((uint8)dflow_id);
  /* If DFLOW Index does not exist, then create a new one */
  if (mac_dflow_index == UE_MAX_MAC_D_FLOW)
  {
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"EDCH Mac-d FLOW Index for DFLOW %d,not found",dflow_id);
    return(tdsrrcllc_backup_eul_from_ota(ie_ptr,rb_id,logch_mode));
  }
  else
  {
    for (index=0;
      index < tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].num_logch; index++)
    {
      if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[index].rb_id == rb_id)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RB_ID %d, logical channel identity %d, mac_id: %d already present",
          rb_id, ie_ptr->ul_TrCH_Type.u.e_dch->logicalChannelIdentity,
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_mac_d_flow_id);      
        return(SUCCESS);
      }
    }
  }

  /* Get the Logical Channel Index */
  logch_idx = tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].num_logch;
  rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);

  if (rlc_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {

    /*This is the case where RB-mapping info is present in rb-reconfig-info list or affected list*/
    if (tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get DL Log Chl mode for LC %d",rlc_lc_id);
      return(FAILURE);
    }
  }
    /* Also update the Logical Channel ID here */
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].rlc_id = rlc_lc_id;
  /* Save the and RLC info DDI info*/
  rlc_pdu_sizelist = &ie_ptr->ul_TrCH_Type.u.e_dch->rlc_PDU_SizeList;

  /* If more than 1 UL RLC PDU size is configured for this RB, the different sizes will use subsequent DDI values starting
    from this DDI value.*/
  ddi = ie_ptr->ul_TrCH_Type.u.e_dch->ddi;
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].num_rlc_size_ddi_info = 0;

  while ((rlc_pdu_sizelist->n > cnt) && (cnt < TDSRRCLLC_MAX_NUM_OF_RLC_PDU_SIZE_PER_LOGCHAN)) 
  {
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].
      rlc_size_ddi_info[cnt].ddi = ddi;

    switch (rlc_pdu_sizelist->elem[cnt].t)
    {
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType1:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType1 * 8) + 16;
        break;
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType2:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType2->part1 * 32) + 272;
        if (rlc_pdu_sizelist->elem[cnt].u.sizeType2->m.part2Present)
        {
          rlc_size += (rlc_pdu_sizelist->elem[cnt].u.sizeType2->part2 * 8);
        }
        break;
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType3:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType3->part1 * 64) + 1040;
        if (rlc_pdu_sizelist->elem[cnt].u.sizeType3->m.part2Present)
        {
          rlc_size += (rlc_pdu_sizelist->elem[cnt].u.sizeType3->part2 * 8);
        }
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC TF type in OctetMode %d",rlc_pdu_sizelist->elem[cnt].t);
        break;
    }
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].
      rlc_size_ddi_info[cnt].rlc_pdu_size_in_bits = rlc_size;

    /* Increment the number of valid fields in rlc_size_ddi_info[] */
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].num_rlc_size_ddi_info++;
    cnt++;
    /* Increment the DDI */
    ddi++;
   
  }
  if(FALSE == allow_new_channels)
  {
    if(logch_mode == UE_MODE_ACKNOWLEDGED_DATA)
    {
        /*Check for UL fixed size change and set the reest type*/
      if(FAILURE == tdsrrcllc_check_fixed_ul_rlc_size_change(rb_id,one_sided_reest_present,one_sided_reest,mac_dflow_index,logch_idx))
      {
        return FAILURE;
      }
    }
  }

  /* Ignore the logical channel identity signalled by NW.  Not needed by MAC*/ 
  
  /* Save Radio Bearer ID */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].rb_id = rb_id;

  /* Get the priority between a user's different RBs (or logical channels). */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].priority =
    ie_ptr->mac_LogicalChannelPriority;
  
  /* Determine the type of Logical Channel mapped to DCH TrCH */
  if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
  } 
  else
  {
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
  }

  /* If set include scheduling info is set to false then dont consider this mac_id while sending schedulingInformation */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].include_in_scheduling_info = 
    ie_ptr->ul_TrCH_Type.u.e_dch->includeInSchedulingInfo;

  /* Set the logical channel Id recieved from the NW */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].lc_id = 
    ie_ptr->ul_TrCH_Type.u.e_dch->logicalChannelIdentity;
  
  /* Save the logical channel mode, if a setup */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].rlc_mode = logch_mode;
  
  /* Increment number of logical channels */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].num_logch++;  

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCEUL:EDCH E-Mac-d FLOW %d, No of dedicated Ch %d, rlc mode: %d(1:UM, >1:AM)",
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_mac_d_flow_id,
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].num_logch,
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].rlc_mode);

  //check RLC Size change only for AM & lets skip this for SRB, 
  //as SRB size doesnt change + EUL already started
  if((logch_mode == UE_MODE_ACKNOWLEDGED_DATA)&&
     (rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)&&
     (tdscurrent_config_ptr->e_dch_transmission == TRUE)&&
     (tdsrrc_find_rb_in_est_rabs(rb_id) == TDSRRC_RB_PRESENT))
  {
    if ((one_sided_reest_present == TRUE) && (one_sided_reest == TRUE))
    {
      if(tdsone_sided_validation_check==TRUE)
      {
        if(tdsordered_config_ptr->am_config[entity_idx].reestab_entity== TDSRLC_RE_ESTABLISH_DL)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Both UL and DL size changed when rlc_OneSidedReEst is TRUE");
           return FAILURE;
        }
        else	  	
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish UL only");
          reestablish_entity = TDSRLC_RE_ESTABLISH_UL;
        }
      }
      else if(tdsone_sided_validation_check==FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish UL only");
        reestablish_entity = TDSRLC_RE_ESTABLISH_UL;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicatedEUL Reestablish DL & UL");
      reestablish_entity = TDSRLC_RE_ESTABLISH_DL_UL;
    }
    entity_idx = tdsrrc_get_am_index(rb_id);
    
    /* LLC processes the UL AM info first, it should find the corresponding
           entity index for DL AM info to be filled in. */
    if (entity_idx == UE_MAX_AM_ENTITY)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not allowed to add new DL AM RLC entity");
      return FAILURE;
    }
    
    if(tdsrrcllc_compare_rlc_size_change(tdsrrcllc_get_ul_rlc_size_for_eul(tdsordered_config_ptr,rb_id),
           tdsrrcllc_get_ul_rlc_size_for_eul(tdscurrent_config_ptr,rb_id)) == TRUE)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"rlcsizeindicatedEUL: OldSize %d,NewSize %d,Reestab[0:B/1:DL] %d ", tdsrrcllc_get_ul_rlc_size_for_eul(tdscurrent_config_ptr,rb_id),
          tdsrrcllc_get_ul_rlc_size_for_eul(tdsordered_config_ptr,rb_id),reestablish_entity);
      
      tdsordered_config_ptr->am_config[entity_idx].reestab_entity = reestablish_entity;
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"rlcsizeindicatedEUL: Old Size %d, New Size %d",tdsrrcllc_get_ul_rlc_size_for_eul(tdscurrent_config_ptr,rb_id),tdsrrcllc_get_ul_rlc_size_for_eul(tdsordered_config_ptr,rb_id));
    }
  }

  tdsrrcllc_free_hanging_rb_mapping_info_index(rb_id,TDSE_DCH_RB_MAPPING);

  return SUCCESS;

}

#endif /* FEATURE_TDSCDMA_HSUPA */

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_single_logical_channel_info_r6()

DESCRIPTION : Process one UL Logical Channel Mapping IE's 
              contents and saves them in the MAC Ordered 
              Configuration Database.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_single_logical_channel_info_r6
(
  tdsrrc_UL_LogicalChannelMapping_r6 *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
  ,boolean one_sided_reest_present
  ,boolean one_sided_reest
  ,boolean allow_new_channels
)
{
  tdsordered_config_type *tmp_config_ptr = NULL;


  if (TDSRRC_CHECK_COMMON_MSG_TYPE( ie_ptr->ul_TrCH_Type,rrc_UL_LogicalChannelMapping_r6_ul_TrCH_Type_dch_rach_usch))
  {
    /* Transport channel type */
    switch (ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.t)
    {
      case T_tdsrrc_UL_TransportChannelType_dch:
        if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
        {
          if (tdsrrcllc_get_ul_dch_mapping_info_r6(ie_ptr, rb_id, logch_mode) == FAILURE)
          {
            return FAILURE;
          }
        }
        else
        {
            if(tdstransition_config.toc_usage == TDSTOC_INVALID)
            {
              tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
            }
  
            tmp_config_ptr = tdsordered_config_ptr;
            tdsordered_config_ptr = tdstransition_config.toc_ptr;
  
            /* Processing functions must think we are going to Cell_DCH state. */
            tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
  
            if(tdsrrcllc_get_ul_dch_mapping_info_r6(ie_ptr, rb_id, logch_mode) == FAILURE)
            {
              tdsordered_config_ptr = tmp_config_ptr;
              tmp_config_ptr = NULL;
              return FAILURE;
            }
  
            tdsordered_config_ptr = tmp_config_ptr;
            tmp_config_ptr = NULL;
        }
        break;
  
      case T_tdsrrc_UL_TransportChannelType_rach:
        {
        if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH ||
           tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH ||
           tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
        {
          if (tdsrrcllc_get_rach_mapping_info_r6(ie_ptr, rb_id, logch_mode,
                              tdsordered_config_ptr->rrc_state_indicator) == FAILURE)
          {
            return FAILURE;
          }
        }
        else
        {
          if (tdsrrcllc_get_rach_mapping_info_r6(ie_ptr, rb_id, logch_mode, TDSRRC_STATE_CELL_DCH) == FAILURE)
          {
            return FAILURE;
          }
        }
        }
        break;
  
      case T_tdsrrc_UL_TransportChannelType_dummy:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"DUMMY chosen by NW");
        return FAILURE;
  
      case T_tdsrrc_UL_TransportChannelType_usch:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"USCH not supported");
        return FAILURE;
  
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd invalid UL TrCh type %d",ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.t);
        return FAILURE;
    }
  }
#ifdef FEATURE_TDSCDMA_HSUPA
  else if(TDSRRC_CHECK_COMMON_MSG_TYPE( ie_ptr->ul_TrCH_Type,rrc_UL_LogicalChannelMapping_r6_ul_TrCH_Type_e_dch))/* rrc_ul_TrCH_Type_e_dch_chosen */
  {
    if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
    {
      if (logch_mode != UE_MODE_TRANSPARENT) 
      {
        if (tdsrrcllc_get_edch_mapping_info_r6(ie_ptr, rb_id, logch_mode
             , one_sided_reest_present, one_sided_reest, allow_new_channels
          ) == FAILURE)
        {
          return FAILURE;
        }
        tdsrrcllc_update_mac_eul_action(TRUE);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot map TM RB on EDCH");
        return FAILURE;
      }
    }
    else
    {
      if(tdstransition_config.toc_usage == TDSTOC_INVALID)
      {
        tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
      }
      
      tmp_config_ptr = tdsordered_config_ptr;
      tdsordered_config_ptr = tdstransition_config.toc_ptr;
      
      /* Processing functions must think we are going to Cell_DCH state. */
      tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
      
      if (logch_mode != UE_MODE_TRANSPARENT) 
      {
        if (tdsrrcllc_get_edch_mapping_info_r6(ie_ptr, rb_id, logch_mode
             , one_sided_reest_present, one_sided_reest, allow_new_channels
            ) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;  
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot map TM RB on EDCH");
        tdsordered_config_ptr = tmp_config_ptr;
        tmp_config_ptr = NULL;  
        return FAILURE;
      }
      
      tdsordered_config_ptr = tmp_config_ptr;
      tmp_config_ptr = NULL;  
    }
  }
#else
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: R6 ASN1 rrc_ul_TrCH_Type_e_dch_chosen not yet supported");
    return FAILURE;
  }
#endif /* FEATURE_TDSCDMA_HSUPA */

  return SUCCESS;

} /* end tdsrrcllc_get_ul_single_logical_channel_info_r6() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_logical_channel_info_r6()

DESCRIPTION : Get UL logical Channel info.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_logical_channel_info_r6
(
  tdsrrc_UL_LogicalChannelMappings_r6 * ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
  ,boolean one_sided_reest_present
  ,boolean one_sided_reest
  ,boolean allow_new_channels
)
{

  /* Process IE info based on number of UL Logical Channels */
  switch (ie_ptr->t)
  {
    /* One UL Logical Channel */
    case T_tdsrrc_UL_LogicalChannelMappings_r6_oneLogicalChannel:
      if(tdsrrcllc_get_ul_single_logical_channel_info_r6(
         ie_ptr->u.oneLogicalChannel, rb_id, logch_mode
         ,one_sided_reest_present,one_sided_reest,allow_new_channels
        )  == FAILURE)
      {
        return FAILURE;
      }
      break;

    /* Two logical channels, currently not supported by RRC-RLC I/F */
    case T_tdsrrc_UL_LogicalChannelMappings_r6_twoLogicalChannels:
      /* TRUE indicates that first logical channel is for Data PDUs and second  
         logical channel is for control PDUs */
      /* FALSE indicates that Data and Control PDUs can be sent on either of them */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Support only 1 Log CH per AM");
        return FAILURE;

      /* Invalid number of logical channels */
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid number of UL logical channels %d",ie_ptr->t );
        return FAILURE;
  } /* end switch */

  return SUCCESS;

} /* end tdsrrcllc_get_ul_logical_channel_info() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_mode_r6()

DESCRIPTION : Gets the RLC mode for a given logical channel.

DEPENDENCIES: None.

RETURN VALUE: uecomdef_logch_mode_e_type indicate whether or 
              not it is TM, UM or AM.

SIDE EFFECTS: None.
============================================================*/
static uecomdef_logch_mode_e_type tdsrrcllc_get_rlc_mode_r6
(
  tdsrrc_RLC_Info_r6 *ie_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  uecomdef_logch_mode_e_type dl_logch_mode = UE_MODE_MAX_NUM;

  /* Uplink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,ul_RLC_Mode))
  {
    switch (ie_ptr->ul_RLC_Mode.t)
    {
      case T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode:
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
        break;
  
      case T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode:
        logch_mode = UE_MODE_UNACKNOWLEDGED;
        break;
  
      case T_tdsrrc_UL_RLC_Mode_ul_TM_RLC_Mode: 
        logch_mode = UE_MODE_TRANSPARENT;
        break;
  
      case T_tdsrrc_UL_RLC_Mode_spare:
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown UL RLC mode %d",ie_ptr->ul_RLC_Mode.t);
        logch_mode = UE_MODE_MAX_NUM;
        break;
    }  /* end UL RLC choice */
  }  /* end if UL RLC present */

  /* Downlink RLC info */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,dl_RLC_Mode))
  {
    switch (ie_ptr->dl_RLC_Mode.t)
    {
      case T_tdsrrc_DL_RLC_Mode_r6_dl_AM_RLC_Mode:
        dl_logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
        break;
  
      case T_tdsrrc_DL_RLC_Mode_r6_dl_UM_RLC_Mode:
        dl_logch_mode = UE_MODE_UNACKNOWLEDGED;
        break;
  
      case T_tdsrrc_DL_RLC_Mode_r6_dl_TM_RLC_Mode: 
        dl_logch_mode = UE_MODE_TRANSPARENT;
        break;
  
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown DL RLC mode %d",ie_ptr->ul_RLC_Mode.t);
        dl_logch_mode = UE_MODE_MAX_NUM;
        break;
    }  /* end DL RLC choice */

    /* If both DL and UL mode is present, check to make sure they are the same */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,ul_RLC_Mode))
    {
      if (logch_mode != dl_logch_mode) 
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"UL %d and DL %d lc mode not a match.  Invalid config",logch_mode,dl_logch_mode);
        logch_mode = UE_MODE_MAX_NUM;
      }
    }
    else
    {
      logch_mode = dl_logch_mode;
    }

  }  /* end if DL RLC present */

  /* Need to know RLC type in either UL or DL */
  if (!((ie_ptr->m.ul_RLC_ModePresent) ||
                            (ie_ptr->m.dl_RLC_ModePresent)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Neither DL nor UL RLC mode present!");
  }


  return logch_mode;

} /* end tdsrrcllc_get_rlc_mode_r6 */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_info_r6 ()

DESCRIPTION : 
  Processes RLC Info IE

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rlc_info_r6
(
tdsrrc_RLC_Info_r6 *ie_ptr,
tdsrrc_RB_Identity    rb_id,
boolean       allow_new_channels
)
{
  uint8 entity_idx;
  boolean new_rlc_am_entity = FALSE;
  
  tdsrlc_reestablish_e_type reestablish_entity = TDSRLC_RE_ESTABLISH_NONE;


  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,altE_bitInterpretation))
  {
    if(((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,ul_RLC_Mode)) &&
        (ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode ))
        ||((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,dl_RLC_Mode)) &&
           (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_RLC_Mode,rrc_DL_RLC_Mode_r6_dl_UM_RLC_Mode))))
    {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"N\\W send ALT E Bit,ALT E Bit  supported");
    }
  }

  /* Uplink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,ul_RLC_Mode))
  {
    if(tdsrrcllc_get_ul_rlc_info(&ie_ptr->ul_RLC_Mode,rb_id,allow_new_channels,&new_rlc_am_entity) == FAILURE)
    {
        return FAILURE;
    }
	
    if(ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode )
    {
      entity_idx = tdsrrcllc_get_ul_um_index(rb_id);
      if(entity_idx < TDSUE_MAX_UL_UM_CHANNEL)
      {
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,altE_bitInterpretation))
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is present");
          tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].alternate_e_bit = TRUE;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is not present");
          tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].alternate_e_bit = FALSE;
        }
      }
    }
	
#ifdef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
    if(ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode )
    {
      entity_idx = tdsrrc_get_am_index(rb_id);
      if(entity_idx == TDSUE_MAX_AM_ENTITY)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"The corresponding UL AM info not present");
        return FAILURE;
      }
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Special HE field is not present in OTA.");        
      tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.special_value_he_config = FALSE;
    }
#endif  
  } /* end if UL RLC present */
  
  /* Downlink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,dl_RLC_Mode))
  {
    switch (ie_ptr->dl_RLC_Mode.t)
    {
      case T_tdsrrc_DL_RLC_Mode_r6_dl_AM_RLC_Mode: /* rrc_dl_AM_RLC_Mode_r5_chosen */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Get DL AM RLC Info for RB %d",rb_id);
        
        entity_idx = tdsrrc_get_am_index(rb_id);
        
        /* LLC processes the UL AM info first, it should find the corresponding
        entity index for DL AM info to be filled in. */
        if (entity_idx == TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not allowed to add new DL AM RLC entity");
          return FAILURE;
        }
        
        if (ie_ptr->rlc_OneSidedReEst == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish DL only");
          reestablish_entity = TDSRLC_RE_ESTABLISH_DL;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish DL & UL");
          reestablish_entity = TDSRLC_RE_ESTABLISH_DL_UL;
        }
        
        if (tdsrrcllc_get_dl_am_rlc_info_r5(ie_ptr->dl_RLC_Mode.u.dl_AM_RLC_Mode, entity_idx, rb_id
          , allow_new_channels, reestablish_entity
          ) == FAILURE)
        {
          return FAILURE;
        }
        
        /* This flag is set when processing UL AM info. */
        if(new_rlc_am_entity)
        {
        /* Indicate that the AM entity has been filled up. LLC does not increase this
        number of entities when process UL AM info.
          */
          tdsordered_config_ptr->rlc_am_parms.nentity++;
          
          /* Make sure UE has not exceeded the maximum number of AM Entities */
          if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too Many AM Entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
            return FAILURE;
          }
          
          /* It always assume to be the shared channel (Control&Data) 
          as we support now.
          */  
          tdsordered_config_ptr->rlc_am_parms.am_info[entity_idx]->dl_nchan = 1;
          
        }
        
#ifdef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
        tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.special_value_he_config = FALSE;
#endif          
        
        break;  /* DL AM */
        
      case T_tdsrrc_DL_RLC_Mode_r6_dl_UM_RLC_Mode: /* rrc_dl_UM_RLC_r6_Mode_chosen NULL */
      
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r6,altE_bitInterpretation))
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is present");
          if(tdsrrcllc_get_dl_rlc_um_info_r6(ie_ptr->dl_RLC_Mode.u.dl_UM_RLC_Mode,rb_id,allow_new_channels
            ,TRUE
            ) == FAILURE)
          {
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is not present");      
          if(tdsrrcllc_get_dl_rlc_um_info_r6(ie_ptr->dl_RLC_Mode.u.dl_UM_RLC_Mode,rb_id,allow_new_channels
            ,FALSE
            ) == FAILURE)
          {
            return FAILURE;
          }
        }
      
        break;
        
      case T_tdsrrc_DL_RLC_Mode_r6_dl_TM_RLC_Mode:
        if(tdsrrcllc_get_dl_rlc_tm_info(ie_ptr->dl_RLC_Mode.u.dl_TM_RLC_Mode->segmentationIndication,rb_id,allow_new_channels)
            == FAILURE)
        {
          return FAILURE;
        }
        break;
        
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown DL RLC mode %d",ie_ptr->dl_RLC_Mode.t);
        return FAILURE;
    } /* end DL RLC choice */
  } /* end if DL RLC present */
  
  /* Got here, everything is a go */
  return SUCCESS;

} /* end tdsrrcllc_get_rlc_info_r6() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_mapping_info_r6()

DESCRIPTION : Get Radio Bearer Mapping Info IE

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rb_mapping_info_r6
(
tdsrrc_RB_MappingInfo_r6      * ie_ptr,
tdsrrc_RB_Identity                 rb_id,
uecomdef_logch_mode_e_type logch_mode
,boolean one_sided_reest_present
,boolean one_sided_reest
,boolean allow_new_channels
)
{
  /*
  * Count to make sure that don't exceed the maximum number 
  * of RB Mux options
  */
  uint8 rb_mux_cnt = 0;

  /* Process the RB Mux option IE in the linked list */
  while ((ie_ptr->n > rb_mux_cnt) && (rb_mux_cnt < MAX_RB_MUX_OPTIONS_REL5))
  {
    /* Check for DL Channel Mapping List */
    if (ie_ptr->elem[rb_mux_cnt].m.dl_LogicalChannelMappingListPresent)
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for DL",rb_id);
      if (tdsrrcllc_get_dl_logical_channel_info_r5(&ie_ptr->elem[rb_mux_cnt].dl_LogicalChannelMappingList,
                                         rb_id, logch_mode) == FAILURE)
      {
        return FAILURE;
      }
      /* Here indicate that MAC Downlink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
    }  /* end dl Log Ch mapping list */

    if (ie_ptr->elem[rb_mux_cnt].m.ul_LogicalChannelMappingsPresent)
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for UL",rb_id);
      if (tdsrrcllc_get_ul_logical_channel_info_r6(&ie_ptr->elem[rb_mux_cnt].ul_LogicalChannelMappings,
                                      rb_id, logch_mode
           ,one_sided_reest_present,one_sided_reest, allow_new_channels
         ) == FAILURE)
      {
        return FAILURE;
      }

      /* Here indicate that MAC Uplink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
    }  /* end ul Log Ch mapping list */

   
    rb_mux_cnt++;

  }  /* end loop of RB Mapping info list */

  return SUCCESS;
} /* end tdsrrcllc_get_rb_mapping_info_r6() */

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
  tdsrrc_SRB_InformationSetupList_r6 * srb_setup_ptr,
  tdsrrc_RB_Identity                   rb_id
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  
  /* Local count of # of SRB's to setup */
  uint8                           srb_cnt = 0;
  
  /* temp rb id which we shall compute if rb_id is not part of
  * srb_setup_ptr when we loop thru the list to get a match
  */
  tdsrrc_RB_Identity                      temp_rb_id = 0;
    
 /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_SRB_InformationSetup_r6     *temp_ie_ptr = ie_ptr;
  
  /* Entity indexes in case we need to look up RLC info from
  * our internal database instead of processing an IE.
  */
  uint8 entity_idx, new_entity_idx;
  
  boolean one_sided_reest_present=FALSE;
  boolean one_sided_reest=FALSE;

  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
  *rlc_Info for the rb_id specified.  Walk thru the srb_setup_head_ptr
  *list until we find a rb_id that matches
  */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r6_same_as_RB))
  {
    while ((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      
      /* Check to see if the RB ID is there ... */
      if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        temp_rb_id = (tdsrrc_RB_Identity)srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }
      
      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        temp_rb_id++;
      }
      
      /*check if the rb_id matches*/
      if (ie_ptr->rlc_InfoChoice.u.same_as_RB == temp_rb_id)
      {
        if(srb_setup_ptr->elem[srb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r6_rlc_Info)
        {
          temp_ie_ptr = &(srb_setup_ptr->elem[srb_cnt]);
          break;
        }
      }
      
      srb_cnt++;
    } /* end while... */
    
    if ((srb_setup_ptr->n == 0) || (srb_cnt >= UE_MAX_SRB_SETUP) || (srb_cnt >= srb_setup_ptr->n))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"same_as_RB %d not in msg,lookup req'd",ie_ptr->rlc_InfoChoice.u.same_as_RB);
      temp_ie_ptr = NULL;
      temp_rb_id = (tdsrrc_RB_Identity)ie_ptr->rlc_InfoChoice.u.same_as_RB;
      
      if ((entity_idx = tdsrrc_get_am_index(temp_rb_id)) != TDSUE_MAX_AM_ENTITY)
      {
      /* This RB is in AM mode. And its reference config is found in OC.
        */
        if ((new_entity_idx = tdsrrcllc_get_1st_hole_for_rlc_am()) == TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't find space for new rb %d",rb_id);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_ul_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->common_config.rb_id = rb_id;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan >
          TDSUE_MAX_UL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many UL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_dl_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan >
          TDSUE_MAX_DL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many DL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->rlc_am_parms.nentity++;
        if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many AM entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
          return FAILURE;
        }
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
      }
      if ((entity_idx = tdsrrcllc_get_ul_um_index(temp_rb_id))      
        != TDSUE_MAX_UL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_ul_um_info(rb_id, TRUE) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      
      if ((entity_idx = tdsrrcllc_get_dl_um_index(temp_rb_id, tdsordered_config_ptr))
        != TDSUE_MAX_DL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_dl_um_info(rb_id, TRUE, tdsordered_config_ptr) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      
      /* SRBs cannot be TM.  If we haven't found it by now, we have
      * a problem. */
      if (logch_mode == UE_MODE_MAX_NUM)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"same_as_RB %d not found in msg or OC",ie_ptr->rlc_InfoChoice.u.same_as_RB);
        return FAILURE;
      }
    }
  } /* end RLC choice == Same as RB */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_r6_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,ul_RLC_Mode))||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,dl_RLC_Mode)&&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,ul_RLC_Mode)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL or UL or Both RLC mode info not provided for rb id %d",rb_id);
      return FAILURE;
    }
  
  }  

  /* Now we have either the RLC Info explicitly meant for this RB OR
   * the RLC Info that is the same as another RB.  If tmp_ie_ptr is NULL,
   * then we've already copied the RLC Info from our internal database.
   */
  if (temp_ie_ptr != NULL)
  {
    /* Get the RLC mode */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r6_rlc_Info) && 
         ((logch_mode = tdsrrcllc_get_rlc_mode_r6(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
       )
    {
      return FAILURE;
    }
      
    /* Get the RLC info */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r6_rlc_Info) && 
         (tdsrrcllc_get_rlc_info_r6(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }
    one_sided_reest = temp_ie_ptr->rlc_InfoChoice.u.rlc_Info->rlc_OneSidedReEst;
    one_sided_reest_present=TRUE;
  }

  
  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info_r6(&ie_ptr->rb_MappingInfo, rb_id, logch_mode
      ,one_sided_reest_present,one_sided_reest,TRUE
     ) == FAILURE)
  {
    return FAILURE;
  }
  
  if(FAILURE == tdsrrcllc_validate_srb_id(rb_id))
  {
    return FAILURE;
  }

  /* everything is configured */
  return SUCCESS;

}

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
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  
  boolean one_sided_reest_present=FALSE;
  boolean one_sided_reest=FALSE;
  #ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
  uint8 entity_idx = 0;
  #endif

  
  /* Check to see if the RLC info is present */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r6,rlc_Info)
    && ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info_r6,ul_RLC_Mode)) ||
    (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info_r6,dl_RLC_Mode))))
  {
    /* Get the RLC mode */
    if ((logch_mode = tdsrrcllc_get_rlc_mode_r6(&ie_ptr->rlc_Info)) == UE_MODE_MAX_NUM)
    {
      return FAILURE;
    }
    
    /* Save off the RLC Info */
    if (tdsrrcllc_get_rlc_info_r6(&ie_ptr->rlc_Info, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, FALSE) == FAILURE)
    {
      return FAILURE;
    }
  } /* end RLC Info Check */

#ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
  /*Copy rlc one side re-establishment flag to OC*/
if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r6,rlc_Info))
{
  if (ie_ptr->rlc_Info.rlc_OneSidedReEst == TRUE)
  {
  
    entity_idx = tdsrrc_get_am_index(ie_ptr->rb_Identity);
    if (entity_idx != UE_MAX_AM_ENTITY)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_ONE_SIDE:  set to ONE_SIDE");
      tdsordered_config_ptr->rlc_one_sided_reest[entity_idx] = TRUE;
    }
  }
}
#endif 
  
  /* Save off the RB Mapping info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r6,rb_MappingInfo))
  {
    /* Remove any existing mapping for the RB */
    tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);
    
    if(ie_ptr->m.rlc_InfoPresent)
    {
      one_sided_reest_present = TRUE;
      one_sided_reest = ie_ptr->rlc_Info.rlc_OneSidedReEst;
    }
	
    
    if (tdsrrcllc_get_rb_mapping_info_r6(&ie_ptr->rb_MappingInfo, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, logch_mode
         ,one_sided_reest_present,one_sided_reest,FALSE
       ) == FAILURE)
    {
      return FAILURE;
    }
  } /* end of RB Mapping info check */
  
#ifdef FEATURE_UMTS_PDCP
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r6,pdcp_Info))
  {
    if (tdsrrc_pdcp_enabled)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDPC info present in rb setup");
      if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,
                                      &ie_ptr->pdcp_Info.pdcp_Info, 
                                      ie_ptr->rb_Identity, 
                                      TDSCPDCP_RECONFIG
                                      ) == FAILURE)
      {
        return FAILURE;
      }
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r6,pdcp_SN_Info)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r6,pdcp_Info)) &&
             ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
              (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
              ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported))))))          
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
      return FAILURE;
    }
  }
#else

  /* Ignore the PDCP info for now */
  /********************************/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r6,pdcp_SN_Info)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r6,pdcp_Info)) &&
             ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
              (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
              ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported))))))          
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }

#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

} /* end tdsrrcllc_get_rb_info_to_reconfig_r6() */

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
)
{
  /* Remove any existing mapping for the RB */
  tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);


  /* Get the RB Mapping info */
  return(tdsrrcllc_get_rb_mapping_info_r6(&ie_ptr->rb_MappingInfo, 
                                (tdsrrc_RB_Identity)ie_ptr->rb_Identity, 
                                UE_MODE_MAX_NUM
                                ,FALSE,FALSE,FALSE
         ));

}  /* end tdsrrcllc_get_rb_info_affected_r6() */

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
  tdsrrc_RB_InformationSetupList_r6   * rb_setup_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type    logch_mode = UE_MODE_MAX_NUM;
  
  /*initialize a counter to check rb count*/
  uint8                         rb_cnt  = 0;

  /*place holder for rb_id we get originally*/
  tdsrrc_RB_Identity                    local_rb_id = (tdsrrc_RB_Identity)ie_ptr->rb_Identity;
  
  /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_RB_InformationSetup_r6     *temp_ie_ptr = ie_ptr;

#ifdef FEATURE_UMTS_PDCP
  uint8 idx = 0;
#endif /* FEATURE_UMTS_PDCP */  
  
  boolean  one_sided_reest_present = FALSE;
  boolean  one_sided_reest = FALSE;
  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
  *rlc_Info for the rb_id specified.  Walk thru the rb_setup_head_ptr
  *list until we find a rb_id that matches
  */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r6_same_as_RB))
  {
    while ((rb_setup_ptr->n > rb_cnt) && (rb_cnt < MAX_RB_PER_RAB))
    {
      if (ie_ptr->rlc_InfoChoice.u.same_as_RB == 
        rb_setup_ptr->elem[rb_cnt].rb_Identity)
      {
        if(rb_setup_ptr->elem[rb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r6_rlc_Info)
        {
           temp_ie_ptr = &(rb_setup_ptr->elem[rb_cnt]);
           break;
        }
      }
  
      rb_cnt++;
    } /* end RB per RAB setup */
    
    if ((rb_setup_ptr->n == 0) || (rb_cnt >= MAX_RB_PER_RAB) || (rb_cnt >= rb_setup_ptr->n))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to match rb_id to same_as_RB");  
      return FAILURE;
    }
  } /* if RLC info choice is "Same as RB" type */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,rrc_RLC_Info_r6,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_r6_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,ul_RLC_Mode))||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,dl_RLC_Mode)&&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r6,ul_RLC_Mode)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL or UL or Both RLC mode info not provided for rb id %d",local_rb_id);
      return FAILURE;
    }
  
  }  
    /* Now we have either the RLC Info explicitly meant for this RB OR
    * the RLC Info that is the same as another RB.Get the RLC mode */
  
    /* Now we have either the RLC Info explicitly meant for this RB OR
    * the RLC Info that is the same as another RB.Get the RLC mode */
  
  if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r6_rlc_Info) && 
       ((logch_mode = tdsrrcllc_get_rlc_mode_r6(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
     )
    {
      return FAILURE;
    }
    
    /* Save off the RLC Info */
  if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r6_rlc_Info) &&
      (tdsrrcllc_get_rlc_info_r6(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, 
       local_rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }
    
    if (temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r6_rlc_Info)
    {
      one_sided_reest_present = TRUE;
      one_sided_reest = temp_ie_ptr->rlc_InfoChoice.u.rlc_Info->rlc_OneSidedReEst;
    }

  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info_r6(&ie_ptr->rb_MappingInfo, local_rb_id, logch_mode
       ,one_sided_reest_present,one_sided_reest,TRUE
      ) == FAILURE)
  {
    return FAILURE;
  }

#ifdef FEATURE_UMTS_PDCP

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r6,pdcp_Info))
  {
    if (tdsrrc_pdcp_enabled)  
    { 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDPC info present in rb setup");
      if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,
                                      &ie_ptr->pdcp_Info, 
                                      local_rb_id, 
                                      TDSCPDCP_SETUP
                                      ) == FAILURE)
      {
        return FAILURE;
      }
      return SUCCESS;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r6,pdcp_Info)) &&
            ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
            (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
            (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,
            rrc_LosslessSRNS_RelocSupport_supported)))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP NV not enabled.  PDCP info not handled");
      return FAILURE;
    }
  }

  /* if fall-thru happens, PDCP layer has to be setup in transparent [pass thru] mode */
  if (cn_Domain == tdsrrc_CN_DomainIdentity_ps_domain) 
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting up PDCP in transparent mode rb_id %d",local_rb_id);
    /*Get the next free index in pdcp_config_req */
    while (idx < MAX_RAB_TO_SETUP) 
    {
      if (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use == FALSE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"pdcp config req index is %d",idx);
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].rb_id = local_rb_id;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.action = SETUP_PDCP;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.is_mcast_call = FALSE;
        tdsordered_config_ptr->pdcp_parms.num_rbs++;
        break;
      }
      else
      {
        idx++;
      }
    }
    /* CPDCP action will be setup, but pdcp info will be null */
    //tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
  }
#else
  /* Ignore the PDCP info for now */
  /********************************/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r6,pdcp_Info)) &&
            ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
            (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
            (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,
            rrc_LosslessSRNS_RelocSupport_supported)))))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }
#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

} /* end tdsrrcllc_get_rb_info_to_setup_r6() */


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
  tdsrrc_SRB_InformationSetupList_r6 * srb_setup_r6_ptr
)
{
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrc_SRB_InformationSetup_r6 * srb5_setup_ptr =NULL;
  tdsrrc_RB_Identity                          srb_id = TDSRRCLLC_INVALID_RB_ID;
#endif/* FEATURE_TDSCDMA_SRB5_SUPPORT */

  if (srb_info_setup_list_present)
  {
    uint8                           srb_cnt =0;
    tdsrrc_RB_Identity                          rb_id = 0;

  
  
    
    while((srb_setup_r6_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      /* Check to see if the RB ID is there ... */
      if(srb_setup_r6_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        rb_id = (tdsrrc_RB_Identity)srb_setup_r6_ptr->elem[srb_cnt].rb_Identity;
      }
      
      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        rb_id++;
      }
      
      /* Save off the RB Mapping info for each SRB */
      if(tdsrrcllc_get_srb_info_to_setup_r6(
        &srb_setup_r6_ptr->elem[srb_cnt], 
        (srb_setup_r6_ptr), 
        rb_id)
        == FAILURE)
      {
        return FAILURE;
      }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      if(rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        /* Pick that pointer that has RB Id > 4 & 
         assume it to be SRB#5 pointer for the time being */

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Probable SRB#5 IE in the message .. processing");
        srb5_setup_ptr = &srb_setup_r6_ptr->elem[srb_cnt];

        srb_id = rb_id;
      }
#else
      if(rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Establishment of SRB greater than ID 4 is not supported");
        return FAILURE;
      }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

      srb_cnt++;
    } /* end SRB setup */
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    /* We are just validating one more SRB, incase 
       SRB#5 is given by n/w */
    if(srb5_setup_ptr != NULL)
    {

      if(tdsrrcllc_update_oc_with_srb5_rel6_ie(srb5_setup_ptr, srb_id) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rejecting SRB#5 configuration given by the network");
        return FAILURE;
      }
    }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */  
#ifdef FEATURE_TDSCDMA_CHANGE_SRB4_PRIRORITY
    tdsrrcllc_check_and_set_srb3_srb4_priority(); 
#endif
  }

  return(SUCCESS);
}

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
  tdsrrc_RAB_InformationSetupList_r6 * rab_setup_ptr
)
{
  if(rab_info_setup_list_present)
  {
    /* Local count of # of RAB's & RB's to setup */
    uint32                             rab_cnt = 0;
    uint32                             rb_cnt  = 0;    
    /* local RAB to setup list pointer */
   
    /* local RB per RAB to setup list pointer */
    tdsrrc_RB_InformationSetupList_r6     * rb_setup_ptr;
    /* Local varible to store head pointer */
   

    
    while((rab_setup_ptr->n > rab_cnt)&& (rab_cnt < MAX_RAB_TO_SETUP))
    {
      rb_setup_ptr  = &rab_setup_ptr->elem[rab_cnt].rb_InformationSetupList;
      rb_cnt =0;
            
      while((rb_setup_ptr->n > rb_cnt ) && (rb_cnt < MAX_RB_PER_RAB))
      {
        if(tdsrrcllc_get_rb_info_to_setup_r6(
           &rb_setup_ptr->elem[rb_cnt],
#ifdef FEATURE_UMTS_PDCP
           rab_setup_ptr->elem[rab_cnt].rab_Info.cn_DomainIdentity,
#endif /* FEATURE_UMTS_PDCP */           
           rb_setup_ptr) == FAILURE)
        {
          return FAILURE;
        }
        
		rb_cnt++;
      } /* end RB per RAB setup */
      
     
      rab_cnt++;
    } /* end rab setup */
  } /* end RAB setup */

  return(SUCCESS);
}

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
  tdsrrc_RB_InformationReconfigList_r6 * rb_reconfig_ptr
)  
{

  if (rb_info_reconfig_list_present)
  {
  
    uint32 idx = 0;
   
    idx = 0;
    while((rb_reconfig_ptr->n > idx) && (idx < TDS_MAX_RB))
    {
      if(tdsrrcllc_get_rb_info_to_reconfig_r6(&rb_reconfig_ptr->elem[idx]) == FAILURE)
      {
        return FAILURE;
      }
    
      idx++;
    } /* end RB reconfig */
  }

  return(SUCCESS);
}

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
  tdsrrc_RB_InformationAffectedList_r6 * rb_affected_ptr
)  
{
  /* Get the affected RB list if present */
  if(rb_info_affected_list_present)
  {
   
    uint32 idx = 0;
    
    while((rb_affected_ptr->n > idx) && (idx < TDS_MAX_RB))
    {
      if(tdsrrcllc_get_rb_info_affected_r6(&rb_affected_ptr->elem[idx]) == FAILURE)
      {
        return (FAILURE);
      }
      
      idx++;
    }
  } /* end Affected RB */

  return(SUCCESS);
}

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
  tdsrrc_RAB_InformationSetupList  *rab_setup_ptr
)  
{
  rab_setup_ptr->n = 1;
  memset (&rab_setup_ptr->elem[0].rab_Info.m, 0, sizeof(rab_setup_ptr->elem[0].rab_Info.m));
    rab_setup_ptr->elem[0].rab_Info.cn_DomainIdentity =
      rab_Info.cn_DomainIdentity;

    if(TDSRRC_MSG_COMMON_BITMASK_IE(rab_Info,tdsrrc_RAB_Info_Post,nas_Synchronisation_Indicator))
    {
      rab_setup_ptr->elem[0].rab_Info.nas_Synchronisation_Indicator =
        rab_Info.nas_Synchronisation_Indicator;
      rab_setup_ptr->elem[0].rab_Info.m.nas_Synchronisation_IndicatorPresent =1;
    }
    rab_setup_ptr->elem[0].rab_Info.rab_Identity = rab_Info.rab_Identity;
    rab_setup_ptr->elem[0].rab_Info.re_EstablishmentTimer =
      predef_rb->re_EstablishmentTimer;
    rab_setup_ptr->elem[0].rb_InformationSetupList =
      predef_rb->rb_InformationList;

  return (SUCCESS);
}


/*==========================================================
FUNCTION  : tdsrrcllc_get_srb_info_to_setup()

DESCRIPTION : Get Signalling Radio Bearer Information to 
              setup.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_srb_info_to_setup
(
  tdsrrc_SRB_InformationSetup     *ie_ptr,
  tdsrrc_SRB_InformationSetupList2 * srb_setup_ptr,
  tdsrrc_RB_Identity                   rb_id
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;

  /* Local count of # of SRB's to setup */
  uint8                           srb_cnt = 0;

  /* temp rb id which we shall compute if rb_id is not part of
   * srb_setup_ptr when we loop thru the list to get a match
   */
  tdsrrc_RB_Identity                      temp_rb_id = 0;

 
  
  /* Local IE pointer - if RLC Info for this RB is the same as 
   * RLC Info for another RB, IE Pointer is made to point to 
   * SRB Info Setup for the same_as RB. 
   * For RB Mapping info, IE Pointer should always point to
   * SRB Info Setup for the RB that came as a parameter.
   */
  tdsrrc_SRB_InformationSetup     *temp_ie_ptr = ie_ptr;
  
  /* Entity indexes in case we need to look up RLC info from
   * our internal database instead of processing an IE.
   */
  uint8 entity_idx, new_entity_idx;

  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
   *rlc_Info for the rb_id specified.  Walk thru the srb_setup_head_ptr
   *list until we find a rb_id that matches
   */
  if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_same_as_RB))
  {
    while((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {

      /* Check to see if the RB ID is there ... */
      if(srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        temp_rb_id = (tdsrrc_RB_Identity)srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }

      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        temp_rb_id++;
      }

      /*check if the rb_id matches*/
      if(ie_ptr->rlc_InfoChoice.u.same_as_RB == temp_rb_id)
      {
        if(srb_setup_ptr->elem[srb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_rlc_Info)
        {
          temp_ie_ptr = &(srb_setup_ptr->elem[srb_cnt]);
          break;
        }
      }

      
      srb_cnt++;
    } /* end while... */

    if ((srb_setup_ptr->n == 0) || (srb_cnt >= UE_MAX_SRB_SETUP) || (srb_cnt >= srb_setup_ptr->n))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"same_as_RB %d not in msg,lookup req'd",ie_ptr->rlc_InfoChoice.u.same_as_RB);
      temp_ie_ptr = NULL;
      temp_rb_id = (tdsrrc_RB_Identity)ie_ptr->rlc_InfoChoice.u.same_as_RB;
      
      if((entity_idx = tdsrrc_get_am_index(temp_rb_id)) != TDSUE_MAX_AM_ENTITY)
      {
        /* This RB is in AM mode. And its reference config is found in OC.
         */
        if ((new_entity_idx = tdsrrcllc_get_1st_hole_for_rlc_am()) == TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't find space for new rb %d",rb_id);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_ul_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->common_config.rb_id = rb_id;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan >
           TDSUE_MAX_UL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many UL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_dl_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan >
           TDSUE_MAX_DL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many DL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->rlc_am_parms.nentity++;
        if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many AM entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
          return FAILURE;
        }
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
      }
       
      if((entity_idx = tdsrrcllc_get_ul_um_index(temp_rb_id))
              != TDSUE_MAX_UL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_ul_um_info(rb_id, TRUE) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      if((entity_idx = tdsrrcllc_get_dl_um_index(temp_rb_id, tdsordered_config_ptr))
              != TDSUE_MAX_DL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_dl_um_info(rb_id, TRUE, tdsordered_config_ptr) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      
      /* SRBs cannot be TM.  If we haven't found it by now, we have
       * a problem. */
      if(logch_mode == UE_MODE_MAX_NUM)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"same_as_RB %d not found in msg or OC",ie_ptr->rlc_InfoChoice.u.same_as_RB);
        return FAILURE;
      }
    }
  } /* end RLC t == Same as RB */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,ul_RLC_Mode))||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,dl_RLC_Mode)&&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,ul_RLC_Mode)))
    {
      ERR("DL or UL or Both RLC mode info not provided for rb id %d", rb_id, 0, 0);
      return FAILURE;
    }
  
  }  

  /* Now we have either the RLC Info explicitly meant for this RB OR
   * the RLC Info that is the same as another RB.  If tmp_ie_ptr is NULL,
   * then we've already copied the RLC Info from our internal database.
   */
  if(temp_ie_ptr != NULL)
  {
    /* Get the RLC mode */
    if((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_rlc_Info) && 
        ((logch_mode = tdsrrcllc_get_rlc_mode(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
      )
    {
      return FAILURE;
    }

    /* Get the RLC info */
    if((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_rlc_Info) && 
        (tdsrrcllc_get_rlc_info(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }
  }

  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if(tdsrrcllc_get_rb_mapping_info(&ie_ptr->rb_MappingInfo, rb_id, logch_mode) == FAILURE)
  {
    return FAILURE;
  }
#ifdef FEATURE_TDSCDMA_CHANGE_SRB4_PRIRORITY
  tdsrrcllc_check_and_set_srb3_srb4_priority(); 
#endif

  if(FAILURE == tdsrrcllc_validate_srb_id(rb_id))
  {
    return FAILURE;
  }

  /* everything is configured */
  return SUCCESS;

} /* end tdsrrcllc_get_srb_info_to_setup() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_setup()

DESCRIPTION : Get Radio Bearer Information to setup.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_setup
(
  tdsrrc_RB_InformationSetup       *ie_ptr,
#ifdef FEATURE_UMTS_PDCP
  tdsrrc_CN_DomainIdentity cn_Domain,
#endif /* FEATURE_UMTS_PDCP*/
  tdsrrc_RB_InformationSetupList   * rb_setup_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type    logch_mode = UE_MODE_MAX_NUM;
  
  /*initialize a counter to check rb count*/
  uint8                         rb_cnt  = 0;

  /*place holder for rb_id we get originally*/
  tdsrrc_RB_Identity                    local_rb_id = (tdsrrc_RB_Identity)ie_ptr->rb_Identity;

  /* Local IE pointer - if RLC Info for this RB is the same as 
   * RLC Info for another RB, IE Pointer is made to point to 
   * SRB Info Setup for the same_as RB. 
   * For RB Mapping info, IE Pointer should always point to
   * SRB Info Setup for the RB that came as a parameter.
   */
  tdsrrc_RB_InformationSetup     *temp_ie_ptr = ie_ptr;

#ifdef FEATURE_UMTS_PDCP
  uint8 idx = 0;
#endif /* FEATURE_UMTS_PDCP */

  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
   *rlc_Info for the rb_id specified.  Walk thru the rb_setup_head_ptr
   *list until we find a rb_id that matches
   */
  if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_same_as_RB))
  {
    while((rb_setup_ptr->n > rb_cnt) && (rb_cnt < MAX_RB_PER_RAB))
    {
      if(ie_ptr->rlc_InfoChoice.u.same_as_RB == 
         rb_setup_ptr->elem[rb_cnt].rb_Identity)
      {
        if(rb_setup_ptr->elem[rb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_rlc_Info)
        {
           temp_ie_ptr = &(rb_setup_ptr->elem[rb_cnt]);
           break;
        }
      }
     
      rb_cnt++;
    } /* end RB per RAB setup */

    if ((rb_setup_ptr->n == 0) || (rb_cnt >= MAX_RB_PER_RAB) || (rb_cnt >= rb_setup_ptr->n))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to match rb_id to same_as_RB");  
      return FAILURE;
    }
  } /* if RLC info t is "Same as RB" type */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,ul_RLC_Mode))||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,dl_RLC_Mode)&&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info,ul_RLC_Mode)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL or UL or Both RLC mode info not provided for rb id %d",local_rb_id);
      return FAILURE;
    }
  
  }  
  /* Now we have either the RLC Info explicitly meant for this RB OR
   * the RLC Info that is the same as another RB.
   */
  /* Get the RLC mode */
  /* Now we have either the RLC Info explicitly meant for this RB OR
   * the RLC Info that is the same as another RB.
   */
  /* Get the RLC mode */
  if((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_rlc_Info) && 
      ((logch_mode = tdsrrcllc_get_rlc_mode(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
    )
    {
      return FAILURE;
    }

    /* Save off the RLC Info */
  if((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_rlc_Info) && 
      (tdsrrcllc_get_rlc_info(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, 
                  local_rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }

  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if(tdsrrcllc_get_rb_mapping_info(&ie_ptr->rb_MappingInfo, local_rb_id, logch_mode)
     == FAILURE)
  {
    return FAILURE;
  }
  

#ifdef FEATURE_UMTS_PDCP
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup,pdcp_Info))
  {
    if (tdsrrc_pdcp_enabled)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDCP info present in rb setup");
      if (tdsrrcllc_get_pdcp_info(&ie_ptr->pdcp_Info, local_rb_id, TDSCPDCP_SETUP) == FAILURE)
      {
        return FAILURE;
      }
    }
    else  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup,pdcp_Info)) &&
      ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
       (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info,headerCompressionInfoList)) ||
       ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info,losslessSRNS_RelocSupport)) &&
        (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported))))) 
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP nv turned off.  Not handling pdcp");
      return FAILURE;
    }
  }
  /* PDCP layer has to be setup in transparent [pass thru] mode */
  if (cn_Domain == tdsrrc_CN_DomainIdentity_ps_domain) 
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting up PDCP in transparent mode rb_id %d",local_rb_id);
    /*Get the next free index in pdcp_config_req */
    while (idx < MAX_RAB_TO_SETUP) 
    {
      if (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use == FALSE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"pdcp config req index is %d",idx);
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].rb_id = local_rb_id;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.action = SETUP_PDCP;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.is_mcast_call = FALSE;

        tdsordered_config_ptr->pdcp_parms.num_rbs++;
        break;
      }
      else
      {
        idx++;
      }
    }
    /* CPDCP action will be setup, but pdcp info will be null for transparent mode */
  }
#else
  /* Ignore the PDCP info for now */
  /********************************/

  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup,pdcp_Info)) &&
      ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
       (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info,headerCompressionInfoList)) ||
       ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info,losslessSRNS_RelocSupport)) &&
        (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)))))                
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }
#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

} /* end tdsrrcllc_get_rb_info_to_setup() */

/*==========================================================
FUNCTION  : get_rb_info_to_relase()

DESCRIPTION : Get Radio Bearer Information to release.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_to_release
(
  tdsrrc_RB_Identity *ie_ptr
)
{

  if(*ie_ptr < DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RB ID %d cannot be released",*ie_ptr);
    return FAILURE;
  }

  tdsrrcllc_unmap_rb(*ie_ptr, TDSOC_ONLY);

  TDSRRC_MSG1(MSG_LEGACY_MED,"Removed RB Mapping Info for RB%d",*ie_ptr);

  /* Now delete the AM Info for this RB (if this is an AM RB) 
  * in the RLC Info.
  */
  return SUCCESS;
} /* end tdsrrcllc_get_rb_info_to_release() */


/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_am_info()

DESCRIPTION : Deletes RLC AM info and re-arranges the RLC
              AM Info array to cover any holes.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_am_info(tdsrrc_RB_Identity rb_id)
{
  uint8  count;
  
  for(count = 0; count < TDSUE_MAX_AM_ENTITY; count ++)
  {
    if((tdsordered_config_ptr->rlc_am_parms.am_info[count]->common_config.rb_id 
        == rb_id) && 
       (tdsordered_config_ptr->rlc_am_parms.am_info[count]->dl_nchan &&
          tdsordered_config_ptr->rlc_am_parms.am_info[count]->ul_nchan)
      )
    {
      /* Reset this RLC AM entity memory and leave it there for the 
         next adding RLC AM entity.
       */
      memset(tdsordered_config_ptr->rlc_am_parms.am_info[count], 
             0,
             sizeof(tdsrlc_am_config_type)); 
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Release/reset RLC AM, RB%d,config%d",rb_id,count);
      /* This is acceptable since we are breaking immediately after */
      tdsordered_config_ptr->rlc_am_parms.nentity --;

      tdsrrcllc_init_am_dl_rlc_size(tdsordered_config_ptr, count);
      break;
    }
  }  /* for (...) */

  if (count == TDSUE_MAX_AM_ENTITY)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"can't locate rb_id %d",rb_id);
  }
}  /* tdsrrcllc_delete_rlc_am_info */

/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_dl_tm_info()

DESCRIPTION : Decrements the count of DL TM logical channels.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_dl_tm_info(rlc_lc_id_type lc_id)
{
  tdsordered_config_type *config_ptr;
  /* Use Current Config when in Solo BCH Operation */
  if(tdsrrcllc_is_cmd_under_process_for_solo_bch_operation())
  {
    config_ptr = tdscurrent_config_ptr;
  }
  else
  {
    config_ptr = tdsordered_config_ptr;
  }

  /* Just decrement the count now since other parameters are not stored/used */
  if (config_ptr->rlc_dl_tm_parms.nchan > 0)
  {
    config_ptr->rlc_dl_tm_parms.nchan --;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No DL TM channels to delete");
  }
} /*lint !e715 */

/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_dl_um_info()

DESCRIPTION : Deletes the UM RB from OC and decrements the count 
              of DL UM logical channels.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_dl_um_info(rlc_lc_id_type lc_id)
{
  uint8 count;
 
  for(count = 0; count < TDSUE_MAX_DL_UM_CHANNEL; count ++)
  {
    if((tdsordered_config_ptr->rlc_dl_um_parms.chan_info[count].lc_id == lc_id) && 
        (tdsordered_config_ptr->rlc_dl_um_parms.chan_info[count].rb_id != TDSRRCLLC_INVALID_RB_ID))
    {
      /* Reset this RLC DL UM entity memory and leave it there for the 
         next adding RLC DL UM entity.
       */
      memset(&tdsordered_config_ptr->rlc_dl_um_parms.chan_info[count], 
             0, sizeof(tdsrrc_rlc_dl_um_channel_config_type)); 
      tdsordered_config_ptr->rlc_dl_um_parms.nchan --;
      tdsordered_config_ptr->rlc_dl_um_parms.chan_info[count].rb_id = TDSRRCLLC_INVALID_RB_ID;

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Release/reset RLC DL UM for LC id %d,config%d",lc_id,count);
      break;
    }
  }  /* for (...) */

  if (count == TDSUE_MAX_DL_UM_CHANNEL)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"can't locate lc_id %d",lc_id);
  }
} /*lint !e715 */

/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_ul_tm_info()

DESCRIPTION : Decrements the count of UL TM logical channels.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_ul_tm_info(rlc_lc_id_type lc_id)
{
  /* Just decrement the count now since other parameters are not stored/used */
  if (tdsordered_config_ptr->rlc_ul_tm_parms.nchan > 0)
  {
    tdsordered_config_ptr->rlc_ul_tm_parms.nchan --;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"nchan: UL TM %d after delete",tdsordered_config_ptr->rlc_ul_tm_parms.nchan);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"No UL TM channels to delete");
  }
} /*lint !e715 */

/*==========================================================
FUNCTION  : tdsrrcllc_delete_rlc_ul_um_info()

DESCRIPTION : Deletes the UM RB and decrements the count of 
              UL UM logical channels.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: None.
============================================================*/
void  tdsrrcllc_delete_rlc_ul_um_info(rlc_lc_id_type lc_id)
{
  uint8 count;

  for(count = 0; count < TDSUE_MAX_UL_UM_CHANNEL; count ++)
  {
    if((tdsordered_config_ptr->rlc_ul_um_parms.chan_info[count].lc_id == lc_id) && 
        (tdsordered_config_ptr->rlc_ul_um_parms.chan_info[count].rb_id != TDSRRCLLC_INVALID_RB_ID))
    {
      /* Reset this RLC UL UM entity memory and leave it there for the 
         next adding RLC UL UM entity.
       */
      memset(&tdsordered_config_ptr->rlc_ul_um_parms.chan_info[count], 
             0, sizeof(tdsrrc_rlc_ul_um_channel_config_type)); 
      tdsordered_config_ptr->rlc_ul_um_parms.nchan --;
      tdsordered_config_ptr->rlc_ul_um_parms.chan_info[count].rb_id = TDSRRCLLC_INVALID_RB_ID;

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Release/reset RLC UL UM for LC id %d,config%d",lc_id,count);
      break;
    }
  }  /* for (...) */

  if (count == TDSUE_MAX_UL_UM_CHANNEL)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"can't locate lc_id %d",lc_id);
  }
} /*lint !e715 */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_affected()

DESCRIPTION : Get Radio Bearer Information to be affected.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_rb_info_affected
(
  tdsrrc_RB_InformationAffected *ie_ptr
)
{
  /* Remove any existing mapping for the RB */
  tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);

  /* Get the RB Mapping info */
  return (tdsrrcllc_get_rb_mapping_info(&ie_ptr->rb_MappingInfo, 
          (tdsrrc_RB_Identity)ie_ptr->rb_Identity, 
          UE_MODE_MAX_NUM));

} /* end tdsrrcllc_get_rb_info_affected() */

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
)
{
  /* Remove any existing mapping for the RB */
  tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);


  /* Get the RB Mapping info */
  return(tdsrrcllc_get_rb_mapping_info_r5(&ie_ptr->rb_MappingInfo, 
                                (tdsrrc_RB_Identity)ie_ptr->rb_Identity, 
                                UE_MODE_MAX_NUM));

} /* end tdsrrcllc_get_rb_info_affected_r5() */

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
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  
  /* Check to see if the RLC info is present */
  if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r5,rlc_Info))
     && ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info_r5,ul_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info_r5,dl_RLC_Mode))))
  {
    /* Get the RLC mode */
    if ((logch_mode = tdsrrcllc_get_rlc_mode_r5(&ie_ptr->rlc_Info)) == UE_MODE_MAX_NUM)
    {
      return FAILURE;
    }
    
    /* Save off the RLC Info */
    if (tdsrrcllc_get_rlc_info_r5(&ie_ptr->rlc_Info, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, FALSE) == FAILURE)
    {
      return FAILURE;
    }
  } /* end RLC Info Check */
  
  /* Save off the RB Mapping info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r5,rb_MappingInfo))
  {
    /* Remove any existing mapping for the RB */
    tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);
    
    if (tdsrrcllc_get_rb_mapping_info_r5(&ie_ptr->rb_MappingInfo, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, logch_mode)
      == FAILURE)
    {
      return FAILURE;
    }
  } /* end of RB Mapping info check */
  
#ifdef FEATURE_UMTS_PDCP
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r5,pdcp_Info))
  {
    if (tdsrrc_pdcp_enabled)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDPC info present in rb setup");
      if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,
                                      &ie_ptr->pdcp_Info.pdcp_Info, 
                                      (tdsrrc_RB_Identity)ie_ptr->rb_Identity, 
                                      TDSCPDCP_RECONFIG
                                      ) == FAILURE)
      {
        return FAILURE;
      }
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r5,pdcp_SN_Info)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r5,pdcp_Info)) &&
             ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
              (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
              ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported))))))          
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
      return FAILURE;
    }
  }
#else
  /* Ignore the PDCP info for now */
  /********************************/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r5,pdcp_SN_Info)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r5,pdcp_Info)) &&
             ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
              (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
              ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported))))))          
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }

#endif /* FEATURE_UMTS_PDCP */
  /* Check to see if the RB supports stop/continue */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r5,rb_StopContinue))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Stop/Continue IE specified");
  }
  
  /* All is well with this IE */
  return SUCCESS;

} /* end tdsrrcllc_get_rb_info_to_reconfig_r5() */

#ifdef FEATURE_TDSCDMA_HSUPA
/*==========================================================
FUNCTION  : tdsrrcllc_remove_e_ul_ded_logch_entry()

DESCRIPTION : Removes the specified entry in the array of
              tdsmac_e_ul_logch_info_s_type entries indicated
              by the supplied pointer.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: The specified entry in the provided array is
              removed, and the rest of the array is moved down
              to fill in the hole.
============================================================*/
static void tdsrrcllc_remove_e_ul_ded_logch_entry
(
  tdsmac_e_ul_logch_info_s_type *e_ul_ded_logch_array,
  uint8                         entry_to_remove
)
{
  if(entry_to_remove == MAX_DED_LOGCHAN_PER_TRCH - 1)
  {
    memset(e_ul_ded_logch_array + entry_to_remove, 0,
           sizeof(tdsmac_e_ul_logch_info_s_type));
  }
  else
  {
    memsmove(e_ul_ded_logch_array + entry_to_remove,
	     sizeof(tdsmac_e_ul_logch_info_s_type) * (MAX_DED_LOGCHAN_PER_TRCH - entry_to_remove - 1),
           e_ul_ded_logch_array + entry_to_remove + 1,
	     sizeof(tdsmac_e_ul_logch_info_s_type) * (MAX_DED_LOGCHAN_PER_TRCH - entry_to_remove - 1));
    
    memset(e_ul_ded_logch_array + (MAX_DED_LOGCHAN_PER_TRCH - 1), 0,
           sizeof(tdsmac_e_ul_logch_info_s_type));
  }
}

#endif /* FEATURE_TDSCDMA_HSUPA */ 
/*==========================================================
FUNCTION  : tdsrrcllc_unmap_rb()

DESCRIPTION : Remove the mapping for the specified RB if it
              exists.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: If a mapping for the specified RB is found in
              tdsordered_config, it is removed and the
              ded_logch_config array it was in is rearranged
              to fill the hole. This function also indicates
              if MAC Uplink or Downlink needs to be reconfigured.
============================================================*/
void tdsrrcllc_unmap_rb
(
  tdsrrc_RB_Identity rb_id,
  tdsrrcllc_unmap_rb_e_type config_db
)
{
  uint8 trch_idx;
  uint8 logch_idx;
  uint8 ndlchan;
  uint8 pre_ndlchan = 0;

  uint32 iterations = 1;

  uint8 dflow_index = 0;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
uint8 mac_ehs_queue_index =0;
#endif
  tdsordered_config_type   *tmp_config_ptr = NULL;

  /* start with OC */
  tmp_config_ptr = tdsordered_config_ptr;

  if ((config_db == TDSOC_AND_TOC) && (tdstransition_config.toc_usage != TDSTOC_INVALID))
  {
    /*In case of Cell_FACH -> Cell_DCH state transition, if TOC is valid, 
    don't remove RB-mapping from TOC.*/
    if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && 
      (tdsrrc_ordered_state == TDSRRC_STATE_CELL_DCH))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Clean only OC for RB-Mapping for RB %d",rb_id);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Need to clean both OC & TOC");
      iterations = 2;
    }
  }
  else if (config_db == TDSTOC_ONLY)
  {
    if (tdstransition_config.toc_usage != TDSTOC_INVALID)
    {
      tmp_config_ptr = tdstransition_config.toc_ptr;
    }
    else
    {
      return;
    }
  }
     
  if( config_db == TDSOC_ONLY || config_db == TDSOC_AND_TOC)
  {
    tdsrrcllc_free_hanging_rb_mapping_info_index(rb_id,TDSALL_RB_MAPPING);
  }

  while (iterations)
  {
    /* fach_info, dl_dch_info, dsch_info, and ul_cctrch.dch_info.dch_info are all
       arrays of size TDSUE_MAX_TRCH, so we will iterate through them all with this
       outer loop. */
    for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
    {
      /* DL FACH */
      ndlchan = tmp_config_ptr->fach_info[trch_idx].ndlchan;
      for(logch_idx = 0; logch_idx < ndlchan; logch_idx++)
      {
        if(tmp_config_ptr->fach_info[trch_idx].dlchan_info[logch_idx].rb_id == rb_id)
        {
          tdsrrcllc_remove_dl_ded_logch_entry(tmp_config_ptr->fach_info[trch_idx].dlchan_info,
                                    logch_idx);
          tmp_config_ptr->fach_info[trch_idx].ndlchan--;
          tmp_config_ptr->dl_fach_trch_info[trch_idx].lg_ch_cnt = 
                              tmp_config_ptr->fach_info[trch_idx].ndlchan;

          /* Here indicate that MAC Downlink need to be reconfigured */
          tmp_config_ptr->reconfig_needed.downlink_mac = TRUE;
        }
        /* No break here - all RBs are mapped to all available FACHes. */
      }

      /* DL DCH */
      ndlchan = tmp_config_ptr->dl_dch_info[trch_idx].ndlchan;
      for(logch_idx = 0; logch_idx < ndlchan; logch_idx++)
      {
        if(tmp_config_ptr->dl_dch_info[trch_idx].dlchan_info[logch_idx].rb_id == rb_id)
        {
          tdsrrcllc_remove_dl_ded_logch_entry(tmp_config_ptr->dl_dch_info[trch_idx].dlchan_info,
                                    logch_idx);
          pre_ndlchan = tmp_config_ptr->dl_dch_info[trch_idx].ndlchan;
          tmp_config_ptr->dl_dch_info[trch_idx].ndlchan--;
          tmp_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt = 
                              tmp_config_ptr->dl_dch_info[trch_idx].ndlchan;

          /* When the number of logical channel mapped to this transport channel 
             going down from 2/multiple to 1, we need to update the tb_size due 
             to the change of MAC header. */
          if (pre_ndlchan > 1 && tmp_config_ptr->dl_dch_info[trch_idx].ndlchan == 1)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"TrCH %d Need to reduce TB size",tdsordered_config_ptr->dl_dch_info[trch_idx].trch_id);
            tmp_config_ptr->dl_dch_trch_info[trch_idx].lg_ch_cnt_down = TRUE;
          }
          /* Here indicate that MAC Downlink needs to be reconfigured */
          tmp_config_ptr->reconfig_needed.downlink_mac = TRUE;
          break;
        }
      }

      if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
      {      
        for (dflow_index = 0; dflow_index < UE_MAX_MAC_D_FLOW; dflow_index++)
        {
          /* DL HSDSCH */
          ndlchan = tmp_config_ptr->mac_dflow_info[dflow_index].ndlchan;
          for (logch_idx = 0; logch_idx < ndlchan; logch_idx++)
          {
            if (tmp_config_ptr->mac_dflow_info[dflow_index].dlchan_info[logch_idx].rb_id == rb_id)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Delete mapping on DFLOW Id %d",tmp_config_ptr->mac_dflow_info[dflow_index].mac_dflow_id);
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCHS:Deleting RB %d,L.ChIndex %d,RLCID %d", rb_id,dflow_index,logch_idx);
              
              tdsrrcllc_remove_dl_ded_logch_entry(tmp_config_ptr->mac_dflow_info[dflow_index].dlchan_info,
                                        logch_idx);
              
              pre_ndlchan = tmp_config_ptr->mac_dflow_info[dflow_index].ndlchan;
              
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Current no of Ded Log Chs %d",tmp_config_ptr->mac_dflow_info[dflow_index].ndlchan);
              
              if (pre_ndlchan == 0)
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:No of HS channels already 0");
              } 
              else
              {
                tmp_config_ptr->mac_dflow_info[dflow_index].ndlchan--;
              }
              
              /* Here indicate that MAC Downlink needs to be reconfigured */
              tmp_config_ptr->reconfig_needed.downlink_mac = TRUE;
              break;
            }
          }
        }
      }

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS

      for (mac_ehs_queue_index = 0; mac_ehs_queue_index < UE_MAX_DL_LOGICAL_CHANNEL; mac_ehs_queue_index++)
      {
  
        if (tmp_config_ptr->dl_macehs_logchan_info[mac_ehs_queue_index].rb_id == rb_id)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"MACEHS:Delete mapping on Queue Id %d",tmp_config_ptr->dl_macehs_logchan_info[mac_ehs_queue_index].queue_id);
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"MACEHS:Deleting RB %d,Queue index %d,LC ID %d", rb_id,mac_ehs_queue_index,tmp_config_ptr->dl_macehs_logchan_info[mac_ehs_queue_index].mac_id);
  
          tmp_config_ptr->dl_macehs_logchan_info[mac_ehs_queue_index].rb_id = TDSRRC_INVALID_RB_ID;
          tmp_config_ptr->dl_macehs_logchan_info[mac_ehs_queue_index].rlc_id = 0;
          tmp_config_ptr->dl_macehs_logchan_info[mac_ehs_queue_index].queue_id = TDSINVALID_QUEUE_ID;
          tmp_config_ptr->dl_macehs_logchan_info[mac_ehs_queue_index].mac_id = TDSINVALID_LOGICAL_CHANNEL_ID;
          tmp_config_ptr->dl_macehs_logchan_info[mac_ehs_queue_index].chan_type = UE_LOGCHAN_NONE;
          tmp_config_ptr->dl_macehs_logchan_info[mac_ehs_queue_index].rlc_mode = UE_MODE_MAX_NUM;
  
          tmp_config_ptr->mac_dl_parms.ndlchan_macehs --;
  
          tmp_config_ptr->mac_ehs_lc_id_in_use[mac_ehs_queue_index] = FALSE;
          
          /* Here indicate that MAC Downlink needs to be reconfigured */
          tmp_config_ptr->reconfig_needed.downlink_mac = TRUE;
          break;
        }
      }


#endif


      /* DL DSCH  */
      ndlchan = tmp_config_ptr->dsch_info[trch_idx].ndlchan;
      for(logch_idx = 0; logch_idx <  ndlchan; logch_idx++)
      {
        if(tmp_config_ptr->dsch_info[trch_idx].dlchan_info[logch_idx].rb_id == rb_id)
        {
          tdsrrcllc_remove_dl_ded_logch_entry(tmp_config_ptr->dsch_info[trch_idx].dlchan_info,
                                    logch_idx);
          tmp_config_ptr->dsch_info[trch_idx].ndlchan--;
          /* Here indicate that MAC Downlink needs to be reconfigured */
          tmp_config_ptr->reconfig_needed.downlink_mac = TRUE;

          break;
        }
      }

      /* UL DCH */
      ndlchan = tmp_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan;
      for(logch_idx = 0; logch_idx < ndlchan; logch_idx++)
      {
        if(tmp_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rb_id == rb_id)
        {
          tdsrrcllc_remove_ul_ded_logch_entry(tmp_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info,
                                    logch_idx);

          pre_ndlchan = tmp_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan;
          tmp_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan--;
          tmp_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt = 
                              tmp_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan;

          /* When the number of logical channel mapped to this transport channel 
             going down from 2/multiple to 1, we need to update the tb_size due 
             to the change of MAC header. */
          if (pre_ndlchan > 1 && tmp_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan == 1)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"TrCH %d Need to reduce TB size",tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].trch_id);
            tmp_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt_down = TRUE;
          }
          /* Here indicate that MAC Uplink needs to be reconfigured */
          tmp_config_ptr->reconfig_needed.uplink_mac = TRUE;
          break;
        }
      }
    }

#ifdef FEATURE_TDSCDMA_HSUPA
    for (dflow_index = 0; dflow_index < TDSMAX_E_DCH_MAC_D_FLOW; dflow_index++)
    {
      ndlchan = tmp_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_index].num_logch;
      for(logch_idx = 0; logch_idx < ndlchan; logch_idx++)
      {
        if(tmp_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_index].e_ul_logch_info[logch_idx].rb_id == rb_id)
        {
          tdsrrcllc_remove_e_ul_ded_logch_entry(tmp_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_index].
                                      e_ul_logch_info, logch_idx);
          tmp_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_index].num_logch--;
          /* Here indicate that MAC Uplink needs to be reconfigured */
          tmp_config_ptr->reconfig_needed.uplink_mac = TRUE;
          
          break;
        }
      }
    }
#endif /* FEATURE_TDSCDMA_HSUPA */


    /* UL RACH  */
    /* ul_cctrch.rach_info, on the other had, is just a single structure, so we
       take care of it outside the loop above. */
    ndlchan = tmp_config_ptr->ul_cctrch.rach_info.ndlchan;
    for(logch_idx = 0; logch_idx < ndlchan; logch_idx++)
    {
      if(tmp_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx].rb_id == rb_id)
      {
        tdsrrcllc_remove_ul_ded_logch_entry(tmp_config_ptr->ul_cctrch.rach_info.dlchan_info,
                                  logch_idx);
        tmp_config_ptr->ul_cctrch.rach_info.ndlchan--;
        for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
        {
          if(tmp_config_ptr->ul_rach_trch_info[trch_idx].trch_id ==
              tmp_config_ptr->ul_cctrch.rach_info.trch_id)
          {
            tmp_config_ptr->ul_rach_trch_info[trch_idx].lg_ch_cnt = 
                               tmp_config_ptr->ul_cctrch.rach_info.ndlchan;
          }
        }
        /* Here indicate that MAC Uplink needs to be reconfigured */
        tmp_config_ptr->reconfig_needed.uplink_mac = TRUE;

        break;
      }
    }

    /* Delete the RB Mapping info for the RB as stored in Cell FACH RB Mapping Info */
    /* Note: This info is stored for later use when transition to CELL_FACH is made.
     */
    /* Remove the Downlink Dedicated Logical Channel Info */
    ndlchan = tmp_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan;
    for(logch_idx = 0; logch_idx <  ndlchan; logch_idx++)
    {
      if(tmp_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[logch_idx].rb_id == rb_id)
      {
        tdsrrcllc_remove_dl_ded_logch_entry(tmp_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info,
                                  logch_idx);
        tmp_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan --;
        break;
      }
    }

    /* Remove the Uplink Dedicated Logical Channel Info */
    ndlchan = tmp_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan;
    {
      for(logch_idx = 0; logch_idx <  ndlchan; logch_idx++)
      {
          if(tmp_config_ptr->rach_rlc_size_restriction_info.rlc_size_list[logch_idx].rb_id == rb_id)
        {
          tdsrrcllc_remove_rlc_size_list_entry(tmp_config_ptr->rach_rlc_size_restriction_info.rlc_size_list,
                                     logch_idx);
          break;
        }
      }
    }

    for(logch_idx = 0; logch_idx <  ndlchan; logch_idx++)
    {
      if(tmp_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[logch_idx].rb_id == rb_id)
      {
        tdsrrcllc_remove_ul_ded_logch_entry(tmp_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info,
                                  logch_idx);
        tmp_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan --;
        break;
      }
    }
    iterations --;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    tdsrrcllc_rearrange_macehs_lc_index(tmp_config_ptr);
#endif
    /* in the next iteration clean the transition config database also */
    tmp_config_ptr = tdstransition_config.toc_ptr;
  } /*while iterations */


} /* tdsrrcllc_unmap_rb */

/*==========================================================
FUNCTION  : tdsrrcllc_remove_dl_ded_logch_entry()

DESCRIPTION : Removes the specified entry in the array of
              tdsmac_dl_ded_logch_config_type entries indicated
              by the supplied pointer.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: The specified entry in the provided array is
              removed, and the rest of the array is moved down
              to fill in the hole.
============================================================*/
void tdsrrcllc_remove_dl_ded_logch_entry
(
  tdsmac_dl_ded_logch_config_type *ded_logch_array,
  uint8                         entry_to_remove
)
{
  if(entry_to_remove == MAX_DED_LOGCHAN_PER_TRCH - 1)
  {
    memset(ded_logch_array + entry_to_remove, 0,
           sizeof(tdsmac_dl_ded_logch_config_type));
  }
  else
  {
    memsmove(ded_logch_array + entry_to_remove,
	     sizeof(tdsmac_dl_ded_logch_config_type) * (MAX_DED_LOGCHAN_PER_TRCH - entry_to_remove - 1),
           ded_logch_array + entry_to_remove + 1,
	     sizeof(tdsmac_dl_ded_logch_config_type) * (MAX_DED_LOGCHAN_PER_TRCH - entry_to_remove - 1));
    
    memset(ded_logch_array + (MAX_DED_LOGCHAN_PER_TRCH - 1), 0,
           sizeof(tdsmac_dl_ded_logch_config_type));
  }
}

/*==========================================================
FUNCTION  : tdsrrcllc_remove_ul_ded_logch_entry()

DESCRIPTION : Removes the specified entry in the array of
              tdsmac_ul_ded_logch_config_type entries indicated
              by the supplied pointer.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: The specified entry in the provided array is
              removed, and the rest of the array is moved down
              to fill in the hole.
============================================================*/
void tdsrrcllc_remove_ul_ded_logch_entry
(
  tdsmac_ul_ded_logch_config_type *ded_logch_array,
  uint8                         entry_to_remove
)
{
  if(entry_to_remove == MAX_DED_LOGCHAN_PER_TRCH - 1)
  {
    memset(ded_logch_array + entry_to_remove, 0,
           sizeof(tdsmac_ul_ded_logch_config_type));
  }
  else
  {
    memsmove(ded_logch_array + entry_to_remove,
	     sizeof(tdsmac_ul_ded_logch_config_type) * (MAX_DED_LOGCHAN_PER_TRCH - entry_to_remove - 1),
           ded_logch_array + entry_to_remove + 1,
	     sizeof(tdsmac_ul_ded_logch_config_type) * (MAX_DED_LOGCHAN_PER_TRCH - entry_to_remove - 1));
    
    memset(ded_logch_array + (MAX_DED_LOGCHAN_PER_TRCH - 1), 0,
           sizeof(tdsmac_ul_ded_logch_config_type));
  }
}


/*==========================================================
FUNCTION  : tdsrrcllc_remove_rlc_size_list_entry()

DESCRIPTION : Removes the specified entry in the array of
              tdsrrcllcoc_rlc_size_list_type entries indicated
              by the supplied pointer.

DEPENDENCIES: None.

RETURN VALUE: None.

SIDE EFFECTS: The specified entry in the provided array is
              removed, and the rest of the array is moved down
              to fill in the hole.
============================================================*/
void tdsrrcllc_remove_rlc_size_list_entry
(
  tdsrrcllcoc_rlc_size_list_type *rlc_size_list_array,
  uint8                        entry_to_remove
)
{
  if(entry_to_remove == MAX_DED_LOGCHAN_PER_TRCH - 1)
  {
    tdsrrcllc_reset_rach_rlc_size_lists(rlc_size_list_array, entry_to_remove, FALSE);
  }
  else
  {
    memsmove(rlc_size_list_array + entry_to_remove,
	     sizeof(tdsrrcllcoc_rlc_size_list_type) * (MAX_DED_LOGCHAN_PER_TRCH - entry_to_remove - 1),
           rlc_size_list_array + entry_to_remove + 1,
	     sizeof(tdsrrcllcoc_rlc_size_list_type) * (MAX_DED_LOGCHAN_PER_TRCH - entry_to_remove - 1));
  }
}

/*==========================================================
FUNCTION  : tdsrrcllc_check_if_rbid_present_in_chan_config_within_index()

DESCRIPTION : This function checks if the RB-Id specified 
              is already present in Chan_config within the index specified.

DEPENDENCIES: None.

RETURN VALUE: 
uint8:   It returns the index at which 
         the RB-Id is present in Chan Config else returns 
         TDS_MAX_RB.

SIDE EFFECTS: None
============================================================*/
uint8 tdsrrcllc_check_if_rbid_present_in_chan_config_within_index
(
  tdsrrc_channel_config_req_type * chan_config_req, 
  tdsrrc_RB_Identity rb_id,
  uint8 count
)
{
  uint8 j;
   
  for(j=0;j<count;j++)
  {
    if(chan_config_req->rb[j].rb_id == rb_id )
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb_id %d found in chan config at index %d",rb_id,j);
      return j;
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"rb_id %d not found  in chan config ",rb_id);

  return TDS_MAX_RB;
}

/*==========================================================
FUNCTION  : tdsrrcllc_check_if_rbid_present_in_chan_config()

DESCRIPTION : This function checks if the RB-Id specified 
              is already present in Chan_config.

DEPENDENCIES: None.

RETURN VALUE: 
boolean: returns TRUE if Rb Id is presrn in  Channel 
         Config Req.It also returns the index at which 
         the RB-Id is present in Chan Config else returns 
         FALSE.

SIDE EFFECTS: None
============================================================*/
uint8 tdsrrcllc_check_if_rbid_present_in_chan_config
(
  tdsrrc_channel_config_req_type * chan_config_req, 
  tdsrrc_RB_Identity rb_id 
)
{
  uint8 j;
   
  for(j=0;j<chan_config_req->num_rb_to_config;j++)
  {
    if(chan_config_req->rb[j].rb_id == rb_id )
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb_id %d found in chan config at index %d",rb_id,j);
      return j;
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"rb_id %d not found  in chan config ",rb_id);
  return TDS_MAX_RB;
}

/*==========================================================
FUNCTION  : tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current()

DESCRIPTION : copies the tdsrrc_ordered_hanging_rb_mapping to tdsrrc_current_hanging_rb_mapping
DEPENDENCIES: None.

RETURN VALUE: None                        

SIDE EFFECTS: None
============================================================*/
void tdsrrcllc_copy_ordered_hanging_rb_mapping_to_current(void)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"copied Standalone OC Database  to Standalone CC Database ");
  memscpy(tdsrrc_current_hanging_rb_mapping, 
	  (sizeof(tdsrb_mapping_info_type)*TDSMAX_HANGING_RB_MAPPING_INFO),
	  tdsrrc_ordered_hanging_rb_mapping,
                            (sizeof(tdsrb_mapping_info_type)*TDSMAX_HANGING_RB_MAPPING_INFO));
}

/*==========================================================
FUNCTION  : tdsrrcllc_init_ordered_hanging_rb_mapping_info()

DESCRIPTION : initialise the tdsrrc_ordered_hanging_rb_mapping info
DEPENDENCIES: None.

RETURN VALUE: None                        

SIDE EFFECTS: None
============================================================*/
void tdsrrcllc_init_ordered_hanging_rb_mapping_info(void)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Init  Standalone OC Database ");
  memset(tdsrrc_ordered_hanging_rb_mapping,0,
              (sizeof(tdsrb_mapping_info_type)*TDSMAX_HANGING_RB_MAPPING_INFO));
}

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
)
{
  uint8 i;
  for(i=0; i < TDSMAX_HANGING_RB_MAPPING_INFO; i++)
  {
    if(tdsrrc_ordered_hanging_rb_mapping[i].in_use == FALSE)
    {
      *index = i;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Allocating index %d in  Standalone OC Database",i);
      tdsrrc_ordered_hanging_rb_mapping[i].in_use = TRUE;
      return SUCCESS;
    }
  }
  return FAILURE;
}


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
)
{
  uint8 i;

  for(i=0; i < TDSMAX_HANGING_RB_MAPPING_INFO; i++)
  {
    if((tdsrrc_ordered_hanging_rb_mapping[i].in_use == TRUE)
        && (tdsrrc_ordered_hanging_rb_mapping[i].rb_id == rb_id)) 
    { 
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"rb_id %d is removed  from Standalone OC Database  at index %d for chan type %d",
                          rb_id,i,chan_type);
      if( chan_type == TDSALL_RB_MAPPING)
      {
        memset(&tdsrrc_ordered_hanging_rb_mapping[i],0,sizeof(tdsrb_mapping_info_type));
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb_id %d is removed  from Standalone OC Database  at index %d ",rb_id,i);
        tdsrrc_ordered_hanging_rb_mapping[i].in_use = FALSE;
        return;
      }
      else
      {
        if( chan_type == TDSUL_DCH_RB_MAPPING )
        {
          if(tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_dch_info.valid == TRUE )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"UL DCH Mapping  deleted from Standalone OC Database at index%d for rb_id %d ",i,tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
            memset(&tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_dch_info,
                           0,sizeof(tdsul_rb_dch_mapping_info_type));
            tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_dch_info.valid = FALSE;
          }
        }
        else if( chan_type == TDSDL_DCH_RB_MAPPING )
        {
          if(tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.dch_map_info.valid == TRUE )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"DL DCH Mapping deleted from Standalone OC Database at index%d for rb_id %d ",i,tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
            memset(&tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.dch_map_info,
                          0,sizeof(tdsdl_rb_dch_mapping_info_type));
            tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.dch_map_info.valid = FALSE;
          }
        }
   #ifdef FEATURE_TDSCDMA_HSUPA
        else  if( chan_type == TDSE_DCH_RB_MAPPING )  
        {
          if(tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.valid == TRUE )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"MAC_E Mapping deleted from Standalone OC Database at index%d for rb_id %d ",i,tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
            memset(&tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace,
                          0,sizeof(tdsul_rb_mac_e_mapping_info_type));
            tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.valid= FALSE;
          }
        }
   #endif
        else if( chan_type == TDSMAC_D_RB_MAPPING )  
        {
          if(tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.macd_map_info.valid == TRUE )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"MAC_D Mapping  deleted from Standalone OC Database at index%d for rb_id %d ",i,tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
            memset(&tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.macd_map_info,
                           0,sizeof(tdsdl_rb_macd_mapping_info_type));
            tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.macd_map_info.valid = FALSE;
          }
        }
        /* 
          * If UL DCH, DL DCH, E-DCH, MAC-D  mappings are deleted for a RB, 
          * we can free the RB from the Stadalone Database
          */
        if((tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_dch_info.valid == FALSE )
             && (tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.dch_map_info.valid == FALSE )
             &&( tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.macd_map_info.valid == FALSE)
   #ifdef FEATURE_TDSCDMA_HSUPA
             &&( tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.valid == FALSE)
   #endif
          )
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"RB id from Standalone OC Database ,and freeing  index %d  ",tdsrrc_ordered_hanging_rb_mapping[i].rb_id,i);
          memset(&tdsrrc_ordered_hanging_rb_mapping[i],0,sizeof(tdsrb_mapping_info_type));
           tdsrrc_ordered_hanging_rb_mapping[i].in_use = FALSE;
        }
      }      
    }
  }
}

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
)
{
  uint8 i;
  for(i=0;i<TDSMAX_HANGING_RB_MAPPING_INFO;i++)
  {
    if((tdsrrc_ordered_hanging_rb_mapping[i].in_use == TRUE) &&
        (tdsrrc_ordered_hanging_rb_mapping[i].rb_id == rb_id ))
    {
      *index = i;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"rb_id %d exists in Standalone OC Database at index %d",rb_id,i);
      return TRUE;
    }
  }
  return FALSE;
}

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
  uint8 index,
  tdsrb_mapping_chan_enum_type chan_type 
)
{
  uint8 idx;
  uint8 i;
  #ifdef FEATURE_TDSCDMA_HSUPA
  uint8 j;
  #endif
   
  if( chan_type == TDSUL_DCH_RB_MAPPING )
  {
    /* At present we consider deletion of RB, 
      * so assumption that only one RB is mapped to this Transport channel
      */
    for(i=0;i<tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].ndlchan;i++ )
    {
      if( tdsrrcllc_check_rb_id_exist_rb_mapping(
         tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[i].rb_id,
         &idx)   == FALSE )
      {
        if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for UL DCH ");
          return;
        }
      }
      tdsrrc_ordered_hanging_rb_mapping[idx].rb_id =
         tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[i].rb_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode =
         tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[i].rlc_mode;
   
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.valid = TRUE;
      /* Storing the UL DCH mapping info to Standalone database */
        
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.tr_id = 
         tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].trch_id;
   
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.log_channel_id =
         tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[i].mac_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.mac_log_priority =
          tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[i].priority;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Backup UL TDSDCH data for RB_id %d at  index %d for tdstrch_id %d ",
                       tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,idx,
                       tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.tr_id);
      
    }
  }
  else  if( chan_type == TDSDL_DCH_RB_MAPPING )
  {
      /* At present we consider deletion of RB, 
        * so assumption that only one RB is mapped to this Transport channel
        */
    for(i=0;i<tdsordered_config_ptr->dl_dch_info[index].ndlchan;i++ )
    {
      if( tdsrrcllc_check_rb_id_exist_rb_mapping(
           tdsordered_config_ptr->dl_dch_info[index].dlchan_info[i].rb_id, &idx) == FALSE )
      {
        if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for DL DCH ");
          return;
        }
      }
      tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.dch_map_info.valid = TRUE;
      /* Storing the DL DCH mapping info to Standalone database */   
      tdsrrc_ordered_hanging_rb_mapping[idx].rb_id =
        tdsordered_config_ptr->dl_dch_info[index].dlchan_info[i].rb_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.dch_map_info.tr_id = 
        tdsordered_config_ptr->dl_dch_info[index].trch_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode =
        tdsordered_config_ptr->dl_dch_info[index].dlchan_info[i].rlc_mode;
      tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.dch_map_info.log_channel_id =
        tdsordered_config_ptr->dl_dch_info[index].dlchan_info[i].mac_id;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Backup DL TDSDCH data for RB_id %d at  index %d for DL tdstrch_id %d ",
                        tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,idx,
                       tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.dch_map_info.tr_id);
    }
  }
  else if( chan_type == TDSRACH_RB_MAPPING )
  {
    for(i=0;i<tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan;i++ )
    {
      if( tdsrrcllc_check_rb_id_exist_rb_mapping(
          tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[i].rb_id, 
           &idx) == FALSE )
      {
        if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for RACH");
          return;
        }
      }
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.rach_info.valid = TRUE;
      /* Storing the RACH mapping info to Standalone database */   
      tdsrrc_ordered_hanging_rb_mapping[idx].rb_id =
             tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[i].rb_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.rach_info.tr_id= 0;
  
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.rach_info.log_channel_id =
             tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[i].mac_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.rach_info.mac_log_priority=
             tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[i].priority;
  
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.rach_info.size_rest_info =
             tdsordered_config_ptr->rach_rlc_size_restriction_info.rlc_size_list[i];
  
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Backup RACH data for RB_id %d at  index %d for tdstrch_id %d ",
                         tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,idx,
                         tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.rach_info.tr_id);
  
    }
  }
  else if( chan_type == TDSFACH_RB_MAPPING )
  {
    for(i=0;i<tdsordered_config_ptr->fach_info[index].ndlchan;i++ )
    {
      if( tdsrrcllc_check_rb_id_exist_rb_mapping(
           tdsordered_config_ptr->dl_dch_info[index].dlchan_info[i].rb_id, 
           &idx) == FALSE )
      {
        if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for FACH ");
          return;
        }
      }
      tdsrrc_ordered_hanging_rb_mapping[idx].rb_id =
          tdsordered_config_ptr->fach_info[index].dlchan_info[i].rb_id;
     
      tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.fach_map_info.valid = TRUE;
      /* Storing the FACH mapping info to Standalone database */      
         /* FACH doesnt ve a Transport Channel ID in the OTA msg */
      tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.fach_map_info.fach_id= 0;
    
      tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.fach_map_info.log_channel_id =
               tdsordered_config_ptr->fach_info[index].dlchan_info[i].mac_id;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Backup FACH data for RB_id %d at  index %d for tdstrch_id %d ",
                        tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,idx,
                       tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.fach_map_info.fach_id);
    }
  }
#ifdef FEATURE_TDSCDMA_HSUPA
  else  if(( chan_type == TDSE_DCH_RB_MAPPING )&& (index < TDSMAX_E_DCH_MAC_D_FLOW ))
  {
    for(i=0;i<tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].num_logch ;i++ )
    {
      if( tdsrrcllc_check_rb_id_exist_rb_mapping(
         tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[i].rb_id ,
         &idx) == FALSE )
      {
        if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for E- DCH ");
          return;
        }
      }
      tdsrrc_ordered_hanging_rb_mapping[idx].rb_id = 
             tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[i].rb_id ;
      tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode= 
             tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[i].rlc_mode;
      
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.valid = TRUE;
          /* Storing the E-DCH mapping info to Standalone database */   
     
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.mac_e_flow_id = 
         tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_mac_d_flow_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.mac_log_priority =
         tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[i].priority;
         
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.log_channel_id=
       tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[i].lc_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.in_sched_info =
         tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[i].include_in_scheduling_info;
      tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.num_rlc_size_ddi_info =
         tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[i].num_rlc_size_ddi_info;
      for(j=0;j<tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.num_rlc_size_ddi_info;j++)
      {
        tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.rlc_size_ddi_info[j] =
            tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[i].rlc_size_ddi_info[j];
      }
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Backup MAC_E data for RB_id %d at  index %d for flow id %d ",
                        tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,idx,
                       tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.mac_e_flow_id); 
    }
  }
#endif/*FEATURE_TDSCDMA_HSUPA*/
  else  if( (chan_type == TDSMAC_D_RB_MAPPING )&& (index < UE_MAX_MAC_D_FLOW ))
  {
    for(i=0;i<tdsordered_config_ptr->mac_dflow_info[index].ndlchan ;i++ )
    {
      if( tdsrrcllc_check_rb_id_exist_rb_mapping(
           tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[i].rb_id,
           &idx) == FALSE )
      {
        if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for MAC-D ");
          return;
        }
      }
      tdsrrc_ordered_hanging_rb_mapping[idx].rb_id = 
            tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[i].rb_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode= 
            tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[i].rlc_mode;
    
      tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.macd_map_info.valid = TRUE;
      /* Storing theMAC-D mapping info to Standalone database */  
        
      tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.macd_map_info.mac_d_flow_id =
        tdsordered_config_ptr->mac_dflow_info[index].mac_dflow_id;
      tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.macd_map_info.log_channel_id =
        tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[i].mac_id;
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Backup MAC_Ddata for RB_id %d at  index %d for flow id %d ",
                        tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,idx,
                        tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.macd_map_info.mac_d_flow_id);
    }
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid info receive for Backup,unkown chan type %d index %d ",chan_type,index);
  }
}

/*==========================================================
FUNCTION  : tdsrrcllc_restore_ul_dch_rb_mapping_info()

DESCRIPTION : This function restores the Rb_mapping info from
              the RB-mapping backup info to the 
              Ordered config for UL DCh.

DEPENDENCIES: None.

RETURN VALUE: FAILURE: if the Rb-mapping info in the Rb-Mapping 
              backup info is invalid
              else 
              return SUCCESS

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type tdsrrcllc_restore_ul_dch_rb_mapping_info
(
  uint8  index 
)
{
  uint8 i,logch_idx;
  tr_ch_id_type tdstrch_id;
  uecomdef_logch_mode_e_type logch_mode;
  rlc_lc_id_type rlc_lc_id;
  uecomdef_status_e_type ret_val = SUCCESS;;
 
  tdstrch_id = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].trch_id;
  
  for(i=0;i<TDSMAX_HANGING_RB_MAPPING_INFO;i++)
  {
    if((tdsrrc_ordered_hanging_rb_mapping[i].in_use == TRUE ) && 
       (tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_dch_info.valid == TRUE ))
    {
      if(tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_dch_info.tr_id == tdstrch_id )
      {
       
        logch_idx = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].ndlchan;
        tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[logch_idx].rb_id = 
            tdsrrc_ordered_hanging_rb_mapping[i].rb_id;
        tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[logch_idx].mac_id=
           tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_dch_info.log_channel_id;
        tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[logch_idx].priority =
          tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_dch_info.mac_log_priority;
        /* Initialize the MAC data structures to allow all RLC sizes, just in case. */
        tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[logch_idx].rlc_size_info.all_tfi_valid = TRUE;


  /* Determine the type of Logical Channel mapped to DCH TrCH */
        if(tdsrrc_ordered_hanging_rb_mapping[i].rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
        }
        else
        {
          tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
        }
        
        
        rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
        if((rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND) && 
            (tdsrrc_ordered_hanging_rb_mapping[i].logch_mode == UE_MODE_MAX_NUM ))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid UL LC ID for RB %d",tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
          ret_val= FAILURE;
        }
        
        if(tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
        {  
          if(tdsrrc_get_am_index(tdsrrc_ordered_hanging_rb_mapping[i].rb_id) != TDSUE_MAX_AM_ENTITY)
          {
            logch_mode = UE_MODE_ACKNOWLEDGED_DATA;            
          }
          else if(tdsrrcllc_get_ul_um_index((tdsrrc_RB_Identity)tdsrrc_ordered_hanging_rb_mapping[i].rb_id )!= 
            TDSUE_MAX_UL_UM_CHANNEL)         
          {
            logch_mode = UE_MODE_UNACKNOWLEDGED;
          }
          else if(tdsrrcllc_get_ul_tm_index(rlc_lc_id)!= UE_MAX_UL_TM_CHANNEL)
          {
            logch_mode = UE_MODE_TRANSPARENT;
          }
          else
          {
            if(tdsrrc_ordered_hanging_rb_mapping[i].logch_mode == UE_MODE_MAX_NUM )
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get UL Log Chl mode for LC %d",rlc_lc_id);
              ret_val =  FAILURE;
            }
          }
        }
  
        /* Also update the Logical Channel ID here */
        tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[logch_idx].rlc_id = rlc_lc_id;
       
        /* Save the logical channel mode, if a setup */
        tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].dlchan_info[logch_idx].rlc_mode = logch_mode;
       
        /* other wise it is just a reconfig of RB Mapping info */
       
        /* Increment number of logical channels */
        tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].ndlchan++;

        /* The logic channel mapped to this TrCH increases from 1 to 2. */
        /* Need to update the TB size to reflect this change.           */
        if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].ndlchan == 2 &&
          tdsordered_config_ptr->ul_dch_trch_info[index].lg_ch_cnt == 1)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TrCH %d Need to increase TB size",tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].trch_id);
          tdsordered_config_ptr->ul_dch_trch_info[index].lg_ch_cnt_up = TRUE;
        }
        tdsordered_config_ptr->ul_dch_trch_info[index].lg_ch_cnt = 
             tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[index].ndlchan;
        tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Restore UL TDSDCH data for RB_id %d at  index %d for tdstrch_id %d ",
                            tdsrrc_ordered_hanging_rb_mapping[i].rb_id,i,
                            tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_dch_info.tr_id);
        tdsrrcllc_free_hanging_rb_mapping_info_index(tdsrrc_ordered_hanging_rb_mapping[i].rb_id, TDSUL_DCH_RB_MAPPING);
      }
    }
  }
  return ret_val;
}

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
uecomdef_status_e_type tdsrrcllc_restore_dl_dch_rb_mapping_info( tr_ch_id_type index)
{
  uint8 i,logch_idx;
  tr_ch_id_type tdstrch_id;
  uecomdef_logch_mode_e_type logch_mode;
  rlc_lc_id_type rlc_lc_id;
  uecomdef_status_e_type ret_val= SUCCESS;
 
  tdstrch_id = tdsordered_config_ptr->dl_dch_info[index].trch_id;
  
  for(i=0;i<TDSMAX_HANGING_RB_MAPPING_INFO;i++)
  {
    if((tdsrrc_ordered_hanging_rb_mapping[i].in_use == TRUE ) && 
       (tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.dch_map_info.valid == TRUE ))
    {
      if(tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.dch_map_info.tr_id == tdstrch_id )
      {       
        
        logch_idx = tdsordered_config_ptr->dl_dch_info[index].ndlchan;
        tdsordered_config_ptr->dl_dch_info[index].dlchan_info[logch_idx].rb_id = 
               tdsrrc_ordered_hanging_rb_mapping[i].rb_id;
        tdsordered_config_ptr->dl_dch_info[index].dlchan_info[logch_idx].mac_id =
               tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.dch_map_info.log_channel_id;
        /* Determine the type of Logical Channel mapped to DCH TrCH */
        if(tdsrrc_ordered_hanging_rb_mapping[i].rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          tdsordered_config_ptr->dl_dch_info[index].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
        }
        else
        {
          tdsordered_config_ptr->dl_dch_info[index].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
        }
   
        rlc_lc_id = tdsrrclcm_get_dl_rlc_lc_id((tdsrrc_RB_Identity)tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
      
        if((rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND) && 
            (tdsrrc_ordered_hanging_rb_mapping[i].logch_mode == UE_MODE_MAX_NUM ))
        {
          
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid DL LC ID for RB %d",tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
          ret_val= FAILURE;
          
        }
     
        if(tdsrrclcm_get_dl_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
        {
          if(tdsrrc_get_am_index(tdsrrc_ordered_hanging_rb_mapping[i].rb_id) != TDSUE_MAX_AM_ENTITY)
          {
            logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
          }
          else if(tdsrrcllc_get_dl_um_index((tdsrrc_RB_Identity)tdsrrc_ordered_hanging_rb_mapping[i].rb_id, 
              tdsordered_config_ptr)!= TDSUE_MAX_DL_UM_CHANNEL) 
          {
            logch_mode = UE_MODE_UNACKNOWLEDGED;
          }
          else if(tdsrrcllc_get_dl_tm_index(rlc_lc_id)!= UE_MAX_DL_TM_CHANNEL)
          {
            logch_mode = UE_MODE_TRANSPARENT;
          }
          else
          {
            if(tdsrrc_ordered_hanging_rb_mapping[i].logch_mode == UE_MODE_MAX_NUM )
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get DL Log Chl mode for LC %d",rlc_lc_id);
              ret_val= FAILURE;
            }
          }
        }
        /* Also update the Logical Channel ID here */
        tdsordered_config_ptr->dl_dch_info[index].dlchan_info[logch_idx].rlc_id = rlc_lc_id;
      
        /* Save the logical channel mode, if a setup */
        tdsordered_config_ptr->dl_dch_info[index].dlchan_info[logch_idx].rlc_mode = logch_mode;
      
        /* Increment number of logical channels */
        tdsordered_config_ptr->dl_dch_info[index].ndlchan++;
        /* The logic channel mapped to this TrCH increases from 1 to 2. */
        /* Need to update the TB size to reflect this change.           */
        if (tdsordered_config_ptr->dl_dch_info[index].ndlchan == 2 &&
          tdsordered_config_ptr->dl_dch_trch_info[index].lg_ch_cnt == 1)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"TrCH %d Need to increase TB size",tdsordered_config_ptr->dl_dch_info[index].trch_id);
          tdsordered_config_ptr->dl_dch_trch_info[index].lg_ch_cnt_up = TRUE;
        }
        tdsordered_config_ptr->dl_dch_trch_info[index].lg_ch_cnt = 
                tdsordered_config_ptr->dl_dch_info[index].ndlchan;
        
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"ndlchan %d after updating OC with Standalone DB",tdsordered_config_ptr->dl_dch_info[index].ndlchan );
        tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Restore DL TDSDCH data for RB_id %d at  index %d for tdstrch_id %d ",
                            tdsrrc_ordered_hanging_rb_mapping[i].rb_id,i,
                           tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.dch_map_info.tr_id);
        tdsrrcllc_free_hanging_rb_mapping_info_index(tdsrrc_ordered_hanging_rb_mapping[i].rb_id, TDSDL_DCH_RB_MAPPING);
      }
    } 
  }
  return ret_val;
}

/*==========================================================
FUNCTION  :   tdsrrcllc_restore_macd_rb_mapping_info()

DESCRIPTION : This function restores the Rb_mapping info from the RB-mapping backup info
               to the Ordered config for MAC-D flow id.

DEPENDENCIES: None.

RETURN VALUE: FAILURE: if the Rb-mapping info in the Rb-Mapping 
              backup info is invalid
              else 
              return SUCCESS

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type  tdsrrcllc_restore_macd_rb_mapping_info( uint8 index)
{
  uint8 i,logch_idx;
  uint8 mac_dflow_id;
  uecomdef_logch_mode_e_type logch_mode;
  rlc_lc_id_type rlc_lc_id;
  uecomdef_status_e_type ret_val= SUCCESS;
    
  mac_dflow_id = tdsordered_config_ptr->mac_dflow_info[index].mac_dflow_id;
  
  for(i=0;i<TDSMAX_HANGING_RB_MAPPING_INFO;i++)
  {
    if((tdsrrc_ordered_hanging_rb_mapping[i].in_use == TRUE ) && 
         (tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.macd_map_info.valid == TRUE ))
    {
      if(tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.macd_map_info.mac_d_flow_id == mac_dflow_id )
      {
                  
        logch_idx = tdsordered_config_ptr->mac_dflow_info[index].ndlchan;
        tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[logch_idx].rb_id = 
           tdsrrc_ordered_hanging_rb_mapping[i].rb_id;
        tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[logch_idx].mac_id =
           tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.macd_map_info.log_channel_id;
   
        /* Determine the type of Logical Channel mapped to DCH TrCH */
        if (tdsrrc_ordered_hanging_rb_mapping[i].rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
        } 
        else
        {
          tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
        }
          
        rlc_lc_id = tdsrrclcm_get_dl_rlc_lc_id((tdsrrc_RB_Identity)tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
        if ((rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)&&
            (tdsrrc_ordered_hanging_rb_mapping[i].logch_mode == UE_MODE_MAX_NUM ))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid DL LC ID for RB %d",tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
          ret_val= FAILURE;
        }
          
        if (tdsrrclcm_get_dl_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
        {
          if(tdsrrc_get_am_index(tdsrrc_ordered_hanging_rb_mapping[i].rb_id) != TDSUE_MAX_AM_ENTITY)
          {
            logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
          }           
          else if(tdsrrcllc_get_dl_um_index((tdsrrc_RB_Identity)tdsrrc_ordered_hanging_rb_mapping[i].rb_id, 
            tdsordered_config_ptr)!= TDSUE_MAX_DL_UM_CHANNEL)         
          {
            logch_mode = UE_MODE_UNACKNOWLEDGED;
          }
          else if(tdsrrcllc_get_dl_tm_index(rlc_lc_id)!= UE_MAX_DL_TM_CHANNEL)
          {
            logch_mode = UE_MODE_TRANSPARENT;
          }
          else
          {
            if(tdsrrc_ordered_hanging_rb_mapping[i].logch_mode == UE_MODE_MAX_NUM )
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get DL Log Chl mode for LC %d",rlc_lc_id);
              ret_val= FAILURE;
            }
          }
        }
        /* Also update the Logical Channel ID here */
        tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[logch_idx].rlc_id = rlc_lc_id;
      
        /* Save the logical channel mode, if a setup */
        tdsordered_config_ptr->mac_dflow_info[index].dlchan_info[logch_idx].rlc_mode = logch_mode;
      
        /* Increment number of logical channels */
        tdsordered_config_ptr->mac_dflow_info[index].ndlchan++;
   
        tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
   
        tdsrrcllc_free_hanging_rb_mapping_info_index(tdsrrc_ordered_hanging_rb_mapping[i].rb_id, 
                                                                                    TDSMAC_D_RB_MAPPING);
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Restore MAC_D data for RB_id %d at  index %d for flow id %d ",
                           tdsrrc_ordered_hanging_rb_mapping[i].rb_id,i,
                           tdsrrc_ordered_hanging_rb_mapping[i].dl_mapping_info.macd_map_info.mac_d_flow_id);
      }
    }
  }
  return ret_val;
}



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
uecomdef_status_e_type tdsrrcllc_backup_eul_from_ota
(
  tdsrrc_UL_LogicalChannelMapping_r6*ie_ptr,  
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
)
{
  uint8 idx;
  tdsrrc_RLC_PDU_SizeList *rlc_pdu_sizelist;
  uint8 ddi = 0;
  uint32 cnt = 0;
  uint16 rlc_size = 0;

  
  /* Store the RB-Mapping info */
  if( tdsrrcllc_check_rb_id_exist_rb_mapping(rb_id, &idx) == FALSE )
  {
    if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for UL DCH ");
      return FAILURE;
    }
  }
  tdsrrc_ordered_hanging_rb_mapping[idx].rb_id = rb_id;
  tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode = logch_mode;
    
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.valid = TRUE;
  
   
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.mac_e_flow_id= 
                 (uint8)ie_ptr->ul_TrCH_Type.u.e_dch->e_DCH_MAC_d_FlowIdentity;
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.mac_log_priority  = 
        (uint8) ie_ptr->mac_LogicalChannelPriority;

  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.log_channel_id =
       (uint8)ie_ptr->ul_TrCH_Type.u.e_dch->logicalChannelIdentity;
  /* Save the and RLC info DDI info*/
  
  rlc_pdu_sizelist = &ie_ptr->ul_TrCH_Type.u.e_dch->rlc_PDU_SizeList;

  /* If more than 1 UL RLC PDU size is configured for this RB, the different sizes will use subsequent DDI values starting
    from this DDI value.*/
  ddi = ie_ptr->ul_TrCH_Type.u.e_dch->ddi;
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.num_rlc_size_ddi_info = 0;

  while ((rlc_pdu_sizelist->n > cnt) && (cnt < TDSRRCLLC_MAX_NUM_OF_RLC_PDU_SIZE_PER_LOGCHAN)) 
  {
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.rlc_size_ddi_info[cnt].ddi = ddi;

    switch (rlc_pdu_sizelist->elem[cnt].t)
    {
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType1:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType1 * 8) + 16;
        break;
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType2:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType2->part1 * 32) + 272;
        if (rlc_pdu_sizelist->elem[cnt].u.sizeType2->m.part2Present)
        {
          rlc_size += (rlc_pdu_sizelist->elem[cnt].u.sizeType2->part2 * 8);
        }
        break;
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType3:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType3->part1 * 64) + 1040;
        if (rlc_pdu_sizelist->elem[cnt].u.sizeType3->m.part2Present)
        {
          rlc_size += (rlc_pdu_sizelist->elem[cnt].u.sizeType3->part2 * 8);
        }
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC TF type in OctetMode %d",rlc_pdu_sizelist->elem[cnt].t);
        break;
    }
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.rlc_size_ddi_info[cnt].rlc_pdu_size_in_bits = rlc_size;

    /* Increment the number of valid fields in rlc_size_ddi_info[] */
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.num_rlc_size_ddi_info++;
    cnt++;
    /* Increment the DDI */
    ddi++;

  }

  /* If set include scheduling info is set to false then dont consider this mac_id while sending schedulingInformation */
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.in_sched_info = 
    ie_ptr->ul_TrCH_Type.u.e_dch->includeInSchedulingInfo;

 
  TDSRRC_MSG3(MSG_LEGACY_HIGH," Adding into Standalone Database for R6 E_DCH for RB-ID %d for FLOW ID %d  at idx %d",
                             tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,
                             tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.mac_e_flow_id,
                             idx);
  
  
      
  return (SUCCESS);
}
/*==========================================================
FUNCTION  : tdsrrcllc_restore_ul_emac_rb_mapping_info()

DESCRIPTION : This function restores the Rb_mapping info from the RB-mapping backup info
               to the Ordered config for E-DCH MAC-d flowid.

DEPENDENCIES: None.

RETURN VALUE: FAILURE: if the Rb-mapping info in the Rb-Mapping 
              backup info is invalid
              else 
              return SUCCESS

SIDE EFFECTS: None
============================================================*/
uecomdef_status_e_type tdsrrcllc_restore_ul_emac_rb_mapping_info( uint8 index)
{
  uint8 i,logch_idx,j;
  uint8 mac_eflow_id;
  uecomdef_logch_mode_e_type logch_mode;
  rlc_lc_id_type rlc_lc_id;
  uecomdef_status_e_type ret_val= SUCCESS;
 
  mac_eflow_id = tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_mac_d_flow_id;
  
  for(i=0;i<TDSMAX_HANGING_RB_MAPPING_INFO;i++)
  {
    if((tdsrrc_ordered_hanging_rb_mapping[i].in_use == TRUE ) && 
         (tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.valid == TRUE))
    {
      if(tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.mac_e_flow_id == mac_eflow_id )
      {            
        logch_idx = tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].num_logch;
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].rb_id = 
                   tdsrrc_ordered_hanging_rb_mapping[i].rb_id;
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].lc_id =
                  tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.log_channel_id;
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].priority =
                 tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.mac_log_priority;
      
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].include_in_scheduling_info =
               tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.in_sched_info;           
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].num_rlc_size_ddi_info =
              tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.num_rlc_size_ddi_info;
        for( j=0;j<tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.num_rlc_size_ddi_info;j++)
        {
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].rlc_size_ddi_info[j]=
              tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.rlc_size_ddi_info[j];
        }
   
        /* Determine the type of Logical Channel mapped to DCH TrCH */
        if (tdsrrc_ordered_hanging_rb_mapping[i].rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
        } 
        else
        {
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
        }
   
   
        rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
        if ((rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)&&
            (tdsrrc_ordered_hanging_rb_mapping[i].logch_mode == UE_MODE_MAX_NUM ))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid DL LC ID for RB %d",tdsrrc_ordered_hanging_rb_mapping[i].rb_id);
          ret_val= FAILURE;
        }
   
        if (tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
        {
          
          if(tdsrrc_get_am_index(tdsrrc_ordered_hanging_rb_mapping[i].rb_id) != TDSUE_MAX_AM_ENTITY)
          {
            logch_mode = UE_MODE_ACKNOWLEDGED_DATA;            
          }           
          else if(tdsrrcllc_get_ul_um_index( (tdsrrc_RB_Identity)tdsrrc_ordered_hanging_rb_mapping[i].rb_id )!= 
            TDSUE_MAX_UL_UM_CHANNEL)         
          {
            logch_mode = UE_MODE_UNACKNOWLEDGED;
          }
          else if(tdsrrcllc_get_ul_tm_index(rlc_lc_id)!= UE_MAX_UL_TM_CHANNEL)
          {
            logch_mode = UE_MODE_TRANSPARENT;
          }
          else
          {
            if(tdsrrc_ordered_hanging_rb_mapping[i].logch_mode == UE_MODE_MAX_NUM )
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get UL Log Chl mode for LC %d",rlc_lc_id);
              ret_val= FAILURE;
            }
          }
        }
   
        /* Also update the Logical Channel ID here */
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].rlc_id = rlc_lc_id;
   
        /* Save the logical channel mode, if a setup */
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].e_ul_logch_info[logch_idx].rlc_mode = logch_mode;
      
        /* Increment number of logical channels */
        tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[index].num_logch++;  
   
        /* Here indicate that MAC Uplink needs to be reconfigured */
        tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
    
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Restore MAC_E data for RB_id %d at  index %d for flow id %d ",
                          tdsrrc_ordered_hanging_rb_mapping[i].rb_id,i,
                         tdsrrc_ordered_hanging_rb_mapping[i].ul_mapping_info.ul_mace.mac_e_flow_id);
        tdsrrcllc_free_hanging_rb_mapping_info_index(tdsrrc_ordered_hanging_rb_mapping[i].rb_id,
                                                                                 TDSE_DCH_RB_MAPPING);
      }
    }
  }
  return ret_val;
}
#endif/*FEATURE_TDSCDMA_HSUPA*/
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
boolean tdsrrcllc_check_ordered_current_mapping_same(uint8 index )
{
  if(tdsrrc_ordered_hanging_rb_mapping[index].rb_id == tdsrrc_current_hanging_rb_mapping[index].rb_id )
  {
    if((tdsrrc_current_hanging_rb_mapping[index].ul_mapping_info.ul_dch_info.valid == TRUE ) &&
      ( tdsrrc_ordered_hanging_rb_mapping[index].ul_mapping_info.ul_dch_info.valid == FALSE ))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"ul_dch_info info valid in CC Standalone DB,but invalid in OC Standalone DB for rbid % at index %d",tdsrrc_current_hanging_rb_mapping[index].rb_id ,index);
      return FALSE;
    }
#ifdef FEATURE_TDSCDMA_HSUPA
    if((tdsrrc_current_hanging_rb_mapping[index].ul_mapping_info.ul_mace.valid == TRUE ) &&
       ( tdsrrc_ordered_hanging_rb_mapping[index].ul_mapping_info.ul_mace.valid == FALSE ))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MAC E info valid in CC Standalone DB,but invalid in OC Standalone DB for rbid %d at index %d",tdsrrc_current_hanging_rb_mapping[index].rb_id ,index);
      return FALSE;
    }
#endif/*FEATURE_TDSCDMA_HSUPA*/
    if((tdsrrc_current_hanging_rb_mapping[index].dl_mapping_info.dch_map_info.valid == TRUE ) &&
      ( tdsrrc_ordered_hanging_rb_mapping[index].dl_mapping_info.dch_map_info.valid == FALSE ))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"dll_dch_info info valid in CC Standalone DB,but invalid in OC Standalone DB for rbid %d at index %d",tdsrrc_current_hanging_rb_mapping[index].rb_id ,index);
      return FALSE;
    }
    if((tdsrrc_current_hanging_rb_mapping[index].dl_mapping_info.macd_map_info.valid == TRUE ) &&
       ( tdsrrc_ordered_hanging_rb_mapping[index].dl_mapping_info.macd_map_info.valid == FALSE ))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MAC D info valid in CC Standalone DB,but invalid in OC Standalone DB for rbid %d at index %d",tdsrrc_current_hanging_rb_mapping[index].rb_id ,index);
      return FALSE;
    }
  }
  else
  {
   TDSRRC_MSG3(MSG_LEGACY_HIGH,"rb-d differe at index, current rb %d ordered %d",index,tdsrrc_current_hanging_rb_mapping[index].rb_id ,tdsrrc_ordered_hanging_rb_mapping[index].rb_id);
   return FALSE;
  }
  return TRUE;
}

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
)
{
  uint8 idx;
  /* Store the RB-Mapping info */
  if( tdsrrcllc_check_rb_id_exist_rb_mapping(rb_id, &idx) == FALSE )
  {
    if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for DL DCH ");
      return FAILURE;
    }
  }
  tdsrrc_ordered_hanging_rb_mapping[idx].rb_id = rb_id;
  tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode = logch_mode;
     
  tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.dch_map_info.valid = TRUE;
     
   tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.dch_map_info.tr_id = tdstrch_id;
    
  tdsrrc_ordered_hanging_rb_mapping[idx].dl_mapping_info.dch_map_info.log_channel_id = mac_id; 
   
  TDSRRC_MSG3(MSG_LEGACY_HIGH," Adding into Standalone Database for DL DCH for RB-ID  %d for Trch id %d  at idx %d",
                 tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,tdstrch_id,idx);
  return (SUCCESS);

}
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
  tdsrrc_UL_LogicalChannelMapping_r6*ie_ptr,  
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
)
{
  uint8 idx;
  /* Store the RB-Mapping info */
  if( tdsrrcllc_check_rb_id_exist_rb_mapping(rb_id, &idx) == FALSE )
  {
    if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for UL DCH ");
      return FAILURE;
    }
  }
  tdsrrc_ordered_hanging_rb_mapping[idx].rb_id = rb_id;
  tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode = logch_mode;
    
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.valid = TRUE;
  
   
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.tr_id = 
                (tr_ch_id_type) ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.u.dch;
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.mac_log_priority  = 
        (uint8) ie_ptr->mac_LogicalChannelPriority;
  /* If Logical Channel ID is present, save MAC Logical Channel Identity */
  if (ie_ptr->ul_TrCH_Type.u.dch_rach_usch->m.logicalChannelIdentityPresent)
  {
     tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.log_channel_id =
       (uint8)ie_ptr->ul_TrCH_Type.u.dch_rach_usch->logicalChannelIdentity;
  }
  else
  {
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.log_channel_id =0;
  }
  TDSRRC_MSG3(MSG_LEGACY_HIGH," Adding into Standalone Database for R6 UL DCH for RB-ID  %d for Trch id %d  at idx %d",
                             tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,
                             tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.tr_id,
                             idx);
  /* Save the RLC Size List information */
      
  return (SUCCESS);
}
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
)
{

  uint8 idx;
  /* Store the RB-Mapping info */
  if( tdsrrcllc_check_rb_id_exist_rb_mapping(rb_id, &idx) == FALSE )
  {
    if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for UL DCH ");
      return FAILURE;
    }
  }
  tdsrrc_ordered_hanging_rb_mapping[idx].rb_id = rb_id;
  tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode = logch_mode;
  
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.valid = TRUE;
  
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.tr_id = 
     (tr_ch_id_type)ie_ptr->ul_TransportChannelType.u.dch;
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.mac_log_priority
      = (uint8)ie_ptr->mac_LogicalChannelPriority;
  /* If Logical Channel ID is present, save MAC Logical Channel Identity */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_UL_LogicalChannelMapping,logicalChannelIdentity))
  {
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.log_channel_id =
         (uint8)ie_ptr->logicalChannelIdentity;
  }
  else
  {
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.log_channel_id =0;    
  } 
  TDSRRC_MSG3(MSG_LEGACY_HIGH," Adding into Standalone Database for UL DCH for RB-ID  %d for Trch id %d  at idx %d",
                             tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,
                             tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.tr_id,
                             idx);

  return (SUCCESS);
}

/*==========================================================
FUNCTION  : tdsrrcllc_validate_rb_mapping_info()

DESCRIPTION : This functions validates the RB mapping information
              for R99, R5 and R6.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE.

SIDE EFFECTS: 
============================================================*/
uecomdef_status_e_type tdsrrcllc_validate_rb_mapping_info(void)
{
  uint8 cntx = 0;
  uint8 cnty = 0;
  uint8 trch_idx = 0;
  tdsordered_config_type *config_ptr;

  if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    if(tdstransition_config.toc_usage != TDSTOC_INVALID)
    {
      config_ptr = tdstransition_config.toc_ptr;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TOC usage is invalid.");
      return SUCCESS;
    }
  }
  /*Validate DL DCH mapping info */
  for (trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++) 
  {
    if((config_ptr->dl_dch_trch_info[trch_idx].dch_present) &&
       (config_ptr->dl_dch_trch_idx_in_use[trch_idx] == TRUE))
    {
      if (config_ptr->dl_dch_info[trch_idx].ndlchan > 1) 
      {
        for (cntx = 0; ((cntx < config_ptr->dl_dch_info[trch_idx].ndlchan) && (cntx < MAX_DED_LOGCHAN_PER_TRCH)); cntx++) 
        {
          if (TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(config_ptr->dl_dch_info[trch_idx].dlchan_info[cntx].chan_type)) 
          {
            if (config_ptr->dl_dch_info[trch_idx].dlchan_info[cntx].mac_id == 
                TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT) 
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Logical Chan ID not present for rb_id %d",config_ptr->dl_dch_info[trch_idx].dlchan_info[cntx].rb_id);
              return FAILURE;
            }
            for (cnty = (cntx+1); ((cnty < config_ptr->dl_dch_info[trch_idx].ndlchan) 
                && (cnty < MAX_DED_LOGCHAN_PER_TRCH)); cnty++) 
            {
              if ((config_ptr->dl_dch_info[trch_idx].dlchan_info[cnty].mac_id  == 
                   config_ptr->dl_dch_info[trch_idx].dlchan_info[cntx].mac_id) &&
                  (TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(config_ptr->dl_dch_info[trch_idx].dlchan_info[cnty].chan_type)) ) 
              {
                TDSRRC_MSG3(MSG_LEGACY_ERROR,"Cannot assign same log chl ID %d to 2 dif rbs: %d %d: fail",
                    config_ptr->dl_dch_info[trch_idx].dlchan_info[cnty].mac_id,
                    config_ptr->dl_dch_info[trch_idx].dlchan_info[cnty].rb_id,
                    config_ptr->dl_dch_info[trch_idx].dlchan_info[cntx].rb_id);
                return (FAILURE);
                
              }
            } 
          }
        }
      } /* if ndlchan > 1*/
    }
  } /* For (trch_idx ...*/

  /* Validate FACH mapping info.  Look at OC always */
  if ((tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan > 1) 
      && (tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan <  MAX_DED_LOGCHAN_PER_TRCH))
  {
    for (cntx = 0; cntx < tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan; cntx++) 
    { 
      if (TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[cntx].chan_type))
      {
        if (tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[cntx].mac_id ==
            TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Logical Chan ID not present for rb_id %d",tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[cntx].rb_id);
          return FAILURE;
        }
        for (cnty = (cntx+1); cnty < tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_ndlchan ; cnty++) 
        {
          if ((tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[cntx].mac_id == 
              tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[cnty].mac_id) &&
              TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[cnty].chan_type)) 
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Cannot assign same log chl ID %d to 2 dif log chans: fail",tdsordered_config_ptr->cell_fach_rb_mapping_info.dl_dlchan_info[cntx].mac_id);
            return (FAILURE);
          }
        } 
      }
    } /*for cntx=0 ... */
  } /* if dl_ndlchan >= 1*/

  /* validate mapping to HSDPA */
  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) 
  {
    for(trch_idx = 0; trch_idx < UE_MAX_MAC_D_FLOW; trch_idx++)
    {
      if((config_ptr->dl_dflow_index_in_use[trch_idx] == TRUE) &&
         (config_ptr->mac_dflow_info[trch_idx].ndlchan > 1))
      {
        for (cntx = 0; cntx < config_ptr->mac_dflow_info[trch_idx].ndlchan; cntx++) 
        {
          if (TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(config_ptr->mac_dflow_info[trch_idx].dlchan_info[cntx].chan_type)) 
          {
            if (config_ptr->mac_dflow_info[trch_idx].dlchan_info[cntx].mac_id ==
                TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT) 
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Logical Chan ID not present for rb_id %d",config_ptr->mac_dflow_info[trch_idx].dlchan_info[cntx].rb_id);
              return FAILURE;
            }
            
            for (cnty = (cntx+1); ((cnty < config_ptr->mac_dflow_info[trch_idx].ndlchan)
                && (cnty < UE_MAX_LOGCHAN_PER_DFLOW)); cnty++) 
            {
              if ((config_ptr->mac_dflow_info[trch_idx].dlchan_info[cntx].mac_id == 
                   config_ptr->mac_dflow_info[trch_idx].dlchan_info[cnty].mac_id) && 
                  TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(config_ptr->mac_dflow_info[trch_idx].dlchan_info[cnty].chan_type))
              {
                TDSRRC_MSG3(MSG_LEGACY_ERROR,"Cannot assign same log chl ID %d to 2 dif rbs: %d %d: fail",
                    config_ptr->mac_dflow_info[trch_idx].dlchan_info[cntx].mac_id,
                    config_ptr->mac_dflow_info[trch_idx].dlchan_info[cntx].rb_id,
                    config_ptr->mac_dflow_info[trch_idx].dlchan_info[cnty].rb_id);
                return (FAILURE);
              }
            } 
          }
        } /* for (cntx = 0; ...*/
      }
    }/*for(trch_idx = 0 ...*/  

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    for(cntx = 0;cntx < config_ptr->mac_dl_parms.ndlchan_macehs; cntx++)
    {
      if((config_ptr->mac_ehs_lc_id_in_use[cntx] == TRUE)
        &&(TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(config_ptr->dl_macehs_logchan_info[cntx].chan_type)))
      {
        if (config_ptr->dl_macehs_logchan_info[cntx].mac_id ==
            TDSINVALID_LOGICAL_CHANNEL_ID) 
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Logical Chan ID not present for rb_id %d",config_ptr->dl_macehs_logchan_info[cntx].rb_id);
          return FAILURE;
        }
        
        for (cnty = (cntx+1); ((cnty < config_ptr->mac_dl_parms.ndlchan_macehs)
            &&(cnty < TDSUE_MAX_DL_LOGICAL_CHANNEL)); cnty++) 
        {
          if ((config_ptr->dl_macehs_logchan_info[cntx].mac_id == 
               config_ptr->dl_macehs_logchan_info[cnty].mac_id) && 
              TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(config_ptr->dl_macehs_logchan_info[cnty].chan_type))
          {
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Cannot assign same log chl ID %d to 2 dif rbs: %d %d: fail",
                config_ptr->dl_macehs_logchan_info[cntx].mac_id,
                config_ptr->dl_macehs_logchan_info[cntx].rb_id,
                config_ptr->dl_macehs_logchan_info[cnty].rb_id);
            return (FAILURE);
          }
        } 
      }
    }
#endif
  }/*if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)*/
   

  /* Validate UL DCH mapping info */
  for(trch_idx = 0; trch_idx < TDSUE_MAX_TRCH; trch_idx++)
  {
    if((config_ptr->ul_dch_trch_info[trch_idx].dch_present) &&
       (config_ptr->ul_dch_trch_idx_in_use[trch_idx] == TRUE))
    {
      if (config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan > 1)
      {
        for (cntx = 0; cntx < config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan; cntx++) 
        {
          if (TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cntx].chan_type))
          {
            if (config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cntx].mac_id == 
                TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT) 
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Logical Chan ID not present for rb_id %d",config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cntx].rb_id);
              return FAILURE;
            }
            
            for (cnty = (cntx+1); (cnty < config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan)
                                  && (cnty < MAX_DED_LOGCHAN_PER_TRCH); cnty++) 
            {
              if ((config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnty].mac_id ==
                   config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cntx].mac_id) &&
                  TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnty].chan_type))
              {
                TDSRRC_MSG3(MSG_LEGACY_ERROR,"Cannot assign same log chl ID %d to 2 dif rbs %d %d: fail",
                    config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cntx].mac_id,
                    config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cntx].rb_id,
                    config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnty].rb_id);
                return (FAILURE);
              }
            }
          }
        }/* for (cntx = 0; ...*/
      }
    }
  }/* for(trch_idx = 0; ...*/

  /* Validate PRACH mapping info.  Look at OC always */
  if ((tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan > 1) 
    &&(tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan < MAX_DED_LOGCHAN_PER_TRCH))
  {
    for (cntx = 0; cntx < tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan ; cntx++) 
    {
      if (TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[cntx].chan_type))
      {
        if (tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[cntx].mac_id == 
            TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT) 
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Logical Chan ID not present for rb_id %d",tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[cntx].rb_id);
          return FAILURE;
        }
        
        for (cnty = (cntx+1); cnty < tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan ; cnty++) 
        {
          if ((tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[cntx].mac_id == 
               tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[cnty].mac_id ) &&
              TDSRRCLLC_LC_TYPE_DCCH_OR_DCTCH(tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[cnty].chan_type))
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Cannot assign same log chl ID %d to 2 dif log chans: fail",tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[cntx].mac_id);
            return FAILURE;
          }
        }
      }
    }
  }

  return SUCCESS;

} /* tdsrrcllc_validate_rb_mapping_info() */

/*==========================================================
FUNCTION  : get_srb_info_to_setup_default_r5()

DESCRIPTION : Get Signalling Radio Bearer Information to 
              setup for Rb-Setup with preconfiguration info.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
uecomdef_status_e_type tdsrrcllc_get_srb_info_to_setup_preconfig_r5
(
tdsrrc_SRB_InformationSetup_r5     *ie_ptr,
tdsrrc_SRB_InformationSetupList_r5 * srb_setup_ptr,
tdsrrc_RB_Identity                   rb_id
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  
  /* Local count of # of SRB's to setup */
  uint8                           srb_cnt = 0;
  
  /* temp rb id which we shall compute if rb_id is not part of
  * srb_setup_ptr when we loop thru the list to get a match
  */
  tdsrrc_RB_Identity                      temp_rb_id = 0;
  

  
  /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_SRB_InformationSetup_r5     *temp_ie_ptr = ie_ptr;
  
  /* Entity indexes in case we need to look up RLC info from
  * our internal database instead of processing an IE.
  */
  uint8 entity_idx, new_entity_idx;
  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
   *rlc_Info for the rb_id specified.  Walk thru the srb_setup_head_ptr
   *list until we find a rb_id that matches
   */
  if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r5_same_as_RB))
  {
    while((srb_setup_ptr != NULL) && (srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {

      /* Check to see if the RB ID is there ... */
      if(srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        temp_rb_id = (tdsrrc_RB_Identity)srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }

      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        temp_rb_id++;
      }

      /*check if the rb_id matches*/
      if(ie_ptr->rlc_InfoChoice.u.same_as_RB == temp_rb_id)
      {
        if(srb_setup_ptr->elem[srb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r5_rlc_Info)
        {
          temp_ie_ptr = &(srb_setup_ptr->elem[srb_cnt]);
          break;
        }
      }

      
      srb_cnt++;
    } /* end while... */

    if (((srb_setup_ptr != NULL) && (srb_setup_ptr->n == 0)) ||
      (srb_cnt >= UE_MAX_SRB_SETUP) ||
      ((srb_setup_ptr != NULL) && (srb_cnt >= srb_setup_ptr->n)))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"same_as_RB %d not in msg,lookup req'd",ie_ptr->rlc_InfoChoice.u.same_as_RB);
      temp_ie_ptr = NULL;
      temp_rb_id = (tdsrrc_RB_Identity)ie_ptr->rlc_InfoChoice.u.same_as_RB;
      
      if((entity_idx = tdsrrc_get_am_index(temp_rb_id)) != TDSUE_MAX_AM_ENTITY)
      {
        /* This RB is in AM mode. And its reference config is found in OC.
         */
        if( (new_entity_idx = tdsrrc_get_am_index(rb_id)) == TDSUE_MAX_AM_ENTITY)
        {
          if ((new_entity_idx = tdsrrcllc_get_1st_hole_for_rlc_am()) == TDSUE_MAX_AM_ENTITY)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't find space for new rb %d",rb_id);
            return FAILURE;
          }
          tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan++;
          tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan++;
          tdsordered_config_ptr->rlc_am_parms.nentity++;
                  
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_ul_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->common_config.rb_id = rb_id;

        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan >
           TDSUE_MAX_UL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many UL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_dl_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.rlc_info;
   
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan >
           TDSUE_MAX_DL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many DL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }

        if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many AM entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
          return FAILURE;
        }
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
      }
             
      if(((entity_idx = tdsrrcllc_get_ul_um_index(temp_rb_id))
              != TDSUE_MAX_UL_UM_CHANNEL) && 
              ((entity_idx = tdsrrcllc_get_ul_um_index(rb_id))
              == TDSUE_MAX_UL_UM_CHANNEL))             
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_ul_um_info(rb_id, TRUE) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
           
      if(((entity_idx = tdsrrcllc_get_dl_um_index(temp_rb_id, tdsordered_config_ptr))
              != TDSUE_MAX_DL_UM_CHANNEL) &&
              ((entity_idx = tdsrrcllc_get_dl_um_index(rb_id, tdsordered_config_ptr))
              == TDSUE_MAX_DL_UM_CHANNEL))
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_dl_um_info(rb_id, TRUE, tdsordered_config_ptr) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      
      /* SRBs cannot be TM.  If we haven't found it by now, we have
       * a problem. */
      if(logch_mode == UE_MODE_MAX_NUM)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"same_as_RB %d not found in msg or OC",ie_ptr->rlc_InfoChoice.u.same_as_RB);
        return FAILURE;
      }
    }
  } /* end RLC choice == Same as RB */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_r5_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,ul_RLC_Mode))||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,ul_RLC_Mode)&&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r5,dl_RLC_Mode)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL or UL or Both RLC mode info not provided for rb id %d",rb_id);
      return FAILURE;
    }
  
  }

  /* Now we have either the RLC Info explicitly meant for this RB OR
  * the RLC Info that is the same as another RB.  If tmp_ie_ptr is NULL,
  * then we've already copied the RLC Info from our internal database.
  */
  if (temp_ie_ptr != NULL)
  {
    /* Get the RLC mode */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r5_rlc_Info) && 
         ((logch_mode = tdsrrcllc_get_rlc_mode_r5(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
       )
    {
      return FAILURE;
    }
    
    /* Get the RLC info */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r5_rlc_Info) && 
         (tdsrrcllc_get_rlc_info_r5(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }
  }
  
  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info_r5(&ie_ptr->rb_MappingInfo, rb_id, logch_mode) == FAILURE)
  {
    return FAILURE;
  }
  
  /* everything is configured */
  return SUCCESS;

} /* end tdsrrcllc_get_srb_info_to_setup_r5() */


#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
/*============================================================================
FUNCTION: tdsrrcllc_update_mac_ehs_ota()

DESCRIPTION:

      
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE

SIDE EFFECTS:
=============================================================================*/
static uecomdef_status_e_type tdsrrcllc_update_mac_ehs_ota
(
tdsrrc_LogicalChannelIdentity logicalChannelIdentity,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode,
uint32         macehs_queue_id
)
{
  uint8 lc_index;

  lc_index = tdsrrcllc_find_lc_index_for_macehs(tdsordered_config_ptr, logicalChannelIdentity,rb_id);
  if(lc_index == TDSUE_MAX_DL_LOGICAL_CHANNEL)
  {
     /* Allocate LC index */
     lc_index = tdsrrcllc_allocate_lc_index_macehs(tdsordered_config_ptr, logicalChannelIdentity);
 
     if(lc_index == TDSUE_MAX_DL_LOGICAL_CHANNEL)
     {
       /* Failure to allocate a new LC Index Index */
       TDSRRC_MSG0(MSG_LEGACY_ERROR,"MACEHS: Failure to allocate new Lc iD index");
       return(FAILURE);
     }
  }         

  if (logch_mode == UE_MODE_MAX_NUM)
  {
   /* This is the case when RB mapping info is coming 
    * in "Affected List". At this point, we need to update the Logical Channel mode 
    * and Logical Channel ID in Cell FACH RB Mapping Info - 
    * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
     */
    rlc_lc_id_type   rlc_lc_id;
  
    rlc_lc_id = tdsrrclcm_get_dl_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if (rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MACEHS: Invalid DL LC ID for RB %d",rb_id);
      return(FAILURE);
    }
    
    if (tdsrrclcm_get_dl_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MACEHS: Failure to get DL Log Chl mode for LC %d",rlc_lc_id);
      return(FAILURE);
    }
    /* Also update the Logical Channel ID here */
    tdsordered_config_ptr->dl_macehs_logchan_info[lc_index].rlc_id = rlc_lc_id;
  }

  tdshsdpa_msg_params.hsdsch_info_present =TRUE; 
  
  tdsordered_config_ptr->dl_macehs_logchan_info[lc_index].rlc_mode= logch_mode;
    
  tdsordered_config_ptr->dl_macehs_logchan_info[lc_index].rb_id = rb_id;
  
  if(rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID )
  {
    tdsordered_config_ptr->dl_macehs_logchan_info[lc_index].chan_type = UE_LOGCHAN_DCCH;
  }
  else
  {
    tdsordered_config_ptr->dl_macehs_logchan_info[lc_index].chan_type = UE_LOGCHAN_DTCH;
  }
  tdsordered_config_ptr->dl_macehs_logchan_info[lc_index].queue_id =   macehs_queue_id;

   return(SUCCESS);
}
#endif/* FEATURE_TDSCDMA_HSDPA_MAC_EHS*/

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_dch_mapping_info_r7()

DESCRIPTION : Processes DL DCH Mapping IE contents and saves 
              them in MAC Ordered Configuration Data  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_dch_mapping_info_r7
(
tdsrrc_DL_LogicalChannelMapping_r7 *ie_ptr,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode
)
{
  
  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id=0;

  /* Get the DL DCH TrCH ID if it exists */
  switch (ie_ptr->dl_TransportChannelType.t)
  {
  case T_tdsrrc_DL_TransportChannelType_r7_dch:
    tdstrch_id = (tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch;
    break;

  case T_tdsrrc_DL_TransportChannelType_r7_dch_and_hsdsch:
    if(ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->t == T_tdsrrc_DL_TransportChannelType_r7_dch_and_hsdsch_mac_hs )
    {
      tdstrch_id = (tr_ch_id_type)ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->u.mac_hs->dch_transport_ch_id;
    }
#ifndef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    else if(ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->t == T_tdsrrc_DL_TransportChannelType_r7_dch_and_hsdsch_mac_ehs )
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported");
      return FAILURE;
    }
#endif
    break;
    
  case T_tdsrrc_DL_TransportChannelType_r7_fach:
  case T_tdsrrc_DL_TransportChannelType_r7_dsch:
  default:
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Expecting DL DCH TrCH ID");
    return FAILURE;
  } /* end DL DCH TrCH ID switch */
  
    /* Get the transport channel index for this transport channel ID.
    * Note: The index should already be there.
  */

  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_LogicalChannelMapping_r7,logicalChannelIdentity))
  {
    return tdsrrcllc_update_dl_dch_ota(rb_id,logch_mode,tdstrch_id, ie_ptr->logicalChannelIdentity);
  }
  else
  {
    return tdsrrcllc_update_dl_dch_ota(rb_id,logch_mode,tdstrch_id, TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT);
  }
  

}
/*==========================================================
FUNCTION  : tdsrrcllc_get_hsdsch_mapping_info_r7()

DESCRIPTION : Processes HSDSCH Mapping IE contents and saves 
              them in MAC Ordered Configuration Data  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_hsdsch_mapping_info_r7
(
tdsrrc_DL_LogicalChannelMapping_r7 *ie_ptr,
tdsrrc_RB_Identity                    rb_id,
uecomdef_logch_mode_e_type    logch_mode
)
{
  /* DFLOW   Id */
  uint32         dflow_id= TDSINVALID_DFLOW_ID;

  boolean mac_hs_chosen = FALSE;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  boolean mac_ehs_chosen = FALSE;
  uint32 macehs_queue_id = TDSINVALID_QUEUE_ID;
#endif

  switch (ie_ptr->dl_TransportChannelType.t)
  {
  case T_tdsrrc_DL_TransportChannelType_r7_dch_and_hsdsch:
     
    if( ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->t == T_tdsrrc_DL_TransportChannelType_r7_dch_and_hsdsch_mac_hs)
    {
        TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:DFLOW Id is %d",ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->u.mac_hs->hsdsch_mac_d_flow_id);
      dflow_id = ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->u.mac_hs->hsdsch_mac_d_flow_id;
      mac_hs_chosen = TRUE;
    }
    else if( ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->t == T_tdsrrc_DL_TransportChannelType_r7_dch_and_hsdsch_mac_ehs)
    {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS

      mac_ehs_chosen = TRUE;
      macehs_queue_id = ie_ptr->dl_TransportChannelType.u.dch_and_hsdsch->u.mac_ehs->hsdsch_mac_ehs_QueueId;
      TDSRRC_MSG1(MSG_LEGACY_LOW,"MACEHS:Queue Id is %d",macehs_queue_id);
#else
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported ");
      return FAILURE;
#endif
    }
    break;

  case T_tdsrrc_DL_TransportChannelType_r7_hsdsch:
    if( ie_ptr->dl_TransportChannelType.u.hsdsch->t == T_tdsrrc_DL_TransportChannelType_r7_hsdsch_mac_hs )
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:DFLOW Id is %d",ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_hs); 
      mac_hs_chosen = TRUE;
      dflow_id = ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_hs;
    }
    else if( ie_ptr->dl_TransportChannelType.u.hsdsch->t == T_tdsrrc_DL_TransportChannelType_r7_hsdsch_mac_ehs )
    {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      mac_ehs_chosen = TRUE;
      macehs_queue_id = ie_ptr->dl_TransportChannelType.u.hsdsch->u.mac_ehs;
      TDSRRC_MSG1(MSG_LEGACY_LOW,"MACEHS Queue Id is %d",macehs_queue_id);
#else
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported ");
      return FAILURE;
#endif
    }
    break;

  default:
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Invalid  Channel Type %d",ie_ptr->dl_TransportChannelType.t);
    return FAILURE;
  }

  if(mac_hs_chosen == TRUE )
  {
    /* If Logical Channel ID is present, save MAC Logical Channel Identity */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_LogicalChannelMapping_r7,logicalChannelIdentity))
    {
      return tdsrrcllc_update_machs_ota(ie_ptr->logicalChannelIdentity,rb_id,logch_mode,dflow_id);
    }
    else 
    {
      return tdsrrcllc_update_machs_ota(TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT,rb_id,logch_mode,dflow_id);
    }
  }
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  else if (mac_ehs_chosen == TRUE )
  {

    /* If Logical Channel ID is present, save MAC Logical Channel Identity */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_DL_LogicalChannelMapping_r7,logicalChannelIdentity))
    {
      return tdsrrcllc_update_mac_ehs_ota(ie_ptr->logicalChannelIdentity,rb_id,logch_mode,macehs_queue_id);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH," Logical ID mandatory for MAC-Ehs ");
      return FAILURE;
    }
  }
#endif
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: MAC EHS not supported");
  }


  return SUCCESS;

}

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_logical_channel_info_r7()

DESCRIPTION : Get DL logical Channel info.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_logical_channel_info_r7
(
tdsrrc_DL_LogicalChannelMappingList_r7 * ie_ptr,
tdsrrc_RB_Identity                       rb_id,
uecomdef_logch_mode_e_type       logch_mode
)
{
  /*
  * Count to make sure that don't exceed number of Logical
  * channels that can be Mapped to a Transport Channel.
  */
  uint8 lc_tc_cnt = 0;
  
  tdsordered_config_type *tmp_config_ptr = NULL;

  
  /* Process each item of the list */
  while ((ie_ptr->n > lc_tc_cnt) && (lc_tc_cnt < MAX_DED_LOGCHAN_PER_TRCH))
  {
    /* Transport channel type */
    switch (ie_ptr->elem[lc_tc_cnt].dl_TransportChannelType.t)
    {

    case T_tdsrrc_DL_TransportChannelType_r7_hsdsch:
      /* Check if logical channel is not TM */
      if (logch_mode == UE_MODE_TRANSPARENT)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:TM Log Channel mapped on HSDSCH");
        return FAILURE;
      }
      if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID )
      {
        /* Insane network. Mapping SRBs on HSDSCH */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:RB %d cannot be mapped on HSDSCH",rb_id);
        return FAILURE;
      }
      if (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
      {
        if (tdsrrcllc_get_hsdsch_mapping_info_r7(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          return FAILURE;
        }
      } 
      else
      {
        /*TBD PSR: Why will ever fo here as the rrc_state_indicator in OC is always DCH
          * But then in that case the mapping info will always be in OC and not in TOC?? 
          */
        if (tdstransition_config.toc_usage == TDSTOC_INVALID)
        {
          tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
        }
        
        tmp_config_ptr = tdsordered_config_ptr;
        tdsordered_config_ptr = tdstransition_config.toc_ptr;
        
        /* Processing functions must think we are going to Cell_DCH state. */
        tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
        
        if (tdsrrcllc_get_hsdsch_mapping_info_r7(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
          return FAILURE;
        }
        
        tdsordered_config_ptr = tmp_config_ptr;
        tmp_config_ptr = NULL;
      }
      break;
    case T_tdsrrc_DL_TransportChannelType_r7_dch:
      if (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
      {
        if (tdsrrcllc_get_dl_dch_mapping_info_r7(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          return FAILURE;
        }
      } 
      else
      {
        if (tdstransition_config.toc_usage == TDSTOC_INVALID)
        {
          tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
        }
        
        tmp_config_ptr = tdsordered_config_ptr;
        tdsordered_config_ptr = tdstransition_config.toc_ptr;
        
        /* Processing functions must think we are going to Cell_DCH state. */
        tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
        
        if (tdsrrcllc_get_dl_dch_mapping_info_r7(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
          return FAILURE;
        }
        
        tdsordered_config_ptr = tmp_config_ptr;
        tmp_config_ptr = NULL;
      }
      break;
      
    case T_tdsrrc_DL_TransportChannelType_r7_fach:

      if (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH ||
        tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH ||
        tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
      {
         /* If Logical Channel ID is present, save MAC Logical Channel Identity */ 
           if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[lc_tc_cnt],tdsrrc_DL_LogicalChannelMapping_r7,logicalChannelIdentity))
           {
             if(tdsrrcllc_get_fach_mapping_info(ie_ptr->elem[lc_tc_cnt].logicalChannelIdentity, rb_id, logch_mode,
                                      tdsordered_config_ptr->rrc_state_indicator) == FAILURE)
             {
               return FAILURE;
             }
           }
           else
           {
             if(tdsrrcllc_get_fach_mapping_info(TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT, rb_id, logch_mode,
                                      tdsordered_config_ptr->rrc_state_indicator) == FAILURE)
             {
               return FAILURE;
             }
           }
      } 
      else
      {
      /* FACH mapping info can come in DCH state. Accept and update
      * local mapping info structure.
        */
         /* If Logical Channel ID is present, save MAC Logical Channel Identity */ 
           if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[lc_tc_cnt],tdsrrc_DL_LogicalChannelMapping_r7,logicalChannelIdentity))
           {
             if(tdsrrcllc_get_fach_mapping_info(ie_ptr->elem[lc_tc_cnt].logicalChannelIdentity, rb_id, logch_mode,
                                      TDSRRC_STATE_CELL_DCH) == FAILURE)
             {
               return FAILURE;
             }
           }
           else
           {
             if(tdsrrcllc_get_fach_mapping_info(TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT, rb_id, logch_mode,
                                      TDSRRC_STATE_CELL_DCH) == FAILURE)
             {
               return FAILURE;
             }
           }
      }
      break;
      
    case T_tdsrrc_DL_TransportChannelType_r7_dsch:
    case T_tdsrrc_DL_TransportChannelType_r5_dch_and_dsch:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"DSCH TrCH not supported");
      return FAILURE;
      
    case T_tdsrrc_DL_TransportChannelType_r7_dch_and_hsdsch:
      if (rb_id < DCCH_DT_LOW_PRI_RADIO_BEARER_ID )
      {
        /* Insane network. Mapping SRBs on HSDSCH */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:RB cannot be mapped on HSDSCH");
        return FAILURE;
      }

      if (tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
      {
        if (tdsrrcllc_get_dl_dch_mapping_info_r7(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          return FAILURE;
        }
        if (tdsrrcllc_get_hsdsch_mapping_info_r7(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          return FAILURE;
        }
      } 
      else
      {
        if (tdstransition_config.toc_usage == TDSTOC_INVALID)
        {
          tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
        }
        
        tmp_config_ptr = tdsordered_config_ptr;
        tdsordered_config_ptr = tdstransition_config.toc_ptr;
        
        /* Processing functions must think we are going to Cell_DCH state. */
        tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
        
        if (tdsrrcllc_get_dl_dch_mapping_info_r7(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
          return FAILURE;
        }
        
        if (tdsrrcllc_get_hsdsch_mapping_info_r7(&ie_ptr->elem[lc_tc_cnt], rb_id, logch_mode) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;
          return FAILURE;
        }
        
        tdsordered_config_ptr = tmp_config_ptr;
        tmp_config_ptr = NULL;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcv'd invalid DL TrCh type %d",ie_ptr->elem[lc_tc_cnt].dl_TransportChannelType.t);
      return FAILURE;
    } /* end Logical channel type switch */
    
    /* Get pointer to the next item in the list */
    
    lc_tc_cnt++;
  } /* end while ie_ptr != NULL */
  
  return SUCCESS;

}
/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_mapping_info_r7()

DESCRIPTION : Get Radio Bearer Mapping Info IE

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rb_mapping_info_r7
(
tdsrrc_RB_MappingInfo_r7      * ie_ptr,
tdsrrc_RB_Identity                 rb_id,
uecomdef_logch_mode_e_type logch_mode
,boolean one_sided_reest_present
,boolean one_sided_reest
,boolean allow_new_channels
)
{
  /*
  * Count to make sure that don't exceed the maximum number 
  * of RB Mux options
  */
  uint8 rb_mux_cnt = 0;

  /* Process the RB Mux option IE in the linked list */
  while ((ie_ptr->n > rb_mux_cnt) && (rb_mux_cnt < MAX_RB_MUX_OPTIONS_REL5))
  {

    /* Check for DL Channel Mapping List */
    if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[rb_mux_cnt],tdsrrc_RB_MappingOption_r7,dl_LogicalChannelMappingList))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for DL",rb_id);
      if (tdsrrcllc_get_dl_logical_channel_info_r7(&ie_ptr->elem[rb_mux_cnt].dl_LogicalChannelMappingList,
                                         rb_id, logch_mode
                                  ) == FAILURE)
      {
        return FAILURE;
      }
      /* Here indicate that MAC Downlink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
    }  /* end dl Log Ch mapping list */

    if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[rb_mux_cnt],tdsrrc_RB_MappingOption_r7,ul_LogicalChannelMappings))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for UL",rb_id);
      if (tdsrrcllc_get_ul_logical_channel_info_r6(&ie_ptr->elem[rb_mux_cnt].ul_LogicalChannelMappings,
                                      rb_id, logch_mode
                                      , one_sided_reest_present, one_sided_reest, allow_new_channels
                                       ) == FAILURE)
      {
        return FAILURE;
      }

      /* Here indicate that MAC Uplink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
    }  /* end ul Log Ch mapping list */

  
    rb_mux_cnt++;

  }  /* end loop of RB Mapping info list */

  return SUCCESS;
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_dl_am_rlc_info_r7()

DESCRIPTION : 
  Read the information for Downlink AM RLC from the 
  received structure and write them into one of the 
  pre-allocated AM memory block.

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_dl_am_rlc_info_r7
(
tdsrrc_DL_AM_RLC_Mode_r7 *ie_ptr,
uint8               entity_idx,
tdsrrc_RB_Identity          rb_id,
boolean       allow_new_channels,
tdsrlc_reestablish_e_type reestablish_entity
)
{
  tdsrlc_dl_info_type *rlc_dl_info;
  
  /* local copy of poitner */
  if (entity_idx == TDSUE_MAX_AM_ENTITY)
  {
    entity_idx = tdsordered_config_ptr->rlc_am_parms.nentity;
  }
  rlc_dl_info = 
    &tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.rlc_info;
  
  /* Turn on the information included flag  */
  rlc_dl_info->rlc_info_incl = TRUE;
  
  /* indicates if in sequence delivery is required */
  rlc_dl_info->in_sequence = ie_ptr->inSequenceDelivery;
  
  /* Get the maximum number of RLC PUs allowed to be rcv'd  */
  if ((rlc_dl_info->rx_window_size = tdsrrcllc_rx_window_size(ie_ptr->receivingWindowSize))
    == TDSRRCLLC_INVALID_WINDOW_SIZE)
  {
    return FAILURE;
  }
  
  /* Get the downlink status info */
  if (tdsrrcllc_get_dl_rlc_status_info(rlc_dl_info, &ie_ptr->dl_RLC_StatusInfo) == FAILURE)
  {
    return FAILURE;
  }
  
  TDSRRC_MSG0(MSG_LEGACY_MED,"Processing DL RLC PDU Size");
  
  if(ie_ptr->dl_RLC_PDU_size.t == T_tdsrrc_DL_AM_RLC_Mode_r7_dl_RLC_PDU_size_fixedSize)
  {

    if(tdsrrcllc_process_dl_pdu_size(ie_ptr->dl_RLC_PDU_size.u.fixedSize, entity_idx,allow_new_channels,reestablish_entity)
     == SUCCESS)
    {
      /* Set this RLC Size in the global structure tdsrlc_size_per_rb */
      if (tdsrrc_set_dl_am_rlc_pdu_size(rb_id, tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.dl_rlc_size) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to set DL AM PDU Size Info in rlc_per_rb");
        return FAILURE;
      }
    }
    else
    {
      return FAILURE;
    }
  }
  else if(ie_ptr->dl_RLC_PDU_size.t == T_tdsrrc_DL_AM_RLC_Mode_r7_dl_RLC_PDU_size_flexibleSize)
  {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
   tdsrrcllc_set_am_dl_rlc_size_ota(
     rb_id,TDSRRCLLC_INVALID_RLC_SIZE ,TDSRRC_FLEXIBLE_PDU_SIZE_SET

      );

    /* 25.331-8.6.4.9: 1> if the IE "Downlink RLC mode" is present and is set to "AM RLC":
                                        3> if the DL RLC PDU size is set to "flexible size":
                                          4> if this radio bearer is a signalling radio bearer:
                                            5> the UE behaviour is unspecified. */
    if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"AM SRB's DL RLC PDU size is set to flexible size,rb id: %d",rb_id);
      return FAILURE;
    }

    if(ie_ptr->dl_RLC_PDU_size.u.flexibleSize == tdsrrc_DL_AM_RLC_Mode_r7_flexibleSize_size15)
    {
      tdsrrcllc_process_flexible_pdu_dl_am(TDSLI_15_BITS, entity_idx,allow_new_channels,reestablish_entity);
    }
    else if (ie_ptr->dl_RLC_PDU_size.u.flexibleSize == tdsrrc_DL_AM_RLC_Mode_r7_flexibleSize_size7)
    {
      tdsrrcllc_process_flexible_pdu_dl_am(TDSLI_7_BITS, entity_idx,allow_new_channels,reestablish_entity);
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHSPA+: Invalid value %d  for Li size for flexible PDU  ",ie_ptr->dl_RLC_PDU_size.u.flexibleSize);
      return FAILURE;
    }
   
#else
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Flexible PDU size not supported ");
#endif
  }

  /* everything is a go */
  return SUCCESS;

}
/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_mode_r7()

DESCRIPTION : Gets the RLC mode for a given logical channel.

DEPENDENCIES: None.

RETURN VALUE: uecomdef_logch_mode_e_type indicate whether or 
              not it is TM, UM or AM.

SIDE EFFECTS: None.
============================================================*/
static uecomdef_logch_mode_e_type tdsrrcllc_get_rlc_mode_r7
(
  tdsrrc_RLC_Info_r7 *ie_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  uecomdef_logch_mode_e_type dl_logch_mode = UE_MODE_MAX_NUM;

  /* Uplink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,ul_RLC_Mode))
  {
    switch (ie_ptr->ul_RLC_Mode.t)
    {
      case T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode:
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
        break;
  
      case T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode:
        logch_mode = UE_MODE_UNACKNOWLEDGED;
        break;
  
      case T_tdsrrc_UL_RLC_Mode_ul_TM_RLC_Mode: 
        logch_mode = UE_MODE_TRANSPARENT;
        break;
  
      case T_tdsrrc_UL_RLC_Mode_spare:
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown UL RLC mode %d",ie_ptr->ul_RLC_Mode.t);
        logch_mode = UE_MODE_MAX_NUM;
        break;
    }  /* end UL RLC t */
  }  /* end if UL RLC present */

  /* Downlink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,dl_RLC_Mode))
  {
    switch (ie_ptr->dl_RLC_Mode.t)
    {
      case T_tdsrrc_DL_RLC_Mode_r7_dl_AM_RLC_Mode:
        dl_logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
        break;
  
      case T_tdsrrc_DL_RLC_Mode_r7_dl_UM_RLC_Mode:
        dl_logch_mode = UE_MODE_UNACKNOWLEDGED;
        break;
  
      case T_tdsrrc_DL_RLC_Mode_r7_dl_TM_RLC_Mode: 
        dl_logch_mode = UE_MODE_TRANSPARENT;
        break;
  
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown DL RLC mode %d",ie_ptr->ul_RLC_Mode.t);
        dl_logch_mode = UE_MODE_MAX_NUM;
        break;
    }  /* end DL RLC choice */

    /* If both DL and UL mode is present, check to make sure they are the same */
    if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,ul_RLC_Mode))
    {
      if (logch_mode != dl_logch_mode) 
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"UL %d and DL %d lc mode not a match.  Invalid config",logch_mode,dl_logch_mode);
        logch_mode = UE_MODE_MAX_NUM;
      }
    }
    else
    {
      logch_mode = dl_logch_mode;
    }

  }  /* end if DL RLC present */

  /* Need to know RLC type in either UL or DL */
  if (!((ie_ptr->m.ul_RLC_ModePresent) ||
                            (ie_ptr->m.dl_RLC_ModePresent)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Neither DL nor UL RLC mode present!");
  }


  return logch_mode;

} /* end tdsrrcllc_get_rlc_mode_r6 */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_info_r7 ()

DESCRIPTION : 
  Processes RLC Info IE

DEPENDENCIES: 
  None.

RETURN VALUE:
  SUCCESS or FAILURE

SIDE EFFECTS:
  None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rlc_info_r7
(
tdsrrc_RLC_Info_r7 *ie_ptr,
tdsrrc_RB_Identity    rb_id,
boolean       allow_new_channels
)
{
  uint8 entity_idx;
  boolean new_rlc_am_entity = FALSE;
  
  tdsrlc_reestablish_e_type reestablish_entity = TDSRLC_RE_ESTABLISH_NONE;


  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,altE_bitInterpretation))
  {
    if(((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,ul_RLC_Mode)) &&
        (ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode ))
        ||((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,dl_RLC_Mode)) &&
           (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->dl_RLC_Mode,rrc_DL_RLC_Mode_r7_dl_UM_RLC_Mode))))
    {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"N\\W send ALT E Bit,ALT E Bit  supported");
    }
  }


  /* Uplink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,ul_RLC_Mode))
  {
    if(tdsrrcllc_get_ul_rlc_info(&ie_ptr->ul_RLC_Mode,rb_id,allow_new_channels,
        &new_rlc_am_entity) == FAILURE)
    {
      return FAILURE;
    }

    if(ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_UM_RLC_Mode )
    {
       entity_idx = tdsrrcllc_get_ul_um_index(rb_id);

      if(entity_idx < TDSUE_MAX_UL_UM_CHANNEL)
      {
        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,altE_bitInterpretation))
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is present");
          tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].alternate_e_bit = TRUE;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is not present");
          tdsordered_config_ptr->rlc_ul_um_parms.chan_info[entity_idx].alternate_e_bit = FALSE;
        }
      }
    }

#ifdef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
    if(ie_ptr->ul_RLC_Mode.t == T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode )
    {
      entity_idx = tdsrrc_get_am_index(rb_id);
  
      if(entity_idx >= TDSUE_MAX_AM_ENTITY)
      {
        return FAILURE;
      }
      else
      {
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,useSpecialValueOfHEField))
        {
          tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.special_value_he_config = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_LOW,"Special HE field is present in OTA.");  
    
        }
        else
        {
          tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.special_value_he_config = FALSE;
        }    
      }
    }
#endif  

  }/* end if UL RLC present */
  
  /* Downlink RLC info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,dl_RLC_Mode))
  {
    switch (ie_ptr->dl_RLC_Mode.t)
    {
      case T_tdsrrc_DL_RLC_Mode_r7_dl_AM_RLC_Mode: /* rrc_dl_AM_RLC_Mode_r7_chosen */
        TDSRRC_MSG1(MSG_LEGACY_LOW,"RRCHS:Get DL AM RLC Info for RB %d",rb_id);
        
        entity_idx = tdsrrc_get_am_index(rb_id);
        
        /* LLC processes the UL AM info first, it should find the corresponding
        entity index for DL AM info to be filled in. */
        if (entity_idx == TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not allowed to add new DL AM RLC entity");
          return FAILURE;
        }
        
        if (ie_ptr->rlc_OneSidedReEst == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish DL only");
          reestablish_entity = TDSRLC_RE_ESTABLISH_DL;
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_MED,"rlcsizeindicated Reestablish DL & UL");
          reestablish_entity = TDSRLC_RE_ESTABLISH_DL_UL;
        }
        
        if (tdsrrcllc_get_dl_am_rlc_info_r7(ie_ptr->dl_RLC_Mode.u.dl_AM_RLC_Mode, entity_idx, rb_id
          , allow_new_channels, reestablish_entity
          ) == FAILURE)
        {
          return FAILURE;
        }
#ifdef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
        if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,useSpecialValueOfHEField))
        {
          tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.special_value_he_config = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Special HE field is present in OTA.");          
        }
        else
        {
          tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.special_value_he_config = FALSE;
        }
#endif

        
        /* This flag is set when processing UL AM info. */
        if(new_rlc_am_entity)
        {
        /* Indicate that the AM entity has been filled up. LLC does not increase this
        number of entities when process UL AM info.
          */
          tdsordered_config_ptr->rlc_am_parms.nentity++;
          
          /* Make sure UE has not exceeded the maximum number of AM Entities */
          if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too Many AM Entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
            return FAILURE;
          }
          
          /* It always assume to be the shared channel (Control&Data) 
          as we support now.
          */  
          tdsordered_config_ptr->rlc_am_parms.am_info[entity_idx]->dl_nchan = 1;
          
        }
        
        break;  /* DL AM */
        
      case T_tdsrrc_DL_RLC_Mode_r7_dl_UM_RLC_Mode: /* rrc_dl_UM_RLC_r6_Mode_chosen NULL */   

        if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RLC_Info_r7,altE_bitInterpretation))
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is present");
          if(tdsrrcllc_get_dl_rlc_um_info_r6(ie_ptr->dl_RLC_Mode.u.dl_UM_RLC_Mode,rb_id,allow_new_channels
                  ,TRUE 
                  ) == FAILURE)
          {
            return FAILURE;
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_LOW,"IE altE-bitInterpretation is not present");      
          if(tdsrrcllc_get_dl_rlc_um_info_r6(ie_ptr->dl_RLC_Mode.u.dl_UM_RLC_Mode,rb_id,allow_new_channels
                  ,FALSE
                  ) == FAILURE)
          {
            return FAILURE;
          }
        }
        break;
        
      case T_tdsrrc_DL_RLC_Mode_r7_dl_TM_RLC_Mode:
        if(tdsrrcllc_get_dl_rlc_tm_info(ie_ptr->dl_RLC_Mode.u.dl_TM_RLC_Mode->segmentationIndication,
            rb_id,allow_new_channels) == FAILURE)
        {
          return FAILURE;
        }
        break;
        
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown DL RLC mode %d",ie_ptr->dl_RLC_Mode.t);
        return FAILURE;
    } /* end DL RLC choice */
  } /* end if DL RLC present */


#ifndef FEATURE_TDSCDMA_RLC_SPECIAL_HE_VALUE
  if (TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(ie_ptr,useSpecialValueOfHEField))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHSPA+: SpecialValueOfHEField not supported");
  }
#endif

  /* Got here, everything is a go */
  return SUCCESS;

} /* end tdsrrcllc_get_rlc_info_r6() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_to_reconfig_r7()

DESCRIPTION : Get Radio Bearer Information to reconfig.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rb_info_to_reconfig_r7
(
tdsrrc_RB_InformationReconfig_r7 *ie_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  boolean  one_sided_reest_present=FALSE;
  boolean  one_sided_reest=FALSE;
#ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
  uint8 entity_idx = 0;
#endif

  /* Check to see if the RLC info is present */
  if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r7,rlc_Info))
     && ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode))))
  {
    /* Get the RLC mode */
    if ((logch_mode = tdsrrcllc_get_rlc_mode_r7(&ie_ptr->rlc_Info)) == UE_MODE_MAX_NUM)
    {
      return FAILURE;
    }
    
    /* Save off the RLC Info */
    if (tdsrrcllc_get_rlc_info_r7(&ie_ptr->rlc_Info, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, FALSE) == FAILURE)
    {
      return FAILURE;
    }
  } /* end RLC Info Check */
  #ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
  /*Copy rlc one side re-establishment flag to OC*/
if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r7,rlc_Info))
{
  if (ie_ptr->rlc_Info.rlc_OneSidedReEst == TRUE)
  {
  
    entity_idx = tdsrrc_get_am_index(ie_ptr->rb_Identity);
    if (entity_idx != UE_MAX_AM_ENTITY)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_ONE_SIDE:  set to ONE_SIDE");
      tdsordered_config_ptr->rlc_one_sided_reest[entity_idx] = TRUE;
    }
  }
}
#endif 

  /* Save off the RB Mapping info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r7,rb_MappingInfo))
  {
    /* Remove any existing mapping for the RB */
    tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);
    
    if(ie_ptr->m.rlc_InfoPresent)
    {
      one_sided_reest_present=TRUE;
      one_sided_reest =ie_ptr->rlc_Info.rlc_OneSidedReEst;
    }
    
    if (tdsrrcllc_get_rb_mapping_info_r7(&ie_ptr->rb_MappingInfo, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, logch_mode
        ,one_sided_reest_present,one_sided_reest, FALSE
	) == FAILURE)
    {
      return FAILURE;
    }
  } /* end of RB Mapping info check */
  
#ifdef FEATURE_UMTS_PDCP
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r7,pdcp_Info))
  {
    if (tdsrrc_pdcp_enabled)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDPC info present in rb setup");
      if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,
        &ie_ptr->pdcp_Info.pdcp_Info, 
        (tdsrrc_RB_Identity)ie_ptr->rb_Identity, 
        TDSCPDCP_RECONFIG
        ) == FAILURE)
      {
        return FAILURE;
      }
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r7,pdcp_SN_Info)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r7,pdcp_Info)) &&
             ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
              (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
              ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported))))))          
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
      return FAILURE;
    }
  }
#else

  /* Ignore the PDCP info for now */
  /********************************/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r7,pdcp_SN_Info)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r7,pdcp_Info)) &&
             ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
              (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
              ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported))))))          
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }

#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

} /* end tdsrrcllc_get_rb_info_to_reconfig_r6() */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_info_affected_r7()

DESCRIPTION : Get Radio Bearer Information to be affected.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rb_info_affected_r7
(
tdsrrc_RB_InformationAffected_r7*ie_ptr
)
{
  /* Remove any existing mapping for the RB */
  tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);


  /* Get the RB Mapping info */
  return(tdsrrcllc_get_rb_mapping_info_r7(&ie_ptr->rb_MappingInfo, 
                                (tdsrrc_RB_Identity)ie_ptr->rb_Identity, 
                                UE_MODE_MAX_NUM
                                ,FALSE,FALSE,FALSE
         ));

}  /* end tdsrrcllc_get_rb_info_affected_r6() */

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
  tdsrrc_RB_InformationSetupList_r7   * rb_setup_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type    logch_mode = UE_MODE_MAX_NUM;
  
  /*initialize a counter to check rb count*/
  uint8                         rb_cnt  = 0;

  /*place holder for rb_id we get originally*/
  tdsrrc_RB_Identity                    local_rb_id = (tdsrrc_RB_Identity)ie_ptr->rb_Identity;
  
  /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_RB_InformationSetup_r7     *temp_ie_ptr = ie_ptr;

#ifdef FEATURE_UMTS_PDCP
  uint8 idx = 0;
#endif /* FEATURE_UMTS_PDCP */  
  
  boolean  one_sided_reest_present=FALSE;
  boolean one_sided_reest =FALSE;

  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
  *rlc_Info for the rb_id specified.  Walk thru the rb_setup_head_ptr
  *list until we find a rb_id that matches
  */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r7_same_as_RB))
  {
    while ((rb_setup_ptr->n > rb_cnt) && (rb_cnt < MAX_RB_PER_RAB))
    {
      if (ie_ptr->rlc_InfoChoice.u.same_as_RB == 
        rb_setup_ptr->elem[rb_cnt].rb_Identity)
      {
        if(rb_setup_ptr->elem[rb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info)
        {
           temp_ie_ptr = &(rb_setup_ptr->elem[rb_cnt]);
           break;
        }
      }
     
      rb_cnt++;
    } /* end RB per RAB setup */
    
    if ((rb_setup_ptr->n == 0) || (rb_cnt >= MAX_RB_PER_RAB) || (rb_cnt >= rb_setup_ptr->n))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to match rb_id to same_as_RB");  
      return FAILURE;
    }
  } /* if RLC info choice is "Same as RB" type */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_r7_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode))||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)&&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL or UL or Both RLC mode info not provided for rb id %d",local_rb_id);
      return FAILURE;
    }
  
  } 
    /* Now we have either the RLC Info explicitly meant for this RB OR
    * the RLC Info that is the same as another RB.Get the RLC mode */
  
    /* Now we have either the RLC Info explicitly meant for this RB OR
    * the RLC Info that is the same as another RB.Get the RLC mode */
  
  if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info) && 
       ((logch_mode = tdsrrcllc_get_rlc_mode_r7(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
     )
    {
      return FAILURE;
    }
    
    /* Save off the RLC Info */
  if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info) && 
      (tdsrrcllc_get_rlc_info_r7(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, local_rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }
    
  if (temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info)
  {
    one_sided_reest_present = TRUE;
    one_sided_reest = temp_ie_ptr->rlc_InfoChoice.u.rlc_Info->rlc_OneSidedReEst;
  }
  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info_r7(&ie_ptr->rb_MappingInfo, local_rb_id, logch_mode
       ,one_sided_reest_present,one_sided_reest,TRUE
      ) == FAILURE)
  {
    return FAILURE;
  }

#ifdef FEATURE_UMTS_PDCP

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r7,pdcp_Info))
  {
    if (tdsrrc_pdcp_enabled)  
    { 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDPC info present in rb setup");
      if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,&ie_ptr->pdcp_Info, 
        local_rb_id, TDSCPDCP_SETUP
      ) == FAILURE)
      {
        return FAILURE;
      }
      return SUCCESS;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r7,pdcp_Info)) &&
            ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
            (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
            (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,
            rrc_LosslessSRNS_RelocSupport_supported)))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP NV not enabled.  PDCP info not handled");
      return FAILURE;
    }
  }

  /* if fall-thru happens, PDCP layer has to be setup in transparent [pass thru] mode */
  if (cn_Domain == tdsrrc_CN_DomainIdentity_ps_domain) 
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting up PDCP in transparent mode rb_id %d",local_rb_id);
    /*Get the next free index in pdcp_config_req */
    while (idx < MAX_RAB_TO_SETUP) 
    {
      if (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use == FALSE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"pdcp config req index is %d",idx);
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].rb_id = local_rb_id;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.action = SETUP_PDCP;
        tdsordered_config_ptr->pdcp_parms.num_rbs++;
        break;
      }
      else
      {
        idx++;
      }
    }
    /* CPDCP action will be setup, but pdcp info will be null */
    //tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
  }
#else
  /* Ignore the PDCP info for now */
  /********************************/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r7,pdcp_Info)) &&
            ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
            (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
            (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,
            rrc_LosslessSRNS_RelocSupport_supported)))))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }
#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

}
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
  tdsrrc_RB_InformationReconfigList_r7 * rb_reconfig_ptr
)  
{

  if (rb_info_reconfig_list_present)
  {
   
    uint32 idx = 0;
   
    idx = 0;
    while((rb_reconfig_ptr->n > idx) && (idx < TDS_MAX_RB))
    {
      if(tdsrrcllc_get_rb_info_to_reconfig_r7(&rb_reconfig_ptr->elem[idx]) == FAILURE)
      {
        return FAILURE;
      }
     
      idx++;
    } /* end RB reconfig */
  }

  return(SUCCESS);
}

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
  tdsrrc_RB_InformationAffectedList_r7 * rb_affected_ptr
)  
{
  /* Get the affected RB list if present */
  if(rb_info_affected_list_present)
  {
    
    uint32 idx = 0;
  
   
    
    while((rb_affected_ptr->n > idx) && (idx < TDS_MAX_RB))
    {
      if(tdsrrcllc_get_rb_info_affected_r7(&rb_affected_ptr->elem[idx]) == FAILURE)
      {
        return (FAILURE);
      }
     
      idx++;
    }
  } /* end Affected RB */

  return(SUCCESS);
}



/*==========================================================
FUNCTION  : tdsrrcllc_get_srb_info_to_setup_r7()

DESCRIPTION : Get Signalling Radio Bearer Information to setup.

REUSABILITY: R6

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_srb_info_to_setup_r7
(
  tdsrrc_SRB_InformationSetup_r7     *ie_ptr,
  tdsrrc_SRB_InformationSetupList_r7  *srb_setup_ptr,
  tdsrrc_RB_Identity                   rb_id
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  
  /* Local count of # of SRB's to setup */
  uint8                           srb_cnt = 0;
  
  /* temp rb id which we shall compute if rb_id is not part of
  * srb_setup_ptr when we loop thru the list to get a match
  */
  tdsrrc_RB_Identity                      temp_rb_id = 0;
  

  
  /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_SRB_InformationSetup_r7     *temp_ie_ptr = ie_ptr;
  
  /* Entity indexes in case we need to look up RLC info from
  * our internal database instead of processing an IE.
  */
  uint8 entity_idx, new_entity_idx;
  
  boolean  one_sided_reest_present=FALSE;
  boolean  one_sided_reest =FALSE;

  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
  *rlc_Info for the rb_id specified.  Walk thru the srb_setup_head_ptr
  *list until we find a rb_id that matches
  */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r7_same_as_RB))
  {
    while ((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      
      /* Check to see if the RB ID is there ... */
      if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        temp_rb_id = (tdsrrc_RB_Identity)srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }
      
      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        temp_rb_id++;
      }
      
      /*check if the rb_id matches*/
      if (ie_ptr->rlc_InfoChoice.u.same_as_RB == temp_rb_id)
      {
        if(srb_setup_ptr->elem[srb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info)
        {
          temp_ie_ptr = &(srb_setup_ptr->elem[srb_cnt]);
          break;
        }
      }
      
      srb_cnt++;
    } /* end while... */
    
    if ((srb_setup_ptr->n == 0) || (srb_cnt >= UE_MAX_SRB_SETUP) || (srb_cnt >= srb_setup_ptr->n))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"same_as_RB %d not in msg,lookup req'd",ie_ptr->rlc_InfoChoice.u.same_as_RB);
      temp_ie_ptr = NULL;
      temp_rb_id = (tdsrrc_RB_Identity)ie_ptr->rlc_InfoChoice.u.same_as_RB;
      
      if ((entity_idx = tdsrrc_get_am_index(temp_rb_id)) != TDSUE_MAX_AM_ENTITY)
      {
      /* This RB is in AM mode. And its reference config is found in OC.
        */
        if ((new_entity_idx = tdsrrcllc_get_1st_hole_for_rlc_am()) == TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't find space for new rb %d",rb_id);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_ul_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->common_config.rb_id = rb_id;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan >
          TDSUE_MAX_UL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many UL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_dl_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan >
          TDSUE_MAX_DL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many DL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->rlc_am_parms.nentity++;
        if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many AM entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
          return FAILURE;
        }
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
      }
      
      if ((entity_idx = tdsrrcllc_get_ul_um_index(temp_rb_id))
        != TDSUE_MAX_UL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_ul_um_info(rb_id, TRUE) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      
      if ((entity_idx = tdsrrcllc_get_dl_um_index(temp_rb_id, tdsordered_config_ptr))
        != TDSUE_MAX_DL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_dl_um_info(rb_id, TRUE, tdsordered_config_ptr) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      
      /* SRBs cannot be TM.  If we haven't found it by now, we have
      * a problem. */
      if (logch_mode == UE_MODE_MAX_NUM)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"same_as_RB %d not found in msg or OC",ie_ptr->rlc_InfoChoice.u.same_as_RB);
        return FAILURE;
      }
    }
  } /* end RLC choice == Same as RB */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_r7_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode))||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)&&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)))
    {
      ERR("DL or UL or Both RLC mode info not provided for rb id %d", rb_id, 0, 0);
      return FAILURE;
    }
  
  } 

  /* Now we have either the RLC Info explicitly meant for this RB OR
   * the RLC Info that is the same as another RB.  If tmp_ie_ptr is NULL,
   * then we've already copied the RLC Info from our internal database.
   */
  if (temp_ie_ptr != NULL)
  {
    /* Get the RLC mode */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info) && 
         ((logch_mode = tdsrrcllc_get_rlc_mode_r7(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
       )
    {
      return FAILURE;
    }
      
    /* Get the RLC info */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info) && 
         (tdsrrcllc_get_rlc_info_r7(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }
  }

  if ((temp_ie_ptr != NULL) && (temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info))
  {
    one_sided_reest_present=TRUE;
    one_sided_reest=temp_ie_ptr->rlc_InfoChoice.u.rlc_Info->rlc_OneSidedReEst;
  }
  
  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info_r7(&ie_ptr->rb_MappingInfo, rb_id, logch_mode
       ,one_sided_reest_present,one_sided_reest,TRUE
      ) == FAILURE)
  {
    return FAILURE;
  }
  
  if(FAILURE == tdsrrcllc_validate_srb_id(rb_id))
  {
    return FAILURE;
  }

  /* everything is configured */
  return SUCCESS;

}
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
  tdsrrc_SRB_InformationSetupList_r7 * srb_setup_r7_ptr
)
{
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrc_SRB_InformationSetup_r7 * srb5_setup_ptr = NULL;
  tdsrrc_RB_Identity                          srb_id = TDSRRCLLC_INVALID_RB_ID;
#endif/* FEATURE_TDSCDMA_SRB5_SUPPORT */

  if (srb_info_setup_list_present)
  {
    uint8                           srb_cnt =0;
    tdsrrc_RB_Identity                          rb_id = 0;
    
  

    
    while((srb_setup_r7_ptr->n > srb_cnt ) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      /* Check to see if the RB ID is there ... */
      if(srb_setup_r7_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        rb_id = (tdsrrc_RB_Identity)srb_setup_r7_ptr->elem[srb_cnt].rb_Identity;
      }
      
      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        rb_id++;
      }
      
      /* Save off the RB Mapping info for each SRB */
      if(tdsrrcllc_get_srb_info_to_setup_r7(
        &srb_setup_r7_ptr->elem[srb_cnt], 
        (srb_setup_r7_ptr), 
        rb_id)
        == FAILURE)
      {
        return FAILURE;
      }
  #ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      if(rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        /* Pick that pointer that has RB Id > 4 & 
         assume it to be SRB#5 pointer for the time being */

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Probable SRB#5 IE in the message .. processing");
        srb5_setup_ptr = &srb_setup_r7_ptr->elem[srb_cnt];

        srb_id = rb_id;
      }
#else
      if(rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Establishment of SRB greater than ID 4 is not supported");
        return FAILURE;
      }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
    
    
      srb_cnt++;
    } /* end SRB setup */

  }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  /* We are just validating one more SRB, incase 
     SRB#5 is given by n/w */
  if(srb5_setup_ptr != NULL)
  {
  
    if(tdsrrcllc_update_oc_with_srb5_rel7_ie(srb5_setup_ptr, srb_id) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rejecting SRB#5 configuration given by the network");
      return FAILURE;
    }
  }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */  
#ifdef FEATURE_TDSCDMA_CHANGE_SRB4_PRIRORITY
  tdsrrcllc_check_and_set_srb3_srb4_priority();
#endif

  return(SUCCESS);
}

/*============================================================================
FUNCTION: tdsrrcllc_process_rab_info_setup_list_r7()

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
  tdsrrc_RAB_InformationSetupList_r7 * rab_setup_ptr
)
{
  if(rab_info_setup_list_present)
  {
    /* Local count of # of RAB's & RB's to setup */
    uint32                             rab_cnt = 0;
    uint32                             rb_cnt  = 0;    
   
    tdsrrc_RB_InformationSetupList_r7     * rb_setup_ptr;
  
    
    while((rab_setup_ptr->n > rab_cnt) && (rab_cnt < MAX_RAB_TO_SETUP))
    {
      rb_setup_ptr  = &rab_setup_ptr->elem[rab_cnt].rb_InformationSetupList;
      
      rb_cnt =0;
      
      while((rb_setup_ptr->n > rb_cnt) && (rb_cnt < MAX_RB_PER_RAB))
      {
        if(tdsrrcllc_get_rb_info_to_setup_r7(
           &rb_setup_ptr->elem[rb_cnt],
#ifdef FEATURE_UMTS_PDCP
           rab_setup_ptr->elem[rab_cnt].rab_Info.cn_DomainIdentity,
#endif /* FEATURE_UMTS_PDCP */           
           rb_setup_ptr) == FAILURE)
        {
          return FAILURE;
        }
       
        rb_cnt++;
      } /* end RB per RAB setup */
      
      
      rab_cnt++;
    } /* end rab setup */
  } /* end RAB setup */

  return(SUCCESS);
}




#ifdef FEATURE_TDSCDMA_REL8
/*==========================================================
FUNCTION  : tdsrrcllc_update_ul_ded_log_chl_info_r8()

DESCRIPTION : Processes UL Logical channel info and fills it
              in passed pointer to MAC I/F struct.

DEPENDENCIES: None.

RETURN VALUE: None

SIDE EFFECTS: None.
============================================================*/
static void  tdsrrcllc_update_ul_ded_log_chl_info_r8
 (
  tdsrrc_UL_LogicalChannelMapping_r8 *ie_ptr, 
  tdsmac_ul_ded_logch_config_type  *dlchan_info,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
 )
{
  /* If Logical Channel ID is present, save MAC Logical Channel Identity */ 
  if(ie_ptr->ul_TrCH_Type.u.dch_rach_usch->m.logicalChannelIdentityPresent)
  {
    dlchan_info->mac_id =
      (uint8)ie_ptr->ul_TrCH_Type.u.dch_rach_usch->logicalChannelIdentity;
  }
  else
  {
    dlchan_info->mac_id = 
      TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT;
  }

  /* MAC Logical Channel Priority */
  dlchan_info->priority =
    (uint8)ie_ptr->mac_LogicalChannelPriority;

  /* Save Radio Bearer ID */
  dlchan_info->rb_id = rb_id;

  /* Determine the type of Logical Channel mapped to DCH TrCH */
  if(rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    dlchan_info->chan_type = UE_LOGCHAN_DCCH;
  }
  else
  {
    dlchan_info->chan_type = UE_LOGCHAN_DTCH;
  }

  /* Save the logical channel mode, if a setup */
  if(logch_mode != UE_MODE_MAX_NUM)
  {
    dlchan_info->rlc_mode = logch_mode;
  }
}  /* tdsrrcllc_update_ul_ded_log_chl_info */

/*==========================================================
FUNCTION  : tdsrrcllc_get_rlc_size_list_r8()

DESCRIPTION : Processes the RLC Size List IE contents and saves 
              them in the RLC size restriction database.  

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: The RLC size restriction database is updated.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rlc_size_list_r8
(
  tdsrrc_UL_LogicalChannelMapping_r8 *ie_ptr,
  tdsrrcllcoc_rlc_size_list_type  *rlc_size_rec,
  tdsrrc_RB_Identity                    rb_id
)
{
  tdsrrc_RLC_SizeExplicitList *rlc_size_list = NULL;

  rlc_size_rec->rb_id = rb_id;
  switch(ie_ptr->ul_TrCH_Type.u.dch_rach_usch->rlc_SizeList.t)
  {
    case T_tdsrrc_UL_LogicalChannelMapping_r8_rlc_SizeList_allSizes:
      rlc_size_rec->restriction_type = TDSRLC_SIZES_ALL;
      break;

    case T_tdsrrc_UL_LogicalChannelMapping_r8_rlc_SizeList_configured:
      rlc_size_rec->restriction_type = TDSRLC_SIZES_CONFIGURED;
      break;

    case T_tdsrrc_UL_LogicalChannelMapping_r8_rlc_SizeList_explicitList:
      rlc_size_rec->restriction_type = TDSRLC_SIZES_EXPLICIT_LIST;
      rlc_size_list = ie_ptr->ul_TrCH_Type.u.dch_rach_usch->rlc_SizeList.u.explicitList;
      if(rlc_size_list == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RLC Size List empty");
        return(FAILURE);
      }
      rlc_size_rec->count = 0;
      while(rlc_size_list->n > rlc_size_rec->count)
      {
        rlc_size_rec->size_idx[rlc_size_rec->count] = 
			(uint16)rlc_size_list->elem[rlc_size_rec->count].rlc_SizeIndex;
        rlc_size_rec->count++;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RLC Size List CHOICE: %d",ie_ptr->ul_TrCH_Type.u.dch_rach_usch->rlc_SizeList.t);
      return(FAILURE);
  }

  return SUCCESS;
}

/*==========================================================
FUNCTION  : tdsrrcllc_get_rach_mapping_info_r8()

DESCRIPTION : Process RACH Mapping IE's contents and saves 
              them in the MAC Ordered Configuration Database.
              If next RRC state is CELL_DCH, save the RACH
              mapping contents in a local data structure in
              OC.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rach_mapping_info_r8
(
  tdsrrc_UL_LogicalChannelMapping_r8 *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode,
  tdsrrc_state_e_type             next_rrc_state
)
{
  /* transport Channel Index */
  uint8   trch_idx  = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Get the logical channel index */
  uint8   logch_idx;
 
  tdsmac_ul_ded_logch_config_type  *dlchan_info;
  rlc_lc_id_type   rlc_lc_id = (uint8)TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
  /* TBD: Although all existing FACHs would be mapped to each of the RBs,
   * fill up only for the 1st FACH - till MAC interface is clarified/updated
   */

  boolean update_rach_rlc_id = FALSE;

  /* Temporary pointer for processing the RLC Size List */
  tdsrrcllcoc_rlc_size_list_type *rlc_size_rec = NULL;


  /* Start with updating Cell FACH RB Mapping Info when the next state is
   * either CELL_DCH or CELL_FACH
   */
  logch_idx = tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan;
  dlchan_info = &tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_dlchan_info[logch_idx];

  /* Save the RLC Size List information */
  tdsordered_config_ptr->rach_rlc_size_restriction_info.mac_update_needed = TRUE;
  rlc_size_rec =
    &(tdsordered_config_ptr->rach_rlc_size_restriction_info.rlc_size_list[logch_idx]);
  if(tdsrrcllc_get_rlc_size_list_r8(ie_ptr, rlc_size_rec, rb_id) == FAILURE)
  {
    return FAILURE;
  }


  if(logch_mode == UE_MODE_MAX_NUM)
  {
    /* This is the case when RB mapping info applicable to CELL_FACH is coming 
    * in "Affected List". At this point, we need to update the Logical Channel mode 
    * and Logical Channel ID in Cell FACH RB Mapping Info - 
    * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
    */
    rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
    if(rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid UL LC ID for RB %d",rb_id);
      return(FAILURE);
    }
    
    if(tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get UL Log Chl mode for LC %d",rlc_lc_id);
    }
    /* Also update the Logical Channel ID here */
    dlchan_info->rlc_id = rlc_lc_id;
    update_rach_rlc_id = TRUE;
  }
  else
  {
    rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id(rb_id);
    if(rlc_lc_id  != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      /* Also update the Logical Channel ID here */
      dlchan_info->rlc_id = rlc_lc_id;
      update_rach_rlc_id = TRUE;
    }
  }
  
  tdsrrcllc_update_ul_ded_log_chl_info_r8(ie_ptr, dlchan_info, rb_id, logch_mode);
  tdsordered_config_ptr->cell_fach_rb_mapping_info.ul_ndlchan ++;  
   
  /* Now update the FACH Info ONLY IF the next state is CELL_FACH */
  if(next_rrc_state == TDSRRC_STATE_CELL_FACH ||
     next_rrc_state == TDSRRC_STATE_CELL_PCH ||
     next_rrc_state == TDSRRC_STATE_URA_PCH)
  {
    if((trch_idx = tdsrrcllc_find_ul_rach_trch_idx()) >= TDSUE_MAX_TRCH)
    {
      return FAILURE;
    }
    logch_idx = tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan;
    dlchan_info = &tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx];
    if(update_rach_rlc_id)
    {
      dlchan_info->rlc_id = rlc_lc_id;
    }
    tdsrrcllc_update_ul_ded_log_chl_info_r8(ie_ptr, dlchan_info, rb_id, logch_mode);

    tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan++;
    tdsordered_config_ptr->ul_rach_trch_info[trch_idx].lg_ch_cnt = 
      tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan;

  }  /* If next state is CELL_FACH, update the FACH Info */
 
  return SUCCESS;

}



/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_dch_mapping_info_r8()

DESCRIPTION : Process UL DCH Mapping IE's contents and saves 
              them in the MAC Ordered Configuration Database.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_dch_mapping_info_r8
(
  tdsrrc_UL_LogicalChannelMapping_r8 *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
)
{
  /* transport Channel Index */
  uint8         trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;

  /* Transport Channel ID */
  tr_ch_id_type   tdstrch_id = 0;

  /* Get the logical channel index */
  uint8         logch_idx;
  rlc_lc_id_type   rlc_lc_id;
  uint8 cnt = 0;


  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_TrCH_Type,
  	  rrc_UL_LogicalChannelMapping_r8_ul_TrCH_Type_dch_rach_usch))
  {
    /* Get the UL DCH TrCH ID if it exists */
    switch (ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.t)
    {
      case T_tdsrrc_UL_TransportChannelType_dch:
        tdstrch_id = (tr_ch_id_type)
          ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.u.dch;
        break;
  
      case T_tdsrrc_UL_TransportChannelType_rach:
      case T_tdsrrc_UL_TransportChannelType_dummy:
      case T_tdsrrc_UL_TransportChannelType_usch:
      default:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Expecting UL DCH TrCH ID");
        return FAILURE;
    } /* end UL DCH TrCH ID switch */
  
    /* Get the transport channel index for this transport channel ID.
     * Note: The index should already be there.
     */
    trch_idx = tdsrrcllc_find_ul_dch_trch_idx(tdsordered_config_ptr, tdstrch_id);
    if(trch_idx >= TDSUE_MAX_TRCH)
    {
      return(tdsrrcllc_backup_ul_dch_from_ota_r8(ie_ptr,rb_id,logch_mode));
    }
  
    /* Get the Logical Channel Index */
    logch_idx = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan;
  
    for (cnt = 0; cnt < tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan; cnt++) 
    {
      if (rb_id == tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnt].rb_id)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RB_ID %d already present",rb_id);
        return (SUCCESS); 
      }
    }

    /* If Logical Channel ID is present, save MAC Logical Channel Identity */
    if (ie_ptr->ul_TrCH_Type.u.dch_rach_usch->m.logicalChannelIdentityPresent)
    {
      
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].mac_id =
        (uint8)ie_ptr->ul_TrCH_Type.u.dch_rach_usch->logicalChannelIdentity;
    }
    else 
    {
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].mac_id =
        TDSRRCLLC_MAC_LOG_CHAN_ID_NOT_PRESENT;
    }

    /* Initialize the MAC data structures to allow all RLC sizes, just in case. */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rlc_size_info.all_tfi_valid = TRUE;

    /* MAC Logical Channel Priority */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].priority =
      (uint8) ie_ptr->mac_LogicalChannelPriority;
  
    /* Save Radio Bearer ID */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rb_id = rb_id;
  
    /* Determine the type of Logical Channel mapped to DCH TrCH */
    if(rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
    {
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
    }
    else
    {
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
    }
  
    if(logch_mode == UE_MODE_MAX_NUM)
    {
      /* This is the case when RB mapping info applicable to CELL_FACH is coming 
       * in "Affected List". At this point, we need to update the Logical Channel mode 
       * and Logical Channel ID in Cell FACH RB Mapping Info - 
       * so find the real mode from DCH Info and send that in place of UE_MODE_MAX_NUM
       */
      rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);
      if(rlc_lc_id  == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid UL LC ID for RB %d",rb_id);
        return(FAILURE);
      }
    
      if(tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Failure to get UL Log Chl mode for LC %d",rlc_lc_id);
      }
  
      /* Also update the Logical Channel ID here */
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rlc_id = rlc_lc_id;
    }
    
    /* Save the logical channel mode, if a setup */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[logch_idx].rlc_mode = logch_mode;
  
    /* other wise it is just a reconfig of RB Mapping info */
  
    /* Increment number of logical channels */
    tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan++;
    
    /* The logic channel mapped to this TrCH increases from 1 to 2. */
    /* Need to update the TB size to reflect this change.           */
    if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan == 2 &&
      tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt == 1)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TrCH %d Need to increase TB size",tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].trch_id);
      tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt_up = TRUE;
    }
    tdsordered_config_ptr->ul_dch_trch_info[trch_idx].lg_ch_cnt = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan;
  }
  else /* rrc_ul_TrCH_Type_e_dch_chosen */
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: R6 ASN1 rrc_ul_TrCH_Type_e_dch_chosen not yet supported");
  }
  tdsrrcllc_free_hanging_rb_mapping_info_index(rb_id,TDSUL_DCH_RB_MAPPING);

  return SUCCESS;

}

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_single_logical_channel_info_r8()

DESCRIPTION : Process one UL Logical Channel Mapping IE's 
              contents and saves them in the MAC Ordered 
              Configuration Database.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_single_logical_channel_info_r8
(
  tdsrrc_UL_LogicalChannelMapping_r8 *ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
 ,boolean one_sided_reest_present
 ,boolean one_sided_reest
 ,boolean allow_new_channels
)
{
  tdsordered_config_type *tmp_config_ptr = NULL;

  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_TrCH_Type,
  	  rrc_UL_LogicalChannelMapping_r8_ul_TrCH_Type_dch_rach_usch))
  {
    /* Transport channel type */
    switch (ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.t)
    {
      case T_tdsrrc_UL_TransportChannelType_dch:
        if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
        {
          if (tdsrrcllc_get_ul_dch_mapping_info_r8(ie_ptr, rb_id, logch_mode) == FAILURE)
          {
            return FAILURE;
          }
        }
        else
        {
            if(tdstransition_config.toc_usage == TDSTOC_INVALID)
            {
              tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
            }
  
            tmp_config_ptr = tdsordered_config_ptr;
            tdsordered_config_ptr = tdstransition_config.toc_ptr;
  
            /* Processing functions must think we are going to Cell_DCH state. */
            tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
  
            if(tdsrrcllc_get_ul_dch_mapping_info_r8(ie_ptr, rb_id, logch_mode) == FAILURE)
            {
              tdsordered_config_ptr = tmp_config_ptr;
              tmp_config_ptr = NULL;
              return FAILURE;
            }
  
            tdsordered_config_ptr = tmp_config_ptr;
            tmp_config_ptr = NULL;
        }
        break;
  
      case T_tdsrrc_UL_TransportChannelType_rach:
        if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_FACH ||
           tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_PCH ||
           tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_URA_PCH)
        {
          if (tdsrrcllc_get_rach_mapping_info_r8(ie_ptr, rb_id, logch_mode,
                              tdsordered_config_ptr->rrc_state_indicator) == FAILURE)
          {
            return FAILURE;
          }
        }
        else
        {
          if (tdsrrcllc_get_rach_mapping_info_r8(ie_ptr, rb_id, logch_mode, TDSRRC_STATE_CELL_DCH) == FAILURE)
          {
            return FAILURE;
          }
        }
        break;
  
      case T_tdsrrc_UL_TransportChannelType_dummy:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"DUMMY chosen by NW");
        return FAILURE;
  
      case T_tdsrrc_UL_TransportChannelType_usch:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"USCH not supported");
        return FAILURE;
  
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd invalid UL TrCh type %d",ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.t);
        return FAILURE;
    }
  }
#ifdef FEATURE_TDSCDMA_HSUPA
  else if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->ul_TrCH_Type,
  	rrc_UL_LogicalChannelMapping_r8_ul_TrCH_Type_e_dch))/* rrc_ul_TrCH_Type_e_dch_chosen */
  {
    if(tdsordered_config_ptr->rrc_state_indicator == TDSRRC_STATE_CELL_DCH)
    {
      if (logch_mode != UE_MODE_TRANSPARENT) 
      {
        if (tdsrrcllc_get_edch_mapping_info_r8(ie_ptr, rb_id, logch_mode
            ,one_sided_reest_present,one_sided_reest,allow_new_channels
           ) == FAILURE)
        {
          return FAILURE;
        }
        tdsrrcllc_update_mac_eul_action(TRUE);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot map TM RB on EDCH");
        return FAILURE;
      }
    }
    else
    {
      if(tdstransition_config.toc_usage == TDSTOC_INVALID)
      {
        tdstransition_config.toc_usage = TDSTOC_FOR_DCH;
      }
      
      tmp_config_ptr = tdsordered_config_ptr;
      tdsordered_config_ptr = tdstransition_config.toc_ptr;
      
      /* Processing functions must think we are going to Cell_DCH state. */
      tdsordered_config_ptr->rrc_state_indicator = TDSRRC_STATE_CELL_DCH;
      
      if (logch_mode != UE_MODE_TRANSPARENT) 
      {
        if (tdsrrcllc_get_edch_mapping_info_r8(ie_ptr, rb_id, logch_mode
            , one_sided_reest_present,one_sided_reest,allow_new_channels
            ) == FAILURE)
        {
          tdsordered_config_ptr = tmp_config_ptr;
          tmp_config_ptr = NULL;  
          return FAILURE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot map TM RB on EDCH");
        tdsordered_config_ptr = tmp_config_ptr;
        tmp_config_ptr = NULL;  
        return FAILURE;
      }
      
      tdsordered_config_ptr = tmp_config_ptr;
      tmp_config_ptr = NULL;  
    }
  }
#else
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCEUL: R8 ASN1 rrc_ul_TrCH_Type_e_dch_chosen not yet supported");
    return FAILURE;
  }
#endif /* FEATURE_TDSCDMA_HSUPA */

  return SUCCESS;

}

/*==========================================================
FUNCTION  : tdsrrcllc_get_ul_logical_channel_info_r8()

DESCRIPTION : Get UL logical Channel info.

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_ul_logical_channel_info_r8
(
  tdsrrc_UL_LogicalChannelMappings_r8 * ie_ptr,
  tdsrrc_RB_Identity                    rb_id,
  uecomdef_logch_mode_e_type    logch_mode
 ,boolean one_sided_reest_present
 ,boolean one_sided_reest
 ,boolean allow_new_channels
)
{

  /* Process IE info based on number of UL Logical Channels */
  switch (ie_ptr->t)
  {
    /* One UL Logical Channel */
    case T_tdsrrc_UL_LogicalChannelMappings_r8_oneLogicalChannel:
      if(tdsrrcllc_get_ul_single_logical_channel_info_r8(
         ie_ptr->u.oneLogicalChannel, rb_id, logch_mode
         , one_sided_reest_present, one_sided_reest,allow_new_channels
         ) == FAILURE)
      {
        return FAILURE;
      }
      break;

    /* Two logical channels, currently not supported by RRC-RLC I/F */
    case T_tdsrrc_UL_LogicalChannelMappings_r8_twoLogicalChannels:
      /* TRUE indicates that first logical channel is for Data PDUs and second  
         logical channel is for control PDUs */
      /* FALSE indicates that Data and Control PDUs can be sent on either of them */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Support only 1 Log CH per AM");
        return FAILURE;

      /* Invalid number of logical channels */
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid number of UL logical channels %d",ie_ptr->t );
        return FAILURE;
  } /* end switch */

  return SUCCESS;

}

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
  tdsrrc_SRB_InformationSetupList_r8 * srb_setup_r8_ptr
)
{
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  tdsrrc_SRB_InformationSetup_r8 * srb5_setup_ptr = NULL;
  tdsrrc_RB_Identity                          srb_id = TDSRRCLLC_INVALID_RB_ID;
#endif/* FEATURE_TDSCDMA_SRB5_SUPPORT */

  if (srb_info_setup_list_present)
  {
    uint8                           srb_cnt =0;
    tdsrrc_RB_Identity                          rb_id = 0;
   
    
    while((srb_setup_r8_ptr->n > srb_cnt ) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      /* Check to see if the RB ID is there ... */
      if(srb_setup_r8_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        rb_id = (tdsrrc_RB_Identity)srb_setup_r8_ptr->elem[srb_cnt].rb_Identity;
      }
      
      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        rb_id++;
      }
      
      /* Save off the RB Mapping info for each SRB */
      if(tdsrrcllc_get_srb_info_to_setup_r8(
        &srb_setup_r8_ptr->elem[srb_cnt], 
        (srb_setup_r8_ptr), 
        rb_id)
        == FAILURE)
      {
        return FAILURE;
      }
  #ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
      if(rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        /* Pick that pointer that has RB Id > 4 & 
         assume it to be SRB#5 pointer for the time being */

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Probable SRB#5 IE in the message .. processing");
        srb5_setup_ptr = &srb_setup_r8_ptr->elem[srb_cnt];

        srb_id = rb_id;
      }
#else
      if(rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Establishment of SRB greater than ID 4 is not supported");
        return FAILURE;
      }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
    
      srb_cnt++;
    } /* end SRB setup */

  }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  /* We are just validating one more SRB, incase 
     SRB#5 is given by n/w */
  if(srb5_setup_ptr != NULL)
  {
  
    if(tdsrrcllc_update_oc_with_srb5_rel8_ie(srb5_setup_ptr, srb_id) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Rejecting SRB#5 configuration given by the network");
      return FAILURE;
    }
  }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */  
#ifdef FEATURE_TDSCDMA_CHANGE_SRB4_PRIRORITY
    tdsrrcllc_check_and_set_srb3_srb4_priority();
#endif

  return(SUCCESS);
}
/*==========================================================
FUNCTION  : tdsrrcllc_get_rb_mapping_info_r8()

DESCRIPTION : Get Radio Bearer Mapping Info IE

DEPENDENCIES: None.

RETURN VALUE: SUCCESS or FAILURE

SIDE EFFECTS: None.
============================================================*/
static uecomdef_status_e_type tdsrrcllc_get_rb_mapping_info_r8
(
tdsrrc_RB_MappingInfo_r8      * ie_ptr,
tdsrrc_RB_Identity                 rb_id,
uecomdef_logch_mode_e_type logch_mode
,boolean one_sided_reest_present
,boolean one_sided_reest
,boolean allow_new_channels
)
{
  /*
  * Count to make sure that don't exceed the maximum number 
  * of RB Mux options
  */
  uint8 rb_mux_cnt = 0;

  /* Process the RB Mux option IE in the linked list */
  while ((ie_ptr->n > rb_mux_cnt) && (rb_mux_cnt < MAX_RB_MUX_OPTIONS_REL5))
  {
    /* Check for DL Channel Mapping List */
    if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[rb_mux_cnt],
		tdsrrc_RB_MappingOption_r8,dl_LogicalChannelMappingList))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for DL",rb_id);
      if (tdsrrcllc_get_dl_logical_channel_info_r7(&ie_ptr->elem[rb_mux_cnt].dl_LogicalChannelMappingList,
                                         rb_id, logch_mode
                                    ) == FAILURE)
      {
        return FAILURE;
      }
      /* Here indicate that MAC Downlink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.downlink_mac = TRUE;
    }  /* end dl Log Ch mapping list */

    if (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->elem[rb_mux_cnt],
		tdsrrc_RB_MappingOption_r8,ul_LogicalChannelMappings))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED,"Get RB %d Mapping Info for UL",rb_id);
      if (tdsrrcllc_get_ul_logical_channel_info_r8(&ie_ptr->elem[rb_mux_cnt].ul_LogicalChannelMappings,
                                      rb_id, logch_mode
                                    , one_sided_reest_present,one_sided_reest,allow_new_channels
         ) == FAILURE)
      {
        return FAILURE;
      }

      /* Here indicate that MAC Uplink needs to be reconfigured */
      tdsordered_config_ptr->reconfig_needed.uplink_mac = TRUE;
    }  /* end ul Log Ch mapping list */

    /* Pointer to the next item in the IE list */
  
    rb_mux_cnt++;

  }  /* end loop of RB Mapping info list */

  return SUCCESS;
}

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
  tdsrrc_SRB_InformationSetupList_r8 * srb_setup_ptr,
  tdsrrc_RB_Identity                   rb_id
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  
  /* Local count of # of SRB's to setup */
  uint8                           srb_cnt = 0;
  
  /* temp rb id which we shall compute if rb_id is not part of
  * srb_setup_ptr when we loop thru the list to get a match
  */
  tdsrrc_RB_Identity                      temp_rb_id = 0;
  
  
  /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_SRB_InformationSetup_r8     *temp_ie_ptr = ie_ptr;
  
  /* Entity indexes in case we need to look up RLC info from
  * our internal database instead of processing an IE.
  */
  uint8 entity_idx, new_entity_idx;
  
  boolean one_sided_reest_present=FALSE;
  boolean one_sided_reest=FALSE;

  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
  *rlc_Info for the rb_id specified.  Walk thru the srb_setup_head_ptr
  *list until we find a rb_id that matches
  */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,
    rrc_RLC_InfoChoice_r7_same_as_RB))
  {
    while ((srb_setup_ptr->n > srb_cnt) && (srb_cnt < UE_MAX_SRB_SETUP))
    {
      
      /* Check to see if the RB ID is there ... */
      if (srb_setup_ptr->elem[srb_cnt].m.rb_IdentityPresent)
      {
        temp_rb_id = (tdsrrc_RB_Identity)srb_setup_ptr->elem[srb_cnt].rb_Identity;
      }
      
      /*
      * ... if not, then start with the first SRB and get the next SRB ID
      * each time one is not included in the IE.
      */
      else
      {
        /* Get the next SRB, note this increments to 1 the first time */
        temp_rb_id++;
      }
      
      /*check if the rb_id matches*/
      if (ie_ptr->rlc_InfoChoice.u.same_as_RB == temp_rb_id)
      {
        if(srb_setup_ptr->elem[srb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info)
        {
          temp_ie_ptr = &(srb_setup_ptr->elem[srb_cnt]);
          break;
        }
      }
     
      srb_cnt++;
    } /* end while... */
    
    if ((srb_setup_ptr->n == 0) || (srb_cnt >= UE_MAX_SRB_SETUP) || (srb_cnt >= srb_setup_ptr->n))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"same_as_RB %d not in msg,lookup req'd",ie_ptr->rlc_InfoChoice.u.same_as_RB);
      temp_ie_ptr = NULL;
      temp_rb_id = (tdsrrc_RB_Identity)ie_ptr->rlc_InfoChoice.u.same_as_RB;
      
      if ((entity_idx = tdsrrc_get_am_index(temp_rb_id)) != TDSUE_MAX_AM_ENTITY)
      {
      /* This RB is in AM mode. And its reference config is found in OC.
        */
        if ((new_entity_idx = tdsrrcllc_get_1st_hole_for_rlc_am()) == TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Can't find space for new rb %d",rb_id);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_ul_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_ul_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->common_config.rb_id = rb_id;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan >
          TDSUE_MAX_UL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many UL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->am_config[new_entity_idx].rlc_dl_am_config.rlc_info =
          tdsordered_config_ptr->am_config[entity_idx].rlc_dl_am_config.rlc_info;
        tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan++;
        if(tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->dl_nchan >
          TDSUE_MAX_DL_AM_DATA_CHANNEL)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many DL AM channels %d",tdsordered_config_ptr->rlc_am_parms.am_info[new_entity_idx]->ul_nchan);
          return FAILURE;
        }
        tdsordered_config_ptr->rlc_am_parms.nentity++;
        if(tdsordered_config_ptr->rlc_am_parms.nentity > TDSUE_MAX_AM_ENTITY)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Too many AM entities %d",tdsordered_config_ptr->rlc_am_parms.nentity);
          return FAILURE;
        }
        logch_mode = UE_MODE_ACKNOWLEDGED_DATA;
      }
      
      if ((entity_idx = tdsrrcllc_get_ul_um_index(temp_rb_id))
        != TDSUE_MAX_UL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_ul_um_info(rb_id, TRUE) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      
      if ((entity_idx = tdsrrcllc_get_dl_um_index(temp_rb_id, tdsordered_config_ptr))
        != TDSUE_MAX_DL_UM_CHANNEL)
      {
        /* This RB is in UM mode */
        if(tdsrrcllc_get_rlc_dl_um_info(rb_id, TRUE, tdsordered_config_ptr) == FAILURE)
        {
          return FAILURE;
        }
		
        logch_mode = UE_MODE_UNACKNOWLEDGED;
      }
      
      /* SRBs cannot be TM.  If we haven't found it by now, we have
      * a problem. */
      if (logch_mode == UE_MODE_MAX_NUM)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"same_as_RB %d not found in msg or OC",ie_ptr->rlc_InfoChoice.u.same_as_RB);
        return FAILURE;
      }
    }
  } /* end RLC choice == Same as RB */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_r7_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode))||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)&&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL or UL or Both RLC mode info not provided for rb id %d",rb_id);
      return FAILURE;
    }
  
  } 

  /* Now we have either the RLC Info explicitly meant for this RB OR
   * the RLC Info that is the same as another RB.  If tmp_ie_ptr is NULL,
   * then we've already copied the RLC Info from our internal database.
   */
  if (temp_ie_ptr != NULL)
  {
    /* Get the RLC mode */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info) && 
         ((logch_mode = tdsrrcllc_get_rlc_mode_r7(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
       )
    {
      return FAILURE;
    }
      
    /* Get the RLC info */
    if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info) && 
        (tdsrrcllc_get_rlc_info_r7(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }
    
    if(temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info)
    {
    one_sided_reest_present = TRUE;
    one_sided_reest = temp_ie_ptr->rlc_InfoChoice.u.rlc_Info->rlc_OneSidedReEst;
    }
  }


  
  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info_r8(&ie_ptr->rb_MappingInfo, rb_id, logch_mode
       , one_sided_reest_present,one_sided_reest,TRUE
      ) == FAILURE)
  {
    return FAILURE;
  }
  
  if(FAILURE == tdsrrcllc_validate_srb_id(rb_id))
  {
    return FAILURE;
  }

  /* everything is configured */
  return SUCCESS;

}



 /* end tdsrrcllc_get_ul_logical_channel_info() */

 /* end tdsrrcllc_get_ul_single_logical_channel_info_r6() */

 /* end tdsrrcllc_get_ul_dch_mapping_info() */


 /* end tdsrrcllc_get_rlc_size_list_r6() */

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
  tdsrrc_RAB_InformationSetupList_r8 * rab_setup_ptr
)
{
  if(rab_info_setup_list_present)
  {
    /* Local count of # of RAB's & RB's to setup */
    uint32                             rab_cnt = 0;
    uint32                             rb_cnt  = 0;    
   
    tdsrrc_RB_InformationSetupList_r8     * rb_setup_ptr;
   
    
    while((rab_setup_ptr->n > rab_cnt) && (rab_cnt < MAX_RAB_TO_SETUP))
    {
      rb_setup_ptr  = &rab_setup_ptr->elem[rab_cnt].rb_InformationSetupList;
      rb_cnt =0;
 
      
      while((rb_setup_ptr->n > rb_cnt) && (rb_cnt < MAX_RB_PER_RAB))
      {
        if(tdsrrcllc_get_rb_info_to_setup_r8(
           &rb_setup_ptr->elem[rb_cnt],
#ifdef FEATURE_UMTS_PDCP
           rab_setup_ptr->elem[rab_cnt].rab_Info.cn_DomainIdentity,
#endif /* FEATURE_UMTS_PDCP */           
           rb_setup_ptr) == FAILURE)
        {
          return FAILURE;
        }
        
        rb_cnt++;
      } /* end RB per RAB setup */
      
      rab_cnt++;
    } /* end rab setup */
  } /* end RAB setup */

  return(SUCCESS);
}

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
  tdsrrc_RB_InformationSetupList_r8   * rb_setup_ptr
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type    logch_mode = UE_MODE_MAX_NUM;
  
  /*initialize a counter to check rb count*/
  uint8                         rb_cnt  = 0;

  /*place holder for rb_id we get originally*/
  tdsrrc_RB_Identity                    local_rb_id = (tdsrrc_RB_Identity)ie_ptr->rb_Identity;
  
  /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_RB_InformationSetup_r8     *temp_ie_ptr = ie_ptr;

#ifdef FEATURE_UMTS_PDCP
  uint8 idx = 0;
#endif /* FEATURE_UMTS_PDCP */  
  
  boolean one_sided_reest_present=FALSE;
  boolean one_sided_reest=FALSE;
  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
  *rlc_Info for the rb_id specified.  Walk thru the rb_setup_head_ptr
  *list until we find a rb_id that matches
  */
  if (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice,rrc_RLC_InfoChoice_r7_same_as_RB))
  {
    while ((rb_setup_ptr->n > rb_cnt) && (rb_cnt < MAX_RB_PER_RAB))
    {
      if (ie_ptr->rlc_InfoChoice.u.same_as_RB == 
        rb_setup_ptr->elem[rb_cnt].rb_Identity)
      {
        if(rb_setup_ptr->elem[rb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info)
        {
           temp_ie_ptr = &(rb_setup_ptr->elem[rb_cnt]);
           break;
        }
      }
      rb_cnt++;
    } /* end RB per RAB setup */
    
    if ((rb_setup_ptr->n == 0) || (rb_cnt >= MAX_RB_PER_RAB) || (rb_cnt >= rb_setup_ptr->n))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to match rb_id to same_as_RB");  
      return FAILURE;
    }
  } /* if RLC info choice is "Same as RB" type */
  else
  {
    /*Reject the configuration If (UL RLC info present && Mode is AM && DL RLC info not present)
    Or (DL RLC info present && Mode is AM && UL RLC info not present)
    Or (Both DL and UL RLC info not present) */
    if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->ul_RLC_Mode.t ==T_tdsrrc_UL_RLC_Mode_ul_AM_RLC_Mode) &&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)&&
        (ie_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t ==T_tdsrrc_DL_RLC_Mode_r7_dl_AM_RLC_Mode) &&
      !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode))||
      (!TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode)&&
         !TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)))
    {
      ERR("DL or UL or Both RLC mode info not provided for rb id %d", local_rb_id, 0, 0);
      return FAILURE;
    }
  
  } 
    /* Now we have either the RLC Info explicitly meant for this RB OR
    * the RLC Info that is the same as another RB.Get the RLC mode */
  
    /* Now we have either the RLC Info explicitly meant for this RB OR
    * the RLC Info that is the same as another RB.Get the RLC mode */
  
  if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info) && 
       ((logch_mode = tdsrrcllc_get_rlc_mode_r7(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
     )
    {
      return FAILURE;
    }
    
    /* Save off the RLC Info */
  if ((temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info) && 
      (tdsrrcllc_get_rlc_info_r7(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, 
        local_rb_id, TRUE) == FAILURE))
    {
      return FAILURE;
    }
  
  if(temp_ie_ptr->rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_r7_rlc_Info)
  {
    one_sided_reest_present = TRUE;
    one_sided_reest =temp_ie_ptr->rlc_InfoChoice.u.rlc_Info->rlc_OneSidedReEst;
  }  
  
  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info_r8(&ie_ptr->rb_MappingInfo, local_rb_id, logch_mode
     , one_sided_reest_present,one_sided_reest,TRUE
      ) == FAILURE)
  {
    return FAILURE;
  }

#ifdef FEATURE_UMTS_PDCP
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r8,pdcp_Info))
  {
    if (tdsrrc_pdcp_enabled)  
    { 
      TDSRRC_MSG0(MSG_LEGACY_MED,"PDPC info present in rb setup");
      if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,&ie_ptr->pdcp_Info, 
        local_rb_id, TDSCPDCP_SETUP
      ) == FAILURE)
      {
        return FAILURE;
      }
      return SUCCESS;
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r8,pdcp_Info)) &&
            ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
            (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
            (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,
            rrc_LosslessSRNS_RelocSupport_supported)))))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP NV not enabled.  PDCP info not handled");
      return FAILURE;
    }
  }

  /* if fall-thru happens, PDCP layer has to be setup in transparent [pass thru] mode */
  if (cn_Domain == tdsrrc_CN_DomainIdentity_ps_domain) 
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting up PDCP in transparent mode rb_id %d",local_rb_id);
    /*Get the next free index in pdcp_config_req */
    while (idx < MAX_RAB_TO_SETUP) 
    {
      if (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use == FALSE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"pdcp config req index is %d",idx);
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].rb_id = local_rb_id;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.action = SETUP_PDCP;
        tdsordered_config_ptr->pdcp_parms.num_rbs++;
        break;
      }
      else
      {
        idx++;
      }
    }
    /* CPDCP action will be setup, but pdcp info will be null */
    //tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
  }
#else
  /* Ignore the PDCP info for now */
  /********************************/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r8,pdcp_Info)) &&
            ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
            (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
            (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,
            rrc_LosslessSRNS_RelocSupport_supported)))))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }
#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

}

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
  tdsrrc_RB_InformationReconfigList_r8 * rb_reconfig_ptr
)  
{

  if (rb_info_reconfig_list_present)
  {
   
    uint32 idx = 0;
    
    idx = 0;
    while((rb_reconfig_ptr->n > idx) && (idx < TDS_MAX_RB))
    {
      if(tdsrrcllc_get_rb_info_to_reconfig_r8(&rb_reconfig_ptr->elem[idx]) == FAILURE)
      {
        return FAILURE;
      }
  
      idx++;
    } /* end RB reconfig */
  }

  return(SUCCESS);
}

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
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;
  
  boolean one_sided_reest_present=FALSE;
  boolean one_sided_reest=FALSE;
  #ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
  uint8 entity_idx = 0;
  #endif

  
  /* Check to see if the RLC info is present */
  if ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r8,rlc_Info))
     && ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info_r7,ul_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info_r7,dl_RLC_Mode))))
  {
    /* Get the RLC mode */
    if ((logch_mode = tdsrrcllc_get_rlc_mode_r7(&ie_ptr->rlc_Info)) == UE_MODE_MAX_NUM)
    {
      return FAILURE;
    }
    
    /* Save off the RLC Info */
    if (tdsrrcllc_get_rlc_info_r7(&ie_ptr->rlc_Info, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, FALSE) == FAILURE)
    {
      return FAILURE;
    }
  } /* end RLC Info Check */
  
    #ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
  /*Copy rlc one side re-establishment flag to OC*/
if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r8,rlc_Info))
{
  if (ie_ptr->rlc_Info.rlc_OneSidedReEst == TRUE)
  {
  
    entity_idx = tdsrrc_get_am_index(ie_ptr->rb_Identity);
    if (entity_idx != UE_MAX_AM_ENTITY)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_ONE_SIDE:  set to ONE_SIDE");
      tdsordered_config_ptr->rlc_one_sided_reest[entity_idx] = TRUE;
    }
  }
}
#endif 
  
  /* Save off the RB Mapping info */
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r8,rb_MappingInfo))
  {
    /* Remove any existing mapping for the RB */
    tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);
    
  if(ie_ptr->m.rlc_InfoPresent)
  {
    one_sided_reest_present = TRUE;
    one_sided_reest =ie_ptr->rlc_Info.rlc_OneSidedReEst;
  }

    
    if (tdsrrcllc_get_rb_mapping_info_r8(&ie_ptr->rb_MappingInfo, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, logch_mode
       , one_sided_reest_present,one_sided_reest,FALSE
        ) == FAILURE)
    {
      return FAILURE;
    }
  } /* end of RB Mapping info check */
  
#ifdef FEATURE_UMTS_PDCP
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r8,pdcp_Info))
  {
    if (tdsrrc_pdcp_enabled)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDPC info present in rb setup");
      if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,
        &ie_ptr->pdcp_Info.pdcp_Info, 
        (tdsrrc_RB_Identity)ie_ptr->rb_Identity, 
        TDSCPDCP_RECONFIG
        ) == FAILURE)
      {
        return FAILURE;
      }
    }
    else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r8,pdcp_SN_Info)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r8,pdcp_Info)) &&
             ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
              (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
              ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported))))))          
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
      return FAILURE;
    }
  }
#else

  /* Ignore the PDCP info for now */
  /********************************/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r8,pdcp_SN_Info)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r8,pdcp_Info)) &&
             ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
              (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
              ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
                tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
               (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
                rrc_LosslessSRNS_RelocSupport_supported))))))          
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }

#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

} /* end tdsrrcllc_get_rb_info_to_reconfig_r6() */

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
  tdsrrc_RB_InformationAffectedList_r8 * rb_affected_ptr
)  
{
  /* Get the affected RB list if present */
  if(rb_info_affected_list_present)
  {
   
    uint32 idx = 0;
  
    while((rb_affected_ptr->n > idx) && (idx < TDS_MAX_RB))
    {
      if(tdsrrcllc_get_rb_info_affected_r8(&rb_affected_ptr->elem[idx]) == FAILURE)
      {
        return (FAILURE);
      }
      idx++;
    }
  } /* end Affected RB */

  return(SUCCESS);
}

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
)
{
  /* Remove any existing mapping for the RB */
  tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);


  /* Get the RB Mapping info */
  return(tdsrrcllc_get_rb_mapping_info_r8(&ie_ptr->rb_MappingInfo, 
                                (tdsrrc_RB_Identity)ie_ptr->rb_Identity, 
                                UE_MODE_MAX_NUM
                                 , FALSE,FALSE,FALSE
            ));

}  /* end tdsrrcllc_get_rb_info_affected_r6() */

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
)
{
  uint8 idx;
  /* Store the RB-Mapping info */
  if( tdsrrcllc_check_rb_id_exist_rb_mapping(rb_id, &idx) == FALSE )
  {
    if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for UL DCH ");
      return FAILURE;
    }
  }
  tdsrrc_ordered_hanging_rb_mapping[idx].rb_id = rb_id;
  tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode = logch_mode;
    
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.valid = TRUE;
  
   
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.tr_id = 
                (tr_ch_id_type) ie_ptr->ul_TrCH_Type.u.dch_rach_usch->ul_TransportChannelType.u.dch;
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.mac_log_priority  = 
        (uint8) ie_ptr->mac_LogicalChannelPriority;
  /* If Logical Channel ID is present, save MAC Logical Channel Identity */
  if (ie_ptr->ul_TrCH_Type.u.dch_rach_usch->m.logicalChannelIdentityPresent)
  {
     tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.log_channel_id =
       (uint8)ie_ptr->ul_TrCH_Type.u.dch_rach_usch->logicalChannelIdentity;
  }
  else
  {
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.log_channel_id =0;
  }
  TDSRRC_MSG3(MSG_LEGACY_HIGH," Adding into Standalone Database for R6 UL DCH for RB-ID  %d for Trch id %d  at idx %d",
                             tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,
                             tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_dch_info.tr_id,
                             idx);
  /* Save the RLC Size List information */
      
  return (SUCCESS);
}

#ifdef FEATURE_TDSCDMA_HSUPA
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
)
{
  /* EDCH MAC-D FLOW Id */
  uint32         dflow_id;
  /* Get the logical channel index */
  uint8         logch_idx;
  rlc_lc_id_type   rlc_lc_id;
  uint8 mac_dflow_index;

  tdsrrc_RLC_PDU_SizeList *rlc_pdu_sizelist;
  uint8 ddi = 0;
  uint32 cnt = 0;
  uint16 rlc_size = 0;
  uint32 index = 0;

  tdsrlc_reestablish_e_type reestablish_entity = TDSRLC_RE_ESTABLISH_NONE;
  uint8 entity_idx = 0;

  dflow_id = ie_ptr->ul_TrCH_Type.u.e_dch->e_DCH_MAC_d_FlowIdentity;
  
  mac_dflow_index = tdsrrcllc_find_e_mac_d_flow_idx((uint8)dflow_id);
  /* If DFLOW Index does not exist, then create a new one */
  if (mac_dflow_index == UE_MAX_MAC_D_FLOW)
  {
    
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"EDCH Mac-d FLOW Index for DFLOW %d,not found",dflow_id);
    return(tdsrrcllc_backup_eul_from_ota_r8(ie_ptr,rb_id,logch_mode));
  }
  else
  {
    for (index=0;
      index < tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].num_logch; index++)
    {
      if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[index].rb_id == rb_id)
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"RB_ID %d, logical channel identity %d, mac_id: %d already present",
          rb_id, ie_ptr->ul_TrCH_Type.u.e_dch->logicalChannelIdentity,
          tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_mac_d_flow_id);      
        return(SUCCESS);
      }
    }
  }

  /* Get the Logical Channel Index */
  logch_idx = tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].num_logch;
  rlc_lc_id = tdsrrclcm_get_ul_rlc_lc_id((tdsrrc_RB_Identity)rb_id);

  if (rlc_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {

    /*This is the case where RB-mapping info is present in rb-reconfig-info list or affected list*/
    if (tdsrrclcm_get_ul_rlc_mode(rlc_lc_id, &logch_mode) == TDSRRCLCM_FAILURE)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Failure to get DL Log Chl mode for LC %d",rlc_lc_id);
      return(FAILURE);
    }
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].rlc_id = rlc_lc_id;
  }
  /* Save the and RLC info DDI info*/
  if(ie_ptr->ul_TrCH_Type.u.e_dch->rlc_PDU_Size.t == 
    T_tdsrrc_UL_LogicalChannelMapping_r8_rlc_PDU_Size_fixedSize)
  {
    rlc_pdu_sizelist = &ie_ptr->ul_TrCH_Type.u.e_dch->rlc_PDU_Size.u.fixedSize->rlc_PDU_SizeList;
    ddi = ie_ptr->ul_TrCH_Type.u.e_dch->rlc_PDU_Size.u.fixedSize->ddi;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: rrc_rlc_PDU_Size_flexibleSize not supported");
    return FAILURE;
  }

  /* If more than 1 UL RLC PDU size is configured for this RB, the different sizes will use subsequent DDI values starting
    from this DDI value.*/

  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].num_rlc_size_ddi_info = 0;

  while ((rlc_pdu_sizelist->n > cnt) && (cnt < TDSRRCLLC_MAX_NUM_OF_RLC_PDU_SIZE_PER_LOGCHAN)) 
  {
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].
      rlc_size_ddi_info[cnt].ddi = ddi;

    switch (rlc_pdu_sizelist->elem[cnt].t)
    {
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType1:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType1 * 8) + 16;
        break;
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType2:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType2->part1 * 32) + 272;
        if (rlc_pdu_sizelist->elem[cnt].u.sizeType2->m.part2Present)
        {
          rlc_size += (rlc_pdu_sizelist->elem[cnt].u.sizeType2->part2 * 8);
        }
        break;
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType3:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType3->part1 * 64) + 1040;
        if (rlc_pdu_sizelist->elem[cnt].u.sizeType3->m.part2Present)
        {
          rlc_size += (rlc_pdu_sizelist->elem[cnt].u.sizeType3->part2 * 8);
        }
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC TF type in OctetMode %d",rlc_pdu_sizelist->elem[cnt].t);
        break;
    }
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].
      rlc_size_ddi_info[cnt].rlc_pdu_size_in_bits = rlc_size;

    /* Increment the number of valid fields in rlc_size_ddi_info[] */
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].num_rlc_size_ddi_info++;
    cnt++;
    /* Increment the DDI */
    ddi++;
 
  }
  if(FALSE == allow_new_channels)
  {
    if(logch_mode == UE_MODE_ACKNOWLEDGED_DATA)
    {
        /*Check for fixed Size change and set the reest type*/
      if(FAILURE == tdsrrcllc_check_fixed_ul_rlc_size_change(rb_id,one_sided_reest_present,one_sided_reest,mac_dflow_index,logch_idx))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failure returned from rrcllc_check_fixed_ul_rlc_size_change functioned ");	            
        return FAILURE;
      }
    }
  }
  /* Ignore the logical channel identity signalled by NW.  Not needed by MAC*/ 
  
  /* Save Radio Bearer ID */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].rb_id = rb_id;

  /* Get the priority between a user's different RBs (or logical channels). */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].priority =
    ie_ptr->mac_LogicalChannelPriority;
  
  /* Determine the type of Logical Channel mapped to DCH TrCH */
  if (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
  {
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].chan_type = UE_LOGCHAN_DCCH;
  } 
  else
  {
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].chan_type = UE_LOGCHAN_DTCH;
  }

  /* If set include scheduling info is set to false then dont consider this mac_id while sending schedulingInformation */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].include_in_scheduling_info = 
    ie_ptr->ul_TrCH_Type.u.e_dch->includeInSchedulingInfo;

  /* Set the logical channel Id recieved from the NW */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].lc_id = 
    ie_ptr->ul_TrCH_Type.u.e_dch->logicalChannelIdentity;
  
  /* Save the logical channel mode, if a setup */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].rlc_mode = logch_mode;
  
  /* Increment number of logical channels */
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].num_logch++;  

  TDSRRC_MSG3(MSG_LEGACY_MED,"RRCEUL:EDCH E-Mac-d FLOW %d, No of dedicated Ch %d, rlc mode: %d(1:UM, >1:AM)",
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_mac_d_flow_id,
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].num_logch,
  tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[mac_dflow_index].e_ul_logch_info[logch_idx].rlc_mode);


  //check RLC Size change only for AM & lets skip this for SRB, 
  //as SRB size doesnt change + EUL already started
  if((logch_mode == UE_MODE_ACKNOWLEDGED_DATA)&&
     (rb_id > DCCH_DT_LOW_PRI_RADIO_BEARER_ID)&&
     (tdscurrent_config_ptr->e_dch_transmission == TRUE)&&
     (tdsrrc_find_rb_in_est_rabs(rb_id) == TDSRRC_RB_PRESENT))
  {
    if ((one_sided_reest_present ==TRUE) && (one_sided_reest == TRUE))
    {
      if(tdsone_sided_validation_check==TRUE)
      {
        if(tdsordered_config_ptr->am_config[entity_idx].reestab_entity== TDSRLC_RE_ESTABLISH_DL)
        {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"Both UL and DL size changed when rlc_OneSidedReEst is TRUE");
           return FAILURE;
        }
        else	  	
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish UL only");
          reestablish_entity = TDSRLC_RE_ESTABLISH_UL;
        }
      }
      else if(tdsone_sided_validation_check==FALSE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicated Reestablish UL only");
        reestablish_entity = TDSRLC_RE_ESTABLISH_UL;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlcsizeindicatedEUL Reestablish DL & UL");
      reestablish_entity = TDSRLC_RE_ESTABLISH_DL_UL;
    }
    entity_idx = tdsrrc_get_am_index(rb_id);
    
    /* LLC processes the UL AM info first, it should find the corresponding
           entity index for DL AM info to be filled in. */
    if (entity_idx == UE_MAX_AM_ENTITY)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not allowed to add new DL AM RLC entity");
      return FAILURE;
    }
    
    if(tdsrrcllc_compare_rlc_size_change(tdsrrcllc_get_ul_rlc_size_for_eul(tdsordered_config_ptr,rb_id),
           tdsrrcllc_get_ul_rlc_size_for_eul(tdscurrent_config_ptr,rb_id)) == TRUE)
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"rlcsizeindicatedEUL: OldSize %d,NewSize %d,Reestab[0:B/1:DL] %d ", tdsrrcllc_get_ul_rlc_size_for_eul(tdscurrent_config_ptr,rb_id),
          tdsrrcllc_get_ul_rlc_size_for_eul(tdsordered_config_ptr,rb_id),reestablish_entity);
      
      tdsordered_config_ptr->am_config[entity_idx].reestab_entity = reestablish_entity;
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"rlcsizeindicatedEUL: Old Size %d, New Size %d",tdsrrcllc_get_ul_rlc_size_for_eul(tdscurrent_config_ptr,rb_id),tdsrrcllc_get_ul_rlc_size_for_eul(tdsordered_config_ptr,rb_id));
    }
  }



  tdsrrcllc_free_hanging_rb_mapping_info_index(rb_id,TDSE_DCH_RB_MAPPING);

  return SUCCESS;

}

/*==========================================================
FUNCTION  : tdsrrcllc_backup_eul_from_ota_r8()

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
)
{
  uint8 idx;
  tdsrrc_RLC_PDU_SizeList *rlc_pdu_sizelist;
  uint8 ddi = 0;
  uint32 cnt = 0;
  uint16 rlc_size = 0;

  
  /* Store the RB-Mapping info */
  if( tdsrrcllc_check_rb_id_exist_rb_mapping(rb_id, &idx) == FALSE )
  {
    if(tdsrrcllc_find_free_hanging_rb_mapping_info(&idx) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No space for RB Mapping backup for UL DCH ");
      return FAILURE;
    }
  }
  tdsrrc_ordered_hanging_rb_mapping[idx].rb_id = rb_id;
  tdsrrc_ordered_hanging_rb_mapping[idx].logch_mode = logch_mode;
    
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.valid = TRUE;
  
   
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.mac_e_flow_id= 
                 (uint8)ie_ptr->ul_TrCH_Type.u.e_dch->e_DCH_MAC_d_FlowIdentity;
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.mac_log_priority  = 
        (uint8) ie_ptr->mac_LogicalChannelPriority;

  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.log_channel_id =
       (uint8)ie_ptr->ul_TrCH_Type.u.e_dch->logicalChannelIdentity;
  /* Save the and RLC info DDI info*/
  
  if(ie_ptr->ul_TrCH_Type.u.e_dch->rlc_PDU_Size.t == 
  	   T_tdsrrc_UL_LogicalChannelMapping_r8_rlc_PDU_Size_fixedSize)
  {
    rlc_pdu_sizelist = &ie_ptr->ul_TrCH_Type.u.e_dch->rlc_PDU_Size.u.fixedSize->rlc_PDU_SizeList;
    ddi = ie_ptr->ul_TrCH_Type.u.e_dch->rlc_PDU_Size.u.fixedSize->ddi;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"REL8: rrc_rlc_PDU_Size_flexibleSize not supported");
    return FAILURE;
  }  

  /* If more than 1 UL RLC PDU size is configured for this RB, the different sizes will use subsequent DDI values starting
    from this DDI value.*/
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.num_rlc_size_ddi_info = 0;

  while ((rlc_pdu_sizelist->n > 0) && (cnt < TDSRRCLLC_MAX_NUM_OF_RLC_PDU_SIZE_PER_LOGCHAN)) 
  {
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.rlc_size_ddi_info[cnt].ddi = ddi;

    switch (rlc_pdu_sizelist->elem[cnt].t)
    {
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType1:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType1 * 8) + 16;
        break;
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType2:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType2->part1 * 32) + 272;
        if (rlc_pdu_sizelist->elem[cnt].u.sizeType2->m.part2Present)
        {
          rlc_size += (rlc_pdu_sizelist->elem[cnt].u.sizeType2->part2 * 8);
        }
        break;
      case T_tdsrrc_OctetModeRLC_SizeInfoType1_sizeType3:
        rlc_size = (rlc_pdu_sizelist->elem[cnt].u.sizeType3->part1 * 64) + 1040;
        if (rlc_pdu_sizelist->elem[cnt].u.sizeType3->m.part2Present)
        {
          rlc_size += (rlc_pdu_sizelist->elem[cnt].u.sizeType3->part2 * 8);
        }
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RRC TF type in OctetMode %d",rlc_pdu_sizelist->elem[cnt].t);
        break;
    }
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.rlc_size_ddi_info[cnt].rlc_pdu_size_in_bits = rlc_size;

    /* Increment the number of valid fields in rlc_size_ddi_info[] */
    tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.num_rlc_size_ddi_info++;
    cnt++;
    /* Increment the DDI */
    ddi++;

  }

  /* If set include scheduling info is set to false then dont consider this mac_id while sending schedulingInformation */
  tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.in_sched_info = 
    ie_ptr->ul_TrCH_Type.u.e_dch->includeInSchedulingInfo;

 
  TDSRRC_MSG3(MSG_LEGACY_HIGH," Adding into Standalone Database for R6 E_DCH for RB-ID %d for FLOW ID %d  at idx %d",
                             tdsrrc_ordered_hanging_rb_mapping[idx].rb_id,
                             tdsrrc_ordered_hanging_rb_mapping[idx].ul_mapping_info.ul_mace.mac_e_flow_id,
                             idx);
  
  
      
  return (SUCCESS);
}
#endif /*FEATURE_TDSCDMA_HSUPA*/

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
)
{
  uint32 srb5_trch_id = 0;
  tdsrrc_RB_Identity    srb5_id = 0;
  uint8         trch_idx = TDSRRCLLC_INVALID_TRCH_IDX;
  uecomdef_status_e_type result = FAILURE;

  if(srb_setup_r8_ptr != NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Probable SRB5 INFO .. processing");
    /* Probably SRB5 exists  */
    if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(srb_setup_r8_ptr,
		 tdsrrc_SRB_InformationSetup_r8,rb_Identity))
    {
      srb5_id = (tdsrrc_RB_Identity)srb_setup_r8_ptr->rb_Identity;
    }
    else
    {
      srb5_id = rb_id;
    }

    /* Verify RLC mode - accept iff DL TM mode */
    if(TDSRRC_CHECK_COMMON_MSG_TYPE(srb_setup_r8_ptr->rlc_InfoChoice,
		rrc_RLC_InfoChoice_r7_rlc_Info))
    {
      if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(
	  	srb_setup_r8_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,
	  	ul_RLC_Mode))
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"SRB#5 can't have UL mode .. rejecting");
        return result;
      }
      else if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR_FOR_OBYS_ONLY(
	  	srb_setup_r8_ptr->rlc_InfoChoice.u.rlc_Info,tdsrrc_RLC_Info_r7,
	  	dl_RLC_Mode))
      {
        if(srb_setup_r8_ptr->rlc_InfoChoice.u.rlc_Info->dl_RLC_Mode.t !=
			T_tdsrrc_DL_RLC_Mode_r7_dl_TM_RLC_Mode)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"SRB#5 can't be in other than DL TM mode .. rejecting");
          return result;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"SRB#5 can't be in other than DL .. rejecting");
        return result;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"No other SRB can have the characteristics of SRB#5 .. rejecting");
      return result;
    }

    /* Verify RLC mapping */
    if(TDSRRC_MSG_COMMON_BITMASK_IE(srb_setup_r8_ptr->rb_MappingInfo.elem[0],
		tdsrrc_RB_MappingOption_r8,ul_LogicalChannelMappings))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SRB#5 can't have UL mapping .. rejecting");
      return result;
    }
    else if(TDSRRC_MSG_COMMON_BITMASK_IE(srb_setup_r8_ptr->rb_MappingInfo.elem[0],
		tdsrrc_RB_MappingOption_r8,dl_LogicalChannelMappingList))
    {
      if(TDSRRC_CHECK_COMMON_MSG_TYPE(srb_setup_r8_ptr->rb_MappingInfo.elem[0].dl_LogicalChannelMappingList.
          elem[0].dl_TransportChannelType,rrc_DL_TransportChannelType_r7_dch))
      {
        /* Remember the mapped TRCH ID for SRB5 */
        srb5_trch_id = srb_setup_r8_ptr->rb_MappingInfo.elem[0].dl_LogicalChannelMappingList.elem[0].dl_TransportChannelType.u.dch;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"SRB#5 can't DL mapping other than DCH .. rejecting");
        return result;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SRB#5 can't DL mapping other than DCH .. rejecting");
      return result;
    }

    trch_idx = tdsrrcllc_find_dl_dch_trch_idx((tr_ch_id_type)srb5_trch_id);

    if (trch_idx == TDSRRCLLC_INVALID_TRCH_IDX)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DL DCH TrCH ID %d,no idx found",srb5_trch_id);
      return result;
    }

    /* If we came this far, we found SRB5. Update OC with SRB5 info */
    tdsordered_config_ptr->srb5_info.srb5_rb_exists   = TRUE;
    tdsordered_config_ptr->srb5_info.rb_id            = srb5_id;
    tdsordered_config_ptr->srb5_info.srb5_trch_exists = TRUE;
    tdsordered_config_ptr->srb5_info.trch_id          = srb5_trch_id;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Updated OC with SRB5 INFO->rb id=%d,tchid=%d",srb5_id,srb5_trch_id);
    result = SUCCESS;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"One of the SRB pointers is NULL ... rejecting");
    result = FAILURE;
  }

  return result;
}
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_CHANGE_SRB4_PRIRORITY
/*====================================================================
FUNCTION: tdsrrcllc_check_and_set_srb3_srb4_priority()

DESCRIPTION:
  This function checks the priorities of SRB3 and SRB4 and set the priority of SR4
  priority if both the SRBs have equal priority. SRB4 should have lower priority.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.


SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_check_and_set_srb3_srb4_priority(void)
{
  uint8 dflow_idx,logch_idx,num_dflow,num_logch,srb3_pri=0,srb4_pri=0,srb2_pri=0,
        srb4_dflow_idx=0,srb4_logch_idx=0,srb3_dflow_idx=0,srb3_logch_idx=0;
  boolean is_SRB_transport_channel = FALSE;
  boolean is_SRB_mac_d_flow = FALSE;

  if(tdsordered_config_ptr->ul_cctrch.cctrch_type == TDSRACH)
  {
    //SRBs are mapped on RACH
    num_logch = tdsordered_config_ptr->ul_cctrch.rach_info.ndlchan;
    for (logch_idx = 0; logch_idx < num_logch; logch_idx++)
    {
      if (tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx].rb_id == 
          DCCH_DT_HIGH_PRI_RADIO_BEARER_ID )
      {
        srb3_pri = tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx].priority;
        srb3_logch_idx = logch_idx;
      }
      if (tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx].rb_id == 
          DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
      {
        srb4_pri = tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx].priority;
        srb4_logch_idx = logch_idx;
      }
      if(tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx].rb_id == 
          DCCH_AM_RADIO_BEARER_ID)
      {
        srb2_pri = tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[logch_idx].priority;
      }
    }
    
    if((srb3_pri == srb4_pri) || (srb3_pri == srb2_pri))
    {
      srb3_pri = srb2_pri + 1;
      srb4_pri = srb3_pri + 1;
      tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[srb4_logch_idx].priority = srb4_pri;
      tdsordered_config_ptr->ul_cctrch.rach_info.dlchan_info[srb3_logch_idx].priority = srb3_pri;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Lowered RACH SRB4 %d & SRB3 priority: %d",srb4_pri,srb3_pri);
    }
  }
  if(tdsordered_config_ptr->ul_cctrch.cctrch_type == TDSDCH)
  {
    //SRBs are mapped on DCH
    num_dflow = tdsordered_config_ptr->ul_cctrch.dch_info.ndchs;
    for(dflow_idx =0;dflow_idx < num_dflow;num_dflow++)
    {
      num_logch = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[dflow_idx].ndlchan;
      for (logch_idx = 0; logch_idx < num_logch; logch_idx++)
      {
        if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[dflow_idx].dlchan_info[logch_idx].rb_id == 
            DCCH_DT_HIGH_PRI_RADIO_BEARER_ID )
        {
          srb3_pri = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[dflow_idx].dlchan_info[logch_idx].priority;
          srb3_dflow_idx = dflow_idx;
          srb3_logch_idx = logch_idx;
          is_SRB_transport_channel = TRUE;
        }
        if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[dflow_idx].dlchan_info[logch_idx].rb_id == 
            DCCH_DT_LOW_PRI_RADIO_BEARER_ID)
        {
          srb4_pri = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[dflow_idx].dlchan_info[logch_idx].priority;
          srb4_dflow_idx = dflow_idx;
          srb4_logch_idx = logch_idx;
        }
        if (tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[dflow_idx].dlchan_info[logch_idx].rb_id == 
            DCCH_AM_RADIO_BEARER_ID)
        {
          srb2_pri = tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[dflow_idx].dlchan_info[logch_idx].priority;
        }
      }
      if((srb2_pri != 0) && (srb3_pri != 0) && (srb4_pri != 0))
      {
        break;
      }
    }
    
    if((is_SRB_transport_channel == TRUE) && ((srb3_pri == srb4_pri) || (srb3_pri == srb2_pri)))
    {
      srb3_pri = srb2_pri + 1;
      srb4_pri = srb3_pri + 1;
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[srb3_dflow_idx].dlchan_info[srb3_logch_idx].priority = srb3_pri;
      tdsordered_config_ptr->ul_cctrch.dch_info.dch_info[srb4_dflow_idx].dlchan_info[srb4_logch_idx].priority = srb4_pri;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Lowered RACH SRB4 %d & SRB3 priority: %d",srb4_pri,srb3_pri);
    }
  }

#ifdef FEATURE_TDSCDMA_HSUPA
  //SRBs are mapped on E-DCH
  srb2_pri = srb3_pri = srb4_pri = 0;
  num_dflow = tdsordered_config_ptr->mac_e_config.num_e_mac_d_flow;
  for(dflow_idx =0; dflow_idx < num_dflow; num_dflow++)
  {
    num_logch = tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_idx].num_logch;
    for (logch_idx = 0; logch_idx < num_logch; logch_idx++)
    {
      if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_idx].e_ul_logch_info[logch_idx].rb_id == 
          DCCH_DT_HIGH_PRI_RADIO_BEARER_ID )
      {
        srb3_pri = tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_idx].e_ul_logch_info[logch_idx].priority;
        srb3_dflow_idx = dflow_idx;
        srb3_logch_idx = logch_idx;
        is_SRB_mac_d_flow = TRUE;
      }
      if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_idx].e_ul_logch_info[logch_idx].rb_id == 
          DCCH_DT_LOW_PRI_RADIO_BEARER_ID )
      {
        srb4_pri = tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_idx].e_ul_logch_info[logch_idx].priority;
        srb4_dflow_idx = dflow_idx;
        srb4_logch_idx = logch_idx;
      }
      if (tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_idx].e_ul_logch_info[logch_idx].rb_id == 
          DCCH_AM_RADIO_BEARER_ID )
      {
        srb2_pri = tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[dflow_idx].e_ul_logch_info[logch_idx].priority;
      }
    }
    if((srb2_pri != 0) && (srb3_pri != 0) && (srb4_pri != 0))
    {
      break;
    }
  }
  
  if((is_SRB_mac_d_flow == TRUE) && ((srb3_pri == srb4_pri) || (srb3_pri == srb2_pri)))
  {
    srb3_pri = srb2_pri + 1;
    srb4_pri = srb3_pri + 1;
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[srb3_dflow_idx].e_ul_logch_info[srb3_logch_idx].priority = srb3_pri;
    tdsordered_config_ptr->mac_e_config.e_mac_d_flow_info[srb4_dflow_idx].e_ul_logch_info[srb4_logch_idx].priority = srb4_pri;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Lowered RACH SRB4 %d & SRB3 priority: %d",srb4_pri,srb3_pri);
  }
#endif /*FEATURE_TDSCDMA_HSUPA*/
}
#endif/*FEATURE_TDSCDMA_CHANGE_SRB4_PRIRORITY*/
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
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type    logch_mode = UE_MODE_MAX_NUM;
  
  /*initialize a counter to check rb count*/
  uint8                         rb_cnt  = 0;

#ifdef FEATURE_UMTS_PDCP
  uint8 idx = 0;
#endif /* FEATURE_UMTS_PDCP */

  /*place holder for rb_id we get originally*/
  tdsrrc_RB_Identity                    local_rb_id = (tdsrrc_RB_Identity)ie_ptr->rb_Identity;
  
  /* Local IE pointer - if RLC Info for this RB is the same as 
  * RLC Info for another RB, IE Pointer is made to point to 
  * SRB Info Setup for the same_as RB. 
  * For RB Mapping info, IE Pointer should always point to
  * SRB Info Setup for the RB that came as a parameter.
  */
  tdsrrc_RB_InformationSetup_r4     *temp_ie_ptr = ie_ptr;

  
  /*check if rlc_InfoChoice is same_as_RB_Chosen and then get the
  *rlc_Info for the rb_id specified.  Walk thru the rb_setup_head_ptr
  *list until we find a rb_id that matches
  */
  if(TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->rlc_InfoChoice, rrc_RLC_InfoChoice_same_as_RB))
  {
    if (rb_setup_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"rb_setup_ptr is NULL");
      return FAILURE;
    }
    while ((rb_setup_ptr->n > rb_cnt) && (rb_cnt < MAX_RB_PER_RAB))
    {
      if (ie_ptr->rlc_InfoChoice.u.same_as_RB == 
        rb_setup_ptr->elem[rb_cnt].rb_Identity)
      {
        if(rb_setup_ptr->elem[rb_cnt].rlc_InfoChoice.t == T_tdsrrc_RLC_InfoChoice_rlc_Info)
        {
            temp_ie_ptr = &(rb_setup_ptr->elem[rb_cnt]);
            break;
        }
      }
      rb_cnt++;
    } /* end RB per RAB setup */
    
    if ((rb_setup_ptr->n == 0) || (rb_cnt >= MAX_RB_PER_RAB) || (rb_cnt >= rb_setup_ptr->n))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to match rb_id to same_as_RBin OTA msg");  
      return FAILURE;
    }
  } /* if RLC info choice is "Same as RB" type */

  /* Now we have either the RLC Info explicitly meant for this RB OR
    * the RLC Info that is the same as another RB.Get the RLC mode */
  
    if ((logch_mode = 
      tdsrrcllc_get_rlc_mode(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info)) == UE_MODE_MAX_NUM)
    {
      return FAILURE;
    }
    /* Save off the RLC Info */
    if (tdsrrcllc_get_rlc_info(temp_ie_ptr->rlc_InfoChoice.u.rlc_Info, 
      local_rb_id, TRUE) == FAILURE)
    {
      return FAILURE;
    }

  /* Use the IE Pointer that came as a parameter to get the RB Mapping info */
  if (tdsrrcllc_get_rb_mapping_info(&ie_ptr->rb_MappingInfo, local_rb_id, logch_mode)
    == FAILURE)
  {
    return FAILURE;
  }
  
#ifdef FEATURE_UMTS_PDCP

  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r4,pdcp_Info))
  {
  if (tdsrrc_pdcp_enabled)  
  { 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDPC info present in rb setup");
    if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,
                                    &ie_ptr->pdcp_Info, 
                                    local_rb_id, 
                                    TDSCPDCP_SETUP
                                    ) == FAILURE)
    {
      return FAILURE;
    }
    return SUCCESS;
  }
  else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r4,pdcp_Info)) &&
      ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
       (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
       ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
        (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)))))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP NV not enabled.  PDCP info not handled");
    return FAILURE;
  }
  }

  /* if fall-thru happens, PDCP layer has to be setup in transparent [pass thru] mode */
  if (cn_Domain == tdsrrc_CN_DomainIdentity_ps_domain) 
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Setting up PDCP in transparent mode rb_id %d",local_rb_id);
    /*Get the next free index in pdcp_config_req */
    while (idx < MAX_RAB_TO_SETUP) 
    {
      if (tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use == FALSE) 
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"pdcp config req index is %d",idx);
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].rb_id = local_rb_id;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.action = SETUP_PDCP;
        tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].pdcp_cfg_req.is_mcast_call = FALSE;

        tdsordered_config_ptr->pdcp_parms.num_rbs++;
        break;
      }
      else
      {
        idx++;
      }
    }
    /* CPDCP action will be setup, but pdcp info will be null */
    //tdsordered_config_ptr->pdcp_parms.pdcp_config_req[idx].in_use = TRUE;
  }
#else
  /* Ignore the PDCP info for now */
  /********************************/
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationSetup_r4,pdcp_Info)) &&
      ((ie_ptr->pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
       (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
       ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info,tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
        (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.losslessSRNS_RelocSupport,rrc_LosslessSRNS_RelocSupport_supported)))))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }
#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

} /* end tdsrrcllc_get_rb_info_to_setup_r4() */
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
)
{
  /* local variable for log channel mode */
  uecomdef_logch_mode_e_type logch_mode = UE_MODE_MAX_NUM;

  /* Check to see if the RLC info is present */
  if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig,rlc_Info))
     && ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info,ul_RLC_Mode)) ||
         (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->rlc_Info,tdsrrc_RLC_Info,dl_RLC_Mode))))
  {
    /* Get the RLC mode */
    if((logch_mode = tdsrrcllc_get_rlc_mode(&ie_ptr->rlc_Info)) == UE_MODE_MAX_NUM)
    {
      return FAILURE;
    }
  
    /* Save off the RLC Info */
    if(tdsrrcllc_get_rlc_info(&ie_ptr->rlc_Info, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, FALSE) == FAILURE)
    {
      return FAILURE;
    }
  } /* end RLC Info Check */

  /* Save off the RB Mapping info */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig,rb_MappingInfo))
  {
    /* Remove any existing mapping for the RB */
    tdsrrcllc_unmap_rb((tdsrrc_RB_Identity)ie_ptr->rb_Identity, TDSOC_AND_TOC);

    if(tdsrrcllc_get_rb_mapping_info(&ie_ptr->rb_MappingInfo, (tdsrrc_RB_Identity)ie_ptr->rb_Identity, logch_mode)
      == FAILURE)
    {
      return FAILURE;
    }
  } /* end of RB Mapping info check */
  
  /* Check to see if the RB supports stop/continue */
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig,rb_StopContinue))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB Stop/Continue IE recvd");
  }

#ifdef FEATURE_UMTS_PDCP
  if (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig,pdcp_Info))
  {
  if (tdsrrc_pdcp_enabled)
  { 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PDPC info present in rb setup");
    if (tdsrrcllc_get_pdcp_info_later_than_r3(&tdsordered_config_ptr->pdcp_parms,
                                      &ie_ptr->pdcp_Info.pdcp_Info, 
                                      (tdsrrc_RB_Identity)ie_ptr->rb_Identity, 
                                      TDSCPDCP_RECONFIG
                                      ) == FAILURE)
    {
      return FAILURE;
    }
  }

  /* copy PDCP handle from R5 begin */
  else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r4,pdcp_SN_Info)) ||
          ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r4,pdcp_Info)) &&
           ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
            (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
              tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
              tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
             (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
              rrc_LosslessSRNS_RelocSupport_supported))))))          
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
    return FAILURE;
  }
  /* copy PDCP handle from R5 end */
  }
#else

  /* Ignore the PDCP info for now */
  /********************************/
  else if((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r4,pdcp_SN_Info)) ||
          ((TDSRRC_MSG_COMMON_BITMASK_IE_PTR(ie_ptr,tdsrrc_RB_InformationReconfig_r4,pdcp_Info)) &&
           ((ie_ptr->pdcp_Info.pdcp_Info.pdcp_PDU_Header == tdsrrc_PDCP_PDU_Header_present) ||
            (TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
              tdsrrc_PDCP_Info_r4,headerCompressionInfoList)) ||
            ((TDSRRC_MSG_COMMON_BITMASK_IE(ie_ptr->pdcp_Info.pdcp_Info,
              tdsrrc_PDCP_Info_r4,losslessSRNS_RelocSupport)) &&
             (TDSRRC_CHECK_COMMON_MSG_TYPE(ie_ptr->pdcp_Info.pdcp_Info.losslessSRNS_RelocSupport,
              rrc_LosslessSRNS_RelocSupport_supported))))))          
  {
  TDSRRC_MSG0(MSG_LEGACY_ERROR,"PDCP Info not supported");
  return FAILURE;
  }
#endif /* FEATURE_UMTS_PDCP */

  /* All is well with this IE */
  return SUCCESS;

} /* end tdsget_rb_info_to_reconfig() */


/*lint +e818 */




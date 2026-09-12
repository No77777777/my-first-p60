#ifndef L1POSI_H
#define L1POSI_H

/*==========================================================================

             L1 Positioning Manager Internal Header

DESCRIPTION
  This file contains code that is used by the WCDMA L1 subsystem to support
  UMTS Positioning Features.

  Copyright (c) 2003 - 2007 by Qualcomm Technologies Incorporated.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsl1posi.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/12/07   rc      Changed feature name FEATURE_USER_PLANE_NMR to 
                   FEATURE_USER_PLANE_MRL as recommended by LSM team.
05/29/07   kps     FEATURE_CGPS: GPS interface change support
04/25/05   mc      Added P4 DateTime and Author header.
06/04/04   scm     Adjust debug message defines.
01/06/03   scm     Featurize lsm_wcdma_cell_meas_update() define with
                   FEATURE_USER_PLANE_NMR.
04/28/03   djm     Initial revision.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "tdscdma_variation.h"
#include "msg.h"

#ifndef  FEATURE_TDSCDMA_PLT
#include "tdsrrcdata.h"
//#include "mm_umts.h"
#endif

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/

/*
 * MSG_*() - Control Functions, WL1POS Internal
 */
#define  L1POS_MSG_HIGH(str, a,b,c)                MSG_HIGH(str,a,b,c)
#define  L1POS_MSG_MED(str, a,b,c)                 MSG_MED(str,a,b,c)
#define  L1POS_MSG_LOW(str, a,b,c)                 MSG_LOW(str,a,b,c)
#define  L1POS_MSG_OFF(str, a,b,c)


#if defined(FEATURE_MMGPS) 

#define RRC_SRNC_MASK      ((uint32)(0xFFF00000))
#define RRC_SRNC_SHIFT     (20)
#define RRC_URNTI_TO_RNCID(urnti)         ( ((urnti) & RRC_SRNC_MASK) >> RRC_SRNC_SHIFT )

/*
 * WCDMA L1 - RRC Interface Requirements
 */
/* RRC_GET_RNC_IDENT() */
__inline boolean RRC_GET_RNC_IDENT( uint16 *p_rnc_ident )
{
  boolean ret = FALSE;
  uint32  urnti;

  if ( tdsrrcllc_get_current_urnti(&urnti) == TDSU_RNTI_PRESENT )
  {
    ret = TRUE;
    *p_rnc_ident = RRC_URNTI_TO_RNCID(urnti);
  }

  return (ret);
}

/*
 * code taken from tdsrrccsp_update_plmn_list()
 */
/* RRC_GET_LAI_IDENT() */
__inline boolean RRC_GET_LAI_IDENT( rrc_plmn_identity_type plmn_id,
                                    tdsrrc_int_cell_id_type   cell_id,
                                    rrc_lai_type *p_lai_ident )
{
  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;

  /* Pointer to SIB1 */
  rrc_SysInfoType1 *sib1_ptr;

#if 0
  unsigned char *data_ptr;
  uint32 i;
#endif

  sib1_ptr = (rrc_SysInfoType1 *) tdsrrc_sibdb_return_sib_for_cell(
                                            rrc_systemInformationBlockType1,
                                            plmn_id,
                                            cell_id);
  if( sib1_ptr == NULL )
  {
    /* SIB1 for this PLMN, CEll ID combination not present */
    MSG_HIGH("SIB1 for MCC %d, MNC %d, Cell ID %d not present",
                      TDSRRC_CSP_GET_INT_MCC(plmn_id),
                      TDSRRC_CSP_GET_INT_MNC(plmn_id),
                      cell_id);
    return (FALSE);
  } /* SIB1 not present */

  /* The SIB1 is present */
  nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.length;

  /* Check if the RRC MM interface structure array can hold this info */
  if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.length > RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
    MSG_ERROR("Not enough space for Common NAS Sys Info (%d bytes)",
                        sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.length, 0, 0);
    return (FALSE);
  }

  /* Fill in the Common NAS system info now */
  memscpy( nas_common_info.value,
           sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.length,
          sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.value,
          sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.length );
#if 0
  data_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.value;
  for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.length; i ++)
  {
    nas_common_info.value[i] =  *data_ptr;
    data_ptr ++;
  }
#endif

  /* Get the LAI status from local utility */
  /* TBD: Call MM utility to initialize lai first. */
  if(mm_extract_lac_from_sib_info(p_lai_ident, &nas_common_info) == FALSE)
  {
    MSG_ERROR("Could not extract LAI from NAS Common sys info", 0, 0, 0);
    return (FALSE);
  }

  /* Since MM doesn't know the PLMN from NAS system info, fill in the PLMN now */
  p_lai_ident->plmn = plmn_id;

  return (TRUE);
}

#ifdef FEATURE_TDSCDMA_L1_MMGPS
/* These should be provided by LSM.  Only define them for WL1 unit test when
** there is no LSM subsystem present. */
#ifdef FEATURE_USER_PLANE_MRL
#define lsm_wcdma_cell_meas_update(meas_ptr)      TRUE
#endif

#endif

#endif /* FEATURE_MMGPS || FEATURE_TDSCDMA_L1_MMGPS  */

#endif /* L1POSI_H */

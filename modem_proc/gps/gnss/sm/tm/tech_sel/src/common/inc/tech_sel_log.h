/*======================================================================

  Technology Selector Log Header File

 GENERAL DESCRIPTION
  This file contains function prototypes for logging
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2016 - 22018 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/common/inc/tech_sel_log.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
05/17/16   ss    Initial creation of file.
======================================================================*/
#ifndef TECH_SEL_LOG_H
#define TECH_SEL_LOG_H
#include "log.h"
#include "tech_sel_event.h"
#include "tech_sel_rules.h"
#include "tech_sel_gnss_tech_mgr.h"
#include "tech_sel_wifi_tech_mgr.h"

#if defined(T_WINNT)
#error code not present
#endif

typedef PACKED struct PACKED_POST
{
  log_hdr_type xx_hdr;
  uint8       u_Version; /* Version number of DM log */
  uint32      q_Fcount; /* Local millisecond counter */

  techSelEventType e_EventType;
  techSelRuleNum e_RuleNum;
} tech_sel_rule_engine_decision_log_rpt;

typedef PACKED struct PACKED_POST
{
  log_hdr_type xx_hdr;
  uint8       u_Version; /* Version number of DM log */
  uint32      q_Fcount; /* Local millisecond counter */

  gnss_MgpPedStatusEnumType e_PedStatus;
} tech_sel_ped_status_change_log_rpt;

typedef PACKED struct PACKED_POST
{
  log_hdr_type xx_hdr;
  uint8       u_Version; /* Version number of DM log */
  uint32      q_Fcount; /* Local millisecond counter */

  gnss_QualIndEnumType e_GnssQualityInd;
} tech_sel_gnss_qual_change_log_rpt;

typedef PACKED struct PACKED_POST
{
  log_hdr_type xx_hdr;
  uint8       u_Version; /* Version number of DM log */
  uint32      q_Fcount; /* Local millisecond counter */

  gnss_PedDevCxtEnumType  e_PedDevCtxInd;
} tech_sel_ped_dev_ctx_change_log_rpt;

typedef PACKED struct PACKED_POST
{
  log_hdr_type xx_hdr;
  uint8       u_Version; /* Version number of DM log */
  uint32      q_Fcount; /* Local millisecond counter */

  techSelGnssStrength  e_GnssStrength;
} tech_sel_gnss_strength_change_log_rpt;

typedef PACKED struct PACKED_POST
{
  log_hdr_type xx_hdr;
  uint8       u_Version; /* Version number of DM log */
  uint32      q_Fcount; /* Local millisecond counter */

  techSelWifiStrength  e_WifiStrength;
} tech_sel_wifi_strength_change_log_rpt;

typedef enum
{
  TECH_SEL_LPPM_START_REQ,
  TECH_SEL_LPPM_MODIFY_REQ,
  TECH_SEL_LPPM_STOP_REQ,
  TECH_SEL_LPPM_MAX_REQ = TECH_SEL_LPPM_STOP_REQ,
  TECH_SEL_LPPM_REQ_FORECE_32 = 2147483647 /* Force 32bit */
} tech_sel_lppm_request_e_type;

typedef PACKED struct PACKED_POST
{
  tech_sel_lppm_request_e_type  e_LppmReqType;
  uint32  q_TbmMsecs;
  tm_sess_power_mode_e_type e_PowerMode;
} tech_sel_lppm_request_info;

typedef PACKED struct PACKED_POST
{
  log_hdr_type xx_hdr;
  uint8       u_Version; /* Version number of DM log */
  uint32      q_Fcount; /* Local millisecond counter */

  tech_sel_lppm_request_info z_LppmReqInfo;
} tech_sel_lppm_request_log_rpt;

#if defined(T_WINNT)
#error code not present
#endif
/********************************************************************
*
* tech_sel_log_rule_engine_decision
*
* Function description:
* This function is used to log TechSel Rule Engine Decision
*
* Parameters: 
* e_EventType: Event Type
* e_RuleNum: Rule Number
* Return:
* Void
*  
*********************************************************************
*/  
void tech_sel_log_rule_engine_decision(techSelEventType e_EventType, techSelRuleNum e_RuleNum);
/********************************************************************
*
* tech_sel_log_ped_status_change_ind
*
* Function description:
* This function is used to log Pedestrian Status Change Indication
*
* Parameters: 
* e_PedStatus: Ped Status
* Return:
* Void
*  
*********************************************************************
*/  
void tech_sel_log_ped_status_change_ind(gnss_MgpPedStatusEnumType e_PedStatus);
/********************************************************************
*
* tech_sel_log_gnss_qual_change_ind
*
* Function description:
* This function is used to log Gnss Quality Change Indication
*
* Parameters: 
* e_GnssQualityInd: Gnss Quality
* Return:
* Void
*  
*********************************************************************
*/  
void tech_sel_log_gnss_qual_change_ind(gnss_QualIndEnumType  e_GnssQualityInd);
/********************************************************************
*
* tech_sel_log_ped_dev_ctx_change_ind
*
* Function description:
* This function is used to log Ped Device Context  Change Indication
*
* Parameters: 
* e_PedDevCtxInd: Ped Device Context
* Return:
* Void
*  
*********************************************************************
*/  
  void tech_sel_log_ped_dev_ctx_change_ind(gnss_PedDevCxtEnumType  e_PedDevCtxInd);
/********************************************************************
*
* tech_sel_log_gnss_strength_change_ind
*
* Function description:
* This function is used to log Gnss Strength Change Indication
*
* Parameters: 
* e_GnssStrength: Gnss Strength
* Return:
* Void
*  
*********************************************************************
*/  
void tech_sel_log_gnss_strength_change_ind(techSelGnssStrength e_GnssStrength);
/********************************************************************
*
* tech_sel_log_wifi_strength_change_ind
*
* Function description:
* This function is used to log Wifi Strength Change Indication
*
* Parameters: 
* e_WifiStrength: Wifi Strength
* Return:
* Void
*  
*********************************************************************
*/  
void tech_sel_log_wifi_strength_change_ind(techSelWifiStrength e_WifiStrength);
/********************************************************************
*
* tech_sel_log_lppm_request
*
* Function description:
* This function is used to log TechSel LPPM requests
*
* Parameters: 
* pz_LppmReqInfo: Pointer to LPPM Reques Info Struct
* Return:
* Void
*  
*********************************************************************
*/  
void tech_sel_log_lppm_request(tech_sel_lppm_request_info* pz_LppmReqInfo);


#endif

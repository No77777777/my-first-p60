/*======================================================================

 Technology Selector PDR Rule

 GENERAL DESCRIPTION
  This file contains declaration of PDR Rule Class
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2016 - 2018 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/rules/inc/tech_sel_rule_pdr.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_RULE_PDR_H
#define TECH_SEL_RULE_PDR_H

#include "tech_sel_rules.h"
#include "tech_sel_rules_engine.h"

#define TECHSEL_TBM_1000_MILLI_SECONDS 1000 /*1 second in milli-seconds*/
#define TECHSEL_TBM_5000_MILLI_SECONDS 5000 /*5 seconds in milli-seconds*/
#define TECHSEL_TBM_10000_MILLI_SECONDS 10000 /*10 seconds in milli-seconds*/

#define TECHSEL_PED_EXIT_EVAL_TBM_MILLI_SECONDS  1000 /*1 second in milli-seconds*/

#define TECHSEL_PED_SENSOR_ASSIST_NOT_AVAIL_DEFAULT_TBM_MILLI_SECONDS    1000  /*1 second in milli-seconds*/
#define TECHSEL_PED_SENSOR_ASSIST_AVAIL_DEFAULT_TBM_MILLI_SECONDS        3000  /*3 seconds in milli-seconds*/

#define TECHSEL_PED_SENSOR_ASSIST_NOT_AVAIL_PROFILE_0_TBM_MILLI_SECONDS    1000  /*1 second in milli-seconds*/
#define TECHSEL_PED_SENSOR_ASSIST_AVAIL_PROFILE_0_TBM_MILLI_SECONDS        3000  /*3 seconds in milli-seconds*/

#define TECHSEL_PED_SENSOR_ASSIST_NOT_AVAIL_PROFILE_1_TBM_MILLI_SECONDS    2000 /*2 seconds in milli-seconds*/
#define TECHSEL_PED_SENSOR_ASSIST_AVAIL_PROFILE_1_TBM_MILLI_SECONDS        3000 /*3 seconds in milli-seconds*/

#define TECHSEL_PED_SENSOR_ASSIST_NOT_AVAIL_PROFILE_2_TBM_MILLI_SECONDS    3000  /*3 seconds in milli-seconds*/
#define TECHSEL_PED_SENSOR_ASSIST_AVAIL_PROFILE_2_TBM_MILLI_SECONDS        5000  /*5 seconds in milli-seconds*/


/*This class acts as base class for all PDR\LPPM Rules*/
class TechSelRulePdr :public TechSelRule {
  public:      

      /*Public Member Functions*/
      TechSelRulePdr(techSelRuleNum u_ruleNum);

      /*Since all PDR\LPPM Rules have same type of session interest, this function is implemented
          in this base class*/
      boolean isSessionOfInterest(techSel_PosSessInfo *p_GnssSessionInfo);

      /*Since different rules have different implementation of below 3 functions, they are implemented
          in the child class. Return False from here for code  completeness only*/
      boolean eventOfInterest(TechSelEvent *pz_event){return FALSE;};
      boolean isConditionMet(TechSelEvent *pz_event){return FALSE;};
      boolean executeAction(TechSelEvent *pz_event){return FALSE;};

      /*This function is needed by all PDR\LPPM Rules and hence implemented in this base class*/
      uint32 selectLppmTbm(void);

  private:  
      /*Private Member variables*/
      void updateLppmTbmTable(uint32 q_DefaultTbmMsecs, uint32 q_InDoorsTbmMsecs, uint32 q_OutDoorTbmMsecs);
      /*LPPM TBM table*/
      uint32  mq_LppmTbmTable[GNSS_QUALITY_IND_MAX + 1][GNSS_PED_DEV_CTX_MAX + 1];
      
};


#endif /*TECH_SEL_RULE_PDR_H*/


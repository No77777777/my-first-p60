/*======================================================================

 Technology Selector Wifi assistance Rule

 GENERAL DESCRIPTION
  This file contains declaration of Wifi assistance Rule Class
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2016 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/rules/inc/tech_sel_rule_wifi_assist.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_RULE_WIFI_ASSIST_H
#define TECH_SEL_RULE_WIFI_ASSIST_H

#include "tech_sel_rules.h"
#include "tech_sel_rules_engine.h"
#include "tech_sel_rule_wifi_assist_helper.h"

/*This class acts as base class for all Wifi assistance Rules*/
class TechSelRuleWifiAssist :public TechSelRule {
  public:      
      /*Public Member Functions*/
      TechSelRuleWifiAssist(techSelRuleNum u_ruleNum);

      /*Since all PDR\LPPM Rules have same type of session interest, this function is implemented
          in this base class*/      
      boolean isSessionOfInterest(techSel_PosSessInfo *p_GnssSessionInfo);
      
      /*Since different rules have different implementation of below 3 functions, they are implemented
          in the child class. Return False from here for code  completeness only*/
      boolean eventOfInterest(TechSelEvent *pz_event){return FALSE;};
      boolean isConditionMet(TechSelEvent *pz_event){return FALSE;};
      boolean executeAction(TechSelEvent *pz_event){return FALSE;};

      /*This function is needed by all Wifi assistance Rules to start a CPI. 
             Hence implemented in base class*/
      boolean startCpi(void);
      /*This function is needed by all Wifi assistance Rules to stop a CPI. 
            Hence implemented in base class*/
      boolean stopCpi(void);
      /*This function is needed by all Wifi assistance Rules to check if CPI is running. 
            Hence implemented in base class*/      
      boolean isCpiRunning(void);

  private:
      /*Pointer to Wifi Assistance Helper Instance*/
      TechSelRuleWifiAssistHelper* m_pWifiAssistHelper;
      
};


#endif /*TECH_SEL_RULE_WIFI_ASSIST_H*/


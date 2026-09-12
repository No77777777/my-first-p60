/*======================================================================

 Technology Selector Rules Header File

 GENERAL DESCRIPTION
  This file contains declaration of Rules  Class
  
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

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/rules/inc/tech_sel_rules.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_RULES_H
#define TECH_SEL_RULES_H

#include "tech_sel_common.h"
#include "tech_sel_event.h"

/*Rule Number Enums*/
typedef enum
{
  TECHSEL_RULE_MIN,
  TECHSEL_RULE_NUM_1, /*Wifi Assistance Rule*/
  TECHSEL_RULE_NUM_2, /*Wifi Assistance Rule*/
  TECHSEL_RULE_NUM_3, /*Wifi Assistance Rule*/
  TECHSEL_RULE_NUM_4, /*Wifi Assistance Rule*/
  TECHSEL_RULE_NUM_5, /*Wifi Assistance Rule*/
  TECHSEL_RULE_NUM_6, /*Wifi Assistance Rule*/
  TECHSEL_RULE_NUM_7, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_8, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_9, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_10, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_11, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_12, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_13, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_14, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_15, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_16, /*PDR\LPPM Rule*/
  TECHSEL_RULE_NUM_17, /*PDR\LPPM Rule*/
  TECHSEL_RULE_MAX,
  TECHSEL_RULE_MAX_ALIGN = 2147483647 /* Force 32bit */ /*Added only for alignement*/
} techSelRuleNum;

/*Base class for TechSel Rules*/
/*All TechSel Rules should inherit this base class*/
class TechSelRule{
  public:
    /*Public Member Functions*/    
    TechSelRule(techSelRuleNum u_ruleNum);
    virtual ~TechSelRule(){};

    /*Function to get the Rule number of the Rule*/
    techSelRuleNum getRuleNum(void){return mu_ruleNum;};

    /*Pure virtual functions to force child class to implement these function*/
    
    /*Function to check whether the Rule is interested in the session which is active*/
    virtual boolean isSessionOfInterest(techSel_PosSessInfo *p_GnssSessionInfo) = 0;
    /*Function to check whether the Rule is interested in the event which is generated*/
    virtual boolean eventOfInterest(TechSelEvent *pz_event)= 0;
    /*Function to check whether the conditions which are required by the Rule is met*/
    virtual boolean isConditionMet(TechSelEvent *pz_event) = 0;
    /*Function to execute the action of the Rule*/
    virtual boolean executeAction(TechSelEvent *pz_event) = 0;
    
  private:  
      /*Private Member variables*/
      techSelRuleNum mu_ruleNum;
};

#endif


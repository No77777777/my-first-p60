/*======================================================================

 Technology Selector Rule 4

 GENERAL DESCRIPTION
  This file contains declaration of  Rule 4 Class
  
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

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/rules/inc/tech_sel_rule_4.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_RULE_4_H
#define TECH_SEL_RULE_4_H

#include "tech_sel_rules.h"
#include "tech_sel_rule_wifi_assist.h"
#include "tech_sel_rules_engine.h"

/*Wifi Fix Stop for Wifi Weak*/
class TechSelRule4 :public TechSelRuleWifiAssist {
  public:      
      /*Public Member Functions*/
      TechSelRule4();

      /*Implementation of the inhertited virtual functions*/
      boolean eventOfInterest(TechSelEvent *pz_event);
      boolean isConditionMet(TechSelEvent *pz_event);
      boolean executeAction(TechSelEvent *pz_event);

  private:  
      /*Private Member variables*/
      
};


#endif 

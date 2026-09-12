/*======================================================================

 Technology Selector Rule 10

 GENERAL DESCRIPTION
  This file contains declaration of  Rule 10 Class
  
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

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/rules/inc/tech_sel_rule_10.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_RULE_10_H
#define TECH_SEL_RULE_10_H

#include "tech_sel_rules.h"
#include "tech_sel_rule_pdr.h"
#include "tech_sel_rules_engine.h"

/*Rule for querying Lppm on ped entry*/
class TechSelRule10 :public TechSelRulePdr {
  public:      
      /*Public Member Functions*/
      TechSelRule10();

      /*Implementation of the inhertited virtual functions*/
      boolean isSessionOfInterest(techSel_PosSessInfo *p_GnssSessionInfo);
      boolean eventOfInterest(TechSelEvent *pz_event);
      boolean isConditionMet(TechSelEvent *pz_event);
      boolean executeAction(TechSelEvent *pz_event);

  private:  
      /*Private Member variables*/
      
};



#endif 

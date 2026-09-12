/*======================================================================

 Technology Selector Rule 14

 GENERAL DESCRIPTION
  This file contains declaration of  Rule 14 Class
  
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

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/rules/inc/tech_sel_rule_14.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_RULE_14_H
#define TECH_SEL_RULE_14_H

#include "tech_sel_rules.h"
#include "tech_sel_rule_pdr.h"
#include "tech_sel_rules_engine.h"

/*Rule for Modifying LPPM M2 TBM*/
class TechSelRule14 :public TechSelRulePdr {
  public:      
#define TECHSEL_LPPM_TBM_HYST_MSECS (5*1000) /*5 seconds*/
    
      /*Public Member Functions*/
      TechSelRule14();

      /*Implementation of the inherited virtual functions*/
      boolean isSessionOfInterest(techSel_PosSessInfo *p_GnssSessionInfo);
      boolean eventOfInterest(TechSelEvent *pz_event);
      boolean isConditionMet(TechSelEvent *pz_event);
      boolean executeAction(TechSelEvent *pz_event);

  private:  
      /*Private Member variables*/
      /*LPPM TBM hysteresis timer is started when the context changes requiring a LPPM TBM change.
        After the expiry of the time, LPPM TBM is updated. This is to avoid frequent LPPM TBM updates*/      
      os_TimerMsgType*    mz_LppmTbmHystTimer; 
};


#endif 

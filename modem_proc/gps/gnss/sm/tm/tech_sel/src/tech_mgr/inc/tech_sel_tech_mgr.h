/*======================================================================

              Technology Selector  Tech Manager Header File

 GENERAL DESCRIPTION
  This file contains declaration of Tech Manager Class
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2016 Qualcomm Technologies, Inc. 
 Copyright (c) 2019 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/tech_mgr/inc/tech_sel_tech_mgr.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
06/13/19   py    Added WIFI power surge event
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_TECH_MGR_H
#define TECH_SEL_TECH_MGR_H

#include "comdef.h"
#include "tech_sel_api.h"
#include "tech_sel_common.h"

typedef enum
{
  TECH_SEL_TECH_MGR_NONE = 0,
  TECH_SEL_TECH_MGR_GNSS, /*GNSS Technology Manager*/
  TECH_SEL_TECH_MGR_WIFI, /*Wifi Technology Manager*/
  TECH_SEL_TECH_MGR_MAX = 0xFF
} techSel_TechMgrType;

/*GNSS Strength Enum*/
typedef enum
{
  TECHSEL_GNSS_STRENGTH_UNKNOWN, /*GNSS strength Uknown*/
  TECHSEL_GNSS_STRENGTH_WEAK, /*GNSS strength Weak*/
  TECHSEL_GNSS_STRENGTH_STRONG, /*GNSS strength Strong*/
  TECHSEL_GNSS_STRENGTH_MAX = 2147483647 /* Force 32bit */
}techSelGnssStrength;


/*Wifi Strength Enum*/
typedef enum
{
  TECHSEL_WIFI_STRENGTH_UNKNOWN, /*Wifi Strength Unknown*/
  TECHSEL_WIFI_STRENGTH_WEAK, /*Wifi Strength Weak*/
  TECHSEL_WIFI_STRENGTH_STRONG, /*Wifi Strength Strong*/
  TECHSEL_WIFI_STRENGTH_POWER_SURGE, /*Wifi Generic event to stop CPI immediately*/
  TECHSEL_WIFI_STRENGTH_MAX = 2147483647 /* Force 32bit */
}techSelWifiStrength;

/*Technology Manager Base Class*/
class TechSelTechMgr{
  public:
    TechSelTechMgr(techSel_TechMgrType techMgrType): 
      mTechMgrType(techMgrType){};
    
    virtual ~TechSelTechMgr(){};    
    
  private:
      /*Technology Manager Type*/
      techSel_TechMgrType mTechMgrType;
};

#endif /*TECH_SEL_TECH_MGR_H*/

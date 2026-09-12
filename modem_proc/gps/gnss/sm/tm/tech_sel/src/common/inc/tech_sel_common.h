/*======================================================================

  Technology Selector Common Header File

 GENERAL DESCRIPTION
  This file contains common declarations that Tech Selector shall use
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2016 - 2021 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/common/inc/tech_sel_common.h#2 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
12/15/21   sai   Added new NV for ODCPI and change in ODCPI back off logic
03/02/16   ss    Initial creation of file.
======================================================================*/
#ifndef TECH_SEL_COMMON_H
#define TECH_SEL_COMMON_H
#include "comdef.h"
#include "tech_sel_ext_intf.h"

/* Tech Selector MSG Macros */
#define TECHSEL_MSG_MASK_LOW    MSG_LEGACY_LOW
#define TECHSEL_MSG_MASK_MED    MSG_LEGACY_MED
#define TECHSEL_MSG_MASK_HIGH   MSG_LEGACY_HIGH
#define TECHSEL_MSG_MASK_ERROR  MSG_LEGACY_ERROR
#define TECHSEL_MSG_MASK_FATAL  MSG_LEGACY_FATAL

#define TECHSEL_MSG_0(lvl,a)                    MSG(MSG_SSID_GPSSM,lvl,a)
#define TECHSEL_MSG_1(lvl,a,b)                  MSG_1(MSG_SSID_GPSSM,lvl,a,b)
#define TECHSEL_MSG_2(lvl,a,b,c)                MSG_2(MSG_SSID_GPSSM,lvl,a,b,c)
#define TECHSEL_MSG_3(lvl,a,b,c,d)              MSG_3(MSG_SSID_GPSSM,lvl,a,b,c,d)
#define TECHSEL_MSG_4(lvl,a,b,c,d,e)            MSG_4(MSG_SSID_GPSSM,lvl,a,b,c,d,e)
#define TECHSEL_MSG_5(lvl,a,b,c,d,e,f)          MSG_5(MSG_SSID_GPSSM,lvl,a,b,c,d,e,f)
#define TECHSEL_MSG_6(lvl,a,b,c,d,e,f,g)        MSG_6(MSG_SSID_GPSSM,lvl,a,b,g,d,e,f,g)
#define TECHSEL_MSG_7(lvl,a,b,c,d,e,f,g,h)      MSG_7(MSG_SSID_GPSSM,lvl,a,b,c,d,e,f,g,h)
#define TECHSEL_MSG_8(lvl,a,b,c,d,e,f,g,h,i)    MSG_8(MSG_SSID_GPSSM,lvl,a,b,c,d,e,f,g,h,i)
#define TECHSEL_MSG_9(lvl,a,b,c,d,e,f,g,h,i,j)  MSG_9(MSG_SSID_GPSSM,lvl,a,b,c,d,e,f,g,h,i,j)

/* Add Tech Selector MSG Sprintf Macros here when needed */

typedef enum
{
  TECHSEL_TIMER_ID_START = TM_TECHSEL_TIMER_ID_RANGE,

  /*GNSS Tech Manager Timers*/
  TECHSEL_GNSS_TECH_MGR_TIMER_ID_START = TECHSEL_TIMER_ID_START,
  TECHSEL_GNSS_TECH_MGR_POS_RETRY_TIMER_ID = TECHSEL_GNSS_TECH_MGR_TIMER_ID_START,
  TECHSEL_GNSS_TECH_MGR_TIMER_ID_MAX = TECHSEL_GNSS_TECH_MGR_POS_RETRY_TIMER_ID,

  /*Wifi Tech Manager Timers*/
  TECHSEL_WIFI_TECH_MGR_TIMER_ID_START,

  /* Timer to keep track of the max interval for batched wifi scan collection */
  TECHSEL_WIFI_TECH_MGR_BATCH_WIFI_SCAN_MAX_INTERVAL_TIMER_ID = TECHSEL_WIFI_TECH_MGR_TIMER_ID_START,
  TECHSEL_WIFI_TECH_MGR_TIMER_ID_MAX = TECHSEL_WIFI_TECH_MGR_BATCH_WIFI_SCAN_MAX_INTERVAL_TIMER_ID,

  TECHSEL_RULES_TIMER_ID_START,
  /*Gnss Strength Hysteresis Timer is started as soon as Gnss strength changes. At expiry
      of the timer, Gnss strength event is generated.*/
  TECHSEL_RULES_GNSS_STRENGTH_HYST_TIMER_ID = TECHSEL_RULES_TIMER_ID_START,    
  /*LPPM TBM hysteresis timer is started when the context changes requiring a LPPM TBM change.
    After the expiry of the time, LPPM TBM is updated. This is to avoid frequent LPPM TBM updates*/
  TECHSEL_RULES_LPPM_TBM_HYST_TIMER_ID,
  /*Wifi Weak Hysteresis Timer is started as soon as Wifi weak is detected. After the timer expiry,
    Wifi Weak event is generated.*/
  TECHSEL_RULES_WIFI_WEAK_HYST_TIMER_ID,  
  /*Wifi unknown hysteresis timer is started when wifi fix request fails and Wifi weak state is detected(no wifi fix is injected).
    After expiry of the timer, wifi strength is considered Unknown*/
  TECHSEL_RULES_WIFI_UNKNOWN_HYST_TIMER_ID,  
  /*Wifi position wait timer is started right after requesting a Wifi position fix. After the timer expiry,
    Wifi weak is detected*/
  TECHSEL_RULES_WIFI_POS_WAIT_TIMER_ID,  
  /*ODCPI Back Off timer to avoided CPIs for Back Off duration
  */
  TECHSEL_RULES_ODCPI_BACK_OFF_TIMER_ID,  
  TECHSEL_RULES_TIMER_ID_MAX = TECHSEL_RULES_ODCPI_BACK_OFF_TIMER_ID
} techSelTimerIdType;


#endif


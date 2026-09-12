#ifndef _UIMAGE_TRANS_H_
#define _UIMAGE_TRANS_H_
/*==============================================================================
  FILE:         uimage_trans.h

  OVERVIEW:     Types & prototypes for internal uimage transition functions

  DEPENDENCIES: None
  
                Copyright (c) 2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/uSleep/src/transition/uimage_trans.h#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "icbarb.h"

/*==============================================================================
                                        TYPES
 =============================================================================*/
#define MAX_COMMANDS_IN_SET  8  //worst case as given by bus team

typedef struct uSleep_icb_data_s
{
  uint16              numInCurrentSet;
  uint16              numInOffSet;
  //rpmh_command_set_t  currentCmdSet[MAX_COMMANDS_IN_SET];
  //rpmh_command_set_t  offCmdSet[MAX_COMMANDS_IN_SET];
}uSleep_icb_data;

typedef struct uSleep_transition_data_s
{
  uSleep_icb_data     icb;
}uSleep_transition_data;

/*==============================================================================
                        NORMAL MODE FUNCTION DECLARATIONS
 =============================================================================*/
/**
 * uImageTrans_initialize
 *
 * @brief Performs any setup required to trigger uImage entry
 */ 
void uImageTrans_initialize(void);

/** 
 * uImageTrans_prepareForEntry
 * 
 * @brief Perform any setup and begin the transition to uImage mode
 */
void uImageTrans_prepareForEntry(void);

#endif /* _UIMAGE_TRANS_H_ */


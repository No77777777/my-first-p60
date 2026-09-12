#ifndef _USLEEP_NPA_H_
#define _USLEEP_NPA_H_
/*==============================================================================
  FILE:         uSleep_npa.h

  OVERVIEW:     Types & prototypes for the various uSleep node requirements 

  DEPENDENCIES: None
  
                Copyright (c) 2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/uSleep/src/unpa_nodes/uSleep_npai.h#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"

/*==============================================================================
                           FUNCTION DEFINITIONS
 =============================================================================*/
/** 
 * uSleepUNPA_initialize
 * 
 * @brief Initializes the internal power restriction nodes
 */
void uSleepUNPA_initialize(void);

#endif /* _USLEEP_NPA_H_ */


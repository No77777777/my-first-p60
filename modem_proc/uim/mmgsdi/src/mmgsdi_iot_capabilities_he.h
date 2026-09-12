#ifndef MMGSDI_IOT_CAPABILITIES_HE_H
#define MMGSDI_IOT_CAPABILITIES_HE_H

/*
  ===========================================================================

  Copyright (c) 2017, 2019 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

  ===========================================================================*/
/*===========================================================================
                        EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
12/24/19   cj      Update EF TST byte 11  
01/31/17   nc      Initial revision
===========================================================================*/

#include "mmgsdi_nv.h"
#include "mmgsdi_iot.h"
/*----------------------------------------------------------------------------
 * MODULE: IOT FEATURES WITH ENABLED OR DISABLED VALUE
 *--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 *  Refer to the FR 37084 design document for details on individual FRs and PoCs
 *  All IOT related features that are implemented or not implemented by
 *  NAS/LTE/MMODE shall be defined in this header file.
 *  
 *  when a new header file is created for a new target the feature status
 *  shall be determined based on the FR details and also by communicating
 *  with the tech teams
 *--------------------------------------------------------------------------*/
/* NAS FR 29014 CR 902822 */
#define FEATURE_MMGSDI_NAS_SIG_PRIORITY_SUPPORT                               MMGSDI_FEATURE_ENABLED  
/* NAS FR 29016 CR 905857 */                                                                                           
#define FEATURE_MMGSDI_MINIMUM_PER_SEARCH_TIMER_SUPPORT                       MMGSDI_FEATURE_ENABLED 
/* NAS FR 33609 NAS CR 970428 */                             
#define FEATURE_MMGSDI_EXTENDED_ACCESS_BARRING_SUPPORT                        MMGSDI_FEATURE_DISABLED    
/* NAS FR 2831 CR 411128 */                                    
#define FEATURE_MMGSDI_TIMER_T3245_SUPPORT                                    MMGSDI_FEATURE_ENABLED  
/* Not supported by NAS hence keep it as disabled*/     
#define FEATURE_MMGSDI_OVERRIDE_NAS_SIG_LOW_PRIORITY_SUPPORT                  MMGSDI_FEATURE_DISABLED                           
#define FEATURE_MMGSDI_OVERRIDE_EXTENDED_ACCESS_BARRING_SUPPORT               MMGSDI_FEATURE_DISABLED  
/* DATA FR 31709 */ 
#define FEATURE_MMGSDI_SM_RETRY_WAIT_TIME_SUPPORT                             MMGSDI_FEATURE_ENABLED  
/* NAS FR 29014 CR 902822 */    
#define FEATURE_MMGSDI_DELAY_TOLERANT_ACCESS_SUPPORT                          MMGSDI_FEATURE_ENABLED 
/* Based on 73769 EFS/NV */ 
#define FEATURE_MMGSDI_POWER_SAVING_MODE_PSM_SUPPORT                          MMGSDI_FEATURE_ENABLED   
/* Coverage Enhancement Mode A (CEModeA) support feature enabled */                                              
#define FEATURE_MMGSDI_COVERAGE_ENH_MODE_A_SUPPORT                            MMGSDI_FEATURE_DISABLED 
/* Coverage Enhancement Mode b (CEModeB) support feature disabled */                                                     
#define FEATURE_MMGSDI_COVERAGE_ENH_MODE_B_SUPPORT                            MMGSDI_FEATURE_DISABLED 
/* FR 29021, based on NV/EFS #73871*/                                                       
#define FEATURE_MMGSDI_IDLE_MODE_EXT_DIS_RECEPTION_SUPPORT                    MMGSDI_FEATURE_ENABLED  
/* Not supported hence keep it disabled */
#define FEATURE_MMGSDI_CONNECTED_MODE_EXT_DIS_RECEPTION_SUPPORT               MMGSDI_FEATURE_DISABLED
/* Device Based Positioning support feature enabled */
#define FEATURE_MMGSDI_DEVICE_BASED_POSITIONING_SUPPORT                       MMGSDI_FEATURE_ENABLED  
/* Not supported hence keep it disabled */                                  
#define FEATURE_MMGSDI_SINGLE_CELL_POINT_TO_MULTIPOINT_TRANSMISSION_SUPPORT   MMGSDI_FEATURE_DISABLED     

#endif /* MMGSDI_IOT_CAPABILITIES_HE_H */


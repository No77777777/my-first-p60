#ifndef ONEX_M1X_RM_MCPM_H
#define ONEX_M1X_RM_MCPM_H
/*===========================================================================

       C D M A  I N T E R F A C E   W I T H   M C P M    M O D U L E

DESCRIPTION
  This contains all the data and API declarations for the interface with 
  MCPM (Modem Clock and Power Manager) module.

  Copyright (c) 2018 - 2020 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/mccdma.h_v   1.53   02 Oct 2002 13:49:34   sbrelsfo  $
$Header: //components/rel/1x.mpss/8.0/api/public/onex_m1x_rm_mcpm.h#2 $ $DateTime: 2020/02/06 06:26:14 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/06/20   dmd     Add new scenarios for finger enablement
07/25/18   ppr     Initial Version

===========================================================================*/

/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/


/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/



/*--------------------------------------------------------------------------
            ONEX to MCPM CLOCK 2.0 SCENARIO ENUM TYPE
--------------------------------------------------------------------------*/
/* Clock 2.0 ONEX Scenarios Enums */
typedef enum
{
  ONEX_SCENARIO_NONE                      = 0,

  ONEX_SRCH_IDLE_X21X_QTA_STOP            = 1,

  ONEX_SRCH_TC_1X2X_QTA                   = 2,

  ONEX_SRCH_TC_1X_SRCH_REQ                = 3,

  ONEX_SCENARIO_1X_VOICE_CALL             = 4,

  ONEX_SCENARIO_1X_DATA_CALL              = 5,
  
  ONEX_SRCH_FING_INIT                     = 6,

  ONEX_SRCH_FING_NOM                      = ONEX_SRCH_FING_INIT,
  
  ONEX_SRCH_FING_MED                      = 7,
  
  ONEX_SRCH_FING_HIGH                     = 8,
  
  ONEX_SRCH_FING_NOM_MED                  = 9,
  
  ONEX_SRCH_FING_MED_NOM                  = 10,
  
  ONEX_SRCH_FING_HIGH_NOM                 = 11,
  
  ONEX_SRCH_FING_NOM_HIGH                 = 12,
  
  ONEX_SRCH_FING_MED_HIGH                 = 13,
  
  ONEX_SRCH_FING_HIGH_MED                 = 14,

  ONEX_SCENARIO_MAX

} onex_m1x_rm_mcpm_scenario_type; 



/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/



#endif /* ONEX_M1X_RM_MCPM_H */

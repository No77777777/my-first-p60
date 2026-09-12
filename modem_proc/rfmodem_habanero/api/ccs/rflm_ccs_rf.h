#ifndef RFLM_CCS_RF_H
#define RFLM_CCS_RF_H

/*!
  @file
  rflm_ccs_rf.h

  @brief
  RFLM CCS RF driver

  @detail
  API for RFLM CCS Processor and Data Manager RF support. 

*/
/*===========================================================================

  Copyright (c) 2013 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

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
/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfmodem_habanero.mpss/1.11/api/ccs/rflm_ccs_rf.h#1 $

when         who     what, where, why
----------   ---     -----------------------------------------------------------
12/07/15     sbo     Check for CCS awake status
11/13/13     ra      Move to protected
11/11/13     Saul    CCS. Fixed Warnings.
09/03/13     ra      Exported api and definitions to RFLM API CCS folder
07/16/13     ra      Initial port to Bolt RF SW
==============================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "rflm_ccs_rf_intf.h"

// XXX Include deprecated code until techs can clean up
#define RFLM_CCS_SW_PORT_TQ_INDEX 4

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==========================================================================*/

/* flag to indicate if CCS is awake */
extern volatile boolean rf_hal_ccs_is_awake;

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*----------------------------------------------------------------------------*/
boolean
rflm_ccs_start
(
   boolean bypass_vstmr_int_reg
);

#endif /* RFLM_CCS_RF_H */

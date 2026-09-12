
/*!
  @file lte_qmi_msgr.h

  @brief
  Internal variables of the LTE QMI MSGR.
  
*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/lte.mpss/6.6.1/qmi/src/lte_qmi_msgr.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/15   psv     Initial Revision

===========================================================================*/

#ifndef LTE_QMI_MSGR_H
#define LTE_QMI_MSGR_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include <stm2.h>
#include "lte_qmi_task.h"
#include <lte_rrc_ext_msg.h>
#ifdef FEATURE_LTE_DISCOVERY
#include <prose_ext_msg.h>
#include <ds_lted_ext_msg.h>
#endif
#include "lte_qmi_utils.h"

//Define pending queue for LTE QMI if needed

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

extern msgr_umid_type lte_qmi_msgr_umid_list[];

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

void lte_qmi_msgr_init
(
  void
);

void lte_qmi_msgr_process_msgr_sig
(
  void
);

void lte_qmi_msgr_handler( void *payload );


#endif /* LTE_QMI_MSGR_H */

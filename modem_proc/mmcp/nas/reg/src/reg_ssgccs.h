/*===========================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated. All Rights Reserved

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

#ifndef REG_SSGCCS_H
#define REG_SSGCCS_H

#include "sys.h"

/*========================================================================
  FUNCTION reg_ssgccs_send_ehplmn_and_fplmn

  DESCRIPTION
    Send ehplmn and fplmn information

  PARAMETERS
    None

  RETURN VALUE
    None
===========================================================================*/
void reg_ssgccs_send_ehplmn_and_fplmn
(
  void
);

/*========================================================================
  FUNCTION reg_ssgccs_send_camp_status

  DESCRIPTION
    send camp status

  PARAMETERS
    roaming_ind [in] - camp status out

  RETURN VALUE
    None
===========================================================================*/
void reg_ssgccs_send_camp_status
(
  sys_roam_status_e_type roaming_ind,
    sys_plmn_id_s_type   plmn
);

#endif /* REG_SSGCCS_H */

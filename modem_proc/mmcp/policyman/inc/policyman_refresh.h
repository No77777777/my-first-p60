#ifndef _POLICYMAN_REFRESH_H_
#define _POLICYMAN_REFRESH_H_

/**
  @file policyman_refresh.h

  @brief  Handling for PM refresh on SIM swap
*/

/*
    Copyright (c) 2014 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/policyman_refresh.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#ifdef FEATURE_MODEM_CONFIG_REFRESH
#include "mcfg_refresh.h"

/*------------------------------------------------------------------------
  FUNCTION POLICYMAN_REGISTER_FOR_MCFG_REFRESH()

  DESCRIPTION
    With this function, Policy Manager registers with MCFG refresh events as a client
--------------------------------------------------------------------------*/
void policyman_register_for_mcfg_refresh(
  void
);


/*------------------------------------------------------------------------
  FUNCTION policyman_in_refresh_phase()

  DESCRIPTION
    With this function, Policy Manager return refresh information
--------------------------------------------------------------------------*/
boolean policyman_in_refresh_phase(
  void
);


#endif /* FEATURE_MODEM_CONFIG_REFRESH */

#endif /* _POLICYMAN_REFRESH_H_ */

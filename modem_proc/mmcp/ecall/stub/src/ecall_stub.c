
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                    E C A L L   S T U B   A P I S

GENERAL DESCRIPTION
  This file contains stubs for APIs exposed to other modules. These
  stubs will only be compiled and available in non-eCall builds.
  This file should be used to provide stubs for modules not part of 
  Modem such as Audio.

  Copyright (c) 1990 - 2015 Qualcomm Technologies, Inc.
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
 
  $Header: //components/rel/mmcp.mpss/7.9.0/ecall/stub/src/ecall_stub.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
 
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/

#include "sys.h"
#include "ecall_voice_if.h"

/*===========================================================================
                             STUB APIs
===========================================================================*/

/*===========================================================================

STUB_FUNCTION ECALL_VOICE_API

DESCRIPTION
  STUB API for ecall_voice_api function which is used by Voice Adapter to
  communicate with eCall. The stub API will always return
  ECALL_VOICE_EUNSUPPORTED.
  
DEPENDENCIES
  None.

RETURN VALUE
  ECALL_VOICE_EUNSUPPORTED

SIDE EFFECTS
  None.

  
===========================================================================*/
uint32 ecall_voice_api (uint32 cmd_id, void* params, uint32 size)
{
  return ECALL_VOICE_EUNSUPPORTED;
}

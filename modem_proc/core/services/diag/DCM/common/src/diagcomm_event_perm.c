/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
					Diagnostic event filtering using SSM
                   

General Description
This file contains the default event permissions passed in while initializing 
with SSM. 
 
Copyright (c) 2013 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                              Edit History

 $Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diagcomm_event_perm.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/03/13   sr      Permissions in fallback buffer are mentioned in this file

===========================================================================*/

#include "core_variation.h"
#include "comdef.h"
#include <string.h>

/*Fallback buffer permissions for events in the format:
  event_id_start|event_id_end|permission|
  permission value 0: allow
  permission value 1" dont allow
  If the permission file is not present in SFS, then these are the default permissions that are used*/

const char * diag_ssm_event_fb = "0|4095|0|";
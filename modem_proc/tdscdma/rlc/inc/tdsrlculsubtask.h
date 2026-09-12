#ifndef TDSRLCULSUBTASK_H
#define TDSRLCULSUBTASK_H
/*===========================================================================

          U P L I N K   R L C : S U B T A S K   H E A D E R   F I L E 

                
GENERAL DESCRIPTION
  
  This module contains the declaration of the functions and data types
  defined in downlink RLC subtask file.
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.
$Header: //components/rel/tdscdma.mpss/4.3/rlc/inc/tdsrlculsubtask.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

   
when       who     what, where, why
--------   ---     ----------------------------------------------------------    
10/24/10   guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/

/*===========================================================================

FUNCTION PROC_UL_RLC_COMMANDS

DESCRIPTION
  Processes Uplink RLC Commands.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsproc_ul_rlc_commands 
(   
  tdsl2_ul_cmd_enum_type     cmd_id,             /* Command ID                */  
  tdsl2_ul_cmd_data_type     *cmd_data_ptr       /* Pointer to command data   */    
);
#endif  /* TDSRLCULSUBTASK_H */



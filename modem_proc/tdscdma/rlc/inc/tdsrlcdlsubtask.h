#ifndef TDSRLCDLSUBTASK_H
#define TDSRLCDLSUBTASK_H
/*===========================================================================

    D O W N L I N K   R L C : S U B T A S K   H E A D E R   F I L E 

                
GENERAL DESCRIPTION
  
  This module contains the declaration of the functions and data types
  defined in downlink RLC subtask file.
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

===========================================================================*/
/*===========================================================================

FUNCTION PROC_DL_RLC_COMMANDS

DESCRIPTION
  Processes Downlink RLC Commands.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void  tdsproc_dl_rlc_commands 
(   
  tdsl2_dl_cmd_enum_type     cmd_id,            /* Command ID                 */    
  tdsl2_dl_cmd_data_type     *cmd_data_ptr      /* Pointer to command data    */   
);
     
#endif /* TDSRLCDLSUBTASK_H */


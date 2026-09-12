#ifndef RLCDLSUBTASK_H
#define RLCDLSUBTASK_H
/*===========================================================================

    D O W N L I N K   R L C : S U B T A S K   H E A D E R   F I L E 

                
GENERAL DESCRIPTION
  
  This module contains the declaration of the functions and data types
  defined in downlink RLC subtask file.
  
Copyright (c) 1992-2001 by Qualcomm Technologies, Inc.  All Rights Reserved.
Copyright (c) 2008-2009 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/16/15   aa      L2 to pass/process sub-id correctly when DUAL_WCDMA feature is disabled.
03/10/15   aa      Duplicated L2 task related globals and removed AS_ID hardcodings
01/29/15   aa      L2 global reorg for W+W
10/18/10   ssg     Added changes for new WCDMA-L2 interface.
05/11/09   ssg     Updated Copyright Information
05/21/01   rc      Cleaned up code.
11/22/00   rc      Created file.

===========================================================================*/
#include "wcdma_variation.h"
#include "rlcul.h"
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
void  proc_dl_rlc_commands 
(   
  sys_modem_as_id_e_type as_id,
  l2_dl_cmd_enum_type     cmd_id,            /* Command ID                 */    
  l2_dl_cmd_data_type     *cmd_data_ptr      /* Pointer to command data    */   
);
     
#ifdef FEATURE_WCDMA_NEW_L2_INTERFACE
/*===========================================================================

FUNCTION PROC_WCDMA_DL_RLC_COMMANDS

DESCRIPTION
  Processes Downlink RLC Commands.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void  proc_wcdma_dl_rlc_commands 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  wcdma_l2_dl_cmd_enum_type     cmd_id,            /* Command ID                 */    
  wcdma_l2_dl_cmd_data_type     *cmd_data_ptr      /* Pointer to command data    */ 
);
#endif /*FEATURE_WCDMA_NEW_L2_INTERFACE*/

#endif /* RLCDLSUBTASK_H */


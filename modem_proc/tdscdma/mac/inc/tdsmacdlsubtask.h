#ifndef TDSMACDLSUBTASK_H
#define TDSMACDLSUBTASK_H
/*================================================================================

    D O W N L I N K   M A C : S U B T A S K   H E A D E R   F I L E


GENERAL DESCRIPTION

  This module contains the declaration of the functions and data types
  defined in downlink MAC subtask file.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/macdlsubtask.h_v   1.5   11 Jun 2002 18:01:34   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/inc/tdsmacdlsubtask.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------------
10/24/10   guo   Initial release Upmerged WCDMA to TDS

==================================================================================*/
/*==============================================================================================
 FUNCTION NAME  tdsinit_dl_mac()

 PURPOSE  Initializes the DL MAC parameters related to ciphering

 DESCRIPTION
===============================================================================================*/
void tdsinit_dl_mac(void);

/*==============================================================================================
 FUNCTION NAME  tdsproc_dl_mac_commands

 PURPOSE        Process DL MAC Commands

 DESCRIPTION
===============================================================================================*/
void    tdsproc_dl_mac_commands
(
    /* Command ID */
    tdsl2_dl_cmd_enum_type     cmd_id,

    /* Pointer to command data */
    tdsl2_dl_cmd_data_type     *cmd_data_ptr

);

/*==============================================================================================
 FUNCTION NAME  tdsproc_dl_mac_config_signal

 FUNCTION   This function processes the config signal received from the physical layer.
===============================================================================================*/
void tdsproc_dl_mac_config_signal (void);



/*==============================================================================================
 FUNCTION NAME  tdsproc_dl_mac_hs_config_signal

 FUNCTION   This function processes the MAC-hs config signal received from the physical layer.
===============================================================================================*/
void tdsproc_dl_mac_hs_config_signal (void);


/*==============================================================================================
 FUNCTION NAME  tdsproc_dl_mac_config_at_l1_failure_signal

 FUNCTION   This function processes the config failed signal received from the physical layer so
            that mac_hs_reset can be performed if pending.
===============================================================================================*/
void tdsproc_dl_mac_config_at_l1_failure_signal (void);


#endif


#ifndef TDSMACULSUBTASK_H
#define TDSMACULSUBTASK_H
/*===============================================================================================

          U P L I N K   M A C : S U B T A S K   H E A D E R   F I L E 

                
GENERAL DESCRIPTION
  
  This module contains the declaration of the functions and data types
  defined in downlink MAC subtask file.
  
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


================================================================================================*/

/*================================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mac/vcs/maculsubtask.h_v   1.8   10 Jul 2002 10:06:28   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/inc/tdsmaculsubtask.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

===============================================================================================*/
#include "tdsl2ultask.h"

#ifdef FEATURE_TDSCDMA_CRITICAL_SECTION_CLEANUP
/*================================================================================================
 FUNCTION NAME  tdsinit_ul_mac_crit_sect

 DESCRIPTION  This function initialises UL MAC
================================================================================================*/
void  tdsinit_ul_mac_crit_sect(void);
#endif
/*================================================================================================
 FUNCTION NAME  tdsinit_ul_mac

 PURPOSE        Initialize UL MAC 

================================================================================================*/
void    tdsinit_ul_mac 
(
    void
);

/*===============================================================================================
 FUNCTION NAME  tdsproc_ul_mac_commands

 PURPOSE        Process UL MAC Commands

 DESCRIPTION    
================================================================================================*/
void    tdsproc_ul_mac_commands 
(
    /* Command ID */
    tdsl2_ul_cmd_enum_type     cmd_id, 
    
    /* Pointer to command data */
    tdsl2_ul_cmd_data_type     *cmd_data_ptr
    
);


/*=================================================================================================
 FUNCTION NAME  tdsmac_ul_handle_traffic_volume_tmr()

 PURPOSE    This function process expiry of the control duration
================================================================================================*/

void tdsmac_ul_handle_tfcs_control_timer_expiry(void);



/*================================================================================================
 FUNCTION NAME  tdsproc_ul_mac_hfn_req

 PURPOSE        Process UL MAC HFN Request

 PARAMETERS     None

 DESCRIPTION    
================================================================================================*/
extern  void    tdsproc_ul_mac_hfn_req 
(
    void
);

/*===============================================================================================
 FUNCTION NAME  tdsproc_ul_tx_status_ind 
 
 DESCRIPTION    This function processes transmit status received from PHY layer
    
===============================================================================================*/
extern  void    tdsproc_ul_tx_status_ind
(
    tdsl1_prach_status_enum_type   rach_tx_status_ind
);

/*==============================================================================================
 FUNCTION NAME  tdsproc_build_ul_frame_ind

 FUNCTION       This function processes build UL frame event
===============================================================================================*/
extern  void    tdsproc_build_ul_frame_ind
(
    void
);



/*===============================================================================
 FUNCTION NAME  tdsmac_ul_handle_tvm_event_tmr

 PURPOSE    This function is called when MAC traffic volume timer signal
            expires every 10ms. This is used as a base timer and all the other
            timers like trigger_timer..etc are done using counters.
            This function checks for each event mode measurement identity , if the
            timer counters have expired and posts the measurement report to RRC.
================================================================================*/

void tdsmac_ul_handle_tvm_event_tmr (void);


/*===============================================================================
 FUNCTION NAME  tdsmac_ul_handle_tvm_periodic_tmr

 PURPOSE    This function is called when MAC traffic volume timer signal
            expires every (10ms* TDSMAC_UL_PERIODIC_TVM_TIMER_SCALE). This is used as a 
            base timer and report_interval is computed using counters.
            This function checks for each periodic mode measurement identity if the 
            timer counters have expired posts the measurement report to RRC.
================================================================================*/

void tdsmac_ul_handle_tvm_periodic_tmr (void);



/*==============================================================================================
 FUNCTION NAME  tdsproc_ul_mac_config_signal

 FUNCTION   This function processes the config signal received from the physical layer.
===============================================================================================*/
void tdsproc_ul_mac_config_signal (void);

                                         

#endif  /* TDSMACULSUBTASK_H */



#ifndef CMKPI_H
#define CMKPI_H
/*===========================================================================

 C A L L   M A N A G E R   K E Y   P E R F O R M A N C E   I N D I C A T O R

                       H E A D E R   F I L E

DESCRIPTION
GENERAL DESCRIPTION
  


Copyright (c) 2012 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/



/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/api/public/cmkpi.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/
#include <msgr.h>
#include <msgr_umid.h>
#include <sys.h>
#include "cm_qsh_ext.h"
#include "qsh_ext.h"
#include "cmdbg_qsh.h"
#include "cm.h"
#include "cmidbg.h"
#ifdef FEATURE_DISABLED_HABANERO
#include "qevent.h"
#endif

typedef struct
{
uint16 lac_tac;                           /* For GWT, LAC; for L, TAC */
uint8 plmn[3];                           
/*       |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  
         |============================= |
 octet 1 |      MCC Digit 2      |      MCC Digit 1   |
         |-------------------------------------------------|
 octet 2 |      MNC Digit 3      |      MCC Digit 3    |
         |-------------------------------------------------|
 octet 3 |      MNC Digit 2      |      MNC Digit 1   |
         |==============================| */

uint32 Cell_id; /* 0Xffffffff (Invalid Cell Id) */
}cmkpi_3gpp_id_s_type;

typedef struct 
{
  msgr_hdr_s hdr;  /* Sub ID reported here */
  uint8 sys_mode;   /* CDMA,HDR,GSM,UMTS,TDSCDMA,LTE*/
  sys_srv_status_e_type service_status;
  cmkpi_3gpp_id_s_type  gpp_id;
} cmkpi_loc_id_stype;


/*==========================================================================

FUNCTION: cmkpi_oprt_mode

DESCRIPTION
  This function calls into the QSH API to start/stop power-up/airplane mode
  exit to attach measurements.
  KPI: Power-up/airplane mode exit to attach.
  Trigger: LPM exit.
  Ending: 

DEPENDENCIES
  

RETURN VALUE
 

SIDE EFFECTS
  None
==========================================================================*/
extern void cmkpi_oprt_mode(sys_oprt_mode_e_type oprt_mode,
							 sys_modem_as_id_e_type asubs_id);


/*==========================================================================

FUNCTION: cmkpi_time_in_oos

DESCRIPTION
  This function calls into the QSH API to start/stop time in OOS measurements.
  KPI: Time in OOS.
  Trigger: OOS.
  Ending: Full or limted serivce.

DEPENDENCIES
  

RETURN VALUE
 

SIDE EFFECTS
  None
==========================================================================*/
extern void cmkpi_time_in_oos(sys_srv_status_e_type srv_status,
							  sys_modem_as_id_e_type asubs_id);

/*==========================================================================

FUNCTION: cmkpi_time_in_ltd_srv

DESCRIPTION
  This function calls into the QSH API to start/stop limited service measurements.
  KPI: Time in limited service.
  Trigger: Limited service.
  Ending: Full service..

DEPENDENCIES
  

RETURN VALUE
 

SIDE EFFECTS
  None
==========================================================================*/
extern void cmkpi_time_in_ltd_srv(sys_srv_status_e_type srv_status,
							  sys_modem_as_id_e_type asubs_id);
/*==========================================================================

FUNCTION: cmkpi_sim_ready

DESCRIPTION
  This function calls into the QSH API to start/stop sim ready measurements.
  KPI: SIM ready.
  Trigger: online.
  Ending: Sim ready.

DEPENDENCIES
  

RETURN VALUE
 

SIDE EFFECTS
  None
==========================================================================*/
extern void cmkpi_sim_ready(sys_modem_as_id_e_type asubs_id);

/*==========================================================================

FUNCTION: cmkpi_qsh_event_handler

DESCRIPTION
  This function calls into the QSH API for VOLTE/VOWIFI call event and CS call end event handling
  KPI: VOLTE MO/MT, VOWIFI MO/MT, VOLTE/VOWIFI call end, SRVCC, CS call release

DEPENDENCIES
  

RETURN VALUE
 none

SIDE EFFECTS
  None
==========================================================================*/
extern void cmkpi_qsh_event_handler
(
    cm_call_event_e_type call_event, 
    cmcall_s_type* call_ptr
);

/*==========================================================================

FUNCTION: cmkpi_service_status_handler

DESCRIPTION
The cmkpi_service_status_handler helps :
1. Trigger the end and fail events for Power-up/airplane mode exit to 
   attach KPI.
2. Trigger start and end for Time in OOS KPI.
3. Trigger start and end for Time in Limited Service KPI

DEPENDENCIES

RETURN VALUE
 

SIDE EFFECTS
  None
==========================================================================*/
extern void cmkpi_service_status_handler
(
  sys_srv_status_e_type srv_status,
  sys_modem_as_id_e_type asubs_id,
  sys_roam_status_e_type roaming_status,
  multimode_stack_e_type stk_id,
  sys_sys_mode_e_type sys_mode
);


#endif /* CMKPI_H */

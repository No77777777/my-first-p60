#ifndef RRCRCR_H
#define RRCRCR_H
/*===========================================================================
              R R C R C R  H E A D E R  F I L E

DESCRIPTION
 The RRCRCR module consists of RRC CONNECTION RELEASE PROCEDURE 
 implementation. The 3GPP TS 25.331 RRC Protocol specification is used as
 basis for implementation.
  
Copyright (c) 2001, 2003-2005, 2007-2009  Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rrc/vcs/rrcrcr.h_v   1.0   07 Feb 2001 18:29:54   rjeenaga  $
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrcrcr.h#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/04/16   gv      Made changes to attempt freq only acq after long LTA gap
01/28/14   bc      Changes to try ACQ on the frequency provided in connection release during leaving
                   connected mode scan when rplmn info is present in connection release.
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
04/07/10   gkg     ASN.1 migration to 8.a.0
03/15/10   as      Updated RRC Code to support ASN.1 8.9 version
10/07/09   gkg     Changes for REL8 under FEATURE_WCDMA_REL8
05/08/09   ss      Updated Copyright Information
01/19/08   ps      Made changes for ASn1 migration 25.331v7.9  
                   i.e.May 2008 version  (2008-05) 
11/09/05   kc      Added prototype to update release cause var
06/01/04   vk      Declared new fn rrcrcr_is_proc_active() that specifies if
                   RCR is active
02/06/01   rj      Created file.

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "rrccmd_v.h"

/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/

/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/


/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION rrcrcr_init_procedure

DESCRIPTION
  This function will initialize the RRC Connection Release procedure
  substate to RRCRCR_INITIAL Substate. This will be called in the RRCTASK 
  modules along with other Init Procedures.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void rrcrcr_init_procedure
(    
sys_modem_as_id_e_type wrrc_as_id
);
/*===========================================================================

FUNCTION rrcrcr_procedure_event_handler

DESCRIPTION
  This is the entry point for RRC Connection Release procedure. All
  events for RCR come to this function from RRC Dispatcher. Depending on the
  UE state of RRC Connection Release Procedure, it calls the corresponding state 
  handler. These state handlers call substate handlers depending on the 
  the current substate of RRC Connection Release procedure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void rrcrcr_procedure_event_handler
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr                      /* Pointer to the RRC command */
);


/*===========================================================================

FUNCTION rrcrcr_process_690non_critical_ext

DESCRIPTION
  Processes 690 non critical extension of RRC Conn Rel OTA.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void rrcrcr_process_690non_critical_ext
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_RRCConnectionRelease_v690NonCriticalExtensions_1 * rcr_msg_ptr
);

/*===========================================================================

FUNCTION rrcrcr_is_proc_active

DESCRIPTION
  Returns rcr sub-state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

boolean rrcrcr_is_proc_active
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrcrcr_update_release_cause

DESCRIPTION
  This function converts the release cause from ASN1 format into RRC-MM 
  interface format.

DEPENDENCIES
  None

RETURN VALUE
  rrc_rel_cause_e_type: 

SIDE EFFECTS
  None
===========================================================================*/
rrc_rel_cause_e_type rrcrcr_update_release_cause
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_ReleaseCause release_cause  
);

/*===========================================================================

FUNCTION rrcrcr_is_ifreq_rdr_in_prgrs

DESCRIPTION
  Indicates whether inter frequency redirection is in progress

DEPENDENCIES
  None

RETURN VALUE
  TRUE if inter-f redirection is in progress
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/

boolean rrcrcr_is_ifreq_rdr_in_prgrs
(
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION rrcrcr_reset_ifreq_rdr_in_prgrs

DESCRIPTION
  This function resets the inter frequency redirection in progress flag

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void rrcrcr_reset_ifreq_rdr_in_prgrs
(
sys_modem_as_id_e_type wrrc_as_id
);

#endif /* RRCRCR_H */


#ifndef TDSRRCUECE_H
#define TDSRRCUECE_H
/*===========================================================================
                  R R C  U E  C A P A B I L I T Y  E N Q U I R Y

DESCRIPTION
  This header file contains function prototypes for UE Capability Enquiry
  procedure

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcuece.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/03/10   zwj     Added code to handle REL4 OTA Message.

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "tdsrrcasn1.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#define TDSRRC_MAX_SYS_CAPABILITY 16


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



/* =======================================================================
**                          Macro Definitions
** ======================================================================= */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION tdsrrcuece_init_procedure

DESCRIPTION
  This function initializes required parameters for UE Capability Enquiry
  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcuece_init_procedure(void);

/*===========================================================================

FUNCTION tdsrrcuece_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the UE Capability Enquiry procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC) containing UE 
    CAPABILITY ENQUIRY message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void tdsrrcuece_procedure_event_handler(tdsrrc_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION tdsrrcuece_copy_capability_update_requirement

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement
(
  tdsrrc_CapabilityUpdateRequirement capability_update_requirement
                                          /* Capability Update requirement */
);


/*===========================================================================

FUNCTION tdsrrcuece_copy_capability_update_requirement_later_than_r3

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement_later_than_r3
(
  tdsrrc_CapabilityUpdateRequirement_r5 capability_update_requirement
                                          /* Capability Update requirement */
);

/*===========================================================================

FUNCTION tdsrrcuece_copy_capability_update_requirement_r7

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement_r7
(
  tdsrrc_CapabilityUpdateRequirement_r7 *capability_update_requirement
                                          /* Capability Update requirement */
);
#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION tdsrrcuece_copy_capability_update_requirement_r8

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement_r8
(
  tdsrrc_CapabilityUpdateRequirement_r8 *capability_update_requirement
                                          /* Capability Update requirement */
);
#endif /*FEATURE_TDSCDMA_REL8*/
/*===========================================================================

FUNCTION tdsrrcuece_copy_capability_update_requirement_r4

DESCRIPTION
  This function copy the Radio Capability Requirement into internal format.
  This stored information will be utilized by any RRC procedure to build 
  UE Capabilities.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcuece_copy_capability_update_requirement_r4
(
  tdsrrc_CapabilityUpdateRequirement_r4 capability_update_requirement
                                          /* Capability Update requirement */
);
#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================

FUNCTION rrcueci_send_eutra_capabilitites_req_to_lte

DESCRIPTION
  This  function sends EUTRA capabilities request to LTE

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcueci_send_eutra_capabilitites_req_to_lte
(
  void
);

/*===========================================================================

FUNCTION rrcueci_save_eutra_capability

DESCRIPTION
  This  function saves the E-UTRA capability info

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcueci_save_eutra_capability
(
  lte_rrc_eutra_capabilities_rsp_s *lte_eutra_capabilities_rsp
);

/*===========================================================================
FUNCTION rrcueci_append_ue_radio_access_capability_tdd

DESCRIPTION
  This function appends the Radio Access Capability to the message.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcueci_append_ue_radio_access_capability_tdd
(
  tdsrrc_MeasurementCapabilityTDD *tdd_ptr,
  sys_band_mask_e_type lte_band_pref
);
#endif

#endif /* TDSRRCUECE_H */


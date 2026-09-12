#ifndef TDSRRCOSSINIT_H
#define TDSRRCOSSINIT_H
/*===========================================================================
                          R R C O S S I N I T

DESCRIPTION
  This header file contains function prototypes for the functions in 
  tdsrrcossinit.c.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcossinit.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

#include "tdsuecomdef.h"

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


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

FUNCTION tdsrrc_oss_init

DESCRIPTION
  This function initializes the OSS ASN.1 subsystem. It also sets the encoding
  flags, decoding flags, and encoding rules.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS if the OSS ASN.1 subsystem was initialized successfully, otherwisw 
  FAILURE. When FAILURE is returned, the UE should go offline because there
  is somethign drastically wrong with the ASN.1 subsystem and RRC will be 
  unable to process signalling messages.

SIDE EFFECTS
  None.

===========================================================================*/

uecomdef_status_e_type tdsrrc_oss_init(void);
void tdsrrcoss_obj_sys_init
(
  void
);

#endif /* TDSRRCOSSINIT_H */

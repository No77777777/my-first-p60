/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  H D R    S R C H   S C H E D U L E R    G R O U P

                    Search Scheduler Group types Header File

GENERAL DESCRIPTION

   This file contains the databases, typedefs, enums and defines used in
   the HDR revB scheuduler group management. See more header comments in 
   hdrsrchschdgrp.c

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

void hdrsrchschdgrp_init
void hdrsrchschdgrp_tca_update
void hdrsrchschdgrp_get_schdgrp_rpt

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2007 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchschdgrp.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
12/26/2007   jyw     Fixed compiling warnings.
09/06/2007   jyw     Miscellaneous updates to fix compiling warnings.
06/01/2006   jyw     Created this file for HDR revB development.

==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdrsrchsect.h"
#include "hdrsrchset.h"
#include "hdrsrchtypes.h"

#include "hdr_variation.h"
#ifdef FEATURE_HDR_REVB
/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

/*==========================================================================

                     MACROS USED IN THE MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

/*==========================================================================

                        EXTERNAL STRUCTURES, VARIABLES

This section contains the declaration of the external variables, structs
which are used by other files of HDRSRCH module

==========================================================================*/
 
/*--------------------------------*
 * HDR SRCH SCHEDULER GROUP MACROS*
 *--------------------------------*/


#define MAX_NUM_PILOTS_PER_SCHDGRP      6  
  /* Scheduler group is defined as*/

#define HDRSCHDGRP_REV0REVA_SCHDTAG     0xFF
  /* Scheduler Tag used in rev0 or revA system */

/*============================================================================

                            FUNCTION DECLARATIONS

============================================================================*/


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_INIT

DESCRIPTION 
  The function should be called in tc_init

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_tc_init( void ); 


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_INIT

DESCRIPTION 
  One time initializer for the scheduler group

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_init( void );


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_RESET

DESCRIPTION 
  This function initiate the scheduler grp table and rebuild it based on the
  current ASET pilots.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_reset( void ); 


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_TCA_UPDATE

DESCRIPTION 
  This function is called at the end of TCA processing to update the 
  scheduler group table with the latest grouping information.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void  hdrsrchschdgrp_drop_from_schdgrp
( 
  hdrsrch_sect_struct_type *sect_ptr
);


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_TCA_UPDATE

DESCRIPTION 
  This function is called at the TCA processing to update the scheduler 
  group table.

DEPENDENCIES
  Before calling this function, ASET should already been updated with the
  new TCA.

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchschdgrp_tca_update( void );


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_GET_SCHDGRP_INFIO();

DESCRIPTION 
  The function returns the information of outstanding scheduler groups.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_get_schdgrp_rpt
(
  hdrsrch_schdgrp_rpt_type * schdgrp_rpt
);


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_COMMIT_TO_DSP();

DESCRIPTION 
  Commit the scheduler group id to the firmware ASP interface.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_commit_to_dsp( void );
#endif /* FEATURE_HDR_REVB */

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


             U I M    S E L E C T I V E    L O G G I N G


GENERAL DESCRIPTION

  This source file contains the selective logging functionality

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/common/src/uim_selective_logging.c#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
11/02/16   me      NVRUIM F3 reduction  
10/07/16   shr     GSTK/ESTK F3 reduction
07/05/16   ar      Removing array for selective logging
06/22/16   ar      Initial revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_selective_logging.h"


/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
#define UIM_MAX_ITEM_IN_SELECT_LOG        16
#define UIM_EONS                          0
#define UIM_TOOLKIT                       1
#define UIM_NVRUIM                        2


/*===========================================================================

                                      GLOBAL VARIABLES

===========================================================================*/

uint8   uim_selective_log_eons                            = 0;
uint8   uim_selective_log_toolkit                         = 0;
uint8   uim_selective_log_nvruim                          = 0;


/*===========================================================================
   FUNCTION:      UIM_SELECTIVE_LOGGING_INIT

   DESCRIPTION:
     This function initialise the selective logging
     functionality by reading NV item 73880

   DEPENDENCIES:
     None

   LIMITATIONS:
     None


   RETURN VALUE:
    Void

   SIDE EFFECTS:
     None

=============================================================================*/
void uim_selective_logging_init
(
  void
)
{
  uint8   uim_selective_logging[UIM_MAX_ITEM_IN_SELECT_LOG] = {0};

  /* Check UIM PSM NV is enabled on not */
  (void)uim_common_efs_read(UIM_COMMON_EFS_MMGSDI_SELECTIVE_LOGGING,
                            UIM_COMMON_EFS_ITEM_FILE_TYPE,
                            UIM_COMMON_EFS_DEVICE,
                            (uint8 *)uim_selective_logging,
                            sizeof(uim_selective_logging));

  /* 0th byte is for EONS logging */
  uim_selective_log_eons = uim_selective_logging[UIM_EONS];
  /* 1st byte is for Toolkit logging */
  uim_selective_log_toolkit = uim_selective_logging[UIM_TOOLKIT];
  /* 2nd byte is for Nvruim logging */
  uim_selective_log_nvruim = uim_selective_logging[UIM_NVRUIM];
} /* uim_selective_logging_init */

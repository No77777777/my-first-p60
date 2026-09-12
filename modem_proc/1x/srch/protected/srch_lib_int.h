#ifndef SRCH_LIB_INT_H
#define SRCH_LIB_INT_H
/*====*====*====*====*====*====*====*========*====*====*====*====*====*====*==*

        S E A R C H   T A S K  --
        S E A R C H   L I B R A R Y   I N T E R N A L   H E A D E R

GENERAL DESCRIPTION
  This file contains library interfaces to the rest of searcher code
  (outside of the library)

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

  Copyright (c) 2004 - 2015 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.


*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/srch/protected/srch_lib_int.h#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
01/09/15   srk     Feature cleanup.
02/04/11   pk      Tier enforcement changes
10/07/09   sst     Added method to compare structure sizes inside and outside
                   of the library
04/01/09   adw     Standardized format.
12/28/04   bt      Implementation, first cut

=============================================================================*/


/*=============================================================================

                        INCLUDE FILES FOR MODULE

=============================================================================*/


/*===========================================================================

                 SEARCHER LIBRARY FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION SRCH_LIB_INT_VER

DESCRIPTION   Returns the library version string.

DEPENDENCIES  None

RETURN VALUE  Pointer to the library version string.

SIDE EFFECTS  None

===========================================================================*/
extern const char* srch_lib_int_ver(void);

/*===========================================================================

FUNCTION SRCH_LIB_INT_CHECK_MDSP_VER

DESCRIPTION   Checks the MDSP Version at runtime and compares this to the
              version the search library was compliled with.  An error
              results if these versions do not match.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
extern void srch_lib_int_check_mdsp_ver(void);

/*=============================================================================

FUNCTION SRCH_LIB_INT_CHECK_STRUCTS

DESCRIPTION    This functions is used to compare the sizes of srch structures
               inside and outside of the srch library.

DEPENDENCIES   This function must match srch_lib_ext_check_structs which is
               inside the library

RETURN VALUE   None

SIDE EFFECTS   None

=============================================================================*/
extern void srch_lib_int_check_structs( void );

#endif /* SRCH_LIB_INT_H */

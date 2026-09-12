#ifndef TIME_GENOFF_CLIENT_V_H
#define TIME_GENOFF_CLIENT_V_H

/**
@file time_genoff_client_v.h
@brief
This module implements the Generic framework to handle time bases.
*/
/*=============================================================================
NOTE: The @brief description and any detailed descriptions above do not appear 
      in the PDF. 

      The ATS_mainpage.dox file contains all file/group descriptions that
      are in the output PDF generated using Doxygen and Latex. To edit or 
      update any of the file/group text in the PDF, edit the 
      ATS_mainpage.dox file or contact Tech Pubs.

      The above description for this file is part of the time_genoff group 
      description in the ATS_mainpage.dox file. 
=============================================================================*/

/*=============================================================================
  Copyright (c) 2009-2015 QUALCOMM Technologies Incorporated.
  All rights reserved.
  Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================

 T I M E   S E R V I C E   G E N E R I C  F R A M E W O R K  S U B S Y S T E M

GENERAL DESCRIPTION
 Implements Generic framework to handle Time bases for Remote PD. 

EXTERNALIZED FUNCTIONS

  time_genoff_opr
   Does time set/get operation on the basis of Inputs

INITIALIZATION AND SEQUENCING REQUIREMENTS
  Should be initialized with following sequence

=============================================================================*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/time/src/time_genoff_client_v.h#1 $

when         who     what, where, why
--------   ---     ------------------------------------------------------------
08/19/15   abh     Changes for properly handling ATS_PRIVATE base operations.
05/01/14   abh     File created.

=============================================================================*/


/*===========================================================================
                           INCLUDE FILES
=============================================================================*/

#include "time_genoff.h"
#include "time_genoff_v.h"



/*===========================================================================
                           MACRO DEFINITION
=============================================================================*/
#define TIME_GENOFF_PRI 0x50						/* Priority for the worker thread */
#define TIME_GENOFF_STACK_SIZE 4096					/* Stack size for the worker thread */


/*===========================================================================
                           ENUMERATIONS
=============================================================================*/


/*===========================================================================
                           STRUCTURES TYPEDEF
=============================================================================*/

/* Structure for list of external registered callbacks for time update notification. */
typedef struct time_genoff_client_cb_struct_s
{
time_genoff_t2_cb_type cb_func;
uint32 cb_data;
struct time_genoff_client_cb_struct_s * next;
}time_genoff_client_cb_struct_type;
typedef time_genoff_client_cb_struct_type * time_genoff_client_cb_struct_ptr;

/* Internal Time generic offset structure */
typedef struct time_genoff_client_int_struct
{
  /* Generic Offset, always stored in ms */
  int64                           generic_offset;

  /* Sum of generic_offset of current base and its bases 
     in milliseconds format */
  uint64                          sum_generic_offset;

  /* Flag to indicate if the absloute value in sum_generic_offset_ts
      is to be added or substracted from the uptime */
  boolean                         sum_generic_offset_ts_is_negative;

  /* sum_generic_offset in timestamp format */
  time_type                        sum_generic_offset_ts;

  /* Pointer to list of structures containing detail of external callbacks registered for update notification. */
  time_genoff_client_cb_struct_ptr       cb_list_ptr;

  /* Counter of the number of registered cb functions */
  uint32                          number_of_cb_func;

} time_genoff_client_struct_type, *time_genoff_client_ptr;



/*===========================================================================
                           FUNCTION DECLARATIONS
=============================================================================*/



/*-----------------------------------------------------------------------------
  Generic offset information
-----------------------------------------------------------------------------*/



/*=============================================================================

                           FUNCTION PROTOTYPE

=============================================================================*/

/*=============================================================================

FUNCTION TIME_GENOFF_GET_CLIENT_POINTER                                 

DESCRIPTION
  Returns the Pointer to different time bases 

DEPENDENCIES
  None

RETURN VALUE
  The pointer to time_genoff_struct_type

SIDE EFFECTS
  None

=============================================================================*/
time_genoff_client_ptr time_genoff_get_client_pointer(time_bases_type base);
/*=============================================================================

FUNCTION TIME_GENOFF_GET_CLIENT_OFFSET

DESCRIPTION
  Returns generic offset value of a given time base

DEPENDENCIES
  None

RETURN VALUE
  int64 value

SIDE EFFECTS
  None

=============================================================================*/
int64 time_genoff_get_client_offset(time_bases_type base);



#endif /* TIME_GENOFF_CLIENT_V_H */

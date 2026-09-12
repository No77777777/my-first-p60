#ifndef DIAG_MICRO_I_H
#define DIAG_MICRO_I_H
/*==========================================================================

              Diag APIs for uImage 

General Description
  
Copyright (c) 2015 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

===========================================================================*/

/*===========================================================================

                              Edit History

 $Header: //components/rel/core.mpss/10.0/services/diag/micro_diagbuffer/src/diag_micro_i.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/18/15   xy      Created
===========================================================================*/

/*===========================================================================
FUNCTION LOG_MASK_ENABLED

DESCRIPTION
  Checks if the log is enabled in the log mask.
  
PARAMETERS
  mask_ptr - pointer to the log mask
  id - log id
  item - equipment id
  
RETURN VALUE
  TRUE -  The log is enabled in the log mask
  FALSE - Otherwise.
  
===========================================================================*/
boolean log_mask_enabled (const byte *mask_ptr, unsigned int id, unsigned int item);
 
#endif /*DIAG_MICRO_I_H*/
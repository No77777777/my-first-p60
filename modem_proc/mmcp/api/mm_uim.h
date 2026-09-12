#ifndef MMUIM_H
#define MMUIM_H
/*===========================================================================

              MM UIM API files........................................

DESCRIPTION
  This file contains data and function declarations necessary for the
  MM UIM interface related  declaration .

Copyright (c) 2001 Qualcomm Technologies, Incorporated and its licensors.  All Rights 
Reserved.  QUALCOMM Proprietary.  Export of this technology or software 
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*=============================================================================*/
/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "mmgsdilib_common.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

typedef void (*nas_mmgsdi_purge_temporary_identity_callback_type) (uint32 ,boolean );
extern  boolean nas_mmgsdi_purge_temporary_identity_req 
( 
  mmgsdi_session_type_enum_type session_type,
  uint32 user_data,
  nas_mmgsdi_purge_temporary_identity_callback_type  purge_temporary_identity_mmgsdi_callback
);
 

#endif
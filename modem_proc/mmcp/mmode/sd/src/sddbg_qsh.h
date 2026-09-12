#ifndef SDDBG_QSH_EXT_H
#define SDDBG_QSH_EXT_H
/*===========================================================================

     S Y S T E M   D E T E R M I N A T I O N
     
     D E B U G   H E A D E R   F I L E


DESCRIPTION
  This header file contains debug macros and definitions necessary to
  interface with sddbg_qsh.c


Copyright (c) 1991 - 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/



/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/sd/src/sddbg_qsh.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/21/17   KC      Initial release.

===========================================================================*/

#include "comdef.h"    /* Definition for basic types and macros */
#include "qsh.h"
#include "sd_qsh_ext.h"

#if defined(FEATURE_QSH_MDUMP)
/*===========================================================================

FUNCTION sddbg_qsh_cb

DESCRIPTION
  Processes the QSH request .
  Currently we handle only QSH_ACTION_MDUMP .
DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

Note
use SD_API_KEEP_UNCOMPRESSED_IN_ELF ,
which keeps the API in uncompressed section in th elf
 this will be used by QSH dump collection ,
since after the crash happened uncompressed functions
compressed functions wont be accesible
===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void  sddbg_qsh_cb
(
  qsh_client_cb_params_s *cb_params_ptr
);

/*===========================================================================

FUNCTION sddbg_qsh_init

DESCRIPTION
  Initilize SD QSH interface.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void  sddbg_qsh_init();

/*===========================================================================
FUNCTION sddbg_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for SD module.

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void sddbg_qsh_mdump_collect();
#endif

#endif /* CM_QSH_EXT_H */


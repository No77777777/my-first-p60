/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               M U X _ N V _ C


GENERAL DESCRIPTION
   This file contains the NV access interface for mux.

EXTERNALIZED FUNCTIONS
   mux_nv_efs_init

  Copyright (c) 2012 - 2017 Qualcomm Technologies, Inc.
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

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/mux/src/muxnv.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/31/17   ppr     Sub specific 1x EFS Item support chgs
03/31/15   srk     Use subscription id in EFS operations.
06/02/14   pap     Adding diag command to refresh NV items.
08/02/13   fg      modified  mux_nv_efs_init
11/05/12   srk     Check to see if the file is present and contains valid
                   information before doing efs_write.
10/30/12   srk     KW warning fix.
10/19/12   srk     Make only one efs write to the mux config file.
07/15/12   srk     Created the file.
===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "customer.h"
#include "modem_1x_defs.h"

/* Mux */
#include "muxnv_i.h"
#include "txctraffic_i.h"

/* Other */
#include "modem_mem.h"
#include "nv.h"
#include "onex_nv_rd_wt.h"
#include "stringl.h"



/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*--------------------------------------------------------------------------*
 *                  Mux EFS defines                                        *
 *--------------------------------------------------------------------------*/
/* Names and paths of the EFS item files */
LOCAL const efs_item_file_type mux_efs_item_file[MUX_EFS_ITEM_FILE_MAX] =
{
  /* MUX_EFS_DEVICE_ONLY_DTX_PARAMS */
  {
    MUX_EFS_DEVICE_ONLY_DTX_PARAMS_FILENAME, 1
  }
};
/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

/*==========================================================================*
 *                          Utility functions                               *
 *==========================================================================*/

/*===========================================================================

FUNCTION MUX_NV_EFS_INIT

DESCRIPTION
      Performs initialization required for NV item files in the EFS.
      Currently this means generating the config file for diag tools.

DEPENDENCIES
      None

RETURN VALUE
      None

SIDE EFFECTS
      None

===========================================================================*/

void mux_nv_efs_init
(
  void
)
{
  /* Initialize required EFS item files for both Subs (SUB1, SUB2).
   * Anyway MCFG will create a separate EFS file path based on passed Sub_id, 
   * when an EFS Item Read/Write is attempted */
   
   /* MCFG_FS_SUBID_0 = 0, 
    *   Context0: Sub id 0 or primary subscription, 
    *   Equivalent to SYS_MODEM_AS_ID_1 */
  onex_nv_efs_init( MUX_EFS_CONF_FILE,
                    (int) MUX_EFS_ITEM_FILE_MAX,
                    mux_efs_item_file,
                    MCFG_FS_SUBID_0 );

  /* MCFG_FS_SUBID_1 = 1,
   *   Context1: Sub id 1 or Sec subscription, 
   *   Equivalent to SYS_MODEM_AS_ID_2  */
  onex_nv_efs_init( MUX_EFS_CONF_FILE,
                    (int) MUX_EFS_ITEM_FILE_MAX,
                    mux_efs_item_file,
                    MCFG_FS_SUBID_1 );

  #ifdef FEATURE_MODEM_1X_DEVICE_ONLY_DTX
  txc_device_only_dtx_efs_init();
  #endif /* FEATURE_MODEM_1X_DEVICE_ONLY_DTX */

} /* mux_nv_efs_init */


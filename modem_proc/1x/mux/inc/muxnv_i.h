#ifndef MUX_NV_H
#define MUX_NV_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               M U X _ N V _ H


GENERAL DESCRIPTION
   This file contains the NV access interface for mux.

EXTERNALIZED FUNCTIONS
   mux_nv_efs_init

  Copyright (c) 2012 - 2013 Qualcomm Technologies, Inc. 
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

$Header: //components/rel/1x.mpss/8.0/mux/inc/muxnv_i.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/06/13   srk     Fix EFS read/write issues caused due to recent
                   reorganization of NV read/write functionality.
07/20/12   srk     Created the file.
===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "comdef.h"
#include "modem_1x_defs.h"


/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*--------------------------------------------------------------------------*
 *                  Mux EFS defines                                        *
 *--------------------------------------------------------------------------*/

/* Name and path of the conf file for 1X Mux */
#define MUX_EFS_CONF_FILE "/nv/item_files/conf/onex_mux.conf"

/* Enum of EFS item files */
typedef enum
{
  MUX_EFS_DEVICE_ONLY_DTX_PARAMS = 0,
  MUX_EFS_ITEM_FILE_MAX
} mux_efs_item_file_name_type;

/* EFS file name containing default thresholds */
#define MUX_EFS_DEVICE_ONLY_DTX_PARAMS_FILENAME \
        "/nv/item_files/modem/1x/device_only_dtx_params"


/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

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

void mux_nv_efs_init( void );
#endif /* MUX_NV_H */

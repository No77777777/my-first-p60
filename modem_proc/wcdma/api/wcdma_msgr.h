#ifndef WCDMA_MSGR_H
#define WCDMA_MSGR_H
/*===========================================================================
                      WCDMA TECH MODULE DEFINITIONS

DESCRIPTION

  This file contains the WCDMA Tech module to support the MSGR

Copyright (c) 2000-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/api/wcdma_msgr.h#3 $    $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/21/15   cc      Added G2W MSGR variant support
04/06/15   cc      Added G2W MSGR support
02/26/10   rm      Added new line at EOF
01/28/10   rm      Added WCDMA RRC tech module definition
===========================================================================*/


#include "msgr.h"


#define MSGR_WCDMA_RRC        MSGR_TECH_MODULE( MSGR_TECH_WCDMA, 0x01 )
#define MSGR_WCDMA_IRAT       MSGR_TECH_MODULE( MSGR_TECH_WCDMA, 0x02 )

#endif /* #ifndef WCDMA_MSGR_H */

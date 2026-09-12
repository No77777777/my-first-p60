#ifndef MCALTDS_DEF_H
#define MCALTDS_DEF_H

/*============================================================================
                        M C A L T D S _ D E F. H
DESCRIPTION
  This module has common TD-SCDMA MCAL (modem core abstraction layer) definitions

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2010 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/tdscdma.mpss/4.3/l1/src/mcaltds_def.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
08/18/11    asm     Added RF MSGR definitions
11/19/10    sq      Initial draft


===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "msgr.h"       /* Message Router */
#include "msgr_umid.h"  /* UMID definitions */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/***************************************************
 * Module Definitions (bits 23:16 of the UMID)     *
 ***************************************************/

/* SW defined TD-SCDMA L1 modules */
#define MSGR_MODULE_SRCH                  0x00
#define MSGR_MODULE_L1DL                  0x01
#define MSGR_MODULE_L1UL                  0x02
#define MSGR_MODULE_L1M                   0x03
#define MSGR_MODULE_MEAS                  0x04
#define MSGR_MODULE_PLT                   0x05
#define MSGR_MODULE_RF                    0x06
#define MSGR_MODULE_L1_EUL                0x07

/* Combining TECH and MOD to form tech_mod */
#define MSGR_TDSCDMA_SRCH                 MSGR_TECH_MODULE( MSGR_TECH_TDSCDMA, MSGR_MODULE_SRCH )
#define MSGR_TDSCDMA_L1DL                 MSGR_TECH_MODULE( MSGR_TECH_TDSCDMA, MSGR_MODULE_L1DL )
#define MSGR_TDSCDMA_L1UL                 MSGR_TECH_MODULE( MSGR_TECH_TDSCDMA, MSGR_MODULE_L1UL )
#define MSGR_TDSCDMA_L1M                  MSGR_TECH_MODULE( MSGR_TECH_TDSCDMA, MSGR_MODULE_L1M  )
#define MSGR_TDSCDMA_MEAS                 MSGR_TECH_MODULE( MSGR_TECH_TDSCDMA, MSGR_MODULE_MEAS )
#define MSGR_TDSCDMA_PLT                  MSGR_TECH_MODULE( MSGR_TECH_TDSCDMA, MSGR_MODULE_PLT  )
#define MSGR_TDSCDMA_RF                   MSGR_TECH_MODULE( MSGR_TECH_TDSCDMA, MSGR_MODULE_RF   )

#endif /* MCALTDS_DEF_H */

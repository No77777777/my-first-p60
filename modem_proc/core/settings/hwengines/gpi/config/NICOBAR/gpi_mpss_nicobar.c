/*
===========================================================================

FILE:         gpi_mpss_6150.c

DESCRIPTION:  This file implements the GPI Config Data.

===========================================================================

                             Edit History


when       who     what, where, why
--------   ---     --------------------------------------------------------
05/30/18   pc      Updated gpi settings based on latest driver
04/05/18   pc      Updated gpi base for 640
10/27/17   pc      Disabled top level QUPs for 670 as there is no usecase
07/13/17   pc      Updated for 670
04/21/17   ah      Added base definitions.
10/13/16   ah      Initial revision for Napali SSC.

===========================================================================
             Copyright (c) 2016-2018 QUALCOMM Technologies, Incorporated.
                    All Rights Reserved
                   QUALCOMM Proprietary
===========================================================================
*/

#include "gpitgtcfgdata.h"
#include "msmhwiobase.h"

#define GSI_0_BASE   (QUPV3_0_QUPV3_ID_1_BASE + 0x00004000)
#define GSI_1_BASE   (QUPV3_1_QUPV3_ID_1_BASE + 0x00004000)

const tgt_gpi_config_type  tgt_gpi_config[] =
{
   { TRUE,  0, GSI_0_BASE, 432, {0} },
   { FALSE, 1, GSI_1_BASE, 433, {0} },
   { FALSE, 2, 0,          0,   {0} },
};


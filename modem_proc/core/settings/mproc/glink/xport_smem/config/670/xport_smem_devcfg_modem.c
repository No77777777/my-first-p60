/*===========================================================================

            GLink SMEM transport XPORT Configuration Structures

=============================================================================

  @file
    xport_smem_config.c

    Contains structures to be used in Glink SMEM trasnport configuration.

  Copyright (c) 2017 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies, Inc. Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/settings/mproc/glink/xport_smem/config/670/xport_smem_devcfg_modem.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/25/17   vr       Initial version
===========================================================================*/

/*===========================================================================
                        INCLUDE FILES
===========================================================================*/
#include "DALStdDef.h"
#include "xport_smem_settings.h"

/*===========================================================================
                    CONSTANT / MACRO DECLARATIONS
===========================================================================*/

/*===========================================================================
                      LOCAL DATA DEFINATION
===========================================================================*/

/*===========================================================================
                      GLOBAL DATA DEFINATION
===========================================================================*/

/** Processor information */
const xport_smem_devcfg_proc_type xport_smem_devcfg_proc =
{
  "mpss",                  /* Local host name */
  SMEM_MODEM               /* Local host ID   */
};

/** Transports supported */
const xport_smem_devcfg_remote_type xport_smem_devcfg_remote_procs[] =
{
  /* Modem->Apss */
  {
    "apss",                /* Remote host name        */
    SMEM_APPS,             /* Remote host ID          */
    16*1024,               /* Tx FIFO size            */
    4*1024,                /* Tx MTU size             */
    DALIPCINT_PROC_ACPU,  /* Outgoing interrupt      */
    DALIPCINT_GP_0,
    336,                   /* Incoming interrupt      */
    20000                  /* Maximum data rate (tokens/sec) */
  },
  /* Modem->Lpass */
  {
    "lpass",               /* Remote host name        */
    SMEM_ADSP,             /* Remote host ID          */
    16*1024,               /* Tx FIFO size            */
    4*1024,                /* Tx MTU size             */
    DALIPCINT_PROC_LPASS, /* Outgoing interrupt      */
    DALIPCINT_GP_0,
    356,                   /* Incoming interrupt      */
    20000                  /* Maximum data rate (tokens/sec) */
  },
  
  /* Modem-> CDSP */
  {
    "cdsp",               /* Remote host name        */
    SMEM_CDSP,             /* Remote host ID          */
    16*1024,               /* Tx FIFO size            */
    4*1024,                /* Tx MTU size             */
    DALIPCINT_PROC_CDSP,  /* Outgoing interrupt      */
    DALIPCINT_GP_0,
    365,                   /* Incoming interrupt      */
    20000                  /* Maximum data rate (tokens/sec) */
  },  
  
   /* NULL */ 
  {
    NULL,                  /* Remote host name        */
    SMEM_INVALID_HOST,     /* Remote host ID          */
    0,                     /* Tx FIFO size            */
    0,                     /* Tx MTU size             */
    DALIPCINT_PROC_NULL,  /* Outgoing interrupt      */
    DALIPCINT_GP_0,
    0,                    /* Incoming interrupt      */
    0                     /* Maximum data rate (tokens/sec) */
  }
};

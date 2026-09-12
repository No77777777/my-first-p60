/*===========================================================================

            GLink SMEM transport 8997 MPSS Configuration Structures

=============================================================================

  @file
    xport_smem_config.c

    Contains structures to be used in Glink SMEM trasnport configuration.

  Copyright (c) 2015 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies, Inc. Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/settings/mproc/glink/xport_smem/config/8997/xport_smem_devcfg_modem.c#1 $

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
                        DATA DECLARATIONS
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
  /* Modem->Spss */
  {
    "spss",               /* Remote host name        */
    SMEM_SPSS,            /* Remote host ID          */
    2*1024,               /* Tx FIFO size            */
    2*1024,               /* Tx MTU size             */
    {DALIPCINT_PROC_SPSS, /* Outgoing interrupt      */
    DALIPCINT_GP_0},
    453,                  /* Incoming interrupt      */
    0                  /* Maximum data rate (tokens/sec) */
  },
  /* Modem->Apss */
  {
    "apss",                /* Remote host name        */
    SMEM_APPS,             /* Remote host ID          */
    16*1024,               /* Tx FIFO size            */
    4*1024,                /* Tx MTU size             */
    DALIPCINT_PROC_ACPU,  /* Outgoing interrupt      */
    DALIPCINT_GP_3,
    339,                   /* Incoming interrupt      */
    20000                  /* Maximum data rate (tokens/sec) */
  },

  /* Modem->Lpass */
  {
    "lpass",               /* Remote host name        */
    SMEM_ADSP,             /* Remote host ID          */
    16*1024,               /* Tx FIFO size            */
    4*1024,                /* Tx MTU size             */
    DALIPCINT_PROC_LPASS, /* Outgoing interrupt      */
    DALIPCINT_GP_3,
    343,                   /* Incoming interrupt      */
    20000                  /* Maximum data rate (tokens/sec) */
  },
  /* Modem->Slpi */
  {
    "dsps",               /* Remote host name        */
    SMEM_SSC,             /* Remote host ID          */
    16*1024,              /* Tx FIFO size            */
    4*1024,               /* Tx MTU size             */
    {DALIPCINT_PROC_SPS,  /* Outgoing interrupt      */
    DALIPCINT_GP_3},
    359,                  /* Incoming interrupt      */
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

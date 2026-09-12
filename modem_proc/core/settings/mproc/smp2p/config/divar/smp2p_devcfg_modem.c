/*===========================================================================

                     SMP2P DEVICE CONFIGURATION

        This file provides the device configuration for SMP2P

  ---------------------------------------------------------------------------
  Copyright (c) 2021 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  ---------------------------------------------------------------------------
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/

/*===========================================================================
                          INCLUDE FILES
===========================================================================*/

#include "DDIIPCInt.h"
#include "smem_type.h"
#include "smp2p_settings.h"

/*===========================================================================
                    CONSTANT / MACRO DECLARATIONS
===========================================================================*/

/*===========================================================================
                      GLOBAL DATA DEFINATION
===========================================================================*/
/** Local processor info */
const smp2p_proc_info_type smp2p_devcfg_proc_info = 
{ 
  /** This host name */
  "modem",
  
  /** This host SMEM ID */
  SMEM_MODEM,
  
  /** Feature flags supported */
  SMP2P_FFLAGS_SSR_HANDSHAKE,
  
  /** Max number of entries */
  16
};

/** Interrupt info for connected subsystems */
const smp2p_interrupt_info_type smp2p_devcfg_interrupt_info[] = 
{
  /** APPS */
  {
    /* destination */
    SMEM_APPS,
    
    /* destination host type */
    DALIPCINT_PROC_ACPU,
    
    /* outgoing interrupt register */
    DALIPCINT_GP_2,
    
    /* interrupt bits in irq_out register */
    0,
    
    /* incoming interrupt register */
    338,
  },
  
  /** ADSP */
  {
    /* destination */
    SMEM_ADSP,
    
    /* destination host type */
    DALIPCINT_PROC_ADSP,
    
    /* outgoing interrupt register */
    DALIPCINT_GP_2,
    
    /* interrupt bits in irq_out register */
    0,
    
    /* incoming interrupt register */
    342,
  },

#if 0 
  /** CDSP */
  {
    /* destination */
    SMEM_CDSP,
    
    /* destination host type */
    DALIPCINT_PROC_CDSP,
    
    /* outgoing interrupt register */
    DALIPCINT_GP_2,
    
    /* interrupt bits in irq_out register */
    0,
    
    /* incoming interrupt register */
    367,
  },
 #endif
  /** END */
  {
    /* destination */
    SMEM_INVALID_HOST,
    
    /* destination host type */
    DALIPCINT_PROC_NULL,
    
    /* outgoing interrupt register */
    DALIPCINT_INT_32BITS,
    
    /* interrupt bits in irq_out register */
    0,
    
    /* incoming interrupt register */
    0,
  },
};

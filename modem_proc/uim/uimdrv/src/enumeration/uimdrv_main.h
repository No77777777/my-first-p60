#ifndef UIMDRV_MAIN_H
#define UIMDRV_MAIN_H
/*============================================================================
  FILE:         uimdrv_main.h

  OVERVIEW:     This file holds the includes needed for HW enumeration along with
                externs of some uimdrv globals.

  DEPENDENCIES: N/A

                Copyright (c) 2012 - 2016 QUALCOMM Technologies, Inc(QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/enumeration/uimdrv_main.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when        who        what, where, why
------      ----       -----------------------------------------------------------
04/29/16    sam        Reorganise the hotswap mpm wakeup cabale registration code
03/28/16    sam        Register UIM1 hotswap interrupt eventhough it is disabled
10/07/15    hyo        Compiler error fix 
08/11/15    hyo        usb uicc removal
07/07/15    hyo        Moving to QTF Dal implementation 
06/08/15    ll         Removing BOLT and THOR feature flag
05/04/15    sam        Critical section for offtarget 
                       dal_interrupt_controller_register_isr
04/28/15    ll         9x55 Bring-up changes
03/12/15    ks         SIM tray by tieing the card detect gpios
02/03/15    xj         Add USB UICC enabled on slot interface
12/02/14    ks         Feature to support GPIO based hotswap
08/20/14    nmb        9x45 Bring up changes
07/04/14    ssr        Fix for APDU debug Mask
03/27/14    lxu        USB UICC modem implement
01/29/14    sam        Usage of newer style diag macros
09/30/13    akv        Removal of if-else cases and unused code in hw enumeration
09/16/13    yk         Removed the code for old targets and for old features
08/06/13    rm/ak      Bolt RUMI code
08/1/13     sam        APDU logging mask variable moved to uim_instance_global_type
07/25/13    sam        APDU logging mask selection based on run time
                       configuration
07/12/13    nmb        Global Reorganization
06/24/13    js         TSTS changes
05/31/13    js         Fixed compilation issue
02/21/13    ak         UIM1 support on 9x25 v2
02/11/13    akv        UIM API for querying the available/enabled UIM slots
12/05/12    js         UIM Parallel Processing changes
11/01/12    akv        Core cpu vdd voting to avoid Q6 going into power collapse
10/26/12    akv/ssr    HW Enumeration changes
============================================================================*/
#include "uimdrv_enumeration.h"


/* Standard inclusions from uimdimedrv.h */

#include "uim_variation.h"
#include "comdef.h"
#include "err.h"                   /* Error handling utilities             */
#include "uimdrv_msg.h"                   /* Message utilities                    */
#include "uimdrv.h"                /* UIM driver header file               */
#include "uimi.h"                  /* UIM Internal header file             */
#if defined( FEATURE_UIM_T_1_SUPPORT )
#include "crc.h"
#include "uimt1.h"                 /* T=1 protocol support                 */
#endif /* FEATURE_UIM_T_1_SUPPORT */
#include "HALhwio.h"                    /* Target definitions */

#include "uimutil.h"

#include "DDITlmm.h"
#include "DDIClock.h"

#include "DDIInterruptController.h"
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"

  /* Inclusion for MCPM APIs */
#include "mcpm_api.h"

#include "pm.h"
#include "npa.h"

/* Standard inclusions from uimdimedrv.h */

/*UIM instance whose IRQ is mapped to UIM card event IRQ in MPM wakeup table */
#define UIM_IRQ_MAPPED_IN_MPM_TABLE_INSTANCE      UIM_INSTANCE_1

extern uim_interface_instances              uim_hw_if;
extern uimHWConfig                          m_HWConfig;

extern uimdrv_hw_nv_config_info_type uimdrv_hw_nv_config_info;
extern uim_hw_slot_info_type                 uim_hw_slot_info;

extern const uim_hw_param_type uim_hw_param[];

boolean initialize_hw_classes(uim_instance_global_type *uim_ptr);
uim_hw_slot_info_type uimdrv_hw_nv_configuration(void);
boolean uimIsDualSlotEnabled(void);

/* Same ISR is register for GPIO based hotswap irq */
extern void uimUimctrlIsr (uint32 *context );

uint8 uim_get_chip_index_from_table (DalChipInfoFamilyType  chipFamily);

#endif /* UIMDRV_MAIN_H */

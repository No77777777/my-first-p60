#ifndef DRIVERS_COMMON_H
#define DRIVERS_COMMON_H

/*===========================================================================

          1 X   D R I V E R S   C O M M O N   M O D U L E

 DESCRIPTION
  This header file contains definitions for the 1x drivers common module that
  are used by the decoder, encoder, and deinterleaver units.
  This header file is internal to the drivers unit.

  Copyright (c) 2009 - 2013 Qualcomm Technologies, Inc. 
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

===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/1x.mpss/8.0/drivers/src/drivers_common.h#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/20/11   srk     Feature clean up: Mainlined FEATURE_MODEM_1X_SUPPORTS_Q6
03/30/10   jtm     Replaced T_QDSP6 with FEATURE_MODEM_1X_SUPPORTS_Q6.
                   Added include for modem_1x_defs.h
06/11/09   jtm     Replaced FEATURE_1X_Q6_CRIT_SECT with T_QDSP6.
04/01/09   jtm     Added customer.h
11/19/08   jtm     Initial revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "comdef.h"
#include "customer.h"

//1x
#include "modem_1x_defs.h"

//other
#include "rex.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
                        Critical Section macros
--------------------------------------------------------------------------*/

/* The following defines create sections where other code is not allowed to
   run. The appropriate macro must be used depending on the type of critical
   section required. The following table provides a guide for selecting the
   correct macro:

   Q1. Will the critical section protect a shared resource?
       (Shared global data or shared code that more than one task can access
        at the same time.)

   Q2. Is the code time critical in nature?

   Q3. Can the code be called from interrupt context?

  /-----------------------------------------------------------------------\
  |   Question    |                       Resolution                      |
  |  -----------  |                      ------------                     |
  | 1    2    3   |                                                       |
  |---------------|-------------------------------------------------------|
  | No   No   -   | No critical section required.                         |
  |-----------------------------------------------------------------------|
  | No   Yes  -   | Use DRV_ENTER_ATOMIC_SECT()                           |
  |-----------------------------------------------------------------------|
  | Yes  No   No  | Use DRV_ENTER_ISR_SECT()                              |
  |-----------------------------------------------------------------------|
  | Yes  No   Yes | Use DRV_ENTER_CRIT_SECT()                             |
  |-----------------------------------------------------------------------|
  | Yes  Yes  No  | Use DRV_ENTER_ATOMIC_CRIT_SECT()                      |
  |-----------------------------------------------------------------------|
  | Yes  Yes  Yes | Use DRV_ENTER_ATOMIC_ISR_SECT()                       |
  \-----------------------------------------------------------------------/

 *Note* - For sections that are both time critical and a shared resource
          use the appropriate critical section macro with the time critical
          macro nested inside.
*/
#define DRV_ENTER_ATOMIC_SECTION()    REX_DISABLE_PREMPTION()
#define DRV_LEAVE_ATOMIC_SECTION()    REX_ENABLE_PREMPTION()

#define DRV_ENTER_ISR_SECTION(crit_sect)       REX_ISR_LOCK( &crit_sect )
#define DRV_LEAVE_ISR_SECTION(crit_sect)       REX_ISR_UNLOCK( &crit_sect )

#define DRV_ENTER_CRITICAL_SECTION(crit_sect)  rex_enter_crit_sect( &crit_sect )
#define DRV_LEAVE_CRITICAL_SECTION(crit_sect)  rex_leave_crit_sect( &crit_sect )

#define DRV_ENTER_ATOMIC_ISR_SECTION(crit_sect)       \
    REX_ISR_LOCK( &crit_sect );                       \
    REX_DISABLE_PREMPTION()

#define DRV_LEAVE_ATOMIC_ISR_SECTION(crit_sect)       \
    REX_ENABLE_PREMPTION();                           \
    REX_ISR_UNLOCK( &crit_sect )

#define DRV_ENTER_ATOMIC_CRIT_SECT(crit_sect)         \
    rex_enter_crit_sect( &crit_sect );                \
    REX_DISABLE_PREMPTION()

#define DRV_LEAVE_ATOMIC_CRIT_SECT(crit_sect)         \
    REX_ENABLE_PREMPTION();                           \
    rex_leave_crit_sect( &crit_sect )

// declare critical sections that are common to drivers
/*FUTURE: NOT CURRENTLY USED
extern rex_crit_sect_type enc_crit_sect;
extern rex_crit_sect_type dec_crit_sect;
extern rex_crit_sect_type deint_crit_sect;
*/

#endif /* DRIVERS_COMMON_H */

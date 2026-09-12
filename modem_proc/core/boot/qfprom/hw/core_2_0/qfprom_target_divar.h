#ifndef QFPROM_TARGET_H
#define QFPROM_TARGET_H

/*===========================================================================

                        QFPROM  Driver Header  File

DESCRIPTION
 Contains target specific defintions and APIs to be used to read and write
 qfprom values for sec ctrl 3.0 .

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright 2018 by Qualcomm Technologies Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/28/21   rakhi     Initial version for Divar

============================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "qfprom.h"
#include "msmhwio.h"
#include "qfprom_target_common.h"
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* Blow timer clock frequency in Mhz for 7nm LPe technology */
#define QFPROM_BLOW_TIMER_CLK_FREQ_MHZ 4.8

/* Amount of time required to hold charge to blow fuse in micro-seconds */
#define QFPROM_FUSE_BLOW_TIME_IN_US    5

/*
* Configure QFPROM_ACCEL[QFPROM_GATELAST] SW field to a default value of 0x1 for 7FF SOC technology.
* Configure QFPROM_ACCEL[QFPROM_TRIPPT_SEL] SW field to a value of 0x5 (101) to blow at a high resistance setting
* Configure QFPROM_ACCEL[QFPROM_ACCEL] SW field to a value of 0x10.
*/
#define QFPROM_GATELAST_VAL            0x1
#define QFPROM_TRIPPT_SEL_VAL          0x5
#define QFPROM_ACCEL_VAL               0x10

#define QFPROM_ACCEL_VALUE             HWIO_FVAL(QFPROM_ACCEL, QFPROM_GATELAST, QFPROM_GATELAST_VAL) | \
                                       HWIO_FVAL(QFPROM_ACCEL, QFPROM_TRIPPT_SEL, QFPROM_TRIPPT_SEL_VAL) | \
                                       HWIO_FVAL(QFPROM_ACCEL, QFPROM_ACCEL, QFPROM_ACCEL_VAL)

/* QFPROM_ACCEL register default values */

#define QFPROM_GATELAST_RESET_VAL            0x1
#define QFPROM_TRIPPT_SEL_RESET_VAL          0x0
#define QFPROM_ACCEL_RESET_VAL               0x0

#define QFPROM_ACCEL_RESET_VALUE       HWIO_FVAL(QFPROM_ACCEL, QFPROM_GATELAST, QFPROM_GATELAST_RESET_VAL) | \
                                       HWIO_FVAL(QFPROM_ACCEL, QFPROM_TRIPPT_SEL, QFPROM_TRIPPT_SEL_RESET_VAL) | \
                                       HWIO_FVAL(QFPROM_ACCEL, QFPROM_ACCEL, QFPROM_ACCEL_RESET_VAL)

/*
** Raw Address Start and End configuration to use for read/write operations.
** The start address is designed to be at the first supported region LSB address in the 
** memory map. The end address is designed to be the last supported region MSB address
** in the memory map.
*/
#define QFPROM_RAW_LOWEST_ADDRESS_OFFSET           (0x00000120 )
#define QFPROM_RAW_HIGHEST_ADDRESS_OFFSET          (0x0000085C )

#define QFPROM_RAW_LOWEST_ADDRESS          (SECURITY_CONTROL_BASE + QFPROM_RAW_LOWEST_ADDRESS_OFFSET )
#define QFPROM_RAW_HIGHEST_ADDRESS         (SECURITY_CONTROL_BASE + QFPROM_RAW_HIGHEST_ADDRESS_OFFSET )

/*
** Corrected Address Start and End configuration to use for read/write operations.
** The start address is designed to be at the first supported region LSB address in the 
** memory map. The end address is designed to be the last supported region MSB address
** in the memory map.
*/
#define QFPROM_CORRECTED_LOWEST_ADDRESS   (QFPROM_RAW_LOWEST_ADDRESS + QFPROM_RAW_TO_CORRECTED_ADDRESS_OFFSET )
#define QFPROM_CORRECTED_HIGHEST_ADDRESS  (QFPROM_RAW_HIGHEST_ADDRESS + QFPROM_RAW_TO_CORRECTED_ADDRESS_OFFSET )

/* Macro for Read/write permission of corrected address */
#define QFPROM_READ_PERM_CORRECTED_ADDR		HWIO_QFPROM_CORR_RD_PERM_LSB_ADDR
#define QFPROM_WRITE_PERM_CORRECTED_ADDR	HWIO_QFPROM_CORR_WR_PERM_LSB_ADDR

#endif /* QFPROM_TARGET_H */


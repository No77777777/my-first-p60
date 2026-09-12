#ifndef QFPROM_TARGET_H
#define QFPROM_TARGET_H

/*===========================================================================

                        QFPROM  Driver Header  File

DESCRIPTION
 Contains target specific defintions and APIs to be used to read and write
 qfprom values for sec ctrl 3.0 (used by 9x55).

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright  2014 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/boot/qfprom/hw/core_2_0/qfprom_target.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/20/15   jz      Updated for 9x55
10/31/14   jz      Updated for 9x45 support
10/05/12   dh      Fix Sec block frequency value
06/11/12   dh      Initial revision for 8974

============================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* Blow timer clock frequency in Mhz for 20nm SOC technology */
#define QFPROM_BLOW_TIMER_CLK_FREQ_MHZ 5

/* Amount of time required to hold charge to blow fuse in micro-seconds */
#define QFPROM_FUSE_BLOW_TIME_IN_US    5

/*
* Configure QFPROM_ACCEL[QFPROM_TRIPPT_SEL] SW field to a value of 0x4 (011) to blow at a high resistance setting of 7 Kohms.
* Configure QFPROM_ACCEL[QFPROM_ACCEL] SW field to a value of 0x0.
* Leave the value of QFPROM_ACCEL[QFPROM_GATELAST] to a default value of 0x1 for 20SOC technology.
*/
#define QFPROM_ACCEL_VALUE             0xB00

/* QFPROM_ACCEL register default value */
#define QFPROM_ACCEL_RESET_VALUE       0xB00

/*
** Raw Address Start and End configuration to use for read/write operations.
** The start address is designed to be at the first supported region LSB address in the 
** memory map. The end address is designed to be the last supported region MSB address
** in the memory map.
*/
#define QFPROM_RAW_LOWEST_ADDRESS_OFFSET        (0x00000120 )
#define QFPROM_RAW_HIGHEST_ADDRESS_OFFSET       (0x0000044C )

#define QFPROM_READ_PERM_CORRECTED_ADDR         HWIO_QFPROM_CORR_RD_WR_PERM_LSB_ADDR
#define QFPROM_WRITE_PERM_CORRECTED_ADDR        HWIO_QFPROM_CORR_RD_WR_PERM_MSB_ADDR

#endif /* QFPROM_TARGET_H */


#ifndef FLASH_SPI_WRAPPER_H
#define FLASH_SPI_WRAPPER_H

/*=======================================================================
 * FILE:        flash_spi_wrapper.h
 *
 * SERVICES:    Header file for spi wrapper functions
 *
 * DESCRIPTION: Header file shared across all NAND drivers
 *
 * Copyright (c) 2010-2012 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * QUALCOMM Confidential and Proprietary
 *=======================================================================*/

/*===================================================================
 *
 *                       EDIT HISTORY FOR FILE
 *
 *   This section contains comments describing changes made to the
 *   module. Notice that changes are listed in reverse chronological
 *   order.
 *
 *  $Header: //components/rel/core.mpss/10.0/storage/flash/src/dal/flash_spi_wrapper.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $
 *
 *
 * when         who     what, where, why
 * ----------   ---     ----------------------------------------------
 * 10/08/12     eo      Fix SPI NOR compile warnings 
 * 08/21/12     sb      Remove DAL SPI dependency
 * 07/21/11     eo      Virtual addr to Physical addr changes
 * 09/02/10     sv      SPI NOR tools support
 * 04/06/08     sv      Initial Revision
 *==================================================================*/

/*===================================================================
 *
 *                     Include Files
 *
 ====================================================================*/
#include "DALStdErr.h"
#include "DALDeviceId.h"
#include "DALSys.h"

/*-----------------------------------------------------------------------
 *           Definitions and Constants
 *----------------------------------------------------------------------*/

/***********************************************************************
 *           Generic Defines
 **********************************************************************/

/* Init/Configure the SPI wrapper APIs */
void flash_spi_configure(void *spi_configs);

/* Attach to SPI device */
DalDeviceHandle *flash_spi_dev_init(void);

/* Open the SPI device handle */
void flash_spi_dev_open(void);

/* Close the SPI device handle */
void flash_spi_dev_close(void);

#endif  /* FLASH_SPI_WRAPPER_H */

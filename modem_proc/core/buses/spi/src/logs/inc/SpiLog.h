#ifndef _SPI_LOG_H
#define _SPI_LOG_H

/*
===========================================================================

FILE:   SpiLog.h

DESCRIPTION:
    This file contains functions data structure declarations for SPI logging
    driver
 
==========================================================================

        Edit History

$Header: //components/rel/core.mpss/10.0/buses/spi/src/logs/inc/SpiLog.h#1 $

When     Who    What, where, why
-------- ---    -----------------------------------------------------------
09/11/13 sg     Updated common logging mechanism
07/14/12 ag     Include headers common to both EA and DAL environments.
11/02/11 sg     Created

===========================================================================
        Copyright c 2011 - 2013 Qualcomm Technologies Incorporated.
            All Rights Reserved.
            Qualcomm Proprietary/GTDR

===========================================================================
*/
#include "SpiDeviceTypes.h"

#define SPI_LOG_LEVEL_MAX 			SPI_LOG_LEVEL_VERBOSE
#define SPI_LOG_LEVEL_VERBOSE 		5
#define SPI_LOG_LEVEL_INFO 			4
#define SPI_LOG_LEVEL_WARNING 		3
#define SPI_LOG_LEVEL_ERROR 		2
#define SPI_LOG_LEVEL_FATAL_ERROR 	1
#define SPI_LOG_LEVEL_NONE 			0
#define SPIDEVICE_LOG(severity, format_str, args...) 

int32 SpiLog_Init(void);
void SpiLog_SetLogLevel(uint32 uLogLevel);
#endif /* _SPI_LOG_H */


#ifndef __VVDRV_DEO_H__
#define __VVDRV_DEO_H__
/**---------------------------------------------------------------------------
 * @file  VVDRV_deo.h
 *
 * @brief includes the DEO project
 *
 * Copyright (c) 2010-2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/** @mainpage Driver Enhanced Operations
 *
 *  Welcome to DEO project! @n @n
 *  DEO offers a set of tools that could easily integrated to any SW project. @n @n
 *  The DEO project currently includes 7 utilities: @n @n
 *  1. <b>Logging and Debug</b>  - declares the DEO debug defines (deo_debug.h) @n @n
 *  2. <b>Memory Pool</b> - declares the DEO memory pool service (deo_mempool.h) @n @n
 *  3. <b>PCIe Bridge</b> - declares the HAL API for the PCIe RUMI Bridge (deo_rpb.h) @n @n
 *  4. <b>Lists Macros</b> - declares the DEO lists macros (deo_lists.h) @n @n
 *  5. <b>Interrupt Framework</b> - declares a framework for interrupt driven environment (deo_interrupt.h) @n @n
 *  6. <b>Interrupt controller</b> - declares an API for a generic interrupt controller (deo_interrupt_controller.h) @n @n
 *  7. <b>Time</b> - declares the DEO time services (deo_time.h) @n @n 
 * @n
 *  For Initialization of the project please see (deo_init.h) @n @n
 * @n
 *  For more information please contact: Yuval Corey Hershko, Eliad Tsairi, Dan Vardi or Eugene Volfson @n
 */

/************************************************************************/
/*                          INCLUDES                                    */
/************************************************************************/

#include "VVDRV_deo_debug.h"
#include "VVDRV_deo_mempool.h"
#include "VVDRV_deo_lists.h"
#include "VVDRV_deo_interrupt.h"
#include "VVDRV_deo_time.h" 
#include "VVDRV_deo_init.h"

#endif /* __VVDRV_DEO_H__ */

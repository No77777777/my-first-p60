#ifndef __ABT_CFG_H
#define __ABT_CFG_H
/*============================================================================

FILE:      ABT_cfg.h

DESCRIPTION: Definitions for DEVCFG platform data for Bus Timeout Driver.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/interface/ABT_cfg.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2016/05/20  bc     Created

                Copyright (c) 2017 QUALCOMM Technologies, Incorporated.
                             All Rights Reserved.
                          QUALCOMM Proprietary/GTDR
============================================================================*/
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

/*============================================================================
                          MACRO DEFINITION
============================================================================*/
#ifndef TRUE
  #define TRUE true
#endif
#ifndef FALSE
  #define FALSE false
#endif


/*============================================================================
                          TYPE DEFINITION
============================================================================*/

/**
 * ABT configuration Data type
 */
typedef struct
{
    char*    name;                     /**< ABT Slave name */
    uint8_t  slave_id;                 /**< ABT Slave internal id */
    void*    base_addr;                /**< ABT Slave base address */
    char**   clk_name_array;           /**< ABT Slave clk name array */
    uint8_t* clk_disable_array;        /**< ABT Slave clk disable array */
    uint32_t num_of_clk;               /**< Number of clocks to be voted */    
    uint8_t  slave_enable;             /**< Slave enable */
    uint8_t  intr_enable;              /**< Slave Interrupt enable */
    uint8_t  timeout_val;              /**< Slave timeout value */    
    uint8_t  is_noc;                   /**< Flag indicating that this is a NOC, not ABT */
    uint8_t  wakeup_id;                /**< ABT Slave wakeup bit position */
}ABT_slave_info_type;

/**
 * ABT Platform HWIO type
 */
typedef struct
{
    char*    name;                /**< Name of region */
    uint8_t* base_addr;           /**< Base  address */    
}ABT_platform_hwio_info_type;

/**
 * ABT Platform Data type
 */
typedef struct
{
    char*    name;                    /**< Platform name */
    void**   intr_map_addr;           /**< ABT Interrupt map reg address */
    void**   intr_status_addr;        /**< ABT Interrupt pending status 
                                         register address*/
    void*    globa_en_addr;           /**< ABT Slave Global EN reg address */
    uint32_t intr_vector;             /**< Summary Interrupt vector */
    uint32_t intr_priority;           /**< Interrupt priority */
    uint32_t num_status_reg;          /**< Number of ABT Interrupt enable/status registers */
    ABT_platform_hwio_info_type* hwio_map_info;          /**< HWIO map info */    
    uint32_t num_hwio_regions;        /**< Number of HWIO region mappings */    
    void**   wakeup_addr;             /**< ABT Interrupt pending status 
                                         register address*/
    uint32_t num_wakeup_reg;          /**< Number of ABT Interrupt enable/status registers */
}ABT_platform_info_type;


/**
 * ABT Device Property Data type
 */
typedef struct
{
    uint8_t len;                        /**< Length of property data array */
    ABT_slave_info_type* slave_info;  /**< Pointer to cfgdata array*/
    ABT_platform_info_type *platform_info; /**< Pointer to platform info */
}ABT_propdata_type;

#endif /* __ABT_CFG_H */

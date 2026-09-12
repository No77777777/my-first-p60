#ifndef __NOC_ERROR_CFG_H__
#define __NOC_ERROR_CFG_H__
/*============================================================================

FILE:      NOC_error_cfg.h

DESCRIPTION: APIs and data structure declarations for NOC error DEVCFG properties

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/interface/NOC_error_cfg.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2017/03/06  bc     Created

                Copyright (c) 2017 QUALCOMM Technologies, Incorporated.
                             All Rights Reserved.
                          QUALCOMM Proprietary/GTDR
============================================================================*/

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

/*============================================================================
                          DEFINEs/MACROs
============================================================================*/
#ifndef TRUE
  #define TRUE true
#endif
#ifndef FALSE
  #define FALSE false
#endif

#define BIT_FLAG(x) ((uint32_t)(1 << x))

/** ERRLOG flags for log filter */
#define NOC_ERRLOG0   BIT_FLAG(0)
#define NOC_ERRLOG1   BIT_FLAG(1)
#define NOC_ERRLOG2   BIT_FLAG(2)
#define NOC_ERRLOG3   BIT_FLAG(3)
#define NOC_ERRLOG4   BIT_FLAG(4)
#define NOC_ERRLOG5   BIT_FLAG(5)
#define NOC_ERRLOG6   BIT_FLAG(6)

 
/*============================================================================
                          TYPE DEFINITION
============================================================================*/
/**
 * Internal NOC error Info data type
 */
typedef struct
{
    char* name;               /**< NOC name */
    void* base_addr;          /**< NOC base address */
    uint32_t errlog_filter;     /**< NOC ErrLog <n>-bitfield to filter ERRLog<n> */    
    void* sb_base_addr;       /**< NOC sideband base address */
    uint8_t intr_enable;        /**< NOC interrupt enable */
    uint8_t error_fatal;        /**< NOC interrupt error fatal */ 
    uint32_t sb_enable_mask;    /**< NOC interrupt enable mask */
    uint32_t sb_flagout_mask;   /**< NOC flagoutset mask */
}NOCERR_info_type;

/**
 * NOC Error Internal Property Data type
 */
typedef struct
{
    uint8_t len;                        /**< Length of cfgdata array */
    const NOCERR_info_type* NOCInfo;        /**< Pointer to internal cfgdata array*/
}NOCERR_propdata_type;

#endif /* __NOC_ERROR_CFG_H__ */

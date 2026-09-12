#ifndef __ABT_INTERNAL_H__
#define __ABT_INTERNAL_H__
/*============================================================================

FILE:      ABT_internal.h

DESCRIPTION: Common internal function and data structure declarations 
             for AHB timeout driver Interface

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

//#CHANGE - Update when put in the depot
$Header: //components/rel/core.mpss/10.0/systemdrivers/icb/src/common/ABT_internal.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2016/05/20  bc     Removing duplicate headers, and reducing public API
2013/08/30  pm     Renamed ABT_Register_Interrupt() and moved it to ABT_platform.h
2013/04/16  pm     Added ABT_Register_Interrupt()
2012/05/31  av     Created

                Copyright (c) 2013 QUALCOMM Technologies, Incorporated.
                             All Rights Reserved.
                          QUALCOMM Proprietary/GTDR
============================================================================*/
#include "ABT_cfg.h"
#include "ABTimeout.h"
#include "stdint.h"

/*============================================================================
                          DEFINEs/MACROs
============================================================================*/

/* The ERR team and MSG team are having integration issues on MPSS.5.0.  
 * Remove this and use ERR() again from err.h when it starts to compile again.
 * This work-around (to not use ERR) was suggested by ERR owner.  Apparently 
 * it is some problem with MPSS.5.0 not supporting the QShrink 4 feature. */
#if defined(DIAG_USERPD_TEMP)
  #define  ABT_ERR(format, code1, code2, code3)          \
    MSG_ERR_LOG (3, format, code1, code2, code3)
#else
  #define  ABT_ERR(format, code1, code2, code3)          \
    MSG_ERR_LOG (3, format, code1, code2, code3,msg_file,__LINE__)
#endif

/*============================================================================
                          TYPE DEFINITION
============================================================================*/

/**
 * ABT Device Syndrome data type
 */
typedef struct
{
    char*    slave_name;                     /**< ABT slave name             */
    uint32_t id;                             /**< Syndrome ID (BID, PID, MID)*/
    uint32_t addr0;                          /**< Syndrome Address 0 (lower) */
    uint32_t addr1;                          /**< Syndrome Address 1 (upper) */
    uint32_t hready;                         /**< Syndrome HREADY for slaves */
    uint32_t num_slaves;                     /**< Number of slaves on bus    */
}ABT_syndrome_info_type;


/*============================================================================
                                 FUNCTIONS
============================================================================*/

/*==========================================================================*/
/**
@brief 
      This function reads Interrupt status register and captures syndrome
      registers from corresponding ABT slaves.
 
@param[in]  void* arg context data for client.

@return    
      None.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*==============================================================================*/
void* ABT_Handle_Interrupt(void* arg);

#endif /* __ABT_INTERNAL_H__ */

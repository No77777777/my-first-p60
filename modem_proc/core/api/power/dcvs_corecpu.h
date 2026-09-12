/*============================================================================
@file dcvs_corecpu.h

Defines supported keys for vector requests to "/core/cpu" resource, also contains
a definition for the maximum number of hardware threads

Copyright (c) 2009-2013 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

$Header: //components/rel/core.mpss/10.0/api/power/dcvs_corecpu.h#1 $
============================================================================*/
#ifndef DCVS_CORECPU_H
#define DCVS_CORECPU_H

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
 * Constants
 * -------------------------------------------------------------------------*/

//supported keys for mca vector requests
#define MPPS_REQ_KEY           0x4D505053 // 'M' 'P' 'P' 'S'
#define CPP_REQ_KEY            0x43505000 // 'C' 'P' 'P'
#define Q6_CLK_REQ_KEY         0x51434C4B // 'Q' 'C' 'L' 'K'
#define REQUEST_PRIORITY_KEY   0x52515052 // 'R' 'Q' 'P' 'R'
#define MIPS_PER_HW_THREAD_KEY 0x4D504857 // 'M' 'P' 'H' 'W'      

//request prioritys for mca requests
#define REQ_PRIORITY_DEFAULT 0
#define REQ_PRIORITY_LOW     1
#define REQ_PRIORITY_MEDIUM  2
#define REQ_PRIORITY_HIGH    3

#ifdef __cplusplus
}
#endif

#endif // DCVS_CORECPU_H

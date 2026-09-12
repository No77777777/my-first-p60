#ifndef QMP_MSG_CLIENT_H
#define QMP_MSG_CLIENT_H

/*===========================================================================
                             qmp_msg_client.h

DESCRIPTION:
  Implements middleware to send messages to AOP over QMP 

Copyright (c) 2017 QUALCOMM Technologies, Inc. (QTI). All Rights Reserved.  
QUALCOMM Proprietary.  Export of this technology or software is regulated 
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/


#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// Include Files
//---------------------------------------------------------------------------
#include "glink.h"
//---------------------------------------------------------------------------
// Constant / Define Declarations
//---------------------------------------------------------------------------
#define GLINK_NUM_EVENTS         2
#define GLINK_TIMEOUT_USEC       10000000
#define GLINK_DATA_SIZE          0x60

//---------------------------------------------------------------------------
// Type Declarations
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// Forward Declarations
//---------------------------------------------------------------------------

//===========================================================================
//                             Function Definitions
//===========================================================================

/**
 * <!-- send2aop -->
 *
 * @brief Transfers the buffer pointed by arg1 char* to aop of
 *        size arg2. Json string format can be found at go/aopsw
 *       
 *
 * @param json_str    : pointer to a valid json string
 * @param size        : size of the string. STRLEN + 1.
 * 
 */

glink_err_type send2aop(char* json_str, size_t size);


#endif   /* QMP_MSG_CLIENT_H */


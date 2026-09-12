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
#include "../../../mproc/glink/os/inc/glink_os_type.h"
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
 * <!-- qmp_client_init -->
 *
 * @brief Initializes qmp client
 *
 * @param void    : void
 * @param void    : void
 */
void qmp_client_init( void );

/**
 * <!-- qmp_msg_client_close -->
 *
 * @brief Wraps glink_close(), deregister link state callbacks
 *        and DAL events
 *       
 *
 * @param void    : void
 * @return glink_err_type  :
 *         GLINK_STATUS_FAILURE/GLINK_STATUS_SUCCESS
 * 
 */
glink_err_type qmp_msg_client_close(void);

#endif   /* QMP_MSG_CLIENT_H */


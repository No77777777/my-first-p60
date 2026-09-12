/*!
 * \file rr_qsh_event_handler.c
 *
 * This module contains functionality to interface to QSH via event handlers.
* Also, this file is used by QSH to register the qsh event handlers in a dispatch table.
* This table is used to trigger the handlers at appropriate places or scenarios.
 *
 *              Copyright (c) 2016 Qualcomm Technologies, Inc.
 *              All Rights Reserved.
 *              Qualcomm Confidential and Proprietary
 */
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_qsh_event_hdlr.c#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"

#if defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)
#error code not present
#endif //FEATURE_QSH_EVENT_NOTIFY_HANDLER


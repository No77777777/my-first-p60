#ifndef PS_TIMESTAMP_H
#define PS_TIMESTAMP_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             P S _ T I M E S T A M P . H

DESCRIPTION
  Header file defining all of the data types and the interface control block
  for the ps timestamp utilities.

DEPENDENCIES
  None of these functions should EVER be called from Interrupt context!

EXTERNALIZED FUNCTIONS

Copyright (c) 2002-2015 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ps_timestamp.h#2 $
  $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/03/15    wc     Initial version. Added timestamp macros.

===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "dsm_item.h"
#include "qurt_sclk.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

/*===========================================================================

                         EXTERNAL DATA DECLARATIONS

===========================================================================*/


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                      PUBLIC MACRO/FUNCTION DEFINITIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

/*===========================================================================
MACRO PS_DSM_SET_TIMESTAMP()

DESCRIPTION
  This macro sets timestamp on the specified DSM item.

PARAMETERS
  dsm_item_ptr: pointer to the DSM item
  timestamp: timestamp value 

RETURN VALUE
  None
===========================================================================*/
#define PS_DSM_SET_TIMESTAMP(dsm_item_ptr, timestamp)                     \
  dsm_item_set_time_stamp(dsm_item_ptr, timestamp) 

/*===========================================================================
MACRO PS_DSM_GET_TIMESTAMP()

DESCRIPTION
  This macro retrieves timestamp from the specified DSM item.

PARAMETERS
  dsm_item_ptr: pointer to the DSM item.

RETURN VALUE
  Timestamp set on the DSM item.
===========================================================================*/
#define PS_DSM_GET_TIMESTAMP(dsm_item_ptr)                                \
  dsm_item_get_time_stamp(dsm_item_ptr)

/*===========================================================================
MACRO PS_GET_TIMESTAMP_3_4_MS()

DESCRIPTION
  This macro gets current timestamp in 3.4 ms granularity.

PARAMETERS
  None

RETURN VALUE
  A uint16 timestamp in 3.4 ms granularity. 
===========================================================================*/
#ifndef TEST_FRAMEWORK
#define PS_GET_TIMESTAMP_3_4_MS()                                           \
qurt_sysclock_get_hw_ticks_16()
#endif
#ifdef TEST_FRAMEWORK                               
#error code not present
#endif
#endif /* PS_TIMESTAMP_H */


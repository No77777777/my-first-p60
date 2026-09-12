#ifndef MM_IDS_H_
#define MM_IDS_H_
/*===========================================================================

                   GUID interpretation

  Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

$Header: //components/rel/avs.mpss/9.0.c51/api/mmutils/mm_ids.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
6/5/2018   rbhatnk      Initial version
===========================================================================*/
#include "mm_guids.h"
/**
 * Zero is an invalid instance ID.
 *
 * It cannot be used for a container, subgraph or a module-instance.
 */
#define MM_INVALID_INSTANCE_ID                           0

/**
 * Gecko static instance ID range is from 1-0x2000 (including both ends)
 * E.g. 1 is APM_MODULE_INSTANCE_ID
 *
 *
 * Module instance ID, container (instance) ID, subgraph (instance) ID must all be unique.
 * i.e. a number used for module-instance-ID cannot be used for container-ID and so on.
 *
 * The below ranges are applicable for module-instance-ID, subgraph ID, and container ID.
 *
 */
#define MM_GECKO_STATIC_INSTANCE_ID_RANGE_BEGIN             1
#define MM_GECKO_STATIC_INSTANCE_ID_RANGE_END               MM_NON_GUID(0x2000)

/**
 * Static instance ID range for platform driver 0x2001-0x4000 (including both ends)
 */
#define MM_PLATFORM_DRIVER_STATIC_INSTANCE_ID_RANGE_BEGIN   MM_NON_GUID(0x2001)
#define MM_PLATFORM_DRIVER_STATIC_INSTANCE_ID_RANGE_END     MM_NON_GUID(0x4000)

/**
 * Dynamic instance IDs starts from 0x4001. QACT uses this range to assign instance IDs.
 * The range includes this number.
 *
 */
#define MM_DYNAMIC_INSTANCE_ID_RANGE_BEGIN                  MM_NON_GUID(0x4001)


/**
 * Following masks apply to Port-IDs
 *
 * Port-ID has to be unique for the module.
 * Two modules can have same port-ID
 *
 * Port-ID cannot be zero.
 */

#define MM_PORT_ID_INVALID                      0
/**
 * Control ports have MSB set. Data ports have MSB clear.
 * Static Control Ports have 2 Most significant bits set
 */
#define MM_PORT_DATA_TYPE_MASK                       MM_NON_GUID(0x80000000)
#define MM_PORT_DATA_TYPE_SHIFT                      31
#define MM_PORT_DATA_TYPE_DATA                       0
#define MM_PORT_DATA_TYPE_CONTROL                    1

#define MM_CONTROL_PORT_TYPE_MASK                    MM_NON_GUID(0x40000000)
#define MM_CONTROL_PORT_TYPE_SHIFT                   30
#define MM_CONTROL_PORT_TYPE_STATIC                  1      
#define MM_CONTROL_PORT_TYPE_DYNAMIC                 0

/**
 * LSB determines if a port id input (0) or output (1)
 * input - even
 * output - odd
 */
#define MM_PORT_DIR_TYPE_MASK                       MM_NON_GUID(0x00000001)
#define MM_PORT_DIR_TYPE_SHIFT                      0
#define MM_PORT_DIR_TYPE_INPUT                      0
#define MM_PORT_DIR_TYPE_OUTPUT                     1

#endif /* MM_IDS_H_ */


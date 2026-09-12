#ifndef MM_GUIDS_H_
#define MM_GUIDS_H_
/*===========================================================================

                   GUID interpretation

  Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

$Header: //components/rel/avs.mpss/9.0.c51/api/mmutils/mm_guids.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
5/8/18   rbhatnk      Initial version
===========================================================================*/

/**
 * GUIDs are 32 bit numbers.
 *
 */

/**
 * An empty macro to mark non-GUIDs so that GUID script doesn't mistake the ID for a GUID
 *
 */
#define MM_NON_GUID(x)                    x

/**
 * GUID Owners
 */
/**
 * GUID owner is Qualcomm Technologies, Inc.
 */
#define MM_GUID_OWNER_QC                  0x0
/**
 * GUID owner is not Qualcomm Technologies, Inc. 
 * All ISVs, OEMs & Customers must use this range.
 */
#define MM_GUID_OWNER_NON_QC              0x1
   /** Rest are reserved */

/**
 * Mask for Owner
 */
#define MM_GUID_OWNER_MASK                MM_NON_GUID(0xF0000000)
/**
 * Shift for Owner
 */
#define MM_GUID_OWNER_SHIFT               28

/**
 * GUID types
 */
/**
 * Reserved GUID type. Used for legacy IDs.
 */
#define MM_GUID_TYPE_RESERVED             0x0

/**
 * GUID type for Control commands.
 */
#define MM_GUID_TYPE_CONTROL_CMD          0x1
/**
 * GUID type for command responses of control commands.
 */
#define MM_GUID_TYPE_CONTROL_CMD_RSP      0x2
/**
 * GUID type for control events.
 */
#define MM_GUID_TYPE_CONTROL_EVENT        0x3
/**
 * GUID type for data commands
 */
#define MM_GUID_TYPE_DATA_CMD             0x4
/**
 * GUID type for data command responses.
 */
#define MM_GUID_TYPE_DATA_CMD_RSP         0x5
/**
 * GUID type for data events
 */
#define MM_GUID_TYPE_DATA_EVENT           0x6
/**
 * GUID type for module-id
 */
#define MM_GUID_TYPE_MODULE_ID            0x7
/**
 * GUID type for module's param & event-id
 */
#define MM_GUID_TYPE_PARAM_EVENT_ID       0x8
/**
 * GUID type for media format-id.
 */
#define MM_GUID_TYPE_FORMAT_ID            0x9
/**
 * GUID type is CAPI.
 * All CAPI framework extensions, interface extensions
 * their events, params use this.
 *
 * All module params & events must use MM_GUID_TYPE_PARAM_EVENT_ID
 */
#define MM_GUID_TYPE_CAPI                 0xA

/** Rest are reserved */

/**
 * Mask for GUID type
 */
#define MM_GUID_TYPE_MASK                 MM_NON_GUID(0x0F000000)
/**
 * Shift for GUID type
 */
#define MM_GUID_TYPE_SHIFT                24

/**
 * Zero is an invalid ID.
 * To be used like NULL value for pointers.
 */
#define MM_GUID_INVALID                   0



#endif /* MM_GUIDS_H_ */


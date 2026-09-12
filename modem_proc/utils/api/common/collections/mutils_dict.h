/*!
  @file
  mutils_dict.h

  @brief
  Interface for efficient dictionary lookup.

  @note
  Any synchronization should be done by user, i.e. functions are not 
  thread-safe.
*/

/*==============================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments dictribing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/utils.mpss/5.4/api/common/collections/mutils_dict.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
05/25/18   mm      CR 2248612: Implement efficient lookup
03/06/18   mm      CR 2132100: Initial check-in
==============================================================================*/

#ifndef MUTILS_DICT_H
#define MUTILS_DICT_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include <mutils_common.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*!
  Key lookup types supported.
*/
typedef enum
{
  /*! integer, e.g. index/count */
  MUTILS_DICT_KEY_TYPE_INT,

  /*! pointer, or integer with high correlation w.r.t. other keys */
  MUTILS_DICT_KEY_TYPE_PTR,

  /*! string */
  MUTILS_DICT_KEY_TYPE_STR,

  MUTILS_DICT_KEY_TYPE_MAX
} mutils_dict_key_type_e;

/*!
  Lock types supported.
*/
typedef enum
{
  /*! no mutex: client to ensure API calls are serialized */
  MUTILS_DICT_LOCK_TYPE_NONE,

  /*! mutex provided by OS */
  MUTILS_DICT_LOCK_TYPE_OS,

  MUTILS_DICT_LOCK_TYPE_MAX
} mutils_dict_lock_type_e;

/*!
  Opaque type for key.
*/
typedef uint32 mutils_dict_key_t;

/*!
  Opaque type for value stored per key.
*/
typedef void * mutils_dict_val_t;

/*!
  Opaque handle per dictionary.
*/
typedef void * mutils_dict_t;

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*==============================================================================

  FUNCTIONS:  mutils_dict_init
              mutils_dict_deinit

==============================================================================*/
/*!
  Create/free resources associated with dictionary.

  @note
  Bucket count to be budgeted by user. Approximately corresponds to number of
  cache lines utilized across multiple lookups.
*/
/*============================================================================*/
mutils_dict_t mutils_dict_init
(
  /*! type of key lookup */
  mutils_dict_key_type_e    key_type,

  /*! lock type */
  mutils_dict_lock_type_e   lock_type,

  /*! number of buckets to use */
  uint32                    bucket_count
);

void mutils_dict_deinit
(
  /*! dictionary to deinit */
  mutils_dict_t   dict_ptr
);

/*==============================================================================

  FUNCTIONS:  mutils_dict_put
              mutils_dict_get
              mutils_dict_in
              mutils_dict_del
              mutils_dict_size

==============================================================================*/
/*!
  Dictionary accessor functions.

  put()
    Create new key->value mapping, or update if it exists.
  get()
    Get value by key, returning FALSE if it doesn't exist.
  in()
    Return whether or not the key exists.
  del()
    Delete key mapping.
  size()
    Returns number of elements in dictionary.
*/
/*============================================================================*/
void mutils_dict_put
(
  mutils_dict_t             dict,
  mutils_dict_key_t         key,
  mutils_dict_val_t         val
);

boolean mutils_dict_get
(
  mutils_dict_t             dict,
  mutils_dict_key_t         key,
  mutils_dict_val_t *       val_ptr
);

boolean mutils_dict_in
(
  mutils_dict_t             dict,
  mutils_dict_key_t         key
);

void mutils_dict_del
(
  mutils_dict_t             dict,
  mutils_dict_key_t         key
);

uint32 mutils_dict_size
(
  mutils_dict_t             dict
);

/*==============================================================================

  FUNCTIONS:  mutils_dict_lock
              mutils_dict_unlock

==============================================================================*/
/*!
  Lock/unlock helpers.
*/
/*============================================================================*/
void mutils_dict_lock
(
  mutils_dict_t   dict
);

void mutils_dict_unlock
(
  mutils_dict_t   dict
);

#endif /* MUTILS_DICT_H */

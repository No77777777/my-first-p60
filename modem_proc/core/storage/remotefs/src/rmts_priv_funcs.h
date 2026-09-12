/****************************************************************************
 * rmts_priv_funcs.h
 *
 * Rmts client private functions.
 * Copyright (C) 2016 QUALCOMM Technologies, Inc.
 *
 *
 ***************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/remotefs/src/rmts_priv_funcs.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2016-07-18   jv    Create

===========================================================================*/

#ifndef __RMTS_PRIV_FUNCS_H__
#define __RMTS_PRIV_FUNCS_H__


#include "rmts_spinor.h"

int rmts_guid_partition_mapping(struct rmts_guid *guid,
                                       char* partition_name,
            int partition_name_len);



#endif /* __RMTS_PRIV_FUNCS_H__ */

/***********************************************************************
 * rmts_spinor.h
 *
 * RMTS SPINOR module
 * Copyright (C) 2016 QUALCOMM Technologies, Inc.
 *
 ***********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/remotefs/src/rmts_spinor.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2016-02-22   mj   Create

===========================================================================*/

#ifndef __RMTS_SPINOR_H__
#define __RMTS_SPINOR_H__
#include "rmts_api.h"
#ifdef FEATURE_RMTS_USING_SPINOR

void rmts_spinor_init(void);

int rmts_load_partitions_from_spinor_to_ram (
            uint64 shared_ram_addr, uint32 shared_ram_size,
						uint64 modem_ram_addr, uint32 modem_ram_size,
						uint64 golden_ram_addr, uint32 golden_ram_size);

int rmts_write_partition_to_spinor (uint8* buf, uint32 buf_size,
                                    struct rmts_guid *guid);

#else /* FEATURE_RMTS_USING_SPINOR */

static inline void rmts_spinor_init(void)
{

}

static inline int rmts_load_partitions_from_spinor_to_ram (
            uint64 shared_ram_addr, uint32 shared_ram_size,
						uint64 modem_ram_addr, uint32 modem_ram_size,
						uint64 golden_ram_addr, uint32 golden_ram_size)
{
	(void) shared_ram_addr;
	(void) shared_ram_size;
	(void) modem_ram_addr;
	(void) modem_ram_size;
	(void) golden_ram_addr;
	(void) golden_ram_size;
	return 0;
}

static inline int rmts_write_partition_to_spinor (uint8* buf, 
					uint32 buf_size, struct rmts_guid *guid)
{
	(void) buf;
	(void) buf_size;
	(void) guid;
	return 0;
}

#endif /* FEATURE_RMTS_USING_SPINOR */

#endif //__RMTS_SPINOR_H__

/****************************************************************************
 * rmts_flash_shim.h
 *
 * Rmts client module device shim layer.
 * Copyright (C) 2016 QUALCOMM Technologies, Inc.
 *
 *
 ***************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/remotefs/src/rmts_flash_shim.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2013-06-28   rp    Create

===========================================================================*/

#ifndef __RMTS_FLASH_SHIM_H__
#define __RMTS_FLASH_SHIM_H__

#ifndef RMTS_CLIENT_DM_ALIGN_BYTES
  #define RMTS_CLIENT_DM_ALIGN_BYTES       4
#endif

#include "flash.h"
#include "flash_miparti.h"

/* Is this pointer 32-byte aligned?
 * Note that this assumes pointers are uint32-equivalent */
#define RMTS_CLIENT_OKAY_FOR_DM(x) \
              (((uint32)(x) & (RMTS_CLIENT_DM_ALIGN_BYTES-1)) == 0)

struct rmts_client_shim_handle_type;
typedef struct rmts_client_shim_handle_type* rmts_client_shim_handle;


void rmts_client_shim_init (void);

rmts_client_shim_handle rmts_client_shim_open_partition (
                                        const char *partition_name);

int rmts_client_shim_close_partition (rmts_client_shim_handle handle);

uint32 rmts_client_shim_get_page_size (rmts_client_shim_handle handle);

uint32 rmts_client_shim_get_block_size (
                                     rmts_client_shim_handle handle);

uint32 rmts_client_shim_get_block_count(
                                      rmts_client_shim_handle handle);


int rmts_client_shim_erase_block (rmts_client_shim_handle handle,
                                      uint32 block_no);

int
rmts_client_shim_erase_blocks (rmts_client_shim_handle handle,
                              uint32 block_no, uint32 block_count);


int rmts_client_shim_write_page (rmts_client_shim_handle handle,
          uint8 *page_buff, uint32 block_no, uint32 page_no);

int rmts_client_shim_read_page (rmts_client_shim_handle handle,
          uint8 *page_buff, uint32 block_no, uint32 page_no);


void rmts_client_shim_ops_start (void);

void rmts_client_shim_ops_end (void);

#endif /* __RMTS_FLASH_SHIM_H__ */

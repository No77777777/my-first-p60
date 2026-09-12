/****************************************************************************
 * rmts_spinor.c
 *
 * RMTS client module device shim layer.
 * Copyright (C) 2016 QUALCOMM Technologies, Inc.
 *
 *
 ***************************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/remotefs/src/rmts_spinor.c#1 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2016-02-17   jv    Support multiple OEM EFS golden copies.
2016-02-17   mj    Create

===========================================================================*/

#include "rmts_spinor.h"
#include "rmts_flash_shim.h"
#include "err.h"
#include "flash.h"
#include "flash_ops_setup.h"
#include "flash_miparti.h"
#include "flash_nor_partition_tbl.h"
#include "assert.h"
#include "DALFramework.h"
#include "DALSys.h"
#include "DALDeviceId.h"
#include "stringl/stringl.h"
#include "rmts_priv_funcs.h"


#include <string.h>

#define PARTITION_NAME_LENGTH 10

PACKED struct rmts_image_header_boot_header
{
  uint32 image_id;           /**< Identifies the type of image this header
                                  represents (OEM SBL, AMSS, Apps boot loader,
                                  etc.). */
  uint32 header_vsn_num;     /**< Header version number. */
  uint32 image_src;          /**< Offset from end of the Boot header where the
                                  image starts. */
  uint8* image_dest_ptr;     /**< Pointer to location to store image in RAM.
                                  Also, entry point at which image execution
                                  begins. */
  uint32 image_size;         /**< Size of complete image in bytes */
  uint32 code_size;          /**< Size of code region of image in bytes */
  uint8* signature_ptr;      /**< Pointer to images attestation signature */
  uint32 signature_size;     /**< Size of the attestation signature in
                                 bytes */
  uint8* cert_chain_ptr;     /**< Pointer to the chain of attestation
                                 certificates associated with the image. */
  uint32 cert_chain_size;    /**< Size of the attestation chain in bytes */
} PACKED_POST;

#define RMTS_IMG_HEADER_IMAGE_VERSION      (3)
#define RMTS_IMG_HEADER_IMAGE_ID_FS1       (16)
#define RMTS_IMG_HEADER_IMAGE_ID_FS2       (17)
#define RMTS_IMG_HEADER_IMAGE_ID_FSG       (20)
#define RMTS_IMG_HEADER_IMAGE_ID_INVALID   (0x7FFFFFFF)


#undef RMTS_SPINOR_READ_BACK_AND_VERIFY

#ifdef RMTS_SPINOR_READ_BACK_AND_VERIFY

static uint8 rmts_spinor_verify_buf[256];

static int rmts_spinor_readback_cmp(uint8 *buf1, uint8 *buf2, uint32 size)
{
   uint32 i = 0;
   int result = 0;

   for (i=0; i<size; i++)
   {
     if (*buf1 != *buf2)
     {
         result = -1;
         i = size+1;
    }
   }

   return result;
}
#endif

static uint32 convert_64_to_32 (uint64 value)
{
  uint32 msb_32, lsb_32 ;

  msb_32 =(uint32)((value & 0xFFFFFFFF00000000) >> 32);
  lsb_32 = (uint32)(value & 0xFFFFFFFF);

  ASSERT(msb_32 == 0);
  return lsb_32;

}

struct rmts_spinor_partition_debug_info_type
{
  const char *partition_name;
  uint8 *buf;
  uint32 buf_size;
  uint32 block_size;
  uint32 page_size;
  uint32 block_count;
  uint32 total_page_count;
  uint32 ram_block_count;
  uint32 ram_total_page_count;
  uint32 min_block_count;
  uint32 min_total_page_count;
  int result;
};

struct rmts_spinor_debug_info_type
{
  uint64 fs1_ram_addr;
  uint64 fs2_ram_addr;
  uint64 fsg_ram_addr;
  uint32 fs1_ram_size;
  uint32 fs2_ram_size;
  uint32 fsg_ram_size;
  int loaded_to_fs1;
  int loaded_to_fs2;
  
  uint32 parti_idx;
  struct rmts_spinor_partition_debug_info_type parti_info[5];
};

static struct rmts_spinor_debug_info_type rmts_spinor_debug_info;

static int rmts_load_to_ram_from_spinor_helper (const char *partition_name,
             uint8 *buf, uint32 read_size)
{
  uint32 block_size , page_size;
  uint32 block_count, page_count, block, page, block_size_in_bytes;
  uint32 ram_block_count, ram_total_page_count;
  uint32 min_block_count, min_total_page_count;
  uint32 total_page_count;
  int result = -1;
  rmts_client_shim_handle shim_handle;
  struct rmts_spinor_partition_debug_info_type *parti_debug_info = NULL;

  ASSERT(buf != NULL);
  shim_handle = rmts_client_shim_open_partition (partition_name);
  ASSERT (shim_handle != NULL);
  if (shim_handle == NULL)
    return -1;

  block_size = rmts_client_shim_get_block_size (shim_handle);
  page_size = rmts_client_shim_get_page_size (shim_handle);

  block_size_in_bytes = block_size * page_size;
  
  ram_block_count = ((read_size -1 ) / block_size_in_bytes ) + 1;
  ram_total_page_count = ((read_size - 1) / page_size ) + 1;

  block_count = rmts_client_shim_get_block_count(shim_handle);
  total_page_count = block_count * block_size;

  min_block_count = (ram_block_count < block_count) ?
                     ram_block_count : block_count;

  min_total_page_count = (ram_total_page_count < total_page_count) ?
                          ram_total_page_count : total_page_count;
  
  ++rmts_spinor_debug_info.parti_idx;
  if (rmts_spinor_debug_info.parti_idx > 5)
    rmts_spinor_debug_info.parti_idx = 0;
  
  parti_debug_info = &rmts_spinor_debug_info.parti_info[rmts_spinor_debug_info.parti_idx];
  
  parti_debug_info->partition_name = partition_name;
  parti_debug_info->buf = buf;
  parti_debug_info->buf_size = read_size;
  parti_debug_info->block_size = block_size;
  parti_debug_info->page_size = page_size;
  parti_debug_info->block_count = block_count;
  parti_debug_info->total_page_count = total_page_count;
  parti_debug_info->ram_block_count = ram_block_count;
  parti_debug_info->ram_total_page_count = ram_total_page_count;
  parti_debug_info->min_block_count = min_block_count;
  parti_debug_info->min_total_page_count = min_total_page_count;

  page_count = 0;
  for (block = 0; block < min_block_count; ++block)
  {
    for (page = 0; page < block_size; ++page)
    {
      result = rmts_client_shim_read_page (shim_handle, buf,
              block, page);
      if(result != 0)
        break;

      buf += page_size;
      page_count ++;
      if(page_count == min_total_page_count)
        break;
    }
  }

  parti_debug_info->result = result;

  (void) rmts_client_shim_close_partition(shim_handle);

  return result;
}



int rmts_load_partitions_from_spinor_to_ram (
            uint64 shared_ram_addr, uint32 shared_ram_size,
						uint64 modem_ram_addr, uint32 modem_ram_size,
						uint64 golden_ram_addr, uint32 golden_ram_size)
{

  int result;
  struct rmts_image_header_boot_header  *image_header;
  uint32 shared_ram_addr_32, golden_ram_addr_32, modem_ram_addr_32;
  
  memset(&rmts_spinor_debug_info, 0, sizeof(rmts_spinor_debug_info));

  ASSERT(shared_ram_addr != 0);
  if(shared_ram_addr == 0)
    return -1;
  shared_ram_addr_32 = convert_64_to_32(shared_ram_addr);
  if(shared_ram_addr_32 == 0)
    return -1;
  
  ASSERT(golden_ram_addr != 0);
  if(golden_ram_addr == 0)
    return -1;
  golden_ram_addr_32 = convert_64_to_32(golden_ram_addr);
  if(golden_ram_addr_32 == 0)
    return -1;

  ASSERT(modem_ram_addr != 0);
  if(modem_ram_addr == 0)
    return -1;
  modem_ram_addr_32 = convert_64_to_32(modem_ram_addr);
  if(modem_ram_addr_32 == 0)
    return -1;

  rmts_spinor_debug_info.fs1_ram_addr = modem_ram_addr;
  rmts_spinor_debug_info.fs2_ram_addr = shared_ram_addr;
  rmts_spinor_debug_info.fsg_ram_addr = golden_ram_addr;
  
  rmts_spinor_debug_info.fs1_ram_size = modem_ram_size;
  rmts_spinor_debug_info.fs2_ram_size = shared_ram_size;
  rmts_spinor_debug_info.fsg_ram_size = golden_ram_size;

  flash_ops_start ();
  image_header = (struct rmts_image_header_boot_header  *)shared_ram_addr_32;
  ASSERT(image_header != 0);

  memset((uint8 *)modem_ram_addr_32, 0, modem_ram_size);
  memset((uint8 *)shared_ram_addr_32, 0, shared_ram_size);
  memset((uint8 *)golden_ram_addr_32, 0, golden_ram_size);
  /* load efs to either shared or modem ram based on fs type*/

  result = rmts_load_to_ram_from_spinor_helper (FLASH_NOR_PARTI_EFS2,
        (uint8 *) image_header, sizeof(struct rmts_image_header_boot_header));
  ASSERT(result == 0);
  if (result != 0)
  {
    memset(image_header, 0, sizeof(struct rmts_image_header_boot_header));
    goto Error;
  }

  if (image_header->image_id == RMTS_IMG_HEADER_IMAGE_ID_FS1)
  {
    rmts_spinor_debug_info.loaded_to_fs1 = 1;
    result = rmts_load_to_ram_from_spinor_helper (FLASH_NOR_PARTI_EFS2,
                           (uint8 *)modem_ram_addr_32, modem_ram_size);
    ASSERT(result == 0);
    if(result != 0)
      goto Error;
  }
  else if (image_header->image_id == RMTS_IMG_HEADER_IMAGE_ID_FS2)
  {
    rmts_spinor_debug_info.loaded_to_fs2 = 1;
    result = rmts_load_to_ram_from_spinor_helper (FLASH_NOR_PARTI_EFS2,
                         (uint8 *)shared_ram_addr_32, shared_ram_size);
    ASSERT(result == 0);
    if (result != 0)
      goto Error;
  }

  /* load sfp to golden ram*/
  rmts_load_to_ram_from_spinor_helper (FLASH_NOR_PARTI_SFP,
            (uint8 *) golden_ram_addr_32, golden_ram_size);
  ASSERT(result == 0);
  if (result != 0)
      goto Error;

  flash_ops_end ();

  return 0;

Error:
  flash_ops_end ();
  return -1;

}

int rmts_write_partition_to_spinor (uint8* buf, uint32 buf_size,
                                    struct rmts_guid *guid)
{
  char partition_name[PARTITION_NAME_LENGTH];
  rmts_client_shim_handle shim_handle;
  uint32 block_size , page_size;
  uint32 block_count, page_count, block, page, block_size_in_bytes;;
  uint32 total_page_count;
  int result = -1;
  flash_ops_start ();

  if (rmts_guid_partition_mapping(guid, &partition_name[0],
            PARTITION_NAME_LENGTH) < 0)
    goto Error;

  shim_handle = rmts_client_shim_open_partition (partition_name);
  ASSERT (shim_handle != NULL);

  block_size = rmts_client_shim_get_block_size (shim_handle);
  page_size = rmts_client_shim_get_page_size (shim_handle);

  block_size_in_bytes = block_size * page_size;
  block_count = ((buf_size - 1) / block_size_in_bytes ) + 1;
  total_page_count = ((buf_size - 1) / page_size ) + 1;
  page_count = 0;

  result = rmts_client_shim_erase_blocks (shim_handle, 0, block_count);
  ASSERT(result == 0);
  if(result != 0)
    goto Error;
  for (block = 0; block < block_count; ++block)
  {
    for (page = 0; page < block_size; ++page)
    {
      result = rmts_client_shim_write_page (shim_handle, buf, block, page);
      ASSERT(result == 0);

#ifdef RMTS_SPINOR_READ_BACK_AND_VERIFY
      {
         result = rmts_client_shim_read_page (shim_handle,
                      (uint8 *)&rmts_spinor_verify_buf[0], block, page);
         ASSERT(result == 0);
         result = rmts_spinor_readback_cmp((uint8 *)&rmts_spinor_verify_buf[0],
                                      buf, sizeof(rmts_spinor_verify_buf));
         ASSERT(result == 0);
      }
#endif /* RMTS_SPINOR_READ_BACK_AND_VERIFY */

      buf += page_size;
      page_count ++;
      if(page_count == total_page_count)
        break;

    }
  }

   (void)rmts_client_shim_close_partition(shim_handle);

  flash_ops_end ();
  return result;
Error:
  flash_ops_end ();
  return -1;
}

void rmts_spinor_init ()
{
  rmts_client_shim_init ();
}

/****************************************************************************
 * rmts_client_flash_dal_shim.c
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

  $Header: //components/rel/core.mpss/10.0/storage/remotefs/src/rmts_flash_shim.c#1 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2016-02-17   mj    Create

===========================================================================*/


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



#include <string.h>

#define MAIN_BUFF_DESC  0
#define SPARE_BUFF_DESC 1

#define FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC1  0x12345678
#define FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC2  0xDEADBEEF
#define FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC3  0xCAFEABBA
#define FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC4  0xACCBBBAA

struct rmts_client_shim_handle_type
{
  uint32 magic1;
  uint32 magic2;
  int is_in_use;

  flash_handle_t nor_handle;
  struct flash_info  info;
  uint8 *page_buf;
  char partition_name[32];

  /* HAL Workspace mem handle */
  DALSYSMemHandle dal_mem_handle;

  /* DAL Memory descriptor for page operations */
  DALSYS_MEM_DESC_LIST_OBJECT (page_buff_desc, 64 * 2);

  uint32 magic3;
  uint32 magic4;
};

#ifndef RMTS_CLIENT_SHIM_MAX_PARTITONS
  #define RMTS_CLIENT_SHIM_MAX_PARTITONS   3
#endif

static struct rmts_client_shim_handle_type
              rmts_client_shim_partitions[RMTS_CLIENT_SHIM_MAX_PARTITONS];

static void
rmts_client_shim_validate_parti (
                     struct rmts_client_shim_handle_type *shim_parti)
{
  ASSERT (shim_parti != NULL);
  ASSERT (shim_parti->is_in_use == 1);
  ASSERT (
      shim_parti->magic1 == FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC1 &&
      shim_parti->magic2 == FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC2 &&
      shim_parti->magic3 == FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC3 &&
      shim_parti->magic4 == FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC4);
}


static void
rmts_client_shim_init_buffer_descriptor (uint32 op_type,
              void *page_buff_addr, void *spare_buff_addr,
              dalsys_mem_desc_list *buffer_desc,
              struct flash_page_iovec  *page_data_iovec)
{
  DALSysMemDescBuf *main_desc_buff;
  DALSysMemDescBuf *spare_desc_buff;

  DALFW_MemDescInit (NULL, (DALSysMemDescList *) buffer_desc, 2);

  main_desc_buff = DALFW_MemDescBufPtr(buffer_desc, MAIN_BUFF_DESC);
  ASSERT (main_desc_buff != NULL);
  main_desc_buff->VirtualAddr = (DALSYSMemAddr)page_buff_addr;
  main_desc_buff->user = page_data_iovec->total_page_count;

  spare_desc_buff = DALFW_MemDescBufPtr(buffer_desc, SPARE_BUFF_DESC);
  ASSERT (spare_desc_buff != NULL);
  spare_desc_buff->VirtualAddr = (DALSYSMemAddr)spare_buff_addr;
  spare_desc_buff->user = page_data_iovec->total_page_count;

  buffer_desc->dwNumDescBufs = 2;

  switch (op_type)
  {
    case FLASH_READ_SPARE:
      spare_desc_buff->size = (page_data_iovec->total_page_count *
                               page_data_iovec->spare_size_bytes);
      main_desc_buff->VirtualAddr = DALSYS_MEM_ADDR_NOT_SPECIFIED;
      main_desc_buff->size = 0;

      break;

    case FLASH_READ_MAIN:
      main_desc_buff->size = (page_data_iovec->total_page_count *
                              page_data_iovec->main_size_bytes);
      spare_desc_buff->VirtualAddr = DALSYS_MEM_ADDR_NOT_SPECIFIED;
      spare_desc_buff->size = 0;
    break;

    case FLASH_READ_MAIN_SPARE:
      main_desc_buff->size  = (page_data_iovec->total_page_count *
                               page_data_iovec->main_size_bytes);
      spare_desc_buff->size = (page_data_iovec->total_page_count *
                               page_data_iovec->spare_size_bytes);
      break;
  }
}


static uint32
rmts_client_shim_detect_total_page_size (void)
{
  struct rmts_client_shim_handle_type *shim_parti;
  uint32 total_page_size;
  DALResult dal_result;
  int result;

  shim_parti = &rmts_client_shim_partitions[0];

  dal_result = flash_device_attach (DALDEVICEID_FLASH_DEVICE_1,
                                    &shim_parti->nor_handle);
  if (dal_result != DAL_SUCCESS)
  {
    ERR_FATAL ("%d, shim : attach failed", dal_result, 0, 0);
  }

  //TODO: Not sure why we need to open a parition to get total p[age size, Need to investigate if this is needed.

  result = flash_open_partition (shim_parti->nor_handle,
                      (const unsigned char *)FLASH_NOR_PARTI_EFS2);
  if (result != FLASH_DEVICE_DONE)
  {
    ERR_FATAL ("shim : flash-open failed", result, 0, 0);
  }

  result = flash_get_info (shim_parti->nor_handle, FLASH_DEVICE_INFO,
                           &shim_parti->info);
  ASSERT (result == FLASH_DEVICE_DONE);
  ASSERT (shim_parti->info.partition.total_page_size_bytes >= 256);
  total_page_size = shim_parti->info.partition.total_page_size_bytes;

  dal_result = flash_device_close (shim_parti->nor_handle);
  if (dal_result != DAL_SUCCESS)
  {
    ERR_FATAL ("%d, close failed", dal_result, 0, 0);
  }

  return total_page_size;
}


static void
rmts_client_shim_allocate_page_memory (uint32 total_page_size)
{
  struct rmts_client_shim_handle_type *shim_parti;
  void *nand_page_buf = NULL;
  DALSYSMemInfo mem_info;     /* HAL Workspace mem info */
  DALResult dal_result;
  uint32 i;

  for (i = 0; i < RMTS_CLIENT_SHIM_MAX_PARTITONS; ++i)
  {
    shim_parti = &rmts_client_shim_partitions[i];

    dal_result = flash_device_attach (DALDEVICEID_FLASH_DEVICE_1,
                                      &shim_parti->nor_handle);
    if (dal_result != DAL_SUCCESS)
    {
      ERR_FATAL ("%d, shim : attach failed", dal_result, 0, 0);
    }

    dal_result = DALSYS_MemRegionAlloc (
                        DALSYS_MEM_PROPS_UNCACHED | DALSYS_MEM_PROPS_PHYS_CONT,
                        DALSYS_MEM_ADDR_NOT_SPECIFIED,
                        DALSYS_MEM_ADDR_NOT_SPECIFIED,
                        total_page_size,
                        &shim_parti->dal_mem_handle, NULL);
    if (dal_result != DAL_SUCCESS)
    {
      ERR_FATAL ("%d, DALSYS_MemRegionAlloc failed",
                             dal_result, 0, 0);
    }

    memset (&mem_info, 0, sizeof (mem_info));
    dal_result = DALSYS_MemInfo (shim_parti->dal_mem_handle, &mem_info);
    ASSERT (dal_result == DAL_SUCCESS);

    nand_page_buf = (void *)mem_info.VirtualAddr;
    shim_parti->page_buf = (uint8 *) nand_page_buf;
    /*  Need to touch the memory for it to be configured in MMU table */
    DALSYS_memset(nand_page_buf, 0xFF, 10);

    DALFW_MemDescInit(NULL,
                      (DALSysMemDescList *)&shim_parti->page_buff_desc,2);

    shim_parti->magic1 = FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC1;
    shim_parti->magic2 = FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC2;
    shim_parti->magic3 = FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC3;
    shim_parti->magic4 = FLASH_SCRUB_SHIM_HANDLE_TYPE_STRUCT_MAGIC4;
  }
}


rmts_client_shim_handle
rmts_client_shim_open_partition (const char *partition_name)
{
  struct rmts_client_shim_handle_type *shim_parti;
  int result;
  uint32 i, size;

  for (i = 0; i < RMTS_CLIENT_SHIM_MAX_PARTITONS; ++i)
  {
    shim_parti = &rmts_client_shim_partitions[i];
    if (!shim_parti->is_in_use)
    {
      break;
    }
  }

  if (i >= RMTS_CLIENT_SHIM_MAX_PARTITONS)
  {
    ERR_FATAL ("shim no free slot", 0, 0, 0);
    //printf ("shim no free slot");
    return NULL;
  }

  result = flash_open_partition (shim_parti->nor_handle,
                                 (const unsigned char *)partition_name);
  if (result != FLASH_DEVICE_DONE)
  {
    ERR_FATAL ("shim : flash-open failed", result, 0, 0);
    //printf ("shim : flash-open failed %d", result);
    return NULL;
  }

  result = flash_get_info (shim_parti->nor_handle, FLASH_DEVICE_INFO,
                           &shim_parti->info);
  ASSERT (result == FLASH_DEVICE_DONE);
  ASSERT (shim_parti->info.partition.total_page_size_bytes >= 256);

  shim_parti->is_in_use = 1;
  size = strlcpy (shim_parti->partition_name, partition_name,
                  sizeof (shim_parti->partition_name));
  if (size >= sizeof (shim_parti->partition_name))
  {
    //printf ("[%lu, %lu, %lu] Partition name too large\r\n",
                // partition_name[2],partition_name[3], size);
  }

  rmts_client_shim_validate_parti (shim_parti);

  //printf ("[%d, %d, %d] shim-open-part good",
        //partition_name[2],partition_name[3], (uint32)shim_parti);

  return shim_parti;
}



int
rmts_client_shim_close_partition (rmts_client_shim_handle handle)
{
  struct rmts_client_shim_handle_type *shim_parti;
  DALResult dal_result;

  shim_parti = (struct rmts_client_shim_handle_type *)handle;
  rmts_client_shim_validate_parti (shim_parti);

  dal_result = flash_device_close (shim_parti->nor_handle);
  if (dal_result != DAL_SUCCESS)
  {
    ERR_FATAL ("%d, close failed", dal_result, 0, 0);
  }

  shim_parti->is_in_use = 0;

  return dal_result;
}


uint32
rmts_client_shim_get_page_size (rmts_client_shim_handle handle)
{
  struct rmts_client_shim_handle_type *shim_parti;

  shim_parti = (struct rmts_client_shim_handle_type *)handle;
  rmts_client_shim_validate_parti (shim_parti);

  return shim_parti->info.partition.page_size_bytes;
}

uint32
rmts_client_shim_get_block_size (rmts_client_shim_handle handle)
{
  struct rmts_client_shim_handle_type *shim_parti;

  shim_parti = (struct rmts_client_shim_handle_type *)handle;
  rmts_client_shim_validate_parti (shim_parti);

  return shim_parti->info.partition.pages_per_block;
}

uint32
rmts_client_shim_get_block_count (rmts_client_shim_handle handle)
{
  struct rmts_client_shim_handle_type *shim_parti;

  shim_parti = (struct rmts_client_shim_handle_type *)handle;
  rmts_client_shim_validate_parti (shim_parti);

  return shim_parti->info.partition.block_count;
}

int
rmts_client_shim_erase_block (rmts_client_shim_handle handle,
                              uint32 block_no)
{
  struct rmts_client_shim_handle_type *shim_parti;
  struct flash_block_vector block_vector;
  int result;

  shim_parti = (struct rmts_client_shim_handle_type *)handle;
  rmts_client_shim_validate_parti (shim_parti);

  memset (&block_vector, 0, sizeof (block_vector));
  block_vector.start_block = block_no;
  block_vector.block_count = 1;
  block_vector.result_vector = &result;

  result = flash_erase_blocks(shim_parti->nor_handle, &block_vector, 1);
  if (result != FLASH_DEVICE_DONE)
  {
    //printf("shim: Erase-fail, res=%d,blk=%lu",result, block_no);
    ERR_FATAL ("%d, erase failed", result, 0, 0);
  }

  return result;
}

int
rmts_client_shim_erase_blocks (rmts_client_shim_handle handle,
                              uint32 block_no, uint32 block_count)
{
  struct rmts_client_shim_handle_type *shim_parti;
  struct flash_block_vector block_vector;
  int result;
  int tmp_block_no = 0;

  shim_parti = (struct rmts_client_shim_handle_type *)handle;
  rmts_client_shim_validate_parti (shim_parti);

  while (tmp_block_no < block_count)
  {
    memset (&block_vector, 0, sizeof (block_vector));
    block_vector.start_block = (block_no + tmp_block_no);
    block_vector.block_count = 1;
    block_vector.result_vector = &result;

    result = flash_erase_blocks(shim_parti->nor_handle, &block_vector, 1);

    if (result != FLASH_DEVICE_DONE)
    {
      break;
    }

    tmp_block_no++;
  }

  if (result != FLASH_DEVICE_DONE)
  {
    ERR_FATAL ("%d, erase failed", result, 0, 0);
  }

  return result;
}


int
rmts_client_shim_write_page (rmts_client_shim_handle handle, uint8 *page_buff,
                             uint32 block_no, uint32 page_no)
{
  struct rmts_client_shim_handle_type *shim_parti;
  struct flash_page_iovec page_iovec;
  int result;
  uint32 page_size, block_size;

  shim_parti = (struct rmts_client_shim_handle_type *)handle;
  rmts_client_shim_validate_parti (shim_parti);

  /* Make sure that the address of the buffer is meeting the alignment
     expectations of the DMA. */
  ASSERT (RMTS_CLIENT_OKAY_FOR_DM (page_buff));

  page_size = shim_parti->info.partition.page_size_bytes;
  block_size = shim_parti->info.partition.pages_per_block;

  memset (&page_iovec, 0, sizeof (page_iovec));
  page_iovec.start_page = ((block_no * block_size) + page_no);
  page_iovec.total_page_count = 1;
  page_iovec.main_size_bytes = page_size;
  page_iovec.spare_size_bytes = 0;

  rmts_client_shim_init_buffer_descriptor ((uint32)FLASH_WRITE_MAIN,
        page_buff, NULL, (DALSysMemDescList *) &shim_parti->page_buff_desc,
        &page_iovec);

  result = flash_write_pages (shim_parti->nor_handle, FLASH_WRITE_MAIN,
                (DALSysMemDescList *)&shim_parti->page_buff_desc,
                 &page_iovec);

  if (result != FLASH_DEVICE_DONE)
  {
    //rmts_client_LOG_ERR_3 ("shim: Write-fail, res=%d,blk=%d,pg=%d\n",
                      //   result, block_no, page_no);
    //printf ("shim: Write-fail, res=%d,blk=%lu,pg=%lu\n",
                      //result, block_no, page_no);
      ERR_FATAL ("%d, Write failed", result, 0, 0);
    //printf ("%d, Write failed", result);
  }

  return result;
}

int
rmts_client_shim_read_page (rmts_client_shim_handle handle, uint8 *page_buff,
                            uint32 block_no, uint32 page_no)
{
  struct rmts_client_shim_handle_type *shim_parti;
  struct flash_page_iovec page_iovec;
  int result;
  uint32 page_size, block_size;

  shim_parti = (struct rmts_client_shim_handle_type *)handle;
  rmts_client_shim_validate_parti (shim_parti);

  /* Make sure that the address of the buffer is meeting the alignment
     expectations of the DMA. */
  ASSERT (RMTS_CLIENT_OKAY_FOR_DM (page_buff));

  page_size = shim_parti->info.partition.page_size_bytes;
  block_size = shim_parti->info.partition.pages_per_block;

  memset (&page_iovec, 0, sizeof (page_iovec));
  page_iovec.start_page = ((block_no * block_size) + page_no);
  page_iovec.total_page_count = 1;
  page_iovec.main_size_bytes = page_size;
  page_iovec.spare_size_bytes = 0;

  rmts_client_shim_init_buffer_descriptor ((uint32)FLASH_READ_MAIN,
        page_buff, NULL, (DALSysMemDescList *) &shim_parti->page_buff_desc,
        &page_iovec);

  result = flash_read_pages (shim_parti->nor_handle, FLASH_READ_MAIN,
                     (DALSysMemDescList *)&shim_parti->page_buff_desc,
                     &page_iovec);

  if (FLASH_DEVICE_FAIL_PAGE_ERASED == result)
  {
    result = FLASH_DEVICE_DONE;
  }

  return result;
}


volatile int rmts_client_turn_off_ops_start = 0;

static void
rmts_client_shim_ops_init (void)
{
    flash_ops_init ();
}

void
rmts_client_shim_ops_start (void)
{
  if (!rmts_client_turn_off_ops_start)
  {
    flash_ops_start ();
  }
}

void
rmts_client_shim_ops_end (void)
{
  if (!rmts_client_turn_off_ops_start)
  {
    flash_ops_end ();
  }
}

void
rmts_client_shim_init (void)
{
  uint32 total_page_size;

  memset (rmts_client_shim_partitions, 0, sizeof
          (rmts_client_shim_partitions));

  total_page_size = rmts_client_shim_detect_total_page_size ();

  rmts_client_shim_allocate_page_memory (total_page_size);

  rmts_client_shim_ops_init ();

}

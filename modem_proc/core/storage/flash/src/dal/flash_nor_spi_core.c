/*=============================================================================
 *
 * FILE:      flash_nor_spi_core.c
 *
 * DESCRIPTION: This file initializes the flash NOR core APIs
 *              using predefined HAL APIs.
 *
 * PUBLIC CLASSES:  Not Applicable
 *
 * INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 *
 * Copyright (c) 2010-2012 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * QUALCOMM Confidential and Proprietary
 *===========================================================================*/

/*===================================================================
 *
 *                       EDIT HISTORY FOR FILE
 *
 *   This section contains comments describing changes made to the
 *   module. Notice that changes are listed in reverse chronological
 *   order.
 *
 *  $Header: //components/rel/core.mpss/10.0/storage/flash/src/dal/flash_nor_spi_core.c#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $
 *
 * when         who     what, where, why
 * ----------   ---     ----------------------------------------------
 * 03/20/12     sv      Add Micron SPI NOR support, Optimize status check
 * 03/19/12     sv      Add APIs for power management
 * 03/19/12     eo      Support for erase blocks by size
 * 02/28/12     sv      Fix Klocwork error
 * 12/14/11     eo      Fix compiler warnings
 * 11/01/11     eo      Add Winbond NOR support
 * 09/22/11     eo      Support for erase blocks by size
 * 07/20/11     sv      fix issue in erase command detection
 * 03/31/11     sv      fix issue in get_status, add global status reg var
 * 03/30/11     sv      check for WR EN before prog/erase
 * 03/08/11     sv      Remove Sleep handling
 * 02/01/11     eo      Add Maxcronix SPI NOR device support
 * 10/08/10     eo      Add check for begin erase completion to APIs
 * 10/04/10     sv      Move read and write wrappers to flash_nor_spi_init.c
 * 09/15/10     sv      SPI NOR tools support
 * 09/15/10     nr      Added stats framework support
 * 08/18/10     eo      Add interim partition support
 * 06/21/10     eo      Add Babylon and Power management support
 * 04/02/10     sv      Updated for Atmel SPI NOR support
 * 03/15/10     bb      Initial Revision
 *==================================================================*/

/*===================================================================
 *
 *                     Include Files
 *
 ====================================================================*/

#include "flash_nor_spi.h"

/*------------------------------------------------------------------------------
  Variables global to this file.
 ------------------------------------------------------------------------------*/

/* SPI NOR interface functions */
flash_vtbl nor_spi_base_vtbl;

/* global that stores failed status register value */
uint8 nor_spi_status_reg = 0;
/*------------------------------------------------------------------------------
  Variables local in this driver.
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  Following functions are local to this driver.
 ------------------------------------------------------------------------------*/
/*
 * Check for erased page verifying all data is equal to 0xFFFFFFFF.
 */
static int nor_spi_erased_page_check(flash_client_ctxt *client_ctxt,
  DALSysMemDescBuf *buf_desc)
{
  int result = FLASH_DEVICE_FAIL_PAGE_ERASED;
  uint32 i, size;
  uint32 *data;

  data = (uint32 *)buf_desc->VirtualAddr;
  size = buf_desc->size;

  /* Check page data for 0xFFFFFFFF */
  for (i=0; i < (size >> 2); i++)
  {
    if (0xFFFFFFFF != *data)
    {
      result = FLASH_DEVICE_DONE;
      break;
    }
    data++;
  }

  return result;
}

/*
 * Reads any register of SPI NOR flash device. read_reg_cmd is used to
 * specify which register to read.
 * Two buffers - 1 for command read and other to read status are required
 */
static int nor_spi_read_register(flash_client_ctxt *client_ctxt,
  uint8 read_reg_cmd, uint8 *status)
{
  int result = FLASH_DEVICE_DONE;
  struct nor_spi_xfer_buf_info spi_read_buf;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  spi_read_buf.spi_cmd_buf = nor_spi_pdata->cmd_addr_buffer;
  spi_read_buf.cmd_buf_len = NOR_SPI_CMD_LEN;
  spi_read_buf.spi_data_buf = nor_spi_pdata->status_buffer;
  spi_read_buf.data_buf_len = NOR_SPI_STATUS_REG_LEN;
  spi_read_buf.spi_cmd_buf[0] = read_reg_cmd;

  nor_spi_read(&spi_read_buf, &result);

  if(result == FLASH_DEVICE_DONE)
  {
    *status = spi_read_buf.spi_data_buf[0];
  }

  return result;
}

/*
 * Check the error status for each operation. If any of the desired error bits
 * are set in the status registers, it indicates that the operation has failed
 * and the function returns failure.
 */
static int nor_spi_check_error_status(flash_client_ctxt *client_ctxt,
  uint8 status)
{
  int result = FLASH_DEVICE_DONE;
  uint8 status_mask = 0;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  switch(nor_spi_pdata->config_data->family)
  {
    /* For Atmel, just use the status register value passed to verify the
   * status. */
    case FLASH_NOR_SPI_ATMEL:
      status_mask = NOR_SPI_STATUS_AT_ERROR_MASK;
      break;

   /* For Macronix, read the security status register to check for the error
    * status of the operation.*/
    case FLASH_NOR_SPI_MACRONIX:
      status_mask = NOR_SPI_STATUS_MX_PFAIL_MASK | NOR_SPI_STATUS_MX_EFAIL_MASK;
      result = nor_spi_read_register(client_ctxt, NOR_SPI_READ_SECURITY_CMD,
        &status);
      break;

    /* For Micron, read the flag status register to check for the error
     status of the operation. */
    case FLASH_NOR_SPI_MICRON:
      status_mask = NOR_SPI_STATUS_MI_PFAIL_MASK | NOR_SPI_STATUS_MI_EFAIL_MASK;
      result = nor_spi_read_register(client_ctxt, NOR_SPI_READ_FLAG_CMD,
        &status);
      break;

   /* For Winbond No status error check is required. */
  case FLASH_NOR_SPI_WINBOND:
    return result;

     /* For any other NOR family, return Failure */
    default:
      result = FLASH_DEVICE_FAIL;
  }

  /* Check device status */
  if ((result == FLASH_DEVICE_DONE) && (status & status_mask))
  {
    result = FLASH_DEVICE_FAIL;
    nor_spi_status_reg = status;
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
      "Flash: nsces - Device program/erase error bit set!");
  }

  return result;
}

/*
 * Check the error status for each operation. This function polls the
 * device until the current operation completes or timeout occurs and checks
 * the status of the operation. If the operation doesnot complete within the
 * timeout period or the status registers shows that an operation has failed,
 * then the function returns failure.
 */
static int nor_spi_check_status(flash_client_ctxt *client_ctxt,
  enum nor_spi_dev_op nor_spi_op)
{
  uint8 status = 0;
  uint32 timeout = 0;
  int result = FLASH_DEVICE_FAIL;

  /* Obtain the timeout based on the operation */
  switch (nor_spi_op)
  {
    case NOR_SPI_ERASE_OP:
     timeout = NOR_SPI_ERASE_TIMEOUT;
     break;

    case NOR_SPI_WRITE_OP:
     timeout = NOR_SPI_PROGRAM_TIMEOUT;
       break;

  default:
     return result;
  }

  /* Query the device to see if the device completed the operation
   * Loop until the operation completes or timeout is reached
   */
  result = nor_spi_read_register(client_ctxt, NOR_SPI_READ_STATUS_CMD, &status);

  while ((status & NOR_SPI_STATUS_BUSY_MASK) && (--timeout) &&
    (result == FLASH_DEVICE_DONE))
  {
    flash_watchdog_type *watchdog = NULL;

    watchdog = (flash_watchdog_type *)flash_get_property(FLASH_KICK_WATCHDOG_ID);
    if (FLASH_PROPERTY_NOT_DEFINED == (uint32)watchdog)
    {
      FLASHLOG(3,(" Flash: Failed to get watchdog function.\n"));
      return FLASH_DEVICE_FAIL;
    }

    result = nor_spi_read_register(client_ctxt, NOR_SPI_READ_STATUS_CMD,
      &status);

    /* Kick the watchdog timer */
    watchdog->kick_watchdog();
  }

  /* Check if timeout occurred */
  if (!timeout)
  {
    result = FLASH_DEVICE_FAIL;
    nor_spi_status_reg = status;
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
      "Flash: nscs - Timeout expired, during operation %u!", nor_spi_op);
  }

  /* Check of error status of the operation that has just completed */
  if (result == FLASH_DEVICE_DONE)
  {
    result = nor_spi_check_error_status(client_ctxt, status);
  }

  return result;
}

/*
 * Write enable register for SPI NOR flash device.
 */
static int nor_spi_write_enable(flash_client_ctxt *client_ctxt)
{
  int result = FLASH_DEVICE_DONE;
  uint8 status = 0;
  uint8 timeout = 20;
  struct nor_spi_xfer_buf_info spi_write_buf;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  spi_write_buf.spi_cmd_buf = nor_spi_pdata->cmd_addr_buffer;
  spi_write_buf.cmd_buf_len = NOR_SPI_CMD_LEN;
  spi_write_buf.spi_data_buf = NULL;
  spi_write_buf.data_buf_len = 0;
  spi_write_buf.spi_cmd_buf[0] = NOR_SPI_WRITE_ENABLE_CMD;

  nor_spi_write(&spi_write_buf, &result);

  if (result == FLASH_DEVICE_DONE)
  {
     while (timeout--)
     {
       /* Read the Status Register and check for Write Enable Latch Bit Enabled */
      result = nor_spi_read_register(client_ctxt, NOR_SPI_READ_STATUS_CMD,
         &status);

      if (result == FLASH_DEVICE_DONE)
      {
        if (status & NOR_SPI_STATUS_WR_EN_MASK)
        {
          break;
        }
      }
      else
      {
        break;
      }
     }
  }

  if ((result != FLASH_DEVICE_DONE) || (timeout == 0))
  {
    result = FLASH_DEVICE_FAIL;
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
          "Flash: nswe - Enabling Write Enable Latch Failed!");
  }

  return result;
}

/* Check for block erase operation completion. */
static int nor_spi_is_block_erase_done(flash_client_ctxt *client_ctxt)
{
  int result = FLASH_DEVICE_DONE;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  if (nor_spi_pdata->erase_in_progress == TRUE)
  {
    /* Poll for Operation Completion and check the error status */
    result = nor_spi_check_status(client_ctxt, NOR_SPI_ERASE_OP);

    /* At this point, device is in ready state. */
    nor_spi_pdata->erase_in_progress = FALSE;
  }

  return result;
}

/*
 * Initiate erase block in SPI NOR flash device.
 */
static int nor_spi_start_block_erase(flash_client_ctxt *client_ctxt,
  uint32 blk_num)
{
  int result = FLASH_DEVICE_DONE;
  uint32 page_num, page_addr;
  uint32 pages_per_block, page_size_bytes;
  struct nor_spi_xfer_buf_info spi_write_buf;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  spi_write_buf.spi_cmd_buf = nor_spi_pdata->cmd_addr_buffer;
  spi_write_buf.cmd_buf_len = NOR_SPI_CMD_LEN + NOR_SPI_ADDR_LEN;
  spi_write_buf.spi_data_buf = NULL;
  spi_write_buf.data_buf_len = 0;

  page_num = blk_num << client_ctxt->client_data.block_size_shift;
  page_addr = page_num << client_ctxt->client_data.page_size_shift;
  pages_per_block = client_ctxt->client_data.pages_per_block;
  page_size_bytes = client_ctxt->client_data.page_size_bytes;

  switch (pages_per_block * page_size_bytes)
  {
    case BLK_SIZE_4K:
      spi_write_buf.spi_cmd_buf[0] = NOR_SPI_ERASE_4KB_BLK_CMD;
      break;
    case BLK_SIZE_32K:
      spi_write_buf.spi_cmd_buf[0] = NOR_SPI_ERASE_32KB_BLK_CMD;
      break;
    case BLK_SIZE_64K:
      spi_write_buf.spi_cmd_buf[0] = NOR_SPI_ERASE_64KB_BLK_CMD;
      break;
  }
  spi_write_buf.spi_cmd_buf[1] = (uint8)((page_addr >> 16) & 0xFF);
  spi_write_buf.spi_cmd_buf[2] = (uint8)((page_addr >> 8) & 0xFF);
  spi_write_buf.spi_cmd_buf[3] = (uint8)((page_addr) & 0xFF);

  nor_spi_write(&spi_write_buf, &result);

  return result;
}

/*
 * Initiate programming page data in SPI NOR flash device.
 */
static int nor_spi_start_page_write(flash_client_ctxt *client_ctxt,
  uint32 page_num, uint32 page_buf, uint32 page_size)
{
  int result = FLASH_DEVICE_DONE;
  struct nor_spi_xfer_buf_info spi_write_buf;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  spi_write_buf.spi_cmd_buf = nor_spi_pdata->cmd_addr_buffer;
  spi_write_buf.cmd_buf_len =
    NOR_SPI_CMD_LEN + NOR_SPI_ADDR_LEN;
  spi_write_buf.spi_data_buf = (uint8 *)page_buf;
  spi_write_buf.data_buf_len = page_size;

  spi_write_buf.spi_cmd_buf[0] = NOR_SPI_PROG_PAGE_CMD;
  spi_write_buf.spi_cmd_buf[1] = (uint8)((page_num >> 16) & 0xFF);
  spi_write_buf.spi_cmd_buf[2] = (uint8)((page_num >> 8) & 0xFF);
  spi_write_buf.spi_cmd_buf[3] = (uint8)((page_num) & 0xFF);

  nor_spi_write(&spi_write_buf, &result);

  return result;
}

/*
 * Erase block operation for SPI NOR flash device.
 */
static int nor_spi_erase_block(flash_client_ctxt *client_ctxt,
  uint32 blk_num, uint8 poll_device_busy)
{
  int result = FLASH_DEVICE_DONE;

  result = nor_spi_write_enable(client_ctxt);

  if (result == FLASH_DEVICE_DONE)
  {
    result = nor_spi_start_block_erase(client_ctxt, blk_num);
  }

  if (poll_device_busy && (result == FLASH_DEVICE_DONE))
  {
    /* Poll for Operation Completion and check the error status */
    result = nor_spi_check_status(client_ctxt, NOR_SPI_ERASE_OP);
  }

  return result;
}

/*
 * Program page data operation for SPI NOR flash device.
 */
static int nor_spi_write_page(flash_client_ctxt *client_ctxt, uint32 page_num,
  DALSysMemDescBuf *buf_desc)
{
  int result = FLASH_DEVICE_DONE;
  int32 bytes_to_send = 0;
  uint32 page_addr = 0, buf_addr = 0;
  uint32 num_shifts = client_ctxt->client_data.page_size_shift;
  uint32 page_size = client_ctxt->client_data.page_size_bytes;
  flash_watchdog_type *watchdog = NULL;

  watchdog = (flash_watchdog_type *)flash_get_property(FLASH_KICK_WATCHDOG_ID);

  if (FLASH_PROPERTY_NOT_DEFINED == (uint32)watchdog)
  {
    FLASHLOG(3,(" Flash: Failed to get watchdog function.\n"));
    return FLASH_DEVICE_FAIL;
  }

  bytes_to_send = (int32)buf_desc->size;
  page_addr = page_num << num_shifts;
  buf_addr = buf_desc->VirtualAddr;

  while ((bytes_to_send > 0) && (result == FLASH_DEVICE_DONE))
  {
    result = nor_spi_write_enable(client_ctxt);

    if (result == FLASH_DEVICE_DONE)
    {
      result = nor_spi_start_page_write(client_ctxt, page_addr, buf_addr,
        page_size);
    }

    if (result == FLASH_DEVICE_DONE)
    {
      /* Poll for Operation Completion and check the error status */
      result = nor_spi_check_status(client_ctxt, NOR_SPI_WRITE_OP);
    }

    bytes_to_send -= page_size;
    buf_addr += page_size;
    page_addr += page_size;
  }

  return result;
}

/*
 * Program partial page data operation for SPI NOR flash device.
 */
static int nor_spi_write_partial_page(flash_client_ctxt *client_ctxt,
  const struct flash_partial_page_op *partial_page_op,
  DALSysMemDescBuf *buf_desc)
{
  int result = FLASH_DEVICE_DONE;
  uint32 phys_page = 0, bytes_to_send = 0;
  uint32 offset = 0, page_addr = 0, buf_addr = 0;
  uint32 num_shifts = client_ctxt->client_data.page_size_shift;
  uint32 max_page_size = client_ctxt->client_data.page_size_bytes;
  flash_watchdog_type *watchdog = NULL;

  watchdog = (flash_watchdog_type *)flash_get_property(FLASH_KICK_WATCHDOG_ID);

  if (FLASH_PROPERTY_NOT_DEFINED == (uint32)watchdog)
  {
    FLASHLOG(3,(" Flash: Failed to get watchdog function.\n"));
    return FLASH_DEVICE_FAIL;
  }

  offset = partial_page_op->byte_offset;
  phys_page = NOR_LP_TO_PP(client_ctxt, partial_page_op->page);
  bytes_to_send = partial_page_op->write_size_bytes;

  /* Check number of bytes to program is within a page's length */
  if ((bytes_to_send + offset) > max_page_size)
  {
    return FLASH_DEVICE_FAIL;
  }
  page_addr = (phys_page << num_shifts) + offset;
  buf_addr = buf_desc->VirtualAddr;

  result = nor_spi_write_enable(client_ctxt);

  if (result == FLASH_DEVICE_DONE)
  {
    result = nor_spi_start_page_write(client_ctxt, page_addr, buf_addr,
      bytes_to_send);
  }

  if (result == FLASH_DEVICE_DONE)
  {
    /* Poll for Operation Completion and check the error status */
    result = nor_spi_check_status(client_ctxt, NOR_SPI_WRITE_OP);
  }

  return result;
}

/*
 * Initiate read page data for SPI NOR flash device.
 */
static int nor_spi_read_page(flash_client_ctxt *client_ctxt, uint32 page_num,
  DALSysMemDescBuf *buf_desc)
{
  int result = FLASH_DEVICE_DONE;
  int32 bytes_to_receive = 0;
  uint32 page_addr = 0, buf_addr = 0, buf_page_cnt = 0;
  uint32 page_size = client_ctxt->client_data.page_size_bytes;
  uint32 num_shifts = client_ctxt->client_data.page_size_shift;
  struct nor_spi_xfer_buf_info spi_read_buf;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  bytes_to_receive = (int32)buf_desc->size;
  page_addr = page_num << num_shifts;
  buf_addr = buf_desc->VirtualAddr;

  /* 'user' parameter denotes total pages this buffer descriptor can hold */
  buf_page_cnt =
    (buf_desc->user < nor_entry_init_data.multi_rw_support_count) ?
    buf_desc->user : nor_entry_init_data.multi_rw_support_count;

  page_size *= buf_page_cnt;

  spi_read_buf.spi_cmd_buf = nor_spi_pdata->cmd_addr_buffer;
  spi_read_buf.cmd_buf_len = NOR_SPI_CMD_LEN + NOR_SPI_ADDR_LEN;

  while ((bytes_to_receive > 0) && (result == FLASH_DEVICE_DONE))
  {
    spi_read_buf.spi_data_buf = (uint8 *)buf_addr;
    spi_read_buf.data_buf_len = page_size;

    spi_read_buf.spi_cmd_buf[0] = NOR_SPI_READ_PAGE_CMD;
    spi_read_buf.spi_cmd_buf[1] = (uint8)((page_addr >> 16) & 0xFF);
    spi_read_buf.spi_cmd_buf[2] = (uint8)((page_addr >> 8) & 0xFF);
    spi_read_buf.spi_cmd_buf[3] = (uint8)((page_addr) & 0xFF);

    nor_spi_read(&spi_read_buf, &result);

    bytes_to_receive -= page_size;
    buf_addr += page_size;
    page_addr += page_size;

    if (bytes_to_receive < page_size)
    {
      page_size = bytes_to_receive;
    }
  }

  return result;
}

/*
 * Retrieve SPI NOR device specific information.
 */
static int nor_spi_get_info (flash_client_ctxt *client_ctxt,
  enum flash_info_type info_type, void *get_data)
{
  int result = FLASH_DEVICE_DONE;

  if (client_ctxt->refs)
  {
    if (FLASH_DEVICE_INFO == info_type)
    {
      struct flash_info *info = (struct flash_info *)get_data;

      /* Copy the device name */
      flash_strcpy((uint8 *)info->device_name,
       (uint8 *)client_ctxt->flash_dev_ctxt->dev_info.common.device_name,32);

      /* Get Device type and width  */
      info->type = client_ctxt->flash_dev_ctxt->dev_info.common.type;
      info->tech = client_ctxt->flash_dev_ctxt->dev_info.common.tech;
      info->width = client_ctxt->flash_dev_ctxt->dev_info.common.width;
      info->block_count =
       client_ctxt->flash_dev_ctxt->dev_info.u.nor.block_count;

      info->write_style =
       client_ctxt->flash_dev_ctxt->dev_info.common.write_style;

      /* Get Device manufacturer/make ID, device ID, version  */
      info->maker_id = client_ctxt->flash_dev_ctxt->dev_info.u.nor.dev_ids[0];
      info->device_id = client_ctxt->flash_dev_ctxt->dev_info.u.nor.dev_ids[1];
      info->version_id =
       client_ctxt->flash_dev_ctxt->dev_info.u.nor.dev_ids[2];

      /* Get Partition specific information  */
      info->partition.ecc_state = FLASH_ECC_NONE;
      info->partition.block_count = client_ctxt->client_data.block_count;
      info->partition.pages_per_block =
       client_ctxt->client_data.pages_per_block;
      info->partition.page_size_bytes =
       client_ctxt->client_data.page_size_bytes;
      info->partition.total_page_size_bytes =
       client_ctxt->client_data.total_page_size_bytes;
      info->partition.max_spare_udata_bytes = 0;
      info->partition.physical_start_block =
       client_ctxt->client_data.partition_block_start;
      info->partition.attributes =
       client_ctxt->client_data.partition_attributes;
      info->partition.reserved1 = 0;
      info->partition.reserved2 = 0;

      /* Get MIBIB block numbers */
      info->mibib_info.old_mibib_block = 0;
      info->mibib_info.new_mibib_block = 0;
    }
    else if (FLASH_NOR_BLOCK_ERASE_CFG_INFO == info_type)
    {
      uint32 cfg_count = 0;
      struct nor_spi_dev_pdata *nor_spi_pdata;
      struct flash_nor_block_erase_cfg_info *erase_data;
      struct flash_nor_block_erase_cfg_info *info =
        (struct flash_nor_block_erase_cfg_info *)get_data;

      nor_spi_pdata = GET_NOR_PDATA(client_ctxt);
      erase_data = &nor_spi_pdata->config_data->erase_cfg_info;
      info->erase_cfg_count = erase_data->erase_cfg_count;

      while (cfg_count < info->erase_cfg_count)
      {
        info->block_cfg[cfg_count].block_count =
          erase_data->block_cfg[cfg_count].block_count;
        info->block_cfg[cfg_count].pages_per_block =
          erase_data->block_cfg[cfg_count].pages_per_block;

        cfg_count++;
      }
    }
    else if(FLASH_NOR_ERASE_REGION_INFO == info_type)
    {
      struct flash_nor_erase_region_info *info =
        (struct flash_nor_erase_region_info *)get_data;

      info->erase_region_count = 1;
      info->erase_region[0].block_count =
        client_ctxt->flash_dev_ctxt->dev_info.u.nor.block_count;
      info->erase_region[0].block_size_in_bytes =
        client_ctxt->client_data.pages_per_block *
        client_ctxt->client_data.page_size_bytes;
    }
  }
  else
  {
   get_data = NULL;
   result = FLASH_DEVICE_FAIL;
  }

  /* Set the driver operation status to debug */
  SET_FLASH_DBG_OP_STATUS(FLASH_DEVICE_DONE);

  return result;
}

/*
 *  Unlock all blocks in SPI NOR flash device.
 */
int nor_spi_unlock_all_blocks(flash_client_ctxt *client_ctxt)
{
  int result = FLASH_DEVICE_DONE;
  uint8 status, mask = 0xFF;
  uint32 wrsr_timeout = NOR_SPI_PROGRAM_TIMEOUT;
  struct nor_spi_xfer_buf_info spi_write_buf;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  result = nor_spi_write_enable(client_ctxt);

  if (result == FLASH_DEVICE_DONE)
  {
    spi_write_buf.spi_cmd_buf = nor_spi_pdata->cmd_addr_buffer;
    spi_write_buf.cmd_buf_len = NOR_SPI_CMD_LEN + NOR_SPI_STATUS_REG_LEN;
    spi_write_buf.spi_data_buf = NULL;
    spi_write_buf.data_buf_len = 0;

    spi_write_buf.spi_cmd_buf[0] = NOR_SPI_WRITE_STATUS_CMD;
    spi_write_buf.spi_cmd_buf[1] = NOR_SPI_GBL_UNPROTECT_BLK;

    nor_spi_write(&spi_write_buf, &result);

    if (result == FLASH_DEVICE_DONE)
    {
      while (wrsr_timeout--)
      {
        /* Read the Status Register and check for Operation Completion */
    result = nor_spi_read_register(client_ctxt, NOR_SPI_READ_STATUS_CMD,
      &status);

        if ((result == FLASH_DEVICE_FAIL) ||
          !(status & NOR_SPI_STATUS_BUSY_MASK))
        {
          break;
        }
      }

      if (!wrsr_timeout)
      {
        result = FLASH_DEVICE_FAIL;
        nor_spi_status_reg = status;
        DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
          "Flash: nsuab - Timeout expired, wrsr update failed. !");
      }
    }

    /* Check device status */
    if (result == FLASH_DEVICE_DONE)
    {
      /* Read the Status Register and verify if all sectors/blocks are
       * unprotected */
      result = nor_spi_read_register(client_ctxt, NOR_SPI_READ_STATUS_CMD,
        &status);

      switch (nor_spi_pdata->config_data->family)
      {
        case FLASH_NOR_SPI_ATMEL:
          mask = NOR_SPI_AT_UNPROTECT_MASK;
          break;
        case FLASH_NOR_SPI_MACRONIX:
          mask = NOR_SPI_MX_UNPROTECT_MASK;
          break;
        case FLASH_NOR_SPI_WINBOND:
          mask = NOR_SPI_WB_UNPROTECT_MASK;
          break;
        case FLASH_NOR_SPI_MICRON:
          mask = NOR_SPI_MI_UNPROTECT_MASK;
          break;
        default:
          mask = 0xFF;
          result = FLASH_DEVICE_FAIL;
          DALSYS_LogEvent(0, DALSYS_LOGEVENT_ERROR,
            "Flash: nsuab - Unknown NOR Family wrsr update failed. !");
      }

      if ((result == FLASH_DEVICE_DONE) && (status & mask))
      {
        result = FLASH_DEVICE_FAIL;
        nor_spi_status_reg = status;
        DALSYS_LogEvent(0, DALSYS_LOGEVENT_ERROR,
          "Flash: nsuab - wrsr update failed!");
      }
    }
  }

  return result;
}

/*
 * Detect the presence of a SPI NOR flash device.
 */
int nor_spi_read_id (flash_client_ctxt *client_ctxt, uint8 *ids)
{
  int result = FLASH_DEVICE_DONE;
  struct nor_spi_xfer_buf_info spi_read_buf;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  spi_read_buf.spi_cmd_buf = nor_spi_pdata->cmd_addr_buffer;
  spi_read_buf.cmd_buf_len = NOR_SPI_CMD_LEN;
  spi_read_buf.spi_data_buf = ids;
  spi_read_buf.data_buf_len = NOR_SPI_ID_LEN;

  /* Read ID */
  spi_read_buf.spi_cmd_buf[0] = NOR_SPI_READDID_CMD;

  nor_spi_read(&spi_read_buf, &result);

  return result;
}/* nor_spi_read_id */

/*
 * Read register status of SPI NOR flash device
 */
static int nor_spi_get_status(flash_client_ctxt *client_ctxt)
{
  int result = FLASH_DEVICE_DONE;
  uint8 status;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  /* Read Status Register to Check for Operation completion */
  result = nor_spi_read_register(client_ctxt, NOR_SPI_READ_STATUS_CMD, &status);

  if (result == FLASH_DEVICE_DONE)
  {
    if (status & NOR_SPI_STATUS_BUSY_MASK)
    {
      result = FLASH_DEVICE_OP_INPROGRESS;
    }
    else
    {
      /* Poll for Operation Completion and check the error status */
      result = nor_spi_check_error_status(client_ctxt, status);

      /* device is in ready state. */
      nor_spi_pdata->erase_in_progress = FALSE;
    }
  }

  return result;
}

/* Enter into Deep Power-Down mode */
static int nor_spi_deep_power_down(flash_client_ctxt *client_ctxt)
{
  int result = FLASH_DEVICE_DONE;

  struct nor_spi_xfer_buf_info spi_write_buf;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);
  struct nor_dev_data *dev_data;
  flash_dev_ctxt *dev_ctxt = client_ctxt->flash_dev_ctxt;

  dev_data = &dev_ctxt->dev_info.u.nor;

  spi_write_buf.spi_cmd_buf = nor_spi_pdata->cmd_addr_buffer;
  spi_write_buf.cmd_buf_len = NOR_SPI_CMD_LEN;
  spi_write_buf.spi_data_buf = NULL;
  spi_write_buf.data_buf_len = 0;
  spi_write_buf.spi_cmd_buf[0] = NOR_SPI_DP_PWR_DWN_CMD;

  nor_spi_write(&spi_write_buf, &result);

  DALSYS_BusyWait(dev_data->dpd_delay_time_us);

  return result;
}

/* Release from Deep Power-Down mode */
int nor_spi_release_deep_power_down(flash_client_ctxt *client_ctxt)
{
  int result = FLASH_DEVICE_DONE;

  struct nor_spi_xfer_buf_info spi_write_buf;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);
  struct nor_dev_data *dev_data;
  flash_dev_ctxt *dev_ctxt = client_ctxt->flash_dev_ctxt;

  dev_data = &dev_ctxt->dev_info.u.nor;

  spi_write_buf.spi_cmd_buf = nor_spi_pdata->cmd_addr_buffer;
  spi_write_buf.cmd_buf_len = NOR_SPI_CMD_LEN;
  spi_write_buf.spi_data_buf = NULL;
  spi_write_buf.data_buf_len = 0;
  spi_write_buf.spi_cmd_buf[0] = NOR_SPI_REL_DP_PWR_DWN_CMD;

  nor_spi_write(&spi_write_buf, &result);

  DALSYS_BusyWait(dev_data->rel_dpd_delay_time_us);

  return result;
}

/*
 * Set the power mode for flash
 */
static int nor_spi_set_power_mode(flash_client_ctxt *client_ctxt,
  enum flash_power_mode power_mode)
{
  int result = FLASH_DEVICE_DONE;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  /* Atmel SPI NOR currently has issues with Deep Power Down support so
   * do nothing for power mode setting for Atmel SPI NOR until issue is
   * resolved
   */
  if (nor_spi_pdata->config_data->family != FLASH_NOR_SPI_ATMEL)
  {
    switch(power_mode)
    {
      case FLASH_POWER_LOW_PWR_MODE:
        result = nor_spi_deep_power_down(client_ctxt);
        break;
      case FLASH_POWER_NORMAL_MODE:
        result = nor_spi_release_deep_power_down(client_ctxt);
        break;
      default:
        result = FLASH_DEVICE_FAIL;
    }
  }
  return result;
}

/*
 * Erase blocks in SPI NOR flash device.
 */
static int nor_spi_erase_blocks(flash_client_ctxt *client_ctxt,
  struct flash_block_vector *block_vector, uint32 iovec_count)
{
  int result = FLASH_DEVICE_DONE;
  uint32 phys_block = 0, tmp_block_count = 0;

  /* Make sure block erase operation is not in progress */
  result = nor_spi_is_block_erase_done(client_ctxt);
  if (result != FLASH_DEVICE_DONE)
  {
    return result;
  }

  (void)nor_spi_erase_block;

  /* For each element in the IOVECTOR */
  while (iovec_count--)
  {
    tmp_block_count = 0;
    phys_block = NOR_LB_TO_PB(client_ctxt, block_vector->start_block);

    /* For the range of blocks specified in the structure */
    while (tmp_block_count < block_vector->block_count)
    {
      /* Erase the logical block */
      block_vector->result_vector[tmp_block_count] =
        nor_spi_erase_block(client_ctxt, phys_block, TRUE);

      /* Set the overall result as failure if one erase fails */
      if (FLASH_DEVICE_DONE != block_vector->result_vector[tmp_block_count])
      {
        result = FLASH_DEVICE_FAIL;
      }
      tmp_block_count++;
      phys_block++;
    }

    block_vector++;
  }

  return result;
}

/*
 * Begin erase operation on a block in SPI NOR flash device.
 */
static int nor_spi_begin_erase_block(flash_client_ctxt *client_ctxt,
  uint32 block)
{
  int result = FLASH_DEVICE_DONE;
  uint32 phys_block;
  struct nor_spi_dev_pdata *nor_spi_pdata = GET_NOR_PDATA(client_ctxt);

  /* Make sure block erase operation is not in progress */
  result = nor_spi_is_block_erase_done(client_ctxt);
  if (result != FLASH_DEVICE_DONE)
  {
    return result;
  }

  (void)nor_spi_erase_block;

  phys_block = NOR_LB_TO_PB(client_ctxt, block);

  /* Set result as failure if issuing erase block cmd fails */
  if (FLASH_DEVICE_DONE != nor_spi_erase_block(client_ctxt, phys_block, FALSE))
  {
    result = FLASH_DEVICE_FAIL;
  }
  else
  {
    /* Set flag to block all other device operations. */
    nor_spi_pdata->erase_in_progress = TRUE;
  }

  return result;
}

/*
 * Initiate erase suspend on SPI NOR flash device
 */
static int nor_spi_erase_suspend(flash_client_ctxt *client_ctxt)
{
  return FLASH_DEVICE_NOT_SUPPORTED;
}

/*
 * Initiate erase resume on SPI NOR flash device
 */
static int nor_spi_erase_resume(flash_client_ctxt *client_ctxt)
{
  return FLASH_DEVICE_NOT_SUPPORTED;
}

/*
 * Writes page_count pages in SPI NOR flash device
 */
static int nor_spi_write_pages(flash_client_ctxt *client_ctxt,
  enum page_write_opcode write_type, dalsys_mem_desc_list *buffer_desc,
  struct flash_page_iovec *page_data_iovec)
{
  int result = FLASH_DEVICE_DONE;
  int32  buf_desc_count, total_bytes_to_send;
  uint32 page_num;
  uint32 num_shifts = client_ctxt->client_data.page_size_shift;
  struct nor_dev_data *dev_data;
  DALSysMemDescBuf *buf_desc;
  flash_dev_ctxt *dev_ctxt = client_ctxt->flash_dev_ctxt;

  /* Make sure block erase operation is not in progress */
  result = nor_spi_is_block_erase_done(client_ctxt);
  if (result != FLASH_DEVICE_DONE)
  {
    return result;
  }

  dev_data = &dev_ctxt->dev_info.u.nor;

  /* Get starting buffer descriptor from buffer desc list */
  buf_desc = DALFW_MemDescBufPtr(buffer_desc, 0);
  buf_desc_count = (int32)buffer_desc->dwNumDescBufs;
  if (buf_desc == NULL)
  {
    result = FLASH_DEVICE_FAIL;
  }

  page_num = NOR_LP_TO_PP(client_ctxt, page_data_iovec->start_page);

  total_bytes_to_send = (int32)(page_data_iovec->total_page_count *
    dev_data->page_size_bytes);

  while ((total_bytes_to_send > 0) && (result == FLASH_DEVICE_DONE))
  {
    result = nor_spi_write_page(client_ctxt, page_num, buf_desc);
    total_bytes_to_send -= buf_desc->size;
    page_num += (buf_desc->size >> num_shifts);

    /* Get next buffer descriptor */
    if (buf_desc_count)
    {
      buf_desc += 2;
      buf_desc_count -= 2;
    }
  }

  return result;
}

/*
 * Writes page_count partial pages in SPI NOR flash device
 */
static int nor_spi_write_partial_pages(flash_client_ctxt *client_ctxt,
  dalsys_mem_desc_list *buffer_desc,
  const struct flash_partial_page_op *partial_page_op)
{
  int result = FLASH_DEVICE_FAIL;
  DALSysMemDescBuf *buf_desc;

  /* Make sure block erase operation is not in progress */
  result = nor_spi_is_block_erase_done(client_ctxt);
  if (result != FLASH_DEVICE_DONE)
  {
    return result;
  }

  /* Get starting buffer descriptor from buffer desc list */
  buf_desc = DALFW_MemDescBufPtr(buffer_desc, 0);
  if ((buffer_desc->dwNumDescBufs > 2) || (buf_desc == NULL))
  {
    result = FLASH_DEVICE_FAIL;
    return result;
  }

  result = nor_spi_write_partial_page(client_ctxt, partial_page_op, buf_desc);

  return result;
}

/*
 * Reads page_count pages in SPI NOR flash device
 */
static int nor_spi_read_pages(flash_client_ctxt *client_ctxt,
  enum page_read_opcode read_type, dalsys_mem_desc_list *buffer_desc,
  struct flash_page_iovec *page_data_iovec)
{
  int result = FLASH_DEVICE_DONE;
  int tmp_result = FLASH_DEVICE_FAIL_PAGE_ERASED;
  int32  buf_desc_count, total_bytes_to_receive;
  uint32 page_num;
  uint32 num_shifts = client_ctxt->client_data.page_size_shift;
  struct nor_dev_data *dev_data;
  DALSysMemDescBuf *buf_desc;
  flash_dev_ctxt *dev_ctxt = client_ctxt->flash_dev_ctxt;

  /* Make sure block erase operation is not in progress */
  result = nor_spi_is_block_erase_done(client_ctxt);
  if (result != FLASH_DEVICE_DONE)
  {
    return result;
  }

  dev_data = &dev_ctxt->dev_info.u.nor;

  /* Get starting buffer descriptor from buffer desc list */
  buf_desc = DALFW_MemDescBufPtr(buffer_desc, 0);
  buf_desc_count = (int32)buffer_desc->dwNumDescBufs;
  if (buf_desc == NULL)
  {
    result = FLASH_DEVICE_FAIL;
  }

  page_num = NOR_LP_TO_PP(client_ctxt, page_data_iovec->start_page);

  total_bytes_to_receive = (int32)(page_data_iovec->total_page_count *
    dev_data->page_size_bytes);

  while ((total_bytes_to_receive > 0) && (result == FLASH_DEVICE_DONE))
  {
    result = nor_spi_read_page(client_ctxt, page_num, buf_desc);

    if ((result == FLASH_DEVICE_DONE) &&
      (tmp_result == FLASH_DEVICE_FAIL_PAGE_ERASED))
    {
      tmp_result = nor_spi_erased_page_check(client_ctxt, buf_desc);
    }
    total_bytes_to_receive -= buf_desc->size;
    page_num += (buf_desc->size >> num_shifts);

    /* Get next buffer descriptor */
    if (buf_desc_count)
    {
      buf_desc += 2;
      buf_desc_count -= 2;
    }
  }

  /* Check erase page status if read pages is ok. */
  if (result == FLASH_DEVICE_DONE)
  {
    result = tmp_result;
  }

  return result;
}

/*
 * Handle get read pointer api in SPI NOR flash device
 */
static int nor_spi_read_pointer(flash_client_ctxt *client_ctxt,
  void **read_addr, uint32 page)
{
  return FLASH_DEVICE_NOT_SUPPORTED;
}

/*
 * Erase blocks of specific size in SPI NOR flash device.
 */
static int nor_spi_erase_blocks_by_size(flash_client_ctxt *client_ctxt,
  enum block_size_opcode block_size_type,
  struct flash_block_vector *block_vector, uint32 iovec_count)
{
  int result = FLASH_DEVICE_DONE;

  /* Call erase blocks API */
  result = nor_spi_erase_blocks(client_ctxt, block_vector, iovec_count);

  return result;
}

/*------------------------------------------------------------------------------
  VTABLE Initialization
 ------------------------------------------------------------------------------*/

flash_vtbl nor_spi_base_vtbl = {
  NULL,                      /* Power event */
  NULL,                      /* Close */
  NULL,                      /* De-init */
  NULL,                      /* Open partition */
  nor_spi_get_info,          /* get_info */
  nor_spi_get_status,        /* Get device status */
  nor_spi_erase_blocks,      /* Erase blocks */
  nor_spi_begin_erase_block, /* begin erase block */
  nor_spi_erase_suspend,     /* erase_suspend */
  nor_spi_erase_resume,      /* erase_resume */
  NULL,                      /* Set block state */
  NULL,                      /* Get block state */
  nor_spi_write_pages,       /* Write pages */
  nor_spi_write_partial_pages, /* write_partial_pages */
  NULL,                      /* write_2x_pages */
  nor_spi_read_pages,        /* read pages */
  nor_spi_read_pointer,      /* read_pointer */
  NULL,                      /* Copy pages */
  NULL,                      /* Get UUID */
  nor_spi_erase_blocks_by_size, /* erase blocks by size */
  nor_spi_set_power_mode     /* Set power mode */
};

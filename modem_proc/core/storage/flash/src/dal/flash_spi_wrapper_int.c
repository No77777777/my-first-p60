/*====================================================================
 *
 * FILE:        flash_spi_wrapper_int.c
 *
 * SERVICES:    Interrupt mode SPI support for flash drivers
 *
 * DESCRIPTION: SPI support for all flash drivers
 *
 * PUBLIC CLASSES:  Not Applicable
 *
 * INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 *
 * Copyright (c) 2010-2013 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * QUALCOMM Confidential and Proprietary
 *==================================================================*/

/*===================================================================
 *
 *                       EDIT HISTORY FOR FILE
 *
 *   This section contains comments describing changes made to the
 *   module. Notice that changes are listed in reverse chronological
 *   order.
 *
 *  $Header: //components/rel/core.mpss/10.0/storage/flash/src/dal/flash_spi_wrapper_int.c#1 $ 
 *  $DateTime: 2019/04/24 00:03:26 $ 
 *  $Author: pwbldsvc $
 *
 *
 * when         who     what, where, why
 * ----------   ---     ----------------------------------------------
 * 05/03/13     eo      Add flash ops mechanism to enable/disable QPIC clocks
 * 10/16/12     eo      Fix initialization of SPI device configuration 
 * 10/08/12     eo      Fix SPI NOR compile warnings 
 * 08/21/12     sb      DAL SPI dependency is only for MPSS
 * 04/05/12     sv      check and do spi open/close for each transfer
 * 01/16/12     eo      Add open/close of spi handle support 
 * 01/03/12     sv      Adapt to flash profiling changes
 * 10/06/11     sv      SPI NOR command log support
 * 08/25/11     sv      SPI NOR support for 9x15
 * 07/20/11     sv      Virtual to Physical addr conversion changes
 * 03/10/11     eo      Fix issue in event timeout
 * 02/10/11     eo      Remove workaround code forcing block mode transfers   
 * 02/01/11     eo      Add Maxcronix SPI NOR device support
 * 09/02/10     sv      NOR tools support
 * 08/11/10     nr      Added stats framework support
 * 06/29/10     eo      Add Babylon support
 * 06/23/10     eo      Support SPI DAL Device ID configuration
 * 04/06/10     sv      Initial Revision
 *==================================================================*/

/*===================================================================
 *
 *                     Include Files
 *
 ====================================================================*/
#include "ddispi.h"
   
#include "flash_nor_spi.h"
#include "flash_nor_config.h"
#include "flash_spi_wrapper.h"
#include "flash_dal_util.h"

#define  FLASH_SPI_EVENT_TIMEOUT_US 3000000 /* 3 sec */

/* Minimun Transfer length supported by DAL SPI Driver */
#define SPI_MIN_TX_LEN 4

/* SPI_NOR_FREQ_HZ can be defined in the .builds file if any
 * other frequency other than the default(20MHz) is required.
 */
#ifndef SPI_NOR_FREQ_HZ
  #define SPI_NOR_FREQ_HZ  26000000
#endif

/* Flash SPI memory information  */
struct flash_spi_meminfo
{
    DALSYSMemHandle  handle_spi_mem;   /* SPI Workspace mem handle */
    DALSYSMemInfo    spi_mem_info;     /* SPI Workspace mem info */
};

/* Flash SPI Interface properties */
struct flash_spi_props
{
  DALSYSEventHandle transfer_done_event[2];  /* Transfer done event */
  SpiTransferType spi_transfer_info;         /* SPI xfer input/output info */
  uint8 *transfer_buf;                       /* Buffer used for spi xfers */
  struct flash_spi_meminfo mem_info;         /* SPI memory allocation info */
};

/* SPI transfer buffer information */
struct flash_spi_xfer_buf_info
{
  SpiDataAddrType tx_addr;
  SpiDataAddrType rx_addr;
  uint32 tx_buf_len;
  uint32 rx_buf_len;
};

/* SPI device handle */
DalDeviceHandle *spi_handle = NULL;

/* used to determine if cache maintenance is required */
extern struct nor_entry_data nor_entry_init_data;

/* Global variable for flash SPI properties */
struct flash_spi_props flash_spi_info = {{0}};

/* Global to store SPI NOR Debug info */
struct flash_nor_spi_dbg_info nor_spi_dbg_info;

/* Counter for flash_spi_dev_open */
static uint32 flash_spi_dev_open_counter = 0;

/* SPI bus device configurations */
struct flash_spi_core_config nor_spi_device_cfg =
{
  {SPI_CLK_NORMAL,
   SPI_CLK_IDLE_HIGH,     /* Same as NOR tools */
   SPI_OUTPUT_FIRST_MODE, /* Same as NOR tools */
   0,                    /* De-assertion time */
   0,                    /* Min slave freq (Hz) */
   SPI_NOR_FREQ_HZ,      /* Max slave freq (Hz) */
   SPI_CS_ACTIVE_LOW,
   SPI_CS_KEEP_ASSERTED
  },
  {0,                    /* Chip Select Slave uses */
   SPI_CORE_MODE_MASTER
  },
  {8,                    /* N: number of bits tranferred in 1 SPI xfer */
   SPI_LOOPBACK_DISABLED,
   SPI_INPUT_PACKING_DISABLED,
   SPI_OUTPUT_UNPACKING_DISABLED,
   0                     /* Slave time out in usec */
  }
};

/* SPI Bus get master device settings */
static void flash_spi_get_master_setting(SpiDeviceInfoType *spi_cfg_info,
struct flash_spi_core_config *spi_cfg)
{
  SpiDeviceParametersType *dev_param;
  SpiDeviceBoardInfoType *dev_board_info;
  SpiTransferParametersType *dev_xfer_param;
  
  dev_param = (SpiDeviceParametersType *)&spi_cfg_info->deviceParameters;
  dev_board_info = (SpiDeviceBoardInfoType *)&spi_cfg_info->deviceBoardInfo;
  dev_xfer_param = (SpiTransferParametersType *)&spi_cfg_info->transferParameters;

  /* Initialize SPI device parameters */
  dev_param->eClockMode = 
    (SpiClockModeType)spi_cfg->dev_param.clock_mode;
  dev_param->eClockPolarity = 
    (SpiClockPolarityType)spi_cfg->dev_param.clock_polarity;
  dev_param->eShiftMode = 
    (SpiShiftModeType)spi_cfg->dev_param.shift_mode;
  dev_param->u32DeassertionTime = 
    spi_cfg->dev_param.cs_deassert_time;
  dev_param->u32MinSlaveFrequencyHz = 
    spi_cfg->dev_param.min_slave_freq_hz;
  dev_param->u32MaxSlaveFrequencyHz = 
    spi_cfg->dev_param.max_slave_freq_hz;
  dev_param->eCSPolarity = 
    (SpiCSPolarityType)spi_cfg->dev_param.cs_polarity;
  dev_param->eCSMode = 
    (SpiCSModeType)spi_cfg->dev_param.cs_mode;

  /* Initialize SPI device board information */
  dev_board_info->nSlaveNumber = 
    spi_cfg->dev_board.cs_num;
  dev_board_info->eCoreMode = 
    (SpiCoreModeType)spi_cfg->dev_board.core_mode;

  /* Initialize SPI device transfer parameters */
  dev_xfer_param->nNumBits = 
    spi_cfg->dev_transfer.num_bits;
  dev_xfer_param->eLoopbackMode = 
    (SpiLoopbackModeType)spi_cfg->dev_transfer.loopback_mode;
  dev_xfer_param->eInputPacking = 
    (SpiInputPackingType)spi_cfg->dev_transfer.input_packing;
  dev_xfer_param->eOutputUnpacking = 
    (SpiOutputUnpackingType)spi_cfg->dev_transfer.output_unpacking;
  dev_xfer_param->eTransferMode = SPI_TRANSFER_MODE_DEFAULT;
  dev_xfer_param->slaveTimeoutUs = 
    spi_cfg->dev_transfer.slave_timeout_us;
   
} // flash_spi_get_master_setting

/* Function to configure the SPI device */
static int flash_spi_dev_config(void)
{
  int result = FLASH_DEVICE_FAIL;
  int tmp_result = DAL_ERROR;
  SpiDeviceInfoType spi_config_info;

  flash_spi_get_master_setting(&spi_config_info, &nor_spi_device_cfg);
  tmp_result = DalSpi_ConfigureDevice(spi_handle, &spi_config_info);

  /* Set the channel mode */
  if(DAL_SUCCESS == tmp_result)
  {
    result = FLASH_DEVICE_DONE;
  }

  return result;
}

/*  This is a SPI transfer wrapper for SPI driver. It transfers
 *  the flash command and data to the Flash device through the 
 *  SPI Interface
 */
static int flash_spi_transfer(SpiTransferType *spi_transfer_info,
  struct flash_spi_xfer_buf_info *spi_buf_info)
{
  int result = FLASH_DEVICE_DONE;
  int inp_data_len = 0, tmp_result = DAL_ERROR;
  uint32 tx_id = 0;
  uint32 event_id = 0;
  uint32 spi_cmd_log_indx = 0;
  uint32 spi_cmd_log_sz = 0;

  FLASH_CORE_PROFILE_START(FLASH_PROFILE_BUS_TRANSFER, \
    (spi_buf_info->tx_buf_len + spi_buf_info->rx_buf_len));

  flash_spi_dev_open();
  
  /* Giving ownership of the events to the caller thread */
  DALSYS_EventCtrl(flash_spi_info.transfer_done_event[0], 
    DALSYS_EVENT_CTRL_RESET);

  DALSYS_EventCtrl(flash_spi_info.transfer_done_event[1], 
    DALSYS_EVENT_CTRL_RESET);

  spi_transfer_info->u32NumOutputTransfers = spi_buf_info->tx_buf_len;
  spi_transfer_info->u32NumInputTransfers = spi_buf_info->rx_buf_len;

  if(spi_buf_info->tx_buf_len >= NOR_SPI_CMD_LOG_BUF_SIZE)
  {
    spi_cmd_log_sz = NOR_SPI_CMD_LOG_BUF_SIZE;
  }
  else
  {
    spi_cmd_log_sz = spi_buf_info->tx_buf_len;
  }	   
 
  spi_cmd_log_indx = nor_spi_dbg_info.nor_spi_next_cmd_log_indx;
  DALSYS_memset(nor_spi_dbg_info.nor_spi_cmd_log[spi_cmd_log_indx].spi_cmd_buf,
    0, NOR_SPI_CMD_LOG_BUF_SIZE);
  DALSYS_memcpy(nor_spi_dbg_info.nor_spi_cmd_log[spi_cmd_log_indx].spi_cmd_buf,
   (uint8 *)spi_buf_info->tx_addr.virtualAddr, spi_cmd_log_sz);
    
  nor_spi_dbg_info.nor_spi_cmd_log[spi_cmd_log_indx].spi_total_tx_bytes =
    spi_buf_info->tx_buf_len;

  nor_spi_dbg_info.nor_spi_next_cmd_log_indx++;
  if (nor_spi_dbg_info.nor_spi_next_cmd_log_indx >= NOR_SPI_MAX_CMD_LOG_ENTRY)
  {
    nor_spi_dbg_info.nor_spi_next_cmd_log_indx = 0;
  }    

  tmp_result = DalSpi_StartTransfer(spi_handle, spi_transfer_info, 
    &spi_buf_info->tx_addr, spi_buf_info->tx_buf_len, &spi_buf_info->rx_addr, 
    spi_buf_info->rx_buf_len, &inp_data_len, &tx_id);

  if (tmp_result == SPI_QUEUED)
  {
      /* Wait for the transfer done event */
    if (DALSYS_EventMultipleWait(flash_spi_info.transfer_done_event, 2, 
        FLASH_SPI_EVENT_TIMEOUT_US, &event_id)!= DAL_SUCCESS)
    {
      result = FLASH_DEVICE_FAIL;
    }
  }
  else
  {
    result = FLASH_DEVICE_FAIL;
  }

  /* Error fatal, if the SPI transfer is failed */
  if (FLASH_DEVICE_DONE != result)
  {
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
                        "Flash: SPI Transfer failed!");
  }
  
  flash_spi_dev_close();
  
  FLASH_CORE_PROFILE_END(FLASH_PROFILE_BUS_TRANSFER);

  return result;
}

/* This function initializes SPI specific data structures
              The following actions are performed:
                1.  Attach the SPI device
                2.  Initialize flash SPI transfer request.
*/
int flash_spi_init(flash_client_ctxt *client_ctxt)
{
  uint32 spi_device_id = 0;
  uint32 alloc_attr;
  uint32 spi_mem_size, temp_addr;
  struct flash_spi_meminfo *spi_mem = &flash_spi_info.mem_info; 
  SpiTransferType *spi_transfer_info = &flash_spi_info.spi_transfer_info;

  spi_device_id = flash_get_property(FLASH_SPI_DAL_DEVICE_ID);

  if (FLASH_PROPERTY_NOT_DEFINED == spi_device_id)
  {
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
      "Flash: Error SPI DAL device id not defined!");
    
    return FLASH_DEVICE_FAIL;
  }

  DALSYS_InitMod(NULL);

  if (spi_handle == NULL)
  {
    /* Attach and open the SPI device */
    if(DAL_SUCCESS != DAL_DeviceAttach(spi_device_id, &spi_handle))
    {
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Error attaching to SPI device driver!");

      return FLASH_DEVICE_FAIL;
    }
  }

  /* Create DAL event for transfer done */
  if(DAL_SUCCESS != DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT,
    &flash_spi_info.transfer_done_event[0] , NULL))
  {
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
      "Flash: Failed to create the SPI done event object!");
    return FLASH_DEVICE_FAIL;
  }

  /* Create DAL event for transfer timeout */
  if(DAL_SUCCESS != DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT | 
    DALSYS_EVENT_ATTR_TIMEOUT_EVENT, &flash_spi_info.transfer_done_event[1] ,
    NULL))
  {
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
      "Flash: Failed to create the SPI timeout event object!");
    return FLASH_DEVICE_FAIL;
  }

  /* Flash SPI Transfer info initialization */
  spi_transfer_info->dalSysEvent = flash_spi_info.transfer_done_event[0];
  spi_transfer_info->queueIfBusy = TRUE;
  spi_transfer_info->transferMode = SPI_TRANSFER_MODE_DEFAULT;
  spi_transfer_info->u32Priority = 0;

  /* Get the driver workspace memory size from property table */
  spi_mem_size = flash_get_property(FLASH_SPI_WS_BUFF_SIZE_ID);

  if (FLASH_PROPERTY_NOT_DEFINED == spi_mem_size)
  {
    FLASHLOG(2, ("Flash: Get SPIWS Memory property failed!\n"));
    return FLASH_DEVICE_FAIL;
  }

  /* Set the allocation attribute to allocate uncached - physically contiguous
   * region
   */
  alloc_attr = DALSYS_MEM_PROPS_PHYS_CONT | DALSYS_MEM_PROPS_UNCACHED;

  /* Workspace Memory for SPI WRAPPER usage */
  if (DAL_SUCCESS != DALSYS_MemRegionAlloc(alloc_attr,
    DALSYS_MEM_ADDR_NOT_SPECIFIED, DALSYS_MEM_ADDR_NOT_SPECIFIED,
    spi_mem_size, &spi_mem->handle_spi_mem, NULL))
  {
    FLASHLOG(2, ("Flash: DRVWS Memory allocation failed!\n"));
    return FLASH_DEVICE_FAIL;
  }
  else
  {
    /* If the allocation is success, get the memory address
     * information */
    DALSYS_MemInfo(spi_mem->handle_spi_mem, &spi_mem->spi_mem_info);

    /* Add SPI memory details to the virtual
     * to physical conversion list */
    flash_vtop_add_item(
      spi_mem->spi_mem_info.VirtualAddr,
      spi_mem->spi_mem_info.PhysicalAddr,
      spi_mem->spi_mem_info.dwLen);

    temp_addr = FLASH_ALIGN_32(spi_mem->spi_mem_info.VirtualAddr);

    /* Data buffer used for SPI input/output transfers */
    flash_spi_info.transfer_buf = (uint8 *)temp_addr;
  }

  /* Initialize SPI NOR debug info structure */
  DALSYS_memset((void *)&nor_spi_dbg_info, 0, sizeof(nor_spi_dbg_info));

  return FLASH_DEVICE_DONE;
}

/*  This function detaches the spi handle.
 */
int flash_spi_close(void)
{
  int result = FLASH_DEVICE_DONE;
    /* Close SPI Device handle for now */
  DalDevice_Close(spi_handle);

  /* Dettach the SPI device */
  DAL_DeviceDetach(spi_handle);

  if (flash_spi_info.mem_info.handle_spi_mem != 0)
  {
    /* Free memory allocated by the driver */
    if (DAL_SUCCESS != DALSYS_DestroyObject(
      (DALSYSObjHandle) flash_spi_info.mem_info.handle_spi_mem))
    {
      result = FLASH_DEVICE_FAIL;
    }
    else
    {        
      flash_spi_info.mem_info.handle_spi_mem = 0;
    }
  }    

  if (result == FLASH_DEVICE_DONE)
  {
    /* Release the handle for transfer done event */
    if (DAL_SUCCESS != DALSYS_DestroyObject(
      (DALSYSObjHandle)&flash_spi_info.transfer_done_event[0]))
    {
      result = FLASH_DEVICE_FAIL;
    }
  }

  if (result == FLASH_DEVICE_DONE)
  {
    /* Release the handle for transfer timeout event */
    if (DAL_SUCCESS != DALSYS_DestroyObject(
      (DALSYSObjHandle)&flash_spi_info.transfer_done_event[1]))
    {
      result = FLASH_DEVICE_FAIL;
    }
  }
  
  /* Reset the counter for flash_spi_dev_open */
  flash_spi_dev_open_counter = 0;
  
  return result;
}

/*  This is SPI read wrapper for SPI driver. This is used to read data from a   
 *  device connected through SPI Interface
 */
void flash_spi_read_op(struct nor_spi_xfer_buf_info *flash_buf_info, int *status)
{
  SpiTransferType *spi_transfer_info;
  struct flash_spi_xfer_buf_info spi_buf_info;

  spi_buf_info.tx_addr.physicalAddr = 
    (void *)flash_vtop_get_physical((uint32)flash_buf_info->spi_cmd_buf);
  spi_buf_info.tx_addr.virtualAddr = flash_buf_info->spi_cmd_buf;
  spi_buf_info.tx_buf_len = flash_buf_info->cmd_buf_len;

  spi_buf_info.rx_addr.physicalAddr = 
    (void *)flash_vtop_get_physical((uint32)flash_spi_info.transfer_buf);
  spi_buf_info.rx_addr.virtualAddr = flash_spi_info.transfer_buf;
  spi_buf_info.rx_buf_len = flash_buf_info->cmd_buf_len + flash_buf_info->data_buf_len;    

  spi_transfer_info = (SpiTransferType *)&flash_spi_info.spi_transfer_info;

  *status = flash_spi_transfer(spi_transfer_info, &spi_buf_info);

  DALSYS_memcpy(flash_buf_info->spi_data_buf, 
    &flash_spi_info.transfer_buf[flash_buf_info->cmd_buf_len], 
    flash_buf_info->data_buf_len);
 
}

/*  This is SPI write wrapper for SPI driver. This is used to write data to a   
 *  device connected through SPI Interface
 */
void flash_spi_write_op(struct nor_spi_xfer_buf_info *flash_buf_info, int *status)
{
  SpiTransferType *spi_transfer_info;
  struct flash_spi_xfer_buf_info spi_buf_info;

  DALSYS_memcpy(&flash_spi_info.transfer_buf[0], 
    flash_buf_info->spi_cmd_buf, flash_buf_info->cmd_buf_len);

  DALSYS_memcpy(&flash_spi_info.transfer_buf[flash_buf_info->cmd_buf_len], 
    flash_buf_info->spi_data_buf, flash_buf_info->data_buf_len);

  spi_buf_info.tx_addr.physicalAddr = 
    (void *)flash_vtop_get_physical((uint32)flash_spi_info.transfer_buf);
  spi_buf_info.tx_addr.virtualAddr = flash_spi_info.transfer_buf; 
  spi_buf_info.tx_buf_len = 
    flash_buf_info->cmd_buf_len + flash_buf_info->data_buf_len;
  
  spi_buf_info.rx_addr.physicalAddr = NULL;
  spi_buf_info.rx_addr.virtualAddr = NULL;
  spi_buf_info.rx_buf_len = 0;

  spi_transfer_info = (SpiTransferType *)&flash_spi_info.spi_transfer_info;

  *status = flash_spi_transfer(spi_transfer_info, &spi_buf_info);
}

/* Attach to SPI device  */
DalDeviceHandle *flash_spi_dev_init(void)
{
  uint32 spi_device_id = 0;

  spi_device_id = flash_get_property(FLASH_SPI_DAL_DEVICE_ID);

  if (FLASH_PROPERTY_NOT_DEFINED == spi_device_id)
  {
    DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
      "Flash: Error SPI DAL device id not defined!");
  }

  DALSYS_InitMod(NULL);

  if (spi_handle == NULL)
  {
    /* Attach and open the SPI device */
    if(DAL_SUCCESS != DAL_DeviceAttach(spi_device_id, &spi_handle))
    {
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Error attaching to SPI device driver!");
    
      spi_handle = NULL;
    }
  }

  return spi_handle;
}

/* Open the SPI device handle */
void flash_spi_dev_open(void)
{
  if (flash_spi_dev_open_counter == 0)
  {
    /* Open the handle */
    if (DAL_SUCCESS != DalDevice_Open(spi_handle, DAL_OPEN_SHARED))
    {
      /* Error Fatal */
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Failed to open Dev SPI!");
    }
  }
  flash_spi_dev_open_counter++;
}

/* Close the SPI device handle */
void flash_spi_dev_close(void)
{
  flash_spi_dev_open_counter--;
  
  if (flash_spi_dev_open_counter == 0)
  {
    /* Close SPI Device handle */
    if (DAL_SUCCESS != DalDevice_Close(spi_handle))
    {
      /* Error Fatal */
      DALSYS_LogEvent(0, DALSYS_LOGEVENT_FATAL_ERROR,
        "Flash: Failed to close Dev SPI!");
    }
  }
}

/*
 * Init/Configure the SPI wrapper APIs
 */
void flash_spi_configure(void *spi_configs)
{
  struct nor_spi_wrapper *wrapper = (struct nor_spi_wrapper *)spi_configs;

  if (NULL != wrapper)
  {
    wrapper->init = flash_spi_init;
    wrapper->close = flash_spi_close;
    wrapper->dev_cfg = flash_spi_dev_config;
    wrapper->read_op = flash_spi_read_op;
    wrapper->write_op = flash_spi_write_op;
  }
}

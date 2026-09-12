/*
===========================================================================

FILE:         ipa_emu_mgr.c

$Header:

===========================================================================

===========================================================================
Copyright © 2017-2018 Qualcomm Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
===========================================================================
*/

/* -----------------------------------------------------------------------
**                           INCLUDES
** ----------------------------------------------------------------------- */

#include <stdarg.h>
#include "ipa_emu_mgr.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include <assert.h>
#include "windows.h"
#include "VVDRV_deo_rpb.h"
#include "VVDRV_deo_debug.h"
#include "VVDRV_deo_init.h"
#include "VVDRV_deo_interrupt.h"
#include "HALhwio_ipa.h"
#include "msmhwiobase_gen_emu.h"
#include "msmhwioreg_gen_emu.h"

/* -----------------------------------------------------------------------
**                           Constants and Macros
** ----------------------------------------------------------------------- */

#define CLASS_ETHERNET (0x20000)
#define IPA_EMU_MGR_NULL (0)
#define IPA_EMU_MGR_MEMORY_SIZE (32 * 1024 * 1024)
#define IPA_EMU_MGR_SRAM_SIZE (8 * 0x1000)

/* -----------------------------------------------------------------------
**                           DATA
** ----------------------------------------------------------------------- */
ipa_emu_mgr_s ipa_emu_mgr = { /* ipaPcDmaPool */
                              DEO_MEMPOOL_DMA_POOL_DEFAULT, 
                              /* ipaGeImemDmaPool */
                              DEO_MEMPOOL_DMA_POOL_DEFAULT,
                              /* ipa_emu_mgr_base_addr */
                              IPA_EMU_MGR_NULL, 
                              /* ipa_emu_mgr_virt_to_phys_addr_diff */
                              IPA_EMU_MGR_NULL,
                              /* ipa_emu_mgr_rci_init_done*/ 
                              0,
                              /* ipa_emu_mgr_dl_cnsmr_init_done*/
                              0,
                              /* ipa_emu_mgr_kill_timer */
                              0
							              };

/* -----------------------------------------------------------------------
**                           EXTERNS
** ----------------------------------------------------------------------- */

/*
* IPA HAL data
*/
extern uint32 bamPhysBaseAddress;
extern uint32 bamVirtBaseAddress;
extern uint32 bamIntNum;
extern uint32 gsiPhysBaseAddress;
extern uint32 gsiVirtBaseAddress;
extern uint32 gsiIntNum;

/*
* C++ wrapper
*/
#ifdef __cplusplus
extern "C" {
#endif

  _CRTIMP int __cdecl kbhit(void);

#ifdef __cplusplus
}
#endif

/**
* @brief   check actual memory area size
*
* @param [in]   addr  The virtual address to the beginning of the memory area
* @param [in]   size  The size.
* @return             The actual size
*/
static uint32 ipa_emu_mgr_check_memory_area_size(void *addr, uint32 size)
{
  const uint32   minMemAddrGranularity = 1024 * 16; /* It could be also 1,
                                                    but 16k is the optimum value
                                                    for Veloce */
  const uint32   initialMagicPattern = 0x11111111;
  uint32*        currentAddr = (uint32*)addr;
  const uint32*  firstAddr = currentAddr;
  uint32         magicPattern = initialMagicPattern;

  DEO_ASSERT((NULL != addr), (
    "ipa_emu_mgr_check_memory_area_size: Null pointer was passed.\n"));

  do
  {
    memcpy(currentAddr, &magicPattern, sizeof(uint32));

    if (*currentAddr != magicPattern)
    {
      /* The magic pattern is wrong */
      break;
    }

    if (*firstAddr != initialMagicPattern)
    {
      /* One of the address bit was truncated */
      break;
    }
    currentAddr += minMemAddrGranularity;
    magicPattern++;
  } while (currentAddr < firstAddr + size / sizeof(uint32)); /* Exit if the
                                                             maximum size was
                                                             exceeded */

  return (uint32)((size_t)(currentAddr - firstAddr) * sizeof(uint32));
}

/**
* Function name:  ipa_emu_mgr_init
* Description:    Initializes deo manager services
*
* Return value: 0 - success, otherwise - failure
* Scope:        global
**/
void ipa_emu_mgr_init(void)
{

  int rc;
  deo_cfg_t cfg;
  struct pcie_translate_registers_s pcie_translate_cfg;
  deo_interrupt_controller_init_t ic_cfg;
  deo_rpb_info_t *pcie_config = NULL;
  deo_mempool_args_t pc_mempool_cfg = { 0 };
  deo_mempool_args_t mempool_cfg = { 0 };
  uint32  actualMemAreaSize = 0;

  int *ipa_emu_mgr_temp;
  void *ipa_emu_mgr_temp_phys;
  deo_dma_handle_t ipa_emu_mgr_temp_handle = { 0 };

  pcie_translate_cfg.P2A_arguments[0].setRegister = TRUE;
  pcie_translate_cfg.P2A_arguments[0].barIndex = 0;
  pcie_translate_cfg.P2A_arguments[0].ambaAddress =
    (void*)((size_t)(HAL_IPA_GEN_EMU_AXI_BASE));
  pcie_translate_cfg.A2P_arguments[0].setRegister = TRUE;
  pcie_translate_cfg.A2P_arguments[0].pcieAmbaAddress =
    (void*)((size_t)(HAL_IPA_GEN_EMU_PCIE_AMBA_BASE));
  cfg.pcie_translate_cfg = (void*)&pcie_translate_cfg;

  cfg.init_dma_mempool = FALSE; /* Do not init the DMA memory pool */
  cfg.interrupt_controller_cfg = NULL;

  rc = VVDRV_deo_init(&cfg);
  DEO_ASSERT((rc == DEO_SUCCESS), (
    "ipa_emu_mgr_init: deo_init failed, ret= %d\n", rc));

  pc_mempool_cfg.dma_vbuf = cfg.mempool_cfg.dma_vbuf;
  pc_mempool_cfg.dma_pbuf = cfg.mempool_cfg.dma_pbuf;
  pc_mempool_cfg.dma_buf_size = cfg.mempool_cfg.dma_buf_size;

  pcie_config = (deo_rpb_info_t *)cfg.pcie_cfg;

    mempool_cfg.dma_vbuf = (void*)((size_t)pcie_config->barAddr[0] +
      0x0c000000);
    mempool_cfg.dma_pbuf = (void*)(0xfc000000);
    mempool_cfg.dma_buf_size = IPA_EMU_MGR_MEMORY_SIZE; /* The internal memory size*/
    mempool_cfg.mem_buf = (void*)((uint32)mempool_cfg.dma_vbuf +
      mempool_cfg.dma_buf_size);
    mempool_cfg.mem_buf_size = IPA_EMU_MGR_MEMORY_SIZE;

    actualMemAreaSize = ipa_emu_mgr_check_memory_area_size(mempool_cfg.dma_vbuf,
      mempool_cfg.dma_buf_size);
    DEO_ASSERT((actualMemAreaSize == mempool_cfg.dma_buf_size), (
      "ipa_emu_mgr_init: Failed to initialize the memory pool."
      " A wrong memory pool size was specified\n"));

    rc = VVDRV_deo_mem_init(&mempool_cfg);
    DEO_ASSERT((rc == DEO_SUCCESS), (
      "ipa_emu_mgr_init: Failed to initialize the memory pool, ret= %d\n", rc));

    /* Init PC mem pool as secondary */
    rc = VVDRV_deo_dma_pool_create(&(ipa_emu_mgr.ipaPcDmaPool));
    DEO_ASSERT((rc == DEO_SUCCESS), (
      "ipa_emu_mgr_init: Unable to create host DMA pool, ret= %d\n", rc));

    rc = VVDRV_deo_dma_pool_add_block(ipa_emu_mgr.ipaPcDmaPool,
      pc_mempool_cfg.dma_buf_size,
      pc_mempool_cfg.dma_vbuf,
      pc_mempool_cfg.dma_pbuf,
      M_ZERO);
    DEO_ASSERT((rc == DEO_SUCCESS), (
      "ipa_emu_mgr_init: Unable to add block to host DMA pool, ret= %d\n", rc));
 
  switch (pcie_config->classCode)
  {
    case CLASS_ETHERNET:
      switch (pcie_config->idVendorDevice)
      {
        /* Check for COPSS */
        case VENDOR_DEVICE_COPSS:
			ic_cfg.base_address = (void*)((size_t)GE_INT_CTL_REG_BASE_PHYS + 
				((size_t)pcie_config->barAddr[0] - (size_t)GEN_EMU_BASE_PHYS));
			cfg.interrupt_controller_cfg = (void*)&ic_cfg;
			break;
          /* Check for R3PC */
        case VENDOR_DEVICE_SNOWBUSH:
        default:
          DEO_ASSERT((0), (
            "ipa_emu_mgr_init: Device 0x%x is not supported, ret= %d\n",
            pcie_config->idVendorDevice, DEO_ERROR));
      }
      break;
    default:
      DEO_ASSERT((0), (
        "ipa_emu_mgr_init: Device 0x%x is not supported, ret= %d\n",
        pcie_config->classCode, DEO_ERROR));
  }

  if (NULL != cfg.interrupt_controller_cfg)
  {
	  rc = VVDRV_deo_interrupt_init(&ic_cfg);
	  DEO_ASSERT(DEO_SUCCESS == rc , (
		  "ipa_emu_mgr_init: VVDRV_deo_interrupt_init failed, ret= %d\n", rc));
  }

  /* DEO init finished - init EMU MGR defines and constants */

  rc = VVDRV_deo_dma_pool_alloc(DEO_MEMPOOL_DMA_POOL_DEFAULT, sizeof(int), 0,
    &ipa_emu_mgr_temp, &ipa_emu_mgr_temp_phys, M_ZERO, &ipa_emu_mgr_temp_handle);
  DEO_ASSERT((rc == DEO_SUCCESS), (
    "ipa_emu_mgr_init: deo_dma_pool_alloc failed, ret= %d\n", rc));

  ipa_emu_mgr.ipa_emu_mgr_virt_to_phys_addr_diff = 
                                                (uint32)ipa_emu_mgr_temp_phys -
                                                (uint32)ipa_emu_mgr_temp;
  
  VVDRV_deo_dma_free(ipa_emu_mgr_temp_handle);

  ipa_emu_mgr.ipa_emu_mgr_base_addr = (uint32)pcie_config->barAddr[0] +
    HAL_IPA_GEN_EMU_IPA_WRAPPER;

  /*Zero out SRAM */
  memset((void*)((uint32)ipa_emu_mgr.ipa_emu_mgr_base_addr
    + IPA_RAM_REG_BASE_OFFS),
    0, IPA_EMU_MGR_SRAM_SIZE);

}

/**
* Function name:  ipa_emu_mgr_uninit
* Description:    Un-initializes deo services
* Parameters:   None
*
* Return value: None
* Scope:        global
**/
void ipa_emu_mgr_uninit(void)
{
	ipa_emu_mgr.ipa_emu_mgr_kill_timer = 1;
	VVDRV_deo_uninit();
}


/* ISR WRAPPERS */

void ipa_emu_mgr_gsi_isr_wrapper(uint32 irq, void *user_data)
{
  gsi_core_isr(user_data);
}

void ipa_emu_mgr_ipa_isr_wrapper(uint32 irq, void *user_data)
{
  ipa_hal_isr_callback(user_data);
}

/* MEMORY MANAGMENT FUNCTIONS */

void* ipa_emu_mgr_alloc(uint32 size)
{
	void* ret = VVDRV_deo_alloc(size, M_ZERO);
	DEO_ASSERT((ret != NULL), (
		"ipa_emu_mgr_alloc: alloc failed %d\n"));
	return ret;
}

void ipa_emu_mgr_free(void* ptr)
{
  ASSERT(NULL != ptr);
  VVDRV_deo_free(ptr);
}

void* ipa_emu_mgr_alloc_align(
  uint32                  size,
  uint32                  align,
  deo_dma_handle_t       *handle
  )
{
  int ret = 0;
  void **vaddr = NULL, **paddr = NULL;
  ret = VVDRV_deo_dma_pool_alloc(DEO_MEMPOOL_DMA_POOL_DEFAULT, size, align, vaddr,
    paddr, M_ZERO/*flags*/, handle);
  DEO_ASSERT((ret == DEO_SUCCESS), (
    "ipa_emu_mgr_alloc: ipa_emu_mgr_alloc_align failed, ret= %d\n", ret));
  return *vaddr;
}

void ipa_emu_mgr_free_align(deo_dma_handle_t handle)
{
	VVDRV_deo_dma_free(handle);
}

void ipa_emu_mgr_gsi_osal_register_isr(void *data)
{
  deo_interrupt_t deo_interupt_def = { 0 };
  int32 status = 0;

  deo_interupt_def.int_source = IPA_EMU_MGR_GSI_INTERRUPT_LINE;
  deo_interupt_def.int_priority = 0; /* High prio */
  deo_interupt_def.isr = ipa_emu_mgr_gsi_isr_wrapper;
  deo_interupt_def.user_data = (void*)data;
  deo_interupt_def.int_trigger = DEO_INT_TRIGGER_LEVEL;
  deo_interupt_def.int_targets = DEO_INT_TARGETS_CPU_ID(IPA_EMU_MGR_EE);
  deo_interupt_def.int_sec_level = DEO_INT_SECURE_LEVEL_NOT_APPLICABLE;
  
  status = VVDRV_deo_interrupt_attach(&deo_interupt_def);

  DEO_ASSERT((status == DEO_SUCCESS), (
    "ipa_emu_mgr_gsi_osal_register_isr: deo_interrupt_attach failed, ret= %d\n",
    status));

  status = VVDRV_deo_interrupt_enable(IPA_EMU_MGR_GSI_INTERRUPT_LINE);

  DEO_ASSERT((status == DEO_SUCCESS), (
    "ipa_emu_mgr_gsi_osal_register_isr: deo_interrupt_enable failed, ret= %d\n",
    status));
}

void ipa_emu_mgr_gsi_osal_deregister_isr()
{
	VVDRV_deo_interrupt_disable(IPA_EMU_MGR_GSI_INTERRUPT_LINE);
	VVDRV_deo_interrupt_detach(IPA_EMU_MGR_GSI_INTERRUPT_LINE);
}

void ipa_emu_mgr_ipa_register_isr()
{
  deo_interrupt_t deo_interupt_def = { 0 };
  int32 status = 0;

  deo_interupt_def.int_source = IPA_EMU_MGR_IPA_INTERRUPT_LINE;
  deo_interupt_def.isr = ipa_emu_mgr_ipa_isr_wrapper;
  deo_interupt_def.user_data = NULL;
  deo_interupt_def.int_priority = 0; /* High prio */
  deo_interupt_def.int_trigger = DEO_INT_TRIGGER_LEVEL;
  deo_interupt_def.int_targets = DEO_INT_TARGETS_CPU_ID(IPA_EMU_MGR_EE);
  deo_interupt_def.int_sec_level = DEO_INT_SECURE_LEVEL_NOT_APPLICABLE;

  status = VVDRV_deo_interrupt_attach(&deo_interupt_def);

  DEO_ASSERT((status == DEO_SUCCESS), (
    "ipa_emu_mgr_ipa_register_isr: deo_interrupt_attach failed, ret= %d\n",
    status));

  status = VVDRV_deo_interrupt_enable(IPA_EMU_MGR_IPA_INTERRUPT_LINE);

  DEO_ASSERT((status == DEO_SUCCESS), (
    "ipa_emu_mgr_ipa_register_isr: deo_interrupt_enable failed, ret= %d\n",
    status));
}

void ipa_emu_mgr_ipa_deregister_isr()
{
	VVDRV_deo_interrupt_disable(IPA_EMU_MGR_IPA_INTERRUPT_LINE);
	VVDRV_deo_interrupt_detach(IPA_EMU_MGR_IPA_INTERRUPT_LINE);
}

void ipa_emu_mgr_device_attach(uint32 device_id,void **handle)
{
  /* DEO does not need to attach */
}

void ipa_emu_mgr_interrupt_done(void* handle, uint32 irq_num)
{
  /* DEO does not have DONE function */
}

void ipa_emu_mgr_time_tick_attach(const char *timer_name,
  ipa_emu_mgr_time_timer_t * handle)
{
  int32 ret = DEO_SUCCESS;
  
  ret = VVDRV_deo_time_timer_start(handle);
  DEO_ASSERT((DEO_SUCCESS == ret), (
    "ipa_emu_mgr_time_tick_attach: deo_time_timer_start failed, ret= %d\n",
    ret));
}

void ipa_emu_mgr_time_tick_get_time_tick_64(ipa_emu_mgr_time_timer_t * handle,
  uint64 *ticks)
{
  int32 ret = DEO_SUCCESS;

  ret = VVDRV_deo_time_timer_get_elapsed_ticks(handle, ticks);
  DEO_ASSERT((DEO_SUCCESS == ret), (
    "ipa_emu_mgr_time_tick_get_time_tick_64: deo_time_timer_get_elapsed_ticks" 
    "failed, ret= %d\n", ret));
}

void* ipa_emu_mgr_virt_to_phys(uint32 addr)
{
	uint32 res = ((addr + IPA_EMU_MGR_VIRT_TO_PHYS_DIFF) & 0xFFFFFFFF);
	ASSERT(res >= IPA_EMU_MGR_COPSS_PHYS_BASE);
	return (void*)res;
}

void* ipa_emu_mgr_phys_to_virt(uint32 addr)
{
	uint32 res = ((addr - IPA_EMU_MGR_VIRT_TO_PHYS_DIFF) & 0xFFFFFFFF);
	ASSERT(res <= IPA_EMU_MGR_COPSS_PHYS_BASE);
	return (void*)res;
}

void ipa_emu_mgr_rci_init_done(void)
{
	ipa_emu_mgr.ipa_emu_mgr_rci_init_done = 1;
}

uint32 ipa_emu_mgr_is_rci_init_done(void)
{
	return ipa_emu_mgr.ipa_emu_mgr_rci_init_done;
}

void ipa_emu_mgr_dl_cnsmr_init_done(void)
{
  ipa_emu_mgr.ipa_emu_mgr_dl_cnsmr_init_done = 1;
}

void ipa_emu_mgr_wait_dl_cnsmr_init_done(void)
{
  while (!ipa_emu_mgr.ipa_emu_mgr_dl_cnsmr_init_done)
  {
    Sleep(1000);
  }
}

#ifndef IPA_EMU_MGR_H
#define IPA_EMU_MGR_H

/*
===========================================================================

FILE:         ipa_emu_mgr.h

$Header:

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/08/18   nl      MOB and off-target IPA emulation memory issue
09/10/17   nl      Added MOB and off-target IPA emulation support
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

#include <string.h>
#include <stdio.h>
#include "comdef.h"
#include "VVDRV_deo.h"
#include "VVDRV_deo_common.h"
#include "VVDRV_deo_mempool.h"
#include "VVDRV_deo_time.h"
#include "msmhwiobase_dut.h"
#include "msmhwioreg_dut.h"

/* -----------------------------------------------------------------------
**                           Data Types
** ----------------------------------------------------------------------- */

typedef struct {
  /*
  * Handle of PC memory pool
  */
  deo_dma_pool_handle_t ipaPcDmaPool;

  /*
  * Handle of GE IMEM pool
  */
  deo_dma_pool_handle_t ipaGeImemDmaPool;

  /**
  * @brief   The base address for COPSS registers
  */
  uint32 ipa_emu_mgr_base_addr;
  uint32 ipa_emu_mgr_virt_to_phys_addr_diff;
  /**
  * @brief   Syncronazation 
  */
  uint32 ipa_emu_mgr_rci_init_done;

  /**
  * @brief   Synchronization
  */
  uint32  ipa_emu_mgr_dl_cnsmr_init_done;

  /**
  * @brief   timer_task
  */
  uint32 ipa_emu_mgr_kill_timer;

}ipa_emu_mgr_s;

typedef void* (*ipa_emu_mgr_isr_type)(void* user_data);
typedef struct ipa_emu_mgr_isr_adapt {
  ipa_emu_mgr_isr_type isr_p;
  void* user_data;
} ipa_emu_mgr_isr_adapt_t;

typedef deo_time_timer_t ipa_emu_mgr_time_timer_t;

/* -----------------------------------------------------------------------
**                           DATA
** ----------------------------------------------------------------------- */

extern ipa_emu_mgr_s ipa_emu_mgr;

/* -----------------------------------------------------------------------
**                           MACROS
** ----------------------------------------------------------------------- */

/* Used for DEO config */
#define IPA_DMA_INTERNAL_MEMORY
#define DEO_CONFIG_MEMPOOL

/* MOB GLOBAL VAR AND ARR MACROs*/

#define IPA_EMU_MGR_VIRT_TO_PHYS_DIFF \
                                  ipa_emu_mgr.ipa_emu_mgr_virt_to_phys_addr_diff

#define IPA_EMU_MGR_VAR_HANDLER_SYM(var) var##_deo_handler
#define IPA_EMU_MGR_VAR_PHYS_SYM(var) var##_deo_phys
#define IPA_EMU_MGR_VAR_SIZE_SYM(var) var##_deo_size
#define IPA_EMU_MGR_ARR_ELEMENT_SIZE_SYM(var) var##_deo_arr_size
#define IPA_EMU_MGR_ARR_SIZE_SYM(var) var##_deo_arr_element_size

#define IPA_EMU_MGR_COPSS_PHYS_BASE GEN_EMU_BASE 
#define IPA_EMU_MGR_IS_PHYS(addr)  ((uint32)addr >= IPA_EMU_MGR_COPSS_PHYS_BASE)

#define IPA_EMU_MGR_VIRT_TO_PHYS(addr) ipa_emu_mgr_virt_to_phys((uint32)addr)

#define IPA_EMU_MGR_PHYS_TO_VIRT(addr) ipa_emu_mgr_phys_to_virt((uint32)addr)
/* Registers Macros */

#define IPA_EMU_MGR_REG_ADDR_TARGET_TO_EMU(reg_addr)  \
                                     (void*)(((uint32)reg_addr \
                                     -IPA_REG_BASE) \
                                     +(uint32)ipa_emu_mgr.ipa_emu_mgr_base_addr)

#define IPA_EMU_MGR_OUT_REG(addr,val) \
                        out_dword(IPA_EMU_MGR_REG_ADDR_TARGET_TO_EMU(addr), val)

#define IPA_EMU_MGR_IN_REG(addr) \
                        in_dword(IPA_EMU_MGR_REG_ADDR_TARGET_TO_EMU(addr))

/* DEFINES */
#define IPA_EMU_MGR_EE  1         /**< QDSP6 processor */

#define IPA_EMU_MGR_IPA_INTERRUPT_LINE (HAL_IPA_GEN_EMU_IPA_INTERRUPT_LINE + 1)
#define IPA_EMU_MGR_GSI_INTERRUPT_LINE (HAL_IPA_GEN_EMU_GSI_INTERRUPT_LINE + 1)

#define IPA_EMU_MGR_SUCCESS  0      /**<  Success value  */
#define IPA_EMU_MGR_ERROR    1      /**<  General error value  */
#define IPA_EMU_MGR_PENDING  2      /**<  Timer is still pending, not an error*/

  /* -----------------------------------------------------------------------
  **                           Functions
  ** ----------------------------------------------------------------------- */
/*
* C++ wrapper
*/
#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================

  FUNCTION:  ipa_emu_mgr_init

==============================================================================*/
/*!
  @brief
    Initialize Emulation environment

  @return
 
*/
/*============================================================================*/
  void ipa_emu_mgr_init(void);
/*==============================================================================

FUNCTION:  ipa_emu_mgr_uninit

==============================================================================*/
/*!
@brief
Clean Emulation environment

@return

*/
/*============================================================================*/
  void ipa_emu_mgr_uninit(void);

  /* ISR WRAPPERS */

/*===========================================================================

FUNCTION:  ipa_emu_mgr_gsi_isr_wrapper

===========================================================================*/
/*!
@brief
Interrupt handler wrapper for GSI interrupts

*/
/*=========================================================================*/
  void ipa_emu_mgr_gsi_isr_wrapper(uint32 irq, void *user_data);
/*===========================================================================

FUNCTION:  ipa_emu_mgr_ipa_isr_wrapper

===========================================================================*/
/*!
@brief
Interrupt handler wrapper for IPA interrupts

*/
/*=========================================================================*/
  void ipa_emu_mgr_ipa_isr_wrapper(uint32 irq, void *user_data);

  /* MEMORY MANAGMENT FUNCTIONS */

  /*============================================================================

  FUNCTION:  ipa_emu_mgr_alloc

  ============================================================================*/
  /*!
  @brief
  Allocate memory of specified size using emulation memory allocator.

  @param
    size: size of memory to be allocated.
  @return
    pointer to the allocated memory
  */
  /*==========================================================================*/
  void* ipa_emu_mgr_alloc(uint32 size);

  /*============================================================================

  FUNCTION:  ipa_emu_mgr_free

  ============================================================================*/
  /*!
  @brief
  Free memory that was allocated using emulation memory allocator.

  @param
    ptr: pointer to the memory to be freed
  
  @return
  
  */
  /*==========================================================================*/
  void ipa_emu_mgr_free(void* ptr);

  /*============================================================================

  FUNCTION:  ipa_emu_mgr_alloc_align

  ============================================================================*/
  /*!
  @brief
  Free aligned memory that was allocated using emulation memory allocator.

  @param
    size: size of memory to allocate
    align: unit of alignment requested from allocator
    handle: pointer to a memory handler that is returned, this used to free the
            allocated memory 

  @return
    pointer to the allocated memory

  */
  /*==========================================================================*/
  void* ipa_emu_mgr_alloc_align(uint32 size, uint32 align,
    deo_dma_handle_t *handle);
  
  /*============================================================================

  FUNCTION:  ipa_emu_mgr_free_align

  ============================================================================*/
  /*!
  @brief
  Allocate memory of specified size using emulation memory allocator that is
  aligned.

  @param
  handle: pointer to the memory handler to free

  @return
  
  */
  /*==========================================================================*/
  void ipa_emu_mgr_free_align(deo_dma_handle_t handle);

  /* DEO API FUNCTIONS */
  /*============================================================================

  FUNCTION:  ipa_emu_mgr_gsi_osal_register_isr

  ============================================================================*/
  /*!
  @brief
    Register the GSI isr

  @param
  data: User data

  @return

  */
  /*==========================================================================*/
  void ipa_emu_mgr_gsi_osal_register_isr(void *data);
  
  /*============================================================================

  FUNCTION:  ipa_emu_mgr_gsi_osal_deregister_isr

  ============================================================================*/
  /*!
  @brief
  Dergister the GSI isr

  @param

  @return

  */
  /*==========================================================================*/
  void ipa_emu_mgr_gsi_osal_deregister_isr();
  
  /*============================================================================

  FUNCTION:  ipa_emu_mgr_ipa_register_isr

  ============================================================================*/
  /*!
  @brief
  Register the IPA isr

  @param
  data: User data

  @return

  */
  /*==========================================================================*/
  void ipa_emu_mgr_ipa_register_isr();

  /*============================================================================

  FUNCTION:  ipa_emu_mgr_ipa_deregister_isr

  ============================================================================*/
  /*!
  @brief
  Dergister the IPA isr

  @param

  @return

  */
  /*==========================================================================*/
  void ipa_emu_mgr_ipa_deregister_isr();

  /*============================================================================

  FUNCTION:  ipa_emu_mgr_device_attach

  ============================================================================*/
  /*!
  @brief
  The function replaces DAL_DeviceAttach for emulation enviorment

  */
  /*==========================================================================*/
  void ipa_emu_mgr_device_attach(uint32 device_id, void **handle);
  
  /*============================================================================

  FUNCTION:  ipa_emu_mgr_interrupt_done

  ============================================================================*/
  /*!
  @brief
  The function replaces DalInterruptController_InterruptDone for 
  emulation enviorment

  */
  /*==========================================================================*/
  void ipa_emu_mgr_interrupt_done(void* handle, uint32 irq_num);

  /*============================================================================

  FUNCTION:  ipa_emu_mgr_time_tick_attach

  ============================================================================*/
  /*!
  @brief
  The function replaces DalTimetick_Attach for
  emulation enviorment

  */
  /*==========================================================================*/
  void ipa_emu_mgr_time_tick_attach(const char *timer_name, 
    ipa_emu_mgr_time_timer_t *handle);
  
  /*============================================================================

  FUNCTION:  ipa_emu_mgr_time_tick_get_time_tick_64

  ============================================================================*/
  /*!
  @brief
  The function replaces DALtick_get_time_tick_64 for
  emulation enviorment

  */
  /*==========================================================================*/
  void ipa_emu_mgr_time_tick_get_time_tick_64(ipa_emu_mgr_time_timer_t * handle,
    uint64 *ticks);
  
  /*============================================================================

  FUNCTION:  ipa_emu_mgr_virt_to_phys

  ============================================================================*/
  /*!
  @brief
  The function converts virtual address to emulation physical address

  @Params: addr - virtual address to convert

  @return: Physical address corespondes to the virtual address
  */
  /*==========================================================================*/
  void* ipa_emu_mgr_virt_to_phys(uint32 addr);
  
  /*============================================================================

  FUNCTION:  ipa_emu_mgr_phys_to_virt

  ============================================================================*/
  /*!
  @brief
  The function converts emulation physical address to virtual address

  @Params: addr - Physical address to convert

  @return: Virtual address corespondes to the virtual address
  */
  /*==========================================================================*/
  void* ipa_emu_mgr_phys_to_virt(uint32 addr);

  /*============================================================================

  FUNCTION:  ipa_emu_mgr_rci_init_done

  ============================================================================*/
  /*!
  @brief
  The function is used to synchornize thread initiation on emulation system, 
  the function is called when all IPA threads are initiated.

  */
  /*==========================================================================*/
  void ipa_emu_mgr_rci_init_done(void);

  /*============================================================================

  FUNCTION:  ipa_emu_mgr_is_rci_init_done

  ============================================================================*/
  /*!
  @brief
  The function is used to synchornize thread initiation on emulation system,
  the function is used to check if all IPA threads initiated successfully

  */
  /*==========================================================================*/
  uint32 ipa_emu_mgr_is_rci_init_done(void);

  /*============================================================================

  FUNCTION:  ipa_emu_mgr_dl_cnsmr_init_done

  ============================================================================*/
  /*!
  @brief
  The function is used to synchornize thread initiation on emulation system,
  the function is called when all IPA threads are initiated.

  */
  /*==========================================================================*/
  void ipa_emu_mgr_dl_cnsmr_init_done(void);
  
  /*============================================================================

  FUNCTION:  ipa_emu_mgr_wait_dl_cnsmr_init_done

  ============================================================================*/
  /*!
  @brief
  The function is used to synchronize thread initiation on emulation system,
  the function is used to check if ipa_dl_cnsmr thread initiated successfully

  */
  /*==========================================================================*/
  void ipa_emu_mgr_wait_dl_cnsmr_init_done(void);

  void ipa_emu_mgr_timer_task_main(dword dummy);
#ifdef __cplusplus
}
#endif

#endif /* IPA_EMU_MGR_H */
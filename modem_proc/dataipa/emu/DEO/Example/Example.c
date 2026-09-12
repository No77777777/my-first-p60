/*=========================================================================*//**
    @file  Example.c

    @brief example file for using the DEO framework

    @example Example.c
*//****************************************************************************/
/*------------------------------------------------------------------------------
    Copyright (c) 2011 Qualcomm Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Confidential and Proprietary
------------------------------------------------------------------------------*/

/************************************************************************/
/*                              INCLUDES                                */
/************************************************************************/

#include "deo.h"
#include "deo_rpb.h"


/************************************************************************/
/*                               MACROS                                 */
/************************************************************************/

#define GenericReadRegister(Base_Address, offset, registervalue) \
   registervalue = Base_Address[offset>>2]

#define GenericWriteRegister(Base_Address, offset, registervalue) \
   Base_Address[offset>>2] = registervalue


/************************************************************************/
/*                               DEFINES                                */
/************************************************************************/

#define WAIT_IN_MS                             500
#define HSI_EMULATION_INTERRUPT_SOURCE         33
#define DMA_BUFFER_SIZE                        64
#define DMA_BUFFER_ALIGNMENT                   8
#define INT_TRIGGER_EMULATION_REGISTER_OFFSET  0x808


/************************************************************************/
/*                              TYPEDEFS                                */
/************************************************************************/

/* Standard buffer (with physical address for DMA) */
typedef struct {
    void        *vaddr;
    void        *dma_addr;
    uint32    buffer_size;
    void        *cookie; 
} buffer_t;


/************************************************************************/
/*                              FUNCTIONS                               */
/************************************************************************/

/**
 * @fn   static void my_isr(uint32 int_source, void *arg)
 *
 * @brief            An example ISR for DEO demo
 *
 * @param            int_source     The int source. 
 * @param            arg   If non-null, the argument. 
 */
static void my_isr(uint32 int_source, void *arg)
{
   (void)arg;

   /* Disabling the interrupt (In the ISR you should only clean the interrupt and not disable it) */
   deo_interrupt_disable(int_source);

   /* A prove that we visited the ISR */
   DEO_LOG_MSG(DEO_DS_INFO, DEO_DEMO, ("DEO ISR was called for interrupt source %d!!!!!\n", int_source));
}

/**
 * @fn   int main(void)
 *
 * @brief            Main entry-point for the example application. 
 *
 * @return           Exit-code for the process - 0 for success, else an error code. 
 */
int main(void)
{
   deo_cfg_t cfg; /* DEO configuration struct */
   struct pcie_translate_registers_s pcie_translate_cfg;
   deo_interrupt_controller_init_t ic_cfg;
   deo_interrupt_t deo_interrupt;
   int user_data;
   deo_rpb_info_t* pcie_cfg;
   buffer_t buffer;

   /* Set P2A translate register */
   pcie_translate_cfg.P2A_arguments[0].setRegister = TRUE;
   pcie_translate_cfg.P2A_arguments[0].barIndex = 0;
   pcie_translate_cfg.P2A_arguments[0].ambaAddress = (void*)0x30000000;

   /* Set A2P translate register */
   pcie_translate_cfg.A2P_arguments[0].setRegister = TRUE;
   pcie_translate_cfg.A2P_arguments[0].pcieAmbaAddress = (void*)(0x30800000);

   /* Init interrupt controller */
   ic_cfg.need_to_init = TRUE;
   ic_cfg.bar_index = 0;
   ic_cfg.offset = 0x140000;

   /* Setting the configuration structure */
   cfg.pcie_translate_cfg = (void*) &pcie_translate_cfg;
   cfg.interrupt_controller_cfg = (void*) &ic_cfg;

   /* Initializing the framework */
   if (DEO_ERROR == deo_init(&cfg))
   {
      DEO_LOG_MSG(DEO_DS_FATAL, DEO_DEMO, ("DEO init failed\n"));
   }

   /* Setting the interrupt structure */
   deo_interrupt.int_source = HSI_EMULATION_INTERRUPT_SOURCE;
   deo_interrupt.int_priority = 0;
   deo_interrupt.int_edge = FALSE;
   deo_interrupt.int_up = FALSE;
   deo_interrupt.user_data = &user_data;
   deo_interrupt.isr = my_isr;

   /* Attaching the interrupt */
   if (DEO_ERROR == deo_interrupt_attach(&deo_interrupt))
   {
      DEO_LOG_MSG(DEO_DS_FATAL, DEO_DEMO, ("DEO interrupt attach failed\n"));
   }

   /* Sleeping to allow the ISR to be called */
   deo_sleep(WAIT_IN_MS);

   /* clearing interrupt status*/
   pcie_cfg = (deo_rpb_info_t*)cfg.pcie_cfg;
   GenericWriteRegister(((unsigned long *)(pcie_cfg->barAddr[0])), INT_TRIGGER_EMULATION_REGISTER_OFFSET, 0x0);
   deo_interrupt_clear(deo_interrupt.int_source);

   /* Enabling the interrupt */
   if (DEO_ERROR == deo_interrupt_enable(deo_interrupt.int_source))
   {
      DEO_LOG_MSG(DEO_DS_FATAL, DEO_DEMO, ("DEO interrupt enable failed\n"));
   }

   /* triggering an interrupt */
   GenericWriteRegister(((unsigned long *)(pcie_cfg->barAddr[0])), INT_TRIGGER_EMULATION_REGISTER_OFFSET, 0xffffffff);

   /* Sleeping to allow the ISR to be called */
   deo_sleep(WAIT_IN_MS);

   /* Allocating 64 bytes of DMA memory */
   buffer.buffer_size = DMA_BUFFER_SIZE;
   if (DEO_ERROR == deo_dma_alloc(buffer.buffer_size, 
                                    DMA_BUFFER_ALIGNMENT, 
                                    &buffer.vaddr,
                                    &buffer.dma_addr, 
                                    M_CACHABLE, 
                                    &buffer.cookie))
   {
      DEO_LOG_MSG(DEO_DS_FATAL, DEO_DEMO, ("DEO DMA alloc failed\n"));
   }

   /* Un-initializing DEO */
   deo_uninit();

   return 0;
}
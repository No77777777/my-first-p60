/**
   @file crash_hook.c
   @author anandj, raholr
   @brief Crash handler for shutdown module in perf.mpss component 
    
   Copyright (c) 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
*/
#include "crash_hook.h"
#include "err.h"
#include "qurt.h"
#include "qurt_tlb.h"
#include "ULogFront.h"
#include "perf_err_cb.h"

#define SIZE_ALIGN 4096
//#ifndef CLADE_PROTECTED_REGION_OFFSET
//  #define CLADE_PROTECTED_REGION_OFFSET (0x600000)
//#endif

#define ALIGNMENT (1024*1024)

/** @brief TLB entry type in fields and 64-bit value form
    @todo anandj This should ideally be exported by QuRT. Until that happens,
    here is it... */
typedef union 
{
    /** @brief Bit breakdown of TLB entry */
    struct
    {
        unsigned int    S: 1;
        unsigned int  PPN:23;
        unsigned int    C: 4;
        unsigned int    U: 1;
        unsigned int    R: 1;
        unsigned int    W: 1;
        unsigned int    X: 1;
        unsigned int  VPN:20;
        unsigned int ASID: 7;
        unsigned int   A0: 1;
        unsigned int   A1: 1;
        unsigned int   EP: 1;
        unsigned int    G: 1;
        unsigned int    V: 1;
    } fields;

    /** 64-bit value of TLB entry */
    unsigned long long int value;

} tlb_entry_t;

/** @brief reference to start_va_enable_fw_dump_overlay It is the starting point
           for firmware dump overlay */
unsigned int __attribute__((section (".data"))) start_va_enable_fw_dump_overlay = 0;

/** @brief Reference to symbol that marks the end of CLADE/Q6Zip's RW build-time
           compressed section */
unsigned int __attribute__((section (".data"))) end_va_enable_fw_dump_overlay = 0;

/** @brief Reference to symbols that marks the start and end of overlay_mem_dump build-time
           section */
unsigned int __attribute__ ((section(".data"))) start_va_map_fw_dump_overlay = 0;

unsigned int __attribute__ ((section(".data"))) end_va_map_fw_dump_overlay = 0;

extern unsigned int __attribute__ ((weak)) __clade_region_high_pd0_start__;
extern unsigned int __attribute__ ((weak)) __clade_protected_reserve_size__;



qurt_paddr_t start_pa_enable_fw_dump_overlay;
unsigned int size_pa_enable_fw_dump_overlay = 0;

extern int perf_has_crashed;
extern int internal_perf_err_cb_register_in_pre_STM(perf_err_cb_ptr cb);
/** @brief ULog handle for this module */
static ULogHandle ulogh;

static void perf_shutdown_crash_hook_enable_firmware_dump_overlay (void)
{
    int qurt_retval;
    tlb_entry_t tlb_entry;
    unsigned int tlb_idx, begin_tlb_idx, end_tlb_idx;
    
    perf_has_crashed = 1;
    /** @brief no VA overlay, need to create a seperate maping for fw dump overlay*/
    if (start_va_map_fw_dump_overlay != 0)
    {
      /*
       * Begin FW overlay mem dump __clade_protected_reserve_size__ bytes away from beginning of region_high_clade 
       * to prevent functions needed by crash handlers/callbacks from being overwritten.
       */
      ULOG_RT_PRINTF_1( ulogh, "Memdump overlay Start VA=0x%08x", start_va_enable_fw_dump_overlay );
      start_va_enable_fw_dump_overlay += (unsigned int)&__clade_protected_reserve_size__;
      ULOG_RT_PRINTF_1( ulogh, "Memdump overlay modified Start VA=0x%08x", start_va_enable_fw_dump_overlay );

      ULOG_RT_PRINTF_1( ulogh, "Memdump overlay End VA=0x%08x", end_va_enable_fw_dump_overlay );
      end_va_enable_fw_dump_overlay=(end_va_enable_fw_dump_overlay&(0xFFF00000));
      ULOG_RT_PRINTF_1( ulogh, "Memdump overlay modified End VA=0x%08x", end_va_enable_fw_dump_overlay );

      size_pa_enable_fw_dump_overlay = end_va_enable_fw_dump_overlay - start_va_enable_fw_dump_overlay;
      ULOG_RT_PRINTF_1( ulogh, "Memdump overlay size=0x%08x", size_pa_enable_fw_dump_overlay );

      ULOG_RT_PRINTF_2( ulogh, "Memdump memory map VA start: 0x%08X  end_va_map_fw_dump_overlay end: 0x%08X",
                          start_va_map_fw_dump_overlay, end_va_map_fw_dump_overlay);
      //lookup physical addresses
      //returns 0 if there is no VA to PA mapping or if translation fails
      start_pa_enable_fw_dump_overlay = qurt_lookup_physaddr(start_va_enable_fw_dump_overlay);
      ULOG_RT_PRINTF_1( ulogh, "Memdump address lookup PA=0x%08x", start_pa_enable_fw_dump_overlay );
      if (!start_pa_enable_fw_dump_overlay)
      {
        ULOG_RT_PRINTF_0( ulogh, "Error: Could not get the start PA for overlay section" );
        return;
      }
      start_pa_enable_fw_dump_overlay = (start_pa_enable_fw_dump_overlay + ALIGNMENT-1) & (~(ALIGNMENT-1)); // align to next 1M
      ULOG_RT_PRINTF_1( ulogh, "Memdump overlay start PA=0x%08x", start_pa_enable_fw_dump_overlay );

      ULOG_RT_PRINTF_2( ulogh, "Memdump creating memory map PA 0x%08X, size = 0x%08X",
                          start_pa_enable_fw_dump_overlay, size_pa_enable_fw_dump_overlay);
      qurt_retval = qurt_mapping_create(start_va_map_fw_dump_overlay, start_pa_enable_fw_dump_overlay, size_pa_enable_fw_dump_overlay, QURT_MEM_CACHE_WRITEBACK, QURT_PERM_READ | QURT_PERM_WRITE);
      if(qurt_retval != QURT_EOK)
      {
        ULOG_RT_PRINTF_1( ulogh, "Error: memory map fail :%x", qurt_retval);
        return;
      }
      ULOG_RT_PRINTF_2( ulogh, "Memdump overwrite address start : 0x%08X end: 0x%08X",
                                ((unsigned int)&__clade_region_high_pd0_start__ + (unsigned int)&__clade_protected_reserve_size__),\
                                ((unsigned int)&__clade_region_high_pd0_start__ + (unsigned int)&__clade_protected_reserve_size__) + size_pa_enable_fw_dump_overlay);
    }

}

void perf_shutdown_crash_hook_rcinit (void)
{
    ULogFront_RealTimeInit( &ulogh, "Perf Crash Hook", 2048, ULOG_MEMORY_LOCAL, ULOG_LOCK_OS );

    if (!internal_perf_err_cb_register_in_pre_STM( perf_shutdown_crash_hook_enable_firmware_dump_overlay ) )
    {
        ULOG_RT_PRINTF_0( ulogh, "Failed to register error callback" );
    }
}

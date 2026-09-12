#ifndef IPA_PORT_H
#define IPA_PORT_H

/* -----------------------------------------------------------------------
**                           INCLUDES
** ----------------------------------------------------------------------- */

#include "HALcomdef.h"
#include "compiler_int.h"
#ifdef DEO_SHARED_CODE
#include "VVDRV_deo_common.h"
#include "VVDRV_deo_debug.h"
#include "VVDRV_deo_interrupt.h"
#include "VVDRV_deo_interrupt_os.h"
#include "VVDRV_deo_lists.h"
#include "VVDRV_deo_mempool.h"
#include "VVDRV_deo_time.h"

/* -----------------------------------------------------------------------
**                           MACROS
** ----------------------------------------------------------------------- */

#define GSI_OS_LOCAL_WRAPPER

#define deo_init VVDRV_deo_init
#define deo_uninit VVDRV_deo_uninit
#define deo_mem_init VVDRV_deo_mem_init
#define deo_mem_uninit VVDRV_deo_mem_uninit
#define deo_mem_defrag VVDRV_deo_mem_defrag
#define deo_mem_log_print VVDRV_deo_mem_log_print
#define deo_alloc VVDRV_deo_alloc
#define deo_free VVDRV_deo_free
#define deo_dma_pool_create VVDRV_deo_dma_pool_create
#define deo_dma_pool_release VVDRV_deo_dma_pool_release
#define deo_dma_pool_alloc VVDRV_deo_dma_pool_alloc
#define deo_dma_pool_add_block VVDRV_deo_dma_pool_add_block
#define deo_dma_free VVDRV_deo_dma_free
#define deo_interrupt_attach VVDRV_deo_interrupt_attach
#define deo_interrupt_detach VVDRV_deo_interrupt_detach
#define deo_interrupt_enable VVDRV_deo_interrupt_enable
#define deo_interrupt_disable VVDRV_deo_interrupt_disable
#define deo_interrupt_enable_all VVDRV_deo_interrupt_enable_all
#define deo_interrupt_disable_all VVDRV_deo_interrupt_disable_all
#define deo_interrupt_clear VVDRV_deo_interrupt_clear
#define deo_time_sleep_ms VVDRV_deo_time_sleep_ms
#define deo_time_sleep_us VVDRV_deo_time_sleep_us
#define deo_sleep deo_time_sleep_ms
#define deo_interrupt_disable_os VVDRV_deo_interrupt_disable_os
#define deo_interrupt_enable_os VVDRV_deo_interrupt_enable_os

#define TIME_GET_MS(time) \
  (uint64)((uint64)(((time).tv_sec) * 1000) + (uint64)(((time).tv_usec) / 1000))

#define TIME_ADD_MS(time, ms) \
   do { \
   (time)->tv_usec += ms * 1000; \
   (time)->tv_sec  += ((time)->tv_usec / (1000 * 1000)); \
   (time)->tv_usec %= (1000 * 1000); \
   } while (0);

#define TIME_DIFF(start, end)   (TIME_GET_MS((end)) - TIME_GET_MS((start)))

#define DEO_TIME_H_TO_MICRO(time) \
            (((uint64)(1000000*((time)->tv_sec))) + ((uint64)((time)->tv_usec)))

/* less than */
#define time_lt(t1, t2) \
   ((t1)->tv_sec < (t2)->tv_sec || \
   ((t1)->tv_sec == (t2)->tv_sec && (t1)->tv_usec < (t2)->tv_usec))

#define time_egt(t1, t2) (!time_lt(t1, t2))
#else
#include "deo_common.h"
#include "deo_debug.h"
#include "deo_interrupt.h"
#include "deo_interrupt_os.h"
#include "deo_lists.h"
#include "deo_mempool.h"
#include "deo_time.h"
#endif
#include <stddef.h>
#include "ipa_debug.h"
#include "VVUTIL_randomization.h"

/* -----------------------------------------------------------------------
**                           MACROS
** ----------------------------------------------------------------------- */

/*  @brief  Size of the storage for random state sroring/restoring service
*/
#define  IPA_RAND_STATE_STORAGE_SIZE   64

/*  @brief  MS Magic Debug Values list (partial)

	Notice (!) that the values (defined below) are valid ONLY for Windows platform, when you are using the debug mode.
	
	For more details, you can visite this page:
	https://en.wikipedia.org/wiki/Magic_number_%28programming%29#Magic_debug_values	
*/
#ifdef CORE_LEVEL_GE
#define IPA_MS_DBG_HEAP_ALLOC                   0xABABABAB /** Used by Microsoft's HeapAlloc() to mark "no man's land" guard bytes after allocated heap memory                   */
#define IPA_MS_DBG_FREE_MEMORY                  0xABADCAFE /** A startup to this value to initialize all free memory to catch errant pointers                                    */
#define IPA_MS_DBG_LOCAL_ALLOC                  0xBAADF00D /** Used by Microsoft's LocalAlloc(LMEM_FIXED) to mark uninitialised allocated heap memory                            */
#define IPA_MS_DBG_ERROR_CODE                   0xBADCAB1E /** Error Code returned to the Microsoft eVC debugger when connection is severed to the debugger                      */
#define IPA_MS_DBG_MAGIC_NUMBER                 0xBEEFCACE /** Used by Microsoft .NET as a magic number in resource files                                                        */
#define IPA_MS_DBG_UNINITIALISED_STACK_MEMORY   0xCCCCCCCC /** Used by Microsoft's C++ debugging runtime library to mark uninitialised stack memory                              */
#define IPA_MS_DBG_UNINITIALISED_HEAP_MEMORY    0xCDCDCDCD /** Used by Microsoft's C++ debugging runtime library to mark uninitialised heap memory                               */
#define IPA_MS_DBG_STOP_ERROR_CODE              0xDEADDEAD /** A Microsoft Windows STOP Error code used when the user manually initiates the crash.                              */
#define IPA_MS_DBG_HEAP_GUARD_BYTES             0xFDFDFDFD /** Used by Microsoft's C++ debugging heap to mark "no man's land" guard bytes before and after allocated heap memory */
#define IPA_MS_DBG_FREED_HEAP_MEMORY            0xFEEEFEEE /** Used by Microsoft's HeapFree() to mark freed heap memory                                                          */
#endif /* end of MS Magic Debug Values list */

#define IPA_NO_INTERRUPT_LINE 0xffffffff

#define IPA_ISR_DONT_CALL_SOFT_INTR 0
#define IPA_ISR_CALL_SOFT_INTR 1
#define IPA_ISR_NOT_RECOGNIZED -1

static uint32 LE32TOH(const void *ptr)
{
    uint32 ret  = (uint32)(((uint8 *)(ptr))[0]);
           ret |= ((uint32)(((uint8 *)(ptr))[1]) << 8);
           ret |= ((uint32)(((uint8 *)(ptr))[2]) << 16);
           ret |= ((uint32)(((uint8 *)(ptr))[3]) << 24);
    return ret;
}

/*
 * Converts from big endian notation to little endian 
 */
#define BE32TOH(ptr) \
   ((uint32)(((uint8 *)(ptr))[3]) | \
   ((uint32)(((uint8 *)(ptr))[2]) << 8) | \
   ((uint32)(((uint8 *)(ptr))[1]) << 16) | \
   ((uint32)(((uint8 *)(ptr))[0]) << 24))

#define HTOLE32(ptr,v) \
   (((uint8 *)(ptr))[0] = (uint8)(v), \
   ((uint8 *)(ptr))[1] = (uint8)((v) >> 8), \
   ((uint8 *)(ptr))[2] = (uint8)((v) >> 16), \
   ((uint8 *)(ptr))[3] = (uint8)((v) >> 24))

#define LE16TOH(ptr) \
   (((uint8*)(ptr))[0] | (((uint8*)(ptr))[1] << 8))

/*
 * Converts from big endian notation to little endian 
 */
#define BE16TOH(ptr) \
   ((uint16)(((uint8 *)(ptr))[1]) | \
   ((uint16)(((uint8 *)(ptr))[0]) << 8))

#define HTOLE16(ptr,v) \
   (((uint8*)(ptr))[0] = (uint8)(v), \
   ((uint8*)(ptr))[1] = (uint8)((v) >> 8))


static uint16 ipa_htons(uint16 h)
{
   return ( ((h & 0xFF00) >> 8) | 
            ((h & 0x00FF) << 8) );
}

static uint32 ipa_htonl(uint32 h)
{
   return ( ((h & 0xFF000000) >> 24) | 
            ((h & 0x00FF0000) >> 8)  |
            ((h & 0x0000FF00) << 8)  |
            ((h & 0x000000FF) << 24) );
}

/**
 * @brief   Arithmetics for fields of modulo 2^N. 
 *
 * @param   cntr            First argument of arithmetic operation (minuend for subtraction; addendum for addition). 
 * @param   to_add/to_sub   Second argument for arithmetic operation (subtrahend for subtraction; 2nd addendum for addition). 
 * @param   size_power_of_2 Size of the group (field). Must be power of 2 (2, 4, 8, 16 etc). 
 */
#define ADD_WRAP_PWR2(cntr, to_add, size_power_of_2)    (((cntr) + (to_add)) & ((size_power_of_2)-1))
#define SUB_WRAP_PWR2(minuend, to_sub, size_power_of_2) (((minuend) + (size_power_of_2) - (to_sub)) & ((size_power_of_2)-1))
#define INC_WRAP_PWR2(cntr, size_power_of_2)            ADD_WRAP_PWR2(cntr, 1, size_power_of_2)
#define DEC_WRAP_PWR2(cntr, size_power_of_2)            SUB_WRAP_PWR2(cntr, 1, size_power_of_2)

/**
 * @brief   Arithmetics for fields of arbitrary size. 
 *
 * @param   cntr            First argument of arithmetic operation (minuend for subtraction; addendum for addition). 
 * @param   to_add/to_sub   Second argument for arithmetic operation (subtrahend for subtraction; 2nd addendum for addition). 
 * @param   size_of_group   Size of the group. 
 */
#define ADD_WRAP(cntr, to_add, size_of_group)           (((cntr) + (to_add)) % (size_of_group))
#define SUB_WRAP(minuend, to_sub, size_of_group)        (((minuend) + (size_of_group) - (to_sub)) % (size_of_group))
#define INC_WRAP(cntr, size_of_group)                   ADD_WRAP(cntr, 1, size_of_group)
#define DEC_WRAP(cntr, size_of_group)                   SUB_WRAP(cntr, 1, size_of_group)




/* -----------------------------------------------------------------------
**                           TYPEDEFS
** ----------------------------------------------------------------------- */

/**
 * @struct  *irq_resource_h
 *
 * @brief   Handle to the IRQ resource. 
*/
typedef struct irq_resource_s *irq_resource_h;

/**
 * @struct  *os_mutex_h
 *
 * @brief   handle to the mutex structure. 
*/
typedef struct os_mutex_s *os_mutex_h;

/**
 * @struct  *os_spinlock_h
 *
 * @brief   Handle to the spinlock structure. 
*/
typedef struct os_spinlock_s *os_spinlock_h;

/**
 * @struct  *os_timer_h
 *
 * @brief   handle to the os_timer_s struct.
*/
typedef struct os_timer_s *os_timer_h;

/**
 * @struct  *os_msleep_h
 *
 * @brief   handle to the os_msleep_s struct. 
*/
typedef struct os_msleep_s *os_msleep_h;

/**
 * @typedef deo_time_t ipa_time_t
 *
 * @brief   Defines an alias representing the operating system time type.
*/
#ifdef DEO_SHARED_CODE
typedef struct
{
   uint32 tv_sec;     /* seconds */
   uint32 tv_usec;    /* microseconds */
} ipa_time_t;
#else
typedef deo_time_t ipa_time_t;
#endif

/**
 * @typedef void (*thread_func)(void *)
 *
 * @brief   Defines an alias representing the the task function prototype.
*/
typedef void (*thread_func)(void *);

/**
 * @typedef int (*interrupt_handler)(void *)
 *
 * @brief   Defines an alias representing the ISR.
*/
typedef int (*interrupt_handler)(void *);

/**
 * @enum ipaEventType
 *
 * @brief   Values that represent IPA events. 
*/
typedef int ipaEventType;

typedef enum os_time_units_e
{
    T_MSEC = 1,
    T_USEC,
    T_SEC
} os_time_units_e;

/**
 * @brief   Printing clients. 
*/
typedef enum IPA_OS_PrintClientType
{
   IPA_OS_PORT,
   IPA_TARGET,
   IPA_DRV,
   IPA_HAL,
   IPA_MGR,
   IPA_TEST,
   IPA_GENERAL,
   IPA_NUM_PRINT_CLIENTS
} IPA_OS_PrintClientType;


/**
 * @brief   Printing levels. 
*/
typedef enum IPA_OS_PrintLevelType
{
   IPA_VERBOSE,
   IPA_INFO,
   IPA_WARNING,
   IPA_ERROR,
   IPA_FATAL,
   IPA_NUM_PRINT_LEVELS
} IPA_OS_PrintLevelType;

/* -----------------------------------------------------------------------
**                           FUNCTIONS
** ----------------------------------------------------------------------- */

/**
 * @brief   Get allocation counters. 
 *
 * @param [out]   dma_allocs     If non-null, the dma allocation counter (via ipa_dma_alloc). 
 * @param [out]   general_allocs If non-null, the general allocation counter (via ipa_alloc). 
*/
void ipa_get_alloc_counters(int* dma_allocs, int* general_allocs);

/**
 * @fn   void *ipa_alloc(uint32 size, uint16 flags)
 *
 * @brief   Ipa allocate memory. 
 *
 * @param   size  The size. 
 * @param   flags The flags (like M_ZERO, M_CACHABLE and M_PAGE_ALIGN). 
 *
 * @return  null if it fails, else a pointer to the new allocated buffer. 
*/
void *ipa_alloc_fn(uint32 size, uint16 flags);

void* ipa_alloc_mark_vi(uint32 size, uint16 flags);

/**
 * @brief   Wrapper for regular ipa_alloc, to collect dbg info
 */
#ifdef   IPA_DBG_MEMORY_LEAKAGE_IS_ENABLED
#define ipa_alloc(size, flags) \
   ipa_alloc_mark_vi(size, flags);\
   DEO_DBG_E(DTEST, ("ipa_alloc : line %5d : fn %-32s : file %s : \n", __LINE__, __FUNCTION__, __FILE__)) 
#else
#define ipa_alloc ipa_alloc_mark_vi
#endif

/**
 * @fn   void ipa_free(void *item)
 *
 * @brief   Frees rge buffer. 
 *
 * @param [in,out]   item  If non-null, the item. 
*/
void ipa_free_fn(void *item);

void ipa_free_mark_vi(void* pBuf);

/**
 * @brief   Wrapper for regular ipa_dma_free, to collect dbg info
 */
#ifdef   IPA_DBG_MEMORY_LEAKAGE_IS_ENABLED
#define ipa_free(item) if (NULL != (item)) { ipa_free_vi(item); (item) = NULL; DEO_DBG_E(DTEST, ("\t\t\t\tipa_free : line %5d : fn %-32s : file %s : \n", __LINE__, __FUNCTION__, __FILE__)); }
#else
#define ipa_free ipa_free_vi
#endif

/**
 * @brief   Wrapper for regular ipa_free
 */
#define ipa_free_vi(item) if (NULL != (item)) { ipa_free_mark_vi(item); (item) = NULL; }

/**
 * @fn   int ipa_dma_alloc(uint32 size, uint16 align, void **vaddr, void **paddr, uint16 flags,
 * void** handle)
 *
 * @brief   Ipa dma allocate. 
 *
 * @param   size              The size. 
 * @param   align             The align. 
 * @param [out]   vaddr    If non-null, the vaddr. 
 * @param [out]   paddr    If non-null, the paddr. 
 * @param   flags             The flags (like M_ZERO, M_CACHABLE and M_PAGE_ALIGN). 
 * @param [out]   handle   If non-null, the handle. 
 *
 * @return  0 if successful or non-zero value on failure. 
*/
int ipa_dma_alloc_fn(uint32 size, uint16 align, void **vaddr, uint64 *paddr,
   uint16 flags, void** handle);

/**
 * @fn   int ipa_dma_alloc(uint32 size, uint16 align, void **vaddr, void **paddr, uint16 flags,
 * void** handle)
 *
 * @brief   Ipa dma allocate. 
 *
 * @param [in]    pool     DMA pool handle
 * @param [in]    size     The size. 
 * @param [in]    align    The align. 
 * @param [out]   vaddr    If non-null, the vaddr. 
 * @param [out]   paddr    If non-null, the paddr. 
 * @param [in]    flags    The flags (like M_ZERO, M_CACHABLE and M_PAGE_ALIGN). 
 * @param [out]   handle   If non-null, the handle. 
 *
 * @return  0 if successful or non-zero value on failure. 
*/
int ipa_dma_pool_alloc_fn(deo_dma_pool_handle_t pool, uint32 size, uint32 align, void **vaddr, uint64 *paddr,
                          uint16 flags, void** handle);

/**
 * @brief   Wrapper for regular ipa_dma_alloc, to collect dbg info
 */
#ifdef   IPA_DBG_MEMORY_LEAKAGE_IS_ENABLED
#define ipa_dma_alloc(size, align, vaddr, paddr, flags, handle) \
   ipa_dma_alloc_fn(size, align, vaddr, paddr, flags, handle);\
   DEO_DBG_E(DTEST, ("ipa_dma_alloc : line %5d : fn %-32s : file %s : \n", __LINE__, __FUNCTION__, __FILE__))
#define ipa_dma_pool_alloc(pool, size, align, vaddr, paddr, flags, handle) \
   ipa_dma_pool_alloc_fn(pool, size, align, vaddr, paddr, flags, handle);\
   DEO_DBG_E(DTEST, ("ipa_dma_alloc : line %5d : fn %-32s : file %s : \n", __LINE__, __FUNCTION__, __FILE__))
#else
#define ipa_dma_alloc ipa_dma_alloc_fn
#define ipa_dma_pool_alloc ipa_dma_pool_alloc_fn
#endif

/**
 * @brief   Wrapper for regular ipa_dma_alloc, assuming use of VIDRV_ipa_BufferType defined in VIDRV_ipa.h 
 */
#define ipa_dma_alloc_vi(align, vi_buf, flags)  ipa_dma_alloc_mark_vi(align, &(vi_buf), flags)

/**
 * @fn   void ipa_dma_free(void* p)
 *
 * @brief   Frees DMA buffer. 
 *
 * @param [in,out]   p  If non-null, the. 
 */
void ipa_dma_free_fn(void* p);

/**
 * @brief   Wrapper for regular ipa_dma_free, to collect dbg info
 */
#ifdef   IPA_DBG_MEMORY_LEAKAGE_IS_ENABLED
   #define ipa_dma_free(handle)  ipa_dma_free_fn(handle); DEO_DBG_E(DTEST, ("\t\t\t\tipa_dma_free : line %5d : fn %-32s : file %s : \n", __LINE__, __FUNCTION__, __FILE__));
#else
   #define ipa_dma_free ipa_dma_free_fn
#endif

/**
 * @brief   Wrapper for regular ipa_dma_free, assuming use of VIDRV_ipa_BufferType defined in VIDRC_ipa.h 
 */
#define ipa_dma_free_vi(vi_buf) ipa_dma_free_mark_vi(&(vi_buf))

/**
 * @fn   void ipa_os_get_time(ipa_time_t *time)
 *
 * @brief   Gets current time according to the operating system. 
 *
 * @param [out]      time  If non-null, the time structure. 
*/
void ipa_os_get_time(ipa_time_t *time);

/**
 * @fn   void ipa_sleep(int sleepTime)
 *
 * @brief   sleep in milliseconds. 
 *
 * @param   sleepTime  Time to sleep in milliseconds. 
*/
void ipa_sleep(uint32 sleepTime);

/**
 * @brief   sleep in uSec. 
 *
 * @param   sleepTime  Time to sleep in uSec. 
*/
#define ipa_sleep_us(x) (void)deo_time_sleep_us((x)) 

/**
 * @brief   Calculate timeout starting from current time. 
 *
 * @param   delay               The delay that should pass starting from the current moment. 
 * @param   units               The delay units - microseconds, milliseconds or seconds. 
 * @param [out]  stopTime       Pointer to marker, that will hold stop time. Use it with ipa_timeout_is_expired
 */
void ipa_timeout_calc(uint32 delay, os_time_units_e units, ipa_time_t* stopTime);

/**
 * @brief   Checks whether calculated delay timeout has expired. 
 *
 * @param   stopTime    Stop time marker obtained from ipa_timeout_calc. 
 *
 * @return  true if timer has expired, false otherwise. 
 */
boolean ipa_timeout_is_expired(const ipa_time_t* stopTime);

/**
 * @fn   irq_resource_h ipa_irq_resource_alloc()
 *
 * @brief   Allocates an interrupt resource. 
 *
 * @return  null if it fails, else the interrupt resource handle. 
*/
irq_resource_h ipa_irq_resource_alloc(void);

/**
 * @fn   void ipa_irq_resource_free(irq_resource_h interruptHandle)
 *
 * @brief   Frees the interrupt handle. 
 *
 * @param   interruptHandle   Handle of the interrupt. 
*/
void ipa_irq_resource_free(irq_resource_h interruptHandle);

/**
 * @fn   int ipa_os_interrupt_setup(uint32 res, void* priv, interrupt_handler isr_func,
 * void *isr_arg, thread_func dsr_func, void *dsr_arg)
 *
 * @brief   IPA operating system interrupt setup.
 * 
 * Starts interrupt service for a given interrupt resource. Supports chaining of multiple
 * interrupt service routines for the same interrupt resource. 
 *
 * @param   res               The interrupt resource. 
 * @param [in]   priv         If non-null, the private context of the interrupt. 
 * @param   isr_func          The ISR func. 
 * @param [in,out]   isr_arg  If non-null, the isr argument. 
 * @param   dsr_func          The DSR func. 
 * @param [in,out]   dsr_arg  If non-null, the dsr argument. 
 *
 * @return  0 - success, otherwise - failure. 
*/
int ipa_os_interrupt_setup(uint32 res, void* priv,
                             interrupt_handler isr_func, void *isr_arg,
                             thread_func dsr_func, void *dsr_arg);

/**
 * @fn   int ipa_os_interrupt_teardown(uint32 res, void* priv)
 *
 * @brief   IPA operating system interrupt tear down.
 * 
 * Stops interrupt service for a given interrupt resource. Supports chaining of multiple
 * interrupt service routines for the same USB core interrupt. 
 *
 * @param        res   The interrupt resource. 
 * @param [in]   priv  If non-null, the private context of the interrupt. 
 *
 * @return  0 - success, otherwise - failure. 
*/
int ipa_os_interrupt_teardown(uint32 res, void* priv);

/**
 * @brief   IPA project specific random function for seed initialization. 
 *
 * @param   seed  The seed. 
*/
void os_srand( uint32 seed );

/**
 * @brief   IPA project specific random function. 
 * @note    Primitive Polynomial P(z) = x^32 + x^30 + x^11 + x^5 + 1, with Period (2^32 - 1)
 *
 * @return  The next pseudo random 32-bit number in the range [0, {2^32 - 1}] . 
*/
uint32 os_rand(void);

/**
 * @brief   Returns the current state of the os_rand() function
 *
 * @return  state of the os_rand() function
*/
uint32 os_get_rand_state(void);

/**
 * @brief   Sets the current state of the os_rand() function.
 *
 * @notice  This function designed to be used only for a code debugging, 
 *          and SHOULD NOT be used for other purposes
 *
 * @param   osRandState   The state of pseudo-random generator, see os_rand()
*/
void os_set_rand_state(uint32 osRandState);

/**
 * @brief   Initialization the rand_state storage machanism.
 *
*/
void os_rand_state_storage_init(void);

/**
 * @brief   Store current state of the os_rand() function.
 *
 * @note    Cannot be called more than IPA_RAND_STATE_STORAGE_SIZE
 * @note    Define IPA_RAND_STATE_STORAGE_DISABLE to block the storing/restoring mechanism
 *
 * @return  return 0 (dummy value), to allow the function usage before any declaration. 
*/
int os_store_rand_state(void);

/**
 * @brief   Restore previous stored state of the os_rand() function.
 * @note    Define IPA_RAND_STATE_STORAGE_DISABLE to block the storing/restoring mechanism
 *
*/
void os_restore_rand_state(void);

/**
 * @brief   Returns TRUE if the function os_set_rand_state was called (rand-debug-mode), and FALSE - otherwise.
 *
 * @return  debug mode flag
*/
boolean os_is_rand_dbg_mode(void);

/**
 * @brief   Returns the IPA's instance of the pseudo-random generator
 *
 * @return  The handle of a pseudo-random numbers generator
*/
VVUTIL_rg_instanceHandle os_get_ipa_rg_instance(void);

/**
 * @brief   Set level for print client. 
 *
 * @param   client    print client. 
 * @param   level     print level.    
 *
 * @return  0  if successful, else failed.
*/
int ipa_os_set_print_level(IPA_OS_PrintClientType client, IPA_OS_PrintLevelType level);

/**
 * @brief   Send print message. 
 *
 * @param   client     print client. 
 * @param   level      print level.   
 * @param   format     pointer to a null-terminated character string specifying how to interpret the data. 
 *
 * @return  0  if successful, else failed.
*/
int ipa_os_print(IPA_OS_PrintClientType client, IPA_OS_PrintLevelType level, char * format, ...);


/**
 * @brief  Stores(Sets) the seed which is used for IPA suite. 
 *
 * @param  seed   the seed of IPA's test. 
*/
void ipa_os_set_seed(uint32 seed);

/**
 * @brief   Returns the seed used for IPA suite. 
 *
 * @return  seed. 
*/
uint32 ipa_os_get_seed(void);

/**
 * @brief   Disable an interrupt line at the controller level
 *
 * @param   int_source        Number of interrupt line
 *
 * @return  TRUE upon success, otherwise an error. 
*/
boolean os_interrupt_disable(uint32 int_source);

/**
 * @brief   Enable an interrupt line at the controller level
 *
 * @param   int_source        Number of interrupt line
 *
 * @return  TRUE upon success, otherwise an error. 
*/
boolean os_interrupt_enable(uint32 int_source);

/** 
 * @brief   Corrupt (XOR 0xFFFFFFFF) the random value of a single instance,  
 *          Repeat random test many times with same seed. 
 *          Each random test will corrupt and different random instance 
 */ 
void ipa_os_corrupt_rand_counter(void); 
#ifdef GSI_OS_LOCAL_WRAPPER

#ifdef WIN32
#define GSI_OS_SWITCH_THREAD(gsi)                           ipa_sleep_us(1)
#else
#define GSI_OS_SWITCH_THREAD(gsi)
#endif
#define GSI_OS_TIMER_DEFINE(idx)                            ipa_time_t __tmrDelta##idx
#define GSI_OS_TIMER_START(idx, timeOutMs)                  ipa_timeout_calc(timeOutMs, T_MSEC, &__tmrDelta##idx)
#define GSI_OS_TIMER_IS_EXPIRED(idx)                        ipa_timeout_is_expired(&__tmrDelta##idx)
#define GSI_OS_REGISTER_ISR(intNum, isrRoutine, hGsi, cfg)  ipa_gsi_interrupt_register(intNum, isrRoutine, hGsi, cfg)
#define GSI_OS_UNREGISTER_ISR(intNum)                       ipa_gsi_interrupt_unregister(intNum)
#define GSI_OS_DMA_MEM_ALLOC(sz, align, bufStruct, phmem)   ipa_gsi_dma_alloc(sz, align, &(bufStruct)->baseVirt, (void**)&(bufStruct)->basePhys, phmem)
#define GSI_OS_DMA_MEM_FREE(hMem)                           ipa_gsi_dma_free(&hMem)
#define GSI_OS_MALLOC(sz)                                   ipa_alloc(sz, 0)
#define GSI_OS_MFREE(hMem)                                  ipa_free_vi(hMem)

boolean ipa_gsi_interrupt_register(uint32 intNum, deo_isr isrRoutine, void *hGsi, void* cfg);
void ipa_gsi_interrupt_unregister(uint32 intNum);
boolean ipa_gsi_dma_alloc(uint32 size, uint32 align, void **vaddr, void **paddr, void **handle);
void ipa_gsi_dma_free(void **handle);
#endif /* GSI_OS_LOCAL_WRAPPER */

#endif /* IPA_PORT_H */

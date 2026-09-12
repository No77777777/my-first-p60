/*!
  @file
  vmem.h

  @brief
  Modem virtual memory manager.

*/

/*===========================================================================

  Copyright (c) 2019 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

  $Header: //components/rel/mcs.mpss/5.2/api/vmem.h#1 $
===========================================================================*/

#ifndef VMEM_H
#define VMEM_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*! @brief VMEM API version. Compare this to vmem_get_api() to ensure
    that VMEM API is in sync.
*/
#define VMEM_API_VERSION                   (0x100)


/*! @brief Priorities for TCM and overlay allocation.
    Whole range is valid.
    MIN/MAX: The absolute bounds of the priority value.
    LOWEST/HIGHEST: The bounds of "dynamic" allocation. This range may get
                    evicted by higher priority.
    UNALLOCATED: For overlays, indicates not allocated in TCM, but accessible from DDR.
                 For allocation manager, no special meaning, just lowest priority.
    LOCKED: Permanently allocated, can never be evicted.
*/
#define VMEM_PRIORITY_MIN                  0
#define VMEM_PRIORITY_UNALLOCATED          0
#define VMEM_PRIORITY_LOWEST               1
#define VMEM_PRIORITY_HIGHEST              254
#define VMEM_PRIORITY_LOCKED               255
#define VMEM_PRIORITY_MAX                  255


/*! @brief A value which never corresponds to a valid address.
*/
#define VMEM_INVALID_ADDR            (0)

/*! @brief Minimum page size for TCM allocations. */
#define VMEM_TCM_MIN_PAGE_SIZE       (16*1024)

/*! @brief Maximum page size for TCM allocations. */
#define VMEM_TCM_MAX_PAGE_SIZE       (64*1024)


/*! @brief VMEM overlay options.
    Use vmem_overlay_opt_init() to initialize this structure before
    overriding any elements.
*/
typedef struct
{
  uint8 priority;     /*!< Priority of overlay (VMEM_PRIORITY_x).
                           UNALLOCATED: Overlay will never be loaded in TCM.
                           LOCKED: Overlay will always be loaded in TCM. Error
                                   if allocation fails.
                           LOWEST..HIGHEST: May be loaded or evicted opportunistically.
                           Writable overlays should only use LOCKED and UNALLOCATED,
                           since dynamic loading/unloading during access will
                           cause TLB exceptions.
                           Default is VMEM_PRIORITY_UNALLOCATED.
                       */
  boolean immediate;  /*!< If true, perform and loading/unloading immediately,
                           within the context of the overlay_config call.
                           Typically LOCKED priority should always set immediate,
                           to ensure that allocation errors are detected in caller
                           context instead of the background thread.
                           Default is FALSE.
                       */
} vmem_overlay_opt_t;


/*! @brief TCM Manager options.
    Use vmem_tcm_opt_init() to initialize this structure before overriding
    any elements.
    See vmem_alloc_opt_t for descriptions.
*/
typedef struct
{
  uint8                  priority;
  boolean contiguous;  /*!< For region allocation only.
                            If TRUE, allocation must be physically contiguous.
                            If FALSE, may be fragmented in physical memory. */
  uint32                 addr;
  void                   (*evict_cb)(uint32 addr,uint32 size, void *arg);
  void                   *evict_arg;

} vmem_tcm_opt_t;



/*! @brief Allocation manager options.
    Use vmem_alloc_opt_init() to initialize this structure before overriding
    any elements.
*/
typedef struct
{
  uint8 priority;     /*!< Priority of allocation (VMEM_PRIORITY_x).
                           LOCKED: Page can never be evicted.
                           MIN..HIGHEST: May be evicted if necessary to satisfy
                           a higher priority request.
                           UNALLOCATED: No special meaning, just minimum priority.
                           Default is LOCKED.
                       */
  uint32 addr;        /*!< Preferred address for allocation. If address is available
                           and unallocated or lower priority, it is allocated.
                           If already allocated at higher priority, allocation
                           will fail.
                           If option priority=MIN/UNALLOCATED, then the preferred
                           allocation will only succeed if the address is unallocated.
                           Set to VMEM_INVALID_ADDR to select any address.
                           Default is VMEM_INVALID_ADDR.
                       */
  void (*evict_cb)(uint32 addr, uint32 size, void *arg);
                      /*!< A callback function to execute if the page is evicted.
                           Optional only if priority=LOCKED, otherwise must be
                           non-NULL.
                           Callback will be executed with the address and size
                           of the required region, not necessarily the add/size
                           of the original allocation (but will overlap with
                           the original allocation).
                           Default is NULL.
                       */
  void *evict_arg;    /*!< An arbitrary argument to pass to the eviction callback,
                           for client context.
                       */
} vmem_alloc_opt_t;


/*! @brief Return structure for allocation manager query. */
typedef struct
{
  uint32 addr;    /*!< Address of start of page. */
  uint32 size;    /*!< Size of page */
  uint8 priority; /*!< Priority of page */
  boolean is_allocated;  /*!< TRUE if page is allocated, FALSE if free */

} vmem_alloc_query_t;

/*! @brief Return structure for overlay query. */
typedef struct
{
  uint8 priority;          /*!< Priority of overlay */
  uint32 page_size;        /*!< Minimum page size, in bytes */
  uint16 num_pages;        /*!< Number of minimum-size pages in region */
  uint16 num_pages_loaded; /*!< Number of minimum-size pages loaded in region */
  uint64 pages_loaded;     /*!< Bitmask of pages loaded */

} vmem_overlay_query_t;
/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif


extern void vmem_init(void);

extern uint32 vmem_get_api(void);


extern uint64 vmem_virt2phys
(
  const void *vaddr
);

extern uint64 vmem_try_virt2phys
(
  const void *vaddr
);


extern void vmem_overlay_opt_init
(
  vmem_overlay_opt_t *opt
);

extern void vmem_overlay_config
(
  void *virt_start,
  void *virt_end,
  vmem_overlay_opt_t *opt
);

/*! @brief Wrapper for vmem_overlay_config with just the overlay name. */
#define VMEM_OVERLAY_CONFIG(_name,_opt) do { \
  extern uint8 __##_name##_start__[]; \
  extern uint8 __##_name##_end__[]; \
  vmem_overlay_config(__##_name##_start__, __##_name##_end__, _opt); \
} while (0)

/*! @brief Wrapper for vmem_overlay_config which does a simple priority change. */
#define VMEM_OVERLAY_CONFIG_PRIORITY(_name,_priority) do { \
  vmem_overlay_opt_t _opt; \
  vmem_overlay_opt_init(&_opt); \
  _opt.priority = _priority; \
  VMEM_OVERLAY_CONFIG(_name,&_opt); \
} while (0)

extern void vmem_overlay_log_snapshot ( void );

extern void vmem_overlay_query
(
  void *virt_start,
  void *virt_end,
  vmem_overlay_query_t *output
);

/*! @brief Wrapper for vmem_overlay_config with just the overlay name. */
#define VMEM_OVERLAY_QUERY(_name,_opt) do { \
  extern uint8 __##_name##_start__[]; \
  extern uint8 __##_name##_end__[]; \
  vmem_overlay_query(__##_name##_start__, __##_name##_end__, _opt); \
} while (0)

extern void vmem_tcm_opt_init
(
  vmem_tcm_opt_t *opt
);

extern uint32 vmem_tcm_request_page
(
  uint32 size,
  const vmem_tcm_opt_t *opt
);

extern void vmem_tcm_release_page
(
  uint32 addr
);

extern void vmem_tcm_set_page_priority
(
  uint32 addr,
  uint8 priority
);

extern void vmem_tcm_atomic_start(void);
extern void vmem_tcm_atomic_end(void);

extern void vmem_tcm_log_snapshot( void );

extern struct vmem_alloc_ctl_s *vmem_alloc_create
(
  uint32 min_page_size,
  uint32 max_page_size
);

extern void vmem_alloc_add_region
(
  struct vmem_alloc_ctl_s *ctl,
  uint32 start_addr,
  uint32 end_addr
);

extern void vmem_alloc_opt_init
(
  vmem_alloc_opt_t *opt
);

extern uint32 vmem_alloc_request
(
  struct vmem_alloc_ctl_s *ctl,
  uint32 size,
  const vmem_alloc_opt_t *opt
);

extern void vmem_alloc_release
(
  struct vmem_alloc_ctl_s *ctl,
  uint32 addr
);

extern void vmem_alloc_set_priority
(
  struct vmem_alloc_ctl_s *ctl,
  uint32 addr,
  uint8 priority
);

extern void vmem_alloc_query_page
(
  struct vmem_alloc_ctl_s *ctl,
  uint32 addr,
  vmem_alloc_query_t *output
);

extern void vmem_alloc_log_snapshot
(
  struct vmem_alloc_ctl_s  *ctl,
  uint8                     type
);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif /* VMEM_H */

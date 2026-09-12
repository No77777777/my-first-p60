/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                                  D S M _ M E M. C

GENERAL DESCRIPTION
  DMSS Data Services memory pool module
  Implementation of allocation and freeing of memory for DSM.

EXTERNALIZED FUNCTIONS

  dsm_free_buffer()
    Return a buffer item to the free queue.

  dsm_free_packet()
    Return the chain of buffer items to the free queue

  dsm_length_packet()
    Return the number of payload bytes in the passed packet buffer chain.

  dsm_offset_new_buffer()
    Return a pointer to the next free item from the pool of free items.
    Moves data pointer of DSM item to specified offset.

  dsm_new_buffer()
    Return a pointer to the next free item from the specified queue.

INITIALIZATION AND SEQUENCING REQUIREMENTS

  dsm_init() must be called prior to any other DSM function.

Copyright (c) 2013 - 2020 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/core.mpss/10.0/memory/dsm/src/dsm_mem.c#5 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/22/16    sko    DSM Library to support any recycle count as low as 1, and call correct error fatal
08/07/13    sko    DSM Augment feature,DSM memory allocation from external memory
08/07/13    sko    Split dsm.c and dsm_mem.c, original file history in dsm.c
04/06/12    sko    Added FEATURE_DSM_LOCKLESS implementation
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Target-independent Include files */
#include "core_variation.h"
#include "comdef.h"
#include "queue_services.h"
#include "dsm_item.h"
#include "dsmi.h"
#include "dsm_lock.h"
#include "msg.h"
#include "err.h"
#include "memory.h"
#include "amssassert.h"
#include <atomic_ops.h>
#include <qurt.h>

#ifdef FEATURE_DSM_NATIVE_ASSERT
#define ASSERT( cond )      assert( cond )
#endif

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/
#define DSM_DS_SL_ITEM_POOL ((dsm_mempool_id_type)(&dsm_ds_sl_item_pool))
extern dsm_pool_mgmt_table_type dsm_ds_sl_item_pool;

#ifdef FEATURE_DSM_AUGMENT

extern dsm_augment_pool_data_type dsma_aug_pools[DSMI_MAX_AUG_POOLS];
extern uint32 dsma_ext_memory_alloc_upper_limit;
static inline void dsma_trigger_task(
  dsm_augment_pool_data_type *aug_data);

void dsma_fill_single_alloc_buffer(
        dsm_augment_pool_data_type *aug_data);

#endif /* FEATURE_DSM_AUGMENT */


#ifdef FEATURE_DSM_LOCKLESS
static inline dsm_item_type * dsmi_atomic_get_item(
        dsm_item_type **top_free_item_ptr);
static inline void dsmi_atomic_free_item(
        dsm_item_type **top_free_item_ptr,
        dsm_item_type **item_ptr,
        void **free_item_ptr);
static inline int32 dsmi_atomic_dec_if_gt_val(
        uint32 *value_ptr,
        uint32 dec_value,
        int32 check_value);
static inline uint32 dsmi_atomic_set_if_lt(
        uint32 *value_ptr,
        uint32 value);

#endif /* FEATURE_DSM_LOCKLESS */
#define DSM_NEWBUF_ENTRIES_BITS  10
#define DSM_NEWBUF_ENTRIES_MASK  ((1<<DSM_NEWBUF_ENTRIES_BITS)-1)
#define DSM_NEWBUF_ENTRIES_MAX   (1<<DSM_NEWBUF_ENTRIES_BITS) /**< No. of log entries */
typedef struct sl_new_buffers_stats{
	uint32 				num;
    const char          *file;
    uint32              line;	
	uint32				free_count;
	uint32				timetick;
} max_sl_new_buffers_stat_t;
static max_sl_new_buffers_stat_t max_sl_new_buffers_req[DSM_NEWBUF_ENTRIES_MAX];

static unsigned int dsm_newbuf_idx;
#ifndef TEST_FRAMEWORK
static unsigned int get_qtimer_32( void )
{
   unsigned int ret;
   asm volatile (" %0 = c30 " : "=r"(ret));
   return ret;
}
#endif
static void dsm_newbuf_update(
            uint32 new_req_size,
            const char * file,
            int line
            )
{

#ifndef TEST_FRAMEWORK    
    static atomic_word_t dsm_newbuf_atomic_idx = ATOMIC_INIT(0);
    unsigned int newbuf_idx;
	newbuf_idx = atomic_inc_return(&dsm_newbuf_atomic_idx)& DSM_NEWBUF_ENTRIES_MASK ;
	dsm_newbuf_idx = newbuf_idx;
    max_sl_new_buffers_req[dsm_newbuf_idx].free_count= dsm_ds_sl_item_pool.free_count;
    max_sl_new_buffers_req[dsm_newbuf_idx].num = new_req_size;
    max_sl_new_buffers_req[dsm_newbuf_idx].file = file;
    max_sl_new_buffers_req[dsm_newbuf_idx].line = (uint32)line;
	max_sl_new_buffers_req[dsm_newbuf_idx].timetick = get_qtimer_32();
#endif    
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                           EXTERNALIZED FUNCTIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

/*===========================================================================
FUNCTION DSMI_ITEM_RESET()

DESCRIPTION
  This resets a DSM item back to its starting state.

DEPENDENCIES
  None

PARAMETERS
  item_ptr - A pointer to an item to be reset

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsmi_item_reset
(
  dsm_item_type * item_ptr
)
{
  /* set the following fields to 0*/
  q_link_s_i( &item_ptr->link );
  
  item_ptr->used = 0;
  item_ptr->dsm_pkt_len = 0;
  item_ptr->app_ptr = 0;
  item_ptr->pkt_ptr = 0;
  item_ptr->dup_ptr = 0;
  item_ptr->app_field = 0;
  item_ptr->kind = 0;
  item_ptr->pkt_bit_field.pkt_bit_field = 0;
  item_ptr->size   = DSM_POOL_ITEM_SIZE(item_ptr->pool_id);
  item_ptr->data_ptr = DSMI_ITEM_HEAD(item_ptr);  
  dsm_item_set_usr_data(item_ptr,0);
}

/*===========================================================================
FUNCTION DSMI_ITEM_RESET_INT()

DESCRIPTION
  This resets a DSM item back to its starting state.

DEPENDENCIES
  None

PARAMETERS
  item_ptr - A pointer to an item to be reset

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static inline void dsmi_item_reset_int
(
  dsm_item_type * item_ptr
)
{
  /* set the following fields to 0*/
  q_link_s_i( &item_ptr->link );
  
  item_ptr->used = 0;
  item_ptr->dsm_pkt_len = 0;
  item_ptr->app_ptr = 0;
  item_ptr->pkt_ptr = 0;
  item_ptr->dup_ptr = 0;
  item_ptr->app_field = 0;
  item_ptr->kind = 0;
  item_ptr->pkt_bit_field.pkt_bit_field = 0;
  item_ptr->size   = DSM_POOL_ITEM_SIZE(item_ptr->pool_id);
  item_ptr->data_ptr = DSMI_ITEM_HEAD(item_ptr);  
  dsm_item_set_usr_data(item_ptr,0);
}

/*===========================================================================
FUNCTION DSMI_ITEM_INIT()

DESCRIPTION
  This inits DSM item, with header and cookie values
  for first time augment allocation.

DEPENDENCIES
  None

PARAMETERS
  item_ptr - A pointer to an item to be reset
  table - Pointer to dsm pool

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static inline void dsmi_item_init
(
  dsm_item_type * item_ptr,
  dsm_pool_mgmt_table_type* table
)
{
  item_ptr->pool_id = (dsm_mempool_id_type)table;

  dsmi_item_reset_int(item_ptr);
#ifdef FEATURE_DSM_MEM_CHK
#ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
  if( DSM_Fill_AA != 0 )
  {
    memset(item_ptr->data_ptr, 0xAA, item_ptr->size);
  }
#endif
  *(uint32 *)(void *)&(item_ptr->data_ptr[item_ptr->size]) = DSM_TAIL_COOKIE;
#endif /* FEATURE_DSM_MEM_CHK */
}

/*===========================================================================
FUNCTION DSMI_UPDATE_CB_LEVELS()

DESCRIPTION
  Update CB levels for the pool
  
DEPENDENCIES
  None.

PARAMETERS
  pool_id - Pool from which new item should be allocated/freed.
  level - Next MEM event callback level/table index

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsmi_update_cb_levels(
    dsm_pool_mgmt_table_type*table,
    dsm_mem_level_enum_type level)
{
    int free_level;
    
    table->next_level = level;
    if ((level > DSM_MEM_LEVEL_MIN) && (level < DSM_MEM_LEVEL_MAX))
    {
      table->alloc_check_free_count_level = table->mem_event_cb_list[table->next_level].avail_item_count;
    }
    else
    {
      table->alloc_check_free_count_level = 0;
    }
    free_level = (int)(table->next_level) + 1;
    
    /* Start checking from the next higher level for free*/
    if (free_level < (int)DSM_MEM_LEVEL_MAX &&
          table->mem_event_cb_list[free_level].level !=
            DSM_MEM_LEVEL_INVALID )
    {
        table->free_check_free_count_level = table->mem_event_cb_list[free_level].avail_item_count;
    }
    else
    {
       table->free_check_free_count_level = 0;
    }

}



/*===========================================================================
FUNCTION DSMI_INVOKE_CB()

DESCRIPTION
  Perform free pool flow management operations. For alloc/free, if the free count has 
  dropped/reached  the specified count, all the  callbacks are called. 
  
DEPENDENCIES
  None.

PARAMETERS
  pool_id - Pool from which new item should be allocated/freed. 
  free_count - Pool count after alloc/free
  IsAlloc - allocation - TRUE, free -FALSE

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void dsmi_invoke_cb
(
  dsm_mempool_id_type pool_id,
  int free_count,
  boolean IsAlloc
)
{
    dsm_pool_mgmt_table_type *table; /* table pointer                      */    
    int next_mem_level;              /* int to hold the next memory level  */
    uint16  opposite_level_index;
    
    table = (dsm_pool_mgmt_table_type*)pool_id;
    
    if (IsAlloc)
    {    
      /*-----------------------------------------------------------------------
           Perform free pool flow management operations. If the free count has 
           dropped to the count specified by the next_level entry, we call all the 
           callbacks for which avail_item_count is same as free_count. We modify 
           next_level such that it points to the highest level which has 
           avail_item_count less than the free_count. 
           -----------------------------------------------------------------------*/
      /* FEATURE_DSM_LOCKLESS - Need to check the condition here again under the lock */
      next_mem_level = (int)(table->next_level);

      while (next_mem_level > (int)DSM_MEM_LEVEL_MIN && next_mem_level < (int)DSM_MEM_LEVEL_MAX && 
             free_count < 
             table->mem_event_cb_list[next_mem_level].avail_item_count)
      {
        
        dsmi_update_cb_levels(table,(dsm_mem_level_enum_type)(next_mem_level-1));        
            
        if (table->mem_event_cb_list[next_mem_level].mem_new_event_cb != 
            NULL)   
        {
          if (table->mem_event_cb_list[next_mem_level]
             .opposite_level != DSM_MEM_LEVEL_INVALID)
          {
          /*-------------------------------------------------------------------  
                  If the is_optimized_callback for next_mem_level is TRUE, set its 
                  callback to FALSE. Set the is_optimized_callback to TRUE for its
                  corresponding opposite level. In Optimized callback, a level's 
                  callback function is only called if its corresponding opposite
                  level's callback function was called last. Else we wait till the
                  corresponding opposite level function has been called.  
                  ---------------------------------------------------------------------*/

          if (table->mem_event_cb_list[next_mem_level].is_optimized_callback)   
          {
            table->mem_event_cb_list[next_mem_level].is_optimized_callback =
              FALSE;
            opposite_level_index = table->mem_event_cb_list[next_mem_level]
              .opposite_level_index;   
            table->mem_event_cb_list[opposite_level_index]
              .is_optimized_callback =  TRUE;
                 table->mem_event_cb_list[next_mem_level]
              .mem_new_event_cb(pool_id,
                                table->mem_event_cb_list[next_mem_level].level,
                                DSM_MEM_OP_NEW);
            table->mem_event_cb_list[next_mem_level].cb_count++;
			if(table == &dsm_ds_sl_item_pool)
			{
				dsm_debug_event_entry(next_mem_level,table->mem_event_cb_list[next_mem_level].cb_count);
			}
#ifdef FEATURE_DSM_AUGMENT
            if (table->pool_stats)
            {
              if (table->augment_data)
              {
                if (free_count <table->pool_stats->stats_free_count_at_lowest_cb_level)
                {
                    table->pool_stats->stats_free_count_at_lowest_cb_level  = free_count;
                    table->pool_stats->stats_item_count_at_lowest_cb_level = table->item_count;
                }
              }
            }
#endif /* FEATURE_DSM_AUGMENT */
            MSG_HIGH_DSM_CONFIG("DSM POOL CB ALLOC pool 0x%x levl %d free %d ",
                              table, next_mem_level,table->free_count); 
          

          }
        }
        else
        {
          table->mem_event_cb_list[next_mem_level]
            .mem_new_event_cb(pool_id,
                              table->mem_event_cb_list[next_mem_level].level,
                              DSM_MEM_OP_NEW);
          table->mem_event_cb_list[next_mem_level].cb_count++;		  
		  if(table == &dsm_ds_sl_item_pool)
		  {
			  dsm_debug_event_entry(next_mem_level,table->mem_event_cb_list[next_mem_level].cb_count);
		  }
#ifdef FEATURE_DSM_AUGMENT
          if (table->pool_stats)
          {
            if (table->augment_data)
            {
              if (free_count <table->pool_stats->stats_free_count_at_lowest_cb_level)
              {
                  table->pool_stats->stats_free_count_at_lowest_cb_level  = free_count;
                  table->pool_stats->stats_item_count_at_lowest_cb_level = table->item_count;
              }
            }
          }
#endif /* FEATURE_DSM_AUGMENT */
        }
      }
      next_mem_level = next_mem_level - 1;  
    }
  }
  else /* for a free call */
  {
    /* Start checking from the next higher level */
    
    /*---------------------------------------------------------------------
      Note that the table->next_level is the highest level for which 
      avail_item_count is less than free_count. Since we are interested in 
      the lowest level for which avail_item_count is more than free_count
      for free operation, we use table->next_level + 1.
    ---------------------------------------------------------------------*/
    
    /*---------------------------------------------------------------------
      Perform free pool flow management operations. If the free count has 
      reached the count specified by the next_level entry, we call all the 
      callbacks with avail_item_count same as free_count. We modify 
      next_level such that it points to the highest level which has 
      avail_item_count less than free_count. 
    ---------------------------------------------------------------------*/
    next_mem_level = (int)(table->next_level) + 1;
    
    while (next_mem_level > (int)DSM_MEM_LEVEL_MIN && next_mem_level < (int)DSM_MEM_LEVEL_MAX && 
         table->mem_event_cb_list[next_mem_level].level !=
         DSM_MEM_LEVEL_INVALID &&
         free_count >=
         table->mem_event_cb_list[next_mem_level].avail_item_count)
    {
      dsmi_update_cb_levels(table,(dsm_mem_level_enum_type)next_mem_level); 
      if (table->mem_event_cb_list[next_mem_level].mem_free_event_cb != 
          NULL)
      {
        if (table->mem_event_cb_list[next_mem_level]
           .opposite_level != DSM_MEM_LEVEL_INVALID)
        {
        /*--------------------------------------------------------------------  
              If the is_optimized_callback for next_mem_level is TRUE, set its 
              callback to FALSE. Set the is_optimized_callback to TRUE for its
              corresponding opposite level. In Optimized callback, a level's 
              callback function is only called if its corresponding opposite 
              level's callback function was called last. Else we wait till the 
              corresponding opposite level function has been called.  
             --------------------------------------------------------------------*/
          if (table->mem_event_cb_list[next_mem_level].is_optimized_callback)
          {
            table->mem_event_cb_list[next_mem_level].is_optimized_callback = 
              FALSE;
            opposite_level_index = table->mem_event_cb_list[next_mem_level]
              .opposite_level_index;
            table->mem_event_cb_list[opposite_level_index]
              .is_optimized_callback =  TRUE;

            table->mem_event_cb_list[next_mem_level].mem_free_event_cb(
              pool_id,
              table->mem_event_cb_list[next_mem_level].level,
              DSM_MEM_OP_FREE);
            table->mem_event_cb_list[next_mem_level].cb_count++;			
			if(table == &dsm_ds_sl_item_pool)
			{
				dsm_debug_event_entry(next_mem_level,table->mem_event_cb_list[next_mem_level].cb_count);
			}
            MSG_HIGH_DSM_CONFIG("DSM POOL CB FREE pool 0x%x level %d free %d ",
                table,next_mem_level,table->free_count);
    
          }
        }
        else
        {
          table->mem_event_cb_list[next_mem_level].mem_free_event_cb(
            pool_id,
            table->mem_event_cb_list[next_mem_level].level,
            DSM_MEM_OP_FREE);
          table->mem_event_cb_list[next_mem_level].cb_count++;
			if(table == &dsm_ds_sl_item_pool)
			{
				dsm_debug_event_entry(next_mem_level,table->mem_event_cb_list[next_mem_level].cb_count);
			}
        }
      }
      next_mem_level = next_mem_level + 1;
    } 
  }
}


#ifdef FEATURE_DSM_LOCKLESS
#ifdef FEATURE_DSM_AUGMENT

/*===========================================================================
FUNCTION dsmi_atomic_augbuffer_free_item()

DESCRIPTION
  Frees item to augment buffer, updating count

DEPENDENCIES
  Atomic operations support for 64 bits.

PARAMETERS
    free_list_ptr - buffer's free list to be updated with free count
                     and top_free_ptr
    item_ptr - Pointer to DSM item
    free_item_ptr - Item_ptr's link to be added to pool's free list

RETURN VALUE
  Free item count in buffer after freeing

SIDE EFFECTS
  None
===========================================================================*/
static inline
uint32 dsmi_atomic_augbuffer_free_item
(
 uint64* free_list_ptr,
 dsm_item_type **item_ptr,
 void **free_item_ptr
)
{
  uint64 temp = 0;
  uint32 free_count;

  asm volatile(
    "1:     %0 = memd_locked(%5)\n\t"
    "       %1 = %L0 \n\t"
    "       %1 = add(%1,#1) \n\t"
    "       memw(%6) = %H0 \n\t" /* item_ptr->app_ptr */
    "       %0 = combine(%7,%1) \n\t"
    "       memd_locked(%5,p0) = %0 \n\t"
    "       if !p0 jump 1b\n\t"
    : "=&r"(temp),"=&r"(free_count),"+m"(free_list_ptr),"+m"(*free_item_ptr)
    : "r"(temp),"r"(free_list_ptr),"r"(free_item_ptr),"r"(*item_ptr)
    : "p0");

    return free_count;
}
/*========================================================================
FUNCTION dsmi_atomic_free_augment_item()

DESCRIPTION
  Adds freed item to  augment allocation head, if the item's pool_id_ext
  matches allocation head's buffer id.

DEPENDENCIES
   Atomic operations support.

PARAMETERS
  top_free_item_ptr - Top Pointer of pool's free list
  item_ptr - Pointer to DSM item
  free_item_ptr - Item_ptr's link to be added to pool's free list
  pool_ext_id - Augment buffer id of freed item

RETURN VALUE
  Boolean to indicate whether the item was added to alloc head

SIDE EFFECTS
  None
=========================================================================*/
static inline
boolean dsmi_atomic_free_augment_item(
dsm_item_type **top_free_item_ptr,
dsm_item_type **item_ptr,
void **free_item_ptr,
uint8 pool_ext_id
)
{
 boolean result;
 dsm_item_type * cur_free_ptr;
 uint32 lpool_ext_id;

 asm volatile(
   "       %2 = #0\n\t"
   "1:     %0 = memw_locked(%5)\n\t"
   "       p0 = cmp.eq(%0,#0)\n\t"
   "       if p0 jump 2f\n\t"
   "       %1 = memub(%0+#40)\n\t" /* item_ptr->pool_id_ext*/
   "       p0 = cmp.eq(%1,%8)\n\t"
   "       if !p0 jump 2f\n\t"
   "       memw(%6) = %0 \n\t" /* *top_free_item_ptr->app_ptr */
   "       memw_locked(%5, p0) = %7\n\t"
   "       if !p0 jump 1b\n\t"
   "       %2 = #1\n\t"
   "2:     \n\t"
   : "=&r"(cur_free_ptr),"=&r"(lpool_ext_id),"=&r"(result),"+m"(*free_item_ptr),"+m"(*top_free_item_ptr)
   : "r" (top_free_item_ptr),"r"(free_item_ptr),"r"(*item_ptr),"r"(pool_ext_id)
   : "p0");


 return result;

}

/*========================================================================
FUNCTION  dsmi_atomic_update_augment_alloc_head()

DESCRIPTION
 If alloc_head is NULL,update it with new_buffer_ptr.

DEPENDENCIES
   Atomic operations support.

PARAMETERS
  top_free_item_ptr - Top free item of allocation head
  new_alloc - New alloc pool_id_ext
  new_buffer_ptr - Top free item of next buffer to be used.
  new_pool_id - new_buffer_ptr->pool_id_ext

RETURN VALUE

SIDE EFFECTS
  None
=========================================================================*/
/* LLVM Bug removed and can use static online here */
static inline   
dsm_item_type* dsmi_atomic_update_augment_alloc_head
(
 dsm_item_type **top_free_item_ptr,
 void **new_alloc,
 dsm_item_type *new_buffer_ptr,
 uint32 new_pool_id,
  dsm_item_type *app_ptr
)
{
  void* next_free_ptr;
  dsm_item_type *result = NULL;

  asm volatile(
    "1:   %1 = #0\n\t"
    "     %0 = memw_locked(%4)\n\t"
    "     p0 = cmp.eq(%0,#0)\n\t"
    "     if !p0 jump 2f\n\t"
    "     %0 = memw(%5+#16)\n\t" /* *top_free_item_ptr =  item_ptr->app_ptr*/
    "     p0 = cmp.eq(%0,%8)\n\t"
    "     if !p0 jump 2f\n\t"
    "     %0 = memw(%8+#36)\n\t" /* *check   app_ptr->references*/
    "     p0 = cmp.eq(%0,#0)\n\t"
    "     if !p0 jump 2f\n\t"
    "     %0 = memw(%8+#16)\n\t" /* *check   app_ptr->app_ptr*/
    "     p0 = cmp.eq(%0,#0)\n\t"    
    "     if p0 jump 2f\n\t"
    "     memw_locked(%4, p0) = %8\n\t"
    "     if !p0 jump 1b\n\t"
    "     memw(%5+#16)=#0\n\t"
    "     memw(%3)=%7\n\t"    
    "     %1=%5\n\t"
    "2:   \n\t"
    : "=&r" (next_free_ptr),"=&r" (result),"+m"(*top_free_item_ptr),"+m"(*new_alloc)
    : "r" (top_free_item_ptr),"r"(new_buffer_ptr),"r"(new_alloc),"r"(new_pool_id),"r"(app_ptr)
    : "p0");

   return result;
}
/*========================================================================
FUNCTION  dsmi_atomic_set_augbuffer_free_items()

DESCRIPTION
  Gets top free item of buffer and sets it to new value

DEPENDENCIES
   Atomic operations support.

PARAMETERS
  free_list_ptr -  pointer to buffer free list
  free_count - Free count to be set
  new_buffer_ptr - Free bufer top item to be set
 

RETURN VALUE
  Current buffer free list head before setting

SIDE EFFECTS
  None
=========================================================================*/
static inline
uint64 dsmi_atomic_set_augbuffer_free_items
(
  uint64 *free_list_ptr,
  uint32 free_count,
  dsm_item_type *new_buffer_ptr
)
{
  uint64 result = 0;
  uint64 temp;

  asm volatile(
      "     %1 = combine(%5,%4) \n\t"
      "1:   %0 = memd_locked(%3)\n\t"
      "     memd_locked(%3, p0) = %1\n\t"
      "     if !p0 jump 1b\n\t"
      "2:\n\t"
      : "=&r" (result),"=&r" (temp),"+m"(free_list_ptr)
      : "r" (free_list_ptr),"r"(free_count),"r"(new_buffer_ptr)
      : "p0");

  return result;
}

/*========================================================================
FUNCTION  dsmi_get_augbuffer_free_items()

DESCRIPTION
  Gets free list of buffer using memd operations

DEPENDENCIES

PARAMETERS
  free_list_ptr -  pointer to buffer free list

RETURN VALUE
  Current buffer free list head

SIDE EFFECTS
  None
=========================================================================*/
static inline
uint64 dsmi_get_augbuffer_free_items
(
  uint64 *free_list_ptr
)
{
  uint64 result = 0;

  asm volatile(
      " %0 = memd(%1)\n\t"
      : "=&r" (result)
      : "r" (free_list_ptr)
      :);

  return result;
}

/*========================================================================
FUNCTION dsma_change_buffers_to_used()

DESCRIPTION
  Function to change buffers to used

DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data

RETURN VALUE
  dsm_item_type* - allocated item

SIDE EFFECTS
=========================================================================*/
static void dsma_change_buffers_to_used(
        dsm_augment_pool_data_type *aug_data)
{
  uint32 i,total;
  i = 0;
  total = aug_data->dbuffer_count;
  while ((total>0) && (i<DSMI_AUGMENT_TOTAL_BLOCKS))
  {
    if (aug_data->buffer[i].dsma_state)
    {
      total--;
      atomic_compare_and_set(&aug_data->buffer[i].dsma_state, DSMA_USED_SPECIAL, DSMA_USED);
    }
    i++;
  }
}

/*========================================================================
FUNCTION dsma_alloc_augment_buffers_few()

DESCRIPTION
  Function to allocate multiple DSM items from reused buffer or recycled buffer

DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data
  num_buffers - Number of DSM Items to be allocated
  pdsm_array - Array that contains allocated items

RETURN VALUE
  Number of items allocated

SIDE EFFECTS
=========================================================================*/
static inline uint32 dsma_alloc_augment_buffers_few(
        dsm_augment_pool_data_type *aug_data,
        uint32 num_buffers,
        dsm_item_type *pdsm_array[],
        const char * file,
        uint32 line )
{
   uint32 allocated = 0;
   dsm_item_type* ldsmitem;
   while(allocated<num_buffers)
   {
      ldsmitem =  dsmi_atomic_get_item(&aug_data->pool->aug_top_free_item);
      if (ldsmitem)
      {
        /* No other checks are necessary
         * reusing items */
        if (ldsmitem->references)
        {
            ERR_FATAL("DSM Library Config Error 6 : Check use after free 0x%x, 0x%x, %d. Contact DSM team",
                                 aug_data->pool,ldsmitem,aug_data->dbuffer_count );
            return 0;
        }
        ldsmitem->references++;
        if (!ldsmitem->app_ptr)
        {
          dsma_fill_single_alloc_buffer(aug_data);
        }
        ldsmitem->app_ptr = 0;
     #ifdef FEATURE_DSM_MEM_CHK
        dsmi_touch_item_int(ldsmitem,file,line);
    #endif
        pdsm_array[allocated]=ldsmitem;
        allocated++;
      }
      else
      {
        break;
      }
   }
   return allocated;
}
/*========================================================================
FUNCTION dsma_alloc_augment_buffers()

DESCRIPTION
  Function to allocate multiple DSM items from reused buffer or recycled buffer

DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data
  num_buffers - Number of DSM Items to be allocated
  pdsm_array - Array that contains allocated items

RETURN VALUE  
  Number of items allocated

SIDE EFFECTS
=========================================================================*/
static inline uint32 dsma_alloc_augment_buffers(
        dsm_augment_pool_data_type *aug_data,
        uint32 num_buffers,
        dsm_item_type *pdsm_array[],
        const char * file,
        uint32 line )
{
  uint32 i,j,total;
  int32 lfreecount;
  dsm_item_type *ldsmitem,*ldsmitem_next;
  uint64 lfree_items;
  uint32 new_count;
  uint32 allocated=0;
  uint32 check_count;

  i = 0;
  total = aug_data->dbuffer_count;
  if(DSM_UNLIKELY((!aug_data->recycle_top_free) && (aug_data->scan_buffer_count)))
  {
      dsma_alloc_dynamic_reuse_recycle_buffer(aug_data);
  }
  check_count = (num_buffers >> 2 );
  if ((check_count < 4) || (!aug_data->scan_buffer_count))
  {
   check_count = 2;
  }

  while ((total>0) && (i<DSMI_AUGMENT_TOTAL_BLOCKS) && (allocated < num_buffers))
  {
    if (aug_data->buffer[i].dsma_state)
    {
     total--;

     if (aug_data->buffer[i].dsma_state == DSMA_USED)
     {
       if (aug_data->buffer[i].dsma_free_list.cnt > check_count)
       {
         if (atomic_compare_and_set(&aug_data->buffer[i].dsma_state, DSMA_USED, DSMA_START_ALLOC))
         {
           lfree_items = dsmi_atomic_set_augbuffer_free_items(&aug_data->buffer[i].free_list,0,0);
           ldsmitem = (dsm_item_type*)((uint32*)&lfree_items)[1];
           lfreecount = ((uint32*)&lfree_items)[0];
           if (lfreecount < (num_buffers-allocated))
           {
               new_count=allocated+lfreecount;
               lfreecount=0;
           }
           else
           {
               new_count=num_buffers;
               lfreecount-=(num_buffers-allocated);
           }
           for (j=allocated;j < new_count ;j++)
           {
             if (DSM_UNLIKELY(ldsmitem->references))
             {
               /* Call error fatal here, before DSM library state is corrupted */
               ERR_FATAL("DSM Library Config Error 2 : Low item count in Pool 0x%x, 0x%x, %d. Contact DSM team",
                 aug_data->pool,ldsmitem,lfreecount );
               return FALSE;
             }
#ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
             /* Costly check for this feature at the time of allocation
             * Items are verified and reset when they are freed or initialized */
             dsmi_verify_buffer(ldsmitem);
             dsmi_item_reset_int(ldsmitem);
#endif /*  FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET */

#ifdef FEATURE_DSM_MEM_CHK
             dsmi_touch_item_int(ldsmitem,file,line);
#endif
             ldsmitem->references++;
             pdsm_array[j] = ldsmitem;
             ldsmitem = ldsmitem->app_ptr;
             pdsm_array[j]->app_ptr = 0;
           }
           allocated=new_count;
           if (lfreecount)
           {
             if (!atomic_compare_and_set(&aug_data->pool->aug_top_free_item, NULL,ldsmitem))
             {
             lfree_items = dsmi_atomic_set_augbuffer_free_items(&aug_data->buffer[i].free_list,lfreecount,ldsmitem);
             ldsmitem_next = (dsm_item_type*)((uint32*)&lfree_items)[1];
             ldsmitem = ldsmitem_next;
             /* Return any items taken additionally */
             while (DSM_UNLIKELY(ldsmitem != NULL))
             {
               ldsmitem_next = (dsm_item_type*)ldsmitem->app_ptr;
               dsmi_atomic_augbuffer_free_item(&aug_data->buffer[i].free_list,&ldsmitem,&ldsmitem->app_ptr);
               ldsmitem = ldsmitem_next;
             }
             }
           }
           aug_data->buffer[i].dsma_state = DSMA_USED;
           if (!aug_data->reuse_trigger_cnt)
           {
             atomic_set(&aug_data->reuse_trigger_cnt,2);
           }
         }
       }
     }
     else
     {
       if (aug_data->buffer[i].dsma_state == DSMA_REUSE)
         {
           if (atomic_compare_and_set(&aug_data->buffer[i].dsma_state, DSMA_REUSE, DSMA_START_ALLOC))
           {
            if(num_buffers >= aug_data->augment_cnt)
            {
              /* Call error fatal here, before DSM library state is corrupted */
              ERR_FATAL("DSM Library Config Error: Bulk alloc %d not supported in Pool 0x%x.Contact DSM team",
                num_buffers,aug_data->pool,0);
              return FALSE;
            }
            /* No need to update pool's free_count */
            ldsmitem = aug_data->buffer[i].dsma_free_list.top_item;
            for (j =allocated; j<num_buffers;j++)
            {
              if (DSM_UNLIKELY(ldsmitem->references))
              {
                /* Call error fatal here, before DSM library state is corrupted */
                ERR_FATAL("DSM Library Config Error 3 : Low item count in Pool 0x%x,0x%x,%d. Contact DSM team",
                     aug_data->pool,ldsmitem,num_buffers);
                return FALSE;
              }
#ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
             /* Costly check for this feature at the time of allocation
                  * Items are verified and reset when they are freed or initialized */
              dsmi_verify_buffer(ldsmitem);
              dsmi_item_reset_int(ldsmitem);
#endif /*  FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET */

#ifdef FEATURE_DSM_MEM_CHK
              dsmi_touch_item_int(ldsmitem,file,line);
#endif
              ldsmitem->references++;
              pdsm_array[j] = ldsmitem;
              ldsmitem = ldsmitem->app_ptr;
              pdsm_array[j]->app_ptr = 0;

            }
            if (!atomic_compare_and_set(&aug_data->pool->aug_top_free_item, NULL,ldsmitem))
            {
              aug_data->buffer[i].dsma_free_list.top_item =  ldsmitem;
              aug_data->buffer[i].dsma_free_list.cnt =  aug_data->augment_cnt- num_buffers+allocated;
            }
            else
            {
                aug_data->buffer[i].free_list=0;
            }
            atomic_set(&aug_data->buffer[i].dsma_state,DSMA_USED);
            atomic_dec(&aug_data->reuse_buffer_cnt);
            atomic_set(&aug_data->reuse_trigger_cnt,3);
            MSG_HIGH_DSM_CONFIG("DSMA bulk alloc buffers %d from reused %d in pool 0x%x",
                    (num_buffers-allocated), i,aug_data->pool);
            allocated=num_buffers;
          }
      }
     }
   }
   i++;
  }

  if (!aug_data->scan_buffer_count)
  {
    if (allocated < num_buffers)
    {
      atomic_set(&aug_data->scan_buffer_count,aug_data->augment_cnt);
    }
    else
    {
      if ((num_buffers > 4) && (aug_data->dbuffer_count > 3))
      {
        atomic_set(&aug_data->scan_buffer_count,DSMA_SCAN_BUFFER_CNT_NORMAL);
      }
    }
  }
  else
  {
      MSG_HIGH_DSM_CONFIG("DSMA bulk alloc buffers %d from recycled %d in pool 0x%x",
              num_buffers, i,aug_data->pool);
  }
  return allocated;
}

/*========================================================================
FUNCTION dsma_fill_single_alloc_buffer()

DESCRIPTION
  Function to allocate DSM item to top_free_item

DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data

RETURN VALUE
  dsm_item_type* - allocated item

SIDE EFFECTS
=========================================================================*/
void dsma_fill_single_alloc_buffer(
        dsm_augment_pool_data_type *aug_data)
{
  uint32 i,total;
  uint64 lfree_items;
  boolean found = FALSE;
  dsm_item_type *ldsmitem = NULL;

  i = 0;
  total = aug_data->dbuffer_count;
  if ((aug_data->scan_buffer_count==aug_data->augment_cnt)
          || (aug_data->pool->aug_top_free_item))
  {
    return;
  }
  while ((total>0) && (i<DSMI_AUGMENT_TOTAL_BLOCKS))
  {
    if (aug_data->buffer[i].dsma_state)
    {
      total--;
      if (aug_data->buffer[i].dsma_state == DSMA_USED)
      {
          if ( (aug_data->buffer[i].dsma_free_list.cnt > 5))
          {
            lfree_items = dsmi_get_augbuffer_free_items(&aug_data->buffer[i].free_list);
            if (atomic_compare_and_set(&aug_data->buffer[i].dsma_state, DSMA_USED, DSMA_START_ALLOC))
            {
              if (atomic_compare_and_set(&aug_data->pool->aug_top_free_item, NULL,7))
              {
                lfree_items = dsmi_atomic_set_augbuffer_free_items(&aug_data->buffer[i].free_list,0,0);
                ldsmitem = (dsm_item_type*)((uint32*)&lfree_items)[1];
                atomic_set(&aug_data->pool->aug_top_free_item,ldsmitem);
                atomic_set(&aug_data->buffer[i].dsma_state, DSMA_USED);
                atomic_set(&aug_data->reuse_trigger_cnt,1);
                found = TRUE;
                break;
              }
              else
              {
                atomic_set(&aug_data->buffer[i].dsma_state,DSMA_USED);
              }
            }
         }
       }
       else if (aug_data->buffer[i].dsma_state == DSMA_REUSE)
       {
         if (atomic_compare_and_set(&aug_data->buffer[i].dsma_state, DSMA_REUSE, DSMA_START_ALLOC))
         {
            lfree_items = dsmi_get_augbuffer_free_items(&aug_data->buffer[i].free_list);
            if (atomic_compare_and_set(&aug_data->pool->aug_top_free_item, NULL,((uint32*)&lfree_items)[1]))
            {
              lfree_items = dsmi_atomic_set_augbuffer_free_items(&aug_data->buffer[i].free_list,0,0);
              atomic_set(&aug_data->buffer[i].dsma_state, DSMA_USED);
              atomic_dec(&aug_data->reuse_buffer_cnt);
              atomic_set(&aug_data->reuse_trigger_cnt,1);
              found = TRUE;
              break;
            }
            else
            {
              atomic_set(&aug_data->buffer[i].dsma_state, DSMA_REUSE);
            }
         }
       }
     }
     i++;
  }
  if (!found)
  {
    dsma_change_buffers_to_used(aug_data);
    if (!aug_data->scan_buffer_count)
    {
      atomic_set(&aug_data->scan_buffer_count,aug_data->augment_cnt);
    }
    else
    {
      atomic_set(&aug_data->scan_buffer_count,0);
    }
  }
}

/*========================================================================
FUNCTION dsma_get_recycle_buffers()

DESCRIPTION
  Fill recycle_top_free from recycle_buffers

DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data

RETURN VALUE  None

SIDE EFFECTS
=========================================================================*/
void dsma_get_recycle_buffers(
        dsm_augment_pool_data_type *aug_data)
{
    uint32 i,buffer;
    uint32 alloc_buffer=0;
    dsm_item_type* recycle_top;

    dsm_item_type* alloc_top = aug_data->alloc_top_free;
    alloc_buffer = DSMA_GET_TOP_FREE_BUFFER_INDEX(alloc_top);
	
    for (i=0; i<3;i++)
    {
      recycle_top = aug_data->recycle_buffers[i];
      if (recycle_top)
      {
        buffer = recycle_top->pool_id_ext-1;
        if((recycle_top->pool_id_ext != alloc_buffer) &&
           (aug_data->buffer[buffer].dsma_state == DSMA_START_ADD_RECYCLE))
        {
          if (atomic_compare_and_set(&aug_data->recycle_top_free, NULL,(uint32)aug_data->recycle_buffers[i]))
          {
            atomic_set(&aug_data->recycle_buffers[i],0);
            atomic_dec(&aug_data->num_recycle_buffers);
            aug_data->buffer[buffer].dsma_state = DSMA_ALLOC_SPECIAL;
            break;
          }
       }
      }
    }

}
/*========================================================================
FUNCTION dsma_add_recycle_buffers()

DESCRIPTION
  Add more recycle buffers for holding on to buffers

DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data
  buffer - Buffer to add

RETURN VALUE  None

SIDE EFFECTS
=========================================================================*/
void dsma_add_recycle_buffers(
        dsm_augment_pool_data_type *aug_data,
        uint32 buffer)
{
    uint32 i,lfree_cnt;
    uint64 recycle_free;

    for (i =0; i<3;i++)
    {
        if (!aug_data->recycle_buffers[i])
        {
          recycle_free = dsmi_get_augbuffer_free_items(&aug_data->buffer[buffer].free_list);
          if((((uint32*)&recycle_free)[0] > aug_data->reuse_cnt)&&
           (((uint32*)&recycle_free)[1]!=0))
          {
            if (atomic_compare_and_set(&aug_data->buffer[buffer].dsma_state, DSMA_USED, DSMA_START_ALLOC))
            {
              if (atomic_compare_and_set(&aug_data->recycle_buffers[i], NULL,((uint32*)&recycle_free)[1]))
              {
                recycle_free = dsmi_get_augbuffer_free_items(&aug_data->buffer[buffer].free_list);
                if (((uint32*)&recycle_free)[0] <= aug_data->reuse_cnt)
                {
                  aug_data->recycle_buffers[i]=0;  
                  aug_data->buffer[buffer].dsma_state = DSMA_USED;
                }
                else
                {
                  recycle_free = dsmi_atomic_set_augbuffer_free_items(&aug_data->buffer[buffer].free_list,0,0);
                  lfree_cnt = ((uint32*)&recycle_free)[0];              
                  aug_data->recycle_buffers[i]=(dsm_item_type*)((uint32*)&recycle_free)[1];                
                  if (!aug_data->recycle_top_free)
                  {
                    if (atomic_compare_and_set(&aug_data->recycle_top_free, NULL,(uint32)aug_data->recycle_buffers[i]))
                    {
                      aug_data->recycle_buffers[i]=0;  
                      aug_data->buffer[buffer].dsma_state = DSMA_ALLOC_SPECIAL;
                    }
                    else
                    {
                      atomic_inc(&aug_data->num_recycle_buffers);
                      aug_data->buffer[buffer].dsma_state = DSMA_START_ADD_RECYCLE;
                    }
                  }
                  else
                  {
                    atomic_inc(&aug_data->num_recycle_buffers);
                    aug_data->buffer[buffer].dsma_state = DSMA_START_ADD_RECYCLE;
                  } 
                  atomic_add(&aug_data->pool->free_count,lfree_cnt);                
                  break;
                }
              }
              else
              {
                aug_data->buffer[buffer].dsma_state = DSMA_USED;
              }
            }
          }
        }
  }

}

/*========================================================================
FUNCTION dsma_alloc_dynamic_reuse_recycle_buffer()

DESCRIPTION
  Traverse through used/reuse buffers and assign to reuse/recycle buffer head

DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data
  
RETURN VALUE  None

SIDE EFFECTS
=========================================================================*/
void dsma_alloc_dynamic_reuse_recycle_buffer(
        dsm_augment_pool_data_type *aug_data)
{
  uint32 i,total,lfree_cnt;
  uint32 alloc_buffer;
  uint64 recycle_free;
  dsm_item_type* alloc_top;

  if (aug_data->num_recycle_buffers)
  {
    dsma_get_recycle_buffers(aug_data);
    return;
  }
  alloc_top = aug_data->alloc_top_free;
  alloc_buffer = DSMA_GET_TOP_FREE_BUFFER_INDEX(alloc_top);

  i = 0;
  total = aug_data->dbuffer_count;
  while ((total>0) && (i < DSMI_AUGMENT_TOTAL_BLOCKS))
  {
    if (aug_data->buffer[i].dsma_state)
    {
     total--;

     if (aug_data->reuse_buffer_cnt)
     {
       if((aug_data->buffer[i].dsma_state == DSMA_REUSE) && ((uint32)aug_data->buffer[i].block<8192))
       {
        if (atomic_compare_and_set(&aug_data->buffer[i].dsma_state, DSMA_REUSE, DSMA_ALLOC))
        {
          if(atomic_compare_and_set(&aug_data->recycle_top_free, NULL, aug_data->buffer[i].dsma_free_list.top_item))
          {
            atomic_add(&aug_data->pool->free_count,aug_data->augment_cnt);
            atomic_dec(&aug_data->reuse_buffer_cnt);
            aug_data->buffer[i].free_list = 0;
            if ((!aug_data->reuse_buffer_cnt) || (aug_data->dynamic_top_free))
            {
              break;
            }
          }
          else if(!aug_data->dsm_task_alloc)
          {
            if (atomic_compare_and_set(&aug_data->dynamic_top_free, NULL, aug_data->buffer[i].dsma_free_list.top_item))
              {
              atomic_add(&aug_data->pool->free_count,aug_data->augment_cnt);
              atomic_dec(&aug_data->reuse_buffer_cnt);
              aug_data->buffer[i].free_list = 0;
              break;
              }
              else
              {
                aug_data->buffer[i].dsma_state = DSMA_REUSE;
              }
          }
          else
          {
            /* Reset it back */
            aug_data->buffer[i].dsma_state = DSMA_REUSE;
          }
        }
       }
     }
     else
     {
       if ((!aug_data->recycle_top_free) && (alloc_buffer != i) &&
               ((uint32)aug_data->buffer[i].block<8192) &&
               (aug_data->buffer[i].dsma_state == DSMA_USED))
       {
         recycle_free = dsmi_get_augbuffer_free_items(&aug_data->buffer[i].free_list);
         if((((uint32*)&recycle_free)[0] > aug_data->reuse_cnt)&&
            (((uint32*)&recycle_free)[1]!=0))
         {
           if (atomic_compare_and_set(&aug_data->buffer[i].dsma_state, DSMA_USED, DSMA_START_SPECIAL))
           {
             if (atomic_compare_and_set(&aug_data->recycle_top_free, NULL,((uint32*)&recycle_free)[1]))
             {
               recycle_free = dsmi_atomic_set_augbuffer_free_items(&aug_data->buffer[i].free_list,0,0);
               lfree_cnt = ((uint32*)&recycle_free)[0];
               atomic_set(&aug_data->recycle_top_free,((uint32*)&recycle_free)[1]);
               atomic_add(&aug_data->pool->free_count,lfree_cnt);
               aug_data->buffer[i].dsma_state = DSMA_ALLOC_SPECIAL;
               break;
             }
             else
             {
               aug_data->buffer[i].dsma_state = DSMA_USED;
             }
           }
         }
        }
       }
     }
    i++;
   }
}

/*========================================================================
FUNCTION DSMA_TRIGGER_TASK()

DESCRIPTION
  This API is used to trigger actions if alloc buffer changes

DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data

RETURN VALUE  None

SIDE EFFECTS - None
=========================================================================*/
static inline void dsma_trigger_task
(
  dsm_augment_pool_data_type *aug_data
)
{
  /*Send signal to task to allocate new item */
  if(DSM_LIKELY(!aug_data->dsm_task_alloc))
  {
    atomic_set(&aug_data->reuse_trigger_cnt,1);
    dsma_alloc_dynamic_reuse_recycle_buffer(aug_data);
    if((!aug_data->dynamic_top_free) &&
        ((aug_data->buffer_available_cnt) || (aug_data->dbuffer_count < aug_data->dbuffer_limit)))
    {
      aug_data->dsm_task_alloc = TRUE;
      dsm_task_augment_alloc_req();
    }
  }
}

/*========================================================================
FUNCTION DSMA_ALLOC_BUFFER_CHANGE_TRIGGER()

DESCRIPTION
  This API is used to trigger actions if alloc buffer changes
  
DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data

RETURN VALUE  None

SIDE EFFECTS - None
=========================================================================*/
static void dsma_alloc_buffer_change_trigger
(
  dsm_augment_pool_data_type *aug_data
)
{
  uint32 alloc_buffer;

  alloc_buffer = dsmi_atomic_set_if_lt(&aug_data->new_alloc,0);
  if(alloc_buffer)
  {
    if (aug_data->recycle_top_free)
    {
      if ((alloc_buffer) == aug_data->recycle_top_free->pool_id_ext)
      {
        if (aug_data->buffer[alloc_buffer-1].dsma_state == DSMA_ALLOC)
        {
          aug_data->buffer[alloc_buffer-1].dsma_state = DSMA_USED;
          MSG_HIGH_DSM_CONFIG("DSMA started reused %d in   free_cnt %d alloc_count %d ",
                                 alloc_buffer-1,aug_data->pool->free_count,aug_data->pool->alloc_count);
          
        }
        else if (aug_data->buffer[alloc_buffer-1].dsma_state == DSMA_ALLOC_SPECIAL)
        {

          aug_data->buffer[alloc_buffer-1].dsma_state = DSMA_USED_SPECIAL;
          MSG_HIGH_DSM_CONFIG("DSMA started recycled %d in   free_cnt %d  alloc_count %d  ",
                                 alloc_buffer-1,aug_data->pool->free_count,aug_data->pool->alloc_count);
        }
        aug_data->recycle_top_free = NULL;            
      }
    }
    if(aug_data->dynamic_top_free)
    {
      if ((alloc_buffer) == aug_data->dynamic_top_free->pool_id_ext)
      {
       aug_data->buffer[alloc_buffer-1].dsma_state = DSMA_USED;       
       aug_data->dynamic_top_free = NULL;
       MSG_HIGH_DSM_CONFIG("DSMA started dynamic %d in  free_cnt %d alloc_count %d ",
                                alloc_buffer-1,aug_data->pool->free_count,aug_data->pool->alloc_count);
      }
    }
     /*Send signal to task to allocate new item */
    dsma_trigger_task(aug_data);
    atomic_set(&aug_data->sbuffer_trigger,0);
    if (aug_data->dbuffer_count > 3)
    {
      atomic_set(&aug_data->scan_buffer_count,DSMA_SCAN_BUFFER_CNT_NORMAL);
    }
  }   
}

/*========================================================================
FUNCTION DSMA_CHANGE_ALLOC_BUFFER()

DESCRIPTION
  This API is used to select new allocation buffer
  
DEPENDENCIES

PARAMETERS
  aug_data - Pool's augment data

RETURN VALUE  Newly allocated item

SIDE EFFECTS - None
=========================================================================*/
static dsm_item_type* dsma_change_alloc_buffer
(
  dsm_augment_pool_data_type *aug_data
)
{
  dsm_item_type *new_buffer=NULL;
  uint32 used=DSMA_FREE;
  dsm_item_type* app_ptr;

  /* Change allocation head buffer */
  if (aug_data->recycle_top_free || aug_data->dynamic_top_free)
  {
    new_buffer = aug_data->recycle_top_free;
    if (new_buffer)
    {
      app_ptr = new_buffer->app_ptr;
      if (app_ptr)
      {
        used = aug_data->buffer[new_buffer->pool_id_ext - 1].dsma_state;          
      }
    }
    if ((used != DSMA_ALLOC)
            && (used != DSMA_ALLOC_SPECIAL))
    {
      new_buffer = aug_data->dynamic_top_free;
      if (new_buffer)
      {
        app_ptr = new_buffer->app_ptr;
        if (app_ptr)
        {
          used = aug_data->buffer[new_buffer->pool_id_ext - 1].dsma_state ;
        }     
      } 
      if (used != DSMA_ALLOC)         
      {
        new_buffer = NULL;  
      }
    }   
    if (new_buffer)
    {
      if (!new_buffer->references)
      {
        new_buffer =  dsmi_atomic_update_augment_alloc_head(&aug_data->alloc_top_free,(void**)&aug_data->new_alloc,
            new_buffer,new_buffer->pool_id_ext,app_ptr);
      }
      else
      {
        new_buffer=NULL;
      }
  
    }
  }
  if (!new_buffer)
  {
    new_buffer = dsmi_atomic_get_item(&aug_data->static_top_free);
    used = atomic_inc_return(&aug_data->sbuffer_trigger);
    if (used == aug_data->augment_cnt >> 2)
    {
      MSG_HIGH_DSM_CONFIG("DSMA started static free_cnt %d alloc_count %d dbuff_cnt %d ",
                                          aug_data->pool->free_count,aug_data->pool->alloc_count,aug_data->dbuffer_count);
      
      dsma_change_buffers_to_used(aug_data);
      dsma_trigger_task(aug_data);
      atomic_set(&aug_data->sbuffer_trigger,0);
      atomic_set(&aug_data->scan_buffer_count,DSMA_SCAN_BUFFER_CNT_NORMAL);
    }
  }
  return new_buffer;
}
/*========================================================================
FUNCTION DSMI_AUGMENT_RELEASE_BUFFER()

DESCRIPTION
  This API is called to release dsm item from augment pool

DEPENDENCIES

PARAMETERS
  table - pool
  item_ptr - Item to be deleted

RETURN VALUE  Modified Pool's free count

SIDE EFFECTS: 
=========================================================================*/
static uint32 dsmi_augment_release_buffer(
        dsm_pool_mgmt_table_type *table,
        dsm_item_type *item_ptr)
{
  dsm_augment_pool_data_type *aug_data;
  dsm_augment_buffer_type *buffer;
  uint32 pool_id_ext;
  uint32 lfree_count,cnt;

  aug_data = table->augment_data;
  pool_id_ext = item_ptr->pool_id_ext;
  buffer = &aug_data->buffer[pool_id_ext-1];

  if(dsmi_atomic_free_augment_item(&aug_data->alloc_top_free,
        &item_ptr,&item_ptr->app_ptr,pool_id_ext))
  {
    /* Removed from alloc_head */
    return atomic_inc_return(&table->free_count);
  }

  if(aug_data->reuse_trigger_cnt)
  {
    cnt = atomic_inc_return(&aug_data->reuse_trigger_cnt);
    if (cnt >aug_data->free_dbuffer_level)
    {
      /* Contiguous free count > free_dbuffer_level
                Trigger freeing of reuse buffers */
      atomic_set(&aug_data->reuse_trigger_cnt,0);
    }
  }
  else
  {
     if ((aug_data->reuse_buffer_cnt) && (!aug_data->dsm_task_free))
     {
       aug_data->dsm_task_free = TRUE;
       MSG_HIGH_DSM_CONFIG("DSMA trigger free in pool 0x%x, reuse_trigger_cnt %d ,free_cnt %d",
           table,aug_data->reuse_buffer_cnt,table->free_count);
       dsm_task_augment_free_req();
     }
  }
  if (DSM_LIKELY(pool_id_ext < DSMI_AUGMENT_TOTAL_BLOCKS))
  {
   lfree_count = dsmi_atomic_augbuffer_free_item(&buffer->free_list,&item_ptr,&item_ptr->app_ptr);

   if(lfree_count > aug_data->reuse_cnt)
   {
     if (buffer->dsma_state == DSMA_USED_SPECIAL)
     {
       atomic_compare_and_set(&buffer->dsma_state,DSMA_USED_SPECIAL,DSMA_USED);
     }
     if(DSM_UNLIKELY((!aug_data->recycle_top_free)))
     {
       if((buffer->dsma_state == DSMA_USED) && ((uint32)buffer->block<8192))
       {
    	   dsma_add_recycle_buffers(aug_data,pool_id_ext-1);
       }
     }
     if(DSM_UNLIKELY((table->free_count < aug_data->sbuffer_cnt) && (aug_data->num_recycle_buffers <3)))
     {
    	 if(buffer->dsma_state == DSMA_USED)
    	 {
    	  dsma_add_recycle_buffers(aug_data,pool_id_ext-1);
    	 }
     }
   }

   if (lfree_count == aug_data->augment_cnt)
   {
     if (atomic_compare_and_set(&buffer->dsma_state,DSMA_USED,DSMA_START_REUSE))
     {
       /* Important, as this buffer may be used for recycling */       
       if (buffer->dsma_free_list.cnt == aug_data->augment_cnt)
       {
         buffer->dsma_state = DSMA_REUSE;
         atomic_inc(&aug_data->reuse_buffer_cnt);
         if (((uint32)buffer->block>8192) &&
         (dsm_get_external_memory_usage() > dsma_ext_memory_alloc_upper_limit))
         {
            aug_data->dsm_task_free = TRUE;
         }
         if((aug_data->reuse_buffer_cnt > DSMI_AUGMENT_TOTAL_REUSE_BUFFERS)||
          (!aug_data->reuse_trigger_cnt))
         {
           aug_data->dsm_task_free = TRUE;
         }
         if (aug_data->dsm_task_free)
         {
           dsm_task_augment_free_req();
         }
       }
       else
       {
           buffer->dsma_state = DSMA_USED;
       }
     }
   }
  }
  else
  {
     dsmi_atomic_free_item(&aug_data->static_top_free, &item_ptr,&item_ptr->app_ptr);
     atomic_inc(&table->free_count);
  }

  return table->free_count;
}

#endif /*#ifdef FEATURE_DSM_AUGMENT*/


/*===========================================================================
FUNCTION dsmi_atomic_dec_if_gt_val()

DESCRIPTION
 Performs atomic subtraction from a variable, if the condition below is met.
 *value_ptr -dec_value >check_value

DEPENDENCIES
  Atomic operations support.

PARAMETERS
 value_ptr - address of value to decrement
 dec_value - value to decrement
 check_value - value to check against
 value_ptr is decremented only if 


RETURN VALUE
  Decremented value, irrespective of the check_value.

SIDE EFFECTS
  None
===========================================================================*/
inline 
 int32 dsmi_atomic_dec_if_gt_val
(
 uint32 *value_ptr,
 uint32 dec_value,
 int32 check_value
)
{
    int32 result;

    __asm__ __volatile__(
        "1:     %0 = memw_locked(%2)\n"
        "       %0 = sub(%0, %4)\n"
        "       p0 = cmp.gt(%0, %3)\n"
        "       if (!p0) jump 2f\n"
        "       memw_locked(%2, p0) = %0\n"
        "       if !p0 jump 1b\n"
        "2:\n"
        : "=&r" (result),"+m" (*value_ptr)
        : "r" (value_ptr), "r"(check_value),"r" (dec_value)
        : "p0");

    return result;


}

/*===========================================================================
FUNCTION dsmi_atomic_add_if_lt_val()

DESCRIPTION
  Performs atomic addition to a variable, if the condition is met.
  Condition :  *value_ptr+inc_value < check_value

DEPENDENCIES
    Atomic operations support.

PARAMETERS
 value_ptr - address of value to decrement
 inc_value - value to decrement
 check_value - value to check against

RETURN VALUE
  Incremented value, irrespective of the check_value.

SIDE EFFECTS
  None
===========================================================================*/
inline 
uint32 dsmi_atomic_add_if_lt_val
(
 uint32 *value_ptr,
 uint32 inc_value,
 uint32 check_value
)
{
    uint32 result;

    __asm__ __volatile__(
        "1:     %0 = memw_locked(%2)\n"
        "       %0 = add(%0, %3)\n"
        "       p0 = cmp.ltu(%0, %4)\n"
        "       if (!p0) jump 2f\n"
        "       memw_locked(%2, p0) = %0\n"
        "       if !p0 jump 1b\n"
        "2:\n"
        : "=&r" (result),"+m" (*value_ptr)
        : "r" (value_ptr), "r" (inc_value),"r"(check_value)
        : "p0");

    return result;


}

/*===========================================================================
FUNCTION dsmi_atomic_set_if_lt()

DESCRIPTION
  Performs atomic addition to a variable, if the condition is met.
  Condition : if  value < *value_ptr
                    *value_ptr = value

DEPENDENCIES
    Atomic operations support.

PARAMETERS
 value_ptr - address of value to decrement
 value - value to SET

RETURN VALUE
  Incremented value, irrespective of the check_value.

SIDE EFFECTS
  None
===========================================================================*/
inline 
uint32 dsmi_atomic_set_if_lt
(
 uint32 *value_ptr,
 uint32 value
)
{
    uint32 result;

    __asm__ __volatile__(
        "1:     %0 = memw_locked(%2)\n"
        "       p0 = cmp.ltu(%3, %0)\n"
        "       if (!p0) jump 2f\n"
        "       memw_locked(%2, p0) = %3\n"
        "       if !p0 jump 1b\n"
        "2:\n"
        : "=&r" (result),"+m" (*value_ptr)
        : "r" (value_ptr), "r"(value)
        : "p0");

    return result;

}

/*===========================================================================
FUNCTION dsmi_atomic_set_if_gt()

DESCRIPTION
  Performs atomic addition to a variable, if the condition is met.
  Condition : if  value > *value_ptr
                    *value_ptr = value

DEPENDENCIES
    Atomic operations support.

PARAMETERS
 value_ptr - address of value to decrement
 value - value to SET

RETURN VALUE
  Value before setting, irrespective of the check_value.

SIDE EFFECTS
  None
===========================================================================*/
inline 
uint32 dsmi_atomic_set_if_gt
(
 uint32 *value_ptr,
 uint32 value
)
{
    uint32 result;

    __asm__ __volatile__(
        "1:     %0 = memw_locked(%2)\n"
        "       p0 = cmp.gtu(%3, %0)\n"
        "       if (!p0) jump 2f\n"
        "       memw_locked(%2, p0) = %3\n"
        "       if !p0 jump 1b\n"
        "2:\n"
        : "=&r" (result),"+m" (*value_ptr)
        : "r" (value_ptr), "r"(value)
        : "p0");

    return result;

}

/*========================================================================
FUNCTION  dsmi_atomic_get_item()

DESCRIPTION
  Allocates new DSM item from allocation head. IF alloc_head is NULL return.

DEPENDENCIES
   Atomic operations support.

PARAMETERS
  top_free_item_ptr - Top free item of allocation head

RETURN VALUE
  A pointer to a 'dsm_item_type'

SIDE EFFECTS
  None
=========================================================================*/
static inline
dsm_item_type * dsmi_atomic_get_item
(
 dsm_item_type **top_free_item_ptr
)
{
  void* next_free_ptr;
  dsm_item_type *result;

  asm volatile(
      "1:     %1 = memw_locked(%3)\n\t"
      "       p0 = cmp.gtu(%1,#9)\n\t"
      "       if (!p0) jump 2f\n\t"
      "       %0 = memw(%1+#16)\n\t" /* *top_free_item_ptr =  item_ptr->app_ptr*/
      "       memw_locked(%3, p0) = %0\n\t"
      "       if !p0 jump 1b\n\t"
      "       if p0 jump 3f\n\t"
      "2:     %1 = #0 \n\t"
      "3:\n\t"
      : "=&r" (next_free_ptr),"=&r" (result),"+m"(*top_free_item_ptr)
      : "r" (top_free_item_ptr)
      : "p0");

  return result;
}

/*===========================================================================
FUNCTION dsmi_atomic_free_item()

DESCRIPTION
  Frees DSM item, by updating
  the pool's top_free_item ptr using atomic memory operations.

DEPENDENCIES
  Atomic operations support.

PARAMETERS
    top_free_item_ptr - Top Pointer of pool's free list
    item_ptr - Pointer to DSM item
    free_item_ptr - Item_ptr's link to be added to pool's free list

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static inline
void dsmi_atomic_free_item
(
 dsm_item_type **top_free_item_ptr,
 dsm_item_type **item_ptr,
 void **free_item_ptr
)
{

  dsm_item_type * cur_free_ptr;
  
  asm volatile(
    "1:     %0 = memw_locked(%3)\n\t"
    "       memw(%4) = %0 \n\t" /* *top_free_item_ptr->app_ptr */
    "       memw_locked(%3, p0) = %5\n\t"
    "       if !p0 jump 1b\n\t"
    : "=&r"(cur_free_ptr),"+m"(*free_item_ptr),"+m"(*top_free_item_ptr)
    : "r" (top_free_item_ptr),"r"(free_item_ptr),"r"(*item_ptr)
    : "p0"); 

}
    
/*===========================================================================
FUNCTION DSMI_NEW_BUFFER()

DESCRIPTION
  This function will return a pointer to the next free item from the free
  queue. Pool id is passed in as parameter. 

DEPENDENCIES
  None.


PARAMETERS
  pool_id - Pool from which new item should be allocated. 

RETURN VALUE
  A pointer to a 'dsm_item_type'

SIDE EFFECTS
  None
===========================================================================*/
dsm_item_type *dsmi_new_buffer
(
 dsm_mempool_id_type pool_id,
 const char * file,
 uint32 line
)
{
  dsm_pool_mgmt_table_type *table; /* table pointer                      */
  dsm_item_type *item_ptr= NULL;         /* pointer to item to retrieve        */
  int32 lfree_count;
#ifdef FEATURE_DSM_AUGMENT
  dsm_augment_pool_data_type * aug_data = NULL;
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-----------------------------------------------------------------------
    Pre-set item pointer then switch on Pool ID to set the free pool 
    stack pointer to the proper point in the proper free stack
  -----------------------------------------------------------------------*/

#ifdef FEATURE_DSM_MEM_CHK
  dsmi_verify_pool_id(pool_id); /* Won't return if pool_id is bogus */
#else
  ASSERT((unsigned int)pool_id > 256);
#endif

  table = (dsm_pool_mgmt_table_type*)pool_id;

	if(pool_id ==DSM_DS_SL_ITEM_POOL)  
	  dsm_newbuf_update(1,file,line);
#ifdef FEATURE_DSM_AUGMENT
  atomic_inc(&table->alloc_count);
  if (table->augment_data)
  {
     item_ptr =  dsmi_atomic_get_item(&table->aug_top_free_item);
     if (item_ptr)
     {
        /* No other checks are necessary
         * reusing items */

        if (item_ptr->references)
        {
          ERR_FATAL("DSM Library Config Error 6 : Check use after free 0x%x, 0x%x, %d. Contact DSM team",
                                                         aug_data->pool,item_ptr,aug_data->dbuffer_count );
          return 0;
        }
        item_ptr->references++;
        if (!item_ptr->app_ptr)
        {
          dsma_fill_single_alloc_buffer((dsm_augment_pool_data_type *)table->augment_data);
        }
        item_ptr->app_ptr = 0;
#ifdef FEATURE_DSM_MEM_CHK
        dsmi_touch_item_int(item_ptr,file,line);
#endif
        return item_ptr;
    }
  }
#endif /* #ifdef FEATURE_DSM_AUGMENT */
  lfree_count = dsmi_atomic_dec_if_gt_val(&table->free_count,1,-1);
  if (lfree_count < 0)
  {
    if (table->pool_stats)
    {
        atomic_inc(&table->pool_stats->stats_num_out_of_memory);
#ifdef FEATURE_DSM_AUGMENT
        atomic_set(&table->pool_stats->stats_item_count_at_out_of_memory,table->item_count);
        atomic_dec(&table->alloc_count);
#endif
    }
    return NULL;
  }
  if (DSM_UNLIKELY(lfree_count <table->alloc_check_free_count_level))
  {
    DSMI_POOL_LOCK(pool_id);
    dsmi_invoke_cb(pool_id, lfree_count,TRUE);
    DSMI_POOL_UNLOCK(pool_id);
  }
#ifdef FEATURE_DSM_MEM_CHK
  /*-----------------------------------------------------------------
      Check to see if a new minimum free count was established. Set the new
      minimum if so.
    -----------------------------------------------------------------*/
  if(lfree_count < table->stats_min_free_count)
  {
    dsmi_atomic_set_if_lt(&table->stats_min_free_count, lfree_count);
  }
#endif
/*-----------------------------------------------------------------
      Actually grab item off of the stack
-----------------------------------------------------------------*/
  item_ptr = dsmi_atomic_get_item(&table->top_free_item);

#ifdef FEATURE_DSM_AUGMENT
  if ((!item_ptr) && (table->augment_data))
  {
      aug_data = (dsm_augment_pool_data_type *)table->augment_data;
      item_ptr = dsmi_atomic_get_item(&aug_data->alloc_top_free);
      if (DSM_UNLIKELY(!item_ptr))
      {
        item_ptr = dsma_change_alloc_buffer(aug_data);
        if (aug_data->scan_buffer_count!=aug_data->augment_cnt)
        {
          dsma_fill_single_alloc_buffer(aug_data);
        }
      }
      if(DSM_UNLIKELY((aug_data->new_alloc) && (item_ptr)))
      {
        dsma_alloc_buffer_change_trigger(aug_data);
      }
  }
#endif/* FEATURE_DSM_AUGMENT */
  if (DSM_LIKELY((item_ptr) && (!item_ptr->references)))
  {
   if (DSM_UNLIKELY(!item_ptr->pool_id))
   {
    /* First time allocation
          * Initialize item's header and cookie values */
    dsmi_item_init(item_ptr,table);
   }
  /*-----------------------------------------------------------------
    Set reference count to 1 and the app_field to 0
  -----------------------------------------------------------------*/
  /* already intlocked don't need dsm_ref functions here */
   item_ptr->app_ptr = 0;
   item_ptr->references++;

#ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
  /* Costly check for this feature at the time of allocation
    * Items are verified and reset when they are freed or initialized */
   dsmi_verify_buffer(item_ptr);
   dsmi_item_reset_int(item_ptr);
#endif /*  FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET */

#ifdef FEATURE_DSM_MEM_CHK
  /*-----------------------------------------------------------------
    Set tracer to allocated
  -----------------------------------------------------------------*/
   dsmi_touch_item_int(item_ptr,file,line);
#endif
  }
  else
  {
    /* when pool is close to empty , this is an uncommon sceario, so no need to call error fatal */
    /* ERR_FATAL("Out of memory in pool with free_count>0 free %d item_count %d pool 0x%x",table->free_count,table->item_count,pool_id); */
    if (!item_ptr)
    {
        atomic_inc(&table->free_count);
        atomic_dec(&table->alloc_count);
    }
    else
    {
      ERR_FATAL("Reference not 0",item_ptr,table,table->free_count);     
    }
  }  
  return item_ptr;
} /* dsm_new_buffer() */
/*===========================================================================
FUNCTION DSMI_RELEASE_BUFFER()

DESCRIPTION
  Performs tasks necessary to actually release a DSM item.
    - Resets Item fields
    - Calls Pool Callbacks
    - Restores item to free queue

DEPENDENCIES
  This function is INTERNAL to dsm.

PARAMETERS
  item_ptr - Pointer to item to return to free queue. 

RETURN VALUE
  The next item in the packet chain.

SIDE EFFECTS
  Note that this does not set item_ptr to NULL, unlike free_packet or
  free_buffer which takes a dsm_item_type** as a parameter.
===========================================================================*/
static void dsmi_release_buffer
(
  dsm_item_type *item_ptr,
  const char * file,
  uint32 line
)
{
  dsm_pool_mgmt_table_type *table;
  dsm_dynamic_pool_data_type *dynamic_pool;
  unsigned int lfree_count;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
  if( item_ptr != NULL )
  {  
    dsmi_verify_packet(item_ptr);
  }

  /*-----------------------------------------------------------------------
    Set the data back to 0xAA.  0xAA was chosen because it is easy to
    recognize in the debugger and will more often than not, cause violent
    death of the application if someone tries to use it.
  -----------------------------------------------------------------------*/
  if( DSM_Fill_AA != 0 && DSM_ITEM_POOL(item_ptr) !=0 )
  {
    memset(item_ptr->data_ptr, 0xAA,
           DSM_POOL_ITEM_SIZE(DSM_ITEM_POOL(item_ptr)));
  }

#endif /* FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET */

#ifdef FEATURE_DSM_MEM_CHK
  /*-----------------------------------------------------------------
    Won't return on bogus pool id 
  -----------------------------------------------------------------*/
  dsmi_verify_pool_id(item_ptr->pool_id);
#endif
  table = (dsm_pool_mgmt_table_type*)(item_ptr->pool_id);
  if (DSM_UNLIKELY(table->dynamic_pool != NULL))
  {
    dynamic_pool = (dsm_dynamic_pool_data_type*)(table->dynamic_pool);
    if (dynamic_pool->release_state)
    {
      if (dsmi_freeze_pool_allocation(table, TRUE))
      {
          dsm_task_dynamic_free_req();
      }
      return;
    }
  }
#ifdef FEATURE_DSM_MEM_CHK
  dsmi_touch_item_int(item_ptr, file, line);
#endif /* FEATURE_DSM_MEM_CHK */

  /*-----------------------------------------------------------------------
    Reset dynamic fields of item then return to proper queue.
    -----------------------------------------------------------------------*/
    dsmi_item_reset_int(item_ptr);

  /*-----------------------------------------------------------------------
    Set the flow mgmt table index, then, use that to set the free stack
    array index. Re-establish the item size per the needs of the particular
    pool.
  -----------------------------------------------------------------------*/
#ifdef FEATURE_DSM_AUGMENT
  atomic_dec(&table->alloc_count);
  if (item_ptr->pool_id_ext != 0)
  {
     lfree_count = dsmi_augment_release_buffer(table,item_ptr);
     /* Returns incremented free_count */
  }
  else /* STATIC POOL */
  {
     dsmi_atomic_free_item(&table->top_free_item, &item_ptr,&item_ptr->app_ptr);
     lfree_count = atomic_inc_return(&table->free_count);
  }
#else
  dsmi_atomic_free_item(&table->top_free_item, &item_ptr,&item_ptr->app_ptr);
  lfree_count = atomic_inc_return(&table->free_count);
#endif

  /*---------------------------------------------------------------------
    Return item to free pool stack then increment appropriate free count.
  ---------------------------------------------------------------------*/
  /* call free event cb if it is registered */
  if( table->free_cb != NULL )
  {
    (table->free_cb)(table->free_cb_user_data,item_ptr);
  }
  /* Start checking from the next level */

  if (DSM_UNLIKELY(table->free_check_free_count_level &&
      lfree_count >= table->free_check_free_count_level))
  {
    DSMI_POOL_LOCK(item_ptr->pool_id);
    dsmi_invoke_cb(item_ptr->pool_id, lfree_count, FALSE);
    DSMI_POOL_UNLOCK(item_ptr->pool_id);
  }
} /* dsmi_release_buffer() */

/*===========================================================================
FUNCTION DSMI_HANDLE_ALLOC_ERROR()

DESCRIPTION
  This function will be called when multiple items are allocated in DSM pool and one of the items
  returned an error. Based on the error, the succesfully allocated items are returned to pool, and the function returns
  or error fatal is called.

DEPENDENCIES
  None.

PARAMETERS
  item_ptr - that caused the error
  pdsm_array - Array of items that need to be returned to pool because of the error
  count - number of items succesfully allocated in pdsm_array

RETURN VALUE
  A pointer to a 'dsm_item_type'

SIDE EFFECTS
  None
===========================================================================*/
void dsmi_handle_alloc_error
(
 dsm_item_type* item_ptr,
 dsm_item_type *pdsm_array[],
 uint32 count
)
{
 uint32 iter;
 if (!item_ptr)
 {
   for (iter = 0; iter <count; iter++)
   {
     pdsm_array[iter]->references--;
     dsmi_release_buffer(pdsm_array[iter],0,0);
   }
   memset(pdsm_array, 0x00, count * sizeof(void *));
 }
 else if (item_ptr->references)
 {
   ERR_FATAL("Reference not 0",item_ptr,pdsm_array,count);
 }
 /*ERR_FATAL("Out of memory in pool with free_count>0 free %d item_count %d pool 0x%x",item_ptr,pdsm_array,count);*/
}

/*===========================================================================
FUNCTION DSM_NEW_BUFFERS()

DESCRIPTION
  This function returns multiple free items from the free
  queue of given pool. If requested count is not available, 
  allocation fails.

DEPENDENCIES
  None.

PARAMETERS
  pool_id - Pool from which new items should be allocated. 
  num_buffers - number of dsm_items requested

RETURN VALUE
  pdsm_array - Pointers to "num_buffers" number of  'dsm_item_type'
  boolean - TRUE - Allocation succeeded.
                 FALSE - Allocation failed. 
                 pdsm_array contents are set to NULL.

SIDE EFFECTS
  PLEASE MAKE SURE THAT THE FUNCTION IS CALLED ONLY FROM HIGH PRIORITY TASKS for 
  the optimum performance 
===========================================================================*/
boolean dsmi_new_buffers 
( 
 dsm_mempool_id_type pool_id, 
 uint32 num_buffers, 
 dsm_item_type *pdsm_array[], 
 const char * file, 
 uint32 line 
) 
{
  dsm_pool_mgmt_table_type *table; /* table pointer                      */
  dsm_item_type *item_ptr;         /* pointer to item to retrieve        */
  uint32 iter;
  int32 lfree_count;
  uint32 laug_alloc = 0;
#ifdef FEATURE_DSM_AUGMENT
  dsm_augment_pool_data_type *aug_data;
#endif
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-----------------------------------------------------------------------
    Pre-set item pointer then switch on Pool ID to set the free pool 
    stack pointer to the proper point in the proper free stack
     -----------------------------------------------------------------------*/

  if (!num_buffers)
  {
    return TRUE;
  }
  
  item_ptr = NULL;

#ifdef FEATURE_DSM_MEM_CHK
  dsmi_verify_pool_id(pool_id); /* Won't return if pool_id is bogus */
#else
  ASSERT((unsigned int)pool_id > 256);
#endif

  table = (dsm_pool_mgmt_table_type*)pool_id;

#ifdef FEATURE_DSM_AUGMENT
  atomic_add(&table->alloc_count,num_buffers);
  aug_data =  (dsm_augment_pool_data_type *)table->augment_data;

  if (aug_data)
  {
    if ((table->aug_top_free_item) && (num_buffers<DSMA_SCAN_BUFFER_CNT_NORMAL))
    {
      laug_alloc = dsma_alloc_augment_buffers_few(aug_data,num_buffers,
                                      pdsm_array,
                                      file,
                                     line);
    }
    if ((laug_alloc<num_buffers) &&(aug_data->scan_buffer_count!=aug_data->augment_cnt))
    {
      laug_alloc += dsma_alloc_augment_buffers(table->augment_data,(num_buffers-laug_alloc),
                      &pdsm_array[laug_alloc],
                      file,
                     line);
    }
  }

  if (laug_alloc < num_buffers)
#endif /*FEATURE_DSM_AUGMENT*/
  {
    lfree_count = dsmi_atomic_dec_if_gt_val(&table->free_count,(num_buffers-laug_alloc),-1);
    if (DSM_UNLIKELY(lfree_count < 0 ))
    {
      MSG_HIGH( "Out of memory in pool %d",pool_id,0,0);//auto-gen, to change remove 'QSR_' and first param
      if (table->pool_stats)
      {
        atomic_inc(&table->pool_stats->stats_num_out_of_memory);
#ifdef FEATURE_DSM_AUGMENT
        atomic_set(&table->pool_stats->stats_item_count_at_out_of_memory,table->item_count);
        atomic_sub(&table->alloc_count,num_buffers);
#endif
      }
      memset(pdsm_array, 0x00, num_buffers * sizeof(void *));
      return FALSE;
    }
#ifdef FEATURE_DSM_MEM_CHK
    if(lfree_count < table->stats_min_free_count)
    {
      dsmi_atomic_set_if_lt(&table->stats_min_free_count, lfree_count);
    }
#endif
    if ( lfree_count <table->alloc_check_free_count_level)
    {
      DSMI_POOL_LOCK(pool_id);
      dsmi_invoke_cb(pool_id, lfree_count, TRUE);
      DSMI_POOL_UNLOCK(pool_id);
    }
    /*-----------------------------------------------------------------
      Actually grab item off of the stack
    -----------------------------------------------------------------*/
       /* Do max possible from static, else from augment buffer */
    for(iter= laug_alloc; iter < num_buffers; iter++)
    {
       item_ptr = dsmi_atomic_get_item(&table->top_free_item);
 #ifdef FEATURE_DSM_AUGMENT
       if ((!item_ptr) && (aug_data))
       {
         item_ptr = dsmi_atomic_get_item(&aug_data->alloc_top_free);
         if (DSM_UNLIKELY(!item_ptr))
         {
           item_ptr = dsma_change_alloc_buffer(aug_data);
         }
         if(DSM_UNLIKELY((aug_data->new_alloc) && (item_ptr)))
         {
           dsma_alloc_buffer_change_trigger(aug_data);
         }
       }
 #endif /* FEATURE_DSM_AUGMENT */
       if (DSM_LIKELY((item_ptr) && (!item_ptr->references)))
       {
         if (DSM_UNLIKELY(!item_ptr->pool_id))
         {
           /* First time allocation
            * Initialize item's header and cookie values */
           dsmi_item_init(item_ptr,table);
         }
         item_ptr->app_ptr = 0;
         item_ptr->references++;

 #ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
         /* Costly check for this feature at the time of allocation
           * Items are verified and reset when they are freed or initialized */
         dsmi_verify_buffer(item_ptr);
         dsmi_item_reset_int(item_ptr);
 #endif /*  FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET */

 #ifdef FEATURE_DSM_MEM_CHK
         dsmi_touch_item_int(item_ptr,file,line);
 #endif
         pdsm_array[iter] = item_ptr;
       }
       else
       {
         dsmi_handle_alloc_error(item_ptr,pdsm_array,iter);
         atomic_sub(&table->alloc_count,num_buffers);
         atomic_add(&table->free_count,(num_buffers-iter));
         return FALSE;
       }
     }
  } /*if not already allocated */

  if(pool_id ==DSM_DS_SL_ITEM_POOL)  
    dsm_newbuf_update(num_buffers,file,line);    
  return TRUE;
} /* dsm_new_buffers() */


#else /* #ifdef FEATURE_DSM_LOCKLESS */

/*===========================================================================
FUNCTION DSM_NEW_BUFFER()

DESCRIPTION
  This function will return a pointer to the next free item from the free
  queue. Pool id is passed in as parameter.

DEPENDENCIES
  None.


PARAMETERS
  pool_id - Pool from which new item should be allocated.

RETURN VALUE
  A pointer to a 'dsm_item_type'

SIDE EFFECTS
  None
===========================================================================*/
dsm_item_type *dsmi_new_buffer
(
 dsm_mempool_id_type pool_id,
 const char * file,
 uint32 line
)
{
  dsm_pool_mgmt_table_type *table; /* table pointer                      */
  dsm_item_type *item_ptr;         /* pointer to item to retrieve        */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-----------------------------------------------------------------------
    Pre-set item pointer then switch on Pool ID to set the free pool
    stack pointer to the proper point in the proper free stack
  -----------------------------------------------------------------------*/
  item_ptr = NULL;

#ifdef FEATURE_DSM_MEM_CHK
  dsmi_verify_pool_id(pool_id); /* Won't return if pool_id is bogus */
#else
  ASSERT((unsigned int)pool_id > 256);
#endif
  table = (dsm_pool_mgmt_table_type*)pool_id;

  DSMI_POOL_LOCK(pool_id);
  if (table->free_count == 0)
  {
    MSG_HIGH("Out of memory in pool %d",pool_id,0,0);
    if (table->pool_stats)
    {
      table->pool_stats->stats_num_out_of_memory++;
    }
    DSMI_POOL_UNLOCK(pool_id);
    return NULL;
  }
  else {

    /*-----------------------------------------------------------------
      Actually grab item off of the stack
    -----------------------------------------------------------------*/
    ASSERT(table->free_count <= table->item_count);
    item_ptr = table->free_stack[--table->free_count];
	if(item_ptr == NULL){return NULL;}
    if (DSM_UNLIKELY(!item_ptr->pool_id))
    {
      /* First time allocation
      * Initialize item's header and cookie values */
      dsmi_item_init(item_ptr,table);
    }

    /*-----------------------------------------------------------------
      Set reference count to 1 and the app_field to 0
    -----------------------------------------------------------------*/
    /* already intlocked don't need dsm_ref functions here */
    ASSERT(item_ptr->references == 0);
    item_ptr->references = 1;


#ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
   /* Costly check for this feature at the time of allocation
   * Items are verified and reset when they are freed or initialized */
   dsmi_verify_buffer(item_ptr);
   dsmi_item_reset_int(item_ptr);
#endif /*  FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET */

#ifdef FEATURE_DSM_MEM_CHK
    /*-----------------------------------------------------------------
      Check to see if a new minimum free count was established. Set the new
      minimum if so.
    -----------------------------------------------------------------*/
    if(table->free_count < table->stats_min_free_count)
    {
      table->stats_min_free_count = table->free_count;
    }

    /*-----------------------------------------------------------------
      Set tracer to allocated
    -----------------------------------------------------------------*/
    dsmi_touch_item_int(item_ptr,file,line);
#endif

    if (DSM_UNLIKELY(table->free_count <table->alloc_check_free_count_level))
    {
      dsmi_invoke_cb(pool_id, table->free_count,TRUE);
    }

  } /* else free count not zero */

  DSMI_POOL_UNLOCK(pool_id);
  return item_ptr;

} /* dsm_new_buffer() */

/*===========================================================================
FUNCTION DSMI_RELEASE_BUFFER()

DESCRIPTION
  Performs tasks necessary to actually release a DSM item.
    - Resets Item fields
    - Calls Pool Callbacks
    - Restores item to free queue

DEPENDENCIES
  This function is INTERNAL to dsm.

PARAMETERS
  item_ptr - Pointer to item to return to free queue. 

RETURN VALUE
  The next item in the packet chain.

SIDE EFFECTS
  Note that this does not set item_ptr to NULL, unlike free_packet or
  free_buffer which takes a dsm_item_type** as a parameter.
===========================================================================*/
static void dsmi_release_buffer
(
  dsm_item_type *item_ptr,
  const char * file,
  uint32 line
)
{
  dsm_pool_mgmt_table_type *table;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
  if( item_ptr != NULL )
  {  
    dsmi_verify_packet(item_ptr);
  }
  /*-----------------------------------------------------------------------
    Set the data back to 0xAA.  0xAA was chosen because it is easy to
    recognize in the debugger and will more often than not, cause violent
    death of the application if someone tries to use it.
  -----------------------------------------------------------------------*/
  if( DSM_Fill_AA != 0 && DSM_ITEM_POOL(item_ptr) !=0 )
  {
    memset(item_ptr->data_ptr, 0xAA,
           DSM_POOL_ITEM_SIZE(DSM_ITEM_POOL(item_ptr)));
  }
#endif /* FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET */


#ifdef FEATURE_DSM_MEM_CHK
  /*-----------------------------------------------------------------
    Won't return on bogus pool id 
  -----------------------------------------------------------------*/
  dsmi_verify_pool_id(item_ptr->pool_id);
#endif
  table = (dsm_pool_mgmt_table_type*)(item_ptr->pool_id);
  if (DSM_UNLIKELY(table->dynamic_pool != NULL))
  {
     if (((dsm_dynamic_pool_data_type*)table->dynamic_pool)->release_state)
     {
       if (dsmi_freeze_pool_allocation(table, TRUE))
       {
         dsm_task_dynamic_free_req();
       }
       return;
     }
   }

#ifdef FEATURE_DSM_MEM_CHK
  dsmi_touch_item_int(item_ptr, file, line);
#endif /* FEATURE_DSM_MEM_CHK */

  /*-----------------------------------------------------------------------
    Reset dynamic fields of item then return to proper queue.
  -----------------------------------------------------------------------*/
  dsmi_item_reset_int(item_ptr);
  
  /*-----------------------------------------------------------------------
    Set the flow mgmt table index, then, use that to set the free stack
    array index. Re-establish the item size per the needs of the particular
    pool.
  -----------------------------------------------------------------------*/
  DSMI_POOL_LOCK(item_ptr->pool_id);

  /*-----------------------------------------------------------------------
    Verify that item is not already on free stack. Work backwards in order
    to catch problem as quickly as possible if it's there.
  -----------------------------------------------------------------------*/
  table->free_stack[table->free_count++] = item_ptr;

 /*---------------------------------------------------------------------
    Return item to free pool stack then increment appropriate free count.
  ---------------------------------------------------------------------*/
  ASSERT(table->free_count <= table->item_count);

  /* call free event cb if it is registered */
  if( table->free_cb != NULL )
  {
    (table->free_cb)(table->free_cb_user_data,item_ptr);
  }
  /* Start checking from the next level */

  
  if (DSM_UNLIKELY(table->free_check_free_count_level &&
       table->free_count >= table->free_check_free_count_level))  
  {
    dsmi_invoke_cb(item_ptr->pool_id, table->free_count, FALSE);
  }
  DSMI_POOL_UNLOCK(item_ptr->pool_id);
} /* dsmi_release_buffer() */

/*===========================================================================
FUNCTION DSMI_NEW_BUFFERS()

DESCRIPTION
  This function returns multiple free items from the free
  queue of given pool. If requested count is not available, 
  allocation fails.

DEPENDENCIES
  None.

PARAMETERS
  pool_id - Pool from which new items should be allocated. 
  num_buffers - number of dsm_items requested

RETURN VALUE
  pdsm_array - Pointers to "num_buffers" number of  'dsm_item_type'
  boolean - TRUE - Allocation succeeded.
                 FALSE - Allocation failed. 
                 pdsm_array contents are set to NULL.

SIDE EFFECTS
  PLEASE MAKE SURE THAT THE FUNCTION IS CALLED ONLY FROM HIGH PRIORITY TASKS for 
  the optimum performance 
===========================================================================*/
boolean dsmi_new_buffers 
( 
 dsm_mempool_id_type pool_id, 
 uint32 num_buffers, 
 dsm_item_type *pdsm_array[], 
 const char * file, 
 uint32 line 
) 
{
  dsm_pool_mgmt_table_type *table; /* table pointer                      */
  dsm_item_type *item_ptr;         /* pointer to item to retrieve        */
  uint32 iter;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-----------------------------------------------------------------------
    Pre-set item pointer then switch on Pool ID to set the free pool 
    stack pointer to the proper point in the proper free stack
     -----------------------------------------------------------------------*/
  item_ptr = NULL;

#ifdef FEATURE_DSM_MEM_CHK
  dsmi_verify_pool_id(pool_id); /* Won't return if pool_id is bogus */
#else
  ASSERT((unsigned int)pool_id > 256);
#endif

  table = (dsm_pool_mgmt_table_type*)pool_id;
  DSMI_POOL_LOCK(pool_id);

  if (DSM_UNLIKELY(table->free_count <  num_buffers))
  {
    MSG_HIGH("Out of memory in pool %d",pool_id,0,0);
    memset(pdsm_array, 0x00, num_buffers * sizeof(void *));
    if (table->pool_stats)
    {
      table->pool_stats->stats_num_out_of_memory++;
    }
    DSMI_POOL_UNLOCK(pool_id);
    return FALSE;
  }
  else
  {
    for(iter= 0; iter < num_buffers; iter++)
    {
      item_ptr = table->free_stack[--table->free_count]; 
      pdsm_array[iter] = item_ptr;
         
      /*-----------------------------------------------------------------
          Set reference count to 1 and the app_field to 0
          -----------------------------------------------------------------*/
      /* already intlocked don't need dsm_ref functions here */
      if (DSM_UNLIKELY(!item_ptr->pool_id))
      {
        /* First time allocation
        * Initialize item's header and cookie values */
        dsmi_item_init(item_ptr,table);
      }
      ASSERT(item_ptr->references == 0);
      item_ptr->references = 1;

#ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
      /* Costly check for this feature at the time of allocation
      * Items are verified and reset when they are freed or initialized */
      dsmi_verify_buffer(item_ptr);
      dsmi_item_reset_int(item_ptr);
#endif /*  FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET */
        
#ifdef FEATURE_DSM_MEM_CHK
      /*-----------------------------------------------------------------
       Set tracer to allocated
      -----------------------------------------------------------------*/
      dsmi_touch_item_int(item_ptr,file,line);
#endif
    }
#ifdef FEATURE_DSM_MEM_CHK
    if(table->free_count < table->stats_min_free_count)
    {
      table->stats_min_free_count = table->free_count; 
    }
#endif
    if (DSM_UNLIKELY(table->free_count <table->alloc_check_free_count_level))
    {
      dsmi_invoke_cb(pool_id, table->free_count, TRUE);
    }
    DSMI_POOL_UNLOCK(pool_id);

  } /* else free count not zero */
  return TRUE;
  
} /* dsm_new_buffer() */


#endif /* FEATURE_DSM_LOCKLESS*/

/*===========================================================================
FUNCTION DSM_OFFSET_NEW_BUFFER()

DESCRIPTION
  This function will return a pointer to the next free item from the free
  pool represented by the passed pool ID parameter.  

  In addition, the data_ptr field of the DSM item will be offset the 
  specified number of bytes into the payload.  A specified offset larger 
  than the size of the item in the pool ID will result in an error and 
  return a NULL DSM item.

DEPENDENCIES
  None.

PARAMETERS
  pool_id - Memory pool new item should be allocated from .
  offset - Space that should be reserved at the head of the item

RETURN VALUE
  A pointer to a 'dsm_item_type'.  The data_ptr will be moved by "offset"
  number of bytes.

SIDE EFFECTS
  None
===========================================================================*/
dsm_item_type *dsmi_offset_new_buffer
(
  dsm_mempool_id_type pool_id,
  uint16 offset,
  const char * file,
  uint32 line
)
{
  dsm_item_type *item_ptr;         /* pointer to item to retrieve */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_DSM_MEM_CHK
  dsmi_verify_pool_id(pool_id); /* Won't return if pool_id is bogus */
#else
  ASSERT((unsigned int)pool_id > 256);
#endif

  if (offset > DSM_POOL_ITEM_SIZE(pool_id)) 
  {
    /*-----------------------------------------------------------------------
      The requested offset is larger than the pool item size.  This is an
      error condition, so return.
    -----------------------------------------------------------------------*/
    ERR("Offset larger than pool item size", 0, 0, 0);
    return NULL;
  }

  /*-------------------------------------------------------------------------
    Offset is ok.  Obtain a free item based on pool ID.
  -------------------------------------------------------------------------*/
  item_ptr = dsmi_new_buffer(pool_id, file, line);
  if (item_ptr == NULL)
  {
    ERR("Unable to allocate offset item", 0, 0, 0);
    return NULL;
  }

  /*-------------------------------------------------------------------------
    Advance the data_ptr by offset number of bytes.  Update the size of 
    the item accordingly.
  -------------------------------------------------------------------------*/
  item_ptr->data_ptr += offset;

  item_ptr->size -= offset;

  return item_ptr;

} /* dsm_offset_new_buffer */


/*===========================================================================
FUNCTION DSM_NEW_DATA_BUFFERS()

DESCRIPTION
  This function will return pointer to payload of new DSM_ITEM.
  Can be only used with DSM_FREE_DATA_BUFFER() function
  Will not work with DSM_FREE_PACKET() function

DEPENDENCIES
  None.

PARAMETERS
  pool_id - Pool from which new item should be allocated.
  num_buffers - Number of buffers to be allocated
  pdsm_array - Array to contain allocated items

RETURN VALUE
  True, if allocated

SIDE EFFECTS
  None
===========================================================================*/
boolean dsmi_new_data_buffers
(
  dsm_mempool_id_type pool_id,
  uint32 num_buffers,
  void *pdsm_array[],
  const char * file,
  uint32 line
)
{
    boolean ret_val = FALSE;
    uint32 i;
    ret_val = dsmi_new_buffers(pool_id,num_buffers,(dsm_item_type **)pdsm_array,file,line);
    if (ret_val)
    {
        for (i = 0; i< num_buffers; i++)
        {
            pdsm_array[i] = ((dsm_item_type*)pdsm_array[i])->data_ptr;
        }
    }
    return ret_val;
}

/*===========================================================================
FUNCTION DSM_FREE_BUFFER()

DESCRIPTION
  This function will return the passed buffer to the proper DSM free queue.
  This includes freeing any duplicated buffer items.

  CAUTION!!!
  You probably want to call dsm_free_packet instead of this routine.
  This function probably does NOT do what you expect.
  
DEPENDENCIES
  None

PARAMETERS
  item_ptr - Pointer to item to return to free queue. 

RETURN VALUE
  The next item in the packet chain.

SIDE EFFECTS
  Note that this does not set item_ptr to NULL, unlike free_packet which
  takes a dsm_item_type** as parameter.
===========================================================================*/
dsm_item_type *dsmi_free_buffer
(
 dsm_item_type *item_ptr,
 const char * file,
 uint32 line
)
{
  dsm_item_type *next_item_ptr;     /* pointer to hold next item in packet */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(item_ptr == NULL)
  {
    return NULL;
  }
#ifdef FEATURE_DSM_MEM_CHK
  dsmi_verify_buffer(item_ptr);
#endif /* FEATURE_DSM_MEM_CHK */


  /*-------------------------------------------------------------------------
    Save pointer to next item of packet (for return value) 
  -------------------------------------------------------------------------*/
  next_item_ptr = item_ptr->pkt_ptr;

  /*-------------------------------------------------------------------------
    If this is a duplicate item, call dsm_free_buffer on the original to
    decrease the reference count to reflect that we are freeing this 
    duplicate.
  -------------------------------------------------------------------------*/
  if(item_ptr->dup_ptr != NULL)
  {
   /*----------------------------------------------------------------------- 
      Reduce references on the original item.  If it has gone to zero, 
      free it.
    -----------------------------------------------------------------------*/
    if(dsmi_ref_dec(item_ptr->dup_ptr) == 0)
    {
      (void)dsmi_release_buffer(item_ptr->dup_ptr, file, line);    
    }
    /*----------------------------------------------------------------------- 
      Clear the dup_ptr field.
      We do this here rather than below for efficiency, since non-duplicate 
      items never have the dup_ptr changed from NULL.
      (note this takes advantage of the fact that the you can't have a 
      duplicate of a duplicate, i.e. references==1 always for duplicates.
    -----------------------------------------------------------------------*/ 
    item_ptr->dup_ptr = NULL;         
  }

  /*------------------------------------------------------------------------- 
    Decrement reference count. If it has gone to zero, free it. 
  -------------------------------------------------------------------------*/
  if(dsmi_ref_dec(item_ptr) == 0)
  {
    (void)dsmi_release_buffer(item_ptr, file, line);    
  } /* if( dsmi_ref_dec... */

 return next_item_ptr;

} /* dsm_free_buffer() */


/*===========================================================================
FUNCTION DSM_FREE_DATA_BUFFER()

DESCRIPTION
  This function will free up dsm items, allocated by DSM_NEW_DATA_BUFFER

  CAUTION!!!
  This function will not work with dsm_item_pointers, and will only work with payload
  allocated via DSM_NEW_DATA_BUFFER

DEPENDENCIES
  None

PARAMETERS
  item_ptr - Pointer to item to return to free queue.

RETURN VALUE
  The next item in the packet chain.

SIDE EFFECTS
===========================================================================*/
void dsmi_free_data_buffer
(
 void *item_payload_ptr,
 const char * file,
 uint32 line
)
{
dsm_item_type* item_ptr;

  if (item_payload_ptr !=NULL)
  {
      item_ptr = DSMI_ITEM_HEAD_FROM_PAYLOAD(item_payload_ptr);
#ifdef FEATURE_DSM_MEM_CHK
     /*-----------------------------------------------------------------
      Won't return on bogus pool id
      -----------------------------------------------------------------*/
      dsmi_verify_pool_id(item_ptr->pool_id);
      if(*(uint32*)(void *)DSMI_ITEM_TAIL(item_ptr) != DSM_TAIL_COOKIE)
      {
         ERR_FATAL("dsm_verify_buffer: Bad Trailer Cookie for item 0x%x",
                   (int)item_ptr, 0, 0);
      }
#endif
      if ( 0 == dsmi_ref_dec(item_ptr))
      {
        (void)dsmi_release_buffer(item_ptr, file, line);
      }
  }
}
/*===========================================================================
FUNCTION DSM_FREE_PACKET()

DESCRIPTION
  This function will return to the proper DSM free queue
  the entire passed packet buffer chain.

  NOTE: This function can accept a NULL pointer parameter.

DEPENDENCIES
  None

PARAMETERS
  pkt_head_ptr - Address of pointer to chain of items to return.  This
                 pointer will be set to NULL on return

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsmi_free_packet
(
 dsm_item_type **pkt_head_ptr,
 const char * file,
 uint32 line
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET
  if(pkt_head_ptr != NULL && *pkt_head_ptr != NULL) 
  {
    dsmi_verify_packet(*pkt_head_ptr);
  }
#endif /* FEATURE_DSM_MEM_CHK_EXPENSIVE_ALWAYS_VERIFY_PACKET */

  if(pkt_head_ptr != NULL)
  {
    while(*pkt_head_ptr != NULL)
    {
      *pkt_head_ptr = dsmi_free_buffer(*pkt_head_ptr,file,line);
    }
  }
} /* dsm_free_packet() */

/*===========================================================================
FUNCTION DSM_FREEZE_POOL_ALOCATION()

DESCRIPTION
  This internal function will freeze pool alocation from a table

DEPENDENCIES
  None

PARAMETERS
  table - Freeze pool allocation of table
  increment_free_count - Whether to increment free_count

RETURN VALUE
  True - If all items are freed
  False - If some items are yet to be freed

SIDE EFFECTS
  None
===========================================================================*/
boolean dsmi_freeze_pool_allocation
(
  dsm_pool_mgmt_table_type *table,
  boolean increment_free_count
)
{
   uint32 lfreecount = 0;
   uint32 all_freed = FALSE;
   if (table->dynamic_pool)
   {
#ifdef FEATURE_DSM_LOCKLESS
   if (table->free_count)
   {
     lfreecount = dsmi_atomic_set_if_lt(&table->free_count,0);
   }
   lfreecount += (increment_free_count)?1:0;
   if (lfreecount)
   {
       lfreecount = atomic_add_return(&(((dsm_dynamic_pool_data_type*)table->dynamic_pool)->released_items),lfreecount);
   }
   else
   {
       lfreecount=((dsm_dynamic_pool_data_type*)table->dynamic_pool)->released_items;
   }
   if (lfreecount >= table->item_count)
   {
       all_freed = TRUE;
   }
#else
   DSMI_POOL_LOCK(table);
   lfreecount = table->free_count;
   lfreecount += (increment_free_count)?1:0;
   table->free_count = 0;
   ((dsm_dynamic_pool_data_type*)table->dynamic_pool)->released_items+=lfreecount;
   lfreecount=((dsm_dynamic_pool_data_type*)table->dynamic_pool)->released_items;
   if (lfreecount >= table->item_count)
   {
     all_freed = TRUE;
   }
   DSMI_POOL_UNLOCK(table);
#endif
   }
   return all_freed;
} /* dsm_free_packet() */

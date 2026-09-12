
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            H D R S R C H   B M   I N T E R F A C E

GENERAL DESCRIPTION
  This module contains the searcher interface to the buffer manager.

EXTERNALIZED FUNCTIONS
  hdrsrchbm_init_cmd_buf_pool
  hdrsrchbm_malloc
  hdrsrchbm_free
  hdrsrchbm_reset_cmd_buf_pool
  hdrsrchbm_return_checked_out_buffers
  hdrsrchbm_group_heap_clean

INITIALIZATION AND SEQUENCING REQUIREMENTS

      Copyright (c) 2005 - 2010 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/21/10   cnx     Fixed compiler warnings.
02/04/09   lyl     Fixed the lint errors.
07/16/07   ljl     Changed the F3 messages to low level.
03/26/07   ljl     Changed the F3 messages to medium level.
10/17/05   ljl     Initial revision
===========================================================================*/

#include "comdef.h"
#include "stm_bm.h"
#include "hdrsrchbm.h"
#include "hdrdebug.h"

/*-----------------------------------------------------------------------------
      Buffer Manager definitions
-----------------------------------------------------------------------------*/

/* Ultimately, should enumerate all the command buffers and use LAST_BUFFER */
#define MAX_CMD_POOLS    7

/* Number and size of various pools */

#define POOL_NUM_256     8
#define POOL_SIZE_256    256

#define POOL_NUM_128     8
#define POOL_SIZE_128    128

#define POOL_NUM_64      8
#define POOL_SIZE_64     64

#define POOL_NUM_32      8
#define POOL_SIZE_32     32

#define TOTAL_POOL_NUM   ( POOL_NUM_256 + POOL_NUM_128 + \
                           POOL_NUM_64 + POOL_NUM_32 )

static stm_bm_handle    hdrsrch_cmd_pool_handle;
  /* handle for calls to bm_ functions */

static stm_bm_list_type hdrsrch_cmd_node_mem[MAX_CMD_POOLS];
  /* memory for buffer list nodes */

/* Allocate the memory pools for the various buffer sizes */

static struct
{
  /*lint -esym( 754, hdr ) not used locally*/
  stm_bm_hdr_type hdr;
  /*lint -esym( 754, buf ) not used locally*/
  uint8           buf[POOL_SIZE_256];
}
hdrsrchbm_256_pool[POOL_NUM_256];

static struct
{
  stm_bm_hdr_type hdr;
  uint8           buf[POOL_SIZE_128];
}
hdrsrchbm_128_pool[POOL_NUM_128];

static struct
{
  stm_bm_hdr_type hdr;
  uint8           buf[POOL_SIZE_64];
}
hdrsrchbm_64_pool[POOL_NUM_64];

static struct
{
  stm_bm_hdr_type hdr;
  uint8           buf[POOL_SIZE_32];
}
hdrsrchbm_32_pool[POOL_NUM_32];

static stm_msg_hdr *hdrsrchbm_debug[TOTAL_POOL_NUM];


/*-----------------------------------------------------------------------------
      Exported Functions
-----------------------------------------------------------------------------*/

/*===========================================================================

FUNCTION HDRSRCHBM_INIT_CMD_BUF_POOL

DESCRIPTION
 Maps static arrays into buffer manager space for dynamic allocation.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_init_cmd_buf_pool( void )
{
  boolean                    success = TRUE;
    /* Whethr init succeed */

  uint32                     i, debug_index = 0;
    /* Index */

/*---------------------------------------------------------------------------*/

  hdrsrch_cmd_pool_handle = stm_bm_create( MAX_CMD_POOLS, hdrsrch_cmd_node_mem );
    /* Create pool handle */

  success &= stm_bm_add_buffer_list( hdrsrch_cmd_pool_handle, POOL_SIZE_32,
                                     POOL_NUM_32,
                                     (uint8 *)hdrsrchbm_32_pool );
    /* Add pool size 32 */

  for (i = 0; i != POOL_NUM_32; i++)
  {
    /* Use void pointer as a bridge to avoid the following compiler warning: 
       "cast increases required alignment of target type". */
    hdrsrchbm_debug[debug_index++] = (stm_msg_hdr *)((void *)(hdrsrchbm_32_pool[i].buf));
  }

  success &= stm_bm_add_buffer_list( hdrsrch_cmd_pool_handle, POOL_SIZE_64,
                                     POOL_NUM_64,
                                     (uint8 *)hdrsrchbm_64_pool );
    /* Add pool size 64 */

  for (i = 0; i != POOL_NUM_64; i++)
  {
    /* Use void pointer as a bridge to avoid the following compiler warning: 
       "cast increases required alignment of target type". */
    hdrsrchbm_debug[debug_index++] = (stm_msg_hdr *)((void *)(hdrsrchbm_64_pool[i].buf));
  }
  
  success &= stm_bm_add_buffer_list( hdrsrch_cmd_pool_handle, POOL_SIZE_128,
                                     POOL_NUM_128,
                                     (uint8 *)hdrsrchbm_128_pool );
    /* Add pool size 128 */

  for (i = 0; i != POOL_NUM_32; i++)
  {
    /* Use void pointer as a bridge to avoid the following compiler warning: 
       "cast increases required alignment of target type". */
    hdrsrchbm_debug[debug_index++] = (stm_msg_hdr *)((void *)(hdrsrchbm_128_pool[i].buf));
  }
  
  success &= stm_bm_add_buffer_list( hdrsrch_cmd_pool_handle, POOL_SIZE_256,
                                     POOL_NUM_256,
                                     (uint8 *)hdrsrchbm_256_pool );
    /* Add pool size 256 */

  for (i = 0; i != POOL_NUM_32; i++)
  {
    /* Use void pointer as a bridge to avoid the following compiler warning: 
       "cast increases required alignment of target type". */
    hdrsrchbm_debug[debug_index++] = (stm_msg_hdr *)((void *)(hdrsrchbm_256_pool[i].buf));
  }

  if (!success)
  {
    ERR_FATAL("Unable to setup buffers", 0, 0, 0);
  }

}  /* hdrsrchbm_init_cmd_buf_pool */


/*===========================================================================

FUNCTION HDRSRCHBM_RESET_CMD_BUF_POOL

DESCRIPTION
 Defend against memory leaks by resetting all buffers.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_reset_cmd_buf_pool( void )
{
  boolean        success = TRUE;

/*---------------------------------------------------------------------------*/

  success &= stm_bm_remove_buffer_list( &hdrsrch_cmd_pool_handle, POOL_SIZE_32 );
  success &= stm_bm_remove_buffer_list( &hdrsrch_cmd_pool_handle, POOL_SIZE_64 );
  success &= stm_bm_remove_buffer_list( &hdrsrch_cmd_pool_handle, POOL_SIZE_128 );
  success &= stm_bm_remove_buffer_list( &hdrsrch_cmd_pool_handle, POOL_SIZE_256 );

  if (!success)
  {
    ERR_FATAL( "Unable to reset buffers", 0, 0, 0 );
  }

  success &= stm_bm_add_buffer_list( hdrsrch_cmd_pool_handle, POOL_SIZE_32,
                                     POOL_NUM_32,
                                     (uint8 *)hdrsrchbm_32_pool );
  success &= stm_bm_add_buffer_list( hdrsrch_cmd_pool_handle, POOL_SIZE_64,
                                     POOL_NUM_64,
                                     (uint8 *)hdrsrchbm_64_pool );
  success &= stm_bm_add_buffer_list( hdrsrch_cmd_pool_handle, POOL_SIZE_128,
                                     POOL_NUM_128,
                                     (uint8 *)hdrsrchbm_128_pool );
  success &= stm_bm_add_buffer_list( hdrsrch_cmd_pool_handle, POOL_SIZE_256,
                                     POOL_NUM_256,
                                     (uint8 *)hdrsrchbm_256_pool );

  if (!success)
  {
    ERR_FATAL("Unable to setup buffers", 0, 0, 0);
  }

} /* hdrsrchbm_reset_cmd_buf_pool */



/*===========================================================================

FUNCTION HDRSRCHBM_RETURN_CHECKED_OUT_BUFFERS

DESCRIPTION
 Emit error messages for each buffer currently checked out. Clear the buffer,
 and return it to the pool.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_return_checked_out_buffers
(
  stm_group_type             *group
    /* Stm group */
)
{

  uint32                     i;
    /* Index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for (i = 0; i != TOTAL_POOL_NUM; i++)
  {
    if (hdrsrchbm_debug[i]->checked_out &&
        hdrsrchbm_debug[i]->group == group)
    {

      HDR_MSG_SRCH_SPRINTF_3( MSG_LEGACY_HIGH,
                              "Buffer %dus old message id %d, sm %s",
                              timetick_diff( hdrsrchbm_debug[i]->time,
                                             timetick_get(), T_USEC ),
                              hdrsrchbm_debug[i]->id,
                              hdrsrchbm_debug[i]->sm->name );

      if ( hdrsrchbm_debug[i]->watermark != STM_MSG_HDR_WATERMARK )
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                        "Corrupted watermark in buffer %d, not freeing",
                        (uint32) hdrsrchbm_debug[i] );
      }
      else
      {
        hdrsrchbm_debug[i]->checked_out = FALSE;
        hdrsrchbm_free( (void *) hdrsrchbm_debug[i] );
      }
    }
  }

} /* hdrsrchbm_return_checked_out_buffers */


/*===========================================================================

FUNCTION HDRSRCHBM_MALLOC

DESCRIPTION
 Request a buffer of size 'size'.  If it's unavailable, we can't carry on,
 so ERR_FATAL.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void *hdrsrchbm_malloc
(
  uint32                     size
    /* specify the size of buffer you want */
)
{
  void *ptr = (void *)stm_bm_get_buffer( hdrsrch_cmd_pool_handle, size );

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  HDR_MSG_SRCH_1( MSG_LEGACY_LOW,
                  "0x%08x being checked out from BM", 
                  ptr );

  if ( ptr == NULL )
  {
    ERR_FATAL( "Unable to allocate a buffer of size %d", size, 0, 0 );
  }

  memset( ptr, 0, size ); /* to enforce correct usage, clear buffers first */

  return ptr;
}  /* hdrsrchbm_malloc */


/*===========================================================================

FUNCTION HDRSRCHBM_FREE

DESCRIPTION
 Return a buffer to the pool.  Failure shouldn't occur unless pointers are
 invalid, so ERR_FATAL.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_free
(
  void                       *ptr
    /* old buffer to be returned to the pool */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if (!stm_bm_return_buffer( hdrsrch_cmd_pool_handle, (uint8 *)ptr))
  {
    ERR_FATAL("Unable to return buffer 0x%08x", (int) ptr, 0, 0);
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_LOW,
                      "0x%08x being returned to BM",
                      ptr );
  }

}  /* hdrsrchbm_free */


/*===========================================================================

FUNCTION HDRSRCHBM_GROUP_HEAP_CLEAN

DESCRIPTION
 Send out a buffer usage report and reset the statistics after a group has
 exited.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/

void hdrsrchbm_group_heap_clean
(
  stm_group_type             *group
    /* Stm group */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  stm_bm_report_stats(hdrsrch_cmd_pool_handle);
    /* dump the statistics */

  hdrsrchbm_return_checked_out_buffers(group);
    /* send out messages pertaining to checked out buffers and return them */

} /* hdrsrchbm_group_heap_clean */


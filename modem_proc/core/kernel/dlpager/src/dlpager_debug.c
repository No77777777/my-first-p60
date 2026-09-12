#include <assert.h>
//#include <atomic_ops.h>
#include "dlpager_debug.h"
#include "dlpager_swappool.h"
#include "dlpager_meta.h"
#include "dlpager_pagesm.h"
#include "dlpager_types.h"
#include "dlpager.h"
#include "dlpager_params.h"

#define WORD_MASK      0xFFFFFFFF
#define HALF_WORD_MASK 0xFFFF
#define BYTE_MASK      0xFF


#define DLPAGER_NUM_MAX_OUTSTANDING_WRITES_RO   32
#define DLPAGER_NUM_MAX_OUTSTANDING_WRITES_RW   32

// FIXME cporter: Not sure if this one is needed. it's in the list olga gave me.
#define DLPAGER_MAX_OUTSTANDING_WRITES          DLPAGER_NUM_MAX_OUTSTANDING_WRITES_RW 

dlpager_debug_write_t dlpager_outstanding_writes_ro[DLPAGER_NUM_MAX_OUTSTANDING_WRITES_RO] = {{0}};
dlpager_debug_write_t dlpager_outstanding_writes_rw[DLPAGER_NUM_MAX_OUTSTANDING_WRITES_RW] = {{0}};

unsigned int dlpager_num_outstanding_writes_ro = 0;
unsigned int dlpager_num_outstanding_writes_rw = 0;

// T32 extension should return error if user tries to write while this counter is non-zero
//atomic_word_t dlpager_debug_is_busy = ATOMIC_INIT(0);
unsigned int dlpager_debug_is_busy = 0;

int dlpager_osam_debug_enabled = 0; // set to 1 by live t32 osam module

void dlpager_debug_apply_outstanding_writes(unsigned int page_va)
{
  
  dlpager_swappool_page_info_t swappool_page_info; // temporary for getting swappool virtual address
  unsigned int swp_page_va;      // target page_va of decompression into the swappool

  unsigned int num_outstanding_writes;   // <= max possible number of writes to a buffer
  unsigned int num_outstanding_writes_i; // index of valid elements looked at in the buffer so far
  dlpager_debug_write_t *outstanding_writes_buf; // points to RO or RW buffer
  unsigned int max_num_outstanding_writes; // size of the buffer
  unsigned int i;                          // index into the buffer
  unsigned int num_applied_writes_rw;      // number of RW pages to which we've applied an outstanding write so far

  // Get the target page_va in the swappool of current decompression
  dlpager_swappool_lookup(page_va, &swappool_page_info);
  swp_page_va = swappool_page_info.va_swap_rw;

  // Switch on address range
  if(dlpager_meta_is_addr_in_text_or_rodata_range(page_va)){
    max_num_outstanding_writes = DLPAGER_NUM_MAX_OUTSTANDING_WRITES_RO;
    num_outstanding_writes     = dlpager_num_outstanding_writes_ro;
    outstanding_writes_buf     = dlpager_outstanding_writes_ro;
  }else{
    max_num_outstanding_writes = DLPAGER_NUM_MAX_OUTSTANDING_WRITES_RW;
    num_outstanding_writes     = dlpager_num_outstanding_writes_rw;
    outstanding_writes_buf     = dlpager_outstanding_writes_rw;
  }
  
  // Loop over the appropriate buffer
  i = 0;
  num_applied_writes_rw = 0;
  num_outstanding_writes_i = 0;
  while(num_outstanding_writes_i < num_outstanding_writes && i < max_num_outstanding_writes){

    // Get the address of a T32 write
    unsigned int debug_write_addr = outstanding_writes_buf[i].address;

    // If valid entry
    if(debug_write_addr != 0){

      // Update counter: We've seen a valid entry
      num_outstanding_writes_i++;

      // If user wrote to our page of interest
      if(DLPAGER_ALIGN_DOWN(debug_write_addr, PAGE_SIZE) == page_va){

        // Write that user's updated value to our page
        unsigned int debug_mask = outstanding_writes_buf[i].mask;
        unsigned int dst_addr = swp_page_va + (debug_write_addr & (PAGE_SIZE - 1));
        if(debug_mask == WORD_MASK)
          *(unsigned int *)   dst_addr = outstanding_writes_buf[i].value;
        else if(debug_mask == HALF_WORD_MASK)
          *(unsigned short *) dst_addr = outstanding_writes_buf[i].value & debug_mask;
        else if(debug_mask == BYTE_MASK)
          *(unsigned char *)  dst_addr = outstanding_writes_buf[i].value & debug_mask;
        else
          ASSERT(0);

        // If this is RW
        if(dlpager_meta_is_addr_in_rwdata_range(page_va)){
          num_applied_writes_rw++; // Update counter: We've just applied a write to a RW page
          outstanding_writes_buf[i].address = 0; // "free" the element by marking the addr as 0
        }
      }
    }
    i++; // Update counter: Move to next element in buffer
  }
  // if we updated any RW pages
  if(num_applied_writes_rw){
    dlpager_num_outstanding_writes_rw -= num_applied_writes_rw;
  }
  
}

static void handle_state(dlpager_page_state_t state, int addend)
{
  // if moving into or out of a "stable" state, update is-busy
  if(state == UNMAPPED_BSS   ||
     state == UNMAPPED_CLEAN ||
     state == MAPPED_CLEAN   ||
     state == MAPPED_DIRTY){
       dlpager_debug_is_busy += addend;
  }
}
void dlpager_debug_handle_start_state(dlpager_page_state_t start_state)
{
  handle_state(start_state, 1);
}
void dlpager_debug_handle_end_state(dlpager_page_state_t end_state)
{
  handle_state(end_state, -1);
}

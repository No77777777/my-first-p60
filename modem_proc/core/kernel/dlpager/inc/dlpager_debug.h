#ifndef  DLPAGER_DEBUG_H
#define  DLPAGER_DEBUG_H

#include "dlpager_types.h"

typedef struct{
  unsigned int address; // uncompressed virtual address of compressed data/text, 0 means the entry is empty
  unsigned int value;   // value written by T32 user
  unsigned int mask;    // size to write at address. word=0xFFFFFFFF, short = 0xFFFF, byte = 0xFF
  unsigned int unused;
}dlpager_debug_write_t;


extern unsigned int dlpager_num_outstanding_writes_ro;
extern unsigned int dlpager_num_outstanding_writes_rw;

// T32 extern should return error of user tries to write when dlpager_debug_is_busy is non-zero
extern unsigned int dlpager_debug_is_busy;

extern int dlpager_osam_debug_enabled;

void dlpager_debug_apply_outstanding_writes(unsigned int page_va);
void dlpager_debug_handle_start_state(dlpager_page_state_t start_state);
void dlpager_debug_handle_end_state(dlpager_page_state_t end_state);


#endif

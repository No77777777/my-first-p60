#ifndef DOG_COMMON_INTERNAL_H
#define DOG_COMMON_INTERNAL_H
/*
#============================================================================
#  Name:
#    dog_common_internal.h
#
#  Description:
#    Watchdog common internal header for HAL, HB and Virtual dog monitoring
#
# Copyright (c) 2018-2020 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
#============================================================================
*/

#include "stdint.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#define dog_all_bits_set(val,bitmask)        (((val) & (bitmask)) == bitmask) ? 1 : 0  // TRUE if all bits (in the bitmask) are set
#define dog_any_bit_set(val,bitmask)         ((val) & (bitmask))                       // TRUE if any bit (in the bitmask) is set

#define dog_set_bit(val,bitmask)             ((val) |= (bitmask))
#define dog_clr_bit(val,bitmask)             ((val) &= ~(bitmask))

typedef enum
{ 
  ALL_ENABLE = 0,                            // nothing is disabled, DEFAULT

  HW_WDOG_DISABLE = 0x1,                     // disables HW WDOG, used for bring-up  
  HW_KICK_DISABLE = 0x2,                     // ensures dog is never pet, results in an intentional timeout if HW_WDOG_DISABLE is not set
  
  HB_DISABLE = 0x4,                          // disables dog HB monitoring of tasks in root PD and user PD
  
  VIRTUAL_DISABLE = 0x8,                     // disables requirement of user PD dog HB reporting to root PD
  USERPD_SPAWN_TIMER_DISABLE = 0x10,         // disables 40 sec PD spawn completion timer in root PD started using on_spawn callback
  USERPD_RCINIT_GRACE_TIMER_DISABLE = 0x20,  // disables 40 sec RCINIT completion grace timer in user PD from root PD
  
  ALL_BUT_KICK_DISABLE = 0x3D,               // Helper to disable all DOG features except kick
  ALL_DISABLE = 0x3F,                        // Helper to disable all DOG features
  
  DOG_DISABLE_STATE_MAX_ENTRIES = 0xFF
} dog_disable_state_e;

#if defined(__cplusplus)
}
#endif

#endif //DOG_COMMON_INTERNAL_H

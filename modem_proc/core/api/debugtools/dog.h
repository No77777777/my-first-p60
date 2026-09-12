#ifndef DOG_H
#define DOG_H
/*
#============================================================================
#  Name:
#    dog.h
#
#  Description:
#    Deprecated dog interface. DO NOT USE
#
# Copyright (c) 2015-2017 by Qualcomm Technologies Inc.  All Rights Reserved.
#============================================================================
*/

#include "comdef.h"             /* Definitions for byte, word, etc.     */
#include "core_pragmas.h"
#include "dog_hb_rex.h"
#include "task.h"

/* LEGACY ONLY */
/* Remove the following references in client codes */

#define DOG_DYNAMIC_API              /* Dog 1.5 */
#define DOG_DEFAULT_TIMEOUT          60000
#define DOG_HW_INIT_COMPLETE         "dog:hw_init_complete"

typedef enum {
  DOG_DEADLKCHK_DISABLE   = 0,      /**< Disable. */
  DOG_DEADLKCHK_ENABLE    = 1,      /**< Enable. */
  DOG_DEADLKCHK_UNDEF     = 0xff,   /**< Undefined. */
} dog_deadlockcheck_type;

extern dog_report_type dog_mm_rpt_2;
extern dog_report_type dog_ftm_rpt_2;
extern dog_report_type dog_hdrmc_rpt_2;
extern dog_report_type dog_plt_rpt_2;
extern const rex_timer_cnt_type dog_plt_rpt_time_2;
extern const rex_timer_cnt_type dog_ftm_rpt_time_2;

#define  DOG_MM_RPT                 dog_mm_rpt_2
#define  DOG_FTM_RPT                dog_ftm_rpt_2
#define  DOG_HDRMC_RPT              dog_hdrmc_rpt_2
#define  DOG_PLT_RPT                dog_plt_rpt_2
#define  DOG_PLT_RPT_TIME           dog_plt_rpt_time_2
#define  DOG_FTM_RPT_TIME           dog_ftm_rpt_time_2

CORE_WARN_DEPRECATED_MSG("Deprecated API","Migrate to dog_hb_report(report)")
void static inline dog_report(dog_report_type report)
{
   return;
}

CORE_WARN_DEPRECATED_MSG("Deprecated API","Migrate to dog_hb_register_qurt(qurt_anysignal_t* sig_p, unsigned int sig_m)")
dog_report_type static inline dog_register_rex(rex_tcb_type* p_tcb, uint32 timeout, uint32 unused)
{
   return 1;
}
#define dog_register(x,y,z) dog_register_rex(x,y,z)

CORE_WARN_DEPRECATED_MSG("Deprecated Unused API","Migrate to new Dog HB APIs")
uint32 static inline dog_get_report_period(dog_report_type dog_report_id)
{
   return 1000;
}

CORE_WARN_DEPRECATED_MSG("Deprecated API","Migrate to dog_hb_deregister_qurt(dog_report_type)")
void static inline dog_deregister(dog_report_type dog_report_id)
{
   return;
}

CORE_WARN_DEPRECATED_MSG("Deprecated Unused API","Migrate to new Dog HB APIs")
void static inline dog_intentional_timeout_stm(void)
{
   return;
}

CORE_WARN_DEPRECATED_MSG("Deprecated Unused API","Migrate to new Dog HB APIs")
void static inline dog_monitor_pause(unsigned int report_id)
{
   return;
}

CORE_WARN_DEPRECATED_MSG("Deprecated Unused API","Migrate to new Dog HB APIs")
void static inline dog_monitor_resume(unsigned int report_id)
{
   return;
}

CORE_WARN_DEPRECATED_MSG("Deprecated Unused API","Migrate to new Dog HB APIs")
void static inline dog_kick( void )
{
   return;
}

CORE_WARN_DEPRECATED_MSG("Deprecated Unused API","Migrate to new Dog HB APIs")
void static inline dog_autokick(boolean enable)
{
   return;
}

#endif

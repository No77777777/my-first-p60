/*===========================================================================

  rpm_fake_sleep.h

  Copyright (c) 2012 Qualcomm Technologies Incorporated.  
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ===========================================================================*/

#ifndef RPM_FAKE_SLEEP_H
#define RPM_FAKE_SLEEP_H

#include "rpm.h"
#include "rpm_mq.h"
#include "rpm_resource.h"

typedef enum
{
  SPM_SHUTDOWN_REQ = 0,
  SPM_BRINGUP_REQ,
  SPM_TRANSITION_COMPLETE,
  SPM_SHUTDOWN_ACK,
  SPM_BRINGUP_ACK,
  SPM_NUM_STATES,
} rpm_spm_entry_reason;

typedef void (*rpm_spm_cb_fcn)(void* context);

void rpm_register_fake_sleep_cb(rpm_spm_cb_fcn cb, void* context);

#endif // RPM_FAKE_SLEEP_H


/*===========================================================================

  Copyright (c) 2016 Qualcomm Technologies, Inc.
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ===========================================================================*/

#include "rpm_messageram.h"
#include "rpmclient.h"
#include "dll_global_def.h"


vmpm_data_t* rpm_message_ram_vmpm(vmpm_masters master)
{
    return &(RPM_MSG_RAM_DATA->master[master].vmpm);
}

rpm_master_stats_t* rpm_message_ram_master_stats(vmpm_masters master)
{
    return &(RPM_MSG_RAM_DATA->master[master].master_stats);
}

// CR 1052292 - WLAN freeze io workaround
// This is used by WLAN FM to configure the proxy register for MPM_WCSSAON_CONFIG register.
// RPM reads the proxy register and program MPM_WCSSAON_CONFIG register when entering Vdd Min.
DLL_API_GLOBAL void rpm_message_ram_set_mpm_proxy_register(uint32 mask, uint32 value)
{
    vmpm_data_t *vmpm      = rpm_message_ram_vmpm(VMPM_MODEM);
    uint32       old_value = vmpm->mpm_proxy_register;

    vmpm->mpm_proxy_register = (old_value & (~mask)) | value;
}

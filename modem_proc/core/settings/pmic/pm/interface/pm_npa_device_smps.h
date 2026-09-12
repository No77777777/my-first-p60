#ifndef PM_NPA_DEVICE_SMPS__H
#define PM_NPA_DEVICE_SMPS__H
/*===========================================================================


P M    N P A   S M P S   D E V I C E    H E A D E R    F I L E

DESCRIPTION
This file contains prototype definitions npa device layer

Copyright (c) 2012-2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/pmic/pm/interface/pm_npa_device_smps.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/22/13   vk      Added suppressible request support
02/12/12   wra     KVP based device setting initial file 
===========================================================================*/

/*===========================================================================

INCLUDE FILES

===========================================================================*/

#include "pm_npa_device.h"
#include "pm_err_flags.h"


/*===========================================================================

TYPE DEFINITIONS

===========================================================================*/
typedef struct  
{
    // [Disable (default), Enable] — max aggregation (left to right)
    unsigned sw_enable_key; // PM_NPA_KEY_SOFTWARE_ENABLE
    unsigned sw_enable_size; // 4
    unsigned sw_enable_value;

    // [AUTO (default), IPEAK, NPM] - max aggregation (left to right)
    unsigned smps_sw_mode_key; // PM_NPA_KEY_SMPS_SOFTWARE_MODE
    unsigned smps_sw_mode_size; // 4
    unsigned smps_sw_mode_value;

    // [X uV] — max aggregation
    unsigned uv_key; // PM_NPA_KEY_MICRO_VOLT
    unsigned uv_size; // 4
    unsigned uv_value;

}pm_npa_smps_kvps;

npa_resource_plugin *pm_npa_get_smps_kvps_plugin(void);

#ifdef PMIC_OFFTARGET_TESTING
#define PMIC_DEV_RMT_RSRC_SMPS( chipid, rtype, index, rpm_name_index )  \
{                                                                       \
    PMIC_DEV_RSRC_NAME_VEC_IN( chipid, rtype, index ),                  \
    rpm_name_index,                                                     \
    PMIC_NPA_RMT_PROTOCOL_RPM_TYPE,                                     \
    &pm_npa_smps_kvps_plugin,                                           \
    npa_remote_resource_local_aggregation_driver_fcn,                   \
    "kvps",                                                             \
    sizeof(pm_npa_smps_kvps)/sizeof(npa_resource_state),                \
    NPA_RESOURCE_VECTOR_STATE | NPA_RESOURCE_SUPPORTS_SUPPRESSIBLE      \
}
#else
    #define PMIC_DEV_RMT_RSRC_SMPS( chipid, rtype, index, rpm_name_index )   \
    {                                                                        \
        PMIC_DEV_RSRC_NAME_VEC_IN( chipid, rtype, index ),                   \
        rpm_name_index,                                                      \
        PMIC_NPA_RMT_PROTOCOL_RPM_TYPE,                                      \
        &pm_npa_smps_kvps_plugin,                                            \
        npa_remote_resource_local_aggregation_no_initial_request_driver_fcn, \
        "kvps",                                                              \
        sizeof(pm_npa_smps_kvps)/sizeof(npa_resource_state),                 \
        NPA_RESOURCE_VECTOR_STATE | NPA_RESOURCE_SUPPORTS_SUPPRESSIBLE       \
    }
#endif

pm_err_flag_type
pm_npa_smps_kvps_init (pm_npa_smps_kvps *kvps);

#endif // PM_NPA_DEVICE_SMPS__H

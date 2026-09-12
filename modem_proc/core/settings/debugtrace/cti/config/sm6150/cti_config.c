/****************************************************************************
 * Copyright (c) 2014 Qualcomm Technologies, Inc. All Rights Reserved.
 *
 * FILE:            core/debugtrace/cti/config/9x25/cti_config.c
 *
 * DESCRIPTION:     Device specific configuration for CTI.
 *
 *                  Specifically contains CTI block -> physical address
 *                  mappings.
 *
 *
 * EDIT HISTORY FOR MODULE
 * --------------------------------------------------------------------------
 *
 * when         who         what, where, why
 * ----------   -------     -----------------
 * 2014-01-24   rpoddar     Initial revision.
 *
 *****************************************************************************/

#include "cti_config.h"

const struct CTIPhysAddrMap cti_phys_addr_array[] = {
    {"wcss_cti0",        0x18ADC000},
    {"wcss_cti1",        0x18ADD000},
    {"wcss_cti2",        0x18ADE000},
};

const struct CTIPhysAddrMapLen cti_phys_addr_array_len[] = {
    { sizeof(cti_phys_addr_array) / sizeof(struct CTIPhysAddrMap) }
};
 

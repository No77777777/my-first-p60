 /**
 * \file gpr_log_diag_stub.c
 * \brief
 *    This file contains stubs of the gpr diag logging files
 *
 *
 * \copyright
 *  Copyright (c) 2022 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
// clang-format off
/*
$Header: //components/dev/gpr.common/1.0/wchaffin.gpr.common.1.0.gpr_logging_fixes/ext/logging/stub_src/gpr_log_stub.c#1 $
*/
// clang-format on

/******************************************************************************
 * Includes                                                                    *
 *****************************************************************************/

#include "ar_osal_error.h"

#include "gpr_api_i.h"
#include "gpr_log.h"


uint32_t gpr_log_get_diag_log_code(uint32_t host_domain_id, uint32_t src_domain_id, uint32_t dest_domain_id)
{
    return 0;
}

uint32_t gpr_log_diag_packet(uint32_t log_code, gpr_packet_t* packet, uint32_t packet_size)
{
    return AR_EOK;
}
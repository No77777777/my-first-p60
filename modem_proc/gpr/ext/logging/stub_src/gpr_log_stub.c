 /**
 * \file gpr_log.c
 * \brief
 *    This file contains logger implementation
 *
 *
 * \copyright
 *  Copyright (c) 2020-2021 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
// clang-format off
/*
$Header: //commercial/MPSS.HA.1.1/Main/modem_proc/gpr/ext/logging/stub_src/gpr_log_stub.c#1 $
*/
// clang-format on

/******************************************************************************
 * Includes                                                                    *
 *****************************************************************************/

#include "ar_osal_error.h"

#include "gpr_api_i.h"
#include "gpr_log.h"



/*****************************************************************************
* Defines                                                                   *
****************************************************************************/

/*****************************************************************************
 * Structure definitions                                                     *
 ****************************************************************************/

/*****************************************************************************
 * Variables                                                                 *
 ****************************************************************************/

/*****************************************************************************
 * Function Definitions                                                      *
 ****************************************************************************/

GPR_INTERNAL uint32_t gpr_log_init(void)
{
  return AR_EOK;
}

GPR_INTERNAL uint32_t gpr_log_deinit(void)
{
  return AR_EOK;
}

GPR_INTERNAL int32_t gpr_log_packet (gpr_packet_t* packet)
{
  return AR_EOK;
}

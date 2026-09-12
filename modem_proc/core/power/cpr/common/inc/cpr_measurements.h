/**
 * @file:  cpr_measurements.h
 * @brief:
 *
 * Copyright (c) 2015 by Qualcomm Technologies Incorporated. All Rights Reserved.
 *
 * $DateTime: 2019/04/24 00:03:26 $
 * $Header: //components/rel/core.mpss/10.0/power/cpr/common/inc/cpr_measurements.h#1 $
 * $Change: 18983508 $
 */
#ifndef CPR_MEASUREMENTS_H
#define	CPR_MEASUREMENTS_H

#include "cpr_types.h"
#include "cpr_cfg.h"

void cpr_measurements_settle(cpr_rail* rail, cpr_voltage_mode mode);
void cpr_measurements_thermal_update(cpr_thermal_region region);
void cpr_measurements_aging(cpr_rail* rail);

#endif

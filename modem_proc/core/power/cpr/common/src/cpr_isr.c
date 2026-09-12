/**
 * @file:  cpr_isr.c
 *
 * Copyright (c) 2015 by Qualcomm Technologies Incorporated. All Rights Reserved.
 *
 * $DateTime: 2019/04/24 00:03:26 $
 * $Header: //components/rel/core.mpss/10.0/power/cpr/common/src/cpr_isr.c#1 $
 * $Change: 18983508 $
 */
#include "cpr_cfg.h"
#include "cpr_utils.h"
#include "cpr_rail.h"

uint32 cpr_isr_get_interrupt(cpr_domain_id railId)
{
    cpr_rail* rail = cpr_utils_get_rail( railId );
    return rail->interruptId;
}

void cpr_isr_process(cpr_domain_id railId)
{
    cpr_rail* rail = cpr_utils_get_rail( railId );
    cpr_rail_isr( rail );
}

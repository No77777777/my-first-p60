/**
 * @file:  cpr_image_init.c
 *
 * Copyright (c) 2015 by Qualcomm Technologies Incorporated. All Rights Reserved.
 *
 * $DateTime: 2019/04/25 18:28:25 $
 * $Header: //components/rel/core.mpss/10.0/power/cpr/image/mpss/target/kamorta/cpr_image_target_init.c#1 $
 * $Change: 19007111 $
 */
#include <string.h>
#include "cpr_logs.h"
#include "cpr_data.h"
#include "cpr_rail.h"
#include "cpr_smem.h"
#include "cpr_image.h"
#include "cpr_measurements.h"
#include "cpr_cfg.h"
#include "cpr_hal.h"
#include "cpr_utils.h"
#include "cpr_image_target_init.h"
#include "cpr_image_target.h"
#include "cprf_hwio.h"
#include "sleep_lpr.h"

//******************************************************************************
// Local Helper Functions
//******************************************************************************


static cpr_cfg_funcs* init_open_loop(cpr_rail* rail)
{
    uint8 rev = 0;
    cpr_rail_state *railState = NULL;

    CPR_LOG_TRACE( "Initializing open loop on %s", rail->name );

    railState = cpr_utils_get_rail_state(rail->id);
    rev = railState ? railState->railCPRRev: 0;

    cpr_rail_set_initial_voltages( rail, (rev == 0), false );


    return NULL;
}

static cpr_cfg_funcs* init_settle(cpr_rail* rail)
{
    CPR_LOG_TRACE( "Settling on %s", rail->name );

    for(int i = 0; i < rail->settleModesCount; i++) {
        cpr_measurements_settle( rail, rail->settleModes[i] );
    }

    CPR_LOG_TRACE( "--- Done with settling ---" );

    return NULL;
}

static cpr_cfg_funcs* init_hw_open_loop(cpr_rail* rail)
{
    CPR_LOG_TRACE( "--- CPR HW Open Loop ---" );

    cpr_rail_init_rail_hw( rail, true );

    init_open_loop(rail);

    cpr_rail_write_all_voltages( rail );

    //sleepLPR_registerNodelessLPR("CPR", SLEEP_ALL_LPRMS);

    return NULL;
}

static cpr_cfg_funcs* enable_hw_open_loop(cpr_rail* rail)
{
    CPR_LOG_TRACE( "--- Enable CPR HW Open Loop ---" );

    cpr_rail_write_cprf_settings( rail, MSS_CPRF_ID );

    return NULL;
}

static cpr_cfg_funcs* init_hw_closed_loop(cpr_rail* rail)
{
    CPR_LOG_TRACE( "--- CPR HW Closed Loop ---" );

    cpr_rail_init_rail_hw( rail, true );

    cpr_measurements_aging( rail );

    init_open_loop(rail);

    CPR_LOG_TRACE( "Configuring closed loop on %s", rail->name );

    cpr_rail_calculate_target_quotients( rail, &cpr_info.railStates[rail->railIdx] );

    cpr_rail_write_all_voltages( rail );

    cpr_rail_write_all_target_quotients( rail );

    //sleepLPR_registerNodelessLPR("CPR", SLEEP_ALL_LPRMS);

    return NULL;
}

static cpr_cfg_funcs* enable_hw_closed_loop(cpr_rail* rail)
{
    CPR_LOG_TRACE( "--- Enable CPR HW Closed Loop ---" );

    cpr_rail_write_cprf_settings( rail, MSS_CPRF_ID );

    cpr_rail_enable_hw_closed_loop( rail );

    return NULL;
}

static cpr_cfg_funcs* init_closed_loop(cpr_rail* rail)
{
    cpr_rail_init_rail_hw( rail, true );

    cpr_measurements_aging( rail );

    init_open_loop(rail);

    CPR_LOG_TRACE( "Configuring closed loop on %s", rail->name );

    cpr_rail_calculate_target_quotients( rail, &cpr_info.railStates[rail->railIdx] );

    init_settle(rail);

    return NULL;
}

static cpr_cfg_funcs* enable_closed_loop(cpr_rail* rail)
{
    CPR_LOG_TRACE( "Enabling closed loop on %s", rail->name );

    cpr_rail_register_isr( rail );

    return NULL;
}


//******************************************************************************
// Default Enablement Structures
//******************************************************************************

static cpr_cfg_funcs init_funcs[CPR_INIT_FUNCS_COUNT] =
{
[CPR_INIT_NONE]           = {.cMode = CPR_CONTROL_NONE,           .init = NULL,             .enable = NULL},
[CPR_INIT_OPEN_LOOP]      = {.cMode = CPR_CONTROL_OPEN_LOOP,      .init = init_open_loop,   .enable = NULL},
[CPR_INIT_SW_CLOSED_LOOP] = {.cMode = CPR_CONTROL_SW_CLOSED_LOOP, .init = init_closed_loop, .enable = enable_closed_loop},
[CPR_INIT_HW_OPEN_LOOP]   = {.cMode = CPR_CONTROL_HW_OPEN_LOOP,   .init = init_hw_open_loop,   .enable = enable_hw_open_loop},
[CPR_INIT_HW_CLOSED_LOOP] = {.cMode = CPR_CONTROL_HW_CLOSED_LOOP, .init = init_hw_closed_loop, .enable = enable_hw_closed_loop},
};

cpr_cfg_funcs* cpr_image_target_get_init_func(cpr_init_funcs_id func_id)
{
    CPR_ASSERT(func_id < CPR_INIT_FUNCS_COUNT);

    return &init_funcs[func_id];
}


/*-------------------------------------------------------------------*
 *                     CPR Hardned Image Fuctions                    *
 *-------------------------------------------------------------------*/
//ToDo: Rename these functions to a more appropriate name.
void cpr_image_enable_closed_loop(void)
{
    cpr_rail *rail = cpr_utils_get_rail(CPR_RAIL_MSS);

    if (!cpr_utils_is_closed_loop_mode(CPR_RAIL_MSS))
        return;

         if (!cpr_hal_cpr_ctrl_interaction_enabled())
        {
            cpr_hal_enable_cpr_ctrl_interaction(&rail->hal, true);

            cpr_hal_enable_cprf_interface(&rail->hal, false);
            cpr_hal_enable_cprf_interface(&rail->hal, true);
        }


    return;
}

void cpr_image_disable_closed_loop(void)
{
    return;
}

void cpr_image_reset_controller(void)
{
    cpr_rail *rail = NULL;

    if(!cpr_info.fullInit)
    {
        CPR_LOG_INFO( "CPRF/CPRC not yet initialized, nothing to reset." );
        return;
    }

    if (!cpr_utils_is_closed_loop_mode(CPR_RAIL_MSS))
        return;

    if (cpr_hal_cpr_ctrl_interaction_enabled())
    {
        rail = cpr_utils_get_rail(CPR_RAIL_MSS);
        cpr_hal_enable_cprf_interface(&rail->hal, false);
        cpr_hal_enable_cprf_interface(&rail->hal, true);
    }


    return;
}

uint32 cpr_image_get_hw_ol_count(uint32 mode)
{
    return cpr_hal_get_hw_ol_count(mode);
}
/**
 * @file:  cpr_image_target_init.h
 * @brief:
 *
 * Copyright (c) 2017 by Qualcomm Technologies Incorporated. All Rights Reserved.
 *
 * $DateTime: 2019/04/25 18:28:25 $
 * $Header: //components/rel/core.mpss/10.0/power/cpr/image/mpss/target/kamorta/cpr_image_target_init.h#1 $
 * $Change: 19007111 $
 */
#ifndef CPR_IMAGE_TARGET_INIT_H
#define CPR_IMAGE_TARGET_INIT_H

//******************************************************************************
// Default Enablement Structures
//******************************************************************************

struct cpr_cfg_funcs;

typedef enum
{
    CPR_INIT_NONE, /* used to disable CPR */
    CPR_INIT_OPEN_LOOP,
    CPR_INIT_SW_CLOSED_LOOP,
    CPR_INIT_HW_OPEN_LOOP,
    CPR_INIT_HW_CLOSED_LOOP,

    CPR_INIT_FUNCS_COUNT
} cpr_init_funcs_id;

struct cpr_cfg_funcs* cpr_image_target_get_init_func(cpr_init_funcs_id func_id);

void cpr_image_enable_closed_loop(void);
void cpr_image_disable_closed_loop(void);
void cpr_image_reset_controller(void);
uint32 cpr_image_get_hw_ol_count(uint32 mode);

#endif


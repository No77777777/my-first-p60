#ifndef PM_PWR_ALG__H
#define PM_PWR_ALG__H

/*! \file pm_pwr_alg.h
 *  \n
 *  \brief   
 *  \details  
 *  \n &copy; Copyright 2011-2015 QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/pwr/src/pm_pwr_alg.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/06/12   hw      Rearchitecturing module driver to peripheral driver
========================================================================== */

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pmapp_npa.h"
#include "bare_metal_info.h"
#include "pm_resources_and_types.h"
#include "hw_module_type.h"
#include "pm_comm.h"
/*===========================================================================

                      TYPES AND STRUCTURES 

===========================================================================*/
#define PM_VOLT_INVALID_RANGE 255


typedef struct
{
    unsigned    start:16;
    unsigned    end:16;
}pm_pwr_range_info_type;

typedef struct
{
    uint32    RangeMin;
    uint32    RangeMax;
    uint32    VStep;            
}pm_pwr_volt_info_type;

/* These fields are mapped to registers that are needed by power algorithms */
typedef struct
{
    pm_register_address_type    base_address;
    pm_register_address_type    peripheral_offset;
    //                                                                    COMMON/SMPS LDO     PMXX24_SMPS    PM855_BOB   PM855_VS
    pm_register_address_type    STATUS;            // STATUS                  0x08 
    pm_register_address_type    STATUS4;           // STATUS                  0x0B
    pm_register_address_type    ULS_VSET_LB;       // ULS_VSET_LB             0x39                            0x68
    pm_register_address_type    STEPPER_VS_CTL;    //STEPPER_VS_CTL           0x3C
    pm_register_address_type    VOLTAGE_CTRL1;     // VOLTAGE_CTL1            0x40 
    pm_register_address_type    VOLTAGE_CTRL2;     // VOLTAGE_CTL2            0x41 
    pm_register_address_type    VSET_VALID_LB;     // VSET_VALID_LB           0x42
    pm_register_address_type    VSET_VALID_UB;     // VSET_VALID_UB           0x43
    pm_register_address_type    MODE_CTL;          // MODE_CTL                0x45 
    pm_register_address_type    EN_CTL;            // EN_CTL                  0x46 
    pm_register_address_type    PD_CTL;            // PD_CTL                  0xA0                             0x48 
    pm_register_address_type    FOLLOW_HWEN;       // FOLLOW_HWEN             0x47                             0x00
    pm_register_address_type    CLK_DIV;           // CLK_DIV(S_CTRL, S_FREQ) 0x50    0x00                     0x00
    pm_register_address_type    QM_MODE;           // QM_MODE(BCUCK_CMN)      0x51    0x00           0x51      0x00
    pm_register_address_type    QM_PS_ENTRY;       // QM_PS_ENTRY(S_PS)       0x63    0x00           0x63      0x00
    pm_register_address_type    QM_PS_EXIT;        // QM_PS_EXIT (S_CTRL)     0x6B    0x00           0x6B      0x00
    pm_register_address_type    ECM_EN_CTL;        // ECM_EN_CTL              0x00    0x00           0x80      0x00
    pm_register_address_type    OCP;               // OCP                     0x88                             0x00
    pm_register_address_type    VOUT_EXT_CTRL1_LB; //VOUT_EXT_CTRL1_LB(forBOB)0x00                             0x4A
    pm_register_address_type    PMIC4_ULS_VSET_LB; // PMIC4 ULS_VSET_LB       0x68                             0x00
    
}pm_pwr_register_info_type;



typedef struct
{
    pm_pwr_volt_info_type         pwr_vset;
    pm_register_address_type      periph_base_address;
    uint8                         periph_type;
    boolean                       is_periph_stepper;
    boolean                       is_periph_unified_reg;
    boolean                       pmic4_flag;
    boolean                       is_ret_present;
}pm_pwr_specific_info_type;


typedef struct
{
    pm_pwr_register_info_type     *pwr_reg_table;
    uint32                        num_of_peripherals;
    pm_pwr_specific_info_type     *pwr_specific_info;
}pm_pwr_data_type;

/*===========================================================================

                     FUNCTION DECLARATIONS 

===========================================================================*/

pm_err_flag_type pm_pwr_is_vreg_ready_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *vreg_ok);

pm_err_flag_type pm_pwr_sw_mode_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_sw_mode_type *sw_mode);

pm_err_flag_type pm_pwr_sw_enable_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type on_off, boolean wait_for_settle);

pm_err_flag_type pm_pwr_sw_enable_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off);

pm_err_flag_type pm_pwr_sw_enable_vreg_ok_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *vreg_ok);

pm_err_flag_type pm_pwr_pin_ctrl_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_on_off_type *on_off, uint8 *select_pin);

pm_err_flag_type pm_pwr_volt_level_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_volt_level_type *volt_level);

//pm_err_flag_type pm_pwr_soft_reset_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *status);

//pm_err_flag_type pm_pwr_soft_reset_exit_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index);

pm_err_flag_type pm_pwr_volt_level_stepper_done_status_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, boolean *stepper_done);

pm_err_flag_type pm_pwr_volt_calculate_vset_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8  peripheral_index, pm_volt_level_type  volt_level, uint32* vset);

pm_err_flag_type pm_pwr_volt_level_alg(pm_pwr_data_type *pwr_data, pm_comm_info_type *comm_ptr, uint8 peripheral_index, pm_volt_level_type  volt_level, boolean wait_for_settle);

#endif /* PM_PWR_ALG__H */

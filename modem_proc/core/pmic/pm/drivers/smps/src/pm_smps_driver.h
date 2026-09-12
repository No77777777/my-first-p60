#ifndef PM_SMPS_DRIVER__H
#define PM_SMPS_DRIVER__H

/*! \file pm_smps_driver.h
 *  \n
 *  \brief   
 *  \details  
 *  \n &copy; Copyright 2012-2013 QUALCOMM Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/smps/src/pm_smps_driver.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/06/12   hw      Rearchitecturing module driver to peripheral driver
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_pwr_alg.h"
#include "pm_comm.h"
#include "pm_smps_p.h"
/*===========================================================================

                     SMPS TYPES AND STRUCTURES 

===========================================================================*/
typedef enum
{
    SMPS_PERPH_SUBTYPE_0X,
    SMPS_PERPH_SUBTYPE_1X,
    SMPS_PERPH_SUBTYPE_2X,
    SMPS_PERPH_SUBTYPE_3X,
    SMPS_PERPH_SUBTYPE_INVALID,
}pm_smps_perph_subtype_type;

typedef struct
{
   pm_register_data_type smps_ilimit_reg_data_lut; 
   uint16 smps_ilimit_2x_lut; 
   uint16 smps_ilimit_3x_lut; 
}SmpsILimDS;

typedef struct
{
    pm_comm_info_type *comm_ptr;
    pm_pwr_data_type  pm_pwr_data;
} pm_smps_data_type;

/** SMPS mode type */
typedef enum
{
    PM_ILIM_SMPS_PWM_MODE,  /**< SMPS PWM mode. */ 
    PM_ILIM_SMPS_AUTO_MODE, /**< SMPS auto mode. For SMPS mode transition
                                  between PFM and PWM. */
    PM_ILIM_SMPS_MODE_INVALID
}pm_smps_ilim_mode_type;

typedef enum 
{
   PM_CLK_TCXO,
   PM_CLK_RC,
   PM_CLK_SOURCE_INVALID
}pm_clk_src_type;

/** SMPS switching frequency. This enumeration assumes an input clock
 * frequency of 19.2 MHz and is 5 bits long. \n
 * Clock frequency = (input clock freq) / ((CLK_PREDIV+1)(CLK_DIV + 1)).
 */
typedef enum 
{
    PM_CLK_19p2_MHz     = 0,   /**< Clock frequency = 19.2 MHz. */
    PM_CLK_9p6_MHz      = 1,   /**< Clock frequency = 9.6 MHz. */
    PM_CLK_6p4_MHz      = 2,   /**< Clock frequency = 6.2 MHz. */
    PM_CLK_4p8_MHz      = 3,   /**< Clock frequency = 4.8 MHz. */
    PM_CLK_3p84_MHz     = 4,   /**< Clock frequency = 3.84 MHz. */
    PM_CLK_3p2_MHz      = 5,   /**< Clock frequency = 3.2 MHz. */
    PM_CLK_2p74_MHz     = 6,   /**< Clock frequency = 2.74 MHz. */
    PM_CLK_2p4_MHz      = 7,   /**< Clock frequency = 2.4 MHz. */
    PM_CLK_2p13_MHz     = 8,   /**< Clock frequency = 2.13 MHz. */
    PM_CLK_1p92_MHz     = 9,   /**< Clock frequency = 1.92 MHz. */
    PM_CLK_1p75_MHz     = 10,  /**< Clock frequency = 1.75 MHz. */
    PM_CLK_1p6_MHz      = 11,  /**< Clock frequency = 1.6 MHz. */
    PM_CLK_1p48_MHz     = 12,  /**< Clock frequency = 1.48 MHz. */
    PM_CLK_1p37_MHz     = 13,  /**< Clock frequency = 1.37 MHz. */
    PM_CLK_1p28_MHz     = 14,  /**< Clock frequency = 1.28 MHz. */
    PM_CLK_1p2_MHz      = 15,  /**< Clock frequency = 1.2 MHz. */
    PM_CLK_1p13_MHz     = 16,  /**< Clock frequency = 1.13 MHz. */
    PM_CLK_1p07_MHz     = 17,  /**< Clock frequency = 1.07 MHz. */
    PM_CLK_1p01_MHz     = 18,  /**< Clock frequency = 1.01 MHz. */
    PM_CLK_960_KHz      = 19,  /**< Clock frequency = 960 kHz. */
    PM_CLK_914_KHz      = 20,  /**< Clock frequency = 914 kHz. */
    PM_CLK_873_KHz      = 21,  /**< Clock frequency = 873 kHz. */
    PM_CLK_835_KHz      = 22,  /**< Clock frequency = 835 kHz. */
    PM_CLK_800_KHz      = 23,  /**< Clock frequency = 800 kHz. */
    PM_CLK_768_KHz      = 24,  /**< Clock frequency = 768 kHz. */
    PM_CLK_738_KHz      = 25,  /**< Clock frequency = 738 kHz. */
    PM_CLK_711_KHz      = 26,  /**< Clock frequency = 711 kHz. */
    PM_CLK_686_KHz      = 27,  /**< Clock frequency = 686 kHz. */
    PM_CLK_662_KHz      = 28,  /**< Clock frequency = 662 kHz. */
    PM_CLK_640_KHz      = 29,  /**< Clock frequency = 640 kHz. */
    PM_CLK_619_KHz      = 30,  /**< Clock frequency = 619 kHz. */
    PM_CLK_600_KHz      = 31,  /**< Clock frequency = 600 kHz. */
    PM_SWITCHING_FREQ_INVALID,
    PM_SWITCHING_FREQ_FREQ_NONE
}pm_smps_switching_freq_type;


/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/
void pm_smps_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_smps_data_type* pm_smps_get_data(uint8 pmic_index);
uint8 pm_smps_get_num_peripherals(uint8 pmic_index);


/*===========================================================================

                        API PROTOTYPE

===========================================================================*/

/** Switches between HPM, LPM, and  modes of a regulator.

    @param[in] pmic_chip              Primary PMIC: 0, Secondary PMIC: 1.

    @param[in] smps_peripheral_index SMPS peripheral index. Starts at 
                                     0 (for the first SMPS peripheral).
                                     See @xnameref{hdr:smpsPeripheralIndex}.

    @param[in] sw_mode                Selects the mode of a regulator.
 
    @return 
    Error flag type -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_smps_sw_mode
(uint8 pmic_chip, uint8 smps_peripheral_index, pm_sw_mode_type sw_mode);

/** 
 * @name pm_smps_pin_ctrl_status 
 *  
 * @brief This function returns the pin control enable 
 *  status and also the selected pins being followed by the
 *  selected power rail enable.
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  perph_index:
 *                Starts from 0 (for first SMPS peripheral)
 * @param[out] select_pin Variable returned to the caller with 
 *                        selected pins being followed by the
 *                        power rail.
 *
 * @return  pm_err_flag_type
 *          PM_ERR_FLAG__SUCCESS = SUCCESS else ERROR.
 *
 */
pm_err_flag_type pm_smps_pin_ctrl_status
(uint8 pmic_chip, uint8 perph_index, uint8 *select_pin);

#if 0
/**
 * @name 
 *     pm_smps_inductor_ilim
 *
 * 
 *  Selects the current limit for the inductor of a selected SMPS.
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] perph_index SMPS peripheral index.
 *                Starts from 0 (for the first SMPS peripheral)
 * @param[in] ilim_level Current limit level. Range -- 3500 mA to 600 mA: \n
 *                 2x SMPS subtype current limit range -- 600 mA to 2700 mA. \n
 *                 3x SMPS subtype current limit range -- 700 mA to 3500 mA
 * @param[in] smps_mode SMPS mode. Valid values: \n
 *                      PM_ILIM_SMPS_PWM_MODE -- Operating in PWM mode. \n
 *                      PM_ILIM_SMPS_AUTO_MODE -- Whenever there is a mode
 *                      transition between PFM and PWM mode in Auto mode
 *                      operation.
 *  
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 * 
 * <b>Example </b> \n
 * @code
 *   pm_smps_inductor_ilim(0,PM_SMPS_1,2500,PM_ILIM_SMPS_PWM_MODE); @endcode
 */
pm_err_flag_type pm_smps_inductor_ilim 
(uint8 pmic_chip, uint8 perph_index, uint16 ilim_level, pm_smps_ilim_mode_type smps_mode);

/**
 * @name 
 *     pm_smps_inductor_ilim_status
 *
 * 
 *  Gets the current limit for the inductor of a selected SMPS by reading the
 *  SPMI register.
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[in] perph_index SMPS peripheral index.
 *                Starts from 0 (for the first SMPS peripheral)
 * @param[out] ilim_level Current limit level. Range -- 3500 mA to 600 mA: \n 
 *                        2x SMPS current limit range -- 600 mA to 2700 mA. \n
 *                        3x SMPS current limit range -- 700 mA to 3500 mA.
 * @param[in] smps_mode SMPS mode. Valid values: \n
 *                      PM_ILIM_SMPS_PWM_MODE -- Operating in PWM mode. \n
 *                      PM_ILIM_SMPS_AUTO_MODE -- Whenever there is a mode
 *                      transition between PFM and PWM mode in Auto mode
 *                      operation.
 * 
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_smps_inductor_ilim_status
( uint8 pmic_chip, uint8 perph_index, uint16* ilim_level, pm_smps_ilim_mode_type smps_mode );

#endif

/** 
 * @name pm_smps_volt_calculate_vset
 *  
 * @brief This function calculates the PMIC register value for
 *        a supplied voltage
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  perph_index:
 *                Starts from 0 (for first SMPS peripheral)
 * @param[in]  volt_level:
 *                Voltage to convert to PMIC register value
 * @param[out] vset:
 *                PMIC register value for supplied voltage
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Under error conditions
 *          like invalid param entered.
 *          PM_ERR_FLAG__SUCCESS = SUCCESS.
 *
 */
pm_err_flag_type pm_smps_volt_calculate_vset
(uint8 pmic_chip, uint8 perph_index, pm_volt_level_type  volt_level, uint32* vset);

/** 
 * @name pm_smps_volt_level_stepper_done_status
 *  
 * @brief This function returns the stepper done status for
 *        the selected power rail
 * 
 * @param[in]  pmic_chip. Primary: 0. Secondary: 1
 * @param[in]  perph_index:
 *                Starts from 0 (for first SMPS peripheral)
 * @param[out]  stepper_done:
 *                Value of stepper done register
 *
 * @return  pm_err_flag_type 
 *          PM_ERR_FLAG__FEATURE_NOT_SUPPORTED = Under error conditions
 *          like invalid param entered.
 *          PM_ERR_FLAG__SUCCESS = SUCCESS.
 *
 */
pm_err_flag_type pm_smps_volt_level_stepper_done_status
(uint8 pmic_chip, uint8 periph_index, boolean *stepper_done);


pm_err_flag_type pm_smps_sw_enable(uint8 pmic_chip, uint8 smps_peripheral_index, pm_on_off_type on_off, boolean wait_for_settle);

pm_err_flag_type pm_smps_volt_level(uint8 pmic_chip, uint8 smps_peripheral_index, pm_volt_level_type volt_level, boolean wait_for_settle);


#endif // PM_SMPS_DRIVER__H

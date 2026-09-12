#ifndef PM_VADC_DRIVER__H 
#define PM_VADC_DRIVER__H

/*! \file
 *  \n
 *  \brief  pm_vadc_driver.h 
 *  \details  This file contains functions prototypes and variable/type/constant
 *  declarations for supporting GPIO pin services for the Qualcomm
 *  PMIC chip set.
 *  \n &copy; Copyright 2010-2013 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/pm/drivers/vadc/src/pm_vadc_driver.h#1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
01/25/16    rl      New File Created
========================================================================== */
/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_target_information.h"

/*===========================================================================

                     TYPE DEFINITIONS 

===========================================================================*/
typedef struct pm_vadc_register_info_type
{
    pm_register_address_type    base_address;          //0x3800
    pm_register_address_type    int_latched_clr;       //0x14
    pm_register_address_type    int_en_set;            //0x15
    pm_register_address_type    int_en_clr;            //0x16
    pm_register_address_type    vcomp_hi_thr;          //0x48   
    pm_register_address_type    vcomp_low2_thr;        //0x49
    pm_register_address_type    vcomp_low1_thr;        //0x4A    
}pm_vadc_register_info_type;

typedef struct pm_vadc_btm_limits
{
    uint32 min_mv;
    uint32 max_mv;
    uint32 step_mv;
}pm_vadc_btm_limits;

typedef struct pm_vadc_data_type
{
    pm_comm_info_type        *comm_ptr;
    pm_vadc_register_info_type *vadc_register;
    pm_vadc_btm_limits *limits;
}pm_vadc_data_type;

typedef enum
{
    PM_VADC_BTM_LOW1_THR=0,       /**< LOW1 Threshold. */
    PM_VADC_BTM_LOW2_THR=1,       /**< LOW2 Threshold. */ 
    PM_VADC_BTM_HI1_THR=2,        /**< HI Threshold. */
    PM_VADC_BTM_THR_INVALID
}pm_vadc_btm_thr_type;

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/
void pm_vadc_driver_init(pm_comm_info_type *comm_ptr, peripheral_info_type *peripheral_info, uint8 pmic_index);

pm_vadc_data_type* pm_vadc_get_data(uint8 pmic_index);

/*===========================================================================

                        API PROTOTYPE

===========================================================================*/
/**
*  Configures threshold voltage, based on threshold type provided.
*
* @param[in] pmic_index Each PMIC device in the systems is enumerated
*                        starting with 0.
* @param[in] thr_type  Threshold type to be configured.
*                        See #pm_vadc_btm_thr_type.
* @param[in] volt_mV   Voltage in mV.
*                        
* @return
*  SUCCESS or Error -- See #pm_err_flag_type.
*
*  <b>Example </b> \n
*   Configure Low 1 voltage threshold on PMIC 0 at 2250 mV:
* @code
*  errFlag = pm_vadc_set_thr_voltage(0, 
*                                    PM_VADC_BTM_LOW1_THR, 
*                                    2250); @endcode
*/
pm_err_flag_type pm_vadc_set_thr_voltage(uint8 pmic_index, 
                                         pm_vadc_btm_thr_type thr_type, 
                                         uint32 volt_mV);


/**
*  Enables/Disables interrupt for threshold level.
*
* @param[in] pmic_index Each PMIC device in the systems is enumerated
*                        starting with 0.
* @param[in] thr_type  Threshold type to be configured.
*                        See #pm_vadc_btm_thr_type.
* @param[in] enabled   Enable or disable interrupt
*                 
* @return
*  SUCCESS or Error -- See #pm_err_flag_type.
*
*  <b>Example </b> \n
*   Enable Low 1 threshold interrupt on PMIC 0:
* @code
*  errFlag = pm_vadc_irq_enable(0, 
*                               PM_VADC_BTM_LOW1_THR, 
*                               TRUE); @endcode
*/                                              
pm_err_flag_type pm_vadc_irq_enable(uint8 pmic_index, 
                                    pm_vadc_btm_thr_type thr_type, 
                                    boolean enabled);

/**
*  Clears latched interrupt for threshold level.
*
* @param[in] pmic_index Each PMIC device in the systems is enumerated
*                        starting with 0.
* @param[in] thr_type  Threshold type to be configured.
*                        See #pm_vadc_btm_thr_type.
*                 
* @return
*  SUCCESS or Error -- See #pm_err_flag_type.
*
*  <b>Example </b> \n
*   Clear Low 1 threshold interrupt latch on PMIC 0:
* @code
*  errFlag = pm_vadc_irq_clear(0, 
*                              PM_BCL_COMP_LOW1_THR); @endcode
*/                                           
pm_err_flag_type pm_vadc_irq_clear(uint8 pmic_index, 
                                   pm_vadc_btm_thr_type thr_type);

/**
*  Registers ISR for threshold level.
*
* @param[in] pmic_index Each PMIC device in the systems is enumerated
*                        starting with 0.
* @param[in] thr_type  Threshold type to be configured.
*                        See #pm_vadc_btm_thr_type.
* @param[in] isr_ptr   Pointer for call-back function
*                 
* @return
*  SUCCESS or Error -- See #pm_err_flag_type.
*
*  <b>Example </b> \n
*   Register function: void* cb_fn(void *pArg, uint32 mask) for LOW1 Thr
* @code
*  errFlag = pm_vadc_register_isr(0, 
*                                 PM_BCL_COMP_LOW1_THR, 
*                                 &cb_fn); @endcode
*/                                                                                                                                                        
pm_err_flag_type pm_vadc_register_isr(uint8 pmic_index, 
                                      pm_vadc_btm_thr_type thr_type, 
                                      void *isr_ptr);

#endif //PM_VADC_DRIVER__H

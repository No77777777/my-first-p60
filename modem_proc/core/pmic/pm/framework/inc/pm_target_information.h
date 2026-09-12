#ifndef __PM_TARGET_INFORMATION_H__
#define __PM_TARGET_INFORMATION_H__

/*! \file pm_target_information.h 
*  \n
*  \brief
*  \n  
*  \n &copy; Copyright 2018 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

                EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module over time.

$Header: //components/rel/core.mpss/10.0/pmic/pm/framework/inc/pm_target_information.h#1 $ 

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_resources_and_types.h"
#include "pm_dal_prop_ids.h"
#include "pm_comm.h"
#include "pm_npa_device.h"
#include "pm_config_target.h"

/*===========================================================================

                     ENUMERATIONS 

===========================================================================*/

/* ENUM defines sequence in which MVC data is arranged*/
typedef enum{
	PM_MSS_VOLT_LEVEL,
	PM_MSS_PBS_SEQ, 
	PM_MSS_RES_1, 
	PM_MSS_RES_2,
	PM_MSS_CFG_COUNT
}pm_mss_reg_cfg_type; 


/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/

/*! \brief This function initializes the target/PMIC related information.
 *  \param[in] None
 *
 *  \return None.
 *
 *  <b>Dependencies</b>
 *  \li DAL framework.
 */
void 
pm_target_info_init(void);

/*! \brief This function returns the target specific props information for the parameter specified property id.
 *
 *  \param[in] prop_id: Property id.
 *
 *  \return A pointer to the information.
 *
 *  <b>Dependencies</b>
 *  \li pm_init() .
 */
void* 
pm_target_info_get_prop_info(const char* prop_id);

/*! \brief This function reads the parameter specified peripheral information.
 *  \param[in] comm_ptr The Comm Channel.
 *
 *  \param[in] peripheral_info A pointer to where the peripheral information is going to be saved.
 *
 *  \return Error flag.
 *
 *  <b>Dependencies</b>
 *  \li pm_processor_init_comms() .
 */
pm_err_flag_type 
pm_target_info_read_periph_rev( pm_comm_info_type *comm_ptr,
                                peripheral_info_type *peripheral_info );

/*! \brief This function checks if pmk is present and changes the pmic index for xo and rtc drivers accordingly.
 *  \param[in] *pmic_index, pointer to pmic index value 
 *  
 *  \return True if PMK is Present, False if not present.
 */
boolean 
pm_target_info_pmk_present(uint8 *pmic_index);

/*! \brief Checks if default config was selected at init
 *  
 *  \return True if default config was selected at target init
 */
boolean 
pm_target_info_is_default_cfg(void);

/*! \brief Gets pam data pointer
 *  
 *  \return Pam data pointer
 */
pm_npa_pam_data_type*
pm_target_info_pam_data(void);

/*! \brief Gets Target data pointer
 *  
 *  \return Target data pointer
 */
pm_target_data_type*
pm_target_info_target_data(void);

#endif //__PM_TARGET_INFORMATION_H__



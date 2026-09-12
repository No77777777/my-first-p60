#ifndef __PM_TARGET_H__
#define __PM_TARGET_H__

/*! \file pm_target_information.h 
*  \n
*  \brief
*  \n  
*  \n &copy; Copyright 2012-2013 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

                EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module over time.

$Header: //components/rel/core.mpss/10.0/pmic/pm/target/inc/pm_target.h#2 $ 

===========================================================================*/
#include "pm_err_flags.h"
#include "pm_npa_device.h"
#include "pm_config_target.h"
/*===========================================================================

                     FUNCTION DECLARATION 

===========================================================================*/

/*! \brief This function returns correct dal xml string to get config info.
 *  \param[in] None
 *
 *  \return pmic props string
 *  \return default_flag: If default config is provided or not
 */
char*
pm_target_get_pmic_props (boolean *default_flag);

/*! \brief Returns pointer to correct pam data based on platform
 *  \param[in] None
 *
 *  \return Pointer to pam data
 */
pm_npa_pam_data_type*
pm_target_get_pam_data(void);

/*! \brief Returns pointer to correct target data based on platform
 *  \param[in] None
 *
 *  \return Pointer to target data
 */
pm_target_data_type*
pm_target_get_target_data(void);

/*! \brief Checks if PMK is present based on platform
 *  \param[in] None
 *
 *  \return is_pmk_present : True if pmk is present
 *                           False if not present
 *  \return pmk_index : PMK index if pmk is present.
 */
pm_err_flag_type
pm_target_get_pmk_info(boolean *is_pmk_present, uint8 *pmk_index);

/*! \brief NPA node to vote for clock buffer hold
 *  \param[in] None
 *
 *  \return None

 */
void
pm_target_clk_buff_hold_init (void);

/*! \brief Target specific pre init functinalities 
 *  \param[in] None
 *
 *  \return None

 */
void
pm_target_pre_init(void);

#endif //__PM_TARGET_H__



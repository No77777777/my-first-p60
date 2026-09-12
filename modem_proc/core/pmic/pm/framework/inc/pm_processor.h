#ifndef PM_PROCESSOR_H
#define PM_PROCESSOR_H

/*! \file
 *  
 *  \brief  modem_proc_processor.h ----This file contains Processor class definition
 *  \details Processor class is what starts the the PMIC system initialization
 *           This class is entirely generated from the PCG spreadsheet
 *  
 *    PMIC code generation Version: 1.0.0.0
 *    This file contains code for Target specific settings and modes.
 *  
 *    &copy; Copyright 2010 Qualcomm Technologies Incorporated, All Rights Reserved
 */

/*===========================================================================

                EDIT HISTORY FOR MODULE

  This document is created by a code generator, therefore this section will
  not contain comments describing changes made to the module over time.

$Header: //components/rel/core.mpss/10.0/pmic/pm/framework/inc/pm_processor.h#1 $ 

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES 

===========================================================================*/

#include "icomm.h"
#include "pm_resources_and_types.h"

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/

/*! \brief This function initializes the processor related information, such as Comm.
 *  \param[in] None
 *
 *  \return None.
 *
 *  <b>Dependencies</b>
 *  \li DAL framework.
 */
void pm_processor_init(void);

/*! \brief This function resolves the parameter specified peripheral information.
 *  \param[in] comm_ptr The Comm Channel.
 *
 *  \param[in] peripheral_info A pointer to where the peripheral information is going to be saved.
 *
 *  \return Error flag.
 *
 *  <b>Dependencies</b>
 *  \li pm_processor_init() .
 */
pm_err_flag_type pm_processor_resolve_peripheral_version(pmiC_IComm *comm_ptr, peripheral_info_type *peripheral_info);

/*! \brief This function creates the comm channels.
 *  \param[in] comm_info The Comm info.
 *
 *  \param[in] current_comm The current comm object.
 *
 *  \param[in] new_comm The new comm object to be created.
 *
 *  \return Error flag.
 *
 *  <b>Dependencies</b>
 *  \li pm_processor_init() .
 */
pm_err_flag_type pm_processor_create_comm_channel(pmiC_CommInfo *comm_info, pmiC_IComm **current_comm, pmiC_IComm **new_comm);

#endif /* PM_PROCESSOR_H */


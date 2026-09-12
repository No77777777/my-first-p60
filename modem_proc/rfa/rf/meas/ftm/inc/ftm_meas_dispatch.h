/*----------------------------------------------------------------------------*/
/*!

  @brief
  This is the header file for the embedded FTM WCDMA Commands 

Copyright (c) 2009 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfa.mpss/4.11/rf/meas/ftm/inc/ftm_meas_dispatch.h#1 $ $DateTime: 2021/03/09 05:11:02 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/14/15   nv      Added changes for Half scripts and NULL2X scripts
03/31/15   nv      First version of CRAT FTM GUI
09/15/09   jps     Created Initial version
*/
/*----------------------------------------------------------------------------*/

#ifndef FTM_MEAS_DISPATCH_H
#define FTM_MEAS_DISPATCH_H

#include "rfa_variation.h"
#include "comdef.h"
#include "ftmicap.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftmdiag.h"

#include "rfmeas_types.h"


typedef PACKED struct 
{
  uint16 error_code;
} ftm_meas_extended_result_type;

/*----------------------------------------------------------------------------*/
/*!
   @brief
   This function handles MEAS FTM requests to run tests and other primitives
   by dispatching the appropriate functions.

   @param
   cmd_ptr - 

   @retval
   Response packet
*/
ftm_rsp_pkt_type ftm_meas_extended_dispatch(ftm_meas_extended_pkt_type *cmd_ptr);

/*!
   @brief
   This function prints all the parameters sent by FTM QRCT GUI in FTM MEAS GENERIC Format.

   @param
   test_ptr - parameters in FTM MEAS GENERIC Format.

   @retval
   None
*/
void ftm_meas_debug_params(rfm_meas_generic_params_type *test_ptr);

/*!
   @brief
   This function copies the parameters sent by FTM QRCT GUI into FTM MEAS GENERIC structure.

   @param
   test_param - The pointer to structure where parameters are copied.

   @param
   buff_ptr - The pointer to structure received from diag.

   @retval
   SUCCESS if we dont encounter any unexpected parameters while copying, else return FAILURE.
*/
boolean ftm_meas_irat_copy_params(rfm_meas_generic_params_type *test_param,uint16 *buff_ptr);

/*!
   @brief
   This function converts parameters stored in halfscript structure format to generic structure format .

   @param
   test_param_half_script_ptr - The pointer to structure where parameters are stored in halfscript format.

   @param
   test_param_generic_ptr      - The pointer to structure where parameters are converted from halfscript format and stored in generic format.

   @retval
   None
*/
void ftm_meas_convert_halfscript_to_generic_structure(rfm_meas_half_scripts_params_type *test_param_ptr_half_script,rfm_meas_generic_params_type *test_param_generic_ptr);

/*!
   @brief
   This function converts parameters stored in NULL2X structure format to generic structure format .

   @param
   test_param_null2x_script_ptr - The pointer to structure where parameters are stored in NULL2X format.

   @param
   test_param_generic_ptr         - The pointer to structure where parameters are converted from halfscript format and stored in generic format.

   @retval
   None
*/
void ftm_meas_convert_null2xscript_to_generic_structure(rfm_meas_null2x_params_type *test_param_null2x_script_ptr,rfm_meas_generic_params_type *test_param_generic_ptr);

/*!
   @brief
   This function prints all the parameters sent by FTM QRCT GUI for NULL2X script.

   @param
   test_param - The pointer to structure where parameters are copied.

   @param
   buff_ptr - The pointer to structure received from diag.

   @retval
   SUCCESS if we dont encounter any unexpected parameters while copying, else return FAILURE.
*/
boolean ftm_meas_null2x_copy_params(rfm_meas_null2x_params_type *test_param,uint16 *buff_ptr);

/*!
   @brief
   This function prints all the parameters sent by FTM QRCT GUI for Half script.

   @param
   test_param - The pointer to structure where parameters are copied.

   @param
   buff_ptr - The pointer to structure received from diag.

   @retval
   SUCCESS if we dont encounter any unexpected parameters while copying, else return FAILURE.
*/
boolean ftm_meas_half_script_copy_params(rfm_meas_half_scripts_params_type *test_param,uint16 *buff_ptr);

#endif  /* FEATURE_FACTORY_TESTMODE */
#endif  /* FTM_MEAS_DISPATCH_H */

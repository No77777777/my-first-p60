#ifndef _PS_DPM_CLATI
#define _PS_DPM_CLATI
/*===========================================================================

                     PS_DPM_CLATI . H
DESCRIPTION
Header file describing global DPM config definitions.

Copyright (c) 2014 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/interface/dpm/src/ps_dpm_clati.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
=========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "ps_dpmi.h"
#include "ps_dpm_clat.h"

#ifdef FEATURE_DATA_PS_464XLAT

/**
  @brief  This API sets the global context info for CLAT and registers
          with hardware

  @param[in] clat_global_cntxt_info   CLAT global context info


  @return  0     SUCCESS
  @return  -1    FAILIRE

  @code
  @endcode
*/
int ps_dpm_clati_reg_global_cntxt_cmd_hdlr
(
  ps_dpm_clat_global_cntxt_info_type  * clat_global_cntxt_info
);

/**
  @brief  This API sets the CLAT context info per pdn and registers
          with hardware

  @param[in] clat_reg_info_ptr   CLAT pdn context info


  @return  0     SUCCESS
  @return  -1    FAILIRE

  @code
  @endcode
*/
int ps_dpm_clati_reg_pdn_cntxt_cmd_hdlr
(
  ps_dpm_clat_reg_pdn_cntxt_info_type   * clat_reg_info_ptr
);

/**
  @brief  This API reregisters the CLAT context info per pdn and registers
          with hardware

  @param[in] clat_rereg_info_ptr   CLAT pdn context info


  @return  0     SUCCESS
  @return  -1    FAILIRE

  @code
  @endcode
*/
int ps_dpm_clati_rereg_pdn_cntxt_cmd_hdlr
(
  ps_dpm_clat_reg_pdn_cntxt_info_type   * clat_rereg_info_ptr
);

/**
  @brief  This API deregisters the CLAT context info per pdn and
          with hardware

  @param[in] clat_dereg_info_ptr   CLAT dereg info ptr

  @return
  @code
  @endcode
*/
void ps_dpm_clati_dereg_pdn_cntxt_cmd_hdlr
(
  ps_dpm_clat_dereg_pdn_cntxt_info_type   * clat_dereg_info_ptr
);
#endif /* FEATURE_DATA_PS_464XLAT */
#endif /* _PS_DPM_CLATI */

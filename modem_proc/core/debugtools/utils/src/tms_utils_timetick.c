/**
@file tms_utils_timetick.c
@brief This file contains the API for the TMS Utilities API 0.x
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2016 - 2018 QUALCOMM Technologies Incorporated.
All rights reserved.
QUALCOMM Confidential and Proprietary.
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/utils/src/tms_utils_timetick.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

#include "tms_utils.h"
#include "tms_utils_msg.h"
#include "tms_dll_api.h"
#include "qurt_sclk.h"

/**
API, Get System Timer value for the TMS Timetick handle
@param[in]     uint64*        Pointer to timetick type used to get timer value
@return        void           Best effort function, no failure action
*/
DLL_API_GLOBAL void tms_get_timetick(uint64* ticktype_p)
{
  /* Best effort, no failure action */
  *ticktype_p = qurt_sysclock_get_hw_ticks();
}

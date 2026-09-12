/**
@file tms_utils.c
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
Copyright (c) 2018 QUALCOMM Technologies Incorporated.
All rights reserved.
QUALCOMM Confidential and Proprietary.
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/utils/src/tms_utils.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

#include "tms_utils.h"
#include "tms_utils_msg.h"
#include "tms_dll_api.h"

/** =====================================================================
 * Function:
 *     tms_utils_init
 *
 * Description:
 *     Utilities initialization function in user/root pd.
 *     Initialize TMS ULogs, and Diag F3.
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void tms_utils_init(void)
{
  tms_utils_msg_init();
}
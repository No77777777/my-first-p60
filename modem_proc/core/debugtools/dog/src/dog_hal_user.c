/** vi: tw=128 ts=3 sw=3 et
@file dog_hal_user.c
@brief This file contains the API details for the Dog Services, API 2.0
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2017-2019 by Qualcomm Technologies Incorporated.  All Rights Reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/dog/src/dog_hal_user.c#2 $
$DateTime: 2019/09/20 04:39:24 $
$Change: 20640401 $
$Author: pwbldsvc $
===============================================================================*/

#include "dog_common_internal.h"

volatile dog_disable_state_e dog_disable_state = ALL_ENABLE;

/** =====================================================================
* Empty wrapper functions
* =====================================================================  */

void dog_force_kick(void)
{
}

void dog_force_bite(void)
{
}

void dog_force_nmi(void)
{
}

void dog_force_nmi_multithreaded(void)
{
}

void dog_force_bark(void)
{
}